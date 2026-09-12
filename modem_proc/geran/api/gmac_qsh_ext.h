#ifndef GMAC_QSH_EXT_H
#define GMAC_QSH_EXT_H

/*! \file gmac_qsh_ext.h

  This file contains definitions for QSH crash dump and metrics data collection.

                Copyright (c) 2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
*/
/* $Header: //components/rel/geran.mpss/7.4.0/api/gmac_qsh_ext.h#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

#include "qsh.h"


#define GMAC_QSH_MAJOR_VER  1
#define GMAC_QSH_MINOR_VER  0


/* --------------------  Definitions for trace buffer  -------------------- */

#define GMAC_QSH_NUM_TRACE_MSGS 100

// Trace buffer entry. 12 bytes.
typedef struct
{
  uint8  next_entry;           /*This will tell whether next entry is free or not*/
  uint32 timestamp_fn;         /*GL1 FN*/
  uint8  gmac_mode;            /*NULL/IDLE/TRANSFER*/
  uint8  state;                /*Corresponding mode state*/
  uint8  message_set;          /*Received/sent message set*/
  uint8  message_id;           /*Received/sent message ID*/
  uint8  message_frag[3];      /*first three bytes of message*/
} gmac_qsh_trace_msg_s;

// Trace buffer. 1200 bytes.
typedef struct
{
  gmac_qsh_trace_msg_s buf[GMAC_QSH_NUM_TRACE_MSGS];   // 100 x 12 bytes = 1200 bytes
} gmac_qsh_trace_msg_buf_s;


/* -----------------------  Definitions for QSH Dump  --------------------- */

typedef enum
{
  GMAC_QSH_DUMP_TAG_DATA    = 0,
  GMAC_QSH_DUMP_TAG_DATA_2  = 1
} gmac_qsh_dump_tag_e;

// GRR_QSH_DUMP_TAG_DATA data structure for QSH. sizeof(hdr) + 1200 bytes.
typedef struct
{
  qsh_dump_tag_hdr_s        hdr;
  gmac_qsh_trace_msg_buf_s  trace_buf;   // 100 x 12 bytes = 1200 bytes
} gmac_qsh_dump_tag_data_s;


/* --------------------  Definitions for QSH Metrics  --------------------- */

typedef enum
{
  GMAC_QSH_METRIC_DATA    = 0,
  GMAC_QSH_METRIC_COUNT
} gmac_qsh_metric_e;

// GMAC_QSH_METRIC_DATA structure

typedef struct
{
  uint8  gmac_mode;            /*NULL/IDLE/TRANSFER*/
  uint8  state;                /*Corresponding mode state*/
  uint8  message_set;          /*Received/sent message set*/
  uint8  message_id;           /*Received/sent message ID*/
} gmac_qsh_metric_trace_msg_s;

typedef struct
{
  qsh_metric_hdr_s            hdr;
  gmac_qsh_metric_trace_msg_s metric_buf;
} gmac_qsh_metric_data_s;

#endif // #ifndef GMAC_QSH_EXT_H

/* EOF */

