/*!
  @file
  hdrsrchmsg.c

  @brief
  HDR message router and messages file.

*/

/*===========================================================================

  Copyright (c) 2009 - 2019 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchmsg.c#2 $ $DateTime: 2019/12/05 01:53:14 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ---     --------------------------------------------------------
12/04/2019   vaa     Changes to support clk plan V2.0 FR changes
01/29/2018   vko     Fixed HDR sending RF status indication to incorrect LTE SUB ID
08/24/2017   rmv     Mainline FEATURE_HDR_QTA_THRU_TRM
08/03/2017   vko     Use NO VARIANT for msgr registering of non-SUB msgs
08/03/2017   vko/vle MSGR Instance ID to Variant ID change
07/04/2016   vko     Reduce F3 msgs
05/16/2016   vko     Reduce excessive F3
05/28/2015   vlc     Added support for DO->LTE redirection in connected state.
05/04/2015   mbs     Supported QTA using TRM API
04/13/2015   vlc     Added RF API changes for D2L.
01/12/2015   wsh     Added L1/FW interface to confirm ASDiv prio ended
10/01/2014   arm     Changes to support HDR band avoidance.
09/30/2014   vlc     Moved code to set the LTE device ID to the correct spot.
09/11/2014   vlc     Changed parameters of 
                     hdrsrchmsg_d2l_send_timed_search_meas_req_msg().
06/04/2014   arm     Changes to support DO to G QTA
02/26/2014   vko     FR 17879: Extended EARFCN
11/08/2013   mbs     Moved TX FW mesg registration to tx hdrtx.c
10/11/2013   vlc     Added support for new LTE TIMED_SEARCH_MEAS_REQ API.
10/08/2013   mbs     Added support for BOLT interface changes
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
06/26/2013   wsh     Added Selective TxD feature
04/22/2013   arm     Added changes for DSDA coexistence manager.  
05/06/2013   mbs     Fixed compiler errors for BOLT build
02/26/2013   mbs     Fixed featurization for IRAT code
12/14/2012   ukl     Fix Klocwork issues
02/13/2013   vlc     Make sure mDSP is awake before sending ASP update 
                     message.
12/19/2012   smd     Added RUMI Tx support.
12/14/2012   ukl     Fix Klocwork issues
12/04/2012   mbs     Fixed compilation errors due to removal of LTE on Triton
07/31/2012   ljl     Registered RF related messages on RUMI.
07/19/2012   smd     Dime sleep updates.
06/29/2012   dsp     Registered offline message for processing.
03/26/2012   vlc     Added debug messages to provide information on search 
                     set when sending update ASP message to FW.
04/24/2012   ljl     Removed mpc cfg msg. 
03/29/2012   smd     Added DIME sleep updates.
03/20/2012   mbs     Added SON search changes
03/16/2012   arm     Resolve TCB errors for Dime. 
01/25/2012   grl     Update gRice rank for all ASP changes.
11/11/2011   arm     HDR idle and access state diversity implemetation.
11/14/2011   vlc     Added check for NULL prior to calling MSGR stop function
                     in hdrsrchmsg_stop() so ASSERT won't trip.
10/28/2011   lyl     Supported MC gRice and cset promotion.
10/19/2011   smd     Added debug code for L2DO IRAT.
10/18/2011   lyl     Supported ASP update response message.
09/26/2011   grl     Removed use of obsolete AEEstd.h
08/24/2011   vlc     Added setting for new use_script param in D2L Start up Req.
06/23/2011   vlc     Added support for RF scripting and RXLM buffers. 
04/27/2011   smd     IRAT L2DO updates for NikeL.
03/02/2011   vlc     Modified hdrsrch_cmd_type and put back memory heap changes.
01/21/2011   ljl     Declared system lost if FW requests reset.
01/06/2011   vlc     Switched messages from LTE for DO->LTE reselection to
                     low priority queue.
                     Removed references to hdrsrchmsg_rf_script_struct_type. 
12/14/2010   ljl     Displayed the return value when msgr_send() failed.
11/18/2010   vlc     Reverted memory heap changes.
11/08/2010   smd     Added PILOT_MEAS_CFG_RSP in message list.
09/10/2010   pxu     Memory Heap changes.
09/01/2010   vlc     Added support for DO->LTE pilot measurements/reselection.
06/21/2010   cnx     Fixed compiler warnings.
06/15/2010   ljl     Supported RF MSGR interface.
03/18/2010   smd     Added channel info in pilot meas rsp for failure cases.
03/10/2010   smd     Updated code of registering command queue with message 
                     router.
03/04/2010   lyl     Used hdrsrchmsg_set_rsp_waiting() to update the flag.
03/03/2010   smd     Updated code based on code review comments.
02/11/2010   ljl     Updated F3 message.
01/08/2010   ljl     Removed the legacy SRCH FW interface. 
01/07/2010   lyl     Used the FWS API.
12/07/2009   ljl     Supported FWS.
10/07/2009   ljl     Added message trace.
10/01/2009   ljl     Processed srch app cfg rsp msg.
09/24/2009   ljl     Updated for srch abort rsp.
09/14/2009   ljl     Supported srch MSGR interface.
09/02/2009   smd     Added messages for LTE to DO pilot measurement.
08/10/2009   ljl     Included AEEstd.h.
07/29/2009   etv     Included hdrfw_msg.h.
07/15/2009   etv     Moved sw module definitions to hdrmsg.h
07/06/2009   lyl     Updated minBestAspSinr interface.
06/30/2009   lyl     Updated for rxAGC unlocked mode interface.
                     Updated for generic responses process.
06/25/2009   wsh     Fixed MOB compile error on ARM compiler
05/18/2009   ljl/lyl Updated for firmware MSGR interface.
05/07/2009   etv     Removed obsolete HDR SRCH messages.
01/23/2009   pmk     Initial version
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "hdr_variation.h"
#include "comdef.h"
#include "customer.h"
#include "hdrsrchmsg.h"
#include "hdrsrchset.h"
#include "hdrsrchtask.h"
#include <amssassert.h>
#include "IxErrno.h"
#include "hdrmsg.h"
#include "hdrfw_msg.h"
#include "srchfw_msg.h"
#include "hdrdebug.h"
#include "hdrsrchcmd.h"
#include "msgr_rex.h"
#include "hdrsrchstate.h"
#include "hdrsrchmcpm.h"
#include "mmoc_msgr.h"
#include "hdrmdsp.h"
#ifdef FEATURE_HDR_TO_LTE
#include "hdrsrchltemeas.h"
#endif /* FEATURE_HDR_TO_LTE */
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
#include "hdrsrchtclte.h"
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
#include "hdrsrch.h"

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#define HDRSRCHMSG_QUEUE_SIZE  10
  /* HDR MSG Queue size */

#define HDRSRCH_IND_TYPE_BEST_ASP_CHANGE  HDR_FW_IND__BEST_ASP_CHANGE
#define HDRSRCH_IND_TYPE_SRCH_DUMP    HDR_FW_IND__SRCH_DUMP


/* Datatype for the hdrsrch message queue */
typedef struct
{
  hdrsrchmsg_s_type          q_items[ HDRSRCHMSG_QUEUE_SIZE ];  
    /* Array for the queue */

  q_type                     msg_q;  
    /* Message queue */

  q_type                     free_q;
    /* Free queue */
} hdrsrchmsg_queue_type;

/* Datatype for the hdrsrch message */
typedef struct
{
  hdrsrchmsg_queue_type      high_pri_rsp_q;  
    /* Message queue for the high priority responses */

  msgr_client_t              high_pri_rsp_client;
    /* MSGR client for high priority responses */

  hdrsrchmsg_queue_type      low_pri_rsp_ind_q;  
    /* Message queue for the low priority responses and indications */

  msgr_client_t              low_pri_rsp_ind_client;
    /* MSGR client for low priority responses and indications */

  msgr_client_t              msg_in_cmd_q_client;
    /* MSGR client for command queue */

  hdrsrchmsg_queue_type      srch_rsp_q;  
    /* Message queue for the srch dump responses */

  msgr_client_t              srch_rsp_client;
    /* MSGR client for srch dump responses */

} hdrsrchmsg_rcvd_struct_type;

#ifdef FEATURE_HDR_TO_LTE
/* Structure to save all DO->LTE messages for DO->LTE reselection 
   together. */
typedef struct {
  lte_cphy_irat_meas_init_req_s         init_req_msg;
    /* LTE IRAT Init Request message */

  lte_cphy_irat_meas_init_req_s         deinit_req_msg;
    /* LTE IRAT Deinit Request message */

  lte_cphy_irat_meas_startup_cmd_s      startup_cmd_msg;
    /* LTE IRAT Start Up Command message */

  lte_cphy_irat_meas_search_req_s       search_req_msg;
    /* LTE IRAT Measurement Search Request message */

  lte_cphy_irat_meas_meas_req_s         meas_req_msg;
    /* LTE IRAT Measurement - Measurement Request message */

  lte_cphy_irat_meas_cleanup_req_s      cleanup_req_msg;
    /* LTE IRAT Measurement Clean Up Request message */

  lte_cphy_irat_meas_abort_req_s        abort_req_msg;
    /* LTE IRAT Abort Request message */

  lte_cphy_irat_meas_timed_srch_meas_req_s timed_search_meas_req_msg;
    /* LTE IRAT Measurement - streamlined request */

} hdrsrchmsg_d2l_msg_struct_type;
#endif /* FEATURE_HDR_TO_LTE */

/*===========================================================================

                         EXTERNAL VARIABLES

===========================================================================*/

hdrsrchmsg_fw_msg_struct_type  hdrsrchmsg;
  /* Global variable to maintain HDR SRCH messages sent to firmware */

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
hdrsrchmsg_lte_msg_struct_type  hdrsrchmsg_lte;
  /* Global variable to maintain HDR SRCH messages sent to LTE for LTE
     to DO reselection */
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

#ifdef FEATURE_HDR_TO_LTE
hdrsrchmsg_d2l_msg_struct_type  hdrsrchmsg_d2l_msg;
  /* Global variable to maintain HDR SRCH messages sent to LTE for DO
     to LTE reselection */
#endif /* FEATURE_HDR_TO_LTE */

/*===========================================================================

                         LOCAL VARIABLES

===========================================================================*/


/* Local variable to maintain HDR SRCH messages received */
LOCAL hdrsrchmsg_rcvd_struct_type    hdrsrchmsg_rcvd;


/* Block of high priority rsp/ind */
msgr_umid_type hdrsrchmsg_high_pri_msgs[] =
{
#ifdef FEATURE_HDR_RUMI
  HDR_FW_RX_START_RSP,
  HDR_FW_RX_STOP_RSP,
  HDR_FW_RXAGC_TRACK_IND,
  HDR_FW_TX_START_RSP,
  HDR_FW_TX_STOP_RSP,
#endif /* FEATURE_HDR_RUMI */
  HDR_FW_SRCH_FING_ASSIGN_RSP,      /* Fing assignment rsp */
  HDR_FW_SRCH_FING_ASSIGN_SLAM_RSP, /* Fing assignment and slam rsp */
  HDR_FW_SRCH_EQ_CONFIG_RSP,        /* Eq config rsp */
  HDR_FW_ASP_UPDATE_RSP,            /* ASP update rsp */
  HDR_FW_STATE_CFG_RSP,             /* HDR State config rsp */
  CDMA_SRCH_FW_APP_CFG_RSP          /* TBD SRCH App cofig rsp */
    /* TBD Keep it so that FW can proceed, otherwise, it crashed */
#ifdef FEATURE_HDR_QTA
  ,HDR_FW_RXAGC_TRACK_QTA_IND        /* FW AGC settled rsp after QTA */
  ,HDR_FW_QTA_STATUS_QUERY_RSP       /* FW Query rsp after QTA */
#endif /* FEATURE_HDR_QTA */

#ifdef FEATURE_HDR_SELECTIVE_TXD
  ,HDR_FW_ASDIV_PRI_CHANGE_RSP
#endif /* FEATURE_HDR_SELECTIVE_TXD */
};


/* Block of low priority rsp/ind/cnf */
msgr_umid_type hdrsrchmsg_low_pri_msgs[] =
{
  HDR_FW_SRCH_FLL_ACQ_RSP,          /* Fll acq rsp */
  HDR_FW_SRCH_SLAM_RSP,             /* Slam rsp */
  HDR_FW_SRCH_FING_ASSIGN_SLAM_RSP, /* Fing assignment and slam rsp */
  HDR_FW_SRCH_RESET_DRC_FILT_RSP,   /* Reste DRC filter rsp */
  HDR_FW_SRCH_FLL_ACCUM_LD_RSP,     /* Fll accum load rsp */
  HDR_FW_BEST_ASP_CHANGE_IND,       /* Best ASP chnage ind */
  HDR_FW_PILOT_MEAS_CFG_RSP,         /* Pilot measurement config rsp */
  HDR_FW_PILOT_MEAS_STOP_STREAM_RSP, /* Pilot measurement stop stream rsp */

#ifdef FEATURE_HDR_TO_LTE
  /* The following messages are confirmation messages coming from LTE
     for DO->LTE reselection.  They need to be in a separate queue from
     the command queue.  This eliminate the race condition when DO receives
     commands (such as the Deactivate command) which require the LTE measurements
     to be aborted.  DO still needs to finish the LTE message sequence and
     receives the LTE confirmations.
 
     Without having a separate queue for the LTE confirmation messages, the pending
     Deactivate command would block DO L1 from processing the LTE confirmations,
     creating a dead lock situation.
 
     It was decided that the low priority queue is a good queue for these LTE
     confirmation messages. */

  HDRSRCH_LTE_IRAT_MEAS_D2L_INIT_CNF,     /* DO->LTE Init confirmation from LTE */
  HDRSRCH_LTE_IRAT_MEAS_D2L_DEINIT_CNF,    /* DO->LTE Deinit confirmation from LTE */
  HDRSRCH_LTE_IRAT_MEAS_D2L_SEARCH_CNF,    /* DO->LTE Search confirmation from LTE */
  HDRSRCH_LTE_IRAT_MEAS_D2L_MEAS_CNF,      /* DO->LTE Meas confirmation from LTE */
  HDRSRCH_LTE_IRAT_MEAS_D2L_CLEANUP_CNF,   /* DO->LTE Abort confirmation from LTE */
  HDRSRCH_LTE_IRAT_MEAS_D2L_ABORT_CNF,     /* DO->LTE Cleanup confirmation from LTE */
  HDRSRCH_LTE_IRAT_MEAS_D2L_TIMED_SRCH_MEAS_CNF, /* DO->LTE combined meas confirmation from LTE */
#endif /* FEATURE_HDR_TO_LTE */

  HDR_FW_RESET_REQ_IND,              /* FW requests reset due to SSBI task queue stuck */
  HDR_FW_SCC_DECODE_IND              /* FW indication that cannot decode SCC in M slots */
#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW    
  ,HDRSRCHMSG_MCS_CXM_FREQID_LIST_IND /* MCS->DO Frequncy Id list received from MCS */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */
#ifdef FEATURE_HDR_BAND_AVOIDANCE
  ,HDRSRCHMSG_MCS_BAND_AVOID_BLIST_IND /* MCS->DO Frequncy Id list received from MCS */
#endif /* FEATURE_HDR_BAND_AVOIDANCE */

};


/* Block of srch dump/abort rsp */
msgr_umid_type hdrsrchmsg_srch_rsp_msgs[] =
{
  CDMA_SRCH_FW_HDR_SRCH_RESULTS_RSP,/* HDR search result rsp */
  CDMA_SRCH_FW_HDR_SRCH_ABORT_RSP   /* HDR search abort rsp */
};

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
/* Block of LTE pilot meas messages */
msgr_umid_type hdrsrchmsg_lte_pilot_meas_msgs[] =
{
  HDR_SRCH_LTE_PILOT_MEAS_REQ,         /* pilot meas request */
  HDR_SRCH_TRAN_SYS_TIME_REQ,          /* tran sys time request */
  HDR_SRCH_LTE_PILOT_MEAS_ABORT_REQ    /* pilot meas abort request */
};
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

#define HDRSRCHMSG_HALF_SLOT_RESPONSE_TIME_US 1000

#ifdef FEATURE_HDR_IRAT_CLK_BOOST
#define HDRSRCHMSG_IRAT_CLK_BOOST_DURATION_US (22000)
#endif /* FEATURE_HDR_IRAT_CLK_BOOST */

/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================

                                FUNCTIONS

===========================================================================*/

/*=========================================================================

FUNCTION     : HDRSRCHMSG_TRACE

DESCRIPTION  : This function stores UMID into trace buffer.

DEPENDENCIES : None

INPUTS       : trace - The message buffer
               id    - UMID

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_trace
(
  hdrsrchmsg_msg_trace_type *trace,
    /* Trace buffer */

  msgr_umid_type id
    /* UMID */
)
{
/*-----------------------------------------------------------------------*/

  trace->msg_trace[trace->offset] = id;

  trace->offset++;
  if ( trace->offset >= HDRSRCHMSG_TRACE_MAX )
  {
    trace->offset = 0;
  }

} /* hdrsrchmsg_trace */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_SEND_MSG

DESCRIPTION  : This function sends the message to MSGR.

DEPENDENCIES : None

INPUTS       : msg - The message pointer
               len - The message length 

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_send_msg
( 
  void                       *msg,
    /* Message to send */

  uint32                     len
    /* The message lenght */
)
{
  errno_enum_type        e_retval;
    /* error code */

/*-----------------------------------------------------------------------*/

  hdrsrchmsg_trace( &hdrsrchmsg.msg_sent, ((msgr_hdr_struct_type *) msg)->id );
    
  e_retval = msgr_send( (msgr_hdr_struct_type *) msg, len );

  if ( e_retval != E_SUCCESS )
  {
    ERR_FATAL( "Send msg: 0x%8x retval %d", 
               ( ( msgr_hdr_struct_type *) msg)->id, e_retval, 0 );
  }

}  /* hdrsrchmsg_send_msg */

/*=========================================================================

FUNCTION     : HDRSRCHMSG_MSG_IN_CMD_Q_INIT

DESCRIPTION  : This function creates MSGR client, associates it with command
               queue and registers the messages with MSGR.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_msg_in_cmd_q_init( void )
{

  errno_enum_type  estatus;
    /* Error status */

  msgr_id_t queue_id;
    /* Queue Identifier */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  estatus = msgr_client_create ( &hdrsrchmsg_rcvd.msg_in_cmd_q_client );

  ASSERT ( estatus == E_SUCCESS );

  estatus = msgr_client_add_rex_q ( &hdrsrchmsg_rcvd.msg_in_cmd_q_client,
                                    HDRSRCH_TCB,
                                    HDRSRCH_CMD_Q_SIG,
                                    &hdrsrchcmd.cmd_q,
                                    MSGR_NO_QUEUE,
                                    MSGR_OFFSETOF( hdrsrch_cmd_q_item_type, cmd ) + 8,
                                      /* offset to message header */
                                    MSGR_OFFSETOF( hdrsrch_cmd_q_item_type, cmd ),
                                      /* offset to legacy command type */
                                    (uint16) sizeof( hdrsrch_cmd_q_item_type ),
                                    &queue_id );
    /* Create the msgr client and associated with rex queues */

  ASSERT ( estatus == E_SUCCESS );

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
  /* Register messages for pilot meas  */
#ifdef FEATURE_HDR_MSGR_VARIANT
  estatus = msgr_register_block_variant( 
                  MSGR_HDR_SRCH,
                  &hdrsrchmsg_rcvd.msg_in_cmd_q_client,
                  MSGR_ID_REX,
                  hdrsrchmsg_lte_pilot_meas_msgs,
                  sizeof( hdrsrchmsg_lte_pilot_meas_msgs )/sizeof( msgr_umid_type ),
                  SYS_MODEM_AS_ID_1);
  
  ASSERT ( estatus == E_SUCCESS );
    
  estatus = msgr_register_block_variant(MSGR_HDR_SRCH,
                                        &hdrsrchmsg_rcvd.msg_in_cmd_q_client,
                                        MSGR_ID_REX,
                                        hdrsrchmsg_lte_pilot_meas_msgs,
                                        sizeof( hdrsrchmsg_lte_pilot_meas_msgs )/sizeof( msgr_umid_type ),
                                        SYS_MODEM_AS_ID_2);
  ASSERT ( estatus == E_SUCCESS );
#else
  estatus = msgr_register_block( 
                MSGR_HDR_SRCH,
                &hdrsrchmsg_rcvd.msg_in_cmd_q_client,
                MSGR_ID_REX,
                hdrsrchmsg_lte_pilot_meas_msgs,
                sizeof( hdrsrchmsg_lte_pilot_meas_msgs )/sizeof( msgr_umid_type ) );

  ASSERT ( estatus == E_SUCCESS );
#endif /* FEATURE_HDR_MSGR_VARIANT */
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

#ifdef FEATURE_HDR_MSGR_VARIANT  
    /* Register Offline command */
  estatus = msgr_register_variant(MSGR_HDR_SRCH, 
                                  &hdrsrchmsg_rcvd.msg_in_cmd_q_client,
                                  MSGR_ID_REX,
                                  MM_MMOC_OFFLINE_IND,
                                  MSGR_NO_VARIANT );
#else
  /* Register Offline command */
  estatus = msgr_register( 
                MSGR_HDR_SRCH,
                &hdrsrchmsg_rcvd.msg_in_cmd_q_client,
                MSGR_ID_REX,
                MM_MMOC_OFFLINE_IND );
#endif /* FEATURE_HDR_MSGR_VARIANT */

  ASSERT ( estatus == E_SUCCESS );

}

/*=========================================================================

FUNCTION     : HDRSRCHMSG_INIT

DESCRIPTION  : This function creates MSGR clients and queues and 
               registers the messages with MSGR.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_init( void )
{

  int                        i;
    /* loop index */

  errno_enum_type            estatus;
    /* Status */

/*-------------------------------------------------------------------------*/

  /* Initialize the queues for high priority responses */
  (void) q_init( &hdrsrchmsg_rcvd.high_pri_rsp_q.msg_q );
  (void) q_init( &hdrsrchmsg_rcvd.high_pri_rsp_q.free_q );

  /* Link the high priority response queue */
  for ( i = 0; i < HDRSRCHMSG_QUEUE_SIZE; i++ )
  {
    q_put( &hdrsrchmsg_rcvd.high_pri_rsp_q.free_q,
           q_link( &hdrsrchmsg_rcvd.high_pri_rsp_q.q_items[i], 
                   &hdrsrchmsg_rcvd.high_pri_rsp_q.q_items[i].qlink)
         );
  }

  estatus = msgr_client_create( &hdrsrchmsg_rcvd.high_pri_rsp_client );
    /* Create the MSGR Client */

  ASSERT( estatus == E_SUCCESS );

  estatus = msgr_client_set_rex_q ( &hdrsrchmsg_rcvd.high_pri_rsp_client,
                                    HDRSRCH_TCB,
                                    HDRSRCH_HIGH_PRI_RSP_SIG,
                                    &hdrsrchmsg_rcvd.high_pri_rsp_q.msg_q,
                                    &hdrsrchmsg_rcvd.high_pri_rsp_q.free_q,
                                    MSGR_OFFSETOF( hdrsrchmsg_s_type, msg ),
                                    MSGR_NO_OFFSET,
                                    (uint16)sizeof(hdrsrchmsg_s_type) );
    /* Set queue and signal for high priority response */

  ASSERT( estatus == E_SUCCESS );

  /* Register messages for high peiority response queue */
  estatus = msgr_register_block( 
                MSGR_HDR_SRCH,
                &hdrsrchmsg_rcvd.high_pri_rsp_client,
                MSGR_ID_REX,
                hdrsrchmsg_high_pri_msgs,
                sizeof( hdrsrchmsg_high_pri_msgs )/sizeof( msgr_umid_type ) );

  ASSERT( estatus == E_SUCCESS );

  /* Initialize the queues for low priority responses and indications */
  (void) q_init( &hdrsrchmsg_rcvd.low_pri_rsp_ind_q.msg_q );
  (void) q_init( &hdrsrchmsg_rcvd.low_pri_rsp_ind_q.free_q );

  /* Link the low priority response and indication queue */
  for ( i = 0; i < HDRSRCHMSG_QUEUE_SIZE; i++ )
  {
    q_put( &hdrsrchmsg_rcvd.low_pri_rsp_ind_q.free_q,
           q_link(&hdrsrchmsg_rcvd.low_pri_rsp_ind_q.q_items[i], 
                  &hdrsrchmsg_rcvd.low_pri_rsp_ind_q.q_items[i].qlink)
         );
  }

  estatus = msgr_client_create( &hdrsrchmsg_rcvd.low_pri_rsp_ind_client );
    /* Create the MSGR Client */

  ASSERT( estatus == E_SUCCESS );

  estatus = msgr_client_set_rex_q ( &hdrsrchmsg_rcvd.low_pri_rsp_ind_client,
                                    HDRSRCH_TCB,
                                    HDRSRCH_LOW_PRI_RSP_IND_SIG,
                                    &hdrsrchmsg_rcvd.low_pri_rsp_ind_q.msg_q,
                                    &hdrsrchmsg_rcvd.low_pri_rsp_ind_q.free_q,
                                    MSGR_OFFSETOF( hdrsrchmsg_s_type, msg ),
                                    MSGR_NO_OFFSET,
                                    (uint16)sizeof(hdrsrchmsg_s_type) );
    /* Set queue and signal for low priority responses and indications */

  ASSERT( estatus == E_SUCCESS );

#ifdef FEATURE_HDR_MSGR_VARIANT    
  estatus = msgr_register_block_variant(
                MSGR_HDR_SRCH,
                &hdrsrchmsg_rcvd.low_pri_rsp_ind_client,
                MSGR_ID_REX,
                hdrsrchmsg_low_pri_msgs,
                sizeof( hdrsrchmsg_low_pri_msgs )/sizeof( msgr_umid_type ),
                SYS_MODEM_AS_ID_1 );
    /* Register messages for low priority response queue */

  ASSERT( estatus == E_SUCCESS );

  estatus = msgr_register_block_variant( 
                MSGR_HDR_SRCH,
                &hdrsrchmsg_rcvd.low_pri_rsp_ind_client,
                MSGR_ID_REX,
                hdrsrchmsg_low_pri_msgs,
                sizeof( hdrsrchmsg_low_pri_msgs )/sizeof( msgr_umid_type ),
                SYS_MODEM_AS_ID_2 );
#else
  estatus = msgr_register_block( 
                MSGR_HDR_SRCH,
                &hdrsrchmsg_rcvd.low_pri_rsp_ind_client,
                MSGR_ID_REX,
                hdrsrchmsg_low_pri_msgs,
                sizeof( hdrsrchmsg_low_pri_msgs )/sizeof( msgr_umid_type ) );
    /* Register messages for low priority response queue */
#endif /* FEATURE_HDR_MSGR_VARIANT */

  ASSERT( estatus == E_SUCCESS );

  /* Initialize the queues for srch responses */
  (void) q_init( &hdrsrchmsg_rcvd.srch_rsp_q.msg_q );
  (void) q_init( &hdrsrchmsg_rcvd.srch_rsp_q.free_q );

  /* Link the srch response queue */
  for ( i = 0; i < HDRSRCHMSG_QUEUE_SIZE; i++ )
  {
    q_put( &hdrsrchmsg_rcvd.srch_rsp_q.free_q,
           q_link(&hdrsrchmsg_rcvd.srch_rsp_q.q_items[i], 
                  &hdrsrchmsg_rcvd.srch_rsp_q.q_items[i].qlink)
         );
  }

  estatus = msgr_client_create( &hdrsrchmsg_rcvd.srch_rsp_client );
    /* Create the MSGR Client */

  ASSERT( estatus == E_SUCCESS );

  estatus = msgr_client_set_rex_q ( &hdrsrchmsg_rcvd.srch_rsp_client,
                                    HDRSRCH_TCB,
                                    HDRSRCH_DUMP_SIG,
                                    &hdrsrchmsg_rcvd.srch_rsp_q.msg_q,
                                    &hdrsrchmsg_rcvd.srch_rsp_q.free_q,
                                    MSGR_OFFSETOF( hdrsrchmsg_s_type, msg ),
                                    MSGR_NO_OFFSET,
                                    (uint16)sizeof(hdrsrchmsg_s_type) );
    /* Set queue and signal for srch responses */

  ASSERT( estatus == E_SUCCESS );

  estatus = msgr_register_block( 
                MSGR_HDR_SRCH,
                &hdrsrchmsg_rcvd.srch_rsp_client,
                MSGR_ID_REX,
                hdrsrchmsg_srch_rsp_msgs,
                sizeof( hdrsrchmsg_srch_rsp_msgs )/sizeof( msgr_umid_type ) );
    /* Register messages for srch response queue */

  ASSERT( estatus == E_SUCCESS );

  /* Init message headers for the messages to be sent to FW */

  msgr_init_hdr( &hdrsrchmsg.tuneaway_start_msg.hdr,
                 MSGR_HDR_SRCH,
                 HDR_FW_TUNEAWAY_START_CMD );

  msgr_init_hdr( &hdrsrchmsg.tuneaway_stop_msg.hdr,
                 MSGR_HDR_SRCH,
                 HDR_FW_TUNEAWAY_STOP_CMD );

  msgr_init_hdr( &hdrsrchmsg.sleep_adj_msg.hdr,
                 MSGR_HDR_SRCH,
                 HDR_FW_SLEEP_ADJ_CMD );

  msgr_init_hdr( &hdrsrchmsg.fpd_msg.hdr,
                 MSGR_HDR_SRCH,
                 HDR_FW_FPD_CMD );

  msgr_init_hdr( &hdrsrchmsg.qpch_msg.hdr,
                 MSGR_HDR_SRCH,
                 HDR_FW_QPCH_CMD );

  msgr_init_hdr( &hdrsrchmsg.indication_cfg_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_INDICATION_CFG_CMD );

#ifdef FEATURE_HDR_SELECTIVE_TXD
  msgr_init_hdr( &hdrsrchmsg.ant_switch_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_ANT_SWITCH_ASDIV_CMD );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  memset( hdrsrchmsg.waiting_for_rsp, 
          0, 
          sizeof( hdrsrchmsg.waiting_for_rsp ) );
    /* Init the response waiting flags array */

  hdrsrchmsg.msg_rcv.offset = 0;
  hdrsrchmsg.msg_sent.offset = 0;
    /* Init message trace buffer */

  hdrsrchmsg_init_asp_update_msg( );
    /* Init ASP update msg */

  hdrsrchmsg_msg_in_cmd_q_init();
    /* Init command queue for message router */

} /* hdrsrchmsg_init() */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_CONFIG_INDICATIONS

DESCRIPTION  : This function configures the indications that HDR SRCH 
               will receive.

DEPENDENCIES : None

INPUTS       : None 

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_config_indications( void )
{

/*-----------------------------------------------------------------------*/

  /* Config HDRSRCH_IND_TYPE_BEST_ASP_CHANGE */
  hdrsrchmsg.indication_cfg_msg.enable = 1;
  hdrsrchmsg.indication_cfg_msg.type   = HDRSRCH_IND_TYPE_BEST_ASP_CHANGE;
  hdrsrchmsg_send_msg( &hdrsrchmsg.indication_cfg_msg.hdr,
                       sizeof( hdrfw_indication_cfg_msg_t ) );

  /* Config HDRSRCH_IND_TYPE_SRCH_DUMP */
  hdrsrchmsg.indication_cfg_msg.enable = 1;
  hdrsrchmsg.indication_cfg_msg.type   = HDRSRCH_IND_TYPE_SRCH_DUMP;
  hdrsrchmsg_send_msg( &hdrsrchmsg.indication_cfg_msg.hdr,
                       sizeof( hdrfw_indication_cfg_msg_t ) );


} /* hdrsrchmsg_config_indications */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_STOP

DESCRIPTION  : This function deletes the MSGR clients.

DEPENDENCIES : None

INPUTS       : msg - The message pointer
               len - The message length 

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_stop( void )
{

  errno_enum_type            estatus;
    /* Status */

/*-----------------------------------------------------------------------*/

#ifdef FEATURE_HDR_BOLT_MODEM
  if( ((void *)hdrsrchmsg_rcvd.high_pri_rsp_client) != NULL ) 
  {
  estatus = msgr_client_delete( &hdrsrchmsg_rcvd.high_pri_rsp_client );
    /* Delete the high priority response client */

  ASSERT( estatus == E_SUCCESS );
  }

  if( ((void *)hdrsrchmsg_rcvd.low_pri_rsp_ind_client) != NULL ) 
  {
  estatus = msgr_client_delete( &hdrsrchmsg_rcvd.low_pri_rsp_ind_client );
    /* Delete the low priority response and indication client */

  ASSERT( estatus == E_SUCCESS );
  }

  if( ((void *)hdrsrchmsg_rcvd.srch_rsp_client) != NULL ) 
  {
  estatus = msgr_client_delete( &hdrsrchmsg_rcvd.srch_rsp_client );
    /* Delete the srch dump response and indication client */

  ASSERT( estatus == E_SUCCESS );
  }
#else
  if( hdrsrchmsg_rcvd.high_pri_rsp_client != NULL ) 
  {
  estatus = msgr_client_delete( &hdrsrchmsg_rcvd.high_pri_rsp_client );
    /* Delete the high priority response client */

  ASSERT( estatus == E_SUCCESS );
  }

  if( hdrsrchmsg_rcvd.low_pri_rsp_ind_client != NULL ) 
  {
  estatus = msgr_client_delete( &hdrsrchmsg_rcvd.low_pri_rsp_ind_client );
    /* Delete the low priority response and indication client */

  ASSERT( estatus == E_SUCCESS );
  }

  if( hdrsrchmsg_rcvd.srch_rsp_client != NULL ) 
  {
  estatus = msgr_client_delete( &hdrsrchmsg_rcvd.srch_rsp_client );
    /* Delete the srch dump response and indication client */

  ASSERT( estatus == E_SUCCESS );
  }
#endif /* FEATURE_HDR_BOLT_MODEM */
} /* hdrsrchmsg_stop */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_GET_LOW_PRI_RSP_IND

DESCRIPTION  : This function gets a message from low priority response 
               and indication queue.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : Response/indication 

SIDE EFFECTS : None

=========================================================================*/

hdrsrchmsg_s_type *hdrsrchmsg_get_low_pri_rsp_ind( void )
{
  hdrsrchmsg_s_type *        msg;
    /* Message */

/*-----------------------------------------------------------------------*/

  msg = (hdrsrchmsg_s_type *) q_get( &hdrsrchmsg_rcvd.low_pri_rsp_ind_q.msg_q );

  return msg;
} /* hdrsrchmsg_get_low_pri_rsp_ind */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_FREE_LOW_PRI_RSP_IND

DESCRIPTION  : This function frees low priority rsponse/indication.

DEPENDENCIES : None

INPUTS       : msg - The message pointer

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_free_low_pri_rsp_ind
(
  hdrsrchmsg_s_type *        msg
    /* Message */
)
{

/*-----------------------------------------------------------------------*/

  (void) q_link( msg, &msg->qlink );
  q_put( &hdrsrchmsg_rcvd.low_pri_rsp_ind_q.free_q, &msg->qlink );
} /* hdrsrchmsg_free_low_pri_rsp_ind */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_GET_SRCH_RSP

DESCRIPTION  : This function gets a message from srch response queue.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : Response 

SIDE EFFECTS : None

=========================================================================*/

hdrsrchmsg_s_type *hdrsrchmsg_get_srch_rsp( void )
{
  hdrsrchmsg_s_type *        msg;
    /* Message */

/*-----------------------------------------------------------------------*/

  msg = (hdrsrchmsg_s_type *) q_get( &hdrsrchmsg_rcvd.srch_rsp_q.msg_q );

  return msg;
} /* hdrsrchmsg_get_srch_rsp */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_FREE_SRCH_RSP

DESCRIPTION  : This function frees srch rsp.

DEPENDENCIES : None

INPUTS       : msg - The message pointer

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_free_srch_rsp
(
  hdrsrchmsg_s_type *        msg
    /* Message */
)
{

/*-----------------------------------------------------------------------*/

  (void) q_link( msg, &msg->qlink );
  q_put( &hdrsrchmsg_rcvd.srch_rsp_q.free_q, &msg->qlink );
} /* hdrsrchmsg_free_srch_rsp */


/*!
    @brief
    MDR MSGR processing on getting task stop
*/
/*=========================================================================

FUNCTION     : HDRSRCHMSG_GET_HIGH_PRI_RSP

DESCRIPTION  : This function gets a high priority response.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : The response.

SIDE EFFECTS : None

=========================================================================*/

hdrsrchmsg_s_type *hdrsrchmsg_get_high_pri_rsp( void )
{
  hdrsrchmsg_s_type *        msg;
    /* Message */

/*-----------------------------------------------------------------------*/

  msg = (hdrsrchmsg_s_type *) q_get( &hdrsrchmsg_rcvd.high_pri_rsp_q.msg_q );
  return msg;
} /* hdrsrchmsg_get_high_pri_rsp */

/*=========================================================================

FUNCTION     : HDRSRCHMSG_FREE_high_PRI_RSP

DESCRIPTION  : This function frees the message.

DEPENDENCIES : None

INPUTS       : msg - The message pointer

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_free_high_pri_rsp
(
  hdrsrchmsg_s_type *        msg
    /* Message */
)
{

/*-----------------------------------------------------------------------*/

  (void) q_link( msg, &msg->qlink );
  q_put( &hdrsrchmsg_rcvd.high_pri_rsp_q.free_q, &msg->qlink );
} /* hdrsrchmsg_free_high_pri_rsp */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_GET_MSG_NAME

DESCRIPTION  : This function returns the message name.

DEPENDENCIES : None

INPUTS       : umid: UMID of the message 

RETURN VALUE : Name of the message.

SIDE EFFECTS : None

=========================================================================*/

const char *hdrsrchmsg_get_msg_name
(
  msgr_umid_type             id
    /* umid */
)
{
/*-----------------------------------------------------------------------*/

  return msgr_get_umid_name( id );

}/* hdrsrchmsg_get_msg_name */

/*=========================================================================

FUNCTION     : HDRSRCHMSG_INIT_ASP_UPDATE_MSG

DESCRIPTION  : This function init the ASP update message.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_init_asp_update_msg( void )
{
/*-----------------------------------------------------------------------*/

  memset( &hdrsrchmsg.asp_update_msg,
          0,
          sizeof( hdrfw_asp_update_msg_t ) );
    /* Clear the message */

  msgr_init_hdr( &hdrsrchmsg.asp_update_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_ASP_UPDATE_CMD );
    /* Init the message header */

}  /* hdrsrchmsg_init_asp_update_msg */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_INIT_HANDOFF_CFG_MSG

DESCRIPTION  : This function init handoff config message.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_init_handoff_cfg_msg( void )
{
/*-----------------------------------------------------------------------*/

  memset( &hdrsrchmsg.handoff_cfg_msg,
          0, 
          sizeof( hdrfw_handoff_cfg_msg_t ) );
    /* Clear the message */

  msgr_init_hdr( &hdrsrchmsg.handoff_cfg_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_HANDOFF_CFG_CMD );
    /* Initialize the message header */
} /* hdrsrchmsg_init_handoff_cfg_msg */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_HO_CFG_SET_REF_CARRIER

DESCRIPTION  : This function set handoff config message's reference carrier.

DEPENDENCIES : None

PARAMETERS   : c - Demod carrier

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/
void hdrsrchmsg_ho_cfg_set_ref_carrier
(
  uint8        c
    /* Demod carrier */
)
{
/*-----------------------------------------------------------------------*/

  hdrsrchmsg.handoff_cfg_msg.handoffRefCarrier = c;
    /* Set the reference carrier */
  hdrsrchmsg.handoff_cfg_msg.handoffRefCarrier_Valid = 
                               HDRSRCHMSG_REF_CARRIER_VALID;
    /* Set the carrier valid */
} /* hdrsrchmsg_ho_cfg_set_ref_carrier */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_HO_CFG_SET_HO_CTRL

DESCRIPTION  : This function set handoff config message's handoff control.

DEPENDENCIES : None

PARAMETERS   : ho_ctrl - Handoff control mode

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/
void hdrsrchmsg_ho_cfg_set_ho_ctrl
(
  uint8        ho_ctrl
    /* Handoff control mode */
)
{
/*-----------------------------------------------------------------------*/

  hdrsrchmsg.handoff_cfg_msg.handoffControl = ho_ctrl;
    /* Set the handoff control */

  hdrsrchmsg.handoff_cfg_msg.handoffControl_Valid = HDRSRCHMSG_HO_CTRL_VALID;
    /* Set the valid field */
} /* hdrsrchmsg_ho_cfg_set_ho_ctrl */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_INIT_HANDOFF_OVRD_MSG

DESCRIPTION  : This function init handoff override message.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_init_handoff_ovrd_msg( void )
{
/*-----------------------------------------------------------------------*/

  memset( &hdrsrchmsg.handoff_ovrd_msg, 
          0, 
          sizeof( hdrfw_handoff_override_msg_t ) );
    /* Clear the message */

  msgr_init_hdr( &hdrsrchmsg.handoff_ovrd_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_HANDOFF_OVERRIDE_CMD );
    /* Initialize the message header */
} /* hdrsrchmsg_init_handoff_ovrd_msg */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_HO_OVRD_SET_TRIGGER_MODE

DESCRIPTION  : This function sets the handoff trigger mode for handoff 
               override message

DEPENDENCIES : None

INPUTS       : c       - The demod carrier;
               asp_idx - The ASP index;

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_ho_ovrd_set_trigger_mode
(
  int8       c,
    /* The demod carrier */

  uint8      asp_idx
    /* The ASP index */
)
{
/*-----------------------------------------------------------------------*/

  /* Set the trigger mode for handoff override message */

  hdrsrchmsg.handoff_ovrd_msg.handoffOverride[c].carrierValid = 
                                                HDRSRCHMSG_CARRIER_VALID;

  hdrsrchmsg.handoff_ovrd_msg.handoffOverride[c].bestASPDict = asp_idx;

  hdrsrchmsg.handoff_ovrd_msg.handoffOverride[c].handoffOverride.override = 
                                                HDRSRCHMSG_HO_OVERRIDE_MODE;

  hdrsrchmsg.handoff_ovrd_msg.handoffOverride[c].handoffOverride.trigger = 
                                                HDRSRCHMSG_HO_TRIGGER_MODE;

  hdrsrchmsg.handoff_ovrd_msg.handoffOverride[c].handoffOverride.resetRCL = 
                                                HDRSRCHMSG_HO_RESET_RCL;

} /* hdrsrchmsg_ho_ovrd_msg_set_trigger_mode */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_HO_OVRD_SET_fw_ctrl

DESCRIPTION  : This function sets the handoff firmare control in handoff 
               override message

DEPENDENCIES : None

INPUTS       : c       - The demod carrier;
               ctrl    - contrl mode;

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_ho_ovrd_set_fw_ctrl
(
  int8       c,
    /* The demod carrier */

  uint8      ctrl
    /* The handoff control */
)
{
/*-----------------------------------------------------------------------*/
  hdrsrchmsg.handoff_ovrd_msg.handoffOverride[c].carrierValid = 
                                                    HDRSRCHMSG_CARRIER_VALID;
  hdrsrchmsg.handoff_ovrd_msg.handoffOverride[c].handoffOverride.override = ctrl;

} /* hdrsrchmsg_ho_ovrd_set_fw_ctrl */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_INIT_HANDOFF_CFG_MSG

DESCRIPTION  : This function init handoff config message.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_init_drc_cfg_msg( void )
{
/*-----------------------------------------------------------------------*/

  memset( &hdrsrchmsg.drc_cfg_msg,
          0, 
          sizeof( hdrfw_drc_cfg_msg_t ) );
    /* Clear the message */

  msgr_init_hdr( &hdrsrchmsg.drc_cfg_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_DRC_CFG_CMD );
    /* Initialize the message header */
} /* hdrsrchmsg_init_drc_cfg_msg */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_DRC_CFG_SET_DRC_OVRD

DESCRIPTION  : This function sets DRC override in DRC config message.

DEPENDENCIES : None

INPUTS       : c - carrier;
               drc_ovrd - DRC override mode

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_drc_cfg_set_drc_ovrd
(
  int8             c,
    /* the carrier */

  int8             drc_ovrd
    /* DRC override mode */
)
{
/*-----------------------------------------------------------------------*/
  /* Set the drc Override and corresponding valid bit in the message */
  hdrsrchmsg.drc_cfg_msg.drcCfg[c].drcOverride = drc_ovrd;
  hdrsrchmsg.drc_cfg_msg.drcCfg[c].drcOverride_Valid = HDRSRCHMSG_DRC_OVRD_VALID;

} /* hdrsrchmsg_drc_cfg_set_drc_ovrd */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_DRC_CFG_SET_DICT_DRC

DESCRIPTION  : This function sets dictated drc value in DRC config message.

DEPENDENCIES : None

INPUTS       : c - The carrier;
               dict_drc - The dictated DRC value;

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_drc_cfg_set_dict_drc
(
  int8             c,
    /* the carrier */

  uint8             dict_drc
    /* Dictated DRC value */
)
{
/*-----------------------------------------------------------------------*/

  hdrsrchmsg.drc_cfg_msg.drcCfg[c].dictDRC = dict_drc;
    /* Set the dictated DRC value in the message */
} /* hdrsrchmsg_drc_cfg_msg_set_dict_drc */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_DRC_CFG_ENABLE_DRC

DESCRIPTION  : This function sets enable drc field in DRC config message.

DEPENDENCIES : None

INPUTS       : c       - The carrier;
               enable  - The enable flag;

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_drc_cfg_enable_drc
(
  int8      c,
    /* The carrier */

  int8      enable
    /* Enable flag */
)
{
/*-----------------------------------------------------------------------*/

  /* Set the enable DRC and the valid fields */

  hdrsrchmsg.drc_cfg_msg.drcCfg[c].enableDRC = enable;

  hdrsrchmsg.drc_cfg_msg.drcCfg[c].enableDRC_Valid = HDRSRCHMSG_EN_DRC_VALID;

} /* hdrsrchmsg_drc_cfg_msg_enable_drc */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_INIT_DRC_FILT_RESET_MSG

DESCRIPTION  : This function init DRC filter reset message.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_init_drc_filt_reset_msg( void )
{
/*-----------------------------------------------------------------------*/

  memset( &hdrsrchmsg.drc_filt_reset_msg,
          0, 
          sizeof( hdrfw_drc_filt_reset_msg_t ) );
    /* Clear the message */

  msgr_init_hdr( &hdrsrchmsg.drc_filt_reset_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_DRC_FILT_RESET_CMD );
    /* Initialize the message header */
} /* hdrsrchmsg_init_drc_filt_reset_msg */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_SET_DRC_FILT_RESET_VALUE

DESCRIPTION  : This function sets DRC filter reset message.

DEPENDENCIES : None

INPUTS       : carrier - carrier index
               value - DRC filter reset value

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_set_drc_filt_reset_value
(
  int8              carrier,
    /* carrier */

  uint16            value
    /* drc filter reset value */
)
{
/*-----------------------------------------------------------------------*/

  hdrsrchmsg.drc_filt_reset_msg.drcFiltResetValue[carrier] = value;
    /* DRC filter output will be set to the passed value upon filter reset */

  hdrsrchmsg.drc_filt_reset_msg.carrierMask |= 0x1 << carrier;
    /* Set the carrier mask */
} /* hdrsrchmsg_set_drc_filt_reset_value */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_SEND_DRC_FILT_RESET_MSG

DESCRIPTION  : This function sends DRC filter reset message.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_send_drc_filt_reset_msg( void )
{
/*-----------------------------------------------------------------------*/

  hdrsrchmsg_send_msg( &hdrsrchmsg.drc_filt_reset_msg.hdr, 
                       sizeof( hdrfw_drc_filt_reset_msg_t ) );
    /* Send the message */

  hdrsrchmsg.waiting_for_rsp[HDRSRCH_DRCFILT_RESET_RSP] = TRUE;
    /* Set the flag */
} /* hdrsrchmsg_send_drc_filt_reset_msg */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_MULTI_ASP_UPDATE_MSG_SET_INFO

DESCRIPTION  : This function provides search set information at the time an 
               an ASP update message is sent to FW.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_multi_asp_update_msg_set_info( void )
{
  int32 rank[HDRSRCH_MAX_NUM_CARRIERS] = {0};
    /* Rank buffer */

  int r_idx, c_idx, p_idx;
    /* Rank index, carrier index and pilot index used in for loops */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for( c_idx = 0; c_idx < HDRSRCH_MAX_NUM_CARRIERS; c_idx++ )
  {
    for ( r_idx = 5; r_idx >= 0; r_idx-- )
    {
      rank[ c_idx ] |= hdrsrchset.subaset[ c_idx ].asp_snr_sorted_arr[ r_idx ] << 
        ( 4 * r_idx );
    }
  }

  HDR_MSG_SRCH_7 ( MSG_LEGACY_LOW, "ASP update msg:c_mask %x,updatemask %x,%x,%x, rank %x,%x,%x",
                   hdrsrchmsg.asp_update_msg.carrierMask,
                   hdrsrchmsg.asp_update_msg.updateMask[0],
                   hdrsrchmsg.asp_update_msg.updateMask[1],
                   hdrsrchmsg.asp_update_msg.updateMask[2],
                   rank[0],
                   rank[1],
                   rank[2]);                 

  for ( c_idx = 0;
        c_idx < hdrsrchset.subaset_cnt;
        c_idx++ )
  {
    for ( p_idx = 0; 
          p_idx < ( hdrsrchset.subaset[ c_idx].aset_cnt +
                    hdrsrchset.subaset[ c_idx].gcset_cnt ); 
          p_idx++ )
    {    
      HDR_MSG_SRCH_5( MSG_LEGACY_LOW, 
                      "Current Subaset[%d]=Ch%d/%d,asp=%d,rpc=%d",
                      c_idx,
                      hdrsrchset.subaset[ c_idx ].channel.chan_num,
                      hdrsrchset.subaset[ c_idx ].aset[ p_idx ]->pn_offset,
                      hdrsrchset.subaset[ c_idx ].aset[ p_idx ]->aset.asp_idx,
                      hdrsrchset.subaset[ c_idx ].aset[ p_idx ]->aset.rpc_cell_idx );
                                   
    } /* end for loop for pilots within subaset */

  } /* end for loop for subasets */

} /* hdrsrchmsg_multi_asp_update_msg_set_info */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_SET_ASP_UPDATE_MASK

DESCRIPTION  : This function set the demod carrier and ASP mask in ASP  
               update message.

DEPENDENCIES : None

INPUTS       : c       - demod carrier
               asp_idx - asp index

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/
void hdrsrchmsg_set_asp_update_mask
(
  uint16      c, 
    /* Demod carrier */

  uint8       asp_idx
    /* ASP index */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   hdrsrchmsg.asp_update_msg.carrierMask |= 0x1 << c;
     /* Set the carrier mask */

   hdrsrchmsg.asp_update_msg.updateMask[c] |= 0x1 << asp_idx;
      /* Set the asp mask */

}  /* hdrsrchmsg_set_asp_update_mask */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_ASP_UPDATE_SET_CARRIER_MASK

DESCRIPTION  : This function sets the demod carrier mask in the ASP  
               update message.

DEPENDENCIES : None

INPUTS       : c       - demod carrier

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_asp_update_set_carrier_mask
(
  uint16      c 
    /* Demod carrier */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchmsg.asp_update_msg.carrierMask |= 0x1 << c;
    /* Set the carrier mask */
} /* hdrsrchmsg_asp_update_set_carrier_mask */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_ASP_UPDATE_SET_ASP_MASK

DESCRIPTION  : This function sets the ASP mask in ASP update message. 

DEPENDENCIES : None

INPUTS       : c       - demod carrier
               asp_idx - asp index

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_asp_update_set_asp_mask
(
  uint16      c, 
    /* Demod carrier */

  uint8       asp_idx
    /* ASP index */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 hdrsrchmsg.asp_update_msg.updateMask[c] |= 0x1 << asp_idx;
    /* Set the asp mask */
} /* hdrsrchmsg_asp_update_set_asp_mask */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_SEND_MULTI_ASP_UPDATE_MSG

DESCRIPTION  : This function sends ASP update message to the FW.

DEPENDENCIES : None

INPUTS       : refresh_rank - Whether to rerank the ASET.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_send_multi_asp_update_msg
( 
  boolean refresh_rank  
    /* Whether to refresh the gRICE ranking. */
)
{
  boolean                           asp_config_status;
    /* Flag indicating whether ASP update response received or not */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Check if mDSP clocks are on */
  if( hdrmdsp_check_dsp_clk() == FALSE )
  {
    return;
  }

  /* Only send if something has been updated. */
  if ( hdrsrchmsg.asp_update_msg.carrierMask != 0 )
  {
      if ( refresh_rank )
      {
        hdrsrchset_grice_build_asp_snr_order_list();
          /* It is imperative that the GRICE ranking is correct when sending an
             ASP update. */
      }
    
      hdrsrchmsg_multi_asp_update_msg_set_info();
        /* Provide search set information for debugging */

      hdrsrchmsg_send_msg( &hdrsrchmsg.asp_update_msg.hdr,
                           sizeof( hdrfw_asp_update_msg_t ) );
        /* Send the message */
    
      hdrsrchmsg_set_rsp_waiting( HDRSRCH_ASP_UPDATE_RSP, TRUE );
        /* Wait for the response. */
          
      asp_config_status = 
               hdrsrch_wait_for_high_pri_rsp( HDRSRCHMSG_HALF_SLOT_RESPONSE_TIME_US, 
                                              HDRSRCH_ASP_UPDATE_RSP );
        /* Wait for the previous configuration response, or wait for 1/2 slot
           for the response */

      if ( asp_config_status == FALSE )
      {
        HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                     "Unable to commit gCSET, ASP update response is outstanding" );
        return;
      } 

      hdrsrchmsg_init_asp_update_msg();
        /* Clear out the ASP update message */
  }

} /* hdrsrchmsg_send_multi_asp_update_msg */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_SEND_ASP_UPDATE_MSG

DESCRIPTION  : This function sends ASP update message.

DEPENDENCIES : None

INPUTS       : demod_idx - demod index
               asp_idx   - asp index

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_send_asp_update_msg
(
  uint8              demod_idx,
    /* Demod index */

  uint8              asp_idx
    /* ASP index */
)
{
/*-----------------------------------------------------------------------*/

  hdrsrchmsg_init_asp_update_msg();
    /* Init the ASP update message */

  hdrsrchmsg_set_asp_update_mask( demod_idx, asp_idx );
    /* Update the carrier and ASP mask */

  hdrsrchmsg_send_msg( &hdrsrchmsg.asp_update_msg.hdr,
                       sizeof( hdrfw_asp_update_msg_t ) );
    /* Tell the FW about the ASP update asynchronously. */

}  /* hdrsrchmsg_send_asp_update_msg */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_SET_RAB_INIT_CTRL

DESCRIPTION  : This function sets the rab init contrl bit in ASP update message.

DEPENDENCIES : None

INPUTS       : demod_idx - demod index;
               asp_idx - asp index;
               rab_init_ctrl - contrl value to set;

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/
void hdrsrchmsg_set_rab_init_ctrl
( 
  int8                             demod_idx,
    /* demod index */

  int8                             asp_idx,
    /* asp index */

  hdrsrchutil_rab_init_ctrl_enum_t rab_init_ctrl
    /* QRab/FRabSoft init contrl */
)
{
/*-----------------------------------------------------------------------*/

  switch ( rab_init_ctrl )
  {
    case HDRSRCH_RESET_QRAB_FRAB:
    {
      hdrsrchmsg.asp_update_msg.initCtl[demod_idx][asp_idx].Z =
                                                HDRSRCHMSG_RESET_QRAB_FRAB;
        /* Set FRABSoft and QRABSoft to zero */

      break;
    }
    case HDRSRCH_RESET_QRAB_ONLY:
    {
      hdrsrchmsg.asp_update_msg.initCtl[demod_idx][asp_idx].F =
                                                HDRSRCHMSG_RESET_QRAB_ONLY;
        /* FRABSoft = max of FRAB, QRABSoft = 0 */

      break;
    }
    default:
    {
      hdrsrchmsg.asp_update_msg.initCtl[demod_idx][asp_idx].Z =
                                                HDRSRCHMSG_RAB_NO_CHANGE;
        /* Clear init control of Z bit */

      hdrsrchmsg.asp_update_msg.initCtl[demod_idx][asp_idx].F =
                                                HDRSRCHMSG_RAB_NO_CHANGE;
        /* Clear init control of F bit */

      break;
    }
  }
} /* hdrsrchmsg_set_rab_init_ctrl */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_BASP_HO_OVRD

DESCRIPTION  : This function sets handoff override and sends the message.

DEPENDENCIES : None

INPUTS       : demod_idx - demod index
               asp_idx   - asp index

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_basp_ho_ovrd
(
  uint8              demod_idx,
    /* Demod index */

  uint8              asp_idx
    /* ASP index */
)
{
/*-----------------------------------------------------------------------*/
  hdrsrchmsg_init_handoff_ovrd_msg();
    /* init handoff override message */

  hdrsrchmsg.handoff_ovrd_msg.handoffOverride[demod_idx].carrierValid = 
                                                    HDRSRCHMSG_CARRIER_VALID;
  hdrsrchmsg.handoff_ovrd_msg.handoffOverride[demod_idx].bestASPDict = asp_idx;
  hdrsrchmsg.handoff_ovrd_msg.handoffOverride[demod_idx].handoffOverride.override = 
                                                    HDRSRCHMSG_HO_OVERRIDE_MODE;
    /* Create the message */

  hdrsrchmsg_send_msg( &hdrsrchmsg.handoff_ovrd_msg.hdr, 
                       sizeof( hdrfw_handoff_override_msg_t ) );
    /* Send the message */

}  /* hdrsrchmsg_basp_ho_ovrd */

#ifdef FEATURE_HDR_DIME_MODEM

/*=========================================================================

FUNCTION     : HDRSRCHMSG_SEND_SLEEP_ADJ_MSG

DESCRIPTION  : This function sets the sleep adjustment value and sends the 
               sleep adjust message.

DEPENDENCIES : None

INPUTS       : sleep_adj - Sleep adjustment

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_send_sleep_adj_msg
(
  uint64      online_univ_stmr,
    /* univ stmr at online interrupt */

  uint64      online_cdma_sys_time,
    /* cdma system time at online interrupt */

  uint32      online_hdr_stmr
    /* HDR RTC value at online interrupt */
)
{
   hdrsrchmsg.sleep_adj_msg.onlineUstmr = online_univ_stmr;
   hdrsrchmsg.sleep_adj_msg.onlineCdmaSystemTime = online_cdma_sys_time;
   hdrsrchmsg.sleep_adj_msg.onlineRtcCx8 = online_hdr_stmr;
    /* Set values */

  hdrsrchmsg_send_msg( &hdrsrchmsg.sleep_adj_msg.hdr,
                       sizeof( hdrfw_sleep_adj_msg_t ) );
    /* Send the sleep adjustment message */
}
#else /* FEATURE_HDR_DIME_MODEM */
/*=========================================================================

FUNCTION     : HDRSRCHMSG_SEND_SLEEP_ADJ_MSG

DESCRIPTION  : This function sets the sleep adjustment value and sends the 
               sleep adjust message.

DEPENDENCIES : None

INPUTS       : sleep_adj - Sleep adjustment

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_send_sleep_adj_msg
(
  uint16      sleep_adj
    /* Sleep adjustment */
)
{
/*-----------------------------------------------------------------------*/
    hdrsrchmsg.sleep_adj_msg.sleepAdjust = sleep_adj;
      /* Set the value */

    hdrsrchmsg_send_msg( &hdrsrchmsg.sleep_adj_msg.hdr,
                         sizeof( hdrfw_sleep_adj_msg_t ) );
      /* Send the sleep adjustment message */

} /* hdrsrchmsg_send_sleep_adj_msg */

#endif /* FEATURE_HDR_DIME_MODEM */

/*=========================================================================

FUNCTION     : HDRSRCHMSG_START_DRC_RAMPDOWN

DESCRIPTION  : This function sets tuneaway mode and tuneaway time, then sends
               the tuneaway start message.

DEPENDENCIES : None

INPUTS       : tuneaway_mode - The tuneaway mode
               tuneaway_time - The tuneaway time

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_start_drc_rampdown
(
  uint16    tuneaway_mode,
    /* Tuneaway mode */

  uint16    tuneaway_time
    /* Tuneaway time of HSTR */
)
{
/*-----------------------------------------------------------------------*/

  hdrsrchmsg.tuneaway_start_msg.tuneAwayMode = tuneaway_mode;  
    /* Set the tuneaway mode */

  hdrsrchmsg.tuneaway_start_msg.tuneAwayTime = tuneaway_time;
    /* Set the tuneaway time */

  hdrsrchmsg_send_msg( &hdrsrchmsg.tuneaway_start_msg.hdr,
                       sizeof( hdrfw_tuneaway_start_msg_t ) );
    /* Send the message */
} /* hdrsrchmsg_start_drc_rampdown */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_SET_RAMPDOWN_MODE

DESCRIPTION  : This function sets tuneaway mode and tuneaway time, then sends
               the tuneaway start message.

DEPENDENCIES : None

INPUTS       : tuneaway_mode - The tuneaway mode

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_set_rampdown_mode
(
  uint16    tuneaway_mode
    /* Tuneaway mode */
)
{
/*-----------------------------------------------------------------------*/

  hdrsrchmsg.tuneaway_start_msg.tuneAwayMode = tuneaway_mode;  
    /* Set the tuneaway mode */

  hdrsrchmsg_send_msg( &hdrsrchmsg.tuneaway_start_msg.hdr,
                       sizeof( hdrfw_tuneaway_start_msg_t ) );
    /* Send the message */
} /* hdrsrchmsg_set_rampdown_mode */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_PROCESS_RCVD_RSP

DESCRIPTION  : This function resets the wating for response flag after 
               receiving the response.

DEPENDENCIES : None

INPUTS       : rsp - which response received

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_process_rcvd_rsp
(
  hdrsrchmsg_rsp_e_type      rsp
    /* FW responses */
)
{
/*-----------------------------------------------------------------------*/

  if ( hdrsrchmsg.waiting_for_rsp[rsp] == FALSE )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                    "FW response %d is received without config",
                    rsp ); 
  }
  else
  {
    hdrsrchmsg_set_rsp_waiting( rsp, FALSE );
      /* Reset waitng for response flag */
  }
} /* hdrsrchmsg_process_rcvd_rsp */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_RSP_IS_RCVD

DESCRIPTION  : This function checks whether the response is received or not.
               
DEPENDENCIES : None

INPUTS       : rsp - response to check

RETURN VALUE : TRUE  - the response is received
               FALSE - otherwise

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchmsg_rsp_is_rcvd
(
  hdrsrchmsg_rsp_e_type       rsp
    /* FW responses */
)
{
/*-----------------------------------------------------------------------*/

  return !hdrsrchmsg.waiting_for_rsp[rsp];
} /* hdrsrchmsg_rsp_is_rcvd */


/*=========================================================================

FUNCTION     : HDRSRCHMSG_SET_RSP_WAITING

DESCRIPTION  : This function sets the response in waiting to be true or false
               based on the flag.
               
DEPENDENCIES : None

INPUTS       : rsp   - which FW response
               flag  - boolean value to be set

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchmsg_set_rsp_waiting
(
  hdrsrchmsg_rsp_e_type       rsp,
    /* The FW responses */

  boolean                     flag
    /* The flag value to be set */
)
{
/*-----------------------------------------------------------------------*/

  hdrsrchmsg.waiting_for_rsp[rsp] = flag;
    /* Set the flag */
} /* hdrsrchmsg_set_rsp_waiting */

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
/*===========================================================================

FUNCTION HDRSRCHMSG_PREPARE_LTE_PILOT_MEAS_RSP_REPORT

DESCRIPTION
  This function prepares pilot measurement response

DEPENDENCIES
  None

PARAMETERS
    pilot_meas_status   :   pilot measurement status

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchmsg_prepare_lte_pilot_meas_rsp_report
( 
  hdrsrch_lte_pilot_meas_status_type   pilot_meas_status,
    /* pilot measurement status */

  sys_channel_type                     chan
    /* band and channel number */
)
{

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Send LTE Pilot Meas Rsp SUB ID %d",
                  hdrsrch_get_subscriber_id() );

  memset( &hdrsrchmsg_lte.lte_pilot_meas_rsp,
          0,
          sizeof( hdrsrch_lte_pilot_meas_rsp_msg_t ) );
    /* Clear the message */

#ifdef FEATURE_HDR_MSGR_VARIANT 
    msgr_init_hdr_variant( &hdrsrchmsg_lte.lte_pilot_meas_rsp.hdr, 
                   MSGR_HDR_SRCH, 
                   HDR_SRCH_LTE_PILOT_MEAS_RSP,
                   hdrsrch_get_subscriber_id());
#else
  msgr_init_hdr( &hdrsrchmsg_lte.lte_pilot_meas_rsp.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_SRCH_LTE_PILOT_MEAS_RSP );
#endif /* FEATURE_HDR_MSGR_VARIANT */
    /* Init the message header */

  hdrsrchmsg_lte.lte_pilot_meas_rsp.status = pilot_meas_status;
    /* Assign pilot measurement status */

  if ( pilot_meas_status != PILOT_MEAS_SUCCESS  )
  {
    hdrsrchmsg_lte.lte_pilot_meas_rsp.sector_count = 0;
      /* Pilot measurement is done unsuccessfully. Return sector count=0 */
    hdrsrchmsg_lte.lte_pilot_meas_rsp.channel = chan;
      /* Populate band/channel for failure response too */
  }
  else
  {
    /* Copy pilot measurement result */

    hdrsrchset_get_lte_pilot_meas_report( &hdrsrchmsg_lte.lte_pilot_meas_rsp );
  }

} /* hdrsrchmsg_prepare_lte_pilot_meas_rsp_report */

/*===========================================================================

FUNCTION HDRSRCHMSG_SEND_LTE_PILOT_MEAS_RSP

DESCRIPTION
  This function sends pilot measurement response back to LTE

DEPENDENCIES
  None

PARAMETERS
    pilot_meas_status   :   pilot measurement status

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchmsg_send_lte_pilot_meas_rsp
( 
  hdrsrch_lte_pilot_meas_status_type   pilot_meas_status,
    /* pilot measurement status */

  sys_channel_type                     chan
    /* band and channel number */
)
{
  hdrsrchmsg_prepare_lte_pilot_meas_rsp_report(
                                                            pilot_meas_status, chan );

  hdrsrchmsg_send_msg( &hdrsrchmsg_lte.lte_pilot_meas_rsp, 
                         sizeof( hdrsrch_lte_pilot_meas_rsp_msg_t ) );
    /* Send the message out */

} /* hdrsrchmsg_send_lte_pilot_meas_rsp */

#ifdef FEATURE_HDR_IRAT_DEBUG
/*===========================================================================

FUNCTION HDRSRCHTIMESYNC_SEND_FAKE_LTE_PILOT_MEAS_RSP

DESCRIPTION
  This function sends a fake pilot measurement response back to LTE. This is
  for debug purpose when real gap is skipped.

DEPENDENCIES
  None

PARAMETERS
    pilot_meas_status   :   pilot measurement status

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchmsg_send_fake_lte_pilot_meas_rsp
( 
  hdrsrch_lte_pilot_meas_status_type   pilot_meas_status,
    /* pilot measurement status */

  sys_channel_type                     chan
    /* band and channel number */
)
{
  int                                   i;
    /* loop index */

  hdrsrch_lte_pilot_meas_rsp_msg_t      *p_pilot_meas_rsp;
    /* Convenience pointer */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset( &hdrsrchmsg_lte.lte_pilot_meas_rsp,
          0,
          sizeof( hdrsrch_lte_pilot_meas_rsp_msg_t ) );
    /* Clear the message */

#ifdef FEATURE_HDR_MSGR_VARIANT 
    msgr_init_hdr_variant( &hdrsrchmsg_lte.lte_pilot_meas_rsp.hdr, 
                   MSGR_HDR_SRCH, 
                   HDR_SRCH_LTE_PILOT_MEAS_RSP,
                   hdrsrch_get_subscriber_id());
#else
  msgr_init_hdr( &hdrsrchmsg_lte.lte_pilot_meas_rsp.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_SRCH_LTE_PILOT_MEAS_RSP );
#endif /* FEATURE_HDR_MSGR_VARIANT */
    /* Init the message header */
 

  hdrsrchmsg_lte.lte_pilot_meas_rsp.status = pilot_meas_status;
    /* Assign pilot measurement status */

  p_pilot_meas_rsp = &hdrsrchmsg_lte.lte_pilot_meas_rsp; 
  /* Copy pilot measurement result */
  hdrsrchset_get_lte_pilot_meas_report( p_pilot_meas_rsp );

  p_pilot_meas_rsp->cdma_time_delta = 0;
  p_pilot_meas_rsp->channel = chan;

  for ( i = 0; i < p_pilot_meas_rsp->sector_count;
        i++ )
  {
    p_pilot_meas_rsp->strength_lin_list[i] = 200;
    p_pilot_meas_rsp->strength_db_list[i] =  8;
    p_pilot_meas_rsp->pn_phase[i] = p_pilot_meas_rsp->pn_list[i]*64;

    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
        "IRAT DEBUG fake Meas rsp: (pn%d) linear=%d db=%d,pn_phase=0x%x",
        p_pilot_meas_rsp->pn_list[i],
        p_pilot_meas_rsp->strength_lin_list[i],
        p_pilot_meas_rsp->strength_db_list[i],
        (uint16)p_pilot_meas_rsp->pn_phase[i] );
  }

  hdrsrchmsg_send_msg( &hdrsrchmsg_lte.lte_pilot_meas_rsp, 
                         sizeof( hdrsrch_lte_pilot_meas_rsp_msg_t ) );
    /* Send the message out */

} /* hdrsrchmsg_send_fake_lte_pilot_meas_rsp */
#endif /* FEATURE_HDR_IRAT_DEBUG */

/*===========================================================================

FUNCTION HDRSRCHMSG_SEND_RF_STATUS_IND

DESCRIPTION
  This function sends RF Status Indication

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchmsg_send_rf_status_ind
(
   hdrsrch_irat_rf_status_type         irat_rf_status
     /* Whether sample capture is done */
)
{

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Send IRAT RF STATUS msg on SUB ID %d",hdrsrch_get_subscriber_id() );
    
  memset( &hdrsrchmsg_lte.rf_status_ind,
           0,
           sizeof( hdrsrchmsg_lte.rf_status_ind ) 
         );
  /* Initialize message */
  
#ifdef FEATURE_HDR_MSGR_VARIANT 
  msgr_init_hdr_variant( &hdrsrchmsg_lte.rf_status_ind.hdr, 
                   MSGR_HDR_SRCH, 
                   HDR_SRCH_IND_RF_STATUS,
                   hdrsrch_get_subscriber_id() );
  
#else
  msgr_init_hdr( &hdrsrchmsg_lte.rf_status_ind.hdr, 
                   MSGR_HDR_SRCH, 
                   HDR_SRCH_IND_RF_STATUS );
  
#endif /* FEATURE_HDR_MSGR_VARIANT */

  hdrsrchmsg_lte.rf_status_ind.rf_status = irat_rf_status;
    /* Set the flag of sample capture done */

  hdrsrchmsg_send_msg( &hdrsrchmsg_lte.rf_status_ind, 
                         sizeof( hdrsrch_rf_status_ind_msg_t ) );
    /* Send the message out */

} /* hdrsrchmsg_send_rf_status_ind */

/*===========================================================================

FUNCTION HDRSRCHMSG_SEND_LTE_PILOT_MEAS_ABORT_RSP

DESCRIPTION
  This function sends LTE Pilot meas abort response

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchmsg_send_lte_pilot_meas_abort_rsp ( void )
{

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Send LTE Pilot Meas Abort Rsp SUB ID %d",
                        hdrsrch_get_subscriber_id() );
  
#ifdef FEATURE_HDR_MSGR_VARIANT 
  msgr_init_hdr_variant( &hdrsrchmsg_lte.lte_pilot_meas_abort_rsp.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_SRCH_RSP_LTE_PILOT_MEAS_ABORT,
                 hdrsrch_get_subscriber_id());
#else
  msgr_init_hdr( &hdrsrchmsg_lte.lte_pilot_meas_abort_rsp.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_SRCH_RSP_LTE_PILOT_MEAS_ABORT );
#endif /* FEATURE_HDR_MSGR_VARIANT */

  hdrsrchmsg_send_msg( &hdrsrchmsg_lte.lte_pilot_meas_abort_rsp, 
                        sizeof( hdrsrch_lte_pilot_meas_abort_rsp_msg_t ) );
           /* Send Pilot Meas Abort Response */

} /* hdrsrchmsg_send_lte_pilot_meas_abort_rsp */


#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

#ifdef FEATURE_HDR_TO_LTE

/*===========================================================================

FUNCTION HDRSRCHMSG_D2L_SEND_INIT_REQ_MSG 
 
DESCRIPTION
  This function sends the INIT_REQ message with the appropriate header
  and parameters to the LTE MSGR interface.  This message is sent as part of
  the init phase for LTE pilot measurements.  The init phase is required for
  LTE to enable LTE FW app and other processing prior to pilot measurements.
  When done, HDR must also send DEINIT_REQ message.
 
DEPENDENCIES
  None

PARAMETERS 
  None 

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchmsg_d2l_send_init_req_msg( void )

{

/*-----------------------------------------------------------------------*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "D2L Send LTE_INIT_REQ message" );

  memset( &hdrsrchmsg_d2l_msg.init_req_msg,
          0,
          sizeof( hdrsrchmsg_d2l_msg.init_req_msg ) 
        );
    /* Initialize message */

#ifdef FEATURE_HDR_MSGR_VARIANT 
    msgr_init_hdr_variant( &hdrsrchmsg_d2l_msg.init_req_msg.hdr, 
                     MSGR_HDR_SRCH, 
                     LTE_CPHY_IRAT_MEAS_D2L_INIT_REQ,
                     hdrsrch_get_subscriber_id()
                   ); 
#else
  msgr_init_hdr( &hdrsrchmsg_d2l_msg.init_req_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 LTE_CPHY_IRAT_MEAS_D2L_INIT_REQ 
               ); 
#endif /* FEATURE_HDR_MSGR_VARIANT */
    /* Create message header */

  hdrsrchmsg_d2l_msg.init_req_msg.rat = LTE_CPHY_IMEAS_RAT_DO;
    /* Tell LTE that DO technology is requesting pilot measurements */

  hdrsrchmsg_send_msg( &hdrsrchmsg_d2l_msg.init_req_msg, 
                       sizeof( hdrsrchmsg_d2l_msg.init_req_msg ) 
                     );
    /* Send message */

} /* hdrsrchmsg_d2l_send_init_req_msg */

/*===========================================================================

FUNCTION HDRSRCHMSG_D2L_SEND_DEINIT_REQ_MSG
 
DESCRIPTION
  This function sends the DEINIT_REQ message with the appropriate header
  and parameters to the LTE MSGR interface.  This message is sent once
  LTE pilot measurements are no longer required.  Upon receiving this message
  LTE SW disables LTE FW app among other shut down processing.  An INIT_REQ
  message is required before any future LTE pilot measurement requests can
  be processed.
 
DEPENDENCIES
  None

PARAMETERS 
  None 

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchmsg_d2l_send_deinit_req_msg( void )
{

/*-----------------------------------------------------------------------*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "D2L Send LTE_DEINIT_REQ message" );

  memset( &hdrsrchmsg_d2l_msg.deinit_req_msg,
          0,
          sizeof( hdrsrchmsg_d2l_msg.deinit_req_msg ) 
        );
    /* Initialize message */

#ifdef FEATURE_HDR_MSGR_VARIANT 
    msgr_init_hdr_variant( &hdrsrchmsg_d2l_msg.deinit_req_msg.hdr, 
                   MSGR_HDR_SRCH, 
                   LTE_CPHY_IRAT_MEAS_D2L_DEINIT_REQ,
                   hdrsrch_get_subscriber_id()
               ); 
#else
  msgr_init_hdr( &hdrsrchmsg_d2l_msg.deinit_req_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 LTE_CPHY_IRAT_MEAS_D2L_DEINIT_REQ 
               ); 
#endif /* FEATURE_HDR_MSGR_VARIANT */
    /* Create message header */

  hdrsrchmsg_send_msg( &hdrsrchmsg_d2l_msg.deinit_req_msg, 
                       sizeof( hdrsrchmsg_d2l_msg.deinit_req_msg ) 
                     );
    /* Send message */

} /* hdrsrchmsg_d2l_send_deinit_req_msg */

/*===========================================================================

FUNCTION HDRSRCHMSG_D2L_SEND_STARTUP_CMD_MSG
 
DESCRIPTION
  This function sends the STARTUP_CMD message with the appropriate header
  and parameters to the LTE MSGR interface.  This message is sent to start
  an LTE pilot measurement process as part of the LTE IRAT interface.
 
DEPENDENCIES
  None

PARAMETERS 
  LTE Frequency to measure
  Frequency bandwidth
  RF script to tune back

RETURN VALUE 
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchmsg_d2l_send_startup_cmd_msg
(
  lte_earfcn_t                            earfcn, 
    /* EARFCN - Evolved Absolute Radio Frequency Channel Number
       or simply the LTE frequency to search and measure */

  hdrsrchmsg_lte_bandwidth_enum_type      bandwidth,
    /* Bandwidth of the freq */

  lte_cphy_irat_meas_startup_rf_params_s *rf_tuneback_script_ptr
    /* Tune back script to be used when returning to DO */
)

{
  errno_enum_type                        status = E_SUCCESS;
    /* Command completion status */

/*-----------------------------------------------------------------------*/

  if( ( bandwidth < HDRSRCHMSG_BW_MIN ) ||
      ( bandwidth > HDRSRCHMSG_BW_MAX )
    )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "D2L LTE frequency bandwidth out of range" );
    status = E_FAILURE;
    return( status );
  }

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "D2L Send LTE_STARTUP_CMD message,earfcn=%d,bw=%d,RFscript=0x%x", 
                  earfcn,                
                  bandwidth,             
                  rf_tuneback_script_ptr );

  memset( &hdrsrchmsg_d2l_msg.startup_cmd_msg,
          0,
          sizeof( hdrsrchmsg_d2l_msg.startup_cmd_msg ) 
        );
    /* Initialize message */

#ifdef FEATURE_HDR_MSGR_VARIANT 
    msgr_init_hdr_variant( &hdrsrchmsg_d2l_msg.startup_cmd_msg.hdr, 
                   MSGR_HDR_SRCH, 
                   LTE_CPHY_IRAT_MEAS_D2L_STARTUP_CMD,
                   hdrsrch_get_subscriber_id()
                 ); 
#else
  msgr_init_hdr( &hdrsrchmsg_d2l_msg.startup_cmd_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 LTE_CPHY_IRAT_MEAS_D2L_STARTUP_CMD 
               ); 
#endif /* FEATURE_HDR_MSGR_VARIANT */
    /* Create message header */
                                                              
  /*--------------------------------------------------------------------------
    Build message payload
  --------------------------------------------------------------------------*/

  hdrsrchmsg_d2l_msg.startup_cmd_msg.earfcn = earfcn;                            
    /* Frequency to measure */
                                                              
  switch( bandwidth )        
    /* Bandwidth of frequency to measure */                                  
  {                                                           
    case HDRSRCHMSG_BW_NRB_6:                                 
      hdrsrchmsg_d2l_msg.startup_cmd_msg.dl_bandwidth = LTE_BW_NRB_6;            
      break; 
                                                       
    case HDRSRCHMSG_BW_NRB_15:                                
      hdrsrchmsg_d2l_msg.startup_cmd_msg.dl_bandwidth = LTE_BW_NRB_15;           
      break;         
                                               
    case HDRSRCHMSG_BW_NRB_25:
      hdrsrchmsg_d2l_msg.startup_cmd_msg.dl_bandwidth = LTE_BW_NRB_25;
      break;

    case HDRSRCHMSG_BW_NRB_50:
      hdrsrchmsg_d2l_msg.startup_cmd_msg.dl_bandwidth = LTE_BW_NRB_50;
      break;

    case HDRSRCHMSG_BW_NRB_75:
      hdrsrchmsg_d2l_msg.startup_cmd_msg.dl_bandwidth = LTE_BW_NRB_75;
      break;

    case HDRSRCHMSG_BW_NRB_100:
      hdrsrchmsg_d2l_msg.startup_cmd_msg.dl_bandwidth = LTE_BW_NRB_100;
      break;

    default:
      break;
        /* Do not expect to get here, range checking has been done above */
  }

  hdrsrchmsg_d2l_msg.startup_cmd_msg.use_scripts = TRUE;
    /* Use RF script API rather than old RF API */

  if( rf_tuneback_script_ptr != NULL ) 
  {
    hdrsrchmsg_d2l_msg.startup_cmd_msg.rf_params = *rf_tuneback_script_ptr;
      /* Tune back RF script */
  }

  hdrsrchmsg_send_msg( &hdrsrchmsg_d2l_msg.startup_cmd_msg, 
                       sizeof( hdrsrchmsg_d2l_msg.startup_cmd_msg ) 
                     );
    /* Send message */

  return( status );

} /* hdrsrchmsg_d2l_send_startup_cmd_msg */

/*===========================================================================

FUNCTION HDRSRCHMSG_D2L_SEND_SEARCH_REQ_MSG
 
DESCRIPTION
  This function sends the SEARCH_REQ message with the appropriate header
  and parameters to the LTE MSGR interface. Typically, LTE needs to search
  once every 3 pilot measurements to ensure that timing is accurate for the
  measurements.
 
DEPENDENCIES
  None

PARAMETERS 
  None 

RETURN VALUE 
  None 

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchmsg_d2l_send_search_req_msg( void )
{

/*-----------------------------------------------------------------------*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "D2L Send LTE_SEARCH_REQ message" );

  memset( &hdrsrchmsg_d2l_msg.search_req_msg,
          0,
          sizeof( hdrsrchmsg_d2l_msg.search_req_msg ) 
        );
    /* Initialize message */

#ifdef FEATURE_HDR_MSGR_VARIANT 
    msgr_init_hdr_variant( &hdrsrchmsg_d2l_msg.search_req_msg.hdr, 
                   MSGR_HDR_SRCH, 
                   LTE_CPHY_IRAT_MEAS_D2L_SEARCH_REQ,
                   hdrsrch_get_subscriber_id()
                 ); 
#else
  msgr_init_hdr( &hdrsrchmsg_d2l_msg.search_req_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 LTE_CPHY_IRAT_MEAS_D2L_SEARCH_REQ 
               ); 
#endif /* FEATURE_HDR_MSGR_VARIANT */
    /* Create message header */

  hdrsrchmsg_send_msg( &hdrsrchmsg_d2l_msg.search_req_msg, 
                       sizeof( hdrsrchmsg_d2l_msg.search_req_msg ) 
                     );
    /* Send message */

} /* hdrsrchmsg_d2l_send_search_req_msg */

/*===========================================================================

FUNCTION HDRSRCHMSG_D2L_SEND_MEAS_REQ_MSG
 
DESCRIPTION
  This function sends the MEAS_REQ message with the appropriate header
  and parameters to the LTE MSGR interface. 
 
  The following was extracted from LTE documentation of this message:
  "This message triggers LTE measurements on already detected LTE cells.
  Cells must be detected via an lte_cphy_irat_meas_search_req_s prior to
  measuring them.  Measurements are performed on the RS tones in the LTE
  frequency grid."
 
DEPENDENCIES
  None

PARAMETERS 
  None 

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchmsg_d2l_send_meas_req_msg
( 
  hdrsrch_lte_search_cnf_params_type    *search_cnf_ptr
)
{
  errno_enum_type                       status = E_SUCCESS;
    /* Command completion status */

  int                                   i;

/*-----------------------------------------------------------------------*/

  if( search_cnf_ptr->num_detected_cells > LTE_CPHY_IMEAS_MAX_DETECTED_CELLS ) 
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "D2L Number of cells=%d exceeds Max",
                    search_cnf_ptr->num_detected_cells );
    status = E_FAILURE;
    return( status );
  }

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "D2L Send LTE_MEAS_REQ message, num cells=%d",
                  search_cnf_ptr->num_detected_cells );

  memset( &hdrsrchmsg_d2l_msg.meas_req_msg,
          0,
          sizeof( hdrsrchmsg_d2l_msg.meas_req_msg ) 
        );
    /* Initialize message */

#ifdef FEATURE_HDR_MSGR_VARIANT 
    msgr_init_hdr_variant( &hdrsrchmsg_d2l_msg.meas_req_msg.hdr, 
                   MSGR_HDR_SRCH, 
                   LTE_CPHY_IRAT_MEAS_D2L_MEAS_REQ,
                   hdrsrch_get_subscriber_id()
                 );  
#else
  msgr_init_hdr( &hdrsrchmsg_d2l_msg.meas_req_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 LTE_CPHY_IRAT_MEAS_D2L_MEAS_REQ 
               ); 
#endif /* FEATURE_HDR_MSGR_VARIANT */
    /* Create message header */

  /*--------------------------------------------------------------------------
    Build message payload
  --------------------------------------------------------------------------*/

  hdrsrchmsg_d2l_msg.meas_req_msg.num_cells = search_cnf_ptr->num_detected_cells;
    /*  Number of cells in frequency to be measured */

  for( i = 0; i < hdrsrchmsg_d2l_msg.meas_req_msg.num_cells; i++ ) 
  {
    hdrsrchmsg_d2l_msg.meas_req_msg.cells[ i ].cell_id = search_cnf_ptr->det_cells[ i ].cell_id;
    hdrsrchmsg_d2l_msg.meas_req_msg.cells[ i ].cp_type = search_cnf_ptr->det_cells[ i ].cp_type;
    hdrsrchmsg_d2l_msg.meas_req_msg.cells[ i ].state = search_cnf_ptr->det_cells[ i ].state;
      /* Give LTE cell information.  LTE gave this to HDR in the
         LTE_CPHY_IRAT_MEAS_D2L_SEARCH_CNF and it needs this information back
         for the measurement. */
  }

  hdrsrchmsg_send_msg( &hdrsrchmsg_d2l_msg.meas_req_msg, 
                       sizeof( hdrsrchmsg_d2l_msg.meas_req_msg ) 
                     );
    /* Send message */

  return( status );

} /* hdrsrchmsg_d2l_send_meas_req_msg */

/*===========================================================================

FUNCTION HDRSRCHMSG_D2L_SEND_CLEANUP_REQ_MSG
 
DESCRIPTION
  This function sends the CLEANUP_REQ message with the appropriate header
  and parameters to the LTE MSGR interface.  This message is sent after LTE
  pilot measurement is received as part of the LTE IRAT interface.
 
DEPENDENCIES
  None

PARAMETERS 
  None 

RETURN VALUE 
  None 

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchmsg_d2l_send_cleanup_req_msg( void )
{

/*-----------------------------------------------------------------------*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "D2L Send LTE_CLEANUP_REQ message" );

  memset( &hdrsrchmsg_d2l_msg.cleanup_req_msg,
          0,
          sizeof( hdrsrchmsg_d2l_msg.cleanup_req_msg ) 
        );
    /* Initialize message */

#ifdef FEATURE_HDR_MSGR_VARIANT 
    msgr_init_hdr_variant( &hdrsrchmsg_d2l_msg.cleanup_req_msg.hdr, 
                   MSGR_HDR_SRCH, 
                   LTE_CPHY_IRAT_MEAS_D2L_CLEANUP_REQ,
                   hdrsrch_get_subscriber_id()
                 ); 
#else
  msgr_init_hdr( &hdrsrchmsg_d2l_msg.cleanup_req_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 LTE_CPHY_IRAT_MEAS_D2L_CLEANUP_REQ 
               ); 
#endif /* FEATURE_HDR_MSGR_VARIANT */
    /* Create message header */

  hdrsrchmsg_send_msg( &hdrsrchmsg_d2l_msg.cleanup_req_msg, 
                       sizeof( hdrsrchmsg_d2l_msg.cleanup_req_msg ) 
                     );
    /* Send message */

} /* hdrsrchmsg_d2l_send_cleanup_req_msg */

/*===========================================================================

FUNCTION HDRSRCHMSG_D2L_SEND_ABORT_REQ_MSG
 
DESCRIPTION
  This function sends the ABORT_REQ message to abort any ongoing search or                                      
  measurement operation.
 
DEPENDENCIES
  None

PARAMETERS 
  None 

RETURN VALUE
  None

SIDE EFFECTS
  Note:
  It is possible that LTE may still send a search or measurement operation
  after HDR sends this command due to the multithreaded nature of the system.

===========================================================================*/

void hdrsrchmsg_d2l_send_abort_req_msg( void )
{

/*-----------------------------------------------------------------------*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "D2L Send LTE_ABORT_REQ message" );

  memset( &hdrsrchmsg_d2l_msg.abort_req_msg,
          0,
          sizeof( hdrsrchmsg_d2l_msg.abort_req_msg ) 
        );
    /* Initialize message */

#ifdef FEATURE_HDR_MSGR_VARIANT 
    msgr_init_hdr_variant( &hdrsrchmsg_d2l_msg.abort_req_msg.hdr, 
                   MSGR_HDR_SRCH, 
                   LTE_CPHY_IRAT_MEAS_D2L_ABORT_REQ,
                   hdrsrch_get_subscriber_id()
                 ); 
#else
  msgr_init_hdr( &hdrsrchmsg_d2l_msg.abort_req_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 LTE_CPHY_IRAT_MEAS_D2L_ABORT_REQ 
               ); 
#endif /* FEATURE_HDR_MSGR_VARIANT */
    /* Create message header */

  hdrsrchmsg_send_msg( &hdrsrchmsg_d2l_msg.abort_req_msg, 
                       sizeof( hdrsrchmsg_d2l_msg.abort_req_msg ) 
                     );
    /* Send message */

} /* hdrsrchmsg_d2l_send_abort_req_msg */

/*===========================================================================

FUNCTION HDRSRCHMSG_D2L_SEND_TIMED_SEARCH_MEAS_REQ_MSG
 
DESCRIPTION
  This function sends the TIMED_SEARCH_MEAS_REQ message which is a combined
  message replacing:
  STARTUP_CMD
  SEARCH_REQ
  MEAS_REQ
  CLEANUP_REQ
 
DEPENDENCIES
  None

PARAMETERS 
  LTE Frequency to measure
  LTE Frequency bandwidth
  LTE Frequency TRM data
  LTE RF parameters

RETURN VALUE 
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command
 
SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchmsg_d2l_send_timed_search_meas_req_msg
(
  lte_earfcn_t                                  earfcn, 
    /* EARFCN - Evolved Absolute Radio Frequency Channel Number
       or simply the LTE frequency to search and measure */

  hdrsrchmsg_lte_bandwidth_enum_type      bandwidth,
    /* Bandwidth of the freq */

  trm_set_client_state_output_data        *set_client_state_output_ptr,
    /* TRM data */

#ifdef FEATURE_HDR_D2L_RF_MEAS
  lte_cphy_irat_meas_rf_param_s           *lte_rf_ptr
    /* LTE RF parameters */
#else
  lte_cphy_irat_meas_startup_rf_params_s  *rf_tuneback_script_ptr
    /* Tune back script to be used when returning to DO */
#endif /* FEATURE_HDR_D2L_RF_MEAS */
)

{
  errno_enum_type                        status = E_SUCCESS;
    /* Command completion status */

/*-----------------------------------------------------------------------*/

  if( ( bandwidth < HDRSRCHMSG_BW_MIN ) ||
      ( bandwidth > HDRSRCHMSG_BW_MAX )
    )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "D2L LTE frequency bandwidth %d out of range",
                    bandwidth );
    status = E_FAILURE;
    return( status );
  }

#ifdef FEATURE_HDR_D2L_RF_MEAS
  if ( lte_rf_ptr == NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "D2L RF param pointer is NULL" );
    status = E_FAILURE;
    return( status );
  }
#endif /* FEATURE_HDR_D2L_RF_MEAS */

  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
                  "D2L/DLR Send TIMED_SEARCH_MEAS_REQ message,earfcn=%d,bw=%d,pridevice=%d,divdevice=%d", 
                  earfcn,                
                  bandwidth,             
                  set_client_state_output_ptr->result.irat_result.band_dev_info[ 0 ].primary_device,
                  set_client_state_output_ptr->result.irat_result.band_dev_info[ 0 ].diversity_device);

  memset( &hdrsrchmsg_d2l_msg.timed_search_meas_req_msg,
          0,
          sizeof( hdrsrchmsg_d2l_msg.timed_search_meas_req_msg ) 
        );
    /* Initialize message */

#ifdef FEATURE_HDR_MSGR_VARIANT 
    msgr_init_hdr_variant( &hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.hdr, 
                   MSGR_HDR_SRCH, 
                   LTE_CPHY_IRAT_MEAS_D2L_TIMED_SRCH_MEAS_REQ,
                   hdrsrch_get_subscriber_id()
                 ); 
#else
  msgr_init_hdr( &hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 LTE_CPHY_IRAT_MEAS_D2L_TIMED_SRCH_MEAS_REQ 
               ); 
#endif /* FEATURE_HDR_MSGR_VARIANT */
    /* Create message header */
                                                              
  /*--------------------------------------------------------------------------
    Build message payload
  --------------------------------------------------------------------------*/

  hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.freq = earfcn;                            

#ifdef FEATURE_HDR_D2L_RF_MEAS
  hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.rf_param = *lte_rf_ptr; 
#else
  if( rf_tuneback_script_ptr != NULL ) 
  {
    hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.rf_params = *rf_tuneback_script_ptr;
      /* Tune back RF script */
  }

  hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.rf_params.lte_device_id = 
    ( uint32 ) set_client_state_output_ptr->result.irat_result.band_dev_info[ 0 ].primary_device;     
  hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.rf_params.lte_div_available = TRUE;
  hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.rf_params.lte_div_device_id = 
    ( uint32 ) set_client_state_output_ptr->result.irat_result.band_dev_info[ 0 ].diversity_device;                      
    /* Frequency to measure and device ID - LTE always use primary and diversity devices */
#endif /* FEATURE_HDR_D2L_RF_MEAS */
                                                              
  hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.dl_bandwith_present = TRUE;
    /* Bandwidth is always provided for D2L */

  switch( bandwidth )        
    /* Bandwidth of frequency to measure */                                  
  {                                                           
    case HDRSRCHMSG_BW_NRB_6:                                 
      hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.dl_bandwidth = LTE_BW_NRB_6;            
      break; 
                                                       
    case HDRSRCHMSG_BW_NRB_15:                                
      hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.dl_bandwidth = LTE_BW_NRB_15;           
      break;         
                                               
    case HDRSRCHMSG_BW_NRB_25:
      hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.dl_bandwidth = LTE_BW_NRB_25;
      break;

    case HDRSRCHMSG_BW_NRB_50:
      hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.dl_bandwidth = LTE_BW_NRB_50;
      break;

    case HDRSRCHMSG_BW_NRB_75:
      hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.dl_bandwidth = LTE_BW_NRB_75;
      break;

    case HDRSRCHMSG_BW_NRB_100:
      hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.dl_bandwidth = LTE_BW_NRB_100;
      break;

    default:
      break;
        /* Do not expect to get here, range checking has been done above */
  }

  hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.gap.asap_gap_start = TRUE;
  hdrsrchmsg_d2l_msg.timed_search_meas_req_msg.gap.time_rat_type = LTE_CPHY_IMEAS_RAT_DO;
    /* Required by LTE */

  if ( hdrsrch_state_id == HDRSRCH_LTE_MEAS_IDLE_STATE )
  {
  status = hdrsrchltemeas_schedule_timed_search ( &hdrsrchmsg_d2l_msg.timed_search_meas_req_msg );
    /* Required API call by LTE, if return failure, do not send request */
  }
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  else if ( hdrsrch_state_id == HDRSRCH_LTE_MEAS_TC_STATE )
  {
    status = hdrsrchtclte_schedule_timed_search ( &hdrsrchmsg_d2l_msg.timed_search_meas_req_msg );
      /* Required API call by LTE, if return failure, do not send request */

  #ifdef FEATURE_HDR_IRAT_CLK_BOOST
    if ( status == E_SUCCESS)
    {
      hdrsrchmcpm_service_boost( HDR_IRAT_D2L_MEAS, 
                                 HDRSRCHMCPM_NO_TRIGGER, 
                                 MCPM_BIMC_BOOST_MASK, 
                                 HDRSRCHMSG_IRAT_CLK_BOOST_DURATION_US);

      HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
        "D2L/DRL HDR state 0x%x, Meas request success, limited CLK boost for duration=%d us", 
        hdrsrch_state_id, HDRSRCHMSG_IRAT_CLK_BOOST_DURATION_US );

    /* for testing only */
      HDR_MSG_PROT_4( MSG_LEGACY_HIGH, "mcpm_service_boost: Boosting clock for Scenario:%d, Scenario trigger:%d, BIMC boost:%d, Duration:%d",
        HDR_IRAT_D2L_MEAS, HDRSRCHMCPM_NO_TRIGGER, MCPM_BIMC_BOOST_MASK, HDRSRCHMSG_IRAT_CLK_BOOST_DURATION_US);
    /* for testing only */
    }
#endif /* FEATURE_HDR_IRAT_CLK_BOOST */
  }
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
  else 
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "D2L/DRL Unexpected call to this function from state %d",
                    hdrsrch_state_id );
    status = E_FAILURE;
  }

  if ( status == E_FAILURE )
  {
    return( status );
  }

  hdrsrchmsg_send_msg( &hdrsrchmsg_d2l_msg.timed_search_meas_req_msg, 
                       sizeof( hdrsrchmsg_d2l_msg.timed_search_meas_req_msg ) 
                     );
    /* Send message */

  return( status );

} /* hdrsrchmsg_d2l_send_timed_search_meas_req_msg */

#endif /* FEATURE_HDR_TO_LTE */

#ifdef FEATURE_HDR_SELECTIVE_TXD
/*===========================================================================

FUNCTION HDRSRCHMSG_SEND_ANT_BLANK_SWITCH_MSG
 
DESCRIPTION
  This function sends msg to FW to do blank/switch of antenna to
  the given position.
 
DEPENDENCIES
  None

PARAMETERS 
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchmsg_send_ant_blank_switch_msg( void )
{
  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Send antenna blank_and_switch_msg" );

  hdrsrchmsg_send_msg( &hdrsrchmsg.ant_switch_msg.hdr, 
                       sizeof( hdrsrchmsg.ant_switch_msg ) 
                     );
    /* Send message */
}
#endif /* FEATURE_HDR_SELECTIVE_TXD */

