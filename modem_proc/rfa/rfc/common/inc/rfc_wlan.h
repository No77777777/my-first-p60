#ifndef RFC_WLAN_H
#define RFC_WLAN_H
/*!
   @file
   rfc_wlan.h

   @brief
   This file contains all the wlan mode specific RFC class definition

*/

/*===========================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:11:02 $ $Author: pwbldsvc $
$Header: //components/rel/rfa.mpss/4.11/rfc/common/inc/rfc_wlan.h#1 $

when       who     what, where, why
-------------------------------------------------------------------------------
 
05/21/15   fhuo      Initial Version 
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif
#include "rfa.h"
#include "rfc_card.h"
#include "rfc_class.h"
#include "rfcommon_msg.h"
//#include "rfcommon_asdiv_manager.h"

/*----------------------------------------------------------------------------*/
/*!
  @name WLAN interface for concrete RF card classes.

  @brief Concrete RF card container classes must derive from this class in order
         to support WLAN operation.
          
*/
/*! @{ */

/*!
  @brief
  Interface definition for RF cards supporting WLAN operation.
*/
class rfc_wlan : public rfa
{
public:
  static rfc_wlan * get_instance(void);
  static rfc_wlan * create_instance(void);

  /*! destructor */
  virtual ~rfc_wlan();

protected:
  /*! hide constructor to disallow instantiation except when 
    implementing this interface through derivation */
  rfc_wlan(void);

  static rfc_wlan *rfc_wlan_ptr;
 


private:
  void init_rf_devices(void);
  rfdevice_class *wlan_trx_devices[RFDEVICE_TYPE_MAX_NUM][RFC_MAX_DEVICE_INSTANCES_PER_BAND];

};

#ifdef __cplusplus
}
#endif

#endif /* RFC_WLAN_H */
