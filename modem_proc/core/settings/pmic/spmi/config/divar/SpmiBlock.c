/**
 * @file:  SpmiBlock.c
 * 
 * Copyright (c) 2013-2017 by Qualcomm Technologies 
 * Incorporated. All Rights Reserved. 
 * 
 * $DateTime: 2021/04/03 18:17:35 $
 * $Header: //components/rel/core.mpss/10.0/settings/pmic/spmi/config/divar/SpmiBlock.c#1 $
 * $Change: 29910388 $ 
 * 
 *                              Edit History
 * Date     Description
 * -------  -------------------------------------------------------------------
 * 10/1/13  Initial Version
 */

#include "SpmiInfo.h"
#include "SpmiTypes.h"
#include "SpmiGenericConsts.h"

#define SPMI_PMIC_ARB_PHYS_ADDRESS 0x1C00000
#define SPMI_BARE_OWNER_NUMBER     2

#define SPMI_MAX_CHANNELS_SUPPORTED   (HWIO_PMIC_ARB_CHNLn_CMD_MAXn + 1)
#define SPMI_MAX_BUSES_SUPPORTED      (1)

static uint16 periphMap[SPMI_MAX_PERIPH_ID + 1];
static ChannelMapEntry channelMap[SPMI_MAX_CHANNELS_SUPPORTED];
static SpmiDeviceGenerics generics = INIT_SPMI_DEV_GENERICS;
static uint32 dbgPhysBaseAddr =  0x08A00000;

static SpmiTargetConfig spmiTgt =
{
    .hwioAddrStr    = "PMIC_ARB",
    .interruptVec   = 427,
};

static SpmiDevInfo spmiDevices[] = {
   {
      .baseAddrs          = (uint8*)SPMI_PMIC_ARB_PHYS_ADDRESS,
      .uDevIdx            = 0,
      .reservedChan       = SPMI_MAX_CHANNELS_SUPPORTED - 1,
      .nextChanIdx        = 0,
      .reservedChanEn     = TRUE,   
      .dynamicChannelMode = FALSE,   
      .periphMap          = periphMap,
      .channelMap         = channelMap,
      .pGenerics          = &generics,
      .target             = (void*) &spmiTgt,    
   },
};

static SpmiDbgDevInfo spmiDbgDevices = {
    //Make string also part of this 
    .dbgBaseAddrsStr = "QDSS_SOC_DBG",
    .dbgBaseAddrs    = (uint8*)&dbgPhysBaseAddr,
};


SpmiInfo spmiInfo = {
  .owner              = SPMI_BARE_OWNER_NUMBER,
  .uNumDevices        = SPMI_MAX_BUSES_SUPPORTED,
  .devices            = spmiDevices,
  .dbgInfo            = &spmiDbgDevices,
};

//SpmiInfo * pSpmiBlockInfo = &spmiInfo;

