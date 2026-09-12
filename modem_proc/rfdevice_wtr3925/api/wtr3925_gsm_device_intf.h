#ifndef WTR3925_GSM_DEVICE_INTF_H
#define WTR3925_GSM_DEVICE_INTF_H
/*! 
  @file
  wtr3925_gsm_device_intf.h
 
  @brief
  Contains the interface for RFC to WTR3925 CDMA driver

  @details
  Contains function prototypes to create WTR3925 CDMA devices and any other
  direct configuration from RFC like setting band/ports, selecting TX LUTs etc

  This header is wtr3925 specific and hence must be included by WTR3925 RF cards 
  only.

  This file will have to be included by RFC to communicate with WTR3925 and must 
  be shipped.

  @addtogroup WTR3925_CDMA
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

  $Header: //components/rel/rfdevice_wtr3925.mpss/2.11/api/wtr3925_gsm_device_intf.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
04/03/14   bm      Updated the copyright information
12/19/13   rp      Removed additional header files.
11/15/13  rp/dps   Class based implementation for GSM device driver.
07/11/12   sr      Added extern "C" conditional compile.
09/27/11   vrb     Push config info from RFC instead of querying from WTR lib
07/17/11   vrb     Added API to create GSM device

==============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include "rfdevice_rxtx_common_class.h"

/*! Max number of devices supported by WTR3925 */
#define WTR3925_MAX_GSM_DEVICES 1

/*----------------------------------------------------------------------------*/
boolean
wtr3925_gsm_create_device
(
  rfdevice_rxtx_common_class* common_device
);

#ifdef __cplusplus
}
#endif

/*! @} */

#endif /* WTR3925_CDMA_DEVICE_INTF_H */
