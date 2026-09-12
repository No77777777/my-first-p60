#ifndef WTR3925_PHYSICAL_DEVICE_H
#define WTR3925_PHYSICAL_DEVICE_H
/*!
  @file
  wtr3925_physical_device.h 

  @brief
  Software abstraction of a WTR3925 physical device.
*/

/*===========================================================================

  Copyright (c) 2013-2014 Qualcomm Technologies, Inc. All Rights Reserved

  Qualcomm Technologies Proprietary and Confidential.

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_wtr3925.mpss/2.11/api/wtr3925_physical_device.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
06/09/14   vv      Update physical/logical device creation with updated physical device interface
05/30/14   zg      Updated the size definition of efs_file_path.
05/19/14   zg      Updated the definition of efs_file_path.
04/28/14   dps     Resolved Klocwork errors
04/03/14   bm      Updated the copyright information
04/02/14   dps     Cleanup Klocwork warnings by preventing copying this object
01/03/14   hm      Fixed compiler warnings
12/04/13   rp      Initial version

===========================================================================*/

#include "rfdevice_physical_device.h"
#include "wtr3925_common_device_intf.h"

class wtr3925_physical_device : public rfdevice_physical_device
{
public:
  wtr3925_physical_device(rfc_phy_device_info_type *wtr3925_cfg_info);

  ~wtr3925_physical_device();

  virtual bool load_self_cal(const char* str);
  
  virtual bool perform_self_cal(const char* str);
  
  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);


private:

  virtual void create_wtr3925_device(rfc_device_cfg_info_type *device_cfg);

  /* Char arrays for strings containing efs base path. The length is defined as the max size of
     EFS file path excluding the file name part */
  char efs_file_path[WTR3925_DEVICE_MAX_EFS_BASE_PATH_SIZE];  

  rfdevice_rxtx_common_class *transceiver_ptr;
  rfc_phy_device_info_type* phy_device_cfg;
  rfdevice_cmn_int_dev_cal_data_type *cal_data;
  
  /*Maintaining old device config sturctue*/
  rfc_device_cfg_info_type device_cfg;
  
  /* Private copy constructor and assignment operator prevent copying
     wtr3925_physical_device objects */
  wtr3925_physical_device& operator=(const wtr3925_physical_device& dev)
  {
    return *this;
  }

  wtr3925_physical_device(const wtr3925_physical_device& src){}
};

#endif /* WTR3925_PHYSICAL_DEVICE_H */

