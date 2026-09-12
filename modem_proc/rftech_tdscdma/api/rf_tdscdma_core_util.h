/*!
 @file rf_tdscdma_core_util.h

 @brief
  This is the header file file for utility functions for the TDSCDMA technology.

 @details

*/

/*===========================================================================

Copyright (c) 2011 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_tdscdma.mpss/3.11/api/rf_tdscdma_core_util.h#1 $

When       who     what, where, why
-------------------------------------------------------------------------------
08/18/15   avi     Moved #define/Constants to rf_tdscdma_core_util.h from rf_tdscdma_core_util.c
05/13/15   jak     Tx/Rx split feature - adaption to WTR interface change
04/08/14   jhe     Add is_band_supported api and file cleanup
09/18/13   ms      Add api for obtaining filtered band 
08/22/13   jr      Removed API rf_tdscdma_core_util_calc_tx_cal_chan_freq()
05/20/13   al      Added util function to convert subband to fullband
02/07/12   kg      Adding new util funcions to get mask and max/min freq
09/24/12   jyu     Added support to get PA_STATE from PA_RANGE
05/27/11   jhe     initial version.

============================================================================*/
#ifndef RF_TDSCDMA_CORE_UTIL_H
#define RF_TDSCDMA_CORE_UTIL_H

#include "rfa_variation.h"
#include "rfcom.h"
#include "rfnv_tdscdma.h"

#ifdef __cplusplus
extern "C" {
#endif



/* ----------------------------------------------------------------------- */
uint32 rf_tdscdma_core_util_get_freq_from_uarfcn(uint16 chan, rfcom_tdscdma_band_type band);

/* ----------------------------------------------------------------------- */
uint16 rf_tdscdma_core_util_get_uarfcn_from_freq(uint32 freq, rfcom_tdscdma_band_type band);

/* ----------------------------------------------------------------------- */
rfcom_tdscdma_band_type rf_tdscdma_core_util_get_band_from_uarfcn(uint16 chan);

/* ----------------------------------------------------------------------- */
rfcom_tdscdma_band_type rf_tdscdma_core_util_get_filtered_band(rfcom_tdscdma_band_type band);

/* ----------------------------------------------------------------------- */
uint32 rf_tdscdma_core_util_get_freq_from_chan(uint16 chan);

uint32 rf_tdscdma_core_util_get_freq_from_chan_common_api(uint32 chan);


/* ----------------------------------------------------------------------- */
rfnv_tdscdma_rx_tbl_type *rf_tdscdma_core_util_init_rx_nv_table(rfcom_tdscdma_band_type band);

/* ----------------------------------------------------------------------- */
rfnv_tdscdma_tx_tbl_type *rf_tdscdma_core_util_init_tx_nv_table(rfcom_tdscdma_band_type band);

/* ----------------------------------------------------------------------- */
uint8 rf_tdscdma_core_util_get_pa_range_from_nv(uint8 pa_index);

/* ----------------------------------------------------------------------- */
uint8 rf_tdscdma_core_util_get_pa_range_map_nv_index(rfcom_tdscdma_band_type band, uint8 pa_range);

/* ----------------------------------------------------------------------- */
int16 rf_tdscdma_core_util_get_pa_comp_up_nv(uint8 pa_index);

/* ----------------------------------------------------------------------- */
int16 rf_tdscdma_core_util_get_pa_comp_down_nv(uint8 pa_index);

/* ----------------------------------------------------------------------- */
int16 rf_tdscdma_core_util_convert_tx_dbm_to_db10_unit( int16 dbmval );

/* ----------------------------------------------------------------------- */
int16 rf_tdscdma_core_util_convert_tx_agc_to_dbm_unit( int16 txagcval );

/* ----------------------------------------------------------------------- */
int16 rf_tdscdma_core_util_get_tx_agc_val( void );

/* ----------------------------------------------------------------------- */
void rf_tdscdma_core_util_pa_range_lookup(int16 txagc, uint8 mpr_idx, boolean pa_override, uint8* parange, uint8* nv_pa_idx);

/* ----------------------------------------------------------------------- */
rf_card_band_type rf_tdscdma_core_util_convert_band_rfcom_to_rfc(rfcom_tdscdma_band_type rfcom_band);

/* ----------------------------------------------------------------------- */
rfcom_tdscdma_band_type rf_tdscdma_core_util_convert_band_rfc_to_rfcom(rf_card_band_type rfc_band);

/*--------------------------------------------------------------------------*/
int32 rf_tdscdma_core_util_math_floor(int32 dividend,int32 divisor);

/*--------------------------------------------------------------------------*/
boolean rf_tdscdma_core_util_get_min_max_rx_freqs(rfcom_tdscdma_band_type band, uint32* min, uint32* max);

/*--------------------------------------------------------------------------*/
uint64 rf_tdscdma_core_util_get_mask(rfcom_tdscdma_band_type band);

/*--------------------------------------------------------------------------*/
boolean rf_tdscdma_core_util_get_band_num(rfcom_tdscdma_band_type band,uint16* num);

/*----------------------------------------------------------------------------*/
void rf_tdscdma_core_get_fullband_from_subband(rfcom_tdscdma_band_type* tdscdma_rfcom_band);

/*----------------------------------------------------------------------------*/
boolean rf_tdscdma_core_util_is_band_supported(rfcom_tdscdma_band_type band, rf_path_enum_type path);

/*----------------------------------------------------------------------------*/
rfdevice_rx_mode_type rf_tdscdma_core_util_rx_mode_type(rfm_device_enum_type device);

/*=============================================================================
                             DATA DEFINITIONS
=============================================================================*/
/*TDSCDMA channel raster in kHz.*/
#define RF_TDSCDMA_CHAN_RASTER_IN_KHZ 200

/*MINIMUM CHANNEL NUMBER FOR EACH BAND*/
#define RF_TDSCDMA_B34_MIN_CHAN    	10054
#define RF_TDSCDMA_B39_MIN_CHAN    	9404
#define RF_TDSCDMA_B40_MIN_CHAN    	11504
#define RF_TDSCDMA_B40_B_MIN_CHAN   11801

/*MAXIMUM CHANNEL NUMBER FOR EACH BAND*/
#define RF_TDSCDMA_B34_MAX_CHAN    	10121
#define RF_TDSCDMA_B39_MAX_CHAN    	9596
#define RF_TDSCDMA_B40_MAX_CHAN    	11996

/*LOWEST FREQUENCY FOR EACH BAND IN kHz*/
#define RF_TDSCDMA_B34_LOW_FREQ 	2010000
#define RF_TDSCDMA_B39_LOW_FREQ 	1880000
#define RF_TDSCDMA_B40_LOW_FREQ 	2300000

/*HIGHEST FREQUENCY FOR EACH BAND IN kHz*/
#define RF_TDSCDMA_B34_HIGH_FREQ 	2025000
#define RF_TDSCDMA_B39_HIGH_FREQ 	1920000
#define RF_TDSCDMA_B40_HIGH_FREQ 	2400000

//TDSCDMA Band mask definitions
#define RF_TDSCDMA_B34_MASK_IDX  0
#define RF_TDSCDMA_B39_MASK_IDX  1
#define RF_TDSCDMA_B40_MASK_IDX  2

#define RF_TDSCDMA_B34_MASK  (uint64)(1<<(uint64)(RF_TDSCDMA_B34_MASK_IDX))
#define RF_TDSCDMA_B39_MASK  (uint64)(1<<(uint64)(RF_TDSCDMA_B39_MASK_IDX))
#define RF_TDSCDMA_B40_MASK  (uint64)(1<<(uint64)(RF_TDSCDMA_B40_MASK_IDX))

#ifdef __cplusplus
}
#endif

#endif

