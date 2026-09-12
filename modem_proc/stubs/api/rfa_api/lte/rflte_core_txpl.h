#ifndef RFLTE_CORE_TXPL_H
#define RFLTE_CORE_TXPL_H

/*!
  @file
  rflte_core_txpl.h

  @brief
  Provides TXPL control.

  @details


*/

/*===========================================================================

Copyright (c) 2010 - 2015 by QUALCOMM TECHNOLOGY, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2018/04/15 02:22:08 $ $Author: mplcsds1 $
$Header: //commercial/MPSS.TA.3.0.c1/Main/modem_proc/stubs/api/rfa_api/lte/rflte_core_txpl.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/25/16   sg      MPR/AMPR CA_NS NV override support
04/14/16   sk      FBRX Tune-away support Phase 2
12/07/15   sg      Make TxPL APIs handle based
12/02/15   sg      [ML1->RF Interface] Make TxPL APIs handle based
11/26/15   mah     Updated Pcmax_update API to take All TxLM handles as inputs for ULCA.
11/15/15   mah     Added support to extract Txpl mode from handle.
11/04/15   mah      txlm handle fix for all api inside rflte_core_computer_p_cmax_update
11/04/15   mah     Changes to enable handle bases TxPL SM.
11/04/15   mah     Change more api to txlm handle based
11/04/15   mah     Handle based TXPL SM.
11/04/15   mah     Added support to Switch TxLM buffer upon ML1's call.
11/03/15   sg      [ML1->RF Interface] Make TxPL APIs handle based
10/22/15   sg      CA_NS based MPR/AMPR computation
10/22/15   sg      CA_NS based MPR/AMPR computation (ML1->RF Interface change)
10/12/15   mah     Make PcMax update Handle based
09/18/15   jj      Move programm FBRx dynamic adc settings to FW 
09/24/15   pkg     Changes to Support TxLM Buffer Index Switching upon Ml1 call.
09/24/15   pkg     Use correct MACRO while indexing into TxLM array.
09/08/15   qzh     Add RFLTE_CORE_TXPL_MAX_RB_NUM_PER_CARRIER for RB check
09/02/15   kr      Added support for tx max pwr based EARFCN NV
08/31/14   kab     Enable LTE FBRx at -45dBm for selftest 
08/28/15    sg     FR28508: Add ULCA Support for MTPL Backoff due to PA thermal
08/25/15    sg     Remove the FEATURE_RF_ASDIV wrapping for antenna posed DTPL/MTPL
                   ML1 interface variable to prevent scons dependencies.
08/20/15    JJ     Moved compute weighted avg tx freqcomp api to rflte_core_freq_comp.c
08/20/15    sg     FR27546: ASDiv SAR limits programmable per RAT/band
08/17/15   pkg     Changes to handle ULCA while accessing DM.
08/13/15    br     Added support for IMD backoff in ULCA
08/11/15   ska     Adding support for ULCA SCC2
08/07/15   sg      FR27572: Add AsDiv SAR backoff support
07/29/15    br     Added support for NLIC with ULCA
07/20/15   pkg     Changes to support FR 24616:Algorithm to mitigate VBatt
                   droop (down to 2.5V) via TX backoff.
05/29/15   kab     Multi-Cluster interface support
05/18/15   JJ      Added ULCA txagc logging support
05/12/15   pkg     Changes to support Freq Comp for Intra Band UL CA.
04/21/15    sk     Re-structure Tx path specific NV's per Tx NV container
04/21/15    sk     Bug fix in ULCA TxAGC update params to correctly
                   update the params for both carriers.
03/25/15    pv     FTM-RF support for new MTPL Interface.
03/19/15    yzw    Modify the function prototype for ULCA
03/19/15   pkg     MTPL Interface changes.
02/19/15   jf      64QAM initial support
11/10/14   pkg     Interface changes to support LTE UL CA Feature.
08/18/14   kab     TxPL changes to support SGLTE Tx power backoff with Coex.
07/25/14    vb     NLIC clock mode support
07/31/14   ndb     Added support for extended EARFCN for LTE
07/16/14   sbm     return mtpl for srs_settings_per_sf. 
05/16/14    pv     Added support for Carrier index to TxPL enable API.
05/06/14    pv     Reduced the FBRx Power threshold to -40dB based on RF Systems's reco.
04/17/14    pv     Added support for passing threshold for power tracking method
04/17/14   svi     PA static NV and code redesign
03/14/14   bsh     NlIC changes: ML1 API call with ul power - API name change
03/14/14   bsh     NlIC changes: ML1 API call with ul power
02/24/14    pv     Added support for FBRx Gain error processing with MTPL.
02/21/14   cri     Fix null access crash during freq comp
02/18/14    pv     Added support for FBRx Gain error processing with Common FBRx Trigger. 
02/18/14   cri     Only allocate freq comp during tx tune
02/13/14    pv     Added support for FBRx Gain error processing.
02/05/14   cri     Freq comp optimization for DM population
09/19/13   bar     ET optimizations for freq comp/pin/pout/delay 
07/29/13   as      Added support for relative backoff on SAR limit based on 
                   TDD-LTE ULDL configuration 
07/24/13    pv     Support for Delay vs Temp comp Enum.
07/02/13   ndb     Added dbm10 to TXAGC & TXAGC to dbm10 conversion macros
06/12/13   php     Increase HDET buffer size to accomodate increase in PDET sample size 
05/07/13   jf      Add multiple freq comp bins support for XPT   
05/02/13   cri     Store pin/pout comp in common structure for calv4
03/22/13   jf      Add 2D lin_vs_temp_vs_freq_comp 
03/15/13   gvn     Update sgsar, coex_WWAN, coex_WLAN limits using MSGR 
03/14/13   jf      Add XPT path delay support 
03/08/13   yzw     Add # define for max tx power 
02/19/13   sbm     Bandedge max power backoff support in LTE(Nikel CR375880) 
12/26/12   gvn     LTE Max Pwr limiting support during WLAN Coex scenario 
                   (merge from Nikel CR401193) 
12/10/12   pl      Remove obsolete TXPL interface 
12/10/12   pl      Add new API for PHR backoff reporting support 
12/04/12   php     Add support for getting HDET reads from FW IND message
11/20/12   gvn     Featurize LTE for Triton 
09/20/12   pl      Identify interfaces referenced by ML1
05/04/12   pl      New MTPL Implemenation
03/22/12   pl      Remove unused interface
03/22/12   pl      Remove obsolete interface
05/23/12   as      LTE Tx power limit for SGLTE SAR and CoEx
04/04/12   pl      Remove obsolete interfaces
01/11/12   cri     Update to LTE Tx Frequency comensation algorithm
11/03/11   pl      Support PHR-Less reporting
09/14/11   can     Merge MDM9K tips to NikeL.
08/18/11   pl      remove unused interface, removed exposed interface that are 
                   supposed to be internal functions
05/26/11   can     HDET Fast Correction (Super ACQ) mode support.
03/04/11   can     LTE SAR support based on index from rfm layer and NV
02/09/11   aca     FTM dac setpoint value override support
01/24/11   aca	   FTM Override DAC Setpoint
12/13/10   kma     Added API rflte_core_txpl_get_pcmax() to acquire pcmax 
11/08/10   can     Added support for emergency overheat TX power backoff.
10/22/10   can     Perform lim vs freq based on the new HDET MSGR message containing Physical RB allocation.
10/22/10   can     Support to calculate MTPL & freq comp for slot1, slot2 and SRS accurately.
10/20/10   can     Support to return the delta between the MTPL and the desired limit.
08/31/10   can     Change to get the DAC setpoint from RFSW TXPL code.
08/11/10   can     Support for LTE TXAGC logging and code clean-up.
08/05/10   can     Updates to SAR backoff implemetation for PHR update.
08/04/10   whc     Changed mtpl_to_l1 calculation to dB/10 using int16.
07/14/10   can     Changes to optimize HDET Low Power Mode.
07/12/10   can     Support for when FW reads HDET ADC and passes RFSW the value.
06/28/10   can     Added SAR support for SVLTE.
05/27/10   can     LOW PDET MODE HDET support.  
05/25/10   can     LIM vs FREQ based on RB allocation.  
05/21/10   can     Added LTE TX RB based FREQ COMP support.  
05/17/10   can     LTE MPR, AMPR and PEMAX support.
05/06/10   tnt     Adding p_cmax routine
05/05/10   can     Initial version.

============================================================================*/

#include "rfcom.h"

#ifdef FEATURE_LTE
#include "rffw_lte_intf.h"
#include "rflte_msg.h"
#include "nv_items.h"
#include "rflte_ext_mc.h"
#include "rflm_api_lte.h"


#define RFLTE_CORE_TXPL_MAX_NUM_SUBFRAMES_PER_FRAME  10
/* -70 dBm in dB10 = -700, 0dBm = 700 in TXAGC count */
#define RFLTE_CORE_TXPL_TXAGC_COUNT_MIN_OFFSET       700
#define RFLTE_CORE_TXPL_HDET_RD_RESULT_NUM_TRANS 12
#define RFLTE_CORE_TXPL_TXAGC_RESOLUTION         10

#define RFLTE_CORE_TXPL_MIN_TX_PWR_FLOOR_DBM        -70
#define RFLTE_CORE_TXPL_MIN_TX_PWR_FLOOR_DB10       (RFLTE_CORE_TXPL_MIN_TX_PWR_FLOOR_DBM * RFLTE_CORE_TXPL_TXAGC_RESOLUTION)

/* min value for TX power limit in dbm */
#define RFLTE_CORE_TXPL_MIN_TX_PWR_LIM           -44
#define RFLTE_CORE_TXPL_MIN_TX_PWR_LIM_DB10      (RFLTE_CORE_TXPL_MIN_TX_PWR_LIM * RFLTE_CORE_TXPL_TXAGC_RESOLUTION)

/* maximum value for TX power limit in dbm */
#define RFLTE_CORE_TXPL_MAX_TX_PWR_LIM           23
#define RFLTE_CORE_TXPL_MAX_TX_PWR_LIM_DB10      (RFLTE_CORE_TXPL_MAX_TX_PWR_LIM * RFLTE_CORE_TXPL_TXAGC_RESOLUTION)

/* max value for Tx power in dbm*/
#define RFLTE_CORE_TXPL_MAX_TX_POWER_DBM        32.3
#define RFLTE_CORE_TXPL_MAX_TX_POWER_DB10       (RFLTE_CORE_TXPL_MAX_TX_POWER_DBM * RFLTE_CORE_TXPL_TXAGC_RESOLUTION)

/* FBRx Power Threshold */
#define RFLTE_CORE_TXPL_FBRX_PWR_THRESHOLD_DB10         -400

/* FBRx Power Threshold - Self Test */
#define RFLTE_CORE_TXPL_FBRX_SELF_TEST_PWR_THRESHOLD_DB10         -450

#ifdef FEATURE_RF_HAS_WTR3925
/* FBRx Power Threshold for Intra ULCA*/
#define RFLTE_CORE_TXPL_FBRX_PWR_THRESHOLD_INTRA_ULCA_DB10        -200

/* FBRx Timer for Non-conflict and No tune-away cases*/
#define RFLTE_CORE_TXPL_FBRX_TIMER_NO_TUNE_AWAY_TIMER              50

/* FBRx Timer for conflict/tune-away cases*/
#define RFLTE_CORE_TXPL_FBRX_TUNE_AWAY_TIMER                       100
#endif

/* Macro to indicate the power to be subtracted per carrier while calulating
** Equal powers from a given combined total power */
#define RFLTE_CORE_TXPL_TX_PWR_CONSTANT                    30

/* The below two macros will be used irrespective of the FEATURE_RF_ASDIV feature being defined or not */
/* ASDiv default antenna position */
#define DEFAULT_ANT_POS                           0

/* ASDiv max number of positions */
#define ASD_POSITION_MAX                          2

/* max value for RB block per carrier, currently max bandwidth per carrier is 20M, so the max RB num is 100*/
#define RFLTE_CORE_TXPL_MAX_RB_NUM_PER_CARRIER    100

/*===========================================================================

  MACROS for TXAGC to DBM10 conversion and DBM10 to TXAGC conversion

===========================================================================*/
#define RFLTE_GET_PWR_IN_DBM10_FROM_TXAGC_COUNT(txagc) \
        ( txagc - RFLTE_CORE_TXPL_TXAGC_COUNT_MIN_OFFSET);

/*=========================================================================*/

#define RFLTE_GET_TXAGC_COUNT_FROM_PWR_IN_DBM10(pwrdbm10) \
        ( pwrdbm10 + RFLTE_CORE_TXPL_TXAGC_COUNT_MIN_OFFSET);

/*=========================================================================*/

/* Structure Definitions */
/*--------------------------------------------------------------------------*/
typedef struct
{
    uint16 addr; /*only 8bits for now*/
    uint8 value;
} rflte_core_txpl_hw_rd_type;

typedef struct
{
    rflte_core_txpl_hw_rd_type hdet_rd[RFLTE_CORE_TXPL_HDET_RD_RESULT_NUM_TRANS];
    uint8 num_trans;
} rflte_core_txpl_hdet_rd_result_type;

/* Strucuture to contain cluster RB info */
typedef struct
{
   /* Number of active clusters  */
   uint8       num_of_active_clusters[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];

   /* All Slot Starting Resource Block value */
   uint8 rb_start[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED][RFLM_LTE_MAX_SLOT][RFLM_LTE_TXAGC_MAX_NUM_CLUSTERS_PER_CELL];

   /* RB Block Size for all slots within the Sub Frame for each Tx Cell */
   uint8 rb_block[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED][RFLM_LTE_MAX_SLOT][RFLM_LTE_TXAGC_MAX_NUM_CLUSTERS_PER_CELL];
  
}rflte_core_ul_rb_cluster_input_s;

typedef struct
{
  /* Active carrier index bitmask.
  ** ML1 will populate this bitmask with the active carriers.
  ** LSB being PCELL and the incrementing bit position hosts the next carrier */
  uint32 active_carr_idx_btmsk;
  
  /* Array of active TxLM buffer indexes */
  lm_handle_type txlm_buffer_idx[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
  
}rflte_core_txpl_switch_txlm_buf_input_param;

/* NS_x type */
typedef enum
{
  RFLTE_CORE_TXPL_NS_XX = 0, 
  RFLTE_CORE_TXPL_CA_NS_XX = 1,
  RFLTE_CORE_TXPL_CA_NC_NS_XX = 2,
  RFLTE_CORE_TXPL_NS_TYPE_INVALID,
}rflte_core_txpl_ns_x_type;

/* The entire array needs to be
   looked upon the carr idx bmsk passed by ML1 */
typedef struct
{
  /* Carrier Index bitmask for which this request needs to be 
  ** processed from ML1. Each SET bit indicates the active
  ** carrier.
  ** Ex: 0000 0011 indicates that Primary and Secondary Carrier
  **     Components are active. */
  uint8  carrier_index_bmsk;

  /*  Current carrier configuration */
  rfa_rf_lte_ul_carrier_config_type_e curr_ul_carrier_config;

  /* Current TxLM buffer Indexes for PCC and SCC */
  lm_handle_type txlm_buffer_idx[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];

  /* Sub-Frame number for which the MTPL and SRS Values are to be
  ** provided to FED (value ranges between 0-9) */

  /* Action subframe number: srs_subframe_num */
  uint8 subframe_num[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];//only idx 0

  /* All Slot Starting Resource Block value. To-Be deprecated when cluster_info is populated */
  uint8 rb_start[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED][RFLM_LTE_MAX_SLOT];
  
  /* RB Block Size for all slots within the Sub Frame for each Tx Cell. To-Be deprecated when cluster_info is populated */
  uint8 rb_block[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED][RFLM_LTE_MAX_SLOT];
  
  /* Used for Stand-alone UpLink Type of NS requested for the Sub Frame 
     Will be deprecated when ML1 changes are available with below new interface */
  uint8 ns_x[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];

  /* NS type and the corresponding value */
  rflte_core_txpl_ns_x_type  ns_type[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
  uint8                      ns_value[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];

  /* P_emax for the subframe */
  int16 p_emax[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
  /* Pe_Max for the SRS Sub frame */
  /* Co-Ex Max for the subframe */
  
  int16 cxm_pmax[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
  /* If PHR reporting is enabled, return MTPL including 
  ** CxM power limit. Otherwise, ie. PHRLess == TRUE, 
  ** return MTPL disregarding the CxM power limit */
  
  boolean phrless_flag[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
  /* TRUE  : If CA NS Back-off has been requested by Network
  ** FALSE : If CA NS Back-off has not been requested by Network */
  
  /* Use only idx 0*/
  uint8 ca_ns_x[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED]; 

  /* Compute reference pcmax as requested by ML1 */
  boolean compute_ref_pcmax[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
  /* Type of UL modulation */
  rflm_lte_mod_type_s modulation_type[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
  
  /* Indicate active slots in current subframe */
  boolean slot_active[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED][RFLM_LTE_MAX_SLOT]; 

  /* Contains start_rb and num_rb information for all clusters per slot per cell */
  rflte_core_ul_rb_cluster_input_s cluster_info;

} rflte_core_compute_mtpl_input_param_s;

/* The entire array needs to be looked upon the carr idx bmsk passed by ML1 */
typedef struct
{
  /* Final calculated MTPL for current antenna position if ASDiv feature is defined (unit: dB10) */
  int16 final_mtpl[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED][RFLM_LTE_MAX_SLOT];

  /* Final calculated DTPL for current antenna position if ASDiv feature is defined (unit: dB10) */
  int16 final_dtpl[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED][RFLM_LTE_MAX_SLOT];

  /* Final calculated Power headroom (unit: dB10) */
  int16 phr_backoff_db10[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED]; 

  /* Max allowed power based on UE class (unit: dB10) */
  int16 tx_max_ue_class_pwr; 

  /* MTPL calculated for two antenna positions: ASD position Index 0 corresponds to MTPL for default position 
     and position Index 1 corresponds to MTPL for swap position */
  int16 ant_pos_based_mtpl[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED][RFLM_LTE_MAX_SLOT][ASD_POSITION_MAX];

  /* DTPL calculated for two antenna positions: ASD position Index 0 corresponds to DTPL for default position 
     and position Index 1 corresponds to DTPL for swap position */
  int16 ant_pos_based_dtpl[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED][RFLM_LTE_MAX_SLOT][ASD_POSITION_MAX];

} rflte_core_compute_mtpl_output_param_s;

/* 2D Lin vs temp vs freq P_in and P_out comp structure */
typedef enum
{
  RFLTE_CORE_TXPL_LEGACY_COMP = 0,
  RFLTE_CORE_TXPL_XPT_P_IN = 1,
  RFLTE_CORE_TXPL_XPT_P_OUT = 2,
}rflte_core_txpl_linearizer_comp_type;


typedef enum
{
    RFLTE_CORE_NLIC_WB = 1,
    RFLTE_CORE_NLIC_NB,
    RFLTE_CORE_NLIC_WB_NB,
    RFLTE_CORE_NLIC_NONE,
}rflte_core_nlic_wb_nb_type;


/* L1 External */
/*--------------------------------------------------------------------------*/
uint16 rflte_core_txpl_get_dac_setpoint (uint32 carrier_idx, rfcom_lte_band_type band_dac_setpoint);
/*--------------------------------------------------------------------------*/
int16 rflte_core_txpl_get_actual_vs_digital_tx_pwr_delta (void);
/*--------------------------------------------------------------------------*/
int16 rflte_core_txpl_get_sar(void);
/*--------------------------------------------------------------------------*/
int16 rflte_core_txpl_get_sar_pcmax_offset(void);
/*--------------------------------------------------------------------------*/
void rflte_core_compute_p_cmax_update(uint32 carrier_idx,
                                      uint8 subframe_num,
                                      rfa_rf_lte_txpl_params_s *tpl_params,
                                      uint8 pa_swpt_backoff,
                                      boolean intra_ulca_active_flag,
                                      lm_handle_type txlm_buffer_idx[RFA_RF_LTE_MAX_CELL_SUPPORTED]);

/*--------------------------------------------------------------------------*/
void rflte_core_compute_mtpl_per_sf(rflte_core_compute_mtpl_input_param_s   *input_params,
                                    rflte_core_compute_mtpl_output_param_s  *output_params);
/*--------------------------------------------------------------------------*/
void rflte_core_txpl_switch_txlm_buf( rflte_core_txpl_switch_txlm_buf_input_param* input_param);
/*--------------------------------------------------------------------------*/

/* RFA internal */
/*--------------------------------------------------------------------------*/
void rflte_core_txpl_init(uint32 carrier_idx, lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/

void rflte_core_txpl_enable_v2(uint32 carrier_idx,boolean enable, 
                               rfcom_lte_earfcn_type tx_channel, 
                               lm_handle_type txlm_buffer_idx);

/*--------------------------------------------------------------------------*/
void rflte_core_txpl_set_hdet_low_power_mode (uint32 carrier_idx, 
                                              boolean enable, 
                                              lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
void rflte_core_txpl_set_mtpl_for_sg(uint32 carrier_idx,
                                     lm_handle_type txlm_buffer_idx, uint32 plimit_mask,
                                     int16 sar_limit_dB10, int16 coex_limit_dB10,
                                     int16 wcn_limit_dB10, int16 subframe_coex_limit_dB10,
                                     int16 vbatt_limit_dB10, int16 pa_thermal_limit_dB10);

/*--------------------------------------------------------------------------*/
void rflte_core_txpl_do_mtpl_fw_reads_hdet_phy_rb(uint32 carrier_idx,
                                                 int16 txpl_txagc_val, 
                                                 uint16 hdet_adc,
                                                 uint8 phy_rb_block,
                                                 uint8 phy_start_rb,
                                                 rflte_core_txpl_hdet_rd_result_type hdet_rd_event_result);  

/* This API will support the FBRx MTPL update */
void rflte_core_txpl_do_fbrx_based_mtpl_update(uint8 carrier_idx, 
                                               int16 txpl_txagc_val, 
                                               int16 fbrx_gain_err,
                                               uint8 phy_rb_block,
                                               uint8 phy_start_rb,
                                               uint8 sys_bw );  
/*--------------------------------------------------------------------------*/
int32 rflte_core_txpl_get_mtpl_for_logging(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
int16 rflte_core_txpl_get_sar_limit_for_logging(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
uint16 rflte_core_txpl_get_hdet_adc_for_logging(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
int16 rflte_core_txpl_get_hdet_for_logging(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
uint8 rflte_core_txpl_get_mpr_for_logging(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
uint8 rflte_core_txpl_get_ampr_for_logging(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
uint8 rflte_core_txpl_get_ns_flag_for_logging(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
uint8 rflte_core_txpl_get_hdet_mode_for_logging(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
boolean rflte_core_txpl_set_dac_setpoint(uint32 carrier_idx, 
                                         uint16 dac_setpoint_override_enable, 
                                         uint16 dac_setpoint_value);
/*--------------------------------------------------------------------------*/
void rflte_core_txpl_set_sar_limit_index(lm_handle_type txlm_buffer_idx, uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
uint16 rflte_core_txpl_get_pdet_mode_swpt(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
boolean rflte_core_txpl_get_low_power_mode_enabled(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
int16 rflte_core_txpl_get_pcmax(void);
/*--------------------------------------------------------------------------*/
uint16 rflte_core_txpl_get_hdet_threshold(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
#ifdef FEATURE_RF_HAS_WTR3925
void rflte_core_txpl_get_fbrx_params(rfa_rf_lte_fbrx_input_params_s* fbrx_input,
                                     rfa_rf_lte_fbrx_output_params_s* fbrx_output);
#endif
/*--------------------------------------------------------------------------*/
uint8 rflte_core_txpl_get_lim_vs_freq_rb_block(void);
/*--------------------------------------------------------------------------*/
void rflte_core_txpl_update_param(uint32 carrier_idx, 
                                  rfa_rf_lte_txpl_params_s *txpl_params, 
                                  lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
int16 rflte_core_txpl_get_min_tx_power_nv(uint32 carrier_idx, 
                                          rfcom_lte_band_type rfcom_lte_band);
/*--------------------------------------------------------------------------*/
int16 rflte_core_txpl_get_nv_txpl(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
boolean rflte_core_txpl_set_mtpl_for_coex_wlan( int16 coex_limit_db10);
/*--------------------------------------------------------------------------*/
int32 rflte_core_xpt_path_delay_comp(uint32 carrier_idx,
                                     rfcom_lte_earfcn_type tx_channel,
                                     rfcom_lte_bw_type bw,
                                     uint8 rb_start,
                                     uint8 rb_block,
                                     boolean delav_vs_temp_comp_enabled,
                                     boolean ftm_mode_char_enabled,
                                     uint16 therm_scaled_read);
/*--------------------------------------------------------------------------*/
void rflte_core_txpl_set_tdd_config_backoff(rfa_rf_lte_tdd_ul_dl_cfg_type tdd_ul_dl_cfg, 
                                            uint32 carrier_idx, 
                                            lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
void rflte_core_txpl_set_sar_tdd_cfg_backoff( uint16 tdd_config_limit_dB10, 
                                              lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
uint16 rflte_core_txpl_get_sar_tdd_cfg_backoff(uint32 carrier_idx);
/*--------------------------------------------------------------------------*/
int16 rflte_core_txpl_get_rc_proc_var(void);
/*--------------------------------------------------------------------------*/
void rflte_core_txpl_set_rc_proc_var(int16 rc_proc_var);
/*--------------------------------------------------------------------------*/
void rflte_core_pwr_trking_loop_timer_update(uint32 carrier_idx, 
                                             uint32 loop_period, 
                                             lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
void rflte_core_def_tx_pwr_trking_loop_timer(uint32 carrier_idx, 
                                             lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
void rflte_core_undef_tx_pwr_trking_loop_timer(uint32 carrier_idx, 
                                               lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
uint32 rflte_core_get_pwr_trking_loop_timer(uint32 carrier_idx, 
                                            lm_handle_type txlm_buffer_idx);
/*--------------------------------------------------------------------------*/
uint16 rflte_core_txpl_get_fbrx_gain_err_for_logging(uint32 carrier_idx);

/*--------------------------------------------------------------------------*/
void rflte_core_txpl_pwr_trking_threshold(uint32 carrier_idx,
                                          lm_handle_type txlm_buffer_idx,
                                          boolean mtpl_enabled,
                                          int16 *pwr_trking_threshold);

uint8 rflte_core_get_txpl_mode(uint32 carrier_idx, 
                               lm_handle_type txlm_buffer_idx);

/*--------------------------------------------------------------------------*/


typedef enum
{
  RFLTE_CORE_TXPL_HDET_LPM_MODE,
  RFLTE_CORE_TXPL_HDET_HPM_MODE,
}rflte_core_txpl_hdet_pwr_type;


typedef enum
{
  RFLTE_CORE_TXPL_NS_01 = 1,
  RFLTE_CORE_TXPL_NS_02 = 2,
  RFLTE_CORE_TXPL_NS_03 = 3,
  RFLTE_CORE_TXPL_NS_04 = 4,
  RFLTE_CORE_TXPL_NS_05 = 5,
  RFLTE_CORE_TXPL_NS_06 = 6,
  RFLTE_CORE_TXPL_NS_07 = 7,
  RFLTE_CORE_TXPL_NS_08 = 8,
  RFLTE_CORE_TXPL_NS_09 = 9,
  RFLTE_CORE_TXPL_NS_10 = 10,
  RFLTE_CORE_TXPL_NS_11 = 11,
  RFLTE_CORE_TXPL_NS_12 = 12,
  RFLTE_CORE_TXPL_NS_13 = 13,
  RFLTE_CORE_TXPL_NS_14 = 14,
  RFLTE_CORE_TXPL_NS_15 = 15,
  RFLTE_CORE_TXPL_NS_16 = 16,
  RFLTE_CORE_TXPL_NS_17 = 17,
  RFLTE_CORE_TXPL_NS_18 = 18,
  RFLTE_CORE_TXPL_NS_19 = 19,
  RFLTE_CORE_TXPL_NS_20 = 20,
  RFLTE_CORE_TXPL_NS_21 = 21,
  RFLTE_CORE_TXPL_NS_22 = 22,
  RFLTE_CORE_TXPL_NS_23 = 23,
  RFLTE_CORE_TXPL_NS_24 = 24,
  RFLTE_CORE_TXPL_NS_25 = 25,
  RFLTE_CORE_TXPL_NS_26 = 26,
  RFLTE_CORE_TXPL_NS_27 = 27,
  RFLTE_CORE_TXPL_NS_28 = 28,
  RFLTE_CORE_TXPL_NS_29 = 29,
  RFLTE_CORE_TXPL_NS_30 = 30,
  RFLTE_CORE_TXPL_NS_31 = 31,
  RFLTE_CORE_TXPL_NS_32 = 32,
  RFLTE_CORE_TXPL_NS_INVALID = 0xFF,
} rflte_core_txpl_ns_type;

/* Aggregated Tx BW */
typedef enum
{
  RFLTE_CORE_TXPL_AGG_BW_25RB_100RB  = 0,
  RFLTE_CORE_TXPL_AGG_BW_50RB_100RB  = 1,
  RFLTE_CORE_TXPL_AGG_BW_75RB_75RB   = 2,
  RFLTE_CORE_TXPL_AGG_BW_75RB_100RB  = 3,
  RFLTE_CORE_TXPL_AGG_BW_100RB_100RB = 4,
  RFLTE_CORE_TXPL_AGG_BW_INVALID,
}rflte_core_txpl_cont_ca_agg_bw_type;

/* Uplink CA configuration type */
typedef enum
{
  RFLTE_CORE_TXPL_CA_1C  = 0,
  RFLTE_CORE_TXPL_CA_7C  = 1,
  RFLTE_CORE_TXPL_CA_38C = 2,
  RFLTE_CORE_TXPL_CA_39C = 3,
  RFLTE_CORE_TXPL_CA_41C = 4,
  RFLTE_CORE_TXPL_CA_42C = 5,
  RFLTE_CORE_TXPL_CA_INVALID,
}rflte_core_txpl_ca_config_type;

/* Aggregated Tx BW */
typedef enum
{
  RFLTE_CORE_TXPL_BW_CLASS_A = 0,
  RFLTE_CORE_TXPL_BW_CLASS_B = 1,
  RFLTE_CORE_TXPL_BW_CLASS_C = 2,
  RFLTE_CORE_TXPL_BW_CLASS_D = 3,
  RFLTE_CORE_TXPL_BW_CLASS_E = 4,
  RFLTE_CORE_TXPL_BW_CLASS_F = 5,
  RFLTE_CORE_TXPL_BW_CLASS_INVALID,
}rflte_core_txpl_bw_class_type;

/* Structure to hold the power backoff info */
typedef struct rflte_core_power_backoff_db10_info
{
/* To hold the slot1 backoff value */
  uint8 slot1_backoffvalue;

/* To hold the slot2 backoff value */
  uint8 slot2_backoffvalue;

/* To hold the final backoff value that will be applied per subframe */
  uint8 backoffvalue;
  
}rflte_core_power_backoff_db10_info_type;

typedef struct 
{
    uint8 sub_frame;
    int16 tx_power;
    uint8 num_rbs;
    uint8 start_rb_index;
    
    /*   For ULCA      */
    int16 ul_tx_power[RFLTE_MC_MAX_CELL_IDX_SUPPORTED];
    uint8 ul_num_rbs[RFLTE_MC_MAX_CELL_IDX_SUPPORTED];
    uint8 ul_start_rb_index[RFLTE_MC_MAX_CELL_IDX_SUPPORTED];
    int32 rx_freq_error[RFLTE_MC_MAX_CELL_IDX_SUPPORTED];
    rflte_core_nlic_wb_nb_type wb_nb;
    lm_handle_type txlm_buffer_idx[RFLTE_MC_MAX_CELL_IDX_SUPPORTED];
} nlic_input_params_per_sf;


typedef struct 
{
    boolean nlic_en[3];
    uint8 victim_id;
    rflte_mc_nlic_clock_mode_type clock_mode;
} nlic_return_params_per_sf;

typedef struct 
{
   boolean imd_backoff_possible;
   uint8   imd_backoff_num_of_nv_items;

   rfcom_lte_band_type pcc_tx_band;
   rfcom_lte_band_type scc_tx_band;

   uint16    I_max;
   int32    alpha_db;

   uint16     slope1_db8; // this will be a db8 number to increase resolution and avoid division
   uint16     slope2_db10;

   uint16     discont_th1;
   uint16     discont_th2;

   uint16    extra_desense1;
   uint16    extra_desense2;
   
} rflm_imd_backoff_params_type;

/*--------------------------------------------------------------------------*/
void rflte_core_txpl_update_nlic_per_sf(nlic_input_params_per_sf *nlic_input_info, 
                                        nlic_return_params_per_sf *nlic_return_info);

/*--------------------------------------------------------------------------*/
void rflm_nlic_get_imd_backoff_params(rflm_imd_backoff_params_type *imd_backoff_input_params);

/*----------------------------------------------------------------------------*/

#endif /*FEATURE_LTE*/
#endif /* RFLTE_CORE_TXPL_H */
