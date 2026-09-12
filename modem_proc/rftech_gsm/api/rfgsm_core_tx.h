#ifndef RFGSM_CORE_TX_H
#define RFGSM_CORE_TX_H

/*!
   @file
   rfgsm_core_tx.h

   @brief

   @details

*/
/*=============================================================================
Copyright (c) 2008 - 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/api/rfgsm_core_tx.h#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/10/15   tws     Fix incorrect AFC being applied in GBTA cases
05/28/15   sc      Enable thermistor and vbatt reads
03/31/15   sc      Add Tx wakeup event build and RFLM index APIs
03/30/15   sc      Add Tx timing update functionality
03/25/15   sc      Add core Tx Wakeup/Sleep APIs
11/20/14   hoh     Remove unused function declaration
06/05/14   ggs     AMAM/AMPM tables should only be recalculated 
                   for GSM when modulation type is 8PSK 
04/28/14   sc      Scale the Tx PA ramps based on subscription 
02/28/14   sc      Pass core data through mc to core layers 
02/17/14   sc      Clean up mDSP APIs
01/21/14   tsr     WTR TXAGC support for Multislot TX 
01/21/14   ggs     Initial changes for TXC DCOC and DPD calibration 
10/28/13   ggs     Downsample base AMAM temp comp LUT by a factor of 4  
06/05/13   svi     Adding Support of Enhanced Temperature Compensation in GSM. 
02/05/13   sb      Created separate APIs to configure RF front end devices based on whether
                   they are called every TX/RX burst or only when the band is changed. 
11/21/12   av      Added rfm_device parameters to mc/core/mdsp/msm layers.
07/26/12   sr      Changes to APIS to take rfm_dev as parameter.
06/19/12   sc      Added Tx band shared memory interface buffer pointer
04/24/12   ggs     Added rfgsm_core_override_pa_range prototype 
08/09/11   av      Added NV support for Digital Baseband compensation 
08/08/11   av      Support for Digital Baseband Compensation
03/19/10   lcl     Pull in changes from QSC6295
02/04/10   sr      Lint & kw fixes 
07/27/09   rsr     Mainline rfgsm_is_in_linear_mode, compier warning cleanup. 
07/06/09   sr      changes to make the operating band to rfcom_gsm_band type
03/13/09   sr      code cleanup for SCMM build
0/14/08    sr      Initial version

============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include "rfcom.h"
#include "rfgsm_nv_cmn.h"
#include "rfm_device_types.h"
#include "rfgsm_core.h"



/*---------------------------------------------------------------------------*/
boolean rfgsm_core_tx_wakeup( uint8 sub_id,
                              rfcom_device_enum_type tx_device,
                              uint32 txlm_buf_index );

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_tx_wakeup_timing_update( uint8 sub_id,
                                            rfcom_device_enum_type tx_device,
                                            rfgsm_timing_info_type * gl1_timing_info );

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_tx_wakeup_build( uint8 sub_id, 
                                    rfcom_device_enum_type tx_device, 
                                    void * gfw_buff_ptr );

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_tx_sleep( uint8 sub_id,
                             rfcom_device_enum_type tx_device );

/*---------------------------------------------------------------------------*/
void rfgsm_core_set_tx_band( rfgsm_core_data_type *core_data,
                             rfcom_gsm_band_type band, 
                             void *shared_mem_ptr );

/*---------------------------------------------------------------------------*/
void rfgsm_core_set_tx_profile_update_flag(rfgsm_core_handle_type *rfgsm_core_handle_ptr);

/*---------------------------------------------------------------------------*/
void rfgsm_core_clear_tx_profile_update_flag(rfgsm_core_handle_type *rfgsm_core_handle_ptr);

/*---------------------------------------------------------------------------*/
int32 rfgsm_core_get_tx_freq_error(rfgsm_core_handle_type *rfgsm_core_handle_ptr);

/*---------------------------------------------------------------------------*/
void rfgsm_core_update_tx_freq_error(rfgsm_core_data_type *core_data, int32  freq_err);

/*---------------------------------------------------------------------------*/
uint32 rfgsm_core_convert_arfcn_to_tx_freq(rfcom_gsm_band_type band, uint16 arfcn );

/*---------------------------------------------------------------------------*/
int16 rfgsm_core_get_enh_temp_comp_pwr_offset(rfm_device_enum_type rfm_dev, int16 scaled_value, uint16 pcl, int16 pwr_in_dbm, rfgsm_modulation_type mod_type);

/*---------------------------------------------------------------------------*/
void rfgsm_core_set_tx_transition_buffer( rfm_device_enum_type rfm_dev,
                                          uint8 tx_slot,
                                          rfgsm_modulation_type mod_type);

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_scale_tx_pa_ramp_data( rfm_device_enum_type rfm_dev,
                                          uint8 sub_id,
                                          uint8 tx_slot,
                                          uint16 pa_scale );

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_tx_set_pa_ramp_data_buffer( rfm_device_enum_type rfm_dev, uint8 buffer_index );

/*---------------------------------------------------------------------------*/
void rfgsm_core_update_linear_amam_ampm_profiles(rfgsm_core_data_type *core_data);

/*---------------------------------------------------------------------------*/
void rfgsm_core_update_linear_amam_temp_comp(rfgsm_core_data_type *core_data);

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_prepare_therm_read( uint8 sub_id, 
                                       rfm_device_enum_type tx_device, 
                                       void * shared_mem_ptr );

#ifdef __cplusplus
}
#endif


#endif

