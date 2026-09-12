/**
 * @file:  SpmiOsImage.c
 * 
 * Copyright (c) 2013 by Qualcomm Technologies Incorporated. All Rights Reserved.
 * 
 * $DateTime: 2019/04/24 00:03:26 $
 * $Header: //components/rel/core.mpss/10.0/pmic/spmi/src/platform/os/mpss/SpmiOsImage.c#1 $
 * $Change: 18983508 $ 
 * 
 *                              Edit History
 * Date     Description
 * -------  -------------------------------------------------------------------
 * 1/20/15  Multiple bus support
 * 11/3/14  Automatic channel assignment
 * 10/1/13  Initial Version
 */

#include "SpmiOs.h"
#include "SpmiBus.h"
#include "SpmiLogs.h"
#include "DALSys.h"
#include "DALDeviceId.h"
#include "DDISpmi.h"
#include "DDIHWIO.h"
#include "DDITimetick.h"
#include "DDIInterruptController.h"
#include "SpmiOsTarget.h"
#include "stdint.h"
#include "DDIChipInfo.h"
#include "SpmiHal.h"

//******************************************************************************
// Macros / Definitions / Constants
//******************************************************************************

// Hard coded pmic arb base address (property)
#define SPMI_HW_DEV_PROP "pmic_arb_base_addr"

// This owner number for the EE this driver is running on (property)
#define OWNER_PROP "owner"

// Name expected by the HWIO driver for the pmic arb base address
#define PMIC_ARB_BASE_ID "PMIC_ARB"
#define SPMI_DEVICES_PROP "spmi_devices"

#define MGPI_POS_ADDR_IDX 2
#define MGPI_MODEM_SPMI_WRITE_PORT 2
#define UIM_PVC_PORT 6
#define TRIG_WRITE_POS 0x90000

//******************************************************************************
// Global Data
//******************************************************************************

static boolean propHdlInited = FALSE;
static DALSYS_PROPERTY_HANDLE_DECLARE(dalPropDev);
static boolean spmiUseHWChannel = FALSE;

//******************************************************************************
// Local Helper Functions
//******************************************************************************

Spmi_Result SpmiOsNhlos_GetDalProp(char* propId, DALSYSPropertyVar* val)
{
    if(!propHdlInited)
    {
        if(DALSYS_GetDALPropertyHandleStr( DALSPMI_DEVICE_ID, dalPropDev ) != DAL_SUCCESS ) {
            SPMI_LOG_ERROR( "Unable to get SPMI property handle" );
            return SPMI_FAILURE_INIT_FAILED;
        }
        
        propHdlInited = TRUE;
    }
    
    return DALSYS_GetPropertyValue( dalPropDev, propId, 0, val ) == DAL_SUCCESS ? 
           SPMI_SUCCESS : SPMI_FAILURE_FUNCTIONALITY_NOT_SUPPORTED;
}


static Spmi_Result 
SpmiOs_WriteLongMgpi( uint8  vSlaveId,
                       Spmi_AccessPriority priority,
                       uint16 address,
                       uint8* data,
                       uint32 len )
{     
    Spmi_Result       rslt            = SPMI_SUCCESS; 
    DALSYSPropertyVar dalProp;
    uint8             slaveId;
    uint8             busId;
    uint8             *base_addr      = NULL;
    uint32            timeout         = SPMI_TIMEOUT_USEC;
    uint32            status          = 0;
    uint32            pvcStatus       = HWIO_PMIC_ARB_PVC_PORTn_STATUS_PVC_PORT_BUSY_BMSK; 
    uint32            pvcFailure      = 0;
    SpmiDevInfo       *pDev;
    SpmiInfo*         pSpmiInfo       = NULL; 

    spmiDecodeVSid( vSlaveId, &busId, &slaveId );
    
    if((rslt = SpmiOsNhlos_GetDalProp( SPMI_DEVICES_PROP, &dalProp )) == SPMI_SUCCESS) 
    {
       pSpmiInfo = (SpmiInfo *)dalProp.Val.pStruct;

       if(pSpmiInfo == NULL) 
       {
          return SPMI_FAILURE_NOT_INITIALIZED;
       }
    }
    else
    {
        return SPMI_FAILURE_NOT_INITIALIZED;
    }

    pDev = SPMI_PDEV( pSpmiInfo, busId );
    
    base_addr = pDev->baseAddrs;
    
    for (uint32 i = 0; i < len; i++)
    {
        /*Populate Address */
        SPMI_HWIO_OUT( HWIO_PMIC_ARB_PVCn_ADDRm_ADDR(base_addr, UIM_PVC_PORT, MGPI_MODEM_SPMI_WRITE_PORT), 
                       (slaveId << HWIO_PMIC_ARB_PVCn_ADDRm_SID_SHFT | 
                        (address + i) << HWIO_PMIC_ARB_PVCn_ADDRm_ADDRESS_SHFT) );
                        
        /*Populate Data */
        SPMI_HWIO_OUT( HWIO_PMIC_MGPI_TRIG_CONFIGn_ADDR( base_addr, MGPI_MODEM_SPMI_WRITE_PORT ), 
                       ( MGPI_POS_ADDR_IDX <<  HWIO_PMIC_MGPI_TRIG_CONFIGn_ADDR_IDX_POS_SHFT ) |
                       ( *(data + i) << HWIO_PMIC_MGPI_TRIG_CONFIGn_DATA_POS_SHFT) );
        
        /*Trigger Pos Write */
        SPMI_HWIO_OUT( HWIO_PMIC_MGPI_TRIG_STATUSn_ADDR( base_addr, MGPI_MODEM_SPMI_WRITE_PORT), TRIG_WRITE_POS);
        
        /*Wait for complete */
        while((status == 0) && (timeout-- > 0) && (pvcStatus != 0)) 
        {
            SpmiOs_Wait( 1 );
            status = SPMI_HWIO_IN( HWIO_PMIC_MGPI_TRIG_STATUSn_ADDR(base_addr, MGPI_MODEM_SPMI_WRITE_PORT) ); 
            status = status & HWIO_PMIC_MGPI_TRIG_STATUSn_SUCCESS_POS_BMSK;
            if(status != 0)
            {
                pvcStatus = SPMI_HWIO_IN(HWIO_PMIC_ARB_PVC_PORTn_STATUS_ADDR(base_addr, UIM_PVC_PORT) );
                
                pvcFailure = pvcStatus & HWIO_PMIC_ARB_PVC_PORTn_STATUS_PVC_PORT_FAILURE_BMSK; 
                
                if(pvcFailure != 0)
                {
                    return SPMI_FAILURE_TRANSACTION_FAILED;
                }
                
                pvcStatus = pvcStatus & HWIO_PMIC_ARB_PVC_PORTn_STATUS_PVC_PORT_BUSY_BMSK;
            }
        }
        
        if (timeout == 0)
        {
            return SPMI_FAILURE_TRANSACTION_TIMEOUT;
        }
        
        SpmiOs_Wait( 10 );
        status = 0;
        pvcStatus = HWIO_PMIC_ARB_PVC_PORTn_STATUS_PVC_PORT_BUSY_BMSK;
        timeout = SPMI_TIMEOUT_USEC;
    }
    
    return SPMI_SUCCESS;
}

//******************************************************************************
// Public API Functions
//******************************************************************************
Spmi_Result SpmiOs_Init(SpmiInfo **ppSpmiInfo)
{
    Spmi_Result rslt = SPMI_SUCCESS;
    DALSYSPropertyVar dalProp;
    DalDeviceHandle* handle;
    uint8** addr;
    uint8** dbgAddrs; 
    SpmiInfo *pSpmiInfo;
    SpmiTargetConfig *pTgt;
    uint32 i;
    DalDeviceHandle *hChipInfo = NULL;
    ChipInfoVersionType version = 0;
    ChipInfoFamilyType  chipFamily = CHIPINFO_FAMILY_UNKNOWN;

    *ppSpmiInfo = NULL;
    if((rslt = SpmiOsNhlos_GetDalProp( SPMI_DEVICES_PROP, &dalProp )) == SPMI_SUCCESS) {
       pSpmiInfo = (SpmiInfo *)dalProp.Val.pStruct;
    }
    else {
       return SPMI_FAILURE_INIT_FAILED;     
    }

    rslt = SpmiOsNhlos_GetDalProp( OWNER_PROP, &dalProp );
    if(rslt != SPMI_SUCCESS) {
       return SPMI_FAILURE_INIT_FAILED;     
    }
    pSpmiInfo->owner = dalProp.Val.dwVal;

    
    // Try to use the HWIO driver to get the base address
    if(DAL_HWIODeviceAttach( DALDEVICEID_HWIO, &handle ) == DAL_SUCCESS && handle != NULL)
    {
        if(DalDevice_Open( handle, DAL_OPEN_SHARED ) == DAL_SUCCESS) {
           for (i=0; i < pSpmiInfo->uNumDevices; i++)
           {  
              addr = &pSpmiInfo->devices[i].baseAddrs ; // just one bus for now.
              pTgt = (SpmiTargetConfig *)pSpmiInfo->devices[i].target;    
              if ( (DalHWIO_MapRegion( handle, pTgt->hwioAddrStr, (uint8**) addr )  != DAL_SUCCESS) ||
                   (NULL == (*addr))) {
                 return SPMI_FAILURE_INIT_FAILED;
              }
           }
           dbgAddrs = &pSpmiInfo->dbgInfo->dbgBaseAddrs;
           DalHWIO_MapRegion( handle, pSpmiInfo->dbgInfo->dbgBaseAddrsStr, (uint8**) dbgAddrs );
           DalDevice_Close( handle );
        } 
    }
    
    /* For Napali Version V1 detect chip and set use_debug flag */
    if( DAL_DeviceAttach(DALDEVICEID_CHIPINFO, &hChipInfo) == DAL_SUCCESS )
    {
       DalDevice_Open(hChipInfo, DAL_OPEN_SHARED);
       DalChipInfo_GetChipFamily(hChipInfo, &chipFamily);
       DalChipInfo_GetChipVersion(hChipInfo, &version);
       DalDevice_Close(hChipInfo);
       
       if(chipFamily == CHIPINFO_FAMILY_SDM845 && version <= CHIPINFO_VERSION(2, 0))
       {
         spmiUseHWChannel = TRUE;

         /*Enable POS configuration for MGPI GRFC 2 */
         SPMI_HWIO_OUT_FIELD ( HWIO_PMIC_MGPI_TRIG_CONFIG_EN2_ADDR(pSpmiInfo->devices[0].baseAddrs), 
                               HWIO_PMIC_MGPI_TRIG_CONFIG_EN2_MGPI_POS_EN, 0x1);
       }
    }
   
    *ppSpmiInfo = pSpmiInfo;
    
    
    return rslt;
}

boolean SpmiOs_HandleTransactionError(SpmiDevInfo *pDev, Spmi_Result* rslt, PmicArbCmd cmd, 
                                      uint8 slaveId, uint16 address, uint8 tries)

{
    return FALSE;
}

void SpmiOs_Wait(uint32 us)
{
    DALSYS_BusyWait( us );
}

Spmi_Result SpmiOs_Malloc(uint32 size, void** buf)
{
    if(DALSYS_Malloc( size, buf ) == DAL_SUCCESS) {
        DALSYS_memset( *buf, 0, size );
        return SPMI_SUCCESS;
    }    
    
    return SPMI_FAILURE_FUNCTIONALITY_NOT_SUPPORTED;
}

Spmi_Result SpmiOs_Free(void* buf)
{
    if(DALSYS_Free( buf ) == DAL_SUCCESS) {
        return SPMI_SUCCESS;
    }
    
    return SPMI_FAILURE_FUNCTIONALITY_NOT_SUPPORTED;
}

Spmi_Result SpmiOs_RegisterISR(SpmiDevInfo *pDev, SpmiOs_IsrPtr isr)
{
    static DalDeviceHandle* interruptDev = NULL;
    SpmiTargetConfig *cfg = (SpmiTargetConfig*)pDev->target;
    DALResult rslt;
    
    if(interruptDev == NULL)
    {
            
            if((rslt = DAL_DeviceAttach( DALDEVICEID_INTERRUPTCONTROLLER, &interruptDev )) != DAL_SUCCESS)
            {
                SPMI_LOG_ERROR( "Could not get handle to interrupt controller.  Result: %d", rslt );
                return SPMI_FAILURE_FUNCTIONALITY_NOT_SUPPORTED;
            }
        }
    
    if(interruptDev != NULL)
    {
        if((rslt = DalInterruptController_RegisterISR( interruptDev,
                                                       cfg->interruptVec,
                                                       isr,
                                                       (void*)(pDev),
                                                       DALINTRCTRL_ENABLE_LEVEL_HIGH_TRIGGER )) == DAL_SUCCESS) 
        {
            return SPMI_SUCCESS;            
        }
        else 
        {
            SPMI_LOG_ERROR( "Could not register for interrupt number %d.  Result: %d",
                            cfg->interruptVec,
                            rslt );
        }
    }
        
    return SPMI_FAILURE_FUNCTIONALITY_NOT_SUPPORTED;
}

uint64 SpmiOs_GetTimeTick() 
{
    uint64 tick = 0;
    static boolean timeHdlInited = FALSE;
    static DalDeviceHandle* timeDev = NULL;
    
    if(!timeHdlInited) 
    {
        if(DalTimetick_Attach( "SystemTimer", &timeDev ) != DAL_SUCCESS) {
            timeDev = NULL;
        }
        timeHdlInited = TRUE;
    }
    
    if(timeDev != NULL) {
        DalTimetick_GetTimetick64( timeDev, &tick );
    }
    
    return tick;
}

SpmiOs_ClkStatus SpmiOs_GetSerialClkState(SpmiDevInfo *pDev)
{
    return SPMI_CLK_STATUS_NOT_SUPPORTED;
}

SpmiOs_ClkStatus SpmiOs_GetAhbClkState(SpmiDevInfo *pDev)
{
    return SPMI_CLK_STATUS_NOT_SUPPORTED;
}

DALResult Spmi_ConfigureFetcherChannel(
    uint32 ClientHandle,
    DalDeviceHandle *h,
    uint16 periph,
    DALBOOL enableInt,
    uint32 *pChannel)
{
   return SPMI_CLK_STATUS_NOT_SUPPORTED;
}


void SpmiOs_ClkStatusInit(void)
{
    // Allow attachment to DAL Clock Driver now that it has initialized.
    // clkHdAllowedToInit = TRUE;
}

boolean SpmiOs_UseHwInterface(PmicArbCmd cmd)
{
    boolean rslt = FALSE;
    rslt = (cmd == PMIC_ARB_CMD_EXTENDED_REG_WRITE_LONG) ? spmiUseHWChannel : FALSE;
    return rslt; 
}

Spmi_Result SpmiOs_ExecuteCommand(PmicArbCmd cmd, Spmi_AccessPriority priority, uint8 vSlaveId, 
                                  uint16 address, uint8* data, uint32 dataLen, uint32* bytesTransacted)
{
   Spmi_Result rslt = SPMI_SUCCESS;

   if(cmd == PMIC_ARB_CMD_EXTENDED_REG_WRITE_LONG)
   {
      rslt =  SpmiOs_WriteLongMgpi ( vSlaveId, priority, address, data, dataLen );
   }
   
   return rslt;
}
