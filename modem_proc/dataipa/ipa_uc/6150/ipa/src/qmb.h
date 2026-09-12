/*!
  @file
  qmb.h

  @brief
  Declares the QMB (dma) driver interface

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

  $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/src/qmb.h#1 $
==============================================================================*/

#ifndef QMB_H_
#define QMB_H_

#include "global_defs.h"
#include "notif_mgr.h"

#define QMB_ADDRESS_BOUNDARY_LIMIT_IN_BYTES           1024   //QMB read/write can not cross address of 1K
#define QMB_ADDRESS_BOUNDARY_LIMIT_BYTE_MASK          (QMB_ADDRESS_BOUNDARY_LIMIT_IN_BYTES-1)

#define QMB_MAX_ALIGN_TRANSFER_SIZE 	   			  128   //maximum size in bytes QMB can read in single operation when system address is align
#define QMB_MAX_NON_ALIGN_TRANSFER_SIZE   			  120   //maximum size in bytes QMB can read in single operation when system address is not align (assuming worst case)

#define QMB_SID                                       0x28 //Stream ID 40


//#pragma pack(push,1) /*Save previous, and turn on 1 byte alignment*/

extern uint32_t gReqdUCVmid;

typedef struct QmbNotifParams_t
{
   notifClient client;  //client for notification - specifies the client once the notification is processed by QMB driver
   uint8_t   id;       //id for notification - user data for the target State machine handling the notification
}QmbNotifParams_t;

//#pragma pack(pop) /* Revert alignment to what it was previously*/

typedef enum
{
   QMB_OPERATION_READ  = 0, 	// Read from System NOC to local memory.
   QMB_OPERATION_WRITE = 1 	// Write to System NOC from local memory.  
}QMB_OPERATION;

/**
 * @brief   Struct carries the information for DMA transfer. This struct is used as element in the pending queue
*/
typedef struct qmbTransferElem_t
{
   uint32_t systemAddress;
   uint16_t localAddress;
   QmbNotifParams_t param;	/**< User parameters to be used to invoke notification once transfer is completed. */
   uint8_t length;
}qmbTransferElem_t;


/**
 * @brief   IRQ Handler for QMB Read Interrupt 
*/
void qmbReadInt_Handler(void);

/**
 * @brief   IRQ Handler for QMB Write Interrupt 
*/
void qmbWriteInt_Handler(void);

/**
 * @brief   Initialize QMB module
*/
void QMB_Init(void);

/**
 * @brief   Process single notification. 
 *
 * @param [in,out]   curNotif Current notification to process. 
*/
void QMB_ProcessNotification(notif_t *);

/**
 * @brief   Send (async) transfer to QMB
 *
 * @param   systemAddress  The system address. 
 * @param   localAddress   The local address (in uC address space). 
 * @param   length         The length of transfer in bytes. 
 * @param   operation      The operation. 
 * @param   param          The parameter to be sent with notification once operation is completed.
*/
/*void QMB_SendTransfer(uint32_t systemAddress,
	                  uint32_t localAddress,
	                  uint32_t length,
	                  QMB_OPERATION operation,
	                  const QmbNotifParams_t );
*/

/**
 * @brief   Perform (sync) transfer operation
 *
 * @param   systemAddress  The system address. 
 * @param   localAddress   The local address (in uC address space). 
 * @param   length         The length of transfer in bytes. 
 * @param   operation      The operation. 
 *
 * @note The transfer size and address are not limited. In some cases transfer is split to several transfers
*/
void QMB_TransferBlocking(uint32_t systemAddress,
	                      uint32_t localAddress,
	                      uint32_t length,
                          QMB_OPERATION operation);

#endif /* QMB_H_ */
