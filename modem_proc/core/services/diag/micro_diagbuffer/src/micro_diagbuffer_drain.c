/*============================================================================
  micro_diagbuffer_drain.c

  micro_diagbuffer draining functions.

  Copyright (c) 2015, 2017 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
============================================================================*/

/* $Header: //components/rel/core.mpss/10.0/services/diag/micro_diagbuffer/src/micro_diagbuffer_drain.c#1 $ */

/*==========================================================================
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/19/17   ph      Tagging the data with dynamic diag IDs.
02/21/17   ph      Tag DIAG ID header to all the diag packets sent out.
08/21/15   vk      Re-factored the file to separate read from drain
08/16/15   vk      file created for micro diagbuffer drain implementation

===========================================================================*/
#ifdef DIAG_UIMAGE_MODE
#include <stddef.h>                 /* For size_t */

#include "micro_diagbuffer.h"       /* For MICRO_DIAGBUFFER_TYPE */
#include "micro_diagbuffer_read.h"  /* For micro_diagbuffer_read(),
                                       micro_diagbuffer_internal_memcpy()
									*/
#include "micro_diagbuffer_drain.h" /* For micro_diagbuffer_drain */
#include "diagcmd.h"                /* For command codes */
#include "diag_v.h"                 /* For DIAG_MY_PROC_ID */
#include "diagbuf_v.h"              /* For diag_send_desc_type */
#include "diagdiag_v.h"             /* For extended structurs, EVENT_PAY_TWO_BYTE */
//#include "diaglogi.h"               /* For LOG_DIAGPKT_OFFSET*/
#include "event_defs.h"             /* For event_id_enum_type */
#include "log.h"                    /* For log_code_type */
#include "diagi_v.h"                /* For DIAG_RPT_SIG */
#include "diagtarget.h"             /* For diag_kick_watchdog() */
#include "assert.h"                 /* For ASSERT */
#include "msg_pkt_defs.h"           /* For msg_desc_type */
#include "time_genoff.h"            /* For time_genoff_get_ts_from_offset_and_timetick() */
#include "diag_qshtrace.h"

/* Since we will be sending one event per report, timestamp will always be FULL for each event.
   defining a const for the same to use in code.
*/
#define MICRO_DIAGBUFFER_FULL_TIME_STAMP 0

extern micro_diagbuffer_handle uimage_diagbuffer;

/* diag_wcdma_wl1_frame_cb from diag.c - used to determine TimeStamp type */
extern diag_wcdma_wl1_frame_cb_type diag_wcdma_wl1_frame_cb;

/* diag_uImage_timer_offset from diag.c
   This variable stores time offset when phone goes in uImage mode. 
*/
extern time_type diag_uimage_time_offset;
extern boolean diag_uimage_is_time_offset_negative;

/* Structs for micro diagbuffer header, log, event and f3 types */
typedef PACK( struct )
{
  uint16 format;
  uint16 length;
  uint64 timetick;
}
micro_diagbuffer_diag_pkts_hdr_type;

typedef PACK( struct )
{
  uint16 event_id;
  uint8 version;
  uint8 pad;
}
micro_diagbuffer_event_type;

typedef PACK( struct )
{
  uint16 log_code;
  uint8 version;
  uint8 pad;
}
micro_diagbuffer_log_type;

typedef PACK( struct )
{
  uint8 num_args;
  uint8 version;
  uint16 pad;
}
micro_diagbuffer_msg_type;

/**
 * <!-- micro_diagbuffer_drain -->
 *
 * @brief convert micro diagbuffer type pkts to regular diag 
 * type pkts and send them. After sending one full pkt it checks 
 * if new signals were sent on diag task in mean while. If so it 
 * exit drain loop to handle signals on diag task. Please note 
 * this routine does not need to know how far along it had drain 
 * from micro diagbuffer because the buffer itself takes care of 
 * that via its log->"read" index.
 *  
 * @param  mask  : mask of diag signals this drain routine runs
 *                 with. It is use to check against new signals
 *                 that may have been set while one full pkt was
 *                 constructed and drained.
 *  
 * @sa           : Pseudo code for this function 
 *                 Step 1. Read one pkt from uimage_diagbuffer
 *                 Step 2. If pkt was successfully read go to Step 3. 
 *                         Exit if buffer is empty or read was not
 *                         successful or a signal was set on Diag task.
 *                 Step 3. Check formate of the packet to determine if it 
 *                         is f3/log/event with or without payload 
 * 
 *                 Step 3.1. If format == MICRO_ULOG_SUBTYPE_DIAG_MSG 
 *                 Step 3.2. Do F3 specific task
 *  
 *                 Step 3.1. If format == MICRO_ULOG_SUBTYPE_DIAG_LOG
 *                 Step 3.3. Do LOG specific task
 * 
 *                 Step 3.1. If format == MICRO_ULOG_SUBTYPE_DIAG_EVENT | 
 *                                  MICRO_ULOG_SUBTYPE_DIAG_EVENT_PAYLOAD
 *                 Step 3.4. Do EVENT with and without payload specific task
 *
 *                 Step 4. Send the packet to the communication layer.
 *                 Step 5. Check if there is a signal on diag task.
 *                 Step 6. go to Step 1. 
 *
 * @return micro_diagbuffer_drain_result 
 */

/* 
   Vinay - ToDo - Drain can be made to work on any micro diagbuffer by providing handle to it, only change needed in this
   function is to call micro_diagbuffer_read() with the handle to desired buffer instead of hardcoded uimage_diagbuffer.
   This will cause function prototype (micro_diagbuffer_drain.h) and defination (below) to change to receive handle and
   call to this function (diagbuf_drain() - diagbuf.c ) to pass buffer handle.
*/
#define TEMP_DIAG_BUF_PKT_SIZE (2 * MICRO_ULOG_MAX_MSG_SIZE)

micro_diagbuffer_drain_result micro_diagbuffer_drain( osal_sigs_t mask )
{
  /* Variable to check return value of micro_diagbuffer_read() */
  micro_diagbuffer_result result = MICRO_DIAGBUFFER_SUCCESS;

  /* Variable to return value from this routine. */
  micro_diagbuffer_drain_result drain_exit_cause = MICRO_DIAGBUFFER_DRAIN_SUCCESS;

  /* sigs is used to check if drain loop should exit before it has been drained completely .*/
  osal_sigs_t sigs;
  int return_val = 0; /* store return value from osal_set_sigs, used for ASSERT */

  /* 
     Source buffer, contains one full micro diagbuffer pkt after a
     successful call to micro_diagbuffer_read().
  */
  uint8 source_micro_diag_buf_pkt[ MICRO_ULOG_MAX_MSG_SIZE ];
  uint8 *source_work_ptr = NULL;       /* ptr variable used for pointer arithmatics on source buffer */

  /* Destination buffer, which will contain one full regular diag pkt. */
  uint8 destination_diag_buf_pkt[ TEMP_DIAG_BUF_PKT_SIZE ];
  uint8 *destination_diag_buf_ptr = NULL; /* ptr variable used in arithmatics on destination buffer */
  uint16 destination_copy_length = 0;     /* variable used to add to destination ptr to reach to a specific bytes for 
                                             writing to destination 
                                          */

  size_t copy_size = 0;

  /* Stores the amount of data placed in the output buffer. */
  uint32 output_count = 0;
  time_type temp_ts = { 0, 0 };

  diag_cmd_ext_v1_type ext_hdr;
  micro_diagbuffer_diag_pkts_hdr_type *micro_diagbuffer_pkt_hdr_ptr;
  uint32 param;
  uint8 param_length = 0;

  /* send.terminate is set to TRUE as we will send one complete packet at a time. */
  diag_send_desc_type send = { NULL, NULL, DIAG_SEND_STATE_START, TRUE, FALSE, FALSE };

  /* read one (last param to the function call below) micro diagbuffer pkt from uimage micro diagbuffer */
  result = micro_diagbuffer_read(uimage_diagbuffer, MICRO_ULOG_MAX_MSG_SIZE, (uint8 *)&source_micro_diag_buf_pkt, &output_count, 1);
  if ( result != MICRO_DIAGBUFFER_SUCCESS || output_count == 0 )
  {
    drain_exit_cause = MICRO_DIAGBUFFER_DRAIN_READ_FROM_MDB_FAILED;
    if ( output_count == 0 )
    {
      drain_exit_cause = MICRO_DIAGBUFFER_DRAIN_BUFFER_EMPTY;
    }
    return ( drain_exit_cause );
  }

  /* process the read msg, send it out, check for signals on Diag task and then read next pkt from
     micro diagbuffer
  */
  while ( output_count > 0 )
  {
    /* Start processing from the buffer. */
    source_work_ptr = source_micro_diag_buf_pkt;

    /* 
       Each micro diagbuffer diag pkt has following format
     
       |format -2 bytes| Length - 2 bytes| Timetick - 8 bytes|<pkt type specific stuff >|
     
       typecast the source_work_ptr to get header field. This will allow us to know which pkt type
       we are working on and get its length, timetick etc.
    */
    micro_diagbuffer_pkt_hdr_ptr = (micro_diagbuffer_diag_pkts_hdr_type *)source_work_ptr;

    /* 
       Move work pointer by header type size, at this point source_work_ptr is pointing to
       pkt type specific portion which needs to be handled accordingly.
    */
    source_work_ptr += sizeof(micro_diagbuffer_diag_pkts_hdr_type);

    /* ToDo: what if the length field is wrong? */
    destination_diag_buf_ptr = destination_diag_buf_pkt;

    /* Now we know the packet type */
    switch ( micro_diagbuffer_pkt_hdr_ptr->format )
    {
      case MICRO_ULOG_SUBTYPE_DIAG_MSG:
      {
        //micro_diagbuffer_process_f3();
        micro_diagbuffer_msg_type *micro_diagbuffer_msg_ptr = NULL;

        uint8 *const_type_access_ptr = NULL;
        char *fmt_str_ptr = NULL;
        char *fname_ptr = NULL;

        uint16 fmt_str_len = 0;
        uint16 fmt_file_len = 0;

        msg_desc_type msg_desc;


        /* get the 32 bits address at source_work_ptr */
        const_type_access_ptr = (uint8 *)((void *)(*(uint32 *)(source_work_ptr)));

        /* At the address pointed by const_type_access_ptr we have line number */
        msg_desc.line = *(uint16 *)(const_type_access_ptr);
        const_type_access_ptr += sizeof(uint16);

        /* At the address pointed by const_type_access_ptr we now have ss_id */
        msg_desc.ss_id = *(uint16 *)(const_type_access_ptr);
        const_type_access_ptr += sizeof(uint16);

        /* At the address pointed by const_type_access_ptr we now have ss_mask */
        msg_desc.ss_mask = *(uint32 *)(const_type_access_ptr);
        const_type_access_ptr += sizeof(uint32);

        fmt_str_ptr = (char *)((void *)(*(uint32 *)const_type_access_ptr));
        fmt_str_len = (int)strlen((char *)(fmt_str_ptr));
        const_type_access_ptr += sizeof(uint32);

        fname_ptr = (char *)(*(uint32 *)const_type_access_ptr);
        fmt_file_len = (int)strlen((char *)(fname_ptr));

        /* have processed const type/const hash - 4 bytes, move source_work_ptr by the same amount */
        source_work_ptr += sizeof(uint32);

        /* get pointer to msg specific stuffs */
        micro_diagbuffer_msg_ptr = (micro_diagbuffer_msg_type *)source_work_ptr;

        /* move source_work_ptr to point to PARAM (field is available) or DATA */
        source_work_ptr += (sizeof(micro_diagbuffer_msg_type));

        /* if not a version we undersatnd, skip processing it. */
        if ( (micro_diagbuffer_msg_ptr->version != DIAGBUF_VERSION_0) && 
             (micro_diagbuffer_msg_ptr->version != DIAGBUF_VERSION_1))
        {
           break;
        }

        if ( micro_diagbuffer_msg_ptr->version == DIAGBUF_VERSION_1 )
        {
          param = *(uint32 *)source_work_ptr;
          source_work_ptr += (sizeof(uint32)); /* As per version 1 of extnded pkt param is 
                                                  4 bytes long */

          /* It's an extended pkt, add ext v1 pkt header */
          ext_hdr.cmd_code = DIAG_CMD_EXT_F;
          ext_hdr.version = micro_diagbuffer_msg_ptr->version;
          ext_hdr.proc_id = DIAG_MY_PROC_ID;
          ext_hdr.id = param;
          copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length),
                                                       (TEMP_DIAG_BUF_PKT_SIZE - destination_copy_length),
                                                       &ext_hdr,
                                                       sizeof(diag_cmd_ext_v1_type));
          destination_copy_length += copy_size;


          /* after this we need to put regular diag packet */
        }

        /* converting to diag buf MSG pkt */
        /* store command code */
        *(destination_diag_buf_ptr + destination_copy_length) = DIAG_EXT_MSG_F;
        destination_copy_length++;

        /* store TS type */
        if ( diag_wcdma_wl1_frame_cb != NULL )
        {
          *(destination_diag_buf_ptr + destination_copy_length) = MSG_TS_TYPE_GW;
        }
        else
        {
          *(destination_diag_buf_ptr + destination_copy_length) = MSG_TS_TYPE_CDMA_FULL;
        }
        destination_copy_length++;

        /* Store number of args */
        *(destination_diag_buf_ptr + destination_copy_length) = micro_diagbuffer_msg_ptr->num_args;
        destination_copy_length++;

        /* Store drop cnt, since reading from circular buffer drop_cnt will be 0 */
        *(destination_diag_buf_ptr + destination_copy_length) = 0; /* Drop count is 0 for micro diagbuffer */
        destination_copy_length++;

        /* Convert timetick to network time */
        time_genoff_get_ts_from_offset_and_timetick(diag_uimage_time_offset, diag_uimage_is_time_offset_negative,
                                                    micro_diagbuffer_pkt_hdr_ptr->timetick, temp_ts);

        /* Store network time */
        copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length), sizeof(time_type),
                                                     temp_ts, sizeof(time_type));
        destination_copy_length += copy_size;

        /* Store msg_desc i.e. line number, ss_id, ss_mask */
        copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length),
                                                     (TEMP_DIAG_BUF_PKT_SIZE - destination_copy_length),
                                                     &msg_desc, sizeof(msg_desc_type));
        destination_copy_length += copy_size;


        /* Store ARGS */
        if ( micro_diagbuffer_msg_ptr->num_args )
        {
          /* source_work_ptr is pointing at ARGS, ARGS are 4*NUM_ARGS long */
          copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length), (TEMP_DIAG_BUF_PKT_SIZE - destination_copy_length),
                                                       source_work_ptr, (sizeof(uint32) * micro_diagbuffer_msg_ptr->num_args));
          destination_copy_length += copy_size;

        }

        /* Store format string */
        copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length), (TEMP_DIAG_BUF_PKT_SIZE - destination_copy_length),
                                                     fmt_str_ptr, fmt_str_len);
        destination_copy_length += copy_size;

        *(char *)(destination_diag_buf_ptr + destination_copy_length) = '\0';
        destination_copy_length++;

        /* Store file name */
        copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length), (TEMP_DIAG_BUF_PKT_SIZE - destination_copy_length),
                                                     fname_ptr, fmt_file_len);
        destination_copy_length += copy_size;

        *(char *)(destination_diag_buf_ptr + destination_copy_length) = '\0';
        destination_copy_length++;

        break;
      } /* end of msg case */
      case MICRO_ULOG_SUBTYPE_DIAG_LOG:
      {
        uint16 log_data_length = 0;
        micro_diagbuffer_log_type *micro_diagbuffer_log_ptr = NULL;

        /* get pointer to log specific stuffs */
        micro_diagbuffer_log_ptr = (micro_diagbuffer_log_type *)source_work_ptr;

        /* move source_work_ptr to point to PARAM (field is available) or DATA */
        source_work_ptr += (sizeof(micro_diagbuffer_log_type));

        /* if not a version we undersatnd, skip processing it. */
        if ( (micro_diagbuffer_log_ptr->version != DIAGBUF_VERSION_0) && 
             (micro_diagbuffer_log_ptr->version != DIAGBUF_VERSION_1))
        {
           break;
        }

        if ( micro_diagbuffer_log_ptr->version == DIAGBUF_VERSION_1 )
        {
          param = *(uint32 *)source_work_ptr;
          source_work_ptr += (sizeof(uint32)); /* As per version 1 of extnded pkt param is 
                                                  4 bytes long */

          param_length = sizeof(uint32); /* length of param field is 4 bytes, will use this 
                                            variable to calculate data length in this log pkt
                                         */
          /* It's an extended pkt, build and copy ext v1 pkt header */
          ext_hdr.cmd_code = DIAG_CMD_EXT_F;
          ext_hdr.version = micro_diagbuffer_log_ptr->version;
          ext_hdr.proc_id = DIAG_MY_PROC_ID;
          ext_hdr.id = param;
          copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length),
                                                       (TEMP_DIAG_BUF_PKT_SIZE - destination_copy_length),
                                                       &ext_hdr, sizeof(diag_cmd_ext_v1_type));
          destination_copy_length += copy_size;

          /* after this we need to put regular diag packet */
        }

        /* Building to regular diag log pkt */
        /* Build Log Header ->
         | Command Code 0x10 | MORE | LENGTH | LENGTH | LOG_CODE | TIMESTAMP | DATA |
         ---------------------
         Command Code - 1 byte long
        */
        *(destination_diag_buf_ptr + destination_copy_length) = DIAG_LOG_F; /* Copy cmd code DIAG_LOG_F */
        destination_copy_length++;

        /* Build Log Header ->
         | Command Code 0x10 | MORE | LENGTH | LENGTH | LOG_CODE | TIMESTAMP | DATA |
                              -------
         More - 1 byte long
        */
        *(destination_diag_buf_ptr + destination_copy_length) = 0; /* MORE field of cmd code DIAG_LOG_F is 0*/
        destination_copy_length++;

        /* Calculate length of the data in the micro diagbuffer type log */
        log_data_length = micro_diagbuffer_pkt_hdr_ptr->length
                          - sizeof(micro_diagbuffer_diag_pkts_hdr_type)
                          - sizeof(micro_diagbuffer_log_type)
                          - param_length;
        /* Build Log Header ->
         | Command Code 0x10 | MORE | LENGTH | LENGTH | LOG_CODE | TIMESTAMP | DATA |
                                     ---------
         Length - 2 byte long, its length of the rest of the packet i.e. length (next field) + Log Code + TS + Data
                  or in other words sizeof(log_header_type) + DATA size.
        */
        *(uint16 *)(destination_diag_buf_ptr + destination_copy_length) = sizeof(log_header_type) + log_data_length;
        destination_copy_length += (sizeof(uint16));

        /* Store length which is part of LOG_ITEM */
        /* Build Log ->
         | Command Code 0x10 | MORE | LENGTH | LENGTH | LOG_CODE | TIMESTAMP | DATA |
                                              ---------
         Length - 2 byte long, its length of the rest of the packet i.e. length (next field) + Log Code + TS + Data
                  or in other words sizeof(log_header_type) + DATA size. 
        */
        *(uint16 *)(destination_diag_buf_ptr + destination_copy_length) = sizeof(log_header_type) + log_data_length;
        destination_copy_length += (sizeof(uint16));

        /* Store Log code field */
        /* Build Log ->
         | Command Code 0x10 | MORE | LENGTH | LENGTH | LOG_CODE | TIMESTAMP | DATA |
                                                      ------------
         log code - 2 byte long.  
        */
        *(uint16 *)(destination_diag_buf_ptr + destination_copy_length) = micro_diagbuffer_log_ptr->log_code;
        destination_copy_length += (sizeof(uint16));

        /* Convert timetick to network time */
        /* following takes timetick from micro diagbuffer header and convert it */
        time_genoff_get_ts_from_offset_and_timetick(diag_uimage_time_offset, diag_uimage_is_time_offset_negative,
                                                    micro_diagbuffer_pkt_hdr_ptr->timetick, temp_ts);

        /* Store network time */
        /* Build Log ->
             | Command Code 0x10 | MORE | LENGTH | LENGTH | LOG_CODE | TIMESTAMP | DATA |
                                                                     -------------
             Time Stamp - 8 byte long.  
        */
        /* Store network time */
        copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length), sizeof(time_type),
                                                     temp_ts, sizeof(time_type));


        destination_copy_length += copy_size;

        /* Store rest of the DATA */
        /* Build Log ->
         | Command Code 0x10 | MORE | LENGTH | LENGTH | LOG_CODE | TIMESTAMP | DATA |
                                                                             --------
         Data - N bytes long. length of data is in log_data_length
        */
        copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length),
                                                     (TEMP_DIAG_BUF_PKT_SIZE - destination_copy_length),
                                                     source_work_ptr,
                                                     log_data_length);

        destination_copy_length += copy_size;
        break;
      } /* end of log case */
      case MICRO_ULOG_SUBTYPE_DIAG_EVENT:
      case MICRO_ULOG_SUBTYPE_DIAG_EVENT_PAYLOAD:
      {
        uint8 event_payload_length = 0;
        uint8 length_field = 0;
        uint8 event_field = 0;
        uint16 outgoing_event_length = 0;
        event_id_type micro_diagbuffer_event_id;

        micro_diagbuffer_event_type *micro_diagbuffer_event_ptr = NULL;

        /* get pointer to event specific stuffs */
        micro_diagbuffer_event_ptr = (micro_diagbuffer_event_type *)source_work_ptr;

        /* move source_work_ptr to point to PARAM (field is available) or DATA */
        source_work_ptr += (sizeof(micro_diagbuffer_event_type));

        /* if not a version we undersatnd, skip processing it. */
        if ( (micro_diagbuffer_event_ptr->version != DIAGBUF_VERSION_0) && 
             (micro_diagbuffer_event_ptr->version != DIAGBUF_VERSION_1))
        {
           break;
        }

        if ( micro_diagbuffer_event_ptr->version == DIAGBUF_VERSION_1 )
        {
          param = *(uint32 *)source_work_ptr;
          source_work_ptr += (sizeof(uint32)); /* As per version 1 of extnded pkt param is 
                                                  4 bytes long 
                                               */

          param_length = sizeof(uint32); /* length of param field is 4 bytes, will use this 
                                            variable to calculate payload length of the event.
                                         */

          /* It's an extended pkt, build and copy ext v1 pkt header */
          ext_hdr.cmd_code = DIAG_CMD_EXT_F;
          ext_hdr.version = micro_diagbuffer_event_ptr->version;
          ext_hdr.proc_id = DIAG_MY_PROC_ID;
          ext_hdr.id = param;
          copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length),
                                                       (TEMP_DIAG_BUF_PKT_SIZE - destination_copy_length),
                                                       &ext_hdr, sizeof(diag_cmd_ext_v1_type));
          destination_copy_length += copy_size;

          /* after this we need to put regular diag packet */
        }

        /* Building to regular diag event pkt */
        /* Build Event Report Header ->
         | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
         ---------------------
         Command Code - 1 byte long
        */
        *(destination_diag_buf_ptr + destination_copy_length) = DIAG_EVENT_REPORT_F; /* Copy cmd code DIAG_EVENT_REPORT_F */
        destination_copy_length++; /* Command code is 1 byte long */

        /* Length of the event */
        /* Build Event Report Header ->
         | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                             ----------
         Length - length of the whole event - Length field is 2 bytes long 
        */
        /*Will calculate length and store in destination*/
        length_field = destination_copy_length;
        destination_copy_length += (sizeof(uint16)); /* Event length is 2 bytes long */

        /* Event ID */
        /* Build Event ->
         | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                                      ------------
         EVENT ID - 2 bytes long. 
        */
        /*Will copy event id at the end as by then we would have calculated payload length and its needs to 
          payload length info is needed to store in bit 14 and bit 15 of the event id */
        event_field = destination_copy_length;
        destination_copy_length += (sizeof(uint16));

        /* Convert timetick to network time */
        /* Build Event ->
         | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                                                 ---------
         TIME - 8 bytes long. Since we are sending one event per report.
        */
        /* following takes timetick from micro diagbuffer header and convert it */
        time_genoff_get_ts_from_offset_and_timetick(diag_uimage_time_offset, diag_uimage_is_time_offset_negative,
                                                    micro_diagbuffer_pkt_hdr_ptr->timetick, temp_ts);

        /* Store network time */
        copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length), sizeof(time_type),
                                                     temp_ts, sizeof(time_type));

        destination_copy_length += copy_size;

        /* 
           Check if event is event with payload type, if is it, store length of payload in destination buffer and
           copy event payload.
        */
        if ( micro_diagbuffer_pkt_hdr_ptr->format == MICRO_ULOG_SUBTYPE_DIAG_EVENT_PAYLOAD )
        {
          /* Calculate event payload length*/
          event_payload_length = micro_diagbuffer_pkt_hdr_ptr->length - sizeof(micro_diagbuffer_diag_pkts_hdr_type)
                                 - sizeof(micro_diagbuffer_event_type)
                                 - param_length;

          /* Store Payload length */
          /* Build Event ->
           | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                                                          ---------------
           PAYLOAD_LEN - 1 byte long.
          */
          *(destination_diag_buf_ptr + destination_copy_length) = event_payload_length;
          destination_copy_length += (sizeof(uint8));

          /* Store payload data */
          /* Build Event ->
           | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                                                                         ---------------
           PAYLOAD_DATA - N (event_payload_length) byets long.
          */
          copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + destination_copy_length),
                                                       (TEMP_DIAG_BUF_PKT_SIZE - destination_copy_length),
                                                       source_work_ptr,
                                                       event_payload_length);

          destination_copy_length += copy_size;
        } /* end of if (micro_diagbuffer_pkt_hdr_ptr->format == MICRO_ULOG_SUBTYPE_DIAG_EVENT_PAYLOAD) */

        /* Since now we know payload length, we can build event id, and copy it to destination */
        micro_diagbuffer_event_id.id = micro_diagbuffer_event_ptr->event_id;

        if ( event_payload_length <= EVENT_PAY_TWO_BYTE )
        {
          micro_diagbuffer_event_id.payload_len = event_payload_length;
        }
        else
        {
          micro_diagbuffer_event_id.payload_len = EVENT_PAY_LENGTH;
          /* 
             add one (for 1 byte) to payload length to commodate 1 byte payload_length field in event (only for event_with_payload)
             we are doing this here to avoid taking one extra variable to track that and use in outgoing_event_length calculation
             below
          */
          event_payload_length += sizeof(uint8);
        }
        micro_diagbuffer_event_id.reserved = 0; /* Reserved bit */
        micro_diagbuffer_event_id.time_trunc_flag = MICRO_DIAGBUFFER_FULL_TIME_STAMP; /* Since we are sending one event per report
                                                                                         time stamp will always be FULL for each 
                                                                                         event.
                                                                                      */

        /* Earlier we left hole in the destination buffer to fill EVENT ID now. */
        /* Build Event ->
         | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                                      ------------
          Store EVENT_ID
        */
        copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + event_field),
                                                     sizeof(event_id_type),
                                                     &micro_diagbuffer_event_id,
                                                     sizeof(event_id_type));

        /* now update length for the event packet */
        /* length = payload length + 2 bytes of event (event id 12 bits, 1 bit reserved, 2 bits payload length flag, 1 bit time length) +
                                     8 bytes of timestamp, since there is one event per report we will always have fill time stamp */
        outgoing_event_length = event_payload_length + sizeof(uint16) + sizeof(uint64);

        /* 
           We calculated length_field above to copy event length now. We do not need to move destination now (as we have already done it above when calculated
           length_field
        */
        /* Build Event ->
         | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                              ---------
          Storing LENGTH 
        */
        copy_size = micro_diagbuffer_internal_memcpy((destination_diag_buf_ptr + length_field), sizeof(uint16),
                                                     &outgoing_event_length, sizeof(uint16));

        break;
      } /* end of event case */
      default:
      {
        drain_exit_cause = MICRO_DIAGBUFFER_DRAIN_EXIT_DUE_TO_UNKOWN_PKT_FORMAT;
        return ( drain_exit_cause ); //error case we cannot have none Diag type in the buffer.
      }
    } /* end of switch (format) */

    /* 
    if we are here we have converted micro diag buffer packet to regular diag buffer pkt,
    now sent the converted packet
    */
    if ( destination_copy_length != 0 ) /* Meaning we have a valid packet in destination */
    {
      send.pkt = (void *)destination_diag_buf_ptr;
      send.last = (void *)((destination_diag_buf_ptr + destination_copy_length) - 1);
      send.state = DIAG_SEND_STATE_START;
      send.terminate = TRUE;
      send.priority = FALSE;
      send.is_cmd_rsp_type = FALSE;
      send.diag_id = diag_get_diagID();

      /* Vinay - ToDo - send to DIAG_STREAM_2 and listeners. */
      (void)diagbuf_send_pkt(&send, DIAG_STREAM_1);
    }
	
    destination_copy_length = 0; /* reset destination_copy_length */
    param_length = 0;            /* reset param_length */
    output_count = 0;            /* output_count is to store how much data is placed per 
                                    read. It needs to reset before next read() call.
                                 */

    /* Add code to check signal on Diag task */
    /* Get signals */
    return_val = osal_get_sigs(diag_task_tcb, &sigs);
    ASSERT(OSAL_SUCCESS == return_val);

    /* RPT_TIMER_SIG?  Kick the dog */
    if ( sigs & DIAG_RPT_SIG )
    {
      diag_kick_watchdog();
    }
    /* mask sig or diag is blocking?  Break out.  */
    if ( sigs & mask )
    {
      drain_exit_cause = MICRO_DIAGBUFFER_DRAIN_EXIT_DUE_TO_SIGNAL_ON_DIAG;
      break; /* Exit loop.  Something more important has happened! */
    }

    result = micro_diagbuffer_read(uimage_diagbuffer, MICRO_ULOG_MAX_MSG_SIZE, (uint8 *)&source_micro_diag_buf_pkt, &output_count, 1);
    if ( result != MICRO_DIAGBUFFER_SUCCESS || output_count == 0 )
    {
      drain_exit_cause = MICRO_DIAGBUFFER_DRAIN_READ_FROM_MDB_FAILED;
      if ( output_count == 0 )
      {
        drain_exit_cause = MICRO_DIAGBUFFER_DRAIN_BUFFER_EMPTY;
      }
      return ( drain_exit_cause );
    } /* end of if result != */
  } /* end of while loop */
  return ( drain_exit_cause );
} /* end of micro_diagbuffer_drain */
#endif /* DIAG_UIMAGE_MODE */
