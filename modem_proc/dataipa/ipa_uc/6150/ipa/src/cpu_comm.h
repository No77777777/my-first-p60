/*!
  @file
  cpu_comm.h

  @brief
  Declares the A7/AP (master) to uC (slave) communication interface

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

  $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/src/cpu_comm.h#1 $
==============================================================================*/

#ifndef CPU_COMM_H_
#define CPU_COMM_H_

#include "global_defs.h"
#include "ipa_hw_interface_i.h"
#include "notif_mgr.h"
#include "nvic_drv.h"
#include "qmb.h"

#ifdef FEATURE_UT 
#include "ipa_hwp_unit_test.h"
#endif 

#define CPU_COMM_MBOX_IDX     (IPA_CPU_2_HW_CMD_MBOX_n  % 16)
#define CPU_COMM_MAILBOX_MASK (1 << CPU_COMM_MBOX_IDX)

#define IPA_UC_FEATURE_DETECT(cmd_opcode) (cmd_opcode >> 5)

/*  
Structure to strore address shared through
IPA_CPU_2_HW_CMD_STORE_ADD command
*/

typedef struct gSharedAddress_s{
uint32_t cmdParams_lo;        
uint32_t cmdParams_hi; 
}gSharedAddress_s;

/**
 * @brief   database for module. 
*/
typedef struct cpuCommDb_t
{
   uint32_t lastCmdData[IPA_HW_MAX_CMD_SIZE/sizeof(uint32_t)];
   IpaHwSharedMemeMapping_t* pSharedMem;
}cpuCommDb_t;


extern cpuCommDb_t gCpuCommDb;	/**< The cpu comm database */

/**
 * @brief   Process CPU commands that are not protocol specific
 *
 * @param   Notification containing the command details.
*/
void CPU_COMM_ProcessCommand(notif_t  *curNotif);



/**
 * @brief   Process single notification from notification manager
 *
 * @param [in,out]   curNotif pointer to notification to processes. 
*/
void CPU_COMM_ProcessNotification(notif_t *curNotif);

/**
 * @brief   Initialize uC/CPU communication module .
 *
 * @param   ucFwVersion The uC firmware version to be written to shared memory.
*/
void CPU_COMM_Init();

/**
 * @brief   Initialize uC/CPU  SRAM area .
 *
 * @param   None
*/
void CPU_COMM_SRAM_Init();


/**
 * @brief   Process single notification from notification manager
 *
 * @param [in,out]   curNotif pointer to notification to processes. 
*/
void CPU_COMM_ProcessNotification(notif_t *curNotif);

/**
 * @brief  Get parameters of last command sent from CPU.
 *
 * @return  pointer to buffer holding the parameters of last command.
*/
uint32_t* 	CPU_COMM_GetLastCmdParams(void);

/**
 * @brief   Set the state of uC and indication of error due to memory access in shared memory .
 *
 * @param   state    The new state to set.
 * @param   address  The address containing the error.
 * @param   isRead   1 - error was due to read operation, 0 - error was due to write operation
*/
void CPU_COMM_SetUcState(uint8_t state, uint32_t address, uint8_t isRead);

/**
 * @brief   Send an event to CPU. 
 *
 * @param   opcode   The opcode of the event. 
 * @param   params   The parameters of the event
*/
void   		CPU_COMM_WriteEvent(uint8_t opcode, uint32_t params);

/**
 * @brief   Send an response (for previous command) to CPU. 
 *
 * @param   opcode   The opcode of the response. 
 * @param   params   The parameters of the response 
 * @param   sendIrq  Caller indicates if irq needs to be 
 *                   triggered to CPU
*/
void   		CPU_COMM_WriteResponse(uint8_t opcode, uint32_t params, uint8_t sendIrq );

/**
 * @brief   IRQ Handler for IPA0 Interrupt 
*/
void ipa0Int_Handler(void);

#endif /*CPU_COMM_H_*/
