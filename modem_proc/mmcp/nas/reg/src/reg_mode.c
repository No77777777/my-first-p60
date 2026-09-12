/*==============================================================================

                             COPYRIGHT INFORMATION

Copyright (c) 2002-2011 Qualcomm Technologies, Incorporated and its licensors.  All Rights
Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                            EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/reg/vcs/reg_mode.c_v   1.8   06 Jun 2002 17:32:22   kabraham  $
$Header: //components/rel/mmcp.mpss/7.9.0/nas/reg/src/reg_mode.c#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
02/13/02   kwa     Initial release.
05/16/02   kwa     Randomized PLMNs with high quality signal strength based on
                   signal strength rather than using a random number generator.
05/20/02   kwa     Replaced calls to ERR with MSG_ERROR.
05/21/02   kwa     Modified output of available PLMN list.
05/22/02   kwa     Support new list types defined in reg_sim.h.
05/29/02   kwa     Changes to support sending a single RAT rather than a mode
                   preference in the mmr_reg_req message.
05/31/02   kwa     Added functionality to initialize the equivalent PLMN list
                   from NVRAM and save the equivalent PLMN list to NVRAM.
06/06/02   kwa     Replaced rat_search_order_p with rat_p in
                   reg_mode_next_higher_priority_plmn
07/30/02   kwa     Changed < to > in reg_mode_sort_plmn_list to account for
                   the fact that signal_strength is returned as the absolute
                   value of a negative number.
08/07/02   kwa     Modified output of available PLMN list.
                   Added check in reg_mode_successful_registration that the
                   PLMN received in the mmr_reg_cnf matches the requested PLMN.
                   Randomized PLMNs with high quality signal strength rather
                   than sorting them based on signal strength to satisfy
                   Telecom Italia Mobile tests of this capability.
08/23/02   kwa     Placed check for zero value divisor when generating a random
                   number in reg_mode_prioritize_plmn_list to eliminate a lint
                   warning.  Minor changes to diagnostic messages used to
                   print out the available PLMN list.
08/26/02   kwa     Removed limited service from the check that registration
                   succeeded in reg_mode_successful_registration.
09/23/02   kwa     Added function reg_mode_allowed_service_domain to handle
                   the allowed service domain if a PLMN is forbidden for GPRS
                   services.
10/01/02   kwa     Updated reg_mode_req_service_domain to return the requeseted
                   service domain filtered by the current SIM/USIM state.
12/30/02   kwa     Updated reg_mode_output_detailed_plmn_list to properly
                   display F3 messages due to changes in DIAG.
12/31/02   kwa     Added function reg_mode_equivalent_plmn.
01/29/03   kwa     Added limited service back into the check that registration
                   succeeded in reg_mode_successful_registration.
02/06/03   kwa     Added check for second RPLMN-RAT combination in
                   reg_mode_next_equivalent_rplmn.  Without this check the MS
                   failed to search for both GSM and WCDMA in manual mode.
04/03/03   kwa     Added function reg_mode_higher_priority_network_available.
05/14/03   kwa     Added code to reg_mode_higher_priority_network_available
                   to initiate a search for the HPLMN when the RPLMN is in the
                   same country as the HPLMN.
05/21/03   kwa     Fixed assorted lint warnings.
06/04/03   kwa     Fixed one remaining lint warning.
09/07/03   kwa     Added function reg_mode_reinitiate_plmn_selection.
                   Updated function reg_mode_unexpected_start_plmn_selection
                   to include the clear_hplmn_search_timer parameter.
09/18/03   kwa     Added reg_mode_band_pref and reg_mode_band_pref_get.
10/03/03   kwa     Changes to support moving some of the equivalent PLMN list
                   functionality to the sys_eplmn_list files.
10/22/03   kwa     Removed the parameters update_equivalent_plmn_list and
                   equivalent_plmn_list_p from reg_mode_finished.  Added code
                   to remove any forbidden PLMNs contained in the equivalent
                   PLMN list before saving it to NVRAM.
10/23/03   kwa     Updated reg_mode_save_equivalent_plmn_list to handle the
                   case when the rplmn is forbidden.
11/16/03   kwa     Added REG_MODE_AUTOMATIC_MODE_MANUAL,
                   reg_mode_available_plmn_list_get, and
                   reg_mode_initial_manual_rat.  Modified the function
                   reg_mode_update_available_plmn_list so it does not remove
                   forbidden PLMNs if used to support manual PLMN selection.
11/18/03   kwa     Fixed bugs which caused higher priority network selection
                   to fail when the mode preference is set to GSM preferred.
11/19/03   kwa     Removed obsolete functions for retrieving the mode name.
01/20/04   kwa     Added reg_mode_start_plmn_selection_on_specified_plmn to
                   properly handle a location updating reject with cause #15.
01/22/04   kwa     Added function reg_mode_network_selection_mode_set.
01/30/04   kwa     Added call to sys_eplmn_list_equivalent_plmn in
                   reg_mode_initialize_equivalent_plmn_list in order to read
                   an nv item for disabling the eplmn feature at power on.
02/02/04   kwa     Removed extern from the definition of the function
                   reg_mode_initial_manual_rat.
02/04/04   kwa     Added function reg_mode_ps_for_request_get.  Added support
                   for PS follow on request.
02/22/04   kwa     Removed extern from the definition of the function
                   reg_mode_band_pref_get.  Fixed use of incorrect enumerated
                   type in reg_mode_next_higher_priority_plmn.
03/17/04   kwa     Added code to delete the equivalent PLMN list from NVRAM
                   if the SIM has changed.
04/04/04   kwa     Removed REG_MODE_IMSI_MAX_LENGTH.
04/28/04   kwa     Fixed a bug in reg_mode_initial_manual_rat which created a
                   condition during manual PLMN selection with RAT that would
                   cause the phone to attempt to acquire service on the
                   incorrect RAT.
05/16/04   kwa     Added function reg_mode_cs_for_request_get.  Added support
                   for CS follow on request.
05/20/04   kwa     Removed the static variable reg_mode_service_state and
                   associated functions.  Added the static variable
                   reg_mode_req_rat and associated functions.  Added calls to
                   reg_mode_req_plmn_set and reg_mode_req_rat_set in the
                   function reg_mode_update_attempted_plmn_list.
06/16/04   kwa     Added code to initialize reg_mode_mode_pref and
                   reg_mode_band_pref.
08/02/04   kwa     Modified the definition of a successful registration in
                   reg_mode_successful_registration to allow the reported RAT
                   to be different than the requested RAT if the phone is
                   operating in dual-mode.
09/13/04   kwa     Added code in reg_mode_next_higher_priority_plmn to indicate
                   that the search for a higher priority PLMN is complete if
                   the next available PLMN in the sorted list is not a
                   preferred PLMN.
11/12/04   kwa     Added the parameter higher_priority_plmn_list_p and support
                   for FEATURE_GTOW_BACKGROUND_PLMN_SEARCH to
                   reg_mode_higher_priority_network_available.
12/07/04   kwa     Modified reg_mode_higher_priority_network_available to
                   account for the case when the HPLMN is an EPLMN.
01/24/05   up      Removed the checking 'if rplmn is undefined' from function
                   reg_mode_next_higher_priority_plmn to allow to get the next
                   priority PLMN at the beginning when RPLMN is not stored.
                   If rplmn is undefined then use mcc of currently serving plmn
                   as the rplmn_mcc to collect priority plmn of same country as
                   currently serving plmn.
01/24/05   up      Added reg_mode_req_mode_pref to store the original
                   requested preferred mode. Also added the function
                   reg_mode_req_mode_pref_get to get the requested pref mode.
                   Updated reg_mode_start_automatic_plmn_selection,
                   reg_mode_start_limited_service,reg_mode_start_plmn_selection
                   reg_mode_start_manual_plmn_selection and 
                   reg_mode_higher_priority_network_available to set the 
                   original preferred mode - reg_mode_req_mode_pref.
02/17/05   up      Modified reg_mode_start_manual_plmn_selection to add a new
                   argument of sys_network_selection_mode_e_type to update
                   n/w selection mode dynamically by the caller. Updated also
                   reg_mode_unexpected_start_manual_plmn_selection for same.
02/22/05   up      Updated for Cingular ENS feature.
                   Added functions reg_mode_restart_plmn_selection. 
                   Updated reg_mode_initialize_equivalent_plmn_list 
                   to delete eplmn list forcefully.
03/22/05   up      Updated function reg_mode_higher_priority_network_available
                   to consider PLMNs having MCC in 310-316 are of same country.
05/18/05   up      Updated function reg_mode_restart_plmn_selection to start 
                   Automatic PLMN selection in HIGH PRIORITY mode.
07/27/05   up      Updated function reg_mode_save_equivalent_plmn_list to 
                   remove the PLMN from ePLMN list if it exists in GPRS
                   forbidden list when MS is in OP mode C.
08/16/05   up      HPLMN search is delayed 10 times when in 3G to reduce the 
                   ignorace of PAGING message during search.
                   This will be removed when BPLMN serach in 3G is supported.
09/05/05   up      Changes for HPLMN search delay is kept under GCF flag.
12/13/05   up      Modified reg_mode_higher_priority_network_available to limit
                   higher priority PLMNs to SYS_PLMN_LIST_MAX_LENGTH size.
01/05/06   sn      Updated reg_mode_plmn_selection, added check to send clear_
                   hplmn_search_timer argument as FALSE if network_selection_
                   mode is AUTOMATIC and TRUE for rest.
02/16/06   sn      EHPLMN Feature. Updated reg_mode_higher_priority_
                   plmn_available, reg_mode_prioritize_plmn_list.
01/31/06   up      Limit HPPLMN list to SYS_PRIORITY_PLMN_LIST_MAX_LENGTH(256).
02/07/06   vdr     Now performing MCC check when FEATURE_DISABLE_MCC_FILTERING
                   macro is turned on.
04/05/06   sn      Updated reg_mode_higher_priority_network_available to take
                   service plmn as rplmn if service state is LIMITED.
04/05/06   sn      Updated reg_mode_higher_priority_network_available to take
                   registered service domain instead of req service domain for
                   getting rplmn.
05/15/06   sn      Updated reg_mode_prioritize_plmn_list. Changed rand() to
                   ran_next(), for better randomization.
06/13/06   sn      Updated reg_mode_higher_priority_network_available to
                   disable mcc based filtering for HPLMN if feature
                   FEATURE_DISABLE_HPLMN_MCC_FILTERING is enabled. 
07/27/06   sn      Added Uniform OOS handling.
                   FEATURE_UMTS_UNIFORM_OOS_HANDLING
10/12/06   sn      WCDMA_1X support
11/23/06   sn      FEATURE_PRESERVE_MANUAL_PLMN_LIST
12/15/09   abhi Removing UOOS code
01/31/11   rajesh  voice domain preference and UE usage setting related changes
08/03/11   jbk      Filter FPLMN from BPLMN search request
08/08/11   jbk      FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
10/10/11   HC      Removed AEEstd.h inclusion as aeestdlib is obsoleted 
                   Replaced the std_strlprintf with C equivalent function snprintf 
11/15/11  abhi   For EFRPLMNSI Selecting RLPMN if HPLMN is not found
11/28/12  am       Fixing compilation warning
7/30/13   pmittal  Replacing ran_next with cryptographically secure mm_secapi_get_random
                   which internally uses secapi_get_random.
==============================================================================*/

/*==============================================================================

                            INCLUDE FILES FOR MODULE

==============================================================================*/
#include "mmcp_variation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stringl/stringl.h>
#include "customer.h"
#include "msg.h"
#include "reg_mode.h"
#include "reg_nv.h"
#include "reg_sim_v.h"
#include "reg_state.h"
#include "sys_eplmn_list_v.h"
#include "reg_ssgccs.h"
#include "reg_ssgccs_api.h"
#include "reg_send.h"
#include "reg_timers.h"
#include "reg_sim.h"
#include "modem_mem.h"

#include "policyman_nas.h"
#include "err.h"
#include "trm.h"
#define REG_MODE_EFNAS_CONF_LEN 30

/*==============================================================================

                                TYPE DEFINITIONS

==============================================================================*/

typedef struct reg_mode_plmn_rat_list_info_s
{
  sys_plmn_id_s_type                        plmn;
  sys_radio_access_tech_e_type              rat;
} reg_mode_plmn_rat_list_info_s_type;

typedef struct reg_mode_plmn_rat_list_s
{
  uint32                                    length;
  reg_mode_plmn_rat_list_info_s_type        info[SYS_PLMN_LIST_MAX_LENGTH];
} reg_mode_plmn_rat_list_s_type;

/*==============================================================================

                          STATIC FUNCTION DECLARATIONS

==============================================================================*/

/*==============================================================================

                               DATA DECLARATIONS

==============================================================================*/

reg_as_id_e_type reg_as_id = REG_AS_ID_1;
byte reg_message_as_id = 1;



#ifdef FEATURE_DUAL_SIM
#if defined(FEATURE_TRIPLE_SIM)  || defined(FEATURE_SGLTE_DUAL_SIM)  /* FEATURE_TRIPLE_SIM */
static boolean reg_mode_selection_to_gw_first_pass_sim[MAX_NAS_STACKS] = {FALSE,FALSE,FALSE};
#define reg_mode_selection_to_gw_first_pass reg_mode_selection_to_gw_first_pass_sim[reg_as_id]
#else
static boolean reg_mode_selection_to_gw_first_pass_sim[MAX_NAS_STACKS] = {FALSE,FALSE};
#define reg_mode_selection_to_gw_first_pass reg_mode_selection_to_gw_first_pass_sim[reg_as_id]
#endif
#else
static boolean reg_mode_selection_to_gw_first_pass;
#endif


#ifdef FEATURE_SGLTE
reg_as_id_e_type  reg_ps_stack_id = REG_AS_ID_1;
reg_as_id_e_type  reg_cs_stack_id = REG_AS_ID_1;
#endif

byte reg_mode_lte_retry_counter = 0;


#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
static sys_rat_pri_list_info_s_type        reg_mode_rat_pri_list_sim[MAX_NAS_STACKS];
#define reg_mode_rat_pri_list              reg_mode_rat_pri_list_sim[reg_as_id]

static sys_rat_pri_list_info_s_type        csfb_start_plmn_sel_rat_pri_list_sim[MAX_NAS_STACKS];
#define csfb_start_plmn_sel_rat_pri_list              csfb_start_plmn_sel_rat_pri_list_sim[reg_as_id]

static sys_radio_access_tech_e_type        csfb_start_plmn_sel_active_rat_sim[MAX_NAS_STACKS];
#define csfb_start_plmn_sel_active_rat              csfb_start_plmn_sel_active_rat_sim[reg_as_id]
#else
static sys_rat_pri_list_info_s_type        reg_mode_rat_pri_list; 
static sys_rat_pri_list_info_s_type        csfb_start_plmn_sel_rat_pri_list; 
static sys_radio_access_tech_e_type        csfb_start_plmn_sel_active_rat;   
#endif


#ifdef FEATURE_DUAL_SIM
#if defined(FEATURE_TRIPLE_SIM)  || defined(FEATURE_SGLTE_DUAL_SIM)  /* FEATURE_TRIPLE_SIM */

boolean reg_mode_is_emergency_call_sim[MAX_NAS_STACKS] = {FALSE,FALSE,FALSE}; 
#define reg_mode_is_emergency_call reg_mode_is_emergency_call_sim[reg_as_id]

#else

boolean reg_mode_is_emergency_call_sim[MAX_NAS_STACKS] = {FALSE,FALSE}; 
#define reg_mode_is_emergency_call reg_mode_is_emergency_call_sim[reg_as_id]

#endif
#else

boolean reg_mode_is_emergency_call = FALSE;

#endif


#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
#if defined(FEATURE_TRIPLE_SIM)  || defined(FEATURE_SGLTE_DUAL_SIM)  /* FEATURE_TRIPLE_SIM */
static uint8 reg_mode_validate_sim_counter_sim[MAX_NAS_STACKS] = {0,0,0};
#define reg_mode_validate_sim_counter reg_mode_validate_sim_counter_sim[reg_as_id]
#else
static uint8 reg_mode_validate_sim_counter_sim[MAX_NAS_STACKS] = {0,0};
#define reg_mode_validate_sim_counter reg_mode_validate_sim_counter_sim[reg_as_id]
#endif
#else
static uint8 reg_mode_validate_sim_counter = 0;
#endif


#ifdef FEATURE_DUAL_SIM
#if defined(FEATURE_TRIPLE_SIM)  || defined(FEATURE_SGLTE_DUAL_SIM)  /* FEATURE_TRIPLE_SIM */
static boolean reg_mode_usim_compliant_sim[MAX_NAS_STACKS] = {FALSE,FALSE,FALSE};
#define reg_mode_usim_compliant reg_mode_usim_compliant_sim[reg_sub_id]
#else
static boolean reg_mode_usim_compliant_sim[MAX_NAS_STACKS] = {FALSE,FALSE};
#define reg_mode_usim_compliant reg_mode_usim_compliant_sim[reg_sub_id]
#endif
#else
static boolean reg_mode_usim_compliant = FALSE;
#endif

#if defined(FEATURE_TRIPLE_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
static sys_network_selection_mode_e_type   reg_mode_network_selection_mode_sim[MAX_NAS_STACKS];
#define reg_mode_network_selection_mode  reg_mode_network_selection_mode_sim[reg_as_id]
static sys_mode_pref_e_type                reg_mode_mode_pref_sim[MAX_NAS_STACKS] = { SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY, SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY,
                                                                                  SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY};
#define reg_mode_mode_pref   reg_mode_mode_pref_sim[reg_as_id]
static sys_mode_pref_e_type                reg_mode_req_mode_pref_sim[MAX_NAS_STACKS] = { SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY, SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY,
                                                                                      SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY};
#define reg_mode_req_mode_pref   reg_mode_req_mode_pref_sim[reg_as_id]
static boolean                             reg_mode_cs_for_request_sim[MAX_NAS_STACKS];
#define reg_mode_cs_for_request    reg_mode_cs_for_request_sim[reg_as_id]
static boolean                             reg_mode_ps_for_request_sim[MAX_NAS_STACKS];
#define reg_mode_ps_for_request   reg_mode_ps_for_request_sim[reg_as_id]
#ifdef FEATURE_GSM_BAND_PREF
static sys_band_mask_type                  reg_mode_band_pref_sim[MAX_NAS_STACKS] ={ SYS_BAND_MASK_EMPTY, SYS_BAND_MASK_EMPTY, SYS_BAND_MASK_EMPTY};
#define reg_mode_band_pref   reg_mode_band_pref_sim[reg_as_id]
#endif
static sys_srv_domain_e_type               reg_mode_req_service_domain_sim[MAX_NAS_STACKS];
#define reg_mode_req_service_domain    reg_mode_req_service_domain_sim[reg_as_id]
static sys_plmn_id_s_type                  reg_mode_req_plmn_sim[MAX_NAS_STACKS];
#define reg_mode_req_plmn       reg_mode_req_plmn_sim[reg_as_id]
static sys_radio_access_tech_e_type        reg_mode_req_rat_sim[MAX_NAS_STACKS];
#define reg_mode_req_rat     reg_mode_req_rat_sim[reg_as_id]

static reg_mode_s_type*                    reg_mode_p_sim[MAX_NAS_STACKS] = { &reg_mode_limited_service,&reg_mode_limited_service, &reg_mode_limited_service};
#define reg_mode_p         reg_mode_p_sim[reg_as_id]

static sys_detailed_plmn_list_s_type       reg_mode_available_plmn_list_sim[MAX_NAS_STACKS];
#define reg_mode_available_plmn_list   reg_mode_available_plmn_list_sim[reg_as_id]

static sys_detailed_plmn_list_s_type       reg_mode_available_plmn_list_manual_sim[MAX_NAS_STACKS];
#define reg_mode_available_plmn_list_manual    reg_mode_available_plmn_list_manual_sim[reg_as_id]

static sys_detailed_plmn_list_s_type*    reg_mode_backgnd_list_sim[MAX_NAS_STACKS]={NULL,NULL,NULL};
#define reg_mode_backgnd_list    reg_mode_backgnd_list_sim[reg_as_id]


static reg_mode_plmn_rat_list_s_type       reg_mode_attempted_plmn_list_sim[MAX_NAS_STACKS];
#define reg_mode_attempted_plmn_list    reg_mode_attempted_plmn_list_sim[reg_as_id]

#ifdef FEATURE_UMTS_1X_HANDOVER_UMTSMSM
#error code not present
#endif

static sys_radio_access_tech_e_type        reg_mode_service_available_rat_sim[MAX_NAS_STACKS] = { SYS_RAT_NONE, SYS_RAT_NONE, SYS_RAT_NONE};
#define reg_mode_service_available_rat    reg_mode_service_available_rat_sim[reg_as_id]

static  boolean  reg_mode_oos_srch_done_sim[MAX_NAS_STACKS][SYS_RAT_MAX];
#define reg_mode_oos_srch_done    reg_mode_oos_srch_done_sim[reg_as_id]

static sys_detailed_plmn_list_info_s_type   reg_mode_managed_roaming_plmn_sim[MAX_NAS_STACKS] = 
{{
  {{0xFF, 0xFF, 0xFF}},
  SYS_RAT_NONE,
  SYS_SRV_CAPABILITY_NOT_AVAILABLE,
  FALSE,
  SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER,
  SYS_SIGNAL_QUALITY_HIGH,
  0
}, {

  {{0xFF, 0xFF, 0xFF}},
  SYS_RAT_NONE,
  SYS_SRV_CAPABILITY_NOT_AVAILABLE,
  FALSE,
  SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER,
  SYS_SIGNAL_QUALITY_HIGH,
  0
},{

  {{0xFF, 0xFF, 0xFF}},
  SYS_RAT_NONE,
  SYS_SRV_CAPABILITY_NOT_AVAILABLE,
  FALSE,
  SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER,
  SYS_SIGNAL_QUALITY_HIGH,
  0
}};
#define reg_mode_managed_roaming_plmn    reg_mode_managed_roaming_plmn_sim[reg_as_id]


static boolean reg_mode_available_plmn_list_populated_sim[MAX_NAS_STACKS] = {FALSE, FALSE, FALSE};
#define reg_mode_available_plmn_list_populated reg_mode_available_plmn_list_populated_sim[reg_as_id]


static reg_mode_srch_tbl_s_type reg_mode_enhanced_hplmn_search_tbl_sim[MAX_NAS_STACKS];
#define reg_mode_enhanced_hplmn_search_tbl reg_mode_enhanced_hplmn_search_tbl_sim[reg_as_id]

static boolean reg_mode_hplmn_to_be_selected_sim[MAX_NAS_STACKS] = {FALSE, FALSE,FALSE};
#define reg_mode_hplmn_to_be_selected reg_mode_hplmn_to_be_selected_sim[reg_as_id]

static sys_scan_scope_e_type reg_mode_scan_scope_sim[MAX_NAS_STACKS] = {SYS_SCAN_SCOPE_FULL_BAND,SYS_SCAN_SCOPE_FULL_BAND, SYS_SCAN_SCOPE_FULL_BAND};
#define reg_mode_scan_scope reg_mode_scan_scope_sim[reg_as_id]

#ifdef FEATURE_NAS_ECALL
extern boolean reg_ecall_profile_sim[MAX_NAS_STACKS];
#define reg_ecall_profile reg_ecall_profile_sim[reg_as_id]
#endif

#ifdef FEATURE_NAS_ENABLE_LTE_BAND_FILTERING
static  reg_mode_lte_scan_e_type  reg_mode_lte_scan_pref_sim[MAX_NAS_STACKS] ={ REG_MODE_LTE_SCAN_NONE, REG_MODE_LTE_SCAN_NONE, REG_MODE_LTE_SCAN_NONE};
#define reg_mode_lte_scan_pref   reg_mode_lte_scan_pref_sim[reg_as_id]
#endif

reg_mode_temp_ehplmn_forbidden_s_type reg_mode_temp_forbidden_list_sim[MAX_NAS_STACKS];
#define reg_mode_temp_forbidden_list reg_mode_temp_forbidden_list_sim[reg_as_id]

reg_mode_temp_ehplmn_gprs_forbidden_s_type reg_mode_temp_gprs_forbidden_list_sim[MAX_NAS_STACKS];
#define reg_mode_temp_gprs_forbidden_list reg_mode_temp_gprs_forbidden_list_sim[reg_as_id]

#elif defined FEATURE_DUAL_SIM  || defined FEATURE_SGLTE  /* FEATURE_DUAL_SIM || FEATURE_SGLTE*/

static sys_network_selection_mode_e_type   reg_mode_network_selection_mode_sim[MAX_NAS_STACKS];
#define reg_mode_network_selection_mode  reg_mode_network_selection_mode_sim[reg_as_id]
static sys_mode_pref_e_type                reg_mode_mode_pref_sim[MAX_NAS_STACKS] = { SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY, SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY};
#define reg_mode_mode_pref   reg_mode_mode_pref_sim[reg_as_id]
static sys_mode_pref_e_type                reg_mode_req_mode_pref_sim[MAX_NAS_STACKS] = { SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY, SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY};
#define reg_mode_req_mode_pref   reg_mode_req_mode_pref_sim[reg_as_id]
static boolean                             reg_mode_cs_for_request_sim[MAX_NAS_STACKS];
#define reg_mode_cs_for_request    reg_mode_cs_for_request_sim[reg_as_id]
static boolean                             reg_mode_ps_for_request_sim[MAX_NAS_STACKS];
#define reg_mode_ps_for_request   reg_mode_ps_for_request_sim[reg_as_id]
#ifdef FEATURE_GSM_BAND_PREF
static sys_band_mask_type                  reg_mode_band_pref_sim[MAX_NAS_STACKS] ={ SYS_BAND_MASK_EMPTY, SYS_BAND_MASK_EMPTY};
#define reg_mode_band_pref   reg_mode_band_pref_sim[reg_as_id]
#endif
static sys_srv_domain_e_type               reg_mode_req_service_domain_sim[MAX_NAS_STACKS];
#define reg_mode_req_service_domain    reg_mode_req_service_domain_sim[reg_as_id]
static sys_plmn_id_s_type                  reg_mode_req_plmn_sim[MAX_NAS_STACKS];
#define reg_mode_req_plmn       reg_mode_req_plmn_sim[reg_as_id]
static sys_radio_access_tech_e_type        reg_mode_req_rat_sim[MAX_NAS_STACKS];
#define reg_mode_req_rat     reg_mode_req_rat_sim[reg_as_id]

static reg_mode_s_type*                    reg_mode_p_sim[MAX_NAS_STACKS] = { &reg_mode_limited_service, &reg_mode_limited_service};
#define reg_mode_p         reg_mode_p_sim[reg_as_id]

static sys_detailed_plmn_list_s_type       reg_mode_available_plmn_list_sim[MAX_NAS_STACKS];
#define reg_mode_available_plmn_list   reg_mode_available_plmn_list_sim[reg_as_id]

static sys_detailed_plmn_list_s_type       reg_mode_available_plmn_list_manual_sim[MAX_NAS_STACKS];
#define reg_mode_available_plmn_list_manual    reg_mode_available_plmn_list_manual_sim[reg_as_id]

static reg_mode_plmn_rat_list_s_type       reg_mode_attempted_plmn_list_sim[MAX_NAS_STACKS];

static sys_detailed_plmn_list_s_type*    reg_mode_backgnd_list_sim[MAX_NAS_STACKS]={NULL,NULL};
#define reg_mode_backgnd_list    reg_mode_backgnd_list_sim[reg_as_id]
#define reg_mode_attempted_plmn_list    reg_mode_attempted_plmn_list_sim[reg_as_id]

#ifdef FEATURE_UMTS_1X_HANDOVER_UMTSMSM
#error code not present
#endif

static sys_radio_access_tech_e_type        reg_mode_service_available_rat_sim[MAX_NAS_STACKS] = { SYS_RAT_NONE, SYS_RAT_NONE};
#define reg_mode_service_available_rat    reg_mode_service_available_rat_sim[reg_as_id]

static  boolean  reg_mode_oos_srch_done_sim[MAX_NAS_STACKS][SYS_RAT_MAX];
#define reg_mode_oos_srch_done    reg_mode_oos_srch_done_sim[reg_as_id]



static sys_detailed_plmn_list_info_s_type   reg_mode_managed_roaming_plmn_sim[MAX_NAS_STACKS] = 
{{
  {{0xFF, 0xFF, 0xFF}},
  SYS_RAT_NONE,
  SYS_SRV_CAPABILITY_NOT_AVAILABLE,
  FALSE,
  SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER,
  SYS_SIGNAL_QUALITY_HIGH,
  0
}, {

  {{0xFF, 0xFF, 0xFF}},
  SYS_RAT_NONE,
  SYS_SRV_CAPABILITY_NOT_AVAILABLE,
  FALSE,
  SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER,
  SYS_SIGNAL_QUALITY_HIGH,
  0
}};
#define reg_mode_managed_roaming_plmn    reg_mode_managed_roaming_plmn_sim[reg_as_id]

static boolean reg_mode_available_plmn_list_populated_sim[MAX_NAS_STACKS] = {FALSE, FALSE};
#define reg_mode_available_plmn_list_populated reg_mode_available_plmn_list_populated_sim[reg_as_id]


static reg_mode_srch_tbl_s_type reg_mode_enhanced_hplmn_search_tbl_sim[MAX_NAS_STACKS];
#define reg_mode_enhanced_hplmn_search_tbl reg_mode_enhanced_hplmn_search_tbl_sim[reg_as_id]

static boolean reg_mode_hplmn_to_be_selected_sim[MAX_NAS_STACKS] = {FALSE, FALSE};
#define reg_mode_hplmn_to_be_selected reg_mode_hplmn_to_be_selected_sim[reg_as_id]

static sys_scan_scope_e_type reg_mode_scan_scope_sim[MAX_NAS_STACKS] = {SYS_SCAN_SCOPE_FULL_BAND,SYS_SCAN_SCOPE_FULL_BAND};
#define reg_mode_scan_scope reg_mode_scan_scope_sim[reg_as_id]

#ifdef FEATURE_NAS_ECALL
extern boolean reg_ecall_profile_sim[MAX_NAS_STACKS];
#define reg_ecall_profile reg_ecall_profile_sim[reg_as_id]
#endif

#ifdef FEATURE_NAS_ENABLE_LTE_BAND_FILTERING
static reg_mode_lte_scan_e_type  reg_mode_lte_scan_pref_sim[MAX_NAS_STACKS] ={ REG_MODE_LTE_SCAN_NONE, REG_MODE_LTE_SCAN_NONE};
#define reg_mode_lte_scan_pref   reg_mode_lte_scan_pref_sim[reg_as_id]
#endif
//FR38535
reg_mode_temp_ehplmn_forbidden_s_type reg_mode_temp_forbidden_list_sim[MAX_NAS_STACKS];
#define reg_mode_temp_forbidden_list reg_mode_temp_forbidden_list_sim[reg_as_id]

reg_mode_temp_ehplmn_gprs_forbidden_s_type reg_mode_temp_gprs_forbidden_list_sim[MAX_NAS_STACKS];
#define reg_mode_temp_gprs_forbidden_list reg_mode_temp_gprs_forbidden_list_sim[reg_as_id]
#else

static sys_network_selection_mode_e_type   reg_mode_network_selection_mode;
static sys_mode_pref_e_type                reg_mode_mode_pref = SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY;
static sys_mode_pref_e_type                reg_mode_req_mode_pref = SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY;
static boolean                             reg_mode_cs_for_request;
static boolean                             reg_mode_ps_for_request;
#ifdef FEATURE_GSM_BAND_PREF
static sys_band_mask_type                  reg_mode_band_pref = SYS_BAND_MASK_EMPTY;
#endif

static sys_scan_scope_e_type reg_mode_scan_scope = SYS_SCAN_SCOPE_FULL_BAND;        

static sys_srv_domain_e_type               reg_mode_req_service_domain;
static sys_plmn_id_s_type                  reg_mode_req_plmn;
static sys_radio_access_tech_e_type        reg_mode_req_rat;

static reg_mode_s_type*                    reg_mode_p = &reg_mode_limited_service;

static sys_detailed_plmn_list_s_type       reg_mode_available_plmn_list;

static sys_detailed_plmn_list_s_type       reg_mode_available_plmn_list_manual;

sys_detailed_plmn_list_s_type*      reg_mode_backgnd_list=NULL;



static reg_mode_plmn_rat_list_s_type       reg_mode_attempted_plmn_list;

#ifdef FEATURE_UMTS_1X_HANDOVER_UMTSMSM
#error code not present
#endif

static sys_radio_access_tech_e_type        reg_mode_service_available_rat = SYS_RAT_NONE;

static  boolean  reg_mode_oos_srch_done[SYS_RAT_MAX];

static sys_detailed_plmn_list_info_s_type   reg_mode_managed_roaming_plmn = 
{
  {{0xFF, 0xFF, 0xFF}},
  SYS_RAT_NONE,
  SYS_SRV_CAPABILITY_NOT_AVAILABLE,
  FALSE,
  SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER,
  SYS_SIGNAL_QUALITY_HIGH,
  0
};

static boolean reg_mode_available_plmn_list_populated = FALSE;

/* Global table to store Higher Priority PLMN search Config list from NV*/
static reg_mode_srch_tbl_s_type reg_mode_enhanced_hplmn_search_tbl;

static boolean reg_mode_hplmn_to_be_selected = FALSE;

#ifdef FEATURE_NAS_ECALL
extern boolean reg_ecall_profile;
#endif

#ifdef FEATURE_NAS_ENABLE_LTE_BAND_FILTERING
static reg_mode_lte_scan_e_type               reg_mode_lte_scan_pref = REG_MODE_LTE_SCAN_NONE;
#endif

//FR38535
reg_mode_temp_ehplmn_forbidden_s_type reg_mode_temp_forbidden_list;
reg_mode_temp_ehplmn_gprs_forbidden_s_type reg_mode_temp_gprs_forbidden_list;
#endif /* FEATURE_DUAL_SIM ||  FEATURE_SGLTE*/

#define REG_MODE_OUTPUT_BUFFER_LENGTH 50

static char reg_mode_output_buffer[REG_MODE_OUTPUT_BUFFER_LENGTH+1];

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
#define sys_eplmn_list_rplmn() (sys_eplmn_per_nas_stacks_list_rplmn((sys_modem_as_id_e_type)reg_as_id))
#define sys_eplmn_list_equivalent_plmn(a) sys_eplmn_per_nas_stacks_list_equivalent_plmn(a,(sys_modem_as_id_e_type)reg_sub_id,(sys_modem_as_id_e_type)reg_as_id) 
#define sys_eplmn_list_update(a,b) sys_eplmn_per_nas_stacks_list_update(a,b,(sys_modem_as_id_e_type)reg_as_id) 
#define sys_eplmn_list_validate(a) sys_eplmn_per_nas_stacks_list_validate(a,(sys_modem_as_id_e_type)reg_as_id)
#define sys_eplmn_list_compare_eplmn_list(a,b) sys_eplmn_per_nas_stacks_list_compare_eplmn_list(a,b, (sys_modem_as_id_e_type)reg_as_id)
#define sys_eplmn_list_equivalent_rplmn(a,b) sys_eplmn_per_nas_stacks_list_equivalent_rplmn(a,b, (sys_modem_as_id_e_type)reg_as_id)
#define sys_eplmn_list_get(a,b) sys_eplmn_per_nas_stacks_list_get(a,b,(sys_modem_as_id_e_type)reg_as_id)
#endif

#if defined(FEATURE_DUAL_SIM)|| defined(FEATURE_SGLTE)
// This variable represents the SIM subscription.
reg_as_id_e_type reg_sub_id_stack[MAX_NAS_STACKS];
#endif
reg_as_id_e_type reg_sub_id = REG_AS_ID_1;



#ifdef FEATURE_ENHANCED_NW_SELECTION 
#define reg_sim_read_ens_flag() reg_sim_per_nas_stacks_read_ens_flag((sys_modem_as_id_e_type)reg_as_id)
#endif


#if defined(FEATURE_TRIPLE_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
static boolean                reg_mode_bst_status_sim[MAX_NAS_STACKS] = {FALSE, FALSE,FALSE};
#define reg_mode_bst_status   reg_mode_bst_status_sim[reg_as_id]
#elif defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
static boolean                reg_mode_bst_status_sim[MAX_NAS_STACKS] = {FALSE, FALSE};
#define reg_mode_bst_status   reg_mode_bst_status_sim[reg_as_id]
#else
static boolean reg_mode_bst_status = FALSE;
#endif



#ifdef FEATURE_EOOS
#error code not present
#endif
#ifdef FEATURE_LTE
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
static sys_rat_pri_list_info_s_type  reg_mode_curr_rat_pri_list_sim[MAX_NAS_STACKS];
#define reg_mode_curr_rat_pri_list reg_mode_curr_rat_pri_list_sim[reg_as_id]

static boolean reg_mode_sms_only_sim[MAX_NAS_STACKS];
#define reg_mode_sms_only reg_mode_sms_only_sim[reg_as_id]

static sys_lte_cs_domain_param_s_type reg_mode_lte_cs_domain_param_sim[MAX_NAS_STACKS];
#define reg_mode_lte_cs_domain_param reg_mode_lte_cs_domain_param_sim[reg_as_id]

static uint32 reg_mode_additional_info_sim[MAX_NAS_STACKS];
#define reg_mode_additional_info reg_mode_additional_info_sim[reg_as_id]
#else
static sys_rat_pri_list_info_s_type  reg_mode_curr_rat_pri_list;
static boolean reg_mode_sms_only;
static sys_lte_cs_domain_param_s_type reg_mode_lte_cs_domain_param;
static uint32 reg_mode_additional_info;
#endif 
#endif 

#ifdef FEATURE_FEMTO_CSG
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
sys_csg_id_type   reg_mode_req_csg_id_sim[MAX_NAS_STACKS];
#define reg_mode_req_csg_id reg_mode_req_csg_id_sim[reg_as_id]
#else
sys_csg_id_type   reg_mode_req_csg_id;
#endif /*FEATURE_DUAL_SIM || defined FEATURE_SGLTE */
#endif /*FEATURE_FEMTO_CSG*/

#ifdef FEATURE_LTE
extern boolean mm_check_connection_active
(
  sys_modem_as_id_e_type sub_id,
  sys_modem_as_id_e_type stack_id_nas
);
#endif

extern boolean mm_is_call_or_registration_ongoing
(
  void
);


#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
reg_mode_3gpp_csfb_rat_list_s_type csfb_failure_rat_search_order_sim[MAX_NAS_STACKS] = {{0}, {0}};
#define csfb_failure_rat_search_order csfb_failure_rat_search_order_sim[reg_as_id]
#else
reg_mode_3gpp_csfb_rat_list_s_type csfb_failure_rat_search_order = {0};
#endif


#if defined(FEATURE_SGLTE) || defined(FEATURE_SVLTE_DUAL_SIM)
extern rex_timer_cnt_type mm_get_ps_timer_value(void);
#endif 

#ifdef FEATURE_SVLTE_DUAL_SIM
boolean reg_mode_timed_service_req_sim[MAX_NAS_STACKS];
#define reg_mode_timed_service_req reg_mode_timed_service_req_sim[reg_as_id]
#endif

#ifdef FEATURE_LTE
#if defined FEATURE_DUAL_SIM || defined FEATURE_TRIPLE_SIM
boolean reg_mode_volte_call_connected_sim[MAX_NAS_STACKS];
#define reg_mode_volte_call_connected reg_mode_volte_call_connected_sim[reg_as_id]
#else
boolean reg_mode_volte_call_connected;
#endif
#endif

#if defined(FEATURE_SVLTE_DUAL_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
boolean reg_mode_service_req_aborted_sim[MAX_NAS_STACKS];
#define reg_mode_service_req_aborted reg_mode_service_req_aborted_sim[reg_as_id]
#endif 

extern nas_crit_sect_type reg_tdscdma_op_list_crit_sect;
/*==============================================================================

                          STATIC FUNCTION DEFINITIONS

==============================================================================*/
static void reg_mode_filter_low_priority_plmn
(
  sys_detailed_plmn_list_s_type* plmn_list,
  boolean                        add_managed_roaming_plmn
)
{
  uint16 i,j;
  const reg_sim_preferred_plmn_list_s_type* low_priority_list = reg_sim_low_priority_plmn_list_get();
  
  if(add_managed_roaming_plmn == TRUE)
  {
    /* Add current managed roaming PLMN into the available list if it is defined */
    if(!sys_plmn_id_is_undefined(reg_mode_managed_roaming_plmn.plmn))
    {
      if(plmn_list->length == SYS_PLMN_LIST_MAX_LENGTH)
      {
        plmn_list->length--;
      }
      plmn_list->info[plmn_list->length].plmn = reg_mode_managed_roaming_plmn.plmn;
      plmn_list->info[plmn_list->length].rat    = reg_mode_managed_roaming_plmn.rat;
      plmn_list->length++;
    }
  }
  
  /* Unset Managed roaming PLMN */
  sys_plmn_undefine_plmn_id(&(reg_mode_managed_roaming_plmn.plmn));
  reg_mode_managed_roaming_plmn.rat= SYS_RAT_NONE;

  for(i = 0; i<low_priority_list->length; i++)
  {
    for(j=0;j<plmn_list->length;j++)
    {
      if(sys_plmn_match(low_priority_list->info[i].plmn, plmn_list->info[j].plmn) &&
         low_priority_list->info[i].rat == plmn_list->info[j].rat)
      {
        /* Keep this entry as most recent low priority plmn rat */
        reg_mode_managed_roaming_plmn = plmn_list->info[j];

        plmn_list->length--; /* decrement length */
        /* remove the current entry from plmn list */
        while(j<plmn_list->length)
        {
          plmn_list->info[j] = plmn_list->info[j+1];
          j++;
        }
      }
    }
  }

  /* Insert first low priority plmn if any low priority plmn is found */
  if(!sys_plmn_id_is_undefined(reg_mode_managed_roaming_plmn.plmn))
  {
    boolean plmn_id_is_undefined;
    boolean mnc_includes_pcs_digit;
    uint32  mcc;
    uint32  mnc;
    
    /* Print the managed roaming plmn if it is defined */
    sys_plmn_get_mcc_mnc
    (
      reg_mode_managed_roaming_plmn.plmn,
      &plmn_id_is_undefined,
      &mnc_includes_pcs_digit,
      &mcc,
      &mnc
    );
    MSG_HIGH_DS_0(REG_SUB, "=REG= Managed Roaming. PLMN");
    if(reg_mode_managed_roaming_plmn.rat == SYS_RAT_GSM_RADIO_ACCESS)
    {
       if(mnc_includes_pcs_digit)
       {
         MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu       GSM", i, mcc, mnc);
       }
       else
       {
         MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu      GSM", i, mcc, mnc);
       }
    }
    else
    {
       if(mnc_includes_pcs_digit) 
       {
         MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu       UMTS", i, mcc, mnc);
       }
       else
       {
         MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu      UMTS", i, mcc, mnc);
       }
    }
  }
}
/*====================================================================
Function: reg_eplmn_list_equivalent_plmn()

Description:

======================================================================*/
boolean reg_eplmn_list_equivalent_plmn(sys_plmn_id_s_type plmn)
{
#if defined FEATURE_DUAL_SIM
	  return sys_eplmn_per_nas_stacks_list_equivalent_plmn(plmn,reg_sub_id,reg_as_id);
#else
	  return sys_eplmn_list_equivalent_plmn(plmn);
#endif
}

/*==============================================================================

FUNCTION NAME

  reg_mode_filter_china_hplmn

DESCRIPTION

  Function that updates the available plmn list for china hplmn or rplmn

RETURN VALUE

none

==============================================================================*/

void reg_mode_filter_china_hplmn
(
  sys_detailed_plmn_list_s_type* plmn_list
)
{
  uint16 i,j;

  for(i=0; (i< plmn_list->length) && (i < SYS_PLMN_LIST_MAX_LENGTH); i++)
  {
    if(reg_mode_is_china_mcc(plmn_list->info[i].plmn) ||
#ifdef FEATURE_EQUIVALENT_HPLMN  
          (reg_sim_is_ehplmn(plmn_list->info[i].plmn))
#else
          (sys_plmn_match(reg_sim_read_hplmn(),plmn_list->info[i].plmn))
#endif
      )
    {
      j = i;
      plmn_list->length--; /* decrement length */
      /* remove the current entry from plmn list */
      while((j<plmn_list->length) && (j < SYS_PLMN_LIST_MAX_LENGTH - 1))
      {
        plmn_list->info[j] = plmn_list->info[j+1];
        j++;
      }
      i--;
    }
  }
}

/*==============================================================================

FUNCTION NAME

  reg_mode_filter_bsr_hplmn

DESCRIPTION

  Function that updates the available plmn list for PLMNs which are not allowed by CM's priority order

RETURN VALUE

none

==============================================================================*/

void reg_mode_filter_bsr_plmn
(
  sys_detailed_plmn_list_s_type* plmn_list
)
{
  uint16 i,j,p;
  p=0;

  for(i=0; (i< plmn_list->length) && (i < SYS_PLMN_LIST_MAX_LENGTH); i++)
  {

    if(!(cm_per_subs_is_plmn_rat_allowed(plmn_list->info[i].plmn,plmn_list->info[i].rat,(sys_modem_as_id_e_type)reg_sub_id,reg_set_cm_stack_id())))
    {

      j = i;
      plmn_list->length--; /* decrement length */
      p++;  /*Keeps track of plmns filtered*/
      /* remove the current entry from plmn list */
      while((j<plmn_list->length) && (j < SYS_PLMN_LIST_MAX_LENGTH - 1))
      {
        plmn_list->info[j] = plmn_list->info[j+1];
        j++;
      }
      i--;
    }
  }
   MSG_HIGH_DS_1(REG_SUB,"=REG= Filtered %d plmn in reg_mode_filter_bsr_plmn",p);

  
}
/*==============================================================================

FUNCTION NAME

  reg_mode_update_vplmn_plmn_rat

DESCRIPTION

  Function that updates the vplmn rat to the global and writes the plmn rat to EFS as well

RETURN VALUE

none

==============================================================================*/

int16 reg_mode_update_vplmn_plmn_rat
(
  sys_plmn_id_s_type plmn,
  sys_radio_access_tech_e_type rat
)
{
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;
  int16 vplmn_length = 0; 
  int16 i=0,j=0,k=0;
  if(!reg_nv_vplmn_list)
  {
    MSG_FATAL_DS(REG_SUB, "=REG= Global Structure is not allocated memory during power up", 0, 0, 0);
  }
  if(sys_plmn_id_is_undefined(plmn))
  {
    return 0;
  }

  vplmn_length = MIN(REG_NV_MAX_VPLMN_LENGTH,reg_nv_get_vplmn_list_length());

  MSG_HIGH_DS_1(REG_SUB, "=REG= VPLMN: Length %d",vplmn_length);
  if( vplmn_length == -1)
  {
    MSG_HIGH_DS_1(REG_SUB, "=REG= VPLMN: Length and vplmn list not initialized %d",vplmn_length);
    return 0;
  }
  for( i =0; (i < vplmn_length) && (i < REG_NV_MAX_VPLMN_LENGTH); i++)
  {
    if(sys_plmn_match(reg_nv_vplmn_list->vplmn_info[i].plmn , plmn) &&
        reg_nv_vplmn_list->vplmn_info[i].rat == rat)
    {
      for( k=i;k>0;k--)
      {
        reg_nv_vplmn_list->vplmn_info[k].plmn = reg_nv_vplmn_list->vplmn_info[k-1].plmn;
        reg_nv_vplmn_list->vplmn_info[k].rat = reg_nv_vplmn_list->vplmn_info[k-1].rat;
      }
      reg_nv_vplmn_list->vplmn_info[k].plmn = plmn;
      reg_nv_vplmn_list->vplmn_info[k].rat = rat;
      MSG_HIGH_DS_0(REG_SUB, "=REG= PLMN alreay in the VPLMN list ,Moved to the top");
      status = reg_nv_write_vplmn_list();
      if(status == MCFG_FS_STATUS_OK)
      {
        MSG_HIGH_DS_0(REG_SUB, "=REG= VPLMN: Writing VPLMN list success");
        reg_nv_output_vplmn_list();
      }
      else
      {
        MSG_HIGH_DS_1(REG_SUB, "=REG= VPLMN: Writing VPLMN list Failed status = %d", status);
      }
      return 0;
    }
  }
  if(vplmn_length < REG_NV_MAX_VPLMN_LENGTH)
  {
    j = vplmn_length;
    reg_nv_vplmn_list->vplmn_length++;
  }
  else 
  {
    j = REG_NV_MAX_VPLMN_LENGTH-1;
    reg_nv_vplmn_list->vplmn_length = REG_NV_MAX_VPLMN_LENGTH;
  }
  for(i=j;i>0;i--)
  {
    reg_nv_vplmn_list->vplmn_info[i].plmn = reg_nv_vplmn_list->vplmn_info[i-1].plmn;
    reg_nv_vplmn_list->vplmn_info[i].rat = reg_nv_vplmn_list->vplmn_info[i-1].rat;
  }
  reg_nv_vplmn_list->vplmn_info[0].plmn = plmn;
  reg_nv_vplmn_list->vplmn_info[0].rat = rat;
  status = reg_nv_write_vplmn_list();
  if(status == MCFG_FS_STATUS_OK)
  {
     MSG_HIGH_DS_0(REG_SUB, "=REG= VPLMN: Writing VPLMN list success");
     reg_nv_output_vplmn_list();
  }
  else
  {
    MSG_HIGH_DS_1(REG_SUB, "=REG= VPLMN: Writing VPLMN list Failed status %d",status);
  }
 return 0;
}
/*==============================================================================

FUNCTION NAME

  reg_mode_is_cm_service_request_same_as_last_except_call_for_request

DESCRIPTION

  Function that compares the current cm service request with the last mmr reg request sent except cs for request and ps for request flag.

RETURN VALUE

  boolean  Flag indicating result of comparision (TRUE/FALSE)

==============================================================================*/
boolean reg_mode_is_cm_service_request_same_as_last_except_call_for_request
(
  const cm_service_req_s_type* msg_p
)
{
#ifdef FEATURE_EQUIVALENT_HPLMN
  reg_sim_plmn_list_s_type* ehplmn_list;
#endif

  if((msg_p->network_selection_mode != reg_mode_network_selection_mode_get()) ||
     (msg_p->req_service_domain != reg_mode_req_service_domain) ||

     (reg_mode_rat_pri_list_all_params_match_except_bst_for_LTE(&msg_p->rat_pri_list_info, reg_mode_rat_pri_list_get()) != TRUE) ||
     ((reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_LTE_RADIO_ACCESS,
                      &msg_p->rat_pri_list_info, FALSE))&&
      (reg_mode_is_bst_set_for_rat(&msg_p->rat_pri_list_info,SYS_RAT_LTE_RADIO_ACCESS) == TRUE) &&
      (reg_mode_is_bst_set_for_rat(reg_mode_rat_pri_list_get(),SYS_RAT_LTE_RADIO_ACCESS) == FALSE)) ||

#ifdef FEATURE_LTE
     (msg_p->sms_only != reg_mode_sms_only_get()) ||
     (reg_mode_lte_cs_domain_param_match(msg_p->lte_cs_domain_param,reg_mode_lte_cs_domain_param_get()) != TRUE) ||
     (msg_p->additional_info != reg_mode_additional_info_get()) ||
#endif
     (msg_p->scan_scope != reg_mode_scan_scope_get())
    )
  {
    return FALSE;
  }

  //Based on the requirement in future following can be extended to other net sel mode and corresponding selection types
  if(msg_p->network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC)
  {
    return TRUE;
  }
#ifdef FEATURE_EQUIVALENT_HPLMN
  ehplmn_list = (reg_sim_plmn_list_s_type *)modem_mem_calloc(1, sizeof(reg_sim_plmn_list_s_type), MODEM_MEM_CLIENT_NAS);
  reg_check_for_null_ptr((void*)ehplmn_list);
  /* Reading EHPLMN list */
  reg_sim_ehplmn_list_get_ptr(ehplmn_list);
#endif 
  if(msg_p->network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
  {
    if((msg_p->parameters.manual.selected_plmn_type == REG_CM_MANUALLY_SELECTED_HPLMN) &&
#ifdef FEATURE_EQUIVALENT_HPLMN
       (sys_plmn_match(ehplmn_list->plmn[0], reg_mode_req_plmn_get())))
#else
       (sys_plmn_match(reg_sim_hplmn_get(), reg_mode_req_plmn_get())))
#endif
    {
#ifdef FEATURE_EQUIVALENT_HPLMN
      modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
      return TRUE;
    }
    else if((msg_p->parameters.manual.selected_plmn_type == REG_CM_MANUALLY_SELECTED_SPECIFIED_PLMN) &&
            (sys_plmn_match(msg_p->parameters.manual.plmn,reg_mode_req_plmn_get())))
    {
#ifdef FEATURE_EQUIVALENT_HPLMN
      modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
      return TRUE;
    }
  }
#ifdef FEATURE_EQUIVALENT_HPLMN
      modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
  return FALSE;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_found_attempted_plmn_rat

DESCRIPTION

  Function that searches for the specified PLMN and RAT in the attempted
  PLMN list.

RETURN VALUE

  boolean  Flag indicating whether the PLMN-RAT combination was found in the
           attempted PLMN list (TRUE) or not (FALSE).

==============================================================================*/

boolean reg_mode_found_attempted_plmn_rat
(
  sys_plmn_id_s_type           plmn,
  sys_radio_access_tech_e_type rat
)
{
  uint32 i;

  for (i = 0; i < reg_mode_attempted_plmn_list.length; i++)
  {
    if ( (sys_plmn_match(reg_mode_attempted_plmn_list.info[i].plmn, plmn) ) &&
         (reg_mode_attempted_plmn_list.info[i].rat == rat) )
    {
      return TRUE;
    }
  }

  return FALSE;
}
/*==============================================================================

FUNCTION NAME

  reg_mode_compare_rat_priority

PARAMETERS 
    rat1, rat2 - two rats to be compared.
    rat_priority_list- 

DESCRIPTION

  Function that compares rat priorty in rat_priorty_list for rat1 and rat2.

RETURN VALUE

  True if rat1 is higher priority
  False if rat2 is higher priorty

==============================================================================*/

boolean reg_mode_compare_rat_priority
(
  sys_radio_access_tech_e_type rat1, 
  sys_radio_access_tech_e_type rat2,
  const reg_mode_3gpp_capability_s_type *rat_priority_list
)
{
  uint8 i;
  boolean ret_val = FALSE;
  
  for (i = 0; i < rat_priority_list->num_items; i++)
  {
    if(rat_priority_list->rat_list[i] == rat2)
    {
      ret_val = FALSE;      
      break;
    }
    else if(rat_priority_list->rat_list[i] == rat1)
    {
      ret_val = TRUE;
      break; 
    }
  }
  return ret_val;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_sort_plmn_list

DESCRIPTION

  Function that sorts a portion of a detailed PLMN list based on the signal
  strength and requested mode preference.

RETURN VALUE

  None

==============================================================================*/

static void reg_mode_sort_plmn_list
(

  const sys_rat_pri_list_info_s_type  *rat_pri_list_info,

  sys_detailed_plmn_list_s_type* plmn_list_p,
  int32                          i_min,
  int32                          i_max
)
{
  sys_detailed_plmn_list_info_s_type temp_plmn_list_info;
  int32 i;
  int32 j;


  reg_mode_3gpp_capability_s_type rat_priority_list;
  reg_mode_get_3gpp_rat_capabilty(&rat_priority_list, rat_pri_list_info
#ifdef FEATURE_SGLTE
  ,FALSE
#endif                                  
  );
 
  for (i = i_min + 1; i < i_max; i++)
  {
    temp_plmn_list_info = plmn_list_p->info[i];

    j = i;

    /*
    ** If the signal strengths are different, then the PLMNs are sorted
    ** solely based on the signal strength.  If the signal strengths
    ** are equal, then the PLMNs are sorted based on the mode
    ** preference and radio access technologies.
    */
    while (
            (j > i_min) &&
            (
              (plmn_list_p->info[j-1].signal_strength > temp_plmn_list_info.signal_strength) ||
              (
                (plmn_list_p->info[j-1].signal_strength == temp_plmn_list_info.signal_strength) &&

               (reg_mode_compare_rat_priority(temp_plmn_list_info.rat, 
                            plmn_list_p->info[j-1].rat, &rat_priority_list))
           
              )
            )
          )

    {
      plmn_list_p->info[j] = plmn_list_p->info[j-1];
      j--;
    }

    plmn_list_p->info[j] = temp_plmn_list_info;
  }

}


/*===========================================================================

FUNCTION reg_mode_enhanced_hplmn_search_parse_mcc_set

DESCRIPTION
  This function parses the MCC set record from the given byte stream (r_data)
  and inserts it into given srch_tbl rec.
  Format of byte stream:
  r_data[0] - Number of MCCs in this record.
  r_data[1] - 0xAB, A = MCC Digit2, B=MCC Digits1
  r_data[2] - 0xFC, F - Not Used(F), C= MCC Digits3
  ...so on.
  A, B, C should be digits 0-9.

DEPENDENCIES
  none

RETURN VALUE
  offset to the end of the record.

SIDE EFFECTS
  none

===========================================================================*/
static uint32 reg_mode_enhanced_hplmn_search_parse_mcc_set(reg_mode_srch_tbl_mcc_set_s_type* rec, byte* r_data)
{
  byte temp, no_entry;
  byte mcc[3];
  uint32 offset =0;

  no_entry = *(r_data);
  rec->no_of_mcc = 0;
  offset+=1;

  for(temp = 0; temp<no_entry; temp++)
  {
    /* Parse the MCC record Stored in 2 bytes for each record, in 
    ** nibble format ie 0x"21 F3" for MCC 123>
    */
    
    mcc[0] = ((*(r_data + offset)) & 0x0F);
    mcc[1] = ((*(r_data + offset)) & 0xF0) >> 4;
    mcc[2] = (*(r_data + offset + 1)) & 0x0F;

    /* MCC value 0 -9 is valid. F in all digits means, 
    ** If all MNC digit as F, then there should only be one such entry 
    ** in the list, it is default record. All other undefined values are ignored.
    */
    if(mcc[0] <= 0x9 && 
       mcc[1] <= 0x9  &&
       mcc[2] <= 0x9)
    {
      rec->mcc[rec->no_of_mcc] = 100 * mcc[0] + 10 * mcc[1] + mcc[2];
      rec->no_of_mcc++;
    }

    /* Increment offset by 2 for next record */
    offset+=2;
    
  }
  
  return offset;
  
}

/*===========================================================================

FUNCTION reg_mode_enhanced_hplmn_search_parse_tbl

DESCRIPTION
  This function parses the given byte stream from NV (nv_config_tbl) to generate internal
  enhanced hplmn search tbl.
  ----------------------------------------------------------------
  NV Encoding Rule:
  ----------------------------------------------------------------
  Byte   Field Name            Description
  ----------------------------------------------------------------
    1    Encoding Type         This described how the data is encoded.
       ( 1=SET Type, other     SET type means there are set of MCCs
       values are for future use)  in records.there are set of MCCs in records.
    2    Number of Records     How Many records are there (applicable for SET TYPE)
    3    Number of MCCs in record Number of MCC in record - 1
    4    MCC digit 2 | MCC digit 1  MCC1 in record-1, 3 digits MCCs stored in 2 bytes.
    5    Not Used (F)| MCC digit 3
    6    MCC digit 2 | MCC digit 1  MCC2 in record-1, 3 digits MCCs stored in 2 bytes.
    7    Not Used (F)| MCC digit 3
  ...so on...

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void reg_mode_enhanced_hplmn_search_parse_tbl(byte *nv_config_tbl)
{
  byte* ptr = nv_config_tbl;
  byte temp;

  /* Read the config value <type of records in nv_config_tbl> */
  reg_mode_enhanced_hplmn_search_tbl.hdr.config = *(ptr);
  ptr+=1;

  /* Read number of records in nv_config_tbl */
  reg_mode_enhanced_hplmn_search_tbl.hdr.no_of_rec = *(ptr);
  ptr+=1;

  if(reg_mode_enhanced_hplmn_search_tbl.hdr.config == TBL_MCC_SET)
  {
    if(reg_mode_enhanced_hplmn_search_tbl.hdr.no_of_rec > 0)
    {
      if(reg_mode_enhanced_hplmn_search_tbl.hdr.no_of_rec > MAX_MCC_SET_IN_TBL)
      {
        /* If number of set exceed MAX allowed, limit it */
        MSG_HIGH_DS_1(REG_SUB, "=REG= Limiting TBL no_of_sets to %d", 
                 MAX_MCC_SET_IN_TBL);
        reg_mode_enhanced_hplmn_search_tbl.hdr.no_of_rec = MAX_MCC_SET_IN_TBL;
        
      }
      /* Populate individual mcc sets */
      for(temp = 0; temp<reg_mode_enhanced_hplmn_search_tbl.hdr.no_of_rec;temp++)
      {
        /* Each MCC record is populated and increment the pointer by offset returned.*/
        ptr+=reg_mode_enhanced_hplmn_search_parse_mcc_set(
                          &(reg_mode_enhanced_hplmn_search_tbl.tbl.mcc_set[temp]), ptr);
        MSG_HIGH_DS_2(REG_SUB, "=REG= Number of MCCs - %d in SET[%d]", 
                      reg_mode_enhanced_hplmn_search_tbl.tbl.mcc_set[temp].no_of_mcc, temp);
      }
    }
    else
    {
       reg_mode_enhanced_hplmn_search_tbl.hdr.no_of_rec = 0;
    }
  }
  else if (reg_mode_enhanced_hplmn_search_tbl.hdr.config == TBL_ALL_MCC)
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= MCC filtering is Disabled - ALL MCCs will be considered for HPPLMN search");
  }
  else
  {
    MSG_HIGH_DS_1(REG_SUB, "=REG= Wrong config hdr in enhanced hplmn search nv_config_tbl %d", 
              reg_mode_enhanced_hplmn_search_tbl.hdr.config);
    reg_mode_enhanced_hplmn_search_tbl.hdr.no_of_rec = 0;
  }
  return;
}


/*===========================================================================

FUNCTION reg_mode_enhanced_hplmn_search_find_mcc_in_set

DESCRIPTION
  This function checks if given MCC is present in given mcc_set.
  <This function is only valid for config type TBL_MCC_SET>

DEPENDENCIES
  <This function is only valid for config type TBL_MCC_SET>

RETURN VALUE
  TRUE: MCC found.
  FALSE: Not found.

SIDE EFFECTS
  none

===========================================================================*/
static boolean reg_mode_enhanced_hplmn_search_find_mcc_in_set(reg_mode_srch_tbl_mcc_set_s_type* mcc_set, sys_mcc_type mcc)
{
  byte temp;

  for(temp=0; temp < mcc_set->no_of_mcc; temp++)
  {
    if(sys_mcc_same_country(mcc_set->mcc[temp], mcc))
    {
      return TRUE;
    }
  }

  return FALSE;
}

/*===========================================================================

FUNCTION reg_mode_enhanced_hplmn_search_mcc_check

DESCRIPTION
  This function checks if given MCC should be searched during higher priority
  plmn search, when currently camped on given plmn.
  
  plmn: Currently camped PLMN
  pplmn_mcc: MCC of the preferred PLMN, to be checked whether it should be 
       included in search or not.

DEPENDENCIES
  none

RETURN VALUE
   1: Include this MCC PLMNs for hplmn search.
   0: Do no include this MCC PLMNs for hplmn search.
  -1: Decide to include or not based on spec.

SIDE EFFECTS
  none

===========================================================================*/
static boolean reg_mode_enhanced_hplmn_search_mcc_check(sys_plmn_id_s_type plmn, sys_mcc_type pplmn_mcc)
{
  byte temp;
  boolean plmn_id_is_undefined;
  boolean mnc_includes_pcs_digit;
  uint32  plmn_mcc;
  uint32  plmn_mnc;
  
  sys_plmn_get_mcc_mnc
  (
    plmn,
    &plmn_id_is_undefined,
    &mnc_includes_pcs_digit,
    &plmn_mcc,
    &plmn_mnc
  );

  if(plmn_id_is_undefined)
    return(FALSE);
  
  /* Logic based on config parameter */
  if(reg_mode_enhanced_hplmn_search_tbl.hdr.config == TBL_MCC_SET)
  {
    for(temp=0; temp <reg_mode_enhanced_hplmn_search_tbl.hdr.no_of_rec;  temp++)
    {
      if(reg_mode_enhanced_hplmn_search_find_mcc_in_set(
                &(reg_mode_enhanced_hplmn_search_tbl.tbl.mcc_set[temp]), plmn_mcc) == TRUE &&
          reg_mode_enhanced_hplmn_search_find_mcc_in_set(
                &(reg_mode_enhanced_hplmn_search_tbl.tbl.mcc_set[temp]), pplmn_mcc) == TRUE)
      {
        return(TRUE);
      }
    }  /* loop */
  }
  else if (reg_mode_enhanced_hplmn_search_tbl.hdr.config == TBL_ALL_MCC)
  {
    return (TRUE);
  }
    /* Not found, so return -1 to search as per spec */
  return(FALSE);
}
/*==============================================================================

                              FUNCTION DEFINITIONS

==============================================================================*/

/*==============================================================================

FUNCTION NAME

  reg_mode_output_detailed_plmn_list

==============================================================================*/

void reg_mode_output_detailed_plmn_list
(
  const sys_detailed_plmn_list_s_type* plmn_list_p
#ifdef FEATURE_FEMTO_CSG
  , boolean csg_list
#endif 
)
{
  boolean plmn_id_is_undefined;
  boolean mnc_includes_pcs_digit;
  uint32  mcc;
  uint32  mnc;
  uint32  i;
  char* rat_name;
  char* service_capability_name;
  char* list_category_name;
  char* signal_quality_name;

  if (plmn_list_p->length == 0)
  {
    return;
  }

  MSG_HIGH_DS_0(REG_SUB, "=REG=    PLMN ID           SVC   LIST SIGNAL");
#ifdef FEATURE_FEMTO_CSG
  if(csg_list == FALSE)
  {
#endif 
    MSG_HIGH_DS_0(REG_SUB, "=REG=  # MCC-MNC F  RAT  DOMAIN   CAT Q RSSI");
#ifdef FEATURE_FEMTO_CSG
  }
  else
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG=  # MCC-MNC  CSG_ID  RAT  DOMAIN   CAT Q RSSI");
  }
#endif 

  for (i = 0; i < MIN(plmn_list_p->length, SYS_PLMN_LIST_MAX_LENGTH) ; i++)
  {
    sys_plmn_get_mcc_mnc
    (
      plmn_list_p->info[i].plmn,
      &plmn_id_is_undefined,
      &mnc_includes_pcs_digit,
      &mcc,
      &mnc
    );

    switch (plmn_list_p->info[i].rat)
    {
      case SYS_RAT_GSM_RADIO_ACCESS:
        rat_name = " GSM";
        break;
      case SYS_RAT_UMTS_RADIO_ACCESS:
        rat_name = "UMTS";
        break;
      case SYS_RAT_TDS_RADIO_ACCESS:
        rat_name = " TDS";
        break;
#ifdef FEATURE_LTE
      case SYS_RAT_LTE_RADIO_ACCESS:
        rat_name = " LTE";
        break;
#endif
      default:
        rat_name = "    ";
        break;
    }

    switch (plmn_list_p->info[i].plmn_service_capability)
    {
      case SYS_SRV_CAPABILITY_NOT_AVAILABLE:
        service_capability_name = "   NONE";
        break;
      case SYS_SRV_CAPABILITY_CS_SERVICE_ONLY:
        service_capability_name = "CS_ONLY";
        break;
      case SYS_SRV_CAPABILITY_PS_SERVICE_ONLY:
        service_capability_name = "PS_ONLY";
        break;
      case SYS_SRV_CAPABILITY_CS_PS_SERVICE:
        service_capability_name = "  CS_PS";
        break;
      default:
        service_capability_name = "       ";
        break;
    }

    switch (plmn_list_p->info[i].list_category)
    {
      case SYS_DETAILED_PLMN_LIST_CATEGORY_HPLMN:
        list_category_name = "HPLMN";
        break;
      case SYS_DETAILED_PLMN_LIST_CATEGORY_PREFERRED:
        list_category_name = "PPLMN";
        break;
      case SYS_DETAILED_PLMN_LIST_CATEGORY_USER_PREFERRED:
        list_category_name = "UPLMN";
        break;
      case SYS_DETAILED_PLMN_LIST_CATEGORY_OPERATOR_PREFERRED:
        list_category_name = "OPLMN";
        break;
      case SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER:
        list_category_name = "OTHER";
        break;
#ifdef FEATURE_FEMTO_CSG
      case SYS_DETAILED_PLMN_LIST_OPERATOR_CSG:
        list_category_name = "OPERATOR";
        break;
      case SYS_DETAILED_PLMN_LIST_ALLOWED_CSG:
        list_category_name = "ALLOWED";
        break;
      case SYS_DETAILED_PLMN_LIST_OTHER_CSG:
        list_category_name = "OTHER";
        break;
#endif 
      default:
        list_category_name = "     ";
        break;
    }

    switch (plmn_list_p->info[i].signal_quality)
    {
      case SYS_SIGNAL_QUALITY_HIGH:
        signal_quality_name = "H";
        break;
      case SYS_SIGNAL_QUALITY_LOW:
        signal_quality_name = "L";
        break;
      default:
        signal_quality_name = " ";
        break;
    }

#ifdef FEATURE_FEMTO_CSG
  if(csg_list == FALSE)
  {
#endif 
#ifdef PERLUTF
      #error code not present
#else
      (void)snprintf
  #endif
      (
        reg_mode_output_buffer,
        REG_MODE_OUTPUT_BUFFER_LENGTH+1,
        ((mnc_includes_pcs_digit) ? 
          "%2lu %03lu-%03lu %s %s %s %s %s %4lu" :
          "%2lu %03lu- %02lu %s %s %s %s %s %4lu"),
        i,
        mcc,
        mnc,
        (plmn_list_p->info[i].plmn_forbidden ? "F" : " "),
        rat_name,
        service_capability_name,
        list_category_name,
        signal_quality_name,
        plmn_list_p->info[i].signal_strength
      );
#ifdef FEATURE_FEMTO_CSG
    }
    else
    {
#ifdef PERLUTF
    #error code not present
#else
    (void)snprintf
#endif
      (
        reg_mode_output_buffer,
        REG_MODE_OUTPUT_BUFFER_LENGTH+1,
        ((mnc_includes_pcs_digit) ? 
          "%2lu %03lu-%03lu %lx %s %s %s %s %4lu" :
          "%2lu %03lu- %02lu %lx %s %s %s %s %4lu"),
        i,
        mcc,
        mnc,
        plmn_list_p->info[i].csg_info.csg_id,
        rat_name,
        service_capability_name,
        list_category_name,
        signal_quality_name,
        plmn_list_p->info[i].signal_strength
      );
    }
#endif 
    MSG_SPRINTF_1(MSG_SSID_DFLT,MSG_LEGACY_HIGH,"%s",reg_mode_output_buffer);
    memset(reg_mode_output_buffer, 0, sizeof(reg_mode_output_buffer));
  }
}


/*==============================================================================

FUNCTION NAME

  reg_mode_network_selection_mode_get

==============================================================================*/

sys_network_selection_mode_e_type reg_mode_network_selection_mode_get
(
  void
)
{
  return reg_mode_network_selection_mode;
}

#ifdef FEATURE_DUAL_SIM
/*==============================================================================

FUNCTION NAME

  reg_mode_network_selection_mode_get_per_subs

==============================================================================*/

sys_network_selection_mode_e_type reg_mode_network_selection_mode_get_per_subs
(
  sys_modem_as_id_e_type as_id
)
{
  return reg_mode_network_selection_mode_sim[as_id];
}
#endif
/*==============================================================================

FUNCTION NAME

  reg_mode_network_selection_mode_set

==============================================================================*/

void reg_mode_network_selection_mode_set
(
  sys_network_selection_mode_e_type network_selection_mode
)
{
  reg_mode_network_selection_mode = network_selection_mode;
}

#ifdef FEATURE_SGLTE
/*==============================================================================

FUNCTION NAME

  reg_mode_copy_network_selection_mode

==============================================================================*/

void reg_mode_copy_network_selection_mode
(
  reg_as_id_e_type src_as_id, 
  reg_as_id_e_type tgt_as_id
)
{
  reg_mode_network_selection_mode_sim[tgt_as_id] = reg_mode_network_selection_mode_sim[src_as_id];
}
#endif

/*==============================================================================           

FUNCTION NAME

  reg_mode_scan_scope_set

==============================================================================*/

sys_scan_scope_e_type reg_mode_scan_scope_get
(
  void
)
{
  return reg_mode_scan_scope;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_scan_scope_set

==============================================================================*/

void reg_mode_scan_scope_set
(
  sys_scan_scope_e_type scan_scope
)
{
  reg_mode_scan_scope = scan_scope;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_mode_pref_get

==============================================================================*/

sys_mode_pref_e_type reg_mode_mode_pref_get
(
  void
)
{
  return reg_mode_mode_pref;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_req_mode_pref_get

==============================================================================*/

sys_mode_pref_e_type reg_mode_req_mode_pref_get
(
  void
)
{
  return reg_mode_req_mode_pref;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_req_mode_pref_set

==============================================================================*/

void reg_mode_req_mode_pref_set
(
  sys_mode_pref_e_type mode_pref
)
{
  reg_mode_req_mode_pref = mode_pref;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_cs_for_request_get

==============================================================================*/

boolean reg_mode_cs_for_request_get
(
  void
)
{
  return reg_mode_cs_for_request;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_ps_for_request_get

==============================================================================*/

boolean reg_mode_ps_for_request_get
(
  void
)
{
  return reg_mode_ps_for_request;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_call_for_bit_set

==============================================================================*/

void reg_mode_call_for_bit_set
(
  boolean cs_for_request,
  boolean ps_for_request
)
{
   reg_mode_cs_for_request = cs_for_request;
   reg_mode_ps_for_request = ps_for_request;
}

#if !defined (FEATURE_LTE)
/*==============================================================================

FUNCTION NAME

  reg_mode_set_emergency_call

==============================================================================*/

void reg_mode_set_emergency_call
(
  uint32 additional_info
)
{
  if(reg_bit_mask_check(additional_info,
              (byte)SYS_ADDTNL_INFO_EMERG_CALL) == TRUE)
  {
     reg_mode_is_emergency_call = TRUE;
  }
  else
  {
     reg_mode_is_emergency_call = FALSE;
  }
 }


boolean reg_mode_get_emergency_call
(
  void
)
{
  return reg_mode_is_emergency_call;
}
#endif

#ifdef FEATURE_GSM_BAND_PREF
/*==============================================================================

FUNCTION NAME

  reg_mode_band_pref_get

==============================================================================*/

sys_band_mask_type reg_mode_band_pref_get
(
  void
)
{
  return reg_mode_band_pref;
}


#endif

#ifdef FEATURE_NAS_ENABLE_LTE_BAND_FILTERING
/*==============================================================================

FUNCTION NAME

  reg_mode_lte_scan_pref_get

==============================================================================*/

reg_mode_lte_scan_e_type reg_mode_lte_scan_pref_get
(
  void
)
{
  return  reg_mode_lte_scan_pref;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_lte_scan_pref_set

==============================================================================*/

void reg_mode_lte_scan_pref_set
(
  reg_mode_lte_scan_e_type lte_scan_pref
)
{
  MSG_HIGH_DS_1(REG_SUB, "=REG= Updating LTE scan pref to %d", lte_scan_pref);
  reg_mode_lte_scan_pref = lte_scan_pref;
}
#endif



/*==============================================================================

FUNCTION NAME

  reg_mode_rat_pri_list_get

==============================================================================*/

sys_rat_pri_list_info_s_type * reg_mode_rat_pri_list_get
(
   void
)
{
   return &reg_mode_rat_pri_list;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_fallback_control_get

==============================================================================*/

boolean reg_mode_fallback_control_get 
(
    void
)
{
   return reg_mode_selection_to_gw_first_pass;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_fallback_control

==============================================================================*/

void reg_mode_fallback_control_set 
(
    boolean value
)
{
   reg_mode_selection_to_gw_first_pass = value;      
}
/*==============================================================================

FUNCTION NAME

  reg_mode_bst_band_match

==============================================================================*/
boolean reg_mode_bst_band_match
(
  const sys_rat_pri_list_info_s_type *rat_pri_list_1, 
  const sys_rat_pri_list_info_s_type *rat_pri_list_2,
  sys_sys_mode_e_type acq_sys_mode
)
{
  uint32 i;
  sys_band_mask_e_type  bst_band_cap_list1 = 0;

  sys_band_mask_e_type  bst_band_cap_list2 = 0;

  boolean bst_band_match = TRUE;

  sys_lte_band_mask_e_type  bst_band_cap_lte_list1;

  sys_lte_band_mask_e_type  bst_band_cap_lte_list2;

  SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&bst_band_cap_lte_list1);

  SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&bst_band_cap_lte_list2);


  for (i = 0; i < rat_pri_list_1->num_items; i++)
  {
    if(rat_pri_list_1->priority_list_info[i].acq_sys_mode == acq_sys_mode)
    {
      if(acq_sys_mode == SYS_SYS_MODE_LTE)
      {
        bst_band_cap_lte_list1 = rat_pri_list_1->priority_list_info[i].bst_band_cap.lte_band_cap;
      }
      else
      {
        bst_band_cap_list1 = rat_pri_list_1->priority_list_info[i].bst_band_cap.chgwt_band_cap;
      }
      break;
    }
  }

  for (i = 0; i < rat_pri_list_2->num_items; i++)
  {
    if(rat_pri_list_2->priority_list_info[i].acq_sys_mode == acq_sys_mode)
    {
      if(acq_sys_mode == SYS_SYS_MODE_LTE)
      {
        bst_band_cap_lte_list2 = rat_pri_list_2->priority_list_info[i].bst_band_cap.lte_band_cap;
      }
      else
      {
        bst_band_cap_list2 = rat_pri_list_2->priority_list_info[i].bst_band_cap.chgwt_band_cap;
      }
      break;
    }
  }

  if(acq_sys_mode == SYS_SYS_MODE_LTE)
  {
    if(SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&bst_band_cap_lte_list1,&bst_band_cap_lte_list2)== FALSE)
    {
      bst_band_match = FALSE;
    }
  }
  else
  {  
    if(bst_band_cap_list1 != bst_band_cap_list2)
  {
    bst_band_match = FALSE;
  }
  }
  MSG_HIGH_DS_1(REG_SUB, "=REG= reg_mode_bst_band_match function returning %d",bst_band_match);
  return bst_band_match;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_rat_pri_list_match

==============================================================================*/
boolean reg_mode_rat_pri_list_match
(
  const sys_rat_pri_list_info_s_type *rat_pri_list_1, 
  const sys_rat_pri_list_info_s_type *rat_pri_list_2
)
{
  uint32 i,j;
  boolean is_rat_found;
  
  if(rat_pri_list_1->num_items != rat_pri_list_2->num_items)
     return FALSE;

  for (i = 0; i < rat_pri_list_1->num_items; i++)
  {
     is_rat_found = FALSE;
     for (j = 0; j < rat_pri_list_2->num_items; j++)
     {
        if(rat_pri_list_1->priority_list_info[i].acq_sys_mode == 
              rat_pri_list_2->priority_list_info[j].acq_sys_mode)
        {
          if(rat_pri_list_1->priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_LTE)
          {
             if( SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&rat_pri_list_1->priority_list_info[i].band_cap.lte_band_cap,
                                       &rat_pri_list_2->priority_list_info[j].band_cap.lte_band_cap)== TRUE)
             {
               is_rat_found = TRUE;
               break;
             }
          }
          else if((rat_pri_list_1->priority_list_info[i].band_cap.chgwt_band_cap ==
              rat_pri_list_2->priority_list_info[j].band_cap.chgwt_band_cap))  
        {
           is_rat_found = TRUE;
           break;
        }        
     }
     }
     if(!is_rat_found)
     {
        return FALSE;
     }
  }
  return TRUE;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_is_rat_pri_list_empty

==============================================================================*/
boolean reg_mode_is_rat_pri_list_empty
(
  sys_rat_pri_list_info_s_type *rat_pri_list
)
{
  if(rat_pri_list->num_items == 0)
  {
    return TRUE;
  }
  return FALSE;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_rat_pri_list_all_params_match_except_bst_for_LTE

==============================================================================*/
boolean reg_mode_rat_pri_list_all_params_match_except_bst_for_LTE
(
  const sys_rat_pri_list_info_s_type *rat_pri_list_1, 
  const sys_rat_pri_list_info_s_type *rat_pri_list_2
)
{
    uint32 i;

    if((rat_pri_list_1->num_items != rat_pri_list_2->num_items) ||
       (rat_pri_list_1->next_acq_sys_index != rat_pri_list_2->next_acq_sys_index) ||
       (rat_pri_list_1->scan_type.new_scan != rat_pri_list_2->scan_type.new_scan) ||
       (rat_pri_list_1->scan_type.use_timer != rat_pri_list_2->scan_type.use_timer))
    {
      return FALSE;
    }
    for (i = 0; i < rat_pri_list_1->num_items; i++)
    {
      if((rat_pri_list_1->priority_list_info[i].acq_sys_mode != 
         rat_pri_list_2->priority_list_info[i].acq_sys_mode)||
         (rat_pri_list_1->priority_list_info[i].acq_sys_time_interval != 
          rat_pri_list_2->priority_list_info[i].acq_sys_time_interval))
      {
        return FALSE;
      }
      else if((rat_pri_list_1->priority_list_info[i].acq_sys_mode != SYS_SYS_MODE_LTE )&&
              ((rat_pri_list_1->priority_list_info[i].band_cap.chgwt_band_cap !=
                rat_pri_list_2->priority_list_info[i].band_cap.chgwt_band_cap)||
               (rat_pri_list_1->priority_list_info[i].bst_rat_acq_required !=
                rat_pri_list_2->priority_list_info[i].bst_rat_acq_required)||
               (rat_pri_list_1->priority_list_info[i].bst_band_cap.chgwt_band_cap !=
                rat_pri_list_2->priority_list_info[i].bst_band_cap.chgwt_band_cap)))
  {
    return FALSE;
  }
      else if((rat_pri_list_1->priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_LTE) &&
               (SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&rat_pri_list_1->priority_list_info[i].band_cap.lte_band_cap,
                                       &rat_pri_list_2->priority_list_info[i].band_cap.lte_band_cap)== FALSE))
    {
      return FALSE;
    }
  }
  return TRUE;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_rat_pri_list_3gpp_sys_mode_match

  This function compares the rat priority lists passed to see both rat priority list has same 3GPP rat acq mode.
  Eg. GW to GWL, GW to G, GL to GW etc. will return FALSE
  Rat order change(GWL to LGW) or band capability change will not be considered, function will still return true if there is
  no change in of 3GPP sys mode entries.

==============================================================================*/
boolean reg_mode_rat_pri_list_3gpp_sys_mode_match
(
  const sys_rat_pri_list_info_s_type *rat_pri_list_1, 
  const sys_rat_pri_list_info_s_type *rat_pri_list_2
)
{
  uint32 i,j;
  reg_mode_3gpp_capability_s_type rat_3gpp_cap_1;
  reg_mode_3gpp_capability_s_type rat_3gpp_cap_2;
  boolean is_rat_found;

  reg_mode_get_3gpp_rat_capabilty(
                      &rat_3gpp_cap_1,
                      rat_pri_list_1
#ifdef FEATURE_SGLTE
                      ,FALSE
#endif
                      );

  reg_mode_get_3gpp_rat_capabilty(
                      &rat_3gpp_cap_2,
                      rat_pri_list_2
#ifdef FEATURE_SGLTE
                      ,FALSE
#endif
                      );
  
  if(rat_3gpp_cap_1.num_items != rat_3gpp_cap_2.num_items)
  {
    return FALSE;
  }

  for (i = 0; i < rat_3gpp_cap_1.num_items; i++)
  {
    is_rat_found = FALSE;
    for (j = 0; j < rat_3gpp_cap_2.num_items; j++)
    {
      if(rat_3gpp_cap_1.rat_list[i] == rat_3gpp_cap_2.rat_list[j])
      {
        is_rat_found = TRUE;
        break;
      }
    }
    if(!is_rat_found)
    {
      return FALSE;
    }
  }
  return TRUE;
}
/*==============================================================================

FUNCTION NAME

  reg_mode_rat_pri_list_order_match

==============================================================================*/
boolean reg_mode_rat_pri_list_order_match
(
  const sys_rat_pri_list_info_s_type *rat_pri_list_1, 
  const sys_rat_pri_list_info_s_type *rat_pri_list_2
)
{
  uint32 i;

  if(rat_pri_list_1->num_items != rat_pri_list_2->num_items)
     return FALSE;

  for (i = 0; i < rat_pri_list_1->num_items; i++)
  {
    if(rat_pri_list_1->priority_list_info[i].acq_sys_mode != 
          rat_pri_list_2->priority_list_info[i].acq_sys_mode)
    {
      return FALSE;
    }
    else
    {
      if(rat_pri_list_1->priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_LTE )
      {
        if( SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&rat_pri_list_1->priority_list_info[i].band_cap.lte_band_cap,
                                       &rat_pri_list_2->priority_list_info[i].band_cap.lte_band_cap)== FALSE)
        {
          return FALSE;
        }
      }
      else
      {
        if(rat_pri_list_1->priority_list_info[i].band_cap.chgwt_band_cap != 
        rat_pri_list_2->priority_list_info[i].band_cap.chgwt_band_cap)
        {
          return FALSE;
        }
      }
     }
  }
  return TRUE;
}



/*==============================================================================

FUNCTION NAME

  reg_mode_rat_pri_list_set

==============================================================================*/

void reg_mode_rat_pri_list_set
(
  const sys_rat_pri_list_info_s_type *rat_pri_list_info
)
{
   memscpy(&reg_mode_rat_pri_list, sizeof(sys_rat_pri_list_info_s_type), rat_pri_list_info, sizeof(sys_rat_pri_list_info_s_type));
}

/*==============================================================================

FUNCTION NAME

  reg_mode_rat_pri_list_reset

==============================================================================*/

void reg_mode_rat_pri_list_reset
(
  void
)
{
   reg_mode_rat_pri_list.num_items = 0;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_bst_status_set

==============================================================================*/

void reg_mode_bst_status_set
(
  boolean bst_status
)
{
   uint32 i;
   reg_mode_bst_status = bst_status;
  
   if(bst_status == FALSE)
   {
     for(i = 0; i < reg_mode_rat_pri_list.num_items; i++)
     {
       reg_mode_rat_pri_list.priority_list_info[i].bst_rat_acq_required = TRUE;
       reg_mode_rat_pri_list.priority_list_info[i].bst_band_cap = reg_mode_rat_pri_list.priority_list_info[i].band_cap;
     }
#ifdef FEATURE_LTE
     for(i = 0; i < reg_mode_curr_rat_pri_list.num_items; i++)
     {
       reg_mode_curr_rat_pri_list.priority_list_info[i].bst_rat_acq_required = TRUE;
       reg_mode_curr_rat_pri_list.priority_list_info[i].bst_band_cap = reg_mode_curr_rat_pri_list.priority_list_info[i].band_cap;
     }
#endif
   }

   MSG_HIGH_DS_1(REG_SUB, "=REG= Set BST STATUS to %d",reg_mode_bst_status);
}

/*==============================================================================

FUNCTION NAME

  reg_mode_bst_status_get

==============================================================================*/

boolean reg_mode_bst_status_get
(
  void
)
{
   return reg_mode_bst_status;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_is_bst_set_for_rat

==============================================================================*/
boolean reg_mode_is_bst_set_for_rat
(
  const sys_rat_pri_list_info_s_type *rat_pri_list_info,
  const sys_radio_access_tech_e_type rat
)
{
  uint32 i;
  boolean status = FALSE;


  for (i = 0; i < rat_pri_list_info->num_items; i++)
  {
    if(rat == reg_mode_convert_acq_mode_to_access_tech
               (rat_pri_list_info->priority_list_info[i].acq_sys_mode))
    {
      if(rat_pri_list_info->priority_list_info[i].bst_rat_acq_required == TRUE)
      {
        status = TRUE;
      }
      break;
    }
  }
  return status;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_get_3gpp_rat_capabilty

  sglte_rat_filter: TRUE - Yes evaluate if RAT to be filtered in SGLTE mode.
  FALSE - Do not evaluate of RAT Filter in SGLTE mode. 
  Means keep all applicable RATs.
==============================================================================*/

void reg_mode_get_3gpp_rat_capabilty
(
  reg_mode_3gpp_capability_s_type *rat_3gpp_capabilty,
  const sys_rat_pri_list_info_s_type *rat_pri_list_info
#ifdef FEATURE_SGLTE
,
  boolean sglte_rat_filter
#endif
)
{
  uint32 i;

  rat_3gpp_capabilty->num_items = 0;
#ifdef FEATURE_SGLTE
  if(!REG_SUB_IS_SGLTE)
  {
    sglte_rat_filter = FALSE;
  }
#endif

  for (i = 0; (i < rat_pri_list_info->num_items) &&
              (rat_3gpp_capabilty->num_items < REG_MODE_MAX_3GPP_RAT); i++)
  {
     if((reg_mode_bst_status_get() == FALSE) ||
         (rat_pri_list_info->priority_list_info[i].bst_rat_acq_required == TRUE))
     {
       if((rat_pri_list_info->priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_GSM)
#ifdef FEATURE_SGLTE
              && ((!sglte_rat_filter) || ((!reg_sglte_mode) ||(reg_as_id==reg_cs_stack_id)))
#endif
         )         
       {
          rat_3gpp_capabilty->rat_list[rat_3gpp_capabilty->num_items] = SYS_RAT_GSM_RADIO_ACCESS;       
          rat_3gpp_capabilty->num_items++;  
       }
       else if ((rat_pri_list_info->priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_WCDMA)
#ifdef FEATURE_SGLTE
                         && ((!sglte_rat_filter) || (((reg_sglte_ue_mode!=SYS_UE_MODE_SGLTE_HOME) && (reg_sglte_ue_mode != SYS_UE_MODE_SGLTE_TEST_ONLY))
                         &&(reg_as_id==reg_cs_stack_id)))
#endif
                         )
       {
          rat_3gpp_capabilty->rat_list[rat_3gpp_capabilty->num_items] = SYS_RAT_UMTS_RADIO_ACCESS;       
          rat_3gpp_capabilty->num_items++;  
       }
       else if (rat_pri_list_info->priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_TDS)
       {
          rat_3gpp_capabilty->rat_list[rat_3gpp_capabilty->num_items] = SYS_RAT_TDS_RADIO_ACCESS;       
          rat_3gpp_capabilty->num_items++;  
       }
       
#ifdef FEATURE_LTE
       else if(rat_pri_list_info->priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_LTE)
       {
          rat_3gpp_capabilty->rat_list[rat_3gpp_capabilty->num_items] = SYS_RAT_LTE_RADIO_ACCESS;       
          rat_3gpp_capabilty->num_items++;  
       }
#endif /* FEATURE_LTE */
     }
  }
}



#ifdef FEATURE_LTE
/*==============================================================================

FUNCTION NAME

  reg_mode_remove_rat_from_rat_pri_list

==============================================================================*/
void reg_mode_remove_rat_from_rat_pri_list
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info,
  const sys_sys_mode_e_type rat
)
{
  uint32 i,j;
  
  rat_pri_list_info->num_items = MIN(rat_pri_list_info->num_items,SYS_MAX_ACQ_PREF_RAT_LIST_NUM);
  for (i= 0; i < rat_pri_list_info->num_items; i++)
  {
    if(rat_pri_list_info->priority_list_info[i].acq_sys_mode == rat)
    {
      for(j = i+1; j < rat_pri_list_info->num_items; j++)
      {
        rat_pri_list_info->priority_list_info[j-1] = rat_pri_list_info->priority_list_info[j];
      }
      rat_pri_list_info->num_items--;
      break;
    }
  }
}

/*==============================================================================

FUNCTION NAME

  reg_mode_curr_rat_pri_list_set

==============================================================================*/

void reg_mode_curr_rat_pri_list_set(sys_rat_pri_list_info_s_type *rat_pri_list_info)
{
  if((reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_LTE_RADIO_ACCESS,
            rat_pri_list_info,FALSE) == TRUE) &&
     (reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_LTE_RADIO_ACCESS,
        &reg_mode_curr_rat_pri_list,FALSE) == FALSE))
  {
    reg_timers_stop_hplmn_irat_search_period_timer();
  }
  memscpy(&reg_mode_curr_rat_pri_list,sizeof(sys_rat_pri_list_info_s_type),rat_pri_list_info,sizeof(sys_rat_pri_list_info_s_type));
}

/*==============================================================================

FUNCTION NAME

  reg_mode_curr_rat_pri_list_get

==============================================================================*/

sys_rat_pri_list_info_s_type* reg_mode_curr_rat_pri_list_get(void)
{
  return &reg_mode_curr_rat_pri_list;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_is_rat_removed_from_curr_list

==============================================================================*/
boolean reg_mode_is_rat_removed_from_curr_list(sys_radio_access_tech_e_type rat)
{
  if((reg_mode_is_rat_present_in_rat_pri_list(rat,&reg_mode_rat_pri_list,FALSE)) &&
     !(reg_mode_is_rat_present_in_rat_pri_list(rat,&reg_mode_curr_rat_pri_list,FALSE)))
  {
    return TRUE;
  }
  return FALSE;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_check_eplmn_backoff_forbidden

==============================================================================*/
boolean reg_mode_check_eplmn_backoff_forbidden
(
  reg_sim_plmn_timer_info_s_type *plmn_timer_info_ptr,
  sys_modem_as_id_e_type as_id
)
{
  sys_plmn_id_s_type eplmn_rplmn;
  sys_plmn_list_s_type eplmn_list;
#ifdef FEATURE_DUAL_SIM
  sys_eplmn_per_nas_stacks_list_get(&eplmn_list, &eplmn_rplmn,as_id);
#else
  sys_eplmn_list_get(&eplmn_list, &eplmn_rplmn);
#endif
  if(reg_mode_equivalent_plmn(eplmn_rplmn, plmn_timer_info_ptr->plmn))
  {
    if(reg_sim_plmn_backoff_forbidden(eplmn_rplmn,as_id) ||
       reg_sim_eplmn_list_backoff_forbidden(&eplmn_list,plmn_timer_info_ptr,as_id))
    {
      return TRUE;
    }
  }
  return FALSE;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_adjust_rat_order_plmn_backoff_forbidden

==============================================================================*/
void reg_mode_adjust_rat_order_plmn_backoff_forbidden
(
  const sys_plmn_id_s_type *plmn, 
  reg_mode_3gpp_capability_s_type *plmn_rat_search_order
)
{
  uint8 i,j;
  plmn_rat_search_order->num_items = MIN(plmn_rat_search_order->num_items,
                                                       REG_MODE_MAX_3GPP_RAT);
  if(reg_sim_plmn_rat_forbidden(*plmn,SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)reg_sub_id))
  {
    for(i = 0;i < plmn_rat_search_order->num_items; i++)
    {
      if(plmn_rat_search_order->rat_list[i] == SYS_RAT_LTE_RADIO_ACCESS)
      {
        for(j = i+1; j < plmn_rat_search_order->num_items; j++)
        {
          plmn_rat_search_order->rat_list[j-1] = plmn_rat_search_order->rat_list[j];
        }
        plmn_rat_search_order->rat_list[j-1] = SYS_RAT_LTE_RADIO_ACCESS;
        break;
      }
    }
  }
}

/*==============================================================================

FUNCTION NAME

  reg_mode_disable_lte
==============================================================================*/

boolean reg_mode_disable_lte(void)
{
  boolean update_mm_for_lte_cap_change = FALSE;
  if(mm_is_call_or_registration_ongoing() == FALSE)
  {
    update_mm_for_lte_cap_change = TRUE;
    reg_mode_remove_rat_from_rat_pri_list(&reg_mode_curr_rat_pri_list, SYS_SYS_MODE_LTE);
  }
  else if(reg_is_timer_active(REG_UPDATE_LTE_CAP_TIMER) == FALSE)
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= mm_is_call_or_registration_ongoing function API returned TRUE");
    reg_timers_start_update_lte_cap_timer();
  }
  return update_mm_for_lte_cap_change;
}



/*==============================================================================

FUNCTION NAME

  reg_mode_reenable_lte
==============================================================================*/

boolean reg_mode_reenable_lte(void)
{
  boolean update_mm_for_lte_cap_change = FALSE;
  sys_srv_status_e_type service_status = reg_state_service_status_get();
  reg_sim_plmn_timer_info_s_type plmn_timer_info;
  plmn_timer_info.plmn = reg_state_service_plmn_get();
  plmn_timer_info.timer_count = 0;
  plmn_timer_info.cause = SYS_BLOCK_PLMN_CAUSE_NONE;

  /*
  **  If LTE is not currently reported to MM for per plmn blocking
  **  and if current camped PLMN or its equivalent PLMN is not
  **  backoff forbidden anymore then
  **     a) if mm connection status is reporting IDLE then enable LTE
  **     b) otherwise start Reenable LTE timer if it is not running.
  */
  if((reg_mode_is_rat_removed_from_curr_list(SYS_RAT_LTE_RADIO_ACCESS)) &&
     !((reg_sim_plmn_rat_forbidden(plmn_timer_info.plmn,SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)reg_sub_id)) ||
        reg_mode_check_eplmn_backoff_forbidden(&plmn_timer_info,(sys_modem_as_id_e_type)reg_sub_id)))
  {
    if(mm_check_connection_active((sys_modem_as_id_e_type)reg_sub_id,
                                  (sys_modem_as_id_e_type)reg_as_id) == TRUE)
    {
      if(reg_is_timer_active(REG_UPDATE_LTE_CAP_TIMER) == FALSE)
      {
        MSG_HIGH_DS_0(REG_SUB, "=REG= mm_check_connection_active function API returned TRUE");
        reg_timers_start_update_lte_cap_timer();
      }
    }
    else if((service_status == SYS_SRV_STATUS_SRV) ||
            (service_status == SYS_SRV_STATUS_LIMITED_REGIONAL))
    {
      reg_timers_stop_update_lte_cap_timer();
      reg_send_eq_plmn_change_ind();
      update_mm_for_lte_cap_change = TRUE;
      reg_mode_curr_rat_pri_list_set(&reg_mode_rat_pri_list);
    }
  }
  return update_mm_for_lte_cap_change;
}



/*==============================================================================

FUNCTION NAME

  reg_mode_update_curr_rat_pri_list
  This function updates  current rat priority list based on backoff forbidden checks on current camped PLMN and its EPLMN.

  1. UE camp on LTE PLMN B.PLMN B gives equivalent PLMN list which has blocked plmn then NAS should declare limited
     service on LTE
  2. UE camp on GW PLMN B. PLMN B gives equivalent PLMN list which has blocked plmn then NAS  should remove LTE
      from the curr rat priority list.
  3. UE camp on GW PLMN B with lte removed from rat list. Now if B gives an new equivalent PLMN list which does not
      have any forbidden PLMNs then NAS  should reenable LTE.
==============================================================================*/

boolean reg_mode_update_curr_rat_pri_list
(
  const sys_plmn_id_s_type *plmn,
  const sys_radio_access_tech_e_type active_rat
)
{

  sys_srv_status_e_type service_status;
  boolean update_mm_for_lte_cap_change = FALSE;
  boolean is_camped_plmn_backoff_forbidden = FALSE;
  boolean is_eplmn_backoff_forbidden = FALSE;
  reg_sim_plmn_timer_info_s_type plmn_timer_info;
  plmn_timer_info.plmn = *plmn;
  plmn_timer_info.timer_count = 0;
  plmn_timer_info.cause = SYS_BLOCK_PLMN_CAUSE_NONE;
  service_status = reg_state_service_status_get();


  /* If UE is not camped on any PLMN or LTE is not present in service request from CM then do not update the list */
  if(((service_status != SYS_SRV_STATUS_LIMITED) &&
      (service_status != SYS_SRV_STATUS_SRV) &&
      (service_status != SYS_SRV_STATUS_LIMITED_REGIONAL)) ||
     (reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_LTE_RADIO_ACCESS,
                                  &reg_mode_rat_pri_list,FALSE) == FALSE))
  {
    return update_mm_for_lte_cap_change;
  }

  is_camped_plmn_backoff_forbidden = reg_sim_plmn_backoff_forbidden(*plmn,(sys_modem_as_id_e_type)reg_sub_id);
  is_eplmn_backoff_forbidden = reg_mode_check_eplmn_backoff_forbidden(&plmn_timer_info,
                                                                         (sys_modem_as_id_e_type)reg_sub_id);

  if(
#ifdef FEATURE_DUAL_SIM
     (reg_sim_plmn_gprs_forbidden(*plmn,(sys_modem_as_id_e_type)reg_sub_id)) &&
#else
     (reg_sim_plmn_gprs_forbidden(*plmn)) &&
#endif
     !(reg_mode_is_rat_removed_from_curr_list(SYS_RAT_LTE_RADIO_ACCESS)) &&
      (active_rat != SYS_RAT_LTE_RADIO_ACCESS))
  {
    update_mm_for_lte_cap_change = reg_mode_disable_lte();
    MSG_HIGH_DS_1(REG_SUB, "=REG= PLMN GPRS forbidden reg_mode_disable_lte returned %d",
                                                        update_mm_for_lte_cap_change);
  }

  /*
  ** In cases where camped PLMN has EPLMN which is forbidden and mm 
  ** reports full service, then reg will add PLMN to backoff list and if LTE
  ** is not removed from the rat priority list then
  **   a) if active rat is LTE (suppose the case where the EPLMN list
  **       reported by LTE network after registration has PLMN which is backoff
  **       forbidden) then REG will send block plmn request to EMM
  **   b) if active rat is not LTE (suppose the case where the EPLMN list
  **       reported by GW network after registration has PLMN which is backoff
  **       forbidden) then REG will update MM with rat priority list with LTE removed.
  */
  else if((is_camped_plmn_backoff_forbidden == TRUE) ||
          (is_eplmn_backoff_forbidden == TRUE))
  {
    if((is_eplmn_backoff_forbidden == TRUE) &&
       (is_camped_plmn_backoff_forbidden == FALSE))
    {
      if(active_rat == SYS_RAT_LTE_RADIO_ACCESS)
      {
        reg_send_mmr_block_plmn_req(plmn_timer_info.plmn,plmn_timer_info.timer_count,
                                  TRUE,plmn_timer_info.cause, plmn_timer_info.eplmn_index);
      }
      else if(reg_sim_add_eplmn_to_backoff_fplmn_list(*plmn, plmn_timer_info.timer_count,
           plmn_timer_info.cause,plmn_timer_info.eplmn_index,(sys_modem_as_id_e_type)reg_sub_id))
      {
        reg_send_cm_fplmn_list_ind(SYS_FORBIDDEN_LIST_TYPE_LTE_BACKOFF_FPLMN);
      }
    }
    if((reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_LTE_RADIO_ACCESS, &reg_mode_curr_rat_pri_list, FALSE)) &&
       (active_rat != SYS_RAT_LTE_RADIO_ACCESS))
    {
      update_mm_for_lte_cap_change = reg_mode_disable_lte();
      MSG_HIGH_DS_3(REG_SUB, "=REG= reg_mode_disable_lte returned %d, is_eplmn_backoff_forbidden %d is_camped_plmn_backoff_forbidden %d",
                    update_mm_for_lte_cap_change,
                    is_eplmn_backoff_forbidden,
                    is_camped_plmn_backoff_forbidden);
    }
  }
  else if((reg_mode_is_rat_removed_from_curr_list(SYS_RAT_LTE_RADIO_ACCESS)) &&
          (is_camped_plmn_backoff_forbidden == FALSE))
  {
    update_mm_for_lte_cap_change = reg_mode_reenable_lte();
    MSG_HIGH_DS_1(REG_SUB, "=REG= reg_mode_reenable_lte returned %d", update_mm_for_lte_cap_change);
  }
  return update_mm_for_lte_cap_change;

}

#endif

/*==============================================================================

FUNCTION NAME

  
  reg_mode_req_service_domain_get

==============================================================================*/

sys_srv_domain_e_type reg_mode_req_service_domain_get
(
  void
)
{
  return reg_sim_allowed_service_domain(reg_mode_req_service_domain);
}


/*==============================================================================

FUNCTION NAME

  reg_mode_allowed_service_domain

==============================================================================*/

sys_srv_domain_e_type reg_mode_allowed_service_domain
(
  sys_plmn_id_s_type                   plmn,
  sys_srv_domain_e_type                req_service_domain
)
{
  switch (req_service_domain)
  {
    case SYS_SRV_DOMAIN_CS_ONLY:
      return SYS_SRV_DOMAIN_CS_ONLY;

    case SYS_SRV_DOMAIN_PS_ONLY:
#ifdef FEATURE_DUAL_SIM
      if (reg_sim_plmn_gprs_forbidden(plmn,(sys_modem_as_id_e_type)reg_sub_id))
#else
      if (reg_sim_plmn_gprs_forbidden(plmn))
#endif
      {
        return SYS_SRV_DOMAIN_NO_SRV;
      }
      else
      {
        return SYS_SRV_DOMAIN_PS_ONLY;
      }

#ifdef FEATURE_DUAL_DATA
    case SYS_SRV_DOMAIN_VOLTE:
#ifdef FEATURE_DUAL_SIM
      if (reg_sim_plmn_gprs_forbidden(plmn,(sys_modem_as_id_e_type)reg_sub_id))
#else
      if (reg_sim_plmn_gprs_forbidden(plmn))
#endif
      {
        return SYS_SRV_DOMAIN_NO_SRV;
      }
      else
      {
        return SYS_SRV_DOMAIN_VOLTE;
      }

    case SYS_SRV_DOMAIN_CS_VOLTE:
      return SYS_SRV_DOMAIN_CS_VOLTE;
#endif 

    case SYS_SRV_DOMAIN_CS_PS:
      return SYS_SRV_DOMAIN_CS_PS;

  
    case SYS_SRV_DOMAIN_NO_SRV:
    default:
      return SYS_SRV_DOMAIN_NO_SRV;
  }
}


/*==============================================================================

FUNCTION NAME

  reg_mode_req_plmn_get

==============================================================================*/

sys_plmn_id_s_type reg_mode_req_plmn_get
(
  void
)
{
  return reg_mode_req_plmn;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_req_plmn_set

==============================================================================*/

void reg_mode_req_plmn_set
(
  sys_plmn_id_s_type plmn
)
{
  reg_mode_req_plmn = plmn;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_req_rat_get

==============================================================================*/

sys_radio_access_tech_e_type reg_mode_req_rat_get
(
  void
)
{
  return reg_mode_req_rat;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_req_rat_set

==============================================================================*/

void reg_mode_req_rat_set
(
  sys_radio_access_tech_e_type rat
)
{
  reg_mode_req_rat = rat;
}

#ifdef FEATURE_ENHANCED_NW_SELECTION
/*==============================================================================

FUNCTION NAME

  reg_mode_restart_plmn_selection

==============================================================================*/

boolean reg_mode_restart_plmn_selection()
{
  boolean status = FALSE;
  
  if (reg_mode_network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC)
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= PLMN rescan is Started" );
    reg_mode_p = &reg_mode_automatic_plmn_selection;

    reg_mode_p->start_automatic_plmn_selection
    (
      SYS_NETWORK_SELECTION_MODE_AUTOMATIC,
      REG_MODE_AUTOMATIC_MODE_NO_PREFERENCE,
      reg_mode_req_mode_pref,
      reg_mode_cs_for_request,
      reg_mode_ps_for_request,
#ifdef FEATURE_GSM_BAND_PREF
      reg_mode_req_service_domain,
      reg_mode_band_pref
#else
      reg_mode_req_service_domain
#endif
      ,&reg_mode_rat_pri_list

#ifdef FEATURE_LTE
     ,reg_mode_sms_only,
      0
#endif
    );
    
    status = TRUE;
  }
  else
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= PLMN reselection is not done in MANUAL/LIMITED mode");
    //SYS_NETWORK_SELECTION_MODE_MANUAL:
    //SYS_NETWORK_SELECTION_MODE_LIMITED:
    //SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION:
    //default:
  }
  return status;
}
#endif

/*==============================================================================

FUNCTION NAME

  reg_mode_start_automatic_plmn_selection

==============================================================================*/

void reg_mode_start_automatic_plmn_selection
(
  sys_network_selection_mode_e_type    network_selection_mode,
  reg_mode_automatic_mode_e_type       automatic_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
#ifdef FEATURE_GSM_BAND_PREF
  sys_srv_domain_e_type                req_service_domain,
  sys_band_mask_type                   band_pref
#else
  sys_srv_domain_e_type                req_service_domain
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  reg_mode_p = &reg_mode_automatic_plmn_selection;

  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_mode_pref              = mode_pref;
  reg_mode_req_mode_pref          = mode_pref;
  reg_mode_req_service_domain     = req_service_domain;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
#ifdef FEATURE_GSM_BAND_PREF
  reg_mode_band_pref              = band_pref;
#endif
  memscpy(&reg_mode_rat_pri_list, sizeof(sys_rat_pri_list_info_s_type), rat_pri_list_info, sizeof(sys_rat_pri_list_info_s_type));

#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif

  reg_mode_p->start_automatic_plmn_selection
  (
    network_selection_mode,
    automatic_mode,
    mode_pref,
    cs_for_request,
    ps_for_request,
#ifdef FEATURE_GSM_BAND_PREF
    req_service_domain,
    band_pref
#else
    req_service_domain
#endif
    ,rat_pri_list_info

#ifdef FEATURE_LTE
  ,sms_only,
   additional_info
#endif 
  );
}


/*==============================================================================

FUNCTION NAME

  reg_mode_start_higher_priority_plmn_selection

==============================================================================*/

boolean reg_mode_start_higher_priority_plmn_selection
(
  const sys_detailed_plmn_list_s_type*       available_plmn_list_p
)
{
  boolean started = FALSE;

  reg_mode_p = &reg_mode_higher_priority_plmn_selection;

  started = reg_mode_p->start_higher_priority_plmn_selection
  (
    available_plmn_list_p
  );
  
  if (started)
  {
    reg_mode_network_selection_mode_set(SYS_NETWORK_SELECTION_MODE_AUTOMATIC);
  }
  
  return started;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_start_higher_priority_plmn_selection

==============================================================================*/

boolean reg_mode_start_higher_priority_irat_plmn_selection
(
  const sys_detailed_plmn_list_s_type*       available_plmn_list_p
)
{
  boolean started = FALSE;

  reg_mode_p = &reg_mode_higher_priority_plmn_selection;

  started = reg_mode_p->start_higher_priority_irat_plmn_selection
  (
    available_plmn_list_p
  );

  if (started)
  {
    reg_mode_network_selection_mode_set(SYS_NETWORK_SELECTION_MODE_AUTOMATIC);
  }
  
  return started;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_start_limited_service

==============================================================================*/

void reg_mode_start_limited_service
(
  sys_network_selection_mode_e_type    network_selection_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
#ifdef FEATURE_GSM_BAND_PREF
  sys_srv_domain_e_type                req_service_domain,
  sys_band_mask_type                   band_pref
#else
  sys_srv_domain_e_type                req_service_domain
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

  ,reg_cm_service_req_e_type           type
#ifdef FEATURE_LTE
 , boolean sms_only,
   uint32  additional_info
#endif
)
{
  reg_mode_p = &reg_mode_limited_service;

  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_mode_pref              = mode_pref;
  reg_mode_req_mode_pref          = mode_pref;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;
#ifdef FEATURE_GSM_BAND_PREF
  reg_mode_band_pref              = band_pref;
#endif
  memscpy(&reg_mode_rat_pri_list, sizeof(sys_rat_pri_list_info_s_type), rat_pri_list_info, sizeof(sys_rat_pri_list_info_s_type  ));

#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif
  reg_mode_p->start_limited_service
  (
    cs_for_request,
    ps_for_request,
#ifdef FEATURE_GSM_BAND_PREF
    req_service_domain,
    band_pref
#else
    req_service_domain
#endif
    ,rat_pri_list_info

    ,type
#ifdef FEATURE_LTE
   ,sms_only,
    additional_info
#endif
  );
}


/*==============================================================================

FUNCTION NAME

  reg_mode_start_manual_plmn_selection

==============================================================================*/

void reg_mode_start_manual_plmn_selection
(
  sys_network_selection_mode_e_type network_selection_mode,
  sys_plmn_id_s_type                   plmn,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
  ,sys_band_mask_type                   band_pref
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

  ,reg_cm_service_req_e_type           type
#ifdef FEATURE_LTE
  ,boolean sms_only,
   uint32  additional_info
#endif
)
{
  reg_mode_p = &reg_mode_manual_plmn_selection;

  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_mode_pref              = mode_pref;
  reg_mode_req_mode_pref          = mode_pref;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;
#ifdef FEATURE_GSM_BAND_PREF
  reg_mode_band_pref              = band_pref;
#endif
  memscpy(&reg_mode_rat_pri_list, sizeof(sys_rat_pri_list_info_s_type), rat_pri_list_info, sizeof(sys_rat_pri_list_info_s_type));


#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif

  reg_mode_p->start_manual_plmn_selection
  (
    network_selection_mode,
    plmn,
    mode_pref,
    cs_for_request,
    ps_for_request,
    req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
    ,band_pref
#endif
    ,rat_pri_list_info

    ,type
#ifdef FEATURE_LTE
   , sms_only,
     additional_info
#endif 
  );
}


/*==============================================================================

FUNCTION NAME

  reg_mode_start_plmn_selection

==============================================================================*/

void reg_mode_start_plmn_selection
(
  sys_network_selection_mode_e_type    network_selection_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
  ,sys_band_mask_type                   band_pref
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

  ,reg_cm_service_req_e_type           type
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  reg_mode_p = &reg_mode_plmn_selection;

  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_mode_pref              = mode_pref;
  reg_mode_req_mode_pref          = mode_pref;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;
#ifdef FEATURE_GSM_BAND_PREF
  reg_mode_band_pref              = band_pref;
#endif
  memscpy(&reg_mode_rat_pri_list, sizeof(sys_rat_pri_list_info_s_type), rat_pri_list_info, sizeof(sys_rat_pri_list_info_s_type));


#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif
  reg_mode_p->start_plmn_selection
  (
    network_selection_mode,
    mode_pref,
    cs_for_request,
    ps_for_request,
    req_service_domain,
    (boolean)((network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) ? FALSE : TRUE)
#ifdef FEATURE_GSM_BAND_PREF
    ,band_pref
#endif
    ,rat_pri_list_info

    ,type
#ifdef FEATURE_LTE
    ,sms_only,
     additional_info
#endif
  );
}

void reg_mode_start_plmn_selection_roaming_rplmn
(
  sys_network_selection_mode_e_type    network_selection_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
  ,sys_band_mask_type                   band_pref
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

  ,reg_cm_service_req_e_type           type
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  reg_mode_p = &reg_mode_plmn_selection;
  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_mode_pref              = mode_pref;
  reg_mode_req_mode_pref          = mode_pref;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;
#ifdef FEATURE_GSM_BAND_PREF
  reg_mode_band_pref              = band_pref;
#endif
  memscpy(&reg_mode_rat_pri_list, sizeof(sys_rat_pri_list_info_s_type), rat_pri_list_info, sizeof(sys_rat_pri_list_info_s_type));

  
#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif
  
  reg_mode_p->start_plmn_selection_roaming_plmn
  (
    network_selection_mode,
    mode_pref,
    cs_for_request,
    ps_for_request,
    req_service_domain,
    TRUE
#ifdef FEATURE_GSM_BAND_PREF
    ,band_pref
#endif
    ,rat_pri_list_info

    ,type
#ifdef FEATURE_LTE
    ,sms_only,
     additional_info
#endif
  );
}


/*==============================================================================

FUNCTION NAME

  reg_mode_start_plmn_selection_on_specified_plmn

==============================================================================*/

void reg_mode_start_plmn_selection_on_specified_plmn
(
  sys_plmn_id_s_type                   plmn,
  sys_network_selection_mode_e_type    network_selection_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain,
#ifdef FEATURE_GSM_BAND_PREF
  sys_band_mask_type                   band_pref,
#endif
  const sys_rat_pri_list_info_s_type  *rat_pri_list_info,

  sys_radio_access_tech_e_type         active_rat
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
 , mmr_reg_req_e_type reg_req_type
 , reg_mode_automatic_mode_e_type automatic_mode
)

{
  reg_mode_p = &reg_mode_plmn_selection;

  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_mode_pref              = mode_pref;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;
#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif

  reg_mode_p->start_plmn_selection_on_specified_plmn
  (
    plmn,
    network_selection_mode,
    mode_pref,
    cs_for_request,
    ps_for_request,
    req_service_domain,
#ifdef FEATURE_GSM_BAND_PREF
    band_pref,
#endif
    rat_pri_list_info,

    active_rat
#ifdef FEATURE_LTE
   , sms_only,
     additional_info
#endif 
   , reg_req_type,
    automatic_mode
  );
}


/*==============================================================================

FUNCTION NAME

  reg_mode_start_manual_plmn_selection_on_specified_plmn

==============================================================================*/

void reg_mode_start_manual_plmn_selection_on_specified_plmn
(
  sys_plmn_id_s_type                   plmn,
  sys_network_selection_mode_e_type    network_selection_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain,
#ifdef FEATURE_GSM_BAND_PREF
  sys_band_mask_type                   band_pref,
#endif
  const sys_rat_pri_list_info_s_type  *rat_pri_list_info,

  sys_radio_access_tech_e_type         active_rat
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
 , mmr_reg_req_e_type reg_req_type
, reg_mode_automatic_mode_e_type automatic_mode
)
{
  reg_mode_p = &reg_mode_manual_plmn_selection;

  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_mode_pref              = mode_pref;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;
#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif

  reg_mode_p->start_plmn_selection_on_specified_plmn
  (
    plmn,
    network_selection_mode,
    mode_pref,
    cs_for_request,
    ps_for_request,
    req_service_domain,
#ifdef FEATURE_GSM_BAND_PREF
    band_pref,
#endif
    rat_pri_list_info,

    active_rat
#ifdef FEATURE_LTE
   , sms_only,
     additional_info
#endif 
   , reg_req_type
    , automatic_mode
  );
}



/*==============================================================================

FUNCTION NAME

  reg_mode_reinitiate_plmn_selection

==============================================================================*/

void reg_mode_reinitiate_plmn_selection
(
  sys_network_selection_mode_e_type    network_selection_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
#ifdef FEATURE_GSM_BAND_PREF
  sys_srv_domain_e_type                req_service_domain,
  sys_band_mask_type                   band_pref
#else
  sys_srv_domain_e_type                req_service_domain
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  reg_mode_p = &reg_mode_plmn_selection;

  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_mode_pref              = mode_pref;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;
#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif

  reg_mode_p->start_plmn_selection
  (
    network_selection_mode,
    mode_pref,
    cs_for_request,
    ps_for_request,
    req_service_domain,
    FALSE
#ifdef FEATURE_GSM_BAND_PREF
    ,band_pref
#endif
    ,rat_pri_list_info

    ,REG_CM_SERVICE_REQ_NORMAL
#ifdef FEATURE_LTE
   , sms_only,
     additional_info
#endif
  );
}

void reg_mode_start_foreground_plmn_search
(
  void
)
{
  if((reg_mode_p->number() == REG_MODE_LIMITED_SERVICE) &&
     (reg_mode_network_selection_mode == SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION))
  {
    reg_mode_network_selection_mode = SYS_NETWORK_SELECTION_MODE_AUTOMATIC;
    
  }
  /* All foreground search will be done as user-reselection*/
  reg_mode_p = &reg_mode_automatic_plmn_selection;

  reg_mode_p->start_foreground_plmn_search
  (
    reg_mode_network_selection_mode,
    reg_mode_mode_pref,
    reg_mode_req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
    , reg_mode_band_pref 
#endif
    , &reg_mode_rat_pri_list

#ifdef FEATURE_LTE
   , reg_mode_sms_only,
     0
#endif
  );
}

boolean reg_mode_start_preferred_plmn_search
(
  void
)
{
  boolean ret_value = FALSE;

  //return ret_value;
  if((reg_mode_p->number() == REG_MODE_LIMITED_SERVICE) &&
     (reg_mode_network_selection_mode == SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION))
  {
    reg_mode_network_selection_mode = SYS_NETWORK_SELECTION_MODE_AUTOMATIC;
    
  }

  /* All foreground search will be done as user-reselection*/
  reg_mode_p = &reg_mode_automatic_plmn_selection;

  ret_value = reg_mode_p->start_preferred_plmn_search
  (
    reg_mode_network_selection_mode,
    reg_mode_mode_pref,
    reg_mode_req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
    , reg_mode_band_pref 
#endif
    , &reg_mode_rat_pri_list

#ifdef FEATURE_LTE
   , reg_mode_sms_only,
     0
#endif
  );
  
  return ret_value;
}
/*==============================================================================

FUNCTION NAME

  reg_mode_finished

==============================================================================*/

boolean reg_mode_finished
(
        sys_plmn_service_state_s_type  service_state,
  const sys_detailed_plmn_list_s_type* available_plmn_list_p
)
{
  return reg_mode_p->finished
  (
    service_state,
    available_plmn_list_p
  );
}


/*==============================================================================

FUNCTION NAME

  reg_mode_number

==============================================================================*/

reg_mode_e_type reg_mode_number
(
  void
)
{
  return reg_mode_p->number();
}


/*==============================================================================

FUNCTION NAME

  reg_mode_successful_registration

==============================================================================*/

boolean reg_mode_successful_registration
(
  sys_plmn_id_s_type                   plmn,
  sys_srv_status_e_type                service_status,
  sys_radio_access_tech_e_type         rat,
  sys_reg_domain_e_type                plmn_reg_type
)
{
  boolean success = FALSE;

  if(reg_mode_network_selection_mode != SYS_NETWORK_SELECTION_MODE_AUTOMATIC_CAMP_ONLY &&
     reg_mode_network_selection_mode != SYS_NETWORK_SELECTION_MODE_MANUAL_CAMP_ONLY)
  {
    success = (boolean)(((rat == reg_mode_req_rat_get()) 
                        ) &&
                         (service_status != SYS_SRV_STATUS_NO_SRV) &&
                         (service_status != SYS_SRV_STATUS_LIMITED) );
  }
  else
  {
    success = (boolean)( (sys_plmn_match(reg_mode_req_plmn_get(), plmn) ||
                         (sys_eplmn_list_equivalent_plmn(reg_mode_req_plmn_get()) &&
                          sys_eplmn_list_equivalent_plmn(plmn))) &&
                         (rat == reg_mode_req_rat_get()) &&
                         (service_status != SYS_SRV_STATUS_NO_SRV) &&
                         ((service_status == SYS_SRV_STATUS_LIMITED) &&
                          ((plmn_reg_type == SYS_REG_DOMAIN_CS_SERVICE_ONLY) ||
                           (plmn_reg_type == SYS_REG_DOMAIN_PS_SERVICE_ONLY) ||
                           (plmn_reg_type == SYS_REG_DOMAIN_CS_PS_SERVICE))) 
                          );

  }

  return success;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_initialize_equivalent_plmn_list

==============================================================================*/

void reg_mode_initialize_equivalent_plmn_list
(
#ifdef FEATURE_ENHANCED_NW_SELECTION
  boolean force_delete
#else
  void
#endif
)
{
  sys_plmn_id_s_type   rplmn;
  sys_plmn_list_s_type equivalent_plmn_list;
  reg_sim_plmn_list_s_type forbidden_plmn_list;
  uint32                   position;
  uint32                   i;
  uint32                   j;
  boolean sim_changed     = FALSE;
  boolean rplmn_gprs_forbidden = FALSE;

#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
  sys_plmn_id_s_type   rplmn_undef;
#endif
  forbidden_plmn_list.length = 0;

#ifdef FEATURE_NAS_ECALL
  if (!reg_ecall_profile)
#endif
  {
    sim_changed = reg_sim_changed;
  }
  
#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
  sys_plmn_undefine_plmn_id(&rplmn_undef);    
#endif 
  /*
  ** Delete the EPLMN list stored in NVRAM if the SIM has changed.
  */
#ifdef FEATURE_ENHANCED_NW_SELECTION
  if (sim_changed || force_delete)
#else
  if (sim_changed)
#endif
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= SIM changed - Delete EPLMN list" );

    equivalent_plmn_list.length = 0;
    sys_plmn_undefine_plmn_id(&rplmn);    
    reg_nv_write_equivalent_plmn_list
    (
      rplmn,
      &equivalent_plmn_list
    );
    if((reg_is_qrd_device == TRUE) && reg_nv_vplmn_list )
    {
      memset(reg_nv_vplmn_list, 0x0, sizeof(reg_nv_vplmn_list_s_type));   

      if(reg_nv_write_vplmn_list() == MCFG_FS_STATUS_OK)
      {
        MSG_HIGH_DS_0(REG_SUB, "=REG= VPLMN: Delete VPLMN list success");
        reg_nv_output_vplmn_list();
      }
    }
  }
  else
  {
    reg_nv_read_equivalent_plmn_list
    (
      &rplmn,
      &equivalent_plmn_list
    );
  }


 
  if (sim_changed)
  {
#ifdef FEATURE_SGLTE
    reg_as_id_e_type temp_as_id;
#endif
#ifdef FEATURE_DUAL_SIM
   reg_mode_clear_ehplmn_forbidden((sys_modem_as_id_e_type)reg_as_id);
   reg_mode_clear_ehplmn_gprs_forbidden((sys_modem_as_id_e_type)reg_as_id);

#else
    reg_mode_clear_ehplmn_forbidden();
    reg_mode_clear_ehplmn_gprs_forbidden();
#endif
    MSG_HIGH_DS_0(REG_SUB, "=REG= SIM changed - delete RPLMN ACT" );
    reg_sim_write_rplmn_rat(SYS_RAT_NONE);
#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
    reg_nv_write_rplmn(rplmn_undef);
    reg_nv_write_rplmn_dup(FALSE);
#ifdef FEATURE_SGLTE
    if(REG_SUB_IS_SGLTE)
    {
      temp_as_id= reg_as_id;
      reg_set_as_id(REG_SGLTE_STACK_2);
      reg_nv_write_rplmn(rplmn_undef);
      reg_nv_write_rplmn_dup(FALSE);
      reg_set_as_id(temp_as_id);
    }
#endif

#endif 
    reg_mode_reset_validate_sim_counter();
    reg_nv_reset_pm_chg_pending_plmn_rat();
  }
#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
  else if( reg_nv_read_rplmn_dup()== FALSE 
#ifdef FEATURE_NAS_ECALL
          && (reg_ecall_profile == FALSE)
#endif
           )
  {
    reg_nv_write_rplmn(rplmn_undef);
  }
#endif


  /*
  ** Remove any forbidden PLMNs contained in the equivalent PLMN list.
  */
#ifdef FEATURE_DUAL_SIM
  (void)reg_sim_read_fplmn_list(&forbidden_plmn_list,FALSE, (sys_modem_as_id_e_type)reg_sub_id );
#else
  (void)reg_sim_read_fplmn_list(&forbidden_plmn_list, FALSE);
#endif

#ifdef FEATURE_DUAL_SIM
  rplmn_gprs_forbidden = reg_sim_plmn_gprs_forbidden(rplmn, (sys_modem_as_id_e_type)reg_sub_id);
#else
  rplmn_gprs_forbidden = reg_sim_plmn_gprs_forbidden(rplmn);
#endif


  if (reg_sim_find_plmn_in_list(rplmn,
                                 &forbidden_plmn_list,
                                 &position))
  {
    sys_plmn_undefine_plmn_id(&rplmn);
    equivalent_plmn_list.length = 0;
  }

  if (equivalent_plmn_list.length != 0)
  {
    i = 0;
    while (i < equivalent_plmn_list.length)
    {
      //if RPLMN is GPRS forbidden then donot delete GPRS FPLMNs from EPLMN list
      if (( reg_sim_find_plmn_in_list(equivalent_plmn_list.plmn[i],
                                     &forbidden_plmn_list,
                                     &position))  ||
#ifdef FEATURE_DUAL_SIM
          ((reg_sim_plmn_gprs_forbidden(equivalent_plmn_list.plmn[i], (sys_modem_as_id_e_type)reg_sub_id)) &&
#else
          ((reg_sim_plmn_gprs_forbidden(equivalent_plmn_list.plmn[i])) &&
#endif
            (rplmn_gprs_forbidden == FALSE)))
      {
        for (j = i+1; j < equivalent_plmn_list.length; j++)
        {
          equivalent_plmn_list.plmn[j-1] =
            equivalent_plmn_list.plmn[j];
        }
        equivalent_plmn_list.length--;
      }
      else
      {
        i++;
      }
    }
  }

  /*
  ** Undefine the RPLMN if the equivalent PLMN list does
  ** not contain any entries.  This is performed as a
  ** separate check since forbidden entries in the list
  ** are removed above.
  */
  if (equivalent_plmn_list.length == 0)
  {
    sys_plmn_undefine_plmn_id(&rplmn);
  }

  sys_eplmn_list_update
  (
    rplmn,
    &equivalent_plmn_list
  );
  
  (void)sys_eplmn_list_validate(TRUE);
  reg_ssgccs_send_ehplmn_and_fplmn();

  (void)sys_eplmn_list_equivalent_plmn(rplmn);
}

/*===========================================================================

FUNCTION  REG_SIM_DECODE_LENGTH

DESCRIPTION
  This function is used to decode the lengh of an input buffer, used while parsing the NAS config file from USIM.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
int reg_sim_decode_length(byte *stream, byte *offset) 
{

  //len should point to the first byte of length field
  int decoded_len = 0;
  int first_byte = 0;
  int i = 0;
  int number_of_octets = 0;

   
  ASSERT( stream != NULL );

  ASSERT ( offset != NULL );

  first_byte = *stream++;

  if( first_byte & 0x80 )
  { 
    //Multi-byte length
    number_of_octets = first_byte & 0x7F;
    for(i = 0; i < number_of_octets; i++ ){
      decoded_len *= 256;
      decoded_len += *stream++;
    }
    *offset = (byte)number_of_octets;
  } else{
    //Single byte length
    decoded_len = first_byte;
    *offset = 1;
  }
  return decoded_len;
}


/*===========================================================================
FUNCTION  reg_mode_parse_nas_config

DESCRIPTION
  This function parses the nas configuration file read from USIM or NV

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void reg_mode_parse_nas_config
(
  byte *nas_config_byte_stream, 
  int32 file_size
)
{
  int32 len = 0;
  byte  offset = 0; 
  byte value = 0;
  boolean tag80_present = FALSE;
  boolean tag83_present = FALSE;
  boolean tag85_present = FALSE;
  boolean tag88_present = FALSE;
  boolean tag89_present = FALSE;

  /* reset nasconfig data before parsing */
  reg_sim_set_low_pri_cfg(FALSE);
  reg_sim_set_min_periodic_search_timer_value(0);
  reg_sim_set_t3245_behavior_config(FALSE);
#ifdef FEATURE_NAS_REL12_SPEC_CHANGE
  reg_sim_set_fast_hplmn_tag(FALSE);
#endif

  file_size = MIN(REG_MODE_EFNAS_CONF_LEN,file_size);

  while((len + 3) <= file_size)
  {
    MSG_HIGH_DS_3(REG_SUB, "=REG= EFNASCONFIG: 0x%02x 0x%02x 0x%02x",
                  nas_config_byte_stream[len],
                  nas_config_byte_stream[len+1], 
                  nas_config_byte_stream[len+2]);
    len += 3;
  }

  len = 0;

  while((file_size > 0) && 
        ((tag80_present == FALSE) ||
         (tag83_present == FALSE) ||
         (tag85_present == FALSE)
#ifdef FEATURE_NAS_REL12_SPEC_CHANGE
        ||(tag88_present == FALSE)
#endif
#if defined (FEATURE_LTE_REL12)
        || (tag89_present == FALSE)
#endif
         ))
  {
    switch(*nas_config_byte_stream)
    {
      case 0x80:
        nas_config_byte_stream++;
        file_size--;
        len = reg_sim_decode_length(nas_config_byte_stream, &offset);
        if(len != 1)
        {
          MSG_ERROR_DS_1(REG_SUB, "Invalid NAS signalling priority value length = %d, must be 1",len);
          return;
        }
        else
        {
          nas_config_byte_stream += offset;
          value = *nas_config_byte_stream;
          if(value == 1)
          {
            reg_sim_set_low_pri_cfg(TRUE);
            tag80_present = TRUE;
          }
          else
          {
            MSG_ERROR_DS_1(REG_SUB, "Invalid NAS signalling priority value = %d, must be 1",value);
          }
          nas_config_byte_stream += len;
          file_size = file_size - offset - len;
        }
        break;

      case 0x81:
        nas_config_byte_stream++;
        file_size--;
        len = reg_sim_decode_length ( nas_config_byte_stream, &offset );
        /* skip the length fields */
        nas_config_byte_stream += offset;
        nas_config_byte_stream += len;
        file_size = file_size - offset - len;
        break;

       case 0x82:
        nas_config_byte_stream++;
        file_size--;
        len = reg_sim_decode_length ( nas_config_byte_stream, &offset );
        /* skip the length fields */
        nas_config_byte_stream += offset;
        nas_config_byte_stream += len;
        file_size = file_size - offset - len;
        break;

      case 0x83:
        nas_config_byte_stream++;
        file_size--;
        len = reg_sim_decode_length ( nas_config_byte_stream, &offset );
        if(len != 1)
        {
          MSG_ERROR_DS_1(REG_SUB, "Invalid Minimum Periodic Search Timer value length = %d, must be 1",len);
          return;
        }
        else
        {
          nas_config_byte_stream += offset;
          value = *nas_config_byte_stream;
          reg_sim_set_min_periodic_search_timer_value(value);
          nas_config_byte_stream += len;
          file_size = file_size - offset - len;
          tag83_present = TRUE;
        }
        break;

      case 0x84:
        nas_config_byte_stream++;
        file_size--;
        len = reg_sim_decode_length ( nas_config_byte_stream, &offset );
        /* skip the length fields */
        nas_config_byte_stream += offset;
        nas_config_byte_stream += len;
        file_size = file_size - offset - len;
        break;

      case 0x85:
        nas_config_byte_stream++;
        file_size--;
        len = reg_sim_decode_length(nas_config_byte_stream, &offset);
        if(len != 1)
        {
          MSG_ERROR_DS_1(REG_SUB, "Invalid Timer T3245 Behaviour value length = %d, must be 1",len);
          return;
        }
        else
        {
          nas_config_byte_stream += offset;
          value = *nas_config_byte_stream;
          if((value == 0) || (value == 1))
          {
            reg_sim_set_t3245_behavior_config((boolean)value);
            tag85_present = TRUE;
          }
          else
          {
            MSG_ERROR_DS_1(REG_SUB, "Invalid Timer T3245 Behaviour value = %d, must be 0 or 1",value);
          }
          nas_config_byte_stream += len;
          file_size = file_size - offset - len;
        }
        break;

       case 0x86:
        nas_config_byte_stream++;
        file_size--;
        len = reg_sim_decode_length ( nas_config_byte_stream, &offset );
        /* skip the length fields */
        nas_config_byte_stream += offset;
        nas_config_byte_stream += len;
        file_size = file_size - offset - len;
        break;

       case 0x87:
        nas_config_byte_stream++;
        file_size--;
        len = reg_sim_decode_length ( nas_config_byte_stream, &offset );
        /* skip the length fields */
        nas_config_byte_stream += offset;
        nas_config_byte_stream += len;
        file_size = file_size - offset - len;
        break;

       case 0x88:
#ifdef FEATURE_NAS_REL12_SPEC_CHANGE
        nas_config_byte_stream++;
        file_size--;
        len = reg_sim_decode_length ( nas_config_byte_stream, &offset );
        if(len < 1)
        {
          MSG_ERROR_DS_0(REG_SUB, "Fast first hplmn search tag is misconfigured, length is zero");
          return;
        }
        else
        {
           /*If length > 1 read the buffer*/
          nas_config_byte_stream += offset;
          value = *nas_config_byte_stream;
          if((value == 0) || (value == 1))
          {
            reg_sim_set_fast_hplmn_tag((boolean)value);
          }
          else
          {
            MSG_ERROR_DS_1(REG_SUB, "Invalid FFHPLMN tag value = %d, must be 0 or 1",value);
          }

        nas_config_byte_stream += len;
        file_size = file_size - offset - len;
        tag88_present = TRUE;
        }
#else
        nas_config_byte_stream++;
        file_size--;
        len = reg_sim_decode_length ( nas_config_byte_stream, &offset );
        /* skip the length fields */
        nas_config_byte_stream += offset;
        nas_config_byte_stream += len;
        file_size = file_size - offset - len;
#endif
        break;


#ifdef  FEATURE_LTE_REL12
       case 0x89: 
        nas_config_byte_stream++;
        file_size--;
        len = reg_sim_decode_length(nas_config_byte_stream, &offset);
        if(len < 1)
        {
          MSG_ERROR_DS_1(REG_SUB, "EUTRAN tag is misconfigured, length is zero",len);
          return;
        }
        else
        {
          nas_config_byte_stream += offset;
          value = *nas_config_byte_stream;
          if((value == 0) || (value == 1))
          {
            reg_sim_set_eutran_not_allowed((boolean)value);
          }
          else
          {
            MSG_ERROR_DS_1(REG_SUB, "Invalid EUTRAN tag value = %d, must be 0 or 1",value);
          }
          nas_config_byte_stream += len;
          file_size = file_size - offset - len;
        }
        break; 
#endif

       default:
        file_size = 0;
        break;
    }
  }
}



/*==============================================================================

FUNCTION NAME

  reg_mode_t3245_efnas_config_behavior

==============================================================================*/

void reg_mode_t3245_efnas_config_behavior(void)
{
  boolean is_fplmn_list_has_valid_plmn = FALSE;
  rex_timer_cnt_type t3245_value = REG_T3245_DEFUALT_VALUE;
  boolean t3245_behavior_enabled = FALSE;

#ifdef FEATURE_DUAL_SIM
  t3245_behavior_enabled = reg_sim_check_if_t3245_behavior_enabled((sys_modem_as_id_e_type)reg_sub_id);
#else
  t3245_behavior_enabled = reg_sim_check_if_t3245_behavior_enabled();
#endif
  if(t3245_behavior_enabled == TRUE)
  {
    uint32 i;
    reg_sim_plmn_list_s_type forbidden_plmn_list;
  
    boolean is_t3245_active = FALSE;
#ifdef FEATURE_DUAL_SIM
    is_t3245_active = reg_is_t3245_timer_active((sys_modem_as_id_e_type)reg_sub_id);
#else
    is_t3245_active = reg_is_t3245_timer_active();
#endif
#ifdef FEATURE_DUAL_SIM
    reg_sim_read_fplmn_list(&forbidden_plmn_list, FALSE, (sys_modem_as_id_e_type)reg_sub_id );
#else
    reg_sim_read_fplmn_list(&forbidden_plmn_list, FALSE);
#endif
  
    for(i = 0; i < forbidden_plmn_list.length && i < REG_SIM_PLMN_LIST_LENGTH; i++)
    {
      if(!sys_plmn_id_is_undefined(forbidden_plmn_list.plmn[i]))
      {
        is_fplmn_list_has_valid_plmn = TRUE;
        break;
      }
    }
    MSG_HIGH_DS_3(REG_SUB, "=REG= sim changed %d, valid FPLMN present %d is_t3245_active %d",
                  reg_sim_changed, 
                  is_fplmn_list_has_valid_plmn,
                  is_t3245_active);

    if(is_fplmn_list_has_valid_plmn == TRUE)
    {
      if(reg_sim_changed == FALSE)
      {
        if(is_t3245_active == FALSE)
        {
          t3245_value = reg_nv_t3245_timer_stored_value_get();
#ifdef FEATURE_DUAL_SIM
          reg_timers_start_t3245_timer(t3245_value, (sys_modem_as_id_e_type)reg_sub_id);
#else
          reg_timers_start_t3245_timer(t3245_value);
#endif
        }
      }
      else
      {
        t3245_value = REG_T3245_DEFUALT_VALUE;
#ifdef FEATURE_DUAL_SIM
        reg_timers_start_t3245_timer(t3245_value, (sys_modem_as_id_e_type)reg_sub_id);
#else
        reg_timers_start_t3245_timer(t3245_value);
#endif
 
      }
    }
  }
  
  if((t3245_behavior_enabled == FALSE) ||
     (is_fplmn_list_has_valid_plmn == FALSE))
  {
    reg_timers_stop_t3245();
  }

}


/*==============================================================================

FUNCTION NAME

  reg_mode_save_equivalent_plmn_list

==============================================================================*/

boolean reg_mode_save_equivalent_plmn_list
(
  sys_plmn_id_s_type                   rplmn,
  sys_plmn_list_s_type*                equivalent_plmn_list_p
)
{
  reg_sim_plmn_list_s_type forbidden_plmn_list;
  uint32                   position;
  uint32                   i;
  uint32                   j;
  boolean                  list_matched;
  boolean                  rplmn_gprs_forbidden = FALSE;

  forbidden_plmn_list.length = 0;

  reg_nv_write_equivalent_plmn_list
  (
    rplmn,
    equivalent_plmn_list_p
  );

  /*
  ** Remove any forbidden PLMNs contained in the equivalent PLMN list.
  */
#ifdef FEATURE_DUAL_SIM
  (void) reg_sim_read_fplmn_list(&forbidden_plmn_list, FALSE, (sys_modem_as_id_e_type)reg_sub_id );
#else
  (void)reg_sim_read_fplmn_list(&forbidden_plmn_list, FALSE);
#endif
#ifdef FEATURE_DUAL_SIM
  rplmn_gprs_forbidden = reg_sim_plmn_gprs_forbidden(rplmn, (sys_modem_as_id_e_type)reg_sub_id);
#else
  rplmn_gprs_forbidden = reg_sim_plmn_gprs_forbidden(rplmn);
#endif

  if (reg_sim_find_plmn_in_list(rplmn,
                                 &forbidden_plmn_list,
                                 &position))
  {
    sys_plmn_undefine_plmn_id(&rplmn);
    equivalent_plmn_list_p->length = 0;
  }

  if (equivalent_plmn_list_p->length != 0)
  {
    i = 0;
    while (i < equivalent_plmn_list_p->length)
    {
      //if RPLMN is GPRS forbidden then donot delete GPRS FPLMNs from EPLMN list
      if (( reg_sim_find_plmn_in_list(equivalent_plmn_list_p->plmn[i],
                                     &forbidden_plmn_list,
                                     &position)) ||
#ifdef FEATURE_DUAL_SIM
          ((reg_sim_plmn_gprs_forbidden(equivalent_plmn_list_p->plmn[i], (sys_modem_as_id_e_type)reg_sub_id))&&
#else
          ((reg_sim_plmn_gprs_forbidden(equivalent_plmn_list_p->plmn[i]))&&
#endif
           (rplmn_gprs_forbidden == FALSE)))
      {
        for (j = i+1; j < equivalent_plmn_list_p->length; j++)
        {
          equivalent_plmn_list_p->plmn[j-1] =
            equivalent_plmn_list_p->plmn[j];
        }
        equivalent_plmn_list_p->length--;
      }
      else
      {
        i++;
      }
    }
  }

  /*
  ** Undefine the RPLMN if the equivalent PLMN list does
  ** not contain any entries.  This is performed as a
  ** separate check since forbidden entries in the list
  ** are removed above.
  */
  if (equivalent_plmn_list_p->length == 0)
  {
    sys_plmn_undefine_plmn_id(&rplmn);
  }

  list_matched = sys_eplmn_list_compare_eplmn_list(rplmn,equivalent_plmn_list_p);
                                                   
  sys_eplmn_list_update
  (
    rplmn,
    equivalent_plmn_list_p
  );

  return list_matched;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_equivalent_plmn

==============================================================================*/

boolean reg_mode_equivalent_plmn
(
  sys_plmn_id_s_type                   rplmn,
  sys_plmn_id_s_type                   serving_plmn
)
{
  if (sys_plmn_match(rplmn, serving_plmn))
  {
    return TRUE;
  }

  if (!sys_plmn_id_is_undefined(rplmn) &&
       sys_plmn_match(rplmn, sys_eplmn_list_rplmn()))
  {
    if (sys_eplmn_list_equivalent_plmn(serving_plmn))
    {
      return TRUE;
    }
  }

  return FALSE;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_is_dual_mode

==============================================================================*/

boolean reg_mode_is_dual_mode
(
  sys_mode_pref_e_type                 mode_pref
)
{

  if ((mode_pref == SYS_MODE_PREF_GSM_RADIO_ACCESS_PREFERRED) ||
      (mode_pref == SYS_MODE_PREF_UMTS_RADIO_ACCESS_PREFERRED))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }

}


/*==============================================================================

FUNCTION NAME

  reg_mode_next_equivalent_rplmn

==============================================================================*/

void reg_mode_next_equivalent_rplmn
(
  sys_plmn_id_s_type*                  plmn_p,
  sys_radio_access_tech_e_type*        rat_p
)
{
  sys_plmn_id_s_type rplmn;
  uint32             i;

  sys_plmn_undefine_plmn_id(plmn_p);
  *rat_p = SYS_RAT_GSM_RADIO_ACCESS;

  rplmn = reg_sim_read_rplmn(reg_mode_req_service_domain_get());

  if (!sys_plmn_id_is_undefined(rplmn))
  {
    for (i = 0; i < reg_mode_available_plmn_list.length; i++)
    {
      /*
      ** Skip any PLMN-RAT combinations that have previously been attempted.
      */
      if (reg_mode_found_attempted_plmn_rat(reg_mode_available_plmn_list.info[i].plmn,
                                            reg_mode_available_plmn_list.info[i].rat) )
      {
        continue;
      }

      /*
      ** Try the second RPLMN-RAT combination if necessary.
      */
      if (sys_plmn_match(rplmn, reg_mode_available_plmn_list.info[i].plmn))
      {
        *plmn_p = reg_mode_available_plmn_list.info[i].plmn;
        *rat_p  = reg_mode_available_plmn_list.info[i].rat;
        break;
      }

      /*
      ** Try any equivalent RPLMN-RAT combination if necessary.
      */
      if (sys_plmn_match(rplmn, sys_eplmn_list_rplmn()))
      {
        if (sys_eplmn_list_equivalent_plmn(reg_mode_available_plmn_list.info[i].plmn))
        {
          *plmn_p = reg_mode_available_plmn_list.info[i].plmn;
          *rat_p  = reg_mode_available_plmn_list.info[i].rat;
          break;
        }
      }
    }
  }
}


/*==============================================================================

FUNCTION NAME

  reg_mode_available_plmn_list_get

==============================================================================*/

const sys_detailed_plmn_list_s_type* reg_mode_available_plmn_list_get
(
  void
)
{
  return &reg_mode_available_plmn_list;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_available_plmn_list_manual_get

==============================================================================*/

const sys_detailed_plmn_list_s_type* reg_mode_available_plmn_list_manual_get
(
  void
)
{
  return &reg_mode_available_plmn_list_manual;
}



/*==============================================================================

FUNCTION NAME

  reg_mode_get_available_backgnd_list

==============================================================================*/

const sys_detailed_plmn_list_s_type* reg_mode_get_available_backgnd_list
(
  void
)
{
  return  reg_mode_backgnd_list;
}



/*==============================================================================

FUNCTION NAME

  reg_mode_clear_available_plmn_list

==============================================================================*/

void reg_mode_clear_available_plmn_list
(
  void
)
{
  reg_mode_available_plmn_list.length = 0;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_reset_available_plmn_list

==============================================================================*/
  
void reg_mode_reset_available_plmn_list
(
  sys_plmn_id_s_type                   plmn,
  const reg_mode_3gpp_capability_s_type *rat_3gpp_capabilty
)
{

  uint32 i,length;
  reg_mode_clear_available_plmn_list();

  for (i = 0; i < rat_3gpp_capabilty->num_items; i++)
  {
     length = reg_mode_available_plmn_list.length;
     
     reg_mode_available_plmn_list.info[length].plmn                    = plmn;
     reg_mode_available_plmn_list.info[length].rat                     = rat_3gpp_capabilty->rat_list[i];
     reg_mode_available_plmn_list.info[length].plmn_service_capability = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
     reg_mode_available_plmn_list.info[length].plmn_forbidden          = FALSE;
     reg_mode_available_plmn_list.info[length].list_category           = SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER;
     reg_mode_available_plmn_list.info[length].signal_quality          = SYS_SIGNAL_QUALITY_HIGH;
     reg_mode_available_plmn_list.info[length].signal_strength         = 0;
     reg_mode_available_plmn_list.length++;
  }
}

/*==============================================================================

FUNCTION NAME

  reg_mode_update_available_plmn_list

==============================================================================*/

void reg_mode_update_available_plmn_list
(
  reg_mode_automatic_mode_e_type automatic_mode,

  const sys_rat_pri_list_info_s_type  *rat_pri_list_info,

        sys_srv_domain_e_type          req_service_domain,
  const sys_detailed_plmn_list_s_type* available_plmn_list_p
)
{
  reg_sim_plmn_list_s_type forbidden_plmn_list;
  boolean                  found_plmn_rat = FALSE;
  uint32                   position;
  uint32                   i;
  uint32                   j;
  
#ifdef FEATURE_DUAL_SIM
  if(automatic_mode == REG_MODE_AUTOMATIC_MODE_CAMPED_PLMN_RAT)
  { 
    MSG_HIGH_DS_1(REG_SUB, "=REG= Available PLMN list (length = %u), not updating available PLMNs for Camped PLMN RAT type service request", reg_mode_available_plmn_list.length);
    reg_mode_output_detailed_plmn_list
    (
      &reg_mode_available_plmn_list
#ifdef FEATURE_FEMTO_CSG
      , FALSE
#endif 
    );
    return;
  }
#endif 
  if (reg_mode_selection_to_gw_first_pass == TRUE)
  {
      MSG_HIGH_DS_1(REG_SUB, "=REG= Available PLMN list (length = %u), not updating available PLMNs for first fallback pass", reg_mode_available_plmn_list.length);
      reg_mode_output_detailed_plmn_list
          (
          &reg_mode_available_plmn_list
#ifdef FEATURE_FEMTO_CSG
          , FALSE
#endif 
          );
      return;
      
  }

  /*
  ** Add PLMN-RAT combinations to the available PLMN list that are not
  ** already contained in the available PLMN list.
  */
  for (i = 0; i < available_plmn_list_p->length; i++)
  {
    found_plmn_rat = FALSE;

    for (j = 0; j < reg_mode_available_plmn_list.length; j++)
    {
      if ( (sys_plmn_match(reg_mode_available_plmn_list.info[j].plmn,
                           available_plmn_list_p->info[i].plmn) ) &&
           (reg_mode_available_plmn_list.info[j].rat ==
              available_plmn_list_p->info[i].rat) )
      {
        /*
        ** Update the information for the PLMN-RAT combination already
        ** contained in the available PLMN list.
        */
        reg_mode_available_plmn_list.info[j] = available_plmn_list_p->info[i];

        found_plmn_rat = TRUE;
        break;
      }
    }

    /*
    ** Add the new PLMN-RAT combination to the available PLMN list.
    */
    if ( !(found_plmn_rat
#ifdef FEATURE_LTE
             ||(reg_sim_plmn_rat_forbidden(available_plmn_list_p->info[i].plmn,
                 available_plmn_list_p->info[i].rat,(sys_modem_as_id_e_type)reg_sub_id))
#endif
       ))
    {
      if (reg_mode_available_plmn_list.length < SYS_PLMN_LIST_MAX_LENGTH)
      {
        reg_mode_available_plmn_list.info[reg_mode_available_plmn_list.length] =
          available_plmn_list_p->info[i];

        reg_mode_available_plmn_list.length++;
      }
    }
  }

  /*
  ** Remove any forbidden PLMN-RAT combinations from the available PLMN list.
  */
  if ((reg_mode_available_plmn_list.length != 0) &&
      (automatic_mode != REG_MODE_AUTOMATIC_MODE_MANUAL))
  {
#ifdef FEATURE_DUAL_SIM
    if (reg_sim_read_fplmn_list(&forbidden_plmn_list, TRUE, (sys_modem_as_id_e_type)reg_sub_id ))
#else
    if (reg_sim_read_fplmn_list(&forbidden_plmn_list, TRUE) )
#endif
    {
      i = 0;
      while (i < reg_mode_available_plmn_list.length)
      {
        /*
        ** Remove PLMN-RAT combinations forbidden for both CS and PS services.
        */
        if ((reg_sim_find_plmn_in_list(reg_mode_available_plmn_list.info[i].plmn,
                                       &forbidden_plmn_list,
                                       &position)) ||
           (
#ifdef FEATURE_DUAL_SIM 
           (reg_sim_per_subs_int_voice_forbidden( reg_mode_available_plmn_list.info[i].plmn,(sys_modem_as_id_e_type)reg_sub_id )) || 
           (reg_sim_per_subs_domestic_voice_forbidden( reg_mode_available_plmn_list.info[i].plmn,(sys_modem_as_id_e_type)reg_sub_id )))
#else
           (reg_sim_int_voice_forbidden( reg_mode_available_plmn_list.info[i].plmn )) || 
           (reg_sim_domestic_voice_forbidden( reg_mode_available_plmn_list.info[i].plmn )))
#endif
	   )
        {
          for (j = i+1; j < reg_mode_available_plmn_list.length; j++)
          {
            reg_mode_available_plmn_list.info[j-1] =
              reg_mode_available_plmn_list.info[j];
          }
          reg_mode_available_plmn_list.length--;
        }
        /*
        ** Remove PLMN-RAT combinations forbidden for PS services if only PS
        ** service is requested.
        */
#ifdef FEATURE_LTE
        else if (
                ((reg_mode_get_volte_call_in_progress() == TRUE)&& 
			     (reg_eplmn_list_equivalent_plmn( reg_mode_available_plmn_list.info[i].plmn )== FALSE))
                )
        {
          for (j = i+1; j < reg_mode_available_plmn_list.length; j++)
          {
            reg_mode_available_plmn_list.info[j-1] =
              reg_mode_available_plmn_list.info[j];
          }
          reg_mode_available_plmn_list.length--;
        }
#endif
        else
        {
          i++;
        }
      }
    }

    /*
    ** Remove PLMN-RAT combinations forbidden for PS services if only PS
    ** service is requested.
    */
    i = 0;
 	while (i < reg_mode_available_plmn_list.length)
 	{
 	  
 	if ( (reg_mode_allowed_service_domain(reg_mode_available_plmn_list.info[i].plmn,
     												req_service_domain) == SYS_SRV_DOMAIN_NO_SRV))
 	  {
 		for (j = i+1; j < reg_mode_available_plmn_list.length; j++)
 		{
 		  reg_mode_available_plmn_list.info[j-1] =
 			reg_mode_available_plmn_list.info[j];
 		}
 		reg_mode_available_plmn_list.length--;
 	  }
 	  else
 	  {
 		i++;
 	  }
 	}

  }

  /*
  ** Prioritize the PLMN-RAT combinations.
  */
  reg_mode_prioritize_plmn_list
  (
    automatic_mode,

    rat_pri_list_info,

    &reg_mode_available_plmn_list
  );

  if(automatic_mode == REG_MODE_AUTOMATIC_MODE_ROAMING_PLMN)
  { 
    reg_mode_filter_china_hplmn(&reg_mode_available_plmn_list);
  }

  if(mm_managed_roaming_enabled)
  {
    if(automatic_mode != REG_MODE_AUTOMATIC_MODE_MANUAL)
    {
      reg_mode_filter_low_priority_plmn(&reg_mode_available_plmn_list, TRUE);
    }
  }

  /*
  ** Output the available PLMN list for debug.
  */
  MSG_HIGH_DS_1(REG_SUB, "=REG= Available PLMN list (length = %u)", reg_mode_available_plmn_list.length);

  reg_mode_output_detailed_plmn_list
  (
    &reg_mode_available_plmn_list
#ifdef FEATURE_FEMTO_CSG
    , FALSE
#endif 
  );

}


#if defined(FEATURE_EQUIVALENT_HPLMN)
boolean reg_mode_keep_ehplmn_to_available_plmn_list(void)
{
  uint32 i;
  boolean list_updated = FALSE;
  sys_detailed_plmn_list_s_type ehplmn_list;
  ehplmn_list.length = 0;

   if (!reg_mode_is_camped_plmn_is_higher_priority_ehplmn(reg_state_service_plmn_get())) 
   {
     for (i=0; i<reg_mode_available_plmn_list.length; i++)
     {
       if(reg_mode_available_plmn_list.info[i].list_category == SYS_DETAILED_PLMN_LIST_CATEGORY_HPLMN && !sys_plmn_match(reg_state_service_plmn_get(),reg_mode_available_plmn_list.info[i].plmn))
       {
         ehplmn_list.info[i] = reg_mode_available_plmn_list.info[i];
         ehplmn_list.length++;
         list_updated = TRUE;
       }
       if (sys_plmn_match(reg_state_service_plmn_get(),reg_mode_available_plmn_list.info[i].plmn))
       {
         break;
       }
     }
     if (ehplmn_list.length > 0)
     {
       reg_mode_available_plmn_list.length = ehplmn_list.length;
       for (i=0; i< ehplmn_list.length; i++)
       {
          reg_mode_available_plmn_list.info[i] = ehplmn_list.info[i];
       }
     }
     else
     {
       list_updated = FALSE;
     }
   }
  return list_updated;
}
#endif

/*==============================================================================

FUNCTION NAME

  reg_mode_update_available_plmn_list_manual

==============================================================================*/

void reg_mode_update_available_plmn_list_manual
(
  reg_mode_automatic_mode_e_type automatic_mode,

  sys_rat_pri_list_info_s_type   *rat_pri_list_info,

  sys_srv_domain_e_type          req_service_domain,
  const sys_detailed_plmn_list_s_type* available_plmn_list_p,
  sys_service_search_e_type   service_search
)
{
  reg_sim_plmn_list_s_type forbidden_plmn_list;
  boolean                  found_plmn_rat = FALSE;
  uint32                   position;
  uint32                   i;
  uint32                   j;
  sys_detailed_plmn_list_s_type*      manual_plmn_list=NULL;
  if(service_search == SYS_SERVICE_SEARCH_BACKGROUND_MANUAL ||
            service_search == SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC)
  {
    int32 file_size = 0;
    file_size=sizeof(uint32)+(available_plmn_list_p->length*sizeof(sys_detailed_plmn_list_info_s_type));

      reg_mode_backgnd_list = (sys_detailed_plmn_list_s_type *)modem_mem_calloc(1, (unsigned int)file_size, MODEM_MEM_CLIENT_NAS);

    manual_plmn_list = reg_mode_backgnd_list;
    reg_check_for_null_ptr((void*)manual_plmn_list );
    manual_plmn_list->length=0;
  }
  else
  {
    manual_plmn_list = &reg_mode_available_plmn_list_manual;
  }

  /*
  ** Add PLMN-RAT combinations to the available PLMN list that are not
  ** already contained in the available PLMN list.
  */
  for (i = 0; i < available_plmn_list_p->length; i++)
  {
    found_plmn_rat = FALSE;

    for (j = 0; j < manual_plmn_list->length; j++)
    {
      if ( (sys_plmn_match(manual_plmn_list->info[j].plmn,
                           available_plmn_list_p->info[i].plmn) ) &&
           (manual_plmn_list->info[j].rat ==
              available_plmn_list_p->info[i].rat) )
      {
        /*
        ** Update the information for the PLMN-RAT combination already
        ** contained in the available PLMN list.
        */
        manual_plmn_list->info[j] = available_plmn_list_p->info[i];

        found_plmn_rat = TRUE;
        break;
      }
    }

    /*
    ** Add the new PLMN-RAT combination to the available PLMN list.
    */
    if ( !found_plmn_rat )
    {
      if (manual_plmn_list->length < SYS_PLMN_LIST_MAX_LENGTH)
      {
        manual_plmn_list->info[manual_plmn_list->length] =
          available_plmn_list_p->info[i];

        manual_plmn_list->length++;
      }
    }
#ifdef FEATURE_LTE
    if ((service_search == SYS_SERVICE_SEARCH_BACKGROUND_MANUAL || service_search == SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC)
          && (reg_sim_plmn_backoff_forbidden(available_plmn_list_p->info[i].plmn,(sys_modem_as_id_e_type)reg_sub_id))) 
    {
       reg_mode_remove_plmn_from_available_background_list(available_plmn_list_p->info[i].plmn, SYS_RAT_LTE_RADIO_ACCESS);
    }
#endif
  }
  
  if(service_search == SYS_SERVICE_SEARCH_BACKGROUND_MANUAL ||
	    service_search == SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC)
  {
  	reg_mode_filter_background_plmn_list();
  }
  /*
  ** Remove any forbidden PLMN-RAT combinations from the available PLMN list.
  */
  if ((manual_plmn_list->length != 0) &&
      (automatic_mode != REG_MODE_AUTOMATIC_MODE_MANUAL))
  {
#ifdef FEATURE_DUAL_SIM
    if (reg_sim_read_fplmn_list(&forbidden_plmn_list, TRUE, (sys_modem_as_id_e_type)reg_sub_id ))
#else
    if (reg_sim_read_fplmn_list(&forbidden_plmn_list, TRUE) )
#endif
    {
      i = 0;
      while (i < manual_plmn_list->length)
      {
        /*
        ** Remove PLMN-RAT combinations forbidden for both CS and PS services.
        */
        if ( reg_sim_find_plmn_in_list(manual_plmn_list->info[i].plmn,
                                       &forbidden_plmn_list,
                                       &position) 
#ifdef FEATURE_LTE
             ||(reg_sim_plmn_rat_forbidden(manual_plmn_list->info[i].plmn,
                 manual_plmn_list->info[i].rat,(sys_modem_as_id_e_type)reg_sub_id))
#endif
           )
        {
          for (j = i+1; j < manual_plmn_list->length; j++)
          {
            manual_plmn_list->info[j-1] =
              manual_plmn_list->info[j];
          }
          manual_plmn_list->length--;
        }
        /*
        ** Remove PLMN-RAT combinations forbidden for PS services if only PS
        ** service is requested.
        */
        else if ( reg_mode_allowed_service_domain(manual_plmn_list->info[i].plmn,
                                                  req_service_domain) == SYS_SRV_DOMAIN_NO_SRV)
        {
          for (j = i+1; j < manual_plmn_list->length; j++)
          {
            manual_plmn_list->info[j-1] =
              manual_plmn_list->info[j];
          }
          manual_plmn_list->length--;
        }
        else
        {
          i++;
        }
      }
    }
  }

  /*
  ** Prioritize the PLMN-RAT combinations.
  */
  
  reg_mode_prioritize_plmn_list
  (
    automatic_mode,

    rat_pri_list_info,

    manual_plmn_list
  );

  if(mm_managed_roaming_enabled)
  {
    if(automatic_mode != REG_MODE_AUTOMATIC_MODE_MANUAL)
    {
      reg_mode_filter_low_priority_plmn(manual_plmn_list,FALSE);
    }
  }

  /*
  ** Output the available PLMN list for debug.
  */
  MSG_HIGH_DS_1(REG_SUB, "=REG= Available PLMN Manual list (length = %u)", manual_plmn_list->length);

  reg_mode_output_detailed_plmn_list
  (
    manual_plmn_list
#ifdef FEATURE_FEMTO_CSG
    , FALSE
#endif 
  );
}




#ifdef FEATURE_FEMTO_CSG

/*==============================================================================

FUNCTION NAME

  reg_mode_update_available_csg_list

==============================================================================*/
void reg_mode_update_available_csg_list
(
  const sys_rat_pri_list_info_s_type  *rat_pri_list_info,
  const sys_detailed_plmn_list_s_type* available_plmn_list_p
)
{

  boolean                  found_plmn_rat = FALSE;

  uint32                   i;
  uint32                   j;

  /*
  ** Add PLMN-RAT combinations to the available PLMN list that are not
  ** already contained in the available PLMN list.
  */
  reg_mode_available_plmn_list_manual.length = 0;

  for (i = 0; i < available_plmn_list_p->length; i++)
  {
    found_plmn_rat = FALSE;

    for (j = 0; j < reg_mode_available_plmn_list_manual.length; j++)
    {
      if ( (sys_plmn_match(reg_mode_available_plmn_list_manual.info[j].plmn,
                           available_plmn_list_p->info[i].plmn) ) &&
           (reg_mode_available_plmn_list_manual.info[j].rat ==
              available_plmn_list_p->info[i].rat) && 
         (reg_mode_available_plmn_list_manual.info[j].csg_info.csg_id == 
                  available_plmn_list_p->info[i].csg_info.csg_id) )
      {
        /*
        ** Update the information for the PLMN-RAT combination already
        ** contained in the available PLMN list.
        */
        reg_mode_available_plmn_list_manual.info[j] = available_plmn_list_p->info[i];

        found_plmn_rat = TRUE;
        break;
      }
    }

    /*
    ** Add the new PLMN-RAT combination to the available PLMN list.
    */
    if ( !found_plmn_rat )
    {
      if (reg_mode_available_plmn_list_manual.length < SYS_PLMN_LIST_MAX_LENGTH)
      {
        reg_mode_available_plmn_list_manual.info[reg_mode_available_plmn_list_manual.length] =
          available_plmn_list_p->info[i];

        reg_mode_available_plmn_list_manual.length++;
      }
    }
  }

  /*
  ** Output the available PLMN list for debug.
  */
  MSG_HIGH_DS_1(REG_SUB, "=REG= Available CSG Manual list (length = %u)", reg_mode_available_plmn_list_manual.length);
  reg_mode_output_detailed_plmn_list
  (
    &reg_mode_available_plmn_list_manual,
    TRUE
  );
}

/*==============================================================================

FUNCTION NAME

  reg_mode_start_automatic_csg_selection

==============================================================================*/

void reg_mode_start_automatic_csg_selection
(
  sys_network_selection_mode_e_type    network_selection_mode,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_plmn_id_s_type          plmn,
  sys_radio_access_tech_e_type  rat,
  sys_csg_id_type                          csg_id,
  sys_srv_domain_e_type                req_service_domain,
  const sys_rat_pri_list_info_s_type  *rat_pri_list_info
#ifdef FEATURE_LTE
  ,boolean sms_only,
   uint32  additional_info  
#endif
)
{
  reg_mode_p = &reg_mode_automatic_csg_selection;

  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;
  memscpy(&reg_mode_rat_pri_list, sizeof(sys_rat_pri_list_info_s_type), rat_pri_list_info, sizeof(sys_rat_pri_list_info_s_type));
#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif 

  reg_mode_p->start_automatic_csg_selection
  (
    network_selection_mode,
    cs_for_request,
    ps_for_request,
    plmn,
    rat,
    csg_id,
    req_service_domain,
    rat_pri_list_info
#ifdef FEATURE_LTE
    ,sms_only,
     additional_info
#endif 
  );

}

/*==============================================================================
FUNCTION NAME:  reg_mode_start_automatic_csg_selection_app
==============================================================================*/
void reg_mode_start_automatic_csg_selection_app
(
  sys_network_selection_mode_e_type    network_selection_mode,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain,
  const sys_rat_pri_list_info_s_type  *rat_pri_list_info
#ifdef FEATURE_LTE
  ,boolean                             sms_only,
   uint32                              additional_info  
#endif
)
{
  reg_mode_p = &reg_mode_automatic_csg_selection;

  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;

  memscpy(&reg_mode_rat_pri_list,
          sizeof(sys_rat_pri_list_info_s_type),
          rat_pri_list_info,
          sizeof(sys_rat_pri_list_info_s_type));
 
#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif 

  reg_mode_p->start_automatic_csg_selection_app
  (
    network_selection_mode,
    cs_for_request,
    ps_for_request,
    req_service_domain,
    rat_pri_list_info
#ifdef FEATURE_LTE
    ,sms_only,
    additional_info
#endif 
  );
}

/*==============================================================================

FUNCTION NAME

  reg_mode_unexpected_start_manual_csg_selection

==============================================================================*/

void reg_mode_unexpected_start_manual_csg_selection
(
  sys_network_selection_mode_e_type network_selection_mode,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_plmn_id_s_type                   plmn,
  sys_radio_access_tech_e_type  rat,
  sys_csg_id_type                          csg_id,
  sys_srv_domain_e_type                req_service_domain,
  const sys_rat_pri_list_info_s_type  *rat_pri_list_info
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info 
#endif 
)
{
  (void) plmn;
  (void) network_selection_mode;
  (void) cs_for_request;
  (void) ps_for_request;
  (void) req_service_domain;
 (void) rat_pri_list_info;
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
#endif
  (void) csg_id;

  MSG_ERROR_DS_1( REG_SUB, "=REG= reg_mode_unexpected_start_manual_csg_selection (%d)",
             reg_mode_number());
}

/*==============================================================================
FUNCTION NAME:  reg_mode_unexpected_start_manual_csg_selection_app
==============================================================================*/
void reg_mode_unexpected_start_manual_csg_selection_app
(
  sys_network_selection_mode_e_type    network_selection_mode,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_plmn_id_s_type                   plmn,
  sys_srv_domain_e_type                req_service_domain,
  const sys_rat_pri_list_info_s_type   *rat_pri_list_info
#ifdef FEATURE_LTE
  ,boolean                             sms_only,
  uint32                               additional_info 
#endif 
)
{
  (void) network_selection_mode;
  (void) cs_for_request;
  (void) ps_for_request;
  (void) plmn;
  (void) req_service_domain;
  (void) rat_pri_list_info;
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
#endif

  MSG_ERROR_DS_1( REG_SUB,
            "=REG= reg_mode_unexpected_start_manual_csg_selection_app (%d)",
             reg_mode_number());
}

/*==============================================================================

FUNCTION NAME

  reg_mode_unexpected_start_automatic_csg_selection

==============================================================================*/
void reg_mode_unexpected_start_automatic_csg_selection
(
  sys_network_selection_mode_e_type    network_selection_mode,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_plmn_id_s_type          plmn,
  sys_radio_access_tech_e_type  rat,
  sys_csg_id_type                          csg_id,
  sys_srv_domain_e_type                req_service_domain,
  const sys_rat_pri_list_info_s_type  *rat_pri_list_info
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info   
#endif 
)
{
  (void) plmn;
  (void) network_selection_mode;
  (void) cs_for_request;
  (void) ps_for_request;
  (void) req_service_domain;
 (void) rat_pri_list_info;
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
#endif
  (void) csg_id;

  MSG_ERROR_DS_1( REG_SUB, "=REG= reg_mode_unexpected_start_automatic_csg_selection (%d)",
             reg_mode_number());
}

/*==============================================================================
FUNCTION NAME:  reg_mode_unexpected_start_automatic_csg_selection_app
==============================================================================*/
void reg_mode_unexpected_start_automatic_csg_selection_app
(
  sys_network_selection_mode_e_type    network_selection_mode,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain,
  const sys_rat_pri_list_info_s_type   *rat_pri_list_info
#ifdef FEATURE_LTE
  ,boolean                             sms_only,
  uint32                               additional_info 
#endif 
)
{
  (void) network_selection_mode;
  (void) cs_for_request;
  (void) ps_for_request;
  (void) req_service_domain;
  (void) rat_pri_list_info;
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
#endif

  MSG_ERROR_DS_1( REG_SUB,
            "=REG= reg_mode_unexpected_start_automatic_csg_selection_app (%d)",
             reg_mode_number());
}

/*==============================================================================

FUNCTION NAME

  reg_mode_req_csg_id_set

==============================================================================*/
void reg_mode_req_csg_id_set(sys_csg_id_type   csg_id)
{
  reg_mode_req_csg_id = csg_id;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_req_csg_id_get

==============================================================================*/
sys_csg_id_type reg_mode_req_csg_id_get(void)
{
  return reg_mode_req_csg_id;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_start_manual_csg_selection

==============================================================================*/

void reg_mode_start_manual_csg_selection
(
  sys_network_selection_mode_e_type    network_selection_mode,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_plmn_id_s_type          plmn,
  sys_radio_access_tech_e_type  rat,
  sys_csg_id_type                          csg_id,
  sys_srv_domain_e_type                req_service_domain,
  const sys_rat_pri_list_info_s_type  *rat_pri_list_info
#ifdef FEATURE_LTE
  ,boolean sms_only,
   uint32  additional_info   
#endif
)
{
  reg_mode_p = &reg_mode_manual_csg_selection;

  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;
  memscpy(&reg_mode_rat_pri_list, sizeof(sys_rat_pri_list_info_s_type), rat_pri_list_info, sizeof(sys_rat_pri_list_info_s_type));
#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif 

  reg_mode_p->start_manual_csg_selection
  (
    network_selection_mode,
    cs_for_request,
    ps_for_request,
    plmn,
    rat,
    csg_id,
    req_service_domain,
    rat_pri_list_info
#ifdef FEATURE_LTE
    ,sms_only,
     additional_info 
#endif 
  );

}

/*==============================================================================
FUNCTION NAME:  reg_mode_start_manual_csg_selection_app
==============================================================================*/
void reg_mode_start_manual_csg_selection_app
(
  sys_network_selection_mode_e_type    network_selection_mode,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_plmn_id_s_type                   plmn,
  sys_srv_domain_e_type                req_service_domain,
  const sys_rat_pri_list_info_s_type  *rat_pri_list_info
#ifdef FEATURE_LTE
  ,boolean                             sms_only,
   uint32                              additional_info  
#endif
)
{
  reg_mode_p = &reg_mode_manual_csg_selection;

  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;

  memscpy(&reg_mode_rat_pri_list,
          sizeof(sys_rat_pri_list_info_s_type),
          rat_pri_list_info,
          sizeof(sys_rat_pri_list_info_s_type));
 
#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif 

  reg_mode_p->start_manual_csg_selection_app
  (
    network_selection_mode,
    cs_for_request,
    ps_for_request,
    plmn,
    req_service_domain,
    rat_pri_list_info
#ifdef FEATURE_LTE
    ,sms_only,
    additional_info
#endif 
  );
}

#endif /*FEATURE_FEMTO_CSG*/

/*==============================================================================

FUNCTION NAME

  reg_mode_unexpected_update_plmn_selection_mode

==============================================================================*/
void reg_mode_unexpected_update_plmn_selection_mode 
(
  reg_mode_automatic_mode_e_type  plmn_selection_mode
)
{
  (void)plmn_selection_mode;
};
/*==============================================================================

FUNCTION NAME

  reg_mode_clear_attempted_plmn_list

==============================================================================*/

void reg_mode_clear_attempted_plmn_list
(
  void
)
{
  reg_mode_attempted_plmn_list.length = 0;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_update_attempted_plmn_list

==============================================================================*/

void reg_mode_update_attempted_plmn_list
(
  sys_plmn_id_s_type                   plmn,
  sys_radio_access_tech_e_type         rat
)
{
  if (!sys_plmn_id_is_undefined(plmn))
  {
    if ( (rat == SYS_RAT_GSM_RADIO_ACCESS) ||
         (rat == SYS_RAT_UMTS_RADIO_ACCESS) ||
         (rat == SYS_RAT_TDS_RADIO_ACCESS)
#ifdef FEATURE_LTE
         || (rat == SYS_RAT_LTE_RADIO_ACCESS)
#endif
       )
    {
      if (!reg_mode_found_attempted_plmn_rat(plmn, rat))
      {
        reg_mode_attempted_plmn_list.info[reg_mode_attempted_plmn_list.length].plmn = plmn;
        reg_mode_attempted_plmn_list.info[reg_mode_attempted_plmn_list.length].rat  = rat;
        reg_mode_attempted_plmn_list.length++;
      }

      reg_mode_req_plmn_set(plmn);
      reg_mode_req_rat_set(rat);
    }
  }
}


/*==============================================================================

FUNCTION NAME

  reg_mode_next_available_plmn

==============================================================================*/

void reg_mode_next_available_plmn
(
  sys_plmn_id_s_type*                  plmn_p,
  sys_radio_access_tech_e_type*        rat_p
)
{
  uint32     i;

  sys_plmn_undefine_plmn_id(plmn_p);
  *rat_p = SYS_RAT_GSM_RADIO_ACCESS;

  for (i = 0; i < reg_mode_available_plmn_list.length; i++)
  {
    if (!reg_mode_found_attempted_plmn_rat(reg_mode_available_plmn_list.info[i].plmn,
                                           reg_mode_available_plmn_list.info[i].rat) )
    {
      *plmn_p = reg_mode_available_plmn_list.info[i].plmn;
      *rat_p  = reg_mode_available_plmn_list.info[i].rat;

      return;
    }
  }
}


/*==============================================================================

FUNCTION NAME

  reg_mode_next_higher_priority_plmn

==============================================================================*/

void reg_mode_next_higher_priority_plmn
(

  sys_plmn_id_s_type*                  plmn_p,
  sys_radio_access_tech_e_type*        rat_p
)
{
  int32 i;

  boolean plmn_id_is_undefined;
  boolean mnc_includes_pcs_digit;
  uint32  rplmn_mcc;
  uint32  rplmn_mnc;
  uint32 aplmn_mcc, aplmn_mnc;


  sys_plmn_id_s_type rplmn;

  sys_plmn_undefine_plmn_id(plmn_p);

  /*
  ** If the available PLMN list is empty then return immediately
  ** without waiting to read the RPLMN from the SIM.
  */
  if (reg_mode_available_plmn_list.length == 0)
  {
    return;
  }

  rplmn = reg_sim_read_rplmn(
             (reg_state_service_status_get() == SYS_SRV_STATUS_SRV)? 
                (reg_state_reg_service_domain_get()) : 
                (reg_mode_req_service_domain_get()));

  if((reg_state_service_status_get() == SYS_SRV_STATUS_LIMITED) ||
     (reg_state_service_status_get() == SYS_SRV_STATUS_LIMITED_REGIONAL) )
  {
    rplmn = reg_state_service_plmn_get();
  }

  sys_plmn_get_mcc_mnc
  (
    rplmn,
    &plmn_id_is_undefined,
    &mnc_includes_pcs_digit,
    &rplmn_mcc,
    &rplmn_mnc
  );

  for (i = 0; i < (int32)reg_mode_available_plmn_list.length; i++)
  {
    /*
    ** If the next available PLMN is not a preferred PLMN then the search for
    ** a higher priority PLMN is complete.
    */
    if (reg_mode_available_plmn_list.info[i].list_category == SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER)
    {
      return;
    }
    else if (!reg_mode_found_attempted_plmn_rat(reg_mode_available_plmn_list.info[i].plmn,
                                                reg_mode_available_plmn_list.info[i].rat) )
    {
   
      sys_plmn_get_mcc_mnc
      (
        reg_mode_available_plmn_list.info[i].plmn,
        &plmn_id_is_undefined,
        &mnc_includes_pcs_digit,
        &aplmn_mcc,
        &aplmn_mnc
      );

      if ( !sys_mcc_same_country(rplmn_mcc, aplmn_mcc) &&
          /* This feature is for customer specific filtering mechanism implementation*/
          (reg_mode_enhanced_hplmn_search_mcc_check(rplmn, aplmn_mcc) == FALSE)
#ifdef FEATURE_DISABLE_HPLMN_MCC_FILTERING
           && !sys_plmn_match(reg_sim_read_hplmn(), reg_mode_available_plmn_list.info[i].plmn) 
#endif
         )
      {
        if(mnc_includes_pcs_digit)
        {
          MSG_MED_DS_2(REG_SUB,"=REG= MCC is different (%03lu-%03lu)",aplmn_mcc,aplmn_mnc);
        }
        else
        {
          MSG_MED_DS_2(REG_SUB,"=REG= MCC is different (%03lu- %02lu)",aplmn_mcc,aplmn_mnc);
        }
        continue;
      }


      /*
      ** If the next available PLMN matches the RPLMN then the search for
      ** a higher priority PLMN is complete.
      */
      if ( sys_plmn_match(rplmn, reg_mode_available_plmn_list.info[i].plmn) )
      {
        return;
      }
      else
      {
        /*
        ** Account for equivalent PLMNs if the PLMN that provided the
        ** equivalent PLMN list matches the RPLMN.
        */
        if (sys_plmn_match(rplmn, sys_eplmn_list_rplmn()))
        {
          if ((sys_eplmn_list_equivalent_rplmn(reg_mode_available_plmn_list.info[i].plmn, rplmn_mcc)) &&
	          (reg_mode_is_camped_plmn_is_higher_priority_ehplmn(rplmn)))
          {
            return;
          }
        }

        *plmn_p = reg_mode_available_plmn_list.info[i].plmn;
        *rat_p  = reg_mode_available_plmn_list.info[i].rat;
         return;
      }
    }
  }
}


/*==============================================================================

FUNCTION NAME

  reg_mode_get_rat_order

DESCRIPTION

  Function reads the two bytes of access technology and returns an array of type sys_radio_access_tech_e_type
  with the supported rats.

RETURN VALUE

  uint8 Number of supported rats
  sys_radio_access_tech_e_type* array of supported rats.

==============================================================================*/

uint8 reg_mode_get_rat_order
(

  const sys_rat_pri_list_info_s_type  *rat_pri_list_info,

  sys_radio_access_tech_e_type *rat,
  reg_sim_plmn_w_act_s_type plmn_w_act,
  reg_mode_3gpp_capability_s_type *rat_list_order
)
{
  uint8 index = 0;
 
  reg_mode_3gpp_capability_s_type rat_3gpp_list;
  uint8                                  j;


  sys_radio_access_tech_e_type  not_found_rats[SYS_RAT_MAX];
  uint8 not_found_index = 0; 


  reg_mode_get_3gpp_rat_capabilty(&rat_3gpp_list, rat_pri_list_info
#ifdef FEATURE_SGLTE
  ,TRUE
#endif                                  
  );
   for (j = 0; j < rat_3gpp_list.num_items; j++)
   {
     switch(rat_3gpp_list.rat_list[j])
     {
       case SYS_RAT_GSM_RADIO_ACCESS:
         if ((plmn_w_act.act[1] & 0x80) ||
             ((plmn_w_act.act[0] == 0) && (plmn_w_act.act[1] == 0)))
         {
           rat[index++] = SYS_RAT_GSM_RADIO_ACCESS;
         }
         else
         {
           not_found_rats[not_found_index++] = SYS_RAT_GSM_RADIO_ACCESS;
         }
         break;
 
       case SYS_RAT_UMTS_RADIO_ACCESS:
         if ((plmn_w_act.act[0] & 0x80) ||
             ((plmn_w_act.act[0] == 0) && (plmn_w_act.act[1] == 0)))
         {
            rat[index++] = SYS_RAT_UMTS_RADIO_ACCESS;
         }
         else
         {
           not_found_rats[not_found_index++] = SYS_RAT_UMTS_RADIO_ACCESS;
         }                
         break;
        /* Currently, per CMCC requirement, the AcTs of UMTS and TDS are the same */
       case SYS_RAT_TDS_RADIO_ACCESS:
         if ((plmn_w_act.act[0] & 0x80) ||
             ((plmn_w_act.act[0] == 0) && (plmn_w_act.act[1] == 0)))
         {
            rat[index++] = SYS_RAT_TDS_RADIO_ACCESS;
         }
         else
         {
           not_found_rats[not_found_index++] = SYS_RAT_TDS_RADIO_ACCESS;
         }   
         break;
#ifdef FEATURE_LTE
       case SYS_RAT_LTE_RADIO_ACCESS:
         if ((plmn_w_act.act[0] & 0x40) ||
             ((plmn_w_act.act[0] == 0) && (plmn_w_act.act[1] == 0)))
         {
           rat[index++] = SYS_RAT_LTE_RADIO_ACCESS;
         }       
         else
         {
           not_found_rats[not_found_index++] = SYS_RAT_LTE_RADIO_ACCESS;
         }            
         break;
#endif /* FEATURE_LTE */
       default :
         break;
     }
   }

  if(rat_list_order != NULL)
  {
    rat_list_order->num_items = 0;
    for(j = 0; j < index; j++)
    {
      rat_list_order->rat_list[rat_list_order->num_items++] = rat[j];
    }
    for(j = 0; j < not_found_index; j++)
    {
      rat_list_order->rat_list[rat_list_order->num_items++] = not_found_rats[j];
    }
  }
  return index;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_higher_priority_network_available

==============================================================================*/

boolean reg_mode_higher_priority_network_available
(
  sys_radio_access_tech_e_type active_rat,
  sys_plmn_rat_list_s_type*    higher_priority_plmn_list_p,
  boolean lte_hplmn_scan,
  sys_rat_pri_list_info_s_type *rat_pri_list_info_local

)
{
  int32 i;
  uint8 k;

  reg_sim_plmn_w_act_s_type *preferred_plmn_list_p = NULL;
  mmgsdi_len_type preferred_plmn_list_length = 0;
  mmgsdi_len_type operator_preferred_plmn_list_length = 0;
  mmgsdi_len_type user_preferred_plmn_list_length = 0;
  mmgsdi_len_type total_pplmn_length = 0;
  boolean preferred_plmn_scan_done = FALSE;
  uint8 number_of_pplmn_supported_rats = 0;
  sys_radio_access_tech_e_type pplmn_supported_rat_order[SYS_RAT_MAX];
#ifdef FEATURE_EQUIVALENT_HPLMN
  reg_sim_plmn_list_s_type* ehplmn_list;
#endif


  sys_rat_pri_list_info_s_type   *rat_pri_list_info;
  reg_mode_3gpp_capability_s_type hplmn_rat_search_order;
  uint8 j;


  boolean plmn_id_is_undefined;
  boolean mnc_includes_pcs_digit;
  uint32  hplmn_mcc;
  uint32  hplmn_mnc;
  uint32  pplmn_mcc;
  uint32  pplmn_mnc;
  uint32  rplmn_mcc;
  uint32  rplmn_mnc;

  sys_plmn_id_s_type hplmn;
  sys_plmn_id_s_type rplmn;
  sys_plmn_id_s_type camped_plmn;  

  boolean higher_priority_network_available = FALSE;

  reg_sim_plmn_list_s_type* forbidden_plmn_list;
  uint32                   position;

#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
  boolean rat_flag[REG_MODE_MAX_3GPP_RAT] = {1};
#endif
  higher_priority_plmn_list_p->length = 0;


  camped_plmn=reg_state_service_plmn_get();


 if(
#ifdef FEATURE_DUAL_SIM 
   (reg_sim_per_subs_int_voice_forbidden( camped_plmn,(sys_modem_as_id_e_type)reg_sub_id ))
#else
   (reg_sim_int_voice_forbidden( camped_plmn ))
#endif
   )
 {
     MSG_HIGH_DS_0(REG_SUB, "=REG= Ignore HPLMN expiry actions as Voice Roaming is OFF");
     return FALSE;
 }

 if(lte_hplmn_scan && rat_pri_list_info_local != NULL)
 {
   memscpy(rat_pri_list_info_local, sizeof(sys_rat_pri_list_info_s_type), 
        reg_mode_rat_pri_list_get(), sizeof(sys_rat_pri_list_info_s_type));

  /* LTE only BPLMN with EHPLMN populated is performed for a new type 
  ** SYS_SERVICE_SEARCH_HPLMN_FREQ upon CFL HPLMN search timer expiry*/
  reg_mode_remove_rat_from_rat_pri_list(rat_pri_list_info_local, SYS_SYS_MODE_GSM);
  reg_mode_remove_rat_from_rat_pri_list(rat_pri_list_info_local, SYS_SYS_MODE_WCDMA);
  reg_mode_remove_rat_from_rat_pri_list(rat_pri_list_info_local, SYS_SYS_MODE_TDS);
 
  reg_mode_get_3gpp_rat_capabilty(&hplmn_rat_search_order, rat_pri_list_info_local
#ifdef FEATURE_SGLTE
  ,TRUE
#endif                                  
  );   
 }
 else
 {
   rat_pri_list_info = reg_mode_rat_pri_list_get();

   reg_mode_get_3gpp_rat_capabilty(&hplmn_rat_search_order, rat_pri_list_info
#ifdef FEATURE_SGLTE
  ,TRUE
#endif                                  
  );  
 }
  
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
  for (i = 0; i < hplmn_rat_search_order.num_items; i++)
  {
    rat_flag[hplmn_rat_search_order.rat_list[i]] = 0;
  }
#endif

  /* 
  ** Get rplmn based on the current registered service domain. 
  ** If domain is CS-PS or CS only or no service, CS rplmn will 
  ** be considered. If domain is PS only PS rplmn will be 
  ** considered.
  */
  rplmn = reg_sim_read_rplmn(reg_state_reg_service_domain_get());

  /*
  ** If the RPLMN cannot be read from the SIM/USIM then
  ** return immediately if MS is not in LIMITED Service.
  ** In case of LIMITED service and RPLMN is undefined then
  ** currently serving PLMN is used in place of RPLMN to
  ** find the priority PLMN LIST.
  */
  if((reg_state_service_status_get() != SYS_SRV_STATUS_LIMITED) &&
     (reg_state_service_status_get() != SYS_SRV_STATUS_LIMITED_REGIONAL) )
  {
    if (sys_plmn_id_is_undefined(rplmn))
    {
      return FALSE;
    }
  }
  else
  {
    rplmn = reg_state_service_plmn_get();
  }
#ifndef FEATURE_EQUIVALENT_HPLMN
  hplmn = reg_sim_read_hplmn();
#endif

  sys_plmn_get_mcc_mnc
  (
    rplmn,
    &plmn_id_is_undefined,
    &mnc_includes_pcs_digit,
    &rplmn_mcc,
    &rplmn_mnc
  );



  forbidden_plmn_list = (reg_sim_plmn_list_s_type *)modem_mem_calloc(1, sizeof(reg_sim_plmn_list_s_type), MODEM_MEM_CLIENT_NAS);
#ifdef FEATURE_EQUIVALENT_HPLMN
  ehplmn_list = (reg_sim_plmn_list_s_type *)modem_mem_calloc(1, sizeof(reg_sim_plmn_list_s_type), MODEM_MEM_CLIENT_NAS);
#endif
  if((forbidden_plmn_list == NULL)
#ifdef FEATURE_EQUIVALENT_HPLMN
      || (ehplmn_list == NULL)
#endif
    )
  {
    MSG_FATAL_DS(REG_SUB, "=REG= Error: Not able to Allocate Buffer", 0, 0, 0);
  }

#ifndef FEATURE_EQUIVALENT_HPLMN

  sys_plmn_get_mcc_mnc
  (
    hplmn,
    &plmn_id_is_undefined,
    &mnc_includes_pcs_digit,
    &hplmn_mcc,
    &hplmn_mnc
  );

  /*
  ** If the RPLMN matches the HPLMN or is equivalent to the HPLMMN then
  ** there is no need to search for the HPLMN so return immediately.
  */
  /*lint -e{539} Reviewed with the process group and recommended for suppression */
  if ( (sys_plmn_match(hplmn, rplmn)) ||
       (sys_plmn_match(rplmn, sys_eplmn_list_rplmn()) &&
        sys_eplmn_list_equivalent_rplmn(hplmn, rplmn_mcc)) )
  {
    return FALSE;
  }

#else
#if defined(FEATURE_EQUIVALENT_HPLMN)
   if(!reg_mode_is_camped_plmn_is_higher_priority_ehplmn(rplmn)) 
  {
    /* Reading EHPLMN list */
      reg_sim_ehplmn_list_get_ptr(ehplmn_list);  //read ehplmns only from SIM card
      reg_sim_read_hplmn_rat_search_order(&hplmn_rat_search_order);
      for(i=0; i< (int32)ehplmn_list->length; i++)
      {
        /* If rplmn matches ehplmn then return FALSE immediately 
        */
        if(sys_plmn_match(ehplmn_list->plmn[i], rplmn))
        {
           modem_mem_free(forbidden_plmn_list, MODEM_MEM_CLIENT_NAS);
#ifdef FEATURE_EQUIVALENT_HPLMN
           modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
           forbidden_plmn_list = NULL;
#ifdef FEATURE_EQUIVALENT_HPLMN
           ehplmn_list = NULL;
#endif
          return higher_priority_network_available;
        }
        /*
        ** If the RPLMN is in the same country as the EHPLMN and the EHPLMN
        ** is not an equivalent PLMN of RPLMN then the phone must search for
        ** the EHPLMN. If all EHPLMN are equivalent PLMN to RPLMN, then no
        ** need to start higher priority search
        */
        hplmn = ehplmn_list->plmn[i];
        sys_plmn_get_mcc_mnc
        (
            hplmn,
            &plmn_id_is_undefined,
            &mnc_includes_pcs_digit,
            &hplmn_mcc,
            &hplmn_mnc
         );
        /*
        ** If the RPLMN is in the same country as the HPLMN and the HPLMN
        ** is not an equivalent PLMN then the phone must search for the HPLMN.
        */
#ifndef FEATURE_DISABLE_HPLMN_MCC_FILTERING
        /* This feature is for customer specific filtering mechanism implementation*/
        if(reg_mode_enhanced_hplmn_search_mcc_check(rplmn, hplmn_mcc) == TRUE ||
           sys_mcc_same_country(hplmn_mcc, rplmn_mcc))
         {
#endif

#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
        for (j = 0; j < hplmn_rat_search_order.num_items && higher_priority_plmn_list_p->length != hplmn_rat_search_order.num_items; j++)
#else
        for (j = 0; j < hplmn_rat_search_order.num_items; j++)
#endif
        {
#if defined(FEATURE_LTE) && !defined(FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN)
      /*
      ** Check if its backoff PLMN
      */
      if (reg_sim_plmn_rat_forbidden(hplmn,hplmn_rat_search_order.rat_list[j],
          (sys_modem_as_id_e_type)reg_sub_id))
      {
        continue;
      }
#endif

#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
      if(rat_flag[hplmn_rat_search_order.rat_list[j]] == 0)
#endif
      {
         higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].plmn = hplmn;				
         higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].rat  = hplmn_rat_search_order.rat_list[j];
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
        rat_flag[hplmn_rat_search_order.rat_list[j]] = 1;
#if defined(FEATURE_LTE)
        if (reg_sim_plmn_rat_forbidden(hplmn,hplmn_rat_search_order.rat_list[j],
        (sys_modem_as_id_e_type)reg_sub_id))
       {
      sys_plmn_undefine_plmn_id(&(higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].plmn));
        }
#endif
#endif
        higher_priority_plmn_list_p->length++;
      }
        }

#ifdef FEATURE_DISABLE_HPLMN_MCC_FILTERING
        /* This feature has requirement that search for HPLMN only if a search is to be done as per spec*/
        if(reg_mode_enhanced_hplmn_search_mcc_check(rplmn, hplmn_mcc) == TRUE ||
       sys_mcc_same_country(hplmn_mcc, rplmn_mcc) &&
       higher_priority_plmn_list_p->length > 0)
       {
         higher_priority_network_available = TRUE;
       }
#else
        if(higher_priority_plmn_list_p->length > 0)
        {
      higher_priority_network_available = TRUE;  
        }
#endif
#ifndef FEATURE_DISABLE_HPLMN_MCC_FILTERING
       }
#endif
#ifdef FEATURE_EQUIVALENT_HPLMN
      }
#endif

#ifndef FEATURE_MODEM_HEAP
      gs_free(forbidden_plmn_list);
#ifdef FEATURE_EQUIVALENT_HPLMN
      gs_free(ehplmn_list);
#endif
#else
      modem_mem_free(forbidden_plmn_list, MODEM_MEM_CLIENT_NAS);
#ifdef FEATURE_EQUIVALENT_HPLMN
      modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
#endif
      forbidden_plmn_list = NULL;
#ifdef FEATURE_EQUIVALENT_HPLMN
      ehplmn_list = NULL;
#endif

    return higher_priority_network_available;
    
  }
  else
  {
#endif

   

  /* Reading EHPLMN list */
  reg_sim_ehplmn_list_get_ptr(ehplmn_list);

  reg_sim_read_hplmn_rat_search_order(&hplmn_rat_search_order);

  for(i=0; i< (int32)ehplmn_list->length; i++)
  {
    /* If rplmn matches ehplmn then return FALSE immediately 
    */
    if(sys_plmn_match(ehplmn_list->plmn[i], rplmn))
    {

      modem_mem_free(forbidden_plmn_list, MODEM_MEM_CLIENT_NAS);
#ifdef FEATURE_EQUIVALENT_HPLMN
      modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif

      forbidden_plmn_list = NULL;
#ifdef FEATURE_EQUIVALENT_HPLMN
      ehplmn_list = NULL;
#endif
      return FALSE;
    }
    
    /*
    ** If the RPLMN is in the same country as the EHPLMN and the EHPLMN
    ** is not an equivalent PLMN of RPLMN then the phone must search for
    ** the EHPLMN. If all EHPLMN are equivalent PLMN to RPLMN, then no
    ** need to start higher priority search
    */

    hplmn = ehplmn_list->plmn[i];
    sys_plmn_get_mcc_mnc
    (
      hplmn,
      &plmn_id_is_undefined,
      &mnc_includes_pcs_digit,
      &hplmn_mcc,
      &hplmn_mnc
    );

#endif
    /*
    ** If the RPLMN is in the same country as the HPLMN and the HPLMN
    ** is not an equivalent PLMN then the phone must search for the HPLMN.
    */
#ifndef FEATURE_DISABLE_HPLMN_MCC_FILTERING
    /* This feature is for customer specific filtering mechanism implementation*/
    if(reg_mode_enhanced_hplmn_search_mcc_check(rplmn, hplmn_mcc) == TRUE ||
        sys_mcc_same_country(hplmn_mcc, rplmn_mcc))
    {
#endif
      if(!(sys_plmn_match(rplmn, sys_eplmn_list_rplmn()) &&
           sys_eplmn_list_equivalent_rplmn(hplmn, rplmn_mcc)))
      {

#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
        for (j = 0; j < hplmn_rat_search_order.num_items && higher_priority_plmn_list_p->length != hplmn_rat_search_order.num_items; j++)
#else
        for (j = 0; j < hplmn_rat_search_order.num_items; j++)
#endif
        {

#if defined(FEATURE_LTE) && !defined(FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN)
          /*
          ** Check if its backoff PLMN
          */
          if (reg_sim_plmn_rat_forbidden(hplmn,hplmn_rat_search_order.rat_list[j],
                                                (sys_modem_as_id_e_type)reg_sub_id))
          {
            continue;
          }
#endif

#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
          if(rat_flag[hplmn_rat_search_order.rat_list[j]] == 0)
#endif
          {
            higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].plmn = hplmn;              
            higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].rat  = hplmn_rat_search_order.rat_list[j];
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
            rat_flag[hplmn_rat_search_order.rat_list[j]] = 1;
#if defined(FEATURE_LTE)
            if (reg_sim_plmn_rat_forbidden(hplmn,hplmn_rat_search_order.rat_list[j],
                                                (sys_modem_as_id_e_type)reg_sub_id))
            {
              sys_plmn_undefine_plmn_id(&(higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].plmn));
            }
#endif
#endif
            higher_priority_plmn_list_p->length++;
          }

        }



#ifdef FEATURE_DISABLE_HPLMN_MCC_FILTERING
        /* This feature has requirement that search for HPLMN only if a search is to be done as per spec*/
        if(reg_mode_enhanced_hplmn_search_mcc_check(rplmn, hplmn_mcc) == TRUE ||
           sys_mcc_same_country(hplmn_mcc, rplmn_mcc) &&
           higher_priority_plmn_list_p->length > 0)
        {
          higher_priority_network_available = TRUE;
        }
#else
        if(higher_priority_plmn_list_p->length > 0)
        {
          higher_priority_network_available = TRUE;  
        }
#endif
      }
#ifdef FEATURE_EQUIVALENT_HPLMN
      else
      {

        modem_mem_free(forbidden_plmn_list, MODEM_MEM_CLIENT_NAS);
        modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);

        ehplmn_list = NULL;
        forbidden_plmn_list = NULL;
        return higher_priority_network_available;
      }
#endif
#ifndef FEATURE_DISABLE_HPLMN_MCC_FILTERING
    }
#endif
#ifdef FEATURE_EQUIVALENT_HPLMN
  }
#endif

   /* LTE only BPLMN with EHPLMN populated is performed for a new type 
  ** SYS_SERVICE_SEARCH_HPLMN_FREQ upon CFL HPLMN search timer expiry so PPLMN will not be added*/
  if (lte_hplmn_scan == FALSE)
  {

    /*
    ** If the preferred PLMN list cannot be read from the SIM/USIM
    ** then return immediately without reading the RPLMN from the SIM.
    */
    preferred_plmn_list_p = reg_sim_read_preferred_plmn_list
                                       (
                                         &preferred_plmn_list_length,
                                         &operator_preferred_plmn_list_length,
                                         &user_preferred_plmn_list_length
                                       );

  if ( preferred_plmn_list_p == NULL)
  {

      modem_mem_free(forbidden_plmn_list, MODEM_MEM_CLIENT_NAS);
#ifdef FEATURE_EQUIVALENT_HPLMN
      modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif

#ifdef FEATURE_EQUIVALENT_HPLMN
     ehplmn_list = NULL;
#endif
    forbidden_plmn_list = NULL;
    return higher_priority_network_available;
  }

  if((operator_preferred_plmn_list_length + user_preferred_plmn_list_length) > 0)
  {
    total_pplmn_length = operator_preferred_plmn_list_length + user_preferred_plmn_list_length;
  }
  else
  {
    total_pplmn_length = preferred_plmn_list_length;
  }

#ifdef FEATURE_DUAL_SIM
  if (!reg_sim_read_fplmn_list(forbidden_plmn_list, TRUE, (sys_modem_as_id_e_type)reg_sub_id ))
#else
  if (!reg_sim_read_fplmn_list(forbidden_plmn_list, TRUE) )
#endif
  {
    forbidden_plmn_list->length = 0;
  }


  for (i = 0; i < (int32)total_pplmn_length && !preferred_plmn_scan_done; i++)
  {
    //SKIP the undefined plmn
    if(sys_plmn_id_is_undefined(preferred_plmn_list_p[i].plmn))
    {
      continue;
    }
    //Get all supported rats for this particular preferred plmn.
      if(lte_hplmn_scan && rat_pri_list_info_local != NULL)
      {
        number_of_pplmn_supported_rats = reg_mode_get_rat_order
                                                         (
                                                     
                                                           rat_pri_list_info_local,
                                                           pplmn_supported_rat_order,
                                                           preferred_plmn_list_p[i],
                                                           NULL
                                                         );
      }
	  else
	  {
	    number_of_pplmn_supported_rats = reg_mode_get_rat_order
                                                         (
                                                     
                                                           rat_pri_list_info,
                                                           pplmn_supported_rat_order,
                                                           preferred_plmn_list_p[i],
                                                           NULL
                                                         );
	  }
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
    for(k = 0; k < number_of_pplmn_supported_rats && !preferred_plmn_scan_done  && higher_priority_plmn_list_p->length != hplmn_rat_search_order.num_items; k++)
#else
    for(k = 0; k < number_of_pplmn_supported_rats && !preferred_plmn_scan_done; k++)
#endif
    {
      /*
      ** If the next preferred PLMN matches the RPLMN then the search for
      ** a higher priority PLMN is complete.
      */
      if ( sys_plmn_match(rplmn, preferred_plmn_list_p[i].plmn))
      {
        preferred_plmn_scan_done = TRUE;
        break;
      }
      else
      {
        /*
        ** Account for equivalent PLMNs if the PLMN that provided the
        ** equivalent PLMN list matches the RPLMN.
        */
        if (sys_plmn_match(rplmn, sys_eplmn_list_rplmn()))
        {
          if (sys_eplmn_list_equivalent_rplmn(preferred_plmn_list_p[i].plmn, rplmn_mcc))
          {
            preferred_plmn_scan_done = TRUE;
            break;
          }
        }


        /*
        ** Check if PLMN is forbidden
        */
        if ( reg_sim_find_plmn_in_list(preferred_plmn_list_p[i].plmn,
                                       forbidden_plmn_list,
                                       &position) || 
             (((pplmn_supported_rat_order[k] == SYS_RAT_GSM_RADIO_ACCESS) ||
                (pplmn_supported_rat_order[k] == SYS_RAT_UMTS_RADIO_ACCESS)) &&
              (reg_mode_req_service_domain_get() == SYS_SRV_DOMAIN_PS_ONLY) &&
#ifdef FEATURE_DUAL_SIM
              reg_sim_plmn_gprs_forbidden(preferred_plmn_list_p[i].plmn, (sys_modem_as_id_e_type)reg_sub_id)
#else
              reg_sim_plmn_gprs_forbidden(preferred_plmn_list_p[i].plmn)
#endif
               )
#ifdef FEATURE_LTE
             ||(reg_sim_plmn_rat_forbidden(preferred_plmn_list_p[i].plmn,
                               pplmn_supported_rat_order[k],(sys_modem_as_id_e_type)reg_sub_id))
#endif
           )
        {
          continue;
        }


        /*
        ** The preferred PLMN does not match the RPLMN or an equivalent
        ** PLMN so check if it is in the same country as the RPLMN.
        */
        sys_plmn_get_mcc_mnc
        (
          preferred_plmn_list_p[i].plmn,
          &plmn_id_is_undefined,
          &mnc_includes_pcs_digit,
          &pplmn_mcc,
          &pplmn_mnc
        );
  
       if(reg_mode_enhanced_hplmn_search_mcc_check(rplmn, pplmn_mcc) == TRUE ||
         sys_mcc_same_country(pplmn_mcc, rplmn_mcc))
        {
          higher_priority_network_available = TRUE;
  

          if (higher_priority_plmn_list_p->length < SYS_PRIORITY_PLMN_LIST_MAX_LENGTH)
          {
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
            if(rat_flag[pplmn_supported_rat_order[k]] == 0)
#endif
            {
              higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].plmn = preferred_plmn_list_p[i].plmn;
              higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].rat  = pplmn_supported_rat_order[k];
              higher_priority_plmn_list_p->length++;
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
              rat_flag[pplmn_supported_rat_order[k]] = 1;
#endif
            }

          }
          else
          {
            MSG_HIGH_DS_2(REG_SUB, "=REG= Preferred PLMNs - %d in SIM exceeded MAX - %d, rest not included", 
                                       total_pplmn_length, i);
            preferred_plmn_scan_done = TRUE;
            break;
          }
        }
      }
    }
  }
  /* 
  ** The memory for "preferred_plmn_list_p" is allocated in function
  ** reg_sim_read_preferred_plmn_list. So freeing this memory as the
  ** processing of this preferred list is done.
  */
  
    modem_mem_free( preferred_plmn_list_p , MODEM_MEM_CLIENT_NAS);
  }
  modem_mem_free(forbidden_plmn_list, MODEM_MEM_CLIENT_NAS);
#ifdef FEATURE_EQUIVALENT_HPLMN
  modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
  preferred_plmn_list_p = NULL;
  forbidden_plmn_list = NULL;
#ifdef FEATURE_EQUIVALENT_HPLMN
  ehplmn_list = NULL;
#endif  
  
  return higher_priority_network_available;
#if defined(FEATURE_EQUIVALENT_HPLMN) && defined(FEATURE_ENHANCED_NW_SELECTION)
  }
#endif
}

/*==============================================================================

FUNCTION NAME

  reg_mode_hplmn_available

==============================================================================*/

boolean reg_mode_hplmn_available
(
#if defined(FEATURE_GTOW_BACKGROUND_PLMN_SEARCH) || defined (FEATURE_RAT_PRIORITY_LIST)
  sys_radio_access_tech_e_type active_rat,
  sys_plmn_rat_list_s_type*    higher_priority_plmn_list_p
#else
  void
#endif
)
{
#if defined (FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN) || defined (FEATURE_EQUIVALENT_HPLMN)
  int32 i;
#endif




#ifdef FEATURE_EQUIVALENT_HPLMN
  reg_sim_plmn_list_s_type* ehplmn_list;
#endif

#ifndef FEATURE_RAT_PRIORITY_LIST
  sys_mode_pref_e_type req_mode_pref;
#if defined(FEATURE_GTOW_BACKGROUND_PLMN_SEARCH)
  sys_mode_pref_e_type hplmn_mode_pref;
#endif
#else
  sys_rat_pri_list_info_s_type   *rat_pri_list_info;
  reg_mode_3gpp_capability_s_type hplmn_rat_search_order;
  uint8 j;
#endif /*FEATURE_RAT_PRIORITY_LIST*/


  boolean plmn_id_is_undefined;
  boolean mnc_includes_pcs_digit;
  uint32  hplmn_mcc;
  uint32  hplmn_mnc;

  uint32  rplmn_mcc;
  uint32  rplmn_mnc;

  sys_plmn_id_s_type hplmn;
  sys_plmn_id_s_type rplmn;
  sys_plmn_id_s_type camped_plmn;  

  boolean higher_priority_network_available = FALSE;

  reg_sim_plmn_list_s_type* forbidden_plmn_list;


#if defined(FEATURE_GTOW_BACKGROUND_PLMN_SEARCH) || defined (FEATURE_RAT_PRIORITY_LIST)
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
  boolean rat_flag[REG_MODE_MAX_3GPP_RAT] = {1};
#endif
  higher_priority_plmn_list_p->length = 0;
#endif

  camped_plmn=reg_state_service_plmn_get();


 if(
#ifdef FEATURE_DUAL_SIM 
   (reg_sim_per_subs_int_voice_forbidden( camped_plmn,(sys_modem_as_id_e_type)reg_sub_id ))
#else
   (reg_sim_int_voice_forbidden( camped_plmn ))
#endif
   )
 {
     MSG_HIGH_DS_0(REG_SUB, "=REG= Ignore HPLMN expiry actions as Voice Roaming is OFF");
     return FALSE;
 }

#ifndef FEATURE_RAT_PRIORITY_LIST
  req_mode_pref = reg_mode_req_mode_pref_get();
#else
  rat_pri_list_info = reg_mode_rat_pri_list_get();
  reg_mode_get_3gpp_rat_capabilty(&hplmn_rat_search_order, rat_pri_list_info
#ifdef FEATURE_SGLTE
  ,TRUE
#endif                                  
  );                                  
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
  for (i = 0; i < hplmn_rat_search_order.num_items; i++)
  {
    rat_flag[hplmn_rat_search_order.rat_list[i]] = 0;
  }
#endif
#endif
  /* 
  ** Get rplmn based on the current registered service domain. 
  ** If domain is CS-PS or CS only or no service, CS rplmn will 
  ** be considered. If domain is PS only PS rplmn will be 
  ** considered.
  */
  rplmn = reg_sim_read_rplmn(reg_state_reg_service_domain_get());

  /*
  ** If the RPLMN cannot be read from the SIM/USIM then
  ** return immediately if MS is not in LIMITED Service.
  ** In case of LIMITED service and RPLMN is undefined then
  ** currently serving PLMN is used in place of RPLMN to
  ** find the priority PLMN LIST.
  */
  if((reg_state_service_status_get() != SYS_SRV_STATUS_LIMITED) &&
     (reg_state_service_status_get() != SYS_SRV_STATUS_LIMITED_REGIONAL) )
  {
    if (sys_plmn_id_is_undefined(rplmn))
    {
      return FALSE;
    }
  }
  else
  {
    rplmn = reg_state_service_plmn_get();
  }
#ifndef FEATURE_EQUIVALENT_HPLMN
  hplmn = reg_sim_read_hplmn();
#endif

  sys_plmn_get_mcc_mnc
  (
    rplmn,
    &plmn_id_is_undefined,
    &mnc_includes_pcs_digit,
    &rplmn_mcc,
    &rplmn_mnc
  );


#ifdef FEATURE_MODEM_HEAP
  forbidden_plmn_list = (reg_sim_plmn_list_s_type *)modem_mem_calloc(1, sizeof(reg_sim_plmn_list_s_type), MODEM_MEM_CLIENT_NAS);
#ifdef FEATURE_EQUIVALENT_HPLMN
  ehplmn_list = (reg_sim_plmn_list_s_type *)modem_mem_calloc(1, sizeof(reg_sim_plmn_list_s_type), MODEM_MEM_CLIENT_NAS);
#endif
#else
  forbidden_plmn_list = (reg_sim_plmn_list_s_type *)gs_alloc(sizeof(reg_sim_plmn_list_s_type));
#ifdef FEATURE_EQUIVALENT_HPLMN
  ehplmn_list = (reg_sim_plmn_list_s_type *)gs_alloc(sizeof(reg_sim_plmn_list_s_type));;
#endif
#endif
  if((forbidden_plmn_list == NULL)
#ifdef FEATURE_EQUIVALENT_HPLMN
      || (ehplmn_list == NULL)
#endif
    )
  {
    MSG_FATAL_DS(REG_SUB, "=REG= Error: Not able to Allocate Buffer", 0, 0, 0);
  }

#ifndef FEATURE_EQUIVALENT_HPLMN

  sys_plmn_get_mcc_mnc
  (
    hplmn,
    &plmn_id_is_undefined,
    &mnc_includes_pcs_digit,
    &hplmn_mcc,
    &hplmn_mnc
  );

  /*
  ** If the RPLMN matches the HPLMN or is equivalent to the HPLMMN then
  ** there is no need to search for the HPLMN so return immediately.
  */
  /*lint -e{539} Reviewed with the process group and recommended for suppression */
  if ( (sys_plmn_match(hplmn, rplmn)) ||
       (sys_plmn_match(rplmn, sys_eplmn_list_rplmn()) &&
        sys_eplmn_list_equivalent_rplmn(hplmn, rplmn_mcc)) )
  {
    return FALSE;
  }

#else
  /* Reading EHPLMN list */
  reg_sim_ehplmn_list_get_ptr(ehplmn_list);
#ifdef FEATURE_RAT_PRIORITY_LIST
  reg_sim_read_hplmn_rat_search_order(&hplmn_rat_search_order);
#endif
  for(i=0; i< (int32)ehplmn_list->length; i++)
  {
    /* If rplmn matches ehplmn then return FALSE immediately 
    */
    if(sys_plmn_match(ehplmn_list->plmn[i], rplmn))
    {
#ifndef FEATURE_MODEM_HEAP
      gs_free(forbidden_plmn_list);
#ifdef FEATURE_EQUIVALENT_HPLMN
      gs_free(ehplmn_list);
#endif
#else
      modem_mem_free(forbidden_plmn_list, MODEM_MEM_CLIENT_NAS);
#ifdef FEATURE_EQUIVALENT_HPLMN
      modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
#endif
      forbidden_plmn_list = NULL;
#ifdef FEATURE_EQUIVALENT_HPLMN
      ehplmn_list = NULL;
#endif
      return FALSE;
    }
    
    /*
    ** If the RPLMN is in the same country as the EHPLMN and the EHPLMN
    ** is not an equivalent PLMN of RPLMN then the phone must search for
    ** the EHPLMN. If all EHPLMN are equivalent PLMN to RPLMN, then no
    ** need to start higher priority search
    */

    hplmn = ehplmn_list->plmn[i];
    sys_plmn_get_mcc_mnc
    (
      hplmn,
      &plmn_id_is_undefined,
      &mnc_includes_pcs_digit,
      &hplmn_mcc,
      &hplmn_mnc
    );

#endif
    /*
    ** If the RPLMN is in the same country as the HPLMN and the HPLMN
    ** is not an equivalent PLMN then the phone must search for the HPLMN.
    */
#ifndef FEATURE_DISABLE_HPLMN_MCC_FILTERING
    /* This feature is for customer specific filtering mechanism implementation*/
    if(reg_mode_enhanced_hplmn_search_mcc_check(rplmn, hplmn_mcc) == TRUE ||
        sys_mcc_same_country(hplmn_mcc, rplmn_mcc))
    {
#endif
      if(!(sys_plmn_match(rplmn, sys_eplmn_list_rplmn()) &&
           sys_eplmn_list_equivalent_rplmn(hplmn, rplmn_mcc)))
      {
#if defined(FEATURE_GTOW_BACKGROUND_PLMN_SEARCH) && !defined(FEATURE_RAT_PRIORITY_LIST)
        hplmn_mode_pref = reg_sim_read_hplmn_rat_search_order(req_mode_pref);

        if ( (hplmn_mode_pref == SYS_MODE_PREF_GSM_RADIO_ACCESS_ONLY) ||
             (hplmn_mode_pref == SYS_MODE_PREF_GSM_RADIO_ACCESS_PREFERRED))
        {
          higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].plmn = hplmn;
          higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].rat  = SYS_RAT_GSM_RADIO_ACCESS;
          higher_priority_plmn_list_p->length++;
        }

        if ( (hplmn_mode_pref == SYS_MODE_PREF_UMTS_RADIO_ACCESS_ONLY) ||
#ifdef FEATURE_UMTS_1X_HANDOVER_UMTSMSM
             #error code not present
#endif
             (hplmn_mode_pref == SYS_MODE_PREF_UMTS_RADIO_ACCESS_PREFERRED) ||
             (hplmn_mode_pref == SYS_MODE_PREF_GSM_RADIO_ACCESS_PREFERRED) )
        {
          higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].plmn = hplmn;
          higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].rat  = SYS_RAT_UMTS_RADIO_ACCESS;
          higher_priority_plmn_list_p->length++;
        }

        if ( (hplmn_mode_pref == SYS_MODE_PREF_UMTS_RADIO_ACCESS_PREFERRED) )
        {
          higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].plmn = hplmn;
          higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].rat  = SYS_RAT_GSM_RADIO_ACCESS;
          higher_priority_plmn_list_p->length++;
        }
#endif
#ifdef FEATURE_RAT_PRIORITY_LIST
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
        for (j = 0; j < hplmn_rat_search_order.num_items && higher_priority_plmn_list_p->length != hplmn_rat_search_order.num_items; j++)
#else
        for (j = 0; j < hplmn_rat_search_order.num_items; j++)
#endif
        {

#if defined(FEATURE_LTE) && !defined(FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN)
          /*
          ** Check if its backoff PLMN
          */
          if (reg_sim_plmn_rat_forbidden(hplmn,hplmn_rat_search_order.rat_list[j],
                                                (sys_modem_as_id_e_type)reg_sub_id))
          {
            continue;
          }
#endif

#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
          if(rat_flag[hplmn_rat_search_order.rat_list[j]] == 0)
#endif
          {
            higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].plmn = hplmn;              
            higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].rat  = hplmn_rat_search_order.rat_list[j];

#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
            rat_flag[hplmn_rat_search_order.rat_list[j]] = 1;
#if defined(FEATURE_LTE)
            if (reg_sim_plmn_rat_forbidden(hplmn,hplmn_rat_search_order.rat_list[j],
                                                (sys_modem_as_id_e_type)reg_sub_id))
            {
              sys_plmn_undefine_plmn_id(&(higher_priority_plmn_list_p->info[higher_priority_plmn_list_p->length].plmn));
            }
#endif
#endif
            higher_priority_plmn_list_p->length++;
          }

        }
#endif


#ifdef FEATURE_DISABLE_HPLMN_MCC_FILTERING
        /* This feature has requirement that search for HPLMN only if a search is to be done as per spec*/
        if(reg_mode_enhanced_hplmn_search_mcc_check(rplmn, hplmn_mcc) == TRUE ||
           sys_mcc_same_country(hplmn_mcc, rplmn_mcc) &&
           higher_priority_plmn_list_p->length > 0)
        {
          higher_priority_network_available = TRUE;
        }
#else
        if(higher_priority_plmn_list_p->length > 0)
        {
          higher_priority_network_available = TRUE;  
        }
#endif
      }
#ifdef FEATURE_EQUIVALENT_HPLMN
      else
      {
#ifndef FEATURE_MODEM_HEAP
        gs_free(forbidden_plmn_list);
        gs_free(ehplmn_list);
#else
        modem_mem_free(forbidden_plmn_list, MODEM_MEM_CLIENT_NAS);
        modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
        ehplmn_list = NULL;
        forbidden_plmn_list = NULL;
        return higher_priority_network_available;
      }
#endif
#ifndef FEATURE_DISABLE_HPLMN_MCC_FILTERING
    }
#endif
#ifdef FEATURE_EQUIVALENT_HPLMN
  }
#endif



#ifdef FEATURE_MODEM_HEAP
 
  modem_mem_free(forbidden_plmn_list, MODEM_MEM_CLIENT_NAS);
#ifdef FEATURE_EQUIVALENT_HPLMN
  modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
#else
  gs_free(forbidden_plmn_list);
#ifdef FEATURE_EQUIVALENT_HPLMN
  gs_free(ehplmn_list);
#endif
#endif
  forbidden_plmn_list = NULL;
#ifdef FEATURE_EQUIVALENT_HPLMN
  ehplmn_list = NULL;
#endif  
  
  return higher_priority_network_available;
}


#if defined(FEATURE_EQUIVALENT_HPLMN)
boolean reg_mode_is_camped_plmn_is_higher_priority_ehplmn(sys_plmn_id_s_type plmn_id) 
{
  boolean result = TRUE;
  boolean plmn_id_is_undefined;
  uint32  home_mcc;
  uint32  home_mnc;
  boolean home_mnc_includes_pcs_digit;
  reg_sim_plmn_list_s_type* ehplmn_list=NULL;

  ehplmn_list = (reg_sim_plmn_list_s_type *)modem_mem_calloc(1, sizeof(reg_sim_plmn_list_s_type), MODEM_MEM_CLIENT_NAS);
  reg_check_for_null_ptr((void*)ehplmn_list);


  if ((reg_state_service_status_get() == SYS_SRV_STATUS_SRV)|| 
      (reg_state_service_status_get() == SYS_SRV_STATUS_LIMITED) ||
      (reg_state_service_status_get() == SYS_SRV_STATUS_LIMITED_REGIONAL))
  {
    sys_plmn_get_mcc_mnc
    (
      reg_sim_read_hplmn (),
      &plmn_id_is_undefined,
      &home_mnc_includes_pcs_digit,
      &home_mcc,
      &home_mnc
    );

  MSG_HIGH_DS_3(REG_SUB, "=REG= Home MCC = %d Home MNC = %d CFL NV = %d",home_mcc, home_mnc,reg_nv_check_cfl_search_enabled());
  

    if ((reg_state_roaming_ind_get() == SYS_ROAM_STATUS_OFF) &&
        (reg_nv_check_cfl_search_enabled() 
#ifdef FEATURE_ENHANCED_NW_SELECTION 
          || (reg_sim_read_ens_flag() &&
              ((home_mcc == 334 && home_mnc == 50)|| (home_mcc == 334 && home_mnc == 90)))
#endif
        ))
     {
         reg_sim_read_ehplmn_from_sim(ehplmn_list);
         if (ehplmn_list->length > 1 && !sys_plmn_match (plmn_id,ehplmn_list->plmn[0]))
         {
           result = FALSE;
         }
       }
   }

#ifndef FEATURE_MODEM_HEAP
#ifdef FEATURE_EQUIVALENT_HPLMN
      gs_free(ehplmn_list);
#endif
#else
#ifdef FEATURE_EQUIVALENT_HPLMN
      modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
#endif
#ifdef FEATURE_EQUIVALENT_HPLMN
      ehplmn_list = NULL;
#endif

   MSG_HIGH_DS_6(REG_SUB, "=REG= PLMN %x %x %x is high priority result = %d PLMN service status = %d reg_sim_read_ens_flag() =%d",
                  plmn_id.identity[0], plmn_id.identity[1], plmn_id.identity[2], result, reg_state_service_status_get(), reg_sim_read_ens_flag());

  return result;
}

boolean reg_mode_is_rlf_plmn_is_higher_priority_ehplmn(void) 
{
  boolean result = TRUE;
  boolean plmn_id_is_undefined;
  uint32  home_mcc;
  uint32  home_mnc;
  boolean home_mnc_includes_pcs_digit;
  reg_sim_plmn_list_s_type* ehplmn_list=NULL;
  sys_plmn_id_s_type plmn_id = reg_state_rlf_plmn_get();

  ehplmn_list = (reg_sim_plmn_list_s_type *)modem_mem_calloc(1, sizeof(reg_sim_plmn_list_s_type), MODEM_MEM_CLIENT_NAS);
  reg_check_for_null_ptr((void*)ehplmn_list);

  if (reg_state_service_status_get() == SYS_SRV_STATUS_NO_SRV)
  {
    sys_plmn_get_mcc_mnc
    (
      reg_sim_read_hplmn (),
      &plmn_id_is_undefined,
      &home_mnc_includes_pcs_digit,
      &home_mcc,
      &home_mnc
    );

  MSG_HIGH_DS_3(REG_SUB, "=REG= Home MCC = %d Home MNC = %d CFL NV = %d",home_mcc, home_mnc,reg_nv_check_cfl_search_enabled());
  

    if ((reg_nv_hplmn_cfl_search_timer_get() != 0) &&
         (reg_state_roaming_ind_get() == SYS_ROAM_STATUS_OFF) &&
         (reg_state_rlf_rat_get() == SYS_RAT_LTE_RADIO_ACCESS) &&
         ((reg_mode_network_selection_mode_get() == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) ||
          (reg_mode_network_selection_mode_get() == SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION)) &&
         (reg_sim_check_mcc_same_country(plmn_id) == TRUE))
     {
         reg_sim_read_ehplmn_from_sim(ehplmn_list);
         if (ehplmn_list->length > 1 && !sys_plmn_match (plmn_id,ehplmn_list->plmn[0]))
         {
           result = FALSE;
         }
       }
   }

#ifndef FEATURE_MODEM_HEAP
#ifdef FEATURE_EQUIVALENT_HPLMN
      gs_free(ehplmn_list);
#endif
#else
#ifdef FEATURE_EQUIVALENT_HPLMN
      modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
#endif
#ifdef FEATURE_EQUIVALENT_HPLMN
      ehplmn_list = NULL;
#endif

   MSG_HIGH_DS_5(REG_SUB, "=REG= PLMN %x %x %x is high priority result = %d PLMN service status = %d ",
                  plmn_id.identity[0], plmn_id.identity[1], plmn_id.identity[2], result, reg_state_service_status_get());

  return result;
}

boolean reg_mode_is_avail_plmn_list_contains_higher_priority_ehplmn(const sys_detailed_plmn_list_s_type* available_plmn_list_p) 
{
  boolean result = FALSE;
  boolean plmn_id_is_undefined;
  uint32 i = 0,j = 0;
  uint32  home_mcc;
  uint32  home_mnc;
  boolean home_mnc_includes_pcs_digit;
  reg_sim_plmn_list_s_type* ehplmn_list=NULL;

  ehplmn_list = (reg_sim_plmn_list_s_type *)modem_mem_calloc(1, sizeof(reg_sim_plmn_list_s_type), MODEM_MEM_CLIENT_NAS);
  reg_check_for_null_ptr((void*)ehplmn_list);

  if ((reg_state_service_status_get() == SYS_SRV_STATUS_NO_SRV) &&
       ((reg_mode_network_selection_mode_get() == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) ||
        (reg_mode_network_selection_mode_get() == SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION)) &&
       ((reg_mode_get_plmn_selection_mode() == REG_MODE_AUTOMATIC_MODE_SUITABLE_SEARCH) ||
        (sys_plmn_id_is_undefined(reg_state_rlf_plmn_get()) == FALSE)))
  {
    reg_mode_set_hplmn_to_be_selected(FALSE);
    sys_plmn_get_mcc_mnc
    (
      reg_sim_read_hplmn (),
      &plmn_id_is_undefined,
      &home_mnc_includes_pcs_digit,
      &home_mcc,
      &home_mnc
    );

    MSG_HIGH_DS_3(REG_SUB, "=REG= Home MCC = %d Home MNC = %d CFL NV = %d",home_mcc, home_mnc,reg_nv_hplmn_cfl_search_timer_get());

    if ((reg_nv_hplmn_cfl_search_timer_get() != 0) &&
         (reg_state_roaming_ind_get() == SYS_ROAM_STATUS_OFF) &&
         (reg_state_rlf_rat_get() == SYS_RAT_LTE_RADIO_ACCESS) &&
         (reg_sim_check_mcc_same_country(reg_state_rlf_plmn_get()) == TRUE))
     {
         reg_sim_read_ehplmn_from_sim(ehplmn_list);

         for (i = 0; i < available_plmn_list_p->length; i++)
         {
           for (j = 0; j < ehplmn_list->length; j++)
           {
             if (sys_plmn_match(reg_state_rlf_plmn_get(),ehplmn_list->plmn[j]))
             {
               break;
             }
             if (sys_plmn_match (available_plmn_list_p->info[i].plmn, ehplmn_list->plmn[j]) &&
                 (available_plmn_list_p->info[i].rat == SYS_RAT_LTE_RADIO_ACCESS))
             {
               result = TRUE;
               break;
             }
           }
         }
         MSG_HIGH_DS_5(REG_SUB, "=REG= PLMN %x %x %x is high priority result = %d PLMN service status = %d ",
                         ehplmn_list->plmn[0].identity[0], ehplmn_list->plmn[0].identity[1], ehplmn_list->plmn[0].identity[2], result, reg_state_service_status_get());
     }
     
   }

#ifndef FEATURE_MODEM_HEAP
#ifdef FEATURE_EQUIVALENT_HPLMN
      gs_free(ehplmn_list);
#endif
#else
#ifdef FEATURE_EQUIVALENT_HPLMN
      modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif
#endif
#ifdef FEATURE_EQUIVALENT_HPLMN
      ehplmn_list = NULL;
#endif

  return result;
}

#endif

/*==============================================================================

FUNCTION NAME

  reg_mode_prioritize_plmn_list

==============================================================================*/

void reg_mode_prioritize_plmn_list
(
  reg_mode_automatic_mode_e_type automatic_mode,

  const sys_rat_pri_list_info_s_type  *rat_pri_list_info,

  sys_detailed_plmn_list_s_type* plmn_list_p
)
{
  sys_detailed_plmn_list_info_s_type *temp_plmn_list_info = NULL;
  reg_sim_plmn_w_act_s_type *preferred_plmn_list_p = NULL;
  mmgsdi_len_type preferred_plmn_list_length = 0;
  mmgsdi_len_type operator_preferred_plmn_list_length = 0;
  mmgsdi_len_type user_preferred_plmn_list_length = 0; 
  mmgsdi_len_type total_pplmn_length = 0;
  uint8 number_of_pplmn_supported_rats = 0;
  sys_radio_access_tech_e_type pplmn_supported_rat_order[SYS_RAT_MAX];
  reg_sim_plmn_list_s_type           *forbidden_plmn_list;
  reg_mode_3gpp_capability_s_type cm_rat_search_order;
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
  boolean plmn_id_is_undefined;
  boolean mnc_includes_pcs_digit;
  uint32  rplmn_mcc;
  uint32  rplmn_mnc;
#endif

  
#ifdef FEATURE_EQUIVALENT_HPLMN
  reg_sim_plmn_list_s_type           *ehplmn_list;
#endif
  sys_plmn_id_s_type                 hplmn;

  reg_mode_3gpp_capability_s_type hplmn_rat_search_order;

  sys_plmn_id_s_type                 rplmn;
  sys_plmn_id_s_type                 temp_plmn;
  int32                              temp_plmn_list_length = 0;
  int32                              i;
  int32                              j;
  int32                              k;
  int32                              divisor;
  int32                              i_min_high_quality = 0;
  boolean                            i_min_high_quality_set = FALSE;
  uint8                              l;

  sys_plmn_id_s_type        user_resel_plmn;
  sys_radio_access_tech_e_type user_resel_rat;
  /*
  ** If the PLMN list is empty then return immediately
  ** without waiting to read the HPLMN from the SIM.
  */
  if (plmn_list_p->length == 0)
  {
    return;
  }


  temp_plmn_list_info = (sys_detailed_plmn_list_info_s_type *)modem_mem_calloc(1, sizeof(sys_detailed_plmn_list_info_s_type), MODEM_MEM_CLIENT_NAS);
  forbidden_plmn_list = (reg_sim_plmn_list_s_type *)modem_mem_calloc(1, sizeof(reg_sim_plmn_list_s_type), MODEM_MEM_CLIENT_NAS);
#ifdef FEATURE_EQUIVALENT_HPLMN
  ehplmn_list = (reg_sim_plmn_list_s_type *)modem_mem_calloc(1, sizeof(reg_sim_plmn_list_s_type), MODEM_MEM_CLIENT_NAS);
#endif
  if((temp_plmn_list_info == NULL) || (forbidden_plmn_list == NULL)
#ifdef FEATURE_EQUIVALENT_HPLMN
      || (ehplmn_list == NULL)
#endif
    )
  {
    MSG_FATAL_DS(REG_SUB, "=REG= Error: Not able to Allocate Buffer", 0, 0, 0);
  }
  /*
  ** Place the RPLMN at the beginning of the list in normal mode.
  */
  rplmn = reg_sim_read_rplmn(reg_mode_req_service_domain_get());

  if(automatic_mode == REG_MODE_AUTOMATIC_MODE_CSFB_FAIL)
  {
      /* GW plmn selection requires placing CS RPLMN on the top*/
      rplmn = reg_sim_read_cs_rplmn();
  }

#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
  if(automatic_mode == REG_MODE_AUTOMATIC_MODE_HIGH_PRIORITY_SEARCH)
  {
    rplmn = reg_state_service_plmn_get();
  }
  else
#endif
  if (automatic_mode == REG_MODE_AUTOMATIC_MODE_NORMAL ||
      automatic_mode == REG_MODE_AUTOMATIC_MODE_MMSS_RPLMN
#ifdef FEATURE_WRLF_SYSTEM_SEL
      || automatic_mode == REG_MODE_AUTOMATIC_MODE_SUITABLE_SEARCH
#endif
      || automatic_mode == REG_MODE_AUTOMATIC_MODE_CSFB_FAIL
     )
  {
    j = 0;
    temp_plmn=rplmn;
#ifdef FEATURE_WRLF_SYSTEM_SEL
    if(automatic_mode == REG_MODE_AUTOMATIC_MODE_SUITABLE_SEARCH)
    {
      temp_plmn=reg_state_rlf_plmn_get();
    }
    else
#endif
    {
       temp_plmn=rplmn;
    }
    for (i = 0; i < (int32)plmn_list_p->length; i++)
    {
      if ( sys_plmn_match(temp_plmn, plmn_list_p->info[i].plmn) )
      {
        memscpy(temp_plmn_list_info,sizeof(sys_detailed_plmn_list_info_s_type),
                             &plmn_list_p->info[i],sizeof(sys_detailed_plmn_list_info_s_type));

        for (k = i; k > j; k--)
        {
          plmn_list_p->info[k] = plmn_list_p->info[k-1];
        }

        plmn_list_p->info[j] = *temp_plmn_list_info;
        j++;
      }
    }
    temp_plmn_list_length = j;
  }
 

#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
  sys_plmn_get_mcc_mnc
  (
    rplmn,
    &plmn_id_is_undefined,
    &mnc_includes_pcs_digit,
    &rplmn_mcc,
    &rplmn_mnc
  );
#endif
  /*
  ** Sort the input PLMN list in place starting with the HPLMN.
  */

#ifndef FEATURE_EQUIVALENT_HPLMN
  hplmn           = reg_sim_read_hplmn();
#endif
 
  reg_mode_get_3gpp_rat_capabilty(&hplmn_rat_search_order, rat_pri_list_info
#ifdef FEATURE_SGLTE
  ,FALSE
#endif                                  
  );                                

  #ifdef FEATURE_CUST_1 
  //If any of EHPLMN is RPLMN then use RPLMN RAT search order
  if((automatic_mode == REG_MODE_AUTOMATIC_MODE_MANUAL) && 
      reg_sim_is_ehplmn(reg_sim_read_rplmn(reg_mode_req_service_domain_get())))
  {
    reg_sim_read_rplmn_rat_search_order(&hplmn_rat_search_order);
  }
  #endif

  reg_sim_read_hplmn_rat_search_order(&hplmn_rat_search_order);


#ifdef FEATURE_EQUIVALENT_HPLMN
  /* Reading EHPLMN list */
  reg_sim_ehplmn_list_get_ptr(ehplmn_list);

  for(j = 0; j<(int32)ehplmn_list->length; j++)
  {
    hplmn = ehplmn_list->plmn[j];
#endif

#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
    if (( (sys_plmn_match(hplmn, rplmn)) ||
           (sys_plmn_match(rplmn, sys_eplmn_list_rplmn()) &&
           sys_eplmn_list_equivalent_rplmn(hplmn, rplmn_mcc)) ) &&
         (reg_mode_is_camped_plmn_is_higher_priority_ehplmn(rplmn)) &&
        (automatic_mode == REG_MODE_AUTOMATIC_MODE_HIGH_PRIORITY_SEARCH))
    {
      i = plmn_list_p->length;
      plmn_list_p->length = temp_plmn_list_length;        
    }
    else
#endif
    {
    for (l = 0; l < hplmn_rat_search_order.num_items; l++)
    {
      for (i = temp_plmn_list_length; i < (int32)plmn_list_p->length; i++)   
      {
        if ( sys_plmn_match(hplmn, plmn_list_p->info[i].plmn) &&
             plmn_list_p->info[i].rat == hplmn_rat_search_order.rat_list[l])
        {
          memscpy(temp_plmn_list_info,sizeof(sys_detailed_plmn_list_info_s_type),
                             &plmn_list_p->info[temp_plmn_list_length],sizeof(sys_detailed_plmn_list_info_s_type));
          plmn_list_p->info[temp_plmn_list_length] = plmn_list_p->info[i];
          plmn_list_p->info[i] = *temp_plmn_list_info;

          plmn_list_p->info[temp_plmn_list_length].list_category =
            SYS_DETAILED_PLMN_LIST_CATEGORY_HPLMN;
          
          temp_plmn_list_length++;
          
          break;
          
        }             
      }
    }
    }

#ifdef FEATURE_EQUIVALENT_HPLMN
  }
#endif

if (automatic_mode == REG_MODE_AUTOMATIC_MODE_IRAT_HIGH_PRIORITY_SEARCH)
{
      i = plmn_list_p->length;
      plmn_list_p->length = temp_plmn_list_length;  
}
  /*
  ** Sort the Preferred PLMNs.
  */
  preferred_plmn_list_p = reg_sim_read_preferred_plmn_list
                                       (
                                         &preferred_plmn_list_length,
                                         &operator_preferred_plmn_list_length,
                                         &user_preferred_plmn_list_length
                                       );

  if ( preferred_plmn_list_p != NULL)
  {
    /* 
    ** If operator and user preferred plmn lists lengths are zero then
    ** we must have read the preffered plmn list from the SIM.
    */
    if((operator_preferred_plmn_list_length + user_preferred_plmn_list_length) > 0)
    {
      total_pplmn_length = operator_preferred_plmn_list_length + user_preferred_plmn_list_length;
    }
    else
    {
      total_pplmn_length = preferred_plmn_list_length;
    }
    for (j = 0; j < (int32)total_pplmn_length; j++)
    {
      //SKIP the undefined plmn
      if(sys_plmn_id_is_undefined(preferred_plmn_list_p[j].plmn))
      {
        continue;
      }
      /* 
      ** Get all the supported rats for this particular preferred plmn.
      ** and scan through the available plmn list for each of this supported
      ** rat.
      */
      number_of_pplmn_supported_rats = reg_mode_get_rat_order
                                                           (
                                                       
                                                             rat_pri_list_info,
                                                        
                                                             pplmn_supported_rat_order,
                                                             preferred_plmn_list_p[j],
                                                             NULL
                                                           );
      for(k = 0; k < number_of_pplmn_supported_rats; k++)
      {
        for (i = temp_plmn_list_length; i < (int32)plmn_list_p->length; i++)
        {
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
          if (( (sys_plmn_match(preferred_plmn_list_p[j].plmn, rplmn)) ||
                 (sys_plmn_match(rplmn, sys_eplmn_list_rplmn()) &&
                  sys_eplmn_list_equivalent_rplmn(preferred_plmn_list_p[j].plmn, rplmn_mcc)) ) &&
               (pplmn_supported_rat_order[k] == reg_state_active_rat_get() ) &&
               (automatic_mode == REG_MODE_AUTOMATIC_MODE_HIGH_PRIORITY_SEARCH))
          {
            i = plmn_list_p->length;
            plmn_list_p->length = temp_plmn_list_length;
            break;
          }
#endif
          if ( sys_plmn_match(preferred_plmn_list_p[j].plmn, plmn_list_p->info[i].plmn) &&
               pplmn_supported_rat_order[k] == plmn_list_p->info[i].rat )
          {
            memscpy(temp_plmn_list_info,sizeof(sys_detailed_plmn_list_info_s_type),
                             &plmn_list_p->info[temp_plmn_list_length],sizeof(sys_detailed_plmn_list_info_s_type));
            plmn_list_p->info[temp_plmn_list_length] = plmn_list_p->info[i];
            plmn_list_p->info[i] = *temp_plmn_list_info;
            /* 
            ** Preffered PLMN list would contain USER preffered plmns followed by OPERATOR
            ** preffered plmns or preffered plmn list read from SIM. So using the lengths to decide
            ** the list category.
            */
            if((operator_preferred_plmn_list_length + user_preferred_plmn_list_length) <= 0)
            {
              plmn_list_p->info[temp_plmn_list_length].list_category = 
                                           SYS_DETAILED_PLMN_LIST_CATEGORY_PREFERRED;
            }
            else if(j < (int32)user_preferred_plmn_list_length)
            {
              plmn_list_p->info[temp_plmn_list_length].list_category = 
                                           SYS_DETAILED_PLMN_LIST_CATEGORY_USER_PREFERRED;
            }
            else
            {
              plmn_list_p->info[temp_plmn_list_length].list_category = 
                                           SYS_DETAILED_PLMN_LIST_CATEGORY_OPERATOR_PREFERRED;
            }
            temp_plmn_list_length++;
            break;
          }
        }
      }
    }
    /* 
    ** The memory for "preferred_plmn_list_p" is allocated in function
    ** reg_sim_read_preferred_plmn_list. So freeing this memory as the
    ** processing of this preferred list is done.
    */
    modem_mem_free( preferred_plmn_list_p , MODEM_MEM_CLIENT_NAS);

    preferred_plmn_list_p = NULL;
  }

  /*
  ** Sort the PLMNs with received high quality signal.
  */
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
  if (automatic_mode != REG_MODE_AUTOMATIC_MODE_HIGH_PRIORITY_SEARCH)
#endif
  {
      /*Use preferred PLMN from NV */
     preferred_plmn_list_p = reg_nv_get_preferred_plmn_list(&preferred_plmn_list_length);
     if ( preferred_plmn_list_p != NULL && preferred_plmn_list_length > 0)
     {
       for (j = 0; j < (int32)preferred_plmn_list_length; j++)
       {
         //SKIP the undefined plmn
         if(sys_plmn_id_is_undefined(preferred_plmn_list_p[j].plmn))
         {
           continue;
         }
         /* 
         ** Get all the supported rats for this particular preferred plmn.
         ** and scan through the available plmn list for each of this supported
         ** rat.
         */
         number_of_pplmn_supported_rats = reg_mode_get_rat_order
                                                              (
                                                                rat_pri_list_info,
                                                                pplmn_supported_rat_order,
                                                                preferred_plmn_list_p[j],
                                                                NULL
                                                              );
         for(k = 0; k < number_of_pplmn_supported_rats; k++)
         {
           for (i = temp_plmn_list_length; i < (int32)plmn_list_p->length; i++)
           {
             if ( sys_plmn_match(preferred_plmn_list_p[j].plmn, plmn_list_p->info[i].plmn) &&
                  pplmn_supported_rat_order[k] == plmn_list_p->info[i].rat )
             {
               memscpy(temp_plmn_list_info,sizeof(sys_detailed_plmn_list_info_s_type),
                                &plmn_list_p->info[temp_plmn_list_length],sizeof(sys_detailed_plmn_list_info_s_type));
               plmn_list_p->info[temp_plmn_list_length] = plmn_list_p->info[i];
               plmn_list_p->info[i] = *temp_plmn_list_info;
               plmn_list_p->info[temp_plmn_list_length].list_category = 
                                              SYS_DETAILED_PLMN_LIST_CATEGORY_OPERATOR_PREFERRED;

               temp_plmn_list_length++;
               break;
             }
           }
         }
       }
     }

    if(reg_is_qrd_device == TRUE)
    {

      MSG_HIGH_DS_2(REG_SUB, "=REG= Other VPLMN is given priority over HIGH Signal Strength Plmn VPLMN LENGTH= %d total length = %d", 
                                       reg_nv_get_vplmn_list_length(), temp_plmn_list_length);
                                       
      for(i = 0; i < MIN(REG_NV_MAX_VPLMN_LENGTH,reg_nv_get_vplmn_list_length());i++)
      {                                  
        for(j=temp_plmn_list_length;j < (int32)plmn_list_p->length ; j++)
        {
           if((sys_plmn_match(reg_nv_vplmn_list->vplmn_info[i].plmn,plmn_list_p->info[j].plmn)) &&
                reg_nv_vplmn_list->vplmn_info[i].rat == plmn_list_p->info[j].rat)
           {
             memscpy(temp_plmn_list_info,sizeof(sys_detailed_plmn_list_info_s_type),
                             &plmn_list_p->info[temp_plmn_list_length],sizeof(sys_detailed_plmn_list_info_s_type));
             plmn_list_p->info[temp_plmn_list_length] = plmn_list_p->info[j];
             plmn_list_p->info[j] = *temp_plmn_list_info;
             MSG_HIGH_DS_1(REG_SUB, "=REG= Other VPLMN is given priority over HIGH Signal Strength temp_plmn_list_length = %d ", 
                                      temp_plmn_list_length);
             plmn_list_p->info[temp_plmn_list_length].list_category =
                                                 SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER;
             temp_plmn_list_length++;
             break;
           }
         }
       }
       MSG_HIGH_DS_2(REG_SUB, "=REG= Other VPLMN is given priority over HIGH Signal Strength Plmn VPLMN LENGTH= %d total length = %d", 
                                       reg_nv_get_vplmn_list_length(), temp_plmn_list_length);
    }

  i_min_high_quality = (int32)plmn_list_p->length;
  
  if(reg_mode_usim_compliant)
  {
    reg_mode_get_3gpp_rat_capabilty(&cm_rat_search_order, rat_pri_list_info
                 #ifdef FEATURE_SGLTE
                   ,FALSE
                 #endif                                  
                   );                                
    for(k = 0; k < cm_rat_search_order.num_items; k++)
    {
      i_min_high_quality_set = FALSE;
      for (i = temp_plmn_list_length; i < (int32)plmn_list_p->length; i++)
      {
        if ( (plmn_list_p->info[i].signal_quality == SYS_SIGNAL_QUALITY_HIGH ) &&
               plmn_list_p->info[i].rat == cm_rat_search_order.rat_list[k])
        {
          if (!i_min_high_quality_set)
          {
            i_min_high_quality     = temp_plmn_list_length;
            i_min_high_quality_set = TRUE;
          }
    
          memscpy(temp_plmn_list_info,sizeof(sys_detailed_plmn_list_info_s_type),
                                 &plmn_list_p->info[temp_plmn_list_length],sizeof(sys_detailed_plmn_list_info_s_type));
          plmn_list_p->info[temp_plmn_list_length] = plmn_list_p->info[i];
          plmn_list_p->info[i] = *temp_plmn_list_info;
    
          plmn_list_p->info[temp_plmn_list_length].list_category =
            SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER;
    
          temp_plmn_list_length++;
        }
      }
    
      /*
      ** Randomize the PLMNs with received high quality signal.
      */
      if(i_min_high_quality_set == TRUE)
      {
        divisor = temp_plmn_list_length - i_min_high_quality;
      
        for (i = i_min_high_quality; i < temp_plmn_list_length; i++)
        {
          dword ran_temp;
    
          mm_secapi_get_random( (uint8 *)&ran_temp, sizeof(dword) );
          j = i_min_high_quality + (divisor > 0 ?  (int32)(ran_temp % (uint32) divisor) : 0);
    
          memscpy(temp_plmn_list_info,sizeof(sys_detailed_plmn_list_info_s_type),
                                 &plmn_list_p->info[i],sizeof(sys_detailed_plmn_list_info_s_type));
          plmn_list_p->info[i] = plmn_list_p->info[j];
          plmn_list_p->info[j] = *temp_plmn_list_info;
        }
      }
    }
  }
  else
  {
    for (i = temp_plmn_list_length; i < (int32)plmn_list_p->length; i++)
    {
      if ( plmn_list_p->info[i].signal_quality == SYS_SIGNAL_QUALITY_HIGH )
      {
        if (!i_min_high_quality_set)
        {
          i_min_high_quality     = temp_plmn_list_length;
          i_min_high_quality_set = TRUE;
        }

        memscpy(temp_plmn_list_info,sizeof(sys_detailed_plmn_list_info_s_type),
                             &plmn_list_p->info[temp_plmn_list_length],sizeof(sys_detailed_plmn_list_info_s_type));
        plmn_list_p->info[temp_plmn_list_length] = plmn_list_p->info[i];
        plmn_list_p->info[i] = *temp_plmn_list_info;

        plmn_list_p->info[temp_plmn_list_length].list_category =
          SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER;

        temp_plmn_list_length++;
      }
    }

    /*
    ** Randomize the PLMNs with received high quality signal.
    */
    divisor = temp_plmn_list_length - i_min_high_quality;

    for (i = i_min_high_quality; i < temp_plmn_list_length; i++)
    {
      dword ran_temp;

      mm_secapi_get_random( (uint8 *)&ran_temp, sizeof(dword) );
      j = i_min_high_quality + (divisor > 0 ?  (int32)(ran_temp % (uint32) divisor) : 0);

      memscpy(temp_plmn_list_info,sizeof(sys_detailed_plmn_list_info_s_type),
                             &plmn_list_p->info[i],sizeof(sys_detailed_plmn_list_info_s_type));
      plmn_list_p->info[i] = plmn_list_p->info[j];
      plmn_list_p->info[j] = *temp_plmn_list_info;
    }
  }
  /*
  ** Sort the PLMNs with received low quality signal using straight insertion.
  */
  reg_mode_sort_plmn_list
  (

    rat_pri_list_info,

    plmn_list_p,
    temp_plmn_list_length,
    (int32)plmn_list_p->length
  );

  /*
  ** Set the category for the PLMNs with received low quality signal.
  */
  for (i = temp_plmn_list_length; i < (int32)plmn_list_p->length; i++)
  {
    plmn_list_p->info[i].list_category = SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER;
  }

  /*
  ** Set the forbidden flag for all of the PLMNs.
  */
#ifdef FEATURE_DUAL_SIM
  if (reg_sim_read_fplmn_list(forbidden_plmn_list, TRUE,(sys_modem_as_id_e_type)reg_sub_id ))
#else
  if (reg_sim_read_fplmn_list(forbidden_plmn_list, TRUE) )
#endif
  {
    for (j = 0; j < (int32)forbidden_plmn_list->length; j++)
    {
      for (i = 0; i < (int32)plmn_list_p->length; i++)
      {
        if ( sys_plmn_match(forbidden_plmn_list->plmn[j], plmn_list_p->info[i].plmn) )
        {
          plmn_list_p->info[i].plmn_forbidden = TRUE;
        }
      }
    }
  }
  }
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN   
  else
  {
    plmn_list_p->length = temp_plmn_list_length;
  }
#endif

  /*
  ** Place the RPLMN at the end of the list if the user initiated
  ** automatic PLMN selection.
  */
  if (automatic_mode == REG_MODE_AUTOMATIC_MODE_USER_RESELECTION)
  {
    reg_state_user_resel_plmn_rat_get(&user_resel_plmn,&user_resel_rat);

    if(!sys_plmn_id_is_undefined(user_resel_plmn))
    {
      j = 0;
      for (i = 0; i < (int32)plmn_list_p->length; i++)
      {
        if ( sys_plmn_match(user_resel_plmn, plmn_list_p->info[j].plmn) &&
             user_resel_rat == plmn_list_p->info[j].rat )
        {
          memscpy(temp_plmn_list_info,sizeof(sys_detailed_plmn_list_info_s_type),
                             &plmn_list_p->info[j],sizeof(sys_detailed_plmn_list_info_s_type));
          for (k = j+1; k < (int32)plmn_list_p->length; k++)
          {
            plmn_list_p->info[k-1] = plmn_list_p->info[k];
          }
  
          plmn_list_p->info[plmn_list_p->length-1] = *temp_plmn_list_info;
        }
        else
        {
          j++;
        }
      }
    }
  }

#if defined(FEATURE_LTE) || defined(FEATURE_WRLF_SYSTEM_SEL)
  /* NAS will priortize highest priority EHPLMN during RLF scan to abort 
  ** the existing RLF after L-RRC reports Rakuten high priorty EHPLMN 
  ** in available PLMN list during suitable search*/
  if (reg_mode_is_avail_plmn_list_contains_higher_priority_ehplmn(plmn_list_p) == TRUE)
  {
    boolean higher_priority_ehplmn_found = FALSE;
    for (j = 0; j < (int32)ehplmn_list->length; j++)
    {
      if (sys_plmn_match(reg_state_rlf_plmn_get(),ehplmn_list->plmn[j]))
      {
        break;
      }
      for (i = 0; i < (int32)plmn_list_p->length; i++)
      {
        if ( sys_plmn_match(ehplmn_list->plmn[j], plmn_list_p->info[i].plmn) )
        {
          memscpy(temp_plmn_list_info,sizeof(sys_detailed_plmn_list_info_s_type),
                             &plmn_list_p->info[i],sizeof(sys_detailed_plmn_list_info_s_type));

          for (k = i; k > 0; k--)
          {
            plmn_list_p->info[k] = plmn_list_p->info[k-1];
          }
          plmn_list_p->info[0] = *temp_plmn_list_info;
          higher_priority_ehplmn_found = TRUE;
          break;
        }
      }
      if (higher_priority_ehplmn_found)
      {
        break;
      }
    }
  }
#endif
   modem_mem_free(temp_plmn_list_info, MODEM_MEM_CLIENT_NAS);
   modem_mem_free(forbidden_plmn_list, MODEM_MEM_CLIENT_NAS);
#ifdef FEATURE_EQUIVALENT_HPLMN
   modem_mem_free(ehplmn_list, MODEM_MEM_CLIENT_NAS);
#endif

   temp_plmn_list_info = NULL;
   forbidden_plmn_list = NULL;
#ifdef FEATURE_EQUIVALENT_HPLMN
   ehplmn_list = NULL;
#endif
}


/*==============================================================================

FUNCTION NAME

  reg_mode_initial_rat

==============================================================================*/

sys_radio_access_tech_e_type reg_mode_initial_rat
(
  sys_mode_pref_e_type           mode_pref
)
{
  sys_radio_access_tech_e_type rat = SYS_RAT_GSM_RADIO_ACCESS;

#ifdef FEATURE_LTE
  if (mode_pref == SYS_MODE_PREF_LTE_RADIO_ACCESS_ONLY)
  {
    rat = SYS_RAT_LTE_RADIO_ACCESS;
  }
#endif
  if ( (mode_pref == SYS_MODE_PREF_UMTS_RADIO_ACCESS_ONLY) ||
#ifdef FEATURE_UMTS_1X_HANDOVER_UMTSMSM
       #error code not present
#endif
       (mode_pref == SYS_MODE_PREF_UMTS_RADIO_ACCESS_PREFERRED)||
       ((reg_nv_is_wcdma_freq_lock_enabled())&&
        (mode_pref== SYS_MODE_PREF_GSM_RADIO_ACCESS_PREFERRED)))
  {
    rat = SYS_RAT_UMTS_RADIO_ACCESS;
  }

  if (mode_pref == SYS_MODE_PREF_TDS_RADIO_ACCESS_ONLY)
  {
       rat = SYS_RAT_TDS_RADIO_ACCESS;
  }

  return rat;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_initial_manual_rat

DESCRIPTION

  Function that returns the initial radio access technology to use when
  searching for service in manual network selection mode.

RETURN VALUE

  sys_radio_access_tech_e_type  Initial radio access technology.

==============================================================================*/


sys_radio_access_tech_e_type reg_mode_initial_manual_rat
(
  sys_plmn_id_s_type             plmn,
  const sys_rat_pri_list_info_s_type *rat_pri_list_info
)
{
  uint32 i;
  reg_mode_3gpp_capability_s_type rat_priority_list;
  
  for (i = 0; i < reg_mode_available_plmn_list.length; i++)
  {
    if (sys_plmn_match(reg_mode_available_plmn_list.info[i].plmn, plmn) && 
         reg_mode_is_rat_present_in_rat_pri_list(
           reg_mode_available_plmn_list.info[i].rat,rat_pri_list_info, TRUE))

    {
#ifdef FEATURE_SGLTE
     if ((REG_SGLTE_DUAL_STACK) && (reg_as_id == reg_ps_stack_id) && 
         (reg_mode_available_plmn_list.info[i].rat == SYS_RAT_GSM_RADIO_ACCESS) )
     {
        continue;
     }
#endif
     return reg_mode_available_plmn_list.info[i].rat;
    }
  }

  reg_mode_get_3gpp_rat_capabilty(&rat_priority_list,rat_pri_list_info
#ifdef FEATURE_SGLTE
  ,TRUE
#endif
  );
  return rat_priority_list.rat_list[0];
}




/*==============================================================================

FUNCTION NAME

  reg_mode_unexpected_start_automatic_plmn_selection

==============================================================================*/

void reg_mode_unexpected_start_automatic_plmn_selection
(
  sys_network_selection_mode_e_type    network_selection_mode,
  reg_mode_automatic_mode_e_type       automatic_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
#ifdef FEATURE_GSM_BAND_PREF
  sys_srv_domain_e_type                req_service_domain,
  sys_band_mask_type                   band_pref
#else
  sys_srv_domain_e_type                req_service_domain
#endif
  ,const sys_rat_pri_list_info_s_type *rat_pri_list_info

#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  (void) network_selection_mode;
  (void) automatic_mode;
  (void) mode_pref;
  (void) req_service_domain;
  (void) cs_for_request;
  (void) ps_for_request;
#ifdef FEATURE_GSM_BAND_PREF
  (void) band_pref;
#endif
  (void) rat_pri_list_info;

#ifdef FEATURE_LTE
  (void)sms_only;
  (void)additional_info;
#endif

  MSG_ERROR_DS_1( REG_SUB, "=REG= start_automatic_plmn_selection (%d)",
             reg_mode_number());
}


/*==============================================================================

FUNCTION NAME

  reg_mode_unexpected_start_higher_priority_plmn_selection

==============================================================================*/

boolean reg_mode_unexpected_start_higher_priority_plmn_selection
(
  const sys_detailed_plmn_list_s_type* available_plmn_list_p
)
{
  (void) available_plmn_list_p;

  MSG_ERROR_DS_1( REG_SUB, "=REG= start_higher_priority_plmn_selection (%d)",
             reg_mode_number());

  return FALSE;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_unexpected_start_higher_priority_irat_plmn_selection

==============================================================================*/

boolean reg_mode_unexpected_start_higher_priority_irat_plmn_selection
(
  const sys_detailed_plmn_list_s_type* available_plmn_list_p
)
{
  (void) available_plmn_list_p;

  MSG_ERROR_DS_1( REG_SUB, "=REG= start_higher_priority_irat_plmn_selection (%d)",
             reg_mode_number());

  return FALSE;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_unexpected_start_limited_service

==============================================================================*/

void reg_mode_unexpected_start_limited_service
(
  boolean                              cs_for_request,
  boolean                              ps_for_request,
#ifdef FEATURE_GSM_BAND_PREF
  sys_srv_domain_e_type                req_service_domain,
  sys_band_mask_type                   band_pref
#else
  sys_srv_domain_e_type                req_service_domain
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

  ,reg_cm_service_req_e_type           type
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif

)
{
  (void) cs_for_request;
  (void) ps_for_request;
  (void) req_service_domain;
#ifdef FEATURE_GSM_BAND_PREF
  (void) band_pref;
#endif
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
#endif

  MSG_ERROR_DS_1( REG_SUB, "=REG= start_limited_service (%d)",
             reg_mode_number());
}


/*==============================================================================

FUNCTION NAME

  reg_mode_unexpected_start_manual_plmn_selection

==============================================================================*/

void reg_mode_unexpected_start_manual_plmn_selection
(
  sys_network_selection_mode_e_type network_selection_mode,
  sys_plmn_id_s_type                   plmn,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
  ,sys_band_mask_type                   band_pref
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

  ,reg_cm_service_req_e_type           type
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif

)
{
  (void) network_selection_mode;
  (void) plmn;
  (void) mode_pref;
  (void) cs_for_request;
  (void) ps_for_request;
  (void) req_service_domain;
#ifdef FEATURE_GSM_BAND_PREF
  (void) band_pref;
#endif
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
#endif

  (void) type;

  MSG_ERROR_DS_1( REG_SUB, "=REG= start_manual_plmn_selection (%d)",
             reg_mode_number());
}


/*==============================================================================

FUNCTION NAME

  reg_mode_unexpected_start_plmn_selection

==============================================================================*/

void reg_mode_unexpected_start_plmn_selection
(
  sys_network_selection_mode_e_type    network_selection_mode,
  sys_mode_pref_e_type               mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain,
  boolean                              clear_hplmn_search_timer
#ifdef FEATURE_GSM_BAND_PREF
  ,sys_band_mask_type                   band_pref
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

  ,reg_cm_service_req_e_type            type
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  (void) network_selection_mode;
  (void) mode_pref;
  (void) cs_for_request;
  (void) ps_for_request;
  (void) req_service_domain;
  (void) clear_hplmn_search_timer;
#ifdef FEATURE_GSM_BAND_PREF
  (void) band_pref;
#endif
  (void) rat_pri_list_info;


  (void) type;
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
#endif

  MSG_ERROR_DS_1( REG_SUB, "=REG= start_plmn_selection (%d)",
             reg_mode_number());
}

/*==============================================================================
FUNCTION NAME

  reg_mode_unexpected_start_plmn_selection_roaming_plmn

==============================================================================*/
void reg_mode_unexpected_start_plmn_selection_roaming_plmn
(
  sys_network_selection_mode_e_type    network_selection_mode,
  sys_mode_pref_e_type               mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain,
  boolean                              clear_hplmn_search_timer
#ifdef FEATURE_GSM_BAND_PREF
  ,sys_band_mask_type                   band_pref
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info

  ,reg_cm_service_req_e_type            type
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  (void) network_selection_mode;
  (void) mode_pref;
  (void) cs_for_request;
  (void) ps_for_request;
  (void) req_service_domain;
  (void) clear_hplmn_search_timer;
#ifdef FEATURE_GSM_BAND_PREF
  (void) band_pref;
#endif
  (void) rat_pri_list_info;


  (void) type;
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
#endif

  MSG_ERROR_DS_1( REG_SUB, "=REG= start_plmn_selection_roaming_plmn (%d)",
             reg_mode_number());
}

/*==============================================================================

FUNCTION NAME

  reg_mode_unexpected_start_plmn_selection_on_specified_plmn

==============================================================================*/

void reg_mode_unexpected_start_plmn_selection_on_specified_plmn
(
  sys_plmn_id_s_type                   plmn,
  sys_network_selection_mode_e_type    network_selection_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain,
#ifdef FEATURE_GSM_BAND_PREF
  sys_band_mask_type                   band_pref,
#endif
  const sys_rat_pri_list_info_s_type *rat_pri_list_info,

  sys_radio_access_tech_e_type         active_rat
#ifdef FEATURE_LTE
 , boolean sms_only,
   uint32  additional_info
#endif
, mmr_reg_req_e_type reg_req_type
, reg_mode_automatic_mode_e_type automatic_mode
)
{
  (void) plmn;
  (void) network_selection_mode;
  (void) mode_pref;
  (void) cs_for_request;
  (void) ps_for_request;
  (void) req_service_domain;
#ifdef FEATURE_GSM_BAND_PREF
  (void) band_pref;
#endif
  (void) rat_pri_list_info;


  (void) active_rat;
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
#endif
  (void) reg_req_type;
  (void) automatic_mode;

  MSG_ERROR_DS_1( REG_SUB, "=REG= start_plmn_selection_on_specified_plmn (%d)",
             reg_mode_number());
}

void reg_mode_unexpected_start_foreground_plmn_search
(
  sys_network_selection_mode_e_type    network_selection_mode,  
  sys_mode_pref_e_type                 mode_pref,               
  sys_srv_domain_e_type                req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
  , sys_band_mask_type                   band_pref
#endif
  , const sys_rat_pri_list_info_s_type *rat_pri_list_info

#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  (void) network_selection_mode;
  (void) mode_pref;
  (void) req_service_domain;
#ifdef FEATURE_GSM_BAND_PREF
  (void) band_pref;
#endif
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
#endif

  MSG_ERROR_DS_1( REG_SUB, "=REG= start_foreground_plmn_search (%d)",
             reg_mode_number());  
}

boolean reg_mode_unexpected_start_preferred_plmn_search
(
  sys_network_selection_mode_e_type    network_selection_mode,  
  sys_mode_pref_e_type                 mode_pref,               
  sys_srv_domain_e_type                req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
  , sys_band_mask_type                   band_pref
#endif
  , const sys_rat_pri_list_info_s_type *rat_pri_list_info

#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  (void) network_selection_mode;
  (void) mode_pref;
  (void) req_service_domain;
#ifdef FEATURE_GSM_BAND_PREF
  (void) band_pref;
#endif
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
#endif

  MSG_ERROR_DS_1( REG_SUB, "=REG= start_foreground_plmn_search (%d)",
             reg_mode_number());  

  return TRUE;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_get_scan_status

==============================================================================*/
reg_cm_scan_status_e_type reg_mode_scan_status_get(void)
{

#ifdef FEATURE_EOOS 
  #error code not present
#endif 


#ifdef FEATURE_EOOS
  #error code not present
#else
#if defined(FEATURE_SGLTE_DUAL_SIM) || defined(FEATURE_SVLTE_DUAL_SIM)
  if(reg_mode_service_req_aborted == TRUE)
  {
    return REG_CM_SCAN_STATUS_ABORTED;
  }
#endif 
  if(reg_state_last_stop_mode_reason_get() == SYS_STOP_MODE_REASON_MODE_CHANGE)
  {
     return REG_CM_SCAN_STATUS_ABORTED_STOP_MODE;
  }
  if(reg_mode_service_available_rat != SYS_RAT_NONE || reg_state_service_status_get() != SYS_SRV_STATUS_NO_SRV)
  {
    return REG_CM_SCAN_STATUS_COMPLETE_PLMN_FOUND;
  }
  else
  {
    return REG_CM_SCAN_STATUS_COMPLETE_NO_PLMN;
  }
#endif /*FEATURE_EOOS*/ 
}

/*==============================================================================

FUNCTION NAME

  reg_mode_remove_from_attempted_plmn_list

==============================================================================*/
void reg_mode_remove_from_attempted_plmn_list
(
  sys_plmn_id_s_type                   plmn,
  sys_radio_access_tech_e_type         rat
)
{
  uint32 i;
  boolean found = FALSE;

  for (i = 0; (i < reg_mode_attempted_plmn_list.length) && (i < SYS_PLMN_LIST_MAX_LENGTH); i++)
  {
    if(found == TRUE && i > 0)
    {
      reg_mode_attempted_plmn_list.info[i-1] = reg_mode_attempted_plmn_list.info[i];    
    }
    else if ( (sys_plmn_match(reg_mode_attempted_plmn_list.info[i].plmn, plmn) ) &&
         (reg_mode_attempted_plmn_list.info[i].rat == rat) )
    {
      found = TRUE;
    }
  }
  if(found == TRUE)
  {
    reg_mode_attempted_plmn_list.length--;
  }
}

/*==============================================================================

FUNCTION NAME

  reg_mode_remove_from_available_plmn_list

==============================================================================*/
void reg_mode_remove_from_available_plmn_list
(
  sys_plmn_id_s_type                   plmn,
  sys_radio_access_tech_e_type         rat
)
{
  uint32 i;
  boolean found = FALSE;

  for (i = 0; (i < reg_mode_available_plmn_list.length) && (i < SYS_PLMN_LIST_MAX_LENGTH); i++)
  {
    if(found == TRUE && i > 0)
    {
      reg_mode_available_plmn_list.info[i-1] = reg_mode_available_plmn_list.info[i];    
    }
    else if ( (sys_plmn_match(reg_mode_available_plmn_list.info[i].plmn, plmn) ) &&
              (reg_mode_available_plmn_list.info[i].rat == rat) )
    {
      found = TRUE;
    }
  }
  if(found == TRUE)
  {
    reg_mode_available_plmn_list.length--;
  }
}




/*==============================================================================

FUNCTION NAME

  reg_mode_remove_plmn_from_available_background_list

==============================================================================*/
void reg_mode_remove_plmn_from_available_background_list
(
  sys_plmn_id_s_type                   plmn,
  sys_radio_access_tech_e_type         rat
) 
{
  uint32 i;
  boolean found = FALSE;
  for (i = 0; (i < reg_mode_backgnd_list->length) && (i < SYS_PLMN_LIST_MAX_LENGTH); i++)
  {
    if(found == TRUE && i > 0)
    {
      reg_mode_backgnd_list->info[i-1] = reg_mode_backgnd_list->info[i];    
    }
    else if ( (sys_plmn_match(reg_mode_backgnd_list->info[i].plmn, plmn) ) &&
              (reg_mode_backgnd_list->info[i].rat == rat) )
    {
      found = TRUE;
    }
  }
  if(found == TRUE)
  {
    reg_mode_backgnd_list->length--;
  }
}



#ifdef FEATURE_UMTS_1X_HANDOVER_UMTSMSM
#error code not present
#endif

/*==============================================================================

FUNCTION NAME

  reg_mode_copy_available_plmn_list_manual

==============================================================================*/
void reg_mode_copy_available_plmn_list_manual
(
  boolean direction
)
{
  /*
  ** Direction FALSE means copy to Manual list.
  ** Direction TRUE means copy from Manual list.
  */
  if(direction == FALSE) 
  {
    reg_mode_available_plmn_list_manual = reg_mode_available_plmn_list; 
  }
  else
  {
    reg_mode_available_plmn_list = reg_mode_available_plmn_list_manual;
  }

  return;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_clear_available_plmn_list_manual

==============================================================================*/
void reg_mode_clear_available_plmn_list_manual()
{
  reg_mode_available_plmn_list_manual.length = 0;
  return;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_copy_available_plmn_backgnd_list

==============================================================================*/
void reg_mode_copy_available_plmn_backgnd_list
(
)
{
  uint32    i;
  /*
  ** Direction FALSE means copy to Manual list.
  ** Direction TRUE means copy from Manual list.
  */
    if(reg_mode_backgnd_list != NULL)
    {
      reg_mode_available_plmn_list.length= MIN(REG_SIM_PLMN_LIST_LENGTH ,reg_mode_backgnd_list->length); 
      for (i = 0; i < reg_mode_backgnd_list->length; i++)
      {
       reg_mode_available_plmn_list.info[i] =
                           reg_mode_backgnd_list->info[i];
      }
    }
    else
    {
      reg_mode_available_plmn_list.length=0;
    }
  return;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_clear_available_plmn_manual_backgnd

==============================================================================*/

void reg_mode_clear_plmn_backgnd_list()
{
  if(reg_mode_backgnd_list != NULL)
  {

    modem_mem_free(reg_mode_backgnd_list , MODEM_MEM_CLIENT_NAS);

    reg_mode_backgnd_list = NULL;
  }
}


/*==============================================================================

FUNCTION NAME

  reg_mode_update_service_available_rat

==============================================================================*/
void reg_mode_update_service_available_rat
(
  sys_radio_access_tech_e_type rat
)
{
  reg_mode_3gpp_capability_s_type rat_3gpp_capabilty;
  uint8 index;

  if(reg_mode_service_available_rat != rat)
  {
    if(reg_mode_service_available_rat == SYS_RAT_NONE ||
       rat == SYS_RAT_NONE)
    {
      reg_mode_service_available_rat = rat;
    }

    else
    {
      reg_mode_get_3gpp_rat_capabilty(&rat_3gpp_capabilty, reg_mode_rat_pri_list_get()
#ifdef FEATURE_SGLTE
      ,TRUE
#endif                                  
     );
      for(index = 0; index < rat_3gpp_capabilty.num_items; index++)
      {
        if(rat == rat_3gpp_capabilty.rat_list[index])
        {
          reg_mode_service_available_rat = rat;
          break;
        }
        else if(reg_mode_service_available_rat == rat_3gpp_capabilty.rat_list[index])
        {
          break;
        }
      }
    }

    }
  MSG_HIGH_DS_1(REG_SUB, "=REG= Updated service available rat to %d", reg_mode_service_available_rat); 
  return;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_service_available_rat_get

==============================================================================*/
sys_radio_access_tech_e_type reg_mode_service_available_rat_get()
{
  return reg_mode_service_available_rat;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_filter_available_plmn_list_rat

==============================================================================*/
void reg_mode_filter_available_plmn_list_rat
(
  sys_radio_access_tech_e_type         rat
)
{
  uint32                   i;
  uint32                   j;

  j=0;
  for (i = 0; i < reg_mode_available_plmn_list.length; i++)
  {
    if(reg_mode_available_plmn_list.info[i].rat == rat)
    {
      /* remove this entry */
      j++;
    }
    else
    {
      reg_mode_available_plmn_list.info[i-j] = reg_mode_available_plmn_list.info[i];
    }
  }
  reg_mode_available_plmn_list.length-=j;
}

sys_detailed_plmn_list_info_s_type reg_mode_managed_roaming_plmn_get
(
  void
)
{
  return reg_mode_managed_roaming_plmn;
}
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
void reg_per_nas_stacks_mode_managed_roaming_plmn_reset
(
  sys_modem_as_id_e_type stack_id_nas
)
{
  if(IS_NOT_VALID_STACK_ID(stack_id_nas))
  {
    MSG_ERROR_DS_1(REG_SUB, "=MM= Invalid STACK ID:%d", stack_id_nas+1);
    return;
  }
  sys_plmn_undefine_plmn_id(&reg_mode_managed_roaming_plmn_sim[stack_id_nas].plmn);
  reg_mode_managed_roaming_plmn_sim[stack_id_nas].rat = SYS_RAT_NONE;
}
#endif

void reg_mode_managed_roaming_plmn_reset
(
  void
)
{
  sys_plmn_undefine_plmn_id(&reg_mode_managed_roaming_plmn.plmn);
  reg_mode_managed_roaming_plmn.rat = SYS_RAT_NONE;
}

mmr_reg_req_e_type reg_mode_convert_cm_req_type_to_mm
(
  reg_cm_service_req_e_type type, 
  sys_network_selection_mode_e_type network_selection_mode
)
{
  switch(type)
  {
    case REG_CM_SERVICE_REQ_NORMAL:
    case REG_CM_SERVICE_REQ_PERIODIC:
    case REG_CM_SERVICE_REQ_LAST_CAMPED_RAT:
       return MMR_REG_REQ_NORMAL;
    case REG_CM_SERVICE_REQ_BETTER_SYSTEM_RESELECTION:
      return MMR_REG_REQ_BETTER_SYSTEM_RESELECTION;
    case REG_CM_SERVICE_REQ_SYSTEM_CHANGE:
      return MMR_REG_REQ_SYSTEM_CHANGE;
    case REG_CM_SERVICE_REQ_USER_SELECTION:
    case REG_CM_SERVICE_REQ_USER_PREFERRED_PLMN_RAT:
      if(network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL ||
         network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL_CAMP_ONLY ||
         network_selection_mode == SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION)
        return MMR_REG_REQ_USER_SELECTION;
      else
        return MMR_REG_REQ_NORMAL;
    case REG_CM_SERVICE_REQ_SRV_MODE_UPDATE:
      return MMR_REG_REQ_SRV_MODE_UPDATE;
      break;
    default:
      return MMR_REG_REQ_NONE;
  }
}

/*===========================================================================

FUNCTION reg_mode_enhanced_hplmn_srch_init

DESCRIPTION
  This function initializes the enhanced hplmn search tbl and re-populates
  the tbl from the NV.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void reg_mode_enhanced_hplmn_srch_init(void)
{
  byte nv_config_tbl[REG_NV_CONFIG_TBL_MAX_SIZE];

  memset(nv_config_tbl, 0, sizeof(nv_config_tbl));
  
  /* Read the table from NV */
  reg_nv_read_enhanced_hplmn_srch_tbl(nv_config_tbl);

  /* Parse the table and store in global */
  reg_mode_enhanced_hplmn_search_parse_tbl(nv_config_tbl);

  return;
}


#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_LTE)
/*==============================================================================

FUNCTION NAME

  reg_mode_lte_enabled_on_sub

==============================================================================*/
boolean reg_mode_lte_enabled_on_sub
(
  uint32 sub_capability_mask
)
{
  if(sub_capability_mask & SYS_SYS_MODE_MASK_LTE)
  {
     return TRUE;
  }
  return FALSE;
}
#endif

#if defined(FEATURE_LTE) || defined(FEATURE_WRLF_SYSTEM_SEL)
/*==============================================================================

FUNCTION NAME

  reg_mode_filter_available_plmn_list_rat

==============================================================================*/
void reg_mode_unexpected_start_suitable_cell_selection
(
  sys_network_selection_mode_e_type    network_selection_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
  ,sys_band_mask_type                   band_pref
#endif
  ,const sys_rat_pri_list_info_s_type *rat_pri_list_info
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif 
)
{
  (void) network_selection_mode;
  (void) mode_pref;
  (void) cs_for_request;
  (void) ps_for_request;
  (void) req_service_domain;
#ifdef FEATURE_GSM_BAND_PREF
  (void) band_pref;
#endif
  (void) rat_pri_list_info;
#ifdef FEATURE_LTE
  (void) sms_only;
  (void) additional_info;
 #endif
  MSG_ERROR_DS_1(REG_SUB, "=REG= start_suitable_cell_selection (%d)",
             reg_mode_number());
}

void reg_mode_start_suitable_cell_selection
(
  sys_network_selection_mode_e_type    network_selection_mode,
  sys_mode_pref_e_type                 mode_pref,
  boolean                              cs_for_request,
  boolean                              ps_for_request,
  sys_srv_domain_e_type                req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
  ,sys_band_mask_type                   band_pref
#endif
  ,const sys_rat_pri_list_info_s_type  *rat_pri_list_info
#ifdef FEATURE_LTE
  , boolean sms_only,
    uint32  additional_info
#endif
)
{
  reg_mode_p = &reg_mode_plmn_selection;
  reg_mode_mode_pref = mode_pref;
  reg_mode_network_selection_mode = network_selection_mode;
  reg_mode_cs_for_request         = cs_for_request;
  reg_mode_ps_for_request         = ps_for_request;
  reg_mode_req_service_domain     = req_service_domain;
#ifdef FEATURE_GSM_BAND_PREF
  reg_mode_band_pref              = band_pref;
#endif
  memscpy(&reg_mode_rat_pri_list, sizeof(sys_rat_pri_list_info_s_type), rat_pri_list_info, sizeof(sys_rat_pri_list_info_s_type));

#ifdef FEATURE_LTE
  reg_mode_sms_only = sms_only;
  reg_mode_additional_info = additional_info;
#endif 
  reg_mode_p->start_suitable_cell_selection
  (
    network_selection_mode,
    mode_pref,
    cs_for_request,
    ps_for_request,
    req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
    ,band_pref
#endif
    ,rat_pri_list_info
#ifdef FEATURE_LTE
   ,sms_only,
    additional_info
#endif
  );

}

#endif 

/*===========================================================================
FUNCTION    reg_bit_mask_check

DESCRIPTION
  Function checks value passed to see if particular element  is set

DEPENDENCIES
  None

RETURN VALUE
  returns TRUE if particular bit is set, FALSE otherwise.

SIDE EFFECTS
  None
===========================================================================*/

boolean reg_bit_mask_check
(
  uint32 value,
  byte bit_mask
)
{
  if((value & (1 << bit_mask)) > 0)
  {
    return TRUE;
  }
  return FALSE;
}



#ifdef FEATURE_LTE


/*==============================================================================

FUNCTION NAME

  reg_mode_is_only_req_rat_present_in_rat_pri_list

  This function will return true if the RAT priority list contains LTE only or GSM only based on requested rat and BST is TRUE
  
==============================================================================*/

boolean reg_mode_is_only_req_rat_present_in_rat_pri_list
(
  const sys_rat_pri_list_info_s_type *rat_pri_list_info, sys_sys_mode_e_type rat_sys_mode
)
{
  uint32 index;
  boolean found = FALSE;
 

  for(index = 0; index < rat_pri_list_info->num_items; index++)
  {
    if(rat_pri_list_info->priority_list_info[index].bst_rat_acq_required == TRUE)
    {
      if(rat_sys_mode == 
           rat_pri_list_info->priority_list_info[index].acq_sys_mode)
      {
        found = TRUE;
      }
      else if( (rat_pri_list_info->priority_list_info[index].acq_sys_mode == SYS_SYS_MODE_WCDMA) ||
               (rat_pri_list_info->priority_list_info[index].acq_sys_mode == SYS_SYS_MODE_TDS) )
      {
        found = FALSE;
        break;
      }
      else if( (rat_sys_mode == SYS_SYS_MODE_GSM && (rat_pri_list_info->priority_list_info[index].acq_sys_mode == SYS_SYS_MODE_LTE)) ||
               ( rat_sys_mode == SYS_SYS_MODE_LTE && (rat_pri_list_info->priority_list_info[index].acq_sys_mode == SYS_SYS_MODE_GSM)) )
      {
        found = FALSE;
        break;
      }
    }
  }
  return found;
}

#endif

/*==============================================================================

FUNCTION NAME

  reg_mode_is_rat_present_in_rat_pri_list

==============================================================================*/

boolean reg_mode_is_rat_present_in_rat_pri_list
(
  sys_radio_access_tech_e_type rat,
  const sys_rat_pri_list_info_s_type *rat_pri_list_info,
  boolean use_bst
)
{
  uint32 index;
  boolean found = FALSE;
 
  sys_sys_mode_e_type rat_sys_mode = SYS_SYS_MODE_NONE;

  switch(rat)
  {
    case SYS_RAT_GSM_RADIO_ACCESS:
      rat_sys_mode = SYS_SYS_MODE_GSM;
      break;
    case SYS_RAT_UMTS_RADIO_ACCESS:
      rat_sys_mode = SYS_SYS_MODE_WCDMA;
      break;
    case SYS_RAT_TDS_RADIO_ACCESS:
      rat_sys_mode = SYS_SYS_MODE_TDS;
      break; 
#ifdef FEATURE_LTE
    case SYS_RAT_LTE_RADIO_ACCESS:
      rat_sys_mode = SYS_SYS_MODE_LTE;
      break; 
#endif
    default :
      MSG_HIGH_DS_1(REG_SUB,"=REG= default rat is %d", rat);
      break;
  }

  for(index = 0; index < rat_pri_list_info->num_items; index++)
  {
    if((rat_sys_mode == 
           rat_pri_list_info->priority_list_info[index].acq_sys_mode)
           && ((use_bst == FALSE) || (reg_mode_bst_status_get() == FALSE)
           || (rat_pri_list_info->priority_list_info[index].bst_rat_acq_required == TRUE)))
    {
      found = TRUE;
      break;
    }
  }

  return found;
}

#ifdef FEATURE_EOOS
#error code not present
#endif /*FEATURE_EOOS*/
/*==============================================================================

FUNCTION NAME

  reg_mode_convert_acq_mode_aceess_tech

==============================================================================*/

sys_radio_access_tech_e_type reg_mode_convert_acq_mode_to_access_tech
(
   sys_sys_mode_e_type acq_sys_mode
)
{
  sys_radio_access_tech_e_type rat = SYS_RAT_NONE;

  switch(acq_sys_mode)
  {
    case SYS_SYS_MODE_GSM:
      rat = SYS_RAT_GSM_RADIO_ACCESS;
      break;
    case SYS_SYS_MODE_WCDMA:
      rat = SYS_RAT_UMTS_RADIO_ACCESS;
      break;
    case SYS_SYS_MODE_TDS:
      rat = SYS_RAT_TDS_RADIO_ACCESS;
      break;     
    case SYS_SYS_MODE_LTE:
      rat = SYS_RAT_LTE_RADIO_ACCESS;
      break;     
    default :
      MSG_HIGH_DS_1(REG_SUB,"=REG= default acq_sys_mode is %d",acq_sys_mode);
      break;
  }      
  return rat;
}


#if defined(FEATURE_LTE) && defined(FEATURE_MMSS_3_1)
/*==============================================================================

FUNCTION NAME

  reg_mode_is_multimode_enabled

==============================================================================*/

boolean reg_mode_is_multimode_enabled
(
  const sys_rat_pri_list_info_s_type *rat_pri_list_info
)
{
  uint32 index;  
  boolean is_mmode_enabled = FALSE;

  for(index = 0; index < rat_pri_list_info->num_items; index++)
  {
    if(rat_pri_list_info->priority_list_info[index].acq_sys_mode == SYS_SYS_MODE_CDMA ||
         rat_pri_list_info->priority_list_info[index].acq_sys_mode == SYS_SYS_MODE_HDR) 
    {
      is_mmode_enabled = TRUE;
      break;
    }
  }
  return is_mmode_enabled;
}
#endif

void reg_mode_set_hplmn_to_be_selected( boolean hplmn_to_be_selected)
{
  reg_mode_hplmn_to_be_selected = hplmn_to_be_selected; 
  MSG_HIGH_DS_1(REG_SUB, "=REG= SET HPMLN to be given priority in OOS/Power up %d", reg_mode_hplmn_to_be_selected);
}

boolean reg_mode_get_hplmn_to_selected
(
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
   sys_modem_as_id_e_type as_id
#else
   void
#endif
)
{

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
   MSG_HIGH_DS_1(REG_SUB, "=REG= HPMLN to be given priority in OOS/Power up - %d",reg_mode_hplmn_to_be_selected_sim[as_id]);
   return reg_mode_hplmn_to_be_selected_sim[as_id];
   
#else
  MSG_HIGH_DS_1(REG_SUB, "=REG= HPMLN to be given priority in OOS/Power up - %d",reg_mode_hplmn_to_be_selected);
  return reg_mode_hplmn_to_be_selected;
#endif

}

#ifdef FEATURE_TDSCDMA
/*==========================================================================

FUNCTION NAME

  reg_mode_get_3GPP_coverage

  FUNCTION PARAMETERS
  sys_radio_access_tech_e_type rat

  DESCRIPTION
  For a given RAT, returns TRUE, if there is a coverage for this RAT. Otherwise,
  it returns FALSE.
===========================================================================*/
boolean reg_mode_get_3GPP_coverage(sys_detailed_plmn_list_s_type available_plmn_list,
                                   sys_radio_access_tech_e_type rat)
{

   uint32 i;
   /*
   ** Per System's team, the acceptable signal range is -120dBm to -25dBm. 
   ** This range is mapped to positive values when NAS receives the 
   ** list of PLMNs
   */
    for ( i= 0; (i < available_plmn_list.length) && (i < SYS_PLMN_LIST_MAX_LENGTH); i++)
    {
      if ((available_plmn_list.info[i].rat == rat) &&
          (available_plmn_list.info[i].signal_strength > 25) &&
           (available_plmn_list.info[i].signal_strength < 120))
      {
        return TRUE;
      }
    }
    return FALSE;
}
#endif

#if defined(FEATURE_LTE) 
/*==============================================================================

FUNCTION NAME

  reg_mode_sms_only_get

==============================================================================*/
boolean reg_mode_sms_only_get
(
  void
)
{
  return reg_mode_sms_only;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_additional_info_get

==============================================================================*/
uint32 reg_mode_additional_info_get
(
  void
)
{
  return reg_mode_additional_info;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_reset_addtnl_info

==============================================================================*/
void reg_mode_reset_addtnl_info
(
  sys_addtnl_info_e_type  bit_mask
)
{
  //reset the requested field of reg_mode_additional_info
  reg_mode_additional_info = reg_mode_additional_info & (~(1 << bit_mask));
}


/*==============================================================================

FUNCTION NAME

  reg_mode_lte_cs_domain_param_get

==============================================================================*/
sys_lte_cs_domain_param_s_type reg_mode_lte_cs_domain_param_get
(
  void
)
{
  return reg_mode_lte_cs_domain_param;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_lte_cs_domain_param_set

==============================================================================*/
void reg_mode_lte_cs_domain_param_set
(
  sys_lte_cs_domain_param_s_type  lte_cs_domain_param
)
{
  reg_mode_lte_cs_domain_param = lte_cs_domain_param;
}


/*==============================================================================

FUNCTION NAME

  reg_mode_start_plmn_selection_to_gw

==============================================================================*/

void reg_mode_start_plmn_selection_to_gw
(
  void
)
{
  uint32 index, i, j;
  sys_plmn_id_s_type   plmn;
  sys_priority_list_info_s_type temp_list_info;
  sys_scan_scope_e_type scan_scope = SYS_SCAN_SCOPE_ACQ_DB;


  sys_rat_pri_list_info_s_type  rat_pri_list;
 

  uint32 tg_index;
  boolean plmn_tdscdma;
  sys_priority_list_info_s_type t_priority_list; 

  reg_mode_p = &reg_mode_plmn_selection;


  plmn = reg_sim_read_cs_rplmn();
  reg_mode_bst_status_set(TRUE);
  
  MSG_HIGH_DS_1(REG_SUB, "=REG= plmn selection to gw, pass %d", reg_mode_selection_to_gw_first_pass);

  // No need to calculate these parameters twice
  if (reg_mode_selection_to_gw_first_pass == FALSE) 
  {
      reg_mode_selection_to_gw_first_pass = TRUE;
    
      

  for(index = 0; index < reg_mode_rat_pri_list.num_items; index++)
  {
    if(reg_mode_rat_pri_list.priority_list_info[index].acq_sys_mode == SYS_SYS_MODE_LTE)
    {
       reg_mode_rat_pri_list.priority_list_info[index].bst_rat_acq_required = FALSE;       
    }
#ifdef FEATURE_CMCC_SECURE_CALL
    else if(reg_mode_rat_pri_list.priority_list_info[index].acq_sys_mode != SYS_SYS_MODE_TDS  
            && reg_state_secure_call_req_type == SYS_SECURE_CALL_START)
    {
       reg_mode_rat_pri_list.priority_list_info[index].bst_rat_acq_required = FALSE;       
    }
#endif
    else
    {
      reg_mode_rat_pri_list.priority_list_info[index].bst_rat_acq_required = TRUE;
    } 
  }

  rat_pri_list = reg_mode_rat_pri_list;

    /* On CSFB failure for TDCDMA opertators 
             1. give priority to GSM if CSFB fails when UE is in TDSCDMA PLMNs
             2. Search TDSCDMA at last if UE is not in TDSCDMA operator PLMNs*/  
  plmn_tdscdma = reg_mode_is_plmn_td_compliant(plmn);
  if(reg_mode_is_usim_compliant() && (plmn_tdscdma == TRUE))
  {
    for(index = 0; index < rat_pri_list.num_items; index++)
    {
      if(rat_pri_list.priority_list_info[index].acq_sys_mode == SYS_SYS_MODE_GSM)
      {
        t_priority_list = rat_pri_list.priority_list_info[index];
        for(tg_index = index; tg_index > 0; tg_index--)
        {
          rat_pri_list.priority_list_info[tg_index] = 
                rat_pri_list.priority_list_info[tg_index-1];
        }
        rat_pri_list.priority_list_info[0] = t_priority_list;
        break;
      }
    }
  }
  /* Modify the rat priority list, such that the rats with higher priority in sib6 or sib7
     indications from AS are tried first*/
  else if(csfb_failure_rat_search_order.num_items > 0)
  {
    for(index = 0; index < csfb_failure_rat_search_order.num_items; index++)
    {
      for(i = index; i < rat_pri_list.num_items; i++)
      {
        if(((csfb_failure_rat_search_order.rat_list[index].rat == SYS_RAT_GSM_RADIO_ACCESS) &&
            (rat_pri_list.priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_GSM))
           ||
           ((csfb_failure_rat_search_order.rat_list[index].rat == SYS_RAT_UMTS_RADIO_ACCESS) &&
            (rat_pri_list.priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_WCDMA))
           ||
           ((csfb_failure_rat_search_order.rat_list[index].rat == SYS_RAT_TDS_RADIO_ACCESS) &&
            (rat_pri_list.priority_list_info[i].acq_sys_mode == SYS_SYS_MODE_TDS)))
        {
          if(i > index)
          {
            temp_list_info = rat_pri_list.priority_list_info[i];
            j = i;
            while(j > index)
            {
              rat_pri_list.priority_list_info[j] = rat_pri_list.priority_list_info[j-1];
              j--;
            }
            rat_pri_list.priority_list_info[index] = temp_list_info;
          }
          break;
        }
      }
    }

    /* Output RAT search order */
    MSG_HIGH_DS_8(REG_SUB, "Num items = %d. RAT1 = %d, RAT2 = %d, RAT3 = %d, RAT4 = %d, RAT5 = %d, RAT6 = %d, RAT7 = %d",
                  rat_pri_list.num_items,
                  rat_pri_list.priority_list_info[0].acq_sys_mode,
                  rat_pri_list.priority_list_info[1].acq_sys_mode,
                  rat_pri_list.priority_list_info[2].acq_sys_mode,
                  rat_pri_list.priority_list_info[3].acq_sys_mode,
                  rat_pri_list.priority_list_info[4].acq_sys_mode,
                  rat_pri_list.priority_list_info[5].acq_sys_mode,
                  rat_pri_list.priority_list_info[6].acq_sys_mode);
  }
  
  //store values in first pass
  csfb_start_plmn_sel_rat_pri_list = rat_pri_list;
  csfb_start_plmn_sel_active_rat = reg_state_active_rat_get();
      
  } // first pass
  else
  {
      scan_scope = SYS_SCAN_SCOPE_FULL_BAND;
      reg_mode_selection_to_gw_first_pass = FALSE;
  }

  reg_mode_scan_scope_set(scan_scope);

  reg_mode_p->start_plmn_selection_on_specified_plmn
  (
    plmn,
    reg_mode_network_selection_mode,
    reg_mode_mode_pref,
    0,
    0,
    reg_mode_req_service_domain
#ifdef FEATURE_GSM_BAND_PREF
    ,reg_mode_band_pref
#endif
    ,&csfb_start_plmn_sel_rat_pri_list

    ,csfb_start_plmn_sel_active_rat
#ifdef FEATURE_LTE
    ,reg_mode_sms_only,
     0,
#endif   
    MMR_REG_REQ_ACQ_GW,
    REG_MODE_AUTOMATIC_MODE_CSFB_FAIL
  );
}

#endif /*FEATURE_LTE*/

/*==============================================================================

FUNCTION NAME

  reg_mode_set_usim_compliancy
 
DESCRIPTION 
 
   TDSCDMA oems will populate the EFS with EHPLMNs. This function performs
   a check if the card inserted in the UE belongs to the TDSCDMA operators.
   Called at power up.

==============================================================================*/
void reg_mode_set_usim_compliancy()
{

   sys_plmn_id_s_type hplmn;
   reg_sim_plmn_list_s_type* oem_list_efs = NULL;
   uint32 i;

   /* Read the list of PLMN that have been set for a TDSCDMA network
   */
  NAS_ENTER_CRIT_SECT(reg_tdscdma_op_list_crit_sect);
   oem_list_efs = reg_nv_tdscdma_plmn_list_get();
   ASSERT(oem_list_efs);
   reg_mode_usim_compliant = FALSE;

   /* Read the IMSI based HPLMN */
   hplmn = reg_sim_read_hplmn();

   /* Determine, if IMSI belongs to the PLMNs specified in EHPLMN NV */
   for (i = 0; i < oem_list_efs->length; ++i)
   {
       if ( sys_plmn_match( hplmn, oem_list_efs->plmn[i]))       
       {
           MSG_HIGH_DS_0(REG_SUB, "=REG= SIM compliant with EFS EHPLMNs");            
           reg_mode_usim_compliant = TRUE;
           break;
       }
   }     
  NAS_EXIT_CRIT_SECT(reg_tdscdma_op_list_crit_sect);
}
/*==============================================================================

FUNCTION NAME

  reg_is_usim_compliant
 
DESCRIPTION 
 
   This function returns a boolean value that indicates if the USIM card
   is compliant with TDSCDMA OEMs

==============================================================================*/
boolean reg_mode_is_usim_compliant()
{
    return reg_mode_usim_compliant;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_is_plmn_compliant
 
DESCRIPTION 
 
   This function returns a boolean value that indicates if the passed in
   plmn is compliant with TDSCDMA OEMs

==============================================================================*/
boolean reg_mode_is_plmn_td_compliant(sys_plmn_id_s_type plmn)
{     

   reg_sim_plmn_list_s_type* oem_list_efs = NULL;
   uint32 i;
   boolean td_plmn_comp = FALSE;
   NAS_ENTER_CRIT_SECT(reg_tdscdma_op_list_crit_sect);
   oem_list_efs = reg_nv_tdscdma_plmn_list_get();
   ASSERT(oem_list_efs);
   for (i = 0; i < oem_list_efs->length; ++i)
   {
       if ( sys_plmn_match( plmn, oem_list_efs->plmn[i]))       
       {
         MSG_HIGH_DS_0(REG_SUB, "=REG= PLMN compliant with EFS EHPLMNs");            
         td_plmn_comp = TRUE;
         break;
       }
   }    
   NAS_EXIT_CRIT_SECT(reg_tdscdma_op_list_crit_sect);
   return td_plmn_comp;
}


/*==============================================================================

FUNCTION NAME

   reg_mode_check_validate_sim_counter
 
DESCRIPTION 
 
   This function returns checks if validate SIM counter value exceeeded max counter.
==============================================================================*/
boolean reg_mode_check_validate_sim_counter(void)
{
  boolean return_value = FALSE;
  uint8 max_validate_counter_value = reg_nv_max_validate_sim_counter_get();
  if(max_validate_counter_value == REG_NV_DEFAULT_MAX_VALIDATE_SIM_COUNTER)
  {
    //This counter is not supported return TRUE
    return_value = TRUE;
  }
  else if(reg_mode_validate_sim_counter < max_validate_counter_value)
  {
    return_value = TRUE;
  }
  return return_value;
}
#ifdef FEATURE_SGLTE

/*==============================================================================

FUNCTION NAME

   reg_mode_get_validate_sim_counter
 
DESCRIPTION 
 
   This function returns the validate sim timer value.
==============================================================================*/
uint8 reg_mode_get_validate_sim_counter(void)
{
  return reg_mode_validate_sim_counter;
}


/*==============================================================================

FUNCTION NAME

   reg_mode_set_validate_sim_counter
 
DESCRIPTION 
 
   This function sets the validate sim timer value.
==============================================================================*/
void reg_mode_set_validate_sim_counter(uint8 value)
{
  reg_mode_validate_sim_counter = value;
}
#endif
/*==============================================================================

FUNCTION NAME

   reg_mode_revalidate_sim_allowed
 
DESCRIPTION 
 
   This function returns a boolean value that indicates if Validate SIM timer needs to be started.
   This function will check if USIM card is compliant with TDSCDMA OEMs and if passed PLMN is 
   compliant with TDSCDMA OEMs.

==============================================================================*/
boolean  reg_mode_revalidate_sim_allowed(sys_plmn_id_s_type plmn)
{
  boolean return_value = FALSE;
  if((reg_nv_is_sim_invalid_recovery_enabled
     || (reg_mode_is_usim_compliant() &&
         reg_mode_is_plmn_td_compliant(plmn))) &&
     (reg_nv_validate_sim_timer_value_get() != REG_TIMERS_MAX_TIMER_VALUE)&&
      reg_mode_check_validate_sim_counter() &&     
      (reg_nv_gcf_flag_get() == FALSE))
  {
    return_value = TRUE;
  }
  MSG_HIGH_DS_3(REG_SUB, "=REG= Revalidate SIM allowed return value %d, reg_mode_validate_sim_counter %d reg_nv_is_sim_invalid_recovery_enabled : %d ",
                return_value, 
                reg_mode_validate_sim_counter, 
                reg_nv_is_sim_invalid_recovery_enabled);
  return return_value;
}

/*==============================================================================

FUNCTION NAME

   reg_mode_increment_validate_sim_counter
 
DESCRIPTION 
 
   This function increments the validate sim counter

==============================================================================*/
void reg_mode_increment_validate_sim_counter(void)
{
  if(reg_nv_max_validate_sim_counter_get() != 
                REG_NV_DEFAULT_MAX_VALIDATE_SIM_COUNTER)
  {
    reg_mode_validate_sim_counter++;
    MSG_HIGH_DS_1(REG_SUB, "=REG= validate sim counter is incremented to %d", reg_mode_validate_sim_counter);
  }
}

/*==============================================================================

FUNCTION NAME

   reg_mode_reset_validate_sim_counter
 
DESCRIPTION 
 
   This function resets the validate sim counter.

==============================================================================*/
void reg_mode_reset_validate_sim_counter(void)
{
  reg_mode_validate_sim_counter = 0;
  MSG_HIGH_DS_0(REG_SUB, "=REG= validate sim counter is reset");
}


boolean reg_mode_is_usim_compliant_for_cm(sys_modem_as_id_e_type as_id)
{
#ifdef FEATURE_DUAL_SIM
  MSG_HIGH_DS_2(REG_SUB, "=REG= sub is %d reg mode is usim compliant %d", as_id, reg_mode_usim_compliant_sim[as_id]);
  return reg_mode_usim_compliant_sim[as_id];
#else
  MSG_HIGH_DS_2(REG_SUB, "=REG= sub is %d reg mode is usim compliant %d", as_id, reg_mode_usim_compliant);
  return reg_mode_usim_compliant;
#endif
}

#ifdef FEATURE_SGLTE
void reg_mode_perform_merge_operation(void)
{
//  reg_mode_network_selection_mode_sim[reg_ps_stack_id] = reg_mode_network_selection_mode_sim[reg_cs_stack_id];
//  reg_mode_p_sim[reg_ps_stack_id] = reg_mode_p_sim[reg_cs_stack_id];
  reg_mode_cs_for_request_sim[reg_ps_stack_id] = reg_mode_cs_for_request_sim[reg_cs_stack_id];
//  reg_mode_band_pref_sim[reg_ps_stack_id] = reg_mode_band_pref_sim[reg_cs_stack_id];//swap
  reg_mode_req_service_domain_sim[reg_ps_stack_id] = SYS_SRV_DOMAIN_CS_PS;
  reg_mode_req_plmn_sim[reg_ps_stack_id] = reg_mode_req_plmn_sim[reg_cs_stack_id];
  reg_mode_req_rat_sim[reg_ps_stack_id] = reg_mode_req_rat_sim[reg_cs_stack_id];
  reg_mode_available_plmn_list_sim[reg_ps_stack_id] = reg_mode_available_plmn_list_sim[reg_cs_stack_id];
  reg_mode_lte_retry_counter = 0;                   
  reg_mode_clear_available_plmn_list();
  reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id] = reg_mode_available_plmn_list_manual_sim[reg_cs_stack_id];
  reg_mode_clear_available_plmn_list_manual();
  reg_mode_clear_plmn_backgnd_list();
  reg_mode_attempted_plmn_list_sim[reg_ps_stack_id] = reg_mode_attempted_plmn_list_sim[reg_cs_stack_id];
  reg_mode_clear_attempted_plmn_list();
  reg_mode_service_available_rat_sim[reg_ps_stack_id] = reg_mode_service_available_rat_sim[reg_cs_stack_id];
  reg_mode_service_available_rat_sim[reg_cs_stack_id] = SYS_RAT_NONE;
  reg_mode_hplmn_to_be_selected_sim[reg_cs_stack_id] = FALSE;
//  reg_mode_managed_roaming_plmn_sim[reg_cs_stack_id] = { 0 }; //swap
}



void reg_mode_update_dual_stack_variables(void)
{
  if( reg_mode_service_available_rat_sim[reg_ps_stack_id] == SYS_RAT_GSM_RADIO_ACCESS)
  {
    reg_mode_service_available_rat_sim[reg_ps_stack_id] = SYS_RAT_NONE ;
    reg_mode_service_available_rat_sim[REG_SGLTE_STACK_2] = SYS_RAT_GSM_RADIO_ACCESS;
  }
}

/* move the GSM plmn's to CS stack*/
void reg_mode_copy_available_plmn_list_for_split(void)
{
  uint32 i =0;
  uint32 j =0;

  reg_mode_available_plmn_list_sim[reg_cs_stack_id].length = 0;

  /*repeat the loop until end of the list */
  while(i< reg_mode_available_plmn_list_sim[reg_ps_stack_id].length)
  {
    /* if rat is GSM move it to CS stack */
    if(reg_mode_available_plmn_list_sim[reg_ps_stack_id].info[i].rat == SYS_RAT_GSM_RADIO_ACCESS)
    {
      reg_mode_available_plmn_list_sim[reg_cs_stack_id].\
        info[reg_mode_available_plmn_list_sim[reg_cs_stack_id].length++] = reg_mode_available_plmn_list_sim[reg_ps_stack_id].info[i];
      /* remove the GSM rat from PS stack */
      for(j = i; j < reg_mode_available_plmn_list_sim[reg_ps_stack_id].length-1; j++)
      {
        reg_mode_available_plmn_list_sim[reg_ps_stack_id].info[j] = reg_mode_available_plmn_list_sim[reg_ps_stack_id].info[j+1];
      }
      reg_mode_available_plmn_list_sim[reg_ps_stack_id].length = reg_mode_available_plmn_list_sim[reg_ps_stack_id].length-1;
    }
    else
    {
      i++;
    }
  } /*end of while*/
} 
/*Assumed to run on Hybrid stack**/
void reg_mode_perform_split_operation(void)
{
   reg_as_id_e_type reg_local_as_id;
//  reg_mode_network_selection_mode_sim[reg_cs_stack_id] = reg_mode_network_selection_mode_sim[reg_ps_stack_id];
  reg_mode_p_sim[reg_cs_stack_id] = reg_mode_p_sim[reg_ps_stack_id];
  reg_mode_cs_for_request_sim[reg_cs_stack_id] = reg_mode_cs_for_request_sim[reg_ps_stack_id];
  reg_mode_cs_for_request_sim[reg_ps_stack_id] = FALSE;
//  reg_mode_band_pref_sim[reg_cs_stack_id] = reg_mode_band_pref_sim[reg_ps_stack_id];// swap
  reg_mode_req_service_domain_sim[reg_ps_stack_id] = SYS_SRV_DOMAIN_PS_ONLY;
  reg_mode_req_service_domain_sim[reg_cs_stack_id] = SYS_SRV_DOMAIN_CS_ONLY;
  if( reg_mode_service_available_rat_sim[reg_ps_stack_id] == SYS_RAT_GSM_RADIO_ACCESS)
  {
    reg_mode_service_available_rat_sim[reg_cs_stack_id] = SYS_RAT_GSM_RADIO_ACCESS ;
  }
  else
  {
    reg_mode_service_available_rat_sim[reg_cs_stack_id] = SYS_RAT_NONE ;
  }
  reg_mode_service_available_rat_sim[reg_ps_stack_id] = SYS_RAT_NONE;
  reg_mode_req_plmn_sim[reg_cs_stack_id] = reg_mode_req_plmn_sim[reg_ps_stack_id];
  reg_mode_lte_retry_counter = 0;                   
//  reg_mode_req_rat_sim[reg_ps_stack_id] = SYS_RAT_LTE_RADIO_ACCESS;
  reg_mode_req_rat_sim[reg_cs_stack_id] = SYS_RAT_GSM_RADIO_ACCESS;
  reg_mode_copy_available_plmn_list_for_split();
  reg_mode_available_plmn_list_manual_sim[reg_cs_stack_id] = reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id];
  reg_mode_attempted_plmn_list_sim[reg_cs_stack_id] = reg_mode_attempted_plmn_list_sim[reg_ps_stack_id];
  reg_local_as_id = reg_as_id;
  reg_set_as_id(reg_ps_stack_id);
//  reg_mode_clear_available_plmn_list();
  reg_mode_clear_attempted_plmn_list();
  reg_mode_clear_available_plmn_list_manual();
  reg_mode_clear_plmn_backgnd_list();
  reg_set_as_id(reg_local_as_id);
/*swap managed roaming PLMN list PLMN list*/
}
#endif
#ifdef FEATURE_SGLTE
/*==============================================================================

FUNCTION NAME

  reg_mode_copy_available_plmn_lists
 
DESCRIPTION 
 
   This function transfers the available PLMN list between stacks

==============================================================================*/
void reg_mode_copy_available_plmn_lists(sys_service_search_e_type service_search)
{      
    uint32 i = 0;
    uint32 j = 0;
    uint32 new_length;            
    
    /* Note that 2 lists are used here and there are separate requirements for each */
    if (service_search == SYS_SERVICE_SEARCH_NONE)
    {        
        /* Copy the GSM PLMNs from the main stack to the hybrid one */
        for (i = 0; (i < reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id].length) && 
            (i < SYS_PLMN_LIST_MAX_LENGTH); i++)
        {            
            if(reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id].info[i].rat == 
                SYS_RAT_GSM_RADIO_ACCESS)
            {               
                reg_mode_available_plmn_list_manual_sim[reg_cs_stack_id].info[j] = 
                    reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id].info[i];
                j++;
                reg_mode_available_plmn_list_manual_sim[reg_cs_stack_id].length = j;
            }            
        }
        /* Now remove the GSM PLMNs from the main stack list */
        new_length = reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id].length;
        
        for (i = reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id].length; 
        (i > 0) && (i < SYS_PLMN_LIST_MAX_LENGTH+1);
        i--)
        {                
            if (reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id].info[i-1].rat == 
                SYS_RAT_GSM_RADIO_ACCESS)
            {
                if (i != (reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id].length))
                {                
                    j = i-1;
                    while ((j < new_length - 1) && (j < SYS_PLMN_LIST_MAX_LENGTH - 1))
                    {
                        reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id].info[j] =
                            reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id].info[j+1];
                        j++;
                    }
                    
                }
                new_length--;
            }
        }        
        reg_mode_available_plmn_list_manual_sim[reg_ps_stack_id].length = new_length;
    }
    else if (service_search == SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC ||
         service_search == SYS_SERVICE_SEARCH_BACKGROUND_MANUAL)
    {
        if (reg_mode_backgnd_list_sim[reg_ps_stack_id] == NULL)
        {
            return;
        }
        /* Remove the GSM PLMNs from the main stack list */
        new_length = reg_mode_backgnd_list_sim[reg_ps_stack_id]->length;
        
        for (i = reg_mode_backgnd_list_sim[reg_ps_stack_id]->length; 
        (i > 0) && (i < SYS_PLMN_LIST_MAX_LENGTH+1);
        i--)
        {                
            if (reg_mode_backgnd_list_sim[reg_ps_stack_id]->info[i-1].rat == 
                SYS_RAT_GSM_RADIO_ACCESS)
            {
                if (i != (reg_mode_backgnd_list_sim[reg_ps_stack_id]->length))
                {                
                    j = i-1;
                    while ((j < new_length - 1) && (j < SYS_PLMN_LIST_MAX_LENGTH - 1))
                    {
                        reg_mode_backgnd_list_sim[reg_ps_stack_id]->info[j] =
                            reg_mode_backgnd_list_sim[reg_ps_stack_id]->info[j+1];
                        j++;
                    }
                    
                }
                new_length--;
            }
        }        
        reg_mode_backgnd_list_sim[reg_ps_stack_id]->length = new_length;
    }
 
}
#endif


void reg_mode_reset_retry_counter(void)
{
  reg_mode_lte_retry_counter= 0;
}

void reg_mode_start_appropriate_irat_timers(void)
{
  boolean cell_not_in_lte_lists = FALSE;

  cell_not_in_lte_lists = reg_state_cell_not_present_in_lte_lists();

  if(!cell_not_in_lte_lists)
  {
    reg_timers_start_lte_irat_search_period_timer();
  }

  if(reg_state_active_rat_get() == SYS_RAT_GSM_RADIO_ACCESS  ||
     ((reg_state_active_rat_get() == SYS_RAT_TDS_RADIO_ACCESS ||
       reg_state_active_rat_get() == SYS_RAT_UMTS_RADIO_ACCESS) && cell_not_in_lte_lists))
  {
    reg_timers_start_hplmn_irat_search_period_timer();
  }
#if defined (FEATURE_SGLTE) 
  if(REG_SUB_IS_SGLTE && reg_state_get_rlf_started() && reg_timer_bplmn_irat_running())
  {
    /* this rlf flag needs to be reset here, if the timer is being started due to rlf*/
    reg_state_set_rlf_started(FALSE);
  }
#endif
}

void reg_mode_stop_appropriate_period_timer(void)
{

boolean cell_not_in_lte_lists = FALSE;
byte found_in_avaialble_list = 0;
byte found_in_non_avaialble_list =0;

reg_state_cell_present_in_list( &found_in_avaialble_list, &found_in_non_avaialble_list);

cell_not_in_lte_lists = !(found_in_avaialble_list || found_in_non_avaialble_list);

   if (
#ifdef FEATURE_SGLTE
       REG_SGLTE_DUAL_STACK || (!reg_sglte_mode && reg_state_active_rat_get() == SYS_RAT_LTE_RADIO_ACCESS)
#else
       reg_state_active_rat_get() == SYS_RAT_LTE_RADIO_ACCESS
#endif
       ) /* SGLTE state 2 condition */
   {
     /* Stop the timer if got service on LTE  */
     reg_timers_stop_all_irat_search_period_timer();
   }
   if(reg_state_active_rat_get() == SYS_RAT_TDS_RADIO_ACCESS)
   {
     if(!cell_not_in_lte_lists)
     {
       reg_timers_stop_hplmn_irat_search_period_timer();
     }
   }
}

void reg_mode_update_lte_lists(void)
{

  byte found_in_avaialble_list = 0;
  byte found_in_non_avaialble_list =0;
  byte i=0;
  boolean earfcn_added = FALSE;
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)            
byte event_size = 1;
byte diag_event[1];
#else
byte event_size = 0;
#endif

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
diag_event[0] = mm_sub_id;
#endif

  reg_state_cell_present_in_list( &found_in_avaialble_list, &found_in_non_avaialble_list);
  MSG_HIGH_DS_2(REG_SUB, "=REG= LTE IRAT BPLMN : update the lte cell list Active RAT = %d CELL ID = %d",
                reg_state_active_rat_get(),
                reg_get_cell_id());

  for (i=0;i< reg_mode_available_plmn_list.length;i++)
  {
    if(reg_mode_available_plmn_list.info[i].rat == SYS_RAT_LTE_RADIO_ACCESS)
    {
      if( reg_state_active_rat_get() == SYS_RAT_TDS_RADIO_ACCESS)                  
      {
        earfcn_added = reg_state_add_tds_or_gsm_or_w_cell_id_to_lte_list(TRUE, SYS_RAT_TDS_RADIO_ACCESS, reg_mode_available_plmn_list.info[i].earfcn);
        if(found_in_avaialble_list == FALSE || earfcn_added == TRUE)
        {
          reg_send_mmr_pseudo_list_change_ind(SYS_RAT_TDS_RADIO_ACCESS, TRUE);
        }
      }
      else if( reg_state_active_rat_get() == SYS_RAT_GSM_RADIO_ACCESS)                 
      {
        earfcn_added = reg_state_add_tds_or_gsm_or_w_cell_id_to_lte_list(TRUE, SYS_RAT_GSM_RADIO_ACCESS, reg_mode_available_plmn_list.info[i].earfcn );
        if(found_in_avaialble_list == FALSE || earfcn_added == TRUE)
        {
          reg_send_mmr_pseudo_list_change_ind(SYS_RAT_GSM_RADIO_ACCESS,TRUE);
        }
      }
      else if( reg_state_active_rat_get() == SYS_RAT_UMTS_RADIO_ACCESS)                 
      {
        earfcn_added = reg_state_add_tds_or_gsm_or_w_cell_id_to_lte_list(TRUE, SYS_RAT_UMTS_RADIO_ACCESS, reg_mode_available_plmn_list.info[i].earfcn );
        if(found_in_avaialble_list == FALSE || earfcn_added == TRUE)
        {
          reg_send_mmr_pseudo_list_change_ind(SYS_RAT_UMTS_RADIO_ACCESS,TRUE);
        }
      }

      if(found_in_non_avaialble_list)
      {
       reg_state_lte_non_available_list_ptr = reg_state_delete_tds_or_gsm_or_w_cell_from_list(reg_state_lte_non_available_list_ptr, reg_state_active_rat_get());                
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)    
        nas_event_report_payload(EVENT_NAS_LTE_IRAT_NOTAVAIL_TO_AVAIL,
               (uint8)event_size,(void*)diag_event,reg_message_as_id);
#else
        nas_event_report_payload(EVENT_NAS_LTE_IRAT_NOTAVAIL_TO_AVAIL,
               (uint8)event_size,NULL,reg_message_as_id);
#endif
       if(reg_state_lte_non_available_list_ptr == NULL)
       {
         reg_nv_clear_non_available_efs_list();
       }
       MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : deleated from non available list");               
      }
      if(reg_get_cell_id() != 0xFFFF)
      {
        reg_state_lte_available_cell_id.plmn  = reg_state_service_plmn_get();
        reg_state_lte_available_cell_id.rat  = reg_state_active_rat_get();
        reg_state_lte_available_cell_id.lac  = reg_state_service_lac_get();
        reg_state_lte_available_cell_id.cell_id= reg_get_cell_id();
       }
       reg_state_hplmn_irat_timer_count = 0;
       MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN: Cell ID is saved and reg_state_hplmn_irat_timer_count is reset");
      break;
    }
  }


  if( i == reg_mode_available_plmn_list.length && reg_state_get_lte_search_requested())
  {
     int8 count; 
     if(!found_in_avaialble_list && !found_in_non_avaialble_list)
     {
       reg_mode_lte_retry_counter ++;
     }
     if(found_in_avaialble_list)
     {
       count = reg_state_increment_cell_list_counter(reg_state_lte_available_list_ptr,reg_state_service_plmn_get(),reg_state_active_rat_get(),reg_state_service_lac_get(),reg_get_cell_id(),-1);
       MSG_HIGH_DS_1(REG_SUB, "=REG= LTE IRAT BPLMN: total count of not getting lte on GSM/TDS cell is %d", count);
     }
     if((found_in_avaialble_list && count >= reg_nv_lte_search_config.max_lte_failures) ||
        ((!(found_in_avaialble_list || found_in_non_avaialble_list)) && reg_mode_lte_retry_counter >= reg_nv_lte_search_config.max_lte_failures_on_neutral_cell) )
     {
       (void)reg_state_add_tds_or_gsm_or_w_cell_id_to_lte_list(FALSE, reg_state_active_rat_get(), INVALID_EARFCN);          
       MSG_HIGH_DS_1(REG_SUB, "=REG= LTE IRAT BPLMN : added to LTE non available list cell ID = %d, delete the same from LTE available list if present",
                     reg_get_cell_id());
       if(found_in_avaialble_list == TRUE)
       {
          reg_send_mmr_pseudo_list_change_ind(reg_state_active_rat_get(), FALSE);
       }
       reg_state_lte_available_list_ptr = reg_state_delete_tds_or_gsm_or_w_cell_from_list(reg_state_lte_available_list_ptr,reg_state_active_rat_get());       
       if(reg_state_lte_available_list_ptr == NULL)
       {
         reg_nv_clear_available_efs_list();
       }
       if(found_in_avaialble_list)   
       {
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
         nas_event_report_payload(EVENT_NAS_LTE_IRAT_AVAIL_TO_NOTAVAIL,
              (uint8)event_size,(void*)diag_event,reg_message_as_id);
#else
         nas_event_report_payload(EVENT_NAS_LTE_IRAT_AVAIL_TO_NOTAVAIL,
            (uint8)event_size,NULL,reg_message_as_id);
#endif
       }
       MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : deleted from LTE available list");                  
     }
  }
  MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : LTE available list"); 
  reg_state_dump_the_list(reg_state_lte_available_list_ptr,TRUE);
  MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : LTE not available list"); 
  reg_state_dump_the_list(reg_state_lte_non_available_list_ptr,FALSE);
  reg_state_set_lte_search_requested(FALSE);
}

/*==============================================================================

FUNCTION NAME

  reg_mode_rat_search_start
 
DESCRIPTION 
 
   This function starts a high priority rat search if certain conditions
   are met

==============================================================================*/
boolean reg_mode_rat_search_start ()
{
    boolean condition_met = FALSE;



    MSG_HIGH_DS_4(REG_SUB, "=REG= In reg_mode_rat_search_start USIM compliance = %d,reg_nv_w_bplmn_pseudo_enabled = %d, reg_nv_pseudo_reselection_allowed = %d, Cell Id = %d",
                  reg_mode_is_usim_compliant(),
                  reg_nv_w_bplmn_pseudo_enabled(),
                  reg_nv_pseudo_reselection_allowed(),
                  reg_get_cell_id());

    MSG_HIGH_DS_1(REG_SUB, "=REG= Forced IRAT = %d",reg_nv_is_forced_irat_enabled());

      /* Evaluate legacy set of conditions first */
      if ( (reg_nv_w_bplmn_pseudo_enabled()
            ||reg_nv_is_forced_irat_enabled()
            )
#ifdef FEATURE_SGLTE
         &&
          (! reg_sglte_mode)
#endif
       )
       {
#ifdef FEATURE_LTE
              /* Evaluate second set of conditions*/
              condition_met = ((reg_state_roaming_ind_get() == SYS_ROAM_STATUS_OFF)&&
               ((reg_state_active_rat_get() == SYS_RAT_TDS_RADIO_ACCESS) ||
                                (reg_state_active_rat_get() == SYS_RAT_GSM_RADIO_ACCESS) ||
                                (reg_state_active_rat_get() == SYS_RAT_UMTS_RADIO_ACCESS)) &&
                                (reg_mode_is_usim_compliant() || reg_nv_w_bplmn_pseudo_enabled())&&
                (reg_get_cell_id() != 0xFFFF )&&
                reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_LTE_RADIO_ACCESS, reg_mode_curr_rat_pri_list_get(), FALSE) && 
                reg_mode_network_selection_mode_get() != SYS_NETWORK_SELECTION_MODE_LIMITED);
              MSG_HIGH_DS_1(REG_SUB, "=REG= Starting RAT search 0 %d", condition_met);
#endif
       }
#ifdef FEATURE_SGLTE
       else if (reg_sglte_mode && reg_state_active_rat_get() == SYS_RAT_TDS_RADIO_ACCESS) /* SGLTE state 2 condition */
       {
              condition_met = ((reg_state_roaming_ind_get() == SYS_ROAM_STATUS_OFF)&&
                reg_mode_is_usim_compliant() && 
                (reg_get_cell_id() != 0xFFFF )&&
                reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_LTE_RADIO_ACCESS, reg_mode_rat_pri_list_get(), FALSE) && 
                reg_mode_network_selection_mode_get() != SYS_NETWORK_SELECTION_MODE_LIMITED);
               MSG_HIGH_DS_1(REG_SUB, "=REG= Starting RAT search 1 %d", condition_met);

       }
#endif

#ifdef FEATURE_SGLTE
       if (IS_UE_IN_SGLTE_STATE_4 && (reg_state_get_plmn_service_capability ((sys_modem_as_id_e_type)reg_cs_stack_id) == SYS_SRV_CAPABILITY_CS_PS_SERVICE)) /* SGLTE state 4 condition */       
       {           
           condition_met = ((reg_state_roaming_ind_get() == SYS_ROAM_STATUS_OFF)&&
               (reg_state_active_rat_get() == SYS_RAT_GSM_RADIO_ACCESS) &&
                reg_mode_is_usim_compliant() && 
                 (reg_get_cell_id() != 0xFFFF )&&
                (reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_LTE_RADIO_ACCESS, reg_mode_rat_pri_list_get(), FALSE)
#ifdef FEATURE_TDSCDMA
                 ||   reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_TDS_RADIO_ACCESS, reg_mode_rat_pri_list_get(), FALSE)
#endif
                  ) &&
                reg_mode_network_selection_mode_get() != SYS_NETWORK_SELECTION_MODE_LIMITED);
             MSG_HIGH_DS_1(REG_SUB, "=REG= Starting RAT search 2 %d", condition_met);

       }
#endif
       if (condition_met)
       {
         reg_mode_start_appropriate_irat_timers();
       }
       return condition_met;
}
/*==============================================================================

FUNCTION NAME

  reg_mode_rat_search_stop
 
DESCRIPTION 
 
   This function stops the high priority rat search if certain conditions
   are met

==============================================================================*/
boolean reg_mode_rat_search_stop (sys_radio_access_tech_e_type  rat)
{
    boolean condition_met = FALSE;

       /* Evaluate legacy set of conditions first */
      if ( (reg_nv_w_bplmn_pseudo_enabled()
            || reg_nv_is_forced_irat_enabled()
            )
#ifdef FEATURE_SGLTE
         &&
          (! reg_sglte_mode)
#endif
       )
       {
              /* Evaluate second set of conditions*/
              condition_met = TRUE;
              MSG_HIGH_DS_1(REG_SUB, "=REG= Stopping RAT search 0 %d", condition_met);
       }
#ifdef FEATURE_SGLTE
       else if (REG_SGLTE_DUAL_STACK) /* SGLTE state 2 condition */
       {
              /* Stop the timer if got service on LTE  */
              condition_met = (rat == SYS_RAT_LTE_RADIO_ACCESS);
              MSG_HIGH_DS_1(REG_SUB, "=REG= Stopping RAT search 1 %d", condition_met);
       }
       else if (IS_UE_IN_SGLTE_STATE_4 && (reg_state_get_plmn_service_capability ((sys_modem_as_id_e_type)reg_cs_stack_id) == SYS_SRV_CAPABILITY_CS_PS_SERVICE)) /* SGLTE state 4 condition */
       {
           /* Stop the timer if got service on LTE or TD and USIM is compliant */
           condition_met = 
               (((rat == SYS_RAT_TDS_RADIO_ACCESS) || (rat == SYS_RAT_LTE_RADIO_ACCESS)));
           MSG_HIGH_DS_1(REG_SUB, "=REG= Stopping RAT search 2 %d", condition_met);
       }
#endif
       if (condition_met)
       {
           reg_mode_stop_appropriate_period_timer();
       }
       return condition_met;
}
/*==============================================================================

FUNCTION NAME

  reg_mode_update_plmn_selection_mode

==============================================================================*/
void reg_mode_update_plmn_selection_mode
(
  reg_mode_automatic_mode_e_type  plmn_selection_mode
)
{
   reg_mode_p->update_plmn_selection_mode(plmn_selection_mode);
}

/*==============================================================================

FUNCTION NAME

  reg_set_cm_stack_id

==============================================================================*/
sys_modem_stack_id_e_type reg_set_cm_stack_id(void)
{
  sys_modem_stack_id_e_type stack_id = SYS_MODEM_STACK_ID_1;
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
  if(REG_SUB_IS_SGLTE)
  {
    if(reg_as_id != reg_sub_id)
    {
      stack_id = SYS_MODEM_STACK_ID_2;
    }
  }
#endif 
  return stack_id;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_trm_timeout_value_get

==============================================================================*/

uint32 reg_mode_trm_timeout_value_get(sys_radio_access_tech_e_type rat)
{

  uint32   trm_timeout = (uint32)REG_TIMERS_MAX_TIMER_VALUE;

#ifdef FEATURE_SVLTE_DUAL_SIM
  if (reg_mode_timed_service_req == TRUE)
  {
     trm_timeout = mm_get_ps_timer_value()/1000;    
  }
#endif 

#ifdef FEATURE_SGLTE_DUAL_SIM
  if (REG_SGLTE_DUAL_STACK &&
      reg_state_get_trm_failure_sglte_main())
  {
    /* timed service req, to check continuity of failure */
    trm_timeout = mm_get_ps_timer_value()/1000;
  }
#endif 

  return trm_timeout;
}

#if defined(FEATURE_SVLTE_DUAL_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
/*==============================================================================

FUNCTION NAME

  reg_mode_service_req_aborted_set

==============================================================================*/

void reg_mode_service_req_aborted_set(boolean service_req_aborted)
{
  reg_mode_service_req_aborted = service_req_aborted;
}

#endif 
#ifdef FEATURE_SVLTE_DUAL_SIM
/*==============================================================================

FUNCTION NAME

  reg_mode_timed_service_req_set

==============================================================================*/
void reg_mode_timed_service_req_set(uint32 additional_info)
{
  if (reg_bit_mask_check(additional_info,
                         (byte)SYS_ADDTNL_INFO_TIMED_REQUEST) == TRUE)
  {
    reg_mode_timed_service_req = TRUE;
    MSG_HIGH_DS_0(REG_SUB, "=REG= CM_TIMED_SERVICE_REQ" );
  }
  else
  {
    reg_mode_timed_service_req = FALSE;
  }
}
#endif 
#ifdef FEATURE_LTE

/*==============================================================================

FUNCTION NAME

  reg_mode_volte_call_in_progress

==============================================================================*/
void reg_mode_set_volte_call_in_progress(uint32 additional_info, sys_network_selection_mode_e_type network_selection_mode,reg_cm_service_req_e_type  type)
{
  if ((network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC)&&
       (reg_bit_mask_check(additional_info,(byte)SYS_ADDTNL_INFO_IMS_CALL_CONNECTED) == TRUE) && 
       (type == REG_CM_SERVICE_REQ_NORMAL || type == REG_CM_SERVICE_REQ_SUITABLE_SEARCH))
  {
    reg_mode_volte_call_connected = TRUE;
    MSG_HIGH_DS_0(REG_SUB, "=REG= Volte call connected" );
  }
  else
  {
    reg_mode_volte_call_connected = FALSE;
  }
}
boolean reg_mode_get_volte_call_in_progress(void)
{
  return reg_mode_volte_call_connected;
}
/*==============================================================================

FUNCTION NAME

  reg_mode_lte_cs_domain_param_match

==============================================================================*/
boolean reg_mode_lte_cs_domain_param_match
(
  const sys_lte_cs_domain_param_s_type lte_cs_domain_param_from_cm, 
  const sys_lte_cs_domain_param_s_type lte_cs_domain_param_in_reg
)
{
  /* Return successful match if is_voice_domain_ie_valid
     is FALSE in lte_cs_domain_param in CM_SERVICE_REQ */
  if(lte_cs_domain_param_from_cm.is_voice_domain_ie_valid == FALSE)
  {
    return TRUE;
  }
  else if(lte_cs_domain_param_in_reg.is_voice_domain_ie_valid == TRUE)
  {
    if((lte_cs_domain_param_from_cm.voice_or_data_centric !=
        lte_cs_domain_param_in_reg.voice_or_data_centric) ||
       (lte_cs_domain_param_from_cm.voice_domain_pref !=
        lte_cs_domain_param_in_reg.voice_domain_pref))
    {
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
  return TRUE;
}
#endif

boolean reg_mode_is_china_mcc(sys_plmn_id_s_type plmn)
{
  boolean  plmn_id_is_undefined;
  boolean  mnc_includes_pcs_digit;
  uint32   mcc;
  uint32   mnc;
  boolean result = FALSE;

  sys_plmn_get_mcc_mnc(plmn,&plmn_id_is_undefined,&mnc_includes_pcs_digit,
                         &mcc,&mnc);

  if(mcc == 455 || mcc == 460) /*change to decimal laterif required*/
  {
    result = TRUE;
  }
  return result;
}

/*==============================================================================
  FUNCTION NAME  reg_mode_remove_rat_from_3gpp_rat_list   
 -- Removed desired rat from rat list
==============================================================================*/
void reg_mode_remove_rat_from_3gpp_rat_list(reg_mode_3gpp_capability_s_type *reg_3gpp_rat_list,const sys_radio_access_tech_e_type rat_to_remove)
{ 

  uint8 i,j;      
  for (i= 0; i < reg_3gpp_rat_list->num_items; i++)  
  {    
    if(reg_3gpp_rat_list->rat_list[i] == rat_to_remove)   
    {      
      for(j = i+1; j < reg_3gpp_rat_list->num_items; j++)    
      {       
        reg_3gpp_rat_list->rat_list[j-1] =           
        reg_3gpp_rat_list->rat_list[j];      
      }
       reg_3gpp_rat_list->num_items--;    
       break;   
    } 
  } 
}


/*==============================================================================
  FUNCTION NAME

              reg_mode_determine_first_plmn

  FUNCTION PARAMETERS
     plmn - Caller will pass the RPLMN
     reg_3gpp_rat_list - This is rat priority list from CM

  DESCRIPTION
  -- This function determines the first plmn with which PLMN SELECTION needs to be started 
==============================================================================*/
void reg_mode_determine_first_plmn
(
  sys_plmn_id_s_type *plmn, 
  reg_mode_3gpp_capability_s_type *rat_list_3gpp,
  const sys_rat_pri_list_info_s_type *rat_pri_list_info,
  boolean preferred_foreground_search
)
{
  policyman_status_t  status;
  sys_mcc_type pm_found_mcc;
  
  boolean plmn_id_is_undefined;
  boolean mnc_includes_pcs_digit;
  uint32  plmn_mcc;
  uint32  plmn_mnc;
  reg_sim_plmn_w_act_s_type *preferred_plmn_list_p = NULL;
  mmgsdi_len_type preferred_plmn_list_length = 0;
  mmgsdi_len_type operator_preferred_plmn_list_length = 0; 
  int32 index; 
  int32 found_index = 0;
  mmgsdi_len_type user_preferred_plmn_list_length = 0; 
  uint8 supported_rats = 0;
  sys_radio_access_tech_e_type pplmn_supported_rat_order[SYS_RAT_MAX];
  sys_detailed_plmn_list_info_s_type reg_mode_temp_available_plmn;
  uint8 number_of_pplmn_supported_rats = 0;
  boolean found = FALSE; 
  reg_sim_plmn_list_s_type           *ehplmn_list_ptr = NULL;
  mmgsdi_len_type total_pplmn_length = 0; 
  uint32 i,j;
  nv_rplmnact_type rplmnact;
  boolean rplmn_mcc_match = TRUE;
  boolean rplmnact_undefined = TRUE;

  reg_mode_available_plmn_list_populated = FALSE;

  /* This function is not GCF compatible, return if GCF test is being executed */
  if (reg_nv_gcf_flag_get() == TRUE)
      return;

 if ((reg_mode_network_selection_mode_get() != SYS_NETWORK_SELECTION_MODE_AUTOMATIC) &&
       (reg_mode_network_selection_mode_get() != SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION) &&
       (reg_mode_network_selection_mode_get() != SYS_NETWORK_SELECTION_MODE_AUTOMATIC_CAMP_ONLY))
        return;

  reg_nv_read_rplmnact(&rplmnact);
  rplmnact_undefined = ((rplmnact.act[0] == 0) && (rplmnact.act[1] == 0));

  if(preferred_foreground_search)
  {
    sys_plmn_get_mcc_mnc
    (
      *plmn,
      &plmn_id_is_undefined,
      &mnc_includes_pcs_digit,
      &pm_found_mcc,
      &plmn_mnc
    );
  }
  else
  {
    status = policyman_get_current_mcc((sys_modem_as_id_e_type)reg_sub_id, &pm_found_mcc);

    /*
      ** 1. Get current MCC from PM, check if it is different from RPLMN MCC if yes then find PPLMN matching PM MCC
      **
      ** 2. If RPLMNACT is undefined and if PM MCC is unavailable or RPLMN MCC is same as PM MCC then find RPLMN
      **     in EHPLMN or PPLMN list if  available use its priority order, if not available follow rat priority list order.
      */

   if (POLICYMAN_SUCCEEDED(status))
   {
     MSG_HIGH_DS_1(REG_SUB, "=REG= Policyman reported mcc = %d", pm_found_mcc);
     sys_plmn_get_mcc_mnc
     (
       *plmn,
       &plmn_id_is_undefined,
       &mnc_includes_pcs_digit,
       &plmn_mcc,
       &plmn_mnc
     );

     if(pm_found_mcc != 1665 && pm_found_mcc != 0)
     {
       rplmn_mcc_match = sys_mcc_same_country(plmn_mcc, pm_found_mcc);
     }

   }
  }

  if((!rplmn_mcc_match) || (rplmnact_undefined) || (preferred_foreground_search))
  {
   /* Compare with HPLMN */
#ifdef FEATURE_EQUIVALENT_HPLMN
    ehplmn_list_ptr = (reg_sim_plmn_list_s_type *)modem_mem_calloc(1, sizeof(reg_sim_plmn_list_s_type), MODEM_MEM_CLIENT_NAS);  
    if (ehplmn_list_ptr == NULL)
    {
     MSG_FATAL_DS(REG_SUB, "=REG= Error: Not able to Allocate Buffer", 0, 0, 0);
    }

    reg_sim_ehplmn_list_get_ptr(ehplmn_list_ptr);

    for(i = 0; i < ehplmn_list_ptr->length; i++)
    {
       if(sys_plmn_id_is_undefined(ehplmn_list_ptr->plmn[i]) ||
	   	  ((preferred_foreground_search) && sys_plmn_match(*plmn,ehplmn_list_ptr->plmn[i])))
       {
         continue;
       }
       if(!rplmn_mcc_match || preferred_foreground_search)
       {
         
         sys_plmn_get_mcc_mnc
         (
           ehplmn_list_ptr->plmn[i],
           &plmn_id_is_undefined,
           &mnc_includes_pcs_digit,
           &plmn_mcc,
           &plmn_mnc
         );
         if(sys_mcc_same_country(plmn_mcc, pm_found_mcc)
#ifdef FEATURE_LTE
           && !((reg_mode_is_only_req_rat_present_in_rat_pri_list(rat_pri_list_info,SYS_SYS_MODE_LTE) == TRUE ) && 
                (reg_sim_plmn_rat_forbidden(ehplmn_list_ptr->plmn[i],
                               SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)reg_sub_id)))
#endif
           )
         {
           *plmn = ehplmn_list_ptr->plmn[i];
           MSG_HIGH_DS_2(REG_SUB, "=REG= PLMN to start PLMN selection (%d-%d)", plmn_mcc, plmn_mnc);
           found = TRUE;
           reg_sim_read_hplmn_rat_search_order(rat_list_3gpp);
           break;
         }
       }
       else if(rplmnact_undefined)
       {
         if(sys_plmn_match(ehplmn_list_ptr->plmn[i], *plmn)
#ifdef FEATURE_LTE
           && !((reg_mode_is_only_req_rat_present_in_rat_pri_list(rat_pri_list_info,SYS_SYS_MODE_LTE) == TRUE ) && 
                (reg_sim_plmn_rat_forbidden(ehplmn_list_ptr->plmn[i],
                               SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)reg_sub_id)))
#endif
           )
         {
           found = TRUE;
           reg_sim_read_hplmn_rat_search_order(rat_list_3gpp);
           break;
         }
       }
    }
    modem_mem_free( ehplmn_list_ptr , MODEM_MEM_CLIENT_NAS);

    if(found == TRUE)
    {
      return; 
    }
#endif 
    /* Compare with OPLMN/UPLMN list*/
    preferred_plmn_list_p = reg_sim_read_preferred_plmn_list
                                        (
                                          &preferred_plmn_list_length,
                                          &operator_preferred_plmn_list_length,
                                          &user_preferred_plmn_list_length
                                        );
    if ( preferred_plmn_list_p == NULL)
    {
      return;
    }
    if((operator_preferred_plmn_list_length + user_preferred_plmn_list_length) > 0)
    {
      total_pplmn_length = operator_preferred_plmn_list_length + user_preferred_plmn_list_length;
    }
    else
    {
      total_pplmn_length = preferred_plmn_list_length;
    }

    for(index = 0; index < (int32)total_pplmn_length; index ++)
    {
       if(sys_plmn_id_is_undefined(preferred_plmn_list_p[index].plmn)||
	   	  ((preferred_foreground_search) && sys_plmn_match(*plmn,preferred_plmn_list_p[index].plmn)))
       {
         continue;
       }
       if(!rplmn_mcc_match || preferred_foreground_search)
       {
         
         sys_plmn_get_mcc_mnc
         (
           preferred_plmn_list_p[index].plmn,
           &plmn_id_is_undefined,
           &mnc_includes_pcs_digit,
           &plmn_mcc,
           &plmn_mnc
         );

         /* If Only RAT enabled in preferred PLMN entry is LTE or preferred PLMN is forbidden/backoff forbidden then skip that PLMN. */

         if(sys_mcc_same_country(plmn_mcc, pm_found_mcc) &&
#ifdef FEATURE_DUAL_SIM
           (!reg_sim_per_subs_plmn_forbidden(preferred_plmn_list_p[index].plmn, (sys_modem_as_id_e_type)reg_sub_id))
#else
           (!reg_sim_plmn_forbidden(preferred_plmn_list_p[index].plmn))
#endif
#ifdef FEATURE_LTE
           && !(((preferred_plmn_list_p[index].act[0] & 0x40)==0x40) &&
                (preferred_plmn_list_p[index].act[1] == 0x00) &&
                (reg_sim_plmn_rat_forbidden(preferred_plmn_list_p[index].plmn,
                               SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)reg_sub_id)))
#endif
           ) 
         {
           number_of_pplmn_supported_rats = reg_mode_get_rat_order
                                                           (
                                                             rat_pri_list_info,
                                                             pplmn_supported_rat_order,
                                                             preferred_plmn_list_p[index],
                                                             rat_list_3gpp
                                                           );
           *plmn = preferred_plmn_list_p[index].plmn;
           MSG_HIGH_DS_2(REG_SUB, "=REG= PLMN to start PLMN selection (%d-%d)", plmn_mcc, plmn_mnc);
           reg_mode_available_plmn_list_populated = TRUE;
           reg_mode_reset_available_plmn_list(preferred_plmn_list_p[index].plmn,rat_list_3gpp);
           found = TRUE;
           supported_rats = number_of_pplmn_supported_rats;
           found_index = index;
           break;
         }
       }
       else if(rplmnact_undefined)
       {
         if(sys_plmn_match(preferred_plmn_list_p[index].plmn, *plmn) &&
#ifdef FEATURE_DUAL_SIM
             (!reg_sim_per_subs_plmn_forbidden(preferred_plmn_list_p[index].plmn, (sys_modem_as_id_e_type)reg_sub_id))
#else
             (!reg_sim_plmn_forbidden(preferred_plmn_list_p[index].plmn))
#endif
#ifdef FEATURE_LTE
           && !(((preferred_plmn_list_p[index].act[0] & 0x40)==0x40) &&
                (preferred_plmn_list_p[index].act[1] == 0x00) &&
                (reg_sim_plmn_rat_forbidden(preferred_plmn_list_p[index].plmn,
                               SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)reg_sub_id)))
#endif
           )
         {
           number_of_pplmn_supported_rats = reg_mode_get_rat_order
                                                             (
                                                               rat_pri_list_info,
                                                               pplmn_supported_rat_order,
                                                               preferred_plmn_list_p[index],
                                                               rat_list_3gpp
                                                             );
           MSG_HIGH_DS_0(REG_SUB, "=REG= rplmnact_undefined - Match with SIM preferred PLMN ");
           found = TRUE;
           break;
         }
       }
    }

    if(found == TRUE && (!rplmn_mcc_match || preferred_foreground_search) && (supported_rats < reg_mode_available_plmn_list.length))
    {
      for(index = found_index; index < (int32)total_pplmn_length; index ++)
      {
        if(sys_plmn_id_is_undefined(preferred_plmn_list_p[index].plmn)||
	   	    ((preferred_foreground_search) && sys_plmn_match(*plmn,preferred_plmn_list_p[index].plmn)))
        {
          continue;
        }
        sys_plmn_get_mcc_mnc
        ( preferred_plmn_list_p[index].plmn,
          &plmn_id_is_undefined,
          &mnc_includes_pcs_digit,
          &plmn_mcc,
          &plmn_mnc
        );
         /* If Only RAT enabled in preferred PLMN entry is LTE or preferred PLMN is forbidden/backoff forbidden then skip that PLMN. */

        if(sys_mcc_same_country(plmn_mcc, pm_found_mcc) &&
#ifdef FEATURE_DUAL_SIM
            (!reg_sim_per_subs_plmn_forbidden(preferred_plmn_list_p[index].plmn, (sys_modem_as_id_e_type)reg_sub_id))
#else
             (!reg_sim_plmn_forbidden(preferred_plmn_list_p[index].plmn))
#endif
#ifdef FEATURE_LTE
              && !(((preferred_plmn_list_p[index].act[0] & 0x40)==0x40) &&
                  (preferred_plmn_list_p[index].act[1] == 0x00) &&
                  (reg_sim_plmn_rat_forbidden(preferred_plmn_list_p[index].plmn,
                               SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)reg_sub_id)))
#endif
             ) 
        {
          number_of_pplmn_supported_rats = reg_mode_get_rat_order
                                                           (
                                                             rat_pri_list_info,
                                                             pplmn_supported_rat_order,
                                                             preferred_plmn_list_p[index],
                                                             NULL
                                                           );
             
          for(j= 0; j < number_of_pplmn_supported_rats; j++)
          {
            for(i = supported_rats; i < reg_mode_available_plmn_list.length; i++)
            {
              if(reg_mode_available_plmn_list.info[i].rat == pplmn_supported_rat_order[j])
              {
                if(i > supported_rats)
                {
                  reg_mode_temp_available_plmn = reg_mode_available_plmn_list.info[supported_rats];
                  reg_mode_available_plmn_list.info[supported_rats] = reg_mode_available_plmn_list.info[i];
                  reg_mode_available_plmn_list.info[i] =  reg_mode_temp_available_plmn;
                }
                reg_mode_available_plmn_list.info[supported_rats].plmn = preferred_plmn_list_p[index].plmn;
                supported_rats++;
              }
            }
          }
          if(supported_rats >= reg_mode_available_plmn_list.length)
          {
            break; 
          }
        }

      }
                                                       
    }
    modem_mem_free( preferred_plmn_list_p , MODEM_MEM_CLIENT_NAS);
    if(found == TRUE)
    {
      return; 
    }
    /* Find in NV preffered list */
     preferred_plmn_list_p = reg_nv_get_preferred_plmn_list(&preferred_plmn_list_length);
     if ( preferred_plmn_list_p != NULL && preferred_plmn_list_length > 0)
     {
       for(index = 0; index < (int32)preferred_plmn_list_length; index ++)
       {
          if(sys_plmn_id_is_undefined(preferred_plmn_list_p[index].plmn) ||
	   	     ((preferred_foreground_search) && sys_plmn_match(*plmn,preferred_plmn_list_p[index].plmn)))
          {
            continue;
          }
          if(!rplmn_mcc_match || preferred_foreground_search)
          {
            sys_plmn_get_mcc_mnc
            (
              preferred_plmn_list_p[index].plmn,
              &plmn_id_is_undefined,
              &mnc_includes_pcs_digit,
              &plmn_mcc,
              &plmn_mnc
            );
    
            if(sys_mcc_same_country(plmn_mcc, pm_found_mcc) &&
#ifdef FEATURE_DUAL_SIM
               (!reg_sim_per_subs_plmn_forbidden(preferred_plmn_list_p[index].plmn, (sys_modem_as_id_e_type)reg_sub_id))
#else
               (!reg_sim_plmn_forbidden(preferred_plmn_list_p[index].plmn))
#endif
#ifdef FEATURE_LTE
           && !(((preferred_plmn_list_p[index].act[0] & 0x40)==0x40) &&
                (preferred_plmn_list_p[index].act[1] == 0x00) &&
                (reg_sim_plmn_rat_forbidden(preferred_plmn_list_p[index].plmn,
                               SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)reg_sub_id)))
#endif
              )
            {
                number_of_pplmn_supported_rats = reg_mode_get_rat_order
                                                                (
                                                                  rat_pri_list_info,
                                                                  pplmn_supported_rat_order,
                                                                  preferred_plmn_list_p[index],
                                                                  rat_list_3gpp
                                                                );
                *plmn = preferred_plmn_list_p[index].plmn;
                MSG_HIGH_DS_2(REG_SUB, "=REG= PLMN to start PLMN selection (%d-%d)", plmn_mcc, plmn_mnc);
                reg_mode_reset_available_plmn_list(preferred_plmn_list_p[index].plmn,rat_list_3gpp);
                found = TRUE;
                reg_mode_available_plmn_list_populated = TRUE;
                supported_rats = number_of_pplmn_supported_rats;
                found_index = index;
                break;
            }
          }
          else if(rplmnact_undefined)
          {
            if(sys_plmn_match(preferred_plmn_list_p[index].plmn, *plmn) &&
#ifdef FEATURE_DUAL_SIM
                (!reg_sim_per_subs_plmn_forbidden(preferred_plmn_list_p[index].plmn, (sys_modem_as_id_e_type)reg_sub_id))
#else
                (!reg_sim_plmn_forbidden(preferred_plmn_list_p[index].plmn))
#endif
#ifdef FEATURE_LTE
           && !(((preferred_plmn_list_p[index].act[0] & 0x40)==0x40) &&
                (preferred_plmn_list_p[index].act[1] == 0x00) &&
                (reg_sim_plmn_rat_forbidden(preferred_plmn_list_p[index].plmn,
                               SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)reg_sub_id)))
#endif
              )
            {
              number_of_pplmn_supported_rats = reg_mode_get_rat_order
                                                                (
                                                                  rat_pri_list_info,
                                                                  pplmn_supported_rat_order,
                                                                  preferred_plmn_list_p[index],
                                                                  rat_list_3gpp
                                                                );
             
              MSG_HIGH_DS_0(REG_SUB, "=REG= rplmnact_undefined - Match with NV preferred PLMN ");
              break;
            }
          }
       }
       if(found == TRUE && (!rplmn_mcc_match || preferred_foreground_search) && (supported_rats < reg_mode_available_plmn_list.length))
       {
         for(index = found_index; index < (int32)preferred_plmn_list_length; index ++)
         {
           if(sys_plmn_id_is_undefined(preferred_plmn_list_p[index].plmn) ||
	   	     ((preferred_foreground_search) && sys_plmn_match(*plmn,preferred_plmn_list_p[index].plmn)))
           {
             continue;
           }
           sys_plmn_get_mcc_mnc
           (
             preferred_plmn_list_p[index].plmn,
             &plmn_id_is_undefined,
             &mnc_includes_pcs_digit,
             &plmn_mcc,
             &plmn_mnc
           );
         /* If Only RAT enabled in preferred PLMN entry is LTE or preferred PLMN is forbidden/backoff forbidden then skip that PLMN. */
       
           if(sys_mcc_same_country(plmn_mcc, pm_found_mcc) &&
#ifdef FEATURE_DUAL_SIM
             (!reg_sim_per_subs_plmn_forbidden(preferred_plmn_list_p[index].plmn, (sys_modem_as_id_e_type)reg_sub_id))
#else
             (!reg_sim_plmn_forbidden(preferred_plmn_list_p[index].plmn))
#endif
#ifdef FEATURE_LTE
              && !(((preferred_plmn_list_p[index].act[0] & 0x40)==0x40) &&
                   (preferred_plmn_list_p[index].act[1] == 0x00) &&
                   (reg_sim_plmn_rat_forbidden(preferred_plmn_list_p[index].plmn,
                            SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)reg_sub_id)))
#endif
              ) 
            {
              number_of_pplmn_supported_rats = reg_mode_get_rat_order
                                                        (
                                                          rat_pri_list_info,
                                                          pplmn_supported_rat_order,
                                                          preferred_plmn_list_p[index],
                                                          NULL
                                                        );
          
              for(j= 0; j < number_of_pplmn_supported_rats; j++)
              {
                for(i = supported_rats; i < reg_mode_available_plmn_list.length; i++)
                {
                  if(reg_mode_available_plmn_list.info[i].rat == pplmn_supported_rat_order[j])
                  {
                    if(i > supported_rats)
                    {
                      reg_mode_temp_available_plmn = reg_mode_available_plmn_list.info[supported_rats];
                      reg_mode_available_plmn_list.info[supported_rats] = reg_mode_available_plmn_list.info[i];
                      reg_mode_available_plmn_list.info[i] =  reg_mode_temp_available_plmn;
                    }
                    reg_mode_available_plmn_list.info[supported_rats].plmn = preferred_plmn_list_p[index].plmn;
                    supported_rats++;
                  }
                }
              }
              if(supported_rats >= reg_mode_available_plmn_list.length)
              {
                break; 
              }
           }
       
         }
                                                    
        }
      }
  }
  return;
}

#ifdef FEATURE_LTE
/*==============================================================================
FUNCTION NAME

  reg_mode_update_csfb_failure_rat_search_order_list

DESCRIPTION 
  This function updates csfb_failure_rat_search_order list
  RATs are arranged in decreasing order of RAT priority

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  none
==============================================================================*/
void reg_mode_update_csfb_failure_rat_search_order_list
(
  uint8                          priority,
  sys_radio_access_tech_e_type   rat
)
{
  uint8 i = 0, j = 0;
  boolean found =  FALSE;
  reg_mode_3gpp_csfb_rat_s_type temp;
  
  /* Update the priority of the specified RAT if the RAT is already
     present in the list */
  for(i = 0; i < csfb_failure_rat_search_order.num_items; i++)
  {
    if(csfb_failure_rat_search_order.rat_list[i].rat == rat)
    {
      csfb_failure_rat_search_order.rat_list[i].priority = priority;
      found = TRUE;
      break;
    }
  }

  /* Add the RAT to the list if not already present */
  if((found == FALSE) &&
     (csfb_failure_rat_search_order.num_items < REG_MODE_MAX_3GPP_CSFB_RAT))
  {
    csfb_failure_rat_search_order.rat_list[csfb_failure_rat_search_order.num_items].rat = rat;
    csfb_failure_rat_search_order.rat_list[csfb_failure_rat_search_order.num_items].priority = priority;
    csfb_failure_rat_search_order.num_items++;
  }

  /* Sort the RAT list in decreasing order of priority */
  for(i = 0; i < (csfb_failure_rat_search_order.num_items - 1); i++)
  {
    for(j = 0; j < (csfb_failure_rat_search_order.num_items - 1 - i); j++)
    {
      if(csfb_failure_rat_search_order.rat_list[j].priority < csfb_failure_rat_search_order.rat_list[j+1].priority)
      {
        temp = csfb_failure_rat_search_order.rat_list[j];
        csfb_failure_rat_search_order.rat_list[j] = csfb_failure_rat_search_order.rat_list[j+1];
        csfb_failure_rat_search_order.rat_list[j+1] = temp;        
      }
    }
  }
}

/*==============================================================================
FUNCTION NAME

  reg_mode_remove_rat_from_csfb_failure_rat_search_order_list

DESCRIPTION 
  This function removes a RAT from csfb_failure_rat_search_order list
  from a specified index position

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  none
==============================================================================*/
void reg_mode_remove_rat_from_csfb_failure_rat_search_order_list
(
  uint8 index
)
{
  uint8 i = 0;
  
  if(index >= csfb_failure_rat_search_order.num_items)
  {
    return;
  }

  for(i = index; i < (csfb_failure_rat_search_order.num_items - 1); i++)
  {
    csfb_failure_rat_search_order.rat_list[i] = 
                         csfb_failure_rat_search_order.rat_list[i+1];
  }
  csfb_failure_rat_search_order.num_items--;
}

/*==============================================================================
FUNCTION NAME

  reg_mode_process_rrc_sib_info_ind

DESCRIPTION 
  This function processes SIB6 and SIB7 info received from AS.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  none
==============================================================================*/

void reg_mode_process_rrc_sib_info_ind
(
  reg_cmd_type *reg_cmd_ptr
)
{
  uint8 i = 0;
  mmr_rrc_sib6_info_ind_s_type *sib6_info_ind_ptr = NULL;
  mmr_rrc_sib7_info_ind_s_type *sib7_info_ind_ptr = NULL;

  switch(reg_cmd_ptr->cmd.hdr.message_id)
  {
    case MMR_RRC_SIB6_INFO_IND:
      sib6_info_ind_ptr = (mmr_rrc_sib6_info_ind_s_type*) &(reg_cmd_ptr->cmd.mmr_rrc_sib6_info_ind);
      MSG_HIGH_DS_1(REG_SUB, "sib6_present = %d", sib6_info_ind_ptr->sib6_present);

      if(sib6_info_ind_ptr->sib6_present == TRUE)
      {
        MSG_HIGH_DS_2(REG_SUB, "priority_UTRAN_FDD = %d, priority_UTRAN_TDD = %d",
                      sib6_info_ind_ptr->priority_UTRAN_FDD,
                      sib6_info_ind_ptr->priority_UTRAN_TDD);

        /* Remove WCDMA and TDS RATs from csfb_failure_rat_search_order list
           if SIB6 Info is received with INVALID SIB Priority */
        for(i = 0; i < csfb_failure_rat_search_order.num_items; i++)
        {
          if(((csfb_failure_rat_search_order.rat_list[i].rat == SYS_RAT_UMTS_RADIO_ACCESS) &&
              (sib6_info_ind_ptr->priority_UTRAN_FDD == MMR_LTE_RRC_INVALID_SIB_PRIORITY))
             ||
             ((csfb_failure_rat_search_order.rat_list[i].rat == SYS_RAT_TDS_RADIO_ACCESS) &&
              (sib6_info_ind_ptr->priority_UTRAN_TDD == MMR_LTE_RRC_INVALID_SIB_PRIORITY))
            )
          {
            reg_mode_remove_rat_from_csfb_failure_rat_search_order_list(i);
          }
        }
        
        /* Update RAT priority for WCDMA RAT*/
        if(sib6_info_ind_ptr->priority_UTRAN_FDD != MMR_LTE_RRC_INVALID_SIB_PRIORITY)
        {
           reg_mode_update_csfb_failure_rat_search_order_list(
                 (uint8)(sib6_info_ind_ptr->priority_UTRAN_FDD), SYS_RAT_UMTS_RADIO_ACCESS);
        }

        /* Update RAT priority for TDS RAT*/
        if(sib6_info_ind_ptr->priority_UTRAN_TDD != MMR_LTE_RRC_INVALID_SIB_PRIORITY)
        {
           reg_mode_update_csfb_failure_rat_search_order_list(
                 (uint8)(sib6_info_ind_ptr->priority_UTRAN_TDD), SYS_RAT_TDS_RADIO_ACCESS);
        }
      }
      /* else remove WCDMA and TDS RATs, if present from csfb_failure_rat_search_order list */
      else
      {
        for(i = 0; i < csfb_failure_rat_search_order.num_items; i++)
        {
          if((csfb_failure_rat_search_order.rat_list[i].rat == SYS_RAT_UMTS_RADIO_ACCESS) ||
             (csfb_failure_rat_search_order.rat_list[i].rat == SYS_RAT_TDS_RADIO_ACCESS))
          {
            reg_mode_remove_rat_from_csfb_failure_rat_search_order_list(i);
          }
        }
      }
      break;

    case MMR_RRC_SIB7_INFO_IND:
      sib7_info_ind_ptr = (mmr_rrc_sib7_info_ind_s_type*) &(reg_cmd_ptr->cmd.mmr_rrc_sib7_info_ind);
      MSG_HIGH_DS_1(REG_SUB, "sib7_present = %d", sib7_info_ind_ptr->sib7_present);

      if(reg_cmd_ptr->cmd.mmr_rrc_sib7_info_ind.sib7_present == TRUE)
      {
        MSG_HIGH_DS_1(REG_SUB, "priority = %d", sib7_info_ind_ptr->priority);

        /* Remove GSM RAT from csfb_failure_rat_search_order list
           if SIB7 Info is received with INVALID SIB Priority */
        for(i = 0; i < csfb_failure_rat_search_order.num_items; i++)
        {
          if((csfb_failure_rat_search_order.rat_list[i].rat == SYS_RAT_GSM_RADIO_ACCESS) &&
             (sib7_info_ind_ptr->priority == MMR_LTE_RRC_INVALID_SIB_PRIORITY))
          {
            reg_mode_remove_rat_from_csfb_failure_rat_search_order_list(i);
            break;
          }
        }

        /* Update RAT priority for GSM RAT*/
        if(sib7_info_ind_ptr->priority != MMR_LTE_RRC_INVALID_SIB_PRIORITY)
        {
           reg_mode_update_csfb_failure_rat_search_order_list(
                 (uint8)(sib7_info_ind_ptr->priority), SYS_RAT_GSM_RADIO_ACCESS);
        }
      }
      /* else remove GSM RAT, if present from csfb_failure_rat_search_order list */
      else
      {
        for(i = 0; i < csfb_failure_rat_search_order.num_items; i++)
        {
          if(csfb_failure_rat_search_order.rat_list[i].rat == SYS_RAT_GSM_RADIO_ACCESS)
          {
            reg_mode_remove_rat_from_csfb_failure_rat_search_order_list(i);
            break;
          }
        }
      }
      break;

    default:
      MSG_ERROR_DS_1(REG_SUB, "Unexpected message %d received",reg_cmd_ptr->cmd.hdr.message_id);
      return;
  }


  /* Output the final list */
  MSG_HIGH_DS_7(REG_SUB,"Num items = %d. RAT1 = %d: pri = %d, RAT2 = %d: pri = %d, RAT3 = %d: pri = %d",
                csfb_failure_rat_search_order.num_items,
                csfb_failure_rat_search_order.rat_list[0].rat,
                csfb_failure_rat_search_order.rat_list[0].priority,
                csfb_failure_rat_search_order.rat_list[1].rat,
                csfb_failure_rat_search_order.rat_list[1].priority,
                csfb_failure_rat_search_order.rat_list[2].rat,
                csfb_failure_rat_search_order.rat_list[2].priority);
}
#endif
boolean reg_mode_get_available_plmn_list_populated(void)
{
 return reg_mode_available_plmn_list_populated;
}
void reg_mode_set_available_plmn_list_populated(boolean value)
{
  reg_mode_available_plmn_list_populated = value;
}


#ifdef FEATURE_QSH_DUMP
void get_dump_reg_mode_vars (reg_debug_data_s_type* ptr, reg_as_id_e_type as_id)
{
    ASSERT (ptr != NULL);
#if defined (FEATURE_DUAL_SIM) || defined (FEATURE_SGLTE)
    ptr->reg_mode_hplmn_to_be_selected_debug = reg_mode_hplmn_to_be_selected_sim[as_id];
#else
    ptr->reg_mode_hplmn_to_be_selected_debug = reg_mode_hplmn_to_be_selected;
#endif
    ptr->reg_mode_lte_retry_counter_debug = reg_mode_lte_retry_counter;
#
#if defined (FEATURE_DUAL_SIM) || defined (FEATURE_SGLTE)
    ptr->reg_mode_network_selection_mode_debug = reg_mode_network_selection_mode_sim[as_id];
    ptr->reg_mode_rat_pri_list_debug = reg_mode_rat_pri_list_sim[as_id];
    ptr->reg_mode_req_plmn_debug = reg_mode_req_plmn_sim[as_id];
    ptr->reg_mode_req_rat_debug = reg_mode_req_rat_sim[as_id];

    ptr->reg_mode_req_service_domain_debug = reg_mode_req_service_domain_sim[as_id];
    ptr->reg_mode_scan_scope_debug = reg_mode_scan_scope_sim[as_id];
    ptr->reg_mode_service_available_rat_debug = reg_mode_service_available_rat_sim[as_id];
    ptr->reg_mode_validate_sim_counter_debug = reg_mode_validate_sim_counter_sim[as_id];

#else
    ptr->reg_mode_network_selection_mode_debug = reg_mode_network_selection_mode;
    ptr->reg_mode_rat_pri_list_debug = reg_mode_rat_pri_list;
    ptr->reg_mode_req_plmn_debug = reg_mode_req_plmn;
    ptr->reg_mode_req_rat_debug = reg_mode_req_rat;

    ptr->reg_mode_req_service_domain_debug = reg_mode_req_service_domain;
    ptr->reg_mode_scan_scope_debug = reg_mode_scan_scope;
    ptr->reg_mode_service_available_rat_debug = reg_mode_service_available_rat;
    ptr->reg_mode_validate_sim_counter_debug = reg_mode_validate_sim_counter;
#endif
#if defined(FEATURE_SVLTE_DUAL_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
    ptr->reg_mode_service_req_aborted_debug = reg_mode_service_req_aborted_sim[as_id];
#endif
#ifdef FEATURE_LTE
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
    ptr->reg_mode_sms_only_debug = reg_mode_sms_only_sim[as_id];
#endif
#endif
#ifdef FEATURE_SVLTE_DUAL_SIM
    ptr->reg_mode_timed_service_req_debug = reg_mode_timed_service_req_sim[as_id];
#endif
#ifdef FEATURE_DUAL_SIM
    ptr->reg_mode_usim_compliant_debug = reg_mode_usim_compliant_sim[as_id];
#endif
    
}
#endif

#ifdef FEATURE_NAS_ENABLE_LTE_BAND_FILTERING
/*==============================================================================
FUNCTION NAME

  reg_mode_is_lte_band_filtering_applicable

DESCRIPTION 
  This function checks if LTE TDD and FDD bands need to be filtered.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  none
==============================================================================*/
boolean reg_mode_is_lte_band_filtering_applicable
(
  const sys_rat_pri_list_info_s_type  *rat_list
)
{
  boolean result = FALSE;

  /*
    Apply the band filtering only if:
    1. EFS is set to CMCC device type 2
    2. UE camps in CMCC network
      (note: PLMN might not be available at this stage. PLMN should be checked later - while
             sending MMR_REG_REQ)
    3. LTE RAT must be in RAT priority list
  */


  if( (reg_nv_td_op_device_type_get() == (uint8)REG_TD_OP_DEVICE_TYPE_2) &&
      (reg_mode_is_usim_compliant() == TRUE)  &&
      (reg_mode_is_rat_present_in_rat_pri_list(SYS_RAT_LTE_RADIO_ACCESS, rat_list, TRUE) ) )
  {
    result = TRUE;
  }
  MSG_HIGH_DS_2(MM_SUB, "=MM= reg_nv_td_op_device_type_get set to %d reg_mode_is_lte_band_filtering_applicable() returns %d", reg_nv_td_op_device_type_get(),result);
  return result;
}

void reg_mode_update_lte_scan_bands(sys_rat_pri_list_info_s_type  *rat_list, reg_mode_lte_scan_e_type lte_band_type)
{
   int index;
   sys_lte_band_mask_e_type band_cap = SYS_LTE_BAND_MASK_CONST_NONE;
   sys_rat_pri_list_info_s_type  *orig_rat_list = reg_mode_rat_pri_list_get();

   if ( (lte_band_type <= REG_MODE_LTE_SCAN_NONE) || (lte_band_type >= REG_MODE_LTE_SCAN_MAX) )
   {
     MSG_HIGH_DS_0(REG_SUB,"=REG= Invalid LTE band type");
     return;
   }

   for (index = 0; index < (int)rat_list->num_items; index++)
   {
     if( (SYS_SYS_MODE_LTE == rat_list->priority_list_info[index].acq_sys_mode) &&
         (rat_list->priority_list_info[index].bst_rat_acq_required == TRUE) )
     {
        /*Get LTE TDD only bands*/
        band_cap = lte_rrc_lte_tdd_only_band_preference(orig_rat_list->priority_list_info[index].bst_band_cap.lte_band_cap);
        break;
     }
     /* If LTE is not requested, return*/
     else if (index == ((int)rat_list->num_items - 1 ) )
     {
        MSG_HIGH_DS_0(REG_SUB,"=REG= LTE not in RAT list");
        return;
     }
   }

   /*Do not apply L-TDD and L-FDD band filtering if only L-FDD or only L-TDD bands are available. */
   if (SYS_LTE_BAND_MASK_CHK_IF_EMPTY(&band_cap) ||
       SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&(orig_rat_list->priority_list_info[index].bst_band_cap.lte_band_cap), &band_cap))
   {
      MSG_HIGH_DS_0(REG_SUB,"=REG= Only L-TDD or only L-FDD are enabled. No LTE band filtering is applied.");
      reg_mode_lte_scan_pref_set(REG_MODE_LTE_SCAN_NONE);
      return;
   }

   if (lte_band_type == REG_MODE_LTE_SCAN_TDD)
   {
     rat_list->priority_list_info[index].bst_band_cap.lte_band_cap = band_cap;
   }
   else
   {
     /*Filter FDD bands from original band cap */
     rat_list->priority_list_info[index].bst_band_cap.lte_band_cap = orig_rat_list->priority_list_info[index].bst_band_cap.lte_band_cap;
     SYS_LTE_BAND_MASK_REMOVE_MASK(&(rat_list->priority_list_info[index].bst_band_cap.lte_band_cap), &band_cap);
   }
   
   return;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_update_available_plmn_list

==============================================================================*/

void reg_mode_append_available_plmn_list
(
  sys_detailed_plmn_list_s_type* available_plmn_list,
  sys_plmn_id_s_type                   plmn,
  sys_radio_access_tech_e_type         rat
)
{
  boolean                  found_plmn_rat = FALSE;
  uint32                   i;

  for (i = 0; i < available_plmn_list->length; i++)
  {
    if ( (sys_plmn_match(plmn, available_plmn_list->info[i].plmn) ) &&
         (available_plmn_list->info[i].rat == rat) ) 
    {
       found_plmn_rat = TRUE;
       break;
    }
  }
       
  /*
  ** Add the new PLMN-RAT combination to the available PLMN list.
  */
  if (found_plmn_rat &&
      (reg_sim_plmn_rat_forbidden(plmn, rat, (sys_modem_as_id_e_type)reg_sub_id) == FALSE) )
  {
    if (reg_mode_available_plmn_list.length < SYS_PLMN_LIST_MAX_LENGTH)
    {
      reg_mode_available_plmn_list.info[reg_mode_available_plmn_list.length] =
             available_plmn_list->info[i];

      reg_mode_available_plmn_list.length++;
    }
    else
    {
      MSG_HIGH_DS_1(REG_SUB, "=REG= Cannot add to Available PLMN list (length = %u)", reg_mode_available_plmn_list.length);
      return;
    }
  }
  else
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= Cannot append forbidden PLMN - RAT to Available list");
    return;
  }

  reg_mode_output_detailed_plmn_list
  (
    &reg_mode_available_plmn_list
#ifdef FEATURE_FEMTO_CSG
    , FALSE
#endif 
  );
}
#endif
/*==============================================================================

FUNCTION NAME

  reg_mode_add_ehplmn_temp_forbidden

==============================================================================*/

void reg_mode_add_ehplmn_temp_forbidden
(
	sys_plmn_id_s_type		plmn
#ifdef FEATURE_DUAL_SIM
		   ,sys_modem_as_id_e_type	as_id
#endif

)
{

byte count;

#ifdef FEATURE_DUAL_SIM
	if ( reg_mode_is_ehplmn_forbidden(plmn,(sys_modem_as_id_e_type)as_id) == FALSE )
	{
		if ( reg_mode_temp_forbidden_list_sim[as_id].length == MAX_TEMP_LIST )
			 {
				 for(count = 0; (count < reg_mode_temp_forbidden_list_sim[as_id].length-1) && (count < MAX_TEMP_LIST-1); count++)
				 {
					(void)memscpy( &reg_mode_temp_forbidden_list_sim[as_id].plmn[count], PLMN_SIZE,
								  &reg_mode_temp_forbidden_list_sim[as_id].plmn[count+1], PLMN_SIZE );
				 }
				 memscpy(&reg_mode_temp_forbidden_list_sim[as_id].plmn[reg_mode_temp_forbidden_list_sim[as_id].length-1], PLMN_SIZE,
							  &plmn, PLMN_SIZE );
		
			 }
			 else
			 {
				(void)memscpy(&reg_mode_temp_forbidden_list_sim[as_id].plmn[reg_mode_temp_forbidden_list_sim[as_id].length], PLMN_SIZE,
							  &plmn, PLMN_SIZE );
				reg_mode_temp_forbidden_list_sim[as_id].length = reg_mode_temp_forbidden_list_sim[as_id].length + 1;
			 }

	}
#else
   if ( reg_mode_is_ehplmn_forbidden(plmn) == FALSE )
  {
     if ( reg_mode_temp_forbidden_list.length == MAX_TEMP_LIST )
	 {
	     for(count = 0; (count < reg_mode_temp_forbidden_list.length-1) && (count < MAX_TEMP_LIST-1); count++)
         {
            (void)memscpy( &reg_mode_temp_forbidden_list.plmn[count], PLMN_SIZE,
                          &reg_mode_temp_forbidden_list.plmn[count+1], PLMN_SIZE );
         }
         memscpy(&reg_mode_temp_forbidden_list.plmn[reg_mode_temp_forbidden_list.length-1], PLMN_SIZE,
					  &plmn, PLMN_SIZE );

	 }
	 else
	 {
		(void)memscpy(&reg_mode_temp_forbidden_list.plmn[reg_mode_temp_forbidden_list.length], PLMN_SIZE,
					  &plmn, PLMN_SIZE );
		reg_mode_temp_forbidden_list.length = reg_mode_temp_forbidden_list.length + 1;
	 }
  }
#endif

  
}

void reg_mode_remove_ehplmn_temp_forbidden
(
	sys_plmn_id_s_type		plmn
#ifdef FEATURE_DUAL_SIM
		   ,sys_modem_as_id_e_type	as_id
#endif

)
{
  byte  count;
#ifdef FEATURE_DUAL_SIM
  for (count = 0; count < reg_mode_temp_forbidden_list_sim[as_id].length; count++ )
  {
	 if (memcmp( &reg_mode_temp_forbidden_list_sim[as_id].plmn[count],
				 &plmn,
				 PLMN_SIZE ) == 0 )	  /* if match found */
	 {

		/*
		 * remove entry, and shuffle up
		 */
		for ( ; (count < reg_mode_temp_forbidden_list_sim[as_id].length-1) && (count < MAX_NO_FORBIDDEN_LAIS-1); count++ )
		{
		   (void)memscpy( &reg_mode_temp_forbidden_list_sim[as_id].plmn[count],PLMN_SIZE,
				   &reg_mode_temp_forbidden_list_sim[as_id].plmn[count+1], PLMN_SIZE );
		}

		reg_mode_temp_forbidden_list_sim[as_id].length = (byte)(reg_mode_temp_forbidden_list_sim[as_id].length - 1) ;

	 }
  }
#else
  for (count = 0; count < reg_mode_temp_forbidden_list.length; count++ )
  {
	 if (memcmp( &reg_mode_temp_forbidden_list.plmn[count],
				 &plmn,
				 PLMN_SIZE ) == 0 )	  /* if match found */
	 {

		/*
		 * remove entry, and shuffle up
		 */
		for ( ; (count < reg_mode_temp_forbidden_list.length-1) && (count < MAX_NO_FORBIDDEN_LAIS-1); count++ )
		{
		   (void)memscpy( &reg_mode_temp_forbidden_list.plmn[count],PLMN_SIZE,
				   &reg_mode_temp_forbidden_list.plmn[count+1], PLMN_SIZE );
		}

		reg_mode_temp_forbidden_list.length = (byte)(reg_mode_temp_forbidden_list.length - 1) ;

	 }
  }
#endif
}
boolean reg_mode_is_ehplmn_forbidden
(
	sys_plmn_id_s_type		plmn
#ifdef FEATURE_DUAL_SIM
	   ,sys_modem_as_id_e_type	as_id
#endif


)
{
	byte	 count;
	boolean  return_value = FALSE;
#ifdef FEATURE_DUAL_SIM
    if(reg_nv_is_ehplmn_forbid_enabled(as_id) == FALSE)
#else
	if(reg_nv_is_ehplmn_forbid_enabled() == FALSE)
#endif
    {
		return return_value;
    }
	
#ifdef FEATURE_DUAL_SIM	
	  for (count = 0; count < reg_mode_temp_forbidden_list_sim[as_id].length; count++ )
#else
	  for (count = 0; count < reg_mode_temp_forbidden_list.length; count++ )
#endif
	  {
#ifdef FEATURE_DUAL_SIM
  		if (memcmp( &reg_mode_temp_forbidden_list_sim[as_id].plmn[count],
                  &plmn,
                  PLMN_SIZE ) == 0 )    /* if match found */
#else
  		if (memcmp( &reg_mode_temp_forbidden_list.plmn[count],
                  &plmn,
                  PLMN_SIZE ) == 0 )    /* if match found */

#endif
      	{
         return_value = TRUE;
         break;
      	}
	  }
	 return return_value;
}


void reg_mode_clear_ehplmn_forbidden
(
#ifdef FEATURE_DUAL_SIM
		   sys_modem_as_id_e_type	as_id
#endif
)
{
  MSG_HIGH_DS_0(REG_SUB, "=REG= Power ON, Clear temp Forbidden list");
#ifdef FEATURE_DUAL_SIM
if(reg_mode_temp_forbidden_list_sim[as_id].length !=0)
{
  reg_mode_temp_forbidden_list_sim[as_id].length=0;
}

#else
  if(reg_mode_temp_forbidden_list.length !=0)
  {
	reg_mode_temp_forbidden_list.length=0;
  }
#endif

}



void reg_mode_add_ehplmn_temp_gprs_forbidden
(
	sys_plmn_id_s_type		plmn
#ifdef FEATURE_DUAL_SIM
	   ,sys_modem_as_id_e_type	as_id
#endif
)
{
byte count;

#ifdef FEATURE_DUAL_SIM
  if ( reg_mode_is_ehplmn_gprs_forbidden(plmn,as_id) == FALSE )
  {
     if ( reg_mode_temp_gprs_forbidden_list_sim[as_id].length == MAX_TEMP_LIST )
	 {
	     for(count = 0; (count < reg_mode_temp_forbidden_list_sim[as_id].length-1) && (count < MAX_TEMP_LIST-1); count++)
         {
            (void)memscpy( &reg_mode_temp_gprs_forbidden_list_sim[as_id].plmn[count], PLMN_SIZE,
                          &reg_mode_temp_gprs_forbidden_list_sim[as_id].plmn[count+1], PLMN_SIZE );
         }
         memscpy(&reg_mode_temp_gprs_forbidden_list_sim[as_id].plmn[reg_mode_temp_gprs_forbidden_list_sim[as_id].length-1], PLMN_SIZE,
					  &plmn, PLMN_SIZE );

	 }
	 else
	 {
		(void)memscpy(&reg_mode_temp_gprs_forbidden_list_sim[as_id].plmn[reg_mode_temp_gprs_forbidden_list_sim[as_id].length], PLMN_SIZE,
					  &plmn, PLMN_SIZE );
		reg_mode_temp_gprs_forbidden_list_sim[as_id].length = reg_mode_temp_gprs_forbidden_list_sim[as_id].length + 1;
	 }
  }


#else
 if ( reg_mode_is_ehplmn_gprs_forbidden(plmn) == FALSE )
  {
     if ( reg_mode_temp_gprs_forbidden_list.length == MAX_TEMP_LIST )
	 {
	     for(count = 0; (count < reg_mode_temp_forbidden_list.length-1) && (count < MAX_TEMP_LIST-1); count++)
         {
            (void)memscpy( &reg_mode_temp_gprs_forbidden_list.plmn[count], PLMN_SIZE,
                          &reg_mode_temp_gprs_forbidden_list.plmn[count+1], PLMN_SIZE );
         }
         memscpy(&reg_mode_temp_gprs_forbidden_list.plmn[reg_mode_temp_gprs_forbidden_list.length-1], PLMN_SIZE,
					  &plmn, PLMN_SIZE );

	 }
	 else
	 {
		(void)memscpy(&reg_mode_temp_gprs_forbidden_list.plmn[reg_mode_temp_gprs_forbidden_list.length], PLMN_SIZE,
					  &plmn, PLMN_SIZE );
		reg_mode_temp_gprs_forbidden_list.length = reg_mode_temp_gprs_forbidden_list.length + 1;
	 }
  }

#endif

}

void reg_mode_remove_ehplmn_temp_gprs_forbidden
(
	sys_plmn_id_s_type		plmn
#ifdef FEATURE_DUAL_SIM
		   ,sys_modem_as_id_e_type	as_id
#endif

)
{
  byte  count;
#ifdef FEATURE_DUAL_SIM
  for (count = 0; count < reg_mode_temp_gprs_forbidden_list_sim[as_id].length; count++ )
  {
	 if (memcmp( &reg_mode_temp_gprs_forbidden_list_sim[as_id].plmn[count],
				 &plmn,
				 PLMN_SIZE ) == 0 )	  /* if match found */
	 {

		/*
		 * remove entry, and shuffle up
		 */
		for ( ; (count < reg_mode_temp_gprs_forbidden_list_sim[as_id].length-1) && (count < MAX_NO_FORBIDDEN_LAIS-1); count++ )
		{
		   (void)memscpy( &reg_mode_temp_gprs_forbidden_list_sim[as_id].plmn[count],PLMN_SIZE,
				   &reg_mode_temp_gprs_forbidden_list_sim[as_id].plmn[count+1], PLMN_SIZE );
		}

		reg_mode_temp_gprs_forbidden_list_sim[as_id].length = (byte)(reg_mode_temp_gprs_forbidden_list_sim[as_id].length - 1) ;

	 }
  }
#else
  for (count = 0; count < reg_mode_temp_gprs_forbidden_list.length; count++ )
  {
	 if (memcmp( &reg_mode_temp_gprs_forbidden_list.plmn[count],
				 &plmn,
				 PLMN_SIZE ) == 0 )	  /* if match found */
	 {

		/*
		 * remove entry, and shuffle up
		 */
		for ( ; (count < reg_mode_temp_gprs_forbidden_list.length-1) && (count < MAX_NO_FORBIDDEN_LAIS-1); count++ )
		{
		   (void)memscpy( &reg_mode_temp_gprs_forbidden_list.plmn[count],PLMN_SIZE,
				   &reg_mode_temp_gprs_forbidden_list.plmn[count+1], PLMN_SIZE );
		}

		reg_mode_temp_gprs_forbidden_list.length = (byte)(reg_mode_temp_gprs_forbidden_list.length - 1) ;

	 }
  }
#endif
}

boolean reg_mode_is_ehplmn_gprs_forbidden
(
	sys_plmn_id_s_type		plmn
#ifdef FEATURE_DUAL_SIM
	   ,sys_modem_as_id_e_type	as_id
#endif	
)
{
	byte	 count;
	boolean  return_value = FALSE;
	
#ifdef FEATURE_DUAL_SIM
    if(reg_nv_is_ehplmn_forbid_enabled(as_id) == FALSE)
#else
	if(reg_nv_is_ehplmn_forbid_enabled() == FALSE)
#endif
    {
		return return_value;
    }
#ifdef FEATURE_DUAL_SIM
    for (count = 0; count < reg_mode_temp_gprs_forbidden_list_sim[as_id].length; count++ )
	{
	  if (memcmp( &reg_mode_temp_gprs_forbidden_list_sim[as_id].plmn[count],
				&plmn,
				PLMN_SIZE ) == 0 )	  /* if match found */
	  {
	   return_value = TRUE;
	   break;
	  }
    }
#else
	  for (count = 0; count < reg_mode_temp_gprs_forbidden_list.length; count++ )
	  {
  		if (memcmp( &reg_mode_temp_gprs_forbidden_list.plmn[count],
                  &plmn,
                  PLMN_SIZE ) == 0 )    /* if match found */
      	{
         return_value = TRUE;
         break;
      	}
	  }
#endif
      
	 return return_value;
}


void reg_mode_clear_ehplmn_gprs_forbidden
(
#ifdef FEATURE_DUAL_SIM
		  sys_modem_as_id_e_type	as_id
#endif
)
{
  MSG_HIGH_DS_0(REG_SUB, "=REG= Power ON, Clear temp Forbidden list");
#ifdef FEATURE_DUAL_SIM
 if(reg_mode_temp_gprs_forbidden_list_sim[as_id].length !=0)
 {
	reg_mode_temp_gprs_forbidden_list_sim[as_id].length=0;
 }

#else
  if(reg_mode_temp_gprs_forbidden_list.length !=0)
  {
	reg_mode_temp_gprs_forbidden_list.length=0;
  }
#endif
}

/*
**Function to check if HPLMN search has been performed on all RATs in HPLMN RAT search order
**returns TRUE if search is not completed
*/
boolean reg_mode_hplmn_timer_restart( const sys_detailed_plmn_list_s_type*       found_list)
{
    boolean result = FALSE;
    boolean plmn_found = FALSE;
    sys_plmn_id_s_type current_plmn;
    sys_radio_access_tech_e_type current_rat;
    uint32 i,j;





/* Compare the search list sent to lower layers and the current available found_list*/
/* If the first N-1 plmn-rat combination is present in the found list, then consider search as incomplete and return TRUE*/


  sys_plmn_rat_list_s_type *searched_plmn_list = NULL;
#ifdef FEATURE_MODEM_HEAP
  searched_plmn_list = (sys_plmn_rat_list_s_type *)modem_mem_calloc(1, sizeof(sys_plmn_rat_list_s_type), MODEM_MEM_CLIENT_NAS);
#else
  searched_plmn_list = (sys_plmn_rat_list_s_type *)gs_alloc(sizeof(sys_plmn_rat_list_s_type));
#endif
  reg_check_for_null_ptr((void*)searched_plmn_list );
  searched_plmn_list->length = 0;






//extract the searched_plmn_list, considering only ehplmns
   if (reg_mode_hplmn_available
        (
#if defined(FEATURE_GTOW_BACKGROUND_PLMN_SEARCH ) || defined(FEATURE_RAT_PRIORITY_LIST)
                reg_state_active_rat_get(),
                searched_plmn_list
#endif
        ))
  {



    //compare with found plmn list n-1 entries of searched_plmn_list
    for ( i=0 ; i < searched_plmn_list->length-1; i++)
    {
       current_plmn = searched_plmn_list->info[i].plmn;
       current_rat = searched_plmn_list->info[i].rat;

      //for back-off or grps forbidden plmn-rat combos
      if(sys_plmn_id_is_undefined(current_plmn))
        continue;
      
      for ( j=0 ; j < found_list->length ; j++)
      {
        if(sys_plmn_match(found_list->info[j].plmn , current_plmn) && current_rat == found_list->info[j].rat )
        {
          result = TRUE;
          plmn_found = TRUE; 
          break;
        }
      }
      if(plmn_found)
      {
        break;
      }
    }

  }
  else
  {
   //No search was done. ideally this case should not be hit anywhere
    result = FALSE;
  }


    modem_mem_free(searched_plmn_list, MODEM_MEM_CLIENT_NAS);


  return result;

}

/*==============================================================================

FUNCTION NAME

  reg_mode_reset_oos_search_done_flags

==============================================================================*/

void reg_mode_reset_oos_search_done_flags(
#ifdef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type	as_id
#endif
)
{
#ifdef FEATURE_DUAL_SIM
  reg_mode_oos_srch_done_sim[as_id][SYS_RAT_GSM_RADIO_ACCESS]  = FALSE;
  reg_mode_oos_srch_done_sim[as_id][SYS_RAT_UMTS_RADIO_ACCESS] = FALSE;
  reg_mode_oos_srch_done_sim[as_id][SYS_RAT_TDS_RADIO_ACCESS]  = FALSE;
#ifdef FEATURE_LTE
  reg_mode_oos_srch_done_sim[as_id][SYS_RAT_LTE_RADIO_ACCESS]  = FALSE; 
#endif  
  
#else
  reg_mode_oos_srch_done[SYS_RAT_GSM_RADIO_ACCESS]  = FALSE;
  reg_mode_oos_srch_done[SYS_RAT_UMTS_RADIO_ACCESS] = FALSE;
  reg_mode_oos_srch_done[SYS_RAT_TDS_RADIO_ACCESS]  = FALSE;
#ifdef FEATURE_LTE
  reg_mode_oos_srch_done[SYS_RAT_LTE_RADIO_ACCESS]  = FALSE; 
#endif  

#endif

  MSG_HIGH_DS_0(REG_SUB, "=REG= Reset OOS search done flags to 0");
  return;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_update_search_done_flag

==============================================================================*/
void reg_mode_update_oos_search_done_flag
(
  boolean flag, 
  sys_radio_access_tech_e_type rat
#ifdef FEATURE_DUAL_SIM
  ,sys_modem_as_id_e_type       as_id
#endif
)
{
  if(sys_radio_access_tech_is_valid(rat)) 
  {
#ifdef FEATURE_DUAL_SIM
    reg_mode_oos_srch_done_sim[as_id][rat]=flag;
#else
    reg_mode_oos_srch_done[rat]=flag;
#endif
    MSG_HIGH_DS_2(REG_SUB,"=REG= Updated OOS search done flag (%d) for RAT: %d", flag, rat); 
  }
  else
  {
    MSG_HIGH_DS_2(REG_SUB, "=REG= Invalid RAT.  Flag (%d) RAT: %d", flag, rat); 
  }

  return;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_get_rat_search_done_flag

==============================================================================*/
boolean reg_mode_get_rat_search_done_flag
(
  sys_radio_access_tech_e_type rat
#ifdef FEATURE_DUAL_SIM
  ,sys_modem_as_id_e_type       as_id
#endif
)
{
  boolean search_done = FALSE;
  if(sys_radio_access_tech_is_valid(rat)) 
  {
#ifdef FEATURE_DUAL_SIM
    search_done = reg_mode_oos_srch_done_sim[as_id][rat];
#else
    search_done = reg_mode_oos_srch_done[rat];
#endif
    MSG_HIGH_DS_2(REG_SUB,"=REG= OOS search done flag (%d) for RAT: %d", search_done, rat); 
  }
  else
  {
    MSG_HIGH_DS_2(REG_SUB, "=REG= Invalid RAT.  Flag (%d) RAT: %d", search_done, rat); 
  }

  return search_done;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_convert_access_tech_to_acq_mode

==============================================================================*/

sys_sys_mode_e_type reg_mode_convert_access_tech_to_acq_mode
(
   sys_radio_access_tech_e_type ac_tech_mode
)
{
  sys_sys_mode_e_type rat = SYS_SYS_MODE_NONE;

  switch(ac_tech_mode)
  {
    case SYS_RAT_GSM_RADIO_ACCESS:
      rat = SYS_SYS_MODE_GSM;
      break;
    case SYS_RAT_UMTS_RADIO_ACCESS:
      rat = SYS_SYS_MODE_WCDMA;
      break;
    case SYS_RAT_TDS_RADIO_ACCESS:
      rat = SYS_SYS_MODE_TDS;
      break;     
    case SYS_RAT_LTE_RADIO_ACCESS:
      rat = SYS_SYS_MODE_LTE;
      break;     
#ifdef FEATURE_NR5G_SA
    case SYS_RAT_NR5G_RADIO_ACCESS:
      rat = SYS_SYS_MODE_NR5G;
      break;
#endif
    default :
      MSG_HIGH_DS_1(REG_SUB,"=REG= default acq_sys_mode is %d",ac_tech_mode);
      break;
  }      
  return rat;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_filter_background_plmn_list

==============================================================================*/

void reg_mode_filter_background_plmn_list
(
  void
)
{
   uint32 i = 0;
   uint32 j = 0;
   uint32 k = 0;
   sys_rat_pri_list_info_s_type rat_pri_list_info = reg_state_manual_rat_pri_list_info_get();

	for(i=1; i <= reg_mode_backgnd_list->length && i<= SYS_PLMN_LIST_MAX_LENGTH; i++)
	   {
		for(j=0; j < rat_pri_list_info.num_items && j< SYS_MAX_ACQ_PREF_RAT_LIST_NUM; j++)
			{
				if(reg_mode_convert_access_tech_to_acq_mode(reg_mode_backgnd_list->info[i-1].rat) == rat_pri_list_info.priority_list_info[j].acq_sys_mode)
					{
						break;
					}
			}
		if(j >= rat_pri_list_info.num_items)
			{
                MSG_HIGH_DS_1(REG_SUB, "=REG= Background list rat %d does not match with rat priority list rat", reg_mode_convert_access_tech_to_acq_mode(reg_mode_backgnd_list->info[i-1].rat));
			    reg_mode_backgnd_list->length = reg_mode_backgnd_list->length - 1;
			    for(k=i; k < reg_mode_backgnd_list->length; k++)
				   {
					   reg_mode_backgnd_list->info[k-1] = reg_mode_backgnd_list->info[k];
				   }
				   i--;
			}	
		}
}

/*==============================================================================

FUNCTION NAME

  reg_mode_filter_available_plmn_list_with_rat_bst

==============================================================================*/
void reg_mode_filter_available_plmn_list_with_rat_bst
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info
)
{
  uint32 i;
  
  rat_pri_list_info->num_items = MIN(rat_pri_list_info->num_items,SYS_MAX_ACQ_PREF_RAT_LIST_NUM);

  for (i = 0; i < rat_pri_list_info->num_items; i++)
  {
    if (rat_pri_list_info->priority_list_info[i].bst_rat_acq_required == FALSE)
    {
      reg_mode_filter_available_plmn_list_rat(reg_mode_convert_acq_mode_to_access_tech
               (rat_pri_list_info->priority_list_info[i].acq_sys_mode));
    }
  }

}
/*****************************************************/
/*                      Collision fallback FR changes                                */
/*                                                                                            */
/*****************************************************/


/*Function to return the last registered rat as set in NV*/

sys_radio_access_tech_e_type reg_mode_get_last_rplmn_rat(void)
{   
    nv_rplmnact_type rplmnact;
    boolean rplmnact_undefined = TRUE;


    reg_nv_read_rplmnact(&rplmnact);
    rplmnact_undefined = ((rplmnact.act[0] == 0) && (rplmnact.act[1] == 0));
    
    if(rplmnact_undefined)
      return SYS_RAT_NONE;
    
    if (rplmnact.act[1] & 0x80)
    {
       return SYS_RAT_GSM_RADIO_ACCESS;
    }
    else if (rplmnact.act[0] & 0x80)  /*both tds and wcdma has 0th bit set as 0x80*/
    {
#ifdef FEATURE_TDSCDMA    
      if (rplmnact.act[0] & TDSCDMA_ACT_BIT) /*TDS has an extra bit set*/
        return SYS_RAT_TDS_RADIO_ACCESS;
      else
#endif        
        return SYS_RAT_UMTS_RADIO_ACCESS;
    }
#ifdef FEATURE_LTE
    else if (rplmnact.act[0] & 0x40)
    {
     return SYS_RAT_LTE_RADIO_ACCESS;
    }
#endif
    return SYS_RAT_NONE;
}
void reg_mode_prioritize_rat_pri_list
(
  reg_mode_3gpp_capability_s_type *rplmn_rat_order,
  sys_radio_access_tech_e_type hp_rat,
  byte pos
)
{
    byte i,j;
    MSG_HIGH_DS_1(REG_SUB,"=REG=  Inside reg_mode_PRioritize_rat_pri_list hp_rat=%d ",hp_rat);
    // pos should always be 1 or more
    for(i = pos; i < MIN(rplmn_rat_order->num_items, REG_MODE_MAX_3GPP_RAT) && i>0; i++)
    {
      if(rplmn_rat_order->rat_list[i] == hp_rat)
      {
        for(j = i; j > pos-1; j--)
        {
          rplmn_rat_order->rat_list[j] = rplmn_rat_order->rat_list[j-1];
        }
        rplmn_rat_order->rat_list[pos-1] = hp_rat;
        break;
      }
    }
    return;
}

void reg_mode_deprioritize_rat_pri_list
(
  reg_mode_3gpp_capability_s_type *rplmn_rat_order,
  sys_radio_access_tech_e_type lp_rat
)
{
    byte i,j;

    MSG_HIGH_DS_1(REG_SUB,"=REG=  Inside reg_mode_DEprioritize_rat_pri_list lp_rat=%d ",lp_rat);
    
    // no need to check the last
    for(i = 0;i < rplmn_rat_order->num_items-1; i++)
    {
      if(rplmn_rat_order->rat_list[i] == lp_rat)
      {
        for(j = i+1; j < rplmn_rat_order->num_items; j++)
        {
          rplmn_rat_order->rat_list[j-1] = rplmn_rat_order->rat_list[j];
        }
        rplmn_rat_order->rat_list[j-1] = lp_rat;
        break;
      }
    }
    return;
}







/*Function move prioritize rat list for plmn selection according to the fall back priority rats given by MCS*/

void reg_mode_prioritize_mcs_rat_order
(
  reg_mode_3gpp_capability_s_type *rplmn_rat_order,
  sys_radio_access_tech_e_type lp_rat,
  sys_radio_access_tech_e_type hp_rat
)
{

  sys_radio_access_tech_e_type last_rplmn_rat = reg_mode_get_last_rplmn_rat();
  sys_radio_access_tech_e_type active_rat = reg_state_active_rat_get();
  byte i;  
  MSG_HIGH_DS_0(REG_SUB,"=REG=  Inside reg_mode_prioritize_mcs_rat_order ");


  // if first rat == lrr
  if(rplmn_rat_order->rat_list[0] == last_rplmn_rat)
  {
     if (reg_mode_is_rat_present_in_rat_pri_list(lp_rat,reg_mode_rat_pri_list_get(),FALSE) && (lp_rat != last_rplmn_rat)) 
     {
         reg_mode_deprioritize_rat_pri_list(rplmn_rat_order,lp_rat);
     }
     else // if lp rat is not present or it is the first to start with, just adjust the hp rat
     {
         reg_mode_prioritize_rat_pri_list(rplmn_rat_order,hp_rat,2);   
     }
   }
   else
   {
      // if first rat is not lrr, follow mcs rat order  
      reg_mode_prioritize_rat_pri_list(rplmn_rat_order,hp_rat,1); // added position param to put in first position
      reg_mode_deprioritize_rat_pri_list(rplmn_rat_order,lp_rat);
   }


 

  /*
  // always deprioritze lp_rat if it not not the active rat
  if (reg_mode_is_rat_present_in_rat_pri_list(lp_rat,reg_mode_rat_pri_list_get(),FALSE) && (lp_rat != last_rplmn_rat || active_rat!=lp_rat))
       reg_mode_deprioritize_rat_pri_list(rplmn_rat_order,lp_rat);

 if (last_rplmn_rat != lp_rat) // if lp rat is not in rat list, move lrr to first
 {
    reg_mode_prioritize_rat_pri_list(rplmn_rat_order,last_rplmn_rat);
 }
 else
 {
    if(active_rat!= SYS_RAT_NONE && active_rat != lp_rat)
    {
       reg_mode_prioritize_rat_pri_list(rplmn_rat_order,active_rat);   
    }
 }
 */
 //print
 MSG_HIGH_DS_0(REG_SUB,"=REG= Printing modified RPLMN rat order ");
 for(i = 0;i < rplmn_rat_order->num_items; i++)
  {
    MSG_HIGH_DS_2(REG_SUB,"=REG= rat %d = %d ",i+1,rplmn_rat_order->rat_list[i]);
  }
 
return;

}


sys_radio_access_tech_e_type reg_mode_convert_fallback_rat_to_access_tech(trm_idle_coll_fallback_enum_t mcs_rat)
{
  sys_radio_access_tech_e_type rat = SYS_RAT_NONE;

  switch(mcs_rat)
  {
    case TRM_FALLBACK_GSM_PREF:
      rat = SYS_RAT_GSM_RADIO_ACCESS;
      break;
    case TRM_FALLBACK_LTE_PREF:
      rat = SYS_RAT_LTE_RADIO_ACCESS;
      break;     
    case TRM_FALLBACK_UMTS_PREF:
      rat = SYS_RAT_UMTS_RADIO_ACCESS;
      break;   
    default :
      rat = SYS_RAT_NONE;
      break; 
  }      
  return rat;
}

/*This function would be called by MCS when fallback occurs and when it ends*/
void reg_mode_set_mcs_pri_cb(trm_idle_coll_event_data* event_data)
{

   sys_radio_access_tech_e_type lp_rat;
   sys_radio_access_tech_e_type hp_rat;
   sys_modem_as_id_e_type cmd_as_id ;
 
  
   cmd_as_id = event_data->action_info.asid;
   lp_rat = reg_mode_convert_acq_mode_to_access_tech(event_data->data.idle_fallback_data.least_priority_rat);
   hp_rat = reg_mode_convert_fallback_rat_to_access_tech(event_data->data.idle_fallback_data.fallback_type);
  
   MSG_HIGH_DS_0(REG_SUB,"=REG=  Inside reg_mode_set_mcs_pri_cb ");
    
   /*Sanity check for sub id*/
   if((IS_NOT_VALID_SUB_ID(cmd_as_id))
         ||(IS_NOT_VALID_STACK_ID(cmd_as_id)))
   {
     MSG_FATAL_DS(REG_SUB, "=REG= MCS Indicated sub id=%d not valid", cmd_as_id, 0, 0);
   }
      
   
   /*Debug F3s*/   
   MSG_HIGH_DS_4(REG_SUB,"=REG=  NEW reg_as_id=%d trm_idle_coll_action_e_type =%d LP rat(G/W/L)=%d HP rat(G/W/L)=%d",cmd_as_id,event_data->action_info.action,lp_rat,hp_rat);

  
   
   
   if(event_data->action_info.action == TRM_IDLE_COLL_ACTION_FALLBACK && event_data->data.idle_fallback_data.fallback_type != TRM_FALLBACK_NONE)
   {
     reg_state_set_mcs_pri_req(TRUE,( reg_as_id_e_type)cmd_as_id);
     reg_state_set_mcs_low_priority_rat(lp_rat,( reg_as_id_e_type)cmd_as_id);
     reg_state_set_mcs_high_priority_rat(hp_rat,( reg_as_id_e_type)cmd_as_id);
   }
   else
   {
     reg_state_set_mcs_pri_req(FALSE,( reg_as_id_e_type)cmd_as_id);
   }
   

  
   return;
}
#ifdef FEATURE_DUAL_SIM
/*==============================================================================

FUNCTION NAME

  reg_mode_set_ra_cap_update

DESCRIPTION
  
  Function to update the additional info bitmask in CM SERVICE REQ to indicate
  radio capability update is needed.
  
RETURN VALUE  

==============================================================================*/
uint32 reg_mode_set_ra_cap_update
(
  uint32 additional_info
)
{
  uint32 result;
  result = additional_info | BM(SYS_ADDTNL_INFO_FORCE_RADIO_CAP_UPDATE);
  return result;
}
#endif

/*==============================================================================

FUNCTION NAME

  reg_mode_priotitize_hplmn_over_rplmn

DESCRIPTION
  
  Function chooses ehplmn over rplmn for rakuten if conditions are met
  
RETURN VALUE  

==============================================================================*/
sys_plmn_id_s_type reg_mode_priotitize_hplmn_over_rplmn(sys_plmn_id_s_type rplmn)
{
#ifdef FEATURE_EQUIVALENT_HPLMN
  reg_sim_plmn_list_s_type	   ehplmn_list;
#else
  sys_plmn_id_s_type		   hplmn;
#endif
  sys_plmn_id_s_type           plmn;
  sys_mcc_type                mcc;  
  boolean                    plmn_id_is_undefined = FALSE;
  boolean                    mnc_includes_pcs_digit = FALSE;
  sys_mnc_type          mnc;

  
  if((reg_nv_cfl_search_config.cfl_fast_scan_time != 0) &&
#ifdef FEATURE_DUAL_SIM
   	  (reg_sim_check_mcc_same_country_per_sub(rplmn,(sys_modem_as_id_e_type)reg_as_id) == TRUE) &&
#else
     (reg_sim_check_mcc_same_country(rplmn) == TRUE) &&
#endif
      ((reg_mode_network_selection_mode_get() == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) ||
      (reg_mode_network_selection_mode_get() == SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION))&&
      ((reg_state_service_status_get() == SYS_SRV_STATUS_NO_SRV) ||
        reg_state_get_power_up_system_selection() == TRUE))
    {
#ifdef FEATURE_EQUIVALENT_HPLMN
      reg_sim_read_ehplmn_from_sim(&ehplmn_list);
      plmn = ehplmn_list.plmn[0];
#else
      hplmn = reg_sim_read_hplmn();
      plmn = hplmn;
#endif
	  reg_mode_set_hplmn_to_be_selected(FALSE);
    }
	else
	{
      plmn = rplmn;
	}
	
	sys_plmn_get_mcc_mnc(plmn,
						 &plmn_id_is_undefined,
						 &mnc_includes_pcs_digit,
						 &mcc,
						 &mnc); 

	MSG_HIGH_DS_5(REG_SUB, "=REG= plmn = %d-%d, reg_nv_hplmn_cfl_search_timer = %d, service status = %d, power up = %d", mcc,mnc,
	                 reg_nv_hplmn_cfl_search_timer_get(), reg_state_service_status_get(), reg_state_get_power_up_system_selection());
	return plmn;
}

/*==============================================================================

FUNCTION NAME

  reg_mode_skip_eplmn_list

DESCRIPTION
  
  Function checks if eplmn needs to be filled or not
  
RETURN VALUE  

==============================================================================*/
boolean reg_mode_skip_eplmn_list
(
sys_plmn_id_s_type      *req_plmn,
sys_modem_as_id_e_type  stack_id
)
{
	
	if((reg_nv_cfl_search_config.cfl_fast_scan_time != 0) &&
#ifdef FEATURE_DUAL_SIM
	  (reg_sim_check_mcc_same_country_per_sub(*req_plmn, (reg_as_id_e_type)stack_id) == TRUE) &&
      (reg_mode_network_selection_mode_get_per_subs(stack_id) == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) &&
	  ((reg_state_service_status_get_per_subs(stack_id) == SYS_SRV_STATUS_NO_SRV) ||
		reg_state_get_power_up_system_selection_per_subs(stack_id) == TRUE))
#else
	  (reg_sim_check_mcc_same_country(req_plmn) == TRUE) &&
      (reg_mode_network_selection_mode_get() == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) &&
	  ((reg_state_service_status_get() == SYS_SRV_STATUS_NO_SRV) ||
		reg_state_get_power_up_system_selection() == TRUE))
#endif
    {
	  return TRUE;
	}
	return FALSE;
}
