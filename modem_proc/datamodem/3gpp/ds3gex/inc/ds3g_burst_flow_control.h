#ifndef DS3G_BURST_FLOW_CONTROL
#define DS3G_BURST_FLOW_CONTROL
/*===========================================================================

                3 G   D A T A   S E R V I C E S   M A N A G E R

                           H E A D E R   F I L E

DESCRIPTION
  This file contains functions and definitions exported for burst based flow control
  Copyright (c) 2018 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ds3gex/inc/ds3g_burst_flow_control.h#1 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/02/15   ms      Introduced this file for burst based

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "rex.h"

#ifdef FEATURE_DATA_FLOW_MGMT

#include "cm.h"
#include "cm_dualsim.h"
#include "sys.h"
#include "dsm.h"
#include "dstask_v.h"
#include "dsat_v.h"
#include "dsmgr_api.h"
#include "ds3gsubsmgr.h"
#include "ds_sys.h"
#include "ds3g_ext.h"
#include "cm_gw.h"
#include "ps_iface_defs.h"
#include "msgr.h"



/*---------------------------------------------------------------------------
  Flow control latency log event
---------------------------------------------------------------------------*/
typedef PACK(struct)
{  
  uint8                eps_id;              
  /* Bearer information */
  uint16               sample_set;          
  /* Num samples used in avg calculation*/
  uint32               average_delay_usec;
  /* Average latency in microseconds */
  uint8                reserved2;
  /* for future use */
  uint16               reserved1;  
  /* for future use */
}ds3g_burst_flow_latency_log_payload_s_type; 

/*---------------------------------------------------------------------------
  Flow control log event
---------------------------------------------------------------------------*/
typedef PACK(struct)
{  
  uint8                bearer_id;              
  /* Bearer information */
  uint32               grant_size;          
  /* grant size*/
  uint32               flow_mask;
  /* flow_mask */
  uint64               current_wm_count;
  /* current wm count */
  uint8                ack_required:1;    
  /* ack_required */
  uint8                hyst_timer_exp:1;
  /* hyst timer expired or not */
  uint8                ul_direction:2;   
  /* uplink direction */ 
  uint8                reserved3:4;
  /* for future use */
  uint8                reserved2;
  /* for future use */
  uint16               reserved1;  
  /* for future use */
}ds3g_burst_flow_log_payload_s_type; 


typedef enum
{
  DS3G_BURST_RAT_UNKNOWN                        = 0,
  DS3G_BURST_RAT_2G                             = 1,
  DS3G_BURST_RAT_3G                             = 2,
  DS3G_BURST_RAT_4G                             = 3,
  DS3G_BURST_RAT_MAX
} ds3g_burst_rata_info;

/*---------------------------------------------------------------------------
   Structure to calculate the average time latency between flow enable and
   AP sending the 1st packet
---------------------------------------------------------------------------*/
typedef struct
{  
  uint32              average_delay_usec;  /* average latency stats in microsecs */
  timetick_type       start_time_tick;     /* Start time in ticks */
  timetick_type       end_time_tick;       /* End time in ticks */
  boolean             compute_avg_latency; /* When to compute the avg */ 
  uint16              sample_set_cnt;      /* Current sample set value */
}ds3g_burst_flow_enable_latency_s_type;

typedef struct
{
  /*periodic timer count */
  uint32                                    periodic_timer_count;

  uint64                                     requested_grant_size;
  /* Flow latency stats */
  ds3g_burst_flow_enable_latency_s_type      flow_latency;
  /* Subscription ID */
  sys_modem_as_id_e_type                     subs_id;
  /* Back pointer to bearer context */
  ps_phys_link_type                         *phys_link_ptr;

  rex_timer_type                            burst_prohibit_timer;

  boolean                                   prohibit_timer_running;

  boolean                                   pending_grant_request;

  boolean                                   grant_request_ack;
}ds3g_burst_based_flow_control_s;

extern int64 ds3g_burst_flow_grant_threshold;

/*===========================================================================
FUNCTION  DS3G_BURST_DISABLE_FLOW

DESCRIPTION
  This function handles the burst based  flow disable api and updates
  the burst based parameters appropriately.
  
PARAMETERS
  phys_link_ptr      Physical Link Pointer
  wm_ptr             WM Pointer
  flow_mask          Flow Mask
  burst_based param  Burst based parameters

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds3g_burst_disable_flow
(
   ps_phys_link_type                   *phys_link_ptr,
   void                                *wm_ptr, 
   uint64                              flow_mask,
   ds3g_burst_based_flow_control_s     *burst_based_param_ptr,
  boolean                              tcp_bidirectional
); 

/*===========================================================================
FUNCTION  DS3G_BURST_SEND_SYNC_REQ

DESCRIPTION
  This function sends the sync message request to 5g. This fills in the eps
  bearer id, flow mask and fileld indicating whether flow is disabled
 
 
PARAMETERS
  eps_bearer_id      Eps bearer id
  flow_enabled       TRUE - indicates flow enable sync/False indicates flow disable
                     sync
  flow_mask          Flow Mask

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds3g_burst_send_sync_req
(
   eps_bearer_id_T                     eps_bearer_id,
   boolean                             flow_enabled,
   uint64                              flow_mask,
   sys_modem_as_id_e_type              subs_id
);

/*===========================================================================
FUNCTION  DS3G_BURST_SEND_GRANT_REQ

DESCRIPTION
  This function sends the grant message request to 5g. This fills in the eps
  bearer id, flow mask 
 
 
PARAMETERS
  eps_bearer_id      Eps bearer id
  flow_enabled       TRUE - indicates flow enable sync/False indicates flow disable
                     sync
  flow_mask          Flow Mask

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds3g_burst_send_grant_req
(
  eps_bearer_id_T                     eps_bearer_id,
  uint64                              flow_mask,
  sys_modem_as_id_e_type               subs_id
);

/*===========================================================================
FUNCTION      DS3_BURST_FLOW_CONTROL_SYNC_RSP_HANDLER

DESCRIPTION
  This function handles the flow control sync response from 5G side. 
    
PARAMETERS  
  msgrtype  - UMID of the message sent
  subs_id   - Subscription ID
  dsmsg_ptr - Pointer to the payload
 
DEPENDENCIES
  None.

RETURN VALUE 
  TRUE   -- if no errors
  FALSE  -- otherwise
  
SIDE EFFECTS
  None
===========================================================================*/
boolean ds3g_burst_flow_control_sync_rsp_handler
(
  msgr_umid_type                msgrtype,
  sys_modem_as_id_e_type        subs_id,
  const msgr_hdr_struct_type    *dsmsg_ptr
);
/*===========================================================================
FUNCTION      DS3G_BURST_FLOW_CONTROL_GRANT_BYTES_RSP_HANDLER

DESCRIPTION
  This function handles the grant bytes  response from 5G side. 
    
PARAMETERS  
  msgrtype  - UMID of the message sent
  subs_id   - Subscription ID
  dsmsg_ptr - Pointer to the payload
 
DEPENDENCIES
  None.

RETURN VALUE 
  TRUE   -- if no errors
  FALSE  -- otherwise
  
SIDE EFFECTS
  None
===========================================================================*/
boolean ds3g_burst_flow_control_grant_bytes_rsp_handler
(
  msgr_umid_type                msgrtype,
  sys_modem_as_id_e_type        subs_id,
  const msgr_hdr_struct_type    *dsmsg_ptr
);
/*===========================================================================
FUNCTION  DS3G_BURST_ENABLE_FLOW

DESCRIPTION
  This function handles the flow enable api and updates the burst based
  parameters appropriately.
  
PARAMETERS
  phys_link_ptr       Physical Link Pointer
  wm_ptr              WM Pointer
  flow_mask           Flow Mask
  burst_based param   Burst based parameters

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds3g_burst_enable_flow
(
   ps_phys_link_type                   *phys_link_ptr,
   void                                *wm_ptr, 
   uint64                              flow_mask,
  ds3g_burst_based_flow_control_s     *burst_based_param_ptr,
  boolean                             tcp_bidirectional
);

/*===========================================================================
FUNCTION  DS3G_BURST_PROCESS_PERIODIC_TIMER_EXP

DESCRIPTION
  This function handles the hysterisis timer expiry api and updates
  the burst based parameters appropriately.
  
PARAMETERS
  phys_link_ptr      Physical Link Pointer
  wm_ptr             WM Pointer
  flow_mask          Flow Mask
  burst_based param  Burst based parameters

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  ds3g_burst_process_periodic_timer_exp
(
  ps_phys_link_type                   *phys_link_ptr,
  void                                *wm_ptr, 
  uint64                              flow_mask,
  ds3g_burst_based_flow_control_s     *burst_based_param_ptr,
  ds3g_burst_rata_info                rat_info,
  boolean                             tcp_bidirectional
);

/*===========================================================================
FUNCTION  DS3G_BURST_LOg_BYTES_RECVD

DESCRIPTION
  This function is used to log the bytes that has been received
  
PARAMETERS
  wm_ptr             WM Pointer
 
DEPENDENCIES
  None.

RETURN VALUE
  boolean .

SIDE EFFECTS
  None.

===========================================================================*/
void ds3g_burst_log_bytes_recvd
(
   void                                *wm_ptr 
);

/*===========================================================================
FUNCTION  DS3G_BURST_ENABLE_FLOW_EX

DESCRIPTION
  This function handles the flow enable api and updates the burst based
  parameters appropriately.In this api grant bytes are passed as additional
  parameter. This api is uses if grant bytes are received from remote module
  
PARAMETERS
  phys_link_ptr       Physical Link Pointer
  wm_ptr              WM Pointer
  flow_mask           Flow Mask
  burst_based param   Burst based parameters

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds3g_burst_enable_flow_ex
(
  ps_phys_link_type                   *phys_link_ptr,
  void                                *wm_ptr, 
  uint64                              flow_mask,
  ds3g_burst_based_flow_control_s     *burst_based_param_ptr,
  uint64                              grant_bytes,
  boolean                             tcp_bidirectional
);
/*===========================================================================
FUNCTION  DS3G_BURST_GET_BYTES_ALLOWED

DESCRIPTION
  This function gets the bytes allowed based on the wm pointer
  
PARAMETERS
  wm_ptr  Wm Pointer

DEPENDENCIES
  None.

RETURN VALUE
  boolean .

SIDE EFFECTS
  None.

===========================================================================*/
INLINE int64 ds3g_burst_get_bytes_allowed
( 
   dsm_watermark_type                   *wm_ptr
)
{
  int64                              bytes_allowed = 0;
  int64                              grant_threshold = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*---------------------------------------------------------------------
    By default set the grant threshold to 5% of the high WM. IF the
    threshold is set through diag, then use that value.
  ---------------------------------------------------------------------*/
  grant_threshold = ds3g_burst_flow_grant_threshold;

  if (grant_threshold == 0)
  {
    grant_threshold = (int64)wm_ptr->hi_watermark / 20;
}

  bytes_allowed = (int64)wm_ptr->hi_watermark - 
     (int64)wm_ptr->current_cnt - (int64)grant_threshold;

  return  ( bytes_allowed > 0 ) ? bytes_allowed: 0;

}/* ds3g_burst_get_bytes_allowed */

/*===========================================================================
FUNCTION  DS_3GPP_BURST_FLOW_LOG_LATENCY_EV

DESCRIPTION
  This function is used to print the flow latency log event in DS task.
 
PARAMETERS
  burst_param_ptr      -- pointer to burst structure
  
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
 
===========================================================================*/
void ds_3gpp_burst_flow_log_latency_ev
(
  ds3g_burst_based_flow_control_s               *burst_param_ptr
);

/*===========================================================================
FUNCTION  DS3G_BURST_FLOW_PROCESS_FLOW_LATENCY_PARAMS

DESCRIPTION
  This function is 
 
PARAMETERS
  start_logging    -- Flag to indicate whether ot not to start logging
  sample_set       -- Number of samples after which log event needs to
                      be printed
 
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
 
===========================================================================*/
void ds3g_burst_flow_process_flow_latency_params
( 
  boolean                              start_logging, 
  uint16                               sample_set
);

/*===========================================================================
FUNCTION  DS3G_BURST_FLOW_COMPUTE_AVG_LATENCY

DESCRIPTION
  This function computes the average time taken for the 1st packet to arrive
  after flow is enabled
 
PARAMETERS
  burst_param_ptr      -- pointer to burst structure  

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds3g_burst_flow_compute_avg_latency
(
  ds3g_burst_based_flow_control_s        *burst_param_ptr 
);

/*===========================================================================
FUNCTION      DS3G_BURST_GET_UL_ON_5G

DESCRIPTION
  This util function is used to check if a particular physlink
  is on 5G or not
   
PARAMETERS  
 phys_link_ptr  - phys link pointer
 
DEPENDENCIES
  None.

RETURN VALUE 
  TRUE    -- If physlink is currently configured as 5G
  FALSE   -- otherwise
  
SIDE EFFECTS
  None
===========================================================================*/

INLINE boolean ds3g_burst_get_ul_on_5g
(
  ps_phys_link_type         *phys_link_ptr
)
{
  boolean ret_val = FALSE;
#ifdef FEATURE_LAPP_DATA
  #error code not present
#endif /*FEATURE_LAPP_DATA*/

  return ret_val;
}

/*===========================================================================
FUNCTION      DS3G_BURST_FLOW_PROCESS_SET_THRESHOLD

DESCRIPTION
  This function is used to set the grant threshold value that can be
  used as part of sending flow enable command
   
PARAMETERS  
 threshold_value  -- Value of threshold in bytes
 
DEPENDENCIES
  None.

RETURN VALUE 
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void ds3g_burst_flow_process_set_threshold
(
  int64                           threshold_value
);

#endif /*FEATURE_DATA_FLOW_MGMT */

#endif
