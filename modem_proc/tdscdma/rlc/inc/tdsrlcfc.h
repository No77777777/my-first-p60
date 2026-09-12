#ifndef TDSRLCFC_H
#define TDSRLCFC_H
/*===========================================================================

          U P L I N K   R L C : S U B T A S K   H E A D E R   F I L E


GENERAL DESCRIPTION

  This module contains the declaration of the functions and data types
  defined in tdsrlcfc.c.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.
$Header: //components/rel/tdscdma.mpss/4.3/rlc/inc/tdsrlcfc.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/13/14   sarao  CFCM changes for RLC
10/24/10   guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/
#include "tdscdma_variation.h"
#include "tdsuecomdef.h"
#include "tdsrlcul.h"
#include "tdsrlci.h"
#include "cfcm.h"

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL

typedef enum {
  TDSRLC_FC_UPLINK = 0x00,
  TDSRLC_FC_DOWNLINK = 0x01
}tdsrlc_fc_direction_e_type;

typedef enum {
  TDSRLC_FC_TASK_CMD = 0,
  TDSRLC_FC_TIMER_CMD
}tdsrlc_fc_src_cmd_e_type;

typedef enum {
  TDSWS_NO_OP = 0,
  TDSWS_STEP_DOWN,
  TDSWS_STEP_UP,
  TDSWS_FREEZE,
  TDSWS_MAX_OP
} tdsrlci_fc_ws_step_dir_e_type;

typedef enum {
  TDSRLC_FC_FW_CMD_OFF = 0,
  TDSRLC_FC_FW_CMD_ON =1,
  TDSRLC_FC_FW_CMD_UP =1,
  TDSRLC_FC_FW_CMD_DN =1,
  TDSRLC_FC_FW_CMD_NOOP
} tdsrlc_fc_fw_cmd_e_type;

#define TDSRX_WINDOW_MIN_SIZE 1
#define TDSRX_WINDOW_SHUTDOWN_SIZE 0


typedef struct {
  /* Timer for which RLC would act on FC messages perodically. */
  uint16  rlc_fc_status_check_duration;

  /* Intial window size, at the start of flow control. */
  uint16  rlc_fc_init_ws;

  /* Number of steps taken to reach window size. */
  uint16  rlc_fc_ws_stp;

  /* Window size is increased in steps of (WS/RLC_FC_INC_WS_STP).
   This variable holds the current WS step applied on all RLC entity. */
  uint16  rlc_fc_ws_inc;

  /* Either to increase or decrease Peer WS  */
  tdsrlci_fc_ws_step_dir_e_type rlc_fc_ws_step_dir;

  /* Window size stepping timer */
  rex_timer_type  *rlc_fc_step_timer;

  /* TRUE indicates the FC is active in RLC. */
  boolean        rlc_fc_status;

  /* Register FC flag */
  boolean rlc_fc_register_f;

} tdsrlc_fc_type;



typedef struct
{
  q_link_type link;
  cfcm_cmd_e cmd;
  uint32 step_timer;
  uint32 monitors_active;
}tdsrlc_cfcm_cmd_out_type;

#endif

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#define TDSRLC_ACQ_FC_LOCK()          REX_ISR_LOCK(&tdsrlc_fc_sem)
#define TDSRLC_ACQ_FC_UNLOCK()        REX_ISR_UNLOCK(&tdsrlc_fc_sem)
#else
#define TDSRLC_ACQ_FC_LOCK()
#define TDSRLC_ACQ_FC_UNLOCK()
#endif

#define TDSRLC_GET_FC_ARB_PTR(dir, ctrl_blk) (dir) ? &(ctrl_blk->ctrl.am_ctrl.multiclient_dl_fc_arbitarator) : &(ctrl_blk->ctrl.am_ctrl.multiclient_ul_fc_arbitarator)
#define TDSRLC_GET_CFCM_ENTITY(dir) (dir) ? TDSRLC_DL_FC_ENTITY_CFCM : TDSRLC_UL_FC_ENTITY_CFCM

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL

/*===========================================================================

FUNCTION RLC_FC_CMD_HDLR

DESCRIPTION
  Command handler for commands from FC Module.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_fc_cmd_hdlr
(
  tdsrlc_fc_direction_e_type fc_dir  
);


/*===========================================================================

FUNCTION TDSRLC_FC_TIMER_CMD_HDLR

DESCRIPTION
  Command handler for commands from CFCM timer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_fc_timer_cmd_hdlr
(
  tdsrlc_fc_direction_e_type fc_dir
);


/*===========================================================================

FUNCTION RLCI_FC_NEW_WIN_SIZE

DESCRIPTION
  The function calculates the window size under the current cpu load.

DEPENDENCIES
  None.

RETURN VALUE
  Determined window size under current CPU load.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlci_fc_new_win_size
(
  uint16 conf_win_size,
  tdsrlc_fc_direction_e_type fc_dir
);

/*===========================================================================

FUNCTION RLCI_FC_CTL_BLK_RESET_REPORT

DESCRIPTION
  One of the User plane AM hits RESET. Should have no need for flow control
  after RESET. Resume every one in the User Plane to their full config'd
  window size.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_fc_ctl_blk_reset_report
(
  rlc_lc_id_type lc_id,
  tdsrlc_fc_direction_e_type fc_dir
);
/*===========================================================================

FUNCTION RLC_DL_FC_INIT

DESCRIPTION
  Initiliazes DL flow control states in rlc.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_fc_init(void);

/*===========================================================================

FUNCTION RLC_DL_FC_RESET

DESCRIPTION
  Clears DL flow control states in rlc.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_fc_reset(void);


/*===========================================================================

FUNCTION RLC_DL_FC_TX_NEW_WIN_SUFI

DESCRIPTION
  New window size is calculated based on last FC command and tx. new window
  SUFI to that effect.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_dl_fc_tx_new_win_sufi
(
  tdsrlci_ul_ctrl_blk_type *ul_ctrl_blk_ptr /* Logical channel id that has to be flow controled.*/
);

/*===========================================================================

FUNCTION RLCI_DL_FC_STATUS

DESCRIPTION
  Returns RLC-DL flow control status.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlci_dl_fc_status(void);

/*===========================================================================

FUNCTION RLC_DL_SET_FC_CONFIG

DESCRIPTION
  Downlink configuraton set by QxDM.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_set_fc_config
(
  uint16 ws_step_period,
  uint16 ws_step_stage,
  uint16 init_ws
);
/*===========================================================================

FUNCTION RLC_UL_FC_INIT

DESCRIPTION
  Initiliazes UL flow control states in rlc.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_fc_init(void);

/*===========================================================================

FUNCTION RLC_UL_FC_RESET

DESCRIPTION
  Clears UL flow control states in rlc.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_fc_reset(void);

/*===========================================================================

FUNCTION RLCI_UL_FC_CALC_NEW_TX_WIN_SIZE

DESCRIPTION
  This function computes the tx window size that needs to be applied.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_fc_calc_new_tx_win_size
(
  tdsrlci_ul_ctrl_blk_type *ul_ctrl_blk_ptr  /* Logical channel id that has to be flow controled.*/
);

/*===========================================================================

FUNCTION RLCI_UL_FC_STATUS

DESCRIPTION
  Returns RLC-UL flow control status.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlci_ul_fc_status(void);

/*===========================================================================

FUNCTION RLC_UL_SET_FC_CONFIG

DESCRIPTION
  Uplink configuraton set by QxDM.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_set_fc_config
(
  uint16 ws_step_period,
  uint16 ws_step_stage,
  uint16 init_ws
);

#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

/*===========================================================================

FUNCTION MULTI_CLIENT_FC_INIT

DESCRIPTION
  Initialization of the multiclient fc 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsmulti_client_fc_init (
  tdsmulticlient_fc_arbitarator_type *fc_arb_ptr,
  uint16 num_flow_control_entity,
  uint16 configured_max_flow,
  tdsoutput_flow_to_output_trigger_function_type output_fn,
  void *fc_obj
  );

/*===========================================================================

FUNCTION MULTI_CLIENT_FC_REGISTER_CLIENT

DESCRIPTION
  Registration of induvidual client into muliclient FC

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsmulti_client_fc_register_client (
  tdsmulticlient_fc_arbitarator_type *fc_arb_ptr,
  uint16 fc_entity,
  uint16 init_state,
  tdsstate_to_entity_flow_function_type state_to_entity_flow_function);

#if 0
/*===========================================================================

FUNCTION MULTI_CLIENT_FC_DEREGISTER_CLIENT

DESCRIPTION
  deregistration of client from multiclient fc

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsmulti_client_fc_deregister_client (
  tdsmulticlient_fc_arbitarator_type *fc_arb_ptr,
  uint16 fc_entity);
#endif
/*===========================================================================

FUNCTION RLC_MULTI_CLIENT_FC_ARBITRATE

DESCRIPTION
  Multiclient flow control central arbitarator.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsmulti_client_fc_arbitrate(
  tdsmulticlient_fc_arbitarator_type *fc_arb_ptr,
  uint16 fc_entity,
  uint16 new_observed_state,
  boolean lock_fc_semaphore);

/*===========================================================================

FUNCTION MULTI_CLIENT_FC_MODIFY_MAX_FLOW

DESCRIPTION
  configuration of max flow

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsmulti_client_fc_modify_max_flow (
  tdsmulticlient_fc_arbitarator_type *fc_arb_ptr,
  uint16 new_max_flow,
  boolean output_action );
/*===========================================================================

FUNCTION RLC_DL_MULTI_CLIENT_FC_INIT

DESCRIPTION
  rlc fc initialization.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_multi_client_fc_init(
  tdsrlci_ul_am_ctrl_type *ctrl_blk);

/*===========================================================================

FUNCTION RLC_DL_MULTI_CLIENT_FC_OUTPUT_TRIGGER

DESCRIPTION
  The output action function for rlc multiclient fc

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_multi_client_fc_output_trigger(
  uint16 output_flow,
  void *fc_obj);


#if 0
/*===========================================================================

FUNCTION RLC_DL_MULTI_CLIENT_FC_DSM_ENTITY_FLOW_FUNCTION

DESCRIPTION
  State to output flow mapping function for dsm.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_dl_multi_client_fc_dsm_entitiy_flow_function(
  uint16 state, 
  uint16 configured_max_flow);

/*===========================================================================

FUNCTION RLC_DL_MULTI_CLIENT_FC_DUP_ENTITY_FLOW_FUNCTION

DESCRIPTION
  State to output flow mapping for DUP.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_dl_multi_client_fc_dup_entitiy_flow_function(
  uint16 state, 
  uint16 configured_max_flow);
#endif

/*===========================================================================

FUNCTION RLC_MULTI_CLIENT_FC_PS_WM_ENTITY_FLOW_FUNCTION

DESCRIPTION
  State to output flow mapping for PS WM

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_dl_multi_client_fc_ps_wm_entitiy_flow_function(
  uint16 state, 
  uint16 configured_max_flow);

#if defined(FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL)
/*===========================================================================

FUNCTION RLC_MULTI_CLIENT_FC_CFCM_ENTITY_FLOW_FUNCTION

DESCRIPTION
  State to output flow mapping function for CFCM

DEPENDENCIES
  None.

RETURN VALUE
  New Window Size to be set

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_multi_client_fc_cfcm_entitiy_flow_function(
  uint16 state, 
  uint16 configured_max_flow);

#endif

/*===========================================================================

FUNCTION RLC_DL_MULTI_CLIENT_FC_GET_WIN_SIZE

DESCRIPTION
  Mapping from output flow to window size.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlci_dl_multi_client_fc_get_win_size(
  tdsrlci_ul_am_ctrl_type *ctrl_blk);

/*===========================================================================

FUNCTION RLC_DL_MULTI_CLIENT_FC_GET_WIN_SIZE

DESCRIPTION
  Mapping from output flow to window size.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_dl_multi_client_fc_get_win_size(
  uint16 rlc_id);

/*===========================================================================

FUNCTION RLC_MULTI_CLIENT_FC_MODIFY_MAX_FLOW

DESCRIPTION
  Mapping from output flow to window size.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_multi_client_fc_modify_max_flow(
  uint16 rlc_id,
  uint16 new_max_flow,
  boolean output_action
  );

/*===========================================================================

FUNCTION RLC_UL_MULTI_CLIENT_FC_INIT

DESCRIPTION
  rlc fc initialization.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_multi_client_fc_init(
  tdsrlci_ul_am_ctrl_type *ctrl_blk);

/*===========================================================================

FUNCTION RLC_UL_MULTI_CLIENT_FC_OUTPUT_TRIGGER

DESCRIPTION
  The output action function for rlc multiclient fc

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_multi_client_fc_output_trigger(
  uint16 output_flow,
  void *fc_obj);
/*===========================================================================

FUNCTION RLCI_UL_MULTI_CLIENT_FC_GET_WIN_SIZE

DESCRIPTION
  Mapping from output flow to window size.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

uint16 tdsrlci_ul_multi_client_fc_get_win_size(
tdsrlci_ul_am_ctrl_type *am_ctl_blk_ptr);

/*===========================================================================

FUNCTION RLC_UL_MULTI_CLIENT_FC_MODIFY_MAX_FLOW

DESCRIPTION
  rlc ul wrapper function to modify max flow.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_multi_client_fc_modify_max_flow(
  uint16 rlc_id,
  uint16 new_max_flow,
  boolean output_action
  );
#ifdef FEATURE_TDSCDMA_CFCM
/*===========================================================================

FUNCTION TDSRLC_CFCM_REGISTER

DESCRIPTION
  Register UL and DL with the CFCM module

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_cfcm_register(void);

/*===========================================================================

FUNCTION TDSRLC_CFCM_DEREGISTER

DESCRIPTION
  Deregister UL and DL with the CFCM module

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_cfcm_deregister(void);

/*===========================================================================

FUNCTION TDSRLC_UL_CFCM_CB

DESCRIPTION
  CFCM Callback Function for UL client

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_cfcm_cb(cfcm_cmd_type_s *cfcm_cmd);

/*===========================================================================

FUNCTION RLC_DL_CFCM_CB

DESCRIPTION
  CFCM Callback Function for DL client

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_cfcm_cb(cfcm_cmd_type_s *cfcm_cmd);
#endif //FEATURE_TDSCDMA_CFCM
/*===========================================================================

FUNCTION TDSRLC_CFCM_PROC_SHUT_DOWN_COMMAND

DESCRIPTION
  Processes SHUT DOWN command received from CFCM module

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_cfcm_proc_shut_down_command(tdsrlc_fc_direction_e_type dir, uint32 monitors_active);
 
/*===========================================================================

FUNCTION TDSRLC_PROC_CFCM_COMMAND

DESCRIPTION
  Sets the UL/DL window size to the value passed as win_size

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_proc_cfcm_command(tdsrlc_fc_direction_e_type dir, uint8 win_size);




#if 0
#ifdef FEATURE_TDSCDMA_TEMPERATURE_FC
/*===========================================================================

FUNCTION RLC_MULTI_CLIENT_FC_TMEPERATURE_ENTITY_FLOW_FUNCTION

DESCRIPTION
  State to output flow mapping for Temperature

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_multi_client_fc_temperature_entitiy_flow_function(
  uint16 state, 
  uint16 configured_max_flow);

/*===========================================================================

FUNCTION RLC_TEMPERATURE_FC_INIT

DESCRIPTION
  Initialization function of temperature flow control

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_temperature_fc_init( void );

/*===========================================================================

FUNCTION RLC_TEMPERATURE_FC_STEP_TIMER_CB

DESCRIPTION
  RLC temperature flowcontrol's step timer  call back

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_temperature_fc_step_timer_cb( unsigned long cb_val );

/*===========================================================================

FUNCTION RLC_TEMPERATURE_FC_DEM_OK_CB

DESCRIPTION
  RLC temperature fc, dem okay call back 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_temperature_fc_dem_okay_cb( void );

/*===========================================================================

FUNCTION RLC_TEMPERATURE_FC_DEM_WARNING_CB

DESCRIPTION
  RLC temperature fc, dem warning call back 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_temperature_fc_dem_warning_cb( void );

/*===========================================================================

FUNCTION RLC_TEMPERATURE_FC_DEM_CRITICAL_CB

DESCRIPTION
  RLC temperature fc, dem critical call back 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_temperature_fc_dem_critical_cb( void );

/*===========================================================================

FUNCTION RLC_TEMPERATURE_PROCESS_TIMER_SIG

DESCRIPTION
  RLC temperature flowcontrol's step timer  signal processing function

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_temperature_fc_process_timer_sig( void );

/*===========================================================================

FUNCTION RLC_TEMPERATURE_PROCESS_DEM_SIG

DESCRIPTION
  RLC temperature FC dem sig processing function

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_temperature_fc_process_dem_sig( void );

#ifdef FEATURE_TDSCDMA_NPA_THERMAL
/*===========================================================================

FUNCTION TDSRLC_INITIALIZE_TEMPERATURE_EVENT_CB

DESCRIPTION
  Registers temperature change event when NPA Resource is available

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_intialize_temperature_event_cb
(
  void          *context,
  unsigned int  event_type, 
  void          *data,       
  unsigned int  data_size 
);
 
/*===========================================================================

FUNCTION TDSRLC_TEMPERATURE_CHANGE_EVENT_HANDLER

DESCRIPTION
  Handles temperature change events by NPA

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_temperature_change_event_handler
(
  void          *context,
  unsigned int  event_type, 
  void          *data,       
  unsigned int  data_size 
);
#endif

#endif
#endif
#endif  /* TDSRLCFC_H */

