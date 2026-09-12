/*===========================================================================
                        R R C   C S P   U T I L
DESCRIPTION

   This file contains Utility functions for Cell Selection Procedure. These
   include the following categories -
    - Functions to evaluate cell restrictions.
    - Function to initialize Internal Data.
    - Initial and Stored Cell Selection Controller.
    - Function to initialize Selection Data.
    - Functions to fill Acq Entry from SIB.
    - Functions to send cmds to L1 (only those relevant to CSP).
    - Function to send cmds to SIB (Only those relevant to CSP)
    - Functions to send cmds to MM (Only those relevant to CSP)

   It implements part of the PLMN Search mechanism. The implemented
   blocks in PLMN Search mechanism are marked with a "=======" boundary below.

              CSP PLMN SEARCH MECHANISM
              
       -----------   ============    -------------    ---------
       |         |   | CSP      |    | Acq DB    |    | Acq   |
       |CSP      |   | Initial  |    | Access    |    | DB    |
       |State    |   | And .    |<-->| Functions |<-->|       |
       |Machine  |   | Stored   |    | -PLMN     |    |       |
       |Functions|<->|Selection |    | -Acq type |    |       |
       |         |   |Controller|    -------------    ---------
       |         |   |-PLMN     |    
       |         |   |-PLMN     |    ------------
       -----------   | type     |<-->| CSP      |
            |        ============    | Frequency|
            |           |            | Scan     |
            |           |            | Algorithm| 
       ===========      |            ------------             
       |Internal |   ============       
       |  Data   |   |Selection |
       |         |   |          |
       ===========   |Data      |
                     ============

EXTERNALIZED FUNCTIONS   
  tdsrrccsp_find_cell_bar_status()
    This function is responsible for finding the status of a cell in terms of
    Cell Barred Status.

  rrc_csp_find_cell_access()
    This function is responsible for finding the status of Access Control due
    to Access Class Restrictions.

  tdsrrccsp_init_int_data()
    This function is responsible for initializing the internal data structure
    to default values.

  tdsrrccsp_continue_stored_initial_select_ctrl()
    This function continues with the already started Initial and Stored Cell 
    Selection. It looks for a frequency for a "specific" PLMN or "Any" PLMN.

  tdsrrccsp_start_cell_selection()
    This function is responsible for starting a New Stored or Initial Cell 
    Selection.

  tdsrrccsp_init_select_data()
    This function is responsible for initializing the internal Search data 
    structure to default values.

  tdsrrccsp_compare_expected_cell()
    This function compares the expected acq. type with the actual Acquisition
    status mask and PLMN status mask and declares the actual cell as Better
    than/Same as/Below the expected cell.

  tdsrrccsp_send_llc_cmd()
    This function is responsible for allocating command buffer, filling it up 
    and sending it to RRC internal command queue. It takes care of RRC 
    internal commands (non-funneling type) going to LLC.

  tdsrrccsp_send_l1_cmd()
    This function is responsible for filling up data structures
    for all L1 commands sent by CSP. This also allocates cmd
    buffer and sends the command.

  tdsrrccsp_fill_acq_entry_from_sib()
    This function is responsible for filling up all fields of Acq Entry from 
    SIB data and SIB Confirmation command data.

  tdsrrccsp_send_sib_cmd()
    This function is responsible for allocating and filling up data structures
    for all RRC Internal commands to SIB procedure sent by CSP. It also 
    sends these cmds to SIB.

  tdsrrccsp_gen_status_mask()
    This function generates an Acq. status mask from Cell Barred status and 
    LAI Status.

  tdsrrccsp_send_mm_cmd()
    This function is responsible for filling up data structures for all 
    commands to MM. It further enqueues the command for MM.

  rrc_csp_send_mm_act_cnf_cmd()
    This function is responsible for filling up data structures for Activation 
    Confirm command and sending it to MM.

  tdsrrccsp_send_ccm_cmd()
    This function is responsible for filling up data structures for Cell 
    Change Request and sending it to CCM.

  tdsrrccsp_get_serving_pccpch_parms()
    This function fills up STTD indicator and scrambling code for serving cell 
    PCCPCH.

  tdsrrccsp_send_next_freq_to_l1()
    This function sends either an Acquisition Request or a 
    Frequency scan request to L1 (via LLC) based on the passed
    type of frequency scan.

  tdsrrccsp_handle_no_available_freq()
    This function handles failure to find any frequency for acquisition.

  tdsrrccsp_send_chan_release()
    Sends an TDSRRC_CHANNEL_CONFIG_REQ to LLC to release all channels.

  tdsrrccsp_start_sel_leaving_conn_mode()
    Starts cell selection on leaving connected mode. It starts with the
    last camped on cell.

  tdsrrccsp_cmd_to_mm_needed()
    This function determines if there is a need to send a 
    command to MM. If a command is needed to be sent, then
    which command is needed is also determined.

  tdsrrccsp_mm_primitive_sent()
    This function determines the CSP event based on the MM
    command and service status that was sent to MM. 

  tdsrrccsp_init_data_from_service_req()
    This function stores the data passed in TDSRRC_SERVICE_REQ.


INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrccsputil.c#2 $    $DateTime: 2021/02/04 23:39:42 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/08/11   mkh     Events renamed to TDS
03/11/11   hx      Change the present check for accessClassBarredList in 
                   tdsrrccsp_find_access_control
03/10/11   zz     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/


/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "msg.h"
#include "err.h"
#include "tdsuecomdef.h"
#include "task.h"
#include "IxErrno.h"

#include "tdsrrcasn1.h"
#include "tdsrrcmm_v.h"
#include "tdsrrcintif.h"
#include "tdsrrccmd_v.h"
#include "tdsl1rrcif.h"
#include "tdsl1mtask.h"

#include "tdsrrccspi.h"
#include "tdsrrcsibdb.h"
#include "tdsrrcsibproc.h"
#include "tdsrrcccm.h"
#include "tdsrrccsp.h"
#include "tdsrrccspi.h"
#include "tdsrrcllc.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcmcm.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "tdsrrctmr.h"
#include "mm_umts.h"
#include "tdsrrccu.h"
#include "tdsrrccui.h"
#include "tdsrrcueci.h"
#include "tdsrrcrce.h"
#include "tdsrrccho.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "assert.h"
#include "tdsrrcpg1.h"

#ifdef FEATURE_RF
#include "rfm.h"
#endif

#include "sys_eplmn_list.h"

#ifdef FEATURE_EQUIVALENT_HPLMN
#include "reg_sim.h"
#endif

/*#include "rr_rrc_if.h"*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"

#include "reg_sim.h"
#include "tdsrrcmeas.h"
#include "tdsrrcllcoc.h"
#include "tdsrrclogging.h"

#ifdef FEATURE_LTE_TO_TDSCDMA
#include "tds_rrc_api.h"
#endif

#if defined(FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
#include <lte_rrc_ext_api.h>
#endif

#ifdef FEATURE_WRLF_SYSTEM_SEL
#include "tdsrrcrbe.h"
#endif
#include "tdsrrcdt.h"
#include "tdsl1trm.h"
#include "tdsrrcwrm.h"
#include "tdsrrcnv.h"

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsl1rrcif.h"
#include "tdsCFAParser_i.h"

#ifdef FEATURE_TDSCDMA_BAND_SPEC_DC_HSDPA
#include "rfm_wcdma.h"
#endif

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#include "tdsrrcfreefloating.h"
#endif

#include "fs_lib.h"
#include "tdsrrcmisc.h"
#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
#include "tdsrrcmeasi.h" //for tdsrrcmeas_configure_l1_with_priority_info_eutra_cell_list
#endif
#include "tdsrrcqsh.h"
#include "reg_sim.h"

/*!!! This should be defined when W+T mode is supported !!!*/
/* #define FEATURE_LOCAL_GWBAND_HAS_W */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
 
/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/* This data structure stores the Selection data for Initial and Stored
 * Selection Controller.
 */

tdsrrc_csp_select_ctrl_data_type  tdsrrc_csp_curr_select_data;

tdsrrc_csp_select_ctrl_data_type  tdsrrc_csp_prev_select_data;

extern rrc_service_ind_type tdsrrc_service_ind_backup;

extern sys_hs_ind_e_type tdshsdpa_hsupa_ind;

extern RRC_HOME_ZONE_IND_CB_TYPE *tdsrrc_home_zone_ind_cb_func;

/* When GCF flag is enabled then UE should not go into deep sleep
 * when UE is OOS in RRC Connected Mode
 */

extern tdsrrc_csp_acq_entry_type  tdslist_db[TDS_MAX_ACQ_DB_NV_ENTRIES];


extern boolean tdsdsim_bplmn_search_complete_w_cycle;

#ifdef FEATURE_3GPP_CSFB_TDS
extern boolean tdsrrc_csfb_call_status;
#endif
extern tdsrrc_ds_tuneaway_status_change_ind_type tds_ds_tuneaway_status_change_ind;
#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
extern tdsrrcrce_csfb_status_e_type tdsrrc_csfb_extended_status;
#endif

extern boolean tdsrrc_special_settings_nv_read;
extern boolean tdsrrc_special_settings_for_testing_nv;

extern tdsrrc_csp_band_f_pattern_type  tdsrrccsp_band_f_pattern;
extern tdsrrc_csp_band_a_pattern_type  tdsrrccsp_band_a_pattern;
extern tdsrrc_csp_band_e_pattern_type  tdsrrccsp_band_e_pattern;
#define TDSRRC_FORBIDDEN_CELL_BAR_TIMER_LENGTH 300

/* If TDS is last rat when foreground manual plmn list, we need to start lock wait timer */
extern boolean  tdsrrccsp_start_lock_wait_timer_for_forground_plmn;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
	** Forward Declarations
	** ----------------------------------------------------------------------- */

#if defined(FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
void tdsrrc_csp_add_frequency_to_detected_cell_list
(
  detected_frequency_type *detected_frequency
);
#endif

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


#ifdef FEATURE_BPLMN_SEARCH_320MS
/*===========================================================================

FUNCTION          RRC_CSP_CHECK_320MS_BPLMN

DESCRIPTION       This functions checks if CSP is required to do 320ms lossy BPLMN

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
boolean tdsrrccsp_check_320ms_bplmn(void)
{
  boolean ret_val=FALSE;
  if( (tdsrrc_csp_int_data.bplmn_nw_sel_mode != SYS_NETWORK_SELECTION_MODE_AUTOMATIC) &&
      ((tdsrrc_get_state() == TDSRRC_STATE_CELL_PCH) || (tdsrrc_get_state() == TDSRRC_STATE_URA_PCH)) && 
      (tdsrrcllc_get_drx_info() == (MIN_TD2TD_NORMAL_BPLMN_DRX_CYCLE_LEN/2))
     )
  {
    ret_val=TRUE;
  }
  return ret_val;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*===========================================================================

FUNCTION          RRC_SUB_SFN

DESCRIPTION       This function subtract two system frame numbers.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
uint16 tdsrrccsp_sub_sfn(uint16 sfn, uint16 subtract_by)
{
  if (sfn < subtract_by) 
  {
   return (sfn + 4096 - subtract_by);
  }
  else 
  {
    return (sfn - subtract_by);
  }
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
#endif


/* -----------------------------------------------------------------------
** Local Defines
** ----------------------------------------------------------------------- */

/* Converts hcs tcr_max_hyst from ASN1 format to L1 format */

#define  RRCCSP_RET_HCS_TCR_MAX_HYST(max_hyst)  \
(  \
 (max_hyst == tdsrrc_T_CRMaxHyst_notUsed) ? TDSL1_T_CR_MAX_HYST_NOT_USED : \
  ((max_hyst == tdsrrc_T_CRMaxHyst_t10) ? TDSL1_T_CR_MAX_HYST_10S : \
  ((max_hyst == tdsrrc_T_CRMaxHyst_t20) ? TDSL1_T_CR_MAX_HYST_20S :\
  ((max_hyst == tdsrrc_T_CRMaxHyst_t30) ? TDSL1_T_CR_MAX_HYST_30S :\
  ((max_hyst == tdsrrc_T_CRMaxHyst_t40) ? TDSL1_T_CR_MAX_HYST_40S :\
  ((max_hyst == tdsrrc_T_CRMaxHyst_t50) ? TDSL1_T_CR_MAX_HYST_50S :\
  (max_hyst == tdsrrc_T_CRMaxHyst_t60) ? TDSL1_T_CR_MAX_HYST_60S : TDSL1_T_CR_MAX_HYST_70S \
  ))))) \
)

/* =======================================================================
**                            Function Definitions
** ======================================================================= */

static void tdsrrccsp_display_CN_domains
(
   rrc_to_mm_sink_e_type         cmd_id,
   uint32                        num_cn_domains, 
   rrc_cn_domain_identity_e_type id1, 
   rrc_cn_domain_identity_e_type id2
)
{

   if (num_cn_domains == 0)
   {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC to MM cmd(%d): 0 cn_domain.",cmd_id);
   }
   else if (num_cn_domains == 1)
   {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSRRC to MM cmd(%d): 1 cn_domain: %d.",cmd_id,id1);
   }
   else if (num_cn_domains == 2)
   {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDSRRC to MM cmd(%d): 2 cn_domains: %d, %d.", cmd_id, id1, id2);
   }
   else
   {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"TDSRRC to MM cmd(%d): invalid cn_domain number: %d.",cmd_id,num_cn_domains);
   }
        
}

 /*===========================================================================

FUNCTION          RRC_CSP_BCCH_AND_NAS_LAIS_MATCH

DESCRIPTION       This function is responsible for comparing LAI received
                  on BCCH with one sent by NAS. It declares a match if both 
                  LAC and PLMNs match.
DEPENDENCIES
                  None
RETURN VALUE
                  TRUE: LAIs match
                  FALSE: LAIs don't match
SIDE EFFECTS
                  None
===========================================================================*/
static boolean  rrc_csp_bcch_and_nas_lais_match(rrc_lai_type bcch_lai, rrc_lai_type nas_lai)
{
  uint8  i;

  /* If both the LAC and PLMNs match, declare a match for LAI */
  if(tdsrrc_bcch_and_nas_plmns_match(bcch_lai.plmn, nas_lai.plmn))
  {
    for(i=0; i<RRC_MAX_GSMMAP_LAC_LENGTH; i++)
    {
      if(bcch_lai.lac[i] != nas_lai.lac[i])
      {
        return(FALSE);
      }
    }
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }
} /* rrc_csp_bcch_and_nas_lais_match */

/*===========================================================================

FUNCTION          tdsrrccsp_get_supported_band_list

DESCRIPTION       This function checks which bands are supported and sends the
                  populated list to UECI.

DEPENDENCIES
                  
RETURN VALUE
                  None.

SIDE EFFECTS
                  None.
                  
===========================================================================*/
void tdsrrccsp_get_supported_band_list(tdsrrcueci_band_storage_type * ueci_band)
{
  /* TDD bands */
  if(tdsrrccsp_is_preference_band(RRC_CSP_TDS_A_BAND))
  {
    ueci_band->band_type_present[tdsrrc_RadioFrequencyBandTDD_a]=TRUE;
    ueci_band->band_type[ueci_band->num_supported_bands]=(uint32)tdsrrc_RadioFrequencyBandTDD_a;
    ueci_band->num_supported_bands++;
  }

  if(tdsrrccsp_is_preference_band(RRC_CSP_TDS_F_BAND))
  {
    ueci_band->band_type_present_tddv860[ueci_band->num_supported_bands_tddv860]=TRUE;
    ueci_band->band_type_tddv860[ueci_band->num_supported_bands_tddv860]=(uint32)tdsrrc_RadioFrequencyBandTDDext_f;
    ueci_band->num_supported_bands_tddv860++;
  }

  if(tdsrrccsp_is_preference_band(RRC_CSP_TDS_E_BAND))
  {
    ueci_band->band_type_present_tddv860[ueci_band->num_supported_bands_tddv860]=TRUE;
    ueci_band->band_type_tddv860[ueci_band->num_supported_bands_tddv860]=(uint32)tdsrrc_RadioFrequencyBandTDDext_e;
    ueci_band->num_supported_bands_tddv860++;
  }
  
} /* tdsrrccsp_get_supported_band_list */

/*===========================================================================

FUNCTION          RRC_IS_BCCH_PLMN_AN_HPLMN_OR_EHPLMN

 DESCRIPTION       This function 
                          If FEATURE_EQUIVALENT_HPLMN is not defined,
                             calls Is_bcch_plmn_an_hplmn 
                           Else
                             calls Is_bcch_plmn_an_hplmn and reg_sim_is_ehplmn
                           When FEATURE_DUAL_SIM id defined, takes care of calling appropriate functions

DEPENDENCIES
                  None

RETURN VALUE
                  TRUE: Access allowed on this cell
                  FALSE: Access NOT allowed on this cell.

SIDE EFFECTS
                  None
===========================================================================*/
static boolean tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn
(
  rrc_plmn_identity_type plmn_id
)
{
    boolean is_hplmn_or_ehplmn = FALSE;

#ifdef FEATURE_EQUIVALENT_HPLMN
  sys_plmn_id_s_type nas_plmn_id;
#endif

#ifndef FEATURE_EQUIVALENT_HPLMN

#ifdef FEATURE_DUAL_SIM
  is_hplmn_or_ehplmn = mm_per_subs_is_bcch_plmn_an_hplmn(plmn_id,tdsrrc_get_as_id());
#else
  is_hplmn_or_ehplmn = Is_bcch_plmn_an_hplmn(plmn_id);
#endif /*FEATURE_DUAL_SIM*/

#else
  nas_plmn_id = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(plmn_id);

#ifdef FEATURE_DUAL_SIM
  is_hplmn_or_ehplmn = reg_sim_per_subs_is_ehplmn(nas_plmn_id,tdsrrc_get_as_id());
#else
  is_hplmn_or_ehplmn = reg_sim_is_ehplmn(nas_plmn_id);
#endif /*FEATURE_DUAL_SIM*/

#endif

  return is_hplmn_or_ehplmn;
}
/*===========================================================================

FUNCTION          RRC_CSP_VERIFY_HI_AC_ACCESS

DESCRIPTION       This function is responsible for finding if access to cell 
                  is allowed due to one of UE's Hi Access Classes.

DEPENDENCIES
                  None

RETURN VALUE
                  TRUE: Access allowed on this cell
                  FALSE: Access NOT allowed on this cell.

SIDE EFFECTS
                  None
===========================================================================*/
static boolean 
tdsrrccsp_verify_hi_ac_access (rrc_plmn_identity_type plmn_id,
                             tdsrrc_int_access_class_barred_list_type  ac_barred_list)
{
  tdsrrc_int_access_class_index_e_type i;
  uint16 tmp_shift_bit_mask = RRC_ACCESS_CLASS_11_BIT;

  rrc_plmn_identity_type home_plmn_id;

#ifdef FEATURE_DUAL_SIM
  home_plmn_id = mmumts_convert_nas_plmn_id_to_rrc_plmn_id(
                            reg_sim_per_subs_hplmn_get(tdsrrc_get_as_id()));
#else
  home_plmn_id = mmumts_convert_nas_plmn_id_to_rrc_plmn_id(reg_sim_hplmn_get());
#endif


  /* Check the UE Hi Access Class Bitmask to verify access due to any of the
   * UE Hi Access Classes
   */
  for( i = TDSRRC_INT_ACCESS_CLASS_11_INDEX, tmp_shift_bit_mask = 0x0001; 
       i < TDSRRC_INT_ACCESS_CLASS_MAX_INDEX;
       i++, tmp_shift_bit_mask <<= 1 )
  {
    if(( tdsrrc_hi_access_class & tmp_shift_bit_mask ) && 
       ( ac_barred_list.barred[i] == FALSE ))
    {
      /* The Cell is accessible for UE Hi Access Class. Now check if we
       * fulfill rules about Applicability of Access Class in serving network
       * Refer to 22.011, sec. 4.3
       */
      /* Get the home_plmn_id from NAS */ 
      tdsrrc_csp_int_data.home_plmn_id = home_plmn_id;

      if(((i == TDSRRC_INT_ACCESS_CLASS_11_INDEX) ||
          (i == TDSRRC_INT_ACCESS_CLASS_15_INDEX)) && 
          (tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn(plmn_id)))
      {
        /* We are in Home PLMN and UE Access Class is 11 or 15 - Full Access
         * is allowed.
         */
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"In HPLMN UE Hi AC %d  - Access Allowed",i);
         return(TRUE);
      }
      else if ( ((i == TDSRRC_INT_ACCESS_CLASS_12_INDEX) || 
                 (i == TDSRRC_INT_ACCESS_CLASS_13_INDEX) || 
                 (i == TDSRRC_INT_ACCESS_CLASS_14_INDEX)) && 
                (TDSRRC_CSP_SAME_MCC(plmn_id, tdsrrc_csp_int_data.home_plmn_id)))
      {
        /* We are on Access Class 12-14 and are in Home Country. Full Access
         * is allowed 
         */
         TDSRRC_MSG1(MSG_LEGACY_LOW,"In Home MCC UE Hi AC %d - Access Allowed",i);
         return(TRUE);
      }
    }
  }

  /* If we are here, it means the UE does not have even ONE hi access class 
   * that is:
   *       - NOT barred in System Info message AND 
   *       - UE satisfies Acceptability of Access Class in Serving network
   * Return a FALSE
   */
  return(FALSE);
}  /* tdsrrccsp_verify_hi_ac_access  */


/*===========================================================================

FUNCTION          RRC_CSP_FIND_CELL_BAR_STATUS

DESCRIPTION       This function is responsible for finding the status of
                  a cell in terms of Cell Barred status. The
                  Cell Access Restrictions due to Access Control are ignored
                  by this function.

DEPENDENCIES
                  - SIB procedure should have SIB 3 available for this cell.
                  - The UE Access Class should be known.

RETURN VALUE
                  FAILURE: If Cell Barred status couldn't be evaluated.
                  SUCCESS: Cell Barred status could be successfully evaluated.

SIDE EFFECTS
                  None

===========================================================================*/
uecomdef_status_e_type  
tdsrrccsp_find_cell_bar_status (rrc_plmn_identity_type  plmn_id, 
                              tdsrrc_int_cell_id_type  cell_id,
                              tdsrrc_csp_cell_bar_e_type  *cell_bar
                              ,rrc_plmn_identity_type common_plmn_id
                              )
{

  tdsrrc_SysInfoType3 *sib3_ptr;
  tdsrrc_SysInfoType4 *sib4_ptr;
  tdsrrc_CellAccessRestriction *sib3_4_access_restriction_ptr;
 
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  tdsrrc_csp_substate_e_type curr_csp_substate = tdsrrccsp_get_csp_substate();  
  tdsrrc_MasterInformationBlock *mib_ptr = NULL;

  tdsrrc_SysInfoType1 *sib1_ptr = NULL;


  rrc_plmn_identity_type sibidx_plmn_id;

  tdsrrc_csp_int_data.tds_cell_time_to_reselect = 0;
  *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;

  sibidx_plmn_id = common_plmn_id;
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sib_idx PLMN: MCC %d-MNC %d",TDSRRC_CSP_GET_INT_MCC(sibidx_plmn_id),TDSRRC_CSP_GET_INT_MNC(sibidx_plmn_id));
  /* Let us first read SIB3 - we should have that SIB for sure */
  /* TBD - Should we pass PLMN and Index to SIB ? */
  if((tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN)
     && (!tdsrrccsp_is_dummy_plmn_search(tdsrrc_csp_int_data.curr_req_plmn.plmn_id))
    )
  {
      sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrc_sibdb_return_sib_for_cell(
                              tdsrrc_SIB1, 
                              sibidx_plmn_id, cell_id);
    mib_ptr = (tdsrrc_MasterInformationBlock*)tdsrrc_sibdb_return_sib_for_cell(tdsrrc_MIB,sibidx_plmn_id,cell_id);

    sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrc_sibdb_return_sib_for_cell(
                              tdsrrc_SIB3, 
                              sibidx_plmn_id, cell_id);
    if( sib3_ptr == NULL )
    {
      /* SIB3 for this PLMN, CEll ID combination not present */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"ERR: SIB3 for MCC %d, MNC %d, Cell ID %d not present",
            TDSRRC_CSP_GET_INT_MCC(sibidx_plmn_id), TDSRRC_CSP_GET_INT_MNC(sibidx_plmn_id), cell_id);
      return(FAILURE);
    } /* SIB3 not present */

    if((rrc_state != TDSRRC_STATE_DISCONNECTED) && (rrc_state != TDSRRC_STATE_CONNECTING))
    {
      if(sib3_ptr->sib4indicator == TRUE)
      {
        /* Read SIB4. If it's not there mark and error and continue with SIB3 */
        sib4_ptr = (tdsrrc_SysInfoType4 *) tdsrrc_sibdb_return_sib_for_cell(
                                  tdsrrc_SIB4, 
                                  sibidx_plmn_id, cell_id);
        if( sib4_ptr == NULL )
        {
          /* SIB4 not present */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB4 indicated but not present,using SIB3");
          sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
        }
        else
        {
          sib3_4_access_restriction_ptr = &(sib4_ptr->cellAccessRestriction);
        }
      }
      else
      {
        sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
      }
    }
    else
    {
      sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
    }
  }
  /* TD2TD BPLMN search is in progress */
  else
  {
    mib_ptr = (tdsrrc_MasterInformationBlock*)tdsrrcsib_bplmn_return_sib_for_cell(tdsrrc_MIB);
    sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrcsib_bplmn_return_sib_for_cell(tdsrrc_SIB1);
    
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: reading SIB3 for cell bar/access restrictions");
    sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrcsib_bplmn_return_sib_for_cell(tdsrrc_SIB3);
                              
    if( sib3_ptr == NULL )
    {
      /* SIB3 for this PLMN, CEll ID combination not present */
      TDSRRC_MSG0(MSG_LEGACY_FATAL,"SIB3 empty when SIB_CNF returned with SUCCESS");
	  return(FAILURE);
    } /* SIB3 not present */

    sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
  }
  if (sib1_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_FATAL,"SIB1 empty when SIB_CNF returned with SUCCESS");
    return(FAILURE);
  }
  if(mib_ptr == NULL)
  {
     TDSRRC_MSG0(MSG_LEGACY_FATAL,"MIB empty when SIB_CNF returned with SUCCESS");
     return (FAILURE);
  }
  /* The SIB3 is present - lets disect it */


    if(tdsrrccsp_bar_forbid_cells(sib3_ptr,common_plmn_id,mib_ptr,sib1_ptr))
    {
       *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;
       tdsrrc_csp_int_data.tds_cell_time_to_reselect = TDSRRC_FORBIDDEN_CELL_BAR_TIMER_LENGTH;
       return SUCCESS;
    }


  if((TDSRRC_CHECK_COMMON_MSG_TYPE2(sib3_4_access_restriction_ptr->cellBarred,rrc_CellBarred,
                                                notBarred )) &&
     ( sib3_4_access_restriction_ptr->cellReservedForOperatorUse == 
                                                     tdsrrc_ReservedIndicator_notReserved ) &&
     ( sib3_4_access_restriction_ptr->cellReservationExtension == 
                                                        tdsrrc_ReservedIndicator_notReserved ))
  {
    /* Cell is NOT barred and NOT Reserved */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Not Barred,Not reserved");
    *cell_bar = TDSRRC_CSP_CELL_OK;
  } /* If Cell is NOT barred and NOT Reserved */

  else if((TDSRRC_CHECK_COMMON_MSG_TYPE2(sib3_4_access_restriction_ptr->cellBarred,rrc_CellBarred,
                                                      notBarred )) &&
          ( sib3_4_access_restriction_ptr->cellReservedForOperatorUse == 
                                                     tdsrrc_ReservedIndicator_notReserved ) &&
          ( sib3_4_access_restriction_ptr->cellReservationExtension == 
                                                      tdsrrc_ReservedIndicator_reserved ))
  {
    /* Cell is NOT Barred but RESERVED for SoLSA - Treat it as Barred since the 
     * UE does not have SoLSA support (TBD: May change for UEs supporting 
     * Rel 2000 )
     */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell treated Barred future Reserved");
    tdsrrc_csp_int_data.tds_cell_time_to_reselect = 1280;

    if((tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ) 
        &&  (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED))
    {
      /* Cell is Barred. Ignore the Intra-frequency cell re-selection indicator */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore Intra-freq cell resel ind");
      *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;
    }
    else if(((curr_csp_substate == TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB) 
               || (curr_csp_substate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB))
              && (tdsrrc_csp_int_data.new_cell_acq_entry.freq == tdsrrc_csp_int_data.curr_acq_entry.freq))
    {
      /* Intra-frequency neighbor frequency is barred. Bar the whole frequency */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Intra-Freq Ngbr Cell Freq barred. Sndng Freq Barred to L1");
      *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
    }
    else
    {
      /* Cell is Not Barred, but RESERVED for SoLSA (is treated as BARRED) AND
       * "Intra frequency Cell re-selection Indicator" is NOT TDSALLOWED */ 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Intra-freq Not Allowed");
      *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
    }
  } /* If Cell is NOT Barred, not reserved for Operator but RESERVED for SoLSA */

  else if ((TDSRRC_CHECK_COMMON_MSG_TYPE2(sib3_4_access_restriction_ptr->cellBarred,rrc_CellBarred,
                                                      notBarred) ) &&
     ( sib3_4_access_restriction_ptr->cellReservedForOperatorUse == 
                                                      tdsrrc_ReservedIndicator_reserved ))
  {
    /* Cell is NOT Barred but RESERVED for Operator */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell reserved for OPerator");


    /* Now check the UE Access Class to determine Cell Status */

   if(( tdsrrc_hi_access_class != RRC_HI_ACCESS_CLASS_ABSENT ) &&
       (((tdsrrc_hi_access_class & RRC_ACCESS_CLASS_11_BIT) ||
         (tdsrrc_hi_access_class & RRC_ACCESS_CLASS_15_BIT)) &&
        (tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn(plmn_id))))
    {
      /* UEs assigned to Access Class 11 or 15 may select/re-select this cell if in the home PLMN.
       * UEs assigned to an Access Class in the range 0 to 9 and 12 to 14 shall behave as if
       * cell status "barred" is indicated using the value "not allowed" in the
       * IE "Intra-frequency cell re-selection indicator" and the maximum value for Tbarred
       */

       TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE High AC present and in HPLMN");
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell treated NOT Barred");     
       *cell_bar = TDSRRC_CSP_CELL_OK;
    }
    else
    {
      if(tdsrrc_hi_access_class == RRC_HI_ACCESS_CLASS_ABSENT)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE High AC not present");
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell treated Barred (Op Reserved)");
      tdsrrc_csp_int_data.tds_cell_time_to_reselect = 1280;
      tdsrrc_csp_int_data.cell_reserved_for_operator_use = TRUE;
      if((tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ) 
          &&  (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED))
      {
        /* Cell is Barred. Ignore the Intra-frequency cell re-selection indicator */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore Intra-freq cell resel ind");
        *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;
      }
      else if(((curr_csp_substate == TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB) 
                 || (curr_csp_substate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB))
                && (tdsrrc_csp_int_data.new_cell_acq_entry.freq == tdsrrc_csp_int_data.curr_acq_entry.freq))
      {
        /* Intra-frequency neighbor frequency is barred. Barring the frequency */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Intra-Freq Ngbr Cell Freq barred. Sndng Freq Barred to L1");
        *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
      }
      else
      {
        /* UE has either a High Access Class but is NOT in Home PLMN
         * OR UE does NOT have a High Access Class */
        /* Cell is TREATED AS BARRED for this UE */
        /* Cell is Not Barred, but RESERVED for Operator (is treated as BARRED) 
         * AND "Intra frequency Cell re-selection Indicator" is NOT TDSALLOWED */ 
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"No UE Hi AC or not in HPLMN,Intra-freq Not Allowed");
        *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;       
      }
    }
  } /* If Cell is NOT Barred but RESERVED for Operator */

  else if (TDSRRC_CHECK_COMMON_MSG_TYPE2(sib3_4_access_restriction_ptr->cellBarred,rrc_CellBarred,
                                                      barred ))
  {

    switch(sib3_4_access_restriction_ptr->cellBarred.u.barred->t_Barred)
    {
      case tdsrrc_T_Barred_s10:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 10;
        break;
      case tdsrrc_T_Barred_s20:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 20;
        break;
      case tdsrrc_T_Barred_s40:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 40;
        break;
      case tdsrrc_T_Barred_s80:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 80;
        break;
      case tdsrrc_T_Barred_s160:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 160;
        break;
      case tdsrrc_T_Barred_s320:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 320;
        break;
      case tdsrrc_T_Barred_s640:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 640;
        break;
      case tdsrrc_T_Barred_s1280:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 1280;
        break;
      default:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unexpected. Setting the time to 1280 secs");
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 1280;
        break;
    }
    
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell is indicated as Barred");
    /* Cell is indicated as Barred */
    if( sib3_4_access_restriction_ptr->cellBarred.u.barred->
                      intraFreqCellReselectionInd == tdsrrc_AllowedIndicator_allowed )
    {
      /* Cell is Barred
       * AND "Intra frequency Cell re-selection Indicator" is TDSALLOWED */ 
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Intra-freq Allowed");
      *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;
    }
    else
    {
      if((tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ)
         && (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED))
      {
        /* Cell is Barred. Ignore the Intra-frequency cell re-selection indicator */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore Intra-freq cell resel ind");
        *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;
      }
      else if(((curr_csp_substate == TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB) 
                 || (curr_csp_substate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB))
                && (tdsrrc_csp_int_data.new_cell_acq_entry.freq == tdsrrc_csp_int_data.curr_acq_entry.freq))
      {
        /* Intra-frequency neighbor frequency is barred. Barring only the cell instead of the
         * the frequency as the serving cell is still not barred
         */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Intra-Freq Ngbr Cell Freq barred. Sndng Cell Barred instead of Freq Barred to L1");
        *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
      }
      else
      {
        /* Cell is Barred
         * AND "Intra frequency Cell re-selection Indicator" is NOT TDSALLOWED */ 
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Intra-freq Not Allowed");
        *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
      }
    }
  } /* Cell is indicated as Barred */
  else
  {
    /* We should not come here */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: Error in cell bar evaluation logic");
    tdsrrc_csp_int_data.tds_cell_time_to_reselect = 1280;

    if((tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ) 
        &&  (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED))
    {
      /* Cell is Barred. Ignore the Intra-frequency cell re-selection indicator */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore Intra-freq cell resel ind");
      *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;
    }
    else if(((curr_csp_substate == TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB) 
               || (curr_csp_substate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB))
              && (tdsrrc_csp_int_data.new_cell_acq_entry.freq == tdsrrc_csp_int_data.curr_acq_entry.freq))
    {
      /* Intra-frequency neighbor frequency is barred. Barring the frequency */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Intra-Freq Ngbr Cell Freq barred. Sndng Freq Barred to L1");
      *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
    }
    else
    {
      *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
    }
  }

  return(SUCCESS);
} /* tdsrrccsp_find_cell_bar_status() */

#ifdef FEATURE_DSAC
/*====================================================================================

FUNCTION          RRC_CHECK_IF_DSAC_IS_PRESENT

DESCRIPTION     This function is to check if DSAC info is present in SIB3 (or) not

DEPENDENCIES

RETURN VALUE
                Returns value TRUE   if DSAC information is present in SIB3
                Returns value FALSE value if DSAC information is not present in SIB3
SIDE EFFECTS
                  None
======================================================================================*/
boolean  tdsrrccsp_check_if_dsac_is_present(  tdsrrc_SysInfoType3   *sib3_ptr)
{
  if(sib3_ptr->m.v4b0NonCriticalExtensionsPresent)
  {
    if(sib3_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent )
    {
      if(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.m.v5c0NoncriticalExtensionPresent)
      {
        if(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
            v5c0NoncriticalExtension.m.v670NonCriticalExtensionPresent )
        {
          if(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
              v5c0NoncriticalExtension.v670NonCriticalExtension.
              sysInfoType3_v670ext.m.domainSpecificAccessRestrictionParametersForPLMNOfMIBPresent )
          {
            return TRUE;
          }
        }
      }
    }
  }
return FALSE;
}

/*===========================================================================

FUNCTION          RRC_CSP_FIND_DSAC_RESTRICTIONS

DESCRIPTION       This function is responsible for finding the status of
                  Access Control due to Domain Specific Access Class Restrictions.

DEPENDENCIES
                  None.

RETURN VALUE
                  None.

SIDE EFFECTS
                  None.
===========================================================================*/
void tdsrrccsp_find_dsac_restrictions
(
  tdsrrc_DomainSpecificAccessRestrictionParam_v670ext dsac_local,
  tdsrrc_dsac_info_type* rrc_dsac_info,
  tdsrrc_int_access_class_barred_list_type* rrc_int_access_class_barred_list,
  rrc_plmn_identity_type  plmn_id,
  tdsrrc_int_cell_id_type    cell_id
)
{

  tdsrrc_int_access_class_barred_list_type rrc_dsac_access_class_barred_list;

  /* Set DSAC Info present variable TRUE */
  rrc_dsac_info->valid = TRUE;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Finding the DSAC restrictions");

  /* If CS/PS Domain access class restriction is present */ 
  if((dsac_local.cSDomainSpecificAccessRestriction.t == 
    T_tdsrrc_DomainSpecificAccessRestriction_v670ext_restriction )
    && (dsac_local.pSDomainSpecificAccessRestriction.t == 
    T_tdsrrc_DomainSpecificAccessRestriction_v670ext_restriction))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: Both CS and PS domain restrictions are present ");

    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(dsac_local.cSDomainSpecificAccessRestriction.u.restriction,
        domainSpecficAccessClassBarredList)) &&
       (dsac_local.cSDomainSpecificAccessRestriction.u.restriction->domainSpecficAccessClassBarredList.n != 0))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: CS DSAC List is present");
      /* Get the CS Domain Specific Access Control Information */
      tdsrrc_translate_access_class_barred_list(&dsac_local.cSDomainSpecificAccessRestriction.u.restriction->domainSpecficAccessClassBarredList, 
                                             &rrc_dsac_access_class_barred_list);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: CS DSAC List is not present");
      rrc_dsac_access_class_barred_list = *rrc_int_access_class_barred_list;
    }

    /* Fill the CS domain specific access class information from access class barred list */ 
    tdsrrccsp_fill_access_class_info(rrc_dsac_access_class_barred_list,
                               &(rrc_dsac_info->cs_access_control),
                               plmn_id,
                               cell_id); 

    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(dsac_local.pSDomainSpecificAccessRestriction.u.restriction,
        domainSpecficAccessClassBarredList)) &&
       (dsac_local.pSDomainSpecificAccessRestriction.u.restriction->domainSpecficAccessClassBarredList.n != 0))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: PS DSAC List is present");
      /* Get the PS Domain Specific Access Control Information */      
      tdsrrc_translate_access_class_barred_list(&dsac_local.pSDomainSpecificAccessRestriction.u.restriction->domainSpecficAccessClassBarredList,
                                             &rrc_dsac_access_class_barred_list);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: PS DSAC List is not present");
      rrc_dsac_access_class_barred_list = *rrc_int_access_class_barred_list;
    }


    /* Fill the PS domain specific access class informattion from access class barred list */ 
    tdsrrccsp_fill_access_class_info(rrc_dsac_access_class_barred_list,
                               &(rrc_dsac_info->ps_access_control),
                               plmn_id,
                               cell_id); 
  }
  else if(dsac_local.cSDomainSpecificAccessRestriction.t == T_tdsrrc_DomainSpecificAccessRestriction_v670ext_restriction && 
    dsac_local.pSDomainSpecificAccessRestriction.t!= T_tdsrrc_DomainSpecificAccessRestriction_v670ext_restriction)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: CS Domain restriction is present but not PS");

    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(dsac_local.cSDomainSpecificAccessRestriction.u.restriction,
        domainSpecficAccessClassBarredList)) &&
       (dsac_local.cSDomainSpecificAccessRestriction.u.restriction->domainSpecficAccessClassBarredList.n != 0))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: CS DSAC List is present");
      /* Get the CS Domain Specific Access Control Information */
      tdsrrc_translate_access_class_barred_list(&dsac_local.cSDomainSpecificAccessRestriction.u.restriction->domainSpecficAccessClassBarredList, 
                                             &rrc_dsac_access_class_barred_list);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: CS DSAC List is not present");
      rrc_dsac_access_class_barred_list = *rrc_int_access_class_barred_list;
    }

    /* Fill the CS domain specific access class information from access class barred list */ 
    tdsrrccsp_fill_access_class_info(rrc_dsac_access_class_barred_list,
                               &(rrc_dsac_info->cs_access_control),
                               plmn_id,
                               cell_id);
    rrc_dsac_info->ps_access_control = TDSRRC_CSP_CELL_ACCESS_ALLOWED;
  }
  else if(dsac_local.cSDomainSpecificAccessRestriction.t !=
    T_tdsrrc_DomainSpecificAccessRestriction_v670ext_restriction 
    && dsac_local.pSDomainSpecificAccessRestriction.t == 
    T_tdsrrc_DomainSpecificAccessRestriction_v670ext_restriction)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: PS Domain restriction is present but not CS");

    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(dsac_local.pSDomainSpecificAccessRestriction.u.restriction,
        domainSpecficAccessClassBarredList)) &&
       (dsac_local.pSDomainSpecificAccessRestriction.u.restriction->domainSpecficAccessClassBarredList.n != 0))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: PS DSAC List is present");
      /* Get the PS Domain Specific Access Control Information */
      tdsrrc_translate_access_class_barred_list(&dsac_local.pSDomainSpecificAccessRestriction.u.restriction->domainSpecficAccessClassBarredList, 
                                             &rrc_dsac_access_class_barred_list);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: PS DSAC List is not present");
      rrc_dsac_access_class_barred_list = *rrc_int_access_class_barred_list;
    }

    /* Fill the PS domain specific access class information from access class barred list */ 
    tdsrrccsp_fill_access_class_info(rrc_dsac_access_class_barred_list,
                               &(rrc_dsac_info->ps_access_control),
                               plmn_id,
                               cell_id);
    rrc_dsac_info->cs_access_control = TDSRRC_CSP_CELL_ACCESS_ALLOWED;	
  }
  else  /*Both CS/PS Domain DSAC are not chosen hence no CS/PS restrictions apply */
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: No Domain restrictions are present for CS/PS");
    rrc_dsac_info->cs_access_control = TDSRRC_CSP_CELL_ACCESS_ALLOWED;
    rrc_dsac_info->ps_access_control = TDSRRC_CSP_CELL_ACCESS_ALLOWED;
  }
  if(tdsrrc_lo_access_class == RRC_LO_ACCESS_CLASS_ABSENT)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Low Access class Absent");
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Previous DSAC info CS: %d PS: %d",rrc_dsac_info->cs_access_control,rrc_dsac_info->ps_access_control);
    if(rrc_dsac_info->cs_access_control == TDSRRC_CSP_CELL_ACCESS_ALLOWED)
    {
      rrc_dsac_info->cs_access_control = TDSRRC_CSP_NO_REGULAR_CELL_ACCESS;
    }
    else if(rrc_dsac_info->cs_access_control == TDSRRC_CSP_REGULAR_CELL_ACCESS_ONLY)
    {
      rrc_dsac_info->cs_access_control = TDSRRC_CSP_NO_CELL_ACCESS;
    }
    if(rrc_dsac_info->ps_access_control == TDSRRC_CSP_CELL_ACCESS_ALLOWED)
    {
      rrc_dsac_info->ps_access_control = TDSRRC_CSP_NO_REGULAR_CELL_ACCESS;
    }
    else if(rrc_dsac_info->ps_access_control == TDSRRC_CSP_REGULAR_CELL_ACCESS_ONLY)
    {
      rrc_dsac_info->ps_access_control = TDSRRC_CSP_NO_CELL_ACCESS;
    }
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"After Updation DSAC info CS: %d PS: %d",rrc_dsac_info->cs_access_control,rrc_dsac_info->ps_access_control);
  }
}

/*====================================================================================

FUNCTION          RRC_CSP_FILL_DSAC_INFO_FOR_SHARED_PLMNS

DESCRIPTION     This function fills the DSAC info for shared PLMNs from SIB3

DEPENDENCIES

RETURN VALUE
                  None

SIDE EFFECTS
                  None
======================================================================================*/
void tdsrrccsp_fill_dsac_info_for_shared_plmns
(
  uint8 number_of_plmns,
  tdsrrc_SysInfoType3   *sib3_ptr,
  tdsrrc_dsac_info_for_shared_plmns_type* dsac_info_for_shared_plmns
)
{
  uint16 i = 0;

  for(i = 0; i < (TDSMAX_SHARED_PLMNS-1);i++)
  {
    dsac_info_for_shared_plmns[i].valid = FALSE;
  }

  if(sib3_ptr->m.v4b0NonCriticalExtensionsPresent)
  {
    if(sib3_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent )
    {
      if(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.m.v5c0NoncriticalExtensionPresent)
      {
        if(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
            v5c0NoncriticalExtension.m.v670NonCriticalExtensionPresent )
        {
          if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
             v5c0NoncriticalExtension.v670NonCriticalExtension.
             sysInfoType3_v670ext,domainSpecificAccessRestictionForSharedNetwork)) &&
             (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
             v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext,
             domainSpecificAccessRestictionForSharedNetwork)))
          {
            if(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
               v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
               domainSpecificAccessRestictionForSharedNetwork.t ==
               T_tdsrrc_DomainSpecificAccessRestrictionForSharedNetwork_v670ext_domainSpecificAccessRestictionList)
            {
              if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                 v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                 domainSpecificAccessRestictionForSharedNetwork.u.domainSpecificAccessRestictionList,
                 rrc,
                 domainSpecificAccessRestrictionParametersForOperator1)) )
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: DSAC info for operator 1 present in SIB3");
                dsac_info_for_shared_plmns[0].valid = TRUE;
                dsac_info_for_shared_plmns[0].dsac_info = sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                             v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                             domainSpecificAccessRestictionForSharedNetwork.u.domainSpecificAccessRestictionList->
                             domainSpecificAccessRestrictionParametersForOperator1;
               }
              if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                 v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                 domainSpecificAccessRestictionForSharedNetwork.u.domainSpecificAccessRestictionList,
                 rrc,
                 domainSpecificAccessRestrictionParametersForOperator2)) )
               {
                 TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: DSAC info for operator 2 present in SIB3");
                 dsac_info_for_shared_plmns[1].valid = TRUE;
                 dsac_info_for_shared_plmns[1].dsac_info = sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                             v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                             domainSpecificAccessRestictionForSharedNetwork.u.domainSpecificAccessRestictionList->
                             domainSpecificAccessRestrictionParametersForOperator2;
               }
              if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                 v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                 domainSpecificAccessRestictionForSharedNetwork.u.domainSpecificAccessRestictionList,
                 rrc,
                 domainSpecificAccessRestrictionParametersForOperator3)) )
               {
                 TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: DSAC info for operator 3 present in SIB3");
                 dsac_info_for_shared_plmns[2].valid = TRUE;
                 dsac_info_for_shared_plmns[2].dsac_info= sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                             v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                             domainSpecificAccessRestictionForSharedNetwork.u.domainSpecificAccessRestictionList->
                             domainSpecificAccessRestrictionParametersForOperator3;
               }
              if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                 v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                 domainSpecificAccessRestictionForSharedNetwork.u.domainSpecificAccessRestictionList,
                 rrc,
                 domainSpecificAccessRestrictionParametersForOperator4)) )
              {
                 TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: DSAC info for operator 4 present in SIB3");
                 dsac_info_for_shared_plmns[3].valid = TRUE;
                 dsac_info_for_shared_plmns[3].dsac_info= sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                             v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                             domainSpecificAccessRestictionForSharedNetwork.u.domainSpecificAccessRestictionList->
                             domainSpecificAccessRestrictionParametersForOperator4;
               }
              if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                 v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                 domainSpecificAccessRestictionForSharedNetwork.u.domainSpecificAccessRestictionList,
                 rrc,
                 domainSpecificAccessRestrictionParametersForOperator5)) )
               {
                 TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: DSAC info for operator 5 present in SIB3");
                 dsac_info_for_shared_plmns[4].valid = TRUE;
                 dsac_info_for_shared_plmns[4].dsac_info= sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                             v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                             domainSpecificAccessRestictionForSharedNetwork.u.domainSpecificAccessRestictionList->
                             domainSpecificAccessRestrictionParametersForOperator5;
               }
            }
            else if(sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                    v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                    domainSpecificAccessRestictionForSharedNetwork.t ==
                    T_tdsrrc_DomainSpecificAccessRestrictionForSharedNetwork_v670ext_domainSpecificAccessRestictionParametersForAll)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: DSAC info for all operators present in SIB3");
              for(i = 0; i < (number_of_plmns-1); i++)
              {
                dsac_info_for_shared_plmns[i].valid = TRUE;
                memscpy(&(dsac_info_for_shared_plmns[i].dsac_info), sizeof(tdsrrc_DomainSpecificAccessRestrictionParam_v670ext),sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
                            v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.
                            domainSpecificAccessRestictionForSharedNetwork.u.domainSpecificAccessRestictionParametersForAll,
                            sizeof(tdsrrc_DomainSpecificAccessRestrictionParam_v670ext));
              }
            }
          }
        }
      }
    }
  }
}

#endif /* FEATURE_DSAC */

/*===========================================================================

FUNCTION          RRC_TRANSLATE_ACCESS_CONTROL_TO_ACCESS_CLASS

DESCRIPTION     This function is responsible for conversion of internal cell access control 
                          from CSP to access class in conjunction with RRC<->MM interface

DEPENDENCIES

RETURN VALUE
                  Noe
SIDE EFFECTS
                  None
===========================================================================*/

static void tdsrrccsp_translate_access_control_to_access_class
(
  tdsrrc_csp_access_control_e_type access_control, 
  mm_as_cell_access_status_T*access_class
)
{
  switch(access_control)
  {
    case TDSRRC_CSP_CELL_ACCESS_ALLOWED:
      *access_class = MM_AS_ACCESS_ALL_CALLS;
      break;

    case TDSRRC_CSP_NO_REGULAR_CELL_ACCESS:
      *access_class = MM_AS_ACCESS_EMERGENCY_ONLY;
      break;

    case TDSRRC_CSP_NO_CELL_ACCESS:
      *access_class = MM_AS_ACCESS_NO_CALLS;
      break;

    case TDSRRC_CSP_REGULAR_CELL_ACCESS_ONLY:
      *access_class = MM_AS_ACCESS_NORMAL_ONLY;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid access control received %d",access_control);
      *access_class = MM_AS_ACCESS_ALL_CALLS;
      break;
  }
}

/*===========================================================================

FUNCTION          RRC_CSP_FIND_ACCESS_CONTROL

DESCRIPTION       This function is responsible for finding the status of
                  Access Control due to Access Class Restrictions.

                  Note that this function doesn't care about Cell Barred status
                  OR Cell Reservations.

DEPENDENCIES
                  - SIB procedure should have SIB 3 available for this cell.
                  - The UE Access Class should be known.

RETURN VALUE
                  SUCCESS: Cell Access restrictions for this cell could be 
                           calculated.
                  FAILURE: Failure to find out Cell Access restrictions due 
                           to multiple reasons.
SIDE EFFECTS
                  None.
===========================================================================*/
uecomdef_status_e_type  
tdsrrccsp_find_access_control(tdsrrc_csp_access_control_e_type  *access_control, tdsrrc_dsac_info_type *rrc_dsac_info,
                            rrc_plmn_identity_type  plmn_id,
                            tdsrrc_int_cell_id_type    cell_id
                           ,rrc_plmn_identity_type  common_plmn_id
                            )
{
  uint16  i;
#ifdef FEATURE_DSAC
  boolean look_at_shared_list = FALSE;
  tdsrrc_MasterInformationBlock *mib_ptr;
  rrc_plmn_identity_type mib_PLMN_ID;
  uint8 num_of_shared_plmns;
  rrc_plmn_identity_type shared_plmn_list[TDSMAX_SHARED_PLMNS];
#endif
  tdsrrc_SysInfoType3   *sib3_ptr;
  tdsrrc_int_access_class_barred_list_type   rrc_int_access_class_barred_list;
  tdsrrc_SysInfoType4 *sib4_ptr;
  tdsrrc_CellAccessRestriction *sib3_4_access_restriction_ptr;
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  rrc_plmn_identity_type sibidx_plmn_id;
  
  *access_control = TDSRRC_CSP_NO_CELL_ACCESS;
  rrc_dsac_info->valid = FALSE;

  sibidx_plmn_id = common_plmn_id;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB Index PLMN is MCC%d - MNC%d",TDSRRC_CSP_GET_INT_MCC(sibidx_plmn_id),TDSRRC_CSP_GET_INT_MNC(sibidx_plmn_id));

#ifdef FEATURE_DSAC
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: is_curr_dsac_info_valid value is %d ",tdsrrc_csp_int_data.is_curr_dsac_info_valid);
  tdsrrc_csp_int_data.is_prev_dsac_info_valid = tdsrrc_csp_int_data.is_curr_dsac_info_valid;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: curr dsac info valid is set to false");
  tdsrrc_csp_int_data.is_curr_dsac_info_valid = FALSE;
#endif 

  if((tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN)
     && (!tdsrrccsp_is_dummy_plmn_search(tdsrrc_csp_int_data.curr_req_plmn.plmn_id))
    )
  {
    /* PLMN and Cell ID of the current camped on cell are known. Get SIB3 */
    sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrc_sibdb_return_sib_for_cell(
                              tdsrrc_SIB3, 
                              sibidx_plmn_id, cell_id);
    if( sib3_ptr == NULL )
    {
      /* SIB3 for this PLMN, Cell ID combination not present 
       * This should NOT happen 
       */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"ERR: SIB3 for MCC %d, MNC %d, Cell ID %d not present", 
            TDSRRC_CSP_GET_INT_MCC(sibidx_plmn_id), TDSRRC_CSP_GET_INT_MNC(sibidx_plmn_id), cell_id);
      return(FAILURE);
    }

    if((rrc_state != TDSRRC_STATE_DISCONNECTED) && (rrc_state != TDSRRC_STATE_CONNECTING))
    {
      if(sib3_ptr->sib4indicator == TRUE)
      {
        /* Read SIB4. If it's not there mark and error and continue with SIB3 */
        sib4_ptr = (tdsrrc_SysInfoType4 *) tdsrrc_sibdb_return_sib_for_cell(
                                  tdsrrc_SIB4, 
                                  sibidx_plmn_id, cell_id);
        if( sib4_ptr == NULL )
        {
          /* SIB4 not present */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB4 indicated but not present,using SIB3");
          sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
        }
        else
        {
          sib3_4_access_restriction_ptr = &(sib4_ptr->cellAccessRestriction);
        }
      }
      else
      {
        sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
      }
    }
    else
    {
      sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
    }
  }
  /* TD2TD BPLMN search is in progress */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: Reading SIB3 Info");
    /* Get SIB3 info */
    sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrcsib_bplmn_return_sib_for_cell(tdsrrc_SIB3);

    if( sib3_ptr == NULL )
    {
      /* SIB3 is NULL This should NOT happen */
      TDSRRC_MSG0(MSG_LEGACY_FATAL,"SIB3 not read but sib_cnf returned w SUCCESS");
	  return(FAILURE);
    }
    sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
  }

  /* We ARE camped on a cell and VALID SIBS for that cell exist - proceed further 
   * Check if UE has all access, only emergency access or No access 
   */
#ifdef FEATURE_DSAC
  for (i=0; i<(uint16)TDSRRC_INT_ACCESS_CLASS_MAX_INDEX; i++)
  {
    rrc_int_access_class_barred_list.barred[i] = FALSE;
  }
#endif

  /* First check if the Access Class Barred List is present or not */
  if(!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib3_4_access_restriction_ptr,
      accessClassBarredList))) 
  {
    /* Access Class Barred List is NOT present */
    /* If UE has a Valid Low Access Class, all calls are allowed.
     */
    TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: Access Class Barred List Not present ");
    if(tdsrrc_lo_access_class != RRC_LO_ACCESS_CLASS_ABSENT)
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"Cell %d Access: All calls",cell_id);
      TDSRRC_MSG1(MSG_LEGACY_LOW,"AC bar list absent,valid UE Low AC %d",tdsrrc_lo_access_class); 
      *access_control = TDSRRC_CSP_CELL_ACCESS_ALLOWED;
    }
    else /* UE does not have a valid Low Access Class Only Emergency calls are allowed */
    {
     /* NOTE: There is no need to check if UE has a Hi Access Class if UE's Low
      * Access Class is absent. All UEs are supposed to have a Low Access Class 
      * An absence of UE Low Access Class may actually be a "No SIM card" case.
      */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Cell %d Access: Emergency calls",cell_id);
      TDSRRC_MSG0(MSG_LEGACY_LOW,"AC bar list absent,No UE Low AC");
                                                        
      *access_control = TDSRRC_CSP_NO_REGULAR_CELL_ACCESS;
    }
  } /* If Access Class barred List is NOT present */ 
  else  /* Access Class barred list is present */
  {
    /* First store the access class into a local data structure */
    tdsrrc_translate_access_class_barred_list(&sib3_4_access_restriction_ptr->accessClassBarredList,
                                           &rrc_int_access_class_barred_list);
    tdsrrccsp_fill_access_class_info(rrc_int_access_class_barred_list,
                               access_control,
                               plmn_id,
                               cell_id);
  }

#ifdef FEATURE_DSAC

  if((tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN)
     && (!tdsrrccsp_is_dummy_plmn_search(tdsrrc_csp_int_data.curr_req_plmn.plmn_id))
    )
  {
    mib_ptr = (tdsrrc_MasterInformationBlock *) tdsrrc_sibdb_return_sib_for_cell(
                                                                          tdsrrc_MIB,
                                                                          sibidx_plmn_id,
                                                                          cell_id
                                                                         );
  }
  else
  {
    mib_ptr = (tdsrrc_MasterInformationBlock *) tdsrrcsib_bplmn_return_sib_for_cell(
                                                                           tdsrrc_MIB);
  }

  if( mib_ptr == NULL )
  {
    /* SIB3 is NULL This should NOT happen */
    TDSRRC_MSG0(MSG_LEGACY_FATAL,"MIB not read but sib_cnf returned w SUCCESS");
	return(FAILURE);
  }

  if(tdsrrc_is_nw_shared(mib_ptr))
  {
    tdsrrc_translate_gsm_map_plmn_id(&(mib_ptr->plmn_Type.u.gsm_MAP->plmn_Identity),&mib_PLMN_ID);
    if(!tdsrrccsp_is_mib_plmn_valid(mib_ptr) ||
       !tdsrrc_bcch_plmns_match(mib_PLMN_ID,plmn_id))
    {
      look_at_shared_list = TRUE;
    }
  }

  if(!look_at_shared_list && tdsrrccsp_check_if_dsac_is_present(sib3_ptr) == TRUE)
  {
    /* First store the access class pointer into a local data structure */
    tdsrrc_DomainSpecificAccessRestrictionParam_v670ext dsac_local = sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v5c0NoncriticalExtension.v670NonCriticalExtension.sysInfoType3_v670ext.domainSpecificAccessRestrictionParametersForPLMNOfMIB;

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC:DSAC Info is present in SIB 3");
    tdsrrc_csp_int_data.is_curr_dsac_info_valid = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"is_curr_dsac_info_valid is set to TRUE");

    tdsrrccsp_find_dsac_restrictions(dsac_local,
                                   rrc_dsac_info,
                                   &rrc_int_access_class_barred_list,
                                   plmn_id,
                                   cell_id
                                   );
  }

  if(look_at_shared_list)
  {
    uint16 k = 0;
    tdsrrc_dsac_info_for_shared_plmns_type dsac_info_for_shared_plmns[TDSMAX_SHARED_PLMNS-1];
    tdsrrc_DomainSpecificAccessRestrictionParam_v670ext dsac_local;
    num_of_shared_plmns = tdsrrccsp_construct_multiple_plmn_list_from_mib(mib_ptr,shared_plmn_list);
    if (num_of_shared_plmns >= TDSMAX_SHARED_PLMNS)
    {
      num_of_shared_plmns = TDSMAX_SHARED_PLMNS-1;
    }

    tdsrrccsp_fill_dsac_info_for_shared_plmns(num_of_shared_plmns,
                                            sib3_ptr,
                                            dsac_info_for_shared_plmns);

    if(tdsrrccsp_is_mib_plmn_valid(mib_ptr))
    {
      /* Because Mib PLMN will be the first one among the shared PLMN list */
      k++;
    }

    for(i = 0;((i < TDSMAX_SHARED_PLMNS-1) && (i <= num_of_shared_plmns) && (k < TDSMAX_SHARED_PLMNS)); i++,k++)
    {
      dsac_info_for_shared_plmns[i].plmn_id = shared_plmn_list[k];
    }

    /* By now dsac_info_for_shared_plmns contains all the shared PLMNs and their dsac restrictions */
    for(i = 0;(i < TDSMAX_SHARED_PLMNS-1) && (i < num_of_shared_plmns); i++)
    {
      if(tdsrrc_bcch_plmns_match(plmn_id, dsac_info_for_shared_plmns[i].plmn_id))
      {
        /* We are sure that this is not a mib common PLMN */
        break;
      }
    }

    if((i == num_of_shared_plmns) || (i == TDSMAX_SHARED_PLMNS-1))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"NWS: Choosen PLMN not present in the MIB of this cell");
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Setting DSAC info valid to FALSE");
      tdsrrc_csp_int_data.is_curr_dsac_info_valid = FALSE;
    }
    else if(dsac_info_for_shared_plmns[i].valid == TRUE)
    {
      dsac_local = dsac_info_for_shared_plmns[i].dsac_info;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:DSAC Info for chosen PLMN present in SIB 3");
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Setting DSAC info valid to TRUE");
      tdsrrc_csp_int_data.is_curr_dsac_info_valid = TRUE;
      tdsrrccsp_find_dsac_restrictions(dsac_local,
                                     rrc_dsac_info,
                                     &rrc_int_access_class_barred_list,
                                     plmn_id,
                                     cell_id
                                     );
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS:DSAC Info for chosen PLMN not present in SIB 3");
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Setting DSAC info valid to FALSE");
      tdsrrc_csp_int_data.is_curr_dsac_info_valid = FALSE;
    }
  }

  if(tdsrrc_csp_int_data.is_curr_dsac_info_valid == TRUE)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: CS cell access: %d, PS cell access: %d",rrc_dsac_info->cs_access_control
                                                           ,rrc_dsac_info->ps_access_control
                                                           ,0);
  }
#endif /* FEATURE_DSAC */
  return (SUCCESS);
} /* tdsrrccsp_find_access_control */

/*===========================================================================

FUNCTION          RRC_FILL_ACCESS_CLASS_INFO

DESCRIPTION     This function is responsible for filling CS/PS Domain Specific Access Class information from access 
                          class barred list pointer
                          
DEPENDENCIES

RETURN VALUE
                  None
SIDE EFFECTS
                  None
===========================================================================*/
void tdsrrccsp_fill_access_class_info(tdsrrc_int_access_class_barred_list_type rrc_int_access_class_barred_list, 
	                                                   tdsrrc_csp_access_control_e_type *access_control,
	                                                   rrc_plmn_identity_type  plmn_id,
	                                                   tdsrrc_int_cell_id_type    cell_id)
{  

    /* For UEs with access classes 11 to 15, Emergency calls are not allowed if
     * both "Access Class 10" and relevant Access Class (11 to 15) are barred.
     * Otherwise Emergency Calls are allowed - 25.304 (section 5.3.1.3)
     */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UE's High Class = %d",tdsrrc_hi_access_class);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UE's Low Class = %d",tdsrrc_lo_access_class);

    if(rrc_int_access_class_barred_list.barred[TDSRRC_INT_ACCESS_CLASS_10_INDEX]
            == TRUE)
    {
      if ( (tdsrrc_hi_access_class != RRC_HI_ACCESS_CLASS_ABSENT) && 
           (tdsrrccsp_verify_hi_ac_access(plmn_id, 
                                          rrc_int_access_class_barred_list)
           == TRUE) )
      {
        /* Since UE's Hi Access Class is not barred and allows access to
         * regular calls we have full access
         */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell %d Access: All calls",cell_id);
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"AC10 barred,UE Hi AC 0x%x has access",tdsrrc_hi_access_class);
          *access_control = TDSRRC_CSP_CELL_ACCESS_ALLOWED;
      }
      else
      {
        /* Since UE's High Access Class is barred or UE has no Hi Access
         * Class Emergency calls are NOT permitted
         */
        /* Check if UE has regular access - Check if UE lo Access Class is NOT barred */
        if ((tdsrrc_lo_access_class != RRC_LO_ACCESS_CLASS_ABSENT) &&
            (rrc_int_access_class_barred_list.barred[tdsrrc_lo_access_class]
                  == FALSE))
        {
          /* The cell is available for regular access but not for emergency calls
           * since the UE Low Access Class is NOT barred.
           */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell %d Access: Regular calls only",cell_id);
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"UE Low AC %d not barred",tdsrrc_lo_access_class);
          *access_control = TDSRRC_CSP_REGULAR_CELL_ACCESS_ONLY;
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell %d Access: No Calls",cell_id);
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"AC10 barred,UE Lo AC 0x%x barred | No Hi AC | No SIM",tdsrrc_lo_access_class);
          *access_control = TDSRRC_CSP_NO_CELL_ACCESS;
        }
      }
    } /* If "Access Class 10" bit is set */

    /* Check if UE has full access - Check if UE lo Access Class is NOT barred */
    else if ((tdsrrc_lo_access_class != RRC_LO_ACCESS_CLASS_ABSENT) &&
        (rrc_int_access_class_barred_list.barred[tdsrrc_lo_access_class]
              == FALSE))
    {
      /* The cell is available for full Access since the UE Low Access Class
       * is NOT barred. Since Access Class 0-9 are not barred
       * in Sys Info message and "Access Class 10" Bit is not set, Regular and 
       * emergency calls will be allowed.
       */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell %d Access: All calls",cell_id);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UE Low AC %d not barred",tdsrrc_lo_access_class);
      *access_control = TDSRRC_CSP_CELL_ACCESS_ALLOWED;
    }

    /* Check if UE has full access - Check if Access to one of the UE Hi Access 
     * Classes is permitted */
    else if ((tdsrrc_hi_access_class != RRC_HI_ACCESS_CLASS_ABSENT) &&
             (tdsrrccsp_verify_hi_ac_access(plmn_id, 
                                          rrc_int_access_class_barred_list)
             == TRUE ))
    {
      /* Since Access Class 11-15 are not barred in Sys Info message and
       * "Access Class 10" Bit is Not set, regular and emergency calls
       * are allowed.
       */

      /* We are in Home PLMN and UE Access Class is 11 to 15 - Full Access
       * is allowed.
       */
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell %d Access: all calls",cell_id);
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"UE Hi AC 0x%x has access",tdsrrc_hi_access_class);
       *access_control = TDSRRC_CSP_CELL_ACCESS_ALLOWED;
    }

    else
    { 
      /* Access to emergency calls IS permitted since "Access Class 10" bit is 
       * NOT set to rrc_barred
       */
      /* The current camped on Cell is restricted for Regular Call Access */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell %d Access:  Emergency calls",cell_id);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"AC10 not barred UE Hi and Lo AC: no Access"); 

      *access_control = TDSRRC_CSP_NO_REGULAR_CELL_ACCESS;
    } /* If "Access Class 10" bit is NOT set */
  return;
} /* tdsrrccsp_find_access_control */


/*===========================================================================

FUNCTION          RRC_CSP_FIND_RRC_CELL_ACCESS

DESCRIPTION       This function is responsible for finding the cell access
                  status. The cell access status is based on the cell 
                  restriction and access control.

DEPENDENCIES

RETURN VALUE
                  Cell Access status as understood by modules outside CSP.
SIDE EFFECTS
                  None
===========================================================================*/
static mm_as_cell_access_status_T tdsrrccsp_find_rrc_cell_access
(
 rrc_service_info_type *service_info_ptr,
 tdsrrc_dsac_info_type *rrc_dsac_info,           /* DSAC Specific Info*/
 tdsrrc_csp_access_control_e_type access_control,/* Access Control*/
 tdsrrc_csp_acq_status_type  acq_status_mask    /* Acquisition status mask */
)   
{
  mm_as_cell_access_status_T      cell_access_status;
  /* Initialise cell_access_status to supress Lint warning*/
  cell_access_status = MM_AS_ACCESS_NORMAL_ONLY;

  if(acq_status_mask & TDSRRC_CSP_IDLE_CELL_BAR_BIT)
  {
    /* Cell is barred as given by Cell status and cell reservations.
     * Check the cell access as given by Access Control.
     */
#ifndef FEATURE_DSAC
      if(access_control == TDSRRC_CSP_NO_CELL_ACCESS)
      {
        /* Cell is barred AND Access to emergency calls NOT allowed */
        service_info_ptr->cell_access = MM_AS_ACCESS_NO_CALLS;
      }
      else
      {
        /* Cell is barred AND Access to emergency calls are allowed */
        service_info_ptr->cell_access = MM_AS_ACCESS_EMERGENCY_ONLY;
      }
#else
    if(rrc_dsac_info->valid != TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: DSAC Info is not present ");
      if(access_control == TDSRRC_CSP_NO_CELL_ACCESS)
      {
        /* Cell is barred AND Access to emergency calls NOT allowed */
        service_info_ptr->cell_access = MM_AS_ACCESS_NO_CALLS;
      }
      else
      {
        /* Cell is barred AND Access to emergency calls are allowed */
        service_info_ptr->cell_access = MM_AS_ACCESS_EMERGENCY_ONLY;
      }
    }    
    else
    {
      /* Fill the cs access control information */
      if(rrc_dsac_info->cs_access_control== TDSRRC_CSP_NO_CELL_ACCESS)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: CS Cell Access,No calls allowed ");
        /* Cell is barred AND Access to emergency calls NOT allowed */
        service_info_ptr->cs_cell_access= MM_AS_ACCESS_NO_CALLS;
      }
      else
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: CS Cell Access,Emergency Calls only");
        /* Cell is barred AND Access to emergency calls are allowed */
        service_info_ptr->cs_cell_access = MM_AS_ACCESS_EMERGENCY_ONLY;
      }
      /* Fill the ps access control information */
      if(rrc_dsac_info->ps_access_control== TDSRRC_CSP_NO_CELL_ACCESS)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: PS Cell Access,No calls allowed ");
        /* Cell is barred AND Access to emergency calls NOT allowed */
        service_info_ptr->ps_cell_access= MM_AS_ACCESS_NO_CALLS;
      }
      else
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: PS Cell Access,Emergency Calls only ");
        /* Cell is barred AND Access to emergency calls are allowed */
        service_info_ptr->ps_cell_access= MM_AS_ACCESS_EMERGENCY_ONLY;
      }
    }
#endif    
  }
  else
  {

#ifndef FEATURE_DSAC
    tdsrrccsp_translate_access_control_to_access_class(access_control, 
                        &(service_info_ptr->cell_access));
#else
    /* Cell is not barred as given by Cell status and cell reservations.
     * Check the cell access as given by Access Control.
     */
    if(rrc_dsac_info->valid != TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"DSAC: DSAC Info is not present ");
      tdsrrccsp_translate_access_control_to_access_class(access_control, 
                      &(service_info_ptr->cell_access));
    }
    else
    {
      /* Fill the cs access control information */
      tdsrrccsp_translate_access_control_to_access_class(rrc_dsac_info->cs_access_control,
                       &(service_info_ptr->cs_cell_access));
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: CS Cell Access is  = %d ",service_info_ptr->cs_cell_access);

      /* Fill the ps access control information */
      tdsrrccsp_translate_access_control_to_access_class(rrc_dsac_info->ps_access_control, 
                       &(service_info_ptr->ps_cell_access));    
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: PS Cell Access is = %d ",service_info_ptr->ps_cell_access);	

    }

 #endif
  } /* Cell not barred as given by Cell status */
  return(cell_access_status);
} /* tdsrrccsp_find_rrc_cell_access */


/*===========================================================================

FUNCTION          RRC_CSP_FIND_FORBID_LAI

DESCRIPTION       This function is responsible for finding the status of
                  the passed LAI. It checks the list of forbidden LAIs
                  to determine the status.

DEPENDENCIES
                  - The Forbidden LAI list in RRC CSP Internal data 
                    should have been initialized.

RETURN VALUE
                  SUCCESS: If LAI status could be successfully evaluated.
                  FAILURE: If LAI status could not be evaluated.
SIDE EFFECTS
                  None
===========================================================================*/
static uecomdef_status_e_type
tdsrrccsp_find_forbid_lai(rrc_lai_type  *lai, 
                        tdsrrc_csp_lai_status_e_type *lai_status)
{
  uint32 i;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type curr_sub_id;
#endif
  *lai_status = TDSRRC_CSP_LAI_OK;
  
  /* If LAI is to be checked for forbidden status, compare given LAI with
   * ones in forbidden LAI list in RRC CSP Internal Data 
   */
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  curr_sub_id = tdsrrc_get_as_id();
    
  if((curr_sub_id < SYS_MODEM_AS_ID_1) || (curr_sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC: invalid sub_id %d", curr_sub_id);
    return SUCCESS;
  }

  for( i = 0; i < tdsrrc_csp_int_data.forbid_lai_list[curr_sub_id].num_lais; i++ )
  {
    if( rrc_csp_bcch_and_nas_lais_match(*lai, tdsrrc_csp_int_data.forbid_lai_list[curr_sub_id].lai[i]) )
    {
      if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_CELL_RESELECTION_SCAN) ||
         (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_CC_ORDER_SCAN))
      {
        tdsrrc_csp_int_data.forbidden_lai = tdsrrc_csp_int_data.forbid_lai_list[curr_sub_id].lai[i];
      }

      *lai_status = TDSRRC_CSP_FORBID_LAI;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LAI is forbidden");
      break;
    }
  }
#else
  for( i = 0; i < tdsrrc_csp_int_data.forbid_lai_list.num_lais; i++ )
  {
    if( rrc_csp_bcch_and_nas_lais_match(*lai, tdsrrc_csp_int_data.forbid_lai_list.lai[i]) )
    {
      if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_CELL_RESELECTION_SCAN) ||
        (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_CC_ORDER_SCAN))
      {
        tdsrrc_csp_int_data.forbidden_lai = tdsrrc_csp_int_data.forbid_lai_list.lai[i];
      }

      *lai_status = TDSRRC_CSP_FORBID_LAI;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LAI is forbidden");
      break;
    }
  }
#endif

  return(SUCCESS);
} /* tdsrrccsp_find_forbid_lai */

/*===========================================================================

FUNCTION          RRC_CSP_CHECK_FOR_LAI_FORBIDDEN

DESCRIPTION       This function is responsible for finding the status of
                  the passed LAI. It checks the list of forbidden LAIs
                  to determine the status.

DEPENDENCIES
                  - The Forbidden LAI list in RRC CSP Internal data 
                    should have been initialized.

RETURN VALUE
                  SUCCESS: If LAI status could be successfully evaluated.
                  FAILURE: If LAI status could not be evaluated.
SIDE EFFECTS
                  None
===========================================================================*/

boolean tdsrrccsp_check_for_lai_forbidden(tdsrrc_SysInfoType1 *sib1_ptr,
                                               tdsrrc_MasterInformationBlock *mib_ptr)
{
  tdsrrc_csp_lai_status_e_type  lai_status;
  rrc_lai_type  lai;
  uint32 i;
  rrc_plmn_identity_type shared_plmn_list[TDSMAX_SHARED_PLMNS];
  uint8 shared_plmn_idx = 0;
  uint8  num_of_shared_plmns = 0;
  boolean nw_shared = FALSE;  
  unsigned char  *data_ptr;
  rrc_gsm_map_nas_sys_info_type   nas_common_info;
  rrc_plmn_identity_type mib_plmn;
  
  rrc_plmn_identity_type mib_plmn_id;
  tdsrrc_translate_gsm_map_plmn_id(&(mib_ptr->plmn_Type.u.gsm_MAP->plmn_Identity),&mib_plmn);
  if(tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC)
  {
    nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;
    data_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
    for( i = 0; i <sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i++)
    {
      nas_common_info.value[i] =  *data_ptr;
      data_ptr ++;
    }
    if(mm_extract_lac_from_sib_info(&lai, &nas_common_info) == FALSE)
    {
      ERR_FATAL("Could not extract LAI from NAS Common sys info",0,0,0);
    }
    lai.plmn = mib_plmn;
    if(tdsrrccsp_find_forbid_lai(&lai, &lai_status) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure finding LAI status");
      return FALSE;
    }
    if(lai_status != TDSRRC_CSP_FORBID_LAI)
    {
      return FALSE;
    }
    if(tdsrrc_is_nw_shared(mib_ptr))
    {
      nw_shared = TRUE;
      num_of_shared_plmns = tdsrrccsp_construct_multiple_plmn_list_from_mib(mib_ptr,shared_plmn_list);
    }
    do
    {
      if(nw_shared)
      {
        mib_plmn_id = shared_plmn_list[shared_plmn_idx++];
      }
      else
      {
        mib_plmn_id=mib_plmn;
      }
      lai.plmn = mib_plmn_id;
      if(tdsrrccsp_find_forbid_lai(&lai, &lai_status)  == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure finding LAI status");
        return FALSE;
      }
      if(lai_status != TDSRRC_CSP_FORBID_LAI)
      {
        return FALSE;
      }
    }while(shared_plmn_idx < num_of_shared_plmns);

    if(lai_status == TDSRRC_CSP_FORBID_LAI)
    {
      tdsrrc_csp_int_data.bar_for_resel_only = TRUE;
      /* Send Cell bar request to L1 */
      tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_FREQ_BARRED, 
                                       TDSFORBIDDEN_RESEL_FREQ_BAR_TIMER_IN_SEC);
      tdsrrc_csp_int_data.bar_for_resel_only = FALSE;
	  tdsrrc_csp_int_data.tds_cell_time_to_reselect = TDSFORBIDDEN_RESEL_FREQ_BAR_TIMER_IN_SEC;
      if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)
      {
        tdsrrc_csp_int_data.plmn_fnd_during_fgnd_svc_srch = TRUE;
      }
      return TRUE;
    }  
  }
  return FALSE;
}
/*===========================================================================

FUNCTION          TDSRRCCSP_STORE_PSEUDO_FR_INFO

DESCRIPTION       Store pseudo fast return info that configured in SIB3 
                  into tdsrrc_csp_int_data.
                  Pseudo FR solution uses MappingInfo IE in CellSelectReselectInfo
                  structure, and redefines the parameters in that IE.
                  Detailly, the parameters are interpreted as
                  1) Function type  - This parameter provides frequency  range£º
                                      Enumerated (Frequency Range1, Frequency Range2, 
                                      Frequency Range3,RSCP threshold)
                  2) Map_parameter_1- a)	If function type=" RSCP threshold"£¬this is 
                                          the 'RSCP Threshold' in the range of [0, 90];
                                          Otherwise, ignore.
                                      b)	If function type=" Frequency Range1'£¬this is 
                                          the frequency mapping is the range of [0, 99]. 
                                      c)	If function type=" Frequency Range2'£¬this is 
                                          the frequency mapping is the range of [0, 99]. 
                                      d)	If function type=" Frequency Range3'£¬this is 
                                          the frequency mapping is the range of [0, 75]. 
                                          Ignore if out of the range.
                                      e)	If function type has other values, ignore this 
                                          parameter.
                  3) Map_parameter_2- Same interpretation as Map_parameter_1
                  4) Upper_limit    - a)	If function type=" Frequency Range1'£¬this is 
                                          the frequency mapping is the range of [1, 91]. 
                                      b)	If function type=" Frequency Range2'£¬this is 
                                          the frequency mapping is the range of [1, 91]. 
                                      c)	If function type=" Frequency Range3'£¬this is 
                                          the frequency mapping is the range of [1, 75]. 
                                          Ignore if out of the range.
                                      d)	If function type has other values, ignore this 
                                          parameter.
                  Note£º
                  The frequency range defined in 3GPP TDD LCR parameter is [0, 16383]. 
                  In order to use 'Mapping Info' for specifying TDS frequency range, the 
                  following mapping is used:
 	                Frequency range1 1880MHz-1900 MHz: range [9400, 9499] mapped to [0, 99].
 	                Frequency range2 1900MHz-1920 MHz: range [9500, 9599] mapped to [0, 99]. 
 	                Frequency range3 2010MHz-2025 MHz: range [10050, 10125] mapped to [0, 75]. 
                  RSCP threshold: Primary CCPCH RSCP is [-115..-25], mapped to [0, 90]


DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      The old PFR info is overwritten by new one.
===========================================================================*/
static void tdsrrccsp_store_pseudo_fr_info(tdsrrc_SysInfoType3 *sib3_ptr)
{
  uint8                   i, n;
  /* The total number of frequency */
  uint8                   freq_num = 0; 
  uint16                  freq_list[TDSRRC_CSP_MAX_FPR_FREQ];
  /* the start frequency of different range, 9400 for range1, 9500 for range2, 10050 for range3 */
  uint16                  freq_base = TDSRRC_PFR_FREQUENCY_RANGE1_BASE; 
  /* whether RSCP_threshold is included */
  boolean                 rscp_threshold_present = FALSE;
  /* whether the primary frequency of the serving cell is included */
  boolean                 primary_freq_present = FALSE;
  boolean                 duplicate_freq = FALSE;
  tdsrrc_MappingFunctionParameterList   * sib_3_mapping_ptr = NULL;
  

  if(sib3_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PFR: sib3_ptr is NULL");
    return;
  }
 
  /* As per CMCC's requirement, if SIB3 contains MappingInfo of "UTRA TDD 3.84Mcps", it means network
   * turns on Pseudo Fast Return (PFR) feature in that cell.
   * Note: PFR is not contained in SIB4, so always check SIB3 even SIB4 is present
   */
  if(sib3_ptr->cellSelectReselectInfo.m.mappingInfoPresent)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"PFR SIB3 mappingInfo %d elems",sib3_ptr->cellSelectReselectInfo.mappingInfo.n);
    
    for(i = 0; i < sib3_ptr->cellSelectReselectInfo.mappingInfo.n; i++)
    {
      if(sib3_ptr->cellSelectReselectInfo.mappingInfo.elem[i].rat == tdsrrc_RAT_utra_TDD)
      {
        sib_3_mapping_ptr = &(sib3_ptr->cellSelectReselectInfo.mappingInfo.elem[i].mappingFunctionParameterList);

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"PFR SIB3 TDD mapping %d elems",sib_3_mapping_ptr->n);
        
        for(n = 0; n < sib_3_mapping_ptr->n; n++)
        {
          if(sib_3_mapping_ptr->elem[n].functionType == tdsrrc_MappingFunctionType_functionType4) /* RSCP threshold */
          {
            /* set this RSCP threshold only if it's valid and previous RSCP threhold is not set or larger than this one */
            if((sib_3_mapping_ptr->elem[n].m.mapParameter1Present) &&
               (sib_3_mapping_ptr->elem[n].mapParameter1 <= 90) &&
               (!((rscp_threshold_present) && (tdsrrc_csp_int_data.pseudo_fr_info.rscp_threshold <= sib_3_mapping_ptr->elem[n].mapParameter1))))
            {
              tdsrrc_csp_int_data.pseudo_fr_info.rscp_threshold = sib_3_mapping_ptr->elem[n].mapParameter1;
              rscp_threshold_present = TRUE;
            }
            /* The IE Map_parameter_1 is MD with default value 0 */
            else if(!(sib_3_mapping_ptr->elem[n].m.mapParameter1Present))
            {
              tdsrrc_csp_int_data.pseudo_fr_info.rscp_threshold = 0;
              rscp_threshold_present = TRUE;
            }

            TDSRRC_MSG1(MSG_LEGACY_HIGH,"PFR SIB3 rscp_threshold_present %d elems",rscp_threshold_present);
          }
          else
          {
            /* Check if maximum frequencies reached */
            if(freq_num >= TDSRRC_CSP_MAX_FPR_FREQ)
            {
              continue;
            }
            
            if(sib_3_mapping_ptr->elem[n].functionType == tdsrrc_MappingFunctionType_linear) /* frequency range1 */
            {
              /* Range [9400, 9499]: 1880MHz-1900MHz */
              freq_base = TDSRRC_PFR_FREQUENCY_RANGE1_BASE;
            }
            else if(sib_3_mapping_ptr->elem[n].functionType == tdsrrc_MappingFunctionType_functionType2) /* frequency range2 */
            {
              /* Range [9500, 9599]: 1900MHz-1920MHz */
              freq_base = TDSRRC_PFR_FREQUENCY_RANGE2_BASE;
            }
            else if(sib_3_mapping_ptr->elem[n].functionType == tdsrrc_MappingFunctionType_functionType3) /* frequency range3 */
            {
              /* Range [10050, 10125]: 2010MHz-2025MHz */
              freq_base = TDSRRC_PFR_FREQUENCY_RANGE3_BASE;
            }
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unexpected functionType %d",sib_3_mapping_ptr->elem[n].functionType);
              continue;
            }

            /* Get frequency from Map_parameter_1 */
            if(sib_3_mapping_ptr->elem[n].m.mapParameter1Present)
            {
              freq_list[freq_num++] = freq_base + sib_3_mapping_ptr->elem[n].mapParameter1;
            }
            else
            {
              /* The IE Map_parameter_1 is MD with default value 0 */
              freq_list[freq_num++] = freq_base; 
            }
            
            if(freq_num >= TDSRRC_CSP_MAX_FPR_FREQ)
            {
              continue;
            }
            /* Get frequency from Map_parameter_2 */
            freq_list[freq_num++] = freq_base + sib_3_mapping_ptr->elem[n].mapParameter2;

            if(freq_num >= TDSRRC_CSP_MAX_FPR_FREQ)
            {
              continue;
            }
            /* Get frequency from Upper_limit */
            if(sib_3_mapping_ptr->elem[n].m.upperLimitPresent)
            {
              freq_list[freq_num++] = freq_base + sib_3_mapping_ptr->elem[n].upperLimit;
            }
            else
            {
              /* The IE upper_limit is mandatory present if Mapping Function Parameter
               * List has not reached maxMeasIntervals and is not needed otherwise
               */
              if ((n + 1) < sib_3_mapping_ptr->n)
              {
                TDSRRC_MSG2(MSG_LEGACY_ERROR,"PFR upperLimit should be present for %d of %d list",n,sib_3_mapping_ptr->n);
                /* Do not declare invalid PFR message for now */
              }
            }
          }
        }/* for sib_3_mapping_ptr->n */
      }/* if tdsrrc_RAT_utra_TDD */
    }/* for mappingInfo.n */

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"PFR SIB3 freq_num=%d",freq_num);

    for(i = 0; i < freq_num; i++)
    {
      /* Check if the frequency is valid. 
       * Two possible reasons for the frequency to be invalid:
       * 1) The IE value isn't in the range that described above, or
       * 2) The band is not supported by this UE
       */
      if(tdsrrc_validate_dl_arfcn(freq_list[i]) == FAILURE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"PFR: freq %d is not valid",freq_list[i]);
        continue;
      }

      /* If UE receives two or more copies of the same frequency value,
       * it stores and uses one of them
       */
      duplicate_freq = FALSE;
      for(n = 0; n < tdsrrc_csp_int_data.pseudo_fr_info.num_freq; n++)
      {
        if(tdsrrc_csp_int_data.pseudo_fr_info.freq_list[n] == freq_list[i])
        {
          duplicate_freq = TRUE;
          break;
        }
      }
      if(duplicate_freq)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"PFR: duplicated freq %d is discarded",freq_list[i]);
        continue;
      }

      /* Save the frequency */
      tdsrrc_csp_int_data.pseudo_fr_info.freq_list[
        tdsrrc_csp_int_data.pseudo_fr_info.num_freq] = freq_list[i];
      tdsrrc_csp_int_data.pseudo_fr_info.num_freq++;

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"PFR: freq %d is stored",freq_list[i]);

      /* primary frequency is included */
      primary_freq_present = TRUE;
    }

    /* It's required that RSCP_threshold and primary frequency are both included
     * to say pseudo fast return is enable
     */
    if(rscp_threshold_present && primary_freq_present)
    {
      tdsrrc_csp_int_data.pseudo_fr_info.pseudo_fr_active = TRUE;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"PFR is enabled, num_freq = %d,RSCP threshold = %d",tdsrrc_csp_int_data.pseudo_fr_info.num_freq,tdsrrc_csp_int_data.pseudo_fr_info.rscp_threshold);
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"PFR is disabled, Primary freq present %d,RSCP threshold present %d",primary_freq_present,rscp_threshold_present);
      memset(&tdsrrc_csp_int_data.pseudo_fr_info, 0x00, sizeof(tdsrrc_csp_interrat_pseudo_fr_info_type));
    }

  }/* mappingInfoPresent */
  else
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"PFR SIB3 mappingInfo not Present");
  }

  return;
}


#if 0
/*===========================================================================

FUNCTION          RRCCSP_REMOVE_ACQDB_BANDS_FROM_PRIORITIZED_LIST

DESCRIPTION       Removes bands corresponding to entries in ACQ-DB from the prioritized band list.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
static void tdsrrccsp_remove_acqdb_bands_from_prioritized_list(void)
{
  int8 idx,idy = 0;

  if(tdsrrc_csp_curr_select_data.num_of_prioritized_bands > MAX_NUM_OF_TDS_BANDS)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Exceeded the max no of bands %d",tdsrrc_csp_curr_select_data.num_of_prioritized_bands);
    return;
  }
  
  for(idx = (tdsrrc_csp_curr_select_data.num_of_prioritized_bands-1); idx >= 0 ; idx--)
  {
    if(tdsrrc_csp_int_data.deep_sleep_acq_db_band_mask & tdsrrc_csp_curr_select_data.prioritized_bands[idx])
    {
      if(idx != (tdsrrc_csp_curr_select_data.num_of_prioritized_bands-1))
      {
        for(idy = idx+1; (idy < tdsrrc_csp_curr_select_data.num_of_prioritized_bands) && (idy < MAX_NUM_OF_TDS_BANDS); idy++)
        {
          tdsrrc_csp_curr_select_data.prioritized_bands[idy -1] = tdsrrc_csp_curr_select_data.prioritized_bands[idy];
        }
      }
      tdsrrc_csp_curr_select_data.num_of_prioritized_bands--;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"WBP: Removing band 0x%x, num_pri_bands: %d, idx: %d", tdsrrc_csp_curr_select_data.prioritized_bands[idx], 
         tdsrrc_csp_curr_select_data.num_of_prioritized_bands, idx);
    }
  }
}
#endif


/*===========================================================================

FUNCTION          RRC_CSP_INIT_INT_DATA

DESCRIPTION       This function is responsible for initializing the internal
                  data structure to default values.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void  tdsrrccsp_init_int_data(void)
{ 
  /* Set the last CSP event to "No Event" */
  tdsrrc_csp_int_data.last_mm_primitive = TDSRRC_CSP_NO_EVENT_RCVD;
  tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_CELL_SELECTION_SCAN;

  /* Lets presume that MCC=0 and MNC=0 work as Invalid PLMN 
   * TBD: A way to indicate invalid PLMN
   * NOTE: It is presumed that Home PLMN is of GSM MAP type.
   */
  tdsrrc_csp_int_data.home_plmn_id.mcc[0] = 0;
  tdsrrc_csp_int_data.home_plmn_id.mcc[1] = 0;
  tdsrrc_csp_int_data.home_plmn_id.mcc[2] = 1;
  tdsrrc_csp_int_data.home_plmn_id.num_mnc_digits  = 3;
  tdsrrc_csp_int_data.home_plmn_id.mnc[0] = 0;
  tdsrrc_csp_int_data.home_plmn_id.mnc[1] = 0;
  tdsrrc_csp_int_data.home_plmn_id.mnc[2] = 1;


  /* Reset the plmn list and scan type in CSP internal data */
  tdsrrc_csp_int_data.plmn_list.num_plmns = 0;

  tdsrrc_csp_int_data.interrat_reselect_failure_reason = TDSRRC_UMTS_NONE;
  tdsrrc_csp_int_data.tds_cell_squalmin = 0;
  tdsrrc_csp_int_data.tds_cell_srxlevmin = 0;

  tdsrrc_csp_int_data.dest_freq_present = FALSE;
  tdsrrc_csp_int_data.dest_cpid_present = FALSE;
  tdsrrc_csp_int_data.pending_type = TDSRRC_CSP_NONE_PENDING;

  tdsrrc_csp_int_data.curr_acq_count = 0;

  tdsrrc_csp_int_data.init_cell_sel_ind_during_wtointerrat = FALSE;
  tdsrrc_csp_int_data.init_cell_sel_ind_during_stop_mode_change = FALSE;

  /* BPLMN 2G->3G */
  tdsrrc_csp_int_data.bplmn_new_srch               = TRUE;
  tdsrrc_csp_int_data.bplmn_srch_tmr_expired       = TRUE;

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
  tdsrrc_csp_int_data.send_partial_results         = FALSE;
  memset(&tdsrrc_csp_int_data.bplmn_search_timer_value,0,sizeof(tds_manual_search_timer_value_type));
#endif

  tdsrrc_csp_int_data.bplmn_sib_try_next_best_cell = FALSE;
  tdsrrc_csp_int_data.bplmn_srch_cnf               = TRUE;
  tdsrrc_csp_int_data.bplmn_acq_status             = FALSE;

  tdsrrc_csp_int_data.bplmn_req_list_rd_index  = 0;
  tdsrrc_csp_int_data.bplmn_fnd_list_wr_index  = 0;
  tdsrrc_csp_int_data.bplmn_num_req            = 0;  
  tdsrrc_csp_int_data.bplmn_current_band_initialized = FALSE;

  tdsrrc_csp_int_data.bplmn_bgnd_mib_rd_cnt  = 0;
  tdsrrc_csp_int_data.bplmn_bgnd_mib_rd_freq = 0;
  tdsrrc_csp_int_data.bplmn_srch_vars_status = TD2TD_SEARCH_STOP;
  tdsrrc_csp_int_data.bplmn_trans_id= TDSINVALID_LIST_TRANS_ID;
  tdsrrc_csp_int_data.bplmn_dual_mode_enabled = FALSE;
  tdsrrc_csp_int_data.bplmn_tdscdma_mode_enabled = TRUE;

#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
  tdsrrc_csp_int_data.service_search_bgnd = SYS_SERVICE_SEARCH_NONE;
#endif
  
  tdsrrc_csp_int_data.rsp_to_rr = SEND_G2TD_RESEL_REJECT;
  tdsrrc_csp_int_data.oos_gsm_srch_done       = FALSE;
  tdsrrc_csp_int_data.oos_gsm_num_avail_plmns = 0;
  /* Initialize the variable with an invalid value */
  tdsrrc_csp_int_data.aset_cpid_code = TDSRRC_CSP_INVALID_CPID;


  /* Initialize variables associated with TD2G BPLMN search */
  tdsrrccsp_init_td2g_plmn_srch_int_data();

  /* Initialize GSM list size to 0 */

  /* Initialize Deep Sleep in RRC Connected Mode Variables */
  tdsrrc_csp_int_data.oos_initial_no_deep_sleep_tmr_expd = TRUE;
  tdsrrc_csp_int_data.oos_conn_mode_ds_tmr_expd          = TRUE;

  tdsrrc_csp_int_data.bar_for_resel_only= FALSE;
  tdsrrc_csp_int_data.non_eq_plmn_barred=FALSE;

  tdsrrctmr_stop_timer(RRCTMR_G2TD_REDIRECTION_WAIT_TIMER);
  tdsrrctmr_stop_timer(RRCTMR_G2TD_REDIRECTION_LFS_TIMER);

  tdsrrc_csp_int_data.gtow_redirection_timer_expired = TRUE;
  tdsrrc_csp_int_data.gtow_redir_retry_lfs_scan = FALSE;
  tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies = 0;
  tdsrrc_csp_int_data.cel_resel_ind_rcvd_waiting_trans_cnf = FALSE;
  tdsrrc_csp_int_data.cel_trans_cnf_recvd_at_llc = TRUE;

  tdsrrc_csp_int_data.skip_gsm_search = FALSE;

  tdsrrc_csp_int_data.est_req_for_emerg_call = FALSE;



  /* ZZTBD: should make LFS TRUE default for TDD ? */
  tdsrrc_csp_int_data.do_lfs_scan = FALSE;

  /* Reset ACQ_MODE to LFS */
  /* Temp CR428312 not to use until PFL is supported */
  /* tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_LFS; */
  tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_NORMAL;
  /* Initialize acq_fs_status to TDSL1_ACQ_FS_NONE*/
  tdsrrc_csp_int_data.acq_fs_status = TDSL1_ACQ_FS_NONE;

  tdsrrc_csp_int_data.mode_change_reason = SYS_STOP_MODE_REASON_NONE;

  tdsrrc_csp_int_data.stop_pending_for_wtog_abort = FALSE;

  tdsrrc_csp_int_data.preemption_handling_in_progress = FALSE;
  tdsrrc_csp_int_data.csp_waiting_for_wl1_to_release_lock = FALSE;
  tdsrrc_csp_int_data.set_l1_to_release_lock_when_drop_all = FALSE;
  tdsrrcwrm_stop_lock_wait_timer();
  tdsrrc_csp_int_data.gsm_camped_suitably = FALSE;
  tdsrrc_csp_int_data.cell_reserved_for_operator_use = FALSE;
#if defined(FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  tdsrrc_csp_int_data.interrat_type = TDSRRC_CSP_INTERRAT_NONE;
  tdsrrc_csp_int_data.bplmn_interrat_type = TDSRRC_CSP_INTERRAT_NONE;
  tdsrrc_csp_int_data.detected_frequency_list.num_freqs = 0;
  tdsrrc_csp_int_data.exclude_preredir_info_in_conn_req_msg = FALSE;
  tdsrrc_csp_int_data.cgi_info_included = FALSE;
#endif

  tdsrrc_csp_int_data.plmn_srch_order.num_rats = 0;
  tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx = -1;

  /* Default to A & F TDS bands only. This is for G2TD PLMN search 
   * where the command does not contain band information.
   * For RRC_SERVICE_REQ, tdsrrccsp_update_band_preference()
   * will update tds_band_mask through band_pref
   */
  tdsrrc_csp_int_data.tds_band_mask = RRC_CSP_TDS_A_BAND;
  /* Set band F by condition */
  if(TDSRRC_CSP_SUPPORT_LOWER_BAND_F)
  {
    tdsrrc_csp_int_data.tds_band_mask |= RRC_CSP_TDS_F_LOWER_BAND;
  }
  if(TDSRRC_CSP_SUPPORT_UPPER_BAND_F)
  {
    tdsrrc_csp_int_data.tds_band_mask |= RRC_CSP_TDS_F_UPPER_BAND;
  }

  /* Reset OOS flag if UE is already in DISCONNECTED state */
#ifdef FEATURE_WRLF_SYSTEM_SEL
  if(
        (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
     && (TDSRRC_STATE_DISCONNECTED == tdsrrc_get_state())
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reset ue_in_conn_oos flag");
    tdsrrc_csp_int_data.ue_in_conn_oos = FALSE;
  }
  
  /* Stop CONN_MODE_OOS_TIMER if OOS flag is not set */
  if(!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
  {
    if(!tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd)
    {
      tdsrrctmr_stop_timer(TDSRRCTMR_CONN_MODE_OOS_TIMER);
      tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd = TRUE;
    }
  }

  tdsrrc_csp_int_data.rlf_detected = FALSE;

#ifdef FEATURE_3GPP_CSFB_TDS
  tdsrrccsp_free_sib_cont_list();
  tdsrrc_csp_int_data.sib_read_csfb_active = FALSE;
  tdsrrc_csp_int_data.include_csfb_ind = FALSE;
#endif

#ifdef FEATURE_LTE_TO_TDSCDMA
  tdsrrctmr_stop_timer(TDSRRCTMR_LTE_TO_TDSCDMA_REDIR_SRCH_TIMER);
#endif

  tdsrrc_csp_int_data.allow_lte_resel = TRUE;
  tdsrrc_csp_int_data.l2t_redi_freq_compl = TRUE;
  tdsrrc_csp_int_data.l2t_r9_redirect = TRUE;
  tdsrrc_csp_int_data.l2t_redirect_irat_acqdb_selected = FALSE;

  tdsrrc_csp_int_data.use_special_freq_for_acq = 
     tdsrrc_csp_int_data.tds_special_freq_enabled;

  tdsrrc_csp_int_data.g2t_redi_freq_compl = TRUE;
  tdsrrc_csp_int_data.is_mset_compl_for_rlf_oos = TRUE;
  tdsrrc_csp_int_data.rlf_skip_first_svc_req = FALSE;
  tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.num_cell = 0;
  tdsrrc_csp_int_data.wrm_lock_tmr_started = FALSE;
  tdsrrc_csp_int_data.trm_timeout_value = 0xFFFFFFFF;
  tdsrrc_csp_int_data.is_shallow_search_complete = FALSE; 
  tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan = FALSE;
  tdsrrc_csp_int_data.continue_cell_selection_per_s = FALSE;
  tdsrrc_csp_int_data.td2td_manual_bplmn_guard_srch_timer_remaining = 0;
  tdsrrc_csp_int_data.is_no_trm_lock_for_raw_scan = FALSE;

  tdsrrc_csp_int_data.csfb_extended_status = FALSE;

  tdsrrc_csp_int_data.bplmn_drx_cnt = 0;
  tdsrrc_csp_int_data.is_high_priority_for_irat_bplmn_srch = FALSE;
  tdsrrc_csp_int_data.is_irat_manual_foreground_plmn_srch = FALSE;

  tdsrrc_csp_int_data.is_t2x_bplmn_srched_in_curr_drx = FALSE;
} /* tdsrrccsp_init_int_data */

/*===========================================================================

FUNCTION          RRC_CSP_INIT_WTOG_PLMN_SRCH_INT_DATA

DESCRIPTION       This function is responsible for initializing the internal
                  data structure to default values for TD2G PLMN Search.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void  tdsrrccsp_init_td2g_plmn_srch_int_data( void )
{ 
  rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_aborted = FALSE;
  rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active  = FALSE;
  rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found    = 0;
  rrc_csp_td2g_bplmn_int_data.wtog_hplmn_found        = FALSE;
  rrc_csp_td2g_bplmn_int_data.wtog_rem_drx_tmr_value  = 0;
  rrc_csp_td2g_bplmn_int_data.wtog_srch_done          = FALSE;
  rrc_csp_td2g_bplmn_int_data.restart_bplmn_search    = FALSE;
#ifdef FEATURE_TDSCDMA_TO_LTE
  rrc_csp_td2g_bplmn_int_data.td2l_plmns_found    = 0;
#endif
  rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_suspended = FALSE;
} /* tdsrrccsp_init_td2g_plmn_srch_int_data */




/*===========================================================================

FUNCTION          RRCCSP_IS_SUPPORTED_BAND

DESCRIPTION       This function checks whether the passed in band is supported
                  by the UE or not.

DEPENDENCIES
                  
RETURN VALUE
                  None.

SIDE EFFECTS
                  None.
                  
===========================================================================*/
boolean tdsrrccsp_is_supported_band(tdsrrc_csp_band_class_type band_class)
{
  /* TDSRRC_MSG1(MSG_LEGACY_HIGH,"band_class is 0x%x",band_class); */

  /* TDD code must come here */
  if(band_class & tdsrrc_csp_int_data.tds_band_mask)
  {
    /* TDSRRC_MSG1(MSG_LEGACY_HIGH,"band_class 0x%x is supported",band_class); */
    return TRUE;
  }
  else
  {
    /* TDSRRC_MSG1(MSG_LEGACY_HIGH,"band_class 0x%x not supported",band_class); */
    return FALSE;
  }

} /* tdsrrccsp_is_supported_band */



/*===========================================================================

FUNCTION          RRCCSP_IS_PREFERENCE_BAND

DESCRIPTION       This function checks whether the passed in band is in preference
                  band list from NAS.

DEPENDENCIES
                  
RETURN VALUE
                  BOOLEAN.

SIDE EFFECTS
                  None.
                  
===========================================================================*/
boolean tdsrrccsp_is_preference_band(tdsrrc_csp_band_class_type band_class)
{
  boolean ret = FALSE;

  switch(band_class)
  {
    case RRC_CSP_TDS_A_BAND:
      if(tdsrrc_csp_int_data.band_pref & SYS_BAND_MASK_TDS_BANDA)
      {
        ret = TRUE;
      }
      break;

    case RRC_CSP_TDS_F_LOWER_BAND:
    case RRC_CSP_TDS_F_UPPER_BAND:
    case RRC_CSP_TDS_F_BAND:
      if(tdsrrc_csp_int_data.band_pref & SYS_BAND_MASK_TDS_BANDF)
      {
        ret = TRUE;
      }
      break;

    case RRC_CSP_TDS_E_BAND:
      if(tdsrrc_csp_int_data.band_pref & SYS_BAND_MASK_TDS_BANDE)
      {
        ret = TRUE;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Unsupported band_class 0x%x",band_class);
      break;
  }

  return ret;
} /* tdsrrccsp_is_supported_band */


/*===========================================================================

FUNCTION          TDSRRCCSP_GET_FIRST_BAND_TO_BE_SCANNED

DESCRIPTION       This function returns the first frequency band to be scanned.

DEPENDENCIES
                  
RETURN VALUE
                  None.

SIDE EFFECTS
                  None.
                  
===========================================================================*/
static tdsrrc_csp_band_class_type
tdsrrccsp_get_first_band_to_be_scanned( void )
{
  uint16 tmp_shift_bit_mask = TDSRRC_CSP_BAND_MAX >> 1;

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Bands, First = 0x%x, Current = 0x%x, next = 0x%x",
           tdsrrc_csp_curr_select_data.first_band_to_be_scanned,
           tdsrrc_csp_curr_select_data.current_band,
           tdsrrc_csp_curr_select_data.next_band_mask);

  /* reset sub band */
  tdsrrc_csp_curr_select_data.current_sub_band = 0;

  /* Update the next_band_mask. Remove first_band_to_be_scanned 
   * from it. 
   */
  tdsrrc_csp_curr_select_data.next_band_mask &= 
    ~tdsrrc_csp_curr_select_data.first_band_to_be_scanned;


  if(tdsrrc_csp_curr_select_data.prioritized_bands_read_idx < tdsrrc_csp_curr_select_data.num_of_prioritized_bands)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"WBP:prioritized_bands_read_idx: %d,num_of_prioritized_bands: %d ",tdsrrc_csp_curr_select_data.prioritized_bands_read_idx,tdsrrc_csp_curr_select_data.num_of_prioritized_bands);

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: next_band_mask : 0x%x",tdsrrc_csp_curr_select_data.next_band_mask);
    tmp_shift_bit_mask = tdsrrc_csp_curr_select_data.prioritized_bands[tdsrrc_csp_curr_select_data.prioritized_bands_read_idx];


    tdsrrc_csp_curr_select_data.next_band_mask &= ~tmp_shift_bit_mask;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: Band to be scanned: 0x%x ",tmp_shift_bit_mask);
    tdsrrc_csp_curr_select_data.prioritized_bands_read_idx++;
    tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan = TRUE;
    return tmp_shift_bit_mask;

  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: next_band_mask : 0x%x",tdsrrc_csp_curr_select_data.next_band_mask);
  tmp_shift_bit_mask = TDSRRC_CSP_BAND_MAX >> 1;



  while(tmp_shift_bit_mask)
  {
    if(tmp_shift_bit_mask & tdsrrc_csp_curr_select_data.next_band_mask)
    {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Addl: Remove the band 0x%x from the band mask",tmp_shift_bit_mask);

        /* Eliminate the particular band being returned from the
         * next_band_mask in preparation for next time
         */
        tdsrrc_csp_curr_select_data.next_band_mask &= ~tmp_shift_bit_mask;
        return tmp_shift_bit_mask;
    }
    else
    {
      tmp_shift_bit_mask = tmp_shift_bit_mask >> 1;
    }
  }

  return TDSRRC_CSP_BAND_MAX;

} /* tdsrrccsp_get_next_band_to_be_scanned */


/*===========================================================================

FUNCTION          RRCCSP_GET_NEXT_BAND_TO_BE_SCANNED

DESCRIPTION       This function returns the next frequency band to be scanned
                  based on the last scanned band.

DEPENDENCIES
                  
RETURN VALUE
                  None.

SIDE EFFECTS
                  None.
                  
===========================================================================*/
tdsrrc_csp_band_class_type
tdsrrccsp_get_next_band_to_be_scanned( void )
{
  uint16 start_freq, end_freq;
  uint16 tmp_shift_bit_mask = TDSRRC_CSP_BAND_MAX >> 1;
  boolean is_no_trm_lock_for_raw_scan = tdsrrc_csp_int_data.is_no_trm_lock_for_raw_scan;
  tdsrrc_csp_int_data.is_no_trm_lock_for_raw_scan = FALSE;
  TDSRRC_MSG4(MSG_LEGACY_HIGH,"Bands, First = 0x%x, Current = 0x%x, next = 0x%x, Sub_band = %d",
           tdsrrc_csp_curr_select_data.first_band_to_be_scanned,
           tdsrrc_csp_curr_select_data.current_band,
           tdsrrc_csp_curr_select_data.next_band_mask,
           tdsrrc_csp_curr_select_data.current_sub_band);
  /*debug trace*/
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Next_band_mask_for_deep: 0x%x ",tdsrrc_csp_curr_select_data.next_band_mask_for_deep);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Is_shallow_search_complete: %d,is_prioritized_band_used_for_freq_scan:%d ",tdsrrc_csp_int_data.is_shallow_search_complete,tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan);

  if((TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress() && 
      ((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) ||
       (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))) ||
     (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) ||
     (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN))
  {
    switch(tdsrrc_csp_curr_select_data.current_band)
    {
      case RRC_CSP_TDS_A_BAND:
        start_freq = TDSL1_BAND_A_CHAN_MIN;
        end_freq = TDSL1_BAND_A_CHAN_MAX;
        break;

      case RRC_CSP_TDS_F_LOWER_BAND:
        start_freq = TDSL1_BAND_F_CHAN_MIN;
        end_freq = TDSL1_BAND_F_CHAN_FS_LOWER_MAX;
        break;

      case RRC_CSP_TDS_F_UPPER_BAND:
        start_freq = TDSL1_BAND_F_CHAN_FS_UPPER_MIN;
        end_freq = TDSL1_BAND_F_CHAN_MAX;
        break;

      case RRC_CSP_TDS_E_BAND:
        start_freq = tdsrrccsp_get_band_e_lower_bound();
        end_freq = tdsrrccsp_get_band_e_upper_bound();
        break;

      default:
        return TDSRRC_CSP_BAND_MAX;
    }

    /* Check if sub band is exhausted */
    if(start_freq + (tdsrrc_csp_curr_select_data.current_sub_band + 1) * TDSRRC_CSP_SUB_BAND_GAP < end_freq)
    {
      /* Move to next sub band, but keep band unchanged */
      tdsrrc_csp_curr_select_data.current_sub_band++;
      return tdsrrc_csp_curr_select_data.current_band;
    }
  }
  /* reset sub band since it's time to move to next band */
  tdsrrc_csp_curr_select_data.current_sub_band = 0;

  /* Update the next_band_mask. Remove first_band_to_be_scanned 
   * from it. 
   */
  if (!((tdsrrc_csp_curr_select_data.first_band_to_be_scanned == tdsrrc_csp_curr_select_data.current_band)&& 
  	  (is_no_trm_lock_for_raw_scan)))
  {
	  tdsrrc_csp_curr_select_data.next_band_mask &= 
	    ~tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"acq_mode: %d deep_acq_needed: %d",tdsrrc_csp_int_data.acq_mode,tdsrrc_csp_int_data.deep_acq_needed);
#if 0
  if ((tdsrrc_csp_int_data.acq_mode == TDSL1_ACQ_MODE_SHALLOW)
      &&(tdsrrc_csp_int_data.deep_acq_needed == FALSE))
  {  
    /* If deep_acq_needed == FALSE when SHALLOW, means there is no freq result.  
     * Remove this band from next_band_mask_for_deep so that we will not try this band when DEEP
     */
     tdsrrc_csp_curr_select_data.next_band_mask_for_deep &= ~tdsrrc_csp_curr_select_data.current_band;
	 /*debug trace*/
     TDSRRC_MSG2(MSG_LEGACY_HIGH,"remove bands from next_band_mask_for_deep: 0x%x due to no need deep,next_band_mask_for_deep: 0x%x",tdsrrc_csp_curr_select_data.current_band,tdsrrc_csp_curr_select_data.next_band_mask_for_deep);
  }
#endif
  tdsrrc_csp_int_data.deep_acq_needed = FALSE;
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Leo: prioritized_bands_read_idx: %d num_of_prioritized_bands: %d",tdsrrc_csp_curr_select_data.prioritized_bands_read_idx,tdsrrc_csp_curr_select_data.num_of_prioritized_bands);
  /* Check next band to search until band mask is empty*/
  while ((tdsrrc_csp_curr_select_data.next_band_mask)|| (tdsrrc_csp_curr_select_data.next_band_mask_for_deep))
  { 
    /* set is_shallow_search_complete as TRUE if all bands exhausted for shallow search*/
    if (tdsrrc_csp_curr_select_data.next_band_mask == 0)
    {
#ifdef FEATURE_TDSCDMA_ACQ_ENHANCEMENT
      if (tdsrrc_csp_int_data.acq_fs_status == TDSL1_ACQ_FS_CONTINUE)
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Try last ACQ after SHALLOW search complete and has Fine Scan result during SHALLOW search");
         return TDSRRC_CSP_BAND_MAX;
      }
      else
      {  
         /* Reset acq_fs_status to TDSL1_ACQ_FS_NONE before start DEEP search */
         tdsrrc_csp_int_data.acq_fs_status = TDSL1_ACQ_FS_NONE;
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Start DEEP search loop");
      }
#endif
      tdsrrc_csp_int_data.is_shallow_search_complete = TRUE;
      /* If prioritized band is used before, set prioritized_bands_read_idx to 0 to retry prioritized band*/
      if (tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan == TRUE)
      {
        tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan = FALSE;
        tdsrrc_csp_curr_select_data.prioritized_bands_read_idx = 0;
      }
      /*set next_band_mask to next_band_mask_for_deep to retry these band for deep*/
      tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_curr_select_data.next_band_mask_for_deep; 
      tdsrrc_csp_curr_select_data.next_band_mask_for_deep = 0;
    }
  
    /* set acq_mode according to SHALLOW or DEEP */
    if (tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
    {
      tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_SHALLOW;
    }
    else
    {
      tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_DEEP;
    }
    /* Band search flow
     * 1. prioritized bands (shallow) if under deep sleep
     * 2. other bands (shallow) (if other bands exist except prioritized bands)
     * 3. prioritized bands (deep)
     * 4. other bands (deep) (if other bands exist except prioritized bands)
     * If prioritized band not used, then follow band A->upper band F->lower band F-> band E
     * first "shallow" for all bands then "deep"
     */
    while(tdsrrc_csp_curr_select_data.prioritized_bands_read_idx < tdsrrc_csp_curr_select_data.num_of_prioritized_bands)
    { 
      /*Indicate whether use prioritized band for freq scan.  This flag is used to retry prioritized band for DEEP if SHALLOW search is complete*/
      if (tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan == FALSE)
      {
        tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan = TRUE;
		/*debug trace*/
		TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set is_prioritized_band_used_for_freq_scan");
      }
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"WBP:prioritized_bands_read_idx: %d,num_of_prioritized_bands: %d ",tdsrrc_csp_curr_select_data.prioritized_bands_read_idx,tdsrrc_csp_curr_select_data.num_of_prioritized_bands);

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: next_band_mask : 0x%x",tdsrrc_csp_curr_select_data.next_band_mask);

      
      tmp_shift_bit_mask = tdsrrc_csp_curr_select_data.prioritized_bands[tdsrrc_csp_curr_select_data.prioritized_bands_read_idx];
      tdsrrc_csp_curr_select_data.prioritized_bands_read_idx++;
      /* Add prioritized_bands to next_band_mask_for_deep due to it may not exist in next_band_mask and next_band_mask_for_deep */
      if (tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
      {
         tdsrrc_csp_curr_select_data.next_band_mask_for_deep |= tmp_shift_bit_mask;
      }
      /* When SHALLOW search not complete, just try every prioritized_bands for SHALLOW
       * When SHALLOW search complete, just try these prioritized_bands that included in next_band_mask_for_deep
       * We add prioritized_bands to next_band_mask_for_deep when doing SHALLOW and remove these bands with no results of freq scan
       * so just trying these prioritized_bands that included in next_band_mask_for_deep can skip these bands with no results of freq scan in SHALLOW
       */
      if ((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
          ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
            &&(tdsrrc_csp_curr_select_data.next_band_mask & tmp_shift_bit_mask)))
	  {
         tdsrrc_csp_curr_select_data.next_band_mask &= ~tmp_shift_bit_mask;
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: Band to be scanned: 0x%x ",tmp_shift_bit_mask);
         /*Skip current band search for shallow/deep if this band does not support shallow/deep*/
         switch(tmp_shift_bit_mask)
         {
           case RRC_CSP_TDS_A_BAND:
              if (((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                   && (tdsrrccsp_band_a_pattern.band_shallow == TRUE))
                 ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                   &&(tdsrrccsp_band_a_pattern.band_deep == TRUE)))
              {
                 return tmp_shift_bit_mask;
              }
              break;  
           
           case RRC_CSP_TDS_F_UPPER_BAND:
              if (((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                   && (tdsrrccsp_band_f_pattern.upper_band_shallow == TRUE))
                 ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                   &&(tdsrrccsp_band_f_pattern.upper_band_deep == TRUE)))
              {
                 return tmp_shift_bit_mask;
              }
              break;          

           case RRC_CSP_TDS_F_LOWER_BAND:
              if (((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                   && (tdsrrccsp_band_f_pattern.lower_band_shallow == TRUE))
                 ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                   &&(tdsrrccsp_band_f_pattern.lower_band_deep == TRUE)))
              {
                 return tmp_shift_bit_mask;
              }
              break;
           case RRC_CSP_TDS_E_BAND:
              if (((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                   && (tdsrrccsp_band_e_pattern.band_shallow == TRUE))
                 ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                   &&(tdsrrccsp_band_e_pattern.band_deep== TRUE)))
              {
                 return tmp_shift_bit_mask;
              }
              break;  

           default:
		      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid prioritized bands 0x%x",tmp_shift_bit_mask);
              break;        
         }
	  }
    }

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: next_band_mask : 0x%x",tdsrrc_csp_curr_select_data.next_band_mask);
    tmp_shift_bit_mask = TDSRRC_CSP_BAND_MAX >> 1;

    while(tmp_shift_bit_mask)
    {
      if(tmp_shift_bit_mask & tdsrrc_csp_curr_select_data.next_band_mask)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Addl: Remove the band 0x%x from the band mask",tmp_shift_bit_mask);

        /* Eliminate the particular band being returned from the
         * next_band_mask in preparation for next time
         */
        tdsrrc_csp_curr_select_data.next_band_mask &= ~tmp_shift_bit_mask;
        /*Skip current band search for shallow/deep if this band does not support shallow/deep*/
        switch(tmp_shift_bit_mask)
        {
          case RRC_CSP_TDS_A_BAND:
             if (((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                  && (tdsrrccsp_band_a_pattern.band_shallow == TRUE))
                ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                  &&(tdsrrccsp_band_a_pattern.band_deep == TRUE)))
             {
                return tmp_shift_bit_mask;
             }
             break;  
           
          case RRC_CSP_TDS_F_UPPER_BAND:
             if (((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                  && (tdsrrccsp_band_f_pattern.upper_band_shallow == TRUE))
                ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                  &&(tdsrrccsp_band_f_pattern.upper_band_deep == TRUE)))
             {
                return tmp_shift_bit_mask;
             }
             break;          

          case RRC_CSP_TDS_F_LOWER_BAND:
             if (((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                  && (tdsrrccsp_band_f_pattern.lower_band_shallow == TRUE))
                ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                  &&(tdsrrccsp_band_f_pattern.lower_band_deep == TRUE)))
             {
                return tmp_shift_bit_mask;
             }
             break;
          case RRC_CSP_TDS_E_BAND:
             if (((tdsrrc_csp_int_data.is_shallow_search_complete == FALSE)
                  && (tdsrrccsp_band_e_pattern.band_shallow == TRUE))
                ||((tdsrrc_csp_int_data.is_shallow_search_complete == TRUE)
                  &&(tdsrrccsp_band_e_pattern.band_deep == TRUE)))
             {
                return tmp_shift_bit_mask;
             }
             break;  

          default:
		     TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid bands 0x%x",tmp_shift_bit_mask);
             break;
        }
      }
      tmp_shift_bit_mask = tmp_shift_bit_mask >> 1;
    }
  }
  tdsrrc_csp_int_data.is_shallow_search_complete = FALSE;
  tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan = FALSE;
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Leo: two loop complete, is_shallow_search_complete %d,prioritized_band_used %d",tdsrrc_csp_int_data.is_shallow_search_complete,tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan);
  return TDSRRC_CSP_BAND_MAX;

} /* tdsrrccsp_get_next_band_to_be_scanned */



/*===========================================================================

FUNCTION          RRC_CSP_CONTINUE_INITIAL_STORED_SEL_CNTRL

DESCRIPTION       This function continues with the already started Initial 
                  and Stored Cell Selection
                  It looks for a frequency for a "specific" PLMN or
                  "Any" PLMN. It saves its current search state in Current
                  Selection Controller Data.

DEPENDENCIES
                  This function is closely knit with 
                  tdsrrccsp_compare_expected_cell() and
                  tdsrrccsp_get_match_acq_entry().
                  A change in logic in either must be reflected in others.

RETURN VALUE
                  SUCCESS: A frequency has been found where Acquisition 
                           can be tried.
                  FAILURE: No frequency found (all exhausted). Time for a 
                           deep sleep ?

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_continue_stored_initial_select_ctrl
(
/*lint -e715*/
  tdsrrc_csp_acq_e_type       *ret_type_acq_ptr,
  tdsrrc_csp_acq_entry_type   *ret_acq_entry_ptr,
  tdsrrc_csp_freq_scan_e_type *ret_type_freq_scan
)
{
  rrc_plmn_identity_type      req_plmn_id;
  tdsrrc_freq_type               ret_freq;
  tdsrrc_csp_acq_entry_type      tmp_acq_entry;
  tdsrrc_csp_acq_entry_type      *acq_entry_ptr  = NULL;

  /* Indicates when all search is exhausted with no success */
  boolean      all_search_exhausted = FALSE;



  tdsrrclog_rrccsp_scan_start_event_type rrccsp_scan_start_evt;


    /* MM/RR has requested to look for specific PLMN */
  req_plmn_id = tdsrrc_csp_curr_select_data.req_plmn.plmn_id;
  
  /* This is done for BPLMN searches. Once ACQ DB is accessed, this variable should
   * be set to FALSE
   */
  tdsrrc_csp_int_data.bplmn_sib_try_next_best_cell = FALSE;

  do  /* Keep looping till an Acq Entry is found */
  {


    /* Proceed based on Current Selection Data */
    if((tdsrrc_csp_curr_select_data.num_tries_on_dest_freq > 0) &&
       (tdsrrc_csp_curr_select_data.dest_freq_in_use == TRUE))
    {
      tdsrrc_csp_curr_select_data.num_tries_on_dest_freq --;
      acq_entry_ptr = &tmp_acq_entry;
      acq_entry_ptr->freq = tdsrrc_csp_int_data.dest_freq;

      if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
      {
        /* For X2T redirect, always use NORMAL */
        tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_NORMAL;
      }
      else
      {
        /* Use Shallow acq mode for T2T redirection to avoid L1 comparing MSET with redirect target freq list */
        tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_SHALLOW;
      }

#ifdef FEATURE_LTE_TO_TDSCDMA
      if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN) &&
          (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE))
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"L2TD: Freq %d try: %d",tdsrrc_csp_int_data.dest_freq,(TDSRRCCSP_L2TD_MAX_ACQ_TRIES_ON_DIRECTED_FREQ - tdsrrc_csp_curr_select_data.num_tries_on_dest_freq));
      }
      else
#endif
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Freq %d try: %d",tdsrrc_csp_int_data.dest_freq,(TDSRRC_CSP_MAX_ACQ_TRIES_ON_DIRECTED_FREQ - tdsrrc_csp_curr_select_data.num_tries_on_dest_freq));
      }
    } 
#ifdef FEATURE_LTE_TO_TDSCDMA
    /* Perform list scan on the neighbour freq list provided by LTE-RRC */
    else if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN) &&
       (tdsrrc_csp_curr_select_data.dest_freq_in_use == TRUE) &&
       (tdsrrc_csp_int_data.num_freq_in_list_db > 0) &&
       (tdsrrc_csp_int_data.curr_rd_idx == 0)
       && ((!tdsrrc_csp_int_data.l2t_redi_freq_compl && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE))
            || (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_GSM))
       )
    {
      tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_LIST_SCAN;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"LFS_COOS: set curr frequency scan type =%d,current acq_mode: %d",tdsrrc_csp_curr_select_data.scan_info.type_freq_scan,tdsrrc_csp_int_data.acq_mode);                
      *ret_type_freq_scan = TDSRRC_CSP_LIST_SCAN;
#ifdef FEATURE_L2TD_REDIRECT_OPT_IRAT_ACQ_DB 
      if (tdsrrc_csp_int_data.l2t_redirect_irat_acqdb_selected)
      {
        /* For L2T redirect with freqs from irat acq db, use SHALLOW to keep the oreder of ACQ */
        tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_SHALLOW;
		tdsrrc_csp_int_data.l2t_redirect_irat_acqdb_selected = FALSE;
      }
      else
#endif
      {
        /* For L2T redirect with freqs not from irat acq db, use LFS */
      tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_LFS;
      }
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: acq_mode changed to %d",tdsrrc_csp_int_data.acq_mode);
      return SUCCESS;
    }
#endif
#if 0
    else if(tdsrrc_nv_channel_lock_item_value !=0)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NV is already read,ALL SEARCH EXHAUSTED");
      *ret_type_freq_scan = TDSRRC_CSP_NO_SCAN;
      ret_acq_entry_ptr = NULL;
      all_search_exhausted = TRUE;
    }
#endif
    else
    {
      switch(tdsrrc_csp_curr_select_data.acq_type)
      {
        case TDSRRC_CSP_SPEC_PLMN:

          /* Check if Acq. database or complete frequency band is being used */
          if(tdsrrc_csp_curr_select_data.acq_db_in_use == TRUE)
          {
            if(tdsrrc_csp_curr_select_data.acq_db_scan_info.start_with_first)
            {
              TDSRRC_MSG0(MSG_LEGACY_LOW,"Start: Specific PLMN srch ACQ DB");
              tdsrrc_csp_curr_select_data.acq_db_scan_info.start_with_first = FALSE;

              if((tdsrrc_csp_int_data.acq_mode != TDSL1_ACQ_MODE_SHORT) &&
                 (tdsrrc_csp_int_data.acq_mode != TDSL1_ACQ_MODE_SHORTER) &&
                 (tdsrrc_csp_int_data.acq_mode != TDSL1_ACQ_MODE_REACQ) &&
				 (tdsrrc_csp_int_data.acq_mode != TDSL1_ACQ_MODE_LFS))
              {
                /* Use normal ACQ Mode for acquisition based on MSET freq list / ACQ DB */
                tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_NORMAL;
              }

            
              /* Do not reset num_available_freq to 0 when Automatic BPLMN search is in 
               * progress and the second or later PLMN entry in the NAS HPLMN list is 
               * being matched. 
               */
              if(((tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
                  || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN))
                 && (tdsrrc_csp_int_data.bplmn_req_list_rd_index == 0))
              {
                tdsrrc_csp_curr_select_data.num_available_freq = 0;
              }
              /* Reset num_available_freq to 0 for all other scans */
              else if((tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
                      && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN))
              {
                tdsrrc_csp_curr_select_data.num_available_freq = 0;
              }

              acq_entry_ptr = tdsrrccsp_get_first_match_acq_entry(req_plmn_id,
                                            tdsrrc_csp_curr_select_data.acq_type);

              if(acq_entry_ptr == NULL)
              { 
                TDSRRC_MSG1(MSG_LEGACY_MED,"SPEC_PLMN tdsrrc_csp_int_data.acq_db_empty = %d",tdsrrc_csp_int_data.acq_db_empty);
                tdsrrc_csp_curr_select_data.current_band = 
                  tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
                tdsrrc_csp_curr_select_data.current_sub_band = 0;
              }
              else
              {
                //EVENT: EVENT_TDSCDMA_RRCCSP_SCAN_START
                rrccsp_scan_start_evt.scan_type = TDSRRCLOG_ACQ_DB_SCAN;
                event_report_payload(EVENT_TDSCDMA_RRC_CSP_SCAN_START, sizeof(tdsrrclog_rrccsp_scan_start_event_type),
                               (void *)&rrccsp_scan_start_evt);

                /* If Automatic BPLMN search is in progress then determine if 
                 * current_band needs to be set or not based on the value of 
                 * bplmn_current_band_initialized. The value stored in current_band
                 * is used to determine which band needs to be scanned first
                 * during full freq scan.
                 * The check below is to avoid updating current_band multiple times
                 * as it may change the order in which full freq scan is done
                 * on supported bands.
                 */
                if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
                    || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN))
                {
                  if(!tdsrrc_csp_int_data.bplmn_current_band_initialized && !tdsrrc_csp_int_data.tds_prim_freq_list_in_use)
                  {
                    tdsrrc_csp_curr_select_data.current_band = acq_entry_ptr->band_class;
                    tdsrrc_csp_curr_select_data.first_band_to_be_scanned = acq_entry_ptr->band_class;
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting 1st_band and current_band to 0x%x",tdsrrc_csp_curr_select_data.current_band);
                    tdsrrc_csp_int_data.bplmn_current_band_initialized = TRUE;
                  }
                }
                /* Initialize current_band and first_band_to_be_scanned */
                else if(!tdsrrc_csp_int_data.tds_prim_freq_list_in_use)
                {
                  tdsrrc_csp_curr_select_data.current_band = acq_entry_ptr->band_class;
                  tdsrrc_csp_curr_select_data.first_band_to_be_scanned = acq_entry_ptr->band_class;
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting 1st_band and current_band to 0x%x",tdsrrc_csp_curr_select_data.current_band);
                }
                TDSRRC_MSG1(MSG_LEGACY_MED,"SPEC_PLMN tdsrrc_csp_int_data.acq_db_empty = %d",tdsrrc_csp_int_data.acq_db_empty);
              }
            }
            else
            {
              acq_entry_ptr = tdsrrccsp_get_next_match_acq_entry(req_plmn_id,
                                            tdsrrc_csp_curr_select_data.acq_type);
            }
  
            if(acq_entry_ptr == NULL)
            {
              if(--tdsrrc_csp_curr_select_data.num_acq_db_and_dest_freq_cycles > 0) 
              {
                /* If still not done with all cycles, start a new one. 
                 * Set the acq_db in use and also reset the count of num_tries_on_dest_freq
                 */
                tdsrrc_csp_curr_select_data.acq_db_in_use = TRUE;
                tdsrrc_csp_curr_select_data.acq_db_scan_info.start_with_first = TRUE;
                
                /* Based on the current scan type, decide if the destination 
                 * frequency (if present) is to be tried again.
                 */
                if((   (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN) 
                    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN) 
                    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN) 
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
                    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN) 
                    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN) 
                    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN) 
#endif
                    /* For RL Failure the scan is changed to OOS_Scan and hence the need
                     * to add this "if" check.
                     */
                    || ((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) 
                        && (TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
                   ) 
                   &&  (tdsrrc_csp_int_data.dest_freq_present))
                {
                  tdsrrc_csp_curr_select_data.num_tries_on_dest_freq = 
                    TDSRRC_CSP_MAX_ACQ_TRIES_ON_DIRECTED_FREQ;
                  tdsrrc_csp_curr_select_data.dest_freq_in_use = TRUE;
                }
                else
                {
                  tdsrrc_csp_curr_select_data.num_tries_on_dest_freq = 0; 
                  tdsrrc_csp_curr_select_data.dest_freq_in_use = FALSE;
                }
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Start next cycle of AcqDB scan");
              }
              else
              {
                /* Since Acq. DB is exhausted for non-barred, non-forbidden area 
                 * cell selection, start with Initial Cell Selection now
                 */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Spec PLMN: No cell in Acq DB");
                tdsrrc_csp_curr_select_data.acq_db_in_use = FALSE;
                tdsrrc_csp_curr_select_data.dest_freq_in_use = FALSE;
                
                if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN) ||
                   (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN)
                   )
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Srch exhausted scantype %d",tdsrrc_csp_int_data.curr_scan);
                  acq_entry_ptr = NULL;
                  all_search_exhausted = TRUE;
                }
#ifdef FEATURE_TDSCDMA_SCAN_SCOPE
				else if(((tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) ||
					(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)) &&
				   (tdsrrc_csp_int_data.bplmn_scan_scope == SYS_SCAN_SCOPE_ACQ_DB))
				{
				  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Spec PLMN: Acq DB Only Search completed");
				  acq_entry_ptr = NULL;
				  all_search_exhausted = TRUE;
				}
#endif
                else
                {
                  /* Go for full freq scan for other scan types */
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Start: Specific PLMN Freq scan");
                  tdsrrccsp_init_freq_scan_data();
                }
              } /* Acq DB exhausted - all cycles */
            } /* acq entry ptr is NULL */
            else if(!tdsrrc_csp_int_data.tds_prim_freq_list_in_use)
            {
              tdsrrc_csp_curr_select_data.current_band = acq_entry_ptr->band_class;
            }
          } /* Acq DB in use */


          else /* Acq DB is not in use */
          {
            /* Only if the frequency scan timer has expired do we want to do a full
             * frequency scan. Otherwise return NULL.
             */
            /* Get the first or next entry from frequency band (Initial 
             *  Selection) 
             */
            if(tdsrrccsp_get_next_freq_scan_entry(&ret_freq) == FAILURE)
            {
              /* Since all frequencies have been exhausted for a non-barred,
               * non-forbidden reg. area cell search, we have exhausted ALL 
               * possibilities. Lets return a NULL to calling function so that 
               * it takes necessary action (Like going to Deep Sleep ?)
               */
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Spec PLMN: No cell in Freq Band 0x%x",tdsrrc_csp_curr_select_data.current_band);
              /* Scan the next band if supported */
              if((tdsrrc_csp_curr_select_data.current_band = 
                    tdsrrccsp_get_next_band_to_be_scanned()) != TDSRRC_CSP_BAND_MAX)
              {
                all_search_exhausted = FALSE;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Start: Any PLMN freq scan on band 0x%x",tdsrrc_csp_curr_select_data.current_band);
                tdsrrccsp_init_freq_scan_data();

              }
              else
              {
                all_search_exhausted = TRUE;
                acq_entry_ptr = NULL;
              }
            }
            else
            {
              //EVENT: EVENT_TDSCDMA_RRCCSP_SCAN_START
              if(tdsrrc_csp_curr_select_data.scan_info.type_freq_scan == TDSRRC_CSP_RAW_SCAN)
              {
                rrccsp_scan_start_evt.scan_type = TDSRRCLOG_FULL_FREQ_SCAN;
                event_report_payload(EVENT_TDSCDMA_RRC_CSP_SCAN_START, sizeof(tdsrrclog_rrccsp_scan_start_event_type),
                               (void *)&rrccsp_scan_start_evt);
              }
              /* If UE is doing Acquisition scan, check if the particular frequency has already
               * been scanned as part of the initial acquisition database scan
               * If yes, then no need to scan that frequency again
               */
              while((tdsrrc_csp_curr_select_data.scan_info.type_freq_scan == TDSRRC_CSP_ACQ_SCAN) &&
                    (tdsrrccsp_should_freq_be_scanned(tdsrrc_csp_int_data.curr_req_plmn, ret_freq) == FALSE))
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Freq %d already scanned",ret_freq);
                if(tdsrrccsp_get_next_freq_scan_entry(&ret_freq) == FAILURE)
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Spec PLMN: No cell in Freq Band");
                  acq_entry_ptr = NULL;
                  all_search_exhausted = TRUE;
                  break;
                }
              }

              if(all_search_exhausted == FALSE)
              {
                acq_entry_ptr = &tmp_acq_entry;
                acq_entry_ptr->freq = ret_freq;
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"Freq %d scan type %d",ret_freq,tdsrrc_csp_curr_select_data.scan_info.type_freq_scan);
              }
              else
              {
                /* Scan the next band if supported */
                if((tdsrrc_csp_curr_select_data.current_band = 
                      tdsrrccsp_get_next_band_to_be_scanned()) != TDSRRC_CSP_BAND_MAX)
                {
                  all_search_exhausted = FALSE;
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Start: Any PLMN freq scan on band 0x%x",tdsrrc_csp_curr_select_data.current_band);
                  tdsrrccsp_init_freq_scan_data();


                }
                else
                {
                  all_search_exhausted = TRUE;
                  acq_entry_ptr = NULL;
                }
              }
            }
          } /* Acq DB is not in use */
          break;
             
        case TDSRRC_CSP_ANY_PLMN:
          /* Check if Acq. database or complete frequency band is being used */
          if(tdsrrc_csp_curr_select_data.acq_db_in_use == TRUE)
          {
            if(tdsrrc_csp_curr_select_data.acq_db_scan_info.start_with_first)
            {
              TDSRRC_MSG0(MSG_LEGACY_LOW,"Start: Any PLMN srch ACQ DB");
              tdsrrc_csp_curr_select_data.acq_db_scan_info.start_with_first = FALSE;
              tdsrrc_csp_curr_select_data.num_available_freq = 0;

              if((tdsrrc_csp_int_data.acq_mode != TDSL1_ACQ_MODE_SHORT) &&
                 (tdsrrc_csp_int_data.acq_mode != TDSL1_ACQ_MODE_SHORTER) &&
                 (tdsrrc_csp_int_data.acq_mode != TDSL1_ACQ_MODE_REACQ) &&
				 (tdsrrc_csp_int_data.acq_mode != TDSL1_ACQ_MODE_LFS))
              {
                /* Use normal ACQ Mode for acquisition based on MSET freq list / ACQ DB */
                tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_NORMAL;
              }

              acq_entry_ptr = tdsrrccsp_get_first_match_acq_entry(req_plmn_id,
                                            tdsrrc_csp_curr_select_data.acq_type);
              if(acq_entry_ptr == NULL)
              { 
                TDSRRC_MSG1(MSG_LEGACY_MED,"ANY PLMN tdsrrc_csp_int_data.acq_db_empty = %d",tdsrrc_csp_int_data.acq_db_empty);
                tdsrrc_csp_curr_select_data.current_band = 
                  tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
                tdsrrc_csp_curr_select_data.current_sub_band = 0;
              }
              else
              {
                //EVENT: EVENT_TDSCDMA_RRC_CSP_SCAN_START
                rrccsp_scan_start_evt.scan_type = TDSRRCLOG_ACQ_DB_SCAN;
                event_report_payload(EVENT_TDSCDMA_RRC_CSP_SCAN_START, sizeof(tdsrrclog_rrccsp_scan_start_event_type),
                               (void *)&rrccsp_scan_start_evt);

                /* If Automatic BPLMN search is in progress then determine if 
                 * current_band needs to be set or not based on the value of 
                 * bplmn_current_band_initialized. The value stored in current_band
                 * is used to determine which band needs to be scanned first
                 * during full freq scan.
                 * The check below is to avoid updating current_band multiple times
                 * as it may change the order in which full freq scan is done
                 * on supported bands.
                 */
                if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
                    || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN))
                {
                  if(!tdsrrc_csp_int_data.bplmn_current_band_initialized)
                  {
                    tdsrrc_csp_curr_select_data.current_band = acq_entry_ptr->band_class;
                    tdsrrc_csp_curr_select_data.current_sub_band = 0;
                    tdsrrc_csp_curr_select_data.first_band_to_be_scanned = acq_entry_ptr->band_class;
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting 1st_band and current_band to 0x%x",tdsrrc_csp_curr_select_data.current_band);
                    tdsrrc_csp_int_data.bplmn_current_band_initialized = TRUE;
                  }
                }
                /* Initialize current_band and first_band_to_be_scanned */
                else
                {
                  tdsrrc_csp_curr_select_data.current_band = acq_entry_ptr->band_class;
                  tdsrrc_csp_curr_select_data.current_sub_band = 0;
                  tdsrrc_csp_curr_select_data.first_band_to_be_scanned = acq_entry_ptr->band_class;
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting 1st_band and current_band to 0x%x",tdsrrc_csp_curr_select_data.current_band);
                }
                TDSRRC_MSG1(MSG_LEGACY_MED,"ANY PLMN tdsrrc_csp_int_data.acq_db_empty = %d",tdsrrc_csp_int_data.acq_db_empty);
              }
            }
            else
            {
              acq_entry_ptr = tdsrrccsp_get_next_match_acq_entry(req_plmn_id,
                                            tdsrrc_csp_curr_select_data.acq_type);
            }
  
            if(acq_entry_ptr == NULL)
            {
              if(--tdsrrc_csp_curr_select_data.num_acq_db_and_dest_freq_cycles > 0) 
              {
                /* If still not done with all cycles, start a new one. 
                 * Set the acq_db in use and also reset the count of num_tries_on_dest_freq
                 */
                tdsrrc_csp_curr_select_data.acq_db_in_use = TRUE;
                tdsrrc_csp_curr_select_data.acq_db_scan_info.start_with_first = TRUE;
                
                /* Based on the current scan type, decide if the destination 
                 * frequency (if present) is to be tried again.
                 */
                if((   (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN) 
                    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN) 
                    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN) 
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
                    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN) 
                    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN) 
                    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN) 
#endif
                    /* For RL Failure the scan is changed to OOS_Scan and hence the need
                     * to add this "if" check.
                     */
                    || ((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) 
                        && (TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
                   )
                   &&  (tdsrrc_csp_int_data.dest_freq_present))
                {
                  tdsrrc_csp_curr_select_data.num_tries_on_dest_freq = 
                    TDSRRC_CSP_MAX_ACQ_TRIES_ON_DIRECTED_FREQ;
                  tdsrrc_csp_curr_select_data.dest_freq_in_use = TRUE;
                }
                else
                {
                  tdsrrc_csp_curr_select_data.num_tries_on_dest_freq = 0;
                  tdsrrc_csp_curr_select_data.dest_freq_in_use = FALSE;
                }
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Start next cycle of AcqDB scan");
              }
              else
              {
                /* Since Acq. DB is exhausted for non-barred, non-forbidden area 
                 * cell selection, start with Initial Cell Selection now
                 */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Any PLMN: No cell in Acq DB");
                tdsrrc_csp_curr_select_data.acq_db_in_use = FALSE;
                tdsrrc_csp_curr_select_data.dest_freq_in_use = FALSE;

                if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN) ||
                   (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN)
                   )
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Srch exhausted scantype %d",tdsrrc_csp_int_data.curr_scan);
                  acq_entry_ptr = NULL;
                  all_search_exhausted = TRUE;
                }
                else
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Start: Any PLMN freq scan");
                  tdsrrccsp_init_freq_scan_data();
                }
              }  /* All cycles of Acq DB exhausted */
            } /* Acq entry ptr is NULL */
            else
            {
              tdsrrc_csp_curr_select_data.current_band = acq_entry_ptr->band_class;
            }
          }  /* Acq DB is in use */


          else /* Acq DB is not in use */
          {
            /* Only if the frequency scan timer has expired do we want to do a full
             * frequency scan. Otherwise return NULL.
             */
            /* Get the first or next entry from frequency band (Initial 
             *  Selection) 
             */
            if(tdsrrccsp_get_next_freq_scan_entry(&ret_freq) == FAILURE)
            {
              /* Since all frequencies have been exhausted for a non-barred,
               * non-forbidden reg. area cell search, we have exhausted ALL 
               * possibilities. Lets return a NULL to calling function so that 
               * it takes necessary action (Like going to Deep Sleep ?)
               */
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Any PLMN: No cell in Freq Band %d",tdsrrc_csp_curr_select_data.current_band);

              /* Scan the next band if supported */
              if((tdsrrc_csp_curr_select_data.current_band = 
                    tdsrrccsp_get_next_band_to_be_scanned()) != TDSRRC_CSP_BAND_MAX)
              {
                all_search_exhausted = FALSE;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Start: Any PLMN freq scan on band 0x%x",tdsrrc_csp_curr_select_data.current_band);
                tdsrrccsp_init_freq_scan_data();

              }
              else
              {
                all_search_exhausted = TRUE;
                acq_entry_ptr = NULL;
              }
            }
            else
            {
              //EVENT: EVENT_TDSCDMA_RRC_CSP_SCAN_START
              if(tdsrrc_csp_curr_select_data.scan_info.type_freq_scan == TDSRRC_CSP_RAW_SCAN)
              {
                rrccsp_scan_start_evt.scan_type = TDSRRCLOG_FULL_FREQ_SCAN;
                event_report_payload(EVENT_TDSCDMA_RRC_CSP_SCAN_START, sizeof(tdsrrclog_rrccsp_scan_start_event_type),
                               (void *)&rrccsp_scan_start_evt);
              }
              /* If UE is doing Acquisition scan, check if the particular frequency has already
               * been scanned as part of the initial acquisition database scan
               * If yes, then no need to scan that frequency again
               */
              while((tdsrrc_csp_curr_select_data.scan_info.type_freq_scan == TDSRRC_CSP_ACQ_SCAN) &&
                    (tdsrrccsp_should_freq_be_scanned(tdsrrc_csp_int_data.curr_req_plmn, ret_freq) == FALSE))
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Freq %d already scanned (Acq DB scan)",ret_freq);
                if(tdsrrccsp_get_next_freq_scan_entry(&ret_freq) == FAILURE)
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Any PLMN: No cell in Freq Band");
                  acq_entry_ptr = NULL;
                  all_search_exhausted = TRUE;
                  break;
                }
              }

              if(all_search_exhausted == FALSE)
              {
                acq_entry_ptr = &tmp_acq_entry;
                acq_entry_ptr->freq = ret_freq;
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"Freq %d scan type %d",ret_freq,tdsrrc_csp_curr_select_data.scan_info.type_freq_scan);
              }
              else
              {
                /* Scan the next band if supported */
                if((tdsrrc_csp_curr_select_data.current_band = 
                      tdsrrccsp_get_next_band_to_be_scanned()) != TDSRRC_CSP_BAND_MAX)
                {
                  all_search_exhausted = FALSE;
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Start: Any PLMN freq scan on band 0x%x",tdsrrc_csp_curr_select_data.current_band);
                  tdsrrccsp_init_freq_scan_data();
                }
              }
            }
          } /* Acq DB is not in use */
  
          break;
      
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Acq type %d not supported,Invalid",tdsrrc_csp_curr_select_data.acq_type);
          return(FAILURE);
  
      } /* switch(tdsrrc_csp_curr_select_data.acq_type) */
    }
    if(acq_entry_ptr  == NULL)
    {
      /* We exhausted the last acq. type search in either Stored Acq. DataBase 
       * (Stored Cell Selection) or in the frequency band (Initial Cell Selection)
       * Mark Start with First so that next search should start with the first 
       * entry (either in Acq. database or the frequency band)
       */
      tdsrrc_csp_curr_select_data.acq_db_scan_info.start_with_first = TRUE;

      /* ZZTBD: should not apply to TDS where LFS only. To verify ACQ after FS and fine FS after raw */
#if 0
      /* If BPLMN search is in progress then set/reset acquisition database 
       * access variables appropriately.
       */
      if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
      {
        if((tdsrrc_csp_int_data.bplmn_req_list_rd_index + 1) < tdsrrc_csp_int_data.bplmn_num_req)
        {
          tdsrrc_csp_int_data.curr_req_plmn.plmn_id = 
                 tdsrrc_csp_int_data.bplmn_req_list[++tdsrrc_csp_int_data.bplmn_req_list_rd_index].plmn_id;

          req_plmn_id = tdsrrc_csp_curr_select_data.req_plmn.plmn_id = tdsrrc_csp_int_data.curr_req_plmn.plmn_id;

          TDSRRC_MSG2(MSG_LEGACY_HIGH,"BPLMN/TD2TD:Srch nxt PLMN in acqDB;bplmn_rd_ix=%d;num_PLMNs_req=%d",tdsrrc_csp_int_data.bplmn_req_list_rd_index,tdsrrc_csp_int_data.bplmn_num_req);

          tdsrrc_csp_curr_select_data.acq_db_in_use                     = TRUE;
          tdsrrc_csp_curr_select_data.acq_db_scan_info.start_with_first = TRUE;
          all_search_exhausted                                       = FALSE;
          tdsrrc_csp_curr_select_data.num_acq_db_and_dest_freq_cycles   = 1;
        }   
      } /* end RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN */
      else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
      {
        if((tdsrrc_csp_int_data.bplmn_req_list_rd_index + 1) < tdsrrc_csp_int_data.wtow_bplmn_num_req)
        {
          tdsrrc_csp_int_data.curr_req_plmn.plmn_id = 
               tdsrrc_csp_int_data.wtow_bplmn_req_list[++tdsrrc_csp_int_data.bplmn_req_list_rd_index].plmn_id;

          req_plmn_id = tdsrrc_csp_curr_select_data.req_plmn.plmn_id = tdsrrc_csp_int_data.curr_req_plmn.plmn_id;

          TDSRRC_MSG2(MSG_LEGACY_HIGH,"BPLMN/TD2TD:Srch nxt PLMN in acqDB;bplmn_rd_ix=%d;num TDS PLMNs req=%d",tdsrrc_csp_int_data.bplmn_req_list_rd_index,tdsrrc_csp_int_data.wtow_bplmn_num_req);

          tdsrrc_csp_curr_select_data.acq_db_in_use                     = TRUE;
          tdsrrc_csp_curr_select_data.acq_db_scan_info.start_with_first = TRUE;
          all_search_exhausted                                       = FALSE;
          tdsrrc_csp_curr_select_data.num_acq_db_and_dest_freq_cycles   = 1;
        }   
      }/* end RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN */
#endif
    }/* end acq_entry_pt */
  }while((acq_entry_ptr == NULL) && (all_search_exhausted == FALSE));
  

  if(all_search_exhausted == FALSE)
  {
    if(acq_entry_ptr != NULL)
    {
      *ret_acq_entry_ptr = *acq_entry_ptr;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: ACQ Ptr should not be NULL");
    }

    if((tdsrrc_csp_curr_select_data.acq_db_in_use) ||
       (tdsrrc_csp_curr_select_data.dest_freq_in_use))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS: Returning type_freq_scan as TDSRRC_CSP_ACQ_SCAN");
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"LFS: acq_db_in_use %d,dest_freq_in_use %d",tdsrrc_csp_curr_select_data.acq_db_in_use,tdsrrc_csp_curr_select_data.dest_freq_in_use);

      /* If Acquisition Database is in use, the next L1 command has to be an ACQ Req */
      *ret_type_freq_scan =  TDSRRC_CSP_ACQ_SCAN;
    }
    else
    {
      /* The next L1 cmd depends on the current frequency scan type */
      *ret_type_freq_scan =  tdsrrc_csp_curr_select_data.scan_info.type_freq_scan;
    }

    /* If UE is in deep sleep and acquisition database is not in use then set 
     * the current_band as scanned
     */
    if(tdsrrc_csp_int_data.ue_in_deep_sleep && !tdsrrc_csp_curr_select_data.acq_db_in_use)
    { 
      tdsrrc_csp_int_data.deep_sleep_band_scanned_mask |= tdsrrc_csp_curr_select_data.current_band;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"deep_sleep_band_scanned_mask = 0x%x and current_band = 0x%x",tdsrrc_csp_int_data.deep_sleep_band_scanned_mask,tdsrrc_csp_curr_select_data.current_band);
    }

    return(SUCCESS);

  } /* End If All Search was NOT exhausted */
  /* Search Exhausted */
  else
  {
#ifdef FEATURE_TDSCDMA_ACQ_ENHANCEMENT
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"SHALLOW search complete: %d,acq_fs_status: %d ",tdsrrc_csp_int_data.is_shallow_search_complete,tdsrrc_csp_int_data.acq_fs_status);
    /* If acq_fs_status == TDSL1_ACQ_FS_CONTINUE, means we have send ACQ_REQ during frequency scan
     * Or else, means we did not do ACQ yet during frequency scan, then not need to send last ACQ_REQ
     */
    if (tdsrrc_csp_int_data.acq_fs_status == TDSL1_ACQ_FS_CONTINUE)
    {
       tdsrrc_csp_int_data.acq_fs_status = TDSL1_ACQ_FS_STOP;
       /* Set curr_acq_entry and return success, this will lead to send the last ACQ_REQ*/
       *ret_type_freq_scan =  TDSRRC_CSP_ACQ_SCAN;
       ret_acq_entry_ptr->freq = 0;
       return(SUCCESS);
    }
    else
#endif
    {  
#ifdef FEATURE_TDSCDMA_SCAN_SCOPE
      if(((tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) ||
          (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)) &&
         (tdsrrc_csp_int_data.bplmn_scan_scope == SYS_SCAN_SCOPE_ACQ_DB))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN Search_Type ACQ_DB: Deep Sleep Full Scan Counters/Timers are not impacted");
      }
      else
#endif
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"All bands exhausted");
         tdsrrc_csp_int_data.acq_fs_status = TDSL1_ACQ_FS_NONE;
         /* Increment counter for service search if UE is in deep sleep */
         if((tdsrrc_csp_int_data.ue_in_deep_sleep) &&
            (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN) &&
            (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) &&
            (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_MANUAL_SCAN) && 
            (tdsrrc_csp_int_data.full_scan_in_deep_sleep_counter < TDSRRC_MAX_FULL_SCANS_IN_DEEP_SLEEP)
  #ifdef FEATURE_WRLF_SYSTEM_SEL
            && (!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
  #endif
          && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
            )
    
         {
           tdsrrc_csp_int_data.full_scan_in_deep_sleep_counter ++;
    
           TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deep Sleep full scan counter = %d",tdsrrc_csp_int_data.full_scan_in_deep_sleep_counter);
         }
    
         if((tdsrrc_csp_int_data.ue_in_deep_sleep) &&
            (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN) &&
            (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) &&
            (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_MANUAL_SCAN) &&
            (tdsrrc_csp_int_data.full_scan_in_deep_sleep_counter >= TDSRRC_MAX_FULL_SCANS_IN_DEEP_SLEEP) &&
            (tdsrrc_csp_int_data.full_scan_needed)
  #ifdef FEATURE_WRLF_SYSTEM_SEL
            && (!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
  #endif
         && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
           )
         {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Full scan timer started");
           /* Start the Frequency Scan Timer and reset the do_freq_scan flag */
           tdsrrctmr_start_timer( TDSRRCTMR_FREQ_SCAN_TIMER, 
                               TDSRRCTMR_FREQ_SCAN_TIMER_IN_MS );
           tdsrrc_csp_int_data.full_scan_needed = FALSE;
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE should NOT do full scan on next wake up for the same band group");
         }
       }
       /* Search did not yield anything and was Exhausted. */
       return(FAILURE);
    }
  }
} /* tdsrrccsp_continue_stored_initial_select_ctrl */
/*lint +e715*/

/*===========================================================================

FUNCTION          RRC_CSP_INIT_LFS_SCAN

DESCRIPTION       This function initiate datafor starting LFS scan.

DEPENDENCIES
                  None

RETURN VALUE

SIDE EFFECTS
                  None
===========================================================================*/
static void tdsrrccsp_init_lfs_scan(void)
{
  /* Invalidate the scanned indices- defined for Flexible DS*/
  tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index = -1;

  tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq = 0;

  /* Initialize data for LFS scan only if do_lfs_scan is set */
  if(tdsrrc_csp_int_data.do_lfs_scan == TRUE)
  {
    tdsrrc_csp_curr_select_data.acq_db_in_use = FALSE;
    /* Reset available frequency list for each search cycle */
    tdsrrc_csp_curr_select_data.num_available_freq = 0;

    /* Update all the matching entries LFS table from ACQ DB */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: Traverse through ACQ DB and start populating list DB");
    tdsrrccsp_update_match_acq_entries_into_list_db();
	tdsrrc_csp_int_data.rlf_detected = FALSE;
    
    /*List Scan DB is not empty, this means ACQ DB search has resulted 
      few successful entries into List Scan Database
    */
    if(tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq == 0)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: No of freqs returned from ACQ DB are 0 during LFS");
    }
  }
}

#ifndef FEATURE_L2TD_REDIRECT_OPT_IRAT_ACQ_DB
/*===========================================================================

FUNCTION        TDSRRCCSP_UPDATE_TARGET_TDD_CELL_LIST_FOR_NGBR_TDD_FREQ

DESCRIPTION
                This function updates structure tdsrrc_csp_int_data.target_tdd_cell_list with R10 lte sib6 neighbor earfcns, after
                the R10 redirected earfcns from lte are all exhausted.

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
static void tdsrrccsp_update_target_tdd_cell_list_for_ngbr_tdd_freq( void )
{
  uint8 i, j;
  boolean add_to_list;
  uint8 num_of_frequencies = 0;
  tdsrrc_target_info_type completed_target_tdd_cell_list;

  completed_target_tdd_cell_list = tdsrrc_csp_int_data.target_tdd_cell_list;
  tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies = 0;

  for (i=0; i<tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn_count && num_of_frequencies<MAX_UTRAN_TDS_FREQUNECIES; i++)
  {
    add_to_list = TRUE;
    if(tdsrrccsp_is_supported_band(tdsrrc_get_frequency_band((tdsrrc_freq_type)tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i])))
    {
    for (j=0; j<completed_target_tdd_cell_list.num_of_frequencies; j++)
    {
      if (tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i] == completed_target_tdd_cell_list.fdd_cell_list[j].uarfcn)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore sib6 freq %d,already in R10 redirected freq list",tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i]);
		add_to_list = FALSE;
		break;
      }
    }
	if (add_to_list)
	{
      for (j=0; j<num_of_frequencies; j++)
      {
        if (tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i] == tdsrrc_csp_int_data.target_tdd_cell_list.fdd_cell_list[j].uarfcn)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore repeated R10 sib6 freq %d",tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i]);
		  add_to_list = FALSE;
		  break;
        }
      }
	}
	if (add_to_list)
	{
      tdsrrc_csp_int_data.target_tdd_cell_list.fdd_cell_list[num_of_frequencies++].uarfcn = tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i];
	  TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: R10 sib6 freq %d added to target_tdd_cell_list",tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i]);
	}
  }
   else
   {
     TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore unsupported R10 sib6 freq %d",tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i]);
   }
  }
  if (num_of_frequencies == MAX_UTRAN_TDS_FREQUNECIES)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: loaded max nbr of %d freqs,ignore remaining sib6 freqs",MAX_UTRAN_TDS_FREQUNECIES);
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: target_tdd_cell_list updated with %d new R10 sib6 freqs",num_of_frequencies);
  tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies = num_of_frequencies;
  /* clear ngbr_tdd_freq_list so it won't come here after the R10 sib6 freqs are exhausted */
  tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn_count = 0;
}
#endif

/*===========================================================================

FUNCTION          RRC_CSP_INIT_SELECT_DATA

DESCRIPTION       This function is responsible for initializing the internal
                  Search data structure to default values based on the passed
                  PLMN t type.

DEPENDENCIES
                  None

RETURN VALUE
                  SUCCESS: Success initializing Select data based on passed
                           PLMN type.
                  FAILURE: Could not initialize Select data for the passed
                           PLMN type.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_init_select_data(rrc_requested_plmn_type  *req_plmn_ptr)
{
  tdsrrc_csp_int_data.is_mset_compl_for_rlf_oos = TRUE;
#ifdef FEATURE_CMCC_SECURE_CALL
#ifdef FEATURE_LTE_TO_TDSCDMA         
#ifdef FEATURE_L2TD_REDIRECT_R10
  boolean ngbr_tdd_freq_list_in_use = TRUE;
#endif
#endif
#endif
  /*Reset DSDS related flages when start cell selection*/
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Rest DSDS flags and Original values are csp_waiting_for_wl1_to_release_lock(%d),preemption_handling_in_progress(%d),set_l1_to_release_lock_when_drop_all(%d)",
                        tdsrrc_csp_int_data.csp_waiting_for_wl1_to_release_lock,tdsrrc_csp_int_data.preemption_handling_in_progress,tdsrrc_csp_int_data.set_l1_to_release_lock_when_drop_all );
  tdsrrc_csp_int_data.preemption_handling_in_progress = FALSE;
  tdsrrc_csp_int_data.csp_waiting_for_wl1_to_release_lock = FALSE;
  tdsrrc_csp_int_data.set_l1_to_release_lock_when_drop_all = FALSE;
  tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan = FALSE;
  tdsrrc_csp_int_data.is_no_trm_lock_for_raw_scan = FALSE;
  if(tdsrrc_csp_int_data.bplmn_new_srch)
  {
    tdsrrc_csp_int_data.acq_fs_status = TDSL1_ACQ_FS_NONE;
    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) ||
       (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN))
    {
      tdsrrc_csp_int_data.bplmn_new_srch = FALSE;

      /* Use short acq mode for G2TD PLMN search */
      if(tdsrrctmr_get_remaining_time(TDSRRCTMR_BPLMN_SRCH_TIMER) <= MIN_X2TD_NORMAL_BPLMN_DRX_TIMER_IN_MS)
      {
        tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_SHORTER;
      }
      else
      {
        tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_SHORT;
      }
    }
    else if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress() &&
            ((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) ||
            (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)))
    {
      /* Use REACQ acq mode for TD2TD BPLMN search */
      tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_REACQ;
    }
    else
    {
      /* Set to the default acq mode: LFS */
      /* Temp CR428312 not to use until PFL is supported */
      /* tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_LFS; */
      tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_NORMAL;
    }
      tdsrrcwrm_reset_l2_ack_req_mask();
    if((req_plmn_ptr->plmn_type != RRC_GSM_MAP_SPECIFIC_PLMN_REQ) &&
       (req_plmn_ptr->plmn_type != RRC_GSM_MAP_ANY_PLMN_REQ))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Unexpected PLMN type %d",req_plmn_ptr->plmn_type);
      return (FAILURE);
    }

    tdsrrc_csp_curr_select_data.req_plmn = *req_plmn_ptr;
  
    /* 1. Initialize count of AcqDB + DestFReq cycles based on Current Scan */
    /* Set the number of Acq DB + Dest freq cycles to its max value */
  
    if(   (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN) 
       || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN) 
       || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN)
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
       || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN)
       || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN)
       || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN)
#endif
       /* For RL Failure the scan is changed to OOS_Scan and hence the need
        * to add this "if" check.
        */
       || (   (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) 
           && (TDSRRC_STATE_CELL_DCH == tdsrrc_get_state())
           && (tdsrrc_csp_int_data.dch_tx_dest_state == TDSRRC_STATE_CELL_FACH)
           && (!tdsrrccsp_is_state_transition_triggered_by_reconfig_msg())
          )
      )
    {

      tdsrrc_csp_curr_select_data.num_acq_db_and_dest_freq_cycles = TDSRRC_CSP_MAX_NUM_ACQDB_DEST_FREQ_CYCLES;

    }
    else
    {
      tdsrrc_csp_curr_select_data.num_acq_db_and_dest_freq_cycles = 1;
    }

    /* 2. Acq DB Initialization */
    /* Start with looking in the Acq. DB */
    tdsrrc_csp_curr_select_data.acq_db_in_use = TRUE;

    /* Initialize use_shallow_mode_in_next_acq */
    tdsrrc_csp_int_data.use_shallow_mode_in_next_acq = FALSE;

    /* Set "Start with First" to TRUE so that the cell search begins with the
     * first entry of Acq. DB or first frequency in Freq Scan.
     */
    tdsrrc_csp_curr_select_data.acq_db_scan_info.start_with_first    = TRUE;


      /* Reset the next_band_mask appropriately */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the next_band_mask");
      tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_int_data.tds_band_mask;
      /* Record next_band_mask for deep search*/
      tdsrrc_csp_curr_select_data.next_band_mask_for_deep = tdsrrc_csp_curr_select_data.next_band_mask;

      tdsrrccsp_prioritize_tds_bands_for_full_scan();

      if(tdsrrc_csp_curr_select_data.prioritized_bands_read_idx < tdsrrc_csp_curr_select_data.num_of_prioritized_bands)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"WBP:prioritized_bands_read_idx: %d,num_of_prioritized_bands: %d ",tdsrrc_csp_curr_select_data.prioritized_bands_read_idx,tdsrrc_csp_curr_select_data.num_of_prioritized_bands);

        tdsrrc_csp_curr_select_data.first_band_to_be_scanned = tdsrrc_csp_curr_select_data.prioritized_bands[tdsrrc_csp_curr_select_data.prioritized_bands_read_idx];

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: First Band to be scanned: 0x%x ",tdsrrc_csp_curr_select_data.prioritized_bands[tdsrrc_csp_curr_select_data.prioritized_bands_read_idx]);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: next_band_mask : 0x%x",tdsrrc_csp_curr_select_data.next_band_mask);
        tdsrrc_csp_curr_select_data.prioritized_bands_read_idx++;
        tdsrrc_csp_int_data.is_prioritized_band_used_for_freq_scan = TRUE;
      }
      else
      {
        if (tdsrrc_csp_curr_select_data.first_band_to_be_scanned == TDSRRC_CSP_BAND_MAX)
        {
           if (tdsrrc_csp_curr_select_data.next_band_mask & RRC_CSP_TDS_A_BAND)
           {
               tdsrrc_csp_curr_select_data.first_band_to_be_scanned = RRC_CSP_TDS_A_BAND;
           }
           else if (tdsrrc_csp_curr_select_data.next_band_mask & RRC_CSP_TDS_F_LOWER_BAND)
           {
               tdsrrc_csp_curr_select_data.first_band_to_be_scanned = RRC_CSP_TDS_F_LOWER_BAND;
           }
           else if (tdsrrc_csp_curr_select_data.next_band_mask & RRC_CSP_TDS_F_UPPER_BAND)
           {
               tdsrrc_csp_curr_select_data.first_band_to_be_scanned = RRC_CSP_TDS_F_UPPER_BAND;
           }
           else if (tdsrrc_csp_curr_select_data.next_band_mask & RRC_CSP_TDS_E_BAND)
           {
               tdsrrc_csp_curr_select_data.first_band_to_be_scanned = RRC_CSP_TDS_E_BAND;
           }
           TDSRRC_MSG1(MSG_LEGACY_HIGH,"Fisrt band is TDSRRC_CSP_BAND_MAX,set first_band_to_be_scanned to: 0x%x",tdsrrc_csp_curr_select_data.first_band_to_be_scanned);
        }
      }


      /* 3. Freq Scan Initialization */
      /* Initialize the frequency scan data */
      tdsrrccsp_init_freq_scan_data();


    /* 4. Number of tries on destination frequency initialization 
     *    based on Current Scan
     */
    if((tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN) &&
       (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN) &&
       (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN) &&
       (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_LEAVING_CONN_MODE_SCAN)   //Add leaving_conn_mode_scan to handle redirect frequency in RRCConnectionRelease
       #ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
        && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN)
        && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN)
        && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN)
       #endif
       #ifdef FEATURE_LTE_TO_TDSCDMA
        && (!((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN) &&
                  (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE)))
       #endif
      )
    {
      /* There is no destination frequency at this point */
      tdsrrc_csp_curr_select_data.num_tries_on_dest_freq = 0;
      tdsrrc_csp_curr_select_data.dest_freq_in_use = FALSE;
    }
    else
    {
      if(tdsrrc_csp_int_data.dest_freq_present)
	  {
#ifdef FEATURE_LTE_TO_TDSCDMA
      /* For L2TD redirection set max ACQ attempt counter*/
		 if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN) &&
			(tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE))
		 {
		     if (!tdsrrc_csp_int_data.l2t_redi_freq_compl)
			 {
				if(tdsrrc_csp_int_data.l2t_r9_redirect)
				{
				tdsrrc_csp_curr_select_data.num_tries_on_dest_freq = TDSRRCCSP_L2TD_MAX_ACQ_TRIES_ON_DIRECTED_FREQ;
				}
				else /* rel-10 l2t multi-freq redirect */
				{
				  tdsrrc_csp_curr_select_data.num_tries_on_dest_freq = 0;
				}
				TDSRRC_MSG2(MSG_LEGACY_HIGH,"L2TD: Setting max ACQ attempt to %d,l2t_r9_redirect: %d",tdsrrc_csp_curr_select_data.num_tries_on_dest_freq,tdsrrc_csp_int_data.l2t_r9_redirect);
				tdsrrc_csp_curr_select_data.dest_freq_in_use = TRUE;
			 }
			 else
			    {
					tdsrrc_csp_curr_select_data.num_tries_on_dest_freq = 0; 
					tdsrrc_csp_curr_select_data.dest_freq_in_use = FALSE;
				}
		  }
		  else
#endif
		     {
			     tdsrrc_csp_curr_select_data.num_tries_on_dest_freq = TDSRRC_CSP_MAX_ACQ_TRIES_ON_DIRECTED_FREQ;
				 tdsrrc_csp_curr_select_data.dest_freq_in_use = TRUE;
			 }
	   }
	   else
	       {
		       tdsrrc_csp_curr_select_data.num_tries_on_dest_freq = 0;
		       tdsrrc_csp_curr_select_data.dest_freq_in_use = FALSE;
	       }
    }
  
    /* 5. Acquisition type initialization based on Requested PLMN type */
    if(req_plmn_ptr->plmn_type == RRC_GSM_MAP_SPECIFIC_PLMN_REQ)
    {
      /* Look for a non-barred, non-forbidden area cell of the 
       * specific PLMN requested 
       */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Acquisition Type: TDSRRC_CSP_SPEC_PLMN %d-%d, mnc digits %d",
                  TDSRRC_CSP_GET_INT_MCC(req_plmn_ptr->plmn_id),
                  TDSRRC_CSP_GET_INT_MNC(req_plmn_ptr->plmn_id),
                  req_plmn_ptr->plmn_id.num_mnc_digits);
      tdsrrc_csp_curr_select_data.acq_type = TDSRRC_CSP_SPEC_PLMN;
    }
    else 
    {
      /* Look for a cell of Any PLMN */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Acquisition Type: TDSRRC_CSP_ANY_PLMN");
      tdsrrc_csp_curr_select_data.acq_type = TDSRRC_CSP_ANY_PLMN;
    }

  if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
     && ((!tdsrrc_csp_int_data.g2t_redi_freq_compl 
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
         && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_GSM)
#endif
         )
#ifdef FEATURE_LTE_TO_TDSCDMA 
        || (!tdsrrc_csp_int_data.l2t_redi_freq_compl && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE))
#endif
        )
    )
    {
#ifdef FEATURE_CMCC_SECURE_CALL	
#ifdef FEATURE_LTE_TO_TDSCDMA         
#ifdef FEATURE_L2TD_REDIRECT_R10
      /* Append SIB6 neighbors to target if there is no target info in R10 L2T redirection requestion.
       * If appending SIB6 neighbors fail, initialise list scan
       */
      if((tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE)
          &&(!tdsrrc_csp_int_data.l2t_r9_redirect) 
          &&(tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies == 0))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Secure call: L2T Redirection R10,no target info");
        /* in case irat acq db is applied, tdsrrc_csp_int_data.target_tdd_cell_list is loaded with proper list, unless all lists are empty */
#ifndef FEATURE_L2TD_REDIRECT_OPT_IRAT_ACQ_DB
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Secure call: L2T Redirection R10,append sib6 neighbor freq");
        tdsrrccsp_update_target_tdd_cell_list_for_ngbr_tdd_freq();
#endif
        if (tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies == 0)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Secure call:L2T Redirection R10,no sib6 neighbor freq");
	       tdsrrc_csp_int_data.l2t_redi_freq_compl = TRUE;
           tdsrrc_csp_int_data.dest_freq_present = FALSE;
           tdsrrc_csp_curr_select_data.num_tries_on_dest_freq = 0;
		   tdsrrc_csp_curr_select_data.dest_freq_in_use = FALSE;
           tdsrrccsp_init_lfs_scan();
           ngbr_tdd_freq_list_in_use = FALSE;
        }
      }
      if (ngbr_tdd_freq_list_in_use)
#endif       
#endif
#endif
      {
        /*copy the target list to tdslist_db*/
        (void)tdsrrccsp_lfs_update_list_db_with_target_tdd_list();
      }
      /* For G2T redirection, set acq_mode as TDSL1_ACQ_MODE_NORMAL if target frequency num is 1
       * else, set acq_mode as TDSL1_ACQ_MODE_LFS
       */
      if ((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
         && (!tdsrrc_csp_int_data.g2t_redi_freq_compl 
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
         && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_GSM)
#endif
	 ))
      {
         if (tdsrrc_csp_int_data.num_freq_in_list_db == 1)
         {
            tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_NORMAL;
         }
         else if (tdsrrc_csp_int_data.num_freq_in_list_db > 1)
         {
            tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_LFS;
         }
      }
    }
    else
    {
      tdsrrccsp_init_lfs_scan();
    }
    /* there shold not be any continuation of existing LFS at this point */
    tdsrrc_csp_int_data.tds_continue_lfs = FALSE;
	tdsrrc_csp_int_data.is_shallow_search_complete = FALSE;
  }
  else
  {
    tdsrrc_csp_curr_select_data.req_plmn = *req_plmn_ptr;
  }   

  return(SUCCESS);

} /* tdsrrccsp_init_select_data */

/*===========================================================================

FUNCTION          rrc_csp_try_cell_acq_on_nv_given_freq

DESCRIPTION       This function will validate the NV freq.
DEPENDENCIES
                  None

RETURN VALUE
SIDE EFFECTS
                  None
===========================================================================*/

boolean tdsrrc_csp_try_cell_acq_on_nv_given_freq
(
    tdsrrc_freq_type tds_freq,
    tdsrrc_csp_acq_e_type  *ret_type_acq_ptr,
    tdsrrc_csp_acq_entry_type  *ret_acq_entry_ptr,
    tdsrrc_csp_freq_scan_e_type *ret_type_freq_scan
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"ACQ for NV Freq %d",tds_freq);

  if(SUCCESS == tdsrrc_validate_dl_arfcn(tds_freq))
  {
    ret_acq_entry_ptr->freq = tds_freq;
    *ret_type_freq_scan     = TDSRRC_CSP_ACQ_SCAN;

    tdsrrc_csp_curr_select_data.current_band = tdsrrc_get_frequency_band(
                                              tdsrrc_csp_int_data.curr_acq_entry.freq);

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting current_band to 0x%x",tdsrrc_csp_curr_select_data.current_band);

    return (TRUE);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: freq %d not in valid band group",tds_freq);
    return (FALSE);
  }
}


/*===========================================================================

FUNCTION          tdsrrccsp_start_cell_selection

DESCRIPTION       This function is responsible for starting a New Stored or
                  Initial Cell Selection. It initializes the Selection Data
                  based on the passed requested PLMN before calling 
                  tdsrrccsp_continue_stored_initial_select_ctrl which takes off
                  from there.

DEPENDENCIES
                  None

RETURN VALUE
                  SUCCESS: A frequency has been found where Acquisition 
                           can be tried.
                  FAILURE: No frequency found (all exhausted) or passed data not 
                           valid. Time for a deep sleep ?
SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_start_cell_selection
(
  rrc_requested_plmn_type  *req_plmn_ptr, 
  tdsrrc_csp_acq_e_type  *ret_type_acq_ptr,
  tdsrrc_csp_acq_entry_type  *ret_acq_entry_ptr,
  tdsrrc_csp_freq_scan_e_type *ret_type_freq_scan
)
{
  uecomdef_status_e_type  ret_status = SUCCESS;
  boolean nv_freq_valid = FALSE;
#if 0
  uint16 tds_nv_freq = 0;
#endif
  /* Initialize Selection data so as to start with the correct Acq. type 
   * corresponding to Requested PLMN type. 
   */ 
  if(tdsrrccsp_init_select_data(req_plmn_ptr) == SUCCESS)
  {
    *ret_type_acq_ptr = tdsrrc_csp_curr_select_data.acq_type;

    tdsrrc_csp_int_data.num_cpid_det = 0;

    /*******************************************************
     * ZZTBD: changed logic in following block due to LTE support
     *        need test verification
     *******************************************************/
    /* Determine if there is a freq in NV that should be
     * scanned first 
     */
    TDSRRC_MSG4(MSG_LEGACY_HIGH,"In tdsrrccsp_start_cell_selection, use_special_freq_for_acq=%d, opt_nv=0x%x, irat_nv=0x%x, audio_DTX=%d", 
                                 tdsrrc_csp_int_data.use_special_freq_for_acq,
                                 tds_nv_optional_feature_list,
                                 tds_nv_interrat_feature_ctrl,
                                 tdsrrc_nv_enable_audio_scr_mode);
    
    if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)
       && (tdsrrc_csp_int_data.tds_special_freq_enabled && tdsrrc_csp_int_data.use_special_freq_for_acq)
      )
    {
      nv_freq_valid = tdsrrc_csp_try_cell_acq_on_nv_given_freq
                      (
                          tdsrrc_csp_int_data.tds_special_freq,
                          ret_type_acq_ptr,
                          ret_acq_entry_ptr,
                          ret_type_freq_scan
                      );
      if(nv_freq_valid == TRUE)
      {
        tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_NORMAL;
        return SUCCESS;                      
      }
    }
#if 0    
    if(tdsrrc_nv_channel_lock_item_value != 0)
    {

      tds_nv_freq = tdsrrc_nv_channel_lock_item_value;
      nv_freq_valid = tdsrrc_csp_try_cell_acq_on_nv_given_freq
                      (
                        (tdsrrc_freq_type)tds_nv_freq,
                          ret_type_acq_ptr,
                          ret_acq_entry_ptr,
                          ret_type_freq_scan
                      );
      if(nv_freq_valid == TRUE)
      {
        tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_NORMAL;
        return SUCCESS;                      
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"NV freq not valid %d",tds_nv_freq);
        return FAILURE;
      }
    }
#endif
    /*******************************************************
     * ZZTBD: changed logic in abpve block due to LTE support
     *        need test verification
     *******************************************************/

    /* For transition from Connected Mode to Idle state, attempt acquisition 
     * on the best cell in active set before trying ACQ DB. Do this only
     * if the cpid code has a valid value (< TDSRRC_CSP_INVALID_CPID). Currently cpid code has
     * a valid value only when RCR procedure sends connection release complete
     * to the network.
     *
     * TDS specific:
     * Since PSCs can range from 0 to 512, the scrambling code can also range
     * from 0 to 16*512-1
     */
    if(tdsrrc_csp_int_data.aset_cpid_code < TDSRRC_CSP_INVALID_CPID)
    {
	  /*Add check "scan_info.list_scan.num_freq>0" to do TDSRRC_CSP_LIST_SCAN.  or else ACQ_REQ with 0 freq CRASH may occur
	   *due to scan_info.list_scan is empty (scan_info.list_scan may be empty after tdsrrccsp_init_lfs_scan())
	   */
      if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN)&&
	  	 (tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq > 0))
      {

#if 0   /* Keep it for reference of single frequency ACQ after RRC state change */
        /* ZZTBD: a better way is check current freq. is already in ACQ DB 
         * If it is, only do a LFS scan without first single freq ACQ */
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Connected Mode ->Idle Tx. Attmpt acq on the best cell (cpid %d,pn %d) in active set",tdsrrc_csp_int_data.aset_cpid_code,tdsrrc_csp_int_data.aset_pn_pos);

        /* Populate the required variables with the scr code and pn position of the cell */
        tdsrrc_csp_int_data.curr_acq_entry.scr_code  = tdsrrc_csp_int_data.aset_cpid_code;
        tdsrrc_csp_int_data.pn_pos                   = tdsrrc_csp_int_data.aset_pn_pos;

        tdsrrc_csp_int_data.acq_mode = TDSL1_WCDMA_FULL;
        tdsrrc_csp_int_data.acq_type = TDSL1_WCDMA_FREQ_SCR_POS;

        *ret_type_freq_scan = TDSRRC_CSP_LAST_ASET_SCAN;
#endif
         /* TDD will update tdsrrc_csp_int_data.acq_freq_list from updated ACQ DB 
          * later on in tdsrrccsp_send_next_freq_to_l1()
          * In new ACQ DB, freq. should be sorted by the order of camped cells */

         tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_LIST_SCAN;
         *ret_type_freq_scan = TDSRRC_CSP_LIST_SCAN;
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"aset_cpid_code(%d) < %d when curr_scan(%d) is not LEAVING_CONN_MODE_SCAN",
                                                                            tdsrrc_csp_int_data.aset_cpid_code,
                                                                            TDSRRC_CSP_INVALID_CPID,
                                                                            tdsrrc_csp_int_data.curr_scan);
        /* Since things have been setup, call continue_stored_initial_select_ctrl */
        ret_status = 
        tdsrrccsp_continue_stored_initial_select_ctrl(ret_type_acq_ptr, 
                                                    ret_acq_entry_ptr,
                                                    ret_type_freq_scan);
      }

      /* Invalidate the cpid code */
      tdsrrc_csp_int_data.aset_cpid_code = TDSRRC_CSP_INVALID_CPID;
    }
    /* aset_cpid_code is invalid */
    else
    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN) 
      && (tdsrrc_csp_int_data.num_freq_in_list_db > 0)
         && ((!tdsrrc_csp_int_data.g2t_redi_freq_compl 
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
             && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_GSM)
#endif
             )
#ifdef FEATURE_LTE_TO_TDSCDMA 
            || ((tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE)&& !(tdsrrc_csp_curr_select_data.dest_freq_in_use 
                                                                                  ||(!tdsrrc_csp_curr_select_data.num_tries_on_dest_freq)))
#endif
            )
     )
    {
      tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_LIST_SCAN;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: set curr frequency scan type =%d",tdsrrc_csp_curr_select_data.scan_info.type_freq_scan);                 
      *ret_type_freq_scan = TDSRRC_CSP_LIST_SCAN;
    }
    else
    if((tdsrrc_csp_int_data.do_lfs_scan && tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq > 0) &&
       /* Extra condition to allow continuation of freq scan after BPLMN resume */
       !(!tdsrrc_csp_int_data.bplmn_new_srch && 
         (tdsrrc_csp_curr_select_data.scan_info.type_freq_scan == TDSRRC_CSP_RAW_SCAN ||
          tdsrrc_csp_curr_select_data.scan_info.type_freq_scan == TDSRRC_CSP_FINE_SCAN))
      )
    {
      tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_LIST_SCAN;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: set curr frequency scan type =%d",tdsrrc_csp_curr_select_data.scan_info.type_freq_scan);                 
      *ret_type_freq_scan = TDSRRC_CSP_LIST_SCAN;
    }
    else
#ifdef FEATURE_TDSCDMA_SCAN_SCOPE
    /* When ACQ DB is empty do not perform full band scan
     * For connected mode OOS do_lfs_scan will be FALSE
     */
    if( tdsrrc_csp_int_data.do_lfs_scan &&
       (tdsrrc_csp_int_data.srvc_req_scan_scope == SYS_SCAN_SCOPE_ACQ_DB) &&
       (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)
      )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"ACQ DB empty. Return immediate no service");
      tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_NO_SCAN;
      *ret_type_freq_scan = TDSRRC_CSP_NO_SCAN;
      ret_status = FAILURE;
    }
    else
#endif
    {
      
      /* Since things have been setup, call continue_stored_initial_select_ctrl */
      ret_status = 
      tdsrrccsp_continue_stored_initial_select_ctrl(ret_type_acq_ptr, 
                                                  ret_acq_entry_ptr,
                                                  ret_type_freq_scan);
    }

    return(ret_status);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Failure initializing Select Data for %d PLMN type",req_plmn_ptr->plmn_type);
    return(FAILURE);
  }

} /* tdsrrccsp_start_cell_selection */


/*===========================================================================

FUNCTION          RRCCSP_FILL_IN_PLMN_LAI_DOMAIN_AC_INFO

DESCRIPTION       

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
static uecomdef_status_e_type tdsrrccsp_fill_in_plmn_lai_domain_ac_info(rrc_service_info_type *service_info_ptr)
{
  tdsrrc_csp_access_control_e_type  cell_access_status;
  tdsrrc_dsac_info_type             rrc_dsac_info;
  tdsrrc_SysInfoType1              *sib1_ptr;
  uint32                         num_cn_domains=0,idx=0;
  tdsrrc_CN_DomainSysInfoList    *   cn_domain_sys_info_list_ptr;

  rrc_plmn_identity_type plmn_id;

  tdsrrc_csp_acq_entry_type *acq_entry_ptr;
  tdsrrc_csp_substate_e_type curr_csp_substate = tdsrrccsp_get_csp_substate();  
  
  if((curr_csp_substate== TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB) ||
     (curr_csp_substate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB))
  {
    acq_entry_ptr = &tdsrrc_csp_int_data.new_cell_acq_entry;
  }
  else
  {
    acq_entry_ptr = &tdsrrc_csp_int_data.curr_acq_entry;
  }

  /* 1. Get the plmn that will be passed to NAS in a function call to determine
   *    if the PLMN/LA of the cell are forbidden or not.
   */
  service_info_ptr->selected_plmn.plmn_id = tdsrrc_csp_int_data.curr_acq_entry.plmn_id;

  /*
    Get the common plmn id from curr_nw_db_entry
  */
  if(acq_entry_ptr->is_valid_nw_sharing)
  {
    plmn_id = tdsrrc_csp_int_data.curr_nw_db_entry.common_plmn_id;
  }
  else
  {
    plmn_id = acq_entry_ptr->plmn_id;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB Index PLMN is MCC%d - MNC%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
  /* 2. Get the cell access status of the cell. I.e., fill up service_info structure with 
   *    cell access status
   */
  if(tdsrrccsp_find_access_control(&cell_access_status, 
                                 &rrc_dsac_info, 
                                 acq_entry_ptr->plmn_id,
                                 acq_entry_ptr->cell_id
                                ,plmn_id
                                 ) == FAILURE)
  {
    ERR_FATAL("Failure finding cell access status",0,0,0); 
  }

  (void) tdsrrccsp_find_rrc_cell_access(service_info_ptr, 
                                      &rrc_dsac_info, 
                                      cell_access_status, 
                                      acq_entry_ptr->status_mask);


  /* 3. Get the CN domain information and fill up service_info structure with this information 
   */
  sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrc_sibdb_return_sib_for_cell(
                                            tdsrrc_SIB1,
                                            plmn_id, 
                                            acq_entry_ptr->cell_id);
  if(sib1_ptr == NULL)
  {
    /* SIB1 for this PLMN, CEll ID combination not present */
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"ERR: SIB1 for MCC %d, MNC %d, Cell ID %d not present",
                                            TDSRRC_CSP_GET_INT_MCC(plmn_id), 
                                            TDSRRC_CSP_GET_INT_MNC(plmn_id),
                                            acq_entry_ptr->cell_id);

    return (FAILURE);
  } /* SIB1 not present */


  if(tdsrrccsp_fill_nas_common_info(sib1_ptr, &(service_info_ptr->gsm_map_common_info)) == FAILURE)
  {
    return(FAILURE);
  }

  /* Fill in the CN domain specific NAS system info now */
  num_cn_domains = 0;
  cn_domain_sys_info_list_ptr = &sib1_ptr->cn_DomainSysInfoList; 

  do
  {            
    /* First check if Core network is GSM MAP. */
    if(cn_domain_sys_info_list_ptr->elem[idx].cn_Type.t == T_tdsrrc_CN_DomainSysInfo_cn_Type_gsm_MAP)
    {
      if(tdsrrccsp_fill_nas_cn_specific_info(&cn_domain_sys_info_list_ptr->elem[idx], 
                                          &service_info_ptr->cn_domain_info[num_cn_domains]) == FAILURE)
      {
        return(FAILURE);
      }

      num_cn_domains ++;
    } /* If CN type is GSM MAP */ 

    idx++;

  }while((cn_domain_sys_info_list_ptr->n > idx) && (num_cn_domains < RRC_MAX_CN_DOMAINS));   

  service_info_ptr->num_cn_domains           = num_cn_domains;
  service_info_ptr->gsm_map_common_info_incl = TRUE;
  service_info_ptr->selected_plmn.plmn_type  = RRC_GSM_MAP_PLMN_SEL;
  service_info_ptr->dsac_present             = rrc_dsac_info.valid;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"OOS: #cn_domains %d",service_info_ptr->num_cn_domains);

#ifdef FEATURE_DSAC
  if(!rrc_dsac_info.valid)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"OOS: dsac info invalid. cell access status = %d",cell_access_status);
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"OOS: dsac info valid. CS cell ac st = %d,PS cell ac st = %d",rrc_dsac_info.cs_access_control,rrc_dsac_info.ps_access_control);
  }
#else
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"OOS: dsac NOT defined. cell access status = %d",cell_access_status);
#endif /* FEATURE_DSAC */

  return SUCCESS;
} /* tdsrrccsp_fill_in_plmn_lai_domain_ac_info */


#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
 /*===========================================================================

FUNCTION          RRC_CSP_CHECK_LAI_IN_REJECT_LIST

DESCRIPTION       This function is responsible to check if the current LAI 
                  is in the LAI reject list
DEPENDENCIES
                  None
RETURN VALUE
                  TRUE: If current LAI is present in reject list
                  FALSE: Otherwise
SIDE EFFECTS
                  None
===========================================================================*/
static boolean tdsrrc_csp_check_lai_in_reject_list(inter_task_lai_T curr_lai)
{
  int i;

  for(i=0;i<tdsrrc_csp_int_data.lai_reject_list.length;i++)
  {
    if(memcmp(&curr_lai,&tdsrrc_csp_int_data.lai_reject_list.lai[i],sizeof(inter_task_lai_T)) == 0)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LAI found in reject list");
      return(TRUE);
    }
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"LAI not found in reject list");
  return FALSE;
}
#endif


#ifdef FEATURE_WRLF_SYSTEM_SEL
/*===========================================================================

FUNCTION          RRC_CSP_COMPARE_EXPECTED_CELL_FOR_CONN_OOS

DESCRIPTION       This function compares the expected acq. type with the 
                  actual Acquisition status mask and PLMN status mask and
                  declares the actual cell as Better than/Same as/Below
                  the expected cell, when UE is in connected mode out of service.
           
DEPENDENCIES
                  This function is closely knit with 
                  tdsrrccsp_continue_stored_initial_select_ctrl() and
                  tdsrrccsp_get_match_acq_entry().
                  The logic in this function to compare actual and expected 
                  cells is based on the logic in rrc_csp_continue_stored_
                  initial_select_ctrl() to search cells. A change in logic
                  in either must be reflected in others.

RETURN VALUE
                  TDSRRC_CSP_CELL_BETTER_THAN_EXPECTED: Cell exceeds expectations.
                  TDSRRC_CSP_CELL_AS_EXPECTED: Cell meets expectations.
                  TDSRRC_CSP_CELL_BELOW_EXPECTED: Cell is below expectations.

SIDE EFFECTS
                  None.
===========================================================================*/
tdsrrc_csp_cell_actual_vs_expected_e_type 
tdsrrccsp_compare_expected_cell_for_conn_oos
(
  tdsrrc_csp_plmn_status_e_type cell_plmn_status_wrt_nas_plmn, 
  tdsrrc_csp_plmn_status_e_type cell_plmn_status_wrt_rlf_plmn, 
  tdsrrc_csp_acq_status_type  acq_status_mask,
  tdsrrc_csp_acq_e_type type_acq_expect
)
{
  boolean is_forbidden_lai = FALSE;
  tdsrrc_csp_cell_actual_vs_expected_e_type  
  comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;

  /* Indicates if the cell is barred */
  boolean  cell_barred = FALSE;

  /* Indicates if cell belongs to forbidden reg. area */
  boolean  cell_forbid_reg_area = FALSE;

  rrc_service_info_type service_info;
  /* Indicates whether the service_info is filled in or not. */ 
  boolean service_info_valid = FALSE;

  boolean rrc_plmn_lai_allowed = FALSE;

  boolean is_current_cell_nw_shared = FALSE;


  if((TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()) &&
     (!service_info_valid))
  {
    /* Fill Service Info as it will be passed in a NAS fn call to determine
     * if registration can be performed on the cell or not. 
     */
    if(FAILURE == tdsrrccsp_fill_in_plmn_lai_domain_ac_info(&service_info))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: OOS: failure returned by the fn tdsrrccsp_fill_in_plmn_lai_domain_ac_info");
      return TDSRRC_CSP_CELL_BELOW_EXPECTED;
    }
    service_info_valid = TRUE;
  } 

  if((tdsrrc_csp_int_data.curr_type_acq == TDSRRC_CSP_ANY_PLMN) &&
     (!service_info_valid))
  {
    /* Fill Service Info as it will be passed in a NAS fn call to determine
     * if registration can be performed on the cell or not. 
     */
    if(FAILURE == tdsrrccsp_fill_in_plmn_lai_domain_ac_info(&service_info))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: OOS: failure returned by the fn tdsrrccsp_fill_in_plmn_lai_domain_ac_info");
      return TDSRRC_CSP_CELL_BELOW_EXPECTED;
    }
    service_info_valid = TRUE;
  } 

  is_current_cell_nw_shared = tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing;


  if(acq_status_mask & TDSRRC_CSP_IDLE_CELL_BAR_BIT)
  {
    cell_barred = TRUE;
  }

  if(cell_plmn_status_wrt_nas_plmn != TDSRRC_CSP_PLMN_SAME_AS_REQ)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Actual: BCCH PLMN diff from NAS requested");
  }

  if(cell_plmn_status_wrt_rlf_plmn != TDSRRC_CSP_PLMN_SAME_AS_REQ)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Actual: BCCH PLMN diff from RLF PLMN");
  }

  if(acq_status_mask & TDSRRC_CSP_FORBID_REG_AREA_BIT)
  {
    cell_forbid_reg_area = TRUE;
  }

  if(cell_barred == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Actual: barred cell");
  }

  if(cell_forbid_reg_area == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Actual: Forbid reg area");
  }


  if(!cell_barred)
  {
    /* In case of emergency call service req, camp on any non-barred PLMN. */
    if((tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED)
        && (tdsrrc_csp_int_data.service_req_cause == RRC_MO_CS_CALL))
    {
        /* Cell is Not Barred. Don't care for the PLMN.
         */
        comparison_status = TDSRRC_CSP_CELL_AS_EXPECTED;
        /* For ANY PLMN ideally we can just say comparison status as expected.
         * Added optimization to provide a better among the List of shared PLMNs */

        if((is_current_cell_nw_shared) &&
           (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_MANUAL) &&
           (!cell_forbid_reg_area))
        {
          /*lint -save -e644*/
#ifdef FEATURE_DUAL_SIM
          rrc_plmn_lai_allowed = mm_rrc_per_subs_plmn_lai_allowed(service_info,tdsrrc_get_as_id());
#else
          rrc_plmn_lai_allowed = mm_rrc_plmn_lai_allowed(service_info);
#endif
          /*lint -restore*/
        }

        if((is_current_cell_nw_shared) &&
           (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_MANUAL) &&
           ((cell_forbid_reg_area) || (!rrc_plmn_lai_allowed)))
        {
          comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Expected: Same PLMN & non-barred & non Forbid area");
        }
    }
    /* If PLMN is the same as the RLF PLMN*/
    else if(((cell_plmn_status_wrt_rlf_plmn == TDSRRC_CSP_PLMN_SAME_AS_REQ) &&
               (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_AUTOMATIC))
               || ((cell_plmn_status_wrt_nas_plmn == TDSRRC_CSP_PLMN_SAME_AS_REQ) &&
                    (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC))
            )
    {

      /* If PLMN is same or eq to the RLF PLMN, for Manual selction check if LAI is forbidden.*/
      if (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
      {
        if(is_current_cell_nw_shared)
        {
          tdsrrc_csp_int_data.curr_lai.PLMN_id= mmumts_convert_rrc_plmn_id_to_nas_plmn_id(tdsrrc_csp_int_data.curr_acq_entry.plmn_id);
        }
    
            /*Check for the forbidden LAI*/
#ifndef FEATURE_TDSCDMA_FREE_FLOATING_TASK
    #ifdef FEATURE_DUAL_SIM
        is_forbidden_lai = mm_per_subs_check_lai_in_reject_list(tdsrrc_csp_int_data.curr_lai,tdsrrc_get_as_id());
    #else
        is_forbidden_lai = mm_check_lai_in_reject_list(tdsrrc_csp_int_data.curr_lai);
    #endif
#else
        is_forbidden_lai = tdsrrc_csp_check_lai_in_reject_list(tdsrrc_csp_int_data.curr_lai);
#endif
        if(is_forbidden_lai)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cause 15: is_forbidden_lai= %d",is_forbidden_lai);
          comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
        }
        else
        {
          comparison_status = TDSRRC_CSP_CELL_AS_EXPECTED;
        }
      }
      else if (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_LIMITED)
      {
        /* This condition is questionable acq_status_mask is always 0 and so cell_forbid_reg_area always FALSE */
        if(cell_forbid_reg_area == TRUE)
        {
          comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Expected: Same PLMN & non-barred & non Forbid area");
        }
        else
        {
          comparison_status = TDSRRC_CSP_CELL_AS_EXPECTED;
        }
      }
    }

    /* CSP should proceed with cell selection if
     *     Cell Selection is part of OOS_Scan 
     * and 30s timer has expired 
     * and the PLMN found is NOT ERPLMN (already checked by the status TDSRRC_CSP_PLMN_DIFF_THAN_REQ above) 
     * and the n/w sel mode is NOT Manual (because for Manual mode UE should look for suitable cells only
     *     even after 30s period - details are in the memo)
     * and PLMN and LA are not forbidden
     * and Cell should allow at least normal calls so that registration is guaranteed
     */
    else if((tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd && 
              (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_MANUAL) &&
              (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_AUTOMATIC) &&
              !cell_forbid_reg_area))
    {
  #ifdef FEATURE_DUAL_SIM
      rrc_plmn_lai_allowed = mm_rrc_per_subs_plmn_lai_allowed(service_info,tdsrrc_get_as_id()); 
  #else
      rrc_plmn_lai_allowed = mm_rrc_plmn_lai_allowed(service_info);
  #endif
      if(rrc_plmn_lai_allowed)
      {
        comparison_status = TDSRRC_CSP_CELL_AS_EXPECTED;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"OOS: Cell as expected. Proceed with camping");
      }
      else
      {
        comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;                
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Expected: non-barred & non Forbid area");
      }
    }
  }
  else /* Cell is barred*/
  {
    comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;                
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Expected: Same PLMN & non-barred & non Forbid area");
  }

  switch(comparison_status)
  {
    case TDSRRC_CSP_CELL_BELOW_EXPECTED:
      TDSRRC_MSG0(MSG_LEGACY_MED,"Acquired cell Below Expected");
      break;

    case TDSRRC_CSP_CELL_AS_EXPECTED:
      TDSRRC_MSG0(MSG_LEGACY_MED,"Acquired cell as Expected");
      break;

    case TDSRRC_CSP_CELL_BETTER_THAN_EXPECTED:
      TDSRRC_MSG0(MSG_LEGACY_MED,"Acquired cell Better than Expected");
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Invalid Cell Comparison Status value: %d",comparison_status);
      break;
  }

  return(comparison_status);
}

#endif

/*===========================================================================

FUNCTION          RRC_CSP_COMPARE_EXPECTED_CELL

DESCRIPTION       This function compares the expected acq. type with the 
                  actual Acquisition status mask and PLMN status mask and
                  declares the actual cell as Better than/Same as/Below
                  the expected cell.
           
DEPENDENCIES
                  This function is closely knit with 
                  tdsrrccsp_continue_stored_initial_select_ctrl() and
                  tdsrrccsp_get_match_acq_entry().
                  The logic in this function to compare actual and expected 
                  cells is based on the logic in rrc_csp_continue_stored_
                  initial_select_ctrl() to search cells. A change in logic
                  in either must be reflected in others.

RETURN VALUE
                  TDSRRC_CSP_CELL_BETTER_THAN_EXPECTED: Cell exceeds expectations.
                  TDSRRC_CSP_CELL_AS_EXPECTED: Cell meets expectations.
                  TDSRRC_CSP_CELL_BELOW_EXPECTED: Cell is below expectations.

SIDE EFFECTS
                  None.
===========================================================================*/
tdsrrc_csp_cell_actual_vs_expected_e_type
tdsrrccsp_compare_expected_cell
(
  tdsrrc_csp_plmn_status_e_type cell_plmn_status, 
  tdsrrc_csp_acq_status_type  acq_status_mask,
  tdsrrc_csp_acq_e_type type_acq_expect
)
{        
  boolean is_forbidden_lai = FALSE;
  tdsrrc_csp_cell_actual_vs_expected_e_type  
  comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
  tdsrrc_csp_substate_e_type curr_csp_substate = tdsrrccsp_get_csp_substate();

  /* Indicates if the cell is barred */
  boolean  cell_barred = FALSE;

  /* Indicates if cell belongs to forbidden reg. area */
  boolean  cell_forbid_reg_area = FALSE;

  rrc_service_info_type service_info;
  /* Indicates whether the service_info is filled in or not. */ 
  boolean service_info_valid = FALSE;

  boolean rrc_plmn_lai_allowed = FALSE;

  boolean is_current_cell_nw_shared = FALSE;

  boolean dual_mode_enabled = tdsrrcmcm_is_dualmode_enabled();

  if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) &&
     (!service_info_valid))
  {
    /* Fill Service Info as it will be passed in a NAS fn call to determine
     * if registration can be performed on the cell or not. 
     */
    if(FAILURE == tdsrrccsp_fill_in_plmn_lai_domain_ac_info(&service_info))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: OOS: failure returned by the fn tdsrrccsp_fill_in_plmn_lai_domain_ac_info");
      return TDSRRC_CSP_CELL_BELOW_EXPECTED;
    }
    service_info_valid = TRUE;
  } 

  if((tdsrrc_csp_int_data.curr_type_acq == TDSRRC_CSP_ANY_PLMN) &&
     (!service_info_valid))
  {
    /* Fill Service Info as it will be passed in a NAS fn call to determine
     * if registration can be performed on the cell or not. 
     */
    if(FAILURE == tdsrrccsp_fill_in_plmn_lai_domain_ac_info(&service_info))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: OOS: failure returned by the fn tdsrrccsp_fill_in_plmn_lai_domain_ac_info");
      return TDSRRC_CSP_CELL_BELOW_EXPECTED;
    }
    service_info_valid = TRUE;
  } 

  if((curr_csp_substate == TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB) ||
     (curr_csp_substate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB))
  {
    is_current_cell_nw_shared = tdsrrc_csp_int_data.new_cell_acq_entry.is_valid_nw_sharing;
  }
  else
  {
    is_current_cell_nw_shared = tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing;
  }

  if(acq_status_mask & TDSRRC_CSP_IDLE_CELL_BAR_BIT)
  {
    cell_barred = TRUE;
  }

  if(cell_plmn_status != TDSRRC_CSP_PLMN_SAME_AS_REQ)
  {
    tdsrrc_csp_int_data.interrat_reselect_failure_reason = TDSRRC_UMTS_PLMN_MISMATCH;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Actual: Diff PLMN");
  }

  if(acq_status_mask & TDSRRC_CSP_FORBID_REG_AREA_BIT)
  {
    cell_forbid_reg_area = TRUE;
    tdsrrc_csp_int_data.interrat_reselect_failure_reason = TDSRRC_UMTS_FORBIDDEN_LA;
  }

  if(cell_barred == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Actual: barred cell");
  }
  if(cell_forbid_reg_area == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Actual: Forbid reg area");
  }

  switch(type_acq_expect)
  {
    case TDSRRC_CSP_SPEC_PLMN:
      /* A non-barred cell belonging to a specific PLMN (matching the requested 
       * PLMN) and NOT in a forbidden reg. area was expected. 
       */
      if((cell_plmn_status == TDSRRC_CSP_PLMN_SAME_AS_REQ) && 
         (cell_barred == FALSE))
      {        
        /* If Network select mode is MANUAL then skip the check
         * for forbidden LAIs
         */
        if(
           (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
           /* If Background Service Request is received from NAS/RR when UE mode
            * is manual then RRC needs to do the same checks as are done when
            * n/w sel mode is explicitly set to SYS_NETWORK_SELECTION_MODE_MANUAL 
            */
           || 
           (   ((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
                || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN))
            && (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC)
            && (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_MANUAL)
           )
#endif /* FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH */
          )
        {
          if((curr_csp_substate ==TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB) ||             
             (tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_CELL_RESELECTION_SCAN))
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"curr CSP sub state =%d,curr CSP scan =%d",curr_csp_substate ,tdsrrc_csp_int_data.curr_scan);
            if(cell_forbid_reg_area == TRUE)
            {
              comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Expected: Same PLMN & non-barred & non Forbid area");
            }
            else
            {
              comparison_status = TDSRRC_CSP_CELL_AS_EXPECTED;
            }
          }
          else  /* for rest of the scans */
          {

            if(is_current_cell_nw_shared)
            {
              /*Convert RRC PLMN Id to NAS PLMN Id*/
              if(curr_csp_substate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB)
              {
                tdsrrc_csp_int_data.curr_lai.PLMN_id= mmumts_convert_rrc_plmn_id_to_nas_plmn_id(tdsrrc_csp_int_data.new_cell_acq_entry.plmn_id);
              }
              else
              {
                tdsrrc_csp_int_data.curr_lai.PLMN_id= mmumts_convert_rrc_plmn_id_to_nas_plmn_id(tdsrrc_csp_int_data.curr_acq_entry.plmn_id);
              }
            }

            /*Check for the forbidden LAI*/
#ifndef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_DUAL_SIM
            is_forbidden_lai = mm_per_subs_check_lai_in_reject_list(tdsrrc_csp_int_data.curr_lai,tdsrrc_get_as_id());
#else
            is_forbidden_lai = mm_check_lai_in_reject_list(tdsrrc_csp_int_data.curr_lai);
#endif
#else
            is_forbidden_lai = tdsrrc_csp_check_lai_in_reject_list(tdsrrc_csp_int_data.curr_lai);
#endif
            if(is_forbidden_lai)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cause 15: is_forbidden_lai= %d",is_forbidden_lai);
              comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
            }
            else
            {
              comparison_status = TDSRRC_CSP_CELL_AS_EXPECTED;
            }
          }
        }
        else
        {
          if(cell_forbid_reg_area == TRUE)
          {
            comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Expected: Same PLMN & non-barred & non Forbid area");
          }
          else
          {
            comparison_status = TDSRRC_CSP_CELL_AS_EXPECTED;
          }
        }
      }
      else if(cell_barred)
      {
        comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;                
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Expected: Same PLMN & non-barred & non Forbid area");
      }
      /* Cell_plmn_status == TDSRRC_CSP_PLMN_DIFF_THAN_REQ */
      else 
      {
        /* CSP should proceed with cell selection if
         *     Cell Selection is part of OOS_Scan 
         * and 30s timer has expired 
         * and atleast 1 TDS search is done
         * and the PLMN found is NOT ERPLMN (already checked by the status TDSRRC_CSP_PLMN_DIFF_THAN_REQ above) 
         * and the n/w sel mode is NOT Manual (because for Manual mode UE should look for suitable cells only
         *     even after 30s period - details are in the memo)
         * and PLMN and LA are not forbidden
         * and Cell should allow at least normal calls so that registration is guaranteed
         */
        if(    (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
            && (tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd)
            && (    ((dual_mode_enabled) && (tdsrrc_csp_int_data.oos_gsm_srch_done))
                ||  (!dual_mode_enabled))
            && (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_MANUAL)
            && (!cell_forbid_reg_area))
        {
          /*lint -save -e644*/
#ifdef FEATURE_DUAL_SIM
          rrc_plmn_lai_allowed = mm_rrc_per_subs_plmn_lai_allowed(service_info,tdsrrc_get_as_id()); 
#else
          rrc_plmn_lai_allowed = mm_rrc_plmn_lai_allowed(service_info);
#endif
          /*lint -restore*/
        }
        if(    (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
            && (tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd)
            && (    ((dual_mode_enabled) && (tdsrrc_csp_int_data.oos_gsm_srch_done))
                ||  (!dual_mode_enabled))
            && (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_MANUAL)
            && (!cell_forbid_reg_area)
            /*lint -save -e644*/
            && (rrc_plmn_lai_allowed)
            /*lint -restore*/
          )
        {  
          comparison_status = TDSRRC_CSP_CELL_AS_EXPECTED;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"OOS: Cell as expected. Proceed with camping");
        }
        else
        {
          comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;                
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Expected: Same PLMN & non-barred & non Forbid area");
        }
      }
      break;

    case TDSRRC_CSP_ANY_PLMN:
      /* A NON barred cell in a NON forbidden registration area belonging to 
       * Any PLMN was expected.
       */
      if(cell_barred == FALSE)
      {
        /* Cell is Not Barred. Don't care for the PLMN.
         */
        comparison_status = TDSRRC_CSP_CELL_AS_EXPECTED;
        /* For ANY PLMN ideally we can just say comparison status as expected.
         * Added optimization to provide a better among the List of shared PLMNs */

        if((is_current_cell_nw_shared) &&
           (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) &&
           (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) &&
           (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_MANUAL) &&
           (!cell_forbid_reg_area))
        {
          /*lint -save -e644*/
#ifdef FEATURE_DUAL_SIM
          rrc_plmn_lai_allowed = mm_rrc_per_subs_plmn_lai_allowed(service_info,tdsrrc_get_as_id());
#else
          rrc_plmn_lai_allowed = mm_rrc_plmn_lai_allowed(service_info);
#endif
          /*lint -restore*/
        }

        if((is_current_cell_nw_shared) &&
           (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) &&
           (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) &&
           (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_MANUAL) &&
           ((cell_forbid_reg_area) || (!rrc_plmn_lai_allowed)))
        {
          comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Expected: Same PLMN & non-barred & non Forbid area");
        }
      }
      else
      {
        /* Cell is barred */
        comparison_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Expected: Any PLMN & non-barred");
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid expected type %d of Acq.",type_acq_expect); 
      break;

  } /* switch(type_acq_expect) */

  switch(comparison_status)
  {
    case TDSRRC_CSP_CELL_BELOW_EXPECTED:
      TDSRRC_MSG0(MSG_LEGACY_MED,"Acquired cell Below Expected");
      break;

    case TDSRRC_CSP_CELL_AS_EXPECTED:
      TDSRRC_MSG0(MSG_LEGACY_MED,"Acquired cell as Expected");
      break;

    case TDSRRC_CSP_CELL_BETTER_THAN_EXPECTED:
      TDSRRC_MSG0(MSG_LEGACY_MED,"Acquired cell Better than Expected");
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Invalid Cell Comparison Status value: %d",comparison_status);
      break;
  }

  return(comparison_status);

} /* tdsrrccsp_compare_expected_cell */


/*===========================================================================

FUNCTION          RRC_CSP_GEN_STATUS_MASK

DESCRIPTION       This function generates an Acq. status mask from Cell 
                  Barred status and LAI Status. The Acq. Status mask is
                  understood by Acq. database whereas Cell Barred status and 
                  LAI Status are understood by Cell Selection utility 
                  functions and Cell Selection state machine functions.
                  
                  This utility acts as an interpretor so that the status 
                  returned by Cell selection utilities can be compared with
                  the one returned by Acq. Database access functions.

DEPENDENCIES      None.

RETURN VALUE
                  A bit mask that represents Acq. status.

SIDE EFFECTS
                  None
===========================================================================*/
tdsrrc_csp_acq_status_type 
tdsrrccsp_gen_status_mask(tdsrrc_csp_cell_bar_e_type  cell_bar_status,
                        tdsrrc_csp_lai_status_e_type  lai_status)
{
    tdsrrc_csp_acq_status_type  acq_status_mask = 0;

    /* Set the Cell bar bit if cell is non-barred */
    if(cell_bar_status != TDSRRC_CSP_CELL_OK)
    {
      acq_status_mask |= TDSRRC_CSP_IDLE_CELL_BAR_BIT;
    }

    /* Set the Reg Area barred bit if the cell belongs to a forbidden LAI */
    if(lai_status != TDSRRC_CSP_LAI_OK)
    {
      acq_status_mask |= TDSRRC_CSP_FORBID_REG_AREA_BIT;
    }

    return(acq_status_mask);
} /* tdsrrccsp_gen_status_mask */


/*===========================================================================

FUNCTION          RRC_CSP_FILL_ACQ_ENTRY_FROM_SIB

DESCRIPTION       This function is responsible for filling up all fields of
                  Acq Entry from SIB3 data and SIB Confirmation command data.
                  It also fills the Cell Barred status into the passed pointer
                  for usage by the calling function.

DEPENDENCIES
                  SIB Confirmation pointer should point to valid data.
                  SIB3 should have been received for this cell (identified 
                  by PLMN ID, Cell ID combination).

RETURN VALUE
                  SUCCESS: Acq entry successfully filled.
                  FAILURE: Acq. entry could not be filled (multiple reasons)

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_fill_acq_entry_from_sib( tdsrrc_sib_event_cnf_type *sib_cnf, 
                                 tdsrrc_csp_acq_entry_type *acq_entry_ptr,
                                 tdsrrc_csp_cell_bar_e_type  *cell_bar_status)
{
    
  /* Temporary storage for LAI status */
  tdsrrc_csp_lai_status_e_type  lai_status;

  /* This stores the LAI received in SIB1 */
  rrc_lai_type  lai;

  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_common_info;

  /* Pointer to SIB1 */
  tdsrrc_SysInfoType1 *sib1_ptr;

  unsigned char  *tdsdata_ptr;
  uint32         i;

  boolean shared_plmn_idx = 0;
  boolean hplmn_cell_bar_status_found = FALSE;
  tdsrrc_csp_cell_bar_e_type hplmn_cell_bar_status = TDSRRC_CSP_CELL_OK;
  boolean vplmn_cell_bar_status_found = FALSE;
  tdsrrc_csp_cell_bar_e_type vplmn_cell_bar_status = TDSRRC_CSP_CELL_OK;


  rrc_plmn_identity_type sib_idx_plmn_id;

  /* Don't go further if one of passed pointers is a NULL */
  if((sib_cnf == NULL) || (acq_entry_ptr == NULL) || (cell_bar_status == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: Acq ptr ,SIB Cnf ptr or Cell Bar status ptr is a NULL"); 
    return(FAILURE);
  }

  /* Fill the tmp_acq_entry with SIB data */
  if(!acq_entry_ptr->is_valid_nw_sharing)
  {
    acq_entry_ptr->plmn_id    = sib_cnf->plmn_id;
  }

  if(acq_entry_ptr->is_valid_nw_sharing)
  {
    sib_idx_plmn_id = tdsrrc_csp_int_data.curr_nw_db_entry.common_plmn_id;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Use Common PLMN-ID from curr_nw_db_entry");
  }
  else
  {
    sib_idx_plmn_id = acq_entry_ptr->plmn_id;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sib_idx PLMN: MCC %d-MNC %d",TDSRRC_CSP_GET_INT_MCC(sib_idx_plmn_id),TDSRRC_CSP_GET_INT_MNC(sib_idx_plmn_id));
  acq_entry_ptr->freq       = (tdsrrc_freq_type)sib_cnf->freq;
  acq_entry_ptr->cpid   = (tdsrrc_cpid_type)sib_cnf->cpid;
  acq_entry_ptr->cell_id    = sib_cnf->cell_id;

  acq_entry_ptr->band_class = tdsrrc_get_frequency_band(acq_entry_ptr->freq);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Setting band_class 0x%x for freq %d",acq_entry_ptr->band_class,acq_entry_ptr->freq);

  /* Now generate status mask for this cell */
  /* Fill up the NAS system info here */
  /* Let us first read SIB1 - we should have SIB1 for sure */
  /* TD2TD BPLMN search in progress. Read SIB1 */
 /* Now generate status mask for this cell */
  /* Fill up the NAS system info here */
  /* Let us first read SIB1 - we should have SIB1 for sure */
  if((tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
		 && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
		 && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
		 && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
      && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN)
      && (!tdsrrccsp_is_dummy_plmn_search(tdsrrc_csp_int_data.curr_req_plmn.plmn_id))
     )
  {
    sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrc_sibdb_return_sib_for_cell(
                                              tdsrrc_SIB1,
                                              sib_idx_plmn_id, 
                                              acq_entry_ptr->cell_id);
  }
  /* TD2TD BPLMN search in progress. Read SIB1 */
  else
  {
    sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrcsib_bplmn_return_sib_for_cell(tdsrrc_SIB1);

  }
  if( sib1_ptr == NULL )
  {
    /* SIB3 for this PLMN, CEll ID combination not present */
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"ERR: SIB1 for MCC %d, MNC %d, Cell ID %d not present",
                      TDSRRC_CSP_GET_INT_MCC(sib_idx_plmn_id), 
                      TDSRRC_CSP_GET_INT_MNC(sib_idx_plmn_id),
                      acq_entry_ptr->cell_id);

    return(FAILURE);
  } /* SIB1 not present */

  /* The SIB1 is present */
  nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;
  /* Check if the RRC MM interface structure array can hold this info */
  if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Not enough space for Common NAS Sys Info (%d bytes)",sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
    return(FAILURE);
  }

  /* Fill in the Common NAS system info now */
  tdsdata_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
  for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
  {
    nas_common_info.value[i] =  *tdsdata_ptr;
    tdsdata_ptr ++;
  }

  /* Get the LAI status from local utility */
  /* TBD: Call MM utility to initialize lai first. */
  if(mm_extract_lac_from_sib_info(&lai, &nas_common_info) == FALSE)
  {
    ERR_FATAL("Could not extract LAI from NAS Common sys info",0,0,0);
  }
  
  /* Since MM doesn't know the PLMN from NAS system info, fill in the PLMN now */
  lai.plmn = acq_entry_ptr->plmn_id;

  tdsrrc_csp_int_data.curr_lai.location_area_code[0]= lai.lac[0];
  tdsrrc_csp_int_data.curr_lai.location_area_code[1]= lai.lac[1];
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Cause 15: lac[0] = %d,lac[1]=%d",tdsrrc_csp_int_data.curr_lai.location_area_code[0],tdsrrc_csp_int_data.curr_lai.location_area_code[1]);
  /*Convert RRC PLMN Id to NAS PLMN Id*/
  tdsrrc_csp_int_data.curr_lai.PLMN_id= mmumts_convert_rrc_plmn_id_to_nas_plmn_id(lai.plmn);

  if(tdsrrccsp_find_forbid_lai(&lai, &lai_status) 
     == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure finding LAI status");
    return(FAILURE);
  }

  /* Generate Cell barred status based on SIB info for this cell */
  if(tdsrrccsp_find_cell_bar_status(acq_entry_ptr->plmn_id, 
                                  acq_entry_ptr->cell_id, 
                                  cell_bar_status
                                  ,sib_idx_plmn_id
                                  ) 
      == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure to fill Cell Barred Status");
    return(FAILURE);
  }

  /* Generate Acq. entry status mask for storage in Acq. database */ 
  acq_entry_ptr->status_mask = tdsrrccsp_gen_status_mask(*cell_bar_status, 
                                                       lai_status);

  if(acq_entry_ptr->is_valid_nw_sharing)
  {
    tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx++].status_mask = acq_entry_ptr->status_mask;

    if(tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn(acq_entry_ptr->plmn_id))
    {
      hplmn_cell_bar_status = *cell_bar_status;
      hplmn_cell_bar_status_found = TRUE;
    }
    else
    {
      vplmn_cell_bar_status = *cell_bar_status;
      vplmn_cell_bar_status_found = TRUE;
    }

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"NWS: ACQ Status mask : %d for MCC:MNC - %d:%d",
                                                             tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx-1].status_mask,
                                                             TDSRRC_CSP_GET_INT_MCC(tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx-1].plmn_id),
                                                             TDSRRC_CSP_GET_INT_MNC(tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx-1].plmn_id));

    while((shared_plmn_idx < TDSMAX_SHARED_PLMNS) &&(shared_plmn_idx < tdsrrc_csp_int_data.curr_nw_db_entry.num_of_shared_plmns))
    {
      lai.plmn = tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx].plmn_id;

      if(!acq_entry_ptr->is_valid_nw_sharing)
      {
        sib_idx_plmn_id = lai.plmn;
      }
      (void)tdsrrccsp_find_forbid_lai(&lai, &lai_status);

      /* Below code is added to skip calling tdsrrccsp_find_cell_bar_status again since we have alredy called it once
       * cell bar status gives different results based on hplmn and non-hplmn cases i.e the reason it
       * has to be called twice in the worst case */

      if(tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn(lai.plmn))
      {
        if(!hplmn_cell_bar_status_found)
        {
          /* Generate Cell barred status based on SIB info for this cell */
          if(tdsrrccsp_find_cell_bar_status(lai.plmn, 
                                          acq_entry_ptr->cell_id, 
                                          cell_bar_status
                                          ,sib_idx_plmn_id
                                          ) 
             == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure to fill Cell Barred Status");
            return(FAILURE);
          }
          hplmn_cell_bar_status = *cell_bar_status;
          hplmn_cell_bar_status_found = TRUE;
        }
        else
        {
          *cell_bar_status = hplmn_cell_bar_status;
        }
      }
      else
      {
        if(!vplmn_cell_bar_status_found)
        {
          /* Generate Cell barred status based on SIB info for this cell */
          if(tdsrrccsp_find_cell_bar_status(lai.plmn, 
                                          acq_entry_ptr->cell_id, 
                                          cell_bar_status
                                          ,sib_idx_plmn_id
                                          ) 
             == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure to fill Cell Barred Status");
            return(FAILURE);
          }
          vplmn_cell_bar_status = *cell_bar_status;
          vplmn_cell_bar_status_found = TRUE;
        }
        else
        {
          *cell_bar_status = vplmn_cell_bar_status;
        }
      }

      tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx++].status_mask 
                                                          = tdsrrccsp_gen_status_mask(*cell_bar_status, lai_status);

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"NWS: ACQ Status mask : %d for MCC:MNC - %d:%d",
                                         tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx-1].status_mask,
                                         TDSRRC_CSP_GET_INT_MCC(tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx-1].plmn_id),
                                         TDSRRC_CSP_GET_INT_MNC(tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx-1].plmn_id));
    }

    /* hplmn_cell_bar_status may be different from vplmn_cell_bar_status*/
    if(hplmn_cell_bar_status_found == TRUE)
    {
      *cell_bar_status = hplmn_cell_bar_status;
    }
  }
  return(SUCCESS);
} /* tdsrrccsp_fill_acq_entry_from_sib */


/*===========================================================================

FUNCTION          RRC_CSP_SEND_LLC_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for all LLC (non-funneling) commands sent by CSP. 
                  This also allocates cmd buffer and sends the command.

DEPENDENCIES
                  None.

RETURN VALUE
                  SUCCESS: Command was successfully sent to LLC.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type 
tdsrrccsp_send_llc_cmd(tdsrrc_cmd_e_type  llc_cmd_id)
{

  tdsrrc_cmd_type        *csp_out_cmd_ptr;

  /* Send the command */
  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Populate the cmd data */
    csp_out_cmd_ptr->cmd_hdr.cmd_id = llc_cmd_id;

    switch(llc_cmd_id)
    {
      case TDSRRC_RESET_L1_REQ:
        /* No command data defined so far */
        csp_out_cmd_ptr->cmd.rrc_reset_l1_req.reset_l1_cnf_needed = FALSE;
        if(tdsrrc_csp_int_data.csp_waiting_for_wl1_to_release_lock)
        {
          csp_out_cmd_ptr->cmd.rrc_reset_l1_req.reset_l1_cnf_needed = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: CSP waiting for RESET_L1_CNF");
        }
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: RRC LLC cmd 0x%x not supported",llc_cmd_id);
        /* Call the RRC function to release the Internal Buffer */
        tdsrrc_free_cmd_buf(csp_out_cmd_ptr);
        return(FAILURE);
      }
    /* Send the Internal cmd to RRC */
    tdsrrc_put_int_cmd( csp_out_cmd_ptr );
    return(SUCCESS);
  }
  else
  {
     TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Unable to get cmd buf for cmd 0x%x",llc_cmd_id);
     return(FAILURE);
  }
} /* tdsrrccsp_send_llc_cmd */


/*===========================================================================

FUNCTION RRCCSP_SEND_CPHY_CELL_BAR_REQ

DESCRIPTION

  This function is called by CSP, SIB, and RCE procedures to send TDSL1_CPHY_CELL_BAR_REQ
  to L1 so that the corresponding cell or frequency or active set can be barred.
  
DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrccsp_send_cphy_cell_bar_req 
(
  tdsrrc_csp_cell_bar_info_e_type cell_bar_info,
  uint32 t_barred
)
{

  tdsrrc_csp_acq_entry_type acq_entry = tdsrrc_csp_int_data.curr_acq_entry;
  tdsrrc_csp_substate_e_type curr_csp_substate = tdsrrccsp_get_csp_substate();
  tdsrrc_csp_int_data.cell_bar_info = cell_bar_info;
  tdsrrc_csp_int_data.tds_cell_time_to_reselect = t_barred;

  /* If n/w sel mode is SYS_NETWORK_SELECTION_MODE_LIMITED then we 
   * do not bar any cells. It is like T-barred feature, FEATURE_T_BARRED_SUPPORT,
   * is NOT defined.
   */
  if((tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED) 
     && !(tdsrrc_csp_int_data.bar_for_resel_only)
  )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cell bar req NOT sent to L1,as n/w sel mode is %d",tdsrrc_csp_int_data.network_select_mode);
    tdsrrc_csp_int_data.cell_reserved_for_operator_use = FALSE;
    return;
  } 

  /* Check if TDS cell/frequency needs to be barred.
   * Populate acq_entry with appropriate frequency and scrambling code
   */
  if((tdsrrc_csp_int_data.cell_bar_info != TDSRRC_CSP_GSM_CELL_BARRED)
#ifdef FEATURE_TDSCDMA_TO_LTE
     &&(tdsrrc_csp_int_data.cell_bar_info != TDSRRC_CSP_LTE_CELL_BARRED)
     &&(tdsrrc_csp_int_data.cell_bar_info != TDSRRC_CSP_LTE_FREQ_BARRED)
#endif
    )
  { 
    /* Determine if the neighboring cell is barred or the serving
     * cell is barred. 
     * Note: We could not use scan type TDSRRC_CSP_CELL_RESELECTION_SCAN
     * because on completion of cell reselection, the scan type is not 
     * reset to TDSRRC_CSP_CELL_SELECTION_SCAN.
     */
    if((curr_csp_substate == TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB)
       || (curr_csp_substate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB))
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell/Freq Bar Req to be sent during CSP resel state %d for freq %d, cpid %d", 
                                        curr_csp_substate, tdsrrc_csp_int_data.new_cell_acq_entry.freq, 
                                        tdsrrc_csp_int_data.new_cell_acq_entry.cpid);
      acq_entry.freq     = tdsrrc_csp_int_data.new_cell_acq_entry.freq;
      acq_entry.cpid = tdsrrc_csp_int_data.new_cell_acq_entry.cpid;

      if(!tdsrrccsp_is_inter_frequency_cell_resel_in_progress() 
         && (tdsrrc_csp_int_data.cell_bar_info == RRC_CSP_TDS_FREQ_BARRED)
         && !(tdsrrc_csp_int_data.bar_for_resel_only)
        )
      {
        if(tdsrrcrce_is_rce_procedure_inactive() && (tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Serving Freq Barred. Update CCM with cell loss");
          tdsrrcccm_update_with_cell_loss();
        }
      }
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell/Freq Bar Req to be sent during CSP state %d for freq %d, cpid %d", 
                                          curr_csp_substate, tdsrrc_csp_int_data.curr_acq_entry.freq, 
                                          tdsrrc_csp_int_data.curr_acq_entry.cpid);

      acq_entry.freq     = tdsrrc_csp_int_data.curr_acq_entry.freq;
      acq_entry.cpid = tdsrrc_csp_int_data.curr_acq_entry.cpid;

      if(tdsrrcrce_is_rce_procedure_inactive() && (tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED) && (t_barred != 0))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Serving CPID or Freq Barred. Update CCM with cell loss");
        tdsrrcccm_update_with_cell_loss();
      }
    }
  }

  if(tdsrrccsp_send_l1_cmd(TDSRRC_PROCEDURE_CSP, TDSL1_CPHY_CELL_BAR_REQ, &acq_entry) == SUCCESS)
  {
    TDSRRC_MSG1(MSG_LEGACY_MED,"L1 cmd 0x%x sent",TDSL1_CPHY_CELL_BAR_REQ);
  }
  else
  {
    ERR_FATAL("RRC Unable to get cmd buf for L1 cmd 0x%x",TDSL1_CPHY_CELL_BAR_REQ,0,0);
  }

} /* tdsrrccsp_send_cphy_cell_bar_req */


/*===========================================================================

FUNCTION          RRC_CSP_SEND_L1_FREQ_SCAN_CMD

DESCRIPTION       This function sends a TDSL1_CPHY_FREQ_SCAN_REQ to L1 with a scan
                  type as passed to it in the parameter. 
           
DEPENDENCIES
                  None
RETURN VALUE
                  SUCCESS/FAILURE

SIDE EFFECTS
                  None

===========================================================================*/
static 	uecomdef_status_e_type
tdsrrccsp_send_l1_freq_scan_cmd
(
 tdsrrc_proc_e_type            proc_id,
 tdsrrc_freq_type              freq,
 tdsrrc_csp_freq_scan_e_type   type_freq_scan
)
{
  tdsrrc_cmd_type  *csp_out_cmd_ptr;
  uint16 end_ctr_freq;

  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl1_req_cmd * csp_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&csp_out_cmd_ptr->cmd.rrc_llc_req.l1_req, &csp_cmd_l1_req);

    /* Initialize the LLC command header first */
    csp_out_cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
#ifdef FEATURE_DUAL_SIM
	csp_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.sys_modem_as_id = tdsrrc_get_as_id();
#endif
    csp_out_cmd_ptr->cmd.rrc_llc_req.procedure = proc_id;
    csp_out_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;

    /* Now fill the L1 cmd header */
    csp_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_FREQ_SCAN_REQ; 
    csp_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = 
                                                TDSL1_ACTIVATION_TIME_NONE;

    /* Populate the cmd data */

    if(type_freq_scan == TDSRRC_CSP_RAW_SCAN)
    {
      csp_cmd_l1_req->freq_scan.step
                                            = TDSL1_FREQ_RAW_SCAN;
      csp_cmd_l1_req->freq_scan.
        u.raw_scan_parm.start_ctr_freq      = freq;

      switch(tdsrrc_csp_curr_select_data.current_band)
      {
        case RRC_CSP_TDS_A_BAND:
          csp_cmd_l1_req->freq_scan.band = TDSL1_FREQ_BAND_A;
          end_ctr_freq = TDSL1_BAND_A_CHAN_MAX;
          break;

        case RRC_CSP_TDS_F_LOWER_BAND:
          csp_cmd_l1_req->freq_scan.band = TDSL1_FREQ_BAND_F;
          end_ctr_freq = TDSL1_BAND_F_CHAN_FS_LOWER_MAX;
          break;

        case RRC_CSP_TDS_F_UPPER_BAND:
          csp_cmd_l1_req->freq_scan.band = TDSL1_FREQ_BAND_F;
          end_ctr_freq = TDSL1_BAND_F_CHAN_MAX;
          break;

        case RRC_CSP_TDS_E_BAND:
          csp_cmd_l1_req->freq_scan.band = TDSL1_FREQ_BAND_E;
          end_ctr_freq = tdsrrccsp_get_band_e_upper_bound();
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Frequency band %d not supported",tdsrrc_csp_curr_select_data.current_band);
          /* set end freq as start freq */
          end_ctr_freq = freq; 
          break;
      }

      /* Use sub band for BPLMN only */
      if((TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress() &&
          ((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) ||
           (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))) ||
         (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) ||
         (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN))
      {
        csp_cmd_l1_req->freq_scan.u.raw_scan_parm.end_ctr_freq = 
          ((freq + TDSRRC_CSP_SUB_BAND_GAP - 1) > end_ctr_freq) ? end_ctr_freq : (freq + TDSRRC_CSP_SUB_BAND_GAP - 1);
      }
      else
      {
        csp_cmd_l1_req->freq_scan.u.raw_scan_parm.end_ctr_freq = end_ctr_freq;
      }

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Starting raw scan from UARFCN %d to %d",freq,csp_cmd_l1_req->freq_scan.u.raw_scan_parm.end_ctr_freq);

    }
    else if(type_freq_scan == TDSRRC_CSP_FINE_SCAN)
    {
      csp_cmd_l1_req->freq_scan.step = TDSL1_FREQ_FINE_SCAN;
      csp_cmd_l1_req->freq_scan.
        u.fine_scan_parm.ctr_freq      =   freq;

      /* Send the band information to L1 */
      switch(tdsrrc_csp_curr_select_data.current_band)
      {
        case RRC_CSP_TDS_A_BAND:
          csp_cmd_l1_req->freq_scan.band = TDSL1_FREQ_BAND_A;
          break;

        case RRC_CSP_TDS_F_LOWER_BAND:
        case RRC_CSP_TDS_F_UPPER_BAND:
          csp_cmd_l1_req->freq_scan.band = TDSL1_FREQ_BAND_F;
          break;

        case RRC_CSP_TDS_E_BAND:
          csp_cmd_l1_req->freq_scan.band = TDSL1_FREQ_BAND_E;
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR Invalid current_band 0x%x",tdsrrc_csp_curr_select_data.current_band);
          break;

      }
    } /* TDSRRC_CSP_FINE_SCAN */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: Freq scan type %d not handled here");
    }

    /* Indicate the srch type requested */
    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) ||
       (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)    || 
       (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) || 
       (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
    {
      csp_cmd_l1_req->freq_scan.srch = TDSL1_BPLMN_SRCH_TYPE;
    }
    else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN)
    {
      csp_cmd_l1_req->freq_scan.srch = TDSL1_MPLMN_LIST_SRCH_TYPE;
    }
    else
    {
      csp_cmd_l1_req->freq_scan.srch = TDSL1_SERVICE_SRCH_TYPE;
    }

    /* Send the Internal RRC Command - LLC should directly handle it */
    tdsrrc_put_int_cmd( csp_out_cmd_ptr );

    return(SUCCESS);   
  } /* Got buffer to send cmd */
  else /* Couldn't get buffer to send cmd */
  {
     TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: RRC Unable to get cmd buf for L1 cmd 0x%x",TDSL1_CPHY_FREQ_SCAN_REQ);
     return(FAILURE);
  } 
} /* tdsrrccsp_send_l1_freq_scan_cmd */

/*===========================================================================

FUNCTION          RRC_CSP_FILL_TCRMAX_PARAMS

DESCRIPTION       This function is responsible for filling up the tcr max pameters
                   for both HCS and non HCS  

DEPENDENCIES
                  The passed pointer should be pointing to a valid data.

RETURN VALUE
                  Function returns filled ncr, tcrmaxhyst and tcrmax.

SIDE EFFECTS
                  None
===========================================================================*/
static void tdsrrccsp_fill_tcrmax_params
(
  tdsrrc_T_CRMax *t_CR_Max_ptr,
  tdsl1_t_cr_max_enum_type   *t_crmax_ptr,
  uint16      *n_cr_ptr,
  tdsl1_t_cr_max_hyst_enum_type   *t_crmaxhyst_ptr
)
{
  switch(t_CR_Max_ptr->t)
  {
    case T_tdsrrc_T_CRMax_t30:
      
      (*t_crmax_ptr) = TDSL1_T_CR_MAX_30S;

      (*n_cr_ptr) = (uint16) t_CR_Max_ptr->u.t30->n_CR;
    
          
      (*t_crmaxhyst_ptr) = RRCCSP_RET_HCS_TCR_MAX_HYST(t_CR_Max_ptr->u.t30->t_CRMaxHyst);        
                
      break;
    
    case T_tdsrrc_T_CRMax_t60:
      
      (*t_crmax_ptr) =  TDSL1_T_CR_MAX_60S;

      (*n_cr_ptr) = (uint16)  t_CR_Max_ptr->u.t60->n_CR;
      
          
      (*t_crmaxhyst_ptr) = RRCCSP_RET_HCS_TCR_MAX_HYST(t_CR_Max_ptr->u.t60->t_CRMaxHyst);        
              
      break;

    case T_tdsrrc_T_CRMax_t120:
      
      (*t_crmax_ptr) = TDSL1_T_CR_MAX_120S;

      (*n_cr_ptr) = (uint16) t_CR_Max_ptr->u.t120->n_CR;
      
          
      (*t_crmaxhyst_ptr) = RRCCSP_RET_HCS_TCR_MAX_HYST(t_CR_Max_ptr->u.t120->t_CRMaxHyst);        
              
      break;

    case T_tdsrrc_T_CRMax_t180:
      
      (*t_crmax_ptr) = TDSL1_T_CR_MAX_180S;

      (*n_cr_ptr) = (uint16) t_CR_Max_ptr->u.t180->n_CR;
      
          
      (*t_crmaxhyst_ptr) = RRCCSP_RET_HCS_TCR_MAX_HYST(t_CR_Max_ptr->u.t180->t_CRMaxHyst);        
              
      break;

    case T_tdsrrc_T_CRMax_t240:
      
      (*t_crmax_ptr) = TDSL1_T_CR_MAX_240S;

      (*n_cr_ptr) = (uint16)t_CR_Max_ptr->u.t240->n_CR;
      
          
      (*t_crmaxhyst_ptr) = RRCCSP_RET_HCS_TCR_MAX_HYST(t_CR_Max_ptr->u.t240->t_CRMaxHyst);        
              
      break;  

    default:
      /* should not happen */    
      break;
  }
}

/*===========================================================================

FUNCTION          tdsrrccsp_fill_cell_parm_data

DESCRIPTION       This function fills in cell selection and reselection data
                  for CPHY_CELL_PARM_REQ command.

DEPENDENCIES
                  The passed pointer should be pointing to a valid data.

RETURN VALUE
                  Function returns filled cell_parm_req.

SIDE EFFECTS
                  None
===========================================================================*/
static uecomdef_status_e_type tdsrrccsp_fill_cell_parm_data
(
   tdsl1_cell_parm_cmd_type * cell_parm_req,
   tdsrrc_csp_acq_entry_type        * acq_ptr
)
{
   tdsrrc_SysInfoType3                   * sib3_ptr;
   tdsrrc_SysInfoType4                   * sib4_ptr;
   tdsrrc_CellSelectReselectInfoSIB_3_4  * sib_3_4_sel_resel_info_ptr;
   tdsrrc_SysInfoType11                  * sib11_ptr;
   tdsrrc_SysInfoType12                  * sib12_ptr;
   tdsrrc_RAT_TDD_InfoList               * rat_list_ptr;
   tdsrrc_state_e_type                     rrc_state;
   uecomdef_status_e_type                  ret = FAILURE;
   boolean                                 use_sib4 = FALSE;
   uint16                                  i = 0;


   /* IE Extensions */
   tdsrrc_MappingFunctionParameterList *sib_3_4_mapping_v4b0ext_ptr = NULL;
   tdsrrc_CellSelectReselectInfo_v590ext *sib_3_4_sel_resel_v590ext_ptr = NULL;
   tdsrrc_CellSelectReselectInfoPCHFACH_v5b0ext *sib4_pch_fach_resel_v5b0ext_ptr = NULL;
   tdsrrc_CellSelectReselectInfoTreselectionScaling_v5c0ext *sib_3_4_sel_resel_v5c0ext_ptr = NULL;
   /* Skip v670 extension for DSAC */
   tdsrrc_SysInfoType3_v770ext_IEs *sib3_v770ext_ptr = NULL;
   
   if ((cell_parm_req == NULL) || (acq_ptr == NULL))
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"tdsrrccsp_fill_cell_parm_data: got null ptr parameter");
      return ret;
   }
   
   rrc_state = tdsrrc_get_state();
   TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC STATE = %d",rrc_state);
   /*  Derive the target state based on the scan, so that correct state specific parameters will be used*/
   if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN) ||
      (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN))
   {
      rrc_state = TDSRRC_STATE_CELL_FACH;
   }
   else if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN) ||
            (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN))
   {
      rrc_state = TDSRRC_STATE_CELL_PCH;
   }
   else if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN) ||
            (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN))
   {
      rrc_state = TDSRRC_STATE_URA_PCH;
   }
   else if (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
   {
      if((rrc_state == TDSRRC_STATE_CELL_DCH) && (tdsrrc_csp_int_data.dch_tx_dest_state != TDSRRC_STATE_MAX))
      {
         rrc_state = tdsrrc_csp_int_data.dch_tx_dest_state;
      }
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      else if((rrc_state == TDSRRC_STATE_CELL_FACH) && (tdsrrc_csp_int_data.fach_tx_dest_state != TDSRRC_STATE_MAX))
      {
         rrc_state = tdsrrc_csp_int_data.fach_tx_dest_state;
      }
#endif
   }

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Target STATE = %d",rrc_state);

   /* PLMN and Cell ID of the current camped on cell are known. Get SIB3 */
   if(acq_ptr->is_valid_nw_sharing)
   {
      sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrc_sibdb_return_sib_for_cell(
                                         tdsrrc_SIB3, 
                                         acq_ptr->shared_list_of_plmns.common_plmn_id, 
                                         acq_ptr->cell_id);
   }
   else
   {
      sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrc_sibdb_return_sib_for_cell(
                                         tdsrrc_SIB3, 
                                         acq_ptr->plmn_id, 
                                         acq_ptr->cell_id);
   }

   sib11_ptr = (tdsrrc_SysInfoType11*) tdsrrcsibdb_return_sib_for_srv_cell(
                                      tdsrrc_SIB11);

   sib12_ptr = (tdsrrc_SysInfoType12*) tdsrrcsibdb_return_sib_for_srv_cell(
                                      tdsrrc_SIB12);

   if( sib3_ptr == NULL )
   {
      /* SIB3 for this PLMN, Cell ID combination not present. 
       */
     if(acq_ptr->is_valid_nw_sharing)
     {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB3 for MCC %d, MNC %d, Cell ID %d not present", 
                 TDSRRC_CSP_GET_INT_MCC(acq_ptr->shared_list_of_plmns.common_plmn_id),
                 TDSRRC_CSP_GET_INT_MNC(acq_ptr->shared_list_of_plmns.common_plmn_id), 
                 acq_ptr->cell_id);
     }
     else
     {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB3 for MCC %d, MNC %d, Cell ID %d not present", 
                  TDSRRC_CSP_GET_INT_MCC(acq_ptr->plmn_id),
                  TDSRRC_CSP_GET_INT_MNC(acq_ptr->plmn_id), 
                  acq_ptr->cell_id);
     }

     return ret;
   }

   /* Determine whether to use SIB4 */
   use_sib4 = FALSE;
   if((rrc_state != TDSRRC_STATE_DISCONNECTED) && (rrc_state != TDSRRC_STATE_CONNECTING))
   {
      if(sib3_ptr->sib4indicator == TRUE)
      {
         sib4_ptr = (tdsrrc_SysInfoType4*) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB4);
         if(sib4_ptr == NULL)
         {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB4 indicated but not present,using SIB3");
         }
         else
         {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB4 is used");
            use_sib4 = TRUE;
         }
      }
   }            

   /* Get extension references first */
   /* Make SIB4 complement to SIB3 according to systems team */
   { /* SIB3 first in any case */
      sib_3_4_sel_resel_info_ptr = &(sib3_ptr->cellSelectReselectInfo);

      if (sib3_ptr->m.v4b0NonCriticalExtensionsPresent)
      {
         if (sib3_ptr->v4b0NonCriticalExtensions.sysInfoType3_v4b0ext.m.mapping_LCRPresent)
         {
            sib_3_4_mapping_v4b0ext_ptr = 
            &(sib3_ptr->v4b0NonCriticalExtensions.sysInfoType3_v4b0ext.mapping_LCR.mappingFunctionParameterList);
         }

         if (sib3_ptr->v4b0NonCriticalExtensions.
               m.v590NonCriticalExtensionPresent)
         {
            if (sib3_ptr->v4b0NonCriticalExtensions.
                  v590NonCriticalExtension.sysInfoType3_v590ext.m.cellSelectReselectInfo_v590extPresent)
            {
               sib_3_4_sel_resel_v590ext_ptr =
                  &(sib3_ptr->v4b0NonCriticalExtensions.
                     v590NonCriticalExtension.sysInfoType3_v590ext.cellSelectReselectInfo_v590ext);
            }

            if (sib3_ptr->v4b0NonCriticalExtensions.
                  v590NonCriticalExtension.
                     m.v5c0NoncriticalExtensionPresent)
            {
               if (sib3_ptr->v4b0NonCriticalExtensions.
                     v590NonCriticalExtension.
                        v5c0NoncriticalExtension.sysInfoType3_v5c0ext.m.cellSelectReselectInfoTreselectionScaling_v5c0extPresent)
               {
                  sib_3_4_sel_resel_v5c0ext_ptr =
                     &(sib3_ptr->v4b0NonCriticalExtensions.
                        v590NonCriticalExtension.
                           v5c0NoncriticalExtension.sysInfoType3_v5c0ext.cellSelectReselectInfoTreselectionScaling_v5c0ext);
               }

               if (sib3_ptr->v4b0NonCriticalExtensions.
                     v590NonCriticalExtension.
                        v5c0NoncriticalExtension.
                           m.v670NonCriticalExtensionPresent)
               {
                  /* Skip R6 extension for DSAC */
                  if (sib3_ptr->v4b0NonCriticalExtensions.
                        v590NonCriticalExtension.
                           v5c0NoncriticalExtension.
                              v670NonCriticalExtension.
                                 m.v770NonCriticalExtensionPresent)
                  {
                     sib3_v770ext_ptr =
                        &(sib3_ptr->v4b0NonCriticalExtensions.
                           v590NonCriticalExtension.
                              v5c0NoncriticalExtension.
                                 v670NonCriticalExtension.
                                    v770NonCriticalExtension.sysInfoType3_v770ext);
                  }

                  /* Stop parsing R8 and later extensions */
               }
            }
         }
      }
   }

   if (use_sib4)
   {
      sib_3_4_sel_resel_info_ptr = &(sib4_ptr->cellSelectReselectInfo);
      
      if (sib4_ptr->m.v4b0NonCriticalExtensionsPresent)
      {
         if (sib4_ptr->v4b0NonCriticalExtensions.sysInfoType4_v4b0ext.m.mapping_LCRPresent)
         {
            sib_3_4_mapping_v4b0ext_ptr = 
               &(sib4_ptr->v4b0NonCriticalExtensions.sysInfoType4_v4b0ext.mapping_LCR.mappingFunctionParameterList);
         }

         if (sib4_ptr->v4b0NonCriticalExtensions.
               m.v590NonCriticalExtensionPresent)
         {
            if (sib4_ptr->v4b0NonCriticalExtensions.
                  v590NonCriticalExtension.sysInfoType4_v590ext.m.cellSelectReselectInfo_v590extPresent)
            {
               sib_3_4_sel_resel_v590ext_ptr = 
                  &(sib4_ptr->v4b0NonCriticalExtensions.
                     v590NonCriticalExtension.sysInfoType4_v590ext.cellSelectReselectInfo_v590ext);
            }

            if (sib4_ptr->v4b0NonCriticalExtensions.
                  v590NonCriticalExtension.
                     m.v5b0NonCriticalExtensionPresent)
            {
               if (sib4_ptr->v4b0NonCriticalExtensions.
                     v590NonCriticalExtension.
                        v5b0NonCriticalExtension.sysInfoType4_v5b0ext.m.cellSelectReselectInfoPCHFACH_v5b0extPresent)
               {
                  sib4_pch_fach_resel_v5b0ext_ptr = 
                     &(sib4_ptr->v4b0NonCriticalExtensions.
                        v590NonCriticalExtension.
                           v5b0NonCriticalExtension.sysInfoType4_v5b0ext.cellSelectReselectInfoPCHFACH_v5b0ext);
               }

               if (sib4_ptr->v4b0NonCriticalExtensions.
                     v590NonCriticalExtension.
                        v5b0NonCriticalExtension.
                           m.v5c0NonCriticalExtensionPresent)
               {
                     if (sib4_ptr->v4b0NonCriticalExtensions.
                           v590NonCriticalExtension.
                              v5b0NonCriticalExtension.
                                 v5c0NonCriticalExtension.sysInfoType4_v5c0ext.m.cellSelectReselectInfoTreselectionScaling_v5c0extPresent)
                     {
                        sib_3_4_sel_resel_v5c0ext_ptr = 
                           &(sib4_ptr->v4b0NonCriticalExtensions.
                                v590NonCriticalExtension.
                                   v5b0NonCriticalExtension.
                                      v5c0NonCriticalExtension.sysInfoType4_v5c0ext.cellSelectReselectInfoTreselectionScaling_v5c0ext);
                     }
               }
            }
         }
      }
   }

   /* Set default quantity */
   cell_parm_req->quan_type = TDSL1_MEAS_NONE;

   /* Fill in Cell Select Quality Measure specific fields */
   if(sib11_ptr != NULL)
   {
      if(((rrc_state != TDSRRC_STATE_DISCONNECTED) && (rrc_state != TDSRRC_STATE_CONNECTING)) && (sib12_ptr != NULL))
      {
         /* Connected mode and SIB12 available */
         /* Use cell select quality measure from SIB12 (if present) */
         if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib12_ptr->measurementControlSysInfo.use_of_HCS,
                                         rrc_MeasurementControlSysInfo_use_of_HCS_hcs_not_used))
         {
            switch(sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.t)
            {
               /* Valid for TDD in P-CCPCH RSCP 10.3.7.47 */
               case T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_cpich_RSCP: /* Fall through */
               case T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_cpich_Ec_N0:
                  cell_parm_req->quan_type = TDSL1_RSCP;
                  break;

               default:
                  TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Invalid value %d of Cel Sel Qual Meas in SIB",sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.t);
                  break;
            }
         }
         else if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib12_ptr->measurementControlSysInfo.use_of_HCS,
                                              rrc_MeasurementControlSysInfo_use_of_HCS_hcs_used))
         {
            switch(sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t)
            {
               /* Valid for TDD in P-CCPCH RSCP 10.3.7.47 */
               case T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_cpich_RSCP: /* Fall through */
               case T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_cpich_Ec_N0:
                  cell_parm_req->quan_type = TDSL1_RSCP;
                  break;

               default:
                  TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid value %d of Cel Sel Qual Meas in SIB",sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t);
                  break;
            }
         }
         else
         {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid value %d of use_of_HCS in SIB12",sib12_ptr->measurementControlSysInfo.use_of_HCS.t);                
         }
      }
      else
      {
         /* Use cell select quality measure from SIB11 */
         if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib11_ptr->measurementControlSysInfo.use_of_HCS,
                                         rrc_MeasurementControlSysInfo_use_of_HCS_hcs_not_used))
         {
            switch(sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.t)
            {
               /* Valid for TDD in P-CCPCH RSCP 10.3.7.47 */
               case T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_cpich_RSCP: /* Fall through */
               case T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_cpich_Ec_N0:
                  cell_parm_req->quan_type = TDSL1_RSCP;
                  break;

               default:
                  TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid value %d of Cel Sel Qual Meas in SIB",sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->cellSelectQualityMeasure.t);
                  break;
            }
         }
         else if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib11_ptr->measurementControlSysInfo.use_of_HCS,
                                              rrc_MeasurementControlSysInfo_use_of_HCS_hcs_used))
         {
            switch(sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t)
            {
               /* Valid for TDD in P-CCPCH RSCP 10.3.7.47 */
               case T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_cpich_RSCP: /* Fall through */
               case T_tdsrrc_MeasurementControlSysInfo_cellSelectQualityMeasure_cpich_Ec_N0:
                  cell_parm_req->quan_type = TDSL1_RSCP;
                  break;

               default:
                  TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid value %d of Cel Sel Qual Meas in SIB",sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->cellSelectQualityMeasure.t);
                  break;
            }
         }
         else
         {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid value %d of use_of_HCS in SIB11",sib11_ptr->measurementControlSysInfo.use_of_HCS.t);
         }
      }
   } /* Fill in cell select quality from SIB11/SIB12 */
   else
   { /* Fill in the cell select quality measure from SIB3 (should not happen for TDD) */
      switch(sib_3_4_sel_resel_info_ptr->cellSelectQualityMeasure.t)
      {
         case T_tdsrrc_CellSelectReselectInfoSIB_3_4_cellSelectQualityMeasure_cpich_Ec_N0: /* Fall through */
         /* ASN.1 had t = 2 empty. Put RSCP here anyway */
         case T_tdsrrc_CellSelectReselectInfoSIB_3_4_cellSelectQualityMeasure_cpich_RSCP:
            cell_parm_req->quan_type = TDSL1_RSCP;
            break;

         default:
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid value %d of Cel Sel Qual Meas in SIB",sib_3_4_sel_resel_info_ptr->cellSelectQualityMeasure.t);
            break;
      }
   }

   /* CellSelectReselectInfoSIB-3-4. No Mapping info and Cell selection and 
    * reselection quality measure for TDD. 
    */

   /* CellSelectReselectInfoSIB-3-4. TDD mode specific fields */
   if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib_3_4_sel_resel_info_ptr->modeSpecificInfo,
                                   rrc_CellSelectReselectInfoSIB_3_4_modeSpecificInfo_tdd))
   {
      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd,
                                                s_Intrasearch))
      {
         cell_parm_req->s_intrasrch_incl = TRUE;
         if(sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd->s_Intrasearch < 0)
         {
            cell_parm_req->s_intrasrch = 0;
         }
         else
         {
            cell_parm_req->s_intrasrch = 
              (int16)((sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd->s_Intrasearch * 2) + 1);
         }
      }
      else
      {
         cell_parm_req->s_intrasrch_incl = FALSE;
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd,
                                                s_Intersearch))
      {
         cell_parm_req->s_intersrch_incl = TRUE;
         if(sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd->s_Intersearch < 0)
         {
            cell_parm_req->s_intersrch = 0;
         }
         else
         {
            cell_parm_req->s_intersrch = 
              (int16)((sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd->s_Intersearch * 2) + 1);
         }
      }
      else
      {
         cell_parm_req->s_intersrch_incl = FALSE;
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd,
                                                s_SearchHCS))
      {
         cell_parm_req->s_srchhcs_incl = TRUE;
         if(sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd->s_SearchHCS < 0)
         {
            cell_parm_req->s_srchhcs = 0;
         }
         else
         {
            cell_parm_req->s_srchhcs = 
                (int16)((sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd->s_SearchHCS * 2) + 1);
         }
      }
      else
      {
         cell_parm_req->s_srchhcs_incl = FALSE;
      }

      /* Fill the Inter-RAT information here */
      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd,
                                                rat_List))
      {
         uint32 idx = 0;
         
         cell_parm_req->s_interrat_incl = TRUE;
         rat_list_ptr = &(sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd->rat_List);
         while(rat_list_ptr->n > idx)
         {
            switch(rat_list_ptr->elem[idx].rat_Identifier)
            {
               case tdsrrc_RAT_Identifier_cdma2000: 
                  idx++;
                  continue;
                              
               case tdsrrc_RAT_Identifier_gsm:
               {
                  cell_parm_req->interrat_info[i].rat_type = TDSL1_GSM_SYSTEM;
            
                  cell_parm_req->interrat_info[i].s_interrat_absent = FALSE;
                  if(rat_list_ptr->elem[idx].s_SearchRAT < 0)
                  {
                     cell_parm_req->interrat_info[i].s_interrat = 0;
                  }
                  else if(rat_list_ptr->elem[idx].s_SearchRAT == 45)
                  {
                     /* Actual value is 91 as absent in TDD */
                     cell_parm_req->interrat_info[i].s_interrat_absent = TRUE;
                     cell_parm_req->interrat_info[i].s_interrat = 0;
                  }
                  else
                  {
                     cell_parm_req->interrat_info[i].s_interrat =
                        (int16)((rat_list_ptr->elem[idx].s_SearchRAT * 2) + 1);
                  }
            
                  if(rat_list_ptr->elem[idx].s_Limit_SearchRAT < 0)
                  {
                     cell_parm_req->interrat_info[i].s_limit_srchrat = 0;
                  }
                  else
                  {
                     cell_parm_req->interrat_info[i].s_limit_srchrat =
                        (int16)((rat_list_ptr->elem[idx].s_Limit_SearchRAT * 2) + 1);
                  }
            
                  if(TDSRRC_MSG_LIST_BITMASK_IE(rat_list_ptr,tdsrrc_RAT_TDD_Info,s_HCS_RAT))
                  {
                     cell_parm_req->interrat_info[i].s_hcsrat_incl = TRUE;
                     if(rat_list_ptr->elem[idx].s_HCS_RAT < 0)
                     {
                        cell_parm_req->interrat_info[i].s_hcsrat = 0;
                     }
                     else
                     {
                        cell_parm_req->interrat_info[i].s_hcsrat =
                           (int16)((rat_list_ptr->elem[idx].s_HCS_RAT * 2) + 1);
                     }
                  }
                  else 
                  {
                     cell_parm_req->interrat_info[i].s_hcsrat_incl = FALSE;
                  }

		  /* workaround for CR383396. TL1 does not check s_interrat_absent,
                   TL1 requires s_interrat_incl set to FALSE, if s_interrat is not
                   included for GSM RAT */
                  if (cell_parm_req->interrat_info[i].s_interrat_absent == TRUE)
		  {
                     cell_parm_req->s_interrat_incl = FALSE;
          	     TDSRRC_MSG0(MSG_LEGACY_HIGH,"cell_parm_req->s_interrat_incl set to FALSE due to GSM s_interrat absent");
		  }

                  idx++;
                  i++;
                  break;
               }
               
               default: 
                  idx++;
                  break;
            }
         }
      }
      else
      {
         cell_parm_req->s_interrat_incl = FALSE;
      }

      /* Set the num_rats here */
      cell_parm_req->num_rats = i;

      cell_parm_req->q_rxlevmin =
            (int16)((sib_3_4_sel_resel_info_ptr->modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1);

      /* R4 Mapping LCR parameters */
      cell_parm_req->mapping_lcr_included = FALSE;
      if (sib_3_4_mapping_v4b0ext_ptr != NULL)
      {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"Mapping LCR: %d items in the MappingFunctionParameterList",sib_3_4_mapping_v4b0ext_ptr->n);

         if (sib_3_4_mapping_v4b0ext_ptr->n > 0)
         {
            int16 idx = 0;

            cell_parm_req->mapping_lcr_included = TRUE;

            /* ZZTBD: FDD design only takes the first element in the MappingFunctionParameterList
             * TDD will do the same for now.
             */
            cell_parm_req->mapping_lcr.num_intvl = 1;
            idx = cell_parm_req->mapping_lcr.num_intvl - 1;

            switch (sib_3_4_mapping_v4b0ext_ptr->elem[idx].functionType)
            {
               case tdsrrc_MappingFunctionType_linear:
                  cell_parm_req->mapping_lcr.intvl[idx].func_type = TDSL1_MAP_FUN_LINEAR;
                  break;
                  
               case tdsrrc_MappingFunctionType_functionType2:
                  cell_parm_req->mapping_lcr.intvl[idx].func_type = TDSL1_MAP_FUN_TYPE_2;
                  break;
                  
               case tdsrrc_MappingFunctionType_functionType3:
                  cell_parm_req->mapping_lcr.intvl[idx].func_type = TDSL1_MAP_FUN_TYPE_3;
                  break;
                  
               case tdsrrc_MappingFunctionType_functionType4:
                  cell_parm_req->mapping_lcr.intvl[idx].func_type = TDSL1_MAP_FUN_TYPE_4;
                  break;
                  
               default:
                  cell_parm_req->mapping_lcr.intvl[idx].func_type = TDSL1_MAP_FUN_TYPE_MAX;
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Mapping LCR: unknown functiontype %d",sib_3_4_mapping_v4b0ext_ptr->elem[idx].functionType);
                  break;
            }

            /* Default to 0 according to 10.3.2.5 */
            cell_parm_req->mapping_lcr.intvl[idx].map_parm_1 = 0;
            if (sib_3_4_mapping_v4b0ext_ptr->elem[idx].m.mapParameter1Present)
            {
               cell_parm_req->mapping_lcr.intvl[idx].map_parm_1 = 
                  sib_3_4_mapping_v4b0ext_ptr->elem[idx].mapParameter1;

               /* ZZTBD: in R7 LCR, it indicates the uplink access location of the
                * serving cell. Not sure how to map to ASN.1 corresponding field yet. 
                */
            }

            cell_parm_req->mapping_lcr.intvl[idx].map_parm_2 = 
               sib_3_4_mapping_v4b0ext_ptr->elem[idx].mapParameter2;

            cell_parm_req->mapping_lcr.intvl[idx].up_limit_incl = FALSE;
            if (sib_3_4_mapping_v4b0ext_ptr->elem[idx].m.upperLimitPresent)
            {
               /* ZZTBD: Special conditions given in 10.3.2.5. 
                * Use straightforward approach for now.
                */
               cell_parm_req->mapping_lcr.intvl[idx].up_limit_incl = TRUE;
               cell_parm_req->mapping_lcr.intvl[idx].up_limit = 
                  sib_3_4_mapping_v4b0ext_ptr->elem[idx].upperLimit;
            }
         }
      }

      if((sib_3_4_sel_resel_v590ext_ptr != NULL) && (
            TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_3_4_sel_resel_v590ext_ptr,
                                                   deltaQrxlevmin)))
      {
         TDSRRC_MSG3(MSG_LEGACY_HIGH,"Serving Delta-Qrxlev=%d Qrxlev=%d, Sum=%d",
              (sib_3_4_sel_resel_v590ext_ptr->deltaQrxlevmin * 2),
              cell_parm_req->q_rxlevmin,
              (cell_parm_req->q_rxlevmin + sib_3_4_sel_resel_v590ext_ptr->deltaQrxlevmin * 2));

         cell_parm_req->q_rxlevmin +=
            (int16) (sib_3_4_sel_resel_v590ext_ptr->deltaQrxlevmin * 2);          
      }

      /* Assign default value of QrxlevminOffset to 0 */
      cell_parm_req->q_rxlevmin_offset = 0;
      if ((sib3_v770ext_ptr != NULL) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib3_v770ext_ptr, 
                                                  q_RxlevMin_Offset)))
      {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"Serving QrxlevMin-Offset=%d",sib3_v770ext_ptr->q_RxlevMin_Offset);
         cell_parm_req->q_rxlevmin_offset = sib3_v770ext_ptr->q_RxlevMin_Offset;
      }
   } /* End of TDD specific mode fields */

   /* Fill in generic fields */
   cell_parm_req->qhyst1_s = (uint16) (sib_3_4_sel_resel_info_ptr->q_Hyst_l_S * 2);
   if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) && (sib4_pch_fach_resel_v5b0ext_ptr != NULL))
   {
      if(((rrc_state == TDSRRC_STATE_CELL_PCH) || (rrc_state == TDSRRC_STATE_URA_PCH)) &&
         (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib4_pch_fach_resel_v5b0ext_ptr,
                                                 q_Hyst_l_S_PCH)))
      {
         cell_parm_req->qhyst1_s = (uint16) sib4_pch_fach_resel_v5b0ext_ptr->q_Hyst_l_S_PCH;  
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"updated q_Hyst_l_S for PCH = %d",cell_parm_req->qhyst1_s);      
      }  
      else if((rrc_state == TDSRRC_STATE_CELL_FACH) &&
              (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib4_pch_fach_resel_v5b0ext_ptr,
                                                      q_Hyst_l_S_FACH)))
      {
         cell_parm_req->qhyst1_s = (uint16) sib4_pch_fach_resel_v5b0ext_ptr->q_Hyst_l_S_FACH;
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"updated q_Hyst_l_S for FACH = %d",cell_parm_req->qhyst1_s);
      }
   } 

   cell_parm_req->trselc_ms = (uint16) sib_3_4_sel_resel_info_ptr->t_Reselection_S * 1000;
   TDSRRC_MSG1(MSG_LEGACY_HIGH,"t_Reselection_S in ms = %d",cell_parm_req->trselc_ms);
   if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) && (sib4_pch_fach_resel_v5b0ext_ptr != NULL))
   {
      if(((rrc_state == TDSRRC_STATE_CELL_PCH) || (rrc_state == TDSRRC_STATE_URA_PCH)) &&
         (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib4_pch_fach_resel_v5b0ext_ptr,
                                                 t_Reselection_S_PCH)))
      {
         cell_parm_req->trselc_ms = (uint16) sib4_pch_fach_resel_v5b0ext_ptr->t_Reselection_S_PCH * 1000;   
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"updated t_Reselection_S in ms for PCH = %d",cell_parm_req->trselc_ms);     
      }  
      else if((rrc_state == TDSRRC_STATE_CELL_FACH) &&
              (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib4_pch_fach_resel_v5b0ext_ptr,
                                                      t_Reselection_S_FACH)))
      {
         cell_parm_req->trselc_ms = (uint16) ((sib4_pch_fach_resel_v5b0ext_ptr->t_Reselection_S_FACH * 0.2) * 1000);
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"updated t_Reselection_S in ms for FACH = %d",cell_parm_req->trselc_ms);
      }
   }


   /* HCS Processing Serving cell info assign default values if not present */
   cell_parm_req->hcs_info_incl = TRUE;
   cell_parm_req->hcs_info.hcs_prio = 0;
        
   cell_parm_req->hcs_info.q_hcs = 115;

   if(cell_parm_req->quan_type != TDSL1_RSCP)
   {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDD got %d quan_type not TDSL1_RSCP",cell_parm_req->quan_type);
   }
        
   cell_parm_req->hcs_info.n_cr = 8;
   cell_parm_req->hcs_info.t_crmax = TDSL1_T_CR_MAX_NOT_USED;
   cell_parm_req->hcs_info.t_crmaxhyst = TDSL1_T_CR_MAX_HYST_NOT_USED;        
        
   if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_3_4_sel_resel_info_ptr,
                                             hcs_ServingCellInformation))
   {      
      cell_parm_req->hcs_info.hcs_prio =
         (uint16)sib_3_4_sel_resel_info_ptr->hcs_ServingCellInformation.hcs_PRIO;       
                  
      if(sib_3_4_sel_resel_info_ptr->hcs_ServingCellInformation.q_HCS < 90)
      {
         cell_parm_req->hcs_info.q_hcs =
            (uint16) (115 - sib_3_4_sel_resel_info_ptr->hcs_ServingCellInformation.q_HCS);

         TDSRRC_MSG1(MSG_LEGACY_HIGH,"RSCP serving cell qhcs = %d",cell_parm_req->hcs_info.q_hcs); 
      }
      else
      {
         cell_parm_req->hcs_info.q_hcs = TDSMAX_QHCS_RSCP;
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"RSCP serving cell qhcs spare value set to %d",cell_parm_req->hcs_info.q_hcs);
      }

      if((cell_parm_req->quan_type == TDSL1_RSCP) &&
         (sib_3_4_sel_resel_v590ext_ptr != NULL) && 
         (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_3_4_sel_resel_v590ext_ptr, deltaQhcs)))
      {
         TDSRRC_MSG3(MSG_LEGACY_HIGH,"Serving Delta-Qhcs=%d, Qhcs=%d, Sum=%d",
                  sib_3_4_sel_resel_v590ext_ptr->deltaQhcs,
                  cell_parm_req->hcs_info.q_hcs,
                  (cell_parm_req->hcs_info.q_hcs + (-sib_3_4_sel_resel_v590ext_ptr->deltaQhcs)));

         cell_parm_req->hcs_info.q_hcs += (uint16) (-sib_3_4_sel_resel_v590ext_ptr->deltaQhcs);          
      }
        
      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib_3_4_sel_resel_info_ptr->hcs_ServingCellInformation,
                                            t_CR_Max))
      {
         /* ZZTBD: assume the following algorithm is same for TDD */
         tdsrrccsp_fill_tcrmax_params(&sib_3_4_sel_resel_info_ptr->hcs_ServingCellInformation.t_CR_Max,
                                    &cell_parm_req->hcs_info.t_crmax,
                                    &cell_parm_req->hcs_info.n_cr,
                                    &cell_parm_req->hcs_info.t_crmaxhyst);
      } 
   }
   else /* HCS-ServingCellInformation not included */
   {
      /* TDS L1 requests exact info about if HCS is included */
      cell_parm_req->hcs_info_incl = FALSE;
   }

   cell_parm_req->max_tx_pwr = (int16)sib_3_4_sel_resel_info_ptr->maxAllowedUL_TX_Power;       

   cell_parm_req->nhcs_n_cr = 8;
   cell_parm_req->nhcs_t_crmax = TDSL1_T_CR_MAX_NOT_USED;
   cell_parm_req->nhcs_t_crmaxhyst = TDSL1_T_CR_MAX_HYST_NOT_USED;
   cell_parm_req->tresel_scale = 10;
   cell_parm_req->tresel_interf_scale = 4;
   cell_parm_req->tresel_irat_scale = 4;
   if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) && (sib_3_4_sel_resel_v5c0ext_ptr != NULL))
   {
      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_3_4_sel_resel_v5c0ext_ptr,speedDependentScalingFactor))
      {
         cell_parm_req->tresel_scale = 
              (uint16) sib_3_4_sel_resel_v5c0ext_ptr->speedDependentScalingFactor;
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_3_4_sel_resel_v5c0ext_ptr,interFrequencyTreselectionScalingFactor))
      {
         cell_parm_req->tresel_interf_scale = 
              (uint16) sib_3_4_sel_resel_v5c0ext_ptr->interFrequencyTreselectionScalingFactor;
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_3_4_sel_resel_v5c0ext_ptr,interRATTreselectionScalingFactor))
      {
         cell_parm_req->tresel_irat_scale =
              (uint16) sib_3_4_sel_resel_v5c0ext_ptr->interRATTreselectionScalingFactor;
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib_3_4_sel_resel_v5c0ext_ptr,non_HCS_t_CR_Max))
      {
         tdsrrccsp_fill_tcrmax_params(&sib_3_4_sel_resel_v5c0ext_ptr->non_HCS_t_CR_Max,
                                    &cell_parm_req->nhcs_t_crmax,
                                    &cell_parm_req->nhcs_n_cr,
                                    &cell_parm_req->nhcs_t_crmaxhyst);      
      }
   }

   TDSRRC_MSG3(MSG_LEGACY_HIGH,"nhcs_t_crmax = %d, nhcs_n_cr = %d, nhcs_t_crmaxhyst = %d",
            cell_parm_req->nhcs_t_crmax,
            cell_parm_req->nhcs_n_cr,
            cell_parm_req->nhcs_t_crmaxhyst);

   TDSRRC_MSG3(MSG_LEGACY_HIGH,"tresel_scale = %d, tresel_interf_scale = %d, tresel_irat_scale = %d",
            cell_parm_req->tresel_scale,
            cell_parm_req->tresel_interf_scale,
            cell_parm_req->tresel_irat_scale); 

   /* Keep the following S-value calculation to TDD from FDD */
   if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_CELL_RESELECTION_SCAN) ||
      (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_CC_ORDER_SCAN))
   {
#ifdef FEATURE_RF
      tdsrrc_csp_int_data.tds_cell_srxlevmin =
         cell_parm_req->q_rxlevmin + MAX( (cell_parm_req->max_tx_pwr - RFUMTS_UE_MAX_TX_POWER), 0 );
#else
      /* Should not come here. But just for completeness assigning the q_rxlevmin
       * value here
       */
      tdsrrc_csp_int_data.tds_cell_srxlevmin = cell_parm_req->q_rxlevmin; 

#endif  /* FEATURE_RF */
   }

   /* Reset old PFR info first */
   memset(&tdsrrc_csp_int_data.pseudo_fr_info, 0x00, sizeof(tdsrrc_csp_interrat_pseudo_fr_info_type));

   if (TDSCHECK_OPTIONAL_FEATURE_ENABLE_PFR)
   {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PFR is enabled by NV");
      tdsrrccsp_store_pseudo_fr_info(sib3_ptr);
   }
   else
   {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PFR is disabled by NV");
   }
   

   ret = SUCCESS;
   return ret;
}

/*===========================================================================

FUNCTION          RRC_CSP_SEND_L1_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for all L1 commands sent by CSP. This also allocates cmd
                  buffer and sends the command.
                  Since all L1 cmds go via LLC, the LLC command TDSRRC_LLC_CMD_REQ
                  actaully encapsules the real L1 command. This is the LLC
                  command funneling mechanism.
                  It frees allocated command and embedded buffers
                  when failure is detected before sending the command.

DEPENDENCIES
                  The passed pointer should be pointing to a valid data.

RETURN VALUE
                  SUCCESS: Command was successfully sent to L1.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type 
tdsrrccsp_send_l1_cmd(tdsrrc_proc_e_type  proc_id,
                    tdsl1_cmd_enum_type l1_cmd_id,
                    tdsrrc_csp_acq_entry_type  *acq_entry_ptr)
{
  tdsrrc_cmd_type        *csp_out_cmd_ptr;
  uecomdef_status_e_type  ret = SUCCESS;
  tdsrrclog_rrccsp_acq_start_event_type acq_start_event;

  /* Check validity of Acq Entry pointer */
  if(acq_entry_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: Passed Ptr Union has NULL pointers");
    return(FAILURE);
  }

  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl1_req_cmd * csp_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&csp_out_cmd_ptr->cmd.rrc_llc_req.l1_req, &csp_cmd_l1_req);
    /* Initialize the LLC command header first */
    csp_out_cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
#ifdef FEATURE_DUAL_SIM
	csp_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.sys_modem_as_id = tdsrrc_get_as_id();
#endif
    csp_out_cmd_ptr->cmd.rrc_llc_req.procedure = proc_id;
    csp_out_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;

    /* Now fill the L1 cmd header */
    csp_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = l1_cmd_id; 
    csp_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = 
                                                TDSL1_ACTIVATION_TIME_NONE;
   
     /* Fill up the L1 cmd data */
    switch(l1_cmd_id)
    {
      case TDSL1_CPHY_ACQ_REQ:
      {
        TDSRRC_DEBUG_INCREMENT_STATS(TDSRRC_ACQ_TRIGGERED);
	    TDSRRC_MSG1(MSG_LEGACY_HIGH,"acq_fs_status when send ACQ_REQ: %d",tdsrrc_csp_int_data.acq_fs_status); 
        csp_cmd_l1_req->acq.acq_fs_status = tdsrrc_csp_int_data.acq_fs_status;
        /* If current acq_fs_status is TDSL1_ACQ_FS_START, means this is the first time
         * to send ACQ_REQ during frequency scan.  Then change it to TDSL1_ACQ_FS_CONTINUE. 
         */
        
        if (tdsrrc_csp_int_data.acq_fs_status == TDSL1_ACQ_FS_START)
        {
           tdsrrc_csp_int_data.acq_fs_status = TDSL1_ACQ_FS_CONTINUE;
        } 
#ifdef FEATURE_TDSCDMA_ACQ_ENHANCEMENT
        if (tdsrrc_csp_int_data.acq_fs_status == TDSL1_ACQ_FS_STOP)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send last ACQ_REQ with 0 freq");
           csp_cmd_l1_req->acq.num_freqs = 0;
        }
        else
#endif		
        {
           csp_cmd_l1_req->acq.num_freqs = 1;
        }
        //EVENT: Event_Acquisiton_Start
        acq_start_event.acq_mode = tdsrrc_csp_int_data.acq_mode;
        acq_start_event.num_freq = csp_cmd_l1_req->acq.num_freqs;
        acq_start_event.first_freq = acq_entry_ptr->freq;
		acq_start_event.last_freq = acq_entry_ptr->freq;

        event_report_payload(EVENT_TDSCDMA_RRC_ACQUISITION_START, sizeof(acq_start_event), (void *)&acq_start_event);

        /* This is for single frequency acquisition */
        csp_cmd_l1_req->acq.acq_mode = tdsrrc_csp_int_data.acq_mode;
		/* Will be set to TRUE only for RRC_CSP_GSM_TO_TDS_PSHO_SCAN */
		csp_cmd_l1_req->acq.is_ho = FALSE;
        csp_cmd_l1_req->acq.is_irat = FALSE;
#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
		if (tdsrrc_csp_int_data.acq_mode == TDSL1_ACQ_MODE_IRAT_TIMING)
		{
          csp_cmd_l1_req->acq.rscp = acq_entry_ptr->rscp;
		  csp_cmd_l1_req->acq.freq_timing_list[0].uarfcn = acq_entry_ptr->freq;
		  csp_cmd_l1_req->acq.freq_timing_list[0].lna_state = acq_entry_ptr->lna_state;
		  csp_cmd_l1_req->acq.freq_timing_list[0].ts0_position = acq_entry_ptr->ts0_position;
          csp_cmd_l1_req->acq.freq_timing_list[0].num_of_jds_cells = acq_entry_ptr->num_of_jds_cells;
		  memscpy(csp_cmd_l1_req->acq.freq_timing_list[0].cell_list,TDSIRAT_MAX_TDS_CELLS_PER_FREQ*sizeof(uint8),acq_entry_ptr->cell_list,acq_entry_ptr->num_of_jds_cells*sizeof(uint8));
		}
		else
#endif
		{
          csp_cmd_l1_req->acq.freq_list[0] = acq_entry_ptr->freq;
		}

        if (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN) 
        {// Same for TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN?
          if (tdsrrc_csp_int_data.redirected_cpid < TDSRRC_CSP_INVALID_CPID)
          {
            csp_cmd_l1_req->acq.single_cell = TRUE;
            csp_cmd_l1_req->acq.cpi = tdsrrc_csp_int_data.redirected_cpid;
          }
        }
        else if ((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN)
                 || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN)
                 || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN)
                 || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN)
                 || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN)
                 || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN))
        {
          if (tdsrrc_csp_int_data.dest_cpid_present && (tdsrrc_csp_int_data.dest_cpid < TDSRRC_CSP_INVALID_CPID))
          {
             if (tdsrrc_csp_curr_select_data.num_tries_on_dest_freq)
             {
                csp_cmd_l1_req->acq.single_cell = TRUE;
                csp_cmd_l1_req->acq.cpi = tdsrrc_csp_int_data.dest_cpid;
             }
             else
             {
                /* 1 less try then TDSRRC_CSP_MAX_ACQ_TRIES_ON_DIRECTED_FREQ */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"CPHY_ACQ_REQ(legacy): skip single cell ACQ"); 
             }
           }
        }
        else if (tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_CELL_RESELECTION_SCAN)
        {
            /* For both G2TD and L2TD reselection */
            csp_cmd_l1_req->acq.single_cell = TRUE;
            csp_cmd_l1_req->acq.cpi = tdsrrc_csp_int_data.curr_acq_entry.cpid;
            csp_cmd_l1_req->acq.is_irat = TRUE;
        }
#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
		else if (tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_PSHO_SCAN)
		{
            /* For both G2TD and L2TD CCO/PSHO */
            csp_cmd_l1_req->acq.single_cell = TRUE;
            csp_cmd_l1_req->acq.cpi = tdsrrc_csp_int_data.curr_acq_entry.cpid;
			csp_cmd_l1_req->acq.is_ho = TRUE;
		}
#endif

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"CPHY_ACQ_REQ: is_ho = %d",csp_cmd_l1_req->acq.is_ho);
        
        /* If n/w sel mode is SYS_NETWORK_SELECTION_MODE_LIMITED then do not consider 
         * T-barred timer started for a frequency. N/w sel mode as SYS_NETWORK_SELECTION_MODE_LIMITED
         * implies that UE is searching for service for Emergency Call only (or UE does not
         * have a USIM) and will not attempt registration on the camped cell.
         */
        if(tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED)
        {
          csp_cmd_l1_req->acq.unbar_freqs = TRUE;
        }
        else
        {
          csp_cmd_l1_req->acq.unbar_freqs = FALSE;
        }
        
        TDSRRC_MSG5(MSG_LEGACY_HIGH,"CPHY_ACQ_REQ(legacy): mode %d, unbar %d, freq %d, cmcc nv rd %d, test %d", 
                  csp_cmd_l1_req->acq.acq_mode, 
                  (int) csp_cmd_l1_req->acq.unbar_freqs, 
                  acq_entry_ptr->freq,
                  tdsrrc_special_settings_nv_read,
                  tdsrrc_special_settings_for_testing_nv);

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"CPHY_ACQ_REQ(legacy): curr_scan %d, req_plmn_type %d, nw_sel_mode %d", 
                 tdsrrc_csp_int_data.curr_scan,
                 tdsrrc_csp_int_data.curr_req_plmn.plmn_type,
                 tdsrrc_csp_int_data.network_select_mode);

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"CPHY_ACQ_REQ(legacy): single_cell acq %d, cpid %d, is_irat %d", 
                  csp_cmd_l1_req->acq.single_cell, 
                  csp_cmd_l1_req->acq.cpi,
                  csp_cmd_l1_req->acq.is_irat);

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"CPHY_ACQ_REQ(legacy): special_freq_enabled=%d, special_freq=%d, use_special_freq_for_acq=%d", 
                  tdsrrc_csp_int_data.tds_special_freq_enabled, 
                  tdsrrc_csp_int_data.tds_special_freq,
                  tdsrrc_csp_int_data.use_special_freq_for_acq);

        /* TDD: set LFS flag for ACQ_CNF check */
        if (tdsrrc_csp_int_data.tds_continue_lfs)
        {
           TDSRRC_MSG0(MSG_LEGACY_ERROR,"CPHY_ACQ_REQ(legacy): tds_continue_lfs is still TRUE. Make it to FALSE");
           tdsrrc_csp_int_data.tds_continue_lfs = FALSE;
        }
        break;        
      }

      case TDSL1_CPHY_CELL_PARM_REQ:
        ret = tdsrrccsp_fill_cell_parm_data(&(csp_cmd_l1_req->cell_parm), acq_entry_ptr);
        if (ret == FAILURE)
        {
          /* Call the RRC function to release the Internal Buffer */
          tdsrrc_free_cmd_buf(csp_out_cmd_ptr);
          return ret;
        }

        break;

      case TDSL1_CPHY_CELL_SELECTION_REQ:
        csp_cmd_l1_req->cell_sel.freq = 
                            acq_entry_ptr->freq;
        csp_cmd_l1_req->cell_sel.prim_code_inx =
                            (uint16)acq_entry_ptr->cpid;
        break;

     case TDSL1_CPHY_CELL_TRANSITION_REQ:
       csp_cmd_l1_req->cell_transition.select_type = TDSL1_ASET_UPD_CELL_CAMP;
       csp_cmd_l1_req->cell_transition.req_mask  = 0;

       csp_cmd_l1_req->cell_transition.freq_incl = TRUE;
       csp_cmd_l1_req->cell_transition.rf_freq = (uint16)acq_entry_ptr->freq;
       csp_cmd_l1_req->cell_transition.cpi = (uint8)acq_entry_ptr->cpid;

       csp_cmd_l1_req->cell_transition.dl_cctrch_info    = NULL;
       csp_cmd_l1_req->cell_transition.dl_ctfc_info      = NULL;
       csp_cmd_l1_req->cell_transition.add_dl_phychan_db = NULL;
       csp_cmd_l1_req->cell_transition.ul_cctrch_info    = NULL;
       csp_cmd_l1_req->cell_transition.add_ul_phychan_db = NULL;
       csp_cmd_l1_req->cell_transition.tfcs_ptr          = NULL;

       csp_cmd_l1_req->cell_transition.tg_freq_excl = FALSE;
       csp_cmd_l1_req->cell_transition.tg_cell_excl = FALSE;
       break;

      case TDSL1_CPHY_CELL_BAR_REQ:
        csp_cmd_l1_req->cell_bar_req.t_barred = 
                                            (uint16)tdsrrc_csp_int_data.tds_cell_time_to_reselect;

        csp_cmd_l1_req->cell_bar_req.bar_for_resel_only = 
                                                                                  tdsrrc_csp_int_data.bar_for_resel_only;
        csp_cmd_l1_req->cell_bar_req.non_eq_plmn_barred = 
                                                                                  tdsrrc_csp_int_data.non_eq_plmn_barred;

        TDSRRC_MSG2(MSG_LEGACY_HIGH,"CR151: bar_for_resel_only is %d non-eq %d",tdsrrc_csp_int_data.bar_for_resel_only,tdsrrc_csp_int_data.non_eq_plmn_barred);
        tdsrrc_csp_int_data.non_eq_plmn_barred=FALSE;

        tdsrrc_csp_int_data.cell_reserved_for_operator_use = FALSE;
        switch(tdsrrc_csp_int_data.cell_bar_info)
        {
          case TDSRRC_CSP_GSM_CELL_BARRED:
            csp_cmd_l1_req->cell_bar_req.l1_cell_bar_info = 
                                                                        TDSL1_GSM_CELL_BARRED;
            break;

          case RRC_CSP_TDS_CPID_BARRED:
            csp_cmd_l1_req->cell_bar_req.l1_cell_bar_info = 
                                                                        TDSL1_TDS_CPID_BARRED;
            break;

          case RRC_CSP_TDS_FREQ_BARRED:
            csp_cmd_l1_req->cell_bar_req.l1_cell_bar_info = 
                                                                        TDSL1_TDS_FREQ_BARRED;
            break;

          case RRC_CSP_TDS_ASET_BARRED:
            csp_cmd_l1_req->cell_bar_req.l1_cell_bar_info = 
                                                                        TDSL1_TDS_ASET_BARRED;
            break;
#ifdef FEATURE_TDSCDMA_TO_LTE
          case TDSRRC_CSP_LTE_CELL_BARRED:
            csp_cmd_l1_req->cell_bar_req.l1_cell_bar_info = TDSL1_EUTRA_CELL_BARRED;
            break;

          case TDSRRC_CSP_LTE_FREQ_BARRED:
            csp_cmd_l1_req->cell_bar_req.l1_cell_bar_info = TDSL1_EUTRA_FREQ_BARRED;
            break;
#endif
          default:
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR Incorrect cell bar info %d",tdsrrc_csp_int_data.cell_bar_info);
            /* We do not want to send wrong cell bar request to L1 */
            return (SUCCESS);
        }
        
        /* Check if TDS cell/frequency needs to be barred */
        if((tdsrrc_csp_int_data.cell_bar_info != TDSRRC_CSP_GSM_CELL_BARRED)
#ifdef FEATURE_TDSCDMA_TO_LTE
           &&(tdsrrc_csp_int_data.cell_bar_info != TDSRRC_CSP_LTE_CELL_BARRED)
           &&(tdsrrc_csp_int_data.cell_bar_info != TDSRRC_CSP_LTE_FREQ_BARRED)
#endif
          )
        {
          csp_cmd_l1_req->cell_bar_req.id.tdd.freq = (uint16)acq_entry_ptr->freq;
          csp_cmd_l1_req->cell_bar_req.id.tdd.cpid = (uint8)acq_entry_ptr->cpid;

          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Cell bar req sent to L1. cell_bar_reason: %d,T-barred tmr: %d",csp_cmd_l1_req->cell_bar_req.l1_cell_bar_info,csp_cmd_l1_req->cell_bar_req.t_barred);

          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Cell bar Freq: %d,Cell bar cpid: %d",csp_cmd_l1_req->cell_bar_req.id.tdd.freq,csp_cmd_l1_req->cell_bar_req.id.tdd.cpid);
        }
        /* GSM cell needs to be barred */
        else if (tdsrrc_csp_int_data.cell_bar_info == TDSRRC_CSP_GSM_CELL_BARRED)
        {
          /* Check if GSM cell is being barred due to TD2G Reselection Failure
           * or TD2TD Cell Change Order Failure
           */

          /* TD2TD Cell Change Order Failed bcoz G cell is barred*/
          if(tdsrrccho_is_td2g_cco_active())
          {
            tdsrrccho_fill_gsm_cell_info_for_cell_bar(
                &(csp_cmd_l1_req->cell_bar_req.id.gsm.bcch_arfcn),
                &(csp_cmd_l1_req->cell_bar_req.id.gsm.bsic.ncc),
                &(csp_cmd_l1_req->cell_bar_req.id.gsm.bsic.bcc));
          }
          /* TD2TD Reselection Failed */
          else
          {
            csp_cmd_l1_req->cell_bar_req.id.gsm.bcch_arfcn = 
                    tdsrrc_csp_int_data.saved_rrc_cmd.cmd.cell_rsel_ind.u.gsm_info.cell_info.bcch_arfcn;
            csp_cmd_l1_req->cell_bar_req.id.gsm.bsic.ncc   = 
                    tdsrrc_csp_int_data.saved_rrc_cmd.cmd.cell_rsel_ind.u.gsm_info.cell_info.bsic.ncc;
            csp_cmd_l1_req->cell_bar_req.id.gsm.bsic.bcc   = 
                    tdsrrc_csp_int_data.saved_rrc_cmd.cmd.cell_rsel_ind.u.gsm_info.cell_info.bsic.bcc;
          }
          
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Cell bar req sent to L1. cell_bar_reason: %d, T-barred tmr: %d, bcch_arfcn: %d",
                               tdsrrc_csp_int_data.cell_bar_info, 
                               csp_cmd_l1_req->cell_bar_req.t_barred, 
                               csp_cmd_l1_req->cell_bar_req.id.gsm.bcch_arfcn.num);

          TDSRRC_MSG2(MSG_LEGACY_HIGH,"bsic.ncc: %d,bsic.bcc: %d",csp_cmd_l1_req->cell_bar_req.id.gsm.bsic.ncc,csp_cmd_l1_req->cell_bar_req.id.gsm.bsic.bcc);
        }
#ifdef FEATURE_TDSCDMA_TO_LTE
        else
        {
          csp_cmd_l1_req->cell_bar_req.id.eutra.band_width = 
            tdsrrc_csp_int_data.saved_rrc_cmd.cmd.cell_rsel_ind.u.eutra_info.cell_info.band_width;
          csp_cmd_l1_req->cell_bar_req.id.eutra.EARFCN= 
            tdsrrc_csp_int_data.saved_rrc_cmd.cmd.cell_rsel_ind.u.eutra_info.cell_info.EARFCN;
          csp_cmd_l1_req->cell_bar_req.id.eutra.physical_cell_identity= 
            tdsrrc_csp_int_data.saved_rrc_cmd.cmd.cell_rsel_ind.u.eutra_info.cell_info.physical_cell_identity;
          MSG_5(MSG_SSID_DIAG, MSG_LEGACY_HIGH,
            "TD2L: Cell bar req sent to L1, cell_bar_reason: %d, T-barred tmr: %d, EARFCN: %d PCI: %d, BW: %d",
            csp_cmd_l1_req->cell_bar_req.l1_cell_bar_info,csp_cmd_l1_req->cell_bar_req.t_barred,
            csp_cmd_l1_req->cell_bar_req.id.eutra.EARFCN, csp_cmd_l1_req->cell_bar_req.id.eutra.physical_cell_identity, 
            csp_cmd_l1_req->cell_bar_req.id.eutra.band_width);
        }
#endif
        break;

      case TDSL1_CPHY_GO_TO_SLEEP_IND:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CPHY GO TO SLEEP IND sent to L1 %d"); 
      break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"L1 cmd 0x%x not supported",l1_cmd_id);
        /* Call the RRC function to release the Internal Buffer */
        tdsrrc_free_cmd_buf(csp_out_cmd_ptr);
        return(FAILURE);
    } /* switch(l1_cmd) */

    /* Send the Internal RRC Command - LLC should directly handle it */
    tdsrrc_put_int_cmd( csp_out_cmd_ptr );

    return(SUCCESS);   
  } /* Got buffer to send cmd */
  else /* Couldn't get buffer to send cmd */
  {
     TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRC Unable to get cmd buf for sending L1 cmd 0x%x",l1_cmd_id);
     return(FAILURE);
  } 
}   /* tdsrrccsp_send_l1_cmd */

/*===========================================================================

FUNCTION          RRC_CSP_SEND_L1_TDS_ACQ_REQ

DESCRIPTION       This function is responsible for filling up data structures
                  for all new CPHY_ACQ_REQ command defined in TDD sent by CSP. 
                  This also allocates cmd buffer and sends the command.
                  Since all L1 cmds go via LLC, the LLC command TDSRRC_LLC_CMD_REQ
                  actaully encapsules the real L1 command. This is the LLC
                  command funneling mechanism.

DEPENDENCIES
                  The passed pointer should be pointing to a valid data.

RETURN VALUE
                  SUCCESS: Command was successfully sent to L1.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_send_l1_tds_acq_req(
   tdsrrc_proc_e_type         proc_id,
   uint16                   * freq_list_ptr,
   int16                      freq_index,
   uint8                      num_freqs,
   boolean                    unbar
)
{
   tdsrrc_cmd_type * csp_out_cmd_ptr;
   tdsl1_req_cmd   * csp_cmd_l1_req = NULL;
   uint8 num_freqs_to_send = 0;
   int i = 0;

   // ZZTBD: should be limited to MAX ACQ?
   uint16 freq[TDSL1_MAX_SCAN_FREQ];
   tdsrrclog_rrccsp_acq_start_event_type acq_start_event;
   
   /* Check validity of freq_list_ptr pointer */
   if (freq_list_ptr == NULL)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: Passed freq_list_ptr is NULL");
      return(FAILURE);
   }

   if ((num_freqs > TDSL1_MAX_SCAN_FREQ) || (num_freqs == 0))
   {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: invalid number of freqs (%d) for ACQ",num_freqs);
      return(FAILURE);
   }

   if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC Unable to get cmd buf for sending CPHY_ACQ_REQ in TDS");
      return(FAILURE);
   }

   tdsrrc_get_pointer_l1_req_value(&csp_out_cmd_ptr->cmd.rrc_llc_req.l1_req, &csp_cmd_l1_req);
   /* Initialize the LLC command header first */
   csp_out_cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
   csp_out_cmd_ptr->cmd.rrc_llc_req.procedure = proc_id;
   csp_out_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;

   /* Now fill the L1 cmd header */
   csp_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_ACQ_REQ; 
   csp_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_NONE;
   TDSRRC_DEBUG_INCREMENT_STATS(TDSRRC_ACQ_TRIGGERED);
   TDSRRC_MSG5(MSG_LEGACY_HIGH,"CPHY_ACQ_REQ: mode %d, unbar %d, freqs. %d, cmcc nv rd %d, test %d", 
            tdsrrc_csp_int_data.acq_mode, 
            (int) unbar, 
            num_freqs,
            tdsrrc_special_settings_nv_read,
            tdsrrc_special_settings_for_testing_nv);
   TDSRRC_MSG3(MSG_LEGACY_HIGH,"CPHY_ACQ_REQ: curr_scan %d, req_plmn_type %d, nw_sel_mode %d", 
            tdsrrc_csp_int_data.curr_scan,
            tdsrrc_csp_int_data.curr_req_plmn.plmn_type,
            tdsrrc_csp_int_data.network_select_mode);
   TDSRRC_MSG1(MSG_LEGACY_HIGH,"acq_fs_status when send ACQ_REQ: %d",tdsrrc_csp_int_data.acq_fs_status); 
   csp_cmd_l1_req->acq.acq_fs_status = tdsrrc_csp_int_data.acq_fs_status;
   /* If current acq_fs_status is TDSL1_ACQ_FS_START, means this is the first time
    * to send ACQ_REQ during frequency scan.  Then change it to TDSL1_ACQ_FS_CONTINUE. 
    */
   if (tdsrrc_csp_int_data.acq_fs_status == TDSL1_ACQ_FS_START)
   {
      tdsrrc_csp_int_data.acq_fs_status = TDSL1_ACQ_FS_CONTINUE;
   }
#ifdef FEATURE_TDSCDMA_ACQ_ENHANCEMENT   
   if (tdsrrc_csp_int_data.acq_fs_status == TDSL1_ACQ_FS_STOP)
   {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send last ACQ_REQ with 0 freq");
      csp_cmd_l1_req->acq.num_freqs = 0;
   }
   else
#endif   
   {
      if (num_freqs > 0)
      {
         for (i = freq_index; i < num_freqs; i++)
         {
            freq[i - freq_index] = freq_list_ptr[i];
            num_freqs_to_send++;
         }

         /* Don't scan again for the freq that are already scanned */
         #if 0
         for (i = 0; i < freq_index; i++)
         {
            freq[num_freqs - freq_index + i] = freq_list_ptr[i];
            num_freqs_to_send++;
         }
         #endif
      }
      else
      {
         freq[0] = freq_list_ptr[0];
      }

      for (i = 0; ((i < num_freqs_to_send) && (i < TDSL1_MAX_ACQ_CHANNEL_NUM)); i++)
      {
         csp_cmd_l1_req->acq.freq_list[i] = freq[i];
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"CPHY_ACQ_REQ: F[%d]=%d",i,freq[i]);
      }

      csp_cmd_l1_req->acq.num_freqs = i;
      if (num_freqs != i)
      {
         TDSRRC_MSG3(MSG_LEGACY_HIGH,"CPHY_ACQ_REQ: total in acq_freq_list.freq_list %d, actual sent %d, starts at index %d", 
                     num_freqs,
                     csp_cmd_l1_req->acq.num_freqs, 
                     freq_index);
      }
   }
  
   csp_cmd_l1_req->acq.unbar_freqs = unbar;

   csp_cmd_l1_req->acq.acq_mode = tdsrrc_csp_int_data.acq_mode;

   csp_cmd_l1_req->acq.is_irat = FALSE;
   
   if(tdsrrc_csp_int_data.use_shallow_mode_in_next_acq)
   {
     if((csp_cmd_l1_req->acq.acq_mode != TDSL1_ACQ_MODE_SHALLOW) &&
       (csp_cmd_l1_req->acq.acq_mode != TDSL1_ACQ_MODE_DEEP))
     {
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"Change ACQ mode(%d) to shallow as use_shallow_mode_in_next_acq is set",csp_cmd_l1_req->acq.acq_mode);
       csp_cmd_l1_req->acq.acq_mode = TDSL1_ACQ_MODE_SHALLOW;
     }

     tdsrrc_csp_int_data.use_shallow_mode_in_next_acq = FALSE;
   }
   /* TDD: set LFS flag for ACQ_CNF check */
   tdsrrc_csp_int_data.tds_continue_lfs = TRUE;
   
   //EVENT: Event_Acquisiton_Start
   acq_start_event.acq_mode = csp_cmd_l1_req->acq.acq_mode;
   acq_start_event.num_freq = csp_cmd_l1_req->acq.num_freqs;
   acq_start_event.first_freq = csp_cmd_l1_req->acq.freq_list[0];
   if ((csp_cmd_l1_req->acq.num_freqs > 0) && (csp_cmd_l1_req->acq.num_freqs <= TDSL1_MAX_ACQ_CHANNEL_NUM)) 
   {
     acq_start_event.last_freq = csp_cmd_l1_req->acq.freq_list[csp_cmd_l1_req->acq.num_freqs-1];
   }

   event_report_payload(EVENT_TDSCDMA_RRC_ACQUISITION_START, sizeof(acq_start_event), (void *)&acq_start_event);  
   tdsrrc_log_acq_info(freq_index, num_freqs, csp_cmd_l1_req->acq.acq_mode);   
   /* Send the Internal RRC Command - LLC should directly handle it */
   tdsrrc_put_int_cmd( csp_out_cmd_ptr );

   return(SUCCESS);   
} /* tdsrrccsp_send_l1_tds_acq_req */


/*===========================================================================

FUNCTION          RRC_CSP_SEND_L1_DRX_REQ

DESCRIPTION       This function is responsible for sending TDSL1_CPHY_DRX_REQ
                  to L1.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to send TDSL1_CPHY_DRX_REQ successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
uecomdef_status_e_type tdsrrccsp_send_l1_drx_req(void)
{

  tdsrrc_cmd_type *cmd_ptr;

  /* Send DRX command to L1, no confirm to wait for */

  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl1_req_cmd * csp_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&cmd_ptr->cmd.rrc_llc_req.l1_req, &csp_cmd_l1_req);

    /* Initialize the LLC command header first */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LLC_CMD_REQ;
#ifdef FEATURE_DUAL_SIM
	cmd_ptr->cmd.rrc_llc_req.l1_cmd.sys_modem_as_id = tdsrrc_get_as_id();
#endif
    cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_CSP;
    cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
  
    /* Now fill the L1 cmd header */
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_DRX_REQ;
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_NONE;

    /* Populate the cmd data */
    csp_cmd_l1_req->drx.drx_cycle_len = tdsrrcllc_get_drx_info();
    
    /* Send the Internal RRC command - LLC should directly handle it */
    tdsrrc_put_int_cmd(cmd_ptr);
  }
  else
  {
    ERR_FATAL("Couldn't get internal command buffer",0,0,0);
  }

  return(SUCCESS);
} /*tdsrrccsp_send_l1_drx_req */

/*===========================================================================

FUNCTION          RRC_CSP_SEND_L1_DRX_REQ_FOR_MULTIPLE_DRX

DESCRIPTION       This function is responsible for sending TDSL1_CPHY_DRX_REQ
                  to L1 for multiple drx support feature.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to send TDSL1_CPHY_DRX_REQ successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
uecomdef_status_e_type tdsrrccsp_send_l1_drx_req_for_multiple_drx(void)
{

  tdsrrc_cmd_type *cmd_ptr;

  /* Send DRX command to L1, no confirm to wait for */

  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl1_req_cmd * csp_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&cmd_ptr->cmd.rrc_llc_req.l1_req, &csp_cmd_l1_req);

    /* Initialize the LLC command header first */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_LLC_CMD_REQ;
#ifdef FEATURE_DUAL_SIM
	cmd_ptr->cmd.rrc_llc_req.l1_cmd.sys_modem_as_id = tdsrrc_get_as_id();
#endif
    cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_CSP;
    cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
  
    /* Now fill the L1 cmd header */
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_DRX_REQ;
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_NONE;

    /* Populate the cmd data */
    csp_cmd_l1_req->drx.drx_cycle_len = tdsrrcllc_get_drx_info_for_multiple_drx();
    
    /* Send the Internal RRC command - LLC should directly handle it */
    tdsrrc_put_int_cmd(cmd_ptr);
  }
  else
  {
    ERR_FATAL("Couldn't get internal command buffer",0,0,0);
  }

  return(SUCCESS);
} /*tdsrrccsp_send_l1_drx_req_for_multiple_drx*/
#ifdef FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT
/*===========================================================================

FUNCTION          tdsrrccsp_send_l1_bar_cmd_with_lte_list

DESCRIPTION       This function is responsible for sending LTE bar list to L1 to avoid ping pong T2L redirection.

DEPENDENCIES
                  The passed pointer should be pointing to a valid data.

RETURN VALUE
                  SUCCESS: Command was successfully sent to L1.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type 
tdsrrccsp_send_l1_bar_cmd_with_lte_list(lte_rrc_irat_to_lte_redir_forbidden_earfcn_info_s * redir_forbidden_list_ptr, 
                                                     uint8 t_barred)
{
  tdsrrc_cmd_type        *csp_out_cmd_ptr;
  uecomdef_status_e_type  ret = SUCCESS;

  /* Check validity of Acq Entry pointer */
  if(redir_forbidden_list_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: Passed Ptr Union has NULL pointers");
    return(FAILURE);
  }

  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl1_req_cmd * csp_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&csp_out_cmd_ptr->cmd.rrc_llc_req.l1_req, &csp_cmd_l1_req);
    /* Initialize the LLC command header first */
    csp_out_cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
#ifdef FEATURE_DUAL_SIM
	csp_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.sys_modem_as_id = tdsrrc_get_as_id();
#endif
    csp_out_cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_CSP;
    csp_out_cmd_ptr->cmd.rrc_llc_req.cmd_dest  = TDSLAYER1_PHY;

    /* Now fill the L1 cmd header */
    csp_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_CELL_BAR_REQ; 
    csp_out_cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_NONE;

    csp_cmd_l1_req->cell_bar_req.t_barred = t_barred;
    csp_cmd_l1_req->cell_bar_req.bar_for_resel_only = FALSE;
    csp_cmd_l1_req->cell_bar_req.non_eq_plmn_barred = FALSE;

    tdsrrc_csp_int_data.cell_bar_info = TDSRRC_CSP_REDIR_FAIL_BARRED;
    csp_cmd_l1_req->cell_bar_req.l1_cell_bar_info   = TDSL1_TDS_REDIR_FAIL_BARRED;

    /* Check if TDS cell/frequency needs to be barred */
    if (tdsrrc_csp_int_data.cell_bar_info == TDSRRC_CSP_REDIR_FAIL_BARRED)
    {
      /* Now allocate memory to the pointer inside the command */
      if ( (csp_cmd_l1_req->cell_bar_req.forbidden_earfch_cell_list =
           (lte_rrc_irat_to_lte_redir_forbidden_earfcn_info_s *)tdsl1_mem_alloc(sizeof(lte_rrc_irat_to_lte_redir_forbidden_earfcn_info_s))) == NULL )
      {
        ERR_FATAL("Unable to get Cmd buffer",0,0,0);;
      }

      /*For debug purpose*/
      {
        uint8 earfcn_idx,cell_idx = 0;
	      for (earfcn_idx =0; earfcn_idx< redir_forbidden_list_ptr->earfcn_count; earfcn_idx++ )
	      {
          for (cell_idx =0; cell_idx < redir_forbidden_list_ptr->earfcn_info[earfcn_idx].forbidden_cells_count; cell_idx++ )
	        {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"PING PONG T2L Redir, Bar LTE cell %d of earfcn %d",
                        redir_forbidden_list_ptr->earfcn_info[earfcn_idx].forbidden_cell[cell_idx],
                        redir_forbidden_list_ptr->earfcn_info[earfcn_idx].earfcn);
	        }
	      }
      }

      memscpy(csp_cmd_l1_req->cell_bar_req.forbidden_earfch_cell_list, sizeof(lte_rrc_irat_to_lte_redir_forbidden_earfcn_info_s),
              redir_forbidden_list_ptr, sizeof(lte_rrc_irat_to_lte_redir_forbidden_earfcn_info_s));
      
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"PING PONG T2L Redir,Bar LTE list for %d seconds",csp_cmd_l1_req->cell_bar_req.t_barred);
    }

    /* Send the Internal RRC Command - LLC should directly handle it */
    tdsrrc_put_int_cmd( csp_out_cmd_ptr );

    return(SUCCESS);   
  } /* Got buffer to send cmd */
  else /* Couldn't get buffer to send cmd */
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC Unable to get cmd buf for sending L1 cmd TDSL1_CPHY_CELL_BAR_REQ");
    return(FAILURE);
  } 
}
#endif /*FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT*/
/*===========================================================================

FUNCTION          RRC_CSP_SEND_SIB_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for all RRC Internal commands to SIB procedure sent by CSP.
                  This also allocates cmd buffer and sends the command.
                  TBD: If the data going into commands is conditional on 
                  something, a mechanism will be needed to pass that condition.
                  It could be thru' devicing slightly different names for
                  the same commands with different conditional data.

DEPENDENCIES
                  The pointer to Acquisition Entry should be valid.
RETURN VALUE
                  SUCCESS: RRC Internal command was successfully sent to SIB Proc..
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type  
tdsrrccsp_send_sib_cmd(tdsrrc_cmd_e_type sib_cmd, 
                     tdsrrc_csp_sib_mask_type sib_mask, 
                     tdsrrc_csp_acq_entry_type *acq_entry_ptr,
                     boolean for_serving_cell)
{
  int i;
  tdsrrc_csp_sib_mask_type  gen_mask = TDSMIB_BIT;
  tdsrrc_cmd_type   *csp_out_cmd_ptr;

  /* Check validity of Acq Entry pointer */
  if(acq_entry_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Passed Acq Entry pointer is NULL");
    return(FAILURE);
  }

  /* Send SIB procedure an event to get SIBs */
  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Populate the cmd data */
    csp_out_cmd_ptr->cmd_hdr.cmd_id = sib_cmd;

    switch(sib_cmd)
    {
      case TDSRRC_GET_SPECIFIC_SIBS_REQ:
        /* Get Frequency and Scrambling Code from CSP Internal Data */
        csp_out_cmd_ptr->cmd.get_specific_sibs_req.freq = acq_entry_ptr->freq;
        csp_out_cmd_ptr->cmd.get_specific_sibs_req.cpid = 
            acq_entry_ptr->cpid;
        csp_out_cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell = for_serving_cell;
        csp_out_cmd_ptr->cmd.get_specific_sibs_req.procedure = TDSRRC_PROCEDURE_CSP;
        if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
        {
          csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = 
            TDSRRC_SIB_AUTO_BPLMN_SCAN;
        }
        else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
        {
          csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = 
            TDSRRC_SIB_MANUAL_BPLMN_SCAN;
        }
#ifdef FEATURE_LTE_TO_TDSCDMA
        else if ( (tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_CELL_RESELECTION_SCAN)
             &&(tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE_CGI ))
        {
          csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = 
            TDSRRC_SIB_MANUAL_BPLMN_SCAN;
        }         
#endif
        else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
        {
          csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = 
            RRC_SIB_TD2TD_AUTO_BPLMN_SCAN;
        }
        else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)  
        {
          csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = 
            RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN;
        }
        else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN)
        {
          csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = 
            TDSRRC_SIB_MANUAL_SCAN;
        }
        else if (tdsrrccsp_is_dummy_plmn_search(tdsrrc_csp_int_data.curr_req_plmn.plmn_id))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Optimizaed for dummy PLMN service request: set to TDSRRC_SIB_DUMMY_PLMN_SCAN for sibproc");
          csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = TDSRRC_SIB_DUMMY_PLMN_SCAN;
        }
        else
        {
          csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type =
            TDSRRC_SIB_NORMAL; 
        }
#ifdef FEATURE_3GPP_CSFB_TDS
        if((RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN == tdsrrc_csp_int_data.curr_scan) &&
           (TDSRRC_CSP_INTERRAT_LTE == tdsrrc_csp_int_data.interrat_type))
        {
          uint8 idx;
#ifdef FEATURE_L2TD_REDIRECT_R10
          uint8 idx_freq;
#endif
          if( (tdsrrc_csp_int_data.l2t_r9_redirect) && (acq_entry_ptr->freq == tdsrrc_csp_int_data.dest_freq) &&
             (TDSCDMA_RRC_MAX_UTRA_CONT != (idx = tdsrrccsp_get_idx_sib_cont_list(acq_entry_ptr->cpid))))
          {
            csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = TDSRRC_SIB_READ_FROM_SIB_CONTAINER;
            csp_out_cmd_ptr->cmd.get_specific_sibs_req.sys_info_cont_ptr = &tdsrrc_SIB_container.cont[idx];
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB: Set scan_type to TDSRRC_SIB_READ_FROM_SIB_CONTAINER for Rel-9");
          	}
#ifdef FEATURE_L2TD_REDIRECT_R10
		  /* rel-10 L2T redirect */
		  else if( (!tdsrrc_csp_int_data.l2t_r9_redirect) && 
		  	       (MAX_UTRAN_TDS_FREQUNECIES != (idx_freq = tdsrrccsp_is_redirect_uarfcn_with_sib_container_from_lte(acq_entry_ptr->freq))) &&
                   (TDSCDMA_RRC_MAX_UTRA_CONT != (idx = tdsrrccsp_get_idx_sib_cont_list_r10(idx_freq,acq_entry_ptr->cpid))) )
          {
            csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = TDSRRC_SIB_READ_FROM_SIB_CONTAINER;
            csp_out_cmd_ptr->cmd.get_specific_sibs_req.sys_info_cont_ptr = &tdsrrc_SIB_container.cont[idx];
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB: Set scan_type to TDSRRC_SIB_READ_FROM_SIB_CONTAINER for Rel-10");
          }
#endif
        }
#endif
        /* Indicate which SIBs are needed */
        for(i = 0; 
            i < TDSRRC_TOT_SIBS; 
            i++, gen_mask <<= 1)
        {
          if(sib_mask & gen_mask)
          {
            csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_req[i] = TRUE;
          }
          else
          {
            csp_out_cmd_ptr->cmd.get_specific_sibs_req.sib_req[i] = FALSE;
          }
        }
#ifdef FEATURE_3GPP_CSFB_TDS
        if((tdsrrc_csfb_call_status) &&
           (RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN == tdsrrc_csp_int_data.curr_scan) && 
           (TDSRRC_CSP_INTERRAT_LTE == tdsrrc_csp_int_data.interrat_type))
        {
          /*CSFB is active when camping on TDS due to redirection from LTE*/
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB: SIB cmd being sent when CSFB call is up");
          tdsrrc_csp_int_data.sib_read_csfb_active = TRUE;
        }
        else
        {
          tdsrrc_csp_int_data.sib_read_csfb_active = FALSE;
        }
#endif

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"GET_SPECIFIC_SIBS_REQ: freq %d, cpid %d, sib_mask 0x%x",
                 acq_entry_ptr->freq, acq_entry_ptr->cpid, sib_mask);
        break;

      case TDSRRC_GET_ALL_SIBS_REQ:
        csp_out_cmd_ptr->cmd.get_all_sibs_req.freq = acq_entry_ptr->freq;
        csp_out_cmd_ptr->cmd.get_all_sibs_req.cpid =
                                                 acq_entry_ptr->cpid;
        csp_out_cmd_ptr->cmd.get_all_sibs_req.procedure = TDSRRC_PROCEDURE_CCM;

        TDSRRC_MSG2(MSG_LEGACY_HIGH,"GET_ALL_SIBS_REQ: freq %d,cpid %d",acq_entry_ptr->freq,acq_entry_ptr->cpid);
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"SIB cmd 0x%x not supported",sib_cmd);
        /* Call the RRC function to release the Internal Buffer */
        tdsrrc_free_cmd_buf(csp_out_cmd_ptr);
        return(FAILURE);
      }
    /* Send the Internal cmd to RRC */
    tdsrrc_put_int_cmd( csp_out_cmd_ptr );
    return(SUCCESS);
  }
  else
  {
     TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unable to get cmd buf for cmd 0x%x",sib_cmd);
     return(FAILURE);
  }
} /* tdsrrccsp_send_sib_cmd */



uecomdef_status_e_type  
tdsrrccsp_fill_nas_common_info
(
  tdsrrc_SysInfoType1  *sib1_ptr,
  rrc_gsm_map_nas_sys_info_type  *nas_common_info_ptr
)
{
  unsigned char  *tdsdata_ptr;
  uint32          i;

  /* The SIB1 is present - lets disect it */
  nas_common_info_ptr->length =
    (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;

  /* Check if the RRC MM interface structure array can hold this info */
  if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for Common NAS Sys Info (%d bytes)",sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
    return(FAILURE);
  }

  /* Fill in the Common NAS system info now */
  tdsdata_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
  for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
  {
    nas_common_info_ptr->value[i] = *tdsdata_ptr;
    tdsdata_ptr ++;
  }

  return(SUCCESS);
} /* tdsrrccsp_fill_nas_common_info */


uecomdef_status_e_type  
tdsrrccsp_fill_nas_cn_specific_info
(
  tdsrrc_CN_DomainSysInfo *     asn1_cn_domain_sys_info_ptr,
  rrc_cn_domain_sys_info_type  *rrc_cn_domain_sys_info_ptr
)
{
  unsigned char  *tdsdata_ptr;
  uint32          i;

  if(asn1_cn_domain_sys_info_ptr->cn_DomainIdentity == tdsrrc_CN_DomainIdentity_cs_domain)
  {
    rrc_cn_domain_sys_info_ptr->cn_domain_id = RRC_CS_DOMAIN_CN_ID;
  }
  else
  {
    rrc_cn_domain_sys_info_ptr->cn_domain_id = RRC_PS_DOMAIN_CN_ID;
  }

  rrc_cn_domain_sys_info_ptr->cn_domain_specific_info.gsm_map_info.length = 
    asn1_cn_domain_sys_info_ptr->cn_Type.u.gsm_MAP->numocts;

  /* Check if the RRC MM interface structure array can hold this info */
  if(asn1_cn_domain_sys_info_ptr->cn_Type.u.gsm_MAP->numocts > 
     RRC_MAX_GSM_INFO_OCTET_COUNT)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for NAS Sys Info (%d bytes)",asn1_cn_domain_sys_info_ptr->cn_Type.u.gsm_MAP->numocts);
    return(FAILURE);
  }

  tdsdata_ptr = asn1_cn_domain_sys_info_ptr->cn_Type.u.gsm_MAP->data;

  for( i = 0; i < asn1_cn_domain_sys_info_ptr->cn_Type.u.gsm_MAP->numocts; i ++)
  {
    rrc_cn_domain_sys_info_ptr->cn_domain_specific_info.gsm_map_info.value[i] =
     *tdsdata_ptr;   

    tdsdata_ptr ++;
  } 

  return(SUCCESS);
} /* tdsrrccsp_fill_nas_cn_specific_info */


/*===========================================================================

FUNCTION          RRC_CSP_SEND_SERVICE_IND

DESCRIPTION       This function sends RRC_SERVICE_IND to MM
                  with the current cell's parameters.Currently 
                  used by CHO after a T->G Cell change order failure

DEPENDENCIES
                  None.

RETURN VALUE
                  None.
                  
SIDE EFFECTS
                  None.
===========================================================================*/
void tdsrrccsp_send_service_ind(void)
{
  /* Send Service Indication to MM */
  if(tdsrrccsp_send_mm_cmd(RRC_SERVICE_IND,
                         tdsrrc_csp_int_data.curr_service_status,
                         &tdsrrc_csp_int_data.curr_sel_plmn,
                         &tdsrrc_csp_int_data.curr_acq_entry)
         == FAILURE)
  {
    ERR_FATAL("RRC Unable to send SERVICE_IND to MM",0,0,0);
  }

  /* Update the last sent MM command in the int data for bookkeeping */
  tdsrrc_csp_int_data.last_mm_primitive = TDSRRC_CSP_SRVC_IND_WITH_SRVC_SENT;
}

/*===========================================================================

FUNCTION          tdsrrccsp_send_backup_service_ind

DESCRIPTION       This function sends RRC_SERVICE_IND to MM with backup service ind

DEPENDENCIES
                  None.

RETURN VALUE
                  None.
                  
SIDE EFFECTS
                  None.
===========================================================================*/
void tdsrrccsp_send_backup_service_ind(void)
{
  mm_cmd_type *mm_buf_ptr = NULL;

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_SERVICE_IND)) == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No MM buffer for CSP to MM cmd RRC_SERVICE_IND");
    return;
  }

#ifdef FEATURE_DUAL_SIM
	mm_buf_ptr->cmd.rrc_service_ind.as_id = tdsrrc_get_as_id();
#endif

  mm_buf_ptr->cmd.rrc_service_ind.service_status = tdsrrc_service_ind_backup.service_status;

  memscpy(&(mm_buf_ptr->cmd.rrc_service_ind.service_info), sizeof(rrc_service_info_type), 
          &(tdsrrc_service_ind_backup.service_info), sizeof(rrc_service_info_type));
          
  mm_buf_ptr->cmd.rrc_service_ind.active_band = tdsrrc_service_ind_backup.active_band;
  mm_buf_ptr->cmd.rrc_service_ind.uoos_rem_awake_time = tdsrrc_service_ind_backup.uoos_rem_awake_time;
  mm_buf_ptr->cmd.rrc_service_ind.uoos_rem_sleep_time = tdsrrc_service_ind_backup.uoos_rem_sleep_time;
  mm_buf_ptr->cmd.rrc_service_ind.is_cell_id_valid = tdsrrc_service_ind_backup.is_cell_id_valid;
  mm_buf_ptr->cmd.rrc_service_ind.cell_id = tdsrrc_service_ind_backup.cell_id;
  mm_buf_ptr->cmd.rrc_service_ind.acq_status = tdsrrc_service_ind_backup.acq_status;
  mm_buf_ptr->cmd.rrc_service_ind.no_service_cause = tdsrrc_service_ind_backup.no_service_cause;
  mm_buf_ptr->cmd.rrc_service_ind.suitable_search_time = tdsrrc_service_ind_backup.suitable_search_time;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending backup RRC_SERVICE_IND");
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  
  /* Put the command on MM queue */
  mm_put_cmd(mm_buf_ptr);
  
  /* Update the last sent MM command in the int data for bookkeeping */
  tdsrrc_csp_int_data.last_mm_primitive = TDSRRC_CSP_SRVC_IND_WITH_SRVC_SENT;
}

/*===========================================================================
FUNCTION          RRC_CSP_SEND_INTERRAT_CELL_CHANGE_IND

DESCRIPTION       This function is responsible for filling up data structures
                  for INTERRAT_CELL_CHANGE_IND to MM sent by CSP only for G2TD
                  PCCO. This also allocates cmd buffer and sends the command.
                  It also frees allocated command and embedded buffers
                  when failure is detected before sending the command.

DEPENDENCIES

RETURN VALUE
                  None.
                  
SIDE EFFECTS
                  None.
===========================================================================*/
void tdsrrccsp_send_interrat_cell_change_ind(void)
{
  tdsrrc_SysInfoType1  *sib1_ptr;
  mm_cmd_type *mm_buf_ptr;
  uint32 num_cn_domains=0,idx=0;
  tdsrrc_CN_DomainSysInfoList * cn_domain_sys_info_list_ptr;
  tdsrrc_csp_access_control_e_type   access_control;
  tdsrrc_dsac_info_type rrc_dsac_info;

  rrc_plmn_identity_type common_plmn_id;

  if(tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing)
  {
    common_plmn_id = tdsrrc_csp_int_data.curr_acq_entry.shared_list_of_plmns.common_plmn_id;
  }
  else
  {
    common_plmn_id = tdsrrc_csp_int_data.curr_acq_entry.plmn_id;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sib_idx PLMN: MCC %d-MNC %d",TDSRRC_CSP_GET_INT_MCC(common_plmn_id),TDSRRC_CSP_GET_INT_MNC(common_plmn_id));
  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_INTERRAT_CELL_CHANGE_IND)) == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No MM buffer for RRC_INTERRAT_CELL_CHANGE_IND");
    return;
  }

  /* Fill up header */
  mm_buf_ptr->cmd.rrc_interrat_cell_change_ind.service_status = MM_AS_SERVICE_AVAILABLE;
  mm_buf_ptr->cmd.rrc_interrat_cell_change_ind.service_info.selected_plmn = 
    tdsrrc_csp_int_data.curr_sel_plmn;

  if(tdsrrccsp_find_access_control(&access_control,&rrc_dsac_info,
                                 tdsrrc_csp_int_data.curr_acq_entry.plmn_id,
                                 tdsrrc_csp_int_data.curr_acq_entry.cell_id
                                 ,common_plmn_id
                                 )
     == FAILURE)
  {
    ERR_FATAL("Failure finding cell access status",0,0,0); 
  }

  /* Get the cell access status of the camped on cell */
  (void) tdsrrccsp_find_rrc_cell_access(&(mm_buf_ptr->cmd.rrc_interrat_cell_change_ind.service_info),&rrc_dsac_info, 
                                                               access_control, tdsrrc_csp_int_data.curr_acq_entry.status_mask);

  /* Fill DSAC Info valid */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC:DSAC inof is set");
  mm_buf_ptr->cmd.rrc_interrat_cell_change_ind.service_info.dsac_present = rrc_dsac_info.valid;
  
#ifdef FEATURE_DSAC
  if(rrc_dsac_info.valid == TRUE)
  {
    tdsrrc_csp_int_data.curr_cs_cell_access = mm_buf_ptr->cmd.rrc_interrat_cell_change_ind.service_info.cs_cell_access;
    tdsrrc_csp_int_data.curr_ps_cell_access = mm_buf_ptr->cmd.rrc_interrat_cell_change_ind.service_info.ps_cell_access;
    tdsrrc_csp_int_data.is_dsac_valid = rrc_dsac_info.valid;
   }
   else
   {
     tdsrrc_csp_int_data.curr_cell_access_status = mm_buf_ptr->cmd.rrc_interrat_cell_change_ind.service_info.cell_access;
   }
#else
   {
     tdsrrc_csp_int_data.curr_cell_access_status = mm_buf_ptr->cmd.rrc_interrat_cell_change_ind.service_info.cell_access;
   }
#endif

  /* Fill up the NAS system info here */
  /* Let us first read SIB1 - we should have SIB1 for sure */
  /*
    tdsrrc_csp_int_data.curr_sel_plmn.plmn_id is the same as tdsrrc_csp_int_data.curr_acq_entry.plmn_id
    So use the common_plmn_id itself
*/
  sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrc_sibdb_return_sib_for_cell(
                                            tdsrrc_SIB1,
                                            common_plmn_id, 
                                            tdsrrc_csp_int_data.curr_acq_entry.cell_id);
  if( sib1_ptr == NULL )
  {
    /* SIB3 for this PLMN, CEll ID combination not present */
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB1 for MCC %d, MNC %d, Cell ID %d not present",
                TDSRRC_CSP_GET_INT_MCC(common_plmn_id), 
                TDSRRC_CSP_GET_INT_MNC(common_plmn_id),
                                   tdsrrc_csp_int_data.curr_acq_entry.cell_id);
    mm_free_cmd_buf(mm_buf_ptr);
    return;
  } /* SIB1 not present */

  if(tdsrrccsp_fill_nas_common_info(sib1_ptr, 
                                 &mm_buf_ptr->cmd.rrc_interrat_cell_change_ind.
                                   service_info.gsm_map_common_info)
    == FAILURE)
  {
    mm_free_cmd_buf(mm_buf_ptr);
    return;
  }

  /* Fill in the CN domain specific NAS system info now */
  num_cn_domains = 0;
  cn_domain_sys_info_list_ptr =  &sib1_ptr->cn_DomainSysInfoList; 
  do
  {            
    /* First check if Core network is GSM MAP. */
    if( cn_domain_sys_info_list_ptr->elem[idx].cn_Type.t == 
                                      T_tdsrrc_CN_DomainSysInfo_cn_Type_gsm_MAP)
    {
      if(tdsrrccsp_fill_nas_cn_specific_info(&cn_domain_sys_info_list_ptr->elem[idx], 
                                          &mm_buf_ptr->cmd.rrc_interrat_cell_change_ind.
                                          service_info.cn_domain_info[num_cn_domains])
         == FAILURE)
      {
        mm_free_cmd_buf(mm_buf_ptr);
        return;
      }
      num_cn_domains ++;
    } /* If CN type is GSM MAP */ 
    idx++;

  }while((cn_domain_sys_info_list_ptr->n > idx) && 
        (num_cn_domains < RRC_MAX_CN_DOMAINS));   

  mm_buf_ptr->cmd.rrc_interrat_cell_change_ind.service_info.num_cn_domains = num_cn_domains;

  /* Put the command on MM queue */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending MM cmd INTERRAT_CELL_CHANGE_IND");
  tdscfa_mm_put_cmd(mm_buf_ptr);

  /* For now since CSP doesn't care about if INTERRAT_CELL_CHANGE_IND was sent,
   * just send the last MM primitive sent as SERVICE_IND. May need to change if
   * there dependecy on INTERRAT_CELL_CHANGE_IND
   */
  tdsrrc_csp_int_data.last_mm_primitive = 
    tdsrrccsp_mm_primitive_sent(RRC_SERVICE_IND, MM_AS_SERVICE_AVAILABLE);


} /* tdsrrccsp_send_interrat_cell_change_ind */
/*===========================================================================

FUNCTION          RRC_CSP_SEND_MM_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for all commands to MM sent by CSP.
                  This also allocates cmd buffer and sends the command.
                  It also frees allocated command and embedded buffers
                  when failure is detected before sending the command.
                  It updates the last primtive sent to MM in internal data.

DEPENDENCIES
                  
RETURN VALUE
                  SUCCESS: Command was successfully sent to MM.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type  
tdsrrccsp_send_mm_cmd(rrc_to_mm_sink_e_type  cmd_id, 
                    mm_as_service_state_T  service_status,
                    rrc_selected_plmn_type *sel_plmn_ptr,
                    tdsrrc_csp_acq_entry_type *acq_entry_ptr)
{

#ifdef FEATURE_WRLF_SYSTEM_SEL
  boolean hack_srvc_avail_for_lim_srvc = FALSE;
#endif 
  tdsrrc_SysInfoType1  *sib1_ptr;
  sys_band_mask_e_type sys_band;
  sys_hs_ind_e_type new_hs_ind;
  rrc_plmn_identity_type common_plmn_id;
  #ifdef FEATURE_DSAC
  mm_cmd_type  *mm_common_buf_ptr;
  #endif
  mm_cmd_type *mm_buf_ptr;
  uint32 num_cn_domains,idx;
  tdsrrc_state_e_type rrc_state;
  tdsrrc_CN_DomainSysInfoList * cn_domain_sys_info_list_ptr;
  tdsrrc_csp_access_control_e_type   access_control;
  tdsrrc_dsac_info_type rrc_dsac_info;
  rrc_state = tdsrrc_get_state();
#ifdef FEATURE_TDSCDMA_ACQ_ENHANCEMENT  
  /* If acq_fs_status == TDSL1_ACQ_FS_STOP, get current_band from curr_acq_entry due to current_band may be invalid*/
  if (tdsrrc_csp_int_data.acq_fs_status == TDSL1_ACQ_FS_STOP)
  {
     tdsrrc_csp_curr_select_data.current_band = tdsrrc_get_frequency_band(tdsrrc_csp_int_data.curr_acq_entry.freq);
  }
#endif  
  sys_band = tdsrrc_convert_band_to_sys_band(tdsrrc_csp_curr_select_data.current_band);
  new_hs_ind = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;

#ifdef FEATURE_DSAC
  mm_common_buf_ptr = NULL;
#endif
  mm_buf_ptr = NULL;
  num_cn_domains=0; idx=0;
  /* ZZTBD: to match the default value set in tdsrrccsp_init_int_data() */
  tdsrrc_csp_int_data.do_lfs_scan = FALSE;

  if(acq_entry_ptr->is_valid_nw_sharing)
  {
    common_plmn_id = acq_entry_ptr->shared_list_of_plmns.common_plmn_id;
  }
  else
  {
    common_plmn_id = acq_entry_ptr->plmn_id;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sib_idx PLMN: MCC %d-MNC %d",TDSRRC_CSP_GET_INT_MCC(common_plmn_id),TDSRRC_CSP_GET_INT_MNC(common_plmn_id));

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)|| defined (FEATURE_TDSCDMA_DEDICATED_PRI_INFO)
  if(RRC_SERVICE_CNF == cmd_id)
  {
    if(MM_AS_SERVICE_AVAILABLE == service_status)
    {
      /*UE camped successfull, set the owner to TDSCDMA if TDS had valid dedicated priorities*/
      tdsrrcmeas_send_clear_ded_pri_ind_set_owner();
    }
    /* LTETBD: change to LTE_RRC_DEDICATED_PRI_OWNER_TDSCDMA */
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    else if(LTE_RRC_DEDICATED_PRI_OWNER_TDSCDMA != lte_rrc_dedicated_pri_get_owner(tdsrrc_get_as_id()))
#else
    else if(LTE_RRC_DEDICATED_PRI_OWNER_TDSCDMA != lte_rrc_dedicated_pri_get_owner())
#endif
    {
      tdsrrcmeas_invalidate_dedicated_priority_info();
    }
  }
#endif

#ifndef FEATURE_DSAC
  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(cmd_id)) == NULL)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"No MM buffer for CSP to MM cmd 0x%x",cmd_id);
    return(FAILURE);
  }
#else
  if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG)
  {
    /* Get a buffer from MM task */
    if((mm_common_buf_ptr = mm_tdsrrc_get_cmd_buf(cmd_id)) == NULL)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"No MM buffer for CSP to MM cmd 0x%x",cmd_id);
      return(FAILURE);
    }
  }
  else if(cmd_id == RRC_SERVICE_IND || cmd_id == RRC_SERVICE_CNF)
  {
    /* Get a buffer from MM task */
    TDSRRC_MSG0(MSG_LEGACY_HIGH," DSAC: Get a normal MM Buffer");
    if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(cmd_id)) == NULL)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"No MM buffer for CSP to MM cmd 0x%x",cmd_id);
      return(FAILURE);
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"It shouldn't come here");
  }   
#endif

#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"curr_scan %d, prev_curr_scan %d, svc_srch_bgnd %d", 
                                           tdsrrc_csp_int_data.curr_scan, 
                                           tdsrrc_csp_int_data.prev_curr_scan, 
                                           tdsrrc_csp_int_data.service_search_bgnd);
#endif

  switch(cmd_id)
  {
    case RRC_SERVICE_CNF:
    {
        tdsrrcwrm_stop_lock_wait_timer();

       /* Do not release the lock for CSFB and ECALL cases */
       if((service_status == MM_AS_SERVICE_AVAILABLE) && 
           ((rrc_state == TDSRRC_STATE_CELL_FACH) ||
           (rrc_state == TDSRRC_STATE_CELL_DCH) ||
           ((tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED)&& (tdsrrc_csp_int_data.service_req_cause == RRC_MO_CS_CALL))
#if defined (FEATURE_LTE_TO_TDSCDMA_DSDS) && defined (FEATURE_3GPP_CSFB_TDS)
          || (tdsrrc_csfb_call_status)
#endif
         ))
       {
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSIM: TDSRRC state is %d,MM svc sts %d CSFB call status is set. Do not release the lock",rrc_state,service_status);
       }
       else
       {
        tdsrrcwrm_release_lock();
       }
#ifdef FEATURE_DUAL_SIM
        mm_buf_ptr->cmd.rrc_service_cnf.as_id = tdsrrc_get_as_id();
#endif
/* If WRLF is enabled, SERVICE_CNF with service available and under DCH/FACH, check and change priority
 * If WRLF is not enabled, do not check and change priority. Due to when WRLF is not enabled, RRC will search TDs/other RAT 
 * again until connected timer timeout and back to IDLE.  
 */
#ifdef FEATURE_WRLF_SYSTEM_SEL
     /* If there are detected PLMN's during ANY_PLMN search, hack and send service cnf as service_available
          for updating UI with limited service */
        if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS() && 
           (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN) &&
           (tdsrrc_csp_int_data.curr_type_acq == TDSRRC_CSP_ANY_PLMN) &&
           (service_status == MM_AS_SERVICE_AVAILABLE) && (tdsrrccsp_get_csp_substate() == TDSRRCCSP_CELL_SEL_WT_SIB))
        {
           hack_srvc_avail_for_lim_srvc = TRUE;
           TDSRRC_MSG0(MSG_LEGACY_HIGH, "WRLF:Sending SERVICE_CNF as srvc_available to update UI with Lim srvc.");
        }

        if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN) && 
           ((rrc_state == TDSRRC_STATE_CELL_DCH) ||(rrc_state == TDSRRC_STATE_CELL_FACH)) &&
           (service_status == MM_AS_SERVICE_AVAILABLE) &&
           tds_is_ue_in_dual_standby)
        {
          /*Service is found in DCH/FACH state, increase the priority*/
          trm_reason_enum_t priority;
          if (tdsrrc_ok_for_tune_away())
          {
            if (tdsrrc_is_access_active())
            {
              priority = TRM_ACCESS;
            }
            else
            {
              priority = TRM_BG_TRAFFIC;
            }
          }
          else

          if (tdsrrc_is_traffic_active())
          {
            priority = TRM_TRAFFIC;
          }
          else
          {
            priority = TRM_BG_TRAFFIC;
          }
          tdsrrcwrm_stop_lock_wait_timer();
          tdsrrc_wrm_change_priority(priority, FALSE);	  
          tdrm_retain_lock(TDRM_CLIENT_RRC);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Priority Extended");
        }
        

#endif

        tdsrrcmcm_reset_last_act_cause();
        /* Fill up header */
        mm_buf_ptr->cmd.rrc_service_cnf.service_status = service_status;
        mm_buf_ptr->cmd.rrc_service_cnf.service_info.selected_plmn = *sel_plmn_ptr;
        mm_buf_ptr->cmd.rrc_service_cnf.service_info.gsm_map_common_info_incl = TRUE;
        mm_buf_ptr->cmd.rrc_service_cnf.status = MM_AS_SUCCESS;

        mm_buf_ptr->cmd.rrc_service_cnf.is_cell_id_valid = FALSE;

        /* if a plmn was found by RRC then set ue_oos (UE is Out of Service) to FALSE 
         * This ue_oos flag is used by NAS to determine if limited service request should
         * be sent to RRC or not.
         */
        mm_buf_ptr->cmd.rrc_service_cnf.ue_oos = !tdsrrc_csp_int_data.plmn_fnd_during_fgnd_svc_srch;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"status of ue_oos %d sent to NAS",mm_buf_ptr->cmd.rrc_service_cnf.ue_oos);

      /* Reset the next_band_mask appropriately */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the next_band_mask");
      tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_int_data.tds_band_mask;
      /* Record next_band_mask for deep search*/
      tdsrrc_csp_curr_select_data.next_band_mask_for_deep = tdsrrc_csp_curr_select_data.next_band_mask;
      if(service_status == MM_AS_NO_SERVICE)
      {
        tdsrrc_csp_int_data.csfb_extended_status = FALSE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2T_CSFB: CSFB call extended status is FALSE");
      
        /* If no PLMN selected, fill in the available PLMN list */
        mm_buf_ptr->cmd.rrc_service_cnf.avail_plmn_list =
          tdsrrc_csp_int_data.plmn_list;

        {
           rrc_avail_plmn_type the_plmn;
           uint32 num_plmns = tdsrrc_csp_int_data.plmn_list.num_plmns;
           TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC_SERVICE_CNF w/o SVC. Avail PLMNs %d",num_plmns);

           while (num_plmns)
           {
              the_plmn = tdsrrc_csp_int_data.plmn_list.plmn[--num_plmns];
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"  --- %d:%d, sig %d",TDSRRC_CSP_GET_INT_MCC(the_plmn.plmn),
                                                            TDSRRC_CSP_GET_INT_MNC(the_plmn.plmn),
                                                            the_plmn.signal_strength);
           }
        }
        {
          /* Reset the current band */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the current band");
  
          /* Reset the current_band */
          tdsrrc_csp_curr_select_data.current_band = RRC_CSP_TDS_A_BAND;
          tdsrrc_csp_curr_select_data.current_sub_band = 0;
        }
  
        /* If ue is in deep sleep and available plmn list is empty then    */
        /* start a no_svc_timer so that if RRC receives a svc req from NAS */ 
        /* before this timer expires then return immediately with no svc   */
        /* - optimization (stand by time improvement)                      */
#ifdef FEATURE_TDSCDMA_SCAN_SCOPE
        /* TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER shouldn't be started after performing ACQ DB only service search */
        if((tdsrrc_csp_int_data.ue_in_deep_sleep) && (tdsrrc_csp_int_data.srvc_req_scan_scope != SYS_SCAN_SCOPE_ACQ_DB))
#else
        if(tdsrrc_csp_int_data.ue_in_deep_sleep)
#endif
        {
          if((tdsrrc_csp_int_data.plmn_list.num_plmns==0) && (!tdsrrc_csp_int_data.plmn_fnd_during_fgnd_svc_srch))
          { 
            if(tdsrrc_csp_int_data.deep_sleep_no_svc_tmr_expired)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deep Sleep: Starting no service timer");
              tdsrrctmr_start_timer(  TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER, 
                                    TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER_IN_MS );
              tdsrrc_csp_int_data.deep_sleep_no_svc_tmr_expired = FALSE;

              tdsrrc_csp_int_data.deep_sleep_no_svc_band_scanned_mask |= tdsrrc_csp_int_data.tds_band_mask;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Updating no_svc_band_scanned_mask 0x%x",tdsrrc_csp_int_data.deep_sleep_no_svc_band_scanned_mask);
            }
            else
            {
              tdsrrc_csp_int_data.deep_sleep_no_svc_band_scanned_mask |= tdsrrc_csp_int_data.tds_band_mask;

              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Updating no_svc_band_scanned_mask 0x%x",tdsrrc_csp_int_data.deep_sleep_no_svc_band_scanned_mask);
            }
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"At least one PLMN found. Not updating no_svc_band_scanned_mask 0x%x",tdsrrc_csp_int_data.deep_sleep_no_svc_band_scanned_mask);
          } 
        }

        if(tdsrrc_csp_int_data.oos_due_to_no_resource)
        {
          mm_buf_ptr->cmd.rrc_service_cnf.acq_status = SYS_ACQ_STATUS_NO_RESOURCE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Setting acq_status as SYS_ACQ_STATUS_NO_RESOURCE in service cnf");
        }
        else
        {
          mm_buf_ptr->cmd.rrc_service_cnf.acq_status = SYS_ACQ_STATUS_SUCCESS;
#ifdef FEATURE_QSH_EVENT_METRIC
          TDSRRC_QSH_METRIC_UPDATE(TDSRRC_QSH_CEL_SEL_FAILURE);
#endif
        }
      } /* service not available */

      /* Service is available */
      else  
      {

        mm_buf_ptr->cmd.rrc_service_cnf.is_cell_id_valid = TRUE;
        mm_buf_ptr->cmd.rrc_service_cnf.cell_id = acq_entry_ptr->cell_id;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Reporting Cell_ID :%d to NAS",acq_entry_ptr->cell_id);

        if(tdsrrccsp_find_access_control(&access_control, &rrc_dsac_info,
                                      acq_entry_ptr->plmn_id,
                                      acq_entry_ptr->cell_id
                                      ,common_plmn_id
                                      )
           == FAILURE)
        {
          ERR_FATAL("Failure finding cell access status",0,0,0); 
        }

        /* TBD: Set the number of PLMNs in available PLMN list to 0 for the time being */
        mm_buf_ptr->cmd.rrc_service_cnf.avail_plmn_list.num_plmns = 0;          

        /* Get the cell access status of the camped on cell from CCM */
        
        (void) tdsrrccsp_find_rrc_cell_access(&(mm_buf_ptr->cmd.rrc_service_cnf.service_info), &rrc_dsac_info,
                           access_control,acq_entry_ptr->status_mask);
        tdsrrc_csp_int_data.curr_cell_access_status = mm_buf_ptr->cmd.rrc_service_cnf.service_info.cell_access;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"update cell AC status =%d before sending SERVICE_CNF to NAS",tdsrrc_csp_int_data.curr_cell_access_status);
#ifdef FEATURE_DSAC
        /* Fill CS and PS cell access info when DSAC information is valid*/
        if(rrc_dsac_info.valid == TRUE)
        {
          tdsrrc_csp_int_data.is_dsac_valid  = rrc_dsac_info.valid;
          tdsrrc_csp_int_data.curr_cs_cell_access = mm_buf_ptr->cmd.rrc_service_cnf.service_info.cs_cell_access;
          tdsrrc_csp_int_data.curr_ps_cell_access = mm_buf_ptr->cmd.rrc_service_cnf.service_info.ps_cell_access;
          mm_buf_ptr->cmd.rrc_service_cnf.service_info.dsac_present = rrc_dsac_info.valid;
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: CSP internal DSAC present, CS and PS cell acces info: %d , %d,  %d",
                                                                        tdsrrc_csp_int_data.is_dsac_valid,
                                                                        tdsrrc_csp_int_data.curr_cs_cell_access ,
                                                                        tdsrrc_csp_int_data.curr_ps_cell_access
                                                                        );
        }
        else
        {
          tdsrrc_csp_int_data.curr_cell_access_status = mm_buf_ptr->cmd.rrc_service_cnf.service_info.cell_access;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: DSAC info is not present and CSP internal cell access info is %d ",tdsrrc_csp_int_data.curr_cell_access_status);
        }
#endif

#ifdef FEATURE_WRLF_SYSTEM_SEL
        /* After WRLF, if UE is continuing call on the same cell or EPLMN cell, RRC to send invalid PLMN to NAS so that
             no LA/RA is attemped by NAS. */
        if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS() && !hack_srvc_avail_for_lim_srvc)
        {
          mm_buf_ptr->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_type = sel_plmn_ptr->plmn_type;
          mm_buf_ptr->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id.mcc[0]= 0xFF;
          mm_buf_ptr->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id.mcc[1]= 0xFF;
          mm_buf_ptr->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id.mcc[2]= 0xFF;
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"WRLF: Set mcc[0]=0x%x, mcc[1]=0x%x, mcc[2]=0x%x in RRC_SERVICE_CNF. ",
                         mm_buf_ptr->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id.mcc[0],
                         mm_buf_ptr->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id.mcc[1],
                         mm_buf_ptr->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id.mcc[2]);
 
          mm_buf_ptr->cmd.rrc_service_cnf.service_info.gsm_map_common_info_incl = FALSE;
          mm_buf_ptr->cmd.rrc_service_cnf.service_info.num_cn_domains = 0;
        }
        else
#endif
        {
          /* Fill up the NAS system info here */
          /* Let us first read SIB1 - we should have SIB1 for sure */
          sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrc_sibdb_return_sib_for_cell(
                                                    tdsrrc_SIB1,
                                                    common_plmn_id, 
                                                    acq_entry_ptr->cell_id);
          if( sib1_ptr == NULL )
          {
            /* SIB3 for this PLMN, CEll ID combination not present */
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB1 for MCC %d, MNC %d, Cell ID %d not present",
                        TDSRRC_CSP_GET_INT_MCC(common_plmn_id), 
                        TDSRRC_CSP_GET_INT_MNC(common_plmn_id),
                                           acq_entry_ptr->cell_id);
            return(FAILURE);
          } /* SIB1 not present */

          if(tdsrrccsp_fill_nas_common_info(sib1_ptr, 
                                         &mm_buf_ptr->cmd.rrc_service_cnf.
                                           service_info.gsm_map_common_info)
            == FAILURE)
          {
            return(FAILURE);
          }

          /* Fill in the CN domain specific NAS system info now */
          num_cn_domains = 0;
        cn_domain_sys_info_list_ptr =  &sib1_ptr->cn_DomainSysInfoList; 
          do
          {            
            /* First check if Core network is GSM MAP. */
          if(cn_domain_sys_info_list_ptr->elem[idx].cn_Type.t == 
                                            T_tdsrrc_CN_DomainSysInfo_cn_Type_gsm_MAP)
            {
            if(tdsrrccsp_fill_nas_cn_specific_info(&cn_domain_sys_info_list_ptr->elem[idx], 
                                                  &mm_buf_ptr->cmd.rrc_service_cnf.
                                                  service_info.cn_domain_info[num_cn_domains])
                 == FAILURE)
              {
                return(FAILURE);
              }
              num_cn_domains ++;
            } /* If CN type is GSM MAP */ 
          idx++;

        }while((cn_domain_sys_info_list_ptr->n > idx) && 
                (num_cn_domains < RRC_MAX_CN_DOMAINS));   
        
          mm_buf_ptr->cmd.rrc_service_cnf.service_info.num_cn_domains = num_cn_domains;
        }

        tdsrrccsp_display_CN_domains(cmd_id, 
                                     num_cn_domains,
                                     mm_buf_ptr->cmd.rrc_service_cnf.service_info.cn_domain_info[0].cn_domain_id,
                                     mm_buf_ptr->cmd.rrc_service_cnf.service_info.cn_domain_info[1].cn_domain_id);
        
        /* Set the active band */
        mm_buf_ptr->cmd.rrc_service_cnf.active_band = sys_band;
        tdsrrc_csp_int_data.deep_sleep_band_scanned_mask = 0;

        /* Reset no service band mask to zero since service has been found */
        tdsrrc_csp_int_data.deep_sleep_no_svc_band_scanned_mask = 0;

#ifdef FEATURE_DSAC
        if(rrc_dsac_info.valid == TRUE)
        {
           mm_buf_ptr->cmd.rrc_service_cnf.service_info.dsac_present = rrc_dsac_info.valid;
           tdsrrc_csp_int_data.is_dsac_valid  = rrc_dsac_info.valid;
           /* Print the DSAC Information*/   
           TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC:DSAC parameters in RRC_SERVICE_CNF <DSAC valid, CS Cell Access and PS Cell Access> %d, %d, %d", 
                                                       mm_buf_ptr->cmd.rrc_service_cnf.service_info.dsac_present,
                                                       mm_buf_ptr->cmd.rrc_service_cnf.service_info.cs_cell_access,
                                                       mm_buf_ptr->cmd.rrc_service_cnf.service_info.ps_cell_access);
           TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: rrc_dsac_info.valid value is %d",tdsrrc_csp_int_data.is_dsac_valid);
        }
        else
        {
          mm_buf_ptr->cmd.rrc_service_cnf.service_info.dsac_present = rrc_dsac_info.valid;
          tdsrrc_csp_int_data.is_dsac_valid  = rrc_dsac_info.valid;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: rrc_dsac_info.valid value is %d",tdsrrc_csp_int_data.is_dsac_valid);
        }
#endif
        /* Reset the ue_in_deep_sleep variable */
        tdsrrc_csp_int_data.ue_in_deep_sleep = FALSE;
        tdsrrc_csp_int_data.full_scan_in_deep_sleep_counter = 0;

        if(!tdsrrc_csp_int_data.deep_sleep_no_svc_tmr_expired)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deep Sleep: Stopping no service timer");
          tdsrrctmr_stop_timer(TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER);
          tdsrrc_csp_int_data.deep_sleep_no_svc_tmr_expired = TRUE;
        }

        if(!tdsrrc_csp_int_data.full_scan_needed)
        {
          tdsrrctmr_stop_timer(TDSRRCTMR_FREQ_SCAN_TIMER);
          tdsrrc_csp_int_data.full_scan_needed = TRUE;
        }



#ifdef FEATURE_WRLF_SYSTEM_SEL
        /* Reset the ue_in_conn_oos flag if UE finds a suitable TDS cell during OOS search. */
        if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS() && !hack_srvc_avail_for_lim_srvc)
        {
          /* Send RAB_EST_IND to NAS if UE continues with the existing connection */
          tdsrrcrbe_send_rabm_est_ind_to_upper_layers();
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF: Resetting ue_in_conn_oos flag");
          tdsrrc_csp_int_data.ue_in_conn_oos = FALSE;

          if(!tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd)
          {
            tdsrrctmr_stop_timer(TDSRRCTMR_CONN_MODE_OOS_TIMER);
            tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd = TRUE;
          }
        }
#endif
      } /* Service is available */

      tdsrrc_csp_int_data.trm_timeout_value = 0xFFFFFFFF;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: When sending service cnf,reset tdsrrc_csp_int_data.trm_timeout_value to 0xffffffff");
 
    } /* RRC_SERVICE_CNF */
#ifdef FEATURE_WRLF_SYSTEM_SEL
    if (tdsrrc_csp_int_data.ue_in_connecting_oos == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF: Resetting ue_in_connecting_oos flag");
      tdsrrc_csp_int_data.ue_in_connecting_oos = FALSE;
    }
#endif
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Filling SERVICE_CNF is done");
      break;

    case RRC_SERVICE_IND:
    case RRC_SERVICE_IND_FROM_OTA_MSG:
      {

        if(cmd_id == RRC_SERVICE_IND)
        {
          mm_buf_ptr->cmd.rrc_service_ind.is_cell_id_valid = FALSE;
        }
#ifdef FEATURE_DSAC
        else
        {
          mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = FALSE;
        }
#endif

#ifdef FEATURE_DUAL_SIM
        if(cmd_id == RRC_SERVICE_IND)
        {
          mm_buf_ptr->cmd.rrc_service_ind.as_id = tdsrrc_get_as_id();
        }
#ifdef FEATURE_DSAC
        else
        {
          mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.as_id = tdsrrc_get_as_id();
        }
#endif
#endif
        /* Fill up header */
#ifndef FEATURE_DSAC
        mm_buf_ptr->cmd.rrc_service_ind.service_status = service_status;
        mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn = *sel_plmn_ptr;


#else
        if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH," Cmd Rexd RRC_SERVICE_IND_FROM_OTA_MSG");
          mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_status = service_status;
          if(rrc_state == TDSRRC_STATE_DISCONNECTED)
          {
            mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn = *sel_plmn_ptr;
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: Set mcc[0]=0x%x, mcc[1]=0x%x, mcc[2]=0x%x in RRC_SERVICE_IND_FROM_OTA_MSG ",
                           mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0],
                           mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1],
                           mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]);
          }
          else
          {
            mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_type = sel_plmn_ptr->plmn_type;
            mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0]= 0xFF;
            mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1]= 0xFF;
            mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]= 0xFF;
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: Set mcc[0]=0x%x, mcc[1]=0x%x, mcc[2]=0x%x in RRC_SERVICE_IND_FROM_OTA_MSG ",
                           mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0],
                           mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1],
                           mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2]);
          }


        }
        else if(cmd_id == RRC_SERVICE_IND)   
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH," Cmd Rexd RRC_SERVICE_IND");
           mm_buf_ptr->cmd.rrc_service_ind.service_status = service_status;
           mm_buf_ptr->cmd.rrc_service_ind.service_info.selected_plmn = *sel_plmn_ptr;

#ifdef FEATURE_QSH_EVENT_METRIC
           TDSRRC_QSH_SERVING_CELL_UPDATE();
           if(service_status == MM_AS_NO_SERVICE)
           {
             TDSRRC_QSH_METRIC_UPDATE(TDSRRC_QSH_CEL_SEL_FAILURE);
           }
#endif

           /* NAS expects abort indication before service indication*/
           if((service_status == MM_AS_NO_SERVICE) && (tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
           {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ask RCE to send ABORT_IND first");
              tdsrrcrce_force_initialize(FALSE);
           }
        }
        else
        {
           TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSAC:Should not come here ");
        }
#endif


#ifdef FEATURE_WRLF_SYSTEM_SEL
        /* If UE is in connected mode OOS, stop time only if service is found*/
        if (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
        {
          if(service_status == MM_AS_SERVICE_AVAILABLE)
          { 
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF: Resetting ue_in_conn_oos flag");
            tdsrrc_csp_int_data.ue_in_conn_oos = FALSE;
            if(!tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd)
            {
               tdsrrctmr_stop_timer(TDSRRCTMR_CONN_MODE_OOS_TIMER);
               tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd = TRUE;
            }
          }
        }
        else if(!tdsrrc_csp_int_data.oos_initial_no_deep_sleep_tmr_expd)
        {
          tdsrrctmr_stop_timer(TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER);
          tdsrrc_csp_int_data.oos_initial_no_deep_sleep_tmr_expd = TRUE;
        }
#endif /* bypass triton compiler error once WRLF feature is disabled */

        if(service_status == MM_AS_SERVICE_AVAILABLE)
        {

          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Reporting Cell_ID :%d to NAS",acq_entry_ptr->cell_id);

          if(cmd_id == RRC_SERVICE_IND)
          {
            mm_buf_ptr->cmd.rrc_service_ind.is_cell_id_valid = TRUE;
            mm_buf_ptr->cmd.rrc_service_ind.cell_id = acq_entry_ptr->cell_id;
          }
#ifdef FEATURE_DSAC
          else
          {
            mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid = TRUE;
            mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.cell_id = acq_entry_ptr->cell_id;
          }
#endif

          if(tdsrrccsp_find_access_control(&access_control, &rrc_dsac_info,
                                         acq_entry_ptr->plmn_id,
                                         acq_entry_ptr->cell_id
                                        ,common_plmn_id
                                         )
             == FAILURE)
          {
            ERR_FATAL("Failure finding cell access status",0,0,0); 
          }

        /* Get the cell access status of the camped on cell */
#ifndef FEATURE_DSAC
        (void) tdsrrccsp_find_rrc_cell_access(&(mm_buf_ptr->cmd.rrc_service_ind.service_info), 
                                                                   &rrc_dsac_info,access_control, acq_entry_ptr->status_mask);
        tdsrrc_csp_int_data.curr_cell_access_status = mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"update cell AC status =%d before sending SERVICE_IND to NAS",tdsrrc_csp_int_data.curr_cell_access_status);
#else
        if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG)
        {
          (void) tdsrrccsp_find_rrc_cell_access(&(mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info), &rrc_dsac_info,access_control, acq_entry_ptr->status_mask);
          tdsrrc_csp_int_data.is_dsac_valid  = rrc_dsac_info.valid;
          if(! tdsrrc_csp_int_data.is_dsac_valid)
          {
            tdsrrc_csp_int_data.curr_cell_access_status = mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.cell_access;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: CSP internal cell access info is%d",tdsrrc_csp_int_data.curr_cell_access_status);
          }
          else
          {
            tdsrrc_csp_int_data.curr_cs_cell_access = mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.cs_cell_access;
            tdsrrc_csp_int_data.curr_ps_cell_access = mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.ps_cell_access;
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: CSP internal dsac_valid, CS  and PS cell access info is%d, %d, %d",tdsrrc_csp_int_data.is_dsac_valid,
                                                                                               tdsrrc_csp_int_data.curr_cs_cell_access,
                                                                                               tdsrrc_csp_int_data.curr_ps_cell_access);
          }
        }
        else if(cmd_id == RRC_SERVICE_IND)
        {
          (void) tdsrrccsp_find_rrc_cell_access(&(mm_buf_ptr->cmd.rrc_service_ind.service_info), 
                                                                    &rrc_dsac_info,access_control, acq_entry_ptr->status_mask);
          tdsrrc_csp_int_data.is_dsac_valid  = rrc_dsac_info.valid;
          if(!tdsrrc_csp_int_data.is_dsac_valid)
          {
            tdsrrc_csp_int_data.curr_cell_access_status = mm_buf_ptr->cmd.rrc_service_ind.service_info.cell_access;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: CSP internal cell access info is%d",tdsrrc_csp_int_data.curr_cell_access_status);
          }
          else
          {
            tdsrrc_csp_int_data.curr_cs_cell_access = mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access;
            tdsrrc_csp_int_data.curr_ps_cell_access = mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access;
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC: CSP internal dsac_valid, CS  and PS cell access info is%d, %d, %d",tdsrrc_csp_int_data.is_dsac_valid,
                                                                                               tdsrrc_csp_int_data.curr_cs_cell_access,
                                                                                               tdsrrc_csp_int_data.curr_ps_cell_access);
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSAC: Shouldn't come here");
        }
#endif

        /* Fill up the NAS system info here */
        /* Let us first read SIB1 - we should have SIB1 for sure */
        sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrc_sibdb_return_sib_for_cell(
                                                  tdsrrc_SIB1,
                                                  common_plmn_id, 
                                                  acq_entry_ptr->cell_id);
        if( sib1_ptr == NULL )
        {
          /* SIB3 for this PLMN, CEll ID combination not present */
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB1 for MCC %d, MNC %d, Cell ID %d not present",
                      TDSRRC_CSP_GET_INT_MCC(common_plmn_id), 
                      TDSRRC_CSP_GET_INT_MNC(common_plmn_id),
                                         acq_entry_ptr->cell_id);
          return(FAILURE);
        } /* SIB1 not present */

        /* Fill in the GSM common map information only if state is not CELL_FACH/URA_PCH/CELL_PCH
           and if TD2G CCO is not active.If TD2G CCO is active then the SERVICE_IND should
           include the GSM_MAP_COMMON_INFO even if we are in connected mode.NAS would need this 
           information to get the Location Area info */
        if(((rrc_state == TDSRRC_STATE_CELL_FACH) ||
            (rrc_state == TDSRRC_STATE_CELL_PCH) ||
            (rrc_state == TDSRRC_STATE_URA_PCH)) &&
            (FALSE == tdsrrccho_is_td2g_cco_active()))
        {
#ifndef FEATURE_DSAC
          mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = FALSE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Excluding gsm_map_common_info in RRC_SERVICE_IND");
#else
          if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG)
          {
            mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.gsm_map_common_info_incl = FALSE;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC:Excluding gsm_map_common_info in RRC_SERVICE_IND_FROM_OTA_MSG");
          }
          else if( cmd_id == RRC_SERVICE_IND)
          {
            mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = FALSE;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: Excluding gsm_map_common_info in RRC_SERVICE_IND");
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC:Shoudn't come here ");
          }
#endif
        }
        else
        {
#ifndef FEATURE_DSAC
          mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = TRUE;
          if(tdsrrccsp_fill_nas_common_info(sib1_ptr,
                                         &mm_buf_ptr->cmd.rrc_service_ind.
                                           service_info.gsm_map_common_info)
            == FAILURE)
          {
            return(FAILURE);
          }
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Including gsm_map_common_info in RRC_SERVICE_IND");
#else
          if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG)
          {
            /* Do not fill nas common info in RRC_SERVICE_IND_FROM_OTA_MSG */
            mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.gsm_map_common_info_incl = FALSE;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: gsm_map_common_info_incl set to false in RRC_SERVICE_IND_FROM_NAS_OTA");
          }
          else if(cmd_id == RRC_SERVICE_IND)
          {
            mm_buf_ptr->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl = TRUE;
            if(tdsrrccsp_fill_nas_common_info(sib1_ptr,
                                           &mm_buf_ptr->cmd.rrc_service_ind.
                                           service_info.gsm_map_common_info)
               == FAILURE)
            {
              return(FAILURE);
            }
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Including gsm_map_common_info in RRC_SERVICE_IND");
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSAC:Shouldn't come here");
          }
#endif
        }
        /* Fill in the CN domain specific NAS system info now */
        num_cn_domains = 0;
        idx=0;
#ifndef FEATURE_DSAC
        cn_domain_sys_info_list_ptr =  &sib1_ptr->cn_DomainSysInfoList; 
        do
        {
          /* First check if Core network is GSM MAP. */
          if(cn_domain_sys_info_list_ptr->elem[idx].cn_Type.t == 
                                            T_tdsrrc_CN_DomainSysInfo_cn_Type_gsm_MAP)
          {
            if(tdsrrccsp_fill_nas_cn_specific_info(&cn_domain_sys_info_list_ptr->elem[idx], 
                                                &mm_buf_ptr->cmd.rrc_service_ind.
                                                service_info.cn_domain_info[num_cn_domains])
               == FAILURE)
            {
              return(FAILURE);
            }
            num_cn_domains ++;
          } /* If CN type is GSM MAP */ 
          idx++;

        }while((cn_domain_sys_info_list_ptr->n > idx) && 
               (num_cn_domains < RRC_MAX_CN_DOMAINS));  

        tdsrrccsp_display_CN_domains(cmd_id, 
                                     num_cn_domains,
                                     mm_buf_ptr->cmd.rrc_service_ind.service_info.cn_domain_info[0].cn_domain_id,
                                     mm_buf_ptr->cmd.rrc_service_ind.service_info.cn_domain_info[1].cn_domain_id);
#else
        if(cmd_id != RRC_SERVICE_IND_FROM_OTA_MSG)
        {
          cn_domain_sys_info_list_ptr =  &sib1_ptr->cn_DomainSysInfoList; 
          do
          {
            /* First check if Core network is GSM MAP. */
            if(cn_domain_sys_info_list_ptr->elem[idx].cn_Type.t == 
                                              T_tdsrrc_CN_DomainSysInfo_cn_Type_gsm_MAP)
            {
              if(tdsrrccsp_fill_nas_cn_specific_info(&cn_domain_sys_info_list_ptr->elem[idx], 
                                                  &mm_buf_ptr->cmd.rrc_service_ind.
                                                  service_info.cn_domain_info[num_cn_domains])
                 == FAILURE)
              {
                return(FAILURE);
              }
              num_cn_domains ++;
            } /* If CN type is GSM MAP */ 
            idx++;
          }while((cn_domain_sys_info_list_ptr->n > idx) && 
                 (num_cn_domains < RRC_MAX_CN_DOMAINS));  

          tdsrrccsp_display_CN_domains(cmd_id, 
                                       num_cn_domains,
                                       mm_buf_ptr->cmd.rrc_service_ind.service_info.cn_domain_info[0].cn_domain_id,
                                       mm_buf_ptr->cmd.rrc_service_ind.service_info.cn_domain_info[1].cn_domain_id);
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSAC: nas cn spec info is not included RRC_SERVICE_IND_FROM_OTA_MSG");
        }
#endif

#ifndef FEATURE_DSAC
        mm_buf_ptr->cmd.rrc_service_ind.service_info.num_cn_domains = num_cn_domains;
#else
        if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG)
        {
          mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.num_cn_domains = num_cn_domains;
        }
        else if(cmd_id == RRC_SERVICE_IND)
        {
          mm_buf_ptr->cmd.rrc_service_ind.service_info.num_cn_domains = num_cn_domains;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Shouldn't come here");
        }
#endif

/*Populate DSAC information in mm buffer */
#ifdef FEATURE_DSAC
        if(rrc_dsac_info.valid == TRUE)
        {
          if(cmd_id == RRC_SERVICE_IND)
          {
             mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present = rrc_dsac_info.valid;
             tdsrrc_csp_int_data.is_dsac_valid  = rrc_dsac_info.valid;
             
             /* Print the DSAC Information*/   
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC:DSAC params in RRC_SERVICE_IND <DSAC Present, CS Cell Access and PS Cell Access> %d, %d, %d", 
                                              mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present,
                                              mm_buf_ptr->cmd.rrc_service_ind.service_info.cs_cell_access,
                                              mm_buf_ptr->cmd.rrc_service_ind.service_info.ps_cell_access);
          }
          else if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG)
          {
             mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.dsac_present = rrc_dsac_info.valid;
             tdsrrc_csp_int_data.is_dsac_valid  = rrc_dsac_info.valid;
             /* Print the DSAC Information*/   
             TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSAC:DSAC params in RRC_SERVICE_IND_FROM_OTA_MSG <DSAC Present, CS Cell Access and PS Cell Access> %d, %d, %d", 
                                              mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.dsac_present,
                                              mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.cs_cell_access,
                                              mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.ps_cell_access);
          }
        }
        else
        {
            if(cmd_id == RRC_SERVICE_IND)
            {
              mm_buf_ptr->cmd.rrc_service_ind.service_info.dsac_present = rrc_dsac_info.valid;
              tdsrrc_csp_int_data.is_dsac_valid  = rrc_dsac_info.valid;
              /* Print that no DSAC Information is present*/   
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: rrc_dsac_info.valid value is %d",tdsrrc_csp_int_data.is_dsac_valid);
            }
            else if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG)
            {
               mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.service_info.dsac_present = rrc_dsac_info.valid;
               tdsrrc_csp_int_data.is_dsac_valid  = rrc_dsac_info.valid;
               /* Print  DSAC Information is present/not*/   
               TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSAC: rrc_dsac_info.valid value is %d",tdsrrc_csp_int_data.is_dsac_valid);
            }
          }
#endif

#ifndef FEATURE_DSAC
        /* Set the active band */
        mm_buf_ptr->cmd.rrc_service_ind.active_band = sys_band;
        tdsrrc_csp_int_data.deep_sleep_band_scanned_mask = 0;

          /* Reset no service band mask to zero since service has been found */
        tdsrrc_csp_int_data.deep_sleep_no_svc_band_scanned_mask = 0;
#else
        if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG)
        {
          /* Set the active band */
          mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.active_band = sys_band;
          tdsrrc_csp_int_data.deep_sleep_band_scanned_mask = 0;

          /* Reset no service band mask to zero since service has been found */
         tdsrrc_csp_int_data.deep_sleep_no_svc_band_scanned_mask = 0;
        }
        else if(cmd_id == RRC_SERVICE_IND)
        {
          /* Set the active band */
          mm_buf_ptr->cmd.rrc_service_ind.active_band = sys_band;
          tdsrrc_csp_int_data.deep_sleep_band_scanned_mask = 0;

          /* Reset no service band mask to zero since service has been found */
          tdsrrc_csp_int_data.deep_sleep_no_svc_band_scanned_mask = 0;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSAC:Shouldn't come here");
        }
#endif

        /* Reset the ue_in_deep_sleep variable */
        tdsrrc_csp_int_data.ue_in_deep_sleep = FALSE;
        tdsrrc_csp_int_data.full_scan_in_deep_sleep_counter = 0;

        tdsrrctmr_stop_timer(TDSRRCTMR_FREQ_SCAN_TIMER);
        tdsrrc_csp_int_data.full_scan_needed = TRUE;


        TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF: Setting no srv cause and timer.");
#ifdef FEATURE_WRLF_SYSTEM_SEL
        if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG)
        {
#ifndef FEATURE_DSAC
          mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.no_service_cause = SYS_NO_SVC_CAUSE_NONE;
          mm_buf_ptr->cmd.rrc_service_ind_from_ota_msg.suitable_search_time = 0;
#else
          mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.no_service_cause = SYS_NO_SVC_CAUSE_NONE;
          mm_common_buf_ptr->cmd.rrc_service_ind_from_ota_msg.suitable_search_time = 0;
#endif
        }
        else if(cmd_id == RRC_SERVICE_IND)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF: cmd_id == RRC_SERVICE_IND.");
          mm_buf_ptr->cmd.rrc_service_ind.no_service_cause = SYS_NO_SVC_CAUSE_NONE;
          mm_buf_ptr->cmd.rrc_service_ind.suitable_search_time = 0;
        }
#endif
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF: Done setting no srv cause and timer.");
        } /* Service is available */
        else
        {
          tdsrrc_csp_int_data.csfb_extended_status = FALSE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2T_CSFB: CSFB call extended status is FALSE");
          
          /* Reset the current_band */
          tdsrrc_csp_curr_select_data.current_band = RRC_CSP_TDS_A_BAND;
          /* Update UI with connected mode OOS as FALSE before sending SERVICE_IND as
             no service to NAS.
          */
#ifdef FEATURE_OOSC_USER_ACTION
#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
          if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN)&&
               (tdsrrc_csp_int_data.prev_curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SVC_ST: Update UI with connected mode OOS status as FALSE");
            tdsrrccsp_send_svc_status_to_mm(SYS_SRV_STATUS_NONE, SYS_RAT_TDS_RADIO_ACCESS, 0);
          }
#endif
#endif
#ifdef FEATURE_WRLF_SYSTEM_SEL
          if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
          {
            mm_buf_ptr->cmd.rrc_service_ind.no_service_cause = SYS_NO_SVC_CAUSE_RLF;
            mm_buf_ptr->cmd.rrc_service_ind.suitable_search_time = 30000;

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
            /* If UE encounters RLF after "fast return to LTE after CSFB" hack kicks in, send CLOSE_SESSION_IND for CS domain  to NAS 
               so that LTE is also searched as part of OOS search. */
            if(tdsrrc_csfb_extended_status == CSFB_CS_RB_RELEASED)
            {
              tdsrrcscr_send_close_session_ind(RRC_CS_DOMAIN_CN_ID,RRC_MAX_SESSION_CLOSE_CAUSE);
            }
#endif
          }
          else
          {
            mm_buf_ptr->cmd.rrc_service_ind.no_service_cause = SYS_NO_SVC_CAUSE_NORMAL;
            mm_buf_ptr->cmd.rrc_service_ind.suitable_search_time = 0;
          }
#endif
        }

        /* Skip next Service Request if reporting MM_AS_NO_SERVICE */
        if ((service_status == MM_AS_NO_SERVICE) && (cmd_id == RRC_SERVICE_IND))
        {
            /* Set the flag to skip the very next service request */
            if (TDSCHECK_OPTIONAL_FEATURE_CMCC_RLF_SKIP_SVCREQ && (!tdsrrcmcm_is_tds_only_mode()))
            {
               if ((RRC_EST_EMERGENCY_CALL == tdsrrcrce_get_rrc_est_cause())
                   && (tdsrrc_get_state() != TDSRRC_STATE_DISCONNECTED))
               {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"CR412213 Emergency call,no special OOS handling"); 
                  tdsrrc_csp_int_data.rlf_skip_first_svc_req = FALSE;
               }
               else
               {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"CR412213 set rlf_skip_first_svc_req to TRUE");
               tdsrrc_csp_int_data.rlf_skip_first_svc_req = TRUE;
            }
            }
            else
            {
               tdsrrc_csp_int_data.rlf_skip_first_svc_req = FALSE;
               TDSRRC_MSG1(MSG_LEGACY_HIGH,"CR412213 no skipping. CMCC_RLF_SKIP_SVCREQ=%d",TDSCHECK_OPTIONAL_FEATURE_CMCC_RLF_SKIP_SVCREQ);
            }
        }
        
/*        if(((cmd_id == RRC_SERVICE_IND) && (rrc_state == TDSRRC_STATE_DISCONNECTED)) ||
            ((cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG) && 
              ((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN) || 
                (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) ||
                (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_CONNECTING_STATE_SCAN)) &&
                (service_status == MM_AS_NO_SERVICE)
            )
          )*/
         if((cmd_id == RRC_SERVICE_IND) && (rrc_state == TDSRRC_STATE_DISCONNECTED)
           /*When cell resel happened during wait SIB7 status,after successfully resel CSP
           rel lock and then RCE est con but doesn't acq lock which lead to gsm can't acq lock.*/
		   &&(tdsrrcrce_get_rce_sub_state()!= TDSRRCRCE_WAIT_NEW_CELL_IND_SIB7)		 	
		 	)
        {
#if defined (FEATURE_LTE_TO_TDSCDMA_DSDS) && defined (FEATURE_3GPP_CSFB_TDS)
          /* Do not release the lock for CSFB cases */
          if(!tdsrrc_csfb_call_status)
          {
          /*Release the lock in disconnected state, after sending Service IND*/
          tdsrrcwrm_release_lock();
        }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: CSFB call status is set. Do not release the lock");
          }
#else
          /*Release the lock in disconnected state, after sending Service IND*/
          tdsrrcwrm_release_lock();
#endif
        }
        if((rrc_state == TDSRRC_STATE_CELL_PCH || rrc_state == TDSRRC_STATE_URA_PCH)
            && !(tdsrrccu_check_cu_active_before_lock_rel()))
        {
          tdsrrcwrm_release_lock();
        }

        /* when WRLF is enabled, RRC may set curr_scan to TDSRRC_CSP_LEAVING_CONN_MODE_SCAN when connected mode OOS happen
         * so check and change priority when got service.
         */
        
        if((
#ifdef FEATURE_WRLF_SYSTEM_SEL
            (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN) || 
            (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN) ||
#endif
            (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)) && 
            ((rrc_state == TDSRRC_STATE_CELL_DCH) ||(rrc_state == TDSRRC_STATE_CELL_FACH)) &&\
            (service_status == MM_AS_SERVICE_AVAILABLE) &&
             tds_is_ue_in_dual_standby)
        {
          /*Service is found in DCH/FACH state, increase the priority*/
          trm_reason_enum_t priority;
          if (tdsrrc_ok_for_tune_away())
          {
            if (tdsrrc_is_access_active())
            {
              priority = TRM_ACCESS;
            }
            else
            {
              priority = TRM_BG_TRAFFIC;
            }
          }
          else
          if (tdsrrc_is_traffic_active())
          {
            priority = TRM_TRAFFIC;
          }
          else
          {
            priority = TRM_BG_TRAFFIC;
          }
          tdsrrcwrm_stop_lock_wait_timer();
          tdsrrc_wrm_change_priority(priority, FALSE);
          tdrm_retain_lock(TDRM_CLIENT_RRC);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Priority Extended");
        }

        if(((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
#ifdef FEATURE_WRLF_SYSTEM_SEL
            || ((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN) && 
                (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()))
#endif
            ) && (service_status == MM_AS_NO_SERVICE))
        {
          tdsrrcwrm_stop_lock_wait_timer();
          tdsrrcwrm_release_lock();
        }

#ifdef FEATURE_DUAL_SIM
        if(cmd_id == RRC_SERVICE_IND)
        {
          if((tdsrrc_csp_int_data.oos_due_to_no_resource) && (service_status == MM_AS_NO_SERVICE))
          {
		    /* reset rlf_skip_first_svc_req if no service is caused by Tl1 no lock*/
            tdsrrc_csp_int_data.rlf_skip_first_svc_req = FALSE;
            mm_buf_ptr->cmd.rrc_service_ind.acq_status = SYS_ACQ_STATUS_NO_RESOURCE;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Setting acq_status as SYS_ACQ_STATUS_NO_RESOURCE in service ind");
          }
          else
          {
            mm_buf_ptr->cmd.rrc_service_ind.acq_status = SYS_ACQ_STATUS_SUCCESS;
          }
        }
#endif
#ifdef FEATURE_WRLF_SYSTEM_SEL
        if (tdsrrc_csp_int_data.ue_in_connecting_oos == TRUE)
        {
          tdsrrc_csp_int_data.ue_in_connecting_oos = FALSE;
        }
#endif
      } /* RRC_SERVICE_IND */
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid cmd ID 0x%x for send_mm_cmd",cmd_id); 
      return(FAILURE);

  }  /* switch(cmd_id) */

  /* Put the command on MM queue */
#ifndef FEATURE_DSAC
   TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending MM cmd 0x%x",cmd_id);
   if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
     tdscfa_mm_put_cmd(mm_buf_ptr);
   }

   tdsrrcccm_backup_service_ind(cmd_id, mm_buf_ptr);
   
   mm_put_cmd(mm_buf_ptr);
#else
   if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG)
   {
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending MM cmd 0x%x",cmd_id);
       if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
       {
         tdscfa_mm_put_cmd(mm_common_buf_ptr);
       }
       tdsrrcccm_backup_service_ind(cmd_id, mm_common_buf_ptr);
       mm_put_cmd(mm_common_buf_ptr);
   }
   else if(cmd_id == RRC_SERVICE_IND || cmd_id == RRC_SERVICE_CNF)
   {
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending MM cmd 0x%x",cmd_id);
       if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
       {
         tdscfa_mm_put_cmd(mm_buf_ptr);
       }
       tdsrrcccm_backup_service_ind(cmd_id, mm_buf_ptr);
       mm_put_cmd(mm_buf_ptr);
   }
   else
   {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"Should not come here");
     /* More defensive check*/
     if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
     {
       tdscfa_mm_put_cmd(mm_buf_ptr);
     }
     tdsrrcccm_backup_service_ind(cmd_id, mm_buf_ptr);
     mm_put_cmd(mm_buf_ptr);
   }
#endif   
  if(cmd_id == RRC_SERVICE_CNF)
  {
    if(service_status == MM_AS_SERVICE_AVAILABLE)
    {
      tdsrrctmr_stop_timer(TDSRRCTMR_RSSI_BER_TIMER);
      tdsrrc_rssi_ber_timeout_handler();
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"On sending Service Cnf with service available to NAS also post RSSI");
    } 
  }

  if(cmd_id == RRC_SERVICE_IND)
  {
    if((service_status == MM_AS_NO_SERVICE) && (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN))
    {
      /*Resetting leaving connected mode scan for the case when plmn list request follows no-service in service indication
        Or else, we don't handle the immediate plmn list request properly*/
      tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_CELL_SELECTION_SCAN;
    }
  }

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    if(cmd_id == RRC_SERVICE_IND_FROM_OTA_MSG || cmd_id == RRC_SERVICE_IND || cmd_id == RRC_SERVICE_CNF)
    {
      if(service_status == MM_AS_SERVICE_AVAILABLE)
      {
        /* Get HS Capability Indication */
        new_hs_ind = tdsrrc_get_3g_cell_capability_indicator();
        if(cmd_id == RRC_SERVICE_CNF)
        {
          tdshsdpa_hsupa_ind = new_hs_ind;
          tdsrrc_send_3g_cell_capability_information(tdshsdpa_hsupa_ind);
        }
        else
        {
          if (new_hs_ind == tdshsdpa_hsupa_ind)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:New cell same HS Cap %d",tdshsdpa_hsupa_ind);
          }
          else
          {
            tdshsdpa_hsupa_ind = new_hs_ind;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Send new HS Cap %d",tdshsdpa_hsupa_ind);
            tdsrrc_send_3g_cell_capability_information(tdshsdpa_hsupa_ind);
          }
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:OOS detected. Change cell status");
        if(cmd_id == RRC_SERVICE_CNF)
        {
          tdsrrc_send_3g_cell_capability_information(SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL);
        }
        else
        {
          if(tdshsdpa_hsupa_ind != SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL)
          {
            tdsrrc_send_3g_cell_capability_information(SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL);
          }
        }
        tdshsdpa_hsupa_ind = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
      }
    }
  }
  /* Now set the Last CSP event */
  tdsrrc_csp_int_data.last_mm_primitive = 
    tdsrrccsp_mm_primitive_sent(cmd_id, service_status);

  /* Cleanup any stored G channels */
  tdsrrc_reset_oos_due_to_no_resource_flag();

  return(SUCCESS);
} /* tdsrrccsp_send_mm_cmd */


/*===========================================================================

FUNCTION          RRC_CSP_SEND_MM_PLMN_LIST_CNF_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for PLMN List Confirm command to MM sent by CSP.
                  This also allocates cmd buffer and sends the command.
                  It also frees allocated command and embedded buffers
                  when failure is detected before sending the command.

DEPENDENCIES
                  
RETURN VALUE
                  SUCCESS: Command was successfully sent to MM.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  For time being it always sends an PLMN_LIST_CNF with the 
                  no PLMNs.
===========================================================================*/
uecomdef_status_e_type  
tdsrrccsp_send_mm_plmn_list_cnf_cmd(boolean srch_done)
{
  mm_cmd_type *mm_buf_ptr;
  uint32 i, j;
  rrc_to_mm_sink_e_type   mm_cmd_id = RRC_MM_SINK_CMD_MAX; 

#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"curr_scan %d, prev_curr_scan %d, svc_srch_bgnd %d", 
                                          tdsrrc_csp_int_data.curr_scan, 
                                          tdsrrc_csp_int_data.prev_curr_scan, 
                                          tdsrrc_csp_int_data.service_search_bgnd);
#endif

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_PLMN_LIST_CNF)) == NULL)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"No MM buffer for CSP to MM cmd 0x%x",RRC_PLMN_LIST_CNF);
    return(FAILURE);
  }

  if(!srch_done)
  {
    tdsrrc_csp_int_data.plmn_list.num_plmns = rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found = 0;
#ifdef FEATURE_TDSCDMA_TO_LTE
    rrc_csp_td2g_bplmn_int_data.td2l_plmns_found = 0;
#endif
  }

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.as_id = tdsrrc_get_as_id();
#endif

  /* Fill the PLMN list in the CSP internal data into the one 
   * to be sent in PLMN_LIST_CNF to MM
   */
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.num_plmns = 
                     tdsrrc_csp_int_data.plmn_list.num_plmns + rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found;

#ifdef FEATURE_TDSCDMA_TO_LTE
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.num_plmns += rrc_csp_td2g_bplmn_int_data.td2l_plmns_found;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TD2L: LTE PLMNs = %d ",rrc_csp_td2g_bplmn_int_data.td2l_plmns_found);
#endif

  /* TDD upmerge: RRC_MAX_PLMNS defined in rrcmmif.h. Ok to use by TDD
  */
  if(mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.num_plmns > RRC_MAX_PLMNS)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Number of PLMNS %d > RRC_MAX_PLMNS",mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.num_plmns);
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.num_plmns = RRC_MAX_PLMNS;
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDS PLMNs = %d,GSM PLMNs = %d",tdsrrc_csp_int_data.plmn_list.num_plmns,rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found);

  for(i=0; i < tdsrrc_csp_int_data.plmn_list.num_plmns; i++)
  {
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].plmn = tdsrrc_csp_int_data.plmn_list.plmn[i].plmn;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].rat = tdsrrc_csp_int_data.plmn_list.plmn[i].rat;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].service_capability = tdsrrc_csp_int_data.plmn_list.plmn[i].service_capability;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_quality = tdsrrc_csp_int_data.plmn_list.plmn[i].signal_quality;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_strength = tdsrrc_csp_int_data.plmn_list.plmn[i].signal_strength;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding TDS PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(tdsrrc_csp_int_data.plmn_list.plmn[i].plmn),TDSRRC_CSP_GET_INT_MNC(tdsrrc_csp_int_data.plmn_list.plmn[i].plmn));
  }

  for(j=0; j < rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found; j++, i++)
  {
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].plmn = 
                                            rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].plmn;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].rat  = 
                                            rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].rat;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].service_capability = 
                                            rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].service_capability;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_quality = 
                                            rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].signal_quality;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_strength = 
                                            rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].signal_strength;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding GSM PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].plmn),TDSRRC_CSP_GET_INT_MNC(rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].plmn));
  }

#ifdef FEATURE_TDSCDMA_TO_LTE
  for(j=0; j < rrc_csp_td2g_bplmn_int_data.td2l_plmns_found && i < RRC_MAX_PLMNS; j++, i++)
  {
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].plmn = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].plmn;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].rat  = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].rat;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].service_capability = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].service_capability;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_quality = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].signal_quality;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_strength = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].signal_strength;
#ifdef FEATURE_TDSCDMA_PSEUDO_SIB19
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].earfcn = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].earfcn;

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Adding LTE PLMN %d-%d, EARFCN %d", 
                TDSRRC_CSP_GET_INT_MCC(rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].plmn),
                TDSRRC_CSP_GET_INT_MNC(rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].plmn), 
                rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].earfcn);
#else
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding LTE PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].plmn),TDSRRC_CSP_GET_INT_MNC(rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].plmn));
#endif /* FEATURE_TDSCDMA_PSEUDO_SIB19 */
  }
#endif

  mm_buf_ptr->cmd.rrc_plmn_list_cnf.network_select_mode = SYS_NETWORK_SELECTION_MODE_MANUAL;
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.trans_id = tdsrrc_csp_int_data.bplmn_trans_id;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SSB:: Service Search in PLMN List CNF %d",tdsrrc_csp_int_data.service_search_bgnd);

  if(srch_done)
  {
    /* Set the status field to MM_AS_SUCCESS */
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.status = MM_AS_SUCCESS;
  }
  else
  {
    /* Set the status field to MM_AS_ABORTED */
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.status = MM_AS_ABORTED;
  }

  /* Put the command on MM queue */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending MM cmd 0x%x",RRC_PLMN_LIST_CNF);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Trans I.D in BPLMN_LIST_CNF=%d",mm_buf_ptr->cmd.rrc_plmn_list_cnf.trans_id );
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  mm_put_cmd(mm_buf_ptr);

  /* Since UE is OOS, determine if NO SERVICE shd be sent to NAS or not */
  tdsrrc_csp_int_data.curr_service_status     = MM_AS_NO_SERVICE;
  tdsrrc_csp_int_data.curr_sel_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;


#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  tdsrrc_csp_int_data.detected_frequency_list.num_freqs = 0;
#endif

  if(tdsrrccsp_cmd_to_mm_needed(&mm_cmd_id,
                              tdsrrc_csp_int_data.last_mm_primitive, 
                              tdsrrc_csp_int_data.curr_service_status)
     == TRUE)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"last_mm_prim = %d. Snd cmd %d to NAS",tdsrrc_csp_int_data.last_mm_primitive,mm_cmd_id);

    if(tdsrrccsp_send_mm_cmd(mm_cmd_id,
                           tdsrrc_csp_int_data.curr_service_status, 
                           &tdsrrc_csp_int_data.curr_sel_plmn,
                           &tdsrrc_csp_int_data.curr_acq_entry) 
       == FAILURE)
    {
      ERR_FATAL("RRC Unable to send MM cmd 0x%x",mm_cmd_id,0,0);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"last_mm_prim = %d. Don't send Service Ind with No Service to NAS",tdsrrc_csp_int_data.last_mm_primitive);
  }
  tdsrrcwrm_release_lock();
  tdsrrcwrm_stop_lock_wait_timer();
  return(SUCCESS);
} /* tdsrrccsp_send_mm_plmn_list_cnf_cmd */


/*===========================================================================

FUNCTION          RRC_CSP_SEND_MM_PLMN_LIST_CNF_AS_ABORTED

DESCRIPTION       This function is responsible for filling up data structures
                  for PLMN List Confirm command to MM sent by CSP.
                  This function is called when PLMN_LIST_REQ from NAS needs to
                  be aborted immediately without affecting the ongoing BPLMN
                  search (if it is in progress). The n/w select mode in 
                  PLMN_LIST_CNF is always set to MANUAL in this function .

DEPENDENCIES
                  None
                  
RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void tdsrrccsp_send_mm_plmn_list_cnf_as_aborted(uint8 trans_id)
{
  mm_cmd_type *mm_buf_ptr;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Manual PLMN Search Aborted. Send PLMN_LIST_CNF");
  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_PLMN_LIST_CNF)) == NULL)
  {
    ERR_FATAL("No MM buffer for CSP to MM cmd 0x%x",RRC_PLMN_LIST_CNF,0,0);
  }

  mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.num_plmns = 0;

  mm_buf_ptr->cmd.rrc_plmn_list_cnf.network_select_mode = SYS_NETWORK_SELECTION_MODE_MANUAL;

  /* Set the status field to MM_AS_SUCCESS */
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.status = MM_AS_ABORTED;

  mm_buf_ptr->cmd.rrc_plmn_list_cnf.trans_id = trans_id;

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.as_id = tdsrrc_get_as_id();
#endif

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Trans I.D in BPLMN_LIST_CNF=%d",mm_buf_ptr->cmd.rrc_plmn_list_cnf.trans_id );
  /* Put the command on MM queue */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending MM cmd 0x%x",RRC_PLMN_LIST_CNF);
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  mm_put_cmd(mm_buf_ptr);
  
} /* tdsrrccsp_send_mm_plmn_list_cnf_as_aborted */



/*===========================================================================

FUNCTION          RRC_CSP_BPLMN_SEND_MM_PLMN_LIST_CNF_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for PLMN List Confirm command to MM sent by CSP. 
                  This function sends both TDS and GSM/LTE PLMNs found to NAS.

DEPENDENCIES
                  NONE
                  
RETURN VALUE
                  SUCCESS: Command was successfully sent to MM.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  NONE
===========================================================================*/
uecomdef_status_e_type  
tdsrrccsp_bplmn_send_mm_plmn_list_cnf_cmd(td2td_bplmn_srch_status_type status, uint8 trans_id)
{
  mm_cmd_type *mm_buf_ptr;
  uint32 i, j;  
#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY  
  boolean list_cnf_for_new_req = FALSE;
#endif

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
  if (trans_id != tdsrrc_csp_int_data.bplmn_trans_id)
  {
    list_cnf_for_new_req = TRUE;
  }

  if((tdsrrc_csp_int_data.send_partial_results) &&
     ( !list_cnf_for_new_req))
  {
    tdsrrc_csp_int_data.plmn_list.num_plmns = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MPLMN Opt: Partial Results,total plmns %d",tdsrrc_csp_int_data.plmn_list.num_plmns + rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found);
  }
  else
#endif
  {
  if(status == TD2TD_SEARCH_STOP)
  {
    tdsrrc_csp_int_data.plmn_list.num_plmns = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;
#ifdef FEATURE_TDSCDMA_TO_LTE
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"status = TD2TD_SEARCH_STOP, TDS plmns %d, GSM plmns %d, LTE plmns %d",
               tdsrrc_csp_int_data.plmn_list.num_plmns, 
               rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found,
               rrc_csp_td2g_bplmn_int_data.td2l_plmns_found);
#else
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"status = TD2TD_SEARCH_STOP, TDS plmns %d,GSM plmns %d",tdsrrc_csp_int_data.plmn_list.num_plmns,rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found);
#endif
  }
  else if(status == TD2TD_SEARCH_ABORT)
  {
    tdsrrc_csp_int_data.plmn_list.num_plmns = rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found = 0;
#ifdef FEATURE_TDSCDMA_TO_LTE
    rrc_csp_td2g_bplmn_int_data.td2l_plmns_found = 0;
#endif
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"status = TD2TD_SEARCH_ABORT");
  }
  else
  {
    tdsrrc_csp_int_data.plmn_list.num_plmns = rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found = 0;
    ERR_FATAL("ERR: status = %d",0,0,0);
  }                                            
  }  

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_PLMN_LIST_CNF)) == NULL)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"No MM buffer for CSP to MM cmd 0x%x",RRC_PLMN_LIST_CNF);
    return(FAILURE);
  } 
  
#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.as_id = tdsrrc_get_as_id();
#endif

  /* Store TDS PLMNs */
  for(i=0; i < tdsrrc_csp_int_data.plmn_list.num_plmns && i < RRC_MAX_PLMNS; i++)
  {
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].plmn = tdsrrc_csp_int_data.plmn_list.plmn[i].plmn;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].rat = tdsrrc_csp_int_data.plmn_list.plmn[i].rat;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].service_capability = tdsrrc_csp_int_data.plmn_list.plmn[i].service_capability;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_quality = tdsrrc_csp_int_data.plmn_list.plmn[i].signal_quality;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_strength = tdsrrc_csp_int_data.plmn_list.plmn[i].signal_strength;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding TDS PLMN %d--%d",TDSRRC_CSP_GET_INT_MCC(tdsrrc_csp_int_data.plmn_list.plmn[i].plmn),TDSRRC_CSP_GET_INT_MNC(tdsrrc_csp_int_data.plmn_list.plmn[i].plmn));
  }

  /* Store G PLMNs */
  for(j=0; j < rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found && i < RRC_MAX_PLMNS; j++, i++)
  {
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].plmn = 
                                            rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].plmn;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].rat  = 
                                            rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].rat;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].service_capability = 
                                            rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].service_capability;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_quality = 
                                            rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].signal_quality;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_strength = 
                                            rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].signal_strength;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding GSM PLMN %d--%d",TDSRRC_CSP_GET_INT_MCC(rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].plmn),TDSRRC_CSP_GET_INT_MNC(rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[j].plmn));
  }

#ifdef FEATURE_TDSCDMA_TO_LTE
  for(j=0; j < rrc_csp_td2g_bplmn_int_data.td2l_plmns_found && i < RRC_MAX_PLMNS; j++, i++)
  {
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].plmn = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].plmn;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].rat  = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].rat;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].service_capability = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].service_capability;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_quality = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].signal_quality;
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].signal_strength = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].signal_strength;
#ifdef FEATURE_TDSCDMA_PSEUDO_SIB19
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.plmn[i].earfcn = 
                                            rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].earfcn;

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Adding LTE PLMN %d--%d, EARFCN %d", 
                TDSRRC_CSP_GET_INT_MCC(rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].plmn),
                TDSRRC_CSP_GET_INT_MNC(rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].plmn), 
                rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].earfcn);
#else
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding LTE PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].plmn),TDSRRC_CSP_GET_INT_MNC(rrc_csp_td2g_bplmn_int_data.lte_plmn_list.plmn[j].plmn));
#endif /* FEATURE_TDSCDMA_PSEUDO_SIB19 */
  }
#endif

  /* Fill the PLMN list in the CSP internal data into the one 
   * to be sent in PLMN_LIST_CNF to MM
   */
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.avail_plmn_list.num_plmns = i;

  mm_buf_ptr->cmd.rrc_plmn_list_cnf.network_select_mode = tdsrrc_csp_int_data.bplmn_nw_sel_mode;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"n/w sel mode in PLMN_LIST_CNF = %d",tdsrrc_csp_int_data.bplmn_nw_sel_mode);
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.trans_id = trans_id;
#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
  if(status == TD2TD_SEARCH_PARTIAL)
  {
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.status = MM_AS_SEARCH_PARTIAL;
    tdsrrc_csp_int_data.send_partial_results    = FALSE;
  }
  else
#endif
  if(status == TD2TD_SEARCH_ABORT)
  {
#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
    if (list_cnf_for_new_req == FALSE)
    {
      memset(&tdsrrc_csp_int_data.bplmn_search_timer_value,0,sizeof(tds_manual_search_timer_value_type));
      tdsrrc_csp_int_data.send_partial_results = FALSE;
      tdsrrctmr_stop_timer(TDSRRCTMR_BPLMN_PARTIAL_REPORT_TIMER);
    }
#endif
    /* Set the status field to MM_AS_ABORTED */
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.status = MM_AS_ABORTED;
    if(tdsrrc_get_oos_due_to_no_resource_flag_status() &&
       (tdsrrc_csp_int_data.bplmn_nw_sel_mode == SYS_NETWORK_SELECTION_MODE_MANUAL))
    { 
      mm_buf_ptr->cmd.rrc_plmn_list_cnf.status = MM_AS_ABORTED_DUE_TO_NO_LOCK;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No TRM for MANUAL BPLMN search");
    }
  }
  else
  {
#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
    if (list_cnf_for_new_req == FALSE)
    {
      memset(&tdsrrc_csp_int_data.bplmn_search_timer_value,0,sizeof(tds_manual_search_timer_value_type));
      tdsrrc_csp_int_data.send_partial_results = FALSE;
      tdsrrctmr_stop_timer(TDSRRCTMR_BPLMN_PARTIAL_REPORT_TIMER);
    }
#endif
    /* Set the status field to MM_AS_SUCCESS */
    mm_buf_ptr->cmd.rrc_plmn_list_cnf.status = MM_AS_SUCCESS;
  }

#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
  mm_buf_ptr->cmd.rrc_plmn_list_cnf.service_search = tdsrrc_csp_int_data.service_search_bgnd;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SSB: svc_srch_bgnd %d sent in plmn_list_cnf",tdsrrc_csp_int_data.service_search_bgnd);
#endif

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
  if(status != TD2TD_SEARCH_PARTIAL)
#endif
  {
  /* Clear the backed up BPLMN data */
  tdsrrc_csp_bplmn_data_saved = FALSE;
  }
  /* Put the command on MM queue */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending MM cmd 0x%x",RRC_PLMN_LIST_CNF);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Trans I.D in BPLMN_LIST_CNF=%d",mm_buf_ptr->cmd.rrc_plmn_list_cnf.trans_id );
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  mm_put_cmd(mm_buf_ptr);

  /*reset drx count*/
  tdsrrc_csp_int_data.bplmn_drx_cnt = 0;
    
  return(SUCCESS);
} /* tdsrrccsp_bplmn_send_mm_plmn_list_cnf_cmd */

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
/*===========================================================================

FUNCTION          TDSRRC_GET_INTER_RAT_BPLMN_PRTL_RESULT

DESCRIPTION       This API provides partial found PLMN results to RR during ongoing GtoTD
                  Manual PLMN Search

DEPENDENCIES
                  None
                  
RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
boolean tdsrrc_get_inter_rat_bplmn_prtl_result(sys_detailed_plmn_list_s_type *available_plmn_list)
{
  int i;
  boolean status;
  rrc_plmn_identity_type nas_to_rrc_plmn_id;
  /* Perform an atomic operation*/
  TDS_TASKLOCK();
  for(i=0; i<tdsrrc_csp_int_data.bplmn_fnd_list_wr_index; ++i)
  {
    available_plmn_list->info[i].plmn_service_capability
        = tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[i].plmn_service_capability;
    available_plmn_list->info[i].plmn
        = tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[i].plmn; 
    available_plmn_list->info[i].rat
        = tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[i].rat;
    available_plmn_list->info[i].signal_quality
        = tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[i].signal_quality;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"signal strength >= -95db is considered as SYS_SIGNAL_QUALITY_HIGH");        
    available_plmn_list->info[i].signal_strength
        = tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[i].signal_strength;
    available_plmn_list->info[i].list_category = SYS_DETAILED_PLMN_LIST_CATEGORY_OTHER;
    available_plmn_list->info[i].plmn_forbidden = FALSE;
    nas_to_rrc_plmn_id = mmumts_convert_nas_plmn_id_to_rrc_plmn_id(available_plmn_list->info[i].plmn);

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"MPLMN Opt: plmns in partial result api PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(nas_to_rrc_plmn_id),TDSRRC_CSP_GET_INT_MNC(nas_to_rrc_plmn_id));
  }
  
  available_plmn_list->length = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;
  TDS_TASKFREE();
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"MPLMN Opt: Providing %d Parial Results to RR",available_plmn_list->length);

  if(available_plmn_list->length > 0)  
  {
    status = TRUE;
  }
  else
  {
    status = FALSE;
  }
  return(status);
}

/*===========================================================================

FUNCTION          TDSRRC_GET_INTER_RAT_BPLMN_PRTL_RESULT_SUB_ID

DESCRIPTION       
  This function serves for the same purpose as tdsrrc_get_inter_rat_bplmn_prtl_result, with TDS
  as_id provided for Dual_SIM.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
boolean tdsrrc_get_inter_rat_bplmn_prtl_result_sub_id(
  sys_detailed_plmn_list_s_type *available_plmn_list,
  sys_modem_as_id_e_type as_id
)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  return(tdsrrc_get_inter_rat_bplmn_prtl_result(available_plmn_list));
}

/*===========================================================================

FUNCTION          tdsrrccsp_send_mplmn_partial_report_to_mm

DESCRIPTION       This function is responsible for filling up data structures
                  for PLMN List Confirm command to MM sent by CSP.
                  This function is called when partial MPLMN results needs to be informed to NAS

DEPENDENCIES
                  None
                  
RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void tdsrrccsp_send_mplmn_partial_report_to_mm(void)
{
  int i; 
  sys_detailed_plmn_list_s_type available_plmn_list;
  lte_irat_get_plmn_prtl_results_req_s plmn_srch_req;
  available_plmn_list.length=0;

  memset(&plmn_srch_req,0,sizeof(lte_irat_get_plmn_prtl_results_req_s));

  if(tdsrrc_csp_int_data.bplmn_search_timer_value.MorePLMNTimer > 0)
  {
    tdsrrctmr_start_timer(TDSRRCTMR_BPLMN_PARTIAL_REPORT_TIMER, (unsigned long)(1000*tdsrrc_csp_int_data.bplmn_search_timer_value.MorePLMNTimer));
  }
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  if(rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active
     && tdsrrc_csp_int_data.bplmn_interrat_type == TDSRRC_CSP_INTERRAT_LTE)
  {
    tdsrrc_csp_int_data.send_partial_results = TRUE;
    msgr_init_hdr(&plmn_srch_req.msg_hdr,MSGR_TDSCDMA_RRC,LTE_RRC_TDSCDMA_GET_PLMN_PRTL_RESULTS_REQ);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    msgr_set_hdr_variant(&plmn_srch_req.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
    msgr_set_hdr_inst(&plmn_srch_req.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
    msgr_send(&plmn_srch_req.msg_hdr, sizeof(lte_irat_get_plmn_prtl_results_req_s));

	if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
	{
	  tdscfa_send_message_to_lte_rrc(&plmn_srch_req.msg_hdr, sizeof(lte_irat_get_plmn_prtl_results_req_s));
	}
	TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2L:Sent LTE_RRC_TDSCDMA_GET_PLMN_PRTL_RESULTS_REQ to LTE RRC");
  }
  else
#endif
  {
  if(rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active
     && tdsrrc_get_rr_inter_rat_bplmn_prtl_result(&available_plmn_list)) 
  {
    for(i=0; i < available_plmn_list.length; ++i)
    {
      rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[i].plmn = 
            mmumts_convert_nas_plmn_id_to_rrc_plmn_id(
              available_plmn_list.info[i].plmn);
   
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"%d-%d, GSM RAT %d", 
                                 TDSRRC_CSP_GET_INT_MCC(rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[i].plmn),
                                 TDSRRC_CSP_GET_INT_MNC(rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[i].plmn), 
                                 available_plmn_list.info[i].rat);
     
      rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[i].rat = 
              available_plmn_list.info[i].rat;
   
      rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[i].service_capability = 
              available_plmn_list.info[i].plmn_service_capability;
     
      rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[i].signal_quality = 
              available_plmn_list.info[i].signal_quality;
   
      rrc_csp_td2g_bplmn_int_data.plmn_list.plmn[i].signal_strength = 
              available_plmn_list.info[i].signal_strength;
    }
    
    rrc_csp_td2g_bplmn_int_data.wtog_gsm_plmns_found = 
                                available_plmn_list.length;
  }

  tdsrrc_csp_int_data.send_partial_results = TRUE;
  tdsrrccsp_bplmn_send_mm_plmn_list_cnf_cmd(TD2TD_SEARCH_PARTIAL, tdsrrc_csp_int_data.bplmn_trans_id);
  }
} /* tdsrrc_csp_send_mm_plmn_list_cnf_as_aborted */
#endif /* FEATURE_PARTIAL_MPLMN_DISPLAY */

/*===========================================================================

FUNCTION          RRC_CSP_SEND_CCM_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for Cell Change Request sent to CCM.
                  This also allocates cmd buffer and sends the command.
                  It also frees allocated command and embedded buffers
                  when failure is detected before sending the command.

DEPENDENCIES
                  
RETURN VALUE
                  SUCCESS: Command was successfully sent to CCM.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_send_ccm_cmd(tdsrrc_cmd_e_type  ccm_cmd, 
                     tdsrrc_new_cell_status_e_type  cell_camp_status,
                     tdsrrc_csp_acq_entry_type  *acq_entry_ptr)
{

  tdsrrc_cmd_type   *csp_out_cmd_ptr;
  tdsrrc_csp_access_control_e_type  access_control;
  tdsrrc_dsac_info_type rrc_dsac_info;
  //mm_cmd_type *mm_buf_ptr;
  rrc_service_info_type dummy_service_info;
  rrc_plmn_identity_type common_plmn_id;
  /* Check validity of Acq Entry pointer */
  if((cell_camp_status == TDSRRC_NEW_CELL_SELECTED) && (acq_entry_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Passed Acq Entry pointer is NULL");
    return(FAILURE);
  }

  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    csp_out_cmd_ptr->cmd_hdr.cmd_id = ccm_cmd;
    csp_out_cmd_ptr->cmd.cell_change_req.req_proc_id = TDSRRC_PROCEDURE_CSP;
    csp_out_cmd_ptr->cmd.cell_change_req.new_cell_camp_status = cell_camp_status;

    if(cell_camp_status == TDSRRC_NEW_CELL_SELECTED)
    {

  if(acq_entry_ptr->is_valid_nw_sharing)
  {
    common_plmn_id = acq_entry_ptr->shared_list_of_plmns.common_plmn_id;
  }
  else
  {
    common_plmn_id = acq_entry_ptr->plmn_id;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sib_idx PLMN: MCC %d-MNC %d",TDSRRC_CSP_GET_INT_MCC(common_plmn_id),TDSRRC_CSP_GET_INT_MNC(common_plmn_id));
      if(tdsrrccsp_find_access_control(&access_control, &rrc_dsac_info, 
                                     acq_entry_ptr->plmn_id,
                                     acq_entry_ptr->cell_id
                                     ,common_plmn_id
                                     )
         == FAILURE)
      {
        ERR_FATAL("Failure finding cell access status",0,0,0); 
      }

      /* Fill in the transition type based on the current scan in CSP internal data */
      if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)
#ifdef FEATURE_WRLF_SYSTEM_SEL
          && !TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()
#endif
        )
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                           TDSRRC_CHANGE_CELL_SELECTION;
      }
      else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_RESELECTION_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                           TDSRRC_CHANGE_CELL_RESELECTION;
      }
      /* If curr_scan is OOS_area_scan and current RRC state is DCH then it 
       * implies that the cell selection was triggered due to CELL_DCH ->
       * CELL_FACH/CELL_PCH reconfiguration/RL_falire.
       * Hence based on the destination state, send appropriate cell_change_type
       * to CCM
       */
      else if(((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
#ifdef FEATURE_WRLF_SYSTEM_SEL
                   || TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()
#endif
               )  && (TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()))
      {
        if(tdsrrc_csp_int_data.dch_tx_dest_state == TDSRRC_STATE_CELL_FACH)
        {
          csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                             TDSRRC_CHANGE_CELL_DCH_TO_CELL_FACH;
        }
        else if(tdsrrc_csp_int_data.dch_tx_dest_state == TDSRRC_STATE_CELL_PCH)
        {
          csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                             TDSRRC_CHANGE_CELL_DCH_TO_CELL_PCH;
        }
        else
        {
          csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                             TDSRRC_CHANGE_CELL_DCH_TO_URA_PCH;
        }
      }
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                           TDSRRC_CHANGE_CELL_FACH_TO_CELL_FACH;
      }
      else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                           TDSRRC_CHANGE_CELL_FACH_TO_CELL_PCH;
      }
      else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                           TDSRRC_CHANGE_CELL_FACH_TO_URA_PCH;
      }
      /* If curr_scan is OOS_area_scan and current RRC state is DCH then it 
       * implies that the cell selection was triggered due to CELL_DCH ->
       * CELL_FACH/CELL_PCH reconfiguration/RL_falire.
       * Hence based on the destination state, send appropriate cell_change_type
       * to CCM
       */
      else if(((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
#ifdef FEATURE_WRLF_SYSTEM_SEL
          || TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()
#endif
        ) && (TDSRRC_STATE_CELL_FACH == tdsrrc_get_state()))
      {
        if(tdsrrc_csp_int_data.fach_tx_dest_state == TDSRRC_STATE_CELL_FACH)
        {
          csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                             TDSRRC_CHANGE_CELL_FACH_TO_CELL_FACH;
        }
        else if(tdsrrc_csp_int_data.fach_tx_dest_state == TDSRRC_STATE_CELL_PCH)
        {
          csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                             TDSRRC_CHANGE_CELL_FACH_TO_CELL_PCH;
        }
        else if(tdsrrc_csp_int_data.fach_tx_dest_state == TDSRRC_STATE_URA_PCH)
        {
          csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                             TDSRRC_CHANGE_CELL_FACH_TO_URA_PCH;
        }
        else
        {
          csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                             TDSRRC_CHANGE_RETURN_TO_SERVICE_AREA;
        }
      }
#endif
      else if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
#ifdef FEATURE_WRLF_SYSTEM_SEL
          || (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()
          && (TDSRRC_STATE_DISCONNECTED != tdsrrc_get_state()))
#endif
        )
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                           TDSRRC_CHANGE_RETURN_TO_SERVICE_AREA;
      }
      else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                           TDSRRC_CHANGE_CELL_DCH_TO_CELL_FACH;
      }
      else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                           TDSRRC_CHANGE_CELL_DCH_TO_CELL_PCH;
      }
      else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                           TDSRRC_CHANGE_CELL_DCH_TO_URA_PCH;
      }
      else  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                           TDSRRC_CHANGE_CELL_SELECTION;
      } 
      else  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                      TDSRRC_CHANGE_CELL_SELECTION;
      }
      else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_CELL_RESELECTION_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type = 
                                            TDSRRC_CHANGE_CELL_SELECTION;
      }
      else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type = 
                                            TDSRRC_CHANGE_CELL_SELECTION;
      }
#ifdef FEATURE_INTERRAT_PCCO_G2TD
      else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_CC_ORDER_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type = 
                                            TDSRRC_CHANGE_CELL_SELECTION;
      }
#endif
      else  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_CONNECTING_STATE_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
                                      TDSRRC_CHANGE_RETURN_TO_SERVICE_AREA;
      }
      else  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN)
      {
        csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type =
            TDSRRC_CHANGE_RETURN_TO_SERVICE_AREA;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR Unknown scan type received");
      }

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Scan %d,cell chg type %d, RRC st %d", tdsrrc_csp_int_data.curr_scan,
                  csp_out_cmd_ptr->cmd.cell_change_req.cell_change_type,
                  tdsrrc_get_state());

      csp_out_cmd_ptr->cmd.cell_change_req.cell_id = acq_entry_ptr->cell_id;
      csp_out_cmd_ptr->cmd.cell_change_req.plmn_id = acq_entry_ptr->plmn_id;
      csp_out_cmd_ptr->cmd.cell_change_req.freq = acq_entry_ptr->freq;
      csp_out_cmd_ptr->cmd.cell_change_req.cpid = acq_entry_ptr->cpid;

      /* Now determine the cell access status for this cell */  
#ifdef FEATURE_DSAC      
      csp_out_cmd_ptr->cmd.cell_change_req.dsac_present = FALSE;
#endif
      csp_out_cmd_ptr->cmd.cell_change_req.is_valid_nw_sharing = acq_entry_ptr->is_valid_nw_sharing;
      csp_out_cmd_ptr->cmd.cell_change_req.common_plmn_id = acq_entry_ptr->shared_list_of_plmns.common_plmn_id;
      (void)tdsrrccsp_find_rrc_cell_access(&dummy_service_info, &rrc_dsac_info,
                                                          access_control,acq_entry_ptr->status_mask);
      if(rrc_dsac_info.valid != TRUE)
      {
         csp_out_cmd_ptr->cmd.cell_change_req.new_cell_access = dummy_service_info.cell_access;
      }
      else
      {
#ifdef FEATURE_DSAC
        csp_out_cmd_ptr->cmd.cell_change_req.dsac_present = TRUE;
        csp_out_cmd_ptr->cmd.cell_change_req.new_cs_cell_access = dummy_service_info.cs_cell_access;
        csp_out_cmd_ptr->cmd.cell_change_req.new_ps_cell_access = dummy_service_info.ps_cell_access;
#endif
      }       
    } /* if(cell_camp_status == TDSRRC_NEW_CELL_SELECTED) */
  } /* If able to get int cmd buffer */
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR Unable to get cmd buf for cmd 0x%x",ccm_cmd);
    return(FAILURE);
  }

  /* Send the Internal cmd to RRC */
  tdsrrc_put_int_cmd( csp_out_cmd_ptr );
  return(SUCCESS);

} /* tdsrrccsp_send_ccm_cmd */


/*===========================================================================

FUNCTION          RRCCSP_GET_SERVING_PCCPCH_PARMS

DESCRIPTION       This function fills up scrambling code and STTD indicator
                  for serving cell PCCPCH in the passed parameters.
           
DEPENDENCIES
                  The caller has to ensure that Cell selection would have 
                  got valid values for scrambling code and STTD indicator
                  before this function call is made.

RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_get_serving_pccpch_parms(boolean *tstd_ptr)

{

  *tstd_ptr     = tdsrrc_csp_int_data.serving_pccpch_parms.tstd_indicator;

}



/*===========================================================================

FUNCTION          RRCCSP_GET_CURRENT_PLMN_CELL_ID

DESCRIPTION       This function fills up PLMN & Cell ID.
           
DEPENDENCIES
                  The caller has to ensure that this function will not be called for reselection
                  and also only after CELL_TRANSITION_REQ is sent for cell selection.

RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_get_current_plmn_cell_id(rrc_plmn_identity_type *plmn_id, tdsrrc_int_cell_id_type *cell_id)
{
  *plmn_id = tdsrrc_csp_int_data.curr_acq_entry.plmn_id;
  *cell_id = tdsrrc_csp_int_data.curr_acq_entry.cell_id;

  if(tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing)
  {
    *plmn_id = tdsrrc_csp_int_data.curr_acq_entry.shared_list_of_plmns.common_plmn_id;
  }

}


/*===========================================================================

FUNCTION          RRCCSP_GET_NEIGHBOR_PCCPCH_PARMS

DESCRIPTION       This function fills up scrambling code
                  for neighbor cell PCCPCH in the passed parameter.
           
DEPENDENCIES
                  The caller has to ensure that Cell selection would have 
                  got valid value for scrambling code
                  before this function call is made.

RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_get_neighbor_pccpch_parms(uint8 *cpid_ptr, tdsrrc_freq_type *freq_ptr)
{
  *cpid_ptr = tdsrrc_csp_int_data.new_cell_acq_entry.cpid;
  *freq_ptr     = tdsrrc_csp_int_data.new_cell_acq_entry.freq;
} /* tdsrrccsp_get_neighbor_pccpch_parms */

/*===========================================================================

FUNCTION          RRC_CSP_HANDLE_NO_AVAILABLE_FREQ

DESCRIPTION       This function handles failure to find any frequency for 
                  acquisition. Based on the current service status an dthe last
                  MM primitive exchanged, it determines if and which command
                  needs to be sent to MM.It then sends the command to MM.
           
DEPENDENCIES
                  None
RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void  tdsrrccsp_handle_no_available_freq(void)
{
  rrc_to_mm_sink_e_type   mm_cmd_id = RRC_MM_SINK_CMD_MAX; 

  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN)
  {
    /* Start the full freq scan timer during which no full scans are done
     * for automatic/manual PLMN searches
     */
    tdsrrccsp_check_and_start_plmn_list_full_freq_scan_timer();

    /* Send the PLMN_LIST_CNF to MM */
    if(tdsrrccsp_send_mm_plmn_list_cnf_cmd(TRUE) == FAILURE)
    {
      ERR_FATAL("Not able to send PLMN List Cnf to MM",0,0,0);
    }
    /* Reset the CSP internal data */
    tdsrrccsp_init_int_data();


    return;
  }

  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"OOS in Conn Mode: Sending No Svc to MM. PLMNs found %d ",tdsrrc_csp_int_data.plmn_list.num_plmns);
  }                           

  if(tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq. found for ANY PLMN");
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"No freq. found for PLMN MCC %d MNC %d",TDSRRC_CSP_GET_INT_MCC(tdsrrc_csp_int_data.curr_req_plmn.plmn_id),TDSRRC_CSP_GET_INT_MNC(tdsrrc_csp_int_data.curr_req_plmn.plmn_id));
  }

  /* Send a Service Confirmation back to MM informing that no PLMN was found */
  tdsrrc_csp_int_data.curr_service_status     = MM_AS_NO_SERVICE;
  tdsrrc_csp_int_data.curr_sel_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;

  if(tdsrrccsp_cmd_to_mm_needed(&mm_cmd_id,
                              tdsrrc_csp_int_data.last_mm_primitive, 
                              tdsrrc_csp_int_data.curr_service_status)
     == TRUE)
  {
    if(tdsrrccsp_send_mm_cmd(mm_cmd_id,
                           tdsrrc_csp_int_data.curr_service_status, 
                           &tdsrrc_csp_int_data.curr_sel_plmn,
                           &tdsrrc_csp_int_data.curr_acq_entry) 
       == FAILURE)
    {
      ERR_FATAL("RRC Unable to send MM cmd 0x%x",mm_cmd_id,0,0);
    }
  }


}   /* tdsrrccsp_handle_no_available_freq */

/*===========================================================================

FUNCTION          RRC_CSP_SEND_CHAN_RELEASE

DESCRIPTION       This function sends a Channel Config Request to LLC to 
                  release all channels.
           
DEPENDENCIES
                  None
RETURN VALUE
                  SUCCESS/FAILURE.

SIDE EFFECTS
                  None

===========================================================================*/
uecomdef_status_e_type  tdsrrccsp_send_chan_release(void)
{
  tdsrrc_cmd_type        *csp_out_cmd_ptr;
  /* Start with sending an TDSRRC_CHANNEL_CONFIG_REQ to LLC to release all channels.
   */
  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Populate the cmd data */
    csp_out_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;
    csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_DISCONNECTED;
    csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = TRUE;
    csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 0;
    csp_out_cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_CSP;
    csp_out_cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = FALSE;
    csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
      TDSRRCLLC_CHAN_CFG_REASON_NONE;

    tdsrrc_put_int_cmd( csp_out_cmd_ptr );
    return(SUCCESS);
  }
  else
  {
    return(FAILURE);
  }
} /* tdsrrccsp_send_chan_release */


/*===========================================================================

FUNCTION          tdsrrccsp_send_chan_reconfig

DESCRIPTION       This function sends a Channel Config Request to LLC to 
                  reconfig channels.
           
DEPENDENCIES
                  None
RETURN VALUE
                  SUCCESS/FAILURE.

SIDE EFFECTS
                  None

===========================================================================*/
uecomdef_status_e_type  tdsrrccsp_send_chan_reconfig(void)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  tdsrrc_cmd_type        *csp_out_cmd_ptr;
  /* Start with sending an TDSRRC_CHANNEL_CONFIG_REQ to LLC to release all channels.
   */
  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Populate the cmd data */
    csp_out_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;
    csp_out_cmd_ptr->cmd.chan_config_req.next_state = rrc_state;
    csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
    csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 1;
    /* indicate the logical channel to be reconfigured based on 
    the current substate */
    switch (rrc_state)
    {
      case TDSRRC_STATE_CELL_FACH:
      case TDSRRC_STATE_CONNECTING:
        csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = CCCH_RADIO_BEARER_ID;
        csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSRECONFIG_RB;
        break;

      case TDSRRC_STATE_DISCONNECTED:
      case TDSRRC_STATE_CELL_PCH:
      case TDSRRC_STATE_URA_PCH:
        csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = PCCH_RADIO_BEARER_ID;
        csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSRECONFIG_RB;
        break;

      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Should not come here");
        break;
    }
    csp_out_cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_CSP;
    csp_out_cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = FALSE;
    csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
      TDSRRCLLC_CHAN_CFG_REASON_RECONFIG_PCH_FACH;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sending chan config to reconfig rb:%d rrcst:%d",csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id,rrc_state);
    tdsrrc_put_int_cmd( csp_out_cmd_ptr );
    return(SUCCESS);
  }
  else
  {
    return(FAILURE);
  }
} /* tdsrrccsp_send_chan_reconfig */



/*===========================================================================

FUNCTION          RRCCSP_REL_CHLS_AND_GO_TO_IDLE

DESCRIPTION       This function sends a channel config request to release all
                  channels and take UE to idle. It also checks if the channel
                  release will result in a state change to DISCONNECTED from
                  a non-disconnected state so that CSP can wait for the state
                  change indication before sending NO_SERVICE to MM.
           
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate

SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type  tdsrrccsp_rel_chls_and_go_to_idle(void)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"UE going from %d to IDLE",rrc_state);
  
  tdsrrccsp_cleanup_bplmn_state();

  /* Release all channels and put lower layers to Idle  */
  if(tdsrrccsp_send_chan_release() == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  if(rrc_state != TDSRRC_STATE_DISCONNECTED)
  {
    /* Set the CSP substate to TDSRRCCSP_CELL_SEL_WT_STATE_CHANGE, where
     * on getting the STATE_CHANGE_IND (to IDLE), CSP will send
     * NO SERVICE to MM.
     */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSP waits for state change ind");
    return(TDSRRCCSP_CELL_SEL_WT_STATE_CHANGE);
  }
  /* if BPLMN search is in progress then send to RR - no plmn found */
  else if( (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) 
           || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN))
  {
    /* Set data that needs to be reported to RR */
    tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.srch_done   = TRUE;
    tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.hplmn_found = FALSE;
      
    /* stop TDS stack */
    tdsrrccsp_send_mode_change_req_to_mcm(TDSRRC_MODE_INACTIVE);
    return(TDSRRCCSP_WT_STOP_MODE_CHANGE_CNF);
  }
  else
  {
    tdsrrccsp_handle_no_available_freq();
    return(TDSRRCCSP_NO_CELL_SELECTED);
  }
} /* tdsrrccsp_rel_chls_and_go_to_idle */


/*===========================================================================

FUNCTION          RRC_CSP_SEND_NEXT_FREQ_TO_L1

DESCRIPTION       This function sends either an Acquisition Request or a 
                  Frequency scan request to L1 (via LLC) based on the passed
                  type of frequency scan.
                  The parameters for TDSL1_CPHY_ACQ_REQ and TDSL1_CPHY_FREQ_SCAN_REQ are
                  passed in the acq_entry_ptr.
           
DEPENDENCIES
                  None
RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void  tdsrrccsp_send_next_freq_to_l1
(
 tdsrrc_csp_acq_entry_type       *acq_entry_ptr,
 tdsrrc_csp_freq_scan_e_type     type_freq_scan
)
{
  if(type_freq_scan != TDSRRC_CSP_NO_SCAN)
  {
    /* Start with sending an TDSRRC_RESET_L1_REQ to L1 before a TDSL1_CPHY_ACQ_REQ or a 
     * TDSL1_CPHY_FREQ_SCAN_REQ for Raw Scan.
     */

    if( ((tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
          && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
        || ((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
            && (TDSRRC_CAMPED_ON != tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_CSP))))
    {
      /* No need to send IDLE_REQ for TDD because tearing down BCH should have been called before. */
    }
  }
  else
  {
    ERR_FATAL("type_freq_scan set to NO_SCAN",0,0,0);
  }

  if(type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
  {
    /* Initialize the current acquisition count in CSP internal database */
    tdsrrc_csp_int_data.curr_acq_count = 0;
    tdsrrc_csp_int_data.num_cpid_det = 0;

    if ((tdsrrc_csp_int_data.tds_continue_lfs) && (tdsrrc_csp_int_data.acq_freq_list.num_freq > 0))
    {
      boolean unbar = FALSE;
      
      if (tdsrrc_csp_int_data.acq_freq_list.curr_freq_index == tdsrrc_csp_int_data.acq_freq_list.num_freq - 1)
      {
        /* Restart from the beginning. Should not come here for TDD */
        tdsrrc_csp_int_data.acq_freq_list.curr_freq_index = 0; 
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Should have been handled by tdsrrccsp_get_list_db_match_acq_entry()");
      }
      else
        tdsrrc_csp_int_data.acq_freq_list.curr_freq_index++;
    
      /* Send ACQ_REQ to L1 and indicate to them that they can send a list of scrambling codes in
       * the ACQ_CNF
       */
          
      if (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED)
        unbar = TRUE;
      else
        unbar = FALSE;
      
      if(tdsrrccsp_send_l1_tds_acq_req(TDSRRC_PROCEDURE_CSP, 
                                        tdsrrc_csp_int_data.acq_freq_list.freq_list,
                                        tdsrrc_csp_int_data.acq_freq_list.curr_freq_index,
                                        tdsrrc_csp_int_data.acq_freq_list.num_freq,
                                        unbar) == SUCCESS)
      {
        TDSRRC_MSG0(MSG_LEGACY_MED,"L1 cmd TDSL1_CPHY_ACQ_REQ sent for TDSRRC_CSP_ACQ_SCAN");
      }
      else
      {
        ERR_FATAL("RRC Unable to get cmd buf for L1 cmd TDSL1_CPHY_ACQ_REQ",0,0,0);
      }
      // ZZTBD: continue
    }
    else
    {
      if(tdsrrccsp_send_l1_cmd(TDSRRC_PROCEDURE_CSP, TDSL1_CPHY_ACQ_REQ, acq_entry_ptr) == SUCCESS)
      {
        TDSRRC_MSG0(MSG_LEGACY_MED,"L1 cmd TDSL1_CPHY_ACQ_REQ sent legacy");
      }
      else
      {
        ERR_FATAL("RRC Unable to get cmd buf for L1 cmd TDSL1_CPHY_ACQ_REQ legacy",0,0,0);
      }
    }
  }
  /* TDD can do ACQ on list of frequencies */
  else if(type_freq_scan == TDSRRC_CSP_LIST_SCAN)
  {
    boolean unbar = FALSE;
    int i = 0;

    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
      && ((!tdsrrc_csp_int_data.g2t_redi_freq_compl 
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
          && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_GSM)
#endif
          )
#ifdef FEATURE_LTE_TO_TDSCDMA 
          || (!tdsrrc_csp_int_data.l2t_redi_freq_compl && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE))
#endif
         )
      && (!tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq)
      )
    {
      for(i=0;i<tdsrrc_csp_int_data.num_freq_in_list_db;i++)
      {
        /* Configure the frequencies in the list for the list freq scan in the L1 frequency scan command */
        tdsrrc_csp_int_data.acq_freq_list.freq_list[i] = tdslist_db[i].freq;
        tdsrrc_csp_int_data.acq_freq_list.source[i] = X2T_Redirection;
        /* Update list_scan data structure as well */
        tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[i].ctr_freq = tdslist_db[i].freq;
	    tdsrrc_csp_curr_select_data.scan_info.list_scan.source[i] = X2T_Redirection;
      }
      /* Set the number of frequencies in list scan in the L1 frequency scan command */
      tdsrrc_csp_int_data.acq_freq_list.num_freq = tdsrrc_csp_int_data.num_freq_in_list_db;
      tdsrrc_csp_int_data.acq_freq_list.curr_freq_index = 0;

      tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq = tdsrrc_csp_int_data.num_freq_in_list_db;
      
    }
    else
    {
      int freq_count = 0;
      
      /* Check for OTA message redirect frequency */
      if (tdsrrc_csp_int_data.dest_freq_present
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
          && (tdsrrc_csp_int_data.interrat_type != TDSRRC_CSP_INTERRAT_LTE)
#endif
          )
      {
         tdsrrc_csp_int_data.acq_freq_list.source[freq_count] = Freq_Redirection;
         tdsrrc_csp_int_data.acq_freq_list.freq_list[freq_count++] = tdsrrc_csp_int_data.dest_freq;
      }

      for(i=0;i<tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq;i++)
      {
        if (freq_count < TDSL1_MAX_SCAN_FREQ)
        {
		   /* CR399680 filter out repeated frequency */
           if ((tdsrrc_csp_int_data.dest_freq_present)
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
               && (tdsrrc_csp_int_data.interrat_type != TDSRRC_CSP_INTERRAT_LTE)
#endif
              )
           {
              if (tdsrrc_csp_int_data.dest_freq == tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[i].ctr_freq)
              {
                 TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDD LFS: skip adding freq. %d which is already in the list",tdsrrc_csp_int_data.acq_freq_list.freq_list[0]);
                 continue;
              }
           }

           tdsrrc_csp_int_data.acq_freq_list.source[freq_count] = 
                 tdsrrc_csp_curr_select_data.scan_info.list_scan.source[i];
      
           /* Configure the frequencies in the list for the list freq scan in the L1 frequency scan command */
           tdsrrc_csp_int_data.acq_freq_list.freq_list[freq_count++] = 
             tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[i].ctr_freq;
        }
        else
        {
           TDSRRC_MSG2(MSG_LEGACY_ERROR,"TDS LFS: exceeded max list %d. Freq %d dropped.",freq_count,tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[i].ctr_freq);
        }
      }
      /* Set the number of frequencies in list scan in the L1 frequency scan command */
      tdsrrc_csp_int_data.acq_freq_list.num_freq = freq_count;

      /* ZZTBD: Use current index in case this is the continuation of previous interrupted PLMN search */
      if (tdsrrc_csp_int_data.acq_freq_list.curr_freq_index <    //Leo: ????
            tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index)
         tdsrrc_csp_int_data.acq_freq_list.curr_freq_index = tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index;
      else
      {
         tdsrrc_csp_int_data.acq_freq_list.curr_freq_index = tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index;
         tdsrrc_csp_int_data.acq_freq_list.curr_freq_index++; 
      }
      /* tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index will be updated 
       * in rrccsp_cell_sel_wait_acq_acq according to actual returned freq. from L1
       */
    }

    /* Initialize the current acquisition count in CSP internal database */
    tdsrrc_csp_int_data.curr_acq_count = 0;
    tdsrrc_csp_int_data.num_cpid_det = 0;

    if (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED)
      unbar = TRUE;
    else
      unbar = FALSE;
    
    if(tdsrrccsp_send_l1_tds_acq_req(TDSRRC_PROCEDURE_CSP, 
                                      tdsrrc_csp_int_data.acq_freq_list.freq_list,
                                      tdsrrc_csp_int_data.acq_freq_list.curr_freq_index,
                                      tdsrrc_csp_int_data.acq_freq_list.num_freq,
                                      unbar) == SUCCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"L1 cmd TDSL1_CPHY_ACQ_REQ sent for TDSRRC_CSP_LIST_SCAN");
    }
    else
    {
      ERR_FATAL("RRC Unable to get cmd buf for L1 cmd TDSL1_CPHY_ACQ_REQ LFS",0,0,0);
    }
  }  /* Got buffer to send cmd */
  else  /* Type of scan - Not Acquisition */
  {
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
    /* When it comes to FS, tdsrrc_csp_int_data.l2t_redi_freq_compl must be set to 0 */
    if (!tdsrrc_csp_int_data.l2t_redi_freq_compl)
    {
       TDSRRC_MSG1(MSG_LEGACY_ERROR, "tdsrrc_csp_int_data.l2t_redi_freq_compl is still FALSE,%d",tdsrrc_csp_int_data.l2t_redi_freq_compl);
       tdsrrc_csp_int_data.l2t_redi_freq_compl = TRUE;
    }
#endif
    /* Haizhou 2012/10/29
	 * When to do freq scan, means redirection freq has been tried out. then set g2t_redi_freq_compl as TRUE
	 */
    if (!tdsrrc_csp_int_data.g2t_redi_freq_compl)
    {
       TDSRRC_MSG1(MSG_LEGACY_ERROR, "tdsrrc_csp_int_data.g2t_redi_freq_compl is still FALSE,%d",tdsrrc_csp_int_data.g2t_redi_freq_compl);
       tdsrrc_csp_int_data.g2t_redi_freq_compl = TRUE;
    }
    /* set is_mset_compl_for_rlf_oos as TRUE if start frequency scan*/
    if (tdsrrc_csp_int_data.is_mset_compl_for_rlf_oos == FALSE)
    {
      tdsrrc_csp_int_data.is_mset_compl_for_rlf_oos = TRUE;
    }

    /* Send a frequency scan request */
    if(tdsrrccsp_send_l1_freq_scan_cmd(TDSRRC_PROCEDURE_CSP,
                                     acq_entry_ptr->freq,
                                     type_freq_scan)
       == SUCCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"L1 cmd TDSL1_CPHY_FREQ_SCAN_REQ sent");

    }
    else
    {
      ERR_FATAL("RRC Unable to get cmd buf for L1 cmd TDSL1_CPHY_FREQ_SCAN_REQ",0,0,0);
    }
  } /* Type of scan - Not Acquisition */

} /* tdsrrccsp_send_next_freq_to_l1 */


/*===========================================================================

FUNCTION          tdsrrccsp_send_tr_phy_chl_release

DESCRIPTION       This function sends a Channel Config Request to LLC to 
                  release Transport and Physical channels.
           
DEPENDENCIES
                  None.
RETURN VALUE
                  SUCCESS/FAILURE.
SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type tdsrrccsp_send_tr_phy_chl_release(void)
{
  tdsrrc_cmd_type        *csp_out_cmd_ptr;
  /* Start with sending an TDSRRC_CHANNEL_CONFIG_REQ to LLC to release all channels.
   */
  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Populate the cmd data */
    csp_out_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;
    csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_DISCONNECTED;
    csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
    /* No operation on RB */
    csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 0;
    /* Release Physical and Transport Channels */
    csp_out_cmd_ptr->cmd.chan_config_req.release_phychls = TRUE;
    csp_out_cmd_ptr->cmd.chan_config_req.release_trchls = TRUE;

    csp_out_cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_CSP;
    csp_out_cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = FALSE;

    if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) ||
       (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_CONNECTING_STATE_SCAN) ||
       (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN) 
#ifdef FEATURE_WRLF_SYSTEM_SEL
       || ((TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()) &&
            (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN))
#endif
      )
    {
      csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
        TDSRRCLLC_CHAN_CFG_REASON_OUT_OF_SERVICE;
    }
    else
    {
      csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
        TDSRRCLLC_CHAN_CFG_REASON_NONE;
    }

    tdsrrc_put_int_cmd( csp_out_cmd_ptr );
    return(SUCCESS);
  }
  else
  {
    return(FAILURE);
  }

} /* tdsrrccsp_send_tr_phy_chl_release */


/*===========================================================================

FUNCTION          tdsrrccsp_send_bch_release

DESCRIPTION       This function sends a Channel Config Request to LLC to 
                  release BCCH/BCH/S_PCCPCH pipe.
           
DEPENDENCIES
                  None.
RETURN VALUE
                  SUCCESS/FAILURE.
SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type tdsrrccsp_send_bch_release(boolean cnf_required)
{
  tdsrrc_cmd_type        *csp_out_cmd_ptr;
  /* Start with sending an TDSRRC_CHANNEL_CONFIG_REQ to LLC to release all channels.
   */
  if((csp_out_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Populate the cmd data */
    csp_out_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;
    csp_out_cmd_ptr->cmd.chan_config_req.next_state = TDSRRC_STATE_DISCONNECTED;
    csp_out_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;

    /* No operation on RB */
    csp_out_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 1;
    csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_id = BCCH_S_RADIO_BEARER_ID;
    csp_out_cmd_ptr->cmd.chan_config_req.rb[0].rb_config = TDSRELEASE_RB;

    /* Release Physical and Transport Channels */
    csp_out_cmd_ptr->cmd.chan_config_req.release_phychls = FALSE;
    csp_out_cmd_ptr->cmd.chan_config_req.release_trchls = FALSE;

    csp_out_cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_CSP;
    csp_out_cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = cnf_required;

    csp_out_cmd_ptr->cmd.chan_config_req.chan_config_reason = 
      TDSRRCLLC_CHAN_CFG_REASON_NONE;

    tdsrrc_put_int_cmd( csp_out_cmd_ptr );
    return(SUCCESS);
  }
  else
  {
    return(FAILURE);
  }

} /* tdsrrccsp_send_bch_release */

/*===========================================================================

FUNCTION          RRC_CSP_CMD_TO_MM_NEEDED

DESCRIPTION       This function determines if there is a need to send a 
                  command to MM. If a command is needed to be sent, then
                  which command is needed is also determined.
                  The determination is made based on the last primitive
                  exchanged between MM and CSP and the new service status.
           
DEPENDENCIES
                  None
RETURN VALUE
                  TRUE: Command needs to be sent.
                  FALSE: No command needed.

SIDE EFFECTS
                  None

===========================================================================*/
boolean  tdsrrccsp_cmd_to_mm_needed
( 
  rrc_to_mm_sink_e_type  *mm_cmd_id,
  tdsrrc_csp_mm_primitive_e_type  last_mm_primitive, 
  mm_as_service_state_T      new_service_status
)
{
  boolean  cmd_to_mm = FALSE;
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();


  switch(last_mm_primitive)
  {
    case TDSRRC_CSP_ACT_REQ_RCVD:
      cmd_to_mm = TRUE;
      break;

    case TDSRRC_CSP_SRVC_REQ_RCVD:
      cmd_to_mm = TRUE;
      *mm_cmd_id = RRC_SERVICE_CNF;
      break;

    case TDSRRC_CSP_ACT_CNF_SENT:
      cmd_to_mm = TRUE;
      *mm_cmd_id = RRC_SERVICE_IND;
      break;

    case TDSRRC_CSP_SRVC_CNF_WITH_NO_SRVC_SENT:
    case TDSRRC_CSP_SRVC_IND_WITH_NO_SRVC_SENT:
      /* Since a Service Indication with NO_SERVICE was sent before,
       * another Service Indication is sent only when service is available.
       */
      if(new_service_status == MM_AS_SERVICE_AVAILABLE)
      {
        cmd_to_mm = TRUE;
        *mm_cmd_id = RRC_SERVICE_IND;
      }
      break;

    case TDSRRC_CSP_SRVC_CNF_WITH_SRVC_SENT:
    case TDSRRC_CSP_SRVC_IND_WITH_SRVC_SENT:
      cmd_to_mm = TRUE;
      *mm_cmd_id = RRC_SERVICE_IND;
      break;

    case TDSRRC_CSP_PLMN_LIST_REQ_RCVD:
      cmd_to_mm = TRUE;
      *mm_cmd_id = RRC_SERVICE_IND;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid event %d when sending MM cmd",last_mm_primitive);
      break;
  }

  /* Don't send the RRC_SERVICE_IND in any state other than DISCONNECTED or
   * CONNECTING
   */
  if(*mm_cmd_id == RRC_SERVICE_IND)
  {
    if((rrc_state != TDSRRC_STATE_DISCONNECTED) && (rrc_state != TDSRRC_STATE_CONNECTING))
    {
      cmd_to_mm = FALSE;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Svc ind NOT sent to MM in RRC state %d",rrc_state);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending svc ind to MM");
    }
  }

  /* Notify any location information to the registered callback 
   * if UE is not in Cell_DCH State */
  if(*mm_cmd_id == RRC_SERVICE_IND || *mm_cmd_id == RRC_SERVICE_CNF )
  {
    if (tdsrrc_home_zone_ind_cb_func)
    {
      tdsrrc_trigger_home_zone_ind();
    }
  }

  /* Notify any location information to the registered callback 
   * if UE is not in Cell_DCH State */
  if(*mm_cmd_id == RRC_SERVICE_IND || *mm_cmd_id == RRC_SERVICE_CNF )
  {
    /* Notify the serving cell id change indication to the registered callbacks 
     * if UE is not in Cell_DCH State */
    tdsrrccsp_notify_cell_id_change();
  }
  return(cmd_to_mm);
}   /* tdsrrccsp_cmd_to_mm_needed */

/*===========================================================================

FUNCTION          tdsrrccsp_mm_primitive_sent

DESCRIPTION       This function determines the new MM primitive to be sent
                  based on the command for MM and service status.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Last CSP-MM primitive.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_mm_primitive_e_type  tdsrrccsp_mm_primitive_sent
(
  rrc_to_mm_sink_e_type  last_mm_cmd_id,
  mm_as_service_state_T      last_service_status
)
{
  tdsrrc_csp_mm_primitive_e_type  mm_primitive;

  switch(last_mm_cmd_id)
  {
    case RRC_SERVICE_CNF:
      if(last_service_status == MM_AS_SERVICE_AVAILABLE)
      {
        mm_primitive = TDSRRC_CSP_SRVC_CNF_WITH_SRVC_SENT;
      }
      else
      {
        mm_primitive = TDSRRC_CSP_SRVC_CNF_WITH_NO_SRVC_SENT;
      }
      break;

    case RRC_SERVICE_IND:
      if(last_service_status == MM_AS_SERVICE_AVAILABLE)
      {
        mm_primitive = TDSRRC_CSP_SRVC_IND_WITH_SRVC_SENT;
      }
      else
      {
        mm_primitive = TDSRRC_CSP_SRVC_IND_WITH_NO_SRVC_SENT;
      }
      break;
#ifdef FEATURE_DSAC
    case RRC_SERVICE_IND_FROM_OTA_MSG:
      if(last_service_status == MM_AS_SERVICE_AVAILABLE)
      {
        mm_primitive = TDSRRC_CSP_SRVC_IND_WITH_SRVC_SENT;
      }
      else
      {
        mm_primitive = TDSRRC_CSP_SRVC_IND_WITH_NO_SRVC_SENT;
      }
      break;
#endif

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MM cmd %d: No CSP event",last_mm_cmd_id);
      mm_primitive = TDSRRC_CSP_SRVC_IND_WITH_SRVC_SENT;
      break;
  }

  return(mm_primitive);

} /* tdsrrccsp_mm_primitive_sent */

/*===========================================================================

FUNCTION          tdsrrccsp_init_data_from_service_req

DESCRIPTION       This function stores the data passed in TDSRRC_SERVICE_REQ
                  and initializes RRC data and CSP local data with the passed
                  data.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void  tdsrrccsp_init_data_from_service_req(rrc_service_req_type  *rrc_service_req)
{
  uint32 i;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type sub_id = rrc_service_req->as_id;
  
  if((sub_id < SYS_MODEM_AS_ID_1) || (sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
  	TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC: invalid sub_id %d", sub_id);
  	return;
  }
#endif
  /* Initilize relevant fields of Internal data here */
  tdsrrc_csp_int_data.last_mm_primitive      = TDSRRC_CSP_SRVC_REQ_RCVD;
  tdsrrc_csp_int_data.curr_req_plmn          = rrc_service_req->requested_plmn;
  tdsrrc_csp_int_data.curr_service_status    = MM_AS_NO_SERVICE;

  tdsrrc_csp_int_data.skip_gsm_search = rrc_service_req->grat_scan_done;
  tdsrrc_csp_int_data.can_ehplmn_be_camped = rrc_service_req->is_ehplmn_to_be_camped;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tdsrrc_nv_rel_indicator: 0x%x",tdsrrc_nv_rel_indicator);
  
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_special_settings_nv_read: %d,tdsrrc_special_settings_for_testing_nv: %d",tdsrrc_special_settings_nv_read,tdsrrc_special_settings_for_testing_nv);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tds_nv_optional_feature_list: 0x%x",tds_nv_optional_feature_list);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tdsrrc_nv_band_search_mask: 0x%x",tdsrrc_nv_band_search_mask);
  
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"tds_nv_interrat_feature_ctrl: 0x%x,tds_nv_optional_ueci_list: 0x%x",tds_nv_interrat_feature_ctrl,tds_nv_optional_ueci_list);
  

  tdsrrc_csp_int_data.skip_gsm_search = FALSE;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"G-RAT Scan done in Service Req is %d",tdsrrc_csp_int_data.skip_gsm_search);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"SKIP_GSM: in Service Req is %d from NAS %d",tdsrrc_csp_int_data.skip_gsm_search,rrc_service_req->grat_scan_done);


  /* Initialize the num_plmns to zero for every service request */
  tdsrrc_csp_int_data.plmn_list.num_plmns = 0;

  /* Set this var to FALSE. It signifies that RRC has NOT found any PLMN yet */
  tdsrrc_csp_int_data.plmn_fnd_during_fgnd_svc_srch = FALSE;

  /* Initialize the service capability to SYS_SRV_CAPABILITY_NONE for every service request */
  for(i=0; i< RRC_MAX_PLMNS; i++)
  {
    tdsrrc_csp_int_data.plmn_list.plmn[i].service_capability = SYS_SRV_CAPABILITY_NONE;
  }

  tdsrrc_csp_int_data.curr_sel_plmn.plmn_type   = RRC_GSM_MAP_PLMN_SEL;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  tdsrrc_csp_int_data.forbid_lai_list[sub_id] = rrc_service_req->forbid_lai_list;
#else
  tdsrrc_csp_int_data.forbid_lai_list = rrc_service_req->forbid_lai_list;
#endif
  tdsrrc_csp_int_data.network_select_mode = rrc_service_req->network_select_mode;
  tdsrrc_csp_int_data.band_pref           = rrc_service_req->band_pref;

#ifdef FEATURE_RAT_PRIORITY_LIST
  tdsrrccsp_store_rat_pri_list_to_int_data(&rrc_service_req->rat_pri_list_info);
  tdsrrc_csp_int_data.band_pref = tdsrrccsp_extract_tds_band_pref_from_service_req(&tdsrrc_csp_int_data.rat_pri_list_info);
  tdsrrc_csp_int_data.band_pref_gw = tdsrrccsp_extract_gw_band_pref_from_service_req(&tdsrrc_csp_int_data.rat_pri_list_info);
#endif

  TDSRRC_MSG4(MSG_LEGACY_HIGH,"RRC_SERVICS_REQ: requested PLMN(%d-%d), PLMN type = %d, NW sel mode = %d",
              TDSRRC_CSP_GET_INT_MCC(tdsrrc_csp_int_data.curr_req_plmn.plmn_id),
              TDSRRC_CSP_GET_INT_MNC(tdsrrc_csp_int_data.curr_req_plmn.plmn_id),
              tdsrrc_csp_int_data.curr_req_plmn.plmn_type,
              tdsrrc_csp_int_data.network_select_mode);

  tdsrrc_csp_int_data.service_req_cause = rrc_service_req->service_req_cause;

#ifdef FEATURE_TDSCDMA_SCAN_SCOPE
  tdsrrc_csp_int_data.srvc_req_scan_scope = rrc_service_req->scan_scope;
  TDSRRC_MSG4(MSG_LEGACY_HIGH,"Service req cause : %d, new_scan = %d, acq_scan_type = %d, scan_scope = %d",
              tdsrrc_csp_int_data.service_req_cause,
              rrc_service_req->new_scan, 
              rrc_service_req->acq_scan_type,
              tdsrrc_csp_int_data.srvc_req_scan_scope);
#else
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Service req cause : %d, new_scan = %d, acq_scan_type = %d",
              tdsrrc_csp_int_data.service_req_cause,
              rrc_service_req->new_scan, 
              rrc_service_req->acq_scan_type);
#endif

#ifdef FEATURE_WRLF_SYSTEM_SEL
  /* Do not initialize RCE if connected mode OOS search is in progress*/
  if(!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
  {
    /* As we have got a new service request, Initialize RCE just in case it was waiting for UE to camp on,
     * e.g., TDSRRC_EST_REQ. Abort is sent to MM by this function, if needed.
     */
    tdsrrcrce_force_initialize(TRUE);
  }

  /* Sending Drop-All which takes L1 to SYNC. This is needed because 
   * L1 may be in any state when it receives upcoming commands for cell selection. 
   * This is an extra protection in RRC.
   */
  if(tdsrrccsp_send_llc_cmd(TDSRRC_RESET_L1_REQ) == FAILURE)
  {
    ERR_FATAL("RRC Unable to get cmd buf for RRC cmd 0x%x",TDSRRC_RESET_L1_REQ,0,0);
  }

  /*CR-151*/
  if((tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION) ||
     (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED))
  {
      /* No need to send IDLE_REQ for TDD.
       * Assume TDD L1 already in idle or sync state
       */
    /* Unbar on any cell selection */
    tdsrrc_csp_int_data.bar_for_resel_only = TRUE;
    /* Send Cell bar request to L1 to unbar all the frequencies barred due to CR151*/
    tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_FREQ_BARRED,0);
    tdsrrc_csp_int_data.bar_for_resel_only = FALSE;
  }

  /* For emergency call service search:
     - Stop the full scan and deep sleep no service timer.
     - Reset the deep sleep band scanned mask and no service band scanned mask.
     So that for emergency call, UE performs full scan everytime on all bands. 
   * If UE is in deep sleep and full scan timer is still not started with counter value
     less than 3, the counter will get incremented during LIMITED search cycles. As a 
     result the counter may reach max value 3 and start the full scan timer. In this case,
     if the LIMITED search fails UE will not perform full scan for later service requests
     till the full scan timer expires.
   */
  if((tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED)
     && (rrc_service_req->service_req_cause == RRC_MO_CS_CALL))
  {
    if(!tdsrrc_csp_int_data.full_scan_needed)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deep Sleep: Stopping 30mins timer");
      tdsrrctmr_stop_timer(TDSRRCTMR_FREQ_SCAN_TIMER);
      tdsrrc_csp_int_data.full_scan_needed = TRUE;
    }
    if(!tdsrrc_csp_int_data.deep_sleep_no_svc_tmr_expired)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deep Sleep: Stopping no service timer");
      tdsrrctmr_stop_timer(TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER);
      tdsrrc_csp_int_data.deep_sleep_no_svc_tmr_expired = TRUE;
    }
    tdsrrc_csp_int_data.deep_sleep_band_scanned_mask = 0;
    tdsrrc_csp_int_data.deep_sleep_no_svc_band_scanned_mask = 0;
  } 


  {

    tdsrrccsp_update_band_preference();


    tdsrrc_csp_curr_select_data.first_band_to_be_scanned = RRC_CSP_TDS_A_BAND;

    /* Reset the next_band_mask appropriately */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the next_band_mask");
    tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_int_data.tds_band_mask;
    /* Record next_band_mask for deep search*/
    tdsrrc_csp_curr_select_data.next_band_mask_for_deep = tdsrrc_csp_curr_select_data.next_band_mask;

    tdsrrc_csp_curr_select_data.first_band_to_be_scanned = TDSRRC_CSP_BAND_MAX;
    
      if((tdsrrc_csp_curr_select_data.first_band_to_be_scanned =
          tdsrrccsp_get_first_band_to_be_scanned()) == TDSRRC_CSP_BAND_MAX)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"UE doesn't support a valid TD-SCDMA band");
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Defaulting to TDD Band A");
        tdsrrc_csp_curr_select_data.first_band_to_be_scanned = RRC_CSP_TDS_A_BAND;
        tdsrrc_csp_curr_select_data.current_band = 
          tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
      }
      else
      {
        tdsrrc_csp_curr_select_data.current_band = 
          tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"1st band to be scanned = 0x%x",tdsrrc_csp_curr_select_data.first_band_to_be_scanned);
      }

      


  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"UE in DS %d",tdsrrc_csp_int_data.ue_in_deep_sleep);

  /* Initialize GSM list size to 0 */

  /* If UE is in deep sleep then store the agc values for LFS and FFS in the current select data of CSP, 
      also store the deep sleep scan type based on the scan type received in the 
      SERVICE_REQ  
  */
  tdsrrc_csp_int_data.do_lfs_scan = TRUE;
    /* Store the NAS requested PLMN Id into CSP current selecte data */
    tdsrrc_csp_curr_select_data.req_plmn.plmn_id = rrc_service_req->requested_plmn.plmn_id;
  /* save the trm time out value from NAS which is in the unit of second */
  tdsrrc_csp_int_data.trm_timeout_value = rrc_service_req->trm_timeout_value;
  if(tdsrrc_csp_int_data.trm_timeout_value != 0xFFFFFFFF)
  {
    tdsrrc_csp_int_data.trm_timeout_value *= 1000;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: rcved trm_timeout_value %dms in service req",tdsrrc_csp_int_data.trm_timeout_value);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: trm_timeout_value 0xffffffff in service req");
  }
} /* tdsrrccsp_init_data_from_service_req */

/*===========================================================================

FUNCTION          tdsrrccsp_init_bplmn_data

DESCRIPTION       This function stores the data passed in TDSRRC_INTERRAT_PLMN_SRCH_REQ
                  and initializes RRC data and CSP local data.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void  tdsrrccsp_init_bplmn_data(void)
{
  uint32 i;

  /* Initialize relevant fields of Internal data here */
  if(tdsrrc_csp_int_data.bplmn_new_srch) 
  {
    tdsrrc_csp_int_data.curr_service_status          = MM_AS_NO_SERVICE;
    /* No primitive received from MM */
    tdsrrc_csp_int_data.last_mm_primitive            = TDSRRC_CSP_NO_EVENT_RCVD;  

    /* If 3G->3G BPLMN search is not in progress then initialize
     * 2G->3G BPLMN search data
     */
    if((tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
        && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
    {
      /* Initialize the service capability to SYS_SRV_CAPABILITY_NONE for every service request */
      for(i = 0; i < SYS_PLMN_LIST_MAX_LENGTH; i++)  
      {
        tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[i].plmn_service_capability
                                                        = SYS_SRV_CAPABILITY_NONE;
      }
    }

    tdsrrc_csp_int_data.curr_sel_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;

    /* Process the special case when MM does not give UE's Low Access Class */
    if(tdsrrc_lo_access_class == RRC_LO_ACCESS_CLASS_ABSENT)
    {
      //Since UE's Access Class is missing, the Requested PLMN type will be changed
      //to Any PLMN Request if it is not so already. Take care that this is  done 
      //only if Original Requested PLMN type was not ANSI 41.    
      if(tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_SPECIFIC_PLMN_REQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE Access Class Absent: Change Req PLMN type to ANY PLMN");
        tdsrrc_csp_int_data.curr_req_plmn.plmn_type = RRC_GSM_MAP_ANY_PLMN_REQ;
      }
    }

    
    /* Reset the next_band_mask appropriately */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the next_band_mask");
    tdsrrc_csp_curr_select_data.next_band_mask           = tdsrrc_csp_int_data.tds_band_mask;


    tdsrrc_csp_curr_select_data.first_band_to_be_scanned = TDSRRC_CSP_BAND_MAX;

      if((tdsrrc_csp_curr_select_data.first_band_to_be_scanned =
          tdsrrccsp_get_first_band_to_be_scanned()) == TDSRRC_CSP_BAND_MAX)
      {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"UE doesn't support a valid TD-SCDMA band");
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Defaulting to TDD Band A");
          tdsrrc_csp_curr_select_data.first_band_to_be_scanned = RRC_CSP_TDS_A_BAND;
          tdsrrc_csp_curr_select_data.current_band = tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
      }
      else
      {
          tdsrrc_csp_curr_select_data.current_band = tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"1st band to be scanned = 0x%x",tdsrrc_csp_curr_select_data.first_band_to_be_scanned);
      }



    TDSRRC_MSG1(MSG_LEGACY_HIGH,"UE in DS %d",tdsrrc_csp_int_data.ue_in_deep_sleep);
  }
} /* tdsrrccsp_init_bplmn_data */



/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_on_mm_req

DESCRIPTION       This function starts a cell selection in response to
                  a newly received service request from MM. The acquisition 
                  type is initialized based on the requested PLMN type. The 
                  current scan is set to Cell selection scan.
                  Based on the returned frequency scan type, a CSP substate
                  is returned.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_on_mm_req(void)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan;

  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;


#if defined (FEATURE_LTE_TO_TDSCDMA) || defined (FEATURE_TDSCDMA_TO_LTE)|| defined (FEATURE_TDSCDMA_DEDICATED_PRI_INFO)
  tdsrrcmeas_send_get_dedicated_priority_req();
#endif

  /* Set current scan to "Cell Selection".*/   
  tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_CELL_SELECTION_SCAN;


  /* Update GSM with UE's band preference */
  tdsrr_set_band_pref_internal(tdsrrc_csp_int_data.band_pref_gw);

  /* Reset the next_band_mask appropriately */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the next_band_mask");
  tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_int_data.tds_band_mask;
  /* Record next_band_mask for deep search*/
  tdsrrc_csp_curr_select_data.next_band_mask_for_deep = tdsrrc_csp_curr_select_data.next_band_mask;
  tdsrrc_csp_curr_select_data.num_available_freq = 0;
  
  if(tdsrrccsp_start_cell_selection(&tdsrrc_csp_int_data.curr_req_plmn, 
                                  &tdsrrc_csp_int_data.curr_type_acq, 
                                  &tdsrrc_csp_int_data.curr_acq_entry,
                                  &type_freq_scan) 
    == FAILURE)
  {
#ifdef FEATURE_WRLF_SYSTEM_SEL
    if(((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN) ||
                (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN))
              && TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
    {
      tdsrrc_csp_int_data.curr_service_status     = MM_AS_NO_SERVICE;
      tdsrrc_csp_int_data.curr_sel_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;

      if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)
      {
        if(tdsrrccsp_send_mm_cmd(RRC_SERVICE_CNF,
                               tdsrrc_csp_int_data.curr_service_status, 
                               &tdsrrc_csp_int_data.curr_sel_plmn,
                               &tdsrrc_csp_int_data.curr_acq_entry) 
           == FAILURE)
        {
          ERR_FATAL("RRC Unable to send RRC_SERVICE_CNF",0,0,0);
        }
      }
      else /* curr_scan = TDSRRC_CSP_LEAVING_CONN_MODE_SCAN*/
      {
        if(tdsrrccsp_send_mm_cmd(RRC_SERVICE_IND,
                               tdsrrc_csp_int_data.curr_service_status, 
                               &tdsrrc_csp_int_data.curr_sel_plmn,
                               &tdsrrc_csp_int_data.curr_acq_entry) 
           == FAILURE)
        {
          ERR_FATAL("RRC Unable to send RRC_SERVICE_IND",0,0,0);
        }
        tdsrrccsp_send_disable_bplmn_ind(TRUE);
      }
      return TDSRRCCSP_NO_CELL_SELECTED;
    }
    else
#endif
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq by Stored Initial cell selection");
      return (tdsrrccsp_rel_chls_and_go_to_idle());
    }
  }


  if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
  {
    /* This is unacceptable since a SUCCESS was returned */
    ERR_FATAL("NO_SCAN returned",0,0,0);
  }

  /* Stop RSSI reporting timer when going to wait for lock state, so that RRC does not report invalid values */
  tdsrrctmr_stop_timer(TDSRRCTMR_RSSI_BER_TIMER);
  tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_DEFAULT_SEARCH_TIME_IN_MS,TRUE);
  next_substate = tdsrrccsp_wait_for_wrm_lock(&type_freq_scan);
  if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
  {
    return (next_substate);
  }
  /* Start RSSI reporting timer again after W got the lock. */
  tdsrrctmr_start_timer( TDSRRCTMR_RSSI_BER_TIMER,
                       TDSRRCTMR_RSSI_BER_REPORT_TIME );

  tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);

  if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
      || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
     )
  {
    /* Start waiting for Acquisition Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
  }
  else
  {
    /* Start waiting for Frequency scan Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
  }
  
  return(next_substate);
}

/*===========================================================================

FUNCTION          RRCCSP_CLEANUP_BPLMN_STATE

DESCRIPTION       This function determines if BPLMN search is active or not.
                  If BPLMN search is active then PLMN_LIST_CNF is sent to NAS
                  with status ABORTED and # of PLMNs as 0 and it is also determined 
                  if TDSL1_CPHY_GO_TO_SLEEP_IND should be sent to L1 or not.
                  If TD2G search is in progress (actively or passively) then this
                  function sends INTERRAT_PLMN_SRCH_ABORT_REQ to GSM
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_cleanup_bplmn_state()
{
  

  /* Check if guard timer has expired or not. Notice that we are additionally checking
   * for scan type here to take care of the race condition where in the CSP substate
   * guard timer expired and in the same or its pending substate another PLMN_LIST_REQ
   * was received from NAS. In this case the guard timer would have expired but the
   * previous BPLMN search was in progress. Checking for scan type allows RRC to reset 
   * the BPLMN state appropriately.
   */
  tdsrrccsp_handle_bplmn_suspend_lock_rel_offset_tmr_running();

  if((tdsrrc_csp_int_data.bplmn_guard_srch_tmr_expired)
       && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
         && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
         && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN)
         && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN)
         && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN)   
         && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_LEAVING_CONN_MODE_SCAN)         
      )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: No BPLMN cleanup required");
  }
  /* Determine if Sleep Indication should be sent or not */
  else
  {
    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
        || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: Send sleep ind to L1");
      tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_ABORT, TRUE, TRUE);
    }
    else
    {
      tdsrrc_csp_int_data.bplmn_crit_sec_flag = TRUE;

      if(TD2TD_SEARCH_RESUME == tdsrrccsp_bplmn_srch_in_progress())
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: Send sleep ind to L1");

        if(!rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active)
        {
          tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_ABORT, TRUE, TRUE);
        }
        else
        {
  		    /* If BPLMN search in TD2TD_SEARCH_RESUME state and TD2X BPLMN search is activated, 
  			   * check whether need send abort req to other RATs */
          if(!rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_aborted)
          {
            tdsrrccsp_send_irat_bplmn_abort_req();
          }
          tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_ABORT, TRUE, FALSE);
        }
      }
      else if( (TD2TD_SEARCH_SUSPEND == tdsrrccsp_bplmn_srch_in_progress()) ||
	    /* Sync WCR383539: add check for handling the cases where guard timer expired 
	     * when BPLMN is passively in progress. In this corner case, also need send CNF */
				   (tdsrrc_csp_int_data.bplmn_srch_vars_status == TD2TD_SEARCH_SUSPEND) ||
				   (tdsrrc_csp_int_data.bplmn_srch_vars_status == TD2TDSEARCH_START)	)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: Do NOT send sleep ind to L1");

        if(rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active
           && !(rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_aborted))
        {
          tdsrrccsp_send_irat_bplmn_abort_req();
        }

        tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_ABORT, TRUE, FALSE);
      }
      /* This statement will be removed */
      else if((tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN)
             && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN)
             && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN)   
             && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_LEAVING_CONN_MODE_SCAN)         
             )
      {
        ERR_FATAL("TD2TD ERR: should never come here",0,0,0);
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: No BPLMN cleanup required");
      }
      tdsrrc_csp_int_data.bplmn_crit_sec_flag = FALSE;

#ifdef FEATURE_BPLMN_SEARCH_320MS
      tdsrrc_csp_int_data.bplmn_start_sfn=0;
#endif
    }
  }
} /* tdsrrccsp_cleanup_bplmn_state */

/*===========================================================================

FUNCTION          RRCCSP_CLEANUP_AND_RESTART_BPLMN

DESCRIPTION       This function is called when UE transitions from Idle ->
                  non-idle state and BPLMN n/w sel mode is MANUAL. If BPLMN search 
                  was in progress when this function is called then the BPLMN 
                  search is re-started by resetting appropriate variables. Notice
                  that the guard timer is also reset. This function was added to
                  allow UE to continue Manual search across LAU/RAU when UE
                  returns to Idle/PCH state.                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_cleanup_and_restart_bplmn()
{


  /* Check if guard timer has expired or not. Notice that we are additionally checking
   * for scan type here to take care of the race condition where in the CSP substate
   * guard timer expired and in the same or its pending substate another PLMN_LIST_REQ
   * was received from NAS. In this case the guard timer would have expired but the
   * previous BPLMN search was in progress. Checking for scan type allows RRC to reset 
   * the BPLMN state appropriately.
   */
  if((tdsrrc_csp_int_data.bplmn_guard_srch_tmr_expired)
       && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
         && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: No BPLMN cleanup required");
  }
  /* Determine if Sleep Indication should be sent or not */
  else
  {
    tdsrrc_csp_int_data.bplmn_crit_sec_flag = TRUE;

    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
        || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: Send sleep ind to L1");
      tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_SUSPEND, FALSE, TRUE);
    }
    else
    {
      if(TD2TD_SEARCH_RESUME == tdsrrccsp_bplmn_srch_in_progress())
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: Send sleep ind to L1");

        if(!rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active)
        {
          tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_SUSPEND, FALSE, TRUE);
        }
        else
        {
          tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_SUSPEND, FALSE, FALSE);
        }
      }
      else if(TD2TD_SEARCH_SUSPEND == tdsrrccsp_bplmn_srch_in_progress())
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: Do NOT send sleep ind to L1");

        if(rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active)
        {
          if(!(rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_aborted))
          {
            tdsrrccsp_send_irat_bplmn_abort_req();
          }

          /* Even when TD2G BPLMN search is actively in progress, the bplmn_srch_in_progress()
           * function returns status as suspended because the TDS DRX timer is NOT active for 
           * TD2G search. So even in this case the search needs to be suspended and then
           * re-started */
          tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_SUSPEND, FALSE, FALSE);
        }
      }
      /* This statement will be removed */
      else
      {
        ERR_FATAL("TD2TD ERR: should never come here",0,0,0);
      }
    }

    tdsrrccsp_reset_bplmn_list_search_state(TD2TDSEARCH_START, FALSE, FALSE);
    tdsrrccsp_generate_bplmn_rat_srch_order_update_curr_srch_rat(tdsrrc_csp_int_data.bplmn_nw_sel_mode);

    tdsrrc_csp_int_data.bplmn_crit_sec_flag = FALSE;

    if(tdsrrc_csp_int_data.bplmn_nw_sel_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
    {
      if(tdsrrc_csp_int_data.bplmn_tdscdma_mode_enabled)
      {
      tdsrrccsp_update_td2td_manual_bplmn_list_with_rplmn(tdsrrc_csp_int_data.curr_acq_entry.plmn_id,
                                                      tdsrrc_csp_int_data.curr_acq_entry.cell_id);

    }
  }
  }

  rrc_csp_td2g_bplmn_int_data.restart_bplmn_search = FALSE;
} /* tdsrrccsp_cleanup_and_restart_bplmn */


/*===========================================================================

FUNCTION          tdsrrccsp_release_chls_and_start_sel_on_mm_req

DESCRIPTION       This function releases all physical, transport and logical
                  channels before starting a cell selection in response to
                  a newly received service request from MM.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_release_chls_and_start_sel_on_mm_req(void)
{
  tdsrrc_csp_substate_e_type   next_substate;

  tdsrrccsp_cleanup_bplmn_state();

  /* Abort PG1 whenever we trigger a new camping procedure */
  tdsrrcpg1_check_and_abort_bcch_mod_info_processing();

  /* First call a CCM function to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();

  /* Release all channels and put lower layers to Idle before sending another 
   * Acquisition Req or a Frequency Scan Req.
   */
  if(tdsrrccsp_send_chan_release() == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  /* Inter Frequency Redirection Scan was aborted. Clean up its state */
  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN)
  {
    tdsrrccsp_cleanup_inter_freq_redirection_state();
  }               

  next_substate = tdsrrccsp_start_sel_on_mm_req();

  return(next_substate);
}

/*===========================================================================

FUNCTION          tdsrrccsp_start_bplmn_srch

DESCRIPTION       This function starts a BPLMN search in response to
                  a BPLMN search request from RR (for G2TD searches) or 
                  NAS (for TD2TD searches). 
                  
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_bplmn_srch(void)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan;

  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;
  
  uint32 lock_wait_timer = 0;

  /* If new BPLMN search then initialise data*/
  if(tdsrrc_csp_int_data.bplmn_new_srch)
  {   
    /* Reset the next_band_mask appropriately */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the next_band_mask");
    tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_int_data.tds_band_mask;
    /* Record next_band_mask for deep search*/
    tdsrrc_csp_curr_select_data.next_band_mask_for_deep = tdsrrc_csp_curr_select_data.next_band_mask;

    tdsrrc_csp_curr_select_data.num_available_freq = 0;
  }

  if(tdsrrccsp_start_cell_selection(&tdsrrc_csp_int_data.curr_req_plmn, 
                                  &tdsrrc_csp_int_data.curr_type_acq, 
                                  &tdsrrc_csp_int_data.curr_acq_entry,
                                  &type_freq_scan) 
     == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq by Stored Initial cell selection");

    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
       || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
    {

      /* Start the full freq scan timer during which no full scans are done
       * for automatic/manual PLMN searches
       */
      tdsrrccsp_check_and_start_plmn_list_full_freq_scan_timer();

      if((tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx+1 ) >= tdsrrc_csp_int_data.plmn_srch_order.num_rats) 
      {
        tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_STOP, TRUE, TRUE);
      }
      else
      { 
        tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_SUSPEND, FALSE, TRUE);
        tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx++;
        tdsrrccsp_init_td2g_plmn_srch_int_data_across_rats();
        rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active = TRUE;
#ifdef FEATURE_TDSCDMA_TO_LTE
        if(tdsrrc_csp_int_data.plmn_srch_order.rat[tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx] == SYS_RAT_GSM_RADIO_ACCESS)
        {
          tdsrrc_csp_int_data.bplmn_interrat_type = TDSRRC_CSP_INTERRAT_GSM;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2G : Do BPLMN Srch on GSM");
        }
        else if (tdsrrc_csp_int_data.plmn_srch_order.rat[tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx] == SYS_RAT_LTE_RADIO_ACCESS)
        {
          tdsrrc_csp_int_data.bplmn_interrat_type = TDSRRC_CSP_INTERRAT_LTE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2L: Do BPLMN Srch  on LTE");
        }
#endif
      }

      next_substate = tdsrrccsp_set_to_suitable_or_acceptable_substate();

    }
    else
    {
      return (tdsrrccsp_rel_chls_and_go_to_idle());
    }

  }
  else  /* Another freq found */
  {
    if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
    {
        /* This is unacceptable since a SUCCESS was returned */
        ERR_FATAL("NO_SCAN returned",0,0,0);
    }

  if(!tdrm_check_cli_state_is_in_lock(TDRM_CLIENT_RRC))
  {
    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
       || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
    {
      /*use remaining_time TD2TD DRX timer - 2msec, so that we no need to come out of lock for internal cmds*/
      lock_wait_timer = tdsrrctmr_get_remaining_time(RRCTMR_TD2TD_BPLMN_DRX_SRCH_TIMER) - TD2TD_BPLMN_SEARCH_WAIT_LOCK_NECESSAEY_AHEAD_TIMER_IN_MS;
    }
    else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN)
    {
      lock_wait_timer = TDSRRCTMR_DEFAULT_SEARCH_TIME_IN_MS;
    }
    else
    {
      /*use remaining_time G2TD search timer - 2msec, so that we no need to come out of lock for internal cmds*/
      lock_wait_timer = tdsrrctmr_get_remaining_time(TDSRRCTMR_BPLMN_SRCH_TIMER) - X2TD_BPLMN_SEARCH_WAIT_LOCK_NECESSAEY_AHEAD_TIMER_IN_MS;
    }
    tdsrrcwrm_start_lock_wait_timer(lock_wait_timer,TRUE);
    next_substate = tdsrrccsp_wait_for_wrm_lock(&type_freq_scan);
    if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
    {
      return (next_substate);
    }
  }

    tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);

    if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
      || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
     )
    {
        /* Start waiting for Acquisition Confirmation */
        next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
    }
    else
    {
        /* Start waiting for Frequency scan Confirmation */
        next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
    }
  }  /* Another freq found */

  return(next_substate);
} /* tdsrrccsp_start_bplmn_srch */


/*===========================================================================

FUNCTION          tdsrrccsp_resume_td2g_bplmn_srch

DESCRIPTION       This function starts the TD2G BPLMN search by suspending TDS
                  and sending INTERRAT_PLMN_SRCH_REQ to GSM
                                               
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_resume_td2g_bplmn_srch(void)
{
  tdsrrc_csp_substate_e_type next_substate;

  next_substate = tdsrrccsp_set_to_suitable_or_acceptable_substate();

  /* 
   * Don't acquire lock when suspend TDS for T2G BPLMN search, for two reasons:
   *  1. No RF resource to access for the suspend proc, and the lock will be released when get CNF from L1.
   *  2. Lock may not be able to acquire for RRC if L1 is holding the secondary lock, as L1 need to sleep
   *     to release secondary lock, but RRC don't allow L1 go to sleep as it's going to do T2G BPLMN search. 
   */


  if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2G_BPLMN_AUTOMATIC_SCAN)
     || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2G_BPLMN_MANUAL_SCAN))
  {
    /* Check if enough DRX search time remains to do BPLMN search */
    if(rrc_csp_td2g_bplmn_int_data.wtog_rem_drx_tmr_value < 
                          MIN_TD2TD_BPLMN_SRCH_TIMER_IN_MS - TD2G_BPLMN_SEARCH_OVERHEAD_TIMER_IN_MS)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"TD2G: remaining DRX tmr %d < min %d. Wait for Next DRX. Reset BPLMN DB",rrc_csp_td2g_bplmn_int_data.wtog_rem_drx_tmr_value,MIN_TD2TD_BPLMN_SRCH_TIMER_IN_MS - TD2G_BPLMN_SEARCH_OVERHEAD_TIMER_IN_MS);
      tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_SUSPEND, FALSE, FALSE);
      tdsrrcwrm_release_lock();
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"TD2X: remaining DRX tmr %d >= min %d. Do IRAT Search. Suspend TDS",rrc_csp_td2g_bplmn_int_data.wtog_rem_drx_tmr_value,MIN_TD2TD_BPLMN_SRCH_TIMER_IN_MS - TD2G_BPLMN_SEARCH_OVERHEAD_TIMER_IN_MS);

      /* Send change mode request to MCM to standby for suspending TDS stack */
      tdsrrccsp_send_mode_change_req_to_mcm(TDSRRC_MODE_STANDBY);
      next_substate = TDSRRCCSP_WT_SUSPEND_MODE_CHANGE_CNF;
    }

    /* Stop the DRX Search Timer as this timer will now be started by GSM */
    tdsrrctmr_stop_timer(RRCTMR_TD2TD_BPLMN_DRX_SRCH_TIMER);
    tdsrrc_csp_int_data.bplmn_srch_tmr_expired = TRUE;
  }
  /* Do GSM PLMN Search in foreground as UE is OOS */
  else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2G OOS: Do GSM srch in fgnd. Send Suspend Req to MCM");

    /* Send change mode request to MCM to standby for suspending TDS stack */
    tdsrrccsp_send_mode_change_req_to_mcm(TDSRRC_MODE_STANDBY);
    next_substate = TDSRRCCSP_WT_SUSPEND_MODE_CHANGE_CNF;
  }
  /* Incorrect Scan Type. Print Error Message */
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Incorrect scan type %d in resume_wtog_bplmn_srch fn call",tdsrrc_csp_int_data.curr_scan);
  } 

  return(next_substate);
} /* tdsrrccsp_resume_td2g_bplmn_srch */




/*===========================================================================

FUNCTION          RRC_CSP_CONTINUE_OOS_IN_CONNECTED_MODE_SEARCH

DESCRIPTION       This function is called when curr scan is OOS in Connected Mode
                  scan and CSP has completed ACQ DB and Full Frequency Scan.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
static tdsrrc_csp_substate_e_type tdsrrccsp_continue_oos_in_connected_mode_search(
                              tdsrrc_csp_freq_scan_e_type  *type_freq_scan_ptr)
{
  tdsrrc_csp_substate_e_type next_substate = TDSRRCCSP_NO_CELL_SELECTED;

  /*  --------------------
   *  Dual Mode is Enabled
   *  -------------------- 
   */
  if(tdsrrcmcm_is_dualmode_enabled())
  { 
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Dual Mode is enabled");

    /* If at least 1 PLMN is found after 30s and at least 1 search 
     * has been completed on GSM, then return No Service to MM
     */
    if( (tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd)
          && (tdsrrc_csp_int_data.oos_gsm_srch_done)
            && (tdsrrc_csp_int_data.plmn_list.num_plmns > 0) )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"At least 1 TDS-PLMN found. #TDS-PLMNs %d. Return No Svc to NAS",tdsrrc_csp_int_data.plmn_list.num_plmns);
      tdsrrcccm_update_with_cell_loss();
      return (tdsrrccsp_rel_chls_and_go_to_idle());
    }
    else
    {

      /* Start OOS Service Search on GSM */     
      /* Reset TDS plmns found to 0 */
      tdsrrc_csp_int_data.plmn_list.num_plmns = 0;

      /* No need to send IDLE_REQ for TDD.
       * Assume TDD L1 already in idle or sync state.
       * ZZTBD: or LLC has sent release all to L1 before.
       */

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Chg Mode Req for StandBy to MCM for OOS Svc Srch on G");
      /* Send change mode request to MCM */
      tdsrrccsp_send_mode_change_req_to_mcm(TDSRRC_MODE_STANDBY);
      return(next_substate = TDSRRCCSP_WT_SUSPEND_MODE_CHANGE_CNF);


    }
  } /* Dual Mode is enabled */


  /*  ------------------------
   *  Dual Mode is NOT Enabled
   *  ------------------------
   */
  else
  {
    /* Dual Mode is not enabled. Continue Searching on TDS */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Dual Mode not enabled");

    /* If at least 1 PLMN is found after 30s then return No Service to MM */
    if((tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd)
           && (tdsrrc_csp_int_data.plmn_list.num_plmns > 0))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"At least 1 TDS-PLMN found. #TDS-PLMNs %d. Return No Svc to NAS",tdsrrc_csp_int_data.plmn_list.num_plmns);
      tdsrrcccm_update_with_cell_loss();
      return (tdsrrccsp_rel_chls_and_go_to_idle());
    }

    /* Determine if UE should be put to deep sleep or not. UE is NOT put into deep sleep
     * if GCF flag is enabled.
     */
    else if(   (tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd) 
            && (tdsrrc_csp_int_data.oos_initial_no_deep_sleep_tmr_expd)
            && (FALSE == mcfg_gcf_nv_get_status())
           )
    {
      /* Start sleep timer of 30s */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"OOS: Put UE in Deep Sleep. Start sleep tmr");

      /* Increase the sleep time as UE spends more time in OOS */
      tdsrrctmr_start_timer(TDSRRCTMR_CONN_MODE_OOS_DS_TIMER, 
                         TDSRRCTMR_CONN_MODE_OOS_DS_TIMER_IN_MS);
      tdsrrc_csp_int_data.oos_conn_mode_ds_tmr_expd = FALSE;

      tdsrrccsp_send_conn_mode_oos_ds_req();

      tdsrrcwrm_release_lock();

#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
      if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
      {
        /* Update UI with No Service if No PLMN was found */
        if(!tdsrrc_csp_int_data.oosc_plmn_found)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SRV_ST: Update UI w/ No SERVICE b4 putting UE to sleep");
          tdsrrccsp_send_svc_status_to_mm(SYS_SRV_STATUS_NO_SRV,SYS_RAT_TDS_RADIO_ACCESS, 0);
        }
        /* Reset the oosc_plmn_found var for each search cycle */
        tdsrrc_csp_int_data.oosc_plmn_found=FALSE;
      }
#endif

      next_substate = tdsrrccsp_set_to_suitable_or_acceptable_substate();

      return(next_substate);
    }
    else
    { 
      /* Reset TDS plmns found to 0 */
      tdsrrc_csp_int_data.plmn_list.num_plmns = 0;
      /* Update UI with No Service if No PLMN was found */
#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
      if(!tdsrrc_csp_int_data.oosc_plmn_found)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SRV_ST: Update UI w/ No SERVICE");
        tdsrrccsp_send_svc_status_to_mm(SYS_SRV_STATUS_NO_SRV,SYS_RAT_TDS_RADIO_ACCESS, 0);
      }
      /* Reset the oosc_plmn_found var for each search cycle */
      tdsrrc_csp_int_data.oosc_plmn_found=FALSE;
#endif

      /* Reset the next_band_mask appropriately */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the next_band_mask");
      tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_int_data.tds_band_mask;
      /* Record next_band_mask for deep search*/
      tdsrrc_csp_curr_select_data.next_band_mask_for_deep = tdsrrc_csp_curr_select_data.next_band_mask;

      /* Start again with Stored and Initial cell selection. Initiale Selection
       * data so that Selection Controller starts from the begining.
       */
      if(tdsrrccsp_start_cell_selection(&tdsrrc_csp_int_data.curr_req_plmn, 
                                      &tdsrrc_csp_int_data.curr_type_acq, 
                                      &tdsrrc_csp_int_data.curr_acq_entry,
                                      type_freq_scan_ptr) 
        == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR No freq by Stored Initial cell selection");
        tdsrrcccm_update_with_cell_loss();
        return (tdsrrccsp_rel_chls_and_go_to_idle());
      }
      /* Return added to suppress compilation warning.*/
      return next_substate;
    }


  } /* Dual Mode is NOT enabled */

} /* tdsrrccsp_continue_oos_in_connected_mode_search */



/*===========================================================================

FUNCTION          tdsrrccsp_continue_cell_selection

DESCRIPTION       This function continues with the existing cell selection 
                  with the existing acquisition type. It continues from the
                  point in the frequency scan or in acquisition database where
                  we were last.
                  If all frequencies are exhausted, a cell selection is 
                  re-started in an acquisition type based on the requested 
                  PLMN type.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_continue_cell_selection(void)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan = TDSRRC_CSP_NO_SCAN;

  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;
  uint8 trans_id;    

  boolean continue_cell_selection = TRUE;

  boolean oos_enhancement_in_state_tx = FALSE;

  boolean target_tdd_cell_list_updated = FALSE;

  if ((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)
       && (tdsrrc_csp_int_data.tds_special_freq_enabled && tdsrrc_csp_int_data.use_special_freq_for_acq)
     )
   {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set use_special_freq_for_acq to FALSE");
      tdsrrc_csp_int_data.use_special_freq_for_acq = FALSE;
      return tdsrrccsp_restart_cell_selection(next_substate);
   }

  if(  (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN )
    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN  )
    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN   )  
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN )
    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN  )
    || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN   )  
#endif
    )
  {
    oos_enhancement_in_state_tx = TRUE;
  }

  /* Reset the flag here to determine if 
   * the next ACQ should be continuing LFS or start a new LFS
   */
  tdsrrc_csp_int_data.tds_continue_lfs = FALSE;


  if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN) 
    &&(tdsrrc_csp_curr_select_data.scan_info.type_freq_scan == TDSRRC_CSP_LIST_SCAN)
    &&((!tdsrrc_csp_int_data.g2t_redi_freq_compl 
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
        && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_GSM)
#endif
       )
#ifdef FEATURE_LTE_TO_TDSCDMA 
       || (!tdsrrc_csp_int_data.l2t_redi_freq_compl && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE))
#endif
      )
    )
  {
    /*Get the next entry*/
    /*>if valid send the ACQ req to L1*/
    /*>Else*/
    /*>>If the timer didnt expire, continue the List scan*/
    /*>>If timer expired, behave as if search exhausted, initiate mode change request to STOP, wt for mode change cnf*/
    if (tdsrrccsp_get_list_db_match_acq_entry() == TRUE)
       /* List DB which in X2T redirect case contains redirected frequencies. When it is not exhausted */
    {
      /*Send acquisition request to L1 */
      /* type_freq_scan = TDSRRC_CSP_ACQ_SCAN; */
      type_freq_scan = TDSRRC_CSP_LIST_SCAN;
      /* Continue previous LFS */
      tdsrrc_csp_int_data.tds_continue_lfs = TRUE;
      continue_cell_selection = FALSE;
    }
    else
    {
#ifdef FEATURE_L2TD_REDIRECT_OPT_IRAT_ACQ_DB
      if (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE)
      {
        tdsrrccsp_update_target_tdd_cell_list_freqs();
	    if (tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies > 0)
	    {
		  target_tdd_cell_list_updated = TRUE;
	    }
      }
#else
#ifdef FEATURE_L2TD_REDIRECT_R10
      if ((!tdsrrc_csp_int_data.l2t_r9_redirect) && (tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn_count > 0))
      {
        tdsrrccsp_update_target_tdd_cell_list_for_ngbr_tdd_freq();
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: R10 redirected freq list done,%d sib6 freq copied to target list",tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies);
        if (tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies > 0)
        {
          target_tdd_cell_list_updated = TRUE;
        }
      }
#endif
#endif
      /* load irat acq db/R10 sib6 ngbr freq similar to gtow_redir_retry_lfs_scan for g2t redirect case, as below */
      if (target_tdd_cell_list_updated)
      {
        /*copy the target list to tdslist_db*/
        (void)tdsrrccsp_lfs_update_list_db_with_target_tdd_list();
        tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq = 0;
        tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_LIST_SCAN;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: set curr frequency scan type =%d",tdsrrc_csp_curr_select_data.scan_info.type_freq_scan);
        type_freq_scan = TDSRRC_CSP_LIST_SCAN;
#ifdef FEATURE_L2TD_REDIRECT_OPT_IRAT_ACQ_DB 
        if (tdsrrc_csp_int_data.l2t_redirect_irat_acqdb_selected)
        {
          /* For L2T redirect with freqs from irat acq db, use SHALLOW to keep the oreder of ACQ */
          tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_SHALLOW;
          tdsrrc_csp_int_data.l2t_redirect_irat_acqdb_selected = FALSE;
        }
        else
        {
          /* For L2T redirect with freqs not from irat acq db, keep/recover back to LFS */
          tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_LFS;
        }
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: acq_mode change to %d",tdsrrc_csp_int_data.acq_mode);
#endif
        continue_cell_selection = FALSE;       
      }
      else if(!tdsrrc_csp_int_data.gtow_redirection_timer_expired)
      {
	    if((tdsrrc_csp_int_data.gtow_redir_retry_lfs_scan)
#ifdef FEATURE_LTE_TO_TDSCDMA
           && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_GSM)
#endif
          )
        {
           /*copy the target list to tdslist_db*/
           (void)tdsrrccsp_lfs_update_list_db_with_target_tdd_list();
           /* Haizhou 2012/9/10 reset scan_info.list_scan.num_freq so that we will re-compose
		    * scan_info.list_scan and acq_freq_list in tdsrrccsp_send_next_freq_to_l1(), or else, 
			* we may continue to do LFS list scan and send 0 freq to L1
			*/
		   tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq = 0;
           tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_LIST_SCAN;
           TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: set curr frequency scan type =%d",tdsrrc_csp_curr_select_data.scan_info.type_freq_scan);
           type_freq_scan = TDSRRC_CSP_LIST_SCAN;
           continue_cell_selection = FALSE;
		   tdsrrc_csp_int_data.gtow_redir_retry_lfs_scan = FALSE;
        }
        else
        {
           tdsrrctmr_start_timer(RRCTMR_G2TD_REDIRECTION_LFS_TIMER,RRCTMR_G2TD_REDIRECTION_LFS_TIMER_IN_MS);
           return TDSRRCCSP_CELL_SEL_WT_ACQ;
        }
      }
      else
      {
	    tdsrrctmr_stop_timer(RRCTMR_G2TD_REDIRECTION_LFS_TIMER);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Interrat Redirection: Timer expired,Start ACQ DB Scan + FFS.");
        tdsrrc_csp_int_data.do_lfs_scan= FALSE;
        tdsrrc_csp_curr_select_data.acq_db_in_use = TRUE;
        tdsrrc_csp_curr_select_data.acq_db_scan_info.start_with_first = TRUE;
        tdsrrccsp_init_freq_scan_data();
		tdsrrc_csp_int_data.g2t_redi_freq_compl = TRUE;
        tdsrrc_csp_int_data.l2t_redi_freq_compl = TRUE;
        //tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_ACQ_SCAN;
        /* redirected freqs (include lte sib6 freqs) all tried, continue from ACQ DB */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Redirected freqs (and lte sib6 freqs) tried out,restart cell sel from ACQ DB.");
        next_substate=tdsrrccsp_restart_cell_selection(next_substate);
        return next_substate;
      }
    }
  }
  else
  if(tdsrrc_csp_curr_select_data.scan_info.type_freq_scan == TDSRRC_CSP_LIST_SCAN ) 
  {
   if (tdsrrc_csp_curr_select_data.scan_info.raw_scan_completed &&
       tdsrrc_csp_curr_select_data.scan_info.fine_scan_completed)
   {
     /* TDD: LFS is the result of FINE scan and not from ACQ DB 
      * continue to FINE scan
      */
     type_freq_scan = TDSRRC_CSP_LIST_SCAN;
     continue_cell_selection = TRUE;  
   }
   else
   {
    if(tdsrrccsp_get_list_db_match_acq_entry() == TRUE)
    {
      /*Send acquisition request to L1 */
      /* Was ACQ_SCAN even for TDD. Not sure why. */
      type_freq_scan = TDSRRC_CSP_LIST_SCAN;
      /* Continue previous LFS */
      tdsrrc_csp_int_data.tds_continue_lfs = TRUE;
      continue_cell_selection = FALSE;
    }
#ifdef FEATURE_TDSCDMA_SCAN_SCOPE
    /* Once ACQ DB search is completed report immediate no service */
    else if((tdsrrc_csp_int_data.srvc_req_scan_scope == SYS_SCAN_SCOPE_ACQ_DB) &&
            (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN))
    {
      tdsrrccsp_handle_no_available_freq();
      return TDSRRCCSP_NO_CELL_SELECTED;
    }
#endif
   }
  }
  /* On TD2G OOS redirection search failure and UE continuing search on TDS,LFS needs to be performed. In this case
      freq scan will not be LFS and num_freq>0. num_freq is initialized in tdsrrccsp_init_select_data().
      Also follow LFS scan if cell selection fails on NV DL freq.*/
  else if((
	       (tdsrrc_csp_curr_select_data.scan_info.type_freq_scan == TDSRRC_CSP_LIST_SCAN) &&
	       tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq > 0) &&
            ((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
               || ((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN) && tdsrrc_csp_int_data.tds_special_freq_enabled)))
  {
    tdsrrc_csp_curr_select_data.scan_info.type_freq_scan = TDSRRC_CSP_LIST_SCAN;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: set curr frequency scan type =%d",tdsrrc_csp_curr_select_data.scan_info.type_freq_scan);                 
    type_freq_scan = TDSRRC_CSP_LIST_SCAN;
    continue_cell_selection = FALSE;
  }

  if((continue_cell_selection) && (tdsrrccsp_continue_stored_initial_select_ctrl
      (
        &tdsrrc_csp_int_data.curr_type_acq, 
        &tdsrrc_csp_int_data.curr_acq_entry,
        &type_freq_scan
      )   == FAILURE
    ))
  {
    if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN) 
    {
      /* Reset the next_band_mask appropriately */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the next_band_mask");
      tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_int_data.tds_band_mask;
      /* Record next_band_mask for deep search*/
      tdsrrc_csp_curr_select_data.next_band_mask_for_deep = tdsrrc_csp_curr_select_data.next_band_mask;


      /* Start again with Stored and Initial cell selection. Initiale Selection
       * data so that Selection Controller starts from the begining.
       */
      if(tdsrrccsp_start_cell_selection(&tdsrrc_csp_int_data.curr_req_plmn, 
                                      &tdsrrc_csp_int_data.curr_type_acq, 
                                      &tdsrrc_csp_int_data.curr_acq_entry,
                                      &type_freq_scan) 
        == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq by Stored Initial cell selection");

        /* First call a CCM function to indicate Cell Loss - no new cell */
        /* NOTE: The functional I/F has to be used very carefully. */
        tdsrrcccm_update_with_cell_loss();
        return (tdsrrccsp_rel_chls_and_go_to_idle());
      }
    }
    /* OOS handling in Connected Mode */
    else if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
    {
      next_substate = tdsrrccsp_continue_oos_in_connected_mode_search(&type_freq_scan);

      if(   (next_substate == TDSRRCCSP_CELL_SEL_WT_STATE_CHANGE)
         || (next_substate == TDSRRCCSP_WT_SUSPEND_MODE_CHANGE_CNF)
         || (next_substate == TDSRRCCSP_CAMPED_SUIT_CELL)
         || (next_substate == TDSRRCCSP_CAMPED_ACCEPT_CELL))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"OOS: next_substate %d",next_substate);
        return next_substate;
      } 
    }

    /* OOS handling in Connected Mode */
    else if(oos_enhancement_in_state_tx)
    {
      /* Dual Mode is enabled */
      if(tdsrrcmcm_is_dualmode_enabled())
      { 
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Dual Mode is enabled. Srch on GSM. curr_scan %d",tdsrrc_csp_int_data.curr_scan);

        /* Start OOS Service Search on GSM */     

        /* Reset TDS plmns found to 0 */
        tdsrrc_csp_int_data.plmn_list.num_plmns = 0;

      /* No need to send IDLE_REQ for TDD.
       * Assume TDD L1 already in idle or sync state.
       * ZZTBD: or LLC has sent release all to L1 before.
       */
      
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Chg Mode Req for StandBy to MCM for OOS Svc Srch on G");
        /* Send change mode request to MCM */
        tdsrrccsp_send_mode_change_req_to_mcm(TDSRRC_MODE_STANDBY);
        return(next_substate = TDSRRCCSP_WT_SUSPEND_MODE_CHANGE_CNF);    
      }
      /* Dual Mode is not enabled */
      else
      {
        /* Dual Mode is not enabled. Continue Searching on TDS */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Dual Mode not enabled. Restart srch on TDS. curr_scan %d",tdsrrc_csp_int_data.curr_scan);

        /* Reset TDS plmns found to 0 */
        tdsrrc_csp_int_data.plmn_list.num_plmns = 0;

        /* Reset the next_band_mask appropriately */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the next_band_mask");
        tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_int_data.tds_band_mask;
        /* Record next_band_mask for deep search*/
        tdsrrc_csp_curr_select_data.next_band_mask_for_deep = tdsrrc_csp_curr_select_data.next_band_mask;


        /* Start again with Stored and Initial cell selection. Initiate Selection
         * data so that Selection Controller starts from the begining.
         */
        if(tdsrrccsp_start_cell_selection(&tdsrrc_csp_int_data.curr_req_plmn, 
                                        &tdsrrc_csp_int_data.curr_type_acq, 
                                        &tdsrrc_csp_int_data.curr_acq_entry,
                                        &type_freq_scan) 
          == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR No freq by Stored Initial cell selection");
          tdsrrcccm_update_with_cell_loss();
          return (tdsrrccsp_rel_chls_and_go_to_idle());
        }
      }
    }


    /* If WtoW BLMN search is in progress then send plmn list confirmation
     * to NAS and sleep indication to L1
     */
    else if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
            || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: no more freq fnd. TDS Search completed");

      /* Start the full freq scan timer during which no full scans are done
       * for automatic/manual PLMN searches
       */
      tdsrrccsp_check_and_start_plmn_list_full_freq_scan_timer();

      if((tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx+1 ) >= tdsrrc_csp_int_data.plmn_srch_order.num_rats) 
      {
        tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_STOP, TRUE, TRUE);
      }
      else
      { 
        tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_SUSPEND, FALSE, TRUE);
        tdsrrccsp_init_td2g_plmn_srch_int_data_across_rats();
        rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active = TRUE;

        tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx++;
#ifdef FEATURE_TDSCDMA_TO_LTE
        if(tdsrrc_csp_int_data.plmn_srch_order.rat[tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx] == SYS_RAT_GSM_RADIO_ACCESS)
        {
          tdsrrc_csp_int_data.bplmn_interrat_type = TDSRRC_CSP_INTERRAT_GSM;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2G : Do BPLMN Srch on GSM");
        }
        else if (tdsrrc_csp_int_data.plmn_srch_order.rat[tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx] == SYS_RAT_LTE_RADIO_ACCESS)
        {
          tdsrrc_csp_int_data.bplmn_interrat_type = TDSRRC_CSP_INTERRAT_LTE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2L: Do BPLMN Srch on LTE");
        }
#endif
      }

      next_substate = tdsrrccsp_set_to_suitable_or_acceptable_substate();
 
      return (next_substate);
    }
    /* Do BPLMN Search on GSM in foreground */
    else if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN) 
            && 
            ((tdsrrc_csp_int_data.bplmn_dual_mode_enabled)
#ifdef FEATURE_TDSCDMA_TO_LTE
             || (tdsrrc_csp_int_data.bplmn_lte_mode_enabled)
#endif
            ))
            
    {
      /* Start the full freq scan timer during which no full scans are done
       * for automatic/manual PLMN searches
       */
      tdsrrccsp_check_and_start_plmn_list_full_freq_scan_timer();

      if((tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx+1 ) >= tdsrrc_csp_int_data.plmn_srch_order.num_rats) 
      {
        /* First call a CCM function to indicate Cell Loss - no new cell */
        /* NOTE: The functional I/F has to be used very carefully. */
        tdsrrcccm_update_with_cell_loss();
        return (tdsrrccsp_rel_chls_and_go_to_idle());
      }
      /* Search not yet done on GSM. Do GSM Manual PLMN search */
      else
      {
        /* No need to send IDLE_REQ for TDD.
         * Assume TDD L1 already in idle or sync state
         */
        tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx++;
        tdsrrccsp_init_td2g_plmn_srch_int_data_across_rats();
#ifdef FEATURE_TDSCDMA_TO_LTE
        if(tdsrrc_csp_int_data.plmn_srch_order.rat[tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx] == SYS_RAT_GSM_RADIO_ACCESS)
        {
          tdsrrc_csp_int_data.bplmn_interrat_type = TDSRRC_CSP_INTERRAT_GSM;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2G OOS: Do BPLMN Srch in fgnd on GSM");
        }
        else if (tdsrrc_csp_int_data.plmn_srch_order.rat[tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx] == SYS_RAT_LTE_RADIO_ACCESS)
        {
          tdsrrc_csp_int_data.bplmn_interrat_type = TDSRRC_CSP_INTERRAT_LTE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2L OOS: Do BPLMN Srch in fgnd on LTE");
        }
#endif
        return(tdsrrccsp_resume_td2g_bplmn_srch());
      }
    }

    else if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN) 
            && (TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
            && (tdsrrc_csp_int_data.bplmn_nw_sel_mode == SYS_NETWORK_SELECTION_MODE_MANUAL))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: LeavingConnMode no more freqs. Do Pending Manual Search in fgnd");
      next_substate = tdsrrccsp_continue_bplmn_in_frgrnd();

      return next_substate;
    } 

    else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
    {
      tdsrrc_csp_int_data.rsp_to_rr = SEND_G2TD_REDIRECTION_REJECT;
      tdsrrccsp_send_mode_change_req_to_mcm(TDSRRC_MODE_INACTIVE);
      return (TDSRRCCSP_WT_STOP_MODE_CHANGE_CNF);
    }
#ifdef FEATURE_WRLF_SYSTEM_SEL
    else if(((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN) ||
                (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN))
              && TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
    {
      tdsrrc_csp_int_data.curr_service_status     = MM_AS_NO_SERVICE;
      tdsrrc_csp_int_data.curr_sel_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;

      if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)
      {
        if(tdsrrccsp_send_mm_cmd(RRC_SERVICE_CNF,
                               tdsrrc_csp_int_data.curr_service_status, 
                               &tdsrrc_csp_int_data.curr_sel_plmn,
                               &tdsrrc_csp_int_data.curr_acq_entry) 
           == FAILURE)
        {
          ERR_FATAL("RRC Unable to send RRC_SERVICE_CNF",0,0,0);
        }
      }
      else /* curr_scan = TDSRRC_CSP_LEAVING_CONN_MODE_SCAN*/
      {
        if(tdsrrccsp_send_mm_cmd(RRC_SERVICE_IND,
                               tdsrrc_csp_int_data.curr_service_status, 
                               &tdsrrc_csp_int_data.curr_sel_plmn,
                               &tdsrrc_csp_int_data.curr_acq_entry) 
           == FAILURE)
        {
          ERR_FATAL("RRC Unable to send RRC_SERVICE_IND",0,0,0);
        }
        /* Send disable BPLMN IND to NAS to disable periodic BPLMN searches.*/
        tdsrrccsp_send_disable_bplmn_ind(TRUE);
      }
      return TDSRRCCSP_NO_CELL_SELECTED;
    }
#endif
    else /* For all other scans... */
    {
      /* First call a CCM function to indicate Cell Loss - no new cell */
      /* NOTE: The functional I/F has to be used very carefully. */
      tdsrrcccm_update_with_cell_loss();
      return (tdsrrccsp_rel_chls_and_go_to_idle());
    }
  }  /* No frequency found */

  /* Another freq found */
  if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
  {
    /* This is unacceptable since a SUCCESS was returned */
    ERR_FATAL("NO_SCAN returned",0,0,0);
  }
 
  if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN) ||
     (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN) ||
     (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) ||
     (!tdsdsim_bplmn_search_complete_w_cycle && 
      ((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)||
       (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)))||
     (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)||
     (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
#ifdef FEATURE_WRLF_SYSTEM_SEL
     ||((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN) && 
                (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()))
#endif
    )
  {
    if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN) && 
       (tdsrrccsp_start_lock_wait_timer_for_forground_plmn == TRUE))
    {
	  /*Use 1/2 of TDSRRCTMR_DEFAULT_SEARCH_TIME_IN_MS as some time is used in GSM\LTE*/
      tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_DEFAULT_SEARCH_TIME_IN_MS/2,TRUE);
    }
    next_substate = tdsrrccsp_wait_for_wrm_lock(&type_freq_scan);
    if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
    {
      return (next_substate);
    }
  }

#ifdef FEATURE_WRLF_SYSTEM_SEL
    /* CR412213 Skip FS on first no service ind due to RLF */
    if ((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN)
        && (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()
          ||(tdsrrc_csp_int_data.ue_in_connecting_oos == TRUE))
        && (type_freq_scan == TDSRRC_CSP_RAW_SCAN))
    {
	  if (TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
	  {
	     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Take BPLMN as Foreground PLMN when connected/connecting OOS");
         tdsrrc_csp_int_data.plmn_list.num_plmns = 0;
         trans_id = tdsrrc_csp_int_data.bplmn_trans_id;
         tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_STOP, FALSE, FALSE);
         tdsrrc_csp_int_data.bplmn_trans_id= trans_id;

         next_substate = tdsrrccsp_start_sel_for_plmn_list();
         return next_substate;
	  }
	  else
	  {
	     TDSRRC_MSG1(MSG_LEGACY_HIGH,"skip FS: RLF in connect mode or OOS in connecting state,curr_service_status=%d",tdsrrc_csp_int_data.curr_service_status);
      
         tdsrrc_csp_int_data.curr_service_status     = MM_AS_NO_SERVICE;
         tdsrrc_csp_int_data.curr_sel_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;

         if(tdsrrccsp_send_mm_cmd(RRC_SERVICE_IND,
                                tdsrrc_csp_int_data.curr_service_status, 
                                &tdsrrc_csp_int_data.curr_sel_plmn,
                                &tdsrrc_csp_int_data.curr_acq_entry) 
            == FAILURE) 
         {
           ERR_FATAL("RRC Unable to send RRC_SERVICE_IND",0,0,0);
         }
      
         /* Send disable BPLMN IND to NAS to disable periodic BPLMN searches.*/
         tdsrrccsp_send_disable_bplmn_ind(TRUE);
	     return TDSRRCCSP_NO_CELL_SELECTED;
	  }
    }

    /* skip FS on first no service ind due to OOS */
    /* Do not handle TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_CONNECTING_STATE_SCAN 
       because it needs to go back to RCE.
    */
    if ((((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) && 
           ((tdsrrcrce_get_rce_sub_state() != TDSRRCRCR_WAIT_FOR_SRB_REL_CNF) && (tdsrrcrce_get_rce_sub_state() != TDSRRCRCE_WAIT_FOR_RB0_REL_CNF)))
         || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN))
        && (type_freq_scan == TDSRRC_CSP_RAW_SCAN))
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"CSP OOS scan %d to skip FS, curr_service_status=%d, rce st: %d", 
                  tdsrrc_csp_int_data.curr_service_status, 
                  tdsrrc_csp_int_data.curr_scan, 
                  tdsrrcrce_get_rce_sub_state());
      
      if ((RRC_EST_EMERGENCY_CALL == tdsrrcrce_get_rrc_est_cause())
          && (tdsrrc_get_state() != TDSRRC_STATE_DISCONNECTED))
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Emergency call,skip special OOS handling"); 
      }
      else
      {
      tdsrrc_csp_int_data.curr_service_status     = MM_AS_NO_SERVICE;
      tdsrrc_csp_int_data.curr_sel_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;

      if(tdsrrccsp_send_mm_cmd(RRC_SERVICE_IND,
                             tdsrrc_csp_int_data.curr_service_status, 
                             &tdsrrc_csp_int_data.curr_sel_plmn,
                             &tdsrrc_csp_int_data.curr_acq_entry) 
         == FAILURE)
      {
        ERR_FATAL("RRC Unable to send RRC_SERVICE_IND",0,0,0);
      }
      
      /* Send disable BPLMN IND to NAS to disable periodic BPLMN searches.*/
      tdsrrccsp_send_disable_bplmn_ind(TRUE);

      return TDSRRCCSP_NO_CELL_SELECTED;
    }
    }
#endif

  /* ZZTBD: need to verify all above cases and find out
   * if TDSRRC_CSP_ACQ_SCAN is a continuation of TDSRRC_CSP_LIST_SCAN
   */
  tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);

  if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
      || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
     )
  {
    /* Start waiting for Acquisition Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
  }
  else
  {
    /* Start waiting for Frequency scan Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
  }
  
  return(next_substate);
}

/*===========================================================================

FUNCTION          tdsrrccsp_release_chls_and_continue_cell_selection

DESCRIPTION       This function releases all logical, transport and physical
                  channels before continuing with cell selection.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_release_chls_and_continue_cell_selection(void)
{
  tdsrrc_csp_substate_e_type   next_substate;
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  tdsrrccsp_cleanup_bplmn_state();

  /* Release all channels and put lower layers to Idle before sending another 
   * Acquisition Req or a Frequency Scan Req.
   */

  /* Set appropriate current scan here. Note that this is important 
   * since all channels are to be released and UE state is to be changed to Idle. 
   * Any subsequent cell selection will ALWAYS be in Idle mode - the current 
   * scan should not be left with a value that may be incompatible with 
   * Cell selection in Idle disconnected state.
   */
  switch(tdsrrc_csp_int_data.curr_scan)
  {
    case TDSRRC_CSP_CELL_RESELECTION_SCAN:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting scan to OOS in idle");
      tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN;
      break;

    case TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN:
    case TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN:
    case TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN:
    case TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN:
    case TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN:
    case TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN:
    case TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN:
    case TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN:
    case TDSRRC_CSP_CELL_SELECTION_SCAN:
    case TDSRRC_CSP_LEAVING_CONN_MODE_SCAN:
    case TDSRRC_CSP_PLMN_LIST_SCAN:
    case RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Current scan type is %d",tdsrrc_csp_int_data.curr_scan);
      break;

    case RRC_CSP_GSM_TO_TDS_CELL_RESELECTION_SCAN:
    case RRC_CSP_G2TD_CC_ORDER_SCAN:
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unexpected scan type %d",tdsrrc_csp_int_data.curr_scan);
      break;
  }

  /* First call a CCM function to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();

  if(tdsrrccsp_send_chan_release() == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  if(rrc_state != TDSRRC_STATE_DISCONNECTED)
  {
    /* Set the CSP substate to TDSRRCCSP_CELL_SEL_WT_STATE_CHANGE, where
     * on getting the STATE_CHANGE_IND (to IDLE), CSP will send
     * NO SERVICE to MM.
     */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSP waits for state change ind");
    next_substate = TDSRRCCSP_CELL_SEL_WT_STATE_CHANGE;
  }
  else
  {
    if(tdrm_check_cli_state_is_in_lock(TDRM_CLIENT_RRC) &&
        ((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) ||
         (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN) ||
         (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN))
      )
    {
      tdsrrc_csp_int_data.csp_waiting_for_wl1_to_release_lock = TRUE;

      if(tdsrrccsp_send_llc_cmd(TDSRRC_RESET_L1_REQ) == FAILURE)
      {
        ERR_FATAL("RRC Unable to get cmd buf for RRC cmd 0x%x",TDSRRC_RESET_L1_REQ,0,0);
      }
      return (tdsrrccsp_get_csp_substate());
    }

    next_substate = tdsrrccsp_continue_cell_selection();
  }

  return(next_substate);
}

/*===========================================================================

FUNCTION          tdsrrccsp_release_bch_and_continue_cell_selection

DESCRIPTION       This function releases BCCH/BCH/S_PCCPCH pipe before 
                  continuing with cell selection.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_release_bch_and_continue_cell_selection(void)
{
  tdsrrc_csp_substate_e_type   next_substate;

  /* First call a CCM function to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();

  /* Release all channels and put lower layers to Idle before sending another 
   * Acquisition Req or a Frequency Scan Req.
   */

  if(((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) ||
       (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN) ||
       (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN)||
       (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)||
       (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
    )
  {
    tdsrrc_csp_int_data.csp_waiting_for_wl1_to_release_lock = TRUE;

    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)||
       (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
    {
       if(tdsrrccsp_send_bch_release(TRUE) == FAILURE)
       {
          ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
       }
    }
	else
    {
      if(tdsrrccsp_send_llc_cmd(TDSRRC_RESET_L1_REQ) == FAILURE)
      {
        ERR_FATAL("RRC Unable to get cmd buf for RRC cmd 0x%x",TDSRRC_RESET_L1_REQ,0,0);
      }
	}
    return (tdsrrccsp_get_csp_substate());
  }
  if(tdsrrccsp_send_bch_release(FALSE) == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }
  /* Suspend BPLMN search if the remaining DRX Srch timer is not sufficient (less than 120ms),
   * to avoid the notification of CPHY_GO_TO_SLEEP_IND is too late */
  if(((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) ||
         (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)) &&
     (TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress()) &&
     (!tdsrrc_csp_int_data.bplmn_srch_tmr_expired) &&
     (tdsrrctmr_get_remaining_time(RRCTMR_TD2TD_BPLMN_DRX_SRCH_TIMER) < (TD2TD_BPLMN_SEARCH_OVERHEAD_TIMER_IN_MS + TDSRRC_MIN_WAIT_TIME_TO_L1))
    )
  {
    next_substate = tdsrrccsp_suspend_td2td_bplmn_ahead_of_srch_timer();
  }
  /* Suspend X2TD BPLMN search if the remaining Srch timer is not sufficient (less than 150ms)
   * to avoid return to GSM/LTE too late */
  else if(((tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) ||
          (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)) &&
          (!tdsrrc_csp_int_data.bplmn_srch_tmr_expired) &&
          (tdsrrctmr_get_remaining_time(TDSRRCTMR_BPLMN_SRCH_TIMER) < X2TD_BPLMN_SEARCH_OVERHEAD_TIMER_IN_MS))
  {
    next_substate = tdsrrccsp_suspend_x2td_bplmn_ahead_of_srch_timer();
  }
  else
  {
    next_substate = tdsrrccsp_continue_cell_selection();
  }

  return(next_substate);
}


/*===========================================================================

FUNCTION          RRC_CSP_START_SEL_REDIRECT_FREQ_IN_CONNECTING_STATE
                  

DESCRIPTION       This function starts cell selection on the designated
                  frequency received by RRC in inter-frequency redirection
                  request in connecting state.
                  Only transport and physical channels are released before
                  attempting acquisition on the designated frequency.
                  Cell selection is started for the current acquisition type.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_redirect_freq_in_connecting_state(void)
{
  tdsrrc_csp_substate_e_type next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;

  /* First call a CCM function to indicate Cell Loss - no new cell */
  tdsrrcccm_update_with_cell_loss();

  /* Release transport and physical channels only.*/
  if(tdsrrccsp_send_tr_phy_chl_release() == SUCCESS)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Acq req for redirected freq %d sent to L1",tdsrrc_csp_int_data.curr_acq_entry.freq);

    /* Send redirected frequency to L1 for acquisition */
    tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, TDSRRC_CSP_ACQ_SCAN);
  }
  else
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  return(next_substate);
} /* tdsrrccsp_start_sel_redirect_freq_in_connecting_state */


/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_out_of_service_area

DESCRIPTION       This function starts a cell selection on an out of service
                  area condition. 
                  The cell selection is started in the current acquisition 
                  type. The current scan is set to Out of Service area scan.
                  Based on the returned frequency scan type, a CSP substate
                  is returned.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_out_of_service_area(void)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan;

  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;

  tdsrrccsp_cleanup_bplmn_state();

  /* Set current scan to "Out of Service Area" and 
   * start 30s timer for Spec PLMN search
   */   
#ifdef FEATURE_WRLF_SYSTEM_SEL
  tdsrrc_csp_int_data.ue_in_conn_oos = FALSE;
  if(RRC_EST_EMERGENCY_CALL != tdsrrcrce_get_rrc_est_cause())
  {
    tdsrrc_csp_int_data.ue_in_conn_oos = TRUE;
    tdsrrc_csp_int_data.rlf_plmn_id = tdsrrc_csp_int_data.curr_req_plmn.plmn_id;
  
    /* Perform ACQ-DB search. */
    tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_LEAVING_CONN_MODE_SCAN;
  }
  else
#endif
  {
    tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN;
  }


  tdsrrctmr_start_timer(TDSRRCTMR_CONN_MODE_OOS_TIMER, TDSRRCTMR_CONN_MODE_OOS_TIMER_IN_MS);
  tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd = FALSE;
  tdsrrc_csp_int_data.oos_gsm_srch_done       = FALSE;
  tdsrrc_csp_int_data.oos_gsm_num_avail_plmns = 0;
  tdsrrc_csp_int_data.plmn_list.num_plmns     = 0;


#ifdef FEATURE_WRLF_SYSTEM_SEL
  if(RRC_EST_EMERGENCY_CALL == tdsrrcrce_get_rrc_est_cause())
#endif
  {
    #ifdef FEATURE_DISABLE_DEEP_SLEEP 
    /* Do not start the no deep sleep timer when this feature
     * is enabled. Otherwise, if we start the timer then on
     * its expiry, UE is put into deep sleep which we need to
     * avoid.
     * Also, continue setting the var tdsrrc_csp_int_data.oos_initial_no_deep_sleep_tmr_expd
     * to false so that deep sleep is not triggered.
     */
#else
    tdsrrctmr_start_timer(TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER, 
                       TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER_IN_MS);
#endif /* FEATURE_DISABLE_DEEP_SLEEP */

    tdsrrc_csp_int_data.oos_initial_no_deep_sleep_tmr_expd = FALSE;
    tdsrrc_csp_int_data.oos_conn_mode_ds_tmr_expd          = TRUE;

#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
    if(!tdsrrc_csp_int_data.oos_due_to_no_resource)
    {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"SVC_ST: Sending UI NO SERVICE in OOSC");
       tdsrrccsp_send_svc_status_to_mm(SYS_SRV_STATUS_NO_SRV, SYS_RAT_TDS_RADIO_ACCESS, 0);
       tdsrrc_csp_int_data.oosc_plmn_found = FALSE;
    }
#endif
  }

  /* Save the last camped frequency. This frequency will be specifically
   * scanned during the full frequency scan
   */
  tdsrrc_csp_int_data.last_camped_freq_in_connected_mode = tdsrrc_csp_int_data.curr_acq_entry.freq;

  /* First send a Cell Change Request to CCM to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();


  tdsrrc_csp_int_data.prev_network_select_mode = SYS_NETWORK_SELECTION_MODE_NONE; 

  /* Release all channels and put lower layers to Idle before sending another 
   * Acquisition Req or a Frequency Scan Req.
   */
  if(tdsrrccsp_send_tr_phy_chl_release() == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  /* In TDD, we do LFS */
  tdsrrc_csp_int_data.do_lfs_scan = TRUE;

  if(tdsrrccsp_start_cell_selection
      (
        &tdsrrc_csp_int_data.curr_req_plmn,
        &tdsrrc_csp_int_data.curr_type_acq, 
        &tdsrrc_csp_int_data.curr_acq_entry,
        &type_freq_scan
      )   == FAILURE
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq by Stored Initial cell selection");
    return (tdsrrccsp_rel_chls_and_go_to_idle());
  }

  /* Another freq found */
  if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
  {
    /* This is unacceptable since a SUCCESS was returned */
    ERR_FATAL("NO_SCAN returned",0,0,0);
  }
 
  tdsrrcwrm_release_lock();
#ifdef FEATURE_SGLTE
  /* in this case of sglte mode and resume failed due to no trm resource, we need to report to NAS no svc w/o further 
     waiting for lock. Don't start lock wait tmr, meaning it expires when tdsrrccsp_wait_for_wrm_lock is called */
  if( !((tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE) && (tdsrrc_csp_int_data.oos_due_to_no_resource)) )
#endif
  {
  tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_DEFAULT_SEARCH_TIME_IN_MS,TRUE);
  }
  next_substate = tdsrrccsp_wait_for_wrm_lock(&type_freq_scan);
  if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
  {
    return (next_substate);
  }

  tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);

  if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
      || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
     )
  {
    /* Start waiting for Acquisition Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
  }
  else
  {
    /* Start waiting for Frequency scan Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
  }
  
  return(next_substate);
}

/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_out_of_service_area_in_idle_state

DESCRIPTION       This function starts a cell selection on an out of service
                  area condition in Idle state.
                  All channels are released. 
                  The cell selection is started in the current acquisition 
                  type. The current scan is set to Out of Service area in
                  Idle state scan.
                  Based on the returned frequency scan type, a CSP substate
                  is returned.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_out_of_service_area_in_idle_state(void)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan;
  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;
  boolean is_mbplmn_no_trm_OOS = FALSE;

  /* Set current scan to "Out of Service Area".*/   
  tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN;

  /* First call a CCM function to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();

  /* Release TRM lock if acquired before starting OOS scan */
  tdsrrcwrm_release_lock();
  
  /* Release all channels and put lower layers to Idle before sending another 
   * Acquisition Req or a Frequency Scan Req.
   */
  if(tdsrrccsp_send_chan_release() == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  /* Signal SIB procedure to clear up the active event, if any */
  tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_TO_OOS);

  /* Manual BPLMN search is in progress */
  if(    (TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
      && (tdsrrc_csp_int_data.bplmn_nw_sel_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
      && (!tdsrrc_get_oos_due_to_no_resource_flag_status()) /* Abort PLMN search only for the lock unavailable case */
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Continue Manual PLMN Search in Foreground.");
    next_substate = tdsrrccsp_continue_bplmn_in_frgrnd();
  }
  /* Manual BPLMN search is NOT in progress */
  else
  {
     /* Manual BPLMN search is in progress */
    if((TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
      && (tdsrrc_csp_int_data.bplmn_nw_sel_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
      && (tdsrrc_get_oos_due_to_no_resource_flag_status()) /* Abort PLMN search only for the lock unavailable case */
    )
    {
      is_mbplmn_no_trm_OOS = TRUE; 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No TRM OOS during M BPLMN");
    }
    tdsrrccsp_cleanup_bplmn_state();

    /* In TDD, we do LFS */
    tdsrrc_csp_int_data.do_lfs_scan = TRUE;
    
    if(tdsrrc_csp_int_data.est_req_for_emerg_call)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"OOS: Send NO SERVICE to NAS Immediately");
      tdsrrccsp_handle_no_available_freq();
      next_substate = TDSRRCCSP_NO_CELL_SELECTED;
      tdsrrc_csp_int_data.est_req_for_emerg_call = FALSE;
    }
    else if(tdsrrc_get_oos_due_to_no_resource_flag_status() &&
            (is_mbplmn_no_trm_OOS == TRUE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"OOS: Send NO SERVICE to NAS Immediately");
      tdsrrccsp_handle_no_available_freq();
      next_substate = TDSRRCCSP_NO_CELL_SELECTED;
    }
    else
    {
      if(tdsrrccsp_start_cell_selection
          (
            &tdsrrc_csp_int_data.curr_req_plmn,
            &tdsrrc_csp_int_data.curr_type_acq, 
            &tdsrrc_csp_int_data.curr_acq_entry,
            &type_freq_scan
          )   == FAILURE
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq by Stored Initial cell selection");
        return (tdsrrccsp_rel_chls_and_go_to_idle());
      }
  
      /* Another freq found */
      if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
      {
        /* This is unacceptable since a SUCCESS was returned */
        ERR_FATAL("NO_SCAN returned",0,0,0);
      }
  
      tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);
  
      if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
        || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
       )
      {
        /* Start waiting for Acquisition Confirmation */
        next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
      }
      else
      {
        /* Start waiting for Frequency scan Confirmation */
        next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
      }
    }

  }
  return(next_substate);
}

/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_out_of_service_area_in_connecting_state

DESCRIPTION       This function starts a cell selection on an out of service
                  area condition in Connecting state.
                  All channels are released. 
                  The cell selection is started in the current acquisition 
                  type. The current scan is set to Out of Service area in
                  Connecting state scan.
                  Based on the returned frequency scan type, a CSP substate
                  is returned.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_out_of_service_area_in_connecting_state(void)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan;

  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;
  
  tdsrrccsp_cleanup_bplmn_state();

  /* Set current scan to "Out of Service Area".*/   
  tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_CONNECTING_STATE_SCAN;

  /* Save the last camped frequency. This frequency will be specifically
   * scanned during the full frequency scan
   */
  tdsrrc_csp_int_data.last_camped_freq_in_connected_mode = tdsrrc_csp_int_data.curr_acq_entry.freq;

  /* First call a CCM function to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();


  /* Release tr & phy channels and put lower layers to Idle before sending another 
   * Acquisition Req or a Frequency Scan Req.
   */
  if(tdsrrccsp_send_tr_phy_chl_release() == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  /* In TDD, we do LFS */
  tdsrrc_csp_int_data.do_lfs_scan = TRUE;

  if(tdsrrccsp_start_cell_selection
      (
        &tdsrrc_csp_int_data.curr_req_plmn,
        &tdsrrc_csp_int_data.curr_type_acq, 
        &tdsrrc_csp_int_data.curr_acq_entry,
        &type_freq_scan
      )   == FAILURE
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq by Stored Initial cell selection");
    return (tdsrrccsp_rel_chls_and_go_to_idle());
  }

  /* Another freq found */
  if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
  {
    /* This is unacceptable since a SUCCESS was returned */
    ERR_FATAL("NO_SCAN returned",0,0,0);
  }
 
  tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);

  if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
      || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
     )
  {
    /* Start waiting for Acquisition Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
  }
  else
  {
    /* Start waiting for Frequency scan Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
  }
  
  return(next_substate);
}

/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_from_dch_to_fach

DESCRIPTION       This function starts a cell selection on a transition from
                  CELL_DCH to CELL_FACH state.                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_from_dch_to_fach(tdsrrc_cmd_type  *rrc_cmd_ptr)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan;

  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;
  tdsrrc_state_e_type          rrc_state     =  tdsrrc_get_state();
  
  /* Set current scan to "Out of Service Area".*/   
  tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN;

  /* Store the destination state as it will be used to configure appropriate
   * channels when UE camps on a suitable cell. The reason that we need to
   * store this information although we can deduce the destination state from
   * the curr_scan is because this curr_scan may change to TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN
   * when CU initiates cell selection with cause OOS area indication
   */
  tdsrrc_csp_int_data.dch_tx_dest_state = TDSRRC_STATE_CELL_FACH;

  /* Save the last camped frequency. This frequency will be specifically
   * scanned during the full frequency scan
   */
  tdsrrc_csp_int_data.last_camped_freq_in_connected_mode = tdsrrc_csp_int_data.curr_acq_entry.freq;

  /* Initialize destination frequency info if passed in the command */
  if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq_present)
  {
    tdsrrc_csp_int_data.dest_freq = rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq;
    tdsrrc_csp_int_data.dest_freq_present = TRUE;
  }
  else
  {
    tdsrrc_csp_int_data.dest_freq_present = FALSE;
  }

  /* Initialize destination Pri Scr Code info if passed in the command */
  if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid_present)
  {
    tdsrrc_csp_int_data.dest_cpid = rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid;
    tdsrrc_csp_int_data.dest_cpid_present = TRUE;
  }
  else
  {
    tdsrrc_csp_int_data.dest_cpid_present = FALSE;
  }

  /* First send a Cell Change Request to CCM to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();

  /* Signal SIB procedure to clear up the active event, if any */
  tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_TO_OOS);

  /* Release all channels and put lower layers to Idle before sending another 
   * Acquisition Req or a Frequency Scan Req.
   */
  if(tdsrrccsp_send_tr_phy_chl_release() == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  /* Here start a timer, at the expiry of which, the RRC state is taken back
   * to Idle. This is the max time for Cell selection during a transition from
   * DCH to FACH.
   */                       
  if(tdsrrccsp_is_state_transition_triggered_by_reconfig_msg())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"OOSC: DCH->FACH due to reconfiguration");
    tdsrrctmr_start_timer(TDSRRCTMR_DCH_FACH_CELL_SELECTION_TIMER, 
                       (TDSRRCTMR_DCH_FACH_CELL_SELECTION_TIMER_IN_MS + tdsrrcllc_get_dch_fach_act_time_in_ms()));

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"DCH->FACH timer: started %d ms + act_time %d",TDSRRCTMR_DCH_FACH_CELL_SELECTION_TIMER_IN_MS,tdsrrcllc_get_dch_fach_act_time_in_ms());

    /* set the timer as not expired */
    tdsrrc_csp_int_data.dch_to_fach_pch_tmr_expd = FALSE;
  }
  /* Since the DCH->FACH tx is NOT due to reconfiguration, change the
   * type of scan to OOS_Area_Scan which will allow the UE to search 
   * for service 
   */
  else
  {
    TDSRRC_DEBUG_INCREMENT_STATS(TDSOOS_IN_CONNECTED_MODE);
    switch(rrc_state)
    {
        case TDSRRC_STATE_CELL_FACH:
            TDSRRC_DEBUG_INCREMENT_STATS(TDSOOS_IN_CONNECTED_MODE_CELL_FACH);            
            break;
        case TDSRRC_STATE_CELL_DCH:
            TDSRRC_DEBUG_INCREMENT_STATS(TDSOOS_IN_CONNECTED_MODE_CELL_DCH);
            break;
        case  TDSRRC_STATE_CELL_PCH:
            TDSRRC_DEBUG_INCREMENT_STATS(TDSOOS_IN_CONNECTED_MODE_CELL_PCH);
            break;
        case  TDSRRC_STATE_URA_PCH:
            TDSRRC_DEBUG_INCREMENT_STATS(TDSOOS_IN_CONNECTED_MODE_URA_PCH);
            break;
        default:
            break;
    }

#ifdef FEATURE_WRLF_SYSTEM_SEL
    if(RRC_EST_EMERGENCY_CALL != tdsrrcrce_get_rrc_est_cause())
    {
      tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_LEAVING_CONN_MODE_SCAN;
      tdsrrc_csp_int_data.ue_in_conn_oos = TRUE;
      tdsrrc_csp_int_data.rlf_plmn_id = tdsrrc_csp_int_data.curr_req_plmn.plmn_id;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF: Changing curr_scan to TDSRRC_CSP_LEAVING_CONN_MODE_SCAN. Start splmn srch 30s tmr");
    }
    else
#endif
    {
      tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF: Changing curr_scan to TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN. Start splmn srch 30s tmr");
    }

    tdsrrctmr_start_timer(TDSRRCTMR_CONN_MODE_OOS_TIMER, TDSRRCTMR_CONN_MODE_OOS_TIMER_IN_MS);
    tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd = FALSE;
    tdsrrc_csp_int_data.oos_gsm_srch_done       = FALSE;
    tdsrrc_csp_int_data.oos_gsm_num_avail_plmns = 0;
    tdsrrc_csp_int_data.plmn_list.num_plmns     = 0;


#ifdef FEATURE_WRLF_SYSTEM_SEL
    if(RRC_EST_EMERGENCY_CALL == tdsrrcrce_get_rrc_est_cause())
#endif
    {
#ifdef FEATURE_DISABLE_DEEP_SLEEP 
      /* Do not start the no deep sleep timer when this feature
       * is enabled. Otherwise, if we start the timer then on
       * its expiry, UE is put into deep sleep which we need to
       * avoid.
       * Also, continue setting the var tdsrrc_csp_int_data.oos_initial_no_deep_sleep_tmr_expd
       * to false so that deep sleep is not triggered.
       */
#else

      tdsrrctmr_start_timer(TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER, 
                         TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER_IN_MS);

#endif /* FEATURE_DISABLE_DEEP_SLEEP */

      tdsrrc_csp_int_data.oos_initial_no_deep_sleep_tmr_expd = FALSE;
      tdsrrc_csp_int_data.oos_conn_mode_ds_tmr_expd          = TRUE;

#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
      if(!tdsrrc_csp_int_data.oos_due_to_no_resource)
      {
        tdsrrccsp_send_svc_status_to_mm(SYS_SRV_STATUS_NO_SRV, SYS_RAT_TDS_RADIO_ACCESS, 0);
        tdsrrc_csp_int_data.oosc_plmn_found = FALSE;
      }
#endif
    }

    tdsrrc_csp_int_data.prev_network_select_mode = SYS_NETWORK_SELECTION_MODE_NONE;
   
  }

  /* For TDD, do LFS if not indicated single cell explicitly by Network */
  if (tdsrrc_csp_int_data.dest_cpid_present)
  {
     tdsrrc_csp_int_data.do_lfs_scan = FALSE;
  }
  else
  {
     tdsrrc_csp_int_data.do_lfs_scan = TRUE;
  }

  if(tdsrrccsp_start_cell_selection
      (
        &tdsrrc_csp_int_data.curr_req_plmn,
        &tdsrrc_csp_int_data.curr_type_acq, 
        &tdsrrc_csp_int_data.curr_acq_entry,
        &type_freq_scan
      )   == FAILURE
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq by Stored Initial cell selection");
    return (tdsrrccsp_rel_chls_and_go_to_idle());
  }

  /* Another freq found */
  if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
  {
    /* This is unacceptable since a SUCCESS was returned */
    ERR_FATAL("NO_SCAN returned",0,0,0);
  }
 
  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN
#ifdef FEATURE_WRLF_SYSTEM_SEL
     || tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN
#endif
    )
  {
    tdsrrcwrm_release_lock();
#ifdef FEATURE_SGLTE
    /* in this case of sglte mode and resume failed due to no trm resource, we need to report to NAS no svc w/o further 
           waiting for lock. Don't start lock wait tmr, meaning it expires when tdsrrccsp_wait_for_wrm_lock is called */
    if( !((tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE) && (tdsrrc_csp_int_data.oos_due_to_no_resource)) )
#endif
    {
    tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_DEFAULT_SEARCH_TIME_IN_MS,TRUE);
    }

    next_substate = tdsrrccsp_wait_for_wrm_lock(&type_freq_scan);
    if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
    {
      return (next_substate);
    }
  }

  tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);

  if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
      || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
     )
  {
    /* Start waiting for Acquisition Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
  }
  else
  {
    /* Start waiting for Frequency scan Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
  }
  
  return(next_substate);
} /* tdsrrccsp_start_sel_from_dch_to_fach */


/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_from_dch_to_pch

DESCRIPTION       This function starts a cell selection on a transition from
                  CELL_DCH to CELL_PCH or URA_PCH state.                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_from_dch_to_pch(tdsrrc_cmd_type  *rrc_cmd_ptr)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan;

  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;
  
  /* Set current scan to CELL_PCH or URA_PCH scan */
  if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_state == TDSRRC_STATE_CELL_PCH)
  {
    tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN;

    /* Store the destination state as it will be used to configure appropriate
     * channels when UE camps on a suitable cell. The reason that we need to
     * store this information although we can deduce the destination state from
     * the curr_scan is because this curr_scan may change to TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN
     * when CU initiates cell selection with cause OOS area indication
     */
    tdsrrc_csp_int_data.dch_tx_dest_state = TDSRRC_STATE_CELL_PCH;
  }
  else if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_state == TDSRRC_STATE_URA_PCH)
  {
    tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN;

    /* Store the destination state as it will be used to configure appropriate
     * channels when UE camps on a suitable cell. The reason that we need to
     * store this information although we can deduce the destination state from
     * the curr_scan is because this curr_scan may change to TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN
     * when CU initiates cell selection with cause OOS area indication
     */
    tdsrrc_csp_int_data.dch_tx_dest_state = TDSRRC_STATE_URA_PCH;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Dest state %d not processed here");
    return next_substate;
  }

  /* Save the last camped frequency. This frequency will be specifically
   * scanned during the full frequency scan
   */
  tdsrrc_csp_int_data.last_camped_freq_in_connected_mode = tdsrrc_csp_int_data.curr_acq_entry.freq;


  /* Initialize destination frequency info if passed in the command */
  if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq_present)
  {
    tdsrrc_csp_int_data.dest_freq = rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq;
    tdsrrc_csp_int_data.dest_freq_present = TRUE;
  }
  else
  {
    tdsrrc_csp_int_data.dest_freq_present = FALSE;
  }

  /* Initialize destination Pri Scr Code info if passed in the command */
  if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid_present)
  {
    tdsrrc_csp_int_data.dest_cpid = rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid;
    tdsrrc_csp_int_data.dest_cpid_present = TRUE;
  }
  else
  {
    tdsrrc_csp_int_data.dest_cpid_present = FALSE;
  }

  /* First send a Cell Change Request to CCM to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();

  /* Signal SIB procedure to clear up the active event, if any */
  tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_TO_OOS);

  /* Release all channels and put lower layers to Idle before sending another 
   * Acquisition Req or a Frequency Scan Req.
   */
  if(tdsrrccsp_send_tr_phy_chl_release() == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  /* For TDD, do LFS if not indicated single cell explicitly by Network */
  if (tdsrrc_csp_int_data.dest_cpid_present)
  {
     tdsrrc_csp_int_data.do_lfs_scan = FALSE;
  }
  else
  {
     tdsrrc_csp_int_data.do_lfs_scan = TRUE;
  }

  if(tdsrrccsp_start_cell_selection
      (
        &tdsrrc_csp_int_data.curr_req_plmn,
        &tdsrrc_csp_int_data.curr_type_acq, 
        &tdsrrc_csp_int_data.curr_acq_entry,
        &type_freq_scan
      )   == FAILURE
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq by Stored Initial cell selection");
    return (tdsrrccsp_rel_chls_and_go_to_idle());
  }

  /* Another freq found */
  if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
  {
    /* This is unacceptable since a SUCCESS was returned */
    ERR_FATAL("NO_SCAN returned",0,0,0);
  }
 
  /* Here start a timer, at the expiry of which, the RRC state is taken back
   * to Idle. This is the max time for Cell selection during a transition from
   * DCH to CELL_PCH or URA_PCH.
   */
  tdsrrctmr_start_timer( TDSRRCTMR_DCH_PCH_CELL_SELECTION_TIMER, 
                      TDSRRCTMR_DCH_PCH_CELL_SELECTION_TIMER_IN_MS );

  /* set the timer as not expired */
  tdsrrc_csp_int_data.dch_to_fach_pch_tmr_expd = FALSE;

  tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);

  if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
      || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
     )
  {
    /* Start waiting for Acquisition Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
  }
  else
  {
    /* Start waiting for Frequency scan Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
  }
  
  return(next_substate);
} /* tdsrrccsp_start_sel_from_dch_to_pch */

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION          RRC_CSP_START_SEL_FROM_FACH_TO_FACH

DESCRIPTION       This function starts a cell selection on a transition from
                  CELL_FACH to CELL_FACH state with frequency redirection info.
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_from_fach_to_fach(tdsrrc_cmd_type  *rrc_cmd_ptr)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan;

  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;
  
  tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN;

  /* Store the destination state as it will be used to configure appropriate
   * channels when UE camps on a suitable cell. The reason that we need to
   * store this information although we can deduce the destination state from
   * the curr_scan is because this curr_scan may change to TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN
   * when CU initiates cell selection with cause OOS area indication
   */
  tdsrrc_csp_int_data.fach_tx_dest_state = TDSRRC_STATE_CELL_FACH;

  /* Save the last camped frequency. This frequency will be specifically
   * scanned during the full frequency scan
   */
  tdsrrc_csp_int_data.last_camped_freq_in_connected_mode = tdsrrc_csp_int_data.curr_acq_entry.freq;

  /* Initialize destination frequency info if passed in the command */
  if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq_present)
  {
    tdsrrc_csp_int_data.dest_freq = rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq;
    tdsrrc_csp_int_data.dest_freq_present = TRUE;
  }
  else
  {
    tdsrrc_csp_int_data.dest_freq_present = FALSE;
  }

  /* Initialize destination Pri Scr Code info if passed in the command */
  if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid_present)
  {
    tdsrrc_csp_int_data.dest_cpid = rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid;
    tdsrrc_csp_int_data.dest_cpid_present = TRUE;
  }
  else
  {
    tdsrrc_csp_int_data.dest_cpid_present = FALSE;
  }

  /* First send a Cell Change Request to CCM to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();

  /* Signal SIB procedure to clear up the active event, if any */
  tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_TO_OOS);

  /* Release all channels and put lower layers to Idle before sending another 
   * Acquisition Req or a Frequency Scan Req.
   */
  if(tdsrrccsp_send_tr_phy_chl_release() == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  /* Here start a timer, at the expiry of which, the RRC state is taken back
   * to Idle. This is the max time for Cell selection during a transition from
   * FACH to FACH.
   */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"FACH->FACH due to reconfiguration");
  tdsrrctmr_start_timer(TDSRRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER, 
                     TDSRRCTMR_FACH_TO_FACH_CELL_SELECTION_TIMER_IN_MS);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"FACH->FACH timer: started %d ms",TDSRRCTMR_FACH_TO_FACH_CELL_SELECTION_TIMER_IN_MS);

  /* set the timer as not expired */
  tdsrrc_csp_int_data.fach_to_fach_pch_tmr_expd = FALSE;

  /* For TDD, do LFS if not indicated single cell explicitly by Network */
  if (tdsrrc_csp_int_data.dest_cpid_present)
  {
     tdsrrc_csp_int_data.do_lfs_scan = FALSE;
  }
  else
  {
     tdsrrc_csp_int_data.do_lfs_scan = TRUE;
  }

  if(tdsrrccsp_start_cell_selection
      (
        &tdsrrc_csp_int_data.curr_req_plmn,
        &tdsrrc_csp_int_data.curr_type_acq, 
        &tdsrrc_csp_int_data.curr_acq_entry,
        &type_freq_scan
      )   == FAILURE
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq by Stored Initial cell selection");
    return (tdsrrccsp_rel_chls_and_go_to_idle());
  }

  /* Another freq found */
  if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
  {
    /* This is unacceptable since a SUCCESS was returned */
    ERR_FATAL("NO_SCAN returned",0,0,0);
  }
 
  tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);

  if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
      || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
     )
  {
    /* Start waiting for Acquisition Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
  }
  else
  {
    /* Start waiting for Frequency scan Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
  }
  
  return(next_substate);
} /* tdsrrccsp_start_sel_from_fach_to_fach */

/*===========================================================================

FUNCTION          RRC_CSP_START_SEL_FROM_FACH_TO_PCH

DESCRIPTION       This function starts a cell selection on a transition from
                  CELL_FACH to CELL_PCH or URA_PCH state with frequency redirection info.
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_from_fach_to_pch(tdsrrc_cmd_type  *rrc_cmd_ptr)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan;

  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;

  /* Set current scan to CELL_PCH or URA_PCH scan */
  if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_state == TDSRRC_STATE_CELL_PCH)
  {
    tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN;
  }
  else if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_state == TDSRRC_STATE_URA_PCH)
  {
    tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Dest state %d not processed here");
    return next_substate;
  }

  /* Store the destination state as it will be used to configure appropriate
   * channels when UE camps on a suitable cell. The reason that we need to
   * store this information although we can deduce the destination state from
   * the curr_scan is because this curr_scan may change to TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN
   * when CU initiates cell selection with cause OOS area indication
   */
  tdsrrc_csp_int_data.fach_tx_dest_state = rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_state;

  /* Save the last camped frequency. This frequency will be specifically
   * scanned during the full frequency scan
   */
  tdsrrc_csp_int_data.last_camped_freq_in_connected_mode = tdsrrc_csp_int_data.curr_acq_entry.freq;


  /* Initialize destination frequency info if passed in the command */
  if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq_present)
  {
    tdsrrc_csp_int_data.dest_freq = rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq;
    tdsrrc_csp_int_data.dest_freq_present = TRUE;
  }
  else
  {
    tdsrrc_csp_int_data.dest_freq_present = FALSE;
  }

  /* Initialize destination Pri Scr Code info if passed in the command */
  if(rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid_present)
  {
    tdsrrc_csp_int_data.dest_cpid = rrc_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid;
    tdsrrc_csp_int_data.dest_cpid_present = TRUE;
  }
  else
  {
    tdsrrc_csp_int_data.dest_cpid_present = FALSE;
  }

  /* First send a Cell Change Request to CCM to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();

  /* Signal SIB procedure to clear up the active event, if any */
  tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_DUE_TO_OOS);

  /* Release all channels and put lower layers to Idle before sending another 
   * Acquisition Req or a Frequency Scan Req.
   */
  if(tdsrrccsp_send_tr_phy_chl_release() == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  /* Here start a timer, at the expiry of which, the RRC state is taken back
   * to Idle. This is the max time for Cell selection during a transition from
   * FACH to FACH.
   */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"FACH->PCH/URA_PCH due to reconfiguration");
  tdsrrctmr_start_timer(TDSRRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER, 
                     TDSRRCTMR_FACH_TO_PCH_CELL_SELECTION_TIMER_IN_MS);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"FACH->PCH/URA_PCH timer: started %d ms",TDSRRCTMR_FACH_TO_PCH_CELL_SELECTION_TIMER_IN_MS);

  /* set the timer as not expired */
  tdsrrc_csp_int_data.fach_to_fach_pch_tmr_expd = FALSE;

  /* For TDD, do LFS if not indicated single cell explicitly by Network */
  if (tdsrrc_csp_int_data.dest_cpid_present)
  {
     tdsrrc_csp_int_data.do_lfs_scan = FALSE;
  }
  else
  {
     tdsrrc_csp_int_data.do_lfs_scan = TRUE;
  }

  if(tdsrrccsp_start_cell_selection
      (
        &tdsrrc_csp_int_data.curr_req_plmn,
        &tdsrrc_csp_int_data.curr_type_acq, 
        &tdsrrc_csp_int_data.curr_acq_entry,
        &type_freq_scan
      )   == FAILURE
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq by Stored Initial cell selection");
    return (tdsrrccsp_rel_chls_and_go_to_idle());
  }

  /* Another freq found */
  if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
  {
    /* This is unacceptable since a SUCCESS was returned */
    ERR_FATAL("NO_SCAN returned",0,0,0);
  }
 
  tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);

  if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
      || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
     )
  {
    /* Start waiting for Acquisition Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
  }
  else
  {
    /* Start waiting for Frequency scan Confirmation */
    next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
  }
  
  return(next_substate);
} /* tdsrrccsp_start_sel_from_fach_to_pch */

#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */

/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_leaving_conn_mode

DESCRIPTION       This function starts a cell selection on leaving connected
                  mode.
                  The cell selection is started in the current acquisition 
                  type. The current scan is set to leaving Connected mode scan.
                  Based on the returned frequency scan type, a CSP substate
                  is returned.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_leaving_conn_mode(void)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan;

  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;

  boolean mm_registration_in_progress = FALSE;

  boolean bplmn_srch_actively_in_progress = FALSE;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Leaving conn mode. curr_scan %d",tdsrrc_csp_int_data.curr_scan);

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
  if((tdsrrc_csfb_extended_status != CSFB_INVALID) && tdsrrcmcm_is_lte_mode_enabled()
#ifdef FEATURE_WRLF_SYSTEM_SEL
      && (!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
     )
  {
    if(tdsrrccho_is_td2g_cco_active())
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2G CCO Active. Do not trigger TD2L redirection.");
      return next_substate;
    }

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB_HACK: Suspend TD-SCDMA and start TD2L blind redirection.");
    tdsrrc_csp_int_data.saved_rrc_cmd.cmd.redirect_req.proc_id = TDSRRC_PROCEDURE_CSP;
    tdsrrc_csp_int_data.interrat_type = TDSRRC_CSP_INTERRAT_LTE;

    tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN;
    /* Send change mode request to MCM */
    tdsrrccsp_send_mode_change_req_to_mcm(TDSRRC_MODE_STANDBY);
    next_substate = TDSRRCCSP_WT_SUSPEND_MODE_CHANGE_CNF;
    return next_substate;
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"CSFB Extended Status is %d LTE mode enabled status is %d",tdsrrc_csfb_extended_status,tdsrrcmcm_is_lte_mode_enabled());
    tdsrrc_csfb_extended_status = CSFB_INVALID;
  }
#endif

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* Invalidate the fach_tx_dest_state */
  tdsrrc_csp_int_data.fach_tx_dest_state = TDSRRC_STATE_MAX;
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */

  /* Release TRM lock if acquired before starting leaving connected mode scan */
  tdsrrcwrm_release_lock();
  tdsrrcwrm_reset_l2_ack_req_mask();

  /* Inter Frequency Redirection Scan was aborted. Clean up its state */
  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN)
  {
    tdsrrccsp_cleanup_inter_freq_redirection_state();
  }
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Leaving Conn mode. nw_sel_mode %d, rrc_state %d, srch_st %d", 
                                      tdsrrc_csp_int_data.bplmn_nw_sel_mode,
                                      tdsrrc_get_state(), tdsrrc_csp_int_data.bplmn_srch_vars_status);

  /* If manual plmn search is not in progress or 
   * manual search is in progress but search status is not "START"
   * then cleanup bplmn state.
   */
  if(   (tdsrrc_csp_int_data.bplmn_nw_sel_mode      != SYS_NETWORK_SELECTION_MODE_MANUAL)
     || (tdsrrc_csp_int_data.bplmn_srch_vars_status != TD2TDSEARCH_START))
  {
    /* If BPLMN srch is actively in progress, release channels and send IDLE_REQ to L1 only after L1 is put to sleep */
    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) ||
        (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
    {
      bplmn_srch_actively_in_progress = TRUE;
    }


    if(bplmn_srch_actively_in_progress)
    {
      if(tdsrrccsp_send_chan_release() == FAILURE)
      {
        ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
      }
    }
  }
  /*store BPLMN data if leaving connected mode but BPLMN is not suspend yet. Like PCH->IDLE*/
  if((!tdsrrc_csp_int_data.bplmn_guard_srch_tmr_expired) && 
     (!tdsrrc_csp_bplmn_data_saved) &&
     (tdsrrc_csp_int_data.bplmn_srch_vars_status != TD2TDSEARCH_START))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Save the BPLMN data when leaving conncted mode but BPLMN is ongoing");
    tdsrrc_csp_prev_select_data = tdsrrc_csp_curr_select_data;
    tdsrrc_csp_bplmn_data_saved = TRUE;
    tdsrrc_csp_bplmn_new_srch = tdsrrc_csp_int_data.bplmn_new_srch;
    tdsrrccsp_cleanup_needed_bplmn_data();
    acq_freq_list_curr_freq_index = tdsrrc_csp_int_data.acq_freq_list.curr_freq_index;
    tdsrrccsp_stored_acq_db_curr_rd_index = tdsrrccsp_stored_acq_db.curr_rd_index;
    tds_prim_freq_list_in_use = tdsrrc_csp_int_data.tds_prim_freq_list_in_use;
    tds_prim_freq_list_rd_index = tdsrrc_csp_int_data.tds_prim_freq_list_rd_index;
  }
  else
  if (tdsrrc_csp_int_data.bplmn_guard_srch_tmr_expired && (!tdsrrc_csp_int_data.bplmn_new_srch))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cleanup BPLMN data if BPLMN guard timeout but bplmn_new_srch still not be reset");
    tdsrrccsp_cleanup_needed_bplmn_data();
  }
  
#ifdef FEATURE_WRLF_SYSTEM_SEL
  if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS() && 
      ( tdsrrc_csp_int_data.prev_network_select_mode == SYS_NETWORK_SELECTION_MODE_NONE))
  {
    /* Set the CSP substate to TDSRRCCSP_CELL_SEL_WT_STATE_CHANGE, where
     * on getting the STATE_CHANGE_IND (to IDLE), CSP will send
     * NO SERVICE to MM.
     */
    next_substate = TDSRRCCSP_CELL_SEL_WT_STATE_CHANGE;
    return next_substate;
  }
#endif

  if(((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
#ifdef FEATURE_WRLF_SYSTEM_SEL
       || (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
      ) && (!tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd))
  {
    tdsrrctmr_stop_timer(TDSRRCTMR_CONN_MODE_OOS_TIMER);
    tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd = TRUE;
  }

  if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
      && (!tdsrrc_csp_int_data.oos_initial_no_deep_sleep_tmr_expd))
  {
    tdsrrctmr_stop_timer(TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER);
    tdsrrc_csp_int_data.oos_initial_no_deep_sleep_tmr_expd = TRUE;
  }


  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
  {
    /*Call the NAS API*/
#ifdef FEATURE_DUAL_SIM
     mm_registration_in_progress = mm_per_subs_rrc_registration_in_progress(tdsrrc_get_as_id());
#else
     mm_registration_in_progress = mm_rrc_registration_in_progress();
#endif
    if(mm_registration_in_progress)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MM Reg is in progress,RRC to send NO_SERVICE to NAS");
#ifdef FEATURE_OOSC_USER_ACTION
#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
      tdsrrccsp_send_svc_status_to_mm(SYS_SRV_STATUS_NONE, SYS_RAT_TDS_RADIO_ACCESS, 0);
#endif
#endif /*FEATURE_OOSC_USER_ACTION*/
      tdsrrcccm_update_with_cell_loss();

      /* Set the CSP substate to TDSRRCCSP_CELL_SEL_WT_STATE_CHANGE, where
       * on getting the STATE_CHANGE_IND (to IDLE), CSP will send
       * NO SERVICE to MM.
       */
      next_substate = tdsrrccsp_rel_chls_and_go_to_idle();;
      return next_substate;
    }
  }

  if((tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
#ifdef FEATURE_WRLF_SYSTEM_SEL
    && (!TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
#endif
    )
  {
    tdsrrc_csp_int_data.prev_network_select_mode = SYS_NETWORK_SELECTION_MODE_NONE;
  }

#ifdef FEATURE_WRLF_SYSTEM_SEL
  /* Reset the ue_in_conn_oos flag if UE finds a suitable TDS cell during OOS search. */
  if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF: Resetting ue_in_conn_oos flag");
    tdsrrc_csp_int_data.ue_in_conn_oos = FALSE;
  }
#endif

  /* Saving curr_scan before starting leaving connected mode scan */
  tdsrrc_csp_int_data.prev_curr_scan = tdsrrc_csp_int_data.curr_scan;
  /* Set current scan to Leaving Connected Mode Scan.*/   
  tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_LEAVING_CONN_MODE_SCAN;

  /* If RCR with a frequency, set "do_lfs_scan" as FALSE, so that we will try dest frequency first*/
  if (tdsrrc_csp_int_data.dest_freq_present == TRUE)
  {
    tdsrrc_csp_int_data.do_lfs_scan = FALSE;
  }
  else
  {
    tdsrrc_csp_int_data.do_lfs_scan = TRUE;
  }


  /*If aset_cpid_code is already set, set it to the latest value*/
  /* No need to do this in TDD */
  /* if(tdsrrc_csp_int_data.aset_cpid_code < TDSRRC_CSP_INVALID_CPID)
     {
       tdsrrccsp_get_aset_cpid_pn();
     }
   */


  /* First call a CCM function to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();

  /* clear LFS freq list in case there is a partial list left by previous proc, e.g., BPLMN suspended due to conn setup */
  tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq = 0;

  if(tdsrrccsp_start_cell_selection
      (
        &tdsrrc_csp_int_data.curr_req_plmn,
        &tdsrrc_csp_int_data.curr_type_acq, 
        &tdsrrc_csp_int_data.curr_acq_entry,
        &type_freq_scan
      )   == FAILURE
    )
  {
    return (tdsrrccsp_rel_chls_and_go_to_idle());
  }

  /* Another freq found */
  if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
  {
    /* This is unacceptable since a SUCCESS was returned */
    ERR_FATAL("NO_SCAN returned",0,0,0);
  }
  else if(type_freq_scan == TDSRRC_CSP_LAST_ASET_SCAN)
  {
    /* Send TDSL1_CPHY_ACQ_REQ to L1 and wait for the CNF */
    if(tdsrrccsp_send_l1_cmd(TDSRRC_PROCEDURE_CSP, TDSL1_CPHY_ACQ_REQ, &tdsrrc_csp_int_data.curr_acq_entry) == SUCCESS)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"ACQ_REQ for the best cell in active set. Freq %d, SCR = %d, PN = %d",
                                                    tdsrrc_csp_int_data.curr_acq_entry.freq,
                                                    tdsrrc_csp_int_data.curr_acq_entry.cpid,
                                                    tdsrrc_csp_int_data.pn_pos);

      /* Start waiting for Acquisition Confirmation */
      next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
    }
    else
    {
      ERR_FATAL("RRC Unable to get cmd buf for L1 cmd 0x%x",TDSL1_CPHY_ACQ_REQ,0,0);
    }
  }
  else
  {
    tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);

    if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
      || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
     )
    {
      /* Start waiting for Acquisition Confirmation */
      next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
    }
    else
    {
      /* Start waiting for Frequency scan Confirmation */
      next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
    }
  }

  return(next_substate);
}

/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_for_plmn_list

DESCRIPTION       This function starts scanning for all the available PLMNs
                  Based on the returned frequency scan type, a CSP substate
                  is returned.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_for_plmn_list(void)
{
  tdsrrc_csp_freq_scan_e_type  type_freq_scan;
  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;

  /* Inter Frequency Redirection Scan was aborted. Clean up its state */
  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN)
  {
    tdsrrccsp_cleanup_inter_freq_redirection_state();
  }
  
#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY  
  /*Stop the timer in case background search is converted to foreground search*/
  memset(&tdsrrc_csp_int_data.bplmn_search_timer_value,0,sizeof(tds_manual_search_timer_value_type));
  tdsrrc_csp_int_data.send_partial_results = FALSE;
  tdsrrctmr_stop_timer(TDSRRCTMR_BPLMN_PARTIAL_REPORT_TIMER);
#endif

  /* Set current scan to "PLMN List Scan".*/   
  tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_PLMN_LIST_SCAN;

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
  if(tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
  {
    if(tdsrrc_csp_int_data.rrc_bst_band_pref_incl == TRUE)
    {
      /* GSM needs this for manual */
      tdsrr_set_band_pref_internal(tdsrrc_csp_int_data.band_pref_gw);
		
      /* Update GSM with BST band preference */
      tdsrr_set_bst_band_pref_internal(tdsrrc_csp_int_data.bplmn_band_pref_gw);
    }
    else
    {
      tdsrr_set_band_pref_internal(tdsrrc_csp_int_data.band_pref_gw);
      /* Update GSM with BST band preference */
      tdsrr_set_bst_band_pref_internal(0);
    }
  }
  else
#endif
  {
    /* Update GSM with UE's band preference */
    tdsrr_set_band_pref_internal(tdsrrc_csp_int_data.band_pref_gw);
  }
  
  /* Reset the next_band_mask appropriately */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the next_band_mask");
  tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_int_data.tds_band_mask;
  /* Record next_band_mask for deep search*/
  tdsrrc_csp_curr_select_data.next_band_mask_for_deep = tdsrrc_csp_curr_select_data.next_band_mask;

  tdsrrc_csp_curr_select_data.first_band_to_be_scanned = TDSRRC_CSP_BAND_MAX;

    if((tdsrrc_csp_curr_select_data.first_band_to_be_scanned =
        tdsrrccsp_get_first_band_to_be_scanned()) == TDSRRC_CSP_BAND_MAX)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"UE doesn't support a valid TD-SCDMA band");
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Defaulting to TDD Band A");
      tdsrrc_csp_curr_select_data.first_band_to_be_scanned = RRC_CSP_TDS_A_BAND;
      tdsrrc_csp_curr_select_data.current_band =
        tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
    }
    else
    {
      tdsrrc_csp_curr_select_data.current_band =
        tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"1st band to be scanned = 0x%x",tdsrrc_csp_curr_select_data.first_band_to_be_scanned);
    }

    /* Reset the next_band_mask */
    tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_int_data.tds_band_mask;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"next_band_mask reset to 0x%x",tdsrrc_csp_curr_select_data.next_band_mask);
    /* Record next_band_mask for deep search*/
    tdsrrc_csp_curr_select_data.next_band_mask_for_deep = tdsrrc_csp_curr_select_data.next_band_mask;
   


  tdsrrc_csp_curr_select_data.num_available_freq = 0;

  /* Now set the requested PLMN in Internal data to "Any PLMN".
   * This implies that the actual PLMN value is not valid.
   */
  tdsrrc_csp_int_data.curr_req_plmn.plmn_type = RRC_GSM_MAP_ANY_PLMN_REQ;

  /* Initialize RCE just in case it was waiting for UE to camp on,
   * e.g., TDSRRC_EST_REQ. Abort is sent to MM by this function, if needed.
   */
  tdsrrcrce_force_initialize(TRUE);

  /* Initialize variables associated with TD2G BPLMN search */
  tdsrrccsp_init_td2g_plmn_srch_int_data();


#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  tdsrrc_csp_int_data.detected_frequency_list.num_freqs = 0;
#endif

  /* First call a CCM function to indicate Cell Loss - no new cell */
  /* NOTE: The functional I/F has to be used very carefully. */
  tdsrrcccm_update_with_cell_loss();

  /* Release all channels and put lower layers to Idle before sending another 
   * Acquisition Req or a Frequency Scan Req.
   */
  if(tdsrrccsp_send_chan_release() == FAILURE)
  {
    ERR_FATAL("Failure sending CHAN_CONFIG_REQ",0,0,0);
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"TD2TD: dual_mode_enabled = %d,tdscdma_mode_enabled = %d in PLMN_LIST_REQ",tdsrrc_csp_int_data.bplmn_dual_mode_enabled,tdsrrc_csp_int_data.bplmn_tdscdma_mode_enabled);

#ifndef FEATURE_RAT_PRIORITY_LIST /*Already generate it if FEATURE_RAT_PRIORITY_LIST is enabled*/
  tdsrrccsp_generate_rat_order_for_manual_plmn_srch();
#endif  
  /*if plmn list has no supported rat, send mm plmn list confirm and return directly without any handling*/
  if( 0 == tdsrrc_csp_int_data.plmn_srch_order.num_rats)
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC return plmn list cnf failed directly because num_rats = 0");
     tdsrrccsp_send_mm_plmn_list_cnf_cmd(FALSE);
     return next_substate;
  }

  tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx = 0;

  if(tdsrrc_csp_int_data.plmn_srch_order.rat[0] != SYS_RAT_TDS_RADIO_ACCESS)
  {
    /* No need to send IDLE_REQ for TDD because channel release was called before. */
#ifdef FEATURE_TDSCDMA_TO_LTE
    if(tdsrrc_csp_int_data.plmn_srch_order.rat[0] == SYS_RAT_GSM_RADIO_ACCESS)
    {
      tdsrrc_csp_int_data.bplmn_interrat_type = TDSRRC_CSP_INTERRAT_GSM;
    }
    else if (tdsrrc_csp_int_data.plmn_srch_order.rat[0] == SYS_RAT_LTE_RADIO_ACCESS)
    {
      tdsrrc_csp_int_data.bplmn_interrat_type = TDSRRC_CSP_INTERRAT_LTE;
    }
#endif
    return(tdsrrccsp_resume_td2g_bplmn_srch());
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2G OOS: Do Manual PLMN Srch in fgnd on TDS first");

    if(tdsrrccsp_start_cell_selection
        (
          &tdsrrc_csp_int_data.curr_req_plmn,
          &tdsrrc_csp_int_data.curr_type_acq, 
          &tdsrrc_csp_int_data.curr_acq_entry,
          &type_freq_scan
        )   == FAILURE
      )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq by Stored Initial cell selection");
      return (tdsrrccsp_rel_chls_and_go_to_idle());
    }

    /* Another freq found */
    if(type_freq_scan == TDSRRC_CSP_NO_SCAN)
    {
      /* This is unacceptable since a SUCCESS was returned */
      ERR_FATAL("NO_SCAN returned",0,0,0);
    }

    tdsrrcwrm_start_lock_wait_timer(TDSRRCTMR_DEFAULT_SEARCH_TIME_IN_MS,TRUE);
    next_substate = tdsrrccsp_wait_for_wrm_lock(&type_freq_scan);
    if (type_freq_scan == TDSRRC_CSP_NO_SCAN)
    {
      return next_substate;
    }
 
    tdsrrccsp_send_next_freq_to_l1(&tdsrrc_csp_int_data.curr_acq_entry, type_freq_scan);

    if ((type_freq_scan == TDSRRC_CSP_ACQ_SCAN)
      || (type_freq_scan == TDSRRC_CSP_LIST_SCAN)
     )
    {
      /* Start waiting for Acquisition Confirmation */
      next_substate = TDSRRCCSP_CELL_SEL_WT_ACQ;
    }
    else
    {
      /* Start waiting for Frequency scan Confirmation */
      next_substate = TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF;
    }                     
    return(next_substate);
  }

} /* tdsrrccsp_start_sel_for_plmn_list */


/*===========================================================================

FUNCTION          tdsrrccsp_update_plmn_list

DESCRIPTION       This function reads the SIBs for a cell and checks if it is
                  barred or in the forbidden lai list. If not, it will try to
                  add the PLMN to the PLMN list to be returned to MM in the
                  SERVICE_CNF.
                  
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_update_plmn_list(rrc_plmn_identity_type plmn_id, tdsrrc_int_cell_id_type cell_id)
{
  /* Temporary storage for LAI status */
  tdsrrc_csp_lai_status_e_type  lai_status;

  /* This stores the LAI received in SIB1 */
  rrc_lai_type  lai;

  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_common_info;

  /* Pointer to SIB1 */
  tdsrrc_SysInfoType1 *sib1_ptr = NULL;

  unsigned char  *tdsdata_ptr;
  uint32         i,idx=0;
  uint32         num_plmns;

  /* Temporary storage for Cell Barred Status */
  tdsrrc_csp_cell_bar_e_type  cell_bar_status;

  /* Pointer & counter to loop through the CN System Information List */
  tdsrrc_CN_DomainSysInfoList * cn_ptr = NULL;
  uint8 cn_cnt                    = 0;
#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
  tdsl1_ui_display_info_struct_type l1_data_for_ui_display;
#else
  tdsl1_rssi_info_struct_type l1_rssi;
#endif
  sys_signal_strength_type signal_strength;
  boolean plmn_found = FALSE;
  boolean get_sib1_ptr = TRUE;


  uint32 shared_plmn_idx = 0;
  boolean is_current_cell_nw_shared = tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing;
  boolean found_shared_plmn = FALSE;

  rrc_plmn_identity_type common_plmn_id;
  if(tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing)
  {
    common_plmn_id = tdsrrc_csp_int_data.curr_nw_db_entry.common_plmn_id;
  }
  else
  {
    common_plmn_id = plmn_id;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sib_idx PLMN: MCC %d-MNC %d",TDSRRC_CSP_GET_INT_MCC(common_plmn_id),TDSRRC_CSP_GET_INT_MNC(common_plmn_id));
#ifndef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
  tdsl1_get_data_for_ui_display(&l1_data_for_ui_display);
  /* make it a positive value to maintain backward compatability with the code */
  signal_strength = -(l1_data_for_ui_display.rscp_value);
#else
  tdsl1_get_rssi(&l1_rssi);
  /* Shoud verify l1_rssi.is_valid_rssi first.
   * Because it should not be at ACQ or FS state, 
   * rssi_value should be valid.
   */
  signal_strength = l1_rssi.rssi_value;
#endif /* FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY */
#else
#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
  tdsrrcff_l1_get_data_for_ui_display(&l1_data_for_ui_display);
  /* make it a positive value to maintain backward compatability with the code */
  signal_strength = -(l1_data_for_ui_display.rscp_value);
#else
  tdsrrcff_l1_get_rssi(&l1_rssi);
  signal_strength = l1_rssi.rssi_value;
#endif /* FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY */
#endif /* FEATURE_TDSCDMA_FREE_FLOATING_TASK */


  /* This check has been added for cr109422. During foreground
   * service search if a cell is found on which UE can camp on
   * then set the var plmn_fnd_during_fgnd_svc_srch to TRUE.
   * This var has been added to inform NAS (using var ue_oos 
   * in RRC_SERVICE_CNF) that service is available. NAS looks
   * at this var to determine if limited service request should
   * be sent to RRC or not. Notice that with UOOS, SD always sends
   * Automatic Service Request and it sends Limited Service Request
   * only when NAS informs SD that Limited Service is available
   */
  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)
  {
    {
      tdsrrc_csp_int_data.continue_cell_selection_per_s = FALSE;
      if(!tdsrrccsp_is_s_criteria_met(common_plmn_id,cell_id))
      {
        tdsrrc_csp_int_data.continue_cell_selection_per_s = TRUE;
        /*S-Criteria failure, neither report the PLMN nor set the flag plmn_fnd_during_fgnd_svc_srch to TRUE*/
        TDSRRC_MSG0(MSG_LEGACY_HIGH, "S criteria failed, do not add PLMN to available list");
        return;
      }
      tdsrrc_csp_int_data.plmn_fnd_during_fgnd_svc_srch = TRUE;
    }
  }

#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
  if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) ||
      ((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN) && 
       (tdsrrc_csp_int_data.prev_curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)))
  {
    if((!tdsrrc_csp_int_data.oosc_plmn_found) && (tdsrrccsp_is_s_criteria_met(plmn_id,cell_id)))
    {
      tdsrrc_csp_int_data.oosc_plmn_found = TRUE;
      tdsrrc_csp_int_data.oosc_l1_rssi    = (uint8)signal_strength;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SVC_ST: Imm update UI w/ at least limited svc");
      tdsrrccsp_send_svc_status_to_mm(SYS_SRV_STATUS_LIMITED, SYS_RAT_TDS_RADIO_ACCESS,signal_strength);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SVC_ST: do NOT update UI w/ lim svc status. UI already updated");
    }
  }
#endif

  /* If scan type is not PLMN_LIST_SCAN and PLMN type is not ANY_PLMN,
   * do not add the PLMN to the list if it's the requested PLMN
   */
  if((tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN) &&
      (tdsrrc_csp_int_data.curr_req_plmn.plmn_type != RRC_GSM_MAP_ANY_PLMN_REQ))
  {
  }

  /* If scan type is not PLMN_LIST_SCAN and PLMN type is ANY_PLMN,
   * then do not add the PLMN to the available PLMN list as RRC 
   * should just camp on this PLMN if it is "acceptable" and there 
   * is no need to report it to NAS.
   */
  else if((tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN) &&
          (tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Any PLMN Srch. Don't add PLMN to available plmn list");
    return;
  }


  /* If scan type is OOS_Scan then the available plmn list should not be populated.
   * This is because during OOS search, RRC will camp on the PLMN autonomously
   * and send service indication to NAS with service available instead of
   * reporting the PLMNs to NAS and triggering PLMN selection via NAS. For details 
   * please see the memo on the above feature
   */
  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"OOS: Do NOT populate avail_plmn_list for OOS_Scan");
    return;
  }

#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN
  if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN)
  {
    get_sib1_ptr = FALSE;
  }
#endif

  if(get_sib1_ptr == TRUE)
  {
    /* Now generate status mask for this cell */
    /* Fill up the NAS system info here */
    /* Let us first read SIB1 - we should have SIB1 for sure */
    if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN)
       || (tdsrrccsp_is_dummy_plmn_search(tdsrrc_csp_int_data.curr_req_plmn.plmn_id)))
    {
      sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrcsib_bplmn_return_sib_for_cell(
                                             tdsrrc_SIB1);
    }
    else
    {
      
      sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrc_sibdb_return_sib_for_cell(
                                              tdsrrc_SIB1,
                                              common_plmn_id, 
                                              cell_id);
    }

    if( sib1_ptr == NULL )
    {
      /* SIB3 for this PLMN, CEll ID combination not present */
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB1 for MCC %d, MNC %d, Cell ID %d not present",
                        TDSRRC_CSP_GET_INT_MCC(common_plmn_id), 
                        TDSRRC_CSP_GET_INT_MNC(common_plmn_id),
                        cell_id);
  
      return;
    } /* SIB1 not present */
  
    /* The SIB1 is present */
    nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;
  
    /* Check if the RRC MM interface structure array can hold this info */
    if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for Common NAS Sys Info (%d bytes)",sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
      return;
    }
  
    /* Fill in the Common NAS system info now */
    tdsdata_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
    for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
    {
      nas_common_info.value[i] =  *tdsdata_ptr;
      tdsdata_ptr ++;
    }
  
    /* Get the LAI status from local utility */
    /* TBD: Call MM utility to initialize lai first. */
    if(mm_extract_lac_from_sib_info(&lai, &nas_common_info) == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not extract LAI from NAS Common sys info");
      return;
    }
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RSSI Signal Strength is %d",signal_strength);

  if((tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN) &&
      (tdsrrc_csp_int_data.curr_req_plmn.plmn_type != RRC_GSM_MAP_ANY_PLMN_REQ)
       && is_current_cell_nw_shared)
  {
    for(i=0; i< tdsrrc_csp_int_data.curr_nw_db_entry.num_of_shared_plmns; i++)
    {
      plmn_id = tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[i].plmn_id;
      if( tdsrrc_bcch_and_nas_plmns_match(
            plmn_id, tdsrrc_csp_int_data.curr_req_plmn.plmn_id))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"requested PLMN is in Shared list");
        found_shared_plmn = TRUE;
        break;
      }
    }
  }
  if(!found_shared_plmn)
  {
    do
    {
      if((is_current_cell_nw_shared) && (shared_plmn_idx < tdsrrc_csp_int_data.curr_nw_db_entry.num_of_shared_plmns))
      {
        plmn_id = tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx++].plmn_id;
      }

      /* If scan type is not PLMN_LIST_SCAN and PLMN type is not ANY_PLMN,
       * do not add the PLMN to the list if it's the requested PLMN
       */
      if((tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN) &&
          (tdsrrc_csp_int_data.curr_req_plmn.plmn_type != RRC_GSM_MAP_ANY_PLMN_REQ))
      {
        if( tdsrrc_bcch_and_nas_plmns_match(
              plmn_id, tdsrrc_csp_int_data.curr_req_plmn.plmn_id))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN is the requested PLMN");
          continue;
        }
      }


      /* If the we already have reached the MAX_PLMNS no need to go any further */
      if(tdsrrc_csp_int_data.plmn_list.num_plmns < RRC_MAX_PLMNS)
      {
        /* Initialize num_plmns to the number of PLMNs in the CSP internal data */
        num_plmns = tdsrrc_csp_int_data.plmn_list.num_plmns;

#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN

        if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TD2TD: SIB1/3 not read. Setting defaults. Scan = %d",tdsrrc_csp_int_data.curr_scan);
          /* Check if the PLMN is already present in the PLMN list
           * If the PLMN is not present in the PLMN list, add it
           */

          for(i=0; (i< num_plmns) && !plmn_found; i++)
          {
            /* Check if the PLMN is already in the list */
            if( TDSRRC_CSP_SAME_MCC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn) &&
                TDSRRC_CSP_SAME_MNC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn)
              )
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"PLMN %d-%d already in the PLMN list",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
              plmn_found = TRUE;
            }
          }
    
          if(plmn_found)
          {
            plmn_found = FALSE;
            continue;
          }
    
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                   = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
    
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].plmn = plmn_id;
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].rat 
                                                           = SYS_RAT_TDS_RADIO_ACCESS;
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
          if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
            tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality= SYS_SIGNAL_QUALITY_HIGH;
          }
          else
          {
            tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality 
                                                           = SYS_SIGNAL_QUALITY_LOW;
          }

          tdsrrc_csp_int_data.plmn_list.num_plmns++;

          continue;

        }
#endif

        /* Since MM doesn't know the PLMN from NAS system info, fill in the PLMN now */
        lai.plmn = plmn_id;
      
        /* If it's a Manual PLMN selection scan don't check for
         * forbidden LAIs
         */
        if(
             (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN)
          && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
          )
        {
          if(tdsrrccsp_find_forbid_lai(&lai, &lai_status) 
             == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure finding LAI status");
            return;
          }
    
          /* Check if the cell belongs to a forbidden LAI */
          if(lai_status != TDSRRC_CSP_LAI_OK)
          {
            TDSRRC_MSG0(MSG_LEGACY_MED,"Cell belongs to a forbidden LAI");
            continue;
          }
        }
      
        /* Generate Cell barred status based on SIB info for this cell */
        if(!tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing)
        {
          common_plmn_id = plmn_id;
        }

        if(tdsrrccsp_find_cell_bar_status(plmn_id, 
                                        cell_id, 
                                        &cell_bar_status
                                        ,common_plmn_id
                                        ) 
            == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure to fill Cell Barred Status");
          return;
        }
      
        if(cell_bar_status != TDSRRC_CSP_CELL_OK)
        {
          TDSRRC_MSG0(MSG_LEGACY_MED,"Cell is barred");
          continue;
        }
      
        /* Check if the PLMN is already present in the PLMN list
         * If the PLMN is not present in the PLMN list, add it
         */
      
        for(i=0; (i< num_plmns) && !plmn_found; i++)
        {
          /* Check if the PLMN is already in the list */
          if( TDSRRC_CSP_SAME_MCC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn) &&
              TDSRRC_CSP_SAME_MNC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn)
            )
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"PLMN %d-%d already in the PLMN list",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
            plmn_found = TRUE;
          }
        }
    
        if(plmn_found)
        {
          plmn_found = FALSE;
          continue;
        }
    
        ASSERT(sib1_ptr != NULL);
    
        /* Get the CN Domain identities from the CN Domain Sys Info List */
        cn_ptr = &sib1_ptr->cn_DomainSysInfoList;
    
        cn_cnt = 0;
        idx=0;
        while((cn_ptr->n > idx) && (cn_cnt < UE_MAX_CN_DOMAINS))
        {
          switch(cn_ptr->elem[idx].cn_DomainIdentity)
          {
            case tdsrrc_CN_DomainIdentity_cs_domain:
              if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                   == SYS_SRV_CAPABILITY_PS_SERVICE_ONLY)
              {
                tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                         = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
              }
              else
              {
                tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                         = SYS_SRV_CAPABILITY_CS_SERVICE_ONLY;
              }
              break;
      
            case tdsrrc_CN_DomainIdentity_ps_domain:
              if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                   == SYS_SRV_CAPABILITY_CS_SERVICE_ONLY)
              {
                tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                         = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
              }
              else
              {
                tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                         = SYS_SRV_CAPABILITY_PS_SERVICE_ONLY;
              }
              break;
            
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid CN 0x%x for Service Capability",cn_ptr->elem[idx].cn_DomainIdentity);
              break;
          } /* end CN Identity switch */
      
          /* If both CS and PS services are supported break out of the loop */
          if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability 
               == SYS_SRV_CAPABILITY_CS_PS_SERVICE)
          {
            break;
          }
          /* Next CN Info */
          idx++;
          cn_cnt++;
        } /* end while Core Network Info */
      
        /* If there was no CN domain information set the service capability to SYS_SRV_CAPABILITY_NONE */
        switch(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability)
        {
          case SYS_SRV_CAPABILITY_PS_SERVICE_ONLY:
          case SYS_SRV_CAPABILITY_CS_SERVICE_ONLY:
          case SYS_SRV_CAPABILITY_CS_PS_SERVICE:
            break;
          default:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"No service capability for the PLMN");
            tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                                         = SYS_SRV_CAPABILITY_NONE;
        }
      
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].plmn = plmn_id;
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].rat 
                                                         = SYS_RAT_TDS_RADIO_ACCESS;
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
        if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality = SYS_SIGNAL_QUALITY_HIGH;
        }
        else
        {
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality 
                                                         = SYS_SIGNAL_QUALITY_LOW;
        }
    
        tdsrrc_csp_int_data.plmn_list.num_plmns++;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reached the max number of PLMNs");
        return;
      }
    }while((is_current_cell_nw_shared) && (shared_plmn_idx < tdsrrc_csp_int_data.curr_nw_db_entry.num_of_shared_plmns));
  }
  return;

} /* tdsrrccsp_update_plmn_list */


/*===========================================================================

FUNCTION          tdsrrccsp_update_background_plmn_list

DESCRIPTION       This function reads the SIBs for a cell and checks if it is
                  barred or in the forbidden lai list. If not, it will try to
                  add it to the background PLMN list to be returned to RR in the
                  TDSRRC_INTERRAT_PLMN_SRCH_CNF or TDSRRC_INTERRAT_PLMN_SRCH_ABORT_CNF.
                  
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_update_background_plmn_list(rrc_plmn_identity_type plmn_id, tdsrrc_int_cell_id_type cell_id)
{
  /* Temporary storage for LAI status */
  tdsrrc_csp_lai_status_e_type  lai_status;

  /* This stores the LAI received in SIB1 */
  rrc_lai_type  lai;

  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_common_info;

  rrc_plmn_identity_type nas_to_rrc_plmn_id;

  /* Pointer to SIB1 */
  tdsrrc_SysInfoType1 *sib1_ptr = NULL;

  unsigned char  *tdsdata_ptr;
  uint32         i,idx=0;
  uint32         num_plmns;

  /* Temporary storage for Cell Barred Status */
  tdsrrc_csp_cell_bar_e_type  cell_bar_status;

  /* Pointer & counter to loop through the CN System Information List */
  tdsrrc_CN_DomainSysInfoList* cn_ptr = NULL;
  uint8 cn_cnt                    = 0;

#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
  tdsl1_ui_display_info_struct_type l1_data_for_ui_display;
#else
  tdsl1_rssi_info_struct_type l1_rssi;
#endif

  sys_signal_strength_type signal_strength;
  boolean plmn_found = FALSE;
  boolean get_sib1_ptr = TRUE;

  uint32 shared_plmn_idx = 0;
  boolean is_current_cell_nw_shared = tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing;

#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN
  if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
  {
    get_sib1_ptr = FALSE;
  }
#endif

  if(get_sib1_ptr == TRUE)
  {
    /* Now generate status mask for this cell */
    /* Fill up the NAS system info here */
    /* Let us first read SIB1 - we should have SIB1 for sure */
    sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrcsib_bplmn_return_sib_for_cell(tdsrrc_SIB1);
    if( sib1_ptr == NULL )
    {
      // SIB3 for this PLMN, CEll ID combination not present 
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB1 for MCC %d, MNC %d, Cell ID %d not present",
                        TDSRRC_CSP_GET_INT_MCC(plmn_id), 
                        TDSRRC_CSP_GET_INT_MNC(plmn_id),
                        cell_id);
      return;
    } // SIB1 not present 

    /* The SIB1 is present */
    nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;

    /* Check if the RRC MM interface structure array can hold this info */
    if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for Common NAS Sys Info (%d bytes)",sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
      return;
    }

    /* Fill in the Common NAS system info now */
    tdsdata_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
    for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
    {
      nas_common_info.value[i] =  *tdsdata_ptr;
      tdsdata_ptr ++;
    }

    /* Get the LAI status from local utility */
    /* TBD: Call MM utility to initialize lai first. */
    if(mm_extract_lac_from_sib_info(&lai, &nas_common_info) == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not extract LAI from NAS Common sys info");
      return;
    }
  }

#ifndef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
  memset(&l1_data_for_ui_display, 0, sizeof(tdsl1_ui_display_info_struct_type));
  tdsl1_get_data_for_ui_display(&l1_data_for_ui_display);

  /* make it a positive value to maintain backward compatability with the code */
  signal_strength = -(l1_data_for_ui_display.rscp_value);
#else
  memset(&l1_rssi, 0, sizeof(tdsl1_rssi_info_struct_type));
  tdsl1_get_rssi(&l1_rssi);
  signal_strength = l1_rssi.rssi_value;
#endif /* FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY */
#else
#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
  memset(&l1_data_for_ui_display, 0, sizeof(tdsl1_ui_display_info_struct_type));
  tdsrrcff_l1_get_data_for_ui_display(&l1_data_for_ui_display);

 /* make it a positive value to maintain backward compatability with the code */
  signal_strength = -(l1_data_for_ui_display.rscp_value);
#else
  memset(&l1_rssi, 0, sizeof(tdsl1_rssi_info_struct_type));
  tdsrrcff_l1_get_rssi(&l1_rssi);
  signal_strength = l1_rssi.rssi_value;
#endif /* FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY */
#endif /* FEATURE_TDSCDMA_FREE_FLOATING_TASK */

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RSSI Signal Strength is %d",signal_strength);

  do
  {
    if( (is_current_cell_nw_shared) && 
        (shared_plmn_idx < tdsrrc_csp_int_data.curr_nw_db_entry.num_of_shared_plmns) &&
        (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) )
    {
      plmn_id = tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx++].plmn_id;
    }

    /* If the we already have reached the MAX_PLMNS no need to go any further */
    if(tdsrrc_csp_int_data.bplmn_fnd_list_wr_index < SYS_PLMN_LIST_MAX_LENGTH)
    {
      /* Initialize num_plmns to the number of PLMNs in the CSP internal data */
      num_plmns = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;

#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN
      if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"G2TD BPLMN:ManualScan. SIB1/3 not read. Setting defaults. Scan = %d",tdsrrc_csp_int_data.curr_scan);
        /* Check if the PLMN is already present in the PLMN list
         * If the PLMN is not present in the PLMN list, add it
         */
  
        for(i = 0; (i < num_plmns) && !plmn_found; i++)
        {
          nas_to_rrc_plmn_id 
              = mmumts_convert_nas_plmn_id_to_rrc_plmn_id(
                   tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[i].plmn);
  
          /* Check if the PLMN is already in the list */
          if( TDSRRC_CSP_SAME_MCC(nas_to_rrc_plmn_id, plmn_id) &&
              TDSRRC_CSP_SAME_MNC(nas_to_rrc_plmn_id, plmn_id)
            )
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"BPLMN: PLMN %d-%d already in the PLMN list",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
            plmn_found = TRUE;
          }
        } /* end for loop */

        if(plmn_found)
        {
          plmn_found = FALSE;
          continue;
        }
  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"G2TD BPLMN:ManualScan. PLMN matched. Adding PLMN to found list");
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));

        tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn_service_capability
                              = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
        tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn 
                              = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(plmn_id);
        tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].rat 
                              = SYS_RAT_TDS_RADIO_ACCESS;
        tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].signal_strength = signal_strength;
        if(tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].signal_strength  <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
          tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].signal_quality
                                                          = SYS_SIGNAL_QUALITY_HIGH;
        }
        else
        {
          tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].signal_quality 
                                                         = SYS_SIGNAL_QUALITY_LOW;
        }

        tdsrrc_csp_int_data.bplmn_fnd_list_wr_index++;                              
        tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.length 
                              = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;
  
        continue;
  
      }
#endif
  
      /* Since MM doesn't know the PLMN from NAS system info, fill in the PLMN now */
      lai.plmn = plmn_id;
    
      /* If it's a Manual PLMN selection scan don't check for
       * forbidden LAIs
       */
  
      if(tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
      {
        if(tdsrrccsp_find_forbid_lai(&lai, &lai_status) 
           == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure finding LAI status");
          return;
        }
  
        // Check if the cell belongs to a forbidden LAI 
        if(lai_status != TDSRRC_CSP_LAI_OK)
        {
          TDSRRC_MSG0(MSG_LEGACY_MED,"Cell belongs to a forbidden LAI");
          return;
        }
      }

      /* If it's a G2TD BPLMN selection (Automatic or Manual), don't check for
       * cell barred status because SIB3 is skipped.
       */
      if((tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_MANUAL_SCAN) &&
         (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN))
      {
        /* Generate Cell barred status based on SIB info for this cell */
        if(tdsrrccsp_find_cell_bar_status(plmn_id, 
                                        cell_id, 
                                        &cell_bar_status
                                        ,plmn_id
                                        ) 
            == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure to fill Cell Barred Status");
          return;
        }
    
        if(cell_bar_status != TDSRRC_CSP_CELL_OK)
        {
           TDSRRC_MSG0(MSG_LEGACY_MED,"Cell is barred");
          continue;
        }
      }
    
      /* Check if the PLMN is already present in the PLMN list
       * If the PLMN is not present in the PLMN list, add it
       */
    
      for(i = 0; (i < num_plmns) && !plmn_found; i++)
      {
        nas_to_rrc_plmn_id 
            = mmumts_convert_nas_plmn_id_to_rrc_plmn_id(
                 tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[i].plmn);
  
        /* Check if the PLMN is already in the list */
        if( TDSRRC_CSP_SAME_MCC(nas_to_rrc_plmn_id, plmn_id) &&
            TDSRRC_CSP_SAME_MNC(nas_to_rrc_plmn_id, plmn_id)
          )
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"BPLMN: PLMN %d-%d already in the PLMN list",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
          plmn_found = TRUE;
        }
      } /* end for loop */
  
      if(plmn_found)
      {
        plmn_found = FALSE;
        continue;
      }
  
      ASSERT(sib1_ptr != NULL);
  
      /* Get the CN Domain identities from the CN Domain Sys Info List */
      cn_ptr = &sib1_ptr->cn_DomainSysInfoList;
  
      cn_cnt = 0;
      idx=0;
      while((cn_ptr->n > idx) && (cn_cnt < UE_MAX_CN_DOMAINS))
      {
        switch(cn_ptr->elem[idx].cn_DomainIdentity)
        {
        case tdsrrc_CN_DomainIdentity_cs_domain:
            
            if(tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn_service_capability
                 == SYS_SRV_CAPABILITY_PS_SERVICE_ONLY)
            {
              tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn_service_capability
                                       = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
            }
            else
            {
              tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn_service_capability
                                       = SYS_SRV_CAPABILITY_CS_SERVICE_ONLY;
            }
            break;
    
          case tdsrrc_CN_DomainIdentity_ps_domain:
            if(tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn_service_capability
                 == SYS_SRV_CAPABILITY_CS_SERVICE_ONLY)
            {
              tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn_service_capability
                                       = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
            }
            else
            {
              tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn_service_capability
                                       = SYS_SRV_CAPABILITY_PS_SERVICE_ONLY;
            }
            break;
          
          default:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid CN 0x%x for Service Capability",cn_ptr->elem[idx].cn_DomainIdentity);
            break;
        } /* end CN Identity switch */
    
        /* If both CS and PS services are supported break out of the loop */
        if(tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn_service_capability
             == SYS_SRV_CAPABILITY_CS_PS_SERVICE)
        {
          break;
        }
        /* Next CN Info */
        
        idx++;
        cn_cnt++;
      } /* end while Core Network Info */
      
      /* If there was no CN domain information set the service capability to SYS_SRV_CAPABILITY_NONE */
      switch(tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn_service_capability)
      {
        case SYS_SRV_CAPABILITY_PS_SERVICE_ONLY:
        case SYS_SRV_CAPABILITY_CS_SERVICE_ONLY:
        case SYS_SRV_CAPABILITY_CS_PS_SERVICE:
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No service capability for the PLMN");
          tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn_service_capability
                                                       = SYS_SRV_CAPABILITY_NONE;
      }

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
  
      if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN:ManualScan. PLMN matched. Adding PLMN to found list");
        tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn 
            = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(plmn_id);
        tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].rat 
                                                       = SYS_RAT_TDS_RADIO_ACCESS;
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
        if(tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
          tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].signal_quality 
                                                         = SYS_SIGNAL_QUALITY_HIGH;
        }
        else
        {
          tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].signal_quality 
                                                         = SYS_SIGNAL_QUALITY_LOW;
        }

        tdsrrc_csp_int_data.bplmn_fnd_list_wr_index++;                              
        tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.length = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;
      }
      else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
      {
        for(i = 0; i < tdsrrc_csp_int_data.bplmn_num_req; ++i)
        {
          if(tdsrrc_bcch_and_nas_plmns_match(tdsrrc_csp_int_data.curr_acq_entry.plmn_id,
                                              tdsrrc_csp_int_data.bplmn_req_list[i].plmn_id)
             #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
             /* For background service search, the PLMNs should be reported to NAS even
              * if they do not match with those sent by NAS in the PLMN_LIST_REQ.
              * Hence the check here for the enum service_search_bgnd.
              */
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC)
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_MANUAL)
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_PRIORITY_PLMN)
#ifdef FEATURE_TDSCDMA_TO_LTE
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_IRAT_PLMN)
#endif
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_NONE)
#endif
             #endif
            )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN:AutoScan. PLMN matched. Adding PLMN to found list");
            tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].plmn 
                                                       = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(plmn_id);
            tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].rat 
                                                       = SYS_RAT_TDS_RADIO_ACCESS;
            tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
            if(tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
              tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].signal_quality 
                                                             = SYS_SIGNAL_QUALITY_HIGH;
            }
            else
            {
              tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[num_plmns].signal_quality 
                                                             = SYS_SIGNAL_QUALITY_LOW;
            }
  
            tdsrrc_csp_int_data.bplmn_fnd_list_wr_index++;                              
            tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.length 
                                                       = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;
            return;
          }
        }/* end for loop */   
      }/* end RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN */     
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"update_bplmn_list fn clld whn NOT BPLMN_AUTO/MANUAL SCAN");
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reached the max number of PLMNs");
      return;
    }

  }while( (is_current_cell_nw_shared) && 
          (shared_plmn_idx < tdsrrc_csp_int_data.curr_nw_db_entry.num_of_shared_plmns) &&
          (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) );

  return;
} /* tdsrrccsp_update_background_plmn_list */

/*===========================================================================

FUNCTION          RRCCSP_UPDATE_WTOW_BACKGROUND_PLMN_LIST

DESCRIPTION       This function reads the SIBs for a cell and checks if it is
                  barred or in the forbidden lai list. If not, it will try to
                  add it to the TD2TD background PLMN list to be returned to MM in the
                  RRC_PLMN_LIST_CNF.
                  
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_update_td2td_background_plmn_list(
  rrc_plmn_identity_type plmn_id, tdsrrc_int_cell_id_type cell_id)
{
  /* Temporary storage for LAI status */
  tdsrrc_csp_lai_status_e_type  lai_status;

  /* This stores the LAI received in SIB1 */
  rrc_lai_type  lai;

  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_common_info;

  /* Pointer to SIB1 */
  tdsrrc_SysInfoType1 *sib1_ptr = NULL;

  unsigned char  *tdsdata_ptr;
  uint32         i,idx=0;
  uint32         num_plmns;

  /* Temporary storage for Cell Barred Status */
  tdsrrc_csp_cell_bar_e_type  cell_bar_status;

  /* Pointer & counter to loop through the CN System Information List */
  tdsrrc_CN_DomainSysInfoList* cn_ptr = NULL;
  uint8 cn_cnt                    = 0;
  tdsl1_rssi_info_struct_type l1_rssi;
  sys_signal_strength_type signal_strength;
  boolean plmn_found = FALSE;
  boolean get_sib1_ptr = TRUE;

  uint32 shared_plmn_idx = 0;
  boolean is_current_cell_nw_shared = tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing;

#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN
  if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
  {
    get_sib1_ptr = FALSE;
  }
#endif

  if(get_sib1_ptr == TRUE)
  {
    /* Now generate status mask for this cell */
    /* Fill up the NAS system info here */
    /* Let us first read SIB1 - we should have SIB1 for sure */
    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
        || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
        || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
        || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
        || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN))
    {
      sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrcsib_bplmn_return_sib_for_cell(tdsrrc_SIB1);
    }

    if( sib1_ptr == NULL )
    {
      // SIB3 for this PLMN, CEll ID combination not present 
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB1 for MCC %d, MNC %d, Cell ID %d not present",
                        TDSRRC_CSP_GET_INT_MCC(plmn_id), 
                        TDSRRC_CSP_GET_INT_MNC(plmn_id),
                        cell_id);
  
      return;
    } // SIB1 not present 
  
    /* The SIB1 is present */
    nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;
  
    /* Check if the RRC MM interface structure array can hold this info */
    if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for Common NAS Sys Info (%d bytes)",sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
      return;
    }
  
    /* Fill in the Common NAS system info now */
    tdsdata_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
    for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
    {
      nas_common_info.value[i] =  *tdsdata_ptr;
      tdsdata_ptr ++;
    }
  
    /* Get the LAI status from local utility */
    /* TBD: Call MM utility to initialize lai first. */
    if(mm_extract_lac_from_sib_info(&lai, &nas_common_info) == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not extract LAI from NAS Common sys info");
      return;
    }
  }

  tdsl1_get_rssi_during_wtow_bplmn(&l1_rssi);
  signal_strength = l1_rssi.rssi_value;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RSSI Signal Strength is %d",signal_strength);

  do
  {
    if( (is_current_cell_nw_shared) &&
        (shared_plmn_idx < tdsrrc_csp_int_data.curr_nw_db_entry.num_of_shared_plmns) &&
        (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) )
    {
      plmn_id = tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx++].plmn_id;
    }

    /* If the we already have reached the MAX_PLMNS no need to go any further */
    if(tdsrrc_csp_int_data.bplmn_fnd_list_wr_index < SYS_PLMN_LIST_MAX_LENGTH)
    {
      /* Initialize num_plmns to the number of PLMNs in the CSP internal data */
      num_plmns = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;
  
#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN
      if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TD2TD: SIB1/3 not read. Setting defaults. Scan = %d",tdsrrc_csp_int_data.curr_scan);
        /* Check if the PLMN is already present in the PLMN list
         * If the PLMN is not present in the PLMN list, add it
         */
  
        for(i=0; (i< num_plmns) && !plmn_found; i++)
        {
          /* Check if the PLMN is already in the list */
          if( TDSRRC_CSP_SAME_MCC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn) &&
              TDSRRC_CSP_SAME_MNC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn)
            )
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"PLMN %d-%d already in the PLMN list",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
            plmn_found = TRUE;
          }
        }
  
        if(plmn_found)
        {
          plmn_found = FALSE;
          continue;
        }
  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN:ManualScan. PLMN matched. Adding PLMN to found list");
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
  
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                                                   = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].plmn            = plmn_id;
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].rat             = SYS_RAT_TDS_RADIO_ACCESS; 
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
        if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality 
                                                         = SYS_SIGNAL_QUALITY_HIGH;
        }
        else
        {
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality 
                                                         = SYS_SIGNAL_QUALITY_LOW;
        }
  
        tdsrrc_csp_int_data.bplmn_fnd_list_wr_index++;   
  
        continue;
  
      }
#endif
  
      /* Since MM doesn't know the PLMN from NAS system info, fill in the PLMN now */
      lai.plmn = plmn_id;
    
      /* If it's a Manual PLMN selection scan don't check for
       * forbidden LAIs
       */
  
      if(tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
      {
        if(tdsrrccsp_find_forbid_lai(&lai, &lai_status) 
           == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure finding LAI status");
          return;
        }
  
        // Check if the cell belongs to a forbidden LAI 
        if(lai_status != TDSRRC_CSP_LAI_OK)
        {
          TDSRRC_MSG0(MSG_LEGACY_MED,"Cell belongs to a forbidden LAI");
          return;
        }
      }
    
      /* Generate Cell barred status based on SIB info for this cell */
      if(tdsrrccsp_find_cell_bar_status(plmn_id, 
                                      cell_id, 
                                      &cell_bar_status
                                      ,plmn_id
                                      ) 
          == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure to fill Cell Barred Status");
        return;
      }
  
      if(cell_bar_status != TDSRRC_CSP_CELL_OK)
      {
         TDSRRC_MSG0(MSG_LEGACY_MED,"Cell is barred");
        continue;
      }
    
      /* Check if the PLMN is already present in the PLMN list
       * If the PLMN is not present in the PLMN list, add it
       */
    
      for(i = 0; (i < num_plmns) && !plmn_found; i++)
      {
        /* Check if the PLMN is already in the list */
        if( TDSRRC_CSP_SAME_MCC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn) &&
            TDSRRC_CSP_SAME_MNC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn)
          ) 
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"BPLMN: PLMN %d-%d already in the PLMN list",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
          plmn_found = TRUE;
        }
      } /* end for loop */
  
      if(plmn_found)
      {
        plmn_found = FALSE;
        continue;
      }
  
      ASSERT(sib1_ptr != NULL);
  
      /* Get the CN Domain identities from the CN Domain Sys Info List */
      cn_ptr = &sib1_ptr->cn_DomainSysInfoList;
  
      cn_cnt = 0;
      idx=0;
      while((cn_ptr->n > idx) && (cn_cnt < UE_MAX_CN_DOMAINS))
      {
        switch(cn_ptr->elem[idx].cn_DomainIdentity)
        {
        case tdsrrc_CN_DomainIdentity_cs_domain:
            
            if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                 == SYS_SRV_CAPABILITY_PS_SERVICE_ONLY)
            {
              tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                       = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
            }
            else
            {
              tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                       = SYS_SRV_CAPABILITY_CS_SERVICE_ONLY;
            }
            break;
    
          case tdsrrc_CN_DomainIdentity_ps_domain:
            if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                 == SYS_SRV_CAPABILITY_CS_SERVICE_ONLY)
            {
              tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                       = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
            }
            else
            {
              tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                       = SYS_SRV_CAPABILITY_PS_SERVICE_ONLY;
            }
            break;
          
          default:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid CN 0x%x for Service Capability",cn_ptr->elem[idx].cn_DomainIdentity);
            break;
        } /* end CN Identity switch */
    
        /* If both CS and PS services are supported break out of the loop */
        if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
             == SYS_SRV_CAPABILITY_CS_PS_SERVICE)
        {
          break;
        }
        /* Next CN Info */
        
        idx++;
        cn_cnt++;
      } /* end while Core Network Info */
      
      /* If there was no CN domain information set the service capability to SYS_SRV_CAPABILITY_NONE */
      switch(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability)
      {
        case SYS_SRV_CAPABILITY_PS_SERVICE_ONLY:
        case SYS_SRV_CAPABILITY_CS_SERVICE_ONLY:
        case SYS_SRV_CAPABILITY_CS_PS_SERVICE:
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No service capability for the PLMN");
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                                       = SYS_SRV_CAPABILITY_NONE;
      }
      
      if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN:ManualScan. PLMN matched. Adding PLMN to found list");
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].plmn = plmn_id;
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].rat = SYS_RAT_TDS_RADIO_ACCESS; 
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
        if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality = SYS_SIGNAL_QUALITY_HIGH;
        }
        else
        {
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality 
                                                         = SYS_SIGNAL_QUALITY_LOW;
        }
  
        tdsrrc_csp_int_data.bplmn_fnd_list_wr_index++;                              
      }
      else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
      {
        for(i = 0; i < tdsrrc_csp_int_data.wtow_bplmn_num_req; ++i)
        {
          if(tdsrrc_bcch_and_nas_plmns_match(tdsrrc_csp_int_data.curr_acq_entry.plmn_id,
                                              tdsrrc_csp_int_data.wtow_bplmn_req_list[i].plmn_id)
             #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
             /* For background service search, the PLMNs should be reported to NAS even
              * if they do not match with those sent by NAS in the PLMN_LIST_REQ.
              * Hence the check here for the enum service_search_bgnd.
              */
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC)
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_MANUAL)
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_NONE)
#endif
             #endif
            )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: AutoScan. PLMN matched. Adding PLMN to found list");
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
            tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].plmn = plmn_id;
            tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].rat = SYS_RAT_TDS_RADIO_ACCESS; 
            tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
            if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
              tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality = SYS_SIGNAL_QUALITY_HIGH;
            }
            else
            {
              tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality 
                                                             = SYS_SIGNAL_QUALITY_LOW;
            }
  
            tdsrrc_csp_int_data.bplmn_fnd_list_wr_index++;                              
            return;
          }
        }/* end for loop */   
      }/* end RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN */     
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: update_wtow_background_plmn_list fn called whn NOT BPLMN_AUTO/MANUAL SCAN");
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: Reached the max number of PLMNs");
      return;
    }
  }while( (is_current_cell_nw_shared) &&
          (shared_plmn_idx < tdsrrc_csp_int_data.curr_nw_db_entry.num_of_shared_plmns) &&
          (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) );
  return;
} /* tdsrrccsp_update_td2td_background_plmn_list */

/*===========================================================================

FUNCTION          RRCCSP_UPDATE_WTOW_BACKGROUND_PLMN_LIST_FOR_SIB_INCOMPLETE

DESCRIPTION       This function is called when cell SIB fail hit MAX during BPLMN
                  search. It check whether SIB1/SIB3 exist and reads the SIBs for
				  a cell and checks if it is barred or in the forbidden lai list. 
				  If not, it will try to add it to the TD2TD background PLMN list 
				  to be returned to MM in the RRC_PLMN_LIST_CNF.
                  
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_update_td2td_background_plmn_list_for_sib_incomplete(
  rrc_plmn_identity_type plmn_id, tdsrrc_int_cell_id_type cell_id)
{
  /* Temporary storage for LAI status */
  tdsrrc_csp_lai_status_e_type  lai_status;

  /* This stores the LAI received in SIB1 */
  rrc_lai_type  lai;

  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_common_info;

  /* Pointer to SIB1 */
  tdsrrc_SysInfoType1 *sib1_ptr = NULL;
  tdsrrc_SysInfoType3 *sib3_ptr = NULL;
  unsigned char  *tdsdata_ptr;
  uint32         i,idx=0;
  uint32         num_plmns;

  /* Temporary storage for Cell Barred Status */
  tdsrrc_csp_cell_bar_e_type  cell_bar_status;

  /* Pointer & counter to loop through the CN System Information List */
  tdsrrc_CN_DomainSysInfoList* cn_ptr = NULL;
  uint8 cn_cnt                    = 0;
  tdsl1_rssi_info_struct_type l1_rssi;
  sys_signal_strength_type signal_strength;
  boolean plmn_found = FALSE;
  boolean get_sib1_ptr = TRUE;

  uint32 shared_plmn_idx = 0;
  boolean is_current_cell_nw_shared = tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing;

#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN
  if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
  {
    get_sib1_ptr = FALSE;
  }
#endif

  if(get_sib1_ptr == TRUE)
  {
    /* Now generate status mask for this cell */
    /* Fill up the NAS system info here */
    /* Let us first read SIB1 - we should have SIB1 for sure */
    if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
        || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
        || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
        || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
        || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN))
    {
      sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrcsib_bplmn_return_sib_for_cell(tdsrrc_SIB1);
    }

    if( sib1_ptr != NULL )
    {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN SIB fail cell handling,SIB1 received");
  
       /* The SIB1 is present */
       nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;
  
       /* Check if the RRC MM interface structure array can hold this info */
       if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
       {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for Common NAS Sys Info (%d bytes)",sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
          return;
       }
  
       /* Fill in the Common NAS system info now */
       tdsdata_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
       for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
       {
          nas_common_info.value[i] =  *tdsdata_ptr;
          tdsdata_ptr ++;
       }
  
       /* Get the LAI status from local utility */
       /* TBD: Call MM utility to initialize lai first. */
       if(mm_extract_lac_from_sib_info(&lai, &nas_common_info) == FALSE)
       {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not extract LAI from NAS Common sys info");
          return;
       }
    }
    else
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN SIB fail cell handling, SIB1 not received,do not consider NAS info");

  }
  tdsl1_get_rssi_during_wtow_bplmn(&l1_rssi);
  signal_strength = l1_rssi.rssi_value;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RSSI Signal Strength is %d",signal_strength);

  do
  {
    if( (is_current_cell_nw_shared) &&
        (shared_plmn_idx < tdsrrc_csp_int_data.curr_nw_db_entry.num_of_shared_plmns) &&
        (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) )
    {
      plmn_id = tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[shared_plmn_idx++].plmn_id;
    }

    /* If the we already have reached the MAX_PLMNS no need to go any further */
    if(tdsrrc_csp_int_data.bplmn_fnd_list_wr_index < SYS_PLMN_LIST_MAX_LENGTH)
    {
      /* Initialize num_plmns to the number of PLMNs in the CSP internal data */
      num_plmns = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;
  
#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN
      if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TD2TD: SIB1/3 not read. Setting defaults. Scan = %d",tdsrrc_csp_int_data.curr_scan);
        /* Check if the PLMN is already present in the PLMN list
         * If the PLMN is not present in the PLMN list, add it
         */
  
        for(i=0; (i< num_plmns) && !plmn_found; i++)
        {
          /* Check if the PLMN is already in the list */
          if( TDSRRC_CSP_SAME_MCC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn) &&
              TDSRRC_CSP_SAME_MNC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn)
            )
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"PLMN %d-%d already in the PLMN list",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
            plmn_found = TRUE;
          }
        }
  
        if(plmn_found)
        {
          plmn_found = FALSE;
          continue;
        }
  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN:ManualScan. PLMN matched. Adding PLMN to found list");
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
  
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                                                   = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].plmn            = plmn_id;
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].rat             = SYS_RAT_TDS_RADIO_ACCESS; 
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
        if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality 
                                                         = SYS_SIGNAL_QUALITY_HIGH;
        }
        else
        {
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality 
                                                         = SYS_SIGNAL_QUALITY_LOW;
        }
  
        tdsrrc_csp_int_data.bplmn_fnd_list_wr_index++;   
  
        continue;
  
      }
#endif
      if( sib1_ptr != NULL )
      {
          /* Since MM doesn't know the PLMN from NAS system info, fill in the PLMN now */
          lai.plmn = plmn_id;
    
          /* If it's a Manual PLMN selection scan don't check for
                    * forbidden LAIs
                    */
  
          if(tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
          {
              if(tdsrrccsp_find_forbid_lai(&lai, &lai_status) 
                  == FAILURE)
              {
                 TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure finding LAI status");
                 return;
              }
  
        // Check if the cell belongs to a forbidden LAI 
              if(lai_status != TDSRRC_CSP_LAI_OK)
              {
                  TDSRRC_MSG0(MSG_LEGACY_MED,"Cell belongs to a forbidden LAI");
                  return;
              }
           }
      }

	  sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrcsib_bplmn_return_sib_for_cell(tdsrrc_SIB3);
      if (sib3_ptr != NULL)
      {
          /* Generate Cell barred status based on SIB info for this cell */
          if(tdsrrccsp_find_cell_bar_status(plmn_id, 
                                      cell_id, 
                                      &cell_bar_status
                                      ,plmn_id
                                      ) 
              == FAILURE)
          {
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure to fill Cell Barred Status");
             return;
          }
  
          if(cell_bar_status != TDSRRC_CSP_CELL_OK)
          {
             TDSRRC_MSG0(MSG_LEGACY_MED,"Cell is barred");
             continue;
          }
      }
      /* Check if the PLMN is already present in the PLMN list
       * If the PLMN is not present in the PLMN list, add it
       */
    
      for(i = 0; (i < num_plmns) && !plmn_found; i++)
      {
        /* Check if the PLMN is already in the list */
        if( TDSRRC_CSP_SAME_MCC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn) &&
            TDSRRC_CSP_SAME_MNC(plmn_id, tdsrrc_csp_int_data.plmn_list.plmn[i].plmn)
          ) 
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"BPLMN: PLMN %d-%d already in the PLMN list",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
          plmn_found = TRUE;
        }
      } /* end for loop */
  
      if(plmn_found)
      {
        plmn_found = FALSE;
        continue;
      }
  
   //   ASSERT(sib1_ptr != NULL);
      if (sib1_ptr != NULL)
      {
         /* Get the CN Domain identities from the CN Domain Sys Info List */
         cn_ptr = &sib1_ptr->cn_DomainSysInfoList;
  
         cn_cnt = 0;
         idx=0;
         while((cn_ptr->n > idx) && (cn_cnt < UE_MAX_CN_DOMAINS))
         {
           switch(cn_ptr->elem[idx].cn_DomainIdentity)
           {
              case tdsrrc_CN_DomainIdentity_cs_domain:
            
              if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                 == SYS_SRV_CAPABILITY_PS_SERVICE_ONLY)
              {
                 tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                       = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
              }
              else
              {
                 tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                       = SYS_SRV_CAPABILITY_CS_SERVICE_ONLY;
              }
              break;
    
              case tdsrrc_CN_DomainIdentity_ps_domain:
              if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                 == SYS_SRV_CAPABILITY_CS_SERVICE_ONLY)
              {
                 tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                       = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
              }
              else
              {
                 tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                       = SYS_SRV_CAPABILITY_PS_SERVICE_ONLY;
              }
              break;
          
              default:
                 TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid CN 0x%x for Service Capability",cn_ptr->elem[idx].cn_DomainIdentity);
              break;
           } /* end CN Identity switch */
    
           /* If both CS and PS services are supported break out of the loop */
           if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
             == SYS_SRV_CAPABILITY_CS_PS_SERVICE)
           {
              break;
           }
        /* Next CN Info */
        
           idx++;
           cn_cnt++;
        } /* end while Core Network Info */
      
        /* If there was no CN domain information set the service capability to SYS_SRV_CAPABILITY_NONE */
        switch(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability)
        {
           case SYS_SRV_CAPABILITY_PS_SERVICE_ONLY:
           case SYS_SRV_CAPABILITY_CS_SERVICE_ONLY:
           case SYS_SRV_CAPABILITY_CS_PS_SERVICE:
             break;
           default:
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"No service capability for the PLMN");
          /* Do not set service_capability as SYS_SRV_CAPABILITY_NOT_AVAILABLE or else
		   * NAS will treat the PLMN_LIST_CNF as invalid
		   */
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                                       = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
        }
        }
      else
      {  
	      /* Do not set service_capability as SYS_SRV_CAPABILITY_NOT_AVAILABLE or else
		   * NAS will treat the PLMN_LIST_CNF as invalid
		   */
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability     
                                                       = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
      }
      if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN:ManualScan. PLMN matched. Adding PLMN to found list");
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].plmn = plmn_id;
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].rat = SYS_RAT_TDS_RADIO_ACCESS; 
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
        if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality = SYS_SIGNAL_QUALITY_HIGH;
        }
        else
        {
          tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality 
                                                         = SYS_SIGNAL_QUALITY_LOW;
        }
  
        tdsrrc_csp_int_data.bplmn_fnd_list_wr_index++;                              
      }
      else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
      {
        for(i = 0; i < tdsrrc_csp_int_data.wtow_bplmn_num_req; ++i)
        {
          if(tdsrrc_bcch_and_nas_plmns_match(tdsrrc_csp_int_data.curr_acq_entry.plmn_id,
                                              tdsrrc_csp_int_data.wtow_bplmn_req_list[i].plmn_id)
             #ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
             /* For background service search, the PLMNs should be reported to NAS even
              * if they do not match with those sent by NAS in the PLMN_LIST_REQ.
              * Hence the check here for the enum service_search_bgnd.
              */
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC)
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_MANUAL)
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_PRIORITY_PLMN)
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
             || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_NONE)
#endif
             #endif
            )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: AutoScan. PLMN matched. Adding PLMN to found list");
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding PLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
            tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].plmn = plmn_id;
            tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].rat = SYS_RAT_TDS_RADIO_ACCESS; 
            tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
            if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
              tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality = SYS_SIGNAL_QUALITY_HIGH;
            }
            else
            {
              tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality 
                                                             = SYS_SIGNAL_QUALITY_LOW;
            }
  
            tdsrrc_csp_int_data.bplmn_fnd_list_wr_index++;                              
            return;
          }
        }/* end for loop */   
      }/* end RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN */     
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: update_wtow_background_plmn_list fn called whn NOT BPLMN_AUTO/MANUAL SCAN");
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: Reached the max number of PLMNs");
      return;
    }
  }while( (is_current_cell_nw_shared) &&
          (shared_plmn_idx < tdsrrc_csp_int_data.curr_nw_db_entry.num_of_shared_plmns) &&
          (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) );
  return;
} 

/*===========================================================================

FUNCTION          RRCCSP_UPDATE_WTOW_MANUAL_BPLMN_LIST_WITH_RPLMN

DESCRIPTION       This function updates the TD2TD Available PLMN list with the 
                  PLMN on which UE is camped on. This function is called only
                  when MANUAL PLMN_LIST_REQ is received from NAS and UE is 
                  in service.
                                    
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_update_td2td_manual_bplmn_list_with_rplmn(rrc_plmn_identity_type plmn_id, 
                                                     tdsrrc_int_cell_id_type cell_id)
{
  uint32         num_plmns;

#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
  tdsl1_ui_display_info_struct_type l1_data_for_ui_display;
#else
  tdsl1_rssi_info_struct_type l1_rssi;
#endif

  sys_signal_strength_type signal_strength;

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
  tdsrrc_csp_band_class_type csp_band_class = tdsrrc_get_frequency_band(tdsrrcccm_get_curr_camped_freq());

  /*Do not add current camped information if camped frequency is in not present in BPLMN band preference*/
  if (!(tdsrrc_csp_int_data.bplmn_band_mask & csp_band_class ))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Current camped frequency is not present in bplmn band preference");
    return;
  }
#endif

  /* If we already have reached the MAX_PLMNS no need to go any further */
  if(tdsrrc_csp_int_data.bplmn_fnd_list_wr_index < SYS_PLMN_LIST_MAX_LENGTH)
  {
    /* Initialize num_plmns to the number of PLMNs in the CSP internal data */
    num_plmns = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;
    

#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN

    if(num_plmns > 0)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"TD2TD ERR: num_plmns %d has a value > 0",num_plmns);
      return;
    }

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding RPLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));

    tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                                               = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
    tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].plmn            = plmn_id;
    tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].rat             = SYS_RAT_TDS_RADIO_ACCESS; 

#ifndef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
    tdsl1_get_data_for_ui_display(&l1_data_for_ui_display);
  
    /* make it a positive value to maintain backward compatability with the code */
    signal_strength = -(l1_data_for_ui_display.rscp_value);
#else
    tdsl1_get_rssi(&l1_rssi);
    signal_strength = l1_rssi.rssi_value;
#endif /* FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY */
#else
#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
    tdsrrcff_l1_get_data_for_ui_display(&l1_data_for_ui_display);
    /* make it a positive value to maintain backward compatability with the code */
    signal_strength = -(l1_data_for_ui_display.rscp_value);
#else
    tdsrrcff_l1_get_rssi(&l1_rssi);
    signal_strength = l1_rssi.rssi_value;
#endif /* FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY */
#endif /* FEATURE_TDSCDMA_FREE_FLOATING_TASK */


    
    tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RSSI Signal Strength is %d",signal_strength);
    if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
      tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality = SYS_SIGNAL_QUALITY_HIGH;
    }
    else
    {
      tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality = SYS_SIGNAL_QUALITY_LOW;
    }
    tdsrrc_csp_int_data.bplmn_fnd_list_wr_index++;   

    return;
#else
   {
     /* This stores the LAI received in SIB1 */
     rrc_lai_type  lai;
   
     /* This stores the GSM-MAP NAS system info */
     rrc_gsm_map_nas_sys_info_type   nas_common_info;
     
     /* Pointer to SIB1 */
     tdsrrc_SysInfoType1 *sib1_ptr;
     
   
     unsigned char  *tdsdata_ptr;
     uint32         i,idx=0;
   
     /* Temporary storage for Cell Barred Status */
     tdsrrc_csp_cell_bar_e_type  cell_bar_status;
   
     /* Pointer & counter to loop through the CN System Information List */
     tdsrrc_CN_DomainSysInfoList * cn_ptr = NULL;
     uint8 cn_cnt                    = 0;
       /* Now generate status mask for this cell */
       /* Fill up the NAS system info here */
       /* Let us first read SIB1 - we should have SIB1 for sure */
     sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrcsib_bplmn_return_sib_for_cell(
                                                tdsrrc_SIB1);

       
       if( sib1_ptr == NULL )
       {
         // SIB3 for this PLMN, CEll ID combination not present 
         TDSRRC_MSG3(MSG_LEGACY_ERROR,"SIB1 for MCC %d, MNC %d, Cell ID %d not present",
                           TDSRRC_CSP_GET_INT_MCC(plmn_id), 
                           TDSRRC_CSP_GET_INT_MNC(plmn_id),
                           cell_id);
     
         return;
       } // SIB1 not present 
     
       /* The SIB1 is present */
       nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;
     
       /* Check if the RRC MM interface structure array can hold this info */
       if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
       {
         TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for Common NAS Sys Info (%d bytes)",sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
         return;
       }
     
       /* Fill in the Common NAS system info now */
       tdsdata_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
       for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
       {
         nas_common_info.value[i] =  *tdsdata_ptr;
         tdsdata_ptr ++;
       }
     
       /* Get the LAI status from local utility */
       /* TBD: Call MM utility to initialize lai first. */
     
       if(mm_extract_lac_from_sib_info(&lai, &nas_common_info) == FALSE)
       {
         TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not extract LAI from NAS Common sys info");
         return;
       }
       
       
       /* Since MM doesn't know the PLMN from NAS system info, fill in the PLMN now */
       lai.plmn = plmn_id;
     
       /* Generate Cell barred status based on SIB info for this cell */
       if(tdsrrccsp_find_cell_bar_status(plmn_id, 
                                       cell_id, 
                                       &cell_bar_status
                                      ,plmn_id
                                       ) 
           == FAILURE)
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure to fill Cell Barred Status");
         return;
       }
   
       if(cell_bar_status != TDSRRC_CSP_CELL_OK)
       {
          TDSRRC_MSG0(MSG_LEGACY_MED,"Cell is barred");
          return;
       }
     
       /* Get the CN Domain identities from the CN Domain Sys Info List */
       cn_ptr = &sib1_ptr->cn_DomainSysInfoList;
       idx=0;
       while((cn_ptr->n > idx) && (cn_cnt < UE_MAX_CN_DOMAINS))
       {
         switch(cn_ptr->elem[idx].cn_DomainIdentity)
         {
           case tdsrrc_CN_DomainIdentity_cs_domain:
   
             if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                  == SYS_SRV_CAPABILITY_PS_SERVICE_ONLY)
             {
               tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                        = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
             }
             else
             {
               tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                        = SYS_SRV_CAPABILITY_CS_SERVICE_ONLY;
             }
             break;
   
           case tdsrrc_CN_DomainIdentity_ps_domain:
             if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                  == SYS_SRV_CAPABILITY_CS_SERVICE_ONLY)
             {
               tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                        = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
             }
             else
             {
               tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                        = SYS_SRV_CAPABILITY_PS_SERVICE_ONLY;
             }
             break;
   
           default:
             TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid CN 0x%x for Service Capability",cn_ptr->elem[idx].cn_DomainIdentity);
             break;
         } /* end CN Identity switch */
   
         /* If both CS and PS services are supported break out of the loop */
         if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
              == SYS_SRV_CAPABILITY_CS_PS_SERVICE)
         {
           break;
         }
         /* Next CN Info */
   
         idx++;
         cn_cnt++;
       } /* end while Core Network Info */
   
       /* If there was no CN domain information set the service capability to SYS_SRV_CAPABILITY_NONE */
       switch(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability)
       {
         case SYS_SRV_CAPABILITY_PS_SERVICE_ONLY:
         case SYS_SRV_CAPABILITY_CS_SERVICE_ONLY:
         case SYS_SRV_CAPABILITY_CS_PS_SERVICE:
           break;
         default:
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"No service capability for the PLMN");
           tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].service_capability
                                                        = SYS_SRV_CAPABILITY_NONE;
       }
   
       TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding RPLMN %d-%d",TDSRRC_CSP_GET_INT_MCC(plmn_id),TDSRRC_CSP_GET_INT_MNC(plmn_id));
       tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].plmn = plmn_id;
       tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].rat  = SYS_RAT_TDS_RADIO_ACCESS; 

#ifndef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
       tdsl1_get_data_for_ui_display(&l1_data_for_ui_display);

       /* make it a positive value to maintain backward compatability with the code */
       signal_strength = -(l1_data_for_ui_display.rscp_value);
#else
       tdsl1_get_rssi(&l1_rssi);
       signal_strength = l1_rssi.rssi_value;
#endif /* FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY */
#else
#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
       tdsrrcff_l1_get_data_for_ui_display(&l1_data_for_ui_display);
       /* make it a positive value to maintain backward compatability with the code */
       signal_strength = -(l1_data_for_ui_display.rscp_value);
#else
       tdsrrcff_l1_get_rssi(&l1_rssi);
       signal_strength = l1_rssi.rssi_value;
#endif /* FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY */
#endif /* FEATURE_TDSCDMA_FREE_FLOATING_TASK */

 
       tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength = signal_strength;
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"RSSI Signal Strength is %d",signal_strength);
       if(tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_strength <= TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"signal strength %d >= -%d dbm is considered as SYS_SIGNAL_QUALITY_HIGH",signal_strength,TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM);
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality = SYS_SIGNAL_QUALITY_HIGH;
      }
      else
      {
        tdsrrc_csp_int_data.plmn_list.plmn[num_plmns].signal_quality = SYS_SIGNAL_QUALITY_LOW;
      }
       tdsrrc_csp_int_data.bplmn_fnd_list_wr_index++;    
     }                                        
#endif 
  }  
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reached the max number of PLMNs");
  } 
  return;
} /* tdsrrccsp_update_td2td_manual_bplmn_list_with_rplmn */

/*===========================================================================

FUNCTION          tdsrrccsp_continue_bplmn_in_frgrnd

DESCRIPTION       This function is used to continue ongoing MPLMN Search in background 
                  to foreground.
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP Substate
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_continue_bplmn_in_frgrnd(void)
{

  tdsrrc_csp_substate_e_type   next_substate = TDSRRCCSP_NO_CELL_SELECTED;
  uint8 trans_id;
  sys_service_search_e_type srvc_srch_bgnd;

  /* Restore BPLMN Context*/
  if(tdsrrc_csp_int_data.bplmn_srch_vars_status == TD2TD_SEARCH_SUSPEND)
  {
    if (tdsrrc_csp_bplmn_data_saved)
    {
      /* If BPLMN data is saved restore the needy ones and reset backed up data*/
      tdsrrc_csp_bplmn_data_saved = FALSE; 
      /* Resetting the backed up data */
      tdsrrccsp_restore_saved_bplmn_data();
    }

    tdsrrc_csp_int_data.bplmn_current_band_initialized = FALSE;
    tdsrrc_csp_int_data.bplmn_sib_try_next_best_cell = FALSE;
    tdsrrc_csp_int_data.bplmn_acq_status             = FALSE;
    tdsrrc_csp_int_data.bplmn_bgnd_mib_rd_cnt  = 0;
    tdsrrc_csp_int_data.bplmn_bgnd_mib_rd_freq = 0;
    tdsrrc_csp_int_data.bplmn_suspend_cause = TDSBPLMN_SUSPEND_REASON_NONE;  
    
    if(!tdsrrc_csp_int_data.bplmn_guard_srch_tmr_expired)
    {
      tdsrrctmr_stop_timer(TDSRRCTMR_BPLMN_GUARD_SRCH_TIMER);
      tdsrrc_csp_int_data.bplmn_guard_srch_tmr_expired = TRUE;
    }

    tdsrrcrce_force_initialize(TRUE);

    tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_PLMN_LIST_SCAN;
    tdsrrc_csp_int_data.curr_req_plmn.plmn_type = RRC_GSM_MAP_ANY_PLMN_REQ;
    tdsrrc_csp_curr_select_data.acq_type = TDSRRC_CSP_ANY_PLMN;  
    tdsrrc_csp_int_data.plmn_list.num_plmns = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;

    if(!rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Continue BPLMN search on TDS in foreground");
      next_substate = tdsrrccsp_start_bplmn_srch();
    }
    /* TD2G BPLMN search is in progress */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Continue BPLMN search on G in foreground");
    /* No need to send IDLE_REQ for TDD.
     * Assume TDD L1 already in idle or sync state
     */

      next_substate = tdsrrccsp_resume_td2g_bplmn_srch();
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Restart PLMN Search in Foreground");
    tdsrrc_csp_int_data.plmn_list.num_plmns = 0;
    trans_id = tdsrrc_csp_int_data.bplmn_trans_id;
    srvc_srch_bgnd = tdsrrc_csp_int_data.service_search_bgnd;
    tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_STOP, FALSE, FALSE);
    tdsrrc_csp_int_data.bplmn_trans_id= trans_id;
    tdsrrc_csp_int_data.service_search_bgnd = srvc_srch_bgnd;
    next_substate = tdsrrccsp_start_sel_for_plmn_list();
  }
   
  return(next_substate);
  
}


/*===========================================================================

FUNCTION          tdsrrccsp_is_state_transition_triggered_by_reconfig_msg

DESCRIPTION       This function returns a TRUE if the state transition was
                  trigerred by a Reconfig message and a FALSE when it was
                  done by UE on its own (e.g. due to Radio Link Failure)                  
DEPENDENCIES
                  None
RETURN VALUE
                  Boolean
SIDE EFFECTS
                  None

===========================================================================*/
boolean  tdsrrccsp_is_state_transition_triggered_by_reconfig_msg( void )
{
  boolean         trg_frm_reconfig_msg = TRUE;
  tdsrrc_proc_e_type proc;
  tdsrrcllc_oc_process_state_e_type process_status;

  /* Return FALSE if the state transition was triggered by CU procedure */
  if (TDSOC_SET_FOR_DCH_FACH_TRANS == 
      tdsrrcllc_get_ordered_config_state_and_proc( &proc,
                                                &process_status
                                              ) )
  {
    if (proc == TDSRRC_PROCEDURE_CU)
    {
      trg_frm_reconfig_msg = FALSE;   
    }
  }
  return(trg_frm_reconfig_msg);
}

/*===========================================================================

FUNCTION          tdsrrccsp_is_selected_cell_utran_directed

DESCRIPTION       This function returns a TRUE if the selected cell is the
                  same as directed by UTRAN and a FALSE when it is
                  different or when the cell to be camped is not indicated
                  by UTRAN.
DEPENDENCIES
                  This function should be called ONLY IF a call to
                  tdsrrccsp_is_state_transition_triggered_by_reconfig_msg returns
                  a TRUE.
RETURN VALUE
                  Boolean
SIDE EFFECTS
                  None

===========================================================================*/
boolean  tdsrrccsp_is_selected_cell_utran_directed(void)
{
  /* If both destination frequency and destination CPID are present and
   * the acquired cell is on the same freauency and CPID, return a TRUE.
   * In all other cases, return a FALSE.
   */
  if(tdsrrc_csp_int_data.dest_freq_present)
  {
    if(tdsrrcccm_get_curr_camped_freq() == tdsrrc_csp_int_data.dest_freq)
    {
      if(tdsrrc_csp_int_data.dest_cpid_present)
      {
        if(tdsrrcccm_get_curr_camped_cpid() == tdsrrc_csp_int_data.dest_cpid)
        {
          return(TRUE);
        }
      }
    }
  }

  return(FALSE);
} /* tdsrrccsp_is_selected_cell_utran_directed */


/*===========================================================================

FUNCTION          RRC_CSP_SEND_MM_ACT_IND

DESCRIPTION       This function is responsible for filling up data structures
                  for Activation Indication to MM sent by CSP.
                  This also allocates cmd buffer and sends the command.
                  It also frees allocated command and embedded buffers
                  when failure is detected before sending the command.

DEPENDENCIES
                  
RETURN VALUE
                  SUCCESS: Command was successfully sent to MM.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  
===========================================================================*/
uecomdef_status_e_type  
tdsrrccsp_send_mm_act_ind( void )
{
  mm_cmd_type *mm_buf_ptr;
  tdsrrclog_rrccsp_interrat_pfr_end_event_type pfr_end_event;

  /*Unsync the priority as UE camps on a cell */
  //TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Un-sync priority");
  //tdrm_unsync_reason();

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_ACTIVATION_IND)) == NULL)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"No MM buffer for CSP to MM cmd 0x%x",RRC_ACTIVATION_IND);
    return(FAILURE);
  }

  if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
  {
    mm_buf_ptr->cmd.rrc_activation_ind.status = MM_AS_INTERRAT_REDIRECTION;
    /*update the cause for activation indication*/
    tdsrrcmcm_update_tds_activation_cause(TDSRRCMCM_INTERRAT_CELL_REDIRECTION);
  }
  else
  {
    mm_buf_ptr->cmd.rrc_activation_ind.status = MM_AS_INTERRAT_RESELECTION;
    /*update the cause for activation indication*/
    tdsrrcmcm_update_tds_activation_cause(TDSRRCMCM_INTERRAT_CELL_RESELECTION);
  }

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_activation_ind.as_id = tdsrrc_get_as_id();
#endif

  /* Put the command on MM queue */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending MM cmd 0x%x",RRC_ACTIVATION_IND);
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  mm_put_cmd(mm_buf_ptr);

  /* EVENT logging */
  if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN) &&
     tdsrrc_csp_int_data.pseudo_fr_info.pfr_redirect_ongoing)
  {
    pfr_end_event.success = TRUE;
    event_report_payload(EVENT_TDSCDMA_INTER_RAT_PFR_END, sizeof(pfr_end_event),
                                                         (void *)&pfr_end_event);
    tdsrrc_csp_int_data.pseudo_fr_info.pfr_redirect_ongoing = FALSE;
  }

  return(SUCCESS);
} /* tdsrrccsp_send_mm_act_ind */




/*===========================================================================

FUNCTION          RRCCSP_FILL_INFO_FOR_RR

DESCRIPTION       This function is responsible for filling the band preference
                  and the network select mode to be sent to RR.

DEPENDENCIES
                  
RETURN VALUE
                  None.

SIDE EFFECTS
                  None.
                  
===========================================================================*/
void tdsrrccsp_fill_info_for_rr(sys_network_selection_mode_e_type *network_select_mode)
{
  *network_select_mode = tdsrrc_csp_int_data.network_select_mode;
} /* tdsrrccsp_fill_info_for_rr */


/*===========================================================================

FUNCTION          TDSRRCCSP_FILL_FPR_INFO_FOR_RR

DESCRIPTION       This function is responsible for filling the pseudo fast 
                  return info, including TD frequencies and RSCP threshold,
                  to be sent to RR.

DEPENDENCIES
                  
RETURN VALUE
                  TRUE if Pseudo FR is enable; FALSE otherwise

SIDE EFFECTS
                  None.
                  
===========================================================================*/
boolean tdsrrccsp_fill_pfr_info_for_rr(tdsrr_ho_pseudo_fast_return_type *pfr_info_ptr)
{
  uint8 i;
  boolean pfr_enabled = FALSE;

  if((tdsrrc_csp_int_data.pseudo_fr_info.pseudo_fr_active == TRUE) &&
     (pfr_info_ptr != NULL))
  {
    pfr_info_ptr->rscp_threshold = tdsrrc_csp_int_data.pseudo_fr_info.rscp_threshold;
    pfr_info_ptr->num_frequencies = tdsrrc_csp_int_data.pseudo_fr_info.num_freq;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Fill PFR for rr: num_freq=%d",pfr_info_ptr->num_frequencies);
    
    for(i = 0; (i < pfr_info_ptr->num_frequencies) && (i < MAX_UTRAN_TDS_FREQUNECIES) && (i < TDSRRC_CSP_MAX_FPR_FREQ); i++)
    {
      pfr_info_ptr->frequency_list[i] = tdsrrc_csp_int_data.pseudo_fr_info.freq_list[i];
    }
    pfr_enabled = TRUE;
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Fill PFR for rr: pfr_enabled=%d",pfr_enabled);
  
  return pfr_enabled;
} /* tdsrrccsp_fill_pfr_info_for_rr */


/*===========================================================================

FUNCTION          RRCCSP_SET_UE_IN_DEEP_SLEEP

DESCRIPTION       This function is responsible for setting the deep sleep variable
                  when UE is going to deep sleep.

DEPENDENCIES
                  
RETURN VALUE
                  None.

SIDE EFFECTS
                  None.
                  
===========================================================================*/
void tdsrrccsp_set_ue_in_deep_sleep(void)
{
  tdsrrc_csp_int_data.ue_in_deep_sleep = TRUE;

} /* tdsrrccsp_set_ue_in_deep_sleep */


/*===========================================================================

FUNCTION          RRCCSP_UPDATE_BAND_PREFERENCE

DESCRIPTION       This function updates the information related to supported
                  frequency bands based on the band_pref mask.

DEPENDENCIES
                  
RETURN VALUE
                  None.

SIDE EFFECTS
                  None.
                  
===========================================================================*/
void tdsrrccsp_update_band_preference(void)
{
  uint16 band_search_mask = 0;
  tdsrrc_csp_int_data.tds_band_mask = 0;

  band_search_mask = tdsrrc_nv_band_search_mask;
  if(band_search_mask == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"NV band_search_mask is not set,use A+F by default");
    band_search_mask = (SYS_BAND_MASK_TDS_BANDA|SYS_BAND_MASK_TDS_BANDF);
  }
  
  /* When the bands requested by NAS do not have the the same part that set by RRC NV,  
   * such as NAS sets Band E only and RRC NV sets Band A+F, tdsrrc_csp_int_data.tds_band_mask 
   * will be 0. In this case, it would crash. To cover this scenario, we just set to 
   * the bands requested by NAS.  */
  if((tdsrrc_csp_int_data.band_pref & band_search_mask) == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"The bands requested by NAS are not included in RRC NV!");
    band_search_mask = (SYS_BAND_MASK_TDS_BANDA|SYS_BAND_MASK_TDS_BANDF|SYS_BAND_MASK_TDS_BANDE);
  }

  if((tdsrrc_csp_int_data.band_pref & SYS_BAND_MASK_TDS_BANDA) &&
     (band_search_mask & SYS_BAND_MASK_TDS_BANDA))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDD band A supported by the UE");
    tdsrrc_csp_int_data.tds_band_mask = tdsrrc_csp_int_data.tds_band_mask |
                                         RRC_CSP_TDS_A_BAND;
  }
  
  if((tdsrrc_csp_int_data.band_pref & SYS_BAND_MASK_TDS_BANDF) &&
     (band_search_mask & SYS_BAND_MASK_TDS_BANDF))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDD band F supported by the UE");
    if(TDSRRC_CSP_SUPPORT_LOWER_BAND_F)
    {
      tdsrrc_csp_int_data.tds_band_mask |= RRC_CSP_TDS_F_LOWER_BAND;
    }
    if(TDSRRC_CSP_SUPPORT_UPPER_BAND_F)
    {
      tdsrrc_csp_int_data.tds_band_mask |= RRC_CSP_TDS_F_UPPER_BAND;
    }
  }
  
  if((tdsrrc_csp_int_data.band_pref & SYS_BAND_MASK_TDS_BANDE) &&
     (band_search_mask & SYS_BAND_MASK_TDS_BANDE))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDD band E supported by the UE");
    tdsrrc_csp_int_data.tds_band_mask = tdsrrc_csp_int_data.tds_band_mask |
                                         RRC_CSP_TDS_E_BAND;
  }

  return;
}


#ifdef FEATURE_DISCONNECT_ON_UNSUPPORTED_IRAT_REDIRECTION

/*===========================================================================

FUNCTION          RRCCSP_CHECK_GSM_BANDS_SUPPORTED

DESCRIPTION       This function check if UE can support GSM bands or not.

DEPENDENCIES
                  
RETURN VALUE
                  TRUE: If UE can support GSM.
                  FALSE: Otherwise. 

SIDE EFFECTS
                  None.
                  
===========================================================================*/

boolean tdsrrccsp_check_gsm_bands_supported(void)
{
  sd_ss_band_pref_e_type band_capability;
  
  #ifdef FEATURE_DUAL_SIM
  band_capability = sd_misc_get_subs_supp_band_pref(tdsrrc_get_as_id());
  #else
  band_capability = sd_misc_get_supp_band_pref();
  #endif
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"supp band capability %x",band_capability);
  if (((band_capability) & 
           ( SD_SS_BAND_PREF_GSM_450 |
             SD_SS_BAND_PREF_GSM_480 |
             SD_SS_BAND_PREF_GSM_750 |
             SD_SS_BAND_PREF_GSM_850 |
             SD_SS_BAND_PREF_GSM_EGSM_900 |
             SD_SS_BAND_PREF_GSM_PGSM_900 |
             SD_SS_BAND_PREF_GSM_RGSM_900 |
             SD_SS_BAND_PREF_GSM_DCS_1800 |
             SD_SS_BAND_PREF_GSM_PCS_1900)
       ) != 0)
  {
    return TRUE;
  }
  else 
  {
    return FALSE;
  }
}
#endif

/*===========================================================================

FUNCTION          RRC_CSP_UPDATE_ACQ_DB

DESCRIPTION       This function adds an acquisition entry based on the network
                  direction (e.g. RRC connection release message) to the 
                  acquisition list. While doing so, it also updates the 
                  acquisition database pointers so that the 1st pointer points
                  to this most recent acquisition entry.
                   
           
DEPENDENCIES
                  None

RETURN VALUE
                  Success/Failure. Failure will be an extra-ordinary event and
                  could be due to irreversible causes like corruption of pointers.

SIDE EFFECTS
                  None.

===========================================================================*/
uecomdef_status_e_type  tdsrrccsp_update_acq_db(tdsrrc_freq_type freq)
{
  /* copy curr_acq_entry into a temp entry so that the plmn_id, etc. are the same */
  tdsrrc_csp_acq_entry_type temp_acq_entry = tdsrrc_csp_int_data.curr_acq_entry;

  temp_acq_entry.is_valid_nw_sharing = FALSE;

  /* just update the frequency information. rest of the information remains the same */
  temp_acq_entry.freq = freq;

  if(tdsrrccsp_add_acq_entry(&temp_acq_entry) == SUCCESS)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Freq %d added to ACQ database",freq);
    return SUCCESS;
  }
  else
  {
    return FAILURE;
  }
} /* tdsrrccsp_update_acq_db */


/*===========================================================================

FUNCTION TDSRRC_UPDATE_BAND_PREFERENCE

DESCRIPTION

  This function is called by RR to update the band preference
  in RRC.
  
DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
===========================================================================*/
void tdsrrc_update_band_preference
(
  sys_band_mask_type band_pref_mask  /* band mask */
)
{
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_update_band_preference: old=0x%x,new=0x%x",tdsrrc_csp_int_data.band_pref,band_pref_mask);

  tdsrrc_csp_int_data.band_pref = band_pref_mask;
  tdsrrccsp_update_band_preference();

} /* tdsrrc_update_band_preference */

/*===========================================================================

FUNCTION TDSRRC_UPDATE_BAND_PREFERENCE_SUB_ID

DESCRIPTION

  This function serves for the same purpose as tdsrrc_update_band_preference, with TDS
  as_id provided for Dual_SIM.
  
DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
===========================================================================*/
void tdsrrc_update_band_preference_sub_id
(
  sys_band_mask_type band_pref_mask,  /* band mask */
  sys_modem_as_id_e_type  as_id
)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR, "AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  tdsrrc_update_band_preference(band_pref_mask);
} /* tdsrrc_update_band_preference_sub_id */

/*===========================================================================

FUNCTION TDSRRC_IS_ARFCN_VALID

DESCRIPTION

  This function is called by RR to validate the TDS ARFCN it
  reads from the SIs. RRC validates the range and band of the ARFCN.
  
DEPENDENCIES

  None.

RETURN VALUE

  TRUE - If ARFCN is valid,
  FALSE - Otherwise

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrc_is_arfcn_valid
(
  uint32 UARFCN
) 
{
  if(tdsrrc_validate_dl_arfcn(UARFCN) == SUCCESS)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }

} /* tdsrrc_is_arfcn_valid */

/*===========================================================================

FUNCTION TDSRRC_IS_ARFCN_VALID_SUB_ID

DESCRIPTION

  This function serves for the same purpose as tdsrrc_is_arfcn_valid, with TDS
  as_id provided for Dual_SIM.
  
DEPENDENCIES

  None.

RETURN VALUE

  TRUE - If ARFCN and PLMN_id combination is valid,
  FALSE - Otherwise

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrc_is_arfcn_valid_sub_id
(
  uint32 UARFCN,
  sys_modem_as_id_e_type as_id
) 
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR, "AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  return (tdsrrc_is_arfcn_valid(UARFCN));
}

/*===========================================================================

FUNCTION TDSRRC_SET_UE_IN_DEEP_SLEEP

DESCRIPTION

  This function is called by RR to inform RRC that UE is going to DEEP SLEEP
  state.
  
DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_set_ue_in_deep_sleep( boolean value )
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deep sleep indicated by RR %d",value);
  tdsrrc_csp_int_data.ue_in_deep_sleep = value;

} /* tdsrrc_set_ue_in_deep_sleep */

/*===========================================================================

FUNCTION TDSRRC_SET_UE_IN_DEEP_SLEEP_SUB_ID

DESCRIPTION

  This function serves for the same purpose as tdsrrc_set_ue_in_deep_sleep, with TDS
  as_id provided for Dual_SIM.
  
DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_set_ue_in_deep_sleep_sub_id( boolean value,sys_modem_as_id_e_type as_id)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR, "AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  tdsrrc_set_ue_in_deep_sleep(value);
}

/*===========================================================================

FUNCTION RRCCSP_IS_IN_BPLMN_LIST

DESCRIPTION

  This function is called by SIB procedure once it reads the MIB to check if it
  should proceed further and read SIB1 and SIB3 for that cell. If any of the requested 
  PLMNs are in the bcch shared plmn list then SIB should proceed with reading SIBS.
  
DEPENDENCIES

  None.

RETURN VALUE

  TRUE - If PLMN is in the BPLMN list
  FALSE - Otherwise

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccsp_is_in_bplmn_list (rrc_plmn_identity_type plmn_id
                                 , tdsrrc_MasterInformationBlock *mib_ptr
                                 ) 
{
  uint32                    i;
  tdsrrc_csp_acq_entry_type    bplmn_acq_entry;
  boolean                   plmns_shared = FALSE;

  tdsrrc_csp_nw_sharing_entry_type bplmn_nw_db_entry;
  bplmn_nw_db_entry.num_of_shared_plmns = 0;

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"BPLMN: SIB querying if MCC %d, MNC %d on freq %d is in BPLMN list",
                            TDSRRC_CSP_GET_INT_MCC(plmn_id), TDSRRC_CSP_GET_INT_MNC(plmn_id), 
                            tdsrrc_csp_int_data.curr_acq_entry.freq);

  /* First add this frequency to Available PLMN List to eliminate scanning of 
   * other frequencies during full frequency scan 
   */
  tdsrrccsp_add_freq_to_available_freq_list(tdsrrc_csp_int_data.curr_acq_entry.freq);

  if(   (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
     || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
    )
  {

#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
    /* For background service search, the PLMNs should be reported to NAS even
     * if they do not match with those sent by NAS in the PLMN_LIST_REQ.
     * Hence the check here for the enum service_search_bgnd.
     */
    if(    (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC)
        || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_MANUAL)
        || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_PRIORITY_PLMN)
#ifdef FEATURE_TDSCDMA_TO_LTE
        || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_IRAT_PLMN)
#endif
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
        || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_NONE)
#endif
      )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SSB: Read SIBs for ALL PLMNs");
      return TRUE;
    }
#endif

    if(tdsrrc_is_nw_shared(mib_ptr))
    {
      plmns_shared = TRUE;
      bplmn_acq_entry.is_valid_nw_sharing = TRUE;
      tdsrrccsp_find_shared_plmns(
                               mib_ptr,
                               plmn_id,
                               &bplmn_nw_db_entry
                               );
    }
    else
    {
      bplmn_acq_entry.is_valid_nw_sharing = FALSE;
    }


    /* optimization: for Manual PLMN Search, Read SIBs only if the MIB-PLMN matches with
     * requested one.
     */              
    if(plmns_shared == FALSE)
    {
      if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
      {
        for(i = 0; i < tdsrrc_csp_int_data.bplmn_num_req; ++i)
        {
          if(tdsrrc_bcch_and_nas_plmns_match(plmn_id, tdsrrc_csp_int_data.bplmn_req_list[i].plmn_id))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN:SIB proc -> plmn is in BPLMN list");
            return TRUE;
          }
        }   
      }
      else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
      {
        for(i = 0; i < tdsrrc_csp_int_data.wtow_bplmn_num_req; ++i)
        {
          if(tdsrrc_bcch_and_nas_plmns_match(plmn_id, tdsrrc_csp_int_data.wtow_bplmn_req_list[i].plmn_id))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN:SIB proc -> plmn is in BPLMN list");
            return TRUE;
          }
        }   
      }
    }
    else 
    {
      if(tdsrrccsp_is_any_of_bcch_plmns_in_bplmn_list(bplmn_nw_db_entry))
      {
        return TRUE;
      }
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"BPLMN ERR:SIB proc shd NOT have called this function");
    return FALSE;
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN/TD2TD:SIB proc -> plmn is NOT in BPLMN list");
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN/TD2TD:Add the freq/plmn to ACQ DB and avail_plmn_list");

  /* Add the PLMN to ACQ DB */
  if(bplmn_acq_entry.is_valid_nw_sharing)
  {
    bplmn_acq_entry.plmn_id = bplmn_nw_db_entry.shared_plmns_access_info[0].plmn_id;
  }
  else 
  {
    bplmn_acq_entry.plmn_id      = plmn_id;
  }

  bplmn_acq_entry.status_mask  = 0;
  bplmn_acq_entry.freq         = tdsrrc_csp_int_data.curr_acq_entry.freq;
  bplmn_acq_entry.cpid     = tdsrrc_csp_int_data.curr_acq_entry.cpid;

  bplmn_acq_entry.band_class  = tdsrrc_get_frequency_band(
                                                  tdsrrc_csp_int_data.curr_acq_entry.freq);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"band_class 0x%x for freq %d",bplmn_acq_entry.band_class,bplmn_acq_entry.freq);


  if( tdsrrccsp_add_acq_db_and_nw_db_entries(&bplmn_acq_entry, 
                                          &bplmn_nw_db_entry) == SUCCESS)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"BPLMN: Freq %d added to ACQ database",tdsrrc_csp_int_data.curr_acq_entry.freq);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"BPLMN ERR: Unable to update acq db");
  }

  return FALSE;                 
} /* tdsrrccsp_is_in_bplmn_list */


/*===========================================================================

FUNCTION          tdsrrccsp_init_inter_freq_redirection_data

DESCRIPTION       This function initializes appropriate data before attempting
                  acquisition on redirected frequency.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_init_inter_freq_redirection_data(void)
{       
  /* Save the last camped frequency. This frequency will be specifically
   * scanned during the full frequency scan
   */
  tdsrrc_csp_int_data.last_camped_freq_in_connected_mode = tdsrrc_csp_int_data.curr_acq_entry.freq;

  /* Save camped frequency, in case redirection to new frequency fails */
  tdsrrc_csp_int_data.last_camped_freq              = tdsrrc_csp_int_data.curr_acq_entry.freq;
  tdsrrc_csp_int_data.start_acq_on_last_camped_freq = FALSE;
  tdsrrc_csp_int_data.first_acq_on_redirected_freq  = TRUE;

  tdsrrc_csp_int_data.curr_scan = TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN;

  /* Use Shallow mode for redirection */
  tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_SHALLOW;

#ifdef FEATURE_INTER_FREQ_EM_CALL_REDIRECTION
  /* Initialize the variable to FALSE. This variable is set to TRUE only
   * when type-of-plmn-selection variables are updated for an em call */
  tdsrrc_csp_int_data.em_call_acq_type_updated = FALSE;

  /* Search for ANY_PLMN if an emergency call is redirected */
  tdsrrccsp_update_acq_type_for_em_call_ifreq_redirection(TDSRRC_CSP_ANY_PLMN);
#endif

  return;
} /* tdsrrccsp_init_inter_freq_redirection_data */


/*===========================================================================

FUNCTION          tdsrrccsp_is_ue_in_true_limited_service

DESCRIPTION       This function checks the network select mode and returns TRUE/FALSE
                  to the calling procedure.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
boolean tdsrrccsp_is_ue_in_true_limited_service(void)
{
  if(tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}



/*===========================================================================

FUNCTION          tdsrrccsp_cleanup_inter_freq_redirection_state

DESCRIPTION       This function cleans up appropriate data on completing or
                  aborting inter frequency redirection scan.
                                               
DEPENDENCIES
                  None
RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_cleanup_inter_freq_redirection_state(void)
{   
  /* Stop the wait timer if it was active */
  if(!tdsrrc_csp_int_data.start_acq_on_last_camped_freq)
  {
    tdsrrctmr_stop_timer(TDSRRCTMR_FREQ_REDIRECT_WAIT_TIMER);
    tdsrrc_csp_int_data.start_acq_on_last_camped_freq = TRUE;
  }

#ifdef FEATURE_INTER_FREQ_EM_CALL_REDIRECTION
  /* Reset the variables to their original state if they were
   * updated because of an emergency call
   */
  tdsrrccsp_update_acq_type_for_em_call_ifreq_redirection(TDSRRC_CSP_SPEC_PLMN);
#endif

  return;
} /* tdsrrccsp_cleanup_inter_freq_redirection_state */


/*===========================================================================

FUNCTION: TDSRRC_RR_CHECK_CELL_BARRED

DESCRIPTION:
  This function is called by RR to check if a particular TDS cell is 
  barred or not

DEPENDENCIES:
  NONE

RETURN VALUE:
  TDS cell is not barred, or only the CPID is barred, or the frequency
  is barred

SIDE EFFECTS:
  NONE
====================================================================*/
tdsrrc_cell_bar_info_e_type tdsrrc_check_cell_barred (uint16 freq, uint8 cpid)
{
  tdsrrc_cell_bar_info_e_type cell_bar_info = UMTS_NOT_BARRED;

  switch(tdsl1_check_cell_barred(freq, cpid))
  {
    case TDSL1_TDS_NOT_BARRED:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Informing RR: freq %d,cpid %d NOT barred",freq,cpid);
      cell_bar_info = UMTS_NOT_BARRED;
      break;

    case TDSL1_TDS_CPID_BARRED:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Informing RR: freq %d,cpid %d CPID barred",freq,cpid);
      cell_bar_info = UMTS_PSC_BARRED;
      break;

    case TDSL1_TDS_FREQ_BARRED:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Informing RR: freq %d,cpid %d FREQ barred",freq,cpid);
      cell_bar_info = UMTS_FREQ_BARRED;
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"ERR l1_cell_bar_info incorrect. freq %d,cpid %d NOT barred",freq,cpid);
      cell_bar_info = UMTS_NOT_BARRED;
      break;
  }
  return(cell_bar_info);
} /* tdsrrc_check_cell_barred */

/*===========================================================================
FUNCTION: TDSRRC_CHECK_CELL_BARRED_SUB_ID

DESCRIPTION:
  This function serves for the same purpose as tdsrrc_check_cell_barred, with TDS
  as_id provided for Dual_SIM.

DEPENDENCIES:
  NONE

RETURN VALUE:
  TDS cell is not barred, or only the CPID is barred, or the frequency
  is barred

SIDE EFFECTS:
  NONE
====================================================================*/
tdsrrc_cell_bar_info_e_type tdsrrc_check_cell_barred_sub_id
(
  uint16 freq, uint8 cpid, sys_modem_as_id_e_type as_id
)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR, "AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  return (tdsrrc_check_cell_barred( freq, cpid));
}

/*===========================================================================

FUNCTION          tdsrrccsp_send_cell_param_req_for_pch_fach

DESCRIPTION       This function sends the cell param request to Layer1 if on  
                  transition from FACH to PCH or PCH to FACH, SIb4 is present
                  in the db and has reselection extensions t_reselection or
                  q_hyst.
                                               
DEPENDENCIES
                  None
RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_send_cell_param_req_for_pch_fach(void)
{   
  tdsrrc_SysInfoType3    *sib3_ptr = NULL;
  tdsrrc_SysInfoType4    *sib4_ptr = NULL;
  rrc_plmn_identity_type common_plmn_id;
  if(tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing)
  {
    common_plmn_id = tdsrrc_csp_int_data.curr_nw_db_entry.common_plmn_id;
  }
  else
  {
    common_plmn_id = tdsrrc_csp_int_data.curr_acq_entry.plmn_id;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sib_idx PLMN: MCC %d-MNC %d",TDSRRC_CSP_GET_INT_MCC(common_plmn_id),TDSRRC_CSP_GET_INT_MNC(common_plmn_id));
  /* PLMN and Cell ID of the current camped on cell are known. Get SIB3 */
  sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrc_sibdb_return_sib_for_cell(
                                        tdsrrc_SIB3, 
                                        common_plmn_id, 
                                        tdsrrc_csp_int_data.curr_acq_entry.cell_id);
  if( sib3_ptr == NULL )
  {
    /* SIB3 for this PLMN, Cell ID combination not present. 
     */
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB3 for MCC %d, MNC %d, Cell ID %d not present", 
              TDSRRC_CSP_GET_INT_MCC(tdsrrc_csp_int_data.curr_acq_entry.plmn_id),
               TDSRRC_CSP_GET_INT_MNC(tdsrrc_csp_int_data.curr_acq_entry.plmn_id), 
               tdsrrc_csp_int_data.curr_acq_entry.cell_id);
    return;
  }

  if(sib3_ptr->sib4indicator == TRUE)
  {
     sib4_ptr = (tdsrrc_SysInfoType4*) tdsrrcsibdb_return_sib_for_srv_cell(
                                    tdsrrc_SIB4); 
  }
  if((sib4_ptr != NULL) && (sib4_ptr->m.v4b0NonCriticalExtensionsPresent) &&
    (sib4_ptr->v4b0NonCriticalExtensions.m.
    v590NonCriticalExtensionPresent) &&
    (sib4_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.m.
    v5b0NonCriticalExtensionPresent) && (sib4_ptr->v4b0NonCriticalExtensions.
    v590NonCriticalExtension.v5b0NonCriticalExtension.sysInfoType4_v5b0ext.m.
    cellSelectReselectInfoPCHFACH_v5b0extPresent))
  {
    /* Send TDSL1_CPHY_CELL_PARM_REQ to L1 */
    if(tdsrrccsp_send_l1_cmd(TDSRRC_PROCEDURE_CSP, TDSL1_CPHY_CELL_PARM_REQ,
                                &tdsrrc_csp_int_data.curr_acq_entry) == SUCCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"CELL PARAM REQ for sib4 PCH/FACH info sent");
    }
    else
    {
      ERR_FATAL("RRC Unable to get cmd buf for L1 cmd 0x%x",TDSL1_CPHY_CELL_PARM_REQ,0,0);
    }    
  }  
  return;
} /*tdsrrccsp_send_cell_param_req_for_pch_fach */


/*====================================================================
FUNCTION: RRCCSP_SEND_CONN_MODE_OOS_DS_REQ

DESCRIPTION:
  This function constructs the TDSRRC_CONN_MODE_DEEP_SLEEP_REQ primitive and sends it
  to LLC. This function is called when UE needs to be put in deep sleep in
  RRC Connected Mode.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
====================================================================*/
void tdsrrccsp_send_conn_mode_oos_ds_req (void)
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_CONN_MODE_DEEP_SLEEP_REQ",0,0,0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CONN_MODE_DEEP_SLEEP_REQ;

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"OOS: TDSRRC_CONN_MODE_DEEP_SLEEP_REQ sent to LLC");

  return;
} /* end tdsrrccsp_send_conn_mode_oos_ds_req() */


/*====================================================================
FUNCTION: RRCCSP_SEND_CONN_MODE_OOS_DS_WAKEUP_REQ

DESCRIPTION:
  This function constructs the RRCCSP_SEND_CONN_MODE_OOS_DS_WAKEUP_REQ primitive and sends it
  to LLC. This function is called when UE needs to be woken up from deep sleep in
  RRC Connected Mode.

DEPENDENCIES:
  None

RETURN VALUE:
  uecomdef_status_e_type.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrccsp_send_conn_mode_oos_ds_wakeup_req (void)
{
  tdsrrc_cmd_type *cmd_ptr;

  if(tdsrrcwrm_block_wait_to_acquire_lock_for_time(300, TRM_ACQUISITION, NULL) == FAILURE)
  {
    return FAILURE;
  }

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_CONN_MODE_DEEP_SLEEP_WAKEUP_REQ",0,0,0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CONN_MODE_DEEP_SLEEP_WAKEUP_REQ;

  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"OOS: TDSRRC_CONN_MODE_DEEP_SLEEP_WAKEUP_REQ sent to LLC");

  return SUCCESS;
} /* end tdsrrccsp_send_conn_mode_oos_ds_wakeup_req() */


/*===========================================================================

FUNCTION          RRC_CONVERT_BAND_TO_SYS_BAND

DESCRIPTION       This function takes the band used internally by RRC and 
                  converts it into sys band which is used by CM/SD.
                  
DEPENDENCIES      None

RETURN VALUE      sys band type used by CM/SD. 

SIDE EFFECTS      None
===========================================================================*/
sys_band_mask_e_type tdsrrc_convert_band_to_sys_band(tdsrrc_csp_band_class_type band)
{
  sys_band_mask_e_type ret_sys_band = SYS_BAND_MASK_TDS_BANDA;
  
  switch(band)
  {
    case RRC_CSP_TDS_A_BAND:
      ret_sys_band = SYS_BAND_MASK_TDS_BANDA;
      break;

    case RRC_CSP_TDS_E_BAND:
      ret_sys_band = SYS_BAND_MASK_TDS_BANDE;
      break;

    case RRC_CSP_TDS_F_BAND:
    case RRC_CSP_TDS_F_LOWER_BAND:
    case RRC_CSP_TDS_F_UPPER_BAND:
      ret_sys_band = SYS_BAND_MASK_TDS_BANDF;
      break;

    default: 
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Incorrect band 0x%x passed",band);
      break;
  }
  
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"band 0x%x,sys_band 0x%x",band,ret_sys_band);
  return(ret_sys_band);
} /* tdsrrc_convert_band_to_sys_band */

/*====================================================================
FUNCTION: RRCCSP_SKIP_SIBS_FOR_FPLMN

DESCRIPTION:
         This function checks whether SIB procedure needs to read SIBs or not after reading MIB.
         This function will be used only by SIB module to avoid extra time consumed during SIB
         reading if (all shared) PLMN(s) (are) is forbidden.
         CSP will check for forbidden status of the PLMN only when n/w sel mode is 
         automatic and acq_type is SPECIFIC and Cell SELECTION is in progress.

DEPENDENCIES:
         None

RETURN VALUE:
         TRUE: skip reading SIBS if the passed PLMN is FPLMN
         FALSE: otherwise

SIDE EFFECTS:
         None.
====================================================================*/
boolean tdsrrccsp_skip_sibs_for_fplmn(
  rrc_plmn_identity_type plmn_id
  , tdsrrc_MasterInformationBlock *mib_ptr
  )
{
  tdsrrc_csp_acq_entry_type    fplmn_acq_entry;

  boolean is_plmn_forbidden = FALSE;


  tdsrrc_csp_nw_sharing_entry_type  fplmn_nw_db_entry;
  uint8 shared_plmn_idx = 0;
  fplmn_nw_db_entry.num_of_shared_plmns = 0;

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"FPLMN: curr_type_acq=%d, curr_scan=%d, n/w_sel_mode=%d",
                                                tdsrrc_csp_int_data.curr_type_acq,
                                                tdsrrc_csp_int_data.curr_scan,
                                                tdsrrc_csp_int_data.network_select_mode);

  /* First add this frequency to Available PLMN List to eliminate scanning of 
   * other frequencies during full frequency scan 
   */
  tdsrrccsp_add_freq_to_available_freq_list(tdsrrc_csp_int_data.curr_acq_entry.freq);

  if(((tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) &&
       (tdsrrc_csp_int_data.curr_type_acq == TDSRRC_CSP_SPEC_PLMN) &&
       (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN))
#ifdef FEATURE_WRLF_SYSTEM_SEL
      ||(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS() && 
        !((tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED)
             && (tdsrrc_csp_int_data.service_req_cause == RRC_MO_CS_CALL))
        )
#endif
#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
     ||((tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN) &&
          ((tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC ) 
            ||(tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_PRIORITY_PLMN)
#ifdef FEATURE_TDSCDMA_TO_LTE
            || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_IRAT_PLMN)
#endif
          ))
#endif
     /* Skip reading SIBs for FPLMN during connected mode OOS search for SPEC PLMN search. */
     ||((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) &&
          (tdsrrc_csp_int_data.curr_type_acq == TDSRRC_CSP_SPEC_PLMN))
     )
  {

    if(tdsrrc_is_nw_shared(mib_ptr))
    {
      fplmn_acq_entry.is_valid_nw_sharing = TRUE;
      tdsrrccsp_find_shared_plmns(
                               mib_ptr,
                               plmn_id,
                               &fplmn_nw_db_entry
                               );
    }
    else
    {
      fplmn_acq_entry.is_valid_nw_sharing = FALSE;
    }

    do
    {
      if((fplmn_acq_entry.is_valid_nw_sharing) && (shared_plmn_idx < TDSMAX_SHARED_PLMNS) && (shared_plmn_idx < fplmn_nw_db_entry.num_of_shared_plmns))
      {
        plmn_id = fplmn_nw_db_entry.shared_plmns_access_info[shared_plmn_idx++].plmn_id;
      }

#ifdef FEATURE_DUAL_SIM
      is_plmn_forbidden = reg_sim_per_subs_plmn_forbidden(mmumts_convert_rrc_plmn_id_to_nas_plmn_id(plmn_id),
                                      tdsrrc_get_as_id());
#else
      is_plmn_forbidden = reg_sim_plmn_forbidden(mmumts_convert_rrc_plmn_id_to_nas_plmn_id(plmn_id));
#endif

      if(is_plmn_forbidden == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"FPLMN: BCCH PLMN is not forbidden ");
        return FALSE;
      }

    }while((fplmn_acq_entry.is_valid_nw_sharing) && (shared_plmn_idx < TDSMAX_SHARED_PLMNS) && (shared_plmn_idx < fplmn_nw_db_entry.num_of_shared_plmns));

    /* This check has been added for cr109422. During foreground
    * service search if a cell is found on which UE can camp on
    * then set the var plmn_fnd_during_fgnd_svc_srch to TRUE.
    * This var has been added to inform NAS (using var ue_oos 
    * in RRC_SERVICE_CNF) that service is available. NAS looks
    * at this var to determine if limited service request should
    * be sent to RRC or not. Notice that with UOOS, SD always sends
    * Automatic Service Request and it sends Limited Service Request
    * only when NAS informs SD that Limited Service is available
    */
    if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)
    {
      tdsrrc_csp_int_data.plmn_fnd_during_fgnd_svc_srch = TRUE;
    }

    /* Add the PLMN to ACQ DB */
    /* Avaialable PLMNs are forbidden. Add into ACQ DB */
    fplmn_acq_entry.status_mask  = 0;
    fplmn_acq_entry.cell_id= 0;
    fplmn_acq_entry.freq         = tdsrrc_csp_int_data.curr_acq_entry.freq;
    fplmn_acq_entry.cpid         = tdsrrc_csp_int_data.curr_acq_entry.cpid;

    fplmn_acq_entry.band_class  = tdsrrc_get_frequency_band(
                                                    tdsrrc_csp_int_data.curr_acq_entry.freq);
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"band_class 0x%x for freq %d",fplmn_acq_entry.band_class,fplmn_acq_entry.freq);



    if(fplmn_acq_entry.is_valid_nw_sharing)
    {
      fplmn_acq_entry.plmn_id = fplmn_nw_db_entry.shared_plmns_access_info[0].plmn_id;
    }
    else 
    {
      fplmn_acq_entry.plmn_id      = plmn_id;
    }

    if( tdsrrccsp_add_acq_db_and_nw_db_entries(&fplmn_acq_entry, 
                                            &fplmn_nw_db_entry) == SUCCESS)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"BPLMN: Freq %d added to ACQ database",tdsrrc_csp_int_data.curr_acq_entry.freq);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"BPLMN ERR: Unable to update acq db");
    }

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"FPLMN: BCCH PLMN is forbidden PLMN");
    return TRUE;
  }

  return FALSE;
}

/*====================================================================
FUNCTION: RRCCSP_SKIP_SIBS_FOR_VPLMN

DESCRIPTION:
         This function checks if the PLMN is a visitor PLMN and if it is already 
         present in the list to be sent to NAS.
DEPENDENCIES:
         None

RETURN VALUE:
         TRUE: skip reading SIBS if the passed PLMN is VPLMN and if the PLMN is 
               already added in the list to be sent to NAS.
         FALSE: otherwise

SIDE EFFECTS:
         None.
====================================================================*/
boolean tdsrrccsp_skip_sibs_for_vplmn(
  rrc_plmn_identity_type plmn_id
  , tdsrrc_MasterInformationBlock *mib_ptr
  )
{
  boolean is_valid_nw_sharing;
  uint32 i;
  rrc_plmn_identity_type nas_to_rrc_plmn_id;
  boolean ret_val = FALSE;
  tdsrrc_csp_nw_sharing_entry_type  vplmn_nw_db_entry;
  uint8 shared_plmn_idx = 0;
  uint32 num_plmns = 0;
  vplmn_nw_db_entry.num_of_shared_plmns = 0;
  /* We should skip SIB reading only for cells whose PLMNs are not NAS Requested PLMNs/EPLMNs
     and whose PLMNs are already added to the list to be sent to NAS */
  if(((tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) ||
      (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)) &&
      (tdsrrc_csp_int_data.curr_type_acq == TDSRRC_CSP_SPEC_PLMN) &&
      (tdsrrccsp_check_plmn_match(plmn_id,mib_ptr) == FALSE) &&
      (tdsrrc_csp_int_data.service_search_bgnd != SYS_SERVICE_SEARCH_CSG))
  {
    if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)
    {
      num_plmns = tdsrrc_csp_int_data.plmn_list.num_plmns;
    }
    else if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN) ||
            (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN))
    {
      num_plmns = tdsrrc_csp_int_data.bplmn_fnd_list_wr_index;
    }
    if(tdsrrc_is_nw_shared(mib_ptr))
    {
      is_valid_nw_sharing = TRUE;
      tdsrrccsp_find_shared_plmns(
                               mib_ptr,
                               plmn_id,
                               &vplmn_nw_db_entry
                               );
    }
    else
    {
      is_valid_nw_sharing = FALSE;
    }
    do
    {
      if((is_valid_nw_sharing) && (shared_plmn_idx < vplmn_nw_db_entry.num_of_shared_plmns) &&
         (shared_plmn_idx < TDSMAX_SHARED_PLMNS))
      {
        plmn_id = vplmn_nw_db_entry.shared_plmns_access_info[shared_plmn_idx++].plmn_id;
      }
      for(i=0; i<num_plmns; i++)
      {
        if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN) ||
           (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN))
        {
          if(tdsrrc_csp_int_data.plmn_list.plmn[i].rat == SYS_RAT_TDS_RADIO_ACCESS)
          {
            if( TDSRRC_CSP_GET_INT_MCC(plmn_id) ==
                  TDSRRC_CSP_GET_INT_MCC(tdsrrc_csp_int_data.plmn_list.plmn[i].plmn) &&
                    TDSRRC_CSP_GET_INT_MNC(plmn_id)
                      == TDSRRC_CSP_GET_INT_MNC(tdsrrc_csp_int_data.plmn_list.plmn[i].plmn))
            {
              ret_val = TRUE;
              break;
            }
            else
            {
              ret_val = FALSE;
            }
          }
        }
        else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
        {
          if(tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[i].rat == SYS_RAT_TDS_RADIO_ACCESS)
          {
            nas_to_rrc_plmn_id = mmumts_convert_nas_plmn_id_to_rrc_plmn_id(tdsrrc_csp_int_data.rrc_rr_plmn_srch_cnf.available_plmn_list.info[i].plmn);
            if( TDSRRC_CSP_GET_INT_MCC(plmn_id) ==
                TDSRRC_CSP_GET_INT_MCC(nas_to_rrc_plmn_id) &&
                  TDSRRC_CSP_GET_INT_MNC(plmn_id)
                    == TDSRRC_CSP_GET_INT_MNC(nas_to_rrc_plmn_id))
            {
              ret_val = TRUE;
              break;
            }
            else
            {
              ret_val = FALSE;
            }
          }
        }
      }
      if(FALSE == ret_val)
      {
        return ret_val;
      }
    }while((is_valid_nw_sharing) && (shared_plmn_idx < vplmn_nw_db_entry.num_of_shared_plmns));
  }
  return ret_val;
}

/*===========================================================================

FUNCTION          RRCCSP_IS_CAMPED_CELL_NETWORK_SHARED

DESCRIPTION       Used to find out whether the current camped cell is a part of shared network
                  
DEPENDENCIES      None

RETURN VALUE      TRUE: Camped cell is a part of shared network
                  FALSE: Camped cell belongs to a dedicated network

SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrccsp_is_camped_cell_network_shared()
{
  boolean status = FALSE;
  tdsrrc_MasterInformationBlock *mib;
  mib = (tdsrrc_MasterInformationBlock*)tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_MIB);
  if(tdsrrc_csp_int_data.primary_plmn_Id_present && (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Include primary plmnid in initial direct transfer msg = %d",tdsrrc_csp_int_data.primary_plmn_Id_present);
    status = TRUE;
    return status;
  }
  if(mib != NULL)
  {
    if(tdsrrc_is_nw_shared(mib))
    {
      status = TRUE;
    }
    else
    {
      status = FALSE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: MIB ptr is NULL for camped cell");
    status = FALSE;
  }
  return status;
}

/*===========================================================================

FUNCTION          RRCCSP_IS_MIB_PLMN_VALID

DESCRIPTION       If network is shared finds out the validity of the common PLMN given in the mib.
                  
DEPENDENCIES      None

RETURN VALUE      TRUE: common PLMN is valid.
                  FALSE: common PLMN is not valid and therefore can not be considered for PLMN selection

SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrccsp_is_mib_plmn_valid(tdsrrc_MasterInformationBlock *mib_ptr)
{
  boolean mibPLMN_Identity_valid = FALSE;

  if(tdsrrc_is_nw_shared(mib_ptr))
  {
    mibPLMN_Identity_valid = mib_ptr->v690NonCriticalExtensions.masterInformationBlock_v690ext.multiplePLMN_List.mibPLMN_Identity;
  }
  else
  {
    mibPLMN_Identity_valid = TRUE;
  }

  return mibPLMN_Identity_valid;
}

/*===========================================================================

FUNCTION          RRCCSP_CONSTRUCT_MULTIPLE_PLMN_LIST_FROM_MIB

DESCRIPTION       Constructs the shared PLMN list given the Mib
                  
DEPENDENCIES      None

RETURN VALUE      number of shared PLMNs

SIDE EFFECTS      None
===========================================================================*/
uint8 tdsrrccsp_construct_multiple_plmn_list_from_mib(
                                                   tdsrrc_MasterInformationBlock *mib_ptr,
                                                   rrc_plmn_identity_type *shared_PLMN_list)
{
  rrc_plmn_identity_type mib_PLMN_ID;
  tdsrrc_MultiplePLMN_List_r6 multiple_PLMN_List;
  rrc_plmn_identity_type plmn_id;
  boolean is_mcc_valid;
  byte mcc_to_be_used[RRC_MAX_MCC_DIGITS];
  uint8 number_of_PLMNs = 0;
  int i;
  uint32 n_value =0, n_index = 0;

  tdsrrc_translate_gsm_map_plmn_id(&(mib_ptr->plmn_Type.u.gsm_MAP->plmn_Identity),&mib_PLMN_ID);

  if(tdsrrccsp_is_mib_plmn_valid(mib_ptr))
  {
    shared_PLMN_list[number_of_PLMNs++] = mib_PLMN_ID;
  }

  for(i = 0; i < RRC_MAX_MCC_DIGITS; i++)
  {
    mcc_to_be_used[i] = mib_PLMN_ID.mcc[i];
  }

  multiple_PLMN_List = mib_ptr->v690NonCriticalExtensions.masterInformationBlock_v690ext.multiplePLMN_List;

  n_value = multiple_PLMN_List.multiplePLMNs.n;

  while(n_value != 0)
  {
    tdsrrc_translate_gsm_map_plmn_id_with_optional_mcc(&is_mcc_valid, &(multiple_PLMN_List.multiplePLMNs.elem[n_index]), &plmn_id);

    if(!is_mcc_valid)
    {
      for(i = 0; i < RRC_MAX_MCC_DIGITS; i++)
      {
        plmn_id.mcc[i] = mcc_to_be_used[i];
      }
    }
    else
    {
      for(i = 0; i < RRC_MAX_MCC_DIGITS; i++)
      {
        mcc_to_be_used[i] = plmn_id.mcc[i];
      }
    }

    shared_PLMN_list[number_of_PLMNs++] = plmn_id;
    n_value--;
    n_index++;
  }
  return number_of_PLMNs;
}

/*===========================================================================

FUNCTION          RRCCSP_PRINT_SHARED_PLMNS

DESCRIPTION       prints the list of shared PLMNs
                  
DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
static void tdsrrccsp_print_shared_PLMNs(int number_of_PLMNs, rrc_plmn_identity_type *shared_PLMN_list)
{
  int     i;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"NWS: Number of Shared PLMNs: %d ",number_of_PLMNs);
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Shared PLMN List before Prioritizing ");
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"******************************************************************");
  for(i =0 ; i<number_of_PLMNs && i < TDSMAX_SHARED_PLMNS; i++)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH," MCC:MNC - %d:%d",TDSRRC_CSP_GET_INT_MCC(shared_PLMN_list[i]),TDSRRC_CSP_GET_INT_MNC(shared_PLMN_list[i]));
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"******************************************************************");
}

/*===========================================================================

FUNCTION          RRCCSP_PRIORITIZE_THE_LIST_OF_SHARED_PLMNS

DESCRIPTION       Used to prioritize a PLMN among the available PLMNs based on the current scan type so that
                  this PLMN is considered first for PLMN selection if it is part of the list of shared PLMNs.
                  
DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_prioritize_the_list_of_shared_plmns(void)
{
  int i =0 ;
  tdsrrc_csp_shared_plmns_access_info_type temp_plmn_access_info;
  rrc_plmn_identity_type plmn_tobe_prioritized;
  tdsrrc_csp_nw_sharing_entry_type *bcch_plmn_list = &tdsrrc_csp_int_data.curr_nw_db_entry;

  if( (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_PLMN_LIST_SCAN) ||
       (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN) ||
       (tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_MANUAL_SCAN) )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"NWS: No Need to prioritize. curr_scan : %d",tdsrrc_csp_int_data.curr_scan);
    return;
  }

  if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
  {
    plmn_tobe_prioritized = tdsrrc_csp_int_data.wtow_bplmn_req_list[0].plmn_id;
  }
  else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
  {
    plmn_tobe_prioritized = tdsrrc_csp_int_data.bplmn_req_list[0].plmn_id;
  }
  else
  {
    plmn_tobe_prioritized = tdsrrc_csp_int_data.curr_req_plmn.plmn_id;
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"NWS: Current Scan: %d, PLMN tobe prioritized: MCC:MNC - %d:%d",
                                                                      tdsrrc_csp_int_data.curr_scan,
                                                                      TDSRRC_CSP_GET_INT_MCC(plmn_tobe_prioritized),
                                                                      TDSRRC_CSP_GET_INT_MNC(plmn_tobe_prioritized));

  /* Prioritizing to keep the best PLMN in index 0 */

  for(i = 0; i < bcch_plmn_list->num_of_shared_plmns; i++)
  {
    if(tdsrrc_bcch_and_nas_plmns_match(bcch_plmn_list->shared_plmns_access_info[i].plmn_id,
                                                               plmn_tobe_prioritized))
    {
      /* Move it to the begining */

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"NWS: Prioritizing the PLMN MCC:MNC - %d:%d",TDSRRC_CSP_GET_INT_MCC(bcch_plmn_list->shared_plmns_access_info[i].plmn_id),TDSRRC_CSP_GET_INT_MNC(bcch_plmn_list->shared_plmns_access_info[i].plmn_id));

      temp_plmn_access_info = bcch_plmn_list->shared_plmns_access_info[0];
      bcch_plmn_list->shared_plmns_access_info[0] = bcch_plmn_list->shared_plmns_access_info[i];
      bcch_plmn_list->shared_plmns_access_info[i] = temp_plmn_access_info;
      break;
    }
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"NWS: Number of Shared PLMNs: %d ",bcch_plmn_list->num_of_shared_plmns);
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Shared PLMN List after Prioritizing ");
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"******************************************************************");
  for(i =0 ; i<bcch_plmn_list->num_of_shared_plmns; i++)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH," MCC:MNC - %d:%d",TDSRRC_CSP_GET_INT_MCC(bcch_plmn_list->shared_plmns_access_info[i].plmn_id),TDSRRC_CSP_GET_INT_MNC(bcch_plmn_list->shared_plmns_access_info[i].plmn_id));
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"******************************************************************");

  return;
}

/*===========================================================================

FUNCTION          RRCCSP_FIND_SHARED_PLMNS

DESCRIPTION       Finds out the list of shared PLMNs and fills in the nw_db_entry_ptr.
                  
DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_find_shared_plmns(
                              tdsrrc_MasterInformationBlock* mib_ptr,
                              rrc_plmn_identity_type plmn_id,
                              tdsrrc_csp_nw_sharing_entry_type* nw_db_entry_ptr
                              )
{
  rrc_plmn_identity_type shared_plmn_list[TDSMAX_SHARED_PLMNS];
  uint8 num_of_shared_plmns;
  uint8 i;

  nw_db_entry_ptr->common_plmn_id = plmn_id;
  num_of_shared_plmns = tdsrrccsp_construct_multiple_plmn_list_from_mib(
                                                                     mib_ptr,
                                                                     shared_plmn_list);

  tdsrrccsp_print_shared_PLMNs(num_of_shared_plmns,shared_plmn_list);

  for(i = 0; (i < TDSMAX_SHARED_PLMNS) && (i<num_of_shared_plmns); i++)
  {
    nw_db_entry_ptr->shared_plmns_access_info[i].plmn_id= shared_plmn_list[i];
  }
  nw_db_entry_ptr->num_of_shared_plmns = num_of_shared_plmns;
}

/*===========================================================================

FUNCTION          RRCCSP_GET_CELL_PLMN_STATUS

DESCRIPTION       This function compares the requested PLMN with the currently
                  acquired cells PLMN.
                  
DEPENDENCIES      None

RETURN VALUE      TDSRRC_CSP_PLMN_SAME_AS_REQ
                  TDSRRC_CSP_PLMN_DIFF_THAN_REQ

SIDE EFFECTS      None
===========================================================================*/
tdsrrc_csp_plmn_status_e_type tdsrrccsp_get_cell_plmn_status
(
  tdsrrc_csp_acq_entry_type* acq_entry_ptr,
  rrc_plmn_identity_type reference_plmn
)
{
      tdsrrc_csp_plmn_status_e_type  cell_plmn_status;

      if(tdsrrc_bcch_and_nas_plmns_match(acq_entry_ptr->plmn_id, reference_plmn))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN is the same as requested BCCH and NAS PLMN match");
        cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
      }
      else
      {
        if((tdsrrccsp_check_for_eplmn(
                  mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                    reference_plmn))) == TRUE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"NAS PLMN is in the eq PLMN list");
          if((tdsrrccsp_check_for_eplmn(
                    mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                     acq_entry_ptr->plmn_id))) == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH PLMN is in the eq PLMN list");
            cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH PLMN not in eq PLMN list");
            cell_plmn_status = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"NAS PLMN not in eq PLMN list");
          cell_plmn_status = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;
        }
      }

      if((cell_plmn_status == TDSRRC_CSP_PLMN_DIFF_THAN_REQ) &&
         (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) &&
         (tdsrrc_csp_int_data.can_ehplmn_be_camped) &&
         (tdsrrccsp_get_csp_substate() == TDSRRCCSP_CELL_SEL_WT_SIB) &&
         (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN))
      {
        if(tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn(acq_entry_ptr->plmn_id))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH, "FR20971: BCCH PLMN is in the HPLMN/EHPLMN list ");
          cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
        }
      }

      return cell_plmn_status;
}

/*===========================================================================

FUNCTION TDSRRCCSP_CHECK_PLMN_MATCH

DESCRIPTION

  This function checks the PLMN ID given as an arguement is 
  matching with the NAS requested PLMN or equivalent PLMN.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  boolean

SIDE EFFECTS
===========================================================================*/
boolean tdsrrccsp_check_plmn_match( rrc_plmn_identity_type plmn_id
  , tdsrrc_MasterInformationBlock *mib_ptr)
{
    tdsrrc_csp_plmn_status_e_type cell_plmn_status;
    boolean plmns_match = FALSE;
    tdsrrc_csp_acq_entry_type    *acq_entry =  tdsrrc_malloc(sizeof(tdsrrc_csp_acq_entry_type));

    tdsrrc_csp_nw_sharing_entry_type  *nw_db_entry = tdsrrc_malloc(sizeof(tdsrrc_csp_nw_sharing_entry_type));
    uint8 shared_plmn_idx = 0; 
    nw_db_entry->num_of_shared_plmns = 0;
    cell_plmn_status = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;

    if(tdsrrc_is_nw_shared(mib_ptr))
    {
      acq_entry->is_valid_nw_sharing = TRUE;
      tdsrrccsp_find_shared_plmns(
                               mib_ptr,
                               plmn_id,
                               nw_db_entry
                               );
    }
    else
    {
      acq_entry->is_valid_nw_sharing = FALSE;
    }

  
    do
    {
      if((acq_entry->is_valid_nw_sharing) && (shared_plmn_idx < nw_db_entry->num_of_shared_plmns) &&
         (shared_plmn_idx < TDSMAX_SHARED_PLMNS))
      {
        acq_entry->plmn_id = nw_db_entry->shared_plmns_access_info[shared_plmn_idx++].plmn_id;
      }
      else
      {
        acq_entry->plmn_id = plmn_id;
      }

      if(tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ)
      {
        /* For ANY PLMN Request, we don't care what PLMN is selected */
        cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
        plmns_match = TRUE;
      }
      else/* PLMN request for a specific PLMN */
      {
        cell_plmn_status = tdsrrccsp_get_cell_plmn_status(acq_entry, tdsrrc_csp_int_data.curr_req_plmn.plmn_id);
        if(cell_plmn_status == TDSRRC_CSP_PLMN_SAME_AS_REQ)
        {
          plmns_match = TRUE;
        }
  
      } /* end PLMN request for a specific PLMN */
  
    }while((acq_entry->is_valid_nw_sharing) && 
           (shared_plmn_idx < nw_db_entry->num_of_shared_plmns) &&
           (plmns_match == FALSE));
  
  tdsrrc_free(acq_entry);
  tdsrrc_free(nw_db_entry);
  return plmns_match;
}

/*===========================================================================

FUNCTION          RRCCSP_GET_CELL_STATUS

DESCRIPTION       Finds out the suitability of the cell for camping based on the cell bar status, requesed PLMN etc.
                  
DEPENDENCIES      None

RETURN VALUE      
                  TDSRRC_CSP_CELL_BETTER_THAN_EXPECTED: Cell exceeds expectations.
                  TDSRRC_CSP_CELL_AS_EXPECTED: Cell meets expectations.
                  TDSRRC_CSP_CELL_BELOW_EXPECTED: Cell is below expectations.

SIDE EFFECTS      None
===========================================================================*/
tdsrrc_csp_cell_actual_vs_expected_e_type tdsrrccsp_get_cell_status(
                                                              tdsrrc_csp_plmn_status_e_type* cell_plmn_status,
                                                              tdsrrc_csp_acq_entry_type* acq_entry_ptr
                                                              ,tdsrrc_csp_nw_sharing_entry_type* nw_db_entry_ptr
                                                              )
  {
  tdsrrc_csp_cell_actual_vs_expected_e_type cell_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;

  boolean plmns_match = FALSE;

#ifdef FEATURE_WRLF_SYSTEM_SEL
  tdsrrc_csp_plmn_status_e_type cell_plmn_status_wrt_rlf_plmn = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;
#endif

  uint8 shared_plmn_idx = 0;

  do
  {
    if(acq_entry_ptr->is_valid_nw_sharing)
    {
      acq_entry_ptr->plmn_id = nw_db_entry_ptr->shared_plmns_access_info[shared_plmn_idx].plmn_id;
      acq_entry_ptr->status_mask = nw_db_entry_ptr->shared_plmns_access_info[shared_plmn_idx++].status_mask;
    }

    /* Now decide if the cell is good for camping */

    /* Generate PLMN status based on received and requested PLMN IDs */
    if((tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ)
#ifdef FEATURE_WRLF_SYSTEM_SEL
       && !TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()
#endif
      )
    {
      /* For ANY PLMN Request, we don't care what PLMN is selected */
      *cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
      plmns_match = TRUE;
    }
    else  /* PLMN request for a specific PLMN */
    {
      *cell_plmn_status = tdsrrccsp_get_cell_plmn_status(acq_entry_ptr, tdsrrc_csp_int_data.curr_req_plmn.plmn_id);
      if(*cell_plmn_status == TDSRRC_CSP_PLMN_SAME_AS_REQ)
      {
        plmns_match = TRUE;
      }

#ifdef FEATURE_WRLF_SYSTEM_SEL
      if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
      {
        cell_plmn_status_wrt_rlf_plmn = tdsrrccsp_get_cell_plmn_status(acq_entry_ptr, tdsrrc_csp_int_data.rlf_plmn_id);
      }
#endif
    } /* end PLMN request for a specific PLMN */

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrccsp_get_cell_status: acq_entry_ptr->status_mask = 0x%x, curr_type_acq = %d", 
                acq_entry_ptr->status_mask, 
                tdsrrc_csp_int_data.curr_type_acq);

    /* Compare the cell based on status mask formed from SIBs received for this cell */

    /* Decide if this cell is good enough - If actual cell status is lower than the one
      * expected by Selection Controller, this cell is not selected.
      */
#ifdef FEATURE_WRLF_SYSTEM_SEL
    if(TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
    {
      cell_status = tdsrrccsp_compare_expected_cell_for_conn_oos(*cell_plmn_status,
                                           cell_plmn_status_wrt_rlf_plmn,
                                           acq_entry_ptr->status_mask,
                                           tdsrrc_csp_int_data.curr_type_acq);
    }
    else
#endif
    {
      cell_status = tdsrrccsp_compare_expected_cell(*cell_plmn_status,
                                           acq_entry_ptr->status_mask,
                                           tdsrrc_csp_int_data.curr_type_acq);
    }

  }while((acq_entry_ptr->is_valid_nw_sharing) && 
         (shared_plmn_idx < nw_db_entry_ptr->num_of_shared_plmns) &&
         (cell_status == TDSRRC_CSP_CELL_BELOW_EXPECTED));

  tdsrrc_csp_int_data.bar_for_resel_only = !plmns_match;
  tdsrrc_csp_int_data.non_eq_plmn_barred=!plmns_match;

  TDSRRC_MSG1(MSG_LEGACY_HIGH," tdsrrccsp_get_cell_status: returns cell_status %d", cell_status);

  return cell_status;
}

/*===========================================================================

FUNCTION          RRCCSP_GET_CELL_STATUS_DURING_BPLMN_SEARCH

DESCRIPTION       Finds out the suitability of the cell during bplmn search based on the 
                  cell bar status, list of requesed PLMNs etc.
                  
DEPENDENCIES      None

RETURN VALUE      
                  TDSRRC_CSP_CELL_BETTER_THAN_EXPECTED: Cell exceeds expectations.
                  TDSRRC_CSP_CELL_AS_EXPECTED: Cell meets expectations.
                  TDSRRC_CSP_CELL_BELOW_EXPECTED: Cell is below expectations.

SIDE EFFECTS      None
===========================================================================*/
tdsrrc_csp_cell_actual_vs_expected_e_type tdsrrccsp_get_cell_status_during_bplmn_search(
                                                                                  tdsrrc_csp_acq_entry_type* acq_entry_ptr
                                                                                  ,tdsrrc_csp_nw_sharing_entry_type* nw_db_entry_ptr
                                                                                  )
{
  /* Temporary storage for PLMN status */
  tdsrrc_csp_plmn_status_e_type  cell_plmn_status;
  tdsrrc_csp_cell_actual_vs_expected_e_type cell_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
  rrc_plmn_identity_type  hplmn_id;
  boolean wtow_search = FALSE;

#ifdef FEATURE_EQUIVALENT_HPLMN
  boolean is_hplmn_or_ehplmn_found = FALSE;
  uint32 j = 0;
#endif

  uint8 shared_plmn_idx = 0;

  if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
  {
    hplmn_id = tdsrrc_csp_int_data.bplmn_req_list[0].plmn_id;
    wtow_search = FALSE;
  }
  else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
  {
    hplmn_id = tdsrrc_csp_int_data.wtow_bplmn_req_list[0].plmn_id;
    wtow_search = TRUE;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH," Wrong call to this function for scan type %d",tdsrrc_csp_int_data.curr_scan);
    return cell_status;
  }

  do
  {
    if(acq_entry_ptr->is_valid_nw_sharing)
    {
      acq_entry_ptr->plmn_id = nw_db_entry_ptr->shared_plmns_access_info[shared_plmn_idx].plmn_id;
      acq_entry_ptr->status_mask = nw_db_entry_ptr->shared_plmns_access_info[shared_plmn_idx++].status_mask;
    }

#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
    /* NAS requirement is that when Background Service Request is sent by NAS
      * when UE mode is Manual then RRC should populate the available PLMN list
      * only if the PLMNs match. 
      * If UE is in Automatic Mode, then filtering of PLMNs will be done by NAS
      * and RRC should report ALL available PLMNs.
      */
    if(tdsrrc_csp_int_data.service_search_bgnd != SYS_SERVICE_SEARCH_BACKGROUND_MANUAL)
    {
      if(!wtow_search)
      {
        tdsrrccsp_update_background_plmn_list(acq_entry_ptr->plmn_id, acq_entry_ptr->cell_id);
      }
      else
      {
        tdsrrccsp_update_td2td_background_plmn_list(acq_entry_ptr->plmn_id, acq_entry_ptr->cell_id);
      }
    }
#else
    if(!wtow_search)
    {
      tdsrrccsp_update_background_plmn_list(acq_entry_ptr->plmn_id, acq_entry_ptr->cell_id);
    }
    else
    {
      tdsrrccsp_update_td2td_background_plmn_list(acq_entry_ptr->plmn_id, acq_entry_ptr->cell_id);
    }
#endif

    /* Generate PLMN status based on received and requested PLMN IDs */
    if(tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ)
    {
      /* For ANY PLMN Request, we don't care what PLMN is selected */
      cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
    }
    else  /* match bcch and nas plmns */
    {
      if(tdsrrc_bcch_and_nas_plmns_match(acq_entry_ptr->plmn_id, hplmn_id))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN is the same as requested BCCH and NAS PLMN match");
        cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
      }
      else
      {
#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
        if(   (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC)
            || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_MANUAL)
           )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SSB: Checking for EPLMNs");

          if((tdsrrccsp_check_for_eplmn(
                      mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                       tdsrrc_csp_int_data.curr_req_plmn.plmn_id))) == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"NAS PLMN is in the eq PLMN list");
            if((tdsrrccsp_check_for_eplmn(
                        mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                        acq_entry_ptr->plmn_id))) == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH PLMN is in the eq PLMN list");
              cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH PLMN not in eq PLMN list");
              cell_plmn_status = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"NAS PLMN not in eq PLMN list");
            cell_plmn_status = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;
          }
        }
        else
        {
#endif /* FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH */

#ifdef FEATURE_EQUIVALENT_HPLMN
          /* Initialise the is_hplmn_or_ehplmn_found to FALSE */
          is_hplmn_or_ehplmn_found = FALSE;

          /* Convert the current plmn_id to nas plmn id before using NAS function */

          if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN. Loop through %d PLMNs to find a match",tdsrrc_csp_int_data.bplmn_num_req);
            /* Loop through until HPLMN/EHPLMN is found */
            for(j=1; j<tdsrrc_csp_int_data.bplmn_num_req;j++)
            {
              if(tdsrrc_bcch_and_nas_plmns_match(acq_entry_ptr->plmn_id,
                                      tdsrrc_csp_int_data.bplmn_req_list[j].plmn_id))
              {
                if(tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn(acq_entry_ptr->plmn_id))
                {
                  is_hplmn_or_ehplmn_found = TRUE;
                  break;
                }
              }
            }
          }
          else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN. Loop through %d PLMNs to find a match",tdsrrc_csp_int_data.wtow_bplmn_num_req);
            /* Loop through until HPLMN/EHPLMN is found */
            for(j=1; j<tdsrrc_csp_int_data.wtow_bplmn_num_req;j++)
            {
              if(tdsrrc_bcch_and_nas_plmns_match(acq_entry_ptr->plmn_id,
                                   tdsrrc_csp_int_data.wtow_bplmn_req_list[j].plmn_id))
              {
                if(tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn(acq_entry_ptr->plmn_id))
                {
                   is_hplmn_or_ehplmn_found = TRUE;
                   break;
                }
              }
            }
          }

          /* HPLMN/EHPLMN is found */
          if(is_hplmn_or_ehplmn_found)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"EHPLMN: HPLMN/EHPLMN is found ");
            cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
          }
          else  /* HPLMN/EHPLMN is not found */
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"EHPLMN: HPLMN/EHPLMN is not found ");
            cell_plmn_status = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;
          }
#else
          cell_plmn_status = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;
#endif
#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
        }
#endif
      } /* end eplmn */
    } /* end match bcch and nas plmns */

    /* Compare the cell based on status mask formed from SIBs received for this cell */

    /* Decide if this cell is good enough - If actual cell status is lower than the one
      * expected by Selection Controller, this cell is not selected.
      */
    cell_status = tdsrrccsp_compare_expected_cell(cell_plmn_status,
                                       acq_entry_ptr->status_mask,
                                       tdsrrc_csp_int_data.curr_type_acq);

  }
  while((acq_entry_ptr->is_valid_nw_sharing) && 
         (shared_plmn_idx < nw_db_entry_ptr->num_of_shared_plmns) &&
         (cell_status == TDSRRC_CSP_CELL_BELOW_EXPECTED));

  return cell_status;
}


/*===========================================================================

FUNCTION          TDSRRCCSP_IS_PLMN_MATCH_FOR_AUTO_BPLMN_SEARCH

DESCRIPTION       Finds out the suitability of the cell during bplmn search based on the 
                  cell bar status, list of requesed PLMNs etc.
                  
DEPENDENCIES      None

RETURN VALUE      
                  TDSRRC_CSP_CELL_BETTER_THAN_EXPECTED: Cell exceeds expectations.
                  TDSRRC_CSP_CELL_AS_EXPECTED: Cell meets expectations.
                  TDSRRC_CSP_CELL_BELOW_EXPECTED: Cell is below expectations.

SIDE EFFECTS      None
===========================================================================*/
tdsrrc_csp_cell_actual_vs_expected_e_type tdsrrccsp_is_plmn_match_for_auto_bplmn_search(
                                                                                  tdsrrc_csp_acq_entry_type* acq_entry_ptr
                                                                                  ,tdsrrc_csp_nw_sharing_entry_type* nw_db_entry_ptr
																				  )
{
  /* Temporary storage for PLMN status */
  tdsrrc_csp_plmn_status_e_type  cell_plmn_status;
  tdsrrc_csp_cell_actual_vs_expected_e_type cell_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
  rrc_plmn_identity_type  hplmn_id;
  boolean wtow_search = FALSE;

#ifdef FEATURE_EQUIVALENT_HPLMN
  boolean is_hplmn_or_ehplmn_found = FALSE;
  uint32 j = 0;
#endif

  uint8 shared_plmn_idx = 0;

  if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
  {
    hplmn_id = tdsrrc_csp_int_data.bplmn_req_list[0].plmn_id;
    wtow_search = FALSE;
  }
  else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
  {
    hplmn_id = tdsrrc_csp_int_data.wtow_bplmn_req_list[0].plmn_id;
    wtow_search = TRUE;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH," Wrong call to this function for scan type %d",tdsrrc_csp_int_data.curr_scan);
    return cell_status;
  }

  do
  {
    if(acq_entry_ptr->is_valid_nw_sharing)
    {
      acq_entry_ptr->plmn_id = nw_db_entry_ptr->shared_plmns_access_info[shared_plmn_idx].plmn_id;
      acq_entry_ptr->status_mask = nw_db_entry_ptr->shared_plmns_access_info[shared_plmn_idx++].status_mask;
    }

#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
    /* NAS requirement is that when Background Service Request is sent by NAS
      * when UE mode is Manual then RRC should populate the available PLMN list
      * only if the PLMNs match. 
      * If UE is in Automatic Mode, then filtering of PLMNs will be done by NAS
      * and RRC should report ALL available PLMNs.
      */
    if(tdsrrc_csp_int_data.service_search_bgnd != SYS_SERVICE_SEARCH_BACKGROUND_MANUAL)
    {
      if(!wtow_search)
      {
        tdsrrccsp_update_background_plmn_list(acq_entry_ptr->plmn_id, acq_entry_ptr->cell_id);
      }
      else
      {
        tdsrrccsp_update_td2td_background_plmn_list_for_sib_incomplete(acq_entry_ptr->plmn_id, acq_entry_ptr->cell_id);
      }
    }
#else
    if(!wtow_search)
    {
      tdsrrccsp_update_background_plmn_list(acq_entry_ptr->plmn_id, acq_entry_ptr->cell_id);
    }
    else
    {
      tdsrrccsp_update_td2td_background_plmn_list_for_sib_incomplete(acq_entry_ptr->plmn_id, acq_entry_ptr->cell_id);
    }
#endif

    /* Generate PLMN status based on received and requested PLMN IDs */
    if(tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ)
    {
      /* For ANY PLMN Request, we don't care what PLMN is selected */
      cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
    }
    else  /* match bcch and nas plmns */
    {
      if(tdsrrc_bcch_and_nas_plmns_match(acq_entry_ptr->plmn_id, hplmn_id))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN is the same as requested BCCH and NAS PLMN match");
        cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
      }
      else
      {
#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
        if(   (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC)
            || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_BACKGROUND_MANUAL)
            || (tdsrrc_csp_int_data.service_search_bgnd == SYS_SERVICE_SEARCH_PRIORITY_PLMN)
           )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SSB: Checking for EPLMNs");

          if((tdsrrccsp_check_for_eplmn(
                      mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                       tdsrrc_csp_int_data.curr_req_plmn.plmn_id))) == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"NAS PLMN is in the eq PLMN list");
            if((tdsrrccsp_check_for_eplmn(
                        mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                        acq_entry_ptr->plmn_id))) == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH PLMN is in the eq PLMN list");
              cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH PLMN not in eq PLMN list");
              cell_plmn_status = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"NAS PLMN not in eq PLMN list");
            cell_plmn_status = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;
          }
        }
        else
        {
#endif /* FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH */

#ifdef FEATURE_EQUIVALENT_HPLMN
          /* Initialise the is_hplmn_or_ehplmn_found to FALSE */
          is_hplmn_or_ehplmn_found = FALSE;

          /* Convert the current plmn_id to nas plmn id before using NAS function */

          if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
          {
            /* Loop through until HPLMN/EHPLMN is found */
            for(j=1; j<tdsrrc_csp_int_data.bplmn_num_req;j++)
            {
              if(tdsrrc_bcch_and_nas_plmns_match(acq_entry_ptr->plmn_id,
                                      tdsrrc_csp_int_data.bplmn_req_list[j].plmn_id))
              {
                if(tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn(acq_entry_ptr->plmn_id))
                {
                  is_hplmn_or_ehplmn_found = TRUE;
                  break;
                }
              }
            }
          }
          else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
          {
            /* Loop through until HPLMN/EHPLMN is found */
            for(j=1; j<tdsrrc_csp_int_data.wtow_bplmn_num_req;j++)
            {
              if(tdsrrc_bcch_and_nas_plmns_match(acq_entry_ptr->plmn_id,
                                   tdsrrc_csp_int_data.wtow_bplmn_req_list[j].plmn_id))
              {
                if(tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn(acq_entry_ptr->plmn_id))
                {
                   is_hplmn_or_ehplmn_found = TRUE;
                   break;
                }
              }
            }
          }

          /* HPLMN/EHPLMN is found */
          if(is_hplmn_or_ehplmn_found)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"EHPLMN: HPLMN/EHPLMN is found ");
            cell_plmn_status = TDSRRC_CSP_PLMN_SAME_AS_REQ;
          }
          else  /* HPLMN/EHPLMN is not found */
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"EHPLMN: HPLMN/EHPLMN is not found ");
            cell_plmn_status = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;
          }
#else
          cell_plmn_status = TDSRRC_CSP_PLMN_DIFF_THAN_REQ;
#endif
#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
        }
#endif
      } /* end eplmn */
    } /* end match bcch and nas plmns */

    /* Compare the cell based on status mask formed from SIBs received for this cell */

    /* Decide if this cell is good enough - If actual cell status is lower than the one
      * expected by Selection Controller, this cell is not selected.
      */
    if (cell_plmn_status == TDSRRC_CSP_PLMN_SAME_AS_REQ)
        cell_status = TDSRRC_CSP_CELL_AS_EXPECTED;
  }
  while((acq_entry_ptr->is_valid_nw_sharing) && 
         (shared_plmn_idx < nw_db_entry_ptr->num_of_shared_plmns) &&
         (cell_status == TDSRRC_CSP_CELL_BELOW_EXPECTED));

  return cell_status;
}												



boolean 
	tdsrrccsp_handle_bplmn_sib_fail_hit(uint8 index)
{
  boolean move_to_next_freq = FALSE;
  rrc_plmn_identity_type sib_read_plmn_id;
  tdsrrc_MasterInformationBlock *mib_ptr = NULL;
  boolean is_s_criteria_met = FALSE;
  tdsrrc_SysInfoType3 *sib3_ptr = NULL;
  tdsrrc_csp_cell_actual_vs_expected_e_type cell_status = TDSRRC_CSP_CELL_BELOW_EXPECTED;
  
  if (tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].is_MIB_recv == FALSE)
  {
#if 0
     if((tdsrrc_csp_int_data.num_cpid_det > 0) && ((tdsrrc_csp_int_data.curr_acq_count+1)
		>= tdsrrc_csp_int_data.num_cpid_det))
#endif
     {
          tdsrrc_csp_int_data.bplmn_sib_try_next_best_cell = TRUE;
     }
     TDSRRC_MSG2(MSG_LEGACY_HIGH,"BPLMN cell %d,%d SIB fail hit MAX but MIB not exist",tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].freq,tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].cpid);	
	 return FALSE;
  }
  sib_read_plmn_id = tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].plmn_id;
  sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrcsib_bplmn_return_sib_for_cell(
                                         tdsrrc_SIB3);
  if (sib3_ptr != NULL)
  {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"BPLMN cell d%,d% SIB fail hit MAX and SIB3 exist",tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].freq,tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].cpid);	
	  tdsrrc_csp_int_data.curr_acq_entry.cell_id = tdsrrc_translate_cell_id(&sib3_ptr->cellIdentity);
  }
  
  mib_ptr = (tdsrrc_MasterInformationBlock *) tdsrrcsib_bplmn_return_sib_for_cell(tdsrrc_MIB);
  
  TDSRRC_MSG2(MSG_LEGACY_HIGH, "index=%d,mib_ptr=0x%x",index,(uint32) mib_ptr);
  
  if (mib_ptr)
  {
    if(tdsrrc_is_nw_shared(mib_ptr))
    {
	   tdsrrccsp_find_shared_plmns(
									mib_ptr,
									tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].plmn_id,
								   &tdsrrc_csp_int_data.curr_nw_db_entry
									);
	 
	       /* Prioritize the NAS requested PLMN/HPLMN based on the current scan type  This is because 
		     * our search for good PLMNs is sequential in the list of shared PLMNs and if there is a low priority
		     * PLMN (eplmn) available ahead of the needed PLMN then we may end up in camping on it even though
		     * the requested PLMN is available */
	   tdsrrccsp_prioritize_the_list_of_shared_plmns();
	   tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing = TRUE;
	   tdsrrc_csp_int_data.curr_acq_entry.plmn_id = tdsrrc_csp_int_data.curr_nw_db_entry.shared_plmns_access_info[0].plmn_id;
      sib_read_plmn_id = tdsrrc_csp_int_data.curr_acq_entry.plmn_id;		
    }
    else
    {
	   tdsrrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing = FALSE;
    }
  }

#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN
  if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
  {
      /* Fill the tmp_acq_entry with SIB data */
     tdsrrc_csp_int_data.curr_acq_entry.plmn_id     = sib_read_plmn_id;
     tdsrrc_csp_int_data.curr_acq_entry.freq        = (tdsrrc_freq_type)tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].freq;
     tdsrrc_csp_int_data.curr_acq_entry.cpid    = (tdsrrc_cpid_type)tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].cpid;
     tdsrrc_csp_int_data.curr_acq_entry.status_mask = 0;

         /* At least Emergency calls allowed on this cell */
         /* Store the Current Acq. entry in Acq. database. */
     if( tdsrrccsp_add_acq_db_and_nw_db_entries(&tdsrrc_csp_int_data.curr_acq_entry, 
                                              &tdsrrc_csp_int_data.curr_nw_db_entry) == FAILURE)
     {
         ERR_FATAL("Failure adding acq. entry",0,0,0);
     }

         /* For dual-mode, call the utility to update internal available Background PLMN list.
	           * Note the utility has the intelligence to decide which PLMNs to add to this list.
                  */
     tdsrrccsp_update_td2td_background_plmn_list_for_sib_incomplete(tdsrrc_csp_int_data.curr_acq_entry.plmn_id, tdsrrc_csp_int_data.curr_acq_entry.cell_id);

#if 0
	 if((tdsrrc_csp_int_data.num_cpid_det > 0) && ((tdsrrc_csp_int_data.curr_acq_count+1)
		>= tdsrrc_csp_int_data.num_cpid_det))
#endif
     {
         move_to_next_freq = TRUE;
     }
	 return FALSE;
  }
#endif
  /*
  if (sib3_ptr != NULL)
  {
       if(tdsrrccsp_find_access_control(&access_control, &rrc_dsac_info,sib_read_plmn_id,
                                   (tdsrrc_int_cell_id_type)tdsrrc_csp_int_data.curr_acq_entry.cell_id 
                                   ,sib_read_plmn_id   //Leo ????
                                   )
                                   == FAILURE)
       {
           ERR_FATAL("Failure finding cell access status in CSP sstate %d",next_substate,0,0);
       }
  }
*/
  tdsrrc_csp_int_data.curr_acq_entry.plmn_id = sib_read_plmn_id;
  if( tdsrrccsp_add_acq_db_and_nw_db_entries(&tdsrrc_csp_int_data.curr_acq_entry, 
                                              &tdsrrc_csp_int_data.curr_nw_db_entry) == FAILURE)
  {
      ERR_FATAL("Failure adding acq. entry",0,0,0);
  }

  if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
  {
    /* For dual-mode, call the utility to update internal available Background PLMN list.
         * Note the utility has the intelligence to decide which PLMNs to add to this list.
         */
     tdsrrccsp_update_td2td_background_plmn_list_for_sib_incomplete(tdsrrc_csp_int_data.curr_acq_entry.plmn_id, tdsrrc_csp_int_data.curr_acq_entry.cell_id);
  } 
  else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
  {
                      
      if (sib3_ptr != NULL)
      {
         is_s_criteria_met = tdsrrccsp_is_s_criteria_met(tdsrrc_csp_int_data.curr_acq_entry.plmn_id, tdsrrc_csp_int_data.curr_acq_entry.cell_id);
      }
	  if ((sib3_ptr == NULL) || (is_s_criteria_met == TRUE))
      {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"BPLMN cell d%,d% SIB fail hit MAX and S >0",tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].freq,tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].cpid);	
		  cell_status = tdsrrccsp_is_plmn_match_for_auto_bplmn_search(&tdsrrc_csp_int_data.curr_acq_entry
																		   ,&tdsrrc_csp_int_data.curr_nw_db_entry
																	 );
		  if(cell_status != TDSRRC_CSP_CELL_BELOW_EXPECTED)
	      {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD: HPLMN found!. Stop srch. Snd listcnf to NAS. Snd sleep ind to L1");
               /*Always update available PLMN if the PLMN match with the one NAS request*/
			   TDSRRC_MSG0(MSG_LEGACY_HIGH,"SSB: Cell is as expected. Updting Avail PLMN List");
               tdsrrccsp_update_td2td_background_plmn_list_for_sib_incomplete(tdsrrc_csp_int_data.curr_acq_entry.plmn_id,
                                                        tdsrrc_csp_int_data.curr_acq_entry.cell_id);
               tdsrrc_csp_int_data.bplmn_sib_try_next_best_cell = FALSE;

               tdsrrccsp_reset_bplmn_list_search_state(TD2TD_SEARCH_STOP, TRUE, TRUE);

               return TRUE;
           }
		   else
		   {
		      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD Auto BPLMN: plmn below expected,continue to find HPLMN");
#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
              /* Update available PLMN if not BACKGROUND_MANUAL and S>0*/
              if(tdsrrc_csp_int_data.service_search_bgnd != SYS_SERVICE_SEARCH_BACKGROUND_MANUAL)
              {
                 TDSRRC_MSG0(MSG_LEGACY_HIGH,"SSB: Cell is not as expected. Updting Avail PLMN List for non Manual UE mode");
                 tdsrrccsp_update_td2td_background_plmn_list_for_sib_incomplete(tdsrrc_csp_int_data.curr_acq_entry.plmn_id,
                                                        tdsrrc_csp_int_data.curr_acq_entry.cell_id);
              }
#else
              /*Always update available PLMN if FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH is not enabled*/
              tdsrrccsp_update_td2td_background_plmn_list_for_sib_incomplete(tdsrrc_csp_int_data.curr_acq_entry.plmn_id,
                                                        tdsrrc_csp_int_data.curr_acq_entry.cell_id);
#endif			  
#if 0
              if((tdsrrc_csp_int_data.num_cpid_det > 0) && ((tdsrrc_csp_int_data.curr_acq_count+1)
                                >= tdsrrc_csp_int_data.num_cpid_det))
#endif
              {
                   move_to_next_freq = TRUE;
		      }
		   }
      }
	  else
	  {
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"BPLMN cell d%,d% SIB fail hit MAX but S < 0",tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].freq,tdsrrc_csp_int_data.bplmn_sib_fail_cell_list.bplmn_sib_fail_cell[index].cpid);	
#if 0
         if((tdsrrc_csp_int_data.num_cpid_det > 0) && ((tdsrrc_csp_int_data.curr_acq_count+1)
                                >= tdsrrc_csp_int_data.num_cpid_det))
#endif
         {
            move_to_next_freq = TRUE;
		 }
	  }
  } 
  if (move_to_next_freq)
  {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN cellSIB fail hit MAX and no more suit cells on same freq");	
      tdsrrc_csp_int_data.bplmn_sib_try_next_best_cell = TRUE;
  }
  return FALSE;
}


/*===========================================================================

FUNCTION          RRC_CSP_ADD_FREQ_TO_AVAILABLE_FREQ_LIST

DESCRIPTION       Adds a given frequency to avaialble frequency list. All the frequencies which are in the 
                  predefined range of these frequencies are skipped while scanning later.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_add_freq_to_available_freq_list(tdsrrc_freq_type freq)
{
  uint32 i;
  /* Don't allow duplicate frequencies in available frequencies list */
  boolean  freq_in_list = FALSE;

#if defined(FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  detected_frequency_type detected_tds_frequency;
#endif

  /* Avoid duplicate entry of the frequency in the list */
  for(i=0; i<tdsrrc_csp_curr_select_data.num_available_freq; i++)
  {
      if(freq == tdsrrc_csp_curr_select_data.available_freq_list[i])
      {
          freq_in_list = TRUE;
          break;
      }
  }

  if(!freq_in_list)
  {
    if(tdsrrc_csp_curr_select_data.num_available_freq < TDSRRC_MAX_NUM_FREQ - 1)
    {

      tdsrrc_csp_curr_select_data.available_freq_list[tdsrrc_csp_curr_select_data.num_available_freq++] =
        freq;

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"frequency %d band 0x%x added to available PLMN list",freq,tdsrrc_csp_curr_select_data.current_band);
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Max Avail Freq List reached %d. Resetting num_available_freq to 0",tdsrrc_csp_curr_select_data.num_available_freq);
      tdsrrc_csp_curr_select_data.num_available_freq = 0;


      tdsrrc_csp_curr_select_data.available_freq_list[tdsrrc_csp_curr_select_data.num_available_freq++] =
        freq;

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"frequency %d band 0x%x added to available PLMN list",freq,tdsrrc_csp_curr_select_data.current_band);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"freq %d already in list. Not adding",freq);
  }

#if defined(FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  /*First convet the UARFCN to frequency */
  detected_tds_frequency.frequency = tdsrrccsp_derive_freq_from_uarfcn(freq,tdsrrc_csp_curr_select_data.current_band);
  detected_tds_frequency.bandwidth = TDSRRC_CSP_TDS_BANDWITH_IN_100KHZ;
  detected_tds_frequency.rat = SYS_RAT_TDS_RADIO_ACCESS;
  tdsrrc_csp_add_frequency_to_detected_cell_list(&detected_tds_frequency);
#endif

}

/*===========================================================================

FUNCTION          RRCCSP_DOES_MCC_BELONG_TO_US

DESCRIPTION       This function returns TRUE if the passed PLMN belongs to US
                  otherwise it returns FALSE.
                                    
DEPENDENCIES      None

RETURN VALUE      TRUE/FALSE
                  
SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrccsp_does_mcc_belong_to_US(rrc_plmn_identity_type plmn_id)
{
  if(((TDSRRC_CSP_GET_INT_MCC(plmn_id) >= TDSUSA_MCC_START_RANGE)
        && (TDSRRC_CSP_GET_INT_MCC(plmn_id) <= TDSUSA_MCC_END_RANGE))
     || (TDSRRC_CSP_GET_INT_MCC(plmn_id) == TDSUSA_MCC1))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCC %d belongs to US",TDSRRC_CSP_GET_INT_MCC(plmn_id));
    return TRUE;
  } 
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCC %d does NOT belong to US",TDSRRC_CSP_GET_INT_MCC(plmn_id));
    return FALSE;
  }
} /* tdsrrccsp_does_mcc_belong_to_US */



#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
/*===========================================================================

FUNCTION: RRC_CSP_SEND_SVC_STATUS_TO_MM

DESCRIPTION:

DEPENDENCIES: NONE

RETURN VALUE: NONE

SIDE EFFECTS: NONE
=============================================================================*/
void tdsrrccsp_send_svc_status_to_mm(sys_srv_status_e_type svc_st, 
                                   sys_radio_access_tech_e_type rat, 
                                   sys_signal_strength_type sig_st )
{
  if(tds_ds_tuneaway_status_change_ind.tuneaway_status == FALSE)
  {
  mm_cmd_type *mm_buf_ptr;

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_CONNECTED_MODE_SERVICE_STATUS_IND)) == NULL)
  {
    ERR_FATAL("No MM buffer for RRC_CONNECTED_MODE_SERVICE_STATUS_IND ",0,0,0);
  }

#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.as_id = tdsrrc_get_as_id();
#endif

  mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.service_status = svc_st;
  mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.selected_plmn.plmn_type = RRC_GSM_MAP_PLMN_SEL;
#ifdef  FEATURE_OOSC_USER_ACTION
  mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.is_connected_mode_oos = TRUE;
  if(svc_st == SYS_SRV_STATUS_NONE || svc_st == SYS_SRV_STATUS_SRV)
  {
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.is_connected_mode_oos = FALSE;
  }
  if(svc_st == SYS_SRV_STATUS_NONE)
  {
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.service_status = SYS_SRV_STATUS_NO_SRV;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"OOSC:Connected mode OOS is sent as %d to MM",mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.is_connected_mode_oos);
#endif /*FEATURE_OOSC_USER_ACTION*/

  if(mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.service_status == SYS_SRV_STATUS_NO_SRV)
  {
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.selected_plmn.plmn_id.mcc[0] = 0x0F;
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.selected_plmn.plmn_id.mcc[1] = 0x0F;
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.selected_plmn.plmn_id.mcc[2] = 0x0F;
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.selected_plmn.plmn_id.mnc[0] = 0x0F;
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.selected_plmn.plmn_id.mnc[1] = 0x0F;
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.selected_plmn.plmn_id.mnc[2] = 0x0F;
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.selected_plmn.plmn_id.num_mnc_digits = 3;
  }
  else
  {
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.selected_plmn.plmn_id = 
                                                      tdsrrc_csp_int_data.curr_acq_entry.plmn_id;
  }
  
  mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.rat                   = rat; 

  mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.signal_strength       = sig_st;
  if(sig_st <= 95)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"signal strength (%d) >= -95db is considered as SYS_SIGNAL_QUALITY_HIGH",sig_st);
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.signal_quality = SYS_SIGNAL_QUALITY_HIGH;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"signal strength (%d) < -95db is considered as SYS_SIGNAL_QUALITY_LOW",sig_st);
    mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.signal_quality = SYS_SIGNAL_QUALITY_LOW;
  }


  TDSRRC_MSG3(MSG_LEGACY_HIGH,"SVC_ST: Svc st %d, plmn mcc %d, mnc %d",mm_buf_ptr->cmd.rrc_conn_mode_service_status_ind.service_status,
                                         TDSRRC_CSP_GET_INT_MCC(tdsrrc_csp_int_data.curr_acq_entry.plmn_id), 
                                         TDSRRC_CSP_GET_INT_MNC(tdsrrc_csp_int_data.curr_acq_entry.plmn_id));

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  mm_put_cmd(mm_buf_ptr);
  }
} /* rrc_csp_send_service_status_to_cm */
#endif /* FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE */


/*===========================================================================

FUNCTION          RRCCSP_IS_BAND_ALREADY_ADDED_TO_PRIORITIZED_LIST

DESCRIPTION       Determines whether a band given is already present in the prioritized band list.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
boolean 
tdsrrccsp_is_band_already_added_to_prioritized_list(tdsrrc_csp_band_class_type freq_band, uint8 num_of_bands)
{
  uint8 i;

  for(i = 0; i < num_of_bands; i++)
  {
    if(freq_band == tdsrrc_csp_curr_select_data.prioritized_bands[i])
    {
      return TRUE;
    }
  }

  return FALSE;
}


/*===========================================================================

FUNCTION          RRCCSP_GET_SERVING_FREQUENCY

DESCRIPTION       This function gets the frequency of the serving cell 

DEPENDENCIES
                  None.

RETURN VALUE
                  Serving cell's frequency.

SIDE EFFECTS      None.
===========================================================================*/
tdsrrc_freq_type tdsrrccsp_get_serving_frequency(void)
{
  return tdsrrc_csp_int_data.curr_acq_entry.freq;
}

/*===========================================================================

FUNCTION          RRCCSP_GET_NEIGHBOR_FREQUENCY

DESCRIPTION       This function gets the frequency of the neighbor cell UE is
                  reselecting to
DEPENDENCIES
                  None.

RETURN VALUE
                  Neighbor frequency.

SIDE EFFECTS      None.
===========================================================================*/
tdsrrc_freq_type tdsrrccsp_get_neighbor_frequency(void)
{
  return tdsrrc_csp_int_data.new_cell_acq_entry.freq;
}

/*===========================================================================

FUNCTION          RRCCSP_GET_CURR_NAS_REQUESTED_PLMN

DESCRIPTION       This function returns the specific PLMN on which NAS has 
                  requested RRC to camp.
DEPENDENCIES
                  None.

RETURN VALUE
                  NAS requested PLMN.

SIDE EFFECTS      None.
===========================================================================*/
rrc_plmn_identity_type tdsrrccsp_get_curr_nas_requested_plmn(void)
{
  if(TD2TD_SEARCH_RESUME == tdsrrccsp_bplmn_srch_in_progress())
  {
    return tdsrrc_csp_int_data.prev_curr_req_plmn.plmn_id;
  }
  else
  {
    return tdsrrc_csp_int_data.curr_req_plmn.plmn_id;
  }
}

#ifdef FEATURE_OOSC_USER_ACTION
#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
/*===========================================================================

FUNCTION        RRCCSP_CHECK_AND_SEND_UOOSC_END_IND

DESCRIPTION     This function is called when RRC receives STOP TDS request from NAS
                This function checks if UOOSC search was in progress and sends 
                RRC_CONNECTED_MODE_SERVICE_STATUS_IND with is_connected_mode_oos as FALSE

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrccsp_check_and_send_uoosc_end_ind
(
  void
)
{
  tdsrrc_state_e_type curr_rrc_state = tdsrrc_get_state();
  if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN) &&
    ((curr_rrc_state == TDSRRC_STATE_CELL_DCH)||
     (curr_rrc_state == TDSRRC_STATE_CELL_FACH)||
     (curr_rrc_state == TDSRRC_STATE_CELL_PCH)||
     (curr_rrc_state == TDSRRC_STATE_URA_PCH)))
  {
    /*UOOSC scan was in progress, Send end of UOOSC*/
    tdsrrccsp_send_svc_status_to_mm(SYS_SRV_STATUS_NONE, SYS_RAT_TDS_RADIO_ACCESS, 0);
  }
}
#endif
#endif

/*===========================================================================

FUNCTION          RRC_CSP_CELL_TRANS_CNF_RCVD_AT_LLC

DESCRIPTION       This function is called by LLC on reception of cell_trans_cnf from L1

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_cell_trans_cnf_rcvd_at_llc
(
  void
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC received Cell transition CNF");
  tdsrrc_csp_int_data.cel_trans_cnf_recvd_at_llc = TRUE;
}

/*===========================================================================

FUNCTION          RRCCSP_CHECK_INITIATE_CELL_SELECTION_HANDLING

DESCRIPTION       This function is called to check if CSP is in proper substate to handle 
                         TDSRRC_INITIATE_CELL_SELECTION_IND to transition to DCH or FACH.

DEPENDENCIES      None

RETURN VALUE      TRUE/FALSE

SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrccsp_check_initiate_cell_selection_handling
(
  void
)
{
  tdsrrc_csp_substate_e_type  curr_substate = tdsrrccsp_get_csp_substate();
  if((curr_substate == TDSRRCCSP_CAMPED_SUIT_CELL)
    || (curr_substate == TDSRRCCSP_CAMPED_ACCEPT_CELL))
  {
    return TRUE;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CSP not in a state to accept cell selection req csp state %d ",curr_substate);
  return FALSE;
}

/*===========================================================================

FUNCTION          RRC_CSP_IS_RESELECTION_CLEARED

DESCRIPTION       This function is called on reception of Chan config req for N-BCCH 
               to check if DL WEAK IND was received for this cell and reselection data has been cleared.

DEPENDENCIES      None

RETURN VALUE      TRUE : if reselection is cleared.
                  FLASE: otherwise

SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrccsp_is_reselection_cleared
(
  void
)
{
  boolean resel_cleared = FALSE;
  tdsrrc_csp_substate_e_type curr_csp_substate = tdsrrccsp_get_csp_substate();

  if((curr_csp_substate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_TRANS_CNF) ||
     (curr_csp_substate == TDSRRCCSP_SUIT_CELL_RESELECT_WT_TRANS_CNF))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reselection cancelled and CSP is waiting for transition CNF");
    resel_cleared = TRUE;
  }
  /* For PREEMPTION in DCH/FACH, reset reselection data and set as TDSRRC_CSP_UNLOCK_BY_SCAN
   * For PREEMPTION in PCH/IDLE, reset reselection data only
   */
  else if ((tdsrrc_csp_int_data.new_cell_acq_entry.freq == 0)
           &&(tdsrrc_csp_int_data.new_cell_acq_entry.cpid == 0)
          )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Reselection data has been reset");
    resel_cleared = TRUE;
  }
  return resel_cleared;
}

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
/*===========================================================================

FUNCTION          RRCCSP_CHECK_IF_CAMPED_NORMALLY

DESCRIPTION       This function checks if UE is camped normally, 

DEPENDENCIES      None

RETURN VALUE      TRUE if camped normally
                          FALSE if camped on any cell

SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrccsp_check_if_camped_normally
(
  void
)
{
  boolean camped_normally = FALSE;
  if(tdsrrccsp_bplmn_srch_in_progress() == TD2TD_SEARCH_RESUME)
  {
    if(tdsrrc_csp_int_data.prev_curr_req_plmn.plmn_type == RRC_GSM_MAP_SPECIFIC_PLMN_REQ)
    {
      camped_normally = TRUE;
    }
  }
  else
  {
    if(tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_SPECIFIC_PLMN_REQ)
    {
      camped_normally = TRUE;
    }
  }
  return (camped_normally);
}
#endif

#ifdef FEATURE_RAT_PRIORITY_LIST
/*===========================================================================

FUNCTION          RRC_EXTRACT_TDS_BAND_PREF_FROM_RAT_PRI_LIST

DESCRIPTION       This function extracts the TDS band pref from RAT PRI List

DEPENDENCIES      None

RETURN VALUE      TDS band pref

SIDE EFFECTS      None
===========================================================================*/
sys_band_mask_type tdsrrccsp_extract_tds_band_pref_from_rat_pri_list
(
  sys_rat_pri_list_info_s_type *rat_pri_list
)
{
  uint8 rat_idx = 0;
  sys_band_mask_type t_band_pref = 0;

  for(rat_idx =0;rat_idx < rat_pri_list->num_items &&
               rat_idx< SYS_MAX_ACQ_PREF_RAT_LIST_NUM ; rat_idx ++)
  {
    if(rat_pri_list->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_TDS)
    {
#ifdef LTE_BAND_NUM
      t_band_pref |= rat_pri_list->priority_list_info[rat_idx].band_cap.chgwt_band_cap;
#else
      t_band_pref |= rat_pri_list->priority_list_info[rat_idx].band_cap;
#endif
    }
  }

  if (tdsrrcmisc_is_da_backoff_on())
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"DA Backoff is on,remove band F");
     t_band_pref &= ~(SYS_BAND_MASK_TDS_BANDF);
  }
  
  return t_band_pref;
}

/*===========================================================================

FUNCTION          RRC_EXTRACT_GW_BAND_PREF_FROM_RAT_PRI_LIST

DESCRIPTION       This function extracts the GW band pref from RAT PRI List

DEPENDENCIES      None

RETURN VALUE      GW band pref

SIDE EFFECTS      None
===========================================================================*/
sys_band_mask_type tdsrrccsp_extract_gw_band_pref_from_rat_pri_list
(
  sys_rat_pri_list_info_s_type *rat_pri_list
)
{
  uint8 rat_idx = 0;
  sys_band_mask_type gw_band_pref = 0;
  
  for(rat_idx =0;rat_idx < rat_pri_list->num_items &&
               rat_idx< SYS_MAX_ACQ_PREF_RAT_LIST_NUM ; rat_idx ++)
  {
#ifdef FEATURE_LOCAL_GWBAND_HAS_W
    if(rat_pri_list->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_WCDMA)
    {
#ifdef LTE_BAND_NUM
      gw_band_pref |= rat_pri_list->priority_list_info[rat_idx].band_cap.chgwt_band_cap;
#else
      gw_band_pref |= rat_pri_list->priority_list_info[rat_idx].band_cap;
#endif
    }
#endif /* FEATURE_LOCAL_GWBAND_HAS_W */
    if(rat_pri_list->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_GSM)
    {
#ifdef LTE_BAND_NUM
      gw_band_pref |= rat_pri_list->priority_list_info[rat_idx].band_cap.chgwt_band_cap;
#else
      gw_band_pref |= rat_pri_list->priority_list_info[rat_idx].band_cap;
#endif
    }
  }
  
  return gw_band_pref;
}

/*===========================================================================

FUNCTION          RRC_EXTRACT_TDS_BAND_PREF_FROM_SERVICE_REQ

DESCRIPTION       This function extracts the TDS band pref from RAT PRI List of service_req

DEPENDENCIES      None

RETURN VALUE      TDS band pref

SIDE EFFECTS      None
===========================================================================*/
sys_band_mask_type tdsrrccsp_extract_tds_band_pref_from_service_req
(
  sys_rat_pri_list_info_s_type *rat_pri_list
)
{
  uint8 rat_idx = 0;
  sys_band_mask_type t_band_pref;
  sys_band_mask_type tdscdma_complete_band_mask;

  t_band_pref = 0;

  tdscdma_complete_band_mask = tdsrrccsp_get_sys_tdscdma_band_mask();

  for(rat_idx =0;rat_idx < rat_pri_list->num_items &&
               rat_idx< SYS_MAX_ACQ_PREF_RAT_LIST_NUM ; rat_idx ++)
  {
    if(rat_pri_list->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_TDS)
    {
      /* ZZTBD: check comment in sys_priority_list_info_s_type */
      if(!rat_pri_list->priority_list_info[rat_idx].bst_rat_acq_required)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"BST: TDSCDMA RAT ACQ is indicated as not required ");
      }
#ifdef LTE_BAND_NUM
      t_band_pref = rat_pri_list->priority_list_info[rat_idx].bst_band_cap.chgwt_band_cap;
#else
      t_band_pref = rat_pri_list->priority_list_info[rat_idx].bst_band_cap;
#endif
    }
  }

  t_band_pref &= tdscdma_complete_band_mask;

  if (tdsrrcmisc_is_da_backoff_on())
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"DA Backoff is on,remove band F");
     t_band_pref &= ~(SYS_BAND_MASK_TDS_BANDF);
  }

  return t_band_pref;
}

/*===========================================================================

FUNCTION          RRC_EXTRACT_GW_BAND_PREF_FROM_SERVICE_REQ

DESCRIPTION       This function extracts the GW band pref from RAT PRI List of service_req

DEPENDENCIES      None

RETURN VALUE      GW band pref

SIDE EFFECTS      None
===========================================================================*/
sys_band_mask_type tdsrrccsp_extract_gw_band_pref_from_service_req
(
  sys_rat_pri_list_info_s_type *rat_pri_list
)
{
  uint8 rat_idx = 0;
  sys_band_mask_type w_band_pref, g_band_pref,wcdma_complete_band_mask;
  w_band_pref = g_band_pref = 0;

  wcdma_complete_band_mask = tdsrrccsp_get_sys_wcdma_band_mask();

  for(rat_idx =0;rat_idx < rat_pri_list->num_items &&
               rat_idx< SYS_MAX_ACQ_PREF_RAT_LIST_NUM ; rat_idx ++)
  {
#ifdef FEATURE_LOCAL_GWBAND_HAS_W
    if(rat_pri_list->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_WCDMA)
    {
      if(!rat_pri_list->priority_list_info[rat_idx].bst_rat_acq_required)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"BST: TDS RAT ACQ is indicated as not required ");
      }
#ifdef LTE_BAND_NUM     
      w_band_pref = rat_pri_list->priority_list_info[rat_idx].bst_band_cap.chgwt_band_cap;
#else
      w_band_pref = rat_pri_list->priority_list_info[rat_idx].bst_band_cap;
#endif
    }
#endif /* FEATURE_LOCAL_GWBAND_HAS_W */
    if((rat_pri_list->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_GSM) &&
        (rat_pri_list->priority_list_info[rat_idx].bst_rat_acq_required))
    {
#ifdef LTE_BAND_NUM   
      g_band_pref = rat_pri_list->priority_list_info[rat_idx].bst_band_cap.chgwt_band_cap;
#else
      g_band_pref = rat_pri_list->priority_list_info[rat_idx].bst_band_cap;
#endif
    }
  }

  w_band_pref &= wcdma_complete_band_mask;
  g_band_pref &= ~wcdma_complete_band_mask;

  return (w_band_pref | g_band_pref);  
}

/*===========================================================================

FUNCTION          RRC_CSP_UPDATE_GW_BAND_PREF_AFTER_SUCC_CAMPING

DESCRIPTION       This function updates mode and band preference after successful camping
                         This is called by CCM on successful cell change

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_update_mode_band_pref_after_succ_camping
(
  void
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"BST: Update mode and band pref from non BST");
  tdsrrc_csp_int_data.band_pref           = tdsrrccsp_extract_tds_band_pref_from_rat_pri_list(&tdsrrc_csp_int_data.rat_pri_list_info);
  tdsrrc_csp_int_data.band_pref_gw        = tdsrrccsp_extract_gw_band_pref_from_rat_pri_list(&tdsrrc_csp_int_data.rat_pri_list_info);
  tdsrrccsp_update_band_preference();
  tdsrr_set_band_pref_internal(tdsrrc_csp_int_data.band_pref_gw);
  tdsrrcmcm_update_mode_capability(&tdsrrc_csp_int_data.rat_pri_list_info);
}
/*===========================================================================

FUNCTION          RRC_CSP_STORE_RAT_PRI_LIST_TO_INT_DATA

DESCRIPTION       This function stores given rat_pri_list in CSP INT DATA struct  tdsrrc_csp_int_data.rat_pri_list_info.
                           Depending on featurization, it may remove LTE FDD bands if provided.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_store_rat_pri_list_to_int_data
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info
)
{
  ASSERT(rat_pri_list_info->num_items > 0);
  ASSERT(rat_pri_list_info->num_items <= SYS_MAX_ACQ_PREF_RAT_LIST_NUM);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Store the RAT PRI LIST into csp int data,total %d RATs",rat_pri_list_info->num_items);
  tdsrrc_csp_int_data.rat_pri_list_info = *rat_pri_list_info;

{
  uint8 rat_idx = 0;
  
  for(rat_idx =0;rat_idx < rat_pri_list_info->num_items &&
               rat_idx< SYS_MAX_ACQ_PREF_RAT_LIST_NUM ; rat_idx ++)
  {
#ifdef LTE_BAND_NUM  
    if(rat_pri_list_info->priority_list_info[rat_idx].acq_sys_mode != SYS_SYS_MODE_LTE)
    {
      TDSRRC_MSG4(MSG_LEGACY_HIGH,"==> RAT[%d] = %d, band_cap = (H:0x%x, L:0x%x)",
                  rat_idx,
                  rat_pri_list_info->priority_list_info[rat_idx].acq_sys_mode,
                  QWORD_HIGH(rat_pri_list_info->priority_list_info[rat_idx].band_cap.chgwt_band_cap),
                  QWORD_LOW(rat_pri_list_info->priority_list_info[rat_idx].band_cap.chgwt_band_cap));
    }
    else
    {
      #if (LTE_BAND_NUM == 64)
      TDSRRC_MSG4(MSG_LEGACY_HIGH,"==> RAT[%d] = %d, band_cap = (H:0x%x, L:0x%x)",
                  rat_idx,
                  rat_pri_list_info->priority_list_info[rat_idx].acq_sys_mode,
                  QWORD_HIGH(rat_pri_list_info->priority_list_info[rat_idx].band_cap.lte_band_cap),
                  QWORD_LOW(rat_pri_list_info->priority_list_info[rat_idx].band_cap.lte_band_cap));

      #else /*LTE_BAND_NUM is 256, only print low 64bit of LTE band cap*/
      TDSRRC_MSG4(MSG_LEGACY_HIGH,"==> RAT[%d] = %d, band_cap = (H:0x%x, L:0x%x)",
                  rat_idx,
                  rat_pri_list_info->priority_list_info[rat_idx].acq_sys_mode,
                  QWORD_HIGH(rat_pri_list_info->priority_list_info[rat_idx].band_cap.lte_band_cap.bits_1_64),
                  QWORD_LOW(rat_pri_list_info->priority_list_info[rat_idx].band_cap.lte_band_cap.bits_1_64));
      #endif
    }
#else
    TDSRRC_MSG4(MSG_LEGACY_HIGH,"==> RAT[%d] = %d, band_cap = (H:0x%x, L:0x%x)",
                rat_idx,
                rat_pri_list_info->priority_list_info[rat_idx].acq_sys_mode,
                QWORD_HIGH(rat_pri_list_info->priority_list_info[rat_idx].band_cap),
                QWORD_LOW(rat_pri_list_info->priority_list_info[rat_idx].band_cap));
#endif /*LTE_BAND_NUM*/
  }
}
}
/*===========================================================================

FUNCTION          RRC_CSP_UPDATE_RAT_PRI_LIST_INFO

DESCRIPTION       This function updates rat_pri_list_info in CSP INT DATA

DEPENDENCIES      None

RETURN VALUE      GW band pref

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_update_rat_pri_list_info
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updating the RAT PRI LIST info");
  tdsrrccsp_store_rat_pri_list_to_int_data(rat_pri_list_info);
  tdsrrcmcm_update_mode_capability(&tdsrrc_csp_int_data.rat_pri_list_info);
  tdsrrc_csp_int_data.band_pref = tdsrrccsp_extract_tds_band_pref_from_rat_pri_list(&tdsrrc_csp_int_data.rat_pri_list_info);
  tdsrrc_csp_int_data.band_pref_gw = tdsrrccsp_extract_gw_band_pref_from_rat_pri_list(&tdsrrc_csp_int_data.rat_pri_list_info);
  tdsrrccsp_update_band_preference();
}
/*===========================================================================

FUNCTION          RRC_CSP_GET_RAT_PRI_LIST_INFO

DESCRIPTION       This function fills the arguments with rat_pri_list_info in CSP INT DATA

DEPENDENCIES      None

RETURN VALUE      GW band pref

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_get_rat_pri_list_info
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info
)
{
  *rat_pri_list_info = tdsrrc_csp_int_data.rat_pri_list_info;
}
#endif

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)

#ifdef FEATURE_TDSCDMA_TO_LTE
/*===========================================================================

FUNCTION          RRC_EXTRACT_LTE_BAND_PREF_FROM_RAT_PRI_LIST

DESCRIPTION       This function extracts the LTE band pref from RAT PRI List

DEPENDENCIES      None

RETURN VALUE      LTE band pref

SIDE EFFECTS      None
===========================================================================*/
#ifdef LTE_BAND_NUM  
sys_lte_band_mask_e_type tdsrrc_extract_lte_band_pref_from_rat_pri_list
(
  sys_rat_pri_list_info_s_type *rat_pri_list,
  boolean                       tdd_band_only
)
{
  uint8 rat_idx = 0;
  sys_lte_band_mask_e_type lte_band_pref;
  uint8 band_idx = 0;
  sys_lte_band_mask_e_type lte_band_chk;
  sys_lte_band_mask_e_type bitmask_zero,lte_band_orig; 
  memset(&lte_band_pref, 0x00, sizeof(sys_lte_band_mask_e_type));
  memset(&lte_band_chk, 0x00, sizeof(sys_lte_band_mask_e_type));
  memset(&bitmask_zero, 0x00, sizeof(sys_lte_band_mask_e_type));
  memset(&lte_band_orig, 0x00, sizeof(sys_lte_band_mask_e_type));
  
  for(rat_idx =0;rat_idx < rat_pri_list->num_items &&
               rat_idx< SYS_MAX_ACQ_PREF_RAT_LIST_NUM ; rat_idx ++)
  {
    if(rat_pri_list->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_LTE)
    {
      lte_band_pref = lte_band_orig = rat_pri_list->priority_list_info[rat_idx].band_cap.lte_band_cap;
      break;
    }
  }

  if (tdd_band_only)
  {
    for(band_idx =0 ; 
        !SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&lte_band_pref,&bitmask_zero) && (band_idx < SYS_SBAND_LTE_EUTRAN_BAND_MAX); 
        band_idx++)
    {
      memset(&lte_band_chk, 0x00, sizeof(sys_lte_band_mask_e_type));
      SYS_LTE_BAND_MASK_ADD_BAND(&lte_band_chk, band_idx);
      if(SYS_LTE_BAND_MASK_IS_BAND_PRESENT(&lte_band_pref, band_idx))
      {
        if(!lte_rrc_lte_tdd_is_supported(lte_band_chk))
        {
          SYS_LTE_BAND_MASK_REMOVE_BAND(&lte_band_pref,band_idx);
          TDSRRC_MSG1(MSG_LEGACY_MED,"Only support LTE-TDD,ignore LTE-FDD band %d in rat pri list",band_idx+1);
        }
      }
    }
  }
  #if(LTE_BAND_NUM == 64)
    TDSRRC_MSG4(MSG_LEGACY_HIGH,"tdsrrc_extract_lte_band_pref_from_rat_pri_list: lte_band_pref = H:0x%x,L:0x%x, lte_band_orig = H:0x%x,L:0x%x",
                QWORD_HIGH(lte_band_pref),QWORD_LOW(lte_band_pref),QWORD_HIGH(lte_band_orig),QWORD_LOW(lte_band_orig));
  #else/*LTE_BAND_NUM is 256, only print low 64bit of LTE band cap*/
    TDSRRC_MSG4(MSG_LEGACY_HIGH,"tdsrrc_extract_lte_band_pref_from_rat_pri_list: lte_band_pref = H:0x%x,L:0x%x, lte_band_orig = H:0x%x,L:0x%x",
                QWORD_HIGH(lte_band_pref.bits_1_64),QWORD_LOW(lte_band_pref.bits_1_64),QWORD_HIGH(lte_band_orig.bits_1_64),QWORD_LOW(lte_band_orig.bits_1_64));
  #endif
  return lte_band_pref;
}
#else
sys_band_mask_type tdsrrc_extract_lte_band_pref_from_rat_pri_list
(
  sys_rat_pri_list_info_s_type *rat_pri_list,
  boolean                       tdd_band_only
)
{
  uint8 rat_idx = 0;
  sys_band_mask_type lte_band_pref = 0;
  uint8 band_idx = 0;
  sys_band_mask_type lte_band_chk = 0;

  for(rat_idx =0;rat_idx < rat_pri_list->num_items &&
               rat_idx< SYS_MAX_ACQ_PREF_RAT_LIST_NUM ; rat_idx ++)
  {
    if(rat_pri_list->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_LTE)
    {
      lte_band_pref |= rat_pri_list->priority_list_info[rat_idx].band_cap;
      break;
    }
  }

  if (tdd_band_only)
  {
  for(band_idx =0 ; lte_band_pref && (band_idx < SYS_SBAND_LTE_EUTRAN_BAND_MAX); band_idx++)
  {
    lte_band_chk = (sys_band_mask_type)(((uint64)1) << band_idx);
    if(lte_band_chk & lte_band_pref)
    {
      if(!lte_rrc_lte_tdd_is_supported(lte_band_chk))
      {
        lte_band_pref &= ~lte_band_chk;
        TDSRRC_MSG1(MSG_LEGACY_MED,"Only support LTE-TDD,ignore LTE-FDD band %d in rat pri list",band_idx+1);
      }
    }
  }
  }
  
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrc_extract_lte_band_pref_from_rat_pri_list: lte_band_pref = H:0x%x,L:0x%x",QWORD_HIGH(lte_band_pref),QWORD_LOW(lte_band_pref));
  return lte_band_pref;
}
#endif/*LTE_BAND_NUM*/


/*===========================================================================

FUNCTION          RRCCSP_UPDATE_BPLMN_LTE_INFO_FROM_RAT_PRI_LIST

DESCRIPTION       This function updates the LTE BPLMN variables in rrc_csp_int_data from rat pri list

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_update_bplmn_lte_info_from_rat_pri_list
(
  sys_rat_pri_list_info_s_type *rat_pri_list,
  boolean                       tdd_band_only
)
{
  tdsrrc_csp_int_data.bplmn_lte_mode_enabled = tdsrrcmcm_is_mode_enabled_in_rat_pri_list(rat_pri_list,SYS_SYS_MODE_LTE);
  tdsrrc_csp_int_data.bplmn_lte_band_pref = tdsrrc_extract_lte_band_pref_from_rat_pri_list(rat_pri_list, tdd_band_only);
}

/*===========================================================================

FUNCTION          rrccsp_get_lte_band_pref

DESCRIPTION       This function returns TDD LTE band preference

DEPENDENCIES
                  None.

RETURN VALUE
                  None.

SIDE EFFECTS      None.
===========================================================================*/
#ifdef LTE_BAND_NUM  
sys_lte_band_mask_e_type tdsrrccsp_get_lte_band_pref
#else
sys_band_mask_type tdsrrccsp_get_lte_band_pref
#endif
(
  void
)
{
  return tdsrrc_extract_lte_band_pref_from_rat_pri_list(&tdsrrc_csp_int_data.rat_pri_list_info, TRUE);
}

/*===========================================================================

FUNCTION          rrccsp_get_wg_band_pref

DESCRIPTION       This function populates WCDMA & GSM band preference

DEPENDENCIES
                  None.

RETURN VALUE
                  None

SIDE EFFECTS      None.
===========================================================================*/
void tdsrrccsp_get_tg_band_pref
(
  sys_band_mask_type *tdscdma_band_pref,
  sys_band_mask_type *gsm_band_pref
)
{
  /* ZZTBD: recheck if the band is good for GSM only */
#ifdef FEATURE_LOCAL_GWBAND_HAS_W
  sys_band_mask_type wcdma_complete_band_mask = tdsrrccsp_get_sys_wcdma_band_mask();
#endif
  sys_band_mask_type tdscdma_complete_band_mask = tdsrrccsp_get_sys_tdscdma_band_mask();
  sys_band_mask_type gw_band_pref = tdsrrccsp_extract_gw_band_pref_from_rat_pri_list(&tdsrrc_csp_int_data.rat_pri_list_info);
  sys_band_mask_type td_band_pref = tdsrrccsp_extract_tds_band_pref_from_rat_pri_list(&tdsrrc_csp_int_data.rat_pri_list_info);

  *tdscdma_band_pref = td_band_pref & tdscdma_complete_band_mask ;
#ifdef FEATURE_LOCAL_GWBAND_HAS_W
  *gsm_band_pref = gw_band_pref & ~wcdma_complete_band_mask ;
#else
  *gsm_band_pref = gw_band_pref;
#endif
}

/*===========================================================================

FUNCTION          tdsrrccsp_lte_earfcn_broadcasted_in_sib19

DESCRIPTION       This function check if there is a valid SIB19 with valid 
                  LTE frequencies broadcasted in the current camped cell.

DEPENDENCIES
                  None.

RETURN VALUE
                  TRUE or FALSE

SIDE EFFECTS      None.
===========================================================================*/
boolean tdsrrccsp_lte_earfcn_broadcasted_in_sib19(tdsrrc_SysInfoType19 * sib19_ptr)
{
  uint32 n_index = 0;
  uint32 earfcn;
  tdsrrc_EUTRA_FrequencyAndPriorityInfoList *eutra_freq_pri_list_ptr = NULL;

  if(sib19_ptr && sib19_ptr->m.eutra_FrequencyAndPriorityInfoListPresent)
  {
    eutra_freq_pri_list_ptr = &(sib19_ptr->eutra_FrequencyAndPriorityInfoList);

    while(n_index < eutra_freq_pri_list_ptr->n)
    {
      earfcn = (uint32)eutra_freq_pri_list_ptr->elem[n_index].earfcn;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
      if(lte_rrc_earfcn_is_supported(tdsrrc_get_as_id(),earfcn,tdsrrccsp_get_lte_band_pref()))
#else
      if(lte_rrc_earfcn_is_supported(earfcn,tdsrrccsp_get_lte_band_pref()))
#endif
      {
        return TRUE;
      }
      n_index++;
    }
  }

  return FALSE;
}

#endif /* #ifdef FEATURE_TDSCDMA_TO_LTE */

/*===========================================================================

FUNCTION          rrc_csp_add_frequency_to_detected_cell_list

DESCRIPTION       This function adds WCDMA frequency to detected frequency list

DEPENDENCIES
                  None.

RETURN VALUE
                  None.

SIDE EFFECTS      None.
===========================================================================*/
void tdsrrc_csp_add_frequency_to_detected_cell_list
(
  detected_frequency_type *detected_frequency
)
{
  boolean frequency_found = FALSE;
  uint8 idx = 0;
  for(idx =0;idx<tdsrrc_csp_int_data.detected_frequency_list.num_freqs && idx < LTE_IRAT_MAX_DETECTED_FREQ; idx++)
  {
    if((tdsrrc_csp_int_data.detected_frequency_list.freq[idx].rat == detected_frequency->rat) &&
        (tdsrrc_csp_int_data.detected_frequency_list.freq[idx].frequency == detected_frequency->frequency))
    {
      frequency_found = TRUE;
      break;
    }
  }
  if((!frequency_found) && (tdsrrc_csp_int_data.detected_frequency_list.num_freqs < LTE_IRAT_MAX_DETECTED_FREQ))
  {
    idx= tdsrrc_csp_int_data.detected_frequency_list.num_freqs;
    tdsrrc_csp_int_data.detected_frequency_list.freq[idx] = *detected_frequency;
    tdsrrc_csp_int_data.detected_frequency_list.num_freqs++;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Added frequency %d @ idx %d",detected_frequency->frequency,idx);
  }
}

/*===========================================================================

FUNCTION          RRCCSP_UPDATE_IRAT_DETECTED_FREQUENCIES

DESCRIPTION       This function updates the detected frequencies in 
                  tdsrrc_csp_int_data with detected frequency list
                  received from other RAT

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_update_irat_detected_frequencies
(
  detected_frequency_list_type* detected_frequency_list
)
{
  uint16 idx=0;
  for(idx =0 ;idx < detected_frequency_list->num_freqs;idx++)
  {
    tdsrrc_csp_add_frequency_to_detected_cell_list(&detected_frequency_list->freq[idx]);
  }
  return;
}

#endif /* #if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA) */


/*===========================================================================

FUNCTION          RRCCSP_GENERATE_RAT_ORDER_FOR_AUTO_PLMN_SRCH

DESCRIPTION       This function generates the RAT search order for Automatic PLMN search

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_generate_rat_order_for_auto_plmn_srch
(
  void
)
{
  uint16 rat_idx;
  boolean tds_rat_added, gsm_rat_added;

#if defined (FEATURE_TDSCDMA_TO_LTE)// && !defined (FEATURE_DISABLE_LTE_IRAT_BPLMN)
  boolean lte_rat_added = FALSE;
#ifdef LTE_BAND_NUM  
  sys_lte_band_mask_e_type bitmask_zero; 
  memset(&bitmask_zero, 0x00, sizeof(sys_lte_band_mask_e_type));
#endif  
#endif
  tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx = -1;
  tdsrrc_csp_int_data.plmn_srch_order.num_rats = 0;
  tds_rat_added = gsm_rat_added = FALSE;

  for(rat_idx=0;rat_idx< tdsrrc_csp_int_data.bplmn_num_req &&rat_idx < SYS_PRIORITY_PLMN_LIST_MAX_LENGTH  &&
               tdsrrc_csp_int_data.plmn_srch_order.num_rats < TDSRRC_CSP_MAX_PLMN_SRCH_RATS; rat_idx++)
  {
    if((tdsrrc_csp_int_data.bplmn_rat_req_list[rat_idx] == SYS_RAT_TDS_RADIO_ACCESS) && !tds_rat_added)
    {
      tdsrrc_csp_int_data.plmn_srch_order.rat[tdsrrc_csp_int_data.plmn_srch_order.num_rats++] = SYS_RAT_TDS_RADIO_ACCESS;
      tds_rat_added = TRUE;
    }
    else if((tdsrrc_csp_int_data.bplmn_rat_req_list[rat_idx] == SYS_RAT_GSM_RADIO_ACCESS) 
               && tdsrrc_csp_int_data.bplmn_dual_mode_enabled && !gsm_rat_added)
    {
      tdsrrc_csp_int_data.plmn_srch_order.rat[tdsrrc_csp_int_data.plmn_srch_order.num_rats++] = SYS_RAT_GSM_RADIO_ACCESS;
      gsm_rat_added = TRUE;
    }
#if defined (FEATURE_TDSCDMA_TO_LTE)// && !defined (FEATURE_DISABLE_LTE_IRAT_BPLMN)
    else if((tdsrrc_csp_int_data.bplmn_rat_req_list[rat_idx] == SYS_RAT_LTE_RADIO_ACCESS) 
               && tdsrrc_csp_int_data.bplmn_lte_mode_enabled && !lte_rat_added
            )
    {
#ifdef LTE_BAND_NUM  
      if (!SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&(tdsrrc_csp_int_data.bplmn_lte_band_pref),&bitmask_zero))
#else      
      if (tdsrrc_csp_int_data.bplmn_lte_band_pref)
#endif      
      {
      tdsrrc_csp_int_data.plmn_srch_order.rat[tdsrrc_csp_int_data.plmn_srch_order.num_rats++] = SYS_RAT_LTE_RADIO_ACCESS;
      lte_rat_added = TRUE;
    }
      else
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"bplmn_lte_band_pref is 0,skip LTE auto BPLMN search");
      }
    }
#endif
  }
  tdsrrccsp_print_rat_search_order();
}

#ifndef FEATURE_RAT_PRIORITY_LIST
/*===========================================================================

FUNCTION          RRCCSP_GENERATE_RAT_ORDER_FOR_MANUAL_PLMN_SRCH

DESCRIPTION       This function generates the RAT search order for Manual PLMN search

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_generate_rat_order_for_manual_plmn_srch
(
  void
)
{
  uint8 num_rats;
#ifdef FEATURE_TDSCDMA_TO_LTE
#ifdef LTE_BAND_NUM  
  sys_lte_band_mask_e_type bitmask_zero; 
  memset(&bitmask_zero, 0x00, sizeof(sys_lte_band_mask_e_type));
#endif  
#endif
  tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx = -1;
  num_rats = tdsrrc_csp_int_data.plmn_srch_order.num_rats = 0;

  if(tdsrrc_csp_int_data.bplmn_tdscdma_mode_enabled)
  {
  tdsrrc_csp_int_data.plmn_srch_order.rat[num_rats] = SYS_RAT_TDS_RADIO_ACCESS;
  num_rats ++;
  }
  if(tdsrrc_csp_int_data.bplmn_dual_mode_enabled)
  {
    tdsrrc_csp_int_data.plmn_srch_order.rat[num_rats] = SYS_RAT_GSM_RADIO_ACCESS;
    num_rats ++;
  }
#if defined (FEATURE_TDSCDMA_TO_LTE)// && !defined (FEATURE_DISABLE_LTE_IRAT_BPLMN)
  if(tdsrrc_csp_int_data.bplmn_lte_mode_enabled)
  {
#ifdef LTE_BAND_NUM  
     if (!SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&(tdsrrc_csp_int_data.bplmn_lte_band_pref), &bitmask_zero))
#else
     if (tdsrrc_csp_int_data.bplmn_lte_band_pref)
#endif
     {
    tdsrrc_csp_int_data.plmn_srch_order.rat[num_rats] = SYS_RAT_LTE_RADIO_ACCESS;
    num_rats ++;
  }
     else
     {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"bplmn_lte_band_pref is 0,skip LTE manual BPLMN search");
     }
  }
#endif

  tdsrrc_csp_int_data.plmn_srch_order.num_rats = num_rats;

  tdsrrccsp_print_rat_search_order();

}
#else
/*===========================================================================

FUNCTION          tdsrrccsp_generate_rat_order_for_manual_plmn_srch_from_rat_pri_list

DESCRIPTION       This function generates the RAT search order for Manual PLMN search according to rat_pri_list

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_generate_rat_order_for_manual_plmn_srch_from_rat_pri_list
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info_ptr
)
{
  uint8 num_rats;

  uint8 rat_idx = 0;
  boolean tds_is_added = FALSE;
  boolean gsm_is_added = FALSE;
#ifdef FEATURE_TDSCDMA_TO_LTE
  boolean lte_is_added = FALSE;
#ifdef LTE_BAND_NUM  
  sys_lte_band_mask_e_type bitmask_zero; 
  memset(&bitmask_zero, 0x00, sizeof(sys_lte_band_mask_e_type));
#endif
#endif
  tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx = -1;
  num_rats = tdsrrc_csp_int_data.plmn_srch_order.num_rats = 0;

  for(rat_idx = 0; rat_idx < rat_pri_list_info_ptr->num_items && rat_idx < SYS_MAX_ACQ_PREF_RAT_LIST_NUM; rat_idx++)
  {
    if(rat_pri_list_info_ptr->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_TDS)
    {
      if((tdsrrc_csp_int_data.bplmn_tdscdma_mode_enabled) && (tds_is_added==FALSE))
      {
        tdsrrc_csp_int_data.plmn_srch_order.rat[num_rats] = SYS_RAT_TDS_RADIO_ACCESS;
        num_rats ++;
        tds_is_added = TRUE;
      }
    }
    else if(rat_pri_list_info_ptr->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_GSM)
    {
      if((tdsrrc_csp_int_data.bplmn_dual_mode_enabled)&& (gsm_is_added==FALSE))
      {
        tdsrrc_csp_int_data.plmn_srch_order.rat[num_rats] = SYS_RAT_GSM_RADIO_ACCESS;
        num_rats ++;
        gsm_is_added = TRUE;
      }
    }
    else if(rat_pri_list_info_ptr->priority_list_info[rat_idx].acq_sys_mode == SYS_SYS_MODE_LTE)
    {   
#if defined (FEATURE_TDSCDMA_TO_LTE)
      if((tdsrrc_csp_int_data.bplmn_lte_mode_enabled)&& (lte_is_added==FALSE))
      {
#ifdef LTE_BAND_NUM  
        if (!SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&(tdsrrc_csp_int_data.bplmn_lte_band_pref), &bitmask_zero))
#else
        if (tdsrrc_csp_int_data.bplmn_lte_band_pref)
#endif
        {
          tdsrrc_csp_int_data.plmn_srch_order.rat[num_rats] = SYS_RAT_LTE_RADIO_ACCESS;
          num_rats ++;
          lte_is_added = TRUE;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"bplmn_lte_band_pref is 0,skip LTE manual BPLMN search");
        }
      }
#endif
    }
  }  

  tdsrrc_csp_int_data.plmn_srch_order.num_rats = num_rats;

  tdsrrccsp_print_rat_search_order();

}
#endif/*FEATURE_RAT_PRIORITY_LIST*/
/*===========================================================================

FUNCTION          RRCCSP_GENERATE_BPLMN_RAT_SRCH_ORDER_UPDATE_CURR_SRCH_RAT

DESCRIPTION       This function generates the RAT search order for either Manual of Automatic BPLMN search
                         based on n/W sel mode passeds
                         After generation of RAT order for search, will update the curr srch idx to first RAT

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_generate_bplmn_rat_srch_order_update_curr_srch_rat
(
  sys_network_selection_mode_e_type nw_sel_mode
)
{
  if(nw_sel_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
  {
#ifndef FEATURE_RAT_PRIORITY_LIST/*Already generate it if FEATURE_RAT_PRIORITY_LIST is enabled*/
     tdsrrccsp_generate_rat_order_for_manual_plmn_srch();
#endif
  }
  else
  {
     tdsrrccsp_generate_rat_order_for_auto_plmn_srch();
  }
  if(tdsrrc_csp_int_data.plmn_srch_order.num_rats !=0)
  {
    tdsrrc_csp_int_data.plmn_srch_order.curr_rat_srch_idx = 0;
    if(tdsrrc_csp_int_data.plmn_srch_order.rat[0] == SYS_RAT_TDS_RADIO_ACCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Do TD2TD search first");
      rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active = FALSE;
    }
#ifdef FEATURE_TDSCDMA_TO_LTE
    else if(tdsrrc_csp_int_data.plmn_srch_order.rat[0] == SYS_RAT_LTE_RADIO_ACCESS)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Do TD2L search first");
      rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active = TRUE;
      tdsrrc_csp_int_data.bplmn_interrat_type = TDSRRC_CSP_INTERRAT_LTE;
    }
#endif
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Do TD2G search first");
      rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active = TRUE;
#ifdef FEATURE_TDSCDMA_TO_LTE
      tdsrrc_csp_int_data.bplmn_interrat_type = TDSRRC_CSP_INTERRAT_GSM;
#endif
    }
  }
}
/*===========================================================================

FUNCTION          RRC_CSP_PRINT_RAT_SEARCH_ORDER

DESCRIPTION       This function prints the RAT search order for PLMN_LIST_REQ

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_print_rat_search_order
(
  void
)
{
  uint8 rat_idx;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RAT Search order for PLMN List Req");
  for(rat_idx =0; rat_idx< tdsrrc_csp_int_data.plmn_srch_order.num_rats && rat_idx < TDSRRC_CSP_MAX_PLMN_SRCH_RATS; rat_idx++)
  {
    if(tdsrrc_csp_int_data.plmn_srch_order.rat[rat_idx] == SYS_RAT_GSM_RADIO_ACCESS)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,">%d   ---GSM RAT------",rat_idx);
    }
    else if (tdsrrc_csp_int_data.plmn_srch_order.rat[rat_idx] == SYS_RAT_TDS_RADIO_ACCESS)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,">%d   ---TDS RAT------",rat_idx);
    }
#ifdef FEATURE_TDSCDMA_TO_LTE
    else if (tdsrrc_csp_int_data.plmn_srch_order.rat[rat_idx] == SYS_RAT_LTE_RADIO_ACCESS)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,">%d   ---LTE RAT------",rat_idx);
    }
#endif
  }
}


/*===========================================================================

FUNCTION          RRC_CSP_INIT_WTOG_PLMN_SRCH_INT_DATA_ACROSS_RATS

DESCRIPTION       This function is responsible for initializing the internal data structure to default values for WTOInterrat search
                         without initalzing num of plmns

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void  tdsrrccsp_init_td2g_plmn_srch_int_data_across_rats
(
  void
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"");
  rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_aborted = FALSE;
  rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_active  = FALSE;
  rrc_csp_td2g_bplmn_int_data.wtog_hplmn_found        = FALSE;
  rrc_csp_td2g_bplmn_int_data.wtog_rem_drx_tmr_value  = 0;
  rrc_csp_td2g_bplmn_int_data.wtog_srch_done          = FALSE;
  rrc_csp_td2g_bplmn_int_data.restart_bplmn_search    = FALSE;
  rrc_csp_td2g_bplmn_int_data.wtog_bplmn_srch_suspended = FALSE;
}



/*===========================================================================

FUNCTION          WCDMA_RRC_GET_BAND_FROM_DL_ARFCN

DESCRIPTION       This function is returns W band corresponding to DL UARFCN and PLMN ID

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
/* LTETBD: orignal function wcdma_rrc_get_band_from_dl_uarfcn is
 * called by LTE software under FEATURE_LTE_TO_TDSCDMA
 */

sys_band_mask_type tds_rrc_get_band_from_dl_uarfcn
(
  uint16 uarfcn,
  sys_plmn_id_s_type camped_plmn_id,
  boolean *is_band_valid
)
{
  tdsrrc_csp_band_class_type csp_band_class = 
    tdsrrc_get_frequency_band(
              (tdsrrc_freq_type)uarfcn);

  if(TDSRRC_CSP_BAND_MAX != csp_band_class )
  {
    *is_band_valid = TRUE;
    return (tdsrrc_convert_band_to_sys_band(csp_band_class));
  }

  /*Return any value as this is not going to be used*/
  *is_band_valid = FALSE;
  return SYS_BAND_MASK_TDS_BANDA;
}

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
/*===========================================================================

FUNCTION          TDS_RRC_DERIVE_FREQ_FROM_UARFCN

DESCRIPTION       This function converts the given UARFCN to absolute frequency in multiple of 100KHZ

DEPENDENCIES
                  None

RETURN VALUE
                  E_SUCCESS if the conversion is succesful
                  E_FAILURE if the conversion is not succesful

SIDE EFFECTS
                  None
===========================================================================*/
errno_enum_type tds_rrc_derive_freq_from_uarfcn
(
  uint16 uarfcn,
  uint16 *freq
)
{
  tdsrrc_csp_band_class_type csp_band_class = tdsrrc_get_frequency_band((tdsrrc_freq_type)uarfcn);
  if(TDSRRC_CSP_BAND_MAX != csp_band_class)
  {
    *freq = tdsrrccsp_derive_freq_from_uarfcn(uarfcn,csp_band_class);
    return E_SUCCESS;
  }
  else
  {
    return E_FAILURE;
  }	
}
#endif

/*===========================================================================

FUNCTION          TDS_RRC_RAT_IS_SUPPORTED

DESCRIPTION       This function checks if a PLMN ID is supported by the TD-SCDMA carrier

DEPENDENCIES
                  None

RETURN VALUE
                  TRUE if the PLMN ID is supported.

SIDE EFFECTS
                  The function is called by another RAT
===========================================================================*/
boolean tds_rrc_rat_is_supported(sys_plmn_id_s_type  camped_plmn_id)
{
   boolean match_found = FALSE;
   
   rrc_plmn_identity_type req_plmn = mmumts_convert_nas_plmn_id_to_rrc_plmn_id(camped_plmn_id);
   match_found = reg_mode_is_plmn_td_compliant(camped_plmn_id);

   TDSRRC_MSG3(MSG_LEGACY_MED,"tds_rrc_rat_is_supported: MCC=%d,MNC=%d,plmn_matched=%d",
                                TDSRRC_CSP_GET_INT_MCC(req_plmn),TDSRRC_CSP_GET_INT_MNC(req_plmn),match_found);
   
   return match_found;
}


#ifdef FEATURE_WRLF_SYSTEM_SEL

/*===========================================================================

FUNCTION          tdsrrccsp_is_suitable_srch_ended

DESCRIPTION       This function checks if suitable search has ended

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
boolean tdsrrccsp_is_suitable_srch_ended
(
  void
)
{
  boolean status;
  if(tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd)
  {
    status = TRUE;
  }
  else
  {
    status = FALSE; 
  }
  return status;
}


/*===========================================================================

FUNCTION          RRC_CSP_CHECK_N_SEND_SUITABLE_SRCH_END_IND

DESCRIPTION       This function checks and sends suitable_srch_end_ind
                  to NAS.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void tdsrrccsp_check_n_send_suitable_srch_end_ind
(
  void
)
{
  mm_cmd_type *mm_buf_ptr;

  if(!tdsrrc_csp_int_data.oos_splmn_srch_tmr_expd)
  {
    /* Get a buffer from MM task */
    if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_SUITABLE_SEARCH_END_IND)) == NULL)
    {
      ERR_FATAL("RRC Unable to send RRC_SUITABLE_SEARCH_END_IND to MM",0,0,0);
    }
#ifdef FEATURE_DUAL_SIM
    mm_buf_ptr->cmd.rrc_suitable_search_end_ind.as_id = tdsrrc_get_as_id();
#endif
    /* Put the command on MM queue */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF: Sending MM RRC_SUITABLE_SEARCH_END_IND");
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(mm_buf_ptr);
    }
    mm_put_cmd(mm_buf_ptr);
  }
}
/*===========================================================================

FUNCTION          RRC_CSP_SEND_DISABLE_BPLMN_IND

DESCRIPTION       This function sends disable_bplmn_ind to NAS.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void tdsrrccsp_send_disable_bplmn_ind(boolean status)
{
  mm_cmd_type *mm_buf_ptr;

  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_DISABLE_BPLMN_IND)) == NULL)
  {
    ERR_FATAL("RRC Unable to send RRC_DISABLE_BPLMN_IND to MM",0,0,0);
  }
  
  mm_buf_ptr->cmd.rrc_disable_bplmn_ind.disable_status = status;
#ifdef FEATURE_DUAL_SIM
  mm_buf_ptr->cmd.rrc_disable_bplmn_ind.as_id = tdsrrc_get_as_id();
#endif
  /* Put the command on MM queue */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"WRLF: Sending MM RRC_DISABLE_BPLMN_IND");
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  mm_put_cmd(mm_buf_ptr);
}

#endif

/*===========================================================================

FUNCTION          tdsrrccsp_handle_bplmn_suspend_lock_rel_offset_tmr_running

DESCRIPTION       This function is called to put WL1 to sleep, stop the 10ms lock release offset timer and
                         stop BPLMN DRX timer
                         This function is called on handling BPLMN suspend and BPLMN cleanup

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_handle_bplmn_suspend_lock_rel_offset_tmr_running
(
  void
)
{
  if(0 != tdsrrctmr_get_remaining_time(TDSRRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER))
  {
    tdsrrctmr_stop_timer(RRCTMR_TD2TD_BPLMN_DRX_SRCH_TIMER);
    tdsrrc_csp_int_data.bplmn_srch_tmr_expired = TRUE;
    if(tdsrrccsp_send_l1_cmd(TDSRRC_PROCEDURE_CSP, TDSL1_CPHY_GO_TO_SLEEP_IND, 
                                       &tdsrrc_csp_int_data.curr_acq_entry) == FAILURE)
    {
      ERR_FATAL("RRC Unable to get cmd buf for TDSL1_CPHY_GO_TO_SLEEP_IND",0,0,0);
    }
    tdsrrctmr_stop_timer(TDSRRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER);
#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
    /* If dedicated priority valid timer timeout and need to update meas_req, pending it during T2T BPLMN
     * construct and update it when indicate TL1 to go to sleep*/
    if (tdsrrcmeas_is_update_eutra_priority_info_meas_need() == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Check and update priority info meas req during BPLMN");
      /*Call the function that uses the dedicated priority info and SIB19 info to send meas_req to L1*/
      tdsrrcmeas_configure_l1_with_priority_info_eutra_cell_list();
    }
#endif
  }
}

/*===========================================================================

FUNCTION          tdsrrc_is_ps_abort_required

DESCRIPTION       This function is to be called by CM/SD to check if PS abort can be attemped on W
                         or G, for CS call on the other RAT.
                         1. In PCH/Connected Mode OOS states do not allow PS Abort
                         2. In FACH/DCH state allow PS abort.
                         3. If W is INACTIVE or PS domain in W is not active, then this API returns TRUE to 
                             allow PS abort on G.

DEPENDENCIES      None

RETURN VALUE      TRUE: To allow PS abort. and FALSE otherwise.

SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrc_is_ps_abort_required(void)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  if((TDSRRC_MODE_ACTIVE == tdsrrcmcm_get_rrc_mode())
      && tdsrrcscr_get_signalling_connection_status(RRC_PS_DOMAIN_CN_ID)
      && ((rrc_state == TDSRRC_STATE_CELL_PCH) || (rrc_state == TDSRRC_STATE_URA_PCH) ||
            (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN))
    )
  {
    return FALSE;
  }
  return TRUE;
}



/*===========================================================================

FUNCTION          tdsrrc_reset_oos_due_to_no_resource_flag

DESCRIPTION       This function is called to reset the flag which track OOS due to 
                          no resource.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrc_reset_oos_due_to_no_resource_flag(void)
{
  if(tdsrrc_csp_int_data.oos_due_to_no_resource)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: resetting flag oos_due_to_no_resource.");
    tdsrrc_csp_int_data.oos_due_to_no_resource = FALSE;
  }
}

/*===========================================================================

FUNCTION          tdsrrc_set_oos_due_to_no_resource_flag

DESCRIPTION       This function is called to set the flag which track OOS due to 
                          no resource.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrc_set_oos_due_to_no_resource_flag(void)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: setting flag oos_due_to_no_resource.");
  tdsrrc_csp_int_data.oos_due_to_no_resource = TRUE;
}

/*===========================================================================

FUNCTION          tdsrrc_get_oos_due_to_no_resource_flag_status

DESCRIPTION       This function is called to get the flag status which tracks OOS due to 
                          no resource.

DEPENDENCIES      None

RETURN VALUE      boolean

SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrc_get_oos_due_to_no_resource_flag_status(void)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH, "DSIM: oos_due_to_no_resource flag: %d (0-TL1 has lock,1-TL1 lost lock)",tdsrrc_csp_int_data.oos_due_to_no_resource);
  return (tdsrrc_csp_int_data.oos_due_to_no_resource);
}

/*===========================================================================

FUNCTION          tdsrrccsp_check_sib1_validity

DESCRIPTION       This function calls NAS API to verify LAC info provided in SIB1

DEPENDENCIES      None

RETURN VALUE      TRUE: LAC info is valid.
                  FALSE: Otherwise. 

SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrccsp_check_sib1_validity( tdsrrc_SysInfoType1  *sib1)
{
  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_common_info;
  /* This stores the LAI received in SIB1 */
  rrc_lai_type  lai;

  unsigned char  *data_ptr;
  uint32         i;

  if( sib1 == NULL )
  {
    return FALSE;
  } /* SIB1 not present */

  /* The SIB1 is present */
  nas_common_info.length = (uint32)sib1->cn_CommonGSM_MAP_NAS_SysInfo.numocts;
  /* Check if the RRC MM interface structure array can hold this info */
  if(sib1->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: Not enough space for Common NAS Sys Info (%d bytes)",sib1->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
    return FALSE;
  }

  /* Fill in the Common NAS system info now */
  data_ptr = sib1->cn_CommonGSM_MAP_NAS_SysInfo.data;
  for( i = 0; i < sib1->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
  {
    nas_common_info.value[i] =  *data_ptr;
    data_ptr ++;
  }

  /* Call NAS API to check if LAC info provided in SIB1 is correct. */
  return (mm_extract_lac_from_sib_info(&lai, &nas_common_info));
}

#ifdef FEATURE_3GPP_CSFB_TDS
/*===========================================================================

FUNCTION          rrccsp_post_update_sibs_ind

DESCRIPTION       This function will post update SIBS IND to SIB with cause received in arguments

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_post_update_sibs_ind
(
  tdsrrcsib_update_sibs_cause_e_type cause
)
{
  tdsrrc_cmd_type *rrc_cmd_ptr;
  /* Build RRC_UPDATE_SIBS_IND command to post to SIB */
  /* Allocates the buffer to RRC Internal command */
  if( (rrc_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {     
    rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_UPDATE_SIBS_IND;
    rrc_cmd_ptr->cmd.update_sibs_ind.procedure = TDSRRC_PROCEDURE_CSP;
    rrc_cmd_ptr->cmd.update_sibs_ind.cause = cause;
    /*Lets default the value TAG*/
    rrc_cmd_ptr->cmd.update_sibs_ind.mib_value_tag = 1;
    tdsrrc_put_int_cmd(rrc_cmd_ptr);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB: Sent TDSRRC_UPDATE_SIBS_IND");
  }
  else
  {
    ERR_FATAL("Memory allocation for the TDSRRC_UPDATE_SIBS_IND comamnd failed",0,0,0);
  }
}
#endif

/*===========================================================================

FUNCTION          tdsrrccsp_get_sys_wcdma_band_mask

DESCRIPTION       This function will return supported TDS band mask

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
sys_band_mask_type tdsrrccsp_get_sys_wcdma_band_mask
(
  void
)
{
  /*Define a local band mask */
  sys_band_mask_type local_band_mask= (SYS_BAND_MASK_WCDMA_I_IMT_2000 | SYS_BAND_MASK_WCDMA_II_PCS_1900 |
                                       SYS_BAND_MASK_WCDMA_III_1700 |SYS_BAND_MASK_WCDMA_IV_1700 |
                                       SYS_BAND_MASK_WCDMA_V_850 |SYS_BAND_MASK_WCDMA_VI_800 |
                                       SYS_BAND_MASK_WCDMA_VII_2600 |SYS_BAND_MASK_WCDMA_VIII_900|
                                       SYS_BAND_MASK_WCDMA_IX_1700
                                       );

  return local_band_mask;
}

/*===========================================================================

FUNCTION          tdsrrccsp_get_sys_tdscdma_band_mask

DESCRIPTION       This function will return supported TDSCDMA band mask

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
sys_band_mask_type tdsrrccsp_get_sys_tdscdma_band_mask
(
  void
)
{
  /*Define a local band mask */
  /* ZZTBD: should include all TDS bands? */
  sys_band_mask_type local_band_mask= (SYS_BAND_MASK_TDS_BANDA | 
                                       SYS_BAND_MASK_TDS_BANDE |
                                       SYS_BAND_MASK_TDS_BANDF
                                       );

  return local_band_mask;
}

/*===========================================================================

FUNCTION          rrccsp_init_first_band_to_be_scanned

DESCRIPTION       This function will init the first band to be scanned.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_init_first_band_to_be_scanned(void)
{
  
  tdsrrc_csp_curr_select_data.first_band_to_be_scanned = RRC_CSP_TDS_A_BAND;
  
  /* Reset the next_band_mask appropriately */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting the next_band_mask");
  tdsrrc_csp_curr_select_data.next_band_mask = tdsrrc_csp_int_data.tds_band_mask;
  /* Record next_band_mask for deep search*/
  tdsrrc_csp_curr_select_data.next_band_mask_for_deep = tdsrrc_csp_curr_select_data.next_band_mask;
  
  tdsrrc_csp_curr_select_data.first_band_to_be_scanned = TDSRRC_CSP_BAND_MAX;
  
    if((tdsrrc_csp_curr_select_data.first_band_to_be_scanned =
      tdsrrccsp_get_first_band_to_be_scanned()) != TDSRRC_CSP_BAND_MAX)
    {
      tdsrrc_csp_curr_select_data.current_band = 
        tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"1st band to be scanned = %d",tdsrrc_csp_curr_select_data.first_band_to_be_scanned);
    }
  	  
  
}


/*===========================================================================

FUNCTION          tdsrrccsp_set_est_req_for_emerg_call

DESCRIPTION       This function sets establishment request flag for emergency call

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/

void tdsrrccsp_set_est_req_for_emerg_call(void)
{
  tdsrrc_csp_int_data.est_req_for_emerg_call = TRUE;
}



/*===========================================================================

FUNCTION          tdsrrccsp_get_lte_resel_allow_status

DESCRIPTION       This function will return lte_resel_allow_status

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/

boolean tdsrrccsp_get_lte_resel_allow_status(void)
{
  return tdsrrc_csp_int_data.allow_lte_resel;
}

/*===========================================================================

FUNCTION          tdsrrc_check_cell_bar_status

DESCRIPTION       This function will check the cell bar status for teh cell.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
static void tdsrrccsp_check_cell_bar_status(tdsrrc_SysInfoType3 *sib3_ptr, 
      rrc_plmn_identity_type  plmn_id, tdsrrc_csp_cell_bar_e_type  *cell_bar)
{
  tdsrrc_CellAccessRestriction *sib3_4_access_restriction_ptr;
  tdsrrc_csp_substate_e_type curr_csp_substate = tdsrrccsp_get_csp_substate();  

  if( sib3_ptr == NULL )
  {
    /* SIB3 for this PLMN, CEll ID combination not present */
    ERR_FATAL("CSG: SIB3 empty.",0,0,0);
  } /* SIB3 not present */
  if((tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_MANUAL_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
     && (tdsrrc_csp_int_data.curr_scan != TDSRRC_CSP_PLMN_LIST_SCAN))
  {

    if((tdsrrc_get_state() != TDSRRC_STATE_DISCONNECTED) && (tdsrrc_get_state() != TDSRRC_STATE_CONNECTING))
    {
      if(sib3_ptr->sib4indicator == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSG: We might not have read SIB4 now. Check for cell bar status at CSP");
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSG: This scenario can be handled here in future");
        *cell_bar = TDSRRC_CSP_CELL_OK;
        return;
      }
      else
      {
        sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
      }
    }
    else
    {
      sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
    }
  }
  else
  {
    sib3_4_access_restriction_ptr = &(sib3_ptr->cellAccessRestriction);
  }

  /* The SIB3 is present - lets disect it */

  if((TDSRRC_CHECK_COMMON_MSG_TYPE2(sib3_4_access_restriction_ptr->cellBarred,rrc_CellBarred,
                                                notBarred )) &&
     ( sib3_4_access_restriction_ptr->cellReservedForOperatorUse == 
                                                     tdsrrc_ReservedIndicator_notReserved ) &&
     ( sib3_4_access_restriction_ptr->cellReservationExtension == 
                                                        tdsrrc_ReservedIndicator_notReserved ))
  {
    /* Cell is NOT barred and NOT Reserved */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell Not Barred,Not reserved");
    *cell_bar = TDSRRC_CSP_CELL_OK;
  } /* If Cell is NOT barred and NOT Reserved */

  else if((TDSRRC_CHECK_COMMON_MSG_TYPE2(sib3_4_access_restriction_ptr->cellBarred,rrc_CellBarred,
                                                      notBarred )) &&
          ( sib3_4_access_restriction_ptr->cellReservedForOperatorUse == 
                                                     tdsrrc_ReservedIndicator_notReserved ) &&
          ( sib3_4_access_restriction_ptr->cellReservationExtension == 
                                                      tdsrrc_ReservedIndicator_reserved ))
  {
    /* Cell is NOT Barred but RESERVED for SoLSA - Treat it as Barred since the 
     * UE does not have SoLSA support (TBD: May change for UEs supporting 
     * Rel 2000 )
     */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell treated Barred future Reserved");
    tdsrrc_csp_int_data.tds_cell_time_to_reselect = 1280;

    if((tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ) 
        &&  (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED))
    {
      /* Cell is Barred. Ignore the Intra-frequency cell re-selection indicator */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore Intra-freq cell resel ind");
      *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;
    }
    else
    {
      /* Cell is Not Barred, but RESERVED for SoLSA (is treated as BARRED) AND
       * "Intra frequency Cell re-selection Indicator" is NOT ALLOWED */ 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Intra-freq Not Allowed");
      *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
    }
  } /* If Cell is NOT Barred, not reserved for Operator but RESERVED for SoLSA */

  else if ((TDSRRC_CHECK_COMMON_MSG_TYPE2(sib3_4_access_restriction_ptr->cellBarred,rrc_CellBarred,
                                                      notBarred) ) &&
     ( sib3_4_access_restriction_ptr->cellReservedForOperatorUse == 
                                                      tdsrrc_ReservedIndicator_reserved ))
  {
    /* Cell is NOT Barred but RESERVED for Operator */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell reserved for OPerator");


    /* Now check the UE Access Class to determine Cell Status */

   if(( tdsrrc_hi_access_class != RRC_HI_ACCESS_CLASS_ABSENT ) &&
       (((tdsrrc_hi_access_class & RRC_ACCESS_CLASS_11_BIT) ||
         (tdsrrc_hi_access_class & RRC_ACCESS_CLASS_15_BIT)) &&
        (tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn(plmn_id))))
    {
      /* UEs assigned to Access Class 11 or 15 may select/re-select this cell if in the home PLMN.
       * UEs assigned to an Access Class in the range 0 to 9 and 12 to 14 shall behave as if
       * cell status "barred" is indicated using the value "not allowed" in the
       * IE "Intra-frequency cell re-selection indicator" and the maximum value for Tbarred
       */

       TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE High AC present and in HPLMN");
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell treated NOT Barred");     
       *cell_bar = TDSRRC_CSP_CELL_OK;
    }
    else
    {
      if(tdsrrc_hi_access_class == RRC_HI_ACCESS_CLASS_ABSENT)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE High AC not present");
      }

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell treated Barred (Op Reserved)");
      tdsrrc_csp_int_data.tds_cell_time_to_reselect = 1280;
      tdsrrc_csp_int_data.cell_reserved_for_operator_use = TRUE;
      if((tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ) 
          &&  (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED))
      {
        /* Cell is Barred. Ignore the Intra-frequency cell re-selection indicator */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore Intra-freq cell resel ind");
        *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;
      }
      else if(((curr_csp_substate == TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB) 
                 || (curr_csp_substate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB))
                && (tdsrrc_csp_int_data.new_cell_acq_entry.freq == tdsrrc_csp_int_data.curr_acq_entry.freq))
      {
        /* Intra-frequency neighbor frequency is barred. Barring the frequency */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Intra-Freq Ngbr Cell Freq barred. Sndng Freq Barred to L1");
        *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
      }
      else
      {
        /* UE has either a High Access Class but is NOT in Home PLMN
         * OR UE does NOT have a High Access Class */
        /* Cell is TREATED AS BARRED for this UE */
        /* Cell is Not Barred, but RESERVED for Operator (is treated as BARRED) 
         * AND "Intra frequency Cell re-selection Indicator" is NOT ALLOWED */ 
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"No UE Hi AC or not in HPLMN,Intra-freq Not Allowed");
        *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;       
      }
    }
  } /* If Cell is NOT Barred but RESERVED for Operator */

  else if (TDSRRC_CHECK_COMMON_MSG_TYPE2(sib3_4_access_restriction_ptr->cellBarred,rrc_CellBarred,
                                                      barred ))
  {

    switch(sib3_4_access_restriction_ptr->cellBarred.u.barred->t_Barred)
    {
      case tdsrrc_T_Barred_s10:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 10;
        break;
      case tdsrrc_T_Barred_s20:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 20;
        break;
      case tdsrrc_T_Barred_s40:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 40;
        break;
      case tdsrrc_T_Barred_s80:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 80;
        break;
      case tdsrrc_T_Barred_s160:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 160;
        break;
      case tdsrrc_T_Barred_s320:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 320;
        break;
      case tdsrrc_T_Barred_s640:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 640;
        break;
      case tdsrrc_T_Barred_s1280:
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 1280;
        break;
      default:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unexpected. Setting the time to 1280 secs");
        tdsrrc_csp_int_data.tds_cell_time_to_reselect = 1280;
        break;
    }
    
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell is indicated as Barred");
    /* Cell is indicated as Barred */
    if( sib3_4_access_restriction_ptr->cellBarred.u.barred->
                      intraFreqCellReselectionInd == tdsrrc_AllowedIndicator_allowed )
    {
      /* Cell is Barred
       * AND "Intra frequency Cell re-selection Indicator" is ALLOWED */ 
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Intra-freq Allowed");
      *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;
    }
    else
    {
      if((tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ)
         && (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED))
      {
        /* Cell is Barred. Ignore the Intra-frequency cell re-selection indicator */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore Intra-freq cell resel ind");
        *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;
      }
	  /*
      else if(CSG_ID_IN_SIB3(sib3_ptr))
      {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSG cell ignore intra freq resel not allowed indicator");
          *cell_bar = RRC_CSP_CELL_BAR_INTRA_FREQ;
      }
      */
      else
      {
        /* Cell is Barred
         * AND "Intra frequency Cell re-selection Indicator" is NOT ALLOWED */ 
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Intra-freq Not Allowed");
        *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
      }
    }
  } /* Cell is indicated as Barred */
  else
  {
    /* We should not come here */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: Error in cell bar evaluation logic");
    tdsrrc_csp_int_data.tds_cell_time_to_reselect = 1280;

    if((tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ) 
        &&  (tdsrrc_csp_int_data.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED))
    {
      /* Cell is Barred. Ignore the Intra-frequency cell re-selection indicator */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore Intra-freq cell resel ind");
      *cell_bar = TDSRRC_CSP_CELL_BAR_INTRA_FREQ;
    }
    else if(((curr_csp_substate == TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB) 
               || (curr_csp_substate == TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB))
              && (tdsrrc_csp_int_data.new_cell_acq_entry.freq == tdsrrc_csp_int_data.curr_acq_entry.freq))
    {
      /* Intra-frequency neighbor frequency is barred. Barring the frequency */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Intra-Freq Ngbr Cell Freq barred. Sndng Freq Barred to L1");
      *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
    }
    else
    {
      *cell_bar = TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ;
    }
  }
}
/*===========================================================================

FUNCTION          tdsrrccsp_initial_suitability_check

DESCRIPTION       This function will check initial suitability of the cell

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/

void tdsrrccsp_initial_suitability_check(tdsrrc_MasterInformationBlock *mib_ptr, tdsrrc_SysInfoType3 *sib3_ptr,
                                      rrc_plmn_identity_type mib_plmn_id, 
                                      tdsrrc_sib_status_e_type *status)
{
  rrc_plmn_identity_type shared_plmn_list[TDSMAX_SHARED_PLMNS];
  tdsrrc_csp_cell_bar_e_type  cell_bar_status;
  uint8 shared_plmn_idx = 0, num_of_shared_plmns = 0;
  boolean nw_shared = FALSE;

  boolean hplmn_cell_bar_status_found = FALSE;
  tdsrrc_csp_cell_bar_e_type hplmn_cell_bar_status = TDSRRC_CSP_CELL_OK;
  boolean vplmn_cell_bar_status_found = FALSE;
  tdsrrc_csp_cell_bar_e_type vplmn_cell_bar_status = TDSRRC_CSP_CELL_OK;
  *status = TDSSIB_EVENT_SUCCEEDED;
  if(tdsrrc_is_nw_shared(mib_ptr))
  {
    nw_shared = TRUE;
    num_of_shared_plmns = tdsrrccsp_construct_multiple_plmn_list_from_mib(
                                                                       mib_ptr,
                                                                       shared_plmn_list);
  }

/*
  do
  {
      if(nw_shared)
      {
        mib_plmn_id = shared_plmn_list[shared_plmn_idx++];
      }

      if(rrc_bcch_and_nas_plmns_match(mib_plmn_id, rrc_csp_int_data.curr_req_plmn.plmn_id))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN is the same as requested BCCH and NAS PLMN match");
        plmn_same_or_eq = TRUE;
        break;
      }
      else
      {
        if((rrccsp_check_for_eplmn(
                  mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                    rrc_csp_int_data.curr_req_plmn.plmn_id))) == TRUE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"NAS PLMN is in the eq PLMN list");
          if((rrccsp_check_for_eplmn(
                    mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                     mib_plmn_id))) == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH PLMN is in the eq PLMN list");
            plmn_same_or_eq = TRUE;
            break;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH PLMN not in eq PLMN list");
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"NAS PLMN not in eq PLMN list");
        }
      }

  }while(shared_plmn_idx < num_of_shared_plmns)

  if(!plmn_same_or_eq)
  {
    return FALSE;
  }
  */

  /* CHECK CELL BAR STATUS */

  do
  {
    if(nw_shared)
    {
      mib_plmn_id = shared_plmn_list[shared_plmn_idx++];
    }

    if(tdsrrccsp_is_bcch_plmn_an_hplmn_or_ehplmn(mib_plmn_id))
    {
      if(!hplmn_cell_bar_status_found)
      {
        tdsrrccsp_check_cell_bar_status( sib3_ptr, mib_plmn_id, &cell_bar_status);
        hplmn_cell_bar_status = cell_bar_status;
        hplmn_cell_bar_status_found = TRUE;
      }
      else
      {
        cell_bar_status = hplmn_cell_bar_status;
      }
    }
    else
    {
      if(!vplmn_cell_bar_status_found)
      {
        tdsrrccsp_check_cell_bar_status( sib3_ptr, mib_plmn_id, &cell_bar_status);
        vplmn_cell_bar_status = cell_bar_status;
        vplmn_cell_bar_status_found = TRUE;
      }
      else
      {
        cell_bar_status = vplmn_cell_bar_status;
      }
    }

  }while(shared_plmn_idx < num_of_shared_plmns);

  if(hplmn_cell_bar_status_found == TRUE)
  {
    cell_bar_status = hplmn_cell_bar_status;
  }

 
    if(cell_bar_status == TDSRRC_CSP_CELL_BAR_INTRA_FREQ)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSG: Cell barred");
      /* Send Cell bar request to L1 */
      tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_CPID_BARRED, 
                                            tdsrrc_csp_int_data.tds_cell_time_to_reselect);
      *status = TDSSIB_EVENT_FAILED_CELL_BAR;
    }
    else if(cell_bar_status == TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSG: Freq barred");
      /* Send Cell bar request to L1 */
      tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_FREQ_BARRED, 
                                            tdsrrc_csp_int_data.tds_cell_time_to_reselect);
      *status = TDSSIB_EVENT_FAILED_FREQ_BAR;
    }

}

/*=========================================================================

 FUNCTION     : tdsrrccsp_update_det_cells_with_jds_cells
 
 DESCRIPTION  : When any cell is barred, RRC gets JDS cells from L1 of the
   same frequency to append to the detected cells, because JDS cell
   are more accurate. This function is to append these JDS cell if they are
   not duplicated.
              
 DEPENDENCIES : 

 INPUT PARAMETERS :
   tdsl1_jds_cell_list : number of JDS cells and their CPID.

 RETURN VALUE : 
   void.
       
 SIDE EFFECTS : 
   N/A
==========================================================================*/
void tdsrrccsp_update_det_cells_with_jds_cells(tdsl1_jds_cell_list_type *tdsl1_jds_cell_list)
{
  uint8 i, j;
  boolean dup_flag = FALSE;

  for(i = 0; (i < tdsl1_jds_cell_list->num_cell)&&(i < TDSL1_MAX_JDS_CELL_NUM); i++)
  {
    dup_flag = FALSE;
    
    for(j = 0; j < tdsrrc_csp_int_data.num_cpid_det; j++)
    {
      if(tdsrrc_csp_int_data.cpid_det[j].cpid == tdsl1_jds_cell_list->cell_id[i])
      {
        dup_flag = TRUE;
        break;
      }
    }

    if((!dup_flag) && (tdsrrc_csp_int_data.num_cpid_det < TDSL1_NUM_CPID_DETECTED_MAX))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Add JDS cell %d to the det cell list",tdsl1_jds_cell_list->cell_id[i]);
      tdsrrc_csp_int_data.cpid_det[tdsrrc_csp_int_data.num_cpid_det].cpid = tdsl1_jds_cell_list->cell_id[i];
      tdsrrc_csp_int_data.num_cpid_det++;
    }
  }
}



/*=========================================================================

 FUNCTION     : tdsrrccsp_is_dummy_plmn_id
 
 DESCRIPTION  : This function checks the input PLMN to match dummy PLMN
                which NAS uses for special purpose.
              
 DEPENDENCIES : 

 INPUT PARAMETERS :
   req_plmn : input PLMN for checking.

 RETURN VALUE : 
   TRUE: it is dummy PLMN
   FALSE: it is not dummy PLMN
       
 SIDE EFFECTS : 
   N/A
==========================================================================*/
boolean tdsrrccsp_is_dummy_plmn_id
(
  rrc_plmn_identity_type req_plmn
)
{
   uint8 i = 0;
   boolean ret = TRUE;
   /* FF FF FF is the dummy PLMN */
   
   for (i = 0; i < RRC_MAX_MCC_DIGITS; i++)
   {
      if (req_plmn.mcc[i] != 0xF)
      {
         ret = FALSE;
         return ret;
      }
   }

   for (i = 0; i < RRC_MAX_MNC_DIGITS; i++)
   {
      if (req_plmn.mnc[i] != 0xF)
      {
         ret = FALSE;
         break;
      }
   }

   if (ret)
   {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"It is Dummy PLMN");
   }
   
   return ret;
}

/*=========================================================================

 FUNCTION     : tdsrrccsp_is_dummy_plmn_search
 
 DESCRIPTION  : This function checks the input PLMN to match dummy PLMN
                and the search for CR504264
              
 DEPENDENCIES : 

 INPUT PARAMETERS :
   req_plmn : input PLMN for checking.

 RETURN VALUE : 
   TRUE: it is dummy PLMN search
   FALSE: it is not dummy PLMN search
       
 SIDE EFFECTS : 
   N/A
==========================================================================*/
boolean tdsrrccsp_is_dummy_plmn_search
(
  rrc_plmn_identity_type req_plmn
)
{
return ((tdsrrccsp_is_dummy_plmn_id(tdsrrc_csp_int_data.curr_req_plmn.plmn_id))
         && (tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_SPECIFIC_PLMN_REQ)
         && (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)
         && (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_LIMITED)
         && (tdsrrc_csp_int_data.network_select_mode != SYS_NETWORK_SELECTION_MODE_MANUAL));
}
#ifdef FEATURE_WRLF_SYSTEM_SEL
/*=========================================================================

 FUNCTION     : tdsrrc_set_connecting_oos_flag
 
 DESCRIPTION  : This function set the flag to indicate whether OOS happen during RRC connecting state
              
 DEPENDENCIES : 

 INPUT PARAMETERS :
   is_connecting_oos

 RETURN VALUE : 
   void
       
 SIDE EFFECTS : 
   N/A
==========================================================================*/
void tdsrrc_set_connecting_oos_flag(boolean is_connecting_oos)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Set ue_in_connecting_oos: %d",is_connecting_oos);
  tdsrrc_csp_int_data.ue_in_connecting_oos = is_connecting_oos;
}
#endif
/*===========================================================================

FUNCTION          tdsrrccsp_reset_wait_l1_rel_lock

DESCRIPTION       To reset csp_waiting_for_wl1_to_release_lock flag

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/

void tdsrrccsp_reset_wait_l1_rel_lock()
{
  tdsrrc_csp_int_data.csp_waiting_for_wl1_to_release_lock = FALSE;
}

/*===========================================================================

FUNCTION          tdsrrccsp_set_to_suitable_or_acceptable_substate

DESCRIPTION       This function sets csp substate to suitable or acceptable.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_set_to_suitable_or_acceptable_substate(void)
{
  tdsrrc_csp_substate_e_type next_substate;
  if(tdsrrccsp_check_is_suitable_or_acceptable_substate())
  {
    next_substate = TDSRRCCSP_CAMPED_SUIT_CELL;
  }
  else
  {
    next_substate = TDSRRCCSP_CAMPED_ACCEPT_CELL;
  }
  return next_substate; 
}
/*===========================================================================

FUNCTION          tdsrrccsp_check_is_suitable_or_acceptable_substate

DESCRIPTION       This function checks that csp substate should be set to suitable or acceptable.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  TRUE for suitable substate and FALSE for acceptable state.
SIDE EFFECTS
                  None

===========================================================================*/
boolean tdsrrccsp_check_is_suitable_or_acceptable_substate(void)
{
  boolean is_suitable = FALSE;
  tdsrrc_csp_plmn_status_e_type cell_plmn_status;
  
 if(tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ)
  {
    is_suitable = FALSE;
  }
  else /* Requested PLMN type is for a Specific PLMN */
  {
    /* Determine at this point if based on SIBs of the serving cell, we are still in Suitable Cell Selection */
    cell_plmn_status = tdsrrccsp_get_cell_plmn_status(&tdsrrc_csp_int_data.curr_acq_entry, 
                                        tdsrrc_csp_int_data.curr_req_plmn.plmn_id);
    
    if(TDSRRC_CSP_IN_SUIT_SEL(cell_plmn_status,
                           tdsrrc_csp_int_data.curr_acq_entry.status_mask))
    {
      is_suitable = TRUE;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"OOS: Changing CSP sst to Acceptable");
      is_suitable = FALSE;
    }
  } 
  return is_suitable; 
}


/*===========================================================================

FUNCTION          tdsrrccsp_store_tdd_target_info_freqs

DESCRIPTION       This function stores the uarfcns from tdsrrc_csp_int_data.target_tdd_cell_list to csp internal data
                  structure, while processing L2T redirection req msg.
                  For R9 redirection, tdsrrc_csp_int_data.target_tdd_cell_list contains LTE sib6 ngbr freqs;
                  For R10 redirection, tdsrrc_csp_int_data.target_tdd_cell_list contains redirect target freqs.
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_store_tdd_target_info_freqs(void)
{
  uint8 num_freqs;
  tds_rrc_lte_tds_neigh_list_type *selected_freq_list = NULL;

  memset(&tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9, 0, sizeof(tds_rrc_lte_tds_neigh_list_type));
  memset(&tdsrrc_csp_int_data.tdd_target_freq_list, 0, sizeof(tds_rrc_lte_tds_neigh_list_type));

  /* tdsrrc_csp_int_data.target_tdd_cell_list now contains: for R9, lte sib6 ngbr freqs; for R10, l2t redirect target freq list.
        store them accordingly */
#ifdef FEATURE_L2TD_REDIRECT_R10
  if (!tdsrrc_csp_int_data.l2t_r9_redirect)
  {
    selected_freq_list = &tdsrrc_csp_int_data.tdd_target_freq_list;
  }
  else
#endif
  {
    selected_freq_list = &tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9;
  }

  for (num_freqs=0; num_freqs<tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies &&
                    num_freqs<MAX_UTRAN_TDS_FREQUNECIES &&
                    num_freqs<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ; num_freqs++)
  {
    selected_freq_list->uarfcn[num_freqs] = tdsrrc_csp_int_data.target_tdd_cell_list.fdd_cell_list[num_freqs].uarfcn;
  }
  selected_freq_list->uarfcn_count = num_freqs;
}

#ifdef FEATURE_L2TD_REDIRECT_OPT_IRAT_ACQ_DB
/*===========================================================================

FUNCTION          tdsrrccsp_store_lte_tds_irat_acq_db_freqs

DESCRIPTION       This function stores lte tds irat acq db freqs to csp internal data structure, while processing L2T
                  redirection req msg. It is agreed with LTE RRC that the irat acq db entries are sorted by LTE side
                  based on the success_cnt in descending order before sending to TDS RRC. 
                  For R10 redirect, the freqs are stored in high_cnt and low_cnt two lists per
                  threshold from NV. For R9 redirect, the freqs are all stored in low_cnt list.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_store_lte_tds_irat_acq_db_freqs
(
  tds_rrc_lte_redir_req_type *lte_redir_req
)
{
  uint8 i, high_cnt_freq_nbr, low_cnt_freq_nbr;
  
  memset(&tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list, 0, sizeof(tds_rrc_lte_tds_neigh_list_type));
  memset(&tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list, 0, sizeof(tds_rrc_lte_tds_neigh_list_type));

  high_cnt_freq_nbr = low_cnt_freq_nbr = 0;

  if (!tdsrrc_csp_int_data.l2t_r9_redirect)
  {
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: l2t_acqdb_cnt_threshold_redirection: %d",tdsrrc_nv_l2t_acqdb_cnt_threshold_redirection);
  }

  for (i=0; i<lte_redir_req->tds_irat_acq_db.acq_db_entry_count && i<TDSCDMA_RRC_LTE_MAX_TDS_IRAT_ACQ_DB_FREQ &&
  	        high_cnt_freq_nbr<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ &&
  	        low_cnt_freq_nbr<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ; i++)
  {
#ifdef FEATURE_L2TD_REDIRECT_R10
    if (!tdsrrc_csp_int_data.l2t_r9_redirect &&
   	 	(lte_redir_req->tds_irat_acq_db.tds_irat_acq_db_entries[i].success_count > tdsrrc_nv_l2t_acqdb_cnt_threshold_redirection)
   	   )
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"L2TD: store R10 high_cnt irat acq db freq %d,success_cnt %d",lte_redir_req->tds_irat_acq_db.tds_irat_acq_db_entries[i].uarfcn,lte_redir_req->tds_irat_acq_db.tds_irat_acq_db_entries[i].success_count);
	  tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list.uarfcn[high_cnt_freq_nbr++] = 
   	                      lte_redir_req->tds_irat_acq_db.tds_irat_acq_db_entries[i].uarfcn;
    }
    else
#endif
	{
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"L2TD: store low_cnt irat acq db freq %d,success_cnt %d",lte_redir_req->tds_irat_acq_db.tds_irat_acq_db_entries[i].uarfcn,lte_redir_req->tds_irat_acq_db.tds_irat_acq_db_entries[i].success_count);
	  tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[low_cnt_freq_nbr++] =
	  	                  lte_redir_req->tds_irat_acq_db.tds_irat_acq_db_entries[i].uarfcn;
	}
  }
  tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list.uarfcn_count = high_cnt_freq_nbr;
  tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn_count = low_cnt_freq_nbr;
}

/*===========================================================================

FUNCTION          tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list

DESCRIPTION       This is a utility function to check whether a given uarfcn exists in a uarfcn list
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
static boolean tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list
(
  uint16 uarfcn,
  tds_rrc_lte_tds_neigh_list_type *freq_list
)
{
  uint8 i;
  boolean found = FALSE;

  for (i=0; i<freq_list->uarfcn_count && i<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ; i++)
  {
    if (uarfcn == freq_list->uarfcn[i])
    {
      found = TRUE;
	  break;
    }
  }
  return found;
}

/*===========================================================================

FUNCTION          tdsrrccsp_rmv_dup_freq_from_lte_redirect_req_r9

DESCRIPTION       This function checks and removes dup uarfcns passed by LRRC in R9 L2T redirection req msg, in the
                  priority order of redirected freq, irat acq db freq, lte sib6 ngbr freqs.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_rmv_dup_freq_from_lte_redirect_req_r9(void)
{
  uint8 i;
  tds_rrc_lte_tds_neigh_list_type temp_freq_list; /* temp holder for the non-dup freqs */
  boolean add_to_list;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: check dup freqs in irat acq db/sib6 ngbrs");

  /* rmv any irat acq db freq duped with dest freq, or in irat acq db freq list itself */
  if (tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn_count > 0)
  {
    memset(&temp_freq_list, 0, sizeof(tds_rrc_lte_tds_neigh_list_type));
    for (i=0; i<tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn_count &&
		      i<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ &&
		      temp_freq_list.uarfcn_count<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ; i++)
    {
	  if(tdsrrccsp_is_supported_band(tdsrrc_get_frequency_band((tdsrrc_freq_type)tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i])))
      {
        add_to_list = TRUE;
        if (tdsrrc_csp_int_data.dest_freq == tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i])
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R9 irat acq db freq %d,dup with redirect dest freq",tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i]);
          add_to_list = FALSE;
        }
        if (add_to_list && tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list(
			tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i], &temp_freq_list)
		   )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R9 irat acq db freq %d,dup with prev irat acq db freqs",tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i]);
          add_to_list = FALSE;
        }
        if (add_to_list)
        {
          temp_freq_list.uarfcn[temp_freq_list.uarfcn_count++] = tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i];
        }
      }
      else
      {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore unsupported R9 irat acq db freq %d",tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i]);
      }
    }
    tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list = temp_freq_list;
  }

  /* rmv any lte sib6 ngbr freq duped with dest freq, in irat acq db freq list, or in the sib6 ngbr freq list itself */
  if (tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9.uarfcn_count > 0)
  {
    memset(&temp_freq_list, 0, sizeof(tds_rrc_lte_tds_neigh_list_type));
    for (i=0; i<tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9.uarfcn_count &&
		      i<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ &&
		      temp_freq_list.uarfcn_count<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ; i++)
    {
	  if(tdsrrccsp_is_supported_band(tdsrrc_get_frequency_band((tdsrrc_freq_type)tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9.uarfcn[i])))
      {
        add_to_list = TRUE;
        if (tdsrrc_csp_int_data.dest_freq == tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9.uarfcn[i])
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R9 sib6 ngbr freq %d,dup with redirect dest freq",tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9.uarfcn[i]);
          add_to_list = FALSE;
        }
        if (add_to_list && tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list(
			tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9.uarfcn[i], &tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list)
		   )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R9 sib6 ngbr freq %d,dup with irat acq db freqs",tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9.uarfcn[i]);
          add_to_list = FALSE;
        }
        /* checking of dup freq in R9 sib6 ngbr freq list itself has been performed when processing the l2t redirect req msg and save to
                  tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9. Bypass this step. */
        if (add_to_list)
        {
          temp_freq_list.uarfcn[temp_freq_list.uarfcn_count++] = tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9.uarfcn[i];
        }
      }
      else
      {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore unsupported R9 sib6 ngbr freq %d",tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9.uarfcn[i]);
      }
    }
    tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9 = temp_freq_list;
  }

TDSRRC_MSG2(MSG_LEGACY_HIGH,"L2TD: R9, after rmv dup freqs, irat acqdb freq: %d,sib6 ngbr: %d",tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn_count,tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9.uarfcn_count);
}

/*===========================================================================

FUNCTION          tdsrrccsp_rmv_dup_freq_from_lte_redirect_req_r10

DESCRIPTION       This function checks and removes dup uarfcns passed by LRRC in R10 L2T redirection req msg, in the
                  priority order of high_cnt irat acq db freq, redirected target freq list, low_cnt irat acq db freq,
                  lte sib6 ngbr freqs.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_rmv_dup_freq_from_lte_redirect_req_r10(void)
{
  uint8 i;
  tds_rrc_lte_tds_neigh_list_type temp_freq_list; /* temp holder for the non-dup freqs */
  boolean add_to_list;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: check dup freqs in target list/irat acq db/sib6 ngbrs");

  /* rmv any duped freq in high_cnt irat acq db */
  if (tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list.uarfcn_count > 0)
  	{
	  memset(&temp_freq_list, 0, sizeof(tds_rrc_lte_tds_neigh_list_type));
  	  for (i=0; i<tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list.uarfcn_count &&
	  	        i<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ &&
	  	        temp_freq_list.uarfcn_count<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ; i++)
  	  {
	    if(tdsrrccsp_is_supported_band(tdsrrc_get_frequency_band((tdsrrc_freq_type)tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list.uarfcn[i])))
	    {
		  add_to_list = TRUE;
          if (tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list(
		  	  tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list.uarfcn[i], &temp_freq_list)
		  	 )
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R10 high_cnt irat acq db freq %d,dup with prev high_cnt freqs",tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list.uarfcn[i]);
            add_to_list = FALSE;
	      }
		  if (add_to_list)
		  {
		    temp_freq_list.uarfcn[temp_freq_list.uarfcn_count++] = tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list.uarfcn[i];
		  }
		}
		else
		{
		  TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore unsupported R10 high_cnt irat acq db freq %d",tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list.uarfcn[i]);
		}
  	  }
	  tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list = temp_freq_list;
  	}

  /* rmv any redirect target freqs duped in high_cnt irat acq db freq list, or in the target freq list itself */
  if (tdsrrc_csp_int_data.tdd_target_freq_list.uarfcn_count > 0)
  {
    memset(&temp_freq_list, 0, sizeof(tds_rrc_lte_tds_neigh_list_type));
    for (i=0; i<tdsrrc_csp_int_data.tdd_target_freq_list.uarfcn_count &&
		      i<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ &&
		      temp_freq_list.uarfcn_count<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ; i++)
  	{
	  if(tdsrrccsp_is_supported_band(tdsrrc_get_frequency_band((tdsrrc_freq_type)tdsrrc_csp_int_data.tdd_target_freq_list.uarfcn[i])))
	  {
        add_to_list = TRUE;
		if (tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list(
            tdsrrc_csp_int_data.tdd_target_freq_list.uarfcn[i], &tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list)
           )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: keep R10 redirect target freq %d,dup with high_cnt irat acq db freqs",tdsrrc_csp_int_data.tdd_target_freq_list.uarfcn[i]);
          add_to_list = TRUE;
        }
        /* checking of dup freq in R10 redirect target freq list itself has been performed when processing the l2t redirect req msg and save to
                  tdsrrc_csp_int_data.tdd_target_freq_list. Bypass this step. */
        if (add_to_list)
        {
          temp_freq_list.uarfcn[temp_freq_list.uarfcn_count++] = tdsrrc_csp_int_data.tdd_target_freq_list.uarfcn[i];
        }
      }
	  else
	  {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore unsupported R10 redirect target freq %d",tdsrrc_csp_int_data.tdd_target_freq_list.uarfcn[i]);
	  }
   }
   tdsrrc_csp_int_data.tdd_target_freq_list = temp_freq_list;
  }

  /* rmv any low_cnf irat acq db freq duped in high_cnt irat acq db freq list, in redirect target freqs, or in low_cnt irat acq db freq list itself */
  if (tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn_count > 0)
  {
    memset(&temp_freq_list, 0, sizeof(tds_rrc_lte_tds_neigh_list_type));
    for (i=0; i<tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn_count &&
		      i<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ &&
		      temp_freq_list.uarfcn_count<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ; i++)
  	{
	  if(tdsrrccsp_is_supported_band(tdsrrc_get_frequency_band((tdsrrc_freq_type)tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i])))
	  {
        add_to_list = TRUE;
		if (tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list(
			tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i], &tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list)
		   )
		{
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R10 low_cnt acq db freq %d,dup with high_cnt irat acq db freqs",tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i]);
		  add_to_list = FALSE;
		}
		if (add_to_list && tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list(
			tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i], &tdsrrc_csp_int_data.tdd_target_freq_list)
		   )
		{
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R10 low_cnt acq db freq %d,dup with redirect target freqs",tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i]);
		  add_to_list = FALSE;
		}
		if (add_to_list && tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list(
			tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i], &temp_freq_list)
		   )
		{
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R10 low_cnt irat acq db freq %d,dup with prev low_cnt irat acq db freqs",tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i]);
		  add_to_list = FALSE;
		}
        if (add_to_list)
        {
          temp_freq_list.uarfcn[temp_freq_list.uarfcn_count++] = tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i];
        }
      }
	  else
	  {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore unsupported R10 low_cnt irat acq db freq %d",tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn[i]);
	  }
   }
   tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list = temp_freq_list;
  }

  /* rmv any lte sib6 ngbr freq duped in high_cnt irat acq db freq list, in redirect target freq list, in low_cnt irat acq db freq list,
        or in the lte sib6 ngbr freq list itself */
  if (tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn_count > 0)
  {
    memset(&temp_freq_list, 0, sizeof(tds_rrc_lte_tds_neigh_list_type));
    for (i=0; i<tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn_count &&
		      i<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ &&
		      temp_freq_list.uarfcn_count<TDSCDMA_RRC_LTE_MAX_SIB6_UTRAN_TDD_NEIGH_FREQ; i++)
  	{
	  if(tdsrrccsp_is_supported_band(tdsrrc_get_frequency_band((tdsrrc_freq_type)tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i])))
	  {
        add_to_list = TRUE;
		if (tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list(
			tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i], &tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list)
		   )
		{
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R10 sib6 ngbr freq %d,dup with high_cnt irat acq db freqs",tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i]);
		  add_to_list = FALSE;
		}
		if (add_to_list && tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list(
			tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i], &tdsrrc_csp_int_data.tdd_target_freq_list)
		   )
		{
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R10 sib6 ngbr freq %d,dup with redirect target freqs",tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i]);
		  add_to_list = FALSE;
		}
		if (add_to_list && tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list(
			tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i], &tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list)
		   )
		{
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R10 sib6 ngbr freq %d,dup with low_cnt irat acq db freqs",tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i]);
		  add_to_list = FALSE;
		}
		if (add_to_list && tdsrrccsp_is_uarfcn_in_lte_redirect_info_freq_list(
			tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i], &temp_freq_list)
		   )
		{
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore R10 sib6 ngbr freq %d,dup with prev sib6 ngbr freqs",tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i]);
		  add_to_list = FALSE;
		}
        if (add_to_list)
        {
          temp_freq_list.uarfcn[temp_freq_list.uarfcn_count++] = tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i];
        }	
      }
	  else
	  {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: ignore unsupported R10 sib6 ngbr freq %d",tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn[i]);
	  }
   }
   tdsrrc_csp_int_data.ngbr_tdd_freq_list = temp_freq_list;
 }
 TDSRRC_MSG4(MSG_LEGACY_HIGH,"L2TD: R10, after rmv dup freqs, high_cnt acqdb freq: %d, target freq: %d, low_cnt acqdb freq: %d, sib6 ngbr: %d",
			 tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list.uarfcn_count,
             tdsrrc_csp_int_data.tdd_target_freq_list.uarfcn_count,
			 tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn_count,
			 tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn_count);
}

/*===========================================================================

FUNCTION          tdsrrccsp_update_target_tdd_cell_list_freqs

DESCRIPTION       This function updates the uarfcns in tdsrrc_csp_int_data.target_tdd_cell_list to be used for cell selection
                  during L2T redirection. For R9 redirect, the priority order is: irat acq db freqs (stored in
                  low_cnt_irat_acqdb_freq_list), lte sib6 ngbr freqs. For R10 redirect, the priority order is: high cnt
                  irat acq db freq list, redirect target freq list, low cnt irat acq db freq list, lte sib6 ngbr freq list.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_update_target_tdd_cell_list_freqs(void)
{
  uint8 i;
  uint8 num_of_frequencies = 0;
  tds_rrc_lte_tds_neigh_list_type *selected_freq_list = NULL;

  /* clear uarfcn info in tdsrrc_csp_int_data.target_tdd_cell_list before loading new freq list */
  tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies = 0;

  tdsrrc_csp_int_data.l2t_redirect_irat_acqdb_selected = FALSE;
  tdsrrc_csp_int_data.target_tdd_cell_source = TDSRRC_CSP_TARGET_CELL_SOURCE_NONE;

  if (tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list.uarfcn_count > 0)
  {
    /* freq order in irat acq db needs to be maintained when L1 performs ACQ, mark it so don't use ACQ_MODE_LFS */
	tdsrrc_csp_int_data.l2t_redirect_irat_acqdb_selected = TRUE;
	selected_freq_list = &tdsrrc_csp_int_data.irat_acqdb_high_cnt_freq_list;
    tdsrrc_csp_int_data.target_tdd_cell_source = TDSRRC_CSP_TARGET_CELL_SOURCE_ACQDB_HIGH_CNT;
	TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: try R10 high_cnt irat acqdb freqs");
  }
#ifdef FEATURE_L2TD_REDIRECT_R10
  else if (tdsrrc_csp_int_data.tdd_target_freq_list.uarfcn_count > 0)
  {
    selected_freq_list = &tdsrrc_csp_int_data.tdd_target_freq_list;
    tdsrrc_csp_int_data.target_tdd_cell_source = TDSRRC_CSP_TARGET_CELL_SOURCE_REDIR_TARGET;
	TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: try R10 redirected freqs");
  }
#endif
  else if (tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list.uarfcn_count > 0)
  {
    /* freq order in irat acq db needs to be maintained when L1 performs ACQ, mark it so don't use ACQ_MODE_LFS */
	tdsrrc_csp_int_data.l2t_redirect_irat_acqdb_selected = TRUE;
	selected_freq_list = &tdsrrc_csp_int_data.irat_acqdb_low_cnt_freq_list;
    /* for r9 redirect, dest freq will be tried first. So don't print msg here to avoid confusion. Will print at later point. */
	if (!tdsrrc_csp_int_data.l2t_r9_redirect)
    {
	TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: try low_cnt irat acqdb freqs");
  }
  }
  else if (tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9.uarfcn_count > 0)
  {
    selected_freq_list = &tdsrrc_csp_int_data.ngbr_tdd_freq_list_r9;
	/* for r9 redirect, dest freq will be tried first. So don't print msg here to avoid confusion. Will print at later point. */
  }
#ifdef FEATURE_L2TD_REDIRECT_R10
  else if (tdsrrc_csp_int_data.ngbr_tdd_freq_list.uarfcn_count > 0)
  {
    selected_freq_list = &tdsrrc_csp_int_data.ngbr_tdd_freq_list;
	TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: try R10 sib6 ngbr freqs");
  }
#endif
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2TD: All freq lists empty,except R9 redirected freq");
  }

  if (selected_freq_list)
  {
    for (i=0; i<selected_freq_list->uarfcn_count && num_of_frequencies<MAX_UTRAN_TDS_FREQUNECIES; i++)
    {
      tdsrrc_csp_int_data.target_tdd_cell_list.fdd_cell_list[num_of_frequencies++].uarfcn = selected_freq_list->uarfcn[i];
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: freq %d added to target_tdd_cell_list",selected_freq_list->uarfcn[i]);
    }
    if (num_of_frequencies == MAX_UTRAN_TDS_FREQUNECIES)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: loaded max nbr of %d freqs,ignore remaining freqs",MAX_UTRAN_TDS_FREQUNECIES);
    }
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"L2TD: target_tdd_cell_list updated with %d freqs",num_of_frequencies);
    tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies = num_of_frequencies;
    /* clear selected_freq_list so it won't be tried again next time */
    selected_freq_list->uarfcn_count = 0;
  }
}
#endif

/*===========================================================================

FUNCTION          tdsrrc_mt_call_is_there_call_on_other_sub

DESCRIPTION       This function checks whether MT CS call is on other sub.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  TRUE for MT CS call on the other sub.
SIDE EFFECTS
                  None

===========================================================================*/
boolean tdsrrc_mt_call_is_there_call_on_other_sub(void)
{
  boolean ret = FALSE;
  /* Get the request lock SUB ID*/
#ifdef FEATURE_TDSCDMA_DUAL_SIM
  sys_modem_as_id_e_type gsm_asid = 0;
  boolean get_as_id = FALSE;

  trm_client_enum_t unlock_cli_id = tds_get_current_unlock_client_id();

  if ((unlock_cli_id >= TRM_1X) && (unlock_cli_id <= TRM_LAST_CLIENT))
  {  
     get_as_id = sp_get_asid_for_trm_client(unlock_cli_id, &gsm_asid);
     if(get_as_id && ((gsm_asid >= SYS_MODEM_AS_ID_1) && (gsm_asid <= SYS_MODEM_AS_ID_3)))
     {
       ret = mm_per_subs_is_mt_page_in_progress(gsm_asid);
     }
  }
  
  TDSRRC_MSG4(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: unlock cli %d, get AS_ID succ %d, SUB ID is %d, MT call on other sub(%d)", unlock_cli_id,get_as_id,gsm_asid,ret);
#endif

  return ret; 
}
/*===========================================================================

FUNCTION          tdsrrccsp_bar_forbid_cells

DESCRIPTION       This function the forbidden cell status

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/

boolean tdsrrccsp_bar_forbid_cells(tdsrrc_SysInfoType3 *sib3_ptr,
                                      rrc_plmn_identity_type plmn_id, tdsrrc_MasterInformationBlock *mib_ptr,tdsrrc_SysInfoType1 *sib1_ptr)
{

  uint32 cell_id = 0;
  uint8 i, num_of_shared_plmns = 0, shared_plmn_idx = 0;
  sys_plmn_id_s_type sys_plmn_id = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(plmn_id);
  boolean nw_shared = FALSE;
  rrc_plmn_identity_type shared_plmn_list[TDSMAX_SHARED_PLMNS];
  rrc_lai_type lai;
  uint32 lac = 0;
  unsigned char  *tdsdata_ptr = NULL;
  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_common_info;
  
  if ( (sib3_ptr == NULL)||(mib_ptr == NULL)||(sib1_ptr == NULL) )
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Forbidden_Cell:there is one NULL pointer , SIB3 = %d, MIB = %d, SIB1 = %d",sib3_ptr,mib_ptr,sib1_ptr);
    return FALSE;
  }
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"Forbidden_Cell:check the cell status now.");
  cell_id = tdsrrc_translate_cell_id(&(sib3_ptr->cellIdentity));
    /* The SIB1 is present */
  nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;
  /* Check if the RRC MM interface structure array can hold this info */
  if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Forbidden_Cell:ERR: Not enough space for Common NAS Sys Info (%d bytes)",sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
    return(FALSE);
  }

  /* Fill in the Common NAS system info now */
  tdsdata_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
  for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
  {
    nas_common_info.value[i] =  *tdsdata_ptr;
    tdsdata_ptr ++;
  }

  if(mm_extract_lac_from_sib_info(&lai, &nas_common_info) == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Forbidden_Cell:Could not extract LAI from NAS Common sys info");
    return FALSE;
  }

  if((tdsrrc_csp_int_data.curr_req_plmn.plmn_type == RRC_GSM_MAP_ANY_PLMN_REQ) ||
      ((RRC_EST_EMERGENCY_CALL == tdsrrcrce_get_rrc_est_cause()) && (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)))
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Forbidden_Cell:PLMN type is %d, est cause is %d, curr scan is %d ",
                   tdsrrc_csp_int_data.curr_req_plmn.plmn_type , tdsrrcrce_get_rrc_est_cause(), tdsrrc_csp_int_data.curr_scan);
    return FALSE;
  }
#if 0
  lac = (uint32)b_unpackw(lai.lac, 0, 16); 
#endif

  
  lac = lai.lac[0];

  lac = (lac << 8) | lai.lac[1];
  TDSRRC_MSG3(MSG_LEGACY_ERROR,"Forbidden_Cell:ERR: LAC form sib %d,%d, coverted %d ",lai.lac[0],lai.lac[1],lac );
  if(tdsrrc_is_nw_shared(mib_ptr))
  {
    nw_shared = TRUE;
    num_of_shared_plmns = tdsrrccsp_construct_multiple_plmn_list_from_mib(
                                                                       mib_ptr,
                                                                       shared_plmn_list);
  }


  do
  {
    if(nw_shared)
    {
      plmn_id = shared_plmn_list[shared_plmn_idx++];
      sys_plmn_id = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(plmn_id);
    }
    if (tdsrrccsp_compare_forbidden_cell(sys_plmn_id,cell_id,lac))
    {
      TDSRRC_MSG5(MSG_LEGACY_HIGH,"Forbidden_Cell:Mached forbidden cell found PLMN id is 0x%X 0x%X 0x%X, cell id is %d ,lac is %d ",
                        sys_plmn_id.identity[0], sys_plmn_id.identity[1],sys_plmn_id.identity[2],
                         cell_id,lac);
      return TRUE;
    }
#if 0
    for(i = 0; i < tdsrrcnv_data.num_forbid_cells; i++)
    {
      if(
          (sys_plmn_match(tdsrrcnv_data.rrc_nv_forbid_cells[i].sys_plmn_id,sys_plmn_id)) &&
          (tdsrrcnv_data.rrc_nv_forbid_cells[i].cell_id == cell_id)&&(tdsrrcnv_data.rrc_nv_forbid_cells[i].lac == lac)
        )
      {
        return TRUE;
      }
    }
#endif
    
  }while(shared_plmn_idx < num_of_shared_plmns);

  return FALSE;

}

/*===========================================================================

FUNCTION          tdsrrccsp_check_and_send_bar_forbid_cells

DESCRIPTION       This function will check and send the cell

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/

void tdsrrccsp_check_and_send_bar_forbid_cells(tdsrrc_SysInfoType3 *sib3_ptr,rrc_plmn_identity_type plmn_id, 
                                                   tdsrrc_MasterInformationBlock *mib_ptr,tdsrrc_SysInfoType1 *sib1_ptr,tdsrrc_sib_status_e_type *status)
{
  if ( (sib3_ptr == NULL)||(mib_ptr == NULL)||(sib1_ptr == NULL) )
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Forbidden_Cell:there is one NULL pointer , SIB3 = %d, MIB = %d, SIB1 = %d",sib3_ptr,mib_ptr,sib1_ptr);
    return;
  }
  if(tdsrrccsp_bar_forbid_cells(sib3_ptr,plmn_id,mib_ptr,sib1_ptr))
  {
     tdsrrc_csp_int_data.bar_for_resel_only = TRUE;
    tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_CPID_BARRED,TDSRRC_FORBIDDEN_CELL_BAR_TIMER_LENGTH);
    tdsrrc_csp_int_data.bar_for_resel_only = FALSE;
    *status = TDSSIB_EVENT_FAILED_CELL_BAR;
    return;
  }
}
/*===========================================================================

FUNCTION tdsrrccsp_compare_forbidden_cell

DESCRIPTION
  This  function is used to compare the cell whether it is forbidden cell form efs.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: it is Forbidden cell
  FAlsE:it is not Forbidden cell

===========================================================================*/
boolean tdsrrccsp_compare_forbidden_cell(sys_plmn_id_s_type sys_plmn_id,uint32 cell_id,uint32 lac)
{
  uint8 i = 0;
  rrc_plmn_identity_type plmn_id;
  
  plmn_id = mmumts_convert_nas_plmn_id_to_rrc_plmn_id(sys_plmn_id);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Forbidden_Cell:forbid_cells from SIB: mcc: %d, mnc : %d", 
                TDSRRC_CSP_GET_INT_MCC(plmn_id), 
                TDSRRC_CSP_GET_INT_MNC(plmn_id));
    for(i = 0; i < tdsrrcnv_data.forbid_cells.num_forbid_cell && i< RRC_NV_NUM_FORBID_CELLS ; i++)
    {
       plmn_id = mmumts_convert_nas_plmn_id_to_rrc_plmn_id(tdsrrcnv_data.forbid_cells.forbid_cell[i].sys_plmn_id);
       TDSRRC_MSG2(MSG_LEGACY_HIGH,"Forbidden_Cell:forbid_cells from NV: mcc: %d, mnc : %d", 
                    TDSRRC_CSP_GET_INT_MCC(plmn_id), 
                    TDSRRC_CSP_GET_INT_MNC(plmn_id));

                    
      TDSRRC_MSG4(MSG_LEGACY_HIGH,"Forbidden_Cell:forbid_cells from NV : cell_id: %d, lac : %d. from Sib, cell_id: %d, lac:%d", 
                    tdsrrcnv_data.forbid_cells.forbid_cell[i].cell_id, 
                    tdsrrcnv_data.forbid_cells.forbid_cell[i].lac,cell_id,lac);
      if((sys_plmn_match(tdsrrcnv_data.forbid_cells.forbid_cell[i].sys_plmn_id,sys_plmn_id)) &&
          (tdsrrcnv_data.forbid_cells.forbid_cell[i].cell_id == cell_id)&&(tdsrrcnv_data.forbid_cells.forbid_cell[i].lac == lac)
        )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Forbidden_Cell:Find the Forbidden cell index %d",i);
        return TRUE;
      }
    }
    return FALSE;
}


