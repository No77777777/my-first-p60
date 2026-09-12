/*==========================================================================

                      Diagnostic Packet Definitions

  Description: diagbuffer implementations


Copyright (c) 2016-2017 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            Edit History

  $Header: //components/rel/core.mpss/10.0/services/diag/diagbuffer/src/diagbuffer.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/18/17   ph      Handle the ULOG read index overrun.
09/25/17   ph      Update diagID in the descriptor when sending the data.
03/06/17   ph      diagbuffer API changes and minor updates.
08/17/16   ph      Diag and ULOG integration.
===========================================================================*/

#include "diagbuffer_int.h"
#include "Diag_LSM.h"
#include "diag_qshtrace.h"

/*This holds the recent data drained from a diagbuffer. The data is read from
a diagbuffer handle and save it here. This is passed to send it over the
communication layer (i.e. QSocket here )*/
uint8 payload_diag_send_buf[DIAGBUFFER_MAX_MSG_SIZE];

/**
 * <!-- diagbuffer_set_timestamptype -->
 *
 * @brief Allows the timestamp type to be altered by the user
 *
 *
 * @param h :     A diagbuffer handle.
 * @param type :  Timestamp type
 *
 * @return DAL_SUCCESS if the Timestamp function was correctly set
 *        Errors are defined in the DIAGBUFFER_ERRORS enum.
 */
diagbuffer_result diagbuffer_set_timestamptype(diagbuffer_handle h, uint32 type)
{
  //return error if NULL or Tee handle is passed in
  if(h == NULL || HANDLE_IS_A_TEE((DIAGBUFFER_TYPE*)h))
  {
    return DIAGBUFFER_ERR_INVALIDHANDLE;
  }

  /*Since type is defined as uint32, need not be checked
  for greater than 0 */
  if( type > DIAGBUFFER_TIMESTAMP_TYPE_MAX)
  {
    return DIAGBUFFER_ERR_INVALIDPARAMETER;
  }

  diagbufferInternal_LockGet(((DIAGBUFFER_TYPE*)h));
  ((DIAGBUFFER_TYPE*)h)->param.ts_type = type;
  diagbufferInternal_LockRelease(((DIAGBUFFER_TYPE*)h));

  return DAL_SUCCESS;
}

/*===========================================================================
FUNCTION DIAGBUF_ARE_BUFFERS_EMPTY

DESCRIPTION
  This function checks if ready to drain. If threshold is not reached, it is
  considered empty.

RETURN VALUE
  TRUE  - if the diagbuffer cannot be drained.
  FALSE - if threshold is reached.

===========================================================================*/
boolean DIAGBUFFER_IS_EMPTY(diagbuffer_handle h)
{
  DIAGBUFFER_TYPE* log = (DIAGBUFFER_TYPE *)h;

  /*Uncomment below line once the tool support is in place to enable/disable ULOGS */
  //if( log->param.tool_filter == DIAGBUFFER_TOOL_FILTER_ENABLED )
  {
    if ((log->writerCore->write - log->writerCore->readWriter) > 0 )
    {
        return FALSE;
    }
  }
  return TRUE;
}

/*===========================================================================
FUNCTION DIAGBUFFER_DRAIN

DESCRIPTION
  This function drains a diagbuffer of a given handle. bytes_drained is populated
  with the count of bytes drained from the diagbuffer if successful.

RETURN VALUE
  TRUE  - if the diagbuffer has been drained atleast few bytes.
  FALSE - if error such as diagbuffer is empty or input params are not proper.

===========================================================================*/
boolean diagbuffer_drain(diagbuffer_handle handle, uint32* bytes_drained)
{
    unsigned long int outputFormat;
    diagbuffer_result status;
    uint32 length = 0;

    static diag_send_desc_type send =
    { NULL, NULL, DIAG_SEND_STATE_COMPLETE, TRUE, FALSE, FALSE };

    if(!bytes_drained)
    {
        return FALSE;
    }

    if( !DIAGBUFFER_IS_EMPTY(handle))
    {
      status = ULogDiagFormatPayload(handle, DIAGBUFFER_FORMAT_DIAG, &outputFormat, sizeof(payload_diag_send_buf), (char *)payload_diag_send_buf, bytes_drained);

      length = *bytes_drained;

      if(length > DIAGBUFFER_MAX_MSG_SIZE)
      {
        ERR_FATAL("Copied more bytes than buffer size", 0 ,0 ,0);
      }

      if((DAL_SUCCESS == status) && (length >0))
      {
        send.pkt = &payload_diag_send_buf[0];
        send.last = ((uint8 *)send.pkt) + length - 1;
        send.state = DIAG_SEND_STATE_START;
        send.terminate = TRUE;
        send.priority = FALSE;
        send.is_cmd_rsp_type = FALSE;
        /* Update to local diagID */
        send.diag_id = diag_get_diagID();

        status = diagbuf_send_pkt (&send, DIAG_STREAM_1);
        return TRUE;
      }
      else
      {
        return FALSE;
      }
    }
    return FALSE;
}
/*===========================================================================
FUNCTION DIAGBUFFER_SETUP_HEADER

DESCRIPTION
  This function sets up the header and populates it to the input buffer provided.
  The header would be of diag_cmd_ext_v2_type that tools use to determine the source
  of diagbuffer.

RETURN VALUE
  DAL_SUCCESS  - If the header is setup properly.
                 corresponding error code is returned. Errors are defined in
                 DIAGBUFFER_ERRORS enum.

===========================================================================*/
diagbuffer_result diagbuffer_setup_header(diagbuffer_handle h, void* buf, uint32 size, uint32* length)
{
    diag_cmd_ext_v2_type* v_hdr;
    char temp_buf[DIAGBUFFER_MAX_NAME_SIZE];
    diagbuffer_result result = DAL_ERROR;
    boolean is_offset_negative = FALSE;
    time_type toffset;
    uint64 hi;
    uint32 lo;
    DIAGBUFFER_TYPE *log = (DIAGBUFFER_TYPE *)h;

    if( (log == NULL) || (log->sharedHeader == NULL) ||
        !(log->sharedHeader->logStatus & DIAGBUFFER_STATUS_ENABLED))
    {
      return result;
    }

    if(!length)
    {
      return result;
    }
    if(size < sizeof(diag_cmd_ext_v2_type))
    {
      return result;
    }

    //Update v2 header
    v_hdr = (diag_cmd_ext_v2_type *)buf;
    v_hdr->cmd_code = DIAG_CMD_EXT_F;
    v_hdr->version = DIAGBUF_VERSION_2;
    v_hdr->proc_id = DIAG_MY_PROC_ID;

    /*Sync offset required for QTIMER and DIAG timestamp.*/
    if(log->param.ts_type == DIAGBUFFER_TIMESTAMP_TYPE_QTIMER)
    {
      v_hdr->time_offset_type = DIAG_ULOG_SYNC_OFFSET_TYPE;

      is_offset_negative= time_get_sum_genoff_ts(toffset);

      hi = (uint64)qw_hi(toffset); /*Most significant 32 Bits*/
      lo = qw_lo(toffset);         /*Least Significant 32 Bits*/

      v_hdr->time_offset= ((hi<<32) | lo );

      if(is_offset_negative)
      {
        /*Set MSB in the 64bit value if offset is negative*/
        v_hdr->time_offset|= 0x8000000000000000;
      }
    }
    else
    {
      /*For client custom timestamp, no synchronization needed */
      v_hdr->time_offset_type = DIAG_ULOG_NO_CUSTOM_OFFSET;
      v_hdr->time_offset= 0;
    }
    if ( DAL_SUCCESS == ULogCore_NameGet( h, &temp_buf[0]))
    {
      v_hdr->name_len = strlen(temp_buf);
      strlcpy(v_hdr->ulog_name, temp_buf, (v_hdr->name_len + 1) );
      *length= FPOS(diag_cmd_ext_v2_type, ulog_name) + v_hdr->name_len;
      result = DAL_SUCCESS;
    }
    else
    {
      *length = 0;
      result = DAL_ERROR;
    }

    return result;
}
/**
 * <!-- diagbuffer_set_tx_mode -->
 *
 * @brief Allows the mode of the diagbuffer handle to be altered by clients.
 *  DIAGBUFFER_MODE_STREAM and DIAGBUFFER_MODE_CBUFFER are valid modes that
 *  can be set by the clients.
 *
 * The mode remains default if the user passes invalid mode
 * for the 2nd parameter
 *
 * @param h    : A diagbuffer handle.
 * @param mode : mode to set to the diagbuffer.
 *
 * @return DAL_SUCCESS if the mode was correctly set
 *        Errors are defined in the DIAGBUFFER_ERRORS enum.
 */
diagbuffer_result diagbuffer_set_tx_mode (diagbuffer_handle h, uint8 mode )
{
  diagbuffer_result result = DAL_ERROR;
  DIAGBUFFER_TYPE * log;

  if(mode > DIAGBUFFER_MAX_MODE)
  {
      return result;
  }

  /*TBD-- Validate if handle is proper*/
  log = (DIAGBUFFER_TYPE *)h;

  diagbufferInternal_LockGet(log);

  log->param.tx_mode = mode;

  diagbufferInternal_LockRelease(log);

  if( mode == DIAGBUFFER_MODE_STREAM)
  {
    /* Add buffer to drain queue if not present */
    diagbuf_add_buffer_to_drain(h);
  }
  else
  {
    /* Remove the buffer if exists in drain queue */
    diagbuf_remove_buffer_from_drain(h);
  }
  result = DAL_SUCCESS;
  return result;
}
/**
 * <!-- diagbuffer_set_threshold -->
 *
 * @brief Allows the thresholds such as high_mark and low_mark of the
 *        diagbuffer handle to be altered by clients. high_mark indicates
 *        the threshold at which the data can be drained and low_mark signifies
 *        the point at which draining can be stopped.
 *
 *
 * The mode remains default if the user passes invalid mode
 * for the 2nd parameter
 *
 * @param h        : A diagbuffer handle.
 * @param high_mark: high mark in terms of percentage.
 * @param low_mark : low mark in terms of percentage.
 *
 * @return DAL_SUCCESS if the thresholds are correctly set
 *        Errors are defined in the DIAGBUFFER_ERRORS enum.
 */
diagbuffer_result diagbuffer_set_threshold(diagbuffer_handle h, uint16 high_mark, uint16 low_mark )
{
  DIAGBUFFER_TYPE * log = NULL;

  if( (high_mark<=100) && (low_mark <=100) && (high_mark > low_mark) )
  {
    /*TBD-- Validate if handle is proper*/
    log = (DIAGBUFFER_TYPE *)h;

    diagbufferInternal_LockGet(log);

    log->param.high_mark = high_mark;
    log->param.low_mark  = low_mark;

    diagbufferInternal_LockRelease(log);

    return DAL_SUCCESS;
  }
  else
  {
    return DAL_ERROR;
  }
}
