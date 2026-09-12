#ifndef CMDBG_QSH_EXT_H
#define CMDBG_QSH_EXT_H
/*===========================================================================

     C A L L   M A N A G E R   D E B U G   H E A D E R   F I L E

DESCRIPTION
  This header file contains debug macros and definitions necessary to
  interface with cmdbg_qsh.c


Copyright (c) 1991 - 2015 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/



/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmdbg_qsh.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/09/15   KC      Initial release.

===========================================================================*/

#include "comdef.h"    /* Definition for basic types and macros */
#include "qsh.h"
#include "cmcall.h"
#include "cm_qsh_ext.h"

#ifndef CM_DEBUG
/* keeps the API in uncompressed section in th elf
 ** this will be used by QSH dump collection ,
 ** since after the crash happened uncompressed functions
 ** compressed functions wont be accesible */
#define CM_API_IN_UNCOMPRESSED_IN_ELF __attribute__((section(".uncompressible.text")))
#else
#error code not present
#endif


typedef enum cm_qsh_metrics_chg_type_e
{
  CM_QSH_METRICS_CHG_TYPE_NONE = -1,
  CM_QSH_METRICS_CHG_TYPE_SS_EVENT,
  CM_QSH_METRICS_CHG_TYPE_PH_EVENT,
  CM_QSH_METRICS_CHG_TYPE_SEND_INITIAL_METRIC,
  CM_QSH_METRICS_CHG_TYPE_MAX
} cm_qsh_metrics_chg_type;

typedef struct cmdbg_rssi_sys_mode_info_s {
  int16               rssi;
  sys_sys_mode_e_type sys_mode;
  uint8               counter;
  /* This is incremented if diff between previous RSSI and current RSSI is > +-20 */

}cmdbg_rssi_sys_mode_info_s_type;

/* holds all the information that is associated with QSH */
typedef struct cmdbg_qsh_s  {
  qsh_client_metric_cfg_s metric_cfg[SYS_MODEM_AS_ID_MAX][CM_QSH_METRIC_MAX];
  boolean event_action[CM_QSH_EVENT_MAX];
  boolean events_enabled_state[MAX_AS_IDS][CM_QSH_EVENT_MAX];
  cmdbg_rssi_sys_mode_info_s_type rssi_sys_mode_info[SYS_MODEM_AS_ID_MAX][CM_NO_STACKS];
}cmdbg_qsh_s_type;

cmdbg_qsh_s_type  *cmdbg_qsh_ptr
( void
)CM_API_IN_UNCOMPRESSED_IN_ELF;

/*===========================================================================

FUNCTION cmdbg_qsh_chk_and_send_srv_evts

DESCRIPTION
  Sends srv change events to QSH

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void             cmdbg_qsh_chk_and_send_srv_evts
(sys_modem_as_id_e_type as_id);


/*===========================================================================

FUNCTION cmdbg_qsh_send_call_evts

DESCRIPTION
  Sends call start/end/drop/setup failed events to QSH

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void                                 cmdbg_qsh_send_call_evts
(

  cm_call_event_e_type    call_event,
  /* notify client list of this call event */

  cmcall_s_type    *call_ptr,
  /* pointer to a call object. */

  cm_mm_call_info_s_type   *call_info_ptr
  /* Call information pointer */


);


/*===========================================================================

FUNCTION cmdbg_qsh_send_volte_call_start_end_evts

DESCRIPTION
  Sends VOLTE call start/end events to QSH

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void                                 cmdbg_qsh_send_volte_call_start_end_evts
(

  const cm_ext_volte_call_state_ind_s_type    *volte_state
  /* VOLTE state indication */

);


/*===========================================================================

FUNCTION cmdbg_qsh_send_call_end_evts

DESCRIPTION
  Send call end events to QSH

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void                                 cmdbg_qsh_send_call_end_evts
(

  cmcall_s_type          *call_ptr,
  /* pointer to a call object. */

  uint8                  *start_addr
  /* start address of metric */

);


/*===========================================================================

FUNCTION cmdbg_chk_and_update_srv_metrics

DESCRIPTION
  Updates the metrics buffer and sends to QSH

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void  cmdbg_chk_and_update_srv_metrics
(sys_modem_as_id_e_type as_id,
 /* AS id */
 cm_qsh_metrics_chg_type chg_type
 /* densotes which metric changed */
);


/*===========================================================================

FUNCTION cmdbg_chk_and_update_serving_cell_metrics

DESCRIPTION
  Updates the serving cell metrics buffer and sends to QSH

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void  cmdbg_chk_and_update_serving_cell_metrics(

  sys_modem_as_id_e_type as_id,
  /* AS id */

  multimode_stack_e_type  stk_id
  /* ss where the info changed */

);


/*===========================================================================

FUNCTION cmdbg_chk_and_update_call_end_info_metrics

DESCRIPTION
  Updates the call end metrics buffer and sends to QSH

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void  cmdbg_chk_and_update_call_end_info_metrics(

  cm_call_event_e_type    call_event,
  /* the call event */

  cmcall_s_type          *call_ptr
  /* pointer to a call object. */

);

/*===========================================================================

FUNCTION cmdbg_qsh_determine_3gpp_call_end_evt_type

DESCRIPTION
  determine the call end event from either cause or rej type. Only applicable for 3GPP calls.
  To determine the event:
    call helper functions to determine if event is NORMAL, RX_DISC, TX_DISC, RX_RELEASE
    if reject type is present and value is AS_REJECT:
      map it to event RX_RRC_RELEASE
    if neither cause or rej type can be mapped, map to event OTHERS

DEPENDENCIES
  none

RETURN VALUE
  the call end event, determined from either cause or rej type

SIDE EFFECTS
  none

===========================================================================*/
sys_call_end_event_e_type cmdbg_qsh_determine_3gpp_call_end_evt_type(

  cmcall_s_type    *call_ptr

);

/*===========================================================================

FUNCTION cmdbg_qsh_determine_3gpp2_call_end_evt_type

DESCRIPTION
  Determine the call end event for CDMA CS calls. ( this API does not support HDR )
  The orig failures reported by 1xcp are converted to call end status in CM.
  This call end status can be classified into 3 categories using this API
  a) RLF_OR_WEAK_SIGNAL
  b) NORMAL
  c) OTHERS


DEPENDENCIES
  none

RETURN VALUE
  the call end event type, determined from call_ptr->end_status

SIDE EFFECTS
  none

===========================================================================*/
sys_call_end_event_e_type cmdbg_qsh_determine_3gpp2_call_end_evt_type(

  cmcall_s_type    *call_ptr
  /* pointer to call object */

);

/*===========================================================================

FUNCTION cmdbg_qsh_determine_ims_call_end_evt_type

DESCRIPTION
DEPENDENCIES
  none

RETURN VALUE
  the call end event type, determined from call_ptr->end_status

SIDE EFFECTS
  none

===========================================================================*/
sys_call_end_event_e_type cmdbg_qsh_determine_ims_call_end_evt_type(

  cmcall_s_type    *call_ptr
  /* pointer to call object */

);

/*===========================================================================

FUNCTION cmdbg_qsh_generic_rpt_proc

DESCRIPTION
  Process Generic reports in CMSS


DEPENDENCIES
  cm qsh object must have already been initialized with
  cmdbg_qsh_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmdbg_qsh_generic_rpt_proc(

  const cm_hdr_type   *rpt_ptr
  /* Pointer to generic reports */
);

/*===========================================================================

FUNCTION cmdbg_qsh_init

DESCRIPTION
  Initilize CM QSH interface.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void  cmdbg_qsh_init(void);

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*===========================================================================

FUNCTION cmdbg_qsh_send_call_progress_delay_evts

DESCRIPTION
  Send call end event to QSH (the event is CM_QSH_EVENT_MO_CALL_DELAY).

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void                                 cmdbg_qsh_send_call_progress_delay_evts(void);

/*===========================================================================

FUNCTION cmdbg_qsh_enable_cust_events

DESCRIPTION
  Enable customer events without any commands

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void  cmdbg_qsh_enable_cust_events(void);

/*===========================================================================

FUNCTION cmdbg_qsh_send_mplmn_abort_evts

DESCRIPTION
  Send call end event to QSH (the event is CM_QSH_EVENT_MO_CALL_DELAY).

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void                                 cmdbg_qsh_send_mplmn_abort_evts
(

  sys_modem_as_id_e_type            asubs_id
  /* network list cnf asubs id */,

  reg_cm_network_list_status_e_type status
  /* status of network list cnf */
);



/*===========================================================================

FUNCTION cmdbg_qsh_send_dds_change_evts

DESCRIPTION
  Send DDS change event to QSH (the event is CM_QSH_EVENT_DDS_CHANGE).

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void                                 cmdbg_qsh_send_dds_change_evts(void);

/*===========================================================================

FUNCTION cmdbg_qsh_send_standby_pref_change_evts

DESCRIPTION
  Send standby pref change event to QSH (the event is CM_QSH_EVENT_DDS_CHANGE).

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void                                 cmdbg_qsh_send_standby_pref_change_evts(void);

/*===========================================================================

FUNCTION cmdbg_qsh_update_rssi_fluctuations

DESCRIPTION
  updates RSSI fluctuation statistics

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void                                 cmdbg_qsh_update_rssi_fluctuations
(

  cm_mm_msim_ss_info_s_type    *ss_info_ptr

);

/*===========================================================================

FUNCTION cmdbg_qsh_send_xxx_evts

DESCRIPTION
  Send QSH event to QSH

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void                                 cmdbg_qsh_send_xxx_evts(cm_qsh_event_e evt, sys_modem_as_id_e_type as_id);


#endif
/*===========================================================================

FUNCTION cmdbg_qsh_reset_rssi_fluctations

DESCRIPTION
  Reset RSSi fluctations statistics

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmdbg_qsh_reset_rssi_fluctations(void);

/*===========================================================================

FUNCTION cmdbg_qsh_evt_is_evt_or_dbg_any_enabled

DESCRIPTION
  Checks whether a particular QSH event is enabled on provided asubs_id

DEPENDENCIES
  none

RETURN VALUE
  True/False

SIDE EFFECTS
  none

===========================================================================*/
boolean cmdbg_qsh_evt_is_evt_or_dbg_any_enabled
(
  cm_qsh_event_e         evt,
  sys_modem_as_id_e_type as_id
);


/*===========================================================================

FUNCTION cmdbg_qsh_evt_chg_to_debug_any_if_evt_disabled

DESCRIPTION
  Change to QSH_EVENT_FIELD_DEBUG_ANY_FAILURE if given event is disabled

DEPENDENCIES
  none

RETURN VALUE
  True/False

SIDE EFFECTS
  none

===========================================================================*/
cm_qsh_event_e cmdbg_qsh_evt_chg_to_debug_any_if_evt_disabled
(
  cm_qsh_event_e         evt,
  sys_modem_as_id_e_type as_id
);
/*===========================================================================

FUNCTION cmdbg_is_failure_event

DESCRIPTION
  Evaluates whether given event is a failure event or not.

DEPENDENCIES
  none

RETURN VALUE
  True/False

SIDE EFFECTS
  none

===========================================================================*/
boolean cmdbg_is_failure_event(cm_qsh_event_e         evt);
/*===========================================================================

FUNCTION cmdbg_print_QSH_evt_f3

DESCRIPTION
  Prints F3

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmdbg_print_QSH_evt_f3(cm_qsh_event_e         evt, sys_modem_as_id_e_type as_id);

/*===========================================================================

FUNCTION cmdbg_qsh_is_3gpp_cs_call_end_normal

DESCRIPTION
  helper to determine if 3gpp call end is normal.
  criteria for normal call end:
  -call end reason is CLIENT_END
  -call end reason is NETWORK_END (meaning CM got MT_DISC_IND), with cause code between 1 - 31
  -no MT_DISC_IND and only MT_CALL_END_IND (call end reason is not CLIENT_END or NETWORK_END) with cause 17
  -MT_DISC_IND, followed by MT_CALL_END_IND with cause 17

DEPENDENCIES
  none

RETURN VALUE
  TRUE, if call end is normal call end. FALSE otherwise

SIDE EFFECTS
  none

===========================================================================*/
boolean cmdbg_qsh_is_3gpp_cs_call_end_normal(

  cmcall_s_type    *call_ptr
  /* pointer to call object */

);

/*===========================================================================

FUNCTION cmdbg_qsh_is_call_end_normal

DESCRIPTION
  helper API to determine if call end is normal.
  criteria for normal call end:
  1. call end reason is CLIENT_END ( for all calls)
  Note: 2 is not applicable to 3GPP calls
  2. call end reason is REL_NORMAL ( network release reason for 3GPP2 / IP calls but not for 3GPP calls)
  #ifdef FEATURE_CUST_1
  Note: 3 is only applicable for GWT CS Calls
  3. 3. call end reason is NETWORK_END (meaning CM got MT_DISC_IND. only for 3GPP calls), with cause code between 16 (NORMAL CALL CLEARING)
  #else
  Note: 3, 4,5 are only applicable for 3GPP CS calls
  3. call end reason is NETWORK_END (meaning CM got MT_DISC_IND. only for 3GPP calls), with cause code between 1 - 30
  4. no MT_DISC_IND and only MT_CALL_END_IND (call end reason is not CLIENT_END or NETWORK_END) with cause 17
  5. MT_DISC_IND, followed by MT_CALL_END_IND with cause 17
  #endif
  6. If call is of type GWT PS and released by the network and conditions 8 and 9
     dont apply
  7. If call mode info is LTE and end status is NONE
     And neither conditions a or b are met.
     a) lte_info->esm_cause.valid is TRUE and
         esm_cause is not NAS_ESM_REGULAR_DEACTIVATION
     b) esm_cause.valid is FALSE and
        esm_local_cause.valid is TRUE and
        esm_local_cause.local_cause is DRB_RELEASED_AT_RRC,
        ESM_SYNC_UP_WITH_NW ,EMM_ATTACH_STARTED or ESM_DS_REJECTED_THE_CALL
  8. if abnormal failure flag is set , treat as abnormal failure.
     This flag is set for GW PS calls when :
     a) PDP ACTIVATE REJ happens
     b) CM_PDP_DEACTIVATE_IND with cause NETWORK and reason not REGULAR_DEACTIVATION

  9. check if it has been marked as a normal call end due to
     a)CM_PDP_DEACTIVATE_REJ
     b) CM_PDP_DEACTIVATE_IND with cause type INTERNAL
     c) CM_PDP_DEACTIVATE_IND with cause NETWORK and reason REGULAR_DEACTIVATION

DEPENDENCIES
  none

RETURN VALUE
  TRUE, if call end is normal call end. FALSE otherwise

SIDE EFFECTS
  none

===========================================================================*/
boolean cmdbg_qsh_is_call_end_normal(

  cmcall_s_type    *call_ptr
  /* pointer to call object */

);
#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION cmdbg_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for CM module.

DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
void cmdbg_qsh_mdump_collect();

/*===========================================================================

FUNCTION cmdbg_qsh_enable_rare_events

DESCRIPTION
  By default enable in frequent evetns.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void  cmdbg_qsh_enable_rare_events(void);

#endif

#endif /* CM_QSH_EXT_H */

