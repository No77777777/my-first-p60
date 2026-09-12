/*==============================================================================

FILE:      ABT_dal.c

DESCRIPTION: This file implements DAL platform specific ABT driver.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

//#CHANGE - Update when put in the depot
$Header: //components/rel/core.mpss/10.0/systemdrivers/icb/src/common/ABT_dal.c#1 $ 
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    -----------------------------------------------------------
2014/12/22  tb     Added optional disable for clocks
2014/03/14  ddk    Added support for additional clock voting.
2014/03/10  ddk    Added support for hwio mapping.
2013/08/30  pm     Split out interrupt reg from ABT_init
2013/04/16  pm     Added interrupt priority and split out interrupt registration
2012/05/31  av     Created
 
        Copyright (c) 2012-2014 QUALCOMM Technologies, Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
==============================================================================*/

#include "ABTimeout.h"
#include "ABT_internal.h"
#include "ABT_platform.h"
#include "DALSys.h"
#include "DALStdErr.h"
#include "DALDeviceId.h"
#include "DDIInterruptController.h"
#include "DDIClock.h"
#include "err.h"
#include "DDIHWIO.h"


/*============================================================================
                          DEFINEs/MACROs
============================================================================*/

static DalDeviceHandle *pClock_Handle = NULL;
static DalDeviceHandle *hwio_handle = NULL;


/*============================================================================
                          TYPE DEFINITION
============================================================================*/

/*============================================================================
                                 FUNCTIONS
============================================================================*/

//*============================================================================*/
/**
@brief 
      Performs platform specific initialization for AHB Timeout driver.
      It enables clock/interrupts required for AHB timeout slave.
 
@param[in]  *PropData Pointer to property data structure.

@return    
      ABT_error_type indicating success or error.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
ABT_error_type ABT_Platform_Init( ABT_propdata_type * PropData )
{
  ClockIdType nId;
  uint32_t slave_id,clock_num;

  // Enable Slave Clocks
  if(DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_CLOCK, &pClock_Handle))
  {
      ERR_FATAL("ABTimeout: Device attach to Clock failed.", 0, 0, 0);
      return ABT_ERROR_INIT_FAILURE;
  }

  for(slave_id = 0; slave_id < PropData->len; slave_id++)
  {     
    for(clock_num = 0; 
          clock_num < PropData->slave_info[slave_id].num_of_clk ; 
          clock_num++)
    {   
      if(DAL_SUCCESS != 
            DalClock_GetClockId(pClock_Handle, 
                             PropData->slave_info[slave_id].clk_name_array[clock_num],
                             &nId))
      {
        ERR_FATAL("ABTimeout: Could not get ClockId.", 0, 0, 0);
        return ABT_ERROR_INIT_FAILURE;
      }
  
      if(DAL_SUCCESS != DalClock_EnableClock(pClock_Handle, nId))
      {
        ERR_FATAL("ABTimeout: Could not enable clock.", 0, 0, 0);
        return ABT_ERROR_INIT_FAILURE;
      }
    }
  }

  return ABT_SUCCESS;
}

//*============================================================================*/
/**
@brief 
      Performs platform specific de-initialization for AHB Timeout driver.
      It disables clocks that cannot be left on.
 
@param[in]  *PropData Pointer to property data structure.

@return    
      ABT_error_type indicating success or error.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
ABT_error_type ABT_Platform_Deinit( ABT_propdata_type * PropData )
{
  ClockIdType nId;
  uint32_t slave_id,clock_num;

  // Enable Slave Clocks

  for(slave_id = 0; slave_id < PropData->len; slave_id++)
  {     
    for(clock_num = 0; 
          clock_num < PropData->slave_info[slave_id].num_of_clk ; 
          clock_num++)
    {   
      if(PropData->slave_info[slave_id].clk_disable_array[clock_num])
      {
        if(DAL_SUCCESS != 
              DalClock_GetClockId(pClock_Handle, 
                               PropData->slave_info[slave_id].clk_name_array[clock_num],
                               &nId))
        {
          ERR_FATAL("ABTimeout: Could not get ClockId.", 0, 0, 0);
          return ABT_ERROR_INIT_FAILURE;
        }
  
        if(DAL_SUCCESS != DalClock_DisableClock(pClock_Handle, nId))
        {
          ERR_FATAL("ABTimeout: Could not disable clock.", 0, 0, 0);
          return ABT_ERROR_INIT_FAILURE;
        }
      }
    }
  }
  
  if(DAL_SUCCESS != DAL_DeviceDetach( pClock_Handle))
  {
    ERR_FATAL("ABTimeout: Device deattach to Clock failed.", 0, 0, 0);
    return ABT_ERROR_INIT_FAILURE;
  }
  

  return ABT_SUCCESS;
}

//*============================================================================*/
/**
@brief 
      Performs platform specific initialization for AHB Timeout driver.
      It enables clock/interrupts required for AHB timeout slave.
 
@param[in]  *PropData Pointer to property data structure.

@return    
      ABT_error_type indicating success or error.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
ABT_error_type ABT_Platform_Register_Interrupt( ABT_propdata_type * PropData )
{
  // Register IRQs
  return ABT_Platform_Register_Interrupt_OS(
     PropData->platform_info->intr_vector, 
     ABT_Handle_Interrupt,
     NULL, 
     DALINTRCTRL_ENABLE_CONFIG(DALINTRCTRL_ENABLE_LEVEL_HIGH_TRIGGER, 
                               PropData->platform_info->intr_priority)
     );
}

//*============================================================================*/
/**
@brief 
      Logs syndrome register infomration using platform specific APIs.
 
@param[in]  *syndrome Pointer to syndrome info structure

@return    
      ABT_error_type indicating success or error.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
ABT_error_type ABT_Platform_Log_Syndrome(ABT_syndrome_info_type *syndrome)
{
    ABT_ERR("ABTimeout: Bus %s hang; Syndrome ID: 0x%x", 
                  syndrome->slave_name, syndrome->id, 0);
    ABT_ERR("ABTimeout: Bus %s hang; Syndrome ADDR0: 0x%x", 
                  syndrome->slave_name, syndrome->addr0, 0);
    ABT_ERR("ABTimeout: Bus %s hang; Syndrome ADDR1: 0x%x", 
                  syndrome->slave_name, syndrome->addr1, 0);
    ABT_ERR("ABTimeout: Bus %s hang; Syndrome HREADY: 0x%x", 
                  syndrome->slave_name, syndrome->hready, 0);
    ABT_ERR("ABTimeout: Bus %s hang; Number of Slaves: %d", 
                  syndrome->slave_name, syndrome->num_slaves, 0);
    return ABT_SUCCESS;
}

//*============================================================================*/
/**
@brief 
      Logs syndrome register infomration using platform specific APIs.
 
@param[in]  hung_slave Name of hung slave.

@return    
      ABT_error_type indicating success or error.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
ABT_error_type ABT_Platform_Report_Hang(char *hung_slave)
{
      ERR_FATAL("ABTimeout: AHB Bus hang is detected, Slave: %s !!!",
                 hung_slave, 0, 0);
      return ABT_SUCCESS;
}

//*============================================================================*/
/**
@brief 
      Gets platfrom specific property data structure for configuration.
 
@param[in]  None.

@return    
      ABT_propdata_type* Pointer to property data structure.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
ABT_propdata_type* ABT_Platform_Get_Propdata(void)
{
   DALSYSPropertyVar ABTPropVar;
   DALSYS_PROPERTY_HANDLE_DECLARE(hABTProp);

   /* Get default configuration from config data */
   if (DAL_SUCCESS != DALSYS_GetDALPropertyHandleStr( "/dev/ABTimeout",
                                                      hABTProp ))
   {
     // Can't get property handle
     ABT_ERR("ABTimeout: Can't get DAL property handle.", 0, 0, 0);
     return NULL;
   }

   if ((DAL_SUCCESS == DALSYS_GetPropertyValue(hABTProp,
                                              "ABT_Propdata", 0, &ABTPropVar)))
   {
       if((0 >= ABTPropVar.dwLen))
       {
           ABT_ERR("ABTimeout: Invalid property data length", 0, 0, 0);
           return NULL;
       }
       return (ABT_propdata_type*) ABTPropVar.Val.pStruct;
   }

   ABT_ERR("ABTimeout: Can't get DAL property value", 0, 0, 0);
   return NULL;
}


//*============================================================================*/
/**
@brief 
      Set platfrom specific HWIO mapping.
 
@param[in]  *PropData Pointer to property data structure.

@return    
      ABT_error_type indicating success or error.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/

ABT_error_type ABT_Platform_Set_HWIO_Region( ABT_propdata_type * PropData )
{
  uint8_t           *pBasePtr = NULL;
  const char    *pAddrName = NULL;
  uint32_t i;

  if ((DAL_DeviceAttach(DALDEVICEID_HWIO, &hwio_handle) == DAL_SUCCESS)
        && (hwio_handle != NULL))
  {
    for(i = 0; i < PropData->platform_info->num_hwio_regions; i++)
    {     
      pAddrName = PropData->platform_info->hwio_map_info[i].name;
      pBasePtr = PropData->platform_info->hwio_map_info[i].base_addr;
        
      if ( (DalHWIO_MapRegion(hwio_handle,pAddrName,&pBasePtr) != DAL_SUCCESS) || 
               (pBasePtr == NULL) )
      {
        ERR_FATAL("ABT: Failed to map HWIO region", 0, 0, 0);
      }
    }
  }
  else
  {
    ERR_FATAL("ABT: Failed to attach HWIO DalDevice.",0, 0, 0);
  }  
  return ABT_SUCCESS;
}

//*============================================================================*/
/**
@brief 
      Set platfrom specific HWIO un mapping.
 
@param[in]  *PropData Pointer to property data structure.

@return    
      ABT_error_type indicating success or error.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/

ABT_error_type ABT_Platform_UnSet_HWIO_Region( ABT_propdata_type * PropData )
{
  uint8           *pBasePtr = NULL;
  uint32 i;

  for(i = 0; i < PropData->platform_info->num_hwio_regions; i++)
  {     
    pBasePtr = PropData->platform_info->hwio_map_info[i].base_addr;
      
    if ( (pBasePtr != NULL) && (DalHWIO_UnMapRegion(hwio_handle,pBasePtr) != DAL_SUCCESS))
    {
      ERR_FATAL("ABT: Failed to unmap HWIO region", 0, 0, 0);
    }
  }
  
  if ((DAL_DeviceDetach(hwio_handle) != DAL_SUCCESS))
  {
    ERR_FATAL("ABT: Failed to de-attach HWIO DalDevice.",0, 0, 0);
  }
  return ABT_SUCCESS;
}

