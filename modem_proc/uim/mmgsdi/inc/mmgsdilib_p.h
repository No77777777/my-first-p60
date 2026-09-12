#ifndef MMGSDILIB_P_H
#define MMGSDILIB_P_H
/*===========================================================================


            M M G S D I   L I B R A R Y   D E F I N I T I O N S

                      A N D   F U N C T I O N S


  This file contains Library function that MMGSDI client within UIM can call

===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2011 - 2018, 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved. QUALCOMM Technologies Proprietary.
Export of this technology or software is
regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/inc/mmgsdilib_p.h#4 $ $DateTime: 2020/02/17 23:22:31 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
02/13/20   vgd     Get ATR, ICCID, EID from MMGSDI when card_info is populated
01/02/20   sch     Added support for log masking
08/17/18   rps     Add support for mini dump phase 2
05/23/17   nr      DTF handlers for session activation and de-activation
04/03/17   ks      UIM automatic slot mapping support
12/29/16   dt      Support for QXDM SIM data collection
07/21/16   bcho    UIM integration with QSH events
06/29/16   sp      Added stub functions for EFs under PKCS directory
04/13/16   ar      CDMA/HDR support on 2nd SUB
02/10/16   na      Optimize read record req by reading from cache directly
10/30/15   tkl     Return error when fail to post message to MMGSDI context
10/12/15   vdc     Added support for USAT application pairing
09/09/15   hh      Add support for potential silent file change notification
08/10/15   av      Introduce MMGSDI event for indicating recovery completion
04/08/15   at      Support for SIM detected usecase
08/14/14   tl      Add support for silent PIN verification
01/10/14   df      Remove unnceccary include
10/04/13   vv      Added NV support for ENS sub-features
09/23/13   df      Support for SIM busy
08/14/13   av      Optimize read transparent req by reading from cache directly
04/25/13   vv      Remove the legacy hotswap function
04/24/13   yt      Add NV_GPRS_ANITE_GCF_I to MMGSDI NV cache
04/17/13   tl      Replace Featurization with NV
02/20/13   at      Added NV_FTM_MODE_I while fetching the cached items
02/14/13   tl      Move QMI UIM to MMGSDI context from QMI modem context
01/03/12   abg     Added API to read nv item from mmgsdi cache
12/05/12   vv      Added support for parallel processing in MMGSDI
08/07/12   tl      Added prototype of mmgsdi_get_protocol
03/29/12   shr     Added support for non-prov. app selection using zero length
                   and partial AID, support for Send APDU extension API
03/01/12   bcho    Function prototype comment block updated
02/23/12   shr     Added support for Session Open with MF
12/23/11   shr     Removed featurization for ALWAYS ON features
12/21/11   bcho    Legacy GSDI removal updates
12/07/11   ssr     extern mmgsdi_init_rtre_config function
10/13/11   shr     Adding support for full recovery
09/23/11   nb      Async TP enhancement
08/01/11   vs      Support to open session with specified select rsp
05/11/11   nb      Added prototype for mmgsdi_tp_download_complete()
03/22/11   nb      Initial Revision

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "comdef.h"
#include "mmgsdilib_v.h"
#include "uim_p.h"
#include "qsh.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
/*----------------------------------------------------------------------------
STRUCTURE       MMGSDI_APP_SELECT_WITH_RSP_TYPE
-----------------------------------------------------------------------------*/
/** @brief Contains the information for an application, whether the channel
    on which the application is selected can be shared and the type of
    select response expected
*/
typedef struct {
  mmgsdi_data_type             app_id_data;       /**< Application identifier */
  boolean                      exclusive_channel; /**< Indicates whether other
                                                       session IDs can be
                                                       mapped to the same
                                                       channel */
  mmgsdi_select_rsp_enum_type  select_rsp_type;   /**< Type of select response */
} mmgsdi_app_select_with_rsp_type;

/*=============================================================================

                     FUNCTION DECLARATIONS

=============================================================================*/
/*=============================================================================
FUNCTION:       MMGSDI_SESSION_OPEN_WITH_SELECT_RSP
===============================================================================*/
/**
  Opens a non-provisioning session to an application on the card and returns
  the type of select response requested.

  Application selection using full length AID, partial AID and empty AID (i.e.
  AID length of zero) are supported.
  When partial/empty AID is provided, first/only occurence of the AID on the
  card is selected.

  @param[in] client_id        Client ID of the caller
  @param[in] slot             Slot in which card is inserted
  @param[in] app_info         Application ID, channel exclusivity and
                              select response type
  @param[in] evt_cb_ptr       Application event callback. A client must provide
                              an event callback if it is to receive
                              session-related event notifications. If the
                              pointer is NULL, the client receives no event
                              notifications.
  @param[in] response_cb_ptr  Pointer to the response callback.
  @param[in] client_ref       User data returned upon completion of this
                              command.

  @return
  MMGSDI_SUCCESS          -- Command structure was properly generated and sent
                             to the MMGSDI command queue.\n
  MMGSDI_INCORRECT_PARAMS -- Parameters are not within the appropriate ranges.\n
  MMGSDI_CMD_QUEUE_FULL   -- Command was not queued to the MMGSDI task because
                             the maximum number of commands are already queued.

  @dependencies
  The client must have a valid client ID.

  @limitations
  Function cannot be used with ICC cards.

  @sideeffects
  The event callback for the session of interest is called every time the MMGSDI
  notifies the client on this session about all session-related events.
*/
mmgsdi_return_enum_type  mmgsdi_session_open_with_select_rsp (
  mmgsdi_client_id_type              client_id,
  mmgsdi_slot_id_enum_type           slot,
  mmgsdi_app_select_with_rsp_type    app_info,
  mmgsdi_evt_callback_type           evt_cb_ptr,
  mmgsdi_callback_type               response_cb_ptr,
  mmgsdi_client_data_type            client_ref
);


/*===========================================================================
   FUNCTION:      MMGSDI_GET_RECOVERY_DATA_SYNC

   DESCRIPTION:
     Returns all information needed for recovering a slot which has been Reset.

     This function uses critical sections to protect accesses to
     MMGSDI global data. Hence, it is advisable to pet the dog
     before invoking the function.

   PARAMETERS:
   uim_recovery_data_type recovery_data_ptr:  Output.  Pointer to the MMGSDI
                                                       recovery data type.
                                                       The result is stored
                                                       in this pointer.

   uim_slot_type          slot:               Input.   Slot for which data
                                                       is needed.
   DEPENDENCIES:
     None

   LIMITATIONS:
     Only UIM task can invoke this API

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          Request was successfully handled.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.
     MMGSDI_ERROR:            Generic error.

   SIDE EFFECTS:
     None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_get_recovery_data_sync(
  uim_recovery_data_type *recovery_data_ptr,
  uim_slot_type           uim_slot_id
);


/*============================================================================
   FUNCTION:      MMGSDI_NOTIFY_LINK_EST

   DESCRIPTION:
     Notifies MMGSDI of card detection and provides all information about
     the Cards.
     The API shall be called by UIM after cards have been detected during
     inital power-up, due to a hotswap or as a result of a card RESET command.

   PARAMETERS:
   uim_link_established_data_type link_est_data:  Input.  Info on the detected
                                                          cards.

   DEPENDENCIES:
     None

   LIMITATIONS:
     Only UIM task can invoke this API.

   RETURN VALUE:
     mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
    MMGSDI_CMD_QUEUE_FULL:   The command was not Queued to the MMGSDI Task
                             because the max number of commands are already
                             queued.
    MMGSDI_ERROR:            Generic error.

   SIDE EFFECTS:
     None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_notify_link_est(
  uim_link_established_data_type link_est_data
);


/*============================================================================
   FUNCTION:      MMGSDI_NOTIFY_CARD_ERROR

   DESCRIPTION:
     Notifies MMGSDI of any Card Error and the cause.

   PARAMETERS:
   uim_status_type error_condition:  Input.  Card Error cause.

   uim_slot_type   slot:             Input.  Card slot on which error
                                             has occurred.

   boolean         temporary_error:  Input.  Temporary or permanent
                                             Card error

   DEPENDENCIES:
     None

   LIMITATIONS:
     Only UIM task can invoke this API.

   RETURN VALUE:
     mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
    MMGSDI_CMD_QUEUE_FULL:   The command was not Queued to the MMGSDI Task
                             because the max number of commands are already
                             queued.
    MMGSDI_ERROR:            Generic error.

   SIDE EFFECTS:
     None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_notify_card_error(
  uim_status_type error_condition,
  uim_slot_type   slot,
  boolean         temporary_error
);


/*===========================================================================
FUNCTION   mmgsdi_init_rtre_config_per_context

DESCRIPTION
  Extension function for mmgsdi_init_rtre_config that Initializes the
  RTRE config with CM

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  This function is passsing the RTRE Mode / RTRE Configuration value to the CM and
  later CM will notify this RTRE configuration value to its clients.
===========================================================================*/
void mmgsdi_init_rtre_config_per_context(
  mmgsdi_nv_context_type nv_item_context
);


/*================================================================
FUNCTION:      MMGSDI_SESSION_OPEN_WITH_MF

DESCRIPTION:
  Opens a non-provisioning session and a corresponding logical channel
  on the card.

PARAMETERS:
  client_id:        Client ID of the caller
  slot:             Slot in which card is inserted
  evt_cb_ptr:       Application event callback. A client must provide
                    an event callback if it is to receive
                    session-related event notifications. If the
                    pointer is NULL, the client receives no event
                    notifications.
  response_cb_ptr:  Pointer to the response callback.
  client_ref:       User data returned upon completion of this
                    command.

DEPENDENCIES:
  The client must have a valid client ID.

LIMITATIONS:
  Function cannot be used with ICC cards.
  PIN operations are not supported on the opened Session.

RETURN VALUE:
  mmgsdi_return_enum_type:
    MMGSDI_SUCCESS:          The command structure was properly
                             Generated and queued onto the MMGSDI
                             Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API
                             are not within appropriate ranges.
    MMGSDI_CMD_QUEUE_FULL:   The command was not Queued to the
                             MMGSDI Task because the max number of
                             commands are already queued.

SIDE EFFECTS:
  The event callback for the session of interest is called every time the MMGSDI
  notifies the client on this session about all session-related events.
================================================================*/
mmgsdi_return_enum_type  mmgsdi_session_open_with_mf (
  mmgsdi_client_id_type              client_id,
  mmgsdi_slot_id_enum_type           slot,
  mmgsdi_evt_callback_type           evt_cb_ptr,
  mmgsdi_callback_type               response_cb_ptr,
  mmgsdi_client_data_type            client_ref
);


/*===========================================================================
FUNCTION        MMGSDI_SESSION_READ_CACHE_EXT

DESCRIPTION
  Clients invoke this API to read data from a cached file.

  This API uses critical sections to protect accesses to
  MMGSDI global data. Hence, it is advisable to pet the dog
  before invoking the API.

PARAMETERS
  session_id:                  Session ID of the caller
  file_name:                   The enum used to access the SIM/USIM/RUIM/CSIM
                               Elementary Files.
  read_data_ptr:               Buffer length and pointer. Used to store the
                               data to be read. The memory should be allocated
                               and provided by the caller.
  offset:                      Offset to start the read from
  rec_num:                     Record number to be read from

DEPENDENCIES
  A valid Session ID is required, which can be achieved by calling
  mmgsdi_session_open().

LIMITATIONS
  Either a transparent file or 1 record from a linear fixed file can be cached.
  Always read the entire file/record.

RETURN VALUE
  mmgsdi_return_enum_type:

    MMGSDI_SUCCESS:            The command structure was properly generated
                               and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS:   The parameters supplied to the API are
                               not within appropriate ranges.
    MMGSDI_NOT_FOUND:          The requested file not found
    MMGSDI_NOT_INIT:           Cache not yet initialized

SIDE EFFECTS
  None.
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_session_read_cache_ext (
  mmgsdi_session_id_type       session_id,
  mmgsdi_file_enum_type        file_name,
  mmgsdi_data_type           * read_data_ptr,
  mmgsdi_offset_type           offset,
  mmgsdi_rec_num_type          rec_num
);


/*===========================================================================
FUNCTION        MMGSDI_SESSION_READ_CACHE_BY_SESSION_TYPE

DESCRIPTION
  Clients invoke this API to read data from a cached file by taking 
  session_type.

  This API uses critical sections to protect accesses to
  MMGSDI global data. Hence, it is advisable to pet the dog
  before invoking the API.

PARAMETERS
  session_type:                Session type of the caller
  file_name:                   The enum used to access the SIM/USIM/RUIM/CSIM
                               Elementary Files.
  read_data_ptr:               Buffer length and pointer. Used to store the
                               data to be read. The memory should be allocated
                               and provided by the caller.
  offset:                      Offset to start the read from.
  rec_num:                     Record number to be retrieved from cache

DEPENDENCIES
  A valid Session type is required.
  
LIMITATIONS
  None.

RETURN VALUE
  mmgsdi_return_enum_type:

    MMGSDI_SUCCESS:            The command structure was properly generated
                               and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS:   The parameters supplied to the API are
                               not within appropriate ranges.
    MMGSDI_NOT_FOUND:          The requested file not found
    MMGSDI_NOT_INIT:           Cache not yet initialized

SIDE EFFECTS
  None.
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_session_read_cache_by_session_type (
  mmgsdi_session_type_enum_type  session_type,
  mmgsdi_file_enum_type          file_name,
  mmgsdi_data_type             * read_data_ptr,
  mmgsdi_offset_type             offset,
  mmgsdi_rec_num_type            rec_num
);

/*=============================================================================
FUNCTION        MMGSDI_SESSION_READ_CACHE_FILE_SIZE_BY_SESSION_TYPE

DESCRIPTION
  Clients invoke this API to get the size of the file to be cached for read.

PARAMETERS
  session_id:                  Session type of the caller
  file_name:                   The enum used to access the SIM/USIM/RUIM/CSIM
                               Elementary Files.
  file_size_ptr:               The pointer to the buffer to store the file
                               size. The memory should be allocated and
                               provided by the caller.

DEPENDENCIES
  A valid Session type is required.

LIMITATIONS
  None.

RETURN VALUE
  mmgsdi_return_enum_type:

    MMGSDI_SUCCESS:            The command structure was properly generated
                               and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS:   The parameters supplied to the API are
                               not within appropriate ranges.
    MMGSDI_CMD_QUEUE_FULL:     The command was not queued to the MMGSDI Task
                               because the max numbers of commands are already
                               queued.

SIDE EFFECTS
  None.
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_session_read_cache_file_size_by_session_type (
  mmgsdi_session_id_type       session_type,
  mmgsdi_file_enum_type        file_name,
  mmgsdi_len_type            * file_size_ptr
);

/* ============================================================================
   FUNCTION       MMGSDI_GET_PROTOCOL

   DESCRIPTION
     This function will request protocol for the slot.
     The client calling this function expects an instant response.

   PARAMETERS:
     client_id:         Client ID of the requesting client.
     protocol_ptr:      Buffer to store the protocol information.

   DEPENDENCIES:
     A valid Client ID is required, which can be achieved by calling
     mmgsdi_client_id_and_evt_reg().

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

     MMGSDI_SUCCESS:          The command structure was properly generated
                              and queued onto the MMGSDI Command Queue.
     MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                              within appropriate ranges.
     MMGSDI_NOT_INIT:         Called before CARD_INSERTED on either slot

   SIDE EFFECTS:
     None
===============================================================================*/
mmgsdi_return_enum_type mmgsdi_get_protocol (
  mmgsdi_client_id_type              client_id,
  mmgsdi_slot_id_enum_type           slot,
  mmgsdi_protocol_enum_type        * protocol_ptr
);

/*============================================================================
   FUNCTION:      MMGSDI_NOTIFY_SIM_BUSY

   DESCRIPTION:
     Called by UIM Drivers to inform MMGSDI that the SIM card has been busy
     for a long period while processing a command.  UIM drivers is currently
     unable to send/receive APDUs but expects normal operation to resume.
     MMGSDI must inform its clients so they can take appropiate action to
     cope until normal sim card operations resume.

   PARAMETERS:
     bool          sim_busy :  True  => sim is currently busy
                               False => normal operations resumed
     uim_slot_type slot     :  Affected slot
   DEPENDENCIES:
     None

   LIMITATIONS:
     Only UIM task should invoke this API.

   RETURN VALUE:
     mmgsdi_return_enum_type

       MMGSDI_SUCCESS:          The command structure was properly generated
                                and queued onto the MMGSDI Command Queue.
       MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                                within appropriate ranges.
       MMGSDI_CMD_QUEUE_FULL:   The command was not Queued to the MMGSDI Task
                                because the max number of commands are already
                                queued.
       MMGSDI_ERROR:            Generic error.

   SIDE EFFECTS:
     None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_notify_sim_busy(
  boolean        sim_busy,
  uim_slot_type  slot_id
);

/*============================================================================
   FUNCTION:      MMGSDI_NOTIFY_LINK_EST_SLOT

   DESCRIPTION:
     Notifies MMGSDI of card detection and provides all information about
     the card on a slot.
     The API shall be called by UIM after a card has been detected during
     initial power-up or due to a hotswap or as a result of a card RESET
     command.

   PARAMETERS:
   link_est_data:  Input info of the detected card on a slot
   uim_slot_type:  slot_id

   DEPENDENCIES:
     None

   LIMITATIONS:
     Only UIM task can invoke this API.

   RETURN VALUE:
     mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
    MMGSDI_CMD_QUEUE_FULL:   The command was not Queued to the MMGSDI Task
                             because the max number of commands are already
                             queued.
   MMGSDI_ERROR:             Generic error.


   SIDE EFFECTS:
     None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_notify_link_est_slot(
  uim_slot_link_established_data_type link_est_data,
  uim_slot_type                       slot_id
);

/*============================================================================
   FUNCTION:      MMGSDI_START_POTENTIAL_SILENT_FILE_CHANGE

   DESCRIPTION:
     In some specific cards available in the field when BIP session or SMS PP
     download is triggered, there is a potential for certain files to be update
     without any indication to MMGSDI, so this function will notify MMGSDI when
     any BIP session or SMS PP download starts, so that MMGSDI can perform the
     appropriate action. This will potentially save on redundant SELECTs when
     accessing the EFs.
     This is only a wrapper function for the given functionality.

   PARAMETERS:
     client_id:         Client ID of the requesting client.
     slot:              Slot on which potential file change occurs. 

   DEPENDENCIES:
     None

   LIMITATIONS:
     Only GSTK task should invoke this API.

   RETURN VALUE:
     mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
    MMGSDI_CMD_QUEUE_FULL:   The command was not Queued to the MMGSDI Task
                             because the max number of commands are already
                             queued.
   MMGSDI_ERROR:             Generic error.

   SIDE EFFECTS:
     None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_start_potential_silent_file_change(
  mmgsdi_client_id_type              client_id,
  mmgsdi_slot_id_enum_type           slot
);

/*============================================================================
   FUNCTION:      MMGSDI_NOTIFY_SIM_PRE_INIT

   DESCRIPTION:
     Notifies MMGSDI of SIM card being pre-initialized on the respective slot.
     The API shall be called by UIM after a card has been detected by the GPIOs.
     The intention of this API is to notify MMGSDI that UIMDRV is going to try
     the normal power-up sequence of the card & MMGSDI shall expect either a
     link est API for successful initialization or a card error API if not.

   PARAMETERS:
   uim_slot_type:  slot_id

   DEPENDENCIES:
     None

   LIMITATIONS:
     Only UIM task can invoke this API.

   RETURN VALUE:
     mmgsdi_return_enum_type

    MMGSDI_SUCCESS:          The command structure was properly generated
                             and queued onto the MMGSDI Command Queue.
    MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                             within appropriate ranges.
    MMGSDI_CMD_QUEUE_FULL:   The command was not Queued to the MMGSDI Task
                             because the max number of commands are already
                             queued.
   MMGSDI_ERROR:             Generic error.


   SIDE EFFECTS:
     None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_notify_sim_pre_init(
  uim_slot_type                       slot_id
);

/*============================================================================
FUNCTION:      MMGSDI_NOTIFY_IMEI_SENT_TO_CARD

DESCRIPTION:
  Called by GSTK to notify MMGSDI that the IMEI(SV) has been sent to the
  card in the terminal response of PLI proactive command.

PARAMETERS:
  slot_id: slot information

DEPENDENCIES:
  None

LIMITATIONS:
  Only GSTK task should invoke this API.

RETURN VALUE:
  None

SIDE EFFECTS:
  None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_notify_imei_sent_to_card(
  mmgsdi_slot_id_enum_type      slot_id
);

/*============================================================================
FUNCTION:      MMGSDI_NOTIFY_RECOVERY_COMPLETE

DESCRIPTION:
  Notifies MMGSDI of recovery completion on a card in a slot.
  The API shall be called by UIM drivers after it has successfully finished
  the recovery procedure on a card in a slot.

PARAMETERS:
  uim_slot_type:           uim_slot_id

DEPENDENCIES:
  None

LIMITATIONS:
  Only UIM task can invoke this API.

RETURN VALUE:
  mmgsdi_return_enum_type

  MMGSDI_SUCCESS:          The command structure was properly generated
                           and queued onto the MMGSDI Command Queue.
  MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                           within appropriate ranges.
  MMGSDI_CMD_QUEUE_FULL:   The command was not Queued to the MMGSDI Task
                           because the max number of commands are already
                           queued.
  MMGSDI_ERROR:            Generic error.

SIDE EFFECTS:
  None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_notify_recovery_complete(
  uim_slot_type                       uim_slot_id
);

/*================================================================
FUNCTION  MMGSDI_SET_QMI_UIM_SIG

DESCRIPTION:
  Sets the MMGSDI_QMI_CAT_SIG to begin processing of QMI UIM.

INPUT PARAMETERS:
  None

DEPENDENCIES:
  mmgsdi_task must be finished initialization.

LIMITATIONS:
  None

RETURN VALUE:
  boolean

SIDE EFFECTS:
  None
===============================================================*/
boolean mmgsdi_set_qmi_uim_sig
(
  void
);

/*================================================================
FUNCTION  MMGSDI_SET_SLOT_MAPPING_SIG

DESCRIPTION:
  Sets the MMGSDI_SLOT_MAPPING_SIG to begin processing of slot mapping.

INPUT PARAMETERS:
  None

DEPENDENCIES:
  mmgsdi_task must be finished initialization.

LIMITATIONS:
  None

RETURN VALUE:
  boolean

SIDE EFFECTS:
  None
===============================================================*/
boolean mmgsdi_set_slot_mapping_sig
(
  void
);


/*============================================================================
   FUNCTION:      MMGSDI_QSH_ACTION_EVENT_CONFIG

   DESCRIPTION:
     This function will queue QSH client event config request for async action mode

   PARAMETERS:
     cb_params_ptr              QSH callback params 

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type

       MMGSDI_SUCCESS:          The command structure was properly generated
                                and queued onto the MMGSDI Command Queue.
       MMGSDI_INCORRECT_PARAMS: The parameters supplied to the API are not
                                within appropriate ranges.
       MMGSDI_CMD_QUEUE_FULL:   The command was not Queued to the MMGSDI Task
                                because the max number of commands are already
                                queued.
       MMGSDI_ERROR:            Generic error.

   SIDE EFFECTS:
     None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_qsh_action_event_config
(
  const qsh_client_cb_params_s  * qsh_cb_params_ptr
);

/* ============================================================================
FUNCTION MMGSDI_GET_GLOBAL_CLIENT_ID

DESCRIPTION
  This function returns the global MMGSDI client id.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  mmgsdi_client_id_type

SIDE EFFECTS
  None
============================================================================*/
mmgsdi_client_id_type mmgsdi_get_global_client_id(
  void
);

/* ===========================================================================
FUNCTION MMGSDI_IS_PRIVATE_LOGGING_ENABLED_FOR_SLOT

DESCRIPTION
  This function is used to get whether private logging is enabled. If
  enabled, logging of private data is allowed in the caller. If disabled, otherwise.
  If a test card MCC/MNC of the GW IMSI is present for the slot, it will always
  return ok. If it is a commercial card, it will take into account the value in
  the protected NV item for private logging.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  boolean
    TRUE  : Private logging allowed. It is ok to log privacy sensitive data
    FALSE : Otherwise

SIDE EFFECTS
  None
=========================================================================== */
boolean mmgsdi_is_private_logging_enabled_for_slot(
  mmgsdi_slot_id_enum_type slot_id
);

#ifdef FEATURE_QSH_MDUMP
/*==========================================================================
FUNCTION UIM_QSH_MDUMP_MMGSDI

DESCRIPTION
  This function is called to dump mmgsdi globals

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
QSH_MDUMP_FN_ATTR void uim_qsh_mdump_mmgsdi(void);
#endif /* FEATURE_QSH_MDUMP */

/* ================================================================
   FUNCTION  MMGSDI_GET_ATR_SYNC

   DESCRIPTION:
     This API is called to get the ATR in sync fashion, this API fills
     the data and length of ATR in "atr_data_ptr".

   INPUT PARAMETERS:
     client_id:           Client ID of the caller.
     card_slot:           Specific slot
     atr_data_ptr:        Structure containing length and pointer to
                          ATR data to be filled.
     req_data_len:        Number of characters to be read
   DEPENDENCIES:
     None

   LIMITATIONS:
     API is exposed to only internal UIM clients .

   RETURN VALUE:
     mmgsdi_return_enum_type:

   MMGSDI_SUCCESS          -- ATR data and length is populated succefully in 
                             the passed structure pointer
   MMGSDI_ERROR            -- ATR data is not filled succesfully.
   MMGSDI_INCORRECT_PARAMS -- If the parameters passed are not valid

   SIDE EFFECTS:
     None
===============================================================*/
mmgsdi_return_enum_type mmgsdi_get_atr_sync
(
  mmgsdi_client_id_type    client_id,
  mmgsdi_slot_id_enum_type card_slot,
  mmgsdi_data_type        *atr_data_ptr,
  mmgsdi_len_type          req_data_len
);

/* ================================================================
   FUNCTION  MMGSDI_GET_EID_SYNC

   DESCRIPTION:
     This API is called to get the EID in sync fashion, this API fills
     the data and length of EID in "eid_data_ptr".

   INPUT PARAMETERS:
     client_id:           Client ID of the caller.
     card_slot:           Specific slot
     eid_data_ptr:        Structure containing length and pointer to
                          EID data to be filled.
     req_data_len:        Number of characters to be read
   DEPENDENCIES:
     None

   LIMITATIONS:
     API is exposed to only internal UIM clients .

   RETURN VALUE:
     mmgsdi_return_enum_type:

   MMGSDI_SUCCESS          -- EID data and length is populated succefully in 
                              the passed structure pointer
   MMGSDI_ERROR            -- EID data is not filled succesfully.
   MMGSDI_INCORRECT_PARAMS -- If the parameters passed are not valid

   SIDE EFFECTS:
     None
===============================================================*/

mmgsdi_return_enum_type mmgsdi_get_eid_sync
(
  mmgsdi_client_id_type    client_id,
  mmgsdi_slot_id_enum_type card_slot,
  mmgsdi_data_type         *eid_data_ptr,
  mmgsdi_len_type          req_data_len
);
#endif /* MMGSDILIB_P_H */

