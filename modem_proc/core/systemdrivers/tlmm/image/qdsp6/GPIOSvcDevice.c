/*===========================================================================

FILE:      GPIOSvcDevice.c

DESCRIPTION:
This file contains the hardware abstraction for the micro GPIO driver.
This file provides the public class implementation of the GPIO driver.

PUBLIC CLASSES:
  TLMM_SvcDeviceInit
  TLMM_SvcDeviceGetGpioId

===========================================================================
Copyright (c) 2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
========================================================================================================*/

/*========================================================================================================

  EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/systemdrivers/tlmm/image/qdsp6/GPIOSvcDevice.c#2 $

  when       who     what, where, why
  --------   ---     -------------------------------------------------------------------------------------
  06/02/17   dcf     Created.
  ======================================================================================================*/

#include <DALFramework.h>
#include "DALDeviceId.h"
#include <DALSys.h>
#include "ChipInfo.h"
#include "com_dtypes.h"
#include "GPIOSvcDevice.h"
#include "GPIOTypes.h"
#include "GPIOIFace.h"

#include "DDIPlatformInfo.h"


struct TLMMPropHandleType
{
  DALSYS_PROPERTY_HANDLE_DECLARE(hDevice);
  DALSYS_PROPERTY_HANDLE_DECLARE(hCmnIO);
  DALSYS_PROPERTY_HANDLE_DECLARE(hPlatIO);
};

struct TLMMPropHandleType PropsHandle;

extern uint32 ChipVersion;
extern ChipInfoFamilyType ChipFamily;
TLMMPlatformMapType *pPlatformMap = NULL;
static boolean bHasCommonIO = FALSE;


/*========================================================================================================

 FUNCTION    TLMM_SvcDeviceInit

========================================================================================================*/

boolean GPIO_SvcDeviceInit
(
  void
)
{
  DALResult eResult = DAL_ERROR;
  DALSYSPropertyVar tPropVar;
  uint32 nIdx, nArrSize, nKVPSType;
  DalDeviceHandle *hPlatformInfo = NULL;
  DalPlatformInfoPlatformInfoType PlatformInfo;
  TLMMPlatformMapType *pDeviceInfo = NULL;

  if ( ChipVersion == 0 )
  {
    ChipVersion = (uint32)ChipInfo_GetChipVersion();
  }
  
  ChipFamily = ChipInfo_GetChipFamily();

  if ((DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_PLATFORMINFO, &hPlatformInfo)) ||
      (hPlatformInfo == NULL))
  {
    return FALSE;
  }

  DalPlatformInfo_GetPlatformInfo(hPlatformInfo, &PlatformInfo);

  eResult = DALSYS_GetDALPropertyHandle( DALDEVICEID_TLMM, PropsHandle.hDevice );
  if ( eResult != DAL_SUCCESS )
  {
    return FALSE;
  }

  /*
   * Attempt to retrieve the platform details for this chip.
   */
  eResult = DALSYS_GetPropertyValue( PropsHandle.hDevice, "TlmmPlatformGroups", 0, &tPropVar );

  if( DAL_SUCCESS != eResult )
  {
    return(FALSE);
  }

  nArrSize = (tPropVar.dwLen/sizeof(TLMMPlatformMapType));
  pDeviceInfo = (TLMMPlatformMapType*)tPropVar.Val.pStruct;

  for (nIdx = 0; nIdx < nArrSize; nIdx++)
  {
    if (pDeviceInfo != NULL)
    {
      if ((ChipFamily == pDeviceInfo[nIdx].eChipFamily) &&
          (PlatformInfo.platform == pDeviceInfo[nIdx].Platform.platform) &&
          //  (PlatformInfo.version == pDeviceInfo[nIdx].Platform.version) &&
          (PlatformInfo.subtype == pDeviceInfo[nIdx].Platform.subtype) &&
          (PlatformInfo.fusion == pDeviceInfo[nIdx].Platform.fusion))
      {
        if (pDeviceInfo[nIdx].nKVPS != 0)
        {
          if (DalPlatformInfo_GetKeyValue(hPlatformInfo, 
             (DalPlatformInfoKeyType)pDeviceInfo[nIdx].nKVPS, &nKVPSType) == DAL_SUCCESS)
          {
            if (pDeviceInfo[nIdx].nKVPSType != nKVPSType)
            {
              continue;
            }
          }
        }
        pPlatformMap = &pDeviceInfo[nIdx];
        break;
      }
    }
  }

  if (NULL != pPlatformMap)
  {
    /*
     * Attempt to get the platform-specific GPIO configurations where supported.
     */
    eResult = DALSYS_GetDALPropertyHandleStr(pPlatformMap->pszPlatform, PropsHandle.hPlatIO);
  }
  else
  {
    eResult = DALSYS_GetPropertyValue( PropsHandle.hDevice, "TLMMDefaultPlatformGroup", 0, &tPropVar );
    if ( eResult != DAL_SUCCESS )
    {
      return FALSE;
    }
    
    /*
     * Pick the default configuration corresponding to this ChipFamily
     * If it is not present, Pick the first one from default configs
     */
    pDeviceInfo = (TLMMPlatformMapType*)tPropVar.Val.pStruct;
    for (nIdx = 0; nIdx < nArrSize; nIdx++)
    {
      if (ChipFamily == pDeviceInfo[nIdx].eChipFamily)
      {
        pPlatformMap = &pDeviceInfo[nIdx];  
      }
     }

    if( pPlatformMap == NULL)
    {
    pPlatformMap = &pDeviceInfo[0];
    }
    /*
     * Revert to the default platform-specific GPIO configurations.
     */
    eResult = DALSYS_GetDALPropertyHandleStr(pPlatformMap->pszPlatform, PropsHandle.hPlatIO);
    if ( eResult != DAL_SUCCESS )
    {
      return FALSE;
    }
  }
  
  /*
   * Connect the common IO list if it exists. 
   */
  eResult = DALSYS_GetDALPropertyHandleStr("/gpio/plat_common[1]", PropsHandle.hCmnIO);
  
  if (eResult == DAL_SUCCESS)
  {
    bHasCommonIO = TRUE;
  }
   
  return TRUE;

}


/*========================================================================================================

 FUNCTION    TLMM_SvcDeviceGetGpioId

========================================================================================================*/

GPIOIDType *GPIO_SvcDeviceGetGpioId
(
  const char *pszString
)
{
  DALSYSPropertyVar tPropVar;
  DALResult eResult;
  
  if ( bHasCommonIO == TRUE )
  {
    eResult = DALSYS_GetPropertyValue( PropsHandle.hCmnIO, pszString, 0, &tPropVar );
    
    if ( eResult == DAL_SUCCESS )
    {
      return(GPIOIDType*)tPropVar.Val.pStruct;
    }   
  }
  eResult = DALSYS_GetPropertyValue( PropsHandle.hPlatIO, pszString, 0, &tPropVar );

  if ( eResult == DAL_SUCCESS )
  {
    return (GPIOIDType*)tPropVar.Val.pStruct;
  }
  return NULL;

} /* TLMM_GetGpioId */

