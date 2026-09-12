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
#include "mm_v.h"
#include "mm_ssgccs.h"
#include "mm_ssgccs_api.h"
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
#include "gmm_v.h"
#include "mmsecurity.h"
#include "gmm_msg.h"
#include "gmm_int_v.h"
#include <stringl/stringl.h>

ssgccs_3g_2g_mm_msg_handler mm_msg_handler = NULL;

/*========================================================================
  FUNCTION register_ssgccs_3g_2g_mm_msg_handler

  DESCRIPTION
    Register callback handler for MM info

  PARAMETERS
    msg_handler [in] - callback handler

  RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_2g_mm_msg_handler
(
  ssgccs_3g_2g_mm_msg_handler msg_handler
)
{
  
  mm_msg_handler = msg_handler;
} /* register_ssgccs_3g_2g_mm_msg_handler() */
#endif
/*========================================================================
  FUNCTION mm_ssgccs_handle_msg

  DESCRIPTION
    Process MM incoming/Outgoing message

  PARAMETERS
    incoming_msg [in] - MM incoming/Outgoing message

  RETURN VALUE
    None
===========================================================================*/
void mm_ssgccs_handle_icoming_msg
(
  uint8 message_id, sys_radio_access_tech_e_type rat_info , mm_3g_2g_domain_type domain,mm_cmd_type *incoming_msg
)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  ssgccs_3g_2g_mm_info_type mm_info;
  boolean                   plmn_id_is_undefined;
  boolean                   mnc_includes_pcs_digit;
  uint32                    mcc;
  uint32                    mnc;
  boolean					ssgccs_ind=TRUE;
  uint32					i;
    
  if (mm_msg_handler == NULL)
  {  	
	
    return;
  }
	memset(&mm_info, 0, sizeof(ssgccs_3g_2g_mm_info_type));  
	mm_info.active_rat=rat_info;
	if(rat_info==SYS_RAT_UMTS_RADIO_ACCESS)
 	{
	mm_info.global_cell_id=mm_serving_cell_id;
	}
	else
	{
	mm_info.global_cell_id=mm_coord_camped_cell_id;
	}
	sys_plmn_get_mcc_mnc(mm_serving_plmn.info.plmn,&plmn_id_is_undefined,&mnc_includes_pcs_digit,&mcc,&mnc);
	mm_info.mcc=mcc;
	mm_info.mnc=mnc;
	mm_info.lac[0]=mm_serving_plmn.lac.lac[0];
	mm_info.lac[1]=mm_serving_plmn.lac.lac[1];
	mm_info.as_id=(sys_modem_as_id_e_type)mm_as_id;



	if(domain == PS_DOMAIN_PROCEDURE)
	{
	switch (message_id) {
	
	case GMM_ATTACH_ACCEPT:
      mm_info.msg_id = GMM_ATTACH_ACCEPT_MSG;      
      break;

    case GMM_ATTACH_REJECT:
      mm_info.msg_id = GMM_ATTACH_REJECT_MSG;
	  mm_info.attach_reject_cause= incoming_msg->cmd.mm_data_ind.L3_data[2];      
      break;   

	  case GMM_ROUTING_AREA_UPDATE_REJECT:
	  mm_info.msg_id = GMM_ROUTING_AREA_UPDATE_REJECT_MSG;
	  mm_info.rau_reject_cause= incoming_msg->cmd.mm_data_ind.L3_data[2];	  
      break;

	  case GMM_ROUTING_AREA_UPDATE_ACCEPT:
	  mm_info.msg_id = GMM_ROUTING_AREA_UPDATE_ACCEPT_MSG;	  
      break;

	  case GMM_IDENTITY_REQUEST:
      mm_info.msg_id = GMM_IDENTITY_REQUEST_MSG;
	  mm_info.mobile_id= incoming_msg->cmd.mm_data_ind.L3_data[2] & 0x07;      
      break;  
	  

	 case GMM_AUTHENTICATION_AND_CIPHERING_REQUEST:
      mm_info.msg_id = GMM_AUTHENTICATION_AND_CIPHERING_REQUEST_MSG;
	  mm_info.rand_gmm = get_rand_gmm();	  
      break; 

	  case GMM_AUTHENTICATION_AND_CIPHERING_REJECT:   
	  mm_info.msg_id = GMM_AUTHENTICATION_AND_CIPHERING_REJECT_MSG;	  
      break; 

	  default:		
      
      ssgccs_ind=FALSE; 
      break;
		}
	}

	else if(domain == CS_DOMAIN_PROCEDURE)
	{
	switch (message_id) {

    case LOCATION_UPDATING_ACCEPT:
 	   mm_info.msg_id = MM_LOCATION_UPDATING_ACCEPT_MSG; 	   
 	   break;

	case LOCATION_UPDATING_REJECT:
      mm_info.msg_id = MM_LOCATION_UPDATING_REJECT_MSG;
	  mm_info.lau_reject_cause= incoming_msg->cmd.rr_data_ind.layer3_message[2];      
      break;	
	  
	case IDENTITY_REQUEST:
	  mm_info.msg_id = MM_IDENTITY_REQUEST_MSG;
	  mm_info.mobile_id=incoming_msg->cmd.rr_data_ind.layer3_message[2]& 0x07;	  
	  break;
		  
	case AUTHENTICATION_REQUEST:
      mm_info.msg_id = MM_AUTHENTICATION_REQUEST_MSG;
	  mm_info.rand_mm = get_rand_mm();	  
      break;

	  case AUTHENTICATION_REJECT:
      mm_info.msg_id = MM_AUTHENTICATION_REJECT_MSG;	  
      break;

	case MM_MODE_PREF_IND:
      mm_info.msg_id = MM_MODE_PREF_IND;
	  mm_info.mode_pref.num_items=incoming_msg->cmd.mmr_reg_req.rat_pri_list_info.num_items;
	  for (i = 0; i < mm_info.mode_pref.num_items; ++i)
      {
      mm_info.mode_pref.acq_sys_mode[i]=incoming_msg->cmd.mmr_reg_req.rat_pri_list_info.priority_list_info[i].acq_sys_mode;	  
	  }
      
      break;

    default:	  
      
	  ssgccs_ind=FALSE;
      break;
		}
  }
	if(ssgccs_ind==TRUE)
	{
	mm_msg_handler(&mm_info);
	}
#endif
} /* mm_ssgccs_handle_incoming_msg() */


void mm_ssgccs_handle_outgoing_msg
(
  uint8 message_id, uint8 id_type,sys_radio_access_tech_e_type rat_info
)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
ssgccs_3g_2g_mm_info_type mm_info;

boolean 				  plmn_id_is_undefined;
boolean 				  mnc_includes_pcs_digit;
uint32					  mcc;
uint32					  mnc;

 
  if (mm_msg_handler == NULL)
  {  
	
    return;
  }
  memset(&mm_info, 0, sizeof(ssgccs_3g_2g_mm_info_type));  
  mm_info.active_rat=rat_info;
    if(rat_info==SYS_RAT_UMTS_RADIO_ACCESS)
 	{
	mm_info.global_cell_id=mm_serving_cell_id;
	}
	else
	{
	mm_info.global_cell_id=mm_coord_camped_cell_id;
	}
	sys_plmn_get_mcc_mnc(mm_serving_plmn.info.plmn,&plmn_id_is_undefined,&mnc_includes_pcs_digit,&mcc,&mnc);
	mm_info.mcc=mcc;
	mm_info.mnc=mnc;
	mm_info.lac[0]=mm_serving_plmn.lac.lac[0];
	mm_info.lac[1]=mm_serving_plmn.lac.lac[1];
	mm_info.msg_id = message_id;
	mm_info.as_id=(sys_modem_as_id_e_type)mm_as_id;

	
  
  if(message_id==GMM_ATTACH_REQUEST_MSG)
  	{
  	  mm_info.ue_id=id_type;	  
	  
  	}

  
  if(message_id==GMM_IDENTITY_RESPONSE_MSG)
  	{
  	  mm_info.mobile_id=id_type;	 
  	}

  if(message_id==GMM_AUTHENTICATION_AND_CIPHERING_FAILURE_MSG)
  	{
  	  mm_info.authentication_failure_cause=id_type;	
  	}

  if(message_id==MM_LOCATION_UPDATING_REQUEST_MSG)
  	{
  	  mm_info.ue_id=id_type;
  	}

  if(message_id==MM_IDENTITY_RESPONSE_MSG)
  	{
  	  mm_info.mobile_id=id_type;
  	}

  if(message_id==MM_AUTHENTICATION_FAILURE_MSG)
  	{  	  
	  mm_info.authentication_failure_cause=id_type;	  
  	}
  if(message_id==MM_LOCATION_UPDATING_FAIL_IND)
  	{  	  
	  mm_info.ue_id=id_type;	  
  	}
  if(message_id==MM_ATTACH_REQ_FAIL_IND)
  	{  	  
	  mm_info.ue_id=id_type;	  
  	}
  if(message_id==MM_ROUTING_AREA_UPDATE_FAIL_IND)
  	{  	  
	  mm_info.ue_id=id_type;	  
  	}

  mm_msg_handler(&mm_info);
#endif
}
