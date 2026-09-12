/*==============================================================================
                             COPYRIGHT INFORMATION

Copyright (c) 2002 Qualcomm Technologies, Incorporated and its licensors.  All Rights
Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                            EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.
f
$PVCSPath:  L:/src/asw/MSM5200/reg/vcs/reg_nv.c_v   1.1   06 Jun 2002 17:30:52   kabraham  $
$Header: //components/rel/mmcp.mpss/7.9.0/nas/reg/src/reg_nv.c#2 $ $DateTime: 2025/11/18 02:39:57 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
11/28/12   am      Fixing compilation warning 
05/31/02   kwa     Initial version.
06/06/02   kwa     Eliminated use of memcpy.
12/30/02   kwa     Replaced ERR with MESSAGE_HIGH when attempt to read the
                   equivalent PLMN list from NVRAM fails in
                   reg_nv_read_equivalent_plmn_list.  This message is
                   consistently displayed if the equivalent PLMN list has never
                   been written to NVRAM.
12/31/02   kwa     Added F3 messages to print out the equivalent PLMN list
                   whenever it is saved to NVRAM.
05/22/03   kwa     Modified reg_nv_write_equivalent_plmn_list to pass the
                   eplmn_list_p parameter as a const pointer to eliminate
                   a lint warning.  Replaced reg_nv_equivalent_plmn_list
                   with reg_nv_item to eliminate lint warnings.
07/19/03   kwa     Added functionality to display the equivalent PLMN list
                   after reading it from NVRAM.
10/15/03   kwa     Added functions for reading and writing RPLMNACT.
12/03/03   kwa     Added functions for reading PS LOCI.
03/07/04   kwa     Added function reg_nv_write_cache.
03/17/04   kwa     Added function reg_nv_read_imsi.
04/28/04   kwa     Added call to reg_log_umts_nas_eplmn_list.
07/07/05   up      Added reg_nv_read_gprs_gcf_flag to support GCF 12.2.2.1d.
07/18/05   up      Added reg_nv_ens_enabled_flag to enable the ENS feature.
07/27/05   up      Added reg_nv_equivalent_plmn_list_get to current ePLMN list.
06/09/05   sn      Added flag reg_nv_data_valid flag.
08/09/11   abhi   Added reading of lpm_power_off EFS NV item
11/15/11  abhi   For EFRPLMNSI Selecting RLPMN if HPLMN is not found
09/12/11   HC     Updated the function definition with void argument 
                  for the deprecated function reg_nv_is_lpm_power_off_enabled()
==============================================================================*/

/*==============================================================================

                            INCLUDE FILES FOR MODULE

==============================================================================*/

#include "mmcp_variation.h"
#include "err.h"
#include "nv.h"
#include "reg_log.h"
#include "reg_nv.h"
#include "reg_sim.h"
#include "reg_task.h"
#include "reg_task_int.h"
#include "reg_timers.h"
#include "fs_lib.h"
#include <stringl/stringl.h>
#include "fs_public.h"
#include "reg_timers.h"
#include "reg_mode.h"
#include "modem_mem.h"

#include "ghdi_exp.h"
#include "ghdi_exp_int.h"
#include "ghdi_nvmem.h"
#include "ghdi_nvmem_v.h"
#include "fs_errno.h"
/*==============================================================================

                                TYPE DEFINITIONS

==============================================================================*/

/*==============================================================================

                               DATA DECLARATIONS

==============================================================================*/
#define REG_NV_EFNAS_CONFIG_LENGTH 120
#define REG_NV_EFS_EHPLMN_LIST 7
#define REG_NV_EFS_PREFERRED_LIST 6

#if defined(FEATURE_TRIPLE_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
reg_debug_sim_swap_status_e_type reg_debug_sim_swap_status_sim[MAX_NAS_STACKS] = { REG_DEBUG_SIM_SWAP_UNDEFINED, REG_DEBUG_SIM_SWAP_UNDEFINED, REG_DEBUG_SIM_SWAP_UNDEFINED};
#define reg_debug_sim_swap_status reg_debug_sim_swap_status_sim[reg_as_id]
nv_stat_enum_type reg_debug_nv_imsi_read_error_sim[MAX_NAS_STACKS];
#define reg_debug_nv_imsi_read_error reg_debug_nv_imsi_read_error_sim[reg_as_id]

#elif defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
reg_debug_sim_swap_status_e_type reg_debug_sim_swap_status_sim[MAX_NAS_STACKS] = { REG_DEBUG_SIM_SWAP_UNDEFINED, REG_DEBUG_SIM_SWAP_UNDEFINED};
#define reg_debug_sim_swap_status reg_debug_sim_swap_status_sim[reg_as_id]
nv_stat_enum_type reg_debug_nv_imsi_read_error_sim[MAX_NAS_STACKS];
#define reg_debug_nv_imsi_read_error reg_debug_nv_imsi_read_error_sim[reg_as_id]
#else
reg_debug_sim_swap_status_e_type reg_debug_sim_swap_status = REG_DEBUG_SIM_SWAP_UNDEFINED;
nv_stat_enum_type reg_debug_nv_imsi_read_error;
#endif

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
static nv_equivalent_plmn_list_type reg_nv_equivalent_plmn_list_sim[MAX_NAS_STACKS];
#define reg_nv_equivalent_plmn_list reg_nv_equivalent_plmn_list_sim[reg_as_id]
static nv_rplmnact_type             reg_nv_rplmnact_sim[MAX_NAS_STACKS];
#define reg_nv_rplmnact          reg_nv_rplmnact_sim[reg_as_id]
static nv_item_type                 reg_nv_item_sim[MAX_NAS_STACKS];
#define reg_nv_item                reg_nv_item_sim[reg_as_id]


static nv_cmd_type                  reg_nv_cmd;

static nv_cmd_ext_type              reg_nv_ext_cmd;
#define reg_nv_cmd_per_subs(a) reg_nv_ext_cmd.context = reg_nv_context_id_sim[reg_as_id]; \
                      reg_nv_ext_cmd.nvcmd = a; \
                      nv_cmd_ext(&reg_nv_ext_cmd)
static nv_extended_equivalent_plmn_list_type  reg_nv_extended_equivalent_plmn_list_sim[MAX_NAS_STACKS];
#define reg_nv_extended_equivalent_plmn_list  reg_nv_extended_equivalent_plmn_list_sim[reg_as_id] 

#define REG_EFS_CONTEXT_ID_SUB1  0
#define REG_EFS_CONTEXT_ID_SUB2  1
#define REG_EFS_CONTEXT_ID_SUB3  2

reg_nv_vplmn_list_s_type *reg_nv_vplmn_list_sim[MAX_NAS_STACKS]={NULL};
#define reg_nv_vplmn_list (reg_nv_vplmn_list_sim[reg_as_id])

#else
static nv_equivalent_plmn_list_type reg_nv_equivalent_plmn_list;

static nv_rplmnact_type             reg_nv_rplmnact;

static nv_item_type                 reg_nv_item;

static nv_cmd_type                  reg_nv_cmd;


#define reg_nv_cmd_per_subs(a)               nv_cmd(&reg_nv_cmd)

static nv_extended_equivalent_plmn_list_type  reg_nv_extended_equivalent_plmn_list;
reg_nv_vplmn_list_s_type *reg_nv_vplmn_list= NULL;                   
#endif /* FEATURE_DUAL_SIM*/

#ifdef FEATURE_DUAL_SIM
static rex_timer_cnt_type  reg_nv_t3245_timer_stored_value_sim[MAX_AS_IDS];
#define reg_nv_t3245_timer_stored_value  reg_nv_t3245_timer_stored_value_sim[reg_sub_id] 

static rex_timer_cnt_type  reg_nv_validate_sim_timer_value_sim[MAX_AS_IDS];
#define reg_nv_validate_sim_timer_value  reg_nv_validate_sim_timer_value_sim[reg_sub_id] 
static uint8  reg_nv_max_validate_sim_counter_sim[MAX_AS_IDS];
#define reg_nv_max_validate_sim_counter  reg_nv_max_validate_sim_counter_sim[reg_sub_id] 

static rex_timer_cnt_type  reg_nv_t3245_test_timer_value_sim[MAX_AS_IDS];
#define reg_nv_t3245_test_timer_value  reg_nv_t3245_test_timer_value_sim[reg_sub_id] 
static sys_data_roaming_e_type reg_nv_data_romaing_sim[MAX_AS_IDS];
#define reg_nv_data_romaing  reg_nv_data_romaing_sim[reg_sub_id]
static boolean reg_nv_voice_roaming_sim[MAX_AS_IDS];//FR 33195
#define reg_nv_voice_roaming  reg_nv_voice_roaming_sim[reg_sub_id]
static boolean reg_nv_ehplmn_temp_forbid_enabled_sim[MAX_AS_IDS];
#define reg_nv_ehplmn_temp_forbid_enabled reg_nv_ehplmn_temp_forbid_enabled_sim[reg_sub_id] 
static boolean reg_nv_domestic_voice_roaming_sim[MAX_AS_IDS];
#define reg_nv_domestic_voice_roaming  reg_nv_domestic_voice_roaming_sim[reg_sub_id]

static rex_timer_cnt_type  reg_nv_interlace_scan_timer_value_sim[MAX_AS_IDS];
#define reg_nv_interlace_scan_timer_value  reg_nv_interlace_scan_timer_value_sim[reg_sub_id] 

static boolean reg_nv_allow_hplmn_timer_restart_sim[MAX_AS_IDS];
#define reg_nv_allow_hplmn_timer_restart reg_nv_allow_hplmn_timer_restart_sim[MAX_AS_IDS]

#else
static rex_timer_cnt_type                      reg_nv_t3245_timer_stored_value ; /*Timer value is in milli seconds*/

static rex_timer_cnt_type                      reg_nv_validate_sim_timer_value;
static uint8                                   reg_nv_max_validate_sim_counter;

static rex_timer_cnt_type                      reg_nv_t3245_test_timer_value;
static sys_data_roaming_e_type                 reg_nv_data_romaing;
static boolean                 reg_nv_voice_roaming;//FR 33195
static boolean                             reg_nv_ehplmn_temp_forbid_enabled= FALSE;
static rex_timer_cnt_type                      reg_nv_interlace_scan_timer_value;
static boolean                 reg_nv_domestic_voice_roaming;
static boolean                 reg_nv_allow_hplmn_timer_restart;

#endif

boolean reg_is_qrd_device = FALSE;                             


#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_ENHANCED_NW_SELECTION
#define reg_sim_read_ens_flag() reg_sim_per_nas_stacks_read_ens_flag((sys_modem_as_id_e_type)reg_sub_id)
#endif
#endif

#if defined(FEATURE_TRIPLE_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
static boolean                      reg_nv_data_valid_sim[MAX_NAS_STACKS] = { FALSE, FALSE,FALSE};
#define reg_nv_data_valid           reg_nv_data_valid_sim[reg_as_id] 

#elif defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
static boolean                      reg_nv_data_valid_sim[MAX_NAS_STACKS] = { FALSE, FALSE};
#define reg_nv_data_valid           reg_nv_data_valid_sim[reg_as_id] 
#else
static boolean                      reg_nv_data_valid = FALSE;
#endif

#ifdef FEATURE_ENHANCED_NW_SELECTION
/*
** NV Flag to enable the ENS Feature.
*/
static boolean                      reg_nv_ens_enabled_flag = FALSE;
#endif

#if defined(FEATURE_LTE) && defined(FEATURE_MMSS_3_1)
#ifdef FEATURE_DUAL_SIM
static boolean                      reg_nv_ignore_uplmn_sim[MAX_AS_IDS];
#define reg_nv_ignore_uplmn reg_nv_ignore_uplmn_sim[reg_sub_id]
#else
static boolean                      reg_nv_ignore_uplmn = FALSE;
#endif 
#endif

static boolean                      reg_nv_imsi_switch      = FALSE;

static boolean                      reg_nv_wcdma_freq_lock_enabled = FALSE;
boolean                             reg_nv_lpm_power_off = TRUE;
static boolean                     reg_nv_efrplmnsi_selection = FALSE;
#ifdef FEATURE_DUAL_SIM
static boolean                      reg_nv_forced_irat_enabled_sim[MAX_AS_IDS];
#define reg_nv_forced_irat_enabled reg_nv_forced_irat_enabled_sim[reg_sub_id]

#else
static boolean                      reg_nv_forced_irat_enabled = FALSE;
#endif
static reg_sim_plmn_list_s_type     reg_nv_tdscdma_op_plmn_efs_list; 
static reg_sim_plmn_list_s_type    reg_nv_ehplmn_efs_list;

#define    REG_HPLMN_IRAT_SEARCH_TIMER     5*60*1000  /*default to 5 minutes*/

#ifdef FEATURE_DUAL_SIM
static uint32                       reg_nv_hplmn_irat_search_timer_sim[MAX_AS_IDS]; /*Timer value is in milli seconds*/;
#define reg_nv_hplmn_irat_search_timer reg_nv_hplmn_irat_search_timer_sim[reg_sub_id]
lte_search_timer_feature_config_T reg_nv_lte_search_config_sim[MAX_AS_IDS];
#define reg_nv_lte_search_config  reg_nv_lte_search_config_sim[reg_sub_id]

lte_cfl_search_timer_config_T     reg_nv_cfl_search_config_sim[MAX_AS_IDS];
#define reg_nv_cfl_search_config  reg_nv_cfl_search_config_sim[reg_sub_id]

uint8     reg_cfl_hplmn_search_counter_sim[MAX_AS_IDS];
#define reg_cfl_hplmn_search_counter  reg_cfl_hplmn_search_counter_sim[reg_sub_id]

#else
static uint32                       reg_nv_hplmn_irat_search_timer = REG_HPLMN_IRAT_SEARCH_TIMER /*Timer value is in milli seconds*/;
lte_search_timer_feature_config_T   reg_nv_lte_search_config;
lte_cfl_search_timer_config_T       reg_nv_cfl_search_config;
uint8                               reg_cfl_hplmn_search_counter;
#endif


boolean reg_nv_is_camp_on_2g_if_reg_failure = FALSE;
#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
static sys_plmn_id_s_type  reg_nv_rplmn_sim[MAX_NAS_STACKS];
#define  reg_nv_rplmn reg_nv_rplmn_sim[reg_as_id]

static boolean reg_nv_rplmn_dup_sim[MAX_NAS_STACKS];
#define reg_nv_rplmn_dup reg_nv_rplmn_dup_sim[reg_as_id]
#else
 /*
   RPLMN read from NV and also RPLMN used by REG. 
   This is updated with current RPLMN in all net sel modes except MANUAL CSG
*/
static sys_plmn_id_s_type  reg_nv_rplmn; 

/*
  This is set when reg_nv_rplmn is not updated in manaul csg mode.
  If device is powered off and this is set then only RPLMN is written to NV
*/
static boolean reg_nv_rplmn_dup;          
#endif /*FEATURE_DUAL_SIM*/

#ifdef FEATURE_FEMTO_CSG
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_DISABLE_CSG_BYDEFAULT
static sys_csg_support_e_type    reg_nv_csg_support_sim[MAX_NAS_STACKS] = {SYS_CSG_SUPPORT_DISABLED, SYS_CSG_SUPPORT_DISABLED};
#else
static sys_csg_support_e_type    reg_nv_csg_support_sim[MAX_NAS_STACKS] = {SYS_CSG_SUPPORT_WCDMA, SYS_CSG_SUPPORT_WCDMA};
#endif
#define reg_nv_csg_support reg_nv_csg_support_sim[reg_sub_id]
#else
#ifdef FEATURE_DISABLE_CSG_BYDEFAULT
static sys_csg_support_e_type    reg_nv_csg_support = SYS_CSG_SUPPORT_DISABLED;
#else
static sys_csg_support_e_type    reg_nv_csg_support = SYS_CSG_SUPPORT_WCDMA;
#endif 
#endif 
#endif 

#endif 

#ifdef FEATURE_DUAL_SIM
boolean                                reg_nv_is_hplmn_to_be_selected_sim[MAX_AS_IDS];
#define reg_nv_is_hplmn_to_be_selected reg_nv_is_hplmn_to_be_selected_sim[reg_sub_id]

boolean                                reg_nv_is_roaming_on_in_eplmn_sim[MAX_AS_IDS];
#define reg_nv_is_roaming_on_in_eplmn  reg_nv_is_roaming_on_in_eplmn_sim[reg_sub_id]

boolean                                reg_nv_is_sim_invalid_recovery_enabled_sim[MAX_AS_IDS];
#define reg_nv_is_sim_invalid_recovery_enabled reg_nv_is_sim_invalid_recovery_enabled_sim[reg_sub_id]

boolean                                reg_nv_is_pseudo_reselection_allowed_sim[MAX_AS_IDS];
#define reg_nv_is_pseudo_reselection_allowed reg_nv_is_pseudo_reselection_allowed_sim[reg_sub_id]

boolean                                reg_nv_is_w_bplmn_pseudo_enabled_sim[MAX_AS_IDS];
#define reg_nv_is_w_bplmn_pseudo_enabled reg_nv_is_w_bplmn_pseudo_enabled_sim[reg_sub_id]

boolean                                reg_nv_use_rplmnact_mode_change_sim[MAX_AS_IDS];
#define reg_nv_use_rplmnact_mode_change reg_nv_use_rplmnact_mode_change_sim[reg_sub_id]


#else
boolean                                reg_nv_is_hplmn_to_be_selected = FALSE;
boolean                                reg_nv_is_roaming_on_in_eplmn = FALSE; 
boolean                                reg_nv_is_sim_invalid_recovery_enabled = FALSE;
boolean                                reg_nv_is_pseudo_reselection_allowed = TRUE;
boolean                                reg_nv_is_w_bplmn_pseudo_enabled = FALSE;
boolean                                reg_nv_use_rplmnact_mode_change = FALSE;
#endif
boolean                                reg_nv_is_home_plmn_to_be_selcted_in_home_country = FALSE;

#define REG_NV_MAX_PREF_PLMN_LIST 50

#ifdef FEATURE_DUAL_SIM
reg_sim_plmn_w_act_s_type  *reg_nv_preferred_plmn_list_sim[MAX_AS_IDS] = {NULL,NULL};
#define reg_nv_preferred_plmn_list reg_nv_preferred_plmn_list_sim[reg_as_id]
int32 reg_nv_pref_plmn_list_length_sim[MAX_AS_IDS] = {0,0};
#define reg_nv_pref_plmn_list_length reg_nv_pref_plmn_list_length_sim[reg_as_id]
#else
reg_sim_plmn_w_act_s_type  *reg_nv_preferred_plmn_list = NULL; 
uint32 reg_nv_pref_plmn_list_length = 0;
#endif
extern nas_crit_sect_type reg_tdscdma_op_list_crit_sect;

#ifdef FEATURE_DUAL_SIM
static reg_nv_plmn_rat_list_s_type  reg_nv_pm_chg_pend_plmn_rat_sim[MAX_AS_IDS];
#define  reg_nv_pm_chg_pend_plmn_rat reg_nv_pm_chg_pend_plmn_rat_sim[reg_as_id]
#else
static reg_nv_plmn_rat_list_s_type  reg_nv_pm_chg_pend_plmn_rat;
#endif 

#ifdef FEATURE_NAS_ENABLE_LTE_BAND_FILTERING
#ifdef FEATURE_DUAL_SIM
uint8                             reg_nv_td_op_device_type_sim[MAX_AS_IDS] = {REG_TD_OP_DEVICE_TYPE_NONE, REG_TD_OP_DEVICE_TYPE_NONE};
#define reg_nv_td_op_device_type  reg_nv_td_op_device_type_sim[reg_sub_id]
#elif FEATURE_TRIPLE_SIM
uint8                             reg_nv_td_op_device_type_sim[MAX_AS_IDS] = {REG_TD_OP_DEVICE_TYPE_NONE,REG_TD_OP_DEVICE_TYPE_NONE,REG_TD_OP_DEVICE_TYPE_NONE};
#define reg_nv_td_op_device_type  reg_nv_td_op_device_type_sim[reg_sub_id]
#else
uint8                               reg_nv_td_op_device_type = (uint8)REG_TD_OP_DEVICE_TYPE_NONE;
#endif 
#endif

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
extern mmgsdi_app_enum_type reg_sim_card_mode_sim[MAX_NAS_STACKS] ;
#define reg_sim_card_mode reg_sim_card_mode_sim[reg_as_id]
#else
mmgsdi_app_enum_type reg_sim_card_mode;
#endif

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
extern boolean reg_state_is_pwr_on_manual_plmn_selection_sim[MAX_NAS_STACKS];
#define reg_state_is_pwr_on_manual_plmn_selection reg_state_is_pwr_on_manual_plmn_selection_sim[reg_as_id]
#else
extern boolean reg_state_is_pwr_on_manual_plmn_selection;
#endif

#ifdef FEATURE_DUAL_SIM
#define reg_ghdi_nvmem_read(a,b) ghdi_nvmem_read_per_subs(a,b,reg_nv_context_id)
#else
#define reg_ghdi_nvmem_read(a,b) ghdi_nvmem_read(a,b)
#endif
/*==============================================================================

                          STATIC FUNCTION DEFINITIONS

==============================================================================*/


/*===========================================================================

FUNCTION reg_nv_populate_default_enhanced_hplmn_srch_tbl

DESCRIPTION
  Populate default tbl.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void reg_nv_populate_default_enhanced_hplmn_srch_tbl(byte *nv_config_tbl)
{
#if defined(FEATURE_DISABLE_MCC_FILTERING) && !defined(FEATURE_DISABLE_HPLMN_MCC_FILTERING)
#ifdef FEATURE_ENHANCED_NW_SELECTION
  byte default_tbl[REG_NV_CONFIG_TBL_MAX_SIZE] = {
                                            1, /* Config type is SET type */
                                            1, /* Only One Set present */
                                            10, /* 10 MCC present in the set */
                                                 /* 10 MCC entries */
                                            0x13, 0xF0,
                                            0x13, 0xF1,
                                            0x13, 0xF2,
                                            0x13, 0xF3,
                                            0x13, 0xF4,
                                            0x13, 0xF5,
                                            0x13, 0xF6,
                                            0x03, 0xF2,
                                            0x33, 0xF4,
                                            0x43, 0xF8 
                          };
  
  if(reg_sim_read_ens_flag())
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= Populating default enhanced hplmn search tbl");
  
    /* Copy to tbl byte stream */
    memscpy((void*)nv_config_tbl,REG_NV_CONFIG_TBL_MAX_SIZE, (void*)default_tbl, sizeof(default_tbl));
  }
#endif
#endif

  return;

}
/*===========================================================================

FUNCTION reg_nv_write_rplmnact_cache

DESCRIPTION
  This function writes the cache of RPLMN ACT to the NV location

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void reg_nv_write_rplmnact_cache(void)
{
/*
  ** Fill NV command to write RPLMNACT.
  */
  reg_nv_item.rplmnact = reg_nv_rplmnact;

  reg_nv_cmd.tcb_ptr    = rex_self();
  reg_nv_cmd.sigs       = REG_NVIF_SIG;
  reg_nv_cmd.done_q_ptr = NULL;
  reg_nv_cmd.cmd        = NV_WRITE_F;
  reg_nv_cmd.item       = NV_RPLMNACT_I;
  reg_nv_cmd.data_ptr   = &reg_nv_item;

  /*
  ** Clear the REG_NVIF_SIG signal.
  */
  (void) rex_clr_sigs(rex_self(), REG_NVIF_SIG);

  /*
  ** Send the command to NV.
  */
  reg_nv_cmd_per_subs(&reg_nv_cmd);

  /*
  ** Wait for NV to set the REG_NVIF_SIG signal.
  */
  (void) reg_wait(REG_NVIF_SIG);

  if (reg_nv_cmd.status != NV_DONE_S)
  {
    MSG_ERROR_DS_0(REG_SUB, "=REG= Write RPLMNACT failed");
  }
  else
  {
    MSG_HIGH_DS_2(REG_SUB, "=REG= Wrote RPLMNACT %d %d", reg_nv_rplmnact.act[0], reg_nv_rplmnact.act[1]);
  }
}




boolean reg_nv_pseudo_reselection_allowed(void)
{
  return reg_nv_is_pseudo_reselection_allowed;
}


boolean reg_nv_w_bplmn_pseudo_enabled(void)
{
   return reg_nv_is_w_bplmn_pseudo_enabled;

}
boolean reg_nv_use_rplmnact_mode_change_get(void)
{
  MSG_HIGH_DS_1(REG_SUB,"=REG= NV reg_nv_use_rplmnact_mode_change set to = %d",reg_nv_use_rplmnact_mode_change);
  return reg_nv_use_rplmnact_mode_change;
}
boolean reg_nv_home_plmn_to_be_selcted_in_home_country_get(void)
{
  MSG_HIGH_DS_1(REG_SUB,"=REG= reg_nv_is_home_plmn_to_be_selcted_in_home_country read as = %d",reg_nv_is_home_plmn_to_be_selcted_in_home_country);
  return reg_nv_is_home_plmn_to_be_selcted_in_home_country;
}
/*===========================================================================

FUNCTION reg_nv_output_vplmn_list

DESCRIPTION
  Prints the VPLMN

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void reg_nv_output_vplmn_list                
(
  void
)
{
  sys_plmn_id_s_type plmn;

  boolean plmn_id_is_undefined;
  boolean mnc_includes_pcs_digit;
  uint32  mcc;
  uint32  mnc;

  uint8 i;
  if(!reg_nv_vplmn_list)
  {
    MSG_FATAL_DS(REG_SUB, "=REG= reg_nv_vplmn_list is not initialized, and called for display", 0, 0, 0);
  }
  MSG_HIGH_DS_1(REG_SUB, "=REG= VPLMN PLMN list (length = %d)", reg_nv_vplmn_list->vplmn_length);

  if (reg_nv_vplmn_list->vplmn_length> 0)
  {
    for (i = 0; i < reg_nv_vplmn_list->vplmn_length; i++)
    {
        plmn.identity[0] = reg_nv_vplmn_list->vplmn_info[i].plmn.identity[0];
        plmn.identity[1] = reg_nv_vplmn_list->vplmn_info[i].plmn.identity[1];
        plmn.identity[2] = reg_nv_vplmn_list->vplmn_info[i].plmn.identity[2];
 
        sys_plmn_get_mcc_mnc
        (
         plmn,
         &plmn_id_is_undefined,
         &mnc_includes_pcs_digit,
         &mcc,
         &mnc
         );
        if(mnc_includes_pcs_digit)
        {
          MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu", i, mcc, mnc);
          MSG_HIGH_DS_1(REG_SUB, "=REG= RAT= %d", reg_nv_vplmn_list->vplmn_info[i].rat);
        }
        else
        {
          MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu", i, mcc, mnc);
          MSG_HIGH_DS_1(REG_SUB, "=REG= RAT= %d", reg_nv_vplmn_list->vplmn_info[i].rat);
        }
    }
  }

}
void reg_nv_output_eplmn_list
(
  void
)
{
  sys_plmn_id_s_type plmn;

  boolean plmn_id_is_undefined;
  boolean mnc_includes_pcs_digit;
  uint32  mcc;
  uint32  mnc;

  uint8 i;

  MSG_HIGH_DS_1(REG_SUB, "=REG= Equivalent PLMN list (length = %d)", reg_nv_equivalent_plmn_list.length);

  if (reg_nv_equivalent_plmn_list.length > 0)
  {
    plmn.identity[0] = reg_nv_equivalent_plmn_list.rplmn.identity[0];
    plmn.identity[1] = reg_nv_equivalent_plmn_list.rplmn.identity[1];
    plmn.identity[2] = reg_nv_equivalent_plmn_list.rplmn.identity[2];

    sys_plmn_get_mcc_mnc
    (
      plmn,
      &plmn_id_is_undefined,
      &mnc_includes_pcs_digit,
      &mcc,
      &mnc
    );

    if(mnc_includes_pcs_digit)
    {
      MSG_HIGH_DS_2(REG_SUB, "=REG= Equivalent RPLMN(%03lu-%03lu)", mcc, mnc);
    }
    else
    {
      MSG_HIGH_DS_2(REG_SUB, "=REG= Equivalent RPLMN(%03lu- %02lu)", mcc, mnc);
    }

    MSG_HIGH_DS_0(REG_SUB, "=REG=  # MCC-MNC");

    for (i = 0; i < reg_nv_equivalent_plmn_list.length; i++)
    {
      /* get the additional EPLMNs from extended nv list*/
      if(i >= NV_EQUIVALENT_PLMN_LIST_MAX_SIZE)/*after this use new extended list*/
      {
        byte j = i- NV_EQUIVALENT_PLMN_LIST_MAX_SIZE;
        plmn.identity[0] = reg_nv_extended_equivalent_plmn_list.plmn_identity[j][0];
        plmn.identity[1] = reg_nv_extended_equivalent_plmn_list.plmn_identity[j][1];
        plmn.identity[2] = reg_nv_extended_equivalent_plmn_list.plmn_identity[j][2];
      }
      else
      {
        plmn.identity[0] = reg_nv_equivalent_plmn_list.plmn[i].identity[0];
        plmn.identity[1] = reg_nv_equivalent_plmn_list.plmn[i].identity[1];
        plmn.identity[2] = reg_nv_equivalent_plmn_list.plmn[i].identity[2];
      }
      sys_plmn_get_mcc_mnc
      (
        plmn,
        &plmn_id_is_undefined,
        &mnc_includes_pcs_digit,
        &mcc,
        &mnc
      );
      if(mnc_includes_pcs_digit)
      {
        MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu", i, mcc, mnc);
      }
      else
      {
        MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu", i, mcc, mnc);
      }
    }
  }
}
/*==============================================================================

                              FUNCTION DEFINITIONS

==============================================================================*/

#ifdef FEATURE_ENHANCED_NW_SELECTION
/*==============================================================================

FUNCTION NAME

  reg_nv_read_ens_enabled_flag

==============================================================================*/

boolean reg_nv_read_ens_enabled_flag
(
  void
)
{
  /*
  ** Fill NV command to read NV_ENS_ENABLED.
  */
  reg_nv_cmd.tcb_ptr    = rex_self();
  reg_nv_cmd.sigs       = REG_NVIF_SIG;
  reg_nv_cmd.done_q_ptr = NULL;
  reg_nv_cmd.cmd        = NV_READ_F;
  reg_nv_cmd.item       = NV_ENS_ENABLED_I;
  reg_nv_cmd.data_ptr   = (nv_item_type*)(&reg_nv_ens_enabled_flag);

  /*
  ** Clear the REG_NVIF_SIG signal.
  */
  (void) rex_clr_sigs(rex_self(), REG_NVIF_SIG);

  /*
  ** Send the command to NV.
  */
  nv_cmd(&reg_nv_cmd);

  /*
  ** Wait for NV to set the REG_NVIF_SIG signal.
  */
  (void) reg_wait(REG_NVIF_SIG);

  /*
  ** NV_ENS_ENABLED could not be read from NVRAM.
  */
  if (reg_nv_cmd.status != NV_DONE_S)
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= Reading NV_ENS_ENABLED FLAG failed");

    reg_nv_ens_enabled_flag = FALSE;
  }
  /*
  ** NV_ENS_ENABLED was successfully read from NVRAM.
  */
  else
  {
    MSG_HIGH_DS_1(REG_SUB, "=REG= Read NV_ENS_ENABLED FLAG  - %d", reg_nv_ens_enabled_flag);
  }
  
  return reg_nv_ens_enabled_flag;
}
#endif

/*==============================================================================

FUNCTION NAME

  reg_nv_equivalent_plmn_list_get

==============================================================================*/
nv_equivalent_plmn_list_type reg_nv_equivalent_plmn_list_get
(
  void
)
{
  return reg_nv_equivalent_plmn_list;
}
/*==============================================================================

FUNCTION NAME

  reg_nv_get_vplmn_list_length              

==============================================================================*/
int16 reg_nv_get_vplmn_list_length
(
  void
)
{
  if(reg_nv_vplmn_list)
  {
    return reg_nv_vplmn_list->vplmn_length;
  }
  else
  {
    return -1;
  }
}

/*==============================================================================

FUNCTION NAME

  reg_nv_get_vplmn_list              

==============================================================================*/
void reg_nv_get_vplmn_list
(
  reg_nv_vplmn_list_s_type *vplmn_list
)
{
  uint8 i;

  if(vplmn_list == NULL)
    return;

  vplmn_list->vplmn_length = 0;
  if(reg_nv_vplmn_list)
  {
    if(reg_nv_vplmn_list->vplmn_length > 0)
    {
      vplmn_list->vplmn_length = reg_nv_vplmn_list->vplmn_length;
      for (i = 0; i < reg_nv_vplmn_list->vplmn_length; i++)
      {
        vplmn_list->vplmn_info[i].plmn.identity[0] = reg_nv_vplmn_list->vplmn_info[i].plmn.identity[0];
        vplmn_list->vplmn_info[i].plmn.identity[1] = reg_nv_vplmn_list->vplmn_info[i].plmn.identity[1];
        vplmn_list->vplmn_info[i].plmn.identity[2] = reg_nv_vplmn_list->vplmn_info[i].plmn.identity[2];
        vplmn_list->vplmn_info[i].rat = reg_nv_vplmn_list->vplmn_info[i].rat;
      }
    }
  }
}

void reg_nv_write_pm_chg_pending_plmn_rat(void)
{
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;

  status = mcfg_fs_write("/nv/item_files/modem/nas/pm_chg_plmn_rat",&reg_nv_pm_chg_pend_plmn_rat,
                                     sizeof(reg_nv_pm_chg_pend_plmn_rat), 
                                     MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,
                                     (mcfg_fs_sub_id_e_type)reg_as_id);

  if(status != MCFG_FS_STATUS_OK)
  {
    MSG_ERROR_DS_0(REG_SUB, "=REG= Write reg_nv_pm_chg_pend_plmn_rat to NV failed");
  }
  else
  {  
    MSG_HIGH_DS_0(REG_SUB, "=REG=  Wrote reg_nv_pm_chg_pend_plmn_rat to NV");
  }
}

void reg_nv_get_pm_chg_pending_plmn_rat(sys_plmn_id_s_type *plmn, sys_radio_access_tech_e_type *rat)
{
  *plmn = reg_nv_pm_chg_pend_plmn_rat.plmn;
  *rat = reg_nv_pm_chg_pend_plmn_rat.rat;
}

void reg_nv_set_pm_chg_pending_plmn_rat(sys_plmn_id_s_type plmn, sys_radio_access_tech_e_type rat)
{
  if((sys_plmn_match(plmn, reg_nv_pm_chg_pend_plmn_rat.plmn) == FALSE)||
     (rat != reg_nv_pm_chg_pend_plmn_rat.rat))
  {
    reg_nv_pm_chg_pend_plmn_rat.plmn = plmn;
    reg_nv_pm_chg_pend_plmn_rat.rat = rat;
    reg_nv_write_pm_chg_pending_plmn_rat();
  }
}

void reg_nv_reset_pm_chg_pending_plmn_rat(void)
{
  if(reg_nv_pm_chg_pend_plmn_rat.rat != SYS_RAT_NONE)
  {
    reg_nv_pm_chg_pend_plmn_rat.rat  = SYS_RAT_NONE;
    sys_plmn_undefine_plmn_id(&reg_nv_pm_chg_pend_plmn_rat.plmn);
    reg_nv_write_pm_chg_pending_plmn_rat();
  }
}


/*==============================================================================

FUNCTION NAME

  reg_nv_put_vplmn_list              

==============================================================================*/
void reg_nv_put_vplmn_list
(
  reg_nv_vplmn_list_s_type vplmn_list
)
{
  uint8 i;
  if(reg_nv_vplmn_list)
  {
    reg_nv_vplmn_list->vplmn_length = vplmn_list.vplmn_length;
    if(reg_nv_vplmn_list->vplmn_length > 0)
    {
      for (i = 0; i < reg_nv_vplmn_list->vplmn_length; i++)
      {
        reg_nv_vplmn_list->vplmn_info[i].plmn.identity[0] = vplmn_list.vplmn_info[i].plmn.identity[0];
        reg_nv_vplmn_list->vplmn_info[i].plmn.identity[1] = vplmn_list.vplmn_info[i].plmn.identity[1];
        reg_nv_vplmn_list->vplmn_info[i].plmn.identity[2] = vplmn_list.vplmn_info[i].plmn.identity[2];
        reg_nv_vplmn_list->vplmn_info[i].rat = vplmn_list.vplmn_info[i].rat;
      }
    }
    reg_nv_output_vplmn_list();
  }

}

/*============================================================================== 

FUNCTION NAME

  reg_nv_write_vplmn_list

==============================================================================*/
mcfg_fs_status_e_type reg_nv_write_vplmn_list
(
  void
)
{
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;
  uint32 data_size = (uint32)sizeof(reg_nv_vplmn_list_s_type);
  
  if(!reg_nv_vplmn_list)
  {
    MSG_FATAL_DS(REG_SUB, "=REG= Global Structure is not allocated memory during power up", 0, 0, 0);
  }

  status = mcfg_fs_write("/nv/item_files/modem/nas/vpmln",reg_nv_vplmn_list,data_size,MCFG_FS_O_RDWR | MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)reg_as_id);


  
  MSG_HIGH_DS_1(REG_SUB, "=REG= VPLMN: Write to VPLMN list status = %d", status);

  return status;
}
/*============================================================================== 

FUNCTION NAME

  reg_nv_read_vplmn_list

==============================================================================*/
mcfg_fs_status_e_type reg_nv_read_vplmn_list
(
  void
)
{
  uint32 data_size;
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;
  data_size = (uint32)sizeof(reg_nv_vplmn_list_s_type);
  if(!reg_nv_vplmn_list)
  {

    reg_nv_vplmn_list = (reg_nv_vplmn_list_s_type *) modem_mem_calloc(1,data_size, MODEM_MEM_CLIENT_NAS);
    if(!reg_nv_vplmn_list )
    {
       MSG_FATAL_DS(REG_SUB, "=REG= Could not allocate the memory", 0, 0, 0);
    }

    memset((void*)reg_nv_vplmn_list, 0, data_size);

    status = mcfg_fs_read("/nv/item_files/modem/nas/vpmln", reg_nv_vplmn_list, data_size, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_as_id);

    if(status == MCFG_FS_STATUS_OK)
    {
       MSG_HIGH_DS_0(REG_SUB, "=REG= VPLMN: Reading VPLMN list from NV, updating the Global");
       reg_nv_output_vplmn_list();
    }
    else
    {
       MSG_HIGH_DS_1(REG_SUB, "=REG= VPLMN: Reading VPLMN list from NV failed status = %d", status);
       reg_nv_vplmn_list->vplmn_length = 0;
    }

  }
  return status;
}
/*==============================================================================

FUNCTION NAME

  reg_nv_sys_equivalent_plmn_list_get

==============================================================================*/
void reg_nv_sys_equivalent_plmn_list_get
(
  sys_plmn_list_s_type* eplmn_list_p
)
{
  uint32 i;

  eplmn_list_p->length = MIN(reg_nv_equivalent_plmn_list.length,
                             NV_EQUIVALENT_PLMN_LIST_MAX_SIZE
                           + NV_EXTENDED_EQUIVALENT_PLMN_LIST_MAX_SIZE);

  eplmn_list_p->length = MIN(eplmn_list_p->length,
                               SYS_PLMN_LIST_MAX_LENGTH);

  for (i = 0; i < eplmn_list_p->length; i++)
  {
    /*get additional EPLMNs from the extended list*/
    if(i >= NV_EQUIVALENT_PLMN_LIST_MAX_SIZE)/*after this use new extended list*/
    {
      byte j = (byte)MIN(NV_EXTENDED_EQUIVALENT_PLMN_LIST_MAX_SIZE-1,(i- NV_EQUIVALENT_PLMN_LIST_MAX_SIZE));
      eplmn_list_p->plmn[i].identity[0] = reg_nv_extended_equivalent_plmn_list.plmn_identity[j][0];
      eplmn_list_p->plmn[i].identity[1] = reg_nv_extended_equivalent_plmn_list.plmn_identity[j][1];
      eplmn_list_p->plmn[i].identity[2] = reg_nv_extended_equivalent_plmn_list.plmn_identity[j][2];
    }
    else
    {
      eplmn_list_p->plmn[i].identity[0] = reg_nv_equivalent_plmn_list.plmn[i].identity[0];
      eplmn_list_p->plmn[i].identity[1] = reg_nv_equivalent_plmn_list.plmn[i].identity[1];
      eplmn_list_p->plmn[i].identity[2] = reg_nv_equivalent_plmn_list.plmn[i].identity[2];
    }
  } 

  return;
}

/*==============================================================================

FUNCTION NAME

  reg_nv_read_equivalent_plmn_list

==============================================================================*/

void reg_nv_read_equivalent_plmn_list
(
  sys_plmn_id_s_type*   rplmn_p,
  sys_plmn_list_s_type* eplmn_list_p
)
{
  uint8 i;

  MSG_HIGH_DS_0(REG_SUB, "=REG= Read equivalent PLMN list from cache");
    
  reg_nv_output_eplmn_list();
  
  rplmn_p->identity[0] = reg_nv_equivalent_plmn_list.rplmn.identity[0];
  rplmn_p->identity[1] = reg_nv_equivalent_plmn_list.rplmn.identity[1];
  rplmn_p->identity[2] = reg_nv_equivalent_plmn_list.rplmn.identity[2];
  
    eplmn_list_p->length = MIN(reg_nv_equivalent_plmn_list.length,
                             NV_EQUIVALENT_PLMN_LIST_MAX_SIZE
                             + NV_EXTENDED_EQUIVALENT_PLMN_LIST_MAX_SIZE);
  eplmn_list_p->length = MIN(eplmn_list_p->length,
                             SYS_PLMN_LIST_MAX_LENGTH);
  
  for (i = 0; i < eplmn_list_p->length; i++)
  {
     /*get additional EPLMNs from the extended list*/
      if(i >= NV_EQUIVALENT_PLMN_LIST_MAX_SIZE)/*after this use new extended list*/
      {
        byte j = (byte)MIN(NV_EXTENDED_EQUIVALENT_PLMN_LIST_MAX_SIZE-1, i- NV_EQUIVALENT_PLMN_LIST_MAX_SIZE);
        eplmn_list_p->plmn[i].identity[0] = reg_nv_extended_equivalent_plmn_list.plmn_identity[j][0];
        eplmn_list_p->plmn[i].identity[1] = reg_nv_extended_equivalent_plmn_list.plmn_identity[j][1];
        eplmn_list_p->plmn[i].identity[2] = reg_nv_extended_equivalent_plmn_list.plmn_identity[j][2];
      }
      else 
      {
        eplmn_list_p->plmn[i].identity[0] = reg_nv_equivalent_plmn_list.plmn[i].identity[0];
        eplmn_list_p->plmn[i].identity[1] = reg_nv_equivalent_plmn_list.plmn[i].identity[1];
        eplmn_list_p->plmn[i].identity[2] = reg_nv_equivalent_plmn_list.plmn[i].identity[2];
      }
  }

}


/*==============================================================================

FUNCTION NAME

  reg_nv_write_equivalent_plmn_list

==============================================================================*/

void reg_nv_write_equivalent_plmn_list
(
        sys_plmn_id_s_type    rplmn,
  const sys_plmn_list_s_type* eplmn_list_p
)
{
  uint8 i;

  /*
  ** Copy the RPLMN into the static variable to be written to NVRAM.
  */
  reg_nv_equivalent_plmn_list.rplmn.identity[0] = rplmn.identity[0];
  reg_nv_equivalent_plmn_list.rplmn.identity[1] = rplmn.identity[1];
  reg_nv_equivalent_plmn_list.rplmn.identity[2] = rplmn.identity[2];

  /*
  ** Copy the equivalent PLMN list into the static variable
  ** to be written to NVRAM.
  */
  reg_nv_equivalent_plmn_list.length = NV_EQUIVALENT_PLMN_LIST_MAX_SIZE
                                     + NV_EXTENDED_EQUIVALENT_PLMN_LIST_MAX_SIZE;

  if ((uint8)SYS_PLMN_LIST_MAX_LENGTH < reg_nv_equivalent_plmn_list.length)
  {
    reg_nv_equivalent_plmn_list.length = (uint8)SYS_PLMN_LIST_MAX_LENGTH;
  }

  if (eplmn_list_p->length < reg_nv_equivalent_plmn_list.length)
  {
    reg_nv_equivalent_plmn_list.length = (uint8)eplmn_list_p->length;
  }

  for (i = 0; i < reg_nv_equivalent_plmn_list.length; i++)
  {
    /*get additional EPLMNs from the extended list*/
    if(i >= NV_EQUIVALENT_PLMN_LIST_MAX_SIZE)/*after this use new extended list*/
    {
      uint8 j = i- NV_EQUIVALENT_PLMN_LIST_MAX_SIZE;
      reg_nv_extended_equivalent_plmn_list.plmn_identity[j][0] = eplmn_list_p->plmn[i].identity[0];
      reg_nv_extended_equivalent_plmn_list.plmn_identity[j][1] = eplmn_list_p->plmn[i].identity[1];
      reg_nv_extended_equivalent_plmn_list.plmn_identity[j][2] = eplmn_list_p->plmn[i].identity[2];
    }
    else
    {
      reg_nv_equivalent_plmn_list.plmn[i].identity[0] = eplmn_list_p->plmn[i].identity[0];
      reg_nv_equivalent_plmn_list.plmn[i].identity[1] = eplmn_list_p->plmn[i].identity[1];
      reg_nv_equivalent_plmn_list.plmn[i].identity[2] = eplmn_list_p->plmn[i].identity[2];
    }
  }

  /*
  ** Undefine the unused PLMN ids contained within the equivalent
  ** PLMN list to make it easier to verify that the NV item has
  ** been properly updated.
  */
  for (i = reg_nv_equivalent_plmn_list.length; i < NV_EQUIVALENT_PLMN_LIST_MAX_SIZE + NV_EXTENDED_EQUIVALENT_PLMN_LIST_MAX_SIZE; i++)
  {
    /*get additional EPLMNs from the extended list*/
    if(i >= NV_EQUIVALENT_PLMN_LIST_MAX_SIZE)/*after this use new extended list*/
    {
      uint8 j = i- NV_EQUIVALENT_PLMN_LIST_MAX_SIZE;
      reg_nv_extended_equivalent_plmn_list.plmn_identity[j][0] = 0xFF;
      reg_nv_extended_equivalent_plmn_list.plmn_identity[j][1] = 0xFF;
      reg_nv_extended_equivalent_plmn_list.plmn_identity[j][2] = 0xFF;
    }
    else
    {
      reg_nv_equivalent_plmn_list.plmn[i].identity[0] = 0xFF;
      reg_nv_equivalent_plmn_list.plmn[i].identity[1] = 0xFF;
      reg_nv_equivalent_plmn_list.plmn[i].identity[2] = 0xFF;
    }
  }

  reg_nv_data_valid = TRUE;

  reg_nv_output_eplmn_list();

  reg_log_umts_nas_eplmn_list
  (
    rplmn,
    eplmn_list_p
  );
}


/*==============================================================================

FUNCTION NAME

  reg_nv_read_rplmnact

==============================================================================*/

void reg_nv_read_rplmnact
(
  nv_rplmnact_type* rplmnact_p
)
{
  MSG_HIGH_DS_2(REG_SUB, "=REG= Read RPLMNACT %d %d from cache", reg_nv_rplmnact.act[0], reg_nv_rplmnact.act[1]);
  rplmnact_p->act[0] = reg_nv_rplmnact.act[0];
  rplmnact_p->act[1] = reg_nv_rplmnact.act[1];
}


/*==============================================================================

FUNCTION NAME

  reg_nv_write_rplmnact

==============================================================================*/

void reg_nv_write_rplmnact
(
  nv_rplmnact_type rplmnact
)
{
  reg_nv_data_valid = TRUE;
  reg_nv_rplmnact = rplmnact;
  
  MSG_HIGH_DS_2(REG_SUB, "=REG= Save RPLMNACT %d %d in cache", reg_nv_rplmnact.act[0], reg_nv_rplmnact.act[1]);

  /* Write the RPLMN ACT to the NV as well*/
  reg_nv_write_rplmnact_cache();
}


/*==============================================================================

FUNCTION NAME

  reg_nv_read_data_roaming

==============================================================================*/
sys_data_roaming_e_type  reg_nv_read_data_roaming
(
#ifdef FEATURE_DUAL_SIM
  sys_modem_as_id_e_type as_id
#else
  void
#endif
)
{
#ifdef FEATURE_DUAL_SIM
  return reg_nv_data_romaing_sim[as_id];
#else
  return reg_nv_data_romaing;
#endif
}

/*==============================================================================

FUNCTION NAME

  reg_nv_write_data_roaming

==============================================================================*/
boolean  reg_nv_write_data_roaming
(
  sys_data_roaming_e_type data_roaming
)
{
  if(reg_nv_data_romaing != data_roaming)
  {
    MSG_HIGH_DS_2(REG_SUB, "=REG= Data roaming indication change from %d to %d",
                reg_nv_data_romaing, data_roaming);
    reg_nv_data_romaing = data_roaming;
    return TRUE;
  }
  return FALSE;
}



/*==============================================================================

FUNCTION NAME

  reg_nv_read_voice_roaming

==============================================================================*/
boolean  reg_nv_read_voice_roaming
(
#ifdef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type   as_id
#endif
)
{
#ifdef FEATURE_DUAL_SIM
 return reg_nv_voice_roaming_sim[as_id];
#else
  return reg_nv_voice_roaming;
#endif
}

/*==============================================================================

FUNCTION NAME

  reg_nv_write_voice_roaming

==============================================================================*/
boolean  reg_nv_write_voice_roaming
(
  boolean voice_roaming
)
{
  if(reg_nv_voice_roaming != voice_roaming)
  {
    MSG_HIGH_DS_2(REG_SUB, "=REG= Voice roaming indication change from %d to %d",
                reg_nv_voice_roaming, voice_roaming);
    reg_nv_voice_roaming = voice_roaming;
    return TRUE;
  }
  return FALSE;
}

/*==============================================================================

FUNCTION NAME

  reg_nv_read_domestic_voice_roaming

==============================================================================*/
boolean  reg_nv_read_domestic_voice_roaming
(
#ifdef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type   as_id
#endif
)
{
#ifdef FEATURE_DUAL_SIM
 return reg_nv_domestic_voice_roaming_sim[as_id];
#else
  return reg_nv_domestic_voice_roaming;
#endif
}

/*==============================================================================

FUNCTION NAME

  reg_nv_write_domestic_voice_roaming

==============================================================================*/
boolean  reg_nv_write_domestic_voice_roaming
(
  boolean domestic_voice_roaming
)
{
  if(reg_nv_domestic_voice_roaming != domestic_voice_roaming)
  {
    MSG_HIGH_DS_2(REG_SUB, "=REG= Domestic voice roaming indication change from %d to %d",
                reg_nv_domestic_voice_roaming, domestic_voice_roaming);
    reg_nv_domestic_voice_roaming = domestic_voice_roaming;
    return TRUE;
  }
  return FALSE;
}

#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
/*==============================================================================

FUNCTION NAME

  reg_nv_read_rplmn

==============================================================================*/

sys_plmn_id_s_type reg_nv_read_rplmn
(
  void
)
{
  return reg_nv_rplmn;
}


/*==============================================================================

FUNCTION NAME

  reg_nv_write_rplmn

==============================================================================*/

void reg_nv_write_rplmn
(
  sys_plmn_id_s_type  rplmn
)
{
    reg_nv_rplmn = rplmn;
}

/*==============================================================================

FUNCTION NAME

  reg_nv_write_rplmn_dup

==============================================================================*/

void reg_nv_write_rplmn_dup
(
  boolean  rplmn_dup
)
{
  reg_nv_rplmn_dup = rplmn_dup;
  MSG_HIGH_DS_1(REG_SUB, "=REG= Save reg_nv_rplmn_dup to cache %d", reg_nv_rplmn_dup);
}

/*==============================================================================

FUNCTION NAME

  reg_nv_read_rplmn_dup

==============================================================================*/

boolean reg_nv_read_rplmn_dup
(
  void
)
{
  return reg_nv_rplmn_dup;
}

#ifdef FEATURE_FEMTO_CSG
/*==============================================================================

FUNCTION NAME

  reg_nv_read_csg_support

==============================================================================*/

sys_csg_support_e_type reg_nv_read_csg_support
(
  void
)
{
  return reg_nv_csg_support;
}
void reg_nv_set_csg_support (sys_csg_support_e_type value, reg_as_id_e_type id)
{
    reg_nv_csg_support_sim[id] = value;
}
#endif

#endif 

/*==============================================================================

FUNCTION NAME

  reg_nv_read_psloci

==============================================================================*/

boolean reg_nv_read_psloci
(
  byte* psloci_p
)
{
  boolean status = FALSE;

  /*
  ** Fill NV command to read RPLMNACT.
  */
  reg_nv_cmd.tcb_ptr    = rex_self();
  reg_nv_cmd.sigs       = REG_NVIF_SIG;
  reg_nv_cmd.done_q_ptr = NULL;
  reg_nv_cmd.cmd        = NV_READ_F;
  reg_nv_cmd.item       = NV_GSM_LOCIGPRS_I;
  reg_nv_cmd.data_ptr   = &reg_nv_item;

  /*
  ** Clear the REG_NVIF_SIG signal.
  */
  (void) rex_clr_sigs(rex_self(), REG_NVIF_SIG);

  /*
  ** Send the command to NV.
  */
  reg_nv_cmd_per_subs(&reg_nv_cmd);

  /*
  ** Wait for NV to set the REG_NVIF_SIG signal.
  */
  (void) reg_wait(REG_NVIF_SIG);

  /*
  ** PSLOCI could not be read from NVRAM.
  */
  if (reg_nv_cmd.status != NV_DONE_S)
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= Reading PS LOCI failed");
  }
  /*
  ** PSLOCI was successfully read from NVRAM.
  */
  else
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= Read PS LOCI");

    psloci_p[0]  = reg_nv_item.gsm_locigprs[0];
    psloci_p[1]  = reg_nv_item.gsm_locigprs[1];
    psloci_p[2]  = reg_nv_item.gsm_locigprs[2];
    psloci_p[3]  = reg_nv_item.gsm_locigprs[3];
    psloci_p[4]  = reg_nv_item.gsm_locigprs[4];
    psloci_p[5]  = reg_nv_item.gsm_locigprs[5];
    psloci_p[6]  = reg_nv_item.gsm_locigprs[6];
    psloci_p[7]  = reg_nv_item.gsm_locigprs[7];
    psloci_p[8]  = reg_nv_item.gsm_locigprs[8];
    psloci_p[9]  = reg_nv_item.gsm_locigprs[9];
    psloci_p[10] = reg_nv_item.gsm_locigprs[10];
    psloci_p[11] = reg_nv_item.gsm_locigprs[11];
    psloci_p[12] = reg_nv_item.gsm_locigprs[12];
    psloci_p[13] = reg_nv_item.gsm_locigprs[13];

    status = TRUE;
  }

  return status;
}


/*==============================================================================

FUNCTION NAME

  reg_nv_write_cache

==============================================================================*/

void reg_nv_write_cache
(
  void
)
{
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;
  if(!reg_nv_data_valid)
  {
    MSG_MED_DS_0(REG_SUB, "=REG= Trying to write NV with uninitialized data - blocked");
    return;
  }
  
  /*
  ** Write RPLMNACT cache to NV
  */
  reg_nv_write_rplmnact_cache();
  
  /*
  ** Fill NV command to write the equivalent PLMN list.
  */
  reg_nv_item.equivalent_plmn_list = reg_nv_equivalent_plmn_list;

  reg_nv_cmd.tcb_ptr    = rex_self();
  reg_nv_cmd.sigs       = REG_NVIF_SIG;
  reg_nv_cmd.done_q_ptr = NULL;
  reg_nv_cmd.cmd        = NV_WRITE_F;
  reg_nv_cmd.item       = NV_EQUIVALENT_PLMN_LIST_I;
  reg_nv_cmd.data_ptr   = &reg_nv_item;

  /*
  ** Clear the REG_NVIF_SIG signal.
  */
  (void) rex_clr_sigs(rex_self(), REG_NVIF_SIG);

  /*
  ** Send the command to NV.
  */
  reg_nv_cmd_per_subs(&reg_nv_cmd);

  /*
  ** Wait for NV to set the REG_NVIF_SIG signal.
  */
  (void) reg_wait(REG_NVIF_SIG);

  if (reg_nv_cmd.status != NV_DONE_S)
  {
    MSG_ERROR_DS_0(REG_SUB, "=REG= Write equivalent PLMN list failed");
  }
  else
  {
    reg_nv_data_valid = FALSE;
    /*
    ** Fill NV command to write the extended equivalent PLMN list.
    */
    reg_nv_item.extended_equivalent_plmn_list = reg_nv_extended_equivalent_plmn_list;

    reg_nv_cmd.tcb_ptr    = rex_self();
    reg_nv_cmd.sigs       = REG_NVIF_SIG;
    reg_nv_cmd.done_q_ptr = NULL;
    reg_nv_cmd.cmd        = NV_WRITE_F;
    reg_nv_cmd.item       = NV_EXTENDED_EQUIVALENT_PLMN_LIST_I;
    reg_nv_cmd.data_ptr   = &reg_nv_item;

    /*
    ** Clear the REG_NVIF_SIG signal.
    */
    (void) rex_clr_sigs(rex_self(), REG_NVIF_SIG);

    /*
    ** Send the command to NV.
    */
    reg_nv_cmd_per_subs(&reg_nv_cmd);

    /*
    ** Wait for NV to set the REG_NVIF_SIG signal.
    */
    (void) reg_wait(REG_NVIF_SIG);

    if (reg_nv_cmd.status != NV_DONE_S)
    {
      MSG_ERROR_DS_0(REG_SUB, "=REG= Wrote equivalent PLMN list and extended equivalent PLMN list failed");
    }
    else
    {
      MSG_HIGH_DS_0(REG_SUB, "=REG= Wrote equivalent PLMN list and Wrote extended equivalent PLMN list");
      reg_nv_data_valid = FALSE;
    }
  }
  
  /* Write HPLMN IRAT Search timer */
  status = mcfg_fs_write("/nv/item_files/modem/nas/irat_search_timer",&reg_nv_hplmn_irat_search_timer,sizeof(uint32), MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)reg_sub_id) ;
  if(status != MCFG_FS_STATUS_OK)
  {
    MSG_ERROR_DS_1(REG_SUB, "=REG= Wrote HPLMN IRAT search timer to NV failed status = %d", status);
  }
  else
  {  
    MSG_HIGH_DS_0(REG_SUB, "=REG=  Wrote HPLMN IRAT search timer to NV");
  }


#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
  if(reg_nv_rplmn_dup == FALSE)
  {
    sys_plmn_undefine_plmn_id(&reg_nv_rplmn);
  }
  /* Write RPLMN to NV */
  status = mcfg_fs_write("/nv/item_files/modem/nas/rplmn",&reg_nv_rplmn,sizeof(sys_plmn_id_s_type), MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)reg_as_id);
  if( status != MCFG_FS_STATUS_OK)
  {
    MSG_ERROR_DS_1(REG_SUB, "=REG= Wrote RPLMN to NV failed status = %d", status);
  }
  else
  {  
    MSG_HIGH_DS_3(REG_SUB, "=REG=  Wrote RPLMN to NV %d %d %d", 
                  reg_nv_rplmn.identity[0], 
                  reg_nv_rplmn.identity[1], 
                  reg_nv_rplmn.identity[2]);
  }
#endif   /*FEATURE_FEMTO_CSG*/
  status = mcfg_fs_write("/nv/item_files/modem/nas/data_roam_status",&reg_nv_data_romaing,sizeof(sys_data_roaming_e_type), MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)reg_sub_id);
  if(status != MCFG_FS_STATUS_OK)
  {
    MSG_ERROR_DS_0(REG_SUB, "=REG= Write reg_nv_data_romaing to NV failed");
  }
  else
  {  
    MSG_HIGH_DS_1(REG_SUB, "=REG=  Wrote reg_nv_data_romaing to NV %d",reg_nv_data_romaing);
  }
  //FR 33195
  status = mcfg_fs_write("/nv/item_files/modem/nas/voice_roam_status",&reg_nv_voice_roaming,sizeof(boolean), MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)reg_sub_id);
   if(status != MCFG_FS_STATUS_OK)
   {
      MSG_ERROR_DS_0(REG_SUB, "=REG= Write reg_nv_voice_roaming to NV failed");
   }
   else
   {  
      MSG_HIGH_DS_1(REG_SUB, "=REG=	Wrote reg_nv_voice_roaming to NV %d",reg_nv_voice_roaming);
   }

     status = mcfg_fs_write("/nv/item_files/modem/nas/domestic_voice_roam_status",&reg_nv_domestic_voice_roaming,sizeof(boolean), MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)reg_sub_id);
   if(status != MCFG_FS_STATUS_OK)
   {
      MSG_ERROR_DS_0(REG_SUB, "=REG= Write reg_nv_domestic_voice_roaming to NV failed");
   }
   else
   {  
      MSG_HIGH_DS_1(REG_SUB, "=REG=	Wrote reg_nv_domestic_voice_roaming to NV %d",reg_nv_domestic_voice_roaming);
   }
}


/*==============================================================================

FUNCTION NAME

  reg_nv_read_imsi

==============================================================================*/

boolean reg_nv_read_imsi
(
  reg_nv_imsi_s_type* imsi_p
)
{
  boolean status = FALSE;

  byte i;

  /*
  ** Fill NV command to read IMSI.
  */
  reg_nv_cmd.tcb_ptr    = rex_self();
  reg_nv_cmd.sigs       = REG_NVIF_SIG;
  reg_nv_cmd.done_q_ptr = NULL;
  reg_nv_cmd.cmd        = NV_READ_F;
  reg_nv_cmd.item       = NV_GSMUMTS_IMSI_I;
  reg_nv_cmd.data_ptr   = &reg_nv_item;

  /*
  ** Clear the REG_NVIF_SIG signal.
  */
  (void) rex_clr_sigs(rex_self(), REG_NVIF_SIG);

  /*
  ** Send the command to NV.
  */
  reg_nv_cmd_per_subs(&reg_nv_cmd);

  /*
  ** Wait for NV to set the REG_NVIF_SIG signal.
  */
  (void) reg_wait(REG_NVIF_SIG);

  /*
  ** IMSI could not be read from NVRAM.
  */
  if (reg_nv_cmd.status != NV_DONE_S)
  {
    MSG_HIGH_DS_1(REG_SUB, "=REG= Reading IMSI failed with status: %d", reg_nv_cmd.status);
    reg_debug_nv_imsi_read_error = reg_nv_cmd.status;
  }
  /*
  ** IMSI was successfully read from NVRAM.
  */
  else
  {
    for (i = 0; i < REG_NV_IMSI_MAX_LENGTH; i++)
    {
      imsi_p->digits[i] = reg_nv_item.gsmumts_imsi[i];
    }
    MSG_HIGH_DS_8(REG_SUB, "=REG= IMSI[0] = 0x%02x IMSI[1] = 0x%02x IMSI[2] = 0x%02x  IMSI[3] = 0x%02x \
	                   IMSI[4] = 0x%02x IMSI[5] = 0x%02x IMSI[6] = 0x%02x IMSI[7] = 0x%02x ",imsi_p->digits[0],
					   imsi_p->digits[1],imsi_p->digits[2],imsi_p->digits[3],imsi_p->digits[4],imsi_p->digits[5],
					   imsi_p->digits[6],imsi_p->digits[7]);
    status = TRUE;
  }

  return status;
}

/*==============================================================================

FUNCTION NAME

  reg_nv_write_imsi

==============================================================================*/

boolean reg_nv_write_imsi
(
  reg_nv_imsi_s_type* imsi_p
)
{
  int32 i;

  if(!imsi_p) return FALSE;

  /* Fill NV command to write IMSI. */
  for (i = 0; i < REG_NV_IMSI_MAX_LENGTH; i++)
  {
    reg_nv_item.gsmumts_imsi[i] = imsi_p->digits[i];
    MSG_HIGH_DS_2(REG_SUB, "=REG= IMSI[%u] = 0x%02x", i, imsi_p->digits[i]);
  }

  reg_nv_cmd.tcb_ptr = rex_self();
  reg_nv_cmd.sigs = REG_NVIF_SIG;
  reg_nv_cmd.done_q_ptr = NULL;
  reg_nv_cmd.cmd = NV_WRITE_F;
  reg_nv_cmd.item = NV_GSMUMTS_IMSI_I;
  reg_nv_cmd.data_ptr = &reg_nv_item;

  /* Clear the REG_NVIF_SIG signal. */
  (void) rex_clr_sigs(rex_self(), REG_NVIF_SIG);

  /* Send the command to NV. */
  reg_nv_cmd_per_subs(&reg_nv_cmd);

  /* Wait for NV to set the REG_NVIF_SIG signal. */
  (void) reg_wait(REG_NVIF_SIG);

  if (reg_nv_cmd.status != NV_DONE_S)
  {
    MSG_ERROR_DS_1(REG_SUB, "=REG= Write IMSI failed with status:%d", reg_nv_cmd.status);
    return FALSE;
  }
  else
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= Wrote IMSI");
  }

  return TRUE;
}
/*==============================================================================

FUNCTION NAME

  reg_nv_read_preferred_plmn_list

==============================================================================*/

void  reg_nv_read_preferred_plmn_list
(
  void
)
{
  
  /*
  **EFS NV will contain length of the list in the first byte and length of the list *3 entries 
  **containing valid PLMN IDs. Total length of EFS NV item is 1+REG_EFS_EHPLMN_MAX_LENGTH*3
  */
  byte *preferred_list, *extended_preferred_list;
  sys_plmn_id_s_type preferred_plmn;
  uint32 j;
  mcfg_fs_status_e_type status[8] = {MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR};
  mcfg_fs_status_e_type plmn_status[6] = {MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR,MCFG_FS_STATUS_EFS_ERR};
  if(reg_nv_preferred_plmn_list != NULL)
  {
    modem_mem_free(reg_nv_preferred_plmn_list, MODEM_MEM_CLIENT_NAS);
    reg_nv_preferred_plmn_list = NULL; 
  }
  reg_nv_pref_plmn_list_length = 0;

  preferred_list = (byte*) modem_mem_calloc(1, REG_NV_MAX_PREF_PLMN_LIST*5+1, MODEM_MEM_CLIENT_NAS);
  reg_check_for_null_ptr(preferred_list);
  extended_preferred_list = (byte*) modem_mem_calloc(1, REG_NV_MAX_PREF_PLMN_LIST*5+1, MODEM_MEM_CLIENT_NAS);
  reg_check_for_null_ptr(extended_preferred_list);

  for( j=0; j< REG_NV_EFS_PREFERRED_LIST; j++)
  {
    reg_timers_pet_watchdog();
    switch(j+1)
    {
      case 1:       
       status[0] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn_list", &preferred_list[0], (REG_NV_MAX_PREF_PLMN_LIST*5)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
       plmn_status[0] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn", &preferred_plmn.identity[0], 3, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);     
	   if((plmn_status[0] != MCFG_FS_STATUS_OK) ||
          (((plmn_status[0] == MCFG_FS_STATUS_OK)) && (sys_plmn_id_is_undefined(preferred_plmn))) )
       {
         if(reg_mode_is_usim_compliant() && (status[0] == MCFG_FS_STATUS_OK))
         {
           if(preferred_list[0] > REG_NV_MAX_PREF_PLMN_LIST)
           {
              preferred_list[0] = REG_NV_MAX_PREF_PLMN_LIST;
           }
           if(preferred_list[0] == REG_NV_MAX_PREF_PLMN_LIST)
           {
             status[6] = mcfg_fs_read("/nv/item_files/modem/nas/extended_preferred_plmn_list1", &extended_preferred_list[0], (REG_NV_MAX_PREF_PLMN_LIST*5)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
             if(extended_preferred_list[0] > REG_NV_MAX_PREF_PLMN_LIST)
             {
                extended_preferred_list[0] = REG_NV_MAX_PREF_PLMN_LIST;
             }
           }

           if(( preferred_list[0] <= REG_NV_MAX_PREF_PLMN_LIST) && (preferred_list[0] != 0))
           {
             MSG_HIGH_DS_0(REG_SUB,"=REG= Sim is usim compliant so using old preferred list for CMCC");
             if(( extended_preferred_list[0] <= REG_NV_MAX_PREF_PLMN_LIST) && (extended_preferred_list[0] != 0))
             {
               reg_nv_pref_plmn_list_length = preferred_list[0] + extended_preferred_list[0];
               reg_nv_preferred_plmn_list = (reg_sim_plmn_w_act_s_type*)modem_mem_calloc(1, reg_nv_pref_plmn_list_length*5, MODEM_MEM_CLIENT_NAS);
               reg_check_for_null_ptr((void*)reg_nv_preferred_plmn_list);
               memscpy( reg_nv_preferred_plmn_list, preferred_list[0]*5, &preferred_list[1], preferred_list[0]*5);
               reg_nv_preferred_plmn_list += preferred_list[0];
               memscpy( reg_nv_preferred_plmn_list, extended_preferred_list[0]*5, &extended_preferred_list[1], extended_preferred_list[0]*5);
	       reg_nv_preferred_plmn_list -= preferred_list[0];
             }
             else
             {
               reg_nv_pref_plmn_list_length = preferred_list[0];
               reg_nv_preferred_plmn_list = (reg_sim_plmn_w_act_s_type*)modem_mem_calloc(1, reg_nv_pref_plmn_list_length*5, MODEM_MEM_CLIENT_NAS);
               reg_check_for_null_ptr((void*)reg_nv_preferred_plmn_list);  
               memscpy( reg_nv_preferred_plmn_list, reg_nv_pref_plmn_list_length*5, &preferred_list[1], preferred_list[0]*5);
             }
             modem_mem_free(preferred_list, MODEM_MEM_CLIENT_NAS);
               modem_mem_free(extended_preferred_list, MODEM_MEM_CLIENT_NAS);

             return;
           }
         }
       }   
       break;
      case 2:
       status[1] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn_list1", &preferred_list[0], (REG_NV_MAX_PREF_PLMN_LIST*5)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
       plmn_status[1] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn1", &preferred_plmn.identity[0], 3, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
        break;
      case 3:
       status[2] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn_list2", &preferred_list[0], (REG_NV_MAX_PREF_PLMN_LIST*5)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
       plmn_status[2] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn2", &preferred_plmn.identity[0], 3, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
        break;
      case 4:
       status[3] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn_list3", &preferred_list[0], (REG_NV_MAX_PREF_PLMN_LIST*5)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
       plmn_status[3] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn3", &preferred_plmn.identity[0], 3, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
        break;
      case 5:
       status[4] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn_lis4", &preferred_list[0], (REG_NV_MAX_PREF_PLMN_LIST*5)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
       plmn_status[4] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn4", &preferred_plmn.identity[0], 3, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
        break;
      case 6:
       status[5] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn_list5", &preferred_list[0], (REG_NV_MAX_PREF_PLMN_LIST*5)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
       plmn_status[5] = mcfg_fs_read("/nv/item_files/modem/nas/preferred_plmn5", &preferred_plmn.identity[0], 3, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
        break;
      default:
        break;
    }

    if ((status[j] == MCFG_FS_STATUS_OK) && (plmn_status[j] == MCFG_FS_STATUS_OK))
    {
#ifdef FEATURE_EQUIVALENT_HPLMN

      if(reg_sim_is_ehplmn(preferred_plmn))
#else
      if(sys_plmn_match(reg_sim_read_hplmn(), preferred_plmn))
#endif

      {
        if(preferred_list[0] > REG_NV_MAX_PREF_PLMN_LIST)
        {
          preferred_list[0] = REG_NV_MAX_PREF_PLMN_LIST;
        }
        if((j == 1) && (preferred_list[0] == REG_NV_MAX_PREF_PLMN_LIST))
        {
          status[7] = mcfg_fs_read("/nv/item_files/modem/nas/extended_preferred_plmn_list2", &extended_preferred_list[0], (REG_NV_MAX_PREF_PLMN_LIST*5)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
          if(extended_preferred_list[0] > REG_NV_MAX_PREF_PLMN_LIST)
          {
             extended_preferred_list[0] = REG_NV_MAX_PREF_PLMN_LIST;
          }
        }
	if((j == 0) && (preferred_list[0] == REG_NV_MAX_PREF_PLMN_LIST))
        {
          status[7] = mcfg_fs_read("/nv/item_files/modem/nas/extended_preferred_plmn_list1", &extended_preferred_list[0], (REG_NV_MAX_PREF_PLMN_LIST*5)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
          if(extended_preferred_list[0] > REG_NV_MAX_PREF_PLMN_LIST)
          {
             extended_preferred_list[0] = REG_NV_MAX_PREF_PLMN_LIST;
          }
        }
        /*Update the current preferred list as the preferred list of the current SIM*/
        if(( preferred_list[0] <= REG_NV_MAX_PREF_PLMN_LIST) && (preferred_list[0] != 0))
        {
		  MSG_HIGH_DS_1(REG_SUB,"=REG= PLMN matching with EHPLMN, So using %d list as preferred", j);
          if((j == 1 || j==0) && ( extended_preferred_list[0] <= REG_NV_MAX_PREF_PLMN_LIST) && (extended_preferred_list[0] != 0))
          {
			 
            reg_nv_pref_plmn_list_length = preferred_list[0] + extended_preferred_list[0];
            reg_nv_preferred_plmn_list = (reg_sim_plmn_w_act_s_type*)modem_mem_calloc(1, reg_nv_pref_plmn_list_length*5, MODEM_MEM_CLIENT_NAS);
            reg_check_for_null_ptr((void*)reg_nv_preferred_plmn_list);  
            memscpy( reg_nv_preferred_plmn_list, preferred_list[0]*5, &preferred_list[1], preferred_list[0]*5);
            reg_nv_preferred_plmn_list += preferred_list[0];
            memscpy( reg_nv_preferred_plmn_list, extended_preferred_list[0]*5, &extended_preferred_list[1], extended_preferred_list[0]*5);		   
	    reg_nv_preferred_plmn_list -= preferred_list[0];
          }
          else
          {	
            reg_nv_pref_plmn_list_length = preferred_list[0];
            reg_nv_preferred_plmn_list = (reg_sim_plmn_w_act_s_type*)modem_mem_calloc(1, reg_nv_pref_plmn_list_length*5, MODEM_MEM_CLIENT_NAS);
            reg_check_for_null_ptr((void*)reg_nv_preferred_plmn_list);  
            memscpy( reg_nv_preferred_plmn_list, reg_nv_pref_plmn_list_length*5, &preferred_list[1], preferred_list[0]*5);
          }
          modem_mem_free(preferred_list, MODEM_MEM_CLIENT_NAS);
          modem_mem_free(extended_preferred_list, MODEM_MEM_CLIENT_NAS);

          return;
        }
      }
    }
    else
    {
      continue;
    }
  }
  MSG_HIGH_DS_8(REG_SUB, "=REG= list read status[0] %d, status[1] %d status[2] %d status[3] %d status[4] %d status[5] %d status[6] %d status[7] %d", status[0],status[1],status[2],status[3],status[4],status[5],status[6],status[7]);
  MSG_HIGH_DS_6(REG_SUB, "=REG= list read plmn_status[0] %d, plmn_status[1] %d plmn_status[2] %d plmn_status[3] %d plmn_status[4] %d plmn_status[5] %d", plmn_status[0],plmn_status[1],plmn_status[2],plmn_status[3],plmn_status[4],plmn_status[5]);
  modem_mem_free(preferred_list, MODEM_MEM_CLIENT_NAS);
  modem_mem_free(extended_preferred_list, MODEM_MEM_CLIENT_NAS);

}

/*==============================================================================

FUNCTION NAME

  reg_nv_get_preferred_plmn_list

==============================================================================*/
reg_sim_plmn_w_act_s_type  * reg_nv_get_preferred_plmn_list(uint32 *preferred_plmn_length)
{

  boolean plmn_id_is_undefined;
  boolean mnc_includes_pcs_digit;
  uint32  mcc;
  uint32  mnc;
  int i;
 
  if(reg_nv_preferred_plmn_list == NULL)
  {
    *preferred_plmn_length = 0;    
     return NULL;
  }
    *preferred_plmn_length = reg_nv_pref_plmn_list_length;
     MSG_HIGH_DS_1(REG_SUB, "=REG= NV Preferred PLMN list (length = %d)", reg_nv_pref_plmn_list_length);

     MSG_HIGH_DS_0(REG_SUB, "=REG=  # MCC-MNC  RAT   CAT");
     for (i = 0; i < (int)reg_nv_pref_plmn_list_length; i++)
     {
       //SKIP the undefined plmn
       if(sys_plmn_id_is_undefined(reg_nv_preferred_plmn_list[i].plmn))
       {
         continue;
       }
       sys_plmn_get_mcc_mnc
       (
         reg_nv_preferred_plmn_list[i].plmn,
         &plmn_id_is_undefined,
         &mnc_includes_pcs_digit,
         &mcc,
         &mnc
       ); 

       if((reg_nv_preferred_plmn_list[i].act[0] & 0x80)  &&
           (reg_nv_preferred_plmn_list[i].act[1] == 0x00 ))
       {
         if(mnc_includes_pcs_digit)
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu  UMTS PPLMN", i, mcc, mnc);
         }
         else
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu  UMTS PPLMN", i, mcc, mnc);
         }
       }
       else if((reg_nv_preferred_plmn_list[i].act[0] == 0x00) &&
           (reg_nv_preferred_plmn_list[i].act[1] & 0x80 ))
       {
         if(mnc_includes_pcs_digit)
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu  GSM PPLMN", i, mcc, mnc);
         }
         else
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu  GSM PPLMN", i, mcc, mnc);
         }
       }
       else if((reg_nv_preferred_plmn_list[i].act[0] & 0x80) &&
               (reg_nv_preferred_plmn_list[i].act[1] & 0x80 ))
       {
         if(mnc_includes_pcs_digit)
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu  UMTS-GSM PPLMN", i, mcc, mnc);
         }
         else
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu  UMTS-GSM PPLMN", i, mcc, mnc);
         }
       }
       else if((reg_nv_preferred_plmn_list[i].act[0] == 0x00) &&
               (reg_nv_preferred_plmn_list[i].act[1] == 0x00 ))
       {
         if(mnc_includes_pcs_digit)
         {
#ifdef FEATURE_LTE
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu  UMTS-LTE-GSM PPLMN", i, mcc, mnc);
#else
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu  UMTS-GSM PPLMN", i, mcc, mnc);
#endif
         }
         else
         {
#ifdef FEATURE_LTE
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu  UMTS-LTE-GSM PPLMN", i, mcc, mnc);
#else
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu  UMTS-GSM PPLMN", i, mcc, mnc);
#endif

         }
       }
#ifdef FEATURE_LTE
       else if((reg_nv_preferred_plmn_list[i].act[0] & 0x40) &&
           (reg_nv_preferred_plmn_list[i].act[1] == 0x00 ))
       {
         if(mnc_includes_pcs_digit)
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu  LTE PPLMN", i, mcc, mnc);
         }
         else
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu  LTE PPLMN", i, mcc, mnc);
         }
       }

       else if((reg_nv_preferred_plmn_list[i].act[0] & 0x40) &&
               (reg_nv_preferred_plmn_list[i].act[1] & 0x80 ))
       {
         if(mnc_includes_pcs_digit)
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu  LTE-GSM PPLMN", i, mcc, mnc);
         }
         else
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu  LTE-GSM PPLMN", i, mcc, mnc);
         }
       }
       else if((reg_nv_preferred_plmn_list[i].act[0] & 0xc0) &&
               (reg_nv_preferred_plmn_list[i].act[1] == 0x00 ))
       {
         if(mnc_includes_pcs_digit)
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu  UMTS-LTE PPLMN", i, mcc, mnc);
         }
         else
         {
           MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu  UMTS-LTE PPLMN", i, mcc, mnc);
         }
       }

       else if((reg_nv_preferred_plmn_list[i].act[0] & 0xc0) &&
               (reg_nv_preferred_plmn_list[i].act[1] & 0x80 ))
       {
         if(mnc_includes_pcs_digit)
         {
            MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu-%03lu  UMTS-LTE-GSM PPLMN", i, mcc, mnc);
         }
         else
         {
            MSG_HIGH_DS_3(REG_SUB, "=REG= %2lu %03lu- %02lu  UMTS-LTE-GSM PPLMN", i, mcc, mnc);
         }
       }
   #endif
     }
     return reg_nv_preferred_plmn_list;
}

/*==============================================================================

FUNCTION NAME

  reg_nv_initialize_cache

==============================================================================*/

void reg_nv_initialize_cache(void)
{
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;
  int status_new = 1;
  uint16 wcdma_freq_lock_item = 0;                 
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
  for ( reg_as_id=REG_AS_ID_1; reg_as_id < (reg_as_id_e_type)MAX_NAS_STACKS; reg_as_id++ )
#endif
  {
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
    reg_message_as_id = (byte)(reg_as_id + 1);
    reg_sub_id = reg_sub_id_stack[reg_as_id];
#endif
    /*
    ** Fill NV command to read RPLMNACT.
    */
    reg_nv_cmd.tcb_ptr    = rex_self();
    reg_nv_cmd.sigs       = REG_NVIF_SIG;
    reg_nv_cmd.done_q_ptr = NULL;
    reg_nv_cmd.cmd        = NV_READ_F;
    reg_nv_cmd.item       = NV_RPLMNACT_I;
    reg_nv_cmd.data_ptr   = (nv_item_type*)(&reg_nv_rplmnact);
  
    /*
    ** Clear the REG_NVIF_SIG signal.
    */
    (void) rex_clr_sigs(rex_self(), REG_NVIF_SIG);
  
    /*
    ** Send the command to NV.
    */
    reg_nv_cmd_per_subs(&reg_nv_cmd);
  
    /*
    ** Wait for NV to set the REG_NVIF_SIG signal.
    */
    (void) reg_wait(REG_NVIF_SIG);
  
    /*
    ** RPLMNACT could not be read from NVRAM.
    */
    if (reg_nv_cmd.status != NV_DONE_S)
    {  
      MSG_HIGH_DS_0(REG_SUB, "=REG= Reading RPLMNACT failed");
  
      reg_nv_rplmnact.act[0] = 0;
      reg_nv_rplmnact.act[1] = 0;
    }
    /*
    ** RPLMNACT was successfully read from NVRAM.
    */
    else
    {
      MSG_HIGH_DS_2(REG_SUB, "=REG= Read RPLMNACT %d %d", reg_nv_rplmnact.act[0], reg_nv_rplmnact.act[1]);
      reg_nv_data_valid = TRUE;
    }
  
    
    /*
    ** Fill NV command to read equivalent PLMN list.
    */
    reg_nv_cmd.tcb_ptr    = rex_self();
    reg_nv_cmd.sigs       = REG_NVIF_SIG;
    reg_nv_cmd.done_q_ptr = NULL;
    reg_nv_cmd.cmd        = NV_READ_F;
    reg_nv_cmd.item       = NV_EQUIVALENT_PLMN_LIST_I;
    reg_nv_cmd.data_ptr   = (nv_item_type*)(&reg_nv_equivalent_plmn_list);
  
    /*
    ** Clear the REG_NVIF_SIG signal.
    */
    (void) rex_clr_sigs(rex_self(), REG_NVIF_SIG);
  
    /*
    ** Send the command to NV.
    */
    reg_nv_cmd_per_subs(&reg_nv_cmd);
  
    /*
    ** Wait for NV to set the REG_NVIF_SIG signal.
    */
    (void) reg_wait(REG_NVIF_SIG);
  
    /*
    ** The equivalent PLMN list could not be read from NVRAM so
    ** undefine the output RPLMN and set the output list length to zero.
    */
    if (reg_nv_cmd.status != NV_DONE_S)
    {  
      reg_nv_equivalent_plmn_list.rplmn.identity[0] = 0xFF;
      reg_nv_equivalent_plmn_list.rplmn.identity[1] = 0xFF;
      reg_nv_equivalent_plmn_list.rplmn.identity[2] = 0xFF;
      
      reg_nv_equivalent_plmn_list.length = 0;
  
      MSG_HIGH_DS_0(REG_SUB, "=REG= Reading equivalent PLMN list failed");
    } 
    else
    {
      MSG_HIGH_DS_0(REG_SUB, "=REG= Read equivalent PLMN list");
  
      /*Read the EPLMN extended list if length of EPLMNs is greater than 5*/
      if(reg_nv_equivalent_plmn_list.length > NV_EQUIVALENT_PLMN_LIST_MAX_SIZE)
      {
         reg_nv_equivalent_plmn_list.length = MIN(reg_nv_equivalent_plmn_list.length, NV_EQUIVALENT_PLMN_LIST_MAX_SIZE + NV_EXTENDED_EQUIVALENT_PLMN_LIST_MAX_SIZE);
         /*
         ** Fill NV command to read extended equivalent PLMN list.
         */
         reg_nv_cmd.tcb_ptr    = rex_self();
         reg_nv_cmd.sigs   = REG_NVIF_SIG;
         reg_nv_cmd.done_q_ptr = NULL;
         reg_nv_cmd.cmd    = NV_READ_F;
         reg_nv_cmd.item   = NV_EXTENDED_EQUIVALENT_PLMN_LIST_I;
         reg_nv_cmd.data_ptr   = (nv_item_type*)(&reg_nv_extended_equivalent_plmn_list);
         /*
         ** Clear the REG_NVIF_SIG signal.
         */
         (void) rex_clr_sigs(rex_self(), REG_NVIF_SIG);
  
         /*
         ** Send the command to NV.
         */
         reg_nv_cmd_per_subs(&reg_nv_cmd);
    
         /*
         ** Wait for NV to set the REG_NVIF_SIG signal.
         */
         (void) reg_wait(REG_NVIF_SIG);
       
         /*
         ** The extended equivalent PLMN list could not be read from NVRAM so
         ** undefine the output RPLMN and set the output list length to zero.
         */
         if (reg_nv_cmd.status != NV_DONE_S)
         {
            MSG_HIGH_DS_0(REG_SUB, "=REG= Reading extended equivalent PLMN list failed");
         } 
         else
         {
           MSG_HIGH_DS_0(REG_SUB, "=REG= Read extended equivalent PLMN list");
         }
      }
      else
      {
         MSG_HIGH_DS_1(REG_SUB, "=REG= No need to read extended EPLMN list as length is %d", reg_nv_equivalent_plmn_list.length);
      }
  
      reg_nv_data_valid = TRUE;    
    }
    reg_nv_reset_pm_chg_pending_plmn_rat();
  }

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
  reg_set_as_id(REG_AS_ID_1);
#endif


  reg_nv_read_lte_cell_lists();
  reg_state_start_backoff_lte_search_timer_count(reg_state_lte_non_available_list_ptr);


  status = mcfg_fs_read("/nv/item_files/wcdma/rrc/wcdma_rrc_freq_lock_item", &wcdma_freq_lock_item, sizeof(uint16), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  MSG_HIGH_DS_2(REG_SUB, "=REG= NV wcdma_freq_lock_item read from EFS  with status %d and value is %d",
                status,
                wcdma_freq_lock_item);

  if ((status == MCFG_FS_STATUS_OK )&& (wcdma_freq_lock_item !=0))
  {
    reg_nv_wcdma_freq_lock_enabled = TRUE;
  }
  status = mcfg_fs_read("/nv/item_files/modem/nas/lpm_power_off", &reg_nv_lpm_power_off, sizeof(boolean), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if (status != MCFG_FS_STATUS_OK)
  {
    reg_nv_lpm_power_off = TRUE;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG=  NV write EFS/NV at LPM read and value=%d status = %d",reg_nv_lpm_power_off,status);
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/efrplmnsi_select_rplmn_after_hplmn", &reg_nv_efrplmnsi_selection, sizeof(boolean), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if (status != MCFG_FS_STATUS_OK)
  {
    reg_nv_efrplmnsi_selection = FALSE;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG=  NV read EFRPLMNSI selection =%d status = %d",reg_nv_efrplmnsi_selection,status);

#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
#ifdef FEATURE_DUAL_SIM
  sys_plmn_undefine_plmn_id(&reg_nv_rplmn_sim[REG_AS_ID_1]);
  reg_nv_rplmn_dup_sim[REG_AS_ID_1] = FALSE;
  reg_timers_pet_watchdog();

  status = mcfg_fs_read("/nv/item_files/modem/nas/rplmn", &reg_nv_rplmn_sim[REG_AS_ID_1], sizeof(sys_plmn_id_s_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if(status ==  MCFG_FS_STATUS_OK)
  {
    MSG_HIGH_DS_3(REG_SUB, "=REG=  READ RPLMN from NV %d %d %d",
                  reg_nv_rplmn_sim[REG_AS_ID_1].identity[0],
                  reg_nv_rplmn_sim[REG_AS_ID_1].identity[1],
                  reg_nv_rplmn_sim[REG_AS_ID_1].identity[2]);
    if(!sys_plmn_id_is_undefined(reg_nv_rplmn_sim[REG_AS_ID_1]))
    {
      reg_nv_rplmn_dup_sim[REG_AS_ID_1] = TRUE;
    }
  }

  sys_plmn_undefine_plmn_id(&reg_nv_rplmn_sim[REG_AS_ID_2]);
  reg_nv_rplmn_dup_sim[REG_AS_ID_2] = FALSE;
  reg_timers_pet_watchdog(); 
  status = mcfg_fs_read("/nv/item_files/modem/nas/rplmn", &reg_nv_rplmn_sim[REG_AS_ID_2], sizeof(sys_plmn_id_s_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);

  if(status == MCFG_FS_STATUS_OK)
  {
    MSG_HIGH_DS_3(REG_SUB, "=REG=  READ RPLMN from NV %d %d %d", 
                   reg_nv_rplmn_sim[REG_AS_ID_2].identity[0],
                   reg_nv_rplmn_sim[REG_AS_ID_2].identity[1],
                   reg_nv_rplmn_sim[REG_AS_ID_2].identity[2]);
    if(!sys_plmn_id_is_undefined(reg_nv_rplmn_sim[REG_AS_ID_2]))
    {
      reg_nv_rplmn_dup_sim[REG_AS_ID_2] = TRUE;
    }
  }
#ifdef FEATURE_SGLTE_DUAL_SIM
  sys_plmn_undefine_plmn_id(&reg_nv_rplmn_sim[REG_AS_ID_3]);
  reg_nv_rplmn_dup_sim[REG_AS_ID_3] = FALSE;
  reg_timers_pet_watchdog(); 
  status = mcfg_fs_read("/nv/item_files/modem/nas/rplmn", &reg_nv_rplmn_sim[REG_AS_ID_3], sizeof(sys_plmn_id_s_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_3);

  if(status == MCFG_FS_STATUS_OK)
  {
    MSG_HIGH_DS_3(REG_SUB, "=REG=  READ RPLMN from NV %d %d %d", 
                   reg_nv_rplmn_sim[REG_AS_ID_3].identity[0],
                   reg_nv_rplmn_sim[REG_AS_ID_3].identity[1],
                   reg_nv_rplmn_sim[REG_AS_ID_3].identity[2]);
    if(!sys_plmn_id_is_undefined(reg_nv_rplmn_sim[REG_AS_ID_3]))
    {
      reg_nv_rplmn_dup_sim[REG_AS_ID_3] = TRUE;
    }
  }

#endif
#else
  sys_plmn_undefine_plmn_id(&reg_nv_rplmn);
  reg_nv_rplmn_dup = FALSE;
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/rplmn", &reg_nv_rplmn, sizeof(sys_plmn_id_s_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if(status == MCFG_FS_STATUS_OK)
  {
    MSG_HIGH_DS_3(REG_SUB, "=REG=  READ RPLMN from NV %d %d %d", 
     reg_nv_rplmn.identity[0], reg_nv_rplmn.identity[1], reg_nv_rplmn.identity[2]);
    /* Value read from NV will be defined only if it is different from RPLMN
        So marking duplicate flag TRUE*/
    if(!sys_plmn_id_is_undefined(reg_nv_rplmn))
    {
      reg_nv_rplmn_dup = TRUE;
    }
  }
#endif /*FEATURE_DUAL_SIM*/
#endif /*FEATURE_FEMTO_CSG*/

    status = mcfg_fs_read("/nv/item_files/modem/mmode/qmss_enabled", &reg_is_qrd_device, sizeof(boolean), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
                                                
    if( status == MCFG_FS_STATUS_OK)
    {
      MSG_HIGH_DS_1(REG_SUB,"=REG= setting QRD device setting to %d",reg_is_qrd_device);
    }
    else
    {
      MSG_HIGH_DS_1(REG_SUB,"=REG= setting QRD device to false as read failed status = %d",status);
      reg_is_qrd_device = FALSE;
    }

    if( reg_is_qrd_device == TRUE)
    {
 #ifdef FEATURE_DUAL_SIM
      for ( reg_as_id=REG_AS_ID_1; reg_as_id < (reg_as_id_e_type)MAX_NAS_STACKS; reg_as_id++ )
 #endif
      {
        reg_message_as_id = (byte)(reg_as_id + 1);
        status = reg_nv_read_vplmn_list();                      
 
        if( status == MCFG_FS_STATUS_OK)
        {
          MSG_HIGH_DS_1(REG_SUB, "=REG= NV read VPLMN success Length of VPLMN = %d", reg_nv_get_vplmn_list_length());
        }
        else
        {
           MSG_HIGH_DS_1(REG_SUB, "=REG= NV read VPLMN Falied status = %d", status);
        }
      }
#ifdef FEATURE_DUAL_SIM
     reg_set_as_id(REG_AS_ID_1);
#endif

    }

#ifdef FEATURE_DUAL_SIM
  
   status = mcfg_fs_read("/nv/item_files/modem/nas/allow_hplmn_timer_restart", &reg_nv_allow_hplmn_timer_restart_sim[REG_AS_ID_1], sizeof(boolean), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
 
   MSG_HIGH_DS_2(REG_SUB, "=REG=NV allow_hplmn_timer_restart from EFS with status %d and value %d", status, reg_nv_allow_hplmn_timer_restart_sim[REG_AS_ID_1]);
 
   if (status != MCFG_FS_STATUS_OK)
   {
     reg_nv_allow_hplmn_timer_restart_sim[REG_AS_ID_1] = TRUE;
   }
   
   status = mcfg_fs_read("/nv/item_files/modem/nas/allow_hplmn_timer_restart", &reg_nv_allow_hplmn_timer_restart_sim[REG_AS_ID_2], sizeof(boolean), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);
 
   MSG_HIGH_DS_2(REG_SUB, "=REG=NV allow_hplmn_timer_restart from EFS with status %d and value %d", status, reg_nv_allow_hplmn_timer_restart_sim[REG_AS_ID_2]);
 
   if (status != MCFG_FS_STATUS_OK)
   {
     reg_nv_allow_hplmn_timer_restart_sim[REG_AS_ID_2] = TRUE;
   }

   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer_stored", &reg_nv_t3245_timer_stored_value_sim[REG_AS_ID_1], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

   if(status != MCFG_FS_STATUS_OK)
   {
     reg_nv_t3245_timer_stored_value_sim[REG_AS_ID_1] = REG_T3245_DEFUALT_VALUE;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_t3245_timer_stored_value from EFS  with status %d and value is %d for SIM1",
                 status,
                 reg_nv_t3245_timer_stored_value_sim[REG_AS_ID_1]);
   
   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer_stored", &reg_nv_t3245_timer_stored_value_sim[REG_AS_ID_2], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);

   if(status != MCFG_FS_STATUS_OK)
   {
     reg_nv_t3245_timer_stored_value_sim[REG_AS_ID_2] = REG_T3245_DEFUALT_VALUE;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_t3245_timer_stored_value from EFS with status %d and value is %d for SIM2",
                 status,
                 reg_nv_t3245_timer_stored_value_sim[REG_AS_ID_2]);

#ifdef FEATURE_TRIPLE_SIM
   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer_stored", &reg_nv_t3245_timer_stored_value_sim[REG_AS_ID_3], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_3);


   if(status != MCFG_FS_STATUS_OK)
   {
     reg_nv_t3245_timer_stored_value_sim[REG_AS_ID_3] = REG_T3245_DEFUALT_VALUE;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_t3245_timer_stored_value from EFS with status %d and value is %d for SIM3",
                 status,
                 reg_nv_t3245_timer_stored_value_sim[REG_AS_ID_3]);
#endif
   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer", &reg_nv_validate_sim_timer_value_sim[REG_AS_ID_1], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

   if(status != MCFG_FS_STATUS_OK || reg_nv_validate_sim_timer_value_sim[REG_AS_ID_1] == 0)
   {
     reg_nv_validate_sim_timer_value_sim[REG_AS_ID_1] = REG_TIMERS_MAX_TIMER_VALUE;
   }
   else
   {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_validate_sim_timer_value_sim[REG_AS_ID_1] = reg_nv_validate_sim_timer_value_sim[REG_AS_ID_1] * 1000;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_validate_sim_timer_value from EFS  with status %d and value is %d(ms) for SIM1",
                 status,
                 reg_nv_validate_sim_timer_value_sim[REG_AS_ID_1]);
   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer", &reg_nv_validate_sim_timer_value_sim[REG_AS_ID_2], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);
   
   if(status != MCFG_FS_STATUS_OK || reg_nv_validate_sim_timer_value_sim[REG_AS_ID_2] == 0)
   {
     reg_nv_validate_sim_timer_value_sim[REG_AS_ID_2] = REG_TIMERS_MAX_TIMER_VALUE;
   }
   else
   {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_validate_sim_timer_value_sim[REG_AS_ID_2] = reg_nv_validate_sim_timer_value_sim[REG_AS_ID_2] * 1000;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_validate_sim_timer_value from EFS with status %d and value is %d(ms) for SIM2",
                 status,
                 reg_nv_validate_sim_timer_value_sim[REG_AS_ID_2]);
   
#ifdef FEATURE_TRIPLE_SIM
   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer", &reg_nv_validate_sim_timer_value_sim[REG_AS_ID_3], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_3);


   if(status != MCFG_FS_STATUS_OK || reg_nv_validate_sim_timer_value_sim[REG_AS_ID_3] == 0)
   {
     reg_nv_validate_sim_timer_value_sim[REG_AS_ID_3] = REG_TIMERS_MAX_TIMER_VALUE;
   }
   else
   {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_validate_sim_timer_value_sim[REG_AS_ID_3] = reg_nv_validate_sim_timer_value_sim[REG_AS_ID_3] * 1000;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_validate_sim_timer_value from EFS with status %d and value is %d(ms) for SIM3",
                 status, 
                 reg_nv_validate_sim_timer_value_sim[REG_AS_ID_3]);
#endif
   status = mcfg_fs_read("/nv/item_files/modem/nas/max_validate_sim_counter", &reg_nv_max_validate_sim_counter_sim[REG_AS_ID_1], sizeof(uint8), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

   if(status != MCFG_FS_STATUS_OK || reg_nv_max_validate_sim_counter_sim[REG_AS_ID_1] == 0)
   {
     reg_nv_max_validate_sim_counter_sim[REG_AS_ID_1] = REG_NV_DEFAULT_MAX_VALIDATE_SIM_COUNTER;
   }

   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_max_validate_sim_counter_sim from EFS  with status %d and value is %d for SIM1",
                 status,
                 reg_nv_max_validate_sim_counter_sim[REG_AS_ID_1]);
   status = mcfg_fs_read("/nv/item_files/modem/nas/max_validate_sim_counter", &reg_nv_max_validate_sim_counter_sim[REG_AS_ID_2], sizeof(uint8), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);
   
   if(status != MCFG_FS_STATUS_OK || reg_nv_max_validate_sim_counter_sim[REG_AS_ID_2] == 0)
   {
     reg_nv_max_validate_sim_counter_sim[REG_AS_ID_2] = REG_NV_DEFAULT_MAX_VALIDATE_SIM_COUNTER;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_max_validate_sim_counter_sim from EFS with status %d and value is %d for SIM2",
                 status,
                 reg_nv_max_validate_sim_counter_sim[REG_AS_ID_2]);
   
#ifdef FEATURE_TRIPLE_SIM
   status = mcfg_fs_read("/nv/item_files/modem/nas/max_validate_sim_counter", &reg_nv_max_validate_sim_counter_sim[REG_AS_ID_3], sizeof(uint8), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_3);

   if(status != MCFG_FS_STATUS_OK || reg_nv_max_validate_sim_counter_sim[REG_AS_ID_3] == 0)
   {
     reg_nv_max_validate_sim_counter_sim[REG_AS_ID_3] = REG_NV_DEFAULT_MAX_VALIDATE_SIM_COUNTER;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_max_validate_sim_counter_sim from EFS with status %d and value is %d for SIM3",
                 status,
                 reg_nv_max_validate_sim_counter_sim[REG_AS_ID_3]);
#endif
   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer_test", &reg_nv_t3245_test_timer_value_sim[REG_AS_ID_1], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

   if(status != MCFG_FS_STATUS_OK || reg_nv_t3245_test_timer_value_sim[REG_AS_ID_1] == 0)
   {
     reg_nv_t3245_test_timer_value_sim[REG_AS_ID_1] = REG_TIMERS_MAX_TIMER_VALUE;
   }
   else
   {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_t3245_test_timer_value_sim[REG_AS_ID_1] = reg_nv_t3245_test_timer_value_sim[REG_AS_ID_1] * 1000;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_t3245_test_timer_value from EFS  with status %d and value is %d(ms) for SIM1",
                 status,
                 reg_nv_t3245_test_timer_value_sim[REG_AS_ID_1]);
   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer_test", &reg_nv_t3245_test_timer_value_sim[REG_AS_ID_2], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);
   
   if(status != MCFG_FS_STATUS_OK || reg_nv_t3245_test_timer_value_sim[REG_AS_ID_2] == 0)
   {
     reg_nv_t3245_test_timer_value_sim[REG_AS_ID_2] = REG_TIMERS_MAX_TIMER_VALUE;
   }
   else
   {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_t3245_test_timer_value_sim[REG_AS_ID_2] = reg_nv_t3245_test_timer_value_sim[REG_AS_ID_2] * 1000;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_t3245_test_timer_value from EFS with status %d and value is %d(ms) for SIM2",
                 status,
                 reg_nv_t3245_test_timer_value_sim[REG_AS_ID_2]);

#ifdef FEATURE_TRIPLE_SIM
   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer_test", &reg_nv_t3245_test_timer_value_sim[REG_AS_ID_3], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_3);

   if(status != MCFG_FS_STATUS_OK || reg_nv_t3245_test_timer_value_sim[REG_AS_ID_3] == 0)
   {
     reg_nv_t3245_test_timer_value_sim[REG_AS_ID_3] = REG_TIMERS_MAX_TIMER_VALUE;
   }
   else
   {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_t3245_test_timer_value_sim[REG_AS_ID_3] = reg_nv_t3245_test_timer_value_sim[REG_AS_ID_3] * 1000;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_t3245_test_timer_value from EFS with status %d and value is %d(ms) for SIM3",
                 status,
                 reg_nv_t3245_test_timer_value_sim[REG_AS_ID_3]);
#endif

#else
   status = mcfg_fs_read("/nv/item_files/modem/nas/allow_hplmn_timer_restart", &reg_nv_allow_hplmn_timer_restart, sizeof(boolean), MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)REG_AS_ID_1);

   MSG_HIGH_DS_2(REG_SUB, "=REG=NV allow_hplmn_timer_restart from EFS with status %d and value %d", status, reg_nv_allow_hplmn_timer_restart);

   if(status != MCFG_FS_STATUS_OK)
   {
     reg_nv_allow_hplmn_timer_restart = TRUE;
   }
   
   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer_stored", &reg_nv_t3245_timer_stored_value, sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

   if(status != MCFG_FS_STATUS_OK)
   {
     reg_nv_t3245_timer_stored_value = REG_T3245_DEFUALT_VALUE;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_t3245_timer_stored_value from EFS  with status %d and value is %d",
              status,
              reg_nv_t3245_timer_stored_value);

   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer", &reg_nv_validate_sim_timer_value, sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

    if(status != MCFG_FS_STATUS_OK || reg_nv_validate_sim_timer_value == 0)
   {
     reg_nv_validate_sim_timer_value = REG_TIMERS_MAX_TIMER_VALUE;
   }
    else
   {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_validate_sim_timer_value = reg_nv_validate_sim_timer_value * 1000;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_validate_sim_timer_value from EFS  with status %d and value is %d(ms)",
                 status,
                 reg_nv_validate_sim_timer_value);
   status = mcfg_fs_read("/nv/item_files/modem/nas/max_validate_sim_counter", &reg_nv_max_validate_sim_counter, sizeof(uint8), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

   if(status != MCFG_FS_STATUS_OK)
   {
     reg_nv_max_validate_sim_counter = REG_NV_DEFAULT_MAX_VALIDATE_SIM_COUNTER;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_max_validate_sim_counter from EFS  with status %d and value is %d",
                 status,
                 reg_nv_max_validate_sim_counter);

   status = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer_test", &reg_nv_t3245_test_timer_value, sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

   if(status != MCFG_FS_STATUS_OK || reg_nv_t3245_test_timer_value == 0)
   {
     reg_nv_t3245_test_timer_value = REG_TIMERS_MAX_TIMER_VALUE;
   }
    else
   {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_t3245_test_timer_value = reg_nv_t3245_test_timer_value * 1000;
   }
   MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_t3245_test_timer_value from EFS  with status %d and value is %d",
                 status,
                 reg_nv_t3245_test_timer_value);
#endif

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
  for ( reg_as_id=REG_AS_ID_1; reg_as_id < (reg_as_id_e_type)MAX_NAS_STACKS; reg_as_id++ )
#endif
  {
    reg_message_as_id = (byte)(reg_as_id + 1);
     /* --------------------------------------------------------------------                                
      ** Read the ANITE GCF FLAG value from the NV
      ** ------------------------------------------------------------------*/


  }
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
  reg_set_as_id(REG_AS_ID_1);
#endif

#ifdef FEATURE_DUAL_SIM
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/data_roam_status", &reg_nv_data_romaing_sim[REG_AS_ID_1], sizeof(sys_data_roaming_e_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if(status != MCFG_FS_STATUS_OK )
  {
    reg_nv_data_romaing_sim[REG_AS_ID_1] = SYS_DATA_ROAMING_ON;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_data_romaing from EFS  with status %d and value is %d for SIM1",
                status,
                reg_nv_data_romaing_sim[REG_AS_ID_1]);
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/data_roam_status", &reg_nv_data_romaing_sim[REG_AS_ID_2], sizeof(sys_data_roaming_e_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);
 
  if(status != MCFG_FS_STATUS_OK)
  {
    reg_nv_data_romaing_sim[REG_AS_ID_2] = SYS_DATA_ROAMING_ON;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_data_romaing from EFS with status %d and value is %d for SIM2",
                status,
                reg_nv_data_romaing_sim[REG_AS_ID_2]);
  
#ifdef FEATURE_TRIPLE_SIM
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/data_roam_status", &reg_nv_data_romaing_sim[REG_AS_ID_3], sizeof(sys_data_roaming_e_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_3);
 
  if(status != MCFG_FS_STATUS_OK)
  {
   reg_nv_data_romaing_sim[REG_AS_ID_3] = SYS_DATA_ROAMING_ON;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_data_romaing from EFS with status %d and value is %d for SIM3",
                status,
                reg_nv_data_romaing_sim[REG_AS_ID_3]);

#endif
#else
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/data_roam_status", &reg_nv_data_romaing, sizeof(sys_data_roaming_e_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
  if(status != MCFG_FS_STATUS_OK )
  {
    reg_nv_data_romaing = SYS_DATA_ROAMING_ON;
  }
  MSG_HIGH_DS_2(REG_SUB,"=REG= NV reg_nv_data_romaing from EFS with status %d and value is %d",
                                                                     status,reg_nv_data_romaing);
#endif

#ifdef FEATURE_DUAL_SIM
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/voice_roam_status", &reg_nv_voice_roaming_sim[REG_AS_ID_1], sizeof(boolean), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if(status != MCFG_FS_STATUS_OK )
  {
    reg_nv_voice_roaming_sim[REG_AS_ID_1] = TRUE;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_voice_roaming from EFS  with status %d and value is %d for SIM1",
                status,
                reg_nv_voice_roaming_sim[REG_AS_ID_1]);
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/voice_roam_status", &reg_nv_voice_roaming_sim[REG_AS_ID_2], sizeof(sys_data_roaming_e_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);
 
  if(status != MCFG_FS_STATUS_OK)
  {
    reg_nv_voice_roaming_sim[REG_AS_ID_2] = TRUE;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_voice_roaming from EFS with status %d and value is %d for SIM2",
                status,
                reg_nv_voice_roaming_sim[REG_AS_ID_2]);
  
#ifdef FEATURE_TRIPLE_SIM
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/voice_roam_status", &reg_nv_voice_roaming_sim[REG_AS_ID_3], sizeof(sys_data_roaming_e_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_3);
 
  if(status != MCFG_FS_STATUS_OK)
  {
   reg_nv_voice_roaming_sim[REG_AS_ID_3] = TRUE;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_voice_roaming from EFS with status %d and value is %d for SIM3",
                status,
                reg_nv_voice_roaming_sim[REG_AS_ID_3]);

#endif
#else
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/voice_roam_status", &reg_nv_voice_roaming, sizeof(sys_data_roaming_e_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
  if(status != MCFG_FS_STATUS_OK )
  {
    reg_nv_voice_roaming = TRUE;
  }
  MSG_HIGH_DS_2(REG_SUB,"=REG= NV reg_nv_voice_roaming from EFS with status %d and value is %d",
                                                                     status,reg_nv_voice_roaming);
#endif

#ifdef FEATURE_DUAL_SIM
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/interlace_scan_timer", &reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_1], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if(status != MCFG_FS_STATUS_OK || reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_1] == 0)
  {
     reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_1] = REG_INTERLACE_SCAN_TIMER_DEFUALT_VALUE * 1000;
  }
  else
  {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_1] = reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_1] * 1000;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_interlace_scan_timer_value from EFS with status %d and value is %d(ms) for SIM1",
                 status,
                 reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_1]);

  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/interlace_scan_timer", &reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_2], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);

  if(status != MCFG_FS_STATUS_OK || reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_2] == 0)
  {
     reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_2] = REG_INTERLACE_SCAN_TIMER_DEFUALT_VALUE * 1000;
  }
  else
  {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_2] = reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_2] * 1000;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_interlace_scan_timer_value from EFS with status %d and value is %d(ms) for SIM2",
                 status,
                 reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_2]);
  
#ifdef FEATURE_TRIPLE_SIM
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/interlace_scan_timer", &reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_3], sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_3);
  

  if(status != MCFG_FS_STATUS_OK || reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_3] == 0)
  {
     reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_3] = REG_INTERLACE_SCAN_TIMER_DEFUALT_VALUE * 1000;
  }
  else
  {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_3] = reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_3] * 1000;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_interlace_scan_timer_value from EFS with status %d and value is %d(ms) for SIM3",
                 status,
                 reg_nv_interlace_scan_timer_value_sim[REG_AS_ID_3]);

#endif
#else
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/interlace_scan_timer", &reg_nv_interlace_scan_timer_value, sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if(status != MCFG_FS_STATUS_OK || reg_nv_interlace_scan_timer_value == 0)
  {
     reg_nv_interlace_scan_timer_value = REG_INTERLACE_SCAN_TIMER_DEFUALT_VALUE * 1000;
  }
  else
  {
     //Timer value in EFS is provided in seconds convert to milliseconds.
     reg_nv_interlace_scan_timer_value = reg_nv_interlace_scan_timer_value * 1000;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_interlace_scan_timer_value from EFS with status %d and value is %d(ms)",
                 status,
                 reg_nv_interlace_scan_timer_value);
#endif

#ifdef FEATURE_DUAL_SIM
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/domestic_voice_roam_status", &reg_nv_domestic_voice_roaming_sim[REG_AS_ID_1], sizeof(boolean), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if(status != MCFG_FS_STATUS_OK )
  {
    reg_nv_domestic_voice_roaming_sim[REG_AS_ID_1] = TRUE;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_domestic_voice_roaming_sim from EFS  with status %d and value is %d for SIM1",
                status,
                reg_nv_domestic_voice_roaming_sim[REG_AS_ID_1]);
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/domestic_voice_roam_status", &reg_nv_domestic_voice_roaming_sim[REG_AS_ID_2], sizeof(sys_data_roaming_e_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);
 
  if(status != MCFG_FS_STATUS_OK)
  {
    reg_nv_domestic_voice_roaming_sim[REG_AS_ID_2] = TRUE;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_domestic_voice_roaming_sim from EFS with status %d and value is %d for SIM2",
                status,
                reg_nv_domestic_voice_roaming_sim[REG_AS_ID_2]);
  
#ifdef FEATURE_TRIPLE_SIM
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/domestic_voice_roam_status", &reg_nv_domestic_voice_roaming_sim[REG_AS_ID_3], sizeof(sys_data_roaming_e_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_3);
 
  if(status != MCFG_FS_STATUS_OK)
  {
   reg_nv_domestic_voice_roaming_sim[REG_AS_ID_3] = TRUE;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG= NV reg_nv_domestic_voice_roaming_sim from EFS with status %d and value is %d for SIM3",
                status,
                reg_nv_domestic_voice_roaming_sim[REG_AS_ID_3]);
#endif
#else
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/domestic_voice_roam_status", &reg_nv_domestic_voice_roaming, sizeof(sys_data_roaming_e_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
  if(status != MCFG_FS_STATUS_OK )
  {
    reg_nv_domestic_voice_roaming = TRUE;
  }
  MSG_HIGH_DS_2(REG_SUB,"=REG= NV reg_nv_domestic_voice_roaming from EFS with status %d and value is %d",
                                                                     status,reg_nv_domestic_voice_roaming);
#endif



#if defined FEATURE_DUAL_SIM
  reg_set_as_id(REG_AS_ID_1);
#endif

  reg_nv_reset_pm_chg_pending_plmn_rat();
  status = mcfg_fs_read("/nv/item_files/modem/nas/pm_chg_plmn_rat", &reg_nv_pm_chg_pend_plmn_rat, 
                                   sizeof(reg_nv_pm_chg_pend_plmn_rat), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

#if defined FEATURE_DUAL_SIM
  reg_set_as_id(REG_AS_ID_2);
  reg_nv_reset_pm_chg_pending_plmn_rat();
  status = mcfg_fs_read("/nv/item_files/modem/nas/pm_chg_plmn_rat", &reg_nv_pm_chg_pend_plmn_rat, 
                                   sizeof(reg_nv_pm_chg_pend_plmn_rat), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);
#endif


#if defined FEATURE_DUAL_SIM
  for(reg_sub_id = REG_AS_ID_1; reg_sub_id < MAX_AS_IDS; reg_sub_id++)
#endif
  {
    reg_nv_read_carrier_specific_efs();
  }

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
  reg_set_as_id(REG_AS_ID_1);
#endif

}

#if defined FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION 
  reg_swap_nv_imsi

DESCRIPTION
  Swaps NV IMSI data of current SUB with given SUB data.
  Called when SIM swap is triggered to swap NV data of the SUBs.
	
RETURN VALUE
  None

===========================================================================*/
void reg_swap_nv_imsi
(
  reg_as_id_e_type reg_as_id_target
)
{
  reg_as_id_e_type reg_as_id_current = reg_as_id;
  reg_nv_imsi_s_type current_sim_imsi_from_nv;
  reg_nv_imsi_s_type target_sim_imsi_from_nv;
  boolean last_nv_status = FALSE;

  /* Read IMSI from NV for current SUB */
  last_nv_status = reg_nv_read_imsi(&current_sim_imsi_from_nv);

  /* Change context to target SUB */
  reg_set_as_id(reg_as_id_target);

  /* Read IMSI from NV for target SUB */
  if(last_nv_status)
  {
    last_nv_status = reg_nv_read_imsi(&target_sim_imsi_from_nv);
  }

  /* Write current SUB IMSI into NV of target SUB */
  if(last_nv_status)
  {
    last_nv_status = reg_nv_write_imsi(&current_sim_imsi_from_nv);
  }

  /* Change context to current SUB */
  reg_set_as_id(reg_as_id_current);

  /* Write target SUB IMSI into NV of current SUB */
  if(last_nv_status)
  {
    last_nv_status = reg_nv_write_imsi(&target_sim_imsi_from_nv);
  }

  MSG_HIGH_DS_2(REG_SUB, "=REG= Swapped NV IMSI due to SIM swap for AS IDs: %d - %d ", reg_as_id, reg_as_id_target);

}

/*===========================================================================

FUNCTION 
  reg_handle_sim_swap

DESCRIPTION
  Handles SIM swap scenario. Swaps REG specific NVs and corresponding caches of current SUB
  with the respective data of given SUB.

RETURN VALUE
  none

===========================================================================*/
void reg_handle_sim_swap
(
  reg_as_id_e_type reg_as_id_target
)
{
  reg_as_id_e_type reg_as_id_current = reg_as_id;
  nv_rplmnact_type rplmnact_current, rplmnact_target;
  sys_plmn_id_s_type rplmn_p_current, rplmn_p_target;
#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
  sys_plmn_id_s_type rplmn_current, rplmn_target;
  boolean rplmn_dup_current, rplmn_dup_target;
#endif
  rex_timer_cnt_type reg_nv_t3245_timer_stored_value_current, reg_nv_t3245_timer_stored_value_target;
  reg_nv_vplmn_list_s_type vplmn_list_current, vplmn_list_target;
  sys_plmn_list_s_type eplmn_list_p_current, eplmn_list_p_temp_target;

  /* Cache read for current SUB */
  reg_nv_read_rplmnact(&rplmnact_current);
#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
  rplmn_current = reg_nv_read_rplmn();
  rplmn_dup_current = reg_nv_read_rplmn_dup();
#endif
  reg_nv_t3245_timer_stored_value_current = reg_nv_t3245_timer_stored_value_get();
  reg_nv_get_vplmn_list(&vplmn_list_current);
  reg_nv_read_equivalent_plmn_list(&rplmn_p_current,&eplmn_list_p_current);

  /* Change context to target SUB */
  reg_set_as_id(reg_as_id_target);

  /* Cache read for target SUB */
  reg_nv_read_rplmnact(&rplmnact_target);
#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
  rplmn_target = reg_nv_read_rplmn();
  rplmn_dup_target = reg_nv_read_rplmn_dup();
#endif
  reg_nv_t3245_timer_stored_value_target = reg_nv_t3245_timer_stored_value_get();
  reg_nv_get_vplmn_list(&vplmn_list_target);
  reg_nv_read_equivalent_plmn_list(&rplmn_p_target,&eplmn_list_p_temp_target);

  /* Cache write for target SUB */
  reg_nv_write_rplmnact(rplmnact_current);
#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
  reg_nv_write_rplmn(rplmn_current);
  reg_nv_write_rplmn_dup(rplmn_dup_current);
#endif
  reg_nv_put_vplmn_list(vplmn_list_current);
  reg_nv_write_equivalent_plmn_list(rplmn_p_current,&eplmn_list_p_current);

  /* NV write for target SUB*/
  reg_nv_write_cache();
  reg_nv_write_t3245_to_efs(reg_nv_t3245_timer_stored_value_current);
  if((reg_is_qrd_device == TRUE) && reg_nv_vplmn_list )
  {
    reg_nv_write_vplmn_list();
  }

  /* Change context to current SUB */
  reg_set_as_id(reg_as_id_current);

  /* Cache write for current SUB */
  reg_nv_write_rplmnact(rplmnact_target);
#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
  reg_nv_write_rplmn(rplmn_target);
  reg_nv_write_rplmn_dup(rplmn_dup_target);
#endif
  reg_nv_put_vplmn_list(vplmn_list_target);
  reg_nv_write_equivalent_plmn_list(rplmn_p_target,&eplmn_list_p_temp_target);

  /* NV write for current SUB*/
  reg_nv_write_cache();
  reg_nv_write_t3245_to_efs(reg_nv_t3245_timer_stored_value_target);
  if((reg_is_qrd_device == TRUE) && reg_nv_vplmn_list )
  {
    reg_nv_write_vplmn_list();
  }
  
  /* REG and MM can process sim_available_req in parellel. IMSI need to be swapped by only task (MM or REG) but not by both.
        Whichever task performs swap later, it has to swap NV IMSI. Check if MM already processed SIM swap and swap NV IMSI. */
  NAS_ENTER_CRIT_SECT(mm_sim_swap_crit_sec);
  if(is_sim_swap_event_handled_by_only_one_task((sys_modem_as_id_e_type)reg_as_id))
  {
    /* MM task already processed SIM swap, lets swap NV IMSI also */
    MSG_HIGH_DS_2(REG_SUB, "=REG= SIM swap event is already processed by MM for AS IDs: %d - %d ", reg_as_id, reg_as_id_target);
    reg_swap_nv_imsi(reg_as_id_target);
    set_sim_swap_event_handled_by_only_one_task((sys_modem_as_id_e_type)reg_as_id, FALSE);
	reg_debug_sim_swap_status = REG_DEBUG_SIM_CHANGED_SWAP_DONE_AT_REG_AND_MM;
  }
  else
  {
    /* MM task did not process SIM swap yet, let MM trigger NV IMSI swap */
    set_sim_swap_event_handled_by_only_one_task((sys_modem_as_id_e_type)reg_as_id, TRUE);
	reg_debug_sim_swap_status = REG_DEBUG_SIM_CHANGED_SWAP_DONE_AT_REG;
    MSG_HIGH_DS_2(REG_SUB, "=REG= SIM swap event is not processed by MM yet for AS IDs: %d - %d ", reg_as_id, reg_as_id_target);
  }
  NAS_EXIT_CRIT_SECT(mm_sim_swap_crit_sec);

}
#endif

/*===========================================================================

FUNCTION reg_nv_allow_hplmn_timer_restart_value_get

DESCRIPTION
  Returns allow_hplmn_timer_restart that is read from the NV.

DEPENDENCIES
  none

===========================================================================*/

boolean reg_nv_allow_hplmn_timer_restart_value_get
(
  sys_modem_as_id_e_type as_id
)
{
#ifdef FEATURE_DUAL_SIM
    MSG_HIGH_DS_2(REG_SUB, "=REG= reg_nv_allow_hplmn_timer_restart is %d, As id %d", reg_nv_allow_hplmn_timer_restart_sim[as_id], as_id);
    return reg_nv_allow_hplmn_timer_restart_sim[as_id];
#else
    MSG_HIGH_DS_1(REG_SUB, "=REG= reg_nv_allow_hplmn_timer_restart is %d", reg_nv_allow_hplmn_timer_restart);
    return reg_nv_allow_hplmn_timer_restart;
#endif
}

/*===========================================================================

FUNCTION reg_nv_t3245_timer_stored_value_get

DESCRIPTION
  Returns T3245 that is read from the NV.

DEPENDENCIES
  none

===========================================================================*/

rex_timer_cnt_type reg_nv_t3245_timer_stored_value_get
(
  void
)
{
    MSG_HIGH_DS_1(REG_SUB, "=REG= reg_nv_t3245_timer_stored_value is %d", reg_nv_t3245_timer_stored_value);
  return reg_nv_t3245_timer_stored_value;
}

/*===========================================================================

FUNCTION reg_nv_read_efnas_config

DESCRIPTION
  Reads EFNASCONFIG from NV.

DEPENDENCIES
  none

===========================================================================*/

void reg_nv_read_efnas_config
(
  void
)
{
    byte  *ef_nasconfig_p = NULL;
    mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;
  ef_nasconfig_p = (byte *)modem_mem_calloc(1, (unsigned int)REG_NV_EFNAS_CONFIG_LENGTH, MODEM_MEM_CLIENT_NAS);

  if(ef_nasconfig_p == NULL)
  {
    MSG_ERROR_DS_0(REG_SUB, "=REG= Could not allocate the memory");    
    return;
  }


  status = mcfg_fs_read("/nv/item_files/modem/nas/efnas_config", ef_nasconfig_p,REG_NV_EFNAS_CONFIG_LENGTH, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_as_id);

    MSG_HIGH_DS_1(REG_SUB, "=REG= NV reg_nv_efnas_config from EFS with status %d",status);

  if(status == MCFG_FS_STATUS_OK)
  {
    reg_mode_parse_nas_config(ef_nasconfig_p, REG_NV_EFNAS_CONFIG_LENGTH);
  }

  modem_mem_free(ef_nasconfig_p, MODEM_MEM_CLIENT_NAS);

  ef_nasconfig_p = NULL;
}



/*===========================================================================

FUNCTION reg_nv_validate_sim_timer_value_get

DESCRIPTION
  Returns T3245 that is read from the NV.

DEPENDENCIES
  none

===========================================================================*/

rex_timer_cnt_type reg_nv_validate_sim_timer_value_get
(
  void
)
{
  MSG_HIGH_DS_1(REG_SUB, "=REG= reg_nv_validate_sim_timer_value value %d", reg_nv_validate_sim_timer_value);
  return reg_nv_validate_sim_timer_value;
}


/*===========================================================================

FUNCTION reg_nv_t3245_test_timer_value_get

DESCRIPTION
  Returns T3245 that is read from the NV.
  This value should only be used for test purposes.

DEPENDENCIES
  none

===========================================================================*/

rex_timer_cnt_type reg_nv_t3245_test_timer_value_get
(
  void
)
{
  MSG_HIGH_DS_1(REG_SUB, "=REG= reg_nv_t3245_test_timer_value value %d", reg_nv_t3245_test_timer_value);
  return reg_nv_t3245_test_timer_value;
}

/*===========================================================================

FUNCTION reg_nv_interlace_scan_timer_value_get

DESCRIPTION
  Returns interlace scan timer value that is read from the NV.

DEPENDENCIES
  none

===========================================================================*/

rex_timer_cnt_type reg_nv_interlace_scan_timer_value_get
(
  void
)
{
  MSG_HIGH_DS_1(REG_SUB, "=REG= reg_nv_interlace_scan_timer_value value %d", reg_nv_interlace_scan_timer_value);
  return reg_nv_interlace_scan_timer_value;
}

/*==============================================================================

FUNCTION NAME

  reg_nv_write_t3245_to_efs

==============================================================================*/

void reg_nv_write_t3245_to_efs(rex_timer_cnt_type t3245_value)
{
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;

  reg_nv_t3245_timer_stored_value = t3245_value;
  status = mcfg_fs_write("/nv/item_files/modem/nas/t3245_timer_stored",&t3245_value,sizeof(rex_timer_cnt_type), MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)reg_as_id);
  if( status != MCFG_FS_STATUS_OK)
  {
     MSG_ERROR_DS_1(REG_SUB, "=REG= Writing to NV for T3245 timer failed status = %d", status);
  }
  else
  {
    MSG_HIGH_DS_1(REG_SUB, "=REG=  Wrote %d value for timer T3245 to NV", t3245_value);
  }
}


/*===========================================================================

FUNCTION reg_nv_read_enhanced_hplmn_srch_tbl

DESCRIPTION
  Read ENHANCED_HPLMN_SRCH_TBL from NV.
  If read fails, populate default tbl.

DEPENDENCIES
  none

RETURN VALUE
  Whether NV read was successful or not.

SIDE EFFECTS
  none

===========================================================================*/
void reg_nv_read_enhanced_hplmn_srch_tbl
(
  byte* nv_config_tbl
)
{

  /*
  ** Fill NV command to read IMSI.
  */
  reg_nv_cmd.tcb_ptr    = rex_self();
  reg_nv_cmd.sigs       = REG_NVIF_SIG;
  reg_nv_cmd.done_q_ptr = NULL;
  reg_nv_cmd.cmd        = NV_READ_F;
  reg_nv_cmd.item       = NV_ENHANCED_HPLMN_SRCH_TBL_I;
  reg_nv_cmd.data_ptr   = &reg_nv_item;

  /*
  ** Clear the REG_NVIF_SIG signal.
  */
  (void) rex_clr_sigs(rex_self(), REG_NVIF_SIG);

  /*
  ** Send the command to NV.
  */
  reg_nv_cmd_per_subs(&reg_nv_cmd);

  /*
  ** Wait for NV to set the REG_NVIF_SIG signal.
  */
  (void) reg_wait(REG_NVIF_SIG);

  /*
  ** IMSI could not be read from NVRAM.
  */
  if (reg_nv_cmd.status != NV_DONE_S)
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= Reading ENHANCED_HPLMN_SRCH_TBL failed");
    /* Check ENS feature and populate default table if needed */
    reg_nv_populate_default_enhanced_hplmn_srch_tbl(nv_config_tbl);
  }
  /*
  ** TBL was successfully read from NVRAM.
  */
  else
  {
      memscpy((void*)nv_config_tbl, REG_NV_CONFIG_TBL_MAX_SIZE, (void*)(reg_nv_item.enhanced_hplmn_srch_tbl), REG_NV_CONFIG_TBL_MAX_SIZE);
      MSG_MED_DS_3(REG_SUB, "=REG= REG NV Bytes: %d, %d, %d", reg_nv_item.enhanced_hplmn_srch_tbl[0], reg_nv_item.enhanced_hplmn_srch_tbl[1], reg_nv_item.enhanced_hplmn_srch_tbl[2]);
  }
}

#ifdef FEATURE_LTE
/*===========================================================================

FUNCTION reg_nv_read_epsloci

DESCRIPTION
  Read EPSLOCI from EFS

DEPENDENCIES
  none

RETURN VALUE
  Whether NV read was successful or not.

SIDE EFFECTS
  none

===========================================================================*/

boolean reg_nv_read_epsloci(byte *epsloci)
{
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;
 
  status = mcfg_fs_read("/nvm/alpha/modem/nas/lte_nas_eps_loci", epsloci,REG_EPSLOCI_LENGTH, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_sub_id);
  MSG_HIGH_DS_1(REG_SUB, "=REG=  EPSLOCI read status = %d ", status);

  return (status == MCFG_FS_STATUS_OK);  
}

#ifdef FEATURE_MMSS_3_1
/*===========================================================================

FUNCTION reg_nv_read_ignore_uplmn

DESCRIPTION
 Whether to ignore uplmn or not

DEPENDENCIES
  none

RETURN VALUE

SIDE EFFECTS
  none

===========================================================================*/
boolean reg_nv_read_ignore_uplmn(void)
{
  MSG_HIGH_DS_1(REG_SUB, "=REG= Ignore UPLMN = %d", reg_nv_ignore_uplmn);
  return reg_nv_ignore_uplmn;
}
#endif /*FEATURE_MMSS_3_1*/

#endif /*FEATURE_LTE*/

#ifdef FEATURE_EQUIVALENT_HPLMN
/*===========================================================================

FUNCTION reg_nv_read_efs_ehplmn_file

DESCRIPTION
  Read EHPLMN list from EFS

  Reading from EFS NV failed or NV is set with invalid EHPLMN list length
  (length greater than max supported length 20 or length equal to 0) this function will set ehplmn_list length as 0.
  WHEN NV read is successful and EHPLMN list length is valid this function will copy the EHPLMN list from NV
  into the ehplmn_list.


DEPENDENCIES
  none

RETURN VALUE
  TRUE/FALSE depending on whether valid EHPLMN list is successfully read from NV or not.

SIDE EFFECTS
  none

===========================================================================*/
boolean  reg_nv_read_efs_ehplmn_file
(
  reg_sim_plmn_list_s_type   *ehplmn_list
)
{
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;


  /*
  **EFS NV will contain length of the list in the first byte and length of the list *3 entries 
  **containing valid PLMN IDs. Total length of EFS NV item is 1+REG_EFS_EHPLMN_MAX_LENGTH*3
  */
  byte               plmn_list[REG_EFS_EHPLMN_MAX_LENGTH*3+1];
  sys_plmn_id_s_type       hplmn;
  uint32 i,j;
  boolean                  found_plmn = FALSE;
  reg_as_id_e_type    tmp_as_id ;

  hplmn = reg_sim_read_hplmn();
  for( j=0; j< REG_NV_EFS_EHPLMN_LIST; j++)
  {
    reg_timers_pet_watchdog();
    switch(j+1)
    {
      case 1:       
       status = mcfg_fs_read("/nv/item_files/modem/nas/ehplmn", &plmn_list[0], (REG_EFS_EHPLMN_MAX_LENGTH*3)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
       tmp_as_id = REG_AS_ID_1;
       break;
      case 2:
       status = mcfg_fs_read("/nv/item_files/modem/nas/ehplmn", &plmn_list[0], (REG_EFS_EHPLMN_MAX_LENGTH*3)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);
       tmp_as_id = REG_AS_ID_2;
        break;
      case 3:
       status = mcfg_fs_read("/nv/item_files/modem/nas/ehplmn1", &plmn_list[0], (REG_EFS_EHPLMN_MAX_LENGTH*3)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
        break;
      case 4:
       status = mcfg_fs_read("/nv/item_files/modem/nas/ehplmn2", &plmn_list[0], (REG_EFS_EHPLMN_MAX_LENGTH*3)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
        break;
      case 5:
       status = mcfg_fs_read("/nv/item_files/modem/nas/ehplmn3", &plmn_list[0], (REG_EFS_EHPLMN_MAX_LENGTH*3)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
        break;
      case 6:
       status = mcfg_fs_read("/nv/item_files/modem/nas/ehplmn4", &plmn_list[0], (REG_EFS_EHPLMN_MAX_LENGTH*3)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
        break;
      case 7:
       status = mcfg_fs_read("/nv/item_files/modem/nas/ehplmn5", &plmn_list[0], (REG_EFS_EHPLMN_MAX_LENGTH*3)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
        break;
      default:
        break;
    }
    ehplmn_list->length=0;
    if (status == MCFG_FS_STATUS_OK)
    {
      MSG_MED_DS_1(REG_SUB,"=REG= Reading from EFS NV succeeded %d",status);
      /*Update EHPLMN list length as first byte read from the NV*/
      ehplmn_list->length = plmn_list[0];
      if(( ehplmn_list->length <= REG_EFS_EHPLMN_MAX_LENGTH) && (ehplmn_list->length != 0))
      {
        MSG_HIGH_DS_1(REG_SUB, "=REG= Received valid EHPLMN list length %d", ehplmn_list->length);
        memscpy( &ehplmn_list->plmn[0], sizeof(sys_plmn_id_s_type)*REG_SIM_PLMN_LIST_LENGTH, &plmn_list[1], ehplmn_list->length*3);
      //case 1 or 2 to check usim compliance, else match EHPLMN list with IMSI HPLMN. 
      if(( j< 2 ) && (tmp_as_id == reg_sub_id) && reg_mode_is_usim_compliant())
      {
        found_plmn = TRUE;
        MSG_HIGH_DS_0(REG_SUB, "=REG= SIM compliant use the EHPLMNs");
        break;
      }
        for (i = 0; i < ehplmn_list->length; ++i)
        {
          if ( sys_plmn_match( hplmn, ehplmn_list->plmn[i]))   
          {              
            found_plmn = TRUE;
            MSG_HIGH_DS_0(REG_SUB, "=REG= Match between HPLMN IMSI and EHPLMN");
            break;
          }
        }
      }
      else
      {
        MSG_HIGH_DS_1(REG_SUB, "=REG= Invalid EHPLMN list length %d is received ", ehplmn_list->length);
        /*EHPLMN list length is invalid. update length as 0*/
        ehplmn_list->length=0;
      }     
    }
    else
    {
      MSG_HIGH_DS_1(REG_SUB, "=REG= Reading from EFS EHPLMN NV failed %d", status);
    }
    if(found_plmn)          
    {
      break;
    }
    else
    {
      continue;
    }
  }

  if(found_plmn == FALSE)
  {
    ehplmn_list->length=0;
  }
  return (ehplmn_list->length != 0);
}
/*===========================================================================

FUNCTION reg_nv_read_tdscdma_plmn_file

DESCRIPTION
  Read PLMN list from EFS and populates a local database

  Used for TDSCDMA carrier specific requests


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void  reg_nv_read_tdscdma_plmn_file (void)
{
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;

 
  /*
  **EFS NV will contain length of the list in the first byte and length of the list *3 entries 
  **containing valid PLMN IDs. Total length of EFS NV item is 1+REG_EFS_EHPLMN_MAX_LENGTH*3
  */
  byte               plmn_list[REG_EFS_EHPLMN_MAX_LENGTH*3+1];
  
  

  NAS_ENTER_CRIT_SECT(reg_tdscdma_op_list_crit_sect);
  status = mcfg_fs_read("/nv/item_files/modem/nas/tdscdma_op_plmn_list", &plmn_list[0],(REG_EFS_EHPLMN_MAX_LENGTH*3)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if(status != MCFG_FS_STATUS_OK)
  {
    status = mcfg_fs_read("/nv/item_files/modem/nas/tdscdma_op_plmn_list", &plmn_list[0],(REG_EFS_EHPLMN_MAX_LENGTH*3)+1, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_2);
  }
  
  reg_nv_tdscdma_op_plmn_efs_list.length = 0;
  if (status == MCFG_FS_STATUS_OK)
  {
    MSG_MED_DS_0(REG_SUB, "=REG= Reading from TDSCDMA OP PLMN LIST EFS NV succeeded ");
    /*Update PLMN list length as first byte read from the NV*/
    reg_nv_tdscdma_op_plmn_efs_list.length = plmn_list[0];
    if(( reg_nv_tdscdma_op_plmn_efs_list.length <= REG_EFS_EHPLMN_MAX_LENGTH) && (reg_nv_tdscdma_op_plmn_efs_list.length != 0))
    {
      MSG_HIGH_DS_1(REG_SUB, "=REG= Read a valid TDSCDMA_PLMN list length %d", reg_nv_tdscdma_op_plmn_efs_list.length);
      memscpy( &reg_nv_tdscdma_op_plmn_efs_list.plmn[0],sizeof(sys_plmn_id_s_type)*REG_SIM_PLMN_LIST_LENGTH, &plmn_list[1], reg_nv_tdscdma_op_plmn_efs_list.length*3);      
    }
    else
    {
      MSG_HIGH_DS_1(REG_SUB, "=REG= Invalid TDSCDMA PLMN list length %d is received ", 
                    reg_nv_tdscdma_op_plmn_efs_list.length);
      /*PLMN list length is invalid. update length as 0*/
      reg_nv_tdscdma_op_plmn_efs_list.length=0;
    }
  }
  else
  {
    MSG_HIGH_DS_1(REG_SUB, "=REG= Reading from EFS PLMN NV failed status = %d", status);
  }
  NAS_EXIT_CRIT_SECT(reg_tdscdma_op_list_crit_sect);
}

#endif

void reg_nv_read_lte_cell_lists(void)
{
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;

  uint32 data_size = 0;
  uint32 total_size=0;
  byte *byte_array = NULL;

  
  struct fs_stat  nas_conf_stat;

//  memset((void *)nas_conf_stat,0,sizeof(nas_conf_stat));

  nas_conf_stat.st_size = 0;
  if ( mcfg_fs_stat ("/nv/item_files/modem/nas/lte_avaialable_cell_list",&nas_conf_stat,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)REG_AS_ID_1) != MCFG_FS_STATUS_OK)
  {
      MSG_HIGH_DS_0(REG_SUB, "=REG= efs stat failed for older efs file lte_avaialable_cell_list");
  }
  else
  {
      MSG_HIGH_DS_0(REG_SUB,"=REG= Older efs file exists :lte_avaialable_cell_list - Deleting it");
      status = mcfg_fs_delete("/nv/item_files/modem/nas/lte_avaialable_cell_list", MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
  }
  nas_conf_stat.st_size = 0;

  status = mcfg_fs_stat ("/nv/item_files/modem/nas/lte_available_tg_cell_list",&nas_conf_stat,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if (status != MCFG_FS_STATUS_OK)
  {
    MSG_HIGH_DS_1(REG_SUB,"=REG= LTE IRAT BPLMN : LTE Available List mcfg stat failed ,status %d", status);
  }
  else
  {
    MSG_HIGH_DS_1(REG_SUB, "=REG= LTE IRAT BPLMN : LTE Available List lenght of file is %d ",nas_conf_stat.st_size);           
  }

  total_size = nas_conf_stat.st_size;
  
  byte_array = (byte * )modem_mem_calloc(1, total_size,MODEM_MEM_CLIENT_NAS);
  status = mcfg_fs_read("/nv/item_files/modem/nas/lte_available_tg_cell_list", byte_array, total_size, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if (status == MCFG_FS_STATUS_OK)
  {
    memscpy((void *)&data_size,sizeof(data_size),(void*)&byte_array[0],sizeof(uint32));
    if (data_size == 0 )
    {
      MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : LTE avaialble list total size is zero no reading further");             
    }
    else
    {
      reg_state_lte_available_list_ptr = NULL;
      reg_state_lte_available_list_ptr = reg_convert_byte_array_to_lte_cell_list(reg_state_lte_available_list_ptr,byte_array,TRUE);
    }
  } 
  else
  {
    MSG_HIGH_DS_1(REG_SUB, "=REG= LTE IRAT BPLMN : lte avaialble list efs reading fail, error value %d ", efs_errno);
  }

  if(byte_array != NULL)
  {
    modem_mem_free(byte_array,MODEM_MEM_CLIENT_NAS);
  }
  
  data_size = 0;
  total_size=0; 
  byte_array = NULL;
  status =  MCFG_FS_STATUS_EFS_ERR;


 // memset((void *)nas_conf_stat,0,sizeof(nas_conf_stat));

  nas_conf_stat.st_size = 0;

  if( mcfg_fs_stat ("/nv/item_files/modem/nas/lte_non_avaialable_cell_list",&nas_conf_stat,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)REG_AS_ID_1) != MCFG_FS_STATUS_OK)
  {
      MSG_HIGH_DS_0(REG_SUB, "=REG= efs stat failed for older mcfg file lte_non_avaialable_cell_list");
  }
  else
  {
      MSG_HIGH_DS_0(REG_SUB,"=REG= Older file exists :lte_non_avaialable_cell_list - Deleting it");
      status = mcfg_fs_delete("/nv/item_files/modem/nas/lte_non_avaialable_cell_list", MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);
  }


  nas_conf_stat.st_size = 0;
  status = mcfg_fs_stat ("/nv/item_files/modem/nas/lte_non_available_tg_cell_list",&nas_conf_stat,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if (status != MCFG_FS_STATUS_OK)
  {
    MSG_HIGH_DS_1(REG_SUB,"=REG= LTE IRAT BPLMN : LTE Non Available List mcfg stat failed, status %d ", status);
  }
  else
  {
    MSG_HIGH_DS_1(REG_SUB, "=REG= LTE IRAT BPLMN : LTE Non Available List lenght of file is %d", nas_conf_stat.st_size);
  }

  total_size = nas_conf_stat.st_size;

  byte_array = (byte * )modem_mem_calloc(1, total_size,MODEM_MEM_CLIENT_NAS);
  status = mcfg_fs_read("/nv/item_files/modem/nas/lte_non_available_tg_cell_list", byte_array, total_size, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);

  if (status == MCFG_FS_STATUS_OK)
  {
    memscpy((void *)&data_size,sizeof(data_size),(void*)&byte_array[0],sizeof(uint32));
    if (total_size == 0 )
    {
      MSG_HIGH_DS_0(REG_SUB,"=REG= LTE IRAT BPLMN : LTE not avaialble list total size is zero no reading further");            
    }
    else
    {
      reg_state_lte_non_available_list_ptr = NULL;
      reg_state_lte_non_available_list_ptr = reg_convert_byte_array_to_lte_cell_list(reg_state_lte_non_available_list_ptr,byte_array,FALSE);
    }
  } 
  else
  {
    MSG_HIGH_DS_1(REG_SUB, "=REG= LTE IRAT BPLMN : LTE not avaialble list list efs reading fail error number %d", efs_errno);
  }
  if(byte_array != NULL)
  {
    modem_mem_free(byte_array,MODEM_MEM_CLIENT_NAS);
  }

}
void  reg_nv_write_lte_lists_to_efs(void)
{
  mcfg_fs_status_e_type efs_status = MCFG_FS_STATUS_EFS_ERR;

  uint32 data_size=0;
  byte *byte_array = NULL;
//  int i;

  MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : Trying to Write LTE available list"); 
  byte_array = reg_convert_lte_list_into_array(reg_state_lte_available_list_ptr,&data_size,TRUE);
  if (byte_array == NULL)
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : LTE Available Array memory failed efs writing fail");                                     
  }
  else
  {
/*    for(i =0; i<byte_array[0]; i++)
    {
      MSG_HIGH_DS_1(REG_SUB, "=REG= LTE IRAT BPLMN : LTE Available byte_array[i] = %d", byte_array[i]);                                                          
    }*/
    efs_status = mcfg_fs_write("/nv/item_files/modem/nas/lte_available_tg_cell_list",byte_array,data_size, MCFG_FS_O_RDWR|MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)REG_AS_ID_1);
    if( efs_status != MCFG_FS_STATUS_OK)
    {
      MSG_HIGH_DS_1(REG_SUB, "=REG= LTE IRAT BPLMN : LTE Avaialble list mcfg writing failed status = %d", efs_status);
    }
    else
    {
      MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : lte avaialble list efs writing succeed");
    }    
  }

  if(byte_array != NULL)
  {
    modem_mem_free(byte_array,MODEM_MEM_CLIENT_NAS);
  }
  
  efs_status = 0;
  data_size=0;
  byte_array = NULL;
  MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : Trying to Write LTE non available list to EFS"); 
  byte_array = reg_convert_lte_list_into_array(reg_state_lte_non_available_list_ptr,&data_size,FALSE);
  if (byte_array == NULL)
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : LTE Non Avaialble list Array memmory failed efs writing fail");                                          
  }
  else
  {
 /*   for(i =0; i<byte_array[0]; i++)
    {
       MSG_HIGH_DS_2(REG_SUB, "=REG= LTE IRAT BPLMN : LTE Non Avaialble list byte_array %d = %d", i, byte_array[i]);
    }*/
    efs_status = mcfg_fs_write("/nv/item_files/modem/nas/lte_non_available_tg_cell_list",byte_array,data_size, MCFG_FS_O_RDWR|MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)REG_AS_ID_1);

    if(efs_status != MCFG_FS_STATUS_OK)
    {
       MSG_HIGH_DS_1(REG_SUB, "=REG= LTE IRAT BPLMN : LTE Non Avaialble list mcfg writing failed error no = %d", efs_status);                        
    }
    else
    {
       MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : lte not avaialble list efs writing succeed");
    }
  }
  if(byte_array != NULL)
  {
    modem_mem_free(byte_array,MODEM_MEM_CLIENT_NAS);
  }
//  reg_state_clear_lte_lists();
//  reg_timers_stop_backoff_lte_search_period_timer();
}

void reg_nv_clear_non_available_efs_list(void)
{
  uint32 length = 0;
  uint32 data_size = sizeof(uint32);

  if(mcfg_fs_write("/nv/item_files/modem/nas/lte_non_available_tg_cell_list",&length,data_size, MCFG_FS_O_RDWR|MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)REG_AS_ID_1)!= MCFG_FS_STATUS_OK)
  {
     MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : LTE Non Avaialble list efs writing fail");
  }
  reg_timers_stop_backoff_lte_search_period_timer();
}

void reg_nv_clear_available_efs_list(void)
{
  uint32 length = 0;
  uint32 data_size = sizeof(uint32);
  if(mcfg_fs_write("/nv/item_files/modem/nas/lte_available_tg_cell_list",&length,data_size, MCFG_FS_O_RDWR|MCFG_FS_O_CREAT | MCFG_FS_O_AUTODIR, MCFG_FS_ALLPERMS,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)REG_AS_ID_1)!= MCFG_FS_STATUS_OK)
  {
    MSG_HIGH_DS_0(REG_SUB, "=REG= LTE IRAT BPLMN : LTE Avaialble list efs writing fail");                         
  }
}

/*===========================================================================

FUNCTION reg_nv_imp_variable_prints

DESCRIPTION
 print important efs once rather than always in servcie confimation/inidcation path

DEPENDENCIES
  none

RETURN VALUE

SIDE EFFECTS
  none

===========================================================================*/
void reg_nv_imp_variable_prints()
{
    MSG_HIGH_DS_3(REG_SUB, "=REG= NV IMSI Switch = %d, reg_nv_is_w_bplmn_pseudo_enabled = %d reg_nv_gcf_flag value set to %d", reg_nv_imsi_switch, reg_nv_is_w_bplmn_pseudo_enabled,mcfg_gcf_nv_get_status());
#if defined(FEATURE_FEMTO_CSG) || defined (FEATURE_NAS_ECALL)
    MSG_HIGH_DS_3(REG_SUB, "=REG= RPLMN  %x %x %x in cache",  
                reg_nv_rplmn.identity[0], 
                reg_nv_rplmn.identity[1], 
                reg_nv_rplmn.identity[2]);
#endif
}

/*===========================================================================

FUNCTION reg_nv_is_imsi_switch_enabled

DESCRIPTION
 Whether to Switch the IMSI or not after trying registration on all the available PLMNs

DEPENDENCIES
  none

RETURN VALUE

SIDE EFFECTS
  none

===========================================================================*/
boolean reg_nv_is_imsi_switch_enabled()
{

  return reg_nv_imsi_switch;
}

/*===========================================================================

FUNCTION reg_nv_is_wcdma_freq_lock_enabled

DESCRIPTION
 Whether to Enable the UE to lock to ATT enaled WCDMA FREQ or NOT

DEPENDENCIES
  none

RETURN VALUE

SIDE EFFECTS
  none

===========================================================================*/
boolean reg_nv_is_wcdma_freq_lock_enabled()
{
  MSG_HIGH_DS_1(REG_SUB, "=REG= NV WCDMA FREQ LOCK enabled flag = %d", reg_nv_wcdma_freq_lock_enabled);
  return reg_nv_wcdma_freq_lock_enabled;
}

/*===========================================================================

FUNCTION reg_nv_is_lpm_power_off_enabled

DESCRIPTION
 Whether to write NV/EFS item on LPM

DEPENDENCIES
  none

RETURN VALUE

SIDE EFFECTS
  none

===========================================================================*/
boolean reg_nv_is_lpm_power_off_enabled(void)
{
  MSG_HIGH_DS_1(REG_SUB, "=REG= LPM power off behavior = %d", reg_nv_lpm_power_off);
  return reg_nv_lpm_power_off;  
}


/*===========================================================================

FUNCTION reg_nv_is_ehplmn_forbid_enabled

DESCRIPTION
 Whether to write NV/EFS item on LPM

DEPENDENCIES
  none

RETURN VALUE

SIDE EFFECTS
  none

===========================================================================*/
#ifdef FEATURE_DUAL_SIM
boolean reg_nv_is_ehplmn_forbid_enabled(sys_modem_as_id_e_type as_id)
#else
boolean reg_nv_is_ehplmn_forbid_enabled(void)
#endif
{

#ifdef FEATURE_DUAL_SIM
  return reg_nv_ehplmn_temp_forbid_enabled_sim[as_id];  
#else
  return reg_nv_ehplmn_temp_forbid_enabled; 
#endif
}


/*===========================================================================

FUNCTION reg_nv_efrplmnsi_select_rplmn

DESCRIPTION
 This function return TRUE is UE should select RPLMN is HPMN is not found. 

DEPENDENCIES
  none

RETURN VALUE

SIDE EFFECTS
  none

===========================================================================*/
boolean reg_nv_efrplmnsi_select_rplmn(void)
{ 
  return reg_nv_efrplmnsi_selection;
}
/*===========================================================================

FUNCTION reg_nv_is_forced_irat_enabled

DESCRIPTION
 This function return TRUE is forced IRAT is enabled. 

DEPENDENCIES
  none

RETURN VALUE

SIDE EFFECTS
  none

===========================================================================*/
boolean reg_nv_is_forced_irat_enabled(void)
{     
    return reg_nv_forced_irat_enabled;
}
/*==============================================================================

FUNCTION NAME

  reg_nv_tdscdma_plmn_list_get

==============================================================================*/
reg_sim_plmn_list_s_type* reg_nv_tdscdma_plmn_list_get(void)
{
       return &reg_nv_tdscdma_op_plmn_efs_list; 
}



/*==============================================================================

FUNCTION NAME

  reg_nv_hplmn_irat_search_timer_set

==============================================================================*/
void reg_nv_hplmn_irat_search_timer_set(uint32 timer)
{
  reg_nv_hplmn_irat_search_timer = timer;
  MSG_HIGH_DS_1(REG_SUB, "=REG= Set HPLMN IRAT NV Search Timer to %d", timer);
}
/*==============================================================================

FUNCTION NAME

  reg_nv_hplmn_irat_search_timer_set

==============================================================================*/
uint32 reg_nv_hplmn_irat_search_timer_get()
{
  return reg_nv_hplmn_irat_search_timer;
}


/*==============================================================================

FUNCTION NAME

  reg_nv_hplmn_cfl_search_timer_get

==============================================================================*/
uint32 reg_nv_hplmn_cfl_search_timer_get(void)
{
  return reg_nv_cfl_search_config.cfl_fast_scan_time + (reg_cfl_hplmn_search_counter * reg_nv_cfl_search_config.scan_time_intv);
}


/*===========================================================================

FUNCTION reg_nv_gcf_flag_get

===========================================================================*/

rex_timer_cnt_type reg_nv_gcf_flag_get
(
  void
)
{
  return mcfg_gcf_nv_get_status();
}

/*==============================================================================

FUNCTION NAME

  reg_mode_ehplmn_list_set

==============================================================================*/
void reg_nv_ehplmn_list_set(reg_sim_plmn_list_s_type ehplmn_list_efs)
{
   uint32 i;   
   reg_nv_ehplmn_efs_list.length = ehplmn_list_efs.length;

   for(i = 0; i < ehplmn_list_efs.length; ++i)
   {
     reg_nv_ehplmn_efs_list.plmn[i] = ehplmn_list_efs.plmn[i];
   }
}


/*==============================================================================

FUNCTION NAME

  reg_nv_ehplmn_list_get

==============================================================================*/
void reg_nv_ehplmn_list_get(reg_sim_plmn_list_s_type* ehplmn_list_efs)
{
   uint32 i;
   
   ehplmn_list_efs->length = reg_nv_ehplmn_efs_list.length;

   for(i = 0; i < reg_nv_ehplmn_efs_list.length; ++i)
   {
     ehplmn_list_efs->plmn[i] = reg_nv_ehplmn_efs_list.plmn[i];
   }
}


/*==============================================================================

FUNCTION NAME

  reg_nv_max_validate_sim_counter_get
  DESCRIPTION
  This function returns the setting of max validate sim counter value read from NV.
==============================================================================*/
uint8 reg_nv_max_validate_sim_counter_get(void)
{
   MSG_HIGH_DS_1(REG_SUB, "=REG= reg_nv_max_validate_sim_counter value set to %d", reg_nv_max_validate_sim_counter);
   return reg_nv_max_validate_sim_counter;
}


#ifdef FEATURE_NAS_ENABLE_LTE_BAND_FILTERING
/*==============================================================================

FUNCTION NAME

  reg_nv_td_op_device_type_get
 
 
DESCRIPTION 
 
  This function returns the setting of TDSCDMA operator device type value read
  from NV.
==============================================================================*/
uint8 reg_nv_td_op_device_type_get(void)
{
  return reg_nv_td_op_device_type;
}
#endif

/*===========================================================================
FUNCTION  reg_nv_read_carrier_specific_efs

DESCRIPTION
  This function reads carrier specific EFS/NV values

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void reg_nv_read_carrier_specific_efs(void)
{
  mcfg_fs_status_e_type status = MCFG_FS_STATUS_EFS_ERR;

/* NV item of REG*/
  reg_nv_items_T  reg_nv_items;

#if defined(FEATURE_LTE) && defined(FEATURE_MMSS_3_1)
  byte ignore_uplmn;
#endif

  struct fs_stat  nas_conf_stat;
  nas_conf_stat.st_size = 0;
  
  /* IGNORE UPLMN */
#if defined(FEATURE_LTE) && defined(FEATURE_MMSS_3_1)
  reg_timers_pet_watchdog();
  reg_nv_ignore_uplmn = FALSE;
  status = mcfg_fs_read("/nv/item_files/modem/nas/ignore_uplmn", &ignore_uplmn, sizeof(boolean), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_sub_id);
  if(status  == MCFG_FS_STATUS_OK && ignore_uplmn >= 1)              
  {
    reg_nv_ignore_uplmn = TRUE;
  }

  MSG_HIGH_DS_2(REG_SUB,"=REG= NV ignore_uplmn read from SIM Ignore UPLMN = %d status = %d",reg_nv_ignore_uplmn, status);
#endif

  /* IMSI SWITCH */
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/imsi_switch", &reg_nv_imsi_switch, sizeof(boolean), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)REG_AS_ID_1);


  if(status != MCFG_FS_STATUS_OK)
  {
    reg_nv_imsi_switch = FALSE;
  }

  MSG_HIGH_DS_2(REG_SUB,"=REG=NV imsi_switch read from EFS with status %d and value is %d",
                         status,reg_nv_imsi_switch);

#ifdef FEATURE_FEMTO_CSG
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/csg_support_configuration", &reg_nv_csg_support, sizeof(sys_csg_support_e_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_sub_id);

  if(status != MCFG_FS_STATUS_OK)
  {
#ifdef FEATURE_DISABLE_CSG_BYDEFAULT
    reg_nv_csg_support = SYS_CSG_SUPPORT_DISABLED;
#else
    reg_nv_csg_support = SYS_CSG_SUPPORT_WCDMA;
#endif
  }

  MSG_HIGH_DS_2(REG_SUB,"=REG=NV csg_support read from EFS with status %d and value is %d",
                         status,reg_nv_csg_support);
#endif

  /* FORCED IRAT */
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/forced_irat", &reg_nv_forced_irat_enabled, sizeof(boolean), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_sub_id);


  if(status !=  MCFG_FS_STATUS_OK)
  { 
    reg_nv_forced_irat_enabled = FALSE;    
  }
   
  MSG_HIGH_DS_2(REG_SUB,"=REG=NV reg_nv_forced_irat_enabled from EFS  with status %d and value is %d",
                          status,reg_nv_forced_irat_enabled);

  /* IRAT SEARCH TIMER */
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/irat_search_timer", &reg_nv_hplmn_irat_search_timer, sizeof(uint32), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_sub_id);


  MSG_HIGH_DS_2(REG_SUB,"=REG=NV reg_nv_hplmn_irat_search_timer from EFS with status %d and value is %d",
                  status,reg_nv_hplmn_irat_search_timer);    
  if(status !=  MCFG_FS_STATUS_OK)
  { 
    reg_nv_hplmn_irat_search_timer = REG_HPLMN_IRAT_SEARCH_TIMER;   /*default value is 5 minutes*/   
  }

#ifdef FEATURE_NAS_ENABLE_LTE_BAND_FILTERING
  reg_timers_pet_watchdog();
  status = mcfg_fs_read("/nv/item_files/modem/nas/td_op_device_type", &reg_nv_td_op_device_type, sizeof(uint8), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_sub_id);

  if (status != MCFG_FS_STATUS_OK)
  {
    reg_nv_td_op_device_type = (uint8)REG_TD_OP_DEVICE_TYPE_2;
  }
  MSG_HIGH_DS_2(REG_SUB, "=REG=  NV read TDSCDMA operator device type =%d status = %d", reg_nv_td_op_device_type, status);  
#endif

reg_timers_pet_watchdog();
	status = mcfg_fs_read("/nv/item_files/modem/nas/ehplmn_temp_forbid", &reg_nv_ehplmn_temp_forbid_enabled, sizeof(boolean), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_sub_id);
  
	MSG_HIGH_DS_2(REG_SUB,"=REG=NV reg_nv_ehplmn_temp_forbid_enabled from EFS with status %d and value is %d",
					status,reg_nv_ehplmn_temp_forbid_enabled);    
	if(status !=  MCFG_FS_STATUS_OK)
	{ 
	  reg_nv_ehplmn_temp_forbid_enabled = FALSE;   
	}

  memset( &reg_nv_items, 0x00, sizeof(reg_nv_items_T) );
  reg_nv_is_hplmn_to_be_selected = FALSE;
  reg_nv_is_roaming_on_in_eplmn = FALSE; 
  reg_nv_is_sim_invalid_recovery_enabled = FALSE;
  reg_nv_is_pseudo_reselection_allowed = TRUE;
  reg_nv_is_w_bplmn_pseudo_enabled = FALSE;
  reg_nv_use_rplmnact_mode_change = FALSE;

  reg_nv_lte_search_config.short_lte_scan = 45;
  reg_nv_lte_search_config.long_lte_scan  = 60;
  reg_nv_lte_search_config.max_lte_failures = 128;
  reg_nv_lte_search_config.max_lte_failures_on_neutral_cell = 2;

  status = mcfg_fs_read("/nv/item_files/modem/nas/reg_nv_items", &reg_nv_items, sizeof(reg_nv_items_T), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_sub_id);
  if(status == MCFG_FS_STATUS_OK)
  {
    reg_nv_is_hplmn_to_be_selected = reg_nv_items.is_hplmn_has_to_be_selected;
    if(reg_nv_items.version >= 1)
    {
      reg_nv_is_roaming_on_in_eplmn = reg_nv_items.is_roaming_on_in_eplmn;
    }
    if(reg_nv_items.version >= 2)
    {
      reg_nv_is_sim_invalid_recovery_enabled = reg_nv_items.is_sim_invalid_recovery_enabled;
    }
    if(reg_nv_items.version >= 3)
    {
      reg_nv_is_camp_on_2g_if_reg_failure = reg_nv_items.is_camp_on_2g_if_reg_failure;
    }
    if(reg_nv_items.version >=4)
    {
      reg_nv_is_pseudo_reselection_allowed = (reg_nv_items.is_pseudo_reselection_allowed > 0) ? TRUE : FALSE;
    }
    if(reg_nv_items.version >= 5)
    {
      reg_nv_is_w_bplmn_pseudo_enabled = reg_nv_items.is_w_bplmn_pseudo_enabled;
    }
    MSG_HIGH_DS_2(REG_SUB,"=REG= LTE IRAT BPLMN reg_nv_is_pseudo_reselection_allowed set to = %d reg_nv_is_w_bplmn_pseudo_enabled = %d",reg_nv_is_pseudo_reselection_allowed, reg_nv_is_w_bplmn_pseudo_enabled);
    status = mcfg_fs_read("/nv/item_files/modem/nas/lte_available_timer_values", &reg_nv_lte_search_config, sizeof(lte_search_timer_feature_config_T), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_sub_id);
    reg_timers_pet_watchdog();
    if (status !=  MCFG_FS_STATUS_OK)
    {
      reg_nv_lte_search_config.short_lte_scan = ((reg_nv_is_pseudo_reselection_allowed == TRUE) || (reg_nv_is_w_bplmn_pseudo_enabled == TRUE)) ? 45 : 3;
      reg_nv_lte_search_config.max_lte_failures = ((reg_nv_is_pseudo_reselection_allowed == TRUE) || (reg_nv_is_w_bplmn_pseudo_enabled == TRUE)) ? 128 : 3;
    }
    else
    {
       MSG_HIGH_DS_3(REG_SUB,"=REG= LTE IRAT BPLMN : EFS read Success setting the values: short time %d long time %d max failures %d",reg_nv_lte_search_config.short_lte_scan,reg_nv_lte_search_config.long_lte_scan,reg_nv_lte_search_config.max_lte_failures);
    }
    MSG_HIGH_DS_4(REG_SUB,"=REG= LTE IRAT BPLMN : EFS short time %d long time %d max failures %d neutral cell failure %d",reg_nv_lte_search_config.short_lte_scan,reg_nv_lte_search_config.long_lte_scan,reg_nv_lte_search_config.max_lte_failures,reg_nv_lte_search_config.max_lte_failures_on_neutral_cell);
    if(reg_nv_items.version >= 6)
    {
      reg_nv_use_rplmnact_mode_change = reg_nv_items.use_rplmnact_mode_change;
      MSG_HIGH_DS_1(REG_SUB,"=REG= NV reg_nv_use_rplmnact_mode_change set to = %d",reg_nv_use_rplmnact_mode_change);
    }
    if(reg_nv_items.version >= 7 && reg_sub_id == REG_AS_ID_1) //read only for sub 1
    {
      reg_nv_is_home_plmn_to_be_selcted_in_home_country = reg_nv_items.is_home_plmn_to_be_selcted_in_home_country;
      MSG_HIGH_DS_1(REG_SUB,"=REG= NV reg_nv_is_home_plmn_to_be_selcted_in_home_country set to = %d",reg_nv_is_home_plmn_to_be_selcted_in_home_country);
    }
  }
  if( reg_is_qrd_device == TRUE)
  {
     reg_nv_is_sim_invalid_recovery_enabled = TRUE;
  }
   MSG_HIGH_DS_3(REG_SUB, "=REG= NV reg_nv_is_hplmn_to_be_selected  = %d, reg_nv_is_roaming_on_in_eplmn = %d , reg_nv_is_sim_invalid_recovery_enabled = %d",
                reg_nv_is_hplmn_to_be_selected,
                reg_nv_is_roaming_on_in_eplmn,
                reg_nv_is_sim_invalid_recovery_enabled);
   MSG_HIGH_DS_1(REG_SUB,"=REG= NV reg_nv_is_camp_on_2g_if_reg_failure = %d",reg_nv_is_camp_on_2g_if_reg_failure);

   reg_timers_pet_watchdog();

   reg_nv_cfl_search_config.max_fast_scan_time = REG_TIMERS_MAX_TIMER_VALUE;
   reg_nv_cfl_search_config.cfl_fast_scan_time = 0;
   reg_cfl_hplmn_search_counter = 0;
  
   status = mcfg_fs_read("/nv/item_files/modem/nas/lte_fast_cfl_search_timer", &reg_nv_cfl_search_config, sizeof(lte_cfl_search_timer_config_T), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_sub_id);

   if ( mcfg_fs_stat ("/nv/item_files/modem/nas/lte_fast_cfl_search_timer",&nas_conf_stat,MCFG_FS_TYPE_EFS,(mcfg_fs_sub_id_e_type)reg_sub_id) != MCFG_FS_STATUS_OK)
   {
     MSG_HIGH_DS_2(REG_SUB,"=REG=  size of efs %u  file size = %d",sizeof(lte_cfl_search_timer_config_T),nas_conf_stat.st_size);
   }
   else
   {
     MSG_HIGH_DS_2( REG_SUB,"=REG= efs file exists  size of efs %u file size = %d",sizeof(lte_cfl_search_timer_config_T),nas_conf_stat.st_size);
   }
  
   if(status !=  MCFG_FS_STATUS_OK)
   { 
     reg_nv_cfl_search_config.cfl_fast_scan_time = 0;   /*default value is 0 to disable feature*/
   }
  
   MSG_HIGH_DS_2(REG_SUB,"=REG=NV reg_nv_cfl_search_config from EFS with status %d and value is %d",
                                                status,reg_nv_cfl_search_config.cfl_fast_scan_time); 

  
}


#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================
FUNCTION  reg_nv_process_mcfg_refresh

DESCRIPTION
  This function refreshes REG specific EFS/NV values for the current subscription

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void reg_nv_process_mcfg_refresh(void)
{
  mcfg_fs_status_e_type status1 = MCFG_FS_STATUS_EFS_ERR, status2 = MCFG_FS_STATUS_EFS_ERR;
  ue_nas_rel_compliance_type reg_nv_nas_rel_compliance;

  reg_nv_read_carrier_specific_efs();
  /* Populate enhanced hplmn search tbl */
  reg_mode_enhanced_hplmn_srch_init();
  /* T3245 TIMER */
  reg_timers_pet_watchdog();
  status1 = mcfg_fs_read("/nv/item_files/modem/nas/t3245_timer", &reg_nv_validate_sim_timer_value,sizeof(rex_timer_cnt_type), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_as_id);


  if(status1 != MCFG_FS_STATUS_OK || reg_nv_validate_sim_timer_value == 0)
  {
    reg_nv_validate_sim_timer_value = REG_TIMERS_MAX_TIMER_VALUE;
  }
  else
  {
    /* Timer value in EFS is provided in seconds convert to milliseconds. */
    reg_nv_validate_sim_timer_value = reg_nv_validate_sim_timer_value * 1000;
  }

  /* MAX VALIDATE SIM COUNTER */
  reg_timers_pet_watchdog();
  status2 = mcfg_fs_read("/nv/item_files/modem/nas/max_validate_sim_counter", &reg_nv_max_validate_sim_counter,sizeof(uint8), MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)reg_as_id);

  if(status2 != MCFG_FS_STATUS_OK || reg_nv_validate_sim_timer_value == 0)
  {
    reg_nv_max_validate_sim_counter = REG_NV_DEFAULT_MAX_VALIDATE_SIM_COUNTER;
  }
  MSG_HIGH_DS_4(REG_SUB,"=REG=NV reg_nv_validate_sim_timer_value EFS read status %d. Value is %d(ms), reg_nv_max_validate_sim_counter_sim EFS read status %d. Value is %d(ms).Refresh Done",
                                   status1,reg_nv_validate_sim_timer_value,
                                   status2,reg_nv_max_validate_sim_counter);

  if(( reg_sim_card_mode == MMGSDI_APP_SIM ) || ( reg_sim_card_mode == MMGSDI_APP_USIM ))
  {
    reg_nv_read_tdscdma_plmn_file();      
    reg_mode_set_usim_compliancy();
    (void)reg_sim_read_ehplmn_list();	  
    reg_nv_read_preferred_plmn_list();
  }
  if (reg_ghdi_nvmem_read(NV_NAS_RELEASE_COMPLIANCE_I,
            (ghdi_nvmem_data_T *)&reg_nv_nas_rel_compliance) != GHDI_SUCCESS)
  {
    reg_nv_nas_rel_compliance = NV_NAS_COMPLIANCE_REL11; 
  }
  else if (reg_nv_nas_rel_compliance >= NV_NAS_COMPLIANCE_REL_MAX)
  {
    MSG_ERROR_DS(MM_SUB, "=REG=  Wrong value for NAS_REL_COMPLIANCE NV %d, default R 11",
         reg_nv_nas_rel_compliance,0,0);
    reg_nv_nas_rel_compliance = NV_NAS_COMPLIANCE_REL11;
  }
  if (reg_nv_nas_rel_compliance > NV_NAS_COMPLIANCE_REL6)
  {
    reg_state_is_pwr_on_manual_plmn_selection = TRUE;
  } 
}
#endif

boolean reg_nv_check_cfl_search_enabled(void)
{
   if ((reg_state_active_rat_get() == SYS_RAT_LTE_RADIO_ACCESS) &&
       (reg_nv_cfl_search_config.cfl_fast_scan_time != 0) &&
       (reg_state_roaming_ind_get() == SYS_ROAM_STATUS_OFF) &&
      ((reg_mode_network_selection_mode_get() == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) ||
       (reg_mode_network_selection_mode_get() == SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION)) &&
       (reg_sim_check_mcc_same_country(reg_state_service_plmn_get()) == TRUE) )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

