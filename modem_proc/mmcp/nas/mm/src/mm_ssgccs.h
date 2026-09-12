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


#ifndef MM_SSGCCS_H
#define MM_SSGCCS_H
#define NO_PAYLOAD 0xFF

typedef enum
{
  CS_DOMAIN_PROCEDURE= 0, 
  PS_DOMAIN_PROCEDURE
}mm_3g_2g_domain_type;

/*========================================================================
  FUNCTION mm_ssgccs_handle_msg

  DESCRIPTION
    Process MM incoming and outgoing message
  PARAMETERS
    incoming_msg [in] - MM incoming and outgoing message

  RETURN VALUE
    None
===========================================================================*/


extern void mm_ssgccs_handle_icoming_msg
(
  uint8 message_id, sys_radio_access_tech_e_type rat_info , mm_3g_2g_domain_type domain,mm_cmd_type *incoming_msg
);

extern void mm_ssgccs_handle_outgoing_msg
(
  uint8 message_id, uint8 id_type,sys_radio_access_tech_e_type rat_info
);


#endif /* MM_SSGCCS_H */