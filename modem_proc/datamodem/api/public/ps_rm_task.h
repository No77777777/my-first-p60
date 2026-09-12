#ifndef PS_RM_TASK_H
#define PS_RM_TASK_H
/*===========================================================================

                          P S _ RM _ T A S K . H

DESCRIPTION
  This is the header file for the PS RM Data processing Task. Contained 
  herein are the functions needed to initialize all the modules that execute
  in PS RM task context and the main task processing loop.

EXTERNALIZED FUNCTIONS
  ps_rm_task()
    PS RM Task entry point and main processing loop.

Copyright (c) 2010 - 2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/api/public/ps_rm_task.h#2 $ 
  $DateTime: 2019/10/22 02:53:17 $
  $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/01/16    ds     Removed prototype of ps_rm_task_is_running().
05/04/16    ds     Added prototype of ps_rm_task_is_running().
06/10/10    dm     Created module

===========================================================================*/


/*===========================================================================

                                INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "dog.h"
#include "rex.h"


/*===========================================================================

                             MACROS & DATA DECLARATIONS

===========================================================================*/

extern dog_report_type   ps_rm_dog_rpt_var;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION PS_RM_TASK()

DESCRIPTION
  This function is the entry point and main processing loop for the RM Data
  processing task.

DEPENDENCIES
  None.

RETURN VALUE
  Does not Return.

SIDE EFFECTS
  None.

===========================================================================*/

void
ps_rm_task
(
  uint32 dummy
);

#endif /* PS_RM_TASK_H */

