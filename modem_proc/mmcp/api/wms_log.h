
#ifndef WMS_LOG_H
#define WMS_LOG_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
           W I R E L E S S    M E S S A G I N G   S E R V I C E S
           wms_log.h --

  Implementations wms log packets related Struct element


  -------------

    Copyright (c) 2010-2011 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/api/wms_log.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================
========================  INCLUDE FILES =====================================
===========================================================================*/
#include "mmcp_variation.h"
#include<customer.h>
#include "comdef.h"
#include "log_codes_gsm.h"



#include "log.h"

LOG_RECORD_DEFINE(LOG_WMS_EVENT_NOTIFY_LOG_PACKET_C)

uint8 version;

uint8 client_id;

uint8 event_id;

uint8 notification;

LOG_RECORD_END

LOG_RECORD_DEFINE(LOG_WMS_MSG_IND_NOTIFY_LOG_PACKET_C)
uint8 version;

uint16 cmd_type;

uint8 client_id;

uint8 status;

LOG_RECORD_END




#endif

