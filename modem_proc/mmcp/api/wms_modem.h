#ifndef WMS_MODEM_H
#define WMS_MODEM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
           W I R E L E S S    M E S S A G I N G   S E R V I C E S
           wms_modem.h -- Internal modem api definitions

  This file contains WMS API function prototypes used internally within
  the MODEM AU.

  -------------

    Copyright (c) 2010-2012 QUALCOMM Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Confidential and Proprietary

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

  $Header: //components/rel/mmcp.mpss/7.9.0/api/wms_modem.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include "wms.h"
#include "nv.h"
#include <msgr.h>
#include "wms_msgr_msg.h"

typedef struct
{
  wms_bc_pref_e_type                  bc_pref;
  boolean                             bc_enable; /* BC is actually enabled or not */
  uint16                              bc_table_size;
  nv_sms_bc_service_table_type        bc_table[WMS_BC_TABLE_MAX];
} wms_1x_bc_config_s_type;

typedef struct
{
  msgr_hdr_s                     msg_hdr;        // Message router header 
  sys_modem_as_id_e_type         as_id;          // subscription id 
  wms_msg_transport_type_e_type  transport_id;   // Bearer transport type
  wms_sms_state_e_type           state;          // SMS state
} wms_sms_status_ind_s_type;

/*===========================================================================
FUNCTION wms_get_1x_bc_config

DESCRIPTION
  Return 1X BC configuration information.

DEPENDENCIES
  none

RETURN VALUE
  TURE or FALSE

SIDE EFFECTS
  None.
===========================================================================*/
boolean wms_get_1x_bc_config(
  wms_1x_bc_config_s_type            *bc_conf_ptr
);

/*=========================================================================
FUNCTION wms_bc_addr_match

DESCRIPTION
  This function checks the BC_ADDR of a received broadcast page or
  broadcast message to see if it passes the filter.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - the mobile should process the received message
  FALSE - the mobile should discard the received message

SIDE EFFECTS
  None

=========================================================================*/
boolean wms_bc_addr_match(
  const uint8                        *bc_addr_ptr
);

/*=========================================================================
FUNCTION wms_bc_gw_cb_filter

DESCRIPTION
  This function checks the first block of the CB page
  to see if it passes the filter.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - the mobile should process the received message
  FALSE - the mobile should discard the received message

SIDE EFFECTS
  None

=========================================================================*/
boolean wms_bc_gw_cb_filter(
  sys_modem_as_id_e_type  as_id,
  uint8                  *first_block
);

/*=========================================================================
FUNCTION wms_ts_parse_CDMA_bc                          (INTERNAL MODEM API)

DESCRIPTION
  Parses the CDMA OTA broadcast message and returns the service category
  and the message identifier.

DEPENDENCIES
  None

RETURN VALUE
  WMS_OK_S: parse was successful and values have been returned
  Other: failure code of decode process.

SIDE EFFECTS
  None
=========================================================================*/
wms_status_e_type wms_ts_parse_CDMA_bc(
  wms_ota_msg_s_type                 *OTA_msg_to_parse,  /* IN  */
  wms_service_e_type                 *service_id,        /* OUT */
  uint16                             *message_id         /* OUT */
);

/*=========================================================================
FUNCTION wms_mn_mo_sms_callback                        (INTERNAL MODEM API)

DESCRIPTION
  API to be used by NAS MN to post RP-Packet to IMS through WMS

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=========================================================================*/
wms_status_e_type wms_mn_mo_sms_callback(void *msg_ptr);

#endif /* WMS_MODEM_H */


