/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


             M M G S D I  S U B S C R I P T I O N  S E L E C T I O N  F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains routines used for subscription provisioning
  from MMGSDI EFS.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2013 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_sub_selection.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/23/17   sk      QMI error handling for repeated map sessions api
01/10/17   bcho    Support for slot specific CDMA LESS NV feature
07/28/16   ar      Handle auto-activation race cond by passing an extra param
04/26/16   ar      Removing the mmgsdi_cnf_map_req_to_cnf_enum_type()
04/13/16   ar      CDMA/HDR support on 2nd SUB
08/31/15   yt      Remove length information from requests and confirmations
08/25/15   bcho    Support for CDMALess Device
08/20/15   ar      Removal of multisim_auto_provisioning by using stubs
07/22/15   yt      Activate CSIM/RUIM only when RTRE config is not NV_ONLY
06/11/15   lm      Remove redundant code in slot index validation
05/14/15   yt      Support for mapping sessions to slots
04/29/15   bcho    Support for App specific cache
04/03/15   av      Migrate to mcfg EFS APIs
03/25/15   kk      Subs prov EFS error handling
07/28/14   vv      Remove unused data types
06/16/14   kk      Fixed usim/csim activation sequence
01/11/14   df      Fix off-target compilation error
01/10/14   df      Use v2.0 diag macros
10/16/13   df      Convert gsdi_efs functions to return mmgsdi type
10/04/13   rp      F3 message reduction
08/30/13   tl      Convert channel info table from static to dynamic allocation
08/21/13   spo     Resolving build error
08/20/13   spo     Initial version

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "customer.h"
#include "uim_msg.h"
#include "mmgsdiutil.h"
#include "mmgsdi_session.h"
#include "mmgsdicache.h"
#include "mmgsdi_nv.h"
#include "mmgsdi_sub_selection.h"
#include "mmgsdi_cnf.h"
#include "intconv.h"

/* <EJECT> */
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* ----------------------------------------------------------------------------
   STRUCTURE: MMGSDI_SESSIONS_TO_SLOTS_REQ_INFO_TYPE

   DESCRIPTION:
     This is used to hold the info related to MMGSDI_MAP_SESSIONS_TO_SLOTS_REQ
     being handled currently
     requested_mapping : Sessions-to-slots mapping requested by client
     response_cb_ptr   : Client callback to be called after (de)activation
     client_data       : Client reference data
  ------------------------------------------------------------------------------- */
typedef struct{
  mmgsdi_sessions_to_slots_mapping_type  requested_mapping;
  mmgsdi_client_id_type                  client_id;
  mmgsdi_callback_type                   response_cb_ptr;
  mmgsdi_client_data_type                client_data;
}mmgsdi_sessions_to_slots_req_info_type;

static struct{
  boolean                                is_mapping_valid;
  boolean                                in_progress;
  mmgsdi_sessions_to_slots_mapping_type  mapping;
} mmgsdi_sessions_to_slots_mapping = {FALSE};


/*===========================================================================
FUNCTION:  MMGSDI_SUB_SEL_GET_SESSION_ID_FOR_SLOT

DESCRIPTION
  This function finds the MMGSDI session ID for a slot based on the stored
  sessions-to-slots mapping.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_sub_sel_get_session_id_for_slot(
  mmgsdi_slot_id_enum_type      slot_id,
  mmgsdi_technology_enum_type   tech_type,
  mmgsdi_session_id_type      * session_id_ptr
)
{
  mmgsdi_session_id_type  * session_ids_ptr  = NULL;
  mmgsdi_session_id_type    session_ids_gw[] =
                              {mmgsdi_generic_data_ptr->pri_gw_session_id,
                               mmgsdi_generic_data_ptr->sec_gw_session_id,
                               mmgsdi_generic_data_ptr->ter_gw_session_id};
  mmgsdi_session_id_type    session_ids_1x[] =
                              {mmgsdi_generic_data_ptr->pri_1x_session_id,
                               mmgsdi_generic_data_ptr->sec_1x_session_id,
                               mmgsdi_generic_data_ptr->ter_1x_session_id};

  if((mmgsdi_util_is_slot_valid(slot_id) == FALSE) ||
     session_id_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  if(tech_type == MMGSDI_TECH_3GPP)
  {
    session_ids_ptr = session_ids_gw;
  }
  else
  {
    session_ids_ptr = session_ids_1x;
  }

  if(mmgsdi_sessions_to_slots_mapping.is_mapping_valid)
  {
    if(slot_id == mmgsdi_sessions_to_slots_mapping.mapping.pri_slot)
    {
      *session_id_ptr = session_ids_ptr[0];
    }
    else if(slot_id == mmgsdi_sessions_to_slots_mapping.mapping.sec_slot)
    {
      *session_id_ptr = session_ids_ptr[1];
    }
    else if(slot_id == mmgsdi_sessions_to_slots_mapping.mapping.ter_slot)
    {
      *session_id_ptr = session_ids_ptr[2];
    }
    else
    {
      return MMGSDI_ERROR;
    }
  }
  else
  {
    switch(slot_id)
    {
      case MMGSDI_SLOT_1:
        *session_id_ptr = session_ids_ptr[0];
        break;
      case MMGSDI_SLOT_2:
        *session_id_ptr = session_ids_ptr[1];
        break;
      case MMGSDI_SLOT_3:
        *session_id_ptr = session_ids_ptr[2];
        break;
      default:
        return MMGSDI_ERROR;
    }
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_sub_sel_get_session_id_for_slot */


/*===========================================================================
FUNCTION:  MMGSDI_SUB_SEL_MAP_SESSIONS_TO_SLOTS_SEND_CNF

DESCRIPTION
  This function processes the MMGSDI_MAP_SESSIONS_TO_SLOTS_REQ.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static void mmgsdi_sub_sel_map_sessions_to_slots_send_cnf(
  mmgsdi_return_enum_type                  cnf_status,
  mmgsdi_sessions_to_slots_req_info_type * client_data_ptr
)
{
  mmgsdi_cnf_type  *client_cnf_ptr  = NULL;

  if(client_data_ptr == NULL || client_data_ptr->response_cb_ptr == NULL)
  {
    return;
  }

  mmgsdi_sessions_to_slots_mapping.in_progress = FALSE;

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(client_cnf_ptr, sizeof(mmgsdi_cnf_type));

  if(client_cnf_ptr == NULL)
  {
    return;
  }

  if(cnf_status == MMGSDI_SUCCESS)
  {
    /* Copy requested mapping to global if it was successfully applied */
    mmgsdi_sessions_to_slots_mapping.mapping = client_data_ptr->requested_mapping;
    mmgsdi_sessions_to_slots_mapping.is_mapping_valid = TRUE;
  }

  client_cnf_ptr->map_sessions_to_slots_cnf.response_header.mmgsdi_status =
    cnf_status;
  client_cnf_ptr->map_sessions_to_slots_cnf.response_header.client_id =
    client_data_ptr->client_id;
  client_cnf_ptr->map_sessions_to_slots_cnf.response_header.client_data =
    client_data_ptr->client_data;
  client_cnf_ptr->map_sessions_to_slots_cnf.response_header.response_type =
    MMGSDI_MAP_SESSIONS_TO_SLOTS_REQ;
  client_cnf_ptr->map_sessions_to_slots_cnf.response_header.status_word.present =
     FALSE;

  UIM_MSG_HIGH_1("sending MMGSDI_MAP_SESSIONS_TO_SLOTS_CNF, status: 0x%x",
                 cnf_status);

  client_data_ptr->response_cb_ptr(cnf_status,
                                   MMGSDI_MAP_SESSIONS_TO_SLOTS_REQ,
                                   client_cnf_ptr);

  MMGSDIUTIL_TMC_MEM_FREE(client_cnf_ptr);
} /* mmgsdi_sub_sel_map_sessions_to_slots_send_cnf */


/*===========================================================================
FUNCTION:  MMGSDI_SUB_SEL_FIND_SESSION_TO_DEACTIVATE

DESCRIPTION
  This function searches for any prov session that needs to be deactivated
  based on the requested mapping.

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  Session to deactivate was found
  FALSE: No session needs to be deactivated

SIDE EFFECTS
  None
===========================================================================*/
static boolean mmgsdi_sub_sel_find_session_to_deactivate(
  mmgsdi_sessions_to_slots_mapping_type    sessions_to_slots_mapping,
  mmgsdi_session_id_type                 * session_id_ptr
)
{
  uint8                      i                = 0;
  mmgsdi_return_enum_type    mmgsdi_status    = MMGSDI_ERROR;
  mmgsdi_channel_info_type * channel_info_ptr = NULL;
  mmgsdi_slot_id_enum_type   req_mapping[] = {
                               sessions_to_slots_mapping.pri_slot,
                               sessions_to_slots_mapping.pri_slot,
                               sessions_to_slots_mapping.sec_slot,
                               sessions_to_slots_mapping.sec_slot,
                               sessions_to_slots_mapping.ter_slot,
                               sessions_to_slots_mapping.ter_slot};
  mmgsdi_session_id_type     mmgsdi_session_ids[] = {
                               mmgsdi_generic_data_ptr->pri_gw_session_id,
                               mmgsdi_generic_data_ptr->pri_1x_session_id,
                               mmgsdi_generic_data_ptr->sec_gw_session_id,
                               mmgsdi_generic_data_ptr->sec_1x_session_id,
                               mmgsdi_generic_data_ptr->ter_gw_session_id,
                               mmgsdi_generic_data_ptr->ter_1x_session_id};

  if(session_id_ptr == NULL)
  {
    return FALSE;
  }

  for(i = 0;
      (i < sizeof(req_mapping)/sizeof(req_mapping[0])) &&
       (i < sizeof(mmgsdi_session_ids)/sizeof(mmgsdi_session_ids[0]));
      i++)
  {
    channel_info_ptr = NULL;

    mmgsdi_status = mmgsdi_util_get_session_and_channel_info(
                      mmgsdi_session_ids[i],
                      NULL,
                      &channel_info_ptr);

    if(mmgsdi_status == MMGSDI_SUCCESS &&
       channel_info_ptr != NULL &&
       channel_info_ptr->slot_id != req_mapping[i])
    {
      *session_id_ptr = mmgsdi_session_ids[i];
      return TRUE;
    }
  }

  return FALSE;
} /* mmgsdi_sub_sel_find_session_to_deactivate */


/*===========================================================================
FUNCTION:  MMGSDI_SUB_SEL_FIND_SESSION_TO_ACTIVATE

DESCRIPTION
  This function searches for any prov session that needs to be activated
  based on the requested mapping.

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  Session to activate was found
  FALSE: No session needs to be activated

SIDE EFFECTS
  None
===========================================================================*/
static boolean mmgsdi_sub_sel_find_session_to_activate(
  mmgsdi_sessions_to_slots_mapping_type    sessions_to_slots_mapping,
  mmgsdi_session_id_type                 * session_id_ptr,
  mmgsdi_slot_id_enum_type               * slot_ptr,
  mmgsdi_data_type                       * app_data_ptr
)
{
  uint8                      i                = 0;
  mmgsdi_return_enum_type    mmgsdi_status    = MMGSDI_ERROR;
  mmgsdi_channel_info_type * channel_info_ptr = NULL;
  mmgsdi_slot_id_enum_type   req_mapping[] = {
                               sessions_to_slots_mapping.pri_slot,
                               sessions_to_slots_mapping.pri_slot,
                               sessions_to_slots_mapping.sec_slot,
                               sessions_to_slots_mapping.sec_slot,
                               sessions_to_slots_mapping.ter_slot,
                               sessions_to_slots_mapping.ter_slot};
  mmgsdi_session_id_type     mmgsdi_session_ids[] = {
                               mmgsdi_generic_data_ptr->pri_gw_session_id,
                               mmgsdi_generic_data_ptr->pri_1x_session_id,
                               mmgsdi_generic_data_ptr->sec_gw_session_id,
                               mmgsdi_generic_data_ptr->sec_1x_session_id,
                               mmgsdi_generic_data_ptr->ter_gw_session_id,
                               mmgsdi_generic_data_ptr->ter_1x_session_id};
  mmgsdi_nv_context_type    nv_context        = MMGSDI_NV_CONTEXT_INVALID;

  if((session_id_ptr == NULL) || (app_data_ptr == NULL) || (slot_ptr == NULL))
  {
    return FALSE;
  }

  for(i = 0;
      (i < sizeof(req_mapping)/sizeof(req_mapping[0])) &&
       (i < sizeof(mmgsdi_session_ids)/sizeof(mmgsdi_session_ids[0]));
      i++)
  {
    boolean app_found = FALSE;
    channel_info_ptr = NULL;

    mmgsdi_status = mmgsdi_util_get_session_and_channel_info(
                      mmgsdi_session_ids[i],
                      NULL,
                      &channel_info_ptr);

    if(channel_info_ptr != NULL ||
       req_mapping[i] == MMGSDI_SLOT_NONE)
    {
      continue;
    }

    nv_context = mmgsdi_util_get_efs_item_index_for_slot(req_mapping[i]);
    if (nv_context == MMGSDI_NV_CONTEXT_INVALID)
    {
      continue;
    }

    if((i & 1) == 0)
    {
      /* Find GW app on the slot */
      mmgsdi_util_find_first_sim_usim_app(req_mapping[i], &app_found, app_data_ptr);
    }
    else if((mmgsdi_nv_get_slot_feature_status(
               MMGSDI_FEATURE_CDMA_LESS, nv_context) == MMGSDI_FEATURE_DISABLED))
    {
      nv_stat_enum_type         result         = NV_STAT_ENUM_MAX;
      mmgsdi_nv_item_cache_type mmgsdi_nv_data = {0};

      /* Activate CSIM/RUIM app only if the RTRE configuration indicates
         that 1x sub is allowed from the card */
      result = mmgsdi_nv_get_item(NV_RTRE_CONFIG_I, &mmgsdi_nv_data, nv_context);

      if((result == NV_DONE_S) &&
         (mmgsdi_nv_data.item_value.rtre_config != NV_RTRE_CONFIG_NV_ONLY))
      {
        /* Find 1X app on the slot */
        mmgsdi_util_find_first_ruim_csim_app(req_mapping[i], &app_found, app_data_ptr);
      }
    }

    if(app_found)
    {
      *session_id_ptr = mmgsdi_session_ids[i];
      *slot_ptr = req_mapping[i];
      return TRUE;
    }
  }

  return FALSE;
} /* mmgsdi_sub_sel_find_session_to_activate */


/*===========================================================================
FUNCTION:  MMGSDI_SUB_SEL_MAP_SESSIONS_TO_SLOTS_CB

DESCRIPTION
  This function handles the confirmation for MMGSDI_MAP_SESSIONS_TO_SLOTS_REQ.

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
static void mmgsdi_sub_sel_map_sessions_to_slots_cb(
  mmgsdi_return_enum_type       cnf_status,
  mmgsdi_cnf_enum_type          cnf_type,
  const mmgsdi_cnf_type       * cnf_ptr
)
{
  mmgsdi_return_enum_type                 mmgsdi_status   = MMGSDI_SUCCESS;
  mmgsdi_session_id_type                  session_id      = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_slot_id_enum_type                slot_id         = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_data_type                        app_data        = {0};
  mmgsdi_sessions_to_slots_req_info_type *client_data_ptr = NULL;

  if(cnf_ptr == NULL ||
     (cnf_type != MMGSDI_SESSION_DEACTIVATE_REQ &&
      cnf_type != MMGSDI_SESSION_ACT_OR_SWITCH_PROV_REQ) ||
     cnf_ptr->response_header.client_data == NULL)
  {
    return;
  }

  client_data_ptr = (mmgsdi_sessions_to_slots_req_info_type *)
                       cnf_ptr->response_header.client_data;

  if(cnf_status != MMGSDI_SUCCESS)
  {
    /* In case of error during (de)activation of a session, stop processing
       remaining sessions and return error CNF to the client. */
    mmgsdi_sub_sel_map_sessions_to_slots_send_cnf(cnf_status,
                                                  client_data_ptr);
    MMGSDIUTIL_TMC_MEM_FREE(client_data_ptr);
    return;
  }

  if(mmgsdi_sub_sel_find_session_to_deactivate(client_data_ptr->requested_mapping,
                                               &session_id))
  {
    mmgsdi_status = mmgsdi_session_deactivate_provisioning(
                      session_id,
                      mmgsdi_sub_sel_map_sessions_to_slots_cb,
                      (mmgsdi_client_data_type)client_data_ptr);
    if(mmgsdi_status == MMGSDI_SUCCESS)
    {
      return;
    }
  }
  else if(mmgsdi_sub_sel_find_session_to_activate(client_data_ptr->requested_mapping,
                                                  &session_id,
                                                  &slot_id,
                                                  &app_data))
  {
     mmgsdi_status = mmgsdi_session_activate_or_switch_provisioning(
                       session_id,
                       slot_id,
                       app_data,
                       FALSE,
                       mmgsdi_sub_sel_map_sessions_to_slots_cb,
                       (mmgsdi_client_data_type)client_data_ptr);
    if(mmgsdi_status == MMGSDI_SUCCESS)
    {
      return;
    }
  }

  mmgsdi_sub_sel_map_sessions_to_slots_send_cnf(mmgsdi_status,
                                                client_data_ptr);
  MMGSDIUTIL_TMC_MEM_FREE(client_data_ptr);
} /* mmgsdi_sub_sel_map_sessions_to_slots_cb */


/*===========================================================================
FUNCTION:  MMGSDI_SUB_SEL_PROCESS_MAP_SESSIONS_TO_SLOTS_REQ

DESCRIPTION
  This function processes the MMGSDI_MAP_SESSIONS_TO_SLOTS_REQ.

DEPENDENCIES
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_sub_sel_process_map_sessions_to_slots_req(
  const mmgsdi_map_sessions_to_slots_req_type * sessions_to_slots_req_ptr
)
{
  mmgsdi_session_id_type                  session_id      = MMGSDI_INVALID_SESSION_ID;
  mmgsdi_slot_id_enum_type                slot_id         = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_data_type                        app_data        = {0};
  mmgsdi_sw_status_type                   status_word     = {0};
  mmgsdi_sessions_to_slots_req_info_type *client_data_ptr = NULL;

  MMGSDIUTIL_RETURN_IF_NULL(sessions_to_slots_req_ptr);

  if(mmgsdi_sessions_to_slots_mapping.in_progress)
  {
    /* If a mapping request is already in progress, return MMGSDI_MAPPING_IN_PROGRESS to client */
    return mmgsdi_cnf_build_and_queue(MMGSDI_MAPPING_IN_PROGRESS,
                                      &sessions_to_slots_req_ptr->request_header,
                                      NULL,
                                      NULL,
                                      FALSE,
                                      status_word);
  }

  if(mmgsdi_sessions_to_slots_mapping.is_mapping_valid &&
     (memcmp(&sessions_to_slots_req_ptr->mapping,
             &mmgsdi_sessions_to_slots_mapping.mapping,
             sizeof(mmgsdi_sessions_to_slots_mapping_type)) == 0))
  {
    /* If the stored mapping matches the requested mapping return success to
       client */
    return mmgsdi_cnf_build_and_queue(MMGSDI_SUCCESS,
                                      &sessions_to_slots_req_ptr->request_header,
                                      NULL,
                                      NULL,
                                      FALSE,
                                      status_word);
  }

  /* Mark mapping in progress to TRUE */
  mmgsdi_sessions_to_slots_mapping.in_progress = TRUE;

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(client_data_ptr,
                                     sizeof(mmgsdi_sessions_to_slots_req_info_type));
  if(client_data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  client_data_ptr->requested_mapping = sessions_to_slots_req_ptr->mapping;
  client_data_ptr->client_id = sessions_to_slots_req_ptr->request_header.client_id;
  client_data_ptr->client_data = sessions_to_slots_req_ptr->request_header.client_data;
  client_data_ptr->response_cb_ptr = sessions_to_slots_req_ptr->request_header.response_cb;

  if(mmgsdi_sub_sel_find_session_to_deactivate(sessions_to_slots_req_ptr->mapping,
                                               &session_id))
  {
    return mmgsdi_session_deactivate_provisioning(session_id,
                                                  mmgsdi_sub_sel_map_sessions_to_slots_cb,
                                                  (mmgsdi_client_data_type)client_data_ptr);
  }
  else if(mmgsdi_sub_sel_find_session_to_activate(sessions_to_slots_req_ptr->mapping,
                                                  &session_id,
                                                  &slot_id,
                                                  &app_data))
  {
    return mmgsdi_session_activate_or_switch_provisioning(session_id,
                                                          slot_id,
                                                          app_data,
                                                          FALSE,
                                                          mmgsdi_sub_sel_map_sessions_to_slots_cb,
                                                          (mmgsdi_client_data_type)client_data_ptr);
  }

  MMGSDIUTIL_TMC_MEM_FREE(client_data_ptr);

  /* Copy the mapping to the global before returning SUCCESS */
  mmgsdi_sessions_to_slots_mapping.mapping = sessions_to_slots_req_ptr->mapping;
  mmgsdi_sessions_to_slots_mapping.is_mapping_valid = TRUE;

  mmgsdi_sessions_to_slots_mapping.in_progress = FALSE;
  return mmgsdi_cnf_build_and_queue(MMGSDI_SUCCESS,
                                    &sessions_to_slots_req_ptr->request_header,
                                    NULL,
                                    NULL,
                                    FALSE,
                                    status_word);
} /* mmgsdi_sub_sel_process_map_sessions_to_slots_req */

