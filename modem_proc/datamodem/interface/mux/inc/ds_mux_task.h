#ifndef DS_MUX_TASK_H
#define DS_MUX_TASK_H
/*===========================================================================

                                 D S   M U X   T A S K

                                 H E A D E R   F I L E

DESCRIPTION
  This is the external header file for the MUX Task. This file
  contains all the functions, definitions and data types needed for other
  tasks to interface to the MUX Task.

  Signal and command definitions for the MUX Task, and functions
  that should be used by other tasks to send commands to the MUX
  Task are defined here.

  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/mux/inc/ds_mux_task.h#2 $
  $DateTime: 2019/10/22 02:53:17 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/16   ad      initial version
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "rex.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
  DOG data structures. 
---------------------------------------------------------------------------*/
extern uint32           dog_ds_mux_rpt_var;          /*! Dog report ID     */
extern uint32           dog_ds_mux_rpt_time_var;     /*! Report Period     */
extern rex_timer_type   ds_mux_dog_rpt_timer;        /*! Watchdog Timer    */

/*!
  Task control block for DS_MUX task 
*/
extern rex_tcb_type  ds_mux_tcb;


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*=========================================================================*/
/*!
  @function 
  ds_mux_task

  @brief
  This function is the entry point and main processing loop for the Data
  Service(s) DS_MUX task.

  @details
  This function can only called by the Task main controller to start the 
  DS_MUX task processing loop. 

  @param[in]  dummy - Ignored.

  @dependencies
  None.

  @return
  Does not Return.
*/
/*=========================================================================*/
void ds_mux_task
(
  uint32 dummy
);


#endif /* DS_MUX_TASK_H */
