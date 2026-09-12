#ifndef LTE_QMI_TASK_H
#define LTE_QMI_TASK_H
/*==============================================================================

                LTE_QMI_TASK.H

DESCRIPTION
  This file contains the declarations needed in LTE_QMI Task.

Copyright (c) 2013 - 2015 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
==============================================================================*/

/*==============================================================================

  $Header: 
  $DateTime: 
  $Author: 
 
  when        who    what, where, why
  --------    ---    -------------------------------------------------------------
  07/18/15    psv     created file.
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/

#include "customer.h"
#include "comdef.h"
#include "queue.h"
#include "rex.h"
#include <msgr.h>
#include <msgr_types.h>
#include <msgr_rex.h>
#include "lte_qmi.h"
#include <lte_assert.h>


#include "lte_qmi_msgr.h"
#include "lte_qmi_svc.h"

/*==============================================================================

                           INTERNAL TYPE DEFINITIONS

==============================================================================*/

#define QMI_ASSERT(exp) ERR_FATAL("Assert " #exp " failed: ", 0, 0, 0 ); 
    
/*------------------------------------------------------------------------------
  Task top-level signals
------------------------------------------------------------------------------*/


/*!
  @brief
  An enum describing the bit positions in the lte_qmi_signal_bitmask_t
  */
enum
{
  LTE_QMI_TASK_SERVER_SIG             = 0x00000001, /* Signal that will be
                                                     processed by the QMI
                                                     server */

  LTE_QMI_TASK_MSGR_SIGNAL            = 0x00000002,/* MSGR signal given by
                                                      message router */
  
   /*Space for other Dog timer related signals*/
  LTE_QMI_TASK_SIGNAL_MAX = 0x7F
};
     
typedef uint32 lte_qmi_signal_bitmask_t;

/*All signals that are currently expected to be received by LTE QMI service*/
#define LTE_QMI_EXPECTED_SIGNALS_MASK  (LTE_QMI_TASK_SERVER_SIG | LTE_QMI_TASK_MSGR_SIGNAL)

/*------------------------------------------------------------------------------
  Signal processing Macros
------------------------------------------------------------------------------*/
#define LTE_QMI_TASK_RESET_SIG(signal_mask, signal) signal_mask &= ~signal
#define LTE_QMI_TASK_IS_SIG_SET(signal_mask, signal) ((signal_mask & signal) != 0)


/*==============================================================================

                           EXTERNAL MACRO DEFINITIONS

==============================================================================*/
#define LTE_QMI_TCB_PTR lte_qmi_task_get_tcb_ptr()

/*==============================================================================

                           EXTERNAL FUNCTION DEFINITIONS

==============================================================================*/

/*==============================================================================
FUNCTION LTE_QMI_TASK_GET_TCB_PTR()

DESCRIPTION
  This function is used to return the TCB pointer for LTE_QMI task

PARAMETERS
  NONE

RETURN VALUE
  Pointer to the command

DEPENDENCIES
  None.
 
SIDE EFFECTS
  None¢À

==============================================================================*/
rex_tcb_type* lte_qmi_task_get_tcb_ptr(void);

/*============================================================================*/

void* lte_qmi_task_main
(
  void *arg             /*!< argument passed in during thread creation */  
);


#endif /* LTE_QMI_TASK_H */
