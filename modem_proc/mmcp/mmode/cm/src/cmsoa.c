/*==========================================================================

    C A L L   M A N A G E R  C M  O P E R A T I O N  S W I T C H  F I L E

DESCRIPTION
  This header file contains all the definitions necessary for
  clients to interface with the Call Manager module.

Copyright (c) 2013 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmsoa.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/08/14   xs      hVOLTE SSAC support
01/21/14   sk      Depend on cmss internal srv status for prediction allowance
08/06/13   xs      Add function cmlog_csfb_svlte_switch() to log switch between
                   SVLTE and CSFB
02/03/13   cl      FR1975: Dynamic Switch SVLTE <-> CSFB Official Check-in
01/28/13   cl      Redesign the messages handler approach & adding activate-flag
01/22/13   cl      Fixing PLMN switching, prediction algorithm and enabled-flag
01/14/13   cl      Adding last configuration & VOLTE-12 support
11/30/12   cl      Adding CM UTs and small bug fixes
11/24/12   cl      Fixing CM UTs
11/17/12   cl      Adding EFS item for SVLTE-MCC list
11/16/12   cl      Adding Delay timer to prevent switch() during voice call
11/16/12   cl      Adding delay timer to avoid ping-pong scenarios.
11/14/12   cl      Adding SD to report PLMN-ID along with MCC.
11/12/12   cl      Adding EFS item for SVLTE PLMN-ID lists with dynamic list structure in cmsoa module.
11/10/12   cl      Adding EFS item for CMSOA configurations
11/06/12   cl      Adding new callback (generic type) for future uses.
11/04/12   cl      Connecting SD & CM with MCC messages sending from SD -> CM.
10/26/12   cl      Adding cmregprx and cmsoa handshakes before GWL SRV_REQ
10/23/12   cl      Adding switch() functions switching between CSFB and SVLTE
10/19/12   cl      Adding service indication messages handler.
10/17/12   cl      Adding MCC messages handlers.
10/10/12   cl      Initial release
===========================================================================*/

/*---------------------------------------------------------------------------
** Includes
**---------------------------------------------------------------------------
*/
/*lint -esym(766,customer.h) */
#include "mmcp_variation.h"
#include "customer.h"      /* Customer configuration file */
#include "cm_v.h"
#include "cm_i.h"
#include "cmtaski.h"
#include "cmidbg.h"        /* Call Manager Debug internal header file */
#include "cmefs.h"
#include "cmregprxi.h"
#include "cmph.h"
#include "cmsds.h"
#include "cmll.h"
#include "cmlog_v.h"

#if defined(CM_GW_SUPPORTED) || defined(FEATURE_CM_LTE)
#include "mm.h"
#endif

#include "cmsoa.h"  /* Interface to CM SV Operation Abitration file */

#ifdef CM_DEBUG
#error code not present
#endif

#include "cmmsc.h"
#include "cmmsc_auto.h"
#include "cmsimcoord.h"
#include "policyman.h"
#include "mm.h"
#include "cmpmprx.h"
#include "cmaccessctrl.h"
#include "cmclnup.h"

#if (defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)) || defined(FEATURE_MMODE_DYNAMIC_SV_OPERATION_SWITCH)

/*--------------------------------------------------------------------------
** DEFINES, CONSTANTS and FORWARD DECLARATIONS
**--------------------------------------------------------------------------
*/

/* Forward declaration */
static cmsoa_svlte_plmn_id_node_s_type *cmsoa_svlte_plmn_id_node_buf_else_err_fatal(void);
static int cmsoa_compare_plmn_func(
  void *item_ptr, void *compare_val
);
static void cmsoa_sv_switch_csfb_svlte(
  sys_modem_as_id_e_type     asubs_id
);
static void cmsoa_sv_switch_svlte_csfb(
  sys_modem_as_id_e_type     asubs_id
);
static boolean cmsoa_is_prediction_allowed(
  multimode_stack_e_type stack_id,
  sys_modem_as_id_e_type  asubs_id
);
static boolean cmsoa_is_stack_state_valid(
  sys_modem_as_id_e_type     asubs_id
);

boolean cmsoa_is_srlte_in_inconsistent_state(
  sys_modem_as_id_e_type     asubs_id
);
boolean cmsoa_is_c2kswitch_possible(
  sys_modem_as_id_e_type     asubs_id
);

static void cmsoa_read_soa_config_efs(
  boolean                is_pwr_on,
  sys_modem_as_id_e_type asubs_id
);
boolean cmsoa_check_is_1xsrlte_plmn(sys_plmn_id_s_type plmn_id, sys_modem_as_id_e_type subsId);


/*--------------------------------------------------------------------------
** STATIC FUNCTIONS
**--------------------------------------------------------------------------
*/


/*===========================================================================

FUNCTION cmsoa_state_info_ptr

DESCRIPTION
  This function returns the pointer to CMSOA state info.

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
cmsoa_state_info_s_type *cmsoa_state_info_ptr(void)
{
  static cmsoa_state_info_s_type cmsoa_state_info;

  return &cmsoa_state_info;
}

/*===========================================================================

FUNCTION cmsoa_state_sub_info_ptr

DESCRIPTION
  This function returns the pointer to CMSOA state sub info.

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
cmsoa_state_sub_info_s_type *cmsoa_state_sub_info_ptr(
  sys_modem_as_id_e_type asubs_id
)
{
  cmsoa_state_info_s_type *cmsoa_state_info = cmsoa_state_info_ptr();

  if(!(asubs_id >= SYS_MODEM_AS_ID_1 &&
       asubs_id < ARR_SIZE(cmsoa_state_info->sub_info) &&
       asubs_id < SYS_MODEM_AS_ID_NO_CHANGE))
  {
    CM_MSG_HIGH_1("cmsoa get sub prop wrong sub %d", asubs_id);
    asubs_id = SYS_MODEM_AS_ID_1;
  }
  return cmsoa_state_info->sub_info[asubs_id];
}


/*===========================================================================

FUNCTION cmsoa_is_stack_state_valid

DESCRIPTION
  This function process mcc_msg. It performs prediction if the MCC belongs
  to SVLTE or CSFB country based on a configuration MCC lists.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static boolean cmsoa_is_stack_state_valid(
  sys_modem_as_id_e_type     asubs_id
)
{
  /* stack state */
  cmmsc_stack_state_e_type main_state = CMMSC_STACK_STATE_NONE;
  cmmsc_stack_state_e_type hybr2_state = CMMSC_STACK_STATE_NONE;

  main_state = cmmsimc_get_msc_stack_state(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0));
  hybr2_state = cmmsimc_get_msc_stack_state(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_2));

  /* If state machine is not yet initialized - stop */
  if (main_state == CMMSC_STACK_STATE_NONE ||
      main_state == CMMSC_STACK_STATE_NULL ||
      hybr2_state == CMMSC_STACK_STATE_NONE ||
      hybr2_state == CMMSC_STACK_STATE_NULL)
  {
    CM_MSG_HIGH_0("CMSOA: MCC msg received in wrong state");
    return FALSE;
  }
  return TRUE;
}

/*===========================================================================

FUNCTION cmsoa_svlte_plmn_id_node_buf_else_err_fatal

DESCRIPTION
  This function allocate memory for a node in SVLTE-PLMN-ID list

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static cmsoa_svlte_plmn_id_node_s_type *cmsoa_svlte_plmn_id_node_buf_else_err_fatal (
  void
)
{
  cmsoa_svlte_plmn_id_node_s_type *plmn_node_ptr;

  plmn_node_ptr =
    (cmsoa_svlte_plmn_id_node_s_type *) cm_mem_malloc( sizeof(cmsoa_svlte_plmn_id_node_s_type) );

  /* If allocation failed, error fatal.
  */
  if( plmn_node_ptr == NULL )
  {
    sys_err_fatal_null_ptr_exception();
  }

  return plmn_node_ptr;
}

/*===========================================================================

FUNCTION cmsoa_compare_plmn_func

DESCRIPTION
  This function compares two plmns. Function format is created to used in
  queue ultilities function.

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static int cmsoa_compare_plmn_func(
  void *item_ptr, void *compare_val
)
{

  cmsoa_svlte_plmn_id_node_s_type   *q_node_ptr  = NULL;
  sys_plmn_id_s_type                *compare_ptr = NULL;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(item_ptr    != NULL);
  CM_ASSERT(compare_val != NULL);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  q_node_ptr  = (cmsoa_svlte_plmn_id_node_s_type *) item_ptr;
  compare_ptr = (sys_plmn_id_s_type *) compare_val;

  if (item_ptr == NULL ||
      compare_val == NULL )
  {
    CM_MSG_HIGH_0("CMSOA: item_ptr or compare_val = NULL");
    return((int) FALSE);
  }

  CM_MSG_HIGH_6("CMSOA: comparing PLMN HEX([%x][%x][%x]), against SVLTE PLMN list HEX([%x][%x][%x])",
                compare_ptr->identity[0],
                compare_ptr->identity[1],
                compare_ptr->identity[2],
                q_node_ptr->plmn.identity[0],
                q_node_ptr->plmn.identity[1],
                q_node_ptr->plmn.identity[2]);

  //Method to compare two plmns.
  if( sys_plmn_match(q_node_ptr->plmn,*compare_ptr ))
  {
    CM_MSG_HIGH_0("CMSOA: Compared values are matched");
    return((int) TRUE );
  }

  return((int) FALSE );
}/* cmtask_orig_search_func_act_type */


/*===========================================================================

FUNCTION cmsoa_sv_switch_is_delayed

DESCRIPTION
  This function checks if an SV or non-SV operation switch should happens
  or not. Delay could be caused by:
  a) Voice call is on going
  b) We're ping-ponging between SV and non SV operations.

DEPENDENCIES
 - the reason why we should delay the SV-OPERATION switch()
 - output parameters: the timers which indicates how long we should delay.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static cmsoa_sv_switch_delay_reas_e_type cmsoa_sv_switch_is_delayed(
  sys_modem_as_id_e_type     asubs_id
)
{
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  dword uptime = time_get_uptime_secs();
  cm_orig_q_s_type   *ph_orig_para_top_ptr = cmtask_orig_para_get_top(cmph_map_sub_stk_to_id( asubs_id, MM_STACK_2));
#if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
  mm_sub_stk_id_s_type mm_id = cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0);
  sd_si_info_s_type *ss_true_stk_info_ptr = cmss_true_stack_info_ptr(mm_id);
#endif
  if(cmsoa_state == NULL || !IS_VALID_PH_SUB_PTR(asubs_id))
  {
    return CMSOA_SV_SWITCH_DELAY_REAS_NONE;
  }
  /* If we're in the ping-pong hystersis timer period - wait*/
  /* no ping pong from 1xsrlte to csfb */
  if (cmsoa_state->info.switch_delay_timer != 0 &&
      uptime < cmsoa_state->info.switch_delay_timer &&
      cmsoa_state->info.switch_delay_reason == CMSOA_SV_SWITCH_DELAY_REAS_PINGPONG &&
      cmsoa_state->info.curr_sv_oprt_mode == CMSOA_SV_OPRT_MODE_SVLTE)
  {
    return CMSOA_SV_SWITCH_DELAY_REAS_PINGPONG;
  }

  if ( ph_orig_para_top_ptr!= NULL )
  {
    if(ph_orig_para_top_ptr->orig->orig_mode == SD_SS_ORIG_MODE_VOLTE)
    {
      return CMSOA_SV_SWITCH_DELAY_VOLTE_ON_HYBR;
    }
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If mt voice call is pending, do not trigger switch */
  if (cmph_ptr()->sub_info[asubs_id]->sub_3gpp2_pref.is_1x_mt_call_pending)
  {
    return CMSOA_SV_SWITCH_DELAY_REAS_VOICE_CALL;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
  if (cmsoa_state->config.sxlte_timers.lte_idle_timer > 0)
  {
    CM_MSG_HIGH_1("curr switch delay reas: %d", cmsoa_state->info.switch_delay_reason);

    /* If connection status is not IDLE and switch delay reason is not already LTE_CONNECTED
    ** (implying that Twaitrrcidle timer was already started once), then return with delay reason
    ** LTE_CONNECTED. This will ensure that the Twaitrrcidle timer does not get extended when RRC
    ** remains in connected state for longer than the timer duration.
    */
    if(cmph_is_oprting_in_csfb_pref_srlte(asubs_id)
        && mm_per_stacks_get_connection_status(asubs_id,SYS_MODEM_STACK_ID_1) != SYS_GW_CONN_STATUS_IDLE
        && cmsoa_state->info.switch_delay_reason != CMSOA_SV_SWITCH_DELAY_LTE_CONNECTED)
    {
      return CMSOA_SV_SWITCH_DELAY_LTE_CONNECTED;
    }
  }
#endif

#if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
  if (cmsoa_state->config.attach_complete_timer > 0 &&
      cmsoa_state->info.switch_delay_reason != CMSOA_SV_SWITCH_DELAY_ATTACH_COMPLETE &&
      ss_true_stk_info_ptr != NULL)
  {
    CM_MSG_HIGH_3("CMSOA ATTACH COMPLETE TIMER: sub mode %d mode %d extend_srv_info %d",\
                  cmph_is_oprting_in_csfb_pref_csfb_sub_mode(asubs_id),
                  ss_true_stk_info_ptr->mode,
                  ss_true_stk_info_ptr->extend_srv_info);

    if(cmph_is_oprting_in_csfb_pref_csfb_sub_mode(asubs_id) &&
        (ss_true_stk_info_ptr->mode == SD_MODE_LTE) &&
        (ss_true_stk_info_ptr->extend_srv_info == SYS_EXTEND_SRV_INFO_REG_COMP_ACK_PENDING) )
    {
      return CMSOA_SV_SWITCH_DELAY_ATTACH_COMPLETE;
    }
  }
#endif

  return CMSOA_SV_SWITCH_DELAY_REAS_NONE;

}

/*===========================================================================

  This function evaulates if capabilities to enable are supported.
  This function is less restrictive than cmmsc_auto_is_hybr_gwl_allowed, which
  check if CSFB is enabled or not in order to transition to SRLTE.
  cmmsc_auto_is_hybr_gwl_allowed considers CMSOA state, where this function
  needn't check that because result of this function decides CMSOA state.
===========================================================================*/

EXTERN boolean cmsoa_is_srlte_capability(sys_modem_as_id_e_type as_id)
{
  boolean srlte_cap = FALSE;
  sd_ss_mode_pref_e_type   comm_mode_cap     = SD_SS_MODE_PREF_NONE;
  sd_ss_band_pref_e_type   comm_band_cap     = SD_SS_BAND_PREF_NONE;
  sys_lte_band_mask_e_type comm_lte_band_cap = SYS_LTE_BAND_MASK_CONST_NONE;
  sd_ss_band_pref_e_type   comm_tds_band_cap = SD_SS_BAND_PREF_NONE;

  /* switching to srlte should not be done for non dds sub as non DDS LTE is not for data */
  if((cmph_is_msim() && as_id != cmph_ptr()->device_prop.msim_prop.default_data_subs) 
     ||
     cmmsc_get_1x_sub() != as_id) 
  {
    return FALSE;
  }

  if (IS_VALID_PH_SUB_PTR(as_id))
  {

    /* get mode and band capabilities of PM policy via state machine.  */
    cmph_get_comm_mode_band_capab_with_pm(&comm_mode_cap, &comm_band_cap, &comm_lte_band_cap, &comm_tds_band_cap, as_id);

    /* Intersect with target capabilities */
    comm_mode_cap  = sd_ss_get_supp_mode_pref(comm_mode_cap, comm_band_cap, comm_lte_band_cap, comm_tds_band_cap);

    srlte_cap =
      (cmph_is_valid_mode_pref(CM_MODE_PREF_CDMA_ONLY, as_id) &&
       cmph_is_valid_mode_pref(CM_MODE_PREF_LTE_ONLY, as_id) &&
       SD_MODE_CONTAIN(comm_mode_cap, SD_SS_MODE_PREF_CDMA) &&
       SD_MODE_CONTAIN(comm_mode_cap, SD_SS_MODE_PREF_LTE) &&
       cmph_ptr()->sub_info[as_id]->is_3gpp2_subs_avail);
  }

  srlte_cap &= cmmmgsdi_is_switch_allowed_per_csim_self_activation(as_id);

  CM_MSG_HIGH_6_ASID_EXT("1XSUB: is_srlte_cap_returns %d, rat_cap 0x%x, mode_pref %d, dds %d, 1x sub avail %d",
                         as_id, srlte_cap, comm_mode_cap, 
                         CMPH_SS_MODE_PREF(as_id, cmph_ptr()),
                         cmph_ptr()->device_prop.msim_prop.default_data_subs,
                         cmph_ptr()->sub_info[as_id]->is_3gpp2_subs_avail,
                         (as_id+1));

  return srlte_cap;
}


/*===========================================================================

FUNCTION cmsoa_clear_active_attach_complete_timer

DESCRIPTION
This function clears the attach complete timer,.if delay reason is
CMSOA_SV_SWITCH_DELAY_ATTACH_COMPLETE andattach complete timer
is already running.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmsoa_clear_active_attach_complete_timer(
  sys_modem_as_id_e_type  asubs_id
)
{
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);

  if(cmsoa_state == NULL)
  {
    return;
  }
  if (cmph_is_oprting_in_csfb_pref_csfb_sub_mode(asubs_id) &&
      (cmsoa_state->info.switch_delay_state == CMSOA_SV_OPRT_MODE_SVLTE) &&
      (cmsoa_state->info.switch_delay_reason == CMSOA_SV_SWITCH_DELAY_ATTACH_COMPLETE) &&
      (cmsoa_state->info.is_attach_complete_timer_active == TRUE))
  {
    cmtask_attach_complete_timer_clear(asubs_id);
  }
}


/*===========================================================================

FUNCTION cmsoa_check_and_switch_to_srlte

DESCRIPTION
  This function performs all neccessary checks before it decides to whether
  switching to SRLTE operation.
  If a switch() operation is required, it will call to another function that
  will only performs the switch.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsoa_check_and_switch_to_srlte(
  sys_modem_as_id_e_type as_id,
  boolean is_switch_needed
)
{
  cmmsc_state_machine_s_type *state_machine = cmmsimc_state_machine_ptr(as_id);
  boolean switching_to_srlte = FALSE;
  cmss_s_type    *ss_ptr   = cmss_ptr();
  cmph_s_type  *ph_ptr = cmph_ptr();
  cmsds_sub_prop_s_type *sds_sub_prop_ptr = cmsds_get_sub_prop_ptr(as_id);

  if(sds_sub_prop_ptr == NULL)
  {
    return FALSE;
  }
  if (cmph_is_subs_feature_mode_1x_sxlte(as_id))
  {
    boolean is_srlte_cap = cmsoa_is_srlte_capability(as_id);
    CM_MSG_HIGH_5_ASID_EXT("check_and_switch_to_srlte: main mode_pref=%d,hybr2_operational=%d, srlte_cap=%d, ue_mode=%d",
                           as_id,
                           CMPH_SS_MODE_PREF(as_id,ph_ptr),
                           cmss_is_stack2_operational(as_id),
                           is_srlte_cap,
                           state_machine->stack_common_info.ue_mode,
                           (as_id+1));
                  
    /* For CT+CT case, if dds is moving from sub1 to sub2 along with 1x sub, before getting 
    ** CM_SUBS_CHGD_DONE from mmoc indicating sub switch is done, CM should not switch to srlte
    ** in new dds/1x sub.
    */
    if (cmtask_is_pending_1xsub_switch(as_id))
    {
      return FALSE;
    }
             
    /* Switch SRLTE mode if following conditions are met
    ** 1) ue_mode is SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED
    ** 2) PLMN supports SRLTE   */
    if(state_machine->stack_common_info.ue_mode == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED
        && cmss_is_stack2_operational(as_id) == FALSE
        && is_srlte_cap)
    {
      boolean is_1xsrlte_mcc = FALSE;
      cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop(as_id);
      cm_mm_ss_info_gen_stack_info_s_type *ss_info_all_stack_ptr;
      if(ss_info_sub_prop_ptr == NULL ||
          ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info == NULL)
      {
        return FALSE;
      }
      ss_info_all_stack_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info;

      CM_MSG_HIGH_3("CMSOA: plmn 0x%x 0x%x 0x%x",
                    ss_info_all_stack_ptr->sys_id.id.plmn.identity[0],
                    ss_info_all_stack_ptr->sys_id.id.plmn.identity[1],
                    ss_info_all_stack_ptr->sys_id.id.plmn.identity[2]);

      if(policyman_svd_possible_on_plmn_per_subs(&(ss_info_all_stack_ptr->sys_id.id.plmn),
                                                 FALSE, &is_1xsrlte_mcc, as_id) ==
                                                                POLICYMAN_STATUS_SUCCESS)
      {
        cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(as_id);
        if(cmsoa_state != NULL)
        {
          CM_MSG_HIGH_2("SRLTE: SRV_IND: is_1xsrlte_plmn=%d, curr_sub_mode=%d",
                        is_1xsrlte_mcc,
                        cmsoa_state->info.curr_sv_oprt_mode);
        }
        if(is_1xsrlte_mcc == TRUE)
        {
          sd_si_info_s_type *ss_true_stk_info_ptr = cmss_true_stack_info_ptr(cmph_map_sub_stk_to_id(as_id, MM_STACK_0));
          switching_to_srlte = TRUE;
          if(is_switch_needed)
          {
            if (ss_true_stk_info_ptr != NULL &&
                ss_true_stk_info_ptr->extend_srv_info != SYS_EXTEND_SRV_INFO_REG_COMP_ACK_PENDING)
            {
              cmsoa_clear_active_attach_complete_timer(as_id);
            }
            cmsoa_sv_switch_base(CMSOA_SV_OPRT_MODE_SVLTE, as_id,FALSE);
          }
        }
      }
      else
      {
        CM_MSG_HIGH_0("SRLTE: fail to check policyman plmn - WARNING!!!");
      }
    }
  }
  return switching_to_srlte;
}

/*===========================================================================

FUNCTION cmsoa_err_fatal_wrong_sv_state

DESCRIPTION
  This function performs all neccessary checks to determine a incosistent satate where:
  A) UE has all the capaility to perform switch to SRLTE
  B) But still hybr2 is not operational . For some reason , switch couldnt be performed.

  We check below conditions to do ERR FATAL in such case:
  1.cmsoa sv state=SVLTE, Ue should be in dual stack.
  2.Sub feature mode is SRLTE.
  3.Top of preference queue is PH_OBJ. No other high priority obj is holding the switch.
  4.We are in service
  5.As per device configuration(mode pref, band perf, subscription, hybr pref),
    LTE stack should be operational.
  6.And current camped plmn is SRLTE PLMN.
  7.But still hybr_gw_operational is FALSE, meaning UE is not performing swiytch and is
     still in single stack

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_err_fatal_wrong_sv_state(
  sys_modem_as_id_e_type asubs_id
)
{
  cm_orig_q_s_type  *ph_top_ptr = cmtask_orig_para_get_top(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0));
  cmmsc_state_machine_s_type *state_machine = cmmsimc_state_machine_ptr(asubs_id);
  sd_si_info_s_type         *si_info_ptr = cmss_true_stack_info_ptr(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0));
  boolean is_1xsrlte_mcc = FALSE;
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  cm_mm_ss_info_gen_stack_info_s_type *ss_info_stack_ptr = cmss_get_info_stk_ptr(asubs_id, MM_STACK_0);
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);
  if(cmsoa_state == NULL || si_info_ptr == NULL || ph_sub_ptr == NULL ||
      !cmutil_is_as_id_valid(asubs_id) || ss_info_stack_ptr == NULL
    )
  {
    return;
  }

  if((sys_srv_status_is_full_srv(si_info_ptr->srv_status) == FALSE) ||
      (si_info_ptr->is_stable_in_svc == FALSE))
  {
    CM_MSG_LOW_1("ERR_FATAL: srv status=%d", si_info_ptr->srv_status);
    return;
  }

  if( cmmsc_auto_is_hybr_gwl_allowed(
        state_machine,
        SD_SS_HYBR_PREF_CDMA__LTE__HDR,
        ph_sub_ptr->is_3gpp2_subs_avail,
        FALSE) == FALSE)
  {
    CM_MSG_LOW_0("ERR_FATAL: hybr gwl not allowed");
    return;
  }


  /* In non-MMSS configuration, we do not need to switch to dual stack mode
  */
  if(sd_misc_is_mmss_operation(asubs_id) == FALSE)
  {
    CM_MSG_LOW_0("ERR_FATAL: Not MMSS config");
    return;
  }

  if(!cmsoa_is_srlte_capability(asubs_id))
  {
    CM_MSG_LOW_0("ERR_FATAL: Not SRLTE capable");
    return;
  }

  if(SD_MODE_CONTAIN( BM(SYS_SYS_MODE_LTE),BM(ss_info_stack_ptr->sys_mode))  &&
      (policyman_svd_possible_on_plmn_per_subs(&(ss_info_stack_ptr->sys_id.id.plmn),
                                                FALSE, &is_1xsrlte_mcc, asubs_id) ==
                                                            POLICYMAN_STATUS_SUCCESS))
  {
    if(is_1xsrlte_mcc == FALSE)
    {
      CM_MSG_LOW_2("ERR_FATAL: is_1xsrlte_plmn=%d, curr_sub_mode=%d",
                   is_1xsrlte_mcc,
                   cmsoa_state->info.curr_sv_oprt_mode);
      return;
    }
  }
  else if (SD_MODE_CONTAIN(BM(SYS_SYS_MODE_CDMA),BM(ss_info_stack_ptr->sys_mode)) &&
           cmsoa_state->config.is_c2k_modeswitch2srlte)
  {
    if (!cmsoa_is_c2kswitch_possible(asubs_id))
    {
      CM_MSG_HIGH_1("ERR_FATAL: curr_sub_mode=%d",
                    cmsoa_state->info.curr_sv_oprt_mode);
      return;
    }
  }
  else
  {
    CM_MSG_LOW_0("ERR_FATAL: fail to check policyman plmn - WARNING!!!");
    return;
  }

  if (  (ph_top_ptr != NULL ) && (ph_top_ptr->act_type == CM_ACT_TYPE_PH_OBJ) &&
        (!(cmac_is_in_thermal_emergency(asubs_id)|| cmac_is_in_thermal_emergency_level2(asubs_id))) &&
        !cmcall_is_there_a_call_activity(asubs_id) &&
        (cmcall_is_there_a_call_type_no_call_id_per_sub(asubs_id,CM_CALL_TYPE_PS_DATA,CM_CALL_ID_INVALID) == CM_CALL_ID_INVALID ) &&
        ((cmtask_orig_para_search_act_type(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0), CM_ACT_TYPE_DATA_CALL)) == NULL))
  {
    CM_ERR_FATAL_0("Error:CMSOA:current sv-opr state is SVLTE in Single Stack");
  }

}


/*===========================================================================

FUNCTION cmsoa_sv_switch_base

DESCRIPTION
  This function performs all neccessary checks before it decides to whether
  switching to SVLTE or CSFB operation.
  If a switch() operation is required, it will call to another function that
  will only performs the switch.

DEPENDENCIES
  none

RETURN VALUE
  TRUE - switch happens
  FALSE - swith did no happen

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsoa_sv_switch_base(
  cmsoa_sv_oprt_mode_e_type  new_state,
  sys_modem_as_id_e_type     asubs_id,
  boolean                    switch_on_ims_reg
)
{
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  cmsoa_sv_switch_delay_reas_e_type delay_reas = CMSOA_SV_SWITCH_DELAY_REAS_NONE;
  cmph_s_type                   *ph_ptr = cmph_ptr();

  if(cmsoa_state == NULL|| !IS_VALID_PH_SUB_PTR(asubs_id))
  {
    return FALSE;
  }
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_MSG_HIGH_3_EXT("DS: SUB %d CMSOA: switch_base(), new_state=%d, switch_enabled=%d",
                    asubs_id, new_state, cmsoa_state->config.is_switch_enabled, (asubs_id+1));

  if (cmsoa_state->config.is_switch_enabled == FALSE)
  {
    return FALSE;
  }

  if (!cmph_is_subs_feature_mode_1x_sxlte(asubs_id))
  {
    CM_MSG_HIGH_0("CMSOA: non svlte/srlte sub");
    return FALSE;
  }

  /*For SRLTE, if switch is not allowed as CSIM not yet completely activated, skip the switch
  */
  if(cmph_is_subs_feature_mode_srlte(asubs_id) &&
      new_state == CMSOA_SV_OPRT_MODE_SVLTE &&
      !cmmmgsdi_is_switch_allowed_per_csim_self_activation(asubs_id))
  {
    CM_MSG_HIGH_0("CMSOA: Switch to SRLTE is not allowed as CSIM not activated");
    return FALSE;
  }

  if (ph_ptr->sub_info[asubs_id]->pref_info.network_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_MANUAL)
  {
    CM_MSG_HIGH_0("Skip prediction in MANUAL mode since device will be 3GPP only device");
    return FALSE;
  }

  if (cmclnup_is_ongoing_subsc(BM(asubs_id)) ||
      cmclnup_is_ongoing_oprt_mode() ||
      ph_ptr->device_prop.ph_oprt_mode.is_oprt_mode_change_ongoing)
  {
    CM_MSG_HIGH_0("CMSOA: Skip switch to to ongoing subscription or oprt_mode change");
    return FALSE;
  }

  /* Dont switch to SRLTE ,
  ** while waiting for ESR response for a WPS call .
  ** Since the switch causes NAS to drop the ESR .
  ** However the switch will be forced immediately
  ** after this WPS call ends .
  */
  if (cmcall_is_there_hvolte_call_waiting_for_esr_rsp() != CM_CALL_ID_INVALID
      && new_state == CMSOA_SV_OPRT_MODE_SVLTE)
  {
    CM_MSG_HIGH_0("Hvolte:Switch postponed to call end");
    return FALSE;
  }

  if (cmsoa_state->info.is_attach_complete_timer_active)
  {
    CM_MSG_HIGH_0("ATTACH: Switch stopped due to active attach complete timer");
    return FALSE;
  }

  /* Bail out if current state is the same as new state */
  if (cmsoa_state->info.curr_sv_oprt_mode == new_state)
  {
    /* For ping-pong scenario below, make sure we clear the action and armed
    ** the guard-timer again.
    ** 1. UE switches from CSFB --> SVLTE. Guard-timer is set.
    ** 2. UE switches back SVLTE --> CSFB. Not allowed during hysterisis period.
    ** 3. UE set switch-action. It remains in SVLTE and wait for guard timer expires.
    ** 4. It found MCC belongs to SVLTE again.
    ** 5. We should clear the action in step #3, and re-set in step #1.
    */
    //might not need this condi
    if (cmsoa_state->info.switch_delay_timer != 0 &&
        cmsoa_state->info.switch_delay_reason == CMSOA_SV_SWITCH_DELAY_REAS_PINGPONG
#if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
        && cmph_is_oprting_in_csfb_pref_srlte(asubs_id) == FALSE
#endif
       )
    {
      cmsoa_arm_timer(CMSOA_SV_OPRT_MODE_NONE,
                      CMSOA_SV_SWITCH_DELAY_REAS_PINGPONG,
                      asubs_id);

    }
    CM_MSG_HIGH_1("CMSOA: current sv-opr state = new state (= %d)",new_state);
    return FALSE;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Ignore the ping pong & lte connected delay timers ,
  ** if the switch is due to IMS registration .
  ** However MT page pending delay timer should be considered ,
  ** to make sure no MT calls are dropped due to switch */
  /* If we don't support Voice - new state is either SVLTE (based on the list) or CSFB */
  delay_reas = cmsoa_sv_switch_is_delayed(asubs_id);

  if (delay_reas != CMSOA_SV_SWITCH_DELAY_REAS_NONE
      &&
      (switch_on_ims_reg == FALSE ||
       delay_reas == CMSOA_SV_SWITCH_DELAY_REAS_VOICE_CALL   ||
       delay_reas == CMSOA_SV_SWITCH_DELAY_VOLTE_ON_HYBR))
  {
    cmsoa_state->info.switch_delay_state = new_state;
    /* except for ping-pong we need to arm the timer
    ** for ping-pong, timer is already armed */
    if (delay_reas != CMSOA_SV_SWITCH_DELAY_REAS_PINGPONG)
    {
      cmsoa_arm_timer(new_state,
                      delay_reas,
                      asubs_id);
    }
    CM_MSG_HIGH_4("CMSOA: BUFFER switch, delay_reas %d, switch_delay_state %d, current state %d, sub %d",
                     delay_reas,
                     cmsoa_state->info.switch_delay_state,
                     cmsoa_state->info.curr_sv_oprt_mode,
                     asubs_id);
    return FALSE;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* main logic */

  switch (cmsoa_state->info.curr_sv_oprt_mode)
  {
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    case CMSOA_SV_OPRT_MODE_SVLTE:
    {
      if (new_state == CMSOA_SV_OPRT_MODE_CSFB)
      {
        /* Switching from SVLTE to CSFB */
        cmsoa_sv_switch_svlte_csfb(asubs_id);
      }
      break;
    }

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    case CMSOA_SV_OPRT_MODE_CSFB:
    {
      if (new_state == CMSOA_SV_OPRT_MODE_SVLTE)
      {
        /* Switching from CSFB to SVLTE */
        cmsoa_sv_switch_csfb_svlte(asubs_id);
      }
      break;
    }

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    default:
      CM_MSG_HIGH_0("Incorrect SV State");
      return FALSE;
  }
  return TRUE;
}

static cmsoa_sv_switch_action_e_type cmsoa_state_to_action(cmsoa_sv_oprt_mode_e_type state)
{
  cmsoa_sv_switch_action_e_type action;
  switch( state )
  {

    case CMSOA_SV_OPRT_MODE_SVLTE:
      action = CMSOA_SV_SWITCH_ACTION_SWITCH_SVLTE;
      break;

    case CMSOA_SV_OPRT_MODE_CSFB:
      action = CMSOA_SV_SWITCH_ACTION_SWITCH_CSFB;
      break;

    case CMSOA_SV_OPRT_MODE_NONE:
    default:
      action = CMSOA_SV_SWITCH_ACTION_NONE;
      break;
  } /* switch( ss_info_ptr->sys_id.id_type ) */
  return action;
}


/*===========================================================================

FUNCTION cmsoa_sv_switch_svlte_csfb

DESCRIPTION
  This function performs a switch from SVLTE -> CSFB by turning off the
  hybrid preference to non-SVLTE.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void cmsoa_sv_switch_svlte_csfb(
  sys_modem_as_id_e_type     asubs_id
)
{
  /* Indicate the hybr preference setting */
  cm_hybr_pref_e_type              new_hybr_pref;

  /* Pointer to phone object */
  cmph_s_type                   *ph_ptr = cmph_ptr( );
  cmsoa_state_sub_info_s_type   *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  cmmsc_state_machine_s_type    *state_machine = cmmsimc_state_machine_ptr(asubs_id);
  sys_ue_mode_e_type            curr_ue_mode = state_machine->stack_common_info.ue_mode;
  mm_sub_stk_id_s_type          stk0_mm_id = cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0);
  mm_sub_stk_id_s_type          stk2_mm_id = cmph_map_sub_stk_to_id(asubs_id, MM_STACK_2);
  cm_pref_s_type *pref_ptr = NULL;
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Error checking */

  if (ph_ptr == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  if(cmsoa_state == NULL || ph_sub_ptr == NULL)
  {
    return;
  }

  /* Update the hybr-pref */
  if (ph_sub_ptr->pref_info.hybr_pref == CM_HYBR_PREF_CDMA__LTE__HDR)
  {
    if(state_machine->stack_common_info.ue_mode == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED)
    {
      CM_MSG_HIGH_0("CMSOA: Switching SRLTE -> CSFB");
    }
    else
    {
      CM_MSG_HIGH_0("CMSOA: Switching SVLTE -> CSFB");
    }
    new_hybr_pref = CM_HYBR_PREF_CDMA__HDR;
  }
  else
  {
    CM_MSG_HIGH_0("CMSOA: UE is not in SVLTE mode");
    return;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Execution */
  cmph_adjust_hybr_pref_for_pending_pref(new_hybr_pref);

  /* Set the pref_update_reason so we could send this reason down to SD */
  cmsoa_set_pref_update_reason( SD_SS_PREF_UPDATE_REASON_SWITCH_CSFB, asubs_id );

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the current SV-oprt mode */
  cmsoa_state->info.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_CSFB;

  /* Write back the new SV-OPR mode into the EFS */
  cmsoa_state->config.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_CSFB;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmph_remove_orig_mode_with_priority_end(stk0_mm_id);
  cmph_remove_orig_mode_with_priority_end(stk2_mm_id);

  if ((pref_ptr = cm_pref_ptr()) == NULL) return;

  memscpy(pref_ptr, sizeof(cm_pref_s_type),
          &(ph_sub_ptr->pref_info), sizeof(cm_pref_s_type));
  pref_ptr->hybr_pref = new_hybr_pref;

  (void)cmph_force_pref_on_the_fly_stack(
    cmph_map_sub_stk_to_id(asubs_id, MM_STACK_ALL),
    SD_SS_PREF_REAS_USER,
    CM_ACT_TYPE_PH_OBJ,
    pref_ptr,
    CM_OTASP_ACT_CODE_NONE,
    (cm_act_id_type)ph_ptr,
    CM_ACT_UPDATE_REAS_USER,
    FALSE,
    cmph_get_acq_pri_order_per_sub(asubs_id),
    CM_DEFAULT_VALUE);
  cm_mem_free (pref_ptr);

  if (BETWEEN((int)ph_ptr->device_prop.curr_nam, (int)CM_NAM_NONE, (int)NV_MAX_NAMS))
  {
    CM_MSG_HIGH_2("Changing persistent hybr_pref from %d to %d", ph_sub_ptr->prst_pref_info[cmph_ptr()->device_prop.curr_nam].hybr_pref, new_hybr_pref);
    ph_sub_ptr->prst_pref_info[cmph_ptr()->device_prop.curr_nam].hybr_pref = new_hybr_pref;
  }

  /* Update phone persistent values */
  if (curr_ue_mode != SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED)
  {
    cmsoa_efs_soa_s_type temp_config;
    temp_config.curr_sv_oprt_mode = cmsoa_state->config.curr_sv_oprt_mode;
    temp_config.is_switch_enabled= cmsoa_state->config.is_switch_enabled;
    temp_config.is_volte12= cmsoa_state->config.is_volte12;
    memscpy(temp_config.svlte_mcc_list, sizeof(temp_config.svlte_mcc_list), cmsoa_state->config.svlte_mcc_list, sizeof(cmsoa_state->config.svlte_mcc_list));

    cmsoa_write_soa_config_efs(&temp_config, asubs_id);
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Cleaning up */

  /* After we forced the preference, clear the reason */
  cmsoa_set_pref_update_reason( SD_SS_PREF_UPDATE_REASON_NONE, asubs_id );

  /* Reset variables after switching to CSFB */
  *cmph_get_is_kicked_hybr_2_ptr() = FALSE;

  /* Also reset the voice-call timer / ping-pong timer after we switched */
  cmsoa_arm_timer(CMSOA_SV_OPRT_MODE_NONE,
                  CMSOA_SV_SWITCH_DELAY_REAS_NONE,
                  asubs_id);

  /* Log the dynamic switch event */
  cmlog_csfb_svlte_switch( cmsoa_state_to_action(CMSOA_SV_OPRT_MODE_CSFB),asubs_id);

  return;
}

/*===========================================================================

FUNCTION cmsoa_sv_switch_csfb_svlte

DESCRIPTION
  This function performs a switch from CSFB -> SVLTE by turning off the
  hybrid preference to SVLTE. It also required to check if SVLTE is possible
  before performing the switch.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void cmsoa_sv_switch_csfb_svlte(
  sys_modem_as_id_e_type     asubs_id
)
{
  /* Indicate the hybr preference setting */
  cm_hybr_pref_e_type              new_hybr_pref;

  /* Pointer to phone object */
  cmph_s_type                     *ph_ptr = cmph_ptr( );

  /* Pointer to phone system selection object */
  cmss_s_type                     *ss_ptr = cmss_ptr();

  cmsoa_state_sub_info_s_type   *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  cmmsc_state_machine_s_type    *state_machine = cmmsimc_state_machine_ptr(asubs_id);
  sys_ue_mode_e_type            curr_ue_mode = state_machine->stack_common_info.ue_mode;
  mm_sub_stk_id_s_type          stk0_mm_id = cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0);
  mm_sub_stk_id_s_type          stk2_mm_id = cmph_map_sub_stk_to_id(asubs_id, MM_STACK_2);
  cm_pref_s_type *pref_ptr = NULL;
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Error checking */
  if (ph_ptr == NULL || ss_ptr == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }
  if(cmsoa_state == NULL|| ph_sub_ptr == NULL)
  {
    return;
  }

  if ( ph_ptr->device_prop.trm_cap.is_svlte_supported == FALSE &&
       curr_ue_mode != SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED)
  {
    CM_MSG_HIGH_0("CMSOA: Phone is not capable of SVLTE");
    return;
  }

  /* Update the hybr-pref */
  if ((ph_sub_ptr->pref_info.hybr_pref == CM_HYBR_PREF_CDMA__HDR) ||
      ((ph_sub_ptr->pref_info.hybr_pref == CM_HYBR_PREF_CDMA__LTE__HDR) &&
       (state_machine->stack_common_info.ue_mode == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED))
     )
  {
    if(state_machine->stack_common_info.ue_mode == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED)
    {
      CM_MSG_HIGH_0_EXT("CMSOA: Switching CSFB -> SRLTE", (asubs_id+1));
    }
    else
    {
      CM_MSG_HIGH_0("CMSOA: Switching CSFB -> SVLTE");
    }
    new_hybr_pref = CM_HYBR_PREF_CDMA__LTE__HDR;
  }
  else
  {
    CM_MSG_HIGH_0("CMSOA: UE is not in hybr-mode enabled");
    return;
  }

  CM_MSG_HIGH_2("def-delay-timer %d %d",cmsoa_state->config.sxlte_timers.switch_pingpong_guard_timer,
                cmsoa_state->config.sxlte_timers.lte_idle_timer);


  cmph_adjust_hybr_pref_for_pending_pref(new_hybr_pref);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Execution */

  /* Set pref_update_Reason so we could send this reason to SD */
  {
    sd_ss_pref_update_reason_e_type  pref_update_reas;
    pref_update_reas = cmsoa_get_pref_update_reason(asubs_id);
    if (pref_update_reas != SD_SS_PREF_UPDATE_REASON_ACTIVATE_HYBR2 &&
        pref_update_reas != SD_SS_PREF_UPDATE_REASON_ACTIVATE_HYBR2_SILENT )
    {
      pref_update_reas = SD_SS_PREF_UPDATE_REASON_SWITCH_SVLTE;
      cmsoa_set_pref_update_reason( SD_SS_PREF_UPDATE_REASON_SWITCH_SVLTE, asubs_id );
    }
    CM_MSG_HIGH_2("CMSOA: sub[%d] pref update reason %d", asubs_id,pref_update_reas );
  }
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmph_remove_orig_mode_with_priority_end(stk0_mm_id);
  cmph_remove_orig_mode_with_priority_end(stk2_mm_id);

  /* After switched, update the state */
  cmsoa_state->info.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_SVLTE;

  /* Write back the new SV-OPR mode into the EFS */
  cmsoa_state->config.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_SVLTE;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((pref_ptr = cm_pref_ptr()) == NULL) return;

  memscpy(pref_ptr, sizeof(cm_pref_s_type),
          &(ph_sub_ptr->pref_info), sizeof(cm_pref_s_type));
  pref_ptr->hybr_pref = new_hybr_pref;

  (void)cmph_force_pref_on_the_fly_stack(
    cmph_map_sub_stk_to_id(asubs_id, MM_STACK_ALL),
    SD_SS_PREF_REAS_USER,
    CM_ACT_TYPE_PH_OBJ,
    pref_ptr,
    CM_OTASP_ACT_CODE_NONE,
    (cm_act_id_type)ph_ptr,
    CM_ACT_UPDATE_REAS_USER,
    FALSE,
    cmph_get_acq_pri_order_per_sub(asubs_id),
    CM_DEFAULT_VALUE);
  cm_mem_free (pref_ptr);
  if (BETWEEN((int)ph_ptr->device_prop.curr_nam, (int)CM_NAM_NONE, (int)NV_MAX_NAMS))
  {
    CM_MSG_HIGH_2("Changing persistent hybr_pref from %d to %d", cmph_ptr()->sub_info[asubs_id]->prst_pref_info[cmph_ptr()->device_prop.curr_nam].hybr_pref, new_hybr_pref);

    /* Update phone persistent values */
    cmph_ptr()->sub_info[asubs_id]->prst_pref_info[cmph_ptr()->device_prop.curr_nam].hybr_pref = new_hybr_pref;
  }

  if (curr_ue_mode != SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED)
  {
    cmsoa_efs_soa_s_type temp_config;
    temp_config.curr_sv_oprt_mode = cmsoa_state->config.curr_sv_oprt_mode;
    temp_config.is_switch_enabled= cmsoa_state->config.is_switch_enabled;
    temp_config.is_volte12= cmsoa_state->config.is_volte12;
    memscpy(temp_config.svlte_mcc_list,sizeof(temp_config.svlte_mcc_list), cmsoa_state->config.svlte_mcc_list, sizeof(cmsoa_state->config.svlte_mcc_list));

    cmsoa_write_soa_config_efs(&temp_config, asubs_id);
  }


  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Cleaning up */

  /* After we forced the preference, clear the reason */
  cmsoa_set_pref_update_reason( SD_SS_PREF_UPDATE_REASON_NONE, asubs_id );

  /* Now that we switched to SVLTE - set the ping-pong guard timer */
  if (cmsoa_state->config.sxlte_timers.switch_pingpong_guard_timer > 0)
  {
    cmsoa_arm_timer(CMSOA_SV_OPRT_MODE_NONE,
                    CMSOA_SV_SWITCH_DELAY_REAS_PINGPONG,
                    asubs_id);
  }
  else
  {
    cmsoa_arm_timer(CMSOA_SV_OPRT_MODE_NONE,
                    CMSOA_SV_SWITCH_DELAY_REAS_NONE,
                    asubs_id);
  }

  /* check if we are in incosistent state where although as per capabilities we should be
  ** in dual stack but current sv state says SVLTE and LTE stack is not operational */
  if (cmsoa_is_srlte_in_inconsistent_state(asubs_id))
  {
    cmsoa_err_fatal_wrong_sv_state(asubs_id);
  }

  /* Log the dynamic switch event */
  cmlog_csfb_svlte_switch( cmsoa_state_to_action(CMSOA_SV_OPRT_MODE_SVLTE), asubs_id);

  return;
}


/*--------------------------------------------------------------------------
** FUNCTIONS
** - Section 1: Contains all the queues related functions.
** - Section 2: Contains all the initialization functions, read NV.
** - Section 3: Contains all the command/reports/msg processing functions
** - Section 4: Contains all the Utilities functions
**--------------------------------------------------------------------------
*/


/***************************************************************************
** FUNCTIONS - SECTION 1: Queues / linked list / sending messages
** This section contains function handles queues, tasks related to CMSOA.
***************************************************************************/

/*===========================================================================

FUNCTION cmsoa_svlte_plmn_list_add

DESCRIPTION
  This function add a node which contains a PLMN to the SVLTE-PLMN-ID list

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_svlte_plmn_list_add(

  cm_cmd_q_type                      *q_ptr,
  /* pointer to a command queue to enqueue command buffer onto */

  cmsoa_svlte_plmn_id_node_s_type    *node_ptr
  /* pointer to a command buffer to be enqueued */
)
{

  if (q_ptr == NULL || node_ptr == NULL)
  {
    CM_ERR_0("MMSOA: q_ptr | node_ptr == NULL");
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Enqueue the command buffer
  ** into the specified queue.
  */
  (void) q_link( node_ptr, &node_ptr->link);
  q_put( q_ptr, &node_ptr->link );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  CM_MSG_HIGH_3( "CMSOA: cmsoa_svlte_plmn_list_add(), plmn [%x][%x][%x]",
                 node_ptr->plmn.identity[0],node_ptr->plmn.identity[1],
                 node_ptr->plmn.identity[2] );

  return;
}

/*===========================================================================

FUNCTION cmsoa_svlte_plmn_list_search

DESCRIPTION
  This function search a plmn in the SVLTE-PLMN-ID list.

DEPENDENCIES
  none

RETURN VALUE
  boolean - indicates if the plmn is found or not.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsoa_svlte_plmn_list_search(

  cm_cmd_q_type                      *q_ptr,
  /* pointer to a command queue to enqueue command buffer onto */

  sys_plmn_id_s_type                 *plmn
  /* plmn_info of the node */
)
{
  cmsoa_svlte_plmn_id_node_s_type *node_position = NULL;

  if (q_ptr == NULL || plmn == NULL)
  {
    CM_ERR_0("MMSOA: plmn == NULL or q_ptr == NULL");
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Enqueue the command buffer
  ** into the specified queue.
  */
  node_position = q_linear_search(q_ptr, cmsoa_compare_plmn_func, (void *)plmn);

  if( node_position != NULL )
  {
    CM_MSG_HIGH_3( "CMSOA: Found plmn [%x][%x][%x]",node_position->plmn.identity[0],
                   node_position->plmn.identity[1],node_position->plmn.identity[2]);
    return TRUE;
  }

  return FALSE;

}

/*===========================================================================

FUNCTION cmsoa_svlte_mcc_list_search

DESCRIPTION
  This function search a MCC in the SVLTE-MCC list.

DEPENDENCIES
  none

RETURN VALUE
  boolean - indicates if the MCC is found or not.

SIDE EFFECTS
  none

===========================================================================*/
boolean       cmsoa_svlte_mcc_list_search(

  /* MCC which we want to search */
  uint32     mcc,
  sys_modem_as_id_e_type asubs_id
)
{
  /* State info pointer contains MCC list */
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  int                      i;

  if (cmsoa_state == NULL)
  {
    return FALSE;
  }

  for (i = 0; i < CMSOA_SVLTE_MCC_LIST_MAX; i++)
  {
    if ( cmsoa_state->config.svlte_mcc_list[i] == mcc)
    {
      CM_MSG_HIGH_1("MMSOA: found MCC %d in SVLTE-MCC list", mcc);
      return TRUE;
    }
  }

  return FALSE;
}

/*===========================================================================

FUNCTION cmsoa_regprx_srv_req_proceed_request_msg

DESCRIPTION
  This functions sends a messages to generic report queues asking for
  permission from CMSOA (arbitration module) to send SERVICE_REQ

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_regprx_srv_req_proceed_request_msg(
  mm_sub_stk_id_s_type  mm_id
)
{

  cm_generic_rpt_s_type   *soa_rpt_ptr = NULL;
  soa_rpt_ptr             = cm_generic_rpt_get_buf_else_err_fatal();

  if (soa_rpt_ptr == NULL)
  {
    sys_err_fatal_null_ptr_exception();
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_MSG_HIGH_1("CMSOA: sub %d cm_soa send regprx_req_msg()", mm_id.asubs_id);

  soa_rpt_ptr->hdr.cmd     =  CM_SOA_REGPRX_SRV_REQ_PROCEED;
  soa_rpt_ptr->hdr.asubs_id  = mm_id.asubs_id;
  /* Store the current SV OPRT mode at the moment of request */
  soa_rpt_ptr->generic_rpt.regprx_srv_req.hyrb2_on
    = cmss_is_stack2_operational(mm_id.asubs_id);
  soa_rpt_ptr->generic_rpt.regprx_srv_req.stk_id = (uint8)mm_id.stk_id;

  cm_generic_rpt(soa_rpt_ptr);
}

/***************************************************************************
** FUNCTIONS - SECTION 2: Initializations
** Contains all the initialization functions, read NV.
***************************************************************************/

/*===========================================================================

FUNCTION cmsoa_sub_info_mem_free

DESCRIPTION
  This functions frees cmsoa sub info

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_sub_info_mem_free(
  sys_modem_as_id_e_type asubs_id
)
{
  cmsoa_state_info_s_type *cmsoa_state = cmsoa_state_info_ptr();
  cmsoa_svlte_plmn_id_node_s_type *node_ptr;

  if(!(asubs_id >= SYS_MODEM_AS_ID_1 &&
       asubs_id < ARR_SIZE(cmsoa_state->sub_info) &&
       asubs_id < SYS_MODEM_AS_ID_NO_CHANGE))
  {
    CM_MSG_HIGH_1("cmsoa sub info mem free wrong sub %d", asubs_id);
    asubs_id = SYS_MODEM_AS_ID_1;
  }

  if (cmsoa_state->sub_info[asubs_id] != NULL)
  {
    /* free memory for all plmn nodes and cmsoa structure */
    node_ptr = (cmsoa_svlte_plmn_id_node_s_type *)q_get(&cmsoa_state->sub_info[asubs_id]->config.svlte_plmn_list);
    while(node_ptr != NULL)
    {
      if(node_ptr != NULL)
      {
        cm_mem_free(node_ptr);
        node_ptr = NULL;
      }
      node_ptr = (cmsoa_svlte_plmn_id_node_s_type *)q_get(&cmsoa_state->sub_info[asubs_id]->config.svlte_plmn_list);
    }
    CM_MSG_HIGH_1 ("free cmsoa memory for sub %d", asubs_id);
    /* free memory for cmsoa structure */
    cm_mem_free(cmsoa_state->sub_info[asubs_id]);
    cmsoa_state->sub_info[asubs_id] = NULL;
  }

}

void cmsoa_init_soa_info(
  sys_modem_as_id_e_type asubs_id
)
{
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  sys_plmn_id_s_type         temp_plmn;

  /* Initialize non-configurables values */
  sys_plmn_undefine_plmn_id(&temp_plmn);
  cmsoa_state->info.last_gwl_plmn = temp_plmn;
  cmsoa_state->info.pref_update_reason = SD_SS_PREF_UPDATE_REASON_NONE;

  cmsoa_state->info.switch_delay_timer = 0;
  cmsoa_state->info.switch_delay_state = CMSOA_SV_OPRT_MODE_NONE;

  cmsoa_state->info.is_activated = FALSE;
  cmsoa_state->info.is_svlte_predicted = FALSE;
  cmsoa_state->info.is_attach_complete_timer_active = FALSE;

  cmsoa_state->info.curr_sv_oprt_mode = cmsoa_state->config.curr_sv_oprt_mode;

}

/*===========================================================================

FUNCTION cmsoa_init

DESCRIPTION
  This functions initialized cmsoa state info.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_init(
  boolean           is_pwr_on,
  uint8             refresh_subs_mask
  /* Subs mask whose NVs need to be refreshed
  */
)
{
  uint8                      i, j;
  cmsoa_state_info_s_type   *cmsoa_state_info = cmsoa_state_info_ptr();
  uint8                      num_sub = MIN(MAX_SIMS, cmpmprx_get_num_of_sims());

  /* free extra memory if num of sims decreased */
  for(i=num_sub; i<MIN(MAX_SIMS, cmsoa_state_info->count); i++)
  {
    cmsoa_sub_info_mem_free((sys_modem_as_id_e_type)i);
  }
  cmsoa_state_info->count = num_sub;

  for(j=0; j<num_sub; j++)
  {
    if(cmsoa_state_info->sub_info[j] == NULL)
    {
      cmsoa_state_info->sub_info[j] = (cmsoa_state_sub_info_s_type *)cm_mem_malloc
                                      (sizeof(cmsoa_state_sub_info_s_type));
    }
    if(refresh_subs_mask & BM(j))
    {
      /* Initialization */
      cmsoa_read_soa_config_efs(is_pwr_on, (sys_modem_as_id_e_type)j);
      cmsoa_init_soa_info((sys_modem_as_id_e_type)j);
    }
  }
}

/*===========================================================================

FUNCTION cmsoa_read_plmn_id_efs

DESCRIPTION
  This function reads the SVLTE PLMN ID list from EFS

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsoa_read_plmn_id_efs(
  sys_modem_as_id_e_type asubs_id
)
{
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);

  /* Store all the plmns read to an array - now assume 64 elements */
  sys_plmn_id_s_type plmn_arr[64];
  int32  size = sizeof(plmn_arr);
  int counter = 0;
  int plmn_num = 0;
  sys_subs_feature_t sfmode = SYS_SUBS_FEATURE_MODE_NORMAL;

  /* Clear the plmn_arr[] */
  memset(plmn_arr,0,size );

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if(cmsoa_state == NULL)
  {
    return FALSE;
  }
  cmpmprx_get_subs_feature_mode(asubs_id, &sfmode);
  if(sfmode == SYS_SUBS_FEATURE_MODE_SVLTE &&
      cmefs_get(CMEFS_SOA_SVLTE_PLMN_ID_LIST, (byte *)plmn_arr, size, asubs_id))
  {
    /* Calculate with the read size, how much plmn we have */
    plmn_num = (int)(size / sizeof(sys_plmn_id_s_type));

    /* If we could read the file successfully, then add all the plmn
    ** into the array
    */
    for (counter = 0; counter < plmn_num; counter++)
    {
      /* Dynamically creates a node */
      cmsoa_svlte_plmn_id_node_s_type    *node_ptr =
        cmsoa_svlte_plmn_id_node_buf_else_err_fatal();

      if (node_ptr == NULL)
      {
        continue;
      }

      /* Stop if until we have a plmn id that's not defined */
      if (sys_plmn_id_is_undefined(plmn_arr[counter]))
      {
        modem_mem_free((void *)node_ptr, MODEM_MEM_CLIENT_MMODE);
        break;
      }

      /* Update the node_ptr and adding to the list*/
      node_ptr->plmn = plmn_arr[counter];
      cmsoa_svlte_plmn_list_add(&cmsoa_state->config.svlte_plmn_list, node_ptr);
    }
    return TRUE;
  }
  else
  {
    return FALSE;
  }

  return TRUE;
}

/*===========================================================================

FUNCTION cmsoa_write_plmn_id_efs_default

DESCRIPTION
  This function write default SVLTE-PLMN (311-480) into the EFS

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_write_plmn_id_efs_default(
  sys_modem_as_id_e_type asubs_id
)
{

  sys_plmn_id_s_type plmn_arr[64];
  int32  size = sizeof(sys_plmn_id_s_type);
  sys_subs_feature_t pry_sfmode = SYS_SUBS_FEATURE_MODE_NORMAL;

  cmpmprx_get_subs_feature_mode(asubs_id,&pry_sfmode);

  if(pry_sfmode != SYS_SUBS_FEATURE_MODE_SVLTE)
  {
    return;
  }

  /* Clear the plmn_arr[] */
  memset(plmn_arr,0,size );

  /* Default SVLTE PLMN-ID contains the U.S. MCC:
  ** MCC 311, MNC 480
  */
  sys_plmn_set_mcc_mnc2(TRUE,311,480,&plmn_arr[0]);

  /*set the size to be 1 items only */
  size = size*1;

  CM_MSG_MED_1 ("CMSOA: Writing sub[%d] default SVLTE-PLMN-ID list", asubs_id);
  cmefs_fs_write( CMEFS_SOA_SVLTE_PLMN_ID_LIST,(byte *)&plmn_arr, size, asubs_id);

  return;
}

/*===========================================================================

FUNCTION cmsoa_check_and_dealloc_svlte_plmn_list

DESCRIPTION
  It deallocate memory for cmsoa svlte plmn list.

DEPENDENCIES
  cmsoa_state->svlte_plmn_list should be already intialized

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

static void cmsoa_check_and_dealloc_svlte_plmn_list(sys_modem_as_id_e_type asubs_id)
{

  cmsoa_svlte_plmn_id_node_s_type  *cur_ptr = NULL;
  cmsoa_svlte_plmn_id_node_s_type  *next_ptr = NULL;
  int                      q_counter = 0;
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  q_counter = q_cnt(&(cmsoa_state->config.svlte_plmn_list));

  /* If queue has some elements, delete the memory allocated for them */
  if( q_counter > 0)
  {
    // delete all the items from the queue
    cur_ptr = q_check(&(cmsoa_state->config.svlte_plmn_list));

    if( NULL == cur_ptr )
    {
      sys_err_fatal_null_ptr_exception();
    }

    while((q_counter > 0) && (cur_ptr != NULL))
    {
      next_ptr = q_next(&(cmsoa_state->config.svlte_plmn_list), &(cur_ptr->link));

#ifdef FEATURE_Q_NO_SELF_QPTR
      q_delete( &(cmsoa_state->config.svlte_plmn_list), &cur_ptr->link );
#else
      q_delete( &cur_ptr->link );
#endif

      /* Free memory for this node */
      cm_mem_free( cur_ptr);

      q_counter--;
      cur_ptr = next_ptr ;
    } /* end while */
  }/* end if */

}

/*===========================================================================

FUNCTION cmsoa_read_soa_config_efs

DESCRIPTION
  This function reads SOA configuration from EFS

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void cmsoa_read_soa_config_efs(
  boolean                is_pwr_on,
  sys_modem_as_id_e_type asubs_id
)
{
  int32 size = sizeof(cmsoa_efs_soa_s_type);
  sys_ue_mode_e_type curr_ue_mode;
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  sys_subs_feature_t pry_sfmode = SYS_SUBS_FEATURE_MODE_NORMAL;
  cmsoa_efs_soa_s_type temp_config;
  uint8 i;

  cmpmprx_read_ue_mode(asubs_id,&curr_ue_mode);
  cmpmprx_get_subs_feature_mode(asubs_id,&pry_sfmode);

  CM_MSG_HIGH_2("CMSOA: sub[%d] ue_mode %d",asubs_id, curr_ue_mode);

  /*If this is MCFG refresh, update ue mode read from PM to CMMSC state machine as well */
  if(is_pwr_on == FALSE)
  {
    cmmsc_state_machine_s_type   *state_machine_ptr = cmmsimc_state_machine_ptr(asubs_id);
    state_machine_ptr->stack_common_info.ue_mode = curr_ue_mode;
  }

  if(cmsoa_state == NULL ||
      (pry_sfmode != SYS_SUBS_FEATURE_MODE_SRLTE
       && pry_sfmode != SYS_SUBS_FEATURE_MODE_SVLTE))
  {
    return;
  }

  /* Check if svlte_plmn_list already has some memory,
  ** free it before allocating new memory
  */
  if(!is_pwr_on)
  {
    cmsoa_check_and_dealloc_svlte_plmn_list(asubs_id);
  }
  /* Initialize the SVLTE plmn-id list */
  (void) q_init( &(cmsoa_state->config.svlte_plmn_list) );

  /* Initialize the configurable items with defaults and then read */
  cmsoa_state->config.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_NONE;
  cmsoa_state->config.is_volte12 = TRUE;
  cmsoa_state->config.is_switch_enabled = FALSE;
  cmsoa_state->config.init_sub_mode = CMSOA_SV_OPRT_MODE_NONE;
  cmsoa_state->config.is_c2k_modeswitch2srlte = FALSE;

  for (i = 0; i < CMSOA_SVLTE_MCC_LIST_MAX; i++)
  {
    cmsoa_state->config.svlte_mcc_list[i] = 0;
  }

  size = sizeof(cmsoa_efs_soa_s_type);
  if(cmefs_get(CMEFS_SOA_CONFIG, (byte *)&temp_config, size, asubs_id))
  {
    cmsoa_state->config.curr_sv_oprt_mode = temp_config.curr_sv_oprt_mode;
    cmsoa_state->config.is_switch_enabled = temp_config.is_switch_enabled;
    cmsoa_state->config.is_volte12 = temp_config.is_volte12;
    memscpy(cmsoa_state->config.svlte_mcc_list, sizeof(cmsoa_state->config.svlte_mcc_list), temp_config.svlte_mcc_list, sizeof(temp_config.svlte_mcc_list));
  }
  else
  {
    /* set default values if no EFS is found */
    cmsoa_state->config.is_switch_enabled = FALSE;
    if(pry_sfmode == SYS_SUBS_FEATURE_MODE_SVLTE)
    {
      cmsoa_state->config.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_SVLTE;
      cmsoa_state->config.svlte_mcc_list[0] = 310;
      cmsoa_state->config.svlte_mcc_list[1] = 311;
      cmsoa_state->config.svlte_mcc_list[2] = 312;
    }
    else
    {
      cmsoa_state->config.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_CSFB;
    }
    temp_config.curr_sv_oprt_mode = cmsoa_state->config.curr_sv_oprt_mode;
    temp_config.is_switch_enabled = cmsoa_state->config.is_switch_enabled;
    temp_config.is_volte12 = cmsoa_state->config.is_volte12;
    memscpy(temp_config.svlte_mcc_list, sizeof(temp_config.svlte_mcc_list), cmsoa_state->config.svlte_mcc_list, sizeof(cmsoa_state->config.svlte_mcc_list));
#ifndef FEATURE_DISABLE_CM_INIT_NV_WRITE
    /* And write back the default into EFS item */
    /* Some clean up could be done here by writing the default with passed
    ** in parameters
    */
    cmsoa_write_soa_config_efs(&temp_config, asubs_id);
#endif
  }

  CM_MSG_HIGH_1("CMSOA: init svoa_mode=%d",cmsoa_state->config.curr_sv_oprt_mode);
  if(pry_sfmode == SYS_SUBS_FEATURE_MODE_SRLTE)
  {
    size = sizeof(sxlte_timers_s_type);
    if(!cmefs_get(CMEFS_SXLTE_TIMERS, (byte *)&cmsoa_state->config.sxlte_timers, size, asubs_id))
    {
      cmsoa_state->config.sxlte_timers.switch_pingpong_guard_timer =
        CMSOA_SRLTE_SWITCH_DELAY_REASON_PINGPONG_TIMER;
      cmsoa_state->config.sxlte_timers.lte_idle_timer =
        CMSOA_SRLTE_SWITCH_DELAY_REASON_RRC_IDLE_DEFAULT_TIMER;
    }

    if ( cmefs_get ( CMEFS_ATTACH_COMPLETE_TIMER,(byte *)&cmsoa_state->config.attach_complete_timer, sizeof(unsigned long),
                     asubs_id))
    {
      CM_MSG_HIGH_1 (" CMEFS_ATTACH_COMPLETE_TIMER = %d", cmsoa_state->config.attach_complete_timer);
    }
    else
    {
      CM_ERR_0("Can't read CMEFS_ATTACH_COMPLETE_TIMER");
      /* set default value to 600 ms */
      cmsoa_state->config.attach_complete_timer = CMSOA_ATTACH_COMPLETE_DEFAULT_TIMER;
    }
  }
  /* Check c2k_sxlte_opti_array to decide if
  ** to power up from last sub mode
  */
  cmsoa_state->config.is_pwrup_from_last_submode =
    cmefs_get_c2k_sxlte_opti(C2K_SXLTE_OPTI_PWRUP_FROM_LAST_SUBMODE, asubs_id);

  if(curr_ue_mode == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED)
  {
    cmsoa_state->config.is_switch_enabled = TRUE;
    /* if power up from last sub mode optimization is not enabled
    ** set default curr sub mode to CSFB
    */
    if(cmsoa_state->config.is_pwrup_from_last_submode == FALSE)
    {
      cmsoa_state->config.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_CSFB;
    }
  }

  /* Read value of C2kswitch2srlte from EFS */
  if ( cmefs_get ( CMEFS_C2KSWITCH2SRLTE,
                   (byte *)&cmsoa_state->config.is_c2k_modeswitch2srlte,
                   sizeof(boolean),
                   asubs_id) )
  {
    CM_MSG_HIGH_1 (" CMEFS_C2KSWITCH2SRLTE = %d ", cmsoa_state->config.is_c2k_modeswitch2srlte);
  }
  else
  {
    CM_ERR_0("Can't read CMEFS_C2KSWITCH2SRLTE ");
    /* set default value to TRUE */
    cmsoa_state->config.is_c2k_modeswitch2srlte = TRUE;
  }
  if((curr_ue_mode == SYS_UE_MODE_CSFB ||
      curr_ue_mode == SYS_UE_MODE_NONE ||
      curr_ue_mode == SYS_UE_MODE_NORMAL
     ) &&
      pry_sfmode == SYS_SUBS_FEATURE_MODE_SRLTE
    )
  {
    cmsoa_state->config.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_CSFB;
    cmsoa_state->config.is_switch_enabled = FALSE;
  }
  cmsoa_state->config.init_sub_mode = cmsoa_state->config.curr_sv_oprt_mode;

#ifdef FEATURE_MMODE_DYNAMIC_SV_OPERATION_SWITCH
  if(curr_ue_mode != SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED
      && curr_ue_mode != SYS_UE_MODE_1XSRLTE_ONLY)
  {
    /* After initialize, need to read the value from EFS */
    if (pry_sfmode != SYS_SUBS_FEATURE_MODE_SVLTE ||
        !cmsoa_read_plmn_id_efs(asubs_id))
    {
      /* Dynamically creates a node */
      cmsoa_svlte_plmn_id_node_s_type    *node_ptr =
        cmsoa_svlte_plmn_id_node_buf_else_err_fatal();

      if (node_ptr != NULL)
      {
        /* Default value only has 1 Verizon PLMN ID for SVLTE plmn list */
        sys_plmn_set_mcc_mnc2(TRUE,311,480,&node_ptr->plmn);
        cmsoa_svlte_plmn_list_add(&cmsoa_state->config.svlte_plmn_list, node_ptr);
      }

#ifndef FEATURE_DISABLE_CM_INIT_NV_WRITE
      /* write back to EFS :: better design, should pass in a default list
      ** that way, we only have to set the default value at 1 place
      */
      cmsoa_write_plmn_id_efs_default((sys_modem_as_id_e_type)j);
#endif
    }
  }
#endif


}

/*===========================================================================

FUNCTION cmsoa_write_soa_config_efs

DESCRIPTION
  This function write SOA configuration to EFS. It happens that sometimes
  as we power-down, we need to write to EFS. It also required to write back
  the SV-OPRT mode after we performed the switch. As we hit this function
  we guarantee that the arbitration is currenly enabled and activated.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsoa_write_soa_config_efs(
  cmsoa_efs_soa_s_type *soa_config,
  sys_modem_as_id_e_type asubs_id
)
{
  boolean ret = FALSE;
  int size = sizeof(cmsoa_efs_soa_s_type);

  if (soa_config == NULL)
  {
    CM_ERR_0("CMSOA: write soa_config ptr is null");
    return FALSE;
  }

  ret = cmefs_fs_write( CMEFS_SOA_CONFIG, (byte *)soa_config, size, asubs_id);
  CM_MSG_HIGH_2("CMSOA: Write back SOA config = %d, size=%d", ret, size );

  return ret;
}


/***************************************************************************
** FUNCTIONS - SECTION 3: Messages Processing / Core logic & algorithm
** This section contains function handles each messages/reports and commands.
***************************************************************************/


/*===========================================================================

FUNCTION cmsoa_msg_proc

DESCRIPTION
  This function is a generic function that process messages that sent to
  SOA module.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_msg_proc(
  const cm_generic_rpt_s_type *msg_ptr
)
{
  if (msg_ptr == NULL)
  {
    return;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Switch to process each reports */
  switch( msg_ptr->hdr.cmd )
  {
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    case CM_SOA_MSG_MCC:
      cmsoa_mcc_msg_process(msg_ptr);
      break;

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    case CM_SOA_REGPRX_SRV_REQ_PROCEED:
      cmsoa_regprx_srv_req_proceed_process(msg_ptr);
      break;

    case CM_ATTACH_COMPLETE_RPT:
      cmsoa_attach_complete_rpt_proc(FALSE, msg_ptr->hdr.asubs_id);
      break;

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    default:
      break;
  }
}

/*===========================================================================

FUNCTION cmsoa_mcc_msg_process

DESCRIPTION
  This function process mcc_msg. It performs prediction if the MCC belongs
  to SVLTE or CSFB country based on a configuration MCC lists.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_mcc_msg_process(
  const cm_generic_rpt_s_type *msg_ptr
)
{
  sd_mcc_rpt_s_type *mcc_msg = (sd_mcc_rpt_s_type *)&msg_ptr->generic_rpt.mcc_msg;

  cmsoa_state_sub_info_s_type *cmsoa_state;
  cmsoa_sv_oprt_mode_e_type new_sv_state = CMSOA_SV_OPRT_MODE_NONE;

  /* Get LTE_IMS_VOICE availability */
  cm_sds_ims_voice_avail_e_type lte_ims_voice_availability =
    CMSDS_IMS_VOICE_NONE;

  /* stack state */
  cmmsc_stack_state_e_type main_state = CMMSC_STACK_STATE_NONE;
  cmmsc_stack_state_e_type hybr2_state = CMMSC_STACK_STATE_NONE;

  boolean is_mcc_found = FALSE;

  uint32 mcc = mcc_msg->mcc;
  multimode_stack_e_type mcc_msg_stack_id = mcc_msg->stack_id;
  sys_modem_as_id_e_type mcc_msg_asubs_id = msg_ptr->hdr.asubs_id;

#ifdef FEATURE_DOMAIN_SELECTION
  lte_ims_voice_availability = cmsds_get_lte_ims_voice_availability(mcc_msg_asubs_id);
#endif
  cmsoa_state = cmsoa_state_sub_info_ptr(mcc_msg_asubs_id);
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_MSG_HIGH_1("CMSOA: mcc msg proc sub %d", mcc_msg_asubs_id);
  /* Gather Information & Error Checking */
  if (cmsoa_state == NULL)
  {
    CM_MSG_HIGH_0("CMSOA: sub info NULL");
    return;
  }

  if (!cmsoa_state->info.is_activated)
  {
    return ;
  }

  if (!cmph_is_subs_feature_mode_1x_sxlte(mcc_msg_asubs_id))
  {
    CM_MSG_HIGH_0("CMSOA: sub non svlte/srlte sub");
    return;
  }

  /* Print plmn info */
  if ( mcc_msg->mode == SYS_SYS_MODE_GWL)
  {
    CM_MSG_HIGH_3("CMSOA: MCC_MSG:    plmn 0x%x 0x%x 0x%x",
                  mcc_msg->plmn_id.identity[0],
                  mcc_msg->plmn_id.identity[1],
                  mcc_msg->plmn_id.identity[2]);

    cmsoa_state->info.last_gwl_plmn = mcc_msg->plmn_id;
  }

  main_state = cmmsimc_get_msc_stack_state(cmph_map_sub_stk_to_id(mcc_msg_asubs_id, MM_STACK_0));
  hybr2_state = cmmsimc_get_msc_stack_state(cmph_map_sub_stk_to_id(mcc_msg_asubs_id, MM_STACK_2));

  /* If state machine is not yet initialized - stop */
  if (main_state == CMMSC_STACK_STATE_NONE ||
      main_state == CMMSC_STACK_STATE_NULL)
  {
    CM_MSG_HIGH_0("CMSOA: MCC msg received in wrong state");
    return;
  }

  if (hybr2_state == CMMSC_STACK_STATE_NONE ||
      hybr2_state == CMMSC_STACK_STATE_NULL)
  {
    CM_MSG_HIGH_0("CMSOA: MCC msg received in wrong state");
    return;
  }

#if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
  /* while operating in SRLTE mode ,
          **if we move to an MCC which doesnt supports SRLTE ,
          **then we need to move to single stack mode */

  /* FR 1xsrlte - switch to CSFB ,
  ** since current PLMN doesnt supports SRLTE
  */
  if(cmph_is_subs_feature_mode_srlte(mcc_msg_asubs_id))
  {
    if(cmph_is_oprting_in_csfb_pref_srlte(mcc_msg_asubs_id) &&
        mcc_msg->stack_id == MM_STACK_2 &&
        mcc_msg->srv_status == SYS_SRV_STATUS_SRV)
    {
      boolean is_1xsrlte_plmn = FALSE;

      if(policyman_svd_possible_on_plmn_per_subs(&(mcc_msg->plmn_id), FALSE,
          &is_1xsrlte_plmn, mcc_msg_asubs_id) ==
          POLICYMAN_STATUS_SUCCESS)
      {
        CM_MSG_HIGH_4("SRLTE: MCC_MSG: is_1xsrlte_plmn=%d, stack=%d, curr_sub_mode=%d srv_status=%d",
                      is_1xsrlte_plmn,
                      mcc_msg->stack_id,
                      cmsoa_state->info.curr_sv_oprt_mode,
                      mcc_msg->srv_status);


        if(is_1xsrlte_plmn == FALSE)
        {
          /* reset the ping pong timer ,
          ** since it is not applicable when PLMN is not in white list
          */
          cmsoa_arm_timer(CMSOA_SV_OPRT_MODE_NONE,
                          CMSOA_SV_SWITCH_DELAY_REAS_NONE,
                          mcc_msg_asubs_id);

          cmsoa_sv_switch_base(CMSOA_SV_OPRT_MODE_CSFB,
                               mcc_msg_asubs_id,
                               FALSE);
        }
      }
      else
      {
        CM_ERR_0("SRLTE: fail to check policyman plmn !!!");
      }
    }
    return;
  }
#endif

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Conditions not to perform prediction */


  /* If prediction not allowed */
  if (!cmsoa_is_prediction_allowed( mcc_msg->stack_id, mcc_msg_asubs_id))
  {
    CM_MSG_HIGH_0("CMSOA: Skipping prediction");
    return;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Decision */

  /* Check if MCC belongs to the SVLTE-MCC list */
  is_mcc_found = cmsoa_svlte_mcc_list_search(mcc, mcc_msg_asubs_id);

  if( is_mcc_found )
  {
    cm_mm_ss_info_gen_stack_info_s_type *ss_info_stack_0_ptr = cmss_get_info_stk_ptr(mcc_msg_asubs_id, MM_STACK_0);
    /* If we have already did prediction that we were in SVLTE and now we're
    ** in CSFB because of complete plmn does not belongs to SVLTE PLMN list.
    ** In this case, we don't need to perform prediction because we already
    ** performed it before. This often happens in the transition state, moving
    ** LTE from HYBR-2 stack to MAIN after SVLTE -> CSFB switch.
    */
    if(cmsoa_state->info.is_svlte_predicted)
    {
      CM_MSG_HIGH_0("CMSOA: already performed prediction");
      return;
    }

    /* If current LTE system has lte_support voice, stay with CSFB
    ** This is the scenario where we're in a country that supports SVLTE,
    ** However, we're acquiring LTE system which supports voice. There
    ** no need for SVLTE here. So although we keep detecting MCC supporting
    ** SVLTE, we should ignore them and sticks with CSFB.
    */
    if(lte_ims_voice_availability == CMSDS_IMS_VOICE_AVAIL &&
        ss_info_stack_0_ptr != NULL &&
        ss_info_stack_0_ptr->srv_status == SYS_SRV_STATUS_SRV &&
        ss_info_stack_0_ptr->sys_mode == SYS_SYS_MODE_LTE )
    {
      CM_MSG_HIGH_0("CMSOA: mcc-svlte but LTE supports voice");
      new_sv_state = CMSOA_SV_OPRT_MODE_CSFB;
    }
    else
    {
      new_sv_state = CMSOA_SV_OPRT_MODE_SVLTE;
    }

  }
  else
  {
    new_sv_state = CMSOA_SV_OPRT_MODE_CSFB;
  }

  CM_MSG_HIGH_3("CMSOA: mcc_msg: mcc=%d, mcc_found=%d, new_sv_state =%d",
                mcc, is_mcc_found, new_sv_state);

  cmsoa_sv_switch_base(new_sv_state, mcc_msg_asubs_id,FALSE);
}

#ifdef FEATURE_DOMAIN_SELECTION
/*===========================================================================

FUNCTION cmsoa_process_domain_selection_srv_ind

DESCRIPTION
  This function is called to process new domain selection information
  after we have received new service indications.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_process_domain_selection_srv_ind (

  const sd_si_info_s_type   *si_info_ptr,
  /* Pointer to buffer containing new service info information */

  sys_modem_as_id_e_type     asubs_id
)
{
  cmsoa_sv_oprt_mode_e_type new_sv_state = CMSOA_SV_OPRT_MODE_NONE;
  boolean is_lte_voice_support = FALSE;

  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);

  /* Get CSFB capability and LTE_IMS_VOICE availability */
  cm_call_csfb_e_type csfb_type = cmsds_get_csfb_type_allowed(asubs_id);
  cm_sds_ims_voice_avail_e_type lte_ims_voice_availability =
    cmsds_get_lte_ims_voice_availability(asubs_id);
  cmph_s_type                   *ph_ptr = cmph_ptr();

  if (si_info_ptr == NULL || cmsoa_state == NULL)
  {
    CM_ERR_0("CMSOA: si_info_ptr or sub_info == NULL");
    return; //wrong usage don't cause err fatal
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* return if feature is not activated */
  if (!cmsoa_state->info.is_activated)
  {
    CM_MSG_HIGH_0("CMSOA: Arbitration is not activated");
    return;
  }

  if (!cmsoa_is_stack_state_valid(asubs_id)|| !IS_VALID_PH_SUB_PTR(asubs_id))
    return;

  CM_MSG_HIGH_3("CMSOA: Process Domain Selection, csfb_type=%d, ims=%d, volte-12=%d",
                csfb_type, lte_ims_voice_availability, cmsoa_state->config.is_volte12);

  /* We only process this service indication when we acquire LTE full service
  */
  if (!(si_info_ptr->srv_status == SYS_SRV_STATUS_SRV))
  {
    CM_MSG_HIGH_2("CMSOA: not full service. bail out (mode=%d, srv_status=%d)",
                  si_info_ptr->mode, si_info_ptr->srv_status);
    return;
  }

  if (ph_ptr->sub_info[asubs_id]->pref_info.network_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_MANUAL)
  {
    CM_MSG_HIGH_0("Skip prediction in MANUAL mode since device will be 3GPP only device");
    return;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if(si_info_ptr->mode == SD_MODE_LTE)
  {

    /* If we support VOICE or we fall to CSFB - new state is CSFB */
    if (csfb_type ==   CM_CALL_CSFB_TYPE_1XCSFB ||
        csfb_type == CM_CALL_CSFB_TYPE_PPCSFB)
    {
      is_lte_voice_support = TRUE;
      new_sv_state = CMSOA_SV_OPRT_MODE_CSFB;
      cmsoa_state->info.is_svlte_predicted = TRUE;
    }
    /* If we support IMS/VoLTE (full VoLTE, not VoLTE-2012) then */
    else if (lte_ims_voice_availability == CMSDS_IMS_VOICE_AVAIL &&
             cmsoa_state->config.is_volte12 == FALSE)
    {
      is_lte_voice_support = TRUE;
      new_sv_state = CMSOA_SV_OPRT_MODE_CSFB;
      cmsoa_state->info.is_svlte_predicted = TRUE;
    }
    else
    {
      is_lte_voice_support = FALSE;
      new_sv_state = CMSOA_SV_OPRT_MODE_SVLTE;
    }

    /*
    **   Requirement: If UE has attached on LTE in SV mode and it is learnt that
    **      LTE does not support voice (no VoPS, no SIB-8, no full CS+PS)
    **
    **   a. If LTE PLMN-ID is in SV-LTE PLMN-ID list, UE shall camp on
    **      LTE in SV-LTE mode (ignore domain selection flag)
    **
    **   b. If LTE PLMN-ID is not listed in SV-LTE PLMN-ID list,
    **      UE shall follow domain selection behavior
    **
    **      For e.g:  If UE = voice-centric, Detach from LTE and disable LTE PLMN for X minutes
    **
    **   Requirement: If LTE supports voice, switch to CSFB
    */
    if (is_lte_voice_support == FALSE)
    {
      boolean plmn_found = cmsoa_is_last_plmn_in_svlte_plmn_list(asubs_id);

      CM_MSG_HIGH_1("CMSOA: LTE not support voice. PLMN found=%d",plmn_found);

      if (plmn_found)
      {
        /* New state to be SVLTE regardless of what MCC. Explained below:
        **  1) If we're in Korea (MCC says CSFB),
        **     and this plmn is supporting SVLTE, so switch to SVLTE.
        **
        **  2) If we're in U.S. (MCC says SVLTE), prediction should yields
        **     SVLTE already. This is like a protection.
        **
        */
        new_sv_state = CMSOA_SV_OPRT_MODE_SVLTE;
      }
      else
      {
        /* Even though we're in SVLTE MCC area, the PLMN which we camped
        ** did not support SVLTE, so we need to switch to CSFB
        */
        new_sv_state = CMSOA_SV_OPRT_MODE_CSFB;

        /* If current mode is in SVLTE (MCC indicates SVLTE)
        ** and now we're switching to CSFB
        */
        if (cmsoa_state->info.curr_sv_oprt_mode == CMSOA_SV_OPRT_MODE_SVLTE)
        {
          cmsoa_state->info.is_svlte_predicted = TRUE;
        }
      }
    }
    /* Requirement: If LTE supports voice, switch to CSFB */
    else
    {
      new_sv_state = CMSOA_SV_OPRT_MODE_CSFB;
    }
  }
  else if(si_info_ptr->mode == SD_MODE_GSM || si_info_ptr->mode == SD_MODE_WCDMA)
  {
    /* if current mode is SVLTE, then switch back to CSFB mode, as when GW service is camped onto,
    LTE will be scanned only in CSFB mode. So no point of staying in SV mode */
    if(cmsoa_state->info.curr_sv_oprt_mode == CMSOA_SV_OPRT_MODE_SVLTE)
    {
      new_sv_state = CMSOA_SV_OPRT_MODE_CSFB;
      cmsoa_state->info.is_svlte_predicted = TRUE;
    }
  }

  /* Call switch base to handle the switch if we have decided what is the SV-oprt */
  cmsoa_sv_switch_base(new_sv_state, asubs_id,FALSE);

}
#endif

/*===========================================================================

FUNCTION cmsoa_regprx_srv_req_proceed_process

DESCRIPTION
  This function process the service-request from CMREGPRX - it seemly check
  if the current SV-OPRT is the same at the SV-OPRT at the moment of the
  request.
  a) If it the same - allow service request to go through.
  b) If it's not the same - meaning we have decided to switch SV-OPRT in the
     meanwhile, hence, block the service request.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_regprx_srv_req_proceed_process(
  const cm_generic_rpt_s_type *msg_ptr
)
{
  boolean is_srv_req_proceed = TRUE;
  cm_generic_rpt_u_type *regprx_msg = (cm_generic_rpt_u_type *)&msg_ptr->generic_rpt;
  mm_sub_stk_id_s_type mm_id = cmph_map_sub_stk_to_id(msg_ptr->hdr.asubs_id,
                               regprx_msg->regprx_srv_req.stk_id);

  /* Check if we're not transition the SVLTE <-> CSFB state, then allow the
  ** srv_req
  */
  if (cmss_is_stack2_operational(msg_ptr->hdr.asubs_id) == regprx_msg->regprx_srv_req.hyrb2_on)
  {
    CM_MSG_HIGH_1("CMSOA: Allow Service Request - req_svlte_state=%d",
                  regprx_msg->regprx_srv_req.hyrb2_on);
    is_srv_req_proceed = TRUE;
  }
  else
  {
    CM_MSG_HIGH_1("CMSOA: Block Service Request - req_svlte_state=%d",
                  regprx_msg->regprx_srv_req.hyrb2_on);
    is_srv_req_proceed = FALSE;
  }

  /* Send this messages to CMREGPRX so it could proceed with service request */
  cmregprx_cmd_srv_req_proceed(is_srv_req_proceed, mm_id);

}


/***************************************************************************
** FUNCTIONS - SECTION 4: Utilities function
** This section contains ultilities functions such as get/set, timer, srch.
***************************************************************************/

/*===========================================================================

FUNCTION cmsoa_timer_proc()

DESCRIPTION
  This function process the timer event for CMSOA. Timer is used when we want
  to delay a switch due to either Voice Call or Ping-pong situation.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_timer_proc()
{
  cmsoa_state_sub_info_s_type     *cmsoa_state;
  dword                      uptime  = time_get_uptime_secs();
  uint8 i;
  cmsds_sub_prop_s_type     *sds_sub_prop_ptr;

  for(i=0; i<cmsoa_state_info_ptr()->count; i++)
  {
    cmsoa_state = cmsoa_state_sub_info_ptr((sys_modem_as_id_e_type)i);
    sds_sub_prop_ptr = cmsds_get_sub_prop_ptr((sys_modem_as_id_e_type)i);
    if(cmsoa_state == NULL || sds_sub_prop_ptr == NULL || cmmsc_get_1x_sub() != (sys_modem_as_id_e_type)i)
    {
      continue;
    }

    if (!cmsoa_state->info.is_activated)
    {
      continue;
    }

    if(cmsoa_state->info.switch_delay_reason == CMSOA_SV_SWITCH_DELAY_VOLTE_ON_HYBR &&
        cmsoa_state->info.switch_delay_state == CMSOA_SV_OPRT_MODE_CSFB)
    {
      cmsoa_process_delay_action( cmsoa_state->info.switch_delay_state,(sys_modem_as_id_e_type)i);
    }

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* If the timer expires */
    if (cmsoa_state->info.switch_delay_timer != 0 &&
        uptime >= cmsoa_state->info.switch_delay_timer &&
        cmsoa_state->info.switch_delay_state != CMSOA_SV_OPRT_MODE_NONE)
    {

#if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
      if (cmph_is_oprting_in_csfb_pref_srlte((sys_modem_as_id_e_type)i) == FALSE)
      {
        cmsoa_process_delay_action( cmsoa_state->info.switch_delay_state, (sys_modem_as_id_e_type)i);
      }
      else
#endif
        if (cmsoa_state->info.switch_delay_state == CMSOA_SV_OPRT_MODE_CSFB)
        {
#if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
          mm_sub_stk_id_s_type stk2_mm_id = cmph_map_sub_stk_to_id(i, MM_STACK_2);
          sd_si_info_s_type *ss_true_stk_2_info_ptr = cmss_true_stack_info_ptr(stk2_mm_id);
          /* only process delay action if there is LTE service on hybr2 and no RLF */
          if ( (ss_true_stk_2_info_ptr != NULL &&
                ss_true_stk_2_info_ptr->srv_status == SYS_SRV_STATUS_SRV &&
                cmregprx_get_conn_mode_status(stk2_mm_id, SYS_SYS_MODE_LTE) == FALSE &&
                ss_true_stk_2_info_ptr->mode == SD_MODE_LTE) &&
               (sds_sub_prop_ptr->gpp2_sub_info.csfb_status == CM_1XCSFB_PREREG_SUCCESS ||
                sds_sub_prop_ptr->gpp2_sub_info.csfb_status == CM_1XCSFB_AVAIL ||
                sds_sub_prop_ptr->gen_sub_info.ims_sub_info.ims_reg_status_voice[SYS_SYS_MODE_LTE] == SYS_IMS_REG_STATE_REGISTERED ||
                cmsoa_check_is_1xsrlte_plmn(cmsoa_state->info.last_gwl_plmn, (sys_modem_as_id_e_type)i) == FALSE))
          {
            cmsoa_process_delay_action( cmsoa_state->info.switch_delay_state, (sys_modem_as_id_e_type)i);
          }
          else
          {
            /* reset the switch delay state */
            CM_MSG_HIGH_2("cmsoa_timer_proc: resetting switch delay sub %d status %d",
                          i,
                          sds_sub_prop_ptr->gpp2_sub_info.csfb_status);
            cmsoa_state->info.switch_delay_state = CMSOA_SV_OPRT_MODE_NONE;

            /* reset CSFB status */
            if(sds_sub_prop_ptr->gpp2_sub_info.csfb_status != CM_1XCSFB_NONE)
            {
              CM_MSG_HIGH_1("cmsoa_timer_proc: resetting csfb_status. Current status: %d",
                            sds_sub_prop_ptr->gpp2_sub_info.csfb_status);
              sds_sub_prop_ptr->gpp2_sub_info.csfb_status = CM_1XCSFB_NONE;
            }
          }
#endif
        }
        else if (cmsoa_state->info.switch_delay_state == CMSOA_SV_OPRT_MODE_SVLTE)
        {
          boolean is_1xsrlte_mcc = FALSE;
          cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop((sys_modem_as_id_e_type)i);
          cm_mm_ss_info_gen_stack_info_s_type *ss_info_all_stack_ptr;
          if(ss_info_sub_prop_ptr == NULL ||
              ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info == NULL)
          {
            return;
          }
          ss_info_all_stack_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info;
          CM_MSG_HIGH_3("CMSOA: TIME_PROC: plmn 0x%x 0x%x 0x%x",
                        ss_info_all_stack_ptr->sys_id.id.plmn.identity[0],
                        ss_info_all_stack_ptr->sys_id.id.plmn.identity[1],
                        ss_info_all_stack_ptr->sys_id.id.plmn.identity[2]);

          if(policyman_svd_possible_on_plmn_per_subs
              (&ss_info_all_stack_ptr->sys_id.id.plmn, FALSE, &is_1xsrlte_mcc, i)
              == POLICYMAN_STATUS_SUCCESS)
          {
            CM_MSG_HIGH_3("SRLTE: TIME_PROC: is_1xsrlte_plmn=%d, delay_state=%d, curr_sub_mode=%d",
                          is_1xsrlte_mcc,
                          cmsoa_state->info.switch_delay_state,
                          cmsoa_state->info.curr_sv_oprt_mode);

            if(is_1xsrlte_mcc == TRUE)
            {
              cmsoa_process_delay_action( cmsoa_state->info.switch_delay_state, (sys_modem_as_id_e_type)i);
            }
            else
            {
              /* reset all timers */
              cmsoa_arm_timer(CMSOA_SV_OPRT_MODE_NONE,
                              CMSOA_SV_SWITCH_DELAY_REAS_NONE,
                              (sys_modem_as_id_e_type)i);
            }
          }
          else
          {
            CM_ERR_0("SRLTE: fail to check policyman plmn !!!");
          }
        }
        else
        {
          /* reset all timers */
          cmsoa_arm_timer(CMSOA_SV_OPRT_MODE_NONE,
                          CMSOA_SV_SWITCH_DELAY_REAS_NONE,
                          (sys_modem_as_id_e_type)i);
        }

    }

    /* check if we are in incosistent state where although as per capabilities we should be
    ** in dual stack but current sv state says SVLTE and LTE stack is not operational */
    if (cmsoa_is_srlte_in_inconsistent_state((sys_modem_as_id_e_type)i))
    {
      cmsoa_err_fatal_wrong_sv_state((sys_modem_as_id_e_type)i);
    }
  }
}

/*===========================================================================

FUNCTION cmsoa_process_delay_action()

DESCRIPTION
  This function process the delayed action due to either Voice Call or
  Ping-pong situation.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_process_delay_action(
  cmsoa_sv_oprt_mode_e_type   state,
  sys_modem_as_id_e_type     asubs_id
)
{
  cmsoa_sv_switch_delay_reas_e_type delay_reas = CMSOA_SV_SWITCH_DELAY_REAS_NONE;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Basic checks */
  CM_MSG_HIGH_1("CMSOA: Process Delayed State =%d",state);

  if (state == CMSOA_SV_OPRT_MODE_NONE)
  {
    CM_MSG_HIGH_0("CMSOA: No action taken!");
    return;
  }

  /* As timer expires, we check if we need to continue delaying.
  ** If yes, arm_timer again and exit.
  */
  delay_reas = cmsoa_sv_switch_is_delayed(asubs_id);

  /* if need to delay, arm the timer again */
  if (delay_reas == CMSOA_SV_SWITCH_DELAY_REAS_NONE)
  {
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Get here: timer expired and action will be taken */
    switch (state)
    {
      case CMSOA_SV_OPRT_MODE_SVLTE:
      {
        /* Switching from CSFB --> SVLTE */
        cmsoa_sv_switch_csfb_svlte(asubs_id);
        break;
      }

      case CMSOA_SV_OPRT_MODE_CSFB:
      {
        /* Switching from SVLTE --> CSFB */
        cmsoa_sv_switch_svlte_csfb(asubs_id);
        break;
      }

      default:
      {
        CM_MSG_HIGH_0("CMSOA: invalid action");
      }
    }

  }
  /*no need to do anything for CMSOA_SV_SWITCH_DELAY_REAS_PINGPONG ,
      timer is already running for that */
  else if(delay_reas != CMSOA_SV_SWITCH_DELAY_REAS_PINGPONG)
  {
    cmsoa_arm_timer(state,delay_reas,asubs_id);
  }
  return;
}

/*===========================================================================

FUNCTION cmsoa_arm_timer()

DESCRIPTION
  This function set the timer for delay switch due to voice call.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_arm_timer(
  cmsoa_sv_oprt_mode_e_type          state,
  cmsoa_sv_switch_delay_reas_e_type  switch_delay_reason,
  sys_modem_as_id_e_type             asubs_id
)
{
  dword                     uptime      = time_get_uptime_secs();
  cmsoa_state_sub_info_s_type    *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);

  if(cmsoa_state == NULL)
  {
    return;
  }
  switch( switch_delay_reason )
  {
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    case CMSOA_SV_SWITCH_DELAY_REAS_NONE:
      /* Clear attach complete timer as well since switch_delay_reas is
      ** being set to NONE
      */
      cmtask_attach_complete_timer_clear(asubs_id);
      cmsoa_state->info.switch_delay_timer = 0;
      break;

    case CMSOA_SV_SWITCH_DELAY_VOLTE_ON_HYBR:
      cmsoa_state->info.switch_delay_timer = 0;
      break;

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    case CMSOA_SV_SWITCH_DELAY_REAS_VOICE_CALL:
      cmsoa_state->info.switch_delay_timer = uptime +
                                             CMSOA_SWITCH_DELAY_REASON_VOICE_CALL_TIMER;
      break;

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    case CMSOA_SV_SWITCH_DELAY_REAS_PINGPONG:
      if(cmph_get_feature_mode() == SYS_OVERALL_FEATURE_MODE_SVLTE)
      {
        cmsoa_state->info.switch_delay_timer = uptime +
                                               CMSOA_SWITCH_DELAY_REASON_PINGPONG_TIMER;
      }
      else
      {
        cmsoa_state->info.switch_delay_timer = uptime +
                                               cmsoa_state->config.sxlte_timers.switch_pingpong_guard_timer;
      }
      break;

#if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    case CMSOA_SV_SWITCH_DELAY_LTE_CONNECTED:
      if(cmph_is_oprting_in_csfb_pref_srlte(asubs_id))
      {
        cmsoa_state->info.switch_delay_timer = uptime +
                                               cmsoa_state->config.sxlte_timers.lte_idle_timer;
      }
      break;

    case CMSOA_SV_SWITCH_DELAY_ATTACH_COMPLETE:
      cmtask_attach_complete_timer_start(cmsoa_state->config.attach_complete_timer, asubs_id);
      break;
#endif
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    default:
      CM_MSG_HIGH_1("CMSOA: Unknown delay reason %d",switch_delay_reason);
      break;
  }

  if(switch_delay_reason == CMSOA_SV_SWITCH_DELAY_REAS_NONE
      && state != CMSOA_SV_OPRT_MODE_NONE)
  {
    CM_MSG_HIGH_1("CMSOA: unexpected action=%d",state);
    cmsoa_state->info.switch_delay_state = CMSOA_SV_OPRT_MODE_NONE;
  }
  else
  {
    cmsoa_state->info.switch_delay_state = state;
  }

  cmsoa_state->info.switch_delay_reason = switch_delay_reason;


  CM_MSG_HIGH_6("CMSOA: BUFFER switch, Set delay time: switch_delay_reason=%d, current time=%d, timer =%d, current state=%d, state=%d, sub=%d",
                switch_delay_reason, uptime, cmsoa_state->info.switch_delay_timer, 
                cmsoa_state->info.curr_sv_oprt_mode, state,
                asubs_id);
  return;
}

/*===========================================================================

FUNCTION cmsoa_attach_complete_rpt_proc()

DESCRIPTION
  This function performs the DELAYED SWITCH due to ATTACH COMPLETE IND.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmsoa_attach_complete_rpt_proc(
  boolean                    is_timer_exp,
  sys_modem_as_id_e_type     asubs_id
)
{
  cmsoa_state_sub_info_s_type    *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  sd_si_info_s_type            *si_info_ptr = cmss_true_stack_info_ptr(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0));

  if(cmsoa_state == NULL || si_info_ptr == NULL)
  {
    return;
  }
  if (cmph_is_oprting_in_csfb_pref_csfb_sub_mode(asubs_id) &&
      (cmsoa_state->info.switch_delay_state == CMSOA_SV_OPRT_MODE_SVLTE) &&
      (cmsoa_state->info.switch_delay_reason == CMSOA_SV_SWITCH_DELAY_ATTACH_COMPLETE))
  {
    CM_MSG_HIGH_4("ATTACH: Perform the delayed SR switch, sv_oprt_mode %d %d, enabled %d, sub %d",
                     cmsoa_state->info.curr_sv_oprt_mode,
                     cmsoa_state->config.curr_sv_oprt_mode,
                     cmsoa_state->config.is_switch_enabled,
                     asubs_id);

    if(si_info_ptr->extend_srv_info == SYS_EXTEND_SRV_INFO_REG_COMP_ACK_PENDING)
    {
      si_info_ptr->extend_srv_info = SYS_EXTEND_SRV_INFO_NONE;

      if (!is_timer_exp)
      {
        cmtask_attach_complete_timer_clear(asubs_id);
      }
      else
      {
        cmsoa_state->info.is_attach_complete_timer_active = FALSE;
      }
    }
    /* Dont switch to SRLTE ,
    ** while waiting for ESR response for non-LTE call .
    ** Since switch will cause ESR response to be wrongly mapped
    ** Anyways, UE will switch at end of call
    */
    if (cmcall_is_there_hvolte_call_waiting_for_esr_rsp() != CM_CALL_ID_INVALID )
    {
      CM_MSG_HIGH_0("Hvolte:Clear delay action and postponed switch to call end");
      cmsoa_arm_timer(CMSOA_SV_OPRT_MODE_NONE,
                      CMSOA_SV_SWITCH_DELAY_REAS_NONE,
                      asubs_id);
    }
    else
    {
      cmsoa_process_delay_action(cmsoa_state->info.switch_delay_state, asubs_id);
    }
  }
  else if(si_info_ptr->extend_srv_info == SYS_EXTEND_SRV_INFO_REG_COMP_ACK_PENDING)
  {
    CM_MSG_MED_0("Reset extend_srv_info");
    si_info_ptr->extend_srv_info = SYS_EXTEND_SRV_INFO_NONE;
  }

}

/*===========================================================================

FUNCTION cmsoa_is_prediction_allowed_in_srv_status()

DESCRIPTION
  self descriptive.

DEPENDENCIES
  none

RETURN VALUE
  true if prediction is allowed
  false otherwise

SIDE EFFECTS
  none

===========================================================================*/
static boolean cmsoa_is_prediction_allowed_in_srv_status(sys_srv_status_e_type srv_status)
{
  switch(srv_status)
  {
    case SYS_SRV_STATUS_NO_SRV:
    case SYS_SRV_STATUS_NO_SRV_INTERNAL:
    case SYS_SRV_STATUS_PWR_SAVE:
    case SYS_SRV_STATUS_PWR_SAVE_INTERNAL:
      return TRUE;
    default:
      return FALSE;
  }
}
/*===========================================================================

FUNCTION cmsoa_is_prediction_allowed()

DESCRIPTION
  This function checks if we allow prediction. We don't allow prediction
  when:
  a) feature not enabled
  b) states not initialized
  c) we cannot move to other mode, for example, no subscriptions means UE
     cannot move to SVLTE. So legacy code will take care.
  d) we have already done the prediction and currently having service.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static boolean cmsoa_is_prediction_allowed(
  multimode_stack_e_type   stack_id,
  sys_modem_as_id_e_type   asubs_id
)
{

  sys_srv_status_e_type          main_srv_status;
  sys_srv_status_e_type          hybr2_srv_status;
  boolean                        is_main_allow_prediction = FALSE;
  boolean                        is_hybr2_allow_prediction = FALSE;
  cmph_s_type                   *ph_ptr = cmph_ptr();
  mm_sub_stk_id_s_type           stk0_mm_id = cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0);
  mm_sub_stk_id_s_type           stk2_mm_id = cmph_map_sub_stk_to_id(asubs_id, MM_STACK_2);

  /* stack state */
  cmmsc_stack_state_e_type main_state = cmmsimc_get_msc_stack_state(stk0_mm_id);
  cmmsc_stack_state_e_type hybr2_state = cmmsimc_get_msc_stack_state(stk2_mm_id);
  sd_si_info_s_type             *ss_true_stk_0_info_ptr, *ss_true_stk_2_info_ptr;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If prediction happens on HYBR-2 stack while HYBR-2 is not operational,
  ** Skip this prediction.
  */
  if ((!cmss_is_stack2_operational(asubs_id) && stack_id == MM_STACK_2)||
      !IS_VALID_PH_SUB_PTR(asubs_id))
  {
    return FALSE;
  }

  if (ph_ptr->sub_info[asubs_id]->pref_info.network_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_MANUAL)
  {
    CM_MSG_HIGH_0("Skip prediction in MANUAL mode since device will be 3GPP only device");
    return FALSE;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If MCC is found after we have got service, we should not act upon it
  ** since we only use it for prediction before having service (in OOS)
  */
  ss_true_stk_0_info_ptr = cmss_true_stack_info_ptr(stk0_mm_id);
  if(ss_true_stk_0_info_ptr == NULL)
  {
    return FALSE;
  }
  main_srv_status = ss_true_stk_0_info_ptr->srv_status;

  CM_MSG_HIGH_2("MMSOA: check prediction: main_srv=%d, main stack_state=%d",
                main_srv_status, main_state);

  /* Check main stack first -*/
  if ( cmsoa_is_prediction_allowed_in_srv_status(main_srv_status))
  {
    is_main_allow_prediction = TRUE;
  }

  /* In case main stack acquire 3GPP service first, then we need to also
  ** consider the pref_state machine
  */
  if ( main_srv_status == SYS_SRV_STATUS_LIMITED &&
       /* could possibly contains more state than below listed states */
       (!cmmsc_is_msc_multi_stacks(asubs_id) ||
        !cmmsimc_is_in_srv(stk0_mm_id))
     )
  {
    is_main_allow_prediction = TRUE;
  }

  /* If main does not allow prediction, then we don't need to continue to check
  ** for hybr-2. Just return here.
  */
  if (is_main_allow_prediction == FALSE ||
      cmss_is_stack2_operational(asubs_id) == FALSE)
  {
    return is_main_allow_prediction;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ss_true_stk_2_info_ptr = cmss_true_stack_info_ptr(stk2_mm_id);
  /* Check for hybr-2 */
  if(ss_true_stk_2_info_ptr == NULL)
  {
    return FALSE;
  }

  hybr2_srv_status = ss_true_stk_2_info_ptr->srv_status;

  CM_MSG_HIGH_2("CMSOA: checking prediction hybr2_srv=%d, state=%d",
                hybr2_srv_status, hybr2_state);

  if ( cmsoa_is_prediction_allowed_in_srv_status(hybr2_srv_status))
  {
    is_hybr2_allow_prediction = TRUE;
  }

  /* If we have LIMITED service, and state indicating that we
  ** not yet having service.
  */
  if ( hybr2_srv_status == SYS_SRV_STATUS_LIMITED &&
       hybr2_state == CMMSC_STACK_STATE_ACTIVE)
  {
    is_hybr2_allow_prediction = TRUE;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Return */

  /* Allow prediction to happens if both stacks are ok with it */
  if (is_main_allow_prediction &&
      is_hybr2_allow_prediction)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION cmsoa_get_current_sv_oprt_state()

DESCRIPTION
  This function return current sv_oprt state variable.

RETURN VALUE
  cmsoa_sv_oprt_mode_e_type

===========================================================================*/
cmsoa_sv_oprt_mode_e_type cmsoa_get_current_sv_oprt_state(
  sys_modem_as_id_e_type        asubs_id
)
{
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);

  if (cmsoa_state == NULL)
  {
    CM_MSG_HIGH_1("CMSOA: sub info[%d] is NULL", asubs_id);
    return CMSOA_SV_OPRT_MODE_NONE;
  }

  return cmsoa_state->info.curr_sv_oprt_mode;
}

/*===========================================================================
FUNCTION  cmsoa_is_oprting_in_csfb_pref_srlte

DESCRIPTION
Returns TRUE if UE is operating in 1XSRLTE-Dual mode .



DEPENDENCIES
 cmmsc_init should already be called


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsoa_is_oprting_in_csfb_pref_srlte(
  sys_modem_as_id_e_type asubs_id
)
{
  cmmsc_state_machine_s_type *state_machine = cmmsimc_state_machine_ptr(asubs_id);
  boolean ret = FALSE;

  if(cmph_is_subs_feature_mode_1x_sxlte(asubs_id) &&
      state_machine->stack_common_info.ue_mode == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED &&
      cmsoa_get_current_sv_oprt_state(asubs_id) == CMSOA_SV_OPRT_MODE_SVLTE)
  {
    ret = TRUE;
  }

  return ret;
}

/*===========================================================================

FUNCTION cmsoa_get_pref_update_reason()

DESCRIPTION
  This function returns pref_update_reason variable

RETURN VALUE
  sd_ss_pref_update_reason_e_type

===========================================================================*/
sd_ss_pref_update_reason_e_type cmsoa_get_pref_update_reason(
  sys_modem_as_id_e_type           asubs_id
)
{
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);

  if (cmsoa_state == NULL)
  {
    return SD_SS_PREF_UPDATE_REASON_NONE;
  }

  return cmsoa_state->info.pref_update_reason;
}

/*===========================================================================

FUNCTION cmsoa_set_pref_update_reason()

DESCRIPTION
  This function set pref_update_reason variable

RETURN VALUE
  none

===========================================================================*/
void cmsoa_set_pref_update_reason
(
  sd_ss_pref_update_reason_e_type  reason,
  sys_modem_as_id_e_type           asubs_id
)
{
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);

  if (cmsoa_state == NULL)
  {
    return;
  }

  cmsoa_state->info.pref_update_reason = reason;
  return;
}

/*===========================================================================

FUNCTION cmsoa_is_last_plmn_in_svlte_plmn_list()

DESCRIPTION
  This function check if the last plmn which we have acquired belongs to the
  SVLTE-PLMN-ID list. It often used when we received service indication,
  we want to check if the FULL plmn is in the lists or not.

  Use case:
  1) Power-up and acquire MCC in SVLTE MCC list
  2) Acquire full-plmn that does not belongs to SVLTE-PLMN-ID lists.
  3) Decision: Switch() to CSFB even though we're in MCC belongs to SVLTE-MCC.

  Reason: It's possible that this plmn has not been fully tested with
  SVLTE capabailities.

RETURN VALUE
  none

===========================================================================*/
boolean cmsoa_is_last_plmn_in_svlte_plmn_list(
  sys_modem_as_id_e_type     asubs_id
)
{
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);

  if(cmsoa_state == NULL)
  {
    return FALSE;
  }
  return cmsoa_svlte_plmn_list_search(&cmsoa_state->config.svlte_plmn_list,
                                      &cmsoa_state->info.last_gwl_plmn);
}

/*===========================================================================

FUNCTION cmsoa_activate_arbitration()

DESCRIPTION
  This function checks and activate the SV-Operation Arbitration module.

RETURN VALUE
  none

===========================================================================*/
void cmsoa_activate_arbitration(
  sys_modem_as_id_e_type asubs_id
)
{

  cmsoa_state_sub_info_s_type   *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);

  cmph_s_type             *ph_ptr      = cmph_ptr();

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if(cmsoa_state == NULL|| !IS_VALID_PH_SUB_PTR(asubs_id))
  {
    return;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If feature mode is not svlte, then don't activate */
  if (!cmph_is_subs_feature_mode_1x_sxlte(asubs_id))
  {
    CM_MSG_HIGH_1("CMSOA: sub %d, no svlte - deactivate", asubs_id);
    cmsoa_state->info.is_activated = FALSE;
    return;
  }

  /* If switch is disabled, then don't activate */
  if (!cmsoa_state->config.is_switch_enabled)
  {
    CM_MSG_HIGH_1("CMSOA: sub %d, Switch is not enabled - deactivate", asubs_id);
    cmsoa_state->info.is_activated = FALSE;
    return;
  }

  /* If we are not ONLINE - then don't activate arbitration */
  if ( ph_ptr->device_prop.ph_oprt_mode.oprt_mode != SYS_OPRT_MODE_ONLINE )
  {
    CM_MSG_HIGH_0("CMSOA: Phone not online - deactivate");
    cmsoa_state->info.is_activated = FALSE;
    return;
  }

  /* If we don't have 1X / GW subscription, then MMSS will not be enabled
  ** Hence we will stay in NON-SVLTE anyways. So don't activate the
  ** feature.
  */
  if (!( ph_ptr->sub_info[asubs_id]->is_3gpp2_subs_avail &&
         ph_ptr->sub_info[asubs_id]->is_3gpp_subs_avail ))
  {
    CM_MSG_HIGH_1("CMSOA: sub %d, SVLTE operation is not possible - deactivate", asubs_id);
    cmsoa_state->info.is_activated = FALSE;
    return;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If all the checks passed, activate arbitration */
  CM_MSG_HIGH_1("CMSOA: sub %d, Activate SV-Operation Arbitration", asubs_id);
  cmsoa_state->info.is_activated = TRUE;

}

/*===========================================================================

FUNCTION cmsoa_is_arbitration_activate()

DESCRIPTION
  This function returns if arbitration is activated.

RETURN VALUE
  none

===========================================================================*/
boolean cmsoa_is_arbitration_activate(
  sys_modem_as_id_e_type          asubs_id
)
{
  cmsoa_state_sub_info_s_type   *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);

  if (cmsoa_state == NULL)
  {
    return FALSE;
  }

  return (cmsoa_state->info.is_activated);
}

/*===========================================================================

FUNCTION cmsoa_proc_acq_fail()

DESCRIPTION
  This function process acq_fail messages.

RETURN VALUE
  none

===========================================================================*/
void cmsoa_proc_acq_fail(
  sys_modem_as_id_e_type  asubs_id,

  multimode_stack_e_type  stack_id
)
{
  cmsoa_state_sub_info_s_type   *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);

  if (cmsoa_state == NULL)
  {
    return;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch (stack_id)
  {
    case MM_STACK_0:
    {
      if(cmsoa_state->info.is_svlte_predicted)
      {
        CM_MSG_HIGH_0("CMSOA: acq_fail, clr svlte_predicted flag");
      }

      cmsoa_state->info.is_svlte_predicted = FALSE;
      break;
    }

    default:
      break;
  }
}


/*===========================================================================

FUNCTION cmsoa_msgr_rpt_proc

DESCRIPTION
  Process reports from the LTE lower layers.

DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmsoa_msgr_rpt_proc(
  sys_modem_as_id_e_type asubs_id
)
{
  cmsoa_state_sub_info_s_type *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  cmsds_sub_prop_s_type  *sds_sub_prop_ptr = cmsds_get_sub_prop_ptr(asubs_id);

  if (cmsoa_state == NULL || sds_sub_prop_ptr == NULL ||
      !cmsoa_state->info.is_activated)
  {
    return;
  }

#if defined( FEATURE_LTE_TO_1X )
  /* If the timer expires */
  if (cmsoa_state->info.switch_delay_timer != 0 &&
      cmsoa_state->info.switch_delay_state != CMSOA_SV_OPRT_MODE_NONE &&
      cmsoa_state->info.switch_delay_reason == CMSOA_SV_SWITCH_DELAY_LTE_CONNECTED)
  {
    if (cmsoa_state->info.switch_delay_state == CMSOA_SV_OPRT_MODE_CSFB)
    {
      mm_sub_stk_id_s_type stk2_mm_id = cmph_map_sub_stk_to_id(asubs_id, MM_STACK_2);
      sd_si_info_s_type *ss_true_stk_2_info_ptr = cmss_true_stack_info_ptr(stk2_mm_id);
      /* only process delay action if there is LTE service on hybr2 and no RLF */
      if (ss_true_stk_2_info_ptr != NULL &&
          ss_true_stk_2_info_ptr->srv_status == SYS_SRV_STATUS_SRV &&
          cmregprx_get_conn_mode_status(stk2_mm_id, SYS_SYS_MODE_LTE) == FALSE &&
          ss_true_stk_2_info_ptr->mode == SD_MODE_LTE &&
          (sds_sub_prop_ptr->gpp2_sub_info.csfb_status == CM_1XCSFB_PREREG_SUCCESS ||
           sds_sub_prop_ptr->gpp2_sub_info.csfb_status == CM_1XCSFB_AVAIL ||
           sds_sub_prop_ptr->gen_sub_info.ims_sub_info.ims_reg_status_voice[SYS_SYS_MODE_LTE] == SYS_IMS_REG_STATE_REGISTERED ||
           cmsoa_check_is_1xsrlte_plmn(cmsoa_state->info.last_gwl_plmn, asubs_id) == FALSE) )
      {
        cmsoa_process_delay_action( cmsoa_state->info.switch_delay_state, asubs_id);
      }
      else
      {
        /* reset the switch delay state  */
        CM_MSG_HIGH_2("cmsoa_msgr_rpt_proc: resetting switch delay state %d %d",
                      cmregprx_get_conn_mode_status(stk2_mm_id, SYS_SYS_MODE_LTE),
                      sds_sub_prop_ptr->gpp2_sub_info.csfb_status);
        cmsoa_state->info.switch_delay_state = CMSOA_SV_OPRT_MODE_NONE;

        /* reset CSFB status */
        if(sds_sub_prop_ptr->gpp2_sub_info.csfb_status != CM_1XCSFB_NONE)
        {
          CM_MSG_HIGH_1("cmsoa_msgr_rpt_proc: resetting csfb_status. Current status: %d",
                        sds_sub_prop_ptr->gpp2_sub_info.csfb_status);
          sds_sub_prop_ptr->gpp2_sub_info.csfb_status = CM_1XCSFB_NONE;
        }
      }
    }
  }
#endif
}
/*===========================================================================

FUNCTION cmsoa_check_and_switch_fromc2k_to_srlte

DESCRIPTION
  This function performs all neccessary checks before it decides to whether
  switching to SRLTE operation.
  If a switch() operation is required, it will call to another function that
  will only performs the switch.

  conditions to be met are the following
       If a)is_c2k_modeswitch2srlte = TRUE && UE is capable of SRLTE operation
      ** b) SD reports stable 1x service on MAIN
      ** c) UE mode on MAIN is CSFB preferred
      ** d) UE is currently operating in single stack -CSFB mode
      ** e) UE is not in a 1xcsfb call and is not 1xcsfb capable
      ** and f)1x MCC is part of Home PLMN list


DEPENDENCIES  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsoa_check_and_switch_fromc2k_to_srlte(
  sys_modem_as_id_e_type  asubs_id
)
{
  cmss_s_type               *ss_ptr             = cmss_ptr();
  cmph_s_type               *ph_ptr             = cmph_ptr();
  cmsoa_state_sub_info_s_type   *cmsoa_state        = cmsoa_state_sub_info_ptr(asubs_id);
  boolean                    is_home_mcc        = FALSE;
  sd_si_info_s_type         *si_info_ptr        = NULL;
  boolean                   is_fplmn_sxlte = FALSE;

  cmmsc_state_machine_s_type *state_machine = cmmsimc_state_machine_ptr(asubs_id);
  sys_ue_mode_e_type curr_ue_mode = state_machine->stack_common_info.ue_mode;
  cmsds_sub_prop_s_type *sds_sub_prop_ptr = cmsds_get_sub_prop_ptr(asubs_id);
  sys_modem_as_id_e_type other_sub = (asubs_id == SYS_MODEM_AS_ID_1? SYS_MODEM_AS_ID_2: SYS_MODEM_AS_ID_1);
  cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop(asubs_id);
  mm_sub_stk_id_s_type mm_id = cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0);
  cmregprx_stack_info_s_type      *cmregprx_stack_info_ptr = cmregprx_get_stk_ptr(mm_id);

  if (cmmsc_get_1x_sub() != asubs_id)
  {
    return FALSE ;
  }
  if(cmsoa_state == NULL || sds_sub_prop_ptr == NULL || cmregprx_stack_info_ptr == NULL ||
      ss_info_sub_prop_ptr == NULL || !cmutil_is_as_id_valid(asubs_id))
  {
    return FALSE;
  }
  if (!cmsoa_state->config.is_c2k_modeswitch2srlte)
  {
    CM_MSG_MED_1("c2kswitch: sub %d, is_c2k_modeswitch2srlte disabled", asubs_id);
    return FALSE ;
  }

  if (sds_sub_prop_ptr->gen_sub_config.ue_usage_setting == SYS_UE_USAGE_SETTING_DATA_CENTRIC )
  {
    CM_MSG_HIGH_1("c2kswitch: sub %d, Do not perform switch in data centric device", asubs_id);
    return FALSE ;
  }

  /* Check if SRLTE operation is possible and if UE
  ** is currently in CSFB submode of CSFB preferred UE mode
  */
  if (!(curr_ue_mode                   == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED &&
        cmsoa_state->info.curr_sv_oprt_mode == CMSOA_SV_OPRT_MODE_CSFB &&
        cmss_is_stack2_operational(asubs_id)    == FALSE &&
        cmsoa_is_srlte_capability(asubs_id)
       )
     )
  {
    CM_MSG_MED_4_EXT("c2kswitch: sub %, Not in single stack csfb preferred mode: main mode_pref=%d,hybr2_op=%d, ue_mode=%d",
                     asubs_id,
                     CMPH_SS_MODE_PREF(asubs_id,ph_ptr),
                     cmss_is_stack2_operational(asubs_id),
                     curr_ue_mode,
                     (asubs_id+1));


    return FALSE ;
  }

  /* In non-MMSS configuration, we do not need to switch to dual stack mode
  ** since LTE will be in OOS timeline and  might unnecessarily stop HDR stack
  */
  if(sd_misc_is_mmss_operation(asubs_id) == FALSE)
  {
    CM_MSG_MED_1("c2kswitch: sub %d, Not MMSS config", asubs_id);
    return FALSE ;
  }

  if (cmph_is_in_emergency_cb())
  {
    CM_MSG_MED_1("c2kswitch: sub %d, skip switch due to ECBM mode", asubs_id);
    return FALSE ;
  }

  si_info_ptr = cmss_true_stack_info_ptr(mm_id);

  if (si_info_ptr == NULL)
  {
    CM_MSG_MED_1("c2kswitch: sub %d, si_ptr NULL", asubs_id);
    return FALSE ;
  }
  /* Check if UE is on 1x full service and immediate BSR is complete */
  if (!( sys_srv_status_is_srv(si_info_ptr->srv_status) &&
         si_info_ptr->mode            == SYS_SYS_MODE_CDMA &&
         si_info_ptr->is_pref_srv_acq == FALSE
       )
     )
  {
    CM_MSG_MED_1("c2kswitch: sub %d, No stable 1x service on MAIN", asubs_id);
    return FALSE ;
  }
  /* Check if UE is 1xcsfb capable or is in a 1xcsfb call
  */
#ifdef FEATURE_LTE_TO_1X
  if (cmsds_is_in_1xcsfb_mode(asubs_id))
  {
    CM_MSG_HIGH_1("c2kswitch: sub %d, 1xcsfb activated", asubs_id);
    return FALSE ;
  }
  /* check for a 1xcsfb call in the call obj array
  */
  {
    cm_call_id_type  call_id = CM_CALL_ID_INVALID;
    call_id =  cmcall_is_there_csfb_call_per_sub(asubs_id,
               CM_CALL_CSFB_TYPE_1XCSFB,
               CM_CALL_ID_INVALID);
    if (call_id != CM_CALL_ID_INVALID)
    {
      CM_MSG_HIGH_2("c2kswitch: sub %d, 1xcsfb call active %d", asubs_id, call_id);
      return FALSE ;
    }
  }
  /* check for a 1xcsfb call in the priority Q
  */
  {
    if ((cmtask_orig_para_search_orig_mode(mm_id, SD_SS_ORIG_MODE_1XCSFB_NORMAL_ORIG) != NULL) ||
        (cmtask_orig_para_search_orig_mode(mm_id, SD_SS_ORIG_MODE_1XCSFB_EMERG_ORIG) != NULL)
       )
    {
      CM_MSG_HIGH_1("c2kswitch: sub %d, 1xcsfb obj in pri Q, asubs_id", asubs_id);
      return FALSE ;
    }
  }
#endif //FEATURE_LTE_TO_1X

  /* During the DDS change from SUB2 to SUB1, PS_DETACH will happoen on SUB2 and on SUB1, UE mode will be set to CSFB_PREF.
  If DETACH takes lojng time on SUB2 and if there is a 1X servcie indication, it will cause silent SRLTE switch as top of the queue is phone object and
  UE MODE is already CSFB_PREF. So skip the c2k switch till PS_DETACH is completed and 1X + L preferences are forced to SD.Switch to SRLTE should be
  performed only after the DDS preferences are forced on to SUB1(1X + L) and after IBSR */
  if (cmph_is_feature_mode_msim() &&
      cmregprx_get_substate(cmph_map_sub_stk_to_id(other_sub, MM_STACK_0))
      == CMREGPRX_SUBSTATE_WAIT_PS_DETACH_CNF &&
      ph_ptr->device_prop.msim_prop.default_data_subs == asubs_id )
  {
    CM_MSG_MED_1("c2kswitch: sub %d, PS DETACH is in progress on SUB2. So skip the c2k based switch", asubs_id);
    return FALSE ;
  }

  /* if OTASP or OTAPA call is presnet, dont trigger switch 
       In case PRL commit is success, call end prefernces will not be forced to SD so as switch pref 
       This can lead to inconsistent state */
  if( cmcall_is_there_a_call_type_per_sub(asubs_id, CM_CALL_TYPE_OTAPA, NULL) != CM_CALL_ID_INVALID ||
      cmcall_is_there_a_call_type_per_sub(asubs_id, CM_CALL_TYPE_STD_OTASP, NULL) != CM_CALL_ID_INVALID ||
      cmcall_is_there_a_call_type_per_sub(asubs_id, CM_CALL_TYPE_NON_STD_OTASP, NULL) != CM_CALL_ID_INVALID )
  {
    CM_MSG_HIGH_0("Switch stopped due to active OTAPA/OTASP call");
    return FALSE;
  }

  if(cmsoa_state->info.curr_sv_oprt_mode == CMSOA_SV_OPRT_MODE_CSFB &&
     cmregprx_stack_info_ptr->is_ue_mode_substate_srlte == TRUE)
  {
    CM_MSG_HIGH_1("c2kswitch:sub %d, Switch to CSFB in progress", asubs_id);
    return FALSE;
  }

  /* If we got here conditions a-e are met. Check for HOME mcc with PM.
  ** If for some reason MCC check cannot be done, assume home mcc
  */

  /* Get 1x MCC value */
  if (ss_info_sub_prop_ptr->sub_info.gpp2_sub_info.prl_3gpp2_mcc.count == 0)
  {
    CM_MSG_MED_1("c2kswitch: sub %d, No valid MCC value", asubs_id);
    is_home_mcc = TRUE;
  }
  else
  {
    /* set plmn =  mcc + wildcard mnc
    */
    is_home_mcc = cmsoa_is_c2kswitch_possible(asubs_id);
  }
  if (is_home_mcc)
  {
    /* Check if the top of the MAIN priority Q is the ph object
    */
    cm_orig_q_s_type    *q_top_ptr          = NULL;
    boolean is_switch_done = FALSE;

    q_top_ptr = cmtask_orig_para_get_top(mm_id);

    CM_MSG_HIGH_2("c2kswitch: sub %d, is_lte_Scanned value :%d",
                  asubs_id, si_info_ptr->is_lte_scanned );
    /* If the top is not the phone object or SD indicates that lte has not been
    ** scanned even once, use reason ACTIVATE_HYBR2 for the switch.
    ** This will force LTE scan to happen. If the top is the Phone object, assume that
    ** full scan of LTE has already happened on MAIN and activate HYBR2 with reason
    ** ACTIVATE_HYBR2_SILENT
    */

    /* If any SXLTE PLMN have been added to FPLMN List, the PLMNs should be unblocked and 
    ** use reason ACTIVATE_HYBR2 for the switch. This will force the unblocked SXLTE PLMNs
    ** to be scanned immediately
    */
    #if defined(FEATURE_LTE_TO_1X)
    cmmsc_auto_check_and_unblock_1x_sxlte_plmn(asubs_id,FALSE,&is_fplmn_sxlte);
    #endif
    if( (q_top_ptr != NULL &&
         q_top_ptr->orig->act_priority != CM_ACT_PRIORITY_PH) ||
        (cmtask_orig_para_search_act_type(mm_id,CM_ACT_TYPE_DATA_CALL) != NULL) ||
        (si_info_ptr->is_lte_scanned == FALSE)
        #if defined(FEATURE_LTE_TO_1X)
        ||(is_fplmn_sxlte == TRUE)
        #endif
      )
    {
      cmsoa_set_pref_update_reason( SD_SS_PREF_UPDATE_REASON_ACTIVATE_HYBR2, asubs_id );
    }
    else
    {
      cmsoa_set_pref_update_reason( SD_SS_PREF_UPDATE_REASON_ACTIVATE_HYBR2_SILENT, asubs_id );
    }

    CM_MSG_HIGH_1("c2kswitch:  sub %d, Switch to SRLTE", asubs_id);

    is_switch_done = cmsoa_sv_switch_base(CMSOA_SV_OPRT_MODE_SVLTE, asubs_id, FALSE );
    /*If switch was not performed and pre-rempted for some reason, clear the
    **pref update reas
    */
    if(is_switch_done == FALSE)
    {
      CM_MSG_HIGH_0("c2kswitch:Clear pref_update_reas since switch was pre-empted");
      cmsoa_set_pref_update_reason( SD_SS_PREF_UPDATE_REASON_NONE, asubs_id);
      return FALSE;
    }
    /*1xcsfb status needs to be reset only when device is in CSFB mode and switched to
    ** SRLTE mode after camping on home 1x
    */
#if defined(FEATURE_LTE_TO_1X)
    if(sds_sub_prop_ptr->gpp2_sub_info.csfb_status != CM_1XCSFB_NONE)
    {
      CM_MSG_HIGH_2("sub %d, Resetting csfb_status, Current status: %d",
                    asubs_id, sds_sub_prop_ptr->gpp2_sub_info.csfb_status);
      sds_sub_prop_ptr->gpp2_sub_info.csfb_status = CM_1XCSFB_NONE;
    }
#endif
  }

  return TRUE;
}

/*===========================================================================
  This function sets CMSOA to perform only in CSFB mode. Under no network
  conditions will phone switch to SRLTE mode.
===========================================================================*/

static void cmsoa_srlte_ue_mode_csfb(
  sys_modem_as_id_e_type asubs_id
)
{
  cmsoa_state_sub_info_s_type   *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  if(cmsoa_state == NULL|| !IS_VALID_PH_SUB_PTR(asubs_id))
  {
    return;
  }

  cmsoa_state->info.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_CSFB;
  cmsoa_state->config.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_CSFB;
  cmsoa_state->config.is_switch_enabled = FALSE;

  if (BETWEEN((int)cmph_ptr()->device_prop.curr_nam, (int)CM_NAM_NONE, (int)NV_MAX_NAMS))
  {
    cmph_ptr()->sub_info[asubs_id]->pref_info.hybr_pref =
      cmph_ptr()->sub_info[asubs_id]->prst_pref_info[cmph_ptr()->device_prop.curr_nam].hybr_pref =
        CM_HYBR_PREF_CDMA__HDR;
  }
}

/*===========================================================================
  This function sets in hybr capability to enable SRLTE operation. Since
  modem operates always in SRLTE, CMSOA (dynamic SRLTE<->CSFB) is disabled.
===========================================================================*/

static void cmsoa_srlte_ue_mode_srlte_only(
  sys_modem_as_id_e_type asubs_id
)
{
  cmsoa_state_sub_info_s_type   *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  if(cmsoa_state == NULL|| !IS_VALID_PH_SUB_PTR(asubs_id))
  {
    return;
  }

  cmsoa_state->info.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_SVLTE;
  cmsoa_state->config.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_SVLTE;
  cmsoa_state->config.is_switch_enabled = FALSE;

  if (BETWEEN((int)cmph_ptr()->device_prop.curr_nam, (int)CM_NAM_NONE, (int)NV_MAX_NAMS))
  {
    cmph_ptr()->sub_info[asubs_id]->pref_info.hybr_pref =
      cmph_ptr()->sub_info[asubs_id]->prst_pref_info[cmph_ptr()->device_prop.curr_nam].hybr_pref =
        CM_HYBR_PREF_CDMA__LTE__HDR;
  }
}

/*===========================================================================
  This function sets CMSOA to be in CSFB and if no 1xCSFB is supported on LTE
  then modem will try to switch to SRLTE to have 1x and LTE in standby state.
  Privided if that PLMN is configured to support SRLTE in XML.
===========================================================================*/

static void cmsoa_srlte_ue_mode_csfb_pref(
  sys_modem_as_id_e_type asubs_id
)
{
  cmsoa_state_sub_info_s_type   *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  if(cmsoa_state == NULL)
  {
    return;
  }

  /* the switch will only be true first time when ue powers up
  ** in CSFB preferred mode and SVD feature is not enabled.
  ** curr sub mode and hybr pref would have been set in cmsoa_init()
  */
  if(cmsoa_state->config.is_switch_enabled == FALSE)
  {
    /* If SVD feature is enabled, disable dynamic switch,
    ** set current sub mode to SRLTE and hybr pref to CDMA_LTE_HDR
    */
    if(cmph_get_enable_volte_stack2(asubs_id))
    {
      cmsoa_srlte_ue_mode_srlte_only(asubs_id);
    }
    else
    {
      cmsoa_srlte_ue_mode_csfb(asubs_id);
      cmsoa_state->config.is_switch_enabled = TRUE;
      cmsoa_check_and_switch_fromc2k_to_srlte(asubs_id);
    }
  }
}

/*===========================================================================
  This function helps MSC in changing the UE-mode. When UE-mode changes
  CMSOA may be in between its switch procedures, may be running timers or
  have deferred the switch procedure. In any CMSOA state, if UE-mode changes
  then whether modem is in SRLTE or CSFB is decided by UE-mode. So Dynamic-
  switch module is initialized to support that UE-mode as its done in
  cmph_config_hybr_pref & cmsoa_init.
===========================================================================*/

void cmsoa_srlte_ue_mode_update(
  sys_ue_mode_e_type ue_mode,
  sys_modem_as_id_e_type asubs_id
)
{
  cmsoa_state_sub_info_s_type    *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  if(cmsoa_state == NULL)
  {
    return;
  }

  CM_MSG_HIGH_1_EXT("proc ue_mode change to %d", ue_mode, (asubs_id+1));

  switch(ue_mode)
  {
    case SYS_UE_MODE_CSFB:
      cmsoa_srlte_ue_mode_csfb(asubs_id);
      break;
    case SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED:
      cmsoa_srlte_ue_mode_csfb_pref(asubs_id);
      break;
    case SYS_UE_MODE_1XSRLTE_ONLY:
      cmsoa_srlte_ue_mode_srlte_only(asubs_id);
      break;
    default:
      CM_MSG_HIGH_1("ue_mode %d have no impact on cmsoa state", ue_mode);
      return;
  }

  /* Reset, similar to cmsoa_init values */
  cmsoa_state->info.switch_delay_timer = 0;
  cmsoa_state->info.switch_delay_reason = CMSOA_SV_SWITCH_DELAY_REAS_NONE;
  cmsoa_state->info.switch_delay_state = CMSOA_SV_OPRT_MODE_NONE;
  /* Clear attach complete timer as well since switch_delay_reas is
  ** being set to NONE
  */
  cmtask_attach_complete_timer_clear(asubs_id);

  cmsoa_state->info.pref_update_reason = SD_SS_PREF_UPDATE_REASON_NONE;
  cmsoa_state->info.is_svlte_predicted = FALSE;

  cmsoa_activate_arbitration(asubs_id);
}


/*===========================================================================

FUNCTION cmsoa_skip_dom_sel_due_to_soa()

DESCRIPTION
  This function tells if Domain selection logis of disablign LTE
  should be skipped, as SOA can take care of it.
  1. If in SV mode, Hybr-2 is on
  2. if in Non-SV mode, but PLMN belongs to SV-PLMN list, LTE full service has been reported.

RETURN VALUE
  none

===========================================================================*/
boolean cmsoa_skip_dom_sel_due_to_soa(
  sys_modem_as_id_e_type asubs_id
)
{

  boolean ret_val                        = FALSE;
  cmsoa_state_sub_info_s_type *cmsoa_state   = cmsoa_state_sub_info_ptr(asubs_id);
  sd_si_info_s_type *intl_srv_info_ptr   =  cmss_true_stack_info_ptr(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0));
  sys_plmn_id_s_type plmn;

  if(cmsoa_state == NULL || intl_srv_info_ptr == NULL)
  {
    return FALSE;
  }

  plmn = intl_srv_info_ptr->sid.plmn.plmn_id;
  if(cmsoa_state->info.is_activated)
  {
    CM_MSG_MED_3( "CMSOA: plmn to be checked [%x][%x][%x]",plmn.identity[0],
                  plmn.identity[1],plmn.identity[2]);
    if (cmss_is_stack2_operational(asubs_id))
    {
      ret_val = TRUE;
    }
    else if( (cmsoa_state->info.curr_sv_oprt_mode == CMSOA_SV_OPRT_MODE_CSFB) &&
             ( intl_srv_info_ptr->mode == SYS_SYS_MODE_LTE ) &&
             ( intl_srv_info_ptr->srv_status == SYS_SRV_STATUS_SRV )&&
             ( cmsoa_svlte_plmn_list_search(&cmsoa_state->config.svlte_plmn_list,
                                            &plmn))
           )
    {
      ret_val = TRUE;
    }
  }
  return ret_val;
}


/*===========================================================================

FUNCTION cmsoa_sd_rpt_proc

DESCRIPTION
  Process System Determination reports


DEPENDENCIES
  CMSOA object must have already been initialized with
  cmsoa_init().

  cmss_sd_rpt_proc() should be invoked before calling cmsoa_sd_rpt_proc()

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_sd_rpt_proc(

  const cm_hdr_type   *rpt_ptr
  /* Pointer to SD reports */
)
{

  /*lint -e{826} */
  const cm_sd_rpt_u_type         *sd_rpt_ptr = (cm_sd_rpt_u_type *) rpt_ptr;
  /* Pointer to SD reports */

  CM_ASSERT( sd_rpt_ptr != NULL );

  switch( sd_rpt_ptr->hdr.cmd )
  {

    /* SRV indication
    */
    case CM_SRV_IND_INFO_F:

      /* If a)is_c2k_modeswitch2srlte = TRUE
      ** b) SD reports stable 1x service on MAIN
      ** c) UE mode on MAIN is CSFB preferred
      ** d) UE is currently operating in single stack -CSFB mode
      ** and e)1x MCC is part of Home PLMN list
      */
      if ( sd_rpt_ptr->srv_ind_info.stack_id == MM_STACK_0 )
      {
        cmsoa_check_and_switch_fromc2k_to_srlte(sd_rpt_ptr->hdr.asubs_id);
      }
      break;


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    default:
      break;
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  } /* switch( sd_rpt_ptr->hdr.cmd ) */

} //cmsoa_sd_rpt_proc


/*===========================================================================

FUNCTION cmsoa_is_srlte_in_inconsistent_state

DESCRIPTION
  return if CM is in inconsistent state for SRLTE


DEPENDENCIES
  CMSOA object must have already been initialized with
  cmsoa_init().


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsoa_is_srlte_in_inconsistent_state(
  sys_modem_as_id_e_type     asubs_id
)
{
  if (cmsoa_get_current_sv_oprt_state(asubs_id) == CMSOA_SV_OPRT_MODE_SVLTE &&
      cmph_is_subs_feature_mode_srlte(asubs_id) &&
      cmss_is_stack2_operational(asubs_id) == FALSE &&
      cmss_is_main_operational(asubs_id) &&
      (cmph_ptr()->device_prop.ph_oprt_mode.true_oprt_mode == SYS_OPRT_MODE_ONLINE) &&
      !cmclnup_is_ongoing_subsc(BM(asubs_id))
     )
  {
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION cmsoa_is_c2kswitch_possible

DESCRIPTION
  check if SVD possible for all 3GPP2 MCC/MNC


DEPENDENCIES
  CMSOA object must have already been initialized with
  cmsoa_init().


RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsoa_is_c2kswitch_possible(
  sys_modem_as_id_e_type  asubs_id
)
{
  uint8                      count              = 0;
  cmss_s_type                *ss_ptr            = cmss_ptr();
  sys_plmn_id_s_type         plmn;
  boolean                    is_home_mcc        = FALSE;
  boolean                    ret                = FALSE;
  cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop(asubs_id);
  if(ss_info_sub_prop_ptr == NULL || !cmutil_is_as_id_valid(asubs_id))
  {
    return FALSE;
  }

  for (count = 0; count < ss_info_sub_prop_ptr->sub_info.gpp2_sub_info.prl_3gpp2_mcc.count; count++)
  {
    CM_MSG_MED_1("c2kswitch:MCC %d",ss_info_sub_prop_ptr->sub_info.gpp2_sub_info.prl_3gpp2_mcc.mcc_list[count] );
    ret = sys_plmn_set_mcc_mnc2(TRUE,ss_info_sub_prop_ptr->sub_info.gpp2_sub_info.prl_3gpp2_mcc.mcc_list[count],SYS_WILDCARD_MNC,&plmn);
    if(ret == FALSE)
    {
      CM_MSG_MED_0("c2kswitch:plmn conversion failed");
      continue;
    }
    else
    {
      CM_MSG_HIGH_3("c2kswitch: plmn 0x%x 0x%x 0x%x",
                    plmn.identity[0],
                    plmn.identity[1],
                    plmn.identity[2]);
      if(policyman_svd_possible_on_plmn_per_subs(&plmn, TRUE,&is_home_mcc, asubs_id) !=
          POLICYMAN_STATUS_SUCCESS)
      {
        CM_MSG_HIGH_0("c2kswitch:PM check incomplete");
        continue;
      }
      if (is_home_mcc)
      {
        CM_MSG_MED_0("c2kswitch:PM check -home MCC");
        break;
      }
    }
  }
  return is_home_mcc;
}

/*===========================================================================

FUNCTION cmsoa_check_and_set_bootup_params_to_srlte_state

DESCRIPTION
  If Volte is disabled from AP, and UE mode is srlte_csfb_pref,
  bootup in SRLTE mode instead of CSFB mode.


DEPENDENCIES
  CMSOA object must have already been initialized with
  cmsoa_init().

  cmss_sd_rpt_proc() should be invoked before calling cmsoa_sd_rpt_proc()

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsoa_check_and_set_bootup_params_to_srlte_state(sys_modem_as_id_e_type     asubs_id)
{
  sd_ss_hybr_pref_e_type new_hybr_pref = SD_SS_HYBR_PREF_CDMA__LTE__HDR;
  cmph_s_type *ph_ptr = cmph_ptr();
  cmsoa_state_sub_info_s_type    *cmsoa_state = cmsoa_state_sub_info_ptr(asubs_id);
  cmmsc_state_machine_s_type *state_machine = cmmsimc_state_machine_ptr(asubs_id);
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);

  if(cmsoa_state == NULL || ph_sub_ptr == NULL)
  {
    return;
  }
  CM_MSG_HIGH_2("ue_mode %d, oprt_mode %d",state_machine->stack_common_info.ue_mode, ph_ptr->device_prop.ph_oprt_mode.oprt_mode);

  if( state_machine->stack_common_info.ue_mode == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED
      && ph_ptr->device_prop.ph_oprt_mode.oprt_mode != SYS_OPRT_MODE_ONLINE )
  {
    cmsoa_state->info.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_SVLTE;
    cmsoa_state->config.curr_sv_oprt_mode = CMSOA_SV_OPRT_MODE_SVLTE;


    if (BETWEEN((int)ph_ptr->device_prop.curr_nam, (int)CM_NAM_NONE, (int)NV_MAX_NAMS))
    {
      ph_sub_ptr->pref_info.hybr_pref =
        ph_sub_ptr->prst_pref_info[ph_ptr->device_prop.curr_nam].hybr_pref =
          cmutil_map_sd_hybr_pref_to_cm_hybr_pref(new_hybr_pref);
    }
    ph_sub_ptr->sub_3gpp2_pref.int_hybr_pref = new_hybr_pref;
    ph_sub_ptr->sub_dbg_info.init_hybr_pref = new_hybr_pref;
  }
  return;
}

/*===========================================================================

FUNCTION cmsoa_check_is_1xsrlte_plmn

DESCRIPTION
  Checks whether given given PLMN is SRLTE plmn or not
  Queries Policyman API to get the info

DEPENDENCIES
  None

RETURN VALUE
  TRUE if given PLMN is SRLTE PLMN

SIDE EFFECTS
  none

===========================================================================*/
boolean cmsoa_check_is_1xsrlte_plmn(sys_plmn_id_s_type plmn_id, sys_modem_as_id_e_type subsId)
{

  boolean is_1xsrlte_plmn = FALSE;

  if( policyman_svd_possible_on_plmn_per_subs(&(plmn_id), FALSE,
      &is_1xsrlte_plmn, subsId) == POLICYMAN_STATUS_SUCCESS )
  {
    is_1xsrlte_plmn = is_1xsrlte_plmn;
  }

  return is_1xsrlte_plmn;
}

#endif /* defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE) */
