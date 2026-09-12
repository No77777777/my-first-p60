#ifndef WTR3925_COMMON_DEVICE_INTF_H
#define WTR3925_COMMON_DEVICE_INTF_H
/*! 
  @file
  wtr3925_common_device_intf.h
 
  @brief
  Contains the interface for RFC to WTR3925 Common driver

  @details
  Contains function prototypes to create WTR3925 common devices and any other
  direct configuration from RFC.

  This header is wtr3925 specific and hence must be included by WTR3925 RF cards 
  only.

  This file will have to be included by RFC to communicate with WTR3925 and must 
  be shipped.

  @addtogroup WTR3925_COMMON
  @{
*/

/*==============================================================================

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

===============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_wtr3925.mpss/2.11/api/wtr3925_common_device_intf.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
05/30/14   zg      Added WTR3925_DEVICE_MAX_EFS_BASE_PATH_SIZE.
04/03/14   bm      Updated the copyright information
11/20/13   rp      Changes to support class based implementation for Common 
                   device driver.
09/24/13   rp      Cpp support.
08/12/13   tks     Added support for wxe & trx rffe initialization 
06/10/13   tks     Added support for rffe interface control 
05/01/13   shb     Pass efs_file_path string to legacy device factory - used by 
                   WTR3925/WFR1620
03/07/13  bmg/shb  Added "destructor"
02/24/12   shb     Added wtr3925_common_do_internal_device_cal() to do all 
                   internal cal (RC tuner + HDET autocal) for WTR3925
02/14/12   shb     Added argument to send internal device cal data during device
                   creation
10/06/11   shb     Increased # of supported common devices to 2 for SV cards
07/14/11   shb     Initial Revision

==============================================================================*/

#include "rfdevice_cmn_intf.h"
#include "rfc_common.h"
#include "rfdevice_rxtx_common_class.h"


#ifdef __cplusplus
extern "C" {
#endif


/*! Max number of Common devices supported by WTR3925 */
#define WTR3925_MAX_DEVICES 2

/*! Max size of EFS file paths string for the WTR device. It will be used to 
  initialize char arrays for strings containingg efs paths.
*/
#define WTR3925_DEVICE_MAX_EFS_BASE_PATH_SIZE  100

/*----------------------------------------------------------------------------*/
rfdevice_rxtx_common_class*
wtr3925_common_create_device
(
  rfc_device_cfg_info_type wtr3925_rffe_info,
  rfdevice_cmn_int_dev_cal_data_type cal_data,
  const char* efs_file_path
);

boolean wtr3925_common_create_wxe_device
(
  rfdevice_rxtx_common_class* instance,
  rfc_device_cfg_info_type* cfg
);

#ifdef __cplusplus
}
#endif


/*! @} */

#endif /* WTR3925_COMMON_DEVICE_INTF_H */
