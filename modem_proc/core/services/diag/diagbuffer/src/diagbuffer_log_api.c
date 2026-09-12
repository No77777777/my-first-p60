/*==========================================================================

                      Diagnostic Packet Definitions

  Description: diagbuffer log API implementations


Copyright (c) 2016 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            Edit History

  $Header: //components/rel/core.mpss/10.0/services/diag/diagbuffer/src/diagbuffer_log_api.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/17/16   ph      Diag and ULOG integration.
===========================================================================*/
#include "diagbuffer_int.h"

typedef PACK(struct)
{
  uint8 version;
  void* param;
}diagbuffer_log_ver_type;



/*!
@ingroup log_service
@brief
This function is called to log an accumlated log entry into the respective
diagbuffer. The entry is copied into the buffer immediately.

  A tradeoff must be considered when using this function:

  Pros:
  The caller owns the memory.  Therefore, there is no timing consideration
  involved in filling the contents of that buffer.

  Cons:
  Header contents must be assigned by the caller prior to calling this
  macro.  Therefore, log_set code(), log_set_length() must be used prior
  to this call. log_set_timestamp() may not be necessary since timestmap is
  populated according to the properties of diagbuffer. If log_set_timestamp
  was used on a (non-system) diagbuffer, that time will be overwritten when the
  diagbuffer_log_submit call is made.

  (Resource cost) memory must be allocated by the caller.

  (Resource cost) A copy step is performed (added run-time cost).

  @param[in] *ptr          ptr points to the log which is to be submitted
  @param[in] version       Specifies the version - currently, there are two versions
                           0 - Legacy log packet.
                           1 - The outgoing log packet header will have support to
                               include ID field to differentiate the log pkts
                               coming from different instances of the processor.
  @param[in] *param        Based on the version passed in, this pointer is interpreted
                          as follows-
                          version       pointer
                          0             NULL
                          1             uint32 * . The uint32 value this pointer points to
                                       is the ID of the instance generating the
                                       log packet.

@dependencies

@sideeffects

@return
<ul>
<li> TRUE if log is submitted successfully into diag buffers
<li> FALSE if there is any error.
</ul>

@sa
log_alloc

None
*/
boolean _diagbuffer_log_submit (diagbuffer_handle handle, PACK(void *) log_data, unsigned int version,
                                                                 void * param)
{
  log_hdr_type *hdr_ptr = (log_hdr_type *) log_data;
  uint8 * log_data_payload = NULL;
  log_code_type log_code;
  uint16 * dblPtr;
  uint8 * bytePtr;
  unsigned int length = 0;
  uint32 total_size = 0;
  boolean bReturnVal = FALSE;
  uint32 size =0;
  boolean mask_enabled = FALSE;
  uint32 bufsize_mask;
  uint32 pad;
  uint32 *wordPtr;
  uint32 log_payload_length = 0;
  uint8 param_size = 0;

  if ((hdr_ptr == NULL) || ((version == DIAGBUF_VERSION_1) && (param == NULL))
                        || (version > DIAGBUF_VERSION_1))
  {
    return FALSE;
  }

  log_code = hdr_ptr->code;
  length = hdr_ptr->len;

  if (length <= sizeof(log_hdr_type))
  {
    MSG_1(MSG_SSID_DIAG, MSG_LEGACY_ERROR,"Alloc invalid length %d", length);
    return FALSE;
  }
  else if ((length + LOG_DIAGPKT_OFFSET) > DIAG_MAX_TX_PKT_SIZ)
  {
    MSG_2(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Attempt to allocate %d but Max Length \
                        of a log packet supported is %d",
                        (length + LOG_DIAGPKT_OFFSET), DIAG_MAX_TX_PKT_SIZ);
    return FALSE;
  }

  if ( handle != SYSDIAG_HANDLE )
  {
    DIAGBUFFER_TYPE *log = (DIAGBUFFER_TYPE *) handle;

    /*TBD:Check for masks*/
    //stream_id = log_status_adv(code);
    mask_enabled = TRUE;

    if(mask_enabled > 0)
    {
      bufsize_mask = DIAGBUFFER_BUFSIZE_MASK( log );

      if (version == DIAGBUF_VERSION_0)
      {
        /* No parameters */
        param_size = 0;
      }
      else if (version == DIAGBUF_VERSION_1)
      {
        /* Param field is pointer to a uint32 ID */
        param_size = sizeof(uint32);
      }
      else
      {
        /* Unsupported version */
        return DAL_ERROR;
      }

      log_data_payload = ((uint8*)log_data)+sizeof(log_hdr_type);
      log_payload_length = length - sizeof(log_hdr_type);

      /* format + length + TS + log_code + version + reserved 1 byte (pad)+ param + log_payload_length */
      total_size = 2 + 2 + DIAGBUFFER_TIMESTAMP_SIZE + sizeof(log_code_type) + sizeof(uint8) + sizeof(uint8) + param_size + log_payload_length;

      pad = DIAGBUFFER_PADCOUNT( total_size ) - total_size;

      if (pad >=sizeof(uint32))
      {
          return FALSE;
      }

      size = DIAGBUFFER_BUFSIZE( log );

      if( total_size > size)
      {
        MSG_2(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Attempt to allocate %d but Max \
                                       diagbuffer size is %d", total_size, size);
        return FALSE;
      }

      if (diagbufferInternal_UseFastRam(log))
      {
        uint32 size_idx;
        if ( DIAGBUFFER_FAST_TO_RAM_SUCCESS == diagbufferInternal_FastFmtSizeTS(handle, total_size, DIAGBUFFER_SUBTYPE_DIAG_LOG, &size_idx))
        {
          dblPtr = (uint16 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
          *dblPtr = (uint16)log_code;
          log->writerCore->write += 2;

          /* Version */
          bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
          *bytePtr = (uint8)version;
          log->writerCore->write += 1;

          /* Reserved 1 byte */
          bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
          *bytePtr = 0;
          log->writerCore->write += 1;

          /* Param */
          if (version == 1)
          {
              wordPtr = (uint32 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
              *wordPtr = *((uint32 *)param);
              log->writerCore->write += 4;
          }

          /* Log data */
          bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);

          if ((((log->writerCore->write) & (bufsize_mask)) + log_payload_length) <= (log->bufSize))
          {
            /*if the packet can fit in the buffer*/
            memscpy( bytePtr, log->bufSize-(log->writerCore->write & bufsize_mask), log_data_payload, log_payload_length );
            log->writerCore->write  += log_payload_length;
          }
          else
          {
            /*wrap around case*/
            uint32 first_part_length = log->bufSize - (log->writerCore->write & bufsize_mask);
            uint32 second_part_length = log_payload_length - first_part_length;
            memscpy( bytePtr, (log->bufSize - (log->writerCore->write & bufsize_mask)), log_data_payload, first_part_length );
            log->writerCore->write  += first_part_length;
            bytePtr =  (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
            memscpy( bytePtr, (log->writerCore->readWriter & bufsize_mask), ((byte *)log_data_payload + first_part_length), second_part_length );
            log->writerCore->write  += second_part_length;
          }

          /*add padding at the end of the msg to round it to the next word boundary */
          log->writerCore->write += pad;

          /* Update the message length */
          wordPtr = (uint32 *)&(log->buffer[size_idx & bufsize_mask]);
          *wordPtr |= ((total_size << DIAGBUFFER_LENGTH_SHIFT) & DIAGBUFFER_LENGTH_MASK);

          /* Release the lock, acquired within FastFmtSizeTS */
          diagbufferInternal_LockRelease(log);

          bReturnVal = TRUE;

        }
      }
    }
  }
  else
  {
    bReturnVal = log_submit_ext(log_data, version, param);
  }

  return bReturnVal;
}

/*!
@ingroup log_service
@brief
Allocate a buffer of size 'length' for logging data. The allocation happens from system heap
for a valid diagbuffer handle and from system diagbuffer for a system diag handle.
This returns the pointer to memory location for clients to update the payload of log packet.

@param h                        Handle to the diagbuffer.
                                if "SYSDIAG_HANDLE" is passed, then log gets allocated
                                from the system diag buffer.
@param[in] code                 ID of the log to be reported
@param[in] length               The specified length is the length of the entire log,
                                including the log header.
@param[in] version              Specifies the version - currently, there are two versions
                                0 - Legacy log packet
                                1 - The outgoing log packet header will have support to
                                    include ID field to differentiate the log pkts
                                    coming from different instances of the processor.
@param[in] *param               Based on the version passed in, this pointer is interpreted
                                as follows-
                                version       pointer
                                0             NULL
                                1             uint32 * . The uint32 value this pointer points to,
                                              is the ID of the instance generating the
                                              log packet

 @dependencies
 None

@sideeffects
Since this allocation is made from a shared resource pool, log_commit()
or log_free() must be called as soon as possible and in a timely fashion.
This allocation system has no garbage collection. The actual data gets copied into
the diagbuffer only when client calls log_commit().

@return
<ul>
<li>A pointer to the allocated buffer is returned on success.
<li>If the log code is disabled or there is not enough space, NULL is returned.
</ul>
@sa
log_submit, log_commit
*/
PACK(void*) _diagbuffer_log_alloc(diagbuffer_handle handle, log_code_type code,
                              unsigned int length, unsigned int version, void * param)
{
  log_header_type *hdr_ptr = NULL;
  void *return_ptr = NULL;
  uint8 *log_pkt;
  uint32 total_len;
  uint8* ovr_ptr;
  uint8* copy_ptr;
  diagbuffer_log_ver_type* log_ver_ptr;

  if (length <= sizeof(log_header_type))
  {
    MSG_1(MSG_SSID_DIAG, MSG_LEGACY_ERROR,"Alloc invalid length %d", length);
    return NULL;
  }
  else if ((length + LOG_DIAGPKT_OFFSET) > DIAG_MAX_TX_PKT_SIZ)
  {
    MSG_2(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Attempt to allocate %d but Max Length of a log packet supported is %d", (length + LOG_DIAGPKT_OFFSET), DIAG_MAX_TX_PKT_SIZ);
    return NULL;
  }
  if ( ((version == DIAGBUF_VERSION_1) && (param == NULL))
                        || (version > DIAGBUF_VERSION_1))
  {
    return NULL;
  }

  if(handle == SYSDIAG_HANDLE)
  {
    /*Allocate from system diag buffer */
    return_ptr = log_alloc_ext (code, length, version, param);
  }
  else
  {
    /*Allocate memory of length plus the overrun pattern plus log_version_type.
    In the pointer given to client data would be of below format
    |log header|client payload | 0xDEAD | version | Parameter */
    total_len = length + sizeof(uint16) + sizeof(diagbuffer_log_ver_type) ;

    log_pkt = (uint8 *)malloc(total_len);
    if (log_pkt != NULL)
    {
      hdr_ptr = (log_header_type *)log_pkt;
      log_set_length(hdr_ptr, length);
      log_set_code(hdr_ptr, code);

      /*Write overrun pattern to avoid corruption*/
      ovr_ptr = ((byte *)hdr_ptr) + length;
      *((byte *)ovr_ptr) = 0xDE;
      *((byte *)ovr_ptr + 1) = 0xAD;

      copy_ptr = ((byte *)hdr_ptr) + length + sizeof(uint16);

      log_ver_ptr = (diagbuffer_log_ver_type *)copy_ptr;

      log_ver_ptr->version = (uint8) version;

      if( version == DIAGBUF_VERSION_1)
      {
        log_ver_ptr->param = param;
      }
      else
      {
        log_ver_ptr->param = NULL;
      }

      return_ptr = (void *)hdr_ptr;
    }
  }
  return return_ptr;
}
/*!
@ingroup log_service
@brief
This function commits a log buffer allocated by log_alloc(). Calling this
function tells the logging service that the user is finished with the allocated buffer
and diagnostics layer can transfer this packet over the underlying transport layer.
It is only after log_commit() call, the client data gets copied into the client diagbuffer
and if it is a system diagbuffer, it is marks the log packet as committed since the data was
already been copied in log_alloc() call.


@param h                Handle to the diagbuffer.
                        if "SYSDIAG_HANDLE" is passed, then log commit happens for
                        the log allocated from system diagbuffer.
@param[in] *log_data    ptr pointing to the log which is being commited


@dependencies
'ptr' must point to the address that was returned by a prior call to log_alloc().

@sideeffects
Since this allocation is made from a shared resource pool, this must be
called as soon as possible after a log_alloc call.  This operation is not
intended for logs that take considerable amounts of time.

@return
None

@sa
None
*/
void _diagbuffer_log_commit (diagbuffer_handle handle, PACK(void *) log_data)
{
  log_header_type *hdr_ptr = (log_header_type *)log_data;
  uint32 length;
  uint8 *pattern = NULL;
  diagbuffer_log_ver_type* log_ver_ptr;


  if(handle == SYSDIAG_HANDLE)
  {
    log_commit(log_data);
  }
  else
  {
    if(hdr_ptr)
    {
      hdr_ptr = (log_header_type *)log_data;
      length = hdr_ptr->len;
      pattern = ((byte*)hdr_ptr)+length;

      if ( (*pattern != 0xDE) || ((*(pattern + 1) != 0xAD)) )
      {
        ERR_FATAL("_diagbuffer_log_commit: Log code %04X has overwritten the allocated memory in diagbuffer 0x%x",
                                log_get_code((void *)hdr_ptr), handle, 0);
      }

      log_ver_ptr = (diagbuffer_log_ver_type *) ((uint8*) pattern + sizeof(uint16));

      _diagbuffer_log_submit(handle, log_data, (uint32)(log_ver_ptr->version), log_ver_ptr->param);

      /*Free the memory given to client */
      free(log_data);


    }
  }
}
