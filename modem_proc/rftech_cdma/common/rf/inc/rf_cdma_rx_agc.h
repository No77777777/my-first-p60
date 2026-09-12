#ifndef RF_CDMA_RX_AGC_H
#define RF_CDMA_RX_AGC_H

/*!
  @file
  rf_cdma_rx_agc.h

  @details
  This file exports the definitions and declartions pertaining to RxAGC.
*/

/*==============================================================================

  Copyright (c) 2010-2012 Qualcomm Technologies, Inc.. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc..

==============================================================================*/

/*==============================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rftech_cdma.mpss/3.11/common/rf/inc/rf_cdma_rx_agc.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
04/15/15   zhw     Integrate device driver change with CDMA
04/11/14   spa     Add tech data ptr to rxagc data struct/Jammer detect support
10/09/13   spa     Add rf mode param to configure RxAGC data 
04/11/12   aro     Removed unused RxAGC freeze and unfeeze functions
04/11/12   aro     Documentation Update
02/06/12   hdz     Added rf_cdma_update_rx_agc for reloading rx agc in calls
12/20/10   aro     Added CDMA Freeze/Unfreeze functions
12/09/10   aro     Moved code to LNA specific file
12/08/10   aro     Fixing preprocessor Macros
12/06/10   aro     Initial Release.

==============================================================================*/

#include "comdef.h"
#include "lm_types.h"
#include "rfm_device_types.h"
#include "rfm_mode_types.h"
#include "rfm_cdma_band_types.h"
#include "rfm_gain_state_types.h"
#include "rf_cdma_state_types.h"
#include "rfm_cdma_power_mode_types.h"
#include "rf_cdma_mdsp_types.h"
#include "rfc_cdma.h"
#include "rf_cdma_data.h"

/*----------------------------------------------------------------------------*/
boolean
rf_cdma_update_rx_agc
(
  rfm_device_enum_type device
);

/*----------------------------------------------------------------------------*/
boolean
rf_cdma_mc_configure_rx_agc_data
(
  const rfm_mode_enum_type rf_mode,
  const rfm_device_enum_type device,
  const rfm_cdma_band_class_type band,
  const lm_handle_type rx_handle,
  const rf_cdma_carrier_mode_type carrier_mode,
  const uint32 bw_khz,
  const rfm_cdma_power_mode_type power_mode,
  const rf_cdma_state_type rf_state,
  rf_cdma_mdsp_rx_agc_config_type* rxagc_data
);

/*----------------------------------------------------------------------------*/
boolean
rf_cdma_mc_configure_stop_rx_agc_data
(
  const rfm_device_enum_type device,
  const lm_handle_type rx_handle
);

/*----------------------------------------------------------------------------*/
boolean 
rf_cdma_configure_jd_thresh
(
  const rfm_device_enum_type device,
  const rfc_cdma_logical_device_params_type *logical_dev,
  const rfm_cdma_band_class_type band
);


/*----------------------------------------------------------------------------*/
boolean
rf_cdma_mc_cleanup_lna_fsm
(
  const lm_handle_type rx_handle
) ;


/*----------------------------------------------------------------------------*/
boolean
rf_cdma_rx_adjust_lna_info 
( 
  void*                             rf_cdma_config_ptr ,
  rfm_mode_enum_type                rfm_mode ,
  rf_cdma_mdsp_rx_agc_config_type*  rx_agc_param /* Container for LNA data */
) ;

/*----------------------------------------------------------------------------*/
boolean
rf_cdma_update_rx_agc_with_new_wtr_gain
(
  rfm_device_enum_type                        device ,
  const rf_cdma_data_status_type*             dev_status , 
  const rfc_cdma_logical_device_params_type*  logical_dev ,
  uint8                                       position
) ;

#endif  /* RF_CDMA_RX_AGC_H */

