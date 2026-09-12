#ifndef DS3GTRMMGR_H
#define DS3GTRMMGR_H
/*===========================================================================

                3 G   D A T A   S E R V I C E S   TRM   M A N A G E R

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for 3G Data Services TRM handling.

  Copyright (c) 2001-2016 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:
  $Header:

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/25/16   ag      Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "sys.h"
#include "sys_v.h"
#include "dstask_v.h"


/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*-------------------------------------------------------------------------
  Enum for ds3g_trmmgr proc type type. 
  LSB bit indicated the highest priority and MSB indicates the lowest 

  Current priority order: ( high to low )
   - MMS
   - RS_RA
   - OTHER
-------------------------------------------------------------------------*/

typedef enum
{
  DS3GTRMMGR_PROC_TYPE_NONE              = 0x00,
  DS3GTRMMGR_PROC_TYPE_MMS_BIT           = 0x01,
  DS3GTRMMGR_PROC_TYPE_RS_RA_BIT         = 0x02,
  DS3GTRMMGR_PROC_TYPE_NON_IMS_OTHER_BIT = 0x03,
  DS3GTRMMGR_PROC_TYPE_IPV6_IFACE_BIT    = 0x04,
  DS3GTRMMGR_PROC_TYPE_TCP_RETRANS_BIT   = 0x05,
  DS3GTRMMGR_PROC_TYPE_INTERNET_BIT      = 0x06,
  DS3GTRMMGR_PROC_TYPE_HIGH_PRIO_PS_CALL = 0x07,
  DS3GTRMMGR_PROC_TYPE_MO_DATA_BIT       = 0x08
}ds3gtrmmgr_proc_type_bit_mask;

/*-------------------------------------------------------------------------
  Enum for ds3g_trmmgr PS call state type
-------------------------------------------------------------------------*/
typedef enum
{
  DS3GTRMMGR_PS_CALL_STATE_NO_CALL   = 0,
  DS3GTRMMGR_PS_CALL_STATE_ORIG      = 1,
  DS3GTRMMGR_PS_CALL_STATE_ACTIVE    = 2,
  DS3GTRMMGR_PS_CALL_STATE_DORMANT   = 3,
  DS3GTRMMGR_PS_CALL_STATE_NO_FLOW   = 4
}ds3gtrmmgr_ps_call_state_type;

typedef enum
{
  DS3GTRMMGR_HIGH_PRIO_PS_CALL_START = 1,
  DS3GTRMMGR_HIGH_PRIO_PS_CALL_END   = 2,
  DS3GTRMMGR_VOICE_CALL_START        = 3,
  DS3GTRMMGR_VOICE_CALL_END          = 4
}ds3gtrmmgr_high_prio_ps_call_conflict_act_enum_type;

/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*===========================================================================
FUNCTION      DS3GTRMMGR_INIT

DESCRIPTION   This function performs initialization of ds3gtrmmgr

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gtrmmgr_init(void);

/*===========================================================================
FUNCTION       DS3GTRMMGR_LAST_RS_EVENT_PROCESS

DESCRIPTION    This function will process the last RS event and set 
               the appropriate proc type 

DEPENDENCIES   NONE

RETURN VALUE   TRUE - if the proc type was updated
               FALSE - otherwise


SIDE EFFECTS   NONE
===========================================================================*/

boolean ds3gtrmmgr_last_rs_event_process
(
  sys_modem_as_id_e_type        subs_id,
  ds3geventmgr_rs_ra_enum_type  rs_ra_type
);
/*===========================================================================
FUNCTION       DS3GTRMMGR_IPV6_IFACE_BRINGUP_EVENT_PROCESS

DESCRIPTION    This function will process IPV6_IFACE_BRINGUP_EVENT

DEPENDENCIES   NONE

RETURN VALUE   TRUE - if the proc type was updated
               FALSE - otherwise

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds3gtrmmgr_ipv6_iface_bringup_event_process
(
  sys_modem_as_id_e_type          subs_id,
  ds3geventmgr_ipv6_iface_up_in_progress_enum_type    ipv6_iface_up_type
);
/*===========================================================================
FUNCTION       DS3GTRMMGR_TCP_RETRANSMIT_EVENT_PROCESS

DESCRIPTION    This function will process TCP_RETRANSMIT_EVENT

DEPENDENCIES   NONE

RETURN VALUE   TRUE - if the proc type was updated
               FALSE - otherwise

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds3gtrmmgr_tcp_retansmit_event_process
(
  sys_modem_as_id_e_type          subs_id,
  ds3geventmgr_tcp_retransmit_enum_type    tcp_retrans_type
);
/*===========================================================================
FUNCTION       DS3GTRMMGR_HIGH_PRIO_PS_CALL_EVENT_PROCESS

DESCRIPTION    This function will process TCP_RETRANSMIT_EVENT

DEPENDENCIES   NONE

RETURN VALUE   TRUE - if the proc type was updated
               FALSE - otherwise

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds3gtrmmgr_high_prio_ps_call_event_process
(
  sys_modem_as_id_e_type          subs_id,
  ds3geventmgr_high_prio_ps_call_enum_type    high_prio_ps_call_type
);
/*===========================================================================
FUNCTION DS3GTRMMGR_CENTRALIZE_PROC_HIGH_PRIO_PS_CALL_AND_VOICE_CALL

DESCRIPTION
   this func check if high priority ps call confilct with voice call.
   and decide if need to set/clear bit DS3GTRMMGR_PROC_TYPE_HIGH_PRIO_PS_CALL
   
PARAMETERS
  subid : subscription id
  conflict_action:the action happens which may impact conflict between
  high priority ps call and voice call

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none
===========================================================================*/
void ds3gtrmmgr_centralize_proc_high_prio_ps_call_and_voice_call
(
  sys_modem_as_id_e_type                              subs_id,
  ds3gtrmmgr_high_prio_ps_call_conflict_act_enum_type conflict_action,
  boolean                                            *notify_other_sub
);

/*===========================================================================
FUNCTION       DS3GTRMMGR_PS_CALL_EVENT_PROCESS

DESCRIPTION    This function will process the ps call event and set the
               the appropriate proc type 

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/

void ds3gtrmmgr_ps_call_event_process
(
  sys_modem_as_id_e_type               subs_id,
  ds3geventmgr_event_type              event_id,
  ds3g_data_call_activity_ps_call_type call_type
);

/*===========================================================================
FUNCTION       DS3GTRMMGR_GET_FINAL_PROC_TYPE

DESCRIPTION    This function will return the final proc type

DEPENDENCIES   NONE

RETURN VALUE   final proc type

SIDE EFFECTS   NONE
===========================================================================*/

sys_proc_type_e_type ds3gtrmmgr_get_final_proc_type
(
  sys_modem_as_id_e_type   subs_id
);

/*===========================================================================
FUNCTION       DS3GTRMMGR_GET_PS_CALL_STATE

DESCRIPTION    This function will return the current PS call state

DEPENDENCIES   NONE

RETURN VALUE   final proc type

SIDE EFFECTS   NONE
===========================================================================*/

ds3gtrmmgr_ps_call_state_type ds3gtrmmgr_get_ps_call_state
(
  sys_modem_as_id_e_type   subs_id
);
/*===========================================================================
FUNCTION       DS3GTRMMGR_CHECK_CHANGE_PROC_TYPE_AFTER_DDS_SWITCH

DESCRIPTION    This function will check amd change proc type after dds switch

DEPENDENCIES   only be called after dds switch done

RETURN VALUE   void

SIDE EFFECTS   NONE
===========================================================================*/
void ds3gtrmmgr_check_change_proc_type_after_dds_switch
(
  sys_modem_as_id_e_type   new_dds_subs_id
);
/*===========================================================================
FUNCTION       DS3GTRMMGR_INCR_PROC_TYPE_REF_COUNT

DESCRIPTION    This function will increament the ref count of the approproate
               proc type;

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/

void ds3gtrmmgr_incr_proc_type_ref_count
( 
  sys_modem_as_id_e_type          subs_id,
  ds3gtrmmgr_proc_type_bit_mask   proc_type_bit
);

/*===========================================================================
FUNCTION       DS3GTRMMGR_CHECK_IF_RESET_PROC_TYPE_REQUIRED

DESCRIPTION    This function will decreament the ref count of the approproate
               proc type

DEPENDENCIES   NONE

RETURN VALUE   TRUE: if the ref count goes to 0
               FALSE: otherwise

SIDE EFFECTS   NONE
===========================================================================*/

boolean ds3gtrmmgr_check_if_reset_proc_type_required
( 
  sys_modem_as_id_e_type          subs_id,
  ds3gtrmmgr_proc_type_bit_mask   proc_type_bit
);
/*===========================================================================
FUNCTION       DS3GTRMMGR_SET_HIGH_PRIO_PS_CALL_BIT

DESCRIPTION    This function will set ds3gtrmmgr_high_prio_ps_call_ongoing

PARAMETERS
  subid : subscription id
  high_prio_ps_call_type:start
                         end
DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds3gtrmmgr_set_high_prio_ps_call_bit
(
  sys_modem_as_id_e_type          subs_id,
  ds3geventmgr_high_prio_ps_call_enum_type    high_prio_ps_call_type
);
/*===========================================================================
FUNCTION      DS3GFLOWMGR_PROCESS_MO_DATA_PROTECT_TIMER_EXPIRED_CMD

DESCRIPTION   This function processes DS_CMD_DS3G_MO_DATA_PROTECT_TIMER_EXPIRED command.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds3gtrmmgr_process_mo_data_protect_timer_expired_cmd
(
  ds_cmd_type  *cmd_ptr
);

/*===========================================================================
FUNCTION       DS3GTRMMGR_MODATA_RAB_REESTAB_EVENT_PROCESS

DESCRIPTION    This function will process DS3GEVENTMGR_MO_DATA_RAB_REESTAB_EV

DEPENDENCIES   NONE

RETURN VALUE   TRUE - if the proc type was updated
               FALSE - otherwise

SIDE EFFECTS   NONE
===========================================================================*/
boolean ds3gtrmmgr_modata_rab_reestab_event_process
(
  sys_modem_as_id_e_type                        subs_id,
  ds3geventmgr_mo_data_rab_reestab_enum_type    modata_rab_reestab_type
);

#endif /* DS3GTRMMGR_H */
