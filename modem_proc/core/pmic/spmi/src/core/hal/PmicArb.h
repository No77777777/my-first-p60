/**
 * @file:  PmicArb.h
 * @brief: 
 * 
 * Copyright (c) 2013-2016 by Qualcomm Technologies Incorporated. All Rights Reserved.
 * 
 * $DateTime: 2019/04/24 00:03:26 $
 * $Header: //components/rel/core.mpss/10.0/pmic/spmi/src/core/hal/PmicArb.h#1 $
 * $Change: 18983508 $ 
 * 
 *                              Edit History
 * Date     Description
 * -------  -------------------------------------------------------------------
 * 8/06/15  More than 256 chan support.
 * 1/20/15  Multiple bus support
 * 10/1/13  Initial Version
 */
#ifndef PMICARB_H
#define	PMICARB_H

#include "SpmiTypes.h"
#include "SpmiUtils.h"
#include "SpmiHal.h"
#include "SpmiInfo.h"

typedef struct 
{
    uint32 irqStatus;
    uint32 geniStatus;
    uint32 geniCtrl;
} PmicArbHwStatus;


Spmi_Result PmicArb_Init(SpmiInfo *pSpmiInfo);
Spmi_Result PmicArb_FindChannel(SpmiDevInfo *pDev, uint8 slaveId, uint8 periph, uint8 owner, uint16* channel);
Spmi_Result PmicArb_FindVrmId(SpmiDevInfo *pDev, uint8 slaveId, uint8 periph, uint16* vrmId);
Spmi_Result PmicArb_UpdateCache(SpmiInfo *pSpmiInfo);
Spmi_Result PmicArb_RemoveCacheEntry(SpmiDevInfo *pDev, uint16 chan);
Spmi_Result PmicArb_AddCacheEntry(SpmiDevInfo *pDev, uint16 chan, uint8 sid, uint8 periph);
Spmi_Result PmicArb_ExecuteCommand(SpmiDevInfo *pDev,
                                   PmicArbCmd cmd,
                                   Spmi_AccessPriority priority,
                                   uint8 owner,
                                   uint16 channel,
                                   uint16 offset,
                                   uint8* data,
                                   uint32 dataLen,
                                   uint32* bytesTransacted);


void PmicArb_GetHwStatus(SpmiDevInfo *pDev, PmicArbHwStatus* status);


Spmi_Result 
PmicArb_ExecuteCommandDebug ( uint8*              dbgBaseAddrs,
                              PmicArbCmd          cmd,
                              Spmi_AccessPriority priority, 
                              uint8               slaveId, 
                              uint16              address,
                              uint8*              data, 
                              uint32              dataLen, 
                              uint32*             bytesTransacted );

#endif
