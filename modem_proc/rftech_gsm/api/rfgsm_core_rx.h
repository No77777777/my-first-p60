#ifndef RFGSM_CORE_RX_H
#define RFGSM_CORE_RX_H

/*!
   @file
   rfgsm_core_rx.h

   @brief

   @details

*/
/*=============================================================================
Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/api/rfgsm_core_rx.h#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/14/15   ec      Update rx_sleep api to use op_mode parameter 
08/12/15   sc      Update GL1 desense channel list on PRx wakeup
06/19/15   sc      Update and enable optmised Rx burst and feature guard
05/27/15   sc      Calculate Rx burst data for optimised Rx burst API
05/21/15   sc      Add new Rx burst core API hooks
04/22/15   zbz     WTR interface change to support DRx 
04/10/15   zbz     Interface changes with device driver for split Rx/Tx 
03/30/15   sc      Add Rx timing update functionality
03/26/15   sc      Add legacy functionality to new Rx/Tx Wakeup/Sleep call-flow
03/25/15   sc      Add core Rx Wakeup/Sleep APIs
02/27/15   sc      Ensure NULL NV error checking when processing Rx/Tx band
02/13/15   sc      Initial version

============================================================================*/



/*=============================================================================

                                  INCLUDES

=============================================================================*/
#include "rfcom.h"
#include "rfgsm_core.h"
#include "rfgsm_msg.h"



/*=============================================================================

                             FUNCTION PROTOTYPES

=============================================================================*/
boolean rfgsm_core_rx_wakeup( uint8 sub_id,
                              rfcom_device_enum_type rx_device,
                              uint32 rxlm_buf_index,
                              rfdevice_rx_mode_type op_mode );

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_rx_sleep( uint8 sub_id,
                             rfcom_device_enum_type rx_device,
                             rfgsm_op_mode_type op_mode );

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_rx_wakeup_timing_update( uint8 sub_id,
                                            rfcom_device_enum_type prx_device,
                                            rfcom_device_enum_type drx_device,
                                            rfgsm_op_mode_type op_mode,
                                            rfgsm_timing_info_type * gl1_timing_info );

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_rx_update_desense_chan_list( uint8 sub_id, 
                                                rfcom_device_enum_type prx_device );

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_rx_wakeup_build( uint8 sub_id,
                                    rfcom_device_enum_type prx_device,
                                    rfcom_device_enum_type drx_device,
                                    rfgsm_op_mode_type op_mode,
                                    void * gfw_buff_ptr );

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_rx_set_band( rfgsm_core_data_type *core_data,
                                rfcom_gsm_band_type band );


/*---------------------------------------------------------------------------*/
boolean rfgsm_core_rx_prepare( uint8 sub_id,
                               rfcom_device_enum_type prx_device,
                               rfa_rf_gsm_rx_burst_type * burst_params,
                               uint8 buffer_id,
                               uint32 burst_num );

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_rx_diversity_prepare( uint8 sub_id,
                                         rfcom_device_enum_type prx_device,
                                         rfa_rf_gsm_rx_burst_type * burst_params,
                                         uint8 buffer_id,
                                         uint32 burst_num );

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_rx_build_scripts( uint8 sub_id,
                                     rfcom_device_enum_type prx_device,
                                     rfcom_device_enum_type drx_device,
                                     rfa_rf_gsm_rx_burst_type * burst_params, 
                                     boolean rx_diversity_enabled,
                                     uint8 buffer_id,
                                     uint32 burst_num );

#endif /*RFGSM_CORE_RX_H*/

