#ifndef DSEPSPDNCONTEXTEX_H
#define DSEPSPDNCONTEXTEX_H
/*===========================================================================

            E P S   S P E C I F I C   M O D E   H A N D L E R

===========================================================================*/
/*!
  @file
  ds_eps_pdn_context_ex.h

  @brief
  This header file defines the externalized function that interface DSEPS 
  and ESM.

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/        
/*==============================================================================

  Copyright (c) 2008-2019 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/
/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/lteps_ex/inc/ds_eps_pdn_context_ex.h#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
02/27/18   vj      Added functions to handle the number of configured scells 
                   and mimo/modulation information
==============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_bearer_context.h"
#include "cm.h"

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/
typedef struct 
{
  boolean  dl_256_qam;
  boolean  ul_64_qam;
  boolean  mimo_4_layers;
} ds_eps_lte_mod_mimo_s;

/*----------------------------------------------------------------------------
  Macro's that related to ATTACH PENDING state
----------------------------------------------------------------------------*/
#define DS_EPS_PDN_CNTXT_IS_ATTACH_PENDING(subs_id)\
  ((ds3gdsdif_get_null_bearer_reason_info(subs_id) & DS3GDSDIF_SYS_NULL_BEARER_REASON_ATTACH_PENDING) != 0)
/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#ifdef FEATURE_DATA_4_5G

/*===========================================================================
FUNCTION DS_EPS_PDN_CNTXT_EX_CLEAR_NUM_CONFIGURED_SCELLS

DESCRIPTION
  This function clears the number of configured scells on a given
  subscription for carrier aggregation

PARAMETERS
  subs_id - subscription to retrieve scells

DEPENDENCIES
  None.

RETURN VALUE
  boolean
  
SIDE EFFECTS
  None.
===========================================================================*/

boolean ds_eps_pdn_cntxt_ex_clear_num_configured_scells
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION DS_EPS_PDN_CNTXT_EX_GET_NUM_OF_CONFIGURED_SCELLS

DESCRIPTION
  This function returns the number of configured scells on a given
  subscription for carrier aggregation

PARAMETERS
  subs_id - subscription to retrieve scells

DEPENDENCIES
  None.

RETURN VALUE
  number of configured scells.

SIDE EFFECTS
  None.
===========================================================================*/
uint8 ds_eps_pdn_cntxt_ex_get_num_configured_scells
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION DS_EPS_PDN_CNTXT_EX_PROCESS_NUM_CONFIGURED_SCELLS

DESCRIPTION
  This function processes the number of configured scells on a given
  subscription for carrier aggregation

PARAMETERS
  subs_id - subscription to retrieve scells

DEPENDENCIES
  None.

RETURN VALUE
  number of configured scells.

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_eps_pdn_cntxt_ex_process_num_configured_scells
(
  lte_cphy_ca_event_ind_s    *ds_lte_cphy_ca_event_ptr,
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION DS_EPS_PDN_CNTXT_EX_CLEAR_CA_INFO

DESCRIPTION
  This function clears carrier aggregation related info on a given
  subscription 

PARAMETERS
  subs_id - subscription to retrieve scells

DEPENDENCIES
  None.

RETURN VALUE
  boolean
  
SIDE EFFECTS
  None.
===========================================================================*/

boolean ds_eps_pdn_cntxt_ex_clear_ca_info
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION DS_EPS_PDN_CNTXT_EX_CHECK_MIMO_MOD

DESCRIPTION
  This function checks if the mimo/mod condition for indicating 4.5G has been 
  satisfied

PARAMETERS
  subs_id - subscription to retrieve scells

DEPENDENCIES
  None.

RETURN VALUE
  true if the condition is satisfied.else false

SIDE EFFECTS
  None.
===========================================================================*/
uint8 ds_eps_pdn_cntxt_ex_check_mimo_mod
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION DS_EPS_PDN_CNTXT_EX_INIT

DESCRIPTION

PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_pdn_cntxt_ex_init( void );

/*===========================================================================
FUNCTION DS_EPS_PDN_CNTXT_EX_GET_MIMO_MOD

DESCRIPTION
  This function returns the stored mimo/mod condition

PARAMETERS
  subs_id - subscription to retrieve mimo/mod

DEPENDENCIES
  None.

RETURN VALUE


SIDE EFFECTS
  None.
===========================================================================*/
void ds_eps_pdn_cntxt_ex_get_mimo_mod
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id,
  ds_eps_lte_mod_mimo_s      *mimo_mod
);
#endif /* FEATURE_DATA_4_5G */
/*===========================================================================
FUNCTION DS_EPS_PDN_CNTXT_EX_GET_CONNECT_MODE

DESCRIPTION
  This function returns the stored connect mode status

PARAMETERS
  subs_id - subscription to retrieve connect mode status

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_eps_pdn_cntxt_ex_get_connect_mode
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION DS_EPS_PDN_CNTXT_EX_SET_CONNECT_MODE

DESCRIPTION
  This function cache the connect mode status

PARAMETERS
  subs_id - subscription to retrieve connect mode status

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
void ds_eps_pdn_cntxt_ex_set_connect_mode
(
  boolean connect_mode,
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION      DS_EPS_PDN_CNTXT_GET_LTE_EMERGENCY_ATTACH_PARAMS

DESCRIPTION
  Provide LTE EMERGENCY ATTACH parameter (null_algo) to client
 
  Returns Failure if
    Input parameters are invalid.


PARAMETERS
  arg_val_ptr[in]   -    Pointer to operation specific argument
  ps_errno [out]    -    Specifies type of error in case of error.
                             DS_EFAULT if parameter is invalid
                             DS_EINVAL if operation is invalid
                             DS_NOMEMORY if memory allocation fails
  subs_id
 
DEPENDENCIES
  None.

RETURN VALUE
  0  - Success
 -1 - Failure

SIDE EFFECTS
  None.
===========================================================================*/
int ds_eps_pdn_cntxt_get_lte_emergency_attach_params
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION DS_EPS_PDN_CNTXT_EX_HO_COMPLETE_IND_HDLR

DESCRIPTION
  This function is invoked when DS gets handover complete indication from
  RRC through message router.

PARAMETERS
  msgrtype  - UMID of the message sent
  subs_id   - subscription ID
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
 TRUE/FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_pdn_cntxt_ex_ho_complete_ind_hdlr
(
  msgr_umid_type             msgrtype,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION  DS_EPS_PDN_CNTXT_EX_EVENT_REG

DESCRIPTION
  Register for ds3g event.

PARAMETERS
  event_id
  tech_type
  user_data_ptr

DEPENDENCIES
  None.

RETURN VALUE 
  None. 

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_pdn_cntxt_ex_event_reg
(
  ds3geventmgr_event_type                    event_id,
  ps_sys_tech_enum_type                      tech_type,
  void                                      *user_data_ptr
);
/*===========================================================================
FUNCTION  DS_EPS_PDN_DS3GEVENTMGR_EVENT_CB

DESCRIPTION
  This function is call back for DS3GEVENTMGR events.

PARAMETERS
  event_id
  filter_info_ptr
  event_info_ptr
  data_ptr

DEPENDENCIES
  None.

RETURN VALUE 
  None. 

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_pdn_ds3geventmgr_event_cb
(
  ds3geventmgr_event_type        event_id,
  ds3geventmgr_filter_type      *filter_info_ptr,
  void                          *event_info_ptr,
  void                          *data_ptr
);

/*===========================================================================
FUNCTION  DS_EPC_PDN_CNTXT_EX_PDN_STATUS_CB

DESCRIPTION
  This is the call back function for DS3GEVENTMGR_PDN_STATUS_INFO_EV.
  If IMS PDN is up on iwlan, then check whether need to enter attach pending
  state. Otherwise, reset attach pending state.

PARAMETERS
  tech
  subs_id
  event_info_ptr

DEPENDENCIES
  None.

RETURN VALUE 
  None. 

SIDE EFFECTS
  None.

===========================================================================*/
void ds_epc_pdn_cntxt_ex_pdn_status_cb
(
  ps_sys_tech_enum_type          tech,
  sys_modem_as_id_e_type         subs_id,
  void                          *event_info_ptr
);

/*===========================================================================
FUNCTION DS_EPS_PDN_CNTXT_EX_ATTACH_PENDING_CONDITION_IS_MET

DESCRIPTION
  This function checks all the conditions and return whether to enter attach
  pending state.
  1)Internet APN is throttled
  2)IMS PDN pref system is IWLAN
  3)UE is in HPLMN
  
PARAMETERS
  subs_id   - Subscription ID

DEPENDENCIES
  None.

RETURN VALUE
  TRUE  - if conditions are met to enter LTE attach pending state
  FALSE - otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_eps_pdn_cntxt_ex_attach_pending_condition_is_met
(
  sys_modem_as_id_e_type      subs_id
);

/*===========================================================================
FUNCTION  DS_EPS_PDN_CNTXT_EX_CHECK_ATTACH_PENDING

DESCRIPTION
  This function checks if UE enters attach pending state, set the state and
  notify IMS. Otherwise, if UE is in attach pending state, clean attach sm
  and null bearer reason bit if it is set and reprocess attach IND.

PARAMETERS
  subs_id

DEPENDENCIES
  None.

RETURN VALUE 
  TRUE - Enter attach pending
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_pdn_cntxt_ex_check_attach_pending
(
  sys_modem_as_id_e_type         subs_id
);

/*===========================================================================
FUNCTION  DS_EPS_PDN_CNTXT_EX_RESET_ATTACH_PENDING

DESCRIPTION
  This function is called when attach pending conditions are not met.
  Reset the attach sm and clean attach pending null bearer reason bit.

PARAMETERS
 subs_id

DEPENDENCIES
  None.

RETURN VALUE 
  None. 

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_pdn_cntxt_ex_reset_attach_pending
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_EPS_PDN_CNTXT_EX_REPROCESS_PDN_CONN_IND_HDLR

DESCRIPTION
  This function is called when UE exits attach pending state. Reprocess the
  attach req NAS sent before.
 
PARAMETERS
 subs_id

DEPENDENCIES
  None.

RETURN VALUE 
  None. 

SIDE EFFECTS
  None.

===========================================================================*/
void ds_eps_pdn_cntxt_ex_reprocess_pdn_conn_ind_hdlr
(
  sys_modem_as_id_e_type  subs_id
);

#endif    /* DSEPSPDNCONTEXT_H */

