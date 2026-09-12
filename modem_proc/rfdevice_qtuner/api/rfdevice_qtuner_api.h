#ifndef RFDEVICE_QTUNER_API_H
#define RFDEVICE_QTUNER_API_H

/*=============================================================================

Copyright (c) 2013,2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. 
  Government. Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

  $Header: //components/rel/rfdevice_qtuner.mpss/1.32/api/rfdevice_qtuner_api.h#1 $
  $DateTime: 2021/03/09 04:14:45 $
  $Author: pwbldsvc $
 
when         who   what, where, why
--------   ---   --------------------------------------------------------------- 
02/17/16   ndb    Clean-up the un-used interfaces
08/21/14   vv     Clean up unused device creation APIs()
07/01/14   yb     Add support to make tuner transactions non-extended via rfc
05/27/14   vv     Physical device support
04/09/14  ndb     Added extended_cmd in create_common_qtuner()
03/25/14   yb     Added support for linearizer
03/11/14   yb     Added enum for intermodulation distortion
12/13/13   ndb    Initial version
 
=============================================================================*/
  
/*=============================================================================

                            INCLUDE FILES FOR MODULE
                            

=============================================================================*/
#include "rfc_common.h"
#include "rfdevice_antenna_tuner.h"
#ifdef __cplusplus
#include "rfdevice_physical_device.h"
#endif
/*! @brief Intermodulation Distortion type for Linearizer settings

*/
typedef enum
{
  RFDEVICE_QTUNER_IM_DISTORTION_DEFAULT = 1,
  RFDEVICE_QTUNER_IM_DISTORTION_IP3,
  RFDEVICE_QTUNER_IM_DISTORTION_HD3,
  RFDEVICE_QTUNER_IM_DISTORTION_IHD3,
  RFDEVICE_QTUNER_IM_DISTORTION_IP2,
  RFDEVICE_QTUNER_IM_DISTORTION_HD2,
  RFDEVICE_QTUNER_IM_DISTORTION_IHD2,
  RFDEVICE_QTUNER_IM_DISTORTION_COMMON,
  RFDEVICE_QTUNER_IM_DISTORTION_MAX,
  RFDEVICE_QTUNER_IM_DISTORTION_INVALID = 0xFF
}rfdevice_qtuner_distortion_config_type;


#endif/*RFDEVICE_QTUNER_API_H*/