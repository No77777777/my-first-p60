/*
  @brief: This module provides an internal the interface to the 
  SPMI bus driver software.
  
  Copyright (c) 2015 by Qualcomm Technologies Incorporated. All Rights Reserved. 
  
  $DateTime: 2019/04/24 00:03:26 $
  $Header: //components/rel/core.mpss/10.0/pmic/spmi/src/core/SpmiBusInternal.h#1 $
  $Change: 18983508 $ 
  
                               Edit History
  Date      Description
  --------  -------------------------------------------------------------------
  11/10/15  Initial Version
*/
#ifndef SPMIBUSINTERNAL_H
#define SPMIBUSINTERNAL_H

#include "SpmiTypes.h"
#include "SpmiInfo.h"

/**
 * @brief Get the SPMI bus information structure
 *
 * This function gets the internal SPMI bus information 
 * structure.
 * 
 * @param[in] vSlaveId   Virtual slave Id of the device (see 
 *                       SpmiEncodeVSid)
 * 
 * @return Pointer to the device info structure, or 
 *         NULL on errror
 */
SpmiDevInfo* SpmiBus_GetDevInfo(uint8 vSlaveId);

/**
 * @brief Get the SPMI bus information structure
 *
 * This function gets the internal SPMI bus information 
 * structure.
 * 
 * @return Pointer to the device info structure, or 
 *         NULL on errror
 */
SpmiInfo* SpmiBus_GetInfo(void);

#endif
