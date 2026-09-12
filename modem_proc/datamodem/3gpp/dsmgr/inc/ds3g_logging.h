#ifndef DS3G_LOGGING_H
#define DS3G_LOGGING_H
/*===========================================================================

                      DS_EPC_LOGGING. H
                   
DESCRIPTION
  

EXTERNALIZED FUNCTIONS

 Copyright (c) 2009-2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: 
  $Author: 

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/07/14    sd    Changes to update EPC diag event EVENT_DS_EPC_PDN
                  to accommodate iWLAN_S2B.
04/14/09    sc    Created Module
===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "log_dmss.h"
#include "log.h"
#include "log_codes.h"
#include "ds3geventmgr.h"

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/

#ifdef TEST_FRAMEWORK
#error code not present
#endif

/***************************
EVENT_DS3G_DDS_SWITCH
****************************/
typedef enum
{
  DS3G_DDS_SWITCH_NONE =                0,
  DS3G_DDS_SWITCH_REQ_RECEIVED =        1,
  DS3G_DDS_SWITCH_ALLOWED =             2,
  DS3G_DDS_SWITCH_NOT_ALLOWED =         3,
  DS3G_DDS_SWITCH_APN_CLEANUP_REQ =     4,
  DS3G_DDS_SWITCH_APN_CLEANUP_CNF =     5,  
  DS3G_DDS_SWITCH_SUCCEED =             6,
  DS3G_DDS_SWITCH_FAILED =              7,
  DS3G_DDS_SWITCH_STATUS_MAX 
}ds3g_dds_switch_status_enum_type;

typedef PACK(struct)
{
  uint8   new_subs_id:4;
  uint8   status:4;
}ds3g_dds_switch_status_event_type; 

#ifdef FEATURE_DATA_TSRC
/***************************
EVENT_DS3G_TSRC_CHG
****************************/
typedef PACK(struct)
{
  /* Subscription ID that the event applies to - Default data sub */
  uint8                            subs_id:4;
  /* Reserved for future use */
  uint8                            reserved:4;
  /* Time amount to use to alter the timestamp by, in msec */
  uint32                           stretch_val;
  /* RAT bit mask representing which RATs are enabled and
     can be used for stretch value calculation */
  uint16                           stretch_rats_mask;
  /* Unitless stretch tatio used by IPA for timestamp rewriting calculation */
  uint32                           stretch_ratio;
}ds3g_tsrc_status_event_type; 
#endif /* FEATURE_DATA_TSRC */

/***************************
EVENT_DS3G_DATA_CALL_TRM_CONTROL
****************************/

typedef enum
{
  DS3G_DATA_CALL_EV_UNSPECIFIED  =  0,
  DS3G_DATA_CALL_ORIG_EV =       1,
  DS3G_DATA_CALL_ACTIVE_EV =     2,
  DS3G_DATA_CALL_DORMANT_EV =    3,
  DS3G_DATA_CALL_END_EV =        4,
  DS3G_DATA_CALL_NO_FLOW_EV =    5,
  DS3G_DATA__RS_RA_EV =          6,
  DS3G_DATA_CALL_MAX
}ds3g_data_call_activity_enum_type;

typedef PACK(struct)
{
  uint8  subs_id;
  ds3g_data_call_activity_enum_type event;
  uint8 proc_type_bitmask;
}ds3g_data_call_trm_control_event_type; 


#define DS_POWER_STATS_LOGGING_VERSION 1

typedef struct
{
  uint32  elapsed_time;
  uint32  sleep_time;
  uint32  tx_rx_time;
  uint32  tx_time;
  uint32  rx_time;
  uint32  idle_time;
}ds_power_stats_type; 

/*===========================================================================
  Log Code: LOG_DS_POWER_STATS_INFO_C

  Revision: 1

  Description: @external
               This log packet captures power stats infomation
			   
  Frequency: every 500-1000 ms 

  Size: 
===========================================================================*/
LOG_RECORD_DEFINE(LOG_DS_POWER_STATS_INFO_C)
  uint8                               version;               
  //Log packet version. Range: 0...255.
  uint8                               sub_id;	             
  //Subscriber id: 1 is for SUB1, 2 is for SUB2
  ds_power_stats_type                 power_stats;                 
  //Power Stats Info 
LOG_RECORD_END
/*===========================================================================

                        EXTERNAL FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION       DS3G_LOGGING_DDS_SWITCH_EVENT

DESCRIPTION    LOGGING DDS SWITCH EVENTS

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/

void ds3g_logging_dds_switch_event
(
  uint8 subs_id,
  ds3g_dds_switch_status_enum_type status
);
/*===========================================================================
FUNCTION       DS3G_LOGGING_DATA_CALL_TRM_CONTROL_EVENT

DESCRIPTION    LOGGING DATA CALL RELATIVE EVENTS

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/

void ds3g_logging_data_call_trm_control_event
(
  uint8 subs_id,
  ds3geventmgr_event_type event_id,
  uint8 proc_type_bitmask
);
/*=========================================================================== 
FUNCTION	  DS3G_POWER_STATS_LOGGING 
  
DESCRIPTION   this function log the power stats information 
  
DEPENDENCIES  None 
   
RETURN VALUE  None 
  
SIDE EFFECTS  None 
===========================================================================*/ 
void ds3g_power_stats_logging 
( 
  sys_modem_as_id_e_type	subs_id,
  uint32 elapsed_time,
  uint32 sleep_time,
  uint32 tx_rx_time,
  uint32 tx_time,
  uint32 rx_time,
  uint32 idle_time
);

#ifdef FEATURE_DATA_TSRC
/*===========================================================================
FUNCTION       DS3G_LOGGING_TSRC_CHG_EVENT

DESCRIPTION    This function generates Event Logging for when Timestamp 
               Rewriting Controller changes one of its values. 

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds3g_logging_tsrc_chg_event
(
  uint8 subs_id
);
#endif /* FEATURE_DATA_TSRC */

#endif/* DS3G_LOGGING_H */