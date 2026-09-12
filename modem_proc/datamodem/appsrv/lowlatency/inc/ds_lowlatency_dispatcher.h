/*===========================================================================

                       DS_LOWLATENCY_DISPATCHER.H
DESCRIPTION

   header of DS low latency action dispatcher

EXTERNALIZED FUNCTIONS

 Copyright (c) 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                       EDIT HISTORY FOR FILE
 
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/lowlatency/inc/ds_lowlatency_dispatcher.h#2 $
  $Author: pwbldsvc $  $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who          what, where, why
--------   ------     ---------------------------------------------------------- 
04/13/18   youjunf      changes for LLM phase2  
07/05/17   youjunf      Initial version
===========================================================================*/

#ifndef DS_LOWLATENCY_DISPATCHER_H
#define DS_LOWLATENCY_DISPATCHER_H

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#include "datamodem_variation.h"
#include "ps_sys_ioctl.h"
#include "ds3gsubsmgr.h"
#include "sys.h"

/*===========================================================================
                       TYPE DEFINITIONS
===========================================================================*/

typedef struct
{
  boolean              high_prio_ps_call;
  uint32               ul_latency_value;
  uint32               dl_latency_value;
  boolean              ipa_reduce_aggr;
  boolean              l2_reduce_aggr;
  uint32               target_paging_cycle;
  uint32               paging_cycle_set_to_pcs;
  uint32               cdrx_cycle;
  uint32               rrc_conn_extn_amt;
  uint8                sleep_disallow_mask;
  boolean              enable_ard_llm;
  uint32               pdcp_discard_timer; 
  boolean              enable_aps_llm;
  boolean 			   enable_fast_rrc_release_llm;
}ds_low_latency_action_s_type;

extern ps_sys_ioctl_data_low_latency_info_type low_latency_info_cache[DS3GSUBSMGR_SUBS_ID_MAX];

extern ds_low_latency_action_s_type  low_latency_action_cache[DS3GSUBSMGR_SUBS_ID_MAX];

/*===========================================================================
                   EXTERNAL FUNCTIONS
===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif

/*=================================================================================== 
FUNCTION      DS_LOWLATENCY_DISPATCHER_INIT

DESCRIPTION   This function initializes ds low latency module 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_lowlatency_dispatcher_init(void);


/*==========================================================================
FUNCTION      DS_LOWLATENCY_NV_REFRESH

DESCRIPTION
  Reset and re-initialize ANDSF module during nv refresh event

PARAMETERS
  ds_subs_id[in] - ds subs id on which nv refresh happened

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/ 
void ds_lowlatency_nv_refresh(ds_sys_subscription_enum_type ds_subs_id);


/*===================================================================================
FUNCTION DS_LOWLATENCY_PS_IOCTL_SET_DATA_LOW_LATENCY

DESCRIPTION
  This function handles setting low latency info IOCTL from PS

DEPENDENCIES None

RETURN VALUE
 0 - Success
 -1- Failure

SIDE EFFECTS None
========================================================================================*/
int ds_lowlatency_ps_ioctl_set_low_latency_info
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===================================================================================
FUNCTION DS_LOWLATENCY_PS_IOCTL_GET_LOW_LATENCY

DESCRIPTION
  This function handles getting low latency info IOCTL from PS


DEPENDENCIES None

RETURN VALUE
 0 - Success
 -1- Failure

SIDE EFFECTS None
========================================================================================*/
int ds_lowlatency_ps_ioctl_get_low_latency_info
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);
/*==========================================================================
FUNCTION      DS_LOWLATENCY_LPM_ON_HANDLING

DESCRIPTION
  Reset lower layer's lowlatency config during LPM on event

PARAMETERS
  subs_id[in] - subs id on which lpm on happen

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/ 
void ds_lowlatency_lpm_on_handling
(
  ds_sys_subscription_enum_type ds_subs_id
);
/*==========================================================================
FUNCTION      DS_LOWLATENCY_LPM_OFF_HANDLING

DESCRIPTION
  need to reconfig low latency parameters to lower layers after LPM OFF

PARAMETERS
  subs_id[in] - subs id on which lpm off happen

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/ 
void ds_lowlatency_mode_online_handling
(
  ds_sys_subscription_enum_type ds_subs_id
);
/*==========================================================================
FUNCTION      DS_LOWLATENCY_LPM_OFF_HANDLING

DESCRIPTION
  need to reconfig some low latency parameters to lower layers after device sim
  mode change like from multisim to singlesim or vice verse
  currently only pdcp discard timer will be reconfig

PARAMETERS
  subs_id[in] - subs id 

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/ 
void ds_lowlatency_device_sim_mode_change_handling
(
  ds_sys_subscription_enum_type ds_subs_id
);


#ifdef __cplusplus
}
#endif
#endif /* DS_LOWLATENCY_DISPATCHER_H */





