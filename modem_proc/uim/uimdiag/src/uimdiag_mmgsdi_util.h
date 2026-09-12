#ifndef UIMDIAG_MMGSDI_UTIL_H
#define UIMDIAG_MMGSDI_UTIL_H

/*===========================================================================

===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2009 - 2017, 2023 by QUALCOMM Technologies, Inc (QTI).
All Rights Reserved. QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/uimdiag/src/uimdiag_mmgsdi_util.h#3 $$ $DateTime: 2023/03/05 23:08:44 $ $Author: pwbldsvc $

when       who   what, where, why
--------   ---   -----------------------------------------------------------
02/22/23   ghk   Added macro for max PLMN's in UIM diag cmd
01/11/17   sk    Moved uimdiag_file_to_mmgsdi_table
12/29/16   dt    Support for QXDM sim data collection
03/11/16   ar    Remove UIMDIAG code using Stubs
11/02/15   stv   Rel 12 refresh enforcement policy support
08/27/15   av    Retain legacy enum values of major existing MMGSDI events
05/15/15   yt    Support for mapping sessions to slots
04/25/15   av    Support for permanently disabling CSIM
02/25/15   yt    Reduction of UIMDIAG functionality
02/20/15   vv    Remove perso related code
01/26/15   vv    Add SPDI in uimdiag ef mapping table
09/14/14   kk    Remove unused uimdiag ef mapping entries
06/10/14   tl    Introduce EF 3GPDUPPExt
12/02/13   tkl   Support for SFI
06/27/13   spo   Added support for files under DF TELECOM
11/09/12   yt    Remove parsing of length field for PERSO_GET_DATA_CMD
09/25/12   tl    Adding nvruim_data_req_enum_type to sessionid_type
07/13/12   vv    Renamed CSGI to CSGL
12/23/11   bcho  uimdiag_file_to_mmgsdi_table modified
10/24/11   adp   Removing support for UIMDIAG_MMGSDI_SAP_PIN_VERIFY_CMD,
                 UIMDIAG_MMGSDI_SAP_INITIALIZE_CMD and
                 UIMDIAG_MMGSDI_SAP_DEREGISTER_CMD
10/10/11   adp   Adding uimdiag enum mapping
10/18/11   bcho  Adding support for mmgsdi_session_open_ext API
10/14/11   bcho  Added support for Get Operator Name API
10/10/11   nb    Fixed Incorrect featurization due to merge error
09/23/11   nb    Support for SAP CONNECT/SAP DISCONNECT
09/15/11   adp   Adding support to trigger refresh in uimdiag
09/15/11   nb    Support for new refresh stage
08/22/11   av    Added support for pdown,pup,activate onchip,send apdu commands
12/29/10   ms    Adding support for BT SAP APIs
12/03/10   adp   Adding support for a number of commands
09/07/10   adp   Adding Support for dck_num_retries,lock_down_enable_ltd_access,
                 seek and search commands
08/09/10   adp   Adding support for service enable, disable, get perso key,
                 is service available, perso unblock.
06/09/10   adp   Fixing Compiler Warnings
03/25/10   adp   Adding support for mmgsdi_session_deactivate_provisioning,
                 mmgsdi_session_get_app_capabilities,get_cphs_info
                 isim_authenticate, mmgsdi_send_card_status,
                 create_pkcs15_lookup_table
03/19/10   rk    Added support for JCDMA get info
02/22/10   adp   Adding support for Perso Commands
12/27/09   ap    Adding support for Refresh commands
12/03/09   rk    Adding support for get file attributes, read and write commands
09/02/09   adp   Initial Revision -Added initial mmgsdi session util functions


=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "mmgsdilib_common.h"
#include "diagpkt.h"
#include "uimdiag_util.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
/* Maximum number of plmn in uimdiag command */
#define UIMDIAG_MAX_PLMN_IN_LIST  256


/*=============================================================================

                       FUNCTIONS DECLARATIONS

=============================================================================*/

/*===========================================================================

FUNCTION UIMDIAG_UTIL_CONVERT_FROM_MMGSDI_ITEMS_ENUMCMD

DESCRIPTION
  This function converts mmgsdi enums to uimdiag enums

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_file_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
uimdiag_file_enum_type uimdiag_util_convert_from_mmgsdi_items_enum (
  mmgsdi_file_enum_type mmgsdi_file
);

/*===========================================================================

FUNCTION UIMDIAG_UTIL_CONVERT_TO_MMGSDI_ITEMS_ENUMCMD

DESCRIPTION
  This function converts uimdiag enums to mmgsdi enums

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_file_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
mmgsdi_file_enum_type uimdiag_util_convert_to_mmgsdi_items_enum (
  uimdiag_file_enum_type uimdiag_mmgsdi_file
);

/* ==========================================================================
   FUNCTION:      UIMDIAG_UTIL_CONVERT_FROM_MMGSDI_EVENTS_ENUM

   DESCRIPTION:
     This function returns uint8 equivalent UIMDIAG value (per ICD) of the
     MMGSDI event enum

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     uimdiag_return_enum_type

   SIDE EFFECTS:
     None
==========================================================================*/
uimdiag_return_enum_type uimdiag_util_convert_from_mmgsdi_events_enum (
  mmgsdi_events_enum_type  mmgsdi_evt,
  uint8                   *uimdiag_mmgsdi_evt_ptr
);

/*===========================================================================
FUNCTION: uimdiag_parse_cmd_data

DESCRIPTION:
  Populates the request structure with data from the Diag packet.

INPUTS:
  *req_ptr:      Generic_command struct request pointer
                 which will be populated with data from the diag_req_ptr
  *diag_req_ptr: pointer to raw Diag request packet
   diag_req_len: len of the diag request packet

DEPENDENCIES:
  None

RETURN VALUE:
   UIMDIAG_SUCCESS
   UIMDIAG_ERROR

OUTPUT:
  None
===========================================================================*/
uimdiag_return_enum_type uimdiag_parse_cmd_data (
  uimdiag_generic_req_type  *req_ptr,
  PACKED void               *diag_req_ptr,
  uint32                     diag_req_len
);

#endif /* UIMDIAG_MMGSDI_UTIL_H */
