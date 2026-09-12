#ifndef _POLICYMAN_UE_MODE_H_
#define _POLICYMAN_UE_MODE_H_

/**
  @file policyman_ue_mode.h

  @brief
*/

/*
    Copyright (c) 2013-2017 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_ue_mode.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "sys.h"
#include "policyman_i.h"
#include "policyman_cfgitem.h"
#include "policyman_diag.h"



/*=============================================================================
  UE mode APIs
=============================================================================*/


typedef struct
{
  POLICYMAN_ITEM;

  sys_ue_mode_e_type  mode;
} policyman_ue_mode_item_t;


/*-------- policyman_ue_mode_item_clone --------*/
policyman_item_t * policyman_ue_mode_clone(
  policyman_item_t const *pItem
);

/*-------- policyman_ue_mode_item_new --------*/
/**
@brief

@param

@return

*/
policyman_ue_mode_item_t * policyman_ue_mode_item_new(
  sys_ue_mode_e_type      ue_mode,
  sys_modem_as_id_e_type  subsId
);



/*-------- policyman_ue_mode_get_default --------*/
/**
@brief

@param

@return

*/
policyman_item_t * policyman_ue_mode_get_default(
  mre_set_t              *pItemSet, 
  sys_modem_as_id_e_type  subsId
);


/*-------- policyman_ue_mode_compare --------*/
/**
@brief

@param

@return

*/
boolean policyman_ue_mode_compare(
  policyman_item_t  *pData1,
  policyman_item_t  *pData2
);



/*-------- policyman_ue_mode_display --------*/
/**
@brief

@param

@return

*/
void policyman_ue_mode_display(
  policyman_item_t  *pItem
);



/*-------- policyman_ue_mode_on_update --------*/
/**
@brief

@param

@return

*/
void policyman_ue_mode_on_update(
  policyman_item_t  *pItem
);


/*-------- policyman_ue_mode_action_new --------*/
/**
@brief

@param

@return

*/
mre_status_t policyman_ue_mode_action_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
);

/*-------- policyman_ue_mode_if_action_new --------*/
/**
@brief

@param

@return

*/
mre_status_t policyman_ue_mode_if_action_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
);


/*-------- policyman_condition_ue_mode_new --------*/
/**
@brief

@param

@return

*/
mre_status_t policyman_condition_ue_mode_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
);

/*-------- policyman_ue_mode_update_to_efs --------*/
/**
@brief

@param

@return

*/
void policyman_ue_mode_update_to_efs(
  policyman_item_t const *pItem
);

/*-------- policyman_retrieve_ue_mode --------*/
/**
@brief

@param

@return

*/
boolean policyman_retrieve_ue_mode(
  sys_modem_as_id_e_type   subsId,
  sys_ue_mode_e_type      *pUeMode
);

/*-------- policyman_ue_mode_get_default_from_efs --------*/
/**
@brief

@param

@return

*/
policyman_status_t policyman_ue_mode_get_default_from_efs(
  cfgitem_execute_ctx_t *pCfgCtx
);


/*-------- policyman_ue_mode_normalize_collection --------*/
/**
@brief  Normalize the UE modes in a collection by converting certain XXXX_ONLY
        UE modes to the appropriate real mode.

@param

@return

*/
void policyman_ue_mode_normalize_collection(
  policyman_item_collection_t *pCollection
);


/*-------- policyman_get_current_ue_mode --------*/
/**
@brief  Returns current UE mode on a given subscription

@param

@return

*/
policyman_status_t policyman_get_current_ue_mode(
  sys_modem_as_id_e_type  subsId,
  sys_ue_mode_e_type     *pMode
);

/*-------- policyman_ue_mode_fill_subs_diag_log --------*/
void policyman_ue_mode_fill_subs_diag_log(
  policyman_item_t const  *pItem,
  LOG_PM_SUBS_INFO_type   *log_ptr
);

/*-------- policyman_ue_mode_get_limited_item --------*/
void policyman_ue_mode_get_limited_item(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId
);

/*-------- policyman_ue_mode_mdump --------*/
void policyman_ue_mode_mdump(
  policyman_item_t const *pItem
);
#endif /* _POLICYMAN_UE_MODE_H_ */
