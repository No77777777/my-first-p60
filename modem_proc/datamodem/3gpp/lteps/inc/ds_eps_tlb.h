/*!
  @file
  ds_eps_tlb.h

  @brief
  Test loopback interface

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009 - 2012 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/lteps/inc/ds_eps_tlb.h#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/12   sa      Fixed issue with V4V6 data path.
11/17/11   sa      Added support for V6 Loopback Mode B.
08/10/10   sa      Added support of LTE_TLB_LB_OPEN_IND.
04/21/10   sa      Added support for Loopback Mode B.
08/31/09   ml      Initial revision
===========================================================================*/

#ifndef DS_EPS_TLB_H
#define DS_EPS_TLB_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "ps_iface_defs.h"
#include "ps_meta_info.h"
#include "ps_svc.h"
#include "dsm_pool.h"
#include "lte_tlb.h"
#include "ds_3gpp_bearer_context.h"
#include "ds_eps_bearer_context.h"
#include "ds_3gpp_tlb_rm_hdlr.h"
#include "sys.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#define DS_MAX_TLB_RM_NUM_IFACES 1

typedef  enum
{
  DS_EPS_RM_FLOW_EVT = 0x0,
  DS_EPS_MODE_B_TMR_EXP,
  DS_EPS_TLB_CMD_MAX = 0xFF
}ds_eps_tlb_cmd_e_type;


#define DS_EPS_RM_FLOW_ENABLE  0
#define DS_EPS_RM_FLOW_DISABLE 1


/*-----------------------------------------------------------------------------
  This enum describes the current state of test loopback mode
-----------------------------------------------------------------------------*/
typedef enum
{
  DS_EPS_BEARER_TLB_INACTIVE,   /* Test loopback is inactive */
  DS_EPS_BEARER_TLB_ACTIVE,     /* Test loopback is active */
  DS_EPS_BEARER_TLB_A_CLOSED,   /* Test loopback is closed for mode A */
  DS_EPS_BEARER_TLB_B_CLOSED,   /* Test loopback is closed for mode B */
  DS_EPS_BEARER_TLB_C_CLOSED,   /* Test loopback is closed for mode C */
  DS_EPS_BEARER_TLB_B_OPEN,     /* Test loopback is open for Mode B   */
  DS_EPS_BEARER_TLB_C_OPEN,     /* Test loopback is open for Mode C   */
  DS_EPS_BEARER_TLB_MAX
} ds_eps_bearer_tlb_state_e;


/*-----------------------------------------------------------------------------
  This structure holds the state info of test loopback mode B
-----------------------------------------------------------------------------*/
typedef struct
{
  ds_eps_bearer_tlb_state_e  ds_eps_bearer_tlb_state;
  lte_tlb_mode_e    mode; /*!< TLB mode: Mode A or B or C*/
  boolean           buffer_ip_pdu;      /* Buffer_IP_PDU  */
  boolean           cdma_2000_inited;   /* CDMA 2000 flag */
  dword             ip_pdu_delay_time;  /* IP delay in ms */
  boolean           tmr_delay_modeB_running;
  rex_timer_type    tmr_delay_modeB;
  boolean           sent_close_loopback_ind;
  ds_bearer_context_wm_s      tlb_rx_wm;
}ds_eps_tlb_state_type;

/*-----------------------------------------------------------------------------
  Subscription Specific Informatio of the TLB Module
------------------------------------------------------------------------------*/
typedef struct
{
  uint32                       cookie;
  /* Test loopback mode B RMSM state */
  ds_3gpp_rm_tlb_state_type_s rmsm_state;

  /* Test loopback mode state variables */
  ds_eps_tlb_state_type        tlb_state;

  sys_modem_as_id_e_type       subs_id;

  boolean                      in_use;
} ds_eps_tlb_per_subs_info_type;

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_eps_tlb_qsh_mdump_collect
  
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
void ds_eps_tlb_qsh_mdump_collect
(
  void
);
#endif

/*==========================================================================
FUNCTION ds_eps_tlb_get_per_subs_info_hndl

DESCRIPTION
  This function is used to fetch the TLB information corresponding to
  the given Subs Id
 
PARAMETERS 
  Subs Id - Subscription Id
 
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to the TLB Info
  
SIDE EFFECTS
  None.
  
===========================================================================*/
ds_eps_tlb_per_subs_info_type* ds_eps_tlb_get_per_subs_info_hndl
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DSEPSTLB_INIT

DESCRIPTION
  Initializes test loopback specific handler
  
PARAMETERS  
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void dsEpsTlb_init (void);

/*===========================================================================
FUNCTION DSEPSTLB_PDP_IP_TX_UM_DATA_CB

DESCRIPTION
  This function handles uplink data for bearers established in TLB mode. 
  For TLB mode A, all data will be simply discarded. TLB mode B is currently 
  not supported
  
PARAMETERS  
  call_id:     CM call id
  mode_info_p: mode specific info pointer
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void dsEpsTlb_pdp_ip_tx_um_data_cb
(
  ps_phys_link_type *phys_link_ptr,
  dsm_item_type     **item_ptr,
  ps_meta_info_type *meta_info_ptr,
  void              *tx_info_ptr
);

/*===========================================================================
FUNCTION DSEPSTLB_PDP_IP_RX_UM_DROP_DATA_CB

DESCRIPTION
  This funciton dequeues RX watermar and drops data
  
PARAMETERS  
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean dsEpsTlb_pdp_ip_rx_um_drop_data_cb
(
  ps_sig_enum_type sig,
  void             *user_data_p
);


/*===========================================================================
FUNCTION DSEPSTLB_MODEB_HDLR_CMD()

DESCRIPTION
  This function is called when we process the command posted to DS as a result
  of loopback ModeB .

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void dsEpsTlb_modeB_hdlr_cmd
(
  const ds_cmd_type *cmd_ptr                         /* Pointer to Command */
);

/*===========================================================================
FUNCTION dsEpsTlb_is_tlb_in_mode_C()

DESCRIPTION
  This function is used to check if the UE is in Test loop mode Mode C.

PARAMSTERS 
  subs_id: Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  Boolean 1 = True  ( Test mode in Mode C)
          0 = False ( Not in Test Mode C)
  
SIDE EFFECTS
  None.
  
===========================================================================*/
boolean dsEpsTlb_is_tlb_in_mode_C
(
  sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION dsEpsTlb_set_tlb_mode_A_params

DESCRIPTION
  This function is used to set the tlb rx watermark nonempty call back 
  function and set the signal handler for TLB Mode A

PARAMETERS
 Bearer Index Pointer

DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void dsEpsTlb_set_tlb_mode_A_params
(
  int index
);

/*==============================================================================

FUNCTION DS_3GPP_EMBMS_PER_SUBS_INFO_INIT

DESCRIPTION
  This function allocates and initializes TLB information for the given Subs Id.

PARAMETERS
  subs_id - Subscription Id
  
DEPENDENCIES  
  None

RETURN VALUE
  None

SIDE EFFECTS  
  None
==============================================================================*/
void ds_eps_tlb_per_subs_info_init
(
  sys_modem_as_id_e_type subs_id
);

/*==========================================================================
FUNCTION ds_eps_tlb_validate_per_subs_info_hndl

DESCRIPTION
  This function is used to validate the TLB information handle.
 
PARAMETERS 
  hndl    - Pointer to the TLB Information  
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if the handle is valid
  FALSE, otherwise
  
SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_eps_tlb_validate_per_subs_info_hndl
(
  ds_eps_tlb_per_subs_info_type*  hndl
);

/*===========================================================================
  FUNCTION DSEPSTLB_POST_IFACE_UP_IND_MODE_C

  DESCRIPTION
    This function post IFACE UP indication. If close ready 
    indication is not already send to lower layers, it will be 
    send for Mode C in this function. 

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void dsEpsTlb_post_iface_up_ind_mode_C
(
  sys_modem_as_id_e_type subs_id
);




/*==============================================================================
FUNCTION      DS_EPS_TLB_FREE_CTRL_BLK

DESCRIPTION
  This function is used to free the TLB Info block corresponding to the
  given subs_id.

PARAMETERS
  subs_id: Subscription Id

DEPENDENCIES  
  None

RETURN VALUE  
  None

SIDE EFFECTS
  None
==============================================================================*/
void ds_eps_tlb_free_subs_info_hndl
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION   DSEPSTLB_UM_EVENT_HANDLER_CB

DESCRIPTION
  This callback function is called when one of the registered ps_iface events
  occur on the UM_PKT_IFACE. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
/*ARGSUSED*/
void dsEpsTlb_um_event_handler_cb
(
  ps_iface_type             *this_iface_ptr,
  ps_iface_event_enum_type   event,
  ps_iface_event_info_u_type event_info,
  void                      *user_data_ptr
);

/*==========================================================================
FUNCTION ds_eps_tlb_mode_deactivate

DESCRIPTION
  This function is used to send deactivate TLB IND to clean up eps TLB
  states.
 
PARAMETERS 
  None.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
===========================================================================*/
void ds_eps_tlb_mode_deactivate(void);

/*===========================================================================
FUNCTION DSEPSTLB_RM_RX_DATA_CB

DESCRIPTION
  This function handles rx data on the RM iface which were established in 
  TLB mode. 
  In TLB is closed in mode B, data will be looped back. Otherwise, data will
  be dropped
  
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE if success, FALSE otherwise.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean dsEpsTlb_rm_rx_data_cb
(
  ps_sig_enum_type sig,
  void             *user_data_p
);
/*==========================================================================
FUNCTION ds_eps_tlb_validate_set_tlb_tx_function

DESCRIPTION
This function is used to validate and set the tlb tx function
 in case of tlb Mode B and Mode C
 
PARAMETERS 
  None.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
===========================================================================*/

void ds_eps_tlb_validate_set_tlb_tx_function
(
  ds_bearer_context_s        *bearer_context_p
);

/*===========================================================================
FUNCTION ds_eps_tlb_if_TLB_in_target_mode

DESCRIPTION
  This function check if the sub is in TLB mode in target MODE
 
  
PARAMETERS  
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
 None
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_eps_tlb_if_TLB_in_target_mode
(
  sys_modem_as_id_e_type         subs_id,
  sys_sys_mode_e_type            target_mode
);
#endif /* DS_EPS_TLB_H */
