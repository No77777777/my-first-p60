/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         Diagnostics over STM

General Description
API's in this file are common functionalities and these API's are used
by DCM and LSM.

Following functionalities are handled here.

DIAG_STM_WRITE_PKT
DIAG_STM_ALLOC
DIAG_STM_FREE
DIAG_STM_LOG_ALLOC
DIAG_STM_LOG_FREE

Copyright (c) 2017-2018 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================
	
							  Edit History
	
$Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/diag_stm_common.c#1 $
	
when	   who	   what, where, why
--------   ---	   ----------------------------------------------------------
03/01/18   sa  	   Added ULogCoreInternal.h header file
===========================================================================*/



#include "comdef.h"
#include "stdlib.h"
#include "tracer.h"

#include "diagdiag_v.h"
#include "diagcmd.h"
#include "diagbuf_v.h"
#include "diagi_v.h"
#include "assert.h"

#include "Diag_LSMi.h"
#include "diag_shared_i.h" /* for definition of diag_data struct. */
#include "Diag_LSM_Qdi_wrapper.h"
#include "Diag_LSM_stm.h"
#include "diag_stm.h"
#include "osal.h"
#include "ULogCoreInternal.h"

extern boolean diag_stm_initialized;
extern boolean diag_pthread_initialized; // TLS key initialized for STM. Note  Must be done before diag_stm_enabled can be set to TRUE.
extern osal_tls_key_t diag_tracer_tls_key;

#define TRACER_PROTOCOL_DIAG 1

#ifdef FEATURE_DIAG_STRESS_PROFILE
uint64 diag_tracer_pcycles_total = 0;
uint32 diag_tracer_byte_total = 0;
#endif /* FEATURE_DIAG_STRESS_PROFILE */

/*===========================================================================

FUNCTION DIAG_STM_WRITE_PKT

DESCRIPTION
  Writes the specified packet to STM using the tracer APIs using the tracer
  handle that exists for this thread in TLS. If a handle does not exist, one
  is created and then stored using TLS.

PARAMETERS
  ptr      Pointer to data to be written
  length   Number of bytes to write from ptr

DEPENDENCIES
  diag_stm_initliazed is TRUE

===========================================================================*/
void diag_stm_write_pkt(void* ptr, uint32 length)
{
  tracer_return_enum_t rval;
  static tracer_options_t diag_tracer_options = TRACER_OPT_TIMESTAMP;
  static uint32 diag_tracer_reg_error_cnt = 0;
  static uint32 diag_tracer_write_error_cnt = 0;
  tracer_client_handle_t client_handle;
  boolean write_flag = TRUE;
  int return_val = 0;

  if(( ptr != NULL) && diag_stm_initialized && diag_stm_enabled && (length > 0))
  {
    /* Check if tracer handle has been written into TLS */
    client_handle = osal_pthread_getspecific(diag_tracer_tls_key);
    if(client_handle == NULL)
    {
      write_flag = FALSE;
      /* No handle exists; need to create a handle for this thread */
      rval = tracer_register(&client_handle, TRACER_ENTITY_DIAG, TRACER_PROTOCOL_DIAG);
      if (rval == TRACER_RETURN_SUCCESS)
      {
        return_val = osal_pthread_setspecific(diag_tracer_tls_key, (void*)client_handle);
        ASSERT(return_val == OSAL_SUCCESS);
        write_flag = TRUE;
      }
      else
      {
        // tracer didn't intiialize -- why?
        // Don't send error MSG, as it may result in an recursive loop
        // Increment debug counter for tracer registration
        diag_tracer_reg_error_cnt++;
      }
    }
    /* Send packet to tracer */
    if(write_flag)
    {
#ifdef FEATURE_DIAG_STRESS_PROFILE
      uint64 pcycle_end, pcycle_start;
      pcycle_start = qurt_get_core_pcycles();
#endif /* FEATURE_DIAG_STRESS_PROFILE */

      rval = tracer_data(client_handle, diag_tracer_options, length, ptr);

      if (rval != TRACER_RETURN_SUCCESS)
      {
        // If write fails, increment debug counter for write failures
        diag_tracer_write_error_cnt++;
      }

#ifdef FEATURE_DIAG_STRESS_PROFILE
      pcycle_end = qurt_get_core_pcycles();
      diag_tracer_pcycles_total += pcycle_end-pcycle_start;
      diag_tracer_byte_total += length;
#endif /* FEATURE_DIAG_STRESS_PROFILE */
    }
  }
}

/*===========================================================================

FUNCTION DIAG_STM_ALLOC

DESCRIPTION
  Allocates a buffer using malloc, intended to be transmitted using the
  tracer APIs and then freed. Packets allocated using this API are not intended to
  be processed by the diag task.

  This is a direct wrapper for malloc (no additional headers added, etc.)
  Intended use case is allocating memory for messages and event reports, where
  diag will be determining the exact size of the final packet.

PARAMETERS
  length      Length of buffer to allocate

DEPENDENCIES


===========================================================================*/
PACK(void *) diag_stm_alloc(uint32 length)
{
  void *ptr = NULL;
  if(length > 0)
  {
    ptr = malloc(length);
  }
  return ptr;
}

/*===========================================================================

FUNCTION DIAG_STM_FREE

DESCRIPTION
  Frees a buffer allocated using diag_stm_alloc
 
PARAMETERS 
  ptr      Pointer to free
 
DEPENDENCIES

===========================================================================*/
void diag_stm_free(void *ptr)
{
  free(ptr);
}
/*===========================================================================

FUNCTION DIAG_STM_LOG_ALLOC

DESCRIPTION
  Allocates a log packet using malloc, intended to be transmitted using the
  tracer APIs and then freed. Logs allocated using this API are not intended to
  be processed by the diag task.

  For consistency in other log handling APIs, the method for allocating the logs
  and the headers used are consistent with the standard diagbuf_alloc function.

  The status member of the diagbuf header is set to DIAGBUF_STM_BUFFER_S to
  allow other Diag APIs to know where the buffer was allocated from.

PARAMETERS
  code                 ID of the log to be reported
  length               The specified length is the length of the entire log,
                       including the log header.
  version              Extended log version number.
                         0 (DIAGBUF_VERSION_0) indicates regular log packet
                         1 (DIAGBUF_VERSION_1) indicates log packet with extended header (v1)
                         2 (DIAGBUF_VERSION_2) indicates log packet with extended header (v2)
  parameter            Parameter associated with extended packets
DEPENDENCIES

===========================================================================*/
PACK(void *) diag_stm_log_alloc (uint8 code, uint16 length, uint8 version, void *parameter)
{
  uint32 alloc_length;
  PACK(void *) ptr = NULL; /* Pointer to return.        */
  diagbuf_header_type *pkt_hdr;
 
  char temp_buf[ULOG_MAX_NAME_SIZE];
  uint32 name_len=0;

  if(length==0)
  {
    /* Return NULL if an empty log packet is being requested */
    return NULL;
  }

    alloc_length =
      sizeof(diagbuf_header_type) +  /* Diagbuf header    */
      (uint32)length;     /* Number of bytes requested buffer */

      // CRC field not needed
      //     sizeof(uint16)    +   /* CRC -- can we drop this for STM? */

    if(version >DIAGBUF_MAX_VERSION_NUM)
    {
        return NULL;
    }

    switch(version)
    {
        case DIAGBUF_VERSION_0:
        case DIAGBUF_VERSION_1:
        case DIAGBUF_VERSION_2:
        {
            /* Allocate addition length needed for header. See comments at top of file for more details
            on how this is used. */
            if ((version >DIAGBUF_VERSION_0) && (parameter == NULL))
            {
                /* No parameter passed with extended packet */
                return NULL;
            }

            if(version == DIAGBUF_VERSION_1)
            {
                alloc_length += sizeof(diag_cmd_ext_v1_type);
            }
            else if(version == DIAGBUF_VERSION_2)
            {
                alloc_length += sizeof(diag_cmd_ext_v2_type);

                if(ULogCore_NameGet( parameter, &temp_buf[0]) == DAL_SUCCESS)
                {
                    /*Match found, get length*/
                    name_len= strlen(temp_buf);

                    if(name_len > ULOG_MAX_NAME_SIZE )
                    {
                        ERR_FATAL("Invalid ULog ", 0, 0, 0);
                    }
                }
                else
                {
                    return NULL;
                }
            }

            ptr = malloc(alloc_length);

            if (ptr)
            {
                /* Set header field of buffer */
                pkt_hdr = ((diagbuf_header_type *) ptr);

                if (version == DIAGBUF_VERSION_0)
                {
                  pkt_hdr->length = length;
                  pkt_hdr->chain = DIAGBUF_NO_CHAIN;
                }
                else if (version == DIAGBUF_VERSION_1)
                {
                  diag_cmd_ext_v1_type* ext_pkt_hdr = (diag_cmd_ext_v1_type*)pkt_hdr;

                  /*Populate the header fields*/
                  ext_pkt_hdr->cmd_code= DIAG_CMD_EXT_F;
                  ext_pkt_hdr->version= DIAGBUF_VERSION_1;
                  ext_pkt_hdr->proc_id=DIAG_MY_PROC_ID;
                  ext_pkt_hdr->id = *((uint32*)parameter);

                  pkt_hdr= (diagbuf_header_type *)((byte*)ext_pkt_hdr + sizeof(diag_cmd_ext_v1_type));

                  pkt_hdr->chain = DIAGBUF_SECOND_PKT_CHAIN;
                  pkt_hdr->length = length;
                }
                else
                {
                  diag_cmd_ext_v2_type* ext_pkt_hdr = (diag_cmd_ext_v2_type*)pkt_hdr;

                  /*Populate the header fields*/
                  ext_pkt_hdr->cmd_code= DIAG_CMD_EXT_F;
                  ext_pkt_hdr->version= DIAGBUF_VERSION_2;
                  ext_pkt_hdr->proc_id=DIAG_MY_PROC_ID;
                  /*Do we need to + 1 for NULL termination ? */
                  ext_pkt_hdr->name_len = name_len;
                  ext_pkt_hdr->time_offset_type = 0xFF;
                  ext_pkt_hdr->time_offset = 0;

                  /*Copy the ULOG name*/
                  strlcpy(ext_pkt_hdr->ulog_name, temp_buf, name_len );

                  pkt_hdr= (diagbuf_header_type *)((byte*)ext_pkt_hdr + sizeof(diag_cmd_ext_v2_type));
                  pkt_hdr->chain = DIAGBUF_SECOND_PKT_CHAIN;
                  pkt_hdr->length = length;
                }

                pkt_hdr->status = DIAGBUF_STM_BUFFER_S;
                /* Note: pkt_hdr->pad not needed for STM implementation, since this allocation is not
                   going into diagbuf */
                pkt_hdr->version = version;

                /* Set 'ptr' to pkt_hdr +1 = data following pkt_hdr */
                ptr = pkt_hdr + 1;

                diagpkt_set_cmd_code (ptr, code);
            }
            break;
        }
        default:
            break;
    }



  return ptr;
} /* diag_stm_log_alloc */

/*===========================================================================

FUNCTION DIAG_STM_LOG_FREE

DESCRIPTION
  Frees a buffer allocated using diag_stm_log_alloc (accounts for diagbuf header)

PARAMETERS
  ptr      Pointer to free

DEPENDENCIES

===========================================================================*/
void diag_stm_log_free(void *ptr)
{
  diagbuf_header_type *pkt_hdr;

  if(ptr)
  {
    pkt_hdr = ((diagbuf_header_type *) ptr) - 1;

    /* Check if this is an extended header */
    if (pkt_hdr->chain == DIAGBUF_SECOND_PKT_CHAIN)
    {
      char* head_ptr = (char*)pkt_hdr;

      /* Only version 1 supported currently */
      ASSERT( (pkt_hdr->version = DIAGBUF_VERSION_1) || (pkt_hdr->version = DIAGBUF_VERSION_2));

      if(pkt_hdr->version == DIAGBUF_VERSION_1)
      {
        head_ptr -= sizeof(diag_cmd_ext_v1_type);
      }
      else if (pkt_hdr->version == DIAGBUF_VERSION_2)
      {
        head_ptr -= sizeof(diag_cmd_ext_v1_type);
      }

      free(head_ptr);
    }
    else
    {
      free(pkt_hdr);
    }
  }
}
