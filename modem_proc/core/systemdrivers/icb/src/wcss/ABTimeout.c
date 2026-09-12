/*==============================================================================

FILE:      AHBTimeout.c

DESCRIPTION: This file implements AHB Timeout driver.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

//#CHANGE - Update when put in the depot
$Header: //components/rel/core.mpss/10.0/systemdrivers/icb/src/wcss/ABTimeout.c#1 $ 
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2016/05/23  sds    KW errors
2016/02/11  pm     Branch for Adrastea (Modem User PD)
2014/12/22  tb     Added optional disable for clocks
2014/03/10  ddk    Added support for hwio mapping.
2013/11/14  tb     Added support for multiple enable/status registers 
2013/08/30  pm     Clear any pending interrupts to cover subsystem restart 
2013/05/26  pm     Fix clearing syndrome valid and interrupt status
2012/10/04  av     Support for disabling ABT 
2012/05/31  av     Created
 
        Copyright (c) 2012-2016 QUALCOMM Technologies, Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
==============================================================================*/

#include "ABTimeout.h"
#include "ABT_internal.h"
#include "ABT_platform.h"
#include "DALDeviceId.h"
#include "DDIClock.h"
#include <qurt.h>
#include "qurt_int.h"
#include "NOC_error.h"
#include "dll_global_def.h"
#include <string.h>

/*============================================================================
                                  DEFINES
============================================================================*/

#define MAX_NUM_INTR_REGS   3
#define MAX_NUM_WAKEUP_REGS 3

/**
 * ABT Platform Data type
 */
typedef struct
{
    uint8  hw_version;              /**< ABT_HW_VERSION offset */
    uint8  inst_id;                 /**< ABT_INST_ID offset */
    uint8  num_slaves;              /**< ABT_NUM_SLAVES offset */
    uint8  timer_loadval;           /**< ABT_TIMER_LOADVAL offset */
    uint8  timer_mode;              /**< ABT_MODE offset */
    uint8  intr_status;             /**< ABT_INTR_STATUS offset */
    uint8  intr_clear;              /**< ABT_INTR_CLEAR offset */
    uint8  intr_enable;             /**< ABT_INTR_ENABLE offset */
    uint8  synd_valid;              /**< ABT_SYND_VALID offset */
    uint8  synd_clear;              /**< ABT_SYND_CLEAR offset */
    uint8  synd_id;                 /**< ABT_SYND_ID offset */
    uint8  synd_addr0;              /**< ABT_SYND_ADDR0 offset */
    uint8  synd_addr1;              /**< ABT_SYND_ADDR1 offset */
    uint8  synd_hready;             /**< ABT_SYND_HREADY offset */   
}ABT_hw_type;

/*============================================================================
                                  MACROS
============================================================================*/
#define ABT_OUT32(addr, data)  (*((volatile uint32 *) (addr)) = ((uint32) (data)))
#define ABT_IN32(addr)  ((uint32)(*((volatile uint32 *) (addr))))

#define ABT_REG_ADDR(x,offset)   (((uint8*)SlaveInfo[(x)].base_addr) + (offset))


/*============================================================================
                                 Globals
============================================================================*/

static ABT_propdata_type* PropData;
static ABT_slave_info_type* SlaveInfo;

// Global structure with Syndrome register dump for ABT Timeout in WCSS PD
ABT_syndrome_info_type abt_syndrome_user = {0};

boolean abt_init_done = FALSE;
struct cnss_abt_clk_dal_device
{
   DalDeviceHandle *hClock;
   ClockIdType nClockID;
};

struct cnss_abt_clk_dal_device abt_clk_dal_h = {0};

// This is offset to wakeup abt interrupt disable registers from interrupt enable registers
const uint32 wakeup_abt_disable_intr_offset[] = {0x8, 0x8, 0x8};
// This is offset to abt interrupt disable registers from interrupt enable registers
const uint32 abt_disable_intr_offset[] = {0x4, 0x8, 0x8};

/*============================================================================
                            ABT HW DEFINITION 
============================================================================*/
 /*Define HW_VERSION_(MAJOR)_(MINOR)_(STEP) as ABT_HW_VERSION default reg value*/

 /*Define All supported HW versions supported by this driver.
   Initialize offsets for all valid registers, invalid register for a
   HW version should be initialized to ZERO */

/* HW Version 1.0.0 */
#define HW_VERSION_1_0_0                 0x10000000

/* ABT HW Version 1.0.0 Offsets */
ABT_hw_type abt_hw_1_0_0 = {
    0x00,              /**< ABT_HW_VERSION offset */
    0x04,              /**< ABT_INST_ID offset */
    0x08,              /**< ABT_NUM_SLAVES offset */
    0x0c,              /**< ABT_TIMER_LOADVAL offset */
    0x10,              /**< ABT_MODE offset */
    0x14,              /**< ABT_INTR_STATUS offset */
    0x18,              /**< ABT_INTR_CLEAR offset */
    0x1c,              /**< ABT_INTR_ENABLE offset */
    0x20,              /**< ABT_SYND_VALID offset */
    0x24,              /**< ABT_SYND_CLEAR offset */
    0x28,              /**< ABT_SYND_ID offset */
    0x2c,              /**< ABT_SYND_ADDR0 offset */
    0x30,              /**< ABT_SYND_ADDR1 offset */
    0x34,              /**< ABT_SYND_HREADY offset */   
};

/* Add future HW versions below...*/


//*============================================================================*/
/**
@brief 
      This function returns pointer to ABT_hw_type structure for specified
      Version ID.
 
@param[in]  Version ID.

@return    
      ABT_hw_type*: If version is supported, returns Pointer to ABT_hw_type
      NULL:         If version is not supported, returns NULL

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
static ABT_hw_type* ABT_Get_HW_type(uint32 version)
{
    switch(version)
    {
        case HW_VERSION_1_0_0:
            return &abt_hw_1_0_0;

        /* Add future HW versions below...*/

        default:
            break;
    }
    return NULL;
}

/*===========================================================================*/
/**
@brief 
      This function reads Interrupt status register and captures syndrome
      registers from corresponding ABT slaves.
 
@param[in]  void* arg Context data for client.

@return    
      None.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*==============================================================================*/
void* ABT_Handle_Interrupt(void* arg)
{
 uint8 i, index, slave;
 uint32 intr_status[MAX_NUM_INTR_REGS], synd_valid, hang_detected = 0;
 ABT_hw_type* hw;

 memset(intr_status, 0x0, sizeof(intr_status));

 for(i=0; i<PropData->len; i++)
 {   
     index = SlaveInfo[i].slave_id >> 5;
     slave = SlaveInfo[i].slave_id & 0x1F;

     // Read interrupt status
     intr_status[index] = ABT_IN32(PropData->platform_info->intr_status_addr[index]);
 
     if(intr_status[index] & (1 << slave))
     {
        if (!(SlaveInfo[i].is_noc))
        {
            // Get HW Version type of the Slave
            hw = ABT_Get_HW_type(ABT_IN32((SlaveInfo[i].base_addr)));

            if(hw == NULL)
            {
                // Invalid HW, we shouldn't get here.
                continue;
            }

            // Read syndrom registers
            synd_valid = ABT_IN32(ABT_REG_ADDR(i, hw->synd_valid));  
            if(synd_valid != 0)
            {
                abt_syndrome_user.slave_name = SlaveInfo[i].name;
                abt_syndrome_user.id = ABT_IN32(ABT_REG_ADDR(i, hw->synd_id));
                abt_syndrome_user.addr0 = ABT_IN32(ABT_REG_ADDR(i, hw->synd_addr0));
                abt_syndrome_user.addr1 = ABT_IN32(ABT_REG_ADDR(i, hw->synd_addr1));
                abt_syndrome_user.hready = ABT_IN32(ABT_REG_ADDR(i, hw->synd_hready));
                abt_syndrome_user.num_slaves = ABT_IN32(ABT_REG_ADDR(i, hw->num_slaves));

                // Log syndrome information
                ABT_Platform_Log_Syndrome(&abt_syndrome_user);

                // Log number of hangs detected
                hang_detected++;

                //Clear synd valid
                ABT_OUT32(ABT_REG_ADDR(i, hw->synd_clear), 0x1);

                //Clear Interrupt status
                ABT_OUT32(ABT_REG_ADDR(i, hw->intr_clear), 0x1);
				
				// Once a timeout is identified, dont check for other timeouts as the access to those registers might not be there.
				break;
            }
        }
        else
        {
           // Interrupt intended for NOC -- If NOC Error is set to ERR_FATAL 
           // it will do so in this function
           NOC_Error_Handle_Interrupt();
        }
     }
 }

 if(hang_detected > 0)
 {
     // Report bus-hang to err-services
     ABT_Platform_Report_Hang(abt_syndrome_user.slave_name);
 }
 return arg;
}

/*===========================================================================*/
/**
@brief 
      This function provides an external interface to the AHB Timeout
      interrupt handler.  It is called by other subsystems that detect an
      AHB Timeout (for example, in the process of handling summary interrupts
      that they own.)
 
@param[in]  void

@return    
      None.

@dependencies
      None.
 
@sideeffects 
      ERR_FATAL() is called if an AHB Timeout is found.
*/ 
/*==============================================================================*/
void ABT_Handle_Error(void)
{
 ABT_Handle_Interrupt(NULL);
}

//*============================================================================*/
/**
@brief 
      Initializes AHB config driver. It reads AHB configuration data (register 
      address and config data) from DAL Device Property file and configures 
      AHB registers.
 
@param[in]  None.

@return    
      None.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
void ABT_Init( void )
{
   uint8 i, index, slave, wakeup_index, wakeup_id, noc_initialized = FALSE;
   uint32 reg_data[MAX_NUM_INTR_REGS];
   uint32 wakeup_data[MAX_NUM_WAKEUP_REGS];
   ABT_hw_type* hw;

   if (abt_init_done)
   {
      ABT_DeInit();  // If ABT is already Initialized, DeInit first (Wifi ON-OFF case)
   }

   // Get device property data for configuration
   PropData = ABT_Platform_Get_Propdata();
   if(PropData == NULL)
   {
       return; // ABT_ERROR_INIT_FAILURE
   }

   // Set HWIO region
   if(ABT_SUCCESS != ABT_Platform_Set_HWIO_Region(PropData))
   {
       return; // ABT_ERROR_INIT_FAILURE
   }

   // Check Timeout Slave Global EN
   if(!(ABT_IN32(PropData->platform_info->globa_en_addr) & 0x1))
   {
       // ABT is disabled. Nothing to initialize.
       ABT_Platform_UnSet_HWIO_Region(PropData);
       return;
   }

   SlaveInfo = PropData->slave_info;

   // Perform Platform-specific Initialization
   if(ABT_SUCCESS != ABT_Platform_Init(PropData))
   {
       ABT_Platform_UnSet_HWIO_Region(PropData);
       return; // ABT_ERROR_INIT_FAILURE
   }

   // Enable Interrupts & set timeout values
   memset(reg_data, 0, sizeof(reg_data));
   memset(wakeup_data, 0, sizeof(wakeup_data));
   for(i=0; i < PropData->len; i++)
   {
       // Div 32 to get which register this bit is in
       wakeup_index = SlaveInfo[i].wakeup_id >> 5;
       // If the index is out of range, it means there is no wakeup 
       // needed for this ABT interrupt.
       if (wakeup_index < MAX_NUM_WAKEUP_REGS) 
       {
          // Mod 32 to get the number of bits to shift within the 
          // selected register
          wakeup_id    = SlaveInfo[i].wakeup_id & 0x1F;
          // Generate bit-mask for wakeup mappings
          wakeup_data[wakeup_index] |= (SlaveInfo[i].intr_enable << wakeup_id);
       }

       // Div 32 to get which register this bit is in
       index = SlaveInfo[i].slave_id >> 5;
       // Mod 32 to get the number of bits to shift within the 
       // selected register
       slave = SlaveInfo[i].slave_id & 0x1F;
       // Generate bit-mask for interrupt mappings
       reg_data[index] |= (SlaveInfo[i].intr_enable << slave);

       if (SlaveInfo[i].is_noc) 
       {
          if ((!noc_initialized) && (SlaveInfo[i].intr_enable)) 
          {
             // Initialize NOC Error Driver
             NOC_Error_Init();
             noc_initialized = TRUE;
          }

          // No ABT configuration to be done for NOC
          continue; 
       }

       // Get HW Version type of the Slave
       hw = ABT_Get_HW_type(ABT_IN32((SlaveInfo[i].base_addr)));

       if(hw == NULL)
       {
           // Invalid HW, we shouldn't get here.
           continue;
       }

       // Clear any pending interrupts (covering the subsystem restart case)
       // Clear synd valid
       ABT_OUT32(ABT_REG_ADDR(i, hw->synd_clear), 0x1);
       // Clear Interrupt status
       ABT_OUT32(ABT_REG_ADDR(i, hw->intr_clear), 0x1);

       // Program inst_id 
       ABT_OUT32(ABT_REG_ADDR(i, hw->inst_id), i);

       //Set timer mode to load.
       ABT_OUT32(ABT_REG_ADDR(i, hw->timer_mode), 0x00);

       if(SlaveInfo[i].slave_enable == TRUE && SlaveInfo[i].timeout_val != 0)
       {
           //Set timeout value
           ABT_OUT32((ABT_REG_ADDR(i, hw->timer_loadval)), SlaveInfo[i].timeout_val);

           //Set timer mode to run mode
           ABT_OUT32(ABT_REG_ADDR(i, hw->timer_mode), 0x01);
       }

       // Enable Interrupt
       if(SlaveInfo[i].intr_enable)
       {
           ABT_OUT32(ABT_REG_ADDR(i, hw->intr_enable), 0x01);
       }
   }
   
   // Configure Modem wakeup registers
   for(i=0;i<PropData->platform_info->num_wakeup_reg && i<MAX_NUM_WAKEUP_REGS;i++)
   {
       ABT_OUT32(PropData->platform_info->wakeup_addr[i],
                 ABT_IN32(PropData->platform_info->wakeup_addr[i]) | wakeup_data[i]);     
   }

   // Configure Interrupt Mapping
   for(i=0;i<PropData->platform_info->num_status_reg && i<MAX_NUM_INTR_REGS;i++)
   {
       ABT_OUT32(PropData->platform_info->intr_map_addr[i],
                 ABT_IN32(PropData->platform_info->intr_map_addr[i]) | reg_data[i]);     
   }

   // Register ISR
   if(ABT_SUCCESS != ABT_Platform_Register_Interrupt(PropData))
   {
       ABT_Platform_UnSet_HWIO_Region(PropData);
       return; // ABT_ERROR_INIT_FAILURE
   }

   // Perform Platform-specific De-initialization
   if(ABT_SUCCESS != ABT_Platform_Deinit(PropData))
   {
       ABT_Platform_UnSet_HWIO_Region(PropData);
       return; // ABT_ERROR_INIT_FAILURE
   }

   if(DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_CLOCK, &(abt_clk_dal_h.hClock)))
   {
      ABT_Platform_UnSet_HWIO_Region(PropData);
      return;
   }

   if (DAL_SUCCESS != DalClock_GetClockId(abt_clk_dal_h.hClock, "gcc_wcss_ecahb_clk", &(abt_clk_dal_h.nClockID)))
   {
      DAL_DeviceDetach(abt_clk_dal_h.hClock);
      ABT_Platform_UnSet_HWIO_Region(PropData);
      return;
   }

   // Flag to notify ABT Init is completed
   abt_init_done = TRUE;

   return; // ABT_SUCCESS
}

//*============================================================================*/
/**
@brief 
      De-Initializes AHB config driver.
 
@param[in]  None.

@return    
      None.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
void ABT_DeInit( void )
{
   uint8 i, index, slave, wakeup_index, wakeup_id, noc_deinitialized = FALSE;
   ABT_hw_type* hw;
   uint32 reg_data[MAX_NUM_INTR_REGS] = {0};
   uint32 wakeup_data[MAX_NUM_WAKEUP_REGS] = {0};
   if (abt_init_done)
   {
      // PropData will be initialized by ABT_Init before ABT_DeInit.
	  // Enables required clocks
      if(ABT_SUCCESS != ABT_Platform_Init(PropData))
      {
          return; // ABT_ERROR_DEINIT_FAILURE
      }

      for(i=0; i < PropData->len; i++)
      {
         wakeup_index = SlaveInfo[i].wakeup_id >> 5;
         if (wakeup_index < MAX_NUM_WAKEUP_REGS) 
         {
            wakeup_id = SlaveInfo[i].wakeup_id & 0x1F;
            wakeup_data[wakeup_index] |= (SlaveInfo[i].intr_enable << wakeup_id);
         }

         index = SlaveInfo[i].slave_id >> 5;
         slave = SlaveInfo[i].slave_id & 0x1F;
         reg_data[index] |= (SlaveInfo[i].intr_enable << slave);

		 if (SlaveInfo[i].is_noc) 
         {
            if ((!noc_deinitialized) && (SlaveInfo[i].intr_enable)) 
            {
               // De-Initialize NOC Error Driver
               NOC_Error_DeInit();
               noc_deinitialized = TRUE;
            }
         
            // No ABT De-configuration to be done for NOC
            continue; 
         }

	     hw = ABT_Get_HW_type(ABT_IN32((SlaveInfo[i].base_addr)));
         if(hw == NULL)
         {
             // Invalid HW, we shouldn't get here.
             continue;
         }
         // Clear synd valid
         ABT_OUT32(ABT_REG_ADDR(i, hw->synd_clear), 0x1);
         // Clear Interrupt status
         ABT_OUT32(ABT_REG_ADDR(i, hw->intr_clear), 0x1);
		 // Disable Interrupt
		 ABT_OUT32(ABT_REG_ADDR(i, hw->intr_enable), 0x0);
		 // Clear timer mode
		 ABT_OUT32(ABT_REG_ADDR(i, hw->timer_mode), 0x00);
		 // Reset timeout value
		 ABT_OUT32((ABT_REG_ADDR(i, hw->timer_loadval)), 0x00);
	  }

      // Disable Modem wakeup ABT interrupt
      for(i=0;i<PropData->platform_info->num_wakeup_reg && i<MAX_NUM_WAKEUP_REGS;i++)
      {
          ABT_OUT32(((uint32)PropData->platform_info->wakeup_addr[i] + wakeup_abt_disable_intr_offset[i]), wakeup_data[i]);     
      }

      // Disable ABT Interrupt
      for(i=0;i<PropData->platform_info->num_status_reg && i<MAX_NUM_INTR_REGS;i++)
      {
          ABT_OUT32(((uint32)PropData->platform_info->intr_map_addr[i] + abt_disable_intr_offset[i]), reg_data[i]);     
      }

      qurt_interrupt_deregister(PropData->platform_info->intr_vector);
	  DAL_DeviceDetach(abt_clk_dal_h.hClock);

	  memset(&abt_clk_dal_h, 0, sizeof(abt_clk_dal_h));
      memset(&abt_syndrome_user, 0, sizeof(abt_syndrome_user));

      // Disables the clocks
      if(ABT_SUCCESS != ABT_Platform_Deinit(PropData))
      {
          return; // ABT_ERROR_DEINIT_FAILURE
      }
      ABT_Platform_UnSet_HWIO_Region( PropData );
      SlaveInfo = NULL;
      PropData = NULL;
      abt_init_done = FALSE;
   }
}

/*===========================================================================*/
/**
@brief 
      This function is handling shadow register interrupt which is routed 
      outside of wcss for captures and logs syndrome registers from
      corresponding ABT slaves.
 
@param[in] uint32 intr_vector - Interrupt vector ID.

@return    
      ABT_ERROR_INIT_FAILURE - Unable to process this ABT interrupt
      ABT_SUCCESS - Success

@dependencies
      ABT_Init need to be done
	  ECAHB clock should be ON
 
@sideeffects 
      ERR_FATAL() is called if an AHB Timeout is found.
*/ 
/*==============================================================================*/
DLL_API_GLOBAL ABT_error_type ABT_Handle_External_Interrupt(uint32 intr_vector )
{
	// ABT_Init should be done and ECAHB clock should be ON
   if(abt_init_done == TRUE)
   {
      if (DalClock_IsClockOn(abt_clk_dal_h.hClock, abt_clk_dal_h.nClockID) == TRUE)
      {
         if ( (uint32)ABT_Handle_Interrupt((void*)intr_vector) == intr_vector )
         {
            return ABT_SUCCESS;
         }
         else
         {
            return ABT_ERROR_INIT_FAILURE;
         }
      }
      else
      {
         return ABT_ERROR_CLOCK_NOT_ENABLED;
      }
   }
   else
   {
      return ABT_ERROR_INIT_NOT_DONE;
   }
}
