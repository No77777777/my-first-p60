/*===========================================================================
             Public header just to include fs_diag_access.h

  Copyright (c) 2012-2013 by QUALCOMM Technologies Incorporated.
  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            Edit History

  $Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_diag/inc/mcfg_diag.h#1 $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2015-03-25   sbt    created.
===========================================================================*/

#include "fs_diag_access.h"
#include "diagcmd.h"
#include "qmi_idl_lib.h"
#include "common_v01.h"


/** Identifies the major version number of this API file. */
#define VER_MCFG_FS_DIAG_H_MAJOR     1
/** Identifies the minor version number of this API file. */
#define VER_MCFG_FS_DIAG_H_MINOR     1

/*
 * Initialization function for this module. Registers the packet handlers
 * and sets up local data structures.
 */
void mcfg_diag_init(void);


/* -------------------------------------------------------------------------
 * Definitions of MCFG diagnostic packets.
 * ------------------------------------------------------------------------- */
#ifdef FEATURE_MCFG_DIAG_SUPPORT

#define MCFG_SUBSYS DIAG_SUBSYS_MCFG
#define MCFG_DIAG_CMD_CODE(ptr)           * ((uint8 *) ptr)
#define MCFG_DIAG_SUBSYS_ID(ptr)          * ((uint8 *) ptr + 1)
#define MCFG_DIAG_SUBSYS_CMD_CODE(ptr)    * (uint16 *)((uint8 *) ptr + 2)


/*
 * Header Packet v2
 *
 * used for delayed responses
 */
typedef PACKED struct
{
  uint8 cmd_code;
  uint8 subsys_id;
  uint16 subsys_cmd_code;
  uint32 status;  
  uint16 delayed_rsp_id;
  uint16 rsp_cnt; /* 0, means one response and 1, means two responses */
} diagpkt_subsys_hdr_type_v2;


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
);


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
);


/*===========================================================================

  FUNCTION mcfg_diag_cmd_get_client_cache

  DESCRIPTION
    lookup function for the diag_cache.

  DEPENDENCIES
    None

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
);


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
);


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
);


#endif /* FEATURE_MCFG_DIAG_SUPPORT */