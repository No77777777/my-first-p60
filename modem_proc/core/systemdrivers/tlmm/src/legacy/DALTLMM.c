/*===========================================================================

             M A I N   D A L   T L M M   D R I V E R   F I L E

DESCRIPTION

  Contains the majority of functionality for the DAL TLMM driver.  The
  API in this module MUST always remain target independent.


===========================================================================
             Copyright (c) 2010 - 2015 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/tlmm/src/legacy/DALTLMM.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/27/17   dcf     Revamp for internal re-architecture of the GPIO driver.
03/09/09   dcf     Fix for immediateConfigs in restoring GPIOs from sleep.
02/23/09   an      Mainlining FEATURE_MANAGE_GPIOS_FOR_SLEEP and
                   FEATURE_APPS_GPIOS_LOW_POWER by having internal flags
                   in TLMM.
01/08/09   dcf     General clean up, removal of bare enumerated types and
                   addition of uniform headers and comments.
===========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include <DALStdDef.h>
#include "com_dtypes.h"
#include "DALTLMM.h"
#include "DDITlmm.h"
#include "TlmmDefs.h"
#include "GPIOTypes.h"
#include "GPIO.h"
#include "GPIODiag.h"
#include "msmhwiobase.h"
#include "HALhwio.h"


/*==========================================================================

                     LOCAL TYPE DEFINES FOR THIS MODULE

==========================================================================*/

GPIOClientHandleType GPIOHandle;

/*==========================================================================

                     LOCAL DATA FOR THIS MODULE

==========================================================================*/

uint32 bInitialized = 0x0;

typedef struct
{
  GPIOKeyType    nKey;
  uint32         nFunc;
  GPIOConfigType Config;

}GPIOKeyFuncType;



typedef struct
{
  uint32 ePort;
  uint32 nOffset;
  uint32 nMask;
  uint32 nEnableValue;
  uint32 nDisableValue;

}DALTLMM_PortCfgType;


#define GPIO_NUM_ENTRIES 64
GPIOKeyFuncType gGpioFuncs[GPIO_NUM_ENTRIES];
static DALTLMM_PortCfgType gtHALPorts[DAL_TLMM_NUM_PORTS];

/*==========================================================================

      GENERIC DAL FUNCTIONS NOT CURRENTLY IMPLEMENTED IN THIS MODULE

==========================================================================*/

extern void TLMM_ClearContextData(uint32 nIdx);

DALResult
TLMM_DriverInit(TLMMDrvCtxt *pCtxt)
{
  return DAL_SUCCESS;
}

DALResult
TLMM_DriverDeInit(TLMMDrvCtxt *pCtxt)
{
  return DAL_SUCCESS;
}

DALResult
TLMM_PowerEvent(TLMMClientCtxt *pCtxt, DalPowerCmd PowerCmd,
                    DalPowerDomain PowerDomain)
{
    return DAL_SUCCESS;
}

DALResult
TLMM_Open(TLMMClientCtxt *pCtxt, uint32 dwaccessMode )
{
    return DAL_SUCCESS;
}

DALResult
TLMM_Close(TLMMClientCtxt *pCtxt)
{
    return DAL_SUCCESS;
}

DALResult
TLMM_Info(TLMMClientCtxt *pCtxt,DalDeviceInfo *pdeviceInfo, uint32 dwSize)
{
    return DAL_SUCCESS;
}

DALResult TLMM_PostInit( TLMMClientCtxt *pCtxt );

/*==========================================================================

                     APPLICATION PROGRAMMING INTERFACE

==========================================================================*/


uint32 TLMM_DriveCvtToHW
(
  uint32 nLogicalVal
)
{
  if (nLogicalVal <=200)  { return 0;}
  if (nLogicalVal <=400)  { return 1;}
  if (nLogicalVal <=600)  { return 2;}
  if (nLogicalVal <=800)  { return 3;}
  if (nLogicalVal <=1000) { return 4;}
  if (nLogicalVal <=1200) { return 5;}
  if (nLogicalVal <=1400) { return 6;}

  return 7;

}


uint32 TLMM_DriveCvtToLogical
(
  uint32 nHWVal
)
{
  if (nHWVal <= 7)
  {
    return ((nHWVal + 1) * 200);
  }
  return 1600;

} /* TLMM_DriveCvtToLogical */




/*==========================================================================

  FUNCTION      TLMM_DeviceInit

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_DeviceInit(TLMMClientCtxt *pCtxt)
{
  DALSYSPropertyVar tPropVar;
  uint32 nHALPort = 0;
  uint32 gnTotalPorts = 0;
  uint32 nPortIdx = 0;
  
  /*
   * Check for shared memory.  If already initialized, get a pointer and
   * we're done.
   */
  if( !bInitialized )
  {
    if ( GPIO_SUCCESS != GPIO_Attach(GPIO_DEVICE_TLMM, &GPIOHandle) )
    {
      return DAL_ERROR;
    }
    
     /* 
     * Attempt to retrieve any port configuration structures.
     */
    if (DAL_SUCCESS == DALSYS_GetPropertyValue( pCtxt->pDevCtxt->hProp, "tlmm_ports", 0, &tPropVar ))
    {
      while( tPropVar.Val.pdwVal[nPortIdx] != 0xFFFFFFFF )
      {
        nHALPort = tPropVar.Val.pdwVal[nPortIdx++];

        gtHALPorts[nHALPort].ePort = nHALPort;
        gtHALPorts[nHALPort].nOffset = tPropVar.Val.pdwVal[nPortIdx++];
        gtHALPorts[nHALPort].nMask = tPropVar.Val.pdwVal[nPortIdx++];
        gtHALPorts[nHALPort].nEnableValue = tPropVar.Val.pdwVal[nPortIdx++];
        gtHALPorts[nHALPort].nDisableValue = tPropVar.Val.pdwVal[nPortIdx++];
        gnTotalPorts++;
      }
    }   
    
    bInitialized = TRUE;
  }

  return(DAL_SUCCESS);

} /* TLMM_DeviceInit */


/*==========================================================================

  FUNCTION      TLMM_DeviceDeInit

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_DeviceDeInit
(
  TLMMClientCtxt *pCtxt
)
{
  return DAL_SUCCESS;

} /* TLMM_DeviceDeInit */


/*==========================================================================

  FUNCTION      TLMM_DetachCleanup

  DESCRIPTION   Cleans up client info from a client context after a final
                detach.

==========================================================================*/

DALResult TLMM_DetachCleanup
(
  TLMMClientCtxt *pCtxt
)
{
  #if 0
  uint32 nIdx;

  (void)DALSYS_SyncEnter(pCtxt->pDrvCtxt->hTlmmSync);

  for (nIdx=0; nIdx<gnTotalGPIO; nIdx++)
  {
    if(ganGpioIdUsers[nIdx].nClientId == pCtxt->ClientInfo.nClientId)
    {
      TLMM_ConfigGpioIdInactive(pCtxt, ganGpioIdUsers[nIdx].nGpioKey);
      TLMM_ReleaseGpioId(pCtxt, ganGpioIdUsers[nIdx].nGpioKey);
    }
  }

  TLMM_ClearContextData(pCtxt->ClientInfo.nIdx);

  (void)DALSYS_SyncLeave(pCtxt->pDrvCtxt->hTlmmSync);
  #endif
  return DAL_SUCCESS;
}


/*==========================================================================

  FUNCTION      TLMM_ConfigGpioGroup

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_ConfigGpioGroup
(
  TLMMClientCtxt*     pCtxt,
  DALGpioEnableType   eEnable,
  DALGpioSignalType*  eGpioGroup,
  uint32              nSize
)
{
  uint32 nIdx;

  if (eGpioGroup != NULL)
  {
    for (nIdx = 0; nIdx < nSize; nIdx++)
    {
      if (TLMM_ConfigGpio(pCtxt, eGpioGroup[nIdx], eEnable) != DAL_SUCCESS)
      {
        return DAL_ERROR;
      }
    }
    return DAL_SUCCESS;
  }
  return DAL_ERROR;

} /* TLMM_ConfigGpioGroup */


/*==========================================================================

  FUNCTION      TLMM_ConfigGpio

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_ConfigGpio
(
  TLMMClientCtxt*     pCtxt,
  DALGpioSignalType   eGpioConfig,
  DALGpioEnableType   eEnable
)
{
  GPIOKeyType GPIOKey;
  GPIOConfigType Config = {0};
  
  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandle, DAL_GPIO_NUMBER(eGpioConfig), GPIO_ACCESS_SHARED, &GPIOKey))
  {
    Config.func = DAL_GPIO_FUNCTION(eGpioConfig);
    Config.dir = DAL_GPIO_DIRECTION(eGpioConfig);
    Config.pull = DAL_GPIO_PULL(eGpioConfig);
    Config.drive = TLMM_DriveCvtToLogical(DAL_GPIO_DRIVE(eGpioConfig));
    
    if ( eEnable == DAL_TLMM_GPIO_ENABLE )
    {
    if ( GPIO_SUCCESS == GPIO_ConfigPin(GPIOHandle, GPIOKey, Config) )
    {
      return DAL_SUCCESS;
    }
  }
    else
    {
      if ( GPIO_SUCCESS == GPIO_ConfigPinInactive(GPIOHandle, GPIOKey) )
      {
        return DAL_SUCCESS;
      }
    }
  }
  return DAL_ERROR;

} /* TLMM_ConfigGpio */


/*==========================================================================

  FUNCTION      TLMM_SetPort

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult  TLMM_SetPort
(
  TLMMClientCtxt*  pCtxt,
  DALGpioPortType  ePort,
  uint32           nValue
)
{
  DALResult eResult = DAL_ERROR;
  if( (ePort < DAL_TLMM_NUM_PORTS) && (gtHALPorts[ePort].ePort != DAL_TLMM_NUM_PORTS) )
  {
   // (void)DALSYS_SyncEnter(pCtxt->pDrvCtxt->hTlmmSync);
    
    if( nValue != 0 )
    {
      outpdw(gtHALPorts[ePort].nOffset, (gtHALPorts[ePort].nMask & gtHALPorts[ePort].nEnableValue));
    }
    else
    {
      outpdw(gtHALPorts[ePort].nOffset, (gtHALPorts[ePort].nMask & gtHALPorts[ePort].nDisableValue));
    }
    eResult = DAL_SUCCESS;

   // (void)DALSYS_SyncLeave(pCtxt->pDrvCtxt->hTlmmSync);
  }

  return(eResult);

} /* TLMM_SetPort */


/*==========================================================================

  FUNCTION      TLMM_GetGpioNumber

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult  TLMM_GetGpioNumber
(
  TLMMClientCtxt*    pCtxt,
  DALGpioSignalType  eGpioConfig,
  uint32*            nGpioNumber
)
{
  *nGpioNumber = (uint32)DAL_GPIO_NUMBER(eGpioConfig);
  return(DAL_SUCCESS);

} /* TLMM_GetGpioNumber */


/*==========================================================================

  FUNCTION      TLMM_GpioIn

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult  TLMM_GpioIn
(
  TLMMClientCtxt*    pCtxt,
  DALGpioSignalType  eGpioConfig,
  DALGpioValueType*  eValue
)
{
  GPIOKeyType GPIOKey;
  GPIOValueType PinValue = GPIO_LOW;

  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandle, DAL_GPIO_NUMBER(eGpioConfig), GPIO_ACCESS_SHARED, &GPIOKey))
  {
    if ( GPIO_SUCCESS == GPIO_ReadPin(GPIOHandle, GPIOKey, &PinValue) )
    {
      if (PinValue == GPIO_HIGH)
      {
        *eValue = DAL_GPIO_HIGH_VALUE;        
      }
      else
      {
        *eValue = DAL_GPIO_LOW_VALUE;
      }
      
      return DAL_SUCCESS;
    }
  }
  return DAL_ERROR;

} /* TLMM_GpioIn */


/*==========================================================================

  FUNCTION      TLMM_GpioOutGroup

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult  TLMM_GpioOutGroup
(
  TLMMClientCtxt*    pCtxt,
  DALGpioSignalType* aeGpioConfigGroup,
  uint32             nSize,
  DALGpioValueType   eValue
)
{
  uint32 nIdx;

  if ( NULL != aeGpioConfigGroup )
  {
    for (nIdx = 0; nIdx < nSize; nIdx++)
    {
      if (DAL_SUCCESS != TLMM_GpioOut(pCtxt,
                                      aeGpioConfigGroup[nIdx],
                                      eValue))
      {
        return DAL_ERROR;
      }
    }
    return DAL_SUCCESS;
  }
  return DAL_ERROR;

} /* TLMM_GpioOutGroup */


/*==========================================================================

  FUNCTION      TLMM_GpioOut

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult  TLMM_GpioOut
(
  TLMMClientCtxt*   pCtxt,
 DALGpioSignalType  eGpioConfig,
 DALGpioValueType   eValue
)
{
  GPIOKeyType GPIOKey;
  GPIOValueType ePinValue = GPIO_LOW;

  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandle, DAL_GPIO_NUMBER(eGpioConfig), GPIO_ACCESS_SHARED, &GPIOKey))
  {
    if ( eValue == DAL_GPIO_HIGH_VALUE )
    {
      ePinValue = GPIO_HIGH;
    }
    if ( GPIO_SUCCESS == GPIO_WritePin(GPIOHandle, GPIOKey, ePinValue) )
    {
      return DAL_SUCCESS;
    }
  }
  return DAL_ERROR;

} /* TLMM_GpioOut */


/*==========================================================================

  FUNCTION      TLMM_GetCurrentConfig

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_GetCurrentConfig
(
  TLMMClientCtxt*    pCtxt,
  uint32             nGpioNumber,
  DALGpioSignalType* eGpioConfig
)
{
  GPIOConfigType Config = {0};
  GPIOKeyType GPIOKey = 0;

  if (NULL != eGpioConfig)
  {
    if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandle, nGpioNumber, GPIO_ACCESS_SHARED, &GPIOKey))
    {
      GPIO_GetPinConfig(GPIOHandle, GPIOKey, &Config);
      *eGpioConfig = DAL_GPIO_CFG(nGpioNumber,
                                      Config.func,
                                      Config.dir,
                                      Config.pull,
                                      TLMM_DriveCvtToHW(Config.drive));
      return DAL_SUCCESS;
    }
  }
  return DAL_ERROR;

} /* TLMM_GetCurrentConfig */


/*==========================================================================

  FUNCTION      TLMM_GetPinStatus

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_GetPinStatus
(
  TLMMClientCtxt*     pCtxt,
  uint32              nGpioNumber,
  DALGpioStatusType*  eStatus
)
{
  GPIOKeyType GPIOKey = 0;

  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandle, nGpioNumber, GPIO_ACCESS_SHARED, &GPIOKey))
  {
    if (TRUE == GPIO_IsPinActive(GPIOHandle, GPIOKey))
    {
      *eStatus = (DALGpioStatusType)TRUE;
    }
    else
    {
      *eStatus = (DALGpioStatusType)FALSE;
    }
    return DAL_SUCCESS;
  }
  return DAL_ERROR;

} /* TLMM_GetPinStatus */


/*==========================================================================

  FUNCTION      TLMM_SetInactiveConfig

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_SetInactiveConfig
(
  TLMMClientCtxt*   pCtxt,
  uint32            nGpioNumber,
  DALGpioSignalType nInactiveConfig
)
{
  GPIOKeyType GPIOKey = 0;
  GPIOConfigType Config;
  GPIOValueType  Value;

  Config.func = DAL_GPIO_FUNCTION(nInactiveConfig);
  Config.dir = DAL_GPIO_DIRECTION(nInactiveConfig);
  Config.pull = DAL_GPIO_PULL(nInactiveConfig);
  Config.drive = TLMM_DriveCvtToLogical(DAL_GPIO_DRIVE(nInactiveConfig));

  Value = (GPIOValueType)DAL_GPIO_GET_OUTVAL(nInactiveConfig);

  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandle, nGpioNumber, GPIO_ACCESS_SHARED, &GPIOKey))
  {
    if (GPIO_SUCCESS == GPIO_SetInactiveConfig(GPIOHandle, GPIOKey, Config, Value))
    {
      return(DAL_SUCCESS);
    }
  }

  return(DAL_ERROR);

} /* TLMM_SetInactiveConfig */


/*==========================================================================

  FUNCTION      TLMM_GetInactiveConfig

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_GetInactiveConfig
(
  TLMMClientCtxt*    pCtxt,
  uint32             nGpioNumber,
  DALGpioSignalType* eGpioConfig
)
{
  GPIOKeyType GPIOKey = 0;
  GPIOConfigType Config;
  GPIOValueType  Value;

  if ( eGpioConfig != NULL )
  {
    if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandle, nGpioNumber, GPIO_ACCESS_SHARED, &GPIOKey))
    {
      if (GPIO_SUCCESS == GPIO_GetInactiveConfig(GPIOHandle, GPIOKey, &Config, &Value))
      {
        *eGpioConfig = DAL_GPIO_CFG_OUT(nGpioNumber,
                                        Config.func,
                                        Config.dir,
                                        Config.pull,
                                        TLMM_DriveCvtToHW(Config.drive),
                                        Value);
        return(DAL_SUCCESS);
      }
    }
  }
  return(DAL_ERROR);

} /* TLMM_GetInactiveConfig */



/*==========================================================================

  FUNCTION      TLMM_GetOutput

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_GetOutput
(
  TLMMClientCtxt*   pCtxt,
  uint32            nGpioNumber,
  DALGpioValueType* nValue
)
{
  GPIOKeyType GPIOKey = 0;

  if (NULL != nValue)
  {
    if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandle, nGpioNumber, GPIO_ACCESS_SHARED, &GPIOKey))
    {
      if(TRUE == GPIO_GetOutput(GPIOHandle, GPIOKey))
      {
        *nValue = DAL_GPIO_HIGH_VALUE;
      }
      else
      {
         *nValue = DAL_GPIO_LOW_VALUE;
      }
      return(DAL_SUCCESS);
    }
  }
  return(DAL_ERROR);

} /* TLMM_GetOutput */


/*==========================================================================

  FUNCTION      TLMM_GetGpioId

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_GetGpioId
(
  TLMMClientCtxt*   pCtxt,
  const char*       pszGpioStr,
  uint32            nSize,
  DALGpioIdType*    pnGpioId
)
{
  uint32 nIdx;
  GPIOKeyType GPIOKey = 0;
  GPIOConfigType Config = {0};

  if (GPIO_SUCCESS == GPIO_RegisterPin(GPIOHandle, pszGpioStr, GPIO_ACCESS_SHARED, &Config, &GPIOKey))
  {
    *pnGpioId = (DALGpioIdType)GPIOKey;
    for (nIdx = 0; nIdx<GPIO_NUM_ENTRIES; nIdx++)
    {
      if (gGpioFuncs[nIdx].nKey == 0)
      {
        gGpioFuncs[nIdx].nKey = GPIOKey;
        gGpioFuncs[nIdx].nFunc = Config.func;
        break;
      }
    }
    return DAL_SUCCESS;
  }

  return(DAL_ERROR);

} /* TLMM_GetGpioId */


/*==========================================================================

  FUNCTION      TLMM_ConfigGpioId

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_ConfigGpioId
(
  TLMMClientCtxt*          pCtxt,
  DALGpioIdType            nGpioId,
  DalTlmm_GpioConfigIdType *pUserSettings
)
{
  uint32 nIdx;
  GPIOConfigType Config = {0};

  if (pUserSettings != NULL)
  {
    Config.dir = pUserSettings->eDirection;
    Config.pull = pUserSettings->ePull;
    Config.drive = TLMM_DriveCvtToLogical(pUserSettings->eDriveStrength);

    for ( nIdx=0; nIdx<GPIO_NUM_ENTRIES; nIdx++ )
    {
      if (gGpioFuncs[nIdx].nKey == nGpioId)
      {
        Config.func = gGpioFuncs[nIdx].nFunc;
        gGpioFuncs[nIdx].Config.dir = Config.dir;
        gGpioFuncs[nIdx].Config.pull = Config.pull;
        gGpioFuncs[nIdx].Config.drive = Config.drive;
        gGpioFuncs[nIdx].Config.func = Config.func;
        break;
      }
    }

    if (GPIO_SUCCESS == GPIO_ConfigPin(GPIOHandle, (GPIOKeyType)nGpioId, Config) )
    {
      return DAL_SUCCESS;
    }
  }

  return(DAL_ERROR);

} /* TLMM_ConfigGpioId */


/*==========================================================================

  FUNCTION      TLMM_ConfigGpioIdInactive

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_ConfigGpioIdInactive
(
  TLMMClientCtxt*          pCtxt,
  DALGpioIdType            GPIOKey
)
{
  uint32 nGpioNumber = GPIO_GetPinNumber(GPIOKey);

  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(GPIOHandle, nGpioNumber, GPIO_ACCESS_SHARED, &GPIOKey))
  {
    if (GPIO_SUCCESS == GPIO_ConfigPinInactive(GPIOHandle, GPIOKey))
    {
      return(DAL_SUCCESS);
    }
  }
  return(DAL_ERROR);

} /* TLMM_ConfigGpioIdInactive */


/*==========================================================================

  FUNCTION      TLMM_GpioIdOut

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_GpioIdOut
(
  TLMMClientCtxt*          pCtxt,
  DALGpioIdType            nGpioId,
  DALGpioValueType         eGpioValue
)
{
  GPIOConfigType Config = {0};
  GPIOValueType Val = GPIO_LOW;

  if ( GPIO_SUCCESS == GPIO_GetPinConfig(GPIOHandle, (GPIOKeyType)nGpioId, &Config) )
  {
    if (eGpioValue == DAL_GPIO_HIGH_VALUE)
    {
      Val = GPIO_HIGH;
    }
    GPIO_WritePin(GPIOHandle, (GPIOKeyType)nGpioId, Val);
    return DAL_SUCCESS;
  }

  return DAL_ERROR;

} /* TLMM_GpioIdOut */


/*==========================================================================

  FUNCTION      TLMM_GpioIdIn

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_GpioIdIn
(
  TLMMClientCtxt*          pCtxt,
  DALGpioIdType            nGpioId,
  DALGpioValueType         *eGpioValue
)
{
  GPIOConfigType Config = {0};
  GPIOValueType eValue;

  if ( eGpioValue != NULL )
  {
    if ( GPIO_SUCCESS == GPIO_GetPinConfig(GPIOHandle, (GPIOKeyType)nGpioId, &Config) )
    {
      if ( GPIO_SUCCESS == GPIO_ReadPin(GPIOHandle, (GPIOKeyType)nGpioId, &eValue) )
      {
        if (eValue == GPIO_HIGH)
        {
          *eGpioValue = DAL_GPIO_HIGH_VALUE;        
        }
        else
        {
          *eGpioValue = DAL_GPIO_LOW_VALUE;
        }
      }
      return DAL_SUCCESS;
    }
  }
  return DAL_ERROR;

} /* TLMM_GpioIdIn */


/*==========================================================================

  FUNCTION      TLMM_ReleaseGpioId

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_ReleaseGpioId
(
  TLMMClientCtxt*          pCtxt,
  DALGpioIdType            nGpioId
)
{
  return(DAL_SUCCESS);

} /* TLMM_ReleaseGpioId */


/*==========================================================================

  FUNCTION      TLMM_SelectGpioIdMode

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_SelectGpioIdMode
(
  TLMMClientCtxt*           pCtxt,
  DALGpioIdType             nGpioId,
  DalGpioModeType           eMode,
  DalTlmm_GpioConfigIdType* pUserSettings
)
{
  uint32 nIdx;
  GPIOConfigType Config;
  
  for (nIdx = 0; nIdx<GPIO_NUM_ENTRIES; nIdx++)
  {
    if (gGpioFuncs[nIdx].nKey == nGpioId)
    {
      if ( eMode == DAL_GPIO_MODE_PRIMARY )
      {
        Config.func = gGpioFuncs[nIdx].nFunc;
      }
      else
      {
        Config.func = 0;
      }
      if ( pUserSettings != NULL )
      {
        Config.dir = (uint32)pUserSettings->eDirection;
        Config.pull = (uint32)pUserSettings->ePull;
        Config.drive = (uint32)TLMM_DriveCvtToLogical(pUserSettings->eDriveStrength);
      }
      else
      {
        Config.dir = gGpioFuncs[nIdx].Config.dir;
        Config.pull = gGpioFuncs[nIdx].Config.pull;
        Config.drive = gGpioFuncs[nIdx].Config.drive;
      }
      
      if (GPIO_SUCCESS == GPIO_ConfigPin(GPIOHandle, (GPIOKeyType)nGpioId, Config) )
      {
        return (DAL_SUCCESS);
      }
    }
  }
  return(DAL_SUCCESS);

} /* TLMM_SelectGpioIdMode */


/*==========================================================================

  FUNCTION      TLMM_GetGpioIdSettings

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_GetGpioIdSettings
(
  TLMMClientCtxt*            pCtxt,
  DALGpioIdType              nGpioId,
  DalTlmm_GpioIdSettingsType *pGpioSettings
)
{
  GPIOConfigType Config = {0};

  if (pGpioSettings != NULL)
  {
    if (GPIO_SUCCESS == GPIO_GetPinConfig(GPIOHandle, (GPIOKeyType)nGpioId, &Config))
    {
      pGpioSettings->nFunctionSelect = Config.func;
      pGpioSettings->Settings.eDirection = (DALGpioDirectionType)Config.dir;
      pGpioSettings->Settings.ePull = (DALGpioPullType)Config.pull;
      pGpioSettings->Settings.eDriveStrength = (DALGpioDriveType)TLMM_DriveCvtToHW(Config.drive);
      pGpioSettings->nGpioNumber = GPIO_GetPinNumber((GPIOKeyType)nGpioId);
      return DAL_SUCCESS;
    }
  }
  return DAL_ERROR;

} /* TLMM_GetGpioIdSettings */


/*==========================================================================

  FUNCTION      TLMM_ConfigGpioIdModeIndexed

  DESCRIPTION   See DALTLMM.h

==========================================================================*/

DALResult TLMM_ConfigGpioIdModeIndexed
(
  TLMMClientCtxt* pCtxt,
  DALGpioIdType   nGpioId,
  uint32          nMode
)
{
  return(DAL_SUCCESS);

}

