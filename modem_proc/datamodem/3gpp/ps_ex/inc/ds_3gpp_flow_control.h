
/*!
  @file
  ds_3gpp_flow_control.h

  @brief
  Manages Flow contexts and provides interface to PDN and bearer

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009-2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_flow_control.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/12/10   sa      Added support for dynamic cid for flow context.
01/20/10   vd      Changes to support UE Qos modification.
07/17/09   vd      Initial version.
===========================================================================*/

#ifndef DS3GPPFLOWCONTROL_H
#define DS3GPPFLOWCONTROL_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_3GPP
#include "ds3gmgr.h"
#include "ds_3gpp_bearer_context.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/
typedef enum
{  
   DS_OFFLOAD_FC_TIMER_START = 0,    ///<  start flow disable timer 
   DS_OFFLOAD_FC_EVENT = 1,          ///<  flow disable timer expire
   DS_OFFLOAD_FC_TIMER_CLEAR = 2     ///<  clear flow disable timer
} ds_3gpp_offload_fc_status_enum;

typedef PACK(struct)
{
  ds_3gpp_offload_fc_status_enum     offload_fc_status;  ///< status for offload fc
  uint8                              reserved[3];        ///< reserved
  uint32                             bearer_inst;        ///< bearer context instance
  uint32                             timer_duration;     ///< flow disable timer duration
}ds_3gpp_offload_fc_event_type;    

typedef enum
{  
   DS_FCF_TUNEAWAY_INVALID    = 0,   ///<  invalid state
   DS_FCF_TUNEAWAY_STOP       = 1,   ///<  start tune away
   DS_FCF_TUNEAWAY_START      = 2    ///<  stop tune away
} ds_3gpp_fcf_tuneaway_status_enum;

typedef enum
{  
   DS_FCF_TUNEAWAY_FLOW_CTL_INVALID    = 0,   ///<  invalid state
   DS_FCF_TUNEAWAY_FLOW_CTL_STOP       = 1,   ///<  stop flow control
   DS_FCF_TUNEAWAY_FLOW_CTL_START      = 2    ///<  start flow control
} ds_3gpp_fcf_tuneaway_flow_ctl_status_enum;

typedef enum
{  
   DS_FCF_TUNEAWAY_TIMER_INVALID    = 0,   ///<  invalid state
   DS_FCF_TUNEAWAY_TIMER_STOP       = 1,   ///<  stop timer
   DS_FCF_TUNEAWAY_TIMER_START      = 2,   ///<  start timer
   DS_FCF_TUNEAWAY_TIMER_EXPIRE     = 3    ///<  timer expires
} ds_3gpp_fcf_tuneaway_timer_status_enum;

typedef PACK(struct)
{
  ds_3gpp_fcf_tuneaway_status_enum           tuneaway_status;  ///< start or stop tuneaway indication
  ds_3gpp_fcf_tuneaway_flow_ctl_status_enum  flow_ctl_status;  ///< enable or disable flow control indication
  ds_3gpp_fcf_tuneaway_timer_status_enum     fcf_timer_status; ///< fcf timer status
  uint8                                      fcf_timer_val;    ///< fcf timer duration
  uint32                                     bearer_inst;      ///< bearer context instance
  uint32                                     current_cnt;      ///< current count in the watermark
  uint64                                     reverved;         ///< move ul_tput_in_bytes to ds_3gpp_bidir_tput_event_type
  uint8                                      reserved[4];      ///< reserved
}ds_3gpp_fcf_tuneaway_event_type;    

typedef PACK(struct)
{
  uint16         ul_tput_threshold;///< UL throughput threshold, in Mbps
  uint16         dl_tput_threshold;///< DL throughput threshold, in Mbps
  uint16         ul_tput;          ///< UL throughput, in Mbps
  uint16         dl_tput;          ///< DL throughput, in Mbps
  boolean        ack_eval;         ///< indicate if ack cnt satisfy threshold
  boolean        tcp_bidir;        ///< the BiDir flag indicate to ML1
  uint8          reserved[10];     ///< reserved
}ds_3gpp_bidir_tput_event_type;    

/*---------------------------------------------------------------------------
  Struct to read Bi-Dir parameters in EFS
---------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  boolean   fcf_enable; /* FCF enable flag */
  uint32    timer_val;  /* FCF timer value, unit is second */
  uint16    uplink_throughput;  /* UL Tput threshold to detect Bi-Dir session, in Mbps*/
  uint16    downlink_throughput;/* DL Tput threshold to detect Bi-Dir session, in Mbps*/
}ds_3gpp_fcf_bidir_struct_type;

extern ds_3gpp_fcf_bidir_struct_type ds_3gpp_fcf_bidir_params;

#define DS3GPP_BIDIR_WIN_COUNT 2 /*num of consecutive windows each of 2s duration*/
#define DS3GPP_FCF_TMR_DEFAULT_VAL 30 /* in ms */
#define DS3GPP_UPLINK_THROUGHPUT_VAL 4 /* Mbps */
#define DS3GPP_DOWNLINK_THROUGHPUT_VAL 150 /* Mbps */


/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_SET_TCP_BI_DIRECTION

DESCRIPTION
  This function sets the tcp_bi_direction flag  

PARAMETERS
  ds_bearer_context_s  *bearer_cntxt_p
  boolean                     tcp_bi_direction

DEPENDENCIES
  None.

RETURN VALUE


SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_flow_control_set_tcp_bi_direction
(
  ds_bearer_context_s  *bearer_cntxt_p,
  boolean              tcp_bi_direction
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_SET_PENDING_DISABLE_FCF

DESCRIPTION
  This function sets the pending_disable_fcf flag  

PARAMETERS
  ds_bearer_context_s  *bearer_cntxt_p,
  boolean              pending_disable_fcf

DEPENDENCIES
  None.

RETURN VALUE


SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_flow_control_set_pending_disable_fcf
(
  ds_bearer_context_s  *bearer_cntxt_p,
  boolean              pending_disable_fcf
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_SET_LTE_RF_UNAVAIL

DESCRIPTION
  This function sets the lte_rf_unavailable flag when receiving LTE_CPHY_RF_UNAVAILABLE_IND 

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE


SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_flow_control_set_lte_rf_unavail
(
  boolean                 lte_rf_unavailable,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_GET_LTE_RF_UNAVAIL

DESCRIPTION
  This function returns TRUE if LTE is in a tune away gap

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_flow_control_get_lte_rf_unavail
(
  sys_modem_as_id_e_type     subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_ALLOC_FCF_TIMER

DESCRIPTION
  This function allocate the FCF timer

PARAMETERS  
  *bearer_cntxt_p  - bearer_context pointer

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
void ds_3gpp_flow_control_alloc_fcf_timer
(
  ds_bearer_context_s  *bearer_cntxt_p,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_FREE_FCF_TIMER

DESCRIPTION
  This function free the FCF timer handler

PARAMETERS  
  *bearer_cntxt_p  - bearer_context pointer

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
void ds_3gpp_flow_control_free_fcf_timer
(
  ds_bearer_context_s  *bearer_cntxt_p
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_STOP_FCF_TIMER

DESCRIPTION
  This function stops the FCF timer

PARAMETERS  
  *bearer_cntxt_p  - bearer_context pointer

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
void ds_3gpp_flow_control_stop_fcf_timer
(
  ds_bearer_context_s  *bearer_cntxt_p
);


/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_TMR_FCF_EXP_HDLR

DESCRIPTION
  This function disable flow control framework when FCF timer expire
  
PARAMETERS 
  subs_id - subscription for the ue cat
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  ue category number
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_flow_control_tmr_fcf_exp_hdlr
(
  void  *user_data_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_DISABLE_FLOW_CONTROL

DESCRIPTION
  This function disables flow control framework  

PARAMETERS  
  *bearer_cntxt_p  - bearer_context pointer
  subs_id          - subscription id

DEPENDENCIES 
  None.

RETURN VALUE 
  None.

SIDE EFFECTS 
  None.

===========================================================================*/
void ds_3gpp_flow_control_disable_flow_control
(
  ds_bearer_context_s    *bearer_cntxt_p,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_TUNEAWAY_START_HDLR

DESCRIPTION
  This function kick in flow control at the beginning of LTE tune away gap
  
PARAMETERS 
  subs_id - subscription for the ue cat
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  ue category number
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_flow_control_tuneaway_start_hdlr
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_TUNEAWAY_END_HDLR

DESCRIPTION
  This function kick in flow control at the end of LTE tune away gap
  
PARAMETERS 
  subs_id - subscription for the ue cat
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  ue category number
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_flow_control_tuneaway_end_hdlr
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_LTE_CPHY_RF_UNAVAIL_HDLR

DESCRIPTION
  This function is used to handle the message from LTE.This message is
  given by ml1 to provide its tuneaway starting.

PARAMETERS
  msgr_type - UMID of the message sent
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is received correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_3gpp_flow_control_rf_unavail_hdlr
(
  msgr_umid_type             msgr_type,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_LTE_CPHY_RF_AVAIL_HDLR

DESCRIPTION
  This function is used to handle the message from LTE.This message is
  given by ml1 to provide its tuneaway ending.

PARAMETERS
  msgr_type - UMID of the message sent
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is received correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_3gpp_flow_control_rf_avail_hdlr
(
  msgr_umid_type             msgr_type,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_BIDIR_EVAL_BY_THROUGHPUT

DESCRIPTION
  This function evaluate TCP Bi-Directional session by throughput across all bearers
  of PDN type as INTERNET
  
PARAMETERS 
  subs_id - subscription for the ue cat
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_flow_control_bidir_eval_by_throughput
(
  boolean                ack_eval,
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_FLOW_CONTROL_OFFLOAD_FC_LOGGING

DESCRIPTION   this function log the offload flow control info

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_3gpp_flow_control_offload_fc_logging
(
  ds_3gpp_offload_fc_status_enum     offload_fc_status,  
  uint32                             bearer_inst,        
  uint32                             timer_duration,     
  sys_modem_as_id_e_type             subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_FLOW_CONTROL_FCF_TUNEAWAY_LOGGING

DESCRIPTION   this function log the FCF and tuneaway info

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_3gpp_flow_control_fcf_tuneaway_logging
(
  ds_3gpp_fcf_tuneaway_status_enum          tuneaway_status,
  ds_3gpp_fcf_tuneaway_flow_ctl_status_enum fc_status,
  uint32                                    current_cnt,      
  uint64                                    current_tput,      
  uint32                                    bearer_inst,      
  ds_3gpp_fcf_tuneaway_timer_status_enum    fcf_timer_status,
  uint8                                     fcf_timer_val,
  sys_modem_as_id_e_type                    subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_FLOW_CONTROL_BIDIR_TPUT_LOGGING

DESCRIPTION   this function log the threshold and throughput to report bidir to ML1

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_3gpp_flow_control_bidir_tput_logging
(
  uint16                  ul_tput_threshold,
  uint16                  dl_tput_threshold,
  uint16                  ul_tput,
  uint16                  dl_tput,
  boolean                 ack_eval,
  boolean                 tcp_bidir,
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_GET_LTE_PAGE_SHARE_STATUS

DESCRIPTION
  This function returns TRUE if LTE is in page share state

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_flow_control_get_lte_page_share_status(void);

/*===========================================================================
FUNCTION DS_3GPP_FLOW_CONTROL_PAGE_SHARE_STATUS_HDLR

DESCRIPTION
  This function is used to handle the message from LTE RRC to indicates 
  whether page is shared on both SUBs.

PARAMETERS
  msgr_type - UMID of the message sent
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE is message is received correctly

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean ds_3gpp_flow_control_page_share_status_hdlr
(
  msgr_umid_type             msgr_type,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

#endif /* FEATURE_DATA_3GPP */
#endif /* DS3GPPFLOWCONTROL_H */

