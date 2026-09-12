#ifndef LOGGING_EXT_H
#define LOGGING_EXT_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            Logging Changes

General Description
This files contains logging changes.


Copyright (c) 2006 - 2016 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
============================================================================

   $Header: //components/rel/wcdma.mpss/8.4.0/api/logging_ext.h#3 $
   $DateTime: 2020/01/24 03:56:36 $
                           Edit History

when       who     what, where, why
--------   ---     ----------------------------------------------------------
1/25/2017  sk      Add logging changes.

===========================================================================*/

#include "customer.h"
#include "diagcmd.h" 
#include "dog_hb_rex.h"
#include "comdef.h"
#include "customer.h"
#include "msg.h"
#include "err.h"
#include "queue.h"
#include "rex.h"
#include "event.h"
#include "sys.h"
#include "log.h"

 /*===========================================================================
FUNCTION     log_utils_log_packet_commit
   
DESCRIPTION
  This function commits the log packets.
   
DEPENDENCIES
  None
   
RETURN VALUE
  None
   
SIDE EFFECTS
  None
   ===========================================================================*/

void log_utils_log_packet_commit(sys_modem_as_id_e_type wrrc_as_id,void* ptr);
/*===========================================================================
FUNCTION     log_utils_log_packet_submit
  
DESCRIPTION
  This function submits the log packets.
  
DEPENDENCIES
  None
  
RETURN VALUE
  boolean
  
SIDE EFFECTS
  None
===========================================================================*/
boolean log_utils_log_packet_submit(sys_modem_as_id_e_type wrrc_as_id,void* ptr);

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

#endif

