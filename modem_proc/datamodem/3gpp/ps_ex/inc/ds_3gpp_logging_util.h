#ifndef DS_3GPP_BEARER_LOGGING_H
#define DS_3GPP_BEARER_LOGGING_H
/*!
  @file
  ds_3gpp_logging_util.h

  @brief
  Logging utility functions and routines

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2017 Qualcomm Technologies Incorporated. All Rights Reserved

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

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/20/17   sxi      Initial version.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"

#include "ds_3gpp_bearer_context.h"
#include "log.h"

#define DS_BEARER_LOGGING_VERSION  0
#define LOG_DS_BEARER_INFO_C_type  ds_3gpp_bearer_log_info_table

typedef PACK(struct)
{  
  uint8                      sub_id;           ///<  subscription id 1 based
  uint8                      bearer_id;        ///<  To indicate  Eps Bearer id/Nsapi
  ds_bearer_context_state_e  state;            ///<  Bearer state 
  uint8                      cm_call_id;       ///<  CM call id
  uint8                      call_mode;        ///<  CM call mode
  uint8                      pdn_context_inst; ///<  Indicate pdn context instance associated
  uint8                      is_default;       ///<  1 indicating default bearer; 0 indicating dedicated
  uint8                      profile_id;       ///<  Profile Id 
  uint8                      v4_instance_id;   ///<  v4 Iface instance Id; 0xFF if the iface ptr is NULL
  uint8                      v6_instance_id;   ///<  v6 Iface instance Id; 0xFF if the iface ptr is NULL
  uint32                     ul_low_wm_level;  ///<  wm low level
  uint32                     ul_high_wm_level; ///<  wm high level
  uint32                     ul_dne_wm_level;  ///<  wm dne level
  uint32                     ul_packet_drop;   ///<  wm packet drop
}ds_3gpp_bearer_log_info; 

typedef PACK(struct)
{  
  log_hdr_type               header;
    /*Log packet version. Range: 0...255.*/
  uint32                     version;          
    /*Number of bearers */
  uint8                      num_bearers;
    /*bearer array */
  ds_3gpp_bearer_log_info    bearer_info[DS_3GPP_MAX_BEARER_CONTEXT];
} ds_3gpp_bearer_log_info_table;

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION      DS_3GPP_LOGGING_BEARER_STATE

DESCRIPTION   this function is logging the bearer info

DEPENDENCIES  None 
 
PARAMETERS 
  num_bearers - number of activate bearer(s)
  bearer_info - pointer to the bearer info
  subs_id - subscription ID
   
RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_3gpp_logging_bearer_state
(                                                                             
  uint8                    num_bearers,
  ds_3gpp_bearer_log_info  *bearer_info,
  sys_modem_as_id_e_type   subs_id
);

#endif /* DS_3GPP_BEARER_LOGGING_H */
