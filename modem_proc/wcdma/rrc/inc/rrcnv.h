#ifndef RRCNV_H
#define RRCNV_H
/*===========================================================================
                  R R C   N V   M O D U L E

DESCRIPTION
  This header file contains function prototypes for the RRC NV routines.
  These routines have been created to read and write RRC information into the NV.

Copyright (c) 2001-2003,2006-2009 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/RRC/vcs/rrcnv.h_v   1.0   12 Feb 2002 10:48:58   bumatt  $
$Header: //components/rel/wcdma.mpss/8.4.0/rrc/inc/rrcnv.h#9 $ $DateTime: 2024/09/20 01:56:28 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/22/20   ha      Enable HI AC even when LO AC absen through NV
12/15/21    ha     FR53313 : Requirement to have configurable pseudo meas defer timer First time entering to DCH through NV
05/21/20    gc      Made changes to block FRLTE for specific PLMNs
18/5/20   gcs     Added NV support to disable/enable feature FRLTE for LTOW CSFB due to supplementary service and Made changes to trigger FRLTE only if NV is enabled
04/20/20   gcs     FR59710: Made changes to delay FRLTE and WtoL Resel for MAV
06/24/19   gcs     Made changes for FR 57381
11/20/18   ak      Made changes for FR 53313
02/23/17   sas     Changes to have NV gaurd to control the Truncated/Full CA feature.
04/12/18   ak      Made changes to read complete sibs independent of their entry in SIBdb.
22/09/16   sg      Made change to handle back to back SMC
02/21/18   nr      Added code to support LTA during activation time
01/18/18   vs      Added NV for allowing LTA during access priority
10/28/16   sr      Changes to bar the PSC for 1280 seconds if MAX attempts fail and the cell doesn't have any intra and inter f nbrs
10/20/16   sr      FR 38167: Changes to restrict the code to specific PLMN
10/19/16   sr      FR 38167: Cell Reselection from WCDMA to LTE B66 Support
05/03/16   sp      Made changes to have a maximum wait time for allowing LTAs despite of pending L2 acks
03/30/16   sn      Changes for FR30515
03/09/16   sn      Changes for FR30881
10/20/15   gv      Made changes to indicate success to MCFG when NV referesh is triggered for SUB where W is not active
09/25/15   sp      Added NV support for MSIM DBDC band combination
08/04/15   sg      Made changes to fill edch category to 6 and disable 16QAM if DCHSUPA NV is enabled 
08/07/15   sp      NV changes to control l2 ack wait timer during reselection
07/20/15   sr      Made changes for FR29345 EFS/NV Settings to be Read per Sub
07/08/15   sr      TRM-U changes for NAS-RRC interface
05/21/15   vi      Added NV to half the DRX length.
05/20/15   sr      Changes for NV refresh specific to W+W
03/10/15   sas     NV Changes to control reading of all SIBs for every reselection irrespective of SIBDB.
01/22/15   bc      Changes to provide NV control to allow TA for loop back call and CS Data call
12/29/14   bc      Made changes to control the range of frequencies to be skipped around a WCDMA cell by an nv.
11/27/14   sr      NV control for FR23776
10/16/14   bc      Changes to enable FR21174 and FR21035
11/03/14   ad      Made changes to control different DB DC band combinations through NV
10/22/14   gv      Made changes to have connected mode feature set independent of TA status 
10/08/14   sn      NV for new features
10/06/14   gv      Made changes to follow same feature set in DSDA irrespective of number of active subs
09/17/14   sg      Made changes to move all RRC nv macros to wnv_rrc.h
08/08/14   ac      Update NV defaults for rrc_dsds_concurrency, rrc_dsda_concurrency, rrc_tsts_concurrency
07/25/14   sr      API return type change for NV refresh FR
07/17/14   sr      Changes for NV refresh FR
07/15/14   ac      NV reorganization
06/23/14   vi      Made changes to skip SIB7 reading if SIB7 expiration timer is stil running
06/20/14   sa      Made changes to support UL 16QAM
06/17/14   sp      Made changes to mainline DCH-FACH transition optimization changes
06/13/14   rmn     Moving rrc_dormancy_support_nv outside DSDS Feature Flag
05/27/14   sn      Changes to configure dormancy feature through NV
04/29/14   sa      Made changes for NV support for default config in CELL FACH
05/14/14   ymu     Feature name FEATURE_WCDMA_DC_HSUPA_FRAME replacement with FEATURE_WCDMA_DC_HSUPA 
05/12/14   sn      Changes to control SIB sleep before receiving SBs using NV
05/09/14   sr      Made changes to disable UL compression capability in DSDS/DSDA mode.
04/16/14   sp      Made changes to exclude DCHSUPA from 3C concurrency NVs
04/11/14   vi      Made changes to disable FE_FACH capability in DSDS/DSDA mode.
04/10/14   db      Made changes to enable/disable feature support in DSDS/DSDA/TSTS modes through NV
03/28/14   vi      Added new NV rrc_constant_320ms_drx_nv 
03/17/14   sr      Added NV support for uplink compression capability
03/05/14   sn      Changes to control ASF timer with a NV 
09/25/13   ad      Made changes to split NV control of FDPCH and CPC DTX/DRX features
12/24/13   db      Introduced new NV to allow CPC,HSFACH,HSRACH,DC-HSDPA in DSDS and DSDA mode
12/13/13   sn      Manual PLMN search optimizations
11/03/13   sg      Made changes to update review comments for 3C 
11/27/13   sp      Made changes to add nv support and to save time ticks for 
                   rr to/from rrc mm to/from rrc and to add msg_type for crash debug ota changes
11/14/13   sa      DC feature support will be controlled thru UE Cat
10/30/13   as      Made changes to add feature code for FEATURE_WCDMA_3C_HSDPA
05/15/13   db      Made changes to enhance NV 67333 to control FDPCH and CPC DTX/DRX features independently
05/09/13   sn      Reduce fine/raw scan skip range for manual CSG search
12/07/12   sn      CSG feature changes
11/29/12   mp      Made chagnes to ignore meas reports for PSC lock under NV
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
07/03/12   sks     Made changes to support NV item for wtol_TDD ps_ho
05/02/12   rd      Made changes to support NV item for wtol_ps_ho
04/02/12   db      Made changes to disable CPC and FDPCH through NV
03/29/12   pm      Added code to support the band priority nv
02/24/12   sn      Changes to periodically save ACQ DB in NV
03/09/12   gv      Provided NV support to enable/disable PPAC functionality
01/02/12   pm      Added rrc_wtol_cm_support_nv to put FEATURE_WCDMA_CM_LTE_SEARCH feature under nv check 
10/31/11   sks     Added support for FEATURE_WCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB.
06/29/11   su      Added code for NV support to enable/disable
                   the FEATURE_RRC_DO_NOT_FORWARD_PAGE.
05/10/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.97.50
05/05/11   vg      added channal locking feature support under rrccsp_channel_locking_nv_status
                   condition
05/03/11   su      Added code changes to provide NV support to enable/disable
                   FEATURE_3GPP_CSFB_SKIP_OPT_WCDMA_SIBS.
03/29/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.94.50
03/28/11   su      Made changes to set SRB2 suspend_offset based on 
                   rrc_set_srb2_act_time_nv item.
03/14/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.93.50
03/08/11   su      Added code to support enabling and disabling of 
                   FEATURE_UEUT_IGNORE_CELL_BAR_RESERVE_STATUS using NV item.
03/07/11   su      Added code to support device type NV.
02/07/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.90.50
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
10/07/09   gkg     Changes for REL8 under FEATURE_WCDMA_REL8
05/08/09   ss      Updated Copyright Information
04/29/09   ss      Made changes to compiler warnings on gcc compilers.
01/20/08   ps      Made changes to make the default HSUPA CAT as 5 if
                   FEATURE_WCDMA HSUPA NV DEFAULT CATEGORY 5 is defined.
                   Even though 2 ms TTI is also supported
12/08/08   ps      Made changes to set the MAC-HS(R5/R6 HS-DSCH )CAT 
                   according to global variable "rrc_machs_cat" when the 
                   HSDPA CAT is above 12 for R7
11/25/08   ps      Made changes to reject OTA if N\W configures 64 QAM, but
                   NV indicates that 64 QAm is not supported by UE.
09/12/08   ps      Added code for REl7 under FEATURE_WCDMA_REL7
02/17/08   sm      Added function rrc_get_hsupa_category() which is called by L1
                   to get the hsupa category
02/01/08   sm      Modified default value of nv_hsupa_category when feature 
               FEATURE WCDMA HSUPA 2MS TTI is defined
10/08/07   da      Extern rrc_wcdma_hsupa_cm_enabled global used by other modules to
                   check the status for HSUPA_CM NV item status
09/12/06   sm/da   Fix code review comments and general clean-up for REL6 and HSUPA
08/29/06   sm/da   Made changes required for R6 ASN.1 migration.  Also merged
                   FEATURE_WCDMA_REL6 and FEATURE WCDMA HSUPA.
01/25/02   bu      Created file with prototypes for NV routines.

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "wcdma_variation.h"
#include <string.h>

#include "err.h"
#include "nv.h"
#include "rrcdata_v.h" 
#include "fs_public.h"
#include "wnv_rrc.h"

#ifdef FEATURE_MODEM_CONFIG_REFRESH
#include <mcfg_refresh.h>
#endif

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
#define RRC_NV_CONF_FILE_PERMISSIONS_COMMON 0777
#define RRC_NV_NUM_FORBID_CELLS 20
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* WCDMA forbidden cell structure */
typedef struct
{
  sys_plmn_id_s_type sys_plmn_id;
  uint32 cell_id;
} rrc_nv_forbid_cell_type;

/*Common EFS file structure*/
typedef struct
{
  sys_sys_mode_e_type rat;
  uint32 cell_id;
  uint32 lac;
  sys_plmn_id_s_type sys_plmn_id;
} rrc_efs_forbid_cell_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

typedef struct
{
  /* variable to indicate support of feature list supported.
     * This variable will be set using the value of NV item 'NV_WCDMA_OPTIONAL_FEATURE_LIST_I'
     * WCDMA_NV_OPTIONAL_FEATURE_HSDPA 0x1<<1 
     * WCDMA_NV_OPTIONAL_FEATURE_HSUPA 0x1<<2
     * WCDMA_NV_OPTIONAL_FEATURE_MBMS 0x1<<3
     * WCDMA_NV_OPTIONAL_FEATURE_RX_DIVERSITY 0x1<<4
     * WCDMA_NV_OPTIONAL_FEATURE_PS_DATA_HO 0x1<<5
     * all other bits are spare bits - for future use
     */
  uint32 wcdma_nv_optional_feature_list;


  /* variable to store the hsupa category.
     * This variable will be set using the value of NV item 'NV_WCDMA_HSUPA_CATEGORY_I'
     */
  uint16 wcdma_nv_hsupa_category;

  /* Boolean to control HSUPA + CM feature enabling\disabling via NV item*/
  boolean rrc_wcdma_hsupa_cm_enabled;

  uint8 rrc_device_type_nv;


  uint8 rrc_ignore_cell_bar_nv;

#ifdef FEATURE_WCDMA_SNOW3G_SECURITY
  boolean snow3g_security_algo_supported;
#endif

#ifdef FEATURE_SMC_SRB2_SUSPENSION
  uint8 rrc_set_srb2_act_time_nv;
#endif


  uint8 rrc_csfb_skip_sib11_opt_nv;

  uint16 rrc_nv_channel_lock_item_value; 

  uint8 rrc_do_not_forward_page_nv;

#ifdef FEATURE_WCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB

  boolean rrc_fast_return_to_lte_after_csfb_nv;

  uint16 rrc_fast_return_to_lte_after_csfb_timer_nv;

  rrc_nv_delay_return_to_lte_type rrc_delay_return_to_lte_nv;

  boolean rrc_fast_return_to_lte_after_csfb_ss_nv;

#endif

  boolean rrc_wtol_cm_support_nv;

  boolean rrc_ppac_support_nv;

  boolean rrc_save_acqdb_nv;

  uint8 rrc_disable_cpc_fdpch_nv;

  uint8 rrc_disable_crash_debug_info_nv;

#ifdef FEATURE_WCDMA_DEBUG_ATTEMPT_FULL_SRCH
  boolean rrc_enable_psc_lock_nv;
#endif

  /* If NV item not set yet, we default to Bands */
  rrc_nv_band_priority_config_type  rrc_nv_band_priority_config;

  /* EFS files to manage band priority configuration NV items. */
  boolean wcdma_rrc_wtol_ps_ho_support_nv;

  boolean wcdma_rrc_wtol_tdd_ps_ho_support_nv;

#ifdef FEATURE_FEMTO_CSG
  uint32 wcdma_rrc_csg_max_prune_count_nv; /* Number of ASF searches in one month */

  /*If enabled manual CSG search considers minimum spacing between two carriers as 2.5MHz otherwise 4.4MHz*/
  boolean wcdma_csg_srch_carrier_space_nv;

  uint8 wcdma_asf_timer_nv;
#endif

  uint32 wcdma_rrc_freq_scan_timer_nv;

/* This defines the max time for which service requests should not be entertained on finding no service
 * for a previous service request in deep sleep and empty available plmns.
 */
  uint32 wcdma_rrc_deep_sleep_no_svc_timer_nv;

/* This defines the max time (default - 15 mins) for which BPLMN Frequency scan should not be initiated */
  uint32 wcdma_rrc_bplmn_freq_scan_timer_nv;

#ifdef FEATURE_GANSS_SUPPORT
  boolean rrc_ganss_support_nv;
#endif

  uint8 rrc_dormancy_support_nv;

  /*If enabled WRRC starts SIB sleep before receiving Scheduling blocks*/
  boolean wcdma_rrc_sib_sleep_sb_nv;

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH
  uint8 rrc_fe_fach_support_nv;
#endif

#ifdef FEATURE_DUAL_SIM
uint16 rrc_dsds_concurrency;

uint16 rrc_dsda_concurrency;

uint8 wcdma_rrc_time_to_allow_lta_during_access_priority;
#ifdef FEATURE_TRIPLE_SIM
uint16 rrc_tsts_concurrency;
#endif /*FEATURE_TRIPLE_SIM*/
#endif /*FEATURE_DUAL_SIM*/

#ifdef FEATURE_WCDMA_MULTI_FREQ_BAND_INDICATOR
  boolean rrc_mfbi_support_nv;
#endif

  boolean rrc_constant_320ms_drx_nv;

  uint16 rrc_disable_meas_nv;

#ifdef FEATURE_WCDMA_UL_COMPR
  /* NV# 72576 */
  boolean rrc_ul_compr_cap_nv;
#endif
  boolean rrc_prioritize_850_band_nv;


#ifdef FEATURE_WCDMA_DEFAULT_CFG_CELLFACH
/*Support HS-FACH and CPC by default*/
  uint8 rrc_default_cfg_in_cell_fach_nv;
#endif
  uint32 rrc_sib7_time_nv;

  boolean rrc_meas_id_extn_support;

  /*Bit mask to for RRC idle features*/
  uint32 wcdma_rrc_idle_features;

  uint16 wcdma_rrc_db_dc_band_comb_nv_control;

  uint8 nv_hsdpa_category;

  uint16 rrc_feature_nv_item_value;

  uint8 wcdma_freq_skip_range_nv;

  boolean enable_ta_for_cs_datacall_nv;

  boolean rrc_read_sibs_for_resel_enabled_nv;

  boolean rrc_half_drx_len_nv;

  boolean rrc_read_sibs_enabled_nv;

#ifdef FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME
  wcdma_rrc_support_lta_type  wcdma_rrc_support_lta;
#endif

#ifdef FEATURE_DUAL_SIM
  uint16 rrc_change_trm_priority_tmr_nv;
#endif
  rrc_nv_defer_cel_resel_waiting_for_l2_ack_type wcdma_rrc_defer_cel_resel_waiting_for_l2_ack;

  rrc_nv_forbid_cell_type rrc_nv_forbid_cells[RRC_NV_NUM_FORBID_CELLS];
  uint32 num_forbid_cells;

  uint16 wcdma_rrc_msim_db_dc_band_comb_nv_control[MAX_UE_MODES];

  rrc_nv_pseudo_sib19_type rrc_nv_psuedo_sib19_nv;

  rrc_nv_msim_opt_type wcdma_rrc_msim_opt;

  rrc_plmn_list_type rrc_block_frlte_on_plmn_list;

  rrc_plmn_identity_type efs_plmn_id;
  rrc_nv_gsm_fallback_info_type rrc_gsm_fallback_info;

  rrc_nv_pseudo_meas_info_type  rrc_pseudo_meas_info;

  rrc_nv_pseudo_meas_rxagc_info_type  rrc_pseudo_meas_rxagc_info;

  rrc_nv_pseudo_meas_triggers_info_type  rrc_pseudo_meas_triggers_info;

  rrc_nv_pseudo_meas_rsrp_rsrq_info_type     rrc_pseudo_meas_rsrp_rsrq_info;

  boolean rrc_cell_bar_upon_rach_failure_nv;
  uint16  rrc_cell_bar_duration_secs_nv;
  rrc_smc_buffer_support_type  rrc_buffer_smc_support;

  boolean rrc_req_lte_for_truncated_ca_nv;

  rrc_mcc_list_type rrc_req_lte_for_truncated_ca_on_mcc_list_nv;
boolean rrc_wtol_redirect_after_srvcc_nv;

#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
  uint16 rrc_pseudo_meas_forbidden_earfcn_timer_nv;
#endif

  uint16 rrc_pseudo_meas_defer_tmr_value_first_time_in_dch_nv;
  uint8 rrc_enable_hi_ac;
} rrcnv_data_type;

#ifdef FEATURE_DUAL_WCDMA
extern rrcnv_data_type rrcnv_data_sim[MAX_RRC_STACKS];
#define rrcnv_data rrcnv_data_sim[WAS_ID_M]
#else
extern rrcnv_data_type rrcnv_data;
#endif

extern const uint8 rrc_machs_cat;
extern const uint8 rrc_macehs_cat;

/* =======================================================================
**                          Macro Definitions
** ======================================================================= */


/* HSDPA: Bit 0 <uint32: bit31..bit0> */
#define WCDMA_NV_OPTIONAL_FEATURE_HSDPA 0x1<<1 

/* HSUPA: Bit 1 <uint32: bit31..bit0> */
#define WCDMA_NV_OPTIONAL_FEATURE_HSUPA 0x1<<2

/* MBMS: Bit 2 <uint32: bit31..bit0> */
#define WCDMA_NV_OPTIONAL_FEATURE_MBMS 0x1<<3

/* RX DIVERSITY: Bit 3 <uint32: bit31..bit0> */
#define WCDMA_NV_OPTIONAL_FEATURE_RX_DIVERSITY 0x1<<4

/* PS DATA HO: Bit 4 <uint32: bit31..bit0> */
#define WCDMA_NV_OPTIONAL_FEATURE_PS_DATA_HO 0x1<<5

#define CHECK_WCDMA_OPTIONAL_FEATURE_HSUPA_SUPPORTED (rrcnv_data.wcdma_nv_optional_feature_list & WCDMA_NV_OPTIONAL_FEATURE_HSUPA)

#define CHECK_WCDMA_OPTIONAL_FEATURE_HSDPA_SUPPORTED (rrcnv_data.wcdma_nv_optional_feature_list & WCDMA_NV_OPTIONAL_FEATURE_HSDPA)
#define CHECK_WCDMA_OPTIONAL_FEATURE_MACEHS_SUPPORTED (rrcnv_data.nv_hsdpa_category > 12 )

#ifdef FEATURE_WCDMA_MIMO
#define CHECK_WCDMA_OPTIONAL_FEATURE_MIMO_SUPPORTED (rrcnv_data.nv_hsdpa_category >= 15 )
#endif

#ifdef FEATURE_WCDMA_DC_HSDPA
#define CHECK_WCDMA_OPTIONAL_FEATURE_DC_HSDPA_SUPPORTED (rrcnv_data.nv_hsdpa_category >= 21)
#endif /*FEATURE_WCDMA_DC_HSDPA*/
#ifdef FEATURE_WCDMA_3C_HSDPA
#define CHECK_WCDMA_OPTIONAL_FEATURE_3C_HSDPA_SUPPORTED (rrcnv_data.nv_hsdpa_category >= 29)
#endif
#ifdef FEATURE_WCDMA_DC_HSUPA
#define CHECK_WCDMA_OPTIONAL_FEATURE_DC_HSUPA_SUPPORTED (rrcnv_data.wcdma_nv_hsupa_category == 8)
#endif /*FEATURE_WCDMA_DC_HSUPA*/
#ifdef FEATURE_WCDMA_16_QAM
#define CHECK_WCDMA_OPTIONAL_FEATURE_16_QAM_SUPPORTED (rrcnv_data.wcdma_nv_hsupa_category == 7)
#endif
/*64QAM is not supported if NV is 15 or 16 . It is supported for HS cat 13, 14, 17,18*/
/*With the introduction of DC-HSDPA, 64QAM isn't supported for CAT 21 and 22 either.*/
#define CHECK_WCDMA_OPTIONAL_FEATURE_64QAM_SUPPORTED ((rrcnv_data.nv_hsdpa_category >= 13) && \
  ((rrcnv_data.nv_hsdpa_category != 15) && (rrcnv_data.nv_hsdpa_category != 16) && (rrcnv_data.nv_hsdpa_category != 21) && (rrcnv_data.nv_hsdpa_category != 22)))

#define CHECK_CONCURRENCY_FOR_EFACH(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!(nv_value & NV_DS_HSFACH_CONCURRENCY))

#define CHECK_CONCURRENCY_FOR_MAC_I(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!(nv_value & NV_DS_MAC_I_CONCURRENCY))

#define CHECK_CONCURRENCY_FOR_CPC(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!(nv_value & NV_DS_CPC_CONCURRENCY))

#define CHECK_CONCURRENCY_FOR_EDRX(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!((nv_value & (NV_DS_EDRX_CONCURRENCY|NV_DS_HSFACH_CONCURRENCY)) == \
       (NV_DS_EDRX_CONCURRENCY|NV_DS_HSFACH_CONCURRENCY) ))

#define CHECK_CONCURRENCY_FOR_HSRACH(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!((nv_value & (NV_DS_HSRACH_CONCURRENCY|NV_DS_HSFACH_CONCURRENCY|NV_DS_MAC_I_CONCURRENCY)) == \
       (NV_DS_HSRACH_CONCURRENCY|NV_DS_HSFACH_CONCURRENCY|NV_DS_MAC_I_CONCURRENCY)))

#ifdef FEATURE_WCDMA_DC_HSDPA
#define CHECK_CONCURRENCY_FOR_DCHSDPA(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!(nv_value & NV_DS_DCHSDPA_CONCURRENCY))
#endif

#ifdef FEATURE_WCDMA_DC_HSUPA
#define CHECK_CONCURRENCY_FOR_DCHSUPA(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!((nv_value & (NV_DS_DCHSUPA_CONCURRENCY|NV_DS_DCHSDPA_CONCURRENCY|NV_DS_MAC_I_CONCURRENCY)) == \
       (NV_DS_DCHSUPA_CONCURRENCY|NV_DS_DCHSDPA_CONCURRENCY|NV_DS_MAC_I_CONCURRENCY)))
#endif

#ifdef FEATURE_WCDMA_3C_HSDPA
#define CHECK_CONCURRENCY_FOR_3CHSDPA(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!((nv_value & (NV_DS_3CHSDPA_CONCURRENCY|NV_DS_DCHSDPA_CONCURRENCY)) == \
        (NV_DS_3CHSDPA_CONCURRENCY|NV_DS_DCHSDPA_CONCURRENCY)))
#endif

#ifdef FEATURE_WCDMA_DB_DC_HSDPA
#define CHECK_CONCURRENCY_FOR_DBDC_HSDPA(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!((nv_value & (NV_DS_DBDC_HSDPA_CONCURRENCY|NV_DS_DCHSDPA_CONCURRENCY)) == \
       (NV_DS_DBDC_HSDPA_CONCURRENCY|NV_DS_DCHSDPA_CONCURRENCY)))
#endif

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH
#define CHECK_CONCURRENCY_FOR_FE_FACH(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!(nv_value & NV_FE_FACH_CONCURRENCY))
#endif

#ifdef FEATURE_WCDMA_UL_COMPR
#define CHECK_CONCURRENCY_FOR_UL_COMP(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!(nv_value & NV_UL_COMPRESSION_CONCURRENCY))
#endif

#ifdef FEATURE_DUAL_SIM
#define CHECK_CONCURRENCY_FOR_W_BMC(nv_value) \
  (rrc_dsds_int_data.wcdma_look_for_concurrency_nv) && \
   (!(nv_value & NV_W_BMC_CONCURRENCY))
#endif

/*FOR PSEUDO MEAS FR*/
#define CHECK_FOR_TRIGGER      (RRC_ELEVATOR_ENABLED|RRC_LOW_THRPUT_ENABLED)

/*Bit 0 NV#73584  Enable this to skip SIB7 read for PCH to FACH transition*/
#define RRC_SIB7_SKIP_READ 0x01

/*Bit 1 in NV#73584 Enable this to perform QTA during SIB reading */
#define RRC_QTA_SIB_READ   0x02

/*Bit 2 NV#73584  Set this to 1 to enable split acq during BPLMN*/
#define RRC_ACQ_SPLIT_DURING_BPLMN 0x04

/*Bit 3 in NV#73584 Enable this to perform QTA during signalling */
#define RRC_QTA_DURING_SIGNALLING   0x08

/*Bit 4 NV#73584  Set this to 1 to enable CSFB call timeline optimization */
#define RRC_CSFB_CALL_TIMELINE_OPT 0x10

/*Bit 5 in NV#73584  Set this to 1 to skip reading sibs further after MIB,SIB1,SIB3/SIB4 are read on Non NAS req PLMN*/
#define RRC_SKIP_SIBS_ON_NON_NAS_REQ_PLMN_OPT 0x20

/*Bit 6 in NV#73584  Set this to 1 to enable reporting CA capability on only NW req LTE bands*/
#define RRC_NW_REQ_LTE_BAND_CAP_REPORT 0x40

/*Bit 7 in NV#73584  Set this to 1 to enable Inter SIB Sleep for SIB-segments*/
#define RRC_ISS_FOR_SIB_SEGMENTS 0x80

/*Bit 8 in NV#73584  Set this to 1 to enable WTOL Band 66 reselection support */
#define RRC_EXTENDED_EARFCN_SUPPORT 0x100

/*Bit 9 in NV#73584  Set this to 1 to enable barring opt for bad cells */
#define RRC_BARRING_OPT_RACH_FAILURE 0x200

#define WCDMA_RRC_IDLE_FEATURES(x) (rrcnv_data.wcdma_rrc_idle_features & (x))
/* =======================================================================
**                        Function Declarations
** ======================================================================= */

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
nv_stat_enum_type rrc_get_nv(sys_modem_as_id_e_type wrrc_as_id,
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
nv_stat_enum_type rrc_put_nv(sys_modem_as_id_e_type wrrc_as_id,
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
);

/*===========================================================================
FUNCTION RRC_QUEUE_NV_WRITE

DESCRIPTION
  Posts a NV write request to async_efs_put

RETURN VALUE
  Zero if request is successful otherwise -1

DEPENDENCIES
===========================================================================*/
uint32 rrc_queue_nv_write(sys_modem_as_id_e_type wrrc_as_id,
  nv_items_enum_type item,  /* which item */
  nv_item_type *data_ptr,  /* pointer to data for item */
  uint32 size
);

/*===========================================================================
FUNCTION RRC_REPLACE_NV

DESCRIPTION
  Replace an item to NV memory.  Wait till write is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
nv_stat_enum_type rrc_replace_nv(sys_modem_as_id_e_type wrrc_as_id,
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
);

/*===========================================================================
FUNCTION RRC_FREE_NV

DESCRIPTION
  Free an item in NV memory.  Wait till free is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
nv_stat_enum_type rrc_free_nv(  sys_modem_as_id_e_type wrrc_as_id,
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
);

/*===========================================================================

FUNCTION rrc_read_nv_wcdma_optional_feature_list

DESCRIPTION
  This  function gets the NV item NV_WCDMA_OPTIONAL_FEATURE_LIST_I and stores it in 
  rrc global variable 'wcdma_nv_optional_feature_list'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_nv_wcdma_optional_feature_list
(
sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrc_read_nv_wcdma_hsupa_category

DESCRIPTION
  This  function gets the NV item NV_WCDMA_HSUPA_CATEGORY_I and stores it in 
  rrc global variable 'wcdma_nv_hsupa_category'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_nv_wcdma_hsupa_category
(
  sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrc_read_nv_items

DESCRIPTION
  This  function should call all the functions which read NV items to be used in RRC

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_nv_items(sys_modem_as_id_e_type wrrc_as_id);




/*===========================================================================

FUNCTION rrc_read_nv_item

DESCRIPTION
This function internally calls rrc_create_nv_item() and rrc_read_nv_item() 
in order to create and read the NV items.This uses the globally defined NV conf file,
NV conf item, address of default value to be written and size of the NV item to call 
the above functions.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void rrc_read_nv_item
(
  sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================
FUNCTION rrc_get_from_efs

DESCRIPTION
  Reads the data from given EFS file

RETURN VALUE
  Boolean

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
boolean rrc_get_from_efs(sys_modem_as_id_e_type wrrc_as_id, char*, void*,fs_size_t);


/*===========================================================================
FUNCTION RRC_PUT_EFS

DESCRIPTION
  Write data to given EFS file.

RETURN VALUE
  Boolean

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
boolean rrc_put_efs(sys_modem_as_id_e_type wrrc_as_id, char*, void*,fs_size_t);


/*===========================================================================

FUNCTION rrc_read_all_nv_items

DESCRIPTION
This function internally calls other NV read functions

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_all_nv_items(sys_modem_as_id_e_type wrrc_as_id);

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================
FUNCTION rrc_nv_refresh_mcfg_regiser_cb

DESCRIPTION
  Function used to register for callback for receiving NV refresh indication

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
void rrc_nv_refresh_mcfg_regiser(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================
FUNCTION rrc_post_nv_refresh_ind

DESCRIPTION
  Callback fucntion invoked by MCFG to post NV refresh indication

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
boolean rrc_post_nv_refresh_ind_cb(mcfg_refresh_info_s_type *p_info);


/*===========================================================================
FUNCTION rrc_read_nv_refresh

DESCRIPTION
  Reads the NV/EFS values on receiving NV refresh indication

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
void rrc_read_nv_refresh(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION rrc_read_nv_item_refresh

DESCRIPTION
This function reads the NV items when SIM hotswap happens

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_read_nv_item_refresh(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================
FUNCTION rrc_nv_refresh_map_sub_index_to_as_id

DESCRIPTION
  Maps SUB index from the MCFG callback to W-AS-ID.

RETURN VALUE
  sys_modem_as_id_e_type

DEPENDENCIES
  None
===========================================================================*/
sys_modem_as_id_e_type rrc_nv_refresh_map_sub_index_to_as_id(mcfg_refresh_index_type sub_index);

#endif
/*===========================================================================
FUNCTION rrc_get_band_comb_control_nv

DESCRIPTION
  This function dumps nv values

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/

uint16 rrc_get_band_comb_control_nv(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION rrcnv_get_msim_opt_enabled_nv

DESCRIPTION
  This function returns msim opt nv enabled status

DEPENDENCIES
  None


RETURN VALUE
  uint8


SIDE EFFECTS
  None

===========================================================================*/

uint8 rrcnv_get_msim_opt_enabled_nv(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================
FUNCTION rrcnv_get_msim_opt_block_lta_timer

DESCRIPTION
  This function gets LTA block timer nv value

DEPENDENCIES
  None


RETURN VALUE
  uint16


SIDE EFFECTS
  None

===========================================================================*/

uint16 rrcnv_get_msim_opt_block_lta_timer(sys_modem_as_id_e_type wrrc_as_id);

#endif /* RRCNV_H */
