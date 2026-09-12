/*===========================================================================
                        D S _3 G P P _ R M S M _ A T I . C 

DESCRIPTION

  This files contains the internal function for ATCoP 
  mode handler interactions. 
  

EXTERNALIZED FUNCTIONS

Copyright (c) 2009 - 2018 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            EDIT HISTORY FOR FILE
  
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/rmsm/src/ds_3gpp_rmsm_ati.c#2 $ 
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---   -----------------------------------------------------------
09/25/12    ss    3GPP MH LTE-Off compilation fixes.
04/30/12    nd    Requesting 2 PDPs for NULL/same APN, dynamic addressing and
                  same PDP type is against the 3gpp standards.
11/11/11    sa    Fixed de-registration of Flow modify events.
09/16/11    ss    Fixed memory corruption due to uninitialized memory access.
07/21/11    sa    Fixed port range issue.
09/13/10    sa    Fixed QCDGEN handler to send more data.
08/31/10    sa    Added support for default pdp type as IPv4v6.
06/17/10    ss    Fixed Klocwork error.
05/19/10    sa    Fixed crash during phys_link_ptr de-registration.
05/14/10    sa    Added support for packet event reporting.
05/12/10    sa    Added support for dynamic parameter related commands.
03/15/10    sa    Added +CGACT support for secondary PDP context,
                  +CGACT support for IPv4v6 profile, +CGCMOD command.
01/15/10    sa    Added support for $QCDGEN and +CGPADDR API.
12/11/09    sa    Created module
===========================================================================*/


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include <stringl/stringl.h>

#ifdef FEATURE_DATA_3GPP

#include "msg.h"
#include "ds3gmgr.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_rmsm_ati.h"
#include "ps_iface.h"
#include "ps_crit_sect.h"
#include "ps_handle_mgr.h"

#include "ds_3gpp_iface_ev_cb_hdlr.h"
#include "ds_3gpp_phys_link_ev_cb_hdlr.h"
#include "ds_3gpp_flow_ev_cb_hdlr.h"
#include "ds_3gpp_bearer_flow_manager.h"
#include "modem_mem.h"
#include "ds_3gpp_profile_cache_hdlr.h"
#include "ds_dsd_apm_ext_i.h"
/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
                            EXTERNAL DEFINITIONS
= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
                            INTERNAL DEFINITIONS
= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/
#define DS_3GPP_RMSM_ATI_MIN_DATA_LEN   21
#define DS_3GPP_RMSM_ATI_MAX_FRAME_SIZE 1500

#define DS_3GPP_RMSM_ATI_PKT_EVT_TYPE_INFO   0
#define DS_3GPP_RMSM_ATI_PKT_EVT_TYPE_INFO_REQ 1

#define DS_3GPP_RMSM_ATI_PKT_EVT_TFT_ONLY_CHANGE  0
#define DS_3GPP_RMSM_ATI_PKT_EVT_QOS_ONLY_CHANGE  1
#define DS_3GPP_RMSM_ATI_PKT_EVT_BOTH_TFT_QOS_CHANGE 2

typedef struct
{
  int                          ds_3gpp_rmsm_at_instance; /* RMSM_AT instance */
  ps_iface_event_enum_type     event;                    /* Flow     event   */
  ps_iface_event_info_u_type   event_info;               /* Event info       */
  ps_flow_type                *flow_ptr;                 /* FLow pointer     */
} ds_3gpp_rmsm_at_flow_type;

typedef struct
{
  int                          ds_3gpp_rmsm_at_instance; /* RMSM_AT instance */
  ps_iface_event_enum_type     event;                    /* PS Iface event   */
  ps_iface_event_info_u_type   event_info;               /* Event info       */
  ps_iface_type               *iface_ptr;                /* IFACE pointer    */
  boolean                      result;                    /* result of reg   */
} ds_3gpp_rmsm_at_type;



#define DS_3GPP_RMSM_SINGLE_IFACE_EV_COUNT 2

static ps_iface_event_enum_type single_iface_ev_p[DS_3GPP_RMSM_SINGLE_IFACE_EV_COUNT] = 
       {IFACE_UP_EV,IFACE_DOWN_EV};

#define DS_3GPP_RMSM_IFACE_EV_COUNT 2
static ps_iface_event_enum_type iface_ev_p[DS_3GPP_RMSM_IFACE_EV_COUNT] = 
       {IFACE_FLOW_ENABLED_EV,IFACE_FLOW_DISABLED_EV};

#define DS_3GPP_RMSM_PHYS_LINK_EV_COUNT 3
static ps_iface_event_enum_type phys_link_ev_p[DS_3GPP_RMSM_PHYS_LINK_EV_COUNT] = 
       {PHYS_LINK_FLOW_ENABLED_EV,PHYS_LINK_FLOW_DISABLED_EV,PHYS_LINK_GONE_EV};


#define DS_3GPP_RMSM_FLOW_EV_COUNT 2
static ps_iface_event_enum_type flow_ev_p[DS_3GPP_RMSM_FLOW_EV_COUNT] = 
       { FLOW_ACTIVATED_EV,FLOW_NULL_EV};

#define DS_3GPP_RMSM_FLOW_MOD_EV_COUNT 2
static ps_iface_event_enum_type flow_ev_mod_p[DS_3GPP_RMSM_FLOW_MOD_EV_COUNT] = 
       { FLOW_MODIFY_REJECTED_EV,FLOW_MODIFY_ACCEPTED_EV};
   

/*===========================================================================
                            Forward Declarations
===========================================================================*/
  ds_3gpp_ati_pdp_state_type ds_3gpp_ati_pdp_state[DS_3GPP_RMSM_AT_UM_MAX_INSTANCES] = {{NULL}};
  rex_crit_sect_type         ds_3gpp_rmsm_crit_sect;

#define DS_3GPP_RMSM_ATI_BYTE_REV_CPY(d,s,l) \
  {uint8 idx; for (idx=0; idx<l; idx++) *(d + idx) = *(s + ((l - idx) - 1));} 
/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
                          LOCAL FUNCTION DEFINTIONS
= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

LOCAL void ds_3gpp_rmsm_ati_um_iface_down_handler
(
  ds_3gpp_rmsm_at_instance_type  instance,
  ps_iface_event_info_u_type     event_info, 
  ps_iface_type                 *iface_ptr
);
LOCAL void ds_3gpp_rmsm_ati_um_iface_up_handler
(
  ds_3gpp_rmsm_at_instance_type  instance,
  ps_iface_event_info_u_type     event_info,
  ps_iface_type                 *iface_ptr
);

LOCAL void ds_3gpp_rmsm_ati_um_event_handler_cb
(
  ps_iface_type             *this_iface_ptr,
  ps_iface_event_enum_type   event,
  ps_iface_event_info_u_type event_info,
  void                      *user_data_ptr
);

LOCAL ds_3gpp_rmsm_at_instance_type ds_3gpp_rmsm_ati_get_inst_num_from_um_iface
(
  const ps_iface_type* um_iface_ptr
);

LOCAL boolean  ds_3gpp_rmsm_ati_allocate_instance
(
  uint32 profile_number,
  sys_modem_as_id_e_type          subs_id,
  ds_3gpp_rmsm_at_instance_type*  instance
);

LOCAL boolean  ds_3gpp_rmsm_ati_activate_v4_iface
( 
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
);

LOCAL boolean  ds_3gpp_rmsm_ati_activate_v6_iface
( 
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
);

LOCAL dsat_result_enum_type ds_3gpp_rmsm_ati_tear_down_v4_iface
(
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
);

LOCAL dsat_result_enum_type ds_3gpp_rmsm_ati_tear_down_v6_iface
(
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
);

LOCAL boolean ds_3gpp_rmsm_ati_allocate_um_iface_ptr
( 
  ds_3gpp_ati_pdp_state_type     *pdp_ptr,
  ds_umts_pdp_type_enum_type      pdp_type
);

LOCAL ps_iface_state_enum_type ds_3gpp_rmsm_ati_get_um_iface_state_of_iface_ptr
(
  ps_iface_type          *ps_iface_ptr
);

LOCAL boolean ds_3gpp_rmsm_ati_generate_ip_frame
(
  dsm_item_type **ip_frame,
  uint16 total_data_len
);

LOCAL boolean ds_3gpp_rmsm_ati_send_data
(
  ps_iface_type                    *ps_iface_ptr,
  uint32                           *data_len
);

LOCAL void ds_3gpp_rmsm_at_um_iface_flow_evt_hdlr
(
  ds_cmd_type *ds_cmd_ptr
);

LOCAL void ds_3gpp_rmsm_at_um_iface_evt_hdlr
(
  ds_3gpp_rmsm_at_type    *rmsm_at_ptr
);

LOCAL void ds_3gpp_rmsm_phys_link_um_event_handler_cb
(
  ps_phys_link_type          *this_phys_link_ptr,
  ps_iface_event_enum_type   event,
  ps_iface_event_info_u_type event_info,
  void                      *user_data_ptr
);

LOCAL void ds_3gpp_rmsm_at_data_cmd_hdlr
(
  ds_cmd_type *ds_cmd_ptr
);

LOCAL void ds_3gpp_rmsm_at_flow_evt_hdlr
(
  ds_cmd_type *ds_cmd_ptr
);

LOCAL boolean ds_3gpp_rmsm_ip_tx_um_data_cb
(
  ps_sig_enum_type sig,
  void             *pkt_instance
);

LOCAL void ds_3gpp_rmsm_flow_event_handler_cb
(
  ps_flow_type                * flow_ptr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    event_info,
  void                        * user_data_ptr
);

LOCAL ds_3gpp_rmsm_at_instance_type ds_3gpp_rmsm_ati_get_inst_num_from_flow_ptr
(
  const ps_flow_type* flow_ptr
);

LOCAL dsat_result_enum_type ds_3gpp_rmsm_ati_activate_sec_pdp_ctx
(
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
);

LOCAL dsat_result_enum_type ds_3gpp_rmsm_ati_deactivate_sec_pdp_ctx
(
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
);

LOCAL dsat_result_enum_type ds_3gpp_rmsm_ati_modify_sec_pdp_ctx
(
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
);

#ifdef FEATURE_DATA_WCDMA_PS
LOCAL boolean ds_3gpp_rmsm_ati_conv_flow_to_umts_app_qos_from_id
(
  ds_umts_pdp_profile_type           *pdp_ptr,
  qos_spec_type                      *app_flow
);
#endif /* FEATURE_DATA_WCDMA_PS */

#ifdef FEATURE_GSM_GPRS
LOCAL boolean ds_3gpp_rmsm_ati_conv_flow_to_gprs_app_qos_from_id
(
  ds_umts_pdp_profile_type           *pdp_ptr,
  qos_spec_type                      *app_flow
);
#endif /* FEATURE_GSM_GPRS */

LOCAL boolean ds_3gpp_rmsm_ati_pdp_state_type_validate
(
    ds_3gpp_ati_pdp_state_type     *pdp_ptr
);

/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
                        INTERNAL FUNCTION DEFINTIONS
= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

/*===========================================================================
  MACRO DS_3GPP_RMSM_SET_FLOW_STATE
  
  DESCRIPTION   
    Given the call instance, sets the flow enabled flag
  
  DEPENDENCIES  
    None.
  
  RETURN VALUE  
    See Descritpion.
    
  SIDE EFFECTS  
    None.
  ===========================================================================*/
#define DS_3GPP_RMSM_SET_FLOW_STATE(i,flag)\
    ds_3gpp_ati_pdp_state[i].ds_3gpp_ati_pdp_state_type_dyn_p->um_flow_enabled = flag
  
  /*===========================================================================
  MACRO DS_3GPP_RMSM_GET_FLOW_STATE
  
  DESCRIPTION   
    Given the call instance, gets the flow enabled flag
  
  DEPENDENCIES  
    None.
  
  RETURN VALUE  
    See Descritpion.
    
  SIDE EFFECTS  
    None.
  ===========================================================================*/
#define DS_3GPP_RMSM_GET_FLOW_STATE(i)\
    ds_3gpp_ati_pdp_state[i].ds_3gpp_ati_pdp_state_type_dyn_p->um_flow_enabled
  
  /*===========================================================================
  MACRO DS_3GPP_RMSM_GET_PENDING_DATA
  
  DESCRIPTION   
    Given the call instance, gets the bytes of data pending
  
  DEPENDENCIES  
    None.
  
  RETURN VALUE  
    See Descritpion.
    
  SIDE EFFECTS  
    None.
  ===========================================================================*/
#define DS_3GPP_RMSM_GET_PENDING_DATA(i)\
    ds_3gpp_ati_pdp_state[i].ds_3gpp_ati_pdp_state_type_dyn_p->pending_data


/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_GET_INFO_PTR_FROM_INSTANCE
  
  DESCRIPTION
    Returns the pointer to 3GPP RMSM_AT instance for the specified instance 
    
  NOTE: THIS IS A RESTRICTED FUNCTION AND IS INTENDED TO BE USED BY 
  DS_3GPP_RMSM_AT MODULES ONLY
  
  RETURN VALUE
    Pointer to the RMSM AT info structure if a valid instance is passed, else
    NULL
  
  
  DEPENDENCIES
    None
  
  SIDE EFFECTS
    None
  ===========================================================================*/
ds_3gpp_ati_pdp_state_type *  ds_3gpp_rmsm_get_info_ptr_from_instance
(
  ds_3gpp_rmsm_at_instance_type instance
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( (instance >=0) && (instance < DS_3GPP_RMSM_AT_UM_MAX_INSTANCES ) )
  {
    if(ds_3gpp_ati_pdp_state[instance].in_use == TRUE)
    {
      DS_3GPP_MSG1_HIGH ("Instance valid is %d ",instance);
      return &ds_3gpp_ati_pdp_state[instance];
    }
  }
  return NULL;

} /* ds_3gpp_rmsm_get_info_ptr_from_instance */


/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_GET_UM_IFACE_STATE_OF_IFACE_PTR
  
  DESCRIPTION
    This function returns the UM IFACE state from um_iface pointer.

  RETURN VALUE
    PS_IFACE_STATE

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
LOCAL ps_iface_state_enum_type ds_3gpp_rmsm_ati_get_um_iface_state_of_iface_ptr
(
  ps_iface_type          *ps_iface_ptr
)
{
  ps_iface_state_enum_type  iface_state = IFACE_STATE_INVALID;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (ps_iface_ptr == NULL)
  {
    return iface_state;
  }
  iface_state = ps_iface_state(ps_iface_ptr);

  DS_3GPP_MSG3_HIGH("State of iface ptr [0x%x:%x] is %d",
          ps_iface_ptr->name,
          ps_iface_ptr->instance,
          (int)iface_state);
  return iface_state;
} /* ds_3gpp_rmsm_at_get_um_iface_state */


/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_CGACT_ACTIVATION

  DESCRIPTION
    This function activates the Profile, if not already activated .
    If the profile is already activated return SUCCESS immediately.

  RETURN VALUE
    DSAT_OK -- Profile is already activated
    DSAT_ERROR -- In case of any FAILURE
    DSAT_ASYNC_CMD -- Profile is being activated.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsat_result_enum_type  ds_3gpp_rmsm_ati_cgact_activation
(
  uint32                          profile_number,
  sys_modem_as_id_e_type  subs_id,
  void *                  user_info_ptr
)
{
  ds_umts_pdp_profile_status_etype  result = DS_UMTS_PDP_SUCCESS;
  ds_3gpp_rmsm_at_instance_type     instance =0;
  ds_3gpp_ati_pdp_state_type       *at_pdp_state_ptr = NULL;
  boolean                           v4_act_result = TRUE;
  boolean                           v6_act_result = TRUE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_3GPP_MSG2_HIGH ("HANDLE CGACT for Pkt data call profile number (%d)"
                     " subs_id (%d)", profile_number, subs_id);

  if(!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return DSAT_ERROR;
  }
  
  /* Getting instance, if the same profile is already activated 
     return that instance */
  if( FALSE == ds_3gpp_rmsm_ati_allocate_instance( profile_number, 
                                                   subs_id,
                                                &instance))
  {
    return DSAT_ERROR;
  }
  at_pdp_state_ptr = ds_3gpp_rmsm_get_info_ptr_from_instance(instance);
  if (NULL == at_pdp_state_ptr)
  {
    return DSAT_ERROR;
  }
  
  /* check if it is already activated , if so return from here */

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(at_pdp_state_ptr))
  {
       return DSAT_ERROR;
  } 

  if( at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id == profile_number )
  {
    DS_3GPP_MSG0_LOW ("Already activated return OK immdediately");
    return DSAT_OK;
  }

  /*-------------------------------------------------------------------------
    Set the profile number,user info and validity flag
  -------------------------------------------------------------------------*/
  at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id = profile_number;
  at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr = user_info_ptr;

  /*-------------------------------------------------------------------------
    Set the subscription id
  -------------------------------------------------------------------------*/
  at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->subs_id = subs_id;

  /*-------------------------------------------------------------------------
    Get the profile information & see what type of context is defined
  -------------------------------------------------------------------------*/
  result=ds_umts_get_pdp_profile_context_info_per_subs(
                          (uint16)at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id, 
                          dsumts_subs_mgr_get_subs_id(at_pdp_state_ptr->
                                                       ds_3gpp_ati_pdp_state_type_dyn_p->subs_id),
                         &(at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info));

  if( result != DS_UMTS_PDP_SUCCESS ) 
  {
    DS_3GPP_MSG0_LOW("Accessing Profile_context_info failed");
    return DSAT_ERROR;
  }
  else
  {
#ifdef FEATURE_SECONDARY_PDP
    /* Currently Secondary context activation is rejected */
    if( at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.secondary_flag == TRUE )
    {
      DS_3GPP_MSG1_LOW ("Context Specified is secondary.",
              at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id);
      return ds_3gpp_rmsm_ati_activate_sec_pdp_ctx(at_pdp_state_ptr);
    }
#endif /* FEATURE_SECONDARY_PDP */
  }

  if (at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.pdp_type == DS_UMTS_PDP_IPV4 ||
      at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.pdp_type == DS_UMTS_PDP_PPP )
  {
    v6_act_result = FALSE;
  }
  else if (at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.pdp_type == DS_UMTS_PDP_IPV6)
  {
    v4_act_result = FALSE;
  }

  at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state = DS_3GPP_ATIF_PDP_ACTIVATING;

  /* Activate V4 profile, if context is v4v6 or v4 */
  if ( (at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.pdp_type == DS_UMTS_PDP_IPV4 ) ||
       (at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.pdp_type == DS_UMTS_PDP_PPP )||
       (at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.pdp_type == DS_UMTS_PDP_IPV4V6 ) )
    
  {
    v4_act_result = ds_3gpp_rmsm_ati_activate_v4_iface( at_pdp_state_ptr);
  }

  /* Activate V6 profile, if context is v4v6 or v6 */
  if ( (at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.pdp_type == DS_UMTS_PDP_IPV6 ) ||
       (at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.pdp_type == DS_UMTS_PDP_IPV4V6 ) )
    
  {
    v6_act_result = ds_3gpp_rmsm_ati_activate_v6_iface( at_pdp_state_ptr);
  }
  
  if ( ( FALSE == v4_act_result ) && (FALSE == v6_act_result) )
  {
    DS_3GPP_MSG0_LOW (" Activation failed,return ERROR");
    at_pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state = DS_3GPP_ATIF_PDP_DEACTIVATING;
    return DSAT_ERROR;
  }
  return DSAT_ASYNC_CMD;
} /* ds_3gpp_rmsm_ati_cgact_activation() */

/*===========================================================================
  FUNCTION ds_3gpp_rmsm_ati_force_teardown_PDN

  DESCRIPTION
    This function performs force tear down of PDN with profile num and
    subs_id.

  RETURN VALUE
    DSAT_OK -- Profile is deactivated
    DSAT_ERROR -- In case of any FAILURE

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/

dsat_result_enum_type  ds_3gpp_rmsm_ati_force_teardown_PDN
(
  uint32                profile_number,
  sys_modem_as_id_e_type  subs_id
)
{
  ds_dsd_apm_req_pay_load           apm_payload = {0};
  ds_dsd_ext_error_code             dsd_result = DS_DSD_APM_SUCCESS;
  dsat_result_enum_type             result = DSAT_ERROR;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*  Dont perform force teardown if profile_number is not attach profile */
  dsd_result = ds_dsd_apm_req_ex(DS_DSD_APM_ATTCH_REQ, &apm_payload,subs_id);
  if (DS_DSD_APM_SUCCESS != dsd_result ||
          apm_payload.profile_id != profile_number)
  {
    goto func_exit;
  }

  /* Dont teardown if there's no valid PDN other than with this profile num */
  if(!dsPdnContext_any_other_pdn_diff_pf_num_active(profile_number, subs_id))
  {
    goto func_exit;
  }

  /* Send for OTA teardown for the profile id */
  if(ds_3gpp_pdn_cntx_teardown_pdn_per_profile(profile_number,
                PS_NET_DOWN_REASON_INTERNAL_CALL_ENDED, subs_id))
  {
    result = DSAT_OK;
  }

  func_exit :
    return result;
}  /* ds_3gpp_rmsm_ati_force_teardown_PDN */


/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_CGACT_DEACTIVATION

  DESCRIPTION
    This function de-activates the Profile activated by this module,
    if not already de-activated .
    If the profile is already de-activated return SUCCESS immediately.

  RETURN VALUE
    DSAT_OK -- Profile is already deactivated
    DSAT_ERROR -- In case of any FAILURE
    DSAT_ASYNC_CMD -- Profile is being deactivated.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsat_result_enum_type  ds_3gpp_rmsm_ati_cgact_deactivation
(
  uint32                profile_number,
  sys_modem_as_id_e_type  subs_id,
  void *                user_info_ptr
)
{

  ds_3gpp_rmsm_at_instance_type  instance =0;
  ds_3gpp_ati_pdp_state_type *info_ptr = NULL;
  dsat_result_enum_type result = DSAT_OK;
  dsat_result_enum_type v4_result = DSAT_OK;
  dsat_result_enum_type v6_result = DSAT_OK;
  ds_pdn_context_state_e pdn_state = DS_PDN_CONTEXT_STATE_DOWN ;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_3GPP_MSG2_HIGH("HANDLE +CGACT deact for Pkt data call profilenumber (%d)"
                     "subs_id (%d)", profile_number, subs_id);
  if(!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return DSAT_ERROR;
  }

  instance = ds_3gpp_rmsm_ati_get_inst_num_from_profile(profile_number, subs_id);

  if ( DS_3GPP_RMSM_AT_UM_MAX_INSTANCES == instance)
  {
    /* profile was already de-activated or not a +CGACT activated context*/

    /* Query MH to check whether the profile is active */
    pdn_state = ds_pdn_cntxt_get_pdn_context_state(profile_number, subs_id);
    if( DS_PDN_CONTEXT_STATE_DOWN != pdn_state)
    {
      /* Profile is activated by other application */
      result = ds_3gpp_rmsm_ati_force_teardown_PDN(profile_number, subs_id);
    }
    else
    {
      /* Profile is already de-activated. So return DSAT_OK */
      result = DSAT_OK;
    }
    DS_3GPP_MSG2_HIGH("PDN state [%d], result_code [%d]",pdn_state,result);
    return result;
  }
  info_ptr=ds_3gpp_rmsm_get_info_ptr_from_instance(instance);
  if (NULL == info_ptr)
  {
    return DSAT_ERROR;
  }

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(info_ptr))
  {
    return DSAT_ERROR;
  } 
 

#ifdef FEATURE_SECONDARY_PDP
  if(info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p != NULL &&
     info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.secondary_flag == TRUE )
  {
    DS_3GPP_MSG1_LOW ("Context Specified for de-activation secondary.",
            info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id);
    return ds_3gpp_rmsm_ati_deactivate_sec_pdp_ctx(info_ptr);
  }
#endif /* FEATURE_SECONDARY_PDP */

  if (info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p != NULL &&
      info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr != NULL ) 
  {
    v4_result = ds_3gpp_rmsm_ati_tear_down_v4_iface(info_ptr);
  }
  if (info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p != NULL && 
      info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr != NULL ) 
  {
    v6_result = ds_3gpp_rmsm_ati_tear_down_v6_iface(info_ptr);
  }
  if( (v4_result == DSAT_ASYNC_CMD) || (v6_result == DSAT_ASYNC_CMD) )
  {
    /* In case of v4v6, if v4 has returned error, 
       we will wait till iface down event for v6 */
    if(info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p != NULL)
    {
      info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state = DS_3GPP_ATIF_PDP_DEACTIVATING;
    }
    return DSAT_ASYNC_CMD;
  }
  else if ((DSAT_OK == v4_result) && (DSAT_OK == v6_result) )
  {
    result = DSAT_OK;
  }
  else
  {
    result = DSAT_ERROR;
  }
  DS_3GPP_MSG1_LOW ("Profile  de-activation result %d",result);

  return result;

}/* ds_3gpp_rmsm_ati_cgact_deactivation */


/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_ALLOCATE_UM_IFACE_PTR

  DESCRIPTION
    This function retrieves the pointer to UM iface in the system which can
    be used to originate the packet data call. The iface pointer is obtained
    from the routing lookup. If no policy info is provided, the router 
    returns the default UM interface.
    
    The PDP profile pointer is passed in the acl policy info.
    
    An UM iface is returned only if there is one available and it is not
    in IFACE_DOWN state
  
  RETURN VALUE
    TRUE : if Successful in getting a iface pointer
    FALSE : if no iface pointer is available
  
  DEPENDENCIES
    None
    
  SIDE EFFECTS
    None
===========================================================================*/
boolean ds_3gpp_rmsm_ati_allocate_um_iface_ptr
( 
  ds_3gpp_ati_pdp_state_type     *pdp_ptr,
  ds_umts_pdp_type_enum_type      pdp_type
)
{
  acl_policy_info_type      *acl_policy_info = NULL;
  ip_pkt_info_type          ip_info;
  ps_iface_type             **ps_iface_ptr= NULL;
  ps_iface_type             *um_iface_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (pdp_ptr == NULL)
  {
    return FALSE;
  }

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(pdp_ptr))
  {
    DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 

  acl_policy_info = (acl_policy_info_type *)modem_mem_alloc(
                                       sizeof(acl_policy_info_type),
                                       MODEM_MEM_CLIENT_DATA_CRIT );

  if (acl_policy_info == NULL ) 
  {
    return FALSE;
  }

    /* ACL info population */
  acl_init_policy_info(acl_policy_info);
  memset( acl_policy_info, 0, sizeof(acl_policy_info_type) );
  acl_policy_info->iface.kind = DSS_IFACE_NAME; 
  acl_policy_info->iface.info.name = DSS_IFACE_UMTS;
  acl_policy_info->policy_flag = DSS_IFACE_POLICY_SPECIFIED;
  acl_policy_info->pdp_info = (acl_pdp_ctx_cfg_type) (pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id);
  acl_policy_info->subs_id = ds3gsubsmgr_subs_id_cm_to_ds(pdp_ptr->
                                       ds_3gpp_ati_pdp_state_type_dyn_p->subs_id);
  /* ip_info population */
  memset( &ip_info, 0, sizeof(ip_info) );
  if(pdp_type == DS_UMTS_PDP_IPV6)
  {
    ip_info.ip_vsn = IP_V6;
    ps_iface_ptr = &pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr;
    acl_policy_info->ip_family = IPV6_ADDR;
  }
  else if (pdp_type == DS_UMTS_PDP_IPV4)
  {
    ps_iface_ptr = &pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr;
    ip_info.ip_vsn = IP_V4;
    ip_info.ip_hdr.v4.dest.ps_s_addr = ROUTE_DEFAULT_DST;
    acl_policy_info->ip_family = IPV4_ADDR;
  }
  else
  {
    modem_mem_free((void *)acl_policy_info, MODEM_MEM_CLIENT_DATA_CRIT );
    return FALSE;
  }

  /*-------------------------------------------------------------------------
    Bring_up has to be set to TRUE here so that the in_use flag is to TRUE
    when the IFACE gets assigned.
  -------------------------------------------------------------------------*/
  acl_policy_info->bring_up = TRUE;
  if ( route_get( &ip_info, 
                  acl_policy_info, 
                  FALSE, 
                  NULL, 
                  &um_iface_ptr ) < 0 )
  {
    *ps_iface_ptr = NULL;
    modem_mem_free((void *)acl_policy_info, MODEM_MEM_CLIENT_DATA_CRIT );
    return FALSE;
  }

  /*-------------------------------------------------------------------------
    For requesting another PDP, either of the three needs to be
    unique:
    1.  APN
    2.  PDP type IP vs PPP
    3.  2 static IP addrs or (1 static+ 1 dynamic addr)

    If UM iface passed in ACL is already bounded to rmsm instance hence is already 
    associated with some other call in any state then we will cancel this new activation.
  -------------------------------------------------------------------------*/
  DS_3GPP_MSG2_HIGH("um iface: [0x%x:%x]",
						 um_iface_ptr->name,
						 um_iface_ptr->instance);
  if (ds_3gpp_rmsm_ati_get_inst_num_from_um_iface(um_iface_ptr) < DS_3GPP_RMSM_AT_UM_MAX_INSTANCES)
  {
    DS_3GPP_MSG0_LOW("iface is already in use");
    *ps_iface_ptr = NULL;
    modem_mem_free((void *)acl_policy_info, MODEM_MEM_CLIENT_DATA_CRIT );
    return FALSE;
  }
  
  *ps_iface_ptr = um_iface_ptr;
  modem_mem_free((void *)acl_policy_info, MODEM_MEM_CLIENT_DATA_CRIT );
  return TRUE;
} /* ds_3gpp_rmsm_ati_allocate_um_iface_ptr */


/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_ACTIVATE_V4_IFACE

  DESCRIPTION
    This function allocates a V4 iface and registers 
    for IFACE UP, IFACE DOWN, IFACE FLOW and PHYSLINK 
    FLOW events.
  
  RETURN VALUE
    FALSE  : if failure
    TRUE : if Success
  
  DEPENDENCIES
    None
    
  SIDE EFFECTS
    None
===========================================================================*/
LOCAL boolean  ds_3gpp_rmsm_ati_activate_v4_iface
( 
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
)
{
  ps_iface_type *ps_iface_ptr = NULL;
  int16  ps_iface_errno = DS_EWOULDBLOCK;
  ps_iface_event_info_u_type iface_event_info;
  boolean ret_val = FALSE;
  int result =0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( FALSE == ds_3gpp_rmsm_ati_allocate_um_iface_ptr(pdp_ptr,DS_UMTS_PDP_IPV4))
  {
    return FALSE;
  }

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(pdp_ptr))
  {
         return FALSE;
  } 

  memset( &iface_event_info, 0, sizeof(ps_iface_event_info_u_type) );

  ps_iface_ptr = pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr;

  DS_3GPP_MSG2_HIGH("Activate v4 iface [0x%x:%x]",
                      ps_iface_ptr->name,
                      ps_iface_ptr->instance);

  /*---------------------------------------------------------------------
    Register IFACE_UP and DOWN events with UM packet interface
  ---------------------------------------------------------------------*/
  if (pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr == NULL)
  {
    ret_val = ds_3gpp_iface_reg_ev(single_iface_ev_p,
                                   DS_3GPP_RMSM_SINGLE_IFACE_EV_COUNT,
                                   ds_3gpp_rmsm_ati_um_event_handler_cb,
                                   NULL,
                                   NULL);
    if(ret_val != TRUE)
    {
      return FALSE;
    }

  }

  /*---------------------------------------------------------------------
    Register FLOW ENABLED and DISABLED events with UM packet interface
  ---------------------------------------------------------------------*/

  ret_val = ds_3gpp_iface_reg_ev(iface_ev_p,
                                 DS_3GPP_RMSM_IFACE_EV_COUNT,
                                 ds_3gpp_rmsm_ati_um_event_handler_cb,
                                 ps_iface_ptr,
                                 NULL);
  if(!ret_val)
  {
    return FALSE;
  }


  /*---------------------------------------------------------------------
    Bring up the Um IFACE interface. If there is an error, 
    delete the allocated instance and return FALSE.
  ---------------------------------------------------------------------*/
  result = ps_iface_bring_up_cmd( ps_iface_ptr, 
                              &ps_iface_errno,
                              NULL );

  DS_3GPP_MSG2_LOW ("V4 UM i/f bring_up result [%d] ps_error %d ", 
                result, 
                ps_iface_errno);

  if ( result == 0 )
  {
    /* we need to make sure before that the context is already up or not */
    /* if already up , return OK */
    /* Post IFACE_UP_EV from here so that rmsm_at transitions to correct
       state
    */
    ds_3gpp_rmsm_ati_um_event_handler_cb( ps_iface_ptr,
                                          IFACE_UP_EV,
                                          iface_event_info,
                                          NULL );

    return TRUE;
  }

  if ( ps_iface_errno != DS_EWOULDBLOCK )
  {

    /* Return ERROR */
    (void)ds_3gpp_iface_dereg_ev(single_iface_ev_p,
                                 DS_3GPP_RMSM_SINGLE_IFACE_EV_COUNT,
                                 ds_3gpp_rmsm_ati_um_event_handler_cb,
                                 NULL);

    (void)ds_3gpp_iface_dereg_ev(iface_ev_p,
                                 DS_3GPP_RMSM_IFACE_EV_COUNT,
                                 ds_3gpp_rmsm_ati_um_event_handler_cb,
                                 ps_iface_ptr);
    
    return FALSE;
  }

  return TRUE;
}/* ds_3gpp_rmsm_ati_activate_v4_iface */

/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_ACTIVATE_V6_IFACE

  DESCRIPTION
    This function allocates a V6 iface and registers 
    for IFACE UP and IFACE DOWN events.
  
  RETURN VALUE
    FALSE  : if failure
    TRUE : if Success
  
  DEPENDENCIES
    None
    
  SIDE EFFECTS
    None
===========================================================================*/
LOCAL boolean  ds_3gpp_rmsm_ati_activate_v6_iface
( 
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
)
{
  ps_iface_type *ps_iface_ptr = NULL;
  int16  ps_iface_errno = DS_EWOULDBLOCK;
  ps_iface_event_info_u_type iface_event_info;
  boolean ret_val = FALSE;
  int result =0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (pdp_ptr == NULL)
  {
    return FALSE;
  }
  memset( &iface_event_info, 0, sizeof(ps_iface_event_info_u_type) );
  
  if( FALSE == ds_3gpp_rmsm_ati_allocate_um_iface_ptr(pdp_ptr,DS_UMTS_PDP_IPV6))
  {
    return FALSE;
  }

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(pdp_ptr))
  {
         return FALSE;
  } 

  ps_iface_ptr = pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr ;

  DS_3GPP_MSG2_HIGH("Activate v6 iface %d:%d",
                     ps_iface_ptr->name, ps_iface_ptr->instance);

  /*---------------------------------------------------------------------
    Register IFACE_UP and DOWN events with UM packet interface
  ---------------------------------------------------------------------*/
  if (pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr == NULL)
  {

    ret_val = ds_3gpp_iface_reg_ev(single_iface_ev_p,
                            DS_3GPP_RMSM_SINGLE_IFACE_EV_COUNT,
                            ds_3gpp_rmsm_ati_um_event_handler_cb,
                            NULL,
                            NULL);
    if(!ret_val)
    {
      return FALSE;
    }
  }
  /*---------------------------------------------------------------------
    Bring up the Um IFACE interface. If there is an error, 
    delete the allocated instance and return FALSE.
  ---------------------------------------------------------------------*/
  result = ps_iface_bring_up_cmd( ps_iface_ptr, 
                              &ps_iface_errno,
                              NULL );
  DS_3GPP_MSG2_LOW ("V6 UM i/f bring_up result [%d] ps_error %d ", 
                result, 
                ps_iface_errno);

  if ( result == 0 )
  {
    /* we need to make sure before that the context is already up or not */
    /* if already up , return OK */
    /* Post IFACE_UP_EV from here so that rmsm_at transitions to correct
       state
    */

    ds_3gpp_rmsm_ati_um_event_handler_cb( ps_iface_ptr,
                                          IFACE_UP_EV,
                                          iface_event_info,
                                          NULL );

    return TRUE;
  }

  if ( ps_iface_errno != DS_EWOULDBLOCK )
  {

    /* Return ERROR */
    if (pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr == NULL)
    {
	    ds_3gpp_iface_dereg_ev(single_iface_ev_p,
                             DS_3GPP_RMSM_SINGLE_IFACE_EV_COUNT,
                             ds_3gpp_rmsm_ati_um_event_handler_cb,
                             NULL);
    }
    return FALSE;
  }
  return TRUE;
}/* ds_3gpp_rmsm_ati_activate_v6_iface */



LOCAL boolean ds_3gpp_rmsm_ati_pdp_state_type_validate
(
    ds_3gpp_ati_pdp_state_type     *pdp_ptr
)
{
    boolean ret_val = FALSE;
  ds_3gpp_ati_pdp_state_type_dyn_s *ati_pdp_state_type_dyn_p = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(pdp_ptr == NULL)
  {
    return ret_val;
  }

  ati_pdp_state_type_dyn_p=pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p;

  if ((ati_pdp_state_type_dyn_p != NULL) && 
       pdp_ptr->in_use == TRUE)

  {
    ret_val = TRUE;
  }

  return ret_val;

}

/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_TEAR_DOWN_V4_IFACE

  DESCRIPTION
    This function tear down a V4 iface and de-registers 
    all IFACE and PHYSLINK events registered.
  
  RETURN VALUE
    FALSE  : if failure
    TRUE : if Success
  
  DEPENDENCIES
    None
    
  SIDE EFFECTS
    None
===========================================================================*/
LOCAL dsat_result_enum_type ds_3gpp_rmsm_ati_tear_down_v4_iface
(
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
)
{
  ds_3gpp_rmsm_at_instance_type  instance =0;
  int16  ps_iface_errno = DS_EWOULDBLOCK;
  dsat_result_enum_type result = DSAT_ASYNC_CMD;
  ps_iface_type*      um_iface_ptr = NULL;
  ps_iface_type*      v6_iface_ptr = NULL;
  int ps_result;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (pdp_ptr == NULL)
  {
    return DSAT_ERROR;
  }

   if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(pdp_ptr))
  {
    DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 

  if (pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr == NULL)
  {
    return DSAT_ERROR;
  }
  um_iface_ptr = pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr;
  instance =  ds_3gpp_rmsm_ati_get_inst_num_from_um_iface (um_iface_ptr);
  v6_iface_ptr = pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr;

  /*---------------------------------------------------------------------
    Tear down the v4 UM pkt iface first.
    Checking for the return code as the iface could already be down.
  ---------------------------------------------------------------------*/
  ps_result = ps_iface_tear_down_cmd ( um_iface_ptr, 
                                &ps_iface_errno,
                                NULL );
  DS_3GPP_MSG5_HIGH("V4 ps_iface [0x%x:%x] UM tear down result %d, err_no %d , v6_ptr %d",
                       um_iface_ptr->name,um_iface_ptr->instance, 
                       ps_result,ps_iface_errno, v6_iface_ptr); 
  
  if ( ( ps_result == 0 ) || (ps_iface_errno != DS_EWOULDBLOCK))
  {
    /*-------------------------------------------------------------------
       If the UM packet iface is already down, we should de-allocate
       the instance if not done yet and return OK to ATCoP immediately.
      Deregister all flow/physlink events with v4 UM packet interface
      and IFACE events only when v6 iface pointer is NULL
    ---------------------------------------------------------------------*/
   (void)ds_3gpp_iface_dereg_ev(iface_ev_p,
                                DS_3GPP_RMSM_IFACE_EV_COUNT,
                                ds_3gpp_rmsm_ati_um_event_handler_cb,
                                um_iface_ptr);  

    
   (void)ds_3gpp_phys_link_dereg_ev(phys_link_ev_p,
                                    DS_3GPP_RMSM_PHYS_LINK_EV_COUNT,
                                    ds_3gpp_rmsm_phys_link_um_event_handler_cb,
                                    PS_IFACE_GET_PHYS_LINK(um_iface_ptr));
    

    if( v6_iface_ptr == NULL)
    {

      (void)ds_3gpp_iface_dereg_ev(single_iface_ev_p,
                                   DS_3GPP_RMSM_SINGLE_IFACE_EV_COUNT,
	        				                 ds_3gpp_rmsm_ati_um_event_handler_cb,
                                   NULL); 
      /* De-register only if v6 iface pointer is NULL */

    if(IFACE_UP ==ps_iface_state(um_iface_ptr))
    {
      /* This might be a default profile ,return ERROR*/
      result = DSAT_ERROR;
    } 
    else
    {
      result = DSAT_OK;
    }
    pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr = NULL;
    /* de allocate the instance already brought up 
       only if v6 iface is NULL */
    ds_3gpp_rmsm_ati_deallocate_instance (instance);
    return result;
  }
    /* Waiting for v6 iface IFACE down event, return async cmd*/
    return DSAT_ASYNC_CMD;
  }
  else
  {
    /*-------------------------------------------------------------------
       Wait for IFACE_DOWN event and then send final response
       Change the state to deactivation state. 
    -------------------------------------------------------------------*/
    return DSAT_ASYNC_CMD;
  }

}/* ds_3gpp_rmsm_ati_tear_down_v4_iface */

/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_TEAR_DOWN_V6_IFACE

  DESCRIPTION
    This function tear down a V6 iface and de-registers 
    all IFACE events registered.
  
  RETURN VALUE
    FALSE  : if failure
    TRUE : if Success
  
  DEPENDENCIES
    None
    
  SIDE EFFECTS
    None
===========================================================================*/

LOCAL dsat_result_enum_type ds_3gpp_rmsm_ati_tear_down_v6_iface
(
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
)
{
  ds_3gpp_rmsm_at_instance_type  instance =0;
  int16  ps_iface_errno = DS_EWOULDBLOCK;
  dsat_result_enum_type result = DSAT_ASYNC_CMD;
  ps_iface_type*      um_iface_ptr = NULL;
  ps_iface_type*      v4_iface_ptr = NULL;
  int ps_result =0 ;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (pdp_ptr == NULL)
  {
    return DSAT_ERROR;
  }

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(pdp_ptr))
  {
    DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 

  if (pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr == NULL)
  {
    return DSAT_ERROR;
  }
  um_iface_ptr = pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr;
  v4_iface_ptr = pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr;
  instance =  ds_3gpp_rmsm_ati_get_inst_num_from_um_iface (um_iface_ptr);

  /*---------------------------------------------------------------------
    Tear down the v4 UM pkt iface first.
    Checking for the return code as the iface could already be down.
  ---------------------------------------------------------------------*/
  ps_result= ps_iface_tear_down_cmd ( um_iface_ptr, 
                                &ps_iface_errno,
                                NULL );
  
  DS_3GPP_MSG5_HIGH("V6 ps_iface [0x%x:%x] UM tear down result %d, err_no %d , v4_ptr %d",
                       um_iface_ptr->name,um_iface_ptr->instance, 
                       ps_result,ps_iface_errno, v4_iface_ptr);
  
  if (( ps_result == 0 ) || (ps_iface_errno != DS_EWOULDBLOCK) )
  {
    /*-------------------------------------------------------------------
       If the UM packet iface is already down, we should de-allocate
       the instance if not done yet and return OK to ATCoP immediately.
    -------------------------------------------------------------------*/
    DS_3GPP_MSG0_LOW ("UM IFACE is already down, deallocating IFACE instance ");

    /*---------------------------------------------------------------------
      Deregister all the events with UM packet interface
    ---------------------------------------------------------------------*/
    if(pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr == NULL)
    {

      (void)ds_3gpp_iface_dereg_ev(single_iface_ev_p,
                                   DS_3GPP_RMSM_SINGLE_IFACE_EV_COUNT,
                                   ds_3gpp_rmsm_ati_um_event_handler_cb,
                                   NULL); 

      if(IFACE_UP ==ps_iface_state(um_iface_ptr))
      {
        /* This might be a default profile ,return ERROR*/
        result = DSAT_ERROR;
      } 
      else
      {
        result = DSAT_OK;
      }
    }
    pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr = NULL;
    /* de allocate the instance already brought up 
       only if v4 iface is NULL */
    if(pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr == NULL)
    {
      ds_3gpp_rmsm_ati_deallocate_instance (instance);
    }
    else
    {
      /*-------------------------------------------------------------------
         Wait for IFACE_DOWN event and then send final response
      -------------------------------------------------------------------*/
      result= DSAT_ASYNC_CMD;

    }
    return result;
  }
  else
  {
    /*-------------------------------------------------------------------
       Wait for IFACE_DOWN event and then send final response
       Change the state to deactivation state. 
    -------------------------------------------------------------------*/
    return DSAT_ASYNC_CMD;
  }
}/* ds_3gpp_rmsm_ati_tear_down_v6_iface */


/*===========================================================================
FUNCTION   DS_3GPP_RMSM_ATI_ALLOCATE_INSTANCE

DESCRIPTION
  Returns the pointer to RMSM AT instance if present 
  
NOTE: THIS IS A RESTRICTED FUNCTION AND IS INTENDED TO BE USED BY 
DS_3GPP_AT MODULES ONLY


RETURN VALUE
  Pointer to the instance number:
              valid DS_3GPP_RMSM instance  if Success
              NULL if failure to allocate an instance

DEPENDENCIES
  ds_3gpp_ati_init_all_instances() should have been called first.

SIDE EFFECTS
  None
===========================================================================*/
LOCAL boolean  ds_3gpp_rmsm_ati_allocate_instance
(
  uint32 profile_number,
  sys_modem_as_id_e_type          subs_id,
  ds_3gpp_rmsm_at_instance_type*  instance
)
{
  ds_3gpp_rmsm_at_instance_type index = 0;
  boolean result = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if (NULL == instance)
  {
    return FALSE;
  }
  
  /* Check if an instance is already allocated for this profile number */
  *instance = ds_3gpp_rmsm_ati_get_inst_num_from_profile(profile_number, subs_id);
  if(DS_3GPP_RMSM_AT_UM_MAX_INSTANCES != *instance)
  {
    DS_3GPP_MSG1_LOW ("Already activated instance = %d",*instance);
    return TRUE;
  }

  for (index =0; (index <DS_3GPP_RMSM_AT_UM_MAX_INSTANCES); index ++)
  {
    if(ds_3gpp_ati_pdp_state[index].in_use == FALSE)
    {
      *instance = index;
      ds_3gpp_ati_pdp_state[index].in_use = TRUE;
      ds_3gpp_ati_pdp_state[index].ds_3gpp_ati_pdp_state_type_dyn_p=(ds_3gpp_ati_pdp_state_type_dyn_s *)modem_mem_alloc(
                    sizeof(ds_3gpp_ati_pdp_state_type_dyn_s),MODEM_MEM_CLIENT_DATA);

     memset((void *)ds_3gpp_ati_pdp_state[index].ds_3gpp_ati_pdp_state_type_dyn_p,0x0,sizeof(ds_3gpp_ati_pdp_state_type_dyn_s));
     ds_3gpp_ati_pdp_state[index].ds_3gpp_ati_pdp_state_type_dyn_p->subs_id = subs_id;

    /*-----------------------------------------------------------------------
      Assign a ps handle to the instance, this keeps track of whether or
      not the instance is valid to process the ps signal callback for rx
      data
    -----------------------------------------------------------------------*/
    ds_3gpp_ati_pdp_state[index].ds_3gpp_ati_pdp_state_type_dyn_p->ps_handle
      = ps_handle_mgr_get_handle(PS_HANDLE_MGR_CLIENT_3GPP_RMSM, index);

      result = TRUE;
      break;
    }
  }

  *instance = index;
  DS_3GPP_MSG2_LOW ("Allocated Instance = %d, result %d",index, result);
  return result;
} /* ds_3gpp_rmsm_ati_allocate_instance */

/*===========================================================================
FUNCTION   DS_3GPP_RMSM_ATI_DEALLOCATE_INSTANCE

DESCRIPTION
  This function de-allocates RMSM AT instance if present 
  
NOTE: THIS IS A RESTRICTED FUNCTION AND IS INTENDED TO BE USED BY 
DS_3GPP_AT MODULES ONLY

RETURN VALUE
  None

DEPENDENCIES
  ds_3gpp_ati_init_all_instances() should have been called first.

SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_rmsm_ati_deallocate_instance
(
  ds_3gpp_rmsm_at_instance_type  instance
)
{
  DS_3GPP_MSG1_HIGH("In ds_3gpp_rmsm_ati_deallocate_instance instance id %d",
                     instance);
  if ( DS_3GPP_RMSM_AT_UM_MAX_INSTANCES <= instance)
  {
    return;
  }
  
  DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_rmsm_crit_sect);
  
  if(ds_3gpp_ati_pdp_state[instance].in_use == TRUE)
  {
    ds_3gpp_ati_pdp_state[instance].in_use = FALSE;

    /* Invalidate the ps handle */
    ds_3gpp_ati_pdp_state[instance].ds_3gpp_ati_pdp_state_type_dyn_p->ps_handle
      = DS_3GPP_RMSM_AT_INVALID_HANDLE;

    modem_mem_free(ds_3gpp_ati_pdp_state[instance].ds_3gpp_ati_pdp_state_type_dyn_p, 
                                   MODEM_MEM_CLIENT_DATA);
    ds_3gpp_ati_pdp_state[instance].ds_3gpp_ati_pdp_state_type_dyn_p=NULL;
  }
  DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_rmsm_crit_sect);
}/* ds_3gpp_rmsm_ati_deallocate_instance */

/*===========================================================================
FUNCTION   DS_3GPP_RMSM_ATI_INIT_ALL_INSTANCES

DESCRIPTION
  This function initializes all RMSM AT instances 
  
NOTE: THIS IS A RESTRICTED FUNCTION AND IS INTENDED TO BE USED BY 
DS_3GPP_AT MODULES ONLY


RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void  ds_3gpp_rmsm_ati_init_all_instances( void )
{
  int i =0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Initialized the state information for each instance of RMSM AT
  -------------------------------------------------------------------------*/
  for (i=0; i<DS_3GPP_RMSM_AT_UM_MAX_INSTANCES; i++)
  {
    memset(&ds_3gpp_ati_pdp_state[i],0x0,sizeof(ds_3gpp_ati_pdp_state_type));
    ds_3gpp_ati_pdp_state[i].in_use = FALSE;

  }/* end of for loop*/

  /*-------------------------------------------------------------------------
    Initialize ps handles for RMSM
  -------------------------------------------------------------------------*/
  ps_handle_mgr_init_client(PS_HANDLE_MGR_CLIENT_3GPP_RMSM,
                            DS_3GPP_RMSM_AT_UM_MAX_INSTANCES,
                            DS_3GPP_RMSM_AT_MIN_HANDLE,
                            DS_3GPP_RMSM_AT_MAX_HANDLE);
  DS_3GPP_INIT_CRIT_SECTION(&ds_3gpp_rmsm_crit_sect);
}/* ds_3gpp_rmsm_ati_init_all_instances */

/*===========================================================================
FUNCTION   DS_3GPP_RMSM_ATI_GET_INST_NUM_FROM_FLOW_PTR

DESCRIPTION
  This function returns the RMSM AT instance from Um Iface pointer if present 

RETURN VALUE
  The instance number:
     valid DS_3GPP_RMSM instance  if Success
     DS_3GPP_RMSM_AT_UM_MAX_INSTANCES if failure to allocate an instance

DEPENDENCIES
  ds_3gpp_ati_init_all_instances() should have been called first.

SIDE EFFECTS
  None
===========================================================================*/
LOCAL ds_3gpp_rmsm_at_instance_type ds_3gpp_rmsm_ati_get_inst_num_from_flow_ptr
(
  const ps_flow_type* flow_ptr
)
{
  int i;
  ds_3gpp_rmsm_at_instance_type instance = DS_3GPP_RMSM_AT_UM_MAX_INSTANCES;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( NULL == flow_ptr)
  {
    return instance;
  }

  for (i=0; i<DS_3GPP_RMSM_AT_UM_MAX_INSTANCES; i++)
  {
    /* Return Instance if the iface pointer matches 
       with either the v4 or v6 iface pointer
    */
    if ( (ds_3gpp_ati_pdp_state[i].in_use == TRUE)  &&
         (ds_3gpp_ati_pdp_state[i].ds_3gpp_ati_pdp_state_type_dyn_p->flow_ptr == flow_ptr) )
    {
      instance = i;
      break;
    }
  }
  return instance;
}/* ds_3gpp_rmsm_ati_get_inst_num_from_flow_ptr */
/*===========================================================================
FUNCTION   DS_3GPP_RMSM_ATI_GET_INST_NUM_FROM_UM_IFACE

DESCRIPTION
  This function returns the RMSM AT instance from Um Iface pointer if present 

RETURN VALUE
  The instance number:
     valid DS_3GPP_RMSM instance  if Success
     DS_3GPP_RMSM_AT_UM_MAX_INSTANCES if failure to allocate an instance

DEPENDENCIES
  ds_3gpp_ati_init_all_instances() should have been called first.

SIDE EFFECTS
  None
===========================================================================*/

ds_3gpp_rmsm_at_instance_type ds_3gpp_rmsm_ati_get_inst_num_from_um_iface
(
  const ps_iface_type* um_iface_ptr
)
{
  int i;
  ds_3gpp_rmsm_at_instance_type instance = DS_3GPP_RMSM_AT_UM_MAX_INSTANCES;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( NULL == um_iface_ptr)
  {
    return NULL;
  }

  for (i=0; i<DS_3GPP_RMSM_AT_UM_MAX_INSTANCES; i++)
  {
    /* Return Instance if the iface pointer matches 
       with either the v4 or v6 iface pointer
    */
    if ( (ds_3gpp_ati_pdp_state[i].in_use == TRUE)  &&
         ( (ds_3gpp_ati_pdp_state[i].ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr == um_iface_ptr) ||
           (ds_3gpp_ati_pdp_state[i].ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr == um_iface_ptr) ))
    {
      instance = i;
      break;
    }
  }
  return instance;
}/* ds_3gpp_rmsm_ati_get_inst_num_from_um_iface */

/*===========================================================================
FUNCTION   DS_3GPP_RMSM_ATI_GET_INST_NUM_FROM_PROFILE

DESCRIPTION
  This function returns the RMSM AT instance from profile number if present 
  
NOTE: THIS IS A RESTRICTED FUNCTION AND IS INTENDED TO BE USED BY 
DSUMTS_AT MODULES ONLY

RETURN VALUE
  The instance number:
     valid DS_3GPP_RMSM instance  if Success
     DS_3GPP_RMSM_AT_UM_MAX_INSTANCES if failure to allocate an instance

DEPENDENCIES
  ds_3gpp_ati_init_all_instances() should have been called first.

SIDE EFFECTS
  None
===========================================================================*/
ds_3gpp_rmsm_at_instance_type ds_3gpp_rmsm_ati_get_inst_num_from_profile
(
  uint32 profile_number,
  sys_modem_as_id_e_type  subs_id 
)
{
  int i;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ds_3gpp_rmsm_at_instance_type instance = DS_3GPP_RMSM_AT_UM_MAX_INSTANCES;

  for (i=0; i<DS_3GPP_RMSM_AT_UM_MAX_INSTANCES; i++)
  {
    if ( (ds_3gpp_ati_pdp_state[i].in_use == TRUE)  &&
         (ds_3gpp_ati_pdp_state[i].ds_3gpp_ati_pdp_state_type_dyn_p->profile_id == profile_number)&&
         (ds_3gpp_ati_pdp_state[i].ds_3gpp_ati_pdp_state_type_dyn_p->subs_id == subs_id))
    {
      instance = i;
      break;
    }
  }
  return instance;
}/* ds_3gpp_rmsm_ati_get_inst_num_from_um_iface */

/*===========================================================================
FUNCTION   DS_3GPP_RMSM_ATI_GET_V4_UM_IFACE_STATE

DESCRIPTION
  This function returns the RMSM AT instance from v4 Um Iface pointer if present 
  
NOTE: THIS IS A RESTRICTED FUNCTION AND IS INTENDED TO BE USED BY 
DSUMTS_AT MODULES ONLY

RETURN VALUE
  PS IFACE STATE

DEPENDENCIES
  ds_3gpp_ati_init_all_instances() should have been called first.

SIDE EFFECTS
  None
===========================================================================*/
ps_iface_state_enum_type ds_3gpp_rmsm_ati_get_v4_um_iface_state
(
  ds_3gpp_rmsm_at_instance_type instance
)
{
  ps_iface_state_enum_type iface_state = IFACE_STATE_INVALID;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(&ds_3gpp_ati_pdp_state[instance]))
  {
    DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 

  iface_state = ps_iface_state(ds_3gpp_ati_pdp_state[instance].\
                            ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr);
  DS_3GPP_MSG1_LOW ("V4 Iface state = %d",(int)iface_state);
  
  return iface_state;
}/* ds_3gpp_rmsm_ati_get_v4_um_iface_state */

/*===========================================================================
FUNCTION   DS_3GPP_RMSM_ATI_GET_V6_UM_IFACE_STATE

DESCRIPTION
  This function returns the RMSM AT instance from v6 Um Iface pointer if present 
  
NOTE: THIS IS A RESTRICTED FUNCTION AND IS INTENDED TO BE USED BY 
DSUMTS_AT MODULES ONLY

RETURN VALUE
  PS IFACE STATE

DEPENDENCIES
  ds_3gpp_ati_init_all_instances() should have been called first.

SIDE EFFECTS
  None
===========================================================================*/
ps_iface_state_enum_type ds_3gpp_rmsm_ati_get_v6_um_iface_state
(
  ds_3gpp_rmsm_at_instance_type instance
)
{
  ps_iface_state_enum_type iface_state = IFACE_STATE_INVALID;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(&ds_3gpp_ati_pdp_state[instance]))
  {
    DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 


  iface_state = ps_iface_state(ds_3gpp_ati_pdp_state[instance].ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr);
  DS_3GPP_MSG1_LOW ("v6 Iface state = %d",(int)iface_state);
  
  return iface_state;
}/* ds_3gpp_rmsm_ati_get_v6_um_iface_state */

/*===========================================================================
  FUNCTION DS_3GPP_RMSM_GET_V4_IFACE_PTR
  
  DESCRIPTION   
    Given the call instance, returns the v4 ps iface_ptr.
  
  DEPENDENCIES  
    None.
  
  RETURN VALUE  
    See Descritpion.
    
  SIDE EFFECTS  
    None.
  ===========================================================================*/
ps_iface_type* ds_3gpp_rmsm_get_v4_iface_ptr
(
  ds_3gpp_rmsm_at_instance_type instance
)
{
  if (DS_3GPP_RMSM_AT_UM_MAX_INSTANCES == instance )
  {
    return NULL;
  }

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(&ds_3gpp_ati_pdp_state[instance]))
  {
     return NULL;
  } 


  return ds_3gpp_ati_pdp_state[instance].ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr;
} /* ds_3gpp_rmsm_get_v4_iface_ptr */

/*===========================================================================
  FUNCTION DS_3GPP_RMSM_GET_V6_IFACE_PTR
  
  DESCRIPTION   
    Given the call instance, returns the v6 ps iface_ptr.
  
  DEPENDENCIES  
    None.
  
  RETURN VALUE  
    See Descritpion.
    
  SIDE EFFECTS  
    None.
  ===========================================================================*/
ps_iface_type* ds_3gpp_rmsm_get_v6_iface_ptr
(
  ds_3gpp_rmsm_at_instance_type instance
)
{
  if (DS_3GPP_RMSM_AT_UM_MAX_INSTANCES == instance )
  {
    return NULL;
  }
  
  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(&ds_3gpp_ati_pdp_state[instance]))
  {
    return NULL;
  } 

  return ds_3gpp_ati_pdp_state[instance].ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr;
} /* ds_3gpp_rmsm_get_v4_iface_ptr */

/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_GET_FLOW_PTR
  
  DESCRIPTION   
    Given the call instance, returns the flow_ptr.
  
  DEPENDENCIES  
    None.
  
  RETURN VALUE  
    See Description.
    
  SIDE EFFECTS  
    None.
  ===========================================================================*/
ps_flow_type* ds_3gpp_rmsm_ati_get_flow_ptr
(
  ds_3gpp_rmsm_at_instance_type instance
)
{
  if (DS_3GPP_RMSM_AT_UM_MAX_INSTANCES == instance )
  {
    return NULL;
  }

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(&ds_3gpp_ati_pdp_state[instance]))
  {
    DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 

  return ds_3gpp_ati_pdp_state[instance].ds_3gpp_ati_pdp_state_type_dyn_p->flow_ptr;
} /* ds_3gpp_rmsm_ati_get_flow_ptr */

/*===========================================================================
FUNCTION   DS_3GPP_RMSM_ATI_UM_EVENT_HANDLER_CB

DESCRIPTION
  This callback function is called when one of the registered ps_iface events
  occur on the UM_PKT_IFACE.  Registraion is done for various events based
  upon the call type during call setup. As part of event handling
  appropriate event is posted to DS_3GPP_RMSM_AT.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
/*ARGSUSED*/
void ds_3gpp_rmsm_ati_um_event_handler_cb
(
  ps_iface_type             *this_iface_ptr,
  ps_iface_event_enum_type   event,
  ps_iface_event_info_u_type event_info,
  void                      *user_data_ptr
)
{
  ds_3gpp_rmsm_at_instance_type   instance = DS_3GPP_RMSM_AT_UM_MAX_INSTANCES;
  boolean                         post_event = FALSE;
  ps_phys_link_type              *phys_link_ptr;
  ds_3gpp_rmsm_at_type            rmsm_at_data = {0};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  instance = ds_3gpp_rmsm_ati_get_inst_num_from_um_iface(this_iface_ptr);

  if ( DS_3GPP_RMSM_AT_UM_MAX_INSTANCES <= instance)
  {
    /* We are registering with NULL handler 
       so we recieve events for all IFACE's, return for invalid
    */
    return;
  }
  DS_3GPP_MSG3_HIGH( "Recd event %d for iface 0x%x RMSM inst", 
                     event, this_iface_ptr, instance);

  /*-------------------------------------------------------------------------
    Process flow control events first since they may get posted during data
    transfer
  -------------------------------------------------------------------------*/

  switch( event )
  {
    case IFACE_DOWN_EV:
      if( event_info.iface_down_info.state != IFACE_DOWN )
      {
        post_event= TRUE;
      }
      break;

    case IFACE_UP_EV:
      post_event = TRUE;
      break;

    default:
      break;

    case IFACE_FLOW_ENABLED_EV:
      /*---------------------------------------------------------------------
        Set signal to PS if data is pending to resume data transfer.
      ---------------------------------------------------------------------*/
      phys_link_ptr = PS_IFACE_GET_PHYS_LINK(this_iface_ptr);

      if ( ( PS_PHYS_LINK_FLOW_ENABLED( phys_link_ptr ) )&&
           ( DS_3GPP_RMSM_GET_PENDING_DATA(instance) >0) )
      {
        DS_3GPP_RMSM_SET_FLOW_STATE(instance,TRUE);
        /*---------------------------------------------------------------------
         Send signal to PS to send data to UM IFACE
        ---------------------------------------------------------------------*/
        PS_SET_SIGNAL( PS_3GPP_RMSM_AT_UM_RX_Q_SIGNAL );
      }
      break;

    case IFACE_FLOW_DISABLED_EV:
      /*---------------------------------------------------------------------
        We don't need to do anything when UM flow is disabled, transfer 
        function will stop sending data based on this flag.
      ---------------------------------------------------------------------*/
      if ( DS_3GPP_RMSM_GET_PENDING_DATA(instance) >0)
      {
        /* clear the signal if already Set */
        PS_CLR_SIGNAL(PS_3GPP_RMSM_AT_UM_RX_Q_SIGNAL);
      }
      DS_3GPP_RMSM_SET_FLOW_STATE(instance,FALSE);
      break;

  } /* switch( event ) */ 
  if(post_event == FALSE)
  {
    return;
  }
  
  /* Construct rmsm_at_data structure */
  rmsm_at_data.event = event;
  rmsm_at_data.ds_3gpp_rmsm_at_instance = instance;
  memscpy( &(rmsm_at_data.event_info),
           sizeof(rmsm_at_data.event_info),
           &event_info,
           sizeof(event_info));

  rmsm_at_data.iface_ptr = this_iface_ptr;
  
  /* invoke the event handler */
  ds_3gpp_rmsm_at_um_iface_evt_hdlr(&rmsm_at_data);
}/* ds_3gpp_rmsm_ati_um_event_handler_cb */


/*===========================================================================
FUNCTION   DS_3GPP_RMSM_FLOW_EVENT_HANDLER_CB()

DESCRIPTION
  This callback function is called when one of the registered ps_flow 
  events occur on the UM_PKT_IFACE.  Registraion is done for various events 
  based upon the call type during call setup. As part of event handling
  appropriate event is posted to DS_3GPP_RMSM.

DEPENDENCIES
  ds_3gpp_rmsm_ati_init_all_instances() should have been called first.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
/* ARGSUSED */
LOCAL void ds_3gpp_rmsm_flow_event_handler_cb
(
  ps_flow_type                * flow_ptr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    event_info,
  void                        * user_data_ptr
)
{
  ds_cmd_type                    *cmd_ptr = NULL;
  ds_3gpp_rmsm_at_flow_type      *data_ptr = NULL;
  ds_3gpp_rmsm_at_instance_type   instance = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (user_data_ptr == NULL)
  {
    return;
  }
  
  instance = ds_3gpp_rmsm_ati_get_inst_num_from_flow_ptr(flow_ptr);
  if((instance < 0 )|| (instance >= DS_3GPP_RMSM_AT_UM_MAX_INSTANCES))
  {
    return;
  }
  DS_3GPP_MSG3_MED( "Recd event %d for flow ptr 0x%x, RMS inst", 
                    event, flow_ptr, instance);

  /*-------------------------------------------------------------------------
    Process flow control events.
  -------------------------------------------------------------------------*/
  switch( event )
  {
    case FLOW_ACTIVATED_EV:
    case FLOW_NULL_EV:
    case FLOW_MODIFY_ACCEPTED_EV:
    case FLOW_MODIFY_REJECTED_EV:
      cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_rmsm_at_flow_type));
      if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
      {
        return;
      }
      
      cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_RMSM_AT_FLOW;
      data_ptr = (ds_3gpp_rmsm_at_flow_type*)cmd_ptr->cmd_payload_ptr;
      data_ptr->event = event;
      memscpy( &(data_ptr->event_info),
               sizeof(data_ptr->event_info),
               &event_info,
               sizeof(event_info));

      data_ptr->ds_3gpp_rmsm_at_instance = instance;
      data_ptr->flow_ptr = flow_ptr;
      ds_put_cmd( cmd_ptr);
      break;
    default:
    
      break;

  } /* switch( event ) */

}/* ds_3gpp_rmsm_flow_event_handler_cb */


/*===========================================================================
FUNCTION   DS_3GPP_RMSM_PHYS_LINK_UM_EVENT_HANDLER_CB()

DESCRIPTION
  This callback function is called when one of the registered ps_phys_link 
  events occur on the UM_PKT_IFACE.  Registraion is done for various events 
  based upon the call type during call setup. As part of event handling
  appropriate event is posted to DS_3GPP_RMSM.

DEPENDENCIES
  ds_3gpp_rmsm_ati_init_all_instances() should have been called first.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
/* ARGSUSED */
LOCAL void ds_3gpp_rmsm_phys_link_um_event_handler_cb
(
  ps_phys_link_type          *this_phys_link_ptr,
  ps_iface_event_enum_type   event,
  ps_iface_event_info_u_type event_info,
  void                      *user_data_ptr
)
{
  ds_3gpp_rmsm_at_instance_type instance = 0;
  ds_3gpp_ati_pdp_state_type *pdp_state_ptr = NULL;
  ps_iface_type             *iface_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if (user_data_ptr == NULL)
  {
    return;
  }
  pdp_state_ptr = user_data_ptr;

  if(pdp_state_ptr->in_use == FALSE)
  {
    return;
  }
  
  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(pdp_state_ptr))
  {
    return;
  } 

  iface_ptr =  pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr;

  if(iface_ptr == NULL)
  {
    return;
  }

  instance = ds_3gpp_rmsm_ati_get_inst_num_from_um_iface(iface_ptr);
  if ((instance < 0 )|| (instance >= DS_3GPP_RMSM_AT_UM_MAX_INSTANCES))
  {
    return;
  }

  DS_3GPP_MSG3_MED( "Recd event %d for phys link 0x%x for v4 iface, RMSM inst", 
                    event, this_phys_link_ptr,instance);

  /*-------------------------------------------------------------------------
    Process flow control events first since they may get posted during data
    transfer
  -------------------------------------------------------------------------*/

  switch( event )
  {
    case PHYS_LINK_FLOW_ENABLED_EV:
      /*---------------------------------------------------------------------
        Set signal to PS if data is pending to resume data transfer.
      ---------------------------------------------------------------------*/
      if( PS_IFACE_FLOW_ENABLED(iface_ptr) )
      {
        DS_3GPP_RMSM_SET_FLOW_STATE(instance,TRUE);
        if ( DS_3GPP_RMSM_GET_PENDING_DATA(instance) >0)
        {
          /*---------------------------------------------------------------------
           Send signal to PS to send data to UM IFACE
          ---------------------------------------------------------------------*/
          PS_SET_SIGNAL( PS_3GPP_RMSM_AT_UM_RX_Q_SIGNAL );
        }
      }
      break;

    case PHYS_LINK_FLOW_DISABLED_EV:
      /*---------------------------------------------------------------------
        We don't need to do anything when UM flow is disabled, transfer 
        function will stop sending data based on this flag.
      ---------------------------------------------------------------------*/
      DS_3GPP_RMSM_SET_FLOW_STATE(instance,FALSE);

      if ( DS_3GPP_RMSM_GET_PENDING_DATA(instance) >0)
      {
        /* clear the signal if already Set */
        PS_CLR_SIGNAL(PS_3GPP_RMSM_AT_UM_RX_Q_SIGNAL);
      }
      break;

    case PHYS_LINK_GONE_EV:
      if(this_phys_link_ptr != pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_phys_link_ptr)
      {
        return;
      }
      if(pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_phys_link_ptr != NULL)
      {
        (void)ds_3gpp_phys_link_dereg_ev(phys_link_ev_p,
                                         DS_3GPP_RMSM_PHYS_LINK_EV_COUNT,
                                         ds_3gpp_rmsm_phys_link_um_event_handler_cb,
                                         pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_phys_link_ptr);

        pdp_state_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_phys_link_ptr = NULL;
      }
      break;

    default:
      break;
  } /* switch( event ) */

}/* ds_3gpp_rmsm_phys_link_um_event_handler_cb */



/*===========================================================================
  FUNCTION DS_3GPP_RMSM_AT_PROCESS_CMD()

  DESCRIPTION
    This function receives a cmd posted to DS_3GPP_RMSM AT interface 
    in DS task and passes it to the appropriate handling.

  RETURN VALUE
    ds_cmd_ptr: Cmd buffer passed with the cmd

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void ds_3gpp_rmsm_at_process_cmd
(
  ds_cmd_type *ds_cmd_ptr
)
{
  ds_cmd_enum_type cmd;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (NULL == ds_cmd_ptr)
  {
    return;
  }

  cmd  = ds_cmd_ptr->hdr.cmd_id;
  switch(cmd)
  {
    case DS_CMD_3GPP_RMSM_AT_SEND_DATA:
      ds_3gpp_rmsm_at_data_cmd_hdlr(ds_cmd_ptr);
      break;

    case DS_CMD_3GPP_RMSM_AT_FLOW:
      ds_3gpp_rmsm_at_flow_evt_hdlr(ds_cmd_ptr);
      break;

      default:
        break;
  }

} /* ds_3gpp_rmsm_at_process_cmd */
/*===========================================================================
  FUNCTION DS_3GPP_RMSM_AT_UM_IFACE_EVT_HDLR()

  DESCRIPTION
    This function process IFACE_UP, IFACE_DOWN
    events

  RETURN VALUE
    ds_cmd_ptr: Cmd buffer passed with the cmd

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
LOCAL void ds_3gpp_rmsm_at_um_iface_evt_hdlr
(
  ds_3gpp_rmsm_at_type           *rmsm_at_ptr
)
{
  ds_3gpp_rmsm_at_instance_type   instance=0;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  instance = rmsm_at_ptr->ds_3gpp_rmsm_at_instance;

  DS_3GPP_MSG3_HIGH( "Recd event %d for RMSM instance %d iface_ptr 0x%x",
                      rmsm_at_ptr->event, instance,
                      rmsm_at_ptr->iface_ptr);

  switch(rmsm_at_ptr->event)
  {
    case  IFACE_DOWN_EV:
      ds_3gpp_rmsm_ati_um_iface_down_handler(instance,
                                             rmsm_at_ptr->event_info,
                                             rmsm_at_ptr->iface_ptr);
      break;

    case  IFACE_UP_EV:
      ds_3gpp_rmsm_ati_um_iface_up_handler(instance,
                                           rmsm_at_ptr->event_info,
                                           rmsm_at_ptr->iface_ptr);
      break;

    default:
     break;
  }

}/* ds_3gpp_rmsm_at_um_iface_evt_hdlr */

/*===========================================================================
  FUNCTION DS_3GPP_RMSM_AT_DATA_CMD_HDLR

  DESCRIPTION
    This function process data sending command handler.

  RETURN VALUE
    ds_cmd_ptr: Cmd buffer passed with the cmd

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
LOCAL void ds_3gpp_rmsm_at_data_cmd_hdlr
(
  ds_cmd_type *ds_cmd_ptr
)
{
  ds_3gpp_rmsm_at_instance_type  instance = DS_3GPP_RMSM_AT_UM_MAX_INSTANCES;
  ds_3gpp_ati_pdp_state_type    *info_ptr = NULL;
  dsat_rmsm_info_s_type          dsat_rmsm_info;
  ds_3gpp_rmsm_at_type          *rmsm_at_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if ((NULL == ds_cmd_ptr) || (NULL == ds_cmd_ptr->cmd_payload_ptr))
  {
    return;
  }
  
  rmsm_at_ptr = (ds_3gpp_rmsm_at_type*)ds_cmd_ptr->cmd_payload_ptr;

  instance = rmsm_at_ptr->ds_3gpp_rmsm_at_instance;
  if((instance < 0 )|| (instance >= DS_3GPP_RMSM_AT_UM_MAX_INSTANCES))
  {
    return;
  }
  info_ptr = ds_3gpp_rmsm_get_info_ptr_from_instance(instance);

  if( NULL == info_ptr )
  {
    DS_3GPP_MSG0_LOW ("RMSM inst already freed,return ERROR with NULL user_info");
    /* Send final response to ATCoP */
    dsat_rmsm_info.result = DSAT_ERROR;
    dsat_rmsm_info.reason_type = DSAT_RMSM_INVALID_REASON;
    dsat_rmsm_info.user_info_ptr = NULL;
    dsat_rmsm_done_handler(DSAT_RMSM_QCDGEN_CMD,dsat_rmsm_info);
    return;
  }

    if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(info_ptr))
  {
    DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 

    if (info_ptr->in_use != TRUE)
    {
       return;
    }

  /* check if any pending data for transfer */
  if(info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->pending_data !=0)
  {
    /*---------------------------------------------------------------------
     Send signal to PS only if flow is enabled
    ---------------------------------------------------------------------*/
    if(info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->um_flow_enabled == TRUE )
    {
      DS_3GPP_MSG0_LOW ("settng signal ");
      PS_SET_SIGNAL( PS_3GPP_RMSM_AT_UM_RX_Q_SIGNAL );
    }
    return;
  }

  DS_3GPP_MSG1_LOW ("Data transfer is complete, return result %d",
                (int)rmsm_at_ptr->result);
  /* Send final response to ATCoP */
  if(rmsm_at_ptr->result == TRUE)
  {
    dsat_rmsm_info.result = DSAT_OK;
  }
  else
  {
    dsat_rmsm_info.result = DSAT_ERROR;
  }
  dsat_rmsm_info.reason_type = DSAT_RMSM_INVALID_REASON;
  dsat_rmsm_info.user_info_ptr = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr;
  dsat_rmsm_done_handler(DSAT_RMSM_QCDGEN_CMD,dsat_rmsm_info);
  /* clear user info pointer */
  info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr= NULL;

}/* ds_3gpp_rmsm_at_data_cmd_hdlr */

/*===========================================================================
  FUNCTION DS_3GPP_RMSM_AT_FLOW_EVT_HDLR

  DESCRIPTION
    This function process flow events handler.

  RETURN VALUE
    ds_cmd_ptr: Cmd buffer passed with the cmd

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
LOCAL void ds_3gpp_rmsm_at_flow_evt_hdlr
(
  ds_cmd_type *ds_cmd_ptr
)
{
  ds_3gpp_rmsm_at_instance_type instance=0;
  ps_flow_type                 *flow_ptr;
  ps_flow_type                 *assoc_flow_ptr = NULL;
  ds_3gpp_ati_pdp_state_type   *info_ptr;
  dsat_rmsm_info_s_type         dsat_rmsm_info;
  dsat_result_enum_type         result = DSAT_ASYNC_CMD;
  dsat_cgerep_evt_info_s_type   pkt_evt_info = {0};
  ds_3gpp_rmsm_at_flow_type    *rmsm_at_flow_info_ptr = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if ((ds_cmd_ptr == NULL) || (ds_cmd_ptr->cmd_payload_ptr == NULL))
  {
    return;
  }

  rmsm_at_flow_info_ptr = (ds_3gpp_rmsm_at_flow_type*)ds_cmd_ptr->cmd_payload_ptr;

  instance = rmsm_at_flow_info_ptr->ds_3gpp_rmsm_at_instance;
  flow_ptr = rmsm_at_flow_info_ptr->flow_ptr;
  
  if((instance < 0 )|| (instance >= DS_3GPP_RMSM_AT_UM_MAX_INSTANCES))
  {
    return;
  }
  if ( flow_ptr == NULL)
  {
    return;
  }

  info_ptr=ds_3gpp_rmsm_get_info_ptr_from_instance(instance);
  
  if(NULL == info_ptr)
  {
    return;
  }

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(info_ptr))
  {
         return;
  } 
  DS_3GPP_MSG2_HIGH( "Recd Flow event %d for RMSM instance %d",
                     rmsm_at_flow_info_ptr->event, instance);

  /* Populate event_info */
  pkt_evt_info.cid  = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id;
  pkt_evt_info.cm_subs_id = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->subs_id;
  pkt_evt_info.p_cid = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.primary_id;
  pkt_evt_info.evt_type  = DS_3GPP_RMSM_ATI_PKT_EVT_TYPE_INFO;

  switch(rmsm_at_flow_info_ptr->event)
  {
    case  FLOW_ACTIVATED_EV:
      /* get CGACT state */
      if( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state == DS_3GPP_ATIF_PDP_ACTIVATING )
      {
        /* Get the associated flow pointer */
        assoc_flow_ptr = PS_FLOW_GET_ASSOC_PS_FLOW(flow_ptr);
        if( assoc_flow_ptr == NULL )
        {
          /* Set the associated flow pointer to flow pointer.
             In case of context not activated by +CGACT,
             flow pointer will not be associated flow ptr.
          */
          assoc_flow_ptr = flow_ptr;
        }
        /* update the profile number in flow context*/
        if(( assoc_flow_ptr == NULL ) ||
           ( FALSE == ds_flow_context_update_dynamic_cid(assoc_flow_ptr,
                                                      (info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id) )))
        {
          DS_3GPP_MSG0_LOW ("Failed to update the profile id");
        }
        info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state = DS_3GPP_ATIF_PDP_ACTIVATED;
        /* Call ATCoP complete handler ,with OK */
        dsat_rmsm_info.result = DSAT_OK;
        dsat_rmsm_info.user_info_ptr = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr;
        dsat_rmsm_info.reason_type = DSAT_RMSM_INVALID_REASON;
        dsat_rmsm_done_handler(DSAT_RMSM_CGACT_CMD,dsat_rmsm_info);
        /* Clear the user infor pointer for activation */
        info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr = NULL;

        /* send Packet event reporting */
        dsat_rmsm_pkt_event_rpt_handler(DSAT_MS_ACTIVATE,&pkt_evt_info);
      }
      break;
    case  FLOW_NULL_EV:
      if( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state == DS_3GPP_ATIF_PDP_ACTIVATING )
      {
      /* Call ATCoP complete handler ,with ERROR */
        result = DSAT_ERROR;
      }
      else if( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state == DS_3GPP_ATIF_PDP_DEACTIVATING )
      {
        /* Call ATCoP complete handler ,with OK */
        result = DSAT_OK;
      }
      /* check if MODIFY is pending if so return ERROR and 
         de-reg the call backs 
      */
      if( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->modify_pending == TRUE )
      {
        /* Call ATCoP complete handler ,with ERROR */
        dsat_rmsm_info.result = DSAT_ERROR;
        dsat_rmsm_info.user_info_ptr = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr;
        dsat_rmsm_info.reason_type = DSAT_RMSM_INVALID_REASON;  
        dsat_rmsm_done_handler(DSAT_RMSM_CGCMOD_CMD,dsat_rmsm_info);
        /* Clear the user info pointer for de-activation */
        info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr = NULL;
        
        /* de-register all MODIFY handlers */
       (void)ds_3gpp_flow_dereg_ev(flow_ev_mod_p,
                                   DS_3GPP_RMSM_FLOW_MOD_EV_COUNT,
                                   ds_3gpp_rmsm_flow_event_handler_cb,
                                   NULL);
    }
      else if(result != DSAT_ASYNC_CMD)
      {
        /* Call ATCoP complete handler ,with result */
        dsat_rmsm_info.result = result;
        dsat_rmsm_info.user_info_ptr = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr;
        dsat_rmsm_info.reason_type = DSAT_RMSM_EXT_INFO_CODE;
        dsat_rmsm_info.reason.ext_info_code
          = rmsm_at_flow_info_ptr->event_info.flow_event_info.info_code;
        dsat_rmsm_done_handler(DSAT_RMSM_CGACT_CMD,dsat_rmsm_info);
        /* Clear the user infor pointer for de-activation */
        info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr = NULL;

        /* send Packet event reporting */
        dsat_rmsm_pkt_event_rpt_handler(DSAT_MS_DEACTIVATE,&pkt_evt_info);
      }
      /* Deregister all QOS flow event's */
      (void)ds_3gpp_flow_dereg_ev(flow_ev_p,
                                  DS_3GPP_RMSM_FLOW_EV_COUNT,
                                  ds_3gpp_rmsm_flow_event_handler_cb,
                                  NULL);
                               
      /* de-allocate the instance */
      ds_3gpp_rmsm_ati_deallocate_instance (instance);
      break;
    case FLOW_MODIFY_ACCEPTED_EV:
    case FLOW_MODIFY_REJECTED_EV:
      /* check if MODIFY is pending if so return ERROR/OK and 
         de-reg the call backs 
      */
      if(rmsm_at_flow_info_ptr->event == FLOW_MODIFY_ACCEPTED_EV)
      {
        result = DSAT_OK;
        /* send Packet event reporting */
        pkt_evt_info.change_reason = DS_3GPP_RMSM_ATI_PKT_EVT_BOTH_TFT_QOS_CHANGE;
        dsat_rmsm_pkt_event_rpt_handler(DSAT_MS_MODIFY,&pkt_evt_info);
      }
      else
      {
        result = DSAT_ERROR;
      }
      if( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->modify_pending == TRUE )
      {
        info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->modify_pending = FALSE;
        /* Call ATCoP complete handler ,with OK */
        dsat_rmsm_info.result = result;
        dsat_rmsm_info.reason_type = DSAT_RMSM_INVALID_REASON;    
        dsat_rmsm_info.user_info_ptr = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr;
        dsat_rmsm_done_handler(DSAT_RMSM_CGCMOD_CMD,dsat_rmsm_info);
        /* Clear the user infor pointer for activation */
        info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr = NULL;

        /* de-register all MODIFY handlers */
        PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
               /* de-register all MODIFY handlers */
       (void)ds_3gpp_flow_dereg_ev(flow_ev_mod_p,
                                   DS_3GPP_RMSM_FLOW_MOD_EV_COUNT,
                                   ds_3gpp_rmsm_flow_event_handler_cb,
                                   NULL);
    
        PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
                         
      }
      else
      {
        /* The flow must be modified by the network */
        /* send Packet event reporting */
        pkt_evt_info.change_reason = DS_3GPP_RMSM_ATI_PKT_EVT_BOTH_TFT_QOS_CHANGE;
        dsat_rmsm_pkt_event_rpt_handler(DSAT_NW_MODIFY,&pkt_evt_info);
      }
      break;
    default:
      break;
  }

}/* ds_3gpp_rmsm_at_flow_evt_hdlr */


/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_UM_IFACE_UP_HANDLER

  DESCRIPTION
    This function process the IFACE_UP event posted to DS_3GPP_RMSM AT 
    interface

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
LOCAL void ds_3gpp_rmsm_ati_um_iface_up_handler
(
  ds_3gpp_rmsm_at_instance_type  instance,
  ps_iface_event_info_u_type     event_info,
  ps_iface_type                 *iface_ptr
)
{
  ds_3gpp_ati_pdp_state_type *info_ptr= NULL;
  dsat_rmsm_info_s_type dsat_rmsm_info;
  dsat_cgerep_evt_info_s_type  pkt_evt_info = {0};
  ps_phys_link_type*   v4_phys_link_ptr = NULL;
  int ps_iface_result = 0;
  boolean change_state = TRUE;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if((instance < 0 )|| (instance >= DS_3GPP_RMSM_AT_UM_MAX_INSTANCES))
  {
    return;
  }
  if ( iface_ptr == NULL)
  {
    return;
  }

  info_ptr=ds_3gpp_rmsm_get_info_ptr_from_instance(instance);
  
  if(NULL == info_ptr)
  {
    return;
  }

    if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(info_ptr))
  {
         return;
  } 
  
  /* check if iface pointer is same as v4 or v6 */
  if( !( ( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr) ||
         ( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr)))
  {
    return;
  }

  memset(&dsat_rmsm_info, 0 , sizeof(dsat_rmsm_info_s_type));

  /* wait for both ifaces UP or DOWN event before sending
     response */
  if ( ( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr) &&
       ( NULL != info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr ) &&
       (IFACE_UP != ds_3gpp_rmsm_ati_get_v6_um_iface_state (instance ) ) )
  {
    change_state = FALSE;
  }
  else if ( ( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr) &&
            ( NULL != info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr ) &&
            (IFACE_UP != ds_3gpp_rmsm_ati_get_v4_um_iface_state (instance ) ) )
  {
    change_state = FALSE;
  }
  /*---------------------------------------------------------------------
    Um iface is up. 
  ---------------------------------------------------------------------*/
  if( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state == DS_3GPP_ATIF_PDP_ACTIVATING )
  {
    if (change_state == TRUE )
    {
       /* Send success of activation  if any of the iface is 
          in UP state */
      info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->iface_state = DS_3GPP_RMSM_ATIF_UM_IFACE_UP_ONLY_STATE;
      info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state = DS_3GPP_ATIF_PDP_ACTIVATED;
      /* Call ATCoP complete handler ,with OK */
      dsat_rmsm_info.result = DSAT_OK;
      dsat_rmsm_info.user_info_ptr = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr;
      dsat_rmsm_done_handler(DSAT_RMSM_CGACT_CMD,dsat_rmsm_info);
      /* Clear the user infor pointer for activation */
      info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr = NULL;

      /* send Packet event reporting */
      pkt_evt_info.cid = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id;
      pkt_evt_info.cm_subs_id = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->subs_id;
      dsat_rmsm_pkt_event_rpt_handler(DSAT_PDN_ACTIVATE,&pkt_evt_info);
    }

    /* register for Physlink events for v4 iface */
    if( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr)
    {
      v4_phys_link_ptr = PS_IFACE_GET_PHYS_LINK(iface_ptr);
      if(v4_phys_link_ptr == NULL)
      {
        return;
      }
      
      ps_iface_result = ds_3gpp_phys_link_reg_ev( phys_link_ev_p,
                                 DS_3GPP_RMSM_PHYS_LINK_EV_COUNT,
                                 ds_3gpp_rmsm_phys_link_um_event_handler_cb,
                                 v4_phys_link_ptr,
                                 info_ptr);

      if( ps_iface_result < 0)
      {
        return;
      }
      
      info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_phys_link_ptr = v4_phys_link_ptr;
      DS_3GPP_MSG1_LOW ("V4 Physlink pointer = 0x%1x",v4_phys_link_ptr);
    }
  }
  /* We are in de-activating phase */
  else if ( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state == DS_3GPP_ATIF_PDP_DEACTIVATING )
  {
    /* failure of +CGACT de-activating command , 
       return ERROR and de-allocate the instance */
    DS_3GPP_MSG0_LOW ("+CGACT de-activation is failure ");
    dsat_rmsm_info.result = DSAT_ERROR;
    dsat_rmsm_info.reason_type = DSAT_RMSM_IFACE_NET_DOWN_REASON;
    dsat_rmsm_info.reason.iface_net_down = PS_NET_DOWN_REASON_VALIDATION_FAILURE;
    dsat_rmsm_info.user_info_ptr = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr;
    dsat_rmsm_done_handler(DSAT_RMSM_CGACT_CMD,dsat_rmsm_info);
    /* To Do de-reg of all events */
    /* de allocate the instance already brought up */
    ds_3gpp_rmsm_ati_deallocate_instance (instance);
  }
} /* ds_3gpp_rmsm_ati_um_iface_up_handler */


/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_UM_IFACE_DOWN_HANDLER

  DESCRIPTION
    This function process the IFACE_DOWN event posted to DS_3GPP_RMSM AT 
    interface

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
LOCAL void ds_3gpp_rmsm_ati_um_iface_down_handler
(
  ds_3gpp_rmsm_at_instance_type  instance,
  ps_iface_event_info_u_type     event_info,
  ps_iface_type                 *iface_ptr
)
{
  ds_3gpp_ati_pdp_state_type *info_ptr= NULL;
  dsat_result_enum_type result = DSAT_ASYNC_CMD;
  dsat_rmsm_info_s_type dsat_rmsm_info;
  dsat_cgerep_evt_info_s_type  pkt_evt_info = {0};
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if((instance < 0 )|| (instance >= DS_3GPP_RMSM_AT_UM_MAX_INSTANCES))
  {
    return;
  }

  info_ptr=ds_3gpp_rmsm_get_info_ptr_from_instance(instance);

  if (NULL == info_ptr)
  {
    return;
  }

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(info_ptr))
  {
         return;
  } 
 
    /* check if iface pointer is same as v4 or v6 */
  if( !( ( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr) ||
         ( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr)))
  {
    return;
  }
  /*---------------------------------------------------------------------
     Um iface is DOWN. 
  ---------------------------------------------------------------------*/
  if( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state == DS_3GPP_ATIF_PDP_DEACTIVATING )
  {
    /* success of de-activation until both ipv4 and ipv6 ifaces down*/
    if(((iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr)&&
        (NULL == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr)) ||
       ((iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr)&&
        (NULL == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr)))
    {
      result = DSAT_OK;
    }
    else
    {
      result = DSAT_ASYNC_CMD;
    }
  }
  else if ( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state == DS_3GPP_ATIF_PDP_ACTIVATING )
  {
    if ( ( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr) &&
          (NULL != info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr) )
    {
      if (IFACE_UP == ds_3gpp_rmsm_ati_get_v4_um_iface_state(instance))
      {
        result = DSAT_OK;
      }
      else
      {
        result = DSAT_ASYNC_CMD;
      }
    }
    else if ( ( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr) &&
          (NULL != info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr) )
    {
      if (IFACE_UP == ds_3gpp_rmsm_ati_get_v6_um_iface_state(instance))
      {
        result = DSAT_OK;
      }
      else
      {
        result = DSAT_ASYNC_CMD;
      }
    }
    else
    {
      /* failure of +CGACT command , return ERROR and 
         de-allocate the instance */
      result = DSAT_ERROR;
    }
  }

  DS_3GPP_MSG2_HIGH("+CGACT de-activation  for RMSM inst %d, result %d ",
                    instance , result);
  if( DSAT_ASYNC_CMD != result )
  {
    dsat_rmsm_info.result = result;
    dsat_rmsm_info.reason_type = DSAT_RMSM_IFACE_NET_DOWN_REASON;
    dsat_rmsm_info.reason.iface_net_down= 
                 event_info.iface_down_info.netdown_reason;
    dsat_rmsm_info.user_info_ptr = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr;
    pkt_evt_info.cid = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id;
    pkt_evt_info.cm_subs_id = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->subs_id;

    /* Send final response to ATCoP */
    dsat_rmsm_done_handler(DSAT_RMSM_CGACT_CMD, dsat_rmsm_info);

    /* send Packet event reporting */
    dsat_rmsm_pkt_event_rpt_handler(DSAT_PDN_DEACTIVATE,&pkt_evt_info);
  }

  /* Check if rmsm instance is deallocated as part of rmsm_done_handler */
  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(info_ptr))
  {
    return;
  }

  /* if pending data is present then QCDGEN command 
     response is pending , send result code 
  */
  if( (info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->pending_data !=0) && ( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr))
  {
    dsat_rmsm_info.result = DSAT_ERROR;
    dsat_rmsm_info.user_info_ptr = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr;
    dsat_rmsm_info.reason_type = DSAT_RMSM_INVALID_REASON;  
    dsat_rmsm_done_handler(DSAT_RMSM_QCDGEN_CMD,dsat_rmsm_info);
  }
  /*---------------------------------------------------------------------
    Deregister all the events with UM packet interface
  ---------------------------------------------------------------------*/
  if( ( ( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr) &&
        (NULL == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr) ) ||
      ( ( iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr) &&
        (NULL == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr) ) )
  {
	  (void)ds_3gpp_iface_dereg_ev(single_iface_ev_p,
                                 DS_3GPP_RMSM_SINGLE_IFACE_EV_COUNT,
                                 ds_3gpp_rmsm_ati_um_event_handler_cb,
                                 NULL);

  }
  /* Set iface pointer as NULL, physlink de-registration of 
      the v4 iface will happen in PHYS_LINK_GONE_EV ,
      whichever comes first.
  */
  if(iface_ptr == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr)
  {

    (void)ds_3gpp_iface_dereg_ev(iface_ev_p,
                                 DS_3GPP_RMSM_IFACE_EV_COUNT,
                                 ds_3gpp_rmsm_ati_um_event_handler_cb,
                                 iface_ptr);

    info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr = NULL;
    if(info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_phys_link_ptr != NULL)
    {
      /* Deregister the physlink events */
     (void)ds_3gpp_phys_link_dereg_ev(phys_link_ev_p,
                                      DS_3GPP_RMSM_PHYS_LINK_EV_COUNT,
                                      ds_3gpp_rmsm_phys_link_um_event_handler_cb,
                                      info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p
                                      ->v4_phys_link_ptr);

      info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_phys_link_ptr = NULL;
    }
  }
  else
  {
    info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr = NULL;
  }

  /* de allocate the instance already brought up 
     if both iface pointers are NULL */
  if( ( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr == NULL ) &&
      ( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr == NULL ) )
  {
    ds_3gpp_rmsm_ati_deallocate_instance (instance);
  }
}/* ds_3gpp_rmsm_ati_um_iface_down_handler*/

/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_CGACT_QUERY

  DESCRIPTION
    This function returns teh profile status of all PDP contexts. It 
    retrieves data from DS_3GPP_RMSM_ST interface and Mode handler for
    valid PDP profiles.

  RETURN VALUE
    DSAT_OK -- if SUCCESS
    DSAT_ERROR -- if FAILURE

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsat_result_enum_type ds_3gpp_rmsm_ati_cgact_query
(
  ds_3gpp_atif_profile_status_info_type * profile_status_info_ptr,
  sys_modem_as_id_e_type                  subs_id 
)
{
  ds_3gpp_rmsm_at_instance_type instance = 0;
  int i=0;
  ds_pdn_context_state_e    state = DS_PDN_CONTEXT_STATE_DOWN;
  uint8                                         profile_nums_arr[DS_UMTS_MAX_PDP_PROFILE_NUM];
  uint8                                         profile_count = 0;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (NULL == profile_status_info_ptr || (!ds3gsubsmgr_is_subs_id_valid(subs_id)))
  {
    return DSAT_ERROR;
  }

  DS_3GPP_MSG1_HIGH ("ds_3gpp_rmsm_ati_cgact_query for subs id %d",subs_id);
  
  memset((void*)profile_status_info_ptr,0x0,
    sizeof(ds_3gpp_atif_profile_status_info_type));

  profile_count = ds_3gpp_profile_cache_get_valid_profile_numbers(
                                                 dsumts_subs_mgr_get_subs_id(subs_id),
                 profile_nums_arr, 
                 DS_UMTS_MAX_PDP_PROFILE_NUM);

  profile_status_info_ptr->profile_count = profile_count;

  for (i=0;i<profile_count;i++)
  {
    if(ds_3gpp_profile_cache_get_cache_index(profile_nums_arr[i],
                                              dsumts_subs_mgr_get_subs_id(subs_id)) == -1)
    {
      continue;
    }

    instance = ds_3gpp_rmsm_ati_get_inst_num_from_profile(profile_nums_arr[i], subs_id);
    if(instance != DS_3GPP_RMSM_AT_UM_MAX_INSTANCES)
    {
      DS_3GPP_MSG1_LOW ("profile %d activated by +CGACT",profile_nums_arr[i]);
      profile_status_info_ptr->list[i].profile_num = profile_nums_arr[i];
      profile_status_info_ptr->list[i].act_status = DS_3GPP_ATIF_PDP_ACTIVATED;
    }
    else
    {
        //Check with amit if this would be inside the if below or not
        profile_status_info_ptr->list[i].profile_num = profile_nums_arr[i];
        /* Query LTE MH to check whether the profile is active */
        state = ds_pdn_cntxt_get_pdn_context_state(profile_nums_arr[i], subs_id);
        if( DS_PDN_CONTEXT_STATE_DOWN != state)
        {
          /* Update the state as active if state is not in DOWN state */
          profile_status_info_ptr->list[i].act_status = DS_3GPP_ATIF_PDP_ACTIVATED;
      }
    }/* else */
  }/* for */
  return DSAT_OK;
}/* ds_3gpp_rmsm_ati_cgact_query */

/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_PROCESS_QCDGEN

  DESCRIPTION
    This function process data to a context activated
    using CGACT PDP IP profile.

  RETURN VALUE
    DSAT_ERROR -- In case of any FAILURE
    DSAT_ASYNC_CMD -- in case of SUCCESS

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsat_result_enum_type ds_3gpp_rmsm_ati_process_qcdgen
(
  uint32                                  profile_number,
  sys_modem_as_id_e_type                  subs_id,
  uint32                                  total_data_len,
  void *                                  user_info_ptr
)
{

  ds_3gpp_rmsm_at_instance_type   instance = DS_3GPP_RMSM_AT_UM_MAX_INSTANCES;
  ds_umts_pdp_context_type        pdp_context;
  ds_3gpp_ati_pdp_state_type     *info_ptr=NULL;
  ps_iface_type                  *ps_iface_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return DSAT_ERROR;
  }

  /* Check whether the profile is already activated by +CGACT */
  instance = ds_3gpp_rmsm_ati_get_inst_num_from_profile(profile_number, subs_id);
  if ( DS_3GPP_RMSM_AT_UM_MAX_INSTANCES == instance)
  {
    return DSAT_ERROR;
  }
  info_ptr = ds_3gpp_rmsm_get_info_ptr_from_instance(instance);

  if (info_ptr == NULL)
  {
    return DSAT_ERROR;
  }

    if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(info_ptr))
  {
    DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 


  pdp_context = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info;

  DS_3GPP_MSG3_HIGH("QCDGEN V4 Iface pointer %d PDP type is %d, profile no",
                     info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr,
                     pdp_context.pdp_type,
                     profile_number);

  /* 
    QCDGEN command supports only for IPV4, Ipv4v6 context where v4 iface is valid.
  */
  if( (pdp_context.pdp_type == DS_UMTS_PDP_PPP) ||
      (NULL == info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr ) )
  {

    return DSAT_ERROR;
  }
  /* Check if the profile is primary */
  if(pdp_context.secondary_flag == TRUE)
  {
    return DSAT_ERROR;
  }

 /* check whether DATA len is > DS_3GPP_RMSM_ATI_MIN_DATA_LEN*/
  if (DS_3GPP_RMSM_ATI_MIN_DATA_LEN > total_data_len)
  {
    return DSAT_ERROR;
  }

  /* Generate data */
  ps_iface_ptr = info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr;
  if (NULL == ps_iface_ptr)
  {
    return DSAT_ERROR;
  }

  /* Check Iface state, is flow enabled, if not update
    only the pending data transfer. When flow is enabled
    data transfer will happen. Send DSAT_ASYNC_CMD 
  */
  info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->pending_data = total_data_len;
  /* Update user info pointer */
  info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr = user_info_ptr;
  info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->um_flow_enabled = 
        PS_IFACE_FLOW_ENABLED( info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr);

  /*-------------------------------------------------------------------------  
    Set the handler for the PS_3GPP_RMSM_AT_UM_RX_Q_SIGNAL
    signal for data transfer.
    PS handles are assigned when the intance is allocated, and invalidated
    when the instance is deallocated. The original handle is passed to the 
    callback to make sure that only instances with valid handles are 
    processed.
  -------------------------------------------------------------------------*/
  (void)ps_set_sig_handler(
                    PS_3GPP_RMSM_AT_UM_RX_Q_SIGNAL,
                    ds_3gpp_rmsm_ip_tx_um_data_cb, 
                 (void *)info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->ps_handle);
  
  /*-------------------------------------------------------------------------  
    Enable it.
  -------------------------------------------------------------------------*/
  ps_enable_sig(PS_3GPP_RMSM_AT_UM_RX_Q_SIGNAL);

  /*---------------------------------------------------------------------
   Send signal to PS to send data to UM IFACE to start flow
  ---------------------------------------------------------------------*/
  PS_SET_SIGNAL( PS_3GPP_RMSM_AT_UM_RX_Q_SIGNAL );

  return DSAT_ASYNC_CMD;
} /* ds_3gpp_rmsm_ati_process_qcdgen */

/*===========================================================================
FUNCTION DS_3GPP_RMSM_IP_TX_UM_DATA_CB

DESCRIPTION
  This function forwards pkts received for Um interface 
  from ATCoP . 

PARAMETERS   
    sig    - Signal raised
    instance   - instance of teh IFACE for which data transfer 
                 is pending
DEPENDENCIES 
  None.
  
RETURN VALUE 
  FALSE -- failure
  TRUE  -- Success

SIDE EFFECTS 
  None.
  
===========================================================================*/
LOCAL boolean ds_3gpp_rmsm_ip_tx_um_data_cb
(
  ps_sig_enum_type sig,
  void             *user_data_p
)
{
  int32                           ps_handle = DS_3GPP_RMSM_AT_INVALID_HANDLE;
  ds_3gpp_rmsm_at_instance_type   instance = DS_3GPP_RMSM_AT_UM_MAX_INSTANCES;
  ds_3gpp_ati_pdp_state_type     *info_ptr = NULL;
  ds_cmd_type                    *cmd_ptr = NULL;
  ds_3gpp_rmsm_at_type           *data_ptr = NULL;
  boolean                         result = TRUE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the instance from the ps handle */
  ps_handle = (int32)user_data_p;
  instance = ps_handle_mgr_get_index(PS_HANDLE_MGR_CLIENT_3GPP_RMSM,
                                     ps_handle);

  DS_3GPP_ENTER_CRIT_SECTION(&ds_3gpp_rmsm_crit_sect);
  info_ptr = ds_3gpp_rmsm_get_info_ptr_from_instance(instance);

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(info_ptr))
  {
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_rmsm_crit_sect);
    return FALSE;
  } 

  if(info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->ps_handle != ps_handle)
  {
    DS_3GPP_MSG2_LOW ("rmsm instance ps handle is not valid: "
                      "original handle %d, current handle %d. "
                      "No attempt made to dequeue Um data to PS",
                      ps_handle, 
                      info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->ps_handle);
    return result;
  }

  if (info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->pending_data != 0 )
  {
    result = ds_3gpp_rmsm_ati_send_data(info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr,
                                      &info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->pending_data);
  }
  else
  {
    /* This should not have happened */
    DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_rmsm_crit_sect);
    return result;
  }
  if(result == FALSE)
  {
    info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->pending_data=0;
    DS_3GPP_MSG0_LOW("Failed to send data to UM iface");
  }
  
   DS_3GPP_LEAVE_CRIT_SECTION(&ds_3gpp_rmsm_crit_sect);
  /* post a command to set signal again */
  cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_rmsm_at_type));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    return FALSE;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_RMSM_AT_SEND_DATA;
  data_ptr = (ds_3gpp_rmsm_at_type*)cmd_ptr->cmd_payload_ptr;

  data_ptr->ds_3gpp_rmsm_at_instance = instance;
  data_ptr->result = result;
  ds_put_cmd( cmd_ptr);
  /* always return true */
  return TRUE;
}/* ds_3gpp_rmsm_ip_tx_um_data_cb */

/*===========================================================================

FUNCTION DS_3GPP_RMSM_ATI_GENERATE_IP_FRAME

DESCRIPTION
  This function allocates a dsm_item_type and uses dsm_pushdown_tail() to
  fill with canned IP frame of total length specified and using values for
  fields:

DEPENDENCIES
  None

RETURN VALUE
  returns a boolean: TRUE on success, FALSE on failure.

SIDE EFFECTS
  On success, sets ip_frame_buf to point to allocated and IP frame filled 
  dsm_item_type chain.

===========================================================================*/
LOCAL boolean ds_3gpp_rmsm_ati_generate_ip_frame
(
  dsm_item_type **ip_frame,
  uint16 total_data_len
)
{
  /* Default values for fields of IP header */
#define VERSION       4
#define IHL           5           /* Size in 32b words */
#define TOS           0
#define DEF_PKT_LEN   1500      
#define IDENT         0
#define TTL           32
#define PROTO         17          /* UDP */
#define SRC_ADDR      0xC0A80001  /* 192.168.0.1 */
#define DEST_ADDR     0xC0A80002  /* 192.168.0.2 */
/* Other constants */
#define MIN_PKT_SIZE  ((IHL << 2) + 1) /* IP header + 1 byte data */
#define CALC_CKSUM    0

  boolean result = TRUE;
  static uint16 identifier = 0;
  uint16 pkt_data_len;
  struct ip ip_hdr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( NULL == ip_frame )
  {
    return FALSE;
  }
  
  if ((total_data_len <= MIN_PKT_SIZE) || 
         (total_data_len > DEF_PKT_LEN))
  {
    return FALSE;
  }

  if ( NULL == (*ip_frame = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL)) )
  {
    result = FALSE;
  }
  else
  {
    memset((void *)&ip_hdr, 0, sizeof(struct ip));

    ip_hdr.version       = VERSION;
    ip_hdr.tos           = TOS;
    ip_hdr.length        = total_data_len;
    ip_hdr.id            = ++identifier;
    ip_hdr.ttl           = TTL;
    ip_hdr.protocol      = PROTO;
    ip_hdr.ihl           = IHL;
    ip_hdr.source.ps_s_addr = ps_htonl(SRC_ADDR);              
    ip_hdr.dest.ps_s_addr   = ps_htonl(DEST_ADDR);              
    pkt_data_len = total_data_len - (IHL << 2);

    /* If pushing IP packet header or packet data onto tail of dsm item chain
       should fail, exit with result set FALSE */
    if ( pkt_data_len != dsm_pushdown_tail( ip_frame, 
                                            (void*)NULL, 
                                            pkt_data_len,
                                                     DSM_ITEM_POOL(*ip_frame) ) ||
         !ps_ip4_hdr_create( ip_frame,
                             &ip_hdr,
                             IP_CS_NEW ) )
    {
      result = FALSE;
    }
  }
  if ( FALSE == result && 
       *ip_frame != NULL )
  {
    dsm_free_packet(ip_frame);
  }
  return result;
} /* ds_3gpp_rmsm_ati_generate_ip_frame */

/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_SEND_DATA

  DESCRIPTION
    This function sends data to a PS Iface pointer 
    and updates the data len with remaining data_bytes.

  RETURN VALUE
    TRUE -- Success of data transfer
    FALSE -- failure of data transfer

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
LOCAL boolean ds_3gpp_rmsm_ati_send_data
(
  ps_iface_type                    *ps_iface_ptr,
  uint32                           *data_len_ptr
)
{
  uint32 data_len_to_send =0;
  dsm_item_type            *ip_frame = NULL; 
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((NULL == ps_iface_ptr) || (NULL == data_len_ptr))
  {
    return FALSE;
  }
  /* Choose for data_len a value less than or equal to maximimum frame
     size that won't leave a quantity of data less than minimum data 
     length left to send
  */
  if ( *data_len_ptr > DS_3GPP_RMSM_ATI_MAX_FRAME_SIZE )
  {
    data_len_to_send = 
      ( (*data_len_ptr - DS_3GPP_RMSM_ATI_MAX_FRAME_SIZE) >= 
        DS_3GPP_RMSM_ATI_MIN_DATA_LEN ) ?
      DS_3GPP_RMSM_ATI_MAX_FRAME_SIZE : 
      (uint32)(*data_len_ptr - DS_3GPP_RMSM_ATI_MIN_DATA_LEN);
  }
  else
  {
    data_len_to_send = *data_len_ptr;
  }
  /* Genarate IP frame */
  if( FALSE == ds_3gpp_rmsm_ati_generate_ip_frame(&ip_frame,
                                                  (uint16)data_len_to_send))
  {
    DS_3GPP_MSG0_LOW ("IP packet generation failed");
    return FALSE;
  }
  /* Transmit the generated packet */
  if( -1 == ps_iface_tx_cmd(ps_iface_ptr,&ip_frame,NULL))
  {
    dsm_free_packet(&ip_frame);
    return FALSE;
  }
  *data_len_ptr = *data_len_ptr -data_len_to_send;
  DS_3GPP_MSG3_HIGH ("Sent data on ps_iface_ptr %x, qty %d, rem bytes %d",
             ps_iface_ptr, data_len_to_send,*data_len_ptr);
  return TRUE;
}/* ds_3gpp_rmsm_ati_send_data */


/*===========================================================================
  FUNCTION DS_3GPP_RMSM_ATI_GET_PDP_ADDR

  DESCRIPTION
  
  This function returns the PDP addr of the PDN connection for which 
  the Profile number is passed.

  RETURN VALUE
    DSAT_OK -- if SUCCESS
    DSAT_ERROR -- if FAILURE

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsat_result_enum_type ds_3gpp_rmsm_ati_get_pdp_addr
(
  uint32                    profile_num,
  sys_modem_as_id_e_type    subs_id,
  ds_umts_pdp_addr_type    *pdp_addr_ptr
)
{
  if (NULL == pdp_addr_ptr)
  {
    return DSAT_ERROR;
  }
  if(!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return DSAT_ERROR;
  }

  if(FALSE == ds_pdn_cntxt_get_pdn_context_pdp_addr(profile_num,
                                                    subs_id,
                                pdp_addr_ptr))
  { 
    return DSAT_ERROR;
  }
  return DSAT_OK;
}/* ds_3gpp_rmsm_ati_get_pdp_addr */

/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_CGCMOD_HANDLER
  
  DESCRIPTION
    This Function is called by RMSM IF for modification of profiles
    requested by +CGCMOD Command. This function returns DSAT_ERROR if the 
    profile is not activated by +CGACT. Only the profile.

  DEPENDENCIES
    None.
  
  RETURN VALUE
    DSAT_ERROR: in case of Failure
    DSAT_ASYNC_CMD: Modification is in process.
  
  SIDE EFFECTS
    None
===========================================================================*/
dsat_result_enum_type ds_3gpp_rmsm_ati_cgcmod_handler
(
  uint32                                profile_number,
  sys_modem_as_id_e_type                subs_id,
  void *                                user_info_ptr
)
{
  ds_3gpp_rmsm_at_instance_type  instance =0;
  ds_3gpp_ati_pdp_state_type *info_ptr = NULL;
  dsat_result_enum_type       result = DSAT_ASYNC_CMD;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return DSAT_ERROR;
  }
  /* Check if the context is actiavted by +CGACT command */
  instance = ds_3gpp_rmsm_ati_get_inst_num_from_profile(profile_number, subs_id);

  if ( DS_3GPP_RMSM_AT_UM_MAX_INSTANCES == instance)
  {
    return DSAT_ERROR;
  }

  /* check if the requested profile is primary or secondary */
  info_ptr=ds_3gpp_rmsm_get_info_ptr_from_instance(instance);
  if (NULL == info_ptr)
  {
    return DSAT_ERROR;
  }

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(info_ptr))
  {
    DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 
  

#ifdef FEATURE_SECONDARY_PDP
  if (info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.secondary_flag == TRUE)
  {
    /* Modify the secondary PDP context */
    result = ds_3gpp_rmsm_ati_modify_sec_pdp_ctx(info_ptr);
  }
  else
#endif /* FEATURE_SECONDARY_PDP */
  {
    /* Fail Primary PDP context modification */
    result =  DSAT_ERROR;
  }

  if(DSAT_ASYNC_CMD == result)
  {
    /* Update user info pointer */
    info_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->user_info_ptr = user_info_ptr;
  }
   DS_3GPP_MSG2_HIGH("UE initiated primary PDP Modify for profile %d, result %d",
                      profile_number, result);
  return result;
}/* ds_3gpp_rmsm_ati_cgcmod_handler */

/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_VERIFY_QOS_PARAMS
  
  DESCRIPTION
    This Function verifies QoS params and converts to app format. 

  DEPENDENCIES
    None.
  
  RETURN VALUE
    DSAT_ERROR: in case of Failure
    DSAT_ASYNC_CMD : in case of Success
  
  SIDE EFFECTS
    None
===========================================================================*/
static dsat_result_enum_type ds_3gpp_rmsm_ati_verify_qos_params
(
  ds_umts_pdp_profile_type *pdp_profile_ptr,
  qos_spec_type            *app_qos
)
{
  dsat_result_enum_type  ret_val = DSAT_ASYNC_CMD;

  if ( (NULL == pdp_profile_ptr) || (NULL == app_qos) )
  {
    return DSAT_ERROR;
  }

  if (pdp_profile_ptr->qos_request_lte.valid_flg  == TRUE )
  {
    if(FALSE == ds_3gpp_rmsm_ati_conv_flow_to_lte_app_qos_from_id(
                                           pdp_profile_ptr,
                                           app_qos))
    {
      DS_3GPP_MSG0_LOW ("Convertion to LTE APP QOS failed");
      ret_val = DSAT_ERROR;
    }
  }
#ifdef FEATURE_DATA_WCDMA_PS
  else if ((pdp_profile_ptr->qos_request_umts.valid_flg == TRUE ) ||
           (pdp_profile_ptr->qos_minimum_umts.valid_flg == TRUE ))
  {
    if ( FALSE == ds_3gpp_rmsm_ati_conv_flow_to_umts_app_qos_from_id(
                                           pdp_profile_ptr,
                                           app_qos) )
    {
      DS_3GPP_MSG0_LOW ("Convertion to UMTS APP QOS failed");
      ret_val = DSAT_ERROR;
    }
  }
#endif /* FEATURE_DATA_WCDMA_PS */
#ifdef FEATURE_GSM_GPRS
  else if ((pdp_profile_ptr->qos_request_gprs.valid_flg == TRUE ) ||
           (pdp_profile_ptr->qos_minimum_gprs.valid_flg == TRUE )) 
  {
    if ( FALSE == ds_3gpp_rmsm_ati_conv_flow_to_gprs_app_qos_from_id(
                                           pdp_profile_ptr,
                                           app_qos))
    {
      DS_3GPP_MSG0_LOW ("Convertion to GPRS APP QOS failed");
      ret_val = DSAT_ERROR;
    }
  }
#endif /* FEATURE_GSM_GPRS */
  else
  {
    DS_3GPP_MSG0_LOW ("No QOS specified in profile, return Failure");
    ret_val = DSAT_ERROR;
  }

  return ret_val;
}

/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_ACTIVATE_SEC_PDP_CTX
  
  DESCRIPTION
    This Function is called  for activation of secondary 
    PDP profiles by +CGACT command. 

  DEPENDENCIES
    None.
  
  RETURN VALUE
    DSAT_ERROR: in case of Failure
    DSAT_OK : if context is already activated
    DSAT_ASYNC_CMD: ACTIVATION is in process.
  
  SIDE EFFECTS
    None
===========================================================================*/
LOCAL dsat_result_enum_type ds_3gpp_rmsm_ati_activate_sec_pdp_ctx
(
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
)
{
  sint15                   ps_errno;
  int                      ret_val;
  ds_umts_pdp_profile_type  *pdp_profile_ptr = NULL;
  dsat_result_enum_type       result = DSAT_ASYNC_CMD;
  ps_iface_ioctl_qos_request_type  qos_request;
  qos_spec_type                    app_qos;  
  ds_umts_pdp_profile_status_etype status = DS_UMTS_PDP_FAIL;
  byte   primary_id =0;
  ds_3gpp_rmsm_at_instance_type  primary_inst ;
  ps_iface_type              *ps_iface_ptr;
  ds_3gpp_ati_pdn_dynamic_info_type     pdn_dynamic_parm = {0};
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (pdp_ptr == NULL)
  {
    return DSAT_ERROR;
  }

   if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(pdp_ptr))
  {
    DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 


  primary_id = pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_context_info.primary_id;
  if(ds_3gpp_profile_cache_get_cache_index(primary_id, 
             dsumts_subs_mgr_get_subs_id(pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->subs_id)) == -1)
  {
    DS_3GPP_MSG1_LOW ("Invalid primary ID %d",primary_id);
    return DSAT_ERROR;
  }

  /* Check if the primary profile is already activated by +CGACT */
  primary_inst = ds_3gpp_rmsm_ati_get_inst_num_from_profile(primary_id,
                                pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->subs_id);

  if(primary_inst == DS_3GPP_RMSM_AT_UM_MAX_INSTANCES)
  {
    DS_3GPP_MSG1_HIGH("Assoc primary id %d is not activated by +CGACT ",primary_id);
    if(FALSE == ds_pdn_cntxt_get_ati_dynamic_info(primary_id,
                 pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->subs_id,
                                                  &pdn_dynamic_parm))
    {
      DS_3GPP_MSG0_LOW ("ds_pdn_cntxt_get_ati_dynamic_info returned false");
      return DSAT_ERROR;
    }
    pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr = pdn_dynamic_parm.v4_iface_ptr;
    pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr = pdn_dynamic_parm.v6_iface_ptr;
  }
  else
  {
    /* assign the primary's iface pointer to this secondary context */
    pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr = ds_3gpp_rmsm_get_v4_iface_ptr(primary_inst);
    pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr = ds_3gpp_rmsm_get_v6_iface_ptr(primary_inst);
  }

  pdp_profile_ptr = (ds_umts_pdp_profile_type *)modem_mem_alloc(
        sizeof(ds_umts_pdp_profile_type),
        MODEM_MEM_CLIENT_DATA );

  if (pdp_profile_ptr == NULL) 
  {
    DS_3GPP_MSG0_LOW ("FAIL modem_mem_alloc for ds_umts_pdp_profile_type");
    return DSAT_ERROR;
  }
  
  status = ds_umts_get_pdp_profile_all_data_per_subs( 
                                             (uint16)pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id,
                                             dsumts_subs_mgr_get_subs_id(pdp_ptr->
                                                                ds_3gpp_ati_pdp_state_type_dyn_p->subs_id),
                                             pdp_profile_ptr 
                                           );
  if(DS_UMTS_PDP_SUCCESS == status)
  {
    memset(&qos_request, 0, sizeof(ps_iface_ioctl_qos_request_type));
    memset(&app_qos, 0, sizeof(qos_spec_type));

    app_qos.rx.fltr_template.list_ptr = 
        (ip_filter_type *)modem_mem_calloc(MAX_PACKET_FILTERS,
                                           sizeof(ip_filter_type),
                                           MODEM_MEM_CLIENT_DATA);
    if(app_qos.rx.fltr_template.list_ptr == NULL)
    {
      DS_3GPP_MSG0_LOW ("Heap Memory Allocation failed");
      modem_mem_free((void *)pdp_profile_ptr, 
                     MODEM_MEM_CLIENT_DATA );
      return DSAT_ERROR;
    }

    result = ds_3gpp_rmsm_ati_verify_qos_params(pdp_profile_ptr, &app_qos);

    if( ( result != DSAT_ERROR)&&
        (FALSE == ds_3gpp_rmsm_ati_conv_tft_to_app_tft_from_id(
                                           DS_3GPP_RMSM_TFT_OPCODE_CREATE,
                                           pdp_profile_ptr->tft,
                                           0,
                                           DS_UMTS_MAX_TFT_PARAM_SETS,
                                           &app_qos)) )
    {
      DS_3GPP_MSG0_LOW ("Convertion TFT to APP TFT failed");
      result = DSAT_ERROR;
    }
  }
  else
  {
    DS_3GPP_MSG0_LOW ("Unable to retrieve profile data");
    modem_mem_free((void *)pdp_profile_ptr, 
                     MODEM_MEM_CLIENT_DATA );
    return DSAT_ERROR;
  }
  /* Need to fill the qos_ptr in request info */
  qos_request.qos_ptr = &app_qos;

  if( DSAT_ERROR != result)
  {
    /* iface assignment based on primary context */
    ps_iface_ptr = pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v4_um_iface_ptr;
    if( ps_iface_ptr == NULL ) 
    {
      ps_iface_ptr = pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->v6_um_iface_ptr;
    }
    if(ps_iface_ptr == NULL)
    {
      DS_3GPP_MSG0_LOW ("No valid Iface pointer, return ERROR");
      result = DSAT_ERROR;
    }
    if(result != DSAT_ERROR)
    {
      /* Register for flow NULL and FLOW activated events */
      if(ds_3gpp_flow_reg_ev(flow_ev_p,
                             DS_3GPP_RMSM_FLOW_EV_COUNT,
                             ds_3gpp_rmsm_flow_event_handler_cb,
                             NULL,
                             pdp_ptr) != TRUE)
      {
        DS_3GPP_MSG0_LOW ("Couldn't register for Flow event!");
        result = DSAT_ERROR;
      }

      if(result != DSAT_ERROR)
      {
        /* call PS iface API for flow request */
        DS_3GPP_MSG0_LOW ("Calling ps_flow_ioctl for QOS request");

        ret_val = ps_iface_ioctl(ps_iface_ptr,
                                 PS_IFACE_IOCTL_QOS_REQUEST,
                                 &qos_request,
                                 &ps_errno);
        if(ret_val != 0)
        {
          DS_3GPP_MSG0_LOW ("Failed to activate the Secondary");
          /* Deregister all QOS flow event's */

          (void)ds_3gpp_flow_dereg_ev(flow_ev_p,
                                      DS_3GPP_RMSM_FLOW_EV_COUNT,
                                      ds_3gpp_rmsm_flow_event_handler_cb,
                                      NULL);
          result =  DSAT_ERROR;
        }
        else
        {
          pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->flow_ptr = qos_request.flow_ptr;
          result = DSAT_ASYNC_CMD;
          pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state = DS_3GPP_ATIF_PDP_ACTIVATING;
        }
      }
    }
  }

  modem_mem_free((void *)pdp_profile_ptr, 
                   MODEM_MEM_CLIENT_DATA );
  modem_mem_free((void *)app_qos.rx.fltr_template.list_ptr, 
                   MODEM_MEM_CLIENT_DATA);
return result;

} /* ds_3gpp_rmsm_ati_activate_sec_pdp_ctx */

/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_DEACTIVATE_SEC_PDP_CTX
  
  DESCRIPTION
    This Function is called  for de-activation of secondary 
    PDP profiles by +CGACT command. 

  DEPENDENCIES
    None.
  
  RETURN VALUE
    DSAT_ERROR: in case of Failure
    DSAT_OK : if flow is already de-activated
    DSAT_ASYNC_CMD: Modification is in process.
  
  SIDE EFFECTS
    None
===========================================================================*/
LOCAL dsat_result_enum_type ds_3gpp_rmsm_ati_deactivate_sec_pdp_ctx
(
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
)
{
  sint15                   ps_errno;
  int                      ret_val;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_3GPP_MSG0_HIGH("Calling ps_flow_ioctl for QOS release");
  if (pdp_ptr == NULL)
  {
    return DSAT_ERROR;
  }

   if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(pdp_ptr))
  {
    DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 
 

  ret_val = ps_flow_ioctl(pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->flow_ptr,
                        PS_FLOW_IOCTL_QOS_RELEASE,
                        0,
                        &ps_errno);
  if(ret_val != 0)
  {
    DS_3GPP_MSG0_LOW ("Failed to release the Secondary");
    /* Deregister all QOS flow event's */
    (void)ds_3gpp_flow_dereg_ev(flow_ev_p,
                                DS_3GPP_RMSM_FLOW_EV_COUNT,
                                ds_3gpp_rmsm_flow_event_handler_cb,
                                NULL);

    return DSAT_ERROR;
  }
  pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->connect_state = DS_3GPP_ATIF_PDP_DEACTIVATING;

  return DSAT_ASYNC_CMD;
} /* ds_3gpp_rmsm_ati_deactivate_sec_pdp_ctx */


/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_MODIFY_SEC_PDP_CTX
  
  DESCRIPTION
    This Function is called  for modification of secondary 
    PDP profiles activated by +CGACT command. 

  DEPENDENCIES
    None.
  
  RETURN VALUE
    DSAT_ERROR: in case of Failure
    DSAT_ASYNC_CMD: Modification is in process.
  
  SIDE EFFECTS
    None
===========================================================================*/
LOCAL dsat_result_enum_type ds_3gpp_rmsm_ati_modify_sec_pdp_ctx
(
  ds_3gpp_ati_pdp_state_type     *pdp_ptr
)
{
  dsat_result_enum_type       result = DSAT_ASYNC_CMD;
  ps_flow_ioctl_qos_modify_type qos_modify;
  qos_spec_type                    app_qos;  
  sint15                   ps_errno;
  ds_umts_pdp_profile_type  *pdp_profile_ptr = NULL;
  ds_umts_pdp_profile_status_etype status = DS_UMTS_PDP_FAIL;
  int ret_val = 0;
  qos_spec_field_mask_type  qos_spec_field_mask;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  pdp_profile_ptr = (ds_umts_pdp_profile_type *)modem_mem_alloc(
      sizeof(ds_umts_pdp_profile_type),
      MODEM_MEM_CLIENT_DATA );

  if ((NULL == pdp_ptr) || (pdp_profile_ptr == NULL))
  {
    DS_3GPP_MSG0_LOW ("FAIL modem_mem_alloc for ds_umts_pdp_profile_type");
    return DSAT_ERROR;
  }

  if(!ds_3gpp_rmsm_ati_pdp_state_type_validate(pdp_ptr))
  {
     DS_3GPP_MSG0_LOW ("Not a valid instance ds_3gpp_rmsm_ati_pdp_state");
  } 


  
  memset (pdp_profile_ptr,0x0,sizeof(ds_umts_pdp_profile_type));
  status = ds_umts_get_pdp_profile_all_data_per_subs( 
                                             (uint16)pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->profile_id,
                                              dsumts_subs_mgr_get_subs_id(pdp_ptr->
                                                                ds_3gpp_ati_pdp_state_type_dyn_p->subs_id),
                                             pdp_profile_ptr 
                                           );
    memset(&qos_modify, 0, sizeof(ps_flow_ioctl_qos_modify_type));
    memset(&app_qos, 0, sizeof(qos_spec_type));
	
  if(DS_UMTS_PDP_SUCCESS == status)
  {
    app_qos.rx.fltr_template.list_ptr = 
        (ip_filter_type *)modem_mem_calloc(MAX_PACKET_FILTERS,
                                           sizeof(ip_filter_type),
                                           MODEM_MEM_CLIENT_DATA);
    if(app_qos.rx.fltr_template.list_ptr == NULL)
    {
      DS_3GPP_MSG0_LOW ("Heap Memory Allocation failed");
      modem_mem_free((void *)pdp_profile_ptr, 
                     MODEM_MEM_CLIENT_DATA );
      return DSAT_ERROR;
    }   

    result = ds_3gpp_rmsm_ati_verify_qos_params(pdp_profile_ptr, &app_qos);

    if( (result != DSAT_ERROR) && 
        (FALSE == ds_3gpp_rmsm_ati_conv_tft_to_app_tft_from_id(
                                           DS_3GPP_RMSM_TFT_OPCODE_CREATE,
                                           pdp_profile_ptr->tft,
                                           0,
                                           DS_UMTS_MAX_TFT_PARAM_SETS,
                                           &app_qos)) )
    {
      DS_3GPP_MSG0_LOW ("Convertion to APP TFT failed");
      result = DSAT_ERROR;
    }
  }
  else
  {
    DS_3GPP_MSG0_LOW ("Unable to retrieve profile data");
    result = DSAT_ERROR;
  }

  if (DSAT_ERROR != result)
  {
    /* Register for flow Modify accept and 
       modify reject events */
    if(ds_3gpp_flow_reg_ev(flow_ev_mod_p,
                           DS_3GPP_RMSM_FLOW_MOD_EV_COUNT,
                           ds_3gpp_rmsm_flow_event_handler_cb,
                           NULL,
                           pdp_ptr) != TRUE)
    {
      DS_3GPP_MSG0_LOW ("Couldn't register for Flow Modify Accepted event!");
      result = DSAT_ERROR;
    }
  
   
    if(DSAT_ERROR != result)
    {
      pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->modify_pending = TRUE;

      /*-------------------------------------------------------------------------
        If Any TX filters present, check to see if the flow's current qos spec
        filed mask has any TX flows ( => it has TX filters) hence the filters are
        getting modified and so set the TX_FLTR_MODIFY_MASK
        If the qos filed mask has no TX flows, then filter is added 1st time, 
        so don't set MODIFY MASK
      -------------------------------------------------------------------------*/
      qos_spec_field_mask = PS_FLOW_GET_QOS_FIELD_MASK(pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->flow_ptr);
 
      if(app_qos.rx.fltr_template.num_filters > 0)
      {
        if(qos_spec_field_mask & (uint16)QOS_MASK_RX_FLOW)
        {
          app_qos.field_mask |= (uint16)QOS_MODIFY_MASK_RX_FLTR_MODIFY;
        }
      }
      /* Need to fill the qos_ptr in request info */
      qos_modify.qos_ptr = &app_qos;
    
      /* call PS iface API for flow modify request */
      ret_val = ps_flow_ioctl(pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->flow_ptr, 
                              PS_FLOW_IOCTL_QOS_MODIFY,
                              &qos_modify,
                              &ps_errno);

      if( ret_val != 0 )
      {
        DS_3GPP_MSG0_LOW ("Failed to modify the Qos");
        /* de-register all MODIFY handlers */
        PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
               /* de-register all MODIFY handlers */
       (void)ds_3gpp_flow_dereg_ev(flow_ev_mod_p,
                                   DS_3GPP_RMSM_FLOW_MOD_EV_COUNT,
                                   ds_3gpp_rmsm_flow_event_handler_cb,
                                   NULL);
        
        PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

        result = DSAT_ERROR;
        pdp_ptr->ds_3gpp_ati_pdp_state_type_dyn_p->modify_pending = FALSE;
      }
    }
  }

  modem_mem_free((void *)pdp_profile_ptr, 
                 MODEM_MEM_CLIENT_DATA );
  modem_mem_free((void *)app_qos.rx.fltr_template.list_ptr,
                 MODEM_MEM_CLIENT_DATA);
  return result;
} /* ds_3gpp_rmsm_ati_modify_sec_pdp_ctx */

#ifdef FEATURE_DATA_WCDMA_PS
/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_CONV_FLOW_TO_UMTS_APP_QOS_FROM_ID
  
  DESCRIPTION
    This Function is called for converting the QOS flow present 
    in the profile context info to UMTS APP QOS. 

  DEPENDENCIES
    None.
  
  RETURN VALUE
    TRUE: if conversion is Success
    FALSE : if conversion is Failure
  
  SIDE EFFECTS
    None
===========================================================================*/
LOCAL boolean ds_3gpp_rmsm_ati_conv_flow_to_umts_app_qos_from_id
(
  ds_umts_pdp_profile_type           *pdp_ptr,
  qos_spec_type                      *app_flow
)
{
  ps_flow_ioctl_primary_qos_get_granted_flow_spec_type* flow_spec_ptr;
  boolean result = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((pdp_ptr == NULL) || (app_flow == NULL))
  {
    return FALSE;
  }

  if( ( pdp_ptr->qos_request_umts.valid_flg == FALSE) &&
      ( pdp_ptr->qos_minimum_umts.valid_flg == FALSE) )
  {
    DS_3GPP_MSG0_LOW ("No valid UMTS QOS specified in profile, return FALSE");
    return FALSE;
  }
  flow_spec_ptr = 
       (ps_flow_ioctl_primary_qos_get_granted_flow_spec_type *)modem_mem_alloc(
        sizeof(ps_flow_ioctl_primary_qos_get_granted_flow_spec_type),
        MODEM_MEM_CLIENT_DATA );
  if ( NULL == flow_spec_ptr)
  {
    DS_3GPP_MSG0_LOW ("FAIL modem_mem_alloc for flow_spec_ptr");
    return FALSE;
  }

  /*--------------------------------------------------------------------------
                Convert REQ QoS parameters 
  ---------------------------------------------------------------------------*/
  if( pdp_ptr->qos_request_umts.valid_flg == TRUE)
  {
    DS_3GPP_MSG0_LOW ("Converting UMTS request QOS to APP Qos");
    memset( flow_spec_ptr,0x0,
            sizeof(ps_flow_ioctl_primary_qos_get_granted_flow_spec_type));
    result = dsumtsps_conv_umts_qos_to_app_qos(&(pdp_ptr->qos_request_umts),
                                            flow_spec_ptr);
    if(result == TRUE)
    {
        app_flow->field_mask |= (uint16)QOS_MASK_RX_FLOW;
        //app_flow->field_mask |= (uint16)QOS_MASK_TX_FLOW;
        app_flow->rx.flow_template.req_flow = flow_spec_ptr->rx_ip_flow;
        app_flow->tx.flow_template.req_flow = flow_spec_ptr->tx_ip_flow;
    }
  }
  /*--------------------------------------------------------------------------
                Convert MIN QoS parameters 
  ---------------------------------------------------------------------------*/
  if( (result == TRUE) && 
      (pdp_ptr->qos_minimum_umts.valid_flg == TRUE) )
  {
    DS_3GPP_MSG0_LOW ("Converting UMTS Minimum QOS to APP Qos");
    memset( flow_spec_ptr,0x0,
          sizeof(ps_flow_ioctl_primary_qos_get_granted_flow_spec_type));
    result = dsumtsps_conv_umts_qos_to_app_qos(&(pdp_ptr->qos_minimum_umts),
                                            flow_spec_ptr);
    if (result == TRUE)
    {
      app_flow->field_mask |= (uint16)QOS_MASK_RX_FLOW;
      //app_flow->field_mask |= (uint16)QOS_MASK_TX_FLOW;
      app_flow->rx.flow_template.min_req_flow = flow_spec_ptr->rx_ip_flow;
      app_flow->tx.flow_template.min_req_flow = flow_spec_ptr->tx_ip_flow;
    }
  }
  modem_mem_free((void *)flow_spec_ptr, MODEM_MEM_CLIENT_DATA );
  return result;
}/* ds_3gpp_rmsm_ati_conv_flow_to_umts_app_qos_from_id */
#endif /* FEATURE_DATA_WCDMA_PS */

#ifdef FEATURE_GSM_GPRS
/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_CONV_FLOW_TO_GPRS_APP_QOS_FROM_ID
    
  DESCRIPTION
    This Function is called for converting the QOS flow present 
    in the profile context info to GPRS APP QOS. 

  DEPENDENCIES
    None.
  
  RETURN VALUE
    TRUE: if conversion is Success
    FALSE : if conversion is Failure

  SIDE EFFECTS
    None
===========================================================================*/
LOCAL boolean ds_3gpp_rmsm_ati_conv_flow_to_gprs_app_qos_from_id
(
  ds_umts_pdp_profile_type           *pdp_ptr,
  qos_spec_type                      *app_flow
)
    {
  ps_flow_ioctl_primary_qos_get_granted_flow_spec_type* flow_spec_ptr;
  boolean result = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    
  if ((pdp_ptr == NULL) || (app_flow == NULL))
  {
    return FALSE;
  }

  if( ( pdp_ptr->qos_request_gprs.valid_flg == FALSE) &&
      ( pdp_ptr->qos_minimum_gprs.valid_flg == FALSE) )
  {
    DS_3GPP_MSG0_LOW ("No valid GPRS QOS specified in profile, return FALSE");
    return FALSE;
  }
  flow_spec_ptr = 
       (ps_flow_ioctl_primary_qos_get_granted_flow_spec_type *)modem_mem_alloc(
        sizeof(ps_flow_ioctl_primary_qos_get_granted_flow_spec_type),
        MODEM_MEM_CLIENT_DATA );
  if ( NULL == flow_spec_ptr)
  {
    DS_3GPP_MSG0_LOW ("FAIL modem_mem_alloc for flow_spec_ptr");
    return FALSE;
  }
  /*--------------------------------------------------------------------------
                Convert REQ QoS parameters 
  ---------------------------------------------------------------------------*/
  if( pdp_ptr->qos_request_gprs.valid_flg == TRUE)
  {
    DS_3GPP_MSG0_LOW ("Converting GPRS request QOS to APP Qos");
    memset( flow_spec_ptr,0x0,
            sizeof(ps_flow_ioctl_primary_qos_get_granted_flow_spec_type));
    result = dsumtsps_conv_gprs_qos_to_app_qos(&(pdp_ptr->qos_request_gprs),
                                            flow_spec_ptr);
    if (result == TRUE)
    {
      app_flow->field_mask |= (uint16)QOS_MASK_RX_FLOW;
      //app_flow->field_mask |= (uint16)QOS_MASK_TX_FLOW;
      app_flow->rx.flow_template.req_flow = flow_spec_ptr->rx_ip_flow;
      app_flow->tx.flow_template.req_flow = flow_spec_ptr->tx_ip_flow;
    }
  }

  /*--------------------------------------------------------------------------
                Convert MIN QoS parameters 
  ---------------------------------------------------------------------------*/
  if( pdp_ptr->qos_minimum_gprs.valid_flg == TRUE)
  {
    DS_3GPP_MSG0_LOW ("Converting GPRS Minimum QOS to APP Qos");
    memset( flow_spec_ptr,0x0,
            sizeof(ps_flow_ioctl_primary_qos_get_granted_flow_spec_type));
    (void)dsumtsps_conv_gprs_qos_to_app_qos(&(pdp_ptr->qos_request_gprs),
                                            flow_spec_ptr);
    if (result == TRUE)
    {
      app_flow->field_mask |= (uint16)QOS_MASK_RX_FLOW;
//      app_flow->field_mask |= (uint16)QOS_MASK_TX_FLOW;
      app_flow->rx.flow_template.min_req_flow = flow_spec_ptr->rx_ip_flow;
      app_flow->tx.flow_template.min_req_flow = flow_spec_ptr->tx_ip_flow;
    }
  }

  modem_mem_free((void *)flow_spec_ptr, MODEM_MEM_CLIENT_DATA );
  return TRUE;
}/* ds_3gpp_rmsm_ati_conv_flow_to_gprs_app_qos_from_id */
#endif /* FEATURE_GSM_GPRS */

/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_CONV_FLOW_TO_LTE_APP_QOS_FROM_ID

  DESCRIPTION
    This Function is called for converting the QOS flow present 
    in the profile context info to LTE APP QOS. 

  DEPENDENCIES
    None.

  RETURN VALUE
    TRUE: if conversion is Success
    FALSE : if conversion is Failure

  SIDE EFFECTS
    None
===========================================================================*/
boolean ds_3gpp_rmsm_ati_conv_flow_to_lte_app_qos_from_id
(
  ds_umts_pdp_profile_type           *pdp_ptr,
  qos_spec_type                      *app_flow
)
{
  ip_flow_type *app_rx_flow = NULL;
  boolean result = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((pdp_ptr == NULL) || (app_flow == NULL))
  {
    return FALSE;
  }

  /*--------------------------------------------------------------------------
                           Convert QoS parameters 
  ---------------------------------------------------------------------------*/

  app_rx_flow = &(app_flow->rx.flow_template.req_flow);

  if (pdp_ptr->qos_request_lte.valid_flg == TRUE)
  {
    /* convert LTE profile QoS to App Qos */

    /* RX Flow update */
    app_flow->field_mask |= (uint16)QOS_MASK_RX_FLOW;
    app_rx_flow->data_rate.format_type = DATA_RATE_FORMAT_MIN_MAX_TYPE;
    app_rx_flow->data_rate.format.min_max.max_rate = 
                                pdp_ptr->qos_request_lte.max_dl_bit_rate* 1000;
    app_rx_flow->data_rate.format.min_max.guaranteed_rate =
                                pdp_ptr->qos_request_lte.g_dl_bit_rate* 1000;
    if( (app_rx_flow->data_rate.format.min_max.max_rate!=0 ) ||
        (app_rx_flow->data_rate.format.min_max.guaranteed_rate) )
    {
      app_rx_flow->field_mask |=(int)(IPFLOW_MASK_DATA_RATE);
    }
    app_rx_flow->lte_params.lte_qci = 
                            (lte_qci_enum_type)pdp_ptr->qos_request_lte.qci;
    app_rx_flow->field_mask |=(int)(IPFLOW_MASK_LTE_QCI); 
    result = TRUE;
  }

  DS_3GPP_MSG2_LOW ("Converting LTE QOS - APP QoS, LTE valid flag %d, result %d",
                     pdp_ptr->qos_request_lte.valid_flg , result);
  return result;
}/* ds_3gpp_rmsm_ati_conv_to_lte_app_qos_from_id */

/*===========================================================================
FUNCTION DS_3GPP_RMSM_ATI_CONV_TFT_TO_APP_TFT_FROM_ID

DESCRIPTION
  This function fills in the Flow TFT parameters based on the operation code,
  requested profile TFT and filter offset and quantity information. 
  
PARAMETERS
  opcode     - Operation code.
  *req_tft   - Requested TFT to copy filter info from.
  filter_ofs - Offset at which to start pulling filter info from: invalid 
               packet filter definitions are skipped when counting the offset 
               and copying the number of filters given, so this is offet into
               an imaginary array filled only with valid packet filters.
  filter_qty - Number of filters to copy.
  *app_flow   - Apps Flow TFT to copy into.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  TRUE: if conversion is Success
  FALSE : if conversion is Failure
  
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_rmsm_ati_conv_tft_to_app_tft_from_id
(
  ds_3gpp_rmsm_tft_opcode_enum_type  opcode,
  ds_umts_tft_params_type      *req_tft,
  uint8                         filter_ofs,
  uint8                         filter_qty,
  qos_spec_type                *app_flow
)
{
  ip_filter_spec_type*     rx_fltr_template;
  uint8 efs_idx, orig_idx, fltr_cnt;
  uint16 src_port=0;
  uint16 src_range = 0;
  uint16 dest_port=0;
  uint16 dest_range =0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((req_tft == NULL) || (app_flow == NULL) || 
       ((filter_ofs + filter_qty) >= DS_UMTS_MAX_TFT_PARAM_SETS ))
  {
    return FALSE;
  }

  /*--------------------------------------------------------------------------
                           Convert TFT parameters 
  ---------------------------------------------------------------------------*/

  rx_fltr_template = &(app_flow->rx.fltr_template);

  /* Count "filter_ofs" valid packet filter definitions */
  for ( efs_idx = 0, fltr_cnt = 0; ;efs_idx++ )
  {
    if ( efs_idx >= DS_UMTS_MAX_TFT_PARAM_SETS )
    {
      return FALSE;
    }

    if ( req_tft[efs_idx].valid_flg )
    {
      fltr_cnt++;

      if ( fltr_cnt > filter_ofs )
      {
        break;
      }
    }
  }
  rx_fltr_template->num_filters = 0;

  /* req_idx set by above for loop to "filter_ofs-th" valid packet filter */
  /* Count and copy filter_qty valid packet filters... */
  for ( orig_idx = 0, fltr_cnt = 0; fltr_cnt < filter_qty;
        efs_idx++ )
  {  
    if ( efs_idx >= DS_UMTS_MAX_TFT_PARAM_SETS )
    {
      break;
    }
    /* Transfer info from Profile to Apps TFT */
    if ( req_tft[efs_idx].valid_flg )
    {
      if ( DS_3GPP_RMSM_TFT_OPCODE_DEL_FILTER != opcode )
      {
        app_flow->field_mask |= (uint16)QOS_MASK_RX_FLOW;
#ifdef FEATURE_DATA_PS_IPV6
        if ( DS_IP_V6 == req_tft[efs_idx].src_addr_mask.address.ip_vsn )
        {
          rx_fltr_template->list_ptr[orig_idx].ip_vsn = IP_V6;
          if(req_tft[efs_idx].src_addr_mask.mask.prefix_len != 0)
          {
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v6.field_mask |= 
               (int)IPFLTR_MASK_IP6_SRC_ADDR;
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v6.src.prefix_len =
              req_tft[efs_idx].src_addr_mask.mask.prefix_len;

            DS_3GPP_RMSM_ATI_BYTE_REV_CPY(
               rx_fltr_template->list_ptr[orig_idx].ip_hdr.v6.src.addr.\
                  in6_u.u6_addr8,
               req_tft[efs_idx].src_addr_mask.address.pdp_addr.pdp_addr_ipv6.\
                  in6_u.u6_addr8,
               sizeof(req_tft[efs_idx].src_addr_mask.address.pdp_addr.\
                          pdp_addr_ipv6.in6_u.u6_addr8));

          }
          if(req_tft[efs_idx].flow_label != 0)
          {
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v6.flow_label =
              req_tft[efs_idx].flow_label;
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v6.field_mask |= 
              (uint32)IPFLTR_MASK_IP6_FLOW_LABEL;
          }
          else if (req_tft[efs_idx].prot_num != 0)
          {
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v6.next_hdr_prot = 
              req_tft[efs_idx].prot_num;
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v6.field_mask |=
              (int)IPFLTR_MASK_IP6_NEXT_HDR_PROT;
          }
          if(req_tft[efs_idx].tos_mask != 0)
          {
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v6.trf_cls.val =  
              ((req_tft[efs_idx].tos_mask & 0xFF00) >> 8);
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v6.trf_cls.mask = 
              (req_tft[efs_idx].tos_mask & 0x00FF);
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v6.field_mask |= 
              (uint32)IPFLTR_MASK_IP6_TRAFFIC_CLASS;
          }
        } else
#endif /* FEATURE_DATA_PS_IPV6 */
        {
          rx_fltr_template->list_ptr[orig_idx].ip_vsn = IP_V4;
          if(req_tft[efs_idx].src_addr_mask.address.pdp_addr.pdp_addr_ipv4 != 0)
          {
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v4.field_mask = 
                 (ipfltr_ip4_hdr_field_mask_type)IPFLTR_MASK_IP4_SRC_ADDR;
            /* convert to host format */
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v4.src.addr.ps_s_addr =
               ps_ntohl(req_tft[efs_idx].src_addr_mask.address.\
                            pdp_addr.pdp_addr_ipv4);
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v4.src.subnet_mask.\
                                                               ps_s_addr=
               ps_ntohl(req_tft[efs_idx].src_addr_mask.mask.mask);
          }
          if(req_tft[efs_idx].tos_mask != 0)
          {
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v4.field_mask |= 
                 (ipfltr_ip4_hdr_field_mask_type)IPFLTR_MASK_IP4_TOS;
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v4.tos.val =  
                   ((req_tft[efs_idx].tos_mask & 0xFF00) >> 8);
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v4.tos.mask = 
                   (req_tft[efs_idx].tos_mask & 0x00FF);
          }
          if(req_tft[efs_idx].prot_num != 0)
          {
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v4.next_hdr_prot = 
                        req_tft[efs_idx].prot_num;
            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v4.field_mask |= 
                                    (int)IPFLTR_MASK_IP4_NEXT_HDR_PROT;
            DS_3GPP_MSG3_LOW ("TOS value = %d, TOS mask = %d",
                            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v4.tos.val,
                            rx_fltr_template->list_ptr[orig_idx].ip_hdr.v4.tos.mask,
                            0);
          }
        }
        /* Set the Packet Filter Identifier Mask : Destination Port */
        if ( req_tft[efs_idx].dest_port_range.from < 
             req_tft[efs_idx].dest_port_range.to )
        {
          dest_port = req_tft[efs_idx].dest_port_range.from;
          dest_range = (req_tft[efs_idx].dest_port_range.to -
                        req_tft[efs_idx].dest_port_range.from);
        }
        else if ( req_tft[efs_idx].dest_port_range.from != 0 )
        {
          dest_port = req_tft[efs_idx].dest_port_range.from;
          dest_range = 0;
        }
        /* Set the Packet Filter Identifier Mask : Source Port */
        if ( req_tft[efs_idx].src_port_range.from < 
             req_tft[efs_idx].src_port_range.to )
        {
          src_port = req_tft[efs_idx].src_port_range.from;
          src_range = (req_tft[efs_idx].src_port_range.to -
                       req_tft[efs_idx].src_port_range.from);
        }
        else if ( req_tft[efs_idx].src_port_range.from != 0 )    
        {
          src_port = req_tft[efs_idx].src_port_range.from;
          src_range = 0;
        }
        /* convert to host format port details */
        if(req_tft[efs_idx].prot_num ==(byte)PS_IPPROTO_TCP)
        {
          if( dest_port != 0 ) 
          {
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.tcp.dst.port = 
              ps_ntohs(dest_port);
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.tcp.dst.range = 
              (dest_range);
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.tcp.field_mask |=
            (uint8)IPFLTR_MASK_TCP_DST_PORT;
          }
          if( src_port != 0 )
          {
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.tcp.src.port = 
               ps_ntohs(src_port);
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.tcp.src.range = 
               (src_range);
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.tcp.field_mask |=
            (uint8)IPFLTR_MASK_TCP_SRC_PORT;
          }
        }
        else if(req_tft[efs_idx].prot_num ==(byte)PS_IPPROTO_UDP)
        {
          if( dest_port != 0 ) 
          {
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.dst.port = 
              ps_ntohs(dest_port);
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.dst.range = 
              (dest_range);
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.field_mask |=
                                         (int)IPFLTR_MASK_UDP_DST_PORT;
          }
          if( src_port != 0 )
          {
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.src.port = 
              ps_ntohs(src_port);
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.src.range = 
              (src_range);
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.field_mask |=
                                         (int)IPFLTR_MASK_UDP_SRC_PORT;
          }
        }
        else
        {
          if( dest_port != 0 )
          { 
          /*use either TCP or UDP as both are of same format and size*/
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.dst.port =
            ps_ntohs(dest_port);
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.dst.range =
                0;
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.field_mask |=
                                         (int)IPFLTR_MASK_UDP_DST_PORT;
          }
          if( src_port != 0 )
          {
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.src.port =
            ps_ntohs(src_port);
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.src.range =
                0;
          rx_fltr_template->list_ptr[orig_idx].next_prot_hdr.udp.field_mask |=
                                         (int)IPFLTR_MASK_UDP_SRC_PORT;
          }
        }

        rx_fltr_template->list_ptr[orig_idx].ipfltr_aux_info.fi_id = 0;

        if( DS_MAX_FI_PRECED > req_tft[efs_idx].eval_prec_id )
        {
          rx_fltr_template->list_ptr[orig_idx].ipfltr_aux_info.fi_precedence =
                      req_tft[efs_idx].eval_prec_id;
        }
        else
        {
          rx_fltr_template->list_ptr[orig_idx].ipfltr_aux_info.fi_precedence = 0;
        }

      }/* ( DS_UMTS_TFT_OPCODE_DEL_FILTER != opcode ) */
      rx_fltr_template->num_filters ++;
      orig_idx++;
      fltr_cnt++;
    }/* ( req_tft[efs_idx].valid_flg )*/
  }/* for loop ending */
  
  return TRUE;
}/* ds_3gpp_rmsm_ati_conv_flow_to_lte_app_qos_from_id */

/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_READ_PRIM_DYNAMIC_PARAMS
  
  DESCRIPTION
    This function returns the primary dynamic params requested 
    by ATCoP for LTE.

  DEPENDENCIES
    None.
  
  RETURN VALUE
    FALSE: in case of Failure
    TRUE: If retrieval of dynamic data is success.
  
  SIDE EFFECTS
    None
===========================================================================*/
boolean ds_3gpp_rmsm_ati_read_prim_dynamic_params
(
  uint32                                      profile_number,
  sys_modem_as_id_e_type                      subs_id,
  ds_3gpp_atif_dynamic_params_info_type      *dynamic_param_ptr
)
{
  ds_3gpp_ati_pdn_dynamic_info_type     pdn_dynamic_parm = {0};
  ds_3gpp_atif_primary_dynamic_info_type*  prim_info_ptr ;
  ip_addr_type            prim_dns_addr ;
  ip_addr_type            sec_dns_addr ;
  ps_ip_addr_type         gateway_addr ;
  ip_addr_type            pcscf_addr[DS_3GPP_ATIF_MAX_PCSCF_SERVERS];
  uint32 count = DS_3GPP_ATIF_MAX_PCSCF_SERVERS;
  uint32  v6_count = DS_3GPP_ATIF_MAX_PCSCF_SERVERS;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (dynamic_param_ptr == NULL)
  {
    return FALSE;
  }
    /* Query MH to check whether the profile is active */
  if( DS_PDN_CONTEXT_STATE_DOWN == 
        ds_pdn_cntxt_get_pdn_context_state(profile_number, subs_id))
  {
    /* Not an activated context return false */
    return FALSE;
  }

  if(FALSE == ds_pdn_cntxt_get_ati_dynamic_info(profile_number,
                                                subs_id,
                                          &pdn_dynamic_parm))
  {
    return FALSE;
  }

  DS_3GPP_MSG1_MED("Returning DS_3GPP_ATIF_PRIM_INFO for profile no %d", 
                   profile_number);
  prim_info_ptr = &(dynamic_param_ptr->u.prim_dynamic_info);
  dynamic_param_ptr->info_type = DS_3GPP_ATIF_PRIM_INFO;

  /* Populate bearer ID, APN */
  prim_info_ptr->bearer_id = pdn_dynamic_parm.bearer_id;
  memscpy(&(prim_info_ptr->apn[0]), sizeof(prim_info_ptr->apn),
  	      &(pdn_dynamic_parm.apn[0]),
                  sizeof(prim_info_ptr->apn));

  /* get PDP address */
  (void)ds_pdn_cntxt_get_pdn_context_pdp_addr(profile_number,
                                                     subs_id,
                                &(prim_info_ptr->ip_addr));

  prim_dns_addr.type = IP_ADDR_INVALID;
  sec_dns_addr.type = IP_ADDR_INVALID;

  /* Get V4 DNS , gateway address, Primary and secondary CSCF addr */
  if(pdn_dynamic_parm.v4_iface_ptr != NULL)
  {
    ps_iface_get_v4_dns_addrs(pdn_dynamic_parm.v4_iface_ptr,
                              &prim_dns_addr,
                              &sec_dns_addr);
    if ( IP_ADDR_INVALID != prim_dns_addr.type )
    {
      //DS_IP_V4;
      memscpy(&(prim_info_ptr->v4_ipcp.ipcp4_options.primary_dns),
              sizeof(prim_info_ptr->v4_ipcp.ipcp4_options.primary_dns),
             &(prim_dns_addr.addr.v4),sizeof( uint32));
    }
    if ( IP_ADDR_INVALID != sec_dns_addr.type )
    {
       // DS_IP_V4
      memscpy(&(prim_info_ptr->v4_ipcp.ipcp4_options.secondary_dns),
              sizeof( prim_info_ptr->v4_ipcp.ipcp4_options.secondary_dns),
             &(sec_dns_addr.addr.v4),sizeof( uint32));
    }

    /* Gate way address */
    gateway_addr.type = IPV4_ADDR;
    ps_iface_get_peer_addr( pdn_dynamic_parm.v4_iface_ptr,
                            &gateway_addr );
    if (gateway_addr.type == IPV4_ADDR)
    {
      memscpy(&(prim_info_ptr->v4_ipcp.ipcp4_options.gateway_addr),
	  	      sizeof(prim_info_ptr->v4_ipcp.ipcp4_options.gateway_addr),
             &(gateway_addr.addr.v4.ps_s_addr),
             sizeof( uint32));
    }

    memset(&pcscf_addr[0],0x0,sizeof(pcscf_addr));
    /* CSCF address */
    ps_iface_get_sip_serv_addr(pdn_dynamic_parm.v4_iface_ptr, 
                               &pcscf_addr[0], 
                               &count);
    if(pcscf_addr[0].type == IPV4_ADDR)
    {
      /* copy the first into primary and second to Scscf */
      prim_info_ptr->prim_pcscf_addr.ip_vsn = DS_IP_V4;
      memscpy( &(prim_info_ptr->prim_pcscf_addr.pdp_addr.pdp_addr_ipv4),
	  	       sizeof(prim_info_ptr->prim_pcscf_addr.pdp_addr.pdp_addr_ipv4),
              &(pcscf_addr[0].addr.v4),
              sizeof(uint32));
    }
    if(pcscf_addr[1].type == IPV4_ADDR)
    {
      prim_info_ptr->sec_pcscf_addr.ip_vsn = DS_IP_V4;
      memscpy( &(prim_info_ptr->sec_pcscf_addr.pdp_addr.pdp_addr_ipv4),
	  	       sizeof(prim_info_ptr->sec_pcscf_addr.pdp_addr.pdp_addr_ipv4),
              &(pcscf_addr[1].addr.v4),
              sizeof(uint32));
    }
  }

  /* Get V6 DNS address */
  prim_dns_addr.type = IP_ADDR_INVALID;
  sec_dns_addr.type = IP_ADDR_INVALID;

  if(pdn_dynamic_parm.v6_iface_ptr != NULL)
  {
    ps_iface_get_v6_dns_addrs(pdn_dynamic_parm.v6_iface_ptr,
                              &prim_dns_addr,
                              &sec_dns_addr);
    if ( IP_ADDR_INVALID != prim_dns_addr.type )
    {
      //DS_IP_V6
      memscpy(&(prim_info_ptr->v6_ipcp.ipv6cp_options.primary_dns.ps_s6_addr64[0]),
              (sizeof(uint64)*2),
             &(prim_dns_addr.addr.v6[0]),
             (sizeof(uint64)*2));
    }
    if ( IP_ADDR_INVALID != sec_dns_addr.type )
    {
      //DS_IP_V6
      memscpy(&(prim_info_ptr->v6_ipcp.ipv6cp_options.secondary_dns.ps_s6_addr64[0]),
              (sizeof(uint64)*2),
             &(sec_dns_addr.addr.v6[0]),
             (sizeof(uint64)*2));
    }

    gateway_addr.type = IPV6_ADDR;
    ps_iface_get_peer_addr( pdn_dynamic_parm.v6_iface_ptr,
                            &gateway_addr );
    if (gateway_addr.type == IPV6_ADDR)
    {
      memscpy(&(prim_info_ptr->v6_ipcp.ipv6cp_options.gateway_addr.ps_s6_addr64[0]),
	  	      (sizeof(uint64)*2),
             &(gateway_addr.addr.v6.ps_s6_addr64[0]),
             (sizeof(uint64)*2));
    }

    memset(&pcscf_addr[0],0x0,sizeof(pcscf_addr));
    /* V6 CSCF address */
    ps_iface_get_sip_serv_addr(pdn_dynamic_parm.v6_iface_ptr, 
                               &pcscf_addr[0], 
                               &v6_count);

    if(pcscf_addr[0].type == IPV6_ADDR)
    {
      /* copy the first into primary and second to Scscf */
      prim_info_ptr->prim_pcscf_addr.ip_vsn = (ds_ip_version_enum_type)
                    ((int) prim_info_ptr->prim_pcscf_addr.ip_vsn + (int)DS_IP_V6);
      memscpy( &(prim_info_ptr->prim_pcscf_addr.pdp_addr.pdp_addr_ipv6.in6_u.u6_addr64[0]),
	  	       (sizeof(uint64)*2),
              &(pcscf_addr[0].addr.v6[0]),
              (sizeof(uint64)*2));
    }
    if(pcscf_addr[1].type == IPV6_ADDR)
    {
      prim_info_ptr->sec_pcscf_addr.ip_vsn = (ds_ip_version_enum_type)
                    ((int) prim_info_ptr->sec_pcscf_addr.ip_vsn + (int)DS_IP_V6);
      memscpy( &(prim_info_ptr->sec_pcscf_addr.pdp_addr.pdp_addr_ipv6.in6_u.u6_addr64[0]),
	  	       (sizeof(uint64)*2),
              &(pcscf_addr[1].addr.v6[0]),
              (sizeof(uint64)*2));
    }
  }
  return TRUE;
}/* ds_3gpp_rmsm_ati_read_prim_dynamic_params */


/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_CONVERT_APP_TFT_TO_PROFILE
  
  DESCRIPTION
    This function converts the APP TFT into Profile TFT.

  DEPENDENCIES
    None.
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
===========================================================================*/
void ds_3gpp_rmsm_ati_convert_app_tft_to_profile
(
  ip_filter_type        *ip_fltr_p,
  ds_umts_tft_params_type    *tft_p
)
{

  if ((ip_fltr_p == NULL) || (tft_p == NULL))
  {
    return;
  }
  
  tft_p->valid_flg = TRUE;
  /* last 4 bits is teh filter id, so mask rest of the bits. */
  tft_p->filter_id = (byte)((ip_fltr_p->ipfltr_aux_info.fi_id)& (0xF));
  tft_p->eval_prec_id = (byte)ip_fltr_p->ipfltr_aux_info.fi_precedence;

  if(ip_fltr_p->ip_vsn == IP_V6)
  {
    tft_p->src_addr_mask.address.ip_vsn = DS_IP_V6;
    if(ip_fltr_p->ip_hdr.v6.field_mask & (uint8)IPFLTR_MASK_IP6_NEXT_HDR_PROT)
    {
      tft_p->prot_num = ip_fltr_p->ip_hdr.v6.next_hdr_prot;
    }
    if(ip_fltr_p->ip_hdr.v6.field_mask & (uint8)IPFLTR_MASK_IP6_SRC_ADDR)
    {
      tft_p->src_addr_mask.mask.prefix_len = ip_fltr_p->ip_hdr.v6.src.prefix_len;

      memscpy( (void*)&(tft_p->src_addr_mask.address.pdp_addr.\
                       pdp_addr_ipv6.in6_u),
                      (sizeof(uint64)*2),
              (void*)&(ip_fltr_p->ip_hdr.v6.\
                          src.addr.in6_u),
             sizeof(uint64)*2);
    }
    if(ip_fltr_p->ip_hdr.v6.field_mask & (uint8)IPFLTR_MASK_IP6_FLOW_LABEL)
    {
      tft_p->flow_label = ip_fltr_p->ip_hdr.v6.flow_label;
    }

    if(ip_fltr_p->ip_hdr.v6.field_mask & (uint8)IPFLTR_MASK_IP6_TRAFFIC_CLASS)
    {
      tft_p->tos_mask = (ip_fltr_p->ip_hdr.v6.trf_cls.val) <<8;
      tft_p->tos_mask |= ip_fltr_p->ip_hdr.v6.trf_cls.mask; 
    }
  }/* Ipv6 */
  else
  {
    tft_p->src_addr_mask.address.ip_vsn = DS_IP_V4;
    if(ip_fltr_p->ip_hdr.v4.field_mask & (uint8)IPFLTR_MASK_IP4_SRC_ADDR)
    {
      tft_p->src_addr_mask.address.pdp_addr.pdp_addr_ipv4 =
        ip_fltr_p->ip_hdr.v4.src.addr.ps_s_addr ;
      tft_p->src_addr_mask.mask.mask = 
        ip_fltr_p->ip_hdr.v4.src.subnet_mask.ps_s_addr;
    }
    if(ip_fltr_p->ip_hdr.v4.field_mask & (uint8)IPFLTR_MASK_IP4_TOS)
    {
      tft_p->tos_mask = (ip_fltr_p->ip_hdr.v4.tos.val) <<8;
      tft_p->tos_mask |= ip_fltr_p->ip_hdr.v4.tos.mask ;
    }
    if (ip_fltr_p->ip_hdr.v4.field_mask &(uint8)IPFLTR_MASK_IP4_NEXT_HDR_PROT)
    {
      tft_p->prot_num = ip_fltr_p->ip_hdr.v4.next_hdr_prot ;
    }
  }/* Ipv4 */

  DS_3GPP_MSG1_LOW ("TOS = %d",tft_p->tos_mask);

  if(tft_p->prot_num == (byte)PS_IPPROTO_TCP)
  {
    if(ip_fltr_p->next_prot_hdr.tcp.field_mask &(uint8)IPFLTR_MASK_TCP_DST_PORT)
    {
      tft_p->dest_port_range.from = ip_fltr_p->next_prot_hdr.tcp.dst.port;
      tft_p->dest_port_range.to = ip_fltr_p->next_prot_hdr.tcp.dst.port+
                            ip_fltr_p->next_prot_hdr.tcp.dst.range ;
    }
    if(ip_fltr_p->next_prot_hdr.tcp.field_mask & (uint8)IPFLTR_MASK_TCP_SRC_PORT)
    {
      tft_p->src_port_range.from = ip_fltr_p->next_prot_hdr.tcp.dst.port;
      tft_p->src_port_range.to = ip_fltr_p->next_prot_hdr.tcp.dst.port+
                            ip_fltr_p->next_prot_hdr.tcp.dst.range ;
    }
  }/* TCP */
  else if (tft_p->prot_num ==(byte)PS_IPPROTO_UDP)
  {
    if(ip_fltr_p->next_prot_hdr.udp.field_mask & (uint8)IPFLTR_MASK_TCP_DST_PORT)
    {
      tft_p->dest_port_range.from = ip_fltr_p->next_prot_hdr.udp.dst.port;
      tft_p->dest_port_range.to = ip_fltr_p->next_prot_hdr.udp.dst.range ;
    }
    if(ip_fltr_p->next_prot_hdr.udp.field_mask & (uint8)IPFLTR_MASK_TCP_SRC_PORT)
    {
      tft_p->src_port_range.from = ip_fltr_p->next_prot_hdr.udp.dst.port;
      tft_p->src_port_range.to = ip_fltr_p->next_prot_hdr.udp.dst.range ;
    }
  }/* UDP */
  else if (tft_p->prot_num ==(byte)PS_IPPROTO_ESP)
  {
    if(ip_fltr_p->next_prot_hdr.esp.field_mask & (uint8)IPFLTR_MASK_ESP_SPI)
    {
      tft_p->ipsec_spi = (uint16)ip_fltr_p->next_prot_hdr.esp.spi;
    }
  }
}/* ds_3gpp_rmsm_ati_convert_app_tft_to_profile */

/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_CHECK_AND_UPDATE_DUPLICATE_FI_ID
  
  DESCRIPTION
    This function checks for duplicate filter id and updates
     the direction to bi-direction if a duplicate is found.

  DEPENDENCIES
    None.
  
  RETURN VALUE
    TRUE -- if duplicate id is found
    FALSE -- if no id is found on current list
  
  SIDE EFFECTS
    None
===========================================================================*/
boolean ds_3gpp_rmsm_ati_check_and_update_duplicate_fi_id
(
  uint8                         filter_id,
  uint8                         no_of_tft,
  ds_3gpp_atif_tft_info_type    *tft_list_p
)
{
  uint8 loop =0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (tft_list_p == NULL)
  {
    return FALSE;
  }
  for(loop=0;loop <no_of_tft;loop++)
  {
    if(tft_list_p[loop].tft_params.filter_id == filter_id)
    {
      DS_3GPP_MSG2_LOW ("duplicate filter id(%d) found @ index %d",filter_id,loop);
      tft_list_p[loop].direction =  DS_3GPP_TFT_BIDIRECTIONAL;
      return TRUE;
    }
  }
  return FALSE;
}/* ds_3gpp_rmsm_ati_check_and_update_duplicate_fi_id */

/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_READ_SEC_DYNAMIC_PARAMS
  
  DESCRIPTION
    This function returns the secondary dynamic params requested 
    by ATCoP for LTE.

  DEPENDENCIES
    None.
  
  RETURN VALUE
    FALSE: in case of Failure
    TRUE: If retrieval of dynamic data is success.
  
  SIDE EFFECTS
    None
===========================================================================*/
boolean ds_3gpp_rmsm_ati_read_sec_dynamic_params
(
  uint32                                      profile_number,
  sys_modem_as_id_e_type                      subs_id,
  ds_3gpp_atif_dynamic_e_type                 info_type,
  ds_3gpp_atif_dynamic_params_info_type      *dynamic_param_ptr
)
{
  ds_3gpp_lte_qos_params_type *    lte_qos_p;
  ip_flow_type *app_rx_flow = NULL;
  ip_flow_type *app_tx_flow = NULL;
  ds_3gpp_atif_tft_info_type* tft_info_p;
  uint8 loop = 0;
  uint8 index=0;
  uint8 max_filters =0;
  boolean nw_flow = FALSE;
  ip_filter_spec_type*     rx_fltr_template;
  ip_filter_spec_type*     tx_fltr_template;
  ds_flow_dynamic_info_s*    filter_info_p;
  ip_filter_type*  rx_fltr_p= NULL;  // rx filter list
  ip_filter_type* tx_fltr_p = NULL;  // tx filter list
  
  boolean result = TRUE;
  uint8   filter_id =0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if (dynamic_param_ptr == NULL)
  {
    return FALSE;
  }
  filter_info_p = (ds_flow_dynamic_info_s *)modem_mem_alloc(
      sizeof(ds_flow_dynamic_info_s),
      MODEM_MEM_CLIENT_DATA );

  if (filter_info_p == NULL) 
  {
    return FALSE;
  }

  rx_fltr_p = (ip_filter_type *)modem_mem_alloc(
      (MAX_PACKET_FILTERS * sizeof(ip_filter_type)),
      MODEM_MEM_CLIENT_DATA );

  if(rx_fltr_p== NULL )
  {
    modem_mem_free((void *)filter_info_p, MODEM_MEM_CLIENT_DATA );
    return FALSE;
  }

  tx_fltr_p = (ip_filter_type *)modem_mem_alloc(
      (MAX_PACKET_FILTERS * sizeof(ip_filter_type)),
      MODEM_MEM_CLIENT_DATA );

  if(tx_fltr_p== NULL )
  {
    modem_mem_free((void *)filter_info_p, MODEM_MEM_CLIENT_DATA );
    modem_mem_free((void *)rx_fltr_p, MODEM_MEM_CLIENT_DATA );
    return FALSE;
  }
 
  memset (dynamic_param_ptr,0x0,sizeof(ds_3gpp_atif_dynamic_params_info_type));
  memset(rx_fltr_p, 0x0, sizeof(ip_filter_type)*MAX_PACKET_FILTERS);
  memset(tx_fltr_p, 0x0, sizeof(ip_filter_type)*MAX_PACKET_FILTERS);
  memset(filter_info_p, 0x0, sizeof(ds_flow_dynamic_info_s));

  filter_info_p->qos_spec.rx.fltr_template.list_ptr = rx_fltr_p;
  filter_info_p->qos_spec.tx.fltr_template.list_ptr = tx_fltr_p;

  if( FALSE == ds_flow_context_get_ati_dynamic_info(profile_number,
                                                    subs_id,
                                                    filter_info_p))
  {
    modem_mem_free((void *)filter_info_p, MODEM_MEM_CLIENT_DATA );
    modem_mem_free((void *)tx_fltr_p, MODEM_MEM_CLIENT_DATA );
    modem_mem_free((void *)rx_fltr_p, MODEM_MEM_CLIENT_DATA );
    return FALSE;
  }

  DS_3GPP_MSG1_LOW ("TFT info type %d",info_type);

  if (info_type == DS_3GPP_ATIF_SEC_INFO)
  {
    dynamic_param_ptr->info_type =DS_3GPP_ATIF_SEC_INFO;
    dynamic_param_ptr->u.sec_dynamic_info.cid = profile_number;
    dynamic_param_ptr->u.sec_dynamic_info.p_cid =   
                          filter_info_p->p_cid;
    dynamic_param_ptr->u.sec_dynamic_info.bearer_id =
                          filter_info_p->bearer_id;
    result = TRUE;
  }
  else if (info_type == DS_3GPP_ATIF_QOS_INFO)
  {
    dynamic_param_ptr->info_type = DS_3GPP_ATIF_QOS_INFO;

    lte_qos_p = &(dynamic_param_ptr ->u.dynamic_qos);
    app_rx_flow = &(filter_info_p->qos_spec.rx.flow_template.req_flow);
    app_tx_flow = &(filter_info_p->qos_spec.tx.flow_template.req_flow);

    lte_qos_p->valid_flg = TRUE;
    if (app_rx_flow->field_mask & (uint32)IPFLOW_MASK_LTE_QCI)
    {
      lte_qos_p->qci = (ds_3gpp_lte_qci_e_type )app_rx_flow->lte_params.lte_qci;
    }

    if(app_rx_flow->field_mask & (uint32)IPFLOW_MASK_DATA_RATE)
    {
      if (app_rx_flow->data_rate.format_type == DATA_RATE_FORMAT_MIN_MAX_EX_TYPE) 
      {
        lte_qos_p->max_dl_bit_rate = 
          (app_rx_flow->data_rate.format.min_max_ex.max_rate)/1000;
        lte_qos_p->g_dl_bit_rate = 
          (app_rx_flow->data_rate.format.min_max_ex.guaranteed_rate)/1000;
      }
      else
      {
        lte_qos_p->max_dl_bit_rate = 
          (app_rx_flow->data_rate.format.min_max.max_rate)/1000;
        lte_qos_p->g_dl_bit_rate = 
          (app_rx_flow->data_rate.format.min_max.guaranteed_rate)/1000;
      }
    }
    
    if(app_tx_flow->field_mask & (uint32)IPFLOW_MASK_DATA_RATE)
    {
      if (app_tx_flow->data_rate.format_type == DATA_RATE_FORMAT_MIN_MAX_EX_TYPE) 
      {
         lte_qos_p->max_ul_bit_rate = 
            (app_tx_flow->data_rate.format.min_max_ex.max_rate)/1000;
         lte_qos_p->g_ul_bit_rate = 
             (app_tx_flow->data_rate.format.min_max_ex.guaranteed_rate)/1000;
      }
      else
      {
        lte_qos_p->max_ul_bit_rate = 
          (app_tx_flow->data_rate.format.min_max.max_rate)/1000;
        lte_qos_p->g_ul_bit_rate = 
          (app_tx_flow->data_rate.format.min_max.guaranteed_rate)/1000;
      }
      result = TRUE;
    }
  }
  /* request for TFT info */
  else if (info_type == DS_3GPP_ATIF_TFT_INFO)
  {
    dynamic_param_ptr->info_type = DS_3GPP_ATIF_TFT_INFO;
    tft_info_p = &(dynamic_param_ptr->u.dynamic_tft[0]);
    rx_fltr_template = &(filter_info_p->qos_spec.rx.fltr_template);
    tx_fltr_template = &(filter_info_p->qos_spec.tx.fltr_template);

    if(filter_info_p->flow_type & DS_3GPP_NW_FLOW_TYPE)
    {
      nw_flow = TRUE;
    }
    index =0;
    if(filter_info_p->qos_spec.field_mask & (uint16)QOS_MASK_RX_FLOW)
    {
      max_filters =  MIN(DS_TFT_MAX_FILTERS,
                          rx_fltr_template->num_filters);
      for(loop =0;loop < max_filters;loop++)
      {
        tft_info_p[index].direction = DS_3GPP_TFT_DOWNLINK;
        tft_info_p[index].nw_flow   = nw_flow;
        ds_3gpp_rmsm_ati_convert_app_tft_to_profile(
                                      &(rx_fltr_template->list_ptr[loop]),
                                      &(tft_info_p[index].tft_params));
        index++;
      }/* for loop .. */
    }/* (app_qos.field_mask &QOS_MASK_RX_FLOW) */

    if(filter_info_p->qos_spec.field_mask & (uint16)QOS_MASK_TX_FLOW)
    {
      max_filters =  MIN(DS_TFT_MAX_FILTERS,
                          tx_fltr_template->num_filters);
      for(loop =0; (loop < max_filters) && (index < DS_3GPP_MAX_FILTERS); loop++)
      {
        /* Check if there is rx filter already,
           then update that filter direction as bi-directional
           and start processing next 
        */
        filter_id = (uint8)(tx_fltr_template->list_ptr[loop].ipfltr_aux_info.fi_id & (0xF));
        if(FALSE == ds_3gpp_rmsm_ati_check_and_update_duplicate_fi_id(
                           filter_id,
                           index,
                           tft_info_p ))
        {
          tft_info_p[index].direction = DS_3GPP_TFT_UPLINK;
          
          ds_3gpp_rmsm_ati_convert_app_tft_to_profile(
                                        &(tx_fltr_template->list_ptr[loop]),
                                        &(tft_info_p[index].tft_params));
          index++;
        }
      }/* for loop .. */
    }/* (app_qos.field_mask &QOS_MASK_RX_FLOW) */
    result = TRUE;
  }
  else
  {
    result = FALSE;
  }
  modem_mem_free((void *)filter_info_p, MODEM_MEM_CLIENT_DATA );
  modem_mem_free((void *)tx_fltr_p, MODEM_MEM_CLIENT_DATA );
  modem_mem_free((void *)rx_fltr_p, MODEM_MEM_CLIENT_DATA );
  
  return result;
}/* ds_3gpp_rmsm_ati_read_sec_dynamic_params */

/*===========================================================================
  FUNCTION   DS_3GPP_RMSM_ATI_READ_DYNAMIC_PARAMS
  
  DESCRIPTION
    This Function is called by ATCoP for reading the dynamic parameters 
    associated with the primary /secondary activated profiles .
    This function returns DSAT_ERROR if the profile is not activated .

  DEPENDENCIES
    None.
  
  RETURN VALUE
    FALSE: in case of Failure
    TRUE: If retrieval of dynamic data is success.
  
  SIDE EFFECTS
    None
===========================================================================*/
boolean ds_3gpp_rmsm_ati_read_dynamic_params
(
  uint32                                      profile_number,
  sys_modem_as_id_e_type                      subs_id,
  ds_3gpp_atif_dynamic_e_type                 info_type,
  ds_3gpp_atif_dynamic_params_info_type      *dynamic_param_ptr
)
{
  boolean result = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_3GPP_MSG1_MED( "In ds_3gpp_rmsm_ati_read_dynamic_params for porile no %d",
                    profile_number);
  if ( dynamic_param_ptr == NULL )
  {
    return result;
  }

  if(!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return result;
  }

  switch (info_type)
  {
    case DS_3GPP_ATIF_PRIM_INFO:
      result = ds_3gpp_rmsm_ati_read_prim_dynamic_params(profile_number,
                                                         subs_id,
                                                         dynamic_param_ptr);
      break;

    case DS_3GPP_ATIF_SEC_INFO:
      result = ds_3gpp_rmsm_ati_read_sec_dynamic_params(profile_number,
                                                        subs_id,
                                                      DS_3GPP_ATIF_SEC_INFO,
                                                      dynamic_param_ptr);
      break;

    case DS_3GPP_ATIF_QOS_INFO:
      result = ds_3gpp_rmsm_ati_read_sec_dynamic_params(profile_number,
                                                        subs_id,
                                                      DS_3GPP_ATIF_QOS_INFO,
                                                      dynamic_param_ptr);
      break;

    case DS_3GPP_ATIF_TFT_INFO:
      result = ds_3gpp_rmsm_ati_read_sec_dynamic_params(profile_number,
                                                        subs_id,
                                                      DS_3GPP_ATIF_TFT_INFO,
                                                      dynamic_param_ptr);
      break;

    default:
      break;
  }
  return result;
}/* ds_3gpp_rmsm_ati_read_dynamic_params */

#endif /* FEATURE_DATA_3GPP*/

