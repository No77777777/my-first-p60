/*!
  @file
  ds_api.c

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009-2018 Qualcomm Technologies Incorporated. All Rights Reserved

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

===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps/src/ds_3gpp_api.c#4 $

when         who     what, where, why
--------     ---     -------------------------------------------------------- 
01/06/12     vb      Function call to dsBearerCntxt_is_ps_data_available() 

===========================================================================*/

#include "dsumtsps.h"
#include "ds_3gppi_utils.h"
#include "ds_3gpp_bearer_context.h"
#include "dstask_v.h"
#include "ds3gpp_api.h"
#include "dsutil.h"
#include "ds3gcfgmgr.h"
#include "ds_3gpp_nv_manager.h"

#ifdef FEATURE_DATA_LTE
#include "ds_eps_pdn_context.h"
#endif /* FEATURE_DATA_LTE */
#include "ds_3gpp_pdn_context_ex.h"

#ifdef FEATURE_DATA_LTE
static ds_3gpp_bearer_err_info_type ds_3gpp_beaer_err_info = {0};
#endif /* FEATURE_DATA_LTE */

static ds_3gpp_downlink_actual_throughput_ind_cb_type  
          ds_3gpp_wrrc_cb_info[SYS_MODEM_AS_ID_MAX] = {NULL};

extern ps_crit_sect_type ps_sys_crit_section;
/*===========================================================================
FUNCTION DS_3GPP_NOTIFY_IND

DESCRIPTION
   This is a generic function, used to notify DS module.

PARAMETERS
 indication type and 
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_notify_ind(ds_3gpp_notify_ind_e_type indication, void *notify_ind_data_ptr)
{
  ds_cmd_type               *cmd_ptr = NULL; 
  sys_modem_as_id_e_type    *data_ptr = NULL;
  ds_3gpp_rau_event_s_type  *ind_ptr=
                        ((ds_3gpp_rau_event_s_type*)notify_ind_data_ptr);
/*--------------------------------------------------------------------------------*/
  if(ind_ptr == NULL)
  {
    DS_3GPP_ERROR_LOG_0("ds_3gpp_notify_ind: recieved NULL ptr");
    return;
  }

   DS_3GPP_MSG3_HIGH("ds_3gpp_notify_ind: ind type:%d evt:%d rau type:%d", 
                      indication, ind_ptr->event, ind_ptr->rau_type);

  if(indication == DS_3GPP_RAU_IND)
  {
    switch(ind_ptr->event)
    {
     case DS_3GPP_RAU_COMPLETE_EVT:

	if(ind_ptr->rau_type != DS_3GPP_RA_CAHNGE_TYPE)
        {
	   return;
	}
	if( (cmd_ptr = ds_allocate_cmd_buf(sizeof(sys_modem_as_id_e_type))) == NULL )
        {
          DS_3GPP_ERROR_LOG_0("DS3GPP cant get cmd buf from DS task");
          return;
        }

          cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_RAU_COMPLTE_CMD;
        data_ptr = (sys_modem_as_id_e_type*)cmd_ptr->cmd_payload_ptr;
        *data_ptr = (sys_modem_as_id_e_type)ind_ptr->subs_id;
        ds_put_cmd(cmd_ptr);
       break;
     default:
       break;			
  }
  }
}/*ds3gpp_notify_ind*/
/*===========================================================================
FUNCTION DS3GPP_IS_PLMN_BLOCKED_FORCE_LTE_PREF

DESCRIPTION
   This is called by lower layers during powere off to see if any PLMN is blocked.
   lower layer will set their next pref sys based on the PLMN blocked status

PARAMETERS
  subs_id 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If any PLMN is blocked
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds3gpp_is_plmn_blocked_force_lte_pref
(
  sys_modem_as_id_e_type	  subs_id
)
{
  boolean ret_val = FALSE;
/*-------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_LTE
 if(ds_3gpp_pdn_cntxt_get_blocked_plmn_list_size(subs_id) > 0)
 {
   ret_val = TRUE;
   DS_3GPP_MSG2_HIGH_EX(subs_id,
                    "ds_3gpp_plmn_unblock_force_lte_perf:%d, on subs_id: %d",
                    ret_val, subs_id);
 }
#endif /* FEATURE_DATA_LTE */

  return ret_val;
}/*ds3gpp_is_plmn_blocked_force_lte_pref*/

/*===========================================================================
FUNCTION DSUMTSPS_IS_PS_DATA_AVAILABLE

DESCRIPTION
   This function returns TRUE if data is available in any of the uplink
   buffers, otherwise, it returns FALSE.
   The function is accessible at the SU api level.

PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If Data is available
  FALSE - If Data is not available

SIDE EFFECTS
  None.

===========================================================================*/

boolean ds3gpp_is_ps_data_available(void)
{
  sys_modem_as_id_e_type subs_id = ds3g_get_ps_subs_id();
  /*-----------------------------------------------------------------------*/

  return ds3gpp_per_subs_is_ps_data_available(subs_id);
}

boolean ds3gpp_per_subs_is_ps_data_available
(
  sys_modem_as_id_e_type subs_id
)
{  
  boolean ps_data_available;
  /*-----------------------------------------------------------------------*/

  ps_data_available = dsBearerCntxt_is_ps_data_available(subs_id);

  DS_3GPP_MSG1_MED_EX(subs_id,"dsumtsps_is_ps_data_available returns %s",
				     (ps_data_available)?"TRUE":"FALSE");

  return ps_data_available;

}

/*===========================================================================
FUNCTION DS3GPP_IS_UE_INIT_QOS_SUPPORTED

DESCRIPTION
   This function returns TRUE if umts UE initiated QOS support
   is available, otherwise, it returns FALSE.

PARAMETERS
  None.
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If UE initiated UMTS QOS support is available
  FALSE - If UE initiated UMTS QOS support is not available

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds3gpp_is_ue_init_qos_supported(void)
{
  boolean umts_qos_available = FALSE;
  
#if defined (FEATURE_DATA_WCDMA_PS) || defined (FEATURE_GSM_GPRS)
  nv_stat_enum_type   status;
  char item_file_path[] = "/nv/item_files/modem/data/3gpp/umtsps/qos_support";
      
  status = ds3gcfgmgr_read_efs_nv(
              item_file_path,
              (void *) &umts_qos_available,
              sizeof(umts_qos_available)
           );
  
  if (status != NV_DONE_S)
  {
    umts_qos_available = FALSE;
  }
#endif /* defined (FEATURE_DATA_WCDMA_PS) || defined (FEATURE_GSM_GPRS) */

  DS_3GPP_MSG1_HIGH("ds3gpp_is_ue_init_qos_supported returns %d",
                    (umts_qos_available)?1:0);
  return umts_qos_available;
}/* ds3gpp_is_ue_init_qos_supported */

/*===========================================================================
FUNCTION DS_3GPP_UPDATE_PCH_STATE

DESCRIPTION
  This function is used by RRC in WCDMA and TDSCDMA mode to indicate when
  UTRAN moves UE to PCH states: CELL_PCH and URA_PCH. Same needs to be
  propagated to applications. It will be useful for saving power.

PARAMETERS
  sys_mode  - system network mode, PCH states are applicable only in case of
             TDSCDMA and WCDMA.
  pch_event - In or out of PCH state.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_update_pch_state
(
  sys_sys_mode_e_type sys_mode,
  ds_3gpp_pch_event_e_type pch_event
)
{
  sys_modem_as_id_e_type     subs_id = ds3g_get_ps_subs_id();
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ds_3gpp_update_pch_state_sub(subs_id,sys_mode, pch_event);
}
/*===========================================================================
FUNCTION DS_3GPP_RRC_STATE_UPDATE

DESCRIPTION
  This function is used by RRC in WCDMA/TDS mode to indicate rrc state changes

PARAMETERS
  subs_id   - Subscription ID
  sys_mode  - WCDMA/TDS
  rrc_state - RRC state

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_rrc_state_update
(
   sys_modem_as_id_e_type               subs_id,
   sys_sys_mode_e_type                  sys_mode,
   sys_wcdma_rrc_state_e_type           rrc_state
)
{
   ds_cmd_type                        *cmd_ptr = NULL;
   ds_3gpp_rrc_state_ind_cmd_type     *data_ptr = NULL;

   DS_3GPP_MSG1_HIGH_EX(subs_id, "RRC state indication %d", rrc_state);

   cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_rrc_state_ind_cmd_type));
   if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
   {
     DS3GPP_ASSERT(0);
     return;
   }
   else
   {
     cmd_ptr->hdr.cmd_id        = DS_CMD_3GPP_RRC_STATE_IND_PROC;
     data_ptr = (ds_3gpp_rrc_state_ind_cmd_type *)cmd_ptr->cmd_payload_ptr;

     data_ptr->rrc_state = rrc_state;
     data_ptr->subs_id = subs_id;
     data_ptr->sys_mode = sys_mode;
     ds_put_cmd(cmd_ptr);
   }
}
/*===========================================================================
FUNCTION DS_3GPP_UPDATE_PCH_STATE_SUB

DESCRIPTION
  This function is used by RRC in WCDMA and TDSCDMA mode to indicate when
  UTRAN moves UE to PCH states: CELL_PCH and URA_PCH. Same needs to be
  propagated to applications. It will be useful for saving power.

PARAMETERS
  subs_id   - Subscription ID
  sys_mode  - system network mode, PCH states are applicable only in case of
             TDSCDMA and WCDMA.
  pch_event - In or out of PCH state.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_update_pch_state_sub
(
  sys_modem_as_id_e_type    subs_id,
  sys_sys_mode_e_type       sys_mode,
  ds_3gpp_pch_event_e_type  pch_event  
)
{
  ds_cmd_type                        *cmd_ptr = NULL;
  ds_3gpp_rrc_pch_ind_proc_cmd_type  *data_ptr = NULL;

  DS_3GPP_MSG2_HIGH_EX(subs_id,
       "PCH indication %d in sys mode %d", pch_event, sys_mode);

  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_rrc_pch_ind_proc_cmd_type));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }
  else
  {
    cmd_ptr->hdr.cmd_id        = DS_CMD_3GPP_RRC_PCH_IND_PROC;
    data_ptr = (ds_3gpp_rrc_pch_ind_proc_cmd_type*)cmd_ptr->cmd_payload_ptr;

    data_ptr->pch_event = pch_event;
    data_ptr->call_mode = sys_mode;
    data_ptr->subs_id = subs_id;
    ds_put_cmd(cmd_ptr);
  }
}
/*===========================================================================
FUNCTION DS_TDSCDMA_CONFIGURE_UL_DATA_RATE

DESCRIPTION
  This function is used by RRC in TDSCDMA mode to indicate the updated
  data rate on a particular rab_id/nsapi changes. Based on which
  wm levels are adjusted on that bearer

PARAMETERS
  rab_id  - Identifier used to uniquely identify a data bearer
  ul_data_rate - Updated data rate of the bearer
  subs_id      - Subscription on which this call is made

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_tdscdma_configure_ul_data_rate
(
   uint32 rab_id,  
   uint32  ul_data_rate,
   sys_modem_as_id_e_type subs_id
)
{
  
  ds_cmd_type               *cmd_ptr = NULL;
  ds_3gpp_ul_rab_info_type  *ul_rab_info_ptr = NULL;

  DS_3GPP_MSG3_HIGH_EX(subs_id,
                       "Configuring Ul rab id: %d data rate: %d subs_id %d ",
                       rab_id, ul_data_rate, subs_id);

  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_ul_rab_info_type));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }
  else
  {
    cmd_ptr->hdr.cmd_id =	DS_CMD_3GPP_TD_CONFIG_UL_DATA_RATE;
    ul_rab_info_ptr = (ds_3gpp_ul_rab_info_type*)cmd_ptr->cmd_payload_ptr;
    ul_rab_info_ptr->rab_id = rab_id;
    ul_rab_info_ptr->ul_data_rate = ul_data_rate;
    ul_rab_info_ptr->subs_id = subs_id;
    ds_put_cmd(cmd_ptr);
  }
}

/*===========================================================================
FUNCTION DS_3GPP_COMPARE_PDP_ADDRESS

DESCRIPTION
  This function is used by NAS to compare the pdp_address when network
  sends MODIFY_REQUEST with pdp_address included. DS will compare the
  pdp_address passed by NAS against the pdp_address which is configured on
  V4/V6 interfaces.

PARAMETERS
  identifier   - Identifier used to uniquely identify a PDP/PDN Context.
                 (For W/G/TD it would be nsapi, For LTE it would be eps_bid)
  sys_mode     - Current sys_mode.
  subs_id      - Subscription on which the PDP/PDN is active.
  *pdp_address - Structure containing the pdp_address and pdp_type_num 
                 given by network, which would be used for comparison.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE -  if PDP address sent by network is same as the one configured.
  FALSE - if PDP address sent by network is different than the one
          configured.

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_compare_pdp_address
(
  uint8 identifier,
  sys_sys_mode_e_type sys_mode,
  sys_modem_as_id_e_type subs_id,
  pdp_address_T *pdp_address
)
{
  boolean ret_val = FALSE;
  if (NULL == pdp_address)
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "Invalid pdp_address ptr passed");
	return FALSE;
  }

  ret_val = ds_3gpp_pdn_cntxt_compare_pdp_address(identifier, 
  	                                           sys_mode, 
  	                                           subs_id, 
  	                                           pdp_address);
  DS_3GPP_MSG1_HIGH_EX(subs_id, "PDP addr compare returning: %d", ret_val);
  return ret_val;
}

/*===========================================================================
FUNCTION DS_WCDMA_SET_CONFIGURED_DATA_RATE_IND

DESCRIPTION
  This function is used by  WCDMA rrc to set the configured uplink data rate
  and configured downlink data rate

PARAMETERS
  subs_id      - Subscription on which this call is made
  uplink_configured_kbps - Uplink configured data rate
  downlink_configured_kbps - Downlink configured data rate

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_set_configured_data_rate_ind
(
   sys_modem_as_id_e_type  subs_id,
   uint32  uplink_configured_kbps,
   uint32  downlink_configured_kbps
)
{
  ds_cmd_type               *cmd_ptr = NULL;
  ds_3gpp_configured_datarate_type *configured_data_info_ptr=NULL;

  DS_3GPP_MSG3_HIGH_EX(subs_id,
    "Setting configured Data rate:ul %d data rate:dl  %d data rate %d  cm subs_id", 
    uplink_configured_kbps, downlink_configured_kbps,subs_id);

  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_configured_datarate_type));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }
  else
  {
    cmd_ptr->hdr.cmd_id =	DS_CMD_3GPP_CONFIGURE_ULDL_DATA_RATE;
    configured_data_info_ptr = (ds_3gpp_configured_datarate_type*)cmd_ptr->cmd_payload_ptr;
    configured_data_info_ptr->uplink_configured_kbps = uplink_configured_kbps;
    configured_data_info_ptr->downlink_configured_kbps = downlink_configured_kbps;
    configured_data_info_ptr->subs_id = subs_id;
    ds_put_cmd(cmd_ptr);
  }
}
/*===========================================================================
FUNCTION DS_WCDMA_SET_UPLINK_DATA_RATE_IND

DESCRIPTION
  This function is used by  WCDMA rrc to set the uplink data rate
  and confidence

PARAMETERS
  subs_id      - Subscription on which this call is made
  uplink_data_rate_kbps - Uplink configured data rate
  confidence            - Uplink confidence

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_set_uplink_data_rate_ind
(
   sys_modem_as_id_e_type  subs_id,
   uint32  uplink_data_rate_kbps,
   uint8   confidence
)
{
#ifdef FEATURE_DATA_RAVE_SUPPORT 
  ds_cmd_type                 *cmd_ptr = NULL;
  ds_3gpp_uplink_datarate_ind *uplink_data_info_ptr=NULL;

  DS_3GPP_MSG3_HIGH_EX(subs_id,
    "Setting Uplink Data rate:ul %d data rate:dl  %d data rate %d  cm subs_id", 
    uplink_data_rate_kbps, confidence,subs_id);

  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_uplink_datarate_ind));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }
  else
  {
    cmd_ptr->hdr.cmd_id =	DS_CMD_3GPP_UPLINK_DATA_RATE_IND;
    uplink_data_info_ptr = (ds_3gpp_uplink_datarate_ind*)cmd_ptr->cmd_payload_ptr;
    uplink_data_info_ptr->uplink_datarate_kbps = uplink_data_rate_kbps;
    uplink_data_info_ptr->confidence = confidence;
    uplink_data_info_ptr->subs_id = subs_id;
    ds_put_cmd(cmd_ptr);
  }
#endif
}


/*===========================================================================
FUNCTION DS_WCDMA_QUAL_ESTIMATION_REPORT_IND

DESCRIPTION
  This function is used by  WCDMA rlc to set the downlink estimated rate
  and confidence

PARAMETERS
  subs_id      - Subscription on which this call is made
  wcdma_qual_estimation_report_data - Downlink data
 
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_qual_estimation_report_ind
(
   sys_modem_as_id_e_type  subs_id,
   ds_3gpp_downlink_estimated_data_ind w_data
)
{
  ds_cmd_type                 *cmd_ptr = NULL;
  ds_3gpp_downlink_estimated_data_ind *data_ptr=NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  DS_3GPP_MSG3_HIGH_EX(subs_id,
      "Received Downlink Data rate: %d kbps confidence:%d for  "
      "cm subs_id %d", w_data.data_rate_in_kbps, w_data.confidence ,subs_id);
  DS_3GPP_MSG3_HIGH_EX(subs_id,
      "Received Downlink freeze flag: %d kbps moved_out:%d for  "
      "cm subs_id %d", w_data.data_freeze, w_data.moved_out ,subs_id);

  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_downlink_estimated_data_ind));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }
  else
  {
    cmd_ptr->hdr.cmd_id =	DS_CMD_3GPP_DOWNLINK_ESTM_DATA_RATE_IND;
    data_ptr = (ds_3gpp_downlink_estimated_data_ind*)cmd_ptr->cmd_payload_ptr;
    data_ptr->data_rate_in_kbps = w_data.data_rate_in_kbps;
    data_ptr->confidence = w_data.confidence;
    data_ptr->data_freeze = w_data.data_freeze;
    data_ptr->moved_out = w_data.moved_out;
    data_ptr->subs_id = subs_id;
    ds_put_cmd(cmd_ptr);
  }
}


#ifdef FEATURE_DATA_LTE
/*===========================================================================
FUNCTION DS_3GPP_GET_BEARER_ERR_INFO_PTR

DESCRIPTION
  This function returns pointer to to bearer error cache variable

PARAMETERS
None
  
DEPENDENCIES
  None.

RETURN VALUE
  Cache Pointer
  
SIDE EFFECTS
  None.
  
===========================================================================*/
ds_3gpp_bearer_err_info_type* ds_3gpp_get_bearer_err_info_ptr (void)
{
   return &ds_3gpp_beaer_err_info;
}

/*===========================================================================
FUNCTION DS_3GPP_GET_BEARER_ERR_THRESHHOLD_CNT

DESCRIPTION
  This function returns threshhold count value for malformed packets 
   cached at DS 

PARAMETERS
None
  
DEPENDENCIES
  None.

RETURN VALUE
  Threshhold packet count
  
SIDE EFFECTS
  None.
  
===========================================================================*/
uint8 ds_3gpp_get_bearer_err_threshhold_cnt
(
  sys_modem_as_id_e_type         subs_id
)
{
  uint8    thresh_count = 0; 
  /* -----------------------------------------------------*/
  thresh_count = ds_3gpp_beaer_err_info.threshhold_pkt_cnt;

  DS_3GPP_MSG1_HIGH_EX(subs_id,
    "ds_3gpp_get_bearer_err_threshhold_cnt(): thresh_count: %d", 
    thresh_count);
  return thresh_count;
}

/*===========================================================================
FUNCTION DS_3GPP_REG_PDCP_BEARER_ERR_CB

DESCRIPTION
  This function is exposed to pdcp to register a callback function to be 
  triggered when malformed packet count on particular bearer exceeds 
  threshhold limit.

PARAMETERS
  pdcp_cb_ptr - callback ptr paseed from PDCP
  threshhold count- malformed packet limit
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE/FALSE.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
boolean ds_3gpp_reg_pdcp_bearer_err_cb 
( 
  ds_3gpp_bearer_err_ind_cb_type pdcp_cb_fptr, 
  uint8   threshhold_pkt_count 
)
{ 
  ds_3gpp_bearer_err_info_type* bearer_err_info_p = NULL;
  boolean                       ret_val = FALSE;
 /*-----------------------------------------------------------------*/
  DS_3GPP_MSG2_HIGH("ds_3gpp_reg_pdcp_bearer_err_cb(): cb_ptr: 0x%x, threshhold: %d", 
                                                 pdcp_cb_fptr, threshhold_pkt_count);
 
  bearer_err_info_p = ds_3gpp_get_bearer_err_info_ptr();
 
  if ((bearer_err_info_p != NULL) && (threshhold_pkt_count != 0))
  {
    bearer_err_info_p->pdpc_cb_fptr = pdcp_cb_fptr;
    bearer_err_info_p->threshhold_pkt_cnt = threshhold_pkt_count;
    
    ret_val = TRUE;
  }
  return ret_val;
}

#endif /* FEATURE_DATA_LTE */

/*===========================================================================
FUNCTION:  ds_3gpp_get_ip_type

DESCRIPTION
  This API will be used to get the supprted IP type for the bearer.
  

PARAMETERS
  inst_id    : subs_id
  bearer_id  : eps_bearer_id
  call_mode  : system call_mode

DEPENDENCIES
  None.

RETURN VALUE
  Supported IP Type

SIDE EFFECTS
  None.  
===========================================================================*/
#ifdef FEATURE_MSIM_L_PLUS_L
ds_3gpp_bearer_ip_support_type_e ds_3gpp_get_ip_type
(
  uint8                   bearer_id,
  sys_sys_mode_e_type     call_mode,
  uint8                   as_id 
)
#else
ds_3gpp_bearer_ip_support_type_e ds_3gpp_get_ip_type
(
  uint8                   inst_id, 
  uint8                   bearer_id,
  sys_sys_mode_e_type     call_mode
)
#endif
{
  ps_iface_type     *v4_iface_ptr = NULL;
  ps_iface_type     *v6_iface_ptr = NULL ;    
  sys_modem_as_id_e_type   subs_id = SYS_MODEM_AS_ID_NONE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_3GPP_MSG2_HIGH_EX(subs_id,
                   "ds_3gpp_get_ip_type called for mode %d with bearer_id %d",
                   call_mode, bearer_id);
  
#ifdef FEATURE_MSIM_L_PLUS_L  
  subs_id = (sys_modem_as_id_e_type)as_id;
#else
  subs_id = INST_ID_TO_SYS_AS_ID(inst_id);
#endif
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "Invalid Subs Id(CM) d", subs_id);
    return DS_3GPP_IP_TYPE_INVALID;
  }

#ifdef FEATURE_DATA_LTE
  if ( call_mode == SYS_SYS_MODE_LTE )
  {
    v4_iface_ptr = ds_eps_pdn_cntxt_get_v4_iface_ptr_from_eps_bearer_id(
                                                           bearer_id, subs_id);
    
    v6_iface_ptr = ds_eps_pdn_cntxt_get_v6_iface_ptr_from_eps_bearer_id(
                                                           bearer_id, subs_id);
  }
  else
#endif /* FEATURE_DATA_LTE */
  {
    /* No support to return PPP type as the return value. */
    v4_iface_ptr = ds_3gpp_bearer_cntxt_get_v4_iface_ptr_from_nsapi(
                     	                                     bearer_id, subs_id);
    v6_iface_ptr = ds_3gpp_bearer_cntxt_get_v6_iface_ptr_from_nsapi(
     	                                                     bearer_id, subs_id);
  }
  
  if ( v4_iface_ptr != NULL && v6_iface_ptr != NULL )
  {
    return DS_3GPP_IPV4V6;
  }
  else if ( v4_iface_ptr != NULL && v6_iface_ptr == NULL )
  {
    return DS_3GPP_IPV4;
  }
  else if ( v4_iface_ptr == NULL && v6_iface_ptr != NULL )
  {
    return DS_3GPP_IPV6;
  }
  else
  {
    return DS_3GPP_IP_TYPE_INVALID;
  }
  
} 

/*===========================================================================
FUNCTION ds3gpp_per_subs_get_config_multi_pdn_same_apn

DESCRIPTION
  This function gets the multi pdn same apn nv value for each subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether Multi Pdn same apn nv is enabled.

SIDE EFFECTS
===========================================================================*/

boolean ds3gpp_per_subs_get_config_multi_pdn_same_apn
(
  sys_modem_as_id_e_type subs_id
)
{
  return ds_3gpp_nv_manager_get_multi_pdn_same_apn(subs_id);
}

/*===========================================================================
FUNCTION ds3gpp_per_subs_get_config_umts_nw_init_qos_support

DESCRIPTION
  This function gets the umts_nw_init_qos_support NV item: 70331
  modem/data/3gpp/umts_nw_initiated_qos_support

PARAMETERS
  None
  
DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether UMTS NW Init QoS is enabled or not

SIDE EFFECTS
  None

===========================================================================*/
boolean ds3gpp_per_subs_get_config_umts_nw_init_qos_support
(
  sys_modem_as_id_e_type subs_id
)
{
  if (ds_3gpp_nv_manager_get_umts_nw_init_qos_support_nv() == DS_3GPP_NV_ENABLED)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================
FUNCTION DS3GPP_BIT_DATA_RATE_SUPP_CONFIG

DESCRIPTION
  This function is the API for RRC. During the attach, RRC would call this
  API to get the latest configuration for bit data rate support.
    
PARAMETERS: 
  subs_id
  *config
 
DEPENDENCIES
  E_SUCCESS: valid configuration populated to *config
  E_FAILURE: No invalid configuration to *config. config = NULL
  
RETURN VALUE 
  ds_3gpp_ran_bit_data_rate_support_config
 
SIDE EFFECTS 
  None 
  
===========================================================================*/
errno_enum_type ds3gpp_get_bit_data_rate_supp_config
(
  sys_modem_as_id_e_type                     subs_id,
  ds_3gpp_ran_bit_data_rate_support_config  *config
)
{
  errno_enum_type   ret_val = E_FAILURE;
  /*-----------------------------------------------------------*/
  if(config == NULL)
  {
    return ret_val;
  }
  ret_val = ds_3gpp_nv_manager_get_ran_bit_rate_config(subs_id, config);
  return ret_val;
}

/*===========================================================================
FUNCTION DS_3GPP_SET_WRRC_CB_FUNCTION

DESCRIPTION
  This function is to store wrrc callback function pointer.

PARAMETERS
  as_id: Subscription ID
  wcdma_rrc_cb_fptr: WCDMA RRC callback funciton pointer
  
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_3gpp_set_wrrc_cb_function
(
  sys_modem_as_id_e_type                         as_id,
  ds_3gpp_downlink_actual_throughput_ind_cb_type wcdma_rrc_cb_fptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds3gsubsmgr_is_subs_id_valid(as_id) ||
      NULL == wcdma_rrc_cb_fptr)
  {
    return;
  }

  ds_3gpp_wrrc_cb_info[as_id] = wcdma_rrc_cb_fptr;

  return;
}/*ds_3gpp_set_wrrc_cb_function*/

/*===========================================================================
FUNCTION DS_3GPP_GET_WRRC_CB_FUNCTION

DESCRIPTION
  This function returns pointer to to cache variable

PARAMETERS
  None
  
DEPENDENCIES
  None.

RETURN VALUE
  Cache Pointer
  
SIDE EFFECTS
  None.
  
===========================================================================*/
ds_3gpp_downlink_actual_throughput_ind_cb_type ds_3gpp_get_wrrc_cb_function
(
  sys_modem_as_id_e_type  subs_id
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return NULL;
  }

  return ds_3gpp_wrrc_cb_info[subs_id];
}/*ds_3gpp_get_wrrc_cb_function*/

/*===========================================================================
FUNCTION DS_3GPP_REG_DOWNLINK_THROUGHPUT_REPORT_CB

DESCRIPTION
  This function is exposed to WCDMA RRC to register a callback function to be 
  triggered when downlink actual throughput reporting is sent.
 
PARAMETERS
  as_id: Subscription ID
  wcdma_rrc_cb_fptr: Callback pointer passed from WCDMA RRC
  
DEPENDENCIES
  None.

RETURN VALUE  
  TRUE: registration successful
  FALSE: registration failed

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_reg_downlink_throughput_report_cb 
( 
  sys_modem_as_id_e_type                         as_id,
  ds_3gpp_downlink_actual_throughput_ind_cb_type wcdma_rrc_cb_fptr
)
{
  boolean  ret = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds3gsubsmgr_is_subs_id_valid(as_id) ||
      NULL == wcdma_rrc_cb_fptr)
  {
    return ret;
  }
  else
  {
    ret = TRUE;
  }
  
  /*Cache the subs id and callback function pointer*/
  ds_3gpp_set_wrrc_cb_function(as_id, wcdma_rrc_cb_fptr);
  /*Set W RRC reporting frequency*/
  ds3gtputmgr_set_w_rrc_throughput_report_frequency(as_id);
  /*Call downlink throughput report handler to check criteria*/
  ds3gtputmgr_downlink_throughput_report_hdlr(as_id);

  return ret;
}/*ds_3gpp_reg_downlink_throughput_report_cb*/

/*===========================================================================
FUNCTION DS_3GPP_IS_W2L_REDIR_ENABLE

DESCRIPTION
  This function checks whether W to L redirection is enabled or not.
 
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  TRUE: w2l redirection feature is enabled
  FALSE: w2l redirection feature is disabled

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_is_w2l_redir_enable
(
  sys_modem_as_id_e_type  subs_id
)
{
  ds3gtputmgr_w2l_redir_config_s   w2l_redir_config = {0};
  boolean                          w2l_redir_is_enabled = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return w2l_redir_is_enabled;
  }

  w2l_redir_config = ds_3gpp_nv_manager_get_w2l_redir_config_nv(subs_id);
  w2l_redir_is_enabled = w2l_redir_config.w2l_enable;

  return w2l_redir_is_enabled;
}/*ds_3gpp_is_w2l_redir_enable*/

/*===========================================================================
FUNCTION DS_3GPP_GET_W2L_REDIR_TSEARCH

DESCRIPTION
  This function gets the time(in seconds)that modem can spend in finding LTE cell.
 
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  Time in seconds

SIDE EFFECTS
  None

===========================================================================*/
uint32 ds_3gpp_get_w2l_redir_tsearch
(
  sys_modem_as_id_e_type  subs_id
)
{
  ds3gtputmgr_w2l_redir_config_s   w2l_redir_config = {0};
  uint32                           tsearch = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return tsearch;
  }

  w2l_redir_config = ds_3gpp_nv_manager_get_w2l_redir_config_nv(subs_id);
  tsearch = w2l_redir_config.tsearch;

  return tsearch;
}/*ds_3gpp_get_w2l_redir_tsearch*/

/*===========================================================================
FUNCTION DS_3GPP_GET_W2L_REDIR_T2

DESCRIPTION
  This function gets minimum time (in seconds) that the UE must wait to
  evaluate singaling power/quality of LTE ferq while in WCDMA connected state.
  
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  Time in seconds

SIDE EFFECTS
  None

===========================================================================*/
uint32 ds_3gpp_get_w2l_redir_t2
(
  sys_modem_as_id_e_type  subs_id
)
{
  ds3gtputmgr_w2l_redir_config_s   w2l_redir_config = {0};
  uint32                           t2 = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return t2;
  }

  w2l_redir_config = ds_3gpp_nv_manager_get_w2l_redir_config_nv(subs_id);
  t2 = w2l_redir_config.t2;

  return t2;
}/*ds_3gpp_get_w2l_redir_t2*/

/*===========================================================================
FUNCTION DS_3GPP_GET_W2L_REDIR_LTE_RSRQ

DESCRIPTION
  This function gets Reference Signal Received Quality LTE frequency(in dB)
  must meet.
 
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  Reference Signal Received Quality

SIDE EFFECTS
  None

===========================================================================*/
int32 ds_3gpp_get_w2l_redir_lte_rsrq
(
  sys_modem_as_id_e_type  subs_id
)
{
  ds3gtputmgr_w2l_redir_config_s   w2l_redir_config = {0};
  int32                            lte_rsrq = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return lte_rsrq;
  }

  w2l_redir_config = ds_3gpp_nv_manager_get_w2l_redir_config_nv(subs_id);
  lte_rsrq = w2l_redir_config.lte_rsrq;

  return lte_rsrq;
}/*ds_3gpp_get_w2l_redir_lte_rsrq*/

/*===========================================================================
FUNCTION DS_3GPP_GET_W2L_REDIR_LTE_RSRP

DESCRIPTION
  This function gets the Reference Signal Received Power that LTE frequency
  (in dB) must meet.
 
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  Reference Signal Received Power

SIDE EFFECTS
  None

===========================================================================*/
int32 ds_3gpp_get_w2l_redir_lte_rsrp
(
  sys_modem_as_id_e_type  subs_id
)
{
  ds3gtputmgr_w2l_redir_config_s   w2l_redir_config = {0};
  int32                            lte_rsrp = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return lte_rsrp;
  }

  w2l_redir_config = ds_3gpp_nv_manager_get_w2l_redir_config_nv(subs_id);
  lte_rsrp = w2l_redir_config.lte_rsrp;

  return lte_rsrp;
}/*ds_3gpp_get_w2l_redir_lte_rsrp*/


