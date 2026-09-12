

#ifndef __MCPM_STATS_H__
#define __MCPM_STATS_H__

/*=========================================================================

           M O D E M   C L O C K   A N D   P O W E R   M A N A G E R 
               
                        H E A D E R   F I L E


  GENERAL DESCRIPTION
    This file contains the definitions for MCPM Stats feature.

  
  EXTERNALIZED FUNCTIONS
    MCPM_Stats_Start_Collection
    MCPM_Stats_End_Collection
    MCPM_Stats_Get_Overall_Active_Time
  
        Copyright (c) 2021 by QUALCOMM Technologies, Inc.  All Rights Reserved.


==========================================================================*/

/*==========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
 
$Header: //components/rel/mpower.mpss/13.1/api/mcpm_stats.h#7 $

when       who     what, where, why 
--------   ---     --------------------------------------------------------- 
04/08/15   ed      Initial version


==========================================================================*/ 

#include <mcpm_api.h>
/*==========================================================================
               FUNCTION DEFINITIONS FOR MODULE
==========================================================================*/

/* =========================================================================
**  Function : MCPM_Stats_Get_Overall_Active_Time
** =========================================================================*/
/**
  Begin collection of statistics across all techs.

  @return
    None

  @dependencies
  TBD.

*/
void MCPM_Stats_Start_Collection(void);


/* =========================================================================
**  Function : MCPM_Stats_Get_Overall_Active_Time
** =========================================================================*/
/**
  End collection of statistics across all techs.

  @return
    None

  @dependencies
  TBD.

*/
void MCPM_Stats_End_Collection(void);


/* =========================================================================
**  Function : MCPM_Stats_Get_Overall_Active_Time
** =========================================================================*/
/**
  Get overall time from stats module. This will also reset statistics to zero.

  @return
  uint64 - Total time that any RX or TX has been active for any technology.
            Time is measured in qticks (19.2MHz freq)
           If Counting is not currently enabled (MCPM_Stats_Start_Collection()
            has not been called) then will return 0.

  @dependencies
  TBD.

*/
uint64 MCPM_Stats_Get_Overall_Active_Time(void);

/*
 * struct statsDataType is shared with QMI when below function is called
 * MCPM_Stats_Get_Detailed_Active_Time
 */
typedef struct
{
  uint32 modemTechActiveTime[MCPM_NUM_RAT_TECH];// Active time in msec since last query
  uint32 wlanActiveTime;// Active time in msec since last query
  uint32 gpsActiveTime; // Active time in msec since last query
  boolean modemActiveVoteStaus;//TRUE if any required type NPA client votes present
  boolean wlanActiveVoteStatus;//TRUE if any required type NPA client votes present
  boolean gpsActiveVoteStatus; //TRUE if any required type NPA client votes present
}statsDataType;

/* =========================================================================
**  Function : MCPM_Stats_Get_Detailed_Active_Time
** =========================================================================*/
/**
  Returns MPSS (modem,wlan,gps) power collapse stats since last query

  @return
  pointer to data of type statsDataType

  @dependencies
  TBD.

*/
statsDataType * MCPM_Stats_Get_Detailed_Active_Time(void);

#endif

