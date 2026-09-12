/*========================================================================*/
/**
@file csmd_bufq.h

@brief This file contains the shared MACRO (opcode) definitions
and structure declarations for csmd operations.
This exposes APIs to the GPR layer.
*/
/*========================================================================
Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================== */
/*========================================================================
Edit History

$Header: L:\Projects\avs.adsp.2.9.Mar8_SIM_2_9_gpr\adsp_proc\avs\main\gpr\datalink\gpr_csmd\inc\csmd_bufq.h

when       who        what, where, why
--------   ---       --------------------------------------------------
06/07/18   akr         Created file.
========================================================================== */
#ifndef CSMD_BUFQ_H
#define CSMD_BUFQ_H
/*==========================================================================
Include files
========================================================================== */
#include "ar_osal_error.h"
#include "ar_osal_types.h"
#include "ar_osal_heap.h"

/*----------------------------------------------------------------------------
* Type Declarations
* -------------------------------------------------------------------------*/
struct q_node
{
   void *         intent_ptr; /*< Ptr to the intent*/
   struct q_node *next;       /*< Ptr to the next node*/
};
typedef struct q_node q_node_t;

/*----------------------------------------------------------------------------
* Function Declarations
* -------------------------------------------------------------------------*/
/*Utility to create the Linked List Queue, malloc intents and store in Free List*/
uint32_t queue_intents(uint32_t num_intents, uint32_t intent_size);
/*Utility called when an intent needs to be used to copy data and forward
      we load data into the intent from the freeQ and move that node to usedQ */
uint32_t use_intent(void **intent_to_use_pptr);
/*Utility called when a used intent needs to be returned to the freeQ*/
uint32_t return_intent(void *ptr);
/*Utility called when all the intents and the List(Q) memories need
      to be freed back to the heap */
uint32_t free_intents();

#endif // CSMD_BUFQ_H
