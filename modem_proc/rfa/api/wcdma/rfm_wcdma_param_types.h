#ifndef RFM_WCDMA_PARAM_TYPES_H
#define RFM_WCDMA_PARAM_TYPES_H
/*! @brief
  @file

  @brief
  This module contains the definitions of the WCDMA RFM interface parameters.
*/

/*===========================================================================


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

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfa.mpss/4.11/api/wcdma/rfm_wcdma_param_types.h#1 $

when       who     what, where, why 
-------   ---     -------------------------------------------------------------
06/04/20   nik    Changes to pass sub_id as param to WTR APIs to handle MSIM
05/24/16   ag     W+W logging changes for inside the W2W gap
10/06/15  ac/rmb  AGC loging changes
08/26/15   ac     backingout the agc logging change for W plus W, will re-submit along with WL1 code
08/18/15  ac      agc logging changes for w plus w
07/29/15  asn     Clean up calls to legacy read RxAGC API
07/27/15  asn     New Power ON OFF API
07/27/15  rmb     Fix Sub id Enum
07/22/15  asn     Added structures for parameters to new RxAGC read API
6/29/15   vbh     Added PRx device param for enter mode
6/16/15   vbh     Added an enum to keep a track of the RF API being tested 
04/24/15  ak      Changes to correct the MAx carriers to prevent false SM update
04/09/15  vbh     Added params for power on/off API
04/02/15  vbh     Added params for Tx wakeup API
02/09/15  dw      Added/Moved a few RFM API params
08/14/14  vbh     Added Tx rfm api interface structures
08/05/14  vbh     Updated the sleep params structure to take a flag to skip AGC disable
04/11/14  vbh     [FR17754] Added interface structure for tune 
3/05/14   vbh     Initial revision

==============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "rfcom.h"
#include "lm_types.h"
#include "rfm_path_types.h"



/*----------------------------------------------------------------------------*/
/*!
  @brief
  Identifies individual carriers of a multi-carrier WCDMA radio.
*/
typedef enum
{

  RFM_WCDMA_CARRIER_0 = 0,
  /*!< Carrier 0 */

  RFM_WCDMA_CARRIER_1 = 1,
  /*!< Carrier 1 */

  RFM_WCDMA_CARRIER_2 = 2,
  /*!< Carrier 2 */

  RFM_WCDMA_CARRIER_MAX

} rfm_wcdma_carrier_type;

/*----------------------------------------------------------------------------*/

/* XPT mode conifguration - APT/EPT/ET */
typedef enum
{
  /*! @brief APT mode */
  RFM_TXAGC_APT,

  /*! @brief EPT mode */
  RFM_TXAGC_EPT, 

  /*! @brief ET mode */
  RFM_TXAGC_ET, 

  /*! @brief max xPT mode */
  RFM_TXAGC_MAX_XPT_MODE  
}rfm_wcdma_xpt_mode_type;

/*----------------------------------------------------------------------------*/
/*! Enumeration to specify the device type for enter mode */
typedef enum
{
  RFM_WCDMA_PRIMARY_RX_DEVICE = 0,
  /*!< Primary Rx device */

  RFM_WCDMA_DIVERSITY_RX_DEVICE = 1,
  /*!< Diversity Rx device */

  RFM_WCDMA_MAX_RX_DEVICE = 2,
  /*!< Max Rx devices supported in WCDMA per carrier */

  RFM_WCDMA_TX_DEVICE = RFM_WCDMA_MAX_RX_DEVICE,
  /*!< Tx device */

  RFM_WCDMA_INVALID_DEVICE
  /*!< INVALID DEVICE */

} rfm_wcdma_device_type;

/*! Enumeration to specify the rfm_wcdma subscription */
typedef enum
{
  RFM_WCDMA_SUB_0 = 0,
  /*!< Subscription 1 */
   
  RFM_WCDMA_SUB_1 = 1,
  /*!< Subscription 2 */

  RFM_WCDMA_NUM_SUBS =2,

  RFM_WCDMA_INVALID_SUB = RFM_WCDMA_NUM_SUBS

}rfm_wcdma_subscription_type;

/*----------------------------------------------------------------------------*/
/*! Macro defining maximum RFM devices supported for concurrent wakeup */
#define RFM_WCDMA_MAX_DEVICE RFM_MAX_WAN_DEVICES

/*----------------------------------------------------------------------------*/
/*! Macro defining maximum carriers supported on a device */
#define RFM_WCDMA_MAX_CARRIERS RFCOM_MAX_CARRIERS

/*----------------------------------------------------------------------------*/
/*Enumeration to specify the RF API being tested. Current use case is to enable RF state validation */

typedef enum
{
  RFM_WCDMA_ENTER_MODE,

  RFM_WCDMA_PREP_TUNE,

  RFM_WCDMA_EXEC_TUNE,

  RFM_WCDMA_PREP_WAKEUP,

  RFM_WCDMA_EXEC_WAKEUP,

  RFM_WCDMA_SLEEP,

  RFM_WCDMA_EXIT

}rfm_wcdma_api_type;

/*----------------------------------------------------------------------------*/

/*! 
  @brief
  Macro type definition for carrier mask. 

  @details
  Defintion of each bit is given by #rfm_wcdma_carrier_type. For example, if carrier 0 and carrier 2
  is to be enabled, carrier mask can be computed as
  (1<<RFM_WCDMA_CARRIER_0)|(1<<RFM_WCDMA_CARRIER_2)
*/
#define rfm_wcdma_carrier_mask_type uint8

/*! @cond vbhide */

/*----------------------------------------------------------------------------*/
/*! Structure to hold the parameters passed by L1 to wakeup WCDMA Rx  */
typedef struct
{
  struct
  {
     boolean is_valid;
     /*!< Flag to indicate if wakeup shall happen on this device */
 
     lm_handle_type rxlm_handle;
     /*!< RxLM buffer to be used to configure the modem hardware */
 
  } api_params[RFM_WCDMA_MAX_DEVICE];
  /*!< Structure to hold wakeup relevant parameters on all supported RFM devices */

}rfm_wcdma_rx_wakeup_params;

/*----------------------------------------------------------------------------*/
/*! Structure to hold the parameters passed by L1 to put WCDMA Rx to sleep  */
typedef struct
{
  struct
  {
     boolean is_valid;
     /*!< Flag to indicate if sleep shall happen on this device */
 
     lm_handle_type rxlm_handle;
     /*!< RxLM buffer to be used to configure the modem hardware */
 
     boolean skip_agc_disable; 
 
  } api_params[RFM_WCDMA_MAX_DEVICE];
  /*!< Structure to hold sleep relevant parameters on all supported RFM devices */

}rfm_wcdma_rx_sleep_params;

/*----------------------------------------------------------------------------*/
/*! Structure to hold the parameters passed by L1 to tune WCDMA Rx */
typedef struct
{
  struct
  {
     boolean is_valid;
     /*!< Flag to indicate if sleep shall happen on this device */
 
     lm_handle_type rxlm_handle;
     /*!< RxLM buffer to be used to configure the modem hardware */

     uint16 channel[RFM_WCDMA_MAX_CARRIERS];
     /*!< List of channels which need to be tuned on the particular device*/

     uint8 carrier_mask;
     /*!< Carrier mask to indicate the number of enabled carriers (0/1/2)*/
 
     rfm_wcdma_subscription_type sub_type;
     /*!< Sub Type to indicate Subscription details (0/1)*/

  } api_params[RFM_WCDMA_MAX_DEVICE];
  /*!< Structure to hold sleep relevant parameters on all supported RFM devices */

}rfm_wcdma_rx_tune_params;

/*----------------------------------------------------------------------------*/

/*! @endcond */

/*! Structure to hold the parameters passed by L1 to initialize WCDMA Tx */
typedef struct
{

  rfm_device_enum_type device;
  /*!< Variable to indicate the RF device to be initialzed for Tx   */

  lm_handle_type txlm_handle;
  /*!< TxLM buffer to be used to configure the modem hardware */

  rfm_wcdma_carrier_mask_type carrier_mask;
  /*!< Index to indicate the carrier configuration on the device*/

  uint16 tx_channel[RFM_WCDMA_MAX_CARRIERS];
  /*!< List of Tx ARFCNs which need to be tuned on the particular device*/

}rfm_wcdma_tx_init_params;
/*----------------------------------------------------------------------------*/

typedef struct
{

  rfm_device_enum_type device;
  /*!< Variable to indicate the RF device to wakeup Tx on   */

  lm_handle_type txlm_handle;
  /*!< TxLM buffer to be used to configure the modem hardware */

  rfm_wcdma_carrier_mask_type carrier_mask;
  /*!< Index to indicate the carrier configuration on the device*/

  uint16 tx_channel[RFM_WCDMA_MAX_CARRIERS];
  /*!< List of Tx ARFCNs which need to be tuned on the particular device*/

}rfm_wcdma_tx_wakeup_params;
/*----------------------------------------------------------------------------*/


/*! Structure to hold the parameters passed by L1 to deinit WCDMA Tx */
typedef struct
{
  
  rfm_device_enum_type device;
  /*!< Variable to indicate the RF device to be initialzed for Tx   */

}rfm_wcdma_tx_deinit_params; 
/*----------------------------------------------------------------------------*/

/*! Structure to hold the parameters passed by L1 to enable WCDMA Tx */
typedef struct
{
  
  rfm_device_enum_type device;
  /*!< Variable to indicate the RF device to be initialzed for Tx   */

  uint16 tx_channel[RFM_WCDMA_MAX_CARRIERS];
  /*!< List of Tx ARFCNs which need to be tuned on the particular device*/

}rfm_wcdma_tx_enable_params; 
/*----------------------------------------------------------------------------*/


/*! Structure to hold the parameters passed by L1 to disable WCDMA Tx */
typedef struct
{
  
  rfm_device_enum_type device;
  /*!< Variable to indicate the RF device to be initialzed for Tx   */

}rfm_wcdma_tx_disable_params; 
/*----------------------------------------------------------------------------*/
typedef struct
{
  uint8        xpt_mode;
  uint8        pa_range;

}rfm_wcdma_txagc_log_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure to hold the parameter which is to be sent to RF during 
temperature compensation update. */
typedef struct
{

  rfm_device_enum_type device;
  /*!< RFM device on which RF is requested to perform temperature 
  compensation update */

} rfm_wcdma_temp_comp_param;

/*----------------------------------------------------------------------------*/
/*! Structure to hold rfm_wcdma_tx_retune_bw param  */
typedef struct
{
  rfm_device_enum_type device;
  /*!< RFM device to tune*/

  uint32 multi_carrier_idx;
  /*!< Multi Carrier Index Bitmask */
}rfm_wcdma_tx_retune_bw_param;

/*----------------------------------------------------------------------------*/
/*! WCDMA specfic enter mode param structure  */
typedef struct
{
  rfm_wcdma_device_type device_type;
  /*!< Enter mode device type */

  rfm_device_enum_type prx_device;
  /*!< Associated PRx device. Need to be popolated for Tx enter*/

}rfm_wcdma_enter_mode_param;
/*----------------------------------------------------------------------------*/

/*! WCDMA specfic power on off param structure  */
typedef struct
{
  boolean on_off;
  /*!< Flag to indicate the power operation */

  rfm_device_enum_type device;
  /*!< rfm device for the the power operation */

  rfm_wcdma_subscription_type sub_type;
  /*!< rfm wcdma mode of operation  */
   
}rfm_wcdma_power_mgmt_param;

/*--------------------------------------------------------------------------*/
/*!
  @brief
  Identifies which antenna is being used
*/
typedef enum
{
   RFM_WCDMA_PRIMARY_ANTENNA = 0,
   /*!< Primary antenna*/

   RFM_WCDMA_DIVERSITY_ANTENNA = 1,
   /*!< Diversity antenna */

   RFM_WCDMA_MAX_ANTENNA,
   /*!< Max number of antennas */

   RFM_WCDMA_INVALID_ANTENNA = RFM_WCDMA_MAX_ANTENNA
}rfm_wcdma_antenna_index;
/*--------------------------------------------------------------------------*/
/*!
  @brief
  Identifies the current mode WCDMA is in
*/
typedef enum
{
   RFM_WCDMA_STANDALONE_MODE = 0,
   RFM_WCDMA_QTA_MODE = 1,
   RFM_WCDMA_MAX_MODE,   
   RFM_WCDMA_INVALID_MODE = RFM_WCDMA_MAX_MODE
}rfm_wcdma_oper_mode;

/*--------------------------------------------------------------------------*/

/*! 
  @brief
  Structure definition to get RxAGC value from MDSP 
*/
typedef struct
{
  rfm_device_enum_type device;
  rfcom_rxagc_type rxagc_type;
  rfcom_multi_carrier_id_type car_id;
  rfcom_rxagc_unit_type rxagc_unit;
  rfcom_wcdma_band_type band;
  rfm_wcdma_antenna_index ant_idx;
  rfm_wcdma_oper_mode oper_mode;  
}rfm_wcdma_rxagc_input_params;

/*--------------------------------------------------------------------------*/

/*! 
  @brief
  Structure definition which returns RxAGC value and RxAGC ON/OFF from MDSP 
*/
typedef struct
{
   int16 rxagc_val;
   boolean rxagc_on;
}rfm_wcdma_rxagc_output_params;

/*--------------------------------------------------------------------------*/
/*!
  @brief
  Structure definition which contains params to hold logging info
*/
typedef struct
{
   rfcom_wcdma_band_type band;
   rfm_device_enum_type rfm_device;
   rfm_wcdma_oper_mode oper_mode;
}rfm_device_log_params;

/*--------------------------------------------------------------------------*/

/*! WCDMA agc log specific structure */
typedef struct
{
   /*! array of prx devices for the num_carriers*/
   rfm_device_log_params rfm_device_prx [RFCOM_MULTI_CARRIER_ID__CUMULATIVE];
 
   /*! array of drx devices for the num_carriers*/
   rfm_device_log_params rfm_device_rxd [RFCOM_MULTI_CARRIER_ID__CUMULATIVE];
 
   /*! the tx device */
   rfm_device_enum_type rfm_device_tx;
 
   boolean 	single_instance;

}rfm_wcdma_log_agc_param_type;

/*!
  @brief
  Identifies RF subscriber.
*/
typedef enum
{
  RFM_WCDMA_LOG_AGC_HANDLE_0 = 0, /*!< AGC Log instance handle 0 */

  RFM_WCDMA_LOG_AGC_HANDLE_1 = 1, /*!< AGC Log instance handle 1 */

  RFM_WCDMA_LOG_AGC_MAX_HANDLE = 2 /*!< Indicates invalid  */

} rfm_wcdma_log_agc_handle_type;

/*! Structure to store Rx SNR parameters per carrier for Modem Assisted Antenna Tuning (MAAT) */
typedef struct
{
  /*Rx channel of the carrier*/  
  uint16 rx_channel;

  /* Rx Path Mask */
  uint8 rx_path_mask;

  /*< rfm path of each Rx antenna */
  rfm_path_type rfm_path[RFM_WCDMA_MAX_RX_DEVICE];

  /* Un-used RX Path EcIo should be filled with floored value */
  int32 ecio_db10[RFM_WCDMA_MAX_RX_DEVICE];
  
  /* Carrier ID */
  uint8 carrier_id; 
} rfm_wcdma_per_cc_maat_rx_params_type_s;

typedef struct
{
  /* ACTIVATED carrier mask */
  uint8 active_carrier_mask;
   
  /* MAAT RX params for the carriers */
  rfm_wcdma_per_cc_maat_rx_params_type_s carrier[RFM_WCDMA_CARRIER_MAX];
} rfm_wcdma_maat_rx_info_type_s;

/*----------------------------------------------------------------------------*/
rfm_wcdma_subscription_type rfwcdma_data_get_subscription_type
(
  rfm_device_enum_type device
);

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
