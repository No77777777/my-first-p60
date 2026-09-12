/*===========================================================================
                             DS_LOWLATENCY_EXT_MSG_H

DESCRIPTION
  This file defines the interface to modem low latency dispatcher service
 
Copyright (c) 2017 Qualcomm Technologies Incorporated. All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/api/public/ds_lowlatency_ext_msg.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who          what, where, why
--------   ------     ---------------------------------------------------------- 
04/13/18   youjunf      Add more message definition for LLM phase2
06/18/17   youjunf      Initial version
===========================================================================*/

#ifndef DS_LOWLATENCY_EXT_MSG_H
#define DS_LOWLATENCY_EXT_MSG_H

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "comdef.h"
#include "msgr.h"
#include "msgr_ds.h"

/*===========================================================================
                  MACROS, TYPEDEFS AND VARIABLES
===========================================================================*/
#define DS_APPSRV_LOW_LATENCY_SET_MODE_INFO_IND_ID            0x01
#define DS_APPSRV_LOW_LATENCY_L2_REDUCE_AGGR_THR_IND_ID       0x02
#define DS_APPSRV_LLM_REDUCE_CDRX_CYCLE_IND_ID                0x03
#define DS_APPSRV_LLM_EXTEND_RRC_CONN_IND_ID                  0x04
#define DS_APPSRV_LLM_CONTROL_SLEEP_MODE_IND_ID               0x05
#define DS_APPSRV_LLM_SET_ARD_LLM_IND_ID                      0x06
#define DS_APPSRV_LLM_GET_ARD_LLM_REQ_ID                      0x07
#define DS_APPSRV_LLM_SET_LEVEL_IND_ID                        0x08
#define DS_APPSRV_LLM_PDCP_DISCARD_TIMER_IND_ID               0x09
#define DS_APPSRV_LLM_SET_APS_IND_ID                          0x0A
#define DS_APPSRV_LLM_SET_FAST_RRC_RELEASE_IND_ID             0x0B



/*sleep mode diallow_mask bits*/
#define DS_LLM_DISALLOW_SLEEP_MASK_BIT_DEEP         0x01
#define DS_LLM_DISALLOW_SLEEP_MASK_BIT_DEEP_LIGHT   0x02



/* low latency mode change ind */
typedef struct
{
  msgr_hdr_s  msg_hdr;            /* Message router header */
  boolean     is_ul_invoke;       /* invoke uplink low latency mode or not */          
  uint32      ul_latency_value;   /* uplink latency value in ms */
  boolean     is_dl_invoke;       /* invoke downlink low latency mode or not */
  uint32      dl_latency_value;   /* downlink latency value in ms */
}ds_low_latency_set_mode_info_ind_s;


/* reduce aggregation threshold ind */
typedef struct
{
  msgr_hdr_s  msg_hdr;            /* Message router header */
  boolean     invoke_reduce;      /* invoke recuding aggregation threshold or not*/
}ds_low_latency_reduce_aggr_info_ind_s;


/* reduce cdrx cycle ind */
typedef struct
{
  msgr_hdr_s  msg_hdr;                 /* Message router header */
  /* boolean flag to indicate whether to reduce CDRX cycle.
     if false, use default CDRX cycle
     if true, reduce CDRX cycle with desired cdrx_cycle_duration*/
  boolean     invoke_reduce;          
  uint32      cdrx_cycle_duration;      /* indicate desired cdrx cycle duration in ms*/ 
}ds_llm_reduce_cdrx_cycle_ind_s;


/* extend rrc connection ind */
typedef struct
{
  msgr_hdr_s  msg_hdr;                 /* Message router header */
  uint32      conn_extn_amt;            /* rrc connection extension amount in seconds*/ 
}ds_llm_extend_rrc_conn_ind_s;


typedef struct
{
  msgr_hdr_s  msg_hdr;                 
  uint32      discard_timer;  /*pdcp discard timer in ms,max value is 150ms*/          
}ds_pdcp_discard_timer_ind_s; 

/* control ML1 sleep mode ind */
typedef struct
{
  msgr_hdr_s  msg_hdr;                 /* Message router header */
  /* bitmask indicating which sleep mode is disallowed,
     bit 0 corresponds to deep_sleep (refer to: #define DS_LLM_DISALLOW_SLEEP_MASK_BIT_DEEP  0x01)
     bit 1 corresponds to deep_light_sleep (refer to: #define DS_LLM_DISALLOW_SLEEP_MASK_BIT_DEEP_LIGHT   0x2)
   
     bit value will be 1 if disallowed, will be 0 if allowed*/ 
  uint8     disallow_mask;             
}ds_llm_control_sleep_mode_ind_s;


/* ARD low latency mode ind */
typedef struct
{
  msgr_hdr_s  msg_hdr;                 /* Message router header */
  /* if set to true, enable ARD low latency mode, which means ARD conditional 1RX is off
     else disable ARD low latency mode, which means ARD conditional 1RX is allowed */
  boolean     enable_llm;                     
}ds_llm_set_ard_llm_ind_s;


/*ARD low latency mode query sent as a request from ML1*/
typedef struct
{
  msgr_hdr_s  msg_hdr;                 /* Message router header */
}ds_llm_get_ard_llm_req_s;

/*low latency level*/
typedef enum
{
  LLM_NORMAL = 1,
  LLM_LOW,
  LLM_VERY_LOW,
  LLM_SUPER_LOW,
  LLM_RESERVED1,
  LLM_RESERVED2
}ds_llm_level_e_type;


/*set low latency level indication*/
typedef struct
{
  msgr_hdr_s            msg_hdr;        /* Message router header */
  ds_llm_level_e_type   ul_level;       /* uplink latency level*/
  ds_llm_level_e_type   dl_level;       /* downlink latency level*/
}ds_llm_set_level_ind_s;

typedef struct
{
  msgr_hdr_s  msg_hdr;                 /* Message router header */
  /* if set to true, means APS(Adaptive Page Skipping) enable,else means APS disable */
  boolean     enable_aps;                     
}ds_llm_set_aps_ind_s;

typedef struct
{
  msgr_hdr_s  msg_hdr;
  boolean     is_fast_rrc_release_enabled;
}ds_llm_set_fast_rrc_release_ind_s;

enum
{
  /*DS APPSRV send*/
  MSGR_DEFINE_UMID(DS, APPSRV, IND, LOW_LATENCY_SET_MODE_INFO,
                   DS_APPSRV_LOW_LATENCY_SET_MODE_INFO_IND_ID,
                   ds_low_latency_set_mode_info_ind_s),

  /*DS APPSRV send*/
  MSGR_DEFINE_UMID(DS, APPSRV, IND, LOW_LATENCY_L2_REDUCE_AGGR_THR,
                   DS_APPSRV_LOW_LATENCY_L2_REDUCE_AGGR_THR_IND_ID,
                   ds_low_latency_reduce_aggr_info_ind_s),

  /*DS APPSRV send*/
  MSGR_DEFINE_UMID(DS, APPSRV, IND, LLM_REDUCE_CDRX_CYCLE,
                   DS_APPSRV_LLM_REDUCE_CDRX_CYCLE_IND_ID,
                   ds_llm_reduce_cdrx_cycle_ind_s),
 
  /*DS APPSRV send*/
  MSGR_DEFINE_UMID(DS, APPSRV, IND, LLM_EXTEND_RRC_CONN,
                   DS_APPSRV_LLM_EXTEND_RRC_CONN_IND_ID,
                   ds_llm_extend_rrc_conn_ind_s),

  /*DS APPSRV send*/
  MSGR_DEFINE_UMID(DS, APPSRV, IND, LLM_CONTROL_SLEEP_MODE,
                   DS_APPSRV_LLM_CONTROL_SLEEP_MODE_IND_ID,
                   ds_llm_control_sleep_mode_ind_s),

  /*DS APPSRV send*/
  MSGR_DEFINE_UMID(DS, APPSRV, IND, LLM_SET_ARD_LLM,
                   DS_APPSRV_LLM_SET_ARD_LLM_IND_ID,
                   ds_llm_set_ard_llm_ind_s),

  /*sent from ML1, DS receive*/
  MSGR_DEFINE_UMID(DS, APPSRV, REQ, LLM_GET_ARD_LLM,
                   DS_APPSRV_LLM_GET_ARD_LLM_REQ_ID,
                   ds_llm_get_ard_llm_req_s),

  /*DS APPSRV send*/
  MSGR_DEFINE_UMID(DS, APPSRV, IND, LLM_SET_LEVEL,
                   DS_APPSRV_LLM_SET_LEVEL_IND_ID,
                   ds_llm_set_level_ind_s),

  /*DS APPSRV send*/
  MSGR_DEFINE_UMID(DS, APPSRV, IND, LLM_PDCP_DISCARD_TIMER,
                   DS_APPSRV_LLM_PDCP_DISCARD_TIMER_IND_ID,
                   ds_pdcp_discard_timer_ind_s),

  /*DS APPSRV send*/
  MSGR_DEFINE_UMID(DS, APPSRV, IND, LLM_SET_APS,
                   DS_APPSRV_LLM_SET_APS_IND_ID,
                   ds_llm_set_aps_ind_s),

  /*DS APPSRV send*/
  MSGR_DEFINE_UMID(DS, APPSRV, IND, LLM_SET_FAST_RRC_RELEASE,
                   DS_APPSRV_LLM_SET_FAST_RRC_RELEASE_IND_ID,
                   ds_llm_set_fast_rrc_release_ind_s)
};

#endif /* DS_LOWLATENCY_EXT_MSG_H */

