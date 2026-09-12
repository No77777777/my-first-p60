/*!
  @file
  ds_3gpp_loopback_hdlr.h

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009-2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_loopback_hdlr.h#4 $ $DateTime: 2021/02/08 22:33:06 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/30/14   sb     Loopback modehandler using 3gppmodehandler wm

===========================================================================*/

#include "cm.h"
#include "cm_gw.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_bearer_context.h"
#include "rrcdata.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif
#include "ipa.h"
#include "ds_3gpp_diag_hdlr.h"
#include "timer.h"

/*--------------------------------------------------------------------------- 
  Default bearer direction for Uplink and downlink is LTE
---------------------------------------------------------------------------*/
#define DS_3GPP_LOOPBACK_DEFAULT_BEARER_DIR       1 

typedef enum lb_client_event_reg_e {
  LB_CLIENT_EVENT_REG,
    /**< Event registration. */
  LB_CLIENT_EVENT_DEREG,
    /**< Event deregistration. */
  LB_CLIENT_SS_EVENT_REG,
  LB_CLIENT_EVENT_MAX   /* FOR INTERNAL CM USE ONLY! */
/** @endcond
*/

} lb_client_event_reg_e_type;

typedef enum
{
  LOOPBACK_TOKEN_DEFAULT = 0,
  LOOPBACK_TOKEN_LOOPBACK_ENABLED,
  LOOPBACK_TOKEN_L2_LOOPBACK_ENABLED,
  LOOPBACK_TOKEN_CLOCK_VOTE_MASK,
  LOOPBACK_TOKEN_SYS_MODE,
  LOOPBACK_TOKEN_DL_MULF,
  LOOPBACK_TOKEN_DL_DUPF,
  LOOPBACK_TOKEN_UL_ONLY,
  LOOPBACK_TOKEN_NR5G_ONLY,
  LOOPBACK_TOKEN_REPLICATION_FACTOR,
  LOOPBACK_TOKEN_LTE_LB_ONLY
}ds_3gpp_loopback_hdlr_token;

/*---------------------------------------------------------------------------
TYPEDEF DS_3GPP_LOOPBACK_HDLR_INITIAL_BEARER_SETUP

DESCRIPTION
  Enum for initial bearer type
---------------------------------------------------------------------------*/
typedef enum
{
  LOOPBACK_MCG_ONLY = 0,
  LOOPBACK_SCG_ONLY = 1,
  LOOPBACK_UL_MCG_SPLIT_DL = 2,
  LOOPBACK_UL_SCG_SPLIT_DL = 3
}ds_3gpp_loopback_hdlr_initial_bearer_setup;

typedef enum
{
  DS_3GPP_LOOPBACK_DEFAULT_DEQUEUE_MODE = 0,
  DS_3GPP_LOOPBACK_TIMER_DEQUEUE_MODE,
  DS_3GPP_LOOPBACK_THRESHOLD_DEQUEUE_MODE
}ds_3gpp_loopback_dequeue_mode_e_type;

/*-------------------------------------------------------------------------
  Dequeue config structure that will be populated through diag cmd
  Defines the way packets can be dequeued from UL WM.
-------------------------------------------------------------------------*/
typedef struct 
{
  /* Mode to select dequeue mode.
     Timer based or threshold based */
  ds_3gpp_loopback_dequeue_mode_e_type    dequeue_mode;
  /*  Timer intervals at which UL WM is dequeued */
  uint16                                  dequeue_timer_val_msec; 
  /*  Indicates whether dequeue timer is running or not */
  boolean                                 dequeue_tmr_running;  
  /* Timer to control dequeueing packets from WM. This
     timer is non-deferrable timer */
  timer_type                              dequeue_timer;
  /* Num bytes to dequeue on timer expiry */
  uint32                                  dequeue_bytes;
  /* Minimun number of packets to be dequeued */
  uint32                                  min_range_bytes;
  /* Max number of packets to be dequeued */
  uint32                                  max_range_bytes;
}ds_3gpp_loopback_dequeue_cfg_s_type;

/*----------------------------------------------------------------------------
  Enum specifies the action that needs to be taken after a packet is dequeued
  and delivered to IPA
----------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_LOOPBACK_POST_DEQUEUE_NO_OP = 0,
  DS_3GPP_LOOPBACK_POST_DEQUEUE_LOSS,
  DS_3GPP_LOOPBACK_POST_DEQUEUE_REORDER,
  DS_3GPP_LOOPBACK_POST_DEQUEUE_REPLICATION
}ds_3gpp_loopback_post_dequeue_ops_e_type;

typedef struct
{
  /* Post dequeue operation mode to be used */
  ds_3gpp_loopback_post_dequeue_ops_e_type     post_dequeue_op_mode;
  /* Percentage of samples to act on */
  uint8                                        sample_set;
}ds_3gpp_loopback_post_dequeue_ops_s_type;

typedef struct
{
  uint8                                       current_eps_id;
  uint8                                       current_call_id;
  uint32                                      clock_vote_mask;
  uint8                                       lb_dl_mulf;
  uint8                                       lb_dl_dupf;
  uint8                                       lb_ul_only;
  boolean                                     ds_3gpp_loopback_mode;
  boolean                                     ds_3gpp_l2_loopback_mode;
  ps_sig_enum_type                            lb_rx_sig;
  sys_sys_mode_e_type                         loopback_sys_mode;
  sys_modem_as_id_e_type                      loopback_subs_id;
  ds_3gpp_loopback_hdlr_initial_bearer_setup  initial_bearer_direction;
  boolean                                     send_rab_rej;
  uint32                                      replication_factor;
  ds_3gpp_loopback_dequeue_cfg_s_type         dequeue_cfg_info;
  boolean                                     lte_loopback_only;
  ds_3gpp_loopback_post_dequeue_ops_s_type    post_dequeue_ops_info;
}ds_3gpp_loopback_context_info_type;

typedef enum lb_client_status_e {
  LB_CLIENT_ERR_NONE=-1,   /* FOR INTERNAL CM USE ONLY! */
  LB_CLIENT_OK,
  LB_CLIENT_ERR_EVENT_REG,
  LB_CLIENT_MAX
}lb_client_status_e_type;



typedef struct
{
  cm_call_event_e_type cm_event_type;
  cm_mm_call_event_f_type  *call_event_func;
}lb_call_info;

typedef struct
{
  cm_ss_event_e_type             cm_ss_event_type;
  cm_mm_msim_ss_event_f_type      *ss_event_func;
}lb_call_ss_info;


typedef struct{
  uint8                is_active;
  uint8                loop_back_call_id;
  uint8                loop_back_eps_id;
  uint8                loop_back_rb_id;
  boolean              v4_call;
  boolean              v6_call;
  boolean              ra_received;
  dsm_watermark_type*  tx_wm_ptr;
  dsm_watermark_type*  rx_wm_ptr;
}loopback_wm_info_type;

typedef struct
{
  uint8 eps_id;
  sys_modem_as_id_e_type subs_id;
}loopback_wm_reg_type;

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_3gpp_loopback_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for MEM Pool global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_3gpp_loopback_qsh_mdump_collect
(
  void
);
#endif
/*-------------------------------------------------------------------------
  Structure used for transfering control PDU information between 4G and
  NR5G modems
-------------------------------------------------------------------------*/
typedef struct
{
  uint8                           eps_id;
  sys_modem_as_id_e_type          subs_id;
  uint32                          num_bytes;
}loopback_cntrl_pdu_info;

lb_client_status_e_type ds_3gpp_loopback_client_call_reg(
    cm_mm_call_event_f_type         *call_event_func,
        /* Pointer to a callback function to notify the client of call
        ** events */
    lb_client_event_reg_e_type  event_reg_type,
    cm_call_event_e_type        call_event
);

lb_client_status_e_type ds_3gpp_loopback_client_ss_call_reg(
    cm_mm_msim_ss_event_f_type      *ss_event_func,
        /* Pointer to a callback function to notify the client of call
        ** events */
    lb_client_event_reg_e_type  event_reg_type,
    cm_ss_event_e_type        call_event
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_PDN_CONNECTIVITY REQUEST

DESCRIPTION
  This function is responsible for handling pdn connectivty sent as part of
  Lte call bring up in case of Loopback configuration. This function is responsible
  for setting up the apn, ip address based on the pdp type and send an
  activate bearer indication This function is also responsible for setting up the
  loopback wm info table with the call id, eps id  and initialize the singal for
  further data processing.
 
PARAMETERS
  cm_call_cmd_cb_f_type  cmd_cb_func,
  void                   *data_block_ptr,
  const cm_pdn_connectivity_s_type   *pdn_connectivity_ptr
    pointer to the request specific data
 
 
DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_loopback_pdn_connectivity_req(

  cm_call_cmd_cb_f_type  cmd_cb_func,
    /**< client callback function */

  void                   *data_block_ptr,
    /**< pointer to client callback data block */


  const cm_pdn_connectivity_s_type   *pdn_connectivity_ptr
    /**< pointer to the request specific data */
);


void ds_3gpp_loopback_hdlr_attach(void);


void ds_3gpp_loopback_send_pdnconnection_ind(void);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_PDN_CONNECTIVITY REQUEST

DESCRIPTION
  This function is responsible for handling activate bearer response as part of
  Lte call bring up in case of Loopback configuration.This function is responsible
  for sending cm call connected event followed by attach complte incase of INITIAL
  EPS Bearer. This functions is also responsible for sending the Rab Reestabilishment
  Indication which will inturn set up the data path.
 
PARAMETERS
  call_id - call id passed during bearer activation
  act_bearer_rsp_ptr - activate bearer response pointer

DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_loopback_act_bearer_rsp
(
   cm_call_id_type                  call_id,
   const cm_act_bearer_rsp_s_type   *act_bearer_rsp_ptr
);

/*===========================================================================
FUNCTION  DS_3GPP_LOOPBACK_LTE_PDCPDL_RAB_REGISTER_REQ

DESCRIPTION
  This function is responsible for handling lte pdcp ul rab registration.
  This function stores the uplink wm pointer in the loopback_wm_table 
 
PARAMETERS
  eps_bearer_id      eps bearer id
  tx_wm_item         tx wm pointer
  uint8              inst_id

DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_loopback_lte_pdcpul_rab_register_req
(
   uint8 eps_bearer_id,
   dsm_watermark_type* tx_wm_item,
   sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION  DS_3GPP_LOOPBACK_LTE_PDCPDL_RAB_REGISTER_REQ

DESCRIPTION
  This function is responsible for handling lte pdcp ul rab deregistration.
 
PARAMETERS
  eps_bearer_id      eps bearer id
  uint8              inst_id

DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_loopback_lte_pdcpul_rab_deregister_req
(
   uint8 eps_bearer_id,
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_LTE_PDCPDL_RAB_REGISTER_REQ

DESCRIPTION
  This function is responsible for handling lte pdcp dl rab registration.
  This function stores the downlink wm pointer in the loopback_wm_table.
  This function is also reponsible for setting up the signal handler and
  register for nonempty callback function.
 
PARAMETERS
  eps_bearer_id      eps bearer id
  rx_wm_item         rx wm pointer
  inst_id

DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_loopback_lte_pdcpdl_rab_register_req
(
   uint8 eps_bearer_id,
   dsm_watermark_type* rx_wm_item,
   sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_UL_RAB_REGISTER_CNF

DESCRIPTION
  This function is responsible for sending lte pdcp ul rab registration
  confirmation
 
PARAMETERS
  eps_id      eps bearer id
  inst_id     subscription id

DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_ul_rab_register_cnf
(
   uint8 eps_id,
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_UL_RAB_REGISTER_CNF

DESCRIPTION
  This function is responsible for sending lte pdcp ul rab registration
  confirmation
 
PARAMETERS
  eps_id      eps bearer id
  inst_id     subscription id

DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_ul_rab_deregister_cnf
(
   uint8 eps_id,
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_DL_RAB_REGISTER_CNF

DESCRIPTION
  This function is responsible for sending lte pdcp dl rab registration
  confirmation
 
PARAMETERS
  eps_id      eps bearer id
  inst_id     subscription id

DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_dl_rab_register_cnf
(
   uint8 eps_id,
   sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_PROCESS_CMDS

DESCRIPTION
  This function is responsible for processing the UL confirmation as well
  as DL confirmation.
 
PARAMETERS
  cmd_ptr

DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_hdlr_process_cmds
(
  const ds_cmd_type      *cmd_ptr
);

void ds_3gpp_loopback_hdlr_tx_data
(
  dsm_watermark_type *wm_p,
  void               *call_back_data
);

boolean ds_3gpp_loopback_hdlr_process_data_pkt
( 
  ps_sig_enum_type sig, 
  void *user_data_p
);  

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_PROCESS_DATA_PKT_WRAPPER

DESCRIPTION
  This is the callback function called as part of lb_rx_sig signal getting
  set. 
 
PARAMETERS
  sig          - singal which was set
  user_data_p  - index 

DEPENDENCIES
  

RETURN VALUE
..TRUE/FALSE -if False;signal will be set again;  
 
 
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_loopback_hdlr_process_data_pkt_wrapper
( 
  ps_sig_enum_type sig, 
  void *user_data_p
);  

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_PDN_DISCONNECT_REQ

DESCRIPTION
  This function is responsible for handling pdn disconnect request in case of
  LTE loopback mode.This function inturn will invoke CM_CALL_EVENT_END 
 
PARAMETERS
  eps_id  -eps bearer id
  pdn_disconnect_ptr - pdn disconnect ptr

DEPENDENCIES
  

RETURN VALUE
..TRUE/FALSE - indicating whether operation is succcesful 
 
SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_loopback_hdlr_pdn_disconnect_req
(
  uint8 eps_id,
  const cm_pdn_disconnect_s_type   *pdn_disconnect_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_SEND_FULL_SERVICE_IND

DESCRIPTION
  This function is responsible for sending full service indication This function
  is used to send  CM_SS_EVENT_SRV_CHANGED for the sys mode read as part of
  loopback efs configuration
 
PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None 
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_hdlr_send_fullservice_ind(void);


/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_SEND_SUSPEND_RESUME_IND

DESCRIPTION
  This function is responsible for sending suspend /resume indication
 
 
PARAMETERS
  ps_data_suspend_flag

DEPENDENCIES
  None

RETURN VALUE
  None 
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_hdlr_send_suspend_resume_ind
(
  boolean   ps_data_suspend_flag
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_RES_ALLOC_REQ

DESCRIPTION
  This function is responsible for handling resource allocation request from the
  3GPP Modehandler in case of Loopback configuration. This inturn will send  activate
  bearer indication and intiate a network initiated bearer.
 
PARAMETERS
  bearer_alloc_params_ptr - bearer alloc paramaters pointer

DEPENDENCIES
  None

RETURN VALUE
  ..TRUE/FALSE - indicating whether operation is succcesful
 
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_loopback_res_alloc_req
(
  cm_res_alloc_s_type *bearer_alloc_params_ptr
);


/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_CALL_CMD_ORIG_CC_PER_SUBS

DESCRIPTION
  This function is responsible for handling call cmd origination for all non
  lte mode of operation in case of Loopback config
 
PARAMETERS
 gw_ps_orig_params_ptr    - gw ps origination paramaters 
 asubs_id,                - subs_id
	*return_call_id_ptr      - return call_id_ptr

DEPENDENCIES
  None

RETURN VALUE
  ..TRUE/FALSE - indicating whether operation is succcesful
 
SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_loopback_hdlr_call_cmd_orig_cc_per_subs
(
  cm_gw_ps_orig_params_s_type *gw_ps_orig_params_ptr,
	sys_modem_as_id_e_type      asubs_id,
  cm_call_id_type             *return_call_id_ptr
);


/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_RRC_RETURN_LC_INFO_FOR_RAB

DESCRIPTION
  This function is responsible for filling the lc information pointer based
  on the rab id passed by the client.This function is invoked as part of WCDMA/TDS
  mode of operation
 
PARAMETERS
  lc_info_ptr  - Logical channel information pointer
 
DEPENDENCIES
  None

RETURN VALUE
  ..RRC_RAB_FOUND/RRC_RAB_NOT_FOUND - indicating whether operation is succcesful
 
SIDE EFFECTS
  None.

===========================================================================*/
rrc_rab_search_e_type ds_3gpp_loopback_hdlr_rrc_return_lc_info_for_rab
(
  rrc_user_plane_lc_info_type *lc_info_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_L2_REG_UL_WM

DESCRIPTION
  This function is responsible for registering the uplink wm with the coresponding
  rb id passed .This function is invoked as part of all non-LTE mode of operation.

 
PARAMETERS
  rlc_ul_id - rlc id/nsapi in case of Wcdma/TDS/gsm
  tx_wm_ptr - tx wm pointer
 
DEPENDENCIES
  None

RETURN VALUE
  
 
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_l2_reg_ul_wm
(
  uint8 rlc_ul_id,
  dsm_watermark_type* tx_wm_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_L2_REG_DL_WM

DESCRIPTION
  This function is responsible for registering the downlink wm with the coresponding
  rb id passed .This function is invoked as part of all non-LTE mode of operation.

 
PARAMETERS
  rlc_ul_id - rlc id/nsapi in case of Wcdma/TDS/gsm
  rx_wm_ptr - tx wm pointer
 
DEPENDENCIES
  None

RETURN VALUE
  
 
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_l2_reg_dl_wm
(
  uint8 rlc_dl_id,
  dsm_watermark_type* rx_wm_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_CALL_CMD_END

DESCRIPTION
  This function is responsible for handling call cmd end in case of Umts mode
  operation when Loopback is configured. 

 
PARAMETERS
  call_end_params - call end paramters pointer
 
DEPENDENCIES
  None

RETURN VALUE
  
 
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_loopback_hdlr_call_cmd_end
(
  cm_end_params_s_type  *call_end_params
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_SYS_MODE

DESCRIPTION
  This function is used to return the current loopback handler sys mode.
  This is read as part of efs configuration information
 
PARAMETERS
  None

DEPENDENCIES
  None.

RETURN VALUE
..loopback_sys_mode -current loopback sys mode
 
SIDE EFFECTS
  None.

===========================================================================*/

sys_sys_mode_e_type ds_3gpp_loopback_hdlr_sys_mode(void);


/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_IS_ENABLED

DESCRIPTION
  This function is used to check whether Loopback is enabled in the current
  mode of operation.
 
PARAMETERS

DEPENDENCIES
  

RETURN VALUE
..boolean - TRUE/FALSE (indicating whether Loopback is enabled)

SIDE EFFECTS
  None.

===========================================================================*/

boolean ds_3gpp_loopback_is_enabled(void);

/*===========================================================================
FUNCTION DS_3GPP_L2_LOOPBACK_IS_ENABLED

DESCRIPTION
  This function is used to check whether L2 Loopback is enabled in the current
  mode of operation.
 
PARAMETERS

DEPENDENCIES
  

RETURN VALUE
..boolean - TRUE/FALSE (indicating whether Loopback is enabled)

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_3gpp_l2_loopback_is_enabled(void);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_READ_EFS

DESCRIPTION
  This function is used to read the efs during dstask initialization.This function
  will be used to read the following paramters:
  LOOPBACK_ENABLED:1
  SYS MODE:9
  DL_MULF:3
  DL_DUPF:0
  UL_ONLY:0
 
PARAMETERS

DEPENDENCIES
  

RETURN VALUE
..boolean - TRUE/FALSE (indicating whether Loopback is enabled)

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_hdlr_read_efs(void);


/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_CHECK_TOKENID

DESCRIPTION
  This function is used to check the paramter read from the efs file against
  the preconfigured loopback parameters and approprioate token id is returned
 
PARAMETERS
  from-
  to-

 
 
DEPENDENCIES
  

RETURN VALUE
..ds_3gpp_loopback_hdlr_token - Token read from the efs

SIDE EFFECTS
  None.

===========================================================================*/

ds_3gpp_loopback_hdlr_token ds_3gpp_loopback_hdlr_check_token_id
(
  char *from,
  char *to
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_SNDCP_PDP_REQ

DESCRIPTION
  This function is responsible for sending sndcp pdcp reg confirmation
  in case of gsm mode of operation. This callback invokation is needed to
  configure the wm and registration

 
PARAMETERS
  nsapi,
  pdp_ul_suspend_fnc_ptr,
  pdp_ul_resume_fnc_ptr,
 ( *pdp_dl_fnc_ptr )( void* ds_context, dsm_item_type **npdu ),
 *ds_context,
 cipher,
( *pdp_reg_cnf_fnc_ptr )( void* ds_context, boolean success )
 
DEPENDENCIES
  None

RETURN VALUE
  
 
SIDE EFFECTS
  None.

===========================================================================*/

void  ds_3gpp_loopback_hdlr_sndcp_pdp_reg
(
  uint8        nsapi,
  wm_cb_type   pdp_ul_suspend_fnc_ptr,
  wm_cb_type   pdp_ul_resume_fnc_ptr,
  void         ( *pdp_dl_fnc_ptr )( void* ds_context, dsm_item_type **npdu ),
  void         *ds_context,
  boolean      cipher,
  void         ( *pdp_reg_cnf_fnc_ptr )( void* ds_context, boolean success )
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_SEND_CM_CALL_BACK_EVENT

DESCRIPTION
  This function is responsible for calling the appropriate cm call event function
  based on the event and call info pointer passed
 
PARAMETERS
..cm_event_type - CM Call Event Type 
  call_info_ptr - CM Call Info Pointer
 
DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_hdlr_send_cm_call_back_event
(
   cm_call_event_e_type cm_event_type,
   cm_mm_call_info_s_type *call_info_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_GET_EPSID

DESCRIPTION
  This function is used to return the eps id to be used for the call.This is
  done by incrementing the current eps id evertime. This is intialized to the
  default eps id.
 
PARAMETERS

 
DEPENDENCIES
  

RETURN VALUE
..eps_id - eps id to be used for the call id 
 
SIDE EFFECTS
  None.

===========================================================================*/

uint8 ds_3gpp_loopback_hdlr_get_epsid(void);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_GET_CALLID

DESCRIPTION
  This function is used to return the callid to be used for the call.This is
  done by incrementing the current eps id evertime. This is intialized to the
  default callid.
 
PARAMETERS

 
DEPENDENCIES
  

RETURN VALUE
..call_id - call id to be used for the call
 
SIDE EFFECTS
  None.

===========================================================================*/



uint8 ds_3gpp_loopback_hdlr_get_callid(void);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_PDP_DATA_CTRL_SETUP_CB

DESCRIPTION
  This function sets up the path to enqueue data between two bearers
 
PARAMETERS
  *phys_link_ptr - ptr to the phys link
  **item_ptr - ptr to items to be enqueued
  *meta_info_ptr - ptr to ps meta info
  *tx_info_ptr - ptr to bearer context
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_pdp_data_ctrl_setup_cb
(
  ps_phys_link_type *phys_link_ptr,
  dsm_item_type     **item_ptr,
  ps_meta_info_type *meta_info_ptr,
  void              *tx_info_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_PDP_IP_TX_UM_DATA_CB

DESCRIPTION
  This function directly enqueues the data into the other bearer's rx wm
 
PARAMETERS
  *phys_link_ptr - ptr to the phys link
  **item_ptr - ptr to items to be enqueued
  *meta_info_ptr - ptr to ps meta info
  *tx_info_ptr - ptr to bearer context
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_pdp_ip_tx_um_data_cb
(
  ps_phys_link_type *phys_link_ptr,
  dsm_item_type     **item_ptr,
  ps_meta_info_type *meta_info_ptr,
  void              *tx_info_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_GET_IPV4_ADDR

DESCRIPTION
  This function sets the ra_received flag for a bearer to indicate that ipv6
  is complete
 
PARAMETERS
  bearer_id - bearer to set the flag
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_set_bearer_ra_received
(
  uint8 bearer_id
);

#ifdef FEATURE_LAPP_UFS 
#error code not present
#endif /* #ifdef FEATURE_LAPP_UFS */

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_SEND_LOOPBACK_PARAMS

DESCRIPTION
  This function sets the loopback parameters i.e packet replication factor
  and packet delay
 
PARAMETERS
  subs_id             subs id
  eps_id              EPS id
  uplink_direction    Uplink direction of the bearer
  downlink_direction  Downlink direction of the bearer
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_loopback_hdlr_send_loopback_params
(
   sys_modem_as_id_e_type  subs_id,
   uint32                  replication,
   uint32                  delay
);

#ifdef FEATURE_LAPP_UFS 
#error code not present
#endif

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_SEND_RAB_REESTAB_IND

DESCRIPTION
  This function is responsible for sending the CM_CALL_EVENT_RAB_REESTAB_IND
  indication for LTE. This function is invoked from the diag handler.
 
PARAMETERS
.. 
 
DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_send_rab_reestab_ind(uint8 eps_id);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_SEND_RAB_RELEASE_IND

DESCRIPTION
  This function is responsible for sending the CM_CALL_EVENT_RAB_REL_IND
  indication for LTE. This function is invoked from the diag handler.
 
PARAMETERS
.. 
 
DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_send_rab_release_ind(uint8 eps_id);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_SEND_RAB_REJECT_IND

DESCRIPTION
  This function is responsible for sending the CM_CALL_EVENT_RAB_REJ_IND
  indication for LTE. This function is invoked from the diag handler.
 
PARAMETERS
.. 
 
DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_loopback_send_rab_reject_ind(uint8 eps_id);

/*===========================================================================
FUNCTION  DS_3GPP_LOOPBACK_SET_RAB_REJECT

DESCRIPTION
  This function sets the parameter that controls whether rab reject or rab reetab indication 
  will be sent for rab reestab request.

PARAMETERS     1. subs_id: Subscription id
               2. bearer_direction: UL/DL directions

DEPENDENCIES
  None.

RETURN VALUE
  True if successful else false

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_set_rab_reject
(
  sys_modem_as_id_e_type  subs_id,
  boolean                 rab_reject
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_RAB_REESTAB REQ

DESCRIPTION
  This function is responsible for handling rab reestablish request. This will send back 
  a rab reestablish indication or a rab reject depending on if 
  DS3GDIAGMGR_CMD_CODE_REQ_ITEM_3GPP_SEND_RAB_MESSAGE has been used to set a reject response
 
PARAMETERS
   
 
DEPENDENCIES
  

RETURN VALUE
..
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_loopback_rab_reestab_req
(
   cm_rab_reestab_params_s_type  *rab_params
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_SEND_RAB_REESTAB_IND_DIRECTION

DESCRIPTION
  This function is responsible for sending the CM_CALL_EVENT_RAB_REESTAB_IND
  indication for LTE with direction.
 
PARAMETERS
  eps_id              -- Bearer ID
  uplink_direction    -- Current uplink direction
  downlink_direction  -- Current downlink direction
 
DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_send_rab_reestab_ind_direction
(
  uint8                    eps_id,
  uint8                    uplink_direction,
  uint8                    downlink_direction
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_TRIGGER_SWITCH_FAILURE

DESCRIPTION 
  This function provides the ability to exercise switch failure scenarios
  by setting failure timers for specific switch related procedures.
 
PARAMETERS 
  bearer_id              : EPS bearer ID 
  trigger_switch_failure : Enable or disable switch failures
  procedure              : specific procedure to fail
  timer_ms               : timer value in ms
  subs_id                : subscription id. Using sys_modem_as_id_e_type.
 
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_hdlr_trigger_switch_failure
(    
  uint8                          bearer_id,
  boolean                        trigger_switch_failure,
  uint8                          procedure,
  uint32                         timer_ms,
  uint32                         subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_HDLR_DEQUEUE_CFG_PARAMS

DESCRIPTION
  This function handles dequeue timer expiry upon which all the packets from
  UL WM are dequeued and put in DL WM. Timer will be restarted once all
  packets are dequeued.
 
PARAMETERS  
  dequeue_mode           -- Dequeue mode - Timer mode or threshold mode
  dequeue_timer_val_ms   -- Timer value in msec
  dequeue_bytes          -- Num bytes to dequeue on timer expiry
  min_range_bytes        -- Min thresold to wait for dequeueing packets
  max_range_bytes        -- Max thresold to wait for dequeueing packets
 
DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_hdlr_dequeue_cfg_params
(
  uint8                          dequeue_mode, 
  uint16                         dequeue_timer_val_ms,
  uint32                         dequeue_bytes,
  uint32                         min_range_bytes, 
  uint32                         max_range_bytes
);

/*===========================================================================
FUNCTION ds_3gpp_loopback_hdlr_set_post_dequeue_ops_params

DESCRIPTION
  This function sets paramters for post dequeue operations that need to be
  performed before giving the packet to IPA
 
PARAMETERS  
  post_dequeue_op   -- operation mode (emulate pkt loss or reordering)
  percentage        -- Percentage of pkts to operate on.
 
DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_hdlr_set_post_dequeue_ops_params
(
  uint8                     post_dequeue_op,
  uint8                     percentage
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_GET_CNTRL_PDU

DESCRIPTION
  This function creates a dsm item to be transferred from 4G to 5G as part
  of control PDU movement.
  Currently dsm item will carry information regarding EPS bearer ID, subs ID
  and number of packets in the WM at the time of switch.
 
PARAMETERS
  eps_id    -- Bearer ID
  subs_id   -- Subsription ID

DEPENDENCIES
  None

RETURN VALUE
  Pointer to dsm item
 
SIDE EFFECTS
  None.

===========================================================================*/
dsm_item_type* ds_3gpp_loopback_get_cntrl_pdu
(
  uint8                                     eps_id,
  sys_modem_as_id_e_type                    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_FREE_CNTRL_PDU

DESCRIPTION
  This function frees the DSM item once its transmitted from 4G to 5G modem
 
PARAMETERS
  dsm_item_ptr    -- pointer to DSM item

DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_free_cntrl_pdu
(
  dsm_item_type                        *dsm_item_ptr
);

/*===========================================================================
FUNCTION ds_3gpp_loopback_send_volte_switch_ind

DESCRIPTION
  This function sends volte switch ind
 
PARAMETERS 
  subs_id       : subscription id. Using sys_modem_as_id_e_type.
  volte status  : if TRUE, VOLTE status on, otherwise off

DEPENDENCIES
  None

RETURN VALUE
 TRUE/FALSE  
 
 
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_loopback_send_volte_switch_ind
(
  uint32                         subs_id,
  boolean                        volte_status
);

/*===========================================================================
FUNCTION DS_3GPP_LOOPBACK_CNTRL_PDU_RECVD

DESCRIPTION
  This function processes the control PDU information received from 5G side.
  Currently we will just print the EPS ID and SUBS_ID for validation
 
PARAMETERS
  dsm_item_ptr    -- pointer to DSM item

DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_cntrl_pdu_recvd
(
  dsm_item_type                        *dsm_item_ptr
);

/*===========================================================================
FUNCTION ds_3gpp_loopback_send_nv_refresh_cmd

DESCRIPTION
  This function sends NV Refresh CMD
 
PARAMETERS 
  subs_id       : subscription id. Using sys_modem_as_id_e_type.

DEPENDENCIES
  None

RETURN VALUE
 None.  
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_loopback_send_nv_refresh_cmd
(
  uint32                         subs_id
);
