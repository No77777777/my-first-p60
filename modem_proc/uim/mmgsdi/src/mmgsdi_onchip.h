#ifndef MMGSDI_ONCHIP_H
#define MMGSDI_ONCHIP_H
/*===========================================================================


           M M G S D I   O N C H I P   H E A D E R S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016, 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.  
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_onchip.h#3 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
02/25/20   vgd     Build onchip auth response data is 3GPP spec defined format
08/30/16   bcho    Optimization of MMGSDI attribute cache access functions
06/22/16   tkl     Stub out additional APIs
06/10/16   sp      Move onchip entry function to onchip file
05/10/16   vdc     Guard with FEATURE_MMGSDI_3GPP2 for 3GPP2 related functions
04/05/16   sp      Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/

#include "mmgsdi.h"

/*===========================================================================
FUNCTION MMGSDI_ONCHIP_INIT_AUTO_ACTIVATION_DATA

DESCRIPTION
  Reads the EFS item to figure out if the OnChip should be enabled on boot up
  or not. If it dictates that OnChip be auto activated on boot up, the EFS
  item contents are validated and are read into the MMGSDI global. This is done
  for all the instances of this EFS item (assuming the corresponding slot index
  is enabled via NV 70210.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE:
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_onchip_init_auto_activation_data (
  uint8 index
);

/* ============================================================================

   FUNCTION:      MMGSDI_ONCHIP_GET_FILE_ATTR

   DESCRIPTION:
     This function returns file attribute for onchip files
     Currently support ICCID only

   DEPENDENCIES:
     None

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

=============================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_get_file_attr(
  mmgsdi_get_file_attr_req_type * req_ptr,
  mmgsdi_protocol_enum_type       protocol
);


/* ============================================================================
  FUNCTION:      MMGSDI_ONCHIP_ACTIVATE

  DESCRIPTION:
    This function will be used to activate the ONCHIP USIM or SIM.  It will
    configure the USIM/SIM Cache.

  DEPENDENCIES:


  LIMITATIONS:
    USIM Authentication in USIM mode is not supported.

  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
    MMGSDI_NOT_SUPPORTED:    When the file has an entry in the Service Table
                             but the service table indicates the card does not
                             have the support

  SIDE EFFECTS:
    None.

  CRITICAL SECTIONS:
    The function uses mmgsdi_client_app_data_crit_sect_ptr to protect accesses
    to slot data ptr, channel info ptr, pin info ptr and app info ptr.
============================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_activate(
  mmgsdi_activate_onchip_sim_req_type * onchip_sim_config_ptr,
  mmgsdi_onchip_mode_enum_type          onchip_mode
);


/*============================================================================
  FUNCTION:      MMGSDI_ONCHIP_ACTIVATE_SESSION

  DESCRIPTION:
    This function activates session for onchip and brings subscription to ready

  DEPENDENCIES:

  LIMITATIONS:
    
  RETURN VALUE:
    mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.

  SIDE EFFECTS:
    None.

  CRITICAL SECTIONS:
============================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_activate_session(
  mmgsdi_session_act_or_switch_prov_req_type *msg_ptr
);


/* ============================================================================
  FUNCTION:      MMGSDI_ONCHIP_DEACTIVATE_SESSION

   DESCRIPTION:
     This function will only update the channel info table info in regard to the 
     session that has been deactivated in the case of onchip.
     This function does not delete app related cache.

   DEPENDENCIES:

   LIMITATIONS:

   RETURN VALUE:
    mmgsdi_return_enum_type
 
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.

   SIDE EFFECTS:
     None.

   CRITICAL SECTIONS:
     The function uses mmgsdi_client_app_data_crit_sect_ptr to protect accesses
     to session info ptr and channel info ptr.
============================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_deactivate_session(
  mmgsdi_slot_id_enum_type             slot_id,
  mmgsdi_int_app_info_type            *app_info_ptr
);


/*===========================================================================
FUNCTION:      MMGSDI_ONCHIP_CREATE_ONCHIP_REQUEST

DESCRIPTION
  This function will build a MMGSDI_ACTIVATE_ONCHIP_SIM_REQ and queue it to
  the MMGSDI Task for further processing.

PARAMS:
  client_id:         Client Id of the caller
  card_slot:         Physical slot for multiple slot targets. Only slot-1
                     is supported
  onchip_sim_config: TLV Data Len and TLV Data.
  onchip_mode:       ICC/UICC mode to be used for onchip behavior
  response_cb_ptr:   Response callback.
  client_ref:        User Data returned upon completion of this cmd.

DEPENDENCIES:
  The client must have a valid client ID.  If the client ID is not part
  of the client_id_table, and failure will be returned in the response.
  Achived by calling mmgsdi_client_id_reg().

  Note: If client fails to provide a client id, meaning the client_id
        passed is 0, then an internal client id will be used.

LIMITATIONS:
  None.

RETURN VALUE:
  mmgsdi_return_enum_type:

    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
    MMGSDI_CMD_QUEUE_FULL:   The command was not Queued to the MMGSDI Task
                             because the max number of commands are already
                             queued.

SIDE EFFECTS:
  Access to a real card will not be possible without a power cycle
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_create_onchip_request (
  mmgsdi_client_id_type              client_id,
  mmgsdi_slot_id_enum_type           card_slot,
  mmgsdi_onchip_sim_config_data_type onchip_sim_config,
  mmgsdi_onchip_mode_enum_type       onchip_mode,
  mmgsdi_callback_type               response_cb_ptr,
  mmgsdi_client_data_type            client_ref
);

/* =============================================================================
   FUNCTION:      MMGSDI_CNF_BUILD_ONCHIP_SIM_DATA

   DESCRIPTION:
     This function builds cnf for the ONCHIP SIM ACTIVATION REQUEST

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command processing was successful.
     MMGSDI_ERROR:            The command processing was not successful.

   SIDE EFFECTS:
     None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_cnf_build_onchip_sim_data(
  mmgsdi_act_onchip_sim_cnf_type         * act_onchip_sim_cnf,
  mmgsdi_slot_id_enum_type                 slot_id);
  
/*===========================================================================
  FUNCTION:      MMGSDI_ONCHIP_UICC_AUTH

  DESCRIPTION:
    This function executes the onchip usim authentication

  DEPENDENCIES:
    None

  LIMITATIONS:
    This function is limited to the use of UICC ie. (technologies of type UICC only).

  RETURN VALUE:
    MMGSDI_SUCCESS:          The command processing was successful.
    MMGSDI_ERROR:            The command processing was not successful.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.

  SIDE EFFECTS:
     None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_onchip_uicc_auth (
  const mmgsdi_auth_req_type   *req_ptr);

#endif /* MMGSDI_ONCHIP_H */
