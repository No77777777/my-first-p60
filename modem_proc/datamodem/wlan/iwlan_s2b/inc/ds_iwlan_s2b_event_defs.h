#ifndef DS_IWLAN_S2B_EVENT_DEFS_H
#define DS_IWLAN_S2B_EVENT_DEFS_H
/*===========================================================================
                           DS_IWLAN_S2B_EVENT_DEFS
===========================================================================*/

/*!
  @file
  ds_iwlan_s2b_event_defs.h

  @brief
  This header describes the event payloads

  @detail
*/

/*===========================================================================

  Copyright (c) 2012-2016 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/wlan/iwlan_s2b/inc/ds_iwlan_s2b_event_defs.h#2 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/17/16    cg     Added EVENT_IWLAN_S2B_FAILURE definition
02/18/16    fn     Added state information 
08/15/13    bf     Added a last and max module id
08/13/13    bf     Addition of generic call ID definition
08/12/13    bf     Revision of SUBEVENT_ID enumeration
07/24/13    bf     Revision of MODULE_ID enumeration
07/18/13    bf     Clean up of padding issue
07/01/13    bf     Initial Declarations
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

// Variations in modem code
#include "comdef.h"
#include "customer.h"
#include "datamodem_variation.h"

#ifdef FEATURE_DATA_IWLAN_S2B

/*===========================================================================
                            DEFINE AND MACROS
===========================================================================*/
/* Used for having a concrete payload size in event logging */
#define DS_IWLAN_S2B_LOGGING_TYPE_SUB_EVENT_ID        uint16  // 2 bytes
#define DS_IWLAN_S2B_LOGGING_TYPE_MODULE_ID           uint8   // 1 byte
#define DS_IWLAN_S2B_LOGGING_TYPE_CALL_ID             uint8   // 1 byte
                                                              // ---------------
                                                              // TOTAL: 4 bytes

/* In some places it is impossible to get call ID */
#define DS_IWLAN_S2B_LOGGING_CALL_ID_GENERIC          254

/* Sometimes a call ID is attempted to be found but an error occurs */
#define DS_IWLAN_S2B_LOGGING_CALL_ID_INVALID          253

#define DS_IWLAN_S2B_LOGGING_APN_BUF_LEN              12

/*===========================================================================
                                ENUMERATIONS
===========================================================================*/
/*
 * Module ID
 */
typedef enum
{
  DS_IWLAN_S2B_LOGGING_INVALID_MODULE        = 0,
  DS_IWLAN_S2B_LOGGING_GENERIC_MODULE        = 1,
  DS_IWLAN_S2B_LOGGING_QMI_WDS               = 2,
  DS_IWLAN_S2B_LOGGING_QMI_DSD               = 3,
  DS_IWLAN_S2B_LOGGING_REV_IP_XPORT          = 4,
  DS_IWLAN_S2B_LOGGING_REV_IP_XPORT_IFACE    = 5,
  DS_IWLAN_S2B_LOGGING_RMNET                 = 6,
  DS_IWLAN_S2B_LOGGING_MPPM                  = 7,
  DS_IWLAN_S2B_LOGGING_DSD                   = 8,
  DS_IWLAN_S2B_LOGGING_EPC                   = 9,
  DS_IWLAN_S2B_LOGGING_3GPP_MODE_HDLR        = 10,
  DS_IWLAN_S2B_LOGGING_3GPP_IFACE            = 11,
  DS_IWLAN_S2B_LOGGING_3GPP2_MODE_HDLR       = 12,
  DS_IWLAN_S2B_LOGGING_3GPP2_IFACE           = 13,
  DS_IWLAN_S2B_LOGGING_WLAN_PROXY_IFACE_HDLR = 14,
  DS_IWLAN_S2B_LOGGING_WLAN_PROXY_IFACE      = 15,
  DS_IWLAN_S2B_LOGGING_IWLAN_S2B_MODE_HDLR   = 16,
  DS_IWLAN_S2B_LOGGING_IWLAN_S2B_IFACE       = 17,
  DS_IWLAN_S2B_LOGGING_IWLAN_S2B_IKEv2_HDLR  = 18,
  DS_IWLAN_S2B_LOGGING_IPSEC_MDM             = 19,
  DS_IWLAN_S2B_LOGGING_PS_IFACE_FRAMEWORK    = 20,
  DS_IWLAN_S2B_LOGGING_PS_SYS                = 21,
  DS_IWLAN_S2B_LOGGING_DS_SYS                = 22,
  DS_IWLAN_S2B_LOGGING_AP                    = 23,
  DS_IWLAN_S2B_LOGGING_APPLICATION           = 24,
  DS_IWLAN_S2B_LOGGING_NETWORK               = 25,

  DS_IWLAN_S2B_LOGGING_LAST_MOD_ID           = DS_IWLAN_S2B_LOGGING_NETWORK,
  DS_IWLAN_S2B_LOGGING_MAX_MODULE_ID         = 255
}ds_iwlan_s2b_event_enum_module_id;

/*
 * Sub-Event ID
 */
typedef enum
{
  DS_IWLAN_S2B_LOGGING_SUBEVENT_INVALID               = 0,

  /* IWLAN S2B MH State Machine States */
  DS_IWLAN_S2B_LOGGING_ENTER_CLOSED_STATE             = 1,
  DS_IWLAN_S2B_LOGGING_ENTER_TUNNEL_PRE_CONFIG_STATE  = 2,
  DS_IWLAN_S2B_LOGGING_ENTER_TUNNEL_CONFIG_STATE      = 3,
  DS_IWLAN_S2B_LOGGING_ENTER_SIO_CONFIG_STATE         = 4,
  DS_IWLAN_S2B_LOGGING_ENTER_RM_CONFIG_STATE          = 5,
  DS_IWLAN_S2B_LOGGING_ENTER_NET_UP_STATE             = 6,
  DS_IWLAN_S2B_LOGGING_ENTER_TUNNEL_RESYNC_STATE      = 7,
  DS_IWLAN_S2B_LOGGING_ENTER_TUNNEL_TEARDOWN_STATE    = 8,

  /* IWLAN S2B MH State Machine Transitions */
  DS_IWLAN_S2B_LOGGING_BRING_UP_EV_IN_CLOSED_STATE    = 9,
  DS_IWLAN_S2B_LOGGING_TRANSISTION_STATE              = 10,

  DS_IWLAN_S2B_LOGGING_SUBEVENT_LAST         = DS_IWLAN_S2B_LOGGING_BRING_UP_EV_IN_CLOSED_STATE,

  DS_WLAN_PROXY_LOG_ENTER_CLOSED_STATE             = 11,
  DS_WLAN_PROXY_LOG_ENTER_SIO_CONFIG_STATE  = 12,
  DS_WLAN_PROXY_LOG_ENTER_RM_CONFIG_STATE      = 13,
  DS_WLAN_PROXY_LOG_ENTER_RM_DOWN_STATE         = 14,
  DS_WLAN_PROXY_LOG_ENTER_NET_UP_STATE          = 15,
  DS_WLAN_PROXY_LOG_ENTER_RECONFIG_STATE             = 16,
  DS_WLAN_PROXY_LOG_ENTER_RM_RESYNC_STATE      = 17,

  DS_IWLAN_S2B_LOGGING_SUBEVENT_MAX          = 65535
}ds_iwlan_s2b_event_enum_subevent_id;

/*
 * Call IP Type Enum
 */
typedef enum
{
  DS_IWLAN_S2B_LOGGING_CALL_IP_TYPE_INVALID = 0,
  DS_IWLAN_S2B_LOGGING_CALL_IP_TYPE_V4      = 1,
  DS_IWLAN_S2B_LOGGING_CALL_IP_TYPE_V6      = 2
} ds_iwlan_s2b_event_enum_call_ip_type;

/*===========================================================================
                            STRUCTURE DEFINITION
===========================================================================*/

/*
 * ds_iwlan_s2b_logging_event_payload
 *
 * Event payload passed to diag framework
 *
 * Size = 4 bytes
 */
typedef PACK(struct)                                              
{                                                                   
  /* Used to describe the specific event within that module */
  DS_IWLAN_S2B_LOGGING_TYPE_SUB_EVENT_ID     sub_event_id;

  /* Used to describe the module calling the logging event */
  DS_IWLAN_S2B_LOGGING_TYPE_MODULE_ID        module_id;

  /* Used to described the PDN that triggered this event */
  DS_IWLAN_S2B_LOGGING_TYPE_CALL_ID          call_id;

}ds_iwlan_s2b_logging_event_payload;


typedef PACK(struct)
{
  uint8        sm_inst;
  uint8        sm_event;  /* wlan_proxy_sm_input_event_type */
  uint8        current_state; /* wlan_proxy_sm_state_type */
  uint8        next_state;   /*wlan_proxy_sm_state_type*/
}ds_iwlan_s2b_sm_payload_s;

/* Size = 24 bytes */
typedef PACK(struct)
{
  uint16 iface_name;
  uint8  iface_instance;
  uint32 call_end_reason;
  uint16 notify_value;
  uint8  failure_cause;
  char   apn_name[DS_IWLAN_S2B_LOGGING_APN_BUF_LEN];
  uint8  apn_len;
  uint8  call_type;
} ds_iwlan_s2b_failure_payload_s;

/*===========================================================================
                       LOG SPECIFIC TYPES OF EVENTS
            ELIMINATES NEED FOR USER TO EVER MESS WITH ACTUAL PAYLOAD
===========================================================================*/
/*===========================================================================
FUNCTION      ds_iwlan_s2b_log_event_simplified

DESCRIPTION   Called to pass parameters to be logged

PARAMETERS    DS_IWLAN_S2B_LOGGING_TYPE_MODULE_ID     module_id
              DS_IWLAN_S2B_LOGGING_TYPE_SUB_EVENT_ID  sub_event_id
              DS_IWLAN_S2B_LOGGING_TYPE_CALL_ID       call_id

DEPENDENCIES  ds_iwlan_s2b_log_event/ds_iwlan_s2b_get_event_payload_ptr

RETURN VALUE  TRUE:Success
              False:Failure

SIDE EFFECTS  Will eventually be calling diag event framework
===========================================================================*/
boolean ds_iwlan_s2b_log_event_simplified
(
  DS_IWLAN_S2B_LOGGING_TYPE_MODULE_ID     module_id,
  DS_IWLAN_S2B_LOGGING_TYPE_SUB_EVENT_ID  sub_event_id,
  DS_IWLAN_S2B_LOGGING_TYPE_CALL_ID       call_id
);

/*===========================================================================
FUNCTION      DS_IWLAN_S2B_SM_EVENT_REPORT

DESCRIPTION   Reports EVENT_IWLAN_S2B_SM

PARAMETERS    None

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/
void ds_iwlan_s2b_sm_event_report
(
   uint8 sm_inst, 
   uint8 cstate,
   uint8 sm_event,
   uint8 nstate,
   uint32 subs_id
);

#endif /* FEATURE_DATA_IWLAN_S2B */
#endif /* DS_IWLAN_S2B_EVENT_DEFS_H */
