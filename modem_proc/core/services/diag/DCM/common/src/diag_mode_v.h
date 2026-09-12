#ifndef DIAG_MODE_V_H
#define DIAG_MODE_V_H
/*==========================================================================

              Diag APIs for buffering and streaming modes

General Description

Copyright (c) 2013-2015,2018 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

===========================================================================*/

/*===========================================================================

                              Edit History

 $Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/diag_mode_v.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/19/18   ph      Configure buffering modes for user PD send buffers.
02/21/17   ph      Tag DIAG ID header to all the diag packets sent out.
11/13/15   sr      Added support for sockets
08/31/15   ph      Added support for buffering mode APIs.
01/01/14   ph      Fixed inconsistent drain behavior in circular and threshold buffering.
08/31/13   is      Support for peripheral buffering mode

===========================================================================*/

/*Version of diag_lock_buffer_api_request command  */
#define DIAG_LOCK_BUFFER_API_VERSION  0x1

/* Indicate diag buffering APIs are locked*/
#define DIAG_BUFFER_API_LOCKED        0x0

/* Indicate diag buffering APIs are unlocked*/
#define DIAG_BUFFER_API_UNLOCKED      0x1

typedef struct
{
  uint16 pattern;
  uint16 length;
}diag_send_buf_header_type;

/* 
This is the header that is tagged along with each 
DIAG packet sent out from peripheral 
*/
typedef struct
{
  uint8 diag_id; // DIAG ID value 
  uint8 reserved; // Reserved 
  uint16 length;  //Length of the data packet  
}diag_tag_hdr_type;

typedef struct
{
uint8* ptr;
uint32 head;
uint32 tail;
uint32 size;
uint32 threshold;
uint32 used;
boolean wrap;
diag_send_buf_header_type* curr_data_block_header;
diag_tag_hdr_type* tag_hdr;
}diag_send_buf_type;

typedef struct
{
  uint8 mode;
  boolean drain;

  uint8* buf_ptr;
  uint32 buf_head;
  uint32 buf_tail;
  uint32 buf_used;
  uint32 buf_size;
  uint32 curr_threshold; /*Maximum index that packet be written to and it changes
                           depending on head and tail movement in buffering*/
  boolean buf_wrap;
  diag_send_buf_header_type* curr_data_block_header;

  uint32 buffered_many_bytes_mark;  /* # of bytes used before we start draining */
  uint8 buffered_many_per_mark;     /* % of buffer used before we start draining */
  uint32 buffered_few_bytes_mark;   /* # of bytes used before we stop draining */
  uint8 buffered_few_per_mark;      /* % of buffer used before we stop draining */
  uint32 byte_dropped_cnt;
  uint32 byte_alloc_cnt;

  //Stores previous counts before a mode switch/reset
  uint8 prev_mode;
  uint32 prev_byte_dropped_cnt;
  uint32 prev_byte_alloc_cnt;
  #if defined(DIAG_MP_MASTER)
  uint32 fwd_cur_dsm_cnt;
  #endif

  //Flat buffer parameters
  uint32 cur_buf_cnt;

} diag_tx_mode_type;

/*===========================================================================
FUNCTION DIAG_BUFFERED_MODE_STREAM (internal)

DESCRIPTION
  Checks if we're currently in one of the buffering modes.

PARAMETERS
  stream_id - Valid stream IDs are between DIAG_MIN_STREAM_ID and DIAG_MAX_STREAM_ID.
              DIAG_STREAM_ALL for all streams.

RETURN VALUE
  TRUE - If we're currently in one of the buffering modes.
  FALSE - Otherwise.

===========================================================================*/
boolean diag_buffered_mode_stream( uint8 stream_id );

/*===========================================================================

FUNCTION DIAG_TX_MODE_INIT (internal)

DESCRIPTION
  Initializes global variable for keeping track of streaming mode vs buffering mode.

PARAMETERS
  None

RETURN VALUE
  None

===========================================================================*/
void diag_tx_mode_init( void );

/*===========================================================================
FUNCTION DIAG_DRAIN_DSM_BUFFER (internal)

DESCRIPTION
  This function drains the buffering DSM pools per stream.

PARAMETERS
  stream_id - stream_id for buffering DSM pool to drain.
              Valid stream IDs are between DIAG_MIN_STREAM_ID and DIAG_MAX_STREAM_ID.
  tx_mode_ptr - tx_mode_ptr to retrieve the drain(send) buffer info.

RETURN VALUE
  None

===========================================================================*/
void diag_drain_dsm_buffer( uint8 stream_id, diag_tx_mode_type* tx_mode_ptr );

/*===========================================================================
FUNCTION diag_buffering_pool_used_bytes_cnt (internal)

DESCRIPTION
  This function returns the # of DSMs used in the buffering pool for
  each stream.

PARAMETERS
  stream_id - Valid stream IDs are between DIAG_MIN_STREAM_ID and DIAG_MAX_STREAM_ID.

RETURN VALUE
  The # of DSMs used in the buffering pool for each stream.

===========================================================================*/
uint32 diag_buffering_pool_used_bytes_cnt( uint8 stream_id );

/*===========================================================================

FUNCTION DIAG_DRAIN_POOL_CHECK

DESCRIPTION
  Checks whether Diag needs to set signal to drain the DSM pool. Diag should
  signal draining if we're passed the threshold in threshold buffering mode
  or if we're in streaming mode.

PARAMETERS
  stream_id - Valid stream IDs are between DIAG_MIN_STREAM_ID and DIAG_MAX_STREAM_ID.
  tx_mode_ptr - tx_mode_ptr to retrieve the drain(send) buffer info.

RETURN VALUE
  TRUE - Diag can drain the pool now.
  FALSE - Buffering pools are not ready to be drained yet.

===========================================================================*/
boolean diag_drain_pool_check( uint8 stream_id, diag_tx_mode_type *tx_mode_ptr );

/*===========================================================================
FUNCTION DIAG_SET_BUFFERING_BUFFER

DESCRIPTION
  Changes the buffer used for peripheral buffering.  New DSM pool will be
  allocated from the memory location passed in through this function.

PARAMETERS
  buf_ptr - Pointer to buffer to be used for peripheral buffering mode.
  length - Length, in bytes, of buf_ptr.
  stream_id - Stream id to set buffer for

RETURN VALUE
  TRUE - If buffer used for peripheral buffering was successfully changed.
  FALSE - Otherwise.

===========================================================================*/
  boolean diag_set_buffering_buffer( uint8 * buf_ptr, uint32 length, uint8 stream_id);
  
#endif /*DIAG_MODE_V_H*/