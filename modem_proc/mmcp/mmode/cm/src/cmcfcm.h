#ifndef _CMCFCM_H
#define _CMCFCM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

C A L L   M A N A G E R   Centralized Flow-control Manager(C F C M)   M O D U L E

GENERAL DESCRIPTION
  This module contains the Call Manager CFCM Interface block, which groups
  all the functionality that is a associated with CM<->CFCM interfacing.




EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
  cm_init_after_task_start() must be call to initialize this module before any other
  function declared in this module is being called.


Copyright (c) 2008 - 2015 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include "cfcm.h"

#define THERMAL_LEVEL_0                       0
#define THERMAL_LEVEL_1                       1
#define THERMAL_LEVEL_2                       2
#define THERMAL_LEVEL_3                       3

/** Enumeration of the reason for doing access control on modem
*/
typedef enum

{

  CM_CFCM_ACTION_NONE = -1,
  /**< FOR INTERNAL CM USE ONLY! */

  CM_CFCM_ACTION_ENTER_EMERG = 0,
  /**< Clients start access control for PPP cleaning. */

  CM_CFCM_ACTION_END_LOCAL_HOLD_CALLS = 1,
  /**< Clients start access control for PDN disabling. */

  CM_CFCM_ACTION_EXIT_EMERG = 2,
  /**< Clients start access control due to unsafe temperature. */

  /** @cond
  */

  CM_CFCM_ACTION_MAX
  /**< FOR INTERNAL CM USE ONLY! */

  /** @endcond
  */
} cm_cfcm_action_e_type;

/*===========================================================================

FUNCTION cm_cfcm_thermal_event_proc

DESCRIPTION
  Processes CFCM thermal indications.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cm_cfcm_thermal_event_proc(const cfcm_cmd_msg_type_s *cfcm_ind);

#endif