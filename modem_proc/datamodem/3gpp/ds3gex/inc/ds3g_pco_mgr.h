#ifndef DS3G_PCO_MGR_H
#define DS3G_PCO_MGR_H
/*===========================================================================

                3 G   D A T A   S E R V I C E S   E V E N T   M A N A G E R

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for 3G Data Services event handling.
  It provides APIs for DS3G event registration and notification.

  Copyright (c) 2001-2015 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:
  $Header:

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/14/16   js      Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "queue.h"
#include "ps_sys.h"
#include "ps_sys_event.h"
#include "ds3gsubsmgr.h"
#include "ds3geventmgr.h"
#include "ds_3gpp_pdn_context.h"

#include "sys.h"
#include "sys_v.h"
#include "dstask_v.h"

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

#define  DS3G_PCO_MAX_CONTAINERS_NUM           2

/*-------------------------------------------------------------------------
  Use array to store multiple containers per APN per pco id
-------------------------------------------------------------------------*/
typedef struct
{
  char                                         apn_name[PS_SYS_MAX_APN_LEN];
  uint8                                        num_containers;
  ps_sys_op_reserved_pco_type                  pco_info[DS3G_PCO_MAX_CONTAINERS_NUM];
}ds3g_pco_mgr_event_apn_info_type;

/*-------------------------------------------------------------------------
  Structure to nofity PCO event info
-------------------------------------------------------------------------*/
typedef struct
{
  uint16                                       pco_id;
  ds3g_pco_mgr_event_apn_info_type            *apn_pco_info[DS_MAX_APN_NUM];
}ds3g_pco_mgr_event_info_type;

/*------------------------------------------------------------------------
   Clients Registers a callback function with DS3G_PCO_MGR
   for PCO event info.
-------------------------------------------------------------------------*/
typedef void (*ds3g_pco_mgr_notify_func_type)(  
           uint16                                pco_id,
           ps_sys_subscription_enum_type         ps_subs_id,
           ds3g_pco_mgr_event_apn_info_type     *apn_pco_info,
           void                                 *data_ptr);

/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*===========================================================================
FUNCTION      DS3G_PCO_MGR_INIT

DESCRIPTION   This function performs initialization of ds3g_pco_mgr event q.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_pco_mgr_init(void);

/*===========================================================================
FUNCTION      DS3G_PCO_MGR_EVENT_REG

DESCRIPTION   Called by clients to register to get DS3G PCO events.

DEPENDENCIES  None.

RETURN VALUE  TRUE: If registration successful.
              FALSE: If registration unsuccessful.

SIDE EFFECTS  Registered client is entered on a queue.
===========================================================================*/
boolean ds3g_pco_mgr_event_reg
(
  uint16                                       pco_id,
  ds3geventmgr_client_type                     client_id,
  ps_sys_subscription_enum_type                ps_subs_id,
  ds3g_pco_mgr_notify_func_type                cb_func_ptr,
  void                                        *cb_data_ptr
);

/*===========================================================================
FUNCTION      DS3G_PCO_MGR_EVENT_DEREG

DESCRIPTION   Called by clients to de-register DS3G PCO event notification.

DEPENDENCIES  None.

RETURN VALUE  TRUE: If de-registration successful.
                    If client previously de-registgered.

              FALSE: If de-registration unsuccessful.

SIDE EFFECTS  De-registered client is dequeued.
===========================================================================*/
boolean ds3g_pco_mgr_event_dereg
(
  ds3geventmgr_client_type              client_id,
  uint16                                pco_id,
  ps_sys_subscription_enum_type         ps_subs_id
);

/*===========================================================================
FUNCTION      DS3G_PCO_MGR_GET_CONF_INFO

DESCRIPTION   Called by clients to get cached information

DEPENDENCIES  None.

RETURN VALUE  TRUE: If operation was successful & payload was provided 
              FALSE: Otherwise 

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_pco_mgr_get_conf_info
(
  uint16                                pco_id,
  ds3g_pco_mgr_event_info_type         *pco_event_info,
  ps_sys_subscription_enum_type         ps_subs_id
);

/*===========================================================================
FUNCTION      DS3G_PCO_MGR_SET_EVENT_INFO

DESCRIPTION   Called by clients to publish DS3G PCO info to be given to 
              registered clients.

DEPENDENCIES  None.

RETURN VALUE  None. 

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_pco_mgr_set_event_info
(
  uint16                                       pco_id,
  ds3g_pco_mgr_event_apn_info_type            *apn_pco_info,
  ps_sys_subscription_enum_type                ps_subs_id
);

/*===========================================================================
FUNCTION DS3G_PCO_MGR_DECODE_PCO_PKT_FOR_INTERNAL

DESCRIPTION   This function decodes the operator reserved PCO received from 
              the network for internal clients.
  
PARAMETERS    const byte                        *protocol_pkt_ptr,
              int                                protocol_pkt_len,
              uint16                             protocol_id,
              ds3g_pco_mgr_event_apn_info_type  *internal_pco_info_ptr
    
DEPENDENCIES  None.

RETURN VALUE  TRUE : if successful
              FALSE : on error
  
SIDE EFFECTS  None.
===========================================================================*/
boolean ds3g_pco_mgr_decode_pco_pkt_for_internal
(
  const byte                            *protocol_pkt_ptr,
  int                                    protocol_pkt_len,
  uint16                                 protocol_id,
  ds3g_pco_mgr_event_apn_info_type      *internal_pco_info_ptr,
  ds3gsubsmgr_subs_id_e_type             subs_id  
);

/*===========================================================================
FUNCTION      DS3G_PCO_MGR_REMOVE_ENTRY_IN_PCO_INFO_CACHE

DESCRIPTION   Remove PCO info for a particular APN in PCO info cache

PARAMETERS    char                      *apn,
              sys_modem_as_id_e_type     subs_id

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds3g_pco_mgr_remove_entry_in_pco_info_cache
(
  char                                  *apn,
  sys_modem_as_id_e_type                 subs_id
);

#endif /* DS3G_PCO_MGR_H */
