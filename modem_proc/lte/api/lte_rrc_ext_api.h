/*!
  @file
  lte_rrc_ext_api.h

  @brief
  This file describes the API interfaces to LTE Radio resource Controller (RRC)
*/


/*===========================================================================

  Copyright (c) 2012 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/lte.mpss/6.6.1/api/lte_rrc_ext_api.h#1 $

when       who       what, where, why
--------   ---       ----------------------------------------------------------
02/28/12   kp        Moved definition of lte_rrc_get_3gpp_release_version to lte_rrc_ext_api.h
04/21/11   sk        Added prototype for lte_rrc_get_tdd_prune_required
04/20/11   np        lte_rrc_lte_tdd_is_supported & lte_rrc_lte_fdd_is_supported
08/05/10   np        Interface for Transfer of dedicated priorities during OOS
04/30/10   np        API to check if a DL EARFCN is supported based on Band Pref
03/19/10   sureshs   Added cell ID and frequency to API
03/18/10   sureshs   Created

===========================================================================*/

#ifndef LTE_RRC_EXT_API_H
#define LTE_RRC_EXT_API_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include <lte_rrc_ext_msg.h>
#include "nv_items.h"
#include "mcfg_fs.h"
#include <trm.h>

/*! @brief Invalid value for LTE sys band returned to the caller
*/
#define LTE_RRC_INVALID_LTE_SYS_BAND 0xFFFF

/* Inserted for L+L code changes
@TODO Need to remove once complete code has been changed for L+L*/
#define LTE_RRC_DUMMY_INSTANCE 0

#define LTE_RRC_MAX_STACKS 2

/*===========================================================================

                   EXTERNAL FUNCTIONS

===========================================================================*/

/*! @brief Enumeration of owners of dedicated priorities
*/
typedef enum
{
  LTE_RRC_DEDICATED_PRI_OWNER_NONE = 0,  /*!< No one owns them i.e. they are not set */
  LTE_RRC_DEDICATED_PRI_OWNER_LTE,       /*!< Owner is LTE  */
  LTE_RRC_DEDICATED_PRI_OWNER_WCDMA,     /*!< Owner is WCDMA */
  LTE_RRC_DEDICATED_PRI_OWNER_GERAN,     /*!< Owner is GERAN */
  LTE_RRC_DEDICATED_PRI_OWNER_TDSCDMA,   /*!< Owner is TDSCDMA */

  LTE_RRC_DEDICATED_PRI_OWNER_MAX

} lte_rrc_dedicated_pri_owner_e;

/*! @brief Enumeration of Cap IEs that need EFS support
DO NOT CHANGE ENUM VALUES
*/
typedef enum
{
  /* Disable IEs Bitmask */
  LTE_RRC_CAP_DISABLE_IE_TM8_TDD = 1, 
  // LTE_RRC_CAP_INTRE_FREQ_GAPLESS_MEAS_SUPPORT_FDD = 2, 
  LTE_RRC_CAP_DISABLE_IE_OTDOA_RSTD_TDD_INTER = 3,
  LTE_RRC_CAP_DISABLE_IE_OTDOA_RSTD_FDD_INTER = 4,
  LTE_RRC_CAP_DISABLE_NON_CA_BAND_SUPPORT = 5,
  LTE_RRC_CAP_DISABLE_REQ_FREQ_BAND_SUPPORT = 6,
  // LTE_RRC_CAP_DISABLE_UL_64_QAM = 7, 
  LTE_RRC_CAP_DISABLE_IE_FREQ_BAND_PRIORITY_ADJUST = 8, //Enabled by default for R10+ UEs
  LTE_RRC_CAP_DISABLE_IE_DL_256QAM = 9,
  LTE_RRC_CAP_DISABLE_IE_3DL_CA = 10, 
  LTE_RRC_CAP_DISABLE_IE_UL_CA = 11, 
  LTE_RRC_CAP_DISABLE_FDD_TDD_CA_WITH_TA = 12,
  LTE_RRC_CAP_DISABLE_IE_UL_64_QAM = 13,
  LTE_RRC_CAP_DISABLE_ALT_TBS_IDX = 14,
  LTE_RRC_CAP_DISABLE_IE_TM9_FDD_8TX = 15,
  LTE_RRC_CAP_DISABLE_IE_INTER_BAND_ULCA = 21,
  LTE_RRC_CAP_DISABLE_IE_MOD_AMPR = 22,
  LTE_RRC_CAP_DISABLE_IE_OTDOA_RSTD_TDD_INTRA = 29,
  LTE_RRC_CAP_DISABLE_IE_OTDOA_RSTD_FDD_INTRA = 30, 
  LTE_RRC_CAP_DISABLE_IE_TDD_PCELL_DUPLEX = 31,
  LTE_RRC_CAP_DISABLE_IE_FDD_PCELL_DUPLEX = 32,
  LTE_RRC_CAP_DISABLE_IE_MAX = 64, 
  /* Enable IEs Bitmask, Should start with 65 */
  //LTE_RRC_CAP_NON_CA_BAND_SUPPORT = 65, 
  LTE_RRC_CAP_ENABLE_INTER_FREQ_GAPLESS_MEAS_SUPPORT_TDD = 66,
  LTE_RRC_CAP_ENABLE_IE_UTRA_PROXIMITY_IND = 67, 
  LTE_RRC_CAP_ENABLE_IE_EUTRA_PROXIMITY_IND = 68, 
  LTE_RRC_CAP_ENABLE_IE_3DL_TDD_CA = 70,
  //LTE_RRC_CAP_ENABLE_IE_TM8_TDD = 71,
  //LTE_RRC_CAP_ENABLE_IE_OTDOA_RSTD_TDD = 72,
  //LTE_RRC_CAP_ENABLE_IE_OTDOA_RSTD_FDD = 73,
  LTE_RRC_CAP_ENABLE_IE_DL_256QAM = 74, //No longer in use
  LTE_RRC_CAP_ENABLE_IE_PMI_RI_RPT_DISABLE_TDD = 76,
  LTE_RRC_CAP_ENABLE_IE_PMI_RI_RPT_DISABLE_FDD = 77,
  //LTE_RRC_CAP_ENABLE_IE_FREQ_BAND_PRIORITY_ADJUST = 78, //Disabled by default R11/R11 non-CMCC UEs
  LTE_RRC_CAP_ENABLE_IE_MULTI_CLUSTER_PUSCH = 79,
  LTE_RRC_CAP_ENABLE_IE_UL_64_QAM_R8_IE = 80,  //For test purpose only
  //LTE_RRC_CAP_ENABLE_FDD_TDD_CA_WITH_TA = 81,
  LTE_RRC_CAP_ENABLE_IE_3DL_CA_DR = 82, 
  LTE_RRC_CAP_ENABLE_IE_UL_CA_DR = 83,
  LTE_RRC_CAP_ENABLE_IE_INTER_BAND_UL_CA_DR = 84,
  LTE_RRC_CAP_ENABLE_INTER_FREQ_GAPLESS_MEAS_SUPPORT_FDD = 85,
 //LTE_RRC_CAP_ENABLE_ALT_TBS_IDX = 86,
  // LTE_RRC_CAP_ENABLE_IE_INTER_BAND_ULCA = 94,
  LTE_RRC_CAP_ENABLE_IE_MAX = 128
} lte_rrc_cap_ies_e;

/* @brief MACRO definitions for 64 bit mask for features controlled/voted by PM */
typedef uint64 lte_pm_cap_feature_mask_t;
typedef enum
{
  LTE_PM_CAP_FEATURE_MASK_APS         = ((lte_pm_cap_feature_mask_t) 0x0001)
}lte_pm_cap_feature_mask_defn_bit_e;

#define LTE_PM_CAP_ALL_FEATURES_ENABLED      ((lte_pm_cap_feature_mask_t) ~0)

/* 
  Returns the current RRC state based on RRC Controller module. This API
  is meant for use by external modules to determine LTE RRC state.
*/
lte_rrc_state_e lte_rrc_get_rrc_state
(
   lte_rrc_mem_instance_type instance
);

/* Returns the PLMN selected and other info for camping. For limited service, it
   returns the first PLMN in SIB1 list. Returns TRUE if success, else FALSE.
*/
extern boolean lte_rrc_get_camped_cell_info
(
  lte_rrc_mem_instance_type instance,
  lte_rrc_plmn_s *selected_plmn_ptr,
   /*!< Pointer to selected PLMN to be filled*/
  lte_earfcn_t *dl_earfcn_ptr, /*!< DL Center frequency of cell */
  lte_phy_cell_id_t *phy_cell_id_ptr,/*!< Physical cell id of cell */
  lte_rrc_global_cell_id_t *cell_identity_ptr,
  /*!< Global cell id (from SIB1) of cell */
  uint16 *tac_ptr /*!< Tracking area code of cell */
);

/* Returns the primary PLMN  and other camped cell info
*/
extern boolean lte_rrc_get_camped_cell_info_with_primary_plmn
(
  lte_rrc_mem_instance_type instance,
  lte_rrc_plmn_s *primary_plmn_ptr,
   /*!< Pointer to primary PLMN to be filled*/
  lte_earfcn_t *dl_earfcn_ptr, /*!< DL Center frequency of cell */
  lte_phy_cell_id_t *phy_cell_id_ptr,/*!< Physical cell id of cell */
  lte_rrc_global_cell_id_t *cell_identity_ptr,
  /*!< Global cell id (from SIB1) of cell */
  uint16 *tac_ptr /*!< Tracking area code of cell */
);

/* Returns TRUE if band corresponding to the DL EARFCN is supported in the LTE band Pref */
extern boolean lte_rrc_earfcn_is_supported
(
  lte_rrc_mem_instance_type instance,
  lte_earfcn_t earfcn, /*!< DL EARFCN */
  sys_lte_band_mask_e_type lte_band_preference /*!< LTE band preference */
);

/* Function to get the owner (stack) of dedicated priorities */
extern lte_rrc_dedicated_pri_owner_e lte_rrc_dedicated_pri_get_owner
(
  lte_rrc_mem_instance_type instance
);

/* Function to set the owner (stack) of dedicated priorities, returns TRUE if owner is updated successfully */
extern boolean lte_rrc_dedicated_pri_set_owner
(
  lte_rrc_mem_instance_type instance,
  lte_rrc_dedicated_pri_owner_e owner
);

/* Function to determine if a cell's downlink arfcn belongs to TDD or FDD bands */
extern boolean lte_rrc_cell_is_tdd
(
  lte_earfcn_t dl_earfcn
);

/* Returns TRUE if any of the FDD bands in the LTE band Pref are present */
extern boolean lte_rrc_lte_fdd_is_supported
(
  sys_lte_band_mask_e_type lte_band_preference /*!< LTE band preference */
);

/* Returns TRUE if any of the TDD bands in the LTE band Pref are present */
extern boolean lte_rrc_lte_tdd_is_supported
(
  sys_lte_band_mask_e_type lte_band_preference /*!< LTE band preference */
);

/* This API Returns TRUE if the input band is TDD, FALSE otherwise. */
extern boolean lte_rrc_lte_band_is_tdd
(
  sys_sband_lte_e_type band  /*!< Band enum */
);

/* This API Returns TRUE if current serving cell is belonging to CN mainland*/
extern boolean lte_rrc_csp_is_matching_cm_mcc
(
lte_rrc_mem_instance_type instance
);

/* Returns TRUE if TDD pruning required */
extern boolean lte_rrc_get_tdd_prune_required
(
  lte_rrc_mem_instance_type instance
);

/* Function to override default 10sec timer value for W2L Redirection List timer,

   Valid values are 1000ms - 10000ms, anything outside the range will default to spec
   defined value.

   If timer is already running, the value takes effect the next time it runs.

   Function returns FALSE if LTE SW is not in a state to accept value*/
extern boolean lte_rrc_irat_from_w_set_redir_list_tmr
(
  lte_rrc_mem_instance_type instance,
  uint64 test_time_ms
);

/* Function to override default timer value for G2L Redirection List timer,
   and deciding Lte search behvior for the given timer.

   Valid timer (test_time_ms) values are 1000ms - 10000ms, anything outside the range will default to spec
   defined value.

   Valid behavior (list_srch_only) values,
   TRUE  - will perform list search contineously on the provided frequencies till timer expires.
   FALSE - will perform one round of list search followed by one round of full search before timer expires.

   If timer is already running, the value takes effect the next time it runs.

   Function returns FALSE if LTE SW is not in a state to accept value*/
extern boolean lte_rrc_irat_from_g_set_redir_list_tmr
(
  lte_rrc_mem_instance_type instance,
  uint64  test_time_ms,
  boolean list_srch_only
);
/*Provides LTE-RRC release version*/
__attribute__((section(".uncompressible.text")))
extern lte_3gpp_release_ver_e lte_rrc_get_3gpp_release_version
(
  lte_rrc_mem_instance_type instance
);

/*Delete Acq db EFS file. Returns 0 if success, -1 in case of failure or EFS is missing*/
extern mcfg_fs_status_e_type lte_rrc_delete_acq_db
(
  lte_rrc_mem_instance_type instance
);
/* Function to provide decoded SI info */
extern int lte_rrc_get_decoded_sib_string
(
  lte_rrc_mem_instance_type instance,
  void *encoded_si_ptr, /*!< Pointer to encoded SI */
  uint32 encoded_si_size, /*!< Size of encoded SI */
  void *buffer_ptr,
  uint32 buffer_length
);

/*API which allow setting LTE-RRC FGI*/
boolean lte_rrc_cmapi_fgi_change_req
(
  lte_rrc_mem_instance_type instance,
  lte_fgi_rel_type_e fgi_rel_type, /*!< Indicate if the FGI is for Rel-8/9/10 */
  uint32 fgi_word, /*!< 32 bit LTE Rel-8/9/10 FGI word, ,
  MSB(Bit -31) is FGI-1 or 33 or 101 &  LSB(Bit -0) is FGI-32 or 64 or 132 */
  lte_rrc_div_duplex_e eutra_mode  /*! Indicates whether to update FDD FGI or TDD FGI*/
);

/*API which requests RRC to reread specific EFS files*/
boolean lte_rrc_cmapi_efs_reread_req
(
  /*!
    Bit position
(starting from right most)	EFS file intended to be reread by LTE RRC
    1	                         /nv/item_files/modem/lte/common/lte_category
    2	                         /nv/item_files/modem/lte/common/ca_disable
    3	                         /nv/item_files/modem/lte/rrc/cap/fgi
    4	                         /nv/item_files/modem/lte/rrc/cap/fgi_tdd
    5	                         /nv/item_files/modem/lte/rrc/cap/fgi_rel9
    6	                         /nv/item_files/modem/lte/rrc/cap/fgi_tdd_rel9
    7	                         /nv/item_files/modem/lte/rrc/cap/fgi_r10
    8	                         /nv/item_files/modem/lte/rrc/cap/fgi_tdd_rel10
    */
  lte_rrc_mem_instance_type instance,
  uint64 efs_bitmask
);

/*API which allow setting LTE-RRC PCI_LOCK Info*/
boolean lte_rrc_cmapi_set_pci_lock_req
(
  lte_rrc_mem_instance_type instance,
  lte_earfcn_t  dl_earfcn,
  /*! PCI */
  uint16 pci
);


/*API which allow setting LTE-RRC CAP band preference info*/
lte_errno_e lte_rrc_cmapi_set_ue_cap_band_preferences
(
  lte_rrc_mem_instance_type      instance,
  sys_lte_band_mask_e_type       lte_band_preference,
  sys_band_mask_type             wcdma_band_preference,
  sys_band_mask_type             gsm_band_preference
);


/*LTE RRC API to provide CMAPI based LTE band preferences to W*/
void lte_rrc_cmapi_get_lte_cap_band_preference
(
  lte_rrc_mem_instance_type instance,
  sys_lte_band_mask_e_type       *lte_band_preference
);

/*API which allow setting LTE-RRC EARFCN_LOCK Info*/
boolean lte_rrc_cmapi_set_earfcn_lock_req
(
  lte_rrc_mem_instance_type instance,
  lte_earfcn_t  dl_earfcn1,
  lte_earfcn_t  dl_earfcn2
);
/* API will return eMBMS coverage status
  0 - implies is not camped on an LTE cell or
      implies both SIB13 and SIB15 are not scheduled in SIB1 of current cell,
  1 - implies SIB13 and optionally SIB15 scheduled in SIB1 of current cell,
  2 - implies SIB15 alone scheduled in SIB1 of current cell. */
extern uint8 lte_rrc_cmapi_get_embms_coverage_status
(
  lte_rrc_mem_instance_type instance
);

/* API will return MTCH data MCS for all PMCHs that are actively being monitored.
  Caller needs to allocate memory for the passed parameter */
extern void lte_rrc_cmapi_get_pmch_datamcs_info
(
  lte_rrc_mem_instance_type instance,
  lte_rrc_datamcs_info_s *datamcs_info_ptr /*!< Ptr to datamcs info */
);

/* API will return MBSFN SNR of all active TMGIs. When TMGIs are not active this API
  returns unicast SNR. The below API call is blocking as RRC has to use msgr framework
  to fetch SNR info from ML1 */
extern void lte_rrc_cmapi_get_mbsfn_snr
(
  lte_rrc_mem_instance_type instance,
  lte_rrc_mbsfn_snr_info_s *mbsfn_snr_info_ptr /*!< Ptr to MBSFN SNR info */
);

/* API will return total number of RLFs, i.e.
      1) RLF Count since LTE is active
      2) RLF count since RRC Connected*/
extern boolean lte_rrc_cmapi_get_num_rlf
(
  lte_rrc_mem_instance_type instance,
  uint8* rlf_count_since_lte_active,
  uint8* rlf_count_since_rrc_connected
);


/*Provides LTE band for the given earfcn. band_is_valid must be verified before use*/
extern sys_sband_lte_e_type lte_rrc_get_band_from_dl_earfcn
(
  lte_rrc_mem_instance_type instance,
  lte_earfcn_t dl_earfcn, /*!< DL EARFCN */
  boolean *band_is_valid_ptr /*!< Is it valid? */
);

/* This function returns an array of LTE_CSG_cells (max LTE_IRAT_MAX_MACRO_CELLS)
   associated with the W_Macro_Ptr */
extern uint8 lte_rrc_csg_get_fp_lcsg_info_for_w
(
  lte_rrc_mem_instance_type instance,
 const source_cell_info_type *w_macro_ptr, /*! W MAcro cell info ptr */
 source_cell_info_type *l_csg_cells_ptr /*! LTE CSG cells array */
);

/* Convert band number to band enum (sys_sband_lte_e_type) */
extern sys_sband_lte_e_type lte_rrc_band_from_int_to_enum
(
  uint16 band  /*!< Band enum */
);

/* Converts earfcn from one band to another (MFBI band combos only allowed) */
extern errno_enum_type lte_rrc_convert_dl_earfcn_by_band
(
  lte_rrc_mem_instance_type instance,
  lte_earfcn_t *dl_earfcn,
  sys_sband_lte_e_type from_band,
  sys_sband_lte_e_type to_band
);

/* Converts earfcn to freq in 100 Mhz */
extern uint16 lte_rrc_get_freq_from_earfcn
(
  lte_rrc_mem_instance_type instance,
  lte_earfcn_t earfcn /*!< Earfcn */
);

extern lte_errno_e lte_rrc_csp_get_global_cell_id(lte_rrc_global_cell_id_t *cell_id_ptr);


/*Provides if LTE MFBI is supported or not*/
extern boolean lte_rrc_cap_mfbi_is_supported
(
  lte_rrc_mem_instance_type instance,
  boolean is_tdd
);

/*Provides if LTE OTDOA is supported or not*/
extern boolean lte_rrc_cap_inter_freq_rstd_meas_supported
(
  lte_rrc_mem_instance_type instance
);

/*Reads the specified NV flag. Read value is populated in item_ptr
  If the NV reading activity is successful, return TRUE, else return FALSE*/
boolean lte_nv_read
(
  nv_items_enum_type nv_item, /* Item to get */
  nv_item_type      *item_ptr /* Pointer where to put the item */
);

boolean lte_rrc_cap_is_feature_supported
(
  lte_rrc_mem_instance_type instance,
  lte_rrc_cap_ies_e cap_ie
);

extern uint8 lte_rrc_cap_inter_freq_gap_less_meas_enabled
(
  lte_rrc_mem_instance_type instance,
  sys_sband_lte_e_type pcell_band,
  sys_sband_lte_e_type scell_band
);

extern boolean lte_rrc_get_lted_feature_status(void);
extern sys_lte_band_mask_e_type lte_rrc_cap_get_lte_band_pref_in_cap_info
(
  lte_rrc_mem_instance_type instance
);

extern sys_sys_mode_e_type lte_rrc_plmn_search_get_curr_search_sys_mode
(
  lte_rrc_mem_instance_type instance
);

/* LTE RRC API to provide value of hold timer 1st tick to NAS */
extern uint32 lte_rrc_get_hold_timer_1st_tick_value
(
  lte_rrc_mem_instance_type instance
);

extern void lte_rrc_meas_trm_fall_back_info_handler
(
  lte_rrc_mem_instance_type instance,
  trm_async_callback_information_type *cb_info
);

extern lte_cphy_power_class_e lte_rrc_get_serv_cell_power_class
(
  lte_rrc_mem_instance_type instance
);

extern boolean lte_rrc_get_inter_rat_bplmn_prtl_result
(
  lte_rrc_mem_instance_type instance,
  sys_detailed_plmn_list_s_type *available_plmn_list
);

#endif /* LTE_RRC_EXT_API_H */

