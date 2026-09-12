#ifndef DS3G_DDS_SWITCH_HDLR_H
#define DS3G_DDS_SWITCH_HDLR_H
/*===========================================================================

                3G DATA DDS SWITCH HANDLER

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for 3G Data DSS switch handling

  Copyright (c) 2001-2019 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:
  $Header:

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/26/16   ag      Initial version.

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
#include "list.h"
#include "ps_iface.h"
#include "ps_sys_event.h"

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
typedef	enum 
{
  DS3G_DDS_SWITCH_PERMANENT = 0x00, 
  /**< Permanent(Default) */
  DS3G_DDS_SWITCH_TEMPORARY = 0x01
  /**< Temporary */
} ds3g_dds_switch_type_enum;


/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/

/*===========================================================================
FUNCTION      DS3G_DDS_SWITCH_HDLR_DDS_SWITCH_REQ

DESCRIPTION   This function performs handles the dds switch IOCLT request from
              the clients

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/

int ds3g_dds_switch_hdlr_dds_switch_req
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno 
);

/*===========================================================================
FUNCTION      DS3G_DDS_SWITCH_HDLR_DDS_SWITCH_ALLOWED_EVENT

DESCRIPTION   This function starts the dds switch data clean up processing

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/


void ds3g_dds_switch_hdlr_dds_switch_allowed_event(void);

/*===========================================================================
FUNCTION      DS3G_DDS_SWITCH_HDLR_IS_DDS_SWITCH_IN_PROGRESS

DESCRIPTION   This function returns if the dds switch is in progress or not.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/

boolean ds3g_dds_switch_hdlr_is_dds_switch_in_progress(void);

/*===========================================================================
FUNCTION      DS3G_DDS_SWITCH_HDLR_DATA_CLEAN_COMPLETE

DESCRIPTION   This function notifies CM that data clean up is complete and
              requests to switch dds.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/

void ds3g_dds_switch_hdlr_data_clean_complete(void);

/*===========================================================================
FUNCTION      DS3G_DDS_SWITCH_HDLR_ALLOWED_CB

DESCRIPTION   This function is registered with CM to know if the DDS switch is
              allowed or not

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/

void ds3g_dds_switch_hdlr_allowed_cb
(
  void                   *data_block_ptr,
  cm_ph_cmd_e_type        ph_cmd,
  cm_ph_cmd_err_e_type    ph_cmd_err
);

/*===========================================================================
FUNCTION      DS3G_DDS_SWITCH_HDLR_SEND_DDS_SWITCH_PS_EVENT

DESCRIPTION   This function will send ps event for DDS switch

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/

void ds3g_dds_switch_hdlr_send_dds_switch_ps_event
(
  sys_modem_as_id_e_type               subs_id,
  ps_sys_event_dds_switch_result_type  dds_switch_result,
  void                                *client_info_ptr,
  ps_sys_client_type                   client_id
);

/*===========================================================================
FUNCTION      DS3G_DDS_SWITCH_HDLR_PROCESS_DDS_SWITCH_REQ_CMD

DESCRIPTION   This function performs handles the dds switch IOCLT request from
              the clients and quesries CM if the dds switch is allowed or not

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/

void ds3g_dds_switch_hdlr_process_dds_switch_req_cmd
(
  ds_cmd_type                *cmd_ptr
);

/*===========================================================================
FUNCTION      DS3G_DDS_SWITCH_HDLR_PROCESS_DDS_SWITCH_ALLOWED_ERR_CMD

DESCRIPTION   This function is called when CM reports an error for dds switch
              allowed request

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/

void ds3g_dds_switch_hdlr_process_dds_switch_allowed_err_cmd(void);

/*===========================================================================
FUNCTION      DS3G_DDS_SWITCH_HDLR_PROCESS_DDS_SWITCH_ERR_CMD

DESCRIPTION   This function is called when CM reports an error for dds switch
              request

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/

void ds3g_dds_switch_hdlr_process_dds_switch_err_cmd(void);

/*===========================================================================
FUNCTION      DS3G_DDS_SWITCH_HDLR_IS_CALL_ALLOWED

DESCRIPTION   This function returns if the call is allowed or not to mode handlers

DEPENDENCIES  None

RETURN VALUE  TRUE: if call is allowed
              FALSE: otherwise.

SIDE EFFECTS  None
===========================================================================*/

boolean ds3g_dds_switch_hdlr_is_call_allowed
(
  ps_iface_type  *iface_ptr,
  boolean         is_ims_call
);

/*===========================================================================
FUNCTION      DS3G_DDS_SWITCH_GET_CURR_DDS_SUBS_ID

DESCRIPTION   This function will get current dds sub

DEPENDENCIES  None

RETURN VALUE  sys_modem_as_id_e_type

SIDE EFFECTS  None

===========================================================================*/
sys_modem_as_id_e_type ds3g_dds_switch_get_curr_dds_subs_id(void);
#endif /* DS3G_DDS_SWITCH_HDLR_H */
