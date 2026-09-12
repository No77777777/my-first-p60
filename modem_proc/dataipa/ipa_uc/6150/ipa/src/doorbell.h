/*=========================================================================*//**
    @file  doorbell.h

    @brief Declares the doorbell driver interface
*//****************************************************************************/
/*------------------------------------------------------------------------------
    Copyright (c) 2017 Qualcomm Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Confidential and Proprietary
------------------------------------------------------------------------------*/

#ifndef DOORBELL_H_
#define DOORBELL_H_

#include "global_defs.h"

#define MBOX_MAX_REG_PER_GROUP                                 16

#define MBOX_GROUP_0                                           0
#define MBOX_GROUP_1                                           1
#define MBOX_GROUP_3                                           3                    
#define MBOX_MAX_GROUPS                                        4


/**
 * @brief   Initialize Doorbell module.
*/
void   DOORBELL_Init(void);

/**
 * @brief   Get 32 LSB of doorbell value (assuming 32 MSB are always the same).
 *          Doorbell holds the latest ring write pointer value (updated by host) in host space
 *
 * @param   mboxGroup   Mailbox group
 * @param   mbox_num    Mailbox register number in that group
 *
 * @return  32 LSB of doorbell value. 
*/
uint32_t DOORBELL_GetValue(uint8_t mboxGroup, uint8_t mbox_num);

/**
 * @brief   Set 32 LSB of doorbell value (assuming 32 MSB are always the same) to local doorbell mapping.
 *          This write is not expected to generate interrupt
 *
 * @param   mboxGroup   Mailbox group
 * @param   mbox_num    Mailbox register number in that group
 * @param   value      The value to write. 32 LSB of the doorbell
*/
void DOORBELL_SetValue(uint8_t mboxGroup, uint8_t mbox_num, uint32_t value);

/**
 * @brief   Enables the mailbox interrupt (corresponding to 
 *          32LSBs) for the specified channel
 *  
 * @param   mboxGroup          Mailbox group 
 * @param   mailboxToEnable    Mailbox Number in that group 
 *          clrDb      Caller indicates if the DB needs to be
 *                     cleared before enabling
 *  
*/
void DOORBELL_EnableInt(uint8_t mboxGroup, uint32_t mailboxToEnable, uint8_t clrDb );

/**
 * @brief   Enables the mailbox interrupt (corresponding to 
 *          32LSBs) for the specified channel
 *  
 * @param   mboxGroup          Mailbox group  
 * @param   mailboxToDisable   Mailbox number in that group 
 *  
*/
void DOORBELL_DisableInt(uint8_t mboxGroup, uint32_t mailboxToDisable);

/**
 * @brief   Enables the mailbox interrupts (corresponding to 
 *          32LSBs) for the specified mbox group
 *  
 * @param   mboxGroup          Mailbox group 
 * @param   mailboxMask        Mailboxes that need to be enabled
 *                             in that group
 *  
*/
void DOORBELL_EnableIntMask(uint8_t mboxGroup, uint32_t mailboxMask);

/**
 * @brief   Disables the mailbox interrupts (corresponding to 
 *          32LSBs) for the specified mbox group
 *  
 * @param   mboxGroup          Mailbox group 
 * @param   mailboxMask        Mailboxes that need to be 
 *                             disabled in that group
 *  
*/
void DOORBELL_DisableIntMask(uint8_t mboxGroup, uint32_t mailboxMask);


#endif
