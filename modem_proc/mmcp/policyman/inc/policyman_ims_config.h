#ifndef _POLICYMAN_IMS_CONFIG_H_
#define _POLICYMAN_IMS_CONFIG_H_

/**
  @file policyman_ims_config.h

  @brief Configuration item for IMS status.
*/

/*
    Copyright (c) 2016-2018 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_ims_config.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "sys.h"
#include "policyman_i.h"
#include "policyman_cfgitem.h"
#include "policyman_diag.h"

typedef struct policyman_ims_config_item_t policyman_ims_config_item_t;

#define PM_IMS_ONLY_FROM_XML "pm:ims_only_from_xml"

/*-------- policyman_ims_config_compare --------*/
boolean policyman_ims_config_compare(
  policyman_item_t  *pItem1,
  policyman_item_t  *pItem2
);

/*-------- policyman_ims_config_item_clone --------*/
policyman_item_t * policyman_ims_config_item_clone(
  policyman_item_t const *pItem
);

/*-------- policyman_ims_config_display --------*/
void policyman_ims_config_display(
  policyman_item_t *pItem
);

/*-------- policyman_ims_config_get_default --------*/
policyman_item_t * policyman_ims_config_get_default(
  mre_set_t              *pItemSet, 
  sys_modem_as_id_e_type  subsId
);

/*-------- policyman_ims_config_item_new --------*/
policyman_ims_config_item_t * policyman_ims_config_item_new(
  mre_set_t              *pItemSet,
  sys_modem_as_id_e_type  subsId,
  boolean                 imsAllowed
);

/*-------- policyman_ims_config_action_new --------*/
mre_status_t policyman_ims_config_action_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

/*-------- policyman_ims_config_ensure_items_for_subs --------*/
void policyman_ims_config_ensure_items_for_subs(
  policyman_state_t *pState,
  mre_set_t         *pItemSet
);

/*-------- policyman_ims_config_mdump --------*/
void policyman_ims_config_mdump(
  policyman_item_t const *pItem
);
#endif /* _POLICYMAN_IMS_CONFIG_H_ */
