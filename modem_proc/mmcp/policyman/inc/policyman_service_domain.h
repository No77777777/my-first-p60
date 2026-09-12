#ifndef _POLICYMAN_SERVICE_DOMAIN_H_
#define _POLICYMAN_SERVICE_DOMAIN_H_

/**
  @file policyman_service_domain.h

  @brief
*/

/*
    Copyright (c) 2015-2017 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_service_domain.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "sys.h"
#include "policyman_i.h"
#include "policyman_cfgitem.h"
#include "policyman_diag.h"

/*=============================================================================
  Service Domain APIs
=============================================================================*/

typedef struct policyman_service_domain_item_t policyman_service_domain_item_t;

/*-------- policyman_service_domain_item_new --------*/
policyman_service_domain_item_t* policyman_service_domain_item_new(
  sys_srv_domain_e_type  service_domain,
  sys_modem_as_id_e_type subsId
);


/*-------- policyman_service_domain_clone --------*/
policyman_item_t * policyman_service_domain_clone(
  policyman_item_t const *pItem
);


/*-------- policyman_service_domain_get_default --------*/
policyman_item_t* policyman_service_domain_get_default(
  mre_set_t              *pItemSet, 
  sys_modem_as_id_e_type  subsId
);


/*-------- policyman_service_domain_compare --------*/
boolean policyman_service_domain_compare(
  policyman_item_t *pItem1,
  policyman_item_t *pItem2
);


/*-------- policyman_service_domain_display --------*/
void policyman_service_domain_display(
  policyman_item_t *pItem
);

/*-------- policyman_service_domain_create_new_action --------*/
mre_status_t policyman_service_domain_create_new_action(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
);

/*-------- policyman_get_service_domain_for_subs --------*/
policyman_status_t policyman_get_service_domain_for_subs(
  sys_modem_as_id_e_type  subsId,
  sys_srv_domain_e_type  *pServiceDomain
);

/*-------- policyman_condition_service_domain_has_new --------*/
mre_status_t policyman_condition_service_domain_has_new(  
  mre_xml_element_t const        *pElem,  
  mre_policy_t                   *pPolicy,  
  mre_condition_t               **ppCondition
);

/*-------- policyman_service_domain_mdump --------*/
void policyman_service_domain_mdump(
  policyman_item_t const *pItem
);
#endif /* _POLICYMAN_SERVICE_DOMAIN_H_ */
