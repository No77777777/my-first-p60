#ifndef CMSDS_H
#define CMSDS_H
/*===========================================================================

C A L L   M A N A G E R   SERVICE DOMAIN SELECTION  H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary to interface
  with CMSDS.C

Copyright (c) 1998 - 2010 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmsds.h#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/22/14   xs      hVOLTE SSAC support
03/06/14   xs      Per RAT SMS domain sel
09/25/13   xs      Add support to send voice pref on legacy RATs
09/25/13   xs      Hold VOPS indication when moving from VOLTE area to non-VOLTE area
08/19/13   fj        Add support for DSDX L+G
03/06/13   qf      FR3255 - Get T3402 value through message router
02/03/13   cl      FR1975: Dynamic Switch SVLTE <-> CSFB
05/21/12   vk      FIX to set default value of lte_disable_duration to T3402.
03/15/12   vk      Addition of domain selection info in CMSS.
02/23/12   vk      Provided config request support to CMSDS
12/20/11   vk      Domain selection fixes
12/01/11   vk      Added PS voice only support to domain selection module
10/31/11   rk      Use CMSS info instead of SD info for Domain selection algorithm
09/14/11   vk      New MMOC Transaction "MMOC_TRANS_DEACT_1XCSFB_PROT_CMD"
                   related changes.
09/14/11   rk      NV items N_min_MO_call_soft_retry and N_maxSIB8
09/02/11   rk      S102 less 1XCSFB
08/25/11   xs      Added EPS network feature support definition (PS voice indication)
07/12/11   xs      HDR optimize handoff enhancement
07/05/11   rk      Support for sending voice domain preference to NAS
05/20/11   xs      Add support for LTE->eHRPD optimized handoff
05/19/11   rk      Handling SIB8 failure
04/05/11   rk      Moving code block to keep 1XCSFB part together
03/29/11   rk      Fix for not doing PPCSFB after receiving AttachAccept with
                   SMSOnly set
03/03/11   rk      Fixing lint issues
03/02/11   rk      Domain selection phase 2 requirements
02/28/11   rk      Adding support for LTE UE mode preference
02/22/11   rk      SMS only registation
02/17/11   cl      Add support FEATURE_MMODE_REMOVE_1X for FUSION-MDM
12/10/10   rk      Sucessful MO call initial support
10/05/10   rk      Merging 1XCSFB pre-registration code
07/31/10   rk      1XCSFB pre-registation initial changes.

===========================================================================*/

/**--------------------------------------------------------------------------
** Includes
** --------------------------------------------------------------------------
*/

#include "mmcp_variation.h"
#include "comdef.h"    /* Definition for basic types and macros */
#include "cm.h"        /* External interface to cm.c */
#include "cm_v.h"
#include "cmi.h"       /* Internal interface to cm.c */
#include "sys.h"       /* System wide definitions */
#include "sys_v.h"
#include "cmph.h"
#include "cmpmprx.h"
#include "sd_i.h"
#include "emm_ext_msg.h"

#if defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE) && defined(FEATURE_1X_SRVCC)
#include "cmxll.h"
#endif

#define CMSDS_UPTIME_NONE  (dword) (-1)

/* Extended LTE disable duration in ms */
#define CMSDS_EXTENDED_LTE_DISABLE_DURATION 1000

#define CMSDS_SIB8_NOT_AVAIL_DURATION 0

#define CMSDS_DISABLE_LTE_DURATION_UPTO_PWR_CYCLE 0

#define CMSDS_DISABLE_IMS_REG_STATUS_WAIT_TIMER   0

#define CMSDS_DISABLE_LTE_DURATION_MIN_VALUE    60
/* Minimum LTE disable duration */

#define CMSDS_DEFAULT_N_MAX_SIB8                  3
/* Max times domain selection can allow SIB8 read failures */

#define CM_1XCSFB_CALL_MAX_SOFT_FAIL_RETRIES      3
/* Max number of ESR attempts that can be done for CSFB call
** (N_min_MO_call_soft_retry)
*/

#define CMSDS_DISABLE_LTE_DURATION_INVALID 0xFFFFFFFF
/* set when NV read fail or NV not present */

#define CMSDS_IMS_REG_STATUS_WAIT_TIMER          35
/* Default value of IMS REG STATUS WAIT TIMER */

typedef enum
{
  CMSDS_IMS_VOICE_NONE = -1,
  /* For internal use only */

  CMSDS_IMS_VOICE_NOT_AVAIL,
  /* LTE IMS voice is not available */

  CMSDS_IMS_VOICE_AVAIL,
  /* LTE IMS voice is available */

  CMSDS_IMS_VOICE_MAX
  /* For internal use only */
} cm_sds_ims_voice_avail_e_type;


/**--------------------------------------------------------------------------
** Datatypes
** --------------------------------------------------------------------------
*/

typedef enum
{
  CM_HDR_TUNNEL_CMD_NONE = -1,
  CM_HDR_TUNNEL_CMD_ACTD,
  CM_HDR_TUNNEL_CMD_DEACTD
} cm_hdr_tunnel_cmd_e_type;

/* IMS call types list for PLMN blocking
*/
typedef enum
{
  CM_IMS_CALL_TYPE_VOICE,
  CM_IMS_CALL_TYPE_SMS,
  CM_IMS_CALL_TYPE_MAX
} cm_ims_call_type_e_type;

typedef enum
{
  CM_IMS_REG_STATUS_NONE = -1,
  CM_IMS_REG_STATUS_FAIL_TEMPORARY,
  CM_IMS_REG_STATUS_FAIL_PERMANENT,
  CM_IMS_REG_STATUS_FAIL_PERMANENT_TILL_PWR_CYCLE,
  CM_IMS_REG_STATUS_FAIL_VOLTE_OFF
} cm_ims_reg_status_e_type;



typedef enum
{
  CM_PLMN_BLOCK_REAS_NO_VOICE,
  CM_PLMN_BLOCK_REAS_NO_SMS,
  CM_PLMN_BLOCK_REAS_MAX
} cm_plmn_block_reson_e_type;

/* Used to indicate whether UE is currently in
** VOPS area or not .
*/
typedef enum
{
  CMSDS_VA_TYPE_NONE = -1,
  /* For internal use only
  */

  CMSDS_VA_TYPE_UNKNOWN,
  /* This will be used when UE is not attached to LTE
  */

  CMSDS_VA_TYPE_VOPS,
  /* It indicates that UE is attached to LTE with VOPS support
  */

  CMSDS_VA_TYPE_NON_VOPS,
  /* It indicates that UE is attached to LTE without VOPS support
  */

  CMSDS_VA_TYPE_MAX
  /* internal use only
  */
} cmsds_va_type;

typedef struct
{
  cmsds_va_type  va;
  sys_plmn_id_s_type associated_plmn_id;
} cmsds_va_s_type;

typedef struct
{
  boolean is_lte_removal_enabled;
  /* Indicate whether LTE removal logic is enabled */

  uint32 vops_off_timer;
  /* Timer in sec to use when NW indicates vops not supported */

  uint32 reg_fail_timer;
  /* Timer in sec to use when NW indicates vops supported, but IMS reg fails */

}cmsds_non_dds_lte_removal_s_type;

/* This is used to hold current status of all IMS call
** activities present
*/
typedef struct
{
  boolean voice_present;
  boolean video_present;
  boolean sms_present;
} cmsds_ext_ims_call_activity_s_type;

/* Holds domain selection config data
*/
typedef struct cmsds_gen_sub_config_s
{

  sys_voice_domain_pref_e_type  voice_domain_pref;
  /* Voice domain preference */

  boolean                       is_voice_domain_pref_locked;
  /* Indicates whether user is allowed to change the voice domain pref or not */

  sys_ue_usage_setting_e_type   ue_usage_setting;
  /* Voice centric or data centric */

  boolean                       is_sms_only;
  /* Is SMS only */

  boolean                       is_sms_mandatory;
  /* Indicates whether SMS is Mandatary */

  sys_sms_domain_pref_e_type    sms_domain_pref[15];
  /* Voice domain preference */

  uint32                        lte_disable_duration;
  /**< Duration in seconds to disable LTE */

  uint32                        lte_disable_duration_nv;
  /**< Duration in seconds to disable LTE, as read form NV */

  boolean                       is_sms_over_s102_supported;
  /* Indicates whether SMS Over S102 is supported or not */

  uint32                        ims_reg_status_wait_timer;
  /* Indicates IMS reg status wait timer */

  boolean                       allow_csfb_upon_ims_reg;
  /* do CSFB calculation even if IMS voice is possible */

  boolean                       is_ims_enabled;
  /* Indicates whether IMS is enabled or not*/

  boolean                       is_lte_release9_supp;
  /* Indicates if the LTE release version is 9 or greater */

  dword                         t3402_value;
  /* Store the current T3402 value in secs */

  boolean                       is_plmn_block_req_in_lte_only_mode;
  /* Indicates whether PLMN blocking is needed in LTE ONLY mode or not */

  sys_cmsds_plmn_block_e_type   plmn_block_type;
  /* Indicates whether PLMN blocking is needed without checking for PPCSFB
  ** if IMS reg fail on non DDS sub
  */

  boolean                       is_plmn_blocking_sync;
  /*if TRUE CM will always wait for IMS registration status before doing PLMN blocking*/

  cmsds_non_dds_lte_removal_s_type lte_removal_config;
  /* On non DDS sub, upon IMS reg fail or vops off, whether to remove LTE
  ** and start a timer to add back LTE
  */

} cmsds_gen_sub_config_s_type;

typedef struct cmsds_3gpp2_sub_config_s
{

  boolean                     rrc_1xcsfb_enabled;
  /* Indicates whether 1xCSFB enabled per EFS config */

  boolean                     supportfordualrxues;
  /* Indicates if dual rx 1xcsfb is enabled */

  uint8                       n_min_MO_call_soft_retry;
  /* min attempts of silent redials of 1XCSFB call over the LTE network
  ** upon short-term failures.silent redial on LTE is done only up to
  ** 20 seconds or until the  number of short-term failures exceeds
  ** n_min_MO_call_soft_retry
  */

  uint8                       n_maxSIB8;
  /* max SIB-8 broadcast slots that UE can wait to complete
  ** reading of SIB8 information
  */

  uint32                      sib8_not_avail_duration;
  /**< Duration in seconds for which domsin selection can wait for SIB8 */

  boolean                     is_mtcsfb_call_redial_supported;
  /* Indicates whether redial mehcanism for MT CSFB call supported */


} cmsds_3gpp2_sub_config_s_type;

typedef struct cmsds_3gpp2_sub_info_s
{

  boolean                     sib8_available;
  /* SIB8 status
  */

  boolean                     is_1xcsfb_activated;
  /* Is 1XCP activated in CSFB mode
  */

  boolean                     is_1xcsfb_deact_in_prog;
  /* Is 1XCP getting deactivated from CSFB mode.
  */

#if defined( FEATURE_LTE_TO_1X )
  cm_1xcsfb_result_e_type     csfb_status;
#endif /* FEATURE_LTE_TO_1X */

  dword                       onexcsfb_status_not_avail_uptime;
  /* Indicate the duration for which sib8 status is not available
  */

#ifdef FEATURE_LTE_TO_HDR_OH
  cm_hdr_tunnel_cmd_e_type    hdr_tunnel_status;
  /* Indicate if HDR tunnel is activated or deactivated by CM
  */
  boolean                     is_act_hdr_on_srv_cnf;
  /* Indicate HDR tunnel mode needs to be activated upon receiving LTE SRV_CNF
  ** with full service instead of attach complete.
  */
#endif

  boolean                     is_esr_supported;
  /* Indicates if ESR is supported in 1XSRLTE mode */

  boolean                     is_cdma_srvcc_activated;

  prot_trans_type             mmoc_trans_id;
  /* Used to store the Transaction ID exchanged with MMOC */

} cmsds_3gpp2_sub_info_s_type;

typedef struct cmsds_ims_sub_info_s
{

  sys_ims_reg_state_e_type        ims_reg_status_sms[SYS_SYS_MODE_MAX];
  /* Indicates IMS registration status */

  sys_ims_reg_state_e_type        ims_reg_status_voice[SYS_SYS_MODE_MAX];
  /* Indicates IMS registration status  for voice services */

  cm_ims_reg_status_e_type        ims_registration_status[CM_IMS_CALL_TYPE_MAX];
  /* Indicates IMS registration failure status */

  dword                           ims_wait_uptime;
  /* Indicate the max duration which we can wait for IMS response
  */

  cm_sds_ims_voice_avail_e_type   lte_ims_voice_avail;
  /* Indicates the current NW support for IMS voice */

  sys_sys_mode_mask_e_type        ims_voice_pref_mask;

  sys_sys_mode_mask_e_type        ims_sms_pref_mask;

  sys_ims_lte_emerg_supp_e_type   lte_ims_emerg_avail;
  /* Indicates the current NW support for IMS emergency call */

  uint32                          ims_reg_backoff_timer;
  /* Indicates the duration of PLMN blocking,
  ** actual blocking time also depends on T3402 and NV
  */

  boolean                         last_sent_ims_emerg_cap;
  /* Store the ims emerg_cap sent to MSGR */

  boolean                         last_sent_ims_reg_cap;
  /* Store the ims_reg_cap value sent to MSGR to L1 */
  
  uint8                           curr_trans_id;
  /* Store the current trans id */


} cmsds_ims_sub_info_s_type;

typedef struct cmsds_gen_sub_info_s
{

  cmsds_ims_sub_info_s_type            ims_sub_info;
  /* IMS related info */

  sys_lte_cs_capability_e_type         lte_cs_capability;
  /* Indicate CS registation status on lte network
  */

  sys_cm_domain_sel_domain_e_type      curr_voice_dom_selected;
  /* Current VOICE domain selected */

  sys_cm_domain_sel_domain_e_type      curr_sms_dom_selected;
  /* Current SMS domain selected */

  boolean                              is_plmn_blocking_in_progress;
  /* Indicates PLMN blocking in progress or not*/

  boolean                              is_plmn_blocking_delayed;
  /* Indicates PLMN blocking is delayed or not*/

  boolean                              voice_support_status;
  /* stores the status of voice support status */

  boolean                              is_activation_in_progress;
  /* Indicates whether device activation is in progress */

  sd_si_info_s_type                  *srv_ind_buf_ptr;
  /* holding the VOPS indication when VOLTE call is ongoing */

  sys_plmn_id_s_type                   plmn_id;
  /**<  PLMN ID of the serving system. */

  uint32                               tssac_hys;
  /* When UE moves from non-VA to VA, the device shall start a hysteresis timer tssac_hys.
  ** domain selection will not notify IMS voice preference until the expiry of tssac_hys.
  ** value 0 means timer is stopped
  */

  cmsds_va_s_type                        va_type;
  /*flag to indicate if it is a volte capable area*/

  boolean                              ext_ims_call_active;
  /* This flag identifies if external IMS call in active state */

  cm_call_type_e_type                  ext_ims_call_type;
  /* This indicates call type external IMS call in active state */

  sys_sys_mode_e_type                  ext_ims_sys_mode;
  /* RAT corresponding to the external IMS call (LTE or WLAN) */

  boolean                              pending_uemode_plmn_block_skipped;

  boolean                              is_block_sent_lte_only;
  /* This flag indicates plmn blocking has been sent due to LTE only */

  uint32                               lte_removal_uptime;
  /* Indicates the uptime to add LTE back to capability. 0 if timer not running */

  cmsds_ext_ims_call_activity_s_type   ext_ims_calls_active;
  /* indicates all active external IMS  call activities */

  cm_plmn_blocking_info_s_type        *pending_plmn_blocking_info;
  /* Indicates the pending plmn blocking info. Only used when plmn_blocking_sync is enabled */

  boolean                              em_call_plmn_block_skipped;
  /* PLMN blocking is pended during Eemergency call to be checked later 
   ** if UE can switch to SRLTE or block plmn after PM is resumed */

  boolean                              is_ignore_stale_ims_reg;
  /* Ignore stale IMS reg status indication when sub is already stopped.
  ** Will be reset when pref change indication is sent to IMS.
  */

  boolean                              block_req_for_call_duration;
  /* Flag to indicate if PLMN blocking should be done only for call duration.
  */

  cm_media_restrict_status_e_type       lte_voice_restrict_status;
  cm_media_restrict_status_e_type       lte_video_restrict_status;

} cmsds_gen_sub_info_s_type;

typedef struct cmsds_sub_prop_s
{

  cmsds_gen_sub_config_s_type     gen_sub_config;

  cmsds_3gpp2_sub_config_s_type   gpp2_sub_config;

  cmsds_gen_sub_info_s_type       gen_sub_info;

  cmsds_3gpp2_sub_info_s_type     gpp2_sub_info;

} cmsds_sub_prop_s_type;

/* Service domain selection type - holds all the information that is associated
** with Service domain selection.
*/
typedef struct cmsds_s
{

  uint8                           count;

  cm_init_mask                    init_mask;
  /* Indicate whether this object was properly initialized
  */

  cmsds_sub_prop_s_type          *sub_prop[MAX_SIMS];
  /* Private Domain Selection Information
  */
  boolean                    is_tau_needed;
  /*if buffered TAU complete needs to be sent */

  nas_tau_type               tau_type;  
  /* Tau type for buffered TAU */
} cmsds_s_type;

/* This holds information regarding non dds LTE removal timer
 * configuration
 */
 typedef PACKED struct PACKED_POST
{

  uint8 version;

  boolean is_lte_removal_enabled;
  /* Indicate whether LTE removal logic is enabled */

  uint32 vops_off_timer;
  /* Timer in min to use when NW indicates vops not supported */

  uint32 reg_fail_timer;
  /* Timer in min to use when NW indicates vops supported, but IMS reg fails */

  uint8 reserved[2];

}cmsds_lte_removal_timer_s_type;

/*===========================================================================

FUNCTION cmsds_next_trans_id

DESCRIPTION
  Update and return the next transaction id. Value will not be 0 or 0xff

DEPENDENCIES
None

RETURN VALUE
  Next transaction id

SIDE EFFECTS
  none

===========================================================================*/
uint8 cmsds_next_trans_id(
  sys_modem_as_id_e_type asubs_id
);

/**--------------------------------------------------------------------------
** Functions
** --------------------------------------------------------------------------
*/


/*===========================================================================

FUNCTION cmsds_ptr

DESCRIPTION
  Return a pointer to the one and only SDS object.

  The domain selectionobject is responsible for:
  1. contaning data related to CSFB
  2. activating/deactivation 1xcsfb


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern cmsds_s_type  *cmsds_ptr
(
  void
);



/*===========================================================================

FUNCTION cmsds_init

DESCRIPTION
  Initializes the domain selection object.

  This function must be called before the before domain selection object
  is being used, in any way, place, or form.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmsds_init
(
  boolean           is_update_info,
  uint8             refresh_subs_mask
);

/*===========================================================================

FUNCTION cmsds_sub_info_mem_free

DESCRIPTION
  This functions frees cmsds sub info

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmsds_sub_info_mem_free(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmsds_reset_per_device

DESCRIPTION
  Initializes the domain selection object.

  This function must be called before the before domain selection object
  is being used, in any way, place, or form.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

extern void cmsds_reset_per_device(void);

/*===========================================================================

@FUNCTION cmsds_cm_supported_rat

@DESCRIPTION
Determined CM supported RATs

@DEPENDENCIES
None

@RETURNS
CM supported RATs

@SIDE_EFFECT
None

===========================================================================*/

sd_ss_mode_pref_e_type cmsds_cm_supported_rat(
  sys_modem_as_id_e_type    asubs_id
);


/*===========================================================================

FUNCTION:cmsds_reset_ims_reg_and_voice_avail_status

DESCRIPTION
Reset IMS VOICE and SMS reg status .
Also, reset ims voice avail status.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsds_reset_ims_reg_and_voice_avail_status
(
  sys_modem_as_id_e_type  as_id
);


/*===========================================================================

FUNCTION cmsds_reset

DESCRIPTION
  Initializes the domain selection object.

  This function must be called before the before domain selection object
  is being used, in any way, place, or form.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmsds_reset_per_sub
(
  sys_modem_as_id_e_type  asubs_id
);


/*===========================================================================

FUNCTION:cmsds_reset_during_detach

DESCRIPTION
Reset

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

extern void cmsds_reset_during_detach
(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmsds_sd_rpt_proc

DESCRIPTION
  Process reports from System Determination.


DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmsds_sd_rpt_proc
(
  const cm_hdr_type   *rpt_ptr
  /* Pointer to SD report */
);



#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)) && \
     defined( FEATURE_LTE_TO_1X )


/*===========================================================================

FUNCTION cmsds_mc_rpt_proc

DESCRIPTION
  Process MC reports


DEPENDENCIES
  Service domain selection object must have already been initialized with
  cmsds_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmsds_mc_rpt_proc
(
  const cm_hdr_type   *rpt_ptr
  /* Pointer to MC report */
);

/*===========================================================================

FUNCTION cmsds_send_1xcsfb_deactivate_req

DESCRIPTION



DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/


extern void cmsds_send_1xcsfb_deactivate_req
(
  prot_deact_e_type deact_reason,

  sys_modem_as_id_e_type  asubs_id

);

/*===========================================================================

FUNCTION cmsds_is_in_1xcsfb_mode

DESCRIPTION
Indicates whether 1XCSFB is activated or not.



DEPENDENCIES
  SDS must have already been initialized with
  cmsds_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmsds_is_in_1xcsfb_mode(

  sys_modem_as_id_e_type  asubs_id

);

/*===========================================================================

FUNCTION cmsds_is_csfb_deact_allowed

DESCRIPTION
  This function checks whether CSFB deactivation is allowed or not.

DEPENDENCIES
  none

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  none

===========================================================================*/
extern boolean  cmsds_is_csfb_deact_allowed
(
  sys_modem_as_id_e_type  asubs_id
);

#endif /*(FEATURE_MMODE_CDMA_800 || FEATURE_MMODE_CDMA_1900) && FEATURE_LTE_TO_1X */


#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)) && \
   defined (FEATURE_CM_LTE) && defined (FEATURE_IP_CALL) && defined(FEATURE_1X_SRVCC)

/*===========================================================================

@FUNCTION cmsds_send_1xsrvcc_deactivate_req

@DESCRIPTION
  Send SRVCC deactivate request  to 1xCP.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
void cmsds_send_1xsrvcc_deactivate_req (
  prot_deact_e_type deact_reason,

  sys_modem_as_id_e_type  asubs_id
);

#endif

#if defined (FEATURE_LTE_TO_1X)

/*===========================================================================

FUNCTION cmsds_is_1xcsfb_call_allowed

DESCRIPTION
  Checks if 1XCSFB MO call can be allowed


DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/


extern boolean cmsds_is_1xcsfb_call_allowed(
  sys_modem_as_id_e_type  asubs_id

);

/*===========================================================================

FUNCTION cmsds_check_and_deact_csfb_mode

DESCRIPTION
  Checks and deactiavtes CP from CSFB mode.


DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/

extern void cmsds_check_and_deact_csfb_mode(
  sys_modem_as_id_e_type  asubs_id
);

#endif


/*===========================================================================

FUNCTION cmsds_is_ppcsfb_call_allowed

DESCRIPTION
   Checks if 3GPP CSFB MO call can be allowed


DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/


extern boolean cmsds_is_ppcsfb_call_allowed(
  sys_modem_as_id_e_type    as_id
);

/*===========================================================================

FUNCTION cmsds_is_cs_domain_sups_allowed

DESCRIPTION
   Checks if MO Sups cmd can be allowed on CS/CSFB domain


DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/


extern boolean cmsds_is_cs_domain_sups_allowed(
  sys_modem_as_id_e_type    as_id
);


/*===========================================================================

@FUNCTION cmsds_is_ppcsfb_possible

@DESCRIPTION
  Checks whether PPCSFB is possible or not.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern  boolean cmsds_is_ppcsfb_possible(
  sys_modem_as_id_e_type    as_id
);


#if defined (FEATURE_3GPP_CSFB) || defined (FEATURE_LTE_TO_1X) || defined (FEATURE_DOMAIN_SELECTION)

/*===========================================================================

FUNCTION cmsds_is_cs_voice_avail

DESCRIPTION
   Indicates whether CS domain is available or not


DEPENDENCIES


RETURN VALUE
  TRUE: If voice support is avaialble
  FALSE.: If voice support is not avaialble

OUTPUT PARAM:
csfb_type: Contains the currently supported CSFB type.

SIDE EFFECTS
  Nothing.

========================================================================== */

extern boolean cmsds_is_cs_voice_avail(
  cm_call_csfb_e_type      *csfb_type,
  sys_modem_as_id_e_type    as_id
);

/*===========================================================================

@FUNCTION cmsds_dom_sel_get_csfb_type_allowed

@DESCRIPTION
 returns csfb type based on current selected domain.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/

extern cm_call_csfb_e_type cmsds_dom_sel_get_csfb_type_allowed
(
  sys_modem_as_id_e_type  as_id,
  /* the subs ID of the call */

  cm_srv_type_e_type      srv_type
  /* the service type of the call */
);

#endif /* ( FEATURE_3GPP_CSFB) ||  (FEATURE_LTE_TO_1X)  */


#ifdef FEATURE_DOMAIN_SELECTION

/*===========================================================================
@FUNCTION cmsds_send_ims_pref_changed_ind

@DESCRIPTION
Will update IMS protocol. of IMS preferance for a specified call type.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_send_ims_pref_changed_ind
(
  sys_sys_mode_e_type         system_mode,
  cm_call_type_mask_type      ims_pref_mask,

  cm_call_type_mask_type   call_type_mask,

  sys_modem_as_id_e_type   as_id

);

/*===========================================================================

@FUNCTION cmsds_reset_srv_ind_buf_ptr

@DESCRIPTION


@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_reset_srv_ind_buf_ptr(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

@FUNCTION cmsds_process_srv_ind

@DESCRIPTION


@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
void cmsds_process_srv_ind (
  mm_sub_stk_id_s_type       mm_id,
  /* stack on which domain selection to be performed.*/

  const sd_si_info_s_type   *si_info_ptr
  /* Pointer to buffer containing new service info information */
);

/*===========================================================================

@FUNCTION cmsds_skip_sds_during_srv_ind

@DESCRIPTION
Checks whether domain selection need to be skipped due to PREF SRV IND
Skip SDS in following cases

1. SD is in NO_SRV state and CM received FULL_SRV_IND on LTE with pref_srv_ind set to TRUE.
2. Skip SDS due to PREF_SRV_IND during silent service lost processing.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern boolean cmsds_skip_sds_during_srv_ind(
  mm_sub_stk_id_s_type       mm_id,

  const sd_si_info_s_type   *si_info_ptr
  /* Pointer to buffer containing new service info information */
);
/*===========================================================================

FUNCTION cmsds_is_psap_callback_possible_on_lte

  This function will check and return boolean based on whether LTE is possible
  or not.

RETURN VALUE
  Boolen
  
===========================================================================*/
boolean  cmsds_is_psap_callback_possible_on_lte(
  mm_sub_stk_id_s_type mm_id
);

/*===========================================================================

@FUNCTION cmsds_is_volte_possible

@DESCRIPTION
Indicates whether VOLTE is possible or not.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern boolean cmsds_is_volte_possible
(
  boolean lte_ims_voice_avail,
  sys_modem_as_id_e_type sub_id
);

/*===========================================================================

FUNCTION cmsds_timer_proc

DESCRIPTION
  Process timer events.

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmsds_timer_proc(

  cm_timer_event_type    timer_event
  /* Indicate specific timer event */
);

/*==========================================================================

FUNCTION cmsds_reset_lte_removal_timer

DESCRIPTION
  Reset LTE removal timer so that LTE will be added back in cmmsc

RETURN VALUE
  none

DEPENDENCIES
  None

===========================================================================*/
extern void cmsds_reset_lte_removal_timer(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

@FUNCTION cmsds_update_and_send_voice_dom_sel_ind

@DESCRIPTION
Update and send current voice domain selected.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
void cmsds_update_and_send_voice_dom_sel_ind
(
  mm_sub_stk_id_s_type          mm_id,
  sys_cm_domain_sel_domain_e_type new_domain,
  sys_sys_mode_e_type           sys_mode
);

/*===========================================================================

@FUNCTION cmsds_update_and_send_sms_dom_sel_ind

@DESCRIPTION
Update and send current SMS domain selected.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/

void cmsds_update_and_send_sms_dom_sel_ind
(
  mm_sub_stk_id_s_type          mm_id,
  sys_cm_domain_sel_domain_e_type new_domain,
  sys_sys_mode_e_type           sys_mode
);

/*===========================================================================

FUNCTION cmsds_update_and_send_sms_dom_sel_ind

DESCRIPTION
  Update and send new sms domain to IMS.

DEPENDENCIES


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmsds_update_and_send_ims_avail
(
  cm_sds_ims_voice_avail_e_type ims_voice_avail,
  sys_sys_mode_e_type           sys_mode,
  sys_modem_as_id_e_type        as_id
);
/*===========================================================================

FUNCTION cmsds_is_block_req_for_call_duration_ps_only

  This function will check and return boolean based on whether PLMN blocking
  should be done for call duration.

RETURN VALUE
  Boolen
  
===========================================================================*/
boolean  cmsds_is_block_req_for_call_duration_ps_only(
  mm_sub_stk_id_s_type mm_id
);

/*===========================================================================
@FUNCTION cmsds_get_call_type_mask_ims

@DESCRIPTION
Returns call_type_mask_according to device configuration

@DEPENDENCIES

None

@RETURNS
  cm_call_type_mask_type

@SIDE_EFFECT

None

===========================================================================*/
extern cm_call_type_mask_type cmsds_get_call_type_mask_ims
(
  boolean                   is_ims_avail,
  sys_sys_mode_mask_e_type  sys_mode_mask,
  sys_modem_as_id_e_type    as_id
);


/*===========================================================================

@FUNCTION cmsds_send_domain_sel_chd_ind

@DESCRIPTION
Update and send current SMS domain selected.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_send_domain_sel_chd_ind
(
  sys_cm_domain_sel_domain_e_type new_domain,

  cm_call_type_mask_type call_type_mask,

  sys_sys_mode_e_type  sys_mode,

  sys_modem_as_id_e_type  as_id
);

/*===========================================================================

@FUNCTION cmsds_map_lte_ims_voice_availability

@DESCRIPTION
Returns IMS VOICE availability based on the service-indication ims_voice
boolean value.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/

cm_sds_ims_voice_avail_e_type cmsds_map_lte_ims_voice_availability
(
  boolean is_ims_voice_avail,
  sys_modem_as_id_e_type sub_id
);

/*===========================================================================

@FUNCTION cmsds_get_lte_ims_voice_availability

@DESCRIPTION
Returns IMS VOICE availability

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
cm_sds_ims_voice_avail_e_type cmsds_get_lte_ims_voice_availability(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

@FUNCTION cmsds_is_LTE_emerg_call_possible

@DESCRIPTION
Returns whether LTE emerg call is possible

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/

boolean cmsds_is_LTE_emerg_call_possible
(
  sys_modem_as_id_e_type  as_id
);

/*===========================================================================

@FUNCTION cmsds_is_current_sys_lte_ims_emergency_capable

@DESCRIPTION

Returns whether LTE system acquired supports IMS emergency

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/

extern boolean cmsds_is_current_sys_lte_ims_emergency_capable(
  sys_modem_as_id_e_type  as_id
);
/*===========================================================================

FUNCTION cmsds_process_dom_sel_get_req

DESCRIPTION
  Processes Domain selection get request from SDS clients

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

========================================================================== */
extern void cmsds_process_dom_sel_get_req
(
  const cm_domain_sel_domain_info_s_type *dom_sel_get_req,
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmsds_ims_cmd_proc

DESCRIPTION
  Process lower layer reports (i.e. replies to  call commands).

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmsds_ims_cmd_proc(

  const cm_sds_cmd_u_type   *cm_cmd_ptr,
  /* Pointer to LL report */

  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmsds_config_cmd_proc

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsds_config_cmd_proc(

  const cm_sds_cmd_u_type   *cm_cmd_ptr,
  /* Pointer to LL report */

  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================
@FUNCTION cmsds_send_sms_domain_pref_ind

@DESCRIPTION
Sends configured sms domain pref to clients

@DEPENDENCIES
None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
void cmsds_send_cnfg_sms_domain_pref_ind (
  void
);

/*===========================================================================
@FUNCTION cmsds_send_voice_domain_pref_cnfg_resp

@DESCRIPTION
Sends configured voice domain pref to clients

@DEPENDENCIES
None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
void cmsds_send_voice_domain_pref_cnfg_resp (

  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================
@FUNCTION cmsds_send_sms_domain_pref_cnfg_resp

@DESCRIPTION
Processes SMS pref changed ind from clients.

@DEPENDENCIES
None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
void cmsds_send_sms_domain_pref_cnfg_resp (

  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================
@FUNCTION cmsds_process_sms_domain_pref_chg_cmd

@DESCRIPTION
Processes SMS pref changed ind from clients.

@DEPENDENCIES
None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
void cmsds_process_sms_domain_pref_chg_cmd (

  const cm_sds_cmd_u_type   *cm_cmd_ptr,
  /* Pointer to client command*/

  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================
@FUNCTION cmsds_send_sms_domain_pref_update_ind

@DESCRIPTION
Sends configured sms domain pref to clients

@DEPENDENCIES
None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
void cmsds_send_sms_domain_pref_cnfg_update_ind (
  sys_modem_as_id_e_type               asubs_id
);

/*===========================================================================

FUNCTION cmsds_client_cmd_proc

DESCRIPTION
  Process client commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsds_client_cmd_proc(

  const cm_msgr_cmd_s_type *rpt_ptr,
  /* Pointer to command*/

  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmsds2_is_voice_media_restricted

DESCRIPTION
  Check if voice media is restricted for the RAT in current location

RETURN VALUE
  Boolean

===========================================================================*/
boolean cmsds_is_voice_media_restricted(
  sys_modem_as_id_e_type asubs_id,
  sys_sys_mode_e_type    sys_mode_to_check
);

/*===========================================================================

FUNCTION cmsds2_process_media_restriction_ind

DESCRIPTION
  Process media restriction indication from IMS

RETURN VALUE
  None

===========================================================================*/
void cmsds_process_media_restriction_ind(
  const cm_sds_cmd_u_type   *cm_cmd_ptr,
  sys_modem_as_id_e_type     asubs_id
);

/*===========================================================================

FUNCTION cmsds2_reset_media_restriction_status

DESCRIPTION
  Reset media restriction status

RETURN VALUE
  None

===========================================================================*/
extern void cmsds_reset_media_restriction_status(
  sys_modem_as_id_e_type     asubs_id
);

/*===========================================================================


FUNCTION cmsds_process_ims_reg_status

DESCRIPTION
  Process IJMS registration status.

DEPENDENCIES
None.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmsds_process_ims_reg_status(

  const cm_sds_cmd_u_type   *cm_cmd_ptr,
  /* Pointer to LL report */

  sys_modem_as_id_e_type     asubs_id
);

/*===========================================================================
@FUNCTION cmsds_send_resp_to_get_imf_pref_req

@DESCRIPTION
Send response to IMS_PREF_GET_REQ

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_send_resp_to_get_imf_pref_req
(
  cm_call_type_mask_type ims_pref_mask,

  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================
@FUNCTION cmsds_send_resp_to_get_imf_pref_req_ext

@DESCRIPTION
Send response to IMS_PREF_GET_REQ_EXT


@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
void cmsds_send_resp_to_get_imf_pref_req_ext
(
  sys_sys_mode_mask_e_type  sys_mode_mask,

  sys_modem_as_id_e_type    asubs_id
);

/*===========================================================================
@FUNCTION cmsds_send_resp_to_domain_sel_get_req

@DESCRIPTION
Send response to DOM_SEL_GET_REQ

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_send_resp_to_domain_sel_get_req
(
  sys_cm_domain_sel_domain_e_type domain_selected,

  cm_call_type_mask_type          call_type_mask,

  sys_modem_as_id_e_type          asubs_id
);


/*===========================================================================
@FUNCTION cmsds_process_dom_sel_ims_pref_get_req

@DESCRIPTION
Handles  IMS_PREF_GET_REQ from IMS

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_process_dom_sel_ims_pref_get_req
(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================
@FUNCTION cmsds_process_dom_sel_ims_pref_get_req_ext

@DESCRIPTION
Handles  IMS_PREF_GET_REQ from IMS

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_process_dom_sel_ims_pref_get_req_ext
(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmsds_process_srv_ind_ims_ps_voice_only

DESCRIPTION
  Processes SERVICE_IND in IMS_PS_VOICE_ONLY config.


DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

========================================================================== */

extern void cmsds_process_srv_ind_ims_ps_voice_only
(
  mm_sub_stk_id_s_type       mm_id,
  /* stack on which domain selection to be performed.*/

  const sd_si_info_s_type   *si_info_ptr
);

/*===========================================================================

@FUNCTION cmsds_process_srv_ind_ims_ps_voice_preferred

@DESCRIPTION
Handles service indication when the voice domain configuration is PS PREF

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_process_srv_ind_ims_ps_voice_preferred
(
  mm_sub_stk_id_s_type       mm_id,
  /* stack on which domain selection to be performed.*/

  const sd_si_info_s_type   *si_info_ptr
);

/*===========================================================================

@FUNCTION cmsds_process_srv_ind_cs_voice_only

@DESCRIPTION
Process the service indication according to CS_VOICE_ONLY voice domain configuration

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/

extern void cmsds_process_srv_ind_cs_voice_only
(
  mm_sub_stk_id_s_type       mm_id,
  /* stack on which domain selection to be performed.*/

  const sd_si_info_s_type  *si_info_ptr
);

/*===========================================================================

@FUNCTION cmsds_voice_dom_sel_cs_voice_only

@DESCRIPTION
Perform voice domain selection in CS_VOICE_ONLY configuration

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_voice_dom_sel_cs_voice_only
(
  mm_sub_stk_id_s_type       mm_id,
  /* stack on which domain selection to be performed.*/

  const sd_si_info_s_type  *si_info_ptr
);


/*===========================================================================

@FUNCTION cmsds_process_srv_ind_cs_voice_preferred

@DESCRIPTION
Process the service indication according to CS_VOICE_PREF voice domain configuration

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/

extern void cmsds_process_srv_ind_cs_voice_preferred
(
  mm_sub_stk_id_s_type       mm_id,
  /* stack on which domain selection to be performed.*/

  const sd_si_info_s_type   *si_info_ptr
);



/*===========================================================================

@FUNCTION cmsds_sms_dom_sel_process_no_ims

@DESCRIPTION
Performs SMS domain selection

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_sms_dom_sel_process_no_ims
(
  mm_sub_stk_id_s_type       mm_id,
  /* stack on which domain selection to be performed.*/
  sys_sys_mode_e_type       sys_mode
);

/*===========================================================================

@FUNCTION cmsds_sms_dom_sel_process_ims_reg_status

@DESCRIPTION
Performs SMS domain selection based on IMS registration status.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_sms_dom_sel_process_ims_reg_status
(
  sys_sys_mode_e_type                   sys_mode,
  cm_domain_sel_ims_reg_status_s_type  *ims_reg_status,
  sys_modem_as_id_e_type                as_id

);

/*===========================================================================

@FUNCTION cmsds_process_no_sms_ind

@DESCRIPTION
Process no sms ind as per is_sms_mandatory configuration.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_process_no_sms_ind
(
  mm_sub_stk_id_s_type       mm_id,
  /* Stack on which domain selection to be performed.*/
  sys_sys_mode_e_type sys_mode
);


/*===========================================================================

@FUNCTION cmsds_process_ims_reg_status_ind

@DESCRIPTION
Process  IMS_REG_STATTUS_IND from IMS

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_voice_dom_sel_process_ims_reg_status
(
  const cm_domain_sel_ims_reg_status_s_type *reg_status_ptr,

  sys_modem_as_id_e_type                     as_id
);

/*===========================================================================

@FUNCTION cmsds_process_voice_not_avail_ind

@DESCRIPTION
Process voice not available ind according to current UE usage setting

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_process_voice_not_avail_ind
(
  mm_sub_stk_id_s_type mm_id
);

/*===========================================================================

@FUNCTION cmsds_process_ps_voice_preferred_but_no_ps

@DESCRIPTION
Handles PS not available scenario when device is configured as IMS PS PREFERRED and CS VOICE secondary.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_process_ps_voice_preferred_but_no_ps
(
  mm_sub_stk_id_s_type  mm_id
);

/*===========================================================================

@FUNCTION cmsds_perform_plmn_blocking

@DESCRIPTION
Performs PLMN plmn blocking

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/

extern void cmsds_perform_plmn_blocking
(
  mm_sub_stk_id_s_type        mm_id,

  cm_plmn_block_reson_e_type  block_reas
);

/*===========================================================================

FUNCTION cmsds_ds_rsra_done_ind_proc

DESCRIPTION
  Process RS/RA done indication from DS. Send pending plmn blocking request if any

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmsds_ds_rsra_done_ind_proc(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

@FUNCTION cmsds_get_t3402_value

@DESCRIPTION
  Returns the T3402 value from cmsds_ptr

@DEPENDENCIES
  None

@RETURNS
  uint32 T3402 value

@SIDE_EFFECT
  None

===========================================================================*/
extern uint32 cmsds_get_t3402_value (
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

@FUNCTION cmsds_t3402_changed_ind_proc

@DESCRIPTION
  Update the T3402 value based on NAS_EMM_T3402_CHANGED_IND.

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None
None

===========================================================================*/
extern void cmsds_t3402_changed_ind_proc (
  emm_t3402_changed_ind_s_type  *t3402_changed_ind,
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

@FUNCTION cmsds_process_ims_config_change_cmd

@DESCRIPTION
Rerun domain selection based on new IMS configuration

@DEPENDENCIES

None

@RETURNS
  None

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_process_ims_config_change_cmd(

  boolean is_ims_enabled
  /* IMS enabled or not */
);

#endif /* FEATURE_DOMAIN_SELECTION */



/*===========================================================================

FUNCTION cmsds_update_ims_voice_support_on_lte

DESCRIPTION
  processes ims_avail status from the srv indication. Notify client IMS voice support status on LTE


DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

========================================================================== */
void cmsds_update_ims_voice_support_on_lte (
  mm_sub_stk_id_s_type  mm_id,
  boolean               is_ims_voice_avail
);

/*===========================================================================

FUNCTION cmsds_update_ims_emerg_support_on_lte

DESCRIPTION
  processes ims_emerg status from the srv indication.


DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

========================================================================== */
extern void cmsds_update_ims_emerg_support_on_lte (
  sys_ims_lte_emerg_supp_e_type ims_emerg_avail,

  sys_modem_as_id_e_type  asubs_id

);

/*===========================================================================

FUNCTION cmsds_update_selected_voice_domain

DESCRIPTION
  Updates Selected voice domain in LTE.

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

========================================================================== */

extern void cmsds_update_selected_voice_domain (

  mm_sub_stk_id_s_type          mm_id,

  sys_cm_domain_sel_domain_e_type voice_domain
);


/*===========================================================================

FUNCTION cmsds_determide_mode_to_add()

DESCRIPTION
  Determines mode to be added while enabling LTE.


DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

========================================================================== */

extern sd_ss_mode_pref_e_type cmsds_determine_mode_to_add
(
  cm_mode_pref_e_type curr_mode_pref
);

/*===========================================================================

FUNCTION cmsds_update_selected_sms_domain

DESCRIPTION
  Updates Selected voice domain in LTE.

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

========================================================================== */

extern void cmsds_update_selected_sms_domain
(
  mm_sub_stk_id_s_type          mm_id,
  sys_cm_domain_sel_domain_e_type sms_domain
);

/*===========================================================================

FUNCTION:cmsds_reset_during_stop_mode

DESCRIPTION
Reset

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

extern void cmsds_reset_during_stop_mode
(
  prot_deact_e_type deact_reason,

  sys_modem_as_id_e_type  asubs_id
);



/*===========================================================================

@FUNCTION cmsds_emergency_call_dom_selection

@DESCRIPTION
 returns domain for emergency call based on current voice domain and whether
 current domain supports emergency.

 if current voice domain does not support emergency it chooses an alternative domain
 if available.
 Eg:
 1. if curr domain is IMS but it does not support emergency, choose CSFB if it is possible
 2. If curr domain is not chosen, but CSFB is possible, choose CSFB as emerg domain
 3. if curr domain is not chosen, but there is only LTE IMS cap service available, choose IMS as emerg domain


@DEPENDENCIES

None

@RETURNS
  1. ss on which emerg call can be originated
  2. Domain on which emerg call can be originated
  3. csfb_type


@SIDE_EFFECT

None

===========================================================================*/
multimode_stack_e_type  cmsds_emergency_call_dom_selection(
  cm_call_csfb_e_type                 *csfb_type_ptr,
  sys_modem_as_id_e_type               asubs_id
);

/*===========================================================================

FUNCTION cmsds_get_sub_prop_ptr

DESCRIPTION
  Helper function to get a pointer to sds_ptr->config[] for a given sub_id.
  Checks that sub_id is in range of sds_ptr->config[] array.

DEPENDENCIES
  none

RETURN VALUE
  cmsds_gen_sub_config_s_type* config

SIDE EFFECTS
  none

===========================================================================*/
extern cmsds_sub_prop_s_type *cmsds_get_sub_prop_ptr(
  sys_modem_as_id_e_type sub_id
);
/*===========================================================================

@FUNCTION cmsds_process_tau_complete_ind

@DESCRIPTION
      IF PLMN-TAI in TAU complete is same as CM has received last from NAS then it will send TAU complete indication to IMS.
      If PLMN-TAI in TAU complete is different from what CM has received last from NAS then CM will buffer TAU complete.
      CM will send TAU complete after receiving new service indication from NAS

@DEPENDENCIES

None

@RETURNS

@SIDE_EFFECT

None

===========================================================================*/
extern void cmsds_process_tau_complete_ind(
  emm_tau_complete_ind_type *tau_complete_ind,
  sys_modem_as_id_e_type asubs_id
);

/*==========================================================================

FUNCTION cmsds_get_srv_ind_buf_ptr_per_subs

DESCRIPTION
  returns the srv_ind_buf_ptr based on asubs_id

RETURN VALUE
  none

DEPENDENCIES
  None

===========================================================================*/
extern sd_si_info_s_type *cmsds_get_srv_ind_buf_ptr_per_subs(
  sys_modem_as_id_e_type asubs_id
);

/*==========================================================================

FUNCTION cmsds_get_ims_enabled

DESCRIPTION
  returns if IMS is enabled from NV and UI setting

RETURN VALUE
  none

DEPENDENCIES
  None

===========================================================================*/
extern boolean cmsds_get_ims_enabled(
  sys_modem_as_id_e_type asubs_id
);

/*==========================================================================

FUNCTION cmsds_reset_is_block_sent_lte_only

DESCRIPTION
  Reset is_block_sent_lte_only flag

RETURN VALUE
  none

DEPENDENCIES
  None

===========================================================================*/
extern void cmsds_reset_is_block_sent_lte_only(
  sys_modem_as_id_e_type asubs_id
);

/*==========================================================================

FUNCTION cmsds_get_is_lte_to_be_removed

DESCRIPTION
  returns if LTE needs to be removed from capability

RETURN VALUE
  none

DEPENDENCIES
  None

===========================================================================*/
boolean cmsds_get_is_lte_to_be_removed(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmsds_is_ext_ims_call_active_per_sub

DESCRIPTION
  This function returns whether there is active ext IMS call on given Sub

DEPENDENCIES
  none

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  none
===========================================================================*/
extern boolean cmsds_is_ext_ims_call_active_per_sub
(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================
FUNCTION cmsds_is_ext_volte_call_active_per_sub

DESCRIPTION
  This function returns whether there is active ext volte call on given Sub

DEPENDENCIES
  none

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  none
===========================================================================*/
extern boolean cmsds_is_ext_volte_call_active_per_sub
(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmsds_is_ext_volte_call_type_active_per_sub

DESCRIPTION
  This function returns whether there is active ext VOLTE call on given sub as per the mask

DEPENDENCIES
  none

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  none
===========================================================================*/
boolean cmsds_is_ext_volte_call_type_active_per_sub
(
  sys_modem_as_id_e_type  asubs_id,

  cm_call_type_mask_type         call_type_mask
);


/*===========================================================================

@FUNCTION cmsds_determine_if_RAT_enabled

@DESCRIPTION
Determined if UE has given RAT enabled
If UE has given RAT enabled either as per CM modes or SD modes, return TRUE.
Ideally, both should be same. However, they can differ in an event of
out of sync between CM and SD.

@DEPENDENCIES
None

@RETURNS
None

@SIDE_EFFECT
None

===========================================================================*/

boolean cmsds_determine_if_RAT_enabled
(
  sys_modem_as_id_e_type    asubs_id,

  sd_ss_mode_pref_e_type    mode_pref
);

/*===========================================================================

FUNCTION cmsds_is_ext_voice

DESCRIPTION
  Check if there exists ext MO/MT ims voice call.

RETURN VALUE
  Boolean

===========================================================================*/
boolean cmsds_is_ext_voice(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmsds_is_ext_vt

DESCRIPTION
  Check if there exists ext MO/MT ims VT call.

RETURN VALUE
  Boolean

===========================================================================*/
boolean cmsds_is_ext_vt(
  sys_modem_as_id_e_type  asubs_id
);



#endif /* CM_DOMSEL_H */
