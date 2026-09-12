/*!
  @file
  interrupts.h

  @brief
  Declares the interrupt driver interface

*/
/*===========================================================================

  Copyright (c) 2017 QUALCOMM Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/src/interrupts.h#1 $
==============================================================================*/
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "comdef.h"
#include "iu_core.h"


/**
 * @brief   Restore interrupts state. 
*/
#define NVIC_EnableInterrupts() \
    do { \
       __asm__ volatile ("ei"); \
    } while ((void)0,0)

/**
 * @brief   Disable all interrupts. 
*/
#define NVIC_DisableInterrupts() \
   do { \
   __asm__ volatile ("di"); \
   } while ((void)0,0)

/**
 * @brief   Wait for interrupts. 
*/
#define NVIC_waitForInterrupts() \
   do { \
   __asm__ volatile ("wait"); \
   } while ((void)0,0)

/**
 * @brief   Enable specific irq. 
 *
 * @param   irq   The irq id. 
*/
void NVIC_EnableIrq(uint8 irqn);

/**
 * @brief   Disable specific irq. 
 *
 * @param   irq   The irq id. 
*/
void NVIC_DisableIrq(uint8 irqn);

/**
* @brief   Disable all irqs. 
*/
void NVIC_DisableIrqs(void);

/**
 * @brief   Clear pending interrupt for specific irq. 
 *
 * @param   irq   The irq id. 
*/
void NVIC_ClearIrq(uint8 irqn);

/**
* @brief   Clear pending interrupt for all irqs. 
*/
void NVIC_ClearIrqs(void);

#endif /* INTERRUPTS_H */
