#ifndef __DS3GPP_EXT_MSG_H__
#define __DS3GPP_EXT_MSG_H__
/*===========================================================================

  Copyright (c) 2012 - 2018 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/api/ds3gpp_ext_msg.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/10/12   ssk      Initial version
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "customer.h"

#include "msgr.h"
#include "msgr_ds.h"
#include "dsumtspdpreg.h"
#include "cm_gw.h"

/* Message IDs
*/

#define DS_3GPP_INTERNAL_MSGR_ID_OFFSET         0x00
#define DS_3GPP_EXTERNAL_MSGR_ID_OFFSET         0x20


/* Enumeration for Traffic class information available as a part of
   UMTS QOS */
typedef enum { 
	DS_TRF_CLASS_CONVERSATIONAL = 1, 
	DS_TRF_CLASS_STREAMING = 2, 
	DS_TRF_CLASS_INTERACTIVE = 3, 
	DS_TRF_CLASS_BACKGROUND = 4, 
} ds_3gpp_traffic_class_enum_type; 

/* Enumeration for QCI information available as a part of
   LTE QOS */
typedef enum { 
	DS_LTE_QCI_1 = 1,
	DS_LTE_QCI_2 = 2, 
	DS_LTE_QCI_3 = 3, 
	DS_LTE_QCI_4 = 4, 
	DS_LTE_QCI_5 = 5, 
	DS_LTE_QCI_6 = 6, 
	DS_LTE_QCI_7 = 7, 
	DS_LTE_QCI_8 = 8, 
	DS_LTE_QCI_9 = 9,
 } ds_3gpp_lte_qci_enum_type; 


typedef enum
{
  DS_LOW_LATENCY_TRAFFIC_STATE_STOP,
  DS_LOW_LATENCY_TRAFFIC_STATE_START,
  DS_LOW_LATENCY_TRAFFIC_STATE_CONTINUE,
  DS_LOW_LATENCY_TRAFFIC_STATE_MAX 
} ds_low_latency_traffic_state_enum_type;

/* Enumeration for supported RATs */
typedef enum {   
  DS_RAT_WCDMA = 0, 
  DS_RAT_TDS = 1, 
  DS_RAT_LTE = 2, 
} ds_3gpp_rat_enum_type; 

typedef struct { 
  byte rab_id; /* Network service access point identifier for this UMTS bearer.*/           
  ds_3gpp_traffic_class_enum_type traffic_class; /* QOS information of this UMTS bearer. */ 
} ds_3gpp_tds_w_qos_info_type; 

typedef struct { 
  byte eps_bearer_id; /* Id of this EPS Bearer. */ 
  ds_3gpp_lte_qci_enum_type qci_value ; /* QOS information of this EPS bearer. */ 
} ds_3gpp_lte_qos_info_type; 

/* Bearer QOS Info Message */
typedef struct { 
  msgr_hdr_struct_type hdr; /* message header */ 
  
  /* Message Payload */ 
  ds_3gpp_rat_enum_type current_bearer_rat; 
  
  union { 
     ds_3gpp_tds_w_qos_info_type tds_w_qos_info; // Valid when bearer is in WCDMA or TDSCDMA
     ds_3gpp_lte_qos_info_type lte_qos_info; //Valid when bearer is in LTE
  } u;

} ds_3gpp_bearer_qos_info_msg_s;

/* embms content desc update msg */
typedef struct
{
  msgr_hdr_s                         msg_hdr;         /* message header */

  /*------------------------------------
   |   payload size  N   |  payload  |
           m                            
   |   4 bytes (uin32)   |  N bytes  |
  -------------------------------------*/
  msgr_attach_s                      dsm_attach;
} ds_3gpp_embms_content_desc_update_msg_s;

/* embms content desc control msg */
typedef struct
{
  msgr_hdr_s                         msg_hdr;         /* message header */

  /*------------------------------------
   |   payload size  N   |  payload  |
           m                            
   |   4 bytes (uin32)   |  N bytes  |
  -------------------------------------*/
  msgr_attach_s                      dsm_attach;
} ds_3gpp_embms_content_desc_control_msg_s;

/* ENUMs for PDN status */
typedef enum {
  DS_PDN_STATUS_DOWN = 0,  /* PDN is in down state */
  DS_PDN_STATUS_UP   = 1,  /* PDN is in up state */
} ds_pdn_status_e_type;

/* IMS PDN Info Message */
typedef struct {
  msgr_hdr_struct_type  hdr;             /* message header */ 
  ds_pdn_status_e_type  ims_pdn_status;  /* IMS PDN status */
}ds_ims_pdn_info_msg_s;

/* UMID definitions
*/
enum
{ 
  MSGR_DEFINE_UMID(DS, 3GPP, IND, BEARER_QOS_INFO, 
                   0x21, ds_3gpp_bearer_qos_info_msg_s),  
};

enum
{
  MSGR_DEFINE_UMID(DS, LTE, REQ, EMBMS_CONTENT_DESC_UPDATE,
                   0x21, ds_3gpp_embms_content_desc_update_msg_s),
  MSGR_DEFINE_UMID(DS, LTE, IND, EMBMS_CONTENT_DESC_CONTROL,
                   0x22, ds_3gpp_embms_content_desc_control_msg_s),
};

typedef struct
{
   msgr_hdr_struct_type hdr; /* message header */ 
  /*! Traffic State */  
  ds_low_latency_traffic_state_enum_type   traffic_state;
  
  /*! Filter result */  
  uint32    filter_handle;    
  /*! Expected periodicity of the packet arrivals for this low latency 
    traffic (in ms).
    Value 0xFFFFFFFF - means unknown */
  uint32                     pkt_interval_time_in_ms;
  
  /*! Expected delta time of the packet from its interval time to wake up
    Value 0xFFFFFFFF - means unknown */
  uint32                     pkt_arrival_delta_in_ms;
  /*! Expected time to wait for the packets at every period 
    for this low latency traffic (in ms).
    Value 0xFFFFFFFF - means unknown  */
  uint32                     pkt_max_wait_time_in_ms;
} ds_mgr_latency_info_ext_type;


typedef enum
{
  DS_3GPP_UPLINK_SCALING_UDC_OFF,
  DS_3GPP_UPLINK_SCALING_UDC_ON
} ds_3gpp_uplink_scaling_reason_enum_type;

typedef struct
{
  msgr_hdr_struct_type                       msg_hdr;            
  /* message header */
  uint16                                     scaling_factor;
  ds_3gpp_uplink_scaling_reason_enum_type    scaling_reason;
} ds_3gpp_uplink_tput_scaling_msg_s;

enum
{ 
  MSGR_DEFINE_UMID(DS, MGR, IND, LOW_LATENCY, 0x24, ds_mgr_latency_info_ext_type),
};

enum
{
  MSGR_DEFINE_UMID(DS, 3GPP, IND, IMS_PDN_INFO, 0x23, ds_ims_pdn_info_msg_s),
};

enum
{
  MSGR_DEFINE_UMID(DS, 3GPP, IND, UPLINK_TPUT_SCALING_INFO, 0x25, ds_3gpp_uplink_tput_scaling_msg_s),
};

typedef struct
{
  msgr_hdr_struct_type                hdr;          /*!< message header */
  /* Message payload */
  uint32                              replication; /*!< Replication */
  uint8                               delay;        /*!< Delay */
} ds_3gpp_nr5g_loopback_diag_ind_s;

enum
{
  MSGR_DEFINE_UMID(DS, 3GPP, IND, NR5G_LOOPBACK_DIAG, 0x26, ds_3gpp_nr5g_loopback_diag_ind_s),
};

typedef struct
{
  msgr_hdr_struct_type                hdr;                     /*!< message header */
  /* Message payload */
  uint8                               current_eps_id;         /*!< EPS ID */
  uint8                               current_call_id;        /*!< Call ID */
  uint32                              clock_vote_mask;        /*!< Clock vote mask */
  uint8                               lb_dl_mulf;              /*!< Multiplication factor */
  uint8                               lb_dl_dupf;              /*!< Duplication factor */
  uint8                               lb_ul_only;              /*!< UL only */
  boolean                             ds_3gpp_loopback_mode;  /*!< Loopback mode */
  boolean                             ds_3gpp_l2_loopback_mode;
  uint8                               loopback_sys_mode;       /*!< Loopback sys mode */
  uint8                               loopback_subs_id;        /*!< Subs id */
  uint32                              replication_factor;      /*!< Replication Factor */
} ds_3gpp_nr5g_loopback_init_ind_s;

enum
{
  MSGR_DEFINE_UMID(DS, 3GPP, IND, NR5G_LOOPBACK_INIT, 0x35, ds_3gpp_nr5g_loopback_init_ind_s),
};

#ifdef FEATURE_LAPP_UFS
#error code not present
#endif /* FEATURE_LAPP_UFS */

/* UMID definitions for DS_3GPP_CM_RS_RA_DONE_IND */
enum
{
  MSGR_DEFINE_UMID(DS, 3GPP, IND, CM_RS_RA_DONE, 0x27, msgr_hdr_struct_type),
};

/* These are for RAN bit rate query from IMS*/
typedef enum
{
  DS_3GPP_BIT_RATE_DOWNLINK      = 0,
  DS_3GPP_BIT_RATE_UPLINK        = 1
} ds_3gpp_bit_rate_direction_e_type;

typedef struct {
  uint8                                eps_bearer_id;
  ds_3gpp_bit_rate_direction_e_type    direction;                           
  uint8                                query_bit_rate;                    
} ds_3gpp_query_bit_data_rate_ind_type;

typedef struct
{
  msgr_hdr_struct_type                       msg_hdr;
  ds_3gpp_query_bit_data_rate_ind_type       query_bit_rate_info;
}ds_3gpp_query_bit_data_rate_ind_msg_s;

enum
{
  MSGR_DEFINE_UMID(DS, 3GPP, IND, QUERY_BIT_RATE, 0x44, ds_3gpp_query_bit_data_rate_ind_msg_s),
};

typedef struct
{
  msgr_hdr_struct_type               msg_hdr;
  boolean                            tcp_bidir; /* Start = 1 and Stop = 0*/                     
}ds_3gpp_tcp_bidir_msg_s_type;

/* UMID definitions for TCP_BIDIR_INFO_IND */
enum
{
  MSGR_DEFINE_UMID(DS, 3GPP, IND, TCP_BIDIR_INFO, 0x45, ds_3gpp_tcp_bidir_msg_s_type),
};
#endif /* !__DS3GPP_EXT_MSG_H__ */
