#ifndef GERAN_NV_H
#define GERAN_NV_H

/*! \file geran_nv.h
 
  This is the header file for geran_nv.c.
  This module contains access functions for NV data.
 
                Copyright (c) 2012-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


$Header: //components/rel/geran.mpss/7.4.0/gcommon/inc/geran_nv.h#3 $ 
$DateTime: 2020/09/18 02:47:25 $$Author: pwbldsvc $

when       who      what, where, why
--------   -------- ---------------------------------------------
25/04/18   rv      CR2225983 - GERAN F3 and log pkt Reduction TA.3.0
05/01/18   bk       CR2166681 add EFS control to FR 37926
25/01/16   cws      CR962379 Disable/enable ASDIV in IDLE/RACH state by NV
15/12/2016  ksb       CR1041734 Error recovery handing at regular interval using NV control
29/03/16   mn        CR995137 Access tune away support: Allowing other tech's paging at GPRS access
06/07/17   mk       CR2071218 FR44121 -Synchronous Ncell Quality Monitor - Added NV control
02/02/17   br       CR1103497 FR 39735: Allowing TA during PAGE RE-ORG mode in GERAN
21/04/17   br       CR1041084 FR 36219: GDET – GMSK Modulation Detector
14/02/17   mk      CR2004610 : FR 40251 (SeNS) - Add NV item to enable SeNS feature
16/03/17   mn      CR2020307 FR40846:Packet transfer Error Recovery mechanism.
03/09/15   kb       CR863908:Added new EFS file structure for sleep control
12/08/15   cws      CR864883  FR27546+FR27544 Add MTPL/power imbalance factors into ASDIV algorithm 
10/03/15   sjv      CR805970: SCell RSSI Threshold set through a new NV for DR to SR Fallback
23/02/15   sjv      CR799313: Using new mcfg APIs replacing efs_get for Geran

===========================================================================*/


/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "customer.h"
#include "comdef.h"
#include "sys.h"
#include "geran_dual_sim.h"
#include "mcfg_fs.h"
#include "grm_type.h"


/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define GERAN_TA_SUPPORTED 1
#define GERAN_TA_NOT_SUPPORTED 0
#define GERAN_NV_MEM_NOT_VALID (-1)

#ifdef FEATURE_DATA_PDU_DEBUG
/* Data PDU debug mask */
#define GERAN_DATA_PDU_DEBUG_GL1            0x01
#define GERAN_DATA_PDU_DEBUG_GRLC_UL        0x02
#define GERAN_DATA_PDU_DEBUG_GRLC_DL        0x04
#define GERAN_DATA_PDU_DEBUG_GLLC_FCS_ERR   0x08
#define GERAN_DATA_PDU_DEBUG_GLLC_GEA_TEST  0x10
#define GERAN_DATA_PDU_DEBUG_GLLC_CIPHER    0x20
#define GERAN_DATA_PDU_DEBUG_GLLC_FULL_FRM  0x40
#endif /* FEATURE_DATA_PDU_DEBUG */


/* GERAN macros for calling MCFG APIs */
#define GERAN_EFS_OPEN(path,oflag,mode,as_id) mcfg_fopen(path, oflag, mode, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)as_id)
#define GERAN_EFS_MKDIR(path,mode) mcfg_mkdir(path, mode, MCFG_FS_TYPE_EFS)
#define GERAN_EFS_WRITE(filedes,buf,size) mcfg_fwrite(filedes, buf, size, MCFG_FS_TYPE_EFS)
#define GERAN_EFS_CLOSE(filedes) mcfg_fclose(filedes,MCFG_FS_TYPE_EFS)
#define GERAN_EFS_DELETE(path,as_id) mcfg_fs_delete(path, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)as_id)

/*NV Item to Set the Serving Cell RSSI threshold for DR to SR Fallback */
#define GERAN_EFS_DR_TO_SR_FALLBACK_RSSI_THRESH "/nv/item_files/gsm/gl1/gsm_dr_to_sr_fallback_rssi_thresh"
/*Default value of gsm_dr_to_sr_fallback_rssi_thresh as -102 dBm*/
#define GSM_DR_TO_SR_FALLBACK_THRESH_DEF   (-102)
#define GERAN_MAX_DSC_PERCENT               100

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * EFS Item paths
 * -------------------------------------------------------------------------*/
#define GERAN_EFS_ANT_SWTCHNG_TX_DIV_EN   "/nv/item_files/gsm/gl1/antenna_switching_tx_diversity_enable"
#define GERAN_EFS_ANT_SWTCHNG_TX_DIV_TH   "/nv/item_files/gsm/gl1/antenna_switching_tx_diversity_threshold"
#define GERAN_EFS_ANT_SWTCH_TX_DIV_MIN_DELTA_BTWN_ANT "/nv/item_files/gsm/gl1/antenna_switch_tx_div_min_delta_btw_antenna"
#define GERAN_EFS_WCDMA_L1_QTA_ROTA "/nv/item_files/wcdma/l1/wl1_qta_rota"
#define GERAN_EFS_GBTA_SUPPORT "/nv/item_files/modem/geran/gbta_support"
#define GERAN_EFS_LTE_CELL_RSRP_TH "/nv/item_files/modem/geran/lte_cell_rsrp_threshold"
#define GERAN_EFS_COMM_RECOVERY_RESTART "/nv/item_files/gsm/gl1/gsm_commercial_recovery_restart"
#define GERAN_EFS_SRCH_ALL_W_CELL "/nv/item_files/gsm/gl1/search_all_w_cell"
#define GERAN_EFS_DEF_RF_CHAIN "/nv/item_files/modem/geran/default_rf_chain"
#define GERAN_EFS_G2X_TA_SUPPORT "/nv/item_files/modem/geran/g2x_ta_support"

#define GERAN_EFS_ANT_SWTCHNG_GSM_T2_IDL_SUSP_TIME "/nv/item_files/gsm/gl1/antenna_switching_gsm_type_2_idle_suspension_time"
#define GERAN_EFS_ANT_SWTCHNG_GSM_T2_TRFC_CRISIS_MODE_THRESH "/nv/item_files/gsm/gl1/antenna_switching_gsm_type_2_traffic_crisis_mode_thre"
#define GERAN_EFS_ANT_SWTCHNG_GSM_T2_IDL_PINGPONG_MIT_THRESH "/nv/item_files/gsm/gl1/antenna_switching_gsm_type_2_idle_pingpong_mitigation_thre"

#define GERAN_EFS_ANT_SWTCHNG_GSM_T2_EN "/nv/item_files/gsm/gl1/antenna_switching_gsm_type_2_en"
#define GERAN_EFS_ANT_SWTCHNG_GSM_TRAFI_MDM_TH "/nv/item_files/gsm/gl1/antenna_switching_gsm_trafi_mdm_threshold"
#define GERAN_EFS_ANT_SWTCHNG_GSM_IDLE_MDM_TH "/nv/item_files/gsm/gl1/antenna_switching_gsm_idle_mdm_threshold"
#define GERAN_EFS_ANT_SWTCHNG_GSM_TRAFI_SENS "/nv/item_files/gsm/gl1/antenna_switching_gsm_trafi_sens"
#define GERAN_EFS_ANT_SWTCHNG_GSM_IDLE_SENS "/nv/item_files/gsm/gl1/antenna_switching_gsm_idle_sens"
#define GERAN_EFS_ANT_SWTCHNG_GSM_QSC_LARGE_DELTA "/nv/item_files/gsm/gl1/antenna_switching_gsm_qsc_large_delta"
#define GERAN_EFS_ANT_SWTCHNG_GSM_QSC_SMALL_DELTA "/nv/item_files/gsm/gl1/antenna_switching_gsm_qsc_small_delta"
#define GERAN_EFS_ANT_SWTCHNG_GSM_QSC_HYST_TIME "/nv/item_files/gsm/gl1/antenna_switching_gsm_qsc_hysteriesis_time"
#define GERAN_EFS_ANT_SWTCHNG_GSM_UL_DL_DELTA_TABLE "/nv/item_files/gsm/gl1/antenna_switch_UL_DL_delta_table"
#define GERAN_EFS_ANT_SWTCHNG_GSM_CONTROL_MASK "/nv/item_files/gsm/gl1/antenna_switch_control_mask"

#define GL1_EFS_MDSP_DEBUG "/nv/item_files/modem/geran/gfw_debug"
#define GL1_EFS_MDSP_DIAG1 "/nv/item_files/modem/geran/gfw_diag_group1"
#define GL1_EFS_MDSP_DIAG2 "/nv/item_files/modem/geran/gfw_diag_group2"
#define GL1_EFS_MDSP_DIAG3 "/nv/item_files/modem/geran/gfw_diag_group3"
#define GL1_EFS_MDSP_DIAG4 "/nv/item_files/modem/geran/gfw_diag_group4"

#define GL1_EFS_DEBUG "/nv/item_files/gsm/l1/l1_debug"
#define GL1_EFS_SLP   "/nv/item_files/gsm/gl1/l1_sleep"
#define GL1_EFS_SLP_ERROR_REC "/nv/item_files/gsm/gl1/l1_sleep_error_rec"
#define GL1_EFS_MDSP_AEQPH4_CTL "/nv/item_files/modem/geran/gfw_aeq_phase4_control"
#define GL1_EFS_DEBUG_LIF_SUPPORT "/nv/item_files/gsm/gl1/lif_feature_control"

#define GL1_PARALLEL_SCH_SUPPORT "/nv/item_files/modem/geran/parallel_sch_support"
#define GL1_G2X_IDLE_TUNEAWAY_SUPPORT "/nv/item_files/modem/geran/g2x_idle_tuneaway_support"
#define GL1_NV_FW_RECOVERY "/nv/item_files/gsm/gl1/gl1_nv_gfw_recovery"

#define GL1_GDET_SUPPORT "/nv/item_files/modem/geran/gdet_support"
#define GL1_GDET_THRESHOLD "/nv/item_files/modem/geran/gdet_threshold"
#define GERAN_EFS_CAMP_FOR_EMERGENCY_CALL "/nv/item_files/modem/geran/camp_for_emergency_call_enabled"


#define GL1_EFS_PKT_RECOVERY   "/nv/item_files/gsm/l1/gl1_err_rec_enabled"

#define GL1_EFS_SYNQ_FR_CONTROL   "/nv/item_files/gsm/gl1/gl1_synq_fr_control"
#define GL1_NV_DISABLE_CONDITIONAL_LOGGING "/nv/item_files/gsm/gl1/gl1_disable_conditional_logging"
#define GL1_EFS_SENS_FR_CONTROL   "/nv/item_files/gsm/gl1/gl1_sens_fr_control"

/***********************************/
/* Hardware-Specific EFS items used by GRR */
/**********************************/
#define GERAN_EFS_G2X_MEASUREMENT_CONTROL "/nv/item_files/modem/geran/grr/g2x_measurement_control"
#define GERAN_EFS_TRRC_SPECIAL_TEST_SETTING_ENABLED "/nv/item_files/modem/tdscdma/rrc/special_test_setting_enabled"
#define GERAN_EFS_RR_ARFCN_LIST "/nv/item_files/modem/geran/rr_efs_arfcn_list"
#define GERAN_EFS_PSCAN_RESULTS_REUSE_TIME_SECS "/nv/item_files/modem/geran/pscan_results_reuse_time_secs"
#define GERAN_EFS_ADD_CRH_DB "/nv/item_files/modem/geran/add_crh_db"
#define GERAN_EFS_ADD_CRH_SCELL_THRESH_DB "/nv/item_files/modem/geran/add_crh_scell_thresh_db"
#define GERAN_EFS_SYS_INFO_CACHE_ENABLED "/nv/item_files/modem/geran/grr/sys_info_cache_enabled"
#define GERAN_EFS_BAND_AVOID_ENABLED "/nv/item_files/modem/geran/band_avoid_enabled"
#define GERAN_EFS_BAND_AVOID_NCELL_THRESHOLD "/nv/item_files/modem/geran/band_avoid_ncell_threshold"
#define GERAN_EFS_BAND_AVOID_MAX_MEAS_RPTS "/nv/item_files/modem/geran/band_avoid_max_meas_rpts"
#define GERAN_EFS_ITERATIVE_SI_ACQ_ENABLED "/nv/item_files/modem/geran/grr/iterative_si_acq_enabled"
#define GERAN_EFS_SCELL_RESYNC_TIME_SECS "/nv/item_files/modem/geran/scell_resync_time_secs"
#define GERAN_EFS_LRRC_BPLMN_CONTROL "/nv/item_files/modem/lte/rrc/bplmn/bplmn_control"
#define GERAN_EFS_CSFB_FCCH_SNR_OFFSET "/nv/item_files/modem/geran/grr/csfb_fcch_snr_offset"
#define GERAN_EFS_CSFB_RSSI_OFFSET "/nv/item_files/modem/geran/grr/csfb_rssi_offset"
#define GERAN_EFS_CSFB_SNR_ENABLED "/nv/item_files/modem/geran/grr/csfb_snr_enabled"
#define GERAN_EFS_ENG_MODE_DEBUG_ENABLED  "/nv/item_files/modem/geran/eng_mode_debug_enabled"
#define GERAN_EFS_VAMOS_SUPPORT "/nv/item_files/modem/geran/vamos_support"
#define GERAN_EFS_LAST_CAMPED_CELL_ENABLED "/nv/item_files/modem/geran/grr/last_camped_cell_enabled"
#define GERAN_EFS_BCCH_DECODES_PER_BAND_ENABLED "/nv/item_files/modem/geran/grr/bcch_decodes_per_band_enabled"
/* QSH debug EFS item may be used by other G modules in future */
#define GERAN_EFS_QSH_DEBUG_ENABLED "/nv/item_files/modem/geran/qsh_debug_enabled"
#define GERAN_EFS_RR_FCELL_LIST "/nv/reg_files/modem/forbidden_cell_list"
#define GERAN_EFS_CSFB_DEPRIOTIZE_GSM_CELL  "/nv/item_files/modem/geran/grr/depriotize_gsm_cell"
#define GERAN_EFS_CS_CAMP_WHEN_NO_SI13_IN_CELL "/nv/item_files/modem/geran/cs_camping_when_no_si13"
#define GERAN_EFS_CAMP_WHEN_NO_SI2BIS_SI2TER   "/nv/item_files/modem/geran/camp_when_no_si2ter_si2bis"
#define GERAN_EFS_REVERT_TO_RANK_BASE "/nv/item_files/modem/geran/revert_to_rank_base"
#define GERAN_EFS_USE_BCCH_PRIO_WHEN_DED_PRIO_IS_VALID_AND_MISSING  "/nv/item_files/modem/geran/use_bcch_priority"
#define GERAN_EFS_SLOW_IRAT_MODE_ENABLED       "/nv/item_files/modem/geran/slow_irat_mode_enabled"

/*************************************/
/* Subscription-Specific EFS items used by GRR */
/************************************/
#define GERAN_EFS_FAST_SI_ACQ_DURING_CSFB_CONTROL "/nv/item_files/modem/geran/grr/fast_si_acq_during_csfb_control"
#define GERAN_EFS_FAST_SI_ACQ_WHEN_CS_ONLY_ENABLED "/nv/item_files/modem/geran/grr/fast_si_acq_when_cs_only_enabled"
#define GERAN_EFS_NEGATIVE_C1_WHITE_LIST "/nv/item_files/modem/geran/negative_c1_white_list"
#define GERAN_EFS_PLMN_SEARCH_FREQ_PRUNING_ENABLED "/nv/item_files/modem/geran/plmn_search_freq_pruning_enabled"
#define GERAN_EFS_G2L_BLIND_REDIR_CONTROL "/nv/item_files/modem/geran/grr/g2l_blind_redir_control"
#define GERAN_EFS_G2L_BLIND_REDIR_AFTER_CSFB_CONTROL "/nv/item_files/modem/geran/grr/g2l_blind_redir_after_csfb_control"
#define GERAN_EFS_G2L_BLIND_REDIR_AFTER_SRVCC_CONTROL "/nv/item_files/modem/geran/grr/g2l_blind_redir_after_srvcc_control"
#define GERAN_EFS_FAST_G2L_USING_PSEUDO_SI2Q_ENABLED "/nv/item_files/modem/geran/fast_g2l_using_pseudo_si2q_enabled"
#define GERAN_EFS_SGLTE_G2X_CONN_MODE_RESEL_ENABLED "/nv/item_files/modem/geran/sglte_g2x_conn_mode_resel_enabled"
#define GERAN_EFS_X2G_FAST_SI_ACQ_WHITE_LIST "/nv/item_files/modem/geran/x2g_fast_si_acq_white_list"
#define GERAN_EFS_READ_PCH_DURING_TRANSFER_WHITELIST "/nv/item_files/modem/geran/read_pch_during_transfer_whitelist"
#define GERAN_EFS_LAST_CAMPED_CELL  "/nv/item_files/modem/geran/grr/last_camped_cell"
#define GERAN_EFS_MULTISIM_SI_REFRESH_DURATION_SECS  "/nv/item_files/modem/geran/grr/multisim_si_refresh_duration_secs"
#define GERAN_EFS_HIGH_SPEED_TRAIN_OPT_WHITE_LIST "/nv/item_files/modem/geran/grr/hst_opt_white_list"
#define GERAN_EFS_MULTISIM_CBCH_SUPPORTED_MODES      "/nv/item_files/modem/geran/grr/cbch_supported_modes"
#define GERAN_EFS_ADD_LAST_SERV_CELL_CSFB_WHITELIST     "/nv/item_files/modem/geran/add_last_serv_cell_csfb_whitelist"
#define GERAN_EFS_FAKE_BS_DETECTION_ENABLED "/nv/item_files/modem/geran/grr/fake_bs_detection_enabled"
#define GERAN_EFS_FAKE_BTS_CELL_BARRING "/nv/item_files/modem/geran/grr/fake_bts_cell_barring_enabled"
#define GERAN_EFS_OVERRIDE_SCELL_SI2Q_ENABLED "/nv/item_files/modem/geran/grr/fallback_override_scell_si2q_enabled"
#define GERAN_EFS_COLL_FALLBACK_ENABLED   "/nv/item_files/modem/geran/grr/coll_fallback_enabled"
#define GERAN_EFS_DECODE_ALL_BCCH_FREQS         "/nv/item_files/modem/geran/grr/decode_all_bcch_freqs"
#define GERAN_EFS_PRIORITIZE_LTE_MEAS_ENABLED "/nv/item_files/modem/geran/prioritize_lte_meas_enabled"
#define GERAN_EFS_PRIORITIZE_LTE_MEAS_INFO "/nv/item_files/modem/geran/prioritize_lte_meas_info"
#define GERAN_EFS_INCREASE_LTE_CELL_PRIORITY "/nv/item_files/modem/geran/increase_lte_cell_priority"

/*************************************/
/* GERAN COMMON EFS items */
/************************************/
#define GERAN_EFS_DATA_PDU_DEBUG_MASK "/nv/item_files/modem/geran/data_pdu_debug_mask"
#define GERAN_EFS_ALLOW_TA_DL_UNACK   "/nv/item_files/modem/geran/allow_ta_dl_unack"
#define GERAN_EFS_PTM_ERR_RECOVERY    "/nv/item_files/modem/geran/ptm_err_recovery"
#define GERAN_EFS_DSC_THRESHOLD_FOR_SLOW_IRAT "/nv/item_files/modem/geran/dsc_threshold_for_slow_irat"

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/

/*!
 * \brief Wrapper API for mcfg_fs_read for reading EFS Items for primary subscription
 * - consistent with legacy efs_get API usage 
 * 
 * \return length of data read if successful else -1
 */
extern int geran_efs_read_primary(const char *path, void *data, uint32 length);

/*!
 * \brief Wrapper API for mcfg_fs_read for reading EFS Items per subscription
 * 
 * \return length of data read if successful else -1
 */
extern int geran_efs_read_per_sub(const char *path, void *data, uint32 length, const sys_modem_as_id_e_type as_id);

/*!
 * \brief Wrapper API for mcfg_fs_stat for getting the EFS item stats per subscription
 * 
 * \return boolean - TRUE if successful, FALSE otherwise
 */
extern boolean geran_efs_stat_per_sub(const char *path, struct fs_stat *data, const sys_modem_as_id_e_type as_id);

/*!
 * \brief Read general GERAN EFS-NV items.
 */
extern void geran_read_efs_nv_items(void);

#ifdef FEATURE_DUAL_SIM
/*!
 * \brief Returns the value of EFS-NV item default_rf_chain.
 * 
 * \return int - {0,1} or -1 if not defined
 */
extern int geran_nv_get_default_rf_chain(void);
#endif /* FEATURE_DUAL_SIM */

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
/*!
 * \brief Returns the GPRS->WCDMA tuneaway support based on the values of EFS-NV items.
 * 
 * \return int - {GERAN_TA_SUPPORTED, GERAN_TA_NOT_SUPPORTED, GERAN_NV_MEM_NOT_VALID} 
 */
extern int geran_nv_g2w_ta_supported(void);

/*!
 * \brief Returns the GPRS->TDSCDMA tuneaway support based on the values of EFS-NV items.
 * 
 * \return int - {GERAN_TA_SUPPORTED, GERAN_TA_NOT_SUPPORTED, GERAN_NV_MEM_NOT_VALID} 
 */
extern int geran_nv_g2t_ta_supported(void);

/*!
 * \brief Returns the GPRS->CDMA tuneaway support based on the values of EFS-NV items.
 * 
 * \return int - {GERAN_TA_SUPPORTED, GERAN_TA_NOT_SUPPORTED, GERAN_NV_MEM_NOT_VALID} 
 */
extern int geran_nv_g21x_ta_supported(void);

/*!
 * \brief Returns the GPRS->LTE tuneaway support based on the values of EFS-NV items.
 * 
 * \return int - {GERAN_TA_SUPPORTED, GERAN_TA_NOT_SUPPORTED, GERAN_NV_MEM_NOT_VALID} 
 */
extern int geran_nv_g2l_ta_supported(void);

#if defined(FEATURE_GTA_DURING_PS_RACH) || defined(FEATURE_G2X_IDLE_TUNEAWAY)
/*!
 * \brief Returns the G->X tech tuneaway support based on the values of EFS-NV items.
 * 
 * \return int - {GERAN_TA_SUPPORTED, GERAN_TA_NOT_SUPPORTED, GERAN_NV_MEM_NOT_VALID} 
 */
extern int geran_nv_g2x_ta_supported(grm_client_enum_t client);

#endif /* FEATURE_GTA_DURING_PS_RACH */

extern int16 gsm_dr_to_sr_falback_rssi_thresh_dBmx16 (gas_id_t gas_id);
extern uint8 geran_nv_get_dsc_threshold_for_slow_irat();

#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)
extern boolean geran_nv_qsh_debug_enabled(void);
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP */

/*!
 * \brief Module initialisation function. Called during RR task initialisation.
 */
extern void geran_nv_init(void);

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*!
 * \brief function for informing MCFG about refresh done. Called from RR task.
 */
extern void geran_nv_refresh_done(const boolean done, const gas_id_t gas_id);
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

#ifdef FEATURE_DATA_PDU_DEBUG
/*!
 * \brief Indicates if data PDU debug print is enabled for a given mask.
 *        
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
extern boolean geran_nv_data_pdu_debug_enabled(const uint8 mask);
#endif /* FEATURE_DATA_PDU_DEBUG */

extern boolean grlc_nv_allow_ta_dl_unack(void);

extern boolean geran_ciphering_logging_enabled(gas_id_t gas_id);

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
extern boolean geran_nv_ptm_err_recovery(void);
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */

#endif /* GERAN_NV_H */

/* EOF */

