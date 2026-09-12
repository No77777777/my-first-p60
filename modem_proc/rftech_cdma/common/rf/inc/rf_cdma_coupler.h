#ifndef RF_CDMA_COUPLER__H
#define RF_CDMA_COUPLER__H
/*!
  @file rf_cdma_coupler.h

  @brief
  <b>This is the COUPLER interface exposed to the MC layer.</b>
  This module contains all CDMA-COUPLER-specific data and
  implementations.

  @addtogroup RF_RFC_CDMA_MC_INTF
  @{

*/

/*==============================================================================

  Copyright (c) 2008 - 2015 by Qualcomm Technologies, Incorporated.  All
  Rights Reserved.

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

$Header: //components/rel/rftech_cdma.mpss/3.11/common/rf/inc/rf_cdma_coupler.h#1 $

==============================================================================*/

#include "comdef.h"
#include "rfm_cdma_band_types.h"
#include "rfc_common.h"
#include "rfc_class.h"
#include "rfdevice_coupler_intf.h"

#ifdef __cplusplus

/*!
  @brief
  RF CDMA COUPLER wrapper device class

  @details
  This class acts as an aggregator class for RF ASM device objects. It is
  provided to configure more than one ASM device for a specific band/path
  configuration.
*/

typedef enum
{
  COUPLER_1 ,
  COUPLER_2 ,
  COUPLER_3 ,
  MAX_CDMA_COUPLER ,
  MAX_CDMA_UNKNOWN_COUPLER = MAX_CDMA_COUPLER
} rf_cdma_coupler_type ;

class rf_cdma_coupler
{
protected:
  uint8    rf_cdma_logical_dev ;
  void*    rfc_device_descriptor[MAX_CDMA_COUPLER] ;
  uint8    num_coupler_instances ;

public:
  /* Constructor - Initialize the aggregator object */
  rf_cdma_coupler( uint8 device ) ;
  /* Add a new RF ASM device to the aggregator */
  bool add_device_instance( void* rfc_device_descriptor ) ;

  uint8 get_num_couplers () { return num_coupler_instances ; }
  void* get_cpler_obj(uint8 idx) { return rfc_device_descriptor[idx] ;}

} ;

#else
/* For visibilty from C code */
typedef struct rf_cdma_coupler rf_cdma_coupler ;
boolean
rf_cdma_config_coupler
(
 rf_cdma_coupler* coupler_obj ,
 rfdevice_coupler_tech_cfg_type tech_cfg_type,  
 void* buff_obj_ptr , 
 rf_device_execution_type dev_action , 
 int16 script_timing
) ;


#endif /* #ifdef __cplusplus */

/*! @} */
#endif /* #ifndef RF_CDMA_COUPLER__H */

/*=============================================================================
when       who     what, where, why
--------   ---     ------------------------------------------------------------
07/14/12   APU     Initial version
xx/xx/xx   abc     Add version history here. 
==============================================================================*/
