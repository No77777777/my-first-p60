#ifndef RFC_WLAN_DATA_H
#define RFC_WLAN_DATA_H
/*!
  @file
  rfc_wlan_data.h

  @brief
  This file contains the class definition for the rfc_wlan_data, which provides the rfc related data
  retrival functionality to WLAN.

*/

/*===========================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfa.mpss/4.11/rfc/common/inc/rfc_wlan_data.h#1 $

when       who     what, where, why
-------------------------------------------------------------------------------
05/21/15   fhuo      init version
============================================================================*/

#include "rfa.h"
#include "rfc_common.h"


/* Definition of rfc_wlan_data class */
class rfc_wlan_data : public rfa
{
public:

  static rfc_wlan_data * get_instance();
  virtual boolean sig_cfg_data_get( rfc_cfg_params_type *cfg, rfc_sig_cfg_type **ptr );
  virtual boolean devices_cfg_data_get( rfc_cfg_params_type *cfg, rfc_device_info_type **ptr );
  // Destructor
  virtual ~rfc_wlan_data();
  
protected:

  // constructor
  rfc_wlan_data();

  static rfc_wlan_data *rfc_wlan_data_ptr;

private:

};

#endif /* RFC_WLAN_DATA_H */

