#ifndef RF_TASK_IND_UMID_H
#define RF_TASK_IND_UMID_H

/*!
   @file
   rf_task_ind_umid.h

   @brief
   RF Driver's task indication external interface file to export MSGR UMIDs.
*/

/*==============================================================================

  Copyright (c) 2012 Qualcomm Technologies Incorporated. All Rights Reserved

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

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/api/common/rf_task_ind_umid.h#2 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 

09/02/22   tej     FR69305 To add support for new error codes for QMI_DMS_SET_OPERATING_MODE
07/06/12   kb      Ported RCINIT changes from RFA3/main/1.0
05/25/12   kb/sar  Modified rf_init_complete_send_ind() and fixed CC Review defects. 
05/25/12   kb/sar  Removed featurization. 
05/25/12   kb/sar  Initial version
==============================================================================*/
#include "comdef.h"
#include "msgr.h"
#include "msgr_umid.h"
#include "rfa_msgr.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Enumeration of failure reason for RF initialization 
*/
typedef enum
{
  RF_INIT_FAILURE_INVALID,
  /**< Invalid RF failure - set this value to align with rfm_init_status_type_e_type */
  
  RF_INIT_FAILURE_NONE,
  /**< No RF failure - set when RF initialization status is successful */
    
  RF_INIT_FAILURE_HW,
  /**< RF hardware failure */
  
  RF_INIT_FAILURE_SW
  /**< RF software failure */  

}rf_init_failure_type_e_type;

/*! 
  @brief
  Struct to hold rfm_init_complete indication data.
 
  @details
  This struct holds the header and status for RFM Init Completion ind message.
  The rf_init_success variable provides the result of rfm_init where 1 = Success and
  0 = Failure.
 */
typedef struct {

  msgr_hdr_struct_type    hdr;

  /*!< Header for rfm_init_complete msg. */ 

  boolean                 rf_init_success;

  rf_init_failure_type_e_type    failure_type;
  /*!< failure type of the rf_init */

  /*!< result from the rf_init(). */ 

} rf_init_comp_msg_type;

/*-----------------------------------------------------------------------*/
/*     RF TASK Command UMID's                                             */
/*-----------------------------------------------------------------------*/

/*! @brief Enumeration of RF TASK Command UMID's
 */
enum
{
  /*! RF TASK Commands
  */

  RFA_RF_TASK_BASE_CMD = MSGR_UMID_BASE( MSGR_RFA_RF_CONTROL, MSGR_TYPE_CMD ),
  /*!< BASE RF TASK COMMAND UMID */

  MSGR_DEFINE_UMID( RFA, RF_CONTROL, CMD, INIT_COMPLETE, 0x0, uint32 ),
  /*!< RFA_RF_CONTROL_INIT_COMPLETE_CMD: Command sent from clients to get 
       rfm init status */
};

/*----------------------------------------------------------------------------*/
/*! @brief Enumeration of RF TASK Response UMIDs */
enum
{

  RFA_RF_TASK_BASE_RSP  = MSGR_UMID_BASE( MSGR_RFA_RF_CONTROL, MSGR_TYPE_RSP ),
  /*!< BASE RF TASK RESPONSE UMID */

  MSGR_DEFINE_UMID( RFA, RF_CONTROL, RSP, INIT_COMPLETE, 0x0, rf_init_comp_msg_type ),
  /*!< RFA_RF_CONTROL_INIT_COMPLETE_RSP: The response sent when RFA_RF_CONTROL_INIT_COMPLETE_CMD 
       is received by rf_task from a client */
};


#ifdef __cplusplus
}
#endif

#endif /* RF_TASK_IND_UMID_H */
