/*****************************************************************************
***
*** TITLE
***
***  GERAN LLC QSH Support
***
***
*** DESCRIPTION
***
***  Definitions of QSH mertric dumps and crash dump for GERAN LLC
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
*** $Header: //components/rel/geran.mpss/7.4.0/api/gllc_qsh_ext.h#1 $
***
*** when       who     what, where, why
*** --------   ---     ------------------------------------------------------
*** 03/03/15    ra     Added QSH support
*** 06/04/15    nn     CR815228 Remove pointer declarations for qsh_dump_tag_data
*****************************************************************************/
#ifndef GLLC_QSH_EXT_H
#define GLLC_QSH_EXT_H

#include <qsh.h>

/* GLLC Debug Info */
#define GLLC_QSH_MAJOR_VER 1
#define GLLC_QSH_MINOR_VER 1

/* Some of the variables are for
** each active SAPI 1,3,5,9,11 (7 removed)
** Hence 5 in total
*/
#define QSH_SAPI_MAX_CNT 5

/* --------  Definitions for trace buffer for inter-task messages  -------- */

/* LLC message buffer element.
** Each contains basic info for every message received and sent by LLC (per gas_id)
*/
typedef struct
{
  /* total x octets per message element for every message sent and received by LLC task */

  uint8   msg_set;      /* say RLC & GMM  */
  uint8   msg_id;       /* 1st octet of msg_frag */

  uint8   llme_state;   /* e.g GLLC_LLME_TLLI_ASSIGNED  */
  uint8   lle_state;    /* e.g GLLC_LLE_TLLI_UNASSIGNED */
  uint8   lle_substate; /* e.g GLLC_LLE_L3_XID_EXCHANGE */
  uint8   lle_sapi_addr;

  uint8   gas_id;       /* subscriber number */
  uint8   msg_frag[5];  /* 5 bytes of extra data */
} gllc_trace_msg_t;

/* -----------------------  Definitions for QSH Dump  --------------------- */

typedef enum
{
  GLLC_QSH_DUMP_TAG_DATA   = 0,
  GLLC_QSH_DUMP_TAG_DATA_2 = 1
} gllc_qsh_dump_tag_e;

/* Key variables in GERAN LLC (per gas_id)
** This structure also contains the array of structures holding basic info of the last 30 messages sent and received
**
** Total size is xx octets by gas_id of LLC Task
*/
typedef struct
{
  /* Reliability class currently in use */
  uint8                 rel_class[QSH_SAPI_MAX_CNT];

  /* LLC_ME Info */
  uint8                 llme_state;
  uint8                 llme_xid_version;
  uint32                llme_new_tlli;
  uint32                llme_old_tlli;

  /* LLC Sapi states */
  uint8                 gllc_lle_sapi_addr[QSH_SAPI_MAX_CNT];
  uint8                 gllc_lle_state[QSH_SAPI_MAX_CNT];
  uint8                 gllc_lle_substate[QSH_SAPI_MAX_CNT];

  /* LLC XID Info */
  uint32                current_iov_i[QSH_SAPI_MAX_CNT];
  uint32                current_t200[QSH_SAPI_MAX_CNT];
  uint8                 current_n200[QSH_SAPI_MAX_CNT];
  uint32                current_t201[QSH_SAPI_MAX_CNT];
  uint16                current_n201_u[QSH_SAPI_MAX_CNT];
  uint16                current_n201_i[QSH_SAPI_MAX_CNT];
  uint16                current_md[QSH_SAPI_MAX_CNT];
  uint16                current_mu[QSH_SAPI_MAX_CNT];
  uint8                 current_kd[QSH_SAPI_MAX_CNT];
  uint8                 current_ku[QSH_SAPI_MAX_CNT];

  /* LLC PDU Stats */
  uint32                unack_l3pdu_tx_cnt[QSH_SAPI_MAX_CNT];
  uint32                unack_octet_tx_cnt[QSH_SAPI_MAX_CNT];
  uint32                unack_l3pdu_rx_cnt[QSH_SAPI_MAX_CNT];
  uint32                unack_octet_rx_cnt[QSH_SAPI_MAX_CNT];
  uint32                llpdu_tx_cnt[QSH_SAPI_MAX_CNT];
  uint32                llpdu_rx_cnt[QSH_SAPI_MAX_CNT];
  uint32                llpdu_fcs_err_cnt[QSH_SAPI_MAX_CNT];
  uint32                llpdu_tlli_err_cnt[QSH_SAPI_MAX_CNT];
  uint32                llpdu_addr_err_cnt[QSH_SAPI_MAX_CNT];
  uint32                llpdu_short_err_cnt[QSH_SAPI_MAX_CNT];

}gllc_dump_tag_data_s;

/* GLLC QSH dump structure */
typedef struct
{
  qsh_dump_tag_hdr_s     hdr;
  gllc_dump_tag_data_s   data;
} gllc_qsh_dump_tag_data_s;

/* ----------------  Definitions for QSH Metrics & Events  -----------------*/

typedef enum
{
  GLLC_QSH_METRIC_DATA    = 0,
  GLLC_QSH_METRIC_COUNT
} gllc_qsh_metric_e;

/* Key metrics/events in GERAN LLC (per gas_id)
** This structure also contains the array of structures holding basic info of the last 25 messages sent and received
*/
typedef struct
{
  /* The last messages sent or received by LLC Task
  */
  gllc_trace_msg_t      msg;

  /* LLC_ME Info */
  uint8                 llme_state;
  uint32                llme_new_tlli;
  uint8                 llme_encrypt_alg;

  /* LLC Sapi states */
  uint8                 gllc_lle_sapi_addr[QSH_SAPI_MAX_CNT];
  uint8                 gllc_lle_state[QSH_SAPI_MAX_CNT];
  uint8                 gllc_lle_substate[QSH_SAPI_MAX_CNT];

  /* Timer and counter values */
  uint8                 current_n200[QSH_SAPI_MAX_CNT];
  uint32                current_t201[QSH_SAPI_MAX_CNT];
  uint16                current_n201_u[QSH_SAPI_MAX_CNT];
  uint16                current_n201_i[QSH_SAPI_MAX_CNT];
} gllc_debug_metrics_s;

/* GLLC QSH metrics structure */
typedef struct
{
  qsh_metric_hdr_s            hdr;
  gllc_debug_metrics_s        data;
} gllc_qsh_metric_data_s;

#endif /* GLLC_QSH_EXT_H */

/* EOF */