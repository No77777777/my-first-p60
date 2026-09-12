/*===========================================================================

                                  R R C T M R

GENERAL DESCRIPTION
 The RRCTMR module consists of RRC timers implementation.

EXTERNALIZED FUNCTIONS



Copyright (c) 2010-2012 Qualcomm Technologies, Incorporated.  
All Rights Reserved.QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrctmr.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/08/11   mkh     Events renamed to TDS
03/10/11   hx      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
08/17/10   rm      Added code to return remaining time for TDSRRCTMR_DED_PRI_VALIDITY_TIMER

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "tdscdma_variation.h"
#include "rex.h"
#include "err.h"
#include "tdsrrccmd_v.h"
#include "tdsrrctmr.h"
#include "tdsrrclogging.h"
#include "tdsrrcsigs.h"
#include "task.h"
#include "tdsCFAParser.h"
/*following lint directive is added to suppress the "ignoring return value" error
only for this file */
/*lint -e534*/
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

static rex_timer_type tdsrrc_t_300;             /* Rex timer T300 */
static rex_timer_type *tdsrrc_t_300_ptr;        /* Pointer Rex timer T300 */

static rex_timer_type tdsrrc_t_308;             /* Rex timer T308 */
static rex_timer_type *tdsrrc_t_308_ptr;        /* Pointer Rex timer T308 */

static rex_timer_type tdsrrc_t_304;             /* Rex timer T304 */
static rex_timer_type *tdsrrc_t_304_ptr;        /* Pointer Rex timer T304 */

static rex_timer_type tdsrrc_t_wait_time;       /* Create Timer for Wait time */
static rex_timer_type *tdsrrc_t_wait_time_ptr;  /* Pointer to Timer for Wait time */

static rex_timer_type tdsrrc_t_ue_to_camp_on;   /* Create Timer for UE to camp on */ 
static rex_timer_type *tdsrrc_t_ue_to_camp_on_ptr;
                                            /* Pointer to Timer for UE to camp on */

static rex_timer_type tdsrrc_t_rrc_con_delay;   /* Create Timer for delaying release of RRC Conn */ 
static rex_timer_type *tdsrrc_t_rrc_con_delay_ptr; /* Pointer to Timer for delaying RRC Conn Rel */
LOCAL rex_timer_type tdsrrc_t_rce_l2ack;       /* Create timer for RCE for L2ACK */
LOCAL rex_timer_type *tdsrrc_t_rce_l2ack_ptr;  /* Pointer to timer for RCE for L2ACK */

static rex_timer_type tdsrrc_t_rcr_l2ack;       /* Create timer for RCR for L2ACK */
static rex_timer_type *tdsrrc_t_rcr_l2ack_ptr;  /* Pointer to timer for RCR for L2ACK */

static rex_timer_type tdsrrc_t_rssi_ber;        /* Create timer for RSSI report */
static rex_timer_type *tdsrrc_t_rssi_ber_ptr;   /* Pointer timer for periodically 
                                               reporting RSSI & BER to CM */

static rex_timer_type tdsrrc_t_sib_wait;        /* Create timer for waiting for
                                               collecting SIBs in a cell. */
static rex_timer_type *tdsrrc_t_sib_wait_ptr;   /* Pointer to timer for waiting for
                                               collecting SIBs in a cell. */
static rex_timer_type tdsrrc_t_302;             /* Rex Timer for T302 */
static rex_timer_type *tdsrrc_t_302_ptr;        /* Pointer Rex timer T302 */

static rex_timer_type tdsrrc_t_305;             /* Rex Timer for T305*/
static rex_timer_type *tdsrrc_t_305_ptr;        /* Pointer Rex timer T305 */

static rex_timer_type tdsrrc_t_316;             /* Rex Timer for T316 */
static rex_timer_type *tdsrrc_t_316_ptr;        /* Pointer Rex timer T316 */

static rex_timer_type tdsrrc_t_317;             /* Rex Timer for T317 */
static rex_timer_type *tdsrrc_t_317_ptr;        /* Pointer Rex timer T317 */

static rex_timer_type tdsrrc_t_307;             /* Rex Timer for T307 */
static rex_timer_type *tdsrrc_t_307_ptr;        /* Pointer Rex timer T307 */

static rex_timer_type tdsrrc_t_314;             /* Rex Timer for T314 */
static rex_timer_type *tdsrrc_t_314_ptr;        /* Pointer Rex timer T314 */

static rex_timer_type tdsrrc_t_315;             /* Rex Timer for T315 */
static rex_timer_type *tdsrrc_t_315_ptr;        /* Pointer Rex timer T315 */

static rex_timer_type tdsrrc_t_sib7_exp;        /* Rex Timer for SIB7 Expiration */
static rex_timer_type *tdsrrc_t_sib7_exp_ptr;   /* Pointer to Rex timer for SIB7 
                                               Expiration */
static rex_timer_type tdsrrc_t_ignore_bcch_mod_exp;       
                                           /* Rex Timer for Ignoring BCCH 
                                              Modifcation Info */
static rex_timer_type *tdsrrc_t_ignore_bcch_mod_exp_ptr; 
                                         /* Pointer to Rex Timer for Ignoring
                                            BCCH Modifcation Info */

static rex_timer_type tdsrrc_t_dch_fach_cell_selection_exp;  /* Rex Timer for 
                                                          * cell selection when
                                                          * going from DCH to FACH
                                                          */    
static rex_timer_type *tdsrrc_t_dch_fach_cell_selection_exp_ptr; /* Pointer to  
                                                              * Rex Timer for 
                                                              * cell selection when
                                                              * going from DCH to FACH
                                                              */    

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
static rex_timer_type tdsrrc_t_fach_to_fach_pch_cell_selection_exp;  /* Rex Timer for 
                                                          * cell selection when
                                                          * going from FACH to FACH/PCH
                                                          */    
static rex_timer_type *tdsrrc_t_fach_to_fach_pch_cell_selection_exp_ptr; /* Pointer to  
                                                              * Rex Timer for 
                                                              * cell selection when
                                                              * going from FACH to FACH/PCH
                                                              */    
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */

static rex_timer_type tdsrrc_t_l1_deadlock_detection_exp;  /* Rex Timer for 
                                                        * L1 deadlock detection.
                                                        */    
static rex_timer_type *tdsrrc_t_l1_deadlock_detection_exp_ptr; /* Pointer to Rex Timer for 
                                                            * L1 deadlock detection.
                                                            */    

static rex_timer_type tdsrrc_t_delay_err_fatal_exp;  /* Rex Timer to delay ERR_FATAL
                                                        */    
static rex_timer_type *tdsrrc_t_delay_err_fatal_exp_ptr; /* Pointer to Rex Timer for 
                                                            * ERR_FATAL delay.
                                                            */    
static rex_timer_type tdsrrc_t_dch_pch_cell_selection_exp;  /* Rex Timer for 
                                                          * cell selection when
                                                          * going from DCH to PCH
                                                          */    
static rex_timer_type *tdsrrc_t_dch_pch_cell_selection_exp_ptr; /* Pointer to  
                                                              * Rex Timer for 
                                                              * cell selection when
                                                              * going from DCH to PCH
                                                              */    

static rex_timer_type tdsrrc_t_3174;             /* Rex Timer for T3174 */
static rex_timer_type *tdsrrc_t_3174_ptr;        /* Pointer Rex timer T3174 */

static rex_timer_type tdsrrc_t_freq_scan_exp;             /* Rex Timer for Freq Scan */
static rex_timer_type *tdsrrc_t_freq_scan_exp_ptr;        /* Pointer Rex timer for Freq Scan */

static rex_timer_type tdsrrc_t_oos;             /* Rex Timer for OOS handling */
static rex_timer_type *tdsrrc_t_oos_ptr;        /* Pointer Rex timer for OOS handling */

static rex_timer_type tdsrrc_t_deep_sleep_no_svc;         /* Rex Timer for Deep Sleep no service handling */
static rex_timer_type *tdsrrc_t_deep_sleep_no_svc_ptr;    /* Pointer Rex timer for Deep Sleep no service handling */

static rex_timer_type tdsrrc_t_phy_chan_failure;       /* Create Timer for Physical chan failure */
static rex_timer_type *tdsrrc_t_phy_chan_failure_ptr;  /* Pointer to Phy chan failure timer */


static rex_timer_type tdsrrc_t_bplmn_srch;             /* Rex Timer for 2G->3G BPLMN search */
static rex_timer_type *tdsrrc_t_bplmn_srch_ptr;        /* Pointer Rex timer 2G->3G BPLMN search */

static rex_timer_type tdsrrc_t_bplmn_full_scan_srch;      /* Rex Timer for 2G->3G BPLMN search full scan*/
static rex_timer_type *tdsrrc_t_bplmn_full_scan_srch_ptr; /* Pointer Rex timer 2G->3G BPLMN search full scan*/


static rex_timer_type tdsrrc_t_freq_redirect_wait_timer; /* Rex timer for inter freq redirection feature*/
static rex_timer_type *tdsrrc_t_freq_redirect_wait_timer_ptr; /* Pointer Rex timer for inter freq redirection feature*/

static rex_timer_type tdsrrc_t_conn_mode_oos_timer; /* Rex timer for OOS in connected mode */
static rex_timer_type *tdsrrc_t_conn_mode_oos_timer_ptr; /* Pointer Rex timer for OOS in connected mode */


static rex_timer_type tdsrrc_t_tdstotds_bplmn_srch;        /* Rex Timer for 3G->3G BPLMN DRX tmr expiry */
static rex_timer_type *tdsrrc_t_tdstotds_bplmn_srch_ptr;   /* Pointer Rex timer 3G->3G DRX tmr expiry */

static rex_timer_type tdsrrc_t_tdstotds_bplmn_guard_srch;      /* Rex Timer for MAX 3G->3G,2G BPLMN search */
static rex_timer_type *tdsrrc_t_tdstotds_bplmn_guard_srch_ptr; /* Pointer Rex timer MAX 3G->3G,2G BPLMN search */

static rex_timer_type tdsrrc_sixhr_sib_update_timer; /* Rex timer for six hrs sib clean up handling */
static rex_timer_type *tdsrrc_sixhr_sib_update_timer_ptr; /* Pointer Rex timer for six hrs sib clean up handling */


static rex_timer_type tdsrrc_t_conn_mode_oos_initial_no_ds_timer; /* Rex timer for initial Tmins continuous search when 
                                                                 UE is OOS in connected mode */
static rex_timer_type *tdsrrc_t_conn_mode_oos_initial_no_ds_timer_ptr; /* Pointer Rex timer for initial Tmins 
                                                                      continuous search */
static rex_timer_type tdsrrc_t_conn_mode_oos_ds_timer; /* Rex timer for OOS in connected mode deep sleep*/
static rex_timer_type *tdsrrc_t_conn_mode_oos_ds_timer_ptr; /* Pointer Rex timer for OOS in connected mode deep sleep */


static rex_timer_type tdsrrc_sib_change_guard_timer; /* Rex timer for sib change guarding with BPLMN*/
static rex_timer_type *tdsrrc_sib_change_guard_timer_ptr; /* Pointer Rex timer for sib change guarding with BPLMN*/

static rex_timer_type tdsrrc_bcch_mod_timer; /* Rex timer for BCCH Modification timer handling */
static rex_timer_type *tdsrrc_bcch_mod_timer_ptr; /* Pointer Rex timer for BCCH Modification timer handling  */


static rex_timer_type tdsrrc_t_gtow_redirection_timer; /* Rex timer for GtoW redirection search */
static rex_timer_type *tdsrrc_t_gtow_redirection_timer_ptr; /* Rex timer for GtoW redirection search */
static rex_timer_type tdsrrc_t_gtow_redir_lfs_timer;
static rex_timer_type *tdsrrc_t_gtow_redir_lfs_timer_ptr;

static rex_timer_type tdsrrc_t_wtog_abort_guard_timer;
static rex_timer_type * tdsrrc_t_wtog_abort_guard_timer_ptr;


static rex_timer_type tdsrrc_t_319;             /* Rex timer T319 */
static rex_timer_type *tdsrrc_t_319_ptr;        /* Pointer Rex timer T319 */

static rex_timer_type tdsrrc_t_320;             /* Rex timer T320 */
static rex_timer_type *tdsrrc_t_320_ptr;        /* Pointer Rex timer T320 */

#ifdef FEATURE_TDSCDMA_REL7_SPECRS

static rex_timer_type tdsrrc_t_couec_timer;
static rex_timer_type *tdsrrc_t_couec_timer_ptr;
#endif /*FEATURE_TDSCDMA_REL7_SPECRS*/

static rex_timer_type tdsrrc_wrm_lock_timer;
static rex_timer_type *tdsrrc_wrm_lock_timer_ptr;
static rex_timer_type tdsrrc_wrm_delay_preemption_timer;
static rex_timer_type *tdsrrc_wrm_delay_preemption_timer_ptr;
static timer_type tdsrrc_dsim_bplmn_lock_rel_off_timer;
static timer_type *tdsrrc_dsim_bplmn_lock_rel_off_timer_ptr;
static rex_timer_type tdsrrc_dsim_wrm_unlock_by_timer;
static rex_timer_type *tdsrrc_dsim_wrm_unlock_by_timer_ptr;

LOCAL rex_timer_type tdsrrc_get_ta_duration_timer;
LOCAL rex_timer_type *tdsrrc_get_ta_duration_timer_ptr;

#if defined(FEATURE_3GPP_FAST_DORMANCY)
static rex_timer_type tdsrrc_t_323;             /* Rex timer T323 */
static rex_timer_type *tdsrrc_t_323_ptr;        /* Pointer Rex timer T323 */
#endif

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
LOCAL rex_timer_type tdsrrc_t_ded_pri_validity_timer;
LOCAL rex_timer_type *tdsrrc_t_ded_pri_validity_timer_ptr;
#endif

#ifdef FEATURE_LTE_TO_TDSCDMA
LOCAL rex_timer_type tdsrrc_t_lte_to_wcdma_redir_search_timer;
LOCAL rex_timer_type *tdsrrc_t_lte_to_wcdma_redir_search_timer_ptr;
#endif

// CR 352866 begin
static rex_timer_type tdsrrc_t_rce_wait_reselect;        /* Rex timer for RCE wait cell reselect */
static rex_timer_type *tdsrrc_t_rce_wait_reselect_ptr;   /* Pointer to Timer for RCE wait cell reselect */
// CR 352866 end

// CR 430034 RAU Reject workaround
static rex_timer_type tdsrrc_t_delay_oos_rlf_rpt_timer;        /* Rex timer for delay OOS/RLF declar to NAS */
static rex_timer_type *tdsrrc_t_delay_oos_rlf_rpt_timer_ptr;   /* Pointer to Timer for delay OOS/RLF declar to NAS */

static rex_timer_type tdsrrc_t_sib_schdule_update_wait_timer;        /* Rex timer for delaying SIB schedule update to L1 */
static rex_timer_type *tdsrrc_t_sib_schdule_update_wait_timer_ptr;   /* Pointer to Timer for delaying SIB schedule update to L1 */

static rex_timer_type tdsrrc_t_sib7_update_wait_timer;        /* Rex timer for sib7 update wait timer */
static rex_timer_type *tdsrrc_t_sib7_update_wait_timer_ptr;   /* Pointer to Timer for sib7 update wait timer */

static rex_timer_type tdsrrc_t_inter_srns_ignore_mr_timer;        /* Rex timer for inter-srns ignore measurement report timer */
static rex_timer_type *tdsrrc_t_inter_srns_ignore_mr_timer_ptr;   /* Pointer to Timer for inter-srns ignore measurement report timer */

static rex_timer_type tdsrrc_t_soft_abort_timer;                /*Rex timer for soft abort protection*/
static rex_timer_type *tdsrrc_t_soft_abort_timer_ptr;           /*Pointer to rex time for soft abort protection*/

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
static rex_timer_type tdsrrc_t_bplmn_partial_report_timer;
static rex_timer_type *tdsrrc_t_bplmn_partial_report_timer_ptr;
#endif
static rex_timer_type tdsrrc_block_cs_resel_nas_api_timer;         /*Rex timer for reselection evaluation using NAS API for CS domain*/
static rex_timer_type *tdsrrc_block_cs_resel_nas_api_timer_ptr;     /*Pointer to rex time for reselection evaluation using NAS API for CS domain*/

static rex_timer_type tdsrrc_block_ps_resel_nas_api_timer;         /*Rex timer for reselection evaluation using NAS API for PS domain*/
static rex_timer_type *tdsrrc_block_ps_resel_nas_api_timer_ptr;     /*Pointer to rex time for reselection evaluation using NAS API for PS domain*/


static rex_timer_type tdsrrc_da_backoff_timer;
static rex_timer_type *tdsrrc_da_backoff_timer_ptr;

static rex_timer_type tdsrrc_ignore_non_exist_mcm_timer;
static rex_timer_type *tdsrrc_ignore_non_exist_mcm_timer_ptr;

static rex_timer_type tdsrrc_abort_resel_to_grr_timer;
static rex_timer_type* tdsrrc_abort_resel_to_grr_timer_ptr;

/* define the td2td manual bplmn search guard timer varables */
static rex_timer_type tdsrrc_td2td_manual_bplmn_guard_srch_timer;
static rex_timer_type *tdsrrc_td2td_manual_bplmn_guard_srch_timer_ptr;
static rex_timer_type tdsrrc_t_sib_stop_part;
static rex_timer_type *tdsrrc_t_sib_stop_part_ptr;

static rex_timer_type tdsrrc_t_sib_mask;
static rex_timer_type *tdsrrc_t_sib_mask_ptr;

static rex_timer_type tdsrrc_t_sib_empty_mask;
static rex_timer_type *tdsrrc_t_sib_empty_mask_ptr;

static rex_timer_type tdsrrc_t_sib_all_one_mask;
static rex_timer_type *tdsrrc_t_sib_all_one_mask_ptr;

static rex_timer_type tdsrrc_t_rce_allow_ta_timer;
static rex_timer_type *tdsrrc_t_rce_allow_ta_timer_ptr;
static rex_timer_type tdsrrc_t_qta_guard_timer;
static rex_timer_type *tdsrrc_t_qta_guard_timer_ptr;
/*Define non deferrable timer group for RRC*/
timer_group_type  tdsrrc_timer_group_non_deferrable;

static rex_timer_type tdsrrc_t_hold_ps_timer;        
static rex_timer_type *tdsrrc_t_hold_ps_timer_ptr;  

/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this ? 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */

/*===========================================================================

FUNCTION tdsrrctmr_get_timer_name

DESCRIPTION
  This function will return timer name by id.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrctmr_get_timer_name(tdsrrctmr_timer_e_type timer, char * timer_name_ptr)
{
  char * name_ptr = NULL;

  switch (timer)
  {
    case TDSRRCTMR_T_300_TIMER:
      name_ptr = "T_300_TIMER";
      break;

    case TDSRRCTMR_T_308_TIMER:
      name_ptr = "T_308_TIMER";
      break;

    case TDSRRCTMR_WAIT_TIME_TIMER:
      name_ptr = "WAIT_TIME_TIMER";
      break;

    case TDSRRCTMR_UE_TO_CAMP_ON_TIMER:
      name_ptr = "UE_TO_CAMP_ON_TIMER";
      break;

    case TDSRRCTMR_RRC_CON_DELAY_TIMER:
      name_ptr = "RRC_CON_DELAY_TIMER";
      break;

    case TDSRRCTMR_RCE_L2ACK_TIMER:
      name_ptr = "RCE_L2ACK_TIMER";
      break;

    case TDSRRCTMR_RCR_L2ACK_TIMER:
      name_ptr = "RCR_L2ACK_TIMER";
      break;

    case TDSRRCTMR_T_304_TIMER:
      name_ptr = "T_304_TIMER";
      break;

    case TDSRRCTMR_RSSI_BER_TIMER:
      name_ptr = "RSSI_BER_TIMER";
      break;

    case TDSRRCTMR_SIB_WAIT_TIMER:
      name_ptr = "SIB_WAIT_TIMER";
      break;

    case TDSRRCTMR_T_302_TIMER:
      name_ptr = "T_302_TIMER";
      break;

    case TDSRRCTMR_T_305_TIMER:
      name_ptr = "T_305_TIMER";
      break;

    case TDSRRCTMR_T_316_TIMER:
      name_ptr = "T_316_TIMER";
      break;

    case TDSRRCTMR_T_317_TIMER:
      name_ptr = "T_317_TIMER";
      break;

    case TDSRRCTMR_T_307_TIMER:
      name_ptr = "T_307_TIMER";
      break;

    case TDSRRCTMR_T_314_TIMER:
      name_ptr = "T_314_TIMER";
      break;

    case TDSRRCTMR_T_315_TIMER:
      name_ptr = "T_315_TIMER";
      break;

    case TDSRRCTMR_SIB7_EXP_TIMER:
      name_ptr = "SIB7_EXP_TIMER";
      break;

    case TDSRRCTMR_IGNORE_BCCH_MOD_TIMER:
      name_ptr = "IGNORE_BCCH_MOD_TIMER";
      break;

    case TDSRRCTMR_DCH_FACH_CELL_SELECTION_TIMER:
      name_ptr = "IGNORE_BCCH_MOD_TIMER";
      break;

    case TDSRRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER:
      name_ptr = "FACH_TO_FACH_PCH_CELL_SELECTION";
      break;

    case TDSRRCTMR_L1_DEADLOCK_DETECT_TIMER:
      name_ptr = "L1_DEADLOCK_DETECT_TIMER";
      break;

    case TDSRRCTMR_DELAY_ERR_FATAL_TIMER:
      name_ptr = "DELAY_ERR_FATAL_TIMER";
      break;

    case TDSRRCTMR_DCH_PCH_CELL_SELECTION_TIMER:
      name_ptr = "DCH_PCH_CELL_SELECTION_TIMER";
      break;

    case TDSRRCTMR_T_3174_TIMER:
      name_ptr = "T_3174_TIMER";
      break;

    case TDSRRCTMR_FREQ_SCAN_TIMER:
      name_ptr = "FREQ_SCAN_TIMER";
      break;

    case TDSRRCTMR_T_OOS_TIMER:
      name_ptr = "T_OOS_TIMER";
      break;

    case TDSRRCTMR_PHY_CHAN_FAILURE_TIMER:
      name_ptr = "PHY_CHAN_FAILURE_TIMER";
      break;

    case TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER:
      name_ptr = "DEEP_SLEEP_NO_SVC_TIMER";
      break;

    case TDSRRCTMR_BPLMN_SRCH_TIMER:
      name_ptr = "BPLMN_SRCH_TIMER";
      break;

    case TDSRRCTMR_BPLMN_FREQ_SCAN_TIMER:
      name_ptr = "BPLMN_FREQ_SCAN_TIMER";
      break;

    case TDSRRCTMR_MANUAL_PLMN_FREQ_SCAN_TIMER:
      name_ptr = "MANUAL_PLMN_FREQ_SCAN_TIMER";
      break;

    case TDSRRCTMR_FREQ_REDIRECT_WAIT_TIMER:
      name_ptr = "FREQ_REDIRECT_WAIT_TIMER";
      break;

    case TDSRRCTMR_CONN_MODE_OOS_TIMER:
      name_ptr = "CONN_MODE_OOS_TIMER";
      break;

    case RRCTMR_TD2TD_BPLMN_DRX_SRCH_TIMER:
      name_ptr = "TD2TD_BPLMN_DRX_SRCH_TIMER";
      break;

    case TDSRRCTMR_BPLMN_GUARD_SRCH_TIMER:
      name_ptr = "BPLMN_GUARD_SRCH_TIMER";
      break;

    case TDSRRCTMR_INACTIVITY_TIMER:
      name_ptr = "INACTIVITY_TIMER";
      break;

    case TDSRRCTMR_SIXHR_SIB_UPDATE_TIMER:
      name_ptr = "SIXHR_SIB_UPDATE_TIMER";
      break;

    case TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER:
      name_ptr = "CONN_MODE_OOS_INITIAL_NO_DS";
      break;

    case TDSRRCTMR_CONN_MODE_OOS_DS_TIMER:
      name_ptr = "CONN_MODE_OOS_DS_TIMER";
      break;

    case TDSRRC_SIB_CHANGE_GUARD_TIMER:
      name_ptr = "SIB_CHANGE_GUARD_TIMER";
      break;

    case TDSRRC_BCCH_MODIFICATION_TIMER:
      name_ptr = "BCCH_MODIFICATION_TIMER";
      break;

    case RRCTMR_G2TD_REDIRECTION_WAIT_TIMER:
      name_ptr = "G2TD_REDIRECTION_WAIT_TIMER";
      break;

    case RRCTMR_G2TD_REDIRECTION_LFS_TIMER:
      name_ptr = "G2TD_REDIRECTION_LFS_TIMER";
      break;

    case RRCTMR_TD2G_ABORT_GUARD_TIMER:
      name_ptr = "TD2G_ABORT_GUARD_TIMER";
      break;

    case TDSRRCTMR_T_319_TIMER:
      name_ptr = "T_319_TIMER";
      break;

    case TDSRRCTMR_T_320_TIMER:
      name_ptr = "T_320_TIMER";
      break;

    case TDSRRCTMR_COUEC_TIMER:
      name_ptr = "COUEC_TIMER";
      break;

    case TDSRRCTMR_WRM_LOCK_TIMER:
      name_ptr = "WRM_LOCK_TIMER";
      break;

    case TDSRRCTMR_WRM_DELAY_PREEMPTION_TIMER:
      name_ptr = "WRM_DELAY_PREEMPTION_TIMER";
      break;

    case TDSRRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER:
      name_ptr = "DSIM_BPLMN_LOCK_REL_OFFSET_TIMER";
      break;

    case TDSRRCTMR_WMR_UNLCOK_BY_TIMER:
      name_ptr = "WMR_UNLCOK_BY_TIMER";
      break;

    case TDSRRCTMR_GET_TA_DURATION_TIMER:
      name_ptr = "GET_TA_DURATION_TIMER";
      break;

    case TDSRRCTMR_T_323_TIMER:
      name_ptr = "T_323_TIMER";
      break;

    case TDSRRCTMR_DED_PRI_VALIDITY_TIMER:
      name_ptr = "DED_PRI_VALIDITY_TIMER";
      break;

    case TDSRRCTMR_LTE_TO_TDSCDMA_REDIR_SRCH_TIMER:
      name_ptr = "LTE_TO_TDSCDMA_REDIR_SRCH_TIMER";
      break;

    case TDSRRCTMR_RCE_WAIT_RESELCT_TIMER:
      name_ptr = "RCE_WAIT_RESELCT_TIMER";
      break;

    case TDSRRCTMR_DELAY_OOS_RLF_RPT_TIMER:
      name_ptr = "DELAY_OOS_RLF_RPT_TIMER";
      break;

    case TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER:
      name_ptr = "SIB_SCHEDULE_UPDATE_WAIT_TIMER";
      break;

    case TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER:
      name_ptr = "SIB7_UPDATE_WAIT_TIMER";
      break;

    case TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER:
      name_ptr = "INTER_SRNS_IGNORE_MR_TIMER";
      break;

    case TDSRRCTMR_SOFT_ABORT_GUARD_TIMER:
      name_ptr = "SOFT_ABORT_GUARD_TIMER";
      break;
      
    case TDSRRCTMR_HOLD_PS_TIMER:
      name_ptr = "HOLD_PS_TIMER";
      break;

    case TDSRRCTMR_BPLMN_PARTIAL_REPORT_TIMER:
      name_ptr = "BPLMN_PARTIAL_REPORT_TIMER";
      break;

    case TDSRRCTMR_BLOCK_CS_RESEL_NAS_API_TIMER:
      name_ptr = "BLOCK_CS_RESEL_NAS_API_TMR";
      break;

    case TDSRRCTMR_BLOCK_PS_RESEL_NAS_API_TIMER:
      name_ptr = "BLOCK_PS_RESEL_NAS_API_TMR";
      break;

    case TDSRRCTMR_IGNORE_NON_EXIST_MCM_TIMER:
      name_ptr = "IGNORE_NON_EXIST_MCM_TMR";
      break;			
    /* Set string name for td2td manual bplmn search guard timer */
    case TDSRRCTMR_TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER:
      name_ptr = "TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER";
      break;

   case TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER:
      name_ptr = "ABORT_RESEL_REQ_TO_GRR_TMR";
      break;
    case TDSRRCTMR_SIB_STOP_PARTITION_TIMER:
      name_ptr = "SIB_STOP_PARTITION_TMR";
      break;

    case TDSRRCTMR_SIB_MASK_INVERT_TIMER:
      name_ptr = "SIB_MASK_INVERT_TMR";
      break;

    case TDSRRCTMR_SIB_EMPTY_MASK_TIMER:
      name_ptr = "SIB_EMPTY_MASK_TMR";
      break;

    case TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER:
      name_ptr = "SIB_ALL_ONE_MASK_TMR";
      break;
      
    case TDSRRCTMR_RCE_ALLOW_TA_TIMER:
      name_ptr = "RCE_ALLOW_TA_TMR";
      break;
	case TDSRRCTMR_QTA_GUARD_TIMER: 
	  name_ptr = "QTA_GUARD_TIMER";
	  break;
    default:
      name_ptr = "unknown";
      break;
  }
  /* Fix the KW error */
  if(strlen(name_ptr) > TDSRRCTMR_TIMER_NAME_LEN_MAX)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"The length of timer (%d) name exceeds the max size and sets to default name",timer);
    name_ptr = "unknown";
  }
  memscpy((void*)timer_name_ptr, strlen(name_ptr), (void*)name_ptr, strlen(name_ptr));

  return;
}

/*===========================================================================

FUNCTION tdsrrc_def_non_defer_timer

DESCRIPTION
  Define non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_def_non_defer_timer(timer_type *p_timer, tdsrrc_non_def_timer_cb_ptr timer_cb_ptr, unsigned long param)
{
   timer_group_set_deferrable( &tdsrrc_timer_group_non_deferrable, FALSE );
   timer_def(p_timer, &tdsrrc_timer_group_non_deferrable, NULL, 0, timer_cb_ptr, param);   
}
/*===========================================================================

FUNCTION tdsrrc_set_non_defer_timer

DESCRIPTION
  Set non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_set_non_defer_timer
(
   timer_type     *p_timer,  /* pointer to timer to set  */
   timetick_type  cnt       /* value to set the timer   */
)
{
   timer_set(p_timer, cnt, 0, T_MSEC);
}
/*===========================================================================

FUNCTION tdsrrc_clr_non_defer_timer

DESCRIPTION
  Clear non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_clr_non_defer_timer(timer_type *p_timer)
{
   timer_clr(p_timer, T_MSEC);
}
/*===========================================================================

FUNCTION tdsrrc_get_non_defer_timer

DESCRIPTION
  Get remaining time of non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
timetick_type tdsrrc_get_non_defer_timer
(
  timer_type *p_timer       /* pointer to the timer to get */
)
{
  return timer_get(p_timer, T_MSEC);
}
/*===========================================================================

FUNCTION tdsrrctmr_timers_callback

DESCRIPTION
  This function will be called when ever RRC timers expire.

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
 /***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrctmr_timers_callback
(
   unsigned long param                   /* Parameter to Identify timer */                               
)
{
  tdsrrc_cmd_type *cmd_ptr;                     /* Pointer to RRC Command */
  tdsrrclog_rrc_timer_expired_event_type rrc_timer_expired_event;  
  char timer_name[TDSRRCTMR_TIMER_NAME_LEN_MAX];

  memset((void *)timer_name, '\0', sizeof(timer_name));

  if(param == TDSRRCTMR_WRM_LOCK_TIMER)
  {
    /*Set TRM Signal, so that RRC is unblocked*/
    (void) rex_set_sigs( &tds_rrc_tcb, TDSRRC_WRM_SIG);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Expired timer:%d",param);
    tdscfa_dsds_add_event(TDSRRC_DSDS_WRM_SIG);
    return ;
  }

  if (param == TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER)
  {
    /* This timer doesn't need to trigger any handler */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Expired timer:%d,TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER",param);
    return;
  }
  
  if (param == TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER)
  {
    /* This timer doesn't need to trigger any handler */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Expired timer:%d,SIB7_UPDATE_WAIT_TIMER",param);
    return;
  }
  
  if (param == TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER)
  {
    /* This timer doesn't need to trigger any handler */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Expired timer:%d,INTER_SRNS_IGNORE_MR_TIMER",param);
    return;
  }

  if (param == TDSRRCTMR_GET_TA_DURATION_TIMER)
  {
    /* This timer doesn't need to trigger any handler */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Expired timer:%d,GET_TA_DURATION_TIMER",param);
    return;
  }	

  if( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {   
    switch(param)
    { 
      case TDSRRCTMR_T_300_TIMER: 
        /* Fill in the command parameters to TDSRRC_T300_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T300_EXPIRED_IND;
        break;

      case TDSRRCTMR_T_308_TIMER: 
        /* Fill in the command parameters to TDSRRC_T308_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T308_EXPIRED_IND;
        break;

      case TDSRRCTMR_T_304_TIMER: 
        /* Fill in the command parameters to TDSRRC_T304_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T304_EXPIRED_IND;
        break;

      case TDSRRCTMR_WAIT_TIME_TIMER:
        /* Fill in the command parameters to TDSRRC_WAIT_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_WAIT_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_PHY_CHAN_FAILURE_TIMER:
        /* Fill in the command parameters to TDSRRC_WAIT_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_PHY_CHAN_FAILURE_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_UE_TO_CAMP_ON_TIMER:
        /* Fill in the command parameters to TDSRRC_TIMER_TO_CAMP_ON_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_TIMER_TO_CAMP_ON_EXPIRED_IND;
        break;

      case TDSRRCTMR_RRC_CON_DELAY_TIMER:
        /* Fill in the command parameters to RRC_SIG_CON_DELAY_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_RRC_CON_DELAY_TIMER_EXPIRED_IND;
        break;
      case TDSRRCTMR_RCE_L2ACK_TIMER:
         /* Fill in the command parameters to RRC_TIMER_FOR_L2ACK_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_TIMER_FOR_RCE_L2ACK_EXPIRED_IND;
        break;

      case TDSRRCTMR_RCR_L2ACK_TIMER:
         /* Fill in the command parameters to RRC_TIMER_FOR_L2ACK_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_TIMER_FOR_RCR_L2ACK_EXPIRED_IND;
        break;

      case TDSRRCTMR_RSSI_BER_TIMER:
         /* Fill in the Command Id to TDSRRC_TIMER_FOR_RSSI_BER_EXPIRED_IND */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_TIMER_FOR_RSSI_BER_EXPIRED_IND;
        break;

      case TDSRRCTMR_SIB_WAIT_TIMER:
         /* Fill in the Command Id to TDSRRC_SIB_WAIT_TIMER_EXPIRED_IND */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIB_WAIT_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_T_302_TIMER: 
        /* Fill in the command parameters to TDSRRC_T302_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T302_EXPIRED_IND;
        break;

      case TDSRRCTMR_T_305_TIMER: 
        /* Fill in the command parameters to TDSRRC_T305_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T305_EXPIRED_IND;
        break;

      case TDSRRCTMR_T_316_TIMER: 
        /* Fill in the command parameters to TDSRRC_T316_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T316_EXPIRED_IND;
        break;

      case TDSRRCTMR_T_317_TIMER: 
        /* Fill in the command parameters to TDSRRC_T317_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T317_EXPIRED_IND;
        break;

      case TDSRRCTMR_T_307_TIMER: 
        /* Fill in the command parameters to TDSRRC_T307_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T307_EXPIRED_IND;
        break;

      case TDSRRCTMR_T_314_TIMER: 
        /* Fill in the command parameters to TDSRRC_T314_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T314_EXPIRED_IND;

        //EVENT: Event_Timer_Expired
        rrc_timer_expired_event.rrc_timer_id = (uint8) TDSRRCLOG_T_314_TIMER;
        event_report_payload(EVENT_TDSCDMA_RRC_TIMER_EXPIRED, sizeof(rrc_timer_expired_event),
                       (void *)&rrc_timer_expired_event);
		TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
			                       TDS_SELF_HOSTING_MAJOR_CALL_RETENTION_EXCEPTION,
			                       TDS_SELF_HOSTING_MINOR_RRC_T314_EXPIRED);

        break;

      case TDSRRCTMR_T_315_TIMER: 
        /* Fill in the command parameters to TDSRRC_T315_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T315_EXPIRED_IND;

        //EVENT: Event_Timer_Expired
        rrc_timer_expired_event.rrc_timer_id = (uint8) TDSRRCLOG_T_315_TIMER;
        event_report_payload(EVENT_TDSCDMA_RRC_TIMER_EXPIRED, sizeof(rrc_timer_expired_event),
                       (void *)&rrc_timer_expired_event);
		TDS_SELF_HOSTING_EVENT_LOG(TDS_SELF_HOSTING_OWNER_RRC,
			                       TDS_SELF_HOSTING_MAJOR_CALL_RETENTION_EXCEPTION,
			                       TDS_SELF_HOSTING_MINOR_RRC_T315_EXPIRED);

        break;

      case TDSRRCTMR_SIB7_EXP_TIMER: 
        /* Fill in the command parameters to TDSRRC_SIB7_EXP_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIB7_EXP_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_IGNORE_BCCH_MOD_TIMER: 
        /* Fill in the command parameters to TDSRRC_IGNORE_BCCH_MOD_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_IGNORE_BCCH_MOD_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_DCH_FACH_CELL_SELECTION_TIMER:
        /* Fill in the command parameters to TDSRRC_DCH_FACH_CELL_SELECTION_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_DCH_FACH_CELL_SELECTION_TIMER_EXPIRED_IND;
        break;

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      case TDSRRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER:
        /* Fill in the command parameters to TDSRRC_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND;
        break;
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */

      case TDSRRCTMR_DCH_PCH_CELL_SELECTION_TIMER:
        /* Fill in the command parameters to TDSRRC_DCH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_DCH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_L1_DEADLOCK_DETECT_TIMER:
        /* Fill in the command parameters to TDSRRC_DCH_FACH_CELL_SELECTION_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRCTMR_L1_DEADLOCK_DETECT_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_DELAY_ERR_FATAL_TIMER:
        /* Fill in the command parameters to TDSRRCTMR_DELAY_ERR_FATAL_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRCTMR_DELAY_ERR_FATAL_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_T_3174_TIMER: 
        /* Fill in the command parameters to TDSRRC_T3174_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T3174_EXPIRED_IND;
        break;

      case TDSRRCTMR_FREQ_SCAN_TIMER: 
        /* Fill in the command parameters to RRC_FREQ_SCAN_TIMER__EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_FREQ_SCAN_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_T_OOS_TIMER: 
        /* Fill in the command parameters to RRC_FREQ_SCAN_TIMER__EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_OOS_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER: 
          /* Fill in the command parameters to TDSRRC_DEEP_SLEEP_NO_SVC_TIMER_EXPIRED_IND Command */
          cmd_ptr->cmd_hdr.cmd_id = TDSRRC_DEEP_SLEEP_NO_SVC_TIMER_EXPIRED_IND;
          break;

      case TDSRRCTMR_BPLMN_SRCH_TIMER:
        /* Fill in the command parameters to TDSRRC_BPLMN_SRCH_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_BPLMN_SRCH_TIMER_EXPIRED_IND;

        //EVENT: Event_BPLMN_Timer_Expired
        rrc_timer_expired_event.rrc_timer_id = (uint8) TDSRRCLOG_BPLMN_SRCH_TIMER;
        event_report_payload(EVENT_TDSCDMA_RRC_TIMER_EXPIRED, sizeof(rrc_timer_expired_event),
                       (void *)&rrc_timer_expired_event);

        break;
        
      case TDSRRCTMR_BPLMN_FREQ_SCAN_TIMER:
        /* Fill in the command parameters to TDSRRC_BPLMN_FREQ_SCAN_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_BPLMN_FREQ_SCAN_TIMER_EXPIRED_IND;
        break;


      case TDSRRCTMR_FREQ_REDIRECT_WAIT_TIMER:
        /* Fill in the command parameters to RRC_FREQ_REDIRECTION_WAIT_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_FREQ_REDIRECT_WAIT_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_CONN_MODE_OOS_TIMER:
        /* Fill in the command parameters to TDSRRC_CONN_MODE_OOS_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CONN_MODE_OOS_TIMER_EXPIRED_IND;
        break;


      case RRCTMR_TD2TD_BPLMN_DRX_SRCH_TIMER:
        /* Fill in the command parameters to RRC_WTOW_BPLMN_SRCH_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_TD2TD_BPLMN_DRX_SRCH_TIMER_EXPIRED_IND;
        break;
  
      case TDSRRCTMR_BPLMN_GUARD_SRCH_TIMER:
        /* Fill in the command parameters to RRC_WTOW_BPLMN_GUARD_SRCH_TIMER Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_BPLMN_GUARD_SRCH_TIMER_EXPIRED_IND;
        break;

      case  TDSRRCTMR_SIXHR_SIB_UPDATE_TIMER:
        /* Fill in the command parameters to TDSRRC_SIX_HRS_SIB_TIMER_UPDATE_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIX_HRS_SIB_TIMER_UPDATE_IND;
        break;

      case TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER:
        /* Fill in the command parameters to TDSRRC_CONN_MODE_OOS_INITIAL_NO_DS_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CONN_MODE_OOS_INITIAL_NO_DS_TIMER_EXPIRED_IND;
        break;
      case TDSRRCTMR_WMR_UNLCOK_BY_TIMER:
        /* Fill in the command parameters to RRCTMR_EOOS_FULL_SCAN_TIMER Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_DUAL_SIM_WRM_UNLOCK_BY_TIMER_EXP_IND;
        break;

      case TDSRRCTMR_CONN_MODE_OOS_DS_TIMER:
        /* Fill in the command parameters to TDSRRC_CONN_MODE_OOS_DS_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CONN_MODE_OOS_DS_TIMER_EXPIRED_IND;
        break;      

      case  TDSRRC_SIB_CHANGE_GUARD_TIMER:
        /* Fill in the command parameters to TDSRRC_SIB_CHANGE_GUARD_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIB_CHANGE_GUARD_TIMER_EXPIRED_IND;
        break;

      case TDSRRC_BCCH_MODIFICATION_TIMER:
        /* Fill in the command parameters to TDSRRC_BCCH_MODIFICATION_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_BCCH_MODIFICATION_TIMER_EXPIRED_IND;
        break;


      case RRCTMR_G2TD_REDIRECTION_WAIT_TIMER:
        /* Fill in the command parameters to RRC_G2TD_REDIRECTION_WAIT_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_G2TD_REDIRECTION_WAIT_TIMER_EXPIRED_IND;
        break;
	  case RRCTMR_G2TD_REDIRECTION_LFS_TIMER:
	    cmd_ptr->cmd_hdr.cmd_id = RRC_G2TD_REDIRECTION_LFS_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_TD2G_ABORT_GUARD_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_TD2G_ABORT_GUARD_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_T_319_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T319_EXPIRED_IND;
        break;


      case TDSRRCTMR_T_320_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T320_EXPIRED_IND;
        break;	

#ifdef FEATURE_TDSCDMA_REL7_SPECRS
      case TDSRRCTMR_COUEC_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_TIMER_COUEC_EXPIRED_IND;
        break;
#endif /*FEATURE_TDSCDMA_REL7_SPECRS*/
      case TDSRRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_BPLMN_SEARCH_RESUME_IND;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_BPLMN_SEARCH_RESUME_IND sent to CSP");
        break;

#if defined(FEATURE_3GPP_FAST_DORMANCY)
      case TDSRRCTMR_T_323_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_T323_EXPIRED_IND;
        break;
#endif
#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
      case TDSRRCTMR_DED_PRI_VALIDITY_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_DED_PRI_VALDITY_TIMER_EXPIRED_IND;
        event_report(EVENT_TDSCDMA_DED_PRIORITIES_VALIDITY_TIMER_EXPIRED);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_DED_PRI_VALDITY_TIMER_EXPIRED_IND");
        break;
#endif
      case TDSRRCTMR_WRM_DELAY_PREEMPTION_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_WRM_PREEMPT_IND;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: TDSRRC_WRM_PREEMPT_IND posted to CSP");
        break;
#ifdef FEATURE_LTE_TO_TDSCDMA
      case TDSRRCTMR_LTE_TO_TDSCDMA_REDIR_SRCH_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LTE_TO_TDSCDMA_SRCH_TIMER_EXPIRY_IND;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_LTE_TO_TDSCDMA_SRCH_TIMER_EXPIRY_IND posted to CSP");
        break;
#endif
      // CR 352866 begin
      case TDSRRCTMR_RCE_WAIT_RESELCT_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_TIMER_TO_RCE_WAIT_RESELCT_EXPIRED_IND;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_TIMER_TO_RCE_WAIT_RESELCT_EXPIRED_IND posted to RCE");
        break;
      // CR 352866 end
      // CR430034 RAU Reject workaround
      case TDSRRCTMR_DELAY_OOS_RLF_RPT_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_DELAY_OOS_RLF_RPT_TIMER_EXPIRED_IND;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_DELAY_OOS_RLF_RPT_TIMER_EXPIRED_IND posted to DISPATCHER. CR430034");
        break;

      case TDSRRCTMR_SOFT_ABORT_GUARD_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SOFT_ABORT_GUARD_TIMER_EXPIRED_IND;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_SOFT_ABORT_GUARD_TIMER_EXPIRED_IND posted to TDSRRCDT");
        break;
      case TDSRRCTMR_HOLD_PS_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_HOLD_PS_TIMER_EXPIRED_IND;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Hold PS: TDSRRC_HOLD_PS_TIMER_EXPIRED_IND posted to DISPATCHER.");
        break;
      case TDSRRCTMR_BLOCK_CS_RESEL_NAS_API_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_BLOCK_CS_RESEL_NAS_API_TIMER_EXPIRED_IND;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_BLOCK_CS_RESEL_NAS_API_TIMER_EXPIRED_IND posted to CSP");
        break;
      case TDSRRCTMR_BLOCK_PS_RESEL_NAS_API_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_BLOCK_PS_RESEL_NAS_API_TIMER_EXPIRED_IND;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_BLOCK_PS_RESEL_NAS_API_TIMER_EXPIRED_IND posted to CSP");
        break;

      case TDSRRCTMR_DA_BACKOFF_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_DA_BACKOFF_TIMER_EXPIRED_IND;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_DA_BACKOFF_TIMER_EXPIRED_IND posted to MISC");
        break;

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
      case TDSRRCTMR_BPLMN_PARTIAL_REPORT_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_BPLMN_PARTIAL_REPORT_TIMER_EXPIRED_IND;
        break;
#endif

      case TDSRRCTMR_IGNORE_NON_EXIST_MCM_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRCTMR_IGNORE_NON_EXIST_MCM_TIMER_EXPIRED_IND;
        break;

      case TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER_EXPIRED_IND;
        break;

      /* Send timeout event for td2td manual bplmn search guard timer */
      case TDSRRCTMR_TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER_EXPIRED_IND;
        break;
      case TDSRRCTMR_SIB_STOP_PARTITION_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIB_STOP_PARTITION_TMR_EXPIRED_IND;
        break;

      case TDSRRCTMR_SIB_MASK_INVERT_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIB_MASK_INVERT_TMR_EXPIRED_IND;
        break;

      case TDSRRCTMR_SIB_EMPTY_MASK_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIB_EMPTY_MASK_TMR_EXPIRED_IND;
        break;

      case TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = TDSRRCTMR_SIB_ALL_ONE_MASK_TMR_EXPIRED_IND;
        break;
  
     case TDSRRCTMR_RCE_ALLOW_TA_TIMER:
      cmd_ptr->cmd_hdr.cmd_id = TDSRRC_RCE_TA_ALLOW_EXPIRED_IND;
      break;
     case TDSRRCTMR_QTA_GUARD_TIMER:
	  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_QTA_GUARD_TIMER_EXPIRED_IND;
	  break;
      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR Unknown RRC timer received!");
        break;
    }

    tdsrrctmr_get_timer_name(param, timer_name);
  
    TDSRRC_MSG_SPRINTF_2(MSG_LEGACY_HIGH, "Expired timer:%d (%s)",param,timer_name);
    tdsrrc_put_int_cmd(cmd_ptr);         
  }
  else      /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Unable to allocate memory for Internal Command",0,0,0);
  } 
} /* tdsrrctmr_timers_callback */

/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================

FUNCTION tdsrrctmr_create_timers

DESCRIPTION
  This function will create all RRC  related timers.
DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
 /***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrctmr_create_timers
(
  void
)
 
{
  /* Create Timer T300 */
  tdsrrc_t_300_ptr = &tdsrrc_t_300;
  rex_def_timer_ex( tdsrrc_t_300_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_T_300_TIMER );

  /* Create Timer T308 */
  tdsrrc_t_308_ptr = &tdsrrc_t_308;
  rex_def_timer_ex( tdsrrc_t_308_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_T_308_TIMER );

  /* Create Timer T304 */
  tdsrrc_t_304_ptr = &tdsrrc_t_304;
  rex_def_timer_ex( tdsrrc_t_304_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_T_304_TIMER );

  /* Create Timer for Wait time */
  tdsrrc_t_wait_time_ptr = &tdsrrc_t_wait_time;
  rex_def_timer_ex( tdsrrc_t_wait_time_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_WAIT_TIME_TIMER );

  /* Create Timer for UE to camp on */ 
  tdsrrc_t_ue_to_camp_on_ptr = &tdsrrc_t_ue_to_camp_on;
  rex_def_timer_ex( tdsrrc_t_ue_to_camp_on_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_UE_TO_CAMP_ON_TIMER );

  /* Create Timer for UE to camp on */ 
  tdsrrc_t_rrc_con_delay_ptr = &tdsrrc_t_rrc_con_delay;
  rex_def_timer_ex( tdsrrc_t_rrc_con_delay_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32) TDSRRCTMR_RRC_CON_DELAY_TIMER );
  /* Create RCE timer for L2 ACK */
  tdsrrc_t_rce_l2ack_ptr = &tdsrrc_t_rce_l2ack;
  rex_def_timer_ex( tdsrrc_t_rce_l2ack_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_RCE_L2ACK_TIMER );

  /* Create RCR timer for L2 ACK */
  tdsrrc_t_rcr_l2ack_ptr = &tdsrrc_t_rcr_l2ack;
  rex_def_timer_ex( tdsrrc_t_rcr_l2ack_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_RCR_L2ACK_TIMER );

  /* Create reporting RSSI_BER timer */
  tdsrrc_t_rssi_ber_ptr = &tdsrrc_t_rssi_ber;
  rex_def_timer_ex( tdsrrc_t_rssi_ber_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_RSSI_BER_TIMER );

  /* Create reporting SIB wait timer */
  tdsrrc_t_sib_wait_ptr = &tdsrrc_t_sib_wait;
  rex_def_timer_ex( tdsrrc_t_sib_wait_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_SIB_WAIT_TIMER );

  /* Create Timer T302 */
  tdsrrc_t_302_ptr = &tdsrrc_t_302;
  rex_def_timer_ex( tdsrrc_t_302_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_T_302_TIMER );

  /* Create Timer T305 */
  tdsrrc_t_305_ptr = &tdsrrc_t_305;
  rex_def_timer_ex( tdsrrc_t_305_ptr,
                    tdsrrctmr_timers_callback,
                  (uint32)  TDSRRCTMR_T_305_TIMER );

  /* Create Timer T316 */
  tdsrrc_t_316_ptr = &tdsrrc_t_316;
  rex_def_timer_ex( tdsrrc_t_316_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_T_316_TIMER );

  /* Create Timer T317 */
  tdsrrc_t_317_ptr = &tdsrrc_t_317;
  rex_def_timer_ex( tdsrrc_t_317_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_T_317_TIMER );

  /* Create Timer T307 */
  tdsrrc_t_307_ptr = &tdsrrc_t_307;
  rex_def_timer_ex( tdsrrc_t_307_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_T_307_TIMER );

  /* Create Timer T314 */
  tdsrrc_t_314_ptr = &tdsrrc_t_314;
  rex_def_timer_ex( tdsrrc_t_314_ptr,
                    tdsrrctmr_timers_callback,
                  (uint32)  TDSRRCTMR_T_314_TIMER );

  /* Create Timer T315 */
  tdsrrc_t_315_ptr = &tdsrrc_t_315;
  rex_def_timer_ex( tdsrrc_t_315_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_T_315_TIMER );

  /* Create Timer for SIB7 Expiration */
  tdsrrc_t_sib7_exp_ptr = &tdsrrc_t_sib7_exp;
  rex_def_timer_ex( tdsrrc_t_sib7_exp_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_SIB7_EXP_TIMER );

  /* Create Timer to Ignore BCCH Modification Info */
  tdsrrc_t_ignore_bcch_mod_exp_ptr = &tdsrrc_t_ignore_bcch_mod_exp;
  rex_def_timer_ex( tdsrrc_t_ignore_bcch_mod_exp_ptr,
                    tdsrrctmr_timers_callback,
                  (uint32)  TDSRRCTMR_IGNORE_BCCH_MOD_TIMER );

  /* Create Timer during Cell Selection, going DCH->FACH */
  tdsrrc_t_dch_fach_cell_selection_exp_ptr = &tdsrrc_t_dch_fach_cell_selection_exp;
  rex_def_timer_ex( tdsrrc_t_dch_fach_cell_selection_exp_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_DCH_FACH_CELL_SELECTION_TIMER );

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* Create Timer during Cell Selection, going FACH->FACH/PCH */
  tdsrrc_t_fach_to_fach_pch_cell_selection_exp_ptr = &tdsrrc_t_fach_to_fach_pch_cell_selection_exp;
  rex_def_timer_ex( tdsrrc_t_fach_to_fach_pch_cell_selection_exp_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER );
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */

  /* Create Timer to detect L1 deadlock */
  tdsrrc_t_l1_deadlock_detection_exp_ptr = &tdsrrc_t_l1_deadlock_detection_exp;
  rex_def_timer_ex( tdsrrc_t_l1_deadlock_detection_exp_ptr,
                    tdsrrctmr_timers_callback,
                  (uint32)  TDSRRCTMR_L1_DEADLOCK_DETECT_TIMER );

  /* Create Timer to delay an ERR_FATAL */
  tdsrrc_t_delay_err_fatal_exp_ptr = &tdsrrc_t_delay_err_fatal_exp;
  rex_def_timer_ex( tdsrrc_t_delay_err_fatal_exp_ptr,
                    tdsrrctmr_timers_callback,
                   (uint32) TDSRRCTMR_DELAY_ERR_FATAL_TIMER );

  /* Create Timer during Cell Selection, going DCH->PCH */
  tdsrrc_t_dch_pch_cell_selection_exp_ptr = &tdsrrc_t_dch_pch_cell_selection_exp;
  rex_def_timer_ex( tdsrrc_t_dch_pch_cell_selection_exp_ptr,
                    tdsrrctmr_timers_callback,
                 (uint32)   TDSRRCTMR_DCH_PCH_CELL_SELECTION_TIMER );

  /* Create Timer for Wait time */
  tdsrrc_t_phy_chan_failure_ptr = &tdsrrc_t_phy_chan_failure;
  rex_def_timer_ex( tdsrrc_t_phy_chan_failure_ptr,
                    tdsrrctmr_timers_callback,
                 (uint32)   TDSRRCTMR_PHY_CHAN_FAILURE_TIMER );

  /* Create Timer T3174 */
  tdsrrc_t_3174_ptr = &tdsrrc_t_3174;
  rex_def_timer_ex( tdsrrc_t_3174_ptr,
                    tdsrrctmr_timers_callback,
                  (uint32)  TDSRRCTMR_T_3174_TIMER );

  /* Create Timer for frequency scan */
  tdsrrc_t_freq_scan_exp_ptr = &tdsrrc_t_freq_scan_exp;
  rex_def_timer_ex( tdsrrc_t_freq_scan_exp_ptr,
                    tdsrrctmr_timers_callback,
                 (uint32)   TDSRRCTMR_FREQ_SCAN_TIMER );

  /* Create Timer for frequency scan */
  tdsrrc_t_oos_ptr = &tdsrrc_t_oos;
  rex_def_timer_ex( tdsrrc_t_oos_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_T_OOS_TIMER );

  /* Create Timer for returning no service if consecutive service requests
   * are received in deep sleep before this timer expires.
   */
  tdsrrc_t_deep_sleep_no_svc_ptr = &tdsrrc_t_deep_sleep_no_svc;
  rex_def_timer_ex( tdsrrc_t_deep_sleep_no_svc_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER );

   /* Create Timer for BPLMN full freq scan */
  tdsrrc_t_bplmn_srch_ptr = &tdsrrc_t_bplmn_srch;
  rex_def_timer_ex( tdsrrc_t_bplmn_srch_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_BPLMN_SRCH_TIMER );

  /* Create Timer for BPLMN full frequency scan */
  tdsrrc_t_bplmn_full_scan_srch_ptr = &tdsrrc_t_bplmn_full_scan_srch;
  rex_def_timer_ex( tdsrrc_t_bplmn_full_scan_srch_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_BPLMN_FREQ_SCAN_TIMER );


  /* Create Timer for Freq Redirect Wait Timer */
  tdsrrc_t_freq_redirect_wait_timer_ptr = &tdsrrc_t_freq_redirect_wait_timer;
  rex_def_timer_ex( tdsrrc_t_freq_redirect_wait_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_FREQ_REDIRECT_WAIT_TIMER );

  /* Create Timer for OOS in Connected Mode handling */
  tdsrrc_t_conn_mode_oos_timer_ptr = &tdsrrc_t_conn_mode_oos_timer;
  rex_def_timer_ex( tdsrrc_t_conn_mode_oos_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_CONN_MODE_OOS_TIMER );



  /* Create Timer for WTOW BPLMN drx search */
  tdsrrc_t_tdstotds_bplmn_srch_ptr = &tdsrrc_t_tdstotds_bplmn_srch;
  rex_def_timer_ex( tdsrrc_t_tdstotds_bplmn_srch_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)RRCTMR_TD2TD_BPLMN_DRX_SRCH_TIMER );

  /* Create Timer for TDSTOTDS/TDSTOG MAX BPLMN allowed search */
  tdsrrc_t_tdstotds_bplmn_guard_srch_ptr = &tdsrrc_t_tdstotds_bplmn_guard_srch;
  rex_def_timer_ex( tdsrrc_t_tdstotds_bplmn_guard_srch_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_BPLMN_GUARD_SRCH_TIMER );

  /* Create Timer for Updating SIX Hours SIB Clean Up Timer*/
  tdsrrc_sixhr_sib_update_timer_ptr = &tdsrrc_sixhr_sib_update_timer;
  rex_def_timer_ex( tdsrrc_sixhr_sib_update_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_SIXHR_SIB_UPDATE_TIMER);


  /* Create Timer for Deep Sleep OOS in Connected Mode handling */
  tdsrrc_t_conn_mode_oos_initial_no_ds_timer_ptr = &tdsrrc_t_conn_mode_oos_initial_no_ds_timer;
  rex_def_timer_ex( tdsrrc_t_conn_mode_oos_initial_no_ds_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER );

  /* Create Timer for Deep Sleep OOS in Connected Mode handling */
  tdsrrc_t_conn_mode_oos_ds_timer_ptr = &tdsrrc_t_conn_mode_oos_ds_timer;
  rex_def_timer_ex( tdsrrc_t_conn_mode_oos_ds_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_CONN_MODE_OOS_DS_TIMER );


  /* Create Timer for SIB CHANGE GUARD Timer*/
  tdsrrc_sib_change_guard_timer_ptr = &tdsrrc_sib_change_guard_timer;
  rex_def_timer_ex( tdsrrc_sib_change_guard_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRC_SIB_CHANGE_GUARD_TIMER);

  /* Create Timer for BCCH Modification Time*/
  tdsrrc_bcch_mod_timer_ptr = &tdsrrc_bcch_mod_timer;
  rex_def_timer_ex( tdsrrc_bcch_mod_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRC_BCCH_MODIFICATION_TIMER);


  /* Create Timer for GtoW Redirection handling */
  tdsrrc_t_gtow_redirection_timer_ptr = &tdsrrc_t_gtow_redirection_timer;
  rex_def_timer_ex( tdsrrc_t_gtow_redirection_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)RRCTMR_G2TD_REDIRECTION_WAIT_TIMER );
  tdsrrc_t_gtow_redir_lfs_timer_ptr = &tdsrrc_t_gtow_redir_lfs_timer;
  rex_def_timer_ex( tdsrrc_t_gtow_redir_lfs_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)RRCTMR_G2TD_REDIRECTION_LFS_TIMER );

  tdsrrc_t_wtog_abort_guard_timer_ptr = &tdsrrc_t_wtog_abort_guard_timer;
  rex_def_timer_ex( tdsrrc_t_wtog_abort_guard_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)RRCTMR_TD2G_ABORT_GUARD_TIMER );


  tdsrrc_t_319_ptr = &tdsrrc_t_319;
  rex_def_timer_ex( tdsrrc_t_319_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_T_319_TIMER);


  tdsrrc_t_320_ptr = &tdsrrc_t_320;
  rex_def_timer_ex( tdsrrc_t_320_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_T_320_TIMER);
  
#ifdef FEATURE_TDSCDMA_REL7_SPECRS
 
  tdsrrc_t_couec_timer_ptr = &tdsrrc_t_couec_timer;
  rex_def_timer_ex( tdsrrc_t_couec_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_COUEC_TIMER);
#endif /*FEATURE_TDSCDMA_REL7_SPECRS*/

  tdsrrc_wrm_lock_timer_ptr = &tdsrrc_wrm_lock_timer;
  rex_def_timer_ex( tdsrrc_wrm_lock_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_WRM_LOCK_TIMER);
  tdsrrc_wrm_delay_preemption_timer_ptr = &tdsrrc_wrm_delay_preemption_timer;
  rex_def_timer_ex( tdsrrc_wrm_delay_preemption_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_WRM_DELAY_PREEMPTION_TIMER);


  tdsrrc_dsim_bplmn_lock_rel_off_timer_ptr = &tdsrrc_dsim_bplmn_lock_rel_off_timer;
  tdsrrc_def_non_defer_timer(tdsrrc_dsim_bplmn_lock_rel_off_timer_ptr,
                    tdsrrctmr_timers_callback, 
                    (uint32)TDSRRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER);
  tdsrrc_dsim_wrm_unlock_by_timer_ptr = & tdsrrc_dsim_wrm_unlock_by_timer;
  rex_def_timer_ex(tdsrrc_dsim_wrm_unlock_by_timer_ptr,
                          tdsrrctmr_timers_callback, 
                          (uint32)TDSRRCTMR_WMR_UNLCOK_BY_TIMER);

  tdsrrc_get_ta_duration_timer_ptr = &tdsrrc_get_ta_duration_timer;
  rex_def_timer_ex( tdsrrc_get_ta_duration_timer_ptr,
                    tdsrrctmr_timers_callback, 
                    (uint32)TDSRRCTMR_GET_TA_DURATION_TIMER);
#if defined(FEATURE_3GPP_FAST_DORMANCY)
  tdsrrc_t_323_ptr = &tdsrrc_t_323;
  rex_def_timer_ex( tdsrrc_t_323_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_T_323_TIMER);
#endif

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
  tdsrrc_t_ded_pri_validity_timer_ptr = &tdsrrc_t_ded_pri_validity_timer;
  rex_def_timer_ex(tdsrrc_t_ded_pri_validity_timer_ptr,
                   tdsrrctmr_timers_callback,
                   (uint32)TDSRRCTMR_DED_PRI_VALIDITY_TIMER);
#endif
#ifdef FEATURE_LTE_TO_TDSCDMA
  tdsrrc_t_lte_to_wcdma_redir_search_timer_ptr = &tdsrrc_t_lte_to_wcdma_redir_search_timer;
  rex_def_timer_ex(tdsrrc_t_lte_to_wcdma_redir_search_timer_ptr,
                   tdsrrctmr_timers_callback,
                   (uint32)TDSRRCTMR_LTE_TO_TDSCDMA_REDIR_SRCH_TIMER);
#endif
  // CR 352866 begin
  tdsrrc_t_rce_wait_reselect_ptr = &tdsrrc_t_rce_wait_reselect;
  rex_def_timer_ex( tdsrrc_t_rce_wait_reselect_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_RCE_WAIT_RESELCT_TIMER);
  // CR 352866 end
  // CR430034 RAU Reject workaround
  tdsrrc_t_delay_oos_rlf_rpt_timer_ptr = &tdsrrc_t_delay_oos_rlf_rpt_timer;
  rex_def_timer_ex( tdsrrc_t_delay_oos_rlf_rpt_timer_ptr,
	                tdsrrctmr_timers_callback,
				    (uint32)TDSRRCTMR_DELAY_OOS_RLF_RPT_TIMER);

  tdsrrc_t_sib_schdule_update_wait_timer_ptr = &tdsrrc_t_sib_schdule_update_wait_timer;
  rex_def_timer_ex( tdsrrc_t_sib_schdule_update_wait_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER);

  tdsrrc_t_sib7_update_wait_timer_ptr = &tdsrrc_t_sib7_update_wait_timer;
  rex_def_timer_ex( tdsrrc_t_sib7_update_wait_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER);

  tdsrrc_t_inter_srns_ignore_mr_timer_ptr = &tdsrrc_t_inter_srns_ignore_mr_timer;
  rex_def_timer_ex( tdsrrc_t_inter_srns_ignore_mr_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER);

  tdsrrc_t_soft_abort_timer_ptr = &tdsrrc_t_soft_abort_timer;
  rex_def_timer_ex( tdsrrc_t_soft_abort_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_SOFT_ABORT_GUARD_TIMER);
  tdsrrc_t_hold_ps_timer_ptr = &tdsrrc_t_hold_ps_timer;
  rex_def_timer_ex( tdsrrc_t_hold_ps_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_HOLD_PS_TIMER);
  tdsrrc_block_cs_resel_nas_api_timer_ptr = &tdsrrc_block_cs_resel_nas_api_timer;
  rex_def_timer_ex( tdsrrc_block_cs_resel_nas_api_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_BLOCK_CS_RESEL_NAS_API_TIMER);

  tdsrrc_block_ps_resel_nas_api_timer_ptr = &tdsrrc_block_ps_resel_nas_api_timer;
  rex_def_timer_ex( tdsrrc_block_ps_resel_nas_api_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_BLOCK_PS_RESEL_NAS_API_TIMER);


  tdsrrc_da_backoff_timer_ptr = &tdsrrc_da_backoff_timer;
  rex_def_timer_ex( tdsrrc_da_backoff_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_DA_BACKOFF_TIMER );

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
  /* Create Timer for BPLMN Reporting*/
  tdsrrc_t_bplmn_partial_report_timer_ptr = &tdsrrc_t_bplmn_partial_report_timer;
  rex_def_timer_ex( tdsrrc_t_bplmn_partial_report_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_BPLMN_PARTIAL_REPORT_TIMER );
#endif

  tdsrrc_ignore_non_exist_mcm_timer_ptr = &tdsrrc_ignore_non_exist_mcm_timer;
  rex_def_timer_ex( tdsrrc_ignore_non_exist_mcm_timer_ptr,
									  tdsrrctmr_timers_callback,
									  (uint32)TDSRRCTMR_IGNORE_NON_EXIST_MCM_TIMER );
  /* Create Timer for td2td manual bplmn allowed search */
  tdsrrc_td2td_manual_bplmn_guard_srch_timer_ptr = &tdsrrc_td2td_manual_bplmn_guard_srch_timer;
  rex_def_timer_ex( tdsrrc_td2td_manual_bplmn_guard_srch_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER );

    tdsrrc_abort_resel_to_grr_timer_ptr = &tdsrrc_abort_resel_to_grr_timer;
    rex_def_timer_ex( tdsrrc_abort_resel_to_grr_timer_ptr,
                                                                tdsrrctmr_timers_callback,
                                                                 (uint32)TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER);
  tdsrrc_t_sib_stop_part_ptr = &tdsrrc_t_sib_stop_part;
  rex_def_timer_ex( tdsrrc_t_sib_stop_part_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_SIB_STOP_PARTITION_TIMER);

  tdsrrc_t_sib_mask_ptr = &tdsrrc_t_sib_mask;
  rex_def_timer_ex( tdsrrc_t_sib_mask_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_SIB_MASK_INVERT_TIMER);

  tdsrrc_t_sib_empty_mask_ptr = &tdsrrc_t_sib_empty_mask;
  rex_def_timer_ex( tdsrrc_t_sib_empty_mask_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_SIB_EMPTY_MASK_TIMER);

  tdsrrc_t_sib_all_one_mask_ptr = &tdsrrc_t_sib_all_one_mask;
  rex_def_timer_ex( tdsrrc_t_sib_all_one_mask_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER);

  tdsrrc_t_rce_allow_ta_timer_ptr = &tdsrrc_t_rce_allow_ta_timer;
  rex_def_timer_ex( tdsrrc_t_rce_allow_ta_timer_ptr,
                    tdsrrctmr_timers_callback,
                    (uint32)TDSRRCTMR_RCE_ALLOW_TA_TIMER);
  tdsrrc_t_qta_guard_timer_ptr = &tdsrrc_t_qta_guard_timer;
  rex_def_timer_ex( tdsrrc_t_qta_guard_timer_ptr,
					  tdsrrctmr_timers_callback,
					  (uint32)TDSRRCTMR_QTA_GUARD_TIMER);
} /* tdsrrctmr_create_timers */

/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*===========================================================================

FUNCTION tdsrrctmr_start_timer

DESCRIPTION
  This function will set the timer value in milli seconds and starts the timer
  timers.
DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrctmr_start_timer
(
  tdsrrctmr_timer_e_type timer,                /* Timer to start */
  unsigned long timer_value                 /* Value for timer */                      
)
{

#ifdef FEATURE_TIMER_DEBUG
  #error code not present
#endif

  char timer_name[TDSRRCTMR_TIMER_NAME_LEN_MAX];

  memset((void *)timer_name, '\0', sizeof(timer_name));

  switch(timer)
  {
    case TDSRRCTMR_T_300_TIMER:
      rex_set_timer(tdsrrc_t_300_ptr, timer_value); 
      break;
    case TDSRRCTMR_T_308_TIMER:
      rex_set_timer(tdsrrc_t_308_ptr, timer_value); 
      break;
    case TDSRRCTMR_T_304_TIMER:
      rex_set_timer(tdsrrc_t_304_ptr, timer_value); 
      break;
    case TDSRRCTMR_WAIT_TIME_TIMER:
      rex_set_timer(tdsrrc_t_wait_time_ptr, timer_value);
      break;
    case TDSRRCTMR_UE_TO_CAMP_ON_TIMER:
      rex_set_timer(tdsrrc_t_ue_to_camp_on_ptr, timer_value);
      break;
    case TDSRRCTMR_RRC_CON_DELAY_TIMER:
      rex_set_timer(tdsrrc_t_rrc_con_delay_ptr, timer_value);
      break;
    case TDSRRCTMR_RCE_L2ACK_TIMER:
      rex_set_timer(tdsrrc_t_rce_l2ack_ptr, timer_value);
      break;
    case TDSRRCTMR_RCR_L2ACK_TIMER:
      rex_set_timer(tdsrrc_t_rcr_l2ack_ptr, timer_value);
      break;
    case TDSRRCTMR_RSSI_BER_TIMER:
      rex_set_timer(tdsrrc_t_rssi_ber_ptr, timer_value);
      break;
    case TDSRRCTMR_SIB_WAIT_TIMER:
      rex_set_timer(tdsrrc_t_sib_wait_ptr, timer_value);
      break;
    case TDSRRCTMR_T_302_TIMER:
      rex_set_timer(tdsrrc_t_302_ptr, timer_value); 
      break;
    case TDSRRCTMR_T_305_TIMER:
      rex_set_timer(tdsrrc_t_305_ptr, timer_value); 
      break;
    case TDSRRCTMR_T_316_TIMER:
      rex_set_timer(tdsrrc_t_316_ptr, timer_value); 
      break;
    case TDSRRCTMR_T_317_TIMER:
      rex_set_timer(tdsrrc_t_317_ptr, timer_value); 
      break;
    case TDSRRCTMR_T_307_TIMER:
      rex_set_timer(tdsrrc_t_307_ptr, timer_value); 
      break;
    case TDSRRCTMR_T_314_TIMER:
      rex_set_timer(tdsrrc_t_314_ptr, timer_value); 
      break;
    case TDSRRCTMR_T_315_TIMER:
      rex_set_timer(tdsrrc_t_315_ptr, timer_value); 
      break;
    case TDSRRCTMR_SIB7_EXP_TIMER:
      rex_set_timer(tdsrrc_t_sib7_exp_ptr, timer_value); 
      break;
    case TDSRRCTMR_IGNORE_BCCH_MOD_TIMER:
      rex_set_timer(tdsrrc_t_ignore_bcch_mod_exp_ptr, timer_value); 
      break;
    case TDSRRCTMR_DCH_FACH_CELL_SELECTION_TIMER:
      rex_set_timer(tdsrrc_t_dch_fach_cell_selection_exp_ptr, timer_value); 
      break;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    case TDSRRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER:
      rex_set_timer(tdsrrc_t_fach_to_fach_pch_cell_selection_exp_ptr, timer_value); 
      break;
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */
    case TDSRRCTMR_DCH_PCH_CELL_SELECTION_TIMER:
      rex_set_timer(tdsrrc_t_dch_pch_cell_selection_exp_ptr, timer_value); 
      break;
    case TDSRRCTMR_L1_DEADLOCK_DETECT_TIMER:
      rex_set_timer(tdsrrc_t_l1_deadlock_detection_exp_ptr, timer_value); 
      break;
    case TDSRRCTMR_DELAY_ERR_FATAL_TIMER:
      rex_set_timer(tdsrrc_t_delay_err_fatal_exp_ptr, timer_value); 
      break;
    case TDSRRCTMR_T_3174_TIMER:
      rex_set_timer(tdsrrc_t_3174_ptr, timer_value); 
      break;
    case TDSRRCTMR_FREQ_SCAN_TIMER:
      rex_set_timer(tdsrrc_t_freq_scan_exp_ptr, timer_value); 
      break;
    case TDSRRCTMR_T_OOS_TIMER:
      rex_set_timer(tdsrrc_t_oos_ptr, timer_value); 
      break;

    case TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER:
      rex_set_timer(tdsrrc_t_deep_sleep_no_svc_ptr, timer_value); 
      break;                                   

    case TDSRRCTMR_PHY_CHAN_FAILURE_TIMER:
      rex_set_timer(tdsrrc_t_phy_chan_failure_ptr, timer_value);
      break;

    case TDSRRCTMR_BPLMN_SRCH_TIMER:
      rex_set_timer(tdsrrc_t_bplmn_srch_ptr, timer_value); 
      break;

    case TDSRRCTMR_BPLMN_FREQ_SCAN_TIMER:
      rex_set_timer(tdsrrc_t_bplmn_full_scan_srch_ptr, timer_value); 
      break;       


    case TDSRRCTMR_FREQ_REDIRECT_WAIT_TIMER:
      rex_set_timer(tdsrrc_t_freq_redirect_wait_timer_ptr, timer_value); 
      break; 

    case TDSRRCTMR_CONN_MODE_OOS_TIMER:
      rex_set_timer(tdsrrc_t_conn_mode_oos_timer_ptr, timer_value); 
      break;       


    case RRCTMR_TD2TD_BPLMN_DRX_SRCH_TIMER:
      rex_set_timer(tdsrrc_t_tdstotds_bplmn_srch_ptr, timer_value); 
      break;
  
    case TDSRRCTMR_BPLMN_GUARD_SRCH_TIMER:
      rex_set_timer(tdsrrc_t_tdstotds_bplmn_guard_srch_ptr, timer_value); 
      break;

    case TDSRRCTMR_SIXHR_SIB_UPDATE_TIMER:
      rex_set_timer(tdsrrc_sixhr_sib_update_timer_ptr, timer_value); 
      break;
      


    case TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER:
      rex_set_timer(tdsrrc_t_conn_mode_oos_initial_no_ds_timer_ptr, timer_value); 
      break;       

    case TDSRRCTMR_CONN_MODE_OOS_DS_TIMER:
      rex_set_timer(tdsrrc_t_conn_mode_oos_ds_timer_ptr, timer_value); 
      break;       

    case TDSRRC_SIB_CHANGE_GUARD_TIMER:
      rex_set_timer(tdsrrc_sib_change_guard_timer_ptr, timer_value); 
      break; 

    case TDSRRC_BCCH_MODIFICATION_TIMER:
      rex_set_timer(tdsrrc_bcch_mod_timer_ptr, timer_value); 
      break;



    case RRCTMR_G2TD_REDIRECTION_WAIT_TIMER:
      rex_set_timer(tdsrrc_t_gtow_redirection_timer_ptr, timer_value); 
      break;
	case RRCTMR_G2TD_REDIRECTION_LFS_TIMER:
	  rex_set_timer(tdsrrc_t_gtow_redir_lfs_timer_ptr, timer_value); 
      break;

    case RRCTMR_TD2G_ABORT_GUARD_TIMER:
      rex_set_timer(tdsrrc_t_wtog_abort_guard_timer_ptr, timer_value); 
      break;


    case TDSRRCTMR_T_319_TIMER:
      rex_set_timer(tdsrrc_t_319_ptr, timer_value); 
      break;

    case TDSRRCTMR_T_320_TIMER:
      rex_set_timer(tdsrrc_t_320_ptr, timer_value);
      break;	  

#ifdef FEATURE_TDSCDMA_REL7_SPECRS

    case TDSRRCTMR_COUEC_TIMER:
      rex_set_timer(tdsrrc_t_couec_timer_ptr, timer_value);
      break;
#endif /*FEATURE_TDSCDMA_REL7_SPECRS*/


#if defined(FEATURE_3GPP_FAST_DORMANCY)
    case TDSRRCTMR_T_323_TIMER:
      rex_set_timer(tdsrrc_t_323_ptr, timer_value);
      break;	
#endif

    case TDSRRCTMR_WRM_LOCK_TIMER:
      rex_set_timer(tdsrrc_wrm_lock_timer_ptr, timer_value);
      break;
    case TDSRRCTMR_WRM_DELAY_PREEMPTION_TIMER:
      rex_set_timer(tdsrrc_wrm_delay_preemption_timer_ptr, timer_value);
      break;
    case TDSRRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER:
      tdsrrc_set_non_defer_timer(tdsrrc_dsim_bplmn_lock_rel_off_timer_ptr, timer_value);
      break;

    case TDSRRCTMR_WMR_UNLCOK_BY_TIMER:
      rex_set_timer(tdsrrc_dsim_wrm_unlock_by_timer_ptr, timer_value);
      break;

    case TDSRRCTMR_GET_TA_DURATION_TIMER:
      rex_set_timer(tdsrrc_get_ta_duration_timer_ptr, timer_value);
      break;

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
    case TDSRRCTMR_DED_PRI_VALIDITY_TIMER:
      rex_set_timer(tdsrrc_t_ded_pri_validity_timer_ptr,timer_value);
      break;
#endif
#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRCTMR_LTE_TO_TDSCDMA_REDIR_SRCH_TIMER:
      rex_set_timer(tdsrrc_t_lte_to_wcdma_redir_search_timer_ptr,timer_value);
      break;
#endif
    // CR 352866 begin
    case TDSRRCTMR_RCE_WAIT_RESELCT_TIMER:
      rex_set_timer(tdsrrc_t_rce_wait_reselect_ptr,timer_value);
      break;
    // CR 352866 end
    // CR430034 RAU Reject workaround
    case TDSRRCTMR_DELAY_OOS_RLF_RPT_TIMER:
      rex_set_timer(tdsrrc_t_delay_oos_rlf_rpt_timer_ptr,timer_value);
      break;

    case TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER:
      rex_set_timer(tdsrrc_t_sib_schdule_update_wait_timer_ptr,timer_value);
      break;

    case TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER:
      rex_set_timer(tdsrrc_t_sib7_update_wait_timer_ptr,timer_value);
      break;

    case TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER:
      rex_set_timer(tdsrrc_t_inter_srns_ignore_mr_timer_ptr,timer_value);
      break;
    
    case TDSRRCTMR_SOFT_ABORT_GUARD_TIMER:
      rex_set_timer(tdsrrc_t_soft_abort_timer_ptr, timer_value);
      break;
    case TDSRRCTMR_HOLD_PS_TIMER:
      rex_set_timer(tdsrrc_t_hold_ps_timer_ptr,timer_value);
      break;
    case TDSRRCTMR_BLOCK_CS_RESEL_NAS_API_TIMER:
      rex_set_timer(tdsrrc_block_cs_resel_nas_api_timer_ptr, timer_value);
      break;
    case TDSRRCTMR_BLOCK_PS_RESEL_NAS_API_TIMER:
      rex_set_timer(tdsrrc_block_ps_resel_nas_api_timer_ptr, timer_value);
      break;


    case TDSRRCTMR_DA_BACKOFF_TIMER:
      rex_set_timer(tdsrrc_da_backoff_timer_ptr, timer_value);
      break;

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
    case TDSRRCTMR_BPLMN_PARTIAL_REPORT_TIMER:
     rex_set_timer(tdsrrc_t_bplmn_partial_report_timer_ptr, timer_value); 
     break;
#endif

    case TDSRRCTMR_IGNORE_NON_EXIST_MCM_TIMER:
      rex_set_timer(tdsrrc_ignore_non_exist_mcm_timer_ptr, timer_value); 
      break;
    /* Set Timer for td2td manual bplmn allowed search */
    case TDSRRCTMR_TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER:
      rex_set_timer(tdsrrc_td2td_manual_bplmn_guard_srch_timer_ptr, timer_value); 
      break;

    case TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER:
      rex_set_timer(tdsrrc_abort_resel_to_grr_timer_ptr, timer_value); 
      break;
    case TDSRRCTMR_SIB_STOP_PARTITION_TIMER:
      rex_set_timer(tdsrrc_t_sib_stop_part_ptr, timer_value);
      break;

    case TDSRRCTMR_SIB_MASK_INVERT_TIMER:
      rex_set_timer(tdsrrc_t_sib_mask_ptr, timer_value);
      break;

    case TDSRRCTMR_SIB_EMPTY_MASK_TIMER:
      rex_set_timer(tdsrrc_t_sib_empty_mask_ptr, timer_value);
      break;

    case TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER:
      rex_set_timer(tdsrrc_t_sib_all_one_mask_ptr, timer_value);
      break;
    
    case TDSRRCTMR_RCE_ALLOW_TA_TIMER:
      rex_set_timer(tdsrrc_t_rce_allow_ta_timer_ptr, timer_value);
     break;
   case TDSRRCTMR_QTA_GUARD_TIMER: 
   	  rex_set_timer(tdsrrc_t_qta_guard_timer_ptr, timer_value);
   	 break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupported timer is attempted to start");
      break; 
  }

  tdsrrctmr_get_timer_name(timer, timer_name);
  TDSRRC_MSG_SPRINTF_3(MSG_LEGACY_HIGH, "Started timer:%d (%s) val:%d ms",timer, timer_name, timer_value);
}/* tdsrrctmr_start_timer */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================

FUNCTION tdsrrctmr_stop_timer

DESCRIPTION
  This function will clears the timer value and stops the timer
  timers.
DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrctmr_stop_timer
(
  tdsrrctmr_timer_e_type timer                 /* Timer to stop */
)       
{
  char timer_name[TDSRRCTMR_TIMER_NAME_LEN_MAX];

  memset((void *)timer_name, '\0', sizeof(timer_name));

  switch(timer)
  {
    case TDSRRCTMR_T_300_TIMER:
      rex_clr_timer(tdsrrc_t_300_ptr);
      break;
    case TDSRRCTMR_T_308_TIMER:
      rex_clr_timer(tdsrrc_t_308_ptr);
      break;
    case TDSRRCTMR_T_304_TIMER:
      rex_clr_timer(tdsrrc_t_304_ptr); 
      break;
    case TDSRRCTMR_WAIT_TIME_TIMER:
      rex_clr_timer(tdsrrc_t_wait_time_ptr);
      break;
    case TDSRRCTMR_UE_TO_CAMP_ON_TIMER:
      rex_clr_timer(tdsrrc_t_ue_to_camp_on_ptr);
      break;
    case TDSRRCTMR_RRC_CON_DELAY_TIMER:
      rex_clr_timer(tdsrrc_t_rrc_con_delay_ptr);
      break;
    case TDSRRCTMR_RCE_L2ACK_TIMER:
      rex_clr_timer(tdsrrc_t_rce_l2ack_ptr);
      break;
    case TDSRRCTMR_RCR_L2ACK_TIMER:
      rex_clr_timer(tdsrrc_t_rcr_l2ack_ptr);
      break;
    case TDSRRCTMR_RSSI_BER_TIMER:
      rex_clr_timer(tdsrrc_t_rssi_ber_ptr);
      break;
    case TDSRRCTMR_SIB_WAIT_TIMER:
      rex_clr_timer(tdsrrc_t_sib_wait_ptr);
      break;
    case TDSRRCTMR_T_302_TIMER:
      rex_clr_timer(tdsrrc_t_302_ptr);
      break;
    case TDSRRCTMR_T_305_TIMER:
      rex_clr_timer(tdsrrc_t_305_ptr);
      break;
    case TDSRRCTMR_T_316_TIMER:
      rex_clr_timer(tdsrrc_t_316_ptr);
      break;
    case TDSRRCTMR_T_317_TIMER:
      rex_clr_timer(tdsrrc_t_317_ptr);
      break;
    case TDSRRCTMR_T_307_TIMER:
      rex_clr_timer(tdsrrc_t_307_ptr);
      break;
    case TDSRRCTMR_T_314_TIMER:
      rex_clr_timer(tdsrrc_t_314_ptr);
      break;
    case TDSRRCTMR_T_315_TIMER:
      rex_clr_timer(tdsrrc_t_315_ptr);
      break;
    case TDSRRCTMR_SIB7_EXP_TIMER:
      rex_clr_timer(tdsrrc_t_sib7_exp_ptr); 
      break;
    case TDSRRCTMR_IGNORE_BCCH_MOD_TIMER:
      rex_clr_timer(tdsrrc_t_ignore_bcch_mod_exp_ptr); 
      break;
    case TDSRRCTMR_DCH_FACH_CELL_SELECTION_TIMER:
      rex_clr_timer(tdsrrc_t_dch_fach_cell_selection_exp_ptr); 
      break;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    case TDSRRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER:
      rex_clr_timer(tdsrrc_t_fach_to_fach_pch_cell_selection_exp_ptr); 
      break;
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */
    case TDSRRCTMR_DCH_PCH_CELL_SELECTION_TIMER:
      rex_clr_timer(tdsrrc_t_dch_pch_cell_selection_exp_ptr); 
      break;
    case TDSRRCTMR_L1_DEADLOCK_DETECT_TIMER:
      rex_clr_timer(tdsrrc_t_l1_deadlock_detection_exp_ptr); 
      break;
    case TDSRRCTMR_DELAY_ERR_FATAL_TIMER:
      rex_clr_timer(tdsrrc_t_delay_err_fatal_exp_ptr); 
      break;
    case TDSRRCTMR_T_3174_TIMER:
      rex_clr_timer(tdsrrc_t_3174_ptr);
      break;
    case TDSRRCTMR_FREQ_SCAN_TIMER:
      rex_clr_timer(tdsrrc_t_freq_scan_exp_ptr); 
      break;
    case TDSRRCTMR_T_OOS_TIMER:
      rex_clr_timer(tdsrrc_t_oos_ptr); 
      break;

    case TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER:
      rex_clr_timer(tdsrrc_t_deep_sleep_no_svc_ptr); 
      break;

     case TDSRRCTMR_PHY_CHAN_FAILURE_TIMER:
      rex_clr_timer(tdsrrc_t_phy_chan_failure_ptr);
      break;  

    case TDSRRCTMR_BPLMN_SRCH_TIMER:
      rex_clr_timer(tdsrrc_t_bplmn_srch_ptr); 
      break;

    case TDSRRCTMR_BPLMN_FREQ_SCAN_TIMER:
      rex_clr_timer(tdsrrc_t_bplmn_full_scan_srch_ptr); 
      break;     


    case TDSRRCTMR_FREQ_REDIRECT_WAIT_TIMER:
      rex_clr_timer(tdsrrc_t_freq_redirect_wait_timer_ptr); 
      break;   

    case TDSRRCTMR_CONN_MODE_OOS_TIMER:
      rex_clr_timer(tdsrrc_t_conn_mode_oos_timer_ptr);
      break;       


    case RRCTMR_TD2TD_BPLMN_DRX_SRCH_TIMER:
      rex_clr_timer(tdsrrc_t_tdstotds_bplmn_srch_ptr); 
      break;
  
    case TDSRRCTMR_BPLMN_GUARD_SRCH_TIMER:
      rex_clr_timer(tdsrrc_t_tdstotds_bplmn_guard_srch_ptr); 
      break;

    case TDSRRCTMR_SIXHR_SIB_UPDATE_TIMER:
      rex_clr_timer(tdsrrc_sixhr_sib_update_timer_ptr);
     break;


    case TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER:
      rex_clr_timer(tdsrrc_t_conn_mode_oos_initial_no_ds_timer_ptr);
      break;       

    case TDSRRCTMR_CONN_MODE_OOS_DS_TIMER:
      rex_clr_timer(tdsrrc_t_conn_mode_oos_ds_timer_ptr);
      break;       

    case TDSRRC_SIB_CHANGE_GUARD_TIMER:
      rex_clr_timer(tdsrrc_sib_change_guard_timer_ptr); 
      break; 

    case TDSRRC_BCCH_MODIFICATION_TIMER:
      rex_clr_timer(tdsrrc_bcch_mod_timer_ptr); 
      break;


    case RRCTMR_G2TD_REDIRECTION_WAIT_TIMER:
      rex_clr_timer(tdsrrc_t_gtow_redirection_timer_ptr);
      break;
	case RRCTMR_G2TD_REDIRECTION_LFS_TIMER:
      rex_clr_timer(tdsrrc_t_gtow_redir_lfs_timer_ptr);
      break;

    case RRCTMR_TD2G_ABORT_GUARD_TIMER:
      rex_clr_timer(tdsrrc_t_wtog_abort_guard_timer_ptr); 
      break;


    case TDSRRCTMR_T_319_TIMER:
      rex_clr_timer(tdsrrc_t_319_ptr);
      break;

    case TDSRRCTMR_T_320_TIMER:
      rex_clr_timer(tdsrrc_t_320_ptr);
      break;

#ifdef FEATURE_TDSCDMA_REL7_SPECRS
    case TDSRRCTMR_COUEC_TIMER:
      rex_clr_timer(tdsrrc_t_couec_timer_ptr);
      break;
#endif /*FEATURE_TDSCDMA_REL7_SPECRS*/


#if defined(FEATURE_3GPP_FAST_DORMANCY)
    case TDSRRCTMR_T_323_TIMER:
      rex_clr_timer(tdsrrc_t_323_ptr);   
      break;  
#endif

    case TDSRRCTMR_WRM_LOCK_TIMER:
      rex_clr_timer(tdsrrc_wrm_lock_timer_ptr);
      break;
    case TDSRRCTMR_WRM_DELAY_PREEMPTION_TIMER:
      rex_clr_timer(tdsrrc_wrm_delay_preemption_timer_ptr);
      break;

    case TDSRRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER:
      tdsrrc_clr_non_defer_timer(tdsrrc_dsim_bplmn_lock_rel_off_timer_ptr);  
      break;
    case TDSRRCTMR_WMR_UNLCOK_BY_TIMER:
      rex_clr_timer(tdsrrc_dsim_wrm_unlock_by_timer_ptr);
      break;

    case TDSRRCTMR_GET_TA_DURATION_TIMER:
      rex_clr_timer(tdsrrc_get_ta_duration_timer_ptr);
      break;


#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
    case TDSRRCTMR_DED_PRI_VALIDITY_TIMER:
      rex_clr_timer(tdsrrc_t_ded_pri_validity_timer_ptr);
      break;
#endif
#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRCTMR_LTE_TO_TDSCDMA_REDIR_SRCH_TIMER:
      rex_clr_timer(tdsrrc_t_lte_to_wcdma_redir_search_timer_ptr);
      break;
#endif
    // CR 352866 begin
    case TDSRRCTMR_RCE_WAIT_RESELCT_TIMER:
      rex_clr_timer(tdsrrc_t_rce_wait_reselect_ptr);
      break;
    // CR 352866 end
    // CR430034 RAU Reject workaround
    case TDSRRCTMR_DELAY_OOS_RLF_RPT_TIMER:
      rex_clr_timer(tdsrrc_t_delay_oos_rlf_rpt_timer_ptr);
      break;

    case TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER:
      rex_clr_timer(tdsrrc_t_sib_schdule_update_wait_timer_ptr);
      break;

    case TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER:
      rex_clr_timer(tdsrrc_t_sib7_update_wait_timer_ptr);
      break;

    case TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER:
      rex_clr_timer(tdsrrc_t_inter_srns_ignore_mr_timer_ptr);
      break;

    case TDSRRCTMR_SOFT_ABORT_GUARD_TIMER:
      rex_clr_timer(tdsrrc_t_soft_abort_timer_ptr);
      break;
    case TDSRRCTMR_HOLD_PS_TIMER:
      rex_clr_timer(tdsrrc_t_hold_ps_timer_ptr);
      break;
    case TDSRRCTMR_BLOCK_CS_RESEL_NAS_API_TIMER:
      rex_clr_timer(tdsrrc_block_cs_resel_nas_api_timer_ptr);
      break;
    case TDSRRCTMR_BLOCK_PS_RESEL_NAS_API_TIMER:
      rex_clr_timer(tdsrrc_block_ps_resel_nas_api_timer_ptr);
      break;


    case TDSRRCTMR_DA_BACKOFF_TIMER:
      rex_clr_timer(tdsrrc_da_backoff_timer_ptr);
      break;

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
    case TDSRRCTMR_BPLMN_PARTIAL_REPORT_TIMER:
     rex_clr_timer(tdsrrc_t_bplmn_partial_report_timer_ptr); 
     break;
#endif   

    case TDSRRCTMR_IGNORE_NON_EXIST_MCM_TIMER:
     rex_clr_timer(tdsrrc_ignore_non_exist_mcm_timer_ptr);
     break;
    /* Stop Timer for td2td manual bplmn allowed search */
    case TDSRRCTMR_TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER:
      rex_clr_timer(tdsrrc_td2td_manual_bplmn_guard_srch_timer_ptr); 
      break;

    case TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER:
     rex_clr_timer(tdsrrc_abort_resel_to_grr_timer_ptr);
     break;
    case TDSRRCTMR_SIB_STOP_PARTITION_TIMER:
      rex_clr_timer(tdsrrc_t_sib_stop_part_ptr);
      break;

    case TDSRRCTMR_SIB_MASK_INVERT_TIMER:
      rex_clr_timer(tdsrrc_t_sib_mask_ptr);
      break;

    case TDSRRCTMR_SIB_EMPTY_MASK_TIMER:
      rex_clr_timer(tdsrrc_t_sib_empty_mask_ptr);
      break;

    case TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER:
      rex_clr_timer(tdsrrc_t_sib_all_one_mask_ptr);
      break;
     
    case TDSRRCTMR_RCE_ALLOW_TA_TIMER:
      rex_clr_timer(tdsrrc_t_rce_allow_ta_timer_ptr);
      break;
	case TDSRRCTMR_QTA_GUARD_TIMER: 
	  rex_clr_timer(tdsrrc_t_qta_guard_timer_ptr);
	 break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR Unsupported timer is attempted to stop");
      break; 
  }

  tdsrrctmr_get_timer_name(timer, timer_name);
  TDSRRC_MSG_SPRINTF_2(MSG_LEGACY_HIGH, "Stopped timer:%d (%s)",timer,timer_name);
} /* tdsrrctmr_stop_timer */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================

FUNCTION tdsrrctmr_get_remaining_time

DESCRIPTION
  This function will return the remaining tics for the Timer passed in.
  
DEPENDENCIES
  None

RETURN VALUE
  Unsigned Long.  Returns 0 if times is not recognized.
  
SIDE EFFECTS
  None
===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
uint64 tdsrrctmr_get_remaining_time
(
  tdsrrctmr_timer_e_type timer                 /* Timer to stop */
)       
{
  uint64 remaining_time = 0; 

  switch(timer)
  {
    case TDSRRCTMR_T_300_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_300_ptr);
      break;
    case TDSRRCTMR_T_308_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_308_ptr);
      break;
    case TDSRRCTMR_T_304_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_304_ptr); 
      break;
    case TDSRRCTMR_WAIT_TIME_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_wait_time_ptr);
      break;
    case TDSRRCTMR_UE_TO_CAMP_ON_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_ue_to_camp_on_ptr);
      break;
    case TDSRRCTMR_RRC_CON_DELAY_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_rrc_con_delay_ptr);
      break;
    case TDSRRCTMR_RCE_L2ACK_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_rce_l2ack_ptr);
      break;
    case TDSRRCTMR_RCR_L2ACK_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_rcr_l2ack_ptr);
      break;
    case TDSRRCTMR_RSSI_BER_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_rssi_ber_ptr);
      break;
    case TDSRRCTMR_SIB_WAIT_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_sib_wait_ptr);
      break;
    case TDSRRCTMR_T_302_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_302_ptr);
      break;
    case TDSRRCTMR_T_305_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_305_ptr);
      break;
    case TDSRRCTMR_T_316_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_316_ptr);
      break;
    case TDSRRCTMR_T_317_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_317_ptr);
      break;
    case TDSRRCTMR_T_307_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_307_ptr);
      break;
    case TDSRRCTMR_T_314_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_314_ptr);
      break;
    case TDSRRCTMR_T_315_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_315_ptr);
      break;
    case TDSRRCTMR_SIB7_EXP_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_sib7_exp_ptr); 
      break;
    case TDSRRCTMR_IGNORE_BCCH_MOD_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_ignore_bcch_mod_exp_ptr); 
      break;
    case TDSRRCTMR_DCH_FACH_CELL_SELECTION_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_dch_fach_cell_selection_exp_ptr); 
      break;
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    case TDSRRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_fach_to_fach_pch_cell_selection_exp_ptr); 
      break;
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */
    case TDSRRCTMR_DCH_PCH_CELL_SELECTION_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_dch_pch_cell_selection_exp_ptr); 
      break;
    case TDSRRCTMR_L1_DEADLOCK_DETECT_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_l1_deadlock_detection_exp_ptr); 
      break;
    case TDSRRCTMR_DELAY_ERR_FATAL_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_delay_err_fatal_exp_ptr); 
      break;
    case TDSRRCTMR_T_3174_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_3174_ptr);
      break;
    case TDSRRCTMR_FREQ_SCAN_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_freq_scan_exp_ptr); 
      break;
    case TDSRRCTMR_T_OOS_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_oos_ptr); 
      break;

    case TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_deep_sleep_no_svc_ptr); 
      break;

     case TDSRRCTMR_PHY_CHAN_FAILURE_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_phy_chan_failure_ptr);
      break;  

    case TDSRRCTMR_BPLMN_SRCH_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_bplmn_srch_ptr); 
      break;

    case TDSRRCTMR_BPLMN_FREQ_SCAN_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_bplmn_full_scan_srch_ptr); 
      break;     


    case TDSRRCTMR_FREQ_REDIRECT_WAIT_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_freq_redirect_wait_timer_ptr); 
      break;   

    case TDSRRCTMR_CONN_MODE_OOS_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_conn_mode_oos_timer_ptr); 
      break;   


    case RRCTMR_TD2TD_BPLMN_DRX_SRCH_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_tdstotds_bplmn_srch_ptr); 
      break;                                                 
  
    case TDSRRCTMR_BPLMN_GUARD_SRCH_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_tdstotds_bplmn_guard_srch_ptr); 
      break;         

    case TDSRRCTMR_SIXHR_SIB_UPDATE_TIMER:
      remaining_time = rex_get_timer(tdsrrc_sixhr_sib_update_timer_ptr);
     break;


    case TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_conn_mode_oos_initial_no_ds_timer_ptr);
     break;

    case TDSRRCTMR_CONN_MODE_OOS_DS_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_conn_mode_oos_ds_timer_ptr);
     break;

    case TDSRRC_SIB_CHANGE_GUARD_TIMER:
      remaining_time = rex_get_timer(tdsrrc_sib_change_guard_timer_ptr); 
      break; 

    case TDSRRC_BCCH_MODIFICATION_TIMER:
      remaining_time = rex_get_timer(tdsrrc_bcch_mod_timer_ptr); 
      break;

    case RRCTMR_G2TD_REDIRECTION_WAIT_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_gtow_redirection_timer_ptr);
     break;

	case RRCTMR_G2TD_REDIRECTION_LFS_TIMER:
	   remaining_time = rex_get_timer(tdsrrc_t_gtow_redir_lfs_timer_ptr);
     break;

    case RRCTMR_TD2G_ABORT_GUARD_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_wtog_abort_guard_timer_ptr);
      break;

 
    case TDSRRCTMR_T_319_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_319_ptr);
      break;

    case TDSRRCTMR_T_320_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_320_ptr);
      break;

#ifdef FEATURE_TDSCDMA_REL7_SPECRS

    case TDSRRCTMR_COUEC_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_couec_timer_ptr);
      break;	  
#endif /* FEATURE_TDSCDMA_REL7_SPECRS */


#if defined(FEATURE_3GPP_FAST_DORMANCY)
    case TDSRRCTMR_T_323_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_323_ptr);
      break;
#endif

    case TDSRRCTMR_WRM_LOCK_TIMER:
      remaining_time = rex_get_timer(tdsrrc_wrm_lock_timer_ptr);
      break;
    case TDSRRCTMR_WRM_DELAY_PREEMPTION_TIMER:
      remaining_time = rex_get_timer(tdsrrc_wrm_delay_preemption_timer_ptr);
      break;
    case TDSRRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER:
      remaining_time = tdsrrc_get_non_defer_timer(tdsrrc_dsim_bplmn_lock_rel_off_timer_ptr);
      break;
    case TDSRRCTMR_WMR_UNLCOK_BY_TIMER:
      remaining_time = rex_get_timer(tdsrrc_dsim_wrm_unlock_by_timer_ptr);
      break;

    case TDSRRCTMR_GET_TA_DURATION_TIMER:
      remaining_time = rex_get_timer(tdsrrc_get_ta_duration_timer_ptr);
      break;
#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
    case TDSRRCTMR_DED_PRI_VALIDITY_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_ded_pri_validity_timer_ptr);
      break;
#endif
#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRCTMR_LTE_TO_TDSCDMA_REDIR_SRCH_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_lte_to_wcdma_redir_search_timer_ptr);
      break;
#endif
    // CR 352866 begin
    case TDSRRCTMR_RCE_WAIT_RESELCT_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_rce_wait_reselect_ptr);
      break;
    // CR 352866 end
    // CR430034 RAU Reject workaround
    case TDSRRCTMR_DELAY_OOS_RLF_RPT_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_delay_oos_rlf_rpt_timer_ptr);
      break;

    case TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_sib_schdule_update_wait_timer_ptr);
      break;

    case TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_sib7_update_wait_timer_ptr);
      break;

    case TDSRRCTMR_INTER_SRNS_IGNORE_MR_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_inter_srns_ignore_mr_timer_ptr);
      break;

    case TDSRRCTMR_SOFT_ABORT_GUARD_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_soft_abort_timer_ptr);
      break;
    case TDSRRCTMR_BLOCK_CS_RESEL_NAS_API_TIMER:
      remaining_time = rex_get_timer(tdsrrc_block_cs_resel_nas_api_timer_ptr);
      break; 
    case TDSRRCTMR_BLOCK_PS_RESEL_NAS_API_TIMER:
      remaining_time = rex_get_timer(tdsrrc_block_ps_resel_nas_api_timer_ptr);
      break;
     
    case TDSRRCTMR_DA_BACKOFF_TIMER:
      remaining_time = rex_get_timer(tdsrrc_da_backoff_timer_ptr);
      break;

    case TDSRRCTMR_IGNORE_NON_EXIST_MCM_TIMER:
      remaining_time = rex_get_timer(tdsrrc_ignore_non_exist_mcm_timer_ptr);
      break;

    case TDSRRCTMR_ABORT_RESEL_REQ_TO_GRR_TIMER:
      remaining_time = rex_get_timer(tdsrrc_abort_resel_to_grr_timer_ptr);
      break;
      
    /* Get remain Time for td2td manual bplmn search guard timer */
    case TDSRRCTMR_TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER:
      remaining_time = rex_get_timer(tdsrrc_td2td_manual_bplmn_guard_srch_timer_ptr); 
      break; 
    case TDSRRCTMR_SIB_STOP_PARTITION_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_sib_stop_part_ptr);
      break;

    case TDSRRCTMR_SIB_MASK_INVERT_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_sib_mask_ptr);
      break;

    case TDSRRCTMR_SIB_EMPTY_MASK_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_sib_empty_mask_ptr);
      break;

    case TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_sib_all_one_mask_ptr);
      break;
    
    case TDSRRCTMR_RCE_ALLOW_TA_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_rce_allow_ta_timer_ptr);
      break;
    case TDSRRCTMR_QTA_GUARD_TIMER:
      remaining_time = rex_get_timer(tdsrrc_t_qta_guard_timer_ptr);
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR Getting remaining time for timer %d is not supported",timer);
      break; 
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Remaining time %d for timer %d",remaining_time,timer);

  return remaining_time;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*The following lint directive limits the scope of this directive to 
 this file only */
/*lint +e534*/
