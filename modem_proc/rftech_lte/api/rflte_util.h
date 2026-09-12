/*! 
 @file rflte_util.h 
 
 @brief 
  This is the header file for utility functions for the LTE technology. 
 
 @details 
 
*/ 
  
/*=========================================================================== 
  
Copyright (c) 2013 - 2020 by Qualcomm Technologies, Inc.  All Rights Reserved. 
  
                           EDIT HISTORY FOR FILE 
  
This section contains comments describing changes made to this file. 
Notice that changes are listed in reverse chronological order. 

$Header: 
       modem_proc\rftech_lte\api\rflte_util.h
  
When       who     what, where, why 
-------------------------------------------------------------------------------
03/12/20   ssl     Added debug_msg_enums
01/27/20   ssl     [CR2594829]Added enum RF_LTE_ENABLE_TIME_PROFILE_MASK
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
10/30/17   pv      Unified band mask support to Policyman
10/26/17   sk      Enable LTE B71 support
08/02/17   ks      Variance id changes
05/03/15   vkm     B66 UL_EARFCN offset is set
05/03/16   vkm     NS support on Band 66: passing right sys band for verfication
04/29/16   sk      LTE VSWR support
03/01/16   mah      Added rflte_util_get_integer_band_from_rfcom_band() as an
                   easier to use variation (no out parameter) of
                   rflte_util_rfcom_band_to_integer_band()
01/14/16   sg      Add API to calculate frequency of out of band emissions
12/08/15   gvn     Static NV re-structuring
11/25/15   am      asm trigger update for tdd split bands.
11/18/15   pkg     Move Tx and FBRx Cal Chan DB to DM.
11/13/15   stw     BW type to BW Hz conversion
10/28/15   ska     Adding support for successive same band cal with different modem chains
10/15/15   vkm     Reverting FR30081
10/14/15   mah      Changes to use correct variables passed in for Tx Tune.
10/12/15   mah      Fix HDET correction for Intra-band ULCA MTPL
10/01/15   kg      Generalize the rx_op enum to account for tx rfm_devices
09/30/15   kab     Add utility API to count number of bits in the bitmask
09/28/15   mg      Utility API to get RFLTE path from carrier type
09/28/15   stw     FR28012 :Alternate Path Used to find correct Split Band from RFC for Rx tuning
09/25/15   sb      FR30081:Apply gain offsets for ASDIV switch position change
09/16/15   npi     Utility API to gets sys band from Tx EARFCN
09/15/15   ska      Adding NS_16 B27 support
08/27/15   bsh     Support checking alternate path when adding/removing cells
07/20/15   pkg     Changes to support FR 24616:Algorithm to mitigate VBatt
                   droop (down to 2.5V) via TX backoff.
07/20/15   mg      WTR Interface Changes -  API to return current Rx operation 
                    mode based on carrier type
07/08/15    JJ     Inter_band FBRx support in rftech_lte
06/26/15   jd      Wtr hopping phase1.
06/25/15   pkg     Changes to support Intra ULCA Tune on bands with splits.
06/17/15   stw     LTE_U : Fix for Band 252 , 255 : Interger Band to RF COM BAND conversion
04/30/13    sk     [Phase-2] Re-structure Tx Path specifc NV's per Tx NV container
04/21/15    sk     Re-structure Tx path specific NV's per Tx NV container
04/21/15    sk     Inter-band UL CA support
04/17/15   pkg     Tx Rx Split feature changes part 4. 
01/14/14   ska     F3 messages cleanup.Defined Rx Debug Mask by replacing Freq Comp Debug Mask.
12/30/14   JJ      Added api to support intra_band ulca
12/05/14   dw      Changes to support split bands based on split specified in RFC.
12/02/14   yzw     Add utility APIs for DL only chan and band
11/04/14   gvn     Add utility api to switch port A\B
10/29/14   stw     Added APIs rflte_util_get_sub_band and rflte_util_get_full_band
10/02/14   kab     Support for Single Rx RXLM Config Mode Selection.
09/02/14   php     Use Char channel list for Pin compensation
07/31/14   ndb     Added support for extended EARFCN for LTE
06/18/14   sbm     Added api to determine if hdet Nv is present or not.
06/04/14    pv     Added support for FBRx RB Based Freq comp.
04/17/14   svi     PA static NV and code redesign
04/11/14   bsh     Split Band Support
03/14/14   bsh     NlIC changes - move ftl calculation to NLIC component 
03/12/14   bsh     Adding NLIC support - calcualte ftl based on rx freq error
03/13/14   svi     Fix UL DL EARFCN Offset for LTE
02/04/14    pv     Added support for FBRx Gain error processing.
08/31/13   bsh     Initial version. 
  
============================================================================*/ 
#ifndef RFLTE_UTIL_H
#define RFLTE_UTIL_H

#include "appmgr.h"
#include "msgr.h"
#include "rfa_msgr.h"
#include "rfcom.h"

#include "rfm_device_config_type.h"
#include "rflte_ext_mc.h"
#include "rflte_nv.h"

/*! FBRx Related Includes */
#include "rfcommon_fbrx_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LTE_IQ_BUFFER_SIZE  16384  /*hardcoded for 16KB*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Band and NS dependent AMPR constant definitions
 
*/
/* B23 - NS11: Table 6.2.4-5 36.101 (Release 11.2) */
#define RFLTE_UTIL_B23_NS_11_LOWER_FREQ_BOUND_KHZ  2000000
#define RFLTE_UTIL_B23_NS_11_UPPER_FREQ_BOUND_KHZ  2012500

/* B26 - NS15: Table 6.2.4-9\6.2.4-10 36.101 (Release 11) */
#define RFLTE_UTIL_B26_NS_15_LOWER_FREQ_BOUND_KHZ  845000
#define RFLTE_UTIL_B26_NS_15_UPPER_FREQ_BOUND_KHZ  849000

/*B27 NS 16 Table 6.2.4-11, 6.2.4-12, 6.2.4-13 */
#define RFLTE_UTIL_B27_NS_16_LOWER_FREQ_BOUND_KHZ 807000
#define RFLTE_UTIL_B27_NS_16_MID_FREQ_BOUND_KHZ 808500
#define RFLTE_UTIL_B27_NS_16_HIGH_FREQ_BOUND_KHZ 812000

/* Size of the RB in KHZ  Please refer the TS 32.101 V11.2.0 (2012-09)*/

#define RFLTE_UTIL_RB_IN_KHZ 180

/*Please refer the table 6.2.2-1 Note 2. Bandedge in Khz where the maxpower can be relaxed*/
#define RFLTE_UTIL_BAND_EDGE_LIMIT_IN_KHZ 4000

/*Absolute value macro*/
#define RFLTE_UTIL_ABS(x)               (((x) < 0) ? -(x) : (x))
/*Round Macro*/
#define RFLTE_UTIL_FLOOR(x,y)           rflte_util_floor(x, y)
/*Round value macro*/
#define RFLTE_UTIL_ROUND(x,y)           RFLTE_UTIL_FLOOR(((2 * (x)) + (y)), (2 * (y)))
/* Saturate Macro: This is used for clipping the results of adjustment
  calculations where the newly calculated value may exceed the legal range
  for the control.*/
#define RFLTE_UTIL_SATURATE( val, min, max )  MAX( MIN( (val), (max) ), (min) )
/*Sign Macro*/
#define RFLTE_UTIL_SIGN(x)              (((x) < 0) ? -1 : 1)
#define RFLTE_UTIL_INT(x)    ((int)x)
#define RFLTE_UTIL_FRAC(x,y) ( RFLTE_UTIL_ABS((int)((float)(((float)x) - ((int)x))*y)) )

#define RFLTE_UTIL_MIN(val1,val2,val3,val4,val5,val6)   MIN( MIN(val1,val2),MIN(val3,MIN(val4,MIN(val5,val6))))

#define RFLTE_UTIL_MIN_3(val1,val2, val3) MIN(val3, MIN(val1,val2))

/*----------------------------------------------------------------------------*/
/*!
  @brief
  FDD UL and DL EARFCN Offset Macros
 
*/
/*!> For FDD Bands between B1 to B28 */
#define RFLTE_UTIL_FDD_DL_UL_EARFCN_OFFSET_B1_TO_B28 18000

/*!> For FDD Bands  B30 and B31 */
#define RFLTE_UTIL_FDD_DL_UL_EARFCN_OFFSET_B30_B31   17890

#define RFLTE_UTIL_FDD_DL_UL_EARFCN_OFFSET_B71   64536

/*!> For FDD Bands  B66 */
#define RFLTE_UTIL_FDD_DL_UL_EARFCN_OFFSET_B66   65536


/*----------------------------------------------------------------------------*/
/*!
  @brief
  TDD UL and DL EARFCN Offset Macros
 
*/
#define RFLTE_UTIL_TDD_DL_UL_EARFCN_OFFSET           0


/*----------------------------------------------------------------------------*/


typedef enum
{
  RF_LTE_DISABLED_DEBUG_MASK = 0x0,  
  RF_LTE_LEGACY_ENABLE_ALL_MASK = 0x1,  
  RF_LTE_TX_PLL_ERROR_CORRECTION_DEBUG_MASK = 0x2,
  RF_LTE_SMPS_PDM_INTERPOLATE_DEBUG_MASK = 0x4,
  RF_LTE_TEMP_COMP_DEBUG_MASK = 0x8,
  RF_LTE_MTPL_DEBUG_MASK = 0x10,
  RF_LTE_RX_DEBUG_MASK = 0x20,  
  RF_LTE_IRAT_DEBUG_MASK = 0x40,
  RF_LTE_MSG_DISPATCH_DEBUG_MASK = 0x80,
  RF_LTE_RX_ENABLE_CRASH_REG_DUMP = 0x16,
  RF_LTE_ENABLE_TIME_PROFILE_MASK = 0x06,
  RF_LTE_ENABLE_HEAP_PROFILE_MASK = 0x08,
  RF_LTE_ENABLE_RXAGC_MASK = 0x0A,
  
} rf_lte_debug_msg_enums;

/*----------------------------------------------------------------------------*/
rfcom_lte_earfcn_type rflte_util_get_uarfcn_from_rx_freq(uint32 rx_freq, rfcom_lte_band_type band);

/*----------------------------------------------------------------------------*/
rfcom_lte_earfcn_type rflte_util_get_uarfcn_from_tx_freq(uint32 tx_freq, rfcom_lte_band_type band);

/*----------------------------------------------------------------------------*/
void rflte_util_get_tx_freq_lo_hi_from_band(rfcom_lte_band_type band, uint32 *f_ul_low, uint32 *f_ul_hi);

/*----------------------------------------------------------------------------*/
rfcom_lte_band_type rflte_util_get_band_from_rx_uarfcn(rfcom_lte_earfcn_type rx_chan);

/*----------------------------------------------------------------------------*/
rfcom_lte_band_type rflte_util_get_band_from_tx_uarfcn(rfcom_lte_earfcn_type tx_chan);

/*----------------------------------------------------------------------------*/
rfcom_lte_band_type
rflte_util_integer_band_to_rfcom_band(
  uint16 integer_band
);

/*----------------------------------------------------------------------------*/
uint32 rflte_util_get_tx_freq_from_tx_chan(rfcom_lte_earfcn_type tx_chan);

/*----------------------------------------------------------------------------*/
void rflte_util_get_tx_freq_lo_hi_from_rb(rfcom_lte_earfcn_type tx_chan, 
                                          rfcom_lte_bw_type tx_bw,
                                          uint8 rb_start, 
                                          uint8 rb_block, 
                                          uint32 *f_ul_low, 
                                          uint32 *f_ul_hi);

/*----------------------------------------------------------------------------*/
rflte_nv_tbl_type *rflte_util_init_nv_table(rfcom_lte_band_type band);

/* ----------------------------------------------------------------------- */
rf_card_band_type  rflte_util_lte_band_helper(rfcom_lte_band_type rfcom_band);

/* ----------------------------------------------------------------------- */
rfcom_lte_band_type rflte_util_convert_rfc_band_to_rfcom_band(rf_card_band_type rfc_band);

/* ----------------------------------------------------------------------- */
rflte_nv_supported_band_type rflte_util_convert_rfcom_band_to_rfnv_band(rfcom_lte_band_type rfcom_band);
/* ----------------------------------------------------------------------- */
void rflte_util_get_tx_cal_boundaries(uint8 sub_id,tx_band_cal_type *tx_band_cal_data, uint32 tx_nv_container, rfcom_lte_band_type band, uint32 carrier_index, lm_handle_type txlm_buffer_idx);
/* ----------------------------------------------------------------------- */
void rflte_util_get_fbrx_cal_boundaries(uint32 carrier_idx, rfcom_lte_earfcn_type *fbrx_cal_chan,uint8 fbrx_cal_chan_size, lm_handle_type txlm_buffer_idx);
/* ----------------------------------------------------------------------- */
rfcommon_fbrx_tx_cfg_type rflte_util_get_fbrx_bw_for_sys_bw(rfcom_lte_bw_type bw);
/* ----------------------------------------------------------------------- */
#ifdef FEATURE_RF_HAS_WTR3925
uint32 rflte_util_get_fbrx_vswr_offset(rfcom_lte_bw_type bw);
#endif
/*----------------------------------------------------------------------------*/
uint8 
rflte_util_get_pa_range_from_pa_state
(
  uint8                  pa_state, 
  rflte_nv_tbl_type     *rflte_nv_tbl_ptr,
  uint32                 tx_nv_container
);

/*----------------------------------------------------------------------------*/
rfcom_lte_band_type rflte_util_get_first_band_from_mask(uint64 band_mask);

/*----------------------------------------------------------------------------*/
boolean rflte_util_is_band_tdd( rfcom_lte_band_type rf_band);

/*----------------------------------------------------------------------------*/
boolean rflte_util_is_hdet_nv_present(uint32 carrier_index, rflte_nv_tbl_type *rflte_nv_tbl_ptr, lm_handle_type txlm_buffer_idx);

/*----------------------------------------------------------------------------*/
rfcom_lte_earfcn_type rflte_util_get_rx_uarfcn_from_tx_uarfcn(rfcom_lte_earfcn_type tx_chan,rfcom_lte_band_type band);

/*----------------------------------------------------------------------------*/
rfcom_lte_earfcn_type rflte_util_get_tx_uarfcn_from_rx_uarfcn(rfcom_lte_earfcn_type rx_chan,rfcom_lte_band_type band);

/*----------------------------------------------------------------------------*/
rfnv_item_id_enum_type rflte_util_get_txlin_nvid(rfcom_lte_band_type band);

/*----------------------------------------------------------------------------*/
rfcom_lte_earfcn_type rflte_util_get_ul_arfcn_from_freq (uint32 freq);

/*----------------------------------------------------------------------------*/
rfcom_lte_band_type rflte_util_convert_nv_band_to_rfcom_band(rflte_nv_supported_band_type band);
/*----------------------------------------------------------------------------*/
boolean rflte_util_check_rfc_band_support(void);
/*----------------------------------------------------------------------------*/
rfnv_item_id_enum_type rflte_util_get_hdetVsAgc_nvid(uint32 tx_nv_container, rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/
uint16 rflte_util_get_num_rb_from_bandwidth(rfcom_lte_bw_type bw);
/*----------------------------------------------------------------------------*/
boolean rflte_util_get_min_max_rx_freqs(rfcom_lte_band_type band, uint32* min, uint32* max);
/*----------------------------------------------------------------------------*/
uint64 rflte_util_get_band_mask(rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/
void rflte_util_get_min_max_tx_freqs(rfcom_lte_band_type band,
                                   uint32* ful_low,
                                   uint32* ful_high);
/*----------------------------------------------------------------------------*/
boolean rflte_util_rfcom_band_to_integer_band(rfcom_lte_band_type band,uint16* num);
/*----------------------------------------------------------------------------*/
uint16 rflte_util_get_integer_band_from_rfcom_band(rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/
void rflte_util_get_fullband_from_subband(rfcom_lte_band_type* lte_rfcom_band);
/*----------------------------------------------------------------------------*/
uint32 rflte_util_get_tx_freq_from_uarfcn(rfcom_lte_earfcn_type tx_chan, rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/
uint16 rflte_util_get_bw_in_khz(rfcom_lte_bw_type bw);
/*----------------------------------------------------------------------------*/
rfcom_lte_bw_type rflte_util_get_bw_type(uint16 bwfreqz);
/*----------------------------------------------------------------------------*/
uint32 rflte_util_get_rx_freq_from_uarfcn(rfcom_lte_earfcn_type rx_chan, rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/
int32 rflte_util_floor(int32 dividend, int32 divisor);
/*----------------------------------------------------------------------------*/
int32 rflte_util_get_txagc_from_dbm10( int16 dbm_10);
/*----------------------------------------------------------------------------*/
int16 rflte_util_get_dbm10_from_txagc( int32 txagc);
/*----------------------------------------------------------------------------*/
rfcom_lte_earfcn_type rflte_util_get_ul_dl_earfcn_offset( rfcom_lte_band_type lte_band );
/*----------------------------------------------------------------------------*/
/* New APIs to support band detection from RFC based on channel, bw and path override index */
rfcom_lte_band_type 
rflte_util_get_band_from_rx_uarfcn_bw_path_override(rfcom_lte_band_type current_band,
                                                    rfcom_device_enum_type device,
                                                    rfcom_lte_earfcn_type rx_chan,
                                                    rfcom_lte_bw_type bw,
                                                    uint8 path_override_index,
                                                    uint8 alt_path);
/*----------------------------------------------------------------------------*/
rfcom_lte_band_type 
rflte_util_get_band_from_tx_uarfcn_bw_path_override(uint8                  sub_id,
                                                    rfcom_lte_band_type    current_band,
                                                    rfcom_lte_earfcn_type  tx_chan,
                                                    rfcom_lte_bw_type      bw,
                                                    uint8                  path_override_index);
/*----------------------------------------------------------------------------*/
rfcom_lte_bw_type rflte_util_get_rfcom_bw_type(uint16 rx_bw_calculated);
/*----------------------------------------------------------------------------*/
rfcom_lte_sub_band_type rflte_util_get_sub_band(rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/
rfcom_lte_band_type rflte_util_get_full_band(rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/
boolean rflte_util_is_dl_only_chan(rfcom_lte_earfcn_type chan);
/*----------------------------------------------------------------------------*/
boolean rflte_util_is_dl_only_band(rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/
uint64 rflte_util_get_splitbands_from_fullband(rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/
void rflte_util_get_freq_offset_from_tx_lo(uint8  sub_id,
                                           rfcom_lte_earfcn_type pcc_tx_chan,
                                           rfcom_lte_bw_type pcc_tx_bw,
                                           rfcom_lte_earfcn_type scc0_tx_chan,
                                           rfcom_lte_bw_type scc0_tx_bw,
                                           int32 *tx_lo_offset_in_KHz_for_pcc,
                                           int32 *tx_lo_offset_in_KHz_for_scc0);
/*----------------------------------------------------------------------------*/
boolean rflte_util_is_intra_band_ul_ca(uint8                  sub_id,
                                       rfcom_lte_band_type    band_pcell,
                                       rfcom_lte_band_type    band_scell,
                                       rfcom_lte_earfcn_type  tx_chan_pcell,
                                       rfcom_lte_earfcn_type  tx_chan_scell,
                                       rfcom_lte_bw_type      tx_bw_pcell,
                                       rfcom_lte_bw_type      tx_bw_scell,
                                       boolean                split_intra_ulca,
                                       rfcom_device_enum_type intra_ulca_dev);
/*----------------------------------------------------------------------------*/
rfcom_lte_earfcn_type rflte_util_get_intra_band_ul_ca_tx_chan(uint8                  sub_id,
                                                              rfcom_lte_earfcn_type  tx_chan_pcell,
                                                              rfcom_lte_earfcn_type  tx_chan_scell,
                                                              rfcom_lte_bw_type      tx_bw_pcell,
                                                              rfcom_lte_bw_type      tx_bw_scell);
/*----------------------------------------------------------------------------*/
rfcom_lte_earfcn_type rflte_util_dl_min_chan(rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/
boolean rflte_util_check_rx_dev_on_band(rfm_device_enum_type index,
                                        rfcom_lte_band_type rf_band);
/*----------------------------------------------------------------------------*/
boolean rflte_util_check_tx_dev_on_band(rfm_device_enum_type index,
                                        rfcom_lte_band_type rf_band);
/*----------------------------------------------------------------------------*/
boolean rflte_util_set_tx_dev_band_mask(rfm_device_enum_type device,
                                        rfcom_lte_band_type band );
/*----------------------------------------------------------------------------*/
boolean rflte_util_set_rx_dev_band_mask(rfm_device_enum_type device,
                                        rfcom_lte_band_type band );
/*----------------------------------------------------------------------------*/
rfcom_lte_band_type rflte_ftm_mc_convert_integer_band_to_rfcom_band(uint16 integer_band);
/*----------------------------------------------------------------------------*/
boolean rflte_util_get_is_split_band(rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/
uint8 rflte_util_get_atuner_rx_path(uint8 path_type);
/*----------------------------------------------------------------------------*/
rfdevice_rx_mode_type rflte_util_get_current_rx_oper_mode(uint16 path_type);
/*----------------------------------------------------------------------------*/
rfm_device_operation_type 
rflte_util_get_rfm_path_from_rflte_path(uint32 path_type);
/*----------------------------------------------------------------------------*/
sys_band_class_e_type 
rflte_util_get_sys_band_from_rx_uarfcn(rfcom_lte_earfcn_type channel);
/*----------------------------------------------------------------------------*/
sys_band_class_e_type 
rflte_util_get_sys_band_from_tx_uarfcn(rfcom_lte_earfcn_type channel);
/*----------------------------------------------------------------------------*/
rfdevice_rx_mode_type rflte_util_get_current_rx_oper_mode_v2 (uint16 path_type);
/*----------------------------------------------------------------------------*/
rflte_path_type rflte_util_get_path_from_carrier_type (uint16  carrier_type);
/*----------------------------------------------------------------------------*/
boolean rflte_util_get_tx_modem_chain(rfcom_lte_band_type band,rfcom_device_enum_type device, uint8 *current_modem_chain, uint32 alt_path);
/*----------------------------------------------------------------------------*/
uint8 rflte_util_count_bits_in_bitmask(uint32 bit_mask);
/*----------------------------------------------------------------------------*/
uint64 rflte_util_get_bw_hz_from_bw_type(rfcom_lte_bw_type bw);
/*----------------------------------------------------------------------------*/
boolean rflte_util_find_nv_container_in_static_struct_and_return_item_id
(
   uint32 nv_container_idx,
   rfnv_rx_static_internal_data_type *rx_static_internal_data,
   uint32 *item_id
);
/*----------------------------------------------------------------------------*/
boolean rflte_util_is_static_data_populated
(
   uint32 nv_item_id,
   uint8 switch_point_type,
   rfnv_rx_static_internal_data_type *rx_static_internal_data
);
/*----------------------------------------------------------------------------*/
uint32 rflte_util_get_f_oob_in_khz(uint8 sub_id,uint16 agg_bw_khz);
/*----------------------------------------------------------------------------*/

rfcom_lte_band_type rflte_util_sys_band_to_rfcom_band( sys_sband_lte_e_type band);

/*-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
boolean rflte_util_validate_sub_id(uint8 sub_id);
/*----------------------------------------------------------------------------*/

boolean rflte_util_get_sys_band_from_rfcom_band
(
  rfcom_lte_band_type rfcom_band,
  sys_band_class_e_type *sys_band
);


#ifdef __cplusplus
}
#endif

#endif

