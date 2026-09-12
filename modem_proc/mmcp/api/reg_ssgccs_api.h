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

#ifndef REG_SSGCCS_API_H
#define REG_SSGCCS_API_H

#include "sys_v.h"
#include "reg_sim.h"

/* EMM info */
typedef struct
{
  boolean is_ehplmn_provided;
  boolean is_fplmn_provided;
  boolean is_camping_information_provided;
  reg_sim_plmn_list_s_type ehplmn_content;
  reg_sim_plmn_list_s_type fplmn_content;
  sys_roam_status_e_type roaming_ind;
  sys_mcc_type mcc;
  sys_mnc_type mnc;
  sys_modem_as_id_e_type as_id;
} ssgccs_reg_info_type;

typedef void (*ssgccs_reg_msg_handler) (ssgccs_reg_info_type*);

/*========================================================================
  FUNCTION register_ssgccs_reg_msg_handler

  DESCRIPTION
    Register callback handler for REG info

  PARAMETERS
    msg_handler [in] - callback handler

  RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_reg_msg_handler
(
  ssgccs_reg_msg_handler msg_handler
);

#endif
