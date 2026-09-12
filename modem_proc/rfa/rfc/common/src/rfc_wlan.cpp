/*!
   @file
   rfc_wlan.cpp

   @brief
   This file contains all the WLAN mode specific implementations to support RFC.
   These are platform independent.

*/

/*===========================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfa.mpss/4.11/rfc/common/src/rfc_wlan.cpp#1 $

when       who     what, where, why
--------------------------------------------------------------------------------
05/22/15   fhuo      Initial Version
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "rfc_card.h"
#include "rfc_wlan.h"
#include "rfc_common.h"
#include "rfc_wlan_data.h"
#include "rfc_class.h"
#include "rfdevice_asm.h" 
//#include "rfc_vreg_mgr.h"
#include "rfcommon_msg.h"
#include "rfcommon_atuner_manager_factory.h"

extern "C"
{
  #include "rfcommon_core.h"

}

/* To Enable/Disable QTF Tuner support */
//#ifdef FEATURE_RF_HAS_QTUNER
//extern "C"
//{
// boolean enable_qtf_tuner_support = FALSE;
//}
//#endif
/* ----------------------------------------------------------------------- */
/*!
  @brief
  RF Card WLAN CORE configuration table.

  @details
  Contains the static values for the CORE parameters for WLAN mode of operation.
  The structure and type is specific to this RF card.
  {warmup_time, tune_time, pwr_on_wakeup, cgagc_settling_time}
  This structure shouldn't be card specific. only the data.. needs change
*/

rfc_wlan *rfc_wlan::rfc_wlan_ptr = (rfc_wlan *)NULL;

/* Pointer to device class to hold common object */


rfc_wlan *rfc_wlan::get_instance(void)
{
  return rfc_wlan_ptr;
}

// create singleton object
rfc_wlan * rfc_wlan::create_instance(void)
{
  if (rfc_wlan_ptr == NULL )
  {
    rfc_wlan_ptr = (rfc_wlan *)new rfc_wlan();
  }
  return rfc_wlan_ptr;
}

/*!
  @brief Indicates whether device initialization has been completed
  for the rf card.

  @details

  @return Device init complete flag 

*/


// Constructor 
rfc_wlan::rfc_wlan()
{
  uint8 dev_instance = 0;
  uint8 dev_type = 0;
  /* Initialize the device init flag */
  for(dev_type = 0; dev_type < RFDEVICE_TYPE_MAX_NUM; dev_type++)
  {
    for (dev_instance = 0; dev_instance < RFC_MAX_DEVICE_INSTANCES_PER_BAND; dev_instance++)
    {
      wlan_trx_devices[dev_type][dev_instance] = NULL;
    }
  }  
  /*  Push band port information for device, pa & asm */
  init_rf_devices();


}

// Destructor
rfc_wlan::~rfc_wlan()
{
  rfc_wlan_ptr = NULL;
}



/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function initializes the rfdevices with the rfc dependent data.

  @details
  This function initializes the rfdevices with the rfc dependent data.

*/

void rfc_wlan::init_rf_devices(void){

  rfc_cfg_params_type cfg;
  rfc_device_info_type *device_info=NULL;
  rfdevice_class *cmn_dev_obj = NULL;
  uint8 i = 0;
  rfdevice_type_enum_type dev_type = RFDEVICE_TYPE_INVALID;
  uint32 instance =0;

  uint8 asm_dev_instance = 0;
  uint8 tuner_dev_instance = 0;

  rfc_intf *rfc_cmn   = rfc_intf::get_instance();
  if (NULL == rfc_cmn)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RFC CMN Object is NOT Created", 0);
    return;
  }
  
  rfc_wlan_data *rfc_data = rfc_wlan_data::get_instance();
  if (rfc_data == NULL) {
    RF_MSG( RF_ERROR,"rfc_data == NULL!" );
    return;
  }

  cfg.alternate_path = 0;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;
  cfg.logical_device = RFM_INVALID_DEVICE;
  rfc_data->devices_cfg_data_get(&cfg, &device_info);
  if (device_info == NULL)
  {
    RF_MSG( RF_ERROR, "rfc_wlan::config_mode_data. Null device data !");
    return;
  }
  rfcom_band_type_u band_tmp;
  /* Attempt to map all devices for this configuration. */
  while (i < device_info->num_devices)
  {
    dev_type = device_info->rf_asic_info[i].device_type;
    instance = device_info->rf_asic_info[i].instance;
	cmn_dev_obj = rfc_cmn->get_cmn_rf_device_object(dev_type, (uint8)instance);       
	if (cmn_dev_obj == NULL){
      RF_MSG( RF_ERROR, "get_cmn_rf_device_object() returns a NULL ptr");
      return;
    }
        
    switch(dev_type)
    {
    
      case RFDEVICE_ASM:
        wlan_trx_devices[dev_type][asm_dev_instance] = cmn_dev_obj;
        asm_dev_instance++;
                   
         //Pushing RX band port
        ((rfdevice_asm *)cmn_dev_obj)->set_rx_band_map(cfg.logical_device, RFM_WLAN_MODE, band_tmp, 
                                                              device_info->rf_asic_info[i].data, 
                                                              RFC_ASIC_INFO_DATA_SIZE); 
        break; /* end case RFDEVICE_ASM */
      default:
        break;     
    } // end switch(dev_type) 
    i++;
  } // end while for going through all devices listed 
}

