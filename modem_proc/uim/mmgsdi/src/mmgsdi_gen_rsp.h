#ifndef MMGSDI_GEN_RSP_H
#define MMGSDI_GEN_RSP_H
/*===========================================================================


           M M G S D I   S A P   R S P   H E A D E R S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2005 - 2010, 2013 - 2014 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.  
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_gen_rsp.h#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
12/10/14   yt      Conversion of APDU streaming on channel 0 to MMGSDI requests
11/05/14   ar      Remove mmgsdi_get_card_reader_status () API
06/30/14   vv      Remove Perso code
10/16/13   df      Removing unused functions
03/02/10   kk      Fixed get all available apps command handling
02/01/10   jk      EPRL Support
02/24/09   nb      Perso Command support in MMGSDI
11/18/08   js      Updated featurization for Send APDU related code
11/15/08   js      Removed MFLO featurization for Send APDU related code
07/17/08   kk      Added support for PKCS 15
09/10/07   tml     Added MFLO Supports.
08/07/06   tml     Added ACL support
05/23/06   tml     BDN support
12/14/05   tml     MMGSDI Cleanup
11/03/05   tml     Fixed header
08/25/05   pv      Initial version

=============================================================================*/

#include "uim_variation.h"
#include "mmgsdi.h"
#include "mmgsdilib.h"

/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_GEN_RSP

   DESCRIPTION:
     This function will process the any generic response
     Note, for BDN and ACL, this function should only come called if it
     is a failed operation, if successful operation, should be handled by
     the get_file_attribute_response and write_response accordingly

   DEPENDENCIES:

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The funtion was successful in getting the file
                              attributes of a particular file.
     MMGSDI_ERROR:
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the function are not
                              within appropriate ranges.

   SIDE EFFECTS:
     None

-----------------------------------------------------------------------------*/
mmgsdi_return_enum_type mmgsdi_gen_rsp(
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr
);

/*=============================================================================
  FUNCTION: MMGSDI_GEN_RSP_GET_ATR

  DESCRIPTION:
    The response to the Get ATR command.  The ATR is internally stored in
    MMGSDI and the response is built internally in MMGSDI

  PARAMETERS:
    uim_rsp_ptr:    UIM response data.

  DEPENDENCIES
    None

  LIMITATIONS
    NONE

  RETURN VALUE
    mmgsdi_return_enum_type

  SIDE EFFECTS
    NONE
==============================================================================*/
mmgsdi_return_enum_type mmgsdi_gen_rsp_get_atr (
  const mmgsdi_uim_report_rsp_type*   uim_rsp_ptr
);

/*===========================================================================
FUNCTION MMGSDI_GEN_RSP_SEND_APDU_RSP

DESCRIPTION
  Function to handle the send APDU response received from UIM.

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None.
===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_gen_rsp_send_apdu_rsp(
  const mmgsdi_uim_report_rsp_type*     uim_rsp_ptr
);

/* ============================================================================
FUNCTION:      MMGSDI_GEN_RSP_SESSION_GET_INFO

DESCRIPTION:
  This function will build extract the session info and prepare the cnf

DEPENDENCIES:
  NONE

LIMITATIONS:
  NONE

RETURN VALUE:
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
SIDE EFFECTS:
  NONE
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_gen_rsp_session_get_info (
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr
);

/* ============================================================================
FUNCTION:      MMGSDI_GEN_RSP_SESSION_READ_PRL

DESCRIPTION:
  This function will build the read prl rsp and prepare the cnf

DEPENDENCIES:
  NONE

LIMITATIONS:
  NONE

RETURN VALUE:
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS:     The cnf was successfully queued to cnf Queue
    MMGSDI_ERROR:       Failed in queuing cnf to cnf Queue

SIDE EFFECTS:
  NONE
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_gen_rsp_session_read_prl (
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr
);

/* ============================================================================
FUNCTION:      MMGSDI_GEN_RSP_SESSION_PROV_APP_INIT_COMPLETE

DESCRIPTION:
  This function will build cnf for session prov app init complete command

DEPENDENCIES:
  NONE

LIMITATIONS:
  NONE

RETURN VALUE:
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
SIDE EFFECTS:
  NONE
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_gen_rsp_session_prov_app_init_complete (
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr
);

/* ============================================================================
FUNCTION:      MMGSDI_GEN_RSP_GET_CPHS_INFO

DESCRIPTION:
  This function will build cnf for get cphs info command

DEPENDENCIES:
  NONE

LIMITATIONS:
  NONE

RETURN VALUE:
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
SIDE EFFECTS:
  NONE
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_gen_rsp_get_cphs_info (
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr
);

/* ============================================================================
FUNCTION:      MMGSDI_GEN_RSP_AVAILABLE_APPS_REQ

DESCRIPTION:
  This function will build cnf for get available apps command

DEPENDENCIES:
  NONE

LIMITATIONS:
  NONE

RETURN VALUE:
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS:     The cnf was successfully queued to cnf Queue
    MMGSDI_ERROR:       Failed in queuing cnf to cnf Queue

SIDE EFFECTS:
  NONE
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_gen_rsp_available_apps_req (
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr
);

/* ============================================================================
FUNCTION:      MMGSDI_GEN_RSP_GET_APP_CAPABILITIES

DESCRIPTION:
  This function will build cnf for get app capabilities command

DEPENDENCIES:
  NONE

LIMITATIONS:
  NONE

RETURN VALUE:
  mmgsdi_return_enum_type
    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
SIDE EFFECTS:
  NONE
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_gen_rsp_get_app_capabilities (
  const mmgsdi_uim_report_rsp_type      * uim_rsp_ptr
);

/*===========================================================================
FUNCTION MMGSDI_GEN_GET_RSP_TABLE_ADD

DESCRIPTION
  Function will add the APDU Response Data cached from the currently
  applicable APDU Exchange with the Card inserted.

DEPENDENCIES
  The APDU_PTR provided must contain a valid pointer and all memory must
  be appropriately allocated to match the data maintained in this table.

RETURN VALUE
  mmgsdi_return_enum_type:
  MMGSDI_SUCCESS:           Get Response Data populated in the apdu_ptr
                            provided.
  MMGSDI_NOT_FOUND:         Get Response Data for the CLIENT ID, SLOT ID,
                            CHANNEL ID is no longer present
  MMGSDI_INCORRECT_PARAMS:  Parameter check failure. NULL DATA PTR provided
                            or Get Response Value exceeds the maximum.
  MMGSDI_ERROR:             Could not find an index location to save the
                            GET APDU Transaction Data.

SIDE EFFECTS
  A Successful add will allocate memory from the head.  This memory
  allocation is freed when the next applicable APDU Exchange occurs.

===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_gen_get_rsp_table_add(
  mmgsdi_client_id_type      client_id,
  mmgsdi_slot_id_enum_type   slot_id,
  uint8                      channel_id,
  const mmgsdi_data_type *   apdu_ptr
);

/*===========================================================================
FUNCTION MMGSDI_GEN_GET_RSP_TABLE_DELETE

DESCRIPTION
  Function shall delete the cached APDU Response Data corresponding to the
  requested Client ID, Slot ID, Channel ID.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type:
  MMGSDI_SUCCESS:           Get Response Data deleted from the cache
  MMGSDI_NOT_FOUND:         No Get Response Data cached for the
                            requested Client ID, Slot ID
  MMGSDI_INCORRECT_PARAMS:  Parameter check failure

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_gen_get_rsp_table_delete(
  mmgsdi_client_id_type    client_id,
  mmgsdi_slot_id_enum_type slot_id,
  uint8                    channel_id
);
#endif /* MMGSDI_GEN_RSP_H */
