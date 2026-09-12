/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            L P A   C O M M A N D   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the LPA functions which handles LPA profile
  management requests coming from the user.

                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_cmd.c#3 $ $DateTime: 2019/12/27 02:07:35 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------
10/23/19   vgd     Set le_present to TRUE when P1 is 91 for phase 2 cards
01/14/19   nc      Add EID to SimSlotStatus radio config HAL
03/29/18   bcho    LPA phase2.1 changes
09/15/17   me      Fixing compiler errors
04/12/17   av      Add support to configure/retrieve default server address
04/11/17   av      Fixes for RAT decoding etc
01/12/17   av      Bug fixes for phase2 PRRID related logic
12/07/16   av      LPA phase 2 initial changes
10/06/16   av      Introduce DS http post retry timer logic
08/03/16   av      Send STATUS to trigger REFRESH
04/28/16   av      Move LPA task init to same group as qmiuim
04/27/16   av      Fixing some compiler warnings and adding more checks
04/11/16   av      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "queue.h"

#include "lpa.h"
#include "lpa_platform.h"
#include "lpa_cmd.h"
#include "lpalib.h"
#include "lpa_state.h"
#include "lpa_mmgsdi.h"
#include "lpa_util.h"
#include "lpa_lpd.h"
#include "lpa_asn1_encoder.h"
#include "lpa_asn1_decoder.h"
#include "lpa_timer.h"
#include <stringl/stringl.h>
#include "lpa_efs.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

#define LPA_CMD_QUEUE_MAX_SIZE                  10
#define LPA_ENABLE_PROFILE_APDU_P1            0x88
#define LPA_ENABLE_PROFILE_APDU_DGI_0         0x3A
#define LPA_ENABLE_PROFILE_APDU_DGI_1         0x03
#define LPA_DISABLE_PROFILE_APDU_P1           0x88
#define LPA_DISABLE_PROFILE_APDU_DGI_0        0x3A
#define LPA_DISABLE_PROFILE_APDU_DGI_1        0x04
#define LPA_ADD_OR_UPDATE_NICKNAME_APDU_P1    0x90
#define LPA_CARD_MEMORY_RESET_APDU_P1         0x88
#define LPA_CARD_MEMORY_RESET_APDU_DGI_0      0x3A
#define LPA_CARD_MEMORY_RESET_APDU_DGI_1      0x09
#define LPA_CARD_MEMORY_RESET_APDU_DGI_LEN    0x03
#define LPA_CARD_MEMORY_RESET_APDU_TAG        0x82
#define LPA_CARD_MEMORY_RESET_APDU_LEN        0x01
#define LPA_CARD_MEMORY_RESET_APDU_VAL        0x01
#define LPA_ICCID_STORE_DATA_TAG              0x5A
#define LPA_DISABLE_PROFILE_APDU_DGI_LEN      0x0C
#define LPA_ENABLE_PROFILE_APDU_DGI_LEN       0x0C
#define LPA_GET_EID_APDU_DATA_P2              0x30
#define LPA_GET_EID_APDU_DATA_BYTE_1          0x5C
#define LPA_GET_EID_APDU_DATA_BYTE_2          0x01
#define LPA_GET_EID_APDU_DATA_BYTE_3          0x5A

#define LPA_DELETE_PROFILE_APDU_RESP_LEN      0x01
#define LPA_DELETE_PROFILE_APDU_RESP          0x00
#define LPA_GET_EID_APDU_RESP_LEN             0x15
#define LPA_GET_EID_APDU_RESP_DGI_0           0xBF
#define LPA_GET_EID_APDU_RESP_DGI_1           0x30
#define LPA_GET_EID_APDU_RESP_DGI_LEN         0x12
#define LPA_EID_APDU_TAG                      0x5A

/* Defines for operator ID comparison */
#define LPA_CMD_MNC_LEN_3_BYTES               0x03
#define LPA_CMD_GID_WILDCHAR_LEN              0x00
#define LPA_CMD_WILDCHAR_MCC_DIGIT            0x0E
#define LPA_CMD_WILDCHAR_MNC_DIGIT            0x0E

static q_type                                 lpa_cmd_q[LPA_NUM_SLOTS];
static lpa_slot_id_enum_type                  lpa_last_cmd_q_processed = LPA_SLOT_2;
static lpa_get_rat_resp_type                  *rat_global_ptr[LPA_NUM_SLOTS] = {NULL};


/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

static lpa_result_enum_type lpa_cmd_internal_get_profiles_info_req (
  lpa_slot_id_enum_type         slot_id
);


/*=============================================================================

                       FUNCTIONS

=============================================================================*/

/*===========================================================================
   FUNCTION:      LPA_UTIL_FREE_TASK_CMD_PTR

   DESCRIPTION:
     Utility to free LPA task cmd pointer

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
===========================================================================*/
static void lpa_cmd_free_task_cmd_ptr(
  lpa_cmd_type                *lpa_cmd_ptr
)
{
  if(lpa_cmd_ptr == NULL)
  {
    return;
  }

  switch(lpa_cmd_ptr->cmd.cmd_enum)
  {
    case LPA_ENABLE_PROFILE_MSG:
    case LPA_DISABLE_PROFILE_MSG:
    case LPA_DELETE_PROFILE_MSG:
    case LPA_CARD_MEMORY_RESET_MSG:
    case LPA_GET_PROFILES_INFO_MSG:
    case LPA_GET_EID_MSG:
    case LPA_ADD_OR_UPDATE_PROFILE_NICKNAME_MSG:
      /* Nothing to free */
      break;
    case LPA_ADD_PROFILE_MSG:
      LPA_FREE(lpa_cmd_ptr->cmd.cmd.add_profile_req.activation_code.data_ptr);
      LPA_FREE(lpa_cmd_ptr->cmd.cmd.add_profile_req.confirmation_code.data_ptr);
      break;
    case LPA_SET_DEFAULT_SERVER_ADDRESS_MSG:
      LPA_FREE(lpa_cmd_ptr->cmd.cmd.set_default_server_address_req.default_smdp_address.data_ptr);
      break;
    default:
      break;
  }
} /* lpa_cmd_free_task_cmd_ptr */


/*===========================================================================
FUNCTION LPA_CMD_INIT

DESCRIPTION
  This function, called in lpa_main, is called to initialize the command
  module.

DEPENDENCIES
  LPA task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_cmd_init (
  void
)
{
  (void) q_init(&lpa_cmd_q[LPA_SLOT_1]);
  (void) q_init(&lpa_cmd_q[LPA_SLOT_2]);
} /* lpa_cmd_init */


/*===========================================================================
   FUNCTION:      LPA_CMD_MMGSDI_CB_PARSE_LEGACY_RESP

   DESCRIPTION:
     This is a function that parses the response received from LPA_MMGSDI
     module for the LPA UI commands.
     This function parses the response as per phase1/phase.1.1 of SGP22.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_mmgsdi_cb_parse_legacy_resp (
  const lpa_mmgsdi_resp_data_type                 *mmgsdi_resp_ptr,
  lpa_response_data_type                          *resp_ptr
)
{
  uint32                                         offset     = 0;
  lpa_result_enum_type                           status     = LPA_SUCCESS;

  if(mmgsdi_resp_ptr == NULL || resp_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  switch(resp_ptr->msg_type)
  {
    case LPA_ENABLE_PROFILE_MSG:
    case LPA_DISABLE_PROFILE_MSG:
    case LPA_CARD_MEMORY_RESET_MSG:
    case LPA_ADD_OR_UPDATE_PROFILE_NICKNAME_MSG:
      /* STORE DATA is sent for multiple operations EnableProfile,
         DisableProfile, AddOrUpdateProfileNickname... nothing
         expected in the response.. so, simply call the client
         callback */
      break;
    case LPA_DELETE_PROFILE_MSG:
      /* For DELETE PROFILE, we need to do some validation of the
         response received from the card - we must receive only one
         byte in the data, and it shoould be 0x00 */
      if(mmgsdi_resp_ptr->data_ptr == NULL ||
         mmgsdi_resp_ptr->data_len != LPA_DELETE_PROFILE_APDU_RESP_LEN ||
         mmgsdi_resp_ptr->data_ptr[0] != LPA_DELETE_PROFILE_APDU_RESP)
      {
        /* According to SGP22, for DELETE APDU, the card returns one byte as
           0x00 to indicate no more data is present. If we didn't receive
           0x00 data from the card, we treat the command as failure */
        status = LPA_GENERIC_ERROR;
      }
      break;
    case LPA_GET_PROFILES_INFO_MSG:
      /* This response is for the GET_STATUS APDU sent for GetProfilesInfo.
         Lets decode the response, this is in ASN1 format. The following
         decoder could malloc Icon (which is the only pointer inside get
         profiles info rsp) */
     (void) lpa_efs_write(LPA_FILE_GET_PROFILES_INFO_PH1_FROM_CARD,
                          mmgsdi_resp_ptr->data_len,
                          mmgsdi_resp_ptr->data_ptr,
                          LPA_SLOT_1);
    status = lpa_asn1_decode_get_profiles_info_resp(mmgsdi_resp_ptr->data_len,
                                                    mmgsdi_resp_ptr->data_ptr,
                                                    &(resp_ptr->message.get_profiles_info_resp));
      break;
    case LPA_GET_EID_MSG:
      /* This response is for GET_DATA APDU sent for GetEID. Lets validate
         the response and call the client callback */
      if(mmgsdi_resp_ptr->data_len != LPA_GET_EID_APDU_RESP_LEN ||
         mmgsdi_resp_ptr->data_ptr == NULL ||
         mmgsdi_resp_ptr->data_ptr[offset++] != LPA_GET_EID_APDU_RESP_DGI_0 ||
         mmgsdi_resp_ptr->data_ptr[offset++] != LPA_GET_EID_APDU_RESP_DGI_1 ||
         mmgsdi_resp_ptr->data_ptr[offset++] != LPA_GET_EID_APDU_RESP_DGI_LEN ||
         mmgsdi_resp_ptr->data_ptr[offset++] != LPA_EID_APDU_TAG ||
         mmgsdi_resp_ptr->data_ptr[offset++] != LPA_EID_LEN)
      {
        status = LPA_GENERIC_ERROR;
        break;
      }
      memscpy(resp_ptr->message.get_eid_resp.eid,
              sizeof(resp_ptr->message.get_eid_resp.eid),
              &mmgsdi_resp_ptr->data_ptr[offset],
              LPA_EID_LEN);
      break;
      default:
        status = LPA_GENERIC_ERROR;
        break;
    }

  LPA_MSG_HIGH_2("lpa_cmd_mmgsdi_cb_parse_legacy_resp status=0x%x for lpa cmd=0x%x",
                 status,
                 resp_ptr->msg_type);
  return status;
} /* lpa_cmd_mmgsdi_cb_parse_legacy_resp */


/*===========================================================================
   FUNCTION:      LPA_CMD_FREE_RAT_GLOBAL

   DESCRIPTION:
     This function frees the global holding the Rules Auth Table

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_free_rat_global (
  lpa_slot_id_enum_type                     lpa_slot
)
{
  if(lpa_slot >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  if(rat_global_ptr[lpa_slot] == NULL)
  {
    return LPA_SUCCESS;
  }

  LPA_FREE(rat_global_ptr[lpa_slot]->rules_auth_table.ppar_entry_ptr);
  memset(rat_global_ptr[lpa_slot], 0x00, sizeof(lpa_get_rat_resp_type));
  LPA_FREE(rat_global_ptr[lpa_slot]);

  return LPA_SUCCESS;
} /* lpa_cmd_free_rat_global */


/*===========================================================================
   FUNCTION:      LPA_CMD_COPY_RAT

   DESCRIPTION:
     This function copies the passed-in RAT to the out parameter.
     This function malloc's the memory. Caller is responsible to free it.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_copy_rat (
  lpa_get_rat_resp_type               *in_rat_ptr,
  lpa_get_rat_resp_type               *out_rat_ptr
)
{
  uint8 index = 0;

  if(in_rat_ptr == NULL ||
     out_rat_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  if(in_rat_ptr->rules_auth_table.num_ppar_entries == 0 ||
     in_rat_ptr->rules_auth_table.ppar_entry_ptr == NULL)
  {
    /* All PPRIDs are forbidden... return SUCCESS */
    return LPA_SUCCESS;
  }

  /* Allocate memory */
  out_rat_ptr->rules_auth_table.ppar_entry_ptr =
    LPA_MALLOC(in_rat_ptr->rules_auth_table.num_ppar_entries * sizeof(lpa_ppar_table_entry_type));

  if(out_rat_ptr->rules_auth_table.ppar_entry_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  /* Copy over the RAT from in_ptr to out_ptr */
  out_rat_ptr->rules_auth_table.num_ppar_entries =
    in_rat_ptr->rules_auth_table.num_ppar_entries;

  for(index = 0; index < out_rat_ptr->rules_auth_table.num_ppar_entries; index++)
  {
    memscpy(&(out_rat_ptr->rules_auth_table.ppar_entry_ptr[index]),
            sizeof(lpa_ppar_table_entry_type),
            &(in_rat_ptr->rules_auth_table.ppar_entry_ptr[index]),
            sizeof(lpa_ppar_table_entry_type));
  }

  return LPA_SUCCESS;
} /* lpa_cmd_copy_rat */


/*===========================================================================
   FUNCTION:      LPA_CMD_COPY_RAT_TO_GLOBAL
   DESCRIPTION:
     This function mallocs a global and copies the passed-in Rules Auth Table
     into it

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_copy_rat_to_global (
  lpa_slot_id_enum_type                          lpa_slot,
  lpa_get_rat_resp_type                         *in_rat_ptr
)
{
  lpa_result_enum_type  lpa_status = LPA_SUCCESS;

  if(in_rat_ptr == NULL ||
     lpa_slot >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Make sure we start clean */
  (void) lpa_cmd_free_rat_global(lpa_slot);

  /* Save the RAT to the global */
  rat_global_ptr[lpa_slot] =
    (lpa_get_rat_resp_type*) LPA_MALLOC(sizeof(lpa_get_rat_resp_type));
  if(rat_global_ptr[lpa_slot] == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  lpa_status = lpa_cmd_copy_rat(in_rat_ptr,
                                rat_global_ptr[lpa_slot]);

  if(LPA_SUCCESS != lpa_status)
  {
    (void) lpa_cmd_free_rat_global(lpa_slot);
  }

  return lpa_status;
} /* lpa_cmd_copy_rat_to_global */


/*===========================================================================
   FUNCTION:      LPA_CMD_IS_OPERATOR_ID_MATCHING

   DESCRIPTION:
     This is a utility function that compares the operator_id in the first
     pointer with the operator_id in the second pointer.
     Note that if there is any wildcard character (as per SGP22) in the second
     pointer, it is considered as a match

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
static boolean lpa_cmd_is_operator_id_matching (
  lpa_operator_id_type          *compare_ptr,
  lpa_operator_id_type          *compare_with_ptr
)
{
  if(compare_ptr == NULL ||
     compare_with_ptr == NULL)
  {
    return FALSE;
  }

  /* Compare the operator_ids taking into account the
     wild characters as per SGP22 */
  if (compare_with_ptr->mcc[0] == LPA_CMD_WILDCHAR_MCC_DIGIT &&
      compare_with_ptr->mcc[1] == LPA_CMD_WILDCHAR_MCC_DIGIT &&
      compare_with_ptr->mcc[2] == LPA_CMD_WILDCHAR_MCC_DIGIT &&
      compare_with_ptr->mnc.mnc_len == LPA_CMD_MNC_LEN_3_BYTES &&
      compare_with_ptr->mnc.mnc_data[0] == LPA_CMD_WILDCHAR_MNC_DIGIT &&
      compare_with_ptr->mnc.mnc_data[1] == LPA_CMD_WILDCHAR_MNC_DIGIT &&
      compare_with_ptr->mnc.mnc_data[2] == LPA_CMD_WILDCHAR_MNC_DIGIT)
  {
    /* If MccMnc is EEEEEE, it means the PPRID is allowed for all operators */
    return TRUE;
  }

  if((compare_with_ptr->mcc[0] == compare_ptr->mcc[0] ||
      compare_with_ptr->mcc[0] == LPA_CMD_WILDCHAR_MCC_DIGIT) &&
     (compare_with_ptr->mcc[1] == compare_ptr->mcc[1] ||
      compare_with_ptr->mcc[1] == LPA_CMD_WILDCHAR_MCC_DIGIT) &&
     (compare_with_ptr->mcc[2] == compare_ptr->mcc[2] ||
      compare_with_ptr->mcc[2] == LPA_CMD_WILDCHAR_MCC_DIGIT) &&
     compare_with_ptr->mnc.mnc_len == compare_ptr->mnc.mnc_len &&
     (compare_with_ptr->mnc.mnc_data[0] == compare_ptr->mcc[0] ||
      compare_with_ptr->mnc.mnc_data[0] == LPA_CMD_WILDCHAR_MNC_DIGIT) &&
     (compare_with_ptr->mnc.mnc_data[1] == compare_ptr->mcc[1] ||
      compare_with_ptr->mnc.mnc_data[1] == LPA_CMD_WILDCHAR_MNC_DIGIT) &&
     (compare_with_ptr->mnc.mnc_data[2] == compare_ptr->mcc[2] ||
      compare_with_ptr->mnc.mnc_data[2] == LPA_CMD_WILDCHAR_MNC_DIGIT))
  {
    /* Any digit can be wildcarded using 'E'. Hence we also compare
       against wildchars above */
    if(compare_with_ptr->gid1.gid_len == LPA_CMD_GID_WILDCHAR_LEN ||
       compare_with_ptr->gid2.gid_len == LPA_CMD_GID_WILDCHAR_LEN)
    {
      return TRUE;
    }

    if(compare_with_ptr->gid1.gid_len == compare_ptr->gid1.gid_len &&
       compare_with_ptr->gid2.gid_len == compare_ptr->gid2.gid_len &&
       memcmp(compare_with_ptr->gid1.gid_data,
              compare_ptr->gid1.gid_data,
              compare_with_ptr->gid1.gid_len) == 0 &&
       memcmp(compare_with_ptr->gid2.gid_data,
              compare_ptr->gid2.gid_data,
              compare_with_ptr->gid2.gid_len) == 0)
    {
      LPA_MSG_MED_0("MccMNc, GID1, GID2 matching");
      return TRUE;
    }
  }

  LPA_MSG_HIGH_0("lpa_cmd_is_operator_id_matching No Match!");
  return FALSE;
} /* lpa_cmd_is_operator_id_matching */


/*===========================================================================
FUNCTION LPA_CMD_COMPARE_RAT_AND_UPDATE_PPRIDS

DESCRIPTION
  This function is used to compare the PPRIDs and operator of a profile with
  the RAT table. If any of those PPRIDs for that operator are allowed,
  that PPRID is returned as 'allowed PPRID' in the out param. Also, if
  user consent is required (per the RAT table), that is also returned in
  another out param.

DEPENDENCIES
  LPA task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_cmd_compare_rat_and_update_pprids(
  lpa_profile_policy_rules_type              *ppr_ptr,
  lpa_rules_auth_table_type                   rules_auth_table
)
{
  uint8                               ppar_index = 0;

  if(ppr_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  if(ppr_ptr->ppr_ids.ppr1 == FALSE &&
     ppr_ptr->ppr_ids.ppr2 == FALSE &&
     ppr_ptr->ppr_ids.ppr3 == FALSE)
  {
    /* No need to update the PRRIDs... */
    return LPA_SUCCESS;
  }

  if(rules_auth_table.num_ppar_entries == 0 ||
     rules_auth_table.ppar_entry_ptr == NULL)
  {
    /* All PPRIDs are forbidden... set all PPRIDs to FALSE */
    ppr_ptr->ppr_ids.ppr1 = FALSE;
    ppr_ptr->ppr_ids.ppr2 = FALSE;
    ppr_ptr->ppr_ids.ppr3 = FALSE;
    return LPA_SUCCESS;
  }

  /* If the PPRID is present in the ProfileInfo for a profile, loop through
     the RAT table to find out if that PPRID is allowed. If not allowed,
     set that PPRID to FALSE in the ProfileInfo for that profile */
  for(ppar_index = 0;
      ppar_index < rules_auth_table.num_ppar_entries;
      ppar_index++)
  {
    uint8          op_id_index       = 0;
    boolean        op_id_match_found = FALSE;

    if(ppr_ptr->ppr_ids.ppr1 &&
       rules_auth_table.ppar_entry_ptr[ppar_index].ppr_ids.ppr1)
    {
      while(op_id_index < rules_auth_table.ppar_entry_ptr[ppar_index].num_operator_ids)
      {
        if(lpa_cmd_is_operator_id_matching(&(ppr_ptr->operator_id),
                                           &(rules_auth_table.ppar_entry_ptr[ppar_index].operator_ids[op_id_index])))
        {
          op_id_match_found = TRUE;
        }
        op_id_index++;
      }

      if(op_id_match_found)
      {
        /* Match found... update the user_consent_required boolean in the ProfileInfo */
        ppr_ptr->user_consent.ppr1_user_consent_required =
          rules_auth_table.ppar_entry_ptr[ppar_index].user_consent_required;
      }
      else
      {
        /* Match not found... set the PPRID in ProfileInfo to FALSE */
        ppr_ptr->ppr_ids.ppr1 = FALSE;
      }
    }

    if(ppr_ptr->ppr_ids.ppr2 &&
       rules_auth_table.ppar_entry_ptr[ppar_index].ppr_ids.ppr2)
    {
      op_id_index       = 0;
      op_id_match_found = FALSE;

      while(op_id_index < rules_auth_table.ppar_entry_ptr[ppar_index].num_operator_ids)
      {
        if(lpa_cmd_is_operator_id_matching(&(ppr_ptr->operator_id),
                                           &(rules_auth_table.ppar_entry_ptr[ppar_index].operator_ids[op_id_index])))
        {
          op_id_match_found = TRUE;
        }
        op_id_index++;
      }

      if(op_id_match_found)
      {
        /* Match found... update the user_consent_required boolean in the ProfileInfo */
        ppr_ptr->user_consent.ppr2_user_consent_required =
          rules_auth_table.ppar_entry_ptr[ppar_index].user_consent_required;
      }
      else
      {
        /* Match not found... set the PPRID in ProfileInfo to FALSE */
        ppr_ptr->ppr_ids.ppr2 = FALSE;
      }
    }

    if(ppr_ptr->ppr_ids.ppr3 &&
       rules_auth_table.ppar_entry_ptr[ppar_index].ppr_ids.ppr3)
    {
      op_id_index       = 0;
      op_id_match_found = FALSE;

      while(op_id_index < rules_auth_table.ppar_entry_ptr[ppar_index].num_operator_ids)
      {
        if(lpa_cmd_is_operator_id_matching(&(ppr_ptr->operator_id),
                                           &(rules_auth_table.ppar_entry_ptr[ppar_index].operator_ids[op_id_index])))
        {
          op_id_match_found = TRUE;
        }
        op_id_index++;
      }

      if(op_id_match_found)
      {
        /* Match found... update the user_consent_required boolean in the ProfileInfo */
        ppr_ptr->user_consent.ppr3_user_consent_required =
          rules_auth_table.ppar_entry_ptr[ppar_index].user_consent_required;
      }
      else
      {
        /* Match not found... set the PPRID in ProfileInfo to FALSE */
        ppr_ptr->ppr_ids.ppr3 = FALSE;
      }
    }
  }

  return LPA_SUCCESS;
} /* lpa_cmd_compare_rat_and_update_pprids */


/*===========================================================================
   FUNCTION:      LPA_CMD_COMPARE_RAT_AND_UPDATE_PPRIDS_ALL_PROFILES

   DESCRIPTION:
     This is a function that compares the PPRIDs in the passed-in get profiles
     info parameter and compares it with the RAT table from the global. After
     comparison, it updated the PPRIDs in the same get profiles info in-out
     pointer.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_compare_rat_and_update_pprids_all_profiles (
  lpa_slot_id_enum_type                            lpa_slot,
  lpa_response_data_type                          *resp_ptr
)
{
  uint8               index                          = 0;

  if(lpa_slot >= LPA_NUM_SLOTS ||
     resp_ptr == NULL ||
     rat_global_ptr[lpa_slot] == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  for(index = 0;
      index < resp_ptr->message.get_profiles_info_resp.num_profiles &&
      index < (sizeof(resp_ptr->message.get_profiles_info_resp.profile_info)/sizeof(resp_ptr->message.get_profiles_info_resp.profile_info[0]));
      index ++)
  {
    (void) lpa_cmd_compare_rat_and_update_pprids(
             &(resp_ptr->message.get_profiles_info_resp.profile_info[index].ppr),
             rat_global_ptr[lpa_slot]->rules_auth_table);
    /* Move to the next profile in ProfilesInfo object */
  }

  return LPA_SUCCESS;
} /* lpa_cmd_compare_rat_and_update_pprids_all_profiles */


/*===========================================================================
   FUNCTION:      LPA_CMD_MMGSDI_CB_PARSE_RESP

   DESCRIPTION:
     This is a function that parses the response received from LPA_MMGSDI
     module for the LPA UI commands.
     This function parses the response as per phase2 of SGP22.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_mmgsdi_cb_parse_resp (
  const lpa_mmgsdi_resp_data_type                 *mmgsdi_resp_ptr,
  lpa_response_data_type                          *resp_ptr
)
{
  lpa_result_enum_type     status = LPA_GENERIC_ERROR;

  if(mmgsdi_resp_ptr == NULL || resp_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  switch(resp_ptr->msg_type)
  {
    case LPA_ENABLE_PROFILE_MSG:
      status = lpa_asn1_decode_enable_profile_resp(mmgsdi_resp_ptr->data_len,
                                                   mmgsdi_resp_ptr->data_ptr);
      break;
    case LPA_DISABLE_PROFILE_MSG:
      status = lpa_asn1_decode_disable_profile_resp(mmgsdi_resp_ptr->data_len,
                                                    mmgsdi_resp_ptr->data_ptr);
      break;
    case LPA_DELETE_PROFILE_MSG:
      status = lpa_asn1_decode_delete_profile_resp(mmgsdi_resp_ptr->data_len,
                                                   mmgsdi_resp_ptr->data_ptr);
      break;
    case LPA_GET_RAT_MSG:
      status = lpa_asn1_decode_get_rat_resp(mmgsdi_resp_ptr->data_len,
                                            mmgsdi_resp_ptr->data_ptr,
                                            &(resp_ptr->message.get_rat_resp));
      if(LPA_SUCCESS == status)
      {
        status = lpa_cmd_copy_rat_to_global(
                   mmgsdi_resp_ptr->slot_id,
                   &(resp_ptr->message.get_rat_resp));
      }
      /* We have retrieved and saved to global the RAT. Now lets
         go ahead and Get Profiles Info from the eUICC.
         Note that RAT table is optional... so, the card might return
         failure for GetRAT. We should not treat that as error and
         should proceed with sending GetProfilesInfo request to the card */
      status = lpa_cmd_internal_get_profiles_info_req(mmgsdi_resp_ptr->slot_id);
      break;
    case LPA_GET_PROFILES_INFO_MSG:
       (void) lpa_efs_write(LPA_FILE_GET_PROFILES_INFO_PH2_FROM_CARD,
                            mmgsdi_resp_ptr->data_len,
                            mmgsdi_resp_ptr->data_ptr,
                            mmgsdi_resp_ptr->slot_id);
      status = lpa_asn1_decode_get_profiles_info_resp(mmgsdi_resp_ptr->data_len,
                                                      mmgsdi_resp_ptr->data_ptr,
                                                    &(resp_ptr->message.get_profiles_info_resp));
      if(LPA_SUCCESS == status)
      {
        /* We already retrieved the RAT. So, now go ahead and compare entries
           in RAT with the profile's pprids. If after comparison we find
           any allowed pprid, we send that in get profiles response to the
           caller of GetProfilesInfo request. */
        (void) lpa_cmd_compare_rat_and_update_pprids_all_profiles(mmgsdi_resp_ptr->slot_id,
                                                                  resp_ptr);
      }
      /* We have updated the PPRID (And the entire GetProfilesInfo response)
         in resp_ptr... we don't need the profiles info global any more */
      (void) lpa_cmd_free_rat_global(mmgsdi_resp_ptr->slot_id);
      break;
    case LPA_CARD_MEMORY_RESET_MSG:
      status = lpa_asn1_decode_euicc_memory_reset_resp(mmgsdi_resp_ptr->data_len,
                                                       mmgsdi_resp_ptr->data_ptr);
      break;
    case LPA_GET_EID_MSG:
      status = lpa_asn1_decode_get_eid_resp(mmgsdi_resp_ptr->data_len,
                                            mmgsdi_resp_ptr->data_ptr,
                                            &resp_ptr->message.get_eid_resp);
      break;
    case LPA_ADD_OR_UPDATE_PROFILE_NICKNAME_MSG:
      status = lpa_asn1_decode_set_nickname_resp(mmgsdi_resp_ptr->data_len,
                                                 mmgsdi_resp_ptr->data_ptr);
      break;
    case LPA_GET_DEFAULT_SERVER_ADDRESS_MSG:
    case LPA_SET_DEFAULT_SERVER_ADDRESS_MSG:
      status = lpa_asn1_decode_euicc_config_address_resp(
                 mmgsdi_resp_ptr->data_len,
                 mmgsdi_resp_ptr->data_ptr,
                 &resp_ptr->message.default_server_address_resp);
      break;
    default:
      status = LPA_GENERIC_ERROR;
      break;
  }

  LPA_MSG_ERR_2("lpa_cmd_mmgsdi_cb_parse_resp, status=0x%x for cmd_type=0x%x",
                status, resp_ptr->msg_type);
  return status;
} /* lpa_cmd_mmgsdi_cb_parse_resp */


/*===========================================================================
   FUNCTION:      LPA_CMD_MMGSDI_CB

   DESCRIPTION:
     This is a callback function called by LPA_MMGSDI module used by it
     to report response for the MMGSDI bound request sent by LPA.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
void lpa_cmd_mmgsdi_cb (
  lpa_result_enum_type                             status,
  const lpa_mmgsdi_resp_data_type                 *mmgsdi_resp_ptr,
  const void                                      *user_data_ptr
)
{
  lpa_response_data_type             *resp_ptr                = NULL;
  boolean                             send_response_to_client = TRUE;

  if(mmgsdi_resp_ptr == NULL)
  {
    return;
  }

  resp_ptr = (lpa_response_data_type*) LPA_MALLOC(sizeof(lpa_response_data_type));
  if(resp_ptr == NULL)
  {
    return;
  }

  resp_ptr->msg_type = (lpa_message_enum_type)user_data_ptr;

  if(status == LPA_SUCCESS ||
     resp_ptr->msg_type == LPA_GET_RAT_MSG)
  {
    lpa_euicc_phase_enum_type          euicc_phase = LPA_PHASE1_1_EUICC;

    /* If we are here because of ENABLE/DISABLE/DELETE/EUICCMEMORYRESET
       command respose from the eUICC, lets send a STATUS on the default
       channel to ensure proactive eUICC is able to trigger a refresh
       right away if possible. We do not care much about the result of
       the operation */
    if(resp_ptr->msg_type == LPA_ENABLE_PROFILE_MSG ||
       resp_ptr->msg_type == LPA_DISABLE_PROFILE_MSG ||
       resp_ptr->msg_type == LPA_DELETE_PROFILE_MSG ||
       resp_ptr->msg_type == LPA_CARD_MEMORY_RESET_MSG)
    {
      (void) lpa_mmgsdi_status(mmgsdi_resp_ptr->slot_id);
    }

    /* If the following fails, it is ok because in that case we treat the
       euicc phase as phase1.1 by default */
    (void) lpa_retrieve_euicc_phase(mmgsdi_resp_ptr->slot_id, &euicc_phase);

    if(euicc_phase == LPA_PHASE1_EUICC ||
       euicc_phase == LPA_PHASE1_1_EUICC)
    {
      status = lpa_cmd_mmgsdi_cb_parse_legacy_resp(mmgsdi_resp_ptr,
                                                   resp_ptr);
    }
    else
    {
      status = lpa_cmd_mmgsdi_cb_parse_resp(mmgsdi_resp_ptr,
                                            resp_ptr);

      /* lpa result enum values greater than UNSUPPORTED PROFILE CLASS are added
         in phase 2.1.*/
      if(euicc_phase < LPA_PHASE2_1_EUICC &&
         status > LPA_ERROR_UNSUPPORTED_PROFILE_CLASS)
      {
        status = LPA_GENERIC_ERROR;
      }

      if(resp_ptr->msg_type == LPA_GET_RAT_MSG &&
         LPA_SUCCESS == status)
      {
        /* GET RAT is an internal command. We should not call the client
           callback (using which the client triggered the GetProfilesInfo) */
        send_response_to_client = FALSE;
      }
      /* The command was processed by the eUICC. Irrespective of the status,
         lets find out if there is any notification that the cards wants to
         send to the server(s). We do not care aobut the return value as this
         is just a best effort. We do this only for GET_PROFILES_INFO. The
         reason is that we assume that get_profiles_info is called by QMIUIM
         after every Enable/Disable/Delete/Add profile operation */
      if(resp_ptr->msg_type == LPA_GET_PROFILES_INFO_MSG)
      {
        (void) lpa_notification_retrieve_and_send(mmgsdi_resp_ptr->slot_id);
      }
    }
  }

  if(send_response_to_client)
  {
    lpa_send_response(status,
                      resp_ptr,
                      mmgsdi_resp_ptr->slot_id,
                      TRUE);
  }

  /* Free everything inside response_ptr */
  lpa_util_free_lpa_response_ptr(resp_ptr);
  LPA_FREE(resp_ptr);
} /* lpa_cmd_mmgsdi_cb */


/*===========================================================================
   FUNCTION:      LPA_CMD_QUEUE_CMD

   DESCRIPTION:
     This function allows client to queue a LPA command to be processed
     in LPA task context.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type  lpa_cmd_queue_cmd (
  lpa_cmd_type                          *lpa_cmd_ptr,
  lpa_slot_id_enum_type                  lpa_slot
)
{
  lpa_result_enum_type  lpa_status  = LPA_SUCCESS;

  if(lpa_cmd_ptr == NULL ||
     lpa_slot >= LPA_NUM_SLOTS)
  {
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  LPA_UTIL_ENTER_TASK_STATE_CRIT_SECT;

  if(!lpa_task_stopped)
  {
    if(q_cnt(&lpa_cmd_q[lpa_slot]) < LPA_CMD_QUEUE_MAX_SIZE)
    {
      (void)q_link(lpa_cmd_ptr, &lpa_cmd_ptr->link);

      /* Put the message on the queue */
      q_put(&lpa_cmd_q[lpa_slot], &lpa_cmd_ptr->link);

      if(UIM_LPA_TCB)
      {
        /* Set the command queue signal */
        (void) rex_set_sigs(UIM_LPA_TCB, LPA_CMD_SIG);
      }
    }
    else
    {
      lpa_status = LPA_ERROR_CMD_QUEUE_FULL;
    }
  }
  else
  {
    LPA_MSG_ERR_0("LPA task stopped, cannot queue command");
    lpa_status = LPA_GENERIC_ERROR;
  }

  LPA_UTIL_LEAVE_TASK_STATE_CRIT_SECT;
  return lpa_status;
} /* lpa_cmd_queue_cmd */


/*===========================================================================
FUNCTION LPA_CMD_CLEANUP

DESCRIPTION
  This function flushes the lpa cmd queue.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_cmd_cleanup (
  lpa_slot_id_enum_type                  lpa_slot
)
{
  lpa_cmd_type  *task_cmd_ptr = NULL;

  if(lpa_slot >= LPA_NUM_SLOTS)
  {
    return;
  }

  while(q_cnt(&lpa_cmd_q[lpa_slot]) != 0)
  {
    task_cmd_ptr = (lpa_cmd_type*)(q_get(&lpa_cmd_q[lpa_slot]));

    if (task_cmd_ptr != NULL)
    {
      if (task_cmd_ptr->lpa_cb_ptr != NULL)
      {
        LPA_MSG_LOW_0("Notify error to client");
        task_cmd_ptr->lpa_cb_ptr(LPA_GENERIC_ERROR,
                                 NULL,
                                 task_cmd_ptr->user_data_ptr);
      }

      LPA_FREE(task_cmd_ptr);
    }
  }
} /* lpa_cmd_cleanup */


/*===========================================================================
FUNCTION LPA_CMD_SET_SIG

DESCRIPTION
  This function sets the LPA command signal.

DEPENDENCIES
  LPA task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_cmd_set_sig (
  void
)
{
  if((q_cnt(&lpa_cmd_q[LPA_SLOT_1]) > 0 &&
      lpa_state_get_current(LPA_SLOT_1) == LPA_STATE_IDLE) ||
     (q_cnt(&lpa_cmd_q[LPA_SLOT_2]) > 0 &&
      lpa_state_get_current(LPA_SLOT_2) == LPA_STATE_IDLE))
  {
    /* Item present on atleast one of the two queues that is in
       IDLE state, that is, ready to process the command. Set the signal */
    (void) rex_set_sigs(UIM_LPA_TCB, LPA_CMD_SIG);
  }
} /* lpa_cmd_set_sig */


/*===========================================================================
FUNCTION LPA_CMD_INTERNAL_GET_PROFILES_INFO_REQ

DESCRIPTION
  Function issues a GetProfilesInfo request.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_internal_get_profiles_info_req (
  lpa_slot_id_enum_type         slot_id
)
{
  lpa_data_type            data;
  lpa_result_enum_type     lpa_status = LPA_GENERIC_ERROR;

  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  memset(&data, 0x00, sizeof(data));

  lpa_status = lpa_asn1_encode_get_profiles_info_request(&data);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  /* Send STORE DATA apdu to the card for GetProfilesInfo operation */
  lpa_status = lpa_mmgsdi_store_data(slot_id,
                                     LPA_PHASE_2_STORE_DATA_P1,
                                     LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                     data,
                                     TRUE,
                                     lpa_cmd_mmgsdi_cb,
                                     (void*) LPA_GET_PROFILES_INFO_MSG);

  LPA_FREE(data.data_ptr);
  data.data_len = 0;

  return lpa_status;
} /* lpa_cmd_internal_get_profiles_info_req */


/*===========================================================================
FUNCTION LPA_CMD_PROCESS_GET_PROFILES_INFO_REQ

DESCRIPTION
  Function processes the get profiles info request. It returns all the
  profiles on the card.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_process_get_profiles_info_req (
  lpa_slot_id_enum_type         slot_id
)
{
  lpa_result_enum_type          lpa_status = LPA_SUCCESS;
  lpa_euicc_phase_enum_type     euicc_phase     = LPA_PHASE1_EUICC;

  lpa_status = lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  if(euicc_phase == LPA_PHASE1_EUICC ||
     euicc_phase == LPA_PHASE1_1_EUICC)
  {
    /* Send GET STATUS to the card in order to retrive profiles info */
    lpa_status = lpa_mmgsdi_get_status(slot_id,
                                       lpa_cmd_mmgsdi_cb,
                                       (void*) LPA_GET_PROFILES_INFO_MSG);
  }
  else
  {
    lpa_data_type            data;

    memset(&data, 0x00, sizeof(data));

    /* Lets first retrieve the RAT table. Once that is retrieved, we will
       trigger the GetProfilesInfo. Note that RAT table is optional... so,
       the card might return failure for GetRAT. We should not treat that
       as error and should proceed with sending GetProfilesInfo request to
       the card */
    lpa_status = lpa_asn1_encode_get_rat_request(&data);
    if(lpa_status != LPA_SUCCESS)
    {
      return lpa_status;
    }

    /* Send STORE DATA apdu to the card for Get RAT */
    lpa_status = lpa_mmgsdi_store_data(slot_id,
                                       LPA_PHASE_2_STORE_DATA_P1,
                                       LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                       data,
                                       TRUE,
                                       lpa_cmd_mmgsdi_cb,
                                       (void*) LPA_GET_RAT_MSG);

    LPA_FREE(data.data_ptr);
    data.data_len = 0;
  }

  return lpa_status;
} /* lpa_cmd_process_get_profiles_info_req */


/*===========================================================================
FUNCTION LPA_CMD_PROCESS_ENABLE_PROFILE_REQ

DESCRIPTION
  Function processes the enable profile request.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_process_enable_profile_req (
  lpa_slot_id_enum_type                        slot_id,
  const lpa_enable_profile_req_type           *enable_profile_req_ptr
)
{
  lpa_euicc_phase_enum_type   euicc_phase      = LPA_PHASE1_EUICC;
  lpa_result_enum_type        lpa_status       = LPA_SUCCESS;
  lpa_data_type               data;
  uint8                       p1               = LPA_ENABLE_PROFILE_APDU_P1;
  boolean                     le_present       = FALSE; 

  if(enable_profile_req_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  memset(&data, 0x00, sizeof(data));

  if(!lpa_util_is_iccid_valid(enable_profile_req_ptr->iccid))
  {
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  lpa_status = lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  if(euicc_phase == LPA_PHASE1_EUICC ||
     euicc_phase == LPA_PHASE1_1_EUICC)
  {
    /* Data field of the STORE DATA APDU for enable_profile request is 15 bytes
       long - initial 5 bytes for Tags/Lengths, and remaining 10 bytes for ICCID */
    uint8                       enable_profile_apdu_data_len = 5 + LPA_ICCID_LEN;
    
    /* For PHASE 1 and PHASE 1_1 EUICC's reponse data is not expected */
    le_present = FALSE;

    /* Malloc and create the Data buffer that needs to be sent with STORE DATA
       SEND APDU command to the card */
    data.data_len = enable_profile_apdu_data_len;
    data.data_ptr = (uint8*) LPA_MALLOC(enable_profile_apdu_data_len);
    if(data.data_ptr == NULL)
    {
      return LPA_ERROR_HEAP_EXHAUSTED;
    }

    data.data_ptr[0] = LPA_ENABLE_PROFILE_APDU_DGI_0;    /* Data[0] */
    data.data_ptr[1] = LPA_ENABLE_PROFILE_APDU_DGI_1;    /* Data[1] */
    data.data_ptr[2] = LPA_ENABLE_PROFILE_APDU_DGI_LEN;  /* Data[2] */
    data.data_ptr[3] = LPA_ICCID_STORE_DATA_TAG;         /* Data[3] */
    data.data_ptr[4] = LPA_ICCID_LEN;                    /* Data[2] */
    memscpy(&data.data_ptr[5],
            LPA_ICCID_LEN,
            enable_profile_req_ptr->iccid.data,
            enable_profile_req_ptr->iccid.data_len);     /* Data[5] onwards contains ICCID */
  }
  else
  {
    p1 = LPA_PHASE_2_STORE_DATA_P1;
    le_present = TRUE;

    lpa_status = lpa_asn1_encode_manage_profile_request(
                   enable_profile_req_ptr->iccid,
                   TRUE,
                   &data,
                   LPA_ASN1_ENCODE_ENABLE_PROFILE);
    if(lpa_status != LPA_SUCCESS)
    {
      return lpa_status;
    }
  }

  /* Send STORE DATA apdu to the card for enabling profile */
  lpa_status = lpa_mmgsdi_store_data(slot_id,
                                     p1,
                                     LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                     data,
                                     le_present,
                                     lpa_cmd_mmgsdi_cb,
                                     (void*) LPA_ENABLE_PROFILE_MSG);
  LPA_FREE(data.data_ptr);
  data.data_len = 0;

  return lpa_status;
} /* lpa_cmd_process_enable_profile_req */


/*===========================================================================
FUNCTION LPA_CMD_PROCESS_DISABLE_PROFILE_REQ

DESCRIPTION
  Function processes the disable profile request.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_process_disable_profile_req (
  lpa_slot_id_enum_type                       slot_id,
  const lpa_disable_profile_req_type         *disable_profile_req_ptr
)
{
  lpa_euicc_phase_enum_type   euicc_phase        = LPA_PHASE1_EUICC;
  lpa_result_enum_type        lpa_status         = LPA_SUCCESS;
  lpa_data_type               data;
  uint8                       p1                 = LPA_ENABLE_PROFILE_APDU_P1;
  boolean                     le_present         = FALSE; 

  if(disable_profile_req_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  memset(&data, 0x00, sizeof(data));

  if(!lpa_util_is_iccid_valid(disable_profile_req_ptr->iccid))
  {
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  lpa_status = lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  if(euicc_phase == LPA_PHASE1_EUICC ||
     euicc_phase == LPA_PHASE1_1_EUICC)
  {
    /* Data field of the STORE DATA APDU for enable_profile request is 15 bytes
       long - initial 5 bytes for Tags/Lengths, and remaining 10 bytes for ICCID */
    uint8 disable_profile_apdu_data_len = 5 + LPA_ICCID_LEN;

    /* For PHASE 1 and PHASE 1_1 EUICC's reponse data is not expected */
    le_present = FALSE;

    /* Malloc and create the Data buffer that needs to be sent with STORE DATA
       SEND APDU command to the card */
    data.data_len = disable_profile_apdu_data_len;
    data.data_ptr = (uint8*) LPA_MALLOC(disable_profile_apdu_data_len);
    if(data.data_ptr == NULL)
    {
      return LPA_ERROR_HEAP_EXHAUSTED;
    }
    data.data_ptr[0] = LPA_DISABLE_PROFILE_APDU_DGI_0;   /* Data[0] */
    data.data_ptr[1] = LPA_DISABLE_PROFILE_APDU_DGI_1;   /* Data[1] */
    data.data_ptr[2] = LPA_DISABLE_PROFILE_APDU_DGI_LEN; /* Data[2] */
    data.data_ptr[3] = LPA_ICCID_STORE_DATA_TAG;         /* Data[3] */
    data.data_ptr[4] = LPA_ICCID_LEN;                    /* Data[2] */
    memscpy(&data.data_ptr[5],
            LPA_ICCID_LEN,
            disable_profile_req_ptr->iccid.data,
            disable_profile_req_ptr->iccid.data_len);    /* Data[5] onwards contains ICCID */
  }
  else
  {
    p1 = LPA_PHASE_2_STORE_DATA_P1;
    le_present = TRUE;

    lpa_status = lpa_asn1_encode_manage_profile_request(
                   disable_profile_req_ptr->iccid,
                   TRUE,
                   &data,
                   LPA_ASN1_ENCODE_DISABLE_PROFILE);
    if(lpa_status != LPA_SUCCESS)
    {
      return lpa_status;
    }
  }

  /* Send STORE DATA apdu to the card for enabling profile */
  lpa_status = lpa_mmgsdi_store_data(slot_id,
                                     p1,
                                     LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                     data,
                                     le_present,
                                     lpa_cmd_mmgsdi_cb,
                                     (void*) LPA_DISABLE_PROFILE_MSG);
  LPA_FREE(data.data_ptr);
  data.data_len = 0;

  return lpa_status;
} /* lpa_cmd_process_disable_profile_req */


/*===========================================================================
FUNCTION LPA_CMD_PROCESS_RESET_CARD_MEMORY_REQ

DESCRIPTION
  Function processes the reset card memory equest.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_process_reset_card_memory_req (
  lpa_slot_id_enum_type                        slot_id,
  const lpa_card_memory_reset_req_type        *card_memory_reset_req_ptr
)
{
  lpa_euicc_phase_enum_type euicc_phase = LPA_PHASE1_EUICC;
  lpa_result_enum_type      lpa_status  = LPA_SUCCESS;
  lpa_data_type             data;
  uint8                     p1          = LPA_CARD_MEMORY_RESET_APDU_P1;
  boolean                   le_present  = FALSE;

  memset(&data, 0x00, sizeof(data));

  if(card_memory_reset_req_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  lpa_status = lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  if(euicc_phase == LPA_PHASE1_EUICC ||
     euicc_phase == LPA_PHASE1_1_EUICC)
  {
    /* Data field of the STORE DATA APDU for reset_card_memory request is 6 bytes
       long - for the TLVs are defined in SGP.22 phase1/1.1*/
    uint8              card_memory_reset_apdu_data_len = 6;

    /* For PHASE 1 and PHASE 1_1 EUICC's reponse data is not expected */
    le_present = FALSE;

    /* Create the Data buffer that needs to be sent with STORE DATA SEND APDU
       command to the card */
    data.data_len = card_memory_reset_apdu_data_len;
    data.data_ptr = (uint8*) LPA_MALLOC(card_memory_reset_apdu_data_len);
    if(data.data_ptr == NULL)
    {
      return LPA_ERROR_HEAP_EXHAUSTED;
    }

    data.data_ptr[0] = LPA_CARD_MEMORY_RESET_APDU_DGI_0;   /* Data[0].. DGI Tag */
    data.data_ptr[1] = LPA_CARD_MEMORY_RESET_APDU_DGI_1;   /* Data[1].. DGI Tag */
    data.data_ptr[2] = LPA_CARD_MEMORY_RESET_APDU_DGI_LEN; /* Data[2].. DGI Tag Len */
    data.data_ptr[3] = LPA_CARD_MEMORY_RESET_APDU_TAG;     /* Data[3].. eUICCMemoryReset Tag */
    data.data_ptr[4] = LPA_CARD_MEMORY_RESET_APDU_LEN;     /* Data[4].. eUICCMemoryReset Len */
    data.data_ptr[5] = LPA_CARD_MEMORY_RESET_APDU_VAL;     /* Data[5].. eUICCMemoryReset Value
                                                            (= Delete all profiles) */
  }
  else
  {
    p1 = LPA_PHASE_2_STORE_DATA_P1;
    le_present = TRUE;

    lpa_status = lpa_asn1_encode_euicc_memory_reset_request(
                   card_memory_reset_req_ptr->option,
                   &data);
    if(lpa_status != LPA_SUCCESS)
    {
      return lpa_status;
    }
  }

  /* Send STORE DATA apdu to the card for euicc reset */
  lpa_status = lpa_mmgsdi_store_data(slot_id,
                                     p1,
                                     LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                     data,
                                     le_present,
                                     lpa_cmd_mmgsdi_cb,
                                     (void*) LPA_CARD_MEMORY_RESET_MSG);

  LPA_FREE(data.data_ptr);
  data.data_len = 0;

  return lpa_status;
} /* lpa_cmd_process_reset_card_memory_req */


/*===========================================================================
FUNCTION LPA_CMD_PROCESS_DELETE_PROFILE_REQ

DESCRIPTION
  Function processes the DELETE PROFILE request.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_process_delete_profile_req (
  lpa_slot_id_enum_type                       slot_id,
  const lpa_delete_profile_req_type          *delete_profile_req_ptr
)
{
  lpa_result_enum_type    lpa_status = LPA_SUCCESS;
  lpa_euicc_phase_enum_type   euicc_phase     = LPA_PHASE1_EUICC;

  if(delete_profile_req_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  lpa_status = lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  if(euicc_phase == LPA_PHASE1_EUICC ||
     euicc_phase == LPA_PHASE1_1_EUICC)
  {
    /* Send GET DATA apdu to the card for retrieving EID */
    lpa_status = lpa_mmgsdi_delete(slot_id,
                           delete_profile_req_ptr->iccid,
                           lpa_cmd_mmgsdi_cb,
                           (void*) LPA_DELETE_PROFILE_MSG);
  }
  else
  {
    lpa_data_type                          data;

    memset(&data, 0x00, sizeof(data));

    lpa_status = lpa_asn1_encode_manage_profile_request(
                   delete_profile_req_ptr->iccid,
                   TRUE,
                   &data,
                   LPA_ASN1_ENCODE_DELETE_PROFILE);
    if(lpa_status != LPA_SUCCESS)
    {
      return lpa_status;
    }

    /* Send STORE DATA apdu to the card for deleting profile */
    lpa_status = lpa_mmgsdi_store_data(slot_id,
                                       LPA_PHASE_2_STORE_DATA_P1,
                                       LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                       data,
                                       TRUE,
                                       lpa_cmd_mmgsdi_cb,
                                       (void*) LPA_DELETE_PROFILE_MSG);

    LPA_FREE(data.data_ptr);
    data.data_len = 0;
  }

  return lpa_status;
} /* lpa_cmd_process_delete_profile_req */


/*===========================================================================
FUNCTION LPA_CMD_PROCESS_GET_EID_REQ

DESCRIPTION
  Function processes the get eid request.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_process_get_eid_req (
  lpa_slot_id_enum_type         slot_id
)
{
  lpa_euicc_phase_enum_type    euicc_phase     = LPA_PHASE1_EUICC;
  lpa_result_enum_type         lpa_status      = LPA_SUCCESS;
  lpa_data_type                data;
  lpa_cache_eid_info_type     *cache_eid_ptr   = NULL;
  lpa_response_data_type      *resp_ptr        = NULL;

  memset(&data, 0x00, sizeof(data));

  lpa_status = lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }
  
  /* If there is EID already cached, try to retrieve it and send
     to the client instead of sending a command to the card.  */
  cache_eid_ptr = lpa_get_slot_eid_instance(slot_id);
  if(cache_eid_ptr != NULL)
  {
    if(cache_eid_ptr->data_len > 0)
	{
      /* copy the EID from the eid global cache to the client response
	  */
	  resp_ptr = (lpa_response_data_type*) LPA_MALLOC(sizeof(lpa_response_data_type));
	  if(resp_ptr == NULL)
	  {
        return LPA_ERROR_HEAP_EXHAUSTED;
	  }
	  
	  resp_ptr->msg_type = LPA_GET_EID_MSG;
	  memscpy(resp_ptr->message.get_eid_resp.eid,
	          sizeof(resp_ptr->message.get_eid_resp.eid),
			  cache_eid_ptr->data,
			  cache_eid_ptr->data_len);
			  
      lpa_send_response(lpa_status,
	                    resp_ptr,
						slot_id,
						TRUE);
      return lpa_status;
	}
  }

  if(euicc_phase == LPA_PHASE1_EUICC ||
     euicc_phase == LPA_PHASE1_1_EUICC)
  {
    /* Data field of the GET DATA APDU for get_eid request is 3 bytes
       long - for the TLVs are defined in SGP.22 phase1/1.1*/
    uint8      get_eid_apdu_data_len = 3;

    /* Create the Data buffer that needs to be sent with GET DATA SEND APDU
       command to the card */
    data.data_len = get_eid_apdu_data_len;
    data.data_ptr = (uint8*) LPA_MALLOC(get_eid_apdu_data_len);
    if(data.data_ptr == NULL)
    {
      return LPA_ERROR_HEAP_EXHAUSTED;
    }

    data.data_ptr[0] = LPA_GET_EID_APDU_DATA_BYTE_1;   /* Data[0] */
    data.data_ptr[1] = LPA_GET_EID_APDU_DATA_BYTE_2;   /* Data[1] */
    data.data_ptr[2] = LPA_GET_EID_APDU_DATA_BYTE_3;   /* Data[2] */

    /* Send GET DATA apdu to the card for retrieving EID */
    lpa_status = lpa_mmgsdi_get_data(slot_id,
                                     LPA_GET_EID_APDU_DATA_P2,
                                     data,
                                     lpa_cmd_mmgsdi_cb,
                                     (void*) LPA_GET_EID_MSG);
  }
  else
  {
    lpa_status = lpa_asn1_encode_get_eid_request(&data);
    if(lpa_status != LPA_SUCCESS)
    {
      return lpa_status;
    }

    /* Send STORE DATA apdu to the card for deleting profile */
    lpa_status = lpa_mmgsdi_store_data(slot_id,
                                       LPA_PHASE_2_STORE_DATA_P1,
                                       LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                       data,
                                       TRUE,
                                       lpa_cmd_mmgsdi_cb,
                                       (void*) LPA_GET_EID_MSG);
  }

  LPA_FREE(data.data_ptr);
  data.data_len = 0;

  return lpa_status;
} /* lpa_cmd_process_get_eid_req */


/*===========================================================================
FUNCTION LPA_CMD_PROCESS_ADD_PROFILE_REQ

DESCRIPTION
  Function processes the add profile request.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_process_add_profile_req (
  lpa_slot_id_enum_type                    slot_id,
  const lpa_add_profile_req_type          *add_profile_req_ptr
)
{
  return lpa_lpd_start_add_profile_procedure(
           add_profile_req_ptr,
           slot_id);
} /* lpa_cmd_process_add_profile_req */


/*===========================================================================
FUNCTION LPA_CMD_PROCESS_ADD_OR_UPDATE_PROFILE_NICKNAME_REQ

DESCRIPTION
  Function processes the add or update profile nickname request.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_process_add_or_update_profile_nickname_req (
  lpa_slot_id_enum_type                              slot_id,
  const lpa_add_or_update_profile_nickname_req_type *add_or_update_profile_nickname_req_ptr
)
{
  lpa_result_enum_type                lpa_status   = LPA_SUCCESS;
  lpa_data_type                       data;
  lpa_asn1_set_nickname_request_type  nickname;
  lpa_euicc_phase_enum_type           euicc_phase  = LPA_PHASE1_EUICC;
  uint8                               p1           = LPA_ADD_OR_UPDATE_NICKNAME_APDU_P1;
  boolean                             le_present   = FALSE;

  memset(&nickname, 0x00, sizeof(nickname));

  if(add_or_update_profile_nickname_req_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  lpa_status = lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  if(euicc_phase != LPA_PHASE1_EUICC &&
     euicc_phase != LPA_PHASE1_1_EUICC)
  {
    p1          = LPA_PHASE_2_STORE_DATA_P1;
    le_present  = TRUE;
  }

  nickname.iccid =  add_or_update_profile_nickname_req_ptr->iccid;
  nickname.profile_nickname =  add_or_update_profile_nickname_req_ptr->nickname;

  /* This encoder might malloc the data.data_ptr if the nickname is
     non null. Make sure to free it when applicable */
  lpa_asn1_encode_set_nickname_request(nickname, &data);

    /* Write raw activation code to the file */
  (void) lpa_efs_write(LPA_FILE_ASN1_ENCODED_NICKNAME,
                       data.data_len,
                       data.data_ptr,
                       slot_id);

  /* Send STORE DATA apdu to the card for euicc reset */
  lpa_status = lpa_mmgsdi_store_data(slot_id,
                                     p1,
                                     LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                     data,
                                     le_present,
                                     lpa_cmd_mmgsdi_cb,
                                     (void*) LPA_ADD_OR_UPDATE_PROFILE_NICKNAME_MSG);
  LPA_FREE(data.data_ptr);
  data.data_len = 0;

  return lpa_status;
} /* lpa_cmd_process_add_or_update_profile_nickname_req */


/*===========================================================================
FUNCTION LPA_CMD_PROCESS_DEFAULT_SERVER_ADDRESS_REQ

DESCRIPTION
  Function processes the configure/retrieve default SM-DS/SM-DP+ address on
  the eUICC

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_cmd_process_default_server_address_req (
  lpa_slot_id_enum_type                         slot_id,
  const lpa_cmd_data_type                      *cmd_data_ptr
)
{
  lpa_result_enum_type                lpa_status   = LPA_SUCCESS;
  lpa_data_type                       data;
  lpa_euicc_phase_enum_type           euicc_phase  = LPA_PHASE1_EUICC;

  if(cmd_data_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  lpa_status = lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  if(euicc_phase == LPA_PHASE1_EUICC ||
     euicc_phase == LPA_PHASE1_1_EUICC)
  {
    return LPA_ERROR_NOT_SUPPORTED;
  }

  if(cmd_data_ptr->cmd_enum == LPA_SET_DEFAULT_SERVER_ADDRESS_MSG)
  {
    /* This is a SET request... ASN1 encode the request.
       This encoder might malloc the data.data_ptr if the nickname is
       non null. Make sure to free it when applicable */
    lpa_asn1_encode_set_default_dpaddress_request(
      cmd_data_ptr->cmd.set_default_server_address_req.default_smdp_address,
      &data);
    /* Write the ASN1 encoded request to the EFS */
    (void) lpa_efs_write(LPA_FILE_ASN1_ENCODED_SET_DEFAULT_ADDRESS,
                         data.data_len,
                         data.data_ptr,
                         slot_id);
  }
  else if(cmd_data_ptr->cmd_enum == LPA_GET_DEFAULT_SERVER_ADDRESS_MSG)
  {
    /* This is a GET request... ASN1 encode the request.
       This encoder might malloc the data.data_ptr if the nickname is
       non null. Make sure to free it when applicable */
    lpa_asn1_encode_get_euicc_config_address_request(&data);
    /* Write the ASN1 encoded request to the EFS */
    (void) lpa_efs_write(LPA_FILE_ASN1_ENCODED_GET_DEFAULT_ADDRESS,
                         data.data_len,
                         data.data_ptr,
                         slot_id);
  }
  else
  {
    return LPA_GENERIC_ERROR;
  }

  /* Send STORE DATA apdu to the card for euicc reset */
  lpa_status = lpa_mmgsdi_store_data(slot_id,
                                     LPA_PHASE_2_STORE_DATA_P1,
                                     LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                     data,
                                     TRUE,
                                     lpa_cmd_mmgsdi_cb,
                                     (void*) cmd_data_ptr->cmd_enum);
  LPA_FREE(data.data_ptr);
  data.data_len = 0;

  return lpa_status;
} /* lpa_cmd_process_default_server_address_req */


/*===========================================================================
   FUNCTION:      LPA_CMD_MMGSDI_SELECT_ISDR_CB

   DESCRIPTION:
     This is a callback function called by LPA_MMGSDI module used by it
     to report response for the SELECT ISDR response. As part of response
     handling of SELECT ISDR in lpa_mmgsdi, SVN (and hence the eUICC phase)
     is set which is used in LPA_CMD module to figure out how to compose
     APDUs for various LPA commands

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
static void lpa_cmd_mmgsdi_select_isdr_cb (
  lpa_result_enum_type                             status,
  const lpa_mmgsdi_resp_data_type                 *mmgsdi_resp_ptr,
  const void                                      *user_data_ptr
)
{
  lpa_cmd_type                   *lpa_cmd_ptr   = NULL;

  (void) mmgsdi_resp_ptr;

  /* Received response from LPA_MMGSDI for SELECT ISDR */
  lpa_cmd_ptr = (lpa_cmd_type*) user_data_ptr;

  if(lpa_cmd_ptr == NULL)
  {
    lpa_cmd_set_sig();
    return;
  }

  if(status == LPA_SUCCESS)
  {
    switch(lpa_cmd_ptr->cmd.cmd_enum)
    {
      case LPA_GET_PROFILES_INFO_MSG:
        status = lpa_cmd_process_get_profiles_info_req(
                   lpa_cmd_ptr->slot_id);
        break;
      case LPA_ENABLE_PROFILE_MSG:
        status = lpa_cmd_process_enable_profile_req(
                   lpa_cmd_ptr->slot_id,
                   &lpa_cmd_ptr->cmd.cmd.enable_profile_req);
        break;
      case LPA_DISABLE_PROFILE_MSG:
        status = lpa_cmd_process_disable_profile_req(
                   lpa_cmd_ptr->slot_id,
                   &lpa_cmd_ptr->cmd.cmd.disable_profile_req);
        break;
      case LPA_DELETE_PROFILE_MSG:
        status = lpa_cmd_process_delete_profile_req(
                   lpa_cmd_ptr->slot_id,
                   &lpa_cmd_ptr->cmd.cmd.delete_profile_req);
        break;
      case LPA_CARD_MEMORY_RESET_MSG:
        status = lpa_cmd_process_reset_card_memory_req(
                   lpa_cmd_ptr->slot_id,
                   &lpa_cmd_ptr->cmd.cmd.card_memory_reset_req);
        break;
      case LPA_GET_EID_MSG:
        status = lpa_cmd_process_get_eid_req(
                   lpa_cmd_ptr->slot_id);
        break;
      case LPA_ADD_PROFILE_MSG:
        status = lpa_cmd_process_add_profile_req(
                   lpa_cmd_ptr->slot_id,
                   &lpa_cmd_ptr->cmd.cmd.add_profile_req);
        break;
      case LPA_ADD_OR_UPDATE_PROFILE_NICKNAME_MSG:
        status = lpa_cmd_process_add_or_update_profile_nickname_req(
                   lpa_cmd_ptr->slot_id,
                   &lpa_cmd_ptr->cmd.cmd.add_or_update_profile_nickname_req);
        break;
      case LPA_SET_DEFAULT_SERVER_ADDRESS_MSG:
      case LPA_GET_DEFAULT_SERVER_ADDRESS_MSG:
        status = lpa_cmd_process_default_server_address_req(
                   lpa_cmd_ptr->slot_id,
                   &(lpa_cmd_ptr->cmd));
        break;
      default:
        status = LPA_GENERIC_ERROR;
        break;
    }
  }

  if(status != LPA_SUCCESS)
  {
    /* Send the response and clear the timer */
    lpa_response_data_type            *resp_ptr = NULL;

    resp_ptr = (lpa_response_data_type*) LPA_MALLOC(sizeof(lpa_response_data_type));
    if(resp_ptr != NULL)
    {
      resp_ptr->msg_type = lpa_cmd_ptr->cmd.cmd_enum;
    }
    lpa_send_response(status,
                      resp_ptr,
                      lpa_cmd_ptr->slot_id,
                      TRUE);
    LPA_FREE(resp_ptr);
  }

  lpa_cmd_free_task_cmd_ptr(lpa_cmd_ptr);
  LPA_FREE(lpa_cmd_ptr);

  /* Set the signal if there are one or more commands still in either of the
     two queues and LPA is currently in IDLE state for that queue/slot. */
  lpa_cmd_set_sig();
} /* lpa_cmd_mmgsdi_select_isdr_cb */


/*===========================================================================
FUNCTION LPA_CMD_HANDLE_CMD_SIG

DESCRIPTION
  This function, called in lpa_main, is called to process the LPA command

DEPENDENCIES
  LPA task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_cmd_handle_cmd_sig (
  void
)
{
  lpa_cmd_type                 *lpa_cmd_ptr              = NULL;
  lpa_result_enum_type          lpa_status               = LPA_GENERIC_ERROR;
  boolean                       lpa_process_slot_1_cmd_q = FALSE;
  boolean                       lpa_process_slot_2_cmd_q = FALSE;

  (void) rex_clr_sigs(rex_self(), LPA_CMD_SIG);

  /* If LPA is not in a valid state to handle a new command then
     leave command in queue and continue processing. In order to
     make sure this 'ignored' command is dequeued when the right
     time arrives, LPA will set the cmd signal once the current
     command finishes processing (that is, when the LPA goes back
     to IDLE state in which it can process a new command). */
  /* Alternate between SLOT1 and SLOT2 queues. */
  if(lpa_state_get_current(LPA_SLOT_1) == LPA_STATE_IDLE &&
     q_cnt(&lpa_cmd_q[LPA_SLOT_1]) > 0)
  {
    lpa_process_slot_1_cmd_q = TRUE;
  }
  if(lpa_state_get_current(LPA_SLOT_2) == LPA_STATE_IDLE &&
     q_cnt(&lpa_cmd_q[LPA_SLOT_2]) > 0)
  {
    lpa_process_slot_2_cmd_q = TRUE;
  }

  /* If there are commands waiting in both the queues, see what was the last
     queue from which the command was processed. This time we should
     alternate to the other queue */
  if(((lpa_process_slot_1_cmd_q == TRUE &&
       lpa_process_slot_2_cmd_q == TRUE) &&
      lpa_last_cmd_q_processed == LPA_SLOT_2)||
     (lpa_process_slot_1_cmd_q == TRUE &&
      lpa_process_slot_2_cmd_q == FALSE))
  {
    lpa_cmd_ptr = (lpa_cmd_type*)(q_get(&lpa_cmd_q[LPA_SLOT_1]));
    lpa_last_cmd_q_processed = LPA_SLOT_1;
  }
  else if(((lpa_process_slot_1_cmd_q == TRUE &&
            lpa_process_slot_2_cmd_q == TRUE) &&
           lpa_last_cmd_q_processed == LPA_SLOT_1)||
          (lpa_process_slot_1_cmd_q == FALSE &&
           lpa_process_slot_2_cmd_q == TRUE))
  {
    lpa_cmd_ptr = (lpa_cmd_type*)(q_get(&lpa_cmd_q[LPA_SLOT_2]));
    lpa_last_cmd_q_processed = LPA_SLOT_2;
  }

  /* If there is a command on either of the queues */
  if(lpa_cmd_ptr == NULL)
  {
    return;
  }

  LPA_MSG_HIGH_1("LPA_CMD_SIG received for command: 0x%x",
                 lpa_cmd_ptr->cmd.cmd_enum);

  /* Start overall sanity timer */
  lpa_timer_set(lpa_cmd_ptr->slot_id, LPA_TIMER_TYPE_SANITY_TIMER);

  /* Save client response callback etc to a global that can later be
     used to send the response to the caller */
  lpa_status = lpa_save_client_data(lpa_cmd_ptr->lpa_cb_ptr,
                                    lpa_cmd_ptr->event_cb_ptr,
                                    lpa_cmd_ptr->user_data_ptr,
                                    lpa_cmd_ptr->slot_id,
                                    lpa_cmd_ptr->cmd.cmd_enum);

  if(lpa_status == LPA_SUCCESS)
  {
    /* First lets move the state to INIT so that we do not process any other
       ES10c command until we are done processing this current command */
    lpa_status = lpa_state_change(LPA_STATE_IDLE, LPA_STATE_INIT, lpa_cmd_ptr->slot_id);
    if(lpa_status == LPA_SUCCESS)
    {
      /* Before this LPA ES10c command can be processed, we need to ensure
         SELECT ISDR has been performed. Once lpa_mmgsdi responds to this,
         we go ahead with the original ES10c command */
      lpa_status = lpa_mmgsdi_select_isdr(lpa_cmd_ptr->slot_id,
                                          lpa_cmd_mmgsdi_select_isdr_cb,
                                          (void*) lpa_cmd_ptr);
    }
  }

  if (lpa_status != LPA_SUCCESS)
  {
    /* Send the response and clear the timer */
    lpa_response_data_type            *resp_ptr = NULL;

    resp_ptr = (lpa_response_data_type*) LPA_MALLOC(sizeof(lpa_response_data_type));
    if(resp_ptr != NULL)
    {
      resp_ptr->msg_type = lpa_cmd_ptr->cmd.cmd_enum;
    }
    lpa_send_response(lpa_status,
                      resp_ptr,
                      lpa_cmd_ptr->slot_id,
                      TRUE);
    LPA_FREE(resp_ptr);
    lpa_cmd_free_task_cmd_ptr(lpa_cmd_ptr);
    LPA_FREE(lpa_cmd_ptr);

    /* Set the signal if there are one or more commands still in either of the
       two queues and LPA is currently in IDLE state for that queue/slot. */
    lpa_cmd_set_sig();
  }
} /* lpa_cmd_handle_cmd_sig */
