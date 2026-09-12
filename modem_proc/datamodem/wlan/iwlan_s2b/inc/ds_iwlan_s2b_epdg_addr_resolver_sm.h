#ifndef DS_IWLAN_S2B_EPDG_ADDR_RESLVR_SM_H
#define DS_IWLAN_S2B_EPDG_ADDR_RESLVR_SM_H
/*===========================================================================
                   DS_IWLAN_S2B_EPDG_ADDR_RESLVR_SM_H
===========================================================================*/
/*!
  @file
  ds_iwlan_s2b_pdn_sm.h

  @brief
  This module contains helper functions for the Address Resolver STM

  @detail
*/

/*===========================================================================

  Copyright (c) 2018 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/


/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/wlan/iwlan_s2b/inc/ds_iwlan_s2b_epdg_addr_resolver_sm.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/02/18    vl     Added changes for Rel. 13 VoWiFi emergency calls.
04/24/17    vls    Added FQDN resolution event
03/27/17    op     Added support for dsda
12/01/16    op     Created Module
===========================================================================*/

/*===========================================================================
                      INCLUDE FILES FOR MODULE
===========================================================================*/

#include "customer.h"


#include "stm2.h"
#include "sys.h"

/* Emergency SM instances are after non-emergency SM instances in the SM array*/
#define EMERGENCY_EPDG_ADDR_RESLVR_SM_START_IDX DS_IWLAN_S2B_CONFIG_MAX_SUBS_ID

typedef enum
{
  DS_IWLAN_S2B_ADDR_RESLVR_SM_MIN_EV = 0,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_WP_IFACE_READY_EV = DS_IWLAN_S2B_ADDR_RESLVR_SM_MIN_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_RESOLVE_FQDN_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_RESOLVE_DNS_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_A_DNS_QUERY_SUCCESS_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_A_DNS_QUERY_ERROR_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_A_DNS_QUERY_TIMEOUT_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_AAAA_DNS_QUERY_SUCCESS_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_AAAA_DNS_QUERY_ERROR_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_AAAA_DNS_QUERY_TIMEOUT_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_v4_CALL_BRINGUP_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_v6_CALL_BRINGUP_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_v4_EPDG_TTL_EXPIRED_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_v6_EPDG_TTL_EXPIRED_EV,
  DS_IWLAN_S2B_ADDR_RESLVER_SM_WLAN_INFO_CHANGED_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_TEARDOWN_EV,
  DS_IWLAN_S2B_ADDR_RESLVR_SM_MAX_EV = DS_IWLAN_S2B_ADDR_RESLVR_SM_TEARDOWN_EV
}ds_iwlan_s2b_epdg_addr_reslvr_sm_event_type; 

void ds_iwlan_s2b_epdg_addr_reslvr_sm_error_hook
(
  stm_status_t                error,
  const char*                 filename,
  uint32                      line,
  struct stm_state_machine_s* sm_ptr
);

/*===========================================================================
FUNCTION      ds_iwlan_s2b_epdg_addr_reslvr_sm_hdlr_alloc_sm()

DESCRIPTION   Initialize PDN SM machine.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_iwlan_s2b_epdg_addr_reslvr_sm_hdlr_alloc_sm
(
  sys_modem_as_id_e_type   subs_id,
  boolean                  is_emergency
);

/*===========================================================================
FUNCTION      ds_iwlan_s2b_epdg_addr_reslvr_sm_hdlr_dealloc_sm()

DESCRIPTION   Dealloc sm 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_iwlan_s2b_epdg_addr_reslvr_sm_hdlr_dealloc_sm
(
  sys_modem_as_id_e_type   subs_id,
  boolean                  is_emergency
);

/*===========================================================================
FUNCTION      ds_iwlan_s2b_epdg_addr_reslvr_sm_hdlr_post_ev()

DESCRIPTION   Send input to PDN State Machine.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_iwlan_s2b_epdg_addr_reslvr_sm_hdlr_post_ev
( 
  ds_iwlan_s2b_epdg_addr_reslvr_sm_event_type event,
  sys_modem_as_id_e_type                      subs_id,
  void*                                       info,
  boolean                                     is_emergency
);

/*===========================================================================
FUNCTION      ds_iwlan_s2b_epdg_addr_reslvr_sm_hdlr_get_state()

DESCRIPTION   Get current state of EPC SYS SM

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
stm_state_t ds_iwlan_s2b_epdg_addr_reslvr_sm_hdlr_get_state
(
  sys_modem_as_id_e_type                      subs_id
);


#endif /* DS_IWLAN_S2B_EPDG_ADDR_RESLVR_SM_H */

