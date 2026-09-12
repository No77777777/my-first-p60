/*! \file rr_nv.c 
 
  This module contains access functions for NV data.
 
                Copyright (c) 2011-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_nv.c#6 $ */
/* $DateTime: 2021/02/07 22:55:02 $$Author: pwbldsvc $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "rr_seg_load.h"
#include "geran_variation.h"
#include "rr_nv.h"
#include "rr_nv_g.h"
#include "rr_channel_mode.h"
#include "rr_gprs_defs.h"
#include "fs_public.h"
#include "rr_msc.h"
#include "geran_multi_sim.h"
#include "sys.h"
#include "geran_nv.h"
#include "gprs_mem.h" 
#include "rr_sim.h"
#ifdef FEATURE_GSM_TDS
#include "ghdi_exp.h"
#include "ghdi_exp_v.h"
#endif /*FEATURE_GSM_TDS*/
#ifdef FEATURE_MODEM_CONFIG_REFRESH
#include "rr_if.h"
#endif /* FEATURE_MODEM_CONFIG_REFRESH */
#include "rr_acq_db.h"
#include "mcfg_nv_api.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define RR_SCELL_RESYNC_TIME_DEFAULT (30) /* 30 seconds */
#define RR_PSCAN_STALENESS_DEFAULT   (4)  /*  4 seconds */

#ifdef FEATURE_FAST_G2L_PSEUDO_SI2Q
/*----------------------------------------------------------------------------
 * Default GERAN, LTE and TDS Parameters
 * -------------------------------------------------------------------------*/
 #define RR_DEFAULT_GERAN_PRIO 1
 #define RR_GERAN_DEF_H_PRIO   0  /* This implies 06 DB */
 #define RR_GERAN_DEF_THRESH_GSM_LOW 15 /* This implies GRR will always measure low Priority IRAT neighbors */
 #define RR_GERAN_DEF_THRESH_PRIO_SRCH 15
 #define RR_GERAN_DEF_T_RESEL 0  /* Zero implies 05 seconds */
 
 #define RR_LTE_DEF_PRIO      7
 #define RR_LTE_DEF_MEAS_BW   0
 #define RR_LTE_DEF_QRXLEV_MIN 0
 #define RR_LTE_DEF_THRESH_HIGH  3
 #define RR_LTE_DEF_THRESH_LOW  0

 #define RR_UTRAN_DEF_PRIO   3
 #define RR_UTRAN_DEF_QRXLEV_MIN 0
 #define RR_UTRAN_DEF_THRESH_HIGH 0
 #define RR_UTRAN_DEF_THRESH_LOW  0

#else
 
 #define RR_LTE_DEF_PRIO      7
 #define RR_LTE_DEF_MEAS_BW   0
 #define RR_LTE_DEF_QRXLEV_MIN 0
 #define RR_LTE_DEF_THRESH_HIGH  3
 #define RR_LTE_DEF_THRESH_LOW  0
 
#endif /* FEATURE_FAST_G2L_PSEUDO_SI2Q */

#define RR_GERAN_H_PRIO 3
#define RR_GERAN_DEF_T_RESEL 0 // 5 seconds
#define RR_GERAN_THRESH_LOW 1   // 2 d b 
#define RR_GERAN_DEF_THRESH_PRIORITY_SRCH 1

#define RR_UTRAN_QRXLEV_MIN 7 //  7 (-105dBm)
#define RR_UTRAN_THRESH_LOW 3    //6 d b
#define RR_UTRAN_THRESH_HIGH 0

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

typedef struct
{
  uint8    split_page_cycle_enc;
  uint16   split_page_cycle;
  uint8    non_drx_timer_enc;
  uint8    non_drx_timer;
  uint8    multislot_class;
  boolean  egprs_enabled;
  uint8    egprs_multislot_class;
  boolean  egprs_8psk_ul_enabled;
  uint8    egprs_8psk_power_capability;
  boolean  geran_feature_pack_1_enabled;
  uint8    interrat_nacc_support;
#ifdef FEATURE_GSM_DTM
  boolean  dtm_enabled;
  uint8    dtm_egprs_multislot_subclass;
#ifdef FEATURE_GSM_EDTM
  boolean  edtm_enabled;
#endif /* FEATURE_GSM_EDTM */
#endif /* FEATURE_GSM_DTM */
  uint8    gea_supported;
#ifdef FEATURE_GSM_GPRS_MSC33
  uint8    hmsc_gprs_coded_ms_class;
  uint8    hmsc_egprs_coded_ms_class;
#endif /* FEATURE_GSM_GPRS_MSC33 */
} rr_nv_data_t;

// Maximum number of PLMNs in EFS white list
#define MAX_NUM_OF_PLMNS 20

// Number of bytes storage per PLMN id
#define BYTES_PER_PLMN 3

typedef struct
{
  uint8 num_of_plmns;
  sys_plmn_id_s_type *plmn_list_ptr;
} rr_plmn_white_list_t;

typedef struct
{
  boolean x2g_fast_si_acq_white_list_present;
  rr_plmn_white_list_t x2g_fast_si_acq_white_list;
} rr_efs_x2g_fast_si_acq_info_t;

#ifdef FEATURE_LOCK_ARFCN
typedef struct
{
  uint8 num_of_arfcn;
  uint16 *arfcn_num_list_ptr;
} rr_efs_arfcn_list_t;
#endif /*FEATURE_LOCK_ARFCN*/

typedef struct
{
  boolean valid;
  struct
  {
    ARFCN_T arfcn;
    BSIC_T bsic;
    LAI_T lai;
  } data;
} rr_efs_last_camped_cell_t;

typedef struct
{
  rr_efs_last_camped_cell_t efs;
  rr_efs_last_camped_cell_t runtime;
} rr_last_camped_cell_t;

#ifdef _WIN32
#pragma pack(push,1) // Save previous, and turn on 1 byte alignment
#endif

typedef PACK(struct)
{
  boolean g2w_enabled;
  boolean g2t_enabled;
  boolean g2l_enabled;
} rr_efs_g2x_measurement_control_t;

#ifdef _WIN32
#pragma pack(pop) // Revert alignment to what it was previously
#endif

typedef struct
{
  sys_sys_mode_e_type rat_type;
  uint32 ci;
  uint32 location_area_code;
  sys_plmn_id_s_type plmn_id;
    
} rr_efs_forbidden_cells_t;

typedef struct
{
  uint8 num_of_barred_cells;
  rr_efs_forbidden_cells_t *forbidden_cells_ptr;
} rr_efs_forbidden_cells_list_t;

typedef struct
{
#ifdef FEATURE_VAMOS
  boolean vamos_enabled;
#endif /* FEATURE_VAMOS */
  boolean fast_si_acq_during_csfb_enabled;
  boolean fast_si_acq_when_cs_only_enabled;
  rr_plmn_white_list_t negative_c1_white_list;
  uint8 pscan_results_reuse_time_secs;
  boolean plmn_search_freq_pruning_enabled;
  rr_efs_x2g_fast_si_acq_info_t x2g_fast_si_acq_info;
#ifdef FEATURE_SGLTE
  uint32  sglte_b39_coex_hysteresis_timeout;
#endif /* FEATURE_SGLTE */
#ifdef FEATURE_GSM_TDS
  boolean tds_lab_config_enabled;
#endif /*FEATURE_GSM_TDS*/
#ifdef FEATURE_SGLTE_G2X_CONN_MODE_RESEL
  boolean sglte_g2x_conn_mode_resel_enabled;
#endif
#ifdef FEATURE_FAST_G2L_PSEUDO_SI2Q
  boolean fast_g2l_using_pseudo_si2q_enabled;
  rr_efs_pseudo_si2q_def_data_t rr_efs_pseudo_si2q_def_data;
#endif
  int8    add_crh_db;
  int8    add_crh_scell_thresh_db;
  boolean sys_info_cache_enabled;
#ifdef FEATURE_GSM_BAND_AVOIDANCE
  boolean band_avoid_enabled;
  uint8 band_avoid_ncell_threshold;
  uint8 band_avoid_max_meas_rpts;
#endif /*FEATURE_GSM_BAND_AVOIDANCE*/
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER 
  rr_plmn_white_list_t read_pch_during_transfer_whitelist;
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
  boolean iterative_si_acq_enabled;
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  uint8   scell_resync_time_secs;
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
#ifdef FEATURE_LOCK_ARFCN
 rr_efs_arfcn_list_t rr_efs_arfcn_list;
#endif /*FEATURE_LOCK_ARFCN*/
  boolean eng_mode_debug_enabled;
  boolean last_camped_cell_enabled;
  rr_last_camped_cell_t last_camped_cell;
  boolean bcch_decodes_per_band_enabled;
#ifdef FEATURE_LTE
  /* Indicates whether G2L blind redirection is enabled.  If enabled, after
  a CS call (for voice, supplementary services or periodic LAU) is released
  RR will initiate a blind redirection to LTE. */
  boolean g2l_blind_redir_after_voice_ss_or_periodic_lau_enabled;

  /* Indicates whether G2L blind redirection after CSFB is enabled.  If
  enabled, after a CS call resulting from a CSFB procedure is released, RR
  will initiate a blind redirection to LTE.  This is based on RR_EST_REQ
  indicating whether the call is being set up as a result of CSFB. */
  boolean g2l_blind_redir_after_csfb_enabled;
  boolean g2l_blind_redir_after_srvcc_enabled;
  uint16 csfb_fcch_snr_offset;
  uint16 csfb_rssi_offset;
  boolean csfb_snr_enabled;
#endif /* FEATURE_LTE */
  rr_efs_g2x_measurement_control_t g2x_measurement_control;
#ifdef FEATURE_LTE
  boolean depriotize_gsm_cell;
#endif /*FEATURE_LTE*/
#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
  boolean qsh_debug_enabled;
#endif /* FEATURE_QSH_EVENT_METRIC */
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  uint16 multisim_si_refresh_duration_secs;
  uint8  cbch_supported_modes;
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GERAN_HST_OPT
  rr_plmn_white_list_t hst_opt_white_list; /*High Speed Train Optimization*/
#endif /*FEATURE_GERAN_HST_OPT*/

  rr_plmn_white_list_t add_last_serv_cell_csfb_whitelist;
#ifdef FEATURE_FAKE_BTS_CELL_INFO
  boolean fake_bs_detection_enabled;
#endif /* FEATURE_FAKE_BTS_CELL_INFO */
  boolean camp_for_emergency_call_enabled;
#ifdef FEATURE_FAKE_BTS_CELL_BARRING
  boolean  fake_bts_cell_barring_enabled; 
#endif /* FEATURE_FAKE_BTS_CELL_BARRING */
  boolean cs_camping_when_no_si13;
  boolean camp_when_no_si2ter_si2bis;
  rr_plmn_white_list_t  revert_to_rank_base;
  boolean use_bcch_priority;
  boolean coll_fallback_enabled;
  boolean fallback_override_scell_si2q_enabled;
  rr_efs_pseudo_si2q_def_data_t rr_efs_override_scell_si2q_data;
  boolean slow_irat_mode_enabled;
  boolean rr_efs_increase_lte_cell_priority_enabled;
  rr_efs_increase_lte_cell_priority_data_t rr_efs_increase_lte_cell_priority_data;
#ifdef FEATURE_PRIORITIZE_LTE_FREQ
  boolean prioritize_lte_meas_enabled;
  rr_efs_prioritize_lte_meas_info_t prioritize_lte_meas_info;
#endif /* FEATURE_PRIORITIZE_LTE_FREQ */
#ifdef FEATURE_GSM_DECODE_ALL_FREQS
  boolean decode_all_bcch_freqs;
#endif //FEATURE_GSM_DECODE_ALL_FREQS
} rr_efs_nv_data_t;

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

// NV data (Note: per sub)
static rr_nv_data_t rr_nv_data[NUM_GERAN_DATA_SPACES];

// EFS-NV data (Note: per sub)
static rr_efs_nv_data_t rr_efs_nv_data[NUM_GERAN_DATA_SPACES];

// EFS-NV data (Note: common for both subs)
static rr_efs_geran_forbidden_cells_list_t rr_efs_geran_forbidden_cells;

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/

/*!
 * \brief Returns the data-space index appropriate for the given AS-ID.
 *
 *  Note: For the NV data, this index is stored according to AS-ID, not GAS-ID. This is to allow the
 *  AS-ID to GAS-ID mapping to be changed without the NV data needing to be re-sent
 *
 * \param as_id
 *
 * \return uint8
 */
static uint8 rr_nv_get_data_space_index(const sys_modem_as_id_e_type as_id)
{
#ifdef FEATURE_TRIPLE_SIM
  if (as_id == SYS_MODEM_AS_ID_3)
  {
    return GERAN_DATA_SPACE_INDEX_3;
  }
#endif // FEATURE_TRIPLE_SIM

#ifdef FEATURE_DUAL_SIM
  if (as_id == SYS_MODEM_AS_ID_2)
  {
    return GERAN_DATA_SPACE_INDEX_2;
  }
#endif // FEATURE_DUAL_SIM

  return GERAN_DATA_SPACE_INDEX_1;
}

/*!
 * \brief Returns a pointer to the module data.
 *
 * \param as_id
 *
 * \return rr_nv_data_t*
 */
static rr_nv_data_t *rr_nv_get_data_ptr(const sys_modem_as_id_e_type as_id)
{
  uint8 as_index = rr_nv_get_data_space_index(as_id);

  return &rr_nv_data[as_index];
}

/*!
 * \brief Returns a pointer to the EFS data.
 *
 * \param as_id
 *
 * \return rr_nv_data_t*
 */
static rr_efs_nv_data_t *rr_nv_get_efs_data_ptr(const sys_modem_as_id_e_type as_id)
{
  uint8 as_index = rr_nv_get_data_space_index(as_id);

  return &rr_efs_nv_data[as_index];
}

/*!
 * \brief Returns a pointer to the EFS data.
 *
 * \param as_id
 *
 * \return rr_efs_geran_forbidden_cells_list_t*
 */
rr_efs_geran_forbidden_cells_list_t *rr_nv_get_f_cells_list_ptr(void)
{
  return &rr_efs_geran_forbidden_cells;
}

/*!
 * \brief Takes the encoded split page cycle code and returns the decoded value
 * 
 * \param split_page_cycle_enc (in)
 * 
 * \return uint16 
 */
static uint16 decode_split_page_cycle(uint8 split_page_cycle_enc)
{
/* Translation of SPLIT PAGE CYCLE (encoded) to SPLIT PAGE CYCLE value for values 65 -> 98 */
  static const uint16 spc_code_to_val[] =
  {
    71, 72, 74, 75, 77, 79, 80, 83, 86, 88, /* indices 0..9   */
    90, 92, 96,101,103,107,112,116,118,128, /* indices 10..19 */
   141,144,150,160,171,176,192,214,224,235, /* indices 20..29 */
   256, 288, 320, 352                       /* indices 30..33 */
  };

  uint16 split_page_cycle;

/*
 SPLIT PAGE CYCLE values - see GSM 04.08 10.5.5.6

 SPLIT PAGE CYCLE CODE   SPLIT PAGE CYCLE value
 0                       704
 1 -> 64                 1 -> 64
 65 -> 98                as constant array above
 all others              1
*/

  if ( split_page_cycle_enc == 0 )
  {
    split_page_cycle = 704;
  }
  else
  if ( ( split_page_cycle_enc >= 1 ) && ( split_page_cycle_enc <= 64 ) )
  {
    split_page_cycle = split_page_cycle_enc;
  }
  else
  if ( ( split_page_cycle_enc >= 65 ) && ( split_page_cycle_enc <= 98 ) )
  {
    split_page_cycle = spc_code_to_val[split_page_cycle_enc - 65];
  }
  else
  {
    split_page_cycle = 1;
  }

  return split_page_cycle;

} /* end decode_split_page_cycle() */

/*!
 * \brief Prints the last-camped cell data (EFS copy)
 * 
 * \param gas_id (in)
 */
static void print_efs_last_camped_cell(const sys_modem_as_id_e_type as_id)
{
  // Obtain a pointer to the EFS data
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  MSG_GERAN_HIGH_0("/nv/item_files/modem/geran/grr/last_camped_cell:");

  MSG_GERAN_HIGH_3("  valid=%d arfcn.num=%d arfcn.band=%d",
                   efs_data_ptr->last_camped_cell.efs.valid,
                   efs_data_ptr->last_camped_cell.efs.data.arfcn.num,
                   efs_data_ptr->last_camped_cell.efs.data.arfcn.band);
}

/*!
 * \brief Resets the last-camped cell data (run-time copy)
 * 
 * \param as_id (in)
 */
static void reset_runtime_last_camped_cell(const sys_modem_as_id_e_type as_id)
{
  // Obtain a pointer to the EFS data
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  RR_NULL_CHECK_FATAL(efs_data_ptr);

  memset(&(efs_data_ptr->last_camped_cell.runtime),
         0,
         sizeof(efs_data_ptr->last_camped_cell.runtime));
}

/*!
 * \brief Resets the last-camped cell data (EFS copy)
 * 
 * \param as_id (in)
 */
static void reset_efs_last_camped_cell(const sys_modem_as_id_e_type as_id)
{
  // Obtain a pointer to the EFS data
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  RR_NULL_CHECK_FATAL(efs_data_ptr);

  memset(&(efs_data_ptr->last_camped_cell.efs),
         0,
         sizeof(efs_data_ptr->last_camped_cell.efs));
}

/*!
 * \brief Reads carrier-specific EFS items and populates the local EFS data store.
 */
static void rr_read_carrier_specific_efs_nv_items(const sys_modem_as_id_e_type as_id)
{
  // Obtain a pointer to the EFS data
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

#ifdef FEATURE_VAMOS
  // Read /nv/item_files/modem/geran/vamos_support
  {
    uint8 vamos_support;

    if (geran_efs_read_per_sub(GERAN_EFS_VAMOS_SUPPORT,
                               &vamos_support,
                               sizeof(vamos_support),
                               as_id) < 0)
    {
      MSG_GERAN_HIGH_0("VAMOS-I enabled (default)");
      vamos_support = 1;
    }

    // vamos_support: 0=OFF 1=VAMOS-I 2=VAMOS-II
    if (vamos_support != 0)
    {
      efs_data_ptr->vamos_enabled = TRUE;
    }
    else
    {
      efs_data_ptr->vamos_enabled = FALSE;
    }
  }
#endif /* FEATURE_VAMOS */

#ifdef FEATURE_LTE
  // Read /nv/item_files/modem/geran/grr/g2l_blind_redir_control
  {
    int8 efs_val;

    if (geran_efs_read_per_sub(
          GERAN_EFS_G2L_BLIND_REDIR_CONTROL,
          &efs_val,
          sizeof(efs_val),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("g2l_blind_redir_control not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("g2l_blind_redir_after_voice_ss_or_periodic_lau_enabled is ENABLED");
      efs_data_ptr->g2l_blind_redir_after_voice_ss_or_periodic_lau_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("g2l_blind_redir_after_voice_ss_or_periodic_lau_enabled is DISABLED");
      efs_data_ptr->g2l_blind_redir_after_voice_ss_or_periodic_lau_enabled = FALSE;
    }
  }

  // Read /nv/item_files/modem/geran/grr/g2l_blind_redir_after_csfb_control
  {
    int8 efs_val;

    if (geran_efs_read_per_sub(
          GERAN_EFS_G2L_BLIND_REDIR_AFTER_CSFB_CONTROL,
          &efs_val,
          sizeof(efs_val),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("g2l_blind_redir_after_csfb_control not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("g2l_blind_redir_after_csfb_enabled is ENABLED");
      efs_data_ptr->g2l_blind_redir_after_csfb_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("g2l_blind_redir_after_csfb_enabled is DISABLED");
      efs_data_ptr->g2l_blind_redir_after_csfb_enabled = FALSE;
    }
  }

  // Read /nv/item_files/modem/geran/grr/g2l_blind_redir_after_srvcc_control
  {
    int8 efs_val;

    if (geran_efs_read_per_sub(
          GERAN_EFS_G2L_BLIND_REDIR_AFTER_SRVCC_CONTROL,
          &efs_val,
          sizeof(efs_val),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("g2l_blind_redir_after_srvcc_control not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("g2l_blind_redir_after_srvcc_enabled is ENABLED");
      efs_data_ptr->g2l_blind_redir_after_srvcc_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("g2l_blind_redir_after_srvcc_enabled is DISABLED");
      efs_data_ptr->g2l_blind_redir_after_srvcc_enabled = FALSE;
    }
  }
#endif /* FEATURE_LTE */

  // Read /nv/item_files/modem/geran/grr/fast_si_acq_during_csfb_control
  {
    int8 efs_val;

    if (geran_efs_read_per_sub(
          GERAN_EFS_FAST_SI_ACQ_DURING_CSFB_CONTROL,
          &efs_val,
          sizeof(efs_val),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("fast_si_acq_during_csfb_control not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("fast_si_acq_during_csfb_enabled is ENABLED");
      efs_data_ptr->fast_si_acq_during_csfb_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("fast_si_acq_during_csfb_enabled is DISABLED");
      efs_data_ptr->fast_si_acq_during_csfb_enabled = FALSE;
    }
  }
#ifdef FEATURE_LOCK_ARFCN
  // Read /nv/item_files/modem/geran/rr_efs_arfcn_list
  {
    struct fs_stat file_status;

    efs_data_ptr->rr_efs_arfcn_list.num_of_arfcn = 0; 

    if (geran_efs_stat_per_sub(
          GERAN_EFS_RR_ARFCN_LIST,
          &file_status,
		  as_id
        ))
    {
       // EFS file size in bytes
       fs_size_t file_size_bytes = file_status.st_size;
       #define BYTES_PER_ARFCN  2
       #define MAX_NUM_OF_ARFCN 20
    
       // Number of ARFCNs that can be represented by this file size, capped at MAX_NUM_OF_ARFCN
       int num_of_arfcn = MIN((file_size_bytes / BYTES_PER_ARFCN), MAX_NUM_OF_ARFCN);
    
       // Number of bytes to allocate to store the PLMN list
       int alloc_size_bytes = num_of_arfcn * BYTES_PER_ARFCN;

       if (efs_data_ptr->rr_efs_arfcn_list.arfcn_num_list_ptr != NULL)
       {
         GPRS_MEM_FREE(efs_data_ptr->rr_efs_arfcn_list.arfcn_num_list_ptr);
         //Re-Initialize ptr to NULL.
         efs_data_ptr->rr_efs_arfcn_list.arfcn_num_list_ptr = NULL;
       }

       efs_data_ptr->rr_efs_arfcn_list.arfcn_num_list_ptr = GPRS_MEM_MALLOC(alloc_size_bytes);

       if (efs_data_ptr->rr_efs_arfcn_list.arfcn_num_list_ptr != NULL)
       {
          /* memset structure to avoid holding invalid values */
          memset(efs_data_ptr->rr_efs_arfcn_list.arfcn_num_list_ptr, 0xFF, alloc_size_bytes);

          if (geran_efs_read_per_sub(
                GERAN_EFS_RR_ARFCN_LIST,
                efs_data_ptr->rr_efs_arfcn_list.arfcn_num_list_ptr,
                alloc_size_bytes,
                as_id
              ) == alloc_size_bytes)
          {
            efs_data_ptr->rr_efs_arfcn_list.num_of_arfcn = num_of_arfcn;
          }
          else
          {
            GPRS_MEM_FREE(efs_data_ptr->rr_efs_arfcn_list.arfcn_num_list_ptr);
            //Re-Initialize ptr to NULL.
            efs_data_ptr->rr_efs_arfcn_list.arfcn_num_list_ptr = NULL;
          }
       }
       else
       {
         MSG_GERAN_ERROR_1("Failed to allocate %d bytes for rr_efs_arfcn_list",
                           alloc_size_bytes);
       }
    }
  }
  
#endif /*FEATURE_LOCK_ARFCN*/
  
  // Read /nv/item_files/modem/geran/x2g_fast_si_acq_white_list
  {
    struct fs_stat file_status;

    efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list_present = FALSE;

    if (geran_efs_stat_per_sub(
          GERAN_EFS_X2G_FAST_SI_ACQ_WHITE_LIST,
          &file_status,
          as_id))
    {
      // EFS file size in bytes
      fs_size_t file_size_bytes = file_status.st_size;

      // Number of PLMNs that can be represented by this file size, capped at MAX_NUM_OF_PLMNS
      int num_of_plmns = MIN((file_size_bytes / BYTES_PER_PLMN), MAX_NUM_OF_PLMNS);

      // Number of bytes to allocate to store the PLMN list
      int alloc_size_bytes = num_of_plmns * BYTES_PER_PLMN;

      if (efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.plmn_list_ptr != NULL)
      {
        GPRS_MEM_FREE(efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.plmn_list_ptr);
        //Re-Initialize ptr to NULL.
        efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.plmn_list_ptr = NULL;
      }

      efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.plmn_list_ptr = GPRS_MEM_MALLOC(alloc_size_bytes);

      if (efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.plmn_list_ptr != NULL)
      {
        /* memset structure to avoid holding invalid values */
        memset(efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.plmn_list_ptr, 0xFF, alloc_size_bytes);

        if (geran_efs_read_per_sub(
              GERAN_EFS_X2G_FAST_SI_ACQ_WHITE_LIST,
              efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.plmn_list_ptr,
              alloc_size_bytes,
              as_id
            ) == alloc_size_bytes)
        {
          efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.num_of_plmns = num_of_plmns;
          efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list_present = TRUE;
        }
        else
        {
          GPRS_MEM_FREE(efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.plmn_list_ptr);
          //Re-Initialize ptr to NULL.
          efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.plmn_list_ptr = NULL;
        }
      }
      else
      {
        MSG_GERAN_ERROR_1("Failed to allocate %d bytes for x2g_fast_si_acq_white_list", alloc_size_bytes);
      }
    }
  }

  // Read /nv/item_files/modem/geran/grr/fast_si_acq_when_cs_only_enabled
  {
    int8 efs_val;

    if (geran_efs_read_per_sub(
          GERAN_EFS_FAST_SI_ACQ_WHEN_CS_ONLY_ENABLED,
          &efs_val,
          sizeof(efs_val),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV fast_si_acq_when_cs_only_enabled not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("Fast SI Acq when CS-only is ENABLED");
      efs_data_ptr->fast_si_acq_when_cs_only_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("Fast SI Acq when CS-only is DISABLED");
      efs_data_ptr->fast_si_acq_when_cs_only_enabled = FALSE;
    }
  }

  // Read /nv/item_files/modem/geran/negative_c1_white_list
  {
    struct fs_stat file_status;

    efs_data_ptr->negative_c1_white_list.num_of_plmns = 0;

    if (geran_efs_stat_per_sub(
          GERAN_EFS_NEGATIVE_C1_WHITE_LIST,
          &file_status,
          as_id))
    {
      // EFS file size in bytes
      fs_size_t file_size_bytes = file_status.st_size;

      // Number of PLMNs that can be represented by this file size, capped at MAX_NUM_OF_PLMNS
      int num_of_plmns = MIN((file_size_bytes / BYTES_PER_PLMN), MAX_NUM_OF_PLMNS);

      // Number of bytes to allocate to store the PLMN list
      int alloc_size_bytes = num_of_plmns * BYTES_PER_PLMN;

      if (efs_data_ptr->negative_c1_white_list.plmn_list_ptr != NULL)
      {
        GPRS_MEM_FREE(efs_data_ptr->negative_c1_white_list.plmn_list_ptr);
        //Re-Initialize ptr to NULL.
        efs_data_ptr->negative_c1_white_list.plmn_list_ptr = NULL;
      }

      efs_data_ptr->negative_c1_white_list.plmn_list_ptr = GPRS_MEM_MALLOC(alloc_size_bytes);

      if (efs_data_ptr->negative_c1_white_list.plmn_list_ptr != NULL)
      {
        /* memset structure to avoid holding invalid values */
        memset(efs_data_ptr->negative_c1_white_list.plmn_list_ptr, 0xFF, alloc_size_bytes);

        if (geran_efs_read_per_sub(
              GERAN_EFS_NEGATIVE_C1_WHITE_LIST,
              efs_data_ptr->negative_c1_white_list.plmn_list_ptr,
              alloc_size_bytes,
              as_id
            ) == alloc_size_bytes)
        {
          efs_data_ptr->negative_c1_white_list.num_of_plmns = num_of_plmns;
        }
        else
        {
          GPRS_MEM_FREE(efs_data_ptr->negative_c1_white_list.plmn_list_ptr);
          //Re-Initialize ptr to NULL.
          efs_data_ptr->negative_c1_white_list.plmn_list_ptr = NULL;
        }
      }
      else
      {
        MSG_GERAN_ERROR_1("Failed to allocate %d bytes for negative C1 whitelist", alloc_size_bytes);
      }
    }
  }

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER 
  // Read /nv/item_files/modem/geran/read_pch_during_transfer_whitelist
  {
    struct fs_stat file_status;

    efs_data_ptr->read_pch_during_transfer_whitelist.num_of_plmns = 0;   

    if (geran_efs_stat_per_sub(
          GERAN_EFS_READ_PCH_DURING_TRANSFER_WHITELIST,
          &file_status,
          as_id))
    {
      // EFS file size in bytes
      fs_size_t file_size_bytes = file_status.st_size;

      // Number of PLMNs that can be represented by this file size, capped at MAX_NUM_OF_PLMNS
      int num_of_plmns = MIN((file_size_bytes / BYTES_PER_PLMN), MAX_NUM_OF_PLMNS);

      // Number of bytes to allocate to store the PLMN list
      int alloc_size_bytes = num_of_plmns * BYTES_PER_PLMN;

      if (efs_data_ptr->read_pch_during_transfer_whitelist.plmn_list_ptr != NULL)
      {
        GPRS_MEM_FREE(efs_data_ptr->read_pch_during_transfer_whitelist.plmn_list_ptr);
        //Re-Initialize ptr to NULL.
        efs_data_ptr->read_pch_during_transfer_whitelist.plmn_list_ptr = NULL;
      }

      efs_data_ptr->read_pch_during_transfer_whitelist.plmn_list_ptr = GPRS_MEM_MALLOC(alloc_size_bytes);

      if (efs_data_ptr->read_pch_during_transfer_whitelist.plmn_list_ptr != NULL)
      {
        /* memset structure to avoid holding invalid values */
        memset(efs_data_ptr->read_pch_during_transfer_whitelist.plmn_list_ptr, 0xFF, alloc_size_bytes);

        if (geran_efs_read_per_sub(
              GERAN_EFS_READ_PCH_DURING_TRANSFER_WHITELIST,
              efs_data_ptr->read_pch_during_transfer_whitelist.plmn_list_ptr,
              alloc_size_bytes,
              as_id
            ) == alloc_size_bytes)
        {
          efs_data_ptr->read_pch_during_transfer_whitelist.num_of_plmns = num_of_plmns;
        }
        else
        {
          GPRS_MEM_FREE(efs_data_ptr->read_pch_during_transfer_whitelist.plmn_list_ptr);
          //Re-Initialize ptr to NULL.
          efs_data_ptr->read_pch_during_transfer_whitelist.plmn_list_ptr = NULL;
        }
      }
      else
      {
        MSG_GERAN_ERROR_1("Failed to allocate %d bytes for read_pch_during_transfer_whitelist", alloc_size_bytes);
      }
    }
  }
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

  // Read /nv/item_files/modem/geran/plmn_search_freq_pruning_enabled
  {
    int8 efs_val;

    if (geran_efs_read_per_sub(
          GERAN_EFS_PLMN_SEARCH_FREQ_PRUNING_ENABLED,
          &efs_val,
          sizeof(efs_val),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV plmn_search_freq_pruning_enabled not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("PLMN search freq pruning is ENABLED");
      efs_data_ptr->plmn_search_freq_pruning_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("PLMN search freq pruning is DISABLED");
      efs_data_ptr->plmn_search_freq_pruning_enabled = FALSE;
    }
  }

#ifdef FEATURE_FAST_G2L_PSEUDO_SI2Q
  // Read /nv/item_files/modem/geran/fast_g2l_using_pseudo_si2q_enabled
  {
    int8 efs_val;

    if (geran_efs_read_per_sub(
          GERAN_EFS_FAST_G2L_USING_PSEUDO_SI2Q_ENABLED,
          &efs_val, 
          sizeof(efs_val),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("fast_g2l_using_pseudo_si2q_enabled not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("fast_g2l_using_pseudo_si2q_enabled is ENABLED");
      efs_data_ptr->fast_g2l_using_pseudo_si2q_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("fast_g2l_using_pseudo_si2q_enabled is DISABLED");
      efs_data_ptr->fast_g2l_using_pseudo_si2q_enabled = FALSE;
    }
  }

  if (efs_data_ptr->fast_g2l_using_pseudo_si2q_enabled)
  {
    MSG_GERAN_HIGH_0("FEATURE_FAST_G2L_PSEUDO_SI2Q IS ENABLED, in NV ");

    /* Set the Default Value */
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.pseudo_si2q_def_data_valid = TRUE;
  
    /* Set Default Params Here */
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_geran_def_data.geran_priority = RR_DEFAULT_GERAN_PRIO; 
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_geran_def_data.h_prio = RR_GERAN_DEF_H_PRIO;
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_geran_def_data.thresh_gsm_low = RR_GERAN_DEF_THRESH_GSM_LOW;
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_geran_def_data.thresh_priority_search = RR_GERAN_DEF_THRESH_PRIO_SRCH;
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_geran_def_data.t_reselection = RR_GERAN_DEF_T_RESEL;

    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_lte_def_data.priority = RR_LTE_DEF_PRIO;
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_lte_def_data.meas_bandwidth = RR_LTE_DEF_MEAS_BW;
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_lte_def_data.qrxlevmin = RR_LTE_DEF_QRXLEV_MIN;
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_lte_def_data.thresh_high = RR_LTE_DEF_THRESH_HIGH;
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_lte_def_data.thresh_low = RR_LTE_DEF_THRESH_LOW;
  
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_utran_def_data.priority = RR_UTRAN_DEF_PRIO;
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_utran_def_data.qrxlevmin = RR_UTRAN_DEF_QRXLEV_MIN;
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_utran_def_data.thresh_high = RR_UTRAN_DEF_THRESH_HIGH;
    efs_data_ptr->rr_efs_pseudo_si2q_def_data.rr_utran_def_data.thresh_low = RR_UTRAN_DEF_THRESH_LOW;
  }
#endif /* FEATURE_FAST_G2L_PSEUDO_SI2Q */

#ifdef FEATURE_SGLTE_G2X_CONN_MODE_RESEL
  // Read /nv/item_files/modem/geran/sglte_g2x_conn_mode_resel_enabled
  {
    int8 efs_val;

    if (geran_efs_read_per_sub(
          GERAN_EFS_SGLTE_G2X_CONN_MODE_RESEL_ENABLED,
          &efs_val, 
          sizeof(efs_val),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("sglte_g2x_conn_mode_resel_enabled not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("sglte_g2x_conn_mode_resel_enabled is ENABLED");
      efs_data_ptr->sglte_g2x_conn_mode_resel_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("sglte_g2x_conn_mode_resel_enabled is DISABLED");
      efs_data_ptr->sglte_g2x_conn_mode_resel_enabled = FALSE;
    }
  }
#endif /* FEATURE_SGLTE_G2X_CONN_MODE_RESEL */

  // Read /nv/item_files/modem/geran/grr/last_camped_cell
  {
    reset_runtime_last_camped_cell(as_id);
    reset_efs_last_camped_cell(as_id);

    // Only read the last_camped_cell file if the feature is enabled. 
    // Note: Ensure device-specific EFS items are read first for this check to be valid. 
    if (efs_data_ptr->last_camped_cell_enabled)
    {
      int result = geran_efs_read_per_sub(
        GERAN_EFS_LAST_CAMPED_CELL,
        &efs_data_ptr->last_camped_cell.efs.data,
        sizeof(efs_data_ptr->last_camped_cell.efs.data),
        as_id
      );

      if (result > 0)
      {
        efs_data_ptr->last_camped_cell.efs.valid = TRUE;

        print_efs_last_camped_cell(as_id);

        efs_data_ptr->last_camped_cell.runtime = efs_data_ptr->last_camped_cell.efs;
      }
    }
  }

#ifdef FEATURE_LTE
  // Read /nv/item_files/modem/geran/grr/depriotize_gsm_cell
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_CSFB_DEPRIOTIZE_GSM_CELL,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
     efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }

   // Update the module store
   if (efs_val == 1)
   {
     MSG_GERAN_HIGH_0("De-Prioritizing GSM cell  is ENABLED");
     efs_data_ptr->depriotize_gsm_cell = TRUE;
   }
   else
   {
     MSG_GERAN_HIGH_0("De-Prioritizing GSM cell is DISABLED");
     efs_data_ptr->depriotize_gsm_cell = FALSE;
   }
 }

#endif /*FEATURE_LTE*/

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  /* Read /nv/item_files/modem/geran/grr/multisim_si_refresh_duration_secs */
  /* A duration of 0ms disables the timer */  
  {
    if (geran_efs_read_per_sub(
          GERAN_EFS_MULTISIM_SI_REFRESH_DURATION_SECS,
          &efs_data_ptr->multisim_si_refresh_duration_secs,
          sizeof(efs_data_ptr->multisim_si_refresh_duration_secs),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV multisim_si_refresh_duration not present, use default 300 seconds");
      efs_data_ptr->multisim_si_refresh_duration_secs = 300; /*5 minutes*/
    }
    else
    {
      MSG_GERAN_HIGH_1("EFS-NV multisim_si_refresh_duration is %ds", efs_data_ptr->multisim_si_refresh_duration_secs);
    }
  }

  /* Read /nv/item_files/modem/geran/grr/cbch_supported_modes */
  {
    if (geran_efs_read_per_sub(
          GERAN_EFS_MULTISIM_CBCH_SUPPORTED_MODES,
          &efs_data_ptr->cbch_supported_modes,
          sizeof(efs_data_ptr->cbch_supported_modes),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_1("EFS-NV cbch_supported_modes not present, use default 0x%02X", DSDS_CBCH_INVERSION_G_PLUS_G);
      efs_data_ptr->cbch_supported_modes = DSDS_CBCH_INVERSION_G_PLUS_G; /* CBCH inversion in G+G mode only */
    }
    else
    {
      MSG_GERAN_HIGH_1("EFS-NV cbch_supported_modes is 0x%02X", efs_data_ptr->cbch_supported_modes);
    }
  }
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GERAN_HST_OPT
  // Read /nv/item_files/modem/geran/hst_opt_white_list
  {
    struct fs_stat file_status;

    efs_data_ptr->hst_opt_white_list.num_of_plmns = 0;

    if (geran_efs_stat_per_sub(
          GERAN_EFS_HIGH_SPEED_TRAIN_OPT_WHITE_LIST,
          &file_status,
          as_id))
    {
      // EFS file size in bytes
      fs_size_t file_size_bytes = file_status.st_size;

      // Number of PLMNs that can be represented by this file size, capped at MAX_NUM_OF_PLMNS
      int num_of_plmns = MIN((file_size_bytes / BYTES_PER_PLMN), MAX_NUM_OF_PLMNS);

      // Number of bytes to allocate to store the PLMN list
      int alloc_size_bytes = num_of_plmns * BYTES_PER_PLMN;

      if (efs_data_ptr->hst_opt_white_list.plmn_list_ptr != NULL)
      {
        GPRS_MEM_FREE(efs_data_ptr->hst_opt_white_list.plmn_list_ptr);
        //Re-Initialize ptr to NULL.
        efs_data_ptr->hst_opt_white_list.plmn_list_ptr = NULL;
      }

      efs_data_ptr->hst_opt_white_list.plmn_list_ptr = GPRS_MEM_MALLOC(alloc_size_bytes);

      if (efs_data_ptr->hst_opt_white_list.plmn_list_ptr != NULL)
      {
        /* memset structure to avoid holding invalid values */
        memset(efs_data_ptr->hst_opt_white_list.plmn_list_ptr, 0xFF, alloc_size_bytes);

        if (geran_efs_read_per_sub(
              GERAN_EFS_HIGH_SPEED_TRAIN_OPT_WHITE_LIST,
              efs_data_ptr->hst_opt_white_list.plmn_list_ptr,
              alloc_size_bytes,
              as_id
            ) == alloc_size_bytes)
        {
          efs_data_ptr->hst_opt_white_list.num_of_plmns = num_of_plmns;
        }
        else
        {
          GPRS_MEM_FREE(efs_data_ptr->hst_opt_white_list.plmn_list_ptr);
          //Re-Initialize ptr to NULL.
          efs_data_ptr->hst_opt_white_list.plmn_list_ptr = NULL;
        }
      }
      else
      {
        MSG_GERAN_ERROR_1("Failed to allocate %d bytes for HST opt whitelist", alloc_size_bytes);
      }
    }
  }
#endif /*FEATURE_GERAN_HST_OPT*/

#ifdef FEATURE_GERAN_ADD_LAST_SCELL_CSFB_OPT
  // Read /nv/item_files/modem/geran/add_last_serv_cell_csfb_whitelist
  {
     struct fs_stat file_status;
  
     efs_data_ptr->add_last_serv_cell_csfb_whitelist.num_of_plmns = 0;   
  
     if (geran_efs_stat_per_sub(
            GERAN_EFS_ADD_LAST_SERV_CELL_CSFB_WHITELIST,
            &file_status,
            as_id))
     {
       // EFS file size in bytes
       fs_size_t file_size_bytes = file_status.st_size;
  
       // Number of PLMNs that can be represented by this file size, capped at MAX_NUM_OF_PLMNS
       int num_of_plmns = MIN((file_size_bytes / BYTES_PER_PLMN), MAX_NUM_OF_PLMNS);
  
       // Number of bytes to allocate to store the PLMN list
       int alloc_size_bytes = num_of_plmns * BYTES_PER_PLMN;

       if (efs_data_ptr->add_last_serv_cell_csfb_whitelist.plmn_list_ptr != NULL)
       {
         GPRS_MEM_FREE(efs_data_ptr->add_last_serv_cell_csfb_whitelist.plmn_list_ptr );
         //Re-Initialize ptr to NULL.
         efs_data_ptr->add_last_serv_cell_csfb_whitelist.plmn_list_ptr = NULL;
       }

       efs_data_ptr->add_last_serv_cell_csfb_whitelist.plmn_list_ptr = GPRS_MEM_MALLOC(alloc_size_bytes);
  
       if (efs_data_ptr->add_last_serv_cell_csfb_whitelist.plmn_list_ptr != NULL)
       {
         /* memset structure to avoid holding invalid values */
         memset(efs_data_ptr->add_last_serv_cell_csfb_whitelist.plmn_list_ptr, 0xFF, alloc_size_bytes);

         if (geran_efs_read_per_sub(
              GERAN_EFS_ADD_LAST_SERV_CELL_CSFB_WHITELIST,
              efs_data_ptr->add_last_serv_cell_csfb_whitelist.plmn_list_ptr,
              alloc_size_bytes,
              as_id
              ) == alloc_size_bytes)
         {
           efs_data_ptr->add_last_serv_cell_csfb_whitelist.num_of_plmns = num_of_plmns;
         }
         else
         {
           GPRS_MEM_FREE(efs_data_ptr->add_last_serv_cell_csfb_whitelist.plmn_list_ptr );
           //Re-Initialize ptr to NULL.
           efs_data_ptr->add_last_serv_cell_csfb_whitelist.plmn_list_ptr = NULL;
         }
       }
       else
       {
         MSG_GERAN_ERROR_1("Failed to allocate %d bytes for add_last_serv_cell_csfb_whitelist", alloc_size_bytes);
       }
     }
   }
#endif /*FEATURE_GERAN_ADD_LAST_SCELL_CSFB_OPT*/

#ifdef FEATURE_FAKE_BTS_CELL_INFO
  // Read /nv/item_files/modem/geran/grr/fake_bs_detection_enabled
  {
    int8 efs_val; 
    if (geran_efs_read_per_sub(GERAN_EFS_FAKE_BS_DETECTION_ENABLED,
                                 &efs_val,
                                 sizeof(efs_val),
                                 as_id) < 0)
  
    {
      MSG_GERAN_HIGH_0("EFS-NV fake_bs_detection_enabled not present");
        efs_val = -1;
    }
   
    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }
   
    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("fake bs detection is ENABLED");
      efs_data_ptr->fake_bs_detection_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("fake bs detection is DISABLED");
      efs_data_ptr->fake_bs_detection_enabled = FALSE;
    }
  }
#endif /* FEATURE_FAKE_BTS_CELL_INFO */

#ifdef FEATURE_FAKE_BTS_CELL_BARRING
   // Read "/nv/item_files/modem/geran/grr/fake_bts_cell_barring_enabled"
   {
     int8 efs_val; 
     if (geran_efs_read_per_sub(GERAN_EFS_FAKE_BTS_CELL_BARRING,
                                  &efs_val,
                                  sizeof(efs_val),
                                  as_id) < 0)
    
     {
        MSG_GERAN_HIGH_0("EFS-NV fake_bts_cell_barring_enabled not present");
        efs_val = -1;
     }
     
      // Sanity-check the result is 0 or 1
     if ((efs_val != 0) && (efs_val != 1))
     {
       efs_val = 1;  // Default is ENABLE for this EFS-NV item
     }
     
      // Update the module store
     if (efs_val == 1)
     {
       MSG_GERAN_HIGH_0("fake bts cell barring is ENABLED");
       efs_data_ptr->fake_bts_cell_barring_enabled = TRUE;
     }
     else
     {
       MSG_GERAN_HIGH_0("fake bts cell barring is DISABLED");
       efs_data_ptr->fake_bts_cell_barring_enabled = FALSE;
     }
   }
#endif /* FEATURE_FAKE_BTS_CELL_BARRING */
  
  // Read /nv/item_files/modem/geran/revert_to_rank_base
  {
     struct fs_stat file_status;
  
     efs_data_ptr->revert_to_rank_base.num_of_plmns = 0;   
  
     if (geran_efs_stat_per_sub(
            GERAN_EFS_REVERT_TO_RANK_BASE,
            &file_status,
            as_id))
     {
       // EFS file size in bytes
       fs_size_t file_size_bytes = file_status.st_size;
  
       // Number of PLMNs that can be represented by this file size, capped at MAX_NUM_OF_PLMNS
       int num_of_plmns = MIN((file_size_bytes / BYTES_PER_PLMN), MAX_NUM_OF_PLMNS);
  
       // Number of bytes to allocate to store the PLMN list
       int alloc_size_bytes = num_of_plmns * BYTES_PER_PLMN;

       if (efs_data_ptr->revert_to_rank_base.plmn_list_ptr != NULL)
       {
         GPRS_MEM_FREE(efs_data_ptr->revert_to_rank_base.plmn_list_ptr );
         //Re-Initialize ptr to NULL.
         efs_data_ptr->revert_to_rank_base.plmn_list_ptr = NULL;
       }

       efs_data_ptr->revert_to_rank_base.plmn_list_ptr = GPRS_MEM_MALLOC(alloc_size_bytes);
  
       if (efs_data_ptr->revert_to_rank_base.plmn_list_ptr != NULL)
       {
         if (geran_efs_read_per_sub(
              GERAN_EFS_REVERT_TO_RANK_BASE,
              efs_data_ptr->revert_to_rank_base.plmn_list_ptr,
              alloc_size_bytes,
              as_id
              ) == alloc_size_bytes)
         {
           efs_data_ptr->revert_to_rank_base.num_of_plmns = num_of_plmns;
         }
       }
       else
       {
         MSG_GERAN_ERROR_1("Failed to allocate %d bytes for revert_to_rank_base", alloc_size_bytes);
       }
     }
   }

// Read "/nv/item_files/modem/geran/grr/coll_fallback_enabled"
  {
    int8 efs_val; 
    if (geran_efs_read_per_sub(GERAN_EFS_COLL_FALLBACK_ENABLED,
                                 &efs_val,
                                 sizeof(efs_val),
                                 as_id) < 0)
   
    {
       MSG_GERAN_HIGH_0("EFS-NV coll_fallback_enabled not present");
       efs_val = -1;
    }
    
     // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }
    
     // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("coll_fallback_enabled is ENABLED");
      efs_data_ptr->coll_fallback_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("coll_fallback_enabled is DISABLED");
      efs_data_ptr->coll_fallback_enabled = FALSE;
    }
  }

  // Read /nv/item_files/modem/geran/g2w_override_scell_si2q_enabled
  {
    int8 efs_val;
    struct fs_stat file_status;
    int alloc_size_bytes;

    /*
      fallback_override_scell_si2q_enabled has to be enabled read nv values from file.
      Or else default values are used
    */
  
    if (geran_efs_read_per_sub(
          GERAN_EFS_OVERRIDE_SCELL_SI2Q_ENABLED,
          &efs_val, 
          sizeof(efs_val),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("fallback_override_scell_si2q_enabled taken from present");
      efs_val = -1;
    }
  
      // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }
  
      // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("fallback_override_scell_si2q_enabled is ENABLED");
      efs_data_ptr->fallback_override_scell_si2q_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("fallback_override_scell_si2q_enabled is DISABLED");
      efs_data_ptr->fallback_override_scell_si2q_enabled = FALSE;
    }
  
    if (efs_data_ptr->fallback_override_scell_si2q_enabled)
    {
      MSG_GERAN_HIGH_0("fallback_override_scell_si2q_enabled IS ENABLED, in NV ");

      if (geran_efs_stat_per_sub(
              GERAN_EFS_OVERRIDE_SCELL_SI2Q_ENABLED,
              &file_status,
              as_id))
        {
          // EFS file size in bytes
          fs_size_t file_size_bytes = file_status.st_size;
          alloc_size_bytes = sizeof(rr_efs_pseudo_si2q_def_data_t);

          memset( &(efs_data_ptr->rr_efs_override_scell_si2q_data), 0, sizeof(rr_efs_pseudo_si2q_def_data_t));
  
          if (geran_efs_read_per_sub(
                GERAN_EFS_OVERRIDE_SCELL_SI2Q_ENABLED,
                &(efs_data_ptr->rr_efs_override_scell_si2q_data),
                alloc_size_bytes,
                as_id
              ) == alloc_size_bytes)
          {          
             /*
               Current G and Utran values are to be read. Eutran values are to be set as zero. 
               They are not used for L+W changes.
             */
  
              MSG_GERAN_HIGH_0("rr_efs_override_scell_si2q_data populated with nv values");
          }
        }
        else
        {
            MSG_GERAN_ERROR_1("Failed to allocate %d bytes for rr_efs_override_scell_si2q_data", alloc_size_bytes);
    
        }       
    }
    else
    {
      /* Set the Default Value */
      efs_data_ptr->rr_efs_override_scell_si2q_data.pseudo_si2q_def_data_valid = TRUE;
    
      /* Set Default Params Here */
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_geran_def_data.geran_priority = RR_RESEL_PRIORITY_7;
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_geran_def_data.h_prio = RR_GERAN_H_PRIO;
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_geran_def_data.thresh_gsm_low = RR_GERAN_THRESH_LOW;
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_geran_def_data.thresh_priority_search = RR_GERAN_DEF_THRESH_PRIORITY_SRCH;
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_geran_def_data.t_reselection = RR_GERAN_DEF_T_RESEL;
   
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_utran_def_data.priority = RR_RESEL_PRIORITY_6;
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_utran_def_data.qrxlevmin = RR_UTRAN_QRXLEV_MIN;
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_utran_def_data.thresh_high = RR_UTRAN_THRESH_HIGH;
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_utran_def_data.thresh_low = RR_UTRAN_THRESH_LOW;

      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_lte_def_data.priority = RR_LTE_DEF_PRIO;
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_lte_def_data.meas_bandwidth = RR_LTE_DEF_MEAS_BW;
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_lte_def_data.qrxlevmin = RR_LTE_DEF_QRXLEV_MIN;
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_lte_def_data.thresh_high = RR_LTE_DEF_THRESH_HIGH;
      efs_data_ptr->rr_efs_override_scell_si2q_data.rr_lte_def_data.thresh_low = RR_LTE_DEF_THRESH_LOW;
    }
  }

#ifdef FEATURE_PRIORITIZE_LTE_FREQ
  // Read /nv/item_files/modem/geran/prioritize_lte_meas_enabled
  {
    int8 efs_val;
    struct fs_stat file_status;
    int alloc_size_bytes;

    if (geran_efs_read_per_sub(
          GERAN_EFS_PRIORITIZE_LTE_MEAS_ENABLED,
          &efs_val, 
          sizeof(efs_val),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("[LTE Prio] EFS-NV prioritize_lte_meas_enabled is not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("[LTE Prio] prioritize_lte_meas_enabled is ENABLED");
      efs_data_ptr->prioritize_lte_meas_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("[LTE Prio] prioritize_lte_meas_enabled is DISABLED");
      efs_data_ptr->prioritize_lte_meas_enabled = FALSE;
    }

    if (efs_data_ptr->prioritize_lte_meas_enabled)
    {
      /* Set Default Params Here */
      efs_data_ptr->prioritize_lte_meas_info.prioritize_meas_timeout = RR_PRIO_LTE_DEF_MEAS_TIMEOUT;
      efs_data_ptr->prioritize_lte_meas_info.defer_timeout = RR_PRIO_LTE_DEF_DEFER_TIMEOUT;
      efs_data_ptr->prioritize_lte_meas_info.max_defer_count = RR_PRIO_LTE_MAX_DEFER_COUNT;
      efs_data_ptr->prioritize_lte_meas_info.max_lte_frequencies = RR_PRIO_LTE_DEF_FREQ_COUNT;
      efs_data_ptr->prioritize_lte_meas_info.thresh_high = RR_PRIO_LTE_DEF_THRESH_HIGH;
      efs_data_ptr->prioritize_lte_meas_info.qrxlevmin = RR_LTE_DEF_QRXLEV_MIN;

      // Read /nv/item_files/modem/geran/prioritize_lte_meas_info
      alloc_size_bytes = sizeof(rr_efs_prioritize_lte_meas_info_t);
      if (geran_efs_stat_per_sub(GERAN_EFS_PRIORITIZE_LTE_MEAS_INFO,
                                 &file_status,
                                 as_id))
      {
         if (geran_efs_read_per_sub(GERAN_EFS_PRIORITIZE_LTE_MEAS_INFO,
                                    &(efs_data_ptr->prioritize_lte_meas_info),
                                    alloc_size_bytes,
                                    as_id) ==alloc_size_bytes)
         {
           MSG_GERAN_HIGH_0("[LTE Prio] prioritize_lte_meas_info populated from NV");
           // check and validate parameters to prioritize lte frequencies
           if (efs_data_ptr->prioritize_lte_meas_info.prioritize_meas_timeout < RR_PRIO_LTE_DEF_MEAS_TIMEOUT)
           {
             MSG_GERAN_ERROR_2("[LTE Prio] prioritize_meas_timeout value %d out of range, capping at %d", 
                                efs_data_ptr->prioritize_lte_meas_info.prioritize_meas_timeout,
                                RR_PRIO_LTE_DEF_MEAS_TIMEOUT);
             efs_data_ptr->prioritize_lte_meas_info.prioritize_meas_timeout = RR_PRIO_LTE_DEF_MEAS_TIMEOUT;
           }
           if (efs_data_ptr->prioritize_lte_meas_info.defer_timeout < RR_PRIO_LTE_DEF_DEFER_TIMEOUT)
           {
             MSG_GERAN_ERROR_2("[LTE Prio] defer_timeout value %d out of range, capping at %d", 
                                efs_data_ptr->prioritize_lte_meas_info.defer_timeout,
                                RR_PRIO_LTE_DEF_DEFER_TIMEOUT);
             efs_data_ptr->prioritize_lte_meas_info.defer_timeout = RR_PRIO_LTE_DEF_DEFER_TIMEOUT;
           }
           if (efs_data_ptr->prioritize_lte_meas_info.max_defer_count > RR_PRIO_LTE_MAX_DEFER_COUNT)
           {
             MSG_GERAN_ERROR_2("[LTE Prio] max_defer_count %d out of range, capping at %d", 
                                efs_data_ptr->prioritize_lte_meas_info.max_defer_count,
                                RR_PRIO_LTE_MAX_DEFER_COUNT);
             efs_data_ptr->prioritize_lte_meas_info.max_defer_count = RR_PRIO_LTE_MAX_DEFER_COUNT;
           }
           if (efs_data_ptr->prioritize_lte_meas_info.max_lte_frequencies == 0 ||
               efs_data_ptr->prioritize_lte_meas_info.max_lte_frequencies > RR_PRIO_LTE_MAX_FREQ_COUNT)
           {
             MSG_GERAN_ERROR_2("[LTE Prio] max_lte_frequencies %d out of range, capping at %d", 
                                efs_data_ptr->prioritize_lte_meas_info.max_lte_frequencies,
                                RR_PRIO_LTE_DEF_FREQ_COUNT);
             efs_data_ptr->prioritize_lte_meas_info.max_lte_frequencies = RR_PRIO_LTE_DEF_FREQ_COUNT;
           }
           if (efs_data_ptr->prioritize_lte_meas_info.thresh_high > 31)
           {
             MSG_GERAN_ERROR_2("[LTE Prio] thresh_high %d out of range, capping at %d", 
                                efs_data_ptr->prioritize_lte_meas_info.thresh_high,
                                RR_PRIO_LTE_DEF_THRESH_HIGH);
             efs_data_ptr->prioritize_lte_meas_info.thresh_high = RR_PRIO_LTE_DEF_THRESH_HIGH;
           }
           if (efs_data_ptr->prioritize_lte_meas_info.qrxlevmin > 31)
           {
             MSG_GERAN_ERROR_2("[LTE Prio] qrxlevmin %d out of range, capping at %d", 
                                efs_data_ptr->prioritize_lte_meas_info.qrxlevmin,
                                RR_LTE_DEF_QRXLEV_MIN);
             efs_data_ptr->prioritize_lte_meas_info.qrxlevmin = RR_LTE_DEF_QRXLEV_MIN;
           }
         }
         else
         {
           MSG_GERAN_HIGH_1("[LTE Prio] Not enough data(size %d) is present in the EFS prioritize_lte_meas_info",alloc_size_bytes);
         }
      }
      else
      {
        MSG_GERAN_HIGH_0("[LTE Prio] EFS prioritize_lte_meas_info is not present");
      }
      MSG_GERAN_HIGH_5("[LTE Prio] prioritize_lte_meas_info:: prioritize_meas_timeout=%d , max_defer_count=%d , max_lte_frequencies=%d , thresh_high=%d , qrxlevmin=%d",
                        efs_data_ptr->prioritize_lte_meas_info.prioritize_meas_timeout,
                        efs_data_ptr->prioritize_lte_meas_info.max_defer_count,
                        efs_data_ptr->prioritize_lte_meas_info.max_lte_frequencies,
                        efs_data_ptr->prioritize_lte_meas_info.thresh_high,
                        efs_data_ptr->prioritize_lte_meas_info.qrxlevmin);
    }
  }
#endif /* FEATURE_PRIORITIZE_LTE_FREQ */

  // Read /nv/item_files/modem/geran/increase_lte_cell_priority
  {
    int8 efs_val;
    struct fs_stat file_status;
    int alloc_size_bytes;

    if (geran_efs_read_per_sub(
          GERAN_EFS_INCREASE_LTE_CELL_PRIORITY,
          &efs_val, 
          sizeof(efs_val),
          as_id) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV increase_lte_cell_priority is not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("EFS-NV increase_lte_cell_priority is ENABLED");
      efs_data_ptr->rr_efs_increase_lte_cell_priority_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("increase_lte_cell_priority is DISABLED");
      efs_data_ptr->rr_efs_increase_lte_cell_priority_enabled = FALSE;
    }

    if (efs_data_ptr->rr_efs_increase_lte_cell_priority_enabled)
    {
      /* Set Default Priority Here */
      efs_data_ptr->rr_efs_increase_lte_cell_priority_data.geran_default_priority = RR_DEFAULT_GERAN_PRIO;
      efs_data_ptr->rr_efs_increase_lte_cell_priority_data.lte_default_priority = RR_LTE_DEF_PRIO;
    }
  }

  return;
}

/*!
 * \brief Reads EFS-NV items and populates the local NV data.
 */
static void rr_read_efs_nv_items(const sys_modem_as_id_e_type as_id)
{
  // Obtain a pointer to the EFS data
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  MSG_GERAN_HIGH_1("Reading EFS data... (as_id=%d)", (int)as_id);

  /* I. Read device-specific EFS items first */

  // Read /nv/item_files/modem/geran/pscan_results_reuse_time_secs

  if (geran_efs_read_primary(GERAN_EFS_PSCAN_RESULTS_REUSE_TIME_SECS,
                             &efs_data_ptr->pscan_results_reuse_time_secs,
                             sizeof(efs_data_ptr->pscan_results_reuse_time_secs)) < 0)
  {
    MSG_GERAN_HIGH_1("pscan_results_reuse_time_secs not present - default=%ds",
                     RR_PSCAN_STALENESS_DEFAULT);
    efs_data_ptr->pscan_results_reuse_time_secs = RR_PSCAN_STALENESS_DEFAULT;
  }
  else
  {
    MSG_GERAN_HIGH_1("pscan_results_reuse_time_secs=%d", efs_data_ptr->pscan_results_reuse_time_secs);
  }

  // Read /nv/item_files/modem/geran/add_crh_db

  if (geran_efs_read_primary(GERAN_EFS_ADD_CRH_DB,
              &efs_data_ptr->add_crh_db, 
              sizeof(efs_data_ptr->add_crh_db)) < 0)
  {
    // If the value if not configured (file not present), the default value is 0
    efs_data_ptr->add_crh_db = 0;
  }

  // Read /nv/item_files/modem/geran/add_crh_scell_thresh_db

  if (efs_data_ptr->add_crh_db != 0)
  {
    if (geran_efs_read_primary(GERAN_EFS_ADD_CRH_SCELL_THRESH_DB,
                               &efs_data_ptr->add_crh_scell_thresh_db, 
                               sizeof(efs_data_ptr->add_crh_scell_thresh_db)) < 0)
    {
      // If the value if not configured (file not present), the default value is -70
      efs_data_ptr->add_crh_scell_thresh_db = -70;
    }
  }

  MSG_GERAN_HIGH_2("add_crh_db=%ddB add_crh_scell_thresh_db=%ddB",
                   efs_data_ptr->add_crh_db, efs_data_ptr->add_crh_scell_thresh_db);

  // Read /nv/item_files/modem/geran/grr/sys_info_cache_enabled
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_SYS_INFO_CACHE_ENABLED,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV sys_info_cache_enabled not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("Sys info cache is ENABLED");
      efs_data_ptr->sys_info_cache_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("Sys info cache is DISABLED");
      efs_data_ptr->sys_info_cache_enabled = FALSE;
    }
  }

#ifdef FEATURE_GSM_BAND_AVOIDANCE
  if (geran_efs_read_primary(GERAN_EFS_BAND_AVOID_ENABLED, 
                             &efs_data_ptr->band_avoid_enabled, 
                             sizeof(efs_data_ptr->band_avoid_enabled)) < 0)
  {
    MSG_GERAN_HIGH_0("Band Avoid enabled (default)");
    efs_data_ptr->band_avoid_enabled = TRUE;
  }
  MSG_GERAN_HIGH_1("Band Avoid enabled: %d", efs_data_ptr->band_avoid_enabled);

  if (geran_efs_read_primary(GERAN_EFS_BAND_AVOID_NCELL_THRESHOLD, 
                             &efs_data_ptr->band_avoid_ncell_threshold, 
                             sizeof(efs_data_ptr->band_avoid_ncell_threshold)) < 0)
  {   
    MSG_GERAN_HIGH_0("Band Avoid Ncell Threshold (default)"); 
    efs_data_ptr->band_avoid_ncell_threshold = 21; /*rxlev = -90dBm*/
  }
  MSG_GERAN_HIGH_1("Band Avoid Ncell Threshold: %d", efs_data_ptr->band_avoid_ncell_threshold);

  if (geran_efs_read_primary(GERAN_EFS_BAND_AVOID_MAX_MEAS_RPTS, 
                             &efs_data_ptr->band_avoid_max_meas_rpts, 
                             sizeof(efs_data_ptr->band_avoid_max_meas_rpts)) < 0)
  {   
    MSG_GERAN_HIGH_0("Band Avoid Max Meas Rpts (default)"); 
    efs_data_ptr->band_avoid_max_meas_rpts = 4;
  }
  MSG_GERAN_HIGH_1("Band Avoid Max Meas Rpts: %d", efs_data_ptr->band_avoid_max_meas_rpts);
#endif /*FEATURE_GSM_BAND_AVOIDANCE*/

  // Read /nv/item_files/modem/geran/grr/iterative_si_acq_enabled
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_ITERATIVE_SI_ACQ_ENABLED,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV iterative_si_acq_enabled not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("Iterative SI acq is ENABLED");
      efs_data_ptr->iterative_si_acq_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("Iterative SI acq is DISABLED");
      efs_data_ptr->iterative_si_acq_enabled = FALSE;
    }
  }  

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  // Read /nv/item_files/modem/geran/scell_resync_time_secs
  if (geran_efs_read_primary(GERAN_EFS_SCELL_RESYNC_TIME_SECS,
                             &efs_data_ptr->scell_resync_time_secs,
                             sizeof(efs_data_ptr->scell_resync_time_secs)) < 0)
  {
    MSG_GERAN_HIGH_1("scell_resync_time_secs not present - default=%ds",
                     RR_SCELL_RESYNC_TIME_DEFAULT);
    efs_data_ptr->scell_resync_time_secs = RR_SCELL_RESYNC_TIME_DEFAULT;
  }
  else
  {
    MSG_GERAN_HIGH_1("scell_resync_time_secs=%d", efs_data_ptr->scell_resync_time_secs);
  }
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  // Read /nv/item_files/modem/geran/eng_mode_debug_enabled
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_ENG_MODE_DEBUG_ENABLED,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV eng_mode_debug_enabled not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("Eng Mode Debug is ENABLED");
      efs_data_ptr->eng_mode_debug_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("Eng Mode Debug is DISABLED");
      efs_data_ptr->eng_mode_debug_enabled = FALSE;
    }
  }

  // Read /nv/item_files/modem/geran/grr/bcch_decodes_per_band_enabled
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_BCCH_DECODES_PER_BAND_ENABLED,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("BCCH decodes per band is ENABLED");
      efs_data_ptr->bcch_decodes_per_band_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("BCCH decodes per band is DISABLED");
      efs_data_ptr->bcch_decodes_per_band_enabled = FALSE;
    }
  }

  // /nv/item_files/modem/geran/grr/last_camped_cell_enabled

  // Note: If bcch_decodes_per_band_enabled is set, then last_camped_cell_enabled is automatically set.

  if (efs_data_ptr->bcch_decodes_per_band_enabled)
  {
    efs_data_ptr->last_camped_cell_enabled = TRUE;
  }
  else
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_LAST_CAMPED_CELL_ENABLED,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("last_camped_cell is ENABLED");
      efs_data_ptr->last_camped_cell_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("last_camped_cell DISABLED");
      efs_data_ptr->last_camped_cell_enabled = FALSE;
    }
  }

  // /nv/item_files/modem/geran/grr/g2x_measurement_control

  if (geran_efs_read_primary(GERAN_EFS_G2X_MEASUREMENT_CONTROL,
                             &efs_data_ptr->g2x_measurement_control,
                             sizeof(efs_data_ptr->g2x_measurement_control)) < 0)
  {
    MSG_GERAN_HIGH_0("g2x_measurement_control not present (default=ENABLED)");

    efs_data_ptr->g2x_measurement_control.g2w_enabled = TRUE;
    efs_data_ptr->g2x_measurement_control.g2t_enabled = TRUE;
    efs_data_ptr->g2x_measurement_control.g2l_enabled = TRUE;
  }
  else
  {
    if ((efs_data_ptr->g2x_measurement_control.g2w_enabled != TRUE &&
         efs_data_ptr->g2x_measurement_control.g2w_enabled != FALSE) ||
        (efs_data_ptr->g2x_measurement_control.g2t_enabled != TRUE &&
         efs_data_ptr->g2x_measurement_control.g2t_enabled != FALSE) ||
        (efs_data_ptr->g2x_measurement_control.g2l_enabled != TRUE &&
         efs_data_ptr->g2x_measurement_control.g2l_enabled != FALSE))
    {
      MSG_GERAN_HIGH_0("g2x_measurement_control inconsistent (default=ENABLED)");

      efs_data_ptr->g2x_measurement_control.g2w_enabled = TRUE;
      efs_data_ptr->g2x_measurement_control.g2t_enabled = TRUE;
      efs_data_ptr->g2x_measurement_control.g2l_enabled = TRUE;
    }
  }

  MSG_GERAN_HIGH_3("g2x_measurement_control(g2w_enabled:%d, g2t_enabled:%d, g2l_enabled:%d)",
                   efs_data_ptr->g2x_measurement_control.g2w_enabled,
                   efs_data_ptr->g2x_measurement_control.g2t_enabled,
                   efs_data_ptr->g2x_measurement_control.g2l_enabled);

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
  // Read /nv/item_files/modem/geran/qsh_debug_enabled
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_QSH_DEBUG_ENABLED,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV qsh_debug_enabled not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("GERAN QSH debug is ENABLED");
      efs_data_ptr->qsh_debug_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("GERAN QSH debug is DISABLED");
      efs_data_ptr->qsh_debug_enabled = FALSE;
    }
  }
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/

  if (geran_efs_read_primary(
        GERAN_EFS_CSFB_SNR_ENABLED,
        &efs_data_ptr->csfb_snr_enabled,
        sizeof(efs_data_ptr->csfb_snr_enabled)) < 0)
  {
    MSG_GERAN_HIGH_0("csfb_snr_enabled not present, use default (TRUE)");
    efs_data_ptr->csfb_snr_enabled = TRUE;
  }
  else
  {
    MSG_GERAN_HIGH_1("csfb_snr_enabled=%d", efs_data_ptr->csfb_snr_enabled);
  }

  if (geran_efs_read_primary(
        GERAN_EFS_CSFB_FCCH_SNR_OFFSET,
        &efs_data_ptr->csfb_fcch_snr_offset,
        sizeof(efs_data_ptr->csfb_fcch_snr_offset)) < 0)
  {
    MSG_GERAN_HIGH_0("csfb_fcch_snr_offset not present, use default (4)");
    efs_data_ptr->csfb_fcch_snr_offset = 4; /* it is a SNR index */
  }
  else
  {
    MSG_GERAN_HIGH_1("csfb_fcch_snr_offset=%d", efs_data_ptr->csfb_fcch_snr_offset);
  }
    
  if (geran_efs_read_primary(
        GERAN_EFS_CSFB_RSSI_OFFSET,
        &efs_data_ptr->csfb_rssi_offset,
        sizeof(efs_data_ptr->csfb_rssi_offset)) < 0)
  {
    MSG_GERAN_HIGH_0("csfb_rssi_offset not present, use default (15)");
    efs_data_ptr->csfb_rssi_offset = 15; /* 10 dbm by default */
  }
  else
  {
    MSG_GERAN_HIGH_1("csfb_rssi_offset=%d", efs_data_ptr->csfb_rssi_offset);
  }
// Read /nv/item_files/modem/geran/camp_for_emergency_call_enabled
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_CAMP_FOR_EMERGENCY_CALL,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV camp_for_emergency_call_enabled not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("GERAN CAMP FOR EMERGENCY CALL is ENABLED");
      efs_data_ptr->camp_for_emergency_call_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("GERAN CAMP FOR EMERGENCY CALL is DISABLED");
      efs_data_ptr->camp_for_emergency_call_enabled = FALSE;
    }
  }
  
  // Read /nv/item_files/modem/geran/cs_camping_when_no_si13
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_CS_CAMP_WHEN_NO_SI13_IN_CELL,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV cs_camping_when_no_si13 not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("CS camping if no SI13  is ENABLED");
      efs_data_ptr->cs_camping_when_no_si13 = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("CS camping if no SI13 is DISABLED");
      efs_data_ptr->cs_camping_when_no_si13 = FALSE;
    }
  }
  // Read /nv/item_files/modem/geran/use_bcch_priority
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_USE_BCCH_PRIO_WHEN_DED_PRIO_IS_VALID_AND_MISSING,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS NV Use BCCH priority when dedicated priorty is valid but missing configured BCCH cell NOT present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("Use BCCH priority when dedicated priorty is valid but missing configured BCCH cell is ENABLED");
      efs_data_ptr->use_bcch_priority = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("Use BCCH priority when dedicated priorty is valid but missing configured BCCH cell is DISABLED");
      efs_data_ptr->use_bcch_priority = FALSE;
    }
  }

  // Read /nv/item_files/modem/geran/camp_when_no_si2ter_si2bis
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_CAMP_WHEN_NO_SI2BIS_SI2TER,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV camp_when_no_si2ter_si2bis not present");
      efs_val = 1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("Camping if no SI2bis or SI2ter is ENABLED");
      efs_data_ptr->camp_when_no_si2ter_si2bis = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("Camping if no SI2bis or SI2ter is DISABLED");
      efs_data_ptr->camp_when_no_si2ter_si2bis = FALSE;
    }
  }

  // Read /nv/item_files/modem/geran/slow_irat_mode_enabled
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_SLOW_IRAT_MODE_ENABLED,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV slow_irat_mode_enabled not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("Slow IRAT mode is ENABLED");
      efs_data_ptr->slow_irat_mode_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("Slow IRAT mode is DISABLED");
      efs_data_ptr->slow_irat_mode_enabled = FALSE;
    }
  }
  
#ifdef FEATURE_GSM_DECODE_ALL_FREQS

  // Read /nv/item_files/modem/geran/grr/decode_all_bcch_freqs
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_DECODE_ALL_BCCH_FREQS,
                               &efs_val,
                               sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV decode_all_bcch_freqs not present");
      efs_val = -1;
    }

    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 1;  // Default is ENABLED for this EFS-NV item
    }

    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("decode_all_bcch_freqs is ENABLED");
      efs_data_ptr->decode_all_bcch_freqs = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("decode_all_bcch_freqs is DISABLED");
      efs_data_ptr->decode_all_bcch_freqs = FALSE;
    }
  }
#endif //FEATURE_GSM_DECODE_ALL_FREQS
  /* II. Read sub-specific EFS items next */
  rr_read_carrier_specific_efs_nv_items(as_id);


  // nv/item_files/modem/geran/grr/rr_efs_fcell_list
  {
    struct fs_stat file_status;
    uint8 i,grr_fcell_index = 0, num_of_gsm_cells = 0;
    sys_mcc_type mcc;
    sys_mnc_type mnc;
    boolean plmn_is_undefined;
    boolean mnc_includes_pcs_digit;

    rr_efs_forbidden_cells_list_t forbidden_cells_list; /* This contains the F-Cells of all the RATs*/
    rr_efs_geran_forbidden_cells_list_t *fcells_list_ptr = rr_nv_get_f_cells_list_ptr(); /* pointer to GERAN only EFS F-Cells */

    forbidden_cells_list.num_of_barred_cells = 0;
    fcells_list_ptr->num_of_fcells = 0;
  
    if (efs_stat(GERAN_EFS_RR_FCELL_LIST,&file_status) == 0)
    {
      // EFS file size in bytes
      fs_size_t file_size_bytes = file_status.st_size;
      #define BYTES_PER_CELL_INFO  (sizeof(rr_efs_forbidden_cells_t))
      #define MAX_NUM_OF_CELLS_PER_RAT 20
      #define MAX_RATS 4

      // Number of forbidden cells that can be represented by this file size
      int number_of_barred_cells = MIN((file_size_bytes / BYTES_PER_CELL_INFO), MAX_NUM_OF_CELLS_PER_RAT * MAX_RATS);
     
      if(number_of_barred_cells)
      {
        // Number of bytes to allocate to store the CGI list
        int alloc_size_bytes = number_of_barred_cells * BYTES_PER_CELL_INFO;
        
        forbidden_cells_list.forbidden_cells_ptr = GPRS_MEM_MALLOC(alloc_size_bytes);
        RR_NULL_CHECK_FATAL(forbidden_cells_list.forbidden_cells_ptr);
        /* memset structure to avoid holding invalid values */
        memset(forbidden_cells_list.forbidden_cells_ptr, 0xFF, alloc_size_bytes);

        if (efs_get(
              GERAN_EFS_RR_FCELL_LIST,
              forbidden_cells_list.forbidden_cells_ptr,
              alloc_size_bytes
              ) == alloc_size_bytes)
        {
          forbidden_cells_list.num_of_barred_cells = number_of_barred_cells;
        }

        // Find the number of GSM F-cells in the EFS F-cell list
        for (i=0;
             ((i<forbidden_cells_list.num_of_barred_cells) && 
              (SYS_SYS_MODE_GSM == forbidden_cells_list.forbidden_cells_ptr[i].rat_type) &&
              (num_of_gsm_cells < MAX_NUM_OF_CELLS_PER_RAT));
              i++)
        {
          num_of_gsm_cells++;
        }
        if(num_of_gsm_cells)
        {
          alloc_size_bytes = num_of_gsm_cells * sizeof(rr_efs_geran_forbidden_cells_t);
          fcells_list_ptr->fcells_ptr = GPRS_MEM_MALLOC(alloc_size_bytes);
          RR_NULL_CHECK_FATAL(fcells_list_ptr->fcells_ptr);

          grr_fcell_index = 0;
          for (i=0; (i < forbidden_cells_list.num_of_barred_cells); i++)
          {
            if ((forbidden_cells_list.forbidden_cells_ptr[i].rat_type == SYS_SYS_MODE_GSM) &&
                (grr_fcell_index < MAX_NUM_OF_CELLS_PER_RAT))
            {
              fcells_list_ptr->fcells_ptr[grr_fcell_index].lai.plmn_id = 
                forbidden_cells_list.forbidden_cells_ptr[i].plmn_id;
              fcells_list_ptr->fcells_ptr[grr_fcell_index].lai.location_area_code = 
                (uint16) forbidden_cells_list.forbidden_cells_ptr[i].location_area_code;
              fcells_list_ptr->fcells_ptr[grr_fcell_index].ci = 
                (uint16) forbidden_cells_list.forbidden_cells_ptr[i].ci;
                
              sys_plmn_get_mcc_mnc(fcells_list_ptr->fcells_ptr[grr_fcell_index].lai.plmn_id,
                                   &plmn_is_undefined,
                                   &mnc_includes_pcs_digit,
                                   &mcc,
                                   &mnc);

              MSG_GERAN_MED_5("EFS F-Cell: Added NV entry %d:: PLMN=%d-%d, LAC=%d, CI=%d ",
                               grr_fcell_index,
                               mcc,
                               mnc,
                               fcells_list_ptr->fcells_ptr[grr_fcell_index].lai.location_area_code,
                               fcells_list_ptr->fcells_ptr[grr_fcell_index].ci);

              grr_fcell_index++;
            }
          }
        }
        else
        {
          MSG_GERAN_LOW_0("No GSM F-cells in GERAN_EFS_RR_FCELL_LIST");
        }
        GPRS_MEM_FREE(forbidden_cells_list.forbidden_cells_ptr);
        //Re-Initialize ptr to NULL.
        forbidden_cells_list.forbidden_cells_ptr = NULL;
      }
      else
      {
        MSG_GERAN_LOW_0("Number of bytes of GERAN_EFS_RR_FCELL_LIST = 0");
      }
      fcells_list_ptr->num_of_fcells = grr_fcell_index;
      MSG_GERAN_MED_1("Number of GERAN Forbidden cell entries in EFS is %d", fcells_list_ptr->num_of_fcells);
    }
  }

  return;
}

/*!
 * \brief Updates the last_camped_call EFS file with the details of the last cell where service was obtained.
 *        Note: If the EFS file does not currently exist, it will be created.
 *        Note: If the last service indication was OOS, then the EFS file will be deleted (if it exists).
 * 
 * \param gas_id (in)
 */
static void write_efs_last_camped_cell(const sys_modem_as_id_e_type as_id)
{
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  if (efs_data_ptr->last_camped_cell.runtime.valid)
  {
    int result; // result of file operation

    if (FALSE == efs_data_ptr->last_camped_cell.efs.valid)
    {
      // The 'last_camped_cell' EFS file doesn't exist. Check the directory structure exists first.

      // Create /nv/item_files/modem/geran folder (if it doesn't exist)
      result = GERAN_EFS_MKDIR("/nv/item_files/modem/geran", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

      MSG_GERAN_HIGH_1("Creating /nv/item_files/modem/geran/ (result=%d)", result);

      // Create /nv/item_files/modem/geran/grr folder (if it doesn't exist)
      result = GERAN_EFS_MKDIR("/nv/item_files/modem/geran/grr", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

      MSG_GERAN_HIGH_1("Creating /nv/item_files/modem/geran/grr/ (result=%d)", result);
    }

    /**
     * Compare the runtime content witht the EFS file content. If the content differs, then update write the 
     * runtime content to the EFS file. 
     * Note: It may be that the EFS file doesn't exist. If this is the case, then last_camped_cell.efs.valid 
     * will be FALSE, which won't match last_camped_cell.runtime.valid (which is TRUE) - therefore the below 
     * comparison will fail, and the EFS file wil get created during the write operation.
     */
    if (memcmp(&efs_data_ptr->last_camped_cell.runtime,
               &efs_data_ptr->last_camped_cell.efs,
               sizeof(efs_data_ptr->last_camped_cell.runtime)) != 0)
    {
      int fd;     // File Descriptor

      /**
       * Open /nv/item_files/modem/geran/grr/last_camped_cell. 
       * If the file doesn't exist, it is created (O_CREAT). 
       * If the file does exist, it is truncated to zero length (i.e. contents deleted) (O_TRUNC).
       */
      fd = GERAN_EFS_OPEN(GERAN_EFS_LAST_CAMPED_CELL,
                          O_WRONLY | O_CREAT | O_TRUNC,
                          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, 
                          as_id);

      MSG_GERAN_HIGH_1("Writing /nv/item_files/modem/geran/grr/last_camped_cell (fd=%d)", fd);

      // A return of 0 or greater means success
      if (fd >= 0)
      {
        // Write to the file
        result = GERAN_EFS_WRITE(fd,
                                 (void *)&efs_data_ptr->last_camped_cell.runtime.data,
                                 sizeof(efs_data_ptr->last_camped_cell.runtime.data));

        if (result <= 0)
        {
          MSG_GERAN_ERROR_1("Error writing last_camped_cell (%d)", result);
        }

        (void) GERAN_EFS_CLOSE(fd);
      }
      else
      {
        MSG_GERAN_ERROR_1("Error creating/opening last_camped_cell (%d)", fd);
      }

      // Now copy 'runtime' to 'efs' as the EFS file matches the contents of 'runtime' now
      efs_data_ptr->last_camped_cell.efs = efs_data_ptr->last_camped_cell.runtime;
    }
    else
    {
      MSG_GERAN_HIGH_0("No update required to /nv/item_files/modem/geran/grr/last_camped_cell");
    }
  }
  else if (efs_data_ptr->last_camped_cell.efs.valid)
  {
    mcfg_fs_status_e_type fs = GERAN_EFS_DELETE(GERAN_EFS_LAST_CAMPED_CELL, as_id);

    MSG_GERAN_HIGH_1("Deleted /nv/item_files/modem/geran/grr/last_camped_cell (fs=%d)", fs);

    reset_efs_last_camped_cell(as_id);
  }
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/

/*!
 * \brief Returns the value of SPLIT_PAGE_CYCLE stored in NV.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return uint16 
 */
uint16 rr_get_nv_split_page_cycle(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->split_page_cycle;

} /* end rr_get_nv_split_page_cycle() */

/*!
 * \brief Returns the value of NON_DRX_TIMER.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return uint8 
 */
uint8 rr_get_nv_non_drx_timer(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->non_drx_timer;

} /* end rr_get_nv_non_drx_timer() */

/*!
 * \brief Returns the value of MULTISLOT_CLASS stored in NV.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return uint8 
 */
uint8 rr_get_nv_multislot_class(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->multislot_class;

} /* end rr_get_nv_multislot_class() */


/*!
 * \brief Returns TRUE if EGPRS is enabled in NV; FALSE otherwise.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return boolean - TRUE if enabled; FALSE otherwise
 */
boolean rr_get_nv_egprs_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->egprs_enabled;

} /* end rr_get_nv_egprs_enabled() */

/*!
 * \brief Returns the value of EGPRS_MULTISLOT_CLASS stored in NV.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return uint8
 */
uint8 rr_get_nv_egprs_multislot_class(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->egprs_multislot_class;

} /* end rr_get_nv_egprs_multislot_class() */

/*!
 * \brief Returns TRUE if the NV parameter EGPRS_8PSK_POWER_CLASS is non-zero; FALSE if zero.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return boolean - TRUE if enabled; FALSE otherwise
 */
boolean rr_get_nv_egprs_8psk_ul_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->egprs_8psk_ul_enabled;

} /* end rr_get_nv_egprs_8psk_ul_enabled() */

/*!
 * \brief Returns the egprs 8psk power capability.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return uint8 
 */
uint8 rr_get_nv_egprs_8psk_power_capability(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->egprs_8psk_power_capability;

} /* end rr_get_nv_egprs_8psk_power_capability() */

/*!
 * \brief Returns TRUE if GERAN FEATURE PACK 1 is enabled in NV; FALSE otherwise.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return boolean - TRUE if enabled; FALSE otherwise
 */
boolean rr_get_nv_geran_feature_pack_1_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->geran_feature_pack_1_enabled;

} /* end rr_get_nv_geran_feature_pack_1_enabled() */

/*!
 * \brief Returns the current mask from NV that indicates support for inter-RAT NACC.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return uint8 - 0x01 indicates W2G, 0x02 indicates G2W, 0x03 indicates both
 */
uint8 rr_get_nv_interrat_nacc_support(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->interrat_nacc_support;

} /* end rr_get_nv_interrat_nacc_support() */

#ifdef FEATURE_GSM_DTM

/*!
 * \brief Indicates if DTM is enabled in NV.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return boolean - TRUE if enabled; FALSE otherwise
 */
boolean rr_get_nv_dtm_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->dtm_enabled;

} /* end rr_get_nv_dtm_enabled() */

/*!
 * \brief Returns the value of DTM_EGPRS_MULTISLOT_SUBCLASS stored in NV.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return uint8 
 */
uint8 rr_get_nv_dtm_egprs_multislot_subclass(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->dtm_egprs_multislot_subclass;

} /* end rr_get_nv_dtm_egprs_multislot_subclass() */

/*!
 * \brief Returns the value of DTM_MULTISLOT_CLASS, derived from DTM_EGPRS_MULTISLOT_SUBCLASS.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return uint8 
 */
uint8 rr_get_nv_dtm_multislot_class(const sys_modem_as_id_e_type as_id)
{
  uint8 dtm_multislot_subclass = rr_get_nv_dtm_egprs_multislot_subclass(as_id);

  return rr_msc_conv_dtm_multislot_subclass_to_class(dtm_multislot_subclass);
}

#ifdef FEATURE_GSM_EDTM
/*!
 * \brief Indicates if EDTM is enabled in NV.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return boolean - TRUE if enabled; FALSE otherwise
 */
boolean rr_get_nv_edtm_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->edtm_enabled;
}
#endif /* FEATURE_GSM_EDTM */

#endif /* FEATURE_GSM_DTM */

/*!
 * \brief Returns the value of GEA_SUPPORTED stored in NV.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return uint8
 */
uint8 rr_get_nv_gea_supported(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->gea_supported;

} /* end rr_get_nv_gea_supported() */

#ifdef FEATURE_GSM_GPRS_MSC33

/*!
 * \brief Returns the value of HMSC_GPRS_CODED_MS_CLASS stored in NV.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return uint8
 */
uint8 rr_get_nv_hmsc_gprs_coded_ms_class(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->hmsc_gprs_coded_ms_class;

} /* end rr_get_nv_hmsc_gprs_coded_ms_class() */

/*!
 * \brief Returns the value of HMSC_EGPRS_CODED_MS_CLASS stored in NV.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return uint8
 */
uint8 rr_get_nv_hmsc_egprs_coded_ms_class(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_data_ptr(as_id)->hmsc_egprs_coded_ms_class;

} /* end rr_get_nv_hmsc_egprs_coded_ms_class() */

#endif /* FEATURE_GSM_GPRS_MCS33 */

#ifdef FEATURE_VAMOS
/*!
 * \brief Indicates if VAMOS is enabled in NV.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return boolean - TRUE is VAMOS is enabled in NV (VAMOS-I or VAMOS-II); FALSE otherwise
 */
boolean rr_get_nv_vamos_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->vamos_enabled;
}
#endif /* FEATURE_VAMOS */

/*!
 * \brief Indicates if Fast SI Acq during CSFB is enabled in NV.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return boolean - TRUE if Fast SI Acq during CSFB is enabled, FALSE otherwise
 */
boolean rr_get_nv_fast_si_acq_during_csfb_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->fast_si_acq_during_csfb_enabled;
}

boolean rr_nv_x2g_fast_si_acq_feature_is_enabled(const sys_modem_as_id_e_type as_id)
{
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  if ((efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list_present) &&
      (efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.num_of_plmns > 0))
  {
    MSG_GERAN_HIGH_0("X2G SKIP SI13 enabled");
    return TRUE;
  }
  else
  {
    MSG_GERAN_HIGH_0("X2G SKIP SI13 NOT enabled");
    return FALSE;
  }	
}

boolean rr_nv_x2g_fast_si_acq_is_plmn_match(sys_plmn_id_s_type plmn, const sys_modem_as_id_e_type as_id)
{
  int i;

  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  // Point to the first PLMN in the list
  sys_plmn_id_s_type *plmn_id_ptr = efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.plmn_list_ptr;

  if (plmn_id_ptr != NULL)
  {
    sys_mcc_type mcc;
    sys_mnc_type mnc;
    boolean is_undefined;
    boolean includes_pcsd;

    sys_plmn_get_mcc_mnc(
      plmn,
      &is_undefined,
      &includes_pcsd,
      &mcc,
      &mnc
    );

    MSG_GERAN_HIGH_2(
      "X2G SKIP SI13 compare PLMN=(%d,%d)",
      mcc,
      mnc
    );

    for (i = 0;
         (i < efs_data_ptr->x2g_fast_si_acq_info.x2g_fast_si_acq_white_list.num_of_plmns) && (i < MAX_NUM_OF_PLMNS);
         i++)
    {
      sys_plmn_get_mcc_mnc(
        *plmn_id_ptr,
        &is_undefined,
        &includes_pcsd,
        &mcc,
        &mnc
      );

      MSG_GERAN_HIGH_2(
        "x2g_fast_si_acq_white_list PLMN=(%d,%d)",
        mcc,
        mnc
      );

      if (sys_plmn_match(plmn, *plmn_id_ptr))
      {
        return TRUE;
      }

      // Point to the next PLMN in the list
      ++plmn_id_ptr;
    }
  }

  return FALSE;
}

/*!
 * \brief Indicates if Fast SI Acq when CS-only is enabled in NV.
 *        Note: Feature is disabled for a Type Approval SIM. 
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_fast_si_acq_when_cs_only_is_enabled(const sys_modem_as_id_e_type as_id)
{
    
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;
  }

  return rr_nv_get_efs_data_ptr(as_id)->fast_si_acq_when_cs_only_enabled;
}

/*!
 * \brief Indicates if the supplied PLMN is present in the C1 relaxation white-list.
 * 
 * \return boolean - TRUE if present, FALSE otherwise
 */
static boolean rr_nv_check_plmn_in_whitelist(sys_plmn_id_s_type *plmn_id_ptr, int num_list_entries, sys_plmn_id_s_type plmn)
{
  int i;

  MSG_GERAN_HIGH_1("Num of PLMN %d ", num_list_entries);
  
  if (plmn_id_ptr != NULL)
  {
    sys_mcc_type mcc;
    sys_mnc_type mnc;
    boolean is_undefined;
    boolean includes_pcsd;

    sys_plmn_get_mcc_mnc(
      plmn,
      &is_undefined,
      &includes_pcsd,
      &mcc,
      &mnc
    );

    for (i = 0; ((i < num_list_entries) && (i < MAX_NUM_OF_PLMNS)); i++)
    {
      sys_plmn_get_mcc_mnc(
        *plmn_id_ptr,
        &is_undefined,
        &includes_pcsd,
        &mcc,
        &mnc
      );

      MSG_GERAN_HIGH_2("White list PLMN=(%d,%d)",mcc,mnc);

      if (sys_plmn_match(plmn, *plmn_id_ptr))
      {
        return TRUE;
      }

      // Point to the next PLMN in the list
      ++plmn_id_ptr;
    }
  }

  return FALSE;
}

rr_efs_pseudo_si2q_def_data_t* rr_get_efs_nv_g2X_override_scell_si2q_data(const sys_modem_as_id_e_type as_id)
{
  return (&rr_nv_get_efs_data_ptr(as_id)->rr_efs_override_scell_si2q_data);
}

boolean rr_get_efs_nv_coll_fallback_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->coll_fallback_enabled;
}

#ifdef FEATURE_FAKE_BTS_CELL_INFO
boolean rr_get_fake_bs_detection_enabled(const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  return rr_nv_get_efs_data_ptr(as_id)->fake_bs_detection_enabled;
}
#endif /* FEATURE_FAKE_BTS_CELL_INFO */

#ifdef FEATURE_FAKE_BTS_CELL_BARRING
boolean rr_get_fake_bts_cell_barring_enabled(const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  return rr_nv_get_efs_data_ptr(as_id)->fake_bts_cell_barring_enabled;
}
#endif /* FEATURE_FAKE_BTS_CELL_BARRING */

#ifdef FEATURE_FAKE_BTS_CELL_DEPRIORITIZE
boolean rr_get_fake_bts_cell_deprioritization_enabled(const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  /*currently cell deprioritization does not have a separate EFS, it uses cell barring EFS only*/
  return rr_nv_get_efs_data_ptr(as_id)->fake_bts_cell_barring_enabled;
}
#endif /*FEATURE_GCELL_DEPRIORITIZING */
#ifdef FEATURE_LOCK_ARFCN
/*!
 * \brief Inidicates if channel is presnet in EFS.
 * 
 * \param channel_to_search (in) -  ARFCN to be serached
 * \param gas_id (in) -  GAS-ID)
 * 
 * \return boolean - TRUE if found, FALSE otherwise
 */
boolean rr_nv_channel_in_efs(uint16 channel_to_search, const gas_id_t gas_id)
{
  uint8 i;

  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  uint16 *current_channel_ptr = efs_data_ptr->rr_efs_arfcn_list.arfcn_num_list_ptr;
  
  MSG_GERAN_HIGH_2_G("ARFCN %d needs to be searched, number of ARFCN in EFS =%d",
                    channel_to_search,
                    efs_data_ptr->rr_efs_arfcn_list.num_of_arfcn);

  if (current_channel_ptr != NULL)
  {
    for (i=0; i<efs_data_ptr->rr_efs_arfcn_list.num_of_arfcn ; ++i)
    {
      if (channel_to_search == *(current_channel_ptr))
      {
        MSG_GERAN_HIGH_0("ARFCN found");
        return TRUE;
      }
      MSG_GERAN_HIGH_1("EFS ARFCN =%d", *(current_channel_ptr)); 
      ++current_channel_ptr;
    }
  }
 return FALSE;
 
}
#endif /*FEATURE_LOCK_ARFCN*/

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER 
boolean rr_nv_read_pch_during_transfer_is_plmn_match(sys_plmn_id_s_type plmn, const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  if (TRUE == mcfg_gcf_nv_get_status())
  {
    MSG_GERAN_HIGH_0_G("Read PCH during xfer disabled (GCF is ON)");  
    return FALSE;
  }
 
  return (rr_nv_check_plmn_in_whitelist(efs_data_ptr->read_pch_during_transfer_whitelist.plmn_list_ptr,
                                        efs_data_ptr->read_pch_during_transfer_whitelist.num_of_plmns,
                                        plmn));
}
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

boolean rr_nv_c1_relaxation_is_plmn_match(sys_plmn_id_s_type plmn, const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);
  
  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;    
  }

  return (rr_nv_check_plmn_in_whitelist(efs_data_ptr->negative_c1_white_list.plmn_list_ptr,
                                        efs_data_ptr->negative_c1_white_list.num_of_plmns,
                                        plmn));
}

/*!
 * \brief Returns the value of the power-scan re-use timer (0=disabled)
 * 
 * \return rr_duration_t - timer vaue in milliseconds
 */
rr_duration_t rr_nv_get_pscan_results_reuse_time_millisecs(const sys_modem_as_id_e_type as_id)
{
  rex_timer_cnt_type timeout_secs = rr_nv_get_efs_data_ptr(as_id)->pscan_results_reuse_time_secs;

  return (CONVERT_SECONDS_TO_MILLISECONDS(timeout_secs));
}

#ifdef FEATURE_SGLTE
#ifdef FEATURE_SGLTE_G2X_CONN_MODE_RESEL
/*!
 * \brief Indicates if SGLTE Connected Mode Reselection feature is enabled in NV.
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_get_nv_sglte_g2x_conn_mode_resel_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->sglte_g2x_conn_mode_resel_enabled;
}
#endif /* FEATURE_SGLTE_G2X_CONN_MODE_RESEL */
#endif /* FEATURE_SGLTE */

#ifdef FEATURE_FAST_G2L_PSEUDO_SI2Q
boolean rr_is_efs_nv_fast_g2l_using_pseudo_si2q_enabled(const sys_modem_as_id_e_type as_id)
{
  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;    
  }

  return rr_nv_get_efs_data_ptr(as_id)->fast_g2l_using_pseudo_si2q_enabled;
}

boolean rr_is_efs_nv_fast_g2l_using_pseudo_si2q_data_valid(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->rr_efs_pseudo_si2q_def_data.pseudo_si2q_def_data_valid;
}

rr_efs_pseudo_si2q_def_data_t* rr_get_efs_nv_fast_g2l_pseudo_si2q_data(const sys_modem_as_id_e_type as_id)
{
  return (&rr_nv_get_efs_data_ptr(as_id)->rr_efs_pseudo_si2q_def_data);
}
#endif /* FEATURE_FAST_G2L_PSEUDO_SI2Q */

/*!
 * \brief Returns the additional C2 cell reselection hysteresis values.
 * 
 * \param add_crh_db_ptr (out)
 * \param add_crh_scell_thresh_db_ptr (out)
 * \param as_id (in)
 * 
 * \return boolean - TRUE if configured, FALSE otherwise
 */
boolean rr_nv_get_add_crh_enabled(
  int8 *add_crh_db_ptr,
  int8 *add_crh_scell_thresh_db_ptr,
  const sys_modem_as_id_e_type as_id
)
{
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  if (efs_data_ptr->add_crh_db != 0)
  {
    *add_crh_db_ptr = efs_data_ptr->add_crh_db;
    *add_crh_scell_thresh_db_ptr = efs_data_ptr->add_crh_scell_thresh_db;


    return TRUE;
  }

  return FALSE;
}

/*!
 * \brief Returns the value of the plmn_search_freq_pruning_enabled. 
 *        Note: Feature is disabled for a Type Approval SIM. 
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_get_plmn_search_freq_pruning_enabled(const sys_modem_as_id_e_type as_id)
{
 
  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;
  }

  return rr_nv_get_efs_data_ptr(as_id)->plmn_search_freq_pruning_enabled;
}

/*!
 * \brief Indicates if the sys info cache functionality is enabled.
 *        Note: Feature is disabled for a Type Approval SIM. 
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_sys_info_cache_is_enabled(const sys_modem_as_id_e_type as_id)
{
  
  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;
  }

  return rr_nv_get_efs_data_ptr(as_id)->sys_info_cache_enabled;
}

/*!
 * \brief Indicates if the iterative updating during SI acq feature is enabled.
 *        Note: Feature is not disabled for a Type Approval SIM.
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_iterative_si_acq_is_enabled(const sys_modem_as_id_e_type as_id)
{
  // Note: Feature is not disabled for a Type Approval SIM

  return rr_nv_get_efs_data_ptr(as_id)->iterative_si_acq_enabled;
}

/*!
 * \brief Indicates if eng mode debug is enabled.
 *        
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_eng_mode_debug_is_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->eng_mode_debug_enabled;
}

#ifdef FEATURE_GSM_BAND_AVOIDANCE
/**
  @brief Returns if band avoidance is enabled
*/
boolean rr_is_band_avoidance_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->band_avoid_enabled;
}

/**
  @brief Returns the threshold for band avoidance ncells as 
         rxlev (0.63)
*/
uint8 rr_get_band_avoid_ncell_threshold(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->band_avoid_ncell_threshold;
}

/**
  @brief Returns the max number of meas repts that can be sent 
  when doing band avoidance. 
*/
uint8 rr_get_band_avoid_max_meas_rpts(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->band_avoid_max_meas_rpts;
}
#endif /*FEATURE_GSM_BAND_AVOIDANCE*/

#ifdef FEATURE_LTE

boolean rr_nv_get_g2l_blind_redir_after_voice_ss_or_periodic_lau_enabled(const sys_modem_as_id_e_type as_id)
{
  
  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;
  }

  return rr_nv_get_efs_data_ptr(as_id)->g2l_blind_redir_after_voice_ss_or_periodic_lau_enabled;
}

boolean rr_nv_get_g2l_blind_redir_after_csfb_enabled(const sys_modem_as_id_e_type as_id)
{
  
  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;
  }

  return rr_nv_get_efs_data_ptr(as_id)->g2l_blind_redir_after_csfb_enabled;
}

boolean rr_nv_get_g2l_blind_redir_after_srvcc_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->g2l_blind_redir_after_srvcc_enabled;
}

/**
  @brief get csfb snr enabled
  @param as_id
  @return the value csfb_snr_enabled
*/
boolean rr_nv_get_csfb_snr_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->csfb_snr_enabled;
}

/**
  @brief get the csfb fcch snr offset for csfb cell selection
  @param as_id
  @return the value csfb_fcch_snr_offset
*/
uint16 rr_nv_get_csfb_fcch_snr_offset(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->csfb_fcch_snr_offset;
}

/**
  @brief get the csfb rssi offset for csfb cell selection
  @param as_id
  @return the value csfb_rssi_offset
*/  
uint16 rr_nv_get_csfb_rssi_offset(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->csfb_rssi_offset;
}

#endif /* FEATURE_LTE */

boolean rr_nv_get_g2w_measurements_enabled(sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->g2x_measurement_control.g2w_enabled;
}

boolean rr_nv_get_g2t_measurements_enabled(sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->g2x_measurement_control.g2t_enabled;
}

boolean rr_nv_get_g2l_measurements_enabled(sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->g2x_measurement_control.g2l_enabled;
}

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
uint32 rr_nv_get_scell_resync_time_millisecs(const sys_modem_as_id_e_type as_id)
{
  uint32 timeout_secs = (uint32)(rr_nv_get_efs_data_ptr(as_id)->scell_resync_time_secs);
  return(CONVERT_SECONDS_TO_MILLISECONDS(timeout_secs));
}

uint32 rr_nv_get_multisim_si_refresh_duration_milliseconds(const sys_modem_as_id_e_type as_id)
{
  uint32 duration_secs = (uint32)(rr_nv_get_efs_data_ptr(as_id)->multisim_si_refresh_duration_secs);
  
  return(CONVERT_SECONDS_TO_MILLISECONDS(duration_secs)); 
}

uint8 rr_nv_get_cbch_supported_modes(const sys_modem_as_id_e_type as_id)
{
  return(rr_nv_get_efs_data_ptr(as_id)->cbch_supported_modes);
}
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

/*!
 * \brief Update the last-camped cell data. This is written to EFS during power-off.
 * 
 * \param arfcn (in)
 * \param bsic (in)
 * \param lai (in)
 * \param gas_id (in)
 */
void rr_nv_set_last_camped_cell(ARFCN_T arfcn, BSIC_T bsic, LAI_T lai, const gas_id_t gas_id)
{
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(RR_GAS_ID_TO_AS_ID);

  efs_data_ptr->last_camped_cell.runtime.valid = TRUE;
  efs_data_ptr->last_camped_cell.runtime.data.arfcn = arfcn;
  efs_data_ptr->last_camped_cell.runtime.data.bsic = bsic;
  efs_data_ptr->last_camped_cell.runtime.data.lai = lai;

  return;
}

/*!
 * \brief Resets the last-camped cell data. Any associated EFS data will be deleted during power-off.
 * 
 * \param gas_id (in)
 */
void rr_nv_reset_last_camped_cell(const gas_id_t gas_id)
{
  reset_runtime_last_camped_cell(RR_GAS_ID_TO_AS_ID);

  /**
   * Note: This will set last_camped_cell.runtime.valid to FALSE. If power-off occurs before 
   * RR_SERVICE_IND/RR_PLMN_SELECT_CNF are sent indicating valid service, then the last-camped cell EFS-NV 
   * item will be deleted (if it exists). 
   */

  return;
}

/*!
 * \brief Returns the last-camped cell data.
 * 
 * \param arfcn_ptr (out)
 * \param bsic_ptr (out)
 * \param lai_ptr (out)
 * \param gas_id (in)
 * 
 * \return boolean - TRUE is the last-camped cell data is valid, FALSE otherwise.
 */
boolean rr_nv_get_last_camped_cell(
  ARFCN_T *arfcn_ptr,
  BSIC_T *bsic_ptr,
  LAI_T *lai_ptr,
  const gas_id_t gas_id
)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  if (efs_data_ptr->last_camped_cell.runtime.valid)
  {
    if (arfcn_ptr != NULL)
    {
      *arfcn_ptr = efs_data_ptr->last_camped_cell.runtime.data.arfcn;
    }

    if (bsic_ptr != NULL)
    {
      *bsic_ptr = efs_data_ptr->last_camped_cell.runtime.data.bsic;
    }

    if (lai_ptr != NULL)
    {
      *lai_ptr = efs_data_ptr->last_camped_cell.runtime.data.lai;
    }

    return TRUE;
  }

  return FALSE;
}

/*!
 * \brief Returns the band of the last-camped cell.
 * 
 * \param gas_id (in)
 * 
 * \return sys_band_T - The band of the last-camped cell, or SYS_BAND_NONE if not valid.
 */
sys_band_T rr_nv_get_last_camped_cell_band(const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  if (efs_data_ptr->last_camped_cell.runtime.valid)
  {
    return efs_data_ptr->last_camped_cell.runtime.data.arfcn.band;
  }

  return SYS_BAND_NONE;
}

/*!
 * \brief Indicates if band-by-band BCCH decoding is enabled or not.
 * 
 * \param as_id (in)
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise.
 */
boolean rr_nv_bcch_decodes_per_band_is_enabled(const sys_modem_as_id_e_type as_id)
{
  
  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;
  }

  return rr_nv_get_efs_data_ptr(as_id)->bcch_decodes_per_band_enabled;
}

#ifdef FEATURE_LTE
/*!
 * \brief Indicates if De-priotizing GSM cell  is enabled or not.
 * 
 * \param as_id (in)
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise.
 */
boolean rr_nv_depriotize_gsm_cell_is_enabled(const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  
  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;
  }

  return rr_nv_get_efs_data_ptr(as_id)->depriotize_gsm_cell;
}
#endif /*FEATURE_LTE*/

#ifdef FEATURE_GERAN_HST_OPT
/*!
 * \brief Indicates if High Speed Train Optimization can be used with the supplied PLMN.
 * 
 * \param plmn (in), gas_id (in)
 * 
 * \return boolean - TRUE if PLMN matches, FALSE otherwise.
 */
boolean rr_nv_hst_opt_is_plmn_match(sys_plmn_id_s_type plmn, const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);
  
  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;    
  }

  return (rr_nv_check_plmn_in_whitelist(efs_data_ptr->hst_opt_white_list.plmn_list_ptr,
                                        efs_data_ptr->hst_opt_white_list.num_of_plmns,
                                        plmn));
}
#endif /*FEATURE_GERAN_HST_OPT*/

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
/*!
 * \brief Indicates if the qsh debug EFS is enabled.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_qsh_debug_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->qsh_debug_enabled;
}
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/

#ifdef FEATURE_IDLE_DRX_SCALING
uint32 rr_nv_get_idle_drx_scaling_suspend_time_millisecs(const sys_modem_as_id_e_type as_id)
{
  uint32 timeout_secs = rr_nv_get_efs_data_ptr(as_id)->idle_drx_scaling_suspend_time_secs;

  return(CONVERT_SECONDS_TO_MILLISECONDS(timeout_secs));
}
#endif /* FEATURE_IDLE_DRX_SCALING */


/*!
 * \brief Called when the NV data is received from NAS.
 *  
 * The NV data is copied into local storage. The Public Store is updated later when PS access is enabled. 
 * Note: the NV data is stored according to AS-ID in this message, not GAS-ID
 * 
 * \param msg_ptr (in) - pointer to the RR_GMM_GPRS_NV_PARAMS_IND message
 */
void rr_nv_process_nv_params_ind(rr_gmm_gprs_nv_params_ind_t *msg_ptr)
{
  rr_nv_data_t *rr_nv_data_ptr;

  // Sanity-check the input
  RR_NULL_CHECK_RETURN_VOID(msg_ptr);

  // Obtain a pointer to the module data
  rr_nv_data_ptr = rr_nv_get_data_ptr(msg_ptr->as_id);

  rr_nv_data_ptr->multislot_class              = msg_ptr->nv_multislot_class;
  rr_nv_data_ptr->non_drx_timer_enc            = msg_ptr->nv_non_drx_timer;
  rr_nv_data_ptr->split_page_cycle_enc         = msg_ptr->nv_split_paging_cycle;
  rr_nv_data_ptr->egprs_enabled                = msg_ptr->nv_edge_feature_support;
  rr_nv_data_ptr->egprs_multislot_class        = msg_ptr->nv_edge_multislot_class;
  rr_nv_data_ptr->egprs_8psk_ul_enabled        = msg_ptr->nv_edge_8psk_power_class != 0;
  rr_nv_data_ptr->egprs_8psk_power_capability  = msg_ptr->nv_edge_8psk_power_capability;
  rr_nv_data_ptr->geran_feature_pack_1_enabled = msg_ptr->nv_edge_nw_ass_cell_change;
  rr_nv_data_ptr->interrat_nacc_support        = msg_ptr->nv_interrat_nacc_support;
#ifdef FEATURE_GSM_DTM
  rr_nv_data_ptr->dtm_enabled                  = msg_ptr->nv_dtm_feature_support;
  rr_nv_data_ptr->dtm_egprs_multislot_subclass = msg_ptr->nv_dtm_multislot_subclass;
#ifdef FEATURE_GSM_EDTM
  rr_nv_data_ptr->edtm_enabled                 = msg_ptr->nv_edtm_feature_support;
#endif /* FEATURE_GSM_EDTM */
#endif /* FEATURE_GSM_DTM */
  rr_nv_data_ptr->gea_supported                = msg_ptr->nv_gea_supported;
#ifdef FEATURE_GSM_GPRS_MSC33
  rr_nv_data_ptr->hmsc_gprs_coded_ms_class     = msg_ptr->nv_hmsc_gprs_coded_ms_class;
  rr_nv_data_ptr->hmsc_egprs_coded_ms_class    = msg_ptr->nv_hmsc_egprs_coded_ms_class;
#endif /* FEATURE_GSM_GPRS_MSC33 */

#if defined(NV_DISABLE_VOICE_SUPPORT_MASK) || defined(NV_DISABLE_CSDATA_SUPPORT_MASK)
  {
    // Assume all call types are allowed
    boolean voice_calls_supported = TRUE;
    boolean csdata_calls_supported = TRUE;

    /* NAS will provide the mask in the RR_GMM_GPRS_NV_PARAMS_IND primitive */
#ifdef NV_DISABLE_VOICE_SUPPORT_MASK
    if ((msg_ptr->nv_disable_call_type & NV_DISABLE_VOICE_SUPPORT_MASK) != 0)
    {
      MSG_GERAN_HIGH_1("ds%d:Voice calls are disabled by NV",(int)msg_ptr->as_id+1);
      voice_calls_supported = FALSE;
    }
#endif /* NV_DISABLE_VOICE_SUPPORT_MASK */

#ifdef NV_DISABLE_CSDATA_SUPPORT_MASK
    if ((msg_ptr->nv_disable_call_type & NV_DISABLE_CSDATA_SUPPORT_MASK) != 0)
    {
      MSG_GERAN_HIGH_1("ds%d:CS data calls are disabled by NV",(int)msg_ptr->as_id+1);
      csdata_calls_supported = FALSE;
    }
#endif /* NV_DISABLE_CSDATA_SUPPORT_MASK */

    rr_channel_mode_set_supported(
      voice_calls_supported,    // voice_calls_supported
      csdata_calls_supported,   // csdata_calls_supported
      msg_ptr->as_id            // as_id
    );
  }
#endif /* NV_DISABLE_VOICE_SUPPORT_MASK | NV_DISABLE_CSDATA_SUPPORT_MASK */

  MSG_GERAN_HIGH_0("NV params from MM");
  MSG_GERAN_HIGH_3("multislot:%d non_drx:%d, spc:%d",
                   (int)rr_nv_data_ptr->multislot_class,
                   (int)rr_nv_data_ptr->non_drx_timer_enc,
                   (int)rr_nv_data_ptr->split_page_cycle_enc );

  MSG_GERAN_HIGH_3("egprs:%d, egprs_multislot:%d, 8psk_ul:%d",
                   (int)rr_nv_data_ptr->egprs_enabled,
                   (int)rr_nv_data_ptr->egprs_multislot_class,
                   (int)rr_nv_data_ptr->egprs_8psk_ul_enabled );

#ifdef FEATURE_GSM_DTM
  {
    int edtm_support;

#ifdef FEATURE_GSM_EDTM
    edtm_support = (int)rr_nv_data_ptr->edtm_enabled;
#else
    edtm_support = 0xFF;
#endif /*  FEATURE_GSM_EDTM */

    MSG_GERAN_HIGH_3("DTM:%1d MSC:%d (EDTM:%02X)",
                     (int)rr_nv_data_ptr->dtm_enabled,
                     (int)rr_nv_data_ptr->dtm_egprs_multislot_subclass,
                     edtm_support);
  }
#endif /* FEATURE_GSM_DTM */

  MSG_GERAN_HIGH_2("geran_feature_pack_1:%d, interrat_nacc 0x%02X",
                   (int)rr_nv_data_ptr->geran_feature_pack_1_enabled,
                   (int)rr_nv_data_ptr->interrat_nacc_support);

  /* Calculate value for split_page_cycle based on encoded value */
  rr_nv_data_ptr->split_page_cycle = decode_split_page_cycle(rr_nv_data_ptr->split_page_cycle_enc);

  /* Calculate value for non_drx_timer based on encoded value */
  rr_nv_data_ptr->non_drx_timer = rr_gprs_decode_drx_timer(rr_nv_data_ptr->non_drx_timer_enc);

  if (GERAN_DATA_SPACE_INDEX_1 == rr_nv_get_data_space_index(msg_ptr->as_id))
  {
    // Use the reception of the first NV message to read the GERAN EFS-NV items
    geran_read_efs_nv_items();
  }

  /* Only GRR EFS items are carrier-specific and should be read per sub */
  rr_read_efs_nv_items(msg_ptr->as_id);

  /* Inform the Multi-Slot Class module that NV data is available */
  rr_msc_nv_ready(msg_ptr->as_id);

} /* end rr_nv_process_nv_params_ind() */

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*!
 * \brief Called from GCOMMON to post GERAN_NV_REFRESH_IND to GRR queue.
 *  
 * \param gas_id - GAS ID
 */
void rr_nv_send_nv_refresh_ind(const gas_id_t gas_id)
{
  MSG_GERAN_HIGH_1_G("Send GERAN_NV_REFRESH_IND to GRR (gas_id=%d)", gas_id);
  rr_send_imsg_geran_nv_refresh_ind(gas_id);
}

/*!
 * \brief Called when the NV refresh ind received from MCFG.
 *  
 * Re-reads carrier specific EFS/NV items. 
 * 
 * \param gas_id - GAS ID
 */
void rr_nv_process_nv_refresh_ind(const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;

  MSG_GERAN_HIGH_1("Refreshing EFS data... (as_id=%d)", (int)as_id);

  /* Re-read carrier specific EFS items */
  rr_read_carrier_specific_efs_nv_items(as_id);
}
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

/*!
 * \brief Updates NV/EFS with any data which needs to be written.
 * 
 * \param gas_id (in)
 */
void rr_nv_write(const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;

  rr_acq_db_save(gas_id);

  if (rr_nv_get_efs_data_ptr(as_id)->last_camped_cell_enabled)
  {
    write_efs_last_camped_cell(as_id);
  }

  return;
}
#ifdef FEATURE_GERAN_ADD_LAST_SCELL_CSFB_OPT
boolean rr_nv_add_last_serv_cell_csfb_is_plmn_match(sys_plmn_id_s_type plmn, const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;    
  }

  return (rr_nv_check_plmn_in_whitelist(efs_data_ptr->add_last_serv_cell_csfb_whitelist.plmn_list_ptr,
                                        efs_data_ptr->add_last_serv_cell_csfb_whitelist.num_of_plmns,
                                        plmn));
}
#endif /*FEATURE_GERAN_ADD_LAST_SCELL_CSFB_OPT*/

/*!
 * \brief Indicates if camping on an emergency call is allowed ahead of normal cells, for special cases.
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_camp_for_emergency_call_enabled(const gas_id_t gas_id)
{
  return rr_nv_get_efs_data_ptr(RR_GAS_ID_TO_AS_ID)->camp_for_emergency_call_enabled;
}

/*!
 * \brief Indicates if camping on a cell which supports GPRS but not tranmsitting SI13 l is allowed or not
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_cs_camp_when_cell_supports_ps_but_no_si13_rcvd(const gas_id_t gas_id)
{
  return rr_nv_get_efs_data_ptr(RR_GAS_ID_TO_AS_ID)->cs_camping_when_no_si13;
}

/*!
 * \brief Indicates if using common priority when BCCH configured cell are missing dedicated priorty and dedicated priorty being valid is allowed or not
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_use_bcch_priority_when_dedicated_priorty_is_missing(const gas_id_t gas_id)
{
  return rr_nv_get_efs_data_ptr(RR_GAS_ID_TO_AS_ID)->use_bcch_priority;
}

/*!
 * \brief Indicates if camping on a cell which indicates SI2bis or SI2ter support but never broadcast
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_camp_when_SI2bis_SI2ter_is_indicated_but_not_sent(const gas_id_t gas_id)
{
  return rr_nv_get_efs_data_ptr(RR_GAS_ID_TO_AS_ID)->camp_when_no_si2ter_si2bis;
}

boolean rr_nv_revert_to_rank_base_if_plmn_match(sys_plmn_id_s_type plmn, const gas_id_t gas_id)
{
  sys_modem_as_id_e_type as_id = RR_GAS_ID_TO_AS_ID;
  rr_efs_nv_data_t *efs_data_ptr = rr_nv_get_efs_data_ptr(as_id);

  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;    
  }

  return (rr_nv_check_plmn_in_whitelist(efs_data_ptr->revert_to_rank_base.plmn_list_ptr,
                                        efs_data_ptr->revert_to_rank_base.num_of_plmns,
                                        plmn));
}

/*!
 * \brief whether slow irat mode is enable or not.
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_get_slow_irat_mode_is_enabled(const gas_id_t gas_id)
{
  return rr_nv_get_efs_data_ptr(RR_GAS_ID_TO_AS_ID)->slow_irat_mode_enabled;
}

/*!
 * \brief Read the EFS-NV items.to check prioritize lte cell is enabled or not
 */
boolean rr_nv_increase_lte_cell_priority_enabled(const sys_modem_as_id_e_type as_id)
{

  /* GCF Validation */
  if (TRUE == mcfg_gcf_nv_get_status())
  {
    return FALSE;
  }

  return (rr_nv_get_efs_data_ptr(as_id)->rr_efs_increase_lte_cell_priority_enabled);
}

/*!
 * \brief Read the EFS-NV items.prioritize lte cell info
 */
rr_efs_increase_lte_cell_priority_data_t* rr_nv_get_increase_lte_cell_priority_data(const sys_modem_as_id_e_type as_id)
{
  return (&rr_nv_get_efs_data_ptr(as_id)->rr_efs_increase_lte_cell_priority_data);
}

#ifdef FEATURE_PRIORITIZE_LTE_FREQ
/*!
 * \brief Read the EFS-NV items.to check prioritize lte meas is enabled or not
 */
boolean rr_nv_prioritize_lte_meas_enabled(const sys_modem_as_id_e_type as_id)
{
  return (rr_nv_get_efs_data_ptr(as_id)->prioritize_lte_meas_enabled);
}

/*!
 * \brief Read the EFS-NV items.prioritize lte meas info
 */
rr_efs_prioritize_lte_meas_info_t* rr_nv_get_prioritize_lte_meas_info(const sys_modem_as_id_e_type as_id)
{
  return (&rr_nv_get_efs_data_ptr(as_id)->prioritize_lte_meas_info);
}
#endif /* FEATURE_PRIORITIZE_LTE_FREQ */

#ifdef FEATURE_GSM_DECODE_ALL_FREQS
/*!
 * \brief Indicates if the decode_all_bcch_freqs EFS is enabled.
 * 
 * \param as_id (in) - AS-ID (Note: Not GAS-ID)
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean rr_nv_decode_all_bcch_freqs_enabled(const sys_modem_as_id_e_type as_id)
{
  return rr_nv_get_efs_data_ptr(as_id)->decode_all_bcch_freqs;
}
#endif //FEATURE_GSM_DECODE_ALL_FREQS

#ifdef PERLUTF
#error code not present
#endif // PERLUTF

/* EOF */

