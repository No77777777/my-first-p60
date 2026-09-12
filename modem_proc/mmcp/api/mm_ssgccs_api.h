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

#ifndef MM_SSGCCS_API_H
#define MM_SSGCCS_API_H

#include "sys.h"

typedef enum
{
  MM_LOCATION_UPDATING_REJECT_MSG= 0, 
  MM_LOCATION_UPDATING_REQUEST_MSG,
  MM_AUTHENTICATION_REJECT_MSG,        			
  MM_AUTHENTICATION_REQUEST_MSG,       			
  MM_AUTHENTICATION_RESPONSE_MSG,      			
  MM_IDENTITY_REQUEST_MSG,
  MM_IDENTITY_RESPONSE_MSG,
  MM_AUTHENTICATION_FAILURE_MSG,

  ROUTING_AREA_UPDATE_REJECT_MSG,
  
  GMM_ATTACH_REQUEST_MSG,
  GMM_ATTACH_REJECT_MSG, 
  GMM_ROUTING_AREA_UPDATE_REJECT_MSG,
  GMM_ROUTING_AREA_UPDATE_REQUEST_MSG,
  GMM_AUTHENTICATION_AND_CIPHERING_REQUEST_MSG,
  GMM_AUTHENTICATION_AND_CIPHERING_RESPONSE_MSG,
  GMM_AUTHENTICATION_AND_CIPHERING_REJECT_MSG,
  GMM_AUTHENTICATION_AND_CIPHERING_FAILURE_MSG,
  
  GMM_IDENTITY_REQUEST_MSG,
  GMM_IDENTITY_RESPONSE_MSG,
  GMM_ATTACH_ACCEPT_MSG,
  GMM_ATTACH_COMPLETE_MSG, 
  GMM_ROUTING_AREA_UPDATE_ACCEPT_MSG,
  GMM_ROUTING_AREA_UPDATE_COMPLETE_MSG,
  MM_LOCATION_UPDATING_ACCEPT_MSG,
  MM_LOCATION_UPDATING_FAIL_IND,
  MM_ATTACH_REQ_FAIL_IND,
  MM_ROUTING_AREA_UPDATE_FAIL_IND,
  MM_MODE_PREF_IND
}mm_3g_2g_message_id_type;

typedef struct ssg_auth_value
{
  byte value_length ;
  byte value_data[16] ;
} ssg_auth_value_type ;

typedef ssg_auth_value_type ssg_mm_rand_type ;

typedef struct 
{
   uint32 num_items;
   sys_sys_mode_e_type   acq_sys_mode[10];
}ssg_rat_list;

/* EMM info */
typedef struct
{
  mm_3g_2g_message_id_type msg_id;
  sys_radio_access_tech_e_type active_rat;//Flag to give whether informaiton is for GSM or UMTS (0-GSM, 1-UMTS)
  sys_modem_as_id_e_type  as_id;
  uint8 attach_reject_cause;
  uint8 lau_reject_cause;
  uint8 rau_reject_cause;
  uint8 authentication_failure_cause;//Failure cause in the authentication failure message sent from UE to network
  uint8 mobile_id;//Sent in Identity request
  uint8 ue_id; //Sent in attach,LAU,RAU  
  uint32 global_cell_id;	   
  uint32 mcc;
  uint32 mnc;
  byte lac[2];
  ssg_mm_rand_type rand_mm;
  ssg_mm_rand_type rand_gmm;
  ssg_rat_list mode_pref;
} ssgccs_3g_2g_mm_info_type;
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
typedef void (*ssgccs_3g_2g_mm_msg_handler) (ssgccs_3g_2g_mm_info_type*);

/*========================================================================
  FUNCTION register_ssgccs_3g_2g_mm_msg_handler

  DESCRIPTION
    Register callback handler for MM/GMM info

  PARAMETERS
    msg_handler [in] - callback handler

  RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_2g_mm_msg_handler
(
  ssgccs_3g_2g_mm_msg_handler msg_handler
);
#endif
#endif
