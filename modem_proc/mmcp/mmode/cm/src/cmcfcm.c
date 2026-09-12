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

#include "cmcfcm.h"
#include "cmpmprx.h"
#include "cmdbg.h"
#include "cmph.h"
#include "cmaccessctrl.h"

#ifdef FEATURE_CM_CFM_AVAILABLE
/*===========================================================================

FUNCTION cm_cfcm_consolidate_thermal_action

DESCRIPTION
  Processes CFCM thermal indications for various monitors(thermal PA, thermal runaway(CX)) and returns
  action to be performed by CM

DEPENDENCIES
  none

RETURN VALUE
  Action to be performed by CM
  CM_CFCM_ACTION_NONE:                 No action needed
  CM_CFCM_ACTION_ENTER_EMERG:          Enters emergency mode
  CM_CFCM_ACTION_END_LOCAL_HOLD_CALLS: CM to end local hold calls
  CM_CFCM_ACTION_EXIT_EMERG:           Exit emergency mode

SIDE EFFECTS
  none

===========================================================================*/

static cm_cfcm_action_e_type cm_cfcm_consolidate_thermal_action(const cfcm_cmd_msg_type_s *cfcm_ind)
{
  cm_cfcm_action_e_type   cfcm_action = CM_CFCM_ACTION_NONE;

  if(((cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_THERMAL_PA)&&
      (cfcm_ind->fc_cmd.monitor_data.pa.state == THERMAL_LEVEL_3))||
      ((cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_THERMAL_CX)&&
       (cfcm_ind->fc_cmd.monitor_data.cx.state == THERMAL_LEVEL_3))||
       #ifdef FEATURE_DISABLED_HABANERO
      ((cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_SKIN_TEMP)&&
       (cfcm_ind->fc_cmd.monitor_data.skin_temp.state == THERMAL_LEVEL_3))||
       #endif
      ((cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_MDM_TEMP)&&
       (cfcm_ind->fc_cmd.monitor_data.mdm_temp.state == THERMAL_LEVEL_3))||
      ((cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_VDD_PEAK_CURR_EST)&&
       (cfcm_ind->fc_cmd.monitor_data.vdd_peak_curr.state == THERMAL_LEVEL_3)))
  {
    cfcm_action = CM_CFCM_ACTION_ENTER_EMERG;
  }
  else if ( cmph_is_dsda()&&
            cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_THERMAL_PA &&
            cfcm_ind->fc_cmd.monitor_data.pa.state == THERMAL_LEVEL_2  &&
            !cmac_is_in_thermal_emergency(SYS_MODEM_AS_ID_1)) /*** AMEYA: CHECK THIS CONDITION LATER ***/
  {
    cfcm_action = CM_CFCM_ACTION_END_LOCAL_HOLD_CALLS;
  }

  /* Only for thermal CX, CM exits thermal emergency upon lvl2 itself
  ** for other monitors it would be at lvl0
  */
  else if((!(cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_THERMAL_PA)||
           (cfcm_ind->fc_cmd.monitor_data.pa.state == THERMAL_LEVEL_0))&&
          (!(cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_MDM_TEMP)||
           (cfcm_ind->fc_cmd.monitor_data.mdm_temp.state == THERMAL_LEVEL_0))&&
           #ifdef FEATURE_DISABLED_HABANERO
          (!(cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_SKIN_TEMP)||
           (cfcm_ind->fc_cmd.monitor_data.skin_temp.state == THERMAL_LEVEL_0))&&
           #endif
          (!(cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_VDD_PEAK_CURR_EST)||
           (cfcm_ind->fc_cmd.monitor_data.vdd_peak_curr.state == THERMAL_LEVEL_0))
         )
  {
    cfcm_action = CM_CFCM_ACTION_EXIT_EMERG;
  }

  return  cfcm_action;

}

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
void cm_cfcm_thermal_event_proc(const cfcm_cmd_msg_type_s *cfcm_ind)
{

  cm_cfcm_action_e_type   cfcm_action = CM_CFCM_ACTION_NONE;

#ifdef FEATURE_CM_THERMAL_EMERGENCY_SUPPORT
  CM_ASSERT(cfcm_ind != NULL);

  cmpmprx_thermal_state_change_policy_update(cfcm_ind);

  cfcm_action = cm_cfcm_consolidate_thermal_action(cfcm_ind);

  CM_MSG_HIGH_2("TH EMERG Recvd CFCM Evt Monitor_mask: %d Consolidated action: %d",cfcm_ind->fc_cmd.monitors_mask, cfcm_action);

  switch(cfcm_action)
  {
    case CM_CFCM_ACTION_ENTER_EMERG:
      cmac_restrict_all_subs_to_emergency(CM_AC_REASON_UNSAFE_TEMP);
      break;

    case CM_CFCM_ACTION_END_LOCAL_HOLD_CALLS:
      cmac_enter_thermal_emergency_level_2();
      break;

    case CM_CFCM_ACTION_EXIT_EMERG:
      cmac_restore_all_subs_to_normal(CM_AC_REASON_UNSAFE_TEMP);
      break;

    default:
      break;
  }

#else

  SYS_ARG_NOT_USED(cfcm_ind);
  CM_MSG_HIGH_0("FEATURE_CM_THERMAL_EMERGENCY_SUPPORT not enabled");

#endif /* FEATURE_CM_THERMAL_EMERGENCY_SUPPORT */
}

#endif /* FEATURE_CM_CFM_AVAILABLE */

