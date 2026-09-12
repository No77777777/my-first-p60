#ifndef DS3GDIAGMGR_H
#define DS3GDIAGMGR_H
/*===========================================================================

                3 G   D A T A   S E R V I C E S   D I A G   M A N A G E R

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for 3G Data Services diag handling.
  It provides APIs for DS3G diag registration and processing.

  Copyright (c) 2015-2018 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ds3gex/inc/ds3gdiagmgr.h#4 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
07/07/15   fj      Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA_DIAG_SUPPORT
#include "diagpkt.h"
#include "diagcmd.h"
#include "diagdiag.h"
#include "dstask_v.h"

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* Diag subsystem req codes currently supported */
#define	DS3GDIAGMGR_SUBSYS_CODE_REQUEST_ITEM 0x0

/* Diag cmd definitions */
typedef enum
{
  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_MIN                               = 0,
  /* 3GPP cmd code req items - 0x1000 to 0x1FFF */
  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_SET_WM_PARAMS                = 0x1000,

  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_SEND_TPUT_CAT_IND            = 0x1001,

  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_SEND_LOOPBACK_PARAMS_IND     = 0x1002,

#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif/* FEATURE_LAPP_UFS */

  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_SEND_RAB_MESSAGE             = 0x100A,

  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_SEND_SUSPEND_RESUME_MESSAGE  = 0x100B,

#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /* FEATURE_LAPP_UFS */

#ifdef FEATURE_DATA_FLOW_MGMT
  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_LOG_FLOW_CNTRL_LATENCY       = 0x100D,

  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_LOG_FLOW_CNTRL_THRESHOLD     = 0x1013,
#endif /* FEATURE_DATA_FLOW_MGMT */

  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_DEQUEUE_CFG_CMD              = 0x100E,

  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_POST_DEQUEUE_OPS_CMD         = 0x100F,

  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_FLOW_CONTROL_PARAMS_CMD        = 0x1010,

  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_SEND_VOLTE_SWITCH_IND        = 0x1011,
  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_SEND_NV_FRESH_CMD            = 0x1012,
#ifdef QWES_FEATURE_ENABLE
  DS_APPSRV_CMD_CODE_REQ_ITEM_CAAS_SET_CLIENT                     = 0x1040,

  DS_APPSRV_CMD_CODE_REQ_ITEM_CAAS_SET_COMM                       = 0x1041,

  DS_APPSRV_CMD_CODE_REQ_ITEM_CAAS_SET_DEF_ALLOWED_STATUS         = 0x1042,

  DS_APPSRV_CMD_CODE_REQ_ITEM_CAAS_GET_FEATURES                   = 0x1045,

  DS_APPSRV_CMD_CODE_REQ_ITEM_CAAS_OPERATION                      = 0x1051,

  DS_APPSRV_CMD_CODE_REQ_ITEM_CAAS_REG_FEAT_STAT                  = 0x1052,
#endif  /* QWES_FEATURE_ENABLE */
  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_MAX                               = 0xFFFF,
} ds3gdiagmgr_cmd_reqitem_code_type;

/* ds3g diag cmd payload structure */
typedef PACKED struct PACKED_POST 
{
  /* cmd_code */
  uint16  cmd_code;
  /* CMD data can be used for different purposes depending on the cmd_code*/
  uint32 cmd_data_1;
  uint32 cmd_data_2;
  uint32 cmd_data_3;
  uint32 cmd_data_4;
  uint32 cmd_data_5;
  uint32 cmd_data_6;
} ds3gdiagmgr_req_payload_type;

/* diag req type structure */
typedef PACKED struct PACKED_POST 
{
  /* Sub System header */
  diagpkt_subsys_header_type   header;
  /* Payload sent for the requested cmd */
  ds3gdiagmgr_req_payload_type payload;     
} ds3gdiagmgr_req_type;

/* diag response type structure */
typedef PACKED struct PACKED_POST 
{
  /* Sub System header */
  diagpkt_subsys_header_type  header;   
  /* echo back the cmd code process status */
  boolean                     status;
  /* Payload sent for the requested cmd */
  ds3gdiagmgr_req_payload_type payload;
} ds3gdiagmgr_rsp_type;

/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*===========================================================================
FUNCTION      DS3GDIAGMGR_INIT

DESCRIPTION   This function initializes the ds3gdiagmgr module and registers 
              ds3g diag cmds with the diag framework.
 
DEPENDENCIES  None 

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gdiagmgr_init(void);

/*===========================================================================
FUNCTION      DS3GDIAGMGR_CMD_HDLR

DESCRIPTION   This function is the callback called by diag framework to 
              process DS3G diag cmds. 
 
DEPENDENCIES  None

RETURN VALUE  A void* response pointer back to diag

SIDE EFFECTS  None
===========================================================================*/
void* ds3gdiagmgr_cmd_hdlr
(
  void*  diag_msg_ptr,
  uint16 diag_msg_len
);

/*===========================================================================
FUNCTION      DS3GDIAGMGR_PROCESS_DS_CMD

DESCRIPTION   This function handles DS_CMD_DIAG_REQ from DS task. 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3gdiagmgr_process_ds_cmd
(
  ds_cmd_type  *cmd_ptr
);
#endif /*FEATURE_DATA_DIAG_SUPPORT*/

#endif /* DS3GDIAGMGR_H */
