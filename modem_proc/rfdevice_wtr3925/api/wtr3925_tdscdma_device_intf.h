#ifndef WTR3925_TDSCDMA_DEVICE_INTF_H
#define WTR3925_TDSCDMA_DEVICE_INTF_H
/*! 
  @file
  wtr3925_tdscdma_device_intf.h
 
  @brief
  Contains the interface for RFC to WTR3925 TDSCDMA driver

  @details
  Contains function prototypes to create WTR3925 TDSCDMA devices and any other
  direct configuration from RFC like setting band/ports, selecting TX LUTs etc

  This header is wtr3925 specific and hence must be included by WTR3925 RF cards 
  only.  This file will have to be included by RFC to communicate with WTR3925
  and must be shipped.

  @addtogroup WTR3925_TDSCDMA
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

  $Header: 

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
04/03/14   bm      Updated the copyright information
11/20/13   rp      Changes to support class based implementation for Common 
                   device driver.
11/08/13   rp      Update create_device API to accept rfdevice_rxtx_common_class 
                   ptr instead of wtr3925_rxtx_common_class ptr
11/08/13   rp      Class based implementation for TDSCDMA device driver.
04/26/13   tks     Added support for rx2 and rx3 receive chains
08/02/11   jyu     Initial Revision

==============================================================================*/

#include "rfdevice_rxtx_common_class.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "rfdevice_tdscdma_intf.h"
#include "rfdevice_cmn_intf.h"
#include "rfcom.h" 

/*! Max number of devices supported by WTR3925 */
#define WTR3925_MAX_TDSCDMA_DEVICES 1

/*----------------------------------------------------------------------------*/
boolean
wtr3925_tdscdma_create_device
(
  rfdevice_rxtx_common_class* common
);

#ifdef __cplusplus
}
#endif

/*! @} */

#endif /* WTR3925_TDSCDMA_DEVICE_INTF_H */



