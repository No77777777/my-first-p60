/*!
  @file
  a2_power.h

  @brief
  The interface to the A2 power collapse feature implementation file

  @author
  araina
*/

/*==============================================================================

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

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/utils.mpss/5.4/api/a2/a2_power.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
08/01/17   am      CR1089189 L plus L changes
01/13/14   ca      DSDA feature is implemented.
11/21/13   ca      Added a2_poweri_request() call back, register
                   and deregister functions.
10/09/13   ar      move internal api prototypes to src folder header files
09/30/13   mm      Removed modem freeze changes
09/11/13   ca      Removed boot up temp client 
08/06/13   yjz     Add A2_POWER_REQ_VOLTE_SLEEP for modem freeze
02/06/12   yjz     Fix Mob link error
10/19/12   ar      added A2_POWER_CLIENT_A2_IP_FILTER client
10/12/12   ar      added A2_POWER_REQ_SMSM_NAK smsm request
09/14/12   ar      added a2_power_send_ack_and_pc_notification_to_apps() api 
04/16/12   ar      added a2 pc mutex operation related apis
02/13/12   ar      added support for A2 PC + SPS RESET feature
01/06/12   ar      added a2_power_collapse_disabled() api 
                   split the A2_POWER_CLIENT_INTERNAL client into PHY and PER
11/22/11   ar      added support for UL PER client for a2 pc
11/10/11   st      Added A2 power collapse support for the WCDMA RRC module
05/24/09   ar      initial version
==============================================================================*/

#ifndef A2_POWER_H
#define A2_POWER_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <comdef.h>
#include <a2_common.h>

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/
  /*!
     @brief
     Clients voting for A2 wake up / shut down
  */
  typedef enum
  {
    A2_POWER_CLIENT_INTERNAL_PHY   = 0,
    A2_POWER_CLIENT_LTE_ML1_UL,
    A2_POWER_CLIENT_LTE_ML1_UL_1,
    A2_POWER_CLIENT_LTE_ML1_DL,
    A2_POWER_CLIENT_LTE_ML1_DL_1,
    A2_POWER_CLIENT_WL1_EUL,
    A2_POWER_CLIENT_WL1_HS,
    A2_POWER_CLIENT_TDSCDMA_EUL,
    A2_POWER_CLIENT_TDSCDMA_HS,                                    
    A2_POWER_CLIENT_MAX
  } a2_power_client_e;

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA chanegs,
  this macro can take an extra argument as sub_id.
  */
#define a2_power_get_hw_state_per_client(client)  \
  a2_poweri_get_hw_state_per_client(A2_SUB_ID_DEFAULT, client)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA chanegs,
  this macro can take an extra argument as sub_id.
  */
#define a2_power_register_req_proc_cb(client_id, cb, user_data)  \
  a2_poweri_register_req_proc_cb(A2_SUB_ID_DEFAULT, client_id, cb, user_data)

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA chanegs,
  this macro can take an extra argument as sub_id.
  */
#define a2_power_deregister_req_proc_cb(client_id)  \
  a2_poweri_deregister_req_proc_cb(A2_SUB_ID_DEFAULT, client_id)
  

/*!
   @brief
   enum for a2 power request - shut down, wake up or apps smsm
*/
typedef enum
{
  A2_POWER_REQ_SHUT_DOWN            = 0,
  A2_POWER_REQ_WAKE_UP              = 1,
  A2_POWER_REQ_MAX
} a2_power_req_e;

/*!
   @brief
   enum for a2 hw power states
*/
typedef enum
{
  A2_POWER_HW_STATE_OFF                 = 0,
  A2_POWER_HW_STATE_ON                  = 1,
  A2_POWER_HW_STATE_MAX
} a2_power_hw_state_e;

/*!
   @brief
   A2_Power_process_req() call back function prototype
*/
typedef void (*a2_power_req_proc_cb)
(
  uint32 user_data, /*!< clients can regester a call back with required user data to be sent back*/
  a2_power_client_e   client_id, /*!< client id corresponding to the A2_power_process() call back*/
  a2_power_req_e      curr_req,  /*!< The power request that is being processed. */
  a2_timetick_t       time_stamp, /*!< Time at which the call back is called*/
  a2_power_hw_state_e hw_state /*!< hw_state of a2 */
);

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

/*==============================================================================

  FUNCTION:  a2_poweri_request

==============================================================================*/
/*!
    @brief
    API for a2 power clients to request for a2 services
 
    @detail
    The following is the example to use demback_bf by other modules:
    demback_ bf should be used as bit fields.
    
    LTE DL cliet               :  demback_bf value should be 
                                 (A2_DEMBACK_ID_BMSK(A2_DEMBACK_ID_0)
                               | A2_DEMBACK_ID_BMSK(A2_DEMBACK_ID_1))
                                  (to use both dembacks),
                                  A2_DEMBACK_ID_BMSK(A2_DEMBACK_ID_0)
                               or A2_DEMBACK_ID_BMSK(A2_DEMBACK_ID_1)
                                  to use either demback 0  or demback 1 
                                  respectively
    HSPA/TDCDMA DL clients     :  demback_bf value should be either 
                                A2_DEMBACK_ID_BMSK(A2_DEMBACK_ID_0) 
                               or A2_DEMBACK_ID_BMSK(A2_DEMBACK_ID_1)
                                  to use either demback 0  or demback 1
    LTE/HSPA/TDCDMA UL clients :  demback_bf value should be 
                                  A2_DEMBACK_ID_BMSK(A2_DEMBACK_ID_INVALID).
 
    @return
    none
*/
/*============================================================================*/
extern void a2_poweri_request
(
  a2_sub_id_t               sub_id, /*!< System subscriber id */
  a2_power_client_e         client, /*!< client making the request */
  a2_power_req_e            req,     /*!< type of request */
  a2_demback_bf_t           demback_bf /*!< dem back bf */
);
/* a2_poweri_request() */

/*==============================================================================

  FUNCTION:  a2_power_get_hw_state

==============================================================================*/
/*!
    @brief
    returns the power status of a2 hw

    @return
    a2_power_hw_state_e - a2 hw power state
*/
/*============================================================================*/
extern a2_power_hw_state_e a2_power_get_hw_state
(
  void
);
/* a2_power_get_hw_state() */

/*==============================================================================

  FUNCTION:  a2_poweri_get_hw_state_per_client

==============================================================================*/
/*!
    @brief
    returns the power status of a2 hw per client

    @return
    a2_power_hw_state_e - a2 hw power state
*/
/*============================================================================*/
a2_power_hw_state_e a2_poweri_get_hw_state_per_client
(
  a2_sub_id_t       sub_id,   /*!< System Subscriber ID */
  a2_power_client_e client /*!< client */
);
/* a2_poweri_get_hw_state_per_client() */


/*===========================================================================

  FUNCTION:  a2_poweri_register_req_proc_cb

===========================================================================*/
/*!
  @brief
  Registers a callback which is called upon up on a2 power process request 
  from clients.
  This function should be called by the clients before a2 power process 
  req is made.
  If clients regeters the cb function, then cb is called after processing 
  a2 power process
  request as an ack.

*/
/*=========================================================================*/
void a2_poweri_register_req_proc_cb
(
  a2_sub_id_t            sub_id,    /*!< system subscriber id */
  a2_power_client_e      client_id, /*!< client id */
  a2_power_req_proc_cb   cb, /*!< the cb to be regestered */ 
  uint32                 user_data   /*!< user data to be sent to the clients 
                                          when cb is called*/

);
/*===========================================================================

  FUNCTION:  a2_poweri_deregister_req_proc_cb

===========================================================================*/
/*!
  @brief
  This function Deregisters a callback which is registered as part of 
  a2_poweri_register_req_proc_cb() by clients.
*/
/*=========================================================================*/
void a2_poweri_deregister_req_proc_cb
(
  a2_sub_id_t            sub_id,    /*!< system subscriber id */
  a2_power_client_e      client_id  /*!< client id */
);

/*!
  This Macro is exposed to other modules. This is used to support the 
  legacy code of other modules. Once other modules implement DSDA chanegs,
  this macro can take an extra argument as sub_id.
  */
INLINE void a2_power_request
(
  a2_power_client_e     client, /*!< client making the request */
  a2_power_req_e        req    /*!< type of request */
)
{
  if(client == A2_POWER_CLIENT_LTE_ML1_DL || client == A2_POWER_CLIENT_LTE_ML1_DL_1)                                                
  {                                                                                       
    a2_poweri_request(A2_SUB_ID_DEFAULT, client, req, A2_DEMBACK_BF_DEFAULT_CAT6);        
  }                                                                                       
  else if(client == A2_POWER_CLIENT_WL1_HS || client == A2_POWER_CLIENT_TDSCDMA_HS)       
  {                                                                                       
    a2_poweri_request(A2_SUB_ID_DEFAULT, client, req, A2_DEMBACK_BF_DEFAULT_DL_PHY_HSPA);  
  }                                                                                       
  else                                                                                    
  {                                                                                       
    a2_poweri_request(A2_SUB_ID_DEFAULT, client, req, A2_DEMBACK_BF_DEFAULT_UL_PHY_SEC);
  }                                                                                      
}
#endif /* A2_POWER_H */

