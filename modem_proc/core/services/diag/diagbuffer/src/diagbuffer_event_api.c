/*==========================================================================

                      Diagnostic Packet Definitions

  Description: diagbuffer Event API implementations


Copyright (c) 2016 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            Edit History

  $Header: //components/rel/core.mpss/10.0/services/diag/diagbuffer/src/diagbuffer_event_api.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/17/16   ph      Diag and ULOG integration.
===========================================================================*/
#include "diagbuffer_int.h"
#include "event.h"
#include "eventi.h"


/*!
   @ingroup event_service

   @brief
   Writes a Diag event packet to the specified log handle. Users are recommended
   to use the macro rather than the API.

   @param handle : Handle to the diagbuffer.
                   if "SYSDIAG_HANDLE" is passed, then event gets allocated
                   in event heap of DIAG.

   @param event_id : Event ID to log, as defined in event_id_enum_type

   @param[in] length : length of the associated  payload. User need to to pass 0 if
                       there is no payload for the event.

   @param[in] *payload : Pointer to the payload structure.Payload should be published.
                         Please refer to 80-V6196-1 for details. Payload to be passed as
                         NULL if there is no payload associated for the event.
                         (Length to be passed as 0 and payload NULL for events without payload)

   @param version : Version number to interpret data type stored in 'param'

   @param param : If version == 0, param is ignored.
                  If version == 1, param is a pointer to a uint32 ID.

   @dependencies
   None

   @sideeffects
   None

   @return
   TRUE on success, FALSE on error

   @sa
   None

 */
boolean _diagbuffer_event_payload(diagbuffer_handle handle, event_id_enum_type event_id,
                                      uint8 length, void *payload,
                                      unsigned int version, void *param)
{
  boolean ret_value = FALSE;
  boolean mask_enabled = FALSE;
  uint16 * dblPtr;
  uint8 * bytePtr;
  uint32 *wordPtr;
  uint32 size;
  uint8 param_size = 0;
  uint8 pad = 0;
  uint32 bufsize_mask = 0;
  uint32 total_size =0;
  uint32 msg_format=0;
  uint8 payload_len =0;


  if (version > DIAGBUF_MAX_VERSION_NUM)
  {
    return FALSE;
  }

  if ((version == DIAGBUF_VERSION_1) && param == NULL)
  {
    return FALSE;
  }

  if ((length >0) && (payload != NULL))
  {
    msg_format = DIAGBUFFER_SUBTYPE_DIAG_EVENT_PAYLOAD;
    payload_len = length;
  }
  else
  {
    /*Make payload length as 0 just in case client calls with non-zero length*/
    payload_len = 0;
    msg_format = DIAGBUFFER_SUBTYPE_DIAG_EVENT;
  }

  /*TBD:Check for masks*/
  //stream_id = log_status_adv(code);
  mask_enabled = TRUE;

  if(mask_enabled)
  {
    if(handle != SYSDIAG_HANDLE)
    {
      /* Check if this is an extended report and allow space for header */
      if (version == 0)
      {
        /* No parameters */
        param_size = 0;
      }
      else if (version == 1)
      {
        /* Param field is pointer to a uint32 ID */
        param_size = sizeof(uint32);
      }
      else
      {
        /* Unsupported version */
        return FALSE;
      }

      if (handle != SYSDIAG_HANDLE)
      {
        DIAGBUFFER_TYPE *log = (DIAGBUFFER_TYPE *) handle;

        bufsize_mask = DIAGBUFFER_BUFSIZE_MASK( log );

        /* format + length + TS + event_id + version + reserved 1 byte (Pad) + param  + payload length */
        total_size = 2 + 2 + DIAGBUFFER_TIMESTAMP_SIZE + sizeof(event_id_enum_type) + sizeof(uint8) + sizeof(uint8) + param_size + payload_len;

        pad = DIAGBUFFER_PADCOUNT( total_size ) - total_size; //add padding at the end of the msg to round it to the next word boundary
        /*pad should not be greater than 4*/
        if (pad >=sizeof(uint32))
        {
          return FALSE;
        }

        size = DIAGBUFFER_BUFSIZE( log );

        if( total_size > size )
        {
          MSG_2(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Event:Attempt to allocate %d but Max Length \
                  of a log packet supported is %d",
                  total_size, size);
          return FALSE;
        }

        if (diagbufferInternal_UseFastRam(log))
        {
          uint32 size_idx;
          if ( DIAGBUFFER_FAST_TO_RAM_SUCCESS == diagbufferInternal_FastFmtSizeTS(handle, total_size, msg_format, &size_idx))
          {
            /* event id */
            dblPtr = (uint16 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
            *dblPtr = (uint16)event_id;
            log->writerCore->write+= 2;

            /* Version */
            bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
            *bytePtr = (uint8)version;
            log->writerCore->write += 1;

            /* reserved byte */
            bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
            *bytePtr = 0;
            log->writerCore->write += 1;

            /* Param */
            if (version == DIAGBUF_VERSION_1)
            {
              wordPtr = (uint32 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
              *wordPtr = *((uint32 *)param);
              log->writerCore->write += 4;
            }

            if(msg_format == DIAGBUFFER_SUBTYPE_DIAG_EVENT_PAYLOAD )
            {
              /* Payload */
              bytePtr = (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
              if ((((log->writerCore->write) & (bufsize_mask)) + length) <= (log->bufSize))
              {
                memscpy( bytePtr, log->bufSize-(log->writerCore->write & bufsize_mask), payload, length );
                log->writerCore->write += length;
              }
              else /* this is wrap around case, where some portion has been written at the end of */
              {    /* the buffer and remaining is being written to in the beginning */
                uint32 first_part_length = log->bufSize - (log->writerCore->write & bufsize_mask);
                uint32 second_part_length = length - first_part_length;
                memscpy( bytePtr, (log->bufSize - (log->writerCore->write & bufsize_mask)), payload, first_part_length );
                log->writerCore->write += first_part_length;
                bytePtr =  (uint8 *)&(log->buffer[log->writerCore->write & bufsize_mask]);
                memscpy( bytePtr, (log->writerCore->readWriter & bufsize_mask), ((byte *)payload + first_part_length), second_part_length );
                log->writerCore->write += second_part_length;
              }
            }
            /*add padding at the end of the msg to round it to the next word boundary */
            log->writerCore->write += pad;

            /* Update the message length */
            wordPtr = (uint32 *)&(log->buffer[size_idx & bufsize_mask]);
            *wordPtr |= ((total_size << DIAGBUFFER_LENGTH_SHIFT) & DIAGBUFFER_LENGTH_MASK);
            ret_value = TRUE;

            /* Release the lock, acquired within FastFmtSizeTS */
            diagbufferInternal_LockRelease(log);

          }
        }
      }

    }
    else
    {
      ret_value = event_report_payload_ext(event_id, length, payload, version, param);
    }
  }
  return ret_value;
}
