/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
           W I R E L E S S    M E S S A G I N G   S E R V I C E S
           wmsutils.c --

  Implementations of common internal WMS functions.


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

  $Header: //components/rel/mmcp.mpss/7.9.0/wms/src/WMSE/wmsutils.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
========================  INCLUDE FILES =====================================
===========================================================================*/
#include "string.h"
#include "wmsutils.h"
#include "err.h"
#include "msg.h"
#include "modem_mem.h"
#include "mm.h"

/*===========================================================================
======================== STATIC DATA ========================================
===========================================================================*/

extern void log_util_log_packet_commit(sys_modem_as_id_e_type as_id, void* ptr);


/*===========================================================================
============================ FUNCTIONS ======================================
===========================================================================*/

/*===========================================================================
FUNCTION wms_mem_malloc

DESCRIPTION
  Function allocates memory from heap and memory would be initialized to zero

DEPENDENCIES
  None

RETURN VALUE
  void pointer

SIDE EFFECTS
  None
===========================================================================*/
void * wms_mem_malloc(
  size_t                             size
)
{
  void *mem_ptr = NULL;

  if (size > 0)
  {
    mem_ptr = modem_mem_alloc( size, MODEM_MEM_CLIENT_WMS );
  }

  if (NULL != mem_ptr)
  {
    memset(mem_ptr, 0, size);
  }
  //MSG_HIGH_2( "malloc @0x%x, size %d", mem_ptr, size );

  return mem_ptr;
} /* wms_mem_malloc() */


/*===========================================================================
FUNCTION wms_mem_free

DESCRIPTION
  Function free memory back to heap.

DEPENDENCIES
  Null pointer checking on mem_ptr must be done before calling this function.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wms_mem_free(
  void                              *mem_ptr
)
{
  //MSG_HIGH_1( "free @0x%x", mem_ptr );
  modem_mem_free( mem_ptr, MODEM_MEM_CLIENT_WMS );
} /* wms_mem_free() */


void wms_check_for_null_ptr (void)
{
   MSG_ERROR_0("Null Pointer");
}





void wms_log_event_notify_packet(uint8 clid,uint8 ev_id,uint8 notify)
{

   LOG_WMS_EVENT_NOTIFY_LOG_PACKET_C_type* log_p;
   log_p = (LOG_WMS_EVENT_NOTIFY_LOG_PACKET_C_type*)log_alloc(LOG_WMS_EVENT_NOTIFY_LOG_PACKET_C,
               sizeof(LOG_WMS_EVENT_NOTIFY_LOG_PACKET_C_type));
   if (log_p)
   {
     log_p->version           = (uint8)1;
     log_p->client_id         =  clid;
     log_p->event_id          =  ev_id;
	 log_p->event_id          =  notify;
     
     log_util_log_packet_commit(SYS_MODEM_AS_ID_NONE, log_p);
   }

}


void wms_log_msg_ind_notify_packet(uint16 cmd_type,uint8 client_id,uint8 status)
{
  LOG_WMS_MSG_IND_NOTIFY_LOG_PACKET_C_type* log_p;
  log_p = (LOG_WMS_MSG_IND_NOTIFY_LOG_PACKET_C_type*)log_alloc(LOG_WMS_MSG_IND_NOTIFY_LOG_PACKET_C,
  				sizeof(LOG_WMS_MSG_IND_NOTIFY_LOG_PACKET_C_type));
  if (log_p)
  {
    log_p->version			 = (uint8)1;
    log_p->cmd_type          = cmd_type;
    log_p->client_id 	     = client_id;
    log_p->status            = status;
    log_util_log_packet_commit(SYS_MODEM_AS_ID_NONE, log_p);
  }


}




