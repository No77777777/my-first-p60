/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

      C A L L   M A N A G E R   P O L I C Y   M A N A G E R   P R O X Y

GENERAL DESCRIPTION
  This file is the main implementation for the Policy Manager(PM) proxy. The
  purpose of PMProxy is to act as a proxy to interface with the Policy Manager.
  Interpreting the Policy Manager messages into Call Manager actions.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  cmpmprx_init() must be called to initialize this module before any other
  function declared in this module is being called.

Copyright (c) 2014 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmpmprx.c#1 $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
07/09/14   ss       Do not enforce policy when UE is OFFLINE
09/12/13    jvo     Fix 2 static analysis errors with one change relating to
                    policy comparison
04/29/13    ak      Initial design

===========================================================================*/
#include "mmcp_variation.h"
#include "customer.h"       /* Customer configuration file */
#include "comdef.h"         /* Definition for basic types and macros */
#include "cmph.h"
#include "cm_msgr_msg.h"
#include "cmtaski.h"
#include "cmidbg.h"
#include "cmpmprx.h"
#include "cmss.h"
#include "cmmsc.h"
#include "cmsimcoord.h"
#include "cmefs.h"
#include "cmsoa.h"
#include "cmsds.h"
#include "cmlog.h"     /* Interface to diag services */
#include "cmlog_v.h"
#include "cmcfcm.h"

//#include "cmwcall.h"
#ifdef FEATURE_DOMAIN_SELECTION
#include "cmsds.h"
#endif
#include "cmclnup.h"

#ifdef CM_DEBUG
#error code not present
#endif
#include "cmemgext.h"

#ifdef TEST_FRAMEWORK
#error code not present
#endif

/*===========================================================================
=============================================================================
=============================================================================
============================  Internal Functions ==================================
=============================================================================
=============================================================================
===========================================================================*/

/* Forward declaration
*/
static policyman_status_t       cmpmprx_retrieve_policy_item_data
(
  cm_policy_config_s_type    *pm_ptr,
  policyman_item_collection_t const *policy_item,
  policy_change_mask         *change_mask,
  policy_change_mask         *device_change_mask,
  boolean                    is_sandbox,
  uint8                      refresh_subs_mask
);
static boolean cmpmprx_init_pm_feature_mode(boolean is_mcfg_refresh );
static void cmpmprx_init_pm_ue_mode(boolean is_pwr_up, uint8 refresh_subs_mask);
static void cmpmprx_set_policy_chg_flag( sys_modem_as_id_e_type asubs_id, policy_change_mask change_value );
static void cmpmprx_reset_policy_chg_flag( sys_modem_as_id_e_type asubs_id, policy_change_mask change_value );
static void cmpmprx_print_current_policy(sys_modem_as_id_e_type asubs_id);
static boolean cmpmprx_compare_policies(cm_policy_config_s_type    *policy_1,cm_policy_config_s_type    *policy_2);
static void    cmpmprx_enforce_service_scan_mode(
  sys_modem_as_id_e_type     asubs_id
);
static void    cmpmprx_enforce_voice_domain_pref(
  sys_modem_as_id_e_type     asubs_id,
  boolean                    force_pref
);

static void    cmpmprx_enforce_volte_state(
    sys_modem_as_id_e_type     asubs_id,
    boolean                    enable_ims
);

static cm_policy_config_s_type cmpm_local;
/* The one and only phone object */

static uint8                   cmpm_active_subs;
static int8                    cmpm_default_data_subs;
static sys_sim_state_e_type    cmpm_last_sim_state[MAX_SIMS];
/* Function Definations
*/
/*===========================================================================

FUNCTION cmpmprx_is_item_value_changed

DESCRIPTION
  Check if the value of any specific item on subs is really changed.

DEPENDENCIES
None

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_is_item_value_changed(
  cm_policy_config_s_type *curr_policy,
  cm_policy_config_s_type *local_policy,
  policyman_item_id_t      item,
  sys_modem_as_id_e_type   sub_id);

/*===========================================================================

FUNCTION cmpm_ptr

DESCRIPTION
  Return a pointer to the one and only phone policy object.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif

cm_policy_config_s_type  *cmpm_ptr( void )
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return &cmpm_local;

} /* cmph_ptr() */

/*===========================================================================

FUNCTION cmpm_sub_ptr

DESCRIPTION
  Return a pointer to sub policy.

DEPENDENCIES
  none

RETURN VALUE
  cm_sub_level_policy_s_type *

SIDE EFFECTS
  none

===========================================================================*/
cm_sub_level_policy_s_type  *cmpm_sub_ptr(sys_modem_as_id_e_type      asubs_id)
{
  if (!(asubs_id >= SYS_MODEM_AS_ID_1 &&
        asubs_id < SYS_MODEM_AS_ID_NO_CHANGE &&
        asubs_id < MAX_SIMS))
  {
    return NULL;
  }
  else
  {
    return cmpm_ptr()->sub_policy[asubs_id];
  }
}

/*===========================================================================

FUNCTION cmpmprx_mem_free

DESCRIPTION
  Free the memory allocated for policy. Elements of the policy are now
  allocated dynamically so need a dedicated function to free the memory.

DEPENDENCIES
  none

RETURN VALUE
  void

SIDE EFFECTS
  none

===========================================================================*/
static void cmpmprx_mem_free(cm_policy_config_s_type *policy_ptr)
{
  uint8 i=0;

  if( policy_ptr == NULL )
  {
    return;
  }

  for( i=0; i<MAX_SIMS; i++ )
  {
    if( policy_ptr->sub_policy[i] != NULL )
    {
      cm_mem_free(policy_ptr->sub_policy[i]);
      policy_ptr->sub_policy[i] = NULL;
    }
  }
  cm_mem_free(policy_ptr);
}

/*===========================================================================

FUNCTION cmpmprx_configure_num_policies

DESCRIPTION
  Based on the new number of policies add or delete an existing instance on
  policy in CMPMPRX.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void                      cmpmprx_configure_num_policies
(
  cm_policy_config_s_type   *pm_ptr,

  size_t                    new_nSims
)
{
  uint8                     i=0;
  uint8                     size = ARR_SIZE(pm_ptr->sub_policy);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If number of Sims has not changed, we still have to deal with same number
  ** of policies. */
  if (pm_ptr->device_mode.nSims == new_nSims)
  {
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If new sims is greater create more policy structures.*/
  for (i=pm_ptr->device_mode.nSims; i<new_nSims && i<size; i++)
  {
    if(pm_ptr->sub_policy[i] == NULL)
    {
      pm_ptr->sub_policy[i] = (cm_sub_level_policy_s_type *) cm_mem_malloc(sizeof(cm_sub_level_policy_s_type));
    }
    pm_ptr->sub_policy[i]->policy_items.policy_band_capability     = SYS_BAND_MASK_ANY;
    pm_ptr->sub_policy[i]->policy_items.policy_lte_band_capability = SYS_LTE_BAND_MASK_CONST_ANY;
    pm_ptr->sub_policy[i]->policy_items.policy_tds_band_capability = SYS_BAND_MASK_ANY;
    pm_ptr->sub_policy[i]->policy_items.service_scan_mode          = POLICYMAN_SVC_MODE_FULL;
    pm_ptr->sub_policy[i]->policy_items.ue_operation_mode          = SYS_UE_MODE_NORMAL;
    pm_ptr->sub_policy[i]->policy_items.policy_domain_capability = SYS_SRV_DOMAIN_CS_PS;

    if(i == 0)
    {
      pm_ptr->sub_policy[i]->policy_items.policy_max_mode_capability = SD_SS_MODE_PREF_ANY;
    }
    else
    {
      pm_ptr->sub_policy[i]->policy_items.policy_max_mode_capability = SD_SS_MODE_PREF_GSM;
    }
    pm_ptr->sub_policy[i]->policy_items.policy_mode_capability     = pm_ptr->sub_policy[i]->policy_items.policy_max_mode_capability;

    pm_ptr->sub_policy[i]->policy_changes = PM_CHG_CHANGE_NONE;
  }

  for(i=new_nSims; i<pm_ptr->device_mode.nSims; i++)
  {
    if(pm_ptr->sub_policy[i] != NULL)
    {
      cm_mem_free(pm_ptr->sub_policy[i]);
      pm_ptr->sub_policy[i] = NULL;
    }
  }
}

static void cmpmprx_configure_policy_max_active( void )
{
  cm_policy_config_s_type   *pm_ptr = cmpm_ptr();
  cmph_s_type               *ph_ptr = cmph_ptr();

  uint8                      subsc_status[MAX_SIMS];
  /**< Array of 3gpp & 3gpp2 subscription bitmask for each sub */

  CM_MSG_HIGH_2("Inform device mode to MMOC nsim %d, nmaxactive %d",
                pm_ptr->device_mode.nSims, pm_ptr->device_mode.nMaxActive);

  /* Report MSIM info */
  cmlog_msim_info(
    ph_ptr->device_prop.msim_prop.user_standby_pref,
    ph_ptr->device_prop.msim_prop.internal_standby_pref,
    ph_ptr->device_prop.msim_prop.active_subs,
    ph_ptr->device_prop.msim_prop.default_data_subs,
    ph_ptr->device_prop.msim_prop.priority_subs,
    ph_ptr->device_prop.msim_prop.dds_switch_type,
    cmpmprx_get_device_mode()
  );

  cmph_get_ph_sub_avail_info(MAX_SIMS, subsc_status);

  /* Push standby pref change to MMOC */
  cmmsimc_proc_cmd_standby_pref_chgd(ph_ptr->device_prop.msim_prop.internal_standby_pref,
                                     ph_ptr->device_prop.msim_prop.active_subs,
                                     subsc_status,
                                     MAX_SIMS,
                                     FALSE);

  /* Inform new Max active subs to MCS */
  cmph_sp_subs_info_update();
}

static void cmpmprx_configure_policy_max_active_data( void )
{
#ifdef FEATURE_MMODE_DUAL_SIM
  sys_modem_as_id_e_type   curr_subs;
  cmph_s_type             *ph_ptr = cmph_ptr();
  cm_policy_config_s_type  *pm_ptr = cmpm_ptr();
  cm_pref_s_type          *pref_ptr = NULL;
  cmph_sub_info_s_type *ph_sub_ptr = NULL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(curr_subs=SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(curr_subs); curr_subs++)
  {
    mm_sub_stk_id_s_type local_mm_id;

    local_mm_id.asubs_id = curr_subs;
    local_mm_id.stk_id = MM_STACK_0;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    if( curr_subs != ph_ptr->device_prop.msim_prop.default_data_subs )
    {
      if(cmcall_check_if_ps_call_active(local_mm_id))
      {
        CM_MSG_HIGH_1("Active PS calls on sub %d", curr_subs);

#if (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900))
        cmcall_end_active_1x_datacalls(curr_subs,TRUE);
#endif
      }

      if(cmcall_is_no_call_in_gw_ps_per_subs(curr_subs) == FALSE )
      {
        cmcall_end_each_call_with_type_per_sub(CM_CALL_TYPE_DATA,
                                               CM_CALL_DUAL_DATA_NOT_SUPPORTED, curr_subs);
      }

      if(cmtask_get_num_stacks(curr_subs)>1)
      {
#if (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900))
        cmcall_end_active_HDR_datacalls(CM_CALL_END_CLIENT_END);
#endif

        local_mm_id.stk_id = MM_STACK_1;
        if(cmtask_orig_para_search_act_type(local_mm_id,CM_ACT_TYPE_DATA_CALL) != NULL)
        {
          cmph_remove_orig_mode( local_mm_id, CM_ACT_TYPE_DATA_CALL, CM_ACT_PRIORITY_MAX );
        }
      }

      /* Delete Data call pref from priority queue, which might be there
      ** for 4 sec after end of data call
      */
      local_mm_id.stk_id = MM_STACK_0;
      if(cmtask_orig_para_search_act_type(local_mm_id,CM_ACT_TYPE_DATA_CALL) != NULL)
      {
        cmph_remove_orig_mode( local_mm_id, CM_ACT_TYPE_DATA_CALL, CM_ACT_PRIORITY_MAX );
      }
    } /* if(!default_data_subs) */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /* Force CS Only mode to NON DDS Subs */
    ph_sub_ptr = cmph_get_sub_ptr(curr_subs);
    if (ph_sub_ptr == NULL)
    {
      continue;
    }
    if ((pref_ptr = cm_pref_ptr()) == NULL) return;

    memscpy(pref_ptr, sizeof(cm_pref_s_type),
            &(ph_sub_ptr->pref_info), sizeof(cm_pref_s_type));
    pref_ptr->srv_domain_pref = CM_SRV_DOMAIN_PREF_CS_ONLY;

    (void)cmph_force_pref_on_the_fly_stack(
      cmph_map_sub_stk_to_id(curr_subs, MM_STACK_ALL),
      SD_SS_PREF_REAS_USER,
      CM_ACT_TYPE_PH_OBJ,
      pref_ptr,
      CM_OTASP_ACT_CODE_NONE,
      (cm_act_id_type)ph_ptr,
      CM_ACT_UPDATE_REAS_USER,
      FALSE,
      cmph_get_acq_pri_order_per_sub(curr_subs),
      CM_DEFAULT_VALUE);

    cm_mem_free (pref_ptr);
  }/* for(loop)*/

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#endif /* FEATURE_MMODE_DUAL_SIM */
}

/*===========================================================================

FUNCTION cmpmprx_enforce_device_policy

DESCRIPTION
  Force action for device policy masks
===========================================================================*/
EXTERN void cmpmprx_enforce_device_policy(
  policy_change_mask  mask
)
{
  policy_change_mask dev_policy_mask = cmpm_ptr()->device_policy_changes;
  boolean            sub_avail = FALSE;
  sys_modem_as_id_e_type local_asubs_id;
  cmph_sub_info_s_type *ph_sub_ptr = NULL;
  uint8 i = 0;

  CM_MSG_HIGH_2("cmpmprx_enforce_device_policy: dev_policy_mask %d, mask 0x%x", dev_policy_mask, mask);

  /* remove all objects with priority END */
  for(i = 0; i < cmpmprx_get_num_of_sims(); i++)
  {
    cmph_remove_orig_mode_with_priority_end_per_subs(i);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( local_asubs_id = SYS_MODEM_AS_ID_1;
       (IS_VALID_PH_SUB_PTR(local_asubs_id) && !sub_avail && local_asubs_id < MAX_SIMS);
       local_asubs_id++ )
  {
    if ((ph_sub_ptr = cmph_get_sub_ptr(local_asubs_id)) != NULL)
    {
      sub_avail |= ph_sub_ptr->is_3gpp2_subs_avail;
      sub_avail |= ph_sub_ptr->is_3gpp_subs_avail;
    }
  }

  /* Enforce device mode policy only if any subscription is available */
  /* Otherwise device policy will be picked up during subscription available procedure */
  if( cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_ONLINE
      && sub_avail )
  {

    if( dev_policy_mask & PM_CHG_DEVICE_MODE &&
        mask & PM_CHG_DEVICE_MODE )
    {
      cmpmprx_configure_policy_max_active();
    }

    if( dev_policy_mask & PM_CHG_MAX_ACTIVE_DATA  &&
        cmpmprx_get_num_of_active_data() == 1     &&
        mask & PM_CHG_MAX_ACTIVE_DATA )
    {
      cmpmprx_configure_policy_max_active_data();
    }
  }
  else
  {
    CM_MSG_HIGH_1("FORCE_POLICY: Device policy discarded. UE NOT Online or subs not avail, mask 0x%x", mask);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( dev_policy_mask & PM_CHG_MAX_MODE_CAP &&
      mask & PM_CHG_MAX_MODE_CAP)
  {
    uint8                  size = MAX_SIMS;
    prot_subsc_chg_e_type  sub_chg[MAX_SIMS];
    uint8 i = 0;

    memset(sub_chg,0,sizeof(sub_chg));
    for (i = 0; i < MIN(size, cmpmprx_get_num_of_sims()); i ++)
    {
      sub_chg[i] = PROT_SUBSC_CHG_ALL;
    }

    cmmsimc_send_subsc_chgd_current_pref(size, sub_chg);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmpm_ptr()->device_policy_changes &= (~mask);

  return;
}

/*===========================================================================

FUNCTION cmpmprx_init_pm_special_volte_e911_calls

DESCRIPTION
  This function is to read E911 specialization from PM.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void cmpmprx_init_pm_special_volte_e911_calls( void )
{
  cm_policy_config_s_type *pm_ptr           = cmpm_ptr();
  uint8                   i;
  /* In cmpmprx_init_pm_feature_mode policyman_get_items_block is already called which makes sure
         POLICYMAN  is initialized. no need call one nore time */

  for(i = 0 ; i < MAX_SIMS && i < pm_ptr->device_mode.nSims; i++)
  {
    if (FALSE == policyman_get_subs_boolean_value("mmcp:e911_volte", (sys_modem_as_id_e_type)i, 
                                                  &pm_ptr->sub_policy[i]->policy_items.special_volte_e911_calls))
    {
      CM_MSG_HIGH_1("INIT PMPRX: get special_volte_e911_calls from PM faied for as_id %d", i);
      pm_ptr->sub_policy[i]->policy_items.special_volte_e911_calls = FALSE;
  }
  else
  {
    /* Print feature mode */
      CM_MSG_HIGH_2("INIT PMPRX: as_id %d, special_volte_e911_calls %d", 
                     i, pm_ptr->sub_policy[i]->policy_items.special_volte_e911_calls);
  }

  /* update SD about the slpecialization item as well */
    sd_ss_update_is_special_volte_e911_calls(i, pm_ptr->sub_policy[i]->policy_items.special_volte_e911_calls);
  }

  return;
}
#ifdef FEATURE_MODEM_CONFIG_REFRESH

/*===========================================================================

FUNCTION cmpmprx_is_valid_mcfg_refresh

DESCRIPTION
  This function validates compatible subs feature mode changes

DEPENDENCIES
  none

RETURN VALUE
  TRUE if valid sfmode change
  FALSE otherwise

SIDE EFFECTS
  none

===========================================================================*/

static boolean cmpmprx_is_valid_mcfg_refresh (

  sys_subs_feature_t    current_feature_mode,
  /* Current subs feature mode */

  sys_subs_feature_t    new_feature_mode
  /* New subs feature mode */

)
{
  uint8 i;
  static const struct
  {

    sys_subs_feature_t    current_feature_mode;
    /* current feature mode */

    sys_subs_feature_t    new_feature_mode;
    /* new feature mode */

  } cmpmprx_feature_mode_change[] =
  {

    /* current feature mode */                     /* new feature mode */
    {SYS_SUBS_FEATURE_MODE_SVLTE,                  SYS_SUBS_FEATURE_MODE_SGLTE},
    {SYS_SUBS_FEATURE_MODE_SVLTE,                  SYS_SUBS_FEATURE_MODE_SRLTE},

    {SYS_SUBS_FEATURE_MODE_SRLTE,                  SYS_SUBS_FEATURE_MODE_SVLTE},
    {SYS_SUBS_FEATURE_MODE_SRLTE,                  SYS_SUBS_FEATURE_MODE_SGLTE},

    {SYS_SUBS_FEATURE_MODE_SGLTE,                  SYS_SUBS_FEATURE_MODE_SVLTE},
    {SYS_SUBS_FEATURE_MODE_SGLTE,                  SYS_SUBS_FEATURE_MODE_SRLTE},

	{SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE,         SYS_SUBS_FEATURE_MODE_SRLTE},
	{SYS_SUBS_FEATURE_MODE_SRLTE,                  SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE},
  };


  /* For multi sim device check for compatible feature mode changes
   */

  for( i=0; i < ARR_SIZE(cmpmprx_feature_mode_change); i++ )
  {
    if( cmpmprx_feature_mode_change[i].current_feature_mode == current_feature_mode &&
        cmpmprx_feature_mode_change[i].new_feature_mode   == new_feature_mode )
    {
      return TRUE;
    }
  }

  CM_MSG_HIGH_2("MCFG refresh: Incompatible feature mode change from %d to %d",
                current_feature_mode, new_feature_mode);

  return FALSE;
}

#endif //#ifdef FEATURE_MODEM_CONFIG_REFRESH

/*===========================================================================

FUNCTION cmpmprx_init_pm_feature_mode

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static boolean cmpmprx_init_pm_feature_mode( boolean is_mcfg_refresh )
{

  cm_policy_config_s_type *pm_ptr           = cmpm_ptr();
  boolean                 is_policy_valid   = FALSE;
  policyman_status_t      status            = POLICYMAN_STATUS_ERR;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  policyman_item_collection_t const   *pCollection;

  policyman_item_id_t     id = POLICYMAN_ITEM_DEVICE_CONFIGURATION;

  if (POLICYMAN_SUCCEEDED(policyman_get_items_block_msim((policyman_item_id_t const *)&id,
                          1,
                          &pCollection)
                         )
     )
  {
    sys_overall_feature_t new_overall_feature_mode = SYS_OVERALL_FEATURE_MODE_NORMAL;
    size_t                new_nSims                = 0;
    size_t                new_nMaxActive           = 0;
    size_t                new_nMaxActiveData       = 1;
    policyman_item_t const  *pItem = NULL;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /* Read the new overall Feature mode.
    */
    pItem = pCollection->pItems[0];
    status = policyman_device_config_overall_feature( pItem,
             &new_overall_feature_mode);
    if (POLICYMAN_SUCCEEDED(status))
    {
      if ( new_overall_feature_mode != SYS_OVERALL_FEATURE_MODE_MULTISIM )
      {
        new_nSims = new_nMaxActive = 1;
      }
      else
      {
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        /* If Feature mode is Multi-Sim, read other config items.
        */
        if(!POLICYMAN_SUCCEEDED(policyman_device_config_num_sims(pItem, &(new_nSims)))
            ||
            !POLICYMAN_SUCCEEDED(policyman_device_config_max_active_data(pItem, &(new_nMaxActiveData)))
            ||
            !POLICYMAN_SUCCEEDED(policyman_device_config_max_active(pItem, &(new_nMaxActive)))
          )
        {
          status = POLICYMAN_STATUS_ERR;
        }
      }
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    if (POLICYMAN_SUCCEEDED(status))
    {
      size_t i=0;

      if(is_mcfg_refresh)
      {
        /* During MCFG refresh on a multisim device when there is device mode change
        ** do not update cmpm ptr values and return after updating the mcfg status
        ** We expect reset in such scenarios
        */
#ifdef FEATURE_MODEM_CONFIG_REFRESH
        if(pm_ptr->device_mode.nSims != new_nSims)
        {
          pm_ptr->mcfg_refresh_status = MCFG_REFRESH_STATUS_INCOMPAT_DEVICE_MODE;
          CM_MSG_HIGH_2("Incompatible device mode change from %d to %d",pm_ptr->device_mode.nSims, new_nSims);
          policyman_item_collection_release(pCollection);
          return FALSE;
        }
#endif
      }

      cmpmprx_configure_num_policies(pm_ptr,new_nSims);

      pm_ptr->feature_mode                 = new_overall_feature_mode;
      pm_ptr->device_mode.nSims            = new_nSims;
      pm_ptr->device_mode.nMaxActive       = new_nMaxActive;
      pm_ptr->device_mode.nMaxActiveData   = new_nMaxActiveData;
      pm_ptr->seqno                        = pCollection->seqno;

      for (i=0; i<new_nSims ; i++)
      {
        sys_subs_feature_t          temp_sub_feature_mode;
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        /* Read the Sub Feature mode.
        */
        if(POLICYMAN_SUCCEEDED(policyman_device_config_get_subs_feature(pItem,0,i,&temp_sub_feature_mode)))
        {
#ifdef FEATURE_MODEM_CONFIG_REFRESH
          /* During MCFG refresh on a multisim device whenever there is
          ** incompatible feature mode change return without updating cmpm ptr values
          ** When MCFG refresh fails device reset will be triggered
          ** When device is DSSS mode allow feature mode changes to go through
          ** as the other sub is not active
          */
          if(is_mcfg_refresh && cmph_is_msim()&&
              (pm_ptr->sub_policy[i]->policy_items.sub_feature_mode != temp_sub_feature_mode) &&
              (cmph_ptr()->device_prop.msim_prop.active_subs != SYS_MODEM_AS_ID_1_MASK))
          {
            if (!cmpmprx_is_valid_mcfg_refresh(pm_ptr->sub_policy[i]->policy_items.sub_feature_mode,
                                               temp_sub_feature_mode))
            {
              pm_ptr->mcfg_refresh_status = MCFG_REFRESH_STATUS_INCOMPAT_FEATURE_MODE;
              policyman_item_collection_release(pCollection);
              return FALSE;
            }

          }
#endif
          pm_ptr->sub_policy[i]->policy_items.sub_feature_mode = temp_sub_feature_mode;
          pm_ptr->sub_policy[i]->debug_var.is_sub_feature_valid = TRUE;
        }
        else
        {
          pm_ptr->sub_policy[i]->policy_items.sub_feature_mode = SYS_SUBS_FEATURE_MODE_NORMAL;
          pm_ptr->sub_policy[i]->debug_var.is_sub_feature_valid = FALSE;
          status = POLICYMAN_STATUS_ERR;
        }
        pm_ptr->sub_policy[i]->debug_var.init_sub_feature_mode =
          pm_ptr->sub_policy[i]->policy_items.sub_feature_mode;
      }
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    if (POLICYMAN_SUCCEEDED(status))
    {
      size_t j=0;
      uint32 max_mode_cap = 0;
      /* Read Device max capabilities */

      for(j=0; j < new_nSims && !is_mcfg_refresh; j++)
      {
        policyman_status_t max_cap_policy_status =
          policyman_device_config_get_current_subphone_capability(pItem,
              j, &max_mode_cap);
        pm_ptr->sub_policy[j]->debug_var.max_cap_return = max_cap_policy_status;
        if(!POLICYMAN_SUCCEEDED(max_cap_policy_status))
        {
          status = POLICYMAN_STATUS_ERR;
          CM_MSG_HIGH_1("INIT PMPRX: MaxCap read error sim %d",j);
        }
        else
        {
          if(status != POLICYMAN_STATUS_ERR)
          {
            status = POLICYMAN_STATUS_SUCCESS;
          }
          pm_ptr->sub_policy[j]->policy_items.policy_max_mode_capability = max_mode_cap;
          pm_ptr->sub_policy[j]->debug_var.pm_max_mode_cap = max_mode_cap;
          CM_MSG_HIGH_2_ASID("INIT PMPRX: updating maxcap to 0x%x",j,max_mode_cap);
        }
      }
    }
    if(POLICYMAN_SUCCEEDED(status))
    {
      is_policy_valid = TRUE;
    }
  }

  policyman_item_collection_release(pCollection);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If num of sims from PM exceeds the MAX_SIMS supported in CM,
  ** default the policy.
  */
  if (pm_ptr->device_mode.nSims > MAX_SIMS      ||
      pm_ptr->device_mode.nMaxActive > MAX_SIMS ||
      pm_ptr->device_mode.nMaxActiveData > MAX_SIMS)
  {
    is_policy_valid = FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Error in reading the policy item(s). Discard everything and initialize to
  ** default values.
  */
  if (!is_policy_valid)
  {
    pm_ptr->feature_mode = SYS_OVERALL_FEATURE_MODE_NORMAL;
    cmpmprx_configure_num_policies(pm_ptr,1);
    pm_ptr->device_mode.nSims = 1;
    pm_ptr->device_mode.nMaxActive = 1;
    pm_ptr->device_mode.nMaxActiveData = 1;
    pm_ptr->seqno = 0;
  }

  /* Print feature mode */
  CM_MSG_HIGH_2("INIT PMPRX: is_policy_valid %d, Overall Feature_mode = %d",
                is_policy_valid, pm_ptr->feature_mode);

  return TRUE;
}

/*===========================================================================

FUNCTION cmpmprx_compare_policies

DESCRIPTION
  To compare policy for all subs.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: change in policy
  False: no change in policy.

SIDE EFFECTS
  none

===========================================================================*/
static boolean                    cmpmprx_compare_policies(

  cm_policy_config_s_type    *old_policy,

  cm_policy_config_s_type    *new_policy
)
{
  unsigned int i = 0;

  for(i=0; i<new_policy->device_mode.nSims; i++)
  {
    if(memcmp(&(old_policy->sub_policy[i]->policy_items),
              &(new_policy->sub_policy[i]->policy_items),
              sizeof(cm_policy_items_s_type)) != 0)
    {
      return TRUE;
    }
  }

  return FALSE;

}

/*===========================================================================

FUNCTION cmpmprx_read_pm_device_mode

DESCRIPTION
  Reads no. of SIM and max active SIMs.
===========================================================================*/

cm_policy_device_mode_s_type   cmpmprx_read_pm_device_mode(void)
{
  cm_policy_device_mode_s_type  device_mode;
  const policyman_item_t *pItem = NULL;
  const policyman_item_id_t id = POLICYMAN_ITEM_DEVICE_CONFIGURATION;
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();

  /* If non multi-sim, set the default value */
  if(pm_ptr->feature_mode != SYS_OVERALL_FEATURE_MODE_MULTISIM)
  {
    device_mode.nSims = 1;
    device_mode.nMaxActive = 1;
    device_mode.nMaxActiveData = 1;
  }
  else
  {
    if (policyman_get_items_block(&id, 1, &pItem)== POLICYMAN_STATUS_SUCCESS)
    {
      policyman_device_config_num_sims(pItem, &(device_mode.nSims));
      policyman_device_config_max_active(pItem, &(device_mode.nMaxActive));
      device_mode.nMaxActiveData = 1;
    }
    else
    {
      device_mode.nSims = 1;
      device_mode.nMaxActive = 1;
      device_mode.nMaxActiveData = 1;
    }
    policyman_item_release(pItem);
  }
  return device_mode;
}

/*===========================================================================

FUNCTION cmpmprx_alloc_local_policy_copy

DESCRIPTION
  Allocate Policy data structure and copy current policy into it.

DEPENDENCIES
  none

RETURN VALUE
  Allocated new policy structure.

SIDE EFFECTS
  none

===========================================================================*/
static cm_policy_config_s_type *cmpmprx_alloc_local_policy_copy()
{
  cm_policy_config_s_type *local_policy;
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();
  uint8 i;
  local_policy = (cm_policy_config_s_type *) cm_mem_malloc(sizeof(cm_policy_config_s_type));

  *local_policy = *pm_ptr;

  for (i=0; i<MIN(local_policy->device_mode.nSims,ARR_SIZE(local_policy->sub_policy)); i++)
  {
    local_policy->sub_policy[i] = (cm_sub_level_policy_s_type *) cm_mem_malloc(sizeof(cm_sub_level_policy_s_type));
    *(local_policy->sub_policy[i]) = *(pm_ptr->sub_policy[i]);
  }

  return local_policy;
}

/*===========================================================================

FUNCTION cmpmprx_update_curr_policy

DESCRIPTION
  Update cmpm_local with new policy retrieved from Policy Manager..

DEPENDENCIES
  cmpm_init()

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void                              cmpmprx_update_curr_policy(

  cm_policy_config_s_type    *local_policy

)
{
  if ( local_policy != NULL )
  {
    cm_policy_config_s_type *curr_policy = cmpm_ptr();
    uint8 sub =0;

    curr_policy->device_mode = local_policy->device_mode;
    curr_policy->device_policy_changes = local_policy->device_policy_changes;
    curr_policy->feature_mode = local_policy->feature_mode;
    curr_policy->is_pm_suspend = local_policy->is_pm_suspend;
#ifdef FEATURE_MODEM_CONFIG_REFRESH
    curr_policy->mcfg_refresh_status = local_policy->mcfg_refresh_status;
#endif
    curr_policy->seqno = local_policy->seqno;

    for(sub=0; cmutil_is_as_id_valid(sub) && sub < MAX_SIMS; sub++)
    {
      if(curr_policy->sub_policy[sub] != NULL && local_policy->sub_policy[sub] != NULL)
      {
        *(curr_policy->sub_policy[sub]) = *(local_policy->sub_policy[sub]);
      }
    }

  }
  return;
}

/*===========================================================================

FUNCTION cmpmprx_init_pm_ue_mode

DESCRIPTION
  Read sglte related configuration from Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void cmpmprx_init_pm_ue_mode(
  boolean is_pwr_up,
  uint8 refresh_subs_mask
)
{

  cm_policy_config_s_type *pm_ptr           = cmpm_ptr();
  boolean                 is_policy_valid   = FALSE;
  uint8                   i                 = 0;
  cm_policy_config_s_type *local_policy     = NULL;
  uint8                   size              = ARR_SIZE(pm_ptr->sub_policy);
  policy_change_mask      device_local_mask = 0;


#if !defined(CM_DEBUG) && !defined(TEST_FRAMEWORK)
  policy_change_mask      local_mask[3]     = {0,0,0};
  policyman_item_id_t     pIds[]            =
  {
    POLICYMAN_ITEM_RAT_CAPABILITY,
    POLICYMAN_ITEM_SERVICE_DOMAIN,
    POLICYMAN_ITEM_RF_BANDS,
    POLICYMAN_ITEM_SVC_MODE,
    POLICYMAN_ITEM_UE_MODE,
    POLICYMAN_ITEM_VOICE_DOMAIN_PREF,
    POLICYMAN_ITEM_RAT_ACQ_ORDER,
    POLICYMAN_ITEM_CALL_MODE,
    POLICYMAN_ITEM_DEVICE_CONFIGURATION,
  };

  size_t                  numIds            = ARR_SIZE(pIds);

  policyman_item_collection_t const  *pCollection;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT( pm_ptr != NULL );

  local_policy = cmpmprx_alloc_local_policy_copy();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Read the configuration from the policy manager. */
  if(POLICYMAN_SUCCEEDED(policyman_get_items_block_msim(pIds,numIds,&pCollection)))
  {
    is_policy_valid = TRUE;

    if( !POLICYMAN_SUCCEEDED(cmpmprx_retrieve_policy_item_data(local_policy,
                                                               pCollection,
                                                               local_mask,
                                                               &device_local_mask,
                                                               FALSE,
                                                               refresh_subs_mask)) )
    {
      is_policy_valid = FALSE;
    }
    else
    {
      cmpmprx_update_curr_policy(local_policy);
      for (i=0; i<(int)pm_ptr->device_mode.nSims; i++)
      {
        if (is_pwr_up)
        {
          local_mask[i] |= (PM_CHG_MODE_CAP | PM_CHG_BAND_CAP | PM_CHG_SVC_SCAN | PM_CHG_UE_MODE |
                            PM_CHG_VOICE_DOMAIN_PREF | PM_CHG_RAT_ACQ_ORDER | PM_CHG_CALL_MODE);
        }
        pm_ptr->sub_policy[i]->policy_changes |= local_mask[i];
      }
      pm_ptr->device_policy_changes |= device_local_mask;
    }
  }

  policyman_item_collection_release(pCollection);

#endif
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Error in reading the policy item(s). Discard everything and initialize to
  ** default values.
  */
  CM_MSG_HIGH_2("INIT PMPRX: is_policy_valid %d, is_pwr_up %d", is_policy_valid, is_pwr_up);
  pm_ptr->dbg_is_init_policy_valid = is_policy_valid;

  if (!is_policy_valid)
  {
    for(i = 0 ; i < pm_ptr->device_mode.nSims && i<size; ++i)
    {
      pm_ptr->sub_policy[i]->policy_items.policy_band_capability     = SYS_BAND_MASK_ANY;
      pm_ptr->sub_policy[i]->policy_items.policy_lte_band_capability = SYS_LTE_BAND_MASK_CONST_ANY;
      pm_ptr->sub_policy[i]->policy_items.policy_tds_band_capability = SYS_BAND_MASK_ANY;
      pm_ptr->sub_policy[i]->policy_items.service_scan_mode          = POLICYMAN_SVC_MODE_FULL;
      pm_ptr->sub_policy[i]->policy_items.ue_operation_mode          = SYS_UE_MODE_NORMAL;
      pm_ptr->sub_policy[i]->policy_items.call_mode                  = SYS_CALL_MODE_NORMAL;
      pm_ptr->sub_policy[i]->policy_items.policy_domain_capability = SYS_SRV_DOMAIN_CS_PS;

#if !defined(CM_DEBUG) && !defined(TEST_FRAMEWORK)
      if(i == 0)
      {
        pm_ptr->sub_policy[i]->policy_items.policy_max_mode_capability = SD_SS_MODE_PREF_ANY;
      }
      else
      {
        pm_ptr->sub_policy[i]->policy_items.policy_max_mode_capability = SD_SS_MODE_PREF_GSM;
      }
#endif
      pm_ptr->sub_policy[i]->policy_items.policy_mode_capability     = pm_ptr->sub_policy[i]->policy_items.policy_max_mode_capability;

      cmpmprx_reset_policy_chg_flag( i,PM_CHG_ALL );
    }
    /* No action need be taken to enforce this policy. So resetting all flags.
    */
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for (i = 0 ; i < pm_ptr->device_mode.nSims ; ++i)
  {
    if(is_pwr_up)
    {
      pm_ptr->sub_policy[i]->is_cm_eligible.cdma_subsc = FALSE;
      pm_ptr->sub_policy[i]->is_cm_eligible.gwl_subsc = FALSE;
    }
    else
    {
      CM_MSG_HIGH_0("INIT PMPRX: Not updating policy consumptin eligibility");
    }
    cmpmprx_print_current_policy(i);
  }

  cmpmprx_mem_free(local_policy);

  return;
}

/*===========================================================================

FUNCTION cmpmprx_init

DESCRIPTION
  Initialize the common functionality of Policy Manager Proxy object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmpmprx_init(
  boolean is_mcfg_refresh,
  uint8 refresh_subs_mask
)
{

  cm_policy_config_s_type *pm_ptr           = cmpm_ptr();

#ifdef FEATURE_MODEM_CONFIG_REFRESH
  pm_ptr->mcfg_refresh_status = MCFG_REFRESH_SUCCESS;
#endif

  /* Query feature_mode from policy mgr */
  /* During MCFG refresh when incompatible device mode/feature mode change is detected
  ** return without continuing with cmpmprx init
  */
  if(!cmpmprx_init_pm_feature_mode(is_mcfg_refresh))
  {
    return;
  }

#ifdef CM_DEBUG
  #error code not present
#endif

#ifdef TEST_FRAMEWORK
  #error code not present
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Set active subs to 0 in init, get max sub capability */
  if (!is_mcfg_refresh)
  {
    cmpm_active_subs = 0;

    /* Initialize is_pm_supsend */
    cmpm_ptr()->is_pm_suspend = FALSE;
    {
      uint8 sim_idx = 0;
      for(sim_idx = 0; 
        sim_idx < cmpmprx_get_num_of_sims() && sim_idx < MAX_SIMS && pm_ptr->sub_policy[sim_idx];
        sim_idx++)
      {
        cmpm_last_sim_state[sim_idx] = SYS_SIM_STATE_NONE;
      }
    }
  }

  cmpm_default_data_subs = -1;
  /* Retrieve the initial policy from the Policy Manager. */
  cmpmprx_init_pm_ue_mode(!is_mcfg_refresh, refresh_subs_mask);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmpmprx_init_pm_special_volte_e911_calls();

  /* Set is_dual_ims per max cap */
  {
    cmph_s_type    *ph_ptr  =  cmph_ptr();
    uint8 sim_idx = 0;
    uint8 count = 0;
    for (sim_idx = 0; 
        sim_idx < cmpmprx_get_num_of_sims() && sim_idx < MAX_SIMS && pm_ptr->sub_policy[sim_idx]; 
        sim_idx++)
    {
      if ((pm_ptr->sub_policy[sim_idx]->policy_items.policy_max_mode_capability & (uint32)SYS_SYS_MODE_MASK_LTE) 
            == (uint32)SYS_SYS_MODE_MASK_LTE)
      {
        count ++;
      }
    }
    if (count == 2 && cmpmprx_get_num_of_sims() == 2)
    {
      ph_ptr->device_prop.msim_prop.is_dual_lte = TRUE;
    }
    else
    {
      ph_ptr->device_prop.msim_prop.is_dual_lte = FALSE;
    }
  }

  return;
}

/*===========================================================================

FUNCTION cmpmprx_set_policy_chg_flag

DESCRIPTION
  Maintain the bitmask for what all policy configuration items have changed
  since last policy was enforced.

  If PM_CHG_NO_CHANGE passed to the function, clear the bitmask.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void cmpmprx_set_policy_chg_flag(

  sys_modem_as_id_e_type asubs_id,

  policy_change_mask change_value
)
{
  if( cmutil_is_as_id_valid(asubs_id))
  {
    cmpm_ptr()->sub_policy[asubs_id]->policy_changes |= change_value;
    CM_MSG_HIGH_3("cmpmprx_set_policy_chg_flag() on subs_id = %d change_value = %d policy_changes = %d",
                  asubs_id,change_value,cmpm_ptr()->sub_policy[asubs_id]->policy_changes);
  }
}

/*===========================================================================

FUNCTION cmpmprx_reset_policy_chg_flag

DESCRIPTION
  Maintain the bitmask for what all policy configuration items have changed
  since last policy was enforced.

  If PM_CHG_NO_CHANGE passed to the function, clear the bitmask.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void                       cmpmprx_reset_policy_chg_flag(

  sys_modem_as_id_e_type     asubs_id,

  policy_change_mask         change_value
)
{
  if( cmutil_is_as_id_valid(asubs_id) )
  {
    cmpm_ptr()->sub_policy[asubs_id]->policy_changes &= (~change_value);
    //CM_MSG_HIGH_3("cmpmprx_reset_policy_chg_flag() on subs_id = %d change_value = %d policy_changes = %d",asubs_id,change_value,cmpm_ptr()->sub_policy[asubs_id]->policy_changes);
  }
}

/*===========================================================================

FUNCTION cmpmprx_reset_device_policy_chg_flag

DESCRIPTION
  Maintain the bitmask for what all device policy configuration items have changed
  since last policy was enforced.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void                       cmpmprx_reset_device_policy_chg_flag(

  policy_change_mask         mask
)
{
  cmpm_ptr()->device_policy_changes &= (~mask);
}

/*===========================================================================

FUNCTION cmpmprx_is_mask_set

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_is_mask_set(

  sys_modem_as_id_e_type     asubs_id,

  policy_change_mask         check_mask


)
{
  cm_policy_config_s_type   *pm_ptr = cmpm_ptr();

  if ( !cmutil_is_as_id_valid(asubs_id) || pm_ptr->sub_policy[asubs_id] == NULL)
  {
    CM_MSG_HIGH_3("is_mask_set: check_mask 0x%x on subs_id %d nSim %d - invalid asubs_id",
                  check_mask,asubs_id,pm_ptr->device_mode.nSims);
    return FALSE;
  }

  if(pm_ptr->sub_policy[asubs_id]->policy_changes & check_mask)
  {
    return TRUE;
  }

  return FALSE;
}

/*===========================================================================

FUNCTION cmpmprx_is_device_mask_set

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_is_device_mask_set(

  policy_change_mask         check_mask,

  cm_policy_config_s_type   *pm_ptr
)
{
  if( pm_ptr == NULL )
  {
    pm_ptr = cmpm_ptr();
  }

  return ((pm_ptr->device_policy_changes & check_mask)== check_mask);
}


/*===========================================================================

FUNCTION cmpmprx_print_current_policy

DESCRIPTION
  Print the value of the configuration items of the current policy.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void cmpmprx_print_current_policy(sys_modem_as_id_e_type asubs_id)
{
  cm_sub_level_policy_s_type *sub_policy_ptr = cmpm_sub_ptr(asubs_id);

  if(sub_policy_ptr != NULL)
  {
    CM_MSG_HIGH_6_ASID("PMPRX: sfmode = %d, MAX_ALLOWED = 0x%x, RATs supported = 0x%x, Service scan mode = %d, call_mode =%d",
                       asubs_id,
                       sub_policy_ptr->policy_items.sub_feature_mode,
                       sub_policy_ptr->policy_items.policy_max_mode_capability,
                       sub_policy_ptr->policy_items.policy_mode_capability,
                       sub_policy_ptr->policy_items.service_scan_mode,
                       sub_policy_ptr->policy_items.call_mode);

    CM_MSG_HIGH_6("PMPRX: UE mode = %d Voice_domain_pref = 0x%x, is_voice_domain_pref_locked 0x%x, domain_cap = %d, policy_changes 0x%x, is_cm_eligible %d",
                  sub_policy_ptr->policy_items.ue_operation_mode,
                  sub_policy_ptr->policy_items.voice_domain_pref,
                  sub_policy_ptr->policy_items.is_voice_dom_pref_locked,
                  sub_policy_ptr->policy_items.policy_domain_capability,
                  sub_policy_ptr->policy_changes,
                  cmpmprx_read_cm_policy_consumption_eligibility(asubs_id));

    CM_MSG_HIGH_4("PMPRX: BANDs CGW = 0x%08x %08x. BANDs TDS = 0x%08x %08x",
                  QWORD_HIGH(sub_policy_ptr->policy_items.policy_band_capability),
                  QWORD_LOW(sub_policy_ptr->policy_items.policy_band_capability),
                  QWORD_HIGH(sub_policy_ptr->policy_items.policy_tds_band_capability),
                  QWORD_LOW(sub_policy_ptr->policy_items.policy_tds_band_capability));

    cm_print_lte_band_mask(sub_policy_ptr->policy_items.policy_lte_band_capability);
  }
  else
  {
    CM_MSG_HIGH_1("PMPRX: Unable to access policy for sub %d",asubs_id);
  }
}

/*===========================================================================

FUNCTION cmpmprx_retrieve_policy_item_data

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static policyman_status_t        cmpmprx_retrieve_policy_item_data
(
  cm_policy_config_s_type    *local_policy_ptr,

  policyman_item_collection_t const  *policy_item,

  policy_change_mask         *change_mask,

  policy_change_mask         *device_change_mask,

  boolean                     is_sandbox, 

  uint8                       refresh_subs_mask
)
{
  policyman_status_t      return_val = POLICYMAN_STATUS_ERR;
  sys_modem_as_id_e_type  sub_id = SYS_MODEM_AS_ID_NONE;
  uint8                   i=0, j=0;
  policyman_item_t const  *pItem;
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();
  policyman_item_id_t     item_id;
  cm_sub_level_policy_s_type  *local_sub_policy_ptr = NULL;
  cm_sub_level_policy_s_type  *sub_policy_ptr = NULL;

  CM_ASSERT( local_policy_ptr != NULL );
#ifndef CM_DEBUG
  CM_ASSERT( policy_item != NULL );
#endif

  /* Updating the policy sequence number */

  local_policy_ptr->seqno = policy_item->seqno;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for (i=0; i<policy_item->numItems; i++)
  {
    pItem = policy_item->pItems[i];
    item_id = policyman_item_get_id_msim(pItem,&sub_id);
    if(sub_id > SYS_MODEM_AS_ID_NONE && sub_id < MAX_SIMS && sub_id < (int)cmpmprx_get_num_of_sims())
    {
      local_sub_policy_ptr = local_policy_ptr->sub_policy[sub_id];
      sub_policy_ptr = cmpm_sub_ptr(sub_id);
    }
    else if(item_id != POLICYMAN_ITEM_DEVICE_CONFIGURATION)
    {
      CM_ERR_2("Policy change received on invalid sub item_id:%d sub_id:%d",item_id,sub_id);
      continue;
    }
    switch (item_id)
    {
      case POLICYMAN_ITEM_RAT_CAPABILITY:
        if(local_sub_policy_ptr != NULL && sub_policy_ptr != NULL)
        {
          return_val = policyman_get_rat_capability( pItem,
                       &(local_sub_policy_ptr->policy_items.policy_mode_capability));

          if (POLICYMAN_SUCCEEDED(return_val) &&
              cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_RAT_CAPABILITY, sub_id))
          {
            change_mask[sub_id] |= PM_CHG_MODE_CAP;
            CM_MSG_HIGH_3_ASID("PM->CM: mode_cap_chgd, new 0x%x current 0x%x",
                               sub_id,
                               local_sub_policy_ptr->policy_items.policy_mode_capability,
                               sub_policy_ptr->policy_items.policy_mode_capability);
          }
        }

        break;

      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_SERVICE_DOMAIN:
        if(local_sub_policy_ptr != NULL)
        {
          return_val = policyman_get_service_domain( pItem,
                       &(local_sub_policy_ptr->policy_items.policy_domain_capability));

          if (POLICYMAN_SUCCEEDED(return_val) &&
              cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_SERVICE_DOMAIN, sub_id))
          {
            change_mask[sub_id] |= PM_CHG_DOMAIN_CAP;
            CM_MSG_HIGH_2_ASID("PM->CM: domain_cap_chgd, new domain_cap=%d",
                               sub_id,
                               local_sub_policy_ptr->policy_items.policy_domain_capability);
          }
        }
        break;

      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_RF_BANDS:
        if(local_sub_policy_ptr != NULL)
        {
          return_val = policyman_get_rf_bands(pItem,
                                              &(local_sub_policy_ptr->policy_items.policy_band_capability),
                                              &(local_sub_policy_ptr->policy_items.policy_lte_band_capability),
                                              &(local_sub_policy_ptr->policy_items.policy_tds_band_capability));

          if ( POLICYMAN_SUCCEEDED(return_val) &&
               cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_RF_BANDS, sub_id))
          {
            change_mask[sub_id] |= PM_CHG_BAND_CAP;
            CM_MSG_HIGH_1("PM->CM[%d]: band_cap_chgd", sub_id);
          }
        }
        break;
      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_SVC_MODE:
        if(local_sub_policy_ptr != NULL && sub_policy_ptr != NULL)
        {
          return_val = policyman_get_svc_mode( pItem,
                                               &(local_sub_policy_ptr->policy_items.service_scan_mode));

          if ( POLICYMAN_SUCCEEDED(return_val) &&
               cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_SVC_MODE, sub_id))
          {
            change_mask[sub_id] |= PM_CHG_SVC_SCAN;
            CM_MSG_HIGH_3_ASID("PM->CM: srv_mode_chgd, srv_mode new %d current %d",
                               sub_id,
                               local_sub_policy_ptr->policy_items.service_scan_mode,
                               sub_policy_ptr->policy_items.service_scan_mode);
          }
        }
        break;
      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_UE_MODE:
        if(local_sub_policy_ptr != NULL && sub_policy_ptr != NULL)
        {
          return_val = policyman_get_ue_mode( pItem,
                                              &(local_sub_policy_ptr->policy_items.ue_operation_mode));

          if ( POLICYMAN_SUCCEEDED(return_val) &&
               cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_UE_MODE, sub_id))
          {
            change_mask[sub_id] |= PM_CHG_UE_MODE;
            CM_MSG_HIGH_3_ASID("PM->CM: ue_mode_chgd, ue_mode new %d current %d",
                               sub_id,
                               local_sub_policy_ptr->policy_items.ue_operation_mode,
                               sub_policy_ptr->policy_items.ue_operation_mode);
          }
        }
        break;
      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_DEVICE_CONFIGURATION:
      {
        size_t                new_nSims                = 0;
        size_t                new_nMaxActive           = 0;
        size_t                new_nMaxActiveData       = 0;
        uint32                max_mode_cap[MAX_SIMS];

        if(!POLICYMAN_SUCCEEDED(policyman_device_config_num_sims(pItem, &(new_nSims)))
            ||
            !POLICYMAN_SUCCEEDED(policyman_device_config_max_active_data(pItem, &(new_nMaxActiveData)))
            ||
            !POLICYMAN_SUCCEEDED(policyman_device_config_max_active(pItem, &(new_nMaxActive)))
          )
        {
          return_val = POLICYMAN_STATUS_ERR;
          CM_MSG_HIGH_0("MaxSIM numSIM nMaxActiveData read error");
        }
        else
        {
          return_val = POLICYMAN_STATUS_SUCCESS;
        }

        if (POLICYMAN_SUCCEEDED(return_val))
        {

          CM_MSG_HIGH_3("PM->CM: device_mode_chgd, nSIM %d nMaxActive %d nMaxActiveData %d",
                        new_nSims, new_nMaxActive, new_nMaxActiveData);

          if ( device_change_mask != NULL )
          {
            if ( local_policy_ptr->device_mode.nSims != new_nSims ||
                 local_policy_ptr->device_mode.nMaxActive != new_nMaxActive )
            {
              cmph_s_type  *ph_ptr       = cmph_ptr();
              /* If sub is deactivating and received device mode change do no enfore policy as
              ** we expect device reset to handle such device mode changes. This check is
              ** required only while processing deactivate_subs cmd.
              */
              if (cmph_get_subs_with_mcfg_refresh_ongoing())
              {
                CM_MSG_HIGH_1("Sub %d is deactivating do not enfore policy now",cmph_get_subs_with_mcfg_refresh_ongoing());
                return POLICYMAN_STATUS_ERR;
              }

              *device_change_mask = PM_CHG_DEVICE_MODE;
            }

            if( local_policy_ptr->device_mode.nMaxActiveData != new_nMaxActiveData )
            {
              *device_change_mask |= PM_CHG_MAX_ACTIVE_DATA;
            }
          }

          cmpmprx_configure_num_policies(local_policy_ptr,new_nSims);

          local_policy_ptr->device_mode.nSims          = new_nSims;
          local_policy_ptr->device_mode.nMaxActive     = new_nMaxActive;
          local_policy_ptr->device_mode.nMaxActiveData = new_nMaxActiveData;
        }

        if(cmpm_active_subs == SYS_MODEM_AS_ID_1_MASK ||
            cmpm_active_subs == SYS_MODEM_AS_ID_2_MASK ||
            cmpm_active_subs == 0)
        {
          /* Read emergency capability from PM */
          if(!POLICYMAN_SUCCEEDED(policyman_get_e911_subphone_capability(pItem, new_nSims, max_mode_cap)))
          {
            return_val = POLICYMAN_STATUS_ERR;
            CM_MSG_HIGH_1("E911 MaxCap: RETRIEVE read error, active subs %d",cmpm_active_subs);
          }
          else
          {
            CM_MSG_HIGH_3("E911 MaxCap: RETRIEVE success, nSims %d, size of max cap %d, active_subs %d",
                          pm_ptr->device_mode.nSims,
                          sizeof(max_mode_cap)/sizeof(uint32),
                          cmpm_active_subs);

            if (local_policy_ptr->device_mode.nSims == 2 && 
                (sizeof(max_mode_cap)/sizeof(uint32)) == 2 && 
                sub_policy_ptr != NULL)
            {
              CM_MSG_HIGH_3("E911 MaxCap: RETRIEVE success, max_mode_cap 0x%x  0x%x, current maxcap[0] 0x%x",
                            max_mode_cap[0], max_mode_cap[1],
                            sub_policy_ptr->policy_items.policy_max_mode_capability);
            }
            if(return_val != POLICYMAN_STATUS_ERR)
            {
              return_val = POLICYMAN_STATUS_SUCCESS;
            }
          }
        }
        else
        {
          /* Read Device max capabilities */
          for(j=0; j < new_nSims; j++)
          {
            if(!POLICYMAN_SUCCEEDED(policyman_device_config_get_current_subphone_capability(pItem,
                                    j, &(max_mode_cap[j]))))
            {
              return_val = POLICYMAN_STATUS_ERR;
              CM_MSG_HIGH_1("MaxCap: read error sim %d",j);
            }
            else
            {
              if (local_policy_ptr->device_mode.nSims == 2 &&
                  sizeof(max_mode_cap)/sizeof(uint32) == 2 && 
                  (sub_policy_ptr != NULL))
              {
                CM_MSG_HIGH_5_ASID("MaxCap: RETRIEVE success,  max_mode_cap 0x%x, nSims %d, active_subs %d, current maxcap[0] 0x%x",
                                   j, max_mode_cap[j],
                                   pm_ptr->device_mode.nSims, cmpm_active_subs,
                                   sub_policy_ptr->policy_items.policy_max_mode_capability
                                  );
              }

              if(return_val != POLICYMAN_STATUS_ERR)
              {
                return_val = POLICYMAN_STATUS_SUCCESS;
              }
            }
          }
        }

        for(j=0; j < new_nSims; j++)
        {
          if(max_mode_cap[j] != local_policy_ptr->sub_policy[j]->policy_items.policy_max_mode_capability)
          {
            local_policy_ptr->sub_policy[j]->policy_items.policy_max_mode_capability = max_mode_cap[j];
            if ( device_change_mask != NULL )
            {
              *device_change_mask |= PM_CHG_MAX_MODE_CAP;
            }
          }
        }
      }
      break;
      /*- - - - - - - - - -- - - - - - - - - - - - - - - - - -*/

      case POLICYMAN_ITEM_VOICE_DOMAIN_PREF:

        if(local_sub_policy_ptr != NULL)
        {
          CM_MSG_HIGH_2_EXT("Voice domain pref: refresh_subs_mask %d, sub_id %d",
                        refresh_subs_mask,
                        sub_id,
                        sub_id);
#ifdef FEATURE_CUST_2
          if(refresh_subs_mask & BM(sub_id))
#endif
          {
            /* During MCFG refresh, read voice domain pref from PM on sub with MCFG refresh only */
            return_val = policyman_get_voice_domain_pref(pItem,
                         &(local_sub_policy_ptr->policy_items.voice_domain_pref),
                         &(local_sub_policy_ptr->policy_items.is_voice_dom_pref_locked));
            if ( POLICYMAN_SUCCEEDED(return_val) &&
                 cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_VOICE_DOMAIN_PREF, sub_id))
            {
              change_mask[sub_id] |= PM_CHG_VOICE_DOMAIN_PREF;
              CM_MSG_HIGH_3_ASID("PM->CM: voice_domain_pref_chgd, new voice_domain_pref=%d is_locked=%d",
                                 sub_id,
                                 local_sub_policy_ptr->policy_items.voice_domain_pref,
                                 local_sub_policy_ptr->policy_items.is_voice_dom_pref_locked);
            }
          }
        }
        break;
      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/

      case POLICYMAN_ITEM_CALL_MODE:
        if(local_sub_policy_ptr != NULL && sub_policy_ptr != NULL)
        {
          return_val = policyman_get_call_mode_from_item(pItem,
                       &(local_sub_policy_ptr->policy_items.call_mode));

          if(POLICYMAN_SUCCEEDED(return_val))
          {
            change_mask[sub_id] |= PM_CHG_CALL_MODE;
            CM_MSG_HIGH_3_ASID("PM->CM: call_mode_chgd, call_mode new %d current %d",
                               sub_id, local_sub_policy_ptr->policy_items.call_mode,
                               sub_policy_ptr->policy_items.call_mode);

            /* If current value is different than the new one, send Volte Mode indication to Clients */
            cmemg_send_call_mode_ind(sub_id, local_sub_policy_ptr->policy_items.call_mode, FALSE);
            /*notify SD*/
            sd_ss_update_call_mode(sub_id, local_sub_policy_ptr->policy_items.call_mode);
          }
        }
        break;
      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_RAT_ACQ_ORDER:
        if(local_sub_policy_ptr != NULL && sub_policy_ptr != NULL)
        {
          CM_MSG_HIGH_1_ASID("PM->CM: tot_chgd",  sub_id);

          return_val = policyman_get_rat_order(pItem,
                                               &(local_sub_policy_ptr->policy_items.acq_order_pref));

          if(POLICYMAN_SUCCEEDED(return_val) &&
              cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_RAT_ACQ_ORDER, sub_id))
          {
            change_mask[sub_id] |= PM_CHG_RAT_ACQ_ORDER;

            CM_MSG_HIGH_6("PM -> CM: new rat_acq : list elements=%d %d %d %d %d %d",
                          local_sub_policy_ptr->policy_items.acq_order_pref.acq_order[0],
                          local_sub_policy_ptr->policy_items.acq_order_pref.acq_order[1],
                          local_sub_policy_ptr->policy_items.acq_order_pref.acq_order[2],
                          local_sub_policy_ptr->policy_items.acq_order_pref.acq_order[3],
                          local_sub_policy_ptr->policy_items.acq_order_pref.acq_order[4],
                          local_sub_policy_ptr->policy_items.acq_order_pref.acq_order[5]);

          }
        }
        break;

      default:
        return_val = POLICYMAN_STATUS_SUCCESS;
    }
  }

#ifdef FEATURE_MMODE_DUAL_SIM
  if(device_change_mask != NULL)
  {
  CM_MSG_HIGH_4("PM->CM: policy_chgd: num_items %d change_mask 0x%x, 0x%x, device_change_mask 0x%x",
                policy_item->numItems, change_mask[0], change_mask[1],
                *device_change_mask);
  }
#else
  CM_MSG_HIGH_2("PM->CM: policy_chgd: num_items %d change_mask 0x%x",
                policy_item->numItems, change_mask[0]);
#endif


  if(!is_sandbox)
  {
    cmpmprx_sanitize_max_and_curr_cap(local_policy_ptr);
  }

#ifndef FEATURE_CUST_2
  SYS_ARG_NOT_USED(refresh_subs_mask);
#endif

  return return_val;
}

/*===========================================================================

FUNCTION cmpmprx_fetch_mode_capability

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_fetch_mode_capability(

  sys_modem_as_id_e_type     asubs_id,

  sys_sys_mode_mask_e_type   *mode_capability_buffer
)
{
  boolean is_eligible = cmpmprx_read_cm_policy_consumption_eligibility(asubs_id);
  cm_sub_level_policy_s_type  *pm_sub_ptr = cmpm_sub_ptr(asubs_id);

  if (!(cmutil_is_as_id_valid(asubs_id) && asubs_id < (int)cmpmprx_get_num_of_sims()))
  {
    return FALSE;
  }

  if ( !cmpmprx_is_mask_set( asubs_id, PM_CHG_MODE_CAP)||
       mode_capability_buffer == NULL       ||
       !is_eligible ||
       cmpmprx_is_device_mask_set(PM_CHG_MAX_MODE_CAP,NULL)
     )
  {
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(pm_sub_ptr != NULL)
  {
    CM_MSG_HIGH_4("PMPRX->CMMSC[%d]: update mode_cap 0x%x->0x%x, change mask: 0x%x",
                  asubs_id,
                  cmmsc_get_msc_rat_cap(asubs_id),
                  pm_sub_ptr->policy_items.policy_mode_capability,
                  pm_sub_ptr->policy_changes);

#ifndef CM_DEBUG
    if (cmph_is_msim() && (cmph_ptr()->device_prop.msim_prop.active_subs & BM(asubs_id))
        && (pm_sub_ptr->policy_items.policy_mode_capability &
            ~((uint32)cmmsc_get_curr_msc_max_cap(asubs_id))) != 0)
    {
      CM_ERR_FATAL_3("PMPRX->CMMSC[%d]: maxcap 0x%x and mode 0x%x out of sync",
                     asubs_id,
                     cmmsc_get_curr_msc_max_cap(asubs_id),
                     pm_sub_ptr->policy_items.policy_mode_capability);
    }
#endif

    *mode_capability_buffer = pm_sub_ptr->policy_items.policy_mode_capability;
    cmpmprx_reset_policy_chg_flag( asubs_id, PM_CHG_MODE_CAP );

    return TRUE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return FALSE;
}

/*===========================================================================

FUNCTION cmpmprx_fetch_domain_capability

DESCRIPTION
  Fetch domain capability.

DEPENDENCIES
None

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_fetch_domain_capability(

  sys_modem_as_id_e_type  asubs_id,

  sys_srv_domain_e_type  *domain_capability_buffer
)
{
  boolean is_eligible = cmpmprx_read_cm_policy_consumption_eligibility(asubs_id);
  cm_sub_level_policy_s_type  *pm_sub_ptr = cmpm_sub_ptr(asubs_id);

  if (!(cmutil_is_as_id_valid(asubs_id) && asubs_id < (int)cmpmprx_get_num_of_sims()))
  {
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( !cmpmprx_is_mask_set( asubs_id, PM_CHG_DOMAIN_CAP)||
       domain_capability_buffer == NULL       ||
       !is_eligible ||
       cmpmprx_is_device_mask_set(PM_CHG_MAX_MODE_CAP,NULL)
     )
  {
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(pm_sub_ptr != NULL)
  {
    CM_MSG_HIGH_2("PMPRX->CMMSC[%d]: update domain_cap 0x%x",
                  asubs_id, pm_sub_ptr->policy_items.policy_domain_capability);

    *domain_capability_buffer = pm_sub_ptr->policy_items.policy_domain_capability;
    cmpmprx_reset_policy_chg_flag( asubs_id, PM_CHG_DOMAIN_CAP );

    return TRUE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return FALSE;
}

/*===========================================================================

FUNCTION cmpmprx_get_voice_domain_pref

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_get_voice_domain_pref(

  sys_modem_as_id_e_type       asubs_id,

  sys_voice_domain_pref_e_type *voice_domain_pref,

  boolean                      *is_voice_dom_pref_locked
)
{
  cm_sub_level_policy_s_type  *pm_sub_ptr = cmpm_sub_ptr(asubs_id);


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (cmutil_is_as_id_valid(asubs_id) && asubs_id < (int)cmpmprx_get_num_of_sims() &&
      pm_sub_ptr != NULL)
  {
    CM_MSG_HIGH_3("PMPRX->CMMSC[%d]: voice domain pref 0x%x, is_locked %d",
                  asubs_id,
                  pm_sub_ptr->policy_items.voice_domain_pref,
                  pm_sub_ptr->policy_items.is_voice_dom_pref_locked);

    *voice_domain_pref = pm_sub_ptr->policy_items.voice_domain_pref;
    *is_voice_dom_pref_locked = pm_sub_ptr->policy_items.is_voice_dom_pref_locked;
  }

  cmpmprx_reset_policy_chg_flag( asubs_id, PM_CHG_VOICE_DOMAIN_PREF);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return TRUE;
}

/*===========================================================================

FUNCTION cmpmprx_get_rat_acq_order_pref

DESCRIPTION
  Returns the acq_order_pref from the current PM policy

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
EXTERN boolean                    cmpmprx_get_rat_acq_order_pref(

  sys_modem_as_id_e_type         asubs_id,

  cm_acq_pri_order_pref_s_type   *rat_acq_order_ptr
)
{
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();
  uint8 i = 0;
  size_t sim_idx = 0;
  cm_sub_level_policy_s_type *sub_policy_ptr = cmpm_sub_ptr(asubs_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(sub_policy_ptr == NULL)
  {
    return FALSE;
  }

  CM_MSG_HIGH_2("PMPRX -> CM: rat_acq_order for asubs_id %d, num_rat %d",
                asubs_id, sub_policy_ptr->policy_items.acq_order_pref.num_rat);

  /* Check if num_rats from PM is valid */
  if(sub_policy_ptr->policy_items.acq_order_pref.num_rat > SYS_MAX_ACQ_PREF_RAT_LIST_NUM ||
      sub_policy_ptr->policy_items.acq_order_pref.num_rat <= 0)
  {
    CM_MSG_HIGH_1("rat_acq_order: invalid num_rat %d, setting default CGWHLT",
                  sub_policy_ptr->policy_items.acq_order_pref.num_rat);

    /* default rat_acq_order has 6 rats CGWHLT */
    rat_acq_order_ptr->num_rat = 6;

    rat_acq_order_ptr->acq_order[0] = SYS_SYS_MODE_CDMA;
    rat_acq_order_ptr->acq_order[1] = SYS_SYS_MODE_GSM;
    rat_acq_order_ptr->acq_order[2] = SYS_SYS_MODE_WCDMA;
    rat_acq_order_ptr->acq_order[3] = SYS_SYS_MODE_HDR;
    rat_acq_order_ptr->acq_order[4] = SYS_SYS_MODE_LTE;
    rat_acq_order_ptr->acq_order[5] = SYS_SYS_MODE_TDS;

    return FALSE;
  }
  else
  {

    CM_MSG_HIGH_6("rat_acq_order[0]: %d [1]: %d [2]: %d [3]: %d [4]: %d [5]: %d",
                  sub_policy_ptr->policy_items.acq_order_pref.acq_order[0],
                  sub_policy_ptr->policy_items.acq_order_pref.acq_order[1],
                  sub_policy_ptr->policy_items.acq_order_pref.acq_order[2],
                  sub_policy_ptr->policy_items.acq_order_pref.acq_order[3],
                  sub_policy_ptr->policy_items.acq_order_pref.acq_order[4],
                  sub_policy_ptr->policy_items.acq_order_pref.acq_order[5]);


    *rat_acq_order_ptr = sub_policy_ptr->policy_items.acq_order_pref;
  }

  for (sim_idx = 0; sim_idx < cmpmprx_get_num_of_sims() ; sim_idx++)
  {
    cmpmprx_reset_policy_chg_flag(sim_idx, PM_CHG_RAT_ACQ_ORDER);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return TRUE;
}

/*===========================================================================

FUNCTION cmpmprx_fetch_max_mode_capability

DESCRIPTION
  Fetch the max mode capability and reset the max mode cap mask.

DEPENDENCIES
None

RETURN VALUE
  uint32

SIDE EFFECTS
  none

===========================================================================*/
uint32    cmpmprx_fetch_max_mode_capability(

  sys_modem_as_id_e_type     asubs_id
)
{
  if(cmutil_is_as_id_valid(asubs_id))
  {
    cmmsc_state_machine_s_type *state_machine = cmmsimc_state_machine_ptr(asubs_id);
    uint32 cmpm_policy_max_mode_cap = cmpm_ptr()->sub_policy[asubs_id]->policy_items.policy_max_mode_capability;

    state_machine->stack_common_info.ue_max_mode_capability = cmpm_policy_max_mode_cap;
    cmpmprx_reset_device_policy_chg_flag(PM_CHG_MAX_MODE_CAP);
    return cmpm_policy_max_mode_cap;
  }
  else
  {
    return (uint32)cmutil_map_sd_mode_pref_to_sys_mode_mask(SD_SS_MODE_PREF_ALL_MODES);
  }
}

/*===========================================================================

FUNCTION cmpmprx_read_nMaxActiveData

DESCRIPTION
  Read nMaxActiveData

===========================================================================*/
uint8    cmpmprx_read_nMaxActiveData()
{
  return cmpm_ptr()->device_mode.nMaxActiveData;
}

/*===========================================================================

FUNCTION cmpmprx_read_call_mode_capability

DESCRIPTION
  Read the call mode capability.
  Note this should only be called during initialization. This does not reset the mask.

DEPENDENCIES
None

RETURN VALUE
  uint32

SIDE EFFECTS
  none

===========================================================================*/
void    cmpmprx_read_call_mode_capability(

  sys_modem_as_id_e_type     asubs_id,

  sys_call_mode_t            *call_mode_capability_buffer
)
{
  cm_policy_config_s_type       *pm_ptr = cmpm_ptr();

  if(cmutil_is_as_id_valid(asubs_id))
  {
    *call_mode_capability_buffer = cmpm_ptr()->sub_policy[asubs_id]->policy_items.call_mode;
  }
  else
  {
    *call_mode_capability_buffer = SYS_CALL_MODE_NORMAL;
  }

  CM_MSG_HIGH_1("read call mode %d",*call_mode_capability_buffer);
}

/*===========================================================================

FUNCTION cmpmprx_get_band_capability

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_get_band_capability(

  sys_modem_as_id_e_type     asubs_id,

  sys_band_mask_e_type       *band_capability_buffer,

  sys_lte_band_mask_e_type   *lte_band_capability_buffer,

  sys_band_mask_e_type       *tds_band_capability_buffer
)
{
  cm_policy_items_s_type *curr_policy_ptr = NULL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( !cmpmprx_is_mask_set( asubs_id,PM_CHG_BAND_CAP)   ||
       band_capability_buffer     == NULL     ||
       lte_band_capability_buffer == NULL     ||
       tds_band_capability_buffer == NULL     ||
       !cmpmprx_read_cm_policy_consumption_eligibility(asubs_id) ||
       cmpmprx_is_device_mask_set(PM_CHG_MAX_MODE_CAP,NULL)
     )
  {
    return FALSE;
  }

  curr_policy_ptr = &(cmpm_ptr()->sub_policy[asubs_id]->policy_items);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_MSG_HIGH_5("PMPRX->CMMSC[%d]: update BANDs CGW = 0x%08x %08x. BANDs TDS = 0x%08x %08x",
                asubs_id,
                QWORD_HIGH(curr_policy_ptr->policy_band_capability),
                QWORD_LOW(curr_policy_ptr->policy_band_capability),
                QWORD_HIGH(curr_policy_ptr->policy_tds_band_capability),
                QWORD_LOW(curr_policy_ptr->policy_tds_band_capability));

  cm_print_lte_band_mask(curr_policy_ptr->policy_lte_band_capability);

  *band_capability_buffer     = curr_policy_ptr->policy_band_capability;
  *lte_band_capability_buffer = curr_policy_ptr->policy_lte_band_capability;
  *tds_band_capability_buffer = curr_policy_ptr->policy_tds_band_capability;

  cmpmprx_reset_policy_chg_flag( asubs_id, PM_CHG_BAND_CAP );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return TRUE;
}

/*===========================================================================

FUNCTION cmpmprx_get_call_mode_capability

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean cmpmprx_get_call_mode_capability(
  sys_modem_as_id_e_type     asubs_id,

  sys_call_mode_t   *call_mode_capability_buffer
)
{

  if(cmutil_is_as_id_valid(asubs_id))
  {
    if ( !cmpmprx_is_mask_set(asubs_id, PM_CHG_CALL_MODE)||
         call_mode_capability_buffer == NULL       ||
         !cmpmprx_read_cm_policy_consumption_eligibility(asubs_id) ||
         cmpmprx_is_device_mask_set(PM_CHG_MAX_MODE_CAP,NULL)
       )
    {
      return FALSE;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    CM_MSG_HIGH_2("PMPRX->CMMSC[%d]: update Call Mode capability %d",asubs_id,
                  cmpm_ptr()->sub_policy[asubs_id]->policy_items.call_mode);

    *call_mode_capability_buffer = cmpm_ptr()->sub_policy[asubs_id]->policy_items.call_mode;
    cmpmprx_reset_policy_chg_flag(asubs_id, PM_CHG_CALL_MODE );

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    return TRUE;
  }
  else
  {
    CM_MSG_HIGH_0("Invalid subs id passed to get call mode capability");
  }
  return FALSE;
}


/*===========================================================================

FUNCTION cmpmprx_fetch_call_mode_capability

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean cmpmprx_fetch_call_mode_capability(

  sys_modem_as_id_e_type     asubs_id,

  sys_call_mode_t   *call_mode_capability_buffer
)
{

  if(cmutil_is_as_id_valid(asubs_id))
  {
    if ( call_mode_capability_buffer == NULL )
    {
      CM_MSG_MED_1("PMPRX->CMMSC[%d]: NO call mode capability update NULL Buffer", asubs_id);
      return FALSE;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    CM_MSG_HIGH_2("PMPRX->CMMSC[%d]: Call Mode capability %d",
                  asubs_id,
                  cmpm_ptr()->sub_policy[asubs_id]->policy_items.call_mode);

    *call_mode_capability_buffer = cmpm_ptr()->sub_policy[asubs_id]->policy_items.call_mode;
    cmpmprx_reset_policy_chg_flag(asubs_id, PM_CHG_CALL_MODE );
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    return TRUE;
  }
  else
  {
    CM_MSG_HIGH_0("Invalid subs id passed to fetch call mode capability");
  }
  return FALSE;
}



/*===========================================================================

FUNCTION cmpmprx_is_special_volte_e911_calls

DESCRIPTION
  Returns specialization value for SYS_SPECIAL_VOLTE_E911_CALLS.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean cmpmprx_is_special_volte_e911_calls(
    sys_modem_as_id_e_type asubs_id
)
{

  cm_policy_config_s_type *pm_ptr           = cmpm_ptr();
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!cmutil_is_as_id_valid(asubs_id))
  {
    return FALSE;
  }

  CM_MSG_LOW_2("PMPRX: asubs_id %d, special_volte_e911_calls %d", asubs_id, 
                pm_ptr->sub_policy[asubs_id]->policy_items.special_volte_e911_calls);

  return pm_ptr->sub_policy[asubs_id]->policy_items.special_volte_e911_calls;
}


/*===========================================================================

FUNCTION cmpmprx_is_ok_to_enforce_policy

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_is_ok_to_enforce_policy(

  cm_policy_config_s_type   *policy_ptr,

  sys_modem_as_id_e_type     asubs_id,
  
  boolean is_cleanup_needed
)
{
  uint8 i=0;

  if(policy_ptr == NULL)
  {
    policy_ptr = cmpm_ptr();
  }

  CM_ASSERT(policy_ptr != NULL);

  if (!cmutil_is_as_id_valid(asubs_id))
  {
    return FALSE;
  }

  /* If Subscription not available
  ** or operating mode not online
  ** or no policy change (not expected)
  */
  if (( (!cmpmprx_read_cm_policy_consumption_eligibility(asubs_id)||
         !(policy_ptr->sub_policy[asubs_id]->policy_changes & PM_CHG_ALL )
        )
        && policy_ptr->device_policy_changes == 0
      ) ||
      cmclnup_is_ongoing_subsc(BM(asubs_id))
     )
  {
    CM_MSG_HIGH_3_ASID("not_ok_to_enforce: due to Subsc not avail or No policy changed, device_policy_changes 0x%x, policy_changes 0x%x",
                       asubs_id,
                       policy_ptr->device_policy_changes,
                       policy_ptr->sub_policy[asubs_id]->policy_changes);
    return FALSE;
  }

  /* Check the UE Operation Mode. If the UE is OFFLINE,
  ** return FALSE. Mode OFFLINE_CDMA and OFFLINE_AMPS need
  ** to be checked here for Legecy compatibility because
  ** QXDM sends down OFFLINE as OFFLINE_CDMA */
  if( cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_OFFLINE      ||
      cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_OFFLINE_CDMA)
  {
    CM_MSG_HIGH_1( "not_ok_to_enforce: UE is OFFLINE. sub %d", asubs_id);
    return FALSE;
  }

  /* Check if not ok to enforce device policy changes
  */
  if(policy_ptr->device_policy_changes != 0 &&
      !cmpmprx_is_ok_to_enforce_device_policy(CM_CALL_ID_INVALID))
  {
    CM_MSG_HIGH_1("not_ok_to_enforce: device_policy_changes 0x%x during call/ECBM/paging",
                  policy_ptr->device_policy_changes);
    return FALSE;
  }

  /* Check if not ok to enforce sub policy changes
  */
  if((policy_ptr->sub_policy[asubs_id]->policy_changes & PM_CHG_ALL ) &&
      !cmpmprx_is_ok_to_enforce_sub_policy(asubs_id))
  {
    CM_MSG_HIGH_2("not_ok_to_enforce: sub %d policy_changes 0x%x during call/ECBM",
                  asubs_id,
                  policy_ptr->sub_policy[asubs_id]->policy_changes);
    return FALSE;
  }

  /* if any ongoing MPLMN search , do not enforce policy */
  if ( cmph_is_get_networks_ongoing(asubs_id))
  {
    CM_MSG_MED_1("not_ok_to_enforce: due to manual search in progress. sub %d", asubs_id);
    return FALSE;
  }

  /* No Voice calls/sms/cs data in traffic or emergency calls. End all the others and
  ** proceed with the UE mode change as per policy.
  */
  if(is_cleanup_needed)
  {
  cmcall_clean_cs_call_from_q(asubs_id);
  }

  if (  policy_ptr->sub_policy[asubs_id]->policy_changes & PM_CHG_UE_MODE)
  {
    return TRUE;
  }

  /* Service scan mode changed, UE mode not changed. */
  if (  policy_ptr->sub_policy[asubs_id]->policy_changes & PM_CHG_SVC_SCAN )
  {
    return TRUE;
  }

  if (  policy_ptr->sub_policy[asubs_id]->policy_changes & PM_CHG_MODE_CAP )
  {
    return TRUE;
  }

  if (policy_ptr->sub_policy[asubs_id]->policy_changes & PM_CHG_DOMAIN_CAP)
  {
    return TRUE;
  }

  if (policy_ptr->sub_policy[asubs_id]->policy_changes & PM_CHG_BAND_CAP )
  {
    return TRUE;
  }

  if (policy_ptr->sub_policy[asubs_id]->policy_changes & PM_CHG_VOICE_DOMAIN_PREF )
  {
    return TRUE;
  }

  if (policy_ptr->sub_policy[asubs_id]->policy_changes & PM_CHG_CALL_MODE )
  {
    return TRUE;
  }

  if (policy_ptr->sub_policy[asubs_id]->policy_changes & PM_CHG_RAT_ACQ_ORDER )
  {
    return TRUE;
  }

  if(policy_ptr->device_policy_changes != 0)
  {
    return TRUE;
  }

  CM_MSG_HIGH_1("not_ok_to_enforce: No conditions met. sub %d", asubs_id);
  return FALSE; /* Code execution should not come here. */
}

/*===========================================================================

FUNCTION cmpmprx_enforce_current_policy

DESCRIPTION
  Process the policy currently in cmpm_local.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void         cmpmprx_enforce_current_policy(

  sys_modem_as_id_e_type     asubs_id

)
{
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();
  cmph_s_type             *ph_ptr = cmph_ptr();
  boolean                 ue_mode_chgd = FALSE;
  cm_act_update_reas_e_type  act_update_reas = CM_ACT_UPDATE_REAS_USER;
  boolean force_top_pref = FALSE;
  mm_sub_stk_id_s_type    local_mm_id;
  cm_sub_level_policy_s_type *pm_sub_ptr = cmpm_sub_ptr(asubs_id);
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);

  if (ph_sub_ptr == NULL || pm_sub_ptr == NULL ||
      !(asubs_id >= SYS_MODEM_AS_ID_1 && asubs_id < MAX_SIMS)
     )
  {
    return;
  }

  local_mm_id.asubs_id = asubs_id;

  CM_MSG_HIGH_3_ASID("FORCE_POLICY: policy_changes 0x%x, device_policy_changes 0x%x",
                     asubs_id,
                     pm_sub_ptr->policy_changes,
                     pm_ptr->device_policy_changes);

  if ( !cmpmprx_is_ok_to_enforce_policy( pm_ptr, asubs_id, TRUE ) )
  {
    return;
  }

  /* remove all objects with priority END */
  cmph_remove_orig_mode_with_priority_end_per_subs(asubs_id);

  cmpmprx_print_current_policy(asubs_id);

  if ( cmpmprx_is_mask_set( asubs_id, PM_CHG_RAT_ACQ_ORDER ))
  {
    ph_sub_ptr->rat_acq_order_pref
      = pm_sub_ptr->policy_items.acq_order_pref;
  }

  if ( cmpmprx_is_mask_set( asubs_id, PM_CHG_MODE_CAP ) )
  {
    cmph_sp_subs_info_update();
  }

#if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
  if( cmph_is_subs_feature_mode_srlte(asubs_id) &&
      cmpmprx_is_mask_set(asubs_id, PM_CHG_UE_MODE) )
  {
    cmsoa_srlte_ue_mode_update(pm_sub_ptr->policy_items.ue_operation_mode, asubs_id);
    ue_mode_chgd = TRUE;
    #ifdef FEATURE_DOMAIN_SELECTION
    cmsds_reset_lte_removal_timer(asubs_id);
    #endif
  }
#endif

  if ( cmpmprx_is_mask_set( asubs_id, PM_CHG_VOICE_DOMAIN_PREF ) )
  {
    cmpmprx_enforce_voice_domain_pref(asubs_id, TRUE);
  }

  if ( cmpmprx_is_mask_set( asubs_id, PM_CHG_SVC_SCAN ) )
  {
    cmpmprx_enforce_service_scan_mode(asubs_id);
    cmph_sp_subs_info_update();
  }
  else if( cmpmprx_is_mask_set( asubs_id, PM_CHG_ALL ) )
  {
    /* If forcing is to be done on MAIN/HYBR_1/HYBR_2 combined */
    cm_orig_q_s_type  *top = NULL;
    cm_pref_s_type *pref_ptr = NULL;
    cm_act_type_e_type   act_type = CM_ACT_TYPE_PH_OBJ;
    cm_act_id_type       act_id = (cm_act_id_type)ph_ptr;
    cm_orig_q_s_type    *ph_obj_ptr = NULL;

    local_mm_id.stk_id = MM_STACK_0;
    ph_obj_ptr = cmtask_orig_para_search_act_type(local_mm_id, CM_ACT_TYPE_PH_OBJ);

    local_mm_id.stk_id = MM_STACK_ALL;
    top = cmtask_orig_para_get_top(local_mm_id);

    if (top == NULL)
    {
      return;
    }

    if((top->act_type == CM_ACT_TYPE_PH_OBJ ||
        top->act_type == CM_ACT_TYPE_POLICYMAN_RESTRICT) )
    {
      force_top_pref = TRUE;
      act_update_reas = CM_ACT_UPDATE_REAS_ACT_START;
    }

    if ((pref_ptr = cm_pref_ptr()) == NULL) return;
    memscpy(pref_ptr, sizeof(cm_pref_s_type),
            &(ph_sub_ptr->pref_info), sizeof(cm_pref_s_type));

    /* If the device is already in full rat mode and the capability is changed, we must
    ** recompute the net_sel_pref and continue to force POLICYMAN_RESTRICT. This will
    ** ensure that the fullrat_status and ORIG_MODE are correctly forced to MMOC/SD.
    */
    if(top->act_type == CM_ACT_TYPE_POLICYMAN_RESTRICT)
    {
      pref_ptr->network_sel_mode_pref =
        cmpmprx_compute_restricted_mode( asubs_id, pref_ptr->network_sel_mode_pref );

      if(ph_obj_ptr != NULL)
      {
        pref_ptr->mode_pref = ph_obj_ptr->orig->orig_mode_pref;
      }
      pref_ptr->srv_domain_pref = CM_SRV_DOMAIN_PREF_CS_ONLY;

      act_type = CM_ACT_TYPE_POLICYMAN_RESTRICT;
      act_id = (cm_act_id_type)pm_ptr;
    }

    CM_MSG_HIGH_5_ASID("FORCE_POLICY: >>PH FORCE_PREF: act_update_reas %d, force_top %d, top act_type %d, net_sel %d",
                       asubs_id,
                       act_update_reas,
                       force_top_pref,
                       top->act_type,
                       pref_ptr->network_sel_mode_pref);

    (void)cmph_force_pref_on_the_fly_stack(
      local_mm_id,
      SD_SS_PREF_REAS_USER,
      act_type,
      pref_ptr,
      CM_OTASP_ACT_CODE_NONE,
      act_id,
      act_update_reas,
      force_top_pref,
      cmph_get_acq_pri_order_per_sub(asubs_id),
      CM_DEFAULT_VALUE);

    cm_mem_free (pref_ptr);
  }

  /*Reset for rat _acq_order here itself*/
  cmpmprx_reset_policy_chg_flag(asubs_id,PM_CHG_RAT_ACQ_ORDER);

#ifdef FEATURE_DOMAIN_SELECTION
  if(cmph_is_subs_feature_mode_srlte(asubs_id) && ue_mode_chgd)
  {
    cmmsc_state_machine_s_type *state_machine = cmmsimc_state_machine_ptr(asubs_id);

    if( state_machine->stack_common_info.ue_mode
        == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED )
    {
      cmsds_sub_prop_s_type     *sds_sub_prop_ptr = cmsds_get_sub_prop_ptr(asubs_id);

      /* For SRLTE, when we receive ue mode change indication from PM with new
      ** ue mode=1xsrlte_csfb_pref(CSFB->SRLTE_CSFB_PREF transition) and we
      ** are already in stable 1x service, we should perform c2k switch from single to
      ** dual stack after ue mode change has been processed.
      ** It can happen in below scenario:
      ** 1.ue_mode=CSFB, and acquired home 1x service
      ** 2.Switch to dual stack wasn't done here was ue_mode=CSFB.
      ** 3.Indicated 1x service to PM.
      ** 4.PM sends ue mode change from CSFB to CSFB_PREF.
      ** 5.we should perform c2k switch from sinlge->dual stack here.
      ** */
#if (defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)) || defined(FEATURE_MMODE_DYNAMIC_SV_OPERATION_SWITCH)
      cmsoa_check_and_switch_fromc2k_to_srlte(asubs_id);
#endif

      if(sds_sub_prop_ptr != NULL &&
          sds_sub_prop_ptr->gen_sub_info.pending_uemode_plmn_block_skipped == TRUE)
      {
        sd_si_info_s_type     si_info;
        mm_sub_stk_id_s_type mm_id = cmph_map_sub_stk_to_id(asubs_id, 0);

        sd_si_info_ptr_get( cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0), &si_info); /*** NEED SD CHANGES FROM NAVEEN ***/

        sds_sub_prop_ptr->gen_sub_info.pending_uemode_plmn_block_skipped = FALSE;

        CM_MSG_HIGH_2("FORCE_POLICY: resume dom sel srv %d mode %d",si_info.srv_status,si_info.mode);
        if(si_info.srv_status == SYS_SRV_STATUS_SRV && si_info.mode == SYS_SYS_MODE_LTE)
        {
          cmsds_process_srv_ind(mm_id, &si_info);
        }
      }
    }
  }
#endif

}

/*===========================================================================

FUNCTION cmpmprx_process_retrieved_policy

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void                       cmpmprx_process_retrieved_policy(

  sys_modem_as_id_e_type     asubs_id,

  cm_policy_config_s_type    *local_policy,

  policy_change_mask         local_mask
)

{
  if ( local_policy != NULL &&
       asubs_id >= SYS_MODEM_AS_ID_1 &&
       asubs_id < MAX_SIMS &&
       asubs_id < (sys_modem_as_id_e_type)cmpmprx_get_num_of_sims() &&
       cmpm_ptr()->sub_policy[asubs_id])
  {
    CM_MSG_HIGH_3("cmpmprx_process_retrieved_policy() on subs_id = %d local_mask = %d policy_changes = %d",
                  asubs_id,local_mask,cmpm_ptr()->sub_policy[asubs_id]->policy_changes);
    local_policy->sub_policy[asubs_id]->policy_changes |= local_mask;
    cmpmprx_update_curr_policy(local_policy);
    cmpmprx_enforce_current_policy(asubs_id);
  }
}

/*===========================================================================

FUNCTION cmpmprx_process_msgr_msg

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static boolean                    cmpmprx_process_msgr_msg(

  cm_policyman_cfg_s         *rpt_ptr,

  cm_policy_config_s_type    *local_policy,

  policy_change_mask         *device_local_mask_ptr,

  boolean                     is_sandbox
  /* Ignore sequence number logic if policy data is from a PM sandbox
  */
)
{
  int                     i                   = 0;
  msgr_attach_s           *pAttach            = NULL;
  boolean                 is_new_policy_valid = TRUE;
  policy_change_mask      local_mask[3]       = {0,0,0};
  policy_change_mask      device_local_mask   = 0;
  cm_policy_config_s_type *curr_policy        = cmpm_ptr();
  boolean                 is_any_policy_chg   = FALSE;
  boolean                 is_policy_seq_invalid = FALSE;
  size_t                  local_seqno = cmpm_ptr()->seqno;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  CM_MSG_HIGH_2("PROC_MSGR: num_attach=%d, is_sandbox %d",
                 msgr_get_num_attach(&(rpt_ptr->msg_hdr)),
                 is_sandbox);

  *device_local_mask_ptr = 0;

  for (i = 0 ; i < msgr_get_num_attach(&(rpt_ptr->msg_hdr)) ; ++i)
  {
    policyman_item_collection_t const *policy_item = NULL;

    pAttach = msgr_get_attach(&(rpt_ptr->msg_hdr), i);
    if (pAttach == NULL || pAttach->type != MSGR_ATTACH_REFCNT)
    {
      CM_MSG_HIGH_1("PROC_MSGR: PMPRX: Invalid attachment %d, Abort new policy",i);
      is_new_policy_valid = FALSE;
      break;
    }

    policy_item = pAttach->data.refcnt.refcnt_obj_ptr;

    /* Ignore sequence number logic if policy data is from a PM sandbox */
    if(!is_sandbox)
    {
      if (policy_item->seqno <= curr_policy->seqno)
      {
        CM_MSG_HIGH_2("PROC_MSGR: current policy is already handled: policy_item->seqno= %d, curr_policy->seqno = %d",policy_item->seqno, curr_policy->seqno);
        return FALSE;
      }

      if(policy_item->seqno != curr_policy->seqno + 1)
      {
        is_policy_seq_invalid = TRUE;
        CM_MSG_HIGH_2("PROC_MSGR: CM-PM policy seq nums are out of Sync policy_item->seqno= %d, curr_policy->seqno = %d",
                      policy_item->seqno, curr_policy->seqno);
        break;
      }
    }

    if(!POLICYMAN_SUCCEEDED(cmpmprx_retrieve_policy_item_data(local_policy,
                            policy_item, local_mask, &device_local_mask, is_sandbox,
                            SYS_MODEM_AS_ID_ALL_ACTIVE_MASK)))
    {
      CM_MSG_HIGH_1("PROC_MSGR: PMPRX: Unable to retrieve data for attachment %d",i);
      /*As the sequence number would be also be valid in case retrieval policy fails*/
      local_seqno = policy_item->seqno;
      is_new_policy_valid = FALSE;
      break;
    }

    local_seqno = policy_item->seqno;
  } /* for() */

  if(!is_sandbox && is_policy_seq_invalid)
  {
    policyman_item_collection_t const *new_policy_item = policyman_get_items_since_sequence_number(curr_policy->seqno);

    if (new_policy_item == NULL)
    {
      CM_MSG_HIGH_0("PROC_MSGR: PMPRX: Invalid attachment %d, Abort new policy");
      is_new_policy_valid = FALSE;
    }
    else if(!POLICYMAN_SUCCEEDED(cmpmprx_retrieve_policy_item_data( local_policy,
                                 new_policy_item, local_mask, &device_local_mask, is_sandbox,
                                 SYS_MODEM_AS_ID_ALL_ACTIVE_MASK)))
    {
      CM_MSG_HIGH_1("PROC_MSGR: PMPRX: Unable to retrieve data for attachment %d",i);
      is_new_policy_valid = FALSE;
    }

    if(new_policy_item != NULL)
    {
      /* update the sequence number both in case retrieval is success or failure
      retrieve fail shall also update policy sequence number */
      local_seqno = new_policy_item->seqno;
      policyman_item_collection_release(new_policy_item);
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  is_any_policy_chg = (device_local_mask != 0);

  for (i=0; i<(int)local_policy->device_mode.nSims; i++)
  {
    if(i<3)
    {
      is_any_policy_chg = (is_any_policy_chg || (local_mask[i] != 0));
    }
    else
    {
      break;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( !is_new_policy_valid                   ||
       !is_any_policy_chg                     ||
       (device_local_mask == 0 &&
        (cmpmprx_compare_policies(local_policy,curr_policy) == 0)))
  {
    CM_MSG_HIGH_3("PROC_MSGR: PMPRX: Discard new policy valid = %d, is_any_policy_chg = %d device_local_mask %d",
                  is_new_policy_valid,is_any_policy_chg,device_local_mask);

    if(!is_sandbox)
    {
      cmpm_ptr()->seqno = local_seqno;
    }
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*Copy device level policy mask   */
  local_policy->device_policy_changes |= device_local_mask;
  *device_local_mask_ptr = device_local_mask;

  /* Copy change mask for each SIM */
  for (i=0; i<(int)local_policy->device_mode.nSims; i++)
  {
    local_policy->sub_policy[i]->policy_changes |= local_mask[i];
    CM_MSG_HIGH_4("PROC_MSGR: Msgr policy[%d] mask = 0x%x, device_policy 0x%x, 0x%x",
                  i,local_policy->sub_policy[i]->policy_changes,
                  device_local_mask, local_policy->device_policy_changes);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return TRUE;
}

/*===========================================================================

FUNCTION cmpmprx_policy_chg_ind_proc

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmpmprx_policy_chg_ind_proc(

  cm_policyman_cfg_s          *rpt_ptr
  /* Pointer to the report sent by Message Router */
)
{
  cm_policy_config_s_type *local_policy = NULL;
  uint8                   i=0;
  policy_change_mask      device_local_policy = 0;
  boolean                 is_policy_forced = FALSE;
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();
  uint8                   size = cmpmprx_get_num_of_sims();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_MSG_HIGH_0("PM->CM: MSGR RXD: policy_chg_ind");

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Loop thru the subs_ids and see if there are any pending policy.
  ** If any policy item has changed, attempt to enforce the new policy first before processing new
  ** policy.
  */
  if (pm_ptr->device_policy_changes == 0)
  {
    for (i=0; i<size; i++)
    {
      if (cmpmprx_is_mask_set( i, PM_CHG_ALL))
      {
        cmpmprx_enforce_current_policy(i);
      }
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( rpt_ptr == NULL )
  {
    CM_MSG_HIGH_0("ERROR!!! PM -> PMPRX: Incoming policy message NULL");
    return;
  }

  /* Allocate local buffer for new memory and initialize it to current policy.
  */
  local_policy = cmpmprx_alloc_local_policy_copy();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Retrieve the policy data into a local buffer. If all the policy items in
  ** the new policy are successfully retrieved, only then update the current
  ** policy and attempt to enforce it.
  */
  if (cmpmprx_process_msgr_msg(rpt_ptr,local_policy,&device_local_policy, FALSE))
  {
    uint32 prev_3gpp2_sub_mode_cap = 0;
    sys_modem_as_id_e_type gpp2_asubs_id = SYS_MODEM_AS_ID_1, other_sub = SYS_MODEM_AS_ID_1;
    uint32 saved_mode_cap[MAX_SIMS] = {0};
    uint8 i = 0;

    for (i = 0; i < MAX_SIMS && pm_ptr->sub_policy[i] != NULL; i ++ )
    {
      saved_mode_cap[i] = pm_ptr->sub_policy[i]->policy_items.policy_mode_capability;
    }

    cmpmprx_update_curr_policy(local_policy);

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /* Enforce device level policy */
    if(pm_ptr->device_policy_changes != 0)
    {
      if(cmpmprx_is_device_mask_set(PM_CHG_MAX_MODE_CAP,NULL))
      {
        if(cmpmprx_is_ok_to_enforce_device_policy(CM_CALL_ID_INVALID))
        {
          cmcall_end_active_data_calls_remove_orig_end();
          CM_MSG_HIGH_1("device_policy_chgd: MSGR, mask=0x%x",pm_ptr->device_policy_changes);
          cmpmprx_enforce_device_policy(PM_CHG_DEVICE_ALL);
        }
      }
      else
      {
        CM_MSG_HIGH_1("device_policy_chgd: MSGR, mask=0x%x",pm_ptr->device_policy_changes);
        cmpmprx_enforce_device_policy(PM_CHG_DEVICE_ALL);
      }
    }
    /* When this condition is false, we are currently processing the policy change
      ** indication from PM associated with a subscription change. PM has received the UIM
      ** event before CM, so we are processing that before processing the UIM event
      ** ourselves. So in this case, skip enforcing the new policy as it will be
      ** enforced next when CM receives the UIM event subscription change.
      */
    gpp2_asubs_id = cmmsc_get_1x_sub();
    if (gpp2_asubs_id >= SYS_MODEM_AS_ID_1 && gpp2_asubs_id < MAX_SIMS)
    {
      other_sub = cmph_get_other_sub(gpp2_asubs_id);
      prev_3gpp2_sub_mode_cap = saved_mode_cap[gpp2_asubs_id];
    }
    if(pm_ptr->device_mode.nSims > 1 &&
        (gpp2_asubs_id >= SYS_MODEM_AS_ID_1 && gpp2_asubs_id < MAX_SIMS) &&
        (other_sub >= SYS_MODEM_AS_ID_1 && other_sub < MAX_SIMS) &&
        pm_ptr->sub_policy[other_sub] != NULL &&
        !((!cmpmprx_is_ok_to_enforce_policy(cmpm_ptr(), gpp2_asubs_id, TRUE) &&
           ((prev_3gpp2_sub_mode_cap & SYS_SYS_MODE_MASK_HDR) ||
            ((prev_3gpp2_sub_mode_cap & SYS_SYS_MODE_MASK_CDMA) &&
             (prev_3gpp2_sub_mode_cap & SYS_SYS_MODE_MASK_LTE)))
          )
          &&
          (cmpmprx_is_ok_to_enforce_policy(cmpm_ptr(), other_sub, TRUE) &&
           (pm_ptr->sub_policy[other_sub]->policy_items.policy_mode_capability & (SYS_SYS_MODE_MASK_TDS)))
         ))
    {

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      /* Loop thru the subs_ids and see if any policy changed. If yes then enforce*/
      /* If any policy item has changed, attempt to enforce the new policy. */
      // Enforce current policy for non T rat SUB first followed by remaining subs
      if(!(pm_ptr->sub_policy[other_sub]->policy_items.policy_mode_capability &
           (SYS_SYS_MODE_MASK_TDS))
        )
      {
        is_policy_forced = TRUE;

        /* no T+3gpp2 conflict, enforce policy */
        if( cmpmprx_is_mask_set(other_sub, PM_CHG_ALL ) )
        {
          cmpmprx_enforce_current_policy(other_sub);
        }
      }

      for (i=0; i<size; i++)
      {
        if(is_policy_forced && (i == other_sub))
        {
          continue;
        }
        CM_MSG_HIGH_2("PMPRX: policy_chgd for sub: mask=0x%x, sim_idx=%d",
                      pm_ptr->sub_policy[i]->policy_changes, i);
        if( cmpmprx_is_mask_set( i, PM_CHG_ALL ) )
        {
          cmpmprx_enforce_current_policy(i);
        }
      }
    }
    else if (pm_ptr->device_mode.nSims == 1)
    {
      if( cmpmprx_is_mask_set(SYS_MODEM_AS_ID_1, PM_CHG_ALL ) )
      {
        cmpmprx_enforce_current_policy(SYS_MODEM_AS_ID_1);
      }
    }
    else
    {
      CM_MSG_HIGH_3("HDR/1x and L found in sub %d, and T found in sub %d, prev_3gpp2_sub_mode_cap 0x%x",
                    gpp2_asubs_id, other_sub, prev_3gpp2_sub_mode_cap);
    }
  }
 

#ifdef FEATURE_CM_DEBUG_BUFFER
  cmdbg_add_to_dbg_buffer(CM_BUFF_MSG_RTR_TYPE, CM_DBG_BUFF_MSGR_RTR_PM_CFG_UPDATE_IND, (void *)local_policy);
#endif

  cmpmprx_mem_free(local_policy);

  policyman_msgr_attachments_release(&(rpt_ptr->msg_hdr));

  return;
}

/*===========================================================================

FUNCTION cmpmprx_update_cm_policy_consumption_eligibility

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void   cmpmprx_update_cm_policy_consumption_eligibility_subsc(

  sys_modem_as_id_e_type          asubs_id,

  cm_policy_subsc_change_e_type   gwl_subsc,

  cm_policy_subsc_change_e_type   cdma_subsc
)
{
  boolean prev_eligibility = cmpmprx_read_cm_policy_consumption_eligibility(asubs_id);
  cm_policy_config_s_type *curr_policy = cmpm_ptr();
  cm_sub_level_policy_s_type *pm_sub_ptr = cmpm_sub_ptr(asubs_id);
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);

  if (pm_sub_ptr == NULL || ph_sub_ptr == NULL)
  {
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(gwl_subsc == CMPMPRX_SUBSC_AVAIL)
  {
    pm_sub_ptr->is_cm_eligible.gwl_subsc = TRUE;
  }
  else if(gwl_subsc == CMPMPRX_SUBSC_NOT_AVAIL)
  {
    pm_sub_ptr->is_cm_eligible.gwl_subsc = FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(cdma_subsc == CMPMPRX_SUBSC_AVAIL)
  {
    pm_sub_ptr->is_cm_eligible.cdma_subsc = TRUE;
  }
  else if(cdma_subsc == CMPMPRX_SUBSC_NOT_AVAIL)
  {
    pm_sub_ptr->is_cm_eligible.cdma_subsc = FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  CM_MSG_HIGH_3_ASID("PMPRX: Updating CM eligibility to consume policy: gwl %d, cdma %d",
                     asubs_id, gwl_subsc, cdma_subsc);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(cmpmprx_read_cm_policy_consumption_eligibility(asubs_id) &&
      cmpmprx_is_mask_set(asubs_id, PM_CHG_RAT_ACQ_ORDER)
    )
  {
    CM_MSG_HIGH_1("PMPRX: rat acq changed on sub:%d", asubs_id);
    ph_sub_ptr->rat_acq_order_pref
      = pm_sub_ptr->policy_items.acq_order_pref;
    cmpmprx_reset_policy_chg_flag(asubs_id,PM_CHG_RAT_ACQ_ORDER);
  }

  /* Check if ok to consume new policy items
  */
  if(!cmpmprx_is_ok_to_enforce_policy(curr_policy, asubs_id, TRUE))
  {
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* There are pending policy changes and CM just became eligible to consume
  ** Enforce service scan or voice domain pref. All others will be read by subscription change.
  */
  if ( !prev_eligibility &&
       cmpmprx_read_cm_policy_consumption_eligibility(asubs_id))
  {
    if(cmpmprx_is_mask_set( asubs_id, PM_CHG_SVC_SCAN ))
    {
      cmpmprx_enforce_service_scan_mode(asubs_id);
    }

    if(cmpmprx_is_mask_set( asubs_id, PM_CHG_VOICE_DOMAIN_PREF ))
    {
      cmpmprx_enforce_voice_domain_pref(asubs_id, FALSE);
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

}

/*===========================================================================

FUNCTION cmpmprx_update_cm_policy_consumption_eligibility

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_enforce_policy_oprt_mode(

  sys_modem_as_id_e_type     asubs_id
)
{
  cm_policy_config_s_type *curr_policy = cmpm_ptr();
  uint8 i=0;
  boolean is_no_op = TRUE;
  /*
  **Loop thru all the subs ids and check for all.
  */
  if (asubs_id == SYS_MODEM_AS_ID_MAX)
  {
    for (i=0; i<curr_policy->device_mode.nSims; i++)
    {
      if ( curr_policy->sub_policy[i]->policy_items.service_scan_mode != POLICYMAN_SVC_MODE_FULL )
      {
        cmpmprx_set_policy_chg_flag(i,PM_CHG_SVC_SCAN);
        is_no_op = FALSE;

        /* There are pending policy changes and CM just became eligible to consume*/
        if ( cmpmprx_read_cm_policy_consumption_eligibility(i) )
        {
          cmpmprx_enforce_current_policy(i);
        }
      }
    }
  }
  else
  {
    if (!cmutil_is_as_id_valid(asubs_id))
    {
      return FALSE;
    }

    if ( curr_policy->sub_policy[asubs_id]->policy_items.service_scan_mode != POLICYMAN_SVC_MODE_FULL )
    {
      cmpmprx_set_policy_chg_flag(asubs_id,PM_CHG_SVC_SCAN);
      is_no_op = FALSE;

      /* There are pending policy changes and CM just became eligible to consume*/
      if ( cmpmprx_read_cm_policy_consumption_eligibility(asubs_id) )
      {
        cmpmprx_enforce_current_policy(asubs_id);
      }
    }
  }

  return (!is_no_op);
}

/*===========================================================================

FUNCTION cmpmprx_resume_policy_manager

DESCRIPTION
  Send the Resume command to Policy Manager.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void                       cmpmprx_resume_policy_manager( void )
{
  CM_MSG_HIGH_0("PMPRX: resume PM");
  policyman_resume();
  cmpm_ptr()->is_pm_suspend = FALSE;
  return;
}

/*===========================================================================

FUNCTION cmpmprx_is_policy_manager_suspended

DESCRIPTION
  Return whether Policy Manager is currently suspended.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_is_policy_manager_suspended( void )
{
  return (cmpm_ptr()->is_pm_suspend);
}

/*===========================================================================

FUNCTION cmpmprx_enforce_sub_policies_avoid_rat_conflict

DESCRIPTION

  Enforces pending sub policies in correct order to avoid RAT concurrency
  issues. Specifically H+T is not supported so we will enforce policy on
  the sub without T first.

RETURN VALUE
  none

===========================================================================*/
void cmpmprx_enforce_sub_policies_avoid_rat_conflict()
{
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();
  sys_modem_as_id_e_type other_sub = cmph_get_other_sub(cmmsc_get_1x_sub());
  cm_sub_level_policy_s_type *pm_other_sub_ptr = cmpm_sub_ptr(other_sub);
  boolean is_policy_forced = FALSE;
  uint8 i = 0;

  if(pm_ptr->device_mode.nSims > 1)
  {
    if(pm_other_sub_ptr == NULL)
    {
      return;
    }

    if(!(pm_other_sub_ptr->policy_items.policy_mode_capability & SYS_SYS_MODE_MASK_TDS))
    {
      is_policy_forced = TRUE;
      if(cmpmprx_is_mask_set(other_sub, PM_CHG_ALL))
      {
        CM_MSG_HIGH_1("PMPRX: no T+3gpp2 conflict, enforcing policy on sub %d", other_sub);
        cmpmprx_enforce_current_policy(other_sub);
      }
    }
  }

  for(i = 0; i < cmpmprx_get_num_of_sims(); i++)
  {
    if(is_policy_forced && (i == other_sub))
    {
      continue;
    }

    if(cmpmprx_is_mask_set(i, PM_CHG_ALL))
    {
      cmpmprx_enforce_current_policy(i);
    }
  }
}
/*===========================================================================

FUNCTION cmpmprx_notify_call_end - remove_feature

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void                       cmpmprx_notify_call_end(

  sys_modem_as_id_e_type     asubs_id,

  boolean                    is_emerg
)
{
  uint8                   i=0;
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();

  if ( is_emerg == TRUE && pm_ptr->is_pm_suspend)
  {
    cmpmprx_resume_policy_manager();
  }

  if (!cmutil_is_as_id_valid(asubs_id))
  {
    return;
  }

  if (pm_ptr->sub_policy[i] != NULL || is_emerg || pm_ptr->sub_policy[i]->policy_changes != 0)
  {
    CM_MSG_HIGH_4_ASID("enforce policy is_em %d, policy_changes 0x%x, device_policy_changes 0x%x",
                       asubs_id, is_emerg,
                       pm_ptr->sub_policy[i]->policy_changes,
                       pm_ptr->device_policy_changes);
  }

  if(pm_ptr->device_policy_changes & PM_CHG_MAX_MODE_CAP)
  {
    CM_MSG_HIGH_1_ASID("Maxcap mask set, not force policy yet", asubs_id);
    return;
  }

  // Enforce current policy for non T rat SUB first followed by remaining subs
  cmpmprx_enforce_sub_policies_avoid_rat_conflict();
}

/*===========================================================================

FUNCTION cmpmprx_notify_emg_call_start

DESCRIPTION
  The function is called to notify cmpmprx that emergency call has started.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void  cmpmprx_notify_emg_call_start()
{
  /* Suspend Policy Manager so it will not process any rules for the duration
  ** of the emergency call.
  */
  if (!cmpmprx_is_policy_manager_suspended())
  {
    CM_MSG_HIGH_0("PMPRX: suspend PM");
    policyman_suspend();
    cmpm_ptr()->is_pm_suspend = TRUE;

  }
  else
  {
    CM_MSG_HIGH_0("PMPRX: PM already in suspend");
  }
}

/*===========================================================================

FUNCTION cmpmprx_read_cm_policy_consumption_eligibility

DESCRIPTION
  Initialize the Policy Manager object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_read_cm_policy_consumption_eligibility(

  sys_modem_as_id_e_type     asubs_id
)
{
  if( cmutil_is_as_id_valid(asubs_id))
  {
    cm_policy_elig_s_type *sub_elig = &(cmpm_ptr()->sub_policy[asubs_id]->is_cm_eligible);

    return (sub_elig->gwl_subsc || sub_elig->cdma_subsc );
  }
  return FALSE ;
}

/*===========================================================================

FUNCTION cmpmprx_compute_restricted_mode

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN cm_network_sel_mode_pref_e_type cmpmprx_compute_restricted_mode(

  sys_modem_as_id_e_type       asubs_id,

  cm_network_sel_mode_pref_e_type  net_sel_mode_pref
)
{
  cm_sub_level_policy_s_type *pm_sub_ptr = cmpm_sub_ptr(asubs_id);
  cm_network_sel_mode_pref_e_type  pm_net_sel_mode_pref = net_sel_mode_pref;

  if (!cmutil_is_as_id_valid(asubs_id) || pm_sub_ptr == NULL)
  {
    return net_sel_mode_pref;
  }

  switch (pm_sub_ptr->policy_items.service_scan_mode)
  {
    case POLICYMAN_SVC_MODE_LIMITED:
      pm_net_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_LIMITED_SRV;
      break;
    case POLICYMAN_SVC_MODE_CAMP_ONLY:
    {
      if ( net_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_MANUAL_CAMP_ONLY   ||
           net_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_MANUAL_LIMITED_SRV ||
           net_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_MANUAL )
      {
        pm_net_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_MANUAL_CAMP_ONLY;
      }
      else
      {
        pm_net_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_AUTO_CAMP_ONLY;
      }

    }
    break;

    case POLICYMAN_SVC_MODE_FULL_RAT:
    {
      if(!(SD_MODE_CONTAIN( pm_sub_ptr->policy_items.policy_mode_capability, SD_SS_MODE_PREF_CDMA_HDR))
        )
      {
        pm_net_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_LIMITED_SRV;
      }
      else
      {
        if ( net_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_MANUAL_CAMP_ONLY   ||
             net_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_MANUAL_LIMITED_SRV ||
             net_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_MANUAL )
        {
          pm_net_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_MANUAL_CAMP_ONLY;
        }
        else
        {
          pm_net_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_AUTO_CAMP_ONLY;
        }
      }
    }
    break;

    case POLICYMAN_SVC_MODE_FULL:
    default:
      break;
  } /* switch */

  if(net_sel_mode_pref != pm_net_sel_mode_pref)
  {
    CM_MSG_HIGH_3_EXT("cmpmprx_compute_restricted_mode: asubs_id %d, net_sel_mode_pref %d, srv_scan:%d",
                      asubs_id, net_sel_mode_pref, pm_sub_ptr->policy_items.service_scan_mode, (asubs_id+1));
  }
  return pm_net_sel_mode_pref;
}

/*===========================================================================

FUNCTION cmpmprx_enforce_service_scan_mode

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void                       cmpmprx_enforce_service_scan_mode(

  sys_modem_as_id_e_type     asubs_id
)
{
  cm_policy_config_s_type *curr_policy = cmpm_ptr();
  cmph_s_type             *ph_ptr = cmph_ptr();
  cm_pref_s_type *pref_ptr = NULL;
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);
  cm_sub_level_policy_s_type *pm_sub_ptr = cmpm_sub_ptr(asubs_id);

  if (ph_sub_ptr == NULL || pm_sub_ptr == NULL)
  {
    return;
  }

  if ( !cmpmprx_is_mask_set( asubs_id, PM_CHG_SVC_SCAN ) )
  {
    return;
  }

  if ((pref_ptr = cm_pref_ptr()) == NULL) return;
  memscpy(pref_ptr, sizeof(cm_pref_s_type),
          &(ph_sub_ptr->pref_info), sizeof(cm_pref_s_type));

  if (pm_sub_ptr->policy_items.service_scan_mode == POLICYMAN_SVC_MODE_FULL)
  {
    mm_sub_stk_id_s_type local_mm_id;
    local_mm_id.asubs_id = asubs_id;

    for ( local_mm_id.stk_id = MM_STACK_0;
          local_mm_id.stk_id < cmtask_get_num_stacks(asubs_id);
          local_mm_id.stk_id++ )
    {
      cm_orig_q_s_type *pm_rest_obj_ptr = cmtask_orig_para_search_act_type( local_mm_id,
                                          CM_ACT_TYPE_POLICYMAN_RESTRICT);
      if( pm_rest_obj_ptr != NULL )
      {
        cm_orig_q_s_type *ph_obj_ptr = cmtask_orig_para_search_act_type( local_mm_id,
                                       CM_ACT_TYPE_PH_OBJ);
        uint16 sys_sel_pref_req_id = (ph_obj_ptr == NULL)? CM_DEFAULT_VALUE:
                                     ph_obj_ptr->orig->sys_sel_pref_req_id;

        /* Update PHONE OBJ if CDMA_HDR is not supported, else retain HICPS
        ** preference if already enforced
        */
        CM_MSG_HIGH_5_EXT("FORCE_POLICY: DS: Sub %d, Exit full_rat_search mode, asubs_id=%d, stk_id=%d, pm mode_cap=%d, band_cap_sup_mode=%d",
                          asubs_id + 1, asubs_id, local_mm_id.stk_id,
                          pm_sub_ptr->policy_items.policy_mode_capability,
                          (int)(sd_ss_get_supp_mode_pref(
                              pm_sub_ptr->policy_items.policy_mode_capability,
                              pm_sub_ptr->policy_items.policy_band_capability,
                              pm_sub_ptr->policy_items.policy_lte_band_capability,
                              pm_sub_ptr->policy_items.policy_tds_band_capability)
                            & SD_SS_MODE_PREF_CDMA_HDR),
                            (asubs_id+1));

        /* Restore the force_top flag for calls still in silent redial. */
        cmcall_restore_force_pref_flag(asubs_id);

        if(! ( SD_MODE_CONTAIN( pm_sub_ptr->policy_items.policy_mode_capability, SD_SS_MODE_PREF_CDMA_HDR) &&
               ((int)(sd_ss_get_supp_mode_pref(
                        pm_sub_ptr->policy_items.policy_mode_capability,
                        pm_sub_ptr->policy_items.policy_band_capability,
                        pm_sub_ptr->policy_items.policy_lte_band_capability,
                        pm_sub_ptr->policy_items.policy_tds_band_capability)
                      & SD_SS_MODE_PREF_CDMA_HDR) != 0) ) )
        {

          (void)cmph_force_pref_on_the_fly_stack(
            local_mm_id,
            SD_SS_PREF_REAS_USER,
            CM_ACT_TYPE_PH_OBJ,
            pref_ptr,
            CM_OTASP_ACT_CODE_NONE,
            (cm_act_id_type) ph_ptr,
            CM_ACT_UPDATE_REAS_ACT_CONT,
            FALSE,
            cmph_get_acq_pri_order_per_sub(asubs_id),
            sys_sel_pref_req_id);
        }
        cmph_delete_update_orig_mode( local_mm_id,
                                      (cm_act_id_type) curr_policy,
                                      TRUE );

      }
	  cmpmprx_reset_policy_chg_flag(asubs_id,PM_CHG_SVC_SCAN);
    }
  }
  else
  {
	cmpmprx_reset_policy_chg_flag(asubs_id,PM_CHG_SVC_SCAN);
    pref_ptr->network_sel_mode_pref =
      cmpmprx_compute_restricted_mode( asubs_id, pref_ptr->network_sel_mode_pref );

    if ( cmpmprx_read_cm_policy_consumption_eligibility(asubs_id) )
    {
      mm_sub_stk_id_s_type local_mm_id = cmph_map_sub_stk_to_id( asubs_id,MM_STACK_0);

      cm_orig_q_s_type  *ph_obj_ptr
        = cmtask_orig_para_search_act_type(local_mm_id,CM_ACT_TYPE_PH_OBJ);

      local_mm_id.stk_id = MM_STACK_ALL;

      /* If HICPS has removed the 3GPP and Hybrid HDR acquisitions are ongoing
      ** we dont want to interrupt those. So instead of ph_ptr mode preference
      ** force the phone object mode preference.
      */
      if ( ph_obj_ptr != NULL )
      {
        pref_ptr->mode_pref = ph_obj_ptr->orig->orig_mode_pref;
      }
      pref_ptr->srv_domain_pref = CM_SRV_DOMAIN_PREF_CS_ONLY;

      /* Reset LTE removal timer and add LTE back when entering full rat search mode */
      #ifdef FEATURE_DOMAIN_SELECTION
      cmsds_reset_lte_removal_timer(asubs_id);
      #endif

      CM_MSG_HIGH_3("FORCE_POLICY: DS: SUB %d Enter full_rat_search mode, asubs_id=%d, mode_pref=%d",
                    asubs_id + 1, asubs_id, pref_ptr->mode_pref);

      /* Set the force_top flags for all ongoing calls to FALSE as all non-emergency
      ** calls will be below the Policy Manager object.
      ** This way Silent redial will not force unnecessary preferences onto SD while
      ** device is in Full RAT mode.
      */
      cmcall_set_force_pref_flag_false(asubs_id);

      /* Since full rat mode does only limited acq, forcing CS only as domain pref*/
      (void)cmph_force_pref_on_the_fly_stack(
        local_mm_id,
        SD_SS_PREF_REAS_USER,
        CM_ACT_TYPE_POLICYMAN_RESTRICT,
        pref_ptr,
        CM_OTASP_ACT_CODE_NONE,
        (cm_act_id_type) curr_policy,
        CM_ACT_UPDATE_REAS_ACT_START,
        TRUE,
        cmph_get_acq_pri_order_per_sub(asubs_id),
        CM_DEFAULT_VALUE);
    }
  }

  cm_mem_free (pref_ptr);
}

/*===========================================================================

FUNCTION cmpmprx_enforce_voice_domain_pref

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void                       cmpmprx_enforce_voice_domain_pref(

  sys_modem_as_id_e_type     asubs_id,

  boolean                    force_pref
)
{
  cmph_s_type             *ph_ptr = cmph_ptr();
  cm_orig_q_s_type        *stack_top_ptr = NULL;
  boolean                 is_force_top = FALSE;
  cmsds_sub_prop_s_type   *sds_sub_prop_ptr = cmsds_get_sub_prop_ptr(asubs_id);
  mm_sub_stk_id_s_type    local_mm_id;
  cm_pref_s_type *pref_ptr = NULL;
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);
  cm_sub_level_policy_s_type *pm_sub_ptr = cmpm_sub_ptr(asubs_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(!cmutil_is_as_id_valid(asubs_id) ||
      ph_sub_ptr == NULL || pm_sub_ptr == NULL)
  {
    return;
  }
  if(!cmpmprx_is_mask_set(asubs_id, PM_CHG_VOICE_DOMAIN_PREF))
  {
    return;
  }

  local_mm_id.asubs_id = asubs_id;
  local_mm_id.stk_id = MM_STACK_ALL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmpmprx_reset_policy_chg_flag(asubs_id,PM_CHG_VOICE_DOMAIN_PREF);

  #ifdef FEATURE_DOMAIN_SELECTION
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Reset VOPS status to send appropriate VOPS support to RM after TAU completion */

  if(sds_sub_prop_ptr == NULL)
  {
    return;
  }
    
  if(sds_sub_prop_ptr->gen_sub_config.voice_domain_pref != pm_sub_ptr->policy_items.voice_domain_pref)
  {
    sds_sub_prop_ptr->gen_sub_info.ims_sub_info.lte_ims_voice_avail = CMSDS_IMS_VOICE_NONE;
  }
  else if(sds_sub_prop_ptr->gen_sub_config.is_voice_domain_pref_locked != pm_sub_ptr->policy_items.is_voice_dom_pref_locked )
  {
    CM_MSG_HIGH_3("Update and Return: only is_locked flag changed for sub %d, sds_ptr %d, cmpm_ptr %d",
                  asubs_id,
                  sds_sub_prop_ptr->gen_sub_config.is_voice_domain_pref_locked,
                  pm_sub_ptr->policy_items.is_voice_dom_pref_locked);
    sds_sub_prop_ptr->gen_sub_config.is_voice_domain_pref_locked = pm_sub_ptr->policy_items.is_voice_dom_pref_locked;
    return;
  }
  else
  {
    CM_MSG_HIGH_3("Return: no change in voice domain pref for sub %d, sds_ptr %d, cmpm_ptr %d",
                  asubs_id,
                  sds_sub_prop_ptr->gen_sub_config.voice_domain_pref,
                  pm_sub_ptr->policy_items.voice_domain_pref);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  CM_MSG_HIGH_3(">>PH FORCE_PREF: voice_dom_pref for sub %d from PM, current: voice_dom_pref %d, is_locked %d",
                asubs_id,
                pm_sub_ptr->policy_items.voice_domain_pref,
                pm_sub_ptr->policy_items.is_voice_dom_pref_locked);

  sds_sub_prop_ptr->gen_sub_config.voice_domain_pref = pm_sub_ptr->policy_items.voice_domain_pref;
  sds_sub_prop_ptr->gen_sub_config.is_voice_domain_pref_locked = pm_sub_ptr->policy_items.is_voice_dom_pref_locked;

  *(CMPH_SS_VOICE_DOMAIN_PREF_PTR(asubs_id, ph_ptr)) = sds_sub_prop_ptr->gen_sub_config.voice_domain_pref;

  if(force_pref)
  {
    stack_top_ptr = cmtask_orig_para_get_top(local_mm_id);

    if(stack_top_ptr != NULL &&
        (stack_top_ptr->act_type == CM_ACT_TYPE_PH_OBJ ||
         stack_top_ptr->act_type == CM_ACT_TYPE_POLICYMAN_RESTRICT))
    {
      is_force_top      = TRUE;
    }

    if ((pref_ptr = cm_pref_ptr()) == NULL) return;

    memscpy(pref_ptr, sizeof(cm_pref_s_type),
            &(ph_sub_ptr->pref_info), sizeof(cm_pref_s_type));

    (void)cmph_force_pref_on_the_fly_stack(
      local_mm_id,
      SD_SS_PREF_REAS_USER,
      CM_ACT_TYPE_PH_OBJ,
      pref_ptr,
      CM_OTASP_ACT_CODE_NONE,
      (cm_act_id_type)ph_ptr,
      CM_ACT_UPDATE_REAS_USER,
      is_force_top,
      cmph_get_acq_pri_order_per_sub(asubs_id),
      CM_DEFAULT_VALUE);
    cm_mem_free (pref_ptr);
  }

  cmefs_fs_write(CMEFS_VOICE_DOMAIN_PREF,
                 (byte *)&sds_sub_prop_ptr->gen_sub_config.voice_domain_pref,
                 sizeof(sys_voice_domain_pref_e_type),
                 asubs_id);

  /* Notify clients of phone event.
  */
  cmph_event_per_subs( CM_PH_EVENT_SYS_SEL_PREF,asubs_id);

#endif /* FEATURE_DOMAIN_SELECTION */
}

/*===========================================================================

FUNCTION cmpmprx_unforce_policy

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void                       cmpmprx_unforce_policy(

  sys_modem_as_id_e_type     asubs_id

)
{
  mm_sub_stk_id_s_type mm_id = {asubs_id,MM_STACK_0};
  // boolean is_policy_device_mode = cmpmprx_is_mask_set(asubs_id, PM_CHG_DEVICE_MODE);

  cmpmprx_set_policy_chg_flag( asubs_id,PM_CHG_ALL );

  for ( mm_id.stk_id = MM_STACK_0; mm_id.stk_id < MM_STACK_ALL; mm_id.stk_id++ )
  {
    cm_orig_q_s_type *pm_rest_obj_ptr = cmtask_orig_para_search_act_type(mm_id,
                                        CM_ACT_TYPE_POLICYMAN_RESTRICT);
    if ( pm_rest_obj_ptr != NULL )
    {
      (void)cmtask_orig_para_delete(mm_id,pm_rest_obj_ptr,TRUE);
    }
  }
}

/*===========================================================================

FUNCTION cmpmprx_sys_sel_pref_proc

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void                         cmpmprx_sys_sel_pref_proc(
  sys_modem_as_id_e_type         asubs_id,
  cm_pref_s_type                *pref_ptr,
  cm_acq_pri_order_pref_s_type  *rat_acq_pri_order,
  uint16                         req_id
)
{
  cm_orig_q_s_type *pm_rest_obj_ptr
    = cmtask_orig_para_search_act_type(cmph_map_sub_stk_to_id(asubs_id,MM_STACK_0),
                                       CM_ACT_TYPE_POLICYMAN_RESTRICT);

  if ( pm_rest_obj_ptr == NULL )
  {
    return;
  }

  if (!cmutil_is_as_id_valid(asubs_id))
  {
    return;
  }

  /* Exit full rat mode if srv scan mode is changed */
  if (cmpmprx_is_mask_set(asubs_id, PM_CHG_SVC_SCAN))
  {
    CM_MSG_HIGH_0(">>PH FORCE_PREF: exit full rat mode");
    cmpmprx_notify_call_end(asubs_id, FALSE);
  }
  else if (pref_ptr)
  {
    pref_ptr->network_sel_mode_pref = cmpmprx_compute_restricted_mode(asubs_id, pref_ptr->network_sel_mode_pref);
    pref_ptr->srv_domain_pref = CM_SRV_DOMAIN_PREF_NO_CHANGE;

    cmph_update_user_ss_pref_in_full_rat(asubs_id,TRUE);
	
    CM_MSG_HIGH_0(">>PH FORCE_PREF: Force the user preferences on PolicyMan obj too");

    (void)cmph_force_pref_on_the_fly_stack(
      cmph_map_sub_stk_to_id(asubs_id, MM_STACK_ALL),
      SD_SS_PREF_REAS_USER,
      CM_ACT_TYPE_POLICYMAN_RESTRICT,
      pref_ptr,
      CM_OTASP_ACT_CODE_NONE,
      (cm_act_id_type)cmpm_ptr(),
      CM_ACT_UPDATE_REAS_ACT_START,
      FALSE,
      rat_acq_pri_order,
      req_id);

  }
}

/*===========================================================================

FUNCTION cmpmprx_get_ue_mode

DESCRIPTION
  Process config commands.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_get_ue_mode(

  sys_modem_as_id_e_type     asubs_id,

  sys_ue_mode_e_type         *ue_mode_buffer
)
{
  if (!cmutil_is_as_id_valid(asubs_id))
  {
    return FALSE;
  }

  if ( !cmpmprx_is_mask_set( asubs_id, PM_CHG_UE_MODE ) ||
       ue_mode_buffer == NULL       ||
       !cmpmprx_read_cm_policy_consumption_eligibility(asubs_id)
     )
  {
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_MSG_HIGH_2("PMPRX->CMMSC[%d]: update Policyman ue_mode=%d",asubs_id,
                cmpm_ptr()->sub_policy[asubs_id]->policy_items.ue_operation_mode);

  *ue_mode_buffer   = cmpm_ptr()->sub_policy[asubs_id]->policy_items.ue_operation_mode;
  cmpmprx_reset_policy_chg_flag( asubs_id, PM_CHG_UE_MODE );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return TRUE;
}

/*===========================================================================

FUNCTION cmpmprx_read_ue_mode

DESCRIPTION
  This API should be used only when
  cmmsc_init is not called yet .

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void                    cmpmprx_read_ue_mode(

  sys_modem_as_id_e_type     asubs_id,

  sys_ue_mode_e_type         *ue_mode_buffer
)
{
#ifdef CM_DEBUG
  #error code not present
#else

  if (!cmutil_is_as_id_valid(asubs_id))
  {
    return;
  }

  CM_MSG_HIGH_2("PMPRX -> CM[%d]: Policyman ue_mode=%d",asubs_id,
                cmpm_ptr()->sub_policy[asubs_id]->policy_items.ue_operation_mode);

  *ue_mode_buffer   = cmpm_ptr()->sub_policy[asubs_id]->policy_items.ue_operation_mode;
#endif
}


/*===========================================================================

FUNCTION cmpmprx_get_device_mode

DESCRIPTION
  Gets device mode which is decided based on no. of SIM and max SIMs that are
  active at a given time.

===========================================================================*/
sys_modem_device_mode_e_type cmpmprx_get_device_mode( void )
{
  cm_policy_config_s_type         *pm_info = cmpm_ptr();
  cm_policy_device_mode_s_type    pm_device_mode = pm_info->device_mode;
  sys_modem_device_mode_e_type  mapped_device_mode = SYS_MODEM_DEVICE_MODE_SINGLE_SIM;

#ifdef FEATURE_MMODE_TRIPLE_SIM
  if(pm_device_mode.nSims == 3)
  {
    mapped_device_mode = SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY;
  }
#endif

  if(pm_device_mode.nSims == 2)
  {
    if(pm_device_mode.nMaxActive == 2)
    {
      mapped_device_mode = SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE;
    }
    else
    {
      mapped_device_mode = SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY;
    }
  }

  pm_info->device_policy_changes &= (~PM_CHG_DEVICE_MODE);

  return mapped_device_mode;
}

/*===========================================================================

FUNCTION cmpmprx_get_num_of_sims

DESCRIPTION
  Get num of sims supported base on Policy manager configuration item.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif

size_t  cmpmprx_get_num_of_sims( void )
{
  return (cmpm_ptr()->device_mode.nSims);
}

/*===========================================================================

FUNCTION cmpmprx_get_num_of_active_data

DESCRIPTION
  Get num of active data supported base on Policy manager configuration item.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
size_t  cmpmprx_get_num_of_active_data( void )
{
  cm_policy_config_s_type           *pm_info = cmpm_ptr();
  return (pm_info->device_mode.nMaxActiveData);
}

/*===========================================================================

FUNCTION cmpmprx_rread_device_mode

DESCRIPTION
  Added for the purpose of debugging in case we need to re-read device mode
===========================================================================*/
void cmpmprx_rread_device_mode( void )
{
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();
  pm_ptr->device_mode = cmpmprx_read_pm_device_mode();
}

/*===========================================================================

FUNCTION cmpmprx_check_sys_sel_pref_policy_conflict

DESCRIPTION
  If the new system selection preferences cause a policy change, check if it
  in conflict with current device state.
  If policy change results in UE mode change and voice call ongoing, then
  mark conflict as TRUE

DEPENDENCIES
  none

RETURN VALUE
  TRUE: New policy retrieved and updated.
  False: No change in policy.

SIDE EFFECTS
  none

===========================================================================*/
cm_ph_cmd_err_e_type              cmpmprx_check_sys_sel_pref_policy_conflict(

  cm_ph_cmd_s_type           *ph_cmd_ptr
)
{
  cm_policyman_cfg_s      *pm_msgr_rsp  = NULL;
  msgr_attach_s           *pAttach            = NULL;
  int                      i = 0;
  uint8                    j = 0;
  policy_change_mask       change_mask = 0;
  cm_sys_sel_pref_params_s_type pref_info;
  cm_ph_cmd_info_s_type    *cmd_info_ptr;
  sys_modem_as_id_e_type    asubs_id;
  cmph_s_type              *ph_ptr = cmph_ptr();
  cm_ph_cmd_err_e_type    return_val = CM_PH_CMD_ERR_NOERR;
  cmph_sub_info_s_type *ph_sub_ptr = NULL;
  policyman_sandbox_t *pSandbox = NULL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmd_info_ptr = CMD_INFO_PTR( ph_cmd_ptr );
  asubs_id = cmd_info_ptr->cmd_subs;
  ph_sub_ptr = cmph_get_sub_ptr(asubs_id);

  /* Ensure that the subscription id is valid.
  */
  if (ph_sub_ptr == NULL)
  {
    CM_MSG_HIGH_1("PMPRX: sys_sel_pref on invalid sub %d default to sub1",asubs_id);
    asubs_id = SYS_MODEM_AS_ID_1;
    ph_sub_ptr = cmph_get_sub_ptr(asubs_id);
  }

  /* Call into Policy manager and retrieve new policy if needed.
  ** Use the same preferences as used in the force_pref_on_fly() below.
  */
  pref_info.mode_pref        = cmd_info_ptr->mode_pref;
  if (ph_sub_ptr)
  {
    pref_info.term_pref        = ph_sub_ptr->pref_info.pref_term;
  }
  pref_info.pref_duration    = cmd_info_ptr->pref_duration;
  pref_info.band_pref        = cmd_info_ptr->band_pref;
  pref_info.lte_band_pref    = cmd_info_ptr->lte_band_pref;
  pref_info.tds_band_pref    = cmd_info_ptr->tds_band_pref;
  pref_info.prl_pref         = cmd_info_ptr->prl_pref;
  pref_info.roam_pref        = cmd_info_ptr->roam_pref;
  pref_info.hybr_pref        = cmd_info_ptr->hybr_pref;
  pref_info.srv_domain_pref  = cmd_info_ptr->srv_domain_pref;
  pref_info.network_sel_mode_pref = cmd_info_ptr->network_sel_mode_pref;
  /* this is to avoid sending null PLMN values to PM when network sel mode is set to no_change from manual */
  if( cmd_info_ptr->network_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_MANUAL &&
      ( sys_plmn_id_is_null(cmd_info_ptr->plmn) || sys_plmn_id_is_undefined(cmd_info_ptr->plmn)) &&
      ph_sub_ptr
    )
  {
    pref_info.plmn_ptr       = &(ph_sub_ptr->pref_info.plmn);
  }
  else
  {
    pref_info.plmn_ptr       = &(cmd_info_ptr->plmn);
  }

  //pref_ptr->rat_acq_order_pref_ptr = &rat_acq_pri_order;
  pref_info.csg_id           = cmd_info_ptr->csg_id;
  pref_info.csg_rat          = cmd_info_ptr->csg_rat;
  pref_info.ue_usage_setting = CMPH_SS_UE_USAGE_SETTING(asubs_id, ph_ptr);
  pref_info.voice_domain_pref = cmd_info_ptr->voice_domain_pref;
  pref_info.lte_disable_cause = cmd_info_ptr->lte_disable_cause;
  pref_info.volte_enable_state = CM_VOLTE_NO_CHANGE;
  pref_info.is_allow_buffering_cmd = cmd_info_ptr->is_allow_buffering_cmd;

  if (cmd_info_ptr->srv_domain_pref == CM_SRV_DOMAIN_PREF_NO_CHANGE)
  {
    pref_info.srv_domain_pref = CMPH_SS_SRV_DOMAIN_PREF( asubs_id, ph_ptr );
  }

  if(pref_info.srv_domain_pref == CM_SRV_DOMAIN_PREF_PS_ATTACH)
  {
    if( CMPH_SS_SRV_DOMAIN_PREF(asubs_id,ph_ptr) == CM_SRV_DOMAIN_PREF_CS_ONLY  ||
        CMPH_SS_SRV_DOMAIN_PREF(asubs_id,ph_ptr) == CM_SRV_DOMAIN_PREF_CS_PS )
    {
      pref_info.srv_domain_pref = CM_SRV_DOMAIN_PREF_CS_PS;
    }
    else if ( CMPH_SS_SRV_DOMAIN_PREF(asubs_id,ph_ptr) == CM_SRV_DOMAIN_PREF_NONE ||
              CMPH_SS_SRV_DOMAIN_PREF(asubs_id,ph_ptr) == CM_SRV_DOMAIN_PREF_PS_ONLY )
    {
      pref_info.srv_domain_pref = CM_SRV_DOMAIN_PREF_PS_ONLY;
    }
  }

  if(pref_info.srv_domain_pref == CM_SRV_DOMAIN_PREF_PS_DETACH)
  {
    if( CMPH_SS_SRV_DOMAIN_PREF(asubs_id,ph_ptr) == CM_SRV_DOMAIN_PREF_CS_ONLY ||
        CMPH_SS_SRV_DOMAIN_PREF(asubs_id,ph_ptr) == CM_SRV_DOMAIN_PREF_CS_PS )
    {
      pref_info.srv_domain_pref = CM_SRV_DOMAIN_PREF_CS_ONLY;
    }
    else if ( CMPH_SS_SRV_DOMAIN_PREF(asubs_id,ph_ptr) == CM_SRV_DOMAIN_PREF_NONE ||
              CMPH_SS_SRV_DOMAIN_PREF(asubs_id,ph_ptr) == CM_SRV_DOMAIN_PREF_PS_ONLY)
    {
      pref_info.srv_domain_pref = CM_SRV_DOMAIN_PREF_NONE;
    }
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the new preferences to Policy Manager and receive the new policy
  ** if it is changed.
  */
  pSandbox = policyman_create_sandbox();

  pm_msgr_rsp = (cm_policyman_cfg_s *)
                policyman_report_preference_change( asubs_id, &pref_info, pSandbox );


  if ( pm_msgr_rsp == NULL )
  {
    CM_MSG_MED_0("PMPRX: New preferences have conflict in Policy Manager, Rsp NULL");

    /* For UT verification*/
#ifdef CM_DEBUG
    #error code not present
#endif
    REF_CNT_OBJ_RELEASE_IF(pSandbox);
    return return_val;
  }

  for (i = 0 ; i < msgr_get_num_attach(&(pm_msgr_rsp->msg_hdr)) ; ++i)
  {
    policyman_item_collection_t const *policy_item = NULL;
    pAttach = msgr_get_attach(&(pm_msgr_rsp->msg_hdr), i);
    if (pAttach == NULL || pAttach->type != MSGR_ATTACH_REFCNT)
    {
      CM_MSG_HIGH_1("PMPRX: Invalid attachment %d.Abort new policy",i);
      change_mask = PM_CHG_CHANGE_NONE;
      break;
    }

    policy_item = pAttach->data.refcnt.refcnt_obj_ptr;

    for (j = 0 ; j < policy_item->numItems ; j++)
    {
      policyman_item_t const *pItem = policy_item->pItems[j];
      switch (policyman_item_get_id_msim(pItem,&asubs_id))
      {
        /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
        case POLICYMAN_ITEM_UE_MODE:
          change_mask |= PM_CHG_UE_MODE;
          break;
        /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
        case POLICYMAN_ITEM_DEVICE_CONFIGURATION:
          change_mask |= PM_CHG_DEVICE_MODE;
          break;

        /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
        default:
          break;
      }
    }
  }

  policyman_report_preference_msg_free( (msgr_hdr_s *)pm_msgr_rsp );
  REF_CNT_OBJ_RELEASE_IF(pSandbox);

  CM_MSG_HIGH_1("PMPRX: New Change mask:%d",change_mask);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if((( change_mask & PM_CHG_UE_MODE ) &&
      ( cmcall_find_active_cs_call_per_sub (asubs_id) != CM_CALL_ID_INVALID
        || cmcall_is_there_a_call_type_per_sub(asubs_id,CM_CALL_TYPE_SMS,NULL) != CM_CALL_ID_INVALID )
     )
      ||
      (( change_mask & PM_CHG_DEVICE_MODE ) &&
       cmcall_find_active_voice_call() != CM_CALL_ID_INVALID )
    )
  {
    return_val = CM_PH_CMD_ERR_IN_USE_S;
  }

  return return_val;
}

/*===========================================================================

FUNCTION cmpmprx_sys_sel_pref_policy_update

DESCRIPTION
  If the new system selection preferences cause a policy change, retrieve it
  from Policy Manager and update the cmpm_local.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: New policy retrieved and updated.
  False: No change in policy.

SIDE EFFECTS
  none

===========================================================================*/
boolean                           cmpmprx_sys_sel_pref_policy_update(

  sys_modem_as_id_e_type     asubs_id,

  cm_sys_sel_pref_params_s_type *pref_ptr,

  cm_ph_cmd_s_type          *ph_cmd_ptr
)
{
  cm_policyman_cfg_s      *pm_msgr_rsp  = NULL;
  cm_policy_config_s_type *local_policy = NULL;
  boolean                 return_val    = FALSE;
  policy_change_mask device_local_policy = 0;
  cmph_s_type       *ph_ptr         = cmph_ptr();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (pref_ptr == NULL)
  {
    CM_MSG_HIGH_0("PMPRX: Incoming pref_ptr NULL");
    return return_val;
  }

  /* Ensure that the subscription id is valid.
  */
  if ( !cmutil_is_as_id_valid( asubs_id ) )
  {
    CM_MSG_HIGH_1("Returning from cmpmprx_sys_sel_pref_policy_update() since as_id is %d",asubs_id);
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the new preferences to Policy Manager and receive the new policy
  ** if it is changed.
  */
  pm_msgr_rsp = (cm_policyman_cfg_s *)
                policyman_report_preference_change( asubs_id, pref_ptr, NULL);


  if ( pm_msgr_rsp == NULL )
  {
    return return_val;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Policy changed so we need to retrieve the new policy data items.
  ** Allocate local buffer for new memory and initialize it to current policy.
  */
  local_policy = cmpmprx_alloc_local_policy_copy();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Retrieve the policy data into a local buffer. If all the policy items in
  ** the new policy are successfully retrieved, only then update the current
  ** policy and attempt to enforce it.
  */
  return_val = cmpmprx_process_msgr_msg(pm_msgr_rsp,local_policy,&device_local_policy, FALSE);

  /* If there is a pref change while ue is in voice all and the retrieved
  ** policy has ue_mode change, dont enforce the policy. Wait for it to arrive
  ** on message router message so it wont interfere with call.
  */
  if ( return_val == TRUE )
  {
    CM_MSG_HIGH_6_ASID("policy_chgd: CM->PM: Polling PM, SYS_SEL_PREF, mode_pref %d->%d, srv_domain_pref %d->%d, new network_sel_mode_pref %d",
                          asubs_id,
                          CMPH_SS_MODE_PREF(asubs_id, ph_ptr),
                          pref_ptr->mode_pref,
                          CMPH_SS_SRV_DOMAIN_PREF(asubs_id, ph_ptr),
                          pref_ptr->srv_domain_pref,
                          pref_ptr->network_sel_mode_pref);
    cmpmprx_update_curr_policy(local_policy);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Enforce device level policy */
  if(device_local_policy != 0)
  {
    CM_MSG_HIGH_3("device_policy_chgd: CM->PM: Polling PM, SYS_SEL_PREF, mask=0x%x, asubs_id=%d, phcmd=%d",
                   device_local_policy, asubs_id,
                   ph_cmd_ptr->cmd);

    if( ph_cmd_ptr->cmd == CM_PH_CMD_SUBSCRIPTION_AVAILABLE     ||
        ph_cmd_ptr->cmd == CM_PH_CMD_SUBSCRIPTION_NOT_AVAILABLE ||
        ph_cmd_ptr->cmd == CM_PH_CMD_DUAL_STANDBY_PREF          ||
        ph_cmd_ptr->cmd == CM_PH_CMD_SUBSCRIPTION_CHANGED )
    {
      cmpmprx_enforce_device_policy(PM_CHG_DEVICE_MODE|PM_CHG_MAX_ACTIVE_DATA);
    }
    else
    {
      if(ph_cmd_ptr->cmd == CM_PH_CMD_SYS_SEL_PREF &&
          cmpmprx_is_device_mask_set(PM_CHG_MAX_MODE_CAP,NULL))
      {
        if(cmpmprx_is_ok_to_enforce_device_policy(CM_CALL_ID_INVALID))
        {
          cmcall_end_active_data_calls_remove_orig_end();
          cmpmprx_enforce_device_policy(PM_CHG_DEVICE_ALL);
        }
        else
        {
          CM_MSG_HIGH_0("device_policy_chgd not forced due to active cs call");
        }
      }
      else
      {
        cmpmprx_enforce_device_policy(PM_CHG_DEVICE_ALL);
      }
    }
  }

  policyman_report_preference_msg_free( (msgr_hdr_s *)pm_msgr_rsp );

  cmpmprx_mem_free(local_policy);
  return return_val;
}

/*===========================================================================

FUNCTION cmpmprx_msim_subsc_changed

DESCRIPTION
  Called when susbcriptions in multi-mode phones change.
  This function calls into PM to update new susbcriptions.
  PM refreshes policies based on this update.
    - Currently PM shall decide enable/disable LTE in multimode sub
      when subscriptions change. Meaning MSIM standby pref change.

===========================================================================*/
boolean cmpmprx_msim_subsc_changed(

  uint8 active_subs,

  sys_modem_as_id_e_type  default_data_subs,

  cm_dds_cause_e_type  dds_status
)
{
  cm_policyman_cfg_s      *pm_msgr_rsp  = NULL;
  cm_policy_config_s_type *local_policy = NULL;
  boolean                 return_val    = FALSE;
  dds_info_t              dds_info;
  policy_change_mask      device_local_policy = 0;
  sys_modem_as_id_e_type  local_sub;
  boolean                 ok_to_enforce = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If neither active_subs nor default_data_subs changed, do not inform PM */

  if(active_subs == cmpm_active_subs &&
      default_data_subs == cmpm_default_data_subs &&
      cmph_ptr()->device_prop.msim_prop.prst_dual_standby_pref.default_data_subs == default_data_subs)
  {
    return FALSE;
  }

  /* PM implementation is such a way that policyman_msim_report_subs_info
  ** is expected to be invoked only when PM is resumed. Since emergency call is
  ** anyway ended by now. Going ahead and resuming PM
  */

  if ( cmpm_ptr()->is_pm_suspend)
  {
    CM_MSG_HIGH_0("PMPRX: resume PM");
    policyman_resume();
    cmpm_ptr()->is_pm_suspend = FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_MSG_HIGH_3("CM->PM: DDS_SWITCH/ACTIVE_SUBS active_subs %d, dds %d, dds_cause %d",
                active_subs, default_data_subs, dds_status);

  cmpm_active_subs = active_subs;
  cmpm_default_data_subs = default_data_subs;

  dds_info.isPermanent = (dds_status == DDS_CAUSE_TEMPORARY)?FALSE:TRUE;
  dds_info.subsId = default_data_subs;
  pm_msgr_rsp = (cm_policyman_cfg_s *)
                policyman_msim_report_subs_info( &dds_info, active_subs, NULL );

  if ( pm_msgr_rsp == NULL )
  {
    CM_MSG_MED_0("PMPRX: New MSIM subs have no effect on policy");
    return TRUE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  local_policy = cmpmprx_alloc_local_policy_copy();
  return_val = cmpmprx_process_msgr_msg(pm_msgr_rsp,local_policy,&device_local_policy, FALSE);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Assuming nSims cannot change in this call.
  */
  for( local_sub = SYS_MODEM_AS_ID_1;
       cmutil_is_as_id_valid(local_sub);
       local_sub++ )
  {
    if( (active_subs & BM(local_sub))
        &&
        ( cmpmprx_is_ok_to_enforce_policy(cmpm_ptr(), local_sub, TRUE)
          || cmpmprx_is_ok_to_enforce_policy(local_policy, local_sub, TRUE)
        )
      )
    {
      ok_to_enforce = TRUE;
      break;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( return_val == TRUE &&
       (ok_to_enforce || local_policy->device_policy_changes != 0)
     )
  {
    CM_MSG_HIGH_3("sub_chgd: DDS_SWITCH/ACTIVE_SUBS, device_policy mask=0x%x, active_subs=%d, ok_to_enforce %d",
                  local_policy->device_policy_changes,
                  active_subs,
                  ok_to_enforce);
    cmpmprx_update_curr_policy(local_policy);
  }
  else
  {
    CM_MSG_HIGH_2("sub_chgd: No device_policy_chgd: DDS_SWITCH/ACTIVE_SUBS, active_subs=%d, PM return_val=%d",
                  active_subs, return_val);
  }

  policyman_report_msg_free( (msgr_hdr_s *)pm_msgr_rsp );

  cmpmprx_mem_free(local_policy);

  /* this is called to update subs capability to TRM/MCS */
  cmph_sp_subs_info_update();

  return return_val;
}

/*===========================================================================

FUNCTION cmpmprx_get_overall_featuremode

DESCRIPTION
  Called from other modules to read the current value of the overall
  featuremode in CMPM
===========================================================================*/
sys_overall_feature_t cmpmprx_get_overall_featuremode(void)
{
  return (cmpm_ptr()->feature_mode);
}

/*===========================================================================

FUNCTION cmpmprx_get_subs_feature_mode

DESCRIPTION
  API for CM to retrieve the new subs specific feature mode.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: New subs featuremode retrieved
  False: Operation failed.

SIDE EFFECTS
  none

===========================================================================*/
EXTERN boolean                    cmpmprx_get_subs_feature_mode(

  sys_modem_as_id_e_type     asubs_id,

  sys_subs_feature_t         *subs_featuremode_buffer
)
{
  if ( subs_featuremode_buffer == NULL || !cmutil_is_as_id_valid(asubs_id))
  {
    CM_MSG_MED_1("PMPRX -> CM: NO policyman ue_mode update sub %d",asubs_id);
    return FALSE;
  }
  else
  {
    sys_subs_feature_t pm_sfmode = cmpm_ptr()->sub_policy[asubs_id]->policy_items.sub_feature_mode;

    CM_MSG_HIGH_2("PMPRX -> CM: Policyman subs_id = %d, sfmode =%d",
                  asubs_id,pm_sfmode);

    /* Map SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE to srlte internally */
    if (pm_sfmode == SYS_SUBS_FEATURE_MODE_DUAL_MULTIMODE)
    {
#ifdef FEATURE_1XSRLTE
      *subs_featuremode_buffer   = SYS_SUBS_FEATURE_MODE_SRLTE;
#else
      *subs_featuremode_buffer   = SYS_SUBS_FEATURE_MODE_NORMAL;
#endif
    }
    else
    {
      *subs_featuremode_buffer   = pm_sfmode;
    }

#ifndef FEATURE_MMODE_SXLTE_G
    if (cmph_ptr()->device_prop.feature_mode == SYS_OVERALL_FEATURE_MODE_MULTISIM)
    {
      CM_MSG_HIGH_2("fmode: changing subs_feature_mode to NORMAL for asubs_id %d, pm %d", asubs_id, *subs_featuremode_buffer);
      *subs_featuremode_buffer = SYS_SUBS_FEATURE_MODE_NORMAL;
    }
#endif

    return TRUE;
  }
}

/*===========================================================================

FUNCTION cmpmprx_clnup_complete_ind

DESCRIPTION
  CM will notify PMPRX that the cleanup for Refresh type 0 OR operating mode
  is complete and PMPRX can enforce any pending policies.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void                       cmpmprx_clnup_complete_ind()
{
  sys_modem_as_id_e_type          asubs_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for (asubs_id = SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid( asubs_id ); asubs_id++ )
  {
    if (cmpmprx_read_cm_policy_consumption_eligibility( asubs_id ) &&
        cmpmprx_is_mask_set( asubs_id, PM_CHG_ALL ))
    {
      cmpmprx_enforce_current_policy(asubs_id);
    }
  }
}


/*===========================================================================

FUNCTION cmpmprx_get_networks_end_ind

DESCRIPTION
  CM will notify PMPRX that the manual search has been completed and any
  pending policies can be enforced

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void                       cmpmprx_get_networks_end_ind()
{
  sys_modem_as_id_e_type          asubs_id;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for (asubs_id = SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid( asubs_id ); asubs_id++ )
  {
    if (cmpmprx_read_cm_policy_consumption_eligibility( asubs_id ) &&
        cmpmprx_is_mask_set( asubs_id, PM_CHG_ALL ))
    {
      CM_MSG_MED_1("Manual search end, enforce buffered policy on sub %d", asubs_id);
      cmpmprx_enforce_current_policy(asubs_id);
    }
  }
}


/*===========================================================================

FUNCTION cmpmprx_process_thermal_policy_change

DESCRIPTION
  Handles policy change due to thermal state change and takes necessary action

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void cmpmprx_process_thermal_policy_change(
  const cfcm_cmd_msg_type_s *cfcm_ind,
  cm_policy_config_s_type  *new_policy
)
{
  cm_policy_config_s_type *pm_ptr  = cmpm_ptr();
  unsigned int             i       = 0;
  uint32                   removed_mode_mask;
  cmph_s_type             *ph_ptr  = cmph_ptr();

  if(!(cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_THERMAL_CX) ||
      cfcm_ind->fc_cmd.monitor_data.cx.state != THERMAL_LEVEL_2
    )
  {
    CM_MSG_HIGH_0("No need to end the calls in current state");
    return;
  }

  for(i=0; i<new_policy->device_mode.nSims; i++)
  {
    if(!cmpmprx_read_cm_policy_consumption_eligibility(i))
      continue;

    if( new_policy->sub_policy[i]->policy_changes & (PM_CHG_SVC_SCAN|PM_CHG_UE_MODE))
    {

      CM_MSG_HIGH_0("PM_CHG_SVC_SCAN|PM_CHG_UE_MODE");

      cmcall_endall_non_emerg_cs_calls(CM_CALL_END_THERMAL_EMERGENCY, BM(i));
      return;
    }

    if(new_policy->sub_policy[i]->policy_changes & PM_CHG_MODE_CAP )
    {
      CM_MSG_HIGH_0("PM_CHG_MODE_CAP");

      removed_mode_mask = (pm_ptr->sub_policy[i]->policy_items.policy_mode_capability &
                           ~new_policy->sub_policy[i]->policy_items.policy_mode_capability);

      if( removed_mode_mask &
          cmph_map_cm_mode_pref_to_sd_mode_pref(CMPH_SS_MODE_PREF(i, ph_ptr)))
      {
        cmcall_endall_non_emerg_cs_calls(CM_CALL_END_THERMAL_EMERGENCY, BM(i));
      }
    }
  }

}

/*===========================================================================

FUNCTION cmpmprx_thermal_state_change_policy_update

DESCRIPTION
  If the new system selection preferences cause a policy change, retrieve it
  from Policy Manager and update the cmpm_local.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: New policy retrieved and updated.
  False: No change in policy.

SIDE EFFECTS
  none

===========================================================================*/
void                           cmpmprx_thermal_state_change_policy_update(

  const cfcm_cmd_msg_type_s *cfcm_ind
)
{
  cm_policyman_cfg_s        *pm_msgr_rsp  = NULL;
  cm_policy_config_s_type   *local_policy = NULL;
  unsigned int               i            = 0;
  policy_change_mask        device_local_policy = 0;
  cm_policy_config_s_type   *pm_ptr       = cmpm_ptr();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (cfcm_ind == NULL)
  {
    CM_MSG_HIGH_0("PMPRX: Incoming cfcm_ind NULL");
    return;
  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the new preferences to Policy Manager and receive the new policy
  ** if it is changed.
  */
  pm_msgr_rsp = (cm_policyman_cfg_s *)
                policyman_report_thermal_state( &cfcm_ind->fc_cmd);


  if ( pm_msgr_rsp == NULL )
  {
    CM_MSG_MED_0("PMPRX: New preferences have no effect on policy");
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Policy changed so we need to retrieve the new policy data items.
  ** Allocate local buffer for new memory and initialize it to current policy.
  */
  local_policy = cmpmprx_alloc_local_policy_copy();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( cmpmprx_process_msgr_msg(pm_msgr_rsp,local_policy,&device_local_policy, FALSE) )
  {
    cmpmprx_process_thermal_policy_change(cfcm_ind,local_policy);
    cmpmprx_update_curr_policy(local_policy);
  }

  policyman_report_preference_msg_free( (msgr_hdr_s *)pm_msgr_rsp );

  cmpmprx_mem_free(local_policy);

  if( (cfcm_ind->fc_cmd.monitors_mask & CFCM_MONITOR_MASK_THERMAL_CX) &&
      cfcm_ind->fc_cmd.monitor_data.cx.state == THERMAL_LEVEL_3 )
  {
    CM_MSG_HIGH_0("In Lv3: policy change will be handled after moving to emergency mode");
    return;
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Enforce device level policy */
  if(cmpm_ptr()->device_policy_changes != 0)
  {
    cmpmprx_enforce_device_policy(PM_CHG_DEVICE_ALL);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Loop thru the subs_ids and see if any policy changed. If yes then enforce*/
  /* If any policy item has changed, attempt to enforce the new policy. */
  for (i=0; i<cmpmprx_get_num_of_sims(); i++)
  {
    if ( cmpmprx_is_mask_set( i, PM_CHG_ALL) )
    {
      cmpmprx_enforce_current_policy(i);
    }
  }
}

/*===========================================================================

FUNCTION cmpmprx_map_volte_mode_enable_to_volte_state

DESCRIPTION
  Handles E911 Volte On/Off command from QMI-V
  and forwards it to PM


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static cm_volte_enable_e_type cmpmprx_map_volte_mode_enable_to_volte_state(boolean volte_mode_from_ap)
{
  return((volte_mode_from_ap == TRUE)?CM_VOLTE_ENABLED:CM_VOLTE_DISABLED);
}

/*===========================================================================

FUNCTION cmpmprx_handle_volte_state_notification_cmd

DESCRIPTION


DEPENDENCIES
  none

RETURN VALUE

SIDE EFFECTS
  none

===========================================================================*/
EXTERN void                    cmpmprx_handle_volte_state_notification_cmd(
  cm_volte_state_notification_cmd_s_type *volte_state_cmd
)
{
  cm_sys_sel_pref_params_s_type *pmprx_pref_ptr = NULL;
  cm_policyman_cfg_s            *pm_msgr_rsp  = NULL;
  cm_policy_config_s_type       *local_policy = NULL;
  boolean                       return_val;
  policy_change_mask            device_local_policy = 0;
#if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
  boolean                       pwr_up_in_srlte;
#endif
  cmph_sub_info_s_type *ph_sub_ptr;
  cmph_s_type *ph_ptr = cmph_ptr();
  sys_modem_as_id_e_type  asubs_id = volte_state_cmd->as_id;
  boolean is_volte_state_chgd = FALSE;

  if (cmph_is_ssim())
  {
    asubs_id = SYS_MODEM_AS_ID_1;
  }

  ph_sub_ptr = cmph_get_sub_ptr(asubs_id);
  if(ph_sub_ptr == NULL)
  {
    return;
  }

  /*Notify QMI-NAS to reset IMS pref chg ind cache*/
  if(!volte_state_cmd->volte_mode_enable)
  {
    cmph_send_msgr_qmi_cache_reset(asubs_id);
  }
  pmprx_pref_ptr = (cm_sys_sel_pref_params_s_type *)
                   cm_mem_malloc(sizeof(cm_sys_sel_pref_params_s_type));

  /* Fill all the fields with No_change, for fields like term_pref use from cmph_ptr */

  pmprx_pref_ptr->mode_pref        = CMPH_SS_MODE_PREF(asubs_id, ph_ptr);
  pmprx_pref_ptr->pref_duration    = 0;
  pmprx_pref_ptr->band_pref        = CMPH_SS_BAND_PREF(asubs_id, ph_ptr);
  pmprx_pref_ptr->lte_band_pref    = CMPH_SS_LTE_BAND_PREF(asubs_id, ph_ptr);
  pmprx_pref_ptr->tds_band_pref    = CMPH_SS_TDS_BAND_PREF(asubs_id, ph_ptr);
  pmprx_pref_ptr->prl_pref         = CMPH_SS_PRL_PREF(asubs_id, ph_ptr);
  pmprx_pref_ptr->roam_pref        = CMPH_SS_ROAM_PREF(asubs_id, ph_ptr);
  pmprx_pref_ptr->hybr_pref        = CMPH_SS_HYBR_PREF(asubs_id, ph_ptr);
  pmprx_pref_ptr->srv_domain_pref  = CMPH_SS_SRV_DOMAIN_PREF(asubs_id, ph_ptr);
  pmprx_pref_ptr->network_sel_mode_pref = CMPH_SS_NETWORK_SEL_MODE_PREF(asubs_id, ph_ptr);
  pmprx_pref_ptr->csg_id           = ph_sub_ptr->pref_info.csg_id;
  pmprx_pref_ptr->csg_rat          = ph_sub_ptr->pref_info.csg_rat;
  pmprx_pref_ptr->ue_usage_setting = CMPH_SS_UE_USAGE_SETTING(asubs_id, ph_ptr);
  pmprx_pref_ptr->term_pref        = ph_sub_ptr->pref_info.pref_term;
  pmprx_pref_ptr->plmn_ptr         = &(ph_sub_ptr->pref_info.plmn);
  pmprx_pref_ptr->voice_domain_pref = CMPH_SS_VOICE_DOMAIN_PREF(asubs_id,ph_ptr);
  pmprx_pref_ptr->lte_disable_cause = ph_sub_ptr->lte_disable_cause;
  pmprx_pref_ptr->is_allow_buffering_cmd = FALSE;

  /* fill new value of volte mode coming from AP */
  pmprx_pref_ptr->volte_enable_state = cmpmprx_map_volte_mode_enable_to_volte_state(volte_state_cmd->volte_mode_enable);

  #ifdef FEATURE_CM_LTE
  /* When VOLTE is toggled ON, send reset blocked PLMN req */
  if(!ph_sub_ptr->volte_sub_info.is_volte_on && volte_state_cmd->volte_mode_enable)
  {
    cmph_reg_send_block_plmn_reset_req(asubs_id, SYS_BLOCK_PLMN_CAUSE_VOICE_NOT_AVAILABLE);
#ifdef FEATURE_DOMAIN_SELECTION
    cmsds_reset_lte_removal_timer(asubs_id);
#endif

   if(volte_state_cmd->volte_mode_enable)
   {
     cmsds_reset_ims_reg_and_voice_avail_status(asubs_id);
   }
  }
  #endif

  CM_MSG_HIGH_6_ASID("MSGR RXD: volte_state_chgd: %d->%d, dds %d, current rat_cap 0x%x, internal_standby_pref %d",
                    asubs_id,
                    ph_sub_ptr->volte_sub_info.is_volte_on,
                    volte_state_cmd->volte_mode_enable, 
                    ph_ptr->device_prop.msim_prop.default_data_subs,
                    cmmsc_get_msc_rat_cap(asubs_id),
                    ph_ptr->device_prop.msim_prop.internal_standby_pref);
  if (ph_sub_ptr->volte_sub_info.is_volte_on != volte_state_cmd->volte_mode_enable)
  {
    /* update ph about new volte status */
    ph_sub_ptr->volte_sub_info.is_volte_on = volte_state_cmd->volte_mode_enable;
    is_volte_state_chgd = TRUE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* When Volte is toggled on then off quickly, and PS detach complete has
  ** not been sent to IMS, send PS detach complete so that IMS is not
  ** stuck waiting for PS detach complete.
  */
  if(is_volte_state_chgd && !ph_sub_ptr->volte_sub_info.is_volte_on)
  {
    ph_sub_ptr->volte_sub_info.is_volte_on_pending = FALSE;
    if(ph_sub_ptr->volte_sub_info.is_ps_detach_comp_pending)
    {
      cmph_send_detach_comp(asubs_id);
    }
  }

  /* Send the new preferences to Policy Manager and receive the new policy
  ** if it is changed.
  */
  pm_msgr_rsp = (cm_policyman_cfg_s*)
                      policyman_report_preference_change( asubs_id, pmprx_pref_ptr, NULL);

  #if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
  if(policyman_get_subs_boolean_value("cm:pwr_up_in_srlte",
                                      SYS_MODEM_AS_ID_NONE, &pwr_up_in_srlte) == TRUE)
  {
    CM_MSG_HIGH_1("PM->CM: volte_state_chgd: pwr_up_in_srlte is %d", pwr_up_in_srlte);
    if(pwr_up_in_srlte == TRUE)
    {
      cmsoa_check_and_set_bootup_params_to_srlte_state(asubs_id);
    }
  }
  #endif
  
  if ( pm_msgr_rsp == NULL && !is_volte_state_chgd)
  {
    CM_MSG_HIGH_0("PMPRX: volte_state_chgd: no effect on policy and no volte_state change");
  }
  else if (pm_msgr_rsp)
  {

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /* Policy changed so we need to retrieve the new policy data items.
    ** Allocate local buffer for new memory and initialize it to current policy.
    */
    local_policy = cmpmprx_alloc_local_policy_copy();
  
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
    /* Retrieve the policy data into a local buffer. If all the policy items in
    ** the new policy are successfully retrieved, only then update the current
    ** policy and attempt to enforce it.
    */
    return_val = cmpmprx_process_msgr_msg(pm_msgr_rsp,local_policy,&device_local_policy, FALSE);
  
    /* If there is a pref change while ue is in voice all and the retrieved
    ** policy has ue_mode change, dont enforce the policy. Wait for it to arrive
    ** on message router message so it wont interfere with call.
    */
    if ( return_val == TRUE )
    {

      cmpmprx_update_curr_policy(local_policy);
      
      CM_MSG_HIGH_2("policy_chgd: volte_state_chgd: is_volte_state_chgd %d, sub %d",
                       is_volte_state_chgd, asubs_id);      

      if( cmpmprx_is_ok_to_enforce_policy( local_policy, asubs_id, TRUE))
      {

      /* If VOLTE is turned on and LTE is on stack 2, PS detach
      ** might need to be sent to IMS so that IMS can retry registration.
      ** When IMS gets PS detach complete, IMS will retry immediately
      ** after CM indicates LTE service on main stack,
      ** instead of based on the backoff timer.
      */
      if(is_volte_state_chgd && ph_sub_ptr->volte_sub_info.is_volte_on &&
        sys_srv_status_is_full_srv(cmss_get_info_srv_status(asubs_id, MM_STACK_2)))
      {
        ph_sub_ptr->volte_sub_info.is_volte_on_pending = TRUE;
      }

      /* enforce the current policy as well,
      as we need to do detach for volte on<->off transition */
      cmpmprx_check_and_enforce_policy();
    }
    }
  
    policyman_report_preference_msg_free( (msgr_hdr_s *)pm_msgr_rsp );
  
    cmpmprx_mem_free(local_policy);
  }
  else if (is_volte_state_chgd)
  {
    cmpmprx_enforce_volte_state(asubs_id, ph_sub_ptr->volte_sub_info.is_volte_on);
  }

  cm_mem_free(pmprx_pref_ptr);
}

/*===========================================================================

FUNCTION cmpmprx_get_persisted_user_plmn

DESCRIPTION
  Read persistent PLMN set by earlier user selection before SSR

DEPENDENCIES
None

RETURN VALUE
  TRUE if read successful
  FALSE otherwise

SIDE EFFECTS
  none

===========================================================================*/

boolean cmpmprx_get_persisted_user_plmn(
  sys_modem_as_id_e_type  as_id,
  boolean                 clear,
  sys_plmn_id_s_type     *plmn_ptr )

{
  return policyman_get_persisted_user_plmn( as_id, clear, plmn_ptr );
}

/*===========================================================================

FUNCTION cmpmprx_report_full_srv_lost

DESCRIPTION
  Reports to PM whenever Full Service Lost indication is received from SD.

===========================================================================*/

boolean cmpmprx_report_full_srv_lost(
  sys_modem_as_id_e_type     subs_id,
  sys_modem_stack_id_e_type  stack_id
)
{
  if(!cmutil_is_as_id_valid(subs_id))
  {
    return FALSE;
  }
  policyman_report_acq_fail(subs_id,stack_id);
  return TRUE;
}

/*===========================================================================

FUNCTION cmpmprx_report_operational_radio_stacks

DESCRIPTION
  Reports to PM whenever there is change in operation flag on either sub/stack.

===========================================================================*/

boolean cmpmprx_report_operational_radio_stacks(
  sys_modem_as_id_e_type     subs_id
)
{
  cmss_s_type *ss_ptr = cmss_ptr();
  uint32 stackMask=0;
  boolean is_oper[MM_STACK_ALL] = {FALSE};
  mm_sub_stk_id_s_type mm_id;

  if (cmutil_is_as_id_valid(subs_id))
  {
    mm_id.asubs_id = subs_id;
    for (mm_id.stk_id = MM_STACK_0; mm_id.stk_id < ARR_SIZE(is_oper); mm_id.stk_id ++)
    {
      is_oper[mm_id.stk_id] = cmss_is_operational(mm_id);
    }

    if (is_oper[MM_STACK_0])
    {
      stackMask |= BM(SYS_MODEM_STACK_ID_1);
    }
    if((is_oper[MM_STACK_1] && cmph_is_sxlte_second_stack_allowed_on_mode(subs_id,CM_MODE_PREF_HDR_ONLY) ) ||
        (is_oper[MM_STACK_2] && cmph_is_sxlte_second_stack_allowed_on_mode(subs_id, CM_MODE_PREF_LTE_ONLY)))
    {
      stackMask |= BM(SYS_MODEM_STACK_ID_2);
    }
  }

  CM_MSG_HIGH_5_ASID("CM->PM: oper_chgd: Reporting operational_radio_stacks. stackMask=%d, is_oper %d %d %d",
                     subs_id,stackMask,
                     is_oper[MM_STACK_0], is_oper[MM_STACK_1], is_oper[MM_STACK_2]);

  policyman_report_operational_radio_stacks(subs_id, stackMask);
  return TRUE;
}

/*===========================================================================

FUNCTION cmpmprx_get_pm_sfmode

DESCRIPTION
  API for CM to get the PM sub-feature mode configured in XML.
  Note: For 7+5, PM sub-feature mode is different from the CM internal sub-feature mode.

DEPENDENCIES
  none

RETURN VALUE
  sys_subs_feature_t

SIDE EFFECTS
  none

===========================================================================*/
sys_subs_feature_t    cmpmprx_get_pm_sfmode(

  sys_modem_as_id_e_type     asubs_id

)
{
  cm_policy_config_s_type  *pm_info = cmpm_ptr();

  if (cmutil_is_as_id_valid(asubs_id))
  {
    return (pm_info->sub_policy[asubs_id]->policy_items.sub_feature_mode);
  }
  else
  {
    return (pm_info->sub_policy[SYS_MODEM_AS_ID_1]->policy_items.sub_feature_mode);
  }
}

/*===========================================================================

FUNCTION cmpmprx_sanitize_max_and_curr_cap

DESCRIPTION
  ERR_FATAL if max mode cap and curr mode cap are not obeying Superset/subset relation

DEPENDENCIES
  none

RETURN VALUE
  sys_subs_feature_t

SIDE EFFECTS
  none

===========================================================================*/
void cmpmprx_sanitize_max_and_curr_cap(cm_policy_config_s_type *pm_ptr)
{
  uint8   i         = 0;
  uint32  diff_bits = 0;
  uint8 size = cmpmprx_get_num_of_sims();

  for (; i< size && cmutil_is_as_id_valid(i); i++)
  {
    // Error Fatal in case if Max Mode Capability is not super set of  capability
    diff_bits = pm_ptr->sub_policy[i]->policy_items.policy_max_mode_capability ^ pm_ptr->sub_policy[i]->policy_items.policy_mode_capability;

    if (cmpm_active_subs & BM(i))
    {
      if (pm_ptr->sub_policy[i]->policy_items.policy_max_mode_capability == SD_SS_MODE_PREF_NONE)
      {
        CM_MSG_HIGH_3("Warning: sub %d PM provided max_mode_cap 0x%x, curr_mode_cap 0x%x",
                         i, 
                         pm_ptr->sub_policy[i]->policy_items.policy_max_mode_capability,
                         pm_ptr->sub_policy[i]->policy_items.policy_mode_capability);
        continue;
      }
      if(pm_ptr->sub_policy[i]->policy_items.policy_mode_capability & diff_bits)
      {
        CM_MSG_HIGH_3(" Sanity fail for sub %d! PM provided incorrect MMC CMC !! max_mode_cap 0x%x, curr_mode_cap 0x%x", i, pm_ptr->sub_policy[i]->policy_items.policy_max_mode_capability,
                      pm_ptr->sub_policy[i]->policy_items.policy_mode_capability);

        CM_ERR_FATAL_0("PM provided incorrect max_mode_cap and curr_mode_cap Error!!");
      }

    }
  }
}

/*===========================================================================

FUNCTION cmpmprx_check_and_enforce_policy

DESCRIPTION
  Force policy if any policy element is set


DEPENDENCIES
  none

RETURN VALUE
  void

SIDE EFFECTS
  none

===========================================================================*/
void    cmpmprx_check_and_enforce_policy(void)
{
  cm_policy_config_s_type  *pm_info = cmpm_ptr();
  uint8 i=0;

  /* Enforce device level policy */
  if(pm_info->device_policy_changes != 0)
  {
    CM_MSG_HIGH_1("cmpmprx_check_and_enforce_policy, policy_changes %d", pm_info->device_policy_changes);
    cmpmprx_enforce_device_policy(PM_CHG_DEVICE_ALL);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Enforce sub level policies, avoiding RAT concurrency issues
  */
  cmpmprx_enforce_sub_policies_avoid_rat_conflict();

  return;
}

/*===========================================================================

FUNCTION cmpmprx_read_mode_capability

DESCRIPTION
  Read the  mode capability.

DEPENDENCIES
None

RETURN VALUE
  uint32

SIDE EFFECTS
  none

===========================================================================*/
uint32    cmpmprx_read_mode_capability(

  sys_modem_as_id_e_type     asubs_id
)
{
  cm_policy_config_s_type  *pm_info = cmpm_ptr();

  if (cmutil_is_as_id_valid(asubs_id))
  {
    return pm_info->sub_policy[asubs_id]->policy_items.policy_mode_capability;
  }
  else
  {
    return (uint32)cmutil_map_sd_mode_pref_to_sys_mode_mask(SD_SS_MODE_PREF_GSM);
  }
}

/*===========================================================================

FUNCTION cmpmprx_compute_fullrat_status

DESCRIPTION
  This function translates network selection mode preference ,.service.scan.mode.,.phone obj acq type
  to fullrat status which indicates whether UE is in fullrat or not.


DEPENDENCIES
  none

RETURN VALUE
  sd_ss_fullrat_pref_e_type

SIDE EFFECTS
  none

===========================================================================*/
sd_ss_fullrat_pref_e_type cmpmprx_compute_fullrat_status
(
  mm_sub_stk_id_s_type mm_id,
  cm_network_sel_mode_pref_e_type  cm_net_sel_mode_pref,
  cm_sub_level_policy_s_type       *pm_sub_ptr
)
{
  sd_ss_fullrat_pref_e_type fullrat_status= SD_SS_FULLRAT_OFF;

  if (pm_sub_ptr
      && pm_sub_ptr->policy_items.service_scan_mode == POLICYMAN_SVC_MODE_FULL_RAT
      && (cm_net_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_LIMITED_SRV ||
          cm_net_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_AUTO_CAMP_ONLY ||
          cm_net_sel_mode_pref == CM_NETWORK_SEL_MODE_PREF_MANUAL_CAMP_ONLY)
      && (cmtask_orig_para_search_act_type(mm_id,CM_ACT_TYPE_POLICYMAN_RESTRICT) != NULL)
     )
  {
    fullrat_status = SD_SS_FULLRAT_ON;
    CM_MSG_HIGH_1("cmpmprx_compute_fullrat_status: computed fullrat_status = %d",
                   fullrat_status );
  }

  return fullrat_status;
} /* cmpmprx_compute_fullrat_status */


/*===========================================================================

FUNCTION cmpmprx_is_item_value_changed

DESCRIPTION
  Check if the value of any specific item on subs is really changed.

DEPENDENCIES
None

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_is_item_value_changed(
  cm_policy_config_s_type *curr_policy,
  cm_policy_config_s_type *local_policy,
  policyman_item_id_t      item,
  sys_modem_as_id_e_type   sub_id
)
{
  cm_sub_level_policy_s_type *curr_sub_policy_ptr = NULL;
  cm_sub_level_policy_s_type *local_sub_policy_ptr = NULL;

  if(!(sub_id > SYS_MODEM_AS_ID_NONE && sub_id < MAX_SIMS))
  {
    return FALSE;
  }

  curr_sub_policy_ptr = curr_policy->sub_policy[sub_id];
  local_sub_policy_ptr = local_policy->sub_policy[sub_id];

  if(curr_sub_policy_ptr == NULL || local_sub_policy_ptr == NULL)
  {
    return FALSE;
  }

  switch(item)
  {
    case POLICYMAN_ITEM_RAT_CAPABILITY:
      if (curr_sub_policy_ptr->policy_items.policy_mode_capability !=
          local_sub_policy_ptr->policy_items.policy_mode_capability)
      {
        return TRUE;
      }
      break;

    case POLICYMAN_ITEM_RF_BANDS:
      if ((curr_sub_policy_ptr->policy_items.policy_band_capability !=
           local_sub_policy_ptr->policy_items.policy_band_capability) ||
          (curr_sub_policy_ptr->policy_items.policy_tds_band_capability !=
           local_sub_policy_ptr->policy_items.policy_tds_band_capability) ||
          !SYS_LTE_BAND_MASK_CHK_IF_EQUAL(
            &(curr_sub_policy_ptr->policy_items.policy_lte_band_capability),
            &(local_sub_policy_ptr->policy_items.policy_lte_band_capability))
         )
      {
        return TRUE;
      }
      break;

    case POLICYMAN_ITEM_SVC_MODE:
      if (curr_sub_policy_ptr->policy_items.service_scan_mode !=
          local_sub_policy_ptr->policy_items.service_scan_mode)
      {
        return TRUE;
      }
      break;

    case POLICYMAN_ITEM_UE_MODE:
      if (curr_sub_policy_ptr->policy_items.ue_operation_mode !=
          local_sub_policy_ptr->policy_items.ue_operation_mode)
      {
        return TRUE;
      }
      break;

    case POLICYMAN_ITEM_VOICE_DOMAIN_PREF:
      if ((curr_sub_policy_ptr->policy_items.voice_domain_pref !=
           local_sub_policy_ptr->policy_items.voice_domain_pref) ||
          (curr_sub_policy_ptr->policy_items.is_voice_dom_pref_locked !=
           local_sub_policy_ptr->policy_items.is_voice_dom_pref_locked))
      {
        return TRUE;
      }
      break;

    case POLICYMAN_ITEM_RAT_ACQ_ORDER:
      if (memcmp(&(curr_sub_policy_ptr->policy_items.acq_order_pref),
                 &(local_sub_policy_ptr->policy_items.acq_order_pref),
                 sizeof(cm_acq_pri_order_pref_s_type)))
      {
        return TRUE;
      }
      break;

    case POLICYMAN_ITEM_CALL_MODE:
      if (curr_sub_policy_ptr->policy_items.call_mode !=
          local_sub_policy_ptr->policy_items.call_mode)
      {
        return TRUE;
      }
      break;

    case POLICYMAN_ITEM_DEVICE_CONFIGURATION :
    default:
      return TRUE;
      break;
  }

  return FALSE;
}

/*===========================================================================

FUNCTION cmpmprx_determine_num_stacks

DESCRIPTION
  Evaluate the number of stacks for the provided asubs_id. Will be used when
  computing the new number of stacks based on eligibility and max_mode_cap
  changes.


DEPENDENCIES
  none

RETURN VALUE
  void

SIDE EFFECTS
  none

===========================================================================*/
uint8  cmpmprx_determine_num_stacks(
  sys_modem_as_id_e_type     asubs_id,
  uint8                      active_subs
)
{
  cm_policy_config_s_type  *pm_ptr = cmpm_ptr();
  cmph_s_type              *ph_ptr = cmph_ptr();
  boolean                   cdma_DDS_allowed = TRUE;
  boolean                   is_subsidized_phn   = FALSE;
  cm_sub_level_policy_s_type  *sub_policy_ptr = cmpm_sub_ptr(asubs_id);
  boolean                   is_dual_lte = cmph_is_dual_lte();

  if(sub_policy_ptr == NULL)
  {
    CM_MSG_HIGH_1("cmpm_sub_policy is NULL for asubs_id = %d, INVALID AS_ID", asubs_id);
    return 1;
  }

  /* Check if the Sub supports 3GPP2 to determine the num of stacks to allocate
  */
  if((sub_policy_ptr->is_cm_eligible.cdma_subsc == CMPMPRX_SUBSC_AVAIL ||
      cmph_ptr()->sub_info[asubs_id]->rtre_control == CM_RTRE_CONTROL_NV)
      &&
      (sub_policy_ptr->policy_items.policy_max_mode_capability & SYS_SYS_MODE_MASK_CDMA)
      &&
      (active_subs & BM(asubs_id))
    )
  {
    /* If the feature mode is not NORMAL, 3 stacks are generally needed. As a further
    ** optimization the CMCC subsidized requirement is also checked. For example, if the
    ** device is CMCC subsidized and the sim cards are CMCC+CT, only 2 stacks are needed
    ** for the CT/1x sub since it will never become the DDS sub. CM will query into PM for
    ** this information. The PM API returns the second argument (in this case FALSE) if the
    ** tag is not present in PM XML. If the tag is found, the PM API will return TRUE if the
    ** device is CMCC subsidized and there is a CMCC card inserted in the device, otherwise
    ** it will return FALSE.
    */
    if (sub_policy_ptr->policy_items.sub_feature_mode != SYS_SUBS_FEATURE_MODE_NORMAL && 
		(is_dual_lte || !policyman_get_subs_boolean_with_default("cm:start_ps_sub", SYS_MODEM_AS_ID_NONE, FALSE)))
    {
      return 3; /* Main + Hybrid HDR + Hybrid LTE*/
    }
    if (sub_policy_ptr->policy_items.policy_max_mode_capability & SYS_SYS_MODE_MASK_HDR)
    {
      return 2; /* Main + Hybrid HDR */
    }
  }

  /* If 3GPP2 is not supported on the sub, there will be only 1 stack
  */
  return 1; /* Main */
}

/*===========================================================================

FUNCTION cmpmprx_is_valid_sub_policy

DESCRIPTION
  Determine if a sub policy is valid for the specified sub.

DEPENDENCIES
  none

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_is_valid_sub_policy(
  sys_modem_as_id_e_type  asubs_id
)
{
  if (!cmutil_is_as_id_valid(asubs_id) || cmpm_ptr()->sub_policy[asubs_id] == NULL)
  {
    return FALSE;
  }

  return TRUE;
}

/*===========================================================================

FUNCTION cmpmprx_read_init_max_mode_capability

DESCRIPTION
  Fetch the max mode capability at cm initizlization.

DEPENDENCIES
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  none

===========================================================================*/
uint32    cmpmprx_read_init_max_mode_capability(

  sys_modem_as_id_e_type     asubs_id
)
{
  if(cmutil_is_as_id_valid(asubs_id))
  {
    return cmpm_ptr()->sub_policy[asubs_id]->policy_items.policy_max_mode_capability;
  }
  else
  {
    return (uint32)cmutil_map_sd_mode_pref_to_sys_mode_mask(SD_SS_MODE_PREF_ALL_MODES);
  }
}

/*===========================================================================

FUNCTION cmpmprx_read_1x_sub

DESCRIPTION
  Read max cap policy and determine the 1x sub.

DEPENDENCIES
  should only be only caled at cm initialization.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
sys_modem_as_id_e_type  cmpmprx_read_1x_sub()
{
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();
  uint8 sub_idx;

  for (sub_idx = 0; sub_idx < cmpmprx_get_num_of_sims(); sub_idx ++)
  {
    if (!cmpmprx_is_valid_sub_policy(sub_idx))
    {
      return SYS_MODEM_AS_ID_NONE;
    }

    if (pm_ptr->sub_policy[sub_idx]->policy_items.policy_max_mode_capability
        & SYS_SYS_MODE_MASK_CDMA)
    {
      return (sys_modem_as_id_e_type)sub_idx;
    }
  }
  return SYS_MODEM_AS_ID_NONE;
}


/*===========================================================================

FUNCTION cmpmprx_retrieve_and_verify_srv_req

DESCRIPTION
  Initialize the Policy Manager object and check whether this can result in SRV req and set the mask if srv req
  can be triggered due to PM config change

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static policyman_status_t        cmpmprx_retrieve_and_verify_srv_req
(
  cm_policy_config_s_type    *local_policy_ptr,

  policyman_item_collection_t const  *policy_item,

  policy_change_mask         *change_mask,

  policy_change_mask         *device_change_mask
)
{
  policyman_status_t      return_val = POLICYMAN_STATUS_ERR;
  sys_modem_as_id_e_type  sub_id = SYS_MODEM_AS_ID_NONE;
  uint8 i=0, j=0;
  policyman_item_t const  *pItem;
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();
  uint8                   size = cmpmprx_get_num_of_sims();
  sys_ue_mode_e_type old_ue_mode, new_ue_mode;
  CM_ASSERT( local_policy_ptr != NULL );
#ifndef CM_DEBUG
  CM_ASSERT( policy_item != NULL );
#endif

  CM_MSG_HIGH_1("cmpmprx_retrieve_and_verify_srv_req: RETRIEVE, numItems %d",
                policy_item->numItems);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for (i=0; i<policy_item->numItems; i++)
  {
    pItem = policy_item->pItems[i];
    switch (policyman_item_get_id_msim(pItem,&sub_id))
    {
      case POLICYMAN_ITEM_RAT_CAPABILITY:
        if(sub_id > SYS_MODEM_AS_ID_NONE &&
            sub_id < MAX_SIMS &&
            sub_id < size
          )
        {
          return_val = policyman_get_rat_capability( pItem,
                       &(local_policy_ptr->sub_policy[sub_id]->policy_items.policy_mode_capability));

          if (POLICYMAN_SUCCEEDED(return_val) &&
              cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_RAT_CAPABILITY, sub_id)
             )
          {

            CM_MSG_HIGH_4_EXT("=DBG=: mode_cap_chgd:asubs_id=%d,local mode_cap=%d,current mode_cap=%d,mode_pref=%d",
                              sub_id,
                              local_policy_ptr->sub_policy[sub_id]->policy_items.policy_mode_capability,
                              pm_ptr->sub_policy[sub_id]->policy_items.policy_mode_capability,
                              CMPH_SS_MODE_PREF(sub_id,cmph_ptr()),
                              (sub_id+1)
                             );

            if( ( (pm_ptr->sub_policy[sub_id]->policy_items.policy_mode_capability) & 
                    cmph_map_cm_mode_pref_to_sd_mode_pref(CMPH_SS_MODE_PREF(sub_id,cmph_ptr())) ) !=
                ( (local_policy_ptr->sub_policy[sub_id]->policy_items.policy_mode_capability) & 
                    cmph_map_cm_mode_pref_to_sd_mode_pref(CMPH_SS_MODE_PREF(sub_id,cmph_ptr())) ) )
            {
              change_mask[sub_id] |= PM_CHG_MODE_CAP;
              CM_MSG_HIGH_3("=DBG=: mode_cap_chgd: asubs_id=%d, mode_cap=%d, change_mask=%d",
                            sub_id,
                            local_policy_ptr->sub_policy[sub_id]->policy_items.policy_mode_capability,
                            change_mask[sub_id]);
            }
          }
        }

        break;

      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_SERVICE_DOMAIN:
        if(sub_id > SYS_MODEM_AS_ID_NONE &&
            sub_id < MAX_SIMS &&
            sub_id < size
          )
        {
          return_val = policyman_get_service_domain( pItem,
                       &(local_policy_ptr->sub_policy[sub_id]->policy_items.policy_domain_capability));

          if (POLICYMAN_SUCCEEDED(return_val))
          {

            CM_MSG_HIGH_4("=DBG=: domain_cap_chgd:asubs_id=%d,local domain_cap=%d,current domain_cap=%d,src_domain_pref=%d",
                          sub_id,
                          local_policy_ptr->sub_policy[sub_id]->policy_items.policy_domain_capability,
                          pm_ptr->sub_policy[sub_id]->policy_items.policy_domain_capability,
                          CMPH_SS_SRV_DOMAIN_PREF(sub_id,cmph_ptr())
                         );

            if( ( (pm_ptr->sub_policy[sub_id]->policy_items.policy_domain_capability) & (CMPH_SS_SRV_DOMAIN_PREF(sub_id,cmph_ptr())) ) !=
                ( (local_policy_ptr->sub_policy[sub_id]->policy_items.policy_domain_capability) & (CMPH_SS_SRV_DOMAIN_PREF(sub_id,cmph_ptr())) ) )
            {
              change_mask[sub_id] |= PM_CHG_DOMAIN_CAP;
              CM_MSG_HIGH_3("=DBG=: domain_cap_chgd: asubs_id=%d, domain_cap=%d, change_mask=%d",
                            sub_id,
                            pm_ptr->sub_policy[sub_id]->policy_items.policy_domain_capability,
                            change_mask[sub_id]);
            }
          }
        }
        break;

      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_RF_BANDS:
        if(sub_id > SYS_MODEM_AS_ID_NONE &&
            sub_id < MAX_SIMS &&
            sub_id < size
          )
        {
          return_val = policyman_get_rf_bands(pItem,
                                              &(local_policy_ptr->sub_policy[sub_id]->policy_items.policy_band_capability),
                                              &(local_policy_ptr->sub_policy[sub_id]->policy_items.policy_lte_band_capability),
                                              &(local_policy_ptr->sub_policy[sub_id]->policy_items.policy_tds_band_capability));

          if ( POLICYMAN_SUCCEEDED(return_val) &&
               cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_RF_BANDS, sub_id))
          {
            change_mask[sub_id] |= PM_CHG_BAND_CAP;

            CM_MSG_HIGH_1("PM -> PMPRX: band_cap_chgd: asubs_id=%d",
                          sub_id);
          }
        }
        break;
      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_SVC_MODE:
        if(sub_id > SYS_MODEM_AS_ID_NONE &&
            sub_id < MAX_SIMS &&
            sub_id < size
          )
        {
          return_val = policyman_get_svc_mode( pItem,
                                               &(local_policy_ptr->sub_policy[sub_id]->policy_items.service_scan_mode));

          if ( POLICYMAN_SUCCEEDED(return_val) &&
               cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_SVC_MODE, sub_id))
          {
            change_mask[sub_id] |= PM_CHG_SVC_SCAN;
            CM_MSG_HIGH_2("PM -> PMPRX: srv_mode_chgd: asubs_id=%d, srv_mode=%d",
                          sub_id,
                          local_policy_ptr->sub_policy[sub_id]->policy_items.service_scan_mode);
          }
        }
        break;
      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_UE_MODE:
        if(sub_id > SYS_MODEM_AS_ID_NONE &&
            sub_id < MAX_SIMS &&
            sub_id < size
          )
        {
          return_val = policyman_get_ue_mode( pItem,
                                              &(local_policy_ptr->sub_policy[sub_id]->policy_items.ue_operation_mode));

          if ( POLICYMAN_SUCCEEDED(return_val) &&
               cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_UE_MODE, sub_id))
          {
            change_mask[sub_id] |= PM_CHG_UE_MODE;
            CM_MSG_HIGH_2("PM -> PMPRX: ue_mode_chgd: asubs_id=%d, ue_mode=%d",
                          sub_id,
                          local_policy_ptr->sub_policy[sub_id]->policy_items.ue_operation_mode);

            old_ue_mode = pm_ptr->sub_policy[sub_id]->policy_items.ue_operation_mode;
            new_ue_mode = local_policy_ptr->sub_policy[sub_id]->policy_items.ue_operation_mode;

            if(!(cmph_is_subs_feature_mode_srlte(sub_id) && 
               SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED == old_ue_mode && 
               SYS_UE_MODE_CSFB == new_ue_mode &&
               cmss_is_stack2_operational(sub_id)))
            {
              change_mask[sub_id] &= ~PM_CHG_UE_MODE;
              CM_MSG_HIGH_1("PM -> PMPRX: reset ue_mode_chgd: asubs_id=%d",
                          sub_id);
            }
          }
        }

        break;
      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_DEVICE_CONFIGURATION:
      {
        size_t                new_nSims                = 0;
        size_t                new_nMaxActive           = 0;
        size_t                new_nMaxActiveData       = 0;
        uint32                max_mode_cap[MAX_AS_IDS];

        if(!POLICYMAN_SUCCEEDED(policyman_device_config_num_sims(pItem, &(new_nSims)))
            ||
            !POLICYMAN_SUCCEEDED(policyman_device_config_max_active_data(pItem, &(new_nMaxActiveData)))
            ||
            !POLICYMAN_SUCCEEDED(policyman_device_config_max_active(pItem, &(new_nMaxActive)))
          )
        {
          return_val = POLICYMAN_STATUS_ERR;
          CM_MSG_HIGH_0("MaxSIM numSIM nMaxActiveData read error");
        }
        else
        {
          return_val = POLICYMAN_STATUS_SUCCESS;
        }

        if (POLICYMAN_SUCCEEDED(return_val))
        {

          CM_MSG_HIGH_3("PM -> PMPRX: device_mode_chgd: nSIM %d nMaxActive %d nMaxActiveData %d",
                        new_nSims, new_nMaxActive, new_nMaxActiveData);

          if ( device_change_mask != NULL )
          {
            if ( local_policy_ptr->device_mode.nSims != new_nSims ||
                 local_policy_ptr->device_mode.nMaxActive != new_nMaxActive )
            {
              cmph_s_type  *ph_ptr       = cmph_ptr();
              /* If sub is deactivating and received device mode change do no enfore policy as
                        ** we expect device reset to handle such device mode changes. This check is
                        ** required only while processing deactivate_subs cmd.
                        */
              if (cmph_get_subs_with_mcfg_refresh_ongoing())
              {
                CM_MSG_HIGH_1("Sub %d is deactivating do not enfore policy now",cmph_get_subs_with_mcfg_refresh_ongoing());
                return POLICYMAN_STATUS_ERR;
              }

              *device_change_mask = PM_CHG_DEVICE_MODE;
            }

            if( local_policy_ptr->device_mode.nMaxActiveData != new_nMaxActiveData )
            {
              *device_change_mask |= PM_CHG_MAX_ACTIVE_DATA;
            }
          }

          cmpmprx_configure_num_policies(local_policy_ptr,new_nSims);

          local_policy_ptr->device_mode.nSims        = new_nSims;
          local_policy_ptr->device_mode.nMaxActive   = new_nMaxActive;
          local_policy_ptr->device_mode.nMaxActiveData   = new_nMaxActiveData;
        }

        CM_MSG_HIGH_2("MaxCap: RETRIEVE, active_subs %d, new_nSims %d",
                      cmpm_active_subs, new_nSims);

        if(cmpm_active_subs == SYS_MODEM_AS_ID_1_MASK || cmpm_active_subs == SYS_MODEM_AS_ID_2_MASK)
        {
          /* Read emergency capability from PM */
          if(!POLICYMAN_SUCCEEDED(policyman_get_e911_subphone_capability(pItem, new_nSims, max_mode_cap)))
          {
            return_val = POLICYMAN_STATUS_ERR;
            CM_MSG_HIGH_1("E911 MaxCap: RETRIEVE read error, active subs %d",cmpm_active_subs);
          }
          else
          {
            CM_MSG_HIGH_2("E911 MaxCap: RETRIEVE success,  nSims %d, size of max cap %d",
                          local_policy_ptr->device_mode.nSims, sizeof(max_mode_cap)/sizeof(uint32));

            if (local_policy_ptr->device_mode.nSims == 2 && (sizeof(max_mode_cap)/sizeof(uint32)) == 2)
            {
              CM_MSG_HIGH_2("E911 MaxCap: RETRIEVE success,  max_mode_cap 0x%x 0x%x",
                            max_mode_cap[0], max_mode_cap[1]);
            }
            if(return_val != POLICYMAN_STATUS_ERR)
            {
              return_val = POLICYMAN_STATUS_SUCCESS;
            }
          }
        }
        else
        {
          /* Read Device max capabilities */
          for(j=0; j < new_nSims; j++)
          {
            if(!POLICYMAN_SUCCEEDED(policyman_device_config_get_current_subphone_capability(pItem,
                                    j, &(max_mode_cap[j]))))
            {
              return_val = POLICYMAN_STATUS_ERR;
              CM_MSG_HIGH_1("MaxCap read error sim %d",j);
            }
            else
            {
              if (local_policy_ptr->device_mode.nSims == 2 && sizeof(max_mode_cap)/sizeof(uint32) == 2)
              {
                CM_MSG_HIGH_2("MaxCap: RETRIEVE success,  max_mode_cap[%d] 0x%x",
                              j, max_mode_cap[j]);
              }

              if(return_val != POLICYMAN_STATUS_ERR)
              {
                return_val = POLICYMAN_STATUS_SUCCESS;
              }
            }
          }
        }

        for(j=0; j < new_nSims && j < MAX_SIMS; j++)
        {
          CM_MSG_HIGH_2("MaxCap: sub %d is %d",j,max_mode_cap[j]);

          if(max_mode_cap[j] != local_policy_ptr->sub_policy[j]->policy_items.policy_max_mode_capability)
          {
            local_policy_ptr->sub_policy[j]->policy_items.policy_max_mode_capability = max_mode_cap[j];
            CM_MSG_HIGH_2("updating max mode cap sub %d to %d",j,max_mode_cap[j]);
            if ( device_change_mask != NULL )
            {
              *device_change_mask |= PM_CHG_MAX_MODE_CAP;
            }
          }
        }
      }
      break;
      /*- - - - - - - - - -- - - - - - - - - - - - - - - - - -*/

      case POLICYMAN_ITEM_VOICE_DOMAIN_PREF:
        if(sub_id > SYS_MODEM_AS_ID_NONE &&
            sub_id < MAX_SIMS &&
            sub_id < size
          )
        {
          return_val = policyman_get_voice_domain_pref(pItem,
                       &(local_policy_ptr->sub_policy[sub_id]->policy_items.voice_domain_pref),
                       &(local_policy_ptr->sub_policy[sub_id]->policy_items.is_voice_dom_pref_locked));
          if ( POLICYMAN_SUCCEEDED(return_val) &&
               cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_VOICE_DOMAIN_PREF, sub_id))
          {
            if (local_policy_ptr->sub_policy[sub_id]->policy_items.policy_mode_capability & SD_SS_MODE_PREF_LTE)
            {
              change_mask[sub_id] |= PM_CHG_VOICE_DOMAIN_PREF;
            }
            CM_MSG_HIGH_4("PM -> PMPRX: voice_domain_pref: asubs_id=%d, voice_domain_pref=%d is_locked=%d,mode_cap=%d",
                          sub_id,
                          local_policy_ptr->sub_policy[sub_id]->policy_items.voice_domain_pref,
                          local_policy_ptr->sub_policy[sub_id]->policy_items.is_voice_dom_pref_locked,
                          local_policy_ptr->sub_policy[sub_id]->policy_items.policy_mode_capability);
          }
        }
        break;
      case POLICYMAN_ITEM_CALL_MODE:
        if(sub_id > SYS_MODEM_AS_ID_NONE &&
            sub_id < MAX_SIMS &&
            sub_id < size
          )
        {
          return_val = policyman_get_call_mode_from_item(pItem, &(local_policy_ptr->sub_policy[sub_id]->policy_items.call_mode));

          if(POLICYMAN_SUCCEEDED(return_val) &&
              cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_CALL_MODE, sub_id))
          {
            change_mask[sub_id] |= PM_CHG_CALL_MODE;
            /* If current value is different than the new one, send Volte Mode indication to Clients */
            cmemg_send_call_mode_ind(sub_id, local_policy_ptr->sub_policy[sub_id]->policy_items.call_mode, FALSE);
          }
        }
        break;
      /*- - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - -*/
      case POLICYMAN_ITEM_RAT_ACQ_ORDER:
        if(sub_id > SYS_MODEM_AS_ID_NONE &&
            sub_id < MAX_SIMS &&
            sub_id < size
          )
        {
          CM_MSG_HIGH_1_ASID("PM->CM: POLICYMAN_ITEM_RAT_ACQ_ORDER: tot_chgd",  sub_id);

          return_val = policyman_get_rat_order(pItem,
                                               &(local_policy_ptr->sub_policy[sub_id]->policy_items.acq_order_pref));

          if(POLICYMAN_SUCCEEDED(return_val) &&
              cmpmprx_is_item_value_changed(pm_ptr, local_policy_ptr, POLICYMAN_ITEM_RAT_ACQ_ORDER, sub_id))
          {
            change_mask[sub_id] |= PM_CHG_RAT_ACQ_ORDER;

            for(j=0; j<local_policy_ptr->sub_policy[sub_id]->policy_items.acq_order_pref.num_rat; j++)
            {
              CM_MSG_HIGH_2("PM -> PMPRX: rat_acq_chgd sub %d: list elements=%d",
                            sub_id, local_policy_ptr->sub_policy[sub_id]->policy_items.acq_order_pref.acq_order[j]);
            }
          }
        }
        break;

      default:
        return_val = POLICYMAN_STATUS_SUCCESS;
    }
    CM_MSG_HIGH_3("Retrieve policy data pitem %d sub id %d change mask %x",pItem, sub_id, change_mask[sub_id]);
  }

  if( cmpmprx_get_num_of_sims() >= 2 )
  {
    CM_MSG_HIGH_3("retrieve policy item data returned %d, sfmode %d %d",return_val, \
                  local_policy_ptr->sub_policy[0]->policy_items.sub_feature_mode,\
                  local_policy_ptr->sub_policy[1]->policy_items.sub_feature_mode);
  }

  return return_val;
}


/*===========================================================================

FUNCTION cmpmprx_proc_msgr_retrieve_srv_req

DESCRIPTION
  Process config commands and returns if a service request can be triggered due to this PM config changes

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static boolean cmpmprx_proc_msgr_retrieve_srv_req(

  cm_policyman_cfg_s         *rpt_ptr,

  cm_policy_config_s_type    *local_policy,

  policy_change_mask         *device_local_mask_ptr)

{
  int                     i                   = 0;
  msgr_attach_s           *pAttach            = NULL;
  boolean                 is_new_policy_valid = TRUE;
  policy_change_mask      local_mask[MAX_SIMS]       = {0};
  policy_change_mask      device_local_mask   = 0;
  cm_policy_config_s_type *curr_policy        = cmpm_ptr();
  boolean                 is_any_policy_chg   = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  CM_MSG_HIGH_1("cmpmprx_proc_msgr_retrieve_srv_req: num_attach=%d",
                msgr_get_num_attach(&(rpt_ptr->msg_hdr)));

  *device_local_mask_ptr = 0;

  for (i = 0 ; i < msgr_get_num_attach(&(rpt_ptr->msg_hdr)) ; ++i)
  {

    policyman_item_collection_t const *policy_item = NULL;

    pAttach = msgr_get_attach(&(rpt_ptr->msg_hdr), i);
    if (pAttach == NULL || pAttach->type != MSGR_ATTACH_REFCNT)
    {
      CM_MSG_HIGH_1("PROC MSGR: PMPRX: Invalid attachment %d, Abort new policy",i);
      is_new_policy_valid = FALSE;
      break;
    }

    policy_item = pAttach->data.refcnt.refcnt_obj_ptr;

    if(!POLICYMAN_SUCCEEDED(cmpmprx_retrieve_and_verify_srv_req( local_policy, policy_item, local_mask, &device_local_mask )))
    {
      CM_MSG_HIGH_1("PROC MSGR: PMPRX: Unable to retrieve data for attachment %d",i);
      is_new_policy_valid = FALSE;
      break;
    }
  } /* for() */


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  is_any_policy_chg = (device_local_mask != 0);

  /*Copy device level policy mask   */
  local_policy->device_policy_changes |= device_local_mask;
  *device_local_mask_ptr |= device_local_mask;

  CM_MSG_HIGH_1("PM->CM: msgr msg, device policy changes: 0x%x",device_local_mask);

  /* Copy change mask for each SIM */
  for (i=0; i<(int)local_policy->device_mode.nSims && (i < MAX_SIMS); i++)
  {
    is_any_policy_chg = (is_any_policy_chg || (local_mask[i] != 0));
    local_policy->sub_policy[i]->policy_changes |= local_mask[i];
    CM_MSG_HIGH_4("PM->CM: Msgr policy[%d] mask = %x, local_mask[%d]=%d",i,local_policy->sub_policy[i]->policy_changes,i,local_mask[i]);
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( !is_new_policy_valid || !is_any_policy_chg )
  {
    CM_MSG_HIGH_2("PROC MSGR: PMPRX: Discard new policy valid = %d, is_any_policy_chg = %d",
                  is_new_policy_valid,is_any_policy_chg);
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return TRUE;
}


/*===========================================================================

FUNCTION cmpmprx_policy_chg_proc_no_enforce

DESCRIPTION
  Process config commands. This is almost same as cmpmprx_policy_chg_ind_proc() with exception that it just processess the policy config items
  i.e., Don't enforce any policy here.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
cm_effective_policy_change_reason_e_type cmpmprx_policy_chg_proc_no_enforce(
    sys_modem_as_id_e_type      asubs_id,
    cm_policyman_cfg_s          *rpt_ptr,
     /* Pointer to the report sent by Message Router */
    sys_non_acq_param_s_type     *non_acq_param
   )
{
  cm_policy_config_s_type *local_policy = NULL;
  policy_change_mask device_local_policy = 0;
  cm_effective_policy_change_reason_e_type ret=CM_EFFECTIVE_POLICY_NO_CHANGE;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_MSG_HIGH_1("CM->PM: cmpmprx_policy_chg_proc_no_enforce, sub %d", 
                   asubs_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( rpt_ptr == NULL )
  {
    CM_MSG_HIGH_0("=DBG= : ERROR!!! PM -> PMPRX: Incoming policy message NULL");
    return CM_EFFECTIVE_POLICY_NO_CHANGE;
  }

  /* Allocate local buffer for new memory and initialize it to current policy.
  */
  local_policy = cmpmprx_alloc_local_policy_copy();

  /* Retrieve the policy data into a local buffer. If all the policy items in
  ** the new policy are successfully retrievedand possibility of srv req being sent for that policy return success.
  */
  if (cmpmprx_proc_msgr_retrieve_srv_req(rpt_ptr,local_policy,&device_local_policy))
  {

    CM_MSG_HIGH_1("Retrieved device local policy 0x%x",device_local_policy);
    ret = CM_EFFECTIVE_POLICY_CHANGE;

  /* If only voice domain preference or any non_acq_parameter is changing inform NAS
    so that NAS could proceed with registration as no new service request will be sent.
  */
    if((local_policy->sub_policy[asubs_id]->policy_changes & ~PM_CHG_VOICE_DOMAIN_PREF) == 0 && device_local_policy == 0)
    {
      ret = CM_EFFECTIVE_NON_ACQ_POLICY_CHANGE;
      non_acq_param->as_id = asubs_id;
      non_acq_param->nonAcqValue |= BM(SYS_NONACQ_INFO_VOICE_DOMAIN_PREF);
      non_acq_param->voice_domain_pref = local_policy->sub_policy[asubs_id]->policy_items.voice_domain_pref;
    }
   }

  cmpmprx_mem_free(local_policy);
  policyman_msgr_attachments_release(&(rpt_ptr->msg_hdr));
  return ret;
}

/*===========================================================================

FUNCTION cmpmprx_session_chgd

DESCRIPTION
  Report session change and receive an early copy of the new policy
  to use from Policyman

===========================================================================*/
void    cmpmprx_session_chgd(

  mmgsdi_session_type_enum_type sessionType,
  boolean                       isAvailable,
  mmgsdi_app_enum_type          appType

)
{
  cm_policy_config_s_type *local_policy = NULL;
  cm_policyman_cfg_s      *pm_msgr_rsp  = NULL;
  policy_change_mask       device_local_policy = 0;
  policyman_session_avail_enum_t notifyType;
  cm_policy_config_s_type   *pm_ptr = cmpm_ptr();

  /* Allocate local buffer for new memory and initialize it to current policy.
  */
  local_policy = cmpmprx_alloc_local_policy_copy();

  if(isAvailable)
  {
    notifyType = SESSION_AVAILABLE;
  }
  else
  {
    notifyType = SESSION_NOT_AVAILABLE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Report session availability and retrieve new policy */

  CM_MSG_HIGH_3("Report session info, notifyType %d sessionType %d appType %d", notifyType, sessionType, appType);
  if((pm_msgr_rsp = (cm_policyman_cfg_s *)
                    policyman_report_session_availability(notifyType, sessionType, appType)))
  {
    if(cmpmprx_process_msgr_msg(pm_msgr_rsp, local_policy, &device_local_policy, FALSE))
    {
      CM_MSG_HIGH_0("Updating curr policy");
      cmpmprx_update_curr_policy(local_policy);
    }

    policyman_report_msg_free( (msgr_hdr_s *)pm_msgr_rsp );
  }

  cmpmprx_mem_free(local_policy);
}

/*===========================================================================

FUNCTION cmpmprx_timer_proc

DESCRIPTION

  Enforce pending policies that were not enforced earlier. Policies will be
  enforced here only after all subs with policy changes are ok to enforce
  them so that concurrency issues are avoided.

===========================================================================*/
void cmpmprx_timer_proc()
{
  cm_policy_config_s_type *pm_ptr = cmpm_ptr();
  cmph_s_type  *ph_ptr       = cmph_ptr();
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;

  for(sub = SYS_MODEM_AS_ID_1; (size_t)sub < cmpmprx_get_num_of_sims(); sub++)
  {
    cm_sub_level_policy_s_type  *pm_sub_ptr = cmpm_sub_ptr(sub);

    if(pm_sub_ptr == NULL)
    {
      continue;
    }

    /* Return if there are no policy changes, or CM is not eligible
    ** to consume the sub policy changes.
    */
    if(pm_ptr->device_policy_changes == 0 &&
       (!cmpmprx_read_cm_policy_consumption_eligibility(sub) ||
        !(pm_sub_ptr->policy_changes & PM_CHG_ALL)))
    {
      continue;
    }

    /* Return if the operating mode is invalid.
    */
    if(ph_ptr->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_OFFLINE ||
       ph_ptr->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_OFFLINE_CDMA ||
       ph_ptr->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_FTM )
    {
      continue;
    }

    /* Check if not ok to enforce device policy changes
    */
    if(pm_ptr->device_policy_changes != 0 &&
       !cmpmprx_is_ok_to_enforce_device_policy(CM_CALL_ID_INVALID))
    {
      continue;
    }


    /* If there are pending device or sub policy changes to enforce, but 
    ** they are still not ok to enforce, continue
    */
    if(!cmpmprx_is_ok_to_enforce_policy(pm_ptr, sub, TRUE))
    {
      continue;
    }

    /* Enforce the pending policy changes 
    */
    cmpmprx_check_and_enforce_policy();
  }

  for(sub = SYS_MODEM_AS_ID_1; (size_t)sub < cmpmprx_get_num_of_sims() && sub < ARR_SIZE(cmph_ptr()->sub_info); sub++)
  {
    if(cmmsc_get_curr_msc_max_cap(sub) != cmph_ptr()->sub_info[sub]->subs_capability)
    {
     cmph_send_subs_info(sub, NULL);
    }
  }

}

/*===========================================================================

FUNCTION  cmpmprx_is_ok_to_enforce_check

DESCRIPTION
  Check if it is ok to enforce pending device policy items. It is ok to enforce
  unless any of the following conditions are true:

  -active voice/emerg/vt/vs/sms/cs_data call (including wifi call) per device
  -device is in emergency callback mode
  -device has an active external IMS call (LTE or wifi)

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the above conditions are not present

SIDE EFFECTS
  none

===========================================================================*/
static boolean cmpmprx_is_ok_to_enforce_check
(
  sys_modem_as_id_e_type  asubs_id,
  boolean                 is_check_device_policy,
  boolean                 is_check_mt_page,
  cm_call_id_type         call_id
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* No policy enforcement is allowed during ECBM
  */
  if(cmph_is_in_emergency_cb())
  {
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if ok to enforce device policy
  */
  if(is_check_device_policy)
  {
    /* Check for any cellular call ongoing including wifi call
    */
    if(cmcall_is_there_a_call_type_no_call_id_per_device(
          CM_CALL_TYPE_VOICE,     call_id) != CM_CALL_ID_INVALID ||
        cmcall_is_there_a_call_type_no_call_id_per_device(
          CM_CALL_TYPE_EMERGENCY, call_id) != CM_CALL_ID_INVALID ||
        cmcall_is_there_a_call_type_no_call_id_per_device(
          CM_CALL_TYPE_VT,        call_id) != CM_CALL_ID_INVALID ||
        cmcall_is_there_a_call_type_no_call_id_per_device(
         CM_CALL_TYPE_VS,        call_id) != CM_CALL_ID_INVALID ||
       cmcall_is_there_a_call_type_no_call_id_per_device(
         CM_CALL_TYPE_SMS,       call_id) != CM_CALL_ID_INVALID)
    {
      return FALSE;
    }

#ifdef FEATURE_DOMAIN_SELECTION
    if(cmsds_is_ext_ims_call_active_per_device())
    {
      return FALSE;
    }
#endif

#if (defined CM_GW_SUPPORTED || defined FEATURE_CM_LTE)
    if(is_check_mt_page &&
        cmph_is_mt_page_in_prog())
    {
      return FALSE;
    }
#endif
  if(cmemg_is_ext_e911_mode())
  {
    return FALSE;
  }
return TRUE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if ok to enforce sub policy
  */
  else
  {
    /* Check for any cellular call ongoing excluding wifi call
    */
    if(cmcall_is_there_cellular_call_per_sub(
          asubs_id,
          (CM_CALL_TYPE_VOICE_MASK |
           CM_CALL_TYPE_EMERG_MASK |
           CM_CALL_TYPE_VT_MASK |
           CM_CALL_TYPE_VS_MASK |
           CM_CALL_TYPE_CS_DATA_MASK |
           CM_CALL_TYPE_SMS_MASK),FALSE) != CM_CALL_ID_INVALID)
    {
      return FALSE;
    }

#if defined FEATURE_DOMAIN_SELECTION
    if(cmsds_is_ext_volte_call_active_per_sub(asubs_id))
    {
      return FALSE;
    }
#endif

#if (defined CM_GW_SUPPORTED || defined FEATURE_CM_LTE)
    if(is_check_mt_page &&
        cmph_is_mt_page_in_prog_per_sub(asubs_id))
    {
      return FALSE;
    }
#endif
  if(cmemg_get_reason_per_sub(asubs_id) == CM_AC_REASON_THIRD_PARTY_IMS_E911)
  {
      return FALSE;
  }

}
  return TRUE;
}

/*===========================================================================

FUNCTION  cmpmprx_is_ok_to_enforce_sub_policy

DESCRIPTION
  Check if it is ok to enforce pending sub policy items. It is ok to enforce
  unless any of the following conditions are true:

  -active voice/emerg/vt/vs/sms/cs_data call (excluding wifi call) per device
  -device is in emergency callback mode
  -device has an active external VOLTE call (excluding external wifi call)

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the above conditions are not present

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_is_ok_to_enforce_sub_policy (sys_modem_as_id_e_type asubs_id)
{
  return cmpmprx_is_ok_to_enforce_check(asubs_id, FALSE, TRUE, CM_CALL_ID_INVALID);
}

/*===========================================================================

FUNCTION  cmpmprx_is_ok_to_enforce_device_policy

DESCRIPTION
  Check if it is ok to enforce pending device policy items. It is ok to enforce
  unless any of the following conditions are true:

  -active voice/emerg/vt/vs/sms/cs_data call (including wifi call) per device
  -device is in emergency callback mode
  -device has an active external IMS call (LTE or wifi)
  -MT paging in progress

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the above conditions are not present

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_is_ok_to_enforce_device_policy (

  cm_call_id_type call_id
)
{
  return cmpmprx_is_ok_to_enforce_check(SYS_MODEM_AS_ID_NONE, TRUE, TRUE, call_id);
}

/*===========================================================================

FUNCTION  cmpmprx_is_ok_to_enforce_device_policy_not_check_mt_page

DESCRIPTION
  Check if it is ok to enforce pending device policy items. It is ok to enforce
  unless any of the following conditions are true:

  -active voice/emerg/vt/vs/sms/cs_data call (including wifi call) per device
  -device is in emergency callback mode
  -device has an active external IMS call (LTE or wifi)

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the above conditions are not present

SIDE EFFECTS
  none

===========================================================================*/
boolean cmpmprx_is_ok_to_enforce_device_policy_not_check_mt_page()
{
  return cmpmprx_is_ok_to_enforce_check(SYS_MODEM_AS_ID_NONE, TRUE, FALSE, CM_CALL_ID_INVALID);
}

/*===========================================================================

FUNCTION cmpmprx_get_subs_for_ims_dereg

DESCRIPTION
  Retrieves a sandbox policy from PM that will result from a DDS switch.
  This will be called at the beginning of DDS switch processing to determine
  which subs have a capability change so that those subs can be IMS
  deregistered.

===========================================================================*/
uint8    cmpmprx_get_subs_for_ims_dereg(

  uint8 active_subs,

  sys_modem_as_id_e_type  default_data_subs,

  cm_dds_cause_e_type  dds_status
)
{
  cm_policyman_cfg_s      *pm_msgr_rsp = NULL;
  policyman_sandbox_t     *pSandbox    = NULL;
  cm_policy_config_s_type *pm_ptr      = cmpm_ptr();
  dds_info_t               dds_info;
  uint8 subs_for_ims_dereg = SYS_MODEM_AS_ID_NO_ACTIVE_MASK;

  dds_info.subsId = default_data_subs;
  dds_info.isPermanent = (dds_status == DDS_CAUSE_TEMPORARY)?FALSE:TRUE;
  pSandbox = policyman_create_sandbox();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Retrieve sandbox policy
  */
  if((pm_msgr_rsp = (cm_policyman_cfg_s *)
                    policyman_msim_report_subs_info( &dds_info, active_subs, pSandbox )))
  {
    cm_policy_config_s_type *local_policy = cmpmprx_alloc_local_policy_copy();
    policy_change_mask       device_local_policy = 0;
    sys_modem_as_id_e_type   sub = SYS_MODEM_AS_ID_NONE;

    /* Process the sandbox policy
    */
    if(cmpmprx_process_msgr_msg(pm_msgr_rsp,local_policy,&device_local_policy, TRUE))
    {
      for(sub = SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(sub); sub++)
      {
        cmph_s_type                *ph_ptr = cmph_ptr();

        sd_ss_mode_pref_e_type      ph_mode;
        sd_ss_band_pref_e_type      ph_band;
        sd_ss_band_pref_e_type      ph_tds_band;
        sys_lte_band_mask_e_type    ph_lte_band;

        sd_ss_mode_pref_e_type      old_mode;
        sd_ss_band_pref_e_type      old_band;
        sd_ss_band_pref_e_type      old_tds_band;
        sys_lte_band_mask_e_type    old_lte_band;

        sd_ss_mode_pref_e_type      new_mode;
        sd_ss_band_pref_e_type      new_band;
        sd_ss_band_pref_e_type      new_tds_band;
        sys_lte_band_mask_e_type    new_lte_band;

        boolean                 is_cap_update_required = FALSE;
        boolean                 is_tau_cap_update_required = FALSE;
        mm_sub_stk_id_s_type mm_id = {sub, MM_STACK_0};


        ph_mode      = cmph_map_cm_mode_pref_to_sd_mode_pref(CMPH_SS_MODE_PREF(sub, ph_ptr));
        ph_band      = cmutil_map_cm_band_pref_to_sd_band_pref(CMPH_SS_BAND_PREF(sub, ph_ptr));
        ph_lte_band  = CMPH_SS_LTE_BAND_PREF(sub, ph_ptr);
        ph_tds_band  = cmutil_map_cm_tds_band_pref_to_sd_tds_band_pref(CMPH_SS_TDS_BAND_PREF(sub, ph_ptr));

        /* Calculate if the new PM rat cap will result in a PS detach (or TAU)
        */

        old_mode = cmmsc_get_prev_reg_cap(mm_id).mode;
        old_band = cmmsc_get_prev_reg_cap(mm_id).band;
        old_tds_band = cmmsc_get_prev_reg_cap(mm_id).tds_band;
        old_lte_band = cmmsc_get_prev_reg_cap(mm_id).lte_band;

        new_mode     = SD_GET_COMMON_MODE(ph_mode, local_policy->sub_policy[sub]->policy_items.policy_mode_capability);
        new_band     = SD_GET_COMMON_BAND(ph_band, local_policy->sub_policy[sub]->policy_items.policy_band_capability);
        new_lte_band = SYS_LTE_BAND_MASK_COMMON(&ph_lte_band, &(local_policy->sub_policy[sub]->policy_items.policy_lte_band_capability));
        new_tds_band = SD_GET_COMMON_BAND(ph_tds_band, local_policy->sub_policy[sub]->policy_items.policy_tds_band_capability);


        is_cap_update_required = cmph_cmd_evaluate_is_cap_update_required(
                                sub,
                                cmph_map_sd_mode_pref_to_cm_mode_pref(old_mode),
                                cmutil_map_sd_band_pref_to_cm_band_pref(old_band),
                                old_lte_band,
                                cmutil_map_sd_tds_band_pref_to_cm_tds_band_pref(old_tds_band),
                                cmph_map_sd_mode_pref_to_cm_mode_pref(new_mode),
                                cmutil_map_sd_band_pref_to_cm_band_pref(new_band),
                                new_lte_band,
                                cmutil_map_sd_tds_band_pref_to_cm_tds_band_pref(new_tds_band),
                                &is_tau_cap_update_required,
                                FALSE);

        /* If any rat in the max capability has changed that is part of the user mode
        ** pref or if a PS detach is needed due to a PM rat cap change, then that sub
        ** needs to perform IMS dereg
        */
        if(is_cap_update_required && !is_tau_cap_update_required)
        {
          CM_MSG_HIGH_6("DUAL_LTE: sub %d sandbox chg, maxcap 0x%x cap 0x%x mode_pref 0x%x max_cap 0x%x is_cap_update %d",
                        sub,
                        local_policy->sub_policy[sub]->policy_items.policy_max_mode_capability,
                        local_policy->sub_policy[sub]->policy_items.policy_mode_capability,
                        ph_mode,
                        is_tau_cap_update_required,
                        is_cap_update_required);

          subs_for_ims_dereg |= BM(sub);
        }
      }
    }

    cmpmprx_mem_free(local_policy);
    policyman_report_msg_free( (msgr_hdr_s *)pm_msgr_rsp );
  }
  else
  {
    CM_MSG_MED_0("DUAL_LTE: policyman_msim_report_subs_info returned NULL");
  }

  REF_CNT_OBJ_RELEASE_IF(pSandbox);

  return subs_for_ims_dereg;
}

/*===========================================================================

FUNCTION cmpmprx_enforce_volte_state

DESCRIPTION
  Process volte state change and enforce new preference for LTE only on non-dds sub.

DEPENDENCIES
None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void    cmpmprx_enforce_volte_state(
    sys_modem_as_id_e_type     asubs_id,
    boolean                    enable_ims
)
{
  cmph_s_type *ph_ptr = cmph_ptr();
  cmph_sub_info_s_type  *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);
  cmmsc_state_machine_s_type *msimc = cmmsimc_state_machine_ptr(asubs_id);
  cm_pref_s_type *pref_ptr = NULL;
  mm_sub_stk_id_s_type     local_mm_id = {asubs_id, MM_STACK_ALL};
  boolean                  force_top_pref = FALSE;
  cm_act_update_reas_e_type act_update_reas = CM_ACT_UPDATE_REAS_USER;
  cm_orig_q_s_type  *top = cmtask_orig_para_get_top(local_mm_id);
  
  if (!msimc || !ph_sub_ptr || !top) {return;}

  /* skip processing for single sim config */
  if (cmph_is_ssim()) {return;}
  
  /* skip processing if single standby or the sub is dds sub.
  ** When volte is switched off, if the sub is non dds and is LTE only,
  ** CM will remove LTE so that device enters power save.
  ** when LTE already disabled, dont return
  */
  if (ph_ptr->device_prop.msim_prop.internal_standby_pref != SYS_MODEM_DS_PREF_DUAL_STANDBY ||
     ph_ptr->device_prop.msim_prop.default_data_subs == asubs_id ||
     (!cmmsc_is_lte_only_sub(asubs_id)
      && (msimc->stack_common_info.non_dds_lte_removal == FALSE
       || enable_ims == FALSE)))
  {
    return;
  }
  
  if((top->act_type == CM_ACT_TYPE_PH_OBJ ||
      top->act_type == CM_ACT_TYPE_POLICYMAN_RESTRICT) )
  {
    force_top_pref = TRUE;
    act_update_reas = CM_ACT_UPDATE_REAS_ACT_START;
  }

  if ((pref_ptr = cm_pref_ptr()) == NULL) return;
  memscpy(pref_ptr, sizeof(cm_pref_s_type),
          &(ph_sub_ptr->pref_info), sizeof(cm_pref_s_type));

  CM_MSG_HIGH_4_ASID("volte_state_chgd: >>PH FORCE_PREF: act_update_reas %d, force_top_pref %d, top act_type %d",
                         asubs_id,
                         act_update_reas,
                         force_top_pref,
                         top->act_type);

  (void)cmph_force_pref_on_the_fly_stack(
                                  local_mm_id,
                                  SD_SS_PREF_REAS_USER,
                                  CM_ACT_TYPE_PH_OBJ,
                                  pref_ptr,
                                  CM_OTASP_ACT_CODE_NONE,
                                  (cm_act_id_type) ph_ptr,
                                  act_update_reas,
                                  force_top_pref,
                                  cmph_get_acq_pri_order_per_sub(asubs_id),
                                  CM_DEFAULT_VALUE);
  cm_mem_free (pref_ptr);
}

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION cmpmprx_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for cmpmprx global variables

DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
void cmpmprx_qsh_mdump_collect()
{
  int sub;

  qsh_mdump_collect_high(&cmpm_local,sizeof(cm_policy_config_s_type));
  for(sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    qsh_mdump_collect_high(cmpm_local.sub_policy[sub],sizeof(cm_sub_level_policy_s_type));
  }
  qsh_mdump_collect_high(&cmpm_active_subs,sizeof(uint8));
  qsh_mdump_collect_high(&cmpm_default_data_subs,sizeof(int8));
}
#endif


/*===========================================================================

FUNCTION cmpmprx_inform_sim_state_chgd_and_evaluate_policy_chg

DESCRIPTION
  Report sim state changes to policymand and returrn TRUE if 
  this results in mode cap chg

DEPENDENCIES
  None

RETURN VALUE
  boolean - if sim state update results in mode cap chg

SIDE EFFECTS
  None

===========================================================================*/
boolean    cmpmprx_inform_sim_state_chgd_and_evaluate_policy_chg(
  sys_sim_state_e_type          sim_state,
  sys_modem_as_id_e_type        asubs_id
)
{
  cm_policy_config_s_type *local_policy = NULL;
  cm_policyman_cfg_s      *pm_msgr_rsp  = NULL;
  policy_change_mask       device_local_policy = 0;
  cm_policy_config_s_type   *pm_ptr = cmpm_ptr();
  boolean is_policy_chg = FALSE;

  /*Check sim_state should be different than last informed */
  if(cmpm_last_sim_state[asubs_id] == sim_state)
  {
    return FALSE;
  }

  /* Report sim state and retrieve new policy */
  CM_MSG_HIGH_3("Report sim info, as_id %d sim_state %d prev_sim_state %d", 
                             asubs_id, sim_state, cmpm_last_sim_state[asubs_id]);

  /* Allocate local buffer for new memory and initialize it to current policy.
  */
  local_policy = cmpmprx_alloc_local_policy_copy();
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* update last informed sim state to PM */
  cmpm_last_sim_state[asubs_id] = sim_state;

  if((pm_msgr_rsp = (cm_policyman_cfg_s *)
                    policyman_report_sim_state(asubs_id, sim_state)))
  {
    if(cmpmprx_process_msgr_msg(pm_msgr_rsp, local_policy, &device_local_policy, FALSE))
    {
      if(local_policy->sub_policy[asubs_id]->policy_changes & PM_CHG_MODE_CAP)
      {
        is_policy_chg = TRUE;
      }
      
    }
    policyman_report_msg_free( (msgr_hdr_s *)pm_msgr_rsp );
  }

  cmpmprx_mem_free(local_policy);
  return is_policy_chg;
}

