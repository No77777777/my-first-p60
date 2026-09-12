#ifndef NVRUIM_H
#define NVRUIM_H
/*==========================================================================

         N V    R - U I M    I N T E R N A L    H E A D E R    F I L E

DESCRIPTION
  This header file contains all the internal interface definitions for
  the NVRUIM subsystem that are used within NVRUIM.

REFERENCES
  IS-820, Removable User Identity Module (RUIM) for Spread Spectrum Systems

Copyright (c) 2009 - 2016 by QUALCOMM Technologies Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR FILE

$Header:

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/21/16   bcho    Handling of parallel requests in NVRUIM
04/13/16   ar      CDMA/HDR support on 2nd SUB
10/02/15   kk      Redirect requests to NV during QCN Restore operation
07/15/14   yt      Support for NV refresh
04/01/14   av      Moving definitions used by NVRUIM task under nvruim/src
02/22/14   av      Replace UIM enums with MMGSDI enums
10/17/13   at      SIM busy support in NVRUIM
05/13/13   vs      Decoupling NV-EFS from NV-RUIM
05/10/12   tl      Fixed MCC 414 to correct spec value
04/25/12   tl      Added support for MCC 0x158 and 0x19E
03/12/12   yt      Move data types from public header
11/11/11   ssr     Ignore the fallback request for non CT card
06/27/11   yt      Removed featurization from UIM public headers
05/16/11   ssr     Added NVRUIM thread safe support
02/08/11   ssr     Moved HRPD check from nvruim to mmgsdi layer
11/02/10   rm      Externize the nv_rtre_control API
08/12/10   ssr     Fixed nvruim 1x primary session for slot2
08/10/10   js      Updated nvruim get data sync to support 3gpd control data
06/18/10   ssr     Add non 3gpd cdma card check
06/17/10   js      Support for fetching nvruim data
                   on fusion
05/13/10   vs      Fix for NV and GSDI deadlock
04/30/10   ssr     Fixed nvruim clear cache
04/23/10   ssr     Fixed UIM 3GPD NV support
02/18/10   vs      Subscription mask fix
02/15/10   nb      Moved for Dual Slot Changes
11/17/09   yb      Moving nvruim_init_operator_card to nvruimi_v.h
09/30/09   mib     Added PACKED_POST for Q6 compilation
08/24/09   ssr     Fixed compilation error
02/24/09   nb      Initial Revision

===========================================================================*/


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "nv.h"
#include "mmgsdilib.h"
#include "nvruim_p.h"

/*===========================================================================

                           DATA DECLARATIONS

===========================================================================*/
/* ----------------------------------------------------------------------------

   ENUM:      NV_RUIM_SUPPORT_STATUS

   DESCRIPTION:
     Status as to whether or not a particular NV item is supported by the
     R-UIM subsystem.
-------------------------------------------------------------------------------*/
typedef enum {
  NV_RUIM_ITEM_NOT_SUPPORTED  = 0,    /* item is not supported by R-UIM  */
  NV_RUIM_SUPPORTS_ITEM       = 1     /* item is supported by R-UIM      */
} nv_ruim_support_status;

/* ----------------------------------------------------------------------------

   ENUM:      NV_RUIM_QCN_RESTORE_STATUS

   DESCRIPTION:
      Status of qcn restore in progress, as read from EFS 
      /nvruim/qcn_restore_inprogress location.
-------------------------------------------------------------------------------*/
typedef enum {
  NV_RUIM_QCN_RESTORE_UNKNOWN         = 0,  /* Restore status unknown  */
  NV_RUIM_QCN_RESTORE_IN_PROGRESS     = 1,  /* Restore in progress        */
  NV_RUIM_QCN_RESTORE_NOT_IN_PROGRESS = 2   /* Restore not in progress   */
} nv_ruim_qcn_restore_status;

/* ----------------------------------------------------------------------------

   ENUM:      NVRUIM_CMD_ENUM_TYPE

   DESCRIPTION:
     Describes the type of NVRUIM Request
-------------------------------------------------------------------------------*/
typedef enum {
  NVRUIM_READ              = 0x00,
  NVRUIM_WRITE,
  NVRUIM_FILE_ATTR
} nvruim_cmd_enum_type;

/* ----------------------------------------------------------------------------

   ENUM:      NVRUIM_REQUEST_ENUM_TYPE

   DESCRIPTION:
     Describes the NVRUIM command received
-------------------------------------------------------------------------------*/
typedef enum {
  NVRUIM_SWITCH_SESSION   = 0x00,
  NVRUIM_NV_ACCESS
} nvruim_request_enum_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_ACCESS_REQ_TYPE

   DESCRIPTION:
     Typedef for the NV request received
-------------------------------------------------------------------------------*/
typedef struct {
  nvruim_nv_context_type   context;
  nv_cmd_type             *nv_cmd_ptr;
} nvruim_access_req_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_REQ_ENUM_TYPE

   DESCRIPTION:
     Typedef for differentiating between NV req and other req
-------------------------------------------------------------------------------*/
typedef struct {
  q_link_type              link;
  nvruim_request_enum_type nvruim_req;
  union {
    nvruim_access_req_type    nvruim_access_req;
    nvruim_session_enum_type  nvruim_session;
  } cmd;
} nvruim_req_type;

/* ----------------------------------------------------------------------------

   STRUCTURE:      NVRUIM_SESSION_DATA_TYPE

   DESCRIPTION:....Contains NVRUIM globals defined for each session
-------------------------------------------------------------------------------*/
typedef struct{
  /* The following item indicates whether the CARD Supports extended 3GPD
     functionality as first defined on OMH on top of IS-820-C.  Gets initialized by
     nvruim_data_3gpd_init_extensions_support which is called from DATA. */
  boolean                       ext_support_3gpd;
  /* Set as default the ESN Usage indicator to return ESN_ME for ESN */
  nvruim_esn_usage_type         esn_usage;
  boolean                       bcsms_svc;
  boolean                       smscap_svc;  
  /* Control Variable which determines whether support for 3GPD should come from
     the R-UIM or from NV */
  nvruim_3gpd_support_status    control_3gpd;
  nvruim_3gpd_nv_card_status    non_3gpd_cdma_card;  
  mmgsdi_protocol_enum_type     uim_instr_prot;
  mmgsdi_session_id_type        session_id;
  boolean                       mmgsdi_session_active;
  mmgsdi_slot_id_enum_type      mmgsdi_slot_id;
  boolean                       hrpd_cdma_svc;
  /* This variable is used to indicate whether the current R-UIM card supports
     HRPD AN Authentication, as per the CDMA service table */
  nvruim_an_hrpd_support_status hrpd_control;
  boolean                       hrpd_allow_to_fallback_to_cave;
  boolean                       hrpd_disabled_card_flag;
  boolean                       uim_use_cave_default;
  boolean                       mip_svc_3gpd;
  boolean                       sip_svc_3gpd;
  /* The following item indicates whether the CARD Supports GPS functionality
     (Service Number 24 in CDMA Service Table) */
  boolean                       nvruim_lbs_support;

}nvruim_session_data_type;

extern nvruim_session_data_type  *nvruim_session_data_ptr[NVRUIM_NUM_1X_SESSION_MAX];
extern boolean                    nvruim_sim_busy[NVRUIM_MAX_CARD_COUNT];

/* EFS file which indicates QCN Restore in progress */
#define NVRUIM_QCN_RESTORE_IN_PROGRESS_EFS "/nvruim/qcn_restore_inprogress"
#define NVRUIM_EFS_DIR                     "/nvruim"

/*=============================================================================

                          NVRUIM MACROS
               MACRO Definitions used through NVRUIM

=============================================================================*/
/* ===========================================================================
   MACRO:       NVRUIM_RETURN_IF_NULL
   DESCRIPTION: Will check the parameter provided and return
                if the pointer is NULL
   ===========================================================================*/
#define NVRUIM_RETURN_IF_NULL(p_var)                                 \
  if ( (p_var) == NULL  )                                            \
  {                                                                  \
    return;                                                          \
  }

/* ===========================================================================
   MACRO:       NVRUIM_RETURN_IF_NULL_2
   DESCRIPTION: Will check the parameter provided and return
                if the pointer is NULL
   ===========================================================================*/
#define NVRUIM_RETURN_IF_NULL_2(p_var1, p_var2)                      \
  if ( (p_var1 == NULL) || (p_var2 == NULL) )                        \
  {                                                                  \
    return;                                                          \
  }

/*===========================================================================
MACRO        NVRUIM_CONTEXT_NOT_VALID
DESCRIPTION
  Check whether the context is valid or not
===========================================================================*/
#define NVRUIM_CONTEXT_NOT_VALID(nvruim_context)    \
  (nvruim_context != NVRUIM_NV_CONTEXT_PRIMARY&&    \
   nvruim_context != NVRUIM_NV_CONTEXT_SECONDARY&&  \
   nvruim_context != NVRUIM_NV_CONTEXT_TERTIARY)

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION NVRUIM_WRITE

DESCRIPTION
  Check to see if the item is supported by the R-UIM.
  If it is supported, write it.

DEPENDENCIES
  None

RETURN VALUE
  NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
  NV_RUIM_SUPPORTS_ITEM         Item is supported, see op_status

  op_status indicates the success/failure of the write.  For items
  which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
  None

===========================================================================*/
nv_ruim_support_status nvruim_write
(
  nv_cmd_type             *nv_cmd_ptr,           /* command block               */
  nvruim_nv_context_type   context,
  nv_stat_enum_type       *op_status             /* status of the I/O operation */
);

/*===========================================================================

FUNCTION NVRUIM_READ

DESCRIPTION
  Check to see if the item is supported by the R-UIM.
  If it is supported, read it.

DEPENDENCIES
  None

RETURN VALUE
  NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
  NV_RUIM_SUPPORTS_ITEM         Item is supported, see op_status

  op_status indicates the success/failure of the read.  For items
  which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
  None

===========================================================================*/
nv_ruim_support_status nvruim_read
(
  nv_cmd_type             *nv_cmd_ptr,       /* command block               */
  nvruim_nv_context_type   context,
  nv_stat_enum_type       *op_status         /* status of the I/O operation */
);

/*===========================================================================

FUNCTION nvruim_is_bcsms_svc_avail

DESCRIPTION
  Gets the bcsms_svc state

RETURN VALUE
  Boolean

DEPENDENCIES/SIDE EFFECTS/NOTES
  None

===========================================================================*/
boolean nvruim_is_bcsms_svc_avail
(
  nvruim_nv_context_type    context
);

/*===========================================================================

FUNCTION nvruim_is_smscap_svc_avail

DESCRIPTION
  Gets the smscap_svc state

RETURN VALUE
  Boolean

DEPENDENCIES/SIDE EFFECTS/NOTES
  None

===========================================================================*/
boolean nvruim_is_smscap_svc_avail
(
  nvruim_nv_context_type    context
);

/*===========================================================================

FUNCTION NVRUIM_IS_LBS_SUPPORT_AVAIL

DESCRIPTION
  The function returns whether or not the RUIM/CSIM card have support for the
  LBS service the service table

RETURN VALUE
  Boolean

DEPENDENCIES/SIDE EFFECTS/NOTES
  None

===========================================================================*/
boolean nvruim_is_lbs_support_avail
(
  nvruim_nv_context_type    context
);

/*===========================================================================

FUNCTION NVRUIM_SEND_MMGSDI_CANCEL

DESCRIPTION
  This function is responsible for cancelling NVRUIM's pending command(s)
  with MMGSDI.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void nvruim_send_mmgsdi_cancel
(
  nvruim_nv_context_type     context
);

/*===========================================================================

FUNCTION nvruim_is_mmgsdi_item_enabled

DESCRIPTION
  Checks if the NV/EFS item is enabled for the appropriate index

RETURN VALUE
  Boolean

DEPENDENCIES/SIDE EFFECTS/NOTES
  None

===========================================================================*/
boolean nvruim_is_mmgsdi_item_enabled
(
  mmgsdi_cached_nv_item_enum_type  item,
  nvruim_nv_context_type           context
);

/*===========================================================================

FUNCTION NVRUIM_MEM_ALLOC

DESCRIPTION
  This function calls modem_mem_calloc() to allocate from Modem Heap.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  a pointer is allocated.
===========================================================================*/
void * nvruim_mem_malloc
(
  dword size
);

/*===========================================================================

FUNCTION NVRUIM_MEM_FREE_NULL_OK

DESCRIPTION
  The NVRUIM_MEM_FREE free the pointer from the Modem Heap. If the pointer
  intended to be free'd is already NULL, the functions just prints a
  message and returns.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  the pointer is freed, if non-null.
===========================================================================*/
void nvruim_mem_free_null_ok
(
  void * ptr
);

/*===========================================================================

FUNCTION NVRUIM_PROCESS_SWITCH_1X_SESSION

DESCRIPTION
  This function process the NVRUIM_SWITCH_SESSION req to switch to the
  input session type

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void nvruim_process_switch_1x_session
(
  nvruim_request_enum_type     nvruim_req,
  nvruim_session_enum_type     nvruim_session
);

#endif /* NVRUIMI_H */
