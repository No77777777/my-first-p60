#ifndef _SLEEP_MASTER_STATS_H_
#define _SLEEP_MASTER_STATS_H_
/*==============================================================================
  FILE:         sleep_masterstats.h
  
  OVERVIEW:     Internal functions & types to gather and adjust
                CX collapse mode statistics
 
  DEPENDENCIES: None
                
                Copyright (c) 2017 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/sleep2.0/src/statistics/sleep_masterstats.h#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include "comdef.h"

/*==============================================================================
                           MACRO DEFINITIONS
 =============================================================================*/
/* Master stats version ID */ 
#define MASTERSTATS_VERSIONID 0x1

/*=======================================================================
 *                         TYPE DEFINITIONS
 *======================================================================*/
/*CX collapse mode statistics*/
typedef struct master_stats_t
{
	/* Version Id to maitain compatibility with APPS */
    uint32 VersionId; 
    /* Number of times DRV has voted voted for AOSS Sleep + CX Collapse */
    uint32 SysSleep_Count;
    /* LAST Timestamp the Master(DRV) voted for AOSS Sleep + CX Collapse */
    uint64 SysSleep_last_entered_at;
    /*  LAST Timestamp the Master(DRV) voted for CX at MO */
    uint64 SysSleep_last_exited_at;
    /* Shows how long the Master (DRV) has been in a sleep mode with CX collapse voted for since device booted up */
    uint64 SysSleep_accumulated_duration; 
} sleep_masterstats;

/*=======================================================================
 *                             GLOBAL FUNCTIONS
 *======================================================================*/
/**
 * sleepMasterstats_init
 *
 * @brief Initilizes the SMEM memory to store Cx collapse stats
 */
void sleepMasterstats_init(void);			
				   
/**
 * sleepMasterstats_updatePowerdownStats
 *
 * @brief Updating the CX collapse mode Entry timestamp in SMEM memory location
 * @param Start_timestamp :  CX collapse Mode Software Entry timestamp
 */
void sleepMasterstats_updatePowerdownStats(uint64 Start_timestamp);
					   
/**
 * sleepMasterstats_updateWakeupStats
 *
 * @brief Updating the CX collapse mode wakeup Stats in SMEM memory location
 * 
 * @param start_time :  CX collapse Mode Entry timestamp
 * @param end_time   :  CX collapse Mode Exit timestamp
 */
void sleepMasterstats_updateWakeupStats(uint64 start_time, uint64 end_time); 							   

#endif /*_SLEEP_MASTER_STATS_H_*/

