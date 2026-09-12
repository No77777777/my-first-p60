#ifndef _POLICYMAN_FULLRAT_CONFIG_H_
#define _POLICYMAN_FULLRAT_CONFIG_H_

/**
  @file policyman_fullrat_config.h

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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_fullrat_config.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "sys.h"
#include "policyman_i.h"

typedef struct fullrat_config_t fullrat_config_t;

typedef fullrat_config_t* (*fullrat_get_fn_t)(sys_modem_as_id_e_type subs);

/*-------- policyman_fullrat_config_get_carrier --------*/
fullrat_config_t *policyman_fullrat_config_get_carrier(
  sys_modem_as_id_e_type subsId
);

/*-------- policyman_fullrat_config_get_post --------*/
fullrat_config_t *policyman_fullrat_config_get_post(
  sys_modem_as_id_e_type subsId
);

/*-------- policyman_fullrat_config_init_per_subs --------*/
void policyman_fullrat_config_init_per_subs(
  sys_modem_as_id_e_type subsId
);

/*-------- policyman_fullrat_config_init_carrier_per_subs --------*/
void policyman_fullrat_config_init_carrier_per_subs(
  sys_modem_as_id_e_type subsId
);

/*-------- policyman_fullrat_config_timer_stop_per_subs --------*/
void policyman_fullrat_config_timer_stop_per_subs(
  sys_modem_as_id_e_type subsId
);

/*-------- policyman_fullrat_config_timer_stop_post_per_subs --------*/
void policyman_fullrat_config_timer_stop_post_per_subs(
  sys_modem_as_id_e_type subsId
);

/*--------- policyman_fullrat_config_reset_timer_expired_per_subs ------*/
void policyman_fullrat_config_reset_timer_expired_per_subs(
  sys_modem_as_id_e_type subsId
);

/*------- policyman_fullrat_config_reset_timer_expired_post_per_subs ------*/
void policyman_fullrat_config_reset_timer_expired_post_per_subs(
  sys_modem_as_id_e_type subsId
);

/*-------- policyman_fullrat_config_deinit --------*/
void policyman_fullrat_config_deinit(
  void
);

/*-------- policyman_fullrat_config_deinit_per_subs --------*/
void policyman_fullrat_config_deinit_per_subs(
  sys_modem_as_id_e_type subsId
);


/*-------- policyman_fullrat_config_new --------*/
mre_status_t policyman_fullrat_config_new(
  mre_xml_element_t const   *pElem,
  mre_policy_t              *pPolicy,
  mre_action_t             **ppAction
);

/*-------- policyman_fullrat_enter_new --------*/
mre_status_t policyman_fullrat_enter_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_action_t            **ppAction
);

/*-------- policyman_get_fullrat_timer --------*/
policyman_timer_t *policyman_get_fullrat_timer(
  sys_modem_as_id_e_type  subsId
);

/*-------- policyman_fullrat_config_evaluate_per_subs --------*/
void policyman_fullrat_config_evaluate_per_subs(
  policy_execute_ctx_t   *pCtx,
  sys_modem_as_id_e_type  subsId,
  fullrat_get_fn_t       pfnGetConfig
);

/*-------- policyman_fullrat_config_evaluate --------*/
void policyman_fullrat_config_evaluate(
  policy_execute_ctx_t   *pCtx,
  fullrat_get_fn_t       pfnGetConfig
);

/*-------- policyman_fullrat_config_notify_service --------*/
void policyman_fullrat_config_notify_service(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
);

#endif /* _POLICYMAN_FULLRAT_CONFIG_H_ */
