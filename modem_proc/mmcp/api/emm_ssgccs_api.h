/*==============================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated. All Rights Reserved

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

==============================================================================*/

#ifndef EMM_SSGCCS_API_H
#define EMM_SSGCCS_API_H

#include "lte_nas_common.h"
#include "emm_ext_msg.h"
/* EMM info */
typedef struct
{
  lte_nas_message_id msg_id;
  uint8 attach_reject_cause;
  uint8 tau_reject_cause;
  uint8 service_reject;
  uint8 cipher_algo;
  uint8 mobile_id;
  uint8 ue_id;
  uint8 authentication_failure_cause;
  uint32 global_cell_id;  // this is global cell id in SIB2 which is 28bits long, ssg has to mask the 28 bits 
  uint32 mcc;
  uint32 mnc;
  lte_nas_emm_tai_lst_info_type        tai_lst;
  sys_modem_as_id_e_type  as_id;
  boolean call_status_info_present;
  uint8 csfb_call_status;
  lte_nas_security_hdr_type sec_header;
} ssgccs_4g_emm_info_type;

typedef void (*ssgccs_4g_emm_msg_handler) (ssgccs_4g_emm_info_type*);

/*========================================================================
  FUNCTION register_ssgccs_4g_emm_msg_handler

  DESCRIPTION
    Register callback handler for EMM info

  PARAMETERS
    msg_handler [in] - callback handler

  RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_4g_emm_msg_handler
(
  ssgccs_4g_emm_msg_handler msg_handler
);

#endif