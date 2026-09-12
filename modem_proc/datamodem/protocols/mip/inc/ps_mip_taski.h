#ifndef PS_MIP_TASKI_H
#define PS_MIP_TASKI_H
/*===========================================================================
                          P S _ M I P _ T A S K . H

DESCRIPTION
  Header file containing the MIP task specific information.  This is intended
  to isolate all of the task specific information to a single module.

Copyright (c) 2008 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

$PVCSPath: L:/src/asw/MM_DATA/vcs/ps_mip_task.h_v   1.1   16 Sep 2002 14:52:18   jayanthm  $
$Header: //components/rel/data.mpss/3.5..2.1/protocols/mip/inc/ps_mip_taski.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
12/19/08    pp     Created module as part of Common Modem Interface:
                   Public/Private split.
===========================================================================*/


/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                             EXTERNAL FUNCTIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

/*===========================================================================
FUNCTION MIP_TASK_POWERUP_INIT()

DESCRIPTION
  This function does the powerup initialization for the MIP module.  This
  includes registering handlers with PS (the task executing it) for the MIP
  RX signals.

PARAMETERS
  None

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void mip_task_powerup_init( void );

#endif /* PS_MIP_TASK_H */
