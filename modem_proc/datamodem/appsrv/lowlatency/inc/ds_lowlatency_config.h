
/*===========================================================================
                      DS_LOWLATENCY_CONFIG.H

DESCRIPTION
 Config header file for DS low latency module

EXTERNALIZED FUNCTIONS

 Copyright (c) 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================
                       EDIT HISTORY FOR FILE
 
  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/lowlatency/inc/ds_lowlatency_config.h#2 $
  $Author: pwbldsvc $  $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who          what, where, why
--------   ------     ---------------------------------------------------------- 
04/13/18   youjunf      changes for LLM phase2  
07/05/17   youjunf      Initial version
===========================================================================*/

#ifndef DS_LOWLATENCY_CONFIG_H
#define DS_LOWLATENCY_CONFIG_H

/*===========================================================================
                   INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#include "ps_sys_ioctl.h"



/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/
typedef enum
{
  LLM_CAP_MIN = 0,
  LLM_CAP_IPA_REDUCE_AGGR = LLM_CAP_MIN,
  LLM_CAP_L2_REDUCE_AGGR,                   
  LLM_CAP_L2_UL_OPT,                        
  LLM_CAP_L2_DL_OPT,                        
  LLM_CAP_NAS_REDUCE_PAGING_CYCLE,          
  LLM_CAP_ML1_REDUCE_CDRX_CYCLE,             
  LLM_CAP_L2_EXTEND_RRC_CONN,               
  LLM_CAP_ML1_DISALLOW_DEEPEST_SLEEP_MODE,  
  LLM_CAP_ML1_DISALLOW_DEPPER_SLEEP_MODE,   
  LLM_CAP_ML1_ARD_LLM,
  LLM_CAP_L2_DISCARD_TIMER,
  LLM_CAP_FAST_RRC_RELEASE,
  LLM_CAP_MAX = LLM_CAP_FAST_RRC_RELEASE
}ds_lowlatency_cap_id_e_type;



/*===========================================================================
                   EXTERNAL FUNCTIONS
===========================================================================*/

/*===========================================================================
FUNCTION      DS_KEEPALIVE_ASSIST_CFG_INIT

DESCRIPTION   This function initializes ds low latency config module 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_lowlatency_cfg_init(void);




/*===========================================================================
FUNCTION      DS_LOWLATENCY_CFG_NV_REFRESH_INIT

DESCRIPTION   This function initializes DS low latency configuration module 
              per sub according to NV refresh 

PARAMETERS    subs_id[xin] - subs idx on which nv refresh happened 
 
DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_lowlatency_cfg_nv_refresh_init(uint8 subs_idx);


/*===========================================================================
FUNCTION      DS_LOWLATENCY_CFG_GET_LATENCY_VALUE

DESCRIPTION   This function get low latency value according to latency_level  

DEPENDENCIES  None

RETURN VALUE  latency_value

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_lowlatency_cfg_get_latency_value
(
  uint8                          subs_idx,
  boolean                        is_ul,
  ps_sys_latency_level_enum_type latency_level
);


/*===========================================================================
FUNCTION      DS_LOWLATENCY_CFG_GET_PAGING_CYCLE

DESCRIPTION   This function get paging cycle duration according to latency_level  

DEPENDENCIES  None

RETURN VALUE  paging cycle duration

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_lowlatency_cfg_get_paging_cycle
( 
  uint8                          subs_idx,      
  ps_sys_latency_level_enum_type latency_level
);



/*===========================================================================
FUNCTION      DS_LOWLATENCY_CFG_GET_CDRX_CYCLE

DESCRIPTION   This function get cdrx cycle duration according to latency_level  

DEPENDENCIES  None

RETURN VALUE  cdrx cycle duration

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_lowlatency_cfg_get_cdrx_cycle
( 
  uint8                          subs_idx,      
  ps_sys_latency_level_enum_type latency_level
);


/*===========================================================================
FUNCTION      DS_LOWLATENCY_CFG_GET_RRC_CONN_EXTN_AMT

DESCRIPTION   This function get the amount of rrc connection extension 
              according to latency_level  

DEPENDENCIES  None

RETURN VALUE  amount of rrc connection extension

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_lowlatency_cfg_get_rrc_conn_extn_amt
( 
  uint8                          subs_idx,      
  ps_sys_latency_level_enum_type latency_level
);


/*===========================================================================
FUNCTION      DS_LOWLATENCY_CFG_GET_CAPABILITY

DESCRIPTION   This function get the capability value
              according to latency_level and cap_id

DEPENDENCIES  None

RETURN VALUE  capability value 
              TRUE is enabled
              FALSE is disabled

SIDE EFFECTS  None
===========================================================================*/
boolean ds_lowlatency_cfg_get_capability
(
  uint8                          subs_idx,
  ps_sys_latency_level_enum_type latency_level,  
  ds_lowlatency_cap_id_e_type    cap_id                                    
);
/*===========================================================================
FUNCTION      DS_LOWLATENCY_CFG_GET_PDCP_DISCARD_TIMER

DESCRIPTION   This function get the discard time according to latency_level  

DEPENDENCIES  None

RETURN VALUE  amount of rrc connection extension

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_lowlatency_cfg_get_pdcp_discard_timer
( 
  uint8                          subs_idx,      
  ps_sys_latency_level_enum_type latency_level
);

/*===========================================================================
FUNCTION      DS_LOWLATENCY_CFG_GET_NV_UL_FLOW_OPT_ENABLE

DESCRIPTION   This function get NV low_latency_ul_flow_opt_enable

DEPENDENCIES  None

RETURN VALUE  TRUE
              FALSE

SIDE EFFECTS  None
===========================================================================*/
int ds_lowlatency_cfg_get_nv_ul_flow_opt_enable
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);
/*===========================================================================
FUNCTION DS_LOWLATENCY_GET_NV_APS_ENABLE

DESCRIPTION
  This function reads the NV item ds_lowlatency_aps_enable

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_lowlatency_get_nv_aps_enable
(
  uint8  subs_indx
);
/*===========================================================================
FUNCTION      DS_LOWLATENCY_CFG_GET_PDCP_DISCARD_TIMER_MSIM_OFFSET

DESCRIPTION   This function get the cached pdcp_discard_timer_msim_offset  

DEPENDENCIES  None

RETURN VALUE  uint32

SIDE EFFECTS  None
===========================================================================*/
uint32 ds_lowlatency_cfg_get_pdcp_discard_timer_msim_offset
( 
  uint8                          subs_idx
);

#endif /* DS_LOWLATENCY_CONFIG_H */
