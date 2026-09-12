#ifndef DS_3GPP_TLB_RM_HDLR_H
#define DS_3GPP_TLB_RM_HDLR_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

          U M T S  L O O P B A C K  M E C H A N I S M

GENERAL DESCRIPTION
  This file contains functions necessary for the implementation of the loopback 
  test mechanism. 

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2015 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

                                                                   
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_tlb_rm_hdlr.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/09/15   ss     Initial version.
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include <stringl/stringl.h>

#include "dsm_item.h"
#include "sys.h"
#include "ps_iface_defs.h"
#include "ps_iface.h"
#include "ps_phys_link.h"
#include "ps_logging_defs.h"
#include "ps_svc.h"

#define DS_3GPP_MAX_IFACE_PER_TLB 2 /* IPv4, IPv6 */
#define DS_3GPP_TLB_IPV4_IFACE 0 /* IPv4 */
#define DS_3GPP_TLB_IPV6_IFACE 1 /* IPv6 */

#define IS_DS3GPPTLB_VALID_INSTANCE(instance)\
    (DS_3GPP_MAX_IFACE_PER_TLB > instance)

#define DS3GPPTLB_GET_INST_FROM_UM_IFACE(iface_ptr,instance, hndl) \
  { \
   if( (TRUE == ps_iface_addr_family_is_v6(iface_ptr)) && \
       (iface_ptr ==  hndl->rmsm_state.um_iface_ptr[DS_3GPP_TLB_IPV6_IFACE]))\
   {\
     instance = DS_3GPP_TLB_IPV6_IFACE;\
   }else if (iface_ptr == hndl->rmsm_state.um_iface_ptr[DS_3GPP_TLB_IPV4_IFACE])\
   {\
     instance = DS_3GPP_TLB_IPV4_IFACE;\
   }\
   else \
   {\
     instance = DS_3GPP_MAX_IFACE_PER_TLB;\
   }\
  }

#define DS3GPPTLB_GET_INST_FROM_RM_IFACE(rm_iface_ptr,instance, hndl) \
  { \
   if(rm_iface_ptr == &(hndl->rmsm_state.rm_lo_b_iface[DS_3GPP_TLB_IPV6_IFACE]))\
   {\
     instance = DS_3GPP_TLB_IPV6_IFACE;\
   }else if(rm_iface_ptr == &(hndl->rmsm_state.rm_lo_b_iface[DS_3GPP_TLB_IPV4_IFACE]))\
   {\
     instance = DS_3GPP_TLB_IPV4_IFACE;\
   }\
   else \
   {\
     instance = DS_3GPP_MAX_IFACE_PER_TLB;\
   }\
  }

#define DS3GPPTLB_GET_INST_FROM_RM_PHYS_LINK_PTR(rm_phys_link_ptr,instance, rm_hndl) \
  { \
   if(rm_phys_link_ptr == \
            &(rm_hndl->rm_lo_phys_link[DS_3GPP_TLB_IPV6_IFACE]))\
   {\
     instance = DS_3GPP_TLB_IPV6_IFACE;\
   }else if(rm_phys_link_ptr == \
            &(rm_hndl->rm_lo_phys_link[DS_3GPP_TLB_IPV4_IFACE]))\
   {\
     instance = DS_3GPP_TLB_IPV4_IFACE;\
   }\
   else \
   {\
     instance = DS_3GPP_MAX_IFACE_PER_TLB;\
   }\
  }

#define DS_3GPP_TLB_MAX_INSTANCES 1

/*-----------------------------------------------------------------------------
  Macro for Low, High and Do not exceed count
  for Mode B Rm iface watermark 
-----------------------------------------------------------------------------*/
#define DS_MODE_B_UL_WM_LO 15200
#define DS_MODE_B_UL_WM_HI 45600
#define DS_MODE_B_UL_WM_DNE 68400

/*----------------------------------------------------------------------------
  Struct to store RM loopback parameters to cater to LTE Loopback ModeB and
  UMTS Loopback Mode4 which require IP packet loopback
----------------------------------------------------------------------------*/
typedef struct
{
  boolean			 init;									 /* initialised or not */
  ps_iface_type *	 um_iface_ptr[DS_3GPP_MAX_IFACE_PER_TLB];/*Um iface pointer*/
  ps_iface_network_type  current_network;		  /* network mode of um iface */
  sys_sys_mode_e_type    current_bearer_mode;
  ps_iface_type 	 rm_lo_b_iface[DS_3GPP_MAX_IFACE_PER_TLB];  
											   /* RM iface for loop back mode */
  ps_phys_link_type  rm_lo_phys_link[DS_3GPP_MAX_IFACE_PER_TLB]; 
						/* physlink data structure	   */
  dsm_watermark_type rm_lo_rx_wmk[DS_3GPP_MAX_IFACE_PER_TLB]; 
						/* watermark for RM loop rx operation */
  q_type			 rm_lo_rx_wmk_q[DS_3GPP_MAX_IFACE_PER_TLB];
						/* queue for RM Loop rx watermark	  */
  boolean			 rm_flow_enabled[DS_3GPP_MAX_IFACE_PER_TLB];
  ps_sig_enum_type	 rx_sig[DS_3GPP_MAX_IFACE_PER_TLB];
  void               *mode_spec_hndl;
}ds_3gpp_rm_tlb_state_type_s;

/*---------------------------------------------------------------------------
  Structure used for Loopback Mode B command
---------------------------------------------------------------------------*/
typedef struct
{
  ps_iface_event_enum_type   event;
  ps_iface_type *     um_iface_ptr;     /* Um iface pointer  */
}ds_3gpp_tlb_iface_event_type;

typedef struct
{
  int                 rm_flow;          /* flow action */
  ps_iface_type *     rm_iface_ptr; /* Rm Iface pointer  */
}ds_3gpp_tlb_rm_flow_evt_type;

typedef  union
{
  ds_3gpp_tlb_iface_event_type    iface_evt_info;
  ds_3gpp_tlb_rm_flow_evt_type    rm_flow_evt_info;
}ds_3gpp_tlb_cmd_type;

typedef struct
{
  int                           event;             /* Event type  */
  ds_3gpp_tlb_cmd_type          event_info;        /* Event info  */
  void                          *hndl;

}ds_3gpp_tlb_cmd_info_type;

void ds_3gpp_tlb_init_rm
(
  ds_3gpp_rm_tlb_state_type_s  *rm_tlb_inst_p,
  void                         *mode_spec_hndl
);

/*===========================================================================
FUNCTION DS_3GPP_TLB_BRING_UP_V4_IFACE

DESCRIPTION
  This function process the loopback mode 
  bringing up the V4 UM iface.
  
DEPENDENCIES 
  None.

RETURN VALUE 
 TRUE - SUCCESS
 FALSE - FAILURE
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
ps_iface_type *ds_3gpp_tlb_bring_up_iface 
(
  ds_umts_pdp_type_enum_type  pdp_type,
  uint16                      prof_num,
  sys_modem_as_id_e_type      subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_TLB_HDLR_REG_UM_EVENTS

DESCRIPTION
  This function registers callbacks for UM events
  
DEPENDENCIES 
  None.

RETURN VALUE 
  TRUE -- SUCCESS
  FALSE -- FAILURE
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_tlb_hdlr_reg_um_events 
(
  ds_3gpp_rm_tlb_state_type_s *rm_tlb_inst,
  sys_modem_as_id_e_type      subs_id,
  ps_iface_event_enum_type    *iface_ev_ptr,
  uint8                       iface_ev_cnt,
  void (*ds_tlb_um_event_handler_cb)(ps_iface_type *, 
                                     ps_iface_event_enum_type,
                                     ps_iface_event_info_u_type,
                                     void *)
);

/*===========================================================================
FUNCTION DS_3GPP_TLB_HDLR_DEREG_UM_EVENTS

DESCRIPTION
  This function registers callbacks for UM events
  
DEPENDENCIES 
  None.

RETURN VALUE 
  None
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_tlb_hdlr_dereg_um_events 
(
  ds_3gpp_rm_tlb_state_type_s *rmsm_state_ptr,
  ps_iface_event_enum_type    *iface_ev_p,
  uint8                       iface_ev_cnt,
  sys_modem_as_id_e_type      subs_id,
  void (*ds_tlb_um_event_handler_cb)(ps_iface_type *, 
                                     ps_iface_event_enum_type,
                                     ps_iface_event_info_u_type,
                                     void *)
);

boolean ds_3gpp_tlb_bringup_um_iface
(
  uint16                       prof_num,
  ds_umts_pdp_type_enum_type   pdp_type,
  sys_modem_as_id_e_type       subs_id,
  ds_3gpp_rm_tlb_state_type_s  *rm_tlb_inst,
  void (*ds_tlb_um_iface_up_evt_hdlr)(ps_iface_type *, void *)  
);

/*===========================================================================
FUNCTION DS_3GPP_TLB_HDLR_REG_RM_EVENTS

DESCRIPTION
  This function registers callbacks for RM phys link flow enable and disable events
  
DEPENDENCIES 
  None.

RETURN VALUE 
  None
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_tlb_hdlr_reg_rm_events 
(
  ps_phys_link_type* rm_phys_link_p,
  ds_3gpp_rm_tlb_state_type_s *rmsm_state_ptr,
  ps_iface_event_enum_type *phys_link_ev_p,
  uint8                     phys_link_ev_cnt
);

/*===========================================================================
FUNCTION DSEPSTLB_HDLR_DEREG_RM_EVENTS

DESCRIPTION
  This function de-registers callbacks for RM phys link flow enable and disable events
  
PARAMETERS  
    
DEPENDENCIES 
  None.

RETURN VALUE 
  None
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_tlb_hdlr_dereg_rm_events 
(
  ps_phys_link_type* rm_phys_link_p,
  ds_3gpp_rm_tlb_state_type_s *rmsm_state_ptr,
  ps_iface_event_enum_type *phys_link_ev_p,
  uint8                     phys_link_ev_cnt
);

boolean ds_3gpp_tlb_setup_rm
(
  ds_3gpp_rm_tlb_state_type_s  *rm_tlb_inst,
  uint8                         instance,
  ps_iface_event_enum_type     *phys_link_ev_p,
  uint8                         phys_link_ev_cnt,
  boolean                      (*ds_tlb_rm_rx_data_cb)(ps_sig_enum_type,
                                                       void *)
);

void ds_3gpp_tlb_hdlr_cleanup_rm
(
  ds_3gpp_rm_tlb_state_type_s  *rmsm_state_ptr,
  ps_iface_event_enum_type     *phys_link_ev_p,
  uint8                         phys_link_ev_cnt,
  sys_modem_as_id_e_type        subs_id
);

void ds_3gpp_tlb_teardown_um_iface
(
  ds_3gpp_rm_tlb_state_type_s  *rm_tlb_inst,
  sys_modem_as_id_e_type        subs_id,
  void (*ds_tlb_um_iface_down_evt_hdlr)(ps_iface_type *, void *)  
);

#endif

