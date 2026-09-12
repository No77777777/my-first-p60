/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         Diagnostics over QSHTRACE

General Description
This routines in this file are used to send mtrace data over STM using the debugtrace
interfaces or over diagbuf.

Copyright (c) 2016-2017 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                              Edit History

 $Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/diag_qshtrace.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/01/17   gn     Added NULL checks for diag_qshtrace_header32_ptr and diag_qshtrace_ptr
10/05/16   sa     File creation
===========================================================================*/


#include "comdef.h"
#include "tracer.h"
#include "diag_stm.h"
#include "diag_qshtrace.h"
#include "diag_qshtrace_i.h"
#include "msg.h"
#include "osal.h"
#include "assert.h"
#include "diagbuf_v.h"
#include "diagcmd.h"
#include "diagdiag_v.h"
#include "ULogFront.h"

#include <stringl/stringl.h>

extern ULogHandle diag_err_log_handle;

extern osal_tls_key_t diag_tracer_tls_key;

//Mode of qshtrace set by the client.
boolean diag_qshtrace_enabled_by_client=FALSE;


//Function pointer to forward traffic to STM or diagbuf based on modes enabled
void (*diag_qshtrace_ptr)(void *hdr_ptr,uint32 hdr_len,void *data_ptr,uint32 data_len);
void (*diag_qshtrace_header32_ptr)(uint32 header,void *data_ptr,uint32 data_len);

/*===========================================================================


FUNCTION  DIAG_QSHTRACE_WRITE_STM

DESCRIPTION
  This is the function called when STM as well as QSHTRACE is enabled. 
  It will call the trace API to write the data over QDSS.

PARAMETERS
  ptr     Pointer to the data to be written
  length  Length of the data to be send 

DEPENDENCIES
  None

RETURN
  None

===========================================================================*/

static void diag_qshtrace_write_stm(void *hdr_ptr, uint32 hdr_len, void *data_ptr, uint32 data_len)
{
  tracer_return_enum_t rval;
  tracer_client_handle_t client_handle;

  /*No check for ptr or length is done to avoid any check and do fast write*/
  client_handle = osal_pthread_getspecific(diag_tracer_tls_key);
  if((__builtin_expect(client_handle == NULL,1)))
    {
      int return_val = 0;
	  static int reg_fail=1;
      /* No handle exists; need to create a handle for this thread */
      rval = tracer_register(&client_handle, TRACER_ENTITY_DIAG, TRACER_PROTOCOL_DIAG);
      if (rval == TRACER_RETURN_SUCCESS)
      {
        return_val = osal_pthread_setspecific(diag_tracer_tls_key, (void*)client_handle);
        ASSERT(return_val == OSAL_SUCCESS);
      }
      else
      	{
      	  ULogFront_RealTimePrintf(diag_err_log_handle, 1, "diag_qshtrace_write_stm REGISTRATION FAILED",reg_fail++);
      	  return;
		}  

    }

  rval = tracer_data_start(client_handle, TRACER_OPT_TIMESTAMP , hdr_len, hdr_ptr);
  rval = tracer_data_finish(client_handle, TRACER_OPT_TIMESTAMP , data_len, data_ptr);  
}

/*===========================================================================


FUNCTION  DIAG_QSHTRACE_HEADER32_WRITE_STM

DESCRIPTION
  This is the function called when STM as well as QSHTRACE is enabled. 
  It will call the trace API to write the data over QDSS with a 32bit header

PARAMETERS
  header  32bit header value
  ptr     Pointer to the data to be written
  length  Length of the data to be send 

DEPENDENCIES
  None

RETURN
  None

===========================================================================*/

static void diag_qshtrace_header32_write_stm(uint32 header, void *data_ptr, uint32 data_len)
{
  tracer_return_enum_t rval;
  tracer_client_handle_t client_handle;

  /*No check for ptr or length is done to avoid any check and do fast write*/
  client_handle = osal_pthread_getspecific(diag_tracer_tls_key);
  if((__builtin_expect(client_handle == NULL,1)))
    {
      int return_val = 0;
	  static int reg_fail=1;
      /* No handle exists; need to create a handle for this thread */
      rval = tracer_register(&client_handle, TRACER_ENTITY_DIAG, TRACER_PROTOCOL_DIAG);
      if (rval == TRACER_RETURN_SUCCESS)
      {
        return_val = osal_pthread_setspecific(diag_tracer_tls_key, (void*)client_handle);
        ASSERT(return_val == OSAL_SUCCESS);
      }
      else
      	{
      	  ULogFront_RealTimePrintf(diag_err_log_handle, 1, "diag_qshtrace_write_stm REGISTRATION FAILED",reg_fail++);
      	  return;
		}  

    }

  rval = tracer_header32_and_data(client_handle, TRACER_OPT_TIMESTAMP , header, data_len, data_ptr);

}

/*===========================================================================

FUNCTION DIAG_QSHTRACE_WRITE_DIAGBUF

DESCRIPTION
  This function is called if either STM is disabled but QSHTRACE is enabled.

PARAMETERS
  header_ptr Pointer to the header to be written
  header_len Length of the header
  data_ptr   Pointer to the data to be written
  data_len   Length of the data to be send 

DEPENDENCIES
  None

RETURN
  None

===========================================================================*/

static void diag_qshtrace_write_diagbuf(void *header_ptr, uint32 header_len, void *data_ptr, uint32 data_len)
{
  uint8* qshtrace_packet = NULL;
  uint32 length = (header_len + data_len);

  /* Verify packet is of the correct type */
  if (((uint8*)header_ptr)[0] != DIAG_QSH_TRACE_PAYLOAD_F)
  {
    /* Invalid packet */
    return;
  }

  if (!diag_qshtrace_enabled_by_client)
  {
    /* QSHTrace packets not enabled */
    return;
  }

  qshtrace_packet = (uint8*)diagbuf_alloc(DIAG_QSH_TRACE_PAYLOAD_F, length, 0, DIAGBUF_ALLOC_REQUEST_QSHTRACE, 0, 0);

  if (qshtrace_packet)
  {
    memscpy((void*)qshtrace_packet, length, header_ptr, header_len);
    memscpy((void*)(qshtrace_packet+header_len), (length-header_len), data_ptr, data_len);
    diagbuf_commit (qshtrace_packet);
  }

  return;  
}


/*===========================================================================

FUNCTION DIAG_QSHTRACE_HEADER32_WRITE_DIAGBUF

DESCRIPTION
  This function is called if either STM is disabled but QSHTRACE is enabled.

PARAMETERS
  header  4 byte header value
  ptr 	  Pointer to the data to be written
  length  Length of the data to be send 

DEPENDENCIES
  None

RETURN
  None

===========================================================================*/
static void diag_qshtrace_header32_write_diagbuf(uint32 header, void *data_ptr, uint32 data_len)
{
  /* Diagbuf path can be rerouted to the generic API */
  diag_qshtrace_write_diagbuf(&header, sizeof(header), data_ptr, data_len);
}

//Public api: Function to write data to STM/dIAGBUF
void diag_qshtrace_write(void *hdr_ptr, uint32 hdr_len, void *data_ptr, uint32 data_len)
{
  if(diag_qshtrace_ptr)
  {
  	diag_qshtrace_ptr(hdr_ptr, hdr_len, data_ptr, data_len);
  }
  else
  {
    ULogFront_RealTimePrintf(diag_err_log_handle,0,"diag_qshtrace_write : diag_qshtrace_enable is not called by the client");
  }  
}

void diag_qshtrace_header32_write(uint32 header, void *data_ptr, uint32 data_len)
{
  if(diag_qshtrace_header32_ptr)
  {
  	diag_qshtrace_header32_ptr(header, data_ptr, data_len);
  }
  else
  {
  	ULogFront_RealTimePrintf(diag_err_log_handle,0,"diag_qshtrace_header32_write : diag_qshtrace_enable is not called by the client");
  }
}


//internal: SWITCHES THE FUNCTION POINTER to diagbuf or STM
uint8 diag_qshtrace_config_mode()
{
   
  //Check these conditions and then switch fn ptr.
  //Only if all the 3 meet, we should switch to sending qshtrace data onto STM
  //1.diag_stm_enabled=TRUE;
  //2. diag_stm_initialised=TRUE;
  //3.diag_qshtrace_enabled_by_client=TRUE;
  if (diag_stm_enabled && diag_stm_initialized && diag_qshtrace_enabled_by_client)
  {
    diag_qshtrace_ptr= &diag_qshtrace_write_stm;
    diag_qshtrace_header32_ptr = &diag_qshtrace_header32_write_stm;    
    diag_qshtrace_mode=DIAG_QSHTRACE_MODE_STM;
  }
  else
  {
    diag_qshtrace_ptr=&diag_qshtrace_write_diagbuf;
    diag_qshtrace_header32_ptr = &diag_qshtrace_header32_write_diagbuf;        
	diag_qshtrace_mode=DIAG_QSHTRACE_MODE_DIAGBUF;
  }    
  return diag_qshtrace_mode;
}


//PUBLIC: Called by clients to enable/disable QSHTRACE
void diag_qshtrace_enable(boolean mode)
{
  diag_qshtrace_enabled_by_client=mode;
  diag_qshtrace_config_mode();
}




