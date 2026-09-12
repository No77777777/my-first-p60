
#ifndef WTR3925_GNSS_DEVICE_INTF_H
#define WTR3925_GNSS_DEVICE_INTF_H
/*! 
  @file
  wtr3925_gnss_device_intf.h
 
  @brief
  Contains the interface for RFC to WTR3925 GNSS driver

  @details
  Contains function prototypes to create WTR3925 GNSS devices

  This header is wtr3925 specific and hence must be included by WTR3925 RF cards 
  only.

  This file will have to be included by RFC to communicate with WTR3925 and must 
  be shipped.

  @addtogroup WTR3925_GNSS
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

  $Header: //components/rel/rfdevice_wtr3925.mpss/2.11/api/wtr3925_gnss_device_intf.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
04/03/14   bm      Updated the copyright information
01/07/13   shb/dbz Added support for GNSS to operate on WTR3925 common instance 1
07/19/11   dbz     Initial Revision

==============================================================================*/

#include "rfdevice_cmn_intf.h"

/*----------------------------------------------------------------------------*/
/*!
   @brief
   Initialize GNSS device pointers.

   @details

   @param path: rf device path.

   @retval None.
*/
void rfdevice_wtr3925_gnss_config(rfgnss_path_enum_type rfgnss_path);

/*----------------------------------------------------------------------------*/
extern boolean
rfdevice_wtr3925_gnss_set_instance_id
(
  uint8 instance_id
);

/*! @} */

#endif /* WTR3925_GNSS_DEVICE_INTF_H */
