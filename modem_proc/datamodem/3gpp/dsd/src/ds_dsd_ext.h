#ifndef DS_DSD_EXT_H
#define DS_DSD_EXT_H
/*===========================================================================

                      D S _ D S D _ E X T . H

DESCRIPTION


EXTERNALIZED FUNCTIONS

 Copyright (c) 2009 - 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsd/src/ds_dsd_ext.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/11/11    sk    Created Module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "dstask_v.h"
#include "ds_dsd_common.h"
#include "cm.h"

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Command queues in Data Services Task.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef enum
{
  DS_DSD_INTERNAL_CMD_QUEUE   = 0,   /* DS Task internal command queue */
  DS_DSD_REGULAR_CMD_QUEUE    = 1,   /* DS Task command queue          */
  DS_DSD_MAX_CMD_QUEUE               /* MAX command queue - Not used   */
}ds_dsd_cmd_queue_enum_type;

/*---------------------------------------------------------------------------
  Mode pref cmd info structure
---------------------------------------------------------------------------*/
typedef struct
{
  cm_mode_pref_e_type         mode_pref;
  ds3gsubsmgr_subs_id_e_type  subs_id;
}ds_dsd_mode_pref_cmd_info_type;

/*---------------------------------------------------------------------------
  Subs capability cmd info structure
---------------------------------------------------------------------------*/
typedef struct
{
  sys_sys_mode_mask_e_type    subs_capability;
  ds3gsubsmgr_subs_id_e_type  subs_id;
}ds_dsd_subs_capability_cmd_info_type;

typedef struct
{
  dsd_radio_type          client_radio;
  ds_dsd_event_info_type  event_info;
}ds_dsd_cmd_info_type;

/*---------------------------------------------------------------------------
  LTE emergency call cmd info structure
---------------------------------------------------------------------------*/
typedef struct
{
  boolean                     in_prog;
  ds3gsubsmgr_subs_id_e_type  subs_id;
}ds_dsd_lte_emerg_call_cmd_type;

#ifdef FEATURE_LAPP
/*---------------------------------------------------------------------------
  5G state change cmd info structure
---------------------------------------------------------------------------*/
typedef struct
{
  ds3gsubsmgr_subs_id_e_type  subs_id;
}ds_dsd_nr5g_re_order_pref_list_cmd_info_type;
#endif /* FEATURE_LAPP */

/*-------------------------------------------------------------------
  DSD event counters
-------------------------------------------------------------------*/
typedef struct
{
  uint8 timer_expired_cmd_cnt;
  uint8 sys_chg_cmd_cnt;
  uint8 recompute_cmd_cnt;
  uint8 ext_notify_cmd_cnt;
  uint8 mode_pref_chg_cmd_cnt;
  uint8 dsd_reset_cmd_cnt;
  uint8 lte_emerg_cmd_cnt;
  uint8 subs_capability_chg_cmd_cnt;
#ifdef FEATURE_LAPP
  uint8 nr5g_acq_lost_cmd_cmt;
#endif /* FEATURE_LAPP */
}ds_dsd_cmd_counter_type;

/*------------------------------------------------------------------------
  DSD ext notification info type
------------------------------------------------------------------------*/
typedef struct
{
  boolean                  ext_notify_pending;
  dsd_radio_type           old_radio_pending;
  dsd_radio_type           new_radio_pending;
  ds_dsd_cmd_counter_type  cmd_counter;
}dsd_ext_notif_info_type;

/*===========================================================================

                        GLOBAL DATA STRUCTURES

===========================================================================*/
extern rex_crit_sect_type       ds_dsd_critical_section;

/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION      DS_DSD_EXT_NOTIFY_PREF_SYSTEM_EXTERNAL

DESCRIPTION   This function notifies the entities outside modem about the
              preferred system change event.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_dsd_ext_notify_pref_system_external
(
  dsd_radio_type              old_pref_sys,
  dsd_radio_type              new_pref_sys,
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_INCR_EVENT_COUNTER

DESCRIPTION   Increment the cmd counter.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_incr_cmd_counter
(
  ds3gsubsmgr_subs_id_e_type  subs_id,
  ds_cmd_enum_type            cmd_id
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_DECR_EVENT_COUNTER

DESCRIPTION   Decrement the event counter.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_decr_cmd_counter
(
  ds3gsubsmgr_subs_id_e_type  subs_id,
  ds_cmd_enum_type            cmd_id
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_PUT_CMD

DESCRIPTION   DSD wrapper function for DS put command functions.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_ext_put_cmd
(
  ds_dsd_cmd_queue_enum_type  cmd_q_id,
  ds3gsubsmgr_subs_id_e_type  subs_id,
  ds_cmd_type                *cmd_ptr
);

/*===========================================================================
FUNCTION      DS_DSD_RADIO_EVENT_GEN

DESCRIPTION   This function is used to generate radio diag events.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_radio_event_gen
(
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_TIMER_EVENT_GEN

DESCRIPTION   This function is used to generate timer events.

DEPENDENCIES  None.
	
RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_timer_event_gen
(
  ds_dsd_timer_event_type     timer_event,
  uint8                       dsd_timer_id,
  ds3gsubsmgr_subs_id_e_type  subs_id,
  rex_timer_cnt_type          timer_value
);

/*===========================================================================
FUNCTION      DS_DSD_THROTTLE_EVENT_GEN

DESCRIPTION   This function is used to generate throttle diag events.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_dsd_throttle_event_gen
(
  dsd_radio_type              dsd_radio,
  boolean                     throttle,
  ds3gsubsmgr_subs_id_e_type  subs_id  
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_GET_TRAT

DESCRIPTION   This function returns trat value

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
boolean ds_dsd_ext_get_trat
(
  ds3gsubsmgr_subs_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_DSD_EXT_GET_DDS_SWITCH_TYPE

DESCRIPTION   This function returns DDS switch type

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
cm_dds_cause_e_type ds_dsd_ext_get_dds_switch_type( void );

/*===========================================================================
FUNCTION      DS_DSD_EXT_GET_RESOLVED_RADIO_RAT_MASK_FOR_NOTIFY

DESCRIPTION   This function returns the resolved rat mask of a state machine 
              belonging to the radio system. For example, when Srv status is
              not indicating full src, rat mask should be NULL BEARER.

DEPENDENCIES  None.

RETURN VALUE  rat_mask.

SIDE EFFECTS  None.
===========================================================================*/
uint32 ds_dsd_ext_get_resolved_radio_rat_mask_for_notify
(
  dsd_radio_type              radio,
  ds3gsubsmgr_subs_id_e_type  subs_id
);

#endif /* DS_DSD_EXT_H */
