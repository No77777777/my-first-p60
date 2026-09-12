/*!
  @file
  notif_mgr.h

  @brief
  Declares the notif manager module interface

*/
/*===========================================================================

  Copyright (c) 2017 - 2018 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/src/notif_mgr.h#1 $
==============================================================================*/

#ifndef NOTIF_MGR_H
#define NOTIF_MGR_H

#include "global_defs.h"
#include "ipa_hw_interface.h"
#include "iu_core.h"
/**
 * @brief   The size of notification queue.
*/
#define NOTIF_QUEUE_DEPTH 10

   // "Client" is the destined target of the notification. If the same
   // notification type could be processed by more than one module, 
   // then context is used by notif mgr to know which module processes
   // the notification

/**
 * @brief   "Client" is the destined target of the notification. If the same
            notification type could be processed by more than one module, 
            then "client" is used by notif mgr to know which module processes
            the notification
*/
typedef enum notifClient 
{
   IMPLICIT_CLNT = 0, //i.e Notif Mgr implicitly knows which module needs to processes this notif
#ifdef FEATURE_UT
   UT_QMB_OP,
   UT_TX_SM,//9
   UT_RX_SM, //10
#endif
}notifClient;

typedef enum notifType 
{  
  UNDEFINED = 0,
  QMB_DONE,
  CPU_COMM,//4
  CLK_STATE_CHG,
  POWER_COLLAPSE,
  RPM_CLK_REQ,
  GSI_INT,//8
  GSI_COMP_NOTIF,
  GSI_INT_NOTIF,
   MBOX_0_DB,
#ifdef FEATURE_UT
   MBOX_1_DB,
   MBOX_2_DB,
   MBOX_3_DB,
#endif
}notifType;

typedef struct notif_t 
{
  notifClient client;
  notifType type;
  uint32_t id;
  uint32_t payload;
} notif_t;

//extern uint32 iu_core_wait(volatile uint8 *addr, uint32 val);

/**
 * @brief   initialize notification manager.
*/
void NOTIF_Init(void);

/**
 * @brief   Push single notification to head of queue
 *
 * @param   newNotif          The new notification. 
 * @param   blockInterrupts   TRUE to block interrupts when pushing notification, FALSE - no need to disable interrupt (notification is sent from interrupt context)
*/
void NOTIF_Push(const notif_t *newNotif, uint32_t blockInterrupts);

/**
 * @brief   Puts the core in Wait For Interrupt state until a 
 *          interrupt is triggered and pushes a notification to
 *          the fifo
 *  
*/
void NOTIF_Wait();

/**
 * @brief   Pop and process notifications from queue. 
 *
 * @note function returns once notification queue is empty
*/
void NOTIF_Process(void); 

/**
 * @brief   Perform system error sequence. 
 *
 * @param   errorType      The error identifier. 
 * @param   errorAddress   The error address. In case no address is relevant to error user shall specify the value IPA_HW_FIRST_ERROR_ADDRESS_RESET_VALUE
 * @param   isReadError    1 - error is related to read operation, 0 - error is related to write operation. In case no address is relevant to error user shall specify the value 0
*/
void NOTIF_HandleSystemError(uint8_t errorType, uint32_t errorAddress, uint32_t isReadError);

/**
 * @brief   IRQ Handler for Hard Fault
*/
void HardFault_Handler(void) __irq;

void DEBUG_SW_BREAK(void);

void assert_ex(uint8_t expr);

#endif /* NOTIF_MGR_H */
