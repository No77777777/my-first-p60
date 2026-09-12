#ifndef TM_DS_IFACE_H
#define TM_DS_IFACE_H


/*===========================================================================
  
                            TM_CM_IFACE  h e a d e r
                   
DESCRIPTION

Copyright (c) 2015 - 2019 Qualcomm Technologies, Inc
Qualcomm Technologies Confidential and Proprietary. All Rights Reserved. 

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/prot/tm_ds_iface.h#1 $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
11/30/18    skm     Initial Release
===========================================================================*/
#include "ds_sys.h"
#include "gps_common.h"

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                               DEFINES                                   */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef struct
{
  uint32                              apn_string_len;
  sint7                               apn_string[DS_SYS_MAX_APN_LEN];
}tm_ds_apn_name_type;                             

/* APN table for each SUB. currently used with emergency APN's*/
typedef struct
{
  tm_ds_apn_name_type           apn_info;
  ds_sys_subscription_enum_type ds_sub;
  cgps_SrvSystemType            srv_sys;
  boolean                       v_subActive;
}tm_ds_sub_apn_entry;

/* Data structures to capture Event type in callback
   Currently holds only SS event, can be extended to
   capture other DS events and WIFI Mac address*/
typedef enum
{
  TM_DS_SS_EVENT
} tm_ds_event_cb_e_type;

/*DS event Payload.*/
typedef struct
{
  sys_sys_mode_e_type    srv_system;    /*Serving system*/
  tm_ds_event_cb_e_type  ds_event_type; /* DS event type*/
  sys_modem_as_id_e_type subs;          /* SUB ID*/
}tm_ds_event_info_s_type;



/*===========================================================================

FUNCTION tm_ds_get_emergency_sys_info

DESCRIPTION
  Function iterates througth DS profiles to get the Emergency APN name. The APN
  Name is saved to be used later to pick the correct RAT information for the
  corresponding APN.
  The DS System information is received as a callback from DS via DS SYS CONF EX
  Saved APN information is used to go throught the SYS CONF list and pick the
  correct RAP for the APN.
 
DEPENDENCIES
  Function is called on 911 call orig and if feature SUPL over VoWIFI beign
  enabled.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void tm_ds_get_emergency_sys_info(void);

/*===========================================================================

FUNCTION tm_ds_get_srv_sys

DESCRIPTION
   Function returns the Serving System on which Data services has brought up
   the Emergency data call.
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern cgps_SrvSystemType tm_ds_get_emergency_srv_sys(void);

/*===========================================================================

FUNCTION  tm_ds_iface_event_proc

DESCRIPTION 
  Handles an incoming DS event

RETURN VALUE 
  None

DEPENDENCIES

===========================================================================*/
extern void tm_ds_iface_event_proc( tm_ds_event_info_s_type* p_data );

/*===========================================================================

FUNCTION tm_ds_iface_init

DESCRIPTION
   Function initializes callbacks with Data Services to listen to
   SYSTEM STATUS EX information from DS subsystem.
   SYSTEM STATUS EX: provide the list of APN's and the corresponding
                     RAT the are on. ex: 1x/G/W/L/WLAN.
 
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void tm_ds_iface_init(void);

#endif /* TM_DS_IFACE_H */


