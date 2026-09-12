/**
 * @file:  SpmiOsTarget.h
 * @brief: Implements platform specific functionality
 * 
 * Copyright (c) 2013 by Qualcomm Technologies Incorporated. All Rights Reserved.
 * 
 * $DateTime: 2019/04/24 00:03:26 $
 * $Header: //components/rel/core.mpss/10.0/pmic/spmi/src/platform/os/mpss/SpmiOsTarget.h#1 $
 * $Change: 18983508 $ 
 * 
 *                              Edit History
 * Date     Description
 * -------  -------------------------------------------------------------------
 * 10/1/13  Initial Version
 */
#ifndef SPMIOSTARGET_H
#define	SPMIOSTARGET_H

#include "SpmiTypes.h"
#include "DDISpmi.h"

extern DALResult Spmi_ConfigureFetcherChannel(
	uint32 ClientHandle,
    DalDeviceHandle *h,
    uint16 periph,
    DALBOOL enableInt,
    uint32 *pChannel);

#endif
