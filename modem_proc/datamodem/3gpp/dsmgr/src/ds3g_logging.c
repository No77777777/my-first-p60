/*===========================================================================
                     DS3G_ LOGGING . C
                   
DESCRIPTION - DIAG EVENTS AND LOG PKTS 
  
 
EXTERNALIZED FUNCTIONS
  

 Copyright (c)2011-2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: 
  $Author: 

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
10/19/16    ZHANGYAN    Created Module
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/


#include "ds3g_logging.h"
#include "event.h"
#include "event_defs.h"
#include "data_msg.h"

#ifdef FEATURE_DATA_TSRC
#include "ds3g_tsrc_mgr.h"
#endif /* FEATURE_DATA_TSRC */
/*===========================================================================

                        EXTERNAL FUNCTION DEFINITION

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
)
{
  ds3g_dds_switch_status_event_type    dds_switch_status;

  dds_switch_status.new_subs_id = subs_id;
  dds_switch_status.status = status;

  DS3G_EVENT_REPORT_PAYLOAD(EVENT_DS3G_DDS_SWITCH,
                            sizeof(ds3g_dds_switch_status_event_type),
                            (void*)&dds_switch_status,
                            subs_id);
  

  
}
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
)
{
  ds3g_data_call_trm_control_event_type  data_call_event;
  ds3g_data_call_activity_enum_type event;
  
  switch(event_id)
  {
    case DS3GEVENTMGR_PS_CALL_ORIG_EV:
      event = DS3G_DATA_CALL_ORIG_EV;
      break;
    case DS3GEVENTMGR_PS_CALL_END_EV:
      event = DS3G_DATA_CALL_END_EV;
      break;
    case DS3GEVENTMGR_PS_CALL_ACTIVE_EV:
      event = DS3G_DATA_CALL_ACTIVE_EV;
      break;
    case DS3GEVENTMGR_PS_CALL_DORMANT_EV:
      event = DS3G_DATA_CALL_DORMANT_EV;
      break;
    case DS3GEVENTMGR_PS_CALL_NO_FLOW_EV:
      event = DS3G_DATA_CALL_NO_FLOW_EV;
      break;
    case DS3GEVENTMGR_RS_RA_EV:
      event = DS3G_DATA__RS_RA_EV;
      break;
    default:
      event = DS3G_DATA_CALL_EV_UNSPECIFIED;
      break;
  }
  data_call_event.subs_id = subs_id;
  data_call_event.event = event;
  data_call_event.proc_type_bitmask = proc_type_bitmask;

  DS3G_EVENT_REPORT_PAYLOAD(EVENT_DS3G_DATA_CALL_TRM_CONTROL,
                            sizeof(ds3g_data_call_trm_control_event_type),
                            (void*)&data_call_event,
                            subs_id);

}

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
)
{
  ds3g_tsrc_status_event_type    tsrc_event_status;
  sys_modem_as_id_e_type           cm_dds_subs_id = ds3gsubsmgr_get_def_data_subs_id();
  ds3g_tsrc_stretch_efs_info_type *tsrw_config_ptr = ds3g_tsrc_get_dds_stretch_efs_info();
  uint16                           stretch_ratio = 0;
  uint32                           current_stretch_value = 0;
  uint16                           current_stretch_mask = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds3gsubsmgr_is_subs_id_valid(cm_dds_subs_id))
  {
    return;
  }

  if (subs_id != cm_dds_subs_id)
  {
    // Requested logging event for non DDS sub
    return;
  }

  if (tsrw_config_ptr == NULL)
  {
    return;
  }

  stretch_ratio = tsrw_config_ptr->stretch_ratio;
  current_stretch_value = ds3g_tsrc_get_current_stretch_value();
  current_stretch_mask = ds3g_tsrc_get_stretch_rats_mask();

  DATA_MSG4_EX(MSG_LEGACY_HIGH, 
              subs_id,
              "ds3g_logging_tsrc_chg_event sub id %d,"
              "stretch val %d, stretch rats mask %d, stretch ratio %d",
              subs_id, 
              current_stretch_value,
              current_stretch_mask,
              stretch_ratio);

  tsrc_event_status.subs_id = subs_id;
  tsrc_event_status.stretch_val = current_stretch_value;
  tsrc_event_status.stretch_rats_mask = current_stretch_mask;
  tsrc_event_status.stretch_ratio = stretch_ratio;
  tsrc_event_status.reserved = 0;

  DS3G_EVENT_REPORT_PAYLOAD(EVENT_DS3G_TSRC_CHG,
                            sizeof(ds3g_tsrc_status_event_type),
                            (void*)&tsrc_event_status,
                            subs_id); 
} /* ds3g_logging_tsrc_chg_event */
#endif /* FEATURE_DATA_TSRC */

/*=========================================================================== 
FUNCTION	  DS3G_POWER_STATS_LOGGING 
  
DESCRIPTION   this function log the power stats info 
  
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
) 
{ 
  LOG_DS_POWER_STATS_INFO_C_type		*log_ptr = NULL;
  /* - - - - - - - - - - - - - -	- - - - - - - - - - - - - - - - - - - - - */ 
 
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id)) 
  { 
    return; 
  } 
  /* Allocate memory for the log ptr */ 
  log_ptr = (LOG_DS_POWER_STATS_INFO_C_type*)log_alloc(LOG_DS_POWER_STATS_INFO_C, 
										     sizeof(LOG_DS_POWER_STATS_INFO_C_type)); 

  if (log_ptr == NULL) 
  { 
    return; 
  } 
  memset(&(log_ptr->power_stats),0,sizeof(ds_power_stats_type));
 
  log_ptr->sub_id = (uint8)ds3gsubsmgr_subs_id_cm_to_ds(subs_id); 
  log_ptr->version = DS_POWER_STATS_LOGGING_VERSION; 
  
  log_ptr->power_stats.elapsed_time = elapsed_time;
  log_ptr->power_stats.sleep_time   = sleep_time;
  log_ptr->power_stats.tx_rx_time   = tx_rx_time;
  log_ptr->power_stats.tx_time      = tx_time;
  log_ptr->power_stats.rx_time      = rx_time;
  log_ptr->power_stats.idle_time    = idle_time;
  
  log_commit(log_ptr);	
  
  return; 
}/* ds3g_power_stats_logging */ 