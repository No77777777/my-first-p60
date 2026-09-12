/*! 
 @file rflte_core_util.h 
 
 @brief 
  This is the header file file for utility functions for the LTE technology. 
 
 @details 
 
*/ 
  
/*=========================================================================== 
  
Copyright (c) 2009 - 2020 by Qualcomm Technologies, Inc.  All Rights Reserved. 
  
                           EDIT HISTORY FOR FILE 
  
This section contains comments describing changes made to this file. 
Notice that changes are listed in reverse chronological order. 

$Header: //components/rel/rftech_lte.mpss/3.11/api/rflte_core_util.h#1 $
  
When       who     what, where, why 
------------------------------------------------------------------------------- 
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
10/26/17   sk      Enable LTE B71 support
01/14/16   sg      Add API to calculate frequency of out of band emissions
11/18/15   pkg     Move Tx and FBRx Cal Chan DB to DM.
08/18/15   avi     Moved #define/Constants to rflte_core_util.h from rflte_util.c
04/21/15    sk     Re-structure Tx path specific NV's per Tx NV container
04/21/15    sk     Inter-band UL CA support 
12/11/14   ars/sdu Added rflte_util_get_rx_freq_from_rx_chan() function
09/04/14   php     Change API to accept device as parameter
08/01/14   ndb     Added support for extended EARFCN for LTE
01/09/14   svi     Split Band Support : Tx path loss addition
08/12/13   bsh     Separate core and util functions
07/17/13   aca     Exported rflte_core_get_txagc_from_dbm10() & rflte_core_get_dbm10_from_txagc()
05/03/13   gvn     STG config for LTE RSB Cal
04/25/13   gvn     Support for Split bands for LTE
04/05/13   kg      Added API's to return rfcom type band mask and max/min
                   freq based on band
03/18/13   sbm     Defined debug NV mask for dispatch messages.
02/19/13   sbm     Bandedge max power backoff support in LTE(Nikel CR375880)
02/15/13   aca     API to convert Tx channel to Rx channel
02/05/13   kg      Added API's to return rfcom type band mask and max/min
                   freq based on band
01/17/13   gvn     Move some APIs to rflte_ext_mc.h since they are used by MCS team
12/06/12   gvn     Add helper function to convert LTE band from rfcom band to rfnv band
11/29/12   gvn     Add rflte_core_get_num_rb_from_bandwidth() (Merge from Nikel)
11/15/12   adk     Backed out Qtuner code
11/09/12   gvn     Add NS_11 support for channel bandwidth 15MHZ and 20MHZ(Merge from Nikel)
11/09/12   gvn     Merge from Nikel. Add new AMPR NVs support (multiple NS support per band) 
09/14/12   jf      Qtuner LTE support (initial)
09/07/12   sbm     Remove rflte_core_get_tx_cal_chan_freq()
09/07/12   sbm     nv tlb ptr changes for CA
06/17/12   pl      Update get NV interface to include device.
06/12/12   id      Add support to get HDET Vs AGC NV Item Id per tech and band 
05/23/12   ndb     Implementation of IM3 backoff for GPS desense
03/22/12   id      Added functions for 
                   1. Tx freq to chan conversion
                   2. Get Tx Multi Lin NV ID 
02/22/12   aca     Cleaned up LTE duplicate code for checking if a band is TDD
01/11/12   cri     Update to LTE Tx Frequency comensation algorithm
09/07/11   whc     New API to decouple PA State from PA Range  
08/18/11   pl      Added an interface to get tx_freq from just tx_channel
08/10/11   tnt     Initial Ant Tuner support  
06/23/11   pl      remove unused functions 
04/14/11   aca     Npler autogen support
04/01/11   bmg     Added extern "C" for C++ safety.
03/17/11   tnt     Add IQ capture functionality
03/02/11   tnt     Add rflte_core_convert_rfc_band_to_rfcom_band() 
01/05/11   pl      Add band info for gpio/grfc related interfaces
12/23/10   cri     Added SPI support for RAFT circuit
05/21/10   can     Added LTE TX RB based FREQ COMP support.  
05/06/10   tnt     Adding rflte_core_get_tx_freq_lo_hi_from_band
04/20/10   pl      Adding Band helper function 
04/19/10   qma     Disable RX function addition.
04/16/10   tnt     Disable TX function addition.
04/16/10   tnt     Adding support for run-time band change for PA_ON_SRC
03/12/10   tnt     Adding code to support GRFC for RF signals
08/25/09   qma     Added rflte_core_init_nv_table() to transfer rfcom LTE 
                   bands to rf nv LTE bands
08/07/09   can     New band and function addition for LTE.
04/16/09   can     Changes for integration. 
04/15/09   can     initial version for MDM9K. 
  
============================================================================*/ 
#ifndef RFLTE_CORE_UTIL_H
#define RFLTE_CORE_UTIL_H

#include "rflte_util.h"
#include "rflte_nv.h"
#include "rflte_core_txpl.h"
#include "rflte_core_iq_capture.h" 

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================================
         DEFINES FOR FUNCTIONS THAT ARE MOVED TO OTHER FILES      
==========================================================================*/	

/*This is required to allow any file ourside rftech lte to continue using 
  rflte_core_util. In future this will be removed*/

/*----------------------------------------------------------------------------*/
#define rflte_core_get_uarfcn_from_rx_freq(rx_freq, band) rflte_util_get_uarfcn_from_rx_freq(rx_freq, band)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_uarfcn_from_tx_freq(tx_freq, band) rflte_util_get_uarfcn_from_tx_freq(tx_freq, band)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_tx_freq_lo_hi_from_band(band, f_ul_low, f_ul_hi) rflte_util_get_tx_freq_lo_hi_from_band(band, f_ul_low, f_ul_hi)

/*----------------------------------------------------------------------------*/
rfcom_lte_band_type rflte_core_get_band_from_rx_uarfcn(rfcom_lte_earfcn_type rx_chan);

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#define rflte_core_get_tx_freq_from_tx_chan(tx_chan) rflte_util_get_tx_freq_from_tx_chan(tx_chan)

#define rflte_core_get_rx_freq_from_rx_chan(rx_chan) rflte_util_get_rx_freq_from_rx_chan(rx_chan)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_tx_freq_lo_hi_from_rb(tx_chan, tx_bw, rb_start, rb_block, f_ul_low, f_ul_hi) rflte_util_get_tx_freq_lo_hi_from_rb(tx_chan, tx_bw, rb_start, rb_block, f_ul_low, f_ul_hi)

/*----------------------------------------------------------------------------*/
#define rflte_core_init_nv_table(band) rflte_util_init_nv_table(band)

/* ----------------------------------------------------------------------- */
#define rflte_core_lte_band_helper(rfcom_band) rflte_util_lte_band_helper(rfcom_band)

/* ----------------------------------------------------------------------- */
#define rflte_core_convert_rfc_band_to_rfcom_band(rfc_band) rflte_util_convert_rfc_band_to_rfcom_band(rfc_band)

/* ----------------------------------------------------------------------- */
#define rflte_core_convert_rfcom_band_to_rfnv_band(rfcom_band) rflte_util_convert_rfcom_band_to_rfnv_band(rfcom_band)

/* ----------------------------------------------------------------------- */
#define rflte_core_get_tx_cal_boundaries(tx_band_cal_data, tx_nv_container, band, txlm_index) rflte_util_get_tx_cal_boundaries(tx_band_cal_data, tx_nv_container, band, carrier_index, txlm_index)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_pa_range_from_pa_state(pa_state, rflte_nv_tbl_ptr, tx_nv_container) rflte_util_get_pa_range_from_pa_state(pa_state, rflte_nv_tbl_ptr,tx_nv_container)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_first_band_from_mask(band_mask) rflte_util_get_first_band_from_mask(band_mask)

/*----------------------------------------------------------------------------*/
#define rflte_core_is_band_tdd(rf_band) rflte_util_is_band_tdd(rf_band)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_rx_uarfcn_from_tx_uarfcn(tx_chan, band) rflte_util_get_rx_uarfcn_from_tx_uarfcn(tx_chan, band)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_tx_uarfcn_from_rx_uarfcn(rx_chan, band) rflte_util_get_tx_uarfcn_from_rx_uarfcn(rx_chan, band)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_ul_arfcn_from_freq(freq) rflte_util_get_ul_arfcn_from_freq(freq)

/*----------------------------------------------------------------------------*/
#define rflte_core_convert_nv_band_to_rfcom_band(band) rflte_util_convert_nv_band_to_rfcom_band(band)

/*----------------------------------------------------------------------------*/
#define rflte_core_check_rfc_band_support(void) rflte_util_check_rfc_band_support(void)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_hdetVsAgc_nvid(band) rflte_util_get_hdetVsAgc_nvid(band)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_num_rb_from_bandwidth(bw) rflte_util_get_num_rb_from_bandwidth(bw)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_min_max_rx_freqs(band, min, max) rflte_util_get_min_max_rx_freqs(band, min, max)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_band_mask(band) rflte_util_get_band_mask(band)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_min_max_tx_freqs(band, ful_low, ful_high) rflte_util_get_min_max_tx_freqs(band, ful_low, ful_high)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_band_num(band, num) rflte_util_rfcom_band_to_integer_band(band, num)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_fullband_from_subband(lte_rfcom_band) rflte_util_get_fullband_from_subband(lte_rfcom_band)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_dbm10_from_txagc(txagc) rflte_util_get_txagc_from_dbm10(txagc)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_txagc_from_dbm10(dbm_10) rflte_util_get_dbm10_from_txagc(dbm_10)

/*----------------------------------------------------------------------------*/
uint32 rflte_core_get_rx_freq_from_uarfcn(rfcom_lte_earfcn_type rx_chan, rfcom_lte_band_type band);
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#define rflte_core_get_bw_in_khz(bw) rflte_util_get_bw_in_khz(bw)

/*----------------------------------------------------------------------------*/
#define rflte_core_get_f_oob_in_khz(sub_id,agg_bw_khz) rflte_util_get_f_oob_in_khz(sub_id,agg_bw_khz)

/*----------------------------------------------------------------------------*/
rfcom_lte_band_type rflte_core_get_band_from_tx_uarfcn(rfcom_lte_earfcn_type tx_chan);

/*----------------------------------------------------------------------------*/
uint32 rflte_core_get_tx_freq_from_uarfcn(rfcom_lte_earfcn_type tx_chan, rfcom_lte_band_type band);

/*----------------------------------------------------------------------------*/
int8 rflte_util_get_path_loss_offset(rfcom_lte_band_type band, rfcom_lte_earfcn_type chan, rflte_nv_tbl_type *lte_nv_tbl_ptr);

/*----------------------------------------------------------------------------*/
/*============================================================================= 
                             DATA DEFINITIONS 
=============================================================================*/

#define INVALID_UINT16 						65535

/*LTE channel raster in kHz.*/
#define RF_LTE_CHANNEL_RASTER_IN_KHZ 100
#define RF_LTE_U_BANDS_BW_COEFF_IN_KHZ        20000
#define RF_LTE_U_BANDS_REMAINDER_COEFF_IN_KHZ 100

/*LOWEST DL FREQUENCIES FOR EACH BAND IN kHz*/
#define LTE_B1_DL_LOW_FREQ          2110000
#define LTE_B2_DL_LOW_FREQ          1930000
#define LTE_B3_DL_LOW_FREQ          1805000
#define LTE_B4_DL_LOW_FREQ          2110000
#define LTE_B5_DL_LOW_FREQ          869000
#define LTE_B6_DL_LOW_FREQ          875000
#define LTE_B7_DL_LOW_FREQ          2620000
#define LTE_B8_DL_LOW_FREQ          925000
#define LTE_B9_DL_LOW_FREQ          1844900
#define LTE_B10_DL_LOW_FREQ         2110000
#define LTE_B11_DL_LOW_FREQ         1475900
#define LTE_B12_DL_LOW_FREQ         729000
#define LTE_B13_DL_LOW_FREQ         746000
#define LTE_B14_DL_LOW_FREQ         758000
#define LTE_B17_DL_LOW_FREQ         734000
#define LTE_B18_DL_LOW_FREQ         860000
#define LTE_B19_DL_LOW_FREQ         875000
#define LTE_B20_DL_LOW_FREQ         791000
#define LTE_B21_DL_LOW_FREQ         1495900
#define LTE_B23_DL_LOW_FREQ         2180000
#define LTE_B25_DL_LOW_FREQ         1930000
#define LTE_B26_DL_LOW_FREQ         859000
#define LTE_B27_DL_LOW_FREQ         852000
#define LTE_B28_DL_LOW_FREQ         758000
#define LTE_B29_DL_LOW_FREQ         717000
#define LTE_B30_DL_LOW_FREQ         2350000
#define LTE_B32_DL_LOW_FREQ         1452000
#define LTE_B33_DL_LOW_FREQ         1900000
#define LTE_B34_DL_LOW_FREQ         2010000
#define LTE_B35_DL_LOW_FREQ         1850000
#define LTE_B36_DL_LOW_FREQ         1930000
#define LTE_B37_DL_LOW_FREQ         1910000
#define LTE_B38_DL_LOW_FREQ         2570000
#define LTE_B39_DL_LOW_FREQ         1880000
#define LTE_B40_DL_LOW_FREQ         2300000
#define LTE_B41_DL_LOW_FREQ	    2496000
#define LTE_B42_DL_LOW_FREQ	    3400000
#define LTE_B252_DL_LOW_FREQ	    5150000
#define LTE_B255_DL_LOW_FREQ	    5725000
#define LTE_B71_DL_LOW_FREQ	    617000

/*HIGHEST DL FREQUENCIES FOR EACH BAND IN kHz source 36.101 Table 5.5-1*/
#define LTE_B1_DL_HIGH_FREQ         2170000
#define LTE_B2_DL_HIGH_FREQ         1990000
#define LTE_B3_DL_HIGH_FREQ         1880000
#define LTE_B4_DL_HIGH_FREQ         2155000
#define LTE_B5_DL_HIGH_FREQ         894000
#define LTE_B6_DL_HIGH_FREQ         885000
#define LTE_B7_DL_HIGH_FREQ         2690000
#define LTE_B8_DL_HIGH_FREQ         960000
#define LTE_B9_DL_HIGH_FREQ         1879900
#define LTE_B10_DL_HIGH_FREQ        2170000
#define LTE_B11_DL_HIGH_FREQ        1495900
#define LTE_B12_DL_HIGH_FREQ        746000
#define LTE_B13_DL_HIGH_FREQ        756000
#define LTE_B14_DL_HIGH_FREQ        768000
#define LTE_B17_DL_HIGH_FREQ        746000
#define LTE_B18_DL_HIGH_FREQ        875000
#define LTE_B19_DL_HIGH_FREQ        890000
#define LTE_B20_DL_HIGH_FREQ        821000
#define LTE_B21_DL_HIGH_FREQ        1510900
#define LTE_B23_DL_HIGH_FREQ        2200000
#define LTE_B25_DL_HIGH_FREQ        1995000
#define LTE_B26_DL_HIGH_FREQ        894000
#define LTE_B27_DL_HIGH_FREQ        869000
#define LTE_B28_DL_HIGH_FREQ        803000
#define LTE_B29_DL_HIGH_FREQ        728000
#define LTE_B30_DL_HIGH_FREQ        2360000
#define LTE_B32_DL_HIGH_FREQ        1496000
#define LTE_B33_DL_HIGH_FREQ        1920000
#define LTE_B34_DL_HIGH_FREQ        2025000
#define LTE_B35_DL_HIGH_FREQ        1910000
#define LTE_B36_DL_HIGH_FREQ        1990000
#define LTE_B37_DL_HIGH_FREQ        1930000
#define LTE_B38_DL_HIGH_FREQ        2620000
#define LTE_B39_DL_HIGH_FREQ        1920000
#define LTE_B40_DL_HIGH_FREQ        2400000
#define LTE_B41_DL_HIGH_FREQ	    2690000
#define LTE_B42_DL_HIGH_FREQ	    3600000
#define LTE_B252_DL_HIGH_FREQ	    5250000
#define LTE_B255_DL_HIGH_FREQ	    5850000
#define LTE_B71_DL_HIGH_FREQ	    652000

/*MINIMUM DL CHANNEL NUMBER FOR EACH BAND*/
#define LTE_B1_DL_MIN_CHAN          0
#define LTE_B2_DL_MIN_CHAN          600
#define LTE_B3_DL_MIN_CHAN          1200
#define LTE_B4_DL_MIN_CHAN          1950
#define LTE_B5_DL_MIN_CHAN          2400
#define LTE_B6_DL_MIN_CHAN          2650
#define LTE_B7_DL_MIN_CHAN          2750
#define LTE_B8_DL_MIN_CHAN          3450
#define LTE_B9_DL_MIN_CHAN          3800
#define LTE_B10_DL_MIN_CHAN         4150
#define LTE_B11_DL_MIN_CHAN         4750
#define LTE_B12_DL_MIN_CHAN         5010
#define LTE_B13_DL_MIN_CHAN         5180
#define LTE_B14_DL_MIN_CHAN         5280
#define LTE_B17_DL_MIN_CHAN         5730
#define LTE_B18_DL_MIN_CHAN         5850
#define LTE_B19_DL_MIN_CHAN         6000
#define LTE_B20_DL_MIN_CHAN         6150
#define LTE_B21_DL_MIN_CHAN         6450
#define LTE_B23_DL_MIN_CHAN         7500
#define LTE_B25_DL_MIN_CHAN    	    8040
#define LTE_B26_DL_MIN_CHAN         8690
#define LTE_B27_DL_MIN_CHAN         9040
#define LTE_B28_DL_MIN_CHAN         9210
#define LTE_B29_DL_MIN_CHAN         9660
#define LTE_B30_DL_MIN_CHAN         9770
#define LTE_B32_DL_MIN_CHAN         9920
#define LTE_B33_DL_MIN_CHAN         36000
#define LTE_B34_DL_MIN_CHAN         36200
#define LTE_B35_DL_MIN_CHAN         36350
#define LTE_B36_DL_MIN_CHAN         36950
#define LTE_B37_DL_MIN_CHAN         37550
#define LTE_B38_DL_MIN_CHAN         37750
#define LTE_B39_DL_MIN_CHAN         38250
#define LTE_B40_DL_MIN_CHAN         38650
#define LTE_B41_DL_MIN_CHAN         39650
#define LTE_B42_DL_MIN_CHAN         41590
#define LTE_B252_DL_MIN_CHAN        255144
#define LTE_B126_DL_MIN_CHAN        64860
#define LTE_B255_DL_MIN_CHAN        260894
#define LTE_B71_DL_MIN_CHAN         68586

/*MAXIMUM CHANNEL NUMBER FOR EACH BAND*/
#define LTE_B1_DL_MAX_CHAN          599
#define LTE_B2_DL_MAX_CHAN          1199
#define LTE_B3_DL_MAX_CHAN          1949
#define LTE_B4_DL_MAX_CHAN          2399
#define LTE_B5_DL_MAX_CHAN          2649
#define LTE_B6_DL_MAX_CHAN          2749
#define LTE_B7_DL_MAX_CHAN          3449
#define LTE_B8_DL_MAX_CHAN          3799
#define LTE_B9_DL_MAX_CHAN          4149
#define LTE_B10_DL_MAX_CHAN         4749
#define LTE_B11_DL_MAX_CHAN         4999
#define LTE_B12_DL_MAX_CHAN         5179
#define LTE_B13_DL_MAX_CHAN         5279
#define LTE_B14_DL_MAX_CHAN         5379
#define LTE_B17_DL_MAX_CHAN         5849
#define LTE_B18_DL_MAX_CHAN         5999
#define LTE_B19_DL_MAX_CHAN         6149
#define LTE_B20_DL_MAX_CHAN         6449
#define LTE_B21_DL_MAX_CHAN         6599
#define LTE_B23_DL_MAX_CHAN         7699
#define LTE_B25_DL_MAX_CHAN    	    8689
#define LTE_B26_DL_MAX_CHAN    	    9039
#define LTE_B27_DL_MAX_CHAN    	    9209
#define LTE_B28_DL_MAX_CHAN    	    9659
#define LTE_B29_DL_MAX_CHAN    	    9769
#define LTE_B30_DL_MAX_CHAN    	    9869
#define LTE_B32_DL_MAX_CHAN    	    10359
#define LTE_B33_DL_MAX_CHAN         36199
#define LTE_B34_DL_MAX_CHAN         36349
#define LTE_B35_DL_MAX_CHAN         36949
#define LTE_B36_DL_MAX_CHAN         37549
#define LTE_B37_DL_MAX_CHAN         37749
#define LTE_B38_DL_MAX_CHAN         38249
#define LTE_B39_DL_MAX_CHAN         38649
#define LTE_B40_DL_MAX_CHAN         39649
#define LTE_B41_DL_MAX_CHAN	    41589
#define LTE_B42_DL_MAX_CHAN	    43589
#define LTE_B252_DL_MAX_CHAN        256143
#define LTE_B126_DL_MAX_CHAN	    64974
#define LTE_B255_DL_MAX_CHAN	    262143
#define LTE_B71_DL_MAX_CHAN	    68935

/*LOWEST UL FREQUENCIES FOR EACH BAND IN kHz*/
#define LTE_B1_UL_LOW_FREQ          1920000
#define LTE_B2_UL_LOW_FREQ          1850000
#define LTE_B3_UL_LOW_FREQ          1710000
#define LTE_B4_UL_LOW_FREQ          1710000
#define LTE_B5_UL_LOW_FREQ          824000
#define LTE_B6_UL_LOW_FREQ          830000
#define LTE_B7_UL_LOW_FREQ          2500000
#define LTE_B8_UL_LOW_FREQ          880000
#define LTE_B9_UL_LOW_FREQ          1749900
#define LTE_B10_UL_LOW_FREQ         1710000
#define LTE_B11_UL_LOW_FREQ         1427900
#define LTE_B12_UL_LOW_FREQ         699000
#define LTE_B13_UL_LOW_FREQ         777000
#define LTE_B14_UL_LOW_FREQ         788000
#define LTE_B17_UL_LOW_FREQ         704000
#define LTE_B18_UL_LOW_FREQ         815000
#define LTE_B19_UL_LOW_FREQ         830000
#define LTE_B20_UL_LOW_FREQ         832000
#define LTE_B21_UL_LOW_FREQ         1447900
#define LTE_B23_UL_LOW_FREQ         2000000
#define LTE_B25_UL_LOW_FREQ         1850000
#define LTE_B26_UL_LOW_FREQ         814000
#define LTE_B27_UL_LOW_FREQ         807000
#define LTE_B28_UL_LOW_FREQ         703000
#define LTE_B30_UL_LOW_FREQ         2305000
#define LTE_B33_UL_LOW_FREQ         1900000
#define LTE_B34_UL_LOW_FREQ         2010000
#define LTE_B35_UL_LOW_FREQ         1850000
#define LTE_B36_UL_LOW_FREQ         1930000
#define LTE_B37_UL_LOW_FREQ         1910000
#define LTE_B38_UL_LOW_FREQ         2570000
#define LTE_B39_UL_LOW_FREQ         1880000
#define LTE_B40_UL_LOW_FREQ         2300000
#define LTE_B41_UL_LOW_FREQ         2496000
#define LTE_B42_UL_LOW_FREQ         3400000
#define LTE_B71_UL_LOW_FREQ         663000

/*HIGHEST UL FREQUENCIES FOR EACH BAND IN kHz source 36.101 Table 5.5-1*/
#define LTE_B1_UL_HIGH_FREQ         1980000
#define LTE_B2_UL_HIGH_FREQ         1910000
#define LTE_B3_UL_HIGH_FREQ         1785000
#define LTE_B4_UL_HIGH_FREQ         1755000
#define LTE_B5_UL_HIGH_FREQ         849000
#define LTE_B6_UL_HIGH_FREQ         840000
#define LTE_B7_UL_HIGH_FREQ         2570000
#define LTE_B8_UL_HIGH_FREQ         915000
#define LTE_B9_UL_HIGH_FREQ         1784900
#define LTE_B10_UL_HIGH_FREQ        1770000
#define LTE_B11_UL_HIGH_FREQ        1447900
#define LTE_B12_UL_HIGH_FREQ        716000
#define LTE_B13_UL_HIGH_FREQ        787000
#define LTE_B14_UL_HIGH_FREQ        798000
#define LTE_B17_UL_HIGH_FREQ        716000
#define LTE_B18_UL_HIGH_FREQ        830000
#define LTE_B19_UL_HIGH_FREQ        845000
#define LTE_B20_UL_HIGH_FREQ        862000
#define LTE_B21_UL_HIGH_FREQ        1462900
#define LTE_B23_UL_HIGH_FREQ        2020000
#define LTE_B25_UL_HIGH_FREQ        1915000
#define LTE_B26_UL_HIGH_FREQ        849000
#define LTE_B27_UL_HIGH_FREQ        824000
#define LTE_B28_UL_HIGH_FREQ        748000
#define LTE_B30_UL_HIGH_FREQ        2315000
#define LTE_B33_UL_HIGH_FREQ        1920000
#define LTE_B34_UL_HIGH_FREQ        2025000
#define LTE_B35_UL_HIGH_FREQ        1910000
#define LTE_B36_UL_HIGH_FREQ        1990000
#define LTE_B37_UL_HIGH_FREQ        1930000
#define LTE_B38_UL_HIGH_FREQ        2620000
#define LTE_B39_UL_HIGH_FREQ        1920000
#define LTE_B40_UL_HIGH_FREQ        2400000
#define LTE_B41_UL_HIGH_FREQ        2690000
#define LTE_B42_UL_HIGH_FREQ        3600000
#define LTE_B71_UL_HIGH_FREQ        698000

/*MINIMUM UL CHANNEL NUMBER FOR EACH BAND*/
#define LTE_B1_UL_MIN_CHAN          18000
#define LTE_B2_UL_MIN_CHAN          18600
#define LTE_B3_UL_MIN_CHAN          19200
#define LTE_B4_UL_MIN_CHAN          19950
#define LTE_B5_UL_MIN_CHAN          20400
#define LTE_B6_UL_MIN_CHAN          20650
#define LTE_B7_UL_MIN_CHAN          20750
#define LTE_B8_UL_MIN_CHAN          21450
#define LTE_B9_UL_MIN_CHAN          21800
#define LTE_B10_UL_MIN_CHAN         22150
#define LTE_B11_UL_MIN_CHAN         22750
#define LTE_B12_UL_MIN_CHAN         23010
#define LTE_B13_UL_MIN_CHAN         23180
#define LTE_B14_UL_MIN_CHAN         23280
#define LTE_B17_UL_MIN_CHAN         23730
#define LTE_B18_UL_MIN_CHAN         23850
#define LTE_B19_UL_MIN_CHAN         24000
#define LTE_B20_UL_MIN_CHAN         24150
#define LTE_B21_UL_MIN_CHAN         24450
#define LTE_B23_UL_MIN_CHAN         25500
#define LTE_B25_UL_MIN_CHAN         26040
#define LTE_B26_UL_MIN_CHAN         26690
#define LTE_B27_UL_MIN_CHAN         27040
#define LTE_B28_UL_MIN_CHAN         27210
#define LTE_B30_UL_MIN_CHAN         27660
#define LTE_B33_UL_MIN_CHAN         36000
#define LTE_B34_UL_MIN_CHAN         36200
#define LTE_B35_UL_MIN_CHAN         36350
#define LTE_B36_UL_MIN_CHAN         36950
#define LTE_B37_UL_MIN_CHAN         37550
#define LTE_B38_UL_MIN_CHAN         37750
#define LTE_B39_UL_MIN_CHAN         38250
#define LTE_B40_UL_MIN_CHAN         38650
#define LTE_B41_UL_MIN_CHAN         39650
#define LTE_B42_UL_MIN_CHAN         41590
#define LTE_B71_UL_MIN_CHAN         133122

/*MAXIMUM UL CHANNEL NUMBER FOR EACH BAND*/
#define LTE_B1_UL_MAX_CHAN          18599
#define LTE_B2_UL_MAX_CHAN          19199
#define LTE_B3_UL_MAX_CHAN          19949
#define LTE_B4_UL_MAX_CHAN          20399
#define LTE_B5_UL_MAX_CHAN          20649
#define LTE_B6_UL_MAX_CHAN          20749
#define LTE_B7_UL_MAX_CHAN          21449
#define LTE_B8_UL_MAX_CHAN          21799
#define LTE_B9_UL_MAX_CHAN          22149
#define LTE_B10_UL_MAX_CHAN         22749
#define LTE_B11_UL_MAX_CHAN         22999
#define LTE_B12_UL_MAX_CHAN         23179
#define LTE_B13_UL_MAX_CHAN         23279
#define LTE_B14_UL_MAX_CHAN         23379
#define LTE_B17_UL_MAX_CHAN         23849
#define LTE_B18_UL_MAX_CHAN         23999
#define LTE_B19_UL_MAX_CHAN         24149
#define LTE_B20_UL_MAX_CHAN         24449
#define LTE_B21_UL_MAX_CHAN         24599
#define LTE_B23_UL_MAX_CHAN         25699
#define LTE_B25_UL_MAX_CHAN         26689
#define LTE_B26_UL_MAX_CHAN         27039
#define LTE_B27_UL_MAX_CHAN         27209
#define LTE_B28_UL_MAX_CHAN         27659
#define LTE_B30_UL_MAX_CHAN         27759
#define LTE_B33_UL_MAX_CHAN         36199
#define LTE_B34_UL_MAX_CHAN         36349
#define LTE_B35_UL_MAX_CHAN         36949
#define LTE_B36_UL_MAX_CHAN         37549
#define LTE_B37_UL_MAX_CHAN         37749
#define LTE_B38_UL_MAX_CHAN         38249
#define LTE_B39_UL_MAX_CHAN         38649
#define LTE_B40_UL_MAX_CHAN         39649
#define LTE_B41_UL_MAX_CHAN         41589
#define LTE_B42_UL_MAX_CHAN         43589
#define LTE_B71_UL_MAX_CHAN         133471

/*Bandwidth in kHz  */

#define LTE_BW_1P4MHz	1400
#define LTE_BW_3MHz	3000
#define LTE_BW_5MHz	5000
#define LTE_BW_10MHz	10000
#define LTE_BW_15MHz	15000
#define LTE_BW_20MHz	20000
#define LTE_BW_40MHz	40000

#define LTE_B252_EARCFN_255242_FREQ_IN_KHZ 5159800 
#define LTE_B252_EARCFN_255243_FREQ_IN_KHZ 5159900 
#define LTE_B252_EARCFN_255244_FREQ_IN_KHZ 5160000 
#define LTE_B252_EARCFN_255245_FREQ_IN_KHZ 5160100 
#define LTE_B252_EARCFN_255246_FREQ_IN_KHZ 5160200 
#define LTE_B252_EARCFN_255442_FREQ_IN_KHZ 5179800 
#define LTE_B252_EARCFN_255443_FREQ_IN_KHZ 5179900 
#define LTE_B252_EARCFN_255444_FREQ_IN_KHZ 5180000 
#define LTE_B252_EARCFN_255445_FREQ_IN_KHZ 5180100 
#define LTE_B252_EARCFN_255446_FREQ_IN_KHZ 5180200 
#define LTE_B252_EARCFN_255642_FREQ_IN_KHZ 5199800 
#define LTE_B252_EARCFN_255643_FREQ_IN_KHZ 5199900 
#define LTE_B252_EARCFN_255644_FREQ_IN_KHZ 5200000 
#define LTE_B252_EARCFN_255645_FREQ_IN_KHZ 5200100 
#define LTE_B252_EARCFN_255646_FREQ_IN_KHZ 5200200 
#define LTE_B252_EARCFN_255842_FREQ_IN_KHZ 5219800 
#define LTE_B252_EARCFN_255843_FREQ_IN_KHZ 5219900 
#define LTE_B252_EARCFN_255844_FREQ_IN_KHZ 5220000 
#define LTE_B252_EARCFN_255845_FREQ_IN_KHZ 5220100 
#define LTE_B252_EARCFN_255846_FREQ_IN_KHZ 5220200 
#define LTE_B252_EARCFN_256042_FREQ_IN_KHZ 5239800 
#define LTE_B252_EARCFN_256043_FREQ_IN_KHZ 5239900 
#define LTE_B252_EARCFN_256044_FREQ_IN_KHZ 5240000 
#define LTE_B252_EARCFN_256045_FREQ_IN_KHZ 5240100 
#define LTE_B252_EARCFN_256046_FREQ_IN_KHZ 5240200 

#define LTE_B255_EARCFN_261092_FREQ_IN_KHZ 5744800 
#define LTE_B255_EARCFN_261093_FREQ_IN_KHZ 5744900 
#define LTE_B255_EARCFN_261094_FREQ_IN_KHZ 5745000 
#define LTE_B255_EARCFN_261095_FREQ_IN_KHZ 5745100 
#define LTE_B255_EARCFN_261096_FREQ_IN_KHZ 5745200 
#define LTE_B255_EARCFN_261292_FREQ_IN_KHZ 5764800 
#define LTE_B255_EARCFN_261293_FREQ_IN_KHZ 5764900 
#define LTE_B255_EARCFN_261294_FREQ_IN_KHZ 5765000 
#define LTE_B255_EARCFN_261295_FREQ_IN_KHZ 5765100 
#define LTE_B255_EARCFN_261296_FREQ_IN_KHZ 5765200 
#define LTE_B255_EARCFN_261492_FREQ_IN_KHZ 5784800 
#define LTE_B255_EARCFN_261493_FREQ_IN_KHZ 5784900 
#define LTE_B255_EARCFN_261494_FREQ_IN_KHZ 5785000 
#define LTE_B255_EARCFN_261495_FREQ_IN_KHZ 5785100 
#define LTE_B255_EARCFN_261496_FREQ_IN_KHZ 5785200 
#define LTE_B255_EARCFN_261692_FREQ_IN_KHZ 5804800 
#define LTE_B255_EARCFN_261693_FREQ_IN_KHZ 5804900 
#define LTE_B255_EARCFN_261694_FREQ_IN_KHZ 5805000 
#define LTE_B255_EARCFN_261695_FREQ_IN_KHZ 5805100 
#define LTE_B255_EARCFN_261696_FREQ_IN_KHZ 5805200 
#define LTE_B255_EARCFN_261892_FREQ_IN_KHZ 5824800 
#define LTE_B255_EARCFN_261893_FREQ_IN_KHZ 5824900 
#define LTE_B255_EARCFN_261894_FREQ_IN_KHZ 5825000 
#define LTE_B255_EARCFN_261895_FREQ_IN_KHZ 5825100 
#define LTE_B255_EARCFN_261896_FREQ_IN_KHZ 5825200 


// LTE Band mask definitions

#define LTE_B1_MASK_IDX   0
#define LTE_B2_MASK_IDX   1
#define LTE_B3_MASK_IDX   2
#define LTE_B4_MASK_IDX   3
#define LTE_B5_MASK_IDX   4
#define LTE_B6_MASK_IDX   5
#define LTE_B7_MASK_IDX   6
#define LTE_B8_MASK_IDX   7
#define LTE_B9_MASK_IDX   8
#define LTE_B10_MASK_IDX  9
#define LTE_B11_MASK_IDX  10
#define LTE_B12_MASK_IDX  11
#define LTE_B13_MASK_IDX  12
#define LTE_B14_MASK_IDX  13
#define LTE_B15_MASK_IDX  14
#define LTE_B16_MASK_IDX  15
#define LTE_B17_MASK_IDX  16
#define LTE_B18_MASK_IDX  17
#define LTE_B19_MASK_IDX  18
#define LTE_B20_MASK_IDX  19
#define LTE_B21_MASK_IDX  20
#define LTE_B22_MASK_IDX  21
#define LTE_B23_MASK_IDX  22
#define LTE_B24_MASK_IDX  23
#define LTE_B25_MASK_IDX  24
#define LTE_B26_MASK_IDX  25
#define LTE_B27_MASK_IDX  26
#define LTE_B28_MASK_IDX  27
#define LTE_B29_MASK_IDX  28
#define LTE_B30_MASK_IDX  29
#define LTE_B31_MASK_IDX  30
#define LTE_B32_MASK_IDX  31
#define LTE_B33_MASK_IDX  32
#define LTE_B34_MASK_IDX  33
#define LTE_B35_MASK_IDX  34
#define LTE_B36_MASK_IDX  35
#define LTE_B37_MASK_IDX  36
#define LTE_B38_MASK_IDX  37
#define LTE_B39_MASK_IDX  38
#define LTE_B40_MASK_IDX  39
#define LTE_B41_MASK_IDX  40
#define LTE_B42_MASK_IDX  41
#define LTE_B43_MASK_IDX  42
#define LTE_B44_MASK_IDX  43
#define LTE_B71_MASK_IDX  58
#define LTE_B125_MASK_IDX 124
#define LTE_B126_MASK_IDX 125
#define LTE_B127_MASK_IDX 126

#define LTE_B1_MASK   (uint64)(1<<(uint64)(LTE_B1_MASK_IDX))
#define LTE_B2_MASK   (uint64)(1<<(uint64)(LTE_B2_MASK_IDX))
#define LTE_B3_MASK   (uint64)(1<<(uint64)(LTE_B3_MASK_IDX))
#define LTE_B4_MASK   (uint64)(1<<(uint64)(LTE_B4_MASK_IDX))
#define LTE_B5_MASK   (uint64)(1<<(uint64)(LTE_B5_MASK_IDX))
#define LTE_B6_MASK   (uint64)(1<<(uint64)(LTE_B6_MASK_IDX))
#define LTE_B7_MASK   (uint64)(1<<(uint64)(LTE_B7_MASK_IDX))
#define LTE_B8_MASK   (uint64)(1<<(uint64)(LTE_B8_MASK_IDX))
#define LTE_B9_MASK   (uint64)(1<<(uint64)(LTE_B9_MASK_IDX))
#define LTE_B10_MASK  (uint64)(1<<(uint64)(LTE_B10_MASK_IDX))
#define LTE_B11_MASK  (uint64)(1<<(uint64)(LTE_B11_MASK_IDX))
#define LTE_B12_MASK  (uint64)(1<<(uint64)(LTE_B12_MASK_IDX))
#define LTE_B13_MASK  (uint64)(1<<(uint64)(LTE_B13_MASK_IDX))
#define LTE_B14_MASK  (uint64)(1<<(uint64)(LTE_B14_MASK_IDX))
#define LTE_B15_MASK  (uint64)(1<<(uint64)(LTE_B15_MASK_IDX))
#define LTE_B16_MASK  (uint64)(1<<(uint64)(LTE_B16_MASK_IDX))
#define LTE_B17_MASK  (uint64)(1<<(uint64)(LTE_B17_MASK_IDX))
#define LTE_B18_MASK  (uint64)(1<<(uint64)(LTE_B18_MASK_IDX))
#define LTE_B19_MASK  (uint64)(1<<(uint64)(LTE_B19_MASK_IDX))
#define LTE_B20_MASK  (uint64)(1<<(uint64)(LTE_B20_MASK_IDX))
#define LTE_B21_MASK  (uint64)(1<<(uint64)(LTE_B21_MASK_IDX))
#define LTE_B22_MASK  (uint64)(1<<(uint64)(LTE_B22_MASK_IDX))
#define LTE_B23_MASK  (uint64)(1<<(uint64)(LTE_B23_MASK_IDX))
#define LTE_B24_MASK  (uint64)(1<<(uint64)(LTE_B24_MASK_IDX))
#define LTE_B25_MASK  (uint64)(1<<(uint64)(LTE_B25_MASK_IDX))
#define LTE_B26_MASK  (uint64)(1<<(uint64)(LTE_B26_MASK_IDX))
#define LTE_B27_MASK  (uint64)(1<<(uint64)(LTE_B27_MASK_IDX))
#define LTE_B28_MASK  (uint64)(1<<(uint64)(LTE_B28_MASK_IDX))
#define LTE_B29_MASK  (uint64)(1<<(uint64)(LTE_B29_MASK_IDX))
#define LTE_B30_MASK  (uint64)(1<<(uint64)(LTE_B30_MASK_IDX))
#define LTE_B31_MASK  (uint64)(1<<(uint64)(LTE_B31_MASK_IDX))
#define LTE_B32_MASK  ((uint64)SYS_BM_64BIT(LTE_B32_MASK_IDX))
#define LTE_B33_MASK  ((uint64)SYS_BM_64BIT(LTE_B33_MASK_IDX))
#define LTE_B34_MASK  ((uint64)SYS_BM_64BIT(LTE_B34_MASK_IDX))
#define LTE_B35_MASK  ((uint64)SYS_BM_64BIT(LTE_B35_MASK_IDX))
#define LTE_B36_MASK  ((uint64)SYS_BM_64BIT(LTE_B36_MASK_IDX))
#define LTE_B37_MASK  ((uint64)SYS_BM_64BIT(LTE_B37_MASK_IDX))
#define LTE_B38_MASK  ((uint64)SYS_BM_64BIT(LTE_B38_MASK_IDX))
#define LTE_B39_MASK  ((uint64)SYS_BM_64BIT(LTE_B39_MASK_IDX))
#define LTE_B40_MASK  ((uint64)SYS_BM_64BIT(LTE_B40_MASK_IDX))
#define LTE_B41_MASK  ((uint64)SYS_BM_64BIT(LTE_B41_MASK_IDX))
#define LTE_B42_MASK  ((uint64)SYS_BM_64BIT(LTE_B42_MASK_IDX))
#define LTE_B43_MASK  ((uint64)SYS_BM_64BIT(LTE_B43_MASK_IDX))
#define LTE_B44_MASK  ((uint64)SYS_BM_64BIT(LTE_B44_MASK_IDX))
#define LTE_B71_MASK  ((uint64)SYS_BM_64BIT(LTE_B71_MASK_IDX))

/* This is used for margin in calculating ulca tx chan */
#define LTE_INTRA_BAND_ULCA_BW_MARGIN 20
/* This is used for checking if ulca upper bound and lower bound is within 40MHz */
#define LTE_INTRA_BAND_ULCA_BW_TOTAL_KHZ 40000
/* This is used for checking if ulca lower bound is within 20MHz */
#define LTE_INTRA_BAND_ULCA_MIN_AGG_BW_TOTAL_KHZ 20000
/* Macro to provide the max channel seperation between two EARFCN's in Intra ULCA */
#define LTE_INTRA_BAND_ULCA_MAX_CHANNEL_SEPERATION 198
/*---------------------------------------------------------------------------*/
#define IS_BIT_SET(mask, position)   ((((mask) >> (position)) & 0x1) == 1)
#define BAND_MASK_WIDTH 4


#ifdef __cplusplus
}
#endif

#endif

