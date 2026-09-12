#ifndef _POLICYMAN_CA_H_
#define _POLICYMAN_CA_H_

/**
  @file policyman_ca.h

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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_ca.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "sys.h"
#include "policyman_i.h"
#include "policyman_cfgitem.h"
#include "policyman_diag.h"

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

#endif /* _POLICYMAN_CA_H_ */
