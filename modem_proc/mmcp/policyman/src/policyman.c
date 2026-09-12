/**
  @file policyman.c

  @brief Public interface to Policy Manager
*/

/*
    Copyright (c) 2013-2018 QUALCOMM Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Technologies Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this
  document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "comdef.h"
#include "mre_efs.h"
#include "mre_engine.h"
#include "policyman_cfgitem.h"
#include "policyman_dbg.h"
#include "policyman_device_config.h"
#include "policyman_diag.h"
#include "policyman_efs.h"
#include "policyman_fullrat_config.h"
#include "policyman_i.h"
#include "policyman_phone_events.h"
#include "policyman_plmn.h"
#include "policyman_policies.h"
#include "policyman_policy.h"
#include "policyman_serving_system.h"
#include "policyman_rf.h"
#include "policyman_rf_bandset_i.h"
#include "policyman_timer.h"
#include "policyman_task.h"

/*------------------------------------------------------------------------
  FUNCTION Prototypes
--------------------------------------------------------------------------*/
void policyman_register_for_cm_events(void);


/*  Boolean used by policyman_test_mode_is_enabled() to know if it has determined
 *  whether policyman is in test mode.  This is a static outside the function
 *  because it must be reset each time policyman starts in unit tests.
 */
static boolean  test_mode_read;
static boolean  in_test_mode;
static boolean  in_FTM;


/*------------------------------------------------------------------------
  Sandbox Global
--------------------------------------------------------------------------*/
STATIC  policyman_sandbox_t   *g_pGlobalSandbox;


/*-------- ftm_mode_enabled --------*/
static boolean ftm_mode_enabled(
  void
)
{
  nv_stat_enum_type result;
  nv_item_type      data;

  result = mre_efs_get_nv_item(NV_FTM_MODE_I, &data, 0);

  return  ((result == NV_DONE_S) && (data.ftm_mode != 0));
}


/*-------- policyman_test_mode_is_enabled --------*/
/**
@brief Check if test_mode is enabled.
*/
boolean policyman_test_mode_is_enabled(
  void
)
{
  if (!test_mode_read)
  {
    test_mode_read = TRUE;
    in_test_mode   = mre_efs_file_exists( POLICYMAN_TEST_MODE_EFS_PATH,
                                          SYS_MODEM_AS_ID_NONE );
  }

  return in_test_mode;
}


/*-------- policyman_any_test_mode_is_enabled --------*/
/**
@brief Check if test_mode or FTM is enabled.
*/
boolean policyman_any_test_mode_is_enabled(
  void
)
{
  return policyman_test_mode_is_enabled() || policyman_FTM_is_enabled();
}


/*-------- policyman_FTM_is_enabled --------*/
/**
@brief  Is policyman in FTM.
*/
boolean policyman_FTM_is_enabled(
  void
)
{
  static boolean ftm_read;

  if (!ftm_read)
  {
    ftm_read = TRUE;
    in_FTM   = ftm_mode_enabled();
  }

  return in_FTM;
}


/*-------- policyman_set_test_mode --------*/
/**
@brief  Set test mode to given value
*/
void policyman_set_test_mode(
  boolean value
)
{
  in_test_mode = value;
  test_mode_read = value;
}

/*-------- policyman_set_FTM --------*/
/**
@brief  Set static in_FTM to given value
*/
void policyman_set_FTM(
  boolean value
)
{
  in_FTM = value;
}


/*-------- policyman_sandbox_dtor --------*/
STATIC void policyman_sandbox_dtor(
  void *pObj
)
{
  policyman_sandbox_t *pSandbox = (policyman_sandbox_t *)pObj;

  MRE_RELEASE_IF(pSandbox->pNamespaceInfo);
  MRE_RELEASE_IF(pSandbox->pPolicySet);
  MRE_RELEASE_IF(pSandbox->pCurrentDB);
  MRE_RELEASE_IF(pSandbox->pSuspendClientSet);
  MRE_MEM_FREE_IF(pSandbox);
}


/*-------- policyman_sandbox_clone_dtor --------*/
STATIC void policyman_sandbox_clone_dtor(
  void *pObj
)
{
  policyman_sandbox_t *pSandbox = (policyman_sandbox_t *)pObj;

  policyman_state_delete_clone(pSandbox->pState);

  policyman_sandbox_dtor(pObj);
}


/*-------- policyman_get_global_sandbox --------*/
policyman_sandbox_t * policyman_get_global_sandbox(
  void
)
{
  return g_pGlobalSandbox;
}


/*-------- policyman_create_global_sandbox --------*/
void policyman_create_global_sandbox(
  void
)
{
  g_pGlobalSandbox = (policyman_sandbox_t *)policyman_mem_alloc(sizeof(policyman_sandbox_t));

  ref_cnt_obj_init(g_pGlobalSandbox, policyman_sandbox_dtor);

  g_pGlobalSandbox->pPolicySet = policyman_get_global_policy_set();
  ref_cnt_obj_add_ref(g_pGlobalSandbox->pPolicySet);

  g_pGlobalSandbox->pState = policyman_state_get_state();

  g_pGlobalSandbox->pNamespaceInfo = policyman_get_namespace_info();
  ref_cnt_obj_add_ref(g_pGlobalSandbox->pNamespaceInfo);

  g_pGlobalSandbox->pCurrentDB = NULL;

  g_pGlobalSandbox->pSuspendClientSet = policyman_get_suspend_client_set();
  ref_cnt_obj_add_ref(g_pGlobalSandbox->pSuspendClientSet);
}


/*-------- policyman_global_sandbox_refresh_policies --------*/
void policyman_global_sandbox_refresh_policies(
  void
)
{
  MRE_RELEASE_IF(g_pGlobalSandbox->pPolicySet);
  MRE_RELEASE_IF(g_pGlobalSandbox->pNamespaceInfo);

  g_pGlobalSandbox->pPolicySet = policyman_get_global_policy_set();
  ref_cnt_obj_add_ref(g_pGlobalSandbox->pPolicySet);

  g_pGlobalSandbox->pNamespaceInfo = policyman_get_namespace_info();
  ref_cnt_obj_add_ref(g_pGlobalSandbox->pNamespaceInfo);

}


/*-------- policyman_is_global_sandbox_suspended --------*/
boolean policyman_is_global_sandbox_suspended(
  void
)
{
  return policyman_is_suspended(policyman_get_global_sandbox());
}


/*-------- policyman_create_sandbox --------*/
policyman_sandbox_t * policyman_create_sandbox(
  void
)
{
  policyman_sandbox_t  *pSandbox;
  mre_set_t            *pPolicySet;
  mre_namespace_info_t *pNamespaceInfo;
  mre_set_t            *pCurrentDb;

  pSandbox = (policyman_sandbox_t *)policyman_mem_alloc(sizeof(policyman_sandbox_t));

  ref_cnt_obj_init(pSandbox, policyman_sandbox_clone_dtor);

  pNamespaceInfo = policyman_namespace_clone(policyman_get_namespace_info());

  pSandbox->pNamespaceInfo    = pNamespaceInfo;
  pSandbox->pPolicySet        = policyman_clone_global_policy_set(pNamespaceInfo);
  pSandbox->pState            = policyman_state_clone();

  pCurrentDb =  policyman_get_cfgitem_db();

  pSandbox->pCurrentDB = policyman_db_clone(pCurrentDb);

  MRE_RELEASE_IF(pCurrentDb);

  pSandbox->pSuspendClientSet = NULL;

  return pSandbox;
}


/*-------- policyman_init --------*/
/**
@brief  Initialize Policy Manager.  Private function to be used by PM task code.

@return
  None
*/
void policyman_init(
  void
)
{
  policyman_state_t  *pState;
  mre_set_t          *pItemSet;
  size_t              subsIndex;
  policyman_status_t  status;
  boolean             globalPolicyWasRead;

  pItemSet = policyman_itemset_new();

  policyman_namespace_info_init();
  policyman_state_init();
  policyman_rf_bandset_init();
  policyman_rf_init();
  policyman_state_static_init();

  pState = policyman_state_get_state();

  policyman_cfgitem_init(pState, pItemSet);

  // Device level initialization
  policyman_policies_init();

  if ( !policyman_cfgitem_limited_capabilities_is_enabled() )
  {
    status = policyman_policy_load_global(pState, pItemSet);

    //global policy read is a success if file not present
    globalPolicyWasRead = (   status == POLICYMAN_STATUS_SUCCESS 
                           || status == POLICYMAN_STATUS_ERR_NOT_PRESENT );

    // Subs level initializaton
    if (globalPolicyWasRead)
    {
      policyman_sub_policies_init(pState, pItemSet);
      policyman_policy_load_post(pState, pItemSet);
    }
  }

  // ensure configs here to allow limited capibilities routine to kick in
  policyman_ensure_configurations(pItemSet);

  policyman_ensure_items_in_itemset(pState, pItemSet);

  policyman_cfgitem_update_items(pState, pItemSet, NULL);
  
  MRE_RELEASE_IF(pItemSet);

  policyman_policy_init_is_complete();
  policyman_efs_initialize();

  policyman_create_global_sandbox();

  policyman_send_all_config_items();

  POLICYMAN_MSG_HIGH_0("---------- Initial database after initialization");
  policyman_cfgitem_display_all();
}

#if defined(TEST_FRAMEWORK)
#error code not present
#endif /* +TEST_FRAMEWORK. */

/*-------- policyman_per_subs_deinit --------*/
/**
@brief  Deinitialize specific subscription state, as well as overall configuration for
           Policy Manager.  Private function to be used by PM task code.

@return
  None
*/
void policyman_per_subs_deinit(
  size_t subs
)
{
  policyman_fullrat_config_deinit_per_subs(subs);
  if (!policyman_any_test_mode_is_enabled())
  {
    policyman_policies_per_subs_deinit(subs);
  }

  policyman_cfgitem_deinit_per_subs(subs);
}


/*-------- policyman_log_item_request --------*/
void policyman_log_item_request(
  policyman_diag_client_request_type_t  requestType,
  size_t                                numRequestedIds,
  policyman_item_id_t const            *pRequestedIdList,
  size_t                                sequenceNumber,
  size_t                                numReturnedItems,
  policyman_item_t           const    **ppReturnedItems
)
{
  if (policyman_caller_is_client())
  {
    policyman_diag_event_client_request_t  event_pkt;
    size_t                                 idx;

    memset(&event_pkt, 0, sizeof(event_pkt));

    event_pkt.client_id = policyman_get_caller_thread_id();
    event_pkt.request_type = requestType;

    if (POLICYMAN_CLIENT_REQUEST_GET_ITEMS != requestType)
    {
      // GET_ITEMS event doesn't have a sequence number, but the
      // GET_ITEMS_MSIM and GET_ITEMS_SINCE_SEQ types do.
      event_pkt.sequence_number = sequenceNumber;
    }

    if (POLICYMAN_CLIENT_REQUEST_GET_ITEMS_SINCE_SEQ != requestType)
    {
      // GET_ITEMS_SINCE_SEQ event doesn't have a list of requested ids,
      // but GET_ITEMS and GET_ITEMS_MSIM types do.
      event_pkt.num_items_requested = numRequestedIds;

      for (idx = 0; idx < numRequestedIds; idx++)
      {
        event_pkt.items_requested[idx] = pRequestedIdList[idx];
      }
    }

    // Returned item count
    event_pkt.num_item_info = numReturnedItems;

    // Returned item list
    for (idx = 0; idx < event_pkt.num_item_info; idx++)
    {
      policyman_item_t   const *pItem;
      sys_modem_as_id_e_type    itemSubs;

      pItem = ppReturnedItems[idx];

      event_pkt.item_info[idx].seqNo  = pItem->seqno;
      event_pkt.item_info[idx].itemId = policyman_item_get_id_msim(pItem, &itemSubs);
      event_pkt.item_info[idx].subsId = itemSubs;
    }

    // ToDo: activate this with a valid event ID
    /*
    event_report_payload( 0x12345, // EVENT_PM_CLIENT_ITEM_REQUEST
                          sizeof(policyman_diag_event_client_request_t),
                          &event_pkt );
    */
  }
}

/*-------- policyman_get_items --------*/
policyman_status_t policyman_get_items(
  policyman_item_id_t const  *pIds,
  size_t                      nIds,
  policyman_item_t const    **ppItem
)
{
  size_t     numItems;

  if (policyman_is_initializing())
  {
    return POLICYMAN_STATUS_ERR_NOT_READY;
  }

  POLICYMAN_CL_MSG_LOW_2( "Client thread 0x%x requested %d items from policyman_get_items()",
                          POLICYMAN_CALLER_THREAD_ID,
                          nIds );

  numItems = policyman_cfgitem_get_items(pIds, nIds, ppItem);

  policyman_log_item_request( POLICYMAN_CLIENT_REQUEST_GET_ITEMS,
                              nIds,
                              pIds,
                              0,
                              numItems,
                              ppItem );

  if (policyman_is_global_sandbox_suspended())
  {
    return POLICYMAN_STATUS_WRN_SUSPENDED;
  }
  else
  {
    return (numItems == nIds) ? POLICYMAN_STATUS_SUCCESS : POLICYMAN_STATUS_ERR_INVALID_ITEM_ID;
  }
}


/*-------- policyman_get_items_msim --------*/
policyman_status_t policyman_get_items_msim(
  policyman_item_id_t const          *pIds,
  size_t                              nIds,
  policyman_item_collection_t const **ppItems
)
{
  policyman_status_t  status;

  if (policyman_is_initializing())
  {
    return POLICYMAN_STATUS_ERR_NOT_READY;
  }

  POLICYMAN_CL_MSG_LOW_2( "Client thread 0x%x requested %d items from policyman_get_items_msim()",
                          POLICYMAN_CALLER_THREAD_ID,
                          nIds );

  status = policyman_cfgitem_get_items_msim(pIds, nIds, (policyman_item_collection_t **) ppItems);

  policyman_log_item_request( POLICYMAN_CLIENT_REQUEST_GET_ITEMS_MSIM,
                              nIds,
                              pIds,
                              (*ppItems)->seqno,
                              (*ppItems)->numItems,
                              (policyman_item_t const **)(*ppItems)->pItems );

  return policyman_is_global_sandbox_suspended()? POLICYMAN_STATUS_WRN_SUSPENDED : status;
}


/*-------- policyman_get_items_block --------*/
policyman_status_t policyman_get_items_block(
  policyman_item_id_t const  *pIds,
  size_t                      nIds,
  policyman_item_t const    **ppItem
)
{

  policyman_block_client_if_initializing();

  return policyman_get_items(pIds, nIds, ppItem);
}


/*-------- policyman_get_items_block_msim --------*/
policyman_status_t policyman_get_items_block_msim(
  policyman_item_id_t const          *pIds,
  size_t                              nIds,
  policyman_item_collection_t const **ppItems
)
{

  policyman_block_client_if_initializing();

  return policyman_get_items_msim(pIds, nIds, ppItems);
}

/*-------- policyman_get_collection_item --------*/
policyman_item_t *policyman_get_collection_item(
  policyman_item_collection_t const *pCollection,
  policyman_item_id_t                id,
  sys_modem_as_id_e_type             subsId
)
{
  policyman_item_t const *pItem = NULL;
  size_t                  i;

  for (i = 0 ; i < pCollection->numItems ; ++i)
  {
    policyman_item_id_t     thisId;
    sys_modem_as_id_e_type  thisSubs;

    pItem = pCollection->pItems[i];
    thisId = policyman_item_get_id_msim(pItem, &thisSubs);

    if (id == thisId && subsId == thisSubs)
    {
      POLICYMAN_CL_MSG_LOW_2( "Requested item(id %d, subs %d) found in the collection",
                              id,
                              subsId );

      return (policyman_item_t *)pItem;
    }
  }

  POLICYMAN_CL_MSG_ERROR_2( "Requested item(id %d, subs %d) NOT found in the collection",
                            id,
                            subsId );
  return NULL;
}

#if defined(TEST_FRAMEWORK)
#error code not present
#endif /* +TEST_FRAMEWORK. */


/*-------- policyman_item_get_id --------*/
policyman_item_id_t policyman_item_get_id(
  policyman_item_t const  *pItem
)
{
  return (pItem == NULL) ? POLICYMAN_ITEM_INVALID
                         : policyman_cfgitem_id(pItem);
}


/*-------- policyman_item_get_id_msim --------*/
policyman_item_id_t policyman_item_get_id_msim(
  policyman_item_t const  *pItem,
  sys_modem_as_id_e_type  *pSubsId
)
{
  return (pItem == NULL) ? POLICYMAN_ITEM_INVALID
                         : policyman_cfgitem_id_msim(pItem, pSubsId);
}


/*-------- policyman_item_addref --------*/
void policyman_item_addref(
  policyman_item_t const  *pItem
)
{
  if (pItem != NULL)
  {
    policyman_cfgitem_addref(pItem);
  }
}


/*-------- policyman_item_collection_release --------*/
void policyman_item_collection_release(
  policyman_item_collection_t const *pCollection
)
{
  ref_cnt_obj_release((void *) pCollection);
}


/*-------- policyman_item_release --------*/
void policyman_item_release(
  policyman_item_t const  *pItem
)
{
  if (pItem != NULL)
  {
    policyman_cfgitem_release(pItem);
  }
}


/*-------- policyman_msgr_attachments_release --------*/
void policyman_msgr_attachments_release(
  msgr_hdr_s  *pMsg
)
{
  if (pMsg != NULL)
  {
    POLICYMAN_MSG_HIGH_1("Client thread called policyman_msgr_attachments_release(0x%08x)", pMsg);

    policyman_cfgitem_release_msg_attach(pMsg);
  }
}

/*-------- policyman_get_subs_boolean_value --------*/
boolean policyman_get_subs_boolean_value(
  char const             *pBooleanName,
  sys_modem_as_id_e_type  subsId,
  boolean                *pValue
)
{
  mre_namespace_info_t  *pInfo;
  mre_status_t  status;

  if (MRE_MAX_SUBS <= subsId)
  {
    status = MRE_STATUS_ERR_NOT_PRESENT;
    goto Done;
  }

  policyman_enter_namespace_crit_sect();

  pInfo = policyman_get_namespace_info();

  status = mre_boolean_get_value_for_subs( pBooleanName,
                                           pValue,
                                           subsId,
                                           pInfo );

  policyman_leave_namespace_crit_sect();

  if (status == MRE_STATUS_SUCCESS)
  {

    POLICYMAN_MSG_HIGH_2( "policyman_get_subs_boolean_value on subs %d returns true with value %d",
                         subsId, 
                         *pValue);
  }
  else
  {
    POLICYMAN_MSG_HIGH_1("policyman_get_subs_boolean_value on subs %d returns false", subsId);
  }

Done:
  return MRE_STATUS_SUCCESS == status;
}

/*-------- policyman_get_subs_boolean_with_default --------*/
boolean policyman_get_subs_boolean_with_default(
  char const             *pBooleanName,
  sys_modem_as_id_e_type  subsId,
  boolean                 defaultValue
)
{
  boolean value;

  return (TRUE == policyman_get_subs_boolean_value(pBooleanName, subsId, &value))
          ? value
          : defaultValue;
}


typedef struct
{
  POLICYMAN_CMD_HDR;

  mre_boolean_t           *pBoolean;
  boolean                  value;
} policyman_set_boolean_cmd_t;


/*-------- execute_boolean_set --------*/
STATIC void execute_boolean_set(
  policyman_cmd_t *pCmd
)
{
  policyman_set_boolean_cmd_t     *pBoolSetCmd = (policyman_set_boolean_cmd_t *) pCmd;
  mre_status_t                     status;
  policyman_sandbox_t             *pSandbox   = pCmd->pSandbox;

  status = mre_boolean_set_value( pBoolSetCmd->pBoolean,
                                  pBoolSetCmd->value);

  POLICYMAN_MSG_HIGH_2( "execute_boolean set: value %d, status %d",
                        pBoolSetCmd->value,
                        status );

  policyman_state_handle_update(pSandbox);
}


/*-------- policyman_set_subs_boolean_value --------*/
policyman_status_t policyman_set_subs_boolean_value(
  char const             *pBooleanName,
  sys_modem_as_id_e_type  subsId,
  boolean                 value
)
{
  policyman_status_t           status = POLICYMAN_STATUS_ERR_NOT_WRITABLE;
  mre_boolean_t               *pBoolean;
  policyman_set_boolean_cmd_t *pCmd;

  pBoolean = (mre_boolean_t *) mre_named_object_find_with_subs( pBooleanName,
                                                                MRE_NAMED_BOOLEAN,
                                                                subsId,
                                                                policyman_get_namespace_info() );

  if (NON_NULL(pBoolean) && mre_boolean_is_writable(pBoolean))
  {
    status = POLICYMAN_STATUS_SUCCESS;

    pCmd = POLICYMAN_CMD_NEW( policyman_set_boolean_cmd_t,
                              execute_boolean_set,
                              NULL,
                              NULL );

    pCmd->pBoolean  = pBoolean;
    pCmd->value     = value;

    policyman_queue_put_cmd((policyman_cmd_t *) pCmd);
    ref_cnt_obj_release(pCmd);
  }

  POLICYMAN_MSG_HIGH_3( SUBS_PREFIX "policyman_set_subs_boolean_value() returns %d with value %d",
                        subsId,
                        status,
                        value );
  return status;
}



/*-------- policyman_is_srlte_plmn --------*/
STATIC policyman_status_t policyman_is_srlte_plmn(
  sys_plmn_id_s_type    *pPlmnId,
  boolean                checkMccOnly,
  boolean               *pSvdPossible,
  sys_modem_as_id_e_type subsId
)
{
  mre_plmn_mcc_set_t   *pMccSet;
  mre_plmn_mcc_set_t   *pPlmnSet;
  boolean               is_srlte_on_any_plmn;
  mre_namespace_info_t *pInfo;
  policyman_status_t    status = POLICYMAN_STATUS_ERR_NOT_PRESENT;

  policyman_enter_namespace_crit_sect();
  pInfo = policyman_get_namespace_info();

  pMccSet  = (mre_plmn_mcc_set_t *) mre_named_object_find_with_subs( SXLTE_MCCS_STR,
                                                                     POLICYMAN_NAMED_MCC_SET,
                                                                     subsId,
                                                                     pInfo );

  if (NON_NULL(pMccSet))
  {
    ref_cnt_obj_add_ref(pMccSet);
  }

  pPlmnSet = (mre_plmn_mcc_set_t *) mre_named_object_find_with_subs( SXLTE_PLMNS_STR,
                                                                     POLICYMAN_NAMED_PLMN_SET,
                                                                     subsId,
                                                                     pInfo );
  if (NON_NULL(pPlmnSet))
  {
    ref_cnt_obj_add_ref(pPlmnSet);
  }

  policyman_leave_namespace_crit_sect();

  is_srlte_on_any_plmn = ( (NULL != pMccSet) && (NULL == pPlmnSet) );

  // If only MCC check is needed, compare against MCC of the PLMNs in list
  if (checkMccOnly || is_srlte_on_any_plmn)
  {
    if (NON_NULL(pMccSet))
    {
      sys_mcc_type   mcc;

      mcc = policyman_plmn_get_mcc(pPlmnId);

      *pSvdPossible = mre_mcc_list_contains(pMccSet->pPolicy, pMccSet, &mcc);
      status = POLICYMAN_STATUS_SUCCESS;
    }
  }
  else
  {
    if (NON_NULL(pPlmnSet))
    {
      *pSvdPossible = mre_plmn_list_contains(pPlmnSet->pPolicy, pPlmnSet, pPlmnId);
      status = POLICYMAN_STATUS_SUCCESS;
    }
  }

  MRE_RELEASE_IF(pMccSet);
  MRE_RELEASE_IF(pPlmnSet);
  return status;
}


/*-------- policyman_svd_possible_on_plmn --------*/
policyman_status_t policyman_svd_possible_on_plmn(
  sys_plmn_id_s_type  *pPlmnId,
  boolean              mccOnly,
  boolean             *pSvdPossible
)
{
  sys_subs_feature_t     feature = SYS_SUBS_FEATURE_MODE_NORMAL;
  policyman_status_t     retval  = POLICYMAN_STATUS_SUCCESS;

  if (NULL == pPlmnId)
  {
    retval = POLICYMAN_STATUS_ERR_INVALID_ARGS;
    POLICYMAN_MSG_ERROR_0("policyman_svd_possible_on_plmn: invalid arguments");
    goto Done;
  }

  /* Get Device config Policy Item
   */
  {
    policyman_item_t    *pItem;
    retval = policyman_get_current_device_config((policyman_item_t **)&pItem);

    if (retval != POLICYMAN_STATUS_SUCCESS)
    {
     POLICYMAN_MSG_ERROR_0("policyman_svd_possible_on_plmn: internal error");
     goto Done;
    }

    (void)policyman_device_config_get_subs_feature(pItem, 0, SYS_MODEM_AS_ID_1, &feature);
    ref_cnt_obj_release(pItem);
  }

  *pSvdPossible = FALSE;
  POLICYMAN_MSG_PLMN_0("queried PLMN is ", *pPlmnId);

  if (  feature == SYS_SUBS_FEATURE_MODE_SRLTE
     || feature == SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE
     )
  {
    retval = policyman_is_srlte_plmn(pPlmnId, mccOnly, pSvdPossible, SYS_MODEM_AS_ID_1);
  }

Done:

 POLICYMAN_MSG_HIGH_3( "policyman_svd_possible_on_plmn: feature %d, retval %d, svdpossible %d",
                       feature,
                       retval,
                       *pSvdPossible );

  return retval;
}


/*-------- policyman_svd_possible_on_plmn_per_subs --------*/
policyman_status_t policyman_svd_possible_on_plmn_per_subs(
  sys_plmn_id_s_type    *pPlmnId,
  boolean                mccOnly,
  boolean               *pSvdPossible,
  sys_modem_as_id_e_type subsId
)
{
  sys_subs_feature_t     feature = SYS_SUBS_FEATURE_MODE_NORMAL;
  policyman_status_t     retval  = POLICYMAN_STATUS_SUCCESS;

  if (NULL == pPlmnId)
  {
    retval = POLICYMAN_STATUS_ERR_INVALID_ARGS;
    POLICYMAN_MSG_ERROR_0("policyman_svd_possible_on_plmn: invalid arguments");
    goto Done;
  }

  /* Get Device config Policy Item
   */
  {
    policyman_item_t    *pItem;
    retval = policyman_get_current_device_config((policyman_item_t **)&pItem);

    if (retval != POLICYMAN_STATUS_SUCCESS)
    {
     POLICYMAN_MSG_ERROR_0("policyman_svd_possible_on_plmn: internal error");
     goto Done;
    }

    (void)policyman_device_config_get_subs_feature(pItem, 0, subsId, &feature);
    ref_cnt_obj_release(pItem);
  }

  *pSvdPossible = FALSE;
  POLICYMAN_MSG_PLMN_0("queried PLMN is ", *pPlmnId);

  if (  feature == SYS_SUBS_FEATURE_MODE_SRLTE
     || feature == SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE
     )
  {
    retval = policyman_is_srlte_plmn(pPlmnId, mccOnly, pSvdPossible, subsId);
  }

Done:

  POLICYMAN_MSG_HIGH_4( "policyman_svd_possible_on_plmn_per_subs: subs %d, feature %d, retval %d, svdpossible %d",
                        subsId,
                        feature,
                        retval,
                        *pSvdPossible );

  return retval;
}

/*-------- policyman_is_svd_operation_allowed --------*/
boolean policyman_is_svd_operation_allowed(
  sys_modem_as_id_e_type subsId
)
{
  policyman_item_t      *pItem = NULL;
  policyman_status_t    status;
  sys_subs_feature_t    feature;
  boolean               retval = FALSE;

  /* Check range for subsId
   */
  if (subsId < SYS_MODEM_AS_ID_1 || subsId > SYS_MODEM_AS_ID_3)
  {
    return retval;
  }

  /* Get Device config Policy Item
   */
  status = policyman_get_current_device_config((policyman_item_t **)&pItem);

  /* UE is allowed for SVD if subs feature mode is not NORMAL and not SRLTE
   */
  if (status == POLICYMAN_STATUS_SUCCESS)
  {
    switch(subsId)
    {
      case SYS_MODEM_AS_ID_1:

        /* get SUBS feature mode
             */
        status = policyman_device_config_get_subs_feature(pItem, 0, subsId, &feature);
        if (status == POLICYMAN_STATUS_SUCCESS)
        {
          retval = !(   feature == SYS_SUBS_FEATURE_MODE_NORMAL
                     || feature == SYS_SUBS_FEATURE_MODE_SRLTE
                    );
        }
       break;

      /* Any other subscription is NORMAL
          */
      case SYS_MODEM_AS_ID_2:
      case SYS_MODEM_AS_ID_3:
      default:
        break;
    }
  }

  MRE_RELEASE_IF(pItem);

  return retval;
}

