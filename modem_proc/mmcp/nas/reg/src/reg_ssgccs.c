/*===========================================================================

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

===========================================================================*/

#include <msgr.h>
#include "reg_ssgccs.h"
#include "reg_ssgccs_api.h"
#include "reg_state.h"
#include "nasutils.h"
#include "stringl/stringl.h"

#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
ssgccs_reg_msg_handler reg_msg_handler = NULL;
#endif

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
)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  
  reg_msg_handler = msg_handler;
#endif
} /* register_ssgccs_reg_msg_handler() */

/*========================================================================
  FUNCTION reg_ssgccs_send_ehplmn_and_fplmn

  DESCRIPTION
    Send ehplmn and fplmn information

  PARAMETERS
    None

  RETURN VALUE
    None
===========================================================================*/
void reg_ssgccs_send_ehplmn_and_fplmn
(
  void 
)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  ssgccs_reg_info_type reg_info;


  if (reg_msg_handler == NULL)
  {
	
    return;
  }
  memset(&reg_info, 0, sizeof(ssgccs_reg_info_type)); 
  reg_info.is_ehplmn_provided = TRUE;
  reg_info.is_fplmn_provided = TRUE;
  reg_info.as_id = reg_sub_id;
#ifdef FEATURE_EQUIVALENT_HPLMN
   reg_info.ehplmn_content = reg_sim_ehplmn_list_get();
#endif
  #ifdef FEATURE_DUAL_SIM
  (void)reg_sim_read_fplmn_list(&reg_info.fplmn_content,FALSE, (sys_modem_as_id_e_type)reg_sub_id );
  reg_info.as_id = reg_sub_id;
#else
  (void)reg_sim_read_fplmn_list(&reg_info.fplmn_content, FALSE);
#endif	
  reg_msg_handler(&reg_info);
#endif
} /* reg_ssgccs_send_ehplmn_and_fplmn() */

/*========================================================================
  FUNCTION reg_ssgccs_send_camp_status

  DESCRIPTION
    send camp status to SSG module

  PARAMETERS
    roaming_ind [in] - camp status out

  RETURN VALUE
    None
===========================================================================*/
void reg_ssgccs_send_camp_status
(
  sys_roam_status_e_type roaming_ind,
  sys_plmn_id_s_type     plmn
)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  ssgccs_reg_info_type reg_info;
  boolean  plmn_id_is_undefined;
  boolean  mnc_includes_pcs_digit;

   if (reg_msg_handler == NULL)
  {
	
    return;
  }

  memset(&reg_info, 0, sizeof(ssgccs_reg_info_type));  

  sys_plmn_get_mcc_mnc(plmn,
                       &plmn_id_is_undefined,
                       &mnc_includes_pcs_digit,
                       &reg_info.mcc,
                       &reg_info.mnc);
  
  reg_info.is_ehplmn_provided = FALSE;
  reg_info.is_fplmn_provided = FALSE;
  reg_info.is_camping_information_provided = TRUE;
  reg_info.roaming_ind = roaming_ind;
  reg_info.as_id = reg_sub_id;
  reg_msg_handler(&reg_info);
#endif
} /* reg_ssgccs_send_camp_status() */