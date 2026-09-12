/*=========================================================================*//**
    @file  debug_logging.h

    @brief Declares of debug logging infrastructures
*//****************************************************************************/
/*------------------------------------------------------------------------------
    Copyright (c) 2013 Qualcomm Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Confidential and Proprietary
------------------------------------------------------------------------------*/

#ifndef DEBUG_LOGGING_H_
#define DEBUG_LOGGING_H_
#include "global_defs.h"
#include "ipa_hw_interface.h"
#include "ipa_hw_interface_i.h"
#include "services.h"


#define IPA_HW_DBG_EVT_PAYLOAD_SIZE_BYTES  (sizeof(IpaHwEventLogInfoData_t))
#define IPA_HW_STATS_INFO_SIZE_BYTES       (sizeof(IpaHwStatsInfoData_t))

extern uint32_t ipa_power_collapse_marker;  /**< Used to indicate if a cold or warm boot has occured. */

/**
 * @brief   Wrapper function to send uC event to CPU.
 *
*/
void DEBUG_SendDbgEvent(void);

/**
 * @brief   Initialize debug module and send event to CPU.
 *
*/
//PRAGMA_PLACE_CODE_IN_DRAM
inline void DEBUG_LogInit(void)
{
   uint32_t statsBaseAddr;


   /* Clear out the location for the payload of the Debug event and the location of statistics structure */
   MemClear(IPA_HW_SRAM_DEBUG_EVENT_ADDR, IPA_HW_DBG_EVT_PAYLOAD_SIZE_BYTES + IPA_HW_STATS_INFO_SIZE_BYTES);

   /*Update the gDbgEvtLog structure and send and event to CPU */
   gDbgEvtLog.featureMask =  (
                                 IPA_HW_FEATURE_BMASK_COMMON
                             );

   /*--------------------------------------------------------------------------
                Populate Statistics related information
   --------------------------------------------------------------------------*/
   statsBaseAddr  = (uint32_t)&gIpaStats;

   /* Compute the base address offset location */
   //gDbgEvtLog.statsInfo.baseAddrOffset = IPA_UC_DRAM_OFFSET + (statsBaseAddr - IPA_UC_DRAM_START_ADDR);
   gDbgEvtLog.statsInfo.baseAddrOffset = statsBaseAddr - IPA_WRAPPER_BASE;

   /* Fill up the Common Stats info in featureInfo array */
   gDbgEvtLog.statsInfo.featureInfo[IPA_HW_FEATURE_COMMON].params.offset = (((uint32_t)&gIpaStats.commonStats) - statsBaseAddr);
   gDbgEvtLog.statsInfo.featureInfo[IPA_HW_FEATURE_COMMON].params.size   = sizeof(IpaHwStatsCommonInfoData_t);
   /*--------------------------------------------------------------------------
               ToDo: Populate circular buffer (ulog) information
   --------------------------------------------------------------------------*/

   /*--------------------------------------------------------------------------
                          Send uC event to CPU on cold boot
   -------------------------------------------------------------------------- */
   if(ipa_power_collapse_marker == 0xDEADDEAD) // Only send on cold boot
   {
       DEBUG_SendDbgEvent(); 
   }


}


#endif /* DEBUG_LOGGING_H_ */
