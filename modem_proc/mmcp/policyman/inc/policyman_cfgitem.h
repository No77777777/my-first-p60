#ifndef _POLICYMAN_CFGITEM_H_
#define _POLICYMAN_CFGITEM_H_

/**
  @file policyman_cfgitem.h

  @brief Functions to manage PM configuration items.
*/

/*
    Copyright (c) 2013-2018, 2021 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_cfgitem.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "policyman_diag.h"
#include "policyman_i.h"
#include "ref_cnt_obj.h"


#define POLICYMAN_ITEM                  \
  _REF_CNT_OBJ;                         \
  policyman_config_info_t *pCfgInfo;    \
  sys_modem_as_id_e_type   asubs_id;    \
  size_t                   seqno


#define POLICYMAN_CFGITEM_NEW(item_id, item_c_type, p_dtor, subs) \
       (item_c_type *)policyman_cfgitem_new_item( (item_id),           \
                                                  sizeof(item_c_type), \
                                                  (p_dtor),            \
                                                  (subs) )


/**
@brief  The definition of a configuration item.

This is a reference-counted object (see ref_cnt_obj.h).  Rather than copying
the data into buffers, code can call ref_cnt_obj_add_ref() on an item and
simply store the pointer to the item.  It will remain valid until
ref_cnt_obj_release() is called on it.

Member fields are:

@param id         The configuration item id for this configuration item.  The
                  data pointed to by this item is interpreted based on this.
@param data       Pointer to the data for this configuration item.  Interpreted
                  based on the value of the id field.
@param data_size  The size of the buffer pointed to by the data member.
*/
struct policyman_item_t
{
  POLICYMAN_ITEM;
};

typedef enum
{
  POLICYMAN_CFG_ERROR_NONE = 0,

  POLICYMAN_CFG_ERROR_NO_DEVICE_XML,
  POLICYMAN_CFG_ERROR_CARRIER_ON_INVALID_SUB,
  POLICYMAN_CFG_ERROR_POLICY_FEATURE_MISMATCH,
  POLICYMAN_CFG_ERROR_DEVICE_IS_NOT_CALIBRATED,
  POLICYMAN_CFG_ERROR_INVALID_DEVICE_CONFIG
} policyman_cfg_error_cause;

typedef struct policyman_limited_capability_t policyman_limited_capability_t;

/*-------- policyman_cfgitem_enter_crit_section --------*/
void policyman_cfgitem_enter_crit_section(
  void
);


/*-------- policyman_cfgitem_leave_crit_section --------*/
void policyman_cfgitem_leave_crit_section(
  void
);


/*-------- policyman_get_cfgitem_db --------*/
mre_set_t * policyman_get_cfgitem_db(
  void
);


/*-------- policyman_cfgitem_id --------*/
policyman_item_id_t policyman_cfgitem_id(
  policyman_item_t const  *pItem
);


/*-------- policyman_cfgitem_id_msim --------*/
policyman_item_id_t policyman_cfgitem_id_msim(
  policyman_item_t const  *pItem,
  sys_modem_as_id_e_type  *pSubsId
);


/*-------- policyman_cfgitem_addref --------*/
#define policyman_cfgitem_addref(p) ref_cnt_obj_add_ref((policyman_item_t *) (p))


/*-------- policyman_cfgitem_release --------*/
#define policyman_cfgitem_release(p)  ref_cnt_obj_release((policyman_item_t *) (p))



/*-------- policyman_cfgitem_init --------*/
/**
@brief  Initialize the configuration item subsystem.

@return
  None
*/
void policyman_cfgitem_init(
  policyman_state_t *pState,
  mre_set_t         *pItemSet
);


/*-------- policyman_cfgitem_ensure_items_per_subs --------*/
/**
@brief  Ensure that all configuration items of a specified subs have values.

See policyman_cfgitem_ensure_items(). Behavior is the same but restricted
to items related to the provided subs.

@return
  None
*/
void policyman_cfgitem_ensure_items_per_subs(
  policyman_state_t      *pState,
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
);

/*-------- policyman_item_ensure_device_config --------*/
void policyman_item_ensure_device_config(
  policyman_state_t *pState,
  mre_set_t         *pItemSet
);



/*-------- policyman_cfgitem_display_all --------*/
/**
@brief  Display all items in the database.
*/
void policyman_cfgitem_display_all(
  void
);


/*-------- policyman_cfgitem_deinit --------*/
/**
@brief  Deinitialize the configuration items.

@param

@return

*/
void policyman_cfgitem_deinit(
  void
);

/*-------- policyman_cfgitem_deinit_per_subs --------*/
void policyman_cfgitem_deinit_per_subs(
  sys_modem_as_id_e_type   subsId
);

/*-------- policyman_cfgitem_new_item --------*/
/**
@brief  Create a new configuration item.

Create a new configuration item of the given size.  The caller should pass the
size of an item derived from policyman_item_t (i.e. one whose structure starts
with POLICYMAN_ITEM).  The returned object has the ID of the item set and has
a single reference on it.

@param[in]  id           ID of the configuration item to create.
@param[in]  itemSize  Size in bytes of the item.
@param[in]  dtor      Destructor function to call when item's reference
                      count reaches 0.

@return
  Pointer to new configuration item.
*/
policyman_item_t* policyman_cfgitem_new_item(
  policyman_item_id_t      id,
  size_t                   itemSize,
  destructor_ptr           dtor,
  sys_modem_as_id_e_type   subsId
);


/*-------- policyman_cfgitem_is_present --------*/
/**
@brief  Checks whether a given item is present in the database.
*/
boolean policyman_cfgitem_is_present(
  policyman_item_id_t    id,
  sys_modem_as_id_e_type subsId
);

/*-------- policyman_cfgitem_already_present --------*/
/**
@brief  Searches multiple subs to determine if an item is present in the database
*/
boolean policyman_cfgitem_already_present(
  policyman_item_id_t    itemId,
  sys_modem_as_id_e_type policySubs,
  policyman_subs_t       targetSubs
);

/*-------- notification_hook --------*/
typedef msgr_hdr_s *(*notification_hook)(
  size_t             nItems,
  policyman_item_t **ppItems
);

typedef struct
{
  msgr_hdr_s          *pMsg;
  notification_hook    hook;
} client_msg_t;

/*-------- policyman_cfgitem_update_items --------*/
/**
@brief  Update a set of configuration items in an atomic manner.

This function updates the set of configuration items that were passed in an
atomic manner - that is, it updates all items or none of the items.  Any given
item can only be updated if either (1) no policy currently is responsible for
the item or (2) the policy responsible for the item is the policy passed as
a parameter.

@param[in]  pItemSet      A pointer to set of to configuration items  that should 
                                   become the new configuration.
@param[in]  pClientMsg    Address of the client message containing a hook function 
                                   and storage to store new policy

@return
    None.
*/
void policyman_cfgitem_update_items(
  policyman_state_t *pState,
  mre_set_t         *pItemSet,
  client_msg_t      *pClientMsg
);


/*-------- policyman_cfgitem_get_items --------*/
/**
@brief  Get a set of items from the current configuration database.

Note that all items returned have had their reference count incremented.  The
caller should release the items when it is finished with them.

@param[in]  pItemIds  A pointer to an array of item IDs that should be fetched.
@param[in]  numIds    The number of item IDs in the array.
@param[out] ppItems   A pointer to an array of storage for item pointers that
                      are to be returned.  This array must be able to hold
                      at least numIds item pointers.

@return
  The number of items returned in the ppItems array.
*/
size_t  policyman_cfgitem_get_items(
  policyman_item_id_t const  *pItemIds,
  size_t                      numIds,
  policyman_item_t const    **ppItems
);


/*-------- policyman_cfgitem_get_item --------*/
/**
@brief  Internal function to just get a specific configuration item - much
        easier to use than policyman_cfgitem_get_items or
        policyman_cfgitem_get_items_msim.

@param

@return

*/
policyman_item_t  const * policyman_cfgitem_get_item(
  policyman_item_id_t     id,
  sys_modem_as_id_e_type  subs
);



/*-------- policyman_cfgitem_get_items_msim --------*/
/**
@brief Get the configuration items specified by a list of configuration ids for
       all subscriptions.

The items returned have had a reference added to them, so once the caller is
done with the item it should call policyman_item_release() on the pointer to
the items.

@param[in]  pIds    Pointer to an array of item IDs desired.
@param[in]  numIds  Number of IDs pointed to by pIds.
@param[out] ppItems Pointer to storage for a pointer to a
                    policyman_item_collection_t.  The client should call
                    policyman_item_collection_release() in order to release
                    this collection.

@return
  - POLICYMAN_STATUS_SUCCESS        -- Call succeeded.
  - POLICYMAN_STATUS_ERR_INVALID_ID -- The list of IDs contained an invalid ID.
*/
policyman_status_t policyman_cfgitem_get_items_msim(
  policyman_item_id_t const    *pIds,
  size_t                        nIds,
  policyman_item_collection_t **ppItems
);

/*-------- policyman_cfgitem_get_items_per_subs --------*/
/**
@brief  Get a set of items from the current configuration database
          for a given subscription.

Note that all items returned have had their reference count incremented.  The
caller should release the items when it is finished with them.

@param[in]  pItemIds  A pointer to an array of item IDs that should be fetched.
@param[in]  numIds    The number of item IDs in the array.
@param[out] ppItems   A pointer to an array of storage for item pointers that
                      are to be returned.  This array must be able to hold
                      at least numIds item pointers.
@param[in]  subs_id    Subscription for which items are requested.

@return
  The number of items returned in the ppItems array.
*/
size_t policyman_cfgitem_get_items_per_subs(
  policyman_item_id_t const  *pItemIds,
  size_t                      numIds,
  policyman_item_t const    **ppItems,
  sys_modem_as_id_e_type      subs_id
);


/*-------- policyman_cfgitem_release_msg_attach --------*/
/**
@brief

@param

@return

*/
void policyman_cfgitem_release_msg_attach(
  msgr_hdr_s  *pMsg
);


/*-------- policyman_cfgitem_notify_msg_dtor --------*/
/**
@brief  Destroy a message that was passed to MSGR.

@param

@return

*/
void policyman_cfgitem_notify_msg_dtor(
  void  *pObject
);

/*-------- policyman_cfgitem_notify_msg_msim_new --------*/
/**
@brief  Return a pointer to a new MSGR message for an update of the
        items passed.

@param

@return

*/
msgr_hdr_s* policyman_cfgitem_notify_msg_msim_new(
  policyman_item_t  **ppItems,
  size_t              numItems,
  size_t             *pMsgSize
);

/*-------- policyman_cfgitem_update_notification_hook --------*/
/**
@brief  Hook function for notification of the result of subs updates.

This creates another MSGR message (not sent by the MSGR) that is returned 
to the caller that initiated the notification.
*/
msgr_hdr_s* policyman_cfgitem_update_notification_hook(
  size_t              nItems,
  policyman_item_t  **ppItems
);

/*-------- policyman_itemset_new --------*/
/**
@brief

@param

@return

*/
mre_set_t * policyman_itemset_new(
  void
);


/*-------- policyman_ensure_default_item --------*/
void policyman_ensure_default_item(
  sys_modem_as_id_e_type  asubs_id,
  policyman_item_id_t     item_id,
  mre_set_t              *pItemSet
);

/*-------- policyman_cfgitem_is_present_in_itemset --------*/
boolean  policyman_cfgitem_is_present_in_itemset(
  mre_set_t               *pItemSet,
  policyman_item_id_t     id,
  sys_modem_as_id_e_type  asubs_id,
  policyman_item_t      **ppItem
);


/*-------- policyman_cfgitem_find_item_in_itemset --------*/
void * policyman_cfgitem_find_item_in_itemset(
  mre_set_t              *pItemSet,
  policyman_item_id_t     id,
  sys_modem_as_id_e_type  asubs_id
);

/*-------- policyman_send_all_config_items --------*/
void policyman_send_all_config_items(
  void
);

/*-------- policyman_cfgitem_get_change_msg_hdr_with_allocate --------*/
msgr_hdr_s * policyman_cfgitem_get_change_msg_hdr_with_allocate(
  policyman_state_t *pState,
  mre_set_t         *pItemSet,
  mre_set_t         *pCurrentDbSet,
  boolean           alwaysAllocate
);

/*-------- policyman_cfgitem_get_change_msg_hdr --------*/
msgr_hdr_s *policyman_cfgitem_get_change_msg_hdr(
  policyman_state_t *pState,
  mre_set_t         *pItemSet,
  mre_set_t         *pCurrentDbSet
);

/*-------- policyman_cfgitem_persist_item --------*/
boolean policyman_cfgitem_persist_item(
  char const             *pPath,
  void                   *pData,
  size_t                  dataSize,
  sys_modem_as_id_e_type  asubs_id
);

/*-------- policyman_cfgitem_update_device_config_for_force_notification --------*/
void policyman_cfgitem_update_device_config_for_force_notification(
  mre_set_t *pItemSet
);

/*-------- policyman_cfgitem_get_device_config --------*/
policyman_item_t * policyman_cfgitem_get_device_config(
  void
);

/*-------- policyman_cfgitem_find_item_in_itemset_or_database --------*/
policyman_item_t* policyman_cfgitem_find_item_in_itemset_or_database(
  mre_set_t              *pItemSet,
  policyman_item_id_t     id,
  sys_modem_as_id_e_type  subsId
);


/*-------- policyman_cfgitem_find_item_based_on_lookup_method --------*/
boolean policyman_cfgitem_find_item_based_on_lookup_method(
  lookup_method_t          method,
  mre_set_t               *pItemSet,
  policyman_item_id_t      id,
  sys_modem_as_id_e_type   subsId,
  policyman_item_t       **ppItem
);


/*-------- policyman_cfgitem_item_is_present_based_on_lookup_method --------*/
boolean policyman_cfgitem_item_is_present_based_on_lookup_method(
  lookup_method_t          method,
  mre_set_t               *pItemSet,
  policyman_item_id_t      id,
  sys_modem_as_id_e_type   subsId
);


/*=============================================================================
  Notify History definitions
=============================================================================*/
typedef struct policyman_notify_history_entry_s   policyman_notify_history_entry_t;
typedef struct policyman_notify_history_s         policyman_notify_history_t;

/*-------- policyman_cfgitem_update_notify_history_cfg --------*/
void policyman_cfgitem_update_notify_history_cfg(
  LOG_PM_CONFIG_INFO_type    *log_ptr,
  int                         notifyHistoryIndex
);

/*-------- policyman_cfgitem_update_notify_history_subs --------*/
void policyman_cfgitem_update_notify_history_subs(
  size_t                      subsIndex,
  LOG_PM_SUBS_INFO_type      *log_ptr,
  int                         notifyHistoryIndex
);

/*-------- policyman_cfgitem_init_notify_history --------*/
void policyman_cfgitem_init_notify_history(
  policyman_state_t  *pState
);

/*-------- policyman_cfgitem_deinit_notify_history --------*/
void policyman_cfgitem_deinit_notify_history(
  policyman_state_t  *pState
);

/*-------- policyman_notify_get_history --------*/
size_t policyman_notify_get_history(
  pm_dump_notify_history_t *h_buffer,
  size_t                    h_len
);

#define POLICYMAN_DBUPDATE_STATE_NONE         0
#define POLICYMAN_DBUPDATE_STATE_SUSPENDED    1
#define POLICYMAN_DBUPDATE_STATE_UPDATING     2

typedef unsigned int policyman_dbupdate_state;

/*-------- policyman_dbupdate_state_get --------*/
policyman_dbupdate_state policyman_dbupdate_state_get(
  void
);

/*-------- policyman_dbupdate_block_signal_wait --------*/
void policyman_dbupdate_block_signal_wait(
  void
);

/*-------- policyman_dbupdate_try_enter_update_state --------*/
boolean policyman_dbupdate_try_enter_update_state(
  void
);

/*-------- policyman_dbupdate_try_enter_suspend_state --------*/
boolean policyman_dbupdate_try_enter_suspend_state(
  void
);

/*-------- policyman_dbupdate_enter_idle_state --------*/
void policyman_dbupdate_enter_idle_state(
  void
);


/*-------- policyman_update_subphone_cap_in_itemset --------*/
boolean policyman_update_subphone_cap_in_itemset(
  mre_set_t *pItemSet
);

/*-------- policyman_cfgitem_set_limited_capabilities --------*/
void policyman_cfgitem_set_limited_capabilities(
  policyman_cfg_error_cause errCause
);

/*-------- policyman_cfgitem_limited_capabilities_is_enabled --------*/
boolean policyman_cfgitem_limited_capabilities_is_enabled(
  void
);

/*-------- policyman_get_limited_error_cause --------*/
policyman_cfg_error_cause policyman_get_limited_error_cause(
  void
);

/*-------- policyman_ensure_items_in_itemset --------*/
void policyman_ensure_items_in_itemset(
  policyman_state_t *pState,
  mre_set_t         *pItemSet
);

/*-------- policyman_db_clone --------*/
mre_set_t * policyman_db_clone(
  mre_set_t const *pCurrentDbSet
);

/*-------- policyman_subs_id_is_configured --------*/
boolean policyman_subs_id_is_configured(
  sys_modem_as_id_e_type subsId
);

/*-------- policyman_cfgitem_clone_itemset --------*/
mre_set_t *policyman_cfgitem_clone_itemset(
  mre_set_t const *pFromSet
);

/*-------- policyman_mdump_cfgitems --------*/
void policyman_cfgitems_mdump(
  void
);
#endif /* _POLICYMAN_CFGITEM_H_ */
