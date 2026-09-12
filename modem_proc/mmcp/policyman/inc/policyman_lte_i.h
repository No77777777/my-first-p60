#ifndef _POLICYMAN_LTE_I_H_
#define _POLICYMAN_LTE_I_H_

/**
  @file policyman_lte_i.h

  @brief
*/

/*
    Copyright (c) 2015-2018 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_lte_i.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "sys.h"
#include "policyman_i.h"
#include "policyman_cfgitem.h"
#include "policyman_diag.h"
#include "policyman_lte.h"

/*=============================================================================
  CA Combo APIs
=============================================================================*/

typedef struct policyman_ca_combo_item_t policyman_ca_combo_item_t;

/*-------- policyman_ca_combo_get_default --------*/
policyman_item_t* policyman_ca_combo_get_default(
  mre_set_t              *pItemSet, 
  sys_modem_as_id_e_type  subsId
);

/*-------- policyman_ca_combo_clone --------*/
policyman_item_t * policyman_ca_combo_clone(
  policyman_item_t const *pItem
);

/*-------- policyman_ca_combo_item_new --------*/
policyman_ca_combo_item_t* policyman_ca_combo_item_new(
  char const              *ca_combo,
  sys_modem_as_id_e_type   asubs_id
);

/*-------- policyman_ca_combo_compare --------*/
boolean policyman_ca_combo_compare(
  policyman_item_t  *pData1,
  policyman_item_t  *pData2
);

/*-------- policyman_ca_combo_display --------*/
void policyman_ca_combo_display(
  policyman_item_t  *pItem
);


/*-------- policyman_ca_combo_action_new --------*/
mre_status_t policyman_ca_combo_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

/*-------- policyman_ca_band_combos_evaluate --------*/
void policyman_ca_band_combos_evaluate(
  policy_execute_ctx_t  *pCtx
);

/*-------- policyman_ca_combo_mdump --------*/
void policyman_ca_combo_mdump(
  policyman_item_t const *pItem
);


/*=============================================================================
  LTE feature APIs
=============================================================================*/

typedef struct policyman_lte_feature_item_t policyman_lte_feature_item_t;


/*-------- policyman_lte_feature_get_default --------*/
policyman_item_t *policyman_lte_feature_get_default(
  mre_set_t              *pItemSet, 
  sys_modem_as_id_e_type  subsId
);


/*-------- policyman_lte_feature_clone --------*/
policyman_item_t *policyman_lte_feature_clone(
  policyman_item_t const *pItem
);


/*-------- policyman_lte_feature_item_new --------*/
policyman_lte_feature_item_t *policyman_lte_feature_item_new(
  uint64                  features,
  sys_modem_as_id_e_type  asubs_id
);


/*-------- policyman_lte_feature_compare --------*/
boolean policyman_lte_feature_compare(
  policyman_item_t  *pData1,
  policyman_item_t  *pData2
);


/*-------- policyman_lte_feature_display --------*/
void policyman_lte_feature_display(
  policyman_item_t  *pItem
);


/*-------- policyman_lte_feature_action_new --------*/
mre_status_t policyman_lte_feature_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

/*-------- policyman_lte_feature_mdump --------*/
void policyman_lte_feature_mdump(
  policyman_item_t const *pItem
);

/*-------- policyman_get_lte_features_from_set_or_db --------*/
/**
@brief  Get the current set of LTE features and return whether they were found
        in the itemset.

@param[in]  pItemSet  Pointer to the itemset to search
@param[in]  subsId    Subscription ID
@param[out] pFeatures Pointer to storage for the features

@return
  TRUE  if the features were found in the itemset
  FALSE if the features came from the database
*/
boolean policyman_get_lte_features_from_set_or_db(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId,
  uint64                 *pFeatures
);


/*-------- policyman_is_lte_feature_enabled --------*/
boolean policyman_is_lte_feature_enabled(
  uint64  features,
  uint64  whichFeature
);


/*-------- policyman_lte_feature_enable --------*/
uint64 policyman_lte_feature_enable(
  uint64  features,
  uint64  whichFeature
);


/*-------- policyman_lte_feature_disable --------*/
uint64 policyman_lte_feature_disable(
  uint64  features,
  uint64  whichFeature
);

#endif

