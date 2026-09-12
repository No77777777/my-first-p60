#ifndef _POLICYMAN_UIM_H_
#define _POLICYMAN_UIM_H_

/**
  @file policyman_uim.h

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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_uim.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "policyman_cm.h"
#include "policyman_i.h"
#include "policyman_state.h"

#include "sys.h"
#include "mmgsdisessionlib.h" /* Included for MMGSDI Session prototypes */
#include "simlock_common.h"    /* Included for SIMLOCK IMSI type */
#include "policyman_dbg.h"


typedef struct
{
  sys_plmn_id_s_type    plmn;
  mmgsdi_app_enum_type  sim_app_type;
  mmgsdi_app_enum_type  cdma_app_type;
  boolean               sim_refreshed;
  boolean               subs_active;
  sys_mcc_type          csim_mcc;
  simlock_imsi_type     simlock_imsi;    /* SIMLOCK IMSI */
} policyman_uim_info_t;

#define MAX_SIM_CARDS  3

/*-------- policyman_uim_init --------*/
boolean policyman_uim_init(
  policyman_uim_info_t  **pUimInfo
);

/*-------- policyman_uim_deinit --------*/
void policyman_uim_deinit(
  policyman_uim_info_t  *pUimInfo
);

/*-------- policyman_uim_info_deep_copy --------*/
policyman_uim_info_t * policyman_uim_info_deep_copy(
  policyman_uim_info_t  *pUimState
);

/*-------- policyman_uim_get_imsi_plmn --------*/
boolean policyman_uim_get_imsi_plmn(
  policyman_state_t   *pInfo,
  size_t               subsId,
  sys_plmn_id_s_type  *pPlmn
);

/*-------- policyman_uim_get_imsi_m_mcc --------*/
uint32 policyman_uim_get_imsi_m_mcc(
  policyman_state_t  *pState,
  size_t              subsId
);

/*-------- policyman_uim_set_sim_type --------*/
boolean policyman_uim_set_sim_type(
  policyman_state_t    *pInfo,
  size_t                subsId,
  mmgsdi_app_enum_type  sim_app_type
);

/*-------- policyman_uim_get_sim_type --------*/
mmgsdi_app_enum_type policyman_uim_get_sim_type(
  policyman_state_t    *pInfo,
  size_t                subsId
);

/*-------- policyman_uim_set_cdma_sim_type --------*/
boolean policyman_uim_set_cdma_sim_type(
  policyman_state_t    *pInfo,
  size_t                subsId,
  mmgsdi_app_enum_type  sim_app_type
);

/*-------- policyman_uim_get_cdma_sim_type --------*/
mmgsdi_app_enum_type policyman_uim_get_cdma_sim_type(
  policyman_state_t *pInfo,
  size_t             subsId
);

/*-------- policyman_uim_set_sim_refresh --------*/
void policyman_uim_set_sim_refresh(
  policyman_state_t *pInfo,
  size_t             subsId,
  boolean            simWasRefreshed
);

/*-------- policyman_uim_get_sim_refresh --------*/
boolean policyman_uim_get_sim_refresh(
  policyman_state_t *pInfo,
  size_t             subsId
);

/*-------- policyman_uim_sim_is_3gpp2 --------*/
boolean policyman_uim_sim_is_3gpp2(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId,
  boolean                 *pCdmaIsFromSim,
  boolean                 *pCdmaIsFromNv
);

/*-------- policyman_condition_sim_type_new --------*/
mre_status_t policyman_condition_sim_type_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
);

/*-------- policyman_condition_imsi_plmn_new --------*/
mre_status_t policyman_condition_imsi_plmn_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
);

/*-------- policyman_condition_imsi_mcc_new --------*/
mre_status_t policyman_condition_imsi_mcc_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
);

/*-------- policyman_condition_sim_refresh_new --------*/
mre_status_t policyman_condition_sim_refresh_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
);

/*-------- policyman_uim_sim_refresh_reset_action_new --------*/
mre_status_t policyman_uim_sim_refresh_reset_action_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_action_t                  **ppAction
);

/*-------- policyman_condition_have_imsi_new --------*/
mre_status_t policyman_condition_have_imsi_new(
  mre_xml_element_t const        *pElem,
  mre_policy_t                   *pPolicy,
  mre_condition_t               **ppCondition
);

/*-------- policyman_uim_fill_diag_session_info --------*/
void policyman_uim_fill_diag_session_info(
  size_t                          count,
  pm_dump_uim_session_info_t     *dump_uim_info
);

/*-------- policyman_uim_fill_diag_uim_info --------*/
void policyman_uim_fill_diag_uim_info(
  policyman_uim_info_t  *src_uim_info,
  pm_dump_uim_info_t    *dump_uim_info
);

/*-------- policyman_condition_simlock_enabled_new --------*/
mre_status_t policyman_condition_simlock_enabled_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
);

/*-------- policyman_simlock_set_imsi --------*/
void policyman_simlock_set_imsi(
  policyman_state_t       *pState,
  sys_modem_as_id_e_type   subsId,
  simlock_imsi_type       *pImsi
);

/*-------- policyman_uim_get_subs_active --------*/
boolean policyman_uim_get_subs_active(
  policyman_state_t      *pState,
  sys_modem_as_id_e_type  subsId
);

/*-------- imsi_is_present --------*/
boolean imsi_is_present(
  policyman_state_t           *pState,
  sys_modem_as_id_e_type       subsId
);

/*-------- sim_type_is_present --------*/
boolean sim_type_is_present(
  policyman_state_t           *pState,
  sys_modem_as_id_e_type       subsId,
  mmgsdi_app_enum_type         desired_sim_app_type
);

/*-------- policyman_condition_fdn_status_new --------*/
mre_status_t policyman_condition_fdn_status_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
);

/*-------- policyman_condition_fdn_has_new --------*/
mre_status_t policyman_condition_fdn_has_new(
  mre_xml_element_t const  *pElem,
  mre_policy_t             *pPolicy,
  mre_condition_t         **ppCondition
);


#endif /* _POLICYMAN_UIM_H_ */
