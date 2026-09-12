/*===========================================================================

  Copyright (c) 2010 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/mmcp.mpss/7.9.0/api/sm_qsh_ext.h#1 $
===========================================================================*/
#ifndef _SM_QSH_EXT_H_
#define _SM_QSH_EXT_H_

#include <customer.h>
#include "qsh.h"
#include "comdef.h"
#include "lte.h"
#include "sm.h"
#include "sm_cm.h"
#include "rabm_gmm.h"
#include "msgr.h"
#include "lte_nas_common.h"


/*===========================================================================

                        MACROS DEFINITION

===========================================================================*/
#define SM_DEBUG_DUMP_BUFFER_SIZE      40

#define SM_QSH_MAJOR_VER 1
#define SM_QSH_MINOR_VER 1


typedef enum
{
  ESM_BC_ON_ATTACH_DEF_BEARER_PENDING_DEBUG,
  ESM_BC_ON_ATTACH_DEF_BEARER_FAILED_DEBUG,
  ESM_BC_ON_ATTACH_DEF_BEARER_SUCCESS_DEBUG,
  ESM_BC_ON_ATTACH_DEF_BEARER_NONE_DEBUG
} esm_bc_on_attach_def_bearer_state_debug_T;

typedef enum
{
  TO_SM_DEBUG,
  FROM_SM_DEBUG,
  TO_ESM_DEBUG,
  FROM_ESM_DEBUG,
  TO_RABM_DEBUG
}sm_esm_message_direction_debug_type_T;

typedef enum 
{
  LTE_NAS_EMC_SRV_NONE_SM_DEBUG,
  LTE_NAS_EMC_SRV_PENDING_SM_DEBUG,
  LTE_NAS_EMC_SRV_ONLY_SM_DEBUG,
  LTE_NAS_EMC_SRV_EXIST_SM_DEBUG

}lte_nas_emc_srv_status_sm_debug_type;

typedef enum
{
  PROTOCOL_DISCRMINATOR_ESM_DEBUG = 2,
  PROTOCOL_DISCRMINATOR_SM_DEBUG = 10
}sm_protocol_discriminator_debug_type;





/*===========================================================================

                        TYPES DEFINITION

===========================================================================*/

/******************************************************************************
**                     ESM DEBUG DATA
******************************************************************************/
typedef struct esm_debug_data_s
{
  esm_bc_on_attach_def_bearer_state_debug_T on_attach_def_bearer_pending;
  boolean                                   isr_status;
  boolean                                   first_def_bearer_ctxt_received;
  byte                                      cur_pti_index;
  lte_nas_emc_srv_status_sm_debug_type         emc_srv_status;
  lte_3gpp_release_ver_e                    rel_ver;
} esm_debug_data_s_type;



/******************************************************************************
**                     SM DEBUG DATA
******************************************************************************/

typedef struct sm_debug_data_s
{
  sm_deact_isr_on_interrat_status_T           sm_deact_isr_on_interrat_status_debug;
  boolean                                     is_multi_pdn_same_apn_debug;
  uint8                                       is_nw_initiated_qos_support_debug;
  word                                        timers_pending_debug[1]; 
  boolean                                     sm_ps_vote_for_reselection_debug;
  byte                                        ps_call_info_ti_debug;
  boolean                                     sig_connection_available_debug;
  boolean                                     ps_signalling_conn_state_debug;
  boolean                                     sm_is_pdp_retranmsit_req_debug;
  boolean                                     cm_initiated_ps_signalling_release_debug;
  smreg_ps_signalling_rel_req_cause_T         cm_initiated_ps_signalling_release_cause_debug;
  boolean                                     timer_est_pen_flag_debug;
  boolean                                     sm_abort_cnf_pending_debug;
  uint32                                      sm_tlli_debug;
  //byte                                        value_debug[GMM_PDP_CONTEXT_STATUS_LENGTH]; 
  sys_modem_as_id_e_type                      sm_as_id_debug;
}sm_debug_data_s_type;


/******************************************************************************
**                     SM ESM DEBUG BUFFER
******************************************************************************/
typedef sm_esm_debug_buffer_s_type sm_qsh_debug_buffer_s_type;



/******************************************************************************
**                     SM Metric Signaling sub structure
******************************************************************************/
typedef struct 
{
  sm_protocol_discriminator_debug_type  protocol_discriminator;

  union
  {
    lte_nas_message_id   esm_message_id;
    sm_layer_msg_T       sm_message_id;
  } message_type;

  // 0 (NW to UE), 1 (UE to NW)
  byte   direction; 

  //ESM or SM cause as per 3GPP spec
  union
  {
    esm_cause_enum_T   esm_cause;
    sm_status_T        sm_cause;
  } cause_value;

  byte   as_id; 
}sm_qsh_signaling_s;


/*===========================================================================

                        QSH MACRO AND STRUCTURE TYPE DEFINITIONS

===========================================================================*/

/******************************************************************************
**  QSH DUMP
**    1. Dump set for DEBUG_DATA
**    3. Dump set for DEBUG BUFFER
******************************************************************************/
typedef enum
{
  SM_QSH_DUMP_TAG_DEBUG_DATA = 0,
  SM_QSH_DUMP_TAG_DEBUG_BUFFER = 1,
  SM_QSH_DUMP_TAG_MAX = 31
}sm_qsh_dump_tag_e;

/******************************************************************************
**  METRICS
**    1. OTA Messages 
******************************************************************************/
typedef enum
{
  SM_QSH_METRIC_SIGNALING = 0,
  SM_QSH_METRIC_MAX
}sm_qsh_metric_e;


/******************************************************************************
**      SM QSH DATA DUMP TYPE.
******************************************************************************/
typedef struct sm_qsh_dump_data_s
{
  qsh_dump_tag_hdr_s hdr; /* this header will have a tag and length */
  esm_debug_data_s_type                 esm_debug_data;
  sm_debug_data_s_type                  sm_debug_data;
}sm_qsh_dump_tag_debug_data_s;


/******************************************************************************
**      SM QSH DEBUG BUFFER DUMP TYPE.(Applicable for
**                           SM_QSH_DUMP_TAG_DEBUG_BUFFER  tag)
******************************************************************************/
typedef struct sm_qsh_dump_debug_buffer_s
{
  qsh_dump_tag_hdr_s hdr; /* this header will have a tag and length */
  sm_qsh_debug_buffer_s_type debug_buffer[SM_DEBUG_DUMP_BUFFER_SIZE];
}sm_qsh_dump_tag_debug_buffer_s;

/******************************************************************************
**      SM QSH METRICS TYPE.for DEBUG SIGNALING(Applicable for
**                           SM_QSH_METRIC_SIGNALING metrics tag)
******************************************************************************/
typedef struct 
{
  qsh_metric_hdr_s hdr; /* This header has the timestamp (uint16) */
  sm_qsh_signaling_s metric_sig;
}sm_qsh_metric_signaling_s;


#endif /* _SM_QSH_EXT_H_ */



