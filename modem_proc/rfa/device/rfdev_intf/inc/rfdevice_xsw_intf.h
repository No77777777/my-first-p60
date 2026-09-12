#ifndef RFDEVICE_XSW_INTF_H
#define RFDEVICE_XSW_INTF_H
/*!
   @file
   rfdevice_xsw_intf.h

   @brief
   
*/

/*===========================================================================

Copyright (c) 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/rfa.mpss/4.11/device/rfdev_intf/inc/rfdevice_xsw_intf.h#1 $

when       who   what, where, why
--------   ---   --------------------------------------------------------------- 
03/01/18   vp    Initial version
==============================================================================*/

/*==============================================================================
                           INCLUDE FILES
==============================================================================*/
#include "rfcom.h"
#include "rfdevice_cmn_intf.h"
#include "rfdevice_xsw.h"
#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
                         Prototypes
==============================================================================*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  interface function programming the cross switch PRX and DRX paths to the desired state

  @param xsconfig
  structure containing configuration data
 
  @param device_ptr
  pointer to xsw device object 
 
  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts  

  @param script_timing
  timing offset for CCS scripts   
  
  @return
  boolean indicating pass or fail  
*/	
boolean rfdevice_xsw_set_cross_switch_config
(
  rfdevice_xsw_config_type* xsconfig,
  void *device_ptr,
  rf_buffer_intf* buff_obj_ptr, 
  rf_device_execution_type dev_action, 
  int16 script_timing       
);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  interface function for populating the XSW band/port map for the default cross
  switch position

  @param xsconfig
  structure containing configuration data
 
  @param device_ptr
  pointer to xsw device object 

  @param data_ptr
  pointer to data containing the port information  

  @param size
  size of dat array pointed to by data_ptr   
  
  @return
  boolean indicating pass or fail  
*/	

boolean rfdevice_xsw_set_band_map_default
(
  rfdevice_xsw_config_type* xsconfig,
  void *device_ptr,
  int32 *data_ptr, 
  uint8 size 
);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  interface function for populating the XSW band/port map for the swap cross
  switch position

  @param xsconfig
  structure containing configuration data
 
  @param device_ptr
  pointer to xsw device object 

  @param data_ptr
  pointer to data containing the port information  

  @param size
  size of dat array pointed to by data_ptr   
  
  @return
  boolean indicating pass or fail  
*/	
boolean rfdevice_xsw_set_band_map_swap
(
  rfdevice_xsw_config_type* xsconfig,
  void *device_ptr,
  int32 *data_ptr, 
  uint8 size 
);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  interface function for populating the XSW band/port map for the SRS default cross
  switch position

  @param xsconfig
  structure containing configuration data
 
  @param device_ptr
  pointer to xsw device object 

  @param data_ptr
  pointer to data containing the port information  

  @param size
  size of dat array pointed to by data_ptr   
  
  @return
  boolean indicating pass or fail  
*/	

boolean rfdevice_xsw_set_srs_band_map_default
(
  rfdevice_xsw_config_type* xsconfig,
  void *device_ptr,
  int32 *data_ptr, 
  uint8 size 
);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  interface function for populating the XSW band/port map for the SRS swap cross
  switch position

  @param xsconfig
  structure containing configuration data
 
  @param device_ptr
  pointer to xsw device object 

  @param data_ptr
  pointer to data containing the port information  

  @param size
  size of dat array pointed to by data_ptr   
  
  @return
  boolean indicating pass or fail  
*/	
boolean rfdevice_xsw_set_srs_band_map_swap
(
  rfdevice_xsw_config_type* xsconfig,
  void *device_ptr,
  int32 *data_ptr, 
  uint8 size 
);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  interface function programming the cross switch in the desired state

  @param xsconfig
  structure containing configuration data
    
  @return
  boolean indicating pass or fail  
*/	
boolean rfdevice_xsw_get_cross_switch_config
(
  rfdevice_xsw_config_type* xsconfig,
  void *device_ptr
);


/*----------------------------------------------------------------------------*/
/*!
  @brief
  interface function for cross switch init

  @details
  API used to config the cross switch for use

  @param rfc_data
  ASIC data capturing port info and default functionality provided by RFC

  @return
  boolean indicating pass or fail   
*/
boolean rfdevice_xsw_init 
(
  void *device_ptr,
  int32* rfc_data
);

/*!
  @brief
  Interface function for cross switch trigger, to latch the pending settings.

  @details
  trigger script
  
  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts  
    
  @param script_timing
  time offset for the script where a negative value refers to the time BEFORE the event

  @return
  boolean indicating pass or fail  
*/
boolean rfdevice_xsw_trigger
(
  rfdevice_xsw_config_type* xsconfig,
  void *device_ptr,
  rf_buffer_intf* buff_obj_ptr, 
  rf_device_execution_type  dev_action, 
  int16 script_timing
);

/*!
  @brief
  Interface function for cross switch sleep.

  @details
  trigger script
  
  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts  
    
  @param script_timing
  time offset for the script where a negative value refers to the time BEFORE the event

  @return
  boolean indicating pass or fail  
*/
boolean rfdevice_xsw_sleep
(
  rfdevice_xsw_config_type* xsconfig,
  void *device_ptr,
  rf_buffer_intf* buff_obj_ptr, 
  rf_device_execution_type  dev_action, 
  int16 script_timing
);


/*!
  @brief
  Interface function for cross switch wakeup.

  @details
  trigger script
  
  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts  
    
  @param script_timing
  time offset for the script where a negative value refers to the time BEFORE the event

  @return
  boolean indicating pass or fail  
*/
boolean rfdevice_xsw_wakeup
(
  rfdevice_xsw_config_type* xsconfig,
  void *device_ptr,
  rf_buffer_intf* buff_obj_ptr, 
  rf_device_execution_type  dev_action, 
  int16 script_timing
);





#ifdef __cplusplus
}
#endif

#endif /* RFDEVICE_COUPLER_INTF_H */


