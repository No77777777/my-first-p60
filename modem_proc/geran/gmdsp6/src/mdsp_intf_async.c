/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  L A Y E R  1  - M D S P   I N T E R F A C E
                 MDSP Interface Module - Asynchronous Messages

GENERAL DESCRIPTION

    This module handles communications to and from the mDSP via MSGR.


==========================================================================

EXTERNALIZED FUNCTIONS
  mdsp_async_intf_init
  mdsp_async_intf_teardown
  mdsp_async_intf_send_blocking_cmd

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2012-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmdsp6/src/mdsp_intf_async.c#3 $

when         who     what, where, why
----------   ------- -----------------------------------------------------
24/07/20     sal     CR2736787 Added G2X cleanup done handler to issue rf_meas_exit immediately on cleanup confirmation.
27/01/20     rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
11/10/17     akm     CR2124554 Add debug code to print MSGR return value
11/08/17    rc        CR2138578  ML logging - GL1 Changes
15/08/15     cja     CR885161 Handle W2G prebuild before rf meas exit of previous gap
15/04/15     pa      CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
29/04/15     npt     CR804870 RF timeline error recovery mechanism
07/04/15     og      CR814507. Further stack reduction in the GSM L1 ISR.
06/03/15     am      CR802884 Correct WLAN_CXM_POLICY_CMD for different subs
27/01/15     am      CR646584 WLAN coex
07/01/15     smd     CR774443 Debug crash when GFW returns "sleep not ready" for more than 4 frames.
10/10/14     pg      CR729410 make DRX exit mode MSGR RSP handling non-blocking
07/10/14     pg      CR730132 F3 cleanup
24/07/14     nk      CR699150 W2G IRAT timeline improvement (remove F3s from GL1)
06/08/14     df      CR536059 Allow Asynchronous sleep for all idle activities
28/02/14     pjr     CR624105 Ensure that the size of mdsp_async_intf_msgr_umid_list is properly calculated
19/02/14     pjr     CR600612 Dedicated WTR Hop initial code drop
05/06/14     pm      CR676039. Removing un-necessary debug filling log and recovered F3.
18/04/14     dv      CR651891 Do not interact with GFW if its not awake.
31/03/14     ss      CR630382 Assert if GFW does not respond to Message in two GSM frames
23/04/14     jj      CR 653796 function  cannot called inside ASSERT
07/03/14     og      CR628402. mutex protect the mdsp async intf commands.
20/02/14     jk      CR620513:Update the GL1_GFW_MSGR_CMD_RSP_TABLE_LEN as new command-response pair is added
07/02/14     nk      CR576899 GBTA feature is not working during DSDA to DSDS fallback
22/01/14     jk      CR606383 GL1 changes to support RxDiversity
19/12/13     jk      CR503359 - GL1 changes to support Rx diversity
22/11/13     jj      CR 580519 log  hw fn send over  asynch command
21/06/13     cs      Major Triton TSTS Syncup
10/05/13     cs      TSTS Updates
14-04-13     pjr     CR475908 dual data spaced firmware queue
12-02-13     pjr     CR451837 modified mdsp_log_qdsp6_async_cmd to log correct
                     async command
12-11-09     tjw     Initial version refactored from l1_task.c and mdsp_intf.c
==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"

#include "mdsp_async_intf.h"
#include "gl1_hw.h" /* mdsp_async_intf_msgr_client */
#include "gl1_hw_g.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_cmd.h"
#include "mdsp_debug.h"
#include "rex.h"

#include "geran_msgs.h"

#include "gl1_mutex.h"

#include "gfw_sw_intf.h"
#include "fws.h"

#include "msgr.h"
#include "geran_msg.h"  /* GERAN MSGR FW UMIDs */

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

/*==========================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

==========================================================================*/

#define NO_RESPONSE 0
typedef struct
{
  msgr_umid_type msg;
  uint32         msg_length;
  msgr_umid_type rsp;
  uint32         rsp_length;
} gl1_gfw_msgr_cmd_rsp_table_t;

#define GL1_GFW_MSGR_CMD_RSP_TABLE_LEN 13

#define GL1_NULL_MSGR_ENTRY  { 0UL, 0, 0UL, NO_RESPONSE }

const gl1_gfw_msgr_cmd_rsp_table_t gl1_gfw_msgr_cmd_rsp_table[NUM_GERAN_DATA_SPACES][GL1_GFW_MSGR_CMD_RSP_TABLE_LEN] =
{
  {
    {GERAN_FW_ASYNC_CMD,           sizeof(GfwHostAsyncCmd),
     GERAN_FW_ASYNC_CMD,           NO_RESPONSE},
    {GERAN_FW_APP_MODE_CONFIG_CMD, sizeof(GfwHostAppModeConfigCmd),
     GERAN_FW_APP_MODE_CONFIG_RSP, sizeof(GfwHostAppModeConfigRsp)},
    {GERAN_FW_RFM_ENTER_MODE_CMD,  sizeof(GfwHostRfmEnterModeCmd),
     GERAN_FW_RFM_ENTER_MODE_RSP,  sizeof(GfwHostRfmEnterModeRsp)},
    {GERAN_FW_SLEEP_CMD,           sizeof(GfwHostSleepCmd),
     GERAN_FW_SLEEP_RSP,           sizeof(GfwHostSleepRsp)},
    {GERAN_FW_SLEEP_READY_CMD,     sizeof(GfwHostAsyncSleepReadyCmd),
     GERAN_FW_SLEEP_READY_CMD,     NO_RESPONSE},
    {GERAN_FW_RFM_SET_TX_BAND_CMD, sizeof(GfwHostRfmSetTxBandCmd),
     GERAN_FW_RFM_SET_TX_BAND_RSP, sizeof(GfwHostRfmSetTxBandRsp)},
    {GERAN_FW_ASYNC_EXIT_CMD,      sizeof(GfwHostAsyncExitCmd),
     GERAN_FW_ASYNC_EXIT_CMD,      NO_RESPONSE}

#ifdef FEATURE_GSM_RX_DIVERSITY
    , { GERAN_FW_RFM_EXIT_MODE_CMD,  sizeof(GfwHostRfmExitModeCmd),
        GERAN_FW_RFM_EXIT_MODE_RSP,  NO_RESPONSE}
#else
    , GL1_NULL_MSGR_ENTRY
#endif

#ifdef FEATURE_GSM_WTR_HOP
    , {GERAN_FW_WTR_CFG_CMD,         sizeof(GfwHostWtrConfigCmd),
       GERAN_FW_WTR_CFG_RSP,         sizeof(GfwHostWtrConfigRsp)}
#else
   , GL1_NULL_MSGR_ENTRY
#endif

    , { GERAN_FW_DEVICE_MODE_CMD,     sizeof(GfwHostAsyncDeviceModeCmd),
        GERAN_FW_DEVICE_MODE_CMD,     NO_RESPONSE}

#ifdef FEATURE_WLAN_COEX_SW_CXM
    , { GERAN_FW_WLAN_CXM_POLICY_CMD,  sizeof(GfwWlanCxmConfigCmd),
        GERAN_FW_WLAN_CXM_POLICY_CMD,  NO_RESPONSE}
#else
    , GL1_NULL_MSGR_ENTRY
#endif
#ifdef FEATURE_GSM_RX_TX_SPLIT
    , { GERAN_FW_TX_EXIT_CMD,  sizeof(GfwHostTxExitCmd),
        GERAN_FW_TX_EXIT_RSP,  sizeof(GfwHostTxExitRsp)}
#else
    , GL1_NULL_MSGR_ENTRY
#endif
    , { GERAN_FW_CLEANUP_DONE_CMD,  sizeof(GfwHostCleanupDoneCmd),
        GERAN_FW_CLEANUP_DONE_CMD,  NO_RESPONSE}

  }

#if NUM_GERAN_DATA_SPACES >= 2
  , {
#if defined (FEATURE_DUAL_SIM)
    {GERAN_FW2_ASYNC_CMD,           sizeof(GfwHostAsyncCmd),
     GERAN_FW2_ASYNC_CMD,           NO_RESPONSE},
    {GERAN_FW2_APP_MODE_CONFIG_CMD, sizeof(GfwHostAppModeConfigCmd),
     GERAN_FW2_APP_MODE_CONFIG_RSP, sizeof(GfwHostAppModeConfigRsp)},
    {GERAN_FW2_RFM_ENTER_MODE_CMD,  sizeof(GfwHostRfmEnterModeCmd),
     GERAN_FW2_RFM_ENTER_MODE_RSP,  sizeof(GfwHostRfmEnterModeRsp)},
    {GERAN_FW2_SLEEP_CMD,           sizeof(GfwHostSleepCmd),
     GERAN_FW2_SLEEP_RSP,           sizeof(GfwHostSleepRsp)},
    {GERAN_FW2_SLEEP_READY_CMD,     sizeof(GfwHostAsyncSleepReadyCmd),
     GERAN_FW2_SLEEP_READY_CMD,     NO_RESPONSE},
    {GERAN_FW2_RFM_SET_TX_BAND_CMD, sizeof(GfwHostRfmSetTxBandCmd),
     GERAN_FW2_RFM_SET_TX_BAND_RSP, sizeof(GfwHostRfmSetTxBandRsp)},
    {GERAN_FW2_ASYNC_EXIT_CMD,      sizeof(GfwHostAsyncExitCmd),
     GERAN_FW2_ASYNC_EXIT_CMD,      NO_RESPONSE}

#ifdef FEATURE_GSM_RX_DIVERSITY
    , { GERAN_FW2_RFM_EXIT_MODE_CMD,  sizeof(GfwHostRfmExitModeCmd),
        GERAN_FW2_RFM_EXIT_MODE_RSP,  NO_RESPONSE}
#else
    , GL1_NULL_MSGR_ENTRY
#endif

#ifdef FEATURE_GSM_WTR_HOP
   , { GERAN_FW2_WTR_CFG_CMD,         sizeof(GfwHostWtrConfigCmd),
       GERAN_FW2_WTR_CFG_RSP,         sizeof(GfwHostWtrConfigRsp)}
#else
    , GL1_NULL_MSGR_ENTRY
#endif

    , {GERAN_FW2_DEVICE_MODE_CMD,     sizeof(GfwHostAsyncDeviceModeCmd),
       GERAN_FW2_DEVICE_MODE_CMD,     NO_RESPONSE}

#ifdef FEATURE_WLAN_COEX_SW_CXM
    , { GERAN_FW2_WLAN_CXM_POLICY_CMD,  sizeof(GfwWlanCxmConfigCmd),
        GERAN_FW2_WLAN_CXM_POLICY_CMD,  NO_RESPONSE}
#else
    , GL1_NULL_MSGR_ENTRY
#endif
#ifdef FEATURE_GSM_RX_TX_SPLIT
    , { GERAN_FW2_TX_EXIT_CMD,  sizeof(GfwHostTxExitCmd),
        GERAN_FW2_TX_EXIT_RSP,  sizeof(GfwHostTxExitRsp)}
#else
    , GL1_NULL_MSGR_ENTRY
#endif
    , { GERAN_FW2_CLEANUP_DONE_CMD,     sizeof(GfwHostCleanupDoneCmd),
        GERAN_FW2_CLEANUP_DONE_CMD,     NO_RESPONSE}

#else
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY
#endif /* FEATURE_DUAL_SIM */
  }
#if NUM_GERAN_DATA_SPACES == 3
  , {
#if defined (FEATURE_TRIPLE_SIM)
    {GERAN_FW3_ASYNC_CMD,           sizeof(GfwHostAsyncCmd),
     GERAN_FW3_ASYNC_CMD,           NO_RESPONSE},
    {GERAN_FW3_APP_MODE_CONFIG_CMD, sizeof(GfwHostAppModeConfigCmd),
     GERAN_FW3_APP_MODE_CONFIG_RSP, sizeof(GfwHostAppModeConfigRsp)},
    {GERAN_FW3_RFM_ENTER_MODE_CMD,  sizeof(GfwHostRfmEnterModeCmd),
     GERAN_FW3_RFM_ENTER_MODE_RSP,  sizeof(GfwHostRfmEnterModeRsp)},
    {GERAN_FW3_SLEEP_CMD,           sizeof(GfwHostSleepCmd),
     GERAN_FW3_SLEEP_RSP,           sizeof(GfwHostSleepRsp)},
    {GERAN_FW3_SLEEP_READY_CMD,     sizeof(GfwHostAsyncSleepReadyCmd),
     GERAN_FW3_SLEEP_READY_CMD,     NO_RESPONSE},
    {GERAN_FW3_RFM_SET_TX_BAND_CMD, sizeof(GfwHostRfmSetTxBandCmd),
     GERAN_FW3_RFM_SET_TX_BAND_RSP, sizeof(GfwHostRfmSetTxBandRsp)},
    {GERAN_FW3_ASYNC_EXIT_CMD,      sizeof(GfwHostAsyncExitCmd),
     GERAN_FW3_ASYNC_EXIT_CMD,      NO_RESPONSE}

#ifdef FEATURE_GSM_RX_DIVERSITY
    , { GERAN_FW3_RFM_EXIT_MODE_CMD,  sizeof(GfwHostRfmExitModeCmd),
        GERAN_FW3_RFM_EXIT_MODE_RSP,  NO_RESPONSE}
#else
    , GL1_NULL_MSGR_ENTRY
#endif

#ifdef FEATURE_GSM_WTR_HOP
   , { GERAN_FW3_WTR_CFG_CMD,         sizeof(GfwHostWtrConfigCmd),
       GERAN_FW3_WTR_CFG_RSP,         sizeof(GfwHostWtrConfigRsp)}
#else
    , GL1_NULL_MSGR_ENTRY
#endif

    , GL1_NULL_MSGR_ENTRY

#ifdef FEATURE_WLAN_COEX_SW_CXM
    , { GERAN_FW3_WLAN_CXM_POLICY_CMD,  sizeof(GfwWlanCxmConfigCmd),
        GERAN_FW3_WLAN_CXM_POLICY_CMD,  NO_RESPONSE}
#else
    , GL1_NULL_MSGR_ENTRY
#endif
#ifdef FEATURE_GSM_RX_TX_SPLIT
    , { GERAN_FW3_TX_EXIT_CMD,  sizeof(GfwHostTxExitCmd),
        GERAN_FW3_TX_EXIT_RSP,  sizeof(GfwHostTxExitRsp)}
#else
    , GL1_NULL_MSGR_ENTRY
#endif
    , { GERAN_FW3_CLEANUP_DONE_CMD,      sizeof(GfwHostCleanupDoneCmd),
        GERAN_FW3_CLEANUP_DONE_CMD,      NO_RESPONSE}
#else
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY,
    GL1_NULL_MSGR_ENTRY
#endif /* FEATURE_TRIPLE_SIM */
  }
#endif /* NUM_GERAN_DATA_SPACES == 3 */
#endif /* NUM_GERAN_DATA_SPACES >= 2 */
};

#define MDSP_ASYNC_INTF_PRIORITY  1
#define MDSP_ASYNC_INTF_DEPTH     3


#ifdef FEATURE_GSM_WTR_HOP
#define WTR_HOP_NUM_RSP 1
#else
#define WTR_HOP_NUM_RSP 0
#endif

#ifdef FEATURE_GSM_RX_TX_SPLIT
#define RX_TX_SPLIT_NUM_RSP 1
#else
#define RX_TX_SPLIT_NUM_RSP 0
#endif

#define MDSP_ASYNC_INTF_MSGR_DIAG_CNT (5 + WTR_HOP_NUM_RSP + RX_TX_SPLIT_NUM_RSP)

static msgr_client_t  mdsp_async_intf_msgr_client[NUM_GERAN_DATA_SPACES];
static msgr_id_t      mdsp_async_intf_msgr_msg_id[NUM_GERAN_DATA_SPACES];
static msgr_umid_type mdsp_async_intf_msgr_umid_list[NUM_GERAN_DATA_SPACES][MDSP_ASYNC_INTF_MSGR_DIAG_CNT] =
{
 {
   GERAN_FW_APP_MODE_CONFIG_RSP,
   GERAN_FW_RFM_ENTER_MODE_RSP,
   GERAN_FW_SLEEP_RSP,
   GERAN_FW_ASYNC_IMMEDIATE_RSP,
   GERAN_FW_RFM_SET_TX_BAND_RSP
#ifdef FEATURE_GSM_WTR_HOP
  ,GERAN_FW_WTR_CFG_RSP
#endif
#ifdef FEATURE_GSM_RX_TX_SPLIT
 ,GERAN_FW_TX_EXIT_RSP
#endif
 }
#if ( defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) )
 , {
     GERAN_FW2_APP_MODE_CONFIG_RSP,
     GERAN_FW2_RFM_ENTER_MODE_RSP,
     GERAN_FW2_SLEEP_RSP,
     GERAN_FW2_ASYNC_IMMEDIATE_RSP,
     GERAN_FW2_RFM_SET_TX_BAND_RSP
#ifdef FEATURE_GSM_WTR_HOP
    ,GERAN_FW2_WTR_CFG_RSP
#endif
#ifdef FEATURE_GSM_RX_TX_SPLIT
 ,GERAN_FW2_TX_EXIT_RSP
#endif
   }

#if defined ( FEATURE_TRIPLE_SIM )
 , {
     GERAN_FW3_APP_MODE_CONFIG_RSP,
     GERAN_FW3_RFM_ENTER_MODE_RSP,
     GERAN_FW3_SLEEP_RSP,
     GERAN_FW3_ASYNC_IMMEDIATE_RSP,
     GERAN_FW3_RFM_SET_TX_BAND_RSP
#ifdef FEATURE_GSM_WTR_HOP
    ,GERAN_FW3_WTR_CFG_RSP
#endif
#ifdef FEATURE_GSM_RX_TX_SPLIT
 ,GERAN_FW3_TX_EXIT_RSP
#endif
   }
#endif /* FEATURE_TRIPLE_SIM */
#endif /* ( FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM ) */
  };

#define MSDP_MSGR_QUEUE_SIZE 6
#define GFW_MSG_ROUTER_SIG  0x0001
typedef struct
{
  q_link_type     link;
  gl1_gfw_msgr_u  msg;

} gfw_msgr_t;
gfw_msgr_t          mdsp_msgr[NUM_GERAN_DATA_SPACES][ MSDP_MSGR_QUEUE_SIZE ];

static q_type    mdsp_message_router_q[NUM_GERAN_DATA_SPACES];
static q_type    mdsp_message_router_free_q[NUM_GERAN_DATA_SPACES];
static boolean   initialised[NUM_GERAN_DATA_SPACES] =
                   { INITIAL_VALUE( FALSE ) };

/*===========================================================================

                                FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION  mdsp_async_intf_init

DESCRIPTION
  Set up MSGR message to GFW Task.

===========================================================================*/
void mdsp_async_intf_init(gas_id_t gas_id)
{
  uint32  i = 0;
  errno_enum_type  msgr_error;

  if (initialised[gas_id] == FALSE)
  {
    initialised[gas_id] = TRUE;

    (void) q_init( &mdsp_message_router_q[gas_id] );
    (void) q_init( &mdsp_message_router_free_q[gas_id] );

    i = 0;

    while( i < MSDP_MSGR_QUEUE_SIZE )
    {
      msgr_hdr_struct_type*  tmp_ptr   = (msgr_hdr_struct_type *) &(mdsp_msgr[gas_id][i].msg.hdr);
      q_link_type*           link_item_ptr = q_link( tmp_ptr, &(mdsp_msgr[gas_id][i].link) );

      q_put( &mdsp_message_router_free_q[gas_id], link_item_ptr );
      i++;
    }
    msgr_error =  msgr_client_create(&mdsp_async_intf_msgr_client[gas_id]) ;

    if( msgr_error != E_SUCCESS)
    {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_0_G(" msgr_client_create Failure");
    }

    msgr_error =  msgr_client_add_mq(
           "GERAN MDSP ASYNC INTF Mailbox",
           &mdsp_async_intf_msgr_client[gas_id],
           (uint8)MDSP_ASYNC_INTF_PRIORITY,
           (uint32)MDSP_ASYNC_INTF_DEPTH,
           (uint32)sizeof(gl1_gfw_msgr_u),
           &mdsp_async_intf_msgr_msg_id[gas_id]);
    if( msgr_error != E_SUCCESS)
    {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_0_G(" msgr_client_add_mq Failure");
    }

    for (i = 0; i < MDSP_ASYNC_INTF_MSGR_DIAG_CNT; i++)
    {
      msgr_error = msgr_register(MSGR_GERAN_FW,
                   &mdsp_async_intf_msgr_client[gas_id],
                   mdsp_async_intf_msgr_msg_id[gas_id],
                   mdsp_async_intf_msgr_umid_list[gas_id][i]) ;
    if( msgr_error != E_SUCCESS)
    {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_0_G(" msgr_register Failure");
    }
    }
  }
}

/*===========================================================================

FUNCTION  mdsp_async_intf_teardown

DESCRIPTION
  Tear down MSGR message to GFW Task.

===========================================================================*/
void mdsp_async_intf_teardown(gas_id_t gas_id)
{
  uint32 i;
  errno_enum_type  msgr_error;

  if (initialised[gas_id] == TRUE)
  {
    initialised[gas_id] = FALSE;
    for (i = 0; i < MDSP_ASYNC_INTF_MSGR_DIAG_CNT; i++)
    {
      msgr_error = msgr_deregister(MSGR_GERAN_FW,
                   &mdsp_async_intf_msgr_client[gas_id],
                   mdsp_async_intf_msgr_umid_list[gas_id][i]) ;

    if( msgr_error != E_SUCCESS)
    {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_0_G(" msgr_deregister Failure");
    }

    }
    msgr_error = msgr_client_delete(&mdsp_async_intf_msgr_client[gas_id]);
    if( msgr_error != E_SUCCESS)
    {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_0_G(" msgr_client_delete Failure");
    }
  }
}

#define MSGR_WAIT_DELAY_MAX 9230    /*  Max period to wait for GFW to respond 2 GSM frames = 9230 micro seconds*/
#define MSGR_WAIT_DELAY_STEP  20    /* Check every 20 uS if GFW responded */

/*===========================================================================

FUNCTION  mdsp_async_intf_send_blocking_cmd

DESCRIPTION
  Send MSGR message to GFW Task.
  Use MSGR blocking wait to receive rsp for this immediate cmd from GFW before continuing

===========================================================================*/
void mdsp_async_intf_send_blocking_cmd( msgr_umid_type msg,
                                        void * payload,
                                        void * response,
                                        gas_id_t gas_id )
{
  errno_enum_type  result;
  uint32           bytes_recvd;
  uint32           msg_num;

  gl1_gfw_msgr_u*         response_ptr;

  uint32 msgr_wait_period = 0;

  /* pre-set to GFW_SLEEP, as we are only interested in if the real value matches GFW_WAKE */
  GfwSleepCmdType SleepCmdType = GFW_SLEEP;
  static msgr_umid_type previous_msg[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };

  mdsp_async_intf_mtx_lock( gas_id );

  /* do not typecast the payload to SleepCmd unless the message is GERAN_FW_SLEEP_CMD, else
     the sleep command type will just be a random value */
  if(msg == GERAN_FW_SLEEP_CMD)
  {
     SleepCmdType = ((gl1_gfw_msgr_u *)payload)->gfwHostSleepCmd.cmd;
     MSG_GERAN_MED_1_G("Sleep cmd %d",SleepCmdType);
  }

  /* If the mdsp is asleep then only a GFW_WAKE command can be sent */
  if (mdsp_awake(gas_id) || (SleepCmdType == GFW_WAKE ) )
  {
     if (response == NULL)
     {
       static gl1_gfw_msgr_u   temp_response[3];

       memset( &temp_response[gas_id], 0, sizeof(gl1_gfw_msgr_u) );

       response_ptr = &temp_response[gas_id];
     }
     else
     {
       response_ptr = (gl1_gfw_msgr_u *)response;
     }
     for (msg_num = 0; msg_num < GL1_GFW_MSGR_CMD_RSP_TABLE_LEN; msg_num++)
     {
       if (msg == gl1_gfw_msgr_cmd_rsp_table[0][msg_num].msg)
       {
         break;
       }
     }
     if (msg_num == GL1_GFW_MSGR_CMD_RSP_TABLE_LEN)
     {
       MSG_GERAN_ERROR_1_G("Invalid GFW UMID %d", msg);

       mdsp_async_intf_mtx_unlock( gas_id );
       return;
     }

     /* initialise msgr header */
     msgr_init_hdr(&((gl1_gfw_msgr_u *)payload)->hdr,MSGR_GERAN_GL1,gl1_gfw_msgr_cmd_rsp_table[gas_id][msg_num].msg);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER	 
     #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
     /* send msgr message to GFW */
     result = msgr_send(&((gl1_gfw_msgr_u *)payload)->hdr, gl1_gfw_msgr_cmd_rsp_table[0][msg_num].msg_length);

     if (result != E_SUCCESS)
     {
#ifdef FEATURE_QSH_MDUMP
       QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
       ERR_GERAN_FATAL_1_G("msgr_send failed, return value : %d", result);
     }

     if ( ( msg != GERAN_FW_SLEEP_CMD ) ||
          ( previous_msg[gas_id] != GERAN_FW_SLEEP_CMD ) ||
          ( SleepCmdType == GFW_WAKE )
        )
     {
       /* We will log all commands sent to FW except consectutive sleep command.In case of GFW being stuck in
          "sleep not ready" state, GL1 would send sleep command in every 50us and crash after 4 frames. The command
          buffer would be flooded.
       */
       mdsp_log_qdsp6_async_cmd(gl1_gfw_msgr_cmd_rsp_table[gas_id][msg_num].msg, gas_id ,((GfwHostAsyncCmd *)payload)->hwFrameNo);
     }
     previous_msg[gas_id] = msg;

     if (gl1_gfw_msgr_cmd_rsp_table[0][msg_num].rsp_length != NO_RESPONSE)
     {

         /* Check if GFW responded with AppModeConfigRsp message */
         result = msgr_receive_nonblock(&mdsp_async_intf_msgr_client[gas_id],
                               (uint8 *)response_ptr,
                               gl1_gfw_msgr_cmd_rsp_table[0][msg_num].rsp_length,
                               &bytes_recvd);

         /*Wait 2 GSM frame for FW to respond. If no response then Assert*/
         while ( result == E_NO_DATA )
         {
           gl1_hw_delay ( MSGR_WAIT_DELAY_STEP , gas_id );
           msgr_wait_period += MSGR_WAIT_DELAY_STEP ;

           /* Check if GFW responded with AppModeConfigRsp message */
           result = msgr_receive_nonblock(&mdsp_async_intf_msgr_client[gas_id],
                             (uint8 *)response_ptr,
                             gl1_gfw_msgr_cmd_rsp_table[0][msg_num].rsp_length,
                             &bytes_recvd);

           /*  Exit while loop if no response for 4.615 ms or if response received */
           if ( msgr_wait_period >= MSGR_WAIT_DELAY_MAX )
           {
             break;
           }
         }


       if (result != E_SUCCESS)
       {
#ifdef FEATURE_QSH_MDUMP
         QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif

         ERR_GERAN_FATAL_1_G("msgr_receive_nonblock failed, return value : %d", result);
       }

       /* check that we received the expected CNF UMID */
       if(response_ptr->hdr.id != gl1_gfw_msgr_cmd_rsp_table[gas_id][msg_num].rsp)
       {
         MSG_GERAN_ERROR_2_G("Expected UMID 0x%x: Received UMID 0x%x", gl1_gfw_msgr_cmd_rsp_table[gas_id][msg_num].rsp, response_ptr->hdr.id);
       }
     }
  }
  mdsp_async_intf_mtx_unlock( gas_id );
}

/* EOF */
