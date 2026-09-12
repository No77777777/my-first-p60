/*!
  @file
  ds_3gpp_burst_flow_hdlr.h

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_burst_flow_hdlr.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/04/18   rs      Initial version
===========================================================================*/
#ifndef DS_3GPP_BURST_FLOW_HDLR_H
#define DS_3GPP_BURST_FLOW_HDLR_H

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

#ifdef FEATURE_DATA_FLOW_MGMT

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTX_FLOW_ACK_TIMER_CB

DESCRIPTION
  This function is called when the flow ack timer expires. This call
  back function simply posts a command to DS. DS then processes the command
  and takes suitable action

PARAMETERS
  unsigned long - Call back data, points to the bearer context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_burst_hdlr_flow_ack_timer_cb
(
   unsigned long callback_data
);


/*===========================================================================
FUNCTION DS_BEARER_CNTXT_HS_TIMER_EXP_CMD()

DESCRIPTION
  This function is called when the hs timer callback is expired.
      
PARAMETERS
  client_data_ptr: Subs Id passed as Client Data Ptr

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_3gpp_burst_hdlr_periodic_timer_exp_cmd
(
  void* client_data_ptr 
);


/*===========================================================================
FUNCTION DS_BEARER_CNTXT_START_HS_TIMER_IF_REQUIRED

DESCRIPTION
  This function checks if there is atleast one bearer in up state 
  in the passed subscription and if the bp timer is not running; the 
  bp timer is started per subscription
  
PARAMETERS
  subs_id  - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_burst_hdlr_start_periodic_timer_if_required
(
   sys_modem_as_id_e_type     subs_id
);


/*===========================================================================
FUNCTION DS_BEARER_CNTXT_STOP_HS_TIMER_IF_NEEDED

DESCRIPTION
  This function checks if there is no bearer in up state 
  in the passed subscription and if the bp timer is running; the 
  bp timer is stopped per subscription
  
PARAMETERS
  subs_id  - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_burst_hdlr_stop_periodic_timer_if_required
(
   sys_modem_as_id_e_type     subs_id
);


/*===========================================================================
FUNCTION DS_BEARER_CNTXT_HS_TIMER_CB()

DESCRIPTION
  This function is registered as the hs timer call back. 
  When the timer is expired, this function is called. The processing is 
  serialized by posting a command to DS.  
      
PARAMETERS
   callback_data : Passed subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_3gpp_burst_hdlr_periodic_timer_cb
(
  void*              callback_data /* Pass Subs id here */
);


/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_SYNC_DISABLE_FLOW

DESCRIPTION
  This function checks if the uplink is 5g,sends sync reqiuest else
  disables flow . 
  
PARAMETERS
  bearer_context_p  :Bearer Context Pointer
  flow_mask         :Flow mask

DEPENDENCIES
  None.

RETURN VALUE
  TRUE,  if the handle is valid
  FALSE, if invalid

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_burst_hdlr_sync_disable_flow
(
  ds_bearer_context_s           *bearer_context_p,
  uint64                        flow_mask
);


/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_GET_GRANT_ENABLE_FLOW

DESCRIPTION
  This function checks if the uplink is 5g,sends grant reqiuest else
  enables flow . 
  
PARAMETERS
  bearer_context_p  :Bearer Context Pointer
  flow_mask         :Flow mask

DEPENDENCIES
  None.

RETURN VALUE
  TRUE,  if the handle is valid
  FALSE, if invalid

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_burst_hdlr_get_grant_enable_flow
(
  ds_bearer_context_s           *bearer_context_p,
  uint64                         flow_mask
);

/*===========================================================================
FUNCTION DS_3GPP_BURST_HDLR_POST_QMAP_ACK_CNF_CMD

DESCRIPTION
  This function is called when the flow ack ioctl is invoked This call
  back function simply posts a command to DS. DS then processes the command
  and takes suitable action

PARAMETERS
  unsigned long - Call back data, points to the bearer context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_burst_hdlr_post_qmap_ack_cnf_cmd
(
  ds_bearer_context_s        *bearer_cntx_p,
  uint8                       flow_ctl_ack
);


/*===========================================================================
FUNCTION DS_3GPP_BURST_HDLR_PROCESS_QMAP_ACK_CNF_CMD

DESCRIPTION
  This function is called when the qmap ack confirmation is processed.

PARAMETERS
  ds_bearer_context_s - Call back data, points to the bearer context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_burst_hdlr_process_qmap_ack_cnf_cmd
(
  ds_bearer_context_s         *bearer_context_p,
  uint8                        ack
);


/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTX_PROCESS_FLOW_ACK_TIMER_EXP_CMD

DESCRIPTION
  This function is called when the flow ack timer expires.

PARAMETERS
  ds_bearer_context_s - Call back data, points to the bearer context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_burst_hdlr_process_flow_ack_timer_exp_cmd
(
  ds_bearer_context_s*      bearer_context_p
);



/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTX_POST_FLOW_CTRL_CMD

DESCRIPTION
  This function is called when the flow ack ioctl is invoked This call
  back function simply posts a command to DS. DS then processes the command
  and takes suitable action

PARAMETERS
  bearer_cntx_p - bearer context pointer\
  flow_enable - TRUE/FALSE whether flow needs to be enabled or disabled
  flow_mask  - flow mask

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_burst_hdlr_post_flow_ctrl_cmd
(
  ds_bearer_context_s        *bearer_cntx_p,
  boolean                    flow_enable,
  uint64                     flow_mask
);


/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTXT_PS_TO_L2_FLOW_CB_HDLR

DESCRIPTION
  This function processes flow control command and enable the flow or disable
  flow
  
PARAMETERS  
  bearer_cntx_p - bearer context pointer\
  flow_enable - TRUE/FALSE whether flow needs to be enabled or disabled
  flow_mask  - flow mask
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_burst_hdlr_process_flow_ctl_cmd
(
  ds_bearer_context_s       *bearer_context_p,
  boolean                   flow_enable,
  uint64                    flow_mask
);


#ifdef FEATURE_LAPP_UFS
#error code not present
#endif  /* FEATURE_LAPP_UFS */

/*===========================================================================
FUNCTION DS3_BURST_FLOW_PROH_TIMER_CB

DESCRIPTION
  This function is called when the burst flow prohibit timer expires. This call
  back function simply posts a command to DS. DS then processes the command
  and takes suitable action

PARAMETERS
  unsigned long - Call back data, points to the bearer context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_burst_flow_proh_timer_cb
(
   unsigned long callback_data
);

/*===========================================================================
FUNCTION DS_3GPP_BEARER_CNTX_PROCESS_PROHIBIT_TIMER_CMD

DESCRIPTION
  This function is called when the flow ack timer expires.

PARAMETERS
  ds_bearer_context_s - Call back data, points to the bearer context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_burst_hdlr_process_prohibit_timer_cmd
(
  ds_bearer_context_s*      bearer_context_p
);

#endif /* FEATURE_DATA_FLOW_MGMT */

#endif /* FEATURE_DATA_3GPP */
#endif /* DS_3GPP_BEARER_CONTEXT_EX_H */
