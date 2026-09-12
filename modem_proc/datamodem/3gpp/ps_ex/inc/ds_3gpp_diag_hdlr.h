#ifndef DS_3GPP_DIAG_HDLR_H
#define DS_3GPP_DIAG_HDLR_H
/*===========================================================================

                             DS 3GPP DIAG HDLR 

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for 3GPP Data Services diag handling.

  Copyright (c) 2015 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_diag_hdlr.h#2 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
07/07/15   fj      Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA_DIAG_SUPPORT
#ifdef FEATURE_DATA_3GPP
#include "ds3gdiagmgr.h"
#include "ds3gsubsmgr.h"

/*===========================================================================

            External DEFINITIONS AND DECLARATIONS FOR MODULE
 
===========================================================================*/
#define DS_3GPP_DIAG_UMTS_UL_WM_MASK      0x01
#define DS_3GPP_DIAG_UMTS_DL_WM_MASK      0x02
#define DS_3GPP_DIAG_LTE_UL_WM_MASK       0x04
#define DS_3GPP_DIAG_LTE_DL_WM_MASK       0x08

/*---------------------------------------------------------------------------
TYPEDEF DS_3GPP_DIAG_WM_RAT_TYPE

DESCRIPTION
  Enum of watermark RAT type
---------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_DIAG_WM_RAT_LTE = 0,
  DS_3GPP_DIAG_WM_RAT_UMTS = 1,
  DS_3GPP_DIAG_WM_RAT_MAX = DS_3GPP_DIAG_WM_RAT_UMTS
}ds_3gpp_diag_wm_rat_type;

/*---------------------------------------------------------------------------
TYPEDEF DS_3GPP_DIAG_WM_STRUCT_TYPE

DESCRIPTION
  Structure type holding the watermark update request from QXDM
---------------------------------------------------------------------------*/
typedef struct
{
  uint8   req_wm_mask;
  uint32  umts_ul_lo;
  uint32  umts_ul_hi;
  uint32  umts_ul_dne;
  uint32  umts_dl_lo;
  uint32  umts_dl_hi;
  uint32  umts_dl_dne;
  uint32  lte_ul_lo;
  uint32  lte_ul_hi;
  uint32  lte_ul_dne;
  uint32  lte_dl_lo;
  uint32  lte_dl_hi;
  uint32  lte_dl_dne;
}ds_3gpp_diag_wm_struct_type;

/*---------------------------------------------------------------------------
TYPEDEF DS_3GPP_DIAG_INITIAL_BEARER_SETUP

DESCRIPTION
  Enum for initial bearer type
---------------------------------------------------------------------------*/
typedef enum
{
  MCG_ONLY = 0,
  SCG_ONLY = 1,
  UL_MCG_SPLIT_DL = 2,
  UL_SCG_SPLIT_DL = 3
}ds_3gpp_diag_initial_bearer_setup;

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================
FUNCTION DS_3GPP_DIAG_HDLR_PROCESS_CMDS

DESCRIPTION
  This function processes the 3gpp diag commands

PARAMETERS
  diag_payload - payload to diag inputs

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - processing complete
  FALSE - processing failed

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_diag_hdlr_process_cmds
(
  ds3gdiagmgr_req_payload_type *diag_payload_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_DIAG_HDLR_GET_REQ_WM

DESCRIPTION
  This function returns ds_3gpp_diag_req_wm if client requests wm update from
  DIAG.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  NULL: if no one updates wm from DIAG.
  &ds_3gpp_diag_req_wm. 

SIDE EFFECTS
  None.

===========================================================================*/
ds_3gpp_diag_wm_struct_type* ds_3gpp_diag_hdlr_get_req_wm
(
  ds3gsubsmgr_subs_id_e_type              subs_id
);

#endif /*FEATURE_DATA_3GPP*/
#endif /*FEATURE_DATA_DIAG_SUPPORT*/

#endif /* DS_3GPP_DIAG_HDLR_H */
