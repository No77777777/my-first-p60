#ifndef RFWCDMA_DATA_H
#define RFWCDMA_DATA_H

/*! 
  @file
  rfwcdma_data.h
 
  @brief
  This file contains prototypes and definitions to be used by Centralized 
  WCDMA Global Data.
*/

/*==============================================================================

  Copyright (c) 2013 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rftech_wcdma.mpss/3.11/rf/mc/inc/rfwcdma_data.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
06/04/20   nik     Changes to pass sub_id as param to WTR APIs to handle MSIM
01/24/20   nik     Changes for Minidump
05/24/16   ag      W+W logging changes for inside the W2W gap
10/28/15   asn     Add critical section for AGC logging when W is in both subs
10/28/15   asn     Add critical section between AOL update and Tx disable
10/06/15   rmb     Add check for Power Mask between standalone and NULL2W measurement.
09/29/15   sd      FR30081 AsDiv with eLNA gain compensation 
09/10/15   rmb     Fix to avoid race condition between Power On and Power off APIs.
07/28/15   ag       Expand defn of mdsp mc mutex lock variable to include more cases
07/27/15   rmb     Add Sub id in the RFWCDMA data.
6/29/15    vbh     populate PRx device param for enter mode
06/17/15   rmb     Add Handle mask for measurement and Critical section for TQ and MDSP
                   allocation/deallocation that happens in standalone and measurement.
06/09/15   ag      Add error state in rfwcdma_data_rf_state enum
05/30/15   dw      Added util functio n to get associated PRx device from Tx device
05/06/15   ak      Add routine to get the active rx mask
04/23/15   vbh     Align RFWCDMA_DATA_MAX_DEVICE with RFM_WCDMA_MAX_DEVICE
04/14/15   vbh     Added a routine to map_ant,carr  index to device
04/08/15   vbh     Added declrartion for validate device state
07/04/15   ak      Migrating to new WCDMA state machine
07/04/15   ak      Rx/Tx Split Changes
4/06/15    vbh     [Rx/Tx split] Add access functions definitions
3/31/15    vbh     [Rx/Tx split] Add enum for RF state, update state machine elements
2/20/15    vbh     MC state machine update to support CRAT
11/17/14   rmb     Move the debug data flags to rfwcdma_data_debug.h
06/26/14   vbh     Add a dbg flag for using script based tune from FTM
06/19/14   aro/ak  Added Support for Prep/Exec Temp Comp Split
06/06/14   ac      NBR support
05/16/14   aro     Added structure to hold power limiting data
02/04/14   vbh     Initial revision on Bolt
10/01/13   aro     Flag to enable v3 AGC log
09/30/13   aro     Doxygen Update
09/30/13   aro     Add Data module initialization
09/30/13   aro     Variable to enable/disable wakeup optimization
09/13/13   aro     Added data structure for critical section lock
09/13/13   aro     Initial Revision.

==============================================================================*/

#include "comdef.h"
#include "rfcommon_locks.h"
#include "rfwcdma_power_limiting.h"
#include "rfwcdma_core_temp_comp.h"
#include "rfwcdma_data_debug.h"
#include "rfwcdma_mc.h"
#include "rfm_wcdma_param_types.h"

#ifdef FEATURE_RF_ASDIV
#include "rfcommon_asdiv_manager.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------------------*/
/*! Enum to indicate the Max WCDMA device */
#define RFWCDMA_DATA_MAX_DEVICE RFM_WCDMA_MAX_DEVICE

/*----------------------------------------------------------------------------*/

typedef enum
{
 /*! @brief RFWCDMA State indicating default configuration after reset*/
  RFWCDMA_STATE_INIT,

 /*! @brief WCDMA State indicating that the device has entered WCDMA mode*/
  RFWCDMA_STATE_ENTER_MODE,

 /*! @brief WCDMA State indicating device in WCDMA mode, rails voted up*/
  RFWCDMA_STATE_POWER_ON,
  
/*! @brief WCDMA State indicating completion of building of scripts for Rx tune */
  RFWCDMA_STATE_RX_PREP,

/*! @brief WCDMA State indicating completion of Tx initialization */
  RFWCDMA_STATE_TX_PREP,

/*! @brief WCDMA State indicating completion of building of scripts for Rx Wakeup */
  RFWCDMA_STATE_RX_PREP_WAKEUP,
 
  /*! @brief WCDMA Rx state.  This state is entered after a successful Rx Tune/ Rx Wakeup. RxAGC up*/
  RFWCDMA_STATE_RX,

  /*! @brief WCDMA Tx state.  This state is entered after enable Tx and indicates TxAGC is running*/
  RFWCDMA_STATE_TX,


  /*! @brief RF WCDMA RX Sleep state.  This state is entered after Rx Sleep is called.*/
  RFWCDMA_STATE_RX_SLEEP,

 /*! @brief RFWCDMA TX Sleep state.  This state is entered when Tx Disable is called */
  RFWCDMA_STATE_TX_SLEEP,

/*! @brief RFWCDMA Power off state.  This state indicates that power rails have been turned off. 
    The device is in WCDMA state and needs Power ON before any tune/ wakeup*/
  RFWCDMA_STATE_POWER_OFF,

/*!@brief RFWCDMA error state - this state is only used for error check purpose only*/
  RFWCDMA_STATE_ERROR

}rfwcdma_data_rf_state_type;

/*! 
  @brief
  Structure Containing WCDMA radio status 
*/ 
typedef struct
{
  rfm_wcdma_device_type dev_type;
  /*!<brief Indicates the type of device: PRx, DRx, Tx etc*/
  
  rfm_wcdma_subscription_type sub_type;
  /*! brief Indicates the Subscription on which device is registered: Sub 0, Sub 1*/
  
  boolean is_tx_enabled;
  /*!<brief Flag indicating if Tx has been enabled*/

  rfwcdma_data_rf_state_type  rf_state;
  /*! <brief Maintains state of each path */

  boolean is_power_on;
  /*!<brief Flag indicating if power rails have been enabled*/

  rfcom_wcdma_band_type curr_band;
  /*! <brief Current band for each path. Default is invalid_band
   Indicates the current band for Rx/Tx based on the device type*/

  uint16 curr_chan[RFCOM_MAX_CARRIERS];
  /*! <brief Current band for each path. Default is invalid_band
   Indicates the current channel for Rx/Tx based on the device type*/

  uint8 rflm_handle;  
  /*! <RFLM buffer index used for the specific rx/tx path */

  uint32 carrier_mask;
  /*! <Multi_carrier mask  */

  uint32 asm_tuner_mask;
  /*! <ASM Tuner status bit map for a RFM device */

  uint32 carrier_mask_shadow;
  /*! <Multi_carrier mask  */

  rfcom_multi_carrier_hspa_id_type num_carriers;
  /*!< no. of rx/tx carriers */

  uint8 alt_path;  
  /*! < Alternate path index used for indicating the physicial rx path used depending on the concurrency */

  uint8 nv_container_index;
  /*!< NV container used for the current device based on the concurrency scenario*/

  uint8 associated_device_rx;
  /*!< Variable to indicate the associated primary or diversity device */

  uint8 associated_device_tx;
  /*!< Variable to indicate the associated Tx device */

  rfwcdma_temp_comp_data_type temp_comp;
  /*!< Data structur holding the temperature compensation data */

} rfwcdma_data_status_type;

/*! 
  @brief
  Structure containing radio status for neighbour WCDMA IRAT measurement
*/ 
typedef struct
{
  uint32 meas_enter_exit_handle_mask;
  /*!< brief Indicates mask for all the RFLM handles that has been through Meas Enter/Exit */

  uint32 meas_power_on_off_dev_mask;
  /*!< brief Indicates mask for all the RFM devices that has been through Meas Power on/off 
    in NULL2W measurement cases*/

  rfcom_wcdma_band_type meas_rfcom_band[RFM_WCDMA_MAX_DEVICE];
  /*!< brief Indicates band for the device inside the meas X2W gap*/
} rfwcdma_data_meas_type;

#ifdef FEATURE_RF_ASDIV
/*! 
  @brief
  Structure holding AsDiv callback info
*/ 
typedef struct
{
  /* AsDiv switch position after callback*/
  rfcommon_asdiv_position_type asdiv_cb_pos;
  
  /*AsDiv callback status, TRUE indicates cb is registered*/
  /*Flag to prevent multiple registrations and de-registrations */
  
  boolean asdiv_cb_reg_status;

  /*Registered cb handle*/
  rfcommon_asdiv_cb_handler_type asdiv_cb_handle;  
} rfwcdma_data_asdiv_type;
#endif

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Structure Containing Common Global Data used by WCDMA Driver. There should not
  be stray global variables. If global variable is required, it must be 
  contained within this structure. 
*/ 
typedef struct
{
  rfwcdma_data_status_type radio_status[RFWCDMA_DATA_MAX_DEVICE];
  /*!< Structure Containing WCDMA radio status  */

  /* Remember this Measurement data need to be updated by WCDMA Measurement apis */
  rfwcdma_data_meas_type meas_radio_data;
  /*!< Structure Containing radio status for neighbour WCDMA IRAT measurement */

  rf_lock_data_type *intf_crit_section;
  /*!< Global Mutex for Common WCDMA Driver */

  rf_lock_data_type tempcomp_crit_section;
  /*!< Global Mutex to avoid Temp Read and RF sleep */

  rf_lock_data_type meas_mc_tq_mdsp_crit_section;
  /*!< Global Mutex to avoid race condition of 
    1. Meas Enter and Standalone Exit mode
    2. Standalone Enter mode and Meas Exit
    3. Standalone Enter and Standalone Exit
    4. Meas Enter and Meas Exit
    against allocating/deallocating TQ pair and MDSP interface */

  rf_lock_data_type power_on_off_crit_section;
  /*!< Global Mutex to avoid race condition of 
   1. Power ON and OFF API in standalone on different Sub
   2. Power on/off in standalone on one Sub and Power on/off in NULL2W measurement on other Sub
   so that we can avoid accidental enable/disable of HAL BUS and Common RFC*/

  rf_lock_data_type  agc_log_crit_section;
  /*!< Global Mutex to avoid race condition where 
    W in both subscriptions is given the same log handle*/
  
  rf_lock_data_type tuner_update_crit_section;
  /*!< Global Mutex to avoid race condition between
  Tx disable and another AOL update. This is to ensure that no AOL update happens during Tx disable */	

  rfwcdma_data_debug_flags_type debug_flags;
  /*!< Data structure used to keep track of trmporary debug variables */

  rfwcdma_power_limiting_data_type plim_data;
  /*!< Structure to hold the power limiting data */

  #ifdef FEATURE_RF_ASDIV
  /*!< Stracture to hold AsDiv related data*/
  rfwcdma_data_asdiv_type asdiv_data;
  #endif
} rfwcdma_data_type;

/*----------------------------------------------------------------------------*/
void
rfwcdma_set_lock_data
(
  rf_lock_data_type *lock_data
);

/*----------------------------------------------------------------------------*/
rf_lock_data_type*
rfwcdma_get_lock_data
(
  void
);

/*----------------------------------------------------------------------------*/
rfwcdma_power_limiting_data_type*
rfwcdma_get_power_limiting_data
(
  void
);

/*----------------------------------------------------------------------------*/
boolean
rfwcdma_data_init
(
  void
);

/*----------------------------------------------------------------------------*/
rf_lock_data_type*
rfwcdma_get_temp_comp_lock_data
(
  void
);

/*----------------------------------------------------------------------------*/
rf_lock_data_type*
rfwcdma_get_power_on_off_lock_data
(
  void
);

/*----------------------------------------------------------------------------*/
rf_lock_data_type*
rfwcdma_get_agc_log_lock_data
(
  void
);

/*----------------------------------------------------------------------------*/
rf_lock_data_type*
rfwcdma_get_tuner_update_lock_data
(
  void
);

/*----------------------------------------------------------------------------*/
rfwcdma_temp_comp_data_type*
rfwcdma_data_get_temp_comp
(
   rfm_device_enum_type device
);

/*----------------------------------------------------------------------------*/
boolean
rfwcdma_data_init_radio_status
(
  rfm_device_enum_type device
);

/*----------------------------------------------------------------------------*/
boolean
rfwcdma_data_init_meas_data
(
  void
);
/*----------------------------------------------------------------------------*/

void rfwcdma_data_update_device_type
(
  rfm_device_enum_type device,
  rfm_wcdma_device_type dev_type
);

/*----------------------------------------------------------------------------*/

void rfwcdma_data_update_subscription_type
(
  rfm_device_enum_type device,
  rfm_wcdma_subscription_type sub_type
);

/*----------------------------------------------------------------------------*/

rfm_wcdma_device_type rfwcdma_data_get_device_type
(
  rfm_device_enum_type device
);

/*----------------------------------------------------------------------------*/
/*rfm_wcdma_subscription_type rfwcdma_data_get_subscription_type
(
  rfm_device_enum_type device
);*/

/*----------------------------------------------------------------------------*/
rf_lock_data_type* rfwcdma_data_get_meas_mc_mdsp_crit_sec_data
(
  void
);

/*----------------------------------------------------------------------------*/
uint32 rfwcdma_data_get_meas_enter_exit_handle_mask
(
  void
);

/*----------------------------------------------------------------------------*/
uint32 rfwcdma_data_get_meas_power_on_off_dev_mask
(
  void
);

/*----------------------------------------------------------------------------*/
boolean rfwcdma_data_udpate_meas_enter_exit_handle_mask
(
  uint32 handle_mask
);

/*----------------------------------------------------------------------------*/
boolean rfwcdma_data_udpate_meas_power_on_off_dev_mask
(
  uint32 dev_mask
);

/*----------------------------------------------------------------------------*/
uint32 rfwcdma_data_get_enabled_device_mask();
/*----------------------------------------------------------------------------*/
uint32 rfwcdma_data_get_rx_tune_device_mask();
/*----------------------------------------------------------------------------*/
void rfwcdma_data_update_tx_status
(
  rfm_device_enum_type device,
  boolean tx_on_flag
);
/*----------------------------------------------------------------------------*/

void rfwcdma_data_update_rf_state
(
  rfm_device_enum_type device,
  rfwcdma_data_rf_state_type rf_state
);
/*----------------------------------------------------------------------------*/

rfwcdma_data_rf_state_type rfwcdma_data_get_rf_state
(
  rfm_device_enum_type device
);
/*----------------------------------------------------------------------------*/

void rfwcdma_data_update_power_flag
(
  rfm_device_enum_type device,
  boolean power_flag
);
/*----------------------------------------------------------------------------*/

uint32 rfwcdma_data_get_power_mask();
/*----------------------------------------------------------------------------*/
uint32 rfwcdma_data_get_rx_power_mask();

/*----------------------------------------------------------------------------*/

rfcom_wcdma_band_type 
rfwcdma_data_get_meas_curr_band
(
rfcom_device_enum_type device
);

/*----------------------------------------------------------------------------*/

void 
rfwcdma_data_set_meas_curr_band
(
  rfcom_device_enum_type device,
  rfcom_wcdma_band_type curr_band
);

/*----------------------------------------------------------------------------*/

rfcom_wcdma_band_type 
rfwcdma_data_get_curr_band(rfcom_device_enum_type device);

/*----------------------------------------------------------------------------*/

void 
rfwcdma_data_set_curr_band
(
  rfcom_device_enum_type device,
  rfcom_wcdma_band_type curr_band_w_rx
);

/*----------------------------------------------------------------------------*/

uint16  
rfwcdma_data_get_curr_chan(
  rfcom_device_enum_type device,
  rfcom_multi_carrier_hspa_id_type carrier
);

/*----------------------------------------------------------------------------*/
void
rfwcdma_data_set_curr_chan
(
  rfcom_device_enum_type device, 
  rfcom_multi_carrier_hspa_id_type carrier,
  uint16 curr_chan
);

/*----------------------------------------------------------------------------*/
uint8  
rfwcdma_data_get_rflm_buf_idx(rfcom_device_enum_type device);

/*----------------------------------------------------------------------------*/
void  
rfwcdma_data_set_rflm_buf_idx
(
  rfcom_device_enum_type device,
  uint8 rxlm_buf_w_idx
);

/*----------------------------------------------------------------------------*/
uint32   
rfwcdma_data_get_carrier_mask(rfcom_device_enum_type device);

/*----------------------------------------------------------------------------*/
void   
rfwcdma_data_set_carrier_mask
(
  rfcom_device_enum_type device,
  uint32 rx_multi_carrier_mask
);

/*----------------------------------------------------------------------------*/
uint32   
rfwcdma_data_get_carrier_mask_shadow(rfcom_device_enum_type device);

/*----------------------------------------------------------------------------*/

void rfwcdma_data_set_carrier_mask_shadow
(
  rfcom_device_enum_type device,
  uint32 rx_carrier_mask
);

/*----------------------------------------------------------------------------*/

rfcom_multi_carrier_hspa_id_type   
rfwcdma_data_get_num_carriers(rfcom_device_enum_type device);
/*----------------------------------------------------------------------------*/
void   
rfwcdma_data_set_num_carriers
(
  rfcom_device_enum_type device,
  rfcom_multi_carrier_hspa_id_type rx_w_carriers
);

/*----------------------------------------------------------------------------*/
rfwcdma_temp_comp_data_type*
rfwcdma_data_get_temp_comp
(
   rfm_device_enum_type device
);

/*----------------------------------------------------------------------------*/
rfwcdma_temp_comp_data_type*
rfwcdma_data_get_temp_comp
(
   rfm_device_enum_type device
);
/*----------------------------------------------------------------------------*/
uint8 
rfwcdma_data_get_alt_path
(
  rfcom_device_enum_type device
);

/*----------------------------------------------------------------------------*/

uint8
rfwcdma_data_get_nv_conatiner
(
  rfcom_device_enum_type device
);
/*----------------------------------------------------------------------------*/

rfcom_device_enum_type
rfwcdma_data_get_associated_device_rx
(
   rfcom_device_enum_type device
);
/*----------------------------------------------------------------------------*/

rfcom_device_enum_type
rfwcdma_data_get_associated_device_tx
(
   rfcom_device_enum_type device
);
/*----------------------------------------------------------------------------*/

rfcom_device_enum_type
rfwcdma_data_get_associated_device_prx_from_tx
(
   rfcom_device_enum_type device
);

/*----------------------------------------------------------------------------*/

void rfwcdma_data_set_alt_path
(
  rfcom_device_enum_type device,
  uint8 alt_path
);

/*----------------------------------------------------------------------------*/
void
rfwcdma_data_set_nv_conatiner
(
  rfcom_device_enum_type device,
  uint8 nv_container_index
);
/*----------------------------------------------------------------------------*/

void
rfwcdma_data_set_associated_device_rx
(
  rfm_device_enum_type device,
  rfm_device_enum_type pair_dev
);

/*----------------------------------------------------------------------------*/

void
rfwcdma_data_set_associated_device_tx
(
  rfcom_device_enum_type device,
  rfm_device_enum_type pair_tx_device
);

/*----------------------------------------------------------------------------*/

rfwcdma_data_status_type* rfwcdma_get_radio_status
(
  rfcom_device_enum_type device
);

/*----------------------------------------------------------------------------*/

rfwcdma_data_meas_type* rfwcdma_get_meas_radio_data
(
  void
);

/*------------------------------------------------------------------------------------------------*/

void rfwcdma_data_set_prx_path
(
  rf_path_enum_type primary_path
);
/*------------------------------------------------------------------------------------------------*/

rf_path_enum_type rfwcdma_data_get_prx_path
(
   void
);

/*------------------------------------------------------------------------------------------------*/


rfm_device_enum_type rfwcdma_data_get_tx_device
(
void
);

/*------------------------------------------------------------------------------------------------*/

boolean rfwcdma_data_val_dev_type(rfm_device_enum_type device, rfm_wcdma_device_type dev_type);

/*------------------------------------------------------------------------------------------------*/
rfm_device_enum_type rfwcdma_data_map_ant_carr_to_device(uint8 ant_idx,
                                                         rfcom_multi_carrier_hspa_id_type carr_idx,
                                                         boolean is_tx);

/*----------------------------------------------------------------------------*/

rfcom_device_enum_type
rfwcdma_data_get_first_prx_device
(
  void
);
/*----------------------------------------------------------------------------*/
uint8 rfwcdma_data_get_handle(rfcom_device_enum_type device);

/*----------------------------------------------------------------------------*/
rfm_device_enum_type rfwcdma_data_get_tx_dev_frm_sub
(
  rfm_wcdma_subscription_type sub_id
);

#ifdef FEATURE_RF_ASDIV
void rfwcdma_data_init_asdiv_data(void);
/*----------------------------------------------------------------------------*/

void rfwcdma_data_set_asdiv_cb_handle(rfcommon_asdiv_cb_handler_type cb_handle);
/*----------------------------------------------------------------------------*/

rfcommon_asdiv_cb_handler_type rfwcdma_data_get_asdiv_cb_handle(void);
/*----------------------------------------------------------------------------*/

void rfwcdma_data_set_asdiv_cb_status(boolean cb_status);
/*----------------------------------------------------------------------------*/

boolean rfwcdma_data_get_asdiv_cb_status(void);

/*----------------------------------------------------------------------------*/
rfcommon_asdiv_position_type* rfwcdma_data_get_asdiv_cb_pos_ptr(void);
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/* New WCDMA variable to get and store all the global variables, so that passing this variable's address should fetch all the data in minindump API */

#ifdef FEATURE_QSH_MDUMP
rfwcdma_data_type*
rf_wcdma_data_get_wcdma_data
(
  void	
);
#endif

#ifdef __cplusplus
#endif

#endif /* RFWCDMA_DATA_H */
