/***********************************************************************
 * fs_diagd.c
 *
 * File System Delayed Diag commands.
 * Copyright (C) 2017 QUALCOMM Technologies, Inc.
 *
 * File System Delayed Diag Commands.
 *
 ***********************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/efs/src/fs_diagd.c#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2017-03-14   rp    Move to the new diag apis.
2017-01-11   rp    Create

===========================================================================*/

#include "fs_config_i.h"

#ifdef FEATURE_FS_DIAGD

#include "fs_diagd.h"
#include "fs_osal.h"
#include "fs_err.h"
#include "fs_util.h"
#include "fs_task_i.h"
#include "fs_priv_funcs.h"
#include "fs_public.h"
#include "fs_timetick.h"
#include "fs_errno.h"

#include "diagcmd.h"
#include "time_svc.h"
#include "stringl/stringl.h"

#include <stddef.h>

/*----------------------------------------------------------------------
 * Debug Information.
 *--------------------------------------------------------------------*/
struct fs_diagd_debug_info
{
  uint32 total_malloc_count;
  uint32 failed_malloc_count;
  uint32 malloc_retry_count;

  uint32 total_malloc_size;
  uint32 total_delay_rsp_pkt;
  uint32 total_time_in_us;

  uint32 efs_image_size;
};

struct fs_diagd_cmd_q_node
{
  uint32 cmd_id;
  uint32 pkt_len;
  diagpkt_subsys_delayed_rsp_id_type delayed_rsp_id;

  union
  {
    struct fs_diagd_seq_gen_req_type seq_gen_req;
    struct fs_diagd_get_efs_image_req_type get_efs_image_req;
  } pkt;

  char path[FS_DIAGD_MAX_PATH_LEN];
};

struct fs_diagd_info_type
{
  fs_os_tcb_type  *fs_task_tcb;

  int cmd_q_count;
  struct fs_diagd_cmd_q_node cmd_q[1];

  uint32 max_pkt_size_in_bytes;
  uint32 pkt_size_in_bytes;

  uint32 max_pkt_in_transit_q;
  uint32 max_pkt_size_in_transit_q;
  uint32 curr_pkt_size_in_transit_q;

  uint32 sleep_time_in_ms;

  struct fs_diagd_debug_info debug_info;
};
static struct fs_diagd_info_type fs_diagd_info;


static uint32
fs_diagd_map_efs_errno_to_diag_errno (uint32 efs_errno_val)
{
  uint32 diag_errno = ENOTHINGTOSYNC;

  switch (efs_errno_val)
  {
    case EPERM:
      diag_errno = 1;
      break;
    case ENOENT:
      diag_errno = 2;
      break;
    case EEXIST:
      diag_errno = 6;
      break;
    case EBADF:
      diag_errno = 9;
      break;
    case ENOMEM:
      diag_errno = 12;
      break;
    case EACCES:
      diag_errno = 13;
      break;
    case EBUSY:
      diag_errno = 16;
      break;
    case EXDEV:
      diag_errno = 18;
      break;
    case ENODEV:
      diag_errno = 19;
      break;
    case ENOTDIR:
      diag_errno = 20;
      break;
    case EISDIR:
      diag_errno = 21;
      break;
    case EINVAL:
      diag_errno = 22;
      break;
    case EMFILE:
      diag_errno = 24;
      break;
    case ETXTBSY:
      diag_errno = 26;
      break;
    case ENOSPC:
      diag_errno = 28;
      break;
    case ESPIPE:
      diag_errno = 29;
      break;
    case FS_ERANGE:
      diag_errno = 34;
      break;
    case ENAMETOOLONG:
      diag_errno = 36;
      break;
    case ENOTEMPTY:
      diag_errno = 39;
      break;
    case ELOOP:
      diag_errno = 40;
      break;
    case ETIMEDOUT:
      diag_errno = 110;
      break;
    case ESTALE:
      diag_errno = 116;
      break;
    case EDQUOT:
      diag_errno = 122;
      break;
    case ENOCARD:
      diag_errno = 301;
      break;
    case EBADFMT:
      diag_errno = 302;
      break;
    case ENOTITM:
      diag_errno = 303;
      break;
    case EROLLBACK:
      diag_errno = 304;
      break;
    case ENOTHINGTOSYNC:
      diag_errno = 306;
      break;
    case EEOF:
      diag_errno = 0x8000;
      break;
    case EUNKNOWN_SFAT:
      diag_errno = 0x8001;
      break;
    case EUNKNOWN_HFAT:
      diag_errno = 0x8002;
      break;
    default:
      /* Do Nothing if we dont need to convert */
      diag_errno = efs_errno_val;
      break;
  }

  return diag_errno;
}


/*----------------------------------------------------------------------
  Cmd node functions
 *--------------------------------------------------------------------*/
static struct fs_diagd_cmd_q_node* fs_diagd_cmd_node_alloc(void)
{
  struct fs_diagd_cmd_q_node *cmd_node = NULL;

  if (fs_diagd_info.cmd_q_count == 0)
  {
    cmd_node = &fs_diagd_info.cmd_q[0];
    fs_diagd_info.cmd_q_count = 1;
  }

  return cmd_node;
}

static void fs_diagd_cmd_node_free(struct fs_diagd_cmd_q_node *cmd_node)
{
  FS_ASSERT (cmd_node != NULL);
  FS_ASSERT (fs_diagd_info.cmd_q_count == 1);
  fs_diagd_info.cmd_q_count = 0;
}

static struct fs_diagd_cmd_q_node* fs_diagd_cmd_node_get (void)
{
  struct fs_diagd_cmd_q_node *cmd_node = NULL;

  if (fs_diagd_info.cmd_q_count == 1)
  {
    cmd_node = &fs_diagd_info.cmd_q[0];
  }

  return cmd_node;
}

/*----------------------------------------------------------------------
 * Diag response packet malloc helper
 *--------------------------------------------------------------------*/
static void* fs_diagd_rsp_pkt_malloc (diagpkt_subsys_cmd_code_type cmd_id,
        diagpkt_subsys_delayed_rsp_id_type delayed_rsp_id, uint32 pkt_len)
{
  void *rsp_pkt = NULL;

  FS_ASSERT (cmd_id < FS_DIAGD_CMD_MAX);
  FS_ASSERT (pkt_len <= fs_diagd_info.pkt_size_in_bytes);

  fs_diagd_info.curr_pkt_size_in_transit_q =
     (uint32 )diagpkt_total_delayed_bytes_queued ();

  FS_MSG_LOW_2 ("[%d, %d], rsp_pkt_malloc",
                  fs_diagd_info.curr_pkt_size_in_transit_q,
                  fs_diagd_info.max_pkt_size_in_transit_q);

  while (fs_diagd_info.curr_pkt_size_in_transit_q >=
         fs_diagd_info.max_pkt_size_in_transit_q)
  {
    FS_MSG_LOW_2 ("[%d, %d], rsp_pkt_malloc wait",
                  fs_diagd_info.curr_pkt_size_in_transit_q,
                  fs_diagd_info.max_pkt_size_in_transit_q);

    ++fs_diagd_info.debug_info.malloc_retry_count;
    fs_util_wait (fs_diagd_info.sleep_time_in_ms);

    fs_diagd_info.curr_pkt_size_in_transit_q =
      (uint32 )diagpkt_total_delayed_bytes_queued ();
  }

  ++fs_diagd_info.debug_info.total_malloc_count;
  fs_diagd_info.debug_info.total_malloc_size += pkt_len;

  rsp_pkt = (void *)diagpkt_subsys_alloc_v2_delay (FS_DIAGD_DIAG_SUBSYS_FS,
                      cmd_id, delayed_rsp_id, pkt_len);

  if (rsp_pkt == NULL)
  {
    ++fs_diagd_info.debug_info.failed_malloc_count;
    FS_MSG_ERROR_3 ("[%d, %d, %d], rsp_pkt_malloc failed",
                  fs_diagd_info.curr_pkt_size_in_transit_q,
                  fs_diagd_info.max_pkt_size_in_transit_q,
                  pkt_len);
  }

  FS_MSG_LOW_2 ("[%d, %d], rsp_pkt_malloc done",
                fs_diagd_info.curr_pkt_size_in_transit_q,
                fs_diagd_info.max_pkt_size_in_transit_q);

  return rsp_pkt;
}

/*----------------------------------------------------------------------
 * Sequence Gen diagd command handler
 *--------------------------------------------------------------------*/
void*
fs_diagd_cmd_seq_gen (void *req_pkt_arg, uint16 req_pkt_len_arg)
{
  struct fs_diagd_seq_gen_req_type *req_pkt;
  struct fs_diagd_seq_gen_imm_rsp_type *rsp_pkt;
  uint32 req_pkt_len, rsp_pkt_len;
  struct fs_diagd_cmd_q_node *cmd_node = NULL;

  FS_MSG_HIGH_1 ("diagd : [%d] seq_gen cmd", req_pkt_len_arg);

  req_pkt_len = sizeof (struct fs_diagd_seq_gen_req_type);
  req_pkt = (struct fs_diagd_seq_gen_req_type *)req_pkt_arg;

  if ((req_pkt == NULL) || (req_pkt_len_arg != req_pkt_len))
  {
    FS_MSG_ERROR_2 ("diagd : [%d, %d] invalid pkt len",
                    req_pkt_len_arg, req_pkt_len);
    return diagpkt_err_rsp (DIAG_BAD_LEN_F, req_pkt_arg, req_pkt_len_arg);
  }

  rsp_pkt_len = sizeof (struct fs_diagd_seq_gen_imm_rsp_type);
  rsp_pkt = (struct fs_diagd_seq_gen_imm_rsp_type *)
              diagpkt_subsys_alloc_v2 (FS_DIAGD_DIAG_SUBSYS_FS,
              FS_DIAGD_CMD_SEQ_GEN, rsp_pkt_len);
  if (rsp_pkt == NULL)
  {
    FS_MSG_ERROR_1 ("diagd : [%d] malloc fail", rsp_pkt_len);
    return diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt_arg, req_pkt_len_arg);
  }

  diagpkt_subsys_set_status (rsp_pkt, 0);
  diagpkt_subsys_set_rsp_cnt (rsp_pkt, 0x8000);

  rsp_pkt->err_code = ENOTHINGTOSYNC;
  rsp_pkt->seq_no = req_pkt->seq_no;
  rsp_pkt->rsp_count = req_pkt->rsp_count;

  cmd_node = fs_diagd_cmd_node_alloc ();
  if (cmd_node == NULL)
  {
    FS_MSG_ERROR_0 ("diagd : cmd q full");
    rsp_pkt->err_code = EMFILE;
    rsp_pkt->err_code=fs_diagd_map_efs_errno_to_diag_errno (rsp_pkt->err_code);
    goto End;
  }

  memset(cmd_node, 0, sizeof(*cmd_node));

  cmd_node->cmd_id = FS_DIAGD_CMD_SEQ_GEN;
  cmd_node->pkt_len = req_pkt_len;
  fs_memscpy (&cmd_node->pkt.seq_gen_req, sizeof (cmd_node->pkt.seq_gen_req),
              req_pkt, req_pkt_len);

  cmd_node->delayed_rsp_id = diagpkt_subsys_get_delayed_rsp_id (rsp_pkt);

  rsp_pkt->err_code = 0;

  fs_os_set_sigs (fs_diagd_info.fs_task_tcb, FS_DIAGD_SIG);

  FS_MSG_HIGH_2 ("diagd : [%d, %d] seq_gen cmd queued",
                 req_pkt->seq_no, req_pkt->rsp_count);

End:
  return rsp_pkt;
}

static void fs_diagd_cmd_seq_gen_task (struct fs_diagd_cmd_q_node *cmd_node)
{
  struct fs_diagd_seq_gen_req_type *req_pkt = NULL;
  struct fs_diagd_seq_gen_del_rsp_type *rsp_pkt = NULL;
  uint32 req_pkt_len, rsp_pkt_len, rsp_count;
  fs_timetick start_timetick, net_timeticks;
  uint32 total_time_in_us;
  uint16 i;

  start_timetick = fs_timetick_get_timetick ();

  req_pkt_len = sizeof (struct fs_diagd_seq_gen_req_type);
  rsp_pkt_len = sizeof (struct fs_diagd_seq_gen_del_rsp_type);

  FS_ASSERT (cmd_node != NULL);
  FS_ASSERT (cmd_node->cmd_id == FS_DIAGD_CMD_SEQ_GEN);
  FS_ASSERT (cmd_node->pkt_len == req_pkt_len);

  FS_MSG_HIGH_0 ("diagd : seq_gen task");

  memset (&fs_diagd_info.debug_info, 0, sizeof(fs_diagd_info.debug_info));

  req_pkt = &cmd_node->pkt.seq_gen_req;

  for ( i = 0; i < req_pkt->rsp_count; ++i )
  {
    rsp_pkt = fs_diagd_rsp_pkt_malloc (FS_DIAGD_CMD_SEQ_GEN,
                     cmd_node->delayed_rsp_id, rsp_pkt_len);
    if (rsp_pkt == NULL)
    {
      FS_MSG_ERROR_1 ("diagd : [%d] malloc fail", rsp_pkt_len);
      goto Error;
    }

    rsp_pkt->err_code = ENOTHINGTOSYNC;

    if (i == (req_pkt->rsp_count - 1))
    {
      rsp_count = (i + 1);
    }
    else
    {
      rsp_count = (0x8000 + i + 1);
    }

    diagpkt_subsys_set_status (rsp_pkt, 0);
    diagpkt_subsys_set_rsp_cnt (rsp_pkt, rsp_count);

    rsp_pkt->seq_no = req_pkt->seq_no;
    rsp_pkt->rsp_count = req_pkt->rsp_count;
    rsp_pkt->rsp_id = (i + 1);
    rsp_pkt->incr_seq_no = (uint32)(rsp_pkt->seq_no + rsp_pkt->rsp_id);

    rsp_pkt->err_code = 0;

    FS_MSG_LOW_2 ("[%d, %d] diagd : seq-gen rsp", i, rsp_count);

    ++fs_diagd_info.debug_info.total_delay_rsp_pkt;

    diagpkt_delay_commit (rsp_pkt);
  }

Error:
  net_timeticks = fs_timetick_get_timetick () - start_timetick;
  total_time_in_us = (uint32) fs_timetick_timetick_diff_to_us (net_timeticks);
  fs_diagd_info.debug_info.total_time_in_us = total_time_in_us;

  FS_MSG_HIGH_3 ("diagd : [%d, %d, %d] : debug info",
    fs_diagd_info.debug_info.total_malloc_count,
    fs_diagd_info.debug_info.failed_malloc_count,
    fs_diagd_info.debug_info.malloc_retry_count);

  FS_MSG_HIGH_3 ("diagd : [%d, %d, %d] : debug info",
    fs_diagd_info.debug_info.total_malloc_size,
    fs_diagd_info.debug_info.total_delay_rsp_pkt,
    fs_diagd_info.debug_info.total_time_in_us);

  return;
}


/*----------------------------------------------------------------------
 * Get EFS image diagd command handler
 *--------------------------------------------------------------------*/
void*
fs_diagd_cmd_get_efs_image (void *req_pkt_arg, uint16 req_pkt_len_arg)
{
  struct fs_diagd_get_efs_image_req_type *req_pkt;
  struct fs_diagd_get_efs_image_imm_rsp_type *rsp_pkt;
  uint32 req_pkt_len, rsp_pkt_len, path_len;
  struct fs_diagd_cmd_q_node *cmd_node = NULL;

  FS_MSG_HIGH_1 ("diagd : [%d] get efs image cmd", req_pkt_len_arg);

  req_pkt_len = sizeof (struct fs_diagd_get_efs_image_req_type);
  req_pkt = (struct fs_diagd_get_efs_image_req_type *)req_pkt_arg;

  if ((req_pkt == NULL) || (req_pkt_len_arg < req_pkt_len))
  {
    FS_MSG_ERROR_2 ("diagd : [%d, %d] invalid pkt len",
                    req_pkt_len_arg, req_pkt_len);
    return diagpkt_err_rsp (DIAG_BAD_LEN_F, req_pkt_arg, req_pkt_len_arg);
  }

  path_len = strlen (req_pkt->path) + 1;
  if ((path_len != req_pkt->path_len) ||
      (path_len > FS_DIAGD_MAX_PATH_LEN))
  {
    FS_MSG_ERROR_2 ("diagd : [%d, %d] invalid path len",
                    req_pkt_len_arg, req_pkt_len);
    return diagpkt_err_rsp (DIAG_BAD_LEN_F, req_pkt_arg, req_pkt_len_arg);
  }

  rsp_pkt_len = sizeof (struct fs_diagd_get_efs_image_imm_rsp_type);
  rsp_pkt = (struct fs_diagd_get_efs_image_imm_rsp_type *)
              diagpkt_subsys_alloc_v2 (FS_DIAGD_DIAG_SUBSYS_FS,
              FS_DIAGD_CMD_GET_EFS_IMAGE, rsp_pkt_len);
  if (rsp_pkt == NULL)
  {
    FS_MSG_ERROR_1 ("diagd : [%d] malloc fail", rsp_pkt_len);
    return diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt_arg, req_pkt_len_arg);
  }

  diagpkt_subsys_set_status (rsp_pkt, 0);
  diagpkt_subsys_set_rsp_cnt (rsp_pkt, 0x8000);

  rsp_pkt->err_code = ENOTHINGTOSYNC;
  rsp_pkt->seq_no = req_pkt->seq_no;

  rsp_pkt->default_pkt_size_in_bytes = fs_diagd_info.pkt_size_in_bytes;
  rsp_pkt->default_pkts_in_transit_queue = fs_diagd_info.max_pkt_in_transit_q;
  rsp_pkt->default_sleep_time_in_ms_while_polling_transit_queue =
    fs_diagd_info.sleep_time_in_ms;
  rsp_pkt->cur_time_in_ms = time_get_ms_native ();

  cmd_node = fs_diagd_cmd_node_alloc ();
  if (cmd_node == NULL)
  {
    FS_MSG_ERROR_0 ("diagd : cmd q full");
    rsp_pkt->err_code = EMFILE;
    rsp_pkt->err_code=fs_diagd_map_efs_errno_to_diag_errno (rsp_pkt->err_code);
    goto End;
  }

  memset (cmd_node, 0, sizeof(*cmd_node));

  cmd_node->cmd_id = FS_DIAGD_CMD_GET_EFS_IMAGE;
  cmd_node->pkt_len = req_pkt_len;
  fs_memscpy (&cmd_node->pkt.get_efs_image_req,
              sizeof (cmd_node->pkt.get_efs_image_req),
              req_pkt, req_pkt_len);
  strlcpy (cmd_node->path, req_pkt->path, sizeof(cmd_node->path));

  cmd_node->delayed_rsp_id = diagpkt_subsys_get_delayed_rsp_id (rsp_pkt);

  rsp_pkt->err_code = 0;

  fs_os_set_sigs (fs_diagd_info.fs_task_tcb, FS_DIAGD_SIG);

  FS_MSG_HIGH_2 ("diagd : [%d, %d] get_efs cmd queued",
                 req_pkt->seq_no, req_pkt->image_type);

End:
  return rsp_pkt;
}

static void
fs_diagd_cmd_get_efs_image_task (struct fs_diagd_cmd_q_node *cmd_node)
{
  struct fs_diagd_get_efs_image_req_type *req_pkt = NULL;
  struct fs_diagd_get_efs_image_del_rsp_type *rsp_pkt = NULL;
  uint32 req_pkt_len, rsp_pkt_len, rsp_count, hdr_size;
  uint32 read_size, payload_size, new_payload_size;
  int result, image_hdl = -1, run_loop;
  int allow_device_specific_files, allow_secure_files;
  fs_timetick start_timetick, net_timeticks;
  uint32 total_time_in_us;
  uint16 i;

  start_timetick = fs_timetick_get_timetick ();

  req_pkt_len = sizeof (struct fs_diagd_get_efs_image_req_type);
  rsp_pkt_len = fs_diagd_info.pkt_size_in_bytes;
  hdr_size = offsetof(struct fs_diagd_get_efs_image_del_rsp_type, data);
  payload_size = (rsp_pkt_len - hdr_size);

  FS_ASSERT (cmd_node != NULL);
  FS_ASSERT (cmd_node->cmd_id == FS_DIAGD_CMD_GET_EFS_IMAGE);
  FS_ASSERT (cmd_node->pkt_len == req_pkt_len);

  FS_MSG_HIGH_0 ("diagd : get efs task");

  memset (&fs_diagd_info.debug_info, 0, sizeof(fs_diagd_info.debug_info));

  FS_MSG_HIGH_3 ("diagd : [%d, %d, %d] transfer config",
                 fs_diagd_info.pkt_size_in_bytes,
                 fs_diagd_info.max_pkt_in_transit_q,
                 fs_diagd_info.sleep_time_in_ms);

  FS_MSG_HIGH_3 ("diagd : [%d, %d, %d] payload size",
                 hdr_size, payload_size, rsp_pkt_len);

  req_pkt = &cmd_node->pkt.get_efs_image_req;

  allow_device_specific_files = 1;
  allow_secure_files = 0;

  result = efs_filesystem_image_open (cmd_node->path, req_pkt->image_type,
              allow_device_specific_files, allow_secure_files);
  if (result < 0)
  {
    int err_val = efs_errno;
    FS_MSG_ERROR_2 ("diagd : [%d, %d] image_open fail", err_val, result);
    goto Error;
  }

  image_hdl = result;

  run_loop = 1;
  for (i = 0; run_loop; ++i )
  {
    rsp_pkt = fs_diagd_rsp_pkt_malloc (FS_DIAGD_CMD_GET_EFS_IMAGE,
                 cmd_node->delayed_rsp_id, rsp_pkt_len);
    if (rsp_pkt == NULL)
    {
      FS_MSG_ERROR_1 ("diagd : [%d] malloc fail", rsp_pkt_len);
      goto Error;
    }

    rsp_pkt->err_code = ENOTHINGTOSYNC;
    rsp_pkt->seq_no = req_pkt->seq_no;
    rsp_pkt->rsp_id = (i + 1);

    diagpkt_subsys_set_status (rsp_pkt, 0);

    new_payload_size = (payload_size - 2);

    memset(rsp_pkt->data, 0xA3, payload_size);
    FS_ASSERT (rsp_pkt->data[new_payload_size + 0] == 0xA3);
    FS_ASSERT (rsp_pkt->data[new_payload_size + 1] == 0xA3);

    read_size = 0;
    result = efs_filesystem_image_read (image_hdl,
                     rsp_pkt->data, new_payload_size);
    if (result <= 0)
    {
      FS_MSG_ERROR_1 ("diagd : [%d] image_read fail", result);
      rsp_count = (i + 1);
      rsp_pkt->err_code = (result < 0) ? efs_errno : 0;
      rsp_pkt->err_code =
        fs_diagd_map_efs_errno_to_diag_errno(rsp_pkt->err_code);
      rsp_pkt->data_size = 0;
      rsp_pkt->is_last_pkt = 1;
      run_loop = 0;
    }
    else
    {
      read_size = (uint32)result;
      rsp_count = (0x8000 + i + 1);
      rsp_pkt->err_code = 0;
      rsp_pkt->data_size = read_size;
      rsp_pkt->is_last_pkt = 0;

      FS_ASSERT (rsp_pkt->data[new_payload_size + 0] == 0xA3);
      FS_ASSERT (rsp_pkt->data[new_payload_size + 1] == 0xA3);
    }

    diagpkt_subsys_set_rsp_cnt (rsp_pkt, rsp_count);

    FS_MSG_HIGH_2 ("[%d, %d] diagd : get-image-rsp", i, rsp_pkt->data_size);

    fs_diagd_info.debug_info.efs_image_size += rsp_pkt->data_size;

    ++fs_diagd_info.debug_info.total_delay_rsp_pkt;

    diagpkt_delay_commit (rsp_pkt);
  }

Error:
  goto End;

End:
  if (image_hdl >= 0)
  {
    efs_filesystem_image_close (image_hdl);
    image_hdl = -1;
  }

  net_timeticks = fs_timetick_get_timetick () - start_timetick;
  total_time_in_us = (uint32) fs_timetick_timetick_diff_to_us (net_timeticks);
  fs_diagd_info.debug_info.total_time_in_us = total_time_in_us;

  FS_MSG_HIGH_3 ("diagd : [%d, %d, %d] : debug info",
    fs_diagd_info.debug_info.total_malloc_count,
    fs_diagd_info.debug_info.failed_malloc_count,
    fs_diagd_info.debug_info.malloc_retry_count);

  FS_MSG_HIGH_3 ("diagd : [%d, %d, %d] : debug info",
    fs_diagd_info.debug_info.total_malloc_size,
    fs_diagd_info.debug_info.total_delay_rsp_pkt,
    fs_diagd_info.debug_info.total_time_in_us);

  FS_MSG_HIGH_1 ("diagd : [%d] : debug info",
    fs_diagd_info.debug_info.efs_image_size);

  return;
}

/*----------------------------------------------------------------------
 * Get Transfer Config command handler
 *--------------------------------------------------------------------*/
void*
fs_diagd_cmd_get_transfer_config (void *req_pkt_arg, uint16 req_pkt_len_arg)
{
  struct fs_diagd_get_transfer_config_req_type *req_pkt;
  struct fs_diagd_get_transfer_config_imm_rsp_type *rsp_pkt;
  uint32 req_pkt_len, rsp_pkt_len;

  FS_MSG_HIGH_1 ("diagd : [%d] get_transfer_cfg cmd", req_pkt_len_arg);

  req_pkt_len = sizeof (struct fs_diagd_get_transfer_config_req_type);
  req_pkt = (struct fs_diagd_get_transfer_config_req_type *)req_pkt_arg;

  if ((req_pkt == NULL) || (req_pkt_len_arg != req_pkt_len))
  {
    FS_MSG_ERROR_2 ("diagd : [%d, %d] invalid pkt len",
                    req_pkt_len_arg, req_pkt_len);
    return diagpkt_err_rsp (DIAG_BAD_LEN_F, req_pkt_arg, req_pkt_len_arg);
  }

  rsp_pkt_len = sizeof (struct fs_diagd_get_transfer_config_imm_rsp_type);
  rsp_pkt = (struct fs_diagd_get_transfer_config_imm_rsp_type *)
              diagpkt_subsys_alloc_v2 (FS_DIAGD_DIAG_SUBSYS_FS,
              FS_DIAGD_CMD_GET_TRANSFER_CONFIG, rsp_pkt_len);
  if (rsp_pkt == NULL)
  {
    FS_MSG_ERROR_1 ("diagd : [%d] malloc fail", rsp_pkt_len);
    return diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt_arg, req_pkt_len_arg);
  }

  diagpkt_subsys_set_status (rsp_pkt, 0);
  diagpkt_subsys_set_rsp_cnt (rsp_pkt, 0x8000);

  rsp_pkt->err_code = ENOTHINGTOSYNC;
  rsp_pkt->seq_no = req_pkt->seq_no;

  rsp_pkt->max_pkt_size_in_bytes = fs_diagd_info.max_pkt_size_in_bytes;
  rsp_pkt->max_pkts_in_transit_queue = FS_DIAGD_MAX_PACKETS_IN_TRANSIT_QUEUE;
  rsp_pkt->max_sleep_time_in_ms_while_polling_transit_queue =
    FS_DIAGD_MAX_SLEEP_TIME_IN_MS_DURING_TRANSIT_QUEUE_POLL;

  rsp_pkt->default_pkt_size_in_bytes = fs_diagd_info.pkt_size_in_bytes;
  rsp_pkt->default_pkts_in_transit_queue = fs_diagd_info.max_pkt_in_transit_q;
  rsp_pkt->default_sleep_time_in_ms_while_polling_transit_queue =
    fs_diagd_info.sleep_time_in_ms;

  rsp_pkt->err_code = 0;

  return rsp_pkt;
}

/*----------------------------------------------------------------------
 * Set Transfer Config command handler
 *--------------------------------------------------------------------*/
void*
fs_diagd_cmd_set_transfer_config (void *req_pkt_arg, uint16 req_pkt_len_arg)
{
  struct fs_diagd_set_transfer_config_req_type *req_pkt;
  struct fs_diagd_set_transfer_config_imm_rsp_type *rsp_pkt;
  uint32 req_pkt_len, rsp_pkt_len;

  FS_MSG_HIGH_1 ("diagd : [%d] set_transfer_cfg cmd", req_pkt_len_arg);

  req_pkt_len = sizeof (struct fs_diagd_set_transfer_config_req_type);
  req_pkt = (struct fs_diagd_set_transfer_config_req_type *)req_pkt_arg;

  if ((req_pkt == NULL) || (req_pkt_len_arg != req_pkt_len))
  {
    FS_MSG_ERROR_2 ("diagd : [%d, %d] invalid pkt len",
                    req_pkt_len_arg, req_pkt_len);
    return diagpkt_err_rsp (DIAG_BAD_LEN_F, req_pkt_arg, req_pkt_len_arg);
  }

  rsp_pkt_len = sizeof (struct fs_diagd_set_transfer_config_imm_rsp_type);
  rsp_pkt = (struct fs_diagd_set_transfer_config_imm_rsp_type *)
              diagpkt_subsys_alloc_v2 (FS_DIAGD_DIAG_SUBSYS_FS,
              FS_DIAGD_CMD_SET_TRANSFER_CONFIG, rsp_pkt_len);
  if (rsp_pkt == NULL)
  {
    FS_MSG_ERROR_1 ("diagd : [%d] malloc fail", rsp_pkt_len);
    return diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt_arg, req_pkt_len_arg);
  }

  diagpkt_subsys_set_status (rsp_pkt, 0);
  diagpkt_subsys_set_rsp_cnt (rsp_pkt, 0x8000);

  rsp_pkt->err_code = ENOTHINGTOSYNC;
  rsp_pkt->seq_no = req_pkt->seq_no;

  rsp_pkt->default_pkt_size_in_bytes = 0;
  rsp_pkt->default_pkts_in_transit_queue = 0;
  rsp_pkt->default_sleep_time_in_ms_while_polling_transit_queue = 0;

  if (req_pkt->default_pkt_size_in_bytes >
      fs_diagd_info.max_pkt_size_in_bytes)
  {
    FS_MSG_ERROR_2 ("diagd : [%d, %d] invalid pkt size",
                    req_pkt->default_pkt_size_in_bytes,
                    fs_diagd_info.pkt_size_in_bytes);
    rsp_pkt->err_code = EINVAL;
    goto End;
  }

  if (req_pkt->default_pkts_in_transit_queue >
      FS_DIAGD_MAX_PACKETS_IN_TRANSIT_QUEUE)
  {
    FS_MSG_ERROR_2 ("diagd : [%d, %d] invalid transit q size",
                    req_pkt->default_pkts_in_transit_queue,
                    fs_diagd_info.max_pkt_in_transit_q);
    rsp_pkt->err_code = EINVAL;
    goto End;
  }

  if (req_pkt->default_sleep_time_in_ms_while_polling_transit_queue >
      FS_DIAGD_MAX_SLEEP_TIME_IN_MS_DURING_TRANSIT_QUEUE_POLL)
  {
    FS_MSG_ERROR_2 ("diagd : [%d, %d] invalid sleep time",
       req_pkt->default_sleep_time_in_ms_while_polling_transit_queue,
                    FS_DIAGD_MAX_SLEEP_TIME_IN_MS_DURING_TRANSIT_QUEUE_POLL);
    rsp_pkt->err_code = EINVAL;
    goto End;
  }

  if (req_pkt->default_pkt_size_in_bytes != 0)
  {
    fs_diagd_info.pkt_size_in_bytes = req_pkt->default_pkt_size_in_bytes;
  }

  if (req_pkt->default_pkts_in_transit_queue != 0)
  {
    fs_diagd_info.max_pkt_in_transit_q =
      req_pkt->default_pkts_in_transit_queue;
    fs_diagd_info.max_pkt_size_in_transit_q =
      (fs_diagd_info.max_pkt_in_transit_q * fs_diagd_info.pkt_size_in_bytes);
  }

  if (req_pkt->default_sleep_time_in_ms_while_polling_transit_queue != 0)
  {
    fs_diagd_info.sleep_time_in_ms =
      req_pkt->default_sleep_time_in_ms_while_polling_transit_queue;
  }

  rsp_pkt->default_pkt_size_in_bytes = fs_diagd_info.pkt_size_in_bytes;
  rsp_pkt->default_pkts_in_transit_queue = fs_diagd_info.max_pkt_in_transit_q;
  rsp_pkt->default_sleep_time_in_ms_while_polling_transit_queue =
    fs_diagd_info.sleep_time_in_ms;

  FS_MSG_HIGH_3 ("diagd : [%d, %d, %d] set transfer config",
                 fs_diagd_info.pkt_size_in_bytes,
                 fs_diagd_info.max_pkt_in_transit_q,
                 fs_diagd_info.sleep_time_in_ms);

  rsp_pkt->err_code = 0;
End:
  rsp_pkt->err_code = fs_diagd_map_efs_errno_to_diag_errno (rsp_pkt->err_code);
  return rsp_pkt;
}

static const diagpkt_user_table_entry_type fs_diagd_tbl[] =
{
  {
    FS_DIAGD_CMD_SEQ_GEN,
    FS_DIAGD_CMD_SEQ_GEN,
    fs_diagd_cmd_seq_gen
  },
  {
    FS_DIAGD_CMD_GET_EFS_IMAGE,
    FS_DIAGD_CMD_GET_EFS_IMAGE,
    fs_diagd_cmd_get_efs_image
  },
  {
    FS_DIAGD_CMD_GET_TRANSFER_CONFIG,
    FS_DIAGD_CMD_GET_TRANSFER_CONFIG,
    fs_diagd_cmd_get_transfer_config
  },
  {
    FS_DIAGD_CMD_SET_TRANSFER_CONFIG,
    FS_DIAGD_CMD_SET_TRANSFER_CONFIG,
    fs_diagd_cmd_set_transfer_config
  },
};


void fs_diagd_signal_received (void)
{
  struct fs_diagd_cmd_q_node *cmd_node = NULL;

  FS_MSG_HIGH_0 ("diagd : signal rcvd");

  cmd_node = fs_diagd_cmd_node_get ();
  if (cmd_node == NULL)
  {
    FS_MSG_ERROR_0 ("diagd : cmd q empty");
    goto End;
  }

  FS_MSG_HIGH_2 ("diagd : [%d, %d] diagd task func",
                 cmd_node->cmd_id, cmd_node->pkt_len);

  switch (cmd_node->cmd_id)
  {
  case FS_DIAGD_CMD_SEQ_GEN:
    fs_diagd_cmd_seq_gen_task (cmd_node);
    break;

  case FS_DIAGD_CMD_GET_EFS_IMAGE:
    fs_diagd_cmd_get_efs_image_task (cmd_node);
    break;

  default:
    FS_MSG_ERROR_2 ("diagd : [%d, %d] invalid cmd",
                    cmd_node->cmd_id, cmd_node->pkt_len);
    break;
  }

  fs_diagd_cmd_node_free (cmd_node);

End:
  FS_MSG_HIGH_0 ("diagd : signal function complete");
}

void fs_diagd_init (void)
{
  uint32 max_pkt_size;

  memset (&fs_diagd_info, 0, sizeof(fs_diagd_info));

  max_pkt_size = (uint32 )diagpkt_max_delayed_rsp_size ();

  fs_diagd_info.fs_task_tcb = fs_os_self ();
  fs_diagd_info.max_pkt_size_in_bytes = max_pkt_size;
  fs_diagd_info.pkt_size_in_bytes = max_pkt_size;

  fs_diagd_info.max_pkt_in_transit_q =
    FS_DIAGD_DEFAULT_PACKETS_IN_TRANSIT_QUEUE;

  fs_diagd_info.max_pkt_size_in_transit_q =
    (fs_diagd_info.max_pkt_in_transit_q * max_pkt_size);

  fs_diagd_info.sleep_time_in_ms =
    FS_DIAGD_DEFAULT_SLEEP_TIME_IN_MS_DURING_TRANSIT_QUEUE_POLL;

  DIAGPKT_DISPATCH_TABLE_REGISTER_V2_DELAY (DIAG_SUBSYS_CMD_VER_2_F,
    DIAG_SUBSYS_FS, fs_diagd_tbl);
}

#endif /* FEATURE_FS_DIAGD */
