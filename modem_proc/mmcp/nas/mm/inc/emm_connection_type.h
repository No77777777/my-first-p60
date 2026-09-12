

/*!
  @file
  emm_connection_handler.h

  @brief
  EMM internal data structure definitions.

*/

/*===========================================================================

  Copyright (c) 2008 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/mmcp.mpss/7.9.0/nas/mm/inc/emm_connection_type.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/02/10   zren    Added failure handling for RRC connection release causes of
                   CRE_FAILURE and OOS_DURING_CRE 
10/06/09   RI      Fix new line warning at EOF.
08/14/09   zren    Modified procedure connection release handling
05/19/09   hnam    Added TRANSMISSION_LTE_RRC_UL_DATA_CNF_FAILURE_CTRL_NOT_CONN 
                   in lte_nas_emm_connection_cause_type
04/17/09   zren    Modified to support saving NAS messages in pended message 
                   list in the format of lte_nas_outgoing_msg_type
                   Added one parameter to function emm_send_rrc_connection_establishment_request() 
                   Sepearted RRC_DATA_CNF and RRC_EST_CNF for RRC_EST_REQ
                   Modified EMM connection state machines
===========================================================================*/

#ifndef _EMM_CONNECTION_TYPE_H_
#define _EMM_CONNECTION_TYPE_H_

#include "mmcp_variation.h"
#include<customer.h>
#ifdef FEATURE_LTE

#include "comdef.h"
#include "lte_rrc_ext_msg.h"

/*===========================================================================

                        MACROS DEFINITION

===========================================================================*/

/*===========================================================================

                        TYPES DEFINITION

===========================================================================*/


/*===========================================================
  Note -emm_connection_state_type - update debug enum "emm_connection_state_debug_type "
  when this is updated
============================================================*/
typedef enum
{
  EMM_IDLE_STATE,
  EMM_WAITING_FOR_RRC_CONFIRMATION_STATE,
  EMM_CONNECTED_STATE,
  EMM_RELEASING_RRC_CONNECTION_STATE
} emm_connection_state_type ;

/*===========================================================================

                        EXTERNAL DEFINITION

===========================================================================*/


/*===========================================================================

                        MACROS DEFINITION

===========================================================================*/

#endif /* _EMM_CONNECTION_TYPE_H_ */


#endif /*FEATURE_LTE*/

