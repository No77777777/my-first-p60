/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====
 *
 *                 MCFG Diagnostics Packet Processing
 *
 *
 *  Copyright (C) 2002-2014  QUALCOMM Technologies, Incorporated.
 *  All Rights Reserved.
 *
 *  Diagnostic packet processing routines for MCFG
 *
 *
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


 /*===========================================================================

                           Edit History

  $Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_diag/src/mcfg_diag.c#2 $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/16/15   sbt      Initial Creation
===========================================================================*/

#include "comdef.h"
#include "diagbuf.h"
#include "diagcmd.h"
#include "diagpkt.h"
#include "mcfg_fs.h"
#include "mcfg_int.h"
#include "mcfg_feature_config.h"
#include <stringl/stringl.h>

#ifdef FEATURE_MCFG_DIAG_SUPPORT

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "fs_public.h"
#include "fs_errno.h"
#include "diag.h"
#include "mcfg_diag.h"

#include "mcfg_diag_i.h"
#include "mcfg_common.h"
#include "mcfg_utils.h"
#include "mcfg_uim.h"
#include "mcfg_validate.h"
#include "mcfg_trl.h"
#include "mcfg_setting.h"
#include "timer.h"
#include "mcfg_refresh_i.h"
#include "mcfg_svc.h"
#include "mcfg_osal.h"


/* -------------------------------------------------------------------------
** Constants and Macros
** ------------------------------------------------------------------------- */

/*==============================================================================
  Variables
==============================================================================*/


/* -------------------------------------------------------------------------
** Definitions
** ------------------------------------------------------------------------- */
	
#ifdef FEATURE_MCFG_DIAG_SUPPORT_EXTENDED
#ifdef  DIAG_MAX_RX_PKT_SIZ
  #define MCFG_LARGEST_DIAG_PACKET     DIAG_MAX_RX_PKT_SIZ
#else
  #define MCFG_LARGEST_DIAG_PACKET     0x800
#endif

typedef struct {
  uint32 targ_pkt_window;       /* Target window size in packets            */
  uint32 targ_byte_window;      /* Target window size in bytes              */
  uint32 host_pkt_window;       /* Host window size in packets              */
  uint32 host_byte_window;      /* Host window size in bytes                */
  uint32 iter_pkt_window;       /* Dir iteration window size in packets     */
  uint32 iter_byte_window;      /* Dir iteration window size in bytes       */
  int32  version;               /* Protocol version number                  */
  int32  min_version;           /* Minimum supported protocol version       */
  int32  max_version;           /* Maximum supported protocol version       */
  int32  feature_bits;          /* Bit mask of supported features           */
} mcfg_fs_diag_params_type;
#endif /*FEATURE_MCFG_DIAG_SUPPORT_EXTENDED*/


/* Lint wants parens around macro but compile will fail */
/*lint -save -e773 */
#define VOID_PACKED_PTR        PACKED void * PACKED_POST
#define MCFG_DECLARE_DIAG_FN(fn)    VOID_PACKED_PTR fn (VOID_PACKED_PTR, uint16)
/*lint -restore */

#ifdef FEATURE_MCFG_DIAG_SUPPORT_EXTENDED
MCFG_DECLARE_DIAG_FN (mcfg_fs_diag_put_handler);
MCFG_DECLARE_DIAG_FN (mcfg_fs_diag_get_handler);
MCFG_DECLARE_DIAG_FN (mcfg_fs_diag_stat_handler);
#endif /*FEATURE_MCFG_DIAG_SUPPORT_EXTENDED*/
MCFG_DECLARE_DIAG_FN (mcfg_diag_request_handler);


/* -------------------------------------------------------------------------
** Local Data
** ------------------------------------------------------------------------- */
#ifdef FEATURE_MCFG_DIAG_SUPPORT_EXTENDED
static mcfg_fs_diag_params_type mcfg_fs_diag_params;
static const mcfg_fs_diag_params_type mcfg_fs_diag_default_params = {
  MCFG_FS_TARG_PKT_WINDOW_DEFAULT,
  MCFG_FS_TARG_BYTE_WINDOW_DEFAULT,
  MCFG_FS_HOST_PKT_WINDOW_DEFAULT,
  MCFG_FS_HOST_BYTE_WINDOW_DEFAULT,
  MCFG_FS_ITER_PKT_WINDOW_DEFAULT,
  MCFG_FS_ITER_BYTE_WINDOW_DEFAULT,
  MCFG_FS_DIAG_VERSION,
  MCFG_FS_DIAG_MIN_VERSION,
  MCFG_FS_DIAG_MAX_VERSION,
  MCFG_FS_FEATURE_BITS
};
#endif /* FEATURE_MCFG_DIAG_SUPPORT_EXTENDED */


/* The dispatch table for the MCFG subsystem.
*/
static const diagpkt_user_table_entry_type mcfg_diag_tbl[] =
{
#ifdef FEATURE_MCFG_DIAG_SUPPORT_EXTENDED
  {MCFG_FS_DIAG_PUT,     MCFG_FS_DIAG_PUT,   mcfg_fs_diag_put_handler},
  {MCFG_FS_DIAG_GET,     MCFG_FS_DIAG_GET,   mcfg_fs_diag_get_handler},
  {MCFG_FS_DIAG_STAT,    MCFG_FS_DIAG_STAT,  mcfg_fs_diag_stat_handler},
#endif /* FEATURE_MCFG_DIAG_SUPPORT_EXTENDED */
};

/* The dispatch table for the MCFG subsystem with delayed response.
*/
static const diagpkt_user_table_entry_type mcfg_diag_delayed_rsp_tbl[] =
{
  {MCFG_SVC_CMD_INDICATION_REGISTER,    MCFG_SVC_CMD_INDICATION_REGISTER,   mcfg_diag_request_handler},
  {MCFG_SVC_CMD_GET_SELECTED_CONFIG,    MCFG_SVC_CMD_GET_SELECTED_CONFIG,   mcfg_diag_request_handler},
  {MCFG_SVC_CMD_SET_SELECTED_CONFIG,    MCFG_SVC_CMD_SET_SELECTED_CONFIG,   mcfg_diag_request_handler},
  {MCFG_SVC_CMD_LIST_CONFIG,            MCFG_SVC_CMD_LIST_CONFIG,           mcfg_diag_request_handler},
  {MCFG_SVC_CMD_DELETE_CONFIG,          MCFG_SVC_CMD_DELETE_CONFIG,         mcfg_diag_request_handler},
  {MCFG_SVC_CMD_LOAD_CONFIG,            MCFG_SVC_CMD_LOAD_CONFIG,           mcfg_diag_request_handler},
  {MCFG_SVC_CMD_ACTIVATE_CONFIG,        MCFG_SVC_CMD_ACTIVATE_CONFIG,       mcfg_diag_request_handler},
  {MCFG_SVC_CMD_GET_CONFIG_INFO,        MCFG_SVC_CMD_GET_CONFIG_INFO,       mcfg_diag_request_handler},
  {MCFG_SVC_CMD_VALIDATE_CONFIG,        MCFG_SVC_CMD_VALIDATE_CONFIG,       mcfg_diag_request_handler},
  {MCFG_SVC_CMD_GET_FEATURE,            MCFG_SVC_CMD_GET_FEATURE,           mcfg_diag_request_handler},
  {MCFG_SVC_CMD_SET_FEATURE,            MCFG_SVC_CMD_SET_FEATURE,           mcfg_diag_request_handler},
  {MCFG_SVC_CMD_GET_CONFIG,             MCFG_SVC_CMD_GET_CONFIG,            mcfg_diag_request_handler},
};

static diag_client_type diag_cache[] = 
{
  {MCFG_SVC_CMD_INDICATION_REGISTER,  0, 0},
  {MCFG_SVC_CMD_LIST_CONFIG,          0, 0},
  {MCFG_SVC_CMD_LOAD_CONFIG,          0, 0},
  {MCFG_SVC_CMD_ACTIVATE_CONFIG,      0, 0},
};

/*===========================================================================

FUNCTION MCFG_DIAG_INIT
DESCRIPTION
  Initialization function for this module. Registers the packet handlers
  and sets up local data structures.
============================================================================*/
void mcfg_diag_init (void)
{
  MCFG_MSG_LOW("====  Registering MCFG Diag cmds ==== ");
#ifdef FEATURE_MCFG_DIAG_SUPPORT_EXTENDED
  mcfg_fs_diag_params = mcfg_fs_diag_default_params;
#endif /* FEATURE_MCFG_DIAG_SUPPORT_EXTENDED */

  /* Register MCFG diag packets. */
  DIAGPKT_DISPATCH_TABLE_REGISTER (MCFG_SUBSYS, mcfg_diag_tbl);

  /*register table for delayed response*/
  DIAGPKT_DISPATCH_TABLE_REGISTER_V2_DELAY_PROC(DIAG_MODEM_PROC, DIAG_SUBSYS_CMD_VER_2_F, MCFG_SUBSYS,
        mcfg_diag_delayed_rsp_tbl);

  return;
} /* END mcfg_diag_init */


/*===========================================================================

  FUNCTION mcfg_diag_cache_init

  DESCRIPTION
    initiates the diag client cache information

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_diag_cache_init
(
  void
)
{
  pdc_service_type *svc = NULL;
  static boolean diag_cache_init = FALSE;
  /*-----------------------------------------------------------------------*/

  if(!diag_cache_init)
  {
    MCFG_MSG_LOW( "initialize diag cache in mcfg_svc global cache");
    svc = mcfg_svc_get_service_state();
    svc->diag_cache = diag_cache;
    svc->pdc_clients[DIAG_CLIENT_INDEX].qmi_client_handle = (struct qmi_client_handle_struct *)&DIAG_CLIENT_HANDLE;
    svc->pdc_clients[DIAG_CLIENT_INDEX].connected = TRUE;

    diag_cache_init = TRUE;
  }
} /* END mcfg_diag_cache_init */


/*===========================================================================

  FUNCTION mcfg_diag_cmd_cache_update

  DESCRIPTION
    updates the client cache information

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_diag_cmd_cache_update
(
  uint16_t message_id
)
  {
  uint16_t msg_id = 0;
  diag_client_type *diag_client = NULL;
  /*-----------------------------------------------------------------------*/

  /* refresh_event_ind and config_change_ind will use same cache as indication_register */
  msg_id = ((message_id == QMI_PDC_REFRESH_IND_V01) || 
            (message_id == QMI_PDC_CONFIG_CHANGE_IND_V01)) ? MCFG_SVC_CMD_INDICATION_REGISTER : message_id;

  diag_client = (diag_client_type *)mcfg_diag_cmd_get_client_cache(msg_id);
  if (diag_client != NULL)
  {
    diag_client->rsp_cnt += 1;
    }
} /* END mcfg_diag_cmd_cache_update */


/*===========================================================================

  FUNCTION mcfg_diag_cmd_get_client_cache

  DESCRIPTION
    lookup function for the diag_cache.

  DEPENDENCIES
    DIAG must be initialized

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void* mcfg_diag_cmd_get_client_cache
(
   uint16_t message_id
)
{
  pdc_service_type      *svc = NULL;
  diag_client_type     *diag_client = NULL;
  uint16_t             diag_cache_size = 0;
  uint8_t              offset = 0;
  /*-----------------------------------------------------------------------*/

  svc = mcfg_svc_get_service_state();
  diag_cache_size = sizeof(diag_cache)/sizeof(diag_client_type);
  diag_client = (diag_client_type *)svc->diag_cache;

  if (diag_client == NULL)
  {
    return NULL;
  }

  for (offset=0; offset<diag_cache_size; offset++)
  {
    if((diag_client + offset)->cmd_id == message_id)
    {
      return (diag_client + offset);
    }
  }

  return NULL;
} /* END mcfg_diag_cmd_get_client_cache */


/*===========================================================================

  FUNCTION mcfg_diag_cmd_resp

  DESCRIPTION
    sends a response for the diag command in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_diag_cmd_resp
(
  void *delayed_rsp,
  qmi_idl_type_of_message_type message_type,
  uint16_t message_id,
  void *resp,
  uint32_t resp_size,
  qmi_error_type_v01 error
)
{
  diagpkt_subsys_hdr_type_v2 *response = NULL;
  uint32_t tlv_len = 0;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do
  {
    if (delayed_rsp == NULL)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR_1("Error %d", err);
      break;
    }
    response = (diagpkt_subsys_hdr_type_v2 *)delayed_rsp;

    if((response->status == 0) && (resp != NULL))
    {
      tlv_len = qmi_idl_get_std_resp_tlv_len();
      if(qmi_idl_message_encode(pdc_get_service_object_v01(), 
                                message_type, 
                                message_id, 
                                resp, 
                                resp_size, 
                                response + 1, 
                                tlv_len,
                                &tlv_len) !=0)
      {
        response->status = QMI_ERR_MALFORMED_MSG_V01;
      }
    }
    else
    {
      response->status = QMI_ERR_MALFORMED_MSG_V01;
    }

    response->rsp_cnt = 0;
    response->delayed_rsp_id = !(response->status == 0 && error == 0) ? 0 : response->delayed_rsp_id;

    MCFG_MSG_LOW_1("subsys_cmd_code 0x%02x", response->subsys_cmd_code);
    MCFG_MSG_LOW_1("status 0x%08x", response->status);
    MCFG_MSG_LOW_1("delayed_rsp_id 0x%04x", response->delayed_rsp_id);
    MCFG_MSG_LOW_1("rsp_cnt 0x%04x", response->rsp_cnt);
    MCFG_MSG_LOW_1("error 0x%08x", error);

    MCFG_MSG_LOW_1("Sent response to diag with payload len %d", tlv_len);
    diagpkt_commit(response);

    mcfg_diag_cmd_cache_update(message_id);
  }while(0);

  /* free memory */
  mcfg_free(resp);
} /* END mcfg_diag_cmd_resp */


/*===========================================================================

  FUNCTION mcfg_diag_cmd_ind

  DESCRIPTION
    sends an indication for the diag command in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_diag_cmd_ind
(
  uint16_t delayed_rsp_id,
  uint16 rsp_cnt,
  qmi_idl_type_of_message_type message_type,
  uint16_t message_id,
  void *p_src,
  uint32_t src_len,
  uint32_t buffer_size,
  qmi_error_type_v01 error
)
{
  diagpkt_subsys_hdr_type_v2 *delayed_rsp = NULL;
  void *buffer = NULL;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do
  {
    if (error != QMI_ERR_NONE_V01)
    {
      err = error;
      MCFG_MSG_ERROR_1("Error %d", err);
      break;
    }

    if (!p_src)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR_1("Error %d", err);
      break;
    }

    buffer = mcfg_malloc (buffer_size);
    if (!buffer)
    {
      err = QMI_ERR_NO_MEMORY_V01;
      MCFG_MSG_ERROR_1("Error %d", err);
      break;
    }
    memset(buffer, 0, buffer_size);

    if(qmi_idl_message_encode(pdc_get_service_object_v01(), 
                              message_type, 
                              message_id, 
                              p_src, 
                              src_len, 
                              buffer, 
                              buffer_size,
                              &buffer_size))
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR_1("Error %d", err);
      break;
    }

    delayed_rsp = (diagpkt_subsys_hdr_type_v2 *)
      diagpkt_subsys_alloc_v2_delay (MCFG_SUBSYS,
                            message_id,
                            delayed_rsp_id,
                            (sizeof(diagpkt_subsys_hdr_type_v2) + buffer_size));
    if (delayed_rsp == NULL)
    {
      err = QMI_ERR_NO_MEMORY_V01;
      MCFG_MSG_ERROR_1("Error %d", err);
      break;
    }

    delayed_rsp->delayed_rsp_id = delayed_rsp_id;
    delayed_rsp->rsp_cnt = rsp_cnt;
    MCFG_MSG_LOW_1("delayed_rsp_id 0x%04x", delayed_rsp_id);
    MCFG_MSG_LOW_1("rsp_cnt 0x%04x", rsp_cnt);

    memscpy(delayed_rsp+1, buffer_size, buffer, buffer_size);
    MCFG_MSG_LOW_1("Sent delayed response to diag with payload len %d", buffer_size);
    diagpkt_delay_commit(delayed_rsp);

    mcfg_diag_cmd_cache_update(message_id);
  }while(0);

  mcfg_free(buffer);

  if(err != QMI_ERR_NONE_V01)
  {
    delayed_rsp = (diagpkt_subsys_hdr_type_v2 *)
      diagpkt_subsys_alloc_v2_delay (MCFG_SUBSYS,
                            message_id,
                            delayed_rsp_id,
                            sizeof(diagpkt_subsys_hdr_type_v2));
    if (!delayed_rsp)
    {
      MCFG_MSG_ERROR_1("Error %d", QMI_ERR_NO_MEMORY_V01);
      return;
    }

    delayed_rsp->delayed_rsp_id = delayed_rsp_id;
    delayed_rsp->rsp_cnt = rsp_cnt & 0x7fff;
    delayed_rsp->status = err;
    MCFG_MSG_LOW_1("delayed_rsp_id 0x%04x", delayed_rsp_id);
    MCFG_MSG_LOW_1("rsp_cnt 0x%04x", (rsp_cnt & 0x7fff));
    MCFG_MSG_LOW_1("status %d", err);

    MCFG_MSG_LOW_1("Sent delayed response to diag with error %d", err);
    diagpkt_delay_commit(delayed_rsp);

    mcfg_diag_cmd_cache_update(message_id);
  }
} /* END mcfg_diag_cmd_ind */



#ifdef FEATURE_MCFG_DIAG_SUPPORT_EXTENDED
/**********************************************************************
 * Packet pathname validation.
 *********************************************************************/

/* Count the number of nul characters in the given string. */
static unsigned
mcfg_fsd_count_nulls (PACKED char *  PACKED_POST name, unsigned length)
{
  unsigned i;
  unsigned result = 0;

  for (i = 0; i < length; i++) {
    if (name[i] == '\0')
      result++;
  }

  return result;
}

/* Convenience macro for checking pathnames.  Takes its argument, and
 * verifies that it contains at least as many nulls.  Arguments are:
 *   request - The typed request packet.
 *   type    - The type of the request packet.
 *   field   - The name of the field containing the pathname
 *   req_len - The total length of the packet
 *   min_nul - Miminum number of nul characters in the string.
 * This macro contains a return, and will return a diag error packet
 * indicating a bad parameter if the request doesn't contain sufficient
 * nulls. */
#define MCFG_FSD_CHECK_NULLS(_req, _type, _field, _req_len, _min_nul) \
  do { \
    if (mcfg_fsd_count_nulls ((_req)->_field, \
          (_req_len) - offsetof (_type, _field)) < (_min_nul)) \
    { \
      return diagpkt_err_rsp (DIAG_BAD_PARM_F, \
          (void *) (_req), (_req_len)); \
    } \
  } while (0)
/**********************************************************************
 * Packet length validation.
 *********************************************************************/

static int
mcfg_fs_diag_is_valid_pkt_len (uint32 pkt_len, uint32 min_pkt_len)
{
  /* Validate pkt_len range */
  if (pkt_len > MCFG_LARGEST_DIAG_PACKET || pkt_len < min_pkt_len)
  {
    return 0;
  }
  return 1;
}


/**********************************************************************
 * Convert the error code given in the target to the one published in
 * efs diag documentation
 *********************************************************************/
uint32
mcfg_fs_get_diag_errno (uint32 posix_errno)
{
  uint32 diag_errno;

  switch (posix_errno)
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
      diag_errno = posix_errno;
      break;
  }

  return diag_errno;
}

/*===========================================================================

FUNCTION MCFG_FS_DIAG_STAT_HANDLER
DESCRIPTION
  Handles the MCFG_FS_DIAG_STAT packet.
============================================================================*/
VOID_PACKED_PTR
mcfg_fs_diag_stat_handler (VOID_PACKED_PTR req_ptr, uint16 pkt_len)
{
  mcfg_fs_diag_stat_req_type *request;
  mcfg_fs_diag_stat_rsp_type *response;
  uint16 rsp_len = sizeof (mcfg_fs_diag_stat_rsp_type);
  struct fs_stat sbuf;
  const char *request_path;

  /*----------------------------------------------------------------------
    Check security, since this is a secure funciton
    ----------------------------------------------------------------------*/
  if (diag_get_security_state() != DIAG_SEC_UNLOCKED) {
    return( diagpkt_err_rsp(DIAG_BAD_SEC_MODE_F, req_ptr, pkt_len) );
  }

  if (pkt_len < sizeof (mcfg_fs_diag_stat_req_type))
    return diagpkt_err_rsp (DIAG_BAD_LEN_F, req_ptr, pkt_len);

  request = (mcfg_fs_diag_stat_req_type *) req_ptr;
  request_path = (const char *) request->path;

  MCFG_FSD_CHECK_NULLS (request, mcfg_fs_diag_stat_req_type,
      path, pkt_len, 1);

  response = (mcfg_fs_diag_stat_rsp_type *)
    diagpkt_subsys_alloc (MCFG_SUBSYS,
                          MCFG_FS_DIAG_STAT,
                          rsp_len);
  if (response == NULL)
    return NULL;

  MCFG_MSG_SPRINTF_2("fpath: %s, sub_id: %d", request_path, request->sub_id);

  if (mcfg_fs_stat (request_path, 
                &sbuf,
                request->fs_type,
                request->sub_id) == 0)
  {
    response->mode   = (int32) sbuf.st_mode;
    response->size   = (int32) sbuf.st_size;
    response->nlink  = (int32) sbuf.st_nlink;
    response->atime  = (int32) sbuf.st_atime;
    response->mtime  = (int32) sbuf.st_mtime;
    response->ctime  = (int32) sbuf.st_ctime;
    response->diag_errno  = 0;
  }
  else
  {
    response->mode   = 0;
    response->size   = 0;
    response->nlink  = 0;
    response->atime  = 0;
    response->mtime  = 0;
    response->ctime  = 0;
    response->diag_errno  = mcfg_fs_get_diag_errno (efs_errno);
  }

  return response;
} /* END mcfg_fs_diag_stat_handler */


/*===========================================================================

FUNCTION MCFG_FS_DIAG_GET_HANDLER
DESCRIPTION
  Handles the MCFG_FS_DIAG_GET packet. The field sequence_number is used for the
  purpose of Synchronization / Correlation purpose. The sequence_number
  received is fetched out of request packet and tucked back into the response
  packet. This would benefit the application with synchronized requests and
  responses.

============================================================================*/
VOID_PACKED_PTR
mcfg_fs_diag_get_handler (VOID_PACKED_PTR req_ptr, uint16 pkt_len)
{
  mcfg_fs_diag_get_req_type *request;
  mcfg_fs_diag_get_rsp_type *response;
  unsigned rsp_len = 0;
  unsigned base_length = 0;
  mcfg_fs_status_e_type read_status = MCFG_FS_STATUS_OK;
  const char *request_path;
  uint16 seq_num;

  /*----------------------------------------------------------------------
    Check security, since this is a secure function
    ----------------------------------------------------------------------*/
  if (diag_get_security_state () != DIAG_SEC_UNLOCKED) {
    return( diagpkt_err_rsp (DIAG_BAD_SEC_MODE_F, req_ptr, pkt_len));
  }

  if (pkt_len < sizeof (mcfg_fs_diag_get_req_type))
    return diagpkt_err_rsp (DIAG_BAD_LEN_F, req_ptr, pkt_len);

  request = (mcfg_fs_diag_get_req_type *) req_ptr;
  request_path = (const char *) request->path;
  /*
   * Fetch the Sequence number commonly used by the calling application
   * for Synchronization / Correlation purpose.
   */
  seq_num = request->sequence_number;

  MCFG_FSD_CHECK_NULLS (request, mcfg_fs_diag_get_req_type, path,
      pkt_len, 1);

  if (request->data_length > MCFG_FS_DIAG_MAX_READ_REQ) {
    return diagpkt_err_rsp (DIAG_BAD_LEN_F, req_ptr, pkt_len);
  }

  base_length = FPOS (mcfg_fs_diag_get_rsp_type, data);
  rsp_len = base_length + request->data_length;

  response = (mcfg_fs_diag_get_rsp_type *)
    diagpkt_subsys_alloc (MCFG_SUBSYS,
                          MCFG_FS_DIAG_GET,
                          rsp_len);
  if (response == NULL)
    return NULL;

  /* Acknowledge back with the Sequence number */
  response->sequence_number = seq_num;

  MCFG_MSG_SPRINTF_3("fpath: %s, fsz: %d, sub_id: %d", request_path, request->data_length, request->sub_id);

  read_status = mcfg_fs_read (request_path,
                        (void *) response->data,
                        request->data_length,
                        request->fs_type,
                        request->sub_id );

  if (read_status == MCFG_FS_STATUS_OK) {
    response->diag_errno      = 0;
  } else { 
    response->diag_errno = mcfg_fs_get_diag_errno (efs_errno);
    diagpkt_shorten (response, base_length);
  }

  return response;
} /* END mcfg_fs_diag_get_handler */


/*===========================================================================

FUNCTION MCFG_FS_DIAG_PUT_HANDLER
DESCRIPTION
  Handles the MCFG_FS_DIAG_PUT packet. The field sequence_number is used for the
  purpose of Synchronization / Correlation purpose. The sequence_number
  received is fetched out of request packet and tucked back into the response
  packet. This would benefit the application with synchronized requests and
  responses.

============================================================================*/
VOID_PACKED_PTR
mcfg_fs_diag_put_handler (VOID_PACKED_PTR req_ptr, uint16 pkt_len)
{
  mcfg_fs_diag_put_req_type *request;
  mcfg_fs_diag_put_rsp_type *response;
  uint16 rsp_len = sizeof (mcfg_fs_diag_put_rsp_type);
  fs_ssize_t nbyte;
  mcfg_fs_status_e_type write_status = MCFG_FS_STATUS_OK;
  unsigned   data_offset;
  char      *put_locn;
  char      *put_path;
  uint16 seq_num;

  /*----------------------------------------------------------------------
    Check security, since this is a secure function
    ----------------------------------------------------------------------*/
  if (diag_get_security_state () != DIAG_SEC_UNLOCKED) {
    return( diagpkt_err_rsp (DIAG_BAD_SEC_MODE_F, req_ptr, pkt_len));
  }

  if (pkt_len < sizeof (mcfg_fs_diag_put_req_type))
    return diagpkt_err_rsp (DIAG_BAD_LEN_F, req_ptr, pkt_len);

  request = (mcfg_fs_diag_put_req_type *) req_ptr;

  put_locn = (char *) request->data;
  nbyte    = request->data_length;
  put_path = (char *) request->data + nbyte;
  data_offset = FPOS (mcfg_fs_diag_put_req_type, data);
  /*
   * Fetch the Sequence number usually used by the calling application
   * for Synchronization / Correlation purpose.
   */
  seq_num = request->sequence_number;

  /* Validate this packet, more complicated than others, because there is a
   * null-terminated string after variable length data. */
  if ((nbyte + data_offset >= pkt_len) ||
      (mcfg_fsd_count_nulls (put_path, pkt_len - (data_offset + nbyte)) < 1))
  {
    return (diagpkt_err_rsp (DIAG_BAD_LEN_F, req_ptr, pkt_len));
  }

  response = (mcfg_fs_diag_put_rsp_type *)
    diagpkt_subsys_alloc (MCFG_SUBSYS,
                          MCFG_FS_DIAG_PUT,
                          rsp_len);
  if (response == NULL)
    return NULL;

  /* Acknowledge back with the Sequence number */
  response->sequence_number = seq_num;

  MCFG_MSG_SPRINTF_3("fpath: %s, fsz: %d, sub_id: %d", put_path, request->data_length, request->sub_id);

  write_status = mcfg_fs_write (put_path,
                           (void *) put_locn,
                           nbyte, request->flags,
                           0777, 
                           request->fs_type,
                           request->sub_id );

  if (write_status != MCFG_FS_STATUS_OK)
  {
    response->diag_errno = mcfg_fs_get_diag_errno (efs_errno);
  }
  else
  {
    response->diag_errno = 0;
  }
  return response;
} /* END mcfg_fs_diag_put_handler */


/*
 * Populuates and returns the error respoonse packet.
 *
 * NOTE:The error passed should be the POSIX or extended errors
 * only this function will perform the mapping before sending the response.
 */
VOID_PACKED_PTR
mcfg_fs_diag_efs2_error_rsp (int32 diag_errno, VOID_PACKED_PTR req_pkt,
                        uint16 req_len)
{
  mcfg_fs_diag_error_rsp_type *response;
  unsigned int rsp_len;

  /* Lint is too stupid to realize that offsetof() has no side
   * effects, so it prohibits the use of the MIN() macro here (error
   * 666).  So we spell it out using small words instead. */
  rsp_len = req_len + offsetof (mcfg_fs_diag_error_rsp_type, pkt);
  if (rsp_len > sizeof (mcfg_fs_diag_error_rsp_type))
    rsp_len = sizeof (mcfg_fs_diag_error_rsp_type);

  response = (mcfg_fs_diag_error_rsp_type *)
    diagpkt_subsys_alloc (MCFG_SUBSYS,
                          MCFG_FS_DIAG_ERR,
                          rsp_len);
  if (response == NULL)
    return NULL;

  response->diag_errno = mcfg_fs_get_diag_errno (diag_errno);

  memscpy ((void *) response->pkt,
              rsp_len - offsetof (mcfg_fs_diag_error_rsp_type, pkt),
              (void *) req_pkt,
              rsp_len - offsetof (mcfg_fs_diag_error_rsp_type, pkt));

  return ((void *) response);
}/* END mcfg_fs_diag_efs2_error_rsp */
#endif /*FEATURE_MCFG_DIAG_SUPPORT_EXTENDED*/

/*===========================================================================

FUNCTION MCFG_DIAG_REQUEST_HANDLER
DESCRIPTION
  Handles the MCFG_DIAG_CMD request packets.
============================================================================*/
VOID_PACKED_PTR
mcfg_diag_request_handler (VOID_PACKED_PTR req_ptr, uint16 pkt_len)
{
  void *pdc_req = NULL;
  void *pdc_rsp = NULL;
  diagpkt_subsys_header_type *request = NULL;
  diagpkt_subsys_hdr_type_v2 *response = NULL;
  const mcfg_svc_cmd_entry_type *svc = NULL;
  mcfg_svc_post_cmd_type post_cmd;
  uint32_t tlv_len = 0;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/
  
  MCFG_MSG_LOW_1("mcfg_diag_request_handler pkt len %d", pkt_len);
  mcfg_diag_cache_init();

  do
  {
    MCFG_CHECK_NULL_PTR_RET_NULL(req_ptr);

    svc = (const mcfg_svc_cmd_entry_type *) mcfg_svc_cmd_get_handler(MCFG_DIAG_SUBSYS_CMD_CODE(req_ptr));
    MCFG_CHECK_NULL_PTR_RET_NULL(svc);

    request = (diagpkt_subsys_header_type *)req_ptr;

    pdc_rsp = mcfg_malloc(svc->size_of_resp_type);
    MCFG_CHECK_NULL_PTR_RET_NULL(pdc_rsp);

    tlv_len = qmi_idl_get_std_resp_tlv_len();
    response = (diagpkt_subsys_hdr_type_v2 *)
      diagpkt_subsys_alloc_v2 (MCFG_SUBSYS,
                               svc->cmd_code,
                               sizeof(diagpkt_subsys_hdr_type_v2)+tlv_len);
    if (!response)
    {
      error = QMI_ERR_NO_MEMORY_V01;
      MCFG_MSG_ERROR_1("mcfg_diag_request_handler: not enough memory %d", error);
      mcfg_free(pdc_rsp);
      return NULL;
    }

    post_cmd.data.diag_post.delayed_rsp_id = response->delayed_rsp_id;
    post_cmd.data.diag_post.delayed_rsp = response;
    post_cmd.type = DIAG_CMD;

    /*----------------------------------------------------------------------
    Check security, since this is a secure funciton
    ----------------------------------------------------------------------*/
    if (diag_get_security_state() != DIAG_SEC_UNLOCKED) 
    {
      response->status = QMI_ERR_INCOMPATIBLE_STATE_V01;
      break;
    }

    if (pkt_len <= sizeof (diagpkt_subsys_header_type))
    {
      response->status = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR_2("mcfg_diag_request_handler incorrect pkt len %d error %d", pkt_len, QMI_ERR_MALFORMED_MSG_V01);
      break;
    }

    pdc_req = mcfg_malloc(svc->size_of_req_type);
    if (!pdc_req)
    {
      error = QMI_ERR_NO_MEMORY_V01;
      MCFG_MSG_ERROR_1("mcfg_diag_request_handler: not enough memory %d", error);
      break;
    }

    if(qmi_idl_message_decode(pdc_get_service_object_v01(), 
                              QMI_IDL_REQUEST, 
                              svc->cmd_code, 
                              request+1, 
                              pkt_len-sizeof(diagpkt_subsys_header_type), 
                              pdc_req, 
                              svc->size_of_req_type))
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR_1("mcfg_diag_request_handler: req decoding error %d", error);
      break;
    }
  }while(0);

  error = svc->handler_ptr(post_cmd, 
                           pdc_req, 
                           pkt_len, 
                           pdc_rsp, 
                           svc->size_of_resp_type);

  return NULL;
}/* END mcfg_diag_request_handler */


#endif /* FEATURE_MCFG_DIAG_SUPPORT */
