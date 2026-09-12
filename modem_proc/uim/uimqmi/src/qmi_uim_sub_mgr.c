/*===========================================================================

                         Q M I _ U I M _ S U B _ M G R. C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2016-17, 2019 - 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_sub_mgr.c#3 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/08/20    rkg    Revert defensive check added in subscription manager code
02/20/20    rkg    Add new TLV in Suscription manager
09/17/19    mm     Add EID to SimSlotStatus radio config HAL
06/12/19    ykr    Add extended card status to SimSlotStatus
06/06/19    vgd    Free uim_msg_ptr only if it is not present in the queue
04/08/19    ng     Updating slot status check via ATR  
03/28/17    ks     Fixing issue in passing parameter to uimqmi_free
12/05/16    ks     Fixing compilation issue with sub-manager feature
11/04/16    me     Combined multi-line ASSERT in one line
07/26/16    sp     Move qmi_uimi_ functions to corresponding files
06/16/16    vdc    Split QMI UIM parsing files for supporting IOE
05/26/16    vdc    Move internal enums, structs to new internal header file
04/20/16    sam    TSDS feature changes
04/13/16    vdc    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "qmi_uim_sub_mgr.h"
#include "qmi_uim_sub_mgr_parsing.h"
#include "qmi_framework.h"
#include "qmi_svc_utils.h"
#include "uim_msg.h"
#include "amssassert.h"
#include <stringl/stringl.h>
#include "qmi_uim_util.h"
#include "uimsub_manager.h"

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

 #ifdef FEATURE_UIM_DS_SUBSCRIPTION_MANAGER
/*===========================================================================
FUNCTION QMI_UIM_SUB_MGR_IS_PHYSICAL_SLOT_ACTIVE_FOR_LOGICAL_SLOT()

DESCRIPTION
  Figures out if for the logical slot, the physical slot is active or not.

PARAMETERS
  uim_slot_type        : logical slot id
  uim_slot_type        : physical slot id

RETURN VALUE
  None

DEPENDENCIES
  QMI UIM service must be initialized and registered with Framework

SIDE EFFECTS
  None

===========================================================================*/
boolean qmi_uim_sub_mgr_is_physical_slot_active_for_logical_slot
(
  uim_slot_type                   logical_slot_id,
  uim_slot_type                   physical_slot_id
)
{
  uint8 slot_idx = 0;

  ASSERT(qmi_uim_global_ptr != NULL);

  /* Convert slot to index */
  switch (physical_slot_id)
  {
    case UIM_SLOT_1:
      slot_idx = 0;
      break;
    case UIM_SLOT_2:
      slot_idx = 1;
      break;
    case UIM_SLOT_3:
      slot_idx = 2;
      break;
    default:
      return FALSE;
  }

  if(slot_idx < qmi_uim_global_ptr->physical_slots_info.num_slots &&
     qmi_uim_global_ptr->physical_slots_info.slot_status_ptr != NULL &&
     qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[slot_idx].logical_slot == logical_slot_id &&
     qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[slot_idx].activity_status == UIM_SLOT_STATE_ACTIVE)
  {
    return TRUE;
  }

  return FALSE;
} /* qmi_uim_sub_mgr_is_physical_slot_active_for_logical_slot() */


/*===========================================================================
FUNCTION QMI_UIM_SUB_MGR_NOTIFY_SLOTS_STATUS_CB()

DESCRIPTION
  A callback called by uimdrv when an association of a logical slot to
  a physical slot changes or whenever there is a change to the card state
  in any physical slot.

PARAMETERS
  uint8                   : number of physical slots
  uim_phy_slot_status_type: status of the physical slots

RETURN VALUE
  None

DEPENDENCIES
  QMI UIM service must be initialized and registered with Framework

SIDE EFFECTS
  None

===========================================================================*/
void qmi_uim_sub_mgr_notify_slots_status_cb
(
  uint8                           num_physical_slots,
  const uim_phy_slot_status_type *physical_slot_status_ptr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  if (num_physical_slots == 0 ||
      physical_slot_status_ptr == NULL)
  {
    UIM_MSG_ERR_0("Invalid physical slots info reported by drivers");
    return;
  }

  msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_PHYSICAL_SLOTS_STATUS);
  if (msg_ptr == NULL)
  {
    return;
  }

  /* Copy the physical slots info into the qmiuim message */
  msg_ptr->data.physical_slots_info.num_slots = num_physical_slots;
  uimqmi_deep_copy(
       (void**)&msg_ptr->data.physical_slots_info.slot_status_ptr,
       (void*)physical_slot_status_ptr,
       sizeof(uim_phy_slot_status_type) * num_physical_slots);

  /* Send command */
  if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
     (present_in_queue))
  {
    uimqmi_free(msg_ptr->data.physical_slots_info.slot_status_ptr);
    uimqmi_free(msg_ptr);
  }
  msg_ptr = NULL;
} /* qmi_uim_sub_mgr_notify_slots_status_cb() */
#endif /* FEATURE_UIM_DS_SUBSCRIPTION_MANAGER */


/*===========================================================================
  FUNCTION QMI_UIM_SUB_MGR_SEND_SLOTS_STATUS_INDICATION()

  DESCRIPTION
    This function sends a slots status indication to a specific client

  PARAMETERS
    clid: ClientID

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_sub_mgr_send_slots_status_indication
(
  uint8                 clid
)
{
  dsm_item_type* indication = NULL;

  ASSERT(clid != QMI_SVC_CLID_UNUSED && qmi_uim_global_ptr);

  /* Insert slots status item in TLV */
  if (FALSE == qmi_uim_sub_mgr_response_physical_slots_status(
                 UIMI_TLV_RESP_TAG_OPTIONAL_1,
                 &indication,
                 qmi_uim_global_ptr->physical_slots_info.slot_status_ptr,
                 qmi_uim_global_ptr->physical_slots_info.num_slots))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }
  
   /* Insert slots status item in TLV */
  (void)qmi_uim_sub_mgr_response_physical_slots_information(
                 UIMI_TLV_RESP_TAG_OPTIONAL_2,
                 &indication,
                 qmi_uim_global_ptr->physical_slots_info.slot_status_ptr,
                 qmi_uim_global_ptr->physical_slots_info.num_slots);
				 
  /* Insert Extended card state in TLV */
  (void)qmi_uim_sub_mgr_response_extended_card_state(
                 UIMI_TLV_RESP_TAG_OPTIONAL_4,
                 &indication,
                 qmi_uim_global_ptr->physical_slots_info.slot_status_ptr,
                 qmi_uim_global_ptr->physical_slots_info.num_slots);
				 
  /* Insert EID info in TLV */				 
  (void)qmi_uim_sub_mgr_response_eid_information(
                 UIMI_TLV_RESP_TAG_OPTIONAL_3,
                 &indication,
                 qmi_uim_global_ptr->physical_slots_info.slot_status_ptr,
                 qmi_uim_global_ptr->physical_slots_info.num_slots);

  /* Send indication to the client */
  (void)uimqmi_send_indication(clid,
                               (uint16)UIMI_CMD_VAL_SLOTS_STATUS_IND,
                               indication,
                               QMUX_SERVICE_UIM);
} /* qmi_uim_sub_mgr_send_slots_status_indication */


/*===========================================================================
FUNCTION QMI_UIM_SUB_MGR_STORE_PHYSICAL_SLOTS_STATUS()

DESCRIPTION
  Copies the physical slots status from the passed-in parameter to the
  QMI UIM global for physical slots status.

PARAMETERS
  uint8                   : number of physical slots supported
  uim_phy_slot_status_type: status of the physical slots

RETURN VALUE
  TRUE                    : Global updated with latest physical slots info
  FALSE                   : Otherwise

DEPENDENCIES
  QMI UIM service must be initialized and registered with Framework

SIDE EFFECTS
  None

===========================================================================*/
static boolean qmi_uim_sub_mgr_store_physical_slots_status
(
  uint8                           num_physical_slots,
  const uim_phy_slot_status_type *physical_slot_status_ptr
)
{
  uint8         index           = 0;
  boolean       is_info_updated = FALSE;

  ASSERT(qmi_uim_global_ptr != NULL);

  /* If number of slots QMI UIM received by calling get_num_physical_slots
     during power up is not same as number of physical slots mentioned in
     the uimdrv callback here, there's a serious problem */
  if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr == NULL ||
     qmi_uim_global_ptr->physical_slots_info.num_slots == 0 ||
     physical_slot_status_ptr == NULL ||
     num_physical_slots != qmi_uim_global_ptr->physical_slots_info.num_slots)
  {
    UIM_MSG_ERR_0("qmi_uim_sub_mgr_store_physical_slots_status: slots validation error");
    return FALSE;
  }

  /* Overwrite the qmi uim global ptr for physical slots status info with
     latest info */
  for(index = 0; index < qmi_uim_global_ptr->physical_slots_info.num_slots && index < UIM_MAX_NUMBER_INSTANCES; index ++)
  {
    /* Do not update the card hotswap status if it has not changed since last
       time we updated it (to CARD_INSERTED or CARD_REMOVED) */
    if(physical_slot_status_ptr[index].card_presence_status != UIM_CARD_UNCHANGED &&
       qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].card_presence_status !=
         physical_slot_status_ptr[index].card_presence_status)
    {
      qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].card_presence_status =
        physical_slot_status_ptr[index].card_presence_status;
      is_info_updated = TRUE;
    }
    if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].activity_status !=
         physical_slot_status_ptr[index].activity_status)
    {
      qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].activity_status =
        physical_slot_status_ptr[index].activity_status;
      is_info_updated = TRUE;
    }
    if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].logical_slot !=
         physical_slot_status_ptr[index].logical_slot)
    {
      qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].logical_slot =
        physical_slot_status_ptr[index].logical_slot;
      is_info_updated = TRUE;
    }
    if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].uim_protocol !=
         physical_slot_status_ptr[index].uim_protocol)
    {
      qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].uim_protocol =
        physical_slot_status_ptr[index].uim_protocol;
      is_info_updated = TRUE;
    }   
    if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].num_apps !=
         physical_slot_status_ptr[index].num_apps)
    {
      qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].num_apps =
        physical_slot_status_ptr[index].num_apps;
      is_info_updated = TRUE;
    }    
    if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].is_euicc !=
         physical_slot_status_ptr[index].is_euicc)
    {
      qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].is_euicc =
        physical_slot_status_ptr[index].is_euicc;
      is_info_updated = TRUE;
    }
    if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].atr_len !=
         physical_slot_status_ptr[index].atr_len ||
       ((physical_slot_status_ptr[index].atr_len > 0) &&
         (memcmp(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].atr,
              physical_slot_status_ptr[index].atr,
              physical_slot_status_ptr[index].atr_len) != 0)))
    {
      qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].atr_len =
          physical_slot_status_ptr[index].atr_len;

      if(physical_slot_status_ptr[index].atr_len > 0 && (physical_slot_status_ptr[index].atr_len <= UIM_MAX_ATR_CHARS))
      {
        (void)memscpy(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].atr,
                      sizeof(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].atr),
                      physical_slot_status_ptr[index].atr,
                      physical_slot_status_ptr[index].atr_len);
      }
      
      is_info_updated = TRUE;
    }    
    if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].iccid_len !=
         physical_slot_status_ptr[index].iccid_len ||
       ((physical_slot_status_ptr[index].iccid_len > 0) &&
        (memcmp(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].iccid,
              physical_slot_status_ptr[index].iccid,
              physical_slot_status_ptr[index].iccid_len) != 0)))
    {
      qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].iccid_len =
          physical_slot_status_ptr[index].iccid_len;

      if(physical_slot_status_ptr[index].iccid_len > 0)
      {          
      (void)memscpy(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].iccid,
                    sizeof(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].iccid),
                    physical_slot_status_ptr[index].iccid,
                    physical_slot_status_ptr[index].iccid_len);
      }

      is_info_updated = TRUE;
    }
    if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].uim_status !=
         physical_slot_status_ptr[index].uim_status)
    {
      qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].uim_status =
        physical_slot_status_ptr[index].uim_status;
      is_info_updated = TRUE;
    }
	if(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].eid_len !=
         physical_slot_status_ptr[index].eid_len ||
       ((physical_slot_status_ptr[index].eid_len > 0) &&
        (memcmp(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].eid,
              physical_slot_status_ptr[index].eid,
              physical_slot_status_ptr[index].eid_len) != 0)))
    {
      qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].eid_len =
          physical_slot_status_ptr[index].eid_len;

      if(physical_slot_status_ptr[index].eid_len > 0)
      {          
        (void)memscpy(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].eid,
                      sizeof(qmi_uim_global_ptr->physical_slots_info.slot_status_ptr[index].eid),
                      physical_slot_status_ptr[index].eid,
                      physical_slot_status_ptr[index].eid_len);
      }

      is_info_updated = TRUE;
    }
  }

  return is_info_updated;
} /* qmi_uim_sub_mgr_store_physical_slots_status() */


/*===========================================================================
  FUNCTION QMI_UIM_SUB_MGR_SEND_SLOTS_STATUS_INDICATION_TO_ALL()

  DESCRIPTION
    This function sends a slots status indication to all QMI_UIM clients

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_sub_mgr_send_slots_status_indication_to_all
(
  void
)
{
  uint8                       j                       = 0;
  qmi_uimi_client_state_type* cl_sp                   = NULL;

  if(qmi_uim_state_ptr == NULL)
  {
    return;
  }

  for (j = 0; j < UIMQMI_SVC_MAX_CLIDS; j++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[j];
    if (cl_sp != NULL &&
        cl_sp->common.clid != QMI_SVC_CLID_UNUSED &&
        cl_sp->uim_info.reg_event_type.physical_slots_status)
    {
      /* Send indication to specific client */
      qmi_uim_sub_mgr_send_slots_status_indication(cl_sp->common.clid);
    }
  }
} /* qmi_uim_sub_mgr_send_slots_status_indication_to_all */


/*===========================================================================
  FUNCTION QMI_UIM_SUB_PROCESS_PHYSICAL_SLOTS_STATUS_MSG()

  DESCRIPTION
    This function handles message from the QMI UIM queue for physical slots
    status from drivers

  PARAMETERS
    uim_message_ptr: Pointer to QMI UIM message from message queue

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_sub_mgr_process_physical_slots_status_msg
(
  qmi_uim_message_type * uim_message_ptr
)
{
  boolean is_info_updated = FALSE;

  ASSERT(uim_message_ptr);

  UIM_MSG_HIGH_1("Processing physical slots status CB from drivers; num physical slots: 0x%x",
                 uim_message_ptr->data.physical_slots_info.num_slots);

  is_info_updated = qmi_uim_sub_mgr_store_physical_slots_status(
                        uim_message_ptr->data.physical_slots_info.num_slots,
                        uim_message_ptr->data.physical_slots_info.slot_status_ptr);

  /* Free the physical slots info in the message */
  if(uim_message_ptr->data.physical_slots_info.slot_status_ptr != NULL)
  {
    uimqmi_free(uim_message_ptr->data.physical_slots_info.slot_status_ptr);
    uim_message_ptr->data.physical_slots_info.slot_status_ptr = NULL;
  }

  if(is_info_updated != FALSE)
  {
    /* Send slots status indication */
    qmi_uim_sub_mgr_send_slots_status_indication_to_all();
  }
} /* qmi_uim_sub_mgr_process_physical_slots_status_msg */


/*===========================================================================
  FUNCTION QMI_UIMI_SWITCH_SLOT()

  DESCRIPTION
    Switches the physical slot tied to current logical slot such that the
    current logical slot now maps to the physical slot mentioned in the request

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type* qmi_uimi_switch_slot
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *             response        = NULL;
  qmi_error_e_type            errval          = QMI_ERR_INTERNAL;
  boolean                     retval          = TRUE;
  qmi_uim_tlv_item_type       tlv_data[2];
  uim_slot_type               uim_log_slot    = UIM_SLOT_NONE;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_PHYSICAL_SLOT;
  tlv_data[1].tag       = UIMI_TLV_TAG_MANDATORY_2;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if(qmi_uim_util_mmgsdi_slot_id_to_uim_slot_id(tlv_data[0].data.slot.slot,
                                                &uim_log_slot) != QMI_ERR_NONE)
  {
    errval = QMI_ERR_NOT_SUPPORTED;
    goto send_result;
  }

#ifdef FEATURE_UIM_DS_SUBSCRIPTION_MANAGER
  /* If the physical slot that is requested to be switched to or to be
     activated is already active, return */
  if(TRUE == qmi_uim_sub_mgr_is_physical_slot_active_for_logical_slot(uim_log_slot,
                                                                      tlv_data[1].data.physical_slot.slot))
  {
    errval = QMI_ERR_NO_EFFECT;
    goto send_result;
  }

  /* Execute UIMDRV function to switch the slot */
  if(UIM_PASS != uim_sm_switch_slot(uim_log_slot,
                                    tlv_data[1].data.physical_slot.slot))
  {
    errval = QMI_ERR_INTERNAL;
  }
#else
  errval = QMI_ERR_NOT_SUPPORTED;
  goto send_result;
#endif /* FEATURE_UIM_DS_SUBSCRIPTION_MANAGER */

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  retval = qmi_svc_put_result_tlv(
             &response,
             (errval != QMI_ERR_NONE) ? QMI_RESULT_FAILURE : QMI_RESULT_SUCCESS,
             errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_switch_slot() */


/*===========================================================================
  FUNCTION qmi_uimi_get_slots_status()

  DESCRIPTION
    Get current physical slots status

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    QMI UIM service must be initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_get_slots_status
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type           errval                   = QMI_ERR_NONE;
  dsm_item_type *            response                 = NULL;
  boolean                    retval                   = TRUE;
  qmi_result_e_type          result                   = QMI_RESULT_SUCCESS;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

#ifdef FEATURE_UIM_DS_SUBSCRIPTION_MANAGER

  if(qmi_uim_global_ptr->physical_slots_info.num_slots == 0||
     qmi_uim_global_ptr->physical_slots_info.slot_status_ptr == NULL)
  {
    /* If QMI UIM never registered with drivers for slots status or if
       or if the number of physical slots reported by drivers is zero,
       return error to user */
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Insert physical slots status data from the global into the response TLV */
  if (FALSE == qmi_uim_sub_mgr_response_physical_slots_status(UIMI_TLV_RESP_TAG_OPTIONAL_1,
                                                              &response,
                                                              qmi_uim_global_ptr->physical_slots_info.slot_status_ptr,
                                                              qmi_uim_global_ptr->physical_slots_info.num_slots))
  {
    errval = QMI_ERR_INTERNAL;
    dsm_free_packet(&response);
  }
  
   /* Insert physical slots information data from the global into the response TLV */
  (void)qmi_uim_sub_mgr_response_physical_slots_information(UIMI_TLV_RESP_TAG_OPTIONAL_2,
                                                              &response,
                                                              qmi_uim_global_ptr->physical_slots_info.slot_status_ptr,
                                                              qmi_uim_global_ptr->physical_slots_info.num_slots);
  
  /* Insert Extended card state in TLV */
  (void)qmi_uim_sub_mgr_response_extended_card_state(
                 UIMI_TLV_RESP_TAG_OPTIONAL_4,
                 &response,
                 qmi_uim_global_ptr->physical_slots_info.slot_status_ptr,
                 qmi_uim_global_ptr->physical_slots_info.num_slots);
  /* Insert EID info in TLV */				 
  (void)qmi_uim_sub_mgr_response_eid_information(UIMI_TLV_RESP_TAG_OPTIONAL_3,
                                                 &response,
                                                 qmi_uim_global_ptr->physical_slots_info.slot_status_ptr,
                                                 qmi_uim_global_ptr->physical_slots_info.num_slots);
#else
  errval = QMI_ERR_NOT_SUPPORTED;
#endif /* FEATURE_UIM_DS_SUBSCRIPTION_MANAGER */

#ifdef FEATURE_UIM_DS_SUBSCRIPTION_MANAGER
send_result:
#endif /* FEATURE_UIM_DS_SUBSCRIPTION_MANAGER */
  /* Insert result item in the TLV */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_get_slots_status() */

