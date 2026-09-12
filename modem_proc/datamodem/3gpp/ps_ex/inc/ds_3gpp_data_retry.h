#ifndef DS_3GPP_DATA_RETRY_H
#define DS_3GPP_DATA_RETRY_H

/*!
  @file
  ds_3gpp_data_retry.h

  @brief
  Internal utility functions and routines

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_data_retry.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/17/16   vs      New file for Data Retry module

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "rex.h"
#include "ds_3gpp_pdn_throttle_sm.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/
/*-------------------------------------------------------------------------- 
  Default value for the IOT throttling PDN reject timer
-------------------------------------------------------------------------*/
#define DS_3GPP_DEFAULT_IOT_TPDN_REJECT_TIMER 24

/*---------------------------------------------------------------------------
  This structure is used to read the NV item(NV:iot_device). 
  It determines if the device is IOT or not and the IOT throttling timer 
  for PDN reject for the below error codes 
   8   : operator determined barring
   27  : missing or unknown APN when the T3396 IE is absent or T3396 is set 0
   29  : user authentication failed
   32  : service option not supported
   33  : requested service option not subscribed
   112 : APN restriction value incompatible with active EPS bearer context
          (for the case where the UE is already attached)
---------------------------------------------------------------------------*/
typedef struct
{ 
  boolean iot_device;      /* NV item to determine if its a IOT device or not */
  uint8   tpdn_reject_iot; /* Throttling PDN reject timer (0-48hrs) Def:24hrs */
}ds_3gpp_iot_throttle_s;

/*===========================================================================
FUNCTION  DS_3GPP_DATA_RETRY_POWER_UP_INIT      

DESCRIPTION
  This function performs power up initialization of the Data Retry
  Throttling module

PARAMETERS
  None.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_data_retry_power_up_init
(
  void 
);

/*===========================================================================
FUNCTION  DS_3GPP_DATA_RETRY_PER_SUBS_INIT      

DESCRIPTION
  This function performs subscription specific initialization of the Data Retry
  Throttling module

PARAMETERS
  None.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_data_retry_per_subs_init
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_DATA_RETRY_IS_ENABLED     

DESCRIPTION
  This function checks whether Data Retry is enabled.

PARAMETERS
  Subscription Id
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if enabled
  FALSE, otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_3gpp_data_retry_is_enabled
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_DATA_RETRY_HANDLE_REJECT_WITH_T3396_INACTIVE
 
DESCRIPTION
  This function handles the reject case where T3396 timer is inactive
  
PARAMETERS
  1. RAT Based PDN Throt Pointer
  3. Throt Identification pointer
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_data_retry_handle_reject_with_t3396_inactive
(
  ds_3gpp_pdn_throt_rat_based_s      *rat_based_pdn_throt_p,
  ds_3gpp_pdn_throt_ident_info_type  *throt_ident_info_p
);

/*===========================================================================
  FUNCTION DS_3GPP_DATA_RETRY_HANDLE_EMM_CAUSE

  DESCRIPTION
  This function handles service request throttling for PDN Connectivity
  rejects with EMM Cause. 
  
  PARAMETERS
  mode_info_p            - Mode Info
  subs_id                - Subscription Id
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None. 
===========================================================================*/
void ds_3gpp_data_retry_handle_emm_cause
(
  cm_call_mode_info_s_type         *mode_info_p,
  sys_modem_as_id_e_type            subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_IS_IOT_THROTTLING_ENABLED

DESCRIPTION
  This function checks if given IOT throttling is enabled.

PARAMETERS   :
   sys_modem_as_id_e_type subs_id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_pdn_throt_is_iot_throttling_enabled
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_CLEAR_IOT_ON_USIM_REMOVAL

DESCRIPTION
  This function clears IOT throttling on removal of USIM.

PARAMETERS
  subs_id
  clear_throttling

DEPENDENCIES
  void

RETURN VALUE  
  void

SIDE EFFECTS

===========================================================================*/
void ds_3gpp_pdn_throt_clear_iot_on_usim_removal
(
  sys_modem_as_id_e_type subs_id
);

/*==========================================================================
  FUNCTION DS_3GPP_DATA_RETRY_INC_FAILURE_CNTR

  DESCRIPTION
  This would increment the failure counter based on data retry spec. 
  
  PARAMETERS
  void* throt_sm_void_ptr,
  void* cb_data
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None. 
===========================================================================*/
void ds_3gpp_data_retry_inc_failure_cntr
(
  void* throt_sm_void_ptr,
  void* cb_data
);
#endif /* DS_3GPP_DATA_RETRY_H */
