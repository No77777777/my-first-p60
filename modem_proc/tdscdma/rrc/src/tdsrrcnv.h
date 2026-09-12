#ifndef TDSRRCNV_H
#define TDSRRCNV_H
/*===========================================================================
                  R R C   N V   M O D U L E

DESCRIPTION
  This header file contains function prototypes for the RRC NV routines.
  These routines have been created to read and write RRC information into the NV.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcnv.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/27/12   xp      Add NV for CCCH message filter.
03/14/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.93.50
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
03/07/11   su      Added code to support device type NV.
02/07/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.90.50
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
11/24/10   yzh     Add ((FEATURE_TDSCDMA_64QAM) || (FEATURE_TDSCDMA_MIMO)) in tdsrrc_macehs_cat
                   which set value in FEATURE_TDSCDMA_64QAM or FEATURE_TDSCDMA_MIMO.

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include <string.h>
#include "tdsrrccspdb.h"
#include "err.h"
#include "nv.h"
#include "tdsrrccmd_v.h"

#include "mcfg_nv_api.h" 

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDS_RRC_NV_FILE_NAME_LEN_MAX  40
#define TDS_RRC_NV_FULL_PATH_LEN_MAX  100
#define RRC_NV_NUM_FORBID_CELLS 20

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* data type stored in EFS */
typedef struct
{
  sys_sys_mode_e_type rat;
  uint32 cell_id;
  uint32 lac;
  sys_plmn_id_s_type  irat_sys_plmn_id;
} tdsrrc_efs_forbid_cell_type;

/* data type for RRC internal */
typedef struct
{
  sys_plmn_id_s_type sys_plmn_id;
  uint32 cell_id;
  uint32 lac;
} tdsrrcnv_forbid_cell_type;

/* data type for RRC internal */
typedef struct
{
  uint32 num_forbid_cell;
  tdsrrcnv_forbid_cell_type forbid_cell[RRC_NV_NUM_FORBID_CELLS];
} tdsrrcnv_forbid_cells_type;

/* could add new NV/EFS item in below structure in future. */
typedef struct{
  tdsrrcnv_forbid_cells_type forbid_cells;
} tdsrrcnv_data_type;

/* Use EFS for TDS RRC NV items */
typedef enum {
   TDS_RRC_NV_VERSION,                           /* uint32                                    */
   TDS_RRC_NV_ACQ_DB,                            /* tds_nv_acq_db_type                        */
   TDS_RRC_NV_ACQ_LIST,                          /* tds_nv_acq_entry_type[MAX_ACQ_DB_ENTRIES] */
   TDS_RRC_NV_RRC_INTEGRITY_ENABLED,             /* boolean                                   */
   TDS_RRC_NV_RRC_CIPHERING_ENABLED,             /* boolean                                   */
   TDS_RRC_NV_RRC_FAKE_SECURITY_ENABLED,         /* boolean                                   */
   TDS_RRC_NV_SPECIAL_FREQ_ENABLED,              /* boolean                                   */
   TDS_RRC_NV_SPECIAL_FREQ,                      /* uint32                                    */
   TDS_RRC_NV_RRC_PDCP_DISABLED,                 /* boolean                                   */
   TDS_RRC_NV_RRC_VERSION,                       /* uint32                                    */
   TDS_RRC_NV_HSDPA_CAT,                         /* uint8                                     */
   TDS_RRC_NV_HSUPA_CAT,                         /* uint8                                     */
   TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST,         /* uint32                                    */
   TDS_RRC_NV_CS_VOICE_OVER_HSPA_ENABLED,        /* boolean                                   */
   TDS_RRC_NV_GERAN_FEATURE_PACK,                /* uint8                                     */
   TDS_RRC_NV_INTERRAT_NACC_SUPPORT,             /* uint8                                     */
   TDS_RRC_NV_CFA_CONF,                          /* uint32                                    */
   TDS_RRC_NV_IGNORE_CELL_BAR_RESERVE_STATUS,    /* uint8                                     */
   TDS_RRC_NV_SNOW3G_ENABLED,                    /* boolean                                   */
   TDS_RRC_NV_SET_SRB2_ACT_TIME,                 /* uint8                                     */
   TDS_RRC_NV_CSFB_SKIP_SIB11_OPT,               /* uint8                                     */
   TDS_RRC_NV_FREQ_LOCK,                         /* uint16                                    */
   TDS_RRC_NV_DO_NOT_FORWARD_PAGE,               /* uint8                                     */
   TDS_RRC_NV_A2_POWER_OPT,                      /* uint8                                     */
   TDS_RRC_NV_FAST_RET_TO_LTE_AFTER_CSFB,        /* boolean                                   */
   TDS_RRC_NV_FAST_RET_TO_LTE_AFTER_CSFB_TIMER,  /* uint16                                    */
   TDS_RRC_NV_SPECIAL_SETTINGS_FOR_TESTING,      /* uint8                                     */
   TDS_RRC_NV_PRIM_FREQ_LIST,                    /* uint16                                    */
   TDS_RRC_NV_INTERRAT_FEATURE_CTRL,             /* uint32                                    */
   TDS_RRC_NV_TDS_L2_OPT_FEATURE_LIST,           /* uint32                                    */
   TDS_RRC_NV_DELAY_OOS_IND_TIMER,               /* uint8                                     */
   TDS_RRC_NV_TDS_OPTIONAL_UECI_LIST,            /* uint32                                    */
   TDS_RRC_NV_SIB_SLEEP_BEFORE_SB,               /* tds_nv_sib_sleep                          */
   TDS_RRC_NV_SIB7_EXP_TIME_FACTOR,              /* uint8                                     */
   TDS_RRC_NV_SILENT_REDIAL_OPT,                 /* boolean                                   */
   TDS_RRC_NV_BAND_SEARCH_MASK,                  /* uint16                                    */
   TDS_RRC_NV_FREQ_CELL_LOCK,                    /* tds_nv_lock_freq_cpid_type                */
   TDS_RRC_NV_RLC_REESTABLISH,                   /* uint8                                      */
   TDS_RRC_NV_BPLMN_IGNOR_SIB19,                 /* boolean                                   */
   TDS_RRC_NV_PSEUDO_SIB19,                      /* tds_nv_pseudo_sib19_type                  */
   TDS_RRC_NV_FLOW_CONTROL_PARAMETERS,           /* tdsnv_flow_control_value_type   */
   TDS_RRC_NV_L2T_ACQDB_CNT_THRESHOLD_REDIRECTION, /* uint8                                     */
   TDS_RRC_NV_HOLD_PS_TMR_VALUE,                 /* uint8                                     */
   TDS_RRC_NV_T2L_PING_PONG_REDIR_BAR_TIME, /* uint8,FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT*/
   TDS_RRC_NV_ENABLE_AUDIO_SCR_MODE,             /* boolean                                   */
   TDS_RRC_NV_EST_FAIL_BAR_CELL,                 /*tds_nv_est_fail_bar_cell_type              */
   TDS_RRC_NV_CS_FAIL_BAR_TIME,                  /*uint16*/
   /* Keep this as the last element */
   TDS_RRC_NV_MAX
} tds_rrc_nv_idx_type;

/* NV items under this array are not belong to RRC but RRC read them locally. */
typedef enum {
   TDS_RRC_NON_RRC_NV_MMCP_ENABLE_THIN_UI_CONFIG,        /* boolean */
   
   /* Keep this as the last element */
   TDS_RRC_NON_RRC_NV_MAX
} tds_rrc_non_rrc_nv_idx_type;

typedef PACKED struct PACKED_POST{ 
  /* Next Acquisition pointer to be read */
  uint32                                           curr_rd_index;
  /* Next Acquisition pointer to write */
  uint32                                           curr_wr_index;
  /* Indicates if the last entry in database pointer list is Invalid */
  boolean                                          last_entry_invalid;
  /* List of ordered indices to acquisition entries */
  uint8                                            acq_list_indices[TDS_MAX_ACQ_DB_NV_ENTRIES];
} tds_nv_acq_db_type;

typedef PACKED struct PACKED_POST{
  uint8                                            version; 
  tdsrrc_csp_lock_mode                             lock_mode;
  uint16                                           lock_freq;
  uint8                                            lock_cpid;
  uint16                                           reserved;
} tds_nv_lock_freq_cell_type;

typedef PACKED struct PACKED_POST{ 
  /* PLMN ID */
  nv_rrc_plmn_identity_type                        plmn_id;
  /* Frequency where PLMN is present */
  uint32                                           freq;
  /* Band Class */
  uint32                                           band_class;
  /* Type of this stored acquisition entry */
  uint32                                           status_mask;
  /* Current Cell ID selected */
  uint32                                           cell_id;
  /* Cell Parameters ID of the Cell */
  uint32                                           cpid;
  /* Location Area Code of the cell */
  uint32                                           lac;
} tds_nv_acq_entry_type; /* Total MAX_ACQ_DB_ENTRIES in TDS_RRC_NV_ACQ_LIST */

typedef PACKED struct PACKED_POST{ 
  /* Flag to enable this SIB sleep feature */
  uint16                                          sib_sleep_flag;
  /* Repetition length in frames (multiple of 8) */
  uint16                                          repetition_len;
  /* End of SIB frame in one repetition cycle */
  uint16                                          sib_end_frame;
  /* Reserved for future use */
  uint16                                          misc_data;
} tds_nv_sib_sleep; /* TDS_RRC_NV_SIB_SLEEP_BEFORE_SB */

typedef PACKED struct PACKED_POST{ 
  boolean       is_pseudo_sib19_enabled;
  
  /* UTRA TDD serving */
  uint8         utra_priority;
  uint8         s_priority_search1;
  uint8         thresh_serving_low;
  
  /* EUTRA Neighbors */
  uint8         eutra_priority;
  uint8         eutra_meas_bandwidth;
  uint8         eutra_thresh_high;
  uint8         eutra_thresh_low;
  int8          eutra_qrxlevmin;

  uint32        reserved;
} tds_nv_pseudo_sib19_type;


typedef PACKED struct PACKED_POST{
	uint16	tdsrlc_fc_min_win_size;  //Minimum value of the win size the FC can use for TX win size.
	uint16 tdsrlc_fc_num_of_steps; //Value of number of increments to be made to reach max configured win size.
	uint8 tdsfw_dl_fc_rtbs_value[8]; //Fw RTBS values for DL FC steps
} tdsnv_flow_control_value_type;

typedef PACKED struct PACKED_POST{
  /* Indicate if the bar cell is enabled or not */
  boolean                                          bar_cell_enable;
  /* threshold value used for bar cell, unit dbm */
  int16                                            bar_cell_rscp_threshold; 
  /* cell bar timer, unit second */
  uint16                                           bar_cell_n300_failure_timer;
} tds_nv_est_fail_bar_cell_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/* variable to indicate support of feature list supported.
 * This variable will be set using the value of NV item 'NV_WCDMA_OPTIONAL_FEATURE_LIST_I'
 * TDS_NV_OPTIONAL_FEATURE_HSDPA 0x1<<1 
 * TDS_NV_OPTIONAL_FEATURE_HSUPA 0x1<<2
 * TDS_NV_OPTIONAL_FEATURE_MBMS 0x1<<3
 * TDS_NV_OPTIONAL_FEATURE_RX_DIVERSITY 0x1<<4
 * TDS_NV_OPTIONAL_FEATURE_PS_DATA_HO 0x1<<5
 * all other bits are spare bits - for future use
 */

extern tdsrrcnv_data_type tdsrrcnv_data_sim[2];
#define tdsrrcnv_data tdsrrcnv_data_sim[tdsrrc_get_as_id()]

extern uint32 tds_nv_optional_feature_list;
extern uint32 tds_nv_l2_opt_feature_list;
extern uint32 tds_nv_interrat_feature_ctrl;
extern uint8  tds_nv_delay_oos_ind_timer;
extern uint8  tds_nv_sib7_exp_time_factor;
extern tds_nv_est_fail_bar_cell_type tdsrrcrce_nv_est_fail_bar_cell;

#ifdef FEATURE_TDSCDMA_HSUPA

/* variable to store the hsupa category.
 * This variable will be set using the value of NV item 'NV_WCDMA_HSUPA_CATEGORY_I'
 */
extern uint16 tds_nv_hsupa_category;

#endif

extern uint8 tdsnv_hsdpa_category;
extern uint8 tdsrrc_machs_cat;
#ifdef FEATURE_TDSCDMA_REL8
/*#if defined(FEATURE_TDSCDMA_64QAM) || defined(FEATURE_TDSCDMA_MIMO)*/
/*temporary solution according to tdsrrcueci.c*/
extern uint8 tdsrrc_macehs_cat;
/*#endif*/
#endif /*FEATURE_TDSCDMA_REL8*/
extern uint8 tdsrrc_device_type_nv;
extern uint8 tdsrrc_ignore_cell_bar_nv;

#ifdef FEATURE_3GPP_CSFB_SKIP_OPT_TDSCDMA_SIBS
extern uint8 tdsrrc_csfb_skip_sib11_opt_nv;
#endif


extern uint8 tdsrrc_do_not_forward_page_nv;

extern uint8 tdsrrc_set_srb2_act_time_nv;

extern boolean tdsrrc_fast_return_to_lte_after_csfb_nv;

extern uint16 tdsrrc_fast_return_to_lte_after_csfb_timer_nv;

/* variable to indicate support of ueci list supported.
 * This variable will be set using the value of NV item 'NV_WCDMA_OPTIONAL_FEATURE_LIST_I'
 * TDS_NV_OPTIONAL_UECI_THIN_UI 0x1<<1
 * all other bits are spare bits - for future use
 */
extern uint32 tds_nv_optional_ueci_list;

extern boolean tds_nv_silent_redial_opt;

/* the NV value of band_search_mask
 *  bit 0 - Band A
 *  bit 1 - N/A
 *  bit 2 - N/A
 *  bit 3 - N/A
 *  bit 4 - Band E
 *  bit 5 - Band F
 *  bit 6~15 - N/A
 */
extern uint16 tdsrrc_nv_band_search_mask;
#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
extern uint8 tdsrrc_rlc_reestablish_nv;
#endif

extern boolean tdsrrc_bplmn_ignore_sib19_nv;

extern tds_rrc_config_e_type tdsrrc_active_config;
//Global NV for CPU based Flow control
extern tdsnv_flow_control_value_type tdscdma_flow_control_nv;

extern uint8  tdsrrc_nv_l2t_acqdb_cnt_threshold_redirection;
extern uint8  tdsrrc_nv_hold_ps_tmr_value;

#ifdef FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT
extern uint8 tdsrrc_nv_t2l_ping_pang_redir_bar_time;
#endif

extern boolean tdsrrc_nv_enable_audio_scr_mode;

/* =======================================================================
**                          Macro Definitions
** ======================================================================= */
#if (defined(FEATURE_TDSCDMA_REL8) || defined(FEATURE_TDSCDMA_REL9)) && \
     defined(FEATURE_TDSCDMA_64QAM)
#define TDSRRC_HSDPA_DEFAULT_CATEGORY 24
#else
#define TDSRRC_HSDPA_DEFAULT_CATEGORY 15
#endif

#ifdef FEATURE_TDSCDMA_HSUPA
#define TDSRRC_TDS_HSUPA_DEFAULT_CATEGORY 6
#endif /* FEATURE_TDSCDMA_HSUPA */

#define TDS_NV_OPTIONAL_FEATURE_ALL_SUPPORTED ~0

#define TDS_NV_OPTIONAL_FEATURE_DEFAULT_VAL   0xBF5EFFBF

#define TDS_NV_OPTIONAL_FEATURE_DEFAULT_VAL_FOR_SPECIAL_TEST  0x1F5FFDFF

#define TDS_NV_OPTIONAL_FEATURE_NONE_SUPPORTED 0

/* HSDPA: Bit 1 <uint32: bit31..bit0> */
#define TDS_NV_OPTIONAL_FEATURE_HSDPA 0x1<<1 

/* HSUPA: Bit 2 <uint32: bit31..bit0> */
#define TDS_NV_OPTIONAL_FEATURE_HSUPA 0x1<<2

/* MBMS: Bit 3 <uint32: bit31..bit0> */
#define TDS_NV_OPTIONAL_FEATURE_MBMS 0x1<<3

/* RX DIVERSITY: Bit 4 <uint32: bit31..bit0> */
#define TDS_NV_OPTIONAL_FEATURE_RX_DIVERSITY 0x1<<4

/* PS DATA HO: Bit 5 <uint32: bit31..bit0> */
#define TDS_NV_OPTIONAL_FEATURE_PS_DATA_HO 0x1<<5
/* Disable FACH IRAT reselection under CS: Bit 9 <uint32: bit31..bit0> */
#define TDS_NV_OPTIONAL_FEATURE_DISABLE_FACH_IRAT_RESEL_CS_BIT (((uint32) 0x1)<<9)

/* ACQ DB auto-save : Bit 7 <uint32: bit31..bit0> */
#define TDS_NV_OPTIONAL_FEATURE_ACQ_DB_AUTO_SAVE_BIT ((uint32) 0x1)<<7

#if 0
#define TDS_NV_OPTIONAL_FEATURE_BPLMN_SIB_FAIL_OPT_BIT  ((uint32) 0x1)<<15
#endif
/* For Band E subdivisions E1, E2 and E3 */
#define TDS_NV_OPTIONAL_FEATURE_SUBBAND_E1_BIT  ((uint32) 0x1)<<17
#define TDS_NV_OPTIONAL_FEATURE_SUBBAND_E2_BIT  ((uint32) 0x1)<<18
#define TDS_NV_OPTIONAL_FEATURE_SUBBAND_E3_BIT  ((uint32) 0x1)<<19

/* For Band F acquisition modes and search patterns control */
#define TDS_NV_OPTIONAL_FEATURE_LOWER_F_SHALLOW ((uint32) 0x1)<<20
#define TDS_NV_OPTIONAL_FEATURE_LOWER_F_DEEP    ((uint32) 0x1)<<21
#define TDS_NV_OPTIONAL_FEATURE_UPPER_F_SHALLOW ((uint32) 0x1)<<22
#define TDS_NV_OPTIONAL_FEATURE_UPPER_F_DEEP    ((uint32) 0x1)<<23
/* For Band A acquisition modes and search patterns control */
#define TDS_NV_OPTIONAL_FEATURE_A_SHALLOW ((uint32) 0x1)<<3
#define TDS_NV_OPTIONAL_FEATURE_A_DEEP    ((uint32) 0x1)<<4
/* For Band E acquisition modes and search patterns control */
#define TDS_NV_OPTIONAL_FEATURE_E_SHALLOW ((uint32) 0x1)<<5
#define TDS_NV_OPTIONAL_FEATURE_E_DEEP    ((uint32) 0x1)<<6

#define TDS_NV_OPTIONAL_FEATURE_CMCC_RLF_SKIP_SVCREQ_BIT  ((uint32) 0x1)<<29

#define TDS_NV_OPTIONAL_FEATURE_ENABLE_RRC_FREQ_PRUNING_BY_ACQ_CNF_BIT  ((uint32) 0x1)<<8
#define TDS_NV_OPTIONAL_FEATURE_ENABLE_RRC_FREQ_PRUNING_BIT  ((uint32) 0x1)<<31

/* LTE to TDS PSHO NV option bitmask */
#define TDS_NV_INTERRAT_FEATURE_L2TD_PSHO  ((uint32) 0x1)
/* TDS to LTE PSHO NV option bitmask */
#define TDS_NV_INTERRAT_FEATURE_TD2L_PSHO  ((uint32) 0x1)<<1
/* IDLE INTERVAL NV option bitmask */
#define TDS_NV_INTERRAT_FEATURE_IDLEINTERVAL  ((uint32) 0x1)<<2
/* DMO NV option bitmask */
#define TDS_NV_INTERRAT_FEATURE_DMO           ((uint32) 0x1)<<3

/* Default value needs to be enabled on NI.6.0 with FEATURE_MPSS_NI_6_0_0 */
/* Disable LTE to TDS PSHO NV option bitmask */
#define TDS_NV_INTERRAT_FEATURE_DISABLE_L2TD_PSHO  ((uint32) 0x1)<<4
/* Disable TDS to LTE PSHO NV option bitmask */
#define TDS_NV_INTERRAT_FEATURE_DISABLE_TD2L_PSHO  ((uint32) 0x1)<<5
/* Disable IDLE INTERVAL NV option bitmask */
#define TDS_NV_INTERRAT_FEATURE_DISABLE_IDLEINTERVAL  ((uint32) 0x1)<<6
/* Disable DMO NV option bitmask */
#define TDS_NV_INTERRAT_FEATURE_DISABLE_DMO           ((uint32) 0x1)<<7

/* T2L conditional ignore DMO NV option bitmask */
#define TDS_NV_INTERRAT_FEATURE_CONDITIONAL_IGNORE_DMO ((uint32) 0x1)<<8

/* Ignore CSFB Indication in rrcConnectionRequest NV option bitmask */
#define TDS_NV_INTERRAT_FEATURE_IGNORE_CSFB_INDICATION ((uint32) 0x1)<<9

#define TDS_NV_DEFAULT_SIB7_EXP_TIME_FACTOR 1

#define TDSCHECK_WCDMA_OPTIONAL_FEATURE_HSUPA_SUPPORTED (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_HSUPA)

#define TDSCHECK_WCDMA_OPTIONAL_FEATURE_HSDPA_SUPPORTED (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_HSDPA)
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
#define TDSCHECK_WCDMA_OPTIONAL_FEATURE_MACEHS_SUPPORTED (tdsnv_hsdpa_category > 12 )
#endif

#ifdef FEATURE_TDSCDMA_MIMO
#define TDSCHECK_WCDMA_OPTIONAL_FEATURE_MIMO_SUPPORTED (tdsnv_hsdpa_category >= 15 )
#endif

#ifdef FEATURE_TDS_DC_HSDPA
#define TDSCHECK_WCDMA_OPTIONAL_FEATURE_DC_HSDPA_SUPPORTED ((tdsnv_hsdpa_category >= 21) && (tdsnv_hsdpa_category <= 24))
#endif /*FEATURE_TDS_DC_HSDPA*/

#ifdef FEATURE_TDSCDMA_64QAM
/*64QAM is not supported if NV is 15 or 16 . It is supported for HS cat 13, 14, 17,18*/
/*With the introduction of DC-HSDPA, 64QAM isn't supported for CAT 21 and 22 either.*/
#define TDSCHECK_WCDMA_OPTIONAL_FEATURE_64QAM_SUPPORTED ((tdsnv_hsdpa_category >= 13) && \
  ((tdsnv_hsdpa_category != 15) && (tdsnv_hsdpa_category != 16) && (tdsnv_hsdpa_category != 21) && (tdsnv_hsdpa_category != 22)))
#endif /*FEATURE_TDSCDMA_64QAM*/

/* MSB 0000 000X 0000 0000, ... LSB */
#define TDSCDMA_NV_OPTIONAL_FEATURE_MTNET_SUPPORT 0x800000
#define TDSCHECK_OPTIONAL_OPTIONAL_FEATURE_MTNET_SUPPORT (tds_nv_optional_feature_list & TDSCDMA_NV_OPTIONAL_FEATURE_MTNET_SUPPORT)

/* CFA support NV bit is controlled by the 8th bit from MSB in TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST */
/* MSB 0000 000X 0000 0000, ... LSB */
#define TDSCDMA_NV_OPTIONAL_FEATURE_CFA_SUPPORTED 0x1000000
#define TDSCDMA_NV_OPTIONAL_FEATURE_CFA_NOT_SUPPORTED ~0x1000000
#define TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED (tds_nv_optional_feature_list & TDSCDMA_NV_OPTIONAL_FEATURE_CFA_SUPPORTED)

/* Assert L1 Error Indication support NV bit is controlled by the 8th bit from MSB in TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST */
/* MSB 0000 000X 0000 0000, ... LSB */
#define TDSCDMA_NV_OPTIONAL_FEATURE_ASSERT_L1_ERR_IND 0x2000000
#define TDSCHECK_OPTIONAL_FEATURE_ASSERT_L1_ERR_IND (tds_nv_optional_feature_list & TDSCDMA_NV_OPTIONAL_FEATURE_ASSERT_L1_ERR_IND)

/*0x8000000 is reserved for filtering out CCCH mis-matched OTA messages*/
#define TDSCDMA_NV_OPTIONAL_FEATURE_FILTER_CCCH_MSG 0x8000000
#define TDSCHECK_OPTIONAL_FEATURE_FILTER_CCCH_MSG (tds_nv_optional_feature_list & TDSCDMA_NV_OPTIONAL_FEATURE_FILTER_CCCH_MSG)

/*0x10000000 is reserved for enable/diable Pseuo Fast Return feature*/
#define TDSCDMA_NV_OPTIONAL_FEATURE_ENABLE_PFR 0x10000000
#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_PFR (tds_nv_optional_feature_list & TDSCDMA_NV_OPTIONAL_FEATURE_ENABLE_PFR)

/* Band E subdivisions */
#define TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E1_SET (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_SUBBAND_E1_BIT)
#define TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E2_SET (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_SUBBAND_E2_BIT)
#define TDSCHECK_OPTIONAL_FEATURE_SUBBAND_E3_SET (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_SUBBAND_E3_BIT)

/* Band F search patterns */
#define TDSCHECK_OPTIONAL_FEATURE_LOWER_F_SHALOW_SET (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_LOWER_F_SHALLOW)
#define TDSCHECK_OPTIONAL_FEATURE_LOWER_F_DEEP_SET   (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_LOWER_F_DEEP)
#define TDSCHECK_OPTIONAL_FEATURE_UPPER_F_SHALOW_SET (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_UPPER_F_SHALLOW)
#define TDSCHECK_OPTIONAL_FEATURE_UPPER_F_DEEP_SET   (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_UPPER_F_DEEP)

/* Band A search patterns */
#define TDSCHECK_OPTIONAL_FEATURE_A_SHALOW_SET (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_A_SHALLOW)
#define TDSCHECK_OPTIONAL_FEATURE_A_DEEP_SET   (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_A_DEEP)

/* Band E search patterns */
#define TDSCHECK_OPTIONAL_FEATURE_E_SHALOW_SET (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_E_SHALLOW)
#define TDSCHECK_OPTIONAL_FEATURE_E_DEEP_SET   (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_E_DEEP)

#define TDSCHECK_OPTIONAL_FEATURE_CMCC_RLF_SKIP_SVCREQ (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_CMCC_RLF_SKIP_SVCREQ_BIT)

#ifndef FEATURE_ENABLE_PSHO_IDLEINTERVAL_DMO_NV_BY_DEFAULT
/* 0x 00000001 is L2TD PSHO NV option */
#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_L2TD_PSHO (tds_nv_interrat_feature_ctrl & TDS_NV_INTERRAT_FEATURE_L2TD_PSHO)
/* 0x 00000002 is TD2L PSHO NV option */
#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_TD2L_PSHO (tds_nv_interrat_feature_ctrl & TDS_NV_INTERRAT_FEATURE_TD2L_PSHO)
/* 0x 00000004 is IDLE INTERVAL NV option */
#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL (tds_nv_interrat_feature_ctrl & TDS_NV_INTERRAT_FEATURE_IDLEINTERVAL)
/* 0x 00000008 is DMO NV option */
#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO          (tds_nv_interrat_feature_ctrl & TDS_NV_INTERRAT_FEATURE_DMO)
#else
/* 0x 00000010 is Disable L2TD PSHO NV option */
#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_L2TD_PSHO (!(tds_nv_interrat_feature_ctrl & TDS_NV_INTERRAT_FEATURE_DISABLE_L2TD_PSHO))
/* 0x 00000020 is Disable TD2L PSHO NV option */
#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_TD2L_PSHO (!(tds_nv_interrat_feature_ctrl & TDS_NV_INTERRAT_FEATURE_DISABLE_TD2L_PSHO))
/* 0x 00000040 is Disable IDLE INTERVAL NV option */
#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL (!(tds_nv_interrat_feature_ctrl & TDS_NV_INTERRAT_FEATURE_DISABLE_IDLEINTERVAL))
/* 0x 00000080 is Disable DMO NV option */
#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO          (!(tds_nv_interrat_feature_ctrl & TDS_NV_INTERRAT_FEATURE_DISABLE_DMO))
#endif

/* 0x 00000100 is conditional ignore DMO NV option. This NV bit is checked only when
   FEATURE_TDSCDMA_REL9_NO_T2L_CONN_MEAS is defined.
   In tds_nv_interrat_feature_ctrl, if this bit set to:
   0 - Means ignore all DMO configuration.
   1 - Means ignore DMO configuration only when all statusFlag are deactivate.
       Otherwise return failure.
*/
#define TDSCHECK_OPTIONAL_FEATURE_CONDITIONAL_IGNORE_DMO (tds_nv_interrat_feature_ctrl & TDS_NV_INTERRAT_FEATURE_CONDITIONAL_IGNORE_DMO)

#define TDSCHECK_OPTIONAL_FEATURE_IGNORE_CSFB_INDICATION (tds_nv_interrat_feature_ctrl & TDS_NV_INTERRAT_FEATURE_IGNORE_CSFB_INDICATION)

#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_RRC_FREQ_PRUNING_BY_ACQ_CNF (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_ENABLE_RRC_FREQ_PRUNING_BY_ACQ_CNF_BIT)

#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_RRC_FREQ_PRUNING (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_ENABLE_RRC_FREQ_PRUNING_BIT)
#if 0
#define TDS_NV_OPTIONAL_FEATURE_BPLMN_SIB_FAIL_OPT (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_BPLMN_SIB_FAIL_OPT_BIT)
#endif
#define TDSCHECK_OPTIONAL_FEATURE_DISABLE_FACH_IRAT_RESEL_CS (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_DISABLE_FACH_IRAT_RESEL_CS_BIT)

#define TDSCHECK_OPTIONAL_FEATURE_ENABLE_ACQ_DB_AUTO_SAVE (tds_nv_optional_feature_list & TDS_NV_OPTIONAL_FEATURE_ACQ_DB_AUTO_SAVE_BIT)

#define TDS_NV_OPTIONAL_UECI_ALL_SUPPORTED ~0
#define TDS_NV_OPTIONAL_UECI_NONE_SUPPORTED 0

/* Bit0 in TDS_RRC_NV_TDS_OPTIONAL_UECI_LIST is reserved, NV starts from bit1 */
/* THIN_UI: Bit 1 <uint32: bit31..bit0> */
#define TDS_NV_OPTIONAL_UECI_DEVICE_TYPE  ((uint32) 0x1)<<1
#define TDSCHECK_OPTIONAL_UECI_ENABLE_DEVICE_TYPE          (tds_nv_optional_ueci_list & TDS_NV_OPTIONAL_UECI_DEVICE_TYPE)

/* MST needs to test QTA in loopback mode for performace comparison. This functionality is not supported by design. */
#define TDS_NV_OPTIONAL_UECI_LB_QTA  ((uint32) 0x1)<<2
#define TDSCHECK_OPTIONAL_UECI_ENABLE_LB_QTA               (tds_nv_optional_ueci_list & TDS_NV_OPTIONAL_UECI_LB_QTA)
/* for trace level, TRUE: enable, FALSE: disable */
#define TDS_NV_OPTIONAL_UECI_L3_TRACE_HIGH  ((uint32) 0x1)<<4
#define TDSCHECK_OPTIONAL_UECI_ENABLE_L3_TRACE_HIGH        (tds_nv_optional_ueci_list & TDS_NV_OPTIONAL_UECI_L3_TRACE_HIGH)

#define TDS_NV_OPTIONAL_UECI_L3_TRACE_MED  ((uint32) 0x1)<<5
#define TDSCHECK_OPTIONAL_UECI_ENABLE_L3_TRACE_MED        (tds_nv_optional_ueci_list & TDS_NV_OPTIONAL_UECI_L3_TRACE_MED)

#define TDS_NV_OPTIONAL_UECI_L2_TRACE_HIGH  ((uint32) 0x1)<<6
#define TDSCHECK_OPTIONAL_UECI_ENABLE_L2_TRACE_HIGH        (tds_nv_optional_ueci_list & TDS_NV_OPTIONAL_UECI_L2_TRACE_HIGH)

#define TDS_NV_OPTIONAL_UECI_L2_TRACE_MED  ((uint32) 0x1)<<7
#define TDSCHECK_OPTIONAL_UECI_ENABLE_L2_TRACE_MED        (tds_nv_optional_ueci_list & TDS_NV_OPTIONAL_UECI_L2_TRACE_MED)

#define TDS_NV_TDSCDMA_RESELECTION_PERMIT_CHECK_USE_NAS_API 0x00300000
#define TDS_NV_TDSCDMA_RESELECTION_PERMIT_CHECK_USE_NAS_API_ENABLE     ~(tds_nv_optional_ueci_list & TDS_NV_TDSCDMA_RESELECTION_PERMIT_CHECK_USE_NAS_API)

#define TDS_NV_OPTIONAL_SRNS_RESELECTION_HOLD_7S  ((uint32) 0x1)<<20
#define TDS_NV_OPTIONAL_SRNS_RESELECTION_HOLD_7S_ENABLE          (tds_nv_optional_ueci_list & TDS_NV_OPTIONAL_SRNS_RESELECTION_HOLD_7S)

#define TDS_NV_OPTIONAL_SRNS_RESELECTION_HOLD_10S  ((uint32) 0x1)<<21
#define TDS_NV_OPTIONAL_SRNS_RESELECTION_HOLD_10S_ENABLE          (tds_nv_optional_ueci_list & TDS_NV_OPTIONAL_SRNS_RESELECTION_HOLD_10S)

#ifdef FEATURE_TDSCDMA_SIB19
#define TDS_NV_OPTIONAL_IGNORE_SIB19_DECODE_FAIL  ((uint32) 0x1)<<22
#define TDS_NV_OPTIONAL_IGNORE_SIB19_DECODE_FAIL_ENABLE          (tds_nv_optional_ueci_list & TDS_NV_OPTIONAL_IGNORE_SIB19_DECODE_FAIL)
#endif

/*Define the RCE report RRC_FAILURE_DUE_TO_CONGESTION instead of RRC_MAX_ACCESS_FAILURE*/
/*The silent redial NV will not be active per CE's request*/
#define TDSCHECK_OPTIONAL_UECI_RCE_RPT_OPT tds_nv_silent_redial_opt
#ifdef FEATURE_TDSCDMA_SRB_RESET
#define TDS_NV_OPTIONAL_UECI_SRB_RESET  ((uint32) 0x1)<<31
#define TDS_NV_OPTIONAL_UECI_SRB_RESET_DISABLE  (tds_nv_optional_ueci_list & TDS_NV_OPTIONAL_UECI_SRB_RESET)
#endif
/* default support bitmask will be updated in the future */
#define TDS_NV_OPTIONAL_UECI_DEFAULT_SUPPORT_BITMASK TDS_NV_OPTIONAL_UECI_NONE_SUPPORTED

#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
#define TDS_NV_RLC_REESTABLISH_ALL_SUPPORTED ~0
#define TDS_NV_RLC_REESTABLISH_NONE_SUPPORTED 0

#define TDS_NV_DEFAULT_RLC_REESTABLISH_BITMASK ((TDS_NV_RLC_ALL_RB_REEST)|(TDS_NV_RLC_SRB_REEST))

#define TDS_NV_RLC_ALL_RB_REEST ((uint8)0x1)<<1
#define TDS_NV_RLC_SRB_REEST ((uint8)0x1)<<2

#define TDSCHECK_RLC_ALL_RB_REEST_ENABLE (tdsrrc_rlc_reestablish_nv & TDS_NV_RLC_ALL_RB_REEST)
#define TDSCHECK_RLC_SRB_REEST_ENABLE (tdsrrc_rlc_reestablish_nv & TDS_NV_RLC_SRB_REEST)
#endif

/* 0x50 indicates both L2/L3 HIGH level traces enabled and MED level traces disabled */
#define TDS_NV_OPTIONAL_UECI_L2L3_TRACE_ENABLE_BITMASK 0x50

/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/*===========================================================================

FUNCTION tdsrrcnv_init

DESCRIPTION
  This function checks the NV conf file and creates it if it doesn't exist.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_init(void);

/*===========================================================================

FUNCTION tdsrrcnv_efs_read

DESCRIPTION
  Read EFS item denoted by efs_id and copy to data_ptr.
  If not read successfully, substitute default values.

DEPENDENCIES
  None

RETURN VALUE
  Actual size read. 0 indicates an error.
  
  *size_ptr is set to the actual number of bytes that were copied into the
  buffer pointed by data_ptr. If the read failed, *size_ptr = 0

SIDE EFFECTS
  None.


==========================================================================*/
uint32 tdsrrcnv_efs_read
(
        tds_rrc_nv_idx_type          nv_item,
            /* passing the nv_item to read.
            */
        byte                        *data_ptr,
            /* Pointer to a data buffer where to store the data that is
            ** associated with the EFS item.
            */
        uint32                       size
            /* Size of the data_ptr buffer 
            */
);

/*===========================================================================

FUNCTION tdsrrcnv_efs_read_generic

DESCRIPTION
  Read EFS item denoted by efs_id (could be EFS item outside RRC)
  and copy to data_ptr.
  If not read successfully, substitute default values.

DEPENDENCIES
  None

RETURN VALUE
  Actual size read. 0 indicates an error.
  
  *size_ptr is set to the actual number of bytes that were copied into the
  buffer pointed by data_ptr. If the read failed, *size_ptr = 0

SIDE EFFECTS
  None.


==========================================================================*/
uint32 tdsrrcnv_efs_read_generic
(
        tds_rrc_non_rrc_nv_idx_type          nv_item,
            /* passing the nv_item to read.
            */
        byte                        *data_ptr,
            /* Pointer to a data buffer where to store the data that is
            ** associated with the EFS item.
            */
        uint32                       size,
            /* Size of the data_ptr buffer 
            */
        char                         *path_ptr
            /* Pointer to the NV item path
            */
);

/*===========================================================================

FUNCTION tdsrrcnv_efs_write

DESCRIPTION
  Write contents of data_ptr to EFS item denoted by nv_item

DEPENDENCIES
  None

RETURN VALUE
  TRUE: sucess
  FALSE: failure

SIDE EFFECTS
  None.

===========================================================================*/

boolean tdsrrcnv_efs_write
(
        tds_rrc_nv_idx_type          nv_item,
            /* passing the nv_item to write.
            */
        const byte                  *data_ptr,
            /* Pointer to a data buffer to write to EFS.
            */
        uint32                       size
            /* Size of the data_ptr buffer.
            */
);

/*===========================================================================

FUNCTION tdsrrcnv_efs_delete

DESCRIPTION
  Delete an nv item from the EFS

DEPENDENCIES
  None

RETURN VALUE
  TRUE: sucess
  FALSE: failure

SIDE EFFECTS
  None.

===========================================================================*/

boolean tdsrrcnv_efs_delete
(
        tds_rrc_nv_idx_type          nv_item
);

/*===========================================================================

FUNCTION tdsrrcnv_get_default_value

DESCRIPTION
  This function gets default value of a particular NV item. 
  Any change of default NV value should update this function.

DEPENDENCIES
  None.

RETURN VALUE
  Actual size of the NV item. 0 indicates an error.

===========================================================================*/
int tdsrrcnv_get_default_value
(
   tds_rrc_nv_idx_type  nv_item,
   byte                *data_ptr,
   int32                size,
   boolean              write_to_nv
);

/*===========================================================================

FUNCTION tdsrrcnv_get_non_rrc_nv_default_value

DESCRIPTION
  This function gets default value of a particular NV item. 
  Any change of default NV value should update this function.

DEPENDENCIES
  None.

RETURN VALUE
  Actual size of the NV item. 0 indicates an error.

===========================================================================*/
uint32 tdsrrcnv_get_non_rrc_nv_default_value
(
   tds_rrc_non_rrc_nv_idx_type  nv_item,
   byte                         *data_ptr,
   uint32                       size
);

/*===========================================================================
FUNCTION RRC_GET_NV

DESCRIPTION
  Get an item from the nonvolatile memory.  Handles nonactive items by
  providing a default value.

RETURN VALUE
  The NV return code, except for NV_NOTACTIVE_S, which is handled
  internally.

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
nv_stat_enum_type tdsrrc_get_nv(
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to space for item */
);


/*===========================================================================
FUNCTION RRC_PUT_NV

DESCRIPTION
  Write an item to NV memory.  Wait till write is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
nv_stat_enum_type tdsrrc_put_nv(
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
);

/*===========================================================================

FUNCTION          tdsrrcnv_read_nv_forbid_cells

DESCRIPTION       This function reads the forbidden cells from NAS NV/EFS file.
           
DEPENDENCIES
                  None

RETURN VALUE
                  None.

SIDE EFFECTS
                  None.

NOTE
                  None.
===========================================================================*/
void tdsrrcnv_read_nv_forbid_cells(sys_modem_as_id_e_type as_id);

/*===========================================================================

FUNCTION          tdsrrcnv_read_nv_refresh

DESCRIPTION       This function reads all the NV/EFS after refresh.
           
DEPENDENCIES
                  None

RETURN VALUE
                  None.

SIDE EFFECTS
                  None.

NOTE
                  None.
===========================================================================*/
void tdsrrcnv_read_nv_refresh(sys_modem_as_id_e_type as_id);

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_tdscdma_optional_feature_list

DESCRIPTION
  This  function gets the NV item NV_WCDMA_OPTIONAL_FEATURE_LIST_I and stores it in 
  rrc global variable 'tds_nv_optional_feature_list'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_tdscdma_optional_feature_list
(
  void
);

#ifdef FEATURE_TDSCDMA_HSUPA
/*===========================================================================

FUNCTION tdsrrcnv_read_nv_tds_hsupa_category

DESCRIPTION
  This  function gets the NV item NV_WCDMA_HSUPA_CATEGORY_I and stores it in 
  rrc global variable 'tds_nv_hsupa_category'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_tds_hsupa_category
(
  void
);
#endif /* FEATURE_TDSCDMA_HSUPA */

/*===========================================================================

FUNCTION tdsrrcnv_read_flow_control_nv_items

DESCRIPTION
  This  function gets the NV item for flow control and stores it in 
  rrc global variable 'tdscdma_flow_control_nv'. L2 will call API tdsrrc_get_rlc_nv_list() to access
  the value of this NV item. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_flow_control_nv_items(void);




/*===========================================================================

FUNCTION tdsrrcnv_read_nv_tds_l2_opt_feature_list

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_TDS_L2_OPT_FEATURE_LIST and stores it in 
  rrc global variable 'tds_nv_l2_opt_feature_list'. L2 will call API tdsrrc_get_l2_opt_nv() to access
  the value of this NV item. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_tds_l2_opt_feature_list(void);

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_interrat_feature_ctrl

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_INTERRAT_FEATURE_CTRL and stores it in 
  rrc global variable 'tds_nv_interrat_feature_ctrl'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_interrat_feature_ctrl(void);

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_silent_redial_opt

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_SILENT_REDIAL_OPT and stores it in 
  rrc global variable 'tds_nv_silent_redial_opt'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_silent_redial_opt(void);

#ifdef FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT
/*===========================================================================

FUNCTION tdsrrcnv_read_nv_t2l_ping_pong_redir_bar_time

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_T2L_PING_PONG_REDIR_BAR_TIME and stores it in 
  rrc global variable 'tdsrrc_nv_t2l_ping_pang_redir_bar_time'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_t2l_ping_pong_redir_bar_time(void);
#endif

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_items

DESCRIPTION
  This  function should call all the functions which read NV items to be used in RRC

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_items(void);




/*===========================================================================

FUNCTION tdsrrcnv_read_nv_item

DESCRIPTION
This function internally calls tdsrrcnv_create_nv_item() and tdsrrcnv_read_from_efs() 
in order to create and read the NV items.This uses the globally defined NV conf file,
NV conf item, address of default value to be written and size of the NV item to call 
the above functions.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrc_read_nv_item
(
  void
);

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_item_default

DESCRIPTION
This function similar to WCDMA's rrc_read_nv_item.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_item_default(void);

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_special_settings_for_testing

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_SPECIAL_SETTINGS_FOR_TESTING and stores it in 
  rrc global variable 'tdsrrc_special_settings_for_testing_nv'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_special_settings_for_testing
(
  void
);

/*===========================================================================

FUNCTION tdsrrcnv_process_get_cmcc_config

DESCRIPTION
  This function gets CMCC config value.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_process_get_cmcc_config(void);

/*===========================================================================

FUNCTION tdsrrcnv_process_set_cmcc_config

DESCRIPTION
  This function sets CMCC config value.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_process_set_cmcc_config(tdsrrc_cmd_type* cmd_ptr);

#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/*===========================================================================

FUNCTION tdsrrcnv_read_nv_tds_rlc_reestablish

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_RLC_REESTABLISH and stores it in 
  rrc global variable 'tdsrrc_rlc_reestablish_nv'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_tds_rlc_reestablish
(
  void
);
#endif

/*===========================================================================

FUNCTION tdsrrcnv_get_pseudo_sib19

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_PSEUDO_SIB19 and stores it in 
  rrc global variable 'tdsrrcnv_pseudo_sib19'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_get_pseudo_sib19(void);

#endif /* TDSRRCNV_H */
