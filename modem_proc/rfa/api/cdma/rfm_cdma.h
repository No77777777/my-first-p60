#ifndef RFM_CDMA_H
#define RFM_CDMA_H

/*!
  @file
  rfm_cdma.h

  @brief
  RF Driver's Common CDMA external interface file.
*/

/*==============================================================================

  Copyright (c) 2010 - 2012 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/api/cdma/rfm_cdma.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
05/15/18   am      Changes to support MAAT
08/03/15   wwl     Add API header file for ASDiv SAR limits programmable
                   per RAT/band
03/09/14   pk      Added interface to query DLNA support
10/08/13   zhw     Expose turn-around constant to RFM layer
04/14/13   APU     Added another get_filtered_tx_power API that can query
                   different kind of filters.
10/25/12   cd/Saul Generalized load DPD table API
                   - Changed request packet type
                   - Added response packet type
                   - Made XPT IQ capture common.
05/25/12   spa     Renamed RFM_CDMA_ERROR_NULL_DEVICE_DATA to
                   RFM_CDMA_ERROR_NULL_DATA
05/25/12   spa     Removed RFM_CDMA_ERROR_INVALID_DEV, since it was unused
04/26/12   zhaow   Added error code RFM_CDMA_ERROR_INVALID_BAND_CHAN for
                   validity checking
04/26/12   aro     Added generic RFM CDMA API status return type
02/06/12   hdz     Added rfm_cdma_update_agc() for re-loading rx/tx agc in calls
09/12/11   sty     Added rfm_cdma_is_tunable_on_device()
06/13/11   sty     Replaced RFM_CDMA_ERROR_NULL_NV_FOR_DEVICE_ON_BAND with
                   generic error code - RFM_CDMA_ERROR_BAD_CONFIG
06/08/11   sty     Added error code for NULL NV on given band for a device
06/07/11   aro     Added Error code for NULL Device Data
03/08/11   aro     Added CDMA healty State enum
01/18/11   aro     Added Invalid Mode Error Code
01/15/11   aro     Added Get Current Band Chan Function
01/10/11   aro     Added Error Code for return values
12/10/10   aro     Initial version

==============================================================================*/

#include "comdef.h"
#include "rfm_device_types.h"
#include "rfm_path_types.h"
#include "rfm_cdma_band_types.h"
#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*/
/*! Code when RF func has healthy RF state. This will be used during RF State
validation. For RFM Status, any positive number refers to successful
execution. */
#define RFM_CDMA_HEALTHY_STATE 0

/*----------------------------------------------------------------------------*/
/*! Error code when RF func Fails inspite of good parameter and state. */
#define RFM_CDMA_ERROR_FAILURE -1

/*----------------------------------------------------------------------------*/
/*! Error code indicating that logical device parameter is invalid*/
#define RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM -2

/*----------------------------------------------------------------------------*/
/*! Error code when trying to tune the radio when RF Dev is in Invalid State. */
#define RFM_CDMA_ERROR_INVALID_DEV_STATE -3

/*----------------------------------------------------------------------------*/
/*! Error code when trying to tune the radio when RF Mode is not correct.*/
#define RFM_CDMA_ERROR_INVALID_MODE -4

/*----------------------------------------------------------------------------*/
/*! Error code indicating the Device has NULL Data.*/
#define RFM_CDMA_ERROR_NULL_DATA -5

/*----------------------------------------------------------------------------*/
/*! Error code indicating that the config error occured                      */
#define RFM_CDMA_ERROR_BAD_CONFIG -6

/*----------------------------------------------------------------------------*/
/*! Error code indicating that the Parameter passed is bad  */
#define RFM_CDMA_ERROR_BAD_PARAM -7

/*----------------------------------------------------------------------------*/
/*! Error code indicating that the band or channel are not valid in the
    current configuration.                                                    */
#define RFM_CDMA_ERROR_INVALID_BAND_CHAN -8

/*----------------------------------------------------------------------------*/
/*! Enumeration indicating the Tx Mode of operation for XPT */
typedef enum
{
  RFM_CDMA_XPT_TX_MODE_NOT_ACTIVE = 0,
  /*!< Enumeration to indicate that XPT mode is not active */

  RFM_CDMA_XPT_TX_MODE_APT,
  /*!< Enumeration to indicate that APT State */

  RFM_CDMA_XPT_TX_MODE_SAPT,
  /*!< Enumeration to indicate that SAPT State with SAPT mode */

  RFM_CDMA_XPT_TX_MODE_SAPT_APT,
  /*!< Enumeration to indicate that SAPT State with APT mode */

  RFM_CDMA_XPT_TX_MODE_SAPT_COMPRESSED,
  /*!< Enumeration to indicate that SAPT State with SAPT mode and
  compressed DPD */

} rfm_cdma_xpt_tx_mode_type;


/*----------------------------------------------------------------------------*/
/*! RF ENUM that holds the various types of filters that FW
  supports. */
typedef enum
{
  /*! Tx filter that returns Tx RMS power. */
  RFM_TX_FILTER_RMS_POWER                   , /* Can be used by SAR clients   */

  /*! Tx filter that returns Tx power only when Tx is on */
  RFM_TX_FILTER_POWER_WHEN_TX_ON             , /* Can be used by COEX clients */

  /*! Invalid Filter Type */
  RF_TX_FILTER_INVALID

}  rfm_filter_type ;


/*----------------------------------------------------------------------------*/
/*!

  @brief
  A data type to hold paramters relevant while doing AsDiv
  switching.
*/
typedef struct
{
  int dummy ;
}  rfm_cdma_asdiv_params ;


/*----------------------------------------------------------------------------*/
/*! Type to indicate the status of RFM CDMA interface status */
typedef int32 rfm_cdma_api_status_type;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  A data type to hold Rx paramters passed to common MAAT module.
*/
typedef struct
{
  /* Rx Band and Channel */
  rfm_cdma_band_class_type rx_band;
  rfm_cdma_chan_type rx_channel;

  /* rfm path of PRx */
  rfm_path_type prx_rfm_path;

  /* rfm path of DRx */
  rfm_path_type drx_rfm_path;

  /* Combined SNR value */
  int16 snr_db10;
  
} rfm_cdma_maat_rx_info_type_s;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  A data type to hold Tx paramters passed to common MAAT module.
*/
typedef struct
{
  /* Flag to indicate Tx active */
  boolean active_tx;

  /* Tx Band and Channel */
  rfm_cdma_band_class_type tx_band;
  rfm_cdma_chan_type tx_channel;

  /* rfm path of Tx */
  rfm_path_type tx_rfm_path;

  int16 txpwr_db10;

} rfm_cdma_maat_tx_info_type_s;


/*----------------------------------------------------------------------------*/
sys_channel_type
rfm_cdma_get_band_chan
(
  rfm_device_enum_type device
);

/*----------------------------------------------------------------------------*/
uint32
rfm_cdma_get_sub_class
(
  rfm_device_enum_type device,
  sys_band_class_e_type band
);

/*----------------------------------------------------------------------------*/
uint32
rfm_cdma_get_rx_carrier_freq
(
  sys_channel_type band_chan
);

/*----------------------------------------------------------------------------*/
uint32
rfm_cdma_get_tx_carrier_freq
(
  sys_channel_type band_chan
);

/*----------------------------------------------------------------------------*/
int
rfm_cdma_get_turnaround_constant
(
  const sys_band_class_e_type band
);

/*----------------------------------------------------------------------------*/
boolean
rfm_cdma_is_band_chan_supported
(
  const rfm_device_enum_type device,
  const sys_channel_type band_chan
);

/*----------------------------------------------------------------------------*/
boolean
rfm_cdma_is_tunable_on_device
(
  const rfm_device_enum_type device,
  const sys_band_class_e_type band
);

/*----------------------------------------------------------------------------*/
boolean
rfm_cdma_update_agc
(
  void
);

/*----------------------------------------------------------------------------*/
boolean
rfm_cdma_is_dlna_supported
(
  const rfm_device_enum_type device
);

/*----------------------------------------------------------------------------*/
boolean
rfm_cdma_prep_asdiv
(
   const rfm_device_enum_type   device  ,
   rfm_cdma_asdiv_params*          params
) ;


/*----------------------------------------------------------------------------*/
boolean
rfm_cdma_end_asdiv
(
  const rfm_device_enum_type   device
) ;

/*----------------------------------------------------------------------------*/
boolean
rfm_cdma_get_maat_info
(
   rfm_cdma_maat_tx_info_type_s *tx_maat_info_ptr,
   rfm_cdma_maat_rx_info_type_s *rx_maat_info_ptr
);


#ifdef __cplusplus
}
#endif

#endif /* RFM_CDMA_H */

