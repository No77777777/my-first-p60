/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          GL1 MSGR Interface Initialisation

GENERAL DESCRIPTION
   This module initialises GSM Msgr IF task.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmsgrif/src/gl1_msgrif_init.c#3 $ $DateTime: 2020/07/28 11:01:41 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------- 
24/07/20   sal      CR2736787 Added G2X cleanup done handler to issue rf_meas_exit immediately on cleanup confirmation.
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
29/10/17   km      CR2133917 GL1 support for L+L
27/07/16   zf      CR1046704: Using MSGR variant
15/08/15   zf      CR887132: Use variant for IRAT CM_GSM_INIT MSGs.
20/08/15   npt     CR878988 GL1 NV control of GFW recovery features
14/08/15   hd      CR889027: Register G2W MSGR message registration at the stack start
05/08/15   hd      CR884936: Handle QTA STARTUP and CLEANUP messages in MSGR IF
29/07/15   dp/dv   CR878618: Changes to wait for rf exit confirmation in gl1 for X2W srch
01/07/15   jj      CR864036 avoid registering WCDMA IRAT message when it is already registered 
25/06/15   sp      CR856664: Register G2W IRAT messenger handlers on GSM Start mode request by using correct AS ID
18/06/15   jj      CR856718 modify the assert in irat message register so that 
                             it wont crah if tried to register second attempt 
11/06/15   zf      CR847878: Using PRE_SRCH_REQ and CNF for G2W
01/06/15   pjr     CR846441 Replaced constant NUM_GERAN_GFW_ROUTER_MSGS with NUM_GERAN_FW_MSGS
05/06/15   dp      CR833596 Making use of MSGR support of multi-instances of SUBs using MSGR variant    
15/04/15   pa      CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
29/04/15   npt     CR804870 RF timeline error recovery mechanism
27/04/15   ws      CR815011 Featurise cfcm_cpu_monitor handling uder FEATURE_DUAL_DATA
09/04/15   jj      CR819956 Thor 2.x RF API change, G2W use MSGR
08/04/15   pjr     CR818535 FR25951 Changes to support Single Voice Session 
16/03/15   pg      CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
11/03/15   pjr     CR806250 Fix featurisation for changes submitted under CR787199
24/02/15   pjr     CR787199 Acquire vocoder immediately after IMS releases it during srvcc handover
27/02/15   am      CR646584 WLAN coex
21/01/15   jj      CR784462 remove FEATURE_THOR_G2X_MSGR_REWORK 
17/10/14   jj      CR741205 moving  G2X CNF from l1 task to MSGR 
11/02/14   jk      CR614254:GL1 changes to support IP2 calibration
30/05/14   jj      CR656854  Resolve Critical Klocwork errors in gdrivers code space
07/04/14   cja     CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
19/03/14   cja     CR632037 Use RF Task for X2G
25/02/14   cos     CR608951 GERAN RFLM FED: Pass subscription ID to GERAN RF in all message router payloads
15/02/14   ip/npt  CR547955: Define multi thread usage on wake up from sleep
01/11/13   pa      CR569692: X2G via RF Task.
14/10/13   pa      CR536820: Added GL1 Msgr If Task. Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "gl1_msgrif_init.h"
#include "customer.h"
#include "geran_tasks.h"
#include "rex_types.h"
#include "rcinit.h"
#include "msg.h"
#include "msgr.h"
#include "msgr_rex.h"
#include "msgr_geran.h"
#include "gl1_msgrif_task.h"
#include "gl1_msgrif_os.h"
#include "geran_dual_sim_g.h"
#include "assert.h"
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
#include "wl1x2wsrchapi.h"
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */


/*
 * MAKE SURE NUM_GERAN_FW_MSGS IS UPDATED WHEN CHANGING gfw_async_intf_msgr_umid_list
 */
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
#define NUM_GERAN_FW_MSGS   7
#else
#define NUM_GERAN_FW_MSGS   6
#endif

static msgr_umid_type gfw_async_intf_msgr_umid_list[NUM_GERAN_DATA_SPACES][NUM_GERAN_FW_MSGS] =
{
  {
    GERAN_FW_ASYNC_RSP,
    GERAN_FW_SLEEP_READY_CMD,
    GERAN_FW_RFM_EXIT_MODE_RSP,
    GERAN_FW_WLAN_CXM_POLICY_RSP,
    GERAN_FW_CCS_STATUS_CMD
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
	  ,GERAN_FW_INVALID_RFLM_TX_INDEX_RSP
#endif
   ,GERAN_FW_CLEANUP_DONE_CMD
  }
#if defined (FEATURE_DUAL_SIM)
  ,
  {
    GERAN_FW2_ASYNC_RSP,
    GERAN_FW2_SLEEP_READY_CMD,
    GERAN_FW2_RFM_EXIT_MODE_RSP,
    GERAN_FW2_WLAN_CXM_POLICY_RSP,
    GERAN_FW2_CCS_STATUS_CMD
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
	  ,GERAN_FW2_INVALID_RFLM_TX_INDEX_RSP
#endif
   ,GERAN_FW2_CLEANUP_DONE_CMD
  }
#if defined ( FEATURE_TRIPLE_SIM )
  ,
  {
    GERAN_FW3_ASYNC_RSP,
    GERAN_FW3_SLEEP_READY_CMD,
    GERAN_FW3_RFM_EXIT_MODE_RSP,
    GERAN_FW3_WLAN_CXM_POLICY_RSP,
    GERAN_FW3_CCS_STATUS_CMD
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
	  ,GERAN_FW3_INVALID_RFLM_TX_INDEX_RSP
#endif
   ,GERAN_FW3_CLEANUP_DONE_CMD
  }
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM*/
};


static msgr_id_t        irat_msgr_id[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE(NULL)};
static msgr_client_t    g2x_msgr_client[NUM_GERAN_DATA_SPACES];


static msgr_umid_type gl1_msgrif_rf_task_msgr_umid_list[NUM_GERAN_DATA_SPACES][NUM_GERAN_RF_TASK_MSGR_MSGS] =
{
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    RFA_RF_GSM_RX_WAKEUP_CNF,
    RFA_RF_GSM_TX_WAKEUP_CNF,
#else
    RFA_RF_GSM_WAKEUP_CNF,
    RFA_RF_GSM_TX_ENABLE_CNF,
#endif
    RFA_RF_GSM_RX_BURST_CNF,
    RFA_RF_GSM_SET_TX_BAND_CNF,
    RFA_RF_GSM_TX_BURST_CNF
#ifdef GERAN_L1_IP2CAL
    ,RFA_RF_GSM_START_IP2_CAL_CNF
#endif
#ifdef GERAN_L1_HLLL_LNASTATE
    ,RFA_RF_GSM_PROCESS_BURST_METRICS_CNF
#endif
  }
#if defined (FEATURE_DUAL_SIM)
  ,
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    RFA_RF_GSM_RX_WAKEUP_SUB2_CNF,
    RFA_RF_GSM_TX_WAKEUP_SUB2_CNF,
#else
    RFA_RF_GSM_WAKEUP_SUB2_CNF,
    RFA_RF_GSM_TX_ENABLE_SUB2_CNF,
#endif
    RFA_RF_GSM_RX_BURST_SUB2_CNF,
    RFA_RF_GSM_SET_TX_BAND_SUB2_CNF,
    RFA_RF_GSM_TX_BURST_SUB2_CNF
#ifdef GERAN_L1_IP2CAL
    ,RFA_RF_GSM_START_IP2_CAL_SUB2_CNF
#endif
#ifdef GERAN_L1_HLLL_LNASTATE
    ,RFA_RF_GSM_PROCESS_BURST_METRICS_SUB2_CNF
#endif
  }
#if defined ( FEATURE_TRIPLE_SIM )
  ,
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    RFA_RF_GSM_RX_WAKEUP_SUB3_CNF,
    RFA_RF_GSM_TX_WAKEUP_SUB3_CNF,
#else
    RFA_RF_GSM_WAKEUP_SUB3_CNF,
    RFA_RF_GSM_TX_ENABLE_SUB3_CNF,
#endif
    RFA_RF_GSM_RX_BURST_SUB3_CNF,
    RFA_RF_GSM_SET_TX_BAND_SUB3_CNF,
    RFA_RF_GSM_TX_BURST_SUB3_CNF
#ifdef GERAN_L1_IP2CAL
    ,RFA_RF_GSM_START_IP2_CAL_SUB3_CNF,
#endif
#ifdef GERAN_L1_HLLL_LNASTATE
    ,RFA_RF_GSM_PROCESS_BURST_METRICS_SUB3_CNF
#endif
  }
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM*/
};
#define NUM_GERAN_RF_TASK_MSGS (sizeof(gl1_msgrif_rf_task_msgr_umid_list[0]) / sizeof(gl1_msgrif_rf_task_msgr_umid_list[0][0]))


/*===========================================================================

                     FUNCTIONS DEFINITION

===========================================================================*/

/*===========================================================================

FUNCTION  get_msgr_tech_module

DESCRIPTION
  This function return tech module based on gas id  
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static msgr_tech_module_type get_msgr_tech_module(gas_id_t gas_id)
{
  msgr_tech_module_type msgr_tech_module = 0;
  switch (gas_id)
  {
  case GERAN_ACCESS_STRATUM_ID_1:
  {
    msgr_tech_module = MSGR_GERAN_GL1_MSGRIF_TASK_1;
  }
  break;

  #ifdef FEATURE_DUAL_SIM
  case GERAN_ACCESS_STRATUM_ID_2:
  {
    msgr_tech_module = MSGR_GERAN_GL1_MSGRIF_TASK_2;
  }
  break;
  #endif /*FEATURE_DUAL_SIM*/

  #if defined ( FEATURE_TRIPLE_SIM )
  case GERAN_ACCESS_STRATUM_ID_3:
  {
    msgr_tech_module = MSGR_GERAN_GL1_MSGRIF_TASK_3;
  }
  break;
  #endif /* FEATURE_TRIPLE_SIM */

  case GERAN_ACCESS_STRATUM_ID_UNDEFINED:
  default:
  {
    MSG_GERAN_FATAL_1_G("Unassigned gas id %d", gas_id);
  }
  } /* end switch */
  
  return msgr_tech_module; 
}

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
/*===========================================================================

FUNCTION  teardown_LTE_irat_messages

DESCRIPTION
  This function deregister LTE CNF with message router 
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void teardown_LTE_irat_messages(gas_id_t gas_id)
{
  errno_enum_type  msgr_error;
  msgr_tech_module_type msgr_tech_module = get_msgr_tech_module(gas_id);
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);
 
  #ifdef  FEATURE_GSM_TO_LTE
  /* @@TODO: Check with Scott if the req and cnf messages can be rearranged to meet
   * the range requirements for the function call below.
   */
  msgr_error = msgr_deregister_range_variant(msgr_tech_module,
                                     &g2x_msgr_client[gas_id],
                                     LTE_CPHY_IRAT_MEAS_G2L_INIT_CNF,
                                     LTE_CPHY_IRAT_MEAS_G2L_BUILD_RF_SCRIPT_CNF,
                                     (uint8)as_id );
  ASSERT(msgr_error == E_SUCCESS);
  #endif  
}

/*===========================================================================

FUNCTION  teardown_TDSCDMA_irat_messages

DESCRIPTION
  This function deregister TDSCDMA CNF with message router 
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void teardown_TDSCDMA_irat_messages(gas_id_t gas_id)
{
  errno_enum_type msgr_error;
  msgr_tech_module_type msgr_tech_module = get_msgr_tech_module(gas_id);
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);
  
  #ifdef FEATURE_GSM_TDS
  msgr_error = msgr_deregister_variant(msgr_tech_module,
                               &g2x_msgr_client[gas_id],
                                       TDSCDMA_IRAT_INIT_CNF,
                                       (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_deregister_variant(msgr_tech_module,
                               &g2x_msgr_client[gas_id],
                                       TDSCDMA_IRAT_ACQ_RSP,
                                       (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_deregister_variant(msgr_tech_module,
                               &g2x_msgr_client[gas_id],
                                       TDSCDMA_IRAT_MEAS_RSP,
                                       (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_deregister_variant(msgr_tech_module,
                               &g2x_msgr_client[gas_id],
                                       TDSCDMA_IRAT_STOP_CNF,
                                       (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_deregister_variant(msgr_tech_module,
                               &g2x_msgr_client[gas_id],
                                       TDSCDMA_IRAT_ABORT_CNF,
                                       (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);
  #endif /* FEATURE_GSM_TDS */
}

#endif /* !FEATURE_GSM_TO_WCDMA_MSGR*/

/*===========================================================================

FUNCTION  register_LTE_irat_messages

DESCRIPTION
  This function register with message route to receive  CNF from LTE
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void register_LTE_irat_messages(gas_id_t gas_id)
{
  errno_enum_type msgr_error;
  msgr_tech_module_type msgr_tech_module = get_msgr_tech_module(gas_id);
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  #ifdef  FEATURE_GSM_TO_LTE
  /* @@TODO: Check with Scott if the req and cnf messages can be rearranged to meet
   * the range requirements for the function call below.
   */
  msgr_error = msgr_register_range_variant(msgr_tech_module,
                                   &g2x_msgr_client[gas_id],
                                   irat_msgr_id[gas_id],
                                   LTE_CPHY_IRAT_MEAS_G2L_INIT_CNF,
                                   LTE_CPHY_IRAT_MEAS_G2L_BUILD_RF_SCRIPT_CNF,
                                           (uint8)as_id);
  if ( msgr_error == E_ALREADY_DONE )
  {
     MSG_GERAN_HIGH_0(" GL1 LTE message already registered ");
  }
  else
  {
     ASSERT(msgr_error == E_SUCCESS); 
  }

  #endif
}
/*===========================================================================

FUNCTION  register_TDSCDMA_irat_messages

DESCRIPTION
  This function register with message route to receive  CNF from TDSCDMA
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void register_TDSCDMA_irat_messages (gas_id_t gas_id)
{
  errno_enum_type msgr_error;
  msgr_tech_module_type msgr_tech_module = get_msgr_tech_module(gas_id);
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);
   
  #ifdef FEATURE_GSM_TDS
  msgr_error = msgr_register_variant(msgr_tech_module,
                             &g2x_msgr_client[gas_id],
                             irat_msgr_id[gas_id],
                                     TDSCDMA_IRAT_INIT_CNF,
                                     (uint8)as_id);
  if ( msgr_error == E_ALREADY_DONE )
  {
     MSG_GERAN_HIGH_0(" GL1 TDSCDMA message already registered ");
     return;
  }
  else
  {
     ASSERT(msgr_error == E_SUCCESS); 
  }

  msgr_error = msgr_register_variant(msgr_tech_module,
                             &g2x_msgr_client[gas_id],
                             irat_msgr_id[gas_id],
                                     TDSCDMA_IRAT_ACQ_RSP,
                                     (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_register_variant(msgr_tech_module,
                             &g2x_msgr_client[gas_id],
                             irat_msgr_id[gas_id],
                                     TDSCDMA_IRAT_MEAS_RSP,
                                     (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_register_variant(msgr_tech_module,
                             &g2x_msgr_client[gas_id],
                             irat_msgr_id[gas_id],
                                     TDSCDMA_IRAT_STOP_CNF,
                                     (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_register_variant(msgr_tech_module,
                             &g2x_msgr_client[gas_id],
                             irat_msgr_id[gas_id],
                                     TDSCDMA_IRAT_ABORT_CNF,
                                     (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);
  #endif /* FEATURE_GSM_TDS */
}

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
/*===========================================================================

FUNCTION  register_WCDMA_irat_messages

DESCRIPTION
  This function register with message route to receive CNF messages from WL1. 
  
  Notice it registers with the "_variant" registration function, meaning only 
  messages sent from this sub's wl1 module will reach the handling code on this
  sub. There is no cross-talk.
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void register_WCDMA_irat_messages(gas_id_t gas_id)
{
  errno_enum_type msgr_error;
  msgr_tech_module_type msgr_tech_module = get_msgr_tech_module(gas_id);
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  MSG_GERAN_HIGH_1_G("registering G2W IRAT Msg handles on as_id=%d",as_id);
  	 
  msgr_error = msgr_register_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    irat_msgr_id[gas_id],
                                    WCDMA_IRAT_G2W_INIT_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_register_variant(msgr_tech_module, 
                                    &g2x_msgr_client[gas_id],
                                    irat_msgr_id[gas_id],
                                    WCDMA_IRAT_G2W_PRE_SRCH_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_register_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    irat_msgr_id[gas_id],
                                    WCDMA_IRAT_G2W_SRCH_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);     
  
  msgr_error = msgr_register_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    irat_msgr_id[gas_id],
                                    WCDMA_IRAT_G2W_ABORT_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);
  
  msgr_error = msgr_register_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    irat_msgr_id[gas_id], 
                                    WCDMA_IRAT_G2W_DEINIT_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);
  
  msgr_error = msgr_register_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    irat_msgr_id[gas_id], 
                                    WCDMA_IRAT_G2W_RF_EXIT_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  /* Register QTA messages here */

  msgr_error = msgr_register_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    irat_msgr_id[gas_id], 
                                    GERAN_GL1_QTA_STARTUP_REQ,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_register_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    irat_msgr_id[gas_id], 
                                    GERAN_GL1_QTA_CLEANUP_REQ,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);
}

/*===========================================================================

FUNCTION  deregister_WCDMA_irat_messages

DESCRIPTION
  This function de-register with message route to stop receive CNF messages from WL1. 
  
  Notice it registers with the "_variant" registration function, meaning only 
  messages sent from this sub's wl1 module will reach the handling code on this
  sub. There is no cross-talk.
  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void deregister_WCDMA_irat_messages(gas_id_t gas_id)
{
  errno_enum_type msgr_error;
  msgr_tech_module_type msgr_tech_module = get_msgr_tech_module(gas_id);
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);

  MSG_GERAN_HIGH_1_G("Deregistering G2W handles on as_id=%d",as_id);
  msgr_error = msgr_deregister_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    WCDMA_IRAT_G2W_INIT_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);
  
  msgr_error = msgr_deregister_variant(msgr_tech_module, 
                                    &g2x_msgr_client[gas_id],
                                    WCDMA_IRAT_G2W_PRE_SRCH_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_deregister_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    WCDMA_IRAT_G2W_SRCH_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);     
  
  msgr_error = msgr_deregister_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    WCDMA_IRAT_G2W_ABORT_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);
  
  msgr_error = msgr_deregister_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    WCDMA_IRAT_G2W_DEINIT_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_deregister_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    WCDMA_IRAT_G2W_RF_EXIT_CNF,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  /* De-register QTA messages here */

  msgr_error = msgr_deregister_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    GERAN_GL1_QTA_STARTUP_REQ,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_deregister_variant(msgr_tech_module,
                                    &g2x_msgr_client[gas_id],
                                    GERAN_GL1_QTA_CLEANUP_REQ,
                                    (uint8)as_id);
  ASSERT(msgr_error == E_SUCCESS);

}

#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

/*===========================================================================

FUNCTION  gl1_msgrif_initialise

DESCRIPTION
  This function initialise MSGR IF task and initialise interface with
  RF and FW task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_msgrif_initialise (gas_id_t gas_id)
{
   /* Is REX critical section and mutex initialisation required?*/

  switch ( gas_id )
  {
  case GERAN_ACCESS_STRATUM_ID_1:
   {
    geran_tcb_set( gl1_msgrif_task_id[gas_id], rex_self());
   }
    break;
#ifdef FEATURE_DUAL_SIM
  case GERAN_ACCESS_STRATUM_ID_2:
   {
    geran_tcb_set( gl1_msgrif_task_id[gas_id], rex_self());
   }
    break;
#endif /*FEATURE_DUAL_SIM*/
#if defined ( FEATURE_TRIPLE_SIM )
  case GERAN_ACCESS_STRATUM_ID_3:
   {
    geran_tcb_set( gl1_msgrif_task_id[gas_id], rex_self());
   }
    break;
#endif /* FEATURE_TRIPLE_SIM */

  case GERAN_ACCESS_STRATUM_ID_UNDEFINED:
  default:
   {
    MSG_GERAN_FATAL_0_G( "Invalid gas_id" );
   }

    break;
  }

  gl1_msgrif_init_rf_task_msgr_intf(gas_id);
  gl1_msgrif_init_async_msgr_intf(gas_id);

  gl1_msgrif_init_G2X_msgr_intf(gas_id);

  if(GAS_ID_IS_MULTIMODE_SUB(gas_id))
  {  
    gl1_msgrif_init_vfr_map_msgr_intf(gas_id);
  }
   
#ifdef FEATURE_DUAL_DATA     
  if(GAS_ID_IS_MULTIMODE_SUB(gas_id))
  {  
   gl1_msgrif_init_cfcm_cpu_monitor_msgr_intf( gas_id);
  }
#endif
  return;
}

/*===========================================================================

FUNCTION  gl1_msgrif_init_rf_task_msgr_intf

DESCRIPTION
  This function registers with message router to receive non blocking CNFs from
  RF Task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static msgr_client_t    rf_task_msgr_client[NUM_GERAN_DATA_SPACES];

void gl1_msgrif_init_rf_task_msgr_intf(gas_id_t gas_id)
{
   errno_enum_type  msgr_error;
   msgr_id_t        msgr_id;
   msgr_tech_module_type msgr_tech_module = 0;
   uint32  i = 0;

   (void) q_init( &gl1_msgrif_rf_task_msgr_q[gas_id] );
   (void) q_init( &gl1_msgrif_rf_task_msgr_free_q[gas_id] );

   while( i < GL1_MSGRIF_RF_TASK_MSGR_QUEUE_SIZE )
   {
     msgr_hdr_struct_type*  tmp_ptr = (msgr_hdr_struct_type *) &(gl1_msgrif_rf_task_msgr[gas_id][i].msg.hdr);
     q_link_type*           link_item_ptr = q_link( tmp_ptr, &(gl1_msgrif_rf_task_msgr[gas_id][i].link) );
     q_put( &gl1_msgrif_rf_task_msgr_free_q[gas_id], link_item_ptr );

     i++;
   }

   /* Register the queue and free queue with the message router */
   msgr_error = msgr_client_create(&rf_task_msgr_client[gas_id]);

   ASSERT(msgr_error == E_SUCCESS);

   msgr_error = msgr_client_add_rex_q(
     &rf_task_msgr_client[gas_id],                        //!< Message Client Handle
     gl1_msgrif_task_tcb_read(gas_id),                          //!< REX TCB pointer
     GL1_MSGRIF_RF_TASK_SIG,                //!< REX TCB pointer
     &gl1_msgrif_rf_task_msgr_q[gas_id],              //!< queue to place the msg
     &gl1_msgrif_rf_task_msgr_free_q[gas_id],         //!< queue to get empty buffer
     offsetof(gl1_msgrif_rf_task_msgr_t, msg), //!< offset to msgr_hdr
     MSGR_NO_OFFSET,                      //!< offset to legacy cmd_type
     sizeof(gl1_msgrif_rf_task_msgr_t),        //!< max size of msgs
     &msgr_id                             //!< queue identifier
   );


   ASSERT(msgr_error == E_SUCCESS);

   switch ( gas_id )
   {
   case GERAN_ACCESS_STRATUM_ID_1:
    {
      msgr_tech_module = MSGR_GERAN_GL1_MSGRIF_TASK_1;

      for (i = 0; i < NUM_GERAN_RF_TASK_MSGS; i++)
      {
        msgr_error = msgr_register(msgr_tech_module,
                                   &rf_task_msgr_client[gas_id],
                                   msgr_id,
                                   gl1_msgrif_rf_task_msgr_umid_list[gas_id][i]);
      }
    }
    break;
#ifdef FEATURE_DUAL_SIM
   case GERAN_ACCESS_STRATUM_ID_2:
    {
      msgr_tech_module = MSGR_GERAN_GL1_MSGRIF_TASK_2;

      for (i = 0; i < NUM_GERAN_RF_TASK_MSGS; i++)
      {
        msgr_error = msgr_register(msgr_tech_module,
                                   &rf_task_msgr_client[gas_id],
                                   msgr_id,
                                   gl1_msgrif_rf_task_msgr_umid_list[gas_id][i]);
      }
    }
    break;
#endif /*FEATURE_DUAL_SIM*/
#if defined ( FEATURE_TRIPLE_SIM )
   case GERAN_ACCESS_STRATUM_ID_3:
    {
      msgr_tech_module = MSGR_GERAN_GL1_MSGRIF_TASK_3;

      for (i = 0; i < NUM_GERAN_RF_TASK_MSGS; i++)
      {
        msgr_error = msgr_register(msgr_tech_module,
                                   &rf_task_msgr_client[gas_id],
                                   msgr_id,
                                   gl1_msgrif_rf_task_msgr_umid_list[gas_id][i]);
      }
    }
    break;
#endif /* FEATURE_TRIPLE_SIM */

   case GERAN_ACCESS_STRATUM_ID_UNDEFINED:
   default:
    {
      MSG_GERAN_FATAL_1_G("Unassigned gas id %d", gas_id);
    }
    break;
   }

   ASSERT(msgr_error == E_SUCCESS);
   return;
}

/*===========================================================================

FUNCTION  gl1_msgrif_init_async_msgr_intf

DESCRIPTION
  This function register with message route to receive non blocking CNF from
  FW.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static msgr_client_t    async_msgr_client[NUM_GERAN_DATA_SPACES];

void gl1_msgrif_init_async_msgr_intf(gas_id_t gas_id)
{
   errno_enum_type  msgr_error;
   msgr_id_t        msgr_id;
   msgr_tech_module_type msgr_tech_module = 0;

   uint32  i = 0;
   (void) q_init( &gl1_msgrif_gfw_async_message_router_q[gas_id] );
   (void) q_init( &gl1_msgrif_gfw_async_message_router_free_q[gas_id] );

   i = 0;

   while(i < GL1_MSGRIF_GFW_ASYNC_MSGR_QUEUE_SIZE)
   {
     msgr_hdr_struct_type*  tmp_ptr   = (msgr_hdr_struct_type *) &(gl1_msgrif_gfw_async_msgr[gas_id][i].msg.hdr);
     q_link_type*           link_item_ptr = q_link( tmp_ptr, &(gl1_msgrif_gfw_async_msgr[gas_id][i].link) );
     q_put( &gl1_msgrif_gfw_async_message_router_free_q[gas_id], link_item_ptr );
     i++;
   }

   /* Register the queue and free queue with the message router */
   msgr_error = msgr_client_create(&async_msgr_client[gas_id]);

   ASSERT(msgr_error == E_SUCCESS);

   msgr_error = msgr_client_add_rex_q(
     &async_msgr_client[gas_id],                              //!< Message Client Handle
     gl1_msgrif_task_tcb_read(gas_id),                                //!< REX TCB pointer
     GL1_MSGRIF_GFW_ASYNC_INTF_ROUTER_SIG,         //!< REX TCB pointer
     &gl1_msgrif_gfw_async_message_router_q[gas_id],           //!< queue to place the msg
     &gl1_msgrif_gfw_async_message_router_free_q[gas_id],      //!< queue to get empty buffer
     offsetof(gl1_msgrif_gfw_async_msgr_t, msg), //!< offset to msgr_hdr
     MSGR_NO_OFFSET,                            //!< offset to legacy cmd_type
     sizeof(gl1_msgrif_gfw_async_msgr_t),        //!< max size of msgs
     &msgr_id                                   //!< queue identifier
   );

   ASSERT(msgr_error == E_SUCCESS);

   for (i = 0; i < NUM_GERAN_FW_MSGS; i++)
   {

    switch ( gas_id )
    {
    case GERAN_ACCESS_STRATUM_ID_1:
    {
    msgr_tech_module = MSGR_GERAN_GL1_MSGRIF_TASK_1;
    }
    break;
#ifdef FEATURE_DUAL_SIM
    case GERAN_ACCESS_STRATUM_ID_2:
    {
    msgr_tech_module = MSGR_GERAN_GL1_MSGRIF_TASK_2;
    }
    break;
#endif /*FEATURE_DUAL_SIM*/
#if defined ( FEATURE_TRIPLE_SIM )
    case GERAN_ACCESS_STRATUM_ID_3:
    {
    msgr_tech_module = MSGR_GERAN_GL1_MSGRIF_TASK_3;
    }
    break;
#endif /* FEATURE_TRIPLE_SIM */

    case GERAN_ACCESS_STRATUM_ID_UNDEFINED:
    default:
    {
      MSG_GERAN_FATAL_1_G("Unassigned gas id %d", gas_id);
    }
    break;
    }

    /* Correct range must be maintinaed within RF Task MSGR API
       Actual range required will need to be checked and updated */
    msgr_error = msgr_register(
      msgr_tech_module,
      &async_msgr_client[gas_id],
      msgr_id,
      gfw_async_intf_msgr_umid_list[gas_id][i]
      );

      ASSERT(msgr_error == E_SUCCESS);
    }
   return;
}

#if defined (FEATURE_GSM_TO_LTE) || defined (FEATURE_GSM_TDS) || defined (FEATURE_GSM_TO_WCDMA_MSGR)
/*===========================================================================

FUNCTION  gl1_msgrif_init_G2X_msgr_intf

DESCRIPTION
  This function register with message route to receive non blocking CNF from
  FW.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_msgrif_init_G2X_msgr_intf(gas_id_t gas_id)
{
   errno_enum_type  msgr_error;
   msgr_id_t        msgr_id;
   uint32  i = 0;
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);
   msgr_tech_module_type msgr_tech_module = get_msgr_tech_module(gas_id );

   (void) q_init( &gl1_msgrif_g2x_message_router_q[gas_id] );
   (void) q_init( &gl1_msgrif_g2x_message_router_free_q[gas_id] );

   i = 0;
   while(i < GL1_MSGR_QUEUE_SIZE)
   {
     msgr_hdr_struct_type*  tmp_ptr   = (msgr_hdr_struct_type *) &(gl1_msgrif_g2x_msgr[gas_id][i].msg.hdr);
     q_link_type*           link_item_ptr = q_link( tmp_ptr, &(gl1_msgrif_g2x_msgr[gas_id][i].link) );
     q_put( &gl1_msgrif_g2x_message_router_free_q[gas_id], link_item_ptr );
     i++;
   }

   /* Register the queue and free queue with the message router */
   msgr_error = msgr_client_create(&g2x_msgr_client[gas_id]);

   ASSERT(msgr_error == E_SUCCESS);

   msgr_error = msgr_client_add_rex_q(
     &g2x_msgr_client[gas_id],                                       //!< Message Client Handle
     gl1_msgrif_task_tcb_read(gas_id),                   //!< REX TCB pointer
     GL1_MSGRIF_G2X_INTF_ROUTER_SIG,               //!< REX TCB pointer
     &gl1_msgrif_g2x_message_router_q[gas_id],     //!< queue to place the msg
     &gl1_msgrif_g2x_message_router_free_q[gas_id],//!< queue to get empty buffer
     offsetof(geran_gl1_msgr_t, msg),              //!< offset to msgr_hdr
     MSGR_NO_OFFSET,                               //!< offset to legacy cmd_type
     sizeof(geran_gl1_msgr_t),                     //!< max size of msgs
     &msgr_id                                      //!< queue identifier
   );

   ASSERT(msgr_error == E_SUCCESS);
   irat_msgr_id[gas_id] = msgr_id ;

   msgr_error = msgr_register_variant(msgr_tech_module,
                                     &g2x_msgr_client[gas_id],
                                     msgr_id,
                                     GERAN_GL1_X2G_IRAT_CM_GSM_INIT_REQ,
                                     (uint8)as_id);
   ASSERT(msgr_error == E_SUCCESS);

#ifndef FEATURE_GSM_TO_WCDMA_MSGR
  if(GAS_ID_IS_MULTIMODE_SUB(gas_id))
  {
#ifdef  FEATURE_GSM_TO_LTE
    register_LTE_irat_messages(gas_id);
#endif
#ifdef FEATURE_GSM_TDS
    register_TDSCDMA_irat_messages(gas_id);
#endif /* FEATURE_GSM_TDS */
  }
#endif /*FEATURE_GSM_TO_WCDMA_MSGR */
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
    register_WCDMA_irat_messages (gas_id);
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
}
#endif /* (FEATURE_GSM_TO_LTE) || defined(FEATURE_GSM_TDS) || defined (FEATURE_GSM_TO_WCDMA_MSGR) */

static msgr_client_t    vfrmap_msgr_client[NUM_GERAN_DATA_SPACES];
static msgr_id_t        vfrmap_msgr_id[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE(NULL)};

void gl1_msgrif_init_vfr_map_msgr_intf(gas_id_t gas_id)
{
   errno_enum_type msgr_error;
   msgr_id_t msgr_id;
   msgr_hdr_struct_type* tmp_ptr;
   q_link_type* link_item_ptr;
   uint32  i = 0;
   
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(gas_id);
   msgr_tech_module_type msgr_tech_module = get_msgr_tech_module(gas_id );

   (void) q_init( &gl1_msgrif_vfrmap_message_router_q[gas_id] );
   (void) q_init( &gl1_msgrif_vfrmap_message_router_free_q[gas_id] );


   i = 0;
   while(i < GL1_MSGR_QUEUE_SIZE)
   {
     msgr_hdr_struct_type*  tmp_ptr   = (msgr_hdr_struct_type *) &(gl1_msgrif_vfrmap_msgr[gas_id][i].msg.msg_hdr);
     q_link_type*           link_item_ptr = q_link( tmp_ptr, &(gl1_msgrif_vfrmap_msgr[gas_id][i].link) );
     q_put( &gl1_msgrif_vfrmap_message_router_free_q[gas_id], link_item_ptr );
     i++;
   }


   /* Register the queue and free queue with the message router */
   msgr_error = msgr_client_create(&vfrmap_msgr_client[gas_id]);

   ASSERT(msgr_error == E_SUCCESS);

   msgr_error = msgr_client_add_rex_q(
         &vfrmap_msgr_client[gas_id],                       //!< Message Client Handle
         gl1_msgrif_task_tcb_read(gas_id),                  //!< REX TCB pointer
         GL1_MGSRIF_VFRMAP_ROUTER_SIG,                      //!< REX TCB pointer
         &gl1_msgrif_vfrmap_message_router_q[gas_id],       //!< queue to place the msg
         &gl1_msgrif_vfrmap_message_router_free_q[gas_id],  //!< queue to get empty buffer
         offsetof(geran_gl1_vfrmap_msgr_t, msg),            //!< offset to msgr_hdr
         MSGR_NO_OFFSET,                                    //!< offset to legacy cmd_type
         sizeof(geran_gl1_vfrmap_msgr_t),                   //!< max size of msgs
         &msgr_id                                           //!< queue identifier
         );

       ASSERT(msgr_error == E_SUCCESS);
       vfrmap_msgr_id[gas_id] = msgr_id ;

       msgr_error = msgr_register(msgr_tech_module,
                                  &vfrmap_msgr_client[gas_id],
                                  msgr_id,
                                  MM_MMOC_VFR_IND);

       ASSERT(msgr_error == E_SUCCESS);

}

/*===========================================================================

FUNCTION  gl1_msgrif_init_cfcm_cpu_monitor_msgr_intf

DESCRIPTION
 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_DUAL_DATA

static msgr_client_t    cfcm_cpu_monitor_msgr_client[NUM_GERAN_DATA_SPACES];
void gl1_msgrif_init_cfcm_cpu_monitor_msgr_intf(gas_id_t gas_id)
{
   errno_enum_type  msgr_error;
   msgr_id_t        msgr_id;
   uint32  i = 0;
   cfcm_reg_req_type_s client_req;

   msgr_tech_module_type msgr_tech_module = get_msgr_tech_module (gas_id );

  (void) q_init( &gl1_msgrif_cfcm_cpu_monitor_router_q[gas_id] );
  (void) q_init( &gl1_msgrif_cfcm_cpu_monitor_router_free_q[gas_id] );

  i = 0;
  while(i < GL1_MSGR_QUEUE_SIZE)
  {
    msgr_hdr_struct_type*  tmp_ptr   = (msgr_hdr_struct_type *) &(gl1_msgrif_cfcm_cpu_monitor_msgr[gas_id][i].msg.hdr);
    q_link_type*           link_item_ptr = q_link( tmp_ptr, &(gl1_msgrif_cfcm_cpu_monitor_msgr[gas_id][i].link) );
    q_put( &gl1_msgrif_cfcm_cpu_monitor_router_free_q[gas_id], link_item_ptr );
    i++;
  }

  /* Register the queue and free queue with the message router */
  msgr_error = msgr_client_create(&cfcm_cpu_monitor_msgr_client[gas_id]);

  ASSERT(msgr_error == E_SUCCESS);

  msgr_error = msgr_client_add_rex_q(
    &cfcm_cpu_monitor_msgr_client[gas_id],                                       //!< Message Client Handle
    gl1_msgrif_task_tcb_read(gas_id),                   //!< REX TCB pointer
    GL1_MSGRIF_CFCM_CPU_MONITOR_SIG,                   //!< REX TCB pointer
    &gl1_msgrif_cfcm_cpu_monitor_router_q[gas_id],     //!< queue to place the msg
    &gl1_msgrif_cfcm_cpu_monitor_router_free_q[gas_id],//!< queue to get empty buffer
    offsetof(geran_gl1_msgr_t, msg),                   //!< offset to msgr_hdr
    MSGR_NO_OFFSET,                                    //!< offset to legacy cmd_type
    sizeof(geran_gl1_msgr_t),                          //!< max size of msgs
    &msgr_id                                           //!< queue identifier
  );

  ASSERT(msgr_error == E_SUCCESS);

     msgr_error = msgr_register(
       msgr_tech_module,
       &cfcm_cpu_monitor_msgr_client[gas_id],
       msgr_id,
       GERAN_GL1_CFCM_CPU_MONITOR_IND
       );
     ASSERT(msgr_error == E_SUCCESS);

    client_req.client_id    = CFCM_CLIENT_GSM;
    client_req.monitor_mask = CFCM_MONITOR_MASK_CPU;
    client_req.req_cb       = NULL;
    client_req.req_umid = GERAN_GL1_CFCM_CPU_MONITOR_IND;
    cfcm_client_register(&client_req);



}
#endif /* FEATURE_DUAL_DATA */

void gl1_msgrif_init_teardown(gas_id_t gas_id)
{
  errno_enum_type  msgr_error;

  msgr_error = msgr_client_delete(&rf_task_msgr_client[gas_id]);
  ASSERT(msgr_error == E_SUCCESS);
  msgr_error = msgr_client_delete(&async_msgr_client[gas_id]);
  ASSERT(msgr_error == E_SUCCESS);
#ifdef FEATURE_THOR_G2X_MSGR_REWORK
  msgr_error = msgr_client_delete(&g2x_msgr_client[gas_id]);
  ASSERT(msgr_error == E_SUCCESS);
#ifdef FEATURE_DUAL_DATA
  msgr_error = msgr_client_delete(&cfcm_cpu_monitor_msgr_client[gas_id]);
  ASSERT(msgr_error == E_SUCCESS);
#endif /* FEATURE_DUAL_DATA */
#endif
  msgr_error = msgr_client_delete(&vfrmap_msgr_client[gas_id]);
  ASSERT(msgr_error == E_SUCCESS);

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
  deregister_WCDMA_irat_messages (gas_id);
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

}

/* EOF */
