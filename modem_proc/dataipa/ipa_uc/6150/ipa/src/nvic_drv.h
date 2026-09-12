/*!
  @file
  nvic_drv.h

  @brief
  Declares the nvic (interrupt) driver interface

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

  $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/src/nvic_drv.h#1 $
==============================================================================*/
#ifndef NVIC_DRV_H
#define NVIC_DRV_H

#ifndef __ASSEMBLER__
#include "interrupts.h"
#endif  // __ASSEMBLER__

/**
 * @brief   IRQs in system. 
 *  
 * @note    These numbers define the uC NVIC interrupt numbers based on their priority. 
 *          Numbers in comment indicate the IU priority order
 *          Lower Number == Higher Priority (IU has fixed priority)
*/
#define NVIC_DRV_NMI_IRQ         0                 //IU Priority:  0
#define NVIC_DRV_SW_EVENT_IRQ    1                  //IU Priority:  1
#define NVIC_DRV_IPA_0_IRQ       2                 //IU Priority:  2
#define NVIC_DRV_IPA_1_IRQ       3                 //IU Priority:  3
#define NVIC_DRV_IPA_2_IRQ       4                 //IU Priority:  4
#define NVIC_DRV_IPA_3_IRQ       5                 //IU Priority:  5
#define NVIC_DRV_QMB_READ_IRQ    6                  //IU Priority:  6
#define NVIC_DRV_QMB_WRITE_IRQ   7                  //IU Priority:  7
#define NVIC_DRV_TIMER_0_IRQ     8                  //IU Priority:  8
#define NVIC_DRV_TIMER_1_IRQ     9                  //IU Priority:  9
#define NVIC_DRV_TIMER_2_IRQ     10                  //IU Priority:  10
#define NVIC_DRV_TIMER_3_IRQ     11                  //IU Priority:  11
#define NVIC_DRV_TIMER_4_IRQ     12                  //IU Priority:  12
#define NVIC_DRV_MBOX_0_IRQ      13                 //IU Priority:  13
#define NVIC_DRV_MBOX_1_IRQ      14                 //IU Priority:  14
#define NVIC_DRV_MBOX_2_IRQ      15                 //IU Priority:  15
#define NVIC_DRV_MBOX_3_IRQ      16                 //IU Priority:  16

#define NVIC_DRV_MBOX_4_IRQ      17                  //IU Priority:  17
#define NVIC_DRV_MBOX_5_IRQ      18                  //IU Priority:  18
#define NVIC_DRV_MBOX_6_IRQ      19                  //IU Priority:  19
#define NVIC_DRV_MBOX_7_IRQ      20                  //IU Priority:  20
#define NVIC_DRV_GSI_IRQ         21                  //IU Priority:  21

#define NVIC_DRV_HW_EVENT_IRQ    22                 //IU Priority:  22

#endif /* NVIC_DRV_H */
