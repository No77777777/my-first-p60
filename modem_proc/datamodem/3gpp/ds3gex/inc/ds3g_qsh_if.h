#ifndef _DS3G_QSH_IF_H
#define _DS3G_QSH_IF_H

/*===========================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/


/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ds3gex/inc/ds3g_qsh_if.h#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
08/27/2018 rg      Added QTF Event for modify profile and handler for
                   get_profile_setting 
9/20/2017  amisra  Deterministic test framework development
==============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#if defined(FEATURE_DS_QSH_INTERFACE) || defined(FEATURE_QSH_MDUMP)
#include "dstask_v.h"
#include "sys.h"
#include "msg.h"
#include "err.h"
#include "rex.h"
#include "qsh.h"
#include "ds_qmi_wds.h"

/*===========================================================================

                         STRUCTURES AND ENUMS

===========================================================================*/

#ifdef FEATURE_DS_QSH_INTERFACE

typedef enum
{
  DS_QSH_EVENT_START_NETWORK_INTERFACE =0,
  DS_QSH_EVENT_DSD_WLAN_AVAILABLE =1,
  DS_QSH_EVENT_DS_EPC_PDN =2,
  DS_QSH_EVENT_PS_SYS_INVOKE_CBACK =3,
  DS_QSH_EVENT_PDN_UP_ON_IWLAN =4,
  DS_QSH_EVENT_PDN_DOWN_ON_IWLAN =5,
  DS_QSH_EVENT_MODIFY_PROFILE_SETTINGS =6
}ds3g_qsh_event_e;

typedef enum
{
  DS_QSH_EVENT_NOTIFY_WLAN_OFF =0,
  DS_QSH_EVENT_NOTIFY_GET_PROFILE_SETTINGS =1
}ds3g_qsh_event_notify_e;

typedef struct
{
  boolean                 trigger_wifi;
  sys_modem_as_id_e_type  subs_id;
}ds_qsh_handler_wlan_off;

typedef struct
{
  uint8                   profile_type;
  uint8                   profile_index;
  sys_modem_as_id_e_type  subs_id;
  boolean                 handle_configure;
}ds_qsh_handler_get_profile_settings;

#define QSH_PROFILE_TYPE_3GPP (0x00)
#define QSH_PROFILE_TYPE_3GPP2 (0x01)

typedef struct
{
  ds_qsh_handler_wlan_off notify_wlan_off;
  ds_qsh_handler_get_profile_settings get_profile_settings;
}ds_qsh_handler;

#endif/*FEATURE_DS_QSH_INTERFACE*/
/*===========================================================================

                         FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================
FUNCTION      DS3G_QSH_IF_INIT

DESCRIPTION   This is the initialization function for QSH.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/

void ds3g_qsh_if_init
( 
  void 
);

#ifdef FEATURE_DS_QSH_INTERFACE
/*===========================================================================
FUNCTION      DS3G_QSH_IF_REPORT_EVENT

DESCRIPTION   This is the function to report event to QSH. The event mask is
			  checked to confirm if the corresponding event is checked. If the
			  event mask is set then event is reported else the event is not 
			  reported.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds3g_qsh_if_report_event
(
  sys_modem_as_id_e_type  subs_id,            /* CM subs id */
  ds3g_qsh_event_e        event_id            /* Event configuration commands */
);

/*===========================================================================
FUNCTION      DS3G_QSH_IF_CMD_HDLR

DESCRIPTION   This is the command handler function for QSH for handling the 
              command put into the queue by the callback function

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds3g_qsh_if_cmd_hdlr
(
  ds_cmd_type *cmd_ptr  /* Command pointer */
);

#endif /*FEATURE_DS_QSH_INTERFACE*/
#endif /* (FEATURE_DS_QSH_INTERFACE) || (FEATURE_QSH_MDUMP) */
#endif /* DS3G_QSH_IF_H */