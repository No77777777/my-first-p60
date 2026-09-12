 /*!
  @file
  ds_3gpp_pre_attach_hdlr.c

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009-2016 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps/src/ds_3gpp_pre_attach_hdlr.c#2 $

when         who     what, where, why
--------     ---     --------------------------------------------------------
07/04/14     vb      Handle pre attach activities

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"

#include "ds_3gppi_utils.h"
#include "ds_3gpp_pre_attach_hdlr.h"
#include "ds_dsd_apm_ext_i.h"
#include "ps_sys.h"
#include "ds3gcmif.h"
#include "sys.h"
#include "ds_3gpp_nv_manager.h"
#include "ds_3gpp_rmsm_ip_common.h"

/*------------------------------------------------------------------------- 
  Enum to indicate PRE ATTACH MODE PREF
  -------------------------------------------------------------------------*/

typedef enum
{
  DS_MODE_PREF_NO_PREF = 0,
  DS_MODE_PREF_GSM_ONLY = 1,
  DS_MODE_PREF_WCDMA_ONLY = 2,
  DS_MODE_PREF_LTE_ONLY = 3,
  DS_MODE_PREF_TDS_ONLY = 4,
}ds_3gpp_pre_attach_mode_pref_e;

/*------------------------------------------------------------------------- 
  Enum to indicate PRE ATTACH SRV DOMAIN PREF
  -------------------------------------------------------------------------*/

typedef enum
{
  DS_SRV_DOM_NO_PREF = 0,
  DS_SRV_DOMAIN_PREF_CS_ONLY = 1,
  DS_SRV_DOMAIN_PREF_PS_ONLY = 2,
  DS_SRV_DOMAIN_PREF_CS_PS_ONLY = 3
}ds_3gpp_pre_attach_srv_dom_pref_e;

/*------------------------------------------------------------------------- 
  This timer when fired dispatches the registered callback function`
  ------------------------------------------------------------------------*/
#define DS_3GPP_PRE_ATTACH_CB_TIMER_INTERVAL (5*1000)

/*------------------------------------------------------------------------- 
  PRE ATTCH DATA container
  -------------------------------------------------------------------------*/

static ds_3gpp_pre_attach_cb_data_type ds_3gpp_pre_attach_data;

/*---------------------------------------------------------------------------
  Struct for DS_CMD_3GPP_PRE_ATTACH_SYS_INFO command
---------------------------------------------------------------------------*/
typedef struct
{
  ps_sys_system_status_ex_type  *sys_info_ptr;
  sys_modem_as_id_e_type         subs_id;
}ds_3gpp_pre_attach_sys_info_cmd_type;



/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_HDLR_CM_PH_INFO_CB

DESCRIPTION
   Call back from CM PH
   This function should save the system preference and
   serving domain preference received from ds3gcmif
   This function also deregisters immediately, This is because
   it is no longer interested in listening to CM PH events
 
PARAMETERS
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pre_attach_hdlr_cm_ph_info_cb
(
  ds3g_cm_ph_ev_info_type  *ph_info_ptr
)
{
  uint8 cb_err = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    /*--------------------------------------------------------------------- 
      If CM PH info is received as NULL, It means CB ERR
      Even in this case deregister, Anyways the mode preference and
      srv domain preference will be reset to 0.
      ---------------------------------------------------------------------*/
    if(ph_info_ptr == NULL)
    {
      cb_err = 1;
    }
    else
    {
      ds_3gpp_pre_attach_data.mode_pref = ph_info_ptr->mode_pref;
      ds_3gpp_pre_attach_data.srv_domain_pref = ph_info_ptr->srv_domain_pref;
      DS_3GPP_MSG2_HIGH_EX(ph_info_ptr->as_id,
                        "Pre attach CM PH CB mode_pref:%d srv_domain:%d",
                        ph_info_ptr->mode_pref,
                        ph_info_ptr->srv_domain_pref);
  
      ds3g_dereg_cm_ph_info(ds_3gpp_pre_attach_hdlr_cm_ph_info_cb,
                            ph_info_ptr->as_id);
    }
  }while(0);


  DS_3GPP_MSG1_HIGH(
          "ds_3gpp_pre_attach_hdlr_get_cm_ph_info returns cb_err:%d",cb_err);
  return;
}/*ds_3gpp_pre_attach_hdlr_cm_ph_info_cb*/

/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_HDLR_SUCCESS_CB

DESCRIPTION
  This function is called when
  1. Attach is complete on LTE
  2. Full service is received on W/G/TD
 
  If pre attach hdlr is waiting on this event, callback function
  will be dispatched 
 
PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_pre_attach_hdlr_success_cb
(
  ps_sys_tech_enum_type           tech_type,
  ps_sys_event_enum_type          event_name,
  ps_sys_subscription_enum_type   subscription_id,
  void                           *event_info_ptr,
  void                           *user_data_ptr
)
{

  ds_cmd_type                           *cmd_ptr = NULL;
  ps_sys_system_status_ex_type          *sys_info_ptr = NULL;
  ds_3gpp_pre_attach_sys_info_cmd_type  *data_ptr = NULL;
  sys_modem_as_id_e_type                 subs_id = SYS_MODEM_AS_ID_NONE;
  uint8                                  err_val = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  do
  {
    /*--------------------------------------------------------------------------
      Sanity check done for input params
    ----------------------------------------------------------------------------*/
    if(event_info_ptr == NULL)
    {
      err_val = 1;
      break;
    }

    if((PS_SYS_TECH_ALL != tech_type) ||
       (PS_SYS_EVENT_SYSTEM_STATUS_EX != event_name))
    {
      err_val = 2;
      break;
    }

    subs_id = ds3gsubsmgr_subs_id_ds_to_cm(
                (ds_sys_subscription_enum_type)subscription_id);

    sys_info_ptr
      = (ps_sys_system_status_ex_type*)modem_mem_alloc(
                                         sizeof(ps_sys_system_status_ex_type),
                                         MODEM_MEM_CLIENT_DATA);

    if(sys_info_ptr == NULL)
    {
      err_val = 3;
      break;
    }

    /*---------------------------------------------------------------------------- 
     If the subs id does not match, no point posting the cmd
     return
      ----------------------------------------------------------------------------*/
    if(ds_3gpp_pre_attach_data.subs_id != subs_id)
    {
      err_val = 4;
      break;
    }

    memscpy(sys_info_ptr,
            sizeof(ps_sys_system_status_ex_type),
            event_info_ptr,
            sizeof(ps_sys_system_status_ex_type));

    cmd_ptr = ds_allocate_cmd_buf(sizeof(ds_3gpp_pre_attach_sys_info_cmd_type));

    if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
    {
      err_val++;
      break;
    }

    err_val = 0;
    cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_PRE_ATTACH_SYS_INFO;
    data_ptr = (ds_3gpp_pre_attach_sys_info_cmd_type*)cmd_ptr->cmd_payload_ptr;
    data_ptr->sys_info_ptr = sys_info_ptr;
    data_ptr->subs_id = subs_id;
    ds_put_cmd_ext(cmd_ptr);
  }while(0);
  
  DS_3GPP_MSG1_HIGH_EX(subs_id,
    "ds_3gpp_pre_attach_success cb:err_val:%d",err_val);
}/*ds_3gpp_pre_attach_hdlr_success_cb*/


/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_SET_CM_SYS_PREF

DESCRIPTION
  This function sets CM pref srv domain preference and mode preference
  only if corresponding NV item is set
 
 
PARAMETERS
  subscription id
  reset_sys_pref - If true reset the sys preferences
 
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
static void ds_3gpp_pre_attach_set_cm_sys_pref
(
  boolean                reset_sys_pref,
  sys_modem_as_id_e_type subs_id
)
{
  uint8                              mode_pref       = 0;
  uint8                              srv_dom_pref    = 0;
  cm_sys_sel_pref_params_s_type      sys_sel_pref_params;
  boolean                            set_sys_sel_to_cm = FALSE;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if(!ds_3gpp_pre_attach_hdlr_active(subs_id))
  {
    return;
  }

  memset(&sys_sel_pref_params,0,sizeof(cm_sys_sel_pref_params_s_type));
  /*------------------------------------------------------------------------ 
    Check to see if pref mode and pref srv_domain NV's are set.
    If set, send the same to NV                               .               
    Currently there is no retry mechanism to set the mode and sys_pref
    CM will default to NV's 850 and NV 10 if this api fails 
  ------------------------------------------------------------------------*/
  mode_pref = ds_3gpp_nv_manager_get_pre_attach_mode_pref();

  srv_dom_pref = ds_3gpp_nv_manager_get_pre_attach_srv_domain_pref();


  if((ds_3gpp_pre_attach_mode_pref_e)mode_pref != DS_MODE_PREF_NO_PREF)
  {
    set_sys_sel_to_cm = TRUE;
    switch(mode_pref)
    {
        case DS_MODE_PREF_LTE_ONLY:
          sys_sel_pref_params.mode_pref = CM_MODE_PREF_LTE_ONLY;
          break;

        case DS_MODE_PREF_TDS_ONLY:
          sys_sel_pref_params.mode_pref = CM_MODE_PREF_TDS_ONLY;
          break;

        case DS_MODE_PREF_GSM_ONLY:
          sys_sel_pref_params.mode_pref = CM_MODE_PREF_GSM_ONLY;
          break;

        case DS_MODE_PREF_WCDMA_ONLY:
          sys_sel_pref_params.mode_pref = CM_MODE_PREF_WCDMA_ONLY;
          break;

        default:
          break;
    }
  }

  if((ds_3gpp_pre_attach_srv_dom_pref_e)srv_dom_pref != DS_SRV_DOM_NO_PREF)
  {
    set_sys_sel_to_cm = TRUE;
    switch(srv_dom_pref)
    {
        case DS_SRV_DOMAIN_PREF_CS_PS_ONLY:
          sys_sel_pref_params.srv_domain_pref = CM_SRV_DOMAIN_PREF_CS_PS;
          break;

        case DS_SRV_DOMAIN_PREF_PS_ONLY:
          sys_sel_pref_params.srv_domain_pref = CM_SRV_DOMAIN_PREF_PS_ONLY;
          break;

        case DS_SRV_DOMAIN_PREF_CS_ONLY:
          sys_sel_pref_params.srv_domain_pref = CM_SRV_DOMAIN_PREF_CS_ONLY;
          break;

        default:
          break;
    }
  }
  
  if(set_sys_sel_to_cm)
  {
    sys_sel_pref_params.term_pref = CM_PREF_TERM_PWR_CYCLE;
    sys_sel_pref_params.band_pref = CM_BAND_PREF_NO_CHANGE;
    sys_sel_pref_params.lte_band_pref = SYS_LTE_BAND_MASK_CONST_NO_CHG;
    sys_sel_pref_params.tds_band_pref = CM_BAND_PREF_NO_CHANGE;
    sys_sel_pref_params.prl_pref = CM_PRL_PREF_NO_CHANGE;
    sys_sel_pref_params.roam_pref = CM_ROAM_PREF_NO_CHANGE;
    sys_sel_pref_params.hybr_pref = CM_HYBR_PREF_NO_CHANGE;
    sys_sel_pref_params.network_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_NO_CHANGE;
    sys_sel_pref_params.ue_usage_setting = SYS_UE_USAGE_SETTING_NO_CHANGE;
    sys_sel_pref_params.voice_domain_pref = SYS_VOICE_DOMAIN_PREF_NO_CHANGE;
    sys_sel_pref_params.lte_disable_cause = CM_LTE_DISABLE_CAUSE_NO_CHANGE;
    sys_sel_pref_params.csg_id = SYS_CSG_ID_INVALID;
    sys_sel_pref_params.csg_rat = SYS_RAT_NONE;
    
    /*------------------------------------------------------------------------------ 
      If reset sys pref is only set CM PH info
      ------------------------------------------------------------------------------*/

    if(!reset_sys_pref)
    {
      ds3g_get_cm_ph_info(ds_3gpp_pre_attach_hdlr_cm_ph_info_cb,
                          subs_id);
    }
    else
    {
      sys_sel_pref_params.mode_pref = ds_3gpp_pre_attach_data.mode_pref;
      sys_sel_pref_params.srv_domain_pref = ds_3gpp_pre_attach_data.srv_domain_pref;
      DS_3GPP_MSG2_HIGH_EX(subs_id, "CM PH SET SYS SEL PREF REQ,srv_domain_pref:%d,mode_pref:%d",
                         sys_sel_pref_params.srv_domain_pref,
                         sys_sel_pref_params.mode_pref);
    }
    ds3g_set_sys_sel_pref(&sys_sel_pref_params,FALSE,subs_id);
  }

  return;

}/*ds_3gpp_pre_attach_set_cm_sys_pref*/



/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_DATA_RESET
 
DESCRIPTION
  This function is used to reset pre attach data
  
PARAMETERS   : 
  subs_id

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None

===========================================================================*/
static void ds_3gpp_pre_attach_data_reset
(
   sys_modem_as_id_e_type subs_id
)
{
  int16                                ps_errno = -1;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ds_3gpp_pre_attach_data.cb_pending = FALSE;
  ds_3gpp_pre_attach_data.cb_fn_ptr = NULL;
  (void)rex_clr_timer(&(ds_3gpp_pre_attach_data.pre_attach_cb_timer));
  ds_3gpp_pre_attach_set_cm_sys_pref(TRUE,subs_id);
  ds_3gpp_pre_attach_data.mode_pref = CM_MODE_PREF_NONE;
  ds_3gpp_pre_attach_data.srv_domain_pref = CM_SRV_DOMAIN_PREF_NONE;
  (void)ps_sys_event_dereg_ex(PS_SYS_TECH_ALL,
                              PS_SYS_EVENT_SYSTEM_STATUS_EX,
                             (ps_sys_subscription_enum_type)
                              ds3gsubsmgr_subs_id_cm_to_ds(subs_id),
                              ds_3gpp_pre_attach_hdlr_success_cb,
                             &ps_errno);
  modem_mem_free(ds_3gpp_pre_attach_data.user_data,
                 MODEM_MEM_CLIENT_DATA);

  ds_3gpp_pre_attach_data.user_data = NULL;
}/*ds_3gpp_pre_attach_data_reset*/


/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_DISPATCH_CB
 
DESCRIPTION
  This function dispatches the cb to the client
  
PARAMETERS   : 
  None

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None

===========================================================================*/
static void ds_3gpp_pre_attach_dispatch_cb
(
  boolean cb_val
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 /*-------------------------------------------------------------------- 
    Clear the timer and dispatch the cb fn only if cb is pending
  --------------------------------------------------------------------*/
  if((ds_3gpp_pre_attach_data.cb_pending) && 
     (ds_3gpp_pre_attach_data.cb_fn_ptr != NULL))
  {
    (void)rex_clr_timer(&(ds_3gpp_pre_attach_data.pre_attach_cb_timer));
    ds_3gpp_pre_attach_data.cb_fn_ptr(ds_3gpp_pre_attach_data.cb_data,cb_val);
    ds_3gpp_pre_attach_data.cb_pending = FALSE;
  }

  return;

}/*ds_3gpp_pre_attach_dispatch_cb*/


/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_TIMER_CB

DESCRIPTION
  This function is called when the pre attach timer expires. This call
  back function simply posts a command to DS. DS then processes the command
  and takes suitable action

PARAMETERS
  unsigned long - Call back data

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void ds_3gpp_pre_attach_timer_cb
(
   unsigned long callback_data
)
{
  ds_cmd_type *cmd_ptr = NULL;
  void        **data_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_3GPP_MSG0_MED("pre attach timer cb");

  /*
   * This function is executed in the timer call back.
   * So all we do here is to put a command to DS and exit
   */
  cmd_ptr = ds_allocate_cmd_buf(sizeof(void*));
  if( (cmd_ptr == NULL) || (cmd_ptr->cmd_payload_ptr == NULL) )
  {
    DS3GPP_ASSERT(0);
    return;
  }

  cmd_ptr->hdr.cmd_id = DS_CMD_3GPP_PRE_ATTACH_TIMER_EXP;
  data_ptr = (void**)cmd_ptr->cmd_payload_ptr;
  *data_ptr = (void*)callback_data;

  ds_put_cmd_ext(cmd_ptr);

} /* ds_3gpp_pre_attach_timer_cb */


/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_SET_OPRT_MODE

DESCRIPTION
   This sets the operating mode to CM, It could be either setting the UE to
   LPM mode or mode online
 
PARAMETERS
   OPRT MODE to be set
 
DEPENDENCIES
  None.

RETURN VALUE
   void
 
SIDE EFFECTS
  None

===========================================================================*/
static boolean ds_3gpp_pre_attach_set_oprt_mode
(
  sys_oprt_mode_e_type      oprt_mode,
  sys_modem_as_id_e_type    subs_id  
)
{

  cm_ph_cmd_cb_f_type *cm_ph_cb_f_ptr = NULL;
  boolean              ret_val        = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
     cm_ph_cb_f_ptr = (cm_ph_cmd_cb_f_type *)ds3g_get_cm_ph_cb_fn_ptr();

     if(!cm_ph_cmd_oprt_mode(cm_ph_cb_f_ptr,
                             NULL,
                             ds3g_get_cm_client_id(),
                             oprt_mode))
     {
       DS_3GPP_ERROR_LOG_1_EX(subs_id, "Failure to set operating mode:d",oprt_mode);
       break;
     }

     ret_val = TRUE;
  }while(0);


  return ret_val;
}/*ds_3gpp_pre_attach_set_oprt_mode*/



/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_HDLR_ACTIVE

DESCRIPTION
  If disable attach on power up and enable lpm on powerup is enabled
  return TRUE
 
PARAMETERS
  subscription id
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_pre_attach_hdlr_active
(
  sys_modem_as_id_e_type subs_id
)
{
  boolean ret_val = FALSE;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if((ds_3gpp_nv_manager_get_disable_attach_on_pwr_up()) &&
     (ds_3gpp_nv_manager_get_enable_lpm_on_pwr_up()))
  {
    ret_val = TRUE;
  }

  return ret_val;

}/*ds_3gpp_pre_attach_hdlr_active*/


/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_HDLR_INIT_STATE

DESCRIPTION
  If disable attach on power up and enable lpm on powerup is enabled
  call cm api to power up the device in low power mode
 
PARAMETERS
  subscription id
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
static uint8 ds_3gpp_pre_attach_hdlr_init_state
(
  sys_modem_as_id_e_type subs_id
)
{
  sys_oprt_mode_e_type oprt_mode       = SYS_OPRT_MODE_NONE;
  uint8                error_debug_msg = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  do
  {

    /*----------------------------------------------------------------- 
      1. Both NV's must be enabled
      2. Old operation mode must not be LPM and CM RPM reset
         must be allowed
    -------------------------------------------------------------------*/
    if(ds_3gpp_pre_attach_hdlr_active(subs_id) == FALSE)
    {
      error_debug_msg = 1;
      break;
    }

    oprt_mode = ds3g_read_cm_current_oprt_mode(subs_id);

    if((oprt_mode == SYS_OPRT_MODE_LPM) || 
       (!cm_rpm_check_reset_allowed()))
    {
      error_debug_msg = 2;
      break;
    }
    
    if(!ds_3gpp_pre_attach_set_oprt_mode(SYS_OPRT_MODE_LPM, subs_id))
    {
      error_debug_msg = 3;
      break;
    }

    error_debug_msg = 0;

  }while(0);

  DS_3GPP_MSG1_HIGH_EX(subs_id, "ds_3gpp_pre_attach_hdlr_init_state returns %d",error_debug_msg);
  return error_debug_msg;
}/*ds_3gpp_pre_attach_hdlr_init_state*/



/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_HDLR_INIT

DESCRIPTION
  If disable attach on power up and enable lpm on powerup is enabled
  call cm api to power up the device in low power mode
 
PARAMETERS
  subscription id
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pre_attach_hdlr_init
(
  sys_modem_as_id_e_type subs_id
)
{
  uint8                              error_debug_msg = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  memset(&ds_3gpp_pre_attach_data,0,sizeof(ds_3gpp_pre_attach_cb_data_type));

  error_debug_msg = ds_3gpp_pre_attach_hdlr_init_state(subs_id);

  /*------------------------------------------------------------------------
     Define the pre attach cb timer. Specify the call back function and the
     call back data.
  ------------------------------------------------------------------------*/
  rex_def_timer_ex(&(ds_3gpp_pre_attach_data.pre_attach_cb_timer),
                    (rex_timer_cb_type) ds_3gpp_pre_attach_timer_cb,
                    NULL);


  return;
}/*ds_3gpp_pre_attach_hdlr_init*/

/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_OPRT_MODE_CB_NO_ERR
 
DESCRIPTION
  This function handles the callback received when DS puts CM into either
  LPM mode or on mode online
  
PARAMETERS   : 
  None

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pre_attach_oprt_mode_cb_no_err
(
  void
)
{
  DS_3GPP_ERROR_LOG_0("OPRT MODE CB NO ERR received");
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
}/*ds_3gpp_pre_attach_oprt_mode_cb_no_err*/


/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_OPRT_MODE_CB_ERR
 
DESCRIPTION
  This function handles the callback received on error
  when DS puts CM into either LPM mode or on mode online
  
PARAMETERS   : 
  None

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pre_attach_oprt_mode_cb_err
(
  void
)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_3GPP_ERROR_LOG_0("OPRT MODE CB ERR received");

    /*------------------------------------------------------------------------- 
   If function ptr is populated then the error was received because attempting
   to put the UE to MODE ONLINE resulted in this ERROR
    -------------------------------------------------------------------------*/
  ds_3gpp_pre_attach_dispatch_cb(FALSE);
}/*ds_3gpp_pre_attach_oprt_mode_cb_err*/

/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_HDLR_CALL_ORIG_REQ

DESCRIPTION 
This function when called checks to see if the NV's are set and if set 
checks to see if UE is in LPM. 
If the UE is in LPM, then request CM to put the device online 
 
PARAMETERS 
   Profile num
   Callback function ptr
   cb data
   password
  *cb_ev_active
  info_data
  subs_id
    
DEPENDENCIES
  None.

RETURN VALUE
  boolean
 
SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_pre_attach_hdlr_call_orig_req
(
   uint16                       profile_num,
   ds_3gpp_pre_attach_cb_fn_ptr pre_attach_cb_fn_ptr,
   void                        *cb_data,
   char                         passwd[],
   boolean                     *cb_ev_active,
   void                        *info_ptr,
   sys_modem_as_id_e_type       subs_id
)
{
  sys_oprt_mode_e_type                 oprt_mode       = SYS_OPRT_MODE_NONE;
  uint8                                error_debug_msg = 0;
  boolean                              ret_val         = TRUE;
  ps_sys_3gpp_lte_attach_pdn_list_type attach_list     = {0};
  int16                                ps_errno = -1;
  int16                                ps_sys_reg_ret_val = -1;
  ds_3gpp_rmsm_ip_common_info_type    *info_p          = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  do
  {

    /*---------------------------------------------------------------------- 
      If the same client tries to re register and the cb is pending
      simply ignore the request
      ----------------------------------------------------------------------*/
    if(((ds_3gpp_pre_attach_data.cb_pending) && 
       (ds_3gpp_pre_attach_data.cb_fn_ptr == pre_attach_cb_fn_ptr)) &&
       (ds_3gpp_pre_attach_data.subs_id == subs_id) &&
       (cb_ev_active != NULL))
    {
      ret_val = FALSE;
      *cb_ev_active = TRUE;
      break;
    }
    /*----------------------------------------------------------------- 
      1. Both NV's must be enabled
      2. Old operation mode must not be MODE ONLINE 
    -------------------------------------------------------------------*/
    if(ds_3gpp_pre_attach_hdlr_active(subs_id) == FALSE)
    {
      error_debug_msg = 1;
      break;
    }
    
    if(cb_ev_active == NULL)
    {
      error_debug_msg = 2;
      break;
    }

    if(passwd[0] == '\0')
    {
      ret_val = FALSE;
     *cb_ev_active = FALSE;
      error_debug_msg = 3;
      break;
    }        

    oprt_mode = ds3g_read_cm_current_oprt_mode(subs_id);

    if(oprt_mode == SYS_OPRT_MODE_ONLINE)
    {
      error_debug_msg = 4;
      break;
    }
    
    /*------------------------------------------------------------------ 
      Make sure the fn ptr is not NULL
      If pre attach fn ptr is already populated exit because there can
      only be one client performing pre attach operation
    -------------------------------------------------------------------*/
    if(pre_attach_cb_fn_ptr == NULL)
    {
      error_debug_msg = 5;
      break;
    }

    if(ds_3gpp_pre_attach_data.cb_fn_ptr != NULL)
    {
      error_debug_msg = 6;
      break;
    }

    ds_3gpp_pre_attach_data.cb_fn_ptr = pre_attach_cb_fn_ptr;
    ds_3gpp_pre_attach_data.cb_data   = cb_data;
    ds_3gpp_pre_attach_data.subs_id   = subs_id;

    attach_list.attach_pdn_profile_list[0]  = profile_num;
    attach_list.attach_pdn_profile_list_num = 1;


    /*---------------------------------------------------------------- 
      Set the attach profile list. This will be used for attach
      -----------------------------------------------------------------*/
    if(ds_dsd_apm_set_attach_pdn_list(&attach_list,
                                      subs_id,
                                      &ps_errno,
                                      NULL) != 0)
    {
      error_debug_msg = 7;
      break;
    }

     /*--------------------------------------------------------------------- 
     Register for PS_SYS_EVENTS. This will be sent when full service on
     any 3gpp rat is received.
     ---------------------------------------------------------------------*/
    ps_sys_reg_ret_val = ps_sys_event_reg_ex(PS_SYS_TECH_ALL,
                                             PS_SYS_EVENT_SYSTEM_STATUS_EX,
                                             (ps_sys_subscription_enum_type)
                                             ds3gsubsmgr_subs_id_cm_to_ds(subs_id),
                                             ds_3gpp_pre_attach_hdlr_success_cb,
                                             NULL,
                                             &ps_errno);
    if (ps_sys_reg_ret_val < 0)
    {
      error_debug_msg = 8;
    }

    ds_3gpp_pre_attach_set_cm_sys_pref(FALSE,subs_id);

    if(!ds_3gpp_pre_attach_set_oprt_mode(SYS_OPRT_MODE_ONLINE, subs_id))
    {
      error_debug_msg = 9;
      break;
    }

    ret_val = FALSE;
    error_debug_msg = 0;
    *cb_ev_active = TRUE;
    ds_3gpp_pre_attach_data.cb_pending = TRUE;

    /*------------------------------------------------------------------------
     Start the timer interval
     If Attach is not completed, then this timer will fire and the
     call back will be sent to the client
    ------------------------------------------------------------------------*/
    (void)rex_set_timer(&(ds_3gpp_pre_attach_data.pre_attach_cb_timer),
                        DS_3GPP_PRE_ATTACH_CB_TIMER_INTERVAL);

    if(info_ptr != NULL)
    {
      info_p = (ds_3gpp_rmsm_ip_common_info_type *)info_ptr;
      ds_3gpp_pre_attach_data.user_data = modem_mem_alloc(sizeof(network_params_info_type),
                                                          MODEM_MEM_CLIENT_DATA);

      if(ds_3gpp_pre_attach_data.user_data != NULL)
      {
        memscpy(ds_3gpp_pre_attach_data.user_data,sizeof(network_params_info_type),
               (void *) &(info_p->ppp_info),sizeof(network_params_info_type));
      }
    }


  }while(0);

  DS_3GPP_MSG1_HIGH_EX(subs_id, "ds_3gpp_pre_attach_hdlr_call_orig_req returns %d",
                    error_debug_msg);
  return ret_val;
}/*ds_3gpp_pre_attach_hdlr_call_orig_req*/


/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_PROCESS_TMR_EXP_CMD

DESCRIPTION
The function is called when the pre attach timer cb expires, 
Send a cb to the client indicating that the call cannot be brought up 
 
PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pre_attach_process_timer_exp_cmd
(
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_3GPP_MSG0_HIGH("ds_3gpp_pre_attach_process_timer_exp_cmd");

  ds_3gpp_pre_attach_dispatch_cb(FALSE);

} /*ds_3gpp_pre_attach_process_timer_exp_cmd*/

/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_HDLR_CLOSE

DESCRIPTION
   When this api is called pre attach hdlr puts the UE to mode LPM
 
PARAMETERS
  subscription id
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_pre_attach_hdlr_close
(
  sys_modem_as_id_e_type subs_id
)
{
  uint8                error_debug_msg = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  error_debug_msg = ds_3gpp_pre_attach_hdlr_init_state(subs_id);
  ds_3gpp_pre_attach_data_reset(subs_id);
  DS_3GPP_MSG1_HIGH_EX(subs_id, "ds_3gpp_pre_attach_hdlr_init_close returns %d",error_debug_msg);
  return;
}/*ds_3gpp_pre_attach_hdlr_close*/

/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_HDLR_GET_CB_DATA

DESCRIPTION
   When this api is called the client's cb data is returned to the caller
   For EX : For DUN calls sm_ptr is stored is returned
 
   The callback fn ptr should match only then the call back data is given
 
PARAMETERS
  subscription id
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void *ds_3gpp_pre_attach_hdlr_get_cb_data
(
  ds_3gpp_pre_attach_cb_fn_ptr  cb_fn_ptr
)
{
  void     *ret_val = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(cb_fn_ptr == ds_3gpp_pre_attach_data.cb_fn_ptr)
  {
    DS_3GPP_MSG0_HIGH("CB data ptr match occured");
    ret_val = ds_3gpp_pre_attach_data.cb_data;
  }

  return ret_val;
}/*ds_3gpp_pre_attach_hdlr_get_cb_data*/


/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_PROCESS_SYS_INFO

DESCRIPTION
  This function process the system info and dispatches the callback to
  RMSM
 
PARAMETERS
  sys_info_ptr:                    System information reported by MPPM

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pre_attach_process_sys_info
(
  void  *cmd_payload_ptr
)
{
  ps_sys_system_status_ex_type         *sys_info_ptr = NULL;
  ps_sys_network_enum_type              tech = PS_SYS_NETWORK_MAX;
  ps_sys_rat_ex_enum_type               rat_val = PS_SYS_RAT_MAX;
  uint64                                so_mask = PS_SYS_SO_EX_UNSPECIFIED;
  ds_3gpp_pre_attach_sys_info_cmd_type  *data_ptr = NULL;
  ds3gsubsmgr_subs_id_e_type            subs_indx;
  uint8                                 err_val = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  data_ptr = (ds_3gpp_pre_attach_sys_info_cmd_type*)cmd_payload_ptr;

  subs_indx = ds3gsubsmgr_subs_id_cm_to_ds3g(data_ptr->subs_id);

  do
  {
    if(!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_indx))
    {
      err_val = 1;
      break;
    }

    if (data_ptr->sys_info_ptr == NULL)
    {
      err_val = 2;
      break;
    }

    sys_info_ptr = data_ptr->sys_info_ptr;

    tech    = sys_info_ptr->avail_sys[0].technology;
    rat_val = sys_info_ptr->avail_sys[0].rat_value;
    so_mask = sys_info_ptr->avail_sys[0].so_mask;

	
    if((tech != PS_SYS_NETWORK_3GPP) || 
       ((rat_val != PS_SYS_RAT_EX_3GPP_WCDMA) &&
        (rat_val != PS_SYS_RAT_EX_3GPP_GERAN)&&
        (rat_val != PS_SYS_RAT_EX_3GPP_LTE ) &&
        (rat_val != PS_SYS_RAT_EX_3GPP_TDSCDMA)))
    {
      err_val = 3;
      DS_LTE_MSG4_MED_EX(subs_indx,
                      "ds_3gpp_pre_attach_process_sys_info: Technology: %d, "
                      "Rat_Mask:%d, So_Mask:0x%x%x",
                       tech, 
                       rat_val, 
                       QWORD_HIGH(so_mask),
                       QWORD_LOW(so_mask));
      break;
    }

    err_val = 0;
    ds_3gpp_pre_attach_dispatch_cb(TRUE);
  }while (0);

  if (data_ptr->sys_info_ptr != NULL)
  {
    modem_mem_free(data_ptr->sys_info_ptr, MODEM_MEM_CLIENT_DATA);
  }

  DS_LTE_MSG1_MED_EX(subs_indx,
                     "ds_3gpp_pre_attach_process_sys_info  returns %d",
                     err_val);

  return;
}/*ds_3gpp_pre_attach_process_sys_info*/


/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_HDLR_GET_USER_DATA

DESCRIPTION
   Get user data from attach hdlr
 
PARAMETERS
    
DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void *ds_3gpp_pre_attach_hdlr_get_user_data
(
  sys_modem_as_id_e_type         subs_id
)
{
  void    *ret_val = NULL;
  uint8    dbg_val = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    if(!ds_3gpp_pre_attach_hdlr_active(subs_id))
    {
      dbg_val = 1;
      break;
    }

    ret_val = ds_3gpp_pre_attach_data.user_data;

  }while(0);
   
  DS_3GPP_MSG2_HIGH_EX(subs_id, "ds_3gpp_pre_attach_hdlr_get_user_data is 0x%x, dbg_val:%d",
                    ret_val,dbg_val);

  return ret_val;
}
