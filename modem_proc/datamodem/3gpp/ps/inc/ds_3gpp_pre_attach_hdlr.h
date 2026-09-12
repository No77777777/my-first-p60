/*!
  @file
  ds_3gpp_pre_attach_hdlr.h

  @brief
  Handles any pre attach activities

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009-2016 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps/inc/ds_3gpp_pre_attach_hdlr.h#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/17/16   vb     Initial version.
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#ifndef DS_3GPP_PRE_ATTACH_HDLR_H
#define DS_3GPP_PRE_ATTACH_HDLR_H

#include "datamodem_variation.h"
#ifdef FEATURE_DATA_3GPP
#include "dstask_v.h"
#include "cm.h"
#include "ps_iface.h"
#include "dsm_queue.h"
#include "ps_ppp.h"
#include "ds3gmgr.h"
#include "ds_3gppi_utils.h"
#include "sys.h"
#include "ps_iface_defs.h"
#include "cm_gw.h"
#include "ps_comp_logging_helper.h"
#include "ps_wmk_logging.h"

/*--------------------------------------------------------------------------- 
  Function ptr that needs to be called to start the dial up procedure
  cb data   ----> cb data passed by the client
  cb_result ----> TRUE/FALSE indication if mode online was successful or not.
  ---------------------------------------------------------------------------*/
typedef void (*ds_3gpp_pre_attach_cb_fn_ptr)
(
  void     *cb_data,
  boolean   cb_result
);

/*--------------------------------------------------------------------- 
  Structure when to handle all pre attach hdlr data like
  timers, cb function ptrs
  --------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_pre_attach_cb_fn_ptr  cb_fn_ptr;
  void                         *cb_data;
  rex_timer_type                pre_attach_cb_timer;  
                                /*If this timer expires, 
                                  always dispatch the timer to the client,
                                  so the client will not be stuck*/
  boolean                       cb_pending;
  cm_srv_domain_pref_e_type     srv_domain_pref;
  cm_mode_pref_e_type           mode_pref;
  void                         *user_data;
  sys_modem_as_id_e_type        subs_id;
}ds_3gpp_pre_attach_cb_data_type;


/*--------------------------------------------------------------------- 
  Structure when set enables to control attach on power up
  NV read structure
  --------------------------------------------------------------------*/
typedef struct
{
  boolean               disable_attach_on_pwr_up;
  boolean               enable_lpm_on_pwr_up;
  uint8                 pre_attach_srv_domain_pref;
  uint8                 pre_attach_mode_pref;
}ds_3gpp_pre_attach_pwr_up_manage_s;

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
);

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
);

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
);
/*===========================================================================
FUNCTION DS_3GPP_PRE_ATTACH_HDLR_CALL_ORIG_REQ

DESCRIPTION 
This function when called checks to see if the NV's are set and if set 
checks to see if UE is in LPM. 
If the UE is in LPM, then request CM to put the device online 
 
PARAMETERS
  profile number
  cb function ptr
  passwd info
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
   char                         passwd_info[],
   boolean                     *cb_ev_active,
   void                        *info_ptr,
   sys_modem_as_id_e_type       subs_id
);

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
void ds_3gpp_pre_attach_process_timer_exp_cmd(void);

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
);

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
);

/*==========================================================================
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
);

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
);

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
);

#endif /* DATA_3GPP*/
#endif /*PRE_ATTACH_HDLR_H*/
