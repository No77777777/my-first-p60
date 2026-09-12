/*! 
  @file
  rfwcdma_data.c
 
  @brief
  This file contains Centralized Global Data used by Common WCDMA Driver.
*/

/*==============================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rftech_wcdma.mpss/3.11/rf/mc/src/rfwcdma_data.c#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
06/04/20   nik     Changes to pass sub_id as param to WTR APIs to handle MSIM
01/24/20   nik     Changes for Minidump
12/31/19   nik     Removing redundant diag messages
02/22/18   nik     KW error fix
07/29/16   ag      Removal of unnecessary F3s
12/08/15   ak      Fix Infinite wait with customer's compiler
10/28/15   asn     Add critical section for AGC logging when W is in both subs
10/28/15   asn     Add critical section between AOL update and Tx disable
10/14/15   asn     Potential infinite loop in rfwcdma_data.c
10/06/15   rmb     Add check for Power Mask between standalone and NULL2W measurement.
09/29/15   sd      FR30081 AsDiv with eLNA gain compensation 
09/24/15   asn     Update F3 to print correct message
09/14/15   asn     Reduce WRF F3s
09/10/15   rmb     Fix to avoid race condition between Power On and Power off APIs.
09/09/15   ak      Add API to return the Tx device given the SUB ID
08/06/15   ag      Invalidate rflm handle properly while exit
07/27/15   rmb     Add Sub id in the RFWCDMA data.
07/15/15   ska	   Add a band check to get the first PRx device
06/30/15   sd      KW warning fix
6/29/15   vbh       populate PRx device param for enter mode
06/22/15   vbh     Update the RF msg type to Med/ Error at appropriate places.
06/17/15   rmb     Add Handle mask for measurement and Critical section for TQ and MDSP
                   allocation/deallocation that happens in standalone and measurement.
06/09/15   ag      Fix klocwork warnings
06/01/15   ak      Changes to Fix the Bug in API returning carrier shadow
05/30/15   dw      Added util function to get associated PRx device from Tx device
05/06/15   vbh     Added a routine to return the Rx Device mask
04/14/15   vbh     Added a routine to map_ant,carr  index to device
04/13/15   ak      Rx/Tx Split Changes for the new CRAT design
07/04/15   ak      Migrating to new WCDMA state machine
07/04/15   ak      Rx/Tx Split Changes
4/6/15     vbh     [Rx/Tx split]Access function implementation
3/31/15    vbh     [Rx/Tx split] Add access functions definitions
2/20/14    vbh     Add helper functions for the MC state machine.
12/10/14   vs      Added flag to enable nbee debug info
11/17/14   rmb     Added Dynamic spur mitigation feature flag. 
07/10/14   aa      Enable plim
06/30/14   aro     Added Plim Data Init function
06/26/14   vbh     Initialize the dbg flag for script based tune 
06/19/14   aro     Added Support for Prep/Exec Temp Comp Split
06/06/14   ac      nbr support
05/15/14   aro     Interface to override TxPlim override mode
02/04/14   vbh     Initial revision on Bolt  
10/01/13   aro     Converted variable to non STATIC
09/30/13   aro     Doxygen Update
09/30/13   aro     Add Data module initialization
09/30/13   aro     Variable to enable/disable wakeup optimization
09/13/13   aro     Added data structure for critical section lock
09/13/13   aro     Initial Revision.

==============================================================================*/

#include "comdef.h"
#include "rfwcdma_data.h"
#include "rfcommon_msg.h"
#include "rfwcdma_core_util.h"
#include "rfc_common.h"

#ifdef FEATURE_QSH_MDUMP
#include "qsh.h" 
#endif

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Variable to store global Data to be used by WCDMA Driver
*/
static rfwcdma_data_type rfwcdma_data;

/*----------------------------------------------------------------------------*/
/*! Temporary Flag to enable wakeup optimzation */
boolean rfwcdma_enable_wakeup_opt = TRUE;

uint16 rfwcdma_data_init_called =0; 
/*============================================================================*/
/*!
  @name Critical Section

  @brief
  This section includes interfaces to update and query the critical section
  data
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Set WCDMA Lock Data

  @details
  This function will set the WCDMA LOCK data.

  @param lock_data
  Lock data to be set as WCDMA Lock
*/
void
rfwcdma_set_lock_data
(
  rf_lock_data_type *lock_data
)
{
  rfwcdma_data.intf_crit_section = lock_data;
} /* rf_cdma_get_lock_data */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  GetW CDMA Lock Data

  @details
  This function will return the WCDMA LOCK data.

  @return
  Writeable Pointer to WCDMA Lock Data
*/
rf_lock_data_type*
rfwcdma_get_lock_data
(
  void
)
{

  /* throw error message if NULL pointer */
  if ( rfwcdma_data.intf_crit_section == NULL )
  {
    RF_MSG( RF_ERROR,"rfwcdma_get_lock_data: Null pointer !! "
                     "Critical Section not initialized !!");
  } /* if ( rfwcdma_mc_state.crit_section == NULL ) */

  return rfwcdma_data.intf_crit_section;
} /* rfwcdma_get_lock_data */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get WCDMA temp comp Lock Data

  @details
  This function will return the WCDMA temp comp LOCK data.

  @return
  Writeable Pointer to WCDMA temp comp Lock Data
*/
rf_lock_data_type*
rfwcdma_get_temp_comp_lock_data
(
  void
)
{

  return &(rfwcdma_data.tempcomp_crit_section);
} /* rfwcdma_get_temp_comp_lock_data */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get WCDMA data for Power on/off

  @details
  This function will return the WCDMA data for Power on/off

  @return
  Writeable Pointer to WCDMA data for Power on/off
*/
rf_lock_data_type*
rfwcdma_get_power_on_off_lock_data
(
  void
)
{

  return &(rfwcdma_data.power_on_off_crit_section);
} /* rfwcdma_get_temp_comp_lock_data */

/*! @} */
/*----------------------------------------------------------------------------*/
/*!
@brief
Get WCDMA lock data for RxAGC logging
	 
@details
This function will return the WCDMA data for the lock for RxAGC logging

@return
Writeable Pointer to WCDMA lock
*/
rf_lock_data_type*
rfwcdma_get_agc_log_lock_data
(
  void
)
{
  return &(rfwcdma_data.agc_log_crit_section);
} /* rfwcdma_get_tuner_update_lock_data */
/*----------------------------------------------------------------------------*/

/*!
@brief
Get WCDMA lock data for AOL update
	 
@details
This function will return the WCDMA data for the lock for AOL

@return
Writeable Pointer to WCDMA lock
*/
rf_lock_data_type*
rfwcdma_get_tuner_update_lock_data
(
  void
)
{
  return &(rfwcdma_data.tuner_update_crit_section);
} /* rfwcdma_get_tuner_update_lock_data */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Init WCDMA radio status

  @details
  This function will initialize WCDMA radio status structure.
 
  @param device_idx
  Structure corresponding to device needs to be initialized
 
  @return
  Status indicating successful initialization
*/

boolean
rfwcdma_data_init_radio_status
(
  rfm_device_enum_type device
)
{
  boolean ret_val = TRUE;
  uint8 carr_idx; 

  if ( device < RFWCDMA_DATA_MAX_DEVICE )
    {
    memset( &(rfwcdma_data.radio_status[device]),0x0,sizeof(rfwcdma_data_status_type) );
    rfwcdma_data.radio_status[device].dev_type = RFM_WCDMA_INVALID_DEVICE;
    rfwcdma_data.radio_status[device].sub_type = RFM_WCDMA_INVALID_SUB;
    rfwcdma_data.radio_status[device].is_tx_enabled = FALSE; 
    rfwcdma_data.radio_status[device].rf_state = RFWCDMA_STATE_INIT;
    rfwcdma_data.radio_status[device].is_power_on = FALSE;
    rfwcdma_data.radio_status[device].curr_band = RFCOM_BAND_INVALID;
    rfwcdma_data.radio_status[device].rflm_handle = 0xFF;
    rfwcdma_data.radio_status[device].carrier_mask = 0;
    rfwcdma_data.radio_status[device].carrier_mask_shadow = 0;
    rfwcdma_data.radio_status[device].num_carriers = RFCOM_MAX_CARRIERS;
    rfwcdma_data.radio_status[device].alt_path =0;
    rfwcdma_data.radio_status[device].nv_container_index =0; 
    rfwcdma_data.radio_status[device].associated_device_rx = RFM_INVALID_DEVICE;
    rfwcdma_data.radio_status[device].associated_device_rx = RFM_INVALID_DEVICE;

	for (carr_idx=0 ; carr_idx<RFCOM_MAX_CARRIERS; carr_idx++)
	{
	  rfwcdma_data.radio_status[device].curr_chan[carr_idx] = 0;
    }
    
  } /* if ( device <= RFWCDMA_DATA_MAX_DEVICE ) */
  else
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_data_init_radio_status: Invalid Dev %d",
              device );
    ret_val = FALSE;
  } /* if! ( device <= RFM_DEVICE_3 ) */

  return ret_val;

}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Init WCDMA Meas radio data

  @details
  This function will initialize WCDMA meas data strcuture.
 
  @return
  Status indicating successful initialization
*/
boolean
rfwcdma_data_init_meas_data
(
  void
)
{
  memset( &(rfwcdma_data.meas_radio_data),0x0,sizeof(rfwcdma_data_meas_type) );

  return TRUE;
}
/*============================================================================*/
/*!
  @name Debug Flags

  @brief
  This section includes interfaces to update and query the debug flags used
  to WCDMA driver
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Init WCDMA debug flags

  @details
  This function will initialize WCDMA debug flags
*/
static void
rfwcdma_init_debug_flags
(
  void
)
{
  rfwcdma_data.debug_flags.enable_opt_wakeup = rfwcdma_enable_wakeup_opt;
  
  rfwcdma_data.debug_flags.enable_rf_nbr_operations = TRUE;
  
  rfwcdma_data.debug_flags.enable_script_based_tune = FALSE; 
  
  rfwcdma_data.debug_flags.apply_notch_msm = TRUE;
  
  rfwcdma_data.debug_flags.enable_nbee_cal_dbg = FALSE;

  rfwcdma_data.debug_flags.prx_path = RF_PATH_0;
  
} /* rfwcdma_init_debug_flags */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get WCDMA debug flags

  @details
  This function will return WCDMA debug flags

  @return
  Writeable Pointer to WCDMA debug flags
*/
rfwcdma_data_debug_flags_type*
rfwcdma_get_debug_flags
(
  void
)
{

  return &(rfwcdma_data.debug_flags);

} /* rfwcdma_get_debug_flags */

/*! @} */


/*============================================================================*/
/*!
  @name Power Limiting

  @brief
  This section includes interfaces used for WCDMA power limiting
*/
/*! @{ */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get WCDMA Power limiting data

  @details
  This function will return WCDMA power limiting data

  @return
  Writeable Pointer to WCDMA power limiting data
*/
rfwcdma_power_limiting_data_type*
rfwcdma_get_power_limiting_data
(
  void
)
{
  return &(rfwcdma_data.plim_data);

} /* rfwcdma_get_power_limiting_data */

/*! @} */


/*============================================================================*/
/*!
  @name Data Initialization

  @brief
  This section includes interfaces pertaining to initialization of WCDMA
  Data module
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Init WCDMA Power limiting data

  @details
  This function will initialize WCDMA power limiting data
*/
static void
rfwcdma_init_data_plim
(
  void
)
{

  /* Enable Plim by default. During Tx initialization, HDET NV will also be checked to enable or
  disable PLIM. This flag is mainly used for debugging purpose*/
  rfwcdma_data.plim_data.enable_plim = TRUE;

} /* rfwcdma_init_data_plim */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Init WCDMA Data module

  @details
  This function will initialize WCDMA data module
*/
boolean
rfwcdma_data_init
(
  void
)
{
  rfm_device_enum_type dev_idx;
  boolean ret_val = TRUE;

  for ( dev_idx = RFM_DEVICE_0; dev_idx < RFWCDMA_DATA_MAX_DEVICE; dev_idx++ )
  {
    ret_val &= rfwcdma_data_init_radio_status(dev_idx );

  } /* for ( dev_idx = RFM_DEVICE_0; dev_idx <= RFWCDMA_DATA_MAX_DEVICE; dev_idx++ ) */
  
  rfwcdma_init_debug_flags();

  rfwcdma_init_data_plim();

#ifdef FEATURE_RF_ASDIV
  rfwcdma_data_init_asdiv_data();
#endif
  
  rfwcdma_data_init_called +=1;
  
  return ret_val;

} /* rfwcdma_init_data */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get pointer to temperature compensation data
 
  @param device
  RFM device for which the temp comp data is requested
 
  @return
  Pointer to temperature compensation data for the given device
*/
rfwcdma_temp_comp_data_type*
rfwcdma_data_get_temp_comp
(
   rfm_device_enum_type device
)
{
  if ( device < RFWCDMA_DATA_MAX_DEVICE )
  {
    return &(rfwcdma_data.radio_status[device].temp_comp);
  } /* if ( device < RFWCDMA_DATA_MAX_DEVICE ) */

  else
  {
    return NULL;
  } /* if ( device < RFWCDMA_DATA_MAX_DEVICE ) */
} /* rfwcdma_data_get_temp_comp */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get pointer to radio status data
 
  @param device
  RFM device for which the radio status data is requested
 
  @return
  Pointer to radio status data for the given device
*/

rfwcdma_data_status_type* rfwcdma_get_radio_status
(
  rfcom_device_enum_type device
)
{
  if ( device < RFWCDMA_DATA_MAX_DEVICE )
  {
    return &(rfwcdma_data.radio_status[device]);
  } /* if ( device < RFWCDMA_DATA_MAX_DEVICE ) */
  else
  {
    return NULL;
  } /* if ( device < RFWCDMA_DATA_MAX_DEVICE ) */
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get pointer to radio status IRAT measurement data
 
  @param
  None
 
  @return
  Pointer to radio status measurement data for the given device
*/

rfwcdma_data_meas_type* rfwcdma_get_meas_radio_data
(
  void
)
{
  return &(rfwcdma_data.meas_radio_data);
}

/*----------------------------------------------------------------------------*/

/*!
  @brief
  Set PRx path
 
  @param device
  RFM device 
 
  @return
  void
*/

void rfwcdma_data_set_prx_path
(
  rf_path_enum_type primary_path
)
{
  rfwcdma_data.debug_flags.prx_path = primary_path;
  RF_MSG_1(RF_MED,"rfwcdma_data: Primary Rx path set to :%d", primary_path);
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get Primary Rx path
 
  @param device
  void  
 
  @return
  rf_path_enum_type primary rx path
*/

rf_path_enum_type rfwcdma_data_get_prx_path
(
   void
)
{
  return rfwcdma_data.debug_flags.prx_path;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Update device type 
 
  @param device
  RFM Device
 
  @param dev_type
  RFM WCDMA device type - PRx, DRx, Tx
 
  @return
  none
*/

void rfwcdma_data_update_device_type
(
  rfm_device_enum_type device,
  rfm_wcdma_device_type dev_type
)
{
  rfwcdma_data.radio_status[device].dev_type = dev_type;
  RF_MSG_2(RF_MED,"rfwcdma_data_update_device_type: RFM device %d type updated to %d",device, dev_type); 
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Update Subscription type 
 
  @param device
  RFM Device
 
  @param sub_type
  rfm_wcdma_subscription_type - Sub 0/Sub 1
 
  @return
  none
*/

void rfwcdma_data_update_subscription_type
(
  rfm_device_enum_type device,
  rfm_wcdma_subscription_type sub_type
)
{
  rfwcdma_data.radio_status[device].sub_type = sub_type;
  RF_MSG_2(RF_MED,"rfwcdma_data_update_subscription_type: RFM device %d type updated to %d",device, sub_type);
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get device type 
 
  @param device
  RFM Device
 
  @return dev_type
  rfm_wcdma_device_type - PRx/DRx/Tx
*/

rfm_wcdma_device_type rfwcdma_data_get_device_type
(
  rfm_device_enum_type device
)
{
      if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
	    {
	      RF_MSG_1(RF_ERROR, "rfwcdma_data_get_device_type: Invalid device %d", device);
	      return RFM_WCDMA_INVALID_DEVICE;
            }

  return rfwcdma_data.radio_status[device].dev_type;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get Subscription type for that particular device
 
  @param device
  RFM Device
 
  @return dev_type
  rfm_wcdma_subscription_type - Sub 0/Sub 1
*/

rfm_wcdma_subscription_type rfwcdma_data_get_subscription_type
(
  rfm_device_enum_type device
)
{
  if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
  {
    RF_MSG_1(RF_ERROR, "rfwcdma_data_get_subscription_type: Invalid device %d", device);
    return RFM_WCDMA_INVALID_SUB;
  }
  else
  {
    RF_MSG_2(RF_MED,"rfwcdma_data_get_subscription_type: [ndebug] RFM device %d type updated to %d", device, rfwcdma_data.radio_status[device].sub_type);
  }

  return rfwcdma_data.radio_status[device].sub_type;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get pointer to the TQ MDSP critical section common to both Measurement and Common data
 
  @param
  None
 
  @return rf_lock_data_type
  Pointer to TQ MDSP critical section data
*/
rf_lock_data_type* rfwcdma_data_get_meas_mc_mdsp_crit_sec_data
(
  void
)
{
  return &(rfwcdma_data.meas_mc_tq_mdsp_crit_section);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get handle mask for all devices that pass through Meas Enter/Exit
 
  @param
  None
 
  @return 
  uint32 Handle mask in Meas Enter/Exit
*/

uint32 rfwcdma_data_get_meas_enter_exit_handle_mask
(
  void
)
{
  return (rfwcdma_data.meas_radio_data.meas_enter_exit_handle_mask);
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get device mask for all devices that pass through Meas Power on/off
 
  @param
  None
 
  @return 
  uint32 device mask that has gone through Meas Power on/off
*/

uint32 rfwcdma_data_get_meas_power_on_off_dev_mask
(
  void
)
{
  return (rfwcdma_data.meas_radio_data.meas_power_on_off_dev_mask);
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Update handle mask to the Meas Radio data
 
  @param
  None
 
  @return 
  boolean flag indicating update status
*/

boolean rfwcdma_data_udpate_meas_enter_exit_handle_mask
(
  uint32 handle_mask
)
{
  rfwcdma_data.meas_radio_data.meas_enter_exit_handle_mask = handle_mask;
  
  return TRUE;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Update dev mask to the Meas Power on/off dev mask in Meas Radio data
 
  @param
  None
 
  @return 
  boolean flag indicating update status
*/

boolean rfwcdma_data_udpate_meas_power_on_off_dev_mask
(
  uint32 dev_mask
)
{
  rfwcdma_data.meas_radio_data.meas_power_on_off_dev_mask = dev_mask;
  
  return TRUE;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get enabled device mask 
 
  @param void
  
  @return 
  uint32 type device mask indicating the WCDMA enabled devices 
*/
uint32 rfwcdma_data_get_enabled_device_mask()
{
  uint32 dev_mask = 0, dev_loop =0;
  for (dev_loop =0; dev_loop < RFWCDMA_DATA_MAX_DEVICE; dev_loop ++ )
  {
    if (rfwcdma_data.radio_status[dev_loop].rf_state != RFWCDMA_STATE_INIT)
    {
       dev_mask |= 1<<dev_loop;
    }
  }
  return dev_mask;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get Rx tune device mask 
  This API will return device mask of all devices in RX_PREP,RX or RX_PREP_WAKEUP states
  @param void
  
  @return 
  uint32 type device mask indicating the WCDMA tune devices 
*/
uint32 rfwcdma_data_get_rx_tune_device_mask()
{
  uint32 dev_mask = 0, dev_loop =0;
  rfwcdma_data_rf_state_type rf_state;
  
  for (dev_loop =0; dev_loop < RFWCDMA_DATA_MAX_DEVICE; dev_loop ++ )
  {
    rf_state = rfwcdma_data.radio_status[dev_loop].rf_state;
	
    if (rf_state == RFWCDMA_STATE_RX_PREP || rf_state == RFWCDMA_STATE_RX ||rf_state == RFWCDMA_STATE_RX_PREP_WAKEUP)
    {
       dev_mask |= 1<<dev_loop;
    }
  }
  return dev_mask;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This API will set the alt path in rfwcdma_data
  @param device
  @param alt_path
   
*/
void rfwcdma_data_set_alt_path
(
  rfcom_device_enum_type device,
  uint8 alt_path
)
{
  if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
  {
	RF_MSG_1(RF_ERROR, "rfwcdma_data_set_alt_path: Invalid device %d", device);
	return;
  }
  rfwcdma_data.radio_status[device].alt_path = alt_path;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This API will get the alt path from rfwcdma_data
  @param device

  @return 
  uint 8 alt_path   
*/

uint8 
rfwcdma_data_get_alt_path
(
  rfcom_device_enum_type device
)
{
  if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
  {
	RF_MSG_1(RF_ERROR, "rfwcdma_data_get_alt_path: Invalid device %d", device);
	return 0;
  }

  return rfwcdma_data.radio_status[device].alt_path; 
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Update Tx status
 
  @param device
  RFM Device
 
  @param tx_on_flag
  Boolean indicating TRUE or FALSE
 
  @return
  void
  
*/
void rfwcdma_data_update_tx_status
(
  rfm_device_enum_type device,
  boolean tx_on_flag
)
{
  rfwcdma_data.radio_status[device].is_tx_enabled = tx_on_flag;
  RF_MSG_2(RF_MED,"rfwcdma_data_update_tx_status: Tx ON is %d for device %d", tx_on_flag,device);
}
/*----------------------------------------------------------------------------*/

/*!
  @brief
  Get radio state on a device
 
  @param device
  RFM Device
 
  @return 
  rfwcdma_data_rf_state_type state
*/
rfwcdma_data_rf_state_type rfwcdma_data_get_rf_state
(
  rfm_device_enum_type device
)
{
   if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
	 {
	    //RF_MSG_1(RF_ERROR, "rfwcdma_data_get_rf_state: Invalid device %d", device);
            return  RFWCDMA_STATE_ERROR;
	 }
  return rfwcdma_data.radio_status[device].rf_state;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Update radio state on a device
 
  @param device
  RFM Device
 
  @param rf_state
  rfwcdma_data_rf_state_type rf state
 
  @return void
*/

void rfwcdma_data_update_rf_state
(
  rfm_device_enum_type device,
  rfwcdma_data_rf_state_type rf_state
)
{

  if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
	 {
	    RF_MSG_1(RF_MED, "rfwcdma_data_update_rf_state: Invalid device %d", device);
            return ;
	 }
  
  RF_MSG_3(RF_MED,"rfwcdma_data_update_rf_state: Device %d, State Update %d -> %d", 
           device,
           rfwcdma_data.radio_status[device].rf_state,
           rf_state);

  rfwcdma_data.radio_status[device].rf_state = rf_state; 
}
/*----------------------------------------------------------------------------*/

/*!
  @brief
  Update power enable flag 
 
  @param device
  RFM Device
 
  @param power_flag
  Boolean indicating TRUE or FALSE
 
  @return
  void
  
*/

void rfwcdma_data_update_power_flag
(
  rfm_device_enum_type device,
  boolean power_flag
)
{
  rfwcdma_data.radio_status[device].is_power_on = power_flag;
  RF_MSG_2(RF_MED,"rfwcdma_data_update_power_flag: For device %d, power is now %d", device, power_flag);
}

/*----------------------------------------------------------------------------*/


/*!
  @brief
  Get device power mask This routine provides a mask indicating the devices which are powered ON
 
  @param void
  
  @return 
  uint32 type device mask indicating the powered up devices 
*/
uint32 rfwcdma_data_get_power_mask()
{
  uint32 dev_mask = 0, dev_loop =0;
  for (dev_loop =0; dev_loop < RFWCDMA_DATA_MAX_DEVICE; dev_loop ++ )
  {
    if (rfwcdma_data.radio_status[dev_loop].is_power_on == TRUE)
    {
       dev_mask |= 1<<dev_loop;
    }
  }
  return dev_mask;
}
/*----------------------------------------------------------------------------*/

/*!
  @brief
  Get device power mask This routine provides a mask indicating the devices which are powered ON
 
  @param void
  
  @return 
  uint32 type device mask indicating the powered up devices 
*/
uint32 rfwcdma_data_get_rx_power_mask()
{
  uint32 dev_mask = 0, dev_loop =0;
  for (dev_loop =0; dev_loop < RFWCDMA_DATA_MAX_DEVICE; dev_loop ++ )
  {
    if(rfwcdma_data.radio_status[dev_loop].dev_type != RFM_WCDMA_TX_DEVICE)
    {
      if (rfwcdma_data.radio_status[dev_loop].is_power_on == TRUE)
      {
        dev_mask |= 1<<dev_loop;
      }
    }
   }
  return dev_mask;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get meas band, this routine provide band inside X2W gap for the asked device
 
  @param device
  
  @return 
  uint32 band for device inside X2W gap
*/
rfcom_wcdma_band_type 
rfwcdma_data_get_meas_curr_band(rfcom_device_enum_type device)
{
   if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
	{
	   RF_MSG_1(RF_MED, "rfwcdma_data_get_curr_band: Invalid device %d", device);
	   return RFCOM_BAND_IMT;
        }

  return rfwcdma_data.meas_radio_data.meas_rfcom_band[device]; 
}

/*----------------------------------------------------------------------------*/
/*!
@brief
  Set meas band, this routine sets up band inside X2W gap for the asked device
 
  @param void
  
  @return boid
*/
void 
rfwcdma_data_set_meas_curr_band
(
  rfcom_device_enum_type device,
  rfcom_wcdma_band_type curr_band
)
{ 
  rfwcdma_data.meas_radio_data.meas_rfcom_band[device] = curr_band;  
}

/*----------------------------------------------------------------------------*/

/*!
  @brief
  Get current RF band of operation 
 
  @param device
  RFM Device
 
  @return
  rfcom_wcdma_band_type band 
*/
rfcom_wcdma_band_type 
rfwcdma_data_get_curr_band(rfcom_device_enum_type device)
{
   if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
	{
	   RF_MSG_1(RF_MED, "rfwcdma_data_get_curr_band: Invalid device %d", device);
	   return RFCOM_BAND_IMT;
        }

  return rfwcdma_data.radio_status[device].curr_band; 
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Set rf band 
 
  @param device
  RFM Device
 
  @return
  void
*/
void 
rfwcdma_data_set_curr_band
(
  rfcom_device_enum_type device,
  rfcom_wcdma_band_type curr_band
)
{
  rfm_wcdma_device_type device_type = rfwcdma_data.radio_status[device].dev_type;

  rfwcdma_data.radio_status[device].curr_band = curr_band;
 
  if (device_type == RFM_WCDMA_PRIMARY_RX_DEVICE || device_type == RFM_WCDMA_DIVERSITY_RX_DEVICE ) 
  {
    RF_MSG_2(RF_MED,"rfwcdma_data_set_curr_band: Rx band %d on device %d",
           curr_band,
           device);
  }
  else if (device_type == RFM_WCDMA_TX_DEVICE) 
  {
    RF_MSG_2(RF_MED,"rfwcdma_data_set_curr_band: Tx band %d on device %d",
             curr_band,
             device); 
             
  }
  else
  {
    RF_MSG_1(RF_MED,"rfwcdma_data_set_curr_band: RF Band update FAILED! device %d has invalid type",
           device);
    return;
  }
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get current RF chan of operation 
 
  @param device
  RFM Device
 
  @return
  rfcom_wcdma_band_type band 
*/
uint16 
rfwcdma_data_get_curr_chan(rfcom_device_enum_type device, rfcom_multi_carrier_hspa_id_type carrier)
{
  if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
  {
    RF_MSG_1(RF_MED, "rfwcdma_data_get_curr_chan: Invalid device %d", device);
	return 65535;
  }

  return rfwcdma_data.radio_status[device].curr_chan[carrier]; 

}

/*----------------------------------------------------------------------------*/

/*!
  @brief
  Set rf band 
 
  @param device
  RFM Device
 
  @return
  void
*/

void
rfwcdma_data_set_curr_chan
(
  rfcom_device_enum_type device, 
  rfcom_multi_carrier_hspa_id_type carrier,
  uint16 curr_chan
)
{
  rfm_wcdma_device_type device_type = rfwcdma_data.radio_status[device].dev_type;

  rfwcdma_data.radio_status[device].curr_chan[carrier] = curr_chan;
 
  if (device_type == RFM_WCDMA_PRIMARY_RX_DEVICE || device_type == RFM_WCDMA_DIVERSITY_RX_DEVICE ) 
  {
    RF_MSG_3(RF_MED,"rfwcdma_data_set_curr_chan: Rx chan[%d]- %d on device %d",
             carrier,
             curr_chan,
             device);
  }
  else if (device_type == RFM_WCDMA_TX_DEVICE) 
  {
    RF_MSG_3(RF_MED,"rfwcdma_data_set_curr_chan: Tx chan[%d] - %d on device %d",
             carrier,
             curr_chan,
             device); 
             
  }
  else
  {
    RF_MSG_1(RF_MED,"rfwcdma_data_set_curr_chan: RF Chan update FAILED! device %d has invalid type",
           device);
    return;
  }
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get RFLM handle 
 
  @param device
  RFM Device
 
  @return
  unit8 type rflm handle
*/

uint8  
rfwcdma_data_get_rflm_buf_idx(rfcom_device_enum_type device)
{
  if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
  {
    RF_MSG_1(RF_MED, "rfwcdma_data_get_rflm_buf_idx: Invalid device %d", device);
    return 255;
  }

  return rfwcdma_data.radio_status[device].rflm_handle;
}

/*----------------------------------------------------------------------------*/

/*!
  @brief
  Set rflm_handle 
 
  @param device
  RFM Device
 
  @return
  void
*/
void rfwcdma_data_set_rflm_buf_idx
(
  rfcom_device_enum_type device,
  uint8 rflm_buff_idx
)
{
  rfm_wcdma_device_type device_type = rfwcdma_data.radio_status[device].dev_type;

  rfwcdma_data.radio_status[device].rflm_handle = rflm_buff_idx;
 
  if (device_type == RFM_WCDMA_PRIMARY_RX_DEVICE || device_type == RFM_WCDMA_DIVERSITY_RX_DEVICE ) 
  {
    RF_MSG_2(RF_MED,"rfwcdma_data_set_rflm_buf_idx: Rx handle update on device %d, handle %d",
           device, 
           rflm_buff_idx);
  }
  else if (device_type == RFM_WCDMA_TX_DEVICE) 
  {
    RF_MSG_2(RF_MED,"rfwcdma_data_set_rflm_buf_idx: Tx handle update on device %d, handle %d",
             device, 
             rflm_buff_idx);
  }
  else
  {
    RF_MSG_1(RF_ERROR,"rfwcdma_data_set_rflm_buf_idx: RF handle update FAILED! device %d invalid type",
           device);
    return;
  }
}

/*----------------------------------------------------------------------------*/

/*!
  @brief
  Get current carrier mask 
 
  @param device
  RFM Device
 
  @return
  uint32 type carrier mask 
*/

uint32 rfwcdma_data_get_carrier_mask(rfcom_device_enum_type device)
{
 return rfwcdma_data.radio_status[device].carrier_mask;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Set carrier mask 
 
  @param device
  RFM Device
 
  @return
  void
*/

void rfwcdma_data_set_carrier_mask
(
  rfcom_device_enum_type device,
  uint32 rx_carrier_mask
)
{
  rfwcdma_data.radio_status[device].carrier_mask = rx_carrier_mask; 
  RF_MSG_2(RF_MED,"rfwcdma_data_set_carrier_mask: device %d, carrier_mask %d", 
           device, 
           rx_carrier_mask);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get carrier mask shadow
 
  @param device
  RFM Device
 
  @return
  uint32 type carrier mask 
*/

uint32 rfwcdma_data_get_carrier_mask_shadow(rfcom_device_enum_type device)
{
  return rfwcdma_data.radio_status[device].carrier_mask_shadow;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Set carrier mask 
 
  @param device
  RFM Device
 
  @return
  void
*/

void rfwcdma_data_set_carrier_mask_shadow
(
  rfcom_device_enum_type device,
  uint32 rx_carrier_mask
)
{
  rfwcdma_data.radio_status[device].carrier_mask_shadow = rx_carrier_mask; 
  RF_MSG_2(RF_MED,"rfwcdma_data_set_carrier_mask_shadow: device %d, carrier_mask %d",
           device, 
           rx_carrier_mask);
}


/*!
  @brief
  Get Tx device
 
  @param device
  Void
 
  @return
  void
*/
rfm_device_enum_type rfwcdma_data_get_tx_device
(
  void
)
{
  rfm_device_enum_type device = RFM_INVALID_DEVICE;
  rfm_device_enum_type ret_device = RFM_INVALID_DEVICE;

  for(device = RFM_MAX_WAN_DEVICES-1; device != RFM_DEVICE_0; device--)
  {
    if(rfwcdma_data.radio_status[device].dev_type == RFM_WCDMA_TX_DEVICE)
    {
      break;
    }
    else
    {
      continue;
    }
  }

  if (rfwcdma_data.radio_status[device].dev_type == RFM_WCDMA_TX_DEVICE) 
  {
    // RF_MSG_1(RF_HIGH,"rfwdma_data_get_tx_device: device %d ",device);
     ret_device = device;
  }
  else
  {
     //RF_MSG_1(RF_HIGH,"rfwdma_data_get_tx_device: Invalid Device %d ",device);
     ret_device = RFM_INVALID_DEVICE ; 
  }

 return ret_device;  

}


/*!
  @brief
  Get Num carrriers for the specified device.
 
  @param device
  Void
 
  @return
  void
*/

rfcom_multi_carrier_hspa_id_type   
rfwcdma_data_get_num_carriers(rfcom_device_enum_type device)
{  
  if(device < RFM_WCDMA_MAX_DEVICE)
   {
    return(rfwcdma_data.radio_status[device].num_carriers);
   }

  else 
   {
    RF_MSG_1(RF_ERROR, "rfwcdma_data_get_num_carriers: Called on invalid ?%d device", device);
    return RFCOM_SINGLE_CARRIER;
   }
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Validate the device type. This api checks if a device is valid for a rx/Tx operation
 
  @param device
  RFM Device
 
  @param dev_type
  Indicates PRx, DRx or Tx device
 
  @return
  flag indicating TRUE or FALSE
*/

boolean rfwcdma_data_val_dev_type(rfm_device_enum_type device, rfm_wcdma_device_type dev_type)
{
  if (rfwcdma_data.radio_status[device].dev_type != dev_type)
  { 
    RF_MSG_3(RF_ERROR,"rfwcdma_data_val_dev_type:device%d exp type:%d curr_type %d",
          device,
          dev_type,
          rfwcdma_data.radio_status[device].dev_type);

    return FALSE;
   }
  else
  {
    return TRUE;
  }
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get associated Rx device 
 
  @param device
  RFM Device
 
  @return
  RFM device Rx associated  with the passed device
*/

rfcom_device_enum_type
rfwcdma_data_get_associated_device_rx
(
   rfcom_device_enum_type device
)
{
  if(device < RFM_MAX_WAN_DEVICES)
  {
    return rfwcdma_data.radio_status[device].associated_device_rx;
  }
  else
  {
    return RFM_INVALID_DEVICE;
  }
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get associated Tx device 
 
  @param device
  RFM Device
 
  @return
  RFM device Tx associated  with the passed device
*/

rfcom_device_enum_type
rfwcdma_data_get_associated_device_tx
(
   rfcom_device_enum_type device
)
{
  return rfwcdma_data.radio_status[device].associated_device_tx;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get associated PRx device from Tx
 
  @param device
  RFM Device
 
  @return
  RFM device PRx associated  with the passed Tx device
*/
rfcom_device_enum_type
rfwcdma_data_get_associated_device_prx_from_tx
(
   rfcom_device_enum_type device
)
{
  rfcom_device_enum_type temp_device = RFM_DEVICE_0;

  temp_device = rfwcdma_data.radio_status[device].associated_device_rx;

  /*Check if Tx device is entered*/
  if (temp_device == RFM_INVALID_DEVICE)
  {
    for (temp_device = RFM_DEVICE_0; temp_device < RFM_MAX_WAN_DEVICES ; temp_device++)
    {
      if ((rfwcdma_data.radio_status[temp_device].dev_type == RFM_WCDMA_PRIMARY_RX_DEVICE)&&
          (rfwcdma_data.radio_status[temp_device].rf_state != RFWCDMA_STATE_INIT)&&
          (rfwcdma_data.radio_status[temp_device].associated_device_tx == device))
      {
        /* Associated device found */
        break;
      }
    }
    if (temp_device == RFM_MAX_WAN_DEVICES)
    {
      RF_MSG_1( RF_ERROR, "rfwcdma_data_get_associated_device_prx_from_tx,"
                          "not able to find associated PRx device for Tx device %d",device );
    }
  }
  return temp_device;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Set associated Rx device 
 
  @param device
  RFM Device
 
  @return
  void
*/

void
rfwcdma_data_set_associated_device_rx
(
  rfm_device_enum_type device,
  rfm_device_enum_type pair_dev
)
{
  rfwcdma_data.radio_status[device].associated_device_rx = pair_dev;

  RF_MSG_2(RF_MED,"rfwcdma_data_set_associated_device_rx: For device %d, paired Rx device %d",
           device, 
           pair_dev);
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get the first primary Rx device from state machine.
  Used by legacy functions where band info is not available. It will loop through
  the state machine and returned the first found primary device.
  
  @param void
 
  @return
  RFM Device
*/

rfcom_device_enum_type
rfwcdma_data_get_first_prx_device
(
  void
)
{
  rfcom_device_enum_type temp_device = RFM_DEVICE_0;

  for (temp_device = RFM_DEVICE_0; temp_device < RFM_MAX_WAN_DEVICES ; temp_device++)
  {
    if ((rfwcdma_data.radio_status[temp_device].dev_type == RFM_WCDMA_PRIMARY_RX_DEVICE)&&
      (rfwcdma_data.radio_status[temp_device].rf_state != RFWCDMA_STATE_INIT)&& 
      (rfwcdma_data.radio_status[temp_device].curr_band!= RFCOM_BAND_INVALID))
    {
      /* Associated device found */
      break;
    }
  }
  if (temp_device == RFM_MAX_WAN_DEVICES)
  {
    RF_MSG( RF_ERROR, "rfwcdma_data_get_first_prx_device, not able to find associated PRx device ");
  }
  else
  {
    // RF_MSG_1( RF_LOW, "rfwcdma_data_get_first_prx_device: %d",temp_device);
  }

  return temp_device;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Set associated Tx device 
 
  @param device
  RFM Device
 
  @return
  void
*/

void
rfwcdma_data_set_associated_device_tx
(
  rfm_device_enum_type device,
  rfm_device_enum_type pair_tx_dev
)
{
  rfwcdma_data.radio_status[device].associated_device_tx = pair_tx_dev;

   RF_MSG_2(RF_MED,"rfwcdma_data_set_associated_device_tx: For device %d, paired Tx device %d",
           device, 
           pair_tx_dev);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Set num of carriers 
 
  @param device
  RFM Device
 
  @return
  void
*/
void   
rfwcdma_data_set_num_carriers
(
  rfcom_device_enum_type device,
  rfcom_multi_carrier_hspa_id_type rx_w_carriers
)
{
  rfwcdma_data.radio_status[device].num_carriers = rx_w_carriers; 
  RF_MSG_2(RF_MED,"rfwcdma_data_set_num_carriers: device %d, num of carriers %d",
           device, 
           rx_w_carriers);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get RFM device from antenna/ carrier index combination
 
  @param ant_idx
  uint8 type index for antenna id
 
  @param carr_idx
  rfcom_multi_carrier_hspa_id_type index for the Rf carrier
 
 @return device
  rfm_device_enum_type device
*/
rfm_device_enum_type rfwcdma_data_map_ant_carr_to_device(uint8 ant_idx,
                                                         rfcom_multi_carrier_hspa_id_type carr_idx,
                                                         boolean is_tx)
{
  rfcom_multi_carrier_hspa_id_type carr_id =0;
  rfm_wcdma_device_type dev_type = RFM_WCDMA_INVALID_DEVICE;
  rfm_device_enum_type dev_id = RFM_INVALID_DEVICE;
  uint16 carrier_mask;
  
  if (!is_tx) 
  {
    if (ant_idx == 0) 
    {
      dev_type = RFM_WCDMA_PRIMARY_RX_DEVICE;
    }
    else if (ant_idx ==1) 
    {
      dev_type = RFM_WCDMA_DIVERSITY_RX_DEVICE;
    }
  }
  else 
  {
    RF_MSG_3(RF_MED, "rfwcdma_data_map_ant_carr_to_device: Ant %d, Carr %d mapped to Tx dev%d",
                                    ant_idx,carr_idx, rfwcdma_data_get_tx_device());
    return rfwcdma_data_get_tx_device();
  }

  for (dev_id =RFM_DEVICE_0; dev_id < RFM_MAX_WAN_DEVICES; dev_id ++ ) 
  {
    if (rfwcdma_data_get_device_type(dev_id)== dev_type) 
    {
      carrier_mask =  rfwcdma_data_get_carrier_mask(dev_id);

      if(carrier_mask && 1<<carr_idx) 
      {
         return dev_id;
      }
    }
  }

  return RFM_INVALID_DEVICE;

}
/*!
  @brief
  Get handle
 
  @param device
  RFM Device
 
  @return
  uint8 for handle
*/

uint8 rfwcdma_data_get_handle(rfcom_device_enum_type device)
{
 return rfwcdma_data.radio_status[device].rflm_handle;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get the Tx device from the Sub
 
  @param sub_id
  Subscription ID on which backoff needs to be performed.
 
  @return
  Tx device associated with the specified sub
*/
rfm_device_enum_type rfwcdma_data_get_tx_dev_frm_sub
(
  rfm_wcdma_subscription_type sub_id
)
{
  rfcom_device_enum_type temp_device = RFM_MAX_DEVICES;	
  for (temp_device = RFM_DEVICE_0; temp_device < RFM_MAX_WAN_DEVICES ; temp_device++)
  {
    if((rfwcdma_data_get_subscription_type(temp_device) == sub_id)
		&& (rfwcdma_data_get_device_type(temp_device) == RFM_WCDMA_TX_DEVICE))
    {
      break; /*Device Found*/
    }
  }

  if (temp_device == RFM_MAX_WAN_DEVICES)
  {
    RF_MSG( RF_ERROR, "rfwcdma_data_get_tx_dev_frm_sub, not able to find the tx device ");
  }
  else
  {
    RF_MSG_1( RF_MED, "rfwcdma_data_get_tx_dev_frm_sub: %d",temp_device);
  }

  return temp_device;

}
	
#ifdef FEATURE_RF_ASDIV
/*!
  @brief
  Set AsDiv cb handle 
   
*/
void rfwcdma_data_init_asdiv_data(void)
{
  rfwcdma_data.asdiv_data.asdiv_cb_pos = ASDIV_POSITION_0;
  
  rfwcdma_data.asdiv_data.asdiv_cb_reg_status = FALSE;

  rfwcdma_data.asdiv_data.asdiv_cb_handle = 0;
}

/*!
  @brief
  Set AsDiv cb handle 
   
*/
void rfwcdma_data_set_asdiv_cb_handle(rfcommon_asdiv_cb_handler_type cb_handle)
{
  rfwcdma_data.asdiv_data.asdiv_cb_handle = cb_handle;
}

/*!
  @brief
  get AsDiv cb handle 
   
*/
rfcommon_asdiv_cb_handler_type rfwcdma_data_get_asdiv_cb_handle(void)
{
  return rfwcdma_data.asdiv_data.asdiv_cb_handle;
}
/*!
  @brief
  Set AsDiv cb status 
   
*/
void rfwcdma_data_set_asdiv_cb_status(boolean cb_status)
{
  rfwcdma_data.asdiv_data.asdiv_cb_reg_status = cb_status;
}
/*!
  @brief
  Get AsDiv cb status 
  TRUE indicates a cb has already been registered.  
*/
boolean rfwcdma_data_get_asdiv_cb_status(void)
{
  return rfwcdma_data.asdiv_data.asdiv_cb_reg_status;
}

/*!
  @brief
  Get AsDiv cb position pointer
   
*/
rfcommon_asdiv_position_type* rfwcdma_data_get_asdiv_cb_pos_ptr(void)
{
  return &(rfwcdma_data.asdiv_data.asdiv_cb_pos);
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Get the static data members rf wcdma data

  @details
  This function will get the rf wcdma data 

  @return
  Return the rfwcdma data 
*/

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
rfwcdma_data_type *
rf_wcdma_data_get_wcdma_data
(
  void  
)
{	
  return &rfwcdma_data;
}
#endif

#endif
/*! @} */
