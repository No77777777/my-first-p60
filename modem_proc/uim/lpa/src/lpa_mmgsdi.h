#ifndef LPA_MMGSDI_H
#define LPA_MMGSDI_H
/*===========================================================================


            L P A   M M G S D I   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016, 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_mmgsdi.h#3 $ $DateTime: 2019/12/27 02:07:35 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/14/19   nc      Add EID to SimSlotStatus radio config HAL
12/07/16   av      LPA phase 2 initial changes  
08/03/16   av      Send STATUS to trigger REFRESH
04/11/16   av      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmgsdilib_common.h"

#include "lpa.h"
#include "lpalib.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

#define LPA_PHASE_2_STORE_DATA_P1               0x91
#define LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM   0x00

typedef enum {
  LPA_MMGSDI_APDU_NONE,
  LPA_MMGSDI_APDU_GET_CHALLENGE,
  LPA_MMGSDI_APDU_GET_STATUS,
  LPA_MMGSDI_APDU_STORE_DATA,
  LPA_MMGSDI_APDU_GET_DATA,
  LPA_MMGSDI_APDU_DELETE
} lpa_apdu_cmd_enum_type;

typedef struct {
  lpa_apdu_cmd_enum_type                cmd_type;
  uint32                                data_len;
  uint8                                *data_ptr;
  lpa_slot_id_enum_type                 slot_id;
} lpa_mmgsdi_resp_data_type;

/* ----------------------------------------------------------------------------
   DATA TYPE:      LPA_MMGSDI_CALLBACK_TYPE
-----------------------------------------------------------------------------*/
typedef void (*lpa_mmgsdi_callback_type) (lpa_result_enum_type              status,
                                          const lpa_mmgsdi_resp_data_type  *resp_ptr,
                                          const void                       *user_data_ptr);

typedef struct {
  mmgsdi_event_data_type mmgsdi_evt_data;
} lpa_mmgsdi_evt_data_type;

typedef struct {
  mmgsdi_return_enum_type status;
  mmgsdi_cnf_enum_type    cnf;
  mmgsdi_cnf_type         cnf_data;
} lpa_mmgsdi_cmd_rsp_data_type;

typedef enum {
  LPA_MMGSDI_CMD_RSP,
  LPA_MMGSDI_EVENT
} lpa_mmgsdi_cmd_enum_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_MMGSDI_CMD_TYPE

   DESCRIPTION:
     Contains information for handling MMGSDI event commands
-------------------------------------------------------------------------------*/
typedef struct {
  q_link_type                      link;
  lpa_mmgsdi_cmd_enum_type         cmd_type;
  union {
    lpa_mmgsdi_evt_data_type       event_data;
    lpa_mmgsdi_cmd_rsp_data_type   rsp_data;
  } data;
} lpa_mmgsdi_cmd_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_MMGSDI_SELECT_ISDR_RESP_TYPE

   DESCRIPTION:
     This structure is for select ISDR response parsing
-------------------------------------------------------------------------------*/
typedef struct {
  uint8                                   svn[LPA_SVN_LEN];
  boolean                                 lpa_e_using_cat;
  boolean                                 lpa_e_using_scws;
} lpa_mmgsdi_select_isdr_resp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_CACHE_EID_INFO_TYPE

   DESCRIPTION:
     This structure is for the EID of an eUICC.
-------------------------------------------------------------------------------*/
typedef struct {
  uint8 data_len;
  uint8 data[LPA_EID_LEN];
} lpa_cache_eid_info_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION LPA_MMGSDI_HANDLE_CMD_SIG

DESCRIPTION
  This function, called in lpa_main, is called to process an MMGSDI event

DEPENDENCIES
  LPA task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_mmgsdi_handle_cmd_sig (
  void
);

/*===========================================================================
   FUNCTION:      LPA_MMGSDI_INIT

   DESCRIPTION:
     LPA MMGSDI init function called during task init

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
===========================================================================*/
void lpa_mmgsdi_init (
  void
);

/*===========================================================================
   FUNCTION:      LPA_MMGSDI_RESET

   DESCRIPTION:
     LPA API to cleanup MMGSDI context

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_mmgsdi_reset(
  void
);

/*===========================================================================
   FUNCTION:      LPA_MMGSDI_COMMAND_CLEANUP

   DESCRIPTION:
     LPA API to cleanup current command related global

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_mmgsdi_command_cleanup(
  lpa_slot_id_enum_type                 lpa_slot
);

/*===========================================================================
   FUNCTION:      LPA_CONVERT_MMGSDI_STATUS_TO_LPA

   DESCRIPTION:
     LPA mmgsdi utility to convert MMGSDI Status to LPA status

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_convert_mmgsdi_status_to_lpa(
  mmgsdi_return_enum_type   mmgsdi_status
);

/*===========================================================================
   FUNCTION:      LPA_MMGSDI_GET_STATUS

   DESCRIPTION:
     API creates and sends a GET STATUS APDU to the card

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_mmgsdi_get_status (
  lpa_slot_id_enum_type         slot_id,
  lpa_mmgsdi_callback_type      callback,
  void                          *user_data_ptr
);

/*===========================================================================
   FUNCTION:      LPA_MMGSDI_GET_DATA

   DESCRIPTION:
     Utility called to send GET DATA command to the card

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_mmgsdi_get_data (
  lpa_slot_id_enum_type         slot_id,
  uint8                         p2,
  lpa_data_type                 data,
  lpa_mmgsdi_callback_type      callback,
  void                          *user_data_ptr
);

/*===========================================================================
   FUNCTION:      LPA_MMGSDI_STORE_DATA

   DESCRIPTION:
     Utility called to send STORE DATA command to the card

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_mmgsdi_store_data (
  lpa_slot_id_enum_type         slot_id,
  uint8                         p1,
  uint8                         p2,
  lpa_data_type                 data,
  boolean                       le_present,
  lpa_mmgsdi_callback_type      callback,
  void                          *user_data_ptr
);

/*===========================================================================
   FUNCTION:      LPA_MMGSDI_GET_CHALLENGE

   DESCRIPTION:
     Utility called to send GET CHALLENGE command to the card

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_mmgsdi_get_challenge (
  lpa_slot_id_enum_type         slot_id,
  lpa_mmgsdi_callback_type      callback,
  void                          *user_data_ptr
);

/*===========================================================================
   FUNCTION:      LPA_MMGSDI_DELETE

   DESCRIPTION:
     API used to send a DELETE APDU to the card

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_mmgsdi_delete (
  lpa_slot_id_enum_type         slot_id,
  lpa_iccid_type                iccid,
  lpa_mmgsdi_callback_type      callback,
  void*                         user_data_ptr
);

/*===========================================================================
   FUNCTION:      LPA_MMGSDI_STATUS

   DESCRIPTION:
     API creates and sends a STATUS APDU to the card on the default channel.
     It does not allow the caller to provide a callback; it just attempts
     sending the STATUS to the card and ignores the response.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_mmgsdi_status (
  lpa_slot_id_enum_type         slot_id
);

/*===========================================================================
   FUNCTION:      LPA_MMGSDI_SELECT_ISDR

   DESCRIPTION:
     This function calls the MMGSDI API for opening of the logical channel
     with the ISD-R and selecting the ISD-R.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_mmgsdi_select_isdr(
  lpa_slot_id_enum_type          slot_id,
  lpa_mmgsdi_callback_type       callback,
  void                          *user_data_ptr
);

/*=============================================================================
FUNCTION        LPA_GET_SLOT_ID_INSTANCE

DESCRIPTION
  This function returns the cached EID from the global ptr for a particular slot.

PARAMETERS
  None

RETURN VALUE
  lpa_cache_eid_info_type
    
===============================================================================*/
lpa_cache_eid_info_type* lpa_get_slot_eid_instance(
  lpa_slot_id_enum_type lpa_slot
);

/*=============================================================================
FUNCTION        LPA_FREE_CACHED_EID_INSTANCE

DESCRIPTION
  This function clears the cached EID of a particular slot.

PARAMETERS
  None

RETURN VALUE
  boolean
    
===============================================================================*/
boolean lpa_free_cached_eid_instance(
  lpa_slot_id_enum_type lpa_slot
);

#endif /* LPA_MMGSDI_H */

