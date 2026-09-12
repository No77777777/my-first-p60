/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                       M M G S D I  G S T K  H E A D E R S

 
GENERAL DESCRIPTION

  This file contains the functions required for interaction with GSTK.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c)2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_gstk.h#1 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/26/16   me     initial versoin

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "uim_variation.h"
#include "comdef.h"
#include "mmgsdilib_p.h"
#include "mmgsdi.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/


/*===========================================================================
FUNCTION MMGSDI_PROCESS_TP_DL_COMPLETE_RESPONSE

DESCRIPTION
  This function performs the actions required after TP download completes

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
 
SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_process_tp_dl_complete_response(
  const mmgsdi_uim_report_rsp_type  * rsp_ptr
);

/*===========================================================================
FUNCTION MMGSDI_PROCESS_TP_DOWNLOAD_COMPLETE

DESCRIPTION
  This function performs the actions required after TP download completes

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type
 
SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_process_tp_download_complete(
  mmgsdi_tp_dl_complete_req_type * req_ptr
);

/*===========================================================================
FUNCTION:      MMGSDI_CARD_INIT_GSTK_TERMINAL_PROFILE_DOWNLOAD

DESCRIPTION
  This function is used to notfiy GSTK to perform a Terminal Profile
  download

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  Triggers the GSTK task to send down a Terminal Profile Download
===========================================================================*/
void mmgsdi_card_init_gstk_terminal_profile_download (
  mmgsdi_session_id_type   session_id,
  mmgsdi_slot_id_enum_type slot,
  mmgsdi_app_enum_type     app_type
);

/*===========================================================================
FUNCTION:      MMGSDI_CARD_INIT_PERFORM_POST_PIN1_TP_IF_NEEDED

DESCRIPTION
  Send Terminal Profile Download if required as part of post-pin1 if required.

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_card_init_perform_post_pin1_tp_if_needed(
  mmgsdi_session_id_type     session_id,
  mmgsdi_slot_id_enum_type   slot_id,
  mmgsdi_app_enum_type       app_type
);

/*===========================================================================
FUNCTION:      MMGSDI_CARD_INIT_TP_STATE_INIT

DESCRIPTION
  Send Terminal Profile Download if required as part of post-pin1 if required.

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_card_init_tp_state_init(
  mmgsdi_slot_data_type   *slot_data_ptr 
);

/*===========================================================================
FUNCTION:      MMGSDI_CARD_INIT_APP_TP_STATE_INIT

DESCRIPTION
  This function set the app TP state 

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmgsdi_card_init_app_tp_state_init(
  mmgsdi_int_app_info_type   * app_info_ptr  
);

/*===========================================================================
FUNCTION:      MMGSDI_CARD_INIT_IS_TOOLKIT_ALLOWED

DESCRIPTION
  This function is used to determine whether or not toolkit is allowed.

DEPENDENCIES
  None

RETURNS
    TRUE     Toolkit is allowed
    FALSE    Toolkit is not allowed

SIDE EFFECTS
  None
===========================================================================*/
boolean mmgsdi_card_init_is_toolkit_allowed(
  mmgsdi_session_id_type   session_id,
  mmgsdi_slot_id_enum_type slot,
  mmgsdi_app_enum_type     app_type
);
