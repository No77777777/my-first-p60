/*!
  @file
  ds_bearer_context_ex.h

  @brief
  Lower layer/Bearer Context interface

  @detail
  OPTIONAL detailed description of this C header file.
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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_bearer_context_ex.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/04/18   rs      Initial version
===========================================================================*/
#ifndef DS_3GPP_BEARER_CONTEXT_EX_H
#define DS_3GPP_BEARER_CONTEXT_EX_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_3GPP
#include "ds_3gpp_bearer_context.h"
#include "ps_dpm.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/



/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/
/*=============================================================================
FUNCTION      DS_3GPP_BEARER_CNTXT_EX_DPM_EV_CB_HDLR

DESCRIPTION
  This is the call back function that is called when the Dpm events are invoked.
  Based on these events; the watermark's nonemmpty function callback is set
  reset.
 
PARAMETERS  
  dpm_event       - The event that happened. This is used to differentiate
                    the events that happened : registration, deregistration
                    reregistration
  cback_info_ptr  - Passed in as void*, this gives us the event details 
            
DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void ds_3gpp_bearer_cntxt_ex_dpm_ev_cb_hdlr
(
  ps_dpm_um_ev_enum_type                    dpm_event, 
  ps_dpm_um_ev_cback_info_type             *cback_info_ptr
);


/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_GET_PHYS_LINK_FROM_EPS_ID

DESCRIPTION
  This function returns a pointer to the physlink corresponding to the eps
  bearer ID passed to it.
  
PARAMETERS 
  eps_id  - EPS ID for which the eps bearer context needs to be retrieved
  subs_id - subscription for the ue cat
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  Physlink pointer
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
ps_phys_link_type* ds_3gpp_bearer_cntxt_get_phys_link_from_eps_id
(
  eps_bearer_id_T                      eps_id,
  sys_modem_as_id_e_type               subs_id
);

#ifdef FEATURE_LAPP_UFS
#error code not present
#endif


/*===========================================================================
FUNCTION      DS_3GPP_BEARER_CNTXT_UPLINK_ON_5G

DESCRIPTION
  This wrapper function is used to check if a particular bearer uplink
  is on 5G or not
   
PARAMETERS  
  bearer_context_p  -- pointer to the bearer context
 
DEPENDENCIES
  None.

RETURN VALUE 
  TRUE    -- If bearer is currently configured as 5G
  FALSE   -- otherwise
  
SIDE EFFECTS
  None
===========================================================================*/

boolean ds_3gpp_bearer_cntxt_uplink_on_5g
(
  ds_bearer_context_s           *bearer_context_p
);

#ifdef FEATURE_DATA_WLAN_MAPCON
/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTX_GET_BEARER_TYPE_EX

DESCRIPTION
  This function is used to get the underlying bearer type. 
  
PARAMETERS
  network_mode      - network sys mode
  bearer_type_ex_p  - bearer_type structure that has to be filled in
  ps_errno          - error code to be returned to PS in case of failure
  subs_id           - subscription id

DEPENDENCIES 
  None.
  
RETURN VALUE 
   Integer value is returned.
   0 - SUCCESS
  -1 - FAILURE
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
int ds_3gpp_bearer_cntxt_get_bearer_type_ex
(
  sys_sys_mode_e_type              network_mode,
  ds_sys_system_status_info_type  *bearer_type_ex_p,
  sint15                          *ps_errno,
  sys_modem_as_id_e_type           subs_id
);
#endif /* FEATURE_DATA_WLAN_MAPCON */

/*===========================================================================
FUNCTION  DS_BEARER_CNTXT_HANDLE_MAX_RAB_REESTAB_RETRY_CNT

DESCRIPTION
  This function handles the max rab_reestab_retry count for a partciular
  bearer. The uplink WM is flushed, phys_link flow is enabled and 
  uplink_data_bitmask would be unset for the bearer. The retry counter
  would be reset to zero.
  
PARAMETERS
  bearer_context_p    The bearer context pointer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_bearer_cntxt_handle_max_rab_reestab_retry_cnt
(
  ds_bearer_context_s* bearer_context_p
);

/*===========================================================================
FUNCTION ds_bearer_cntxt_CALL_RAB_RE_ESTAB_REJ_HDLR

DESCRIPTION
  This function handles rab reject indication for 3GPP modes
  
PARAMETERS  
  call_id     -   CM call id
  call_info   -   mode specific call info
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_bearer_cntxt_call_rab_re_estab_rej_hdlr
(
  cm_call_id_type            call_id,
  dsgwl_ps_call_info_u_type *call_info
);

/*===========================================================================
FUNCTION DS_BEARER_CNTXT_DYN_BACK_PRESSURE_EXP_CMD_HDLR
DESCRIPTION
  This is the timer exp hdlr for dynamic back pressure disable. This would be 
  called in PS task context.

PARAMETERS
  user_data_ptr - user data.

DEPENDENCIES
  None.

RETURN VALUE
  None 

SIDE EFFECTS
  None.
===========================================================================*/
void ds_bearer_cntxt_dyn_back_pressure_exp_cmd_hdlr
(
  ds_bearer_context_s  *bearer_cntxt_p
);
/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_EX_IS_BEARER_DORM_OR_SUSPEND

DESCRIPTION
  This function is to check whether bearer is in dormancy or suspend state
  
PARAMETERS  
  bearer_context_ptr
    
DEPENDENCIES 
  None.

RETURN VALUE 
  TRUE - bearer is in dormancy or suspend state
  FALSE - Otherwise
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_bearer_cntxt_ex_is_bearer_dorm_or_suspend
(
  ds_bearer_context_s       *bearer_context_ptr
);

#endif /* FEATURE_DATA_3GPP */
#endif /* DS_3GPP_BEARER_CONTEXT_EX_H */
