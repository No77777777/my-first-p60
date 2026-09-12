#ifndef REG_TASK_H
#define REG_TASK_H
/*==============================================================================


                   R E G  -  T A S K   H E A D E R   F I L E


                                  DESCRIPTION

  This module defines the types and functions necessary to send a command
  to the REG task.

                             COPYRIGHT INFORMATION

Copyright (c) 2002 Qualcomm Technologies, Incorporated and its licensors.  All Rights
Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                            EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/reg/vcs/reg_task.h_v   1.1   06 Jun 2002 10:45:00   kabraham  $
$Header: //components/rel/mmcp.mpss/7.9.0/api/public/reg_task.h#1 $ $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -------------------------------------------------------------

==============================================================================*/


/*==============================================================================

FUNCTION NAME

  reg_main

DESCRIPTION

  Main routine for the REG task.

RETURN VALUE

  None

==============================================================================*/

#include "trm.h"
extern void reg_main
(
  dword dummy
);
extern void reg_mode_set_mcs_pri_cb(trm_idle_coll_event_data* event_data);
#endif
