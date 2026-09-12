#ifndef M1X_RM_H
#define M1X_RM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  M O D E M   1 X   R E S O U R C E   M G R

GENERAL DESCRIPTION
      This file contains functions for the modem 1x resource manager module.

EXTERNALIZED FUNCTIONS
      m1x_rm_activate
      m1x_rm_deactivate
      m1x_rm_set_state
      m1x_rm_get_state
      m1x_rm_query_capability
      m1x_rm_update_params
      m1x_rm_config_chain
      m1x_rm_preserve_cdma_timing

INITIALIZATION AND SEQUENCING REQUIREMENTS
      Must call m1x_rm_activate() before any other functions in this module
      are invoked.

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Copyright (c) 2011 - 2020 Qualcomm Technologies, Inc. 
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/drivers/inc/m1x_rm.h#4 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/07/20   dmd     FR54762: 1x changes for concurrency Manager
02/06/20   dmd     Add new scenarios for finger enablement
09/15/19   ppr     FR47558 Clock Plan 2.0 Changes
02/12/14   vks     1x mcvs changes for bolt
08/30/13   sst     Add two new voice levels
08/06/13   cjb     Removed configuring DEMBACK_EN register
06/27/13   bb      Changes to shutdown MDSP FW while exiting from system
                   measurement state
05/10/13   cjb     Updated fix for FW shutdown during protocol deactivation.
01/03/13   trc     Remove unused m1x_rm_preserve_cdma_timing() API
03/08/12   adw     Add support for 1x voice advanced Rx settings.
10/24/11   cjb     Changes for IRAT:1x->LTE. Initial working version.
10/04/11   adw     Added support for callback registration.
08/03/11   adw     Support for Lto1x and 1xtoL IRAT requests.
07/18/11   adw     Added parameter update support.
04/27/11   adw     Support pseudo sleep mode.
01/27/11   adw     Added secondary srch client and updated chain type.
01/24/11   adw     Handle invalid initialization cases.
01/24/11   adw     Corrected query capability return type.
01/21/11   adw     Initial revision.

===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "comdef.h"

/* Onex to MCPM Interface */
#include "onex_m1x_rm_mcpm.h"
#include "cmgr_api.h"


/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*-------------------------------------------------------------------------
      Typedefs
-------------------------------------------------------------------------*/
/* Unique identifier for each client of the modem 1x resource mgr module. */
typedef enum
{
  M1X_RM_FIRST_CLIENT,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_SRCH = M1X_RM_FIRST_CLIENT,
  M1X_RM_SRCH_SECONDARY,
  M1X_RM_RX,
  M1X_RM_TX,
  M1X_RM_INTERNAL,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_MAX_CLIENTS
}
m1x_rm_client_type;

/* Unique identifier for each state request that can be made by a client.
   Request arbitration is performed by relying on relative ordering of
   the request states. States must be ordered from lowest to highest
   priority within the enum, with higher value states being supersets
   of lower states. */
typedef enum
{
  M1X_RM_FIRST_STATE,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_1X_INACTIVE = M1X_RM_FIRST_STATE,
  M1X_RM_1X_SLEEP,
  M1X_RM_1X_PSEUDO_SLEEP,
  M1X_RM_1X_WAKEUP,
  M1X_RM_1X_MEAS_LTO1X,
  M1X_RM_1X_IDLE,
  M1X_RM_1X_VOICE,
  M1X_RM_1X_DATA,
  M1X_RM_1X_INIT,
  M1X_RM_1X_MEAS_1XTOL,
  M1X_RM_1X_ALL,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_MAX_STATES
}
m1x_rm_state_type;

/*--------------------------------------------------------------------------
     ONEX to MCPM CLOCK 2.0 SCENARIO ENUM TYPE
--------------------------------------------------------------------------*/
/* Clock 2.0 ONEX Scenarios Enums */
typedef enum
{
  ONEX_M1X_RM_SCENARIO_NONE                   = 0,

  ONEX_M1X_RM_SRCH_TC_1X2X_START_QTA          = 1,

  ONEX_M1X_RM_SRCH_TC_1X2X_EXIT_QTA           = 2,

  ONEX_M1X_RM_SRCH_TC_1X_SRCH_REQ             = 3,

  ONEX_M1X_RM_SRCH_TC_1X_SRCH_REQ_DONE        = 4,

  ONEX_M1X_RM_SCENARIO_1X_VOICE_CALL          = 5,

  ONEX_M1X_RM_SCENARIO_1X_DATA_CALL           = 6,

  ONEX_M1X_RM_SRCH_FING_INIT                  = 7,

  ONEX_M1X_RM_SRCH_FING_NOM                   = ONEX_M1X_RM_SRCH_FING_INIT,
  
  ONEX_M1X_RM_SRCH_FING_MED                   = 8,
  
  ONEX_M1X_RM_SRCH_FING_HIGH                  = 9,
  
  ONEX_M1X_RM_SRCH_FING_NOM_MED               = 10,
  
  ONEX_M1X_RM_SRCH_FING_MED_NOM               = 11,
  
  ONEX_M1X_RM_SRCH_FING_HIGH_NOM              = 12,
  
  ONEX_M1X_RM_SRCH_FING_NOM_HIGH              = 13,
  
  ONEX_M1X_RM_SRCH_FING_MED_HIGH              = 14,
  
  ONEX_M1X_RM_SRCH_FING_HIGH_MED              = 15,

  ONEX_M1X_RM_SCENARIO_MAX

} onex_m1x_rm_scenario_type; 

/* Array used to keep track MCVS Start Requests */
static struct
{
  boolean start_req_sent; /* MCVS Start Req sent to MCPM */
} onex_mcvs_mcpm_req[ ONEX_SCENARIO_MAX ];

/* Unique identifier for resource capability query. */
typedef enum
{
  M1X_RM_FIRST_CAPABILITY,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_DEMOD_CAPABLE = M1X_RM_FIRST_CAPABILITY,
  M1X_RM_VD_CAPABLE,
  M1X_RM_TX_CAPABLE,
  M1X_RM_TD_CAPABLE,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_MAX_CAPABILITIES
}
m1x_rm_capability_type;

/* Unique identifier for chain configuration. */
typedef enum
{
  M1X_RM_FIRST_CHAIN_CONFIG,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_RX_CHAIN_NONE = M1X_RM_FIRST_CHAIN_CONFIG,
  M1X_RM_RX_CHAIN_0,
  M1X_RM_RX_CHAIN_1,
  M1X_RM_RX_CHAIN_BOTH,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_MAX_CHAIN_CONFIGS
}
m1x_rm_chain_type;

/* Unique identifier for callback events for which a client may register. */
typedef enum
{
  M1X_RM_FIRST_CB_EVENT,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_CB_OPTIMIZE_TL_EVENT,
  /* M1X_RM_CB_xxx_EVENT : Add additional events here... */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_MAX_CB_EVENTS
}
m1x_rm_cb_event_type;


/* MCVS Q6 floor request frequency types */
typedef enum
{
  M1X_RM_Q6_REQ_NOM,
  M1X_RM_Q6_REQ_MED,
  M1X_RM_Q6_REQ_HIGH,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_MAX_Q6_REQ
} m1x_rm_q6_freq_req_type;

/* MCVS VPE floor request frequency types */
typedef enum
{
  M1X_RM_VPE_REQ_NOM,
  M1X_RM_VPE_REQ_MED,
  M1X_RM_VPE_REQ_HIGH,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  M1X_RM_MAX_VPE_REQ
} m1x_rm_vpe_freq_req_type;

typedef struct
{
  m1x_rm_q6_freq_req_type  q6_freq;
  m1x_rm_vpe_freq_req_type vpe_freq;
} m1x_rm_mcvs_req_type;

typedef struct
{
  cmgr_scenario_type_e type;
  uint32 scenario_e; /*enum ID defined by tech, used by conc_mgr*/
  uint32 update_scenario_e;
  uint32 priority; /*uint32 number agreed priority for scenario, may change in case of inversion*/
  uint32 flow_ctrl_time_msec; /*time in msec needed for steady state to flow control*/
                              /* sent along with steady state scenario enum*/
                              /*a value of 0xFFFFFFFF means it cannot be flow controlled, i.e. voice call*/
  uint64 start_timestamp; /*uint64 ticks @19.2MHz qurt_sysclock_get_hw_ticks or DalTimetick_GetTimetick64 */
} onex_conc_mgr_config_params_type;

onex_conc_mgr_config_params_type scenario_params;

typedef enum
{
  ONEX_CONCMGR_INVALID_STATE = -1,
  ONEX_CONCMGR_FIRST_STATE = 0,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ONEX_CONCMGR_INACTIVE = ONEX_CONCMGR_FIRST_STATE,
  ONEX_CONCMGR_SLEEP,
  ONEX_CONCMGR_WAKEUP,
  ONEX_CONCMGR_IDLE,
  ONEX_CONCMGR_MEAS_1XTOL,
  ONEX_CONCMGR_VOICE,
  ONEX_CONCMGR_DATA,
  /*Add new steady scenarios here*/

  /*Tramsient scenarios start here*/


  /*Add new transient scenario here*/
  ONEX_CONCMGR_FIRST_TRANSIENT,
  ONEX_CONCMGR_SRCH_FING_INIT = ONEX_CONCMGR_FIRST_TRANSIENT,
  ONEX_CONCMGR_SRCH_FING_NOM = ONEX_CONCMGR_SRCH_FING_INIT,
  ONEX_CONCMGR_SRCH_FING_MED,
  ONEX_CONCMGR_SRCH_FING_HIGH,
  ONEX_CONCMGR_SRCH_FING_NOM_MED,
  ONEX_CONCMGR_SRCH_FING_MED_NOM,
  ONEX_CONCMGR_SRCH_FING_HIGH_NOM,
  ONEX_CONCMGR_SRCH_FING_NOM_HIGH,
  ONEX_CONCMGR_SRCH_FING_MED_HIGH,
  ONEX_CONCMGR_SRCH_FING_HIGH_MED,
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ONEX_CONCMGR_MAX_STATES
}onex_cocnmgr_state_type;

/* Array used to keep track Conc MGR Start Requests */
static struct
{
  boolean start_req_sent; /* Conc MGR Start Req sent */
} onex_concmgr_req[ ONEX_CONCMGR_MAX_STATES ];




/* Callback type */
typedef void ( *m1x_rm_cb_type )( boolean );


/*=============================================================================

                      FUNCTION DECLARATIONS FOR MODULE

=============================================================================*/

/*=============================================================================

FUNCTION       M1X_RM_ACTIVATE
DESCRIPTION    Initialize the modem 1x resource manager module.

DEPENDENCIES   Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_activate( void );

/*=============================================================================

FUNCTION       M1X_RM_DEACTIVATE
DESCRIPTION    De-initialize the modem 1x resource manager module.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_deactivate( void );

/*=============================================================================

FUNCTION       M1X_RM_SET_STATE
DESCRIPTION    Issue a resource request from a particular client, for the
               specified state.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_set_state
(
  m1x_rm_client_type  client,  /* client identifier */
  m1x_rm_state_type   state    /* state being requested */
);

/*=============================================================================

FUNCTION       M1X_RM_GET_STATE
DESCRIPTION    Returns the currently configured resource request state.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   The current resource state that has been requested.

SIDE EFFECTS   None.

=============================================================================*/
m1x_rm_state_type  m1x_rm_get_state( void );

/*=============================================================================

FUNCTION       M1X_RM_GET_MCPM_SCENARIO

DESCRIPTION    Returns the corresponding MCPM scenario type based on input.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   The current resource state that has been requested.

SIDE EFFECTS   None.

=============================================================================*/
onex_m1x_rm_mcpm_scenario_type  m1x_rm_get_mcpm_scenario
( 
  onex_m1x_rm_scenario_type  onex_scenario    /* ONEX Scenario Type */
);

/*=============================================================================

FUNCTION       M1X_RM_SET_MCPM_REQ_STATUS

DESCRIPTION    API Sets the MCPM Request status to passed argument

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_set_mcpm_req_status
( 
  /* ONEX MCPM Scenario Type */
  onex_m1x_rm_mcpm_scenario_type  onex_mcpm_scenario,
  /* Request Status */
  boolean req_status
);

/*=============================================================================

FUNCTION       M1X_RM_GET_MCPM_REQ_STATUS

DESCRIPTION    API returns the MCPM Request status to passed argument

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   TRUE - If MCPM Request Status sent,
               FALSE - Otherwise.

SIDE EFFECTS   None.

=============================================================================*/
boolean m1x_rm_get_mcpm_req_status
( 
  /* ONEX MCPM Scenario Type */
  onex_m1x_rm_mcpm_scenario_type  onex_mcpm_scenario
);

/*=============================================================================

FUNCTION       M1X_RM_SEND_MCPM_STOP_REQ

DESCRIPTION    API used to send MOPM Stop Request for particular MCPM scenario.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.
               Also onex should have sent MCPM Start Request.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_send_mcpm_stop_req
( 
  /* ONEX MCPM Scenario Type */
  onex_m1x_rm_mcpm_scenario_type  onex_mcpm_scenario
);

/*=============================================================================

FUNCTION       M1X_RM_CHECK_AND_SEND_MCPM_STOP_REQ

DESCRIPTION    API used to send MOPM Stop Requests for all the MCPM scenarios, 
               for which onex already sent MCPM Start Requests to MCPM.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.
               Also onex should have sent MCPM Start Request.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_check_and_send_mcpm_stop_req ( void );

/*=============================================================================

FUNCTION       M1X_RM_GET_MCVS_REQ
DESCRIPTION    Returns the currently configured MCVS request.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   The current MCVS request.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_get_mcvs_req (m1x_rm_mcvs_req_type* mcvs_req_ptr);

/*=============================================================================

FUNCTION       M1X_RM_QUERY_CAPABILITY
DESCRIPTION    Query whether the current resource request state provides
               the specified capability.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   TRUE if the capability is supported, FALSE otherwise.

SIDE EFFECTS   None.

=============================================================================*/
boolean  m1x_rm_query_capability
(
  m1x_rm_capability_type  capability  /* capability to query */
);

/*=============================================================================

FUNCTION       M1X_RM_UPDATE_PARAMS
DESCRIPTION    Update the parameters associated with the specified state.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_update_params
(
  m1x_rm_client_type   client,  /* client identifier */
  m1x_rm_state_type    state    /* update params for the specified state */
);

/*=============================================================================

FUNCTION       M1X_RM_UPDATE_MCVS_PARAMS
DESCRIPTION    Update the mcvs parameters.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_update_mcvs_params
(
  m1x_rm_mcvs_req_type *mcvs_req_ptr,    /* MCVS requests */
  onex_m1x_rm_scenario_type onex_scenario /* ONEX Scenario Type */
);

/*=============================================================================

FUNCTION       M1X_RM_CONFIG_CHAIN
DESCRIPTION    Configure resources required by the specified chain.
               This interface is used for both enabling and disabling chain
               based configuration.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_config_chain
(
  m1x_rm_client_type client,  /* client identifier */
  m1x_rm_chain_type  chain    /* chain configuration */
);


/*=============================================================================

FUNCTION       M1X_RM_REGISTER_EVENT_CB
DESCRIPTION    Register a callback with M1X resource manager for a specified
               event. The registered callback will then be invoked when that
               particular event occurs.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_register_event_cb
(
  m1x_rm_cb_event_type  event,  /* specified event */
  m1x_rm_cb_type        cb      /* callback to invoke when event occurs */
);

/*=============================================================================

FUNCTION       M1X_RM_PROTOCOL_SHUTDOWN_CONFIG
DESCRIPTION    API to inform M1X_RM to configure MCPM calls to shutdown MDSP FW
               while goingt to INACTIVE state

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

=============================================================================*/
extern void m1x_rm_protocol_shutdown_config
(
  boolean   enable  /* TRUE => configure for MDSP FW shutdown,
                       FALSE => De-configure MCPM for 1X */
);

/*=============================================================================

FUNCTION       M1X_RM_GET_FING_SCENARIO
DESCRIPTION    This API maps given request to a scenario.

DEPENDENCIES   None

RETURN VALUE   onex_m1x_rm_scenario_type

SIDE EFFECTS   None

=============================================================================*/
onex_m1x_rm_scenario_type m1x_rm_get_fing_scenario
(
  m1x_rm_mcvs_req_type *mcvs_req_data
);

/*=============================================================================

FUNCTION       ONEX_CONCMGR_MAP_FING_SCENARIO
DESCRIPTION    This API maps given request to CONC MGR scenario.

DEPENDENCIES   None

RETURN VALUE   onex_cocnmgr_state_type

SIDE EFFECTS   None

=============================================================================*/

onex_cocnmgr_state_type onex_concmgr_map_fing_scenario
(
  onex_m1x_rm_scenario_type onex_scenario
);

/*=============================================================================

FUNCTION       ONEX_CONCMGR_BACKMAP_FING_SCENARIO
DESCRIPTION    This API maps given CONC MGR scenario to internal scenario type.

DEPENDENCIES   None

RETURN VALUE   onex_m1x_rm_scenario_type

SIDE EFFECTS   None

=============================================================================*/

onex_m1x_rm_scenario_type onex_concmgr_backmap_fing_scenario
(
  onex_cocnmgr_state_type cmgr_scenario
);

/*=============================================================================

FUNCTION       ONEX_CONCMGR_GET_OTHER_TRANS_ACTIVE
DESCRIPTION    This API returns any active transient scenario.

DEPENDENCIES   None

RETURN VALUE   onex_cocnmgr_state_type

SIDE EFFECTS   None

=============================================================================*/

onex_cocnmgr_state_type onex_concmgr_get_other_trans_active( void );

/*=============================================================================

FUNCTION       ONEX_CONCMGR_GET_OTHER_STEADY_ACTIVE
DESCRIPTION    This API returns any active steady scenario.

DEPENDENCIES   None

RETURN VALUE   onex_cocnmgr_state_type

SIDE EFFECTS   None

=============================================================================*/

onex_cocnmgr_state_type onex_concmgr_get_other_steady_active( void );

/*=============================================================================

FUNCTION       ONEX_CB_FUNC_FOR_CONC_MGR
DESCRIPTION    This API is callback that can be called by CONC MGR


DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/

void onex_cb_func_for_conc_mgr
(
  cmgr_tech_type			   tech,	   
  uint32					   scenario, 
  cmgr_scenario_status_e	   status,
  uint32					   return_info
);

/*=============================================================================

FUNCTION       ONEX_CONCMGR_SCENARIO_CONFIG_REQ
DESCRIPTION    This API informs conc mgr about the new scenario and stops older
               scenario.

DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/

void onex_concmgr_scenario_config_req
(
  onex_cocnmgr_state_type scenario, 
  boolean enable
);

/*=============================================================================

FUNCTION       ONEX_CONCMGR_CHECK_AND_SEND_STOP_REQ
DESCRIPTION    This API checks for any active scneario and stops it.


DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/

void onex_concmgr_check_and_send_stop_req ( void );

/*=============================================================================

FUNCTION       ONEX_CONCMGR_GET_REQ
DESCRIPTION    This API returns if a scenario is active or not.


DEPENDENCIES   None

RETURN VALUE   boolean.

SIDE EFFECTS   None

=============================================================================*/

boolean onex_concmgr_get_req 
( 
  onex_cocnmgr_state_type scenario 
);

/*=============================================================================

FUNCTION       ONEX_CONCMGR_SET_REQ
DESCRIPTION    This API sets value for scenario request.


DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/

void onex_concmgr_set_req 
( 
  onex_cocnmgr_state_type scenario, 
  boolean request
);

/*=============================================================================

FUNCTION       ONEX_CONCMGR_SCENARIO_STOP_REQ
DESCRIPTION    This API sends stop request for a given scenario.


DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/

void onex_concmgr_scenario_stop_req
(
  onex_cocnmgr_state_type scenario
);

/*=============================================================================

FUNCTION       ONEX_CONCMGR_MAP_SCENARIO
DESCRIPTION    This API maps internal scenario to CONC MGR scenario.


DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/

uint32 onex_concmgr_map_scenario
(
  onex_cocnmgr_state_type scenario
);
#endif /* M1X_RM_H  */
