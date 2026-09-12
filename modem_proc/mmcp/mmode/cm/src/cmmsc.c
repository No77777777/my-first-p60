/*====*====*===*====*====*====*====*====*====*====*====*====*====*====*====*
             C M    M U L T I - S T A C K    C O N T R O L L E R

                 I M P L E M E N T A T I O N    F I L E

GENERAL DESCRIPTION
  This file implements the CM multi-stack architecture to support multi-stacks
  requirements such as SVLTE, DualSim, etc.

EXTERNALIZED FUNCTIONS
  Functional Interface:



Copyright (c) 2013 - 2015 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmmsc.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/18/14   ss      Added function to check is SS valid for PS detach
01/30/14   ss      Conditional Reset of ue_mode_forced flag
01/02/14   ss      Check for on-going voice or emergency calls before forcing
                   user prefs down to MMoC
11/14/13   ss      Add check to not send UE MODE seicth addl action for pref
                   reas restore
10/07/13   jvo     Replace FEATURE_UICC_RAT_INDICATOR_SUPPORT with
                   is_rat_bal flag
08/13/13   cl      CM supports explicit PS_DETACH when capability from PM changed
07/22/13   fj      Add support for DSDX L+G
07/21/13   skk     FR2921: Network initiated Authentication Procedure Support
                   during emergency call When SIM is perso Locked
07/03/13   dk      KW error fixed in cmmsc_get_policy_mode_band_cap,
                   cmmsc_action_local_deact, cmmsc_send_sys_sel_pref_chgd_cmd
06/19/13   gm      Send pref to SD-ALL stacks before ONLINE/PWRUP ind
02/26/13   cl      Initial release.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "mmcp_variation.h"
#include "customer.h" /* Customer configuration file */

#include "comdef.h"   /* Definition for basic types and macros */
#include "cmph.h"     /* Interface to CM ph functions */
#include "cmefs.h"
#include "cmmsc.h"
#include "cmmsc_auto.h"
#include "cmsimcoord.h"
#include "cmsoa.h"
#include "cmtaski.h"
#include "cmsds.h"
#include "mmoc_v.h"
#include "modem_mem.h"
#include "sys.h"      /* Common types. */
#include "sd_v.h"

#ifdef FEATURE_IP_CALL
#include "cmipappi.h" /* Interface to inform cm ip app about service */
#endif

#if defined(CM_GW_SUPPORTED) || defined(FEATURE_CM_LTE) || defined(FEATURE_TDSCDMA)
#include "cmregprxi.h"
#include "cmss.h"
#endif

#include "cmpmprx.h"
#include "cmemgext.h"
#include "cmaccessctrl.h"
#include "cmdbg.h"

#include "sd_v.h"      /* Interface to system Determination */

#ifdef CM_DEBUG
#error code not present
#endif


/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

static cmmsc_routing_table_s_type auto_route_table;

/*===========================================================================

            FUNCTION DECLARATIONS AND IMPLEMENTATION

This section contains function declaration and implementation for this
feature.

===========================================================================*/

/* Static functions
*/
static void cmmsc_action_power_down(int stack_id,
                                    cmmsc_state_machine_s_type *state_machine);
static void cmmsc_action_sub(
  const cmmsc_event_sub_chg_s_type *evt_pkg
);
static void cmmsc_init_mode_and_capability (
  cmmsc_state_machine_s_type  *state_machine
);


static void cmmsc_get_stack_pref_per_cap (
  cmmsc_state_machine_s_type    *state_machine,
  int                            stack_idx,
  cm_preference_info_s_type     *pref_info,
  cmmsc_stack_capability_s_type *common_cap_ptr
);
static void cmmsc_get_user_mode_pref_per_cap (
  cmmsc_state_machine_s_type    *state_machine,
  int                            stack_idx,
  sd_ss_mode_pref_e_type        *user_mode_pref
);
static void  cmmsc_print_route_table_info(
  cmmsc_state_machine_s_type *state_machine,
  cmmsc_event_e_type  event,
  int                 route_tbl_idx
);
static boolean cmmsc_is_op_mode_multi_stacks (
  cmmsc_state_machine_op_mode_e_type op_mode
);
static boolean cmmsc_force_all_stack_required (
  cmmsc_state_machine_op_mode_e_type prev_op_mode,
  cmmsc_state_machine_op_mode_e_type new_op_mode
);
static cm_mode_band_pref_s_type  cmmsc_current_stack_preference(
  cmmsc_state_machine_s_type  *state_machine
);
static cmmsc_event_e_type cmmsc_get_route_tbl_event(
  cmmsc_routing_table_s_type *route_table,
  int route_idx
);
static cmmsc_state_machine_op_mode_e_type cmmsc_get_route_tbl_mode(
  cmmsc_routing_table_s_type *route_table,
  int route_idx
);
static cmmsc_stack_state_e_type cmmsc_get_route_tbl_stack_state(
  cmmsc_routing_table_s_type *route_table,
  int stack_idx,
  int route_idx
);
static cmmsc_stack_state_e_type cmmsc_get_route_tbl_stack_new_state(
  cmmsc_routing_table_s_type *route_table,
  int stack_idx,
  int route_idx
);
static cmmsc_action_s_type *cmmsc_get_route_tbl_action(
  cmmsc_routing_table_s_type *route_table,
  int stack_idx,
  int route_idx
);
static boolean cmmsc_is_volte_active_on_hybr2 (
  cmmsc_state_machine_s_type  *state_machine
);

static void cmmsc_action_suspend_stack(int stack_id,
                                       cmmsc_state_machine_s_type *state_machine);

static void cmmsc_action_resume_stack(int stack_id,
                                      cmmsc_state_machine_s_type *state_machine);

static void cmmsc_set_domain_cap_dual_lte(
  cmmsc_state_machine_s_type    *state_machine
);

/****************************************************************************
**    Function definitions
****************************************************************************/

/*===========================================================================

FUNCTION cmmsc_route_table_ptr

DESCRIPTION
  This function returns the a pointer to the entry of the Routing Table

DEPENDENCIES
  None.

RETURN VALUE
  cmmsc_routing_table_s_type*

===========================================================================*/
cmmsc_routing_table_s_type *cmmsc_route_table_ptr()
{
  return &auto_route_table;
}

/*===========================================================================

FUNCTION cmmsc_get_curr_msc_ue_mode

DESCRIPTION
  This function gets the current ue mode stored in msc.

DEPENDENCIES
  None.

RETURN VALUE
  sys_ue_mode_e_type.

===========================================================================*/
sys_ue_mode_e_type cmmsc_get_curr_msc_ue_mode(
  cmmsc_state_machine_s_type *state_machine
)
{
  return state_machine->stack_common_info.ue_mode;
}

/*===========================================================================

FUNCTION cmmsc_get_msc_rat_cap

DESCRIPTION
  This function returns the rat cap stored in cmmsc for the specified sub.

DEPENDENCIES
  None.

RETURN VALUE
  sys_sys_mode_mask_e_type.

SIDE EFFECTS
  None.
===========================================================================*/
sys_sys_mode_mask_e_type cmmsc_get_msc_rat_cap(
  sys_modem_as_id_e_type as_id
)
{
  cmmsc_state_machine_s_type *state_machine = NULL;

  if(!cmutil_is_as_id_valid(as_id))
  {
    CM_MSG_HIGH_1("invalid as_id %d", as_id);
    as_id = SYS_MODEM_AS_ID_1;
  }

  state_machine = cmmsimc_state_machine_ptr(as_id);

  if(state_machine == NULL)
  {
    CM_ERR_FATAL_1("MSC: state_machine null ptr as_id=%d", as_id);
    return SYS_SYS_MODE_MASK_NONE;
  }

  return state_machine->stack_common_info.ue_mode_capability;
}

/*===========================================================================

FUNCTION cmmsc_get_curr_msc_max_cap

DESCRIPTION
  This function gets the current max capability stored in msc.

DEPENDENCIES
  None.

RETURN VALUE
  sys_sys_mode_mask_e_type.

===========================================================================*/
sys_sys_mode_mask_e_type cmmsc_get_curr_msc_max_cap(
  sys_modem_as_id_e_type as_id
)
{
  if(!cmutil_is_as_id_valid(as_id))
  {
    CM_MSG_HIGH_1("Warning: Invalid as_id %d", as_id);
    return SYS_SYS_MODE_MASK_NONE;
  }
  else
  {
    cmmsc_state_machine_s_type *state_machine = cmmsimc_state_machine_ptr(as_id);
    return state_machine->stack_common_info.ue_max_mode_capability;
  }
}

/*===========================================================================

FUNCTION cmmsc_state_machine_op_mode_update

DESCRIPTION
  This function update the state machine operation mode (SG,non-SG,CSFB, etc.)

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if there is op mode update
===========================================================================*/
boolean cmmsc_state_machine_op_mode_update(
  cmmsc_state_machine_s_type          *state_machine,
  /* Pointer to the state machines that needs to be updated */

  cmmsc_state_machine_op_mode_e_type   new_mode
  /* Update the current operating mode to new mode */
)
{
  if(state_machine == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return FALSE;
  }

  if(state_machine->op_mode != new_mode)
  {
#ifdef FEATURE_CM_DEBUG_BUFFER
    cm_op_mode_change_entry_s_type op_mode_changed_entry;
    op_mode_changed_entry.as_id = state_machine->asubs_id;
    op_mode_changed_entry.old_op_mode = state_machine->op_mode;
    op_mode_changed_entry.new_op_mode = new_mode;
    cmdbg_add_to_dbg_buffer(CM_BUFF_OP_MODE_CHANGE, 0, ((void *)&op_mode_changed_entry));
#endif

    CM_MSG_HIGH_5_ASID("op_mode_chgd: updating op_mode, new %d, old %d, num_stacks %d->%d",
                       state_machine->asubs_id, new_mode, state_machine->op_mode,
                       cmtask_get_num_stacks(state_machine->asubs_id),
                       cmtask_get_targeted_num_stacks(state_machine->asubs_id));
    state_machine->op_mode = new_mode;
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION cmmsc_compare_state_machine_op_mode

DESCRIPTION
  This function compares two op_modes. Op_Mode are enums and is not always
  'the same' to be equal.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - if equals
===========================================================================*/
boolean cmmsc_compare_state_machine_op_mode(
  cmmsc_state_machine_op_mode_e_type  mode1,
  cmmsc_state_machine_op_mode_e_type  mode2
)
{
  if(mode1 == mode2)
  {
    return TRUE;
  }

  /* If either of mode says "ANY", then they're matched */
  if(mode1 == CMMSC_OP_MODE_ANY ||
      mode2 == CMMSC_OP_MODE_ANY)
  {
    return TRUE;
  }

  if(mode2 == CMMSC_OP_MODE_AUTO_SVLTE)
  {
    return ((mode1 == CMMSC_OP_MODE_AUTO_MAIN_HYBR1) ||
            (mode1 == CMMSC_OP_MODE_AUTO_MAIN_HYBR1_HYBR2));
  }

  if(mode1 == CMMSC_OP_MODE_AUTO_SVLTE)
  {
    return ((mode2 == CMMSC_OP_MODE_AUTO_MAIN_HYBR1) ||
            (mode2 == CMMSC_OP_MODE_AUTO_MAIN_HYBR1_HYBR2));
  }

  return FALSE;

}

/*===========================================================================

FUNCTION cmmsc_compare_stack_states

DESCRIPTION
  This function compares two stack_states. Stack States are enums and
  not always the same to be 'equal'.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - if equals
===========================================================================*/
boolean cmmsc_compare_stack_states(
  cmmsc_stack_state_e_type  state1,
  cmmsc_stack_state_e_type  state2
)
{
  if(state1 == state2)
  {
    return TRUE;
  }

  /* If either of mode says "ANY", then they're matched */
  if(state1 == CMMSC_STACK_STATE_ANY ||
      state2 == CMMSC_STACK_STATE_ANY)
  {
    return TRUE;
  }

  return FALSE;
}

/*===========================================================================

FUNCTION cmmsc_init

DESCRIPTION
  This function initialized the available routing tables with proper data.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_init( void )
{
  /* Initialize auto routing table */
  cmmsc_auto_routing_table_init(&auto_route_table);
}

/*===========================================================================

FUNCTION cmmsc_state_machine_init

DESCRIPTION
  This function should be called to initialize the whole state machine

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_state_machine_init(
  cmmsc_state_machine_s_type *state_machine
)
{
  if(state_machine == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  state_machine->op_mode = CMMSC_OP_MODE_AUTO_MAIN;

  cmmsc_init_multi_stacks(state_machine);
}

/*===========================================================================

FUNCTION cmmsc_state_machine_process

DESCRIPTION
  This function should be called to process with state machine after we
  generated the EVENT

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_state_machine_process(
  cmmsc_state_machine_s_type  *state_machine,
  cmmsc_event_base_s_type     *event_package
)
{
  int route_tbl_idx = -1;

  if(event_package->event != CMMSC_EVENT_NONE)
  {
    /* Get the next action from the route table */
    route_tbl_idx
      = cmmsc_state_machine_find_route_idx(state_machine, event_package->event);

    //cmmsc_print_route_table_info(state_machine, event_package->event, route_tbl_idx);

#ifdef FEATURE_CM_DEBUG_BUFFER
    {
#if 0
      // dissabling as of now, when needed please remove #if 0
      cm_msc_routing_entry_s_type msc_entry;
      msc_entry.as_id = state_machine->asubs_id;
      msc_entry.tbl_id = state_machine->route_tbl;
      msc_entry.idx = route_tbl_idx;
      cmdbg_add_to_dbg_buffer(CM_BUFF_MSC_ROUTING_ENTRY_TYPE, 0, ((void *)&msc_entry));
#endif
    }
#endif

    if(route_tbl_idx != -1)
    {
      /* Perform the action */
      cmmsc_execute_actions(route_tbl_idx, (cmmsc_event_base_s_type *)event_package);

      /* Update the state */
      cmmsc_update_state(state_machine, route_tbl_idx);
    }
  }
}

/*===========================================================================

FUNCTION cmmsc_hicps_sxlte_remove_main_gw_needed

DESCRIPTION
  This function determines whether removing GW from main is allowed or not .
  GWL should be removed from main stack , to avoid GW acq on main stack ,
  when UE decided to switch to SRLTE .
  Otherwise GW acq can delay the LTE acq on hybr2 , which is not desirbale .
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static boolean cmmsc_hicps_sxlte_remove_main_gw_needed(
  cmmsc_state_machine_s_type  *state_machine,
  int                          stack_idx,
  cm_preference_info_s_type   *pref_info
)
{
  sd_si_info_s_type      *si_info_ptr;
  boolean                 ret = FALSE;
  sd_ss_mode_pref_e_type  ph_obj_mode;
  boolean                 is_ph_obj_gw_mode = FALSE;
  mm_sub_stk_id_s_type    mm_id;

  if(state_machine == NULL)
  {
    return FALSE;
  }

  si_info_ptr = cmss_true_stack_info_ptr(cmph_map_sub_stk_to_id(state_machine->asubs_id, MM_STACK_0));
  mm_id = cmph_map_sub_stk_to_id(state_machine->asubs_id, MSC_STK_ID_TO_MM_STK_ID(stack_idx));
  if(si_info_ptr == NULL)
  {
    return FALSE;
  }
  /* Exit if 3GPP2 is not allowed on the sub */
  if(cmmsc_get_1x_sub() != state_machine->asubs_id)
  {
    return FALSE;
  }

  ph_obj_mode =
    SD_GET_COMMON_MODE(
      cmph_map_cm_mode_pref_to_sd_mode_pref(pref_info->mode_pref),
      cmmsc_get_stk_mode_cap(mm_id));

  /* Check if the phone has GW mode */
  is_ph_obj_gw_mode =
    SD_MODE_CONTAIN(
      cmph_map_cm_mode_pref_to_sd_mode_pref(CM_MODE_PREF_GWL),
      ph_obj_mode);

  if(stack_idx == 0 && is_ph_obj_gw_mode
      &&
      (si_info_ptr->srv_status != SYS_SRV_STATUS_NO_SRV ||
       si_info_ptr->srv_status != SYS_SRV_STATUS_PWR_SAVE)
      &&
      si_info_ptr->mode == SYS_SYS_MODE_LTE &&
      pref_info->pref_update_reason == SD_SS_PREF_UPDATE_REASON_SWITCH_SVLTE)
  {
    CM_MSG_HIGH_6("HICPS: stk_id %d ph_obj_mode: %d, is_ph_obj_gw_mode: %d srv_status %d sys_mode %d pref_update_reason: %d",
                  MSC_STK_ID_TO_MM_STK_ID(stack_idx),
                  ph_obj_mode,
                  is_ph_obj_gw_mode,
                  si_info_ptr->srv_status,
                  si_info_ptr->mode,
                  pref_info->pref_update_reason);


    ret = TRUE;
  }

  return ret;
}

/*===========================================================================

FUNCTION cmmsc_perform_action

DESCRIPTION
  This function takes action based on the matched index action.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_execute_actions(
  int                       route_table_idx,
  cmmsc_event_base_s_type  *event_package
)
{
  int                          stack_idx = 0;
  cmmsc_action_e_type          act = CMMSC_ACTION_NONE;
  int                          act_idx;
  cmmsc_routing_table_s_type  *route_table = cmmsc_route_table_ptr();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(act_idx = 0; act_idx < route_table->n_stack; act_idx++)
  {
    cmmsc_action_s_type *action = cmmsc_get_route_tbl_action(route_table, act_idx, route_table_idx);
    if(action != NULL)
    {
      stack_idx = action->stack_id;
      act = action->action;
    }
    if(event_package->state_machine == NULL) return;


    /* Skip the action if the stack_idx is invalid or inactive */
    if(act != CMMSC_ACTION_NONE &&
        !cmmsc_auto_is_valid_stack_idx(event_package->state_machine, stack_idx))
    {
      CM_MSG_HIGH_1("Ignoring stack index %d", stack_idx);
      continue;
    }

    if(act != CMMSC_ACTION_NONE)
    {
      CM_MSG_HIGH_4("MSC: execute action %d on stack_idx %d from tbl_idx %d, sub %d",
                    act, stack_idx, route_table_idx,
                    event_package->state_machine->asubs_id);
    }

    switch(act)
    {
      case CMMSC_ACTION_POWER_DOWN:
      {
        cmmsc_event_oprt_mode_s_type *evt_pkg = (cmmsc_event_oprt_mode_s_type *)event_package;
        cmmsc_action_power_down(stack_idx, evt_pkg->state_machine);
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case CMMSC_ACTION_SUB:
      {
        cmmsc_event_sub_chg_s_type *evt_pkg = (cmmsc_event_sub_chg_s_type *)event_package;
        cmmsc_action_sub(evt_pkg);
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case CMMSC_ACTION_SYS_SEL_PREF:
      {
        cmmsc_event_ss_pref_s_type  *ss_pref;
        boolean                      gwl_removed = FALSE;

        /* casting to correct type */
        ss_pref = (cmmsc_event_ss_pref_s_type *)event_package;
        if(ss_pref->cmd && ss_pref->cmd->pref_info && ss_pref->state_machine)
        {
          if(cmmsc_hicps_sxlte_remove_main_gw_needed(ss_pref->state_machine,
              stack_idx, ss_pref->cmd->pref_info))
          {
            /* Remove GWL from main stack
            ** Since it interuppts LTE acq on HYBR2 .
            */
            gwl_removed = cmmsc_auto_update_gwl_operation(FALSE,
                          SD_SS_PREF_REAS_USER,
                          CM_MODE_PREF_GWL,
                          TRUE,
                          cmph_map_sub_stk_to_id(ss_pref->state_machine->asubs_id, 0));
          }
          if(gwl_removed == FALSE)
          {
            cmmsc_send_sys_sel_pref_chgd_cmd(ss_pref->state_machine,
                                             stack_idx, ss_pref->cmd->pref_info, NULL);
          }
        }
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Force preference with reason user */
      case CMMSC_ACTION_FORCE_USER:
      {
        cmmsc_action_force_top(event_package->state_machine,
                               stack_idx, SD_SS_PREF_REAS_USER);
        break;
      }

      /* Force preference with reason user with additional action as Resume*/
      case CMMSC_ACTION_FORCE_USER_WITH_RESUME:
      {
        if(stack_idx != 0 && event_package->state_machine)
        {
          event_package->state_machine->stack[stack_idx].addl_action.action |=
            (uint8)MMOC_PREF_SYS_ADDL_ACTION_RESUME;
        }
        cmmsc_action_force_top(event_package->state_machine,
                               stack_idx, SD_SS_PREF_REAS_USER);
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Force preference with reason user */
      case CMMSC_ACTION_FORCE_TOP_EXT:
      {
        cmmsc_action_force_top_ext(event_package->state_machine,
                                   (cmmsc_event_force_pref_s_type *)event_package,
                                   stack_idx);
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Force phone preference with additional params ie. orig_mode */
      case CMMSC_ACTION_FORCE_PH_EXT:
      {
        cmmsc_action_force_ph_ext(event_package->state_machine,
                                  (cmmsc_event_force_pref_s_type *)event_package,
                                  stack_idx);
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Force preference with reason restore */
      case CMMSC_ACTION_FORCE_RESTORE:
      {
        cmmsc_action_force_top(event_package->state_machine,
                               stack_idx, SD_SS_PREF_REAS_RESTORE);
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Force preference with reason stack_sync_up_bsr */
      case CMMSC_ACTION_FORCE_STACK_SYNC_UP:
      {
        cmmsc_action_force_top(event_package->state_machine,
                               stack_idx, SD_SS_PREF_REAS_STACK_SYNC_UP);
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Force preference with reason stack_sync_up_bsr */
      case CMMSC_ACTION_FORCE_STACK_SYNC_UP_BSR:
      {
        cmmsc_action_force_top(event_package->state_machine,
                               stack_idx, SD_SS_PREF_REAS_STACK_SYNC_UP_BSR);
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /* Force preference with reason stack_sync_up_bsr
      ** and update_reason = SD_SS_PREF_UPDATE_REASON_HYBR_BSR_TO_HDR_END_SUCCESS
      */
      case CMMSC_ACTION_FORCE_STACK_SYNC_UP_BSR_END:
      {
        cmmsc_addl_pref_sys_chgd_mmoc_cmd_s_type  addl_info;
        sd_ss_pref_update_reason_e_type           pref_update_reas =
          SD_SS_PREF_UPDATE_REASON_HYBR_BSR_TO_HDR_END_SUCCESS;

        addl_info.pref_update_reas = &pref_update_reas;

        cmmsc_action_force_top_addl(event_package->state_machine, stack_idx,
                                    SD_SS_PREF_REAS_STACK_SYNC_UP_BSR, &addl_info);
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case CMMSC_ACTION_G_PS_DOMAIN_TRNSFR:
        break;

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case CMMSC_ACTION_LOCAL_DEACT:
        break;

      case CMMSC_ACTION_LOCAL_ACT:
        break;

      case CMMSC_ACTION_HICPS_REMOVE_3GPP:
      {
        cmmsc_action_hicps_remove_3gpp(event_package->state_machine, stack_idx,
                                       (cmmsc_event_acq_fail_s_type *)event_package);
        break;
      }

      case CMMSC_ACTION_HICPS_COND_KICK:
      {
        cmmsc_action_hicps_cond_kick(event_package->state_machine, stack_idx);
        break;
      }

      case CMMSC_ACTION_PENDING_KICK:
      {
        cmmsc_action_pending_kick(event_package->state_machine,
                                  stack_idx, FALSE);
        break;
      }

      case CMMSC_ACTION_SHORTEN_BSR:
      {
        cmmsc_action_shorten_bsr(event_package->state_machine, stack_idx);
        break;
      }

      case CMMSC_ACTION_RESTORE_3GPP:
      {
        cmmsc_action_restore_3gpp(event_package->state_machine, stack_idx);
        break;
      }

      case CMMSC_ACTION_SVLTE_REMOVE_3GPP:
      {
        cmmsc_action_svlte_remove_3gpp(event_package->state_machine, stack_idx);
        break;
      }

      case CMMSC_ACTION_SVLTE_STOP_STACK:
      {
        cmmsc_action_svlte_stop_stack(event_package->state_machine, stack_idx);
      }

      case CMMSC_ACTION_SVLTE_COND_KICK:
      {
        cmmsc_action_svlte_cond_kick(event_package->state_machine, stack_idx,
                                     (cmmsc_event_acq_fail_s_type *)event_package, FALSE);
        break;
      }

      case CMMSC_ACTION_SUB_CHG_COND_KICK:
      {
        cmmsc_action_svlte_cond_kick(event_package->state_machine, stack_idx,
                                     (cmmsc_event_acq_fail_s_type *)event_package, TRUE);
        break;
      }

      case CMMSC_ACTION_DEFER_3GPP:
      {
        cmmsc_action_defer_3gpp(event_package->state_machine, stack_idx);
        break;
      }

      case CMMSC_ACTION_ENABLE_3GPP:
      {
        cmmsc_action_enable_3gpp(event_package->state_machine, stack_idx,
                                 (cmmsc_param_srv_ind_s_type *)event_package);
        break;
      }

      case CMMSC_ACTION_DISABLE_3GPP:
      {
        cmmsc_action_disable_3gpp(event_package->state_machine, stack_idx,
                                  (cmmsc_param_srv_ind_s_type *)event_package);
        break;
      }

      case CMMSC_ACTION_SYNC_UP:
      {
        cmmsc_action_sync_up(event_package->state_machine, stack_idx,
                             (cmmsc_param_srv_ind_s_type *)event_package);
        break;
      }

      case CMMSC_ACTION_UPDATE_SRV:
      {
        cmmsc_action_update_srv(event_package->state_machine, stack_idx,
                                (cmmsc_param_srv_ind_s_type *)event_package);
        break;
      }

      case CMMSC_ACTION_UPDATE_NO_SRV:
      {
        cmmsc_action_update_no_srv(event_package->state_machine, stack_idx,
                                   (cmmsc_param_srv_ind_s_type *)event_package);
        break;
      }

      case CMMSC_ACTION_RESUME_SRV_ACQ:
      {
        cmmsc_action_resume_srv_acq(event_package->state_machine, stack_idx,
                                    (cmmsc_param_call_ind_s_type *)event_package);
        break;
      }

      case CMMSC_ACTION_KICK_HYBR2:
      {

        cmmsc_action_kick_hybr2(event_package->state_machine, stack_idx);
        break;
      }

      case CMMSC_ACTION_SUSPEND:
      {
        cmmsc_event_suspend_resume_s_type *evt_pkg = (cmmsc_event_suspend_resume_s_type *)event_package;
        cmmsc_action_suspend_stack(stack_idx, evt_pkg->state_machine);
        break;
      }

      case CMMSC_ACTION_RESUME:
      {
        cmmsc_event_suspend_resume_s_type *evt_pkg = (cmmsc_event_suspend_resume_s_type *)event_package;
        cmmsc_action_resume_stack(stack_idx, evt_pkg->state_machine);
        break;
      }

      case CMMSC_ACTION_SXLTE_KICK_NOT_AUTO:
      {
        cmmsc_action_sxlte_kick_not_auto(event_package->state_machine, stack_idx);
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case CMMSC_ACTION_MAX:
      default:
        break;
    }
  }
}

/*===========================================================================

FUNCTION cmmsc_update_state

DESCRIPTION
  This function update the current states to a new states in given
  routing table index.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_update_state(
  cmmsc_state_machine_s_type  *state_machine,
  int                          route_table_idx
)
{
  int                          stack_idx;
  cmmsc_routing_table_s_type  *route_table = cmmsc_route_table_ptr();
  boolean                     operation_flg_chg = FALSE;
  cmmsc_event_e_type             event = CMMSC_EVENT_NONE;

  for(stack_idx = 0; stack_idx < route_table->n_stack; stack_idx++)
  {
    cmmsc_stack_state_e_type new_state =
      cmmsc_get_route_tbl_stack_new_state(route_table,stack_idx,route_table_idx);

    if(new_state == CMMSC_STACK_STATE_NO_CHG)
    {
      continue;
    }
    if(state_machine->stack[stack_idx].state != new_state)
    {
      CM_MSG_HIGH_4_ASID("MSC: updating stack %d, state %d->%d",
                         state_machine->asubs_id,
                         stack_idx,
                         state_machine->stack[stack_idx].state,
                         new_state);
    }

    /* Update stack state */
    state_machine->stack[stack_idx].state = new_state;

    /* Update is_stack_operational flag base on the new state.
    ** cmmsc_update_cmss_is_operational() returns true if the
    ** operational states have toggled.
    */
    if(new_state == CMMSC_STACK_STATE_ACTIVE &&
        cmmsc_update_cmss_is_operational(state_machine, stack_idx, TRUE)
        && !operation_flg_chg)
    {
      operation_flg_chg = TRUE;
    }
    else if((new_state == CMMSC_STACK_STATE_INACTIVE
             || new_state == CMMSC_STACK_STATE_NULL) &&
            cmmsc_update_cmss_is_operational(state_machine, stack_idx, FALSE)
            && !operation_flg_chg)
    {
      operation_flg_chg = TRUE;
    }
  }

  /* If any of the stacks operational states have toggled send a single
  ** indication to PM that notifies the status after the event is processed.
  ** If event is CMMSC_EVENT_POWER_DOWN/CMMSC_EVENT_ONLINE, we send the notification
  ** in cmmsc_proc_cmd_oprt_mode_chgd(). Notifying PM about operational stack changes
  ** for other events can be moved a few levels up. However, that requires
  ** quite some refactoring which will likely be done in the future. For now,
  ** sticking to Online/power down events only.
  */
  event = cmmsc_get_route_tbl_event(route_table,route_table_idx);
  if(operation_flg_chg &&
      event != CMMSC_EVENT_POWER_DOWN 
       && event != CMMSC_EVENT_ONLINE)
  {
    cmpmprx_report_operational_radio_stacks(state_machine->asubs_id);
  }
}

/*===========================================================================

FUNCTION cmmsc_state_machine_find_route_idx

DESCRIPTION
  This function search for the Route Table index based on the current
  state and event.

DEPENDENCIES
  None.

RETURN VALUE
  Route table index.

SIDE EFFECTS
  None.
===========================================================================*/
int cmmsc_state_machine_find_route_idx(
  cmmsc_state_machine_s_type  *state_machine,
  cmmsc_event_e_type           event
)
{
  int                          col_idx, route_tbl_idx;
  cmmsc_routing_table_s_type  *routing_table = cmmsc_route_table_ptr();
  boolean                      is_match = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Validate input parameter */
  if(event == CMMSC_EVENT_NONE)
  {
    return -1;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* First- take the event and check for action */
  for(route_tbl_idx = 0; route_tbl_idx < routing_table->n_row; route_tbl_idx++)
  {
    /* Check for event && current mode. */
    if(cmmsc_get_route_tbl_event(routing_table,route_tbl_idx) == event &&
        cmmsc_compare_state_machine_op_mode(state_machine->op_mode,
                                            cmmsc_get_route_tbl_mode(routing_table, route_tbl_idx))
      )
    {
      /* If event matched then check for each individual states */
      for(col_idx = 0; col_idx < routing_table->n_stack; col_idx++)
      {
        if(!cmmsc_compare_stack_states(state_machine->stack[col_idx].state,
                                       cmmsc_get_route_tbl_stack_state(routing_table,col_idx,route_tbl_idx)))
        {
          break;
        }
      }

      if(col_idx == routing_table->n_stack)
      {
        is_match = TRUE;
        break;
      }
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If match, take action for each stack (this gonna be a different function) */
  if(is_match)
  {
    return route_tbl_idx;
  }

  return -1;
}

/*===========================================================================

FUNCTION cmmsc_init_mode_and_capability

DESCRIPTION
  Initialize the phone capability to default values.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void cmmsc_init_mode_and_capability(
  cmmsc_state_machine_s_type *state_machine
)
{
  state_machine->stack_common_info.ue_mode                = SYS_UE_MODE_NORMAL;
  state_machine->stack_common_info.ue_mode_forced         = TRUE;
  (void)cmpmprx_fetch_max_mode_capability(state_machine->asubs_id);

  state_machine->stack_common_info.ue_band_capability     = SD_SS_BAND_PREF_ANY;
  state_machine->stack_common_info.ue_lte_band_capability = SYS_LTE_BAND_MASK_CONST_ANY;
  state_machine->stack_common_info.ue_tds_band_capability = SD_SS_BAND_PREF_ANY;
  state_machine->stack_common_info.ue_mode_capability     = state_machine->stack_common_info.ue_max_mode_capability;
  state_machine->stack_common_info.ue_mode_capability_from_pm = state_machine->stack_common_info.ue_max_mode_capability;
  state_machine->stack_common_info.ue_domain_capability = SYS_SRV_DOMAIN_CS_PS;
  state_machine->stack_common_info.is_force_reg_pending   = FALSE;
  state_machine->stack_common_info.call_mode              = SYS_CALL_MODE_NORMAL;
  sd_ss_update_call_mode(state_machine->asubs_id, SYS_CALL_MODE_NORMAL);
  state_machine->stack_common_info.power_save_mode = FALSE;
  
}

/*===========================================================================

FUNCTION cmmsc_init_max_mode_capability

DESCRIPTION
  Get the phone capability from cmpmprx.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_init_max_mode_capability(void)
{
  {
    size_t i = 0;
    for(i = 0; i < cmpmprx_get_num_of_sims(); i++)
    {
      cmmsc_state_machine_s_type *state_machine = cmmsimc_state_machine_ptr((sys_modem_as_id_e_type)i);
      (void)cmpmprx_fetch_max_mode_capability(state_machine->asubs_id);
    }
  }
}

/*===========================================================================

FUNCTION cmmsc_get_policy_mode_band_cap

DESCRIPTION
  Retrieve the Mode, domain and Band capability from PMPRX module.
  fetch the call mode capability per subs and store that in stack_common_info as well.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_get_policy_mode_band_cap (
  cmmsc_state_machine_s_type    *state_machine,
  sys_sys_mode_mask_e_type      *mode,
  sys_band_mask_e_type          *band,
  sys_band_mask_e_type          *tds_band,
  sys_lte_band_mask_e_type      *lte_band,
  sys_srv_domain_e_type         *domain
)
{
  sys_call_mode_t   call_mode_buffer;
  sd_ss_mode_pref_e_type sd_mode_cap, sd_mode_pref;
  cmph_sub_info_s_type *ph_sub_ptr ;
  cmph_s_type *ph_ptr = cmph_ptr();

  CM_ASSERT(mode != NULL);
  CM_ASSERT(band != NULL);
  CM_ASSERT(tds_band != NULL);
  CM_ASSERT(lte_band != NULL);

  if(mode == NULL || band == NULL || tds_band == NULL || lte_band == NULL ||
      state_machine == NULL)
  {
    CM_ERR_3("MSC: band null ptr, band %d, tds_band %d, lte_band %d",
             band, tds_band, lte_band);
    return;
  }
  ph_sub_ptr = cmph_get_sub_ptr(state_machine->asubs_id);
  if(ph_sub_ptr == NULL )
  {
    return;
  }
  if(!cmpmprx_read_cm_policy_consumption_eligibility(state_machine->asubs_id))
  {
    state_machine->stack_common_info.ue_mode_capability
      = *mode = cmmsc_get_curr_msc_max_cap(state_machine->asubs_id);
    state_machine->stack_common_info.ue_mode_capability_from_pm = *mode;

    if (cmph_is_dual_lte())
    {
      cmmsc_set_domain_cap_dual_lte(state_machine);
      *domain = state_machine->stack_common_info.ue_domain_capability;
    }
    else
    {
      state_machine->stack_common_info.ue_domain_capability = *domain = SYS_SRV_DOMAIN_CS_PS;
    }
	
	sd_misc_get_mode_band_capability(NULL,(sd_ss_band_pref_e_type *)band,lte_band,(sd_ss_band_pref_e_type *)tds_band);

    state_machine->stack_common_info.ue_band_capability = *band;
	state_machine->stack_common_info.ue_lte_band_capability = *lte_band;
	state_machine->stack_common_info.ue_tds_band_capability = *tds_band;
    state_machine->stack_common_info.call_mode = SYS_CALL_MODE_NORMAL;
    state_machine->stack_common_info.power_save_mode = FALSE;
    state_machine->stack_common_info.non_dds_lte_removal = FALSE;

    CM_MSG_HIGH_2("MSC: GET_POLICY: Not eligible (sub unavail) - set default value on sub%d, cm_mode_cap=0x%x",
                  state_machine->asubs_id,
                  state_machine->stack_common_info.ue_mode_capability);
    return;
  }

  /* Do not get pm cap if device is in offline */
  if(cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_OFFLINE ||
      cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_OFFLINE_CDMA)
  {
    CM_MSG_HIGH_2("MSC: GET_POLICY: Not eligible since UE is OFFLINE. sub %d, cm_mode_cap=0x%x",
                  state_machine->asubs_id,
                  state_machine->stack_common_info.ue_mode_capability);
    *mode = state_machine->stack_common_info.ue_mode_capability;
    *domain = state_machine->stack_common_info.ue_domain_capability;
    *band     = state_machine->stack_common_info.ue_band_capability;
    *lte_band = state_machine->stack_common_info.ue_lte_band_capability;
    *tds_band = state_machine->stack_common_info.ue_tds_band_capability;
    return;
  }  

  if(cmpmprx_fetch_mode_capability(state_machine->asubs_id,mode) == TRUE)
  {
    state_machine->stack_common_info.ue_mode_capability = *mode;
    state_machine->stack_common_info.ue_mode_capability_from_pm = *mode;
  }
  else
  {
    state_machine->stack_common_info.ue_mode_capability = *mode =
      state_machine->stack_common_info.ue_mode_capability_from_pm;
  }

  /* For single standby CS invalid on non-dds sub, PM removes GW from capability.
  ** If mode preference and PM capability has no intersection, UE enters power save
  ** on the only active sub which is against regulation requirement.
  ** Overwriting PM capability with GWL so that for above scenario, UE enters limited mode.
  ** For the scenario where the only remaining capability is LTE and IMS is not enabled,
  ** overwrite PM capability with GW, UE enters limited mode.
  */
  sd_mode_cap = cmutil_map_sys_mode_mask_to_sd_mode_pref(*mode);
  sd_mode_pref = cmph_map_cm_mode_pref_to_sd_mode_pref(ph_sub_ptr->pref_info.mode_pref);
  if(cmss_get_gw_sim_state_per_subs(state_machine->asubs_id) == SYS_SIM_STATE_CS_INVALID &&
    ph_ptr->device_prop.msim_prop.default_data_subs != state_machine->asubs_id &&
    ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_SINGLE_STANDBY &&
    ph_sub_ptr->is_3gpp_subs_avail)
  {
    if(SD_GET_COMMON_MODE(sd_mode_cap, sd_mode_pref) == SD_SS_MODE_PREF_NONE)
    {
    CM_MSG_HIGH_2("MSC: sub %d overwriting current rat_cap 0x%x to GWL",
                    state_machine->asubs_id,
                    state_machine->stack_common_info.ue_mode_capability);
    state_machine->stack_common_info.ue_mode_capability = *mode =
      (SYS_SYS_MODE_MASK_GSM | SYS_SYS_MODE_MASK_WCDMA | SYS_SYS_MODE_MASK_LTE);
  }
    else if(SD_GET_COMMON_MODE(sd_mode_cap, sd_mode_pref) == SD_SS_MODE_PREF_LTE &&
            !cmsds_get_ims_enabled(state_machine->asubs_id))
    {
      CM_MSG_HIGH_2("MSC: sub %d overwriting current rat_cap 0x%x to GW",
                      state_machine->asubs_id,
                      state_machine->stack_common_info.ue_mode_capability);
      state_machine->stack_common_info.ue_mode_capability = *mode =
        (SYS_SYS_MODE_MASK_GSM | SYS_SYS_MODE_MASK_WCDMA);
    }
  }

  /* The sub will enter power save if LTE only and the sub is non dds and volte is off.
  ** otherwise set power save mode to be off.
  */
  if (!cmph_get_volte_state(state_machine->asubs_id) && 
     cmph_ptr()->device_prop.msim_prop.new_default_data_subs != state_machine->asubs_id &&
     cmmsc_is_lte_only_sub(state_machine->asubs_id) &&
     ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY
  )
  {
    state_machine->stack_common_info.power_save_mode = TRUE;
  } 
  else 
  {
    state_machine->stack_common_info.power_save_mode = FALSE;
  }

  if(cmsds_get_is_lte_to_be_removed(state_machine->asubs_id))
  {
    state_machine->stack_common_info.non_dds_lte_removal = TRUE;
  }
  else
  {
    state_machine->stack_common_info.non_dds_lte_removal = FALSE;
  }

  if(cmpmprx_fetch_domain_capability(state_machine->asubs_id,domain) == TRUE )
  {
    state_machine->stack_common_info.ue_domain_capability = *domain;
  }
  else
  {
    *domain = state_machine->stack_common_info.ue_domain_capability;
  }

  if(cmpmprx_get_band_capability(state_machine->asubs_id,
                                 band,
                                 lte_band,
                                 tds_band) == TRUE)
  {
    state_machine->stack_common_info.ue_band_capability = *band;
    state_machine->stack_common_info.ue_lte_band_capability = *lte_band;
    state_machine->stack_common_info.ue_tds_band_capability = *tds_band;
  }
  else
  {
    *band     = state_machine->stack_common_info.ue_band_capability;
    *lte_band = state_machine->stack_common_info.ue_lte_band_capability;
    *tds_band = state_machine->stack_common_info.ue_tds_band_capability;
  }

  if(cmpmprx_get_call_mode_capability(state_machine->asubs_id, &call_mode_buffer))
  {
    state_machine->stack_common_info.call_mode = call_mode_buffer;
    sd_ss_update_call_mode(state_machine->asubs_id, call_mode_buffer);
  }

  // This line is kept at the end of the function to ensure that before exiting we 
  // set service domain properly for VoLTE sub
  cmmsc_set_domain_cap_dual_lte(state_machine);
  *domain = state_machine->stack_common_info.ue_domain_capability;
}

/*===========================================================================

FUNCTION cmmsc_set_stack_capability

DESCRIPTION
  This function set stack capability. This function is called when we
  power-up or send subscriptions. Depending on which harware , we set
  different capability.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_set_stack_capability(
  cmmsc_state_machine_s_type *state_machine
)
{
  sys_subs_feature_t  feature_mode = SYS_SUBS_FEATURE_MODE_NORMAL;
  boolean             is_emergency;

  if(state_machine == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  feature_mode = cmph_get_subs_feature_mode(state_machine->asubs_id);

  is_emergency = state_machine->stack_common_info.is_in_emergency_call;

  /* Apply Feature Level Filtering per Each Sub */
  switch(feature_mode)
  {
    case SYS_SUBS_FEATURE_MODE_SVLTE:
    case SYS_SUBS_FEATURE_MODE_SRLTE:
    case SYS_SUBS_FEATURE_MODE_NORMAL:
      if(is_emergency)
      {
        cmmsc_set_stack_capability_auto_emergency(state_machine);
      }
      else
      {
        cmmsc_set_stack_capability_auto(state_machine);
      }
      break;

    default:
      CM_ERR_1("Invalid feature_mode", feature_mode);
  }
}

/*===========================================================================

FUNCTION cmmsc_pack_pref_info

DESCRIPTION
  This function packs pref info.

DEPENDENCIES
  cmpref2_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_pack_pref_info(
  cm_network_sel_mode_pref_e_type  network_sel_mode_pref,
  /* The network sel mode associated with the selected NAM */

  cm_mode_pref_e_type              mode_pref,
  /* The mode preference that is associated with the selected NAM.
  */

  cm_band_pref_e_type              band_pref,
  /* Band preference that is associated with the selected NAM.
  */

  sys_lte_band_mask_e_type         lte_band_pref,
  /* LTE band preference that is associated with the selected NAM.
  */

  cm_band_pref_e_type              tds_band_pref,
  /* TD-SCDMA band preference that is associated with the selected NAM.
  */

  cm_prl_pref_e_type               prl_pref,
  /* PRL preference that is associated with the selected NAM.
  */

  cm_roam_pref_e_type              roam_pref,
  /* Roam preference that is associated with the selected NAM.
  */

  cm_hybr_pref_e_type              hybr_pref,
  /* New hybrid preference (none, CDMA/HDR).
  */

  sys_plmn_id_s_type               plmn,
  /* The PLMN id to be acquired, valid when the net_sel_mode_pref
  ** is MANUAL.
  ** Applies only for GSM/WCDMA modes.
  */

  cm_srv_domain_pref_e_type        srv_domain_pref,
  /* Service domain preference that is associated with the selected
  ** NAM.
  ** Applies only for GSM/WCDMA modes.
  */

  cm_gw_acq_order_pref_e_type      acq_order_pref,
  /* Preference for the order of acquisition ( WCDMA before GSM,
  ** GSM before WCDMA etc).
  */

  cm_pref_s_type                  *pref_info
  /* Pointer to preference info stuct
  */
)
{
  /* Validate params */
  CM_ASSERT(pref_info != NULL);

  if(pref_info == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  /* Pack the pref info */
  pref_info->mode_pref = mode_pref;
  pref_info->network_sel_mode_pref = network_sel_mode_pref;
  pref_info->band_pref = band_pref;
  pref_info->lte_band_pref = lte_band_pref;
  pref_info->tds_band_pref = tds_band_pref;
  pref_info->prl_pref = prl_pref;
  pref_info->roam_pref = roam_pref;
  pref_info->hybr_pref = hybr_pref;
  pref_info->plmn = plmn;
  pref_info->srv_domain_pref = srv_domain_pref;
  pref_info->acq_order_pref = acq_order_pref;
}

/*===========================================================================

FUNCTION  cmmsc_pack_cmd_for_pref_sys_chgd_per_top_queue

DESCRIPTION
  Pack the param for cmd_pref_sys_chgd base on the preference of the top priority
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmmsc_pack_cmd_for_pref_sys_chgd_per_top_queue(

  mm_sub_stk_id_s_type                mm_id,


  sd_ss_pref_reas_e_type                pref_reas,
  /* Reason for changing the SS-Preference.
  */

  sd_ss_orig_mode_e_type                orig_mode,
  /* The origination mode (normal, OTASP, emergency, other, etc.)
  ** that is associated with the selected NAM.
  */

  boolean                               is_new_policy_tobe_read,
  /* Indicate if to update the policy */

  const cm_orig_q_s_type               *ph_orig_top_ptr,
  /* preference of the top priority queue.
  */

  cm_act_update_reas_e_type             act_update_reas,
  /* Reason for the update
  */

  cmmsc_proc_cmd_pref_sys_chgd_s_type  *cmd
)
{
  cm_preference_info_s_type  *pref_info = NULL;

  if(cmd == NULL)
  {
    CM_ERR_0("cmd ptr null");
    return;
  }

  if(ph_orig_top_ptr == NULL)
  {
    CM_ERR_0("queue_top_ptr null");
    return;
  }

  /* Basically, if policy is going to be used for forcing preferences, change ORIG_END to USER so that SD can trigger acq if required.*/
  if((pref_reas == SD_SS_PREF_REAS_ORIG_END) && is_new_policy_tobe_read)
  {
    pref_reas = SD_SS_PREF_REAS_USER;
  }

  cmd->mm_id = mm_id;

  cmd->is_new_policy_tobe_read = is_new_policy_tobe_read;

  /* Allocate memory for preference info */
  pref_info = (cm_preference_info_s_type *) cm_mem_malloc(
                sizeof(cm_preference_info_s_type));
  cmd->pref_info = pref_info;

  /* Set preference base on the input params */
  pref_info->pref_reas = pref_reas;
  pref_info->orig_mode = orig_mode;
  pref_info->act_update_reas = act_update_reas;

  /* Set preference base on default values */
  pref_info->otasp_band = cmutil_map_otasp_act_code_to_sd_band(CM_OTASP_ACT_CODE_NONE);
  pref_info->otasp_blksys = cmutil_map_otasp_act_code_to_sd_blksys(CM_OTASP_ACT_CODE_NONE);;
  pref_info->avoid_type = SD_SS_AVOID_SYS_IDLE;
  pref_info->avoid_time = 0;

  /* Set preference base on top priority queue */
  pref_info->mode_pref = ph_orig_top_ptr->orig->orig_mode_pref;
  pref_info->band_pref = ph_orig_top_ptr->orig->orig_band_pref;
  pref_info->lte_band_pref = ph_orig_top_ptr->orig->orig_lte_band_pref;
  pref_info->tds_band_pref = ph_orig_top_ptr->orig->orig_tds_band_pref;
  pref_info->hybr_pref = ph_orig_top_ptr->orig->orig_hybr_pref;
  pref_info->prl_pref = ph_orig_top_ptr->orig->orig_prl_pref;
  pref_info->roam_pref = ph_orig_top_ptr->orig->orig_roam_pref;
  pref_info->srv_domain_pref = ph_orig_top_ptr->orig->orig_srv_domain_pref;
  pref_info->csg_id = ph_orig_top_ptr->orig->csg_id;
  pref_info->csg_rat = ph_orig_top_ptr->orig->csg_rat;

  /* Set preference base on ph preference - those preference are not part of priority queue */
  pref_info->network_sel_mode_pref = CMPH_SS_NETWORK_SEL_MODE_PREF(mm_id.asubs_id, cmph_ptr());
  pref_info->camp_mode_pref
    = cmutil_map_cm_net_sel_mode_pref_to_camp_pref(
        pref_info->network_sel_mode_pref);
  pref_info->fullrat_status
    = cmpmprx_compute_fullrat_status(
        mm_id,pref_info->network_sel_mode_pref,cmpm_sub_ptr(mm_id.asubs_id));
  pref_info->rat_acq_order_ptr
    = cmph_get_acq_pri_order_per_sub(mm_id.asubs_id);
  pref_info->acq_order_pref = CMPH_SS_ACQ_ORDER_PREF(mm_id.asubs_id, cmph_ptr());
  pref_info->sys_id.sys_id.id_type = SYS_SYS_ID_TYPE_UMTS;
  pref_info->sys_id.sys_id.id.plmn = CMPH_SS_PLMN_PREF(mm_id.asubs_id, cmph_ptr());
  pref_info->sys_id.prm.gw.sel_type = SYS_MANUAL_SEL_TYPE_USER_SPECIFIED;

  pref_info->hybr1_pref.hybr_mode_pref = ph_orig_top_ptr->orig->orig_mode_pref;
  pref_info->hybr1_pref.hybr_orig_mode = orig_mode;

  /* retrieve if there is any CMSOA pref_update_reason  */
  pref_info->pref_update_reason = ph_orig_top_ptr->orig->pref_update_reason;
}

/*===========================================================================

FUNCTION  cmmsc_pack_cmd_for_pref_sys_chgd_per_ph_pref

DESCRIPTION
  Pack the param for cmd_pref_sys_chgd base on the phone preference.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmmsc_pack_cmd_for_pref_sys_chgd_per_ph_pref(
  sys_modem_as_id_e_type                asubs_id,

  multimode_stack_e_type                ss,
  /* Indicates the ss this pref_sys_chgd command is intended for.
  */

  sd_ss_pref_reas_e_type                pref_reas,
  /* Reason for changing the SS-Preference.
  */

  boolean                               is_new_policy_tobe_read,
  /* Indicate if to update the policy */

  cmmsc_proc_cmd_pref_sys_chgd_s_type  *cmd
)
{
  cm_preference_info_s_type   *pref_info = NULL;
  cmph_s_type                 *ph_ptr = cmph_ptr();
  mm_sub_stk_id_s_type         mm_id = cmph_map_sub_stk_to_id(asubs_id, ss);
  cm_orig_q_s_type            *ph_obj
    = cmtask_orig_para_search_act_id(mm_id, (cm_act_id_type)cmph_ptr());

  if(cmd == NULL)
  {
    CM_ERR_0("cmd ptr null");
    return;
  }

  cmd->mm_id.stk_id = ss;
  cmd->mm_id.asubs_id = asubs_id;
  cmd->is_new_policy_tobe_read = is_new_policy_tobe_read;

  /* Allocate memory for preference info */
  pref_info = (cm_preference_info_s_type *) cm_mem_malloc(
                sizeof(cm_preference_info_s_type));
  cmd->pref_info = pref_info;

  /* Set preference base on the input params */
  pref_info->pref_reas = pref_reas;

  /* Set preference base on phone pref */
  pref_info->network_sel_mode_pref = CMPH_SS_NETWORK_SEL_MODE_PREF(asubs_id, ph_ptr);
  pref_info->orig_mode = cmutil_map_cm_network_sel_mode_pref_to_orig_mode(
                           pref_info->network_sel_mode_pref);
  pref_info->mode_pref = CMPH_SS_MODE_PREF(asubs_id, ph_ptr);

  pref_info->hybr1_pref.hybr_mode_pref = pref_info->mode_pref;
  pref_info->hybr1_pref.hybr_orig_mode = pref_info->orig_mode;

  pref_info->hybr_pref = CMPH_SS_HYBR_PREF(asubs_id, ph_ptr);
  pref_info->band_pref = CMPH_SS_BAND_PREF(asubs_id, ph_ptr);
  pref_info->lte_band_pref = CMPH_SS_LTE_BAND_PREF(asubs_id, ph_ptr);
  pref_info->tds_band_pref = CMPH_SS_TDS_BAND_PREF(asubs_id, ph_ptr);
  pref_info->prl_pref = CMPH_SS_PRL_PREF(asubs_id, ph_ptr);
  pref_info->roam_pref = CMPH_SS_ROAM_PREF(asubs_id, ph_ptr);
  pref_info->srv_domain_pref = CMPH_SS_SRV_DOMAIN_PREF(asubs_id, ph_ptr);
  pref_info->acq_order_pref = CMPH_SS_ACQ_ORDER_PREF(asubs_id, ph_ptr);
  pref_info->rat_acq_order_ptr = cmph_get_acq_pri_order_per_sub(asubs_id);
  pref_info->network_sel_mode_pref = CMPH_SS_NETWORK_SEL_MODE_PREF(asubs_id, ph_ptr);
  pref_info->camp_mode_pref = cmutil_map_cm_net_sel_mode_pref_to_camp_pref(
                                pref_info->network_sel_mode_pref);
  pref_info->fullrat_status = cmpmprx_compute_fullrat_status(
                                mm_id,pref_info->network_sel_mode_pref,cmpm_sub_ptr(mm_id.asubs_id));
  pref_info->sys_id.sys_id.id.plmn = CMPH_SS_PLMN_PREF(asubs_id, ph_ptr);
  pref_info->sys_id.sys_id.id_type = SYS_SYS_ID_TYPE_UMTS;
  pref_info->sys_id.prm.gw.sel_type = SYS_MANUAL_SEL_TYPE_USER_SPECIFIED;

  /* Set preference base on default values */
  pref_info->otasp_band = cmutil_map_otasp_act_code_to_sd_band(CM_OTASP_ACT_CODE_NONE);
  pref_info->otasp_blksys = cmutil_map_otasp_act_code_to_sd_blksys(CM_OTASP_ACT_CODE_NONE);;
  pref_info->avoid_type = SD_SS_AVOID_SYS_IDLE;
  pref_info->avoid_time = 0;
  if( ph_ptr != NULL && ph_ptr->sub_info[asubs_id] != NULL )
  {
    pref_info->csg_id = ph_ptr->sub_info[asubs_id]->pref_info.csg_id;
    pref_info->csg_rat =  ph_ptr->sub_info[asubs_id]->pref_info.csg_rat;
  }
  if (ph_obj != NULL)
  {
    pref_info->pref_update_reason = ph_obj->orig->pref_update_reason;
  }
  else
  {
    pref_info->pref_update_reason = SD_SS_PREF_UPDATE_REASON_NONE;
  }
}

/*===========================================================================

FUNCTION  cmmsc_pack_cmd_for_pref_sys_chgd_per_pref

DESCRIPTION
  Pack the param for cmd_pref_sys_chgd base on the given preference .

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmmsc_pack_cmd_for_pref_sys_chgd_per_pref(
  mm_sub_stk_id_s_type                mm_id,
  /* Indicates the ss this pref_sys_chgd command is intended for.
  */

  sd_ss_pref_reas_e_type                pref_reas,
  /* Reason for changing the SS-Preference.
  */

  sd_ss_orig_mode_e_type                orig_mode,
  /* The origination mode (normal, OTASP, emergency, other, etc.)
  ** that is associated with the selected NAM.
  */

  const cm_pref_s_type                 *ss_pref,
  /* Stack preference that is associated with the selected NAM.
  */

  sd_band_e_type                        otasp_band,
  /* The band-class that is associated with OTASP origination mode.
  ** Note that this parameter is ignored if orig_mode != OTASP.
  */

  sd_blksys_e_type                      otasp_blksys,
  /* The PCS frequency block/Cellular system that is associated
  ** with OTASP origination mode. Note that this parameter is ignored
  ** if orig_mode != OTASP.
  */

  sd_ss_avoid_sys_e_type                avoid_type,
  /* Type of the system to be avoided, valid only
  ** if pref_reason = SD_SS_PREF_REAS_AVOID_SYS.
  */

  dword                                 avoid_time,
  /* Time in seconds for which the system is to be avoided, valid
  ** only if pref_reason = SD_SS_PREF_REAS_AVOID_SYS.
  */

  const cm_acq_pri_order_pref_s_type   *rat_acq_order_ptr,
  /* Rat acquisition priority order
  */

  sd_ss_pref_camp_mode_e_type           camp_mode_pref,
  /* camp only pref
  */

  boolean                               is_new_policy_tobe_read,

  cm_act_update_reas_e_type             act_update_reas,
  /* Reason for the update
  */
  sd_ss_fullrat_pref_e_type             fullrat_status,
  /* Indication to SD whether ue is in full rat
   */

  cmmsc_proc_cmd_pref_sys_chgd_s_type  *cmd
)
{
  cm_preference_info_s_type  *pref_info = NULL;

  if(cmd == NULL)
  {
    CM_ERR_0("cmd ptr null");
    return;
  }

  cmd->mm_id = mm_id;
  cmd->is_new_policy_tobe_read = is_new_policy_tobe_read;

  /* Allocate memory for preference info */
  pref_info = (cm_preference_info_s_type *) cm_mem_malloc(
                sizeof(cm_preference_info_s_type));
  cmd->pref_info = pref_info;

  pref_info->pref_reas  = pref_reas;
  pref_info->orig_mode  = orig_mode;
  pref_info->otasp_band = otasp_band;
  pref_info->otasp_blksys = otasp_blksys;
  pref_info->avoid_type   = avoid_type;
  pref_info->avoid_time   = avoid_time;
  pref_info->rat_acq_order_ptr = rat_acq_order_ptr;
  pref_info->camp_mode_pref    = camp_mode_pref;
  pref_info->fullrat_status     = fullrat_status;
  pref_info->act_update_reas   = act_update_reas;

  if(ss_pref)
  {
    pref_info->pref_update_reason = ss_pref->pref_update_reason;
    pref_info->mode_pref = ss_pref->mode_pref;
    pref_info->band_pref = ss_pref->band_pref;
    pref_info->lte_band_pref = ss_pref->lte_band_pref;
    pref_info->tds_band_pref = ss_pref->tds_band_pref;
    pref_info->hybr_pref = ss_pref->hybr_pref;
    pref_info->prl_pref = ss_pref->prl_pref;
    pref_info->roam_pref = ss_pref->roam_pref;
    pref_info->acq_order_pref = ss_pref->acq_order_pref;
    pref_info->srv_domain_pref = ss_pref->srv_domain_pref;
    pref_info->network_sel_mode_pref = ss_pref->network_sel_mode_pref;
    pref_info->sys_id.sys_id.id_type = SYS_SYS_ID_TYPE_UMTS;
    pref_info->sys_id.sys_id.id.plmn  = ss_pref->plmn;
    pref_info->sys_id.prm.gw.sel_type = SYS_MANUAL_SEL_TYPE_USER_SPECIFIED;
    pref_info->hybr1_pref.hybr_mode_pref = ss_pref->mode_pref;
    pref_info->csg_id = ss_pref->csg_id;
    pref_info->csg_rat = ss_pref->csg_rat;
  }

  pref_info->hybr1_pref.hybr_orig_mode = orig_mode;
}

/*===========================================================================

FUNCTION cmmsc_proc_cmd_oprt_mode_chgd

DESCRIPTION

DEPENDENCIES
  cmpref_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_proc_cmd_oprt_mode_chgd(
  cmmsc_param_oprt_mode_s_type   *oprt_mode_event
  /* New operating mode of the phone. */
)
{
  sys_oprt_mode_e_type           oprt_mode;
  cmmsc_state_machine_s_type    *state_machine;
  cmmsc_event_e_type             event = CMMSC_EVENT_NONE;
  cmmsc_event_oprt_mode_s_type   event_oprt_mode;
  cmph_s_type                   *ph_ptr = cmph_ptr();
  sys_modem_as_id_e_type         sub;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Set parameters */
  if(oprt_mode_event == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  oprt_mode = oprt_mode_event->oprt_mode;
  state_machine = oprt_mode_event->state_machine;

  if (!IS_VALID_PH_SUB_PTR(state_machine->asubs_id))
  {
    return;
  }

#if (defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)) || defined(FEATURE_MMODE_DYNAMIC_SV_OPERATION_SWITCH)
  if(cmph_is_subs_feature_mode_srlte(state_machine->asubs_id))
  {
    cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(state_machine->asubs_id);
    if(cmsoa_state != NULL)
    {
      CM_MSG_HIGH_4_ASID("oprt_chgd: curr_sv_oprt_mode=%d, is_pwrup_from_last_submode = %d, msc rat_cap 0x%x",
                         state_machine->asubs_id,
                         cmsoa_state->config.curr_sv_oprt_mode,
                         cmsoa_state->config.is_pwrup_from_last_submode,
                         state_machine->stack_common_info.ue_mode_capability);
    }
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Terminate any pending get network request. */
  for (sub = SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(sub); sub ++)
  {
    cmph_term_pending_get_networks_req_client_only(
      cmph_map_sub_stk_to_id(sub, MM_STACK_ALL));

    /* Reset is_kicked_hybr_2 */
    *cmph_get_is_kicked_hybr_2_ptr() = FALSE;
    cmmsc_auto_update_is_kick_hdr_pending(state_machine, FALSE);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch(oprt_mode)
  {
    case SYS_OPRT_MODE_ONLINE:
    case SYS_OPRT_MODE_PSEUDO_ONLINE:
    {
      event = CMMSC_EVENT_ONLINE;

      /* if oprt mode is online, send call mode ind to clients */
      if(oprt_mode == SYS_OPRT_MODE_ONLINE)
      {
        uint8 subs_id = SYS_MODEM_AS_ID_1;

        for(subs_id = 0; subs_id < cmpmprx_get_num_of_sims(); subs_id++)
        {
          sys_call_mode_t temp_call_mode_buf = SYS_CALL_MODE_INVALID;
          if(cmpmprx_fetch_call_mode_capability(subs_id, &temp_call_mode_buf))
          {
            cmemg_send_call_mode_ind(subs_id, temp_call_mode_buf, TRUE);
            sd_ss_update_call_mode(subs_id, temp_call_mode_buf);
            cmmsc_set_stack_common_call_mode_per_subs(temp_call_mode_buf, subs_id);
          }
        }
      }

      /* Update op mode per feature mode and set special event to disable hybr1 */
      if(cmmsc_get_1x_sub() == state_machine->asubs_id &&
          cmmsc_auto_online_disable_hybr1(oprt_mode,
                                          ph_ptr->sub_info[state_machine->asubs_id]->pref_info.mode_pref,
                                          ph_ptr->sub_info[state_machine->asubs_id]->sub_3gpp2_pref.int_hybr_pref)
        )
      {
        cmmsc_auto_update_is_hybr1_disabled(state_machine, TRUE);
        event = CMMSC_EVENT_ONLINE_DISABLE_HYBR1;
      }

      break;
    }

    default:
    {
      state_machine->stack_common_info.is_in_emergency_call = FALSE;
      event = CMMSC_EVENT_POWER_DOWN;
      state_machine->stack_common_info.ue_mode_forced = FALSE;
      break;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Setting stacks capability */
  cmmsc_set_stack_capability(state_machine);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Package the event with parameters */
  event_oprt_mode.event = event;
  event_oprt_mode.state_machine = state_machine;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Process in Routing Table */
  cmmsc_state_machine_process(state_machine,
                              (cmmsc_event_base_s_type *)&event_oprt_mode);

  /*For POWER_DOWN/ONLINE event, once the event has been processed,send a
  **notification to PM with the operational stack changes if any.
  */
  if(CMMSC_EVENT_POWER_DOWN==event_oprt_mode.event
      || CMMSC_EVENT_ONLINE==event_oprt_mode.event)
  {
    cmpmprx_report_operational_radio_stacks(state_machine->asubs_id);
  }
}

/*===========================================================================

FUNCTION cmmsc_proc_cmd_suspend_resume

DESCRIPTION

DEPENDENCIES
  cmmsc_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_proc_cmd_suspend_resume(
  cmmsc_param_suspend_resume_subs_s_type   *suspend_resume_event
)
{
  cmmsc_state_machine_s_type         *state_machine;
  cmmsc_event_e_type                  event = CMMSC_EVENT_NONE;
  cmmsc_event_suspend_resume_s_type   event_suspend_resume;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(suspend_resume_event == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  state_machine = suspend_resume_event->state_machine;

  if(suspend_resume_event->asubs_id != state_machine->asubs_id)
  {
    return;
  }

  CM_MSG_HIGH_4("SUSPEND_RESUME: stat_machine asubs_id %d, event asubs_id=%d, suspend=%d, op_mode %d",
                   state_machine->asubs_id,
                   suspend_resume_event->asubs_id,
                   suspend_resume_event->suspend,
                   state_machine->op_mode);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(suspend_resume_event->suspend)
  {
    event = CMMSC_EVENT_SUSPEND;
  }
  else
  {
    event = CMMSC_EVENT_RESUME;
  }
  /* Package the event with parameters */
  event_suspend_resume.event = event;
  event_suspend_resume.state_machine = state_machine;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Process in Routing Table */
  cmmsc_state_machine_process(state_machine,
                              (cmmsc_event_base_s_type *)&event_suspend_resume);
}

cm_mode_band_pref_s_type cmmsc_current_stack_preference(
  cmmsc_state_machine_s_type  *state_machine
)
{
  cm_mode_band_pref_s_type   stack_pref;

  if(state_machine == NULL)
  {
    CM_ERR_FATAL_0("state machine NULL");
  }
  else
  {
    /* Update preference for that stack */
    stack_pref.mode_pref = state_machine->stack_common_info.preference.mode_pref;
    stack_pref.band_pref = state_machine->stack_common_info.preference.band_pref;
    stack_pref.lte_band_pref = state_machine->stack_common_info.preference.lte_band_pref;
    stack_pref.tds_band_pref = state_machine->stack_common_info.preference.tds_band_pref;
    /*
    CM_MSG_HIGH_5("cmmsc_current_stack_preference(): mode_pref %d, band_pref 0x%08x %08x, tds_band_pref 0x%08x %08x",
      state_machine->stack_common_info.preference.mode_pref,
      QWORD_HIGH(state_machine->stack_common_info.preference.band_pref),
      QWORD_LOW(state_machine->stack_common_info.preference.band_pref),
      QWORD_HIGH(state_machine->stack_common_info.preference.tds_band_pref),
      QWORD_LOW(state_machine->stack_common_info.preference.tds_band_pref));

    cm_print_lte_band_mask(state_machine->stack_common_info.preference.lte_band_pref);
    */
  }

  return stack_pref;
}

/*===========================================================================

FUNCTION cmmsc_proc_cmd_pref_sys_chgd

DESCRIPTION
  This function should be called when the phone preferences have changed.
  This module will be responsible for queing pref_sys_chgd command to MMOC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_proc_cmd_pref_sys_chgd(
  cmmsc_param_pref_sys_s_type   *pref_sys_param
)
{
  cmmsc_proc_cmd_pref_sys_chgd_s_type  *cmd = NULL;
  cmmsc_state_machine_s_type           *state_machine = NULL;

  cmmsc_event_e_type                  event = CMMSC_EVENT_SYS_SEL_PREF_MAX;
  cmmsc_event_ss_pref_s_type          event_ss_pref;
  cmmsc_state_machine_op_mode_e_type  old_op_mode;
  boolean                             is_new_policy_tobe_read = 0;
  boolean                             is_emerg_call_actv = FALSE;
  sd_ss_pref_camp_mode_e_type         camp_mode_pref = SD_SS_PREF_CAMP_MODE_NONE;
  cm_preference_info_s_type           *pref_info = NULL;
  multimode_stack_e_type              stk_id;
  mm_sub_stk_id_s_type                mm_id;
  sys_subs_feature_t                  subs_feature_mode;
  cm_hybr_pref_e_type                 hybr_pref = CM_HYBR_PREF_NONE;
  sd_ss_orig_mode_e_type              orig_mode = SD_SS_ORIG_MODE_NONE;
  cm_orig_q_s_type                    *ph_orig_top_ptr     = NULL;
  /* Stack preference that is associated with the selected NAM.
  */
  cmph_s_type                        *ph_ptr = cmph_ptr();
  cmph_sub_info_s_type               *ph_sub_ptr = NULL;
  boolean                             old_pwr_save_mode = FALSE;
  boolean                             is_srv_mode_rat_cap_chg = FALSE;
  boolean                             is_cmph_cap_update_required = FALSE;
  boolean                             old_lte_removal = FALSE;
  boolean                             is_lte_removal_chgd = FALSE;
  cmmsimc_state_s_type  *cmmsimc_state = cmmsimc_state_ptr();
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Validate parameters */
  if(pref_sys_param == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  cmd = pref_sys_param->cmd;
  state_machine = pref_sys_param->state_machine;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(cmd == NULL || state_machine == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }
  
  /* Get the params from the cmd */
  is_new_policy_tobe_read = cmd->is_new_policy_tobe_read;
  mm_id = cmd->mm_id;
  stk_id = mm_id.stk_id;

  pref_info = cmd->pref_info;
  if(pref_info == NULL)
  {
    CM_ERR_0("pref info null ptr");
    return;
  }

  ph_sub_ptr = cmph_get_sub_ptr(state_machine->asubs_id);

  if (!ph_sub_ptr)
  {
    return;
  }

  /* Check if ss is valid per op mode and hybr pref */
  hybr_pref = pref_info->hybr_pref;
  if(!cmmsc_is_valid_ss_per_op_mode(state_machine, stk_id,hybr_pref))
  {
    return;
  }

  if (ph_ptr->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_FTM)
  {
    CM_MSG_HIGH_2_ASID("pref_chgd: stk_id=%d, not send pref in FTM oprt_mode",
                       state_machine->asubs_id, stk_id);
    return;
  }

  CM_MSG_HIGH_6_ASID_EXT("pref_chgd: stk_id=%d, pref mode_pref=%d, is_new_policy_tobe_read=%d, hybr_pref=%d, operator=%d",
                         state_machine->asubs_id, stk_id, pref_info->mode_pref,
                         is_new_policy_tobe_read, hybr_pref,
                         cm_get_operator_info(state_machine->asubs_id),
                         (state_machine->asubs_id+1));

  camp_mode_pref = pref_info->camp_mode_pref;
  orig_mode = pref_info->orig_mode;
  old_pwr_save_mode = state_machine->stack_common_info.power_save_mode;
  old_lte_removal = state_machine->stack_common_info.non_dds_lte_removal;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* For SVLTE, we might have to check and determine the UE by CM.
  */

  /* Save the previous ue mode.  This will be used later to determine */
  old_op_mode = state_machine->op_mode;

  if (is_new_policy_tobe_read)
  {
    is_srv_mode_rat_cap_chg = 
      cmpmprx_is_mask_set( state_machine->asubs_id, 
                             (PM_CHG_SVC_SCAN|PM_CHG_MODE_CAP));
    CM_MSG_HIGH_5_ASID("pref_chgd: stk_id=%d, is_srv_mode_rat_cap_chg %d, srv_mode_chgd %d, mode_cap_chgd %d",
                         state_machine->asubs_id, stk_id, 
                         is_srv_mode_rat_cap_chg,
                         cmpmprx_is_mask_set(state_machine->asubs_id, PM_CHG_SVC_SCAN),
                         cmpmprx_is_mask_set(state_machine->asubs_id, PM_CHG_MODE_CAP));
  }

  /* Determine op mode and set stack cap */
  if(is_new_policy_tobe_read || stk_id == MM_STACK_ALL)
  {
    /* Print ph obj top information for debug purpose */
    cm_orig_q_s_type *ph_orig_top_ptr = cmtask_orig_para_get_top(mm_id);

    /* Update op_mode per latest preference and ue_mode */
    if(cmmsc_auto_update_op_mode(
          state_machine,
          is_new_policy_tobe_read,
          FALSE,
          cmph_map_cm_mode_pref_to_sd_mode_pref(pref_info->mode_pref),
          cmutil_map_cm_hybr_pref_to_sd_hybr_pref(hybr_pref),
          cmph_ptr()->sub_info[state_machine->asubs_id]->is_3gpp_subs_avail,
          cmph_ptr()->sub_info[state_machine->asubs_id]->is_3gpp2_subs_avail,
          pref_info))
    {
      cmph_sp_subs_info_update();
    }

    cmmsc_set_stack_capability_auto(state_machine);
  }

  /* Set capability during entire duration of of emerg call */
   if( cmcall_is_there_cellular_call_per_sub(
          state_machine->asubs_id,
          CM_CALL_TYPE_EMERG_MASK, TRUE) != CM_CALL_ID_INVALID 
      || 
       cmph_is_emerg_pending() ||
      (cmemg_get_reason_per_sub(state_machine->asubs_id) 
          == CM_AC_REASON_THIRD_PARTY_IMS_E911 
           &&
       !(stk_id == MM_STACK_ALL && 
         pref_info->pref_reas == SD_SS_PREF_REAS_ORIG_END &&
         pref_info->orig_mode == SD_SS_ORIG_MODE_NONE)
      )
   )
   {
     is_emerg_call_actv = TRUE;
   }

  if(is_emerg_call_actv ||
      (state_machine->stack_common_info.is_in_emergency_call != is_emerg_call_actv))
  {
    state_machine->stack_common_info.is_in_emergency_call = is_emerg_call_actv;

    /* If emerg orig req is on a specific SS, then don't do special mode setting */
    if(!(is_emerg_call_actv && cmmsc_is_msc_multi_stacks(state_machine->asubs_id)))
    {
      cmmsc_set_stack_capability(state_machine);
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  subs_feature_mode = cmph_get_subs_feature_mode(state_machine->asubs_id);

  /* For operation mode LPM/FTM, we also allow sys_sel_pref to go through
     Override pref_reas to update SD pref but not invoke SD's engine */
  if((ph_ptr->device_prop.ph_oprt_mode.oprt_mode != SYS_OPRT_MODE_ONLINE ||
      ph_ptr->device_prop.ph_oprt_mode.oprt_mode_send_time != CMPH_MAX_UPTIME)
     && 
     !(cmph_is_emerg_pending()
       && pref_info->pref_reas == SD_SS_PREF_REAS_ORIG_START_CS
       && pref_info->orig_mode == SD_SS_ORIG_MODE_EMERG_ORIG
     )
    )
  {
    pref_info->pref_reas = SD_SS_PREF_REAS_RESTORE;

    event = cmmsc_set_sys_sel_pref_event(state_machine->asubs_id, stk_id,
                                         old_op_mode, state_machine->op_mode);
  }
  else if(stk_id == MM_STACK_ALL)
  {
    if(!cmmsc_force_all_stack_required(old_op_mode, state_machine->op_mode))
    {
      event = CMMSC_EVENT_SYS_SEL_PREF_MAIN;
    }
    /* If volte call is active, not force user pref on hybr2 */
    else if(cmmsc_is_volte_active_on_hybr2(state_machine) &&
            orig_mode == SD_SS_ORIG_MODE_NONE)
    {
      event = CMMSC_EVENT_SYS_SEL_PREF_MAIN_HYBR1;
    }
    else
    {
      /* AMEYA: In non-SXLTE, it leads to wrong event and wrong route from routing table. */
      event = CMMSC_EVENT_SYS_SEL_PREF_MAX;// cmmsc_set_sys_sel_pref_event(state_machine->asubs_id, stk_id);
    }
  }
  else
  {
    event = cmmsc_set_sys_sel_pref_event(state_machine->asubs_id, stk_id,
                                         old_op_mode, state_machine->op_mode);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If ss is not active, force pref_reason to restore */
  if(cmph_is_msim()
      && !(cmmsimc_state->dev_info.active_subs & BM(state_machine->asubs_id))
      && pref_info->pref_reas != SD_SS_PREF_REAS_ORIG_END)
  {
    pref_info->pref_reas = SD_SS_PREF_REAS_RESTORE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the top of the priority queue */
  ph_orig_top_ptr = cmtask_orig_para_get_top(mm_id);

  if(ph_orig_top_ptr == NULL)
  {
    CM_ERR_0("MSC: ph_orig_top_ptr null ptr");
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmmsc_update_stack_preference(state_machine,
                                pref_info->mode_pref,
                                pref_info->band_pref,
                                pref_info->lte_band_pref,
                                pref_info->tds_band_pref);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Store the current value of is_cap_update_required which may have already
  ** been evaluated in CMPH
  */
  is_cmph_cap_update_required = ph_sub_ptr->ps_pref.is_cap_update_required;

  if(old_pwr_save_mode != state_machine->stack_common_info.power_save_mode ||
     old_lte_removal != state_machine->stack_common_info.non_dds_lte_removal)
  {
    is_lte_removal_chgd = TRUE;
  }

  /* Check if CMMSC capability update evaluation needs to be performed
  */
  if(cmmsc_is_cap_update_eval_required(is_new_policy_tobe_read,
                                       is_srv_mode_rat_cap_chg,
                                       is_lte_removal_chgd,
                                       mm_id,
                                       state_machine,
                                       pref_info))
  {
    /* Perform CMMSC capability update evaluation.
    ** is_cap_update_required and is_tau_cap_update_required will be determined
    */
    cmmsc_auto_evaluate_cap_update_required_sxlte(state_machine);
  }

  /* Determine if the capability update is needed due to a policy change
  */
  if(is_new_policy_tobe_read &&
     is_cmph_cap_update_required == FALSE &&
     ph_sub_ptr->ps_pref.is_user_ps_detach_required == FALSE &&
     ph_sub_ptr->ps_pref.is_cap_update_required == TRUE)
  {
    ph_sub_ptr->ps_pref.is_policy_chg_cap_update = TRUE;
  }

  /* Inform IMS to de-register if the following conditions are met:
  ** 1. capability update is needed due to a policy change
  ** 2. a PS detach is needed for the capability update (TAU cannot be used)
  ** 3. IMS has not been informed to de-register yet
  */
  if(ph_sub_ptr->ps_pref.is_policy_chg_cap_update &&
     !ph_sub_ptr->ps_pref.is_tau_cap_update_required &&
     ph_sub_ptr->ps_pref.dereg_is_sent == FALSE)
  {
    #if defined(FEATURE_IP_CALL)
    cmipapp_send_srv_domain(CM_PS_DETACH_BY_MMODE_TRIGGER, FALSE, state_machine->asubs_id);
    #endif
  }
#if defined(FEATURE_IP_CALL)
  /* Inform IMS to de-register for the scenario:
  ** UE in SRLTE mode, LTE in service, Volte off->on.
  ** PM capability changes to SRLTE not supported mode (e.g. CHL->LTE only)
  ** CM should inform IMS to de-register. Then when stack 2 is turned off,
  ** CM should inform IMS detach complete so that
  ** IMS can start re-register.
  */
  else if(stk_id == MM_STACK_ALL &&
    ph_sub_ptr->volte_sub_info.is_volte_on_pending &&
    sys_srv_status_is_full_srv(cmss_get_info_srv_status(state_machine->asubs_id, MM_STACK_2)) &&
    !cmmsc_auto_is_hybr2_on(state_machine->op_mode) &&
    cmmsc_auto_is_hybr2_on(state_machine->prev_op_mode))
  {
    ph_sub_ptr->volte_sub_info.is_volte_on_pending = FALSE;
    cmipapp_send_srv_domain(CM_PS_DETACH_BY_MMODE_TRIGGER, FALSE, state_machine->asubs_id);
    ph_sub_ptr->volte_sub_info.is_ps_detach_comp_pending = TRUE;
  }
  else if(stk_id == MM_STACK_ALL)
  {
    ph_sub_ptr->volte_sub_info.is_volte_on_pending = FALSE;
    if(ph_sub_ptr->volte_sub_info.is_ps_detach_comp_pending)
    {
      cmph_send_detach_comp(state_machine->asubs_id);
    }
  }
#endif

  /* If cm has notified IMS to do dereg for PS detach but has later computed that
  ** PS detach does not need to be sent to NAS, CM needs to send PS detach complete
  ** to IMS so that IMS can re-reg
  */
  if(ph_sub_ptr->ps_pref.dereg_is_sent)
  {
    if(ph_sub_ptr->ps_pref.is_cap_update_required != TRUE &&
       ph_sub_ptr->ps_pref.is_user_ps_detach_required != TRUE)
    {
      cmph_send_detach_comp(state_machine->asubs_id);
    }
    /* Reset the flag */
    cmph_ims_dereg_is_sent(state_machine->asubs_id, FALSE);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* if all the conditions meet, restore 3GPP back to phone object
  */
  if(is_new_policy_tobe_read)
  {
    cmmsc_auto_restore_3gpp_policy_chgd(state_machine, mm_id,
                                        &(pref_info->mode_pref));
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmmsc_auto_set_hybr1_pref(state_machine, old_op_mode, pref_info);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Package the event with parameters */
  event_ss_pref.event = event;
  event_ss_pref.state_machine = state_machine;
  event_ss_pref.cmd = cmd;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Consult Routing Table and return action */
  cmmsc_state_machine_process(state_machine,
                              (cmmsc_event_base_s_type *)&event_ss_pref);

}

/*===========================================================================

FUNCTION cmmsc_is_cap_update_eval_required

DESCRIPTION
  The function checks if capability update evaluation is required

DEPENDENCIES
  None

RETURN VALUE
  TRUE if capability update evaluation is required
  FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean cmmsc_is_cap_update_eval_required(
  boolean                      is_new_policy_tobe_read,
  boolean                      is_srv_mode_rat_cap_chgd,
  boolean                      is_pwr_save_mode_chgd,
  mm_sub_stk_id_s_type         mm_id,
  cmmsc_state_machine_s_type  *state_machine,
  cm_preference_info_s_type   *pref_info
)
{
  cmph_s_type       *ph_ptr = cmph_ptr();
  cm_orig_q_s_type  *ph_orig_top_ptr = cmtask_orig_para_get_top(mm_id);
  sys_modem_as_id_e_type asubs_id;
  cmph_sub_info_s_type *ph_sub_ptr;

  if(state_machine == NULL)
  {
    CM_ERR_0("PS_DETACH_EVAL: state_machine NULL");
    return FALSE;
  }
  if(pref_info == NULL)
  {
    CM_ERR_0("PS_DETACH_EVAL: pref_info NULL");
    return FALSE;
  }

  asubs_id = state_machine->asubs_id;
  ph_sub_ptr = cmph_get_sub_ptr(asubs_id);

  if (!cmutil_is_as_id_valid(asubs_id) || ph_sub_ptr == NULL)
  {
    return FALSE;
  }

  if(ph_orig_top_ptr == NULL)
  {
    CM_ERR_0("PS_DETACH_EVAL: ph_orig_top_ptr NULL");
    return FALSE;
  }

  if(ph_ptr->device_prop.ph_oprt_mode.oprt_mode != SYS_OPRT_MODE_ONLINE ||
    !ph_sub_ptr->is_3gpp_subs_avail || 
    mm_id.stk_id == MM_STACK_1 
  )
  {
    return FALSE;
  }

  /* Evaluate capability update only if the top of the priority queue
  ** is the phone object or top is data call and policy changed
  */
  if(ph_orig_top_ptr->act_type != CM_ACT_TYPE_PH_OBJ
    &&
     !(ph_orig_top_ptr->act_type == CM_ACT_TYPE_DATA_CALL &&
       is_new_policy_tobe_read &&
       is_srv_mode_rat_cap_chgd)
  )
  {
    return FALSE;
  }

  /* Conditions to evaluate if a capability update is required:
     1. UE is online
     2. GW subscription is available
     3. pref_reason is SD_SS_PREF_REAS_USER
     4. Either policy change or preference change due to CMSOA or user
     5. For SRLTE/SRLTE+G, sub-mode switch within home should
        not trigger evaluation
  */
  if( pref_info->pref_reas == SD_SS_PREF_REAS_USER
     &&
     (is_new_policy_tobe_read || is_pwr_save_mode_chgd || mm_id.stk_id == MM_STACK_ALL ||
     ((cmph_is_subs_feature_mode_svlte(asubs_id)) &&
        (pref_info->pref_update_reason == SD_SS_PREF_UPDATE_REASON_SWITCH_SVLTE ||
         pref_info->pref_update_reason == SD_SS_PREF_UPDATE_REASON_SWITCH_CSFB))
      )
      &&
      !(cmph_is_subs_feature_mode_srlte(asubs_id) &&
        (pref_info->pref_update_reason == SD_SS_PREF_UPDATE_REASON_SWITCH_SVLTE ||
         pref_info->pref_update_reason == SD_SS_PREF_UPDATE_REASON_SWITCH_CSFB ||
         pref_info->pref_update_reason == SD_SS_PREF_UPDATE_REASON_ACTIVATE_HYBR2 ||
         pref_info->pref_update_reason == SD_SS_PREF_UPDATE_REASON_ACTIVATE_HYBR2_SILENT)
       )
    )
  {
    return TRUE;
  }
  /* Data call is on top of main stack and policy changed */
  else if (is_new_policy_tobe_read && 
         is_srv_mode_rat_cap_chgd &&
         ph_orig_top_ptr->act_type == CM_ACT_TYPE_DATA_CALL && 
         mm_id.stk_id != MM_STACK_2
  )
  {
    CM_MSG_HIGH_1("PS_DETACH_EVAL: sub %d, data call and rat cap/srv mode changed, return TRUE", 
                     state_machine->asubs_id);
    return TRUE;
  }
  /* LTE full service was added for E911 call and top act is PH meaning call has ended */
  else if(ph_sub_ptr->ps_pref.e911_lte_full_srv_added.full_srv_added &&
          ph_orig_top_ptr->act_type == CM_ACT_TYPE_PH_OBJ)
  {
    CM_MSG_HIGH_1("PS_DETACH_EVAL: sub %d, full LTE added for E911, return TRUE",
                  state_machine->asubs_id);
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION cmmsc_is_device_oprt_in_LTE

DESCRIPTION
   Determine if the device will be working in LTE.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmmsc_is_device_oprt_in_LTE(
  cmmsc_state_machine_s_type  *state_machine
)
{
  return(SD_MODE_CONTAIN(cmph_map_cm_mode_pref_to_sd_mode_pref(
                           state_machine->stack_common_info.preference.mode_pref),
                         SD_SS_MODE_PREF_LTE)
         &&
         (state_machine->stack_common_info.ue_mode_capability & SYS_SYS_MODE_MASK_LTE)
         &&
         cmph_ptr()->device_prop.ph_oprt_mode.true_oprt_mode == SYS_OPRT_MODE_ONLINE);
}

/*===========================================================================

FUNCTION cmmsc_is_oper_flag_buff

DESCRIPTION
   Determine if the is_operational flag on Hybrid2 stack should be buffered.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static boolean cmmsc_is_oper_flag_buff(
  cmmsc_state_machine_s_type   *state_machine
)
{
  cmss_s_type *ss_ptr = cmss_ptr();

  /*
  ** 1. Hybrid 2 is currently in LTE service
  ** 2. Hybrid 2 is currently in Sub1 SXLTE operation
  ** 3. LTE still part of the device capability
  ** 4. The feature is enabled
  ** 5. Device is not exiting ONLINE mode
  */
  return(sys_srv_status_is_srv(cmss_get_info_srv_status(state_machine->asubs_id, MM_STACK_2)) &&
         cmss_get_info_sys_mode(state_machine->asubs_id, MM_STACK_2)   == SYS_SYS_MODE_LTE &&
         cmss_get_misc_sub_prop_ptr(state_machine->asubs_id) != NULL &&
         cmss_get_misc_sub_prop_ptr(state_machine->asubs_id)->sub_info.gpp2_sub_info.hold_stack2_oper_flag_timer_value > 0                  &&
         cmmsc_is_device_oprt_in_LTE(state_machine)                  &&
         !cmtask_orig_para_is_obj_on_any_q(CM_ACT_TYPE_EMERG_CALL)
        );
}

/*===========================================================================

FUNCTION cmmsc_update_cmss_is_operational

DESCRIPTION
   Update cmss is_operational for the specified stack.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: when operating status for a subscription(is_stack_operational)is changed.
  FALSE: when ss_info_sub_prop_ptr is NULL or when stack_idx is invalid
         or if gen_info for a stack index is NULL.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmmsc_update_cmss_is_operational(
  cmmsc_state_machine_s_type  *state_machine,
  int                          stack_idx,
  boolean                      is_operational
)
{
  sys_modem_as_id_e_type              as_id = state_machine->asubs_id;
  cm_mm_ss_info_sub_prop_s_type        *ss_info_sub_prop_ptr = cmss_get_info_sub_prop(as_id);
  cm_mm_ss_info_gen_stack_info_s_type  *ss_info_all_stack_ptr;
  cmss_misc_sub_prop_s_type   *ss_misc_sub_ptr = cmss_get_misc_sub_prop_ptr(as_id);

  if(ss_info_sub_prop_ptr == NULL || stack_idx < MM_STACK_0 || stack_idx > MM_STACK_2 ||
      ss_misc_sub_ptr == NULL)
  {
    return FALSE;
  }

  ss_info_all_stack_ptr = ss_info_sub_prop_ptr->stack_info[stack_idx].gen_info;

  if(ss_info_all_stack_ptr == NULL)
  {
    return FALSE;
  }
  if(stack_idx == MM_STACK_2)
  {
	 /* Reset acq fail count on main_stack to PM if LTE is moved from hybrid2 to stk0
    */
    if(ss_info_all_stack_ptr->is_stack_operational == TRUE &&
        is_operational == FALSE &&
        SYS_IS_BIT_CONTAIN(cmmsc_get_msc_rat_cap(state_machine->asubs_id),SD_SS_MODE_PREF_LTE))
    {

       cm_mm_ss_info_3gpp_stack_info_s_type *ss_info_gpp_stack_ptr = ss_info_sub_prop_ptr->stack_info[stack_idx].gpp_info;

       if(ss_info_all_stack_ptr == NULL || ss_info_gpp_stack_ptr == NULL)
       {
         return FALSE;
       }

       policyman_reset_acq_fail(state_machine->asubs_id, BM(SYS_MODEM_STACK_ID_1));
       CM_MSG_HIGH_1("Reset acq_fail on stk0 on sub %d", state_machine->asubs_id);
       cmss_srlte_csfb_mm_info_backup(state_machine->asubs_id);

       /* 
       ** Whenever Stck2 is deactivated during the voice call on other sub, i.e due to user_pref change 
       ** then we will need to reset the ps suspend on stck2 , since after the call end, stck0 will be resumed. 
       ** This is needed to avoid ps_suspend being set to TRUE when UE switches back to Hyrbrid mode.  
       */
       if( ss_info_gpp_stack_ptr->ps_data_suspend == TRUE)
       {
         cmss_process_data_suspend_rpt(FALSE, cmph_map_sub_stk_to_id(as_id,stack_idx));
         CM_MSG_HIGH_3("Resetting ps_data_suspend to %d on sub %d  stck %d ",ss_info_gpp_stack_ptr->ps_data_suspend, 
                                                                             state_machine->asubs_id, stack_idx);
       }
    }    	
    if(ss_info_all_stack_ptr->is_stack_operational == TRUE &&
        is_operational == FALSE &&
        cmmsc_is_oper_flag_buff(state_machine))
    {
      cmss_start_stack2_oper_flag_timer(as_id);
    }
    /* If Hybrid 2 stack is being activated, reset the buffer timer.*/
    else if(ss_info_all_stack_ptr->is_stack_operational == FALSE &&
            is_operational == TRUE )
    {
      ss_misc_sub_ptr->sub_info.gpp2_sub_info.hold_stack2_oper_flag_uptime = CMSS_NO_SRV_UPTIME_NONE;
      /* Process any buffered call */
      cmcall_reset_call_sr(as_id, CM_CALL_HOLD_REAS_HOLD_TIMER);
      cmss_csfb_srlte_mm_info_backup(state_machine->asubs_id);
    }
    cmss_reset_cell_info_upon_stack2_change(is_operational, as_id);
  }

  if(ss_info_all_stack_ptr->is_stack_operational != is_operational)
  {
    /*operating status of a subscription changed*/
    ss_info_all_stack_ptr->is_stack_operational = is_operational;
    return TRUE;
  }

  return FALSE;
}

/*===========================================================================

FUNCTION cmmsc_action_force_top

DESCRIPTION
  Force the top priority queue preference on the specified stack.

DEPENDENCIES
  Stacks must have already been initialized with cmmsc_init_multi_stacks().

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void cmmsc_action_force_top(
  cmmsc_state_machine_s_type  *state_machine,
  int                          stack_idx,
  sd_ss_pref_reas_e_type       pref_reas
)
{
  cmmsc_action_force_top_addl(state_machine, stack_idx, pref_reas, NULL);
}

/*===========================================================================

FUNCTION cmmsc_action_force_top_ext

DESCRIPTION
  Force the top priority queue preference on the specified stack.

DEPENDENCIES
  Stacks must have already been initialized with cmmsc_init_multi_stacks().

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void cmmsc_action_force_top_ext(
  cmmsc_state_machine_s_type     *state_machine,
  cmmsc_event_force_pref_s_type  *evt_package,
  int                             stack_idx
)
{
  cm_orig_q_s_type           *ph_orig_top_ptr = NULL;
  cm_preference_info_s_type   pref_info;
  mm_sub_stk_id_s_type        mm_id;
  cmph_sub_info_s_type       *ph_sub_ptr;

  if(state_machine == NULL || evt_package == NULL)
  {
    CM_ERR_2("MSC: null ptr %d %d", state_machine, evt_package);
    return;
  }

  mm_id = cmph_map_sub_stk_to_id(state_machine->asubs_id,
                                 MSC_STK_ID_TO_MM_STK_ID(stack_idx));
  ph_orig_top_ptr = cmtask_orig_para_get_top(mm_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Make sure top orig ptr is not null
  */
  if(ph_orig_top_ptr == NULL)
  {
    return;
  }

  if ((ph_sub_ptr = cmph_get_sub_ptr(state_machine->asubs_id)) == NULL)
  {
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update base on evt package data */
  pref_info.pref_reas = evt_package->pref_reas;
  pref_info.hybr_pref = evt_package->hybr_pref;
  pref_info.orig_mode = evt_package->orig_mode;

  /* Populate the preference base on top pri queue */
  pref_info.mode_pref = ph_orig_top_ptr->orig->orig_mode_pref;
  pref_info.band_pref = ph_orig_top_ptr->orig->orig_band_pref;
  pref_info.lte_band_pref = ph_orig_top_ptr->orig->orig_lte_band_pref;
  pref_info.tds_band_pref = ph_orig_top_ptr->orig->orig_tds_band_pref;
  pref_info.prl_pref = ph_orig_top_ptr->orig->orig_prl_pref;
  pref_info.roam_pref = ph_orig_top_ptr->orig->orig_roam_pref;
  pref_info.srv_domain_pref = ph_orig_top_ptr->orig->orig_srv_domain_pref;
  pref_info.acq_order_pref = ph_sub_ptr->pref_info.acq_order_pref;
  pref_info.rat_acq_order_ptr = cmph_get_acq_pri_order_per_sub(state_machine->asubs_id);
  pref_info.camp_mode_pref =
    cmutil_map_cm_net_sel_mode_pref_to_camp_pref(
      ph_sub_ptr->pref_info.network_sel_mode_pref);
  pref_info.fullrat_status =
    cmpmprx_compute_fullrat_status(mm_id,ph_sub_ptr->pref_info.network_sel_mode_pref,cmpm_sub_ptr(mm_id.asubs_id));
  pref_info.sys_id.sys_id.id.plmn = ph_sub_ptr->pref_info.plmn;
  pref_info.sys_id.sys_id.id_type = SYS_SYS_ID_TYPE_UMTS;
  pref_info.sys_id.prm.gw.sel_type = SYS_MANUAL_SEL_TYPE_USER_SPECIFIED;
  pref_info.otasp_band = cmutil_map_otasp_act_code_to_sd_band(CM_OTASP_ACT_CODE_NONE);
  pref_info.otasp_blksys = cmutil_map_otasp_act_code_to_sd_blksys(CM_OTASP_ACT_CODE_NONE);
  pref_info.avoid_type = SD_SS_AVOID_SYS_IDLE;
  pref_info.avoid_time = 0;

  /* Update hybr1 specific data */
  pref_info.hybr1_pref.hybr_orig_mode = evt_package->orig_mode;
  pref_info.hybr1_pref.hybr_mode_pref = ph_orig_top_ptr->orig->orig_mode_pref;
  pref_info.pref_update_reason = ph_orig_top_ptr->orig->pref_update_reason;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Force preference to the stack */
  switch(stack_idx)
  {
    case 0:
    case 1:
    case 2:
      cmmsc_send_sys_sel_pref_chgd_cmd(state_machine, stack_idx, &pref_info, NULL);
      break;

    default:
      CM_ERR_1("MSC: Invalid stack_idx %d", stack_idx);
  }
}

/*===========================================================================

FUNCTION cmmsc_action_force_top_addl

DESCRIPTION
  Force the top priority queue preference on the specified stack.

DEPENDENCIES
  Stacks must have already been initialized with cmmsc_init_multi_stacks().

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void cmmsc_action_force_top_addl(
  cmmsc_state_machine_s_type                      *state_machine,
  int                                              stack_idx,
  sd_ss_pref_reas_e_type                           pref_reas,
  const cmmsc_addl_pref_sys_chgd_mmoc_cmd_s_type  *addl_ptr
)
{
  cmph_s_type                *ph_ptr = cmph_ptr();
  cm_orig_q_s_type           *ph_orig_top_ptr = NULL;
  cm_preference_info_s_type   pref_info;
  mm_sub_stk_id_s_type        mm_id;
  cmph_sub_info_s_type *ph_sub_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(state_machine == NULL) return;

  mm_id = cmph_map_sub_stk_to_id(state_machine->asubs_id, MSC_STK_ID_TO_MM_STK_ID(stack_idx));
  ph_sub_ptr = cmph_get_sub_ptr(mm_id.asubs_id);

  /* Make sure top orig ptr is not null
  */
  ph_orig_top_ptr = cmtask_orig_para_get_top(mm_id);

  if(ph_orig_top_ptr == NULL)
  {
    return;
  }

  if (!cmutil_is_mm_id_valid(mm_id) || ph_sub_ptr == NULL)
  {
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Populate the preference base on top pri queue */
  pref_info.pref_reas = pref_reas;
  pref_info.mode_pref = ph_orig_top_ptr->orig->orig_mode_pref;

  if(CMPH_SS_NETWORK_SEL_MODE_PREF(state_machine->asubs_id, ph_ptr ) ==
      CM_NETWORK_SEL_MODE_PREF_MANUAL &&
      ph_orig_top_ptr->orig->orig_mode == SD_SS_ORIG_MODE_NONE )
  {
    pref_info.orig_mode = SD_SS_ORIG_MODE_MANUAL_IDLE;
  }
  else
  {
    pref_info.orig_mode = ph_orig_top_ptr->orig->orig_mode;
  }

  pref_info.band_pref = ph_orig_top_ptr->orig->orig_band_pref;
  pref_info.lte_band_pref = ph_orig_top_ptr->orig->orig_lte_band_pref;
  pref_info.tds_band_pref = ph_orig_top_ptr->orig->orig_tds_band_pref;
  pref_info.hybr_pref = ph_orig_top_ptr->orig->orig_hybr_pref;
  pref_info.prl_pref = ph_orig_top_ptr->orig->orig_prl_pref;
  pref_info.roam_pref = ph_orig_top_ptr->orig->orig_roam_pref;
  pref_info.srv_domain_pref = ph_orig_top_ptr->orig->orig_srv_domain_pref;
  pref_info.acq_order_pref = ph_sub_ptr->pref_info.acq_order_pref;
  pref_info.rat_acq_order_ptr = cmph_get_acq_pri_order_per_sub(state_machine->asubs_id);
  pref_info.camp_mode_pref =
    cmutil_map_cm_net_sel_mode_pref_to_camp_pref(
      ph_sub_ptr->pref_info.network_sel_mode_pref);
  pref_info.fullrat_status =
    cmpmprx_compute_fullrat_status(mm_id,ph_sub_ptr->pref_info.network_sel_mode_pref,cmpm_sub_ptr(mm_id.asubs_id));
  pref_info.sys_id.sys_id.id.plmn = ph_sub_ptr->pref_info.plmn;
  pref_info.sys_id.sys_id.id_type = SYS_SYS_ID_TYPE_UMTS;
  pref_info.sys_id.prm.gw.sel_type = SYS_MANUAL_SEL_TYPE_USER_SPECIFIED;
  pref_info.otasp_band = cmutil_map_otasp_act_code_to_sd_band(CM_OTASP_ACT_CODE_NONE);
  pref_info.otasp_blksys = cmutil_map_otasp_act_code_to_sd_blksys(CM_OTASP_ACT_CODE_NONE);
  pref_info.avoid_type = SD_SS_AVOID_SYS_IDLE;
  pref_info.avoid_time = 0;

  pref_info.hybr1_pref.hybr_mode_pref = ph_orig_top_ptr->orig->orig_mode_pref;

  /* Setting the hybr orig mode to colloc to avoid SD running
     3GPP scripts on HDR stack.
  */
  if (stack_idx == 1)
  {
    pref_info.hybr1_pref.hybr_orig_mode = SD_SS_ORIG_MODE_COLLOC;
    CM_MSG_MED_1("Setting the hybr1 orig mode to %d",pref_info.hybr1_pref.hybr_orig_mode);
  }

  pref_info.pref_update_reason = ph_orig_top_ptr->orig->pref_update_reason;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Force preference to the stack */
  switch(stack_idx)
  {
    case 0:
    case 1:
    case 2:
      cmmsc_send_sys_sel_pref_chgd_cmd(state_machine, stack_idx, &pref_info, addl_ptr);
      break;

    default:
      CM_ERR_1("MSC: Invalid stack_idx %d", stack_idx);
  }
}

static void cmmsc_action_sub(
  const cmmsc_event_sub_chg_s_type  *evt_pkg
)
{
  CM_ASSERT(evt_pkg != NULL);
  if(evt_pkg == NULL)
  {
    sys_err_fatal_null_ptr_exception();
  }
}

/*===========================================================================

FUNCTION cmmsc_action_kick_hybr2

DESCRIPTION
  This function to kick hybr2 stack when get net is completed on main

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void cmmsc_action_kick_hybr2(
  cmmsc_state_machine_s_type  *msc,
  int                          stack_idx
)
{
  CM_MSG_MED_1("cmmsc_action_kick_hybr2(), stack_idx=%d", stack_idx);
  if (msc == NULL) return;

  switch(stack_idx)
  {
    case 2:
    {
      if(!cmmsc_is_hybr2_kick_allowed(msc))
      {
        cmmsc_auto_update_is_kick_hybr2_pending(msc, TRUE);
      }
      else
      {
        cmmsc_action_pending_kick(msc, stack_idx, TRUE);
      }

      break;
    }

    default:
      break;
  }
}

/*===========================================================================

FUNCTION cmmsc_init_stack_common

DESCRIPTION
  This function should be called to initialize stack common info.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_init_stack_common(
  cmmsc_state_machine_s_type  *state_machine
)
{
  if(state_machine == NULL)
  {
    return;
  }

  state_machine->stack_common_info.is_in_emergency_call = FALSE;
  state_machine->stack_common_info.gpp2_info.is_hybr1_disabled = FALSE;
  state_machine->stack_common_info.gpp2_info.stack_kick.is_kick_hdr_pending = FALSE;
  state_machine->stack_common_info.gpp2_info.stack_kick.is_kick_gw = FALSE;
  state_machine->stack_common_info.gpp2_info.stack_kick.is_hybr_2_kick_pending = FALSE;
  state_machine->stack_common_info.gpp2_info.stack_kick.is_stack_sync_bsr_pending = FALSE;

  state_machine->stack_common_info.gpp2_info.timer.acq_hybr_1_uptime = CMMSC_UPTIME_NONE;
  state_machine->stack_common_info.gpp2_info.timer.lost_main_uptime= CMMSC_UPTIME_NONE;
  state_machine->stack_common_info.gpp2_info.timer.avoid_hybr_hdr_uptime= CMMSC_UPTIME_NONE;

  state_machine->stack_common_info.preference.mode_pref = CMPH_PRST_SS_MODE_PREF(state_machine->asubs_id, cmph_ptr());
  state_machine->stack_common_info.preference.band_pref = CMPH_PRST_SS_BAND_PREF(state_machine->asubs_id, cmph_ptr());
  state_machine->stack_common_info.preference.tds_band_pref = CMPH_PRST_SS_TDS_BAND_PREF(state_machine->asubs_id, cmph_ptr());
  state_machine->stack_common_info.preference.lte_band_pref = CMPH_PRST_SS_LTE_BAND_PREF(state_machine->asubs_id, cmph_ptr());
}

/*===========================================================================

FUNCTION cmmsc_init_multi_stacks

DESCRIPTION
  This function should be called to initialize all stacks.

DEPENDENCIES
  cmph_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_init_multi_stacks(
  cmmsc_state_machine_s_type   *state_machine
)
{
  int i;

  if(state_machine == NULL)
  {
    return;
  }

  /* Initialize the stack common info */
  cmmsc_init_stack_common(state_machine);

  /* Initialize the phone capability to default values */
  cmmsc_init_mode_and_capability(state_machine);

  /* Initialized all the stacks */
  for(i = 0; i < CMMSC_STACK_NUM; i++)
  {
    cmmsc_init_stack(state_machine, i);
  }
}

/*===========================================================================

FUNCTION cmmsc_init_stack

DESCRIPTION
  This function should be called to initialize a cmmsc stack

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_init_stack(
  cmmsc_state_machine_s_type  *state_machine,
  int                          stack_idx
)
{
  cmmsc_stack_s_type  *stack_pointer;

  if(state_machine == NULL)
  {
    return;
  }

  stack_pointer = &(state_machine->stack[stack_idx]);

  /* Initialize cmss is_operational */
  if(stack_idx != 0 && cmmsc_update_cmss_is_operational(state_machine, stack_idx, FALSE))
  {
    cmpmprx_report_operational_radio_stacks(state_machine->asubs_id);
  }

  /* Initialize state machine for the stack */
  stack_pointer->stack_id = stack_idx;
  stack_pointer->state = CMMSC_STACK_STATE_NULL;
  cmmsc_set_stack_capability(state_machine);
  stack_pointer->addl_action.action = 0;
  stack_pointer->addl_action.ue_mode = SYS_UE_MODE_NONE;
}

static void cmmsc_action_power_down(
  int                          stack_id,
  cmmsc_state_machine_s_type  *state_machine
)
{
  if(stack_id == 0)
  {
    /* Initialize the stack common info */
    cmmsc_init_stack_common(state_machine);
  }

  switch(stack_id)
  {
    case 0:
    case 1:
    case 2:
      /* Re-init stack */
      cmmsc_init_stack(state_machine, stack_id);
      break;

    default:
      break;
  }
}

/*===========================================================================
FUNCTION cmmsc_is_valid_ss_for_cap_update

DESCRIPTION
  This function checks if the SS is valid to send a PS detach or TAU on based on
  UE feature mode.

DEPENDENCIES
  SUBS FEATURE mode must be initialized properly.

RETURN VALUE
  TRUE:  If SS is valid
  FALSE: Otherwise

SIDE EFFECTS
  None.
===========================================================================*/
static boolean cmmsc_is_valid_ss_for_cap_update(
  sys_modem_as_id_e_type  as_id,
  /* AS_ID of the FEATURE MODE */

  int                     stack_id
  /* stack id value to validate */
)
{
  sys_subs_feature_t    subs_feature_mode = cmph_get_subs_feature_mode(as_id);
  boolean               is_valid = FALSE;
  mm_sub_stk_id_s_type  mm_id;
  boolean               is_oper;

  mm_id.asubs_id = as_id;
  mm_id.stk_id = MM_STACK_2;
  is_oper = cmss_is_operational(mm_id);

  CM_MSG_HIGH_4("MSC: PS_DETACH: Validate asubs_id %d stack %d for cap update, subs_f_mode %d, is_oper %d",
                stack_id, as_id, subs_feature_mode, is_oper);

  switch(subs_feature_mode)
  {
    case SYS_SUBS_FEATURE_MODE_SVLTE:
    case SYS_SUBS_FEATURE_MODE_SRLTE:
    {
      if(is_oper)
      {
#if defined(CM_GW_SUPPORTED) || defined(FEATURE_CM_LTE) || defined(FEATURE_TDSCDMA)
        cmregprx_state_e_type stk0_state = cmregprx_get_state(cmph_map_sub_stk_to_id(as_id, MM_STACK_0));
        cmregprx_state_e_type stk2_state = cmregprx_get_state(cmph_map_sub_stk_to_id(as_id, MM_STACK_2));
        CM_MSG_HIGH_3("MSC: PS_DETACH: sub %d cmregprx_state[0]=%d, cmregprx_state[1]=%d,",
                      as_id, stk0_state, stk2_state);
#endif

        if((stack_id == 2
#if defined(CM_GW_SUPPORTED) || defined(FEATURE_CM_LTE) || defined(FEATURE_TDSCDMA)
            && (stk0_state != CMREGPRX_STATE_ACTIVE
                  ||
                !SD_MODE_CONTAIN(cmmsc_get_stk_mode_cap(
                                   cmph_map_sub_stk_to_id(as_id, MM_STACK_0)),
                                 SD_SS_MODE_PREF_TDS_GSM_WCDMA_LTE))
#endif
           ) ||
            (stack_id == 0
#if defined(CM_GW_SUPPORTED) || defined(FEATURE_CM_LTE) || defined(FEATURE_TDSCDMA)
             && stk2_state!= CMREGPRX_STATE_ACTIVE
             && stk0_state != CMREGPRX_STATE_INACTIVE
#endif
            )
          )
        {
          is_valid = TRUE;
          CM_MSG_HIGH_1("MSC: PS_DETACH: stack %d is valid for cap update", stack_id);
        }
      }
      else if(stack_id != 1)
      {
        is_valid = TRUE;
      }
      break;
    }

    case SYS_SUBS_FEATURE_MODE_NORMAL:
    {
      if(stack_id != 1)
      {
        is_valid = TRUE;
      }
      break;
    }
    default:
    {
      CM_MSG_HIGH_1("MSC: PS_DETACH: Invalid SUBS FEATURE MODE %d", subs_feature_mode);
    }
  }

  return is_valid;
}

/*===========================================================================
FUNCTION cmmsc_update_addl_action_ue_mode_switch

DESCRIPTION
  This function checks and updates addl_action with UE_MODE_SWITCH if needed

DEPENDENCIES


RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_update_addl_action_ue_mode_switch(
  cmmsc_state_machine_s_type   *state_machine,
  int                           stack_idx,
  sys_addtl_action_s_type      *addl_action,
  cm_preference_info_s_type    *pref_info
)
{
  if(addl_action == NULL || state_machine == NULL)
  {
    CM_ERR_2("null ptr addl_action %x state_machine %x", addl_action, state_machine);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* SRLTE handling */

  /* Send the ue mode indication to NAS for srlte */
  if(cmph_is_subs_feature_mode_srlte(state_machine->asubs_id) &&
      state_machine->stack_common_info.ue_mode_forced == FALSE)
  {
    if(stack_idx == 0)
    {
      addl_action->action |= (uint8)MMOC_PREF_SYS_ADDL_ACTION_UE_MODE_SWITCH;
      addl_action->ue_mode = state_machine->stack_common_info.ue_mode;
      addl_action->is_ue_mode_substate_srlte = cmmsc_auto_is_hybr2_on(state_machine->op_mode);
      /* Force rge flag is being set during the CSFb <->SRLTE switches. Need to communicate the 
              TAU info along with pref chg command itself to avoid race conditions of user pref chg collision with 
              switch preferences
           */ 
      if(state_machine->stack_common_info.is_force_reg_pending == TRUE)
      {
        addl_action->is_tau_reqd = TRUE;
        state_machine->stack_common_info.is_force_reg_pending = FALSE;
      }

      CM_MSG_HIGH_4_EXT("CM->MMOC: ue_mode_chgd: sub=%d, addl_action %d, ue_mode %d, is_tau_reqd %d ",
                        state_machine->asubs_id, addl_action->action, addl_action->ue_mode, 
                        addl_action->is_tau_reqd, state_machine->asubs_id);
      state_machine->stack_common_info.ue_mode_forced = TRUE;
      state_machine->stack_common_info.is_ue_mode_chg_in_progress = TRUE;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* SVLTE + G handling */

  /* Only set addtional action on main */
  if(cmph_is_msim() &&
      cmph_is_subs_feature_mode_svlte(state_machine->asubs_id) &&
      state_machine->stack_common_info.ue_mode_forced == FALSE
    )
  {
    /* Reset ue_mode_forced if stack index 0*/
    if(stack_idx == 0)
    {
      addl_action->action |= (uint8)MMOC_PREF_SYS_ADDL_ACTION_UE_MODE_SWITCH;
      addl_action->ue_mode = state_machine->stack_common_info.ue_mode;
      addl_action->is_ue_mode_substate_srlte = FALSE;
      CM_MSG_HIGH_3_EXT("CM->MMOC: ue_mode_chgd:   sub=%d, addl_action %d, ue_mode %d",
                        state_machine->asubs_id, addl_action->action, addl_action->ue_mode, (state_machine->asubs_id+1));
      state_machine->stack_common_info.ue_mode_forced = TRUE;
      state_machine->stack_common_info.is_ue_mode_chg_in_progress = TRUE;
    }
  }
}

static void cmmsc_action_suspend_stack(
  int                          stack_id,
  cmmsc_state_machine_s_type  *state_machine
)
{
  mm_sub_stk_id_s_type mm_id;
  cmph_sub_info_s_type *ph_sub_ptr;

  if(state_machine == NULL) return;

  mm_id = cmph_map_sub_stk_to_id(state_machine->asubs_id, MSC_STK_ID_TO_MM_STK_ID(stack_id));
  ph_sub_ptr = cmph_get_sub_ptr(state_machine->asubs_id);
  if (!cmutil_is_as_id_valid(state_machine->asubs_id) || ph_sub_ptr == NULL ||
      !(stack_id >= MM_STACK_0 && stack_id <= MM_STACK_2))
  {
    return;
  }

  ph_sub_ptr->ss_susp_info[stack_id].susp_due_to_emerg = TRUE;
  mmoc_cmd_suspend_stack_new(mm_id,TRUE, FALSE, PROT_DEACT_DSDX_SUSP);
  cmph_update_suspend_status(mm_id, TRUE, TRUE);
}

/*===========================================================================
  After emergency call is ended, all suspended stacks have to resumed.
  When resume is sent to all suspended stacks in a given subscription:
  if ss=MAIN, clear flag in mmoc & then a pwrup ind is sent from MMOC to SD
  if ss=HYBR1, clear flag in mmoc & wait for 1x acq result to start
  if ss=HYBR2, clear flag in MMOC & start acq by sending pref, if its SVLTE
===========================================================================*/
static void cmmsc_action_resume_stack(
  int                          stack_id,
  cmmsc_state_machine_s_type  *state_machine
)
{
  mm_sub_stk_id_s_type mm_id;
  cmph_sub_info_s_type *ph_sub_ptr;

  if(state_machine == NULL) return;

  mm_id = cmph_map_sub_stk_to_id(state_machine->asubs_id, MSC_STK_ID_TO_MM_STK_ID(stack_id));
  ph_sub_ptr = cmph_get_sub_ptr(state_machine->asubs_id);

  if (!cmutil_is_as_id_valid(state_machine->asubs_id) || ph_sub_ptr == NULL ||
      !(stack_id >= MM_STACK_0 && stack_id <= MM_STACK_2))
  {
    return;
  }

  ph_sub_ptr->ss_susp_info[stack_id].susp_due_to_emerg = FALSE;
  mmoc_cmd_suspend_stack_new(mm_id,FALSE,FALSE, PROT_DEACT_DSDX_SUSP);
  cmph_update_suspend_status(mm_id, TRUE, FALSE);

  if(stack_id == 2 &&
      cmph_is_subs_feature_mode_1x_sxlte(state_machine->asubs_id))
  {
    /* After resuming, HYBR2 in SVLTE needs to start acquisition along
       with MAIN stack, so kickstart fresh HYBR2 acq */
    cmmsc_action_force_top(state_machine, stack_id, SD_SS_PREF_REAS_USER);
  }
}

/*===========================================================================

FUNCTION cmmsc_map_cm_rat_acq_order_pref_to_sd

DESCRIPTION
  This function translates a CM acquisition order pref to sd acquisition order

DEPENDENCIES
  none

RETURN VALUE
  SD service domain preference.

SIDE EFFECTS
  none

===========================================================================*/
static sd_rat_acq_order_s_type cmmsc_map_cm_rat_acq_order_pref_to_sd(
  const cm_acq_pri_order_pref_s_type  *acq_order_pref_ptr
)
{
  sd_rat_acq_order_s_type  sd_rat_acq_order;
  uint8                    i=0;

  memset(&sd_rat_acq_order, 0, sizeof(sd_rat_acq_order_s_type));

  if(acq_order_pref_ptr != NULL)
  {
    sd_rat_acq_order.version = 0xFF;
    sd_rat_acq_order.num_rat = acq_order_pref_ptr->num_rat;
    for(i = 0; i < SYS_MAX_ACQ_PREF_RAT_LIST_NUM; i++)
    {
      sd_rat_acq_order.acq_sys_mode[i] = acq_order_pref_ptr->acq_order[i];
    }
  }
  else
  {
    sd_rat_acq_order.version = 0xFF;
    sd_rat_acq_order.num_rat = 0;
  }

  return sd_rat_acq_order;
} /* cmmsc_map_cm_rat_acq_order_pref_to_sd */

/*===========================================================================

FUNCTION cmmsc_map_sys_domain_to_cm

DESCRIPTION
  This function translates a service domain to cm_srv_domain_pref_e_type.

DEPENDENCIES
  none

RETURN VALUE
  cm_srv_domain_pref_e_type.

SIDE EFFECTS
  none

===========================================================================*/
cm_srv_domain_pref_e_type cmmsc_map_sys_domain_to_cm(
  sys_srv_domain_e_type  srv_domain
)
{
  cm_srv_domain_pref_e_type mapped_service_domain;

  switch ( srv_domain )
  {
    case SYS_SRV_DOMAIN_CS_ONLY:
      mapped_service_domain = CM_SRV_DOMAIN_PREF_CS_ONLY;
      break;

    case SYS_SRV_DOMAIN_PS_ONLY:
      mapped_service_domain = CM_SRV_DOMAIN_PREF_PS_ONLY;
      break;

    case SYS_SRV_DOMAIN_CS_PS:
      mapped_service_domain = CM_SRV_DOMAIN_PREF_CS_PS;
      break;

    case SYS_SRV_DOMAIN_NONE:
    case SYS_SRV_DOMAIN_NO_SRV:
      mapped_service_domain = CM_SRV_DOMAIN_PREF_NONE;
      break;

    case SYS_SRV_DOMAIN_CAMPED:
    case SYS_SRV_DOMAIN_MAX:
    default:
      CM_ERR_1("No mapping for srv_domain=%d", srv_domain);
      mapped_service_domain = CM_SRV_DOMAIN_PREF_NONE;
      break;
  } /* switch ( service_domain_pref ) */

  return mapped_service_domain;
}


void cmmsc_set_force_reg_pending(sys_modem_as_id_e_type asubs_id, boolean val)
{
  cmmsc_state_machine_s_type  *state_machine = cmmsimc_state_machine_ptr(asubs_id);
  state_machine->stack_common_info.is_force_reg_pending = val;
}
void cmmsc_send_sys_sel_pref_chgd_cmd(
  cmmsc_state_machine_s_type                      *state_machine,
  int                                              stack_idx,
  cm_preference_info_s_type                       *pref_info,
  const cmmsc_addl_pref_sys_chgd_mmoc_cmd_s_type  *addl_ptr
)
{
  cmmsc_stack_s_type              *stack_pointer;
  sd_ss_pref_update_reason_e_type  pref_update_reason  =
    SD_SS_PREF_UPDATE_REASON_NONE;
  sys_addtl_action_s_type         addl_action;
  uint16                          sys_sel_pref_req_id = 0;
  sys_csg_id_type                 csg_id              = SYS_CSG_ID_INVALID;
  sys_radio_access_tech_e_type    csg_rat             = SYS_RAT_NONE;
  cm_orig_q_s_type               *ph_orig_top_ptr     = NULL;
  cmph_s_type                    *ph_ptr              = cmph_ptr();
  cmmsc_stack_capability_s_type   common_pref;
  sd_ss_mode_pref_e_type          user_mode_pref      = SD_SS_MODE_PREF_ANY;
  sd_rat_acq_order_s_type         sd_rat_acq_order;
  sd_ss_hybr_pref_e_type          internal_hybr_pref  = SD_SS_HYBR_PREF_NONE;
  cmcall_s_type                  *call_ptr            = NULL;

  sd_ss_pref_reas_e_type          pref_reason;
  sd_ss_pref_camp_mode_e_type     camp_mode;
  sd_ss_orig_mode_e_type          orig_mode = SD_SS_ORIG_MODE_NONE;
  boolean                         is_term_get_net = TRUE;
  mm_sub_stk_id_s_type            mm_id;
  cmph_sub_info_s_type           *ph_sub_ptr;
  cm_act_id_type                  act_id = CM_ACT_ID_NONE;
  cmsds_sub_prop_s_type          *sds_sub_prop_ptr;
  boolean                         is_emerg_call_actv = FALSE;

  addl_action.action = (uint8)MMOC_PREF_SYS_ADDL_ACTION_NONE;
  addl_action.ue_mode = SYS_UE_MODE_NONE;
  addl_action.is_ue_mode_substate_srlte = FALSE;
  addl_action.is_tau_reqd = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(pref_info == NULL)
  {
    CM_ERR_0("MSC: pref info null ptr");
    return;
  }

  mm_id.asubs_id = state_machine->asubs_id;
  mm_id.stk_id = MSC_STK_ID_TO_MM_STK_ID(stack_idx);
  ph_sub_ptr = cmph_get_sub_ptr(mm_id.asubs_id);
  sds_sub_prop_ptr = cmsds_get_sub_prop_ptr(mm_id.asubs_id);
  if(ph_sub_ptr == NULL || !cmutil_is_mm_id_valid(mm_id) ||
      mm_id.stk_id == MM_STACK_ALL)
  {
    CM_ERR_2("MSC: Invalid as_id %d, stack_id %d", mm_id.asubs_id, mm_id.stk_id);
    return;
  }

  if(cmtask_is_stack_pending_for_deact(mm_id))
  {
    CM_MSG_HIGH_2_ASID("MSC->MMOC: pref_chgd, stk %d inactive or pending to be deactivated",
                       mm_id.asubs_id, mm_id.stk_id);
    return;
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the correct preference pointer to the correct stack */
  stack_pointer = &state_machine->stack[stack_idx];

  /* Get the top of the priority queue */
  ph_orig_top_ptr = cmtask_orig_para_get_top(mm_id);

  if(ph_orig_top_ptr == NULL)
  {
    //CM_ERR_0("MSC: ph_orig_top_ptr null ptr");
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Get pref info */

  pref_reason = pref_info->pref_reas;
  camp_mode = pref_info->camp_mode_pref;
  sd_rat_acq_order =
    cmmsc_map_cm_rat_acq_order_pref_to_sd(pref_info->rat_acq_order_ptr);

  /* get hdr orig mode from hybr1_pref */
  if(mm_id.stk_id == MM_STACK_1)
  {
    orig_mode = pref_info->hybr1_pref.hybr_orig_mode;
  }
  else
  {
    orig_mode = pref_info->orig_mode;
  }

  if(ph_orig_top_ptr->act_type == CM_ACT_TYPE_VOICE_CALL
#if defined (FEATURE_3GPP_CSFB)
      || ph_orig_top_ptr->act_type == CM_ACT_TYPE_CS_DATA_CALL
#endif
    )
  {
    call_ptr = cmcall_ptr((cm_call_id_type)ph_orig_top_ptr->orig->act_id);
    if(call_ptr != NULL && call_ptr->direction == CM_CALL_DIRECTION_MT)
    {
      /* Dont terminate GET_NETWORKS if it is a MT call */
      is_term_get_net = FALSE;
    }
  }

  /* Don't terminate ongoing GET_NETWORKS if it's not on this sub
  ** or pref change is on stack 1
  ** or not on this stack and stack 0 of the sub is not suspended.
  */
  if(ph_ptr->device_prop.mm_id_for_get_networks.asubs_id != SYS_MODEM_AS_ID_NONE
      &&
    (ph_ptr->device_prop.mm_id_for_get_networks.asubs_id != mm_id.asubs_id ||
     mm_id.stk_id == MM_STACK_1 ||
    (ph_ptr->device_prop.mm_id_for_get_networks.stk_id != mm_id.stk_id &&
    !ph_sub_ptr->ss_susp_info[MM_STACK_0].is_suspend)))
  {
    is_term_get_net = FALSE;
    CM_MSG_HIGH_4("GET_NET: Don't terminate scan on sub %d stk %d due to new prefs on sub %d stk %d",
                  ph_ptr->device_prop.mm_id_for_get_networks.asubs_id,
                  ph_ptr->device_prop.mm_id_for_get_networks.stk_id,
                  mm_id.asubs_id,
                  mm_id.stk_id);
  }

  /* Terminate the periodic search attempt on the same sub when the pref reason
  ** is START_CS or USER_resel regardless of if the search is actually ongong in AS layer.
  */
  if(mm_id.stk_id != MM_STACK_1 &&
    cmutil_is_periodic_manual_search() &&
    ph_ptr->device_prop.pf_scan_info && 
    mm_id.asubs_id == ph_ptr->device_prop.pf_scan_info->asubs_id &&
    (pref_reason == SD_SS_PREF_REAS_ORIG_START_CS ||
     pref_reason == SD_SS_PREF_REAS_USER_RESEL))
    {
    is_term_get_net = TRUE;
    CM_MSG_HIGH_0("GET_NET: PERIODIC: Terminate the current search attempt");
    }

  if(ph_sub_ptr->force_data_activity_pref_ongoing ==TRUE && ph_ptr->device_prop.man_srch_timer_uptime != 0 && mm_id.stk_id != MM_STACK_1)
    {
      is_term_get_net = FALSE;
      CM_MSG_HIGH_0("GET_NET: Don't terminate scan new pref is due to mms_end_activity");
    }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Terminate the pending get networks request.
  */
  if( (pref_reason == SD_SS_PREF_REAS_ORIG_START_CS ||
       pref_reason == SD_SS_PREF_REAS_ORIG_START_PS ||
       pref_reason == SD_SS_PREF_REAS_ORIG_RESUME   ||
       pref_reason == SD_SS_PREF_REAS_REDIAL_NORM   ||
       pref_reason == SD_SS_PREF_REAS_REDIAL_OPTI   ||
       pref_reason == SD_SS_PREF_REAS_USER          ||
       pref_reason == SD_SS_PREF_REAS_USER_RESEL) &&
      is_term_get_net == TRUE
    )
  {
    boolean end_pf_scan = (pref_reason == SD_SS_PREF_REAS_ORIG_START_CS ||
                          pref_reason == SD_SS_PREF_REAS_USER_RESEL);
                                          
    cmph_term_pending_get_networks_req(mm_id.asubs_id,
                                         end_pf_scan,TRUE);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* if any network scan is ongoing forcing STACK_SYNC_UP_BSR may cause
        a new service req which can abort ongoing network scan.
        Considering user triggered network scan being higher priority pend STACK_SYNC_UP_BSR
        Once network_scan is completed pending STACK_SYNC_UP_BSR is forced to SD */
  if(mm_id.stk_id == MM_STACK_2 &&
      pref_reason == SD_SS_PREF_REAS_STACK_SYNC_UP_BSR &&
      (cmemg_get_reason_per_sub(mm_id.asubs_id) == CM_AC_REASON_THIRD_PARTY_IMS_E911 ||
       cmph_is_get_networks_ongoing(mm_id.asubs_id) == TRUE) &&
      cmph_is_subs_feature_mode_1x_sxlte(mm_id.asubs_id))
  {
    CM_MSG_HIGH_0("MSC: stack synup bsr buffered - no action taken, get networks is ongoing");
    cmmsc_auto_update_is_stack_sync_bsr_pending(state_machine, TRUE);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Update csg_id & csg_rat */

  if(ph_orig_top_ptr->orig != NULL)
  {
    sys_sel_pref_req_id = ph_orig_top_ptr->orig->sys_sel_pref_req_id;
    csg_id = ph_orig_top_ptr->orig->csg_id;
    csg_rat = ph_orig_top_ptr->orig->csg_rat;
    act_id = ph_orig_top_ptr->orig->act_id;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /* Update additional action
  */
  if((ph_sub_ptr->ps_pref.is_cap_update_required == TRUE ||
      ph_sub_ptr->ps_pref.is_user_ps_detach_required == TRUE)
      &&
      (ph_orig_top_ptr->act_type == CM_ACT_TYPE_PH_OBJ ||
       ph_orig_top_ptr->act_type == CM_ACT_TYPE_DATA_CALL ||
       ph_orig_top_ptr->act_type == CM_ACT_TYPE_POLICYMAN_RESTRICT)
      &&
      cmmsc_is_valid_ss_for_cap_update(mm_id.asubs_id, stack_idx)
    )
  {
    if(ph_sub_ptr->ps_pref.is_tau_cap_update_required == TRUE &&
       ph_sub_ptr->ps_pref.is_user_ps_detach_required == FALSE)
    {
      /* Perform TAU
      ** Does not go through MMOC, TAU is indicated to NAS as part of next service request
      */
      ph_sub_ptr->ps_pref.is_cap_update_required = FALSE;
      ph_sub_ptr->ps_pref.is_user_ps_detach_required = FALSE;
      ph_sub_ptr->ps_pref.is_policy_chg_cap_update = FALSE;
      ph_sub_ptr->ps_pref.is_tau_cap_update_required = FALSE;
      addl_action.is_tau_reqd = TRUE;
    }
    else
    {
      /* Perform PS detach
      */
      addl_action.action |= (uint8)MMOC_PREF_SYS_ADDL_ACTION_PS_DETACH;
      /* If DDS is switching and detach required is TRUE, mark detach in progress true*/
      if(mm_id.asubs_id == ph_ptr->device_prop.msim_prop.new_default_data_subs &&
       mm_id.asubs_id != ph_ptr->device_prop.msim_prop.default_data_subs &&
       ph_ptr->sub_info[mm_id.asubs_id] != NULL)
      {
        ph_ptr->sub_info[mm_id.asubs_id]->ps_detach_in_progress = TRUE;
      }

      ph_sub_ptr->ps_pref.is_cap_update_required = FALSE;
      ph_sub_ptr->ps_pref.is_user_ps_detach_required = FALSE;
  
      if(ph_sub_ptr->ps_pref.is_policy_chg_cap_update == TRUE)
      {
        addl_action.action |= (uint8)MMOC_PREF_SYS_ADDL_ACTION_POLICY_PS_DETACH;
        ph_sub_ptr->ps_pref.is_policy_chg_cap_update = FALSE;
      }
    }
    CM_MSG_HIGH_2("MSC: PS_DETACH: addl_action %d, is_tau_reqd %d",
                  addl_action.action,
                  addl_action.is_tau_reqd);
  }
  #ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  /*If user is forcing the preference as NET_SEL_MODE_LIMITED,
   we need to trigger a STOP MODE first to detach from existing NW*/

  if(ph_sub_ptr->is_limited_srv_net_sel_mode_selected &&
     (ph_orig_top_ptr->act_type == CM_ACT_TYPE_PH_OBJ ||
      ph_orig_top_ptr->act_type == CM_ACT_TYPE_DATA_CALL))
  {
    addl_action.action |= (uint8)MMOC_PREF_SYS_ADDL_ACTION_FORCE_LIMITED_SRV;

    CM_MSG_HIGH_1("MSC: Limited Net Sel Mode Selected : addl_action %d",
                  addl_action.action);
  }
  #endif
  /* If top object in the queue is phone object, clear the
  ** pref_change_pending flag.
  */
  if(ph_orig_top_ptr->act_type == CM_ACT_TYPE_PH_OBJ &&
      ph_sub_ptr->is_pref_change_pending &&
      mm_id.stk_id == MM_STACK_0)
  {
    CM_MSG_HIGH_0("Set is_pref_change_pending to FALSE");
    ph_sub_ptr->is_pref_change_pending = FALSE;
  }

  if(state_machine->stack[stack_idx].addl_action.action == MMOC_PREF_SYS_ADDL_ACTION_RESUME)
  {
    addl_action.action |= (uint8)MMOC_PREF_SYS_ADDL_ACTION_RESUME;
    state_machine->stack[stack_idx].addl_action.action &= ~(MMOC_PREF_SYS_ADDL_ACTION_RESUME);
    ph_sub_ptr->ss_susp_info[stack_idx].susp_due_to_emerg = FALSE;
    cmph_update_suspend_status(mm_id, FALSE, FALSE);

    CM_MSG_HIGH_4_ASID("MSC->MMOC: addl_action to resume, susp_state %d %d %d",
                       mm_id.asubs_id,
                       ph_sub_ptr->ss_susp_info[0].is_suspend,
                       ph_sub_ptr->ss_susp_info[1].is_suspend,
                       ph_sub_ptr->ss_susp_info[2].is_suspend);
  }

  /* Update add_action - ue_mode if conditions are met */
  cmmsc_update_addl_action_ue_mode_switch(state_machine, stack_idx, &addl_action, pref_info);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Set pref_update reason */

  if(cmac_is_1x_ppp_dereg_end(mm_id.asubs_id))
  {
    pref_update_reason = SD_SS_PREF_UPDATE_REASON_1X_DEREG_END;
    CM_MSG_MED_0("MSC: pref update reas 1x_DEREG_END");
  }

#ifdef CM_GW_SUPPORTED
  /* Set EF_RAT variables */
  else if(cmph_is_rat_bal_allowed(mm_id.asubs_id) &&
          cmph_is_mode_change_ef_rat(mm_id.asubs_id) &&
          mm_id.stk_id != MM_STACK_1)
  {
    pref_update_reason = SD_SS_PREF_UPDATE_REASON_EF_RAT_MODE_CHG;
    /* Reset the flag */
    cmph_update_mode_change_ef_rat(mm_id.asubs_id, FALSE);
    CM_MSG_MED_0("MSC: hybr2/3:EF_RAT_MODE_CHG set FALSE");
  }
#endif /* CM_GW_SUPPORTED */

#ifdef FEATURE_MMODE_DYNAMIC_SV_OPERATION_SWITCH
  else if(pref_info->pref_update_reason != SD_SS_PREF_UPDATE_REASON_NONE)
  {
    /* After we set this - we're not clearing the variable
    ** pref_update_reason here because we will continue to force on MAIN.
    ** again, so if we clear it here, MAIN will see this value as _NONE value when
    ** MAIN stack is forced.
    */
    pref_update_reason = pref_info->pref_update_reason;
    CM_MSG_HIGH_2_ASID("CMSOA: pref_update_reason=%d", mm_id.asubs_id, pref_update_reason);

    if(cmmsc_get_1x_sub() == mm_id.asubs_id &&
        cmph_is_oprting_in_csfb_pref(mm_id.asubs_id) &&
        (pref_update_reason == SD_SS_PREF_UPDATE_REASON_SWITCH_SVLTE ||
          pref_update_reason == SD_SS_PREF_UPDATE_REASON_ACTIVATE_HYBR2 ||
          pref_update_reason == SD_SS_PREF_UPDATE_REASON_ACTIVATE_HYBR2_SILENT) &&
         !cmmsc_is_op_mode_multi_stacks(state_machine->op_mode)
      )
      {
        pref_update_reason = SD_SS_PREF_UPDATE_REASON_NONE;
      }

  }
  else if(cmmsc_auto_is_hybr2_on(state_machine->op_mode) &&
          (ph_ptr->device_prop.msim_prop.is_dds_change_on_svlte_sub == TRUE) &&
          (mm_id.stk_id == MM_STACK_1) && (pref_reason == SD_SS_PREF_REAS_USER))
  {
    CM_MSG_HIGH_1_ASID("MSC: Skip forceing preferences to HYBR_1 due to DDS change to SV/SR mode",
                       mm_id.asubs_id);
    ph_ptr->device_prop.msim_prop.is_dds_change_on_svlte_sub = FALSE;
    *cmph_hdr_kick_pending() = TRUE;
    cmmsc_auto_update_is_kick_hdr_pending(state_machine, TRUE);
    return;
  }
#endif

  /* Update pref reason as AC_EMERG when device in AC EMERG state
  ** to overide checks in SD for emergency preferences*/

  if(pref_update_reason == SD_SS_PREF_UPDATE_REASON_NONE &&
      cmac_is_in_emergency_only(mm_id.asubs_id))
  {
    pref_update_reason = SD_SS_PREF_UPDATE_REASON_AC_EMERG;
  }
  /*Update the Pref Reason to USER_SS_PREF if the user
     preferences are enforced over the FULL RAT object*/

  if(pref_update_reason == SD_SS_PREF_UPDATE_REASON_NONE &&
     cmph_is_user_ss_pref_in_full_rat(mm_id.asubs_id))
  {
    pref_update_reason = SD_SS_PREF_UPDATE_REASON_USER_SS_PREF_IN_FULL_RAT;
    cmph_update_user_ss_pref_in_full_rat(mm_id.asubs_id,FALSE);
  }
  pref_info->srv_domain_pref = cmtask_compute_srv_domain_pref(mm_id);

  /* Filter mode, band, and domain */
  cmmsc_get_stack_pref_per_cap(state_machine, stack_idx, pref_info, &common_pref);

  /* For emergency call make user mode preference as EMERGENCY so that SD
  ** can look for service on all possible RATs
  */
  
  if (cmcall_is_there_cellular_call_per_sub(state_machine->asubs_id,
                                         CM_CALL_TYPE_EMERG_MASK,TRUE) 
             != CM_CALL_ID_INVALID
      || (cmemg_get_reason_per_sub(state_machine->asubs_id) 
             == CM_AC_REASON_THIRD_PARTY_IMS_E911)
      || cmph_is_emerg_pending()
  )
  {
    is_emerg_call_actv = TRUE;
  }
  if((pref_info->orig_mode == SD_SS_ORIG_MODE_EMERG_ORIG && ph_orig_top_ptr->act_type != CM_ACT_TYPE_POLICYMAN_RESTRICT && is_emerg_call_actv)||
      pref_info->orig_mode == SD_SS_ORIG_MODE_EMERG_CB    ||
      pref_info->orig_mode == SD_SS_ORIG_MODE_1XCSFB_EMERG_ORIG ||
      pref_info->orig_mode == SD_SS_ORIG_MODE_PPCSFB_EMERG_ORIG)
  {
    /* For emergency call/full rat mode if domain is CS only, CM need not to add LTE in mode pref to SD */
    /* For L+L non DDS sub with ims cap, domain would be CS+VOLTE and CM needs to keep LTE in mode pref*/
    /* If LTE emergency call is not possible and user mode pref did not already include LTE,
       don't add LTE to the user mode pref */
    if((common_pref.domain == SYS_SRV_DOMAIN_CS_ONLY)
#if ( defined(FEATURE_3GPP_CSFB) || defined (FEATURE_LTE_TO_1X) || defined (FEATURE_IP_CALL) ) && defined FEATURE_LTE
        || (!cmsds_is_LTE_emerg_call_possible(mm_id.asubs_id) &&
            !cmcall_misc_is_mode_pref( cmph_get_ss_mode_pref(mm_id.asubs_id), CM_MODE_PREF_LTE_ONLY)
           )
#endif
      )
    {
      CM_MSG_HIGH_1("MSC: sub %d, Remove LTE as pref_info->srv_domain_pref is CS only", mm_id.asubs_id);
      if(cmph_is_msim())
      {
        user_mode_pref = SD_SS_MODE_PREF_ANY_BUT_LTE & \
                         cmutil_map_sys_mode_mask_to_sd_mode_pref(
                           cmmsc_get_curr_msc_max_cap(mm_id.asubs_id));
      }
      else
      {
        user_mode_pref = SD_SS_MODE_PREF_ANY_BUT_LTE;
      }
    }
    else
    {
      if(cmph_is_msim())
      {
        user_mode_pref = SD_SS_MODE_PREF_ANY & \
                         cmutil_map_sys_mode_mask_to_sd_mode_pref(
                           cmmsc_get_curr_msc_max_cap(mm_id.asubs_id));
      }
      else
      {
        user_mode_pref = SD_SS_MODE_PREF_ANY;
      }
    }
  }
  else
  {
    if(ph_orig_top_ptr->orig != NULL)
    {
      user_mode_pref = cmph_map_cm_mode_pref_to_sd_mode_pref(
                         ph_orig_top_ptr->orig->user_mode_pref);
    }
    else
    {
      CM_ERR_2("top element in the priority queue of sub %d stack %d is NULL", mm_id.asubs_id, mm_id.stk_id);
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmmsc_get_user_mode_pref_per_cap(state_machine, stack_idx, &user_mode_pref);
  if(mm_id.stk_id == MM_STACK_2 &&
      cmph_is_subs_feature_mode_srlte(mm_id.asubs_id) &&
      cmmsc_auto_is_hybr2_on(state_machine->op_mode))
  {
    user_mode_pref = SD_MODE_ADD(user_mode_pref,SD_SS_MODE_PREF_CDMA);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Set hybr pref to none if hybr is not allowed or single stack is active */
  if((mm_id.stk_id == MM_STACK_ALL || mm_id.stk_id == MM_STACK_0 ||
      (mm_id.stk_id == MM_STACK_1 && pref_reason == SD_SS_PREF_REAS_RESTORE))
      &&
      (!cmmsc_auto_is_hybr_allowed(user_mode_pref,
                                   cmutil_map_cm_hybr_pref_to_sd_hybr_pref(pref_info->hybr_pref))
       ||
       !cmmsc_is_msc_multi_stacks(mm_id.asubs_id))
    )
  {
    internal_hybr_pref = SD_SS_HYBR_PREF_NONE;
  }
  else if (cmtask_get_targeted_num_stacks(mm_id.asubs_id) == 1)
  {
    internal_hybr_pref = SD_SS_HYBR_PREF_NONE;
  }
  else
  {
    internal_hybr_pref = cmutil_map_cm_hybr_pref_to_sd_hybr_pref(pref_info->hybr_pref);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Get the addl info from addl_ptr */

  if(addl_ptr && addl_ptr->pref_update_reas)
  {
    pref_update_reason = *addl_ptr->pref_update_reas;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(cmph_is_subs_feature_mode_1x_sxlte(mm_id.asubs_id) &&
      mm_id.stk_id == MM_STACK_2 &&
      pref_reason != SD_SS_PREF_REAS_RESTORE)
  {
    *cmph_get_is_kicked_hybr_2_ptr() = TRUE;
    cmmsc_auto_update_is_kick_hybr2_pending(state_machine, FALSE);
  }
  if(cmmsc_get_1x_sub() == mm_id.asubs_id &&
      mm_id.stk_id == MM_STACK_1)
  {
    cmmsc_auto_update_is_kick_hdr_pending(state_machine, FALSE);
    *cmph_get_is_kicked_hybr_ptr() = TRUE;
  }

  if(CMPH_SS_NETWORK_SEL_MODE_PREF(mm_id.asubs_id,ph_ptr)   == CM_NETWORK_SEL_MODE_PREF_MANUAL &&
      mm_id.stk_id != MM_STACK_1 && pref_reason != SD_SS_PREF_REAS_USER_RESEL)  // In case of USER_RESEL dont use PM PLMN
  {
    sys_plmn_id_s_type  *plmn_ptr = NULL;
    sys_plmn_id_s_type   cmcc_plmn;
    sys_plmn_undefine_plmn_id(&cmcc_plmn);

    plmn_ptr = &ph_sub_ptr->pref_info.plmn;

    if(plmn_ptr != NULL && !sys_plmn_id_is_undefined(*plmn_ptr))
    {
      CM_MSG_HIGH_0("CM has a valid PLMN!!");
    }
    if(plmn_ptr != NULL && sys_plmn_id_is_undefined(*plmn_ptr) &&
        cmpmprx_get_persisted_user_plmn(mm_id.asubs_id, TRUE, &cmcc_plmn))
    {
      if(sys_plmn_id_is_undefined(cmcc_plmn))
      {
        CM_MSG_HIGH_0("PM returned undefined PLMN !!");
      }
      CM_MSG_HIGH_0("updating PM provided PLMN");
      *plmn_ptr = cmcc_plmn;
      pref_update_reason = SD_SS_PREF_UPDATE_REASON_USER_RESEL_PENDING;
      pref_info->sys_id.sys_id.id.plmn = cmcc_plmn;
      pref_info->sys_id.sys_id.id_type = SYS_SYS_ID_TYPE_UMTS;
      pref_info->sys_id.prm.gw.sel_type = SYS_MANUAL_SEL_TYPE_USER_SPECIFIED;
    }
    else
    {
      CM_MSG_HIGH_0("PM failed to read PLM or CM has a valid PLMN !!");
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Store LTE stack capability,
  ** used later to determine if PS detach or TAU should be triggered
  */
  cmmsc_store_curr_stack_capability_per_ss(state_machine,
      mm_id,
      pref_reason,
      user_mode_pref,
      common_pref,
      addl_action.action);

  #ifdef FEATURE_CM_LTE
  /* For L+L, dual standby, non-DDS sub, mode pref or PM rat cap change case,
  ** If plmn was blocked due to prev pref was LTE only,
  ** and current pref contains G/W/T + L, unblock plmn with cause voice not available
  ** for DDS switch case, plmn unblocking is handled by NAS
  */
  if(cmph_is_dual_lte() && ph_orig_top_ptr->act_type == CM_ACT_TYPE_PH_OBJ &&
    pref_reason == SD_SS_PREF_REAS_USER && mm_id.stk_id == MM_STACK_0 &&
    mm_id.asubs_id != ph_ptr->device_prop.msim_prop.default_data_subs &&   
    ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY &&
    sds_sub_prop_ptr != NULL && sds_sub_prop_ptr->gen_sub_info.is_block_sent_lte_only 
		  &&(
	     (
           !cmmsc_is_lte_only_sub(mm_id.asubs_id) &&
           (SD_MODE_CONTAIN(common_pref.mode, (int)SD_SS_MODE_PREF_LTE) &&
           (SD_MODE_CONTAIN(common_pref.mode, (int)SD_SS_MODE_PREF_GSM) ||
           SD_MODE_CONTAIN(common_pref.mode, (int)SD_SS_MODE_PREF_WCDMA) ||
           SD_MODE_CONTAIN(common_pref.mode, (int)SD_SS_MODE_PREF_TDS)))
	      &&  ph_ptr->device_prop.msim_prop.default_data_subs == ph_ptr->device_prop.msim_prop.new_default_data_subs 
	     ) 
	     ||
	     (
	      (sds_sub_prop_ptr->gen_sub_config.voice_domain_pref != SYS_VOICE_DOMAIN_PREF_NONE && sds_sub_prop_ptr->gen_sub_config.voice_domain_pref != SYS_VOICE_DOMAIN_PREF_CS_VOICE_ONLY)
          &&(sds_sub_prop_ptr->gen_sub_config.plmn_block_type == SYS_CMSDS_PLMN_BLOCK_IMS_CSFB &&
          !cmsds_determine_if_RAT_enabled(mm_id.asubs_id, SD_SS_MODE_PREF_TDS_GSM_WCDMA))
	     )      
       ))
	
	
	/*
    !cmmsc_is_lte_only_sub(mm_id.asubs_id) &&
    (SD_MODE_CONTAIN(common_pref.mode, (int)SD_SS_MODE_PREF_LTE) &&
      (SD_MODE_CONTAIN(common_pref.mode, (int)SD_SS_MODE_PREF_GSM) ||
      SD_MODE_CONTAIN(common_pref.mode, (int)SD_SS_MODE_PREF_WCDMA) ||
      SD_MODE_CONTAIN(common_pref.mode, (int)SD_SS_MODE_PREF_TDS)))
    )*/
  {
    cmsds_reset_is_block_sent_lte_only(mm_id.asubs_id);
    cmph_reg_send_block_plmn_reset_req(mm_id.asubs_id, SYS_BLOCK_PLMN_CAUSE_IMS_REGISTRATION_FAILURE);
  }
  #endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If DDS switch is permanent preceded by a temporary switch or a temporary DDS switch,
  ** indicate the same to MMOC/SD */
  if(cmph_is_msim() && mm_id.stk_id != MM_STACK_1  &&
     (ph_ptr->device_prop.msim_prop.is_dds_switch_type_temp  == TRUE) &&
     (ph_ptr->device_prop.msim_prop.prst_dual_standby_pref.default_data_subs == mm_id.asubs_id) 
    )
  {
    if(!(SD_MODE_CONTAIN(common_pref.mode, ~(int)SD_SS_MODE_PREF_TDS_GSM_WCDMA_LTE)))
    {
      CM_MSG_HIGH_0("Sending pref update reason as is_dds_switch_type_temp to MMOC");
      pref_update_reason = SD_SS_PREF_UPDATE_REASON_DDS_SWITCH_TYPE_TEMP;
    }
    ph_ptr->device_prop.msim_prop.is_dds_switch_type_temp = FALSE;
  }

  /* For DDS switch in dual-LTE config with no max cap change, both subs should
  ** start with acq db scan.
  */
  if(ph_ptr->device_prop.msim_prop.is_dual_lte_dds_switch && 
     mm_id.stk_id == MM_STACK_0)
  {
    CM_MSG_HIGH_2("sub %d stk %d set pref_update_reason DUAL_LTE_DDS_SWITCH",
                  mm_id.asubs_id,
                  mm_id.stk_id);
    pref_update_reason = SD_SS_PREF_UPDATE_REASON_DUAL_LTE_DDS_SWITCH;
  }

  CM_MSG_MED_6_EXT("MSC->MMOC: pref_chgd, sub %d, stk %d, is_dual_lte %d, mode_pref %d, subcap 0x%x, pm rat_cap 0x%x",
                    mm_id.asubs_id,
                    mm_id.stk_id,
                    cmph_is_dual_lte(),
                    pref_info->mode_pref,
                    state_machine->stack_common_info.ue_max_mode_capability,
                    state_machine->stack_common_info.ue_mode_capability,
                    (mm_id.asubs_id+1));

  CM_MSG_MED_6_EXT("MSC->MMOC: pref_chgd, internal_hybr_pref %d, hybr_pref %d, user_mode_pref 0x%x, ph srv_domain_pref %d, msc domain_cap %d, sd sys domain %d",
                   internal_hybr_pref, pref_info->hybr_pref, user_mode_pref,
                   pref_info->srv_domain_pref,
                   state_machine->stack_common_info.ue_domain_capability,
                   common_pref.domain,
                   (mm_id.asubs_id+1));

  if (orig_mode == SD_SS_ORIG_MODE_PPCSFB_NORMAL_ORIG && common_pref.mode == SD_SS_MODE_PREF_LTE)
  {
    // This is to debug an issue where SD goes in infinite loop and crashes. In those crashes we saw
    // the signature that CM is forcing preference with LTE only and CSFB. This error fatal is added to
    // do an error fatal intentionally when the signature is found.
    CM_ERR_FATAL_0("Unexpected PPCSFB ORIG with LTE only mode pref!! ");
  }
  
  /* Send command to MMOC */
  cmmsc_send_mmoc_pref_sys_chgd_cmd(mm_id,
                                    pref_reason,
                                    orig_mode,
                                    common_pref.mode,
                                    common_pref.band,
                                    common_pref.lte_band,
                                    common_pref.tds_band,
                                    cmutil_map_cm_prl_pref_to_sd_prl_pref(pref_info->prl_pref),
                                    cmutil_map_cm_roam_pref_to_sd_roam_pref(pref_info->roam_pref),
                                    internal_hybr_pref,
                                    pref_info->otasp_band,
                                    pref_info->otasp_blksys,
                                    pref_info->avoid_type,
                                    pref_info->avoid_time,
                                    &(pref_info->sys_id),
                                    cmmsc_map_sys_domain_to_sd(common_pref.domain),
                                    cmutil_map_cm_acq_order_pref_to_sd(pref_info->acq_order_pref),
                                    pref_update_reason,
                                    &addl_action,
                                    user_mode_pref,
                                    (const sd_rat_acq_order_s_type *)&sd_rat_acq_order,
                                    sys_sel_pref_req_id,
                                    camp_mode,
                                    csg_id,
                                    csg_rat,
                                    CMPH_SS_VOICE_DOMAIN_PREF(mm_id.asubs_id, ph_ptr),
                                    pref_info->fullrat_status,
                                    cmmsc_get_stk_mode_cap(mm_id),
                                    act_id
                                   );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Update last preferences which were sent */
  if(mm_id.stk_id != MM_STACK_1)
  {
    cm_act_orig_s_type *pref_sent_ptr = cmph_get_last_pref_sent(mm_id);

    if(pref_sent_ptr)
    {
      pref_sent_ptr->act_priority = CM_ACT_PRIORITY_PH;
      pref_sent_ptr->orig_band_pref = cmutil_map_sd_band_pref_to_cm_band_pref(common_pref.band);
      pref_sent_ptr->orig_lte_band_pref = common_pref.lte_band;
      pref_sent_ptr->orig_tds_band_pref = cmutil_map_sd_tds_band_pref_to_cm_tds_band_pref(common_pref.tds_band);
      pref_sent_ptr->orig_hybr_pref = cmutil_map_sd_hybr_pref_to_cm_hybr_pref(internal_hybr_pref);
      pref_sent_ptr->orig_mode = orig_mode;
      pref_sent_ptr->orig_mode_pref = cmph_map_sd_mode_pref_to_cm_mode_pref(common_pref.mode);
      pref_sent_ptr->orig_pref_term = CM_PREF_TERM_NONE;
      pref_sent_ptr->orig_prl_pref = pref_info->prl_pref;
      pref_sent_ptr->orig_roam_pref = pref_info->roam_pref;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* For L+G DSDS */
#ifdef FEATURE_MMODE_DUAL_SIM
  if(cmph_is_msim()&&
      (ph_ptr->device_prop.msim_prop.dds_status == CMPH_DDS_STATUS_PS_DETACH_PENDING))
  {
    /* If there's LTE call(s), change the dds_status to REMOVE_DATA_OBJ
    ** Else, reset the dds_status.
    */
    if(cmcall_is_no_call_in_lte_ps() == FALSE)
    {
      ph_ptr->device_prop.msim_prop.dds_status = CMPH_DDS_STATUS_REMOVE_DATA_OBJ;
    }
    else
    {
      ph_ptr->device_prop.msim_prop.dds_status = CMPH_DDS_STATUS_NONE;
    }
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* For svlte, if is_stack_sync_bsr_pending and call end, force preference */
  if(cmmsc_auto_is_stack_sync_bsr_pending(state_machine) &&
      mm_id.stk_id == MM_STACK_2 &&
      pref_reason == SD_SS_PREF_REAS_ORIG_END)
  {
    cmmsc_send_mmoc_pref_sys_chgd_cmd(mm_id,
                                      SD_SS_PREF_REAS_STACK_SYNC_UP_BSR,
                                      orig_mode,
                                      common_pref.mode,
                                      common_pref.band,
                                      common_pref.lte_band,
                                      common_pref.tds_band,
                                      cmutil_map_cm_prl_pref_to_sd_prl_pref(pref_info->prl_pref),
                                      cmutil_map_cm_roam_pref_to_sd_roam_pref(pref_info->roam_pref),
                                      internal_hybr_pref,
                                      pref_info->otasp_band,
                                      pref_info->otasp_blksys,
                                      pref_info->avoid_type,
                                      pref_info->avoid_time,
                                      &(pref_info->sys_id),
                                      cmmsc_map_sys_domain_to_sd(common_pref.domain),
                                      cmutil_map_cm_acq_order_pref_to_sd(pref_info->acq_order_pref),
                                      pref_update_reason,
                                      &addl_action,
                                      user_mode_pref,
                                      NULL,
                                      sys_sel_pref_req_id,
                                      camp_mode,
                                      csg_id,
                                      csg_rat,
                                      CMPH_SS_VOICE_DOMAIN_PREF(mm_id.asubs_id, ph_ptr),
                                      pref_info->fullrat_status,
                                      cmmsc_get_stk_mode_cap(mm_id),
                                      act_id
                                     );
    cmmsc_auto_update_is_stack_sync_bsr_pending(state_machine, FALSE);
  }

  if(ph_orig_top_ptr->orig != NULL)
  {
    ph_orig_top_ptr->orig->pref_update_reason = SD_SS_PREF_UPDATE_REASON_NONE;
  }
}

/*===========================================================================

FUNCTION cmmsc_store_curr_stack_capability_per_ss

DESCRIPTION
  This function is used to store the capability of the lte stack.
  The stored capability is used later to determine if a PS detach
  or TAU should be triggered.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmmsc_store_curr_stack_capability_per_ss(
  cmmsc_state_machine_s_type *state_machine,
  mm_sub_stk_id_s_type           mm_id,
  sd_ss_pref_reas_e_type pref_reason,
  sd_ss_mode_pref_e_type user_mode_pref,
  cmmsc_stack_capability_s_type common_pref,
  uint8                      action
)
{
  cm_orig_q_s_type       *ph_orig_top_ptr = cmtask_orig_para_get_top(mm_id);
  multimode_stack_e_type  stk_id = mm_id.stk_id;

  if(state_machine == NULL)
  {
    CM_ERR_0("cmmsc_store_curr_stack_capability_per_ss: state_machine NULL");
    return;
  }

  if(ph_orig_top_ptr == NULL)
  {
    CM_ERR_0("cmmsc_store_curr_stack_capability_per_ss: ph_orig_top_ptr NULL");
    return;
  }

  /* Save the capability only if the top of the priority queue
  ** is the phone object
  */
  if(ph_orig_top_ptr->act_type != CM_ACT_TYPE_PH_OBJ 
    &&
    !(ph_orig_top_ptr->act_type == CM_ACT_TYPE_DATA_CALL &&
      (action & MMOC_PREF_SYS_ADDL_ACTION_POLICY_PS_DETACH))
  )
  {
    return;
  }

  /* LTE stack can be the main stack or the hybr2 stack/hybr3 stack
  */
  if(pref_reason == SD_SS_PREF_REAS_USER || 
    pref_reason == SD_SS_PREF_REAS_RESTORE ||
    pref_reason == SD_SS_PREF_REAS_USER_RESEL ||
    (ph_orig_top_ptr->act_type == CM_ACT_TYPE_DATA_CALL &&
      (action & MMOC_PREF_SYS_ADDL_ACTION_POLICY_PS_DETACH))
  )
  {
    state_machine->prev_op_mode = state_machine->op_mode;

    state_machine->stack[stk_id].prev_registration_capability.mode = user_mode_pref;
    state_machine->stack[stk_id].prev_registration_capability.band = common_pref.band;
    state_machine->stack[stk_id].prev_registration_capability.tds_band= common_pref.tds_band;
    state_machine->stack[stk_id].prev_registration_capability.lte_band= common_pref.lte_band;
    state_machine->stack[stk_id].prev_registration_capability.domain= common_pref.domain;

    if (ph_orig_top_ptr->act_type == CM_ACT_TYPE_DATA_CALL)
    {
      CM_MSG_HIGH_3("STORE_CAP: sub %d, mode 0x%x, pref_reason %d", 
                       mm_id.asubs_id, user_mode_pref, pref_reason);
    }
  }
}

/*===========================================================================

FUNCTION cmmsc_send_mmoc_pref_sys_chgd_cmd

DESCRIPTION
  This function is used to command the MMoC to do Preferred system selection
  change. This function will queue the preferred system selection changed
  command to the MMoC Task.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void cmmsc_send_mmoc_pref_sys_chgd_cmd(
  mm_sub_stk_id_s_type           mm_id,
  /**< multimode id containing sub id and stack id.
  */

  sd_ss_pref_reas_e_type         pref_reas,
  /**< Reason for changing the SS-Preference.
  */

  sd_ss_orig_mode_e_type         orig_mode,
  /**< The origination mode (normal, OTASP, emergency, other, etc.)
  ** that is associated with the selected NAM.
  */

  sd_ss_mode_pref_e_type         mode_pref,
  /**< The mode preference that is associated with the selected NAM.
  */

  sd_ss_band_pref_e_type         band_pref,
  /**< Band preference that is associated with the selected NAM.
  */

  sys_lte_band_mask_e_type       lte_band_pref,
  /* LTE band preference that is associated with the selected NAM.
  */

  sd_ss_band_pref_e_type         tds_band_pref,
  /* TDS band preference that is associated with the selected NAM.
  */

  sd_ss_prl_pref_e_type          prl_pref,
  /**< PRL preference that is associated with the selected NAM.
  */

  sd_ss_roam_pref_e_type         roam_pref,
  /**< Roam preference that is associated with the selected NAM.
  */

  sd_ss_hybr_pref_e_type         hybr_pref,
  /**< New hybrid preference (none, CDMA/HDR).
  */

  sd_band_e_type                 otasp_band,
  /**< The band-class that is associated with OTASP origination mode.
  ** Note that this parameter is ignored if orig_mode != OTASP.
  */

  sd_blksys_e_type               otasp_blksys,
  /**< The PCS frequency block/Cellular system that is associated
  ** with OTASP origination mode. Note that this parameter is ignored
  ** if orig_mode != OTASP.
  */

  sd_ss_avoid_sys_e_type         avoid_type,
  /**< Type of the system to be avoided, valid only
  ** if pref_reason = SD_SS_PREF_REAS_AVOID_SYS.
  */

  dword                          avoid_time,
  /**< Time in seconds for which the system is to be avoided, valid
  ** only if pref_reason = SD_SS_PREF_REAS_AVOID_SYS.
  */

  sys_manual_sys_info_s_type    *manual_sys_info_ptr,
  /**< Pointer to struct that defines the manual system.
  */

  sd_ss_srv_domain_pref_e_type   srv_domain_pref,
  /**< Service domain preference that is associated with the selected
  ** NAM.
  ** Applies only for GSM/WCDMA modes.
  */

  sd_ss_acq_order_pref_e_type    acq_order_pref,
  /**< Preference for the order of acquisition ( WCDMA before GSM,
  ** GSM before WCDMA etc).
  */

  sd_ss_pref_update_reason_e_type  pref_update_reas,
  /**< Preference change reason
  */

  sys_addtl_action_s_type        *addl_action,
  /**< To indicate sub action to be performed by MMOC during pref sys chgd cmd
  */

  sd_ss_mode_pref_e_type         user_mode_pref,
  /**< The user/phone mode preference.
  */

  const sd_rat_acq_order_s_type  *rat_acq_order_ptr,
  /**< Rat acquisition order including LTE.
  */

  uint16                         req_id,
  /**< System Selection Preferences request id.
  */

  sd_ss_pref_camp_mode_e_type    camp_only_pref,

  sys_csg_id_type                csg_id,
  /**< CSG identifier */

  sys_radio_access_tech_e_type   csg_rat,
  /**< RAT specified for CSG */

  sys_voice_domain_pref_e_type   voice_domain_pref,
  /**< voice domain pref */

  sd_ss_fullrat_pref_e_type      fullrat_status,
  /* Indication to SD whether ue is in full rat
  */

  uint32                         rat_capability,
  /* Current rat capability
  */

  cm_act_id_type                 act_id
)
{

  if( cmph_is_subs_feature_mode_srlte(mm_id.asubs_id) &&
      cmph_is_oprting_in_csfb_pref_srlte(mm_id.asubs_id) &&
      (csg_id != SYS_CSG_ID_INVALID)
    )
  {
    /* CSG search and selection is not supported in SRLTE mode. So send 
           INVALID CSG parameters to SD
         */
         
    csg_id = SYS_CSG_ID_INVALID;
    csg_rat = SYS_RAT_NONE;

    /* Reset the plmn_id along with CSG parameters, if UE is not in manual mode */
    if(! ((orig_mode == SD_SS_ORIG_MODE_MANUAL_IDLE) ||
          (orig_mode == SD_SS_ORIG_MODE_MANUAL_ORIG)) )
    {
      if(manual_sys_info_ptr != NULL)
      {
        sys_plmn_undefine_plmn_id(&(manual_sys_info_ptr->sys_id.id.plmn));
      }
    }

  }
  
#ifdef FEATURE_CM_DEBUG_BUFFER
  {
    sys_modem_as_id_e_type as_id = mm_id.asubs_id;

    CMDBG_MEM_ALLOC(cm_mmoc_cmd_buff, cm_mmoc_prot_gen_cmd_entry_s_type)

    cm_mmoc_cmd_buff->mm_id             = mm_id;
    cm_mmoc_cmd_buff->pref_reas         = pref_reas;
    cm_mmoc_cmd_buff->orig_mode         = orig_mode;
    cm_mmoc_cmd_buff->mode_pref         = mode_pref;
    cm_mmoc_cmd_buff->band_pref         = band_pref;
    cm_mmoc_cmd_buff->lte_band_pref     = lte_band_pref;
    cm_mmoc_cmd_buff->tds_band_pref     = tds_band_pref;
    cm_mmoc_cmd_buff->roam_pref         = roam_pref;
    cm_mmoc_cmd_buff->hybr_pref         = hybr_pref;
    cm_mmoc_cmd_buff->avoid_type        = avoid_type;
    cm_mmoc_cmd_buff->avoid_time        = avoid_time;
    cm_mmoc_cmd_buff->domain_pref       = srv_domain_pref;
    cm_mmoc_cmd_buff->pref_update_reas  = pref_update_reas;
    cm_mmoc_cmd_buff->addl_action       = *addl_action;
    cm_mmoc_cmd_buff->user_mode_pref    = user_mode_pref;
    cm_mmoc_cmd_buff->camp_mode_pref    = camp_only_pref;
    cm_mmoc_cmd_buff->fullrat_status     = fullrat_status;
    cm_mmoc_cmd_buff->voice_domain_pref = voice_domain_pref;
    cm_mmoc_cmd_buff->mode_cap          = cmmsc_get_msc_rat_cap(as_id);
    cm_mmoc_cmd_buff->stack_mode_cap    = cmmsc_get_stk_mode_cap(mm_id);
    cm_mmoc_cmd_buff->ph_mode_pref      = CMPH_SS_MODE_PREF(as_id, cmph_ptr());
    cm_mmoc_cmd_buff->prst_ph_mode_pref = CMPH_PRST_SS_MODE_PREF(as_id, cmph_ptr());
    cm_mmoc_cmd_buff->act_id = act_id;
    cm_mmoc_cmd_buff->pending_emerg_state=cmph_get_emerg_pending_info().state;

    cmdbg_add_to_dbg_buffer(CM_BUFF_MMOC_CMD_TYPE, MMOC_CMD_PROT_GEN_CMD, ((void *)cm_mmoc_cmd_buff));
  }
#endif

  mmoc_cmd_pref_sys_chgd7(mm_id,
                          pref_reas,
                          orig_mode,
                          mode_pref,
                          band_pref,
                          lte_band_pref,
                          tds_band_pref,
                          prl_pref,
                          roam_pref,
                          hybr_pref,
                          otasp_band,
                          otasp_blksys,
                          avoid_type,
                          avoid_time,
                          manual_sys_info_ptr,
                          srv_domain_pref,
                          acq_order_pref,
                          pref_update_reas,
                          addl_action,
                          user_mode_pref,
                          rat_acq_order_ptr,
                          req_id,
                          camp_only_pref,
                          csg_id,
                          csg_rat,
                          voice_domain_pref,
                          fullrat_status,
                          rat_capability,
                          cmph_get_emerg_pending_info().state
                         );
}

/*===========================================================================

FUNCTION cmmsc_form_sub_data_simx

DESCRIPTION
  This function forms subscription info for sim0 to send to mmoc.

DEPENDENCIES
  cmpref2_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_form_sub_data_simx(
  uint8                   sub_avail,
  /* containing the info of subs_avail for each sub
  */

  cm_pref_s_type         *pref_info,
  /* pref_info pointer
  */

  mmoc_subs_data_s_type  *sub_data,
  /* sub_data pointer
  */

  uint8                   sim_idx,

  mmoc_subsc_chg_e_type   chg_type
)
{
  sd_ss_mode_pref_e_type           mode_pref;
  sd_ss_hybr_pref_e_type           hybr_pref;
  cm_network_sel_mode_pref_e_type  network_sel_mode_pref;
  sd_ss_orig_mode_e_type           orig_mode;
  sys_manual_sys_info_s_type       manual_sys_info;
  sd_ss_band_pref_e_type           band;
  sd_ss_band_pref_e_type           tds_band;
  sys_lte_band_mask_e_type         lte_band;
  sd_ss_srv_domain_pref_e_type     domain;
  cmmsc_state_machine_s_type      *state_machine;

  mm_sub_stk_id_s_type mm_id;
  sys_modem_as_id_e_type           as_id = (sys_modem_as_id_e_type)sim_idx;


  mm_id.asubs_id = as_id;
  mm_id.stk_id = MM_STACK_0;


  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  state_machine = cmmsimc_state_machine_ptr(as_id);

  /* Validate params
  */
  CM_ASSERT(pref_info != NULL);
  CM_ASSERT(sub_data != NULL);

  if(pref_info == NULL || sub_data == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  if(state_machine == NULL)
  {
    CM_ERR_FATAL_0("MSC: state_machine null ptr");
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  mode_pref = cmph_map_cm_mode_pref_to_sd_mode_pref(pref_info->mode_pref);
  band      = cmutil_map_cm_band_pref_to_sd_band_pref(pref_info->band_pref);
  tds_band  = cmutil_map_cm_tds_band_pref_to_sd_tds_band_pref(pref_info->tds_band_pref);
  lte_band  = pref_info->lte_band_pref;
  domain    = cmutil_map_cm_srv_domain_pref_to_sd(pref_info->srv_domain_pref);

  hybr_pref = cmutil_map_cm_hybr_pref_to_sd_hybr_pref(pref_info->hybr_pref);
  hybr_pref = cmmsc_auto_is_hybr_allowed(mode_pref, hybr_pref) ? hybr_pref: SD_SS_HYBR_PREF_NONE;

  network_sel_mode_pref = pref_info->network_sel_mode_pref;
  orig_mode = cmutil_map_cm_network_sel_mode_pref_to_orig_mode(
                network_sel_mode_pref);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Fill Subscription Information */
  sub_data->as_id           = as_id;
  sub_data->nv_context      = CMPH_SS_NV_CONTEXT(as_id, cmph_ptr());
  sub_data->sub_avail       = sub_avail;

  /* If there is a CS or emerg call ongoing, we want to use the current max_cap instead
  ** of the max_cap from PM. This is because PM might have changed the max_cap and
  ** sending a max_cap change to MMOC could cuase the call to drop.
  */
  if(!cmpmprx_is_ok_to_enforce_device_policy_not_check_mt_page() && 
      chg_type != MMOC_SUBSC_CHG_MODE_CHNG)
  {
    sub_data->subs_capability = cmmsc_get_curr_msc_max_cap(as_id);
  }
  else
  {
    sub_data->subs_capability = cmpmprx_fetch_max_mode_capability(as_id);
  }
  sub_data->rat_capability = (uint32)cmmsc_get_msc_rat_cap(as_id);

  sub_data->is_perso_locked = FALSE;

  sub_data->orig_mode       = orig_mode;
  sub_data->mode_pref       = mode_pref;
  sub_data->band_pref       = band;
  sub_data->tds_band_pref   = tds_band;
  sub_data->lte_band_pref   = lte_band;
  sub_data->voice_domain_pref = *(CMPH_SS_VOICE_DOMAIN_PREF_PTR(as_id, cmph_ptr()));

  sub_data->roam_pref       = cmutil_map_cm_roam_pref_to_sd_roam_pref( pref_info->roam_pref);
  sub_data->active_stacks   = cmtask_map_num_stacks_to_bitmask(cmtask_get_targeted_num_stacks(as_id));

  /* 3GPP specific data */
  sub_data->acq_order_pref  = cmutil_map_cm_acq_order_pref_to_sd(pref_info->acq_order_pref);
  sub_data->srv_domain_pref = domain;
  manual_sys_info.sys_id.id_type  = SYS_SYS_ID_TYPE_UMTS;
  manual_sys_info.prm.gw.sel_type = SYS_MANUAL_SEL_TYPE_USER_SPECIFIED;
  manual_sys_info.sys_id.id.plmn  = pref_info->plmn;
  sub_data->manual_sys_info = manual_sys_info;

  /* 3GPP2 specific data */
  sub_data->prl_pref        = cmutil_map_cm_prl_pref_to_sd_prl_pref(pref_info->prl_pref);
  sub_data->hybr_pref       = hybr_pref;

  /* UE mode and sub-mode */
  sub_data->ue_mode         =   state_machine->stack_common_info.ue_mode;
  if(state_machine->op_mode == CMMSC_OP_MODE_AUTO_MAIN_HYBR1_HYBR2
      &&
      (state_machine->stack_common_info.ue_mode == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED ||
       state_machine->stack_common_info.ue_mode == SYS_UE_MODE_1XSRLTE_ONLY)
    )
  {
    sub_data->is_ue_mode_substate_srlte = TRUE;
  }
  else
  {
    sub_data->is_ue_mode_substate_srlte = FALSE;
  }

    sub_data->camp_mode  =
      cmutil_map_cm_net_sel_mode_pref_to_camp_pref(network_sel_mode_pref);
 
  sub_data->fullrat_status = cmpmprx_compute_fullrat_status(mm_id,network_sel_mode_pref,cmpm_sub_ptr(mm_id.asubs_id));

  /* session type */
  sub_data->gpp_session_type= AS_ID_TO_GW_SESSION(as_id);
  sub_data->gpp2_session_type= AS_ID_TO_1X_SESSION(as_id);

  CM_MSG_HIGH_6("SUB MSC->MMOC: sim_idx=%d, band_pref: cgw=0x%08x %08x. TDS = 0x%08x %08x voice_domain_pref %d",
                sim_idx,
                QWORD_HIGH(band),QWORD_LOW(band),
                QWORD_HIGH(tds_band), QWORD_LOW(tds_band), sub_data->voice_domain_pref);

  CM_MSG_HIGH_2_EXT("SUB MSC->MMOC: network_sel_mode_pref %d camp_mode %d",
                    network_sel_mode_pref, sub_data->camp_mode, (as_id+1));


  cm_print_lte_band_mask(lte_band);

  return;
}

/*===========================================================================

FUNCTION cmmsc_store_curr_stack_capability_per_as_id

DESCRIPTION
  This function is used to store the capability of the lte stack.
  The stored capability is used later to determine if a PS detach
  or TAU should be triggered.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmmsc_store_curr_stack_capability_per_as_id(
  sys_modem_as_id_e_type          as_id
)
{

  cmmsc_state_machine_s_type      *state_machine;
  cmmsc_stack_capability_s_type   stack_pref;
  multimode_stack_e_type stk_idx;

  if (!cmutil_is_as_id_valid(as_id))
  {
    CM_MSG_HIGH_1("Invalid as_id %d",as_id);
    return;
  }

  memset(&stack_pref, 0, sizeof(cmmsc_stack_capability_s_type));
  state_machine = cmmsimc_state_machine_ptr_per_sim_idx((int)as_id);
  if(state_machine)
  {
    cm_mode_pref_e_type user_mode_pref;
    cm_orig_q_s_type *orig_ptr_top;
    sys_lte_band_mask_e_type lte_band = CMPH_SS_LTE_BAND_PREF(as_id, cmph_ptr());

    for(stk_idx=MM_STACK_0; stk_idx<cmtask_get_num_stacks(as_id); stk_idx++)
    {
      if(stk_idx == MM_STACK_1)
      {
        continue;
      }
      orig_ptr_top = cmtask_orig_para_get_top(cmph_map_sub_stk_to_id(as_id, stk_idx));
      if (stk_idx == MM_STACK_2 && cmmsc_auto_is_hybr2_on(state_machine->op_mode))
      {
        user_mode_pref = SD_MODE_ADD(user_mode_pref,SD_SS_MODE_PREF_CDMA);
      }
      stack_pref = state_machine->stack[stk_idx].capablity;
      /* Update stack preference based on capability */
      stack_pref.mode =
        SD_GET_COMMON_MODE( stack_pref.mode,
                            cmph_map_cm_mode_pref_to_sd_mode_pref(CMPH_SS_MODE_PREF(as_id, cmph_ptr())));

      stack_pref.band =
        SD_GET_COMMON_BAND( stack_pref.band,
                            cmutil_map_cm_band_pref_to_sd_band_pref(CMPH_SS_BAND_PREF(as_id, cmph_ptr())));

      stack_pref.tds_band =
        SD_GET_COMMON_BAND( stack_pref.tds_band,
                            cmutil_map_cm_band_pref_to_sd_band_pref(CMPH_SS_TDS_BAND_PREF(as_id, cmph_ptr())));

      stack_pref.lte_band =
        SYS_LTE_BAND_MASK_COMMON( &stack_pref.lte_band,
                                  &lte_band);

      user_mode_pref = (orig_ptr_top != NULL && orig_ptr_top->orig!= NULL)?orig_ptr_top->orig->user_mode_pref:\
                       CMPH_SS_MODE_PREF(as_id,cmph_ptr());

      cmmsc_store_curr_stack_capability_per_ss(
        cmmsimc_state_machine_ptr_per_sim_idx((int)as_id),
        cmph_map_sub_stk_to_id(as_id, stk_idx),
        SD_SS_PREF_REAS_USER,
        cmph_map_cm_mode_pref_to_sd_mode_pref(user_mode_pref) & stack_pref.mode,
        stack_pref,
        (uint8)MMOC_PREF_SYS_ADDL_ACTION_NONE
      );

    }
  }
}

/*===========================================================================

FUNCTION cmmsc_proc_cmd_subsc_chgd_set_capability

DESCRIPTION
  This function will set the capability of each stacks, based on given subsc
  scription. We want to split the subscription process as subscription is sent
  on Main, but capability is set from stack (cmmsc).
  - CMMSIMC: 1. set subscription capability
  - CMMSIMC: 2. send subscription on MAIN
  - CMMSIMC: 3. Trigger CMMSC.

  Key note: we don't want to trigger CMMSC Routing Table before CMMSIMC because
  we might take action on CMMSC, which we don't want to take before subscription
  are sent.

DEPENDENCIES
  cmmsc_init_multi_stacks() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_proc_cmd_subsc_chgd_set_capability(
  cmmsc_param_subs_chgd_s_type  *subs_event
)
{
  uint8                        sim_idx = 0;
  cmmsc_state_machine_s_type  *state_machine = NULL;
  boolean is_op_mode_chgd = FALSE;
  cm_orig_q_s_type  *ph_orig_top_ptr = NULL;
  cm_network_sel_mode_pref_e_type  network_sel_mode_pref;

  if(subs_event == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  for(sim_idx = 0; sim_idx < MIN(MAX_SIMS, (cmpmprx_get_num_of_sims())); sim_idx++)
  {
    if((state_machine = cmmsimc_state_machine_ptr_per_sim_idx(sim_idx)) == NULL)
    {
      CM_MSG_HIGH_1("Could not get state_machine for sim %d", sim_idx);
      continue;
    }
  
    if(subs_event->prot_sub_chg[sim_idx] || subs_event->apply_standby_pref_chg)
    {
      /* Update op mode per feature mode */

      /* If subscription is available just after emergency call end(<4 secs)
      ** Update this boolean with latest status
      */
      state_machine->stack_common_info.is_in_emergency_call = cmcall_emergency_call_active();

      /* Update op mode per feature mode */
      if(cmmsc_auto_update_op_mode(state_machine,
                                   FALSE,
                                   TRUE,
                                   subs_event->sub_data[sim_idx]->mode_pref,
                                   subs_event->sub_data[sim_idx]->hybr_pref,
                                   ((subs_event->sub_data[sim_idx]->sub_avail & MASK_GPP) != 0),
                                   ((subs_event->sub_data[sim_idx]->sub_avail & MASK_GPP2) != 0),
                                   NULL))
      {
        is_op_mode_chgd = TRUE;
      }

      cmmsc_set_stack_capability(state_machine);
      cmmsc_store_curr_stack_capability_per_as_id(state_machine->asubs_id);

      /* If the device is already in full rat mode and the capability is changed, we must
      ** recompute the net_sel_pref as per latest mode cap and continue to force as per POLICYMAN_RESTRICT. This will
      ** ensure that the fullrat_status and ORIG_MODE are correctly forced to MMOC/SD.
      */
      ph_orig_top_ptr = cmtask_orig_para_get_top(cmph_map_sub_stk_to_id(state_machine->asubs_id,MM_STACK_0));
      
      if(ph_orig_top_ptr != NULL &&
         ph_orig_top_ptr->act_type == CM_ACT_TYPE_POLICYMAN_RESTRICT)
      {
        if(ph_orig_top_ptr->orig->orig_mode == SD_SS_ORIG_MODE_EMERG_ORIG )
        {
          network_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_LIMITED_SRV;
        }
        else
        {
          switch(cmph_ptr()->sub_info[state_machine->asubs_id]->pref_info.network_sel_mode_pref)
          {
             case CM_NETWORK_SEL_MODE_PREF_MANUAL_CAMP_ONLY:
             case CM_NETWORK_SEL_MODE_PREF_MANUAL_LIMITED_SRV:
             case CM_NETWORK_SEL_MODE_PREF_MANUAL:
               network_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_MANUAL_CAMP_ONLY;
               break;
  
             default:
               network_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_AUTO_CAMP_ONLY;
               break;
          }
        }

        network_sel_mode_pref =
          cmpmprx_compute_restricted_mode( state_machine->asubs_id, network_sel_mode_pref );

        ph_orig_top_ptr->orig->orig_mode = subs_event->sub_data[sim_idx]->orig_mode = \
          cmutil_map_cm_network_sel_mode_pref_to_orig_mode(network_sel_mode_pref);

        subs_event->sub_data[sim_idx]->camp_mode =
        cmutil_map_cm_net_sel_mode_pref_to_camp_pref(network_sel_mode_pref);
      }

    }
    else
    {
      cmmsc_set_stack_capability(state_machine);
      cmmsc_store_curr_stack_capability_per_as_id(state_machine->asubs_id);
    }
  }

  /* OP mode change, if SXLTE and sub feature mode is SVLTE inform
  ** MCS of new simul_cap */
  if(is_op_mode_chgd)
  {
    cmph_sp_subs_info_update();
  }
}

/*===========================================================================

FUNCTION cmmsc_proc_cmd_subsc_chgd

DESCRIPTION
  This function should be called when the subscrption availability has
  changed.This function will pack the info and in turn call the function
  cmmsc_proc_cmd_subsc_chgd_.

DEPENDENCIES
  cmmsc_init_multi_stacks() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void cmmsc_proc_cmd_subsc_chgd(
  cmmsc_param_subs_chgd_s_type  *subs_event
)
{
  cmmsc_event_e_type            event = CMMSC_EVENT_NONE;
  cmmsc_event_sub_chg_s_type    event_sub_chg;
  cmmsc_state_machine_s_type   *state_machine
    = cmmsimc_state_machine_ptr(SYS_MODEM_AS_ID_1);
  size_t                        sim_idx = 0;

  event = CMMSC_EVENT_SUB;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  for(sim_idx = 0; sim_idx < MIN(MAX_SIMS, (cmpmprx_get_num_of_sims())); sim_idx++)
  {
    if((subs_event->prot_sub_chg[sim_idx] != PROT_SUBSC_CHG_NONE ||
        subs_event->apply_standby_pref_chg)
        &&
        (subs_event->active_subs & BM(sim_idx))
      )
    {
      state_machine = cmmsimc_state_machine_ptr((sys_modem_as_id_e_type)sim_idx);
      /* Setting stacks capability */
      cmmsc_set_stack_capability(state_machine);

      if(cmph_is_subs_feature_mode_1x_sxlte(state_machine->asubs_id))
      {
        CM_MSG_HIGH_0("MSC: reset kick");
        *cmph_get_is_kicked_hybr_ptr() = FALSE;
        *cmph_get_is_kicked_hybr_2_ptr() = FALSE;
      }

      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

      /* Package the event with parameters */
      event_sub_chg.event = event;
      event_sub_chg.state_machine = state_machine;
      event_sub_chg.nam = subs_event->nam;
      event_sub_chg.chg_type = subs_event->chg_type;
      memscpy(event_sub_chg.prot_sub_chg,
              sizeof(prot_subsc_chg_e_type)*MAX_SIMS,
              subs_event->prot_sub_chg,
              sizeof(prot_subsc_chg_e_type)*MAX_SIMS);

      memscpy(event_sub_chg.sub_data,
              sizeof(mmoc_subs_data_s_type *)*MAX_SIMS,
              subs_event->sub_data,
              sizeof(mmoc_subs_data_s_type *)*MAX_SIMS);

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      if(subs_event->prot_sub_chg[sim_idx] != PROT_SUBSC_CHG_NONE)
      {
        state_machine->stack_common_info.ue_mode_forced = TRUE;
      }

      /* Consult & Process in Routing Table */
      cmmsc_state_machine_process(state_machine,
                                  (cmmsc_event_base_s_type *)&event_sub_chg);
    }
  }
}

void cmmsc_proc_cmd_on_off(
  boolean                   is_stack_deactivated,
  cmmsc_param_base_s_type  *event_package
)
{
  cmmsc_event_e_type        event = CMMSC_EVENT_NONE;
  cmmsc_event_base_s_type   event_on_off;

  if(is_stack_deactivated == FALSE)
  {
    event = CMMSC_EVENT_ON;
  }
  else
  {
    event = CMMSC_EVENT_OFF;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  event_on_off.event = event;
  event_on_off.state_machine = event_package->state_machine;

  /* Consult & Process in Routing Table */
  cmmsc_state_machine_process(event_on_off.state_machine, &event_on_off);
}

void cmmsc_update_sub_data_per_cap(
  mmoc_subs_data_s_type  *sub_other_data,
  /* Subscription data for sim != 0
  */

  size_t                  sim_idx
)
{
  cmmsc_state_machine_s_type  *state_machine = cmmsimc_state_machine_ptr_per_sim_idx(sim_idx);
  /* Always take the capability of first stack because subscription is sent only on ss=MAIN */
  cmmsc_stack_s_type          *stack_pointer = NULL;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if(sub_other_data == NULL || state_machine == NULL)
  {
    return;
  }

  stack_pointer = &state_machine->stack[0];

  /* Update stack preference based on capability */
  sub_other_data->mode_pref =
    SD_GET_COMMON_MODE(sub_other_data->mode_pref,
                       stack_pointer->capablity.mode);

  sub_other_data->band_pref =
    SD_GET_COMMON_BAND(sub_other_data->band_pref,
                       stack_pointer->capablity.band);

  sub_other_data->tds_band_pref =
    SD_GET_COMMON_BAND(sub_other_data->tds_band_pref,
                       stack_pointer->capablity.tds_band);

  sub_other_data->lte_band_pref =
    SYS_LTE_BAND_MASK_COMMON(&(sub_other_data->lte_band_pref),
                             &(stack_pointer->capablity.lte_band));

  sub_other_data->srv_domain_pref =
    cmmsc_cal_domain_pref(sub_other_data->srv_domain_pref,
                            cmmsc_map_sys_domain_to_sd(stack_pointer->capablity.domain),
                            (sys_modem_as_id_e_type)sim_idx);

  return;
}

static void cmmsc_get_stack_pref_per_cap(
  cmmsc_state_machine_s_type     *state_machine,
  int                             stack_idx,
  cm_preference_info_s_type      *pref_info,
  cmmsc_stack_capability_s_type  *common_cap_ptr
)
{
  cmmsc_stack_s_type  *stack_pointer;

  if(pref_info == NULL)
  {
    CM_ERR_0("MSC: pref info null ptr");
    return;
  }

  if(common_cap_ptr == NULL)
  {
    CM_ERR_0("MSC: stack_cap_ptr null");
    return;
  }

  if(state_machine == NULL)
  {
    return;
  }

  stack_pointer = &state_machine->stack[stack_idx];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Get mode pref from hybr1 specific pref */
  if(stack_idx == 1)
  {
    common_cap_ptr->mode =
      SD_GET_COMMON_MODE(
        cmph_map_cm_mode_pref_to_sd_mode_pref(pref_info->hybr1_pref.hybr_mode_pref),
        stack_pointer->capablity.mode);
  }
  else
  {
    common_cap_ptr->mode =
      SD_GET_COMMON_MODE(
        cmph_map_cm_mode_pref_to_sd_mode_pref(pref_info->mode_pref),
        stack_pointer->capablity.mode);
  }

  common_cap_ptr->band =
    SD_GET_COMMON_BAND(
      cmutil_map_cm_band_pref_to_sd_band_pref(pref_info->band_pref),
      stack_pointer->capablity.band);

  common_cap_ptr->tds_band =
    SD_GET_COMMON_BAND(
      cmutil_map_cm_tds_band_pref_to_sd_tds_band_pref(pref_info->tds_band_pref),
      stack_pointer->capablity.tds_band);

  common_cap_ptr->lte_band =
    SYS_LTE_BAND_MASK_COMMON(&(pref_info->lte_band_pref),
                             &(stack_pointer->capablity.lte_band));

  common_cap_ptr->domain =
    cmutil_map_sd_srv_domain_pref_to_sys(
      cmmsc_cal_domain_pref(
        cmutil_map_cm_srv_domain_pref_to_sd(pref_info->srv_domain_pref),
        cmmsc_map_sys_domain_to_sd(stack_pointer->capablity.domain),
        state_machine->asubs_id));

  if (stack_idx == 0)
  {
    CM_MSG_HIGH_5("sub %d stk %d, domain pref %d, domain_cap %d, domain to sd %d",
                  state_machine->asubs_id,
                  stack_idx,
                  pref_info->srv_domain_pref,
                  stack_pointer->capablity.domain, common_cap_ptr->domain);
  }
}

static void cmmsc_get_user_mode_pref_per_cap(
  cmmsc_state_machine_s_type   *state_machine,
  int                           stack_idx,
  sd_ss_mode_pref_e_type       *user_mode_pref
)
{
  cmmsc_stack_s_type *stack_pointer = &state_machine->stack[stack_idx];

  if(user_mode_pref == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  *user_mode_pref =
    SD_GET_COMMON_MODE( *user_mode_pref, stack_pointer->capablity.mode);
}

sd_ss_srv_domain_pref_e_type cmmsc_get_common_domain(
  sd_ss_srv_domain_pref_e_type  domain1,
  sd_ss_srv_domain_pref_e_type  domain2
)
{
  /* Get the common domain pref of 2 domain_pref */
  if(domain1 == domain2)
  {
    return domain1;
  }
  else if((domain1 == SD_SS_SRV_DOMAIN_PREF_CS_ONLY
           && domain2 == SD_SS_SRV_DOMAIN_PREF_PS_ONLY) ||
          (domain1 == SD_SS_SRV_DOMAIN_PREF_PS_ONLY
           && domain2 == SD_SS_SRV_DOMAIN_PREF_CS_ONLY))
  {
    CM_MSG_HIGH_2("MSC: Common domain none - domain1 %d domain2 %d", domain1, domain2);
    return SD_SS_SRV_DOMAIN_PREF_NONE;
  }
  else if(domain1 == SD_SS_SRV_DOMAIN_PREF_CS_PS)
  {
    return domain2;
  }
  else if(domain2 == SD_SS_SRV_DOMAIN_PREF_CS_PS)
  {
    return domain1;
  }
  else if(cmph_is_dual_lte())
  {
    /* capability will only be changed to CS+PS or CS+VOLTE or CS only
    ** other cases like VOLTE only is not covered here
    */
    if(domain1 == SD_SS_SRV_DOMAIN_PREF_CS_VOLTE)
    {
      if(domain2 == SD_SS_SRV_DOMAIN_PREF_CS_ONLY)
      {
        return SD_SS_SRV_DOMAIN_PREF_CS_VOLTE;
      }
      else
      {
        return SD_SS_SRV_DOMAIN_PREF_VOLTE;
      }
    }
    else if(domain2 == SD_SS_SRV_DOMAIN_PREF_CS_VOLTE)
    {
      if(domain1 == SD_SS_SRV_DOMAIN_PREF_CS_ONLY)
      {
        return SD_SS_SRV_DOMAIN_PREF_CS_VOLTE;
      }
      else
      {
        return SD_SS_SRV_DOMAIN_PREF_VOLTE;
      }
    }
    else
    {
      CM_MSG_HIGH_2("MSC: Common domain none - domain1 %d domain2 %d", domain1, domain2);
      return SD_SS_SRV_DOMAIN_PREF_NONE;
    }
  }
  else
  {
    CM_MSG_HIGH_2("MSC: Common domain none - domain1 %d domain2 %d", domain1, domain2);
    return SD_SS_SRV_DOMAIN_PREF_NONE;
  }
}

/*===========================================================================

FUNCTION cmmsc_cal_domain_pref

DESCRIPTION
  This function returns the final effective domain based on if the sub is dds, input domain preferences.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
sd_ss_srv_domain_pref_e_type cmmsc_cal_domain_pref(
  sd_ss_srv_domain_pref_e_type  domain1,
  sd_ss_srv_domain_pref_e_type  domain2,
  sys_modem_as_id_e_type        asubs_id
)
{
  sd_ss_srv_domain_pref_e_type resulted_domain = SD_SS_SRV_DOMAIN_PREF_NONE;
  boolean is_dual_lte = cmph_is_dual_lte();

  resulted_domain = cmmsc_get_common_domain(domain1, domain2);
  
  /* For dual ims configuration, */
  if ( is_dual_lte && 
     asubs_id != cmph_ptr()->device_prop.msim_prop.new_default_data_subs &&
     cmph_is_mms_ongoing(asubs_id) &&
     (domain1 == SD_SS_SRV_DOMAIN_PREF_CS_PS || 
      domain2 == SD_SS_SRV_DOMAIN_PREF_CS_PS)
  )
  {
    resulted_domain = SD_SS_SRV_DOMAIN_PREF_CS_PS;
  }
  CM_MSG_HIGH_6("DOMAIN: sub %d, is_dual_lte %d, domain1 %d, domain2 %d, resulted domain %d, dds sub %d",
                   asubs_id, is_dual_lte, domain1, domain2, resulted_domain,
                   cmph_ptr()->device_prop.msim_prop.new_default_data_subs);
  return resulted_domain;

}


static void  cmmsc_print_route_table_info(
  cmmsc_state_machine_s_type *state_machine,
  cmmsc_event_e_type  event,
  int                 route_tbl_idx
)
{
  cmmsc_routing_table_s_type *route_table = cmmsc_route_table_ptr();
  cmmsc_routing_table_auto_entry_s_type *auto_entry =
    (cmmsc_routing_table_auto_entry_s_type *)route_table->route_tbl;

  /* Print event and op mode UE will operate on */
  if (route_tbl_idx != -1)
  {
    CM_MSG_HIGH_5_ASID("MSC_TABLE: ue_mode=%d, tbl event=%d, op_mode=%d, route_tbl_idx=%d",
                       state_machine->asubs_id,
                       state_machine->stack_common_info.ue_mode,
                       event,
                       state_machine->op_mode,
                       route_tbl_idx);
  }
  else
  {
    CM_MSG_HIGH_5("MSC_TABLE: sub=%d, ue_mode=%d, tbl_id=%d,  tbl event=%d, op_mode=%d, No entry found",
                  state_machine->asubs_id,
                  state_machine->stack_common_info.ue_mode,
                  route_tbl_idx,
                  event,
                  state_machine->op_mode);
  }

  /* Print current stack state */
  if(route_table->n_stack == CMMSC_AUTO_STACK_NUM && ARR_SIZE(state_machine->stack) >= 3)
  {
    CM_MSG_HIGH_6("MSC_TABLE: stk0 state %d->%d, stk1 state %d->%d, stk2 state %d->%d",
                  state_machine->stack[0].state,
                  auto_entry[route_tbl_idx].new_state[0],
                  state_machine->stack[1].state,
                  auto_entry[route_tbl_idx].new_state[1],
                  state_machine->stack[2].state,
                  auto_entry[route_tbl_idx].new_state[2]);
  }
}

/*===========================================================================

FUNCTION cmmsc_is_op_mode_multi_stacks

DESCRIPTION
  This function returns if the op_mode is multi-stack operation.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static boolean cmmsc_is_op_mode_multi_stacks(
  cmmsc_state_machine_op_mode_e_type  op_mode
)
{
  switch(op_mode)
  {
    case CMMSC_OP_MODE_NORMAL:
    case CMMSC_OP_MODE_FULL_RAT_SEARCH:
      return FALSE;

    /* feature mask auto specific op mode */
    case CMMSC_OP_MODE_AUTO_MAIN_HYBR1:
    case CMMSC_OP_MODE_AUTO_MAIN_HYBR2:
    case CMMSC_OP_MODE_AUTO_MAIN_HYBR1_HYBR2:
      return TRUE;

    /* feature mask auto specific op mode */
    case CMMSC_OP_MODE_AUTO_MAIN:
      return FALSE;

    /* Invalid op_mode for this function */
    case CMMSC_OP_MODE_NONE:
    case CMMSC_OP_MODE_ANY:
    case CMMSC_OP_MODE_MAX:
    default:
      CM_ERR_1("MSC: invalid op_mode %d",op_mode);
      return FALSE;
  }
}

/*===========================================================================

FUNCTION cmmsc_is_msc_multi_stacks

DESCRIPTION
  This function returns if the curr op_mode is multi-stack operation.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
boolean cmmsc_is_msc_multi_stacks(
  sys_modem_as_id_e_type as_id
)
{
  cmmsc_state_machine_s_type *state_machine = cmmsimc_state_machine_ptr(as_id);

  cmmsc_state_machine_op_mode_e_type op_mode = state_machine->op_mode;

  return (cmmsc_is_op_mode_multi_stacks(op_mode));
}

/*===========================================================================

FUNCTION cmmsc_is_valid_ss_per_op_mode

DESCRIPTION
  This function returns if the stack is valid base on the current op mode

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.
===========================================================================*/
boolean cmmsc_is_valid_ss_per_op_mode(
  cmmsc_state_machine_s_type  *state_machine,
  multimode_stack_e_type       stk_id,
  cm_hybr_pref_e_type          hybr_pref
)
{
  cmmsc_state_machine_op_mode_e_type op_mode = state_machine->op_mode;

  if(stk_id == MM_STACK_2 &&
      op_mode != CMMSC_OP_MODE_AUTO_MAIN_HYBR1_HYBR2 &&
      op_mode != CMMSC_OP_MODE_AUTO_MAIN_HYBR2 &&
      hybr_pref != CM_HYBR_PREF_CDMA__LTE__HDR
    )
  {
    return FALSE;
  }
  return TRUE;
}

/*===========================================================================

FUNCTION cmmsc_is_volte_active_on_hybr2

DESCRIPTION
  This function returns if volte call is active on hybr2.

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.
===========================================================================*/
static boolean cmmsc_is_volte_active_on_hybr2(
  cmmsc_state_machine_s_type  *state_machine
)
{
  mm_sub_stk_id_s_type mm_id = cmph_map_sub_stk_to_id(
                                 state_machine->asubs_id, MM_STACK_2);
  cm_orig_q_s_type *ph_orig_top_ptr = cmtask_orig_para_get_top(mm_id);

  if(ph_orig_top_ptr == NULL)
  {
    CM_MSG_HIGH_0("Top_ptr is NULL for HYBR2 stack");
    return FALSE;
  }

  /* Check if svlte hybr2 is in a volte call
  */
  if(cmph_is_subs_feature_mode_1x_sxlte(state_machine->asubs_id) &&
      ph_orig_top_ptr->orig->orig_mode == SD_SS_ORIG_MODE_VOLTE)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}


/*===========================================================================

FUNCTION cmmsc_force_all_stack_required

DESCRIPTION
  This function returns if msc needs to force the preference to all stacks.
  Pre-condition: CMPH indicates stack = MM_STACK_ALL.
  Conditions:
  1. If previous op_mode is multi_stack opeation, and new op_mode is single stack operation.
  2. If previous op_mode is single_stack opeation, and new op_mode is multi stack operation.
  3. If the new op_mode is multi-stack operation.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static boolean cmmsc_force_all_stack_required(
  cmmsc_state_machine_op_mode_e_type  prev_op_mode,
  cmmsc_state_machine_op_mode_e_type  new_op_mode
)
{
  CM_ASSERT_ENUM_IS_INRANGE(prev_op_mode,CMMSC_OP_MODE_MAX);
  CM_ASSERT_ENUM_IS_INRANGE(new_op_mode,CMMSC_OP_MODE_MAX);

  if((cmmsc_is_op_mode_multi_stacks(prev_op_mode) &&
      !cmmsc_is_op_mode_multi_stacks(new_op_mode))
      ||
      (!cmmsc_is_op_mode_multi_stacks(prev_op_mode) &&
       cmmsc_is_op_mode_multi_stacks(new_op_mode))
      ||
      (cmmsc_is_op_mode_multi_stacks(new_op_mode))
    )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

static cmmsc_event_e_type cmmsc_get_route_tbl_event(
  cmmsc_routing_table_s_type  *route_table,
  int                          route_idx
)
{
  cmmsc_routing_table_auto_entry_s_type *auto_entry;

  if(route_table == NULL)
  {
    return CMMSC_EVENT_NONE;
  }

  if(route_idx >= route_table->n_row)
  {
    sys_err_fatal_invalid_value_exception(route_idx);
  }

  auto_entry = (cmmsc_routing_table_auto_entry_s_type *)route_table->route_tbl;
  return auto_entry[route_idx].event;
}

static cmmsc_state_machine_op_mode_e_type cmmsc_get_route_tbl_mode(
  cmmsc_routing_table_s_type  *route_table,
  int                          route_idx
)
{
  cmmsc_routing_table_auto_entry_s_type *auto_entry;

  if(route_table == NULL)
  {
    return CMMSC_OP_MODE_NONE;
  }

  if(route_idx >= route_table->n_row)
  {
    sys_err_fatal_invalid_value_exception(route_idx);
  }

  auto_entry = (cmmsc_routing_table_auto_entry_s_type *)route_table->route_tbl;
  return auto_entry[route_idx].mode;
}

static cmmsc_stack_state_e_type cmmsc_get_route_tbl_stack_state(
  cmmsc_routing_table_s_type  *route_table,
  int                          stack_idx,
  int                          route_idx
)
{
  cmmsc_routing_table_auto_entry_s_type *auto_entry;

  if(route_table == NULL)
  {
    return CMMSC_STACK_STATE_NO_CHG;
  }

  if(route_idx >= route_table->n_row ||
      stack_idx >= route_table->n_stack)
  {
    sys_err_fatal_invalid_value_exception(route_idx);
  }

  auto_entry = (cmmsc_routing_table_auto_entry_s_type *)route_table->route_tbl;
  return auto_entry[route_idx].state[stack_idx];
}

static cmmsc_stack_state_e_type cmmsc_get_route_tbl_stack_new_state(
  cmmsc_routing_table_s_type  *route_table,
  int                          stack_idx,
  int                          route_idx
)
{
  cmmsc_routing_table_auto_entry_s_type *auto_entry;

  if(route_table == NULL)
  {
    return CMMSC_STACK_STATE_NO_CHG;
  }

  if(route_idx >= route_table->n_row ||
      stack_idx >= route_table->n_stack)
  {
    sys_err_fatal_invalid_value_exception(route_idx);
  }

  auto_entry = (cmmsc_routing_table_auto_entry_s_type *)route_table->route_tbl;
  return auto_entry[route_idx].new_state[stack_idx];
}

static cmmsc_action_s_type *cmmsc_get_route_tbl_action(
  cmmsc_routing_table_s_type  *route_table,
  int                          stack_idx,
  int                          route_idx
)
{
  cmmsc_routing_table_auto_entry_s_type *auto_entry;

  if(route_table == NULL)
  {
    return NULL;
  }

  if(route_idx >= route_table->n_row ||
      stack_idx >= route_table->n_stack)
  {
    sys_err_fatal_invalid_value_exception(route_idx);
  }

  auto_entry = (cmmsc_routing_table_auto_entry_s_type *)route_table->route_tbl;
  return &(auto_entry[route_idx].action[stack_idx]);
}

/*===========================================================================

FUNCTION cmmsc_get_stk_mode_cap

DESCRIPTION
  This function returns the mode pref of the top priority queue base on the capability.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
sd_ss_mode_pref_e_type cmmsc_get_stk_mode_cap(
  mm_sub_stk_id_s_type  mm_id
  /* multimode id containing sub id and stack id */
)
{
  cmmsc_stack_s_type          *stack_pointer = NULL;
  uint8                        stk_id = mm_id.stk_id;
  cmmsc_state_machine_s_type  *state_machine
    = cmmsimc_state_machine_ptr(mm_id.asubs_id);

  if(state_machine == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return SD_SS_MODE_PREF_NONE;
  }

  if(stk_id < ARR_SIZE(state_machine->stack))
  {
    stack_pointer = &state_machine->stack[stk_id];
    return stack_pointer->capablity.mode;
  }
  else
  {
    CM_ERR_2("invalid sub %d stk %d", mm_id.asubs_id, stk_id);
    return SD_SS_MODE_PREF_NONE;
  }
}

/*===========================================================================

FUNCTION cmmsc_is_in_srv_state

DESCRIPTION
  The function returns if the stack is in service.

DEPENDENCIES
  None

RETURN VALUE
  boolean.

SIDE EFFECTS
  None

===========================================================================*/
boolean cmmsc_is_in_srv_state(
  cmmsc_state_machine_s_type  *state_machine,
  int                          stack_idx
)
{
  if(state_machine == NULL)
  {
    CM_ERR_0("MSC: state_machine NULL ptr");
    return FALSE;
  }

  return (state_machine->stack[stack_idx].state == CMMSC_STACK_STATE_SRV);
}

/*===========================================================================

===========================================================================*/
boolean cmmsc_is_in_ext_no_srv(
  cmmsc_state_machine_s_type  *state_machine,
  int                          stack_idx
)
{
  if(state_machine == NULL)
  {
    CM_ERR_0("MSC: state_machine NULL ptr");
    return FALSE;
  }

  return (state_machine->stack[stack_idx].state == CMMSC_STACK_STATE_HDR_EXT_NO_SRV);
}

/*===========================================================================

FUNCTION cmmsc_is_in_activated_state

DESCRIPTION
  Checks if a stack either in ACTIVE or SERVICE state. Basically if stack
  is looking for srv or found it.

DEPENDENCIES
  None

RETURN VALUE
  boolean.

SIDE EFFECTS
  None

===========================================================================*/
boolean cmmsc_is_in_activated_state (
  cmmsc_state_machine_s_type  *state_machine,
  int                          stack_idx
)
{
  if(state_machine == NULL)
  {
    CM_ERR_0("MSC: state_machine NULL ptr");
    return FALSE;
  }

  return (state_machine->stack[stack_idx].state == CMMSC_STACK_STATE_SRV ||
          state_machine->stack[stack_idx].state == CMMSC_STACK_STATE_ACTIVE ||
          state_machine->stack[stack_idx].state == CMMSC_STACK_STATE_HDR_EXT_NO_SRV);
}

/*===========================================================================

FUNCTION cmmsc_action_sxlte_kick_not_auto

DESCRIPTION

Kick HYBR2 stack on standby preference change if the user standby
preference is not SYS_MODEM_DS_PREF_AUTO.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void cmmsc_action_sxlte_kick_not_auto(
  cmmsc_state_machine_s_type  *msc,
  int                          stack_idx
)
{
  switch(stack_idx)
  {
    case 2:
      if(cmph_ptr()->device_prop.msim_prop.user_standby_pref != SYS_MODEM_DS_PREF_AUTO)
      {
        if(!cmmsc_is_hybr2_kick_allowed(msc))
        {
          cmmsc_auto_update_is_kick_hybr2_pending(msc, TRUE);
        }
        else
        {
          cmmsc_action_pending_kick(msc, stack_idx, TRUE);
        }
      }
      break;

    default:
      break;
  }
}

void cmmsc_update_stack_preference(
  cmmsc_state_machine_s_type       *state_machine,
  /* Subscription for which preference need to be updated */

  cm_mode_pref_e_type              mode_pref,
  /* Mode preference which to update */

  cm_band_pref_e_type              band_pref,
  /* Band preference which to update */

  sys_lte_band_mask_e_type         lte_band_pref,
  /* LTE band preference which to update */

  cm_band_pref_e_type              tds_band_pref
  /* LTE band preference which to update */
)
{

  if(state_machine->stack_common_info.preference.mode_pref != mode_pref
      || state_machine->stack_common_info.preference.band_pref != band_pref
      || state_machine->stack_common_info.preference.tds_band_pref != tds_band_pref
      || !SYS_LTE_BAND_MASK_CHK_IF_EQUAL(&state_machine->stack_common_info.preference.lte_band_pref, &lte_band_pref))
  {
    CM_MSG_HIGH_6_EXT("update_stack_preference(): mode_pref old %d, new %d | band_pref old 0x%x, new 0x%x | tds_band old 0x%x, new 0x%x",
                      state_machine->stack_common_info.preference.mode_pref,
                      mode_pref,
                      state_machine->stack_common_info.preference.band_pref,
                      band_pref,
                      state_machine->stack_common_info.preference.tds_band_pref,
                      tds_band_pref,
                      (state_machine->asubs_id+1));
    cm_print_lte_band_mask(lte_band_pref);
  }

  state_machine->stack_common_info.preference.mode_pref = mode_pref;
  state_machine->stack_common_info.preference.band_pref = band_pref;
  state_machine->stack_common_info.preference.lte_band_pref = lte_band_pref;
  state_machine->stack_common_info.preference.tds_band_pref = tds_band_pref;
}

void cmmsc_set_stack_common_call_mode_per_subs(
  sys_call_mode_t         call_mode,
  sys_modem_as_id_e_type  subs_id
)
{
  cmmsc_state_machine_s_type *state_machine = NULL;
  if(subs_id > SYS_MODEM_AS_ID_NONE && subs_id < SYS_MODEM_AS_ID_NO_CHANGE)
  {
    state_machine = cmmsimc_state_machine_ptr(subs_id);
    state_machine->stack_common_info.call_mode = call_mode;
    CM_MSG_LOW_2("setting call mode %d, for subs %d", call_mode, subs_id);
  }
  else
  {
    CM_ERR_FATAL_1("MSIMC: invalid subs id to set call mode as_id=%d", subs_id);
  }
}

sys_call_mode_t cmmsc_get_stack_common_call_mode_per_subs(
  sys_modem_as_id_e_type  subs_id
)
{
  cmmsc_state_machine_s_type  *state_machine = NULL;
  sys_call_mode_t              call_mode = SYS_CALL_MODE_NORMAL;

  if(subs_id > SYS_MODEM_AS_ID_NONE && subs_id < SYS_MODEM_AS_ID_NO_CHANGE)
  {
    state_machine = cmmsimc_state_machine_ptr(subs_id);
    call_mode = state_machine->stack_common_info.call_mode;
    CM_MSG_LOW_2("returning call mode %d, for subs %d", call_mode, subs_id);
  }
  else
  {
    CM_ERR_FATAL_1("MSIMC: invalid subs id to fetch call mode as_id=%d", subs_id);
  }
  return call_mode;
}

/*===========================================================================

===========================================================================*/

boolean cmmsc_is_hybr2_kick_allowed(
  cmmsc_state_machine_s_type  *msc
)
{
  boolean allowed = TRUE;

  /* if in emerg mode, do not kick LTE stack with orig mode NONE or pref reas != EMERG;
  ** or if cmregprx is active for main, do not kick LTE.
  */
#ifdef CM_GW_SUPPORTED
  mm_sub_stk_id_s_type mm_id;
  cmregprx_state_e_type state;
  cmph_sub_info_s_type *ph_sub_ptr;
  boolean is_volte_stk2 = FALSE;

  if(msc == NULL) return FALSE;

  mm_id = cmph_map_sub_stk_to_id(msc->asubs_id, MM_STACK_0);
  ph_sub_ptr = cmph_get_sub_ptr(msc->asubs_id);
  state = cmregprx_get_state(mm_id);
  if (!cmutil_is_as_id_valid(msc->asubs_id) || ph_sub_ptr == NULL)
  {
    return FALSE;
  }
  is_volte_stk2 = cmcall_is_there_volte_call_stack2(msc->asubs_id);

  allowed = (state != CMREGPRX_STATE_ACTIVE  ||
             ph_sub_ptr->emerg_call_prop.ph_state == CM_PH_STATE_EMERG_CB ||
             cmcall_emergency_call_active() ||
             is_volte_stk2
            );

  CM_MSG_HIGH_5("asubs_id=%d, is_hybr2_kick_allowed = %d, main cmregprx state=%d, ph state=%d, is_volte_stack2 %d",
                msc->asubs_id, allowed,
                state, ph_sub_ptr->emerg_call_prop.ph_state, is_volte_stk2);
#endif


  return allowed;
}

/*===========================================================================

FUNCTION cmmsc_get_1x_sub

DESCRIPTION
  This function will provide the sub on which 1x is currently supported.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
sys_modem_as_id_e_type cmmsc_get_1x_sub()
{
  sys_modem_as_id_e_type asubs_id;

  for( asubs_id = SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(asubs_id); asubs_id++)
  {
    if(cmmsc_get_curr_msc_max_cap(asubs_id) & SYS_SYS_MODE_MASK_CDMA)
    {
      return asubs_id;
    }
  }
  return SYS_MODEM_AS_ID_1;
}
/*===========================================================================

FUNCTION cmmsc_get_1x_capable_sub

DESCRIPTION
  This function will provide the sub on which 1x is currently supported, if no sub is 1x capable it will return none.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/


sys_modem_as_id_e_type cmmsc_get_1x_capable_sub()
{
  sys_modem_as_id_e_type asubs_id;

  for( asubs_id = SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(asubs_id); asubs_id++)
  {
    if(SD_MODE_CONTAIN(cmmsc_get_curr_msc_max_cap(asubs_id),
                         SD_SS_MODE_PREF_CDMA)
    )
    {
      return asubs_id;
    }
  }
  return SYS_MODEM_AS_ID_NONE;
}

/*===========================================================================

FUNCTION  cmmsc_is_lte_only_sub

DESCRIPTION
 indicates whether the SUB is LTE only SUB or not.

DEPENDENCIES
 none

RETURN VALUE
  ss

SIDE EFFECTS
  none

===========================================================================*/
boolean cmmsc_is_lte_only_sub(
  sys_modem_as_id_e_type  asubs_id
)
{

  /* Pointer to phone object.*/
  sd_ss_mode_pref_e_type  comm_mode;
  sd_ss_band_pref_e_type  comm_band;
  sd_ss_band_pref_e_type  comm_tds_band;
  sys_lte_band_mask_e_type  comm_lte_band;

  /* get mode and band capabilities of PM policy via state machine.  */
  cmph_get_comm_mode_band_capab_with_pm(&comm_mode, &comm_band, &comm_lte_band, &comm_tds_band, asubs_id);

  if(cmcall_is_lte_only_mode(cmph_map_sd_mode_pref_to_cm_mode_pref(comm_mode),\
                             cmutil_map_sd_band_pref_to_cm_band_pref(comm_band),\
                             comm_lte_band,\
                             cmutil_map_sd_tds_band_pref_to_cm_tds_band_pref(comm_tds_band)))
  {
    CM_MSG_HIGH_0("LTE ONLY SUB");
    return TRUE;
  }

  return FALSE;
}

/*===========================================================================

FUNCTION  cmmsc_is_XXX_RAT_enabled_in_CM

DESCRIPTION
 indicates whether the SUB has given RAT enabled in CM

DEPENDENCIES
 none

RETURN VALUE
  ss

SIDE EFFECTS
  none

===========================================================================*/
boolean cmmsc_is_XXX_RAT_enabled_in_CM(
  sys_modem_as_id_e_type  asubs_id,
  sd_ss_mode_pref_e_type  mode_pref
)
{

  /* Pointer to phone object.*/
  sd_ss_mode_pref_e_type  comm_mode;
  sd_ss_band_pref_e_type  comm_band;
  sd_ss_band_pref_e_type  comm_tds_band;
  sys_lte_band_mask_e_type  comm_lte_band;

  /* get mode and band capabilities of PM policy via state machine.  */
  cmph_get_comm_mode_band_capab_with_pm(&comm_mode, &comm_band, &comm_lte_band, &comm_tds_band, asubs_id);


  if((sd_ss_get_supp_mode_pref(
    comm_mode, comm_band, comm_lte_band, comm_tds_band) & SD_SS_MODE_PREF_DIGITAL) & mode_pref)
  {
    CM_MSG_HIGH_2("cmmsc_is_XXX_RAT_enabled_in_CM :  as_id %d, mode_pref %d :: enabled", asubs_id, mode_pref);
    return TRUE;
  }

  return FALSE;
}

/*===========================================================================

FUNCTION  cmmsc_is_XXX_RAT_enabled_in_SD

DESCRIPTION
 indicates whether the SUB has given RAT enabled in SD

DEPENDENCIES
 none

RETURN VALUE
  ss

SIDE EFFECTS
  none

===========================================================================*/
boolean cmmsc_is_XXX_RAT_enabled_in_SD(
  sys_modem_as_id_e_type  asubs_id,
  sd_ss_mode_pref_e_type  mode_pref
)
{

  /* Pointer to phone object.*/
  sd_ss_mode_pref_e_type  sd_mode_pref        = SD_SS_MODE_PREF_NONE;
  sd_ss_band_pref_e_type  sd_band_pref        = SD_SS_BAND_PREF_NONE;
  sys_lte_band_mask_e_type  sd_lte_band_pref  = SYS_LTE_BAND_MASK_CONST_NONE;
  sd_ss_band_pref_e_type  sd_tds_band_pref    = SD_SS_BAND_PREF_NONE;
  boolean                 rat_supported       = FALSE;

  mm_sub_stk_id_s_type  mm_id;

  mm_id.asubs_id = asubs_id;
  mm_id.stk_id = MM_STACK_0;
  
  sd_misc_get_mode_band_per_sub(mm_id, &sd_mode_pref, &sd_band_pref, &sd_lte_band_pref, &sd_tds_band_pref);

  if ((sd_ss_get_supp_mode_pref(
    sd_mode_pref, sd_band_pref, sd_lte_band_pref, sd_tds_band_pref) & SD_SS_MODE_PREF_DIGITAL) & mode_pref)        
  {
    rat_supported = TRUE;
    CM_MSG_HIGH_2("cmmsc_is_XXX_RAT_enabled_in_SD :  as_id %d, mode_pref %d :: enabled", asubs_id, mode_pref);
  }
  
  return rat_supported;
}
/*===========================================================================

FUNCTION cmmsc_map_sys_domain_to_sd

DESCRIPTION
  This function translates a service domain to a service domain preference
  type used by the SD20.

DEPENDENCIES
  none

RETURN VALUE
  SD service domain preference.

SIDE EFFECTS
  none

===========================================================================*/
sd_ss_srv_domain_pref_e_type cmmsc_map_sys_domain_to_sd
(
  sys_srv_domain_e_type  srv_domain
)
{
  sd_ss_srv_domain_pref_e_type mapped_service_domain;

  switch ( srv_domain )
  {
    case SYS_SRV_DOMAIN_CS_ONLY:
      mapped_service_domain = SD_SS_SRV_DOMAIN_PREF_CS_ONLY;
      break;

    case SYS_SRV_DOMAIN_PS_ONLY:
      mapped_service_domain = SD_SS_SRV_DOMAIN_PREF_PS_ONLY;
      break;

    case SYS_SRV_DOMAIN_CS_PS:
      mapped_service_domain = SD_SS_SRV_DOMAIN_PREF_CS_PS;
      break;

    case SYS_SRV_DOMAIN_NONE:
    case SYS_SRV_DOMAIN_NO_SRV:
      mapped_service_domain = SD_SS_SRV_DOMAIN_PREF_NONE;
      break;

    case SYS_SRV_DOMAIN_VOLTE:
      mapped_service_domain = SD_SS_SRV_DOMAIN_PREF_VOLTE;
      break;

    case SYS_SRV_DOMAIN_CS_VOLTE:
      mapped_service_domain = SD_SS_SRV_DOMAIN_PREF_CS_VOLTE;
      break;

    case SYS_SRV_DOMAIN_CAMPED:
    case SYS_SRV_DOMAIN_MAX:
    default:
      CM_ERR_1("SC_SGLTE: No mapping for srv_domain=%d", srv_domain);
      mapped_service_domain = SD_SS_SRV_DOMAIN_PREF_NONE;
      break;
  } /* switch ( service_domain_pref ) */

  return mapped_service_domain;
}

/*===========================================================================

FUNCTION cmmsc_map_cm_srv_domain_to_sys

DESCRIPTION
  This function translates cm_srv_domain_pref_e_type to sys_srv_domain_e_type.

DEPENDENCIES
  none

RETURN VALUE
  sys_srv_domain_e_type.

SIDE EFFECTS
  none

===========================================================================*/
sys_srv_domain_e_type  cmmsc_map_cm_srv_domain_to_sys(
  cm_srv_domain_pref_e_type  srv_domain_pref
)
{
  sys_srv_domain_e_type mapped_domain;


  switch ( srv_domain_pref )
  {

    case CM_SRV_DOMAIN_PREF_NONE:
      mapped_domain = SYS_SRV_DOMAIN_NONE;
      break;

    case CM_SRV_DOMAIN_PREF_CS_ONLY:
      mapped_domain = SYS_SRV_DOMAIN_CS_ONLY;
      break;

    case CM_SRV_DOMAIN_PREF_PS_ONLY:
      mapped_domain = SYS_SRV_DOMAIN_PS_ONLY;
      break;

    case CM_SRV_DOMAIN_PREF_CS_PS:
      mapped_domain = SYS_SRV_DOMAIN_CS_PS;
      break;

    case CM_SRV_DOMAIN_PREF_CS_VOLTE:
      mapped_domain = SYS_SRV_DOMAIN_CS_VOLTE;
      break;

    case CM_SRV_DOMAIN_PREF_VOLTE:
      mapped_domain = SYS_SRV_DOMAIN_VOLTE;
      break;

    default:
      CM_MSG_HIGH_1( "Invalid cm service domain %d", srv_domain_pref);
      mapped_domain = SYS_SRV_DOMAIN_NONE;
      break;
  } /* switch ( service_domain_pref ) */

  return mapped_domain;
}

/*===========================================================================

FUNCTION cmmsc_set_sys_sel_pref_event

DESCRIPTION
  Set the proper sys_sel_pref_event based on the num of stacks allocated.

DEPENDENCIES
  None.

RETURN VALUE
  cmmsc_event_e_type.

SIDE EFFECTS
  None.

===========================================================================*/
cmmsc_event_e_type  cmmsc_set_sys_sel_pref_event (
  sys_modem_as_id_e_type  asubs_id,
  multimode_stack_e_type  stk_id,
  cmmsc_state_machine_op_mode_e_type  prev_op_mode,
  cmmsc_state_machine_op_mode_e_type  new_op_mode
)
{
  cmmsc_event_e_type evt = CMMSC_EVENT_NONE;

  if (stk_id == MM_STACK_ALL)
  {
    if (!cmph_is_subs_feature_mode_1x_sxlte(asubs_id) &&
        !cmmsc_force_all_stack_required(prev_op_mode, new_op_mode))
    {
      evt = CMMSC_EVENT_SYS_SEL_PREF_MAIN;
    }
    else
    {
      uint8 stk_cnt = cmtask_get_targeted_num_stacks(asubs_id);
      switch (stk_cnt)
      {
        case 1:
          evt = CMMSC_EVENT_SYS_SEL_PREF_MAIN;
          break;

        case 2:
          evt = CMMSC_EVENT_SYS_SEL_PREF_MAIN_HYBR1;
          break;

        case 3:
          evt = CMMSC_EVENT_SYS_SEL_PREF_MAX;
          break;

        default:
          CM_ERR_2("sub %d invalid stk_cnt %d", asubs_id, stk_cnt);
          break;
      }
    }
  }
  else if(stk_id == MM_STACK_1) /* NORMAL / 1xSxLTE */
  {
    evt = CMMSC_EVENT_SYS_SEL_PREF_HYBR1;
  }
  else if(stk_id == MM_STACK_2) /* NORMAL /1xSxLTE */
  {
    evt = CMMSC_EVENT_SYS_SEL_PREF_HYBR2;
  }
  else if(stk_id == MM_STACK_0)
  {
    evt = CMMSC_EVENT_SYS_SEL_PREF_MAIN;
  }

  return evt;
}

/*===========================================================================

FUNCTION cmmsc_get_msc_domain_cap

DESCRIPTION
  This function returns the domain cap stored in cmmsc for the specified sub.

DEPENDENCIES
  cmmsc is initialized.

RETURN VALUE
  sys_srv_domain_e_type.

SIDE EFFECTS
  None.
===========================================================================*/
sys_srv_domain_e_type  cmmsc_get_msc_domain_cap (
  sys_modem_as_id_e_type  as_id
)
{
  cmmsc_state_machine_s_type *state_machine = NULL;

  if (!( as_id >= SYS_MODEM_AS_ID_1 && as_id < MAX_AS_IDS))
  {
    CM_MSG_HIGH_1("invalid as_id %d", as_id);
    as_id = SYS_MODEM_AS_ID_1;
  }

  state_machine = cmmsimc_state_machine_ptr(as_id);

  if (state_machine == NULL)
  {
    CM_ERR_FATAL_1("MSC: state_machine null ptr as_id=%d", as_id);
    return SYS_SRV_DOMAIN_NONE;
  }

  return state_machine->stack_common_info.ue_domain_capability;
}

/*===========================================================================

FUNCTION cmmsc_set_domain_cap_dual_lte

DESCRIPTION
  Set domain capability for dual lte configuration.

DEPENDENCIES
None

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
static void cmmsc_set_domain_cap_dual_lte(
  cmmsc_state_machine_s_type    *state_machine
)
{
  cmph_s_type *ph_ptr = cmph_ptr();
  sys_modem_as_id_e_type asubs_id;
  sys_srv_domain_e_type prev_domain_cap;
  boolean is_mms_ongoing = FALSE;

  if(state_machine == NULL) return;

  asubs_id = state_machine->asubs_id;
  prev_domain_cap = state_machine->stack_common_info.ue_domain_capability;

  if(!cmph_is_dual_lte() || !cmutil_is_as_id_valid(asubs_id)) return;

  is_mms_ongoing = cmph_is_mms_ongoing(asubs_id);

  /* Set domain cap to CS+PS if sub is DDS or device not in dual standby */
  if(asubs_id == ph_ptr->device_prop.msim_prop.new_default_data_subs)
  {
    if(ph_ptr->sub_info[asubs_id]->user_srv_domain_pref_setting ==  CM_SRV_DOMAIN_PREF_CS_ONLY)
    {
      state_machine->stack_common_info.ue_domain_capability = SYS_SRV_DOMAIN_CS_ONLY;
    }
    else
    {
      state_machine->stack_common_info.ue_domain_capability = SYS_SRV_DOMAIN_CS_PS;
    } 
  }
  /* Set domain cap to CS+PS if MMS is ongoing */
  else if(is_mms_ongoing)
  {
    state_machine->stack_common_info.ue_domain_capability = SYS_SRV_DOMAIN_CS_PS;
  }
  /* Set domain cap to CS+VOLTE if sub is not DDS and IMS is enabled on sub and MMS not ongoing */
  else if(asubs_id != ph_ptr->device_prop.msim_prop.new_default_data_subs)
  {
    if(ph_ptr->sub_info[asubs_id]->user_srv_domain_pref_setting ==  CM_SRV_DOMAIN_PREF_CS_ONLY)
    {
      state_machine->stack_common_info.ue_domain_capability = SYS_SRV_DOMAIN_CS_ONLY;
    }
    else
    {
      state_machine->stack_common_info.ue_domain_capability = SYS_SRV_DOMAIN_CS_VOLTE;
    } 
  }
  else
  {
    state_machine->stack_common_info.ue_domain_capability = SYS_SRV_DOMAIN_CS_PS;
  }

  if (prev_domain_cap != state_machine->stack_common_info.ue_domain_capability)
  {
    CM_MSG_HIGH_6("PMPRX->CMMSC[%d]: DUAL_LTE: update msc domain_cap %d->%d, dds %d->%d, is_mms %d",
                  asubs_id, 
                  prev_domain_cap,
                  state_machine->stack_common_info.ue_domain_capability,
                  ph_ptr->device_prop.msim_prop.default_data_subs,
                  ph_ptr->device_prop.msim_prop.new_default_data_subs,
                  is_mms_ongoing
                  );
  }
}


/*===========================================================================

FUNCTION cmmsc_get_prev_reg_cap

DESCRIPTION
  This function returns prev_registration_capability for the specified
  mm_id.

DEPENDENCIES
  None.

RETURN VALUE
  mmode_mode_band_s_type.

===========================================================================*/
cmmsc_stack_capability_s_type    cmmsc_get_prev_reg_cap(
  mm_sub_stk_id_s_type  mm_id
)
{
  cmmsc_stack_s_type          *stack_pointer = NULL;
  uint8                        stk_id = mm_id.stk_id;
  cmmsc_state_machine_s_type  *state_machine
    = cmmsimc_state_machine_ptr(mm_id.asubs_id);
  cmmsc_stack_capability_s_type prev_reg_cap;
  
  memset(&prev_reg_cap, 0, sizeof(cmmsc_stack_capability_s_type));
  
  if(state_machine == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return prev_reg_cap;
  }

  if(stk_id < ARR_SIZE(state_machine->stack))
  {
    stack_pointer = &state_machine->stack[stk_id];
    prev_reg_cap = stack_pointer->prev_registration_capability;
  }
  return prev_reg_cap;
}



/*===========================================================================

FUNCTION cmmsc_get_stk_cap

DESCRIPTION
  This function returns stack capability for the specified
  mm_id.

DEPENDENCIES
  None.

RETURN VALUE
  mmode_mode_band_s_type.

===========================================================================*/
cmmsc_stack_capability_s_type    cmmsc_get_stk_cap(
  mm_sub_stk_id_s_type  mm_id
)
{
  cmmsc_stack_s_type          *stack_pointer = NULL;
  uint8                        stk_id = mm_id.stk_id;
  cmmsc_state_machine_s_type  *state_machine
    = cmmsimc_state_machine_ptr(mm_id.asubs_id);
  cmmsc_stack_capability_s_type stk_cap;
  
  memset(&stk_cap, 0, sizeof(cmmsc_stack_capability_s_type));
  
  if(state_machine == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return stk_cap;
  }

  if(stk_id < ARR_SIZE(state_machine->stack))
  {
    stack_pointer = &state_machine->stack[stk_id];
    stk_cap = stack_pointer->capablity;
  }
  return stk_cap;
}



