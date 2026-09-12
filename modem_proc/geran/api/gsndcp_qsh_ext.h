/*****************************************************************************
***
*** TITLE
***
***  GERAN SNDCP QSH Support
***
***
*** DESCRIPTION
***
***  Definitions of QSH mertric dumps and crash dump for GERAN SNDCP
***
***
*** Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*** All Rights Reserved. QUALCOMM Proprietary.
***
*** Export of this technology or software is regulated by the U.S.
*** Government. Diversion contrary to U.S. law prohibited.
***
*****************************************************************************/


/*****************************************************************************
***
***                        EDIT HISTORY FOR MODULE
***
***  This section contains comments describing changes made to the module.
***  Notice that changes are listed in reverse chronological order.
***
*** $Header: //components/rel/geran.mpss/7.4.0/api/gsndcp_qsh_ext.h#1 $
***
*** when       who     what, where, why
*** --------   ---     ------------------------------------------------------
*** 03/03/15    ra     Added QSH support
*** 06/04/15    nn     CR815228 Remove pointer declarations for qsh_dump_tag_data
*****************************************************************************/
#ifndef GSNDCP_QSH_EXT_H
#define GSNDCP_QSH_EXT_H

#include <qsh.h>

#define GSNDCP_QSH_MAJOR_VER 1
#define GSNDCP_QSH_MINOR_VER 1

/* Some variables below can be per NSAPI
** (potentially 12 but in practice only 4 seen) */
#define QSH_NSAPI_MAX_CNT     4
#define QSH_GSN_SAPI_MAX_CNT  4

/* --------  Definitions for trace buffer for inter-task messages  -------- */

/* GSNDCP message buffer element.
** Each contains basic info for every message received and sent by SNDCP (per gas_id)
*/
typedef struct
{
  /* total x octets per message element for every message sent and received by SNDCP task */

  uint8   msg_set;      /* say LLC & SM  */
  uint8   msg_id;       /* 1st octet of msg_frag */

  uint8   sapi_state;   /* e.g EST_PEND or ESTABLISHED */
  uint8   xid_state;    /* e.g XID_CNF_PEND or XID_REQUESTED */
  uint8   nsapi;        /* GSN_NSAPI_5 to GSN_NSAPI_15 */
  uint8   sapi;         /* GSN_SAPI_3 to GSN_SAPI_11 */
  uint8   gas_id;       /* subscriber number */

  uint8   msg_frag[5];  /* 5 bytes of extra data */
} gsndcp_trace_msg_t;

/* -----------------------  Definitions for QSH Dump  --------------------- */

typedef enum
{
  GSNDCP_QSH_DUMP_TAG_DATA   = 0,
  GSNDCP_QSH_DUMP_TAG_DATA_2 = 1,
} gsndcp_qsh_dump_tag_e;

/* Key variables in GERAN SNDCP (per gas_id)
** This structure also contains the array of structures holding basic info of the last 30 messages sent and received
**
** Total size is x octets (per gas_id)
*/
typedef struct
{
  /* States of XID request/negotiation progress */
  uint8                 xid_state[QSH_GSN_SAPI_MAX_CNT];

  uint8                 sapi_state[QSH_GSN_SAPI_MAX_CNT];

  /* All the variables below can be per NSAPI
  ** (potentially 12 but in practice only 4 seen) */

  /* Active NSAPIs captured */
  uint8                 nsapi[QSH_NSAPI_MAX_CNT];

  uint8                 nsapi_ul_state[QSH_NSAPI_MAX_CNT];
  uint8                 nsapi_dl_state[QSH_NSAPI_MAX_CNT];

  /* SNDCP PDP Context Info */
  uint16                n201_u[QSH_GSN_SAPI_MAX_CNT];
  uint16                n201_i[QSH_GSN_SAPI_MAX_CNT];
  uint8                 reliability_class[QSH_NSAPI_MAX_CNT];
  uint8                 peak_throughput[QSH_NSAPI_MAX_CNT];
  uint8                 radio_priority[QSH_NSAPI_MAX_CNT];
  uint8                 data_comp_alg[QSH_NSAPI_MAX_CNT];

  /* SNDCP PDU Stats */
  uint32                snpdu_tx_cnt[QSH_NSAPI_MAX_CNT];
  uint32                snpdu_octet_tx_cnt[QSH_NSAPI_MAX_CNT];
  uint32                snpdu_rx_cnt[QSH_NSAPI_MAX_CNT];
  uint32                snpdu_octet_rx_cnt[QSH_NSAPI_MAX_CNT];

}gsndcp_dump_tag_data_s;

/* GSNDCP_QSH_DUMP_TAG_DATA data structure for QSH */
typedef struct
{
  qsh_dump_tag_hdr_s       hdr;
  gsndcp_dump_tag_data_s   data;
} gsndcp_qsh_dump_tag_data_s;

/* ----------------  Definitions for QSH Metrics & Events  -----------------*/

typedef enum
{
  GSNDCP_QSH_METRIC_DATA    = 0,
  GSNDCP_QSH_METRIC_COUNT
} gsndcp_qsh_metric_e;

/* Key metrics/events in GERAN SNDCP (per gas_id)
** This structure also contains the array of structures holding basic info of the last 25 messages sent and received
*/
typedef struct
{
  /* The last messages sent or received by SNDCP Task
  */
  gsndcp_trace_msg_t    msg;

  /* States of XID request/negotiation progress */
  uint8                 xid_state[QSH_GSN_SAPI_MAX_CNT];

  uint8                 sapi_state[QSH_GSN_SAPI_MAX_CNT];

  /* Active NSAPIs captured */
  uint8                 nsapi[QSH_NSAPI_MAX_CNT];

  /* SNDCP States */
  uint8                 nsapi_ul_state[QSH_NSAPI_MAX_CNT];
  uint8                 nsapi_dl_state[QSH_NSAPI_MAX_CNT];

  /* SNDCP PDP Context Info */
  uint16                n201_u[QSH_GSN_SAPI_MAX_CNT];
  uint16                n201_i[QSH_GSN_SAPI_MAX_CNT];
  uint8                 reliability_class[QSH_NSAPI_MAX_CNT];

  /* SNDCP PDU Stats */
  uint32                snpdu_tx_cnt[QSH_NSAPI_MAX_CNT];
  uint32                snpdu_octet_tx_cnt[QSH_NSAPI_MAX_CNT];
  uint32                snpdu_rx_cnt[QSH_NSAPI_MAX_CNT];
  uint32                snpdu_octet_rx_cnt[QSH_NSAPI_MAX_CNT];
} gsndcp_debug_metrics_s;

/* GSNDCP QSH metrics structure */
typedef struct
{
  qsh_metric_hdr_s            hdr;
  gsndcp_debug_metrics_s      data;
} gsndcp_qsh_metric_data_s;

#endif /* GSNDCP_QSH_EXT_H */

/* EOF */