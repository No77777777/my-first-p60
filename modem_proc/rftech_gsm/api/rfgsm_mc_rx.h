#ifndef RFGSM_MC_RX_H
#define RFGSM_MC_RX_H

/*!
   @file
   rfgsm_mc_rx.h

   @brief


   @details
   
*/

/*===========================================================================
Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:10:17 $
$Header: //components/rel/rftech_gsm.mpss/3.11/api/rfgsm_mc_rx.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/19/15   sc      Update and enable optmised Rx burst and feature guard
04/22/15   sc      Introduce new Rx API
03/12/15   sc      Add Rx Wakeup/Sleep MC interface
03/12/15   sc      Initial version.

============================================================================*/
#include "rfcom.h"
#include "rfgsm_mc.h"
#include "rfm_device_types.h"
#include "rfgsm_msg.h"
#include "rfgsm_core_types.h"



/*----------------------------------------------------------------------------*/
typedef struct
{
  uint8 sub_id;
  rfcom_device_enum_type prx_device;
  rfcom_device_enum_type drx_device;
  rfgsm_op_mode_type op_mode;
  uint8 buffer_id;
  rfa_rf_gsm_rx_burst_type * rx_burst_params;
}rfgsm_mc_rx_data_type;



/*----------------------------------------------------------------------------*/
extern rfgsm_mc_status_type rfgsm_mc_rx_wakeup( uint8 sub_id,
                                                rfcom_device_enum_type prx_device,
                                                rfcom_device_enum_type drx_device,
                                                rfgsm_op_mode_type op_mode,
                                                rfgsm_rxlm_buf_index_type * rxlm_buf_index,
                                                void * gfw_buff_ptr,
                                                rfgsm_timing_info_type * timing_info );

/*----------------------------------------------------------------------------*/
extern rfgsm_mc_status_type rfgsm_mc_rx_sleep( uint8 sub_id,
                                               rfcom_device_enum_type prx_device,
                                               rfcom_device_enum_type drx_device,
                                               rfgsm_op_mode_type op_mode );

/*----------------------------------------------------------------------------*/
extern rfgsm_mc_status_type rfgsm_mc_rx( rfgsm_mc_rx_data_type * rx_data );

/*----------------------------------------------------------------------------*/
rfgsm_mc_status_type rfgsm_mc_rx_init_once( void );

/*----------------------------------------------------------------------------*/
rfgsm_mc_status_type rfgsm_mc_rx_destroy( void );

#endif /* RFGSM_MC_RX_H */
