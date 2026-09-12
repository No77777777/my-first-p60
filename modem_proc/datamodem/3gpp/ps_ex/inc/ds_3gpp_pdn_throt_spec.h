/*!
  @file
  ds_3gpp_throttle_sm.h

  @brief
  Internal utility functions and routines

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_pdn_throt_spec.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/05/11   ss      Changes to support RPM.
08/16/10   hs      Initial version

===========================================================================*/

#ifndef DS_3GPP_PDN_THROT_SPEC_H
#define DS_3GPP_PDN_THROT_SPEC_H


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "rex.h"
#include "ds_3gpp_pdn_throttle_sm.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*-------------------------------------------------------------------------- 
  This structure maintains the parsed back-off timer information
-------------------------------------------------------------------------*/
typedef struct
{
  boolean                    retry_on_rat_change;
  boolean                    retry_on_proc_change;
  boolean                    retry_on_eplmns;
  rex_timer_cnt_type         timer_count;
  boolean                    timer_is_active;  
} ds_pdn_throt_spec_parsed_backoff_info_s;

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_pdn_throt_spec_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for MEM Pool global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_pdn_throt_spec_qsh_mdump_collect
(
  void
);
#endif


/*===========================================================================
FUNCTION      ds_pdn_throt_spec_set_t3396_timer

DESCRIPTION   This function saves the PDN throttling t3396 timer value 
              corresponding to the APN for which it was received.

PARAMETERS 
apn - apn name for which it was received 
timer_val - t3396 timer value 
plmn_id   - plmn passed by the client
subs_id   - subscription id passed by the client

RETURN VALUE 
void 

SIDE EFFECTS  None
===========================================================================*/
boolean ds_pdn_throt_spec_set_t3396_timer 
(
  byte                             *apn,
  uint32                           timer_val,
  sys_plmn_id_s_type               plmn_id,
  sys_modem_as_id_e_type           subs_id,
  ps_iface_net_down_reason_type    down_reason,
  ds_3gpp_pdn_throt_rat_based_s   *rat_based_pdn_throt_p
);

/*===========================================================================
FUNCTION DS_PDN_THROT_SPEC_UNBLOCK_APN_TIMER_EXP_CMD_HDLR

DESCRIPTION
  This function processes cmd to unblock the APN on expiry of the throttling
  timer
  
PARAMETERS   : 
  apn
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/

void ds_pdn_throt_spec_unblock_apn_timer_exp_cmd_hdlr
(
  void                  *throt_sm_p,
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_PDN_THROT_SPEC_UNBLOCK_ALL_APN_GLOBALLY_BLKED_APN

DESCRIPTION
  This function processes cmd to unblock all the globally blocked APN
  
PARAMETERS   : 
  void
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_pdn_throt_spec_unblock_esm_backoff_throttling
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_PDN_THROT_SPEC_TIMESTAMP_APN_THROT_TMR_CMD_HDLR

DESCRIPTION
  This function timestamps the APN throttle timer event in EFS
  
PARAMETERS   : 
  throt_sm_p      - pointer to core Throt state machine
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_pdn_throt_spec_timestamp_apn_throt_tmr_cmd_hdlr
(
  void*                  throt_sm_p,
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_PDN_THROT_SPEC_IMSI_INFO_AVAIL_CMD_HDLR

DESCRIPTION
  This function checks to see if the IMSI info matches the one stored in EFS.
  If yes, powerup global APN throttling is performed. Else, it has already
  been done, so nothing is done in this function.
 
PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_pdn_throt_spec_imsi_info_avail_cmd_hdlr
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_PDN_THROT_SPEC_INIT

DESCRIPTION
  This function does the powerup initialization for the APN throttle
  functionality.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_pdn_throt_spec_init
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION      DS_PDN_THROT_SPEC_CLEAR_T3396_TMR_ENTRY

DESCRIPTION   This function resets the t3396 timer entry from local cache and 
              also the EFS file.
 
PARAMETERS 
apn - apn name for which it was received 
plmn_id   - plmn passed by the client 
rat_mask 
subs_id   - subscription id passed by the client

RETURN VALUE 
void 

SIDE EFFECTS  None
===========================================================================*/
void ds_pdn_throt_spec_clear_t3396_tmr_entry 
(
  byte                      *apn,
  sys_plmn_id_s_type         plmn_id,
  ds_3gpp_throt_rat_e_type   rat_index,
  sys_modem_as_id_e_type     subs_id
);

/*===========================================================================
FUNCTION DS_PDN_THROT_SPEC_LTE_HANLDE_REJECT_WITH_T3396_IE

DESCRIPTION
  This function handles the reject message received with a t3396 IE
  based on Release 10 requirements
  

PARAMETERS   : 
  lte_call_info 				  	 -  LTE call info received in reject from CM
  apn                        -  APN name on the PDN connection
  down_reason
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_pdn_throt_spec_handle_reject_with_t3396_ie
(
  ds_3gpp_pdn_throt_ident_info_type  *throt_ident_info_p,
  boolean                             timer_active,
  rex_timer_cnt_type                  timer_count
);

/*===========================================================================
FUNCTION DS_PDN_THROT_SPEC_SAMPLING_TIMER_EXP_CMD_HDLR 
 
DESCRIPTION
  This function handles the sampling timer expiration command.
  
PARAMETERS   : 
  void

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_pdn_throt_spec_sampling_timer_exp_cmd_hdlr
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_PDN_THROT_SPEC_LTE_HANDLE_NAS_REJECT_WITH_T3396_IE 
 
DESCRIPTION
  This function handles the reject message received from NAS with a t3396 IE
  based on Release 11 requirements

PARAMETERS   : 
  cm_lte_call_info_s_type               *lte_call_info,
  ds_pdn_context_s                      *pdn_context_p,
  ps_iface_net_down_reason_type         down_reason

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_pdn_throt_spec_lte_handle_nas_reject_with_t3396_ie
(
  cm_lte_call_info_s_type      *lte_call_info,
  ds_pdn_context_s             *pdn_context_p,
  ps_iface_net_down_reason_type down_reason
);

/*===========================================================================
FUNCTION  DS_PDN_THROT_SPEC_CLEAR_IS_THROTTLE_FLAG_AND_TIMER

DESCRIPTION
  Stop T3396 timer for v4 and v6, if it is already running and reset the 
  global throttling flag to false.

PARAMETERS
  ds_3gpp_pdn_throt_rat_based_s    *rat_based_pdn_throt_p
  sys_plmn_id_s_type            current_plmn
  sys_modem_as_id_e_type        subs_id
  byte                          *apn
 
DEPENDENCIES
  None.

RETURN VALUE 
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_pdn_throt_spec_clear_is_throttle_flag_and_timer
(
  ds_3gpp_pdn_throt_rat_based_s    *rat_based_pdn_throt_p,
  sys_modem_as_id_e_type            subs_id,
  byte                             *apn
);

/*===========================================================================
FUNCTION DS_PDN_THROT_SPEC_UNBLOCK_APN_TIMER

DESCRIPTION
  This function unblocks apn timer 
 
PARAMETERS   : 
  throt_sm_p
  subs_id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_pdn_throt_spec_unblock_apn_timer
(
  void                       *throt_sm_p,
  sys_modem_as_id_e_type      subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_APN_THROTTLE_GET_T3396_TIMER

DESCRIPTION 
  This function returns the throttling failure timer value

PARAMETERS 
  apn_name     - apn name passed by the client
  plmn_id      - plmn id passed by the client
  rat_type
  subs_id      - subscription passed by the client  

RETURN VALUE 
  PDN throttle Failure Timer value in msec

SIDE EFFECTS 
  None
===========================================================================*/
uint32 ds_pdn_throt_spec_get_t3396_timer
(
  byte                     *apn_name,
  sys_plmn_id_s_type        plmn_id,
  uint8                    *index,
  ds_3gpp_throt_rat_e_type  rat_type,
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_SPEC_T3396_BACKOFF_CAUSE_CODE

DESCRIPTION 
  This function checks whether T3396 related backoff handling may be
  required for the given PS Net Down Reason

PARAMETERS 
  down_reason - PS Net Down Reason
  subs_id     - Subscription Id

RETURN VALUE 
  TRUE,  if T3396 backoff is required
  FALSE, otherwise

SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_pdn_throt_spec_t3396_backoff_cause_code
(
  ps_iface_net_down_reason_type   down_reason,
  sys_modem_as_id_e_type          subs_id
);

/*===========================================================================
FUNCTION      DS_PDN_THROT_SPEC_HANDLING_IS_REQUIRED

DESCRIPTION 
  This function checks whether Spec handling is required for the given
  PS Net Down Reason

PARAMETERS 
  down_reason - PS Net Down Reason
  subs_id     - Subscription Id

RETURN VALUE 
  TRUE,  if T3396 backoff is required
  FALSE, otherwise

SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_pdn_throt_spec_handling_is_required
(
  ps_iface_net_down_reason_type   down_reason,
  sys_modem_as_id_e_type          subs_id,
  ds_pdn_context_s                         *pdn_cntx_p,
  ds_pdn_throt_spec_parsed_backoff_info_s  *parsed_info_p
);

/*===========================================================================
FUNCTION      DS_PDN_THROT_SPEC_PRE_REL12_GET_SM_RETRY_WAIT_TIME

DESCRIPTION 
  This function fetches the SM Retry Wait Timer for Pre-Rel12 devices

PARAMETERS 
  backoff_timer - Backoff timer structure
  subs_id       - Subscription Id
  down_reason   - Failure reason
  timer_count_p - Returns the timer value in milli-seconds

RETURN VALUE 
  TRUE,  if Backoff timer was activated
  FALSE, if Backoff timer was deactivated

SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_pdn_throt_spec_pre_rel12_get_sm_retry_wait_time
(
  timer_3gpp_type3              backoff_timer,
  sys_modem_as_id_e_type        subs_id,
  ps_iface_net_down_reason_type down_reason,
  rex_timer_cnt_type            *timer_count_p
);

/*===========================================================================
FUNCTION      DS_PDN_THROT_SPEC_REL12_GET_SM_RETRY_WAIT_TIME

DESCRIPTION 
  This function fetches the SM Retry Wait Timer for Rel12 devices

PARAMETERS 
  backoff_timer - Backoff timer structure
  subs_id       - Subscription Id
  down_reason   - Failure reason
  timer_count_p - Returns the timer value in milli-seconds

RETURN VALUE 
  TRUE,  if Backoff timer was activated
  FALSE, if Backoff timer was deactivated

SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_pdn_throt_spec_rel12_or_above_get_sm_retry_wait_time
(
  timer_3gpp_type3              backoff_timer,
  sys_modem_as_id_e_type        subs_id,
  ps_iface_net_down_reason_type down_reason,
  rex_timer_cnt_type            *timer_count_p,
  sys_sys_mode_e_type           mode
);

/*===========================================================================
FUNCTION      DS_PDN_THROT_SPEC_GET_SM_RETRY_ON_RAT_CHANGE

DESCRIPTION 
  This function determines whether SM Retry shoulkd be performed upon
  RAT change

PARAMETERS 
  reattempt_indicator - Re-Attempt IE, if any
  backoff_timer       - Backoff IE, if any
  subs_id             - Subscription Id

RETURN VALUE 
  TRUE,  if SM Retry on RAT change is allowed
  FALSE, otherwise

SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_pdn_throt_spec_get_sm_retry_on_rat_change
(
  reattempt_ind_T           reattempt_indicator,
  timer_3gpp_type3          backoff_timer,
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION      DS_PDN_THROT_REL13_SPEC_GET_SM_RETRY_ON_RAT_CHANGE

DESCRIPTION 
  This function determines whether SM Retry should be performed upon
  RAT change for release 13 devices 

PARAMETERS 
  reattempt_indicator - Re-Attempt IE, if any
  backoff_timer       - Backoff IE, if any
  subs_id             - Subscription Id

RETURN VALUE 
  TRUE,  if SM Retry on RAT change is allowed
  FALSE, otherwise

SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_pdn_throt_rel13_spec_get_sm_retry_on_rat_change
(
  reattempt_ind_T               reattempt_indicator,
  timer_3gpp_type3              backoff_timer,
  ps_iface_net_down_reason_type down_reason,
  sys_modem_as_id_e_type        subs_id
);

/*===========================================================================
FUNCTION      DS_PDN_THROT_REL13_SPEC_GET_SM_RETRY_ON_EPLMNS

DESCRIPTION 
  This function determines whether SM Retry should be performed upon
  a PLMN change which is in the list of ePLMNs for release 13 devices. 

PARAMETERS 
  reattempt_indicator - Re-Attempt IE, if any
  backoff_timer       - Backoff IE, if any
  subs_id             - Subscription Id

RETURN VALUE 
  TRUE,  if SM Retry on RAT change is allowed
  FALSE, otherwise

SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_pdn_throt_rel13_spec_get_sm_retry_on_eplmns
(
  reattempt_ind_T               reattempt_indicator,
  timer_3gpp_type3              backoff_timer,
  ps_iface_net_down_reason_type down_reason,
  sys_modem_as_id_e_type        subs_id
);

/*===========================================================================
FUNCTION      DS_PDN_THROT_SPEC_PARSE_BACKOFF_INFO

DESCRIPTION 
  This function parses the Backoff IE received from the Network

PARAMETERS 
  backoff_timer       - Backoff timer structure
  reattempt_indicator - Re-Attempt IE, if any
  subs_id             - Subscription Id
  down_reason         - Failure reason
  parsed_info_p       - Pointer to the parsed information

RETURN VALUE 
  None
 
SIDE EFFECTS 
  None
===========================================================================*/
void ds_pdn_throt_spec_parse_backoff_info
(
  timer_3gpp_type3                         backoff_timer,
  reattempt_ind_T                          reattempt_indicator,
  sys_modem_as_id_e_type                   subs_id,
  ps_iface_net_down_reason_type            down_reason,
  ds_pdn_throt_spec_parsed_backoff_info_s  *parsed_info_p,
  sys_sys_mode_e_type                       mode
);
/*===========================================================================
FUNCTION  DS_PDN_THROT_SPEC_FILL_THROT_IDENT_INFO_AND_PERFORM_THROTTLING

DESCRIPTION
  This function  fills the throttling identifiers and invokes PDN Throt Spec
  module to perform Spec-defined throttling

PARAMETERS
  *apn                : APN Name
  pdn_context_p       :pointer to the pdn context on which reject has been received
  down_reason         : down reason
  parsed_info_p       : The parsed info pointer
  proc_type           : the throttling procedure to be applied
  rat_type            : RAT to be throttled
  subs_id             : Subscription Id
  
DEPENDENCIES
  None.

RETURN VALUE 
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_pdn_throt_spec_fill_throt_ident_info_and_perform_throttling
(
  byte                                    *apn,
  ds_pdn_context_s                        *pdn_context_p,
  ps_iface_net_down_reason_type            down_reason,
  ds_pdn_throt_spec_parsed_backoff_info_s *parsed_info_p,
  ds_3gpp_pdn_throt_proc_e_type            proc_type,
  ds_3gpp_throt_rat_e_type                 rat_type,
  sys_modem_as_id_e_type                   subs_id
);
/*===========================================================================
FUNCTION      DS_PDN_THROT_SPEC_POWER_UP_INIT

DESCRIPTION 
  This function performs power up initialization of the PDN Throt Spec module

PARAMETERS 
 None

RETURN VALUE 
  None
 
SIDE EFFECTS 
  None
===========================================================================*/
void ds_pdn_throt_spec_power_up_init
(
  void 
);

/*===========================================================================
FUNCTION DS_PDN_THROT_SPEC_CLEAR_T3396_ENTRY

DESCRIPTION
  This function clears all T3396 entries

PARAMETERS
  subs_id
  clear_throttling

DEPENDENCIES
  void

RETURN VALUE  
  void

SIDE EFFECTS

===========================================================================*/
void ds_pdn_throt_spec_clear_t3396_entry
(
  sys_modem_as_id_e_type subs_id,
  boolean                clear_esm26_throttle,
  boolean                clear_throttling
);
/*===========================================================================

FUNCTION DS_PDN_THROT_SPEC_TIMER3_VALUE_IE_TO_MSECS

DESCRIPTION
  ThIS function converts GPRS timer 3 value IE converted to milliseconds.
  Note that a return value of zero indicates that the timer has been deactivated.

DEPENDENCIES
  None

RETURN VALUE
  uint64 - timer value in milliseconds

SIDE EFFECTS
  None

===========================================================================*/

uint64 ds_pdn_throt_spec_timer3_value_ie_to_msecs
(
  uint8   timer_value,
  boolean *timer_is_active_p
);

#endif /* DS_3GPP_PDN_THROT_SPEC_H */
