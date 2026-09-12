#ifndef DS3G_DATA_CALL_ACTIVITY_H
#define DS3G_DATA_CALL_ACTIVITY_H
/*===========================================================================

                3G DATA CALL ACTIVITY MANAGER

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for 3G Data Call Activity Manager

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
#include "ps_sys_ioctl.h"

/*---------------------------------------------------------------------------
  Macro to be called only in case of DSDS mode
---------------------------------------------------------------------------*/
#define DS3G_DATA_GET_OTHER_SUBS( subs_id )       \
          ((subs_id == SYS_MODEM_AS_ID_1)?         \
            SYS_MODEM_AS_ID_2:SYS_MODEM_AS_ID_1)

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

typedef enum
{
  DS3G_DATA_CALL_ACTIVITY_PS_CALL_TYPE_OTHER      = 0,
  DS3G_DATA_CALL_ACTIVITY_PS_CALL_TYPE_IMS        = 1,
  DS3G_DATA_CALL_ACTIVITY_PS_CALL_TYPE_MMS        = 2,
  DS3G_DATA_CALL_ACTIVITY_PS_CALL_TYPE_INTERNET   = 3
}ds3g_data_call_activity_ps_call_type;

typedef enum
{
  DS3G_DATA_CALL_ACTIVITY_TEMP_DDS_SWITCH = 0,
  DS3G_DATA_CALL_ACTIVITY_NON_IMS_CALL    = 1,
  DS3G_DATA_CALL_ACTIVITY_PS_REGISTRATION = 2
}ds3g_data_arbitrate_activity_to_cm_bitmask_type;

typedef struct
{
  ps_sys_ioctl_3gpp_ps_reg_type     ps_request;
  sys_modem_as_id_e_type            subs_id;
}ds3g_data_call_ps_registration_info;
/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*===========================================================================
FUNCTION      DS3G_DATA_CALL_ACTIVITY_INIT

DESCRIPTION   This function performs initialization of data call activity manager

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_data_call_activity_init(void);

/*===========================================================================
FUNCTION      DS3G_DATA_CALL_ACTIVITY_NOTIFY_DATA_ACTIVITY_TO_CM

DESCRIPTION   This function notifes data activity to CM

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/

void ds3g_data_call_activity_notify_data_activity_to_cm
(
  sys_modem_as_id_e_type    subs_id,
  sys_data_activity_e_type  data_activity
);

/*===========================================================================
FUNCTION      DS3G_DATA_CALL_ACTIVITY_CB

DESCRIPTION   This callback is called by CM to acknowledge data call activity 
              set by DS

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/

void ds3g_data_call_activity_cb
(
  void                   *data_block_ptr,
  cm_call_cmd_e_type      call_cmd,
  cm_call_cmd_err_e_type  call_cmd_err
);

/*===========================================================================
FUNCTION      DS3G_DATA_CALL_ACTIVITY_DDS_EVENT_CB

DESCRIPTION   This callback is called by DS3GSUBSMGR to notify DDS change event

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/

void ds3g_data_call_activity_dds_event_cb
(
  ds3gsubsmgr_event_type        event_id,
  ds3gsubsmgr_event_info_type  *event_info_ptr,
  void                         *data_ptr
);

/*===========================================================================
FUNCTION       DS3G_DATA_CALL_ACTIVITY_GET_PS_CALL_TYPE

DESCRIPTION    This function return the call type based on the APN name used 
               in call bring up

DEPENDENCIES   NONE

RETURN VALUE   PS call type

SIDE EFFECTS   NONE
===========================================================================*/

ds3g_data_call_activity_ps_call_type ds3g_data_call_activity_get_ps_call_type
(
  sys_modem_as_id_e_type  subs_id,
  char                   *apn_name,
  boolean                 is_ims_or_emergency_call,
  ps_iface_apn_enum_type  apn_type
);

/*===========================================================================
FUNCTION      DS3G_DATA_CALL_ACTIVITY_ARBITRATE_DATA_ACTIVITY_TO_CM

DESCRIPTION   This function will track some variables relative to 
              data activity to cm and according to these variables to 
              decide if notify corresponding data activity to CM

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/
void ds3g_data_call_activity_arbitrate_data_activity_to_cm
(
  sys_modem_as_id_e_type    subs_id,
  sys_data_activity_e_type  data_activity_cm,
  ds3g_data_arbitrate_activity_to_cm_bitmask_type arbitrate_activity_bit
);

/*===========================================================================
FUNCTION    DS3G_DATA_CALL_PS_REG_REQ_HDLR

DESCRIPTION
  Handler for PS_SYS_IOCTL_PS_REG_REQ. Trigger PS REG/DEREG for IMS
  de-registration

PARAMETERS 
  arg_val_ptr: Argument to be populated
  subs_id:     Subscription id
  ps_errno:    Err number if IOCTL cant be handled
   
DEPENDENCIES
  None.

RETURN VALUE
   0  - Success
  -1  - Failure
 
SIDE EFFECTS
  None.
===========================================================================*/ 
int  ds3g_data_call_ps_reg_req_hdlr
(
  void                                    *arg_val_ptr,
  sys_modem_as_id_e_type                   subs_id,
  int16                                   *ps_errno
);

/*===========================================================================
FUNCTION DS3G_DATA_CALL_PROCESS_PS_REG_REQ
 
DESCRIPTION
   This function is used to handle the event when DS task gets command 
   DS_CMD_3GPP_PS_ATTACH_REQ. 

PARAMETERS 
  operation type: either PS_SYS_IOCTL_3GPP_PS_ATTACH or PS_SYS_IOCTL_3GPP_PS_DETACH

DEPENDENCIES 
  
RETURN VALUE 
  void
 
SIDE EFFECTS 
  
===========================================================================*/
void ds3g_data_call_process_ps_reg_req
(
  const ds_cmd_type  *cmd_ptr
);
/*===========================================================================
FUNCTION       DS3G_DATA_CALL_ACTIVITY_NOTIFY_PROC_TYPE_TO_CM

DESCRIPTION    This function will notify final proc type to CM

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/

void ds3g_data_call_activity_notify_proc_type_to_cm
(
  sys_modem_as_id_e_type  subs_id,
  sys_proc_type_e_type    proc_type
);

#endif /* DS3G_DATA_CALL_ACTIVITY_H */
