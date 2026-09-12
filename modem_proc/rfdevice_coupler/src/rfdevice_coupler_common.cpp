/*!
  @file
  rfdevice_coupler_common.cpp

  @brief
  Common interface for 3rd party Coupler devices

*/

/*==============================================================================

  Copyright (c) 2014-2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/rfdevice_coupler.mpss/1.22/src/rfdevice_coupler_common.cpp#1 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
07/14/15   px      Add 3rd party rffe coupler
03/23/15   vv      CW fix
12/26/14   ndb     Initial version

==============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif
#include "fs_lib.h"
#include "fs_public.h"
#include "modem_mem.h"

#ifdef __cplusplus
}/* extern "C"*/
#endif

#include "rf_buffer_intf.h"
#include "rfcommon_msg.h"
#include "rfcommon_time_profile.h"
#include "rfdevice_msg_log.h"
#include "rfdevice_coupler_common.h"
#include "rfdevice_grfc_coupler.h"
#include "rfdevice_rffe_coupler.h"
#include "rfdevice_rffe_coupler_config_main_ag.h"

/*!
  @brief
  create 3rd party Coupler devices

  @details
  create 3rd party Coupler devices and register itself to the parent physical device object passed to it.

  @param rfdevice_physical_ptr : parent physical device object of the coupler device
  @param logical_device_cfg : logical device info of the coupler device
*/

rfdevice_logical_component* create_gen_coupler_object
(
  rfdevice_physical_device *rfdevice_physical_ptr,
  rfc_logical_device_info_type *logical_device_cfg
)
{
  boolean init_status = FALSE;
  rfdevice_logical_component *temp_obj=NULL;
  rfc_phy_device_info_type *phy_device_cfg=NULL;
  rfdevice_rffe_coupler_settings_type coupler_settings;
  rfdevice_rffe_coupler_func_tbl_type coupler_fn_ptrs;

  /*NULL pointer check*/
  if ( (rfdevice_physical_ptr == NULL)           ||
       (logical_device_cfg == NULL)              ||
       ( ((rfdevice_physical_third_party*)rfdevice_physical_ptr)->third_party_phy_dev_cfg == NULL) )
  {
    RF_MSG( RF_ERROR, "create_gen_coupler_object failed ! NULL Pointer detected");
    return NULL;
  }

  phy_device_cfg=((rfdevice_physical_third_party*)rfdevice_physical_ptr)->third_party_phy_dev_cfg;

 /*Create based on the protocol type*/
  if(phy_device_cfg->rf_device_comm_protocol == RFDEVICE_COMM_PROTO_GRFC)
  { 
    /*GRFC coupler*/
    temp_obj = (rfdevice_class*)new rfdevice_grfc_coupler(rfdevice_physical_ptr,
                                         phy_device_cfg,
                                         logical_device_cfg);
    if (temp_obj != NULL)
    {
      if(((rfdevice_grfc_coupler*)temp_obj)->init_status == TRUE)
      { 
        /*store the grfc coupler logical device object in its parent physical device object*/
        ((rfdevice_physical_third_party*)rfdevice_physical_ptr)->store_object(temp_obj);
      }
      else
      {
        delete temp_obj; /* destroy the GRFC coupler device object, as it failed to initialize the device correctly. */
        temp_obj = NULL;
      }
    }
  }
  else if(phy_device_cfg->rf_device_comm_protocol == RFDEVICE_COMM_PROTO_RFFE)
  {
    /*choose the autogen settings based on the specific chip on the rf-card*/
    if (rfdevice_rffe_coupler_factory( phy_device_cfg,   
                                       &coupler_settings,
                                       &coupler_fn_ptrs
                                       ))
    {
      /*RFFE coupler*/
      temp_obj = (rfdevice_class*)(new rfdevice_rffe_coupler(rfdevice_physical_ptr ,
                                                    phy_device_cfg,
                                                    logical_device_cfg,                                       
                                                    &coupler_settings,
                                                    &coupler_fn_ptrs)); 
      if (temp_obj != NULL)
      {
        if(((rfdevice_rffe_coupler*)temp_obj)->init_status == TRUE)
        {
          /*store the rffe coupler logical device object in its parent physical device object*/
          ((rfdevice_physical_third_party*)rfdevice_physical_ptr)->store_object(temp_obj);
        }
        else
        {
          delete temp_obj; /* destroy the RFFE coupler device object, as it failed to initialize the device correctly. */
          temp_obj = NULL;
        }
      }
    }
  }
  else
  {
    RF_MSG( RF_ERROR, "create_gen_coupler_object() failed ! Invalid commnunication protocal passed. Check RFC ");
  }

  return temp_obj;
}
