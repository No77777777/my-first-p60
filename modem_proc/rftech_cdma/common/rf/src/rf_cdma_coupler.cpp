/*!
  @file rf_cdma_coupler.cpp

  @brief
  <b>This is the COUPLER interface exposed to the MC layer.</b>
  This module contains all CDMA-COUPLER-specific data and
  implementations.

  @addtogroup RF_RFC_CDMA_MC_INTF
  @{

*/

/*==============================================================================

  Copyright (c) 2008 - 2015¢À by Qualcomm Technologies, Incorporated.  All Rights Reserved.

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
  written permission of Qualcomm Technologies, Inc..d.

==============================================================================*/

/*=============================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_cdma.mpss/3.11/common/rf/src/rf_cdma_coupler.cpp#1 $
==============================================================================*/                         

#include "rf_cdma_coupler.h"
#include "rfc_cdma_data.h"
#include "rfcommon_msg.h"

/*----------------------------------------------------------------------------*/
/* C interface functionality */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

boolean
rf_cdma_config_coupler
(
 rf_cdma_coupler* coupler_obj ,
 rfdevice_coupler_tech_cfg_type tech_cfg_type,  
 void* buff_obj_ptr , 
 rf_device_execution_type dev_action , 
 int16 script_timing
)
{
  boolean result = TRUE ;
  for ( int c = 0 ; c < coupler_obj->get_num_couplers() ; c++ )
  {
    result &= rfdevice_coupler_config ( coupler_obj->get_cpler_obj(c) ,
                  tech_cfg_type , buff_obj_ptr, dev_action , script_timing ) ;
  }
  return result ;
}

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------*/
/* CDMA COUPLER class implementation */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Constructor - Initializes the parameters for the Coupler
  wrapper object

  @param rfm_device
  Device for which the ASM wrapper object is created
 
  @return NA
*/

rf_cdma_coupler::rf_cdma_coupler 
( 
  uint8 device
)
{
  this->rf_cdma_logical_dev = device ;
  this->num_coupler_instances = 0 ;

  for ( uint8 i = 0; i < MAX_CDMA_COUPLER; i++ )
  {
    this->rfc_device_descriptor[i] = NULL;
  }
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Add RF Coupler device to this wrapper object

  @details
  This wrapper object stores pointers, upto a maximum allowed instance value,
  to RF Coupler device objects. Duplicate objects are not
  included in the wrapper.

  @param new_device
  Pointer to coupler device which must be added to the wrapper
  object

  @retval
  TRUE: If coupler device was added successfully
  FALSE: If adding a new device failed
*/
bool
rf_cdma_coupler::add_device_instance ( void* new_device )
{
  bool ret_val = TRUE;
  uint8 num_couplers = this->num_coupler_instances ;
  uint8 i;
  bool device_already_exists = FALSE;

  if ( new_device != NULL )
  {
    /* Search if this wrapper object already contains the device
    to be added */
    for ( i = 0; i < num_couplers ; i++ )
    {
      if ( this->rfc_device_descriptor[i] == new_device )
      {
        /* Found a match */
        device_already_exists = TRUE;
        break;
      }
    } /* Cycle through existing couplers */ 

    /* If a device is to be added, update the wrapper object */
    if ( num_couplers < MAX_CDMA_COUPLER &&
         device_already_exists == FALSE )
    {
      this->rfc_device_descriptor[num_couplers] = new_device ;
      this->num_coupler_instances ++ ;
      ret_val = TRUE;
    }
    else if ( num_couplers >= MAX_CDMA_COUPLER )
    {
      RF_MSG_1( RF_FATAL , "rf_cdma_coupler::add_device_instance: Cannot "
                "add more than allowed Coupler instances. "
                "Max allowed %d",
                MAX_CDMA_COUPLER ) ;
      ret_val = FALSE;
    }
  }
  else
  {
    RF_MSG( RF_ERROR, "rf_cdma_coupler::add_device_instance: Requesting "
            "to add NULL device" );
    ret_val = FALSE;
  }

  return ret_val;
}

/*! @} */


/*=============================================================================
when       who     what, where, why
--------   ---     ------------------------------------------------------------
07/14/12   APU     Initial version
xx/xx/xx   abc     Add version history here. 
==============================================================================*/

