#ifndef DS_3GPP_PDN_CONTEXT_EX_H
#define DS_3GPP_PDN_CONTEXT_EX_H
/*===========================================================================

            3GPP   S P E C I F I C   M O D E   H A N D L E R

===========================================================================*/
/*!
  @file
  ds_3gpp_pdn_context_ex.h

  @brief
  Redial interface

  @detail
  OPTIONAL detailed description of this H header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2008-2018 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_pdn_context_ex.h#6 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
05/04/18   psn     OTASN pDN requirements
04/17/18   zy      Support for tcp retransmit protect
04/04/18   rs      Initial version
==============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "ds_3gpp_hdlr.h"
#include "ps_iface.h"
#include "ps_aclrules.h"
#include "ps_route.h"
#include "dsumtspdpreg.h"
#include "sys.h"
#include "ps_iface_defs.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_bearer_context.h"
#include "ds_3gpp_pco_epco_hdlr.h"

#ifdef FEATURE_DATA_PS_IPV6
#include "dsgen_iface6_hdlr.h"
#include "dstask_v.h"
#endif /* FEATURE_DATA_PS_IPV6 */

#include "ps_sys_conf.h"
#include "ds_3gppi_utils.h"

#include "cm.h"
#include "ds3gmgr.h"
#include "ds3geventmgr.h"
#include "ds_3gpp_epc_if.h"
#include "ds_sys.h"
#include "ds3gtimer.h"

#include "ds_3gpp_bearer_context.h"
/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*---------------------------------------------------------------------------
  3GPP PS Data Off Support Indication Container Identifier field of PCO
  should be 0x0017 : ref: TS24.008 sec 10.5.6.3
---------------------------------------------------------------------------*/
#define DS3GPP_PS_DATA_OFF_CONTAINER_ID              0x0017
#define DS3GPP_PS_DATA_OFF_UE_STATUS_CONTAINER_LEN   1
#define DS3GPP_PS_DATA_OFF_UE_STATUS_DEACTIVATED     0x01
#define DS3GPP_PS_DATA_OFF_UE_STATUS_ACTIVATED       0x02
#define DS3GPP_PS_DATA_OFF_SUPPORT_IND_CONTAINER_LEN 0

#define DS_3GPP_INVALID_IFACE_INSTANCE                   0xFF

typedef struct
{
  ps_iface_type       *iface_ptr;           /* PS Iface pointer      */
  boolean              tcp_level_trm_req;  /*socket option value*/
}ds_3gpp_iface_tcp_level_trm_req_info_type;

/*---------------------------------------------------------------------------
  Structure containing subscription specific globals in ds_3gpp_pdn_context
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
  PDN context table. We define DS_3GPP_MAX_PDN_CONTEXT as the upper bound on
  the number of PDNs we support connectivity to. This number stems from the
  maximum number of available ifaces.
  The default Ifaces can be used to establish connectivity to one PDN (either
  single address bearer or Dual address bearer). The remaining 4 ifaces can
  establish connectivity to 4 other PDNs at the maximum. This is under the
  assumption that we connect to all these PDNs using single address bearers.
  If dual address bearers are used in any of these non default APNs, then we
  will end up using lesser PDN context structures than what we have.
  DS_3GPP_MAX_PDN_CONTEXT has been set to 5 and this is the worst case that
  is to be supported.
---------------------------------------------------------------------------*/
extern ds_pdn_context_s ds_pdn_context_tbl[DS_3GPP_MAX_PDN_CONTEXT] ;

/*---------------------------------------------------------------------------
  Structure containing subscription specific globals in ds_3gpp_pdn_context
---------------------------------------------------------------------------*/
typedef struct
{
  uint32                                  cookie;
  sys_modem_as_id_e_type                  subs_id;
  sys_plmn_id_s_type                      current_plmn;

  /*---------------------------------------------------------------------------
        Below list tracks the blocked PLMNs at any given point. If a PLMN
        gets unblocked it would be removed from this list.
  ---------------------------------------------------------------------------*/
  list_type                               blocked_plmn_list;

  struct
  {
    uint8                                 max_simul_active_pdp_context;
    sys_plmn_id_s_type                    plmn_id;
  }max_simul_active_pdp_info[DS_3GPP_THROT_RAT_MAX];
  
  /*----------------------------------------------------------------------- 
    Timer starts when sub_not_avail is received from CM and
    OTASN PDN is up
    ------------------------------------------------------------------------*/
  rex_timer_type                          otasn_pdn_timer;

  /*-----------------------------------------------------------------------------
    Boolean flag that indicates if the timer is running. Although REX provides
    API which can be used to check if the timer is running, it is more expensive
    in terms of CPU cycles. Hence this boolean flag is maintained. It becomes
    extremely critical to set or reset this flag appropriately as and when the 
    timer is set or reset. Also this boolean should never be set/reset in any
    other task except the DS task.
    Also on timer expiry, this flag has to be explicitly reset in the function
    that handles the timer_expired command
  -----------------------------------------------------------------------------*/
  boolean                                 is_otasn_timer_running;
} ds_3gpp_pdn_cntxt_per_subs_info_type;

/*---------------------------------------------------------------------------
  This counter is used during LPM processing. This keeps track of the number
  of DS IPV6 ifaces (associated DSSNET SM's as well)  which are already up
  and which needs graceful clean up before modem enters into low power mode.
  When there are no IPV6 ifaces/internal state machines to clean up (or)
  if all the IPV6 SM's are cleaned up successfully, notify DSMGR
  ---------------------------------------------------------------------------*/
#ifdef FEATURE_DATA_PS_IPV6
extern int8 ds_3gpp_num_ipv6_ifaces_to_cleanup[DS3GSUBSMGR_SUBS_ID_MAX] ;
#endif

//Macros for strating/stopping OTASN timers
#define OTASN_TIMER_START 1
#define OTASN_TIMER_STOP 0
/*-----------------------------------------------------------------------------
  Macro to start the timer with a given value. It also sets the
  timer_running flag
-----------------------------------------------------------------------------*/
#define DS3GPP_PDN_CNTXT_SET_OTASN_TIMER(hndl)\
  {\
    (void)rex_set_timer(&(hndl->otasn_pdn_timer), SEC_TO_MSEC(OTASN_PDN_TIMER_VALUE));\
    hndl->is_otasn_timer_running = TRUE;\
  }

/*-----------------------------------------------------------------------------
  Macro to stop the timer. It also resets the timer_running flag
-----------------------------------------------------------------------------*/
#define DS3GPP_PDN_CNTXT_CLR_OTASN_TIMER(hndl)\
  {\
    (void)rex_clr_timer(&(hndl->otasn_pdn_timer));\
     hndl->is_otasn_timer_running = FALSE;\
  }
/*===========================================================================
FUNCTION DS_3GPP_PDN_CONTEXT_GET_PER_SUBS_INFO_HNDL

DESCRIPTION
  This function fetches a handle to the PDN Context Information
  corresponding to the given Subscription.

PARAMETERS
  subs_id   	   : Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  Handle to the PDN Context Information for the given Subscription.

SIDE EFFECTS
  None.
===========================================================================*/
ds_3gpp_pdn_cntxt_per_subs_info_type* ds_3gpp_pdn_context_get_per_subs_info_hndl
(
  sys_modem_as_id_e_type  subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CONTEXT_VALIDATE_PER_SUBS_INFO_HNDL

DESCRIPTION
  This function validates the handle to the Subscription Specific
  PDN Context Information.

PARAMETERS
  hndl  : Handle to the Subscription specific
		  PDN Context Information

DEPENDENCIES
  None.

RETURN VALUE
  TRUE,  if the handle is valid
  FALSE, if invalid

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_3gpp_pdn_context_validate_per_subs_info_hndl
(
  ds_3gpp_pdn_cntxt_per_subs_info_type *hndl
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_INITIATE_GRACEFUL_V6_CLEANUP

DESCRIPTION
  This function handles the v6 SM cleanup gracefully based on the
  cm_command notified to it

PARAMETERS
  cm_notify_type- cm command type that is notified
  net_down_reason - the netdown reason to be posted
  subs_id - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void ds_3gpp_pdn_cntxt_initiate_graceful_v6_cleanup
(
  ds_3gpp_pdn_cntxt_cm_notify_cmd_enum_type  cm_notify_type,
  ps_iface_net_down_reason_type              net_down_reason,
  sys_modem_as_id_e_type                     subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_INITIATE_OTASN_GRACEFUL_V6_CLEANUP

DESCRIPTION
  This function handles the v6 SM cleanup gracefully if OTA pdn goes down

PARAMETERS
  cm_notify_type- cm command type that is notified
  net_down_reason - net_down reaosn to be posted
  is_rab_release - is rab relaeased for OTA pdn
  subs_id - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void ds_3gpp_pdn_cntxt_initiate_otasn_graceful_v6_cleanup
(
  ds_3gpp_pdn_cntxt_cm_notify_cmd_enum_type  cm_notify_type,
  ps_iface_net_down_reason_type              net_down_reason,
  boolean                                    is_rab_release,
  sys_modem_as_id_e_type                     subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTX_HANDLE_V6_CLEANUP

DESCRIPTION
  This function handles the v6 SM cleanup gracefully

PARAMETERS
  pdn_cntxt_p- the pointer to the pdn context
  net_down_reason - the netdown reason to be posted
  subs_id - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void ds_3gpp_pdn_cntx_handle_v6_cleanup
(
  ds_pdn_context_s                          *pdn_cntxt_p,
  ps_iface_net_down_reason_type              net_down_reason,
  sys_modem_as_id_e_type                     subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_SET_OTASN_TIMER

DESCRIPTION
  This function starts or Stops the OTASN PDN delay timer aftr doing some validation
  based on the args passed
 
PARAMETERS
   subs_id : subs id 
   set_timer : Start/Stop the timer
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_3gpp_pdn_cntxt_set_otasn_timer
(
  boolean                                  set_timer,
  sys_modem_as_id_e_type                   subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_FALLBACK_TO_LEGACY_SUBS_NOT_AVAIL_PROC 
 
DESCRIPTION
 This function is invoked when CM determines that the currently subs_not_avail
 procedure which is suspended due to OTASN timer has to be immediately resumed
 due to LPM mode taking a precedence.
 
 CM will shorten the timer on their end to 5s and call the caller of this API.
 This will resume the subs_not_avail procedure immediately
  
PARAMETERS   : 
 subs_id

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_cntxt_fallback_to_legacy_subs_not_avail_proc
(
  sys_modem_as_id_e_type              subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_PROCESS_OTASN_TIMER_CLEANUP

DESCRIPTION
  This function determines whether CM has to be notified rightaway for
  3gpp MH's completion of subs_not_avail cmd processing, if OTASN
  timer is still running.
 
  This might happen when MH receives RAB_RELEASE or CM_CALL_END while timer
  is still running
 
  The determination is made based on the internal fucntions included here
 
PARAMETERS 
  pdn_cntxt_ptr : pointer to the PDN_context on which call_end or RAB release 
  was received 
  is_rab_release = Is OTASN PDN call end due to rab release
  subs_id : subs id 

DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_3gpp_pdn_cntxt_process_otasn_timer_cleanup
(
  ds_pdn_context_s  	        *pdn_cntx_p,
  boolean                      is_rab_release,
  sys_modem_as_id_e_type         subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_OTASN_DELAY_TIMER_EXP_CMD_HDLR 
 
DESCRIPTION
  This function handles the OTASN PDN DELAY timer expiration command.
  
PARAMETERS   : 
  is_rab_release  : process otasn PDN cleanup due to rab rel?
  subs_id : Subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_cntxt_otasn_delay_timer_exp_cmd_hdlr
(
  boolean                is_rab_release,
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_IS_PDN_OF_PDN_LABEL_TYPE

DESCRIPTION
  This function checks if the incoming pdn_cntxt_p passed is of type "pdn_label"
 
PARAMETERS 
   pdn_cntx_p  : pointer to PDN Cntxt to check if it is of pdn_label type
   pdn_label : is the pdn_cntxt_p passed in is of this "pdn_label" type
   subs_id : subs id 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE = If incoming PDN is of type pdn_label
  FALSE = otherwise
  
SIDE EFFECTS
  None.
  
===========================================================================*/
boolean ds_3gpp_pdn_cntxt_is_pdn_of_pdn_label_type
(
  ds_pdn_context_s                 *pdn_cntx_p,
  const char                       *pdn_label,
  sys_modem_as_id_e_type            subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_IS_OTASN_TIMER_RUNNING

DESCRIPTION
  This function tells whetehr OTASN PDN delay timer is currently runing on a
  particular subs 
 
PARAMETERS
   subs_id : subs id 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE : If timer is running
  FALSE: Otherwise
  
SIDE EFFECTS
  None.
  
===========================================================================*/
boolean ds_3gpp_pdn_cntxt_is_otasn_timer_running
(
  sys_modem_as_id_e_type                   subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_IS_ANY_OTASN_PDN_UP

DESCRIPTION
  This function loops over all PDN contexts to see if there is atleast one more
  OTASN PDN (apart from the one passed in) in UP state. Whether the PDN passed in is
  UP or not , does not matter. 

  This function does not take emergency PDNs into consideration.

PARAMETERS
  PDN Context = If NULL is passed, it will break at the first OTASN PDN that is UP.
                if pdn_cntxt_p is a valid PDN, it will return true if any other OTASN
                PDN is found in UP state.

DEPENDENCIES
  Incoming PDN has to be a OTASN PDN/NULL

RETURN VALUE
  TRUE, if there is another OTASN PDN in UP state, other than the PDN passed in.
  FALSE, otherwise

SIDE EFFECTS
===========================================================================*/
boolean ds_3gpp_pdn_cntxt_is_any_otasn_pdn_up
(
  ds_pdn_context_s  	           *pdn_cntx_p,
  sys_modem_as_id_e_type            subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_NOTIFY_DSMGR_OF_SUB_NOT_AVAIL

DESCRIPTION
  This function notifies DSMGR that 3GPP MH has fnifhed processing of
  SUB_NOT_AVAIL event from CM

PARAMETERS
  suns_id - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void ds_3gpp_pdn_cntxt_notify_dsmgr_of_sub_not_avail
(
  sys_modem_as_id_e_type                     subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_IS_SUB_NOT_AVAIL_PROCESSING_DELAYED

DESCRIPTION
  This function checks if sub_not_avail processing has been delayed
  due to OTA pdn being up.

PARAMETERS
  subs_id - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  True - if delayed
  False - if not

SIDE EFFECTS

===========================================================================*/
boolean ds_3gpp_pdn_cntxt_is_sub_not_avail_processing_delayed
(
  sys_modem_as_id_e_type                     subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTXT_OTASN_TIMER_CB

DESCRIPTION
  This function is registered as the timer call back. When the otasn pdn timer
  expires , this function is called. The processing is 
  serialized by posting a command to DS.
      
PARAMETERS
   callback_data : subs id 

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_3gpp_pdn_cntxt_otasn_timer_cb
(
  void*              callback_data /* Pass Subs id here */
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTX_TCP_RETRANS_PROTECT_TIMER_CB

DESCRIPTION
  This function is called when TCP retranmit timer expired

PARAMETERS
  unsigned long - Call back data, points to the PDN context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_cntx_tcp_retrans_protect_timer_cb
(
  unsigned long callback_data
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTX_IFACE_IOCTL_TCP_LEVEL_TRM_REQ_CMD

DESCRIPTION   Handles PS_IFACE_IOCTL_SET_TCP_LEVEL_TRM_REQUEST

PARAMETERS
  iface_ptr: Iface on which IOCTL is invoked
  argval_ptr: tcp_level_trm_req value,TRUE or FALSE
  ps_errno: variable to store error type in case of any

RETURN VALUE  0  : if the IOCTL is processed successfully
              -1 : if the IOCTL is does not get processed

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
int ds_3gpp_pdn_cntxt_iface_ioctl_tcp_level_trm_req_cmd
(
  ps_iface_type      *iface_ptr,
  void               *argval_ptr,
  sint15             *ps_errno
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTX_IFACE_IOCTL_TCP_LEVEL_TRM_REQ_HDLR

DESCRIPTION   Handles DS_CMD_3GPP_IFACE_TCP_LEVEL_TRM_REQ_INFO

PARAMETERS
  void  *cmd_payload_ptr

RETURN VALUE  
  0  : if the IOCTL is processed successfully
  -1 : if the IOCTL is does not get processed

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_3gpp_pdn_cntxt_iface_ioctl_tcp_level_trm_req_hdlr
(
  void  *cmd_payload_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTX_PROCESS_TCP_RETRANS_PROTECT_TMR_EXP_CMD

DESCRIPTION
  This function is called while processing the command enqueued to DS when
  the TCP retransmit protect timer expires. This function will post a 
  DS3GEVENTMGR_TCP_RETRANSMIT_EV to ds3g to end tcp retransmion protect

PARAMETERS
  pdn_cntx_p - Pointer to the PDN context to be torn down

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_cntx_process_tcp_retrans_protect_tmr_exp_cmd
(
  ds_pdn_context_s* pdn_cntx_p
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTX_CLEAR_TCP_RETRANS_PROTECT_TMR

DESCRIPTION
  This function will be called when pdn free but tcp_retrans_protect_timer still 
  not expired,it will clear the timer and send DS3GEVENTMGR_TCP_RETRANS_END

PARAMETERS
  pdn_cntx_p - Pointer to the PDN context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_pdn_cntx_clear_tcp_retrans_protect_tmr
(
  ds_pdn_context_s* pdn_cntx_p
);

/*===========================================================================
FUNCTION   ds_3gpp_pdn_cntx_fill_ps_data_off_ue_support_pco_container

DESCRIPTION
  This function fills the PS Data Off UE Support Container for the PDN
  connection request.

PARAMETERS 
  [out]DS level Protocol Config Option Container to be updated
  [in] Subscription ID

DEPENDENCIES 
  None.

RETURN VALUE  
  None.

SIDE EFFECTS 
  Updates Orig_params contents : 
     orig_params_ptr->protocol_config_options.num_protocol_ids will be updated
     to reflect the no. of protocol config options included by this function
     
===========================================================================*/
void ds_3gpp_pdn_cntx_fill_ps_data_off_ue_support_pco_container
(
  sys_modem_as_id_e_type                subs_id,
  ds_3gpp_protocol_cfg_options_s       *ds_proto_config_option_p
);

/*===========================================================================
FUNCTION   ds_3gpp_pdn_cntx_decode_ps_data_off_support_ind

DESCRIPTION
  This function decodes the 3GPP PS Data Off Support Indication from the PCO.
  The network will only send this empty container to the UE to indicate that
  it supports PS Data Off Feature for this PGW.  Otherwise, it will not
  send the container at all.

PARAMETERS
  protocol_pkt_ptr   -  pointer to the PCO contents field
  protocol_pkt_len   -  container length
  pdn_context_p      -  Pointer to current PDN context
  decode_mask        -  Mask pointer containing status of decode operation

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS 
  None.
===========================================================================*/
void ds_3gpp_pdn_cntx_decode_ps_data_off_support_ind
(
  const byte                      *protocol_pkt_ptr,
  byte                             protocol_pkt_len,
  ds_pdn_context_s                *pdn_cntxt_p,
  uint32                          *decode_mask_ptr
);

/*===========================================================================
FUNCTION    DS_3GPP_PDN_CNTX_PS_DATA_OFF_BEARER_RESOURCE_MODIFY_REQ

DESCRIPTION
  This function detects that the PS data off setting has been changed
  and cycles through all the existing up PDNs to see if they support PS Data
  Off both on the UE and NW side.  If so, trigger UE initiated bearer
  resource modification to the network with the PS Data Off setting in the
  PCO container so that network can know when to start and stop limiting
  downlink traffic.
 
PARAMETERS
  Data On Status:  Value indicates whether the overall data status is
                   on, off or unknown
  Subscription ID: Subscription that the bearer resource modification
                   request is for.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_pdn_cntx_ps_data_off_bearer_resource_modify_req
(
  ps_sys_ps_data_avail_status_enum_type data_on_status,
  sys_modem_as_id_e_type                subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_CNTX_GET_BEARER_TECH_TYPE

DESCRIPTION 
  This function translates CM mode to one of the IFACE tech
  type enum values.
  
PARAMETERS
  mode        - mode defined in CM
  bearer_tech - structure to be populated
  subs_id     - Subscription id

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_pdn_cntx_get_bearer_tech_type
(
  sys_sys_mode_e_type               mode,
  ps_iface_bearer_technology_type  *bearer_tech,
  sys_modem_as_id_e_type            subs_id
);

/*===========================================================================
FUNCTION dsPdnContext_bearerUpIndCb

DESCRIPTION
  This function completes PS iface configuration in response to bearer up ind

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void dsPdnContext_bearerUpIndCb
(
  void  						   *object,
  ds_bearer_context_s   		                   *bearer_cntx_p,
  const cm_call_mode_info_s_type                           *mode_info_p
);

/*===========================================================================
FUNCTION      DS_3GPP_PDN_CONTEXT_BRING_IFACES_UP

DESCRIPTION
  This function is used to bring up the ifaces associated to a particular
  pdn cntxt

PARAMETERS
  object      -  void pointer to the bearer context
  pdn_cntx_p  -  pointer to the pdn context

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_pdn_context_bring_ifaces_up
(
  void                           *object,
  ds_pdn_context_s               *pdn_cntx_p
);

/*===========================================================================
FUNCTION ds_3gpp_pdn_cntxt_get_internet_apn_name

DESCRIPTION
  This function get internet apn name

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
boolean ds_3gpp_pdn_cntxt_get_internet_apn_name
(
  sys_modem_as_id_e_type    subs_id,
  byte                     *internet_apn
);
/*===========================================================================
FUNCTION DS_3GPP_PDN_CONTEXT_GET_APN_TYPE_MASK_FROM_PROF_NUM

DESCRIPTION 
  This util function gets the APN type mask configured in the profile.

PARAMETERS
  profile_num    -- Profile number

RETURN VALUE 
  APN type mask

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
apn_type_mask_type ds_3gpp_pdn_context_get_apn_type_mask_from_prof_num
(
  uint16                             profile_num,
  sys_modem_as_id_e_type             subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_CONTEXT_GET_PROF_APN_TYPE

DESCRIPTION 
  This util function gets the APN type from the profile if present. Else it
  will try to retrieve the information from APN settings configured by
  application

PARAMETERS
  pdn_cntxt_ptr -- pointer to PDN context

RETURN VALUE 
  PS sys APN type information

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
ps_iface_apn_enum_type ds_3gpp_pdn_context_get_prof_apn_type
(
  ds_pdn_context_s             *pdn_cntxt_ptr
);

/*===========================================================================
FUNCTION  DS_3GPP_PDN_CNTXT_IS_TARGET_PS_TYPE_PDN

DESCRIPTION
  This function queries whether the requested PDN is certain type of PDN

PARAMETERS
  *pdn_cntxt_ptr  -             pointer to PDN context
  ds_sys_apn_type          -    target type to be checked

DEPENDENCIES
  None.

RETURN VALUE
  Boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_pdn_cntxt_is_target_ps_type_pdn
(
  ds_pdn_context_s               *pdn_cntxt_ptr,
  ps_iface_apn_enum_type          ps_iface_apn_type 
);
/*===========================================================================
FUNCTION      DS_3GPP_PDN_CNTXT_IS_BEARER_ALLOWED_TO_USE_DISCARD_TIMER

DESCRIPTION
  This function determines if input bearer ID is allowed to use discard timer
  Currently only internet default bearer is allowed.

PARAMETERS
  bearer_id   -- ID of the bearer for which discard timer can be used
  subs_id     -- Subscription ID

RETURN VALUE
  TRUE  -- allowed
  FALSE -- Not allowed

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_3gpp_pdn_cntxt_is_bearer_allowed_to_use_discard_timer
(
  byte                   bearer_id,
  uint8                  subs_id
);

#endif /* DS_3GPP_PDN_CONTEXT_H */