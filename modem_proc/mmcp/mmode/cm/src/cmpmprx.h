#ifndef CMPMPRX_H
#define CMPMPRX_H

/*===========================================================================

          P M P R O X Y    H E A D E R    F I L E

DESCRIPTION
  This file contains all the definitions necessary for the Call Manager ( CM )
  to interface with the Policy Manager module.

Copyright (c) 2002 - 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR FILE

   This section contains comments describing changes made to this file.
   Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmpmprx.h#1 $

when       who     what, where, why
--------   ---    -------------------------------------------------------
04/29/13   ak     Initial release.

===========================================================================*/


#ifndef EXTERN
#define EXTERN
#endif

#include "policyman.h"
#include "policyman_msg.h"
#include "policyman_cm.h"
#include "sys.h"
#include "cmi.h"
#include "cmcall.h"
#include "mcfg_refresh.h"

#define PM_CHG_CHANGE_NONE  0
#define PM_CHG_SVC_SCAN          BM(0)
#define PM_CHG_MODE_CAP          BM(1)
#define PM_CHG_BAND_CAP          BM(2)
#define PM_CHG_UE_MODE           BM(3)
#define PM_CHG_DEVICE_MODE       BM(4)
#define PM_CHG_VOICE_DOMAIN_PREF BM(5)
#define PM_CHG_MAX_ACTIVE_DATA   BM(6)
#define PM_CHG_RAT_ACQ_ORDER     BM(7)
#define PM_CHG_CALL_MODE         BM(8)
#define PM_CHG_MAX_MODE_CAP      BM(9)
#define PM_CHG_DOMAIN_CAP        BM(10)


#define PM_CHG_ALL        ( PM_CHG_SVC_SCAN|\
                            PM_CHG_MODE_CAP|\
                            PM_CHG_BAND_CAP|\
                            PM_CHG_UE_MODE |\
                            PM_CHG_VOICE_DOMAIN_PREF|\
                            PM_CHG_RAT_ACQ_ORDER|\
                            PM_CHG_CALL_MODE|\
                            PM_CHG_DOMAIN_CAP)

#define PM_CHG_DEVICE_ALL ( PM_CHG_MAX_ACTIVE_DATA|\
                            PM_CHG_DEVICE_MODE|\
                            PM_CHG_MAX_MODE_CAP )

#define PM_SUB_FMODE_MASK_SVLTE
#define PM_SUB_FMODE_MASK_SGLTE
#define PM_SUB_FMODE_MASK_NORMAL

typedef uint32  policy_change_mask;
typedef uint32  policy_sub_fmode_mask;

typedef enum cm_policy_subsc_change_e
{
  CMPMPRX_SUBSC_NOT_AVAIL,

  CMPMPRX_SUBSC_AVAIL,

  CMPMPRX_SUBSC_NO_CHANGE
} cm_policy_subsc_change_e_type;

typedef struct
{
  boolean   gwl_subsc;
  boolean   cdma_subsc;
} cm_policy_elig_s_type;

typedef struct
{
  size_t                            nSims;
  size_t                            nMaxActive;
  size_t                            nMaxActiveData;
} cm_policy_device_mode_s_type;

typedef struct
{
  sys_subs_feature_t            sub_feature_mode;

  policyman_svc_mode_t          service_scan_mode;

  sys_ue_mode_e_type            ue_operation_mode;

  uint32                        policy_mode_capability;
  sys_srv_domain_e_type         policy_domain_capability;
  sys_band_mask_e_type          policy_band_capability;
  sys_lte_band_mask_e_type      policy_lte_band_capability;
  sys_band_mask_e_type          policy_tds_band_capability;
  sys_voice_domain_pref_e_type  voice_domain_pref;
  boolean                       is_voice_dom_pref_locked;
  cm_acq_pri_order_pref_s_type  acq_order_pref;
  sys_call_mode_t             call_mode;
  uint32                      policy_max_mode_capability;
  /* to hold specialization value from PM that tells is Volte only mode for
     * E911 calls is enabled */
  boolean                       special_volte_e911_calls;
  
} cm_policy_items_s_type;

typedef struct
{
  uint32                      pm_max_mode_cap;
  policyman_status_t          max_cap_return;
  boolean                     is_sub_feature_valid;
  sys_subs_feature_t          init_sub_feature_mode;

} cm_policy_debug_s_type;

typedef struct sub_level
{

  cm_policy_elig_s_type         is_cm_eligible;
  policy_change_mask            policy_changes;
  cm_policy_items_s_type        policy_items;
  cm_policy_debug_s_type        debug_var;

} cm_sub_level_policy_s_type;

typedef struct policy
{

  policy_change_mask            device_policy_changes;
  boolean                       is_pm_suspend;
  sys_overall_feature_t         feature_mode;
  cm_policy_device_mode_s_type  device_mode;
  
#ifdef FEATURE_MODEM_CONFIG_REFRESH

  mcfg_refresh_status_e_type    mcfg_refresh_status;
#endif
  boolean                       dbg_is_init_policy_valid;
  size_t                        seqno;
  cm_sub_level_policy_s_type    *sub_policy[MAX_SIMS];

} cm_policy_config_s_type;


void cmpmprx_init(boolean is_mcfg_refresh, uint8 refresh_subs_mask);

/*===========================================================================

FUNCTION cmpmprx_update_cm_policy_consumption_eligibility

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmpmprx_update_cm_policy_consumption_eligibility_subsc(

  sys_modem_as_id_e_type          asubs_id,

  cm_policy_subsc_change_e_type   gwl_subsc,

  cm_policy_subsc_change_e_type   cdma_subsc
);
/*===========================================================================

cmpmprx_compute_fullrat_status

DESCRIPTION
  This function returns true when we are in full rat

DEPENDENCIES
  none

RETURN VALUE
sd_ss_fullrat_pref_e_type

SIDE EFFECTS
  none

===========================================================================*/

extern sd_ss_fullrat_pref_e_type cmpmprx_compute_fullrat_status
(
  mm_sub_stk_id_s_type mm_id,
  cm_network_sel_mode_pref_e_type  cm_net_sel_mode_pref,
  cm_sub_level_policy_s_type      *pm_sub_ptr
);

/*===========================================================================

FUNCTION cmpmprx_enforce_policy_oprt_mode

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean                           cmpmprx_enforce_policy_oprt_mode(

  sys_modem_as_id_e_type     asubs_id

);

/*===========================================================================

FUNCTION cmpmprx_read_cm_policy_consumption_eligibility

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_read_cm_policy_consumption_eligibility(

  sys_modem_as_id_e_type     asubs_id

);

/*===========================================================================

FUNCTION cmpmprx_is_mask_set

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern boolean                    cmpmprx_is_mask_set(

  sys_modem_as_id_e_type     asubs_id,

  policy_change_mask         check_mask
);

/*===========================================================================

FUNCTION cmpmprx_is_device_mask_set

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern boolean                    cmpmprx_is_device_mask_set(

  policy_change_mask         check_mask,

  cm_policy_config_s_type   *pm_ptr
);

/*===========================================================================

FUNCTION cmpmprx_policy_chg_ind_proc

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void                       cmpmprx_policy_chg_ind_proc(

  cm_policyman_cfg_s         *rpt_ptr
  /* Pointer to the report sent by Message Router */
);

/*===========================================================================

FUNCTION cmpmprx_fetch_mode_capability

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern boolean                    cmpmprx_fetch_mode_capability(

  sys_modem_as_id_e_type     asubs_id,

  sys_sys_mode_mask_e_type   *mode_capability_buffer
);

/*===========================================================================

FUNCTION cmpmprx_fetch_domain_capability

DESCRIPTION
  Fetch domain capability.

DEPENDENCIES
None

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
boolean                    cmpmprx_fetch_domain_capability(

  sys_modem_as_id_e_type  asubs_id,

  sys_srv_domain_e_type   *domain_capability_buffer
);

/*===========================================================================

FUNCTION cmpmprx_fetch_domain_per_ims_cap

DESCRIPTION
  Fetch domain capability per ims cap enabled

DEPENDENCIES
None

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
boolean                    cmpmprx_fetch_domain_per_ims_cap(

  sys_modem_as_id_e_type  asubs_id,

  sys_srv_domain_e_type  *domain_capability_buffer
);

/*===========================================================================

FUNCTION cmpmprx_fetch_voice_domain_pref

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern boolean                    cmpmprx_get_voice_domain_pref(

  sys_modem_as_id_e_type       asubs_id,

  sys_voice_domain_pref_e_type *voice_domain_pref,

  boolean                      *is_voice_dom_pref_locked
);

/*===========================================================================

FUNCTION cmpmprx_get_rat_acq_order_pref

DESCRIPTION
  Returns the acq_order_pref from the current PM policy

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern boolean                    cmpmprx_get_rat_acq_order_pref(

  sys_modem_as_id_e_type         asubs_id,

  cm_acq_pri_order_pref_s_type   *rat_acq_order_ptr
);

/*===========================================================================

FUNCTION cmpmprx_fetch_max_mode_capability

DESCRIPTION
  Fetch the max mode capability and reset the max mode cap mask.
  This should only be called during initialization.

DEPENDENCIES
None

RETURN VALUE
  uint32

SIDE EFFECTS
  none

===========================================================================*/
uint32    cmpmprx_fetch_max_mode_capability(

  sys_modem_as_id_e_type     asubs_id
);

/*===========================================================================

FUNCTION cmpmprx_read_nMaxActiveData

DESCRIPTION
  Read nMaxActiveData

===========================================================================*/
uint8    cmpmprx_read_nMaxActiveData(void);

/*===========================================================================

FUNCTION cmpmprx_read_call_mode_capability

DESCRIPTION
  Read the call mode capability.
  Note this should only be called during initialization. This does not reset the mask.

DEPENDENCIES
None

RETURN VALUE
  uint32

SIDE EFFECTS
  none

===========================================================================*/

void    cmpmprx_read_call_mode_capability(

  sys_modem_as_id_e_type     asubs_id,

  sys_call_mode_t            *call_mode_capability_buffer
);

/*===========================================================================

FUNCTION cmpmprx_get_band_capability

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern boolean                    cmpmprx_get_band_capability(

  sys_modem_as_id_e_type     asubs_id,

  sys_band_mask_e_type       *band_capability_buffer,

  sys_lte_band_mask_e_type   *lte_band_capability_buffer,

  sys_band_mask_e_type       *tds_band_capability_buffer
);

/*===========================================================================

FUNCTION cmpmprx_get_call_mode_capability

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_get_call_mode_capability(
  sys_modem_as_id_e_type     asubs_id,

  sys_call_mode_t   *call_mode_capability_buffer
);

/*===========================================================================

FUNCTION cmpmprx_fetch_call_mode_capability

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean cmpmprx_fetch_call_mode_capability(
  sys_modem_as_id_e_type     asubs_id,

  sys_call_mode_t   *call_mode_capability_buffer
);


/*===========================================================================

FUNCTION cmpmprx_get_ue_mode

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_get_ue_mode(

  sys_modem_as_id_e_type     asubs_id,

  sys_ue_mode_e_type         *ue_mode_buffer
);

/*===========================================================================

FUNCTION cmpmprx_read_ue_mode

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void cmpmprx_read_ue_mode(

  sys_modem_as_id_e_type     asubs_id,

  sys_ue_mode_e_type         *ue_mode_buffer
);

/*===========================================================================

FUNCTION cmpmprx_get_subs_feature_mode

DESCRIPTION
  API for CM to retrieve the new subs specific feature mode.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: New subs featuremode retrieved
  False: Operation failed.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean                    cmpmprx_get_subs_feature_mode(

  sys_modem_as_id_e_type     asubs_id,

  sys_subs_feature_t         *subs_featuremode_buffer
);
/*===========================================================================

FUNCTION cmpmprx_unforce_policy

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void                       cmpmprx_unforce_policy(

  sys_modem_as_id_e_type     asubs_id

);

/*===========================================================================

FUNCTION cmpmprx_sys_sel_pref_proc

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void    cmpmprx_sys_sel_pref_proc(
  sys_modem_as_id_e_type         asubs_id,
  cm_pref_s_type                *pref_ptr,
  cm_acq_pri_order_pref_s_type  *rat_acq_pri_order,
  uint16                         req_id
);

/*===========================================================================

FUNCTION cmpmprx_notify_call_end

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void                       cmpmprx_notify_call_end(

  sys_modem_as_id_e_type     asubs_id,
  boolean is_emerg
);

/*===========================================================================

FUNCTION cmpmprx_notify_emg_call_start

DESCRIPTION
  The function is called to notify cmpmprx that emergency call has started.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void  cmpmprx_notify_emg_call_start(void);


/*===========================================================================

FUNCTION cmpmprx_get_device_mode

DESCRIPTION
  Gets device mode which is decided based on no. of SIM and max SIMs that are
  active at a given time.

===========================================================================*/
sys_modem_device_mode_e_type cmpmprx_get_device_mode( void );

/*===========================================================================

FUNCTION cmpmprx_get_num_of_sims

DESCRIPTION
  Get num of sims supported base on Policy manager configuration item.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
size_t  cmpmprx_get_num_of_sims( void );

/*===========================================================================

FUNCTION cmpmprx_get_num_of_active_data

DESCRIPTION
  Get num of active data supported base on Policy manager configuration item.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
size_t  cmpmprx_get_num_of_active_data( void );


/*===========================================================================

FUNCTION cmpmprx_rread_device_mode

DESCRIPTION
  Added for the purpose of debugging in case we need to re-read device mode.

===========================================================================*/
void cmpmprx_rread_device_mode( void );

/*===========================================================================

FUNCTION cmpmprx_is_ok_to_enforce_policy

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern boolean                    cmpmprx_is_ok_to_enforce_policy(

  cm_policy_config_s_type *policy_ptr,

  sys_modem_as_id_e_type     asubs_id,
  
  boolean is_cleanup_needed
);

/*===========================================================================

FUNCTION cmpmprx_sys_sel_pref_policy_update

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern boolean                    cmpmprx_sys_sel_pref_policy_update(

  sys_modem_as_id_e_type     asubs_id,

  cm_sys_sel_pref_params_s_type *pref_ptr,

  cm_ph_cmd_s_type          *ph_cmd_ptr
);

/*===========================================================================

FUNCTION cmpmprx_sys_sel_pref_policy_update

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern cm_ph_cmd_err_e_type         cmpmprx_check_sys_sel_pref_policy_conflict(
  cm_ph_cmd_s_type           *ph_cmd_ptr
);

/*===========================================================================

FUNCTION cmpmprx_thermal_state_change_policy_update

DESCRIPTION
  If the new thermal state cause a policy change, retrieve it
  from Policy Manager and update the cmpm_local.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: New policy retrieved and updated.
  False: No change in policy.

SIDE EFFECTS
  none

===========================================================================*/
extern void           cmpmprx_thermal_state_change_policy_update(

  const cfcm_cmd_msg_type_s *cfcm_ind
);

/*===========================================================================

FUNCTION cmpmprx_msim_subsc_changed

DESCRIPTION
  Called when susbcriptions in multi-mode phones change.
  This function calls into PM to update new susbcriptions.
  PM refreshes policies based on this update.
    - Currently PM shall decide enable/disable LTE in multimode sub
      when subscriptions change. Meaning MSIM standby pref change.
===========================================================================*/
boolean cmpmprx_msim_subsc_changed(

  uint8 active_subs,

  sys_modem_as_id_e_type  default_data_subs,

  cm_dds_cause_e_type  dds_status
);

/*===========================================================================

FUNCTION cmpmprx_get_overall_featuremode

DESCRIPTION
  Called from other modules to read the current value of the overall
  featuremode in CMPM
===========================================================================*/
sys_overall_feature_t             cmpmprx_get_overall_featuremode(void);

/*===========================================================================

FUNCTION cmpmprx_clnup_complete_ind

DESCRIPTION
  CM will notify PMPRX that the cleanup for Refresh type 0 OR operating mode
  is complete and PMPRX can enforce any pending policies.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void                       cmpmprx_clnup_complete_ind( void );

/*===========================================================================

FUNCTION cmpmprx_get_networks_end_ind

DESCRIPTION
  CM will notify PMPRX that the manual search has been completed and any
  pending policies can be enforced

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void                       cmpmprx_get_networks_end_ind(void);


/*===========================================================================

FUNCTION cmpmprx_handle_volte_state_notification_cmd

DESCRIPTION


DEPENDENCIES
  none

RETURN VALUE

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void                    cmpmprx_handle_volte_state_notification_cmd(
  cm_volte_state_notification_cmd_s_type *volte_state_cmd
);

/*===========================================================================

FUNCTION cmpmprx_is_special_volte_e911_calls

DESCRIPTION
  Returns specialization value for SYS_SPECIAL_VOLTE_E911_CALLS.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean cmpmprx_is_special_volte_e911_calls(
        sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmpmprx_compute_restricted_mode

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN cm_network_sel_mode_pref_e_type cmpmprx_compute_restricted_mode(

  sys_modem_as_id_e_type           asubs_id,

  cm_network_sel_mode_pref_e_type  net_sel_mode_pref
);

#ifdef CM_DEBUG
#error code not present
#endif
extern cm_policy_config_s_type    *cmpm_ptr( void );

/*===========================================================================

FUNCTION cmpm_sub_ptr

DESCRIPTION
  Return a pointer to sub policy.

DEPENDENCIES
  none

RETURN VALUE
  cm_sub_level_policy_s_type *

SIDE EFFECTS
  none

===========================================================================*/
cm_sub_level_policy_s_type  *cmpm_sub_ptr(sys_modem_as_id_e_type      asubs_id);

/*===========================================================================

FUNCTION cmpmprx_enforce_current_policy

DESCRIPTION
  Process the policy currently in cmpm_local
===========================================================================*/
EXTERN void                       cmpmprx_enforce_current_policy(

  sys_modem_as_id_e_type     asubs_id
);

/*===========================================================================

FUNCTION cmpmprx_enforce_device_policy

DESCRIPTION
  Force action for device policy masks
===========================================================================*/
EXTERN void                       cmpmprx_enforce_device_policy(

  policy_change_mask         mask
);

/*===========================================================================

FUNCTION cmpmprx_get_persisted_user_plmn

DESCRIPTION
  Read persistent PLMN set by earlier user selection before SSR

DEPENDENCIES
None

RETURN VALUE
  TRUE if read successful
  FALSE otherwise

SIDE EFFECTS
  none

===========================================================================*/

boolean cmpmprx_get_persisted_user_plmn(
  sys_modem_as_id_e_type  as_id,
  boolean                 clear,
  sys_plmn_id_s_type     *plmn_ptr );

/*===========================================================================

FUNCTION cmpmprx_report_full_srv_lost

DESCRIPTION
  Reports to PM whenever Full Service Lost indication is received from SD.

===========================================================================*/
EXTERN boolean cmpmprx_report_full_srv_lost(
  sys_modem_as_id_e_type     subs_id,
  sys_modem_stack_id_e_type  stack_id
);

/*===========================================================================

FUNCTION cmpmprx_report_operational_radio_stacks

DESCRIPTION
Reports to PM whenever there is change in operation flag on either sub/stack.

===========================================================================*/
EXTERN boolean cmpmprx_report_operational_radio_stacks(
  sys_modem_as_id_e_type     subs_id
);

/*===========================================================================

FUNCTION cmpmprx_get_pm_sfmode

DESCRIPTION
  API for CM to get the PM sub-feature mode configured in XML.
  Note: For 7+5, PM sub-feature mode is different from the CM internal sub-feature mode.

DEPENDENCIES
  none

RETURN VALUE
  sys_subs_feature_t

SIDE EFFECTS
  none

===========================================================================*/
sys_subs_feature_t    cmpmprx_get_pm_sfmode(

  sys_modem_as_id_e_type     asubs_id

);

/*===========================================================================

FUNCTION cmpmprx_sanitize_max_and_curr_cap

DESCRIPTION
  ERR_FATAL if max mode cap and curr mode cap are not obeying Superset/subset relation

DEPENDENCIES
  none

RETURN VALUE
  sys_subs_feature_t

SIDE EFFECTS
  none

===========================================================================*/
void cmpmprx_sanitize_max_and_curr_cap(cm_policy_config_s_type *pm_ptr);

/*===========================================================================

FUNCTION cmpmprx_check_and_enforce_policy

DESCRIPTION
  Force policy if any policy element is set


DEPENDENCIES
  none

RETURN VALUE
  void

SIDE EFFECTS
  none

===========================================================================*/
void    cmpmprx_check_and_enforce_policy(void);

/*===========================================================================

FUNCTION cmpmprx_read_mode_capability

DESCRIPTION
  Read the  mode capability.

DEPENDENCIES
None

RETURN VALUE
  uint32

SIDE EFFECTS
  none

===========================================================================*/
uint32    cmpmprx_read_mode_capability(

  sys_modem_as_id_e_type     asubs_id
);

/*===========================================================================

FUNCTION cmpmprx_resume_policy_manager

DESCRIPTION
  Send the Resume command to Policy Manager.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void                       cmpmprx_resume_policy_manager( void );

/*===========================================================================

FUNCTION cmpmprx_is_policy_manager_suspended

DESCRIPTION
  Return whether Policy Manager is currently suspended.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean                    cmpmprx_is_policy_manager_suspended( void );

/*===========================================================================

FUNCTION cmpmprx_determine_num_stacks

DESCRIPTION
  Evaluate the number of stacks for the provided asubs_id. Will be used when
  computing the new number of stacks based on eligibility and max_mode_cap
  changes.


DEPENDENCIES
  none

RETURN VALUE
  void

SIDE EFFECTS
  none

===========================================================================*/
uint8  cmpmprx_determine_num_stacks(
  sys_modem_as_id_e_type     asubs_id,
  uint8                      active_subs
);

/*===========================================================================

FUNCTION cmpmprx_is_valid_sub_policy

DESCRIPTION
  Determine if a sub policy is valid for the specified sub.

DEPENDENCIES
  none

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_is_valid_sub_policy(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmpmprx_read_init_max_mode_capability

DESCRIPTION
  Fetch the max mode capability at cm initizlization.

DEPENDENCIES
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  none

===========================================================================*/
uint32    cmpmprx_read_init_max_mode_capability(

  sys_modem_as_id_e_type     asubs_id
);

/*===========================================================================

FUNCTION cmpmprx_read_1x_sub

DESCRIPTION
  Read max cap policy and determine the 1x sub.

DEPENDENCIES
  should only be only caled at cm initialization.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
sys_modem_as_id_e_type  cmpmprx_read_1x_sub(void);

/*===========================================================================

FUNCTION cmpmprx_policy_chg_proc_no_enforce

DESCRIPTION
  Process config commands. This is almost same as cmpmprx_policy_chg_ind_proc() with exception that it just processess the policy config items
  i.e., Don't enforce any policy here.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
cm_effective_policy_change_reason_e_type cmpmprx_policy_chg_proc_no_enforce(
    sys_modem_as_id_e_type      asubs_id,
    cm_policyman_cfg_s          *rpt_ptr,
     /* Pointer to the report sent by Message Router */
    sys_non_acq_param_s_type     *non_acq_param
   );

/*===========================================================================

FUNCTION cmpmprx_proc_msgr_retrieve_srv_req

DESCRIPTION
  Process config commands and returns if a service request can be triggered due to this PM config changes or not

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static boolean                    cmpmprx_proc_msgr_retrieve_srv_req(

  cm_policyman_cfg_s         *rpt_ptr,

  cm_policy_config_s_type    *local_policy,

  policy_change_mask         *device_local_mask_ptr
);

/*===========================================================================

FUNCTION cmpmprx_retrieve_and_verify_srv_req

DESCRIPTION
  Initialize the Policy Manager object and check whether this can result in SRV req and set the mask if srv req
  can be triggered due to PM config change

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static policyman_status_t        cmpmprx_retrieve_and_verify_srv_req
(
  cm_policy_config_s_type    *local_policy_ptr,

  policyman_item_collection_t const  *policy_item,

  policy_change_mask         *change_mask,

  policy_change_mask         *device_change_mask
);

/*===========================================================================

FUNCTION cmpmprx_session_chgd

DESCRIPTION
  Report session change and receive an early copy of the new policy
  to use from Policyman

===========================================================================*/
void    cmpmprx_session_chgd(

  mmgsdi_session_type_enum_type sessionType,
  boolean                       isAvailable,
  mmgsdi_app_enum_type          appType

);

/*===========================================================================

FUNCTION cmpmprx_timer_proc

DESCRIPTION
  Enforce pending policies that could not be enforced earlier due to the
  conditions present in cmcall_is_there_cullular_call_or_in_emerg_cb.

===========================================================================*/
void    cmpmprx_timer_proc(void);

/*===========================================================================

FUNCTION  cmpmprx_is_ok_to_enforce_sub_policy

DESCRIPTION
  Check if it is not ok to enforce pending sub policy items - this happens
  when any of the following conditions are true:

  -active voice/emerg/vt/vs/sms/cs_data call (excluding wifi call) per device
  -device is in emergency callback mode
  -device has MT paging in progress
  -device has an active external VOLTE call (excluding external wifi call)

DEPENDENCIES
  None

RETURN VALUE
  TRUE if condition is met

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_is_ok_to_enforce_sub_policy (sys_modem_as_id_e_type asubs_id);

/*===========================================================================

FUNCTION  cmpmprx_is_ok_to_enforce_device_policy

DESCRIPTION
  Check if there is any active voice/emerg/sms/cs_data call per device,
  or if device is in emergency callback mode

DEPENDENCIES
  None

RETURN VALUE
  TRUE if there is any active cs call or in emerg cb mode

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_is_ok_to_enforce_device_policy (
  cm_call_id_type call_id
);

/*===========================================================================

FUNCTION  cmpmprx_is_ok_to_enforce_device_policy_not_check_mt_page

DESCRIPTION
  Check if there is any active voice/emerg/sms/cs_data call per device,
  or if device is in emergency callback mode

DEPENDENCIES
  None

RETURN VALUE
  TRUE if there is any active cs call or in emerg cb mode

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_is_ok_to_enforce_device_policy_not_check_mt_page(void);

/*===========================================================================

FUNCTION cmpmprx_get_subs_for_ims_dereg

DESCRIPTION
  Retrieves a sandbox policy from PM that will result from a DDS switch.
  This will be called at the beginning of DDS switch processing to determine
  which subs have a capability change so that those subs can be IMS
  deregistered.

===========================================================================*/
uint8    cmpmprx_get_subs_for_ims_dereg(

  uint8 active_subs,

  sys_modem_as_id_e_type  default_data_subs,

  cm_dds_cause_e_type  dds_status
);

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION cmpmprx_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for cmpmprx global variables

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
void cmpmprx_qsh_mdump_collect();
#endif

/*===========================================================================

FUNCTION cmpmprx_inform_sim_state_chgd_and_evaluate_policy_chg

DESCRIPTION
  Report sim state changes to policymand and returrn TRUE if this results in mode cap chg

DEPENDENCIES
  None


RETURN VALUE
  boolean - if sim state update results in mode cap chg


SIDE EFFECTS
  None

===========================================================================*/
boolean    cmpmprx_inform_sim_state_chgd_and_evaluate_policy_chg(
  sys_sim_state_e_type          sim_state,
  sys_modem_as_id_e_type        asubs_id

);

/*===========================================================================

FUNCTION cmpmprx_enforce_sub_policies_avoid_rat_conflict

DESCRIPTION

  Enforces pending sub policies in correct order to avoid RAT concurrency
  issues. Specifically H+T is not supported so we will enforce policy on
  the sub without T first.

RETURN VALUE
  none

===========================================================================*/
void cmpmprx_enforce_sub_policies_avoid_rat_conflict(void);


#endif  /* CMPMPRX_H */
