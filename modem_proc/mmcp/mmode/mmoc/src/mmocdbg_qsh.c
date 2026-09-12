/*====*====*===*====*====*====*====*====*====*====*====*====*====*====*====*
               M U L T I M O D E   C O N T R O L L E R ( MMoC )

                 Q S H  L O G G I N G  F I L E

GENERAL DESCRIPTION
  The MMoC is responsible for logging the below critical imformation for debugging purposes .
  the state information of MMOC , Which can describe the states of different protocols .
  It also have the data about the current transaction being processed .


EXTERNALIZED FUNCTIONS

  Command Interface:

  Others:

REGIONAL FUNCTIONS
  None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The MMoC task should be started before these APIs are called.


Copyright (c) 2002 - 2013 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/mmoc/src/mmocdbg_qsh.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/09/15   KC      Initial release.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "mmoc_qsh_ext.h"
#include "mmoci.h"
#include "mmocdbg.h"

#if defined(FEATURE_QSH_DUMP) || defined(FEATURE_QSH_MDUMP)

/* define a global ptr */
mmoc_qsh_dump_tag_mini_s_type  *mmoc_qsh_dump_tag_0_ptr;

void mmocdbg_copy_subsc_chgd
(
  mmoc_dbg_cmd_subsc_chgd_s_type *subsc_chgd_ptr
)MMOC_API_KEEP_UNCOMPRESSED_IN_ELF;


void  mmocdbg_dump_state_info
(
  mmoc_dbg_state_info_s_type *dump_ptr
)MMOC_API_KEEP_UNCOMPRESSED_IN_ELF;


void  mmocdbg_qsh_dump_dbg_buffer
(
  mmoc_debug_buffer_dbg_s_type *dump_ptr
)MMOC_API_KEEP_UNCOMPRESSED_IN_ELF;


void mmocdbg_qsh_copy_dump
(
  qsh_client_cb_params_s *cb_params_ptr
)MMOC_API_KEEP_UNCOMPRESSED_IN_ELF;

#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
void  mmocdbg_qsh_cb
(
  qsh_client_cb_params_s *cb_params_ptr
);


/*===========================================================================

FUNCTION mmocdbg_copy_subsc_chgd

DESCRIPTION
  Dumps the MMOC tran info

DEPENDENCIES
  mmocdbg_print_message()

RETURNS
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmocdbg_copy_subsc_chgd
(mmoc_dbg_cmd_subsc_chgd_s_type *subsc_chgd_ptr)
{
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();
  int8 sub = -1;

  subsc_chgd_ptr->nSubs = mmoc_info_ptr->dev_prop.subsc_chgd.nSubs;

  subsc_chgd_ptr->dev_prop.chg_type = mmoc_info_ptr->dev_prop.subsc_chgd.dev_prop.chg_type;
  subsc_chgd_ptr->dev_prop.nam = mmoc_info_ptr->dev_prop.subsc_chgd.dev_prop.nam;
  subsc_chgd_ptr->dev_prop.active_subs = mmoc_info_ptr->dev_prop.subsc_chgd.dev_prop.active_subs;
  subsc_chgd_ptr->dev_prop.device_mode = mmoc_info_ptr->dev_prop.subsc_chgd.dev_prop.device_mode;

  for(sub = 0; sub < MAX_SIMS; sub++)
  {
    if(mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub] != NULL)
    {
      subsc_chgd_ptr->sub_prop[sub].as_id = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->as_id;
      subsc_chgd_ptr->sub_prop[sub].active_stacks = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->active_stacks;
      subsc_chgd_ptr->sub_prop[sub].nv_context = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->nv_context;
      subsc_chgd_ptr->sub_prop[sub].is_perso_locked = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->is_perso_locked;
      subsc_chgd_ptr->sub_prop[sub].sub_avail = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->sub_avail;
      subsc_chgd_ptr->sub_prop[sub].prot_subsc_chg = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->prot_subsc_chg;
      subsc_chgd_ptr->sub_prop[sub].subs_capability = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->subs_capability;
      subsc_chgd_ptr->sub_prop[sub].orig_mode = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->orig_mode;
      subsc_chgd_ptr->sub_prop[sub].mode_pref = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->mode_pref;
      subsc_chgd_ptr->sub_prop[sub].band_pref = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->band_pref;
      subsc_chgd_ptr->sub_prop[sub].roam_pref = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->roam_pref;
      subsc_chgd_ptr->sub_prop[sub].lte_band_pref = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->lte_band_pref;
      subsc_chgd_ptr->sub_prop[sub].tds_band_pref = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->tds_band_pref;
      subsc_chgd_ptr->sub_prop[sub].only_subs_cap_change = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->only_subs_cap_change;
      subsc_chgd_ptr->sub_prop[sub].gpp_session_type = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->gpp_session_type;
      subsc_chgd_ptr->sub_prop[sub].acq_order_pref = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->acq_order_pref;
      subsc_chgd_ptr->sub_prop[sub].srv_domain_pref = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->srv_domain_pref;
      subsc_chgd_ptr->sub_prop[sub].gpp2_session_type = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->gpp2_session_type;
      subsc_chgd_ptr->sub_prop[sub].prl_pref = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->prl_pref;
      subsc_chgd_ptr->sub_prop[sub].hybr_pref = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->hybr_pref;
      subsc_chgd_ptr->sub_prop[sub].ue_mode = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->ue_mode;
      subsc_chgd_ptr->sub_prop[sub].is_ue_mode_substate_srlte = mmoc_info_ptr->dev_prop.subsc_chgd.sub_prop[sub]->is_ue_mode_substate_srlte;
    }
    else
    {
      memset(&subsc_chgd_ptr->sub_prop[sub], 0, sizeof(mmoc_dbg_subs_data_s_type));
    }
  }

}


/*===========================================================================

FUNCTION mmocdbg_dump_state_info

DESCRIPTION
  Dumps the MMOC state & mmoc_debug_buffer

DEPENDENCIES
  mmocdbg_print_message()

RETURNS
  None

SIDE EFFECTS
  None

===========================================================================*/
void  mmocdbg_dump_state_info(mmoc_dbg_state_info_s_type *dump_ptr)
{
  mmoc_state_info_s_type *mmoc_info_ptr = mmoc_get_state_info_ptr();
  int8 sub = -1;
  int8 stk = -1;

  dump_ptr->nSubs = mmoc_info_ptr->nSubs;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Copy device info */

  dump_ptr->dev_prop.trans = mmoc_info_ptr->dev_prop.trans;
  dump_ptr->dev_prop.trans_state = mmoc_info_ptr->dev_prop.trans_state;
  memscpy(&dump_ptr->dev_prop.trans_info, sizeof(dump_ptr->dev_prop.trans_info),
          &mmoc_info_ptr->dev_prop.trans_info, sizeof(mmoc_info_ptr->dev_prop.trans_info));

  mmocdbg_copy_subsc_chgd(&dump_ptr->dev_prop.subsc_chgd);

  dump_ptr->dev_prop.is_sd_initialized = mmoc_info_ptr->dev_prop.is_sd_initialized;
  dump_ptr->dev_prop.is_sd_init_called = mmoc_info_ptr->dev_prop.is_sd_init_called;
  dump_ptr->dev_prop.trans_id = mmoc_info_ptr->dev_prop.trans_id;
  dump_ptr->dev_prop.oprt_mode = mmoc_info_ptr->dev_prop.oprt_mode;
  dump_ptr->dev_prop.true_oprt_mode = mmoc_info_ptr->dev_prop.true_oprt_mode;
  dump_ptr->dev_prop.is_ph_stat_sent = mmoc_info_ptr->dev_prop.is_ph_stat_sent;
  dump_ptr->dev_prop.deact_req_idx.asubs_id = mmoc_info_ptr->dev_prop.deact_req_idx.asubs_id;
  dump_ptr->dev_prop.deact_req_idx.stk_id = mmoc_info_ptr->dev_prop.deact_req_idx.stk_id;
  dump_ptr->dev_prop.insanity_count = mmoc_info_ptr->dev_prop.insanity_count;
  dump_ptr->dev_prop.is_standby_sleep = mmoc_info_ptr->dev_prop.is_standby_sleep;
  dump_ptr->dev_prop.is_redir_allowed = mmoc_info_ptr->dev_prop.is_redir_allowed;
  dump_ptr->dev_prop.prev_standby_pref = mmoc_info_ptr->dev_prop.prev_standby_pref;
  dump_ptr->dev_prop.standby_pref = mmoc_info_ptr->dev_prop.standby_pref;
  dump_ptr->dev_prop.prev_active_subs = mmoc_info_ptr->dev_prop.prev_active_subs;
  dump_ptr->dev_prop.active_subs = mmoc_info_ptr->dev_prop.active_subs;
  dump_ptr->dev_prop.device_mode = mmoc_info_ptr->dev_prop.device_mode;
  dump_ptr->dev_prop.prev_device_mode = mmoc_info_ptr->dev_prop.prev_device_mode;
  dump_ptr->dev_prop.max_sanity_time_multiple = mmoc_info_ptr->dev_prop.max_sanity_time_multiple;
  dump_ptr->dev_prop.is_scan_permission = mmoc_info_ptr->dev_prop.is_scan_permission;
  dump_ptr->dev_prop.is_buffer_auto_deact_ind = mmoc_info_ptr->dev_prop.is_buffer_auto_deact_ind;
  dump_ptr->dev_prop.last_stop_req_sent_timestamp = mmoc_info_ptr->dev_prop.last_stop_req_sent_timestamp;
  dump_ptr->dev_prop.onebuild_feature = mmoc_info_ptr->dev_prop.onebuild_feature;
  dump_ptr->dev_prop.dual_switch_subs = mmoc_info_ptr->dev_prop.dual_switch_subs;
  dump_ptr->dev_prop.pri_slot = mmoc_info_ptr->dev_prop.pri_slot;
  dump_ptr->dev_prop.is_ps_priority = mmoc_info_ptr->dev_prop.is_ps_priority;
  dump_ptr->dev_prop.sub_with_3gpp2 = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
  dump_ptr->dev_prop.prev_sub_with_3gpp2 = mmoc_info_ptr->dev_prop.prev_sub_with_3gpp2;
  dump_ptr->dev_prop.sub_with_cap_chg = mmoc_info_ptr->dev_prop.sub_with_cap_chg;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Copy subscription info */

  for(sub = 0; sub < dump_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(mmoc_info_ptr->sub_prop[sub] != NULL)
    {
      dump_ptr->sub_prop[sub].nam = mmoc_info_ptr->sub_prop[sub]->nam;
      dump_ptr->sub_prop[sub].sub_avail = mmoc_info_ptr->sub_prop[sub]->sub_avail;
      dump_ptr->sub_prop[sub].active_stacks = mmoc_info_ptr->sub_prop[sub]->active_stacks;
      dump_ptr->sub_prop[sub].prev_active_stacks = mmoc_info_ptr->sub_prop[sub]->prev_active_stacks;
      dump_ptr->sub_prop[sub].prot_subsc_chg = mmoc_info_ptr->sub_prop[sub]->prot_subsc_chg;
      dump_ptr->sub_prop[sub].hdr_deact_activate_stack = mmoc_info_ptr->sub_prop[sub]->hdr_deact_activate_stack;
      dump_ptr->sub_prop[sub].subs_feature = mmoc_info_ptr->sub_prop[sub]->subs_feature;
      dump_ptr->sub_prop[sub].pm_subs_feature = mmoc_info_ptr->sub_prop[sub]->pm_subs_feature;
      dump_ptr->sub_prop[sub].subs_capability = mmoc_info_ptr->sub_prop[sub]->subs_capability;
      dump_ptr->sub_prop[sub].ps_enabled = mmoc_info_ptr->sub_prop[sub]->ps_enabled;
      dump_ptr->sub_prop[sub].nStacks = mmoc_info_ptr->sub_prop[sub]->nStacks;

      for(stk = 0; stk < dump_ptr->sub_prop[sub].nStacks && stk < MAX_STACKS; stk++)
      {
        if(mmoc_info_ptr->sub_prop[sub]->stack_prop[stk] != NULL)
        {
          dump_ptr->sub_prop[sub].stack_prop[stk].prot_state = mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->prot_state;
          dump_ptr->sub_prop[sub].stack_prop[stk].prot_dormant = mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->prot_dormant;
          dump_ptr->sub_prop[sub].stack_prop[stk].is_gwl_deact_sent = mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->is_gwl_deact_sent;
          dump_ptr->sub_prop[sub].stack_prop[stk].is_suspend = mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->is_suspend;
          dump_ptr->sub_prop[sub].stack_prop[stk].suspend_reas_mask = mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->suspend_reas_mask;
          dump_ptr->sub_prop[sub].stack_prop[stk].last_prot_state = mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->last_prot_state;
        }
        else
        {
          memset(&dump_ptr->sub_prop[sub].stack_prop[stk], 0, sizeof(mmoc_dbg_state_info_stack_prop_s_type));
        }
      }
    }
    else
    {
      memset(&dump_ptr->sub_prop[sub], 0, sizeof(mmoc_dbg_state_info_sub_prop_s_type));
    }
  }
} /* mmocdbg_dump_state_info */



/*===========================================================================

FUNCTION mmocdbg_qsh_dump_dbg_buffer

DESCRIPTION
  Dumps the mmoc_debug_buffer

DEPENDENCIES
  mmocdbg_print_message()

RETURNS
  None

SIDE EFFECTS
  None

===========================================================================*/
void  mmocdbg_qsh_dump_dbg_buffer
(mmoc_debug_buffer_dbg_s_type *dump_ptr)
{
  mmoc_debug_buffer_s_type *mmoc_debug_buffer_ptr = mmoc_get_dbg_buff_ptr();
  uint8 cnt = 0;
  uint8 rpt_cnt = 0;
  dump_ptr->dbg_buf_idx = mmoc_debug_buffer_ptr->dbg_buf_idx;

  for(cnt=0; cnt<MMOC_DBG_MAX_DEBUG_BUFFER_SIZE; cnt++)
  {
    dump_ptr->dbg_buf[cnt].trans_id = mmoc_debug_buffer_ptr->dbg_buf[cnt].trans_id;
    dump_ptr->dbg_buf[cnt].trans_name = mmoc_debug_buffer_ptr->dbg_buf[cnt].trans_name;

    memscpy(dump_ptr->dbg_buf[cnt].addl_info,
            sizeof(dump_ptr->dbg_buf[cnt].addl_info),
            mmoc_debug_buffer_ptr->dbg_buf[cnt].addl_info,
            sizeof(mmoc_debug_buffer_ptr->dbg_buf[cnt].addl_info));

    for(rpt_cnt=0; rpt_cnt<MMOC_DBG_MAX_RPT_COUNT; rpt_cnt++)
    {
      dump_ptr->dbg_buf[cnt].rpt_queue[rpt_cnt].rpt_name =
        mmoc_debug_buffer_ptr->dbg_buf[cnt].rpt_queue[rpt_cnt].rpt_name;

      dump_ptr->dbg_buf[cnt].rpt_queue[rpt_cnt].task_name =
        mmoc_debug_buffer_ptr->dbg_buf[cnt].rpt_queue[rpt_cnt].task_name;

      memscpy(dump_ptr->dbg_buf[cnt].rpt_queue[rpt_cnt].prot_state,
              sizeof(dump_ptr->dbg_buf[cnt].rpt_queue[rpt_cnt].prot_state),
              mmoc_debug_buffer_ptr->dbg_buf[cnt].rpt_queue[rpt_cnt].prot_state,
              sizeof(mmoc_debug_buffer_ptr->dbg_buf[cnt].rpt_queue[rpt_cnt].prot_state));
    }
  }


}

/*===========================================================================

FUNCTION mmocdbg_qsh_copy_dump

DESCRIPTION
  Copies the DUMP to the pointer sent by QSH .

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void mmocdbg_qsh_copy_dump(qsh_client_cb_params_s *cb_params_ptr)
{
  qsh_client_dump_collect_s *dump_params_ptr = &cb_params_ptr->action_params.dump_collect;

  if(dump_params_ptr->dump_iovec.size_bytes >=
      sizeof(mmoc_qsh_dump_tag_mini_s_type))
  {
    if(QSH_DUMP_TAG_ENABLED(dump_params_ptr->dump_tag_mask,MMOC_QSH_DUMP_TAG_MINI))
    {
      qsh_client_action_done_s action_done;
      mmoc_qsh_dump_tag_mini_s_type *mini_dump = (mmoc_qsh_dump_tag_mini_s_type *)dump_params_ptr->dump_iovec.addr;

      qsh_client_dump_tag_hdr_init(&mini_dump->hdr,
                                   MMOC_QSH_DUMP_TAG_MINI,
                                   sizeof(mmoc_qsh_dump_tag_mini_s_type));

      mmocdbg_dump_state_info(&mini_dump->state_info);

      mmocdbg_qsh_dump_dbg_buffer(&mini_dump->dbg_buffer);

      qsh_client_action_done_init(&action_done);
      action_done.cb_params_ptr = cb_params_ptr;
      action_done.params.dump_collect.size_written_bytes
        = sizeof(mmoc_qsh_dump_tag_mini_s_type);
      action_done.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;
      qsh_client_action_done(&action_done);
    }
  }

}

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION mmocdbg_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for MMOC global variables

DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR 
void mmocdbg_qsh_mdump_collect()
{
  mmoc_qsh_mdump_collect();

#ifndef FEATURE_MMOC_LOW_MEM_TARGET
  mmocdebug_qsh_mdump_collect();
#endif

}
#endif

/*===========================================================================

FUNCTION mmocdbg_qsh_cb

DESCRIPTION
  Processes the QSH request .
  Currently we handle only QSH_ACTION_DUMP_COLLECT .
DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

Note
use MMOC_API_KEEP_UNCOMPRESSED_IN_ELF ,
which keeps the API in uncompressed section in th elf
 this will be used by QSH dump collection ,
since after the crash happened uncompressed functions
compressed functions wont be accesible
===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
void  mmocdbg_qsh_cb(qsh_client_cb_params_s *cb_params_ptr)
{

  if (cb_params_ptr == NULL)
  {
    return;
  }

  if(cb_params_ptr->action & QSH_ACTION_DUMP_COLLECT)
  {
    mmocdbg_qsh_copy_dump(cb_params_ptr);
  }
  #ifdef FEATURE_QSH_MDUMP
  if(cb_params_ptr->action & QSH_ACTION_MDUMP)
  {
    mmocdbg_qsh_mdump_collect();
  }
  #endif  //FEATURE_QSH_MDUMP  
  
}

/*===========================================================================

FUNCTION mmocdbg_qsh_init

DESCRIPTION
  Initilize CM QSH interface.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void  mmocdbg_qsh_init()
{
  qsh_client_reg_s qsh_client;

  qsh_client_reg_init(&qsh_client);

  qsh_client.client = QSH_CLT_MMOC;
  qsh_client.major_ver = MMOC_QSH_MAJOR_VER;
  qsh_client.minor_ver = MMOC_QSH_MINOR_VER;
  qsh_client.client_cb_ptr = mmocdbg_qsh_cb;
  qsh_client.cb_action_support_mask = QSH_ACTION_DUMP_COLLECT
#ifdef FEATURE_QSH_MDUMP
                                    | QSH_ACTION_MDUMP 
#endif
                                    ;

  qsh_client.dump_info.max_size_bytes = sizeof(mmoc_qsh_dump_tag_mini_s_type);

  qsh_client_reg(&qsh_client);

#ifdef FEATURE_QSH_MDUMP
  qsh_add_thread_id(qsh_client.client);
#endif

}



#endif
