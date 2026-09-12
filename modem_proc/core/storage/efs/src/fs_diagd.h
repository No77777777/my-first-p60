/***********************************************************************
 * fs_diagd.h
 *
 * File System Delayed Diag commands.
 * Copyright (C) 2017 QUALCOMM Technologies, Inc.
 *
 * File System Delayed Diag commands.
 *
 ***********************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/efs/src/fs_diagd.h#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2017-01-11   rp    Create

===========================================================================*/

#ifndef __FS_DIAGD_H__
#define __FS_DIAGD_H__

#include "comdef.h"
#include "fs_config_i.h"

#ifdef FEATURE_FS_DIAGD

#include "diagpkt.h"
#include "diagcmd.h"

#ifndef FS_DIAGD_DIAG_SUBSYS_FS
  #ifdef EFS_DIAG_USE_THIS_SUBSYS_ID
    #define FS_DIAGD_DIAG_SUBSYS_FS  (EFS_DIAG_USE_THIS_SUBSYS_ID)
  #else
    #define FS_DIAGD_DIAG_SUBSYS_FS  (DIAG_SUBSYS_FS)
  #endif
#endif

/*----------------------------------------------------------------------
 * EFS Delayed Diag commands
 *--------------------------------------------------------------------*/
#define FS_DIAGD_CMD_UNUSED              1000
#define FS_DIAGD_CMD_SEQ_GEN             1001
#define FS_DIAGD_CMD_GET_EFS_IMAGE       1002
#define FS_DIAGD_CMD_GET_TRANSFER_CONFIG 1003
#define FS_DIAGD_CMD_SET_TRANSFER_CONFIG 1004
#define FS_DIAGD_CMD_MAX                 1005


/*----------------------------------------------------------------------
 * Configuration options
 *--------------------------------------------------------------------*/
#ifndef FS_DIAGD_MAX_PATH_LEN
#define FS_DIAGD_MAX_PATH_LEN  (100)
#endif

#ifndef FS_DIAGD_MAX_PACKETS_IN_TRANSIT_QUEUE
#define FS_DIAGD_MAX_PACKETS_IN_TRANSIT_QUEUE (25)
#endif

#ifndef FS_DIAGD_DEFAULT_PACKETS_IN_TRANSIT_QUEUE
#define FS_DIAGD_DEFAULT_PACKETS_IN_TRANSIT_QUEUE (5)
#endif

#ifndef FS_DIAGD_MAX_SLEEP_TIME_IN_MS_DURING_TRANSIT_QUEUE_POLL
#define FS_DIAGD_MAX_SLEEP_TIME_IN_MS_DURING_TRANSIT_QUEUE_POLL (50)
#endif

#ifndef FS_DIAGD_DEFAULT_SLEEP_TIME_IN_MS_DURING_TRANSIT_QUEUE_POLL
#define FS_DIAGD_DEFAULT_SLEEP_TIME_IN_MS_DURING_TRANSIT_QUEUE_POLL (5)
#endif


/*----------------------------------------------------------------------
 * Sequence Generator Command
 *--------------------------------------------------------------------*/
PACKED struct fs_diagd_seq_gen_req_type {
  diagpkt_subsys_header_type hdr;
  uint32 seq_no;
  uint16 rsp_count;
} PACKED_POST;

PACKED struct fs_diagd_seq_gen_imm_rsp_type {
  diagpkt_subsys_header_v2_type hdr;
  uint32 err_code;
  uint32 seq_no;
  uint16 rsp_count;
} PACKED_POST;

PACKED struct fs_diagd_seq_gen_del_rsp_type {
  diagpkt_subsys_header_v2_type hdr;
  uint32 err_code;
  uint32 seq_no;
  uint16 rsp_count;
  uint16 rsp_id;
  uint32 incr_seq_no;
} PACKED_POST;

/*----------------------------------------------------------------------
 * Get EFS image Command
 *--------------------------------------------------------------------*/
PACKED struct fs_diagd_get_efs_image_req_type {
  diagpkt_subsys_header_type hdr;
  uint32 seq_no;
  uint8  image_type;
  uint16 path_len;
  char   path[1];
} PACKED_POST;

PACKED struct fs_diagd_get_efs_image_imm_rsp_type {
  diagpkt_subsys_header_v2_type hdr;
  uint32 err_code;
  uint32 seq_no;
  uint32 default_pkt_size_in_bytes;
  uint32 default_pkts_in_transit_queue;
  uint32 default_sleep_time_in_ms_while_polling_transit_queue;
  uint64 cur_time_in_ms;
} PACKED_POST;

PACKED struct fs_diagd_get_efs_image_del_rsp_type {
  diagpkt_subsys_header_v2_type hdr;
  uint32 err_code;
  uint32 seq_no;
  uint16 rsp_id;
  uint8  is_last_pkt;
  uint16 data_size;
  uint8  data[1];
} PACKED_POST;

/*----------------------------------------------------------------------
 * Get transfer config Command
 *--------------------------------------------------------------------*/
PACKED struct fs_diagd_get_transfer_config_req_type {
  diagpkt_subsys_header_type hdr;
  uint32 seq_no;
} PACKED_POST;

PACKED struct fs_diagd_get_transfer_config_imm_rsp_type {
  diagpkt_subsys_header_v2_type hdr;
  uint32 err_code;
  uint32 seq_no;
  uint32 max_pkt_size_in_bytes;
  uint32 max_pkts_in_transit_queue;
  uint32 max_sleep_time_in_ms_while_polling_transit_queue;
  uint32 default_pkt_size_in_bytes;
  uint32 default_pkts_in_transit_queue;
  uint32 default_sleep_time_in_ms_while_polling_transit_queue;
} PACKED_POST;

/*----------------------------------------------------------------------
 * Set transfer config Command
 *--------------------------------------------------------------------*/
PACKED struct fs_diagd_set_transfer_config_req_type {
  diagpkt_subsys_header_type hdr;
  uint32 seq_no;
  uint32 default_pkt_size_in_bytes;
  uint32 default_pkts_in_transit_queue;
  uint32 default_sleep_time_in_ms_while_polling_transit_queue;
} PACKED_POST;

PACKED struct fs_diagd_set_transfer_config_imm_rsp_type {
  diagpkt_subsys_header_v2_type hdr;
  uint32 err_code;
  uint32 seq_no;
  uint32 default_pkt_size_in_bytes;
  uint32 default_pkts_in_transit_queue;
  uint32 default_sleep_time_in_ms_while_polling_transit_queue;
} PACKED_POST;


/*----------------------------------------------------------------------
 * Exposed functions from the fs-delayed-diag module.
 *--------------------------------------------------------------------*/
void fs_diagd_init (void);
void fs_diagd_signal_received (void);

#else /* FEATURE_FS_DIAGD */

static inline void
fs_diagd_init (void)
{
}

static inline void
fs_diagd_signal_received (void)
{
}

#endif /* FEATURE_FS_DIAGD */

#endif /* not __FS_DIAGD_H__ */
