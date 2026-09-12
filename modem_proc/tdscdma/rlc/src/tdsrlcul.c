/*===========================================================================
               U P L I N K   R A D I O   L I N K   C O N T R O L

DESCRIPTION
  This file provides the functions to process RLC Uplink logical channels.
  
EXTERNALIZED FUNCTIONS
  List any functions that are externalized in this file.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcul.c_v   1.44   12 Jun 2002 18:44:10   tliou  $
$Header: //components/rel/tdscdma.mpss/4.3/rlc/src/tdsrlcul.c#2 $ $DateTime: 2021/02/04 23:39:42 $ $Author: pwbldsvc $


when      who     what, where, why
--------  ---     ---------------------------------------------------------- 
03/30/15  sarao    FR26213 for QSH support 
1/7/14    sarao  Creation and usage of MEDIUM SIZE DSM BUFFER
10/13/14  sarao  CFCM changes for RLC
09/08/11  mkh     Events renamed to TDS
02/07/11  ps      Removal of tdsmac_rlc_write_data_section() for TDD HSUPA
10/24/10  guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "customer.h"
#include "tdsrlcul.h"
#include "tdsrlci.h"
#include "tdsrlcultm.h"
#include "tdsrlculum.h"
#include "tdsrlculam.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccmd_l2.h"
#include "tdsrrcscmgr.h"

#include "tdsmacrlcif.h"

#include "event_defs.h"
#include "event.h"

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  #include "tdsrlcullog.h"
#endif

#include "tdsrlcfc.h"

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
  #include "tdsrlcfc.h"
  #include "dsumtspdpreg.h"
#ifdef FEATURE_DATA_PDCP
    #include "pdcp.h"
#else
    #include "dswpsd.h"
#endif //FEATURE_DATA_PDCP 
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL

#ifdef FEATURE_QXDM_DSM_TRACING
#include "fc_dsm_trace.h"
#endif
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif
#ifdef FEATURE_MODEM_RCINIT
#include "tdsl1m_rcinit.h"
#endif

#include "tdsrlcdl.h"

#ifdef FEATURE_QSH_EVENT_METRIC
#include "tdsrlc_qsh_ext.h"
#endif

extern boolean tdsrlci_dl_pdu_cipher_log_on;
#ifdef FEATURE_QXDM_DSM_TRACING
  fc_dsm_event_hits_stat_type tdsrlc_fc_dsm_event_hits[FC_MAX_POOL_ID];
  uint32 tdsnum_rlc_non_srb_entities_up = 0;
#endif

#ifdef FEATURE_TDSCDMA_MEDIUM_DSM_BUFFER   //Enable this feature when we need RLC PDU size more than 128 bytes.
#define TDSCDMA_DSM_MEDIUM_ITEM_ARRAY_SIZ (TDSCDMA_DSMI_DS_MEDIUM_ITEM_CNT_STATIC * \
  (TDSCDMA_DSMI_MEDIUM_ITEM_SIZ + DSM_ITEM_HEADER_SIZE + 31))
static uint32 tdscdma_dsm_ds_medium_item_array[TDSCDMA_DSM_MEDIUM_ITEM_ARRAY_SIZ/4];
dsm_pool_mgmt_table_type tdscdma_dsm_ds_medium_item_pool;
#endif  //FEATURE_TDSCDMA_MEDIUM_DSM_BUFFER


/* -----------------------------------------------------------------------
** Variables Defineitions
** ----------------------------------------------------------------------- */
#ifdef FEATURE_QSH_EVENT_METRIC
#define TDSRLC_QSH_MAX_METRICS 1
tdsrlc_qsh_status_e_type tdsrlc_qsh_metric_coll_status[TDSCDMA_NUM_SUBS];
tdsrlc_qsh_metric_address_type tdsrlc_metric_address[TDSCDMA_NUM_SUBS];
tdsrlc_qsh_metric_data_stats_s tdsrlc_qsh_metrics;
uint8 tdsrlc_bad_status_pdu_cnt =0;
extern uint32 tdsrlc_qsh_dl_rlc_resets;
extern uint32 tdsrlc_qsh_dl_tput;

extern void tdsrlc_qsh_debug_callback(qsh_client_cb_params_s *param);

//Test support:
qsh_ext_metric_cfg_s  tdsrlc_qsh_metric_cfg_arr[TDSRLC_QSH_MAX_METRICS];

#endif


q_type tdsrlc_fc_cmd_q;
q_type tdsrlc_fc_free_cmd_q;
#define TDSRLC_MAX_FC_CMD_BUF 10
tdsrlc_fc_cmd_type  tdsrlc_fc_cmd_pool[TDSRLC_MAX_FC_CMD_BUF];

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
//rex_timer_type tdsul_tmr_log_blk;         /* Timer for qxdm logging purposes */
tdsrlc_statistic_timer_type ul_statistic_timer;

#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
  uint16 tdsrlc_ul_channel_cnt = 0;
#endif
#endif 

tdsrlc_ul_hdr_buff_type  tdsrlc_ul_hdr;
tdsrlc_pdu_dsm_type      tdsrlc_pdu_dsm_pool; 
tdsrlc_io_vec_list_type  tdsrlc_io_vec_list;
rex_timer_type        tdsrlc_ul_am_base_tmr;
                              /* This is base 10ms timer for RLC AM channels */
                              /* at expiry, periodic status/ control related */
                              /* procedures are triggered*/
boolean               tdsrlc_ul_am_base_tmr_start = FALSE;


/* Table to store the RLC-ids during RESET-status-timer messages */
rlc_lc_id_type  tdsrlc_rst_satus_tmr_tbl[TDSUE_MAX_AM_ENTITY];

typedef struct {
  rlc_lc_id_type    rlc_id;
  tdsrlc_error_e_type  err_type;
} tdsam_reset_id_type;

/* UL control block */
tdsrlci_uplink_table_type tdsuplink_table;

/* rlc_id to ul control block index table */
uint8 tdsul_ctl_blk_id_tbl[TDSUE_MAX_UL_LOGICAL_CHANNEL];

/* Table to store the RLC entity infor which goes to RRC  */
tdsam_reset_id_type tdsam_reset_id_table[TDSUE_MAX_AM_ENTITY];

#ifndef FEATURE_TDSCDMA_SRB_RESET
boolean tdsam_ignore_reset_table[TDSMAC_UL_MAX_RB_ID] = {FALSE};
#else
uint8 tdsam_ignore_reset_num_table[TDSMAC_UL_MAX_RB_ID] = {0};
uint8 tdsam_ignore_bad_status_num = 0;
#endif

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
rex_crit_sect_type tdsam_reset_id_table_mutex;
#endif

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  /* 5s log Timer signaling control block */
  tdsrlc_ul_tm_config_type tdsul_tm_log_ptr; 
  tdsrlc_ul_um_config_type tdsul_um_log_ptr; 
  tdsrlc_am_config_req_type tdsul_am_log_ptr; 
  tdsrlc_ul_log_state_type tdsul_log_state;
  uint16 tdslogging_params[TDSRLCLOG_MAX][2];

  boolean tdsrlci_ul_pdu_cipher_log_on = FALSE;
  tdsrlc_ul_pdu_cipher_info_type tdsrlci_ul_pdu_cipher_log;
#endif

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
uint16 tdsrlc_acti_non_srb_am_ch = 0;
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

uint32 tdsrlc_debug_var = 0;


void tdsrrccu_rlc_ul_data_ind (dsm_watermark_type *wm_ptr, void *ctrl_blk);

boolean tdsrlci_inter_ho_happen = FALSE;

extern rex_crit_sect_type tdsrlc_dl_data_process_mutex;

/* =======================================================================
**                            Function Definitions
** ======================================================================= */
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA 
/*===========================================================================

FUNCTION tdsmac_send_t1_opt_signal

DESCRIPTION
 This function will be called by RLC to send TDSMAC_HS_T1_OPT_IND_SIG to DL HS task.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/

extern void tdsmac_send_t1_opt_signal
(
  void
);
#endif
/*===========================================================================

FUNCTION TDSRLC_DUMP_UL_CTL_BLK_ID_TBL

DESCRIPTION
  dumps uplink_table index values of LCs within ul_ctl_blk_id_tbl

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dump_ul_ctl_blk_id_tbl(void)
{
  uint32 i = 0;

  for (i = 0; i < TDSUE_MAX_UL_LOGICAL_CHANNEL; i++)
  {
    if (TDSINIT_CTL_BLK_ID != tdsul_ctl_blk_id_tbl[i])
    {
      TDSRLC_MSG2(MSG_LEGACY_HIGH,"RLC_DBG: UL LC %d is at index %d in uplink_table",i,tdsul_ctl_blk_id_tbl[i]);
    }
  }
}

/*===========================================================================

FUNCTION TDSRLC_SANITIZE_UL_CTL_BLK_ID_TBL

DESCRIPTION
  Checks for any duplicate uplink_table index values of two different LCs within ul_ctl_blk_id_tbl

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_sanitize_ul_ctl_blk_id_tbl(void)
{
  uint32 i = 0;
  uint32 j = 0;

  for (i = 0; i < TDSUE_MAX_UL_LOGICAL_CHANNEL; i++)
  {
    if (TDSINIT_CTL_BLK_ID != tdsul_ctl_blk_id_tbl[i])
    {
      for (j = (i+1); j < TDSUE_MAX_UL_LOGICAL_CHANNEL; j++)
      {
        if ((TDSINIT_CTL_BLK_ID != tdsul_ctl_blk_id_tbl[j]) 
          && (tdsul_ctl_blk_id_tbl[j] == tdsul_ctl_blk_id_tbl[i]))
        {
          ERR_FATAL("RLC_ERR: UL LC %d & LC %d are at same index %d in uplink_table", i, j, tdsul_ctl_blk_id_tbl[i]);
        }
      }
    }
  }
}

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
/*===========================================================================

FUNCTION RLC_UL_APP_NEW_WINSUFI

DESCRIPTION
  Assigns new window size required by TDSAPP_CLOSE and transmits new window sufi.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_app_new_winsufi
(
  rlc_lc_id_type rlc_id,
  tdsrlci_peer_tx_win_size_e_type peer_win
)
{
  tdsrlci_ul_ctrl_blk_type *ul_ctrl_blk_ptr = NULL;

  if (rlc_id < TDSRLC_UL_AM_USR_PLANE_START_LC_ID)
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"No Flow Control on SRB LC asked by PS,%d",rlc_id);
    return;
  }
  ul_ctrl_blk_ptr = tdsrlci_get_ctl_blk_ptr(rlc_id);
  if(!ul_ctrl_blk_ptr)
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC LC ID:%d not active",rlc_id);
    return;
  }
  tdsmulti_client_fc_arbitrate(&(ul_ctrl_blk_ptr->ctrl.am_ctrl.multiclient_dl_fc_arbitarator), 
                     (uint16)TDSRLC_DL_FC_ENTITY_PS_WM, peer_win, TRUE);

}

#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL

/*===========================================================================

FUNCTION UL_ASSIGN_CTL_BLK

DESCRIPTION
  Assign Uplink control block for an RLC entity.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint8 tdsul_assign_ctl_blk(void)
{
  int i;
  uint8 ret_val = TDSINIT_CTL_BLK_ID;

    /*Locking both EUL and R99 Mutex*/
    (void)TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);

  for (i=0; i<TDSUE_MAX_UL_RLC_ENTITY; i++)
  {
    if (tdsuplink_table.ctrl_blk[i].lc_state == TDSUL_NULL_STATE)
    {
      ret_val = (uint8)i;
      break;
    }
  }

    TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
    TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);

  if (i == TDSUE_MAX_UL_RLC_ENTITY)
  {
    //TDSRLC_MSG0(MSG_LEGACY_ERROR,"Too many RLC entities");
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"Too many RLC entities,%d",i);
  }
  
  return ret_val;
}/* tdsul_assign_ctl_blk() */

#ifdef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
/*===========================================================================

FUNCTION RLC_UL_CRIT_SECT_INIT

DESCRIPTION
  Initializes the RLC Uplink critcical sections.
  
DEPENDENCIES
  This function should be called at task initialization.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_crit_sect_init(void)
{
  /*-------------------------------------------------------------------------
    Initialize UL R99 mutex.
  -------------------------------------------------------------------------*/
  tdsrlc_enh_initialize_mutex();
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  rex_init_crit_sect(&tdsrlc_reset_table_sem);
  rex_init_crit_sect(&tdsam_reset_id_table_mutex);
#endif
}
#endif

/*===========================================================================

FUNCTION RLC_UL_INIT

DESCRIPTION
  Initializes the RLC Uplink Logical channel table.
  
DEPENDENCIES
  This function should be called at task start up.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_init(void)
{
  int i;

  dsm_item_type *item_ptr = NULL;

  TDSRLC_MSG0(MSG_LEGACY_LOW,"Initialize RLC UL");
#ifndef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
  /*-------------------------------------------------------------------------
    Initialize UL R99 mutex.
  -------------------------------------------------------------------------*/
  tdsrlc_enh_initialize_mutex();
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  rex_init_crit_sect(&tdsrlc_reset_table_sem);
  rex_init_crit_sect(&tdsam_reset_id_table_mutex);
#endif
#endif
  /*-------------------------------------------------------------------------
    Initialize the uplink table.
  -------------------------------------------------------------------------*/
  (void)memset(&tdsuplink_table, 0, sizeof(tdsuplink_table));

  /*-------------------------------------------------------------------------
    Initialize the mapping table for control block id and channel id.
  -------------------------------------------------------------------------*/
  for (i=0; i<TDSUE_MAX_UL_LOGICAL_CHANNEL; i++)
  {
    tdsul_ctl_blk_id_tbl[i] = TDSINIT_CTL_BLK_ID;
  }
#ifndef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  rex_init_crit_sect(&tdsam_reset_id_table_mutex);
#endif
#endif
  for (i=0; i<TDSUE_MAX_AM_ENTITY; i++)
  {
    /* Init AM RESET channel ID table */
    tdsam_reset_id_table[i].rlc_id = TDSINIT_CTL_BLK_ID;
    tdsam_reset_id_table[i].err_type = TDSRLC_RESET;
  }

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  //rex_def_timer(&tdsul_tmr_log_blk,tds_l2_ul_get_tcb(), TDSRLC_UL_AM_LOG_TIMER_EXP_SIG);
  //(void) rex_set_timer(&tdsul_tmr_log_blk,(uint32)TDSRLC_AM_LOG_TIMER_BASE); 
  ul_statistic_timer.statistic_timer_value = TDSRLC_AM_LOG_TIMER_BASE;

  (void)memset(&tdsrlci_ul_pdu_cipher_log, 0, sizeof(tdsrlci_ul_pdu_cipher_log));
#endif

  memset(&tdsrlc_pdu_dsm_pool, 0, sizeof(tdsrlc_pdu_dsm_type));

  for(i = 0; i < TDSMAX_UL_PDUS_PER_TTI; ++i)
  {
    /*-----------------------------------------------------------------------
      Pre-alloction of dsm items for building data & control PDUs for R99 
      mapped logical channels.
      Assumption : DSM_DS_SMALL_ITEM_POOL is sufficient now. If bigger R99
      PDU Size is configured, need to select from bigger DSM Pool
    -----------------------------------------------------------------------*/  
    /* Sarao: Initializing medium sized DSM buffer for control PDUs */
#ifdef FEATURE_TDSCDMA_MEDIUM_DSM_BUFFER

    dsm_init_pool(TDSCDMA_DSM_DS_MEDIUM_ITEM_POOL,(uint8*)tdscdma_dsm_ds_medium_item_array,
    TDSCDMA_DSM_MEDIUM_ITEM_ARRAY_SIZ, TDSCDMA_DSMI_MEDIUM_ITEM_SIZ);
	
    if ((item_ptr = dsm_new_buffer(TDSCDMA_DSM_DS_MEDIUM_ITEM_POOL)) == NULL)
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: Control PDU DSM pool pre-allocation from medium pool %d",i);
      break;
    }

#else  
    if ((item_ptr = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL)) == NULL)
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: Control PDU DSM pool pre-allocation from dsm small pool %d",i);
      break;
    }

#endif



    tdsrlc_pdu_dsm_pool.item_ptr[i] = item_ptr;
  }

  /*-------------------------------------------------------------------------
    Perform the AM initialization.
  -------------------------------------------------------------------------*/
  tdsrlci_ul_am_init();

  /*-------------------------------------------------------------------------
    Perform the UM initialization.
  -------------------------------------------------------------------------*/
  tdsrlci_ul_um_init();


  (void)q_init(&tdsrlc_fc_cmd_q);
  (void) q_init(&tdsrlc_fc_free_cmd_q);

  for ( i=0;i < TDSRLC_MAX_FC_CMD_BUF; i++)
  {
    (void) q_link(&tdsrlc_fc_cmd_pool[i], &tdsrlc_fc_cmd_pool[i].link);
    q_put (&tdsrlc_fc_free_cmd_q, &tdsrlc_fc_cmd_pool[i].link);
  }

}/* tdsrlc_ul_init() */


#if 0 //CFCM_related_obsolete_1
//CFCM handle various clients so we dont need these

/*===========================================================================

FUNCTION  RLCI_FC_DSM_CB

DESCRIPTION
  The DSM item pool goes to the DON'T-EXCEED level. RESET RLC channels 
  for user plain to release DSM items on hold.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_fc_dsm_cb
(
  dsm_mempool_id_enum_type  dsm_pool_id,
  dsm_mem_level_enum_type   dsm_mem_level, 
  dsm_mem_op_enum_type      dsm_mem_op
)
{
  tdsrlc_fc_cmd_entity_type entity = TDSRLC_FC_CMD_NULL_ENTITY;
#ifdef FEATURE_QXDM_DSM_TRACING
  fc_dsm_pool_id_type fc_pool_id;
#endif
  TDSRLC_MSG3(MSG_LEGACY_HIGH,"DSM_MULTI_FC : pool= %d mem_level= %d  memop = %d", dsm_pool_id, dsm_mem_level, dsm_mem_op);

/* Check to verify we are getting the call back from the correct DSM pool
/ (based on small item FC or large item FC) and in the right state*/
  if (
      (dsm_pool_id != DSM_DS_LARGE_ITEM_POOL) &&
      (dsm_pool_id != DSM_DS_SMALL_ITEM_POOL) &&
      (dsm_pool_id != DSM_DUP_ITEM_POOL) 
     )
  {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"Invalid dsm pool %d ",dsm_pool_id);
      return;
  }
  if( (dsm_mem_level != DSM_MEM_LEVEL_LINK_LAYER_DNE) &&
      (dsm_mem_level != DSM_MEM_LEVEL_RLC_FEW) &&
      (dsm_mem_level != DSM_MEM_LEVEL_RLC_MANY))
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"Invalid memlevel %d ",dsm_mem_level);
    return;
  }

  if(( (dsm_mem_level == DSM_MEM_LEVEL_LINK_LAYER_DNE) ||
       (dsm_mem_level == DSM_MEM_LEVEL_RLC_FEW) )&&
     (dsm_mem_op != DSM_MEM_OP_NEW))
  {
    TDSRLC_MSG2(MSG_LEGACY_ERROR,"Invalid mem_op %d for memlevel %d ",dsm_mem_op,dsm_mem_level);
    return;
  }
  if( (dsm_mem_level == DSM_MEM_LEVEL_RLC_MANY) &&
      (dsm_mem_op != DSM_MEM_OP_FREE))
  {
    TDSRLC_MSG2(MSG_LEGACY_ERROR,"Invalid mem_op %d for memlevel %d ",dsm_mem_op,dsm_mem_level);
    return;
  }

  if (dsm_pool_id == DSM_DS_LARGE_ITEM_POOL)
  {
    entity = TDSRLC_FC_CMD_LARGE_DSM_ENTITY;
#ifdef FEATURE_QXDM_DSM_TRACING
    fc_pool_id = FC_LARGE_POOL;
#endif
  }
  if (dsm_pool_id == DSM_DUP_ITEM_POOL)
  {
    entity = TDSRLC_FC_CMD_DUP_ENTITY;
#ifdef FEATURE_QXDM_DSM_TRACING
    fc_pool_id = FC_DUP_POOL;
#endif
  }
  if (dsm_pool_id == DSM_DS_SMALL_ITEM_POOL)
  {
    entity = TDSRLC_FC_CMD_SMALL_DSM_ENTITY;
#ifdef FEATURE_QXDM_DSM_TRACING
    fc_pool_id = FC_SMALL_POOL;
#endif
  }
  tdsrlc_post_fc_internal_cmd(entity,dsm_mem_level);

#ifdef FEATURE_QXDM_DSM_TRACING
  switch(dsm_mem_level)
  {
  case DSM_MEM_LEVEL_RLC_MANY:
    tdsrlc_fc_dsm_event_hits[fc_pool_id].many_event_cnt++;
    break;
  case DSM_MEM_LEVEL_RLC_FEW:
    tdsrlc_fc_dsm_event_hits[fc_pool_id].few_event_cnt++;
    break;
  case DSM_MEM_LEVEL_LINK_LAYER_DNE:
    tdsrlc_fc_dsm_event_hits[fc_pool_id].dne_event_cnt++;
    break;
  default:
    break;
  }

#endif
} 

#endif //CFCM_related_obsolete_1


/*===========================================================================

FUNCTION CRLC_UL_CIPHER_CONFIG_HANDLER

DESCRIPTION
  Configures ciphering on one or more Unacknowledged or Acknowledged 
  Mode Channels.  This function configures the ciphering parameters that are 
  recevied from RRC. The ciphering parameters are updated only if the the 
  channel is in TDSUL_DATA_TRANSFER_READY STATE.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_cipher_config_handler
( 
  tdsrlc_ul_cipher_config_type *cipher_ptr  /* Ptr to Uplink Cipher Config.   */
)
{
  tdsrlci_ul_ctrl_blk_type *ctl_blk = NULL;
  int i;
  
/* ToDo: Move logging part outside LOCK
*/
  /*Locking both EUL and R99 Mutex*/
  (void)TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);
  
  TDSRLC_MSG1(MSG_LEGACY_HIGH,"RLC_DBG: recevied UL cipher config for nchan %d ",cipher_ptr->nchan);

  /*------------------------------------------------------------------------- 
    For each channel indicated in the Ciphering Config Request 
  -------------------------------------------------------------------------*/
  for (i = 0;i < cipher_ptr->nchan; i++)
  {
    /*----------------------------------------------------------------------- 
      Get control block index from logic channel ID.
    -----------------------------------------------------------------------*/
    if (tdsul_ctl_blk_id_tbl[cipher_ptr->ciphering[i].lc_id] >= TDSUE_MAX_UL_RLC_ENTITY)
    {
      continue;
    }
    ctl_blk = &tdsuplink_table.ctrl_blk[tdsul_ctl_blk_id_tbl[cipher_ptr->ciphering[i].lc_id]];

    /*----------------------------------------------------------------------- 
      If RLC uplink is in suspended state, copy details to the uplink table
      entry.
    -----------------------------------------------------------------------*/     
    if (tdsul_ctl_blk_id_tbl[cipher_ptr->ciphering[i].lc_id] != TDSINIT_CTL_BLK_ID)
    {
      /*---------------------------------------------------------------------
        Copy the ciphering configuration details to the tdsuplink_table entry.
      ---------------------------------------------------------------------*/
      if (cipher_ptr->ciphering[i].cipher_mode)
      {
        TDSRLC_MSG3(MSG_LEGACY_HIGH,"UL cipher config, LC %d, HFN %x, act_sn %x", 
                 cipher_ptr->ciphering[i].lc_id,cipher_ptr->ciphering[i].hfn, 
                 cipher_ptr->ciphering[i].ciph_act_sn);

        /*-------------------------------------------------------------------
          Backup the old ciphering configuration as RRC might abort the
          new configuration if a cell update is triggered.
        -------------------------------------------------------------------*/
        ctl_blk->cipher_backup = ctl_blk->cipher;

        /*---------------------------------------------------------------------
          UNACKNOWLEDGED Mode cipher config.
        ---------------------------------------------------------------------*/
        if (ctl_blk->lc_mode == UE_MODE_UNACKNOWLEDGED)
        {
          switch (ctl_blk->cipher.cipher_mode)
          {
            /*--------------------------------------------------------------
              There is no PDU retx in UM. We don't need to remember the  
              current cipher_key and hfn when the cipher_mode moves from 
              TDSCIPHER_MOD to TDSCIPHER_ON.                                   
            ---------------------------------------------------------------*/
            case TDSNO_CIPHER:
            case TDSCIPHER_ON:
              /*-------------------------------------------------------------
                If the channel is directly config to TDSCIPHER_ON mode before, 
                but not sending any data yet, take the new cipher key id, 
                HFN and ciphering algorithm. The current key, HFN and 
                ciphering algorithm will not be remembered.
              -------------------------------------------------------------*/
              if (ctl_blk->cipher.new_cipher_on)
              {
                ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 7) | 
                                        ctl_blk->ctrl.um_ctrl.vt_us;
              }
              else
              {
                if (TDSRLCI_SEQ_GT(cipher_ptr->ciphering[i].ciph_act_sn, 
                                ctl_blk->ctrl.um_ctrl.vt_us, TDSRLC_UM_MODULUS))
                {
                  ctl_blk->cipher.cipher_mode = 
                    (ctl_blk->cipher.cipher_mode == TDSNO_CIPHER) ?
                    TDSCIPHER_WAIT : TDSCIPHER_MOD;

                  ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
                  ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
                  ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
                }
                else
                {
                  ctl_blk->cipher.cipher_mode = TDSCIPHER_ON;
                  ctl_blk->cipher.new_cipher_on = TRUE;
                  ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                  ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                  ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                  ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 7) | 
                                          ctl_blk->ctrl.um_ctrl.vt_us;
                  TDSRLC_MSG3(MSG_LEGACY_HIGH,"Cipher ON UL UM, LC %d, key_id %d, cnt %x", 
                            cipher_ptr->ciphering[i].lc_id, 
                            ctl_blk->cipher.cipher_key_id, ctl_blk->cipher.count);
                }
                ctl_blk->cipher.act_sn_old = ctl_blk->cipher.act_sn;
                ctl_blk->cipher.act_sn= cipher_ptr->ciphering[i].ciph_act_sn;
              }
              break;

            case TDSCIPHER_WAIT:
            case TDSCIPHER_MOD:
              ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
              ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
              ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
              break;

            default:
              TDSRLC_MSG0(MSG_LEGACY_HIGH,"Invalid ciphering state");
          }
        }
        /*---------------------------------------------------------------------
          ACKNOWLEDGED Mode cipher config.
        ---------------------------------------------------------------------*/
        else if (ctl_blk->lc_mode == UE_MODE_ACKNOWLEDGED)
        {
          switch (ctl_blk->cipher.cipher_mode)
          {
            case TDSNO_CIPHER:
            case TDSCIPHER_ON:
              /*-------------------------------------------------------------
                If the channel is directly config to TDSCIPHER_ON mode before, 
                but not sending any data yet, take the new cipher key id, 
                HFN and ciphering algorithm. The current ones will not be 
                remembered.
              -------------------------------------------------------------*/
              if (ctl_blk->cipher.new_cipher_on && ctl_blk->cipher.rb_id != 2)
              {
                ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 12) | 
                                        ctl_blk->ctrl.am_ctrl.ul_state.vt_s;
              }
              else
              {
                /*-----------------------------------------------------------
                  Not reaching the new activation time yet. 
                  Enter TDSCIPHER_WAIT/CIPHER_MOD for NO_CIPHER/CIPHER_ON.
                -------------------------------------------------------------*/
                if (TDSRLCI_SEQ_GT(cipher_ptr->ciphering[i].ciph_act_sn, 
                                ctl_blk->ctrl.am_ctrl.ul_state.vt_s, TDSRLC_AM_MODULUS))
                {
                  ctl_blk->cipher.cipher_mode = 
                    (ctl_blk->cipher.cipher_mode == TDSNO_CIPHER) ?
                    TDSCIPHER_WAIT : TDSCIPHER_MOD;

                  ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
                  ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
                  ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
                  ctl_blk->cipher.act_sn_new = cipher_ptr->ciphering[i].ciph_act_sn;
                }
                /*-------------------------------------------------------------
                  Reached the new activation time.
                -------------------------------------------------------------*/
                else
                {
                  /*-------------------------------------------------------------
                    Cipher is on when received this new cipher config.
                  -------------------------------------------------------------*/
                  if (ctl_blk->cipher.cipher_mode == TDSCIPHER_ON)
                  {
                    /*-------------------------------------------------------------
                      Old config is active.
                    -------------------------------------------------------------*/
                    if (ctl_blk->cipher.act_old_cfg)
                    {
                      ctl_blk->cipher.act_old_cfg1 = TRUE;
                      ctl_blk->cipher.cipher_key_id_old1 = ctl_blk->cipher.cipher_key_id_old;
                      ctl_blk->cipher.cipher_algo_old1 = ctl_blk->cipher.cipher_algo_old;
                      ctl_blk->cipher.act_sn_old1 = ctl_blk->cipher.act_sn_old;
                      ctl_blk->cipher.count_old1 = ctl_blk->cipher.count_old;
                    }
                    /*-------------------------------------------------------------
                      No old config is active.
                    -------------------------------------------------------------*/
                    else
                    {
                      ctl_blk->cipher.act_old_cfg = TRUE;
                    }
                    /*-------------------------------------------------------------
                      Back up the current cipher config into old cipher config
                      for retx using.
                    -------------------------------------------------------------*/
                    ctl_blk->cipher.cipher_key_id_old = ctl_blk->cipher.cipher_key_id;
                    ctl_blk->cipher.cipher_algo_old = ctl_blk->cipher.cipher_algo;
                    ctl_blk->cipher.act_sn_old = ctl_blk->cipher.act_sn;
                    /*-----------------------------------------------------------------
                      Since the count_c always move 1 ahead for the next outgoing PDU,
                      We substract the current count_c by 1 to reflect the previous   
                      outgoing PDU.                                                   
                    -----------------------------------------------------------------*/
                    ctl_blk->cipher.count_old = ctl_blk->cipher.count - 1;

                    ctl_blk->cipher.new_cipher_on = TRUE;
                    /*-------------------------------------------------------------
                      Update current active cipher config.
                    -------------------------------------------------------------*/
                    ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                    ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                    ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                    ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 12) | 
                                            ctl_blk->ctrl.am_ctrl.ul_state.vt_s;
                    ctl_blk->cipher.act_sn = cipher_ptr->ciphering[i].ciph_act_sn;
                    ctl_blk->cipher.rb_id = cipher_ptr->ciphering[i].rb_id;
                  }
                  /*-------------------------------------------------------------
                    No Cipher when received this new cipher config.
                  -------------------------------------------------------------*/
                  else
                  {
                    ctl_blk->cipher.new_cipher_on = TRUE;
                    /*-------------------------------------------------------------
                      Update current active cipher config.
                    -------------------------------------------------------------*/
                    ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                    ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                    ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                    ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 12) | 
                                          ctl_blk->ctrl.am_ctrl.ul_state.vt_s;
                    ctl_blk->cipher.act_sn = cipher_ptr->ciphering[i].ciph_act_sn;
                    ctl_blk->cipher.rb_id = cipher_ptr->ciphering[i].rb_id;
                    ctl_blk->cipher.cipher_mode = TDSCIPHER_ON;
                  }
                  TDSRLC_MSG3(MSG_LEGACY_HIGH,"Cipher ON UL AM, LC %d, key_id %d, cnt %x",
                            cipher_ptr->ciphering[i].lc_id, 
                            ctl_blk->cipher.cipher_key_id, ctl_blk->cipher.count);
                }
              }
              break;

            case TDSCIPHER_WAIT:
            case TDSCIPHER_MOD:
              if ((ctl_blk->ctrl.am_ctrl.am_common->rb_id == 2) &&
                  (ctl_blk->cipher.act_sn_new != cipher_ptr->ciphering[i].ciph_act_sn))
              {
                /*-------------------------------------------------------------
                  Store cipher parameters for 2nd pending ciphering session
                  for SRB 2.
                -------------------------------------------------------------*/
                ctl_blk->cipher.act_sn_new1 = cipher_ptr->ciphering[i].ciph_act_sn;
                ctl_blk->cipher.cipher_key_id_new1 = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo_new1 = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn_new1 = cipher_ptr->ciphering[i].hfn;
                ctl_blk->cipher.two_pending = TRUE;
              }
              else
              {
                /*-------------------------------------------------------------
                  For non-SRB2, there is no 2nd pending session.
                  Update the key id and hfn but use the existing activation 
                  time.
                -------------------------------------------------------------*/
                ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
              }
              break;

            default:
              TDSRLC_MSG0(MSG_LEGACY_HIGH,"Invalid Ciphering state");
          }
         
        }
        ctl_blk->cipher.rb_id = cipher_ptr->ciphering[i].rb_id;
      }
      else
      {
        ctl_blk->cipher.cipher_mode = TDSNO_CIPHER;
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL Cipher config,LC %d cipher OFF",cipher_ptr->ciphering[i].lc_id);
      }
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"UL Cipher config,LC %d not up",cipher_ptr->ciphering[i].lc_id);
    }
  }
  TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (log_status(TDSCDMA_UL_RLC_CIPH_CFG_LOG_PACKET))
  {
    TDSRLC_MSG0(MSG_LEGACY_LOW,"LOG UL Cipher config");
    tdsrlc_ul_tx_cipher_log_packet(cipher_ptr);
  }
  else
  {
    TDSRLC_MSG0(MSG_LEGACY_LOW,"NO LOG UL Cipher config");
  }
#endif

} /* tdscrlc_ul_cipher_config_handler() */          

/*===========================================================================

FUNCTION CRLC_UL_PERFORM_SUSPEND_ACTIVITY

DESCRIPTION
  Suspends or stop one or more uplink unacknowledged or acknowledged mode channels.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_perform_suspend_or_stop_functionality
( 
  tdsrlc_ul_stop_suspend_request_type *suspend_stop_ptr,
  tdsrlc_ul_stop_suspend_cnf_type *crlc_suspend_stop_cnf
)
{
  tdsrlci_ul_ctrl_blk_type *ul_ctrl_blk = NULL;
  uint8 index;                        /* Index into Uplink Table          */
  uint8 lc_id;                        /* RLC logical Channel ID           */
  uint32 num_bytes = 0;
  int i;
  rex_crit_sect_type *datapath_mutex = NULL;
  tdsrlci_ul_state_e_type local_state = TDSUL_NULL_STATE;
  
  crlc_suspend_stop_cnf->nchan = 0;

  /*------------------------------------------------------------------------- 
    For each channel indicated in the SUSPEND Request
  -------------------------------------------------------------------------*/
  for (i = 0; i < suspend_stop_ptr->nchan; i++)
  {
    /*----------------------------------------------------------------------- 
      Get the index of the uplink table.
    -----------------------------------------------------------------------*/
    lc_id = suspend_stop_ptr->suspend_stop[i].lc_id;
    index = tdsul_ctl_blk_id_tbl[lc_id];

    /*----------------------------------------------------------------------- 
      Make sure the channel is in the right state and mode.
    -----------------------------------------------------------------------*/
    /*lint -save -e655 */
    if ((index != TDSINIT_CTL_BLK_ID) && 
        (index < TDSUE_MAX_UL_RLC_ENTITY) &&
        (tdsuplink_table.ctrl_blk[index].lc_state & TDSUL_DATA_TRANSFER_READY) && 
        (tdsuplink_table.ctrl_blk[index].lc_mode != UE_MODE_TRANSPARENT))
    /*lint -restore */     
    {
      ul_ctrl_blk = &tdsuplink_table.ctrl_blk[index];
      /* Turn off TDSUL_DATA_TRANSFER_READY state  */
      /*lint -save -e64 -e641 */
      local_state = ul_ctrl_blk->lc_state;
      local_state &= (~TDSUL_DATA_TRANSFER_READY);
      /*lint -restore */
      /* RLC will STOP all rbs on reciving TDSRLC_STOP cmd */
      if(suspend_stop_ptr->ul_cmd_type == TDSRLC_STOP_CMD)
      {
        /*lint -save -e655 */
        local_state |= TDSUL_STOP;
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"Stop RLC LC %d",suspend_stop_ptr->suspend_stop[i].lc_id);
        datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(lc_id);
        ul_ctrl_blk->lc_state = local_state; 
        /*lint -restore */
        if (ul_ctrl_blk->lc_mode == UE_MODE_UNACKNOWLEDGED) 
        {
          crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
            ul_ctrl_blk->ctrl.um_ctrl.vt_us;
        }
        else /* it is UE_MODE_ACKNOWLEDGED */
        {
          crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
            ul_ctrl_blk->ctrl.am_ctrl.ul_state.vt_s;
        }
        TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);	  

      }
      else /* incase of RLC SUSPEND */
      {

      /*--------------------------------------------------------------------- 
        Check if N == 0. 
      ---------------------------------------------------------------------*/
        if (suspend_stop_ptr->suspend_stop[i].n == 0)
        {          
          /*------------------------------------------------------------------- 
            Suspend the channel immediately 
          -------------------------------------------------------------------*/
          /* Turn on TDSUL_SUSPENDED */
          /*lint -save -e655 */
          local_state |= TDSUL_SUSPENDED;
          TDSRLC_MSG1(MSG_LEGACY_HIGH,"Suspend RLC LC %d",suspend_stop_ptr->suspend_stop[i].lc_id);
        datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(lc_id);
          ul_ctrl_blk->lc_state = local_state;

          /*lint -restore */         
          if (ul_ctrl_blk->lc_mode == UE_MODE_UNACKNOWLEDGED) 
          {
            crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
              ul_ctrl_blk->ctrl.um_ctrl.vt_us;
          }
          else /* it is UE_MODE_ACKNOWLEDGED */
          {
            crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
              ul_ctrl_blk->ctrl.am_ctrl.ul_state.vt_s;
          }
          TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);		

        }
        else
        {
          /*------------------------------------------------------------------- 
            Suspend after sending the N PDUS, so set state to 
            WAITING_TO_SUSPEND. 
          -------------------------------------------------------------------*/
          /* Turn on TDSUL_WAITING_TO_SUSPEND bit */
          /*lint -save -e655 */
          local_state |= TDSUL_WAITING_TO_SUSPEND;
          datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(lc_id);
          ul_ctrl_blk->lc_state = local_state;

		  /*lint -restore */
  
          if (ul_ctrl_blk->lc_mode == UE_MODE_UNACKNOWLEDGED) 
          {
            /* Save the number of PDUs can be sent before go suspending */
            ul_ctrl_blk->ctrl.um_ctrl.n_to_suspend = suspend_stop_ptr->suspend_stop[i].n;
          }
          else /* it is UE_MODE_ACKNOWLEDGED */ 
          {
            /* Save the number of PDUs can be sent before go suspending */
            ul_ctrl_blk->ctrl.am_ctrl.ul_state.n_to_suspend = 
                                                      suspend_stop_ptr->suspend_stop[i].n;
          }
          if (ul_ctrl_blk->ctrl.am_ctrl.am_common->rb_id == 2)
          {
            /* Put RLC under config pending state. This prevent PDU building 
               interrupt to change the WM state when RLC calculates suspend 
               offset */
            /*lint -save -e655 */
            ul_ctrl_blk->lc_state |= TDSUL_CONFIG_PENDING;
            /*lint -restore */
            if(ul_ctrl_blk->ul_wm_ptr != NULL)
            {
              num_bytes = ul_ctrl_blk->ul_wm_ptr->current_cnt +
                          ((uint32)ul_ctrl_blk->ul_wm_ptr->q_ptr->cnt *
                           ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length) +
                          (ul_ctrl_blk->special_ind ?
                           ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0) +
                          (ul_ctrl_blk->octet_less ?
                           ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0);

              if (NULL != ul_ctrl_blk->cur_sdu_ptr)
              {
                num_bytes += 
                 ((ul_ctrl_blk->cur_sdu_len - ul_ctrl_blk->cur_sdu_tx_len) + 
                   ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length);
              }
            }
            else
            {
              TDSRLC_MSG3(MSG_LEGACY_HIGH,"UL WM Unregistered on LC %d LC state %d LC Mode %d ", 
                        lc_id, ul_ctrl_blk->lc_state , ul_ctrl_blk->lc_mode);
              num_bytes = (ul_ctrl_blk->special_ind ?
                           ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0) +
                          (ul_ctrl_blk->octet_less ?
                           ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0);
              if (NULL != ul_ctrl_blk->cur_sdu_ptr)
              {
                num_bytes += 
                 ((ul_ctrl_blk->cur_sdu_len - ul_ctrl_blk->cur_sdu_tx_len) + 
                   ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length);
              }
            }
          
            ul_ctrl_blk->ctrl.am_ctrl.ul_state.n_to_suspend += (uint8)(num_bytes / 
              ((ul_ctrl_blk->ctrl.am_ctrl.ul_cnfg_var->ul_rlc_size - 16) >> 3));
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
            if (tdsrlc_debug_var & TDSRLC_DBG_CORRUPTION)
            {            
              TDSRLC_MSG2(MSG_LEGACY_HIGH,"Padding_DBG: LC %d UL RLC size %d",suspend_stop_ptr->suspend_stop[i].lc_id,ul_ctrl_blk->ctrl.am_ctrl.ul_cnfg_var->ul_rlc_size);
            }
#endif
            crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
                (ul_ctrl_blk->ctrl.am_ctrl.ul_state.vt_s +
                 ul_ctrl_blk->ctrl.am_ctrl.ul_state.n_to_suspend
                ) & TDSRLC_AM_MOD_MASK;
  
            /* Has done the suspend offset calculating. Lift the bar. */
            /*lint -e64 -e641 */
            ul_ctrl_blk->lc_state &= (~TDSUL_CONFIG_PENDING);
            /*lint +e64 +e641 */
          }
          else
          {
            crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
                    (ul_ctrl_blk->ctrl.am_ctrl.ul_state.vt_s +
                    suspend_stop_ptr->suspend_stop[i].n) & TDSRLC_AM_MOD_MASK;
          }
          TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);		
        }
      }
      
      crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].lc_id = lc_id;
      crlc_suspend_stop_cnf->nchan++;
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"LC %d not in right state or mode",suspend_stop_ptr->suspend_stop[i].lc_id);
    }
  }          
}/* tdscrlc_ul_perform_suspend_or_stop_functionality() */



/*===========================================================================

FUNCTION CRLC_UL_RESUME_HANDLER

DESCRIPTION
  Resume transmission on one or more uplink unacknowledged or acknowledged 
  mode channels.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_resume_handler
( 
  tdsrlc_ul_resume_type *resume_ptr      /* Ptr to Resume Info                */
)
{
  uint8 index;                        /* Index into Downlink Table         */
  int   i;
  /*Locking both EUL and R99 Mutex*/
  (void)TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);

  for (i = 0; i < resume_ptr->nchan; i++)
  {
    index = tdsul_ctl_blk_id_tbl[resume_ptr->channel[i]];
    /*lint -save -e64 -e641 -e655 */
    if (index != TDSINIT_CTL_BLK_ID && 
        (index < TDSUE_MAX_UL_RLC_ENTITY) &&
        (tdsuplink_table.ctrl_blk[index].lc_state & TDSUL_SUSPENDED ||
         tdsuplink_table.ctrl_blk[index].lc_state & TDSUL_WAITING_TO_SUSPEND ))
    {
      //TDSL2_ACQ_UL_LW_LOCK();    
      tdsuplink_table.ctrl_blk[index].lc_state &= (~(TDSUL_SUSPENDED | TDSUL_WAITING_TO_SUSPEND));
      tdsuplink_table.ctrl_blk[index].lc_state |= TDSUL_DATA_TRANSFER_READY;
      //TDSL2_ACQ_UL_LW_UNLOCK();
      
    }
    /*lint -restore */
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"LC %d not in Suspended State",resume_ptr->channel[i]);
    }
  }

  TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);

  /* not use TDSRRC_CRLC_RESUME_CNF any more */

}/* tdscrlc_ul_resume_handler() */




/*===========================================================================

FUNCTION tdsrlci_get_config_action

DESCRIPTION
  this function checks all the bits of the bitmask and return the
  corresponding action set by RRC to RLC at the tme of channel config req
    
DEPENDENCIES
  None.
  
RETURN VALUE
  action type
  
SIDE EFFECTS
  None.

===========================================================================*/

tdsrlc_channel_action_cnf_e_type tdsrlci_get_config_action(uint16 *cfg_mask)
{
  TDSRLC_MSG1(MSG_LEGACY_MED,"The Config Mask is %x",*cfg_mask);
  if ((*cfg_mask & TDSRLC_MODIFY_CFG) == TDSRLC_MODIFY_CFG)
  {
    (*cfg_mask) &=~TDSRLC_MODIFY_CFG; 
    return TDSRLCI_MODIFY;
  }
  else if((*cfg_mask & TDSRLC_STOP_CFG) == TDSRLC_STOP_CFG)
  {
    (*cfg_mask) &=~TDSRLC_STOP_CFG; 
    return TDSRLCI_STOP;
  }
  else if((*cfg_mask & TDSRLC_CONTINUE_CFG) == TDSRLC_CONTINUE_CFG)
  {
    (*cfg_mask) &=~TDSRLC_CONTINUE_CFG; 
    return TDSRLCI_CONTINUE;
  }
  else if((*cfg_mask & TDSRLC_RE_ESTABLISH_CFG) == TDSRLC_RE_ESTABLISH_CFG)
  {
    (*cfg_mask) &=~TDSRLC_RE_ESTABLISH_CFG; 
    return TDSRLCI_RE_ESTABLISH;
  }
  else
  {
    *cfg_mask = 0;
    return TDSRLCI_NO_ACTION;
  }

}


/*===========================================================================

FUNCTION CRLC_UL_TM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of Transparent Mode Logical 
  channels. This function is called when RRC sends a CRLC_UL_TM_CONFIG_REQ
  to RLC. If a new channel is to be established, the function checks if
  the channels exists. If it already exists, and ERR message is dispalyed.
  If the channel does not exist, the downlink table entry for that channel
  is updated and the state is set to TDSUL_DATA_TRANSFER_READY. If a channel
  is to be released,the function checks if the channel is in the 
  TDSUL_DATA_TRANSFER_READY state. If it is, the channel is set to the NULL
  state and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the 
  TDSUL_DATA_TRANSFER_READY state. If it is, the downlink table entry for the
  channel is updated with the configuration received from RRC.

    
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdscrlc_ul_tm_config_handler 
(
  tdsrlc_ul_tm_config_type *tm_ptr  /* Ptr to Transparent Mode Config Info    */
)
{
  tdsrrc_rlc_cmd_type *rrc_rlc_cmd_ptr; /* Command buffer to store confirm        */ 
  uint8 index;                   /* Index into Downlink Table              */
  int i;
  tdsrlc_channel_action_cnf_e_type action;
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  uint8 j = 0;
  uint8 k = 0;
#endif  

  /*-------------------------------------------------------------------------
    Get a command buffer to store the confirmation information to send to 
    RRC.
  -------------------------------------------------------------------------*/
  rrc_rlc_cmd_ptr = tdsrrc_get_rlc_cmd_buf();
  if (rrc_rlc_cmd_ptr == NULL)
  {
    TDSRLC_MSG0(MSG_LEGACY_HIGH,"UL TM:Could not get cmd buffer");
    return;
  }
  rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CRLC_UL_TM_CONFIG_CNF;
  rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.nchan = tm_ptr->nchan;

  TDSL2_ACQ_DL_LOCK();
  /*Locking both EUL and R99 Mutex*/
  (void)TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);

#ifdef FEATURE_TM_LB
  if((tm_ptr->nchan == 0) && (tm_ptr->loop_back_mode == TDSRLC_LOOPBACK_MODE_2))
  {
    /*-----------------------------------------------------------------------
      All DTCH TM channels will now operate in loop back mode2.
    -----------------------------------------------------------------------*/
    for(i = 0; i < UE_MAX_UL_TM_CHANNEL; ++i)
    {
      if(((index = tdsul_ctl_blk_id_tbl[i]) != TDSINIT_CTL_BLK_ID) &&
        (index < TDSUE_MAX_UL_RLC_ENTITY) &&
        (tdsuplink_table.ctrl_blk[index].lc_type == UE_LOGCHAN_DTCH))
      {
        tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.lpm = TDSRLC_LOOPBACK_MODE_2;
      }
    }
  }
#endif /* FEATURE_TM_LB */

  for (i = 0; i < tm_ptr->nchan; i++)
  {
    rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.tm_config_result[i].lc_id = 
                                                  tm_ptr->chan_info[i].lc_id;
    if (tm_ptr->chan_info[i].lc_id >= TDSUE_MAX_UL_LOGICAL_CHANNEL) 
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"Invalid UL TM LC Id %d",tm_ptr->chan_info[i].lc_id);
      rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                    tm_config_result[i].result = FAILURE;
    }
    else
    {
      switch(tm_ptr->ul_tm_act[i].action)
      {
        case TDSRLC_ESTABLISH:
        /*-------------------------------------------------------------------
          Check if the channel exists. If it does not, get an index 
          into the downlink table and store all the information for the 
          channel. Send a SUCCESS indication in the confirm message to RRC 
        -------------------------------------------------------------------*/
        if ((tdsul_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id] == 
             TDSINIT_CTL_BLK_ID) &&
            ((index = tdsul_assign_ctl_blk()) != TDSINIT_CTL_BLK_ID) &&
            (index < TDSUE_MAX_UL_RLC_ENTITY))
        {
          TDSRLC_MSG2(MSG_LEGACY_HIGH,"UL TM Establish LC %d,Index %d",tm_ptr->chan_info[i].lc_id,index);

          if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
          {
            tdsrlc_dump_ul_ctl_blk_id_tbl();
          }
		  
          tdsul_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id] = index;

          if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
          {
            tdsrlc_sanitize_ul_ctl_blk_id_tbl();
          }
		  
          tdsuplink_table.ctrl_blk[index].lc_mode = UE_MODE_TRANSPARENT;
          tdsuplink_table.ctrl_blk[index].lc_type = tm_ptr->chan_info[i].lc_type;
          tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.rlc_id = tm_ptr->chan_info[i].lc_id;
          tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.seg_ind = 
                                           tm_ptr->chan_info[i].seg_ind;

          tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.act_incl = 
                                          tm_ptr->ul_tm_act[i].act_incl;

          tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.act_time = 
                                          tm_ptr->ul_tm_act[i].act_time;

          tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.timer_discard = 
                                          tm_ptr->chan_info[i].timer_discard;
#ifdef FEATURE_TM_LB
          tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.lpm = TDSRLC_LOOPBACK_NA;
#endif /* FEATURE_TM_LB */
          /*
             Initially LC will be in NULL state, so wait till ISR finishes
          */
          //TDSL2_ACQ_UL_LW_LOCK();
          tdsuplink_table.ctrl_blk[index].lc_state = TDSUL_DATA_TRANSFER_READY;
         // TDSL2_ACQ_UL_LW_UNLOCK();
          
          tdsuplink_table.ctrl_blk[index].ul_wm_ptr = NULL;
          tdsuplink_table.ctrl_blk[index].wm_in_rach = FALSE;
          tdsuplink_table.active_ch++;
          rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                          tm_config_result[i].result = SUCCESS;
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
          tdsul_tm_log_ptr.chan_info[j] = tm_ptr->chan_info[i];
          j++;
          tdsul_log_state.ul_state[k].rlc_id = tm_ptr->chan_info[i].lc_id;
          tdsul_log_state.ul_state[k].lc_state = TDSTM_UL_DATA_TRANSFER_READY;
          k++;
#endif
        }
        else
        {
          /*-----------------------------------------------------------------
            If the channel exists, send a FAILURE indication in the confirm 
            message to RRC.
          -----------------------------------------------------------------*/
          rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                               tm_config_result[i].result = FAILURE;
          TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d exists - Cannot Establish",tm_ptr->chan_info[i].lc_id);
        }
        break;
        case TDSRLC_RELEASE:
        /*-------------------------------------------------------------------
          Check if the channel is in NULL state. If it is not, release 
          the channel and send a SUCCESS indication in the confirm message 
          to RRC.  
        -------------------------------------------------------------------*/
        if ((index = tdsul_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id]) 
                                                    != TDSINIT_CTL_BLK_ID &&
            (index < TDSUE_MAX_UL_RLC_ENTITY))
        {
          TDSRLC_MSG2(MSG_LEGACY_HIGH,"UL TM Release LC %d,Index %d",tm_ptr->chan_info[i].lc_id,index);
          /*
            wait till UL ISR finishes
          */
          //TDSL2_ACQ_UL_LW_LOCK();
          tdsuplink_table.ctrl_blk[index].lc_state = TDSUL_NULL_STATE;
          //TDSL2_ACQ_UL_LW_UNLOCK();
          /* Release discard timer  */
          if(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr != NULL)
          {
            (void) rex_clr_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr);
            /* release timer block */
            rex_delete_timer_ex(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr);
            /* Reset timer sig */
            tdsuplink_table.ctrl_blk[index].tmr_discard_ptr = NULL;
            TDSRLC_MSG1(MSG_LEGACY_HIGH,"Release SDU discard timer,LC %d",tm_ptr->chan_info[i].lc_id);
          }

          tdsuplink_table.active_ch--;
		  
		  
          if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
          {
            tdsrlc_dump_ul_ctl_blk_id_tbl();
          }

          tdsul_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id] = TDSINIT_CTL_BLK_ID;

          if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
          {
            tdsrlc_sanitize_ul_ctl_blk_id_tbl();
          }        

          rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                               tm_config_result[i].result = SUCCESS;
          if(tdsuplink_table.ctrl_blk[index].lc_type ==UE_LOGCHAN_DTCH)
          {
            tdsl2_ivoice.enable_flag = FALSE;
			      TDSRLC_MSG0(MSG_LEGACY_HIGH,"De-registering the call back as all active uplink channels are released ");		
          }			  
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
          tdsul_log_state.ul_state[k].rlc_id = tm_ptr->chan_info[i].lc_id;
          tdsul_log_state.ul_state[k].lc_state = TDSRLC_UL_NULL_STATE;
          k++;
#endif
        }
        else
        {
          /*-----------------------------------------------------------------
            If the channel does not exist, send a FAILURE indication in the 
           confirm message to RRC.
          -----------------------------------------------------------------*/
          rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                               tm_config_result[i].result = FAILURE;
          TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d does'nt exist - Cannot Release",tm_ptr->chan_info[i].lc_id);
        }
        break;
      case TDSRLC_MODIFY:

        rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                               tm_config_result[i].result = SUCCESS;

        if(tm_ptr->ul_tm_act[i].action == TDSRLC_MODIFY)
        {
          while(tm_ptr->ul_tm_act[i].ul_cfg_mask != 0)
          {
            action = tdsrlci_get_config_action(&(tm_ptr->ul_tm_act[i].ul_cfg_mask));
            switch(action)
            {
              case TDSRLCI_MODIFY:
              /*-------------------------------------------------------------------
                Check if Channel is in NULL state. If it is not, modify
                the channel parameters and send a SUCCESS indication in the
                confirm message to RRC.
              -------------------------------------------------------------------*/
              if ((index = tdsul_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id])
                                                          != TDSINIT_CTL_BLK_ID &&
                   (index < TDSUE_MAX_UL_RLC_ENTITY))
              {
                TDSRLC_MSG2(MSG_LEGACY_HIGH,"UL TM Modify LC: %d,Index: %d",i,index);
                /*
                  Entire TM entity is modified here, so protection is verym uch needed
                */
                //TDSL2_ACQ_UL_LW_LOCK();
                tdsuplink_table.ctrl_blk[index].lc_type =
                                            tm_ptr->chan_info[i].lc_type;
                tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.seg_ind =
                                            tm_ptr->chan_info[i].seg_ind;
                tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.act_incl =
                                            tm_ptr->ul_tm_act[i].act_incl;
                tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.act_time =
                                            tm_ptr->ul_tm_act[i].act_time;
                tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.timer_discard =
                                            tm_ptr->chan_info[i].timer_discard;
#ifdef FEATURE_TM_LB
                tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.lpm = TDSRLC_LOOPBACK_NA;
#endif /* FEATURE_TM_LB */
                rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                            tm_config_result[i].result = SUCCESS;
      #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                if (j < UE_MAX_UL_TM_CHANNEL)
                {
                  tdsul_tm_log_ptr.chan_info[j] = tm_ptr->chan_info[i];
                  j++;
                }
      #endif
               //TDSL2_ACQ_UL_LW_UNLOCK();
              }
              else
              {
                /*-----------------------------------------------------------------
                  If the channel does not exist, send a FAILURE indication in the
                  confirm message to RRC.
                -----------------------------------------------------------------*/
                TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d not in Right State - Cannot Mod",tm_ptr->chan_info[i].lc_id);
                rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                     tm_config_result[i].result = FAILURE;
              }
              break;
         
              default:
                rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                   tm_config_result[i].result = FAILURE;
                TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL TM:Invalid Action %d",tm_ptr->ul_tm_act[i].action);
            }
          }
        }
        break;

      default:
        rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                             tm_config_result[i].result = FAILURE;
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL TM:Invalid Action %d",tm_ptr->ul_tm_act[i].action);
      }
    }
  }

  TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_DL_UNLOCK();
  
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (j > 0) 
  {
    tdsul_tm_log_ptr.nchan = j;
    tdsrlc_ul_send_tm_log_packet(&tdsul_tm_log_ptr);
  }
  if (k > 0) 
  {
    tdsul_log_state.nchan = k;
    tdsrlc_ul_send_state_log_packet(&tdsul_log_state);
  }
#endif
  tdsrrc_put_rlc_cmd(rrc_rlc_cmd_ptr);

}/* tdscrlc_ul_tm_config_handler() */


/*===========================================================================

FUNCTION CRLC_UL_UM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of Unacknowledged Mode Logical 
  channels. This function is called when RRC sends a TDSCRLC_UL_UM_CONFIG_REQ
  to RLC. If a new channel is to be established, the function checks if
  the channels exists. If it already exists, and ERR message is dispalyed.
  If the channel does not exist, the downlink table entry for that channel
  is updated and the state is set to TDSUL_DATA_TRANSFER_READY. If a channel
  is to be released,the function checks if the channel is in the 
  TDSUL_DATA_TRANSFER_READY state. If it is, the channel is set to the NULL 
  state and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the 
  TDSUL_DATA_TRANSFER_READY state. If it is, the downlink table entry for the
  channel is updated with the configuration received from RRC.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_um_config_handler 
(
  tdsrlc_ul_um_config_type *um_ptr  /* Ptr to Unacknowledged Mode Config Info */
)
{
  tdsrrc_rlc_cmd_type *rrc_rlc_cmd_ptr; /* Command Buf for storing Confirm        */
  uint8       index;             /* Index into Uplink Table                */      
  int          i;
  tdsrlc_channel_action_cnf_e_type action;
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  uint8 j = 0;
  uint8 k = 0;
#endif  
                                                        
  /*-------------------------------------------------------------------------
    Get a command buffer to store the confirmation information to send to 
    RRC.
  -------------------------------------------------------------------------*/
  rrc_rlc_cmd_ptr = tdsrrc_get_rlc_cmd_buf();
  if (rrc_rlc_cmd_ptr == NULL)
  {
    TDSRLC_MSG0(MSG_LEGACY_HIGH,"Could not get cmd buffer");
    return;
  }
  rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CRLC_UL_UM_CONFIG_CNF;
  rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.nchan = um_ptr->nchan;

  TDSL2_ACQ_DL_LOCK();
  /*Locking both EUL and R99 Mutex*/
  (void)TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);
  
  for (i = 0; i < um_ptr->nchan; i++)
  {
    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].lc_id = 
                                                  um_ptr->chan_info[i].lc_id;
    if (um_ptr->chan_info[i].lc_id >= TDSUE_MAX_UL_LOGICAL_CHANNEL) 
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"Invalid UL UM LC Id %d",um_ptr->chan_info[i].lc_id);
      rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                    um_config_result[i].result = FAILURE;
    }
    else
    {
      switch(um_ptr->ul_um_act[i].action)
      {
        case TDSRLC_ESTABLISH:
          /*-------------------------------------------------------------------
            Check if the channel exists. If it does not, get an index 
            into the downlink table and store all the information for the 
            channel. Send a SUCCESS indication in the confirm message to RRC 
          -------------------------------------------------------------------*/
          if ((tdsul_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id] == TDSINIT_CTL_BLK_ID) &&
             ((index = tdsul_assign_ctl_blk()) != TDSINIT_CTL_BLK_ID) &&
             (index < TDSUE_MAX_UL_RLC_ENTITY))
          {
            TDSRLC_MSG3(MSG_LEGACY_HIGH,"UL UM Establish, LC %d Index %d, Alt E-bit = %d",
                     um_ptr->chan_info[i].lc_id, index,
                     um_ptr->chan_info[i].alternate_e_bit);
            if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
            {
              tdsrlc_dump_ul_ctl_blk_id_tbl();
            }

            tdsul_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id] = index;

            if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
            {
              tdsrlc_sanitize_ul_ctl_blk_id_tbl();
            }          
            
            tdsuplink_table.ctrl_blk[index].lc_mode = UE_MODE_UNACKNOWLEDGED;
            tdsuplink_table.ctrl_blk[index].lc_type = um_ptr->chan_info[i].lc_type;
            tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.rlc_id = 
                                                  um_ptr->chan_info[i].lc_id;
            tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.act_incl = 
                                                um_ptr->ul_um_act[i].act_incl;
            tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.act_time = 
                                                um_ptr->ul_um_act[i].act_time;

            tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard = 
                                            um_ptr->chan_info[i].timer_discard;

            tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.alternate_e_bit = 
                                        um_ptr->chan_info[i].alternate_e_bit;


            tdsuplink_table.ctrl_blk[index].cipher.cipher_mode = TDSNO_CIPHER;
            tdsuplink_table.ctrl_blk[index].cipher.cipher_all = FALSE;
            tdsuplink_table.ctrl_blk[index].cipher.hfn = 0;
            tdsuplink_table.ctrl_blk[index].cipher.new_cipher_on = FALSE;
            tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.li_length =
                    ((um_ptr->chan_info[i].rlc_size >> 3) <= 125) ? 1 : 2;
            tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.sn_wrap = FALSE;
            //tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.rlc_sn = 0;
            tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.vt_us = 0;
            tdsuplink_table.ctrl_blk[index].special_ind = FALSE;
            tdsuplink_table.ctrl_blk[index].octet_less = FALSE;
            tdsuplink_table.ctrl_blk[index].padding = FALSE;

            //TDSL2_ACQ_UL_LW_LOCK();
            tdsuplink_table.ctrl_blk[index].lc_state = TDSUL_DATA_TRANSFER_READY;
            //TDSL2_ACQ_UL_LW_UNLOCK();
            
            tdsuplink_table.ctrl_blk[index].ul_wm_ptr = NULL;
            tdsuplink_table.ctrl_blk[index].cur_sdu_ptr = NULL;
            tdsuplink_table.ctrl_blk[index].cur_sdu_len = 0;
            tdsuplink_table.ctrl_blk[index].cur_sdu_tx_len = 0;
            tdsuplink_table.ctrl_blk[index].wm_in_rach = FALSE;
            tdsuplink_table.active_ch++;
            rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                 um_config_result[i].result = SUCCESS;
#ifdef FEATURE_QXDM_DSM_TRACING
            if (tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.rlc_id >= TDSRLC_UL_AM_USR_PLANE_START_LC_ID)
            {
              if (tdsnum_rlc_non_srb_entities_up++ == 0)
              {
                fc_post_reg_cmd_dsm_trace
                  (DSM_UMTS_TECH,
                   UMTS_RLC_CLIENT,
                   &tdsrlc_fc_get_dsm_trace_cb,
                   0xDEADBEEF);
                TDSRLC_MSG0(MSG_LEGACY_HIGH,"Registered Qxdm DSM tracing");
              }
            }
#endif
  #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
            tdsul_um_log_ptr.chan_info[j] = um_ptr->chan_info[i];
            j++;
            tdsul_log_state.ul_state[k].rlc_id = um_ptr->chan_info[i].lc_id;
            tdsul_log_state.ul_state[k].lc_state = TDSUM_UL_DATA_TRANSFER_READY;
            k++;
  #endif
          }
          else
          {
            /*-----------------------------------------------------------------
              If the channel exists, send a FAILURE indication in the confirm 
              message to RRC.
            -----------------------------------------------------------------*/
            TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d exist or No ctl blk",um_ptr->chan_info[i].lc_id);
            rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                 um_config_result[i].result = FAILURE;
          }
          break;
        case TDSRLC_RELEASE:
          /*-------------------------------------------------------------------
            Check if the channel is in NULL state. If it is not, release 
            the channel and send a SUCCESS indication in the confirm message 
            to RRC.  
          -------------------------------------------------------------------*/
          if ((index = tdsul_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id]) != TDSINIT_CTL_BLK_ID &&
              (index < TDSUE_MAX_UL_RLC_ENTITY))
          {
            //TDSL2_ACQ_UL_LW_LOCK();
            tdsuplink_table.ctrl_blk[index].lc_state = TDSUL_NULL_STATE;
            //TDSL2_ACQ_UL_LW_UNLOCK();
            
            TDSRLC_MSG2(MSG_LEGACY_HIGH,"UL UM Release,LC %d Index %d",um_ptr->chan_info[i].lc_id,index);            
            /* Release discard timer  */
            if(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr != NULL)
            {
              (void) rex_clr_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr);
              /* release timer block */
              rex_delete_timer_ex(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr);
              /* Reset timer sig */
              tdsuplink_table.ctrl_blk[index].tmr_discard_ptr = NULL;
              TDSRLC_MSG1(MSG_LEGACY_HIGH,"Release SDU discard timer,LC %d",um_ptr->chan_info[i].lc_id);
            }
            tdsuplink_table.active_ch--;

            if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
            {
              tdsrlc_dump_ul_ctl_blk_id_tbl();
            }

            tdsul_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id] = TDSINIT_CTL_BLK_ID;

            if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
            {
              tdsrlc_sanitize_ul_ctl_blk_id_tbl();
            }

            rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                 um_config_result[i].result = SUCCESS;
#ifdef FEATURE_QXDM_DSM_TRACING
            if (tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.rlc_id >= TDSRLC_UL_AM_USR_PLANE_START_LC_ID)
            {
              if (tdsnum_rlc_non_srb_entities_up > 0)
              {
                if (--tdsnum_rlc_non_srb_entities_up == 0)
                {
                  fc_post_deregister_cmd_dsm_trace
                    (DSM_UMTS_TECH,
                     UMTS_RLC_CLIENT);                     
                  TDSRLC_MSG0(MSG_LEGACY_HIGH,"Deregistered Qxdm DSM tracing");
                }
              }
              else
              {
                ERR_FATAL("RLC_ERR::Removing nonsrb entitiy, when num_non_srb entity is zero",0,0,0);
              }
            }
#endif

  #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
            tdsul_log_state.ul_state[k].rlc_id = um_ptr->chan_info[i].lc_id;
            tdsul_log_state.ul_state[k].lc_state = TDSRLC_UL_NULL_STATE;
            k++;
  #endif
          }
          else
          {
            /*-----------------------------------------------------------------
              If the channel does not exist, send a FAILURE indication in the 
              confirm message to RRC.
            -----------------------------------------------------------------*/
            TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d not config'd - No Release",um_ptr->chan_info[i].lc_id);
            rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                 um_config_result[i].result = FAILURE;
          }
          break;

        case TDSRLC_MODIFY:
          rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                 um_config_result[i].result = SUCCESS;

          if(um_ptr->ul_um_act[i].action == TDSRLC_MODIFY)
          {
            while(um_ptr->ul_um_act[i].ul_cfg_mask != 0)
            {
              action = tdsrlci_get_config_action(&(um_ptr->ul_um_act[i].ul_cfg_mask));
              switch(action)
              {
                case TDSRLCI_MODIFY:
                  /*-------------------------------------------------------------------
                    Check if Channel is in NULL state. If it is not, modify
                    the channel parameters and send a SUCCESS indication in the
                    confirm message to RRC.
                  -------------------------------------------------------------------*/
                  if ((index = tdsul_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id]) != TDSINIT_CTL_BLK_ID &&
                      (index < TDSUE_MAX_UL_RLC_ENTITY))
                  {
                    //TDSL2_ACQ_UL_LW_LOCK();
                    tdsuplink_table.ctrl_blk[index].lc_type =
                                          um_ptr->chan_info[i].lc_type;
                    tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.act_incl =
                                          um_ptr->ul_um_act[i].act_incl;
                    tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.act_time =
                                          um_ptr->ul_um_act[i].act_time;
                    /*uplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard =
                                          um_ptr->chan_info[i].timer_discard; */

                    if (um_ptr->chan_info[i].timer_discard !=
                        tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard)
                    {
                      /*---------------------------------------------------------
                        if discard timer values changes it update the
                         discard timer pointer
                      ---------------------------------------------------------*/
                      if (um_ptr->chan_info[i].timer_discard == 0)
                      {
                          /*-----------------------------------------------------
                            if discard timer is tunred OFF which was previosly ON 
                          -----------------------------------------------------*/
                        (void) rex_clr_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr);
                        rex_delete_timer_ex(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr);
                        
                        tdsuplink_table.ctrl_blk[index].tmr_discard_ptr = NULL;
                        tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard =
                                                  um_ptr->chan_info[i].timer_discard;

                        TDSRLC_MSG1(MSG_LEGACY_HIGH,"Release SDU discard timer,LC %d",um_ptr->chan_info[i].lc_id);
                      } 
                      else
                      {
                        if (tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard == 0)
                        {
                          /*-------------------------------------------------------
                          if  discard timer is turned ON which was previosly OFF. 
                          --------------------------------------------------------*/ 
                          tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard = 
                                                           um_ptr->chan_info[i].timer_discard;
                          tdsrlci_set_sdu_discard_timer(index, um_ptr->chan_info[i].lc_id);
                        }
                        else if ((um_ptr->chan_info[i].timer_discard > 
                                  tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard) &&
                                  (tdsuplink_table.ctrl_blk[index].tmr_discard_ptr != NULL))
                        {
                          /*-------------------------------------------------------
                          if  discard timer time was increased
                          -------------------------------------------------------*/ 
                          tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard = 
                                                                 um_ptr->chan_info[i].timer_discard;

                          /* set the  timer to (new timer value - elapsed time ) 	*/
                          (void)rex_set_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr,
                                (um_ptr->chan_info[i].timer_discard - rex_get_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr)));
                        }
                        else if (tdsuplink_table.ctrl_blk[index].tmr_discard_ptr != NULL)
                        {
                          /*----------------------------------------------------------------------------
                          if  discard timer time was decreased
                          -----------------------------------------------------------------------------*/ 
                          if (rex_get_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr) >= 
                              um_ptr->chan_info[i].timer_discard)
                          {
                            /*  if timer count > new timer value */
                            tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard =um_ptr->chan_info[i].timer_discard;

                            /* calls callback function immediately */
                            (void)rex_set_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr,0);
                          }
                          else
                          {
                            /*	if timer count < new timer value */
                            tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard =um_ptr->chan_info[i].timer_discard;

                            /* set the  timer to (new timer value - elapsed time )	 */
                            (void)rex_set_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr,(um_ptr->chan_info[i].timer_discard - rex_get_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr)));
                          }
                        }
                      }
                    }
                    else
                    {
                      tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard = 
                                                      um_ptr->chan_info[i].timer_discard;
                    }

                    tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.li_length =
                          ((um_ptr->chan_info[i].rlc_size >> 3) <= 125) ? 1 : 2;
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                         um_config_result[i].result = SUCCESS;
          #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                    if (j < TDSUE_MAX_UL_UM_CHANNEL)
                    {
                      tdsul_um_log_ptr.chan_info[j] = um_ptr->chan_info[i];
                      j++;
                    }
          #endif
                   //TDSL2_ACQ_UL_LW_UNLOCK();
                   TDSRLC_MSG2(MSG_LEGACY_HIGH,"UL UM Modify LC %d,Index %d",um_ptr->chan_info[i].lc_id,index);
                  }
                  else
                  {
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d not config'd - No Mod",um_ptr->chan_info[i].lc_id);
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                         um_config_result[i].result = FAILURE;
                  }
                  break;
          
                case TDSRLCI_STOP:
                  if ((index = tdsul_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id]) != TDSINIT_CTL_BLK_ID &&
                      (index < TDSUE_MAX_UL_RLC_ENTITY))
                  {
                    //TDSL2_ACQ_UL_LW_LOCK();
                    /* Enter into Stop state. */
                    /*lint -save -e64 -e641 -e655 */
                    tdsuplink_table.ctrl_blk[index].lc_state |= TDSUL_STOP;
                    /* Remove the Data transfer ready state.  */
                    tdsuplink_table.ctrl_blk[index].lc_state &= (~TDSUL_DATA_TRANSFER_READY);
                    /*lint -restore */
                    //TDSL2_ACQ_UL_LW_UNLOCK();
                    
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                         um_config_result[i].result = SUCCESS;
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d - Stop",um_ptr->chan_info[i].lc_id);
                  }
                  else
                  {
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d not config'd - No Stop",um_ptr->chan_info[i].lc_id);
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                         um_config_result[i].result = FAILURE;
                  }
                  break;
          
                case TDSRLCI_CONTINUE:
                  if ((index = tdsul_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id]) != TDSINIT_CTL_BLK_ID &&
                      (index < TDSUE_MAX_UL_RLC_ENTITY))
                  {
                    //TDSL2_ACQ_UL_LW_LOCK();
                    /* Enter into the orignal states before enter Stop state. */
                    /*lint -save -e64 -e641 -e655 */
                    tdsuplink_table.ctrl_blk[index].lc_state &= (~TDSUL_STOP);
                    tdsuplink_table.ctrl_blk[index].lc_state |= TDSUL_DATA_TRANSFER_READY;
                    /*lint -restore */
                    //TDSL2_ACQ_UL_LW_UNLOCK();
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                         um_config_result[i].result = SUCCESS;
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d - Continue",um_ptr->chan_info[i].lc_id);
                  }
                  else
                  {
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d not config'd - No Continue",um_ptr->chan_info[i].lc_id);
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                         um_config_result[i].result = FAILURE;
                  }
                  break;
          
                case TDSRLCI_RE_ESTABLISH:
                  if ((index = tdsul_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id]) != TDSINIT_CTL_BLK_ID &&
                      (index < TDSUE_MAX_UL_RLC_ENTITY))
                  {
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d - Re Establish",um_ptr->chan_info[i].lc_id);
                    tdsrlci_re_establish_ul_um(&tdsuplink_table.ctrl_blk[index], &um_ptr->chan_info[i]);
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                         um_config_result[i].result = SUCCESS;
          
          #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                    if (j < TDSUE_MAX_UL_UM_CHANNEL)
                    {
                      tdsul_um_log_ptr.chan_info[j] = um_ptr->chan_info[i];
                      j++;
                    }
          #endif
                  }
                  else
                  {
                    /*-----------------------------------------------------------------
                      The channel does not exist, send a FAILURE indication in the
                      confirm message to RRC.
                    -----------------------------------------------------------------*/
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d not config'd - No Re-establish",um_ptr->chan_info[i].lc_id);
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                         um_config_result[i].result = FAILURE;
                  }
                  break;
          
          
                default:
                  TDSRLC_MSG1(MSG_LEGACY_HIGH,"NO ACTION SPECIFIED FOR LC %d ",um_ptr->chan_info[i].lc_id);
                  break;
              }
            }
          }
          break;
        default:
          TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL UM:Invalid Action %d",um_ptr->ul_um_act[i].action);
          rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                               um_config_result[i].result = FAILURE;
      }
    }
  }          
  
  TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_DL_UNLOCK();
  
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (j > 0) 
  {
    tdsul_um_log_ptr.nchan = j;
    tdsrlc_ul_send_um_log_packet(&tdsul_um_log_ptr);
  }
  if (k > 0) 
  {
    tdsul_log_state.nchan = k;
    tdsrlc_ul_send_state_log_packet(&tdsul_log_state);
  }
#endif
  tdsrrc_put_rlc_cmd(rrc_rlc_cmd_ptr);

}/* tdscrlc_ul_um_config_handler() */


/*===========================================================================

FUNCTION CRLC_UL_AM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of an Acknowledged Mode entity.
  it is called when RRC sends a CRLC_UL_AM_CONFIG_REQ to RLC. If a new 
  entity is to be established, the function checks if the channels exists. 
  If it already exists, an ERR message is dispalyed. If the entity does not 
  exist, an uplink table entry for this entity is updated and the state is
  set to TDSUL_DATA_TRANSFER_READY. 
  If an entity is to be released,the function checks if the channel is in the 
  TDSUL_DATA_TRANSFER_READY state. If it is, the channel is set to the NULL state
  and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the 
  TDSUL_DATA_TRANSFER_READY state. If it is, the downlink table entry for the 
  channel is updated with the configuration received from RRC.


DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_am_config_handler 
(
  tdsrlc_am_config_req_type *am_ptr
)
{
  uint8 ctl_blk_id, no_channels_established;
  uint16 i;
  uint32 index = 0, act_ul_am_chnl = 0;
  tdsl2_dl_cmd_type *dl_cmd;
  tdsrlc_am_config_type *am_cfg_ptr;
  tdsrlc_channel_action_cnf_e_type action;
  
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  uint8 j = 0;
  uint8 k = 0;
#endif  

  dl_cmd = tdsl2_dl_get_cmd_buf();
  if (dl_cmd == NULL)
  {
    TDSRLC_MSG0(MSG_LEGACY_HIGH,"Could not get cmd buffer");
    return;
  }

  TDSRLC_MSG1(MSG_LEGACY_MED,"Rlc release version is %d",tdsl2_nv_rel_indicator);

  TDSL2_ACQ_DL_LOCK();
  /*Locking both EUL and R99 Mutex*/
  (void)TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);

  dl_cmd->cmd_hdr.cmd_id = TDSRLCI_DL_CONFIG_REQ;
  dl_cmd->cmd_data.dl_am_config_req.am_config_req.nentity = am_ptr->nentity;

  if(am_ptr->inter_RNC_ho == TDSRLC_INTER_RNC_HO_START)
  {
    tdsrlci_inter_ho_happen = TRUE;
  }
  else
  {
    tdsrlci_inter_ho_happen = FALSE;
    Bitmap_specail_handle.inter_rnc_ho_start = FALSE;
    Bitmap_specail_handle.inter_rnc_ho_end   = TRUE;
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Rcvd TDSRLC_INTER_RNC_HO_END or TDSRLC_INTER_RNC_HO_NA");
  }

  for (i=0; i<am_ptr->nentity; i++)
  {
    dl_cmd->cmd_data.dl_am_config_req.am_config_req.am_info[i] = 
                                                am_ptr->am_info[i];
    dl_cmd->cmd_data.dl_am_config_req.am_config_req.am_act[i].action = 
                                                am_ptr->am_act[i].action;
    dl_cmd->cmd_data.dl_am_config_req.am_config_req.am_act[i].dl_cfg_mask = am_ptr->am_act[i].dl_cfg_mask;
    am_cfg_ptr = am_ptr->am_info[i];
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
    if (tdsrlc_debug_var & TDSRLC_DBG_CORRUPTION)
    { 
      MSG_9(MSG_SSID_TDSCDMA_RLC, MSG_LEGACY_HIGH,
          "Padding_DBG: UL lc_id %d, action %d, dl_cfg_mask %d, \
           ul_cfg_mask %d, ul_rlc_size %d, dl_rlc_size %d",
           am_cfg_ptr->common_config.ul_data_id, am_ptr->am_act[i].action, 
           am_ptr->am_act[i].dl_cfg_mask, am_ptr->am_act[i].ul_cfg_mask, 
           am_ptr->am_info[i]->rlc_ul_am_config.ul_rlc_size, 
           am_ptr->am_info[i]->rlc_dl_am_config.dl_rlc_size, 0, 0, 0);
    }
#endif
    if (am_cfg_ptr->common_config.ul_data_id >= TDSUE_MAX_UL_LOGICAL_CHANNEL) 
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"Invalid UL AM Data LC Id %d",am_cfg_ptr->common_config.ul_data_id);
      dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
    }
#ifdef FEATURE_TDSCDMA_MEDIUM_DSM_BUFFER
	else  if((am_cfg_ptr->rlc_ul_am_config.ul_rlc_size > (TDSCDMA_DSMI_MEDIUM_ITEM_SIZ*8)) &&
#else
    else  if((am_cfg_ptr->rlc_ul_am_config.ul_rlc_size > TDSMAX_RLC_PDU_SIZE) &&
#endif	 	
                ((tdsl2_ul_get_trch_type(am_cfg_ptr->common_config.ul_data_id, am_cfg_ptr->common_config.rb_id) == TDSCDMA_MAC_TRCH_DCH) ||
                  (tdsl2_ul_get_trch_type(am_cfg_ptr->common_config.ul_data_id, am_cfg_ptr->common_config.rb_id) == TDSCDMA_MAC_TRCH_RACH)))
    {
      TDSRLC_MSG2(MSG_LEGACY_ERROR,"Incorrect value of ul_rlc_size %d,rlc_id %d",am_cfg_ptr->rlc_ul_am_config.ul_rlc_size ,am_cfg_ptr->common_config.ul_data_id);
      dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
    }
    else
    {
      switch(am_ptr->am_act[i].action)
      {
        case TDSRLC_ESTABLISH:
          if ((tdsul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_data_id] == TDSINIT_CTL_BLK_ID) &&
              ((ctl_blk_id = tdsul_assign_ctl_blk()) != TDSINIT_CTL_BLK_ID) &&(ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))         
          {
            if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
            {
              tdsrlc_dump_ul_ctl_blk_id_tbl();
            }

            tdsul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_data_id] = ctl_blk_id;
            tdsul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_control_id] = ctl_blk_id;
            if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
            {
              tdsrlc_sanitize_ul_ctl_blk_id_tbl();
            }
            

            /*--------------------------------------------------------------------
              Check for the configured value of MAXDAT. Since the maximum number 
              of transmissions allowed for a particular PDU is MAXDAT -1, MAXDAT
              should always be greater than 1.
            --------------------------------------------------------------------*/
            if( (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.discard_mode != TDSRLC_TBE) &&
                (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat < 2) )
            {
              TDSRLC_MSG1(MSG_LEGACY_ERROR,"Incorrect value of MAXDAT %d",am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat );
              dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
              break;

            }
            no_channels_established = 
               tdsrlci_establish_ul_am(&tdsuplink_table.ctrl_blk[ctl_blk_id], 
                                                              am_cfg_ptr);
            if(no_channels_established == 0xff)
            {
               TDSRLC_MSG0(MSG_LEGACY_HIGH,"no AM channel available");
               dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
               break;
            }

            else
            {
              tdsuplink_table.active_ch += no_channels_established;
            }
   
            /*-------------------------------------------------------------------------
              Register DSM flow control CB function for user data channel.
            -------------------------------------------------------------------------*/
            if(am_cfg_ptr->common_config.ul_data_id >= TDSRLC_UL_AM_USR_PLANE_START_LC_ID)
            {
#ifdef FEATURE_QXDM_DSM_TRACING
              if (tdsnum_rlc_non_srb_entities_up++ == 0)
              {
                fc_post_reg_cmd_dsm_trace
                  (DSM_UMTS_TECH,
                   UMTS_RLC_CLIENT,
                   &tdsrlc_fc_get_dsm_trace_cb,
                   0xDEADBEEF);
                TDSRLC_MSG0(MSG_LEGACY_HIGH,"Registered Qxdm DSM tracing");
              }
#endif
              tdsrlc_dl_multi_client_fc_init(&(tdsuplink_table.ctrl_blk[ctl_blk_id].ctrl.am_ctrl));
              /* Register with CFCM Module */
#ifdef FEATURE_TDSCDMA_CFCM
              tdsrlc_cfcm_register();  //Register only if supported in the modem.
#else  //FEATURE_TDSCDMA_CFCM.
	      TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_MULTI_FC: CFCM based FC registartion is not done as its not supported on this target");
#endif

#if 0 //CFCM_related_obsolete_2
We dont register for FC using these handles as we have CFCM
              TDSRLC_MSG0(MSG_LEGACY_HIGH,"Register DSM event CB");
/* Initializing the correct RLC FC (based on large item pool/small item pool)*/
              dsm_reg_mem_event_cb(DSM_DS_LARGE_ITEM_POOL, DSM_MEM_LEVEL_RLC_MANY,
                                   DSM_MEM_OP_FREE, tdsrlci_fc_dsm_cb);

              dsm_reg_mem_event_cb(DSM_DS_LARGE_ITEM_POOL, DSM_MEM_LEVEL_RLC_FEW,
                                   DSM_MEM_OP_NEW, tdsrlci_fc_dsm_cb);

              dsm_reg_mem_event_cb(DSM_DS_LARGE_ITEM_POOL, DSM_MEM_LEVEL_LINK_LAYER_DNE,
                                   DSM_MEM_OP_NEW, tdsrlci_fc_dsm_cb);

              dsm_reg_mem_event_cb(DSM_DS_SMALL_ITEM_POOL, DSM_MEM_LEVEL_RLC_MANY,
                                   DSM_MEM_OP_FREE, tdsrlci_fc_dsm_cb);

              dsm_reg_mem_event_cb(DSM_DS_SMALL_ITEM_POOL, DSM_MEM_LEVEL_RLC_FEW,
                                   DSM_MEM_OP_NEW, tdsrlci_fc_dsm_cb);

              dsm_reg_mem_event_cb(DSM_DS_SMALL_ITEM_POOL, DSM_MEM_LEVEL_LINK_LAYER_DNE,
                                   DSM_MEM_OP_NEW, tdsrlci_fc_dsm_cb);
              dsm_reg_mem_event_cb(DSM_DUP_ITEM_POOL, DSM_MEM_LEVEL_RLC_MANY,
                                   DSM_MEM_OP_FREE, tdsrlci_fc_dsm_cb);

              dsm_reg_mem_event_cb(DSM_DUP_ITEM_POOL, DSM_MEM_LEVEL_RLC_FEW,
                                   DSM_MEM_OP_NEW, tdsrlci_fc_dsm_cb);
#ifdef FEATURE_TDSCDMA_TEMPERATURE_FC
             /*Register with DEM for temperature notifications*/
              if( tdsrlc_temperature_fc_data.isDemRegistered == FALSE )
              {
#ifndef FEATURE_TDSCDMA_NPA_THERMAL
                tdsrlc_temperature_fc_data.isRlcDlFcEnabled =
                   therm_curr_mon_cfg_enabled(THERM_CURR_MON_CFG_DL_RLC_THROTTLE);
                if (tdsrlc_temperature_fc_data.isRlcDlFcEnabled )
                {
                  TDSRLC_MSG0(MSG_LEGACY_HIGH,"TEMP_FC::NV DL Temp FC enabled");
                }
                tdsrlc_temperature_fc_data.isRlcUlFcEnabled =
                   therm_curr_mon_cfg_enabled(THERM_CURR_MON_CFG_UL_RLC_THROTTLE);
                if (tdsrlc_temperature_fc_data.isRlcUlFcEnabled )
                {
                  TDSRLC_MSG0(MSG_LEGACY_HIGH,"TEMP_FC::NV UL Temp FC enabled");
                }

                if (tdsrlc_temperature_fc_data.isRlcDlFcEnabled || 
                    tdsrlc_temperature_fc_data.isRlcUlFcEnabled)
                {
                  (void)DEMNotificationRegister
                    (
                      SMSM_MODEM, 
                      DEM_NOTIFY_TEMP_OKAY, 
                      DEM_NOTIFICATION_PRIORITY_NORMAL, 
                      NULL, 
                      (DEM_NOTIFICATION_CALLBACK_TYPE) tdsrlc_temperature_fc_dem_okay_cb
                    );
                  (void)DEMNotificationRegister
                    (
                      SMSM_MODEM, 
                      DEM_NOTIFY_TEMP_WARNING, 
                      DEM_NOTIFICATION_PRIORITY_NORMAL, 
                      NULL, 
                      (DEM_NOTIFICATION_CALLBACK_TYPE) tdsrlc_temperature_fc_dem_warning_cb
                    );
                  (void)DEMNotificationRegister
                    (
                      SMSM_MODEM, 
                      DEM_NOTIFY_TEMP_CRITICAL, 
                      DEM_NOTIFICATION_PRIORITY_NORMAL, 
                      NULL, 
                      (DEM_NOTIFICATION_CALLBACK_TYPE) tdsrlc_temperature_fc_dem_critical_cb
                    );
                }
                else
                {
                  TDSRLC_MSG0(MSG_LEGACY_HIGH,"TEMP_FC::NV UL&DL Temp FC disabled");
                }
#else
                npa_resource_available_cb("/therm/mitigate/pa",
                             tdsrlc_intialize_temperature_event_cb,
                             NULL);
                tdsrlc_temperature_fc_data.isRlcDlFcEnabled = TRUE;
                tdsrlc_temperature_fc_data.isRlcUlFcEnabled = TRUE;
#endif

                tdsrlc_temperature_fc_data.isDemRegistered = TRUE;
              }
#endif


#if 0
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
              /* Register DL FC */
              tdsrlc_dl_fc_register();
              /* Register UL FC */
              tdsrlc_ul_fc_register();
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

#endif //CFCM_related_obsolete_2
#endif
            }

            TDSRLC_MSG2(MSG_LEGACY_HIGH,"UL AM Establish: Data Id=%d,index=%d",am_cfg_ptr->common_config.ul_data_id,ctl_blk_id);
  
            dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
  
  #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
            tdsul_am_log_ptr.am_info[j] = am_cfg_ptr;
            j++;
            tdsul_log_state.ul_state[k].rlc_id = am_cfg_ptr->common_config.ul_data_id;
            tdsul_log_state.ul_state[k].lc_state = TDSAM_UL_CONFIG_PENDING;
            k++;
  #endif
          }
          else
          {
            dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
            TDSRLC_MSG2(MSG_LEGACY_HIGH,"Channel in using -- Data:%d,Control:%d",am_cfg_ptr->common_config.ul_data_id,am_cfg_ptr->common_config.ul_control_id);
          }
          break;
        case TDSRLC_RELEASE:
          if ((ctl_blk_id = tdsul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_data_id])
                          != TDSINIT_CTL_BLK_ID &&
               (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
          {
            tdsuplink_table.active_ch -= tdsrlci_release_ul_am(&tdsuplink_table.ctrl_blk[ctl_blk_id]);
            if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
            {
              tdsrlc_dump_ul_ctl_blk_id_tbl();
            }
            
            tdsul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_data_id] = TDSINIT_CTL_BLK_ID;
            tdsul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_control_id] = TDSINIT_CTL_BLK_ID;

            if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
            {
              tdsrlc_sanitize_ul_ctl_blk_id_tbl();
            }
            

            for (index = 0, act_ul_am_chnl = 0; index < TDSUE_MAX_UL_LOGICAL_CHANNEL; index++)
            {
              ctl_blk_id = tdsul_ctl_blk_id_tbl[index];
              
              if ((ctl_blk_id != TDSINIT_CTL_BLK_ID) && 
                   (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY) &&
                  ((TDSUL_NULL_STATE != tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state) && 
                  (UE_MODE_ACKNOWLEDGED == tdsuplink_table.ctrl_blk[ctl_blk_id].lc_mode)))
              {
                act_ul_am_chnl++;
                break;
              }
            }
            
            if ((0 == act_ul_am_chnl) && (TRUE == tdsrlc_ul_am_base_tmr_start))
            {
              /* Since all AM channels are released, stop base ul am timer */
              (void) rex_clr_timer(&tdsrlc_ul_am_base_tmr);
              tdsrlc_ul_am_base_tmr_start = FALSE;

              TDSRLC_MSG1(MSG_LEGACY_HIGH, "RLC_TMR_DBG: Stopping UL base timer,active_ul_channels %d",tdsuplink_table.active_ch);
            }
            dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
            TDSRLC_MSG2(MSG_LEGACY_HIGH, "UL AM LC %d is released,active_ul_channels %d",am_cfg_ptr->common_config.ul_data_id,tdsuplink_table.active_ch);
  
  #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
            tdsul_log_state.ul_state[k].rlc_id = am_cfg_ptr->common_config.ul_data_id;
            tdsul_log_state.ul_state[k].lc_state = TDSRLC_UL_NULL_STATE;
            k++;
  #endif

#ifdef FEATURE_QXDM_DSM_TRACING
            if (am_cfg_ptr->common_config.ul_data_id >= TDSRLC_UL_AM_USR_PLANE_START_LC_ID)
            {
              if (tdsnum_rlc_non_srb_entities_up > 0)
              {
                if (--tdsnum_rlc_non_srb_entities_up == 0)
                {
                  fc_post_deregister_cmd_dsm_trace
                    (DSM_UMTS_TECH,
                     UMTS_RLC_CLIENT);                     
                  TDSRLC_MSG0(MSG_LEGACY_HIGH,"Deregistered Qxdm DSM tracing");
                }
              }
              else
              {
                ERR_FATAL("RLC_ERR::Removing nonsrb entitiy, when num_non_srb entity is zero",0,0,0);
              }
            }
#endif


#if defined (FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL)
            if(tdsrlc_acti_non_srb_am_ch &&
               am_cfg_ptr->common_config.ul_data_id >= TDSRLC_UL_AM_USR_PLANE_START_LC_ID)
            {
              tdsrlc_acti_non_srb_am_ch--;
              if(!tdsrlc_acti_non_srb_am_ch)
              {
#ifdef FEATURE_TDSCDMA_CFCM			    
		tdsrlc_cfcm_deregister(); 
#endif
            }
            }
#endif /*(FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */
          }
          else
          {
            dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
            TDSRLC_MSG1(MSG_LEGACY_HIGH,"Channel %d not Config'd,No Release",am_cfg_ptr->common_config.ul_data_id);
          }
          break;
  
        case TDSRLC_MODIFY:
          dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;

          if(am_ptr->am_act[i].action == TDSRLC_MODIFY)
          {
            while(am_ptr->am_act[i].ul_cfg_mask != 0)
            {
              action = tdsrlci_get_config_action(&(am_ptr->am_act[i].ul_cfg_mask));
              dl_cmd->cmd_data.dl_am_config_req.am_config_req.am_act[i].ul_cfg_mask = 0;
              switch(action)
              {
                case TDSRLCI_MODIFY :
                  if ((ctl_blk_id = tdsul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_data_id])
                                  != TDSINIT_CTL_BLK_ID &&
                       (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
                  {
                    /*--------------------------------------------------------------------
                      Check for the value of MAXDAT as it might change during a
                      modify procedure. Since the maximum number of transmissions
                      allowed for a particular PDU is MAXDAT -1, MAXDAT should always be
                      greater than 1.
                    --------------------------------------------------------------------*/
                    if( (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.discard_mode != TDSRLC_TBE) &&
                        (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat < 2) )
                    {
                      TDSRLC_MSG1(MSG_LEGACY_ERROR,"Incorrect value of MAXDAT %d",am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat );
                      dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                      break;
        
                    }
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d - Modify",am_cfg_ptr->common_config.ul_data_id);
                    tdsrlci_modify_ul_am(&tdsuplink_table.ctrl_blk[ctl_blk_id], am_cfg_ptr);
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
        
  #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                    if(j < TDSUE_MAX_AM_ENTITY)
                    {
                      tdsul_am_log_ptr.am_info[j] = am_cfg_ptr;
                      j++;
                    }
  #endif
                  }
                  else
                  {
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"Channel %d not Config'd,No Modi",am_cfg_ptr->common_config.ul_data_id);
                  }
                  break;
    
                case TDSRLCI_STOP:
                  if ((ctl_blk_id = tdsul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_data_id])
                                  != TDSINIT_CTL_BLK_ID &&
                       (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
                  {
                    /*
                      This LC state is checked by ISR for every LC, lets make it
                      atomic
                    */
                    //TDSL2_ACQ_UL_LW_LOCK();
                    /* Enter into Stop state. */
                    /*lint -save -e64 -e641 -e655 */
                    tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state |= TDSUL_STOP;
                    /* Remove the Data transfer ready state.  */
                    tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state &= (~TDSUL_DATA_TRANSFER_READY);
                    //TDSL2_ACQ_UL_LW_UNLOCK();
                    
                    /*lint -restore */
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d - Stop",am_cfg_ptr->common_config.ul_data_id);
                  }
                  else
                  {
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d not config'd - No Stop",am_cfg_ptr->common_config.ul_data_id);
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                  }
                  break;
    
                case TDSRLCI_CONTINUE:
                  if ((ctl_blk_id = tdsul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_data_id])
                                  != TDSINIT_CTL_BLK_ID &&
                      (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
                  {
                    /*
                      Stop till ISR finishes.
                      This LC state is checked by ISR for every LC, lets make it
                      atomic.
                      
                    */
                    //TDSL2_ACQ_UL_LW_LOCK();
                    /* Enter into the orignal states before enter Stop state. */
                    /*lint -save -e64 -e641 -e655 */ 
                    tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state &= (~TDSUL_STOP);
                    tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state |= TDSUL_DATA_TRANSFER_READY;
                    /*lint -restore */
                    //TDSL2_ACQ_UL_LW_UNLOCK();
                    
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d - Continue",am_cfg_ptr->common_config.ul_data_id);
                  }
                  else
                  {
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d not config'd - No Continue",am_cfg_ptr->common_config.ul_data_id);
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                  }
                  break;
    
                case TDSRLCI_RE_ESTABLISH:
                  if((ctl_blk_id = 
                      tdsul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_data_id]) 
                      != TDSINIT_CTL_BLK_ID &&
                      (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
                  {
                    /*--------------------------------------------------------------------
                      Check for the value of MAXDAT as it might change during a 
                      re-establish procedure. Since the maximum number of transmissions 
                      allowed for a particular PDU is MAXDAT -1, MAXDAT should always be 
                      greater than 1.
                    --------------------------------------------------------------------*/
                    if( (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.discard_mode != TDSRLC_TBE) &&
                        (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat < 2) )
                    {
                      TDSRLC_MSG1(MSG_LEGACY_ERROR,"Incorrect value of MAXDAT %d",am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat );
                      dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                      break;
        
                    }
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC %d - Re Establish",am_cfg_ptr->common_config.ul_data_id);
                    tdsrlci_re_establish_ul_am(&tdsuplink_table.ctrl_blk[ctl_blk_id], am_cfg_ptr);
        
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
                    dl_cmd->cmd_data.dl_am_config_req.am_config_req.am_act[i].ul_cfg_mask |= TDSRLC_RE_ESTABLISH_CFG;

  #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                    if(j < TDSUE_MAX_AM_ENTITY)
                    {
                      tdsul_am_log_ptr.am_info[j] = am_cfg_ptr;
                      j++;
                    }
                    if(k < TDSUE_MAX_UL_LOGICAL_CHANNEL)
                    {
                      tdsul_log_state.ul_state[k].rlc_id = am_cfg_ptr->common_config.ul_data_id;
                      tdsul_log_state.ul_state[k].lc_state = TDSAM_UL_CONFIG_PENDING;
                      k++;
                    }
  #endif
                  }
                  else
                  {
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"Channel %d not Config'd,Unable to re-establish ",am_cfg_ptr->common_config.ul_data_id);
        
                  }
        
                  break;
    
                default:
                  TDSRLC_MSG1(MSG_LEGACY_HIGH,"NO ACTION SPECIFIED FOR LC %d ",am_cfg_ptr->common_config.ul_data_id);
                  break;
              }
            }
          }
          break;
        default:
          dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
          TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL AM:Invalid Action %d",am_ptr->am_act[i].action);
      }
    }
  }          

  TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_DL_UNLOCK();

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (j > 0) 
  {
    tdsul_am_log_ptr.nentity = j;
    tdsrlc_ul_send_am_log_packet(&tdsul_am_log_ptr);
  }
  if (k > 0) 
  {
    tdsul_log_state.nchan = k;
    tdsrlc_ul_send_state_log_packet(&tdsul_log_state);
  }
#endif
  /* post config request to Downlink */
  tdsl2_dl_put_cmd(dl_cmd);

} /* tdscrlc_ul_am_config_handler() */



/*===========================================================================

FUNCTION  RLCI_SET_SDU_DISCARD_TIMER 

DESCRIPTION
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_set_sdu_discard_timer
(
  uint8 index, 
  rlc_lc_id_type rlc_id
)
{
  dsm_item_type *sdu_ptr = NULL;

  switch (tdsuplink_table.ctrl_blk[index].lc_mode)
  {
    case UE_MODE_TRANSPARENT:
      if (tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.timer_discard != 0)
      {
        if (NULL == tdsuplink_table.ctrl_blk[index].tmr_discard_ptr)
        {
          tdsuplink_table.ctrl_blk[index].tmr_discard_ptr =
            rex_create_timer_ex(tdsrlci_tm_handle_discard_timer_cb, 
                                (uint32)(rlc_id));
          if (NULL == tdsuplink_table.ctrl_blk[index].tmr_discard_ptr)
          {
            TDSRLC_MSG1(MSG_LEGACY_HIGH,"Fail,create RLC UL TM %d Discard Timer",rlc_id);
            return;
          }
        }

        TDSRLC_MSG2(MSG_LEGACY_HIGH,"Set TM discard timer,LC %d,index %d",rlc_id,index);
        (void) rex_set_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr, 
                      (uint32)tdsuplink_table.ctrl_blk[index].ctrl.tm_ctrl.timer_discard);

        if ((sdu_ptr = (dsm_item_type *)q_check(tdsuplink_table.ctrl_blk[index].ul_wm_ptr->q_ptr)) != NULL)
        {
          while (sdu_ptr != NULL)
          {
            /* Put the discard timer stamp on the SDU */
            sdu_ptr->app_field |= 0x020000;
            sdu_ptr = (dsm_item_type *)q_next(tdsuplink_table.ctrl_blk[index].ul_wm_ptr->q_ptr, &(sdu_ptr->link));
          }
        }
      }
      break;

    case UE_MODE_UNACKNOWLEDGED:
      if (tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard != 0)
      {
        if (NULL == tdsuplink_table.ctrl_blk[index].tmr_discard_ptr)
        {
          tdsuplink_table.ctrl_blk[index].tmr_discard_ptr =
            rex_create_timer_ex(tdsrlci_um_handle_discard_timer_cb, 
                                (uint32)(rlc_id));
          if (NULL == tdsuplink_table.ctrl_blk[index].tmr_discard_ptr)
          {
            TDSRLC_MSG1(MSG_LEGACY_HIGH,"Fail,create RLC UL UM %d Discard Timer",rlc_id);
            return;
          }
        }

        TDSRLC_MSG1(MSG_LEGACY_HIGH,"Set UM discard timer,%d",rlc_id);
        (void) rex_set_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr, 
                      (uint32)tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.timer_discard);
        if (NULL != tdsuplink_table.ctrl_blk[index].cur_sdu_ptr)
        {
          tdsuplink_table.ctrl_blk[index].cur_sdu_ptr->app_field |= 0x04;
        }
        if ((sdu_ptr = (dsm_item_type *)q_check(tdsuplink_table.ctrl_blk[index].ul_wm_ptr->q_ptr)) != NULL)
        {
          while (sdu_ptr != NULL)
          {
            /* Put the discard timer stamp on the SDU */
            sdu_ptr->app_field |= 0x04;
            sdu_ptr = (dsm_item_type *)q_next(tdsuplink_table.ctrl_blk[index].ul_wm_ptr->q_ptr, &(sdu_ptr->link));
          }
        }
      }
      break;

    case UE_MODE_ACKNOWLEDGED:
      break;

    default:
      TDSRLC_MSG0(MSG_LEGACY_MED,"UE mode not valid");
  }
}/* tdsrlci_set_sdu_discard_timer() */


/*===========================================================================

FUNCTION DTSCLRLC_UL_REG_FOR_DATA_IND_HANDLER

DESCRIPTION
  Register Data Indication callback function to registered
  WM's non_empty_func_ptr.  
  This function is for Cell_PCH.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsclrlc_ul_reg_for_data_ind_handler
(
  tdsrlc_ul_register_for_data_ind_type *ul_reg_data_ind_ptr
)
{
  int i;
  uint16 index;
  uint32 retx_size = 0;

  for (i = 0; i < ul_reg_data_ind_ptr->num_lc; i++)
  {
  
    index = tdsul_ctl_blk_id_tbl[ul_reg_data_ind_ptr->lc_id[i]];
    if ((ul_reg_data_ind_ptr->lc_id[i] < TDSUE_MAX_UL_LOGICAL_CHANNEL) &&
        (index != TDSINIT_CTL_BLK_ID) &&
        (index < TDSUE_MAX_UL_RLC_ENTITY))
    {
      /*-------------------------------------------------------------------------
        Block Status Timer flag is set. Suspend the 20ms RLC AM timer.
      -------------------------------------------------------------------------*/
      if (ul_reg_data_ind_ptr->block_status_timer &&
          (tdsuplink_table.ctrl_blk[index].lc_mode == UE_MODE_ACKNOWLEDGED))
      {
        tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_tmr_sts = TDSRLC_AM_TIMER_STOP;
        TDSRLC_MSG2(MSG_LEGACY_HIGH, "RLC_TMR_DBG: Stopping for UL lc_id %d,active_ul_channels %d",ul_reg_data_ind_ptr->lc_id[i],tdsuplink_table.active_ch);

        tdsrlci_suspend_dl_am_timer          (tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.am_common->dl_data_id);
      }

      if (tdsuplink_table.ctrl_blk[index].ul_wm_ptr != NULL)
      {
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"Register UL DATA IND,LC %d",ul_reg_data_ind_ptr->lc_id[i]);
        tdsuplink_table.ctrl_blk[index].ul_wm_ptr->non_empty_func_ptr =
                                    tdsrrccu_rlc_ul_data_ind;
        if (TDSRLCI_SEQ_GT(tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_s, 
           tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_ms, TDSRLC_AM_MODULUS))
        {
          retx_size = tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.retx_size_reduced;
        }
        else
        {
          retx_size = tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.retx_size;
        }
        /*------------------------------------------------------------------
          Some PDUs need to be retx ((retx_size > 0) or if there are some 
          PDUs with SN less than vt_s for which ACK has not yet been received
          and polling is configured and poll timer has hit) or there is data
          sitting in the watermark or a control PDU needs to be sent. Set a 
          20ms timer since RRC needs to finish the transition to Cell_PCH 
          state. After the timer expiry, it should call the data indication 
          callback function to leave the Cell_PCH state.
        ------------------------------------------------------------------*/
        if(((tdsuplink_table.ctrl_blk[index].lc_mode == UE_MODE_ACKNOWLEDGED) &&
            (((tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_s != 
              tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_a) && 
              (((TRUE == tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.poll_next) && 
                (FALSE == tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.poll_prohibit))||
                (retx_size > 0) || (tdsuplink_table.ctrl_blk[index].lc_type == UE_LOGCHAN_DCCH))) ||
              tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.ack_pdu)) ||
           (tdsuplink_table.ctrl_blk[index].ul_wm_ptr->current_cnt != 0)
           || (NULL != tdsuplink_table.ctrl_blk[index].cur_sdu_ptr)
         )
        {
          if (NULL != tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.tmr_cellpch_ptr)
          {
            TDSRLC_MSG3(MSG_LEGACY_HIGH,"10ms cellpch timer running for LC %d, WM ptr 0x%x, WM cnt %d", 
                        ul_reg_data_ind_ptr->lc_id[i], 
                        tdsuplink_table.ctrl_blk[index].ul_wm_ptr,
                        tdsuplink_table.ctrl_blk[index].ul_wm_ptr->current_cnt);
          }
          else if((tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.tmr_cellpch_ptr = 
                rex_create_timer_ex(tdsrlci_am_handle_cellpch_timer_cb, 
                          (uint32)(ul_reg_data_ind_ptr->lc_id[i]))) != NULL)
          {
            TDSRLC_MSG3(MSG_LEGACY_HIGH,"Set 10ms cellpch timer,LC %d, WM ptr 0x%x, WM cnt %d", 
                        ul_reg_data_ind_ptr->lc_id[i], 
                        tdsuplink_table.ctrl_blk[index].ul_wm_ptr,
                        tdsuplink_table.ctrl_blk[index].ul_wm_ptr->current_cnt);
            (void)rex_set_timer(tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.tmr_cellpch_ptr, 
                        (uint32)TDSRLC_UL_TIMER_BASE);
          }
        }

      }
      else
      {
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"WM not reg'd,LC %d",ul_reg_data_ind_ptr->lc_id[i]);
      }

    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"LC %d not config",ul_reg_data_ind_ptr->lc_id[i]);
    } 
  }
}/* tdsclrlc_ul_reg_for_data_ind_handler() */


/*===========================================================================

FUNCTION CLRLC_UL_UNREG_FOR_DATA_IND_HANDLER

DESCRIPTION
  De-Register Data Indication callback function from registered
  WM's non_empty_func_ptr.  
  This function is for Cell_PCH.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsclrlc_ul_unreg_for_data_ind_handler
(
  tdsrlc_ul_unregister_for_data_ind_type *ul_unreg_data_ind_ptr
)
{
  int i;
  uint16 index;

  for (i = 0; i < ul_unreg_data_ind_ptr->num_lc; i++)
  {
  
    index = tdsul_ctl_blk_id_tbl[ul_unreg_data_ind_ptr->lc_id[i]];
    if ((ul_unreg_data_ind_ptr->lc_id[i] < TDSUE_MAX_UL_LOGICAL_CHANNEL) &&
        (index != TDSINIT_CTL_BLK_ID) &&
        (index < TDSUE_MAX_UL_RLC_ENTITY))
    {

      /*-------------------------------------------------------------------------
        Timer resume flag is set. Resume the timer which is suspended when 
        registering the data_ind callback.
      -------------------------------------------------------------------------*/
      if (ul_unreg_data_ind_ptr->resume_status_timer &&
          (tdsuplink_table.ctrl_blk[index].lc_mode == UE_MODE_ACKNOWLEDGED))
      {
        tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_tmr_sts = TDSRLC_AM_TIMER_RUNNING;

        TDSRLC_MSG2(MSG_LEGACY_HIGH, "RLC_TMR_DBG: Resuming for UL lc_id %d,active_ul_channels %d",ul_unreg_data_ind_ptr->lc_id[i],tdsuplink_table.active_ch);

        tdsrlci_resume_dl_am_timer           (tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.am_common->dl_data_id);
      }
      if (tdsuplink_table.ctrl_blk[index].ul_wm_ptr != NULL)
      {
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"UNRegister UL DATA IND,LC %d",ul_unreg_data_ind_ptr->lc_id[i]);
        tdsuplink_table.ctrl_blk[index].ul_wm_ptr->non_empty_func_ptr = NULL;
      }
      else
      {
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"WM not reg'd,LC %d",ul_unreg_data_ind_ptr->lc_id[i]);
      }
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"LC %d not config",ul_unreg_data_ind_ptr->lc_id[i]);
    } 
  }
}/* tdsclrlc_ul_unreg_for_data_ind_handler() */


/*===========================================================================

FUNCTION RLCI_AM_HANDLE_CELLPCH_TIMER_CB

DESCRIPTION
  Process CELLPCH_TIMER.
  
  This timer is set because of finding some retx PDUs remaining when RRC 
  registers data_ind CB to go to CELL_PCH. Since the RRC has to finish 
  its procedure, we set the timer and come back later to move UE out of
  CELL_PCH to send those remaining retx PDUs.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_am_handle_cellpch_timer_cb(uint32 cb_param)
{
  rlc_lc_id_type rlc_id = (rlc_lc_id_type)cb_param;
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr;

  /*---------------------------------------------------------------------------
    Obtain control block pointer based on the passed-in RLC logical channel ID.      
  ----------------------------------------------------------------------------*/
  if ((ctl_blk_ptr = tdsrlci_get_ctl_blk_ptr(rlc_id)) == NULL)
  {
    return;
  }

  /*-----------------------------------------------------------------------
    Release this Cell_PCH timer.
  -----------------------------------------------------------------------*/
  rex_delete_timer_ex(ctl_blk_ptr->ctrl.am_ctrl.tmr_cellpch_ptr);
  ctl_blk_ptr->ctrl.am_ctrl.tmr_cellpch_ptr = NULL;
  
  /*-----------------------------------------------------------------------
    Call the data indication callback function to leave CELL_PCH.
  -----------------------------------------------------------------------*/
  if((ctl_blk_ptr->ul_wm_ptr != NULL) && 
     (ctl_blk_ptr->ul_wm_ptr->non_empty_func_ptr != NULL))
  {
    TDSRLC_MSG1(MSG_LEGACY_HIGH,"Call data_ind CB,rlc_id %d",rlc_id);
 
     ctl_blk_ptr->ul_wm_ptr->non_empty_func_ptr(NULL, NULL);
 
  }
}/* tdsrlci_am_handle_cellpch_timer_cb() */


/*===========================================================================

FUNCTION TDSRLC_CHECK_PENDING_SDU

DESCRIPTION
  Checks if there is any data pending for a given LC, or if there are
  any pending ACKs awaited.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - if data is available or ACK is awaited
  otherwise FALSE
  
SIDE EFFECTS
  None.

===========================================================================*/

boolean tdsrlc_check_pending_sdu
(
  rlc_lc_id_type lc_id
)
{
  tdsrlci_ul_ctrl_blk_type   *ctl_blk_ptr = NULL;
  tdsrlci_ul_am_state_type   *ul_state_ptr = NULL;
  uint32      num_bytes = 0;

  if(((ctl_blk_ptr = tdsrlci_get_ctl_blk_ptr(lc_id)) == NULL) ||
     (ctl_blk_ptr->lc_mode != UE_MODE_ACKNOWLEDGED))
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"UL LC ID %d not present",lc_id);
    return FALSE;
  }

  ul_state_ptr = &(ctl_blk_ptr->ctrl.am_ctrl.ul_state);

  num_bytes = tdsrlc_rrc_get_wm_info_req(lc_id);

  if ((ul_state_ptr->vt_a != ul_state_ptr->vt_s) || 
      (num_bytes > 0))
  {
     TDSRLC_MSG3(MSG_LEGACY_HIGH,"Data available num_bytes %d, vt_a %d, vt_s %d", 
       num_bytes, ul_state_ptr->vt_a, ul_state_ptr->vt_s);
     return(TRUE);
  }
  else
  {
    return(FALSE);
  }
}



/*===========================================================================

FUNCTION RLC_UL_REGISTER_SRVC_HANDLER

DESCRIPTION
  This function handles the registration of the watermark for the uplink
  channel.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_register_srvc_handler 
(
  tdsrlc_ul_register_srvc_type *reg_ptr   /* Ptr to WM registration info.     */
)
{
  int i;
  uint8 index;
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS
  ds_umts_qos_tclass_enum_type traffic_class;
#endif 
  rex_crit_sect_type *datapath_mutex = NULL;

  TDSL2_ACQ_DL_LOCK();

  TDSRLC_MSG1(MSG_LEGACY_HIGH,"RLC_WM: Register UL WM called,nchan %d",reg_ptr->nchan);

  /*-------------------------------------------------------------------------
    Register Uplink Watermark for all Channels indicated in the registration
    pointer.
  -------------------------------------------------------------------------*/
  for (i = 0; i < reg_ptr->nchan; i++)
  {
    TDSRLC_MSG2(MSG_LEGACY_HIGH,"RLC_WM: LC id %d UL-WM_ptr 0x%x",reg_ptr->rlc_data[i].lc_id,reg_ptr->rlc_data[i].ul_wm_ptr);

    index = tdsul_ctl_blk_id_tbl[reg_ptr->rlc_data[i].lc_id];
    if ((reg_ptr->rlc_data[i].lc_id < TDSUE_MAX_UL_LOGICAL_CHANNEL) &&
        (index != TDSINIT_CTL_BLK_ID) &&
        (index < TDSUE_MAX_UL_RLC_ENTITY))
    {
      TDSRLC_MSG1(MSG_LEGACY_MED,"RLC_WM: Register UL WM,LC %d",reg_ptr->rlc_data[i].lc_id);
      /*
        no way to indicate this sscenario to UL ISR, so wait till ISR completes
      */
      datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(reg_ptr->rlc_data[i].lc_id);
      tdsuplink_table.ctrl_blk[index].ul_wm_ptr = reg_ptr->rlc_data[i].ul_wm_ptr;

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS
      if (reg_ptr->rlc_data[i].lc_id >= TDSRLC_UL_AM_USR_PLANE_START_LC_ID)
      {
#ifdef FEATURE_DATA_PDCP
        traffic_class = pdcp_get_traffic_class(reg_ptr->rlc_data[i].lc_id);
#else
        traffic_class = dswpsd_get_traffic_class(reg_ptr->rlc_data[i].lc_id);
#endif //FEATURE_DATA_PDCP
        TDSRLC_MSG2(MSG_LEGACY_HIGH,"RLC DBG: received traffic-class %d for LC-id %d",traffic_class,reg_ptr->rlc_data[i].lc_id);
        if ((traffic_class != DS_UMTS_QOS_TCLASS_RESERVED) && 
             (traffic_class >= DS_UMTS_QOS_TCLASS_INTR))
        {
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
          tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.dl_cpu_base_fc_f = TRUE;
          if(tdsrlci_dl_fc_status())
          {
            //tdsrlci_dl_fc_tx_new_win_sufi(&tdsuplink_table.ctrl_blk[index]);
            TDSRLC_MSG1(MSG_LEGACY_HIGH,"DL CPU flow control now,LC %d",reg_ptr->rlc_data[i].lc_id);
          }
          tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.ul_cpu_base_fc_f = TRUE;
          if(tdsrlci_ul_fc_status())
          {
            tdsrlci_ul_fc_calc_new_tx_win_size(&tdsuplink_table.ctrl_blk[index]);
            TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL CPU flow control now,LC %d",reg_ptr->rlc_data[i].lc_id);
          }
#endif /*FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */
        }
        else
        {
          TDSRLC_MSG2(MSG_LEGACY_HIGH,"RLC DBG: Invalid traffic-class %d for LC-id %d",traffic_class,reg_ptr->rlc_data[i].lc_id);
        }
      }
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS

      if (tdsuplink_table.ctrl_blk[index].wm_in_rach)
      {
        tdsuplink_table.ctrl_blk[index].ul_wm_ptr->non_empty_func_ptr = tdsrlci_non_empty_func;
        tdsuplink_table.ctrl_blk[index].wm_in_rach = FALSE;
      }

      /* start SDU discard timer if need  */
      tdsrlci_set_sdu_discard_timer(index, reg_ptr->rlc_data[i].lc_id);
      TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"LC %d not config yet",reg_ptr->rlc_data[i].lc_id);
    } 
  }

  if ((tdsrrc_get_state() == TDSRRC_STATE_CELL_PCH) ||
      (tdsrrc_get_state() == TDSRRC_STATE_URA_PCH))
  {
    /*-------------------------------------------------------------------------
      Inform RRC about the LC's registering of WM.
    -------------------------------------------------------------------------*/
    tdsrlc_rrc_req_to_register_for_data_ind(reg_ptr);
  }


  TDSL2_ACQ_DL_UNLOCK();

}/* tdsrlc_ul_register_srvc_handler() */


/*===========================================================================

FUNCTION RLC_UL_DEREGISTER_SRVC_HANDLER

DESCRIPTION
  This function handles the de-registration of the watermark for the uplink
  channel.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_deregister_srvc_handler 
(
  tdsrlc_ul_deregister_srvc_type *de_reg_ptr /* Ptr to WM registration info.     */
)
{
  int i;
  uint16 index;
  rex_crit_sect_type *datapath_mutex = NULL;

  TDSL2_ACQ_DL_LOCK();

  /*-------------------------------------------------------------------------
    Register Uplink Watermark for all Channels indicated in the registration
    pointer.
  -------------------------------------------------------------------------*/
  for (i = 0; i < de_reg_ptr->nchan; i++)
  {
  
    index = tdsul_ctl_blk_id_tbl[de_reg_ptr->rlc_id[i]];
    if ((de_reg_ptr->rlc_id[i] < TDSUE_MAX_UL_LOGICAL_CHANNEL) &&
        (index != TDSINIT_CTL_BLK_ID) &&
        (index < TDSUE_MAX_UL_RLC_ENTITY))
    {
    /*
      Moving loggin part after ul_wm_ptr is actually set to NULL
    */
    #if 0
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"DeRegister UL WM,LC %d",de_reg_ptr->rlc_id[i]);
    #endif
      datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(de_reg_ptr->rlc_id[i]);
      tdsuplink_table.ctrl_blk[index].ul_wm_ptr = NULL;
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
      tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.dl_cpu_base_fc_f = FALSE;
      tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.ul_cpu_base_fc_f = FALSE;
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS

    
      if(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr != NULL)
      {
        (void) rex_clr_timer(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr);
        /* release timer block */
        rex_delete_timer_ex(tdsuplink_table.ctrl_blk[index].tmr_discard_ptr);
        /* Reset timer sig */
        tdsuplink_table.ctrl_blk[index].tmr_discard_ptr = NULL;
        TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
        
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"Release SDU discard timer,LC %d",de_reg_ptr->rlc_id[i]);
      }
      else
      {
        TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
      }

      TDSRLC_MSG1(MSG_LEGACY_HIGH,"DeRegister UL WM,LC %d",de_reg_ptr->rlc_id[i]);
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"LC %d not config yet",de_reg_ptr->rlc_id[i]);
    } 
  }

  TDSL2_ACQ_DL_UNLOCK();
} /* tdsrlc_ul_deregister_srvc_handler() */


/*===========================================================================

FUNCTION RLC_UL_REGISTER_AM_SDU_CNF_CB_REQ

DESCRIPTION
  Register the SDU confirm callback function as request by RRC.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_register_am_sdu_cnf_cb_req
(
  tdsrlc_register_am_sdu_confirm_callback_type *sdu_cnf_cb_req
)
{
  int i;
  uint16 index;
  rex_crit_sect_type *datapath_mutex = NULL;
  //TDSL2_ACQ_UL_DL_LOCK(); 

  for (i = 0; i < sdu_cnf_cb_req->nchan; i++)
  {
    index = tdsul_ctl_blk_id_tbl[sdu_cnf_cb_req->sdu_cnf_cb[i].lc_id];
    if ((sdu_cnf_cb_req->sdu_cnf_cb[i].lc_id < TDSUE_MAX_UL_LOGICAL_CHANNEL) &&
        (index != TDSINIT_CTL_BLK_ID) &&
        (index < TDSUE_MAX_UL_RLC_ENTITY))
    {
      datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(sdu_cnf_cb_req->sdu_cnf_cb[i].lc_id);
      tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.sdu_cnf_cb = 
                            sdu_cnf_cb_req->sdu_cnf_cb[i].rlc_am_sdu_cnf_cb;
      TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);

      TDSRLC_MSG1(MSG_LEGACY_HIGH,"Register UL LC %d SDU Cnf CB",sdu_cnf_cb_req->sdu_cnf_cb[i].lc_id);
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"LC %d not config yet",sdu_cnf_cb_req->sdu_cnf_cb[i].lc_id);
    } 
  }
  
  //TDSL2_ACQ_UL_DL_UNLOCK();
  
}/* tdsrlc_ul_register_am_sdu_cnf_cb_req() */

#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA 
/*===========================================================================

FUNCTION tdsrlc_cal_delay_time_of_status_report

DESCRIPTION
  Set the delay time of status report for each AM channel

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void tdsrlc_cal_delay_time_of_status_report(uint32 ta_end_time)
{
  uint8 ctrl_blk_id;               /* Index into the Uplink Table */
  uint8 i = 0;
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr    = NULL;
  tdsrlci_dl_ctrl_blk_type *ctl_blk_dl_ptr = NULL;
  uint32 delay_time;
  
  /* DL-Mutex-LOCK*/
  TDSL2_ACQ_DL_LOCK();


  for (i = 0; i < TDSUE_MAX_UL_LOGICAL_CHANNEL; i++)
  {
    /*-----------------------------------------------------------------------
     Check if the channel is in NULL state. If it is not, release the 
     channel.  
    -----------------------------------------------------------------------*/
    if ((ctrl_blk_id = tdsul_ctl_blk_id_tbl[i]) != TDSINIT_CTL_BLK_ID &&
        (ctrl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
    {
      ctl_blk_ptr = &tdsuplink_table.ctrl_blk[ctrl_blk_id];

      if(NULL == ctl_blk_ptr)
      {
        continue;
      }

      ctl_blk_dl_ptr = tdsrlci_dl_get_ctl_blk_ptr(ctl_blk_ptr->ctrl.am_ctrl.am_common_local.dl_data_id);
      
      if((ctl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED) &&
         (ctl_blk_ptr->lc_type == UE_LOGCHAN_DTCH))
      {
        TDSRLC_MSG2(MSG_LEGACY_HIGH,"Status_opt_enalbed %d on RLC ID %d", 
                   ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_opt_enalbed,
                   ctl_blk_ptr->ctrl.am_ctrl.am_common_local.ul_data_id);	
      }
      if((ctl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED) &&
         (ctl_blk_ptr->lc_type == UE_LOGCHAN_DTCH) &&
         (ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_opt_enalbed == TRUE))
      {	
				/*status report is already sent before TA end and status prohibit is running, need to reduce the status prohibit timer */
        if((ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_rpt == FALSE) && 
           ((ctl_blk_dl_ptr != NULL) &&
             (ctl_blk_dl_ptr->ctrl.am_ctrl.dl_state_var.status_prohibit)))
        {
          if (ta_end_time >= ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_rpt_time)
          {
            delay_time = ta_end_time - ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_rpt_time;
          }
          else
          {
            delay_time = (ta_end_time - ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_rpt_time + TDSL2_TA_MAX_TIME + 1);
          }
        }
        /*status report is not sent before TA end, NACK PDU will be sent normally */
        else
        {
          delay_time = 0;
          ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_opt_enalbed = FALSE;
        }

        if((ctl_blk_dl_ptr != NULL) &&
           (ctl_blk_dl_ptr->ctrl.am_ctrl.dl_ctl_cnfg->rlc_info.status_info.status_info_incl))
        {
          if(ctl_blk_dl_ptr->ctrl.am_ctrl.dl_ctl_cnfg->rlc_info.status_info.status_prohibit > delay_time)
          {
            ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_prohibit_time_reduce = 
            (ctl_blk_dl_ptr->ctrl.am_ctrl.dl_ctl_cnfg->rlc_info.status_info.status_prohibit - delay_time);
          }
          else
          {
            ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_prohibit_time_reduce = 0;
          }
        }
        
        TDSRLC_MSG3(MSG_LEGACY_HIGH,"The status prohibit time after reduce %d on RLC ID %d, Status_opt_enalbed %d", 
                    ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_prohibit_time_reduce,
                    ctl_blk_ptr->ctrl.am_ctrl.am_common_local.ul_data_id,
                    ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_opt_enalbed);		
      }        
    }
  }
  /* DL-Mutex-UNLOCK*/
  TDSL2_ACQ_DL_UNLOCK();
}/* tdsrlc_cal_delay_time_of_status_report() */

/*===========================================================================

FUNCTION tdsrlc_clear_opt_of_status_report

DESCRIPTION
  Clear the opt flag of status report for each AM channel

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void tdsrlc_clear_opt_of_status_report(void)
{
  uint8 ctrl_blk_id;               /* Index into the Uplink Table */
  uint8 i = 0;
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;
  /* DL-Mutex-LOCK*/
  TDSL2_ACQ_DL_LOCK();

  for (i = 0; i < TDSUE_MAX_UL_LOGICAL_CHANNEL; i++)
  {
    /*-----------------------------------------------------------------------
     Check if the channel is in NULL state. If it is not, release the 
     channel.  
    -----------------------------------------------------------------------*/
    if ((ctrl_blk_id = tdsul_ctl_blk_id_tbl[i]) != TDSINIT_CTL_BLK_ID &&
        (ctrl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
    {
      ctl_blk_ptr = &tdsuplink_table.ctrl_blk[ctrl_blk_id];

      if(NULL == ctl_blk_ptr)
      {
        continue;
      }
      
      if(ctl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED)
      {	
        ctl_blk_ptr->ctrl.am_ctrl.ul_state.status_opt_enalbed = FALSE;
      }        
    }
  }
  /* DL-Mutex-UNLOCK*/
  TDSL2_ACQ_DL_UNLOCK();
}/* tdsrlc_clear_opt_of_status_report() */

/*===========================================================================

FUNCTION TDSRLC_PROCESS_TA_IND

DESCRIPTION
  Processes the LTA Start/Stop received from RRC

===========================================================================*/
void tdsrlci_process_ta_ind(tdsrlc_ta_info_type *ta_info_ptr)
{
  tdsrlc_ta_ind_e_type ta_ind = ta_info_ptr->tune_away_ind;
  tdsrlc_ta_success_e_type ta_succ_ind = ta_info_ptr->tune_away_succ;
  
  if (ta_succ_ind == TDSRLC_FAIL_IND)
  {
    TDSRLC_MSG1(MSG_LEGACY_HIGH,"RLC_TA: TA %d (1:LTA) stop failed", ta_ind);
    tdsl2_ta_opt_info.turn_away_evaluate_start = FALSE;
    tdsl2_ta_opt_info.turn_away_opt_is_valid   = FALSE;
    return;
  }

  switch (ta_ind)
  {
    case TDSRLC_LTA_START_IND:
      tdsrlc_proc_lta_start_ind(ta_succ_ind);
      break;

    case TDSRLC_LTA_STOP_IND:
      tdsrlc_proc_lta_stop_ind(ta_succ_ind);
      break;

    default:
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"RLC_TA: Invalid value %d for TA Ind", ta_ind);
      break;
  }
}

/*===========================================================================

FUNCTION TDSRLC_PROC_LTA_START_IND

DESCRIPTION
  Processes LTA Start Indication received from RRC

===========================================================================*/
void tdsrlc_proc_lta_start_ind(tdsrlc_ta_success_e_type ta_succ_ind)
{
  if (ta_succ_ind != TDSRLC_TA_SUCC_MAX_ENUM)
    return;

  if(tdsl2_ta_opt_info.turn_away_evaluate_start == TRUE)
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"RLC_ERR: RLC TA evaluate is already started");
  }
  TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_TA: LTA Start Indication received from RRC");

  tdsrlc_clear_opt_of_status_report();

  tdsl2_ta_opt_info.ta_start_time = tdsrlc_get_system_time();
  tdsl2_ta_opt_info.turn_away_evaluate_start = TRUE;
  tdsl2_ta_opt_info.turn_away_opt_is_valid   = FALSE;
}

/*===========================================================================

FUNCTION RLC_PROC_LTA_STOP_IND

DESCRIPTION
  Processes LTA Stop Indication received from RRC

===========================================================================*/
void tdsrlc_proc_lta_stop_ind(tdsrlc_ta_success_e_type ta_succ_ind)
{
  if (ta_succ_ind != TDSRLC_SUCC_IND)
  {
    tdsl2_ta_opt_info.turn_away_evaluate_start = FALSE;
    tdsl2_ta_opt_info.turn_away_opt_is_valid   = FALSE;
    return;
  }

  tdsl2_ta_opt_info.ta_end_time = tdsrlc_get_system_time();
  if(tdsl2_ta_opt_info.turn_away_evaluate_start == TRUE)
  {
    if (tdsl2_ta_opt_info.ta_end_time >= tdsl2_ta_opt_info.ta_start_time)
    {
      tdsl2_ta_opt_info.ta_length = tdsl2_ta_opt_info.ta_end_time - tdsl2_ta_opt_info.ta_start_time;
    }
    else
    {
      tdsl2_ta_opt_info.ta_length  = (tdsl2_ta_opt_info.ta_end_time - tdsl2_ta_opt_info.ta_start_time + TDSL2_TA_MAX_TIME + 1);
    }
    tdsl2_ta_opt_info.turn_away_opt_is_valid = (tdsl2_ta_opt_info.ta_length  > TDSL2_TA_OPT_THRESHOLD) ? TRUE : FALSE;
    /*1. Get the delay time of status report for each AM logical channel
         2.  Send T1 time opt signal to DL HS task */
    if(tdsl2_ta_opt_info.turn_away_opt_is_valid == TRUE)
    {
      tdsrlc_cal_delay_time_of_status_report(tdsl2_ta_opt_info.ta_end_time);
      
      tdsmac_send_t1_opt_signal();
    }
  }
  tdsl2_ta_opt_info.turn_away_evaluate_start = FALSE;
  TDSRLC_MSG2(MSG_LEGACY_HIGH,"RLC_TA: LTA Stop Success Indication received from RRC, TA OPT %d, TA length %dms", 
             tdsl2_ta_opt_info.turn_away_opt_is_valid,
             tdsl2_ta_opt_info.ta_length);
}
/*===========================================================================

FUNCTION TDSRLC_GET_SYSTEM_TIME

DESCRIPTION
  Get system time

===========================================================================*/
uint32 tdsrlc_get_system_time(void)
{
  time_type up_ms;
  uint32 lo32;
  time_get_uptime_ms(up_ms);
  lo32 = qw_lo(up_ms);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRLC:System time %d", lo32);
  return lo32;
}
#endif/*FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA */
      
/*===========================================================================

FUNCTION RLC_UL_ENH_BUFFER_STATUS

DESCRIPTION
  Gets the buffer status of requested uplink logical channels by MAC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_enh_buffer_status
(
  tdsrlc_ul_lc_info_type    *lc_info_ptr,  /* Report buffer status for */
                                        /* only following channels  */
  tdsrlc_ul_buf_status_type *status_ptr    /* Pointer to UL Buf.Status */
)
{
  uint8               report_chan_idx;  /* Number of active channels.*/
  uint8               ctl_blk_id;       /* Control Blk ID            */
  rlc_lc_id_type      rlc_id = 0;

  /*-------------------------------------------------------------------------
    for each UM and TM active logical channel in the uplink table,
    get the buffer status. If the WM is being purged then return a
    zero buffer occupancy on the channel. For AM, block only new 
    data and allow re-transmissions and status.
  -------------------------------------------------------------------------*/
  for(report_chan_idx = 0; report_chan_idx < lc_info_ptr->nchan; report_chan_idx++)
  {
    rlc_id = lc_info_ptr->rlc_id[report_chan_idx];

    /*-------------------------------------------------------------------------
      Reset status memory for this LC
    -------------------------------------------------------------------------*/
    (void)memset(&(status_ptr->chan_info[rlc_id]), 0, sizeof(tdsrlc_ul_lc_buf_status_type));

    if (rlc_id >= TDSUE_MAX_UL_LOGICAL_CHANNEL)
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: RLC LC ID out of Range,%d",rlc_id);
      continue;
    }
    ctl_blk_id = tdsul_ctl_blk_id_tbl[rlc_id];

    if (ctl_blk_id != TDSINIT_CTL_BLK_ID &&
        (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY) &&
        tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state != TDSUL_NULL_STATE)
    {
      if ((tdsuplink_table.ctrl_blk[ctl_blk_id].ul_wm_ptr == NULL)  && 
          (UE_MODE_ACKNOWLEDGED != tdsuplink_table.ctrl_blk[ctl_blk_id].lc_mode))
      {
        TDSRLC_MSG1(MSG_LEGACY_LOW,"Buf Status:UL LC %d WM unregistered",rlc_id);

        if (tdsuplink_table.ctrl_blk[ctl_blk_id].lc_mode == UE_MODE_TRANSPARENT)
        {
          status_ptr->chan_info[rlc_id].npdus = 0;
          status_ptr->chan_info[rlc_id].type = TDSFIXED_PDUS;
        }
        else
        {
          status_ptr->chan_info[rlc_id].nbytes = 0;
          status_ptr->chan_info[rlc_id].type = TDSFLEX_PDUS;
        }        
      }
      else // WM is registered
      {
        switch(tdsuplink_table.ctrl_blk[ctl_blk_id].lc_mode)
        {
          case UE_MODE_TRANSPARENT:
            if((tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state == TDSUL_DATA_TRANSFER_READY) &&
               (tdsuplink_table.ctrl_blk[ctl_blk_id].purge_wm == FALSE))
            {
              tdsrlci_get_tm_buffer_status(&(tdsuplink_table.ctrl_blk[ctl_blk_id]),
                                        &(status_ptr->chan_info[rlc_id]));
            }
            else
            {
              if(tdsuplink_table.ctrl_blk[ctl_blk_id].purge_wm == TRUE)
              {
                TDSRLC_MSG2(MSG_LEGACY_HIGH,"Purging UL WM for LC %d,ctl_blk_id %d",rlc_id,ctl_blk_id);
              }
              else
              {
                TDSRLC_MSG2(MSG_LEGACY_HIGH,"Buf Status:UL LC %d in %x state",rlc_id,tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state);
              }
              status_ptr->chan_info[rlc_id].npdus = 0;
            }
            status_ptr->chan_info[rlc_id].type = TDSFIXED_PDUS;
            break;
            
          case UE_MODE_UNACKNOWLEDGED:
            tdsrlci_get_um_buffer_status(&(tdsuplink_table.ctrl_blk[ctl_blk_id]),
                                      &(status_ptr->chan_info[rlc_id]));

            /* We are in the data_transfer_ready state or   */
            /* Waiting_to _suspend state.                   */
            if((tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state &
                (TDSUL_DATA_TRANSFER_READY | TDSUL_WAITING_TO_SUSPEND)) &&
               (tdsuplink_table.ctrl_blk[ctl_blk_id].purge_wm == FALSE))
            {
              status_ptr->chan_info[rlc_id].suspend_pdu_build_f = FALSE;
            }
            else
            {
              if(tdsuplink_table.ctrl_blk[ctl_blk_id].purge_wm == TRUE)
              {
                TDSRLC_MSG2(MSG_LEGACY_HIGH,"Purging UL WM for LC %d,ctl_blk_id %d",rlc_id,ctl_blk_id);
              }
              else
              {
                TDSRLC_MSG2(MSG_LEGACY_HIGH,"Buf Status:UL LC %d in %x state",rlc_id,tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state);
              }
              status_ptr->chan_info[rlc_id].suspend_pdu_build_f = TRUE;
            }
            status_ptr->chan_info[rlc_id].type = TDSFLEX_PDUS;

            if (tdsrlc_debug_var & 0x00000020)
            {
              TDSRLC_MSG3(MSG_LEGACY_HIGH,"RLC DBG: UM Buf Status:UL LC %d new_data_bytes %d WM_cnt %d", rlc_id,
                         status_ptr->chan_info[rlc_id].new_data_bytes, 
                         tdsuplink_table.ctrl_blk[ctl_blk_id].ul_wm_ptr->current_cnt);
            }
            break;

          case UE_MODE_ACKNOWLEDGED:
            /*---------------------------------------------------------------
              The entity is not in the states of Config_pending state,
              Suspended state or Stop state where there is no data
              allowed to be sent.
            ----------------------------------------------------------------*/
            /*lint -save -e655 */
            if (!(tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state &
                                  (TDSUL_CONFIG_PENDING | TDSUL_STOP)) &&
                !tdsuplink_table.ctrl_blk[ctl_blk_id].purge_wm)
            /*lint -restore */
            {
              status_ptr->chan_info[rlc_id].suspend_pdu_build_f = FALSE;
            }
            else
            {
              TDSRLC_MSG2(MSG_LEGACY_MED,"Buf Status:UL LC %d in %x state",rlc_id,tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state);
              status_ptr->chan_info[rlc_id].suspend_pdu_build_f = TRUE;
            }

            tdsrlci_enh_get_am_buffer_status(&(tdsuplink_table.ctrl_blk[ctl_blk_id]),
                                          &(status_ptr->chan_info[rlc_id]));

            TDSRLC_MSG3(MSG_LEGACY_MED,"Buf Status:UL LC %d, ctl %d win_size %d", rlc_id,
                     status_ptr->chan_info[rlc_id].ctrl_data_bytes,
                     status_ptr->chan_info[rlc_id].effect_win_size);
            TDSRLC_MSG3(MSG_LEGACY_MED,"Buf Status cont:new data %d, retx %d, suspend_flag %d", 
                     status_ptr->chan_info[rlc_id].new_data_bytes,
                     status_ptr->chan_info[rlc_id].retx_data_bytes,
                     status_ptr->chan_info[rlc_id].suspend_pdu_build_f);
            
            status_ptr->chan_info[rlc_id].type = TDSFLEX_PDUS;
            break;

          default:
            TDSRLC_MSG0(MSG_LEGACY_HIGH,"Buf Status:Invalid Mode");
        }/* switch */
      }
    }
    else
    {
      /* Make sure these fields get reset */
      status_ptr->chan_info[rlc_id].npdus           = 0;
      status_ptr->chan_info[rlc_id].nbytes          = 0;
      status_ptr->chan_info[rlc_id].new_data_bytes  = 0;
      status_ptr->chan_info[rlc_id].ctrl_data_bytes = 0;
      status_ptr->chan_info[rlc_id].retx_data_bytes = 0;
      status_ptr->chan_info[rlc_id].effect_win_size = 0;
    }
  } /* for */

}/* tdsrlc_ul_enh_buffer_status */

/*===========================================================================

FUNCTION RLCI_NON_EMPTY_FUNC()

DESCRIPTION
  This callback function resets the callback function non_empty_func_ptr
  for all uplink queues (water-mark) and triggers MAC with 
  tdsmac_ul_trigger_tx_req() to begin transmission.  
  This callback function will be called when the watermark queue detects
  data being enqueued. 
  This is used for Transmit Processing on RACH.  
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_non_empty_func
(
    dsm_watermark_type* wm_ptr,
    void*               func_data

)
{

  uint16 count;                  /* Number of active channels               */
  int    i = 0;

  count = tdsuplink_table.active_ch;
  /*--------------------------------------------------------------------- 
    De-register the non_empty_func_ptr for each of the active channels
    and trigger MAC to start Processing.
  ---------------------------------------------------------------------*/
  while (count > 0 && i < TDSUE_MAX_UL_RLC_ENTITY)    
  {      
    if (tdsuplink_table.ctrl_blk[i].lc_state != TDSUL_NULL_STATE) 
    {
      count--;
      if (tdsuplink_table.ctrl_blk[i].ul_wm_ptr != NULL)
      {
        tdsuplink_table.ctrl_blk[i].ul_wm_ptr->non_empty_func_ptr = NULL;
      }
    }
    i++;   
  }
  TDSRLC_MSG0(MSG_LEGACY_HIGH,"Triggle MAC Tx in Rach");

}/* tdsrlci_non_empty_func() */


/*===========================================================================

FUNCTION RLC_UL_BUILD_PDUS

DESCRIPTION
  Builds uplink PDUs for transmission in a TTI. 
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_build_pdus 
(
  tdsrlc_ul_frame_format_type *frame_ptr,
  tdsrlc_ul_frame_data_type *tdsdata_ptr
)
{
  int i;
  uint16 index;

  uint16    num_pdus = 0;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  uint8 log_index = 0;
  boolean found = FALSE;

  /*-------------------------------------------------------------------------
    This array stores the logging parameter info, the type of PDU packet
    user plane or signalling plane and no. of active channels of each
    type of packet.
  -------------------------------------------------------------------------*/
  uint16 k=0;
  tdslogging_params[TDSRLCLOG_SIG_PDU][TDSRLCLOG_TYPE_IDX] = 
                                    TDSCDMA_UL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET;
  tdslogging_params[TDSRLCLOG_USR_PDU][TDSRLCLOG_TYPE_IDX] = 
                                    TDSCDMA_UL_RLC_AM_USR_PLANE_PDU_LOG_PACKET;
  tdslogging_params[TDSRLCLOG_SIG_PDU][TDSRLCLOG_CH_IDX] = 0;
  tdslogging_params[TDSRLCLOG_USR_PDU][TDSRLCLOG_CH_IDX] = 0;
#endif

  tdsdata_ptr->nchan = 0;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  tdsrlci_ul_pdu_cipher_log_on = log_status(TDSCDMA_UL_RLC_PDU_CIPHER_LOG_PACKET);
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

    /*------------------------------------------------------------------------
      Free UM SDUs transmitted in the previous TTI and initialize the queue.
    -------------------------------------------------------------------------*/
    tdsrlc_um_free_sdu_q(TDSRLC_NON_E_DCH_CHNL);

    /*-----------------------------------------------------------------------
      Intialize the R99 pre-allocated PDU dsm pool
    -----------------------------------------------------------------------*/
    tdsrlc_pdu_dsm_pool.num_item = 0;

  for (i = 0; i < frame_ptr->nchan; i++)
  {
    /*-----------------------------------------------------------------------
      If the Logical Channel ID exceeds the maximum allowed channel ID,
      move to the next channel.
    -----------------------------------------------------------------------*/            
    if (frame_ptr->chan_fmt[i].rlc_id >= TDSUE_MAX_UL_LOGICAL_CHANNEL) 
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"Bld UL PDUS:Invalid LC ID %d",frame_ptr->chan_fmt[i].rlc_id);
      continue;
    }
    /*-----------------------------------------------------------------------
      Get the index into the table.
    -----------------------------------------------------------------------*/
    index = tdsul_ctl_blk_id_tbl[frame_ptr->chan_fmt[i].rlc_id];
    if (index != TDSINIT_CTL_BLK_ID && 
       (index < TDSUE_MAX_UL_RLC_ENTITY) &&
        tdsuplink_table.ctrl_blk[index].lc_state != TDSUL_NULL_STATE)
    {
      if ((tdsuplink_table.ctrl_blk[index].ul_wm_ptr == NULL) &&
	 (tdsuplink_table.ctrl_blk[index].lc_mode != UE_MODE_ACKNOWLEDGED))
      {
        TDSRLC_MSG1(MSG_LEGACY_LOW,"Bld UL PDUs:LC %d WM unregistered",frame_ptr->chan_fmt[i].rlc_id);
        tdsdata_ptr->chan_info[tdsdata_ptr->nchan].npdus = 0;
        tdsdata_ptr->chan_info[tdsdata_ptr->nchan].pdu_size = 0;
      }
      else
      {
        switch (tdsuplink_table.ctrl_blk[index].lc_mode)
        {
          case UE_MODE_TRANSPARENT:
            /*---------------------------------------------------------------
              Build the PDU and update the structure.
            ---------------------------------------------------------------*/
            if (tdsrlc_debug_var & TDSRLC_DBG_UL_TM)
            {
              TDSRLC_MSG3(MSG_LEGACY_HIGH,"UL TM LC %d, Num PDUs %d, PDU size %d",
                     frame_ptr->chan_fmt[i].rlc_id, frame_ptr->chan_fmt[i].npdus,
                     frame_ptr->chan_fmt[i].pdu_size);
            }
            tdsrlci_ul_build_tm_pdus(&(tdsuplink_table.ctrl_blk[index]), 
                                  &frame_ptr->chan_fmt[i], 
                                  &tdsdata_ptr->chan_info[tdsdata_ptr->nchan]);
            break;
          case UE_MODE_UNACKNOWLEDGED:

            num_pdus = 
              tdsrlc_ul_enh_build_non_edch_um_pdus(&(tdsuplink_table.ctrl_blk[index]),
                                                &frame_ptr->chan_fmt[i],
                                                &tdsdata_ptr->chan_info[tdsdata_ptr->nchan]);

            tdsdata_ptr->chan_info[tdsdata_ptr->nchan].npdus = num_pdus;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
            if(!(tdsuplink_table.ctrl_blk[index].ul_pdu_logging) &&
               log_status(TDSCDMA_UL_RLC_UM_USR_PLANE_PDU_LOG_PACKET))
            {
              found = TRUE;
              tdsrlc_ul_channel_cnt ++;

              tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.ul_chan_um_pdu.pdu_size=
                                        frame_ptr->chan_fmt[i].pdu_size;
              for(k=0;
                  k < tdsdata_ptr->chan_info[tdsdata_ptr->nchan].npdus &&
                  k < TDSMAX_UL_RLC_LOG_PDUS; k++)
              {
                tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.ul_chan_um_pdu.log_data[k] =
                  tdsrlc_copy_to_word(tdsdata_ptr->chan_info[tdsdata_ptr->nchan].pdu_info[k].pdu_ptr->data_ptr);
              }
              tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.ul_chan_um_pdu.num_pdu = k;
            }
#endif //FEATURE_TDSCDMA_UM_PDU_LOGGING
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

            /*---------------------------------------------------------------
              If traffic is sent on user plane channels, set the activity
              flag to TDSDATA_TRANSFERRED.
            ---------------------------------------------------------------*/
            TDSRLC_SET_DATA_ACTIVITY_FLAG(frame_ptr->chan_fmt[i].rlc_id, 
                            TDSRLC_UL_UM_USR_PLANE_START_LC_ID);


            break;

          case UE_MODE_ACKNOWLEDGED:
            /*---------------------------------------------------------------
             If data PDUs are being built or going to be built 
             on UL AM user plane channels, set the activity flag to 
             TDSDATA_TRANSFERRED.
            ---------------------------------------------------------------*/
            if(tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_s !=
                    tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_a)
            {
              TDSRLC_SET_DATA_ACTIVITY_FLAG(frame_ptr->chan_fmt[i].rlc_id, 
                    TDSRLC_UL_AM_USR_PLANE_START_LC_ID);
            }

            num_pdus = 
              tdsrlc_ul_enh_build_non_edch_am_pdus(&(tdsuplink_table.ctrl_blk[index]),
                                                &frame_ptr->chan_fmt[i],
                                                &tdsdata_ptr->chan_info[tdsdata_ptr->nchan]);

            tdsdata_ptr->chan_info[tdsdata_ptr->nchan].npdus = num_pdus;


#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING

            if(!(tdsuplink_table.ctrl_blk[index].ul_pdu_logging))
            {
              /*---------------------------------------------------------------
              If LC ID is for user plane AM channels, store the info if
              user plane PDU logging is turned on, otherwise store the info
              if the signaling plane PDU logging is turned on.
              ---------------------------------------------------------------*/
              if((frame_ptr->chan_fmt[i].rlc_id >= TDSRLC_UL_AM_USR_PLANE_START_LC_ID) && 
                        log_status(TDSCDMA_UL_RLC_AM_USR_PLANE_PDU_LOG_PACKET))
              {
                log_index = 1;
                tdslogging_params[log_index][TDSRLCLOG_CH_IDX] ++;
              }
              if((frame_ptr->chan_fmt[i].rlc_id < TDSRLC_UL_AM_USR_PLANE_START_LC_ID) && 
                        log_status(TDSCDMA_UL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET))
              {
                log_index = 0;
                tdslogging_params[log_index][TDSRLCLOG_CH_IDX] ++;
              }

              if((log_status(tdslogging_params[log_index][TDSRLCLOG_TYPE_IDX])) && 
                          (tdslogging_params[log_index][TDSRLCLOG_CH_IDX] > 0))
              {
                found = TRUE;
                /*-------------------------------------------------------------
                Store the number of pdus, pdu size, 4bytes of the pdu, data id, 
                control id in ul_am_pdu_log for purpose of qcat logging.
                -------------------------------------------------------------*/
                tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_chan_am_pdu.pdu_size = 
                                             frame_ptr->chan_fmt[i].pdu_size;

                /*-------------------------------------------------------------
                  Perform PDU logging after the PDU building. No need to check 
                  the UE PDU cap again as it should have done in the PDU building
                -------------------------------------------------------------*/
                for(k=0; 
                    k < tdsdata_ptr->chan_info[tdsdata_ptr->nchan].npdus &&  
                    k < TDSMAX_UL_RLC_LOG_PDUS;
                    k++)
                {
                  tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_chan_am_pdu.log_data[k] = 
                    tdsrlc_copy_to_word(tdsdata_ptr->chan_info[tdsdata_ptr->nchan].pdu_info[k].pdu_ptr->data_ptr);
                }
                tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_chan_am_pdu.num_pdu = k;
              }
            }
            tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_chan_am_stat.tot_num_pdu_byte_txd +=
                                           ((frame_ptr->chan_fmt[i].pdu_size >> 3)* 
                                           tdsdata_ptr->chan_info[tdsdata_ptr->nchan].npdus);

			
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING
#ifdef FEATURE_QSH_EVENT_METRIC
				//Incrementing the QSH Number of PDUs TX for this cycle.
				tdsrlc_qsh_metrics.num_pdu_bytes_tx += ((frame_ptr->chan_fmt[i].pdu_size >> 3)* tdsdata_ptr->chan_info[tdsdata_ptr->nchan].npdus);
				TDSRLC_MSG1(MSG_LEGACY_MED,"TDS_QSH: Incrementing the PDU built count by %d bytes  ",((frame_ptr->chan_fmt[i].pdu_size >> 3)* tdsdata_ptr->chan_info[tdsdata_ptr->nchan].npdus)); 
#endif


            if (tdsrlc_debug_var & TDSRLC_DBG_PDU_BLDG)
            {
              TDSRLC_MSG3(MSG_LEGACY_HIGH,"RLC variables, VT_S:%x,VT_MS:%x,VT_A:%x",
               tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_s,
               tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_ms,
               tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_a);
            }
            break;
          default:
            TDSRLC_MSG0(MSG_LEGACY_HIGH,"Build UL PDUs:Invalid Mode ");  
        }
      }
      tdsdata_ptr->chan_info[tdsdata_ptr->nchan].rlc_id = frame_ptr->chan_fmt[i].rlc_id;
      tdsdata_ptr->nchan++;
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"Build UL PDUs:LC %d does not exist",frame_ptr->chan_fmt[i].rlc_id);
    }
  } /* for */

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING

  if (found || tdsrlci_ul_pdu_cipher_log.num_pdu)
  {
    (void) rex_set_sigs( tds_l2_ul_get_tcb(), TDSRLC_UL_START_LOGGING_SIG);
  }
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING


}/* tdsrlc_ul_build_pdus() */


/*===========================================================================

FUNCTION RLCI_GET_CTL_BLK_PTR

DESCRIPTION
  Builds uplink PDUs for transmission in a TTI. 
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
tdsrlci_ul_ctrl_blk_type *tdsrlci_get_ctl_blk_ptr 
(
  uint8 ch_id
)
{
  uint8 ctl_blk_id;
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;

  if (ch_id < TDSUE_MAX_UL_LOGICAL_CHANNEL &&
      (ctl_blk_id = tdsul_ctl_blk_id_tbl[ch_id]) != TDSINIT_CTL_BLK_ID &&
      (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
  {
    ctl_blk_ptr = &tdsuplink_table.ctrl_blk[ctl_blk_id];
  }

  return (ctl_blk_ptr);

}/* tdsrlci_get_ctl_blk_ptr() */
/*===========================================================================

FUNCTION TDSRLCI_GET_CTL_BLK_PTR

DESCRIPTION
  Returns Ctrl Pointer, if the rb_id is active and LC Type of AM
  Otherwise return NULL
  
DEPENDENCIES
  None.
  
RETURN VALUE
  AM - Control Blk pointer
  Non-AM - NULL
  
SIDE EFFECTS
  None.

===========================================================================*/
tdsrlci_ul_ctrl_blk_type *tdsrlci_get_ctl_blk_ptr_am_rb_id(uint8 rb_id)
{
  int32 ctl_blk_id;
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;

  for (ctl_blk_id = 0; ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY; ctl_blk_id++)
  {
    if ((tdsuplink_table.ctrl_blk[ctl_blk_id].lc_state != TDSUL_NULL_STATE) && 
        ((tdsuplink_table.ctrl_blk[ctl_blk_id].lc_mode >= UE_MODE_ACKNOWLEDGED) && 
         (tdsuplink_table.ctrl_blk[ctl_blk_id].lc_mode <= UE_MODE_ACKNOWLEDGED_CONTROL)) &&
        (tdsuplink_table.ctrl_blk[ctl_blk_id].ctrl.am_ctrl.am_common->rb_id == rb_id))
    {
      ctl_blk_ptr = &(tdsuplink_table.ctrl_blk[ctl_blk_id]);
    }
  }
  
  return (ctl_blk_ptr);

}/* rlci_get_ctl_blk_ptr() */
/*===========================================================================

FUNCTION TDSRLCi_CHK_UL_RESET_PENDING

DESCRIPTION
  This function checks if the UL is in RESET pending state.

DEPENDENCIES
  None

RETURN VALUE
  Boolean indicating, RESET pending or not.

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrlci_chk_ul_reset_pending
(
  rlc_lc_id_type lc_id
)
{
  tdsrlci_ul_ctrl_blk_type *ul_ctrl_blk_ptr;
  boolean               ret_val = FALSE;
  rex_crit_sect_type *datapath_mutex = NULL;
  datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(lc_id);
  ul_ctrl_blk_ptr = tdsrlci_get_ctl_blk_ptr(lc_id);
  
  /*lint -save -e655 */
  if ((ul_ctrl_blk_ptr != NULL) && 
      (ul_ctrl_blk_ptr->lc_state & (tdsrlci_ul_state_e_type)TDSUL_RESET_PENDING))
  {
    ret_val = TRUE;
  }
  /*lint -restore */
  TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  return ret_val;
} /* tdsrlci_chk_ul_reset_pending */

/*===========================================================================

FUNCTION RLCI_SET_UL_NUM_SN_LI

DESCRIPTION
  Updates ul_num_sn_li, which indicates the number of SN-Li
  pairs which will formed on UL. This API is used to copy number
  SN-LI pairs calculated in DL to UL. 
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_set_ul_num_sn_li 
(
  uint8   lc_id,
  uint16  num_sn_li
)
{
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;

  ctl_blk_ptr = tdsrlci_get_ctl_blk_ptr(lc_id);

  if(NULL == ctl_blk_ptr)
  {
    return;
  }
  ctl_blk_ptr->ctrl.am_ctrl.ul_num_sn_li = num_sn_li;

  return;

}/* tdsrlci_get_ctl_blk_ptr() */


/*===========================================================================

FUNCTION RLCI_DL_CONFIG_CNF_HANDLER

DESCRIPTION
  Handle the config confirmation from the Downlink side. This is for AM only.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_dl_config_cnf_handler
(
  tdsrlci_dl_config_cnf_type *cnf_ptr
)
{
  int i;
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr;
  tdsrrc_rlc_cmd_type *rrc_rlc_cmd_ptr;
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  uint8 j = 0;
#endif  //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

  rrc_rlc_cmd_ptr = tdsrrc_get_rlc_cmd_buf();
  if (rrc_rlc_cmd_ptr == NULL)
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"Could not get cmd buffer");
    return;
  }
  rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CRLC_AM_CONFIG_CNF;
  rrc_rlc_cmd_ptr->cmd.crlc_am_config_cnf.nchan = cnf_ptr->nchan;

  TDSL2_ACQ_DL_LOCK();
  /*Locking both EUL and R99 Mutex*/
  (void)TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);

  for (i=0; i<cnf_ptr->nchan; i++)
  {
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
    if (tdsrlc_debug_var & TDSRLC_DBG_CORRUPTION)
    { 
      TDSRLC_MSG3(MSG_LEGACY_HIGH,"Padding_DBG: rb_id %d, UL lc_id %d, action %d",
           cnf_ptr->am_cnf[i].rb_id, cnf_ptr->am_cnf[i].ul_data_id, 
           cnf_ptr->am_cnf[i].action);
    }
#endif

    if (cnf_ptr->am_cnf[i].result == SUCCESS)
    {
      if (
          ((cnf_ptr->am_cnf[i].action == TDSRLCI_ESTABLISH) ||
          (cnf_ptr->am_cnf[i].action == TDSRLCI_RE_ESTABLISH) ||
          (cnf_ptr->am_cnf[i].action == TDSRLCI_MODIFY))
          )
      {
        if((ctl_blk_ptr = tdsrlci_get_ctl_blk_ptr(cnf_ptr->am_cnf[i].ul_data_id)) == NULL)
        {
          TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC-UL LC Id:%d not found",cnf_ptr->am_cnf[i].ul_data_id);
          continue;
        }

        if((cnf_ptr->am_cnf[i].rb_id == DCCH_AM_RADIO_BEARER_ID)
		   || (cnf_ptr->am_cnf[i].rb_id == DCCH_DT_HIGH_PRI_RADIO_BEARER_ID)
		   || (cnf_ptr->am_cnf[i].rb_id == DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
        {
#ifndef FEATURE_TDSCDMA_SRB_RESET
           tdsam_ignore_reset_table[cnf_ptr->am_cnf[i].rb_id] = TRUE;
        }
#else
          tdsam_ignore_reset_num_table[cnf_ptr->am_cnf[i].rb_id] = 0;
        }
        else
        {
          tdsam_ignore_reset_num_table[cnf_ptr->am_cnf[i].rb_id] = TDS_MAX_IGNORE_RESET_INVALID_NUM;
        }
#endif

        if (
            cnf_ptr->am_cnf[i].action == TDSRLCI_ESTABLISH
            )
        {
          //TDSL2_ACQ_UL_LW_LOCK();
          /*--------------------------------------------------------------------- 
            Go to TDSUL_DATA_TRANSFER_READY.
          ----------------------------------------------------------------------*/
          ctl_blk_ptr->lc_state = TDSUL_DATA_TRANSFER_READY;
          //TDSL2_ACQ_UL_LW_UNLOCK();
          
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
          tdsul_log_state.ul_state[j].rlc_id = cnf_ptr->am_cnf[i].ul_data_id;
          tdsul_log_state.ul_state[j].lc_state = TDSAM_UL_DATA_TRANSFER_READY;
          j++;
#endif  //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

          /*--------------------------------------------------------------------- 
            Delay setting up timer until the RLC sends out its first PDU.
          ----------------------------------------------------------------------*/
          ctl_blk_ptr->ctrl.am_ctrl.ul_state.fst_tx_pdu = TRUE;
          ctl_blk_ptr->ctrl.am_ctrl.ul_tmr_sts = TDSRLC_AM_TIMER_UNDEF_STATUS;
          
#if defined (FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL)
          if(cnf_ptr->am_cnf[i].ul_data_id >= TDSRLC_UL_AM_USR_PLANE_START_LC_ID)
          {
            tdsrlc_acti_non_srb_am_ch++;
          }
#endif /* (FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */
        }
        /* action is TDSRLC_RE_ESTABLISH */
        else if (
                    cnf_ptr->am_cnf[i].action == TDSRLCI_RE_ESTABLISH
                )
        {
        /* 
          Wait till UL ISR completes
        */
          //TDSL2_ACQ_UL_LW_LOCK();
          /*--------------------------------------------------------------------- 
            Remove TDSUL_CONFIG_PENDING state and go back to the original states.
          ----------------------------------------------------------------------*/
          /*lint -save -e64 -e641 */
          ctl_blk_ptr->lc_state &= (~TDSUL_CONFIG_PENDING);
          /*lint -restore */
          //TDSL2_ACQ_UL_LW_UNLOCK();
          
          /*-----------------------------------------------------------------
            If re-establishment occured during
            1. UE initiated reset, start a new reset procedure
            2. N/W initiated reset, abort the reset procedure
          -----------------------------------------------------------------*/
          /*lint -save -e655 */ 
          if (ctl_blk_ptr->lc_state & TDSUL_RESET_PENDING)
          /*lint -restore */             
          {
            if (ctl_blk_ptr->ctrl.am_ctrl.ul_state.need_reset_ack || 
                ctl_blk_ptr->ctrl.am_ctrl.ul_state.owe_reset)
            {
              /*-------------------------------------------------------------
                Turn on this flag for the next TTI to tx RESET PDU.
              -------------------------------------------------------------*/
              ctl_blk_ptr->ctrl.am_ctrl.ul_state.owe_reset = TRUE;

              /*-------------------------------------------------------------
                Turn on RESET Timer.
              -------------------------------------------------------------*/
              ctl_blk_ptr->ctrl.am_ctrl.ul_state.need_reset_ack = TRUE;
            }
            else
            {
              /*
                Wait till UL ISR completes
              */
              //TDSL2_ACQ_UL_LW_LOCK();
              /*-------------------------------------------------------------
                This is a case of one side re-establishment(DL), where a 
                NW initiated RESET procedure is terminated due to DL 
                re-establishment. Since UE UL has to come out of RESET_PENDING
                state without sending a RESET_ACK update count_c here..
              -------------------------------------------------------------*/
              ctl_blk_ptr->cipher.count = (ctl_blk_ptr->cipher.hfn << 12);
              TDSRLC_MSG3(MSG_LEGACY_HIGH,"Aborting NW initiated RESET count_c %d, vt_s %d, DL_state(1:rst_pndg) %d",
                 ctl_blk_ptr->cipher.count,ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_s,
                 tdsrlci_chk_dl_reset_pending(ctl_blk_ptr->ctrl.am_ctrl.am_common->dl_data_id));
              /*lint -save -e64 -e641 */
              ctl_blk_ptr->lc_state &= (~TDSUL_RESET_PENDING);
              /*lint -restore */
              //TDSL2_ACQ_UL_LW_UNLOCK();

              tdsrlci_dl_change_state              (ctl_blk_ptr->ctrl.am_ctrl.am_common->dl_data_id);
            }
          }

          if((cnf_ptr->am_cnf[i].rb_id == DCCH_AM_RADIO_BEARER_ID)
	            && (tdsrlci_inter_ho_happen == TRUE))
          {
            Bitmap_specail_handle.inter_rnc_ho_start = TRUE;
	          Bitmap_specail_handle.inter_rnc_ho_end   = FALSE;
	          TDSRLC_MSG2(MSG_LEGACY_HIGH,"BITMAP SH: rb_id %d,action %d,inter_rnc_ho_start",cnf_ptr->am_cnf[i].rb_id,cnf_ptr->am_cnf[i].action);						
          }
        }
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
        /* If RLC FC is active, flow control new rb. */
        if(cnf_ptr->am_cnf[i].ul_data_id >= TDSRLC_UL_AM_USR_PLANE_START_LC_ID)
        {

         tdsmulti_client_fc_modify_max_flow(&(ctl_blk_ptr->ctrl.am_ctrl.multiclient_dl_fc_arbitarator),
                                          tdsrlci_get_rx_win_size(ctl_blk_ptr->ctrl.am_ctrl.am_common->dl_data_id),
                                          FALSE);
         ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins = tdsrlci_dl_multi_client_fc_get_win_size(&(ctl_blk_ptr->ctrl.am_ctrl));
         ctl_blk_ptr->ctrl.am_ctrl.ul_state.rlc_fc_ws.rlc_fc_ws =
            ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins;

          if(tdsrlci_dl_fc_status()
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS
             && ctl_blk_ptr->ctrl.am_ctrl.ul_state.dl_cpu_base_fc_f
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS
            )
          {
            tdsrlci_dl_fc_tx_new_win_sufi(ctl_blk_ptr);
          }
          ctl_blk_ptr->ctrl.am_ctrl.ul_state.rlc_fc_ws.rlc_fc_ws =
          ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins;

          if(tdsrlci_ul_fc_status()
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS
             && ctl_blk_ptr->ctrl.am_ctrl.ul_state.ul_cpu_base_fc_f
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS
             )
          {
            tdsrlci_ul_fc_calc_new_tx_win_size(ctl_blk_ptr);
          }
        }
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
      }
      TDSRLC_MSG1(MSG_LEGACY_MED,"UL AM LC %d config complete",cnf_ptr->am_cnf[i].ul_data_id);
    }

    rrc_rlc_cmd_ptr->cmd.crlc_am_config_cnf.am_config_result[i].rb_id =
                                                  cnf_ptr->am_cnf[i].rb_id;
    rrc_rlc_cmd_ptr->cmd.crlc_am_config_cnf.am_config_result[i].result = 
                                                  cnf_ptr->am_cnf[i].result;
  }
  tdsrlci_dl_pdu_cipher_log_on = log_status(TDSCDMA_DL_RLC_PDU_CIPHER_LOG_PACKET);


  TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_DL_UNLOCK();

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if ( j > 0) 
  {
    tdsul_log_state.nchan = j;
    tdsrlc_ul_send_state_log_packet(&tdsul_log_state);
  }
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  tdsrrc_put_rlc_cmd(rrc_rlc_cmd_ptr);

  tdsrlci_inter_ho_happen = FALSE;

}/* tdsrlci_dl_config_cnf_handler() */



/*===========================================================================

FUNCTION CRLC_UL_AM_RESET_HANDLER

DESCRIPTION
  Reset all AM entities that are configured.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_am_reset_handler(void)
{
  int i;
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  uint8 j = 0;
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

  TDSRLC_MSG0(MSG_LEGACY_HIGH,"Reset all AM Entities");


  for (i = 0; i < TDSUE_MAX_UL_RLC_ENTITY; i++)
  {
    if (tdsuplink_table.ctrl_blk[i].lc_mode == UE_MODE_ACKNOWLEDGED)
    {
      tdsrlci_reset_ul_am(&tdsuplink_table.ctrl_blk[i], TRUE);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
      tdsul_log_state.ul_state[j].rlc_id =
         tdsuplink_table.ctrl_blk[i].ctrl.am_ctrl.am_common->ul_data_id;
      tdsul_log_state.ul_state[j].lc_state = TDSAM_UL_RESET_PENDING;
      j++;
#endif  //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

    }
  }


#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (j > 0) 
  {
    tdsul_log_state.nchan = j;
    tdsrlc_ul_send_state_log_packet(&tdsul_log_state);
  }
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

}/* tdscrlc_ul_am_reset_handler() */


/*===========================================================================

FUNCTION RLCI_SET_POST_RRC_RESET_SIG

DESCRIPTION
  Post TDSRLC_UL_RESET_REPORT_SIG to L2 task. This is due to the MAC UL
  moves to interrupt contents. 
  In interrupt context, the dynamic allocation of memory for RRC 
  command will screw up the heap memory. The REX SIG could have the
  RRC message being posted in the L2 context.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_set_post_rrc_reset_sig
(
  tdsrlc_error_e_type  err_type,
  rlc_lc_id_type    rlc_id
)
{
  int i;

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  TDSL2_MUTEX_LOCK(&tdsam_reset_id_table_mutex);
#endif
  for (i=0; i<TDSUE_MAX_AM_ENTITY; i++)
  {
    /* Found an available entry */
    if (tdsam_reset_id_table[i].rlc_id == TDSINIT_CTL_BLK_ID)
    {
      /* Store RESET information */
      tdsam_reset_id_table[i].rlc_id = rlc_id;
      tdsam_reset_id_table[i].err_type = err_type;
      break;
    }
  }
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  TDSL2_MUTEX_UNLOCK(&tdsam_reset_id_table_mutex);
#endif

  /* Post REX SIG to L2 */
  (void) rex_set_sigs( tds_l2_ul_get_tcb(), TDSRLC_UL_RESET_REPORT_SIG );

}/* tdsrlci_set_post_rrc_reset_sig() */


/*===========================================================================

FUNCTION RLCI_POST_RESET_TO_RRC

DESCRIPTION
  Post the RESET result message to RRC
  The RESET information is stored in the tdsam_reset_id_table. 
  
  This function is called in the L2 context.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_post_reset_to_rrc(void)
{
  tdsrrc_rlc_cmd_type *rrc_rlc_cmd_ptr;
  int i;

  /* Get RRC command buffer */

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  TDSL2_MUTEX_LOCK(&tdsam_reset_id_table_mutex);
#endif
  for (i=0; i<TDSUE_MAX_AM_ENTITY; i++)
  {
    if (tdsam_reset_id_table[i].rlc_id != TDSINIT_CTL_BLK_ID)
    {
      /* Obtain an RRC commend buffer */
      if ((rrc_rlc_cmd_ptr = tdsrrc_get_rlc_cmd_buf()) != NULL)
      {
        rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CRLC_STATUS_IND;
        /* Reset cause: Uplink or Downlink  */
        rrc_rlc_cmd_ptr->cmd.crlc_status_ind.direction = TRUE;
        /* Reset or Reset Fail  */
        rrc_rlc_cmd_ptr->cmd.crlc_status_ind.error_type = tdsam_reset_id_table[i].err_type;
#ifdef FEATURE_QSH_EVENT_METRIC
		if(rrc_rlc_cmd_ptr->cmd.crlc_status_ind.error_type == TDSRLC_RESET_FAIL)
			{
				tdsrlc_qsh_metrics.num_rlc_urecov_err++;
				TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_QSH:: Incrementing the number of unrecoverable errors ");

			}
#endif
        rrc_rlc_cmd_ptr->cmd.crlc_status_ind.lc_id = tdsam_reset_id_table[i].rlc_id;
        /* Enqueue the allocated buffer into RRC command queue  */
        tdsrrc_put_rlc_cmd (rrc_rlc_cmd_ptr);
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"Post RLC RESET to RRC,LC %d",tdsam_reset_id_table[i].rlc_id);
      }
      else 
      { 
        TDSRLC_MSG0(MSG_LEGACY_ERROR,"Could not allocate RRC cmd buf");
      }
      tdsam_reset_id_table[i].rlc_id = TDSINIT_CTL_BLK_ID;
    }
  }
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
 TDSL2_MUTEX_UNLOCK(&tdsam_reset_id_table_mutex);
#endif
}/* tdsrlci_post_reset_to_rrc() */


/*===========================================================================

FUNCTION RLCI_UL_PROC_RESET_MSG

DESCRIPTION
  Process the RLCI_DL_RESET_DONE and RLCI_UL_RESET_REQ messages from 
  DL AM RLC. 
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_proc_reset_msg
(
  tdsl2_ul_cmd_enum_type cmd_id, 
  rlc_lc_id_type ul_rlc_data_id
)
{
  uint8 ctl_id;
  rex_crit_sect_type *datapath_mutex = NULL;
    
  ctl_id = tdsul_ctl_blk_id_tbl[ul_rlc_data_id];
  if ((ctl_id == TDSINIT_CTL_BLK_ID) ||
      (ctl_id >= TDSUE_MAX_UL_RLC_ENTITY))
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC-UL LC Id:%d not found",ul_rlc_data_id);
    return;
  }
  
  if (cmd_id == TDSRLCI_DL_RESET_DONE)
  {
/* Needs protection here, since updating HFN values leads to wrong ciphering in UL
 */
  datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(ul_rlc_data_id);

    tdsuplink_table.ctrl_blk[ctl_id].cipher.hfn = 
                (tdsuplink_table.ctrl_blk[ctl_id].cipher.count >> 12) + 1;

    tdsuplink_table.ctrl_blk[ctl_id].cipher.count = 
            tdsuplink_table.ctrl_blk[ctl_id].cipher.hfn << 12 | 
            tdsuplink_table.ctrl_blk[ctl_id].ctrl.am_ctrl.ul_state.vt_s;

    /* Complete the RESET procedure. Return back to the original state. */
    /*lint -save -e64 -e641 */
    tdsuplink_table.ctrl_blk[ctl_id].lc_state &= (~TDSUL_RESET_PENDING);
    /*lint -restore */
    TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    
    TDSRLC_MSG3(MSG_LEGACY_HIGH,"UL new HFN %x, vt_s %x, LC %d", 
              tdsuplink_table.ctrl_blk[ctl_id].cipher.hfn, 
              tdsuplink_table.ctrl_blk[ctl_id].ctrl.am_ctrl.ul_state.vt_s, ul_rlc_data_id);

    TDSRLC_MSG1(MSG_LEGACY_HIGH,"LC %d Complete RESET procedure",ul_rlc_data_id);

    /* Let RRC know that the RLC AM reset */
    tdsrlci_set_post_rrc_reset_sig(TDSRLC_RESET, ul_rlc_data_id);


#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
    tdsul_log_state.ul_state[0].rlc_id = ul_rlc_data_id;
    switch (tdsuplink_table.ctrl_blk[ctl_id].lc_state) 
    {
      case TDSUL_DATA_TRANSFER_READY:
        tdsul_log_state.ul_state[0].lc_state = TDSAM_UL_DATA_TRANSFER_READY;
        break;
      case TDSUL_WAITING_TO_SUSPEND:
        tdsul_log_state.ul_state[0].lc_state = TDSAM_UL_WAITING_TO_SUSPEND;
        break;
      case TDSUL_SUSPENDED:
        tdsul_log_state.ul_state[0].lc_state = TDSAM_UL_SUSPENDED;
        break;
      case TDSUL_CONFIG_PENDING:
        tdsul_log_state.ul_state[0].lc_state = TDSAM_UL_CONFIG_PENDING;
        break;
      default:
        TDSRLC_MSG0(MSG_LEGACY_ERROR,"Invalid state,QXDM");
    }
    tdsul_log_state.nchan = 1;
    tdsrlc_ul_send_state_log_packet(&tdsul_log_state);
#endif

  }
  else if (cmd_id == TDSRLCI_UL_RESET_REQ)
  {
    TDSL2_MUTEX_LOCK(&tdsrlc_dl_data_process_mutex);

/*
 *  Lock DL mutex so as to make sure that UL-entity is completely
 *  Reset before DL task resumes
 */
    TDSL2_ACQ_DL_LOCK();

    /* Rx RESET request from the peer side via the DL AM RLC. */
    tdsrlci_reset_ul_am(&tdsuplink_table.ctrl_blk[ctl_id], FALSE);
    tdsrlci_request_tx_reset_ack(ul_rlc_data_id);
    TDSL2_ACQ_DL_UNLOCK();

    TDSL2_MUTEX_UNLOCK(&tdsrlc_dl_data_process_mutex);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
    tdsul_log_state.ul_state[0].rlc_id = ul_rlc_data_id;
    tdsul_log_state.ul_state[0].lc_state = TDSAM_UL_RESET_PENDING;
    tdsul_log_state.nchan = 1;
    tdsrlc_ul_send_state_log_packet(&tdsul_log_state);
#endif
  
  }
  else if(cmd_id == TDSRLCI_UL_DUP_RESET_REQ)
  {
    tdsrlci_set_duplicate_reset_rx(ul_rlc_data_id);
  }
  else
  {
    TDSRLC_MSG1(MSG_LEGACY_HIGH,"Unknown UL RLC ID %d reset msg",ul_rlc_data_id);
  }

}/* tdsrlci_ul_proc_reset_msg() */


/*===========================================================================

FUNCTION CRLC_RELEASE_ALL_HANDLER

DESCRIPTION
  Releases all Uplink Channels and sends a command to the Downlink to 
  release all downlink channels.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_release_all_handler(
  void
 )
#if 0 
{
  tdsl2_dl_cmd_type *dl_cmd;          /* Command Buffer for sending Release   */
                                   /* Request to Downlink                  */
  /*-------------------------------------------------------------------------
    Get command Buffer for posting Release ALL Request to RLC downlink.
  -------------------------------------------------------------------------*/
  dl_cmd = tdsl2_dl_get_cmd_buf();
  if (dl_cmd == NULL)
  {
    //ERR_FATAL("Could not get cmd buffer",0,0,0);
    TDSRLC_MSG0(MSG_LEGACY_HIGH,"Could not get cmd buffer");
    return;
  }
  {
    dl_cmd->cmd_hdr.cmd_id = TDSRLCI_DL_RELEASE_ALL_REQ;
  }
	
  /*-------------------------------------------------------------------------
    Post Release Request to Downlink 
  -------------------------------------------------------------------------*/
  TDSRLC_MSG0(MSG_LEGACY_LOW,"Post Release Request to Downlink");
  tdsl2_dl_put_cmd(dl_cmd);
  
  tdsrlci_inter_ho_happen = FALSE;
  Bitmap_specail_handle.inter_rnc_ho_start = FALSE;
  Bitmap_specail_handle.inter_rnc_ho_end   = TRUE;
} /* tdscrlc_release_all_handler() */
#else
{
  uint8 ctrl_blk_id;               /* Index into the Uplink Table          */

#if 0
  tdsl2_dl_cmd_type *dl_cmd;          /* Command Buffer for sending Release   */
                                   /* Request to Downlink                  */
#endif

  uint8 i = 0;
  uint32 index = 0, act_ul_am_chnl = 0;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  uint8 j = 0;
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING 

#if 0
  /*-------------------------------------------------------------------------
    Get command Buffer for posting Release ALL Request to RLC downlink.
  -------------------------------------------------------------------------*/
  dl_cmd = tdsl2_dl_get_cmd_buf();
  if (dl_cmd == NULL)
  {
    //ERR_FATAL("Could not get cmd buffer",0,0,0);
    TDSRLC_MSG0(MSG_LEGACY_HIGH,"Could not get cmd buffer");
    return;
  }
  TDSRLC_MSG0(MSG_LEGACY_LOW,"Release ALL Uplink Channels");
  {
    dl_cmd->cmd_hdr.cmd_id = TDSRLCI_DL_RELEASE_ALL_REQ;
  }
#endif

TDSL2_MUTEX_LOCK(&tdsrlc_dl_data_process_mutex);

  TDSL2_ACQ_DL_LOCK();
  /*Locking both EUL and R99 Mutex*/
  (void)TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);

  /*-------------------------------------------------------------------------
    Release all active Uplink Channels.
  -------------------------------------------------------------------------*/
  while (tdsuplink_table.active_ch > 0)
  {
    /*-----------------------------------------------------------------------
     Check if the channel is in NULL state. If it is not, release the 
     channel.  
    -----------------------------------------------------------------------*/
    if ((ctrl_blk_id = tdsul_ctl_blk_id_tbl[i]) != TDSINIT_CTL_BLK_ID &&
        (ctrl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
    {
      TDSRLC_MSG3(MSG_LEGACY_MED, "Release UL LC %d, Index %d active_ul_ch %d", i, ctrl_blk_id, tdsuplink_table.active_ch);
      if((tdsuplink_table.ctrl_blk[ctrl_blk_id].lc_type == UE_LOGCHAN_DTCH) && (tdsuplink_table.ctrl_blk[ctrl_blk_id].lc_mode == UE_MODE_TRANSPARENT))
      {	
        tdsl2_ivoice.enable_flag = FALSE;
        TDSRLC_MSG0(MSG_LEGACY_HIGH,"De-registering the call back as all active uplink channels are released ");		
      }      
      /*
         LC in NULL-STATE is checked every time ISR tries to access this LC, So keep
         this part of code under protection to make this an atomic operation
      */
      //TDSL2_ACQ_UL_LW_LOCK();
      tdsuplink_table.ctrl_blk[ctrl_blk_id].lc_state = TDSUL_NULL_STATE;
      //TDSL2_ACQ_UL_LW_UNLOCK();
      
      /* Release discard timer  */
      if(tdsuplink_table.ctrl_blk[ctrl_blk_id].tmr_discard_ptr != NULL)
      {
        (void) rex_clr_timer(tdsuplink_table.ctrl_blk[ctrl_blk_id].tmr_discard_ptr);
        /* release timer block */
        rex_delete_timer_ex(tdsuplink_table.ctrl_blk[ctrl_blk_id].tmr_discard_ptr);
        /* Reset timer sig */
        tdsuplink_table.ctrl_blk[ctrl_blk_id].tmr_discard_ptr = NULL;
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"Release SDU discard timer,%d",i);
      }
      if (tdsuplink_table.ctrl_blk[ctrl_blk_id].lc_mode == UE_MODE_ACKNOWLEDGED)
      {
        tdsuplink_table.active_ch -= tdsrlci_release_ul_am(&tdsuplink_table.ctrl_blk[ctrl_blk_id]);
      }
      else
      {
        tdsuplink_table.active_ch--;
      }

      if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
      {
        tdsrlc_dump_ul_ctl_blk_id_tbl();
      }

      tdsul_ctl_blk_id_tbl[i] = TDSINIT_CTL_BLK_ID;                
      if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
      {
        tdsrlc_sanitize_ul_ctl_blk_id_tbl();
      }
	  
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
      tdsul_log_state.ul_state[j].rlc_id = i;
      tdsul_log_state.ul_state[j].lc_state = TDSRLC_UL_NULL_STATE;
      j++;
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING
    
    }
    i++;
  }

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (j > 0) 
  {
    tdsul_log_state.nchan = j;
    tdsrlc_ul_send_state_log_packet(&tdsul_log_state);
  }
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

#ifdef FEATURE_QXDM_DSM_TRACING
  if (tdsnum_rlc_non_srb_entities_up > 0)
  {
    fc_post_deregister_cmd_dsm_trace
      (DSM_UMTS_TECH,
       UMTS_RLC_CLIENT);
    TDSRLC_MSG0(MSG_LEGACY_HIGH,"Deregistered Qxdm DSM tracing");
    tdsnum_rlc_non_srb_entities_up = 0;
  }
#endif

#if defined(FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL)
  if (tdsrlc_acti_non_srb_am_ch)
  {
    tdsrlc_acti_non_srb_am_ch = 0;
#ifdef FEATURE_TDSCDMA_CFCM
    tdsrlc_cfcm_deregister();
#endif
  }
#endif /* (FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL) */

  for (index = 0, act_ul_am_chnl = 0; index < TDSUE_MAX_UL_LOGICAL_CHANNEL; index++)
  {
    ctrl_blk_id = tdsul_ctl_blk_id_tbl[index];
    
    if ((ctrl_blk_id != TDSINIT_CTL_BLK_ID) && 
         (ctrl_blk_id < TDSUE_MAX_UL_RLC_ENTITY) &&
        ((TDSUL_NULL_STATE != tdsuplink_table.ctrl_blk[ctrl_blk_id].lc_state) && 
        (UE_MODE_ACKNOWLEDGED == tdsuplink_table.ctrl_blk[ctrl_blk_id].lc_mode)))
    {
      act_ul_am_chnl++;
      break;
    }
  }

  if ((0 == act_ul_am_chnl) && (TRUE == tdsrlc_ul_am_base_tmr_start))
  {
    /* Since all AM channels are released, stop base ul am timer */
    (void) rex_clr_timer(&tdsrlc_ul_am_base_tmr);
    tdsrlc_ul_am_base_tmr_start = FALSE;
    TDSRLC_MSG1(MSG_LEGACY_HIGH, "RLC_TMR_DBG: Stopping base UL timer,active_ul_channels %d",tdsuplink_table.active_ch);

  }

  /*-------------------------------------------------------------------------
      Release downlink in uplink task directly
   -------------------------------------------------------------------------*/
  tdscrlc_dl_release_all_handler();

  TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_DL_UNLOCK();

#if 0
  /*-------------------------------------------------------------------------
    Post Release Request to Downlink 
  -------------------------------------------------------------------------*/
  TDSRLC_MSG0(MSG_LEGACY_LOW,"Post Release Request to Downlink");
  tdsl2_dl_put_cmd(dl_cmd);
#endif
  TDSL2_MUTEX_UNLOCK(&tdsrlc_dl_data_process_mutex);

  tdsrlci_inter_ho_happen = FALSE;
  Bitmap_specail_handle.inter_rnc_ho_start = FALSE;
  Bitmap_specail_handle.inter_rnc_ho_end   = TRUE;
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA 
  tdsl2_ta_opt_info.turn_away_evaluate_start = FALSE;
  tdsl2_ta_opt_info.turn_away_opt_is_valid   = FALSE;
#endif  
} /* tdscrlc_release_all_handler() */
#endif

//xlong release opt
/*===========================================================================

FUNCTION tdsrlci_ul_proc_release_all_cnf_msg

DESCRIPTION
  Releases all Uplink Channels after receive dl release cnf done.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_proc_release_all_cnf_msg(
  void
 )
{
	uint8 ctrl_blk_id;							 /* Index into the Uplink Table 				 */

	uint8 i = 0;
	uint32 index = 0, act_ul_am_chnl = 0;

	tdsrrc_rlc_cmd_type *rrc_rlc_cmd_ptr;    /* Command Buffer for sending Release  */
                                           /* All confirm to RRC                  */
	
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
	uint8 j = 0;
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING 

	/*-------------------------------------------------------------------------
		Get command Buffer for posting Release ALL confirm Confirm to RRC.
	-------------------------------------------------------------------------*/
	rrc_rlc_cmd_ptr = tdsrrc_get_rlc_cmd_buf();
	if (rrc_rlc_cmd_ptr == NULL)
	{
	  ERR_FATAL("Could not get cmd buffer",0,0,0);
	   /*lint -e527 */
	   return;
	   /*lint +e527 */
	}
	rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CRLC_RELEASE_ALL_CNF;


	TDSL2_ACQ_DL_LOCK();
	/*Locking both EUL and R99 Mutex*/
	(void)TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);

	/*-------------------------------------------------------------------------
		Release all active Uplink Channels.
	-------------------------------------------------------------------------*/
	while (tdsuplink_table.active_ch > 0)
	{
		/*-----------------------------------------------------------------------
		 Check if the channel is in NULL state. If it is not, release the 
		 channel.  
		-----------------------------------------------------------------------*/
		if ((ctrl_blk_id = tdsul_ctl_blk_id_tbl[i]) != TDSINIT_CTL_BLK_ID &&
        (ctrl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
		{
			TDSRLC_MSG3(MSG_LEGACY_HIGH, "Release UL LC %d, Index %d active_ul_ch %d", i, ctrl_blk_id, tdsuplink_table.active_ch);
			/*
				 LC in NULL-STATE is checked every time ISR tries to access this LC, So keep
				 this part of code under protection to make this an atomic operation
			*/
			//TDSL2_ACQ_UL_LW_LOCK();
			tdsuplink_table.ctrl_blk[ctrl_blk_id].lc_state = TDSUL_NULL_STATE;
			//TDSL2_ACQ_UL_LW_UNLOCK();
			
			/* Release discard timer	*/
			if(tdsuplink_table.ctrl_blk[ctrl_blk_id].tmr_discard_ptr != NULL)
			{
				(void) rex_clr_timer(tdsuplink_table.ctrl_blk[ctrl_blk_id].tmr_discard_ptr);
				/* release timer block */
				rex_delete_timer_ex(tdsuplink_table.ctrl_blk[ctrl_blk_id].tmr_discard_ptr);
				/* Reset timer sig */
				tdsuplink_table.ctrl_blk[ctrl_blk_id].tmr_discard_ptr = NULL;
				TDSRLC_MSG1(MSG_LEGACY_HIGH,"Release SDU discard timer,%d",i);
			}
			if (tdsuplink_table.ctrl_blk[ctrl_blk_id].lc_mode == UE_MODE_ACKNOWLEDGED)
			{
				tdsuplink_table.active_ch -= tdsrlci_release_ul_am(&tdsuplink_table.ctrl_blk[ctrl_blk_id]);
			}
			else
			{
				tdsuplink_table.active_ch--;
			}

			if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
			{
				tdsrlc_dump_ul_ctl_blk_id_tbl();
			}

			tdsul_ctl_blk_id_tbl[i] = TDSINIT_CTL_BLK_ID; 							 
			if (TDSRLC_DBG_UL_CTL_BLK_ID_TBL & tdsrlc_debug_var)
			{
				tdsrlc_sanitize_ul_ctl_blk_id_tbl();
			}
		
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
			tdsul_log_state.ul_state[j].rlc_id = i;
			tdsul_log_state.ul_state[j].lc_state = TDSRLC_UL_NULL_STATE;
			j++;
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING
		
		}
		i++;
	}

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
	if (j > 0) 
	{
		tdsul_log_state.nchan = j;
		tdsrlc_ul_send_state_log_packet(&tdsul_log_state);
	}
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

#ifdef FEATURE_QXDM_DSM_TRACING
	if (tdsnum_rlc_non_srb_entities_up > 0)
	{
		fc_post_deregister_cmd_dsm_trace
			(DSM_UMTS_TECH,
			 UMTS_RLC_CLIENT);
		TDSRLC_MSG0(MSG_LEGACY_HIGH,"Deregistered Qxdm DSM tracing");
		tdsnum_rlc_non_srb_entities_up = 0;
	}
#endif


#if defined(FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL)
	if (tdsrlc_acti_non_srb_am_ch)
	{
		tdsrlc_acti_non_srb_am_ch = 0;
#ifdef FEATURE_TDSCDMA_CFCM
		tdsrlc_cfcm_deregister();
#endif
	}
#endif /* (FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL) */

	for (index = 0, act_ul_am_chnl = 0; index < TDSUE_MAX_UL_LOGICAL_CHANNEL; index++)
	{
		ctrl_blk_id = tdsul_ctl_blk_id_tbl[index];
		
		if ((ctrl_blk_id != TDSINIT_CTL_BLK_ID) && 
        (ctrl_blk_id < TDSUE_MAX_UL_RLC_ENTITY) &&
				((TDSUL_NULL_STATE != tdsuplink_table.ctrl_blk[ctrl_blk_id].lc_state) && 
				(UE_MODE_ACKNOWLEDGED == tdsuplink_table.ctrl_blk[ctrl_blk_id].lc_mode)))
		{
			act_ul_am_chnl++;
			break;
		}
	}

	if ((0 == act_ul_am_chnl) && (TRUE == tdsrlc_ul_am_base_tmr_start))
	{
		/* Since all AM channels are released, stop base ul am timer */
		(void) rex_clr_timer(&tdsrlc_ul_am_base_tmr);
		tdsrlc_ul_am_base_tmr_start = FALSE;
		TDSRLC_MSG1(MSG_LEGACY_HIGH, "RLC_TMR_DBG: Stopping base UL timer,active_ul_channels %d",tdsuplink_table.active_ch);

	}

	TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
	TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);
	TDSL2_ACQ_DL_UNLOCK();

	/*-------------------------------------------------------------------------
		Post Release All Confirm to RRC.
	-------------------------------------------------------------------------*/
	TDSRLC_MSG0(MSG_LEGACY_HIGH,"Post Release_All confirm to RRC");
	tdsrrc_put_rlc_cmd(rrc_rlc_cmd_ptr);

}
/*=========================================================================

FUNCTION RLCI_CHANGE_PEER_TX_WIN

DESCRIPTION

    This function changes the peer side's transmission window size.

    This Peer side tx window size changing is triggered by DL activities
    when the configuring DL Rx Win size is greater than the UE can support.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

void tdsrlci_change_peer_tx_win
(
  rlc_lc_id_type rlc_id,
  uint16 rx_window_size
)
{
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr;
  rex_crit_sect_type *datapath_mutex = NULL;

  if ((ctl_blk_ptr = tdsrlci_get_ctl_blk_ptr(rlc_id)) == NULL)
  {
    return;
  }
/*
  UL-ISR sends a WIN-SUFI based on the status of these flags. So lock it under mutex
*/
  datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(rlc_id);

  ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins = rx_window_size;
  ctl_blk_ptr->ctrl.am_ctrl.ul_state.tx_wins = TRUE;
  ctl_blk_ptr->ctrl.am_ctrl.ul_state.retx_win_size = TRUE;
  ctl_blk_ptr->ctrl.am_ctrl.ul_state.n_retx_win = 0;
  ctl_blk_ptr->ctrl.am_ctrl.ul_state.retx_win_cnt = 0;

  TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  TDSRLC_MSG1(MSG_LEGACY_LOW,"Change peer's TX Window,LC %d",rlc_id);

} /* tdsrlci_change_peer_tx_win */

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
/*=========================================================================

FUNCTION RLCI_CHANGE_PEER_TX_WIN_FC

DESCRIPTION

    This function changes the peer side's transmission window size.

    This Peer side tx window size changing is triggered by DL activities
    when, UE is already in FLOW CONTROL due to which UE suggested DL Rx
    window is different than NW Configured initial DL Rx Window. This values
    will be reset to initial configured values due to RESET procedure. Need
    to restore them back to values triggered due to flow control.


DEPENDENCIES
  None.

RETURN VALUE
  window size

SIDE EFFECTS
  None.

=========================================================================*/


uint16 tdsrlci_change_peer_tx_win_fc
(
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr
)
{
  uint16 rlc_win_size = 0;
  rex_crit_sect_type *datapath_mutex = NULL;

  datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(ctl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id);
  if (ctl_blk_ptr->ctrl.am_ctrl.ul_state.rlc_fc_ws.rlc_fc_ws != ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins)
  {
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins = ctl_blk_ptr->ctrl.am_ctrl.ul_state.rlc_fc_ws.rlc_fc_ws;
    rlc_win_size = ctl_blk_ptr->ctrl.am_ctrl.ul_state.rlc_fc_ws.rlc_fc_ws;
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.tx_wins = TRUE;
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.retx_win_size = TRUE;
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.n_retx_win = 0;
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.retx_win_cnt = 0;
  }
  TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  TDSRLC_MSG1(MSG_LEGACY_LOW,"Change peer's TX Window,LC %d",ctl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id);
  return rlc_win_size;

} /* tdsrlci_change_peer_tx_win */


#endif

#ifndef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
/*===========================================================================

FUNCTION RLC_OPEN_PEER_TX_WIN

DESCRIPTION
  Set tx_wins flag for the next TTI to send status PDU to open the peer
  side's TX_WIN_SIZE.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_open_peer_tx_win
(
  rlc_lc_id_type rlc_id,
  tdsrlci_close_rx_win_e_type close_rx_win,
  tdsrlci_open_rx_win_e_type open_rx_win
)
{
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr;
  uint16  rx_win_size;
  uint8 event_win_open = 0;
  rex_crit_sect_type *datapath_mutex = NULL;

  if ((ctl_blk_ptr = tdsrlci_get_ctl_blk_ptr(rlc_id)) == NULL)
  {
    return;
  }

  datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(rlc_id);
  /*lint -save -e64 -e641 */
  ctl_blk_ptr->ctrl.am_ctrl.ul_state.close_rx_win &= (~close_rx_win);
  /*lint -restore */
  TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);

  /*-------------------------------------------------------------------------
    Despite the 1st bit of indicating the opening half size of the peer's 
    tx window, There is someone who close the peer's window.
  -------------------------------------------------------------------------*/
  /*lint -e641 */
  if ((ctl_blk_ptr->ctrl.am_ctrl.ul_state.close_rx_win & 0x7F) != TDSNONE_CLOSE)
  /*lint +e641 */
  {
    TDSRLC_MSG1(MSG_LEGACY_HIGH,"Peer's TX Window closed,LC %d",rlc_id);
  }
  /*-------------------------------------------------------------------------
    There is no one close the peer's tx window.
  -------------------------------------------------------------------------*/
  else
  {
    datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(rlc_id);

    rx_win_size = tdsrlci_get_rx_win_size(ctl_blk_ptr->ctrl.am_ctrl.am_common->dl_data_id);
    
    /*lint -e655 */
    if ((open_rx_win == TDSHALF_WIN) ||
        ((open_rx_win == TDSDONT_CARE) && 
         (ctl_blk_ptr->ctrl.am_ctrl.ul_state.close_rx_win & TDSHALF_CLOSE_WIN)))
    /*lint +e655 */
    {
      ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins = rx_win_size;
    }
    else
    {
      ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins = rx_win_size;
    }
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.tx_wins = TRUE;
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.retx_win_size = TRUE;
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.n_retx_win = 0;
    TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    
    TDSRLC_MSG1(MSG_LEGACY_HIGH,"Open peer's TX Window,LC %d",rlc_id);
    /*----------------------------------------------------------------------
      Post a QXDM event when the peer's transmission window is opened.
      The payload is:
              RLC_window_state: 
                  0  RLC Window Open
                  1  RLC Window PS Close 
                  2  RLC Window DSM Close

              UE_UTRAN:  
                  0  initiated by UE
                  1  initiated by UTRAN   
    ----------------------------------------------------------------------*/
    event_report_payload(EVENT_TDSCDMA_RLC_OPEN_CLOSE, 1,(void *)&event_win_open);
  }


}/* tdsrlc_open_peer_tx_win() */


/*===========================================================================

FUNCTION RLC_CLOSE_PEER_TX_WIN

DESCRIPTION
  Set tx_wins flag for the next TTI to send status PDU to close the peer
  side's TX_WIN_SIZE.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_close_peer_tx_win
(
  rlc_lc_id_type rlc_id, 
  tdsrlci_close_rx_win_e_type close_rx_win
)
{
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr;
  uint8 event_win_close = 0;
  rex_crit_sect_type *datapath_mutex = NULL;

  if ((ctl_blk_ptr = tdsrlci_get_ctl_blk_ptr(rlc_id)) == NULL)
  {
    return;
  }


  datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(rlc_id);
  ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins = 1;
  ctl_blk_ptr->ctrl.am_ctrl.ul_state.tx_wins = TRUE;
  /* Turn on the timer for retx Window SUFI */
  ctl_blk_ptr->ctrl.am_ctrl.ul_state.retx_win_size = TRUE;
  ctl_blk_ptr->ctrl.am_ctrl.ul_state.n_retx_win = 0;

  /* Raise TDSAPP_CLOSE  */
  /*lint -save -e655 */
  ctl_blk_ptr->ctrl.am_ctrl.ul_state.close_rx_win |= close_rx_win;
  /*lint -restore */

  /*----------------------------------------------------------------------
      Post a QXDM event when peer side's transmission window is closed.
      The payload is:
           RLC_window_state: 
                         0  RLC Window Open
                         1  RLC Window PS Close 
                         2  RLC Window DSM Close

           UE_UTRAN:  
                        0  initiated by UE
                        1  initiated by UTRAN   
    ----------------------------------------------------------------------*/
  if (close_rx_win == TDSRX_TX_CLOSE)
  {
    /*lint -e655 */
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.close_rx_win |= TDSHALF_CLOSE_WIN;
    /*lint +e655 */

    event_win_close = 0x0002;
    
  }
  else
  {
    event_win_close = 0x0001;
    
  }
  TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);

  event_report_payload(EVENT_TDSCDMA_RLC_OPEN_CLOSE, 2, (void *)&event_win_close);

  
  TDSRLC_MSG1(MSG_LEGACY_HIGH,"Close peer's TX Window,LC %d",rlc_id);

}/* tdsrlc_close_peer_tx_win() */
#endif //!FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL



#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLCI_UL_AM_HANDLE_QXDM_LOG_TIMER

DESCRIPTION
  Collect the AM statistics data and send the statistics log packet out.
    
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_ul_am_handle_qxdm_log_timer
(
  uint32 ul_log_timer_sig
   /*lint -esym(715,ul_log_timer_sig) */
)
{
  /* UL control block */
  tdsrlci_uplink_table_type *uplink_tbl;
#if 0
	/* Set this log timer tick again */
  (void) rex_set_timer(&tdsul_tmr_log_blk, (uint32)TDSRLC_AM_LOG_TIMER_BASE); 
#endif  
  /*---------------------------------------------------------------------
   Get the log packet with all the statistics information for all the 
   channels to be sent to qxdm for logging purposes every 3seconds.  
   Send the log packet to the qxdm for logging purposes every 3seconds.  
  ---------------------------------------------------------------------*/

  if (log_status(TDSCDMA_UL_RLC_AM_STAT_LOG_PACKET))
  {
    TDSRLC_MSG0(MSG_LEGACY_MED,"Sending V2 log packet");
    uplink_tbl = &tdsuplink_table;
    tdsrlc_ul_tx_am_statistic_pkt(uplink_tbl);
  }
}/* tdsrlci_ul_am_handle_qxdm_log_timer() */

#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING


/*===========================================================================

FUNCTION RLC_RRC_GET_HFN_REQ

DESCRIPTION
   This function is called by RRC to get latest HFN's for all established
   RLC UM and AM radio-bearers.For cipher pending logical channels a value
   of zero is retuned. RLC populates the * to struct tdsrlc_hfn_req_type.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS:  If HFN's for all UM and AM rb's are populated.
  FAILURE:  If no UM and AM rb's are present.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type tdsrlc_rrc_get_hfn_req
(
  tdsrlc_hfn_req_type* hfn_req             /*ptr to hfn request type          */
)
{
  uint16  count;                        /* Number of active Channels       */
  uint16  index = 0;
  uint8 array_len = 0;
  //rex_crit_sect_type *datapath_mutex = NULL;

  /*------------------------------------------------------------------------
    Initialize the number of channels and index to 0.
  ------------------------------------------------------------------------*/
  hfn_req->ul_nchan = 0;

  /*------------------------------------------------------------------------
    Get the number of active channels from the uplink table.
  ------------------------------------------------------------------------*/
  count = tdsuplink_table.active_ch;
  array_len = sizeof(hfn_req->hfn_per_rb_ul)/sizeof(tdsrlc_hfn_type);
  //datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);
  (void)TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);
  while((count > 0) && (index < TDSUE_MAX_UL_RLC_ENTITY) && (hfn_req->ul_nchan < array_len))
  {
    if (tdsuplink_table.ctrl_blk[index].lc_state != TDSUL_NULL_STATE)  
    {
      count--;
      if (tdsuplink_table.ctrl_blk[index].lc_mode != UE_MODE_TRANSPARENT &&
          tdsuplink_table.ctrl_blk[index].cipher.cipher_mode != TDSNO_CIPHER)
      {
        if (tdsuplink_table.ctrl_blk[index].lc_mode == UE_MODE_UNACKNOWLEDGED)
        {
          /*------------------------------------------------------------------------
            Remove the LS 7 bits as of SN.
          ------------------------------------------------------------------------*/
          if (tdsuplink_table.ctrl_blk[index].cipher.cipher_mode == TDSCIPHER_ON)
          {
          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn_valid = TRUE;
          }
          else
          {
            hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn_valid = FALSE;

          }

          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn = 
                          tdsuplink_table.ctrl_blk[index].cipher.count >> 7;

          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_sn = 
                          tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.vt_us;

          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_id = 
                          tdsuplink_table.ctrl_blk[index].ctrl.um_ctrl.rlc_id;
        }
        else
        {
          /*------------------------------------------------------------------------
            Remove the LS 12 bits as of SN.
          ------------------------------------------------------------------------*/
          if (tdsuplink_table.ctrl_blk[index].cipher.cipher_mode == TDSCIPHER_ON)
          {
            hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn_valid = TRUE;
          }
          else
          {
            hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn_valid = FALSE;
          }

          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn = 
                          tdsuplink_table.ctrl_blk[index].cipher.count >> 12;

          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_sn = 
                          tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_s;

          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_id = 
            tdsuplink_table.ctrl_blk[index].ctrl.am_ctrl.am_common->ul_data_id;
        }
        
        TDSRLC_MSG3(MSG_LEGACY_HIGH,"RLC_DBG: RRC req UL Cipher LC %d, rlc_sn %d hfn 0x%x",
                          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_id, 
                          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_sn,
                          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn);

        hfn_req->ul_nchan ++;
      }
    }
    index++;
  }
  //TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);

  /* Get DL HFN */
  (void) tdsrlci_get_dl_hfn                   (hfn_req);

  if (hfn_req->ul_nchan > 0)
  {
    return SUCCESS;
  }
  else
  {
    return FAILURE;
  }
}/* tdsrlc_rrc_get_hfn_req() */




/*===========================================================================

FUNCTION RLC_RRC_GET_WM_INFO_REQ

DESCRIPTION
   This function is called by RRC to get number of bytes in RLC Watermark queue.

DEPENDENCIES
  None.

RETURN VALUE
  RLC returns total number of bytes(uint32) awaiting transmission on the requested
  logical channel.  The following are accounted in bytes:
  1. All SDUs in the water mark Q along with length indicators.
  2. Length indicators, resulting from previous RLC PDU transmission, that indicate
     the previous tx. SDU fits exactly or one octet short of the RLC PDU
     "special_ind" indicates the previouly tx. SDU exactly fits into in AM PDU.
     "octet_less" indicates the previouly tx. SDU is one octet short of AM PDU.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 tdsrlc_rrc_get_wm_info_req
(
  rlc_lc_id_type lc_id             /* logical channel ID for which length of
                                      WM is needed                          */
)
{
  uint8 ctl_blk_id;
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;
  uint32 num_bytes = 0;
  rex_crit_sect_type *datapath_mutex = NULL;

  if (lc_id < TDSUE_MAX_UL_LOGICAL_CHANNEL &&
      (ctl_blk_id = tdsul_ctl_blk_id_tbl[lc_id]) != TDSINIT_CTL_BLK_ID &&
      (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
  {
    ctl_blk_ptr = &tdsuplink_table.ctrl_blk[ctl_blk_id];
    datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(lc_id);
    if(ctl_blk_ptr->ul_wm_ptr != NULL)
    {
      num_bytes = ctl_blk_ptr->ul_wm_ptr->current_cnt +
                ((uint32)ctl_blk_ptr->ul_wm_ptr->q_ptr->cnt *
                 ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length) +
                (ctl_blk_ptr->special_ind ?
                 ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length : 0) +
                (ctl_blk_ptr->octet_less ?
                 ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length : 0);
      if (NULL != ctl_blk_ptr->cur_sdu_ptr)
      {
        num_bytes += ((ctl_blk_ptr->cur_sdu_len - ctl_blk_ptr->cur_sdu_tx_len)
                 + ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length);
      }
    }
    else
    {
      num_bytes = (ctl_blk_ptr->special_ind ?
                   ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length : 0) +
                  (ctl_blk_ptr->octet_less ?
                   ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length : 0);
      if (NULL != ctl_blk_ptr->cur_sdu_ptr)
      {
        num_bytes += ((ctl_blk_ptr->cur_sdu_len - ctl_blk_ptr->cur_sdu_tx_len)
                 + ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length);
      }
    }
    TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  }

  TDSRLC_MSG2(MSG_LEGACY_HIGH,"Payload size, LC %d,num_bytes %d",lc_id,num_bytes);

  return num_bytes;

}/* tdsrlc_rrc_get_wm_info_req() */


#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLCI_GET_UL_PDU_CIPHER_LOG_ON

DESCRIPTION
  This function will be call when each PDU is building. This function is to 
  avoid to check the logging bit from the DIAG every time when a PDU is 
  building, variable tdsrlci_ul_pdu_cipher_log_on will be updated at the 
  beginning of building PDUs each TTI. 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the PDU ciphering parameters logging is on.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlci_get_ul_pdu_cipher_log_on (void)
{
  return tdsrlci_ul_pdu_cipher_log_on;
}


/*===========================================================================

FUNCTION RLCI_GET_UL_PDU_CIPHER_LOG_ON

DESCRIPTION
  Provide the access pointer of tdsrlci_ul_pdu_cipher_log data structure.  

DEPENDENCIES
  None.

RETURN VALUE
  pointer of tdsrlci_ul_pdu_cipher_log.

SIDE EFFECTS
  None.

===========================================================================*/
tdsrlc_ul_pdu_cipher_info_type *tdsrlci_get_ul_pdu_cipher_log_ptr(void)
{
  return &tdsrlci_ul_pdu_cipher_log;
}
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING



/*===========================================================================

FUNCTION RLC_UL_REMOVE_SDUS_FROM_WM

DESCRIPTION
  This function removes the SDUs from the UL WM. The number of SDUs that 
  are purged is the minimum of number specified by higher layers and the
  number of SDUs present in the WM.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_remove_sdus_from_wm
(
  tdsrlci_ul_ctrl_blk_type *ctrl_blk_ptr,
  uint8 tdsnum_sdus
)
{
  dsm_item_type *temp_ptr = NULL;  /* Temporary Pointer                    */
  uint8 tdsnum_sdus_to_discard = 0;   /*Number of SDUs to be discarded        */
  uint8 tdsnum_sdus_in_wm      = 0;
  uint8 mui                 = 0;



  tdsnum_sdus_in_wm = (uint8)q_cnt(ctrl_blk_ptr->ul_wm_ptr->q_ptr);

  if(tdsnum_sdus_in_wm != 0)
  {

    /*-----------------------------------------------------------------------
      Might have to indicate SDU failure to higher layers in future
    -----------------------------------------------------------------------*/
    tdsnum_sdus_to_discard = MIN(tdsnum_sdus, tdsnum_sdus_in_wm);
    TDSRLC_MSG3(MSG_LEGACY_HIGH,"Discarding %d SDUs, no_sdu %d, q_cnt %d frm WM",
                             tdsnum_sdus_to_discard, tdsnum_sdus, tdsnum_sdus_in_wm);

    /*-----------------------------------------------------------------------
      Keep removing the SDUs until the specified number of SDUs have been 
      removed or the WM is empty.
    -----------------------------------------------------------------------*/
    while(tdsnum_sdus_to_discard > 0)
    {
     
      if ((temp_ptr = dsm_dequeue(ctrl_blk_ptr->ul_wm_ptr)) == NULL)
      {
        TDSRLC_MSG0(MSG_LEGACY_ERROR,"UL WM contains a NULL item");
        break;
      }
      else
      {
        /*-------------------------------------------------------------------
          If the mode is AM mode, report the failure for tx of the SDUs
          that are purged.
        -------------------------------------------------------------------*/
        if(ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED)
        {
          mui = (uint8)((temp_ptr->app_field & 0x0000FF00) >> 8);
          if (temp_ptr->app_field & 0x00000001)
          {
            ctrl_blk_ptr->ctrl.am_ctrl.sdu_cnf_cb(
              ctrl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id, 
              FAILURE, mui, WM_PURGE_REQ);
          }
        }

        dsm_free_packet(&temp_ptr);
      }
      tdsnum_sdus_to_discard--;

    } /*while tdsnum_sdus_to_discard > 0 */
  } /* if() */
  else
  {
    TDSRLC_MSG0(MSG_LEGACY_HIGH,"WM is empty");
  }
} /* tdsrlc_ul_remove_sdus_from_wm() */

/*===========================================================================

FUNCTION RLC_UL_PURGE_UPLINK_WM

DESCRIPTION
  This function purges the SDUs from the uplink WM for the logical channels
  specified in the UL command.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_purge_uplink_wm
(
  tdsrlc_ul_purge_wm_ind_type *ul_purge_wm_ind
)
{
  rlc_lc_id_type lc_id;
  uint8 ctl_blk_id, i;
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;
  rex_crit_sect_type *datapath_mutex = NULL;


  /*-------------------------------------------------------------------------
    For each logical channel specified, set the semaphore and then purge the 
    WM. This would prevent reporting of buffer status on those
    logical channels when an interrupt comes in for building the PDUs.
  -------------------------------------------------------------------------*/
  for (i=0; i < ul_purge_wm_ind->num_lc_id; i++)
  {
    lc_id = ul_purge_wm_ind->lc_id[i];

    /*-----------------------------------------------------------------------
      Get the index for the control block in the uplink table 
    -----------------------------------------------------------------------*/
    if(lc_id < TDSUE_MAX_UL_LOGICAL_CHANNEL &&
      (ctl_blk_id = tdsul_ctl_blk_id_tbl[lc_id]) != TDSINIT_CTL_BLK_ID &&
      (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
    {
      ctl_blk_ptr = &tdsuplink_table.ctrl_blk[ctl_blk_id];
      TDSRLC_MSG3(MSG_LEGACY_HIGH,"Purge SDUs frm WM, LC %d, id %d, num %d", lc_id, ctl_blk_id,
                                               ul_purge_wm_ind->num_sdus[i]);

      /*
        In UL-ISR, status of this variable is used as reference. So protect this 
        part of the code, under LW mutex.
      */
      datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(lc_id);
      ctl_blk_ptr->purge_wm = TRUE;
      TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
      
      tdsrlc_ul_remove_sdus_from_wm(ctl_blk_ptr, ul_purge_wm_ind->num_sdus[i]);
      datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(lc_id);
      ctl_blk_ptr->purge_wm = FALSE;
      TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"Invalid LC ID %d",lc_id);
    }
  } /* for */


} /* tdsrlc_ul_purge_uplink_wm() */


/*===========================================================================

FUNCTION CRLC_UL_ABORT_CIPHER_CONFIG

DESCRIPTION
  This function reverts back to the old ciphering configuration if an abort
  is received from RRC while applying the new configuration due to triggering
  of cell update in RRC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_abort_cipher_config
(
  tdsrlc_abort_cipher_config_type *abort_cipher_info
)
{
  int i = 0;
  uint16 index = 0;
  tdsrlci_ul_ctrl_blk_type *ctl_blk = NULL;
  rex_crit_sect_type *datapath_mutex = NULL;

  TDSL2_ACQ_DL_LOCK();
  
  for(i=0;i<abort_cipher_info->nchan;i++)
  {
    /*-----------------------------------------------------------------------
      Get the index to the control block from the logical channel id
      and the control block.
    -----------------------------------------------------------------------*/
    index = tdsul_ctl_blk_id_tbl[abort_cipher_info->lc_id[i]];
    ctl_blk = &tdsuplink_table.ctrl_blk[index];

    TDSRLC_MSG2(MSG_LEGACY_HIGH,"Abort UL Cipher Config for RbId: %d,LC Id: %d",ctl_blk->cipher.rb_id,abort_cipher_info->lc_id[i]);
    /*-----------------------------------------------------------------------
      Revert to the old ciphering configuration.
    -----------------------------------------------------------------------*/
    ctl_blk->cipher = ctl_blk->cipher_backup;

    /*-----------------------------------------------------------------------
      For rb id 2, the sequence number could have gone ahead since the time
      the new SMC was received, so ciphering parameters need to be updated. 
      For other rb id's this would not be the case since they were suspened.
    -----------------------------------------------------------------------*/
    if(ctl_blk->cipher.rb_id == 2)
    {
    /* 
      These parameters are used for ciphering UL-PDUs, so protect this code completely
    */
      datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(abort_cipher_info->lc_id[i]);
      switch (ctl_blk->cipher.cipher_mode)
      {
        /*-------------------------------------------------------------------
          If the ciphering was on before the new SMC, then re-calculate the
          count_c.
        -------------------------------------------------------------------*/
        case TDSCIPHER_ON:
          /*lint -e514 */
          ctl_blk->cipher.count = (((ctl_blk->cipher.count >> 12) +
            (ctl_blk->ctrl.am_ctrl.ul_state.vt_s < 
                                  (ctl_blk->cipher.count & 0xFFF))) << 12) |
              ctl_blk->ctrl.am_ctrl.ul_state.vt_s;
          /*lint +e514 */
          break;
               
        /*-------------------------------------------------------------------
          If the case was either Cipher mod or cipher wait, then check if the
          sequence number at which new ciphering configuration was to be 
          applied has passed or not. If it has, update the ciphering 
          parameters.
        -------------------------------------------------------------------*/
        case TDSCIPHER_MOD:
        case TDSCIPHER_WAIT:

          /*-----------------------------------------------------------------
            If two confiurations were pending (though it is very unlikely
            to happen) check if the sequence number
            has passed the activation seq no. for the second one.
          -----------------------------------------------------------------*/
          if(ctl_blk->cipher.two_pending)
          {
            if(TDSRLCI_SEQ_GE(ctl_blk->ctrl.am_ctrl.ul_state.vt_s,
                   ctl_blk->cipher.act_sn_new1, TDSRLC_AM_MODULUS))
            {
              /*-------------------------------------------------------------
                If the mode was cipher_mod, save the current config as old1,
                new as old and new1 as current.
              -------------------------------------------------------------*/
              if(ctl_blk->cipher.cipher_mode == TDSCIPHER_MOD)
              {
                ctl_blk->cipher.act_old_cfg1 = TRUE;
                ctl_blk->cipher.cipher_key_id_old1 = 
                                               ctl_blk->cipher.cipher_key_id;
                ctl_blk->cipher.cipher_algo_old1 = 
                                                 ctl_blk->cipher.cipher_algo;
                ctl_blk->cipher.count_old1 = ctl_blk->cipher.count;
                ctl_blk->cipher.act_sn_old1 = ctl_blk->cipher.act_sn;
              }

              /*--------------------------------------------------------------
                Make the new config as old one.
              --------------------------------------------------------------*/
              ctl_blk->cipher.cipher_key_id_old = 
                                           ctl_blk->cipher.cipher_key_id_new;
              ctl_blk->cipher.cipher_algo_old = 
                                           ctl_blk->cipher.cipher_algo_new;
              ctl_blk->cipher.act_sn_old = ctl_blk->cipher.act_sn_new;
              ctl_blk->cipher.count_old  = ctl_blk->cipher.act_sn_old | 
                                             (ctl_blk->cipher.hfn_new << 12);
              ctl_blk->cipher.act_old_cfg = TRUE;

              /*-------------------------------------------------------------
                Make the new1 config as the current config.
              -------------------------------------------------------------*/
              ctl_blk->cipher.cipher_key_id = 
                                          ctl_blk->cipher.cipher_key_id_new1;
              ctl_blk->cipher.cipher_algo = ctl_blk->cipher.cipher_algo_new1;
              ctl_blk->cipher.hfn = ctl_blk->cipher.hfn_new1;
              ctl_blk->cipher.act_sn = ctl_blk->cipher.act_sn_new1; 
              /*lint -e514 */
              ctl_blk->cipher.count = ctl_blk->ctrl.am_ctrl.ul_state.vt_s | 
               (((ctl_blk->ctrl.am_ctrl.ul_state.vt_s < ctl_blk->cipher.act_sn)+ 
                                    ctl_blk->cipher.hfn_new1) << 12);
              /*lint +e514 */

              /*--------------------------------------------------------------
                Set the ciphering mode to TDSCIPHER_ON and two_pending flag to 
                false.
              --------------------------------------------------------------*/
              ctl_blk->cipher.cipher_mode = TDSCIPHER_ON;
              ctl_blk->cipher.two_pending = FALSE;
              
              TDSRLC_MSG3(MSG_LEGACY_HIGH,"Cipher ON UL AM, LC %d, key_id %d, cnt %x",
                  ctl_blk->ctrl.am_ctrl.am_common->ul_data_id,
                  ctl_blk->cipher.cipher_key_id, ctl_blk->cipher.count);
            }
            /*---------------------------------------------------------------
              If vt_s lies between the activation seq no. of new and 
              activation seq no. of new1, then backup old config as old1,
              current as old and new as current.
            ---------------------------------------------------------------*/
            else if(TDSRLCI_SEQ_GE(ctl_blk->ctrl.am_ctrl.ul_state.vt_s,
                    ctl_blk->cipher.act_sn_new, TDSRLC_AM_MODULUS))
            {
              if(ctl_blk->cipher.cipher_mode == TDSCIPHER_MOD)
              {
                /*-----------------------------------------------------------
                  Back up old cipher config if present.
                -----------------------------------------------------------*/
                if (ctl_blk->cipher.act_old_cfg)
                {
                  ctl_blk->cipher.act_old_cfg1 = TRUE;
                  ctl_blk->cipher.cipher_key_id_old1 = 
                                           ctl_blk->cipher.cipher_key_id_old;
                  ctl_blk->cipher.cipher_algo_old1 = 
                                             ctl_blk->cipher.cipher_algo_old;
                  ctl_blk->cipher.count_old1 = ctl_blk->cipher.count_old;
                  ctl_blk->cipher.act_sn_old1 = ctl_blk->cipher.act_sn_old;
                }
                else
                {
                  ctl_blk->cipher.act_old_cfg = TRUE;
                }
                /*-----------------------------------------------------------
                  Backup the current config as old.
                -----------------------------------------------------------*/
                ctl_blk->cipher.cipher_key_id_old = 
                                               ctl_blk->cipher.cipher_key_id;
                ctl_blk->cipher.cipher_algo_old = 
                                                 ctl_blk->cipher.cipher_algo;
                ctl_blk->cipher.count_old = ctl_blk->cipher.count;
                ctl_blk->cipher.act_sn_old = ctl_blk->cipher.act_sn;
              }
              /*-------------------------------------------------------------
                Apply the new config and set it to the current config.
              -------------------------------------------------------------*/
              ctl_blk->cipher.cipher_key_id = 
                                           ctl_blk->cipher.cipher_key_id_new;
              ctl_blk->cipher.cipher_algo = ctl_blk->cipher.cipher_algo_new;
              ctl_blk->cipher.hfn = ctl_blk->cipher.hfn_new;
              ctl_blk->cipher.act_sn = ctl_blk->cipher.act_sn_new;
              /*lint -e514 */
              ctl_blk->cipher.count = ctl_blk->ctrl.am_ctrl.ul_state.vt_s | 
                (((ctl_blk->ctrl.am_ctrl.ul_state.vt_s < ctl_blk->cipher.act_sn)+ 
                                    ctl_blk->cipher.hfn) << 12);
              /*lint +e514 */
              TDSRLC_MSG3(MSG_LEGACY_HIGH,"Cipher 1st ON UL AM, LC %d, key_id %d, cnt %x",
                        ctl_blk->ctrl.am_ctrl.am_common->ul_data_id,
                       ctl_blk->cipher.cipher_key_id, ctl_blk->cipher.count);

              /*-------------------------------------------------------------
                Set the new1 configuration to new.
              -------------------------------------------------------------*/
              ctl_blk->cipher.cipher_key_id_new = 
                                          ctl_blk->cipher.cipher_key_id_new1;
              ctl_blk->cipher.cipher_algo_new = 
                                            ctl_blk->cipher.cipher_algo_new1;
              ctl_blk->cipher.hfn_new = ctl_blk->cipher.hfn_new1;
              ctl_blk->cipher.act_sn_new = ctl_blk->cipher.act_sn_new1;

              /*--------------------------------------------------------------
                Set the Cipher mode to TDSCIPHER_MOD and two_pending flag to 
                false.
              --------------------------------------------------------------*/
              ctl_blk->cipher.cipher_mode = TDSCIPHER_MOD;
              ctl_blk->cipher.two_pending = FALSE;
            }
          }
          /*-----------------------------------------------------------------
            If there were no two pending configurations present and VT_S
            has passed the activation seq no., then apply the new ciphering
            config.
          -----------------------------------------------------------------*/
          else
          {
            if(TDSRLCI_SEQ_GE(ctl_blk->ctrl.am_ctrl.ul_state.vt_s,
                 ctl_blk->cipher.act_sn_new, TDSRLC_AM_MODULUS))
            {
              if(ctl_blk->cipher.cipher_mode == TDSCIPHER_MOD)
              {
                /*-----------------------------------------------------------
                  Back up old cipher config.
                -----------------------------------------------------------*/
                if (ctl_blk->cipher.act_old_cfg)
                {
                  ctl_blk->cipher.act_old_cfg1 = TRUE;
                  ctl_blk->cipher.cipher_key_id_old1 = 
                                           ctl_blk->cipher.cipher_key_id_old;
                  ctl_blk->cipher.cipher_algo_old1 = 
                                             ctl_blk->cipher.cipher_algo_old;
                  ctl_blk->cipher.count_old1 = ctl_blk->cipher.count_old;
                  ctl_blk->cipher.act_sn_old1 = ctl_blk->cipher.act_sn_old;
                }
                else
                {
                  ctl_blk->cipher.act_old_cfg = TRUE;
                }
                /*-----------------------------------------------------------
                  Backup the current config as old.
                -----------------------------------------------------------*/
                ctl_blk->cipher.cipher_key_id_old = 
                                               ctl_blk->cipher.cipher_key_id;
                ctl_blk->cipher.cipher_algo_old = 
                                                 ctl_blk->cipher.cipher_algo;
                ctl_blk->cipher.count_old = ctl_blk->cipher.count;
                ctl_blk->cipher.act_sn_old = ctl_blk->cipher.act_sn;
              } 
              /*-------------------------------------------------------------
                Apply the new ciphering config.
              -------------------------------------------------------------*/
              ctl_blk->cipher.cipher_key_id = 
                                           ctl_blk->cipher.cipher_key_id_new;
              ctl_blk->cipher.cipher_algo = ctl_blk->cipher.cipher_algo_new;
              ctl_blk->cipher.hfn = ctl_blk->cipher.hfn_new;
              ctl_blk->cipher.act_sn = ctl_blk->cipher.act_sn_new;
              /*lint -e514 */
              ctl_blk->cipher.count = ctl_blk->ctrl.am_ctrl.ul_state.vt_s | 
               (((ctl_blk->ctrl.am_ctrl.ul_state.vt_s < ctl_blk->cipher.act_sn) + 
                                    ctl_blk->cipher.hfn_new) << 12);
              /*lint +e514 */
              /*-------------------------------------------------------------
                Set the mode to TDSCIPHER_ON.
              -------------------------------------------------------------*/
              ctl_blk->cipher.cipher_mode = TDSCIPHER_ON;
            }
          }
          break;
            
        case TDSNO_CIPHER: 
          break;

        default:
          TDSRLC_MSG0(MSG_LEGACY_HIGH,"Invalid Ciphering State");

      }
      TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    }
  }
  
  TDSL2_ACQ_DL_UNLOCK();
  
}


#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
/*===========================================================================
FUNCTION RLCI_UL_TX_NEW_WIN_SUFI

DESCRIPTION
  This function checks for the window sufi transmission requests and transmits
  the one that is minimum of all.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
/*===========================================================================

FUNCTION RLC_UL_NUM_ACTI_NON_SRB_AM_CH

DESCRIPTION
  Returns number of non SRB AM channels established.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlci_ul_num_acti_non_srb_am_ch(void)
{
  return tdsrlc_acti_non_srb_am_ch;
}
#endif //FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL



/*===========================================================================

FUNCTION RLCI_POST_RESET_STATUS_TIMER_CMD

DESCRIPTION
  This function post a RLC AM RESET timer command to L2 UL task to perform timer 
  resetting in UL-task context.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_post_reset_status_timer_cmd
(
  rlc_lc_id_type  rlc_id
)

{
  uint16 i;
  
  for (i=0; i<TDSUE_MAX_AM_ENTITY; i++)
  {
    /*-----------------------------------------------------------------------
      A duplicated reset timer request for this AM entity
    -----------------------------------------------------------------------*/
    if (tdsrlc_rst_satus_tmr_tbl[i] == rlc_id)
    {
      return;
    }
  }

  for (i=0; i<TDSUE_MAX_AM_ENTITY; i++)
  {
    if (tdsrlc_rst_satus_tmr_tbl[i] == TDSINIT_CTL_BLK_ID)
    {
      tdsrlc_rst_satus_tmr_tbl[i] = rlc_id;
      break;
    }
  }
  
  /*-------------------------------------------------------------------------
    Set TDSRLC_AM_RESET_REQUEST_SIG REX signal.
  -------------------------------------------------------------------------*/
  (void) rex_set_sigs( tds_l2_ul_get_tcb(), TDSRLC_AM_RESET_STS_TMR_REQ_SIG);

}

/*===========================================================================

FUNCTION RLC_PROCESS_STATUS_REPORT_REQ

DESCRIPTION
  MAC call this function to re-generate the status report which the 
  previous sent could be dropped by L1.
  The trigger is from L1 when it decides to trash PDUs from its 
  outgoing buffer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_process_status_report_req
(
  tdsrlc_status_report_req_type *rlc_status_report_req
)
{

  tdsrlci_ul_ctrl_blk_type *ul_ctl_blk = NULL;
  uint16 ctl_blk_id;
  uint16 i =0;
  rex_crit_sect_type *datapath_mutex = NULL;
  /*-------------------------------------------------------------------------
    Send a status report on all logical channels that have been indicated
    in the MAC HS reset command.
  -------------------------------------------------------------------------*/

  for(i = 0; i < (uint16)rlc_status_report_req->nchan; i++)
  {
    /* LC id assigned by RRC is within the range of TDSUE_MAX_UL_LOGICAL_CHANNEL */
    if (rlc_status_report_req->rlc_id_list[i] < TDSUE_MAX_UL_LOGICAL_CHANNEL &&
        (ctl_blk_id = (uint16)tdsul_ctl_blk_id_tbl[rlc_status_report_req->rlc_id_list[i]]) 
        != TDSINIT_CTL_BLK_ID &&
        (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
    {
      ul_ctl_blk = &tdsuplink_table.ctrl_blk[ctl_blk_id];

      if((ul_ctl_blk->lc_state == TDSUL_NULL_STATE) ||
         (ul_ctl_blk->lc_mode < UE_MODE_ACKNOWLEDGED)   )
      {
        TDSRLC_MSG1(MSG_LEGACY_ERROR,"No Status report of UL LC ID: %d",rlc_status_report_req->rlc_id_list[i]);
        continue;
      }

      TDSRLC_MSG1(MSG_LEGACY_HIGH,"Rx'd status report req for LC %d",rlc_status_report_req->rlc_id_list[i]);

    {
      tdsrlci_post_reset_status_timer_cmd(ul_ctl_blk->ctrl.am_ctrl.am_common->dl_data_id);
    }     

      datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(rlc_status_report_req->rlc_id_list[i]);
      ul_ctl_blk->ctrl.am_ctrl.ul_state.ack_pdu =
        ul_ctl_blk->ctrl.am_ctrl.ul_state.status_rpt = TRUE;
      TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"Invalid LC id,%d",rlc_status_report_req->rlc_id_list[i]);
    }
  }
}

/*===========================================================================

FUNCTION L1_L2_DSM_FREE_PACKET

DESCRIPTION
  The DSM item is freed if neither RLC nor L1 is using it.  If it is in use
  with either of these app_field of DSM packet is set to RLC_DI_INUSE_MASK.

 DEPENDENCIES
   None.

 RETURN VALUE
   None.

SIDE EFFECTS
   None.

 ===========================================================================*/
void tdsl1_l2_dsm_free_packet
(
  dsm_item_type **item_ptr
)
{

  if (*item_ptr == NULL)
  {
    TDSRLC_MSG0(MSG_LEGACY_HIGH,"PDU ptr is NULL");
  }
  else
  {
    if((*item_ptr)->app_field & L2_DSM_INUSE_MASK)
    {
      (*item_ptr)->app_field &= ~L2_DSM_INUSE_MASK;
      *item_ptr = NULL;
    }
    else
    {
      dsm_free_packet(item_ptr);
    }
  }
}


/*===========================================================================

FUNCTION  RLC_GET_SRB_UL_CIPH_STATUS

DESCRIPTION

  Response to RRC's query for ciphering status.

DEPENDENCIES
  None

RETURN VALUE
  TDSRLC_NO_CIPHER_CFG_PENDING, no ciphering config pending.
  TDSRLC_CIPHER_CFG_PENDING, yes, there is at least one.
  TDSRLC_CIPHER_STATUS_UNSPECIFIED, can not find the LC.

SIDE EFFECTS
  None.

===========================================================================*/
tdsrlc_cipher_cfg_pending_e_type tdsrlc_get_srb_ul_ciph_status
(
  rlc_lc_id_type  rlc_id,       /* RLC LC ID for SRB2                     */
  uint32          *vt_s,        /* Current VT(S) of UL SRB2               */
  uint32          *act_sn,      /* Act SN of pendinf config,if there is   */
                                /* something pending                      */ 
  uint32          *wm_size      /* WM size in terms of number of RLC PDUs */
)
{
  tdsrlci_ul_ctrl_blk_type *ul_ctrl_blk = NULL;
  uint16 ctl_blk_id;
  uint32 num_bytes;
  uint32 pdu_size_bytes;
  tdsrlc_cipher_cfg_pending_e_type pending_cipher_cfg = TDSRLC_NO_CIPHER_CFG_PENDING;
  rex_crit_sect_type *datapath_mutex = NULL;

  if (rlc_id < TDSUE_MAX_UL_LOGICAL_CHANNEL &&
      (ctl_blk_id = (uint16)tdsul_ctl_blk_id_tbl[rlc_id]) != TDSINIT_CTL_BLK_ID &&
      (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
  {
    ul_ctrl_blk = &tdsuplink_table.ctrl_blk[ctl_blk_id];

    datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(rlc_id);
    /* Put RLC under config pending state. This prevent PDU building 
       interrupt to change the WM state when RLC calculates suspend 
       offset */
    /*lint -save -e655 */
    ul_ctrl_blk->lc_state |= TDSUL_CONFIG_PENDING;
    /*lint -restore */
    TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);

    if (ul_ctrl_blk->cipher.cipher_mode == TDSCIPHER_WAIT ||
        ul_ctrl_blk->cipher.cipher_mode == TDSCIPHER_MOD)
    {
      pending_cipher_cfg = TDSRLC_CIPHER_CFG_PENDING;
      if (ul_ctrl_blk->cipher.two_pending)
      {
        *act_sn = ul_ctrl_blk->cipher.act_sn_new1;
      }
      else
      {
        *act_sn = ul_ctrl_blk->cipher.act_sn_new;
      }
    }
    *vt_s = ul_ctrl_blk->ctrl.am_ctrl.ul_state.vt_s;
    num_bytes = ul_ctrl_blk->ul_wm_ptr->current_cnt +
                ((uint32)ul_ctrl_blk->ul_wm_ptr->q_ptr->cnt *
                 ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length) +
                (ul_ctrl_blk->special_ind ?
                 ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0) +
                (ul_ctrl_blk->octet_less ?
                 ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0);
    if (NULL != ul_ctrl_blk->cur_sdu_ptr)
    {
      num_bytes += ((ul_ctrl_blk->cur_sdu_len - ul_ctrl_blk->cur_sdu_tx_len) +
                  ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length);
    }
    datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(rlc_id);
    /* Has done the suspend offset calculating. Lift the bar. */
    /*lint -e64 -e641 */
    ul_ctrl_blk->lc_state &= (~TDSUL_CONFIG_PENDING);
    /*lint +e64 +e641 */
    TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    
    /* the rlc_size is stored in bits. */
    pdu_size_bytes = ((ul_ctrl_blk->ctrl.am_ctrl.ul_cnfg_var->ul_rlc_size - 16) >> 3);
    *wm_size = (num_bytes / pdu_size_bytes);
    if ((num_bytes % pdu_size_bytes) > 0)
    {
      (*wm_size) += 1;
    }
  }
  else
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"SRB is not config'd,%d",rlc_id);
    pending_cipher_cfg = TDSRLC_CIPHER_STATUS_UNSPECIFIED;
  }

  return pending_cipher_cfg;
}

/*===========================================================================

FUNCTION RLC_RRC_GET_LATEST_PENDING_HFN_REQ

DESCRIPTION
   This function is called by RRC to get latest pending HFN for indicated 
   logical channel ID.  
   RLC populates the info to the struct tdsrlc_pending_hfn_req_type.
   If there is a pending configuration for UL alone then 
   dl_pending_hfn_present flag will be set to FALSE and vice-versa.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/FAILURE:  If either dl_pending_hfn_present or ul_pending_hfn_present
                    flag is set to TRUE then function will return SUCCESS.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type tdsrlc_rrc_get_latest_pending_hfn
(
  rlc_lc_id_type rlc_id,                /* queried RLC logical channel ID */
  tdsrlc_pending_hfn_req_type *pending_hfn
)
{
  tdsrlci_ul_ctrl_blk_type *ul_ctrl_blk = NULL;
  uint16 ctl_blk_id;

  /* initial HFN presenting flag. */
  pending_hfn->dl_pending_hfn_present = FALSE;
  pending_hfn->ul_pending_hfn_present = FALSE;

  if (rlc_id < TDSUE_MAX_UL_LOGICAL_CHANNEL &&
      (ctl_blk_id = (uint16)tdsul_ctl_blk_id_tbl[rlc_id]) != TDSINIT_CTL_BLK_ID &&
      (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
  {
    ul_ctrl_blk = &tdsuplink_table.ctrl_blk[ctl_blk_id];

    if (ul_ctrl_blk->cipher.cipher_mode == TDSCIPHER_WAIT || 
        ul_ctrl_blk->cipher.cipher_mode == TDSCIPHER_MOD)
    {
      pending_hfn->ul_pending_hfn = (ul_ctrl_blk->cipher.two_pending) ?
        ul_ctrl_blk->cipher.hfn_new1 : ul_ctrl_blk->cipher.hfn_new;
      pending_hfn->ul_pending_hfn_present = TRUE;
    }
  }
  else
  {
    TDSRLC_MSG1(MSG_LEGACY_HIGH,"Invalid RLC UL LC,%d",rlc_id);
  }

  tdsrlci_get_dl_latest_pending_hfn    (rlc_id, pending_hfn);

  /* Either dl_pending_hfn_present or ul_pending_hfn_present
     flag is set to TRUE then function will return SUCCESS. 
   */
  if (pending_hfn->dl_pending_hfn_present || pending_hfn->ul_pending_hfn_present)
  {
    return SUCCESS;
  }
  else
  {
    return FAILURE;
  }
}

/*===========================================================================

FUNCTION RLC_RRC_OVERWRITE_HFN

DESCRIPTION
   This function is called by RRC to overwrite the HFN of RLC current using.

   This function will be called after RRC re-establish RLC to overwrite the 
   new HFN that RRC just comes up before the traffic can be resumed.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/FAILURE:  Done successful / no such LC or no ciphering at all for
                    this LC.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type tdsrlc_rrc_overwrite_hfn
(
  rlc_lc_id_type rlc_id,                /* queried RLC logical channel ID */
  uint32 tdshfn,
  tdsrlc_reestablish_e_type direction
)
{
  uecomdef_status_e_type ul_ret = FAILURE;
  uecomdef_status_e_type dl_ret = FAILURE;
  tdsrlci_ul_ctrl_blk_type *ul_ctrl_blk = NULL;
  uint16 ctl_blk_id;
  rex_crit_sect_type *datapath_mutex = NULL;

  /* Perform UL AM overwrite */
  if (direction == TDSRLC_RE_ESTABLISH_DL_UL || direction == TDSRLC_RE_ESTABLISH_UL)
  {
    datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(rlc_id);
    if (rlc_id < TDSUE_MAX_UL_LOGICAL_CHANNEL &&
        (ctl_blk_id = (uint16)tdsul_ctl_blk_id_tbl[rlc_id]) != TDSINIT_CTL_BLK_ID &&
        (ctl_blk_id < TDSUE_MAX_UL_RLC_ENTITY))
    {
      ul_ctrl_blk = &tdsuplink_table.ctrl_blk[ctl_blk_id];

      if (ul_ctrl_blk->lc_mode == UE_MODE_ACKNOWLEDGED &&
          ul_ctrl_blk->cipher.cipher_mode == TDSCIPHER_ON)
      {
        ul_ctrl_blk->cipher.hfn = tdshfn;
        ul_ctrl_blk->cipher.count = tdshfn << 12;
        ul_ret = SUCCESS;
      }
    }
    TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  }

  /* Need to overwrite the DL AM as well. */
  if (direction == TDSRLC_RE_ESTABLISH_DL_UL || direction == TDSRLC_RE_ESTABLISH_DL)
  {
    dl_ret = tdsrlci_dl_overwrite_hfn             (rlc_id, tdshfn);
  }

  /* SUCCESS is 0; FAILURE is 1.
     Only both are FAILURE, then return FAILURE.
  */
  return (((ul_ret == FAILURE) && (dl_ret == FAILURE)) ? FAILURE : SUCCESS);
}

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLC_START_UL_LOGGING

DESCRIPTION
   This function will start logging AM/UM PDUs.

DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_start_ul_logging ()
{

   /*-------------------------------------------------------------------------
    There is data in at least one channel, and the log mask is turned on
    logging the first 4 bytes of each PDU being transmitted.
  -------------------------------------------------------------------------*/
  if(tdslogging_params[TDSRLCLOG_SIG_PDU][TDSRLCLOG_CH_IDX] &&
                        log_status(TDSCDMA_UL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET))
  {
    tdsrlc_ul_tx_am_sig_plane_pdu_log_pkt(&tdsuplink_table,
                              tdslogging_params[TDSRLCLOG_SIG_PDU][TDSRLCLOG_CH_IDX]);
  }
  if(tdslogging_params[TDSRLCLOG_USR_PDU][TDSRLCLOG_CH_IDX] &&
                       log_status(TDSCDMA_UL_RLC_AM_USR_PLANE_PDU_LOG_PACKET))
  {
    tdsrlc_ul_tx_am_usr_plane_pdu_log_pkt(&tdsuplink_table,
                              tdslogging_params[TDSRLCLOG_USR_PDU][TDSRLCLOG_CH_IDX]);
  }

  tdslogging_params[TDSRLCLOG_SIG_PDU][TDSRLCLOG_TYPE_IDX] =
                                    TDSCDMA_UL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET;
  tdslogging_params[TDSRLCLOG_USR_PDU][TDSRLCLOG_TYPE_IDX] =
                                    TDSCDMA_UL_RLC_AM_USR_PLANE_PDU_LOG_PACKET;
  tdslogging_params[TDSRLCLOG_SIG_PDU][TDSRLCLOG_CH_IDX] = 0;
  tdslogging_params[TDSRLCLOG_USR_PDU][TDSRLCLOG_CH_IDX] = 0;

#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
  if(tdsrlc_ul_channel_cnt !=0)
  {
    tdsrlc_ul_tx_um_pdu_log_pkt(&tdsuplink_table, tdsrlc_ul_channel_cnt);
    tdsrlc_ul_channel_cnt = 0;
  }
#endif

  /*-------------------------------------------------------------------------
    PDU ciphering logging is on and there is logging data.
    The logging data is filled in at the time when a PDU is built. It is
    because that the ciphering algorithm is not passed to MAC/L1. Once the
    pdu_cipher_enable flag is off, the cipher parameters stored in the
    pdu_data structure may not be the correct information.
  -------------------------------------------------------------------------*/
  if (tdsrlci_ul_pdu_cipher_log.num_pdu)
  {
    tdsrlc_ul_tx_pdu_cipher_log_pkt(&tdsrlci_ul_pdu_cipher_log);
  }


}
#endif

/*===========================================================================

FUNCTION CRLC_UL_CONTINUE_HANDLER

DESCRIPTION
  Continue transmission on one or more uplink unacknowledged or acknowledged
  mode channels.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_ul_continue_handler
(
  tdsrlc_ul_continue_type *continue_ptr      /* Ptr to continue Info                */
)
{
  uint8 index;                        /* Index into Downlink Table         */
  int   i;
  rex_crit_sect_type *datapath_mutex = NULL;


  for (i = 0; i < continue_ptr->nchan; i++)
  {
    index = tdsul_ctl_blk_id_tbl[continue_ptr->channel[i]];
    /*lint -save -e64 -e641 -e655 */
    if (index != TDSINIT_CTL_BLK_ID &&
        (index < TDSUE_MAX_UL_RLC_ENTITY) &&
        (tdsuplink_table.ctrl_blk[index].lc_state & TDSUL_STOP))
    {
    /*
      Before restoring the lc-state, wait till ISR completes
    */
      datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(continue_ptr->channel[i]);
      /*lint -save -e64 -e641 -e655 */
      tdsuplink_table.ctrl_blk[index].lc_state &= (~TDSUL_STOP);
      tdsuplink_table.ctrl_blk[index].lc_state |= TDSUL_DATA_TRANSFER_READY;
      /*lint -restore */
      TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
      
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"UL LC- %d Continue",continue_ptr->channel[i] );
    }
    /*lint -restore */
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"LC %d not in Stop State",continue_ptr->channel[i]);
    }
   }


}/* tdscrlc_ul_continue_handler() */

/*===========================================================================

FUNCTION tdsrlc_get_num_set

DESCRIPTION
    - This function gets the nak status indicating whether we have any naks
      pending or not.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  nak_bitmap - pointer to bitmap array
  fsn        - fist sequence number from where to count.
  lsn        - last sequence number till which to count.
  mod        - modulus to use..

RETURN VALUE
  number of 1's in bitmap array

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_get_num_set
(
  uint16                *nak_bitmap,
  uint16                fsn,
  uint16                lsn,
  uint16                mod
)
{
  uint8             cnt = 0;
  uint16            last_sn = 0, num_one = 0;
  tdsrlc_16bit_map_type  bitmap;
  tdsrlc_bitmap_type   sn_bitmap;
/*------------------------------------------------------------------------*/

  if ( !TDSRLCI_SEQ_GE( lsn, fsn, TDSRLC_AM_MODULUS ) )
  {
    return 0xFFFF;
  }

  memset( &sn_bitmap, 0, sizeof(tdsrlc_bitmap_type));
  memset( &bitmap, 0, sizeof(tdsrlc_16bit_map_type));

  /*------------------------------------------------------------------------
   Loop and update status from fn to lsn..
  ------------------------------------------------------------------------*/
  do
  {
    /*----------------------------------------------------------------------
     We store pdu status as an array of 16 bit values, where each bit
     corresponds to a pdu sn. We work on one 16-bit value at a time.

     get the last sn which the 16-bit value having fsn corresponds to. 
    ----------------------------------------------------------------------*/
    cnt = TDSRLCI_GET_AM_COUNT_STATUS_OPT_BITMAP(fsn);
    last_sn = ( fsn + cnt ) & (TDSRLC_AM_MODULUS - 1);  

    /*----------------------------------------------------------------------
      If lsn is greater than the last sn  which the 16-bit value having
      fsn corresponds to (last_lsn), do the processing from fsn to 
      last_lsn, update fsn to point to next sn after last_lsn.
    ----------------------------------------------------------------------*/
    if ( !TDSRLCI_SEQ_GT( lsn, last_sn, TDSRLC_AM_MODULUS ) )
    {
      last_sn = lsn;
    }
    /*----------------------------------------------------------------------
      Get the 16 bit bitmap forsn from fsn to lsn..
      copy this to bit based structure and sum up the bits to get the number
      of 1's in the 16 bit value..
    ----------------------------------------------------------------------*/
  /*lint -e502 */
    sn_bitmap.var = TDSRLCI_GET_AM_SN_RANGE_NAK_STATUS( nak_bitmap, fsn, last_sn, mod );
  /*lint +e502 */

    bitmap = sn_bitmap.bit_map;

    if (sn_bitmap.var)
    {
      num_one +=( bitmap.bit0 + bitmap.bit1 + bitmap.bit2 + bitmap.bit3 +
              bitmap.bit4 + bitmap.bit5 + bitmap.bit6 + bitmap.bit7 + 
        bitmap.bit8 + bitmap.bit9 + bitmap.bit10 + bitmap.bit11 + 
        bitmap.bit12 + bitmap.bit13 + bitmap.bit14 + bitmap.bit15 );
    }

    fsn = TDSRLCI_AM_SEQ_INC( last_sn );

  }while (last_sn != lsn);

  return num_one;
}

/*===========================================================================

FUNCTION tdsrlc_get_retx_bitmap

DESCRIPTION
    - This function gets the retx bitmap structure maintained on UL.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  lc_id      - logical channel id.

RETURN VALUE
  retx bitmap - pointer to retx bitmap structure.

SIDE EFFECTS
  None.

===========================================================================*/
tdsrlc_opt_status_type *tdsrlc_get_retx_bitmap
(
  byte                  lc_id  
)
{
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr;
/*------------------------------------------------------------------------*/

  ctl_blk_ptr = tdsrlci_get_ctl_blk_ptr(lc_id);

  if (ctl_blk_ptr == NULL)
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"ctrl_blk_ptr is NULL for LC id %d",lc_id);
    return NULL;
  }

  return( &ctl_blk_ptr->ctrl.am_ctrl.ul_status_opt_bitmap );
}


/*===========================================================================

FUNCTION RLC_UL_GET_UL_HDR

DESCRIPTION

  Returns the pointer to the global header buffer ul_hdr.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
tdsrlc_ul_hdr_buff_type* tdsrlc_get_ul_hdr(void)
{
  return (&tdsrlc_ul_hdr);
} 

/*===========================================================================

FUNCTION RLC_UL_UPDATE_HDR

DESCRIPTION

  Stores the RLC PDU(AM/UM) header, LIs into the local buffer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_update_hdr
(
  uint8         length,     /* 1 -> 8-bit, 2 -> 16-bit */
  uint16        value,      /* AM PDU/UM PDU/LI header */
  tdsrlc_buff_type *hdr_list   /* Pointer to headers      */
)
{
  if (hdr_list->num_bytes > 0)
  {
    /*-----------------------------------------------------------------------
      LI is being stored. Set the E/HE bit of previous hdr before 
      updating the LI
    -----------------------------------------------------------------------*/
    hdr_list->buff[hdr_list->num_bytes - 1] |= TDSRLC_HDR_E_MASK;
  }
  
  if (length == TDSRLC_8_BIT_HDR)
  {
    hdr_list->buff[hdr_list->num_bytes++] = (uint8)value;
  }
  else
  {
    hdr_list->buff[hdr_list->num_bytes++] = ((value & 0xFF00) >> 8);
    hdr_list->buff[hdr_list->num_bytes++] = (value & 0x00FF);
  }

} /* tdsrlc_ul_update_hdr */


/*===========================================================================

FUNCTION RLC_DSM_PACK_DATA

DESCRIPTION

  This will pack the amount of bits from the specified byte & bit position
  in the data_ptr of the PDu

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dsm_pack_data
(
  dsm_item_type *pdu_ptr,   /* Pointer to the PDU dsm item    */
  uint32        pack_data,  /* Data to pack                   */
  uint16        bit_pos,    /* Bit position from the 1st byte */
  uint8         data_len    /* No. of bits to pack            */
)
{
  uint8   temp      = 0xFF;
  uint8   n_bits    = 0;
  uint8   rem_bits  = data_len;
  uint16  byte_pos;

  byte_pos = (uint16)(bit_pos / 8);
  bit_pos &= 0x07;
  
  while (rem_bits > 0)
  {
    if ((bit_pos != 0)|| (rem_bits >=8))
    {
      if(rem_bits < 8)
      {
        n_bits =(8-bit_pos);
        pdu_ptr->data_ptr[byte_pos] &= ((temp << n_bits)|(temp >> (8-n_bits+rem_bits)));
        pdu_ptr->data_ptr[byte_pos] |= ((uint8)(pack_data) << (n_bits-rem_bits));
        n_bits = rem_bits;
      }
      else
      {
        n_bits = (8 - bit_pos);
        pdu_ptr->data_ptr[byte_pos] &= (temp << n_bits); 
        pdu_ptr->data_ptr[byte_pos] |= (uint8)(pack_data >> (rem_bits - n_bits));
      }
      
      bit_pos = 0;
    }
    else if (rem_bits < 8)
    {
      n_bits = rem_bits;
      pdu_ptr->data_ptr[byte_pos] &= (temp >> n_bits);
      pdu_ptr->data_ptr[byte_pos] |= (uint8)(pack_data << (8 - n_bits));
    }
    
    byte_pos++;
    rem_bits-= n_bits;    
  }

} /* tdsrlc_dsm_pack_data */

/*===========================================================================

FUNCTION RLC_UL_FORM_IO_VEC_PER_SDU

DESCRIPTION

  Parses thru the SDU dsm chain to identify the offset with in it, and 
  forms IO vectors for the dsm items corresponding to the PDU payload

DEPENDENCIES
  None.

RETURN VALUE
  Amount of data from this SDU goes as payload to PDU.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_ul_form_io_vec_per_sdu
(
  dsm_item_type       *dsm_ptr,          /* Start dsm_ptr of the SDU chain */
  uint16              offset,            /* Offset with the SDU chain      */
  uint16              payload_len,       /* Amount of data for the PDU     */
  tdsl1_l2_data_type    *l1_ul_tx_buf_ptr  /* Place to store the IO vectors  */
)
{
  uint32                    len         = 0;
  uint16                    tx_data;

  tx_data = payload_len;

  /*-------------------------------------------------------------------------
    Find the dsm_item in the sdu chain, where the payload starts.
  -------------------------------------------------------------------------*/
  while ((dsm_ptr != NULL) && (offset >= dsm_ptr->used))
  {
    offset -= dsm_ptr->used;
    dsm_ptr = dsm_ptr->pkt_ptr;
  }
  
  /*-------------------------------------------------------------------------
    Construct the IO vectors for the DSM items in the same SDU, which are
    as part of this PDU payload.
  -------------------------------------------------------------------------*/
  while ((dsm_ptr != NULL) && (payload_len > 0))
  {
    len = MIN((dsm_ptr->used - offset), payload_len);

    if (0 == len)
    {
      TDSRLC_MSG3(MSG_LEGACY_ERROR, "RLC_ERR: Invalid len, skip filling IOVEC, used %d offst %d pyld_len %d", 
                  dsm_ptr->used, offset, payload_len);
    }
    else
    {
    /*-----------------------------------------------------------------------
      Store the IO vector for this dsm_item in __data_seg_buf[]
    -----------------------------------------------------------------------*/
      tdsrlc_ul_fill_io_vec((dsm_ptr->data_ptr+offset), len, l1_ul_tx_buf_ptr, FALSE);
    }
    
    dsm_ptr = dsm_ptr->pkt_ptr;
    payload_len -= (uint16)len;
    offset = 0;
  }

  return (tx_data - payload_len);
} /* tdsrlc_ul_form_io_vec_per_sdu */

/*===========================================================================

FUNCTION RLC_UL_FORM_NON_EDCH_IO_VEC_PER_SDU

DESCRIPTION

  Parses thru the SDU dsm chain to identify the offset with in it, and 
  forms IO vectors for the dsm items corresponding to the PDU payload

DEPENDENCIES
  None.

RETURN VALUE
  Amount of data from this SDU goes as payload to PDU.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_ul_form_non_edch_io_vec_per_sdu
(
  dsm_item_type       *dsm_ptr,          /* Start dsm_ptr of the SDU chain */
  uint16              offset,            /* Offset with the SDU chain      */
  uint16              payload_len        /* Amount of data for the PDU     */
)
{
  uint32              len = 0;
  uint16              tx_data;

  tx_data = payload_len;

  /*-------------------------------------------------------------------------
    Find the dsm_item in the sdu chain, where the payload starts.
  -------------------------------------------------------------------------*/
  while ((dsm_ptr != NULL) && (offset >= dsm_ptr->used))
  {
    offset -= dsm_ptr->used;
    dsm_ptr = dsm_ptr->pkt_ptr;
  }
  
  /*-------------------------------------------------------------------------
    Construct the IO vectors for the DSM items in the same SDU, which are
    as part of this PDU payload.
  -------------------------------------------------------------------------*/
  while ((dsm_ptr != NULL) && (payload_len > 0))
  {
    len = MIN((dsm_ptr->used - offset), payload_len);

    /*-----------------------------------------------------------------------
      Store the IO vector for this dsm_item in tdsrlc_io_vec_list
    -----------------------------------------------------------------------*/
    tdsrlc_io_vec_list.io_vec[tdsrlc_io_vec_list.num_io_vecs].ptr = 
                                              dsm_ptr->data_ptr+offset;
    tdsrlc_io_vec_list.io_vec[tdsrlc_io_vec_list.num_io_vecs].len = len;
    tdsrlc_io_vec_list.num_io_vecs++;
    
    dsm_ptr = dsm_ptr->pkt_ptr;
    payload_len -= (uint16)len;
    offset = 0;
  }

  return (tx_data - payload_len);
} /* tdsrlc_ul_form_non_edch_io_vec_per_sdu */

/*===========================================================================

FUNCTION RLC_UL_GET_PRE_ALLOC_PDU_DSM

DESCRIPTION

  Provides the next available DSM item for the pre allocated pool for
  building the R99 PDUs.

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to the available DSM item form the pool other wise NULL pointer.

SIDE EFFECTS
  None.

===========================================================================*/
dsm_item_type* tdsrlc_ul_get_pre_alloc_pdu_dsm(void)
{
  dsm_item_type *item_ptr = NULL; 

  if (tdsrlc_pdu_dsm_pool.num_item < TDSMAX_UL_PDUS_PER_TTI)
  {
    item_ptr = tdsrlc_pdu_dsm_pool.item_ptr[tdsrlc_pdu_dsm_pool.num_item++];
    item_ptr->used = 0;
    
    /*-----------------------------------------------------------------------
     Set this pre-allocated DSM item to IN_USE.
     L1 will not free this item, but turn off the IN_USE bit.
     Only UM & AM PDU built for non edch path use this logic.
     TM PDU will be freed by L1 as usually for both EDCH/NON_EDCH path.
    -----------------------------------------------------------------------*/
    item_ptr->app_field |= L2_DSM_INUSE_MASK;
  }

  return (item_ptr);
} /* tdsrlc_ul_get_pre_alloc_pdu_dsm */

/*===========================================================================

FUNCTION RLC_UL_COPY_IO_VEC_DATA

DESCRIPTION

  Copies the data from the set of IO vectors into the pre allocated DSM item
  while building R99 pdus.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_copy_io_vec_data
(
  dsm_item_type *item_ptr,  /* Pointer to the pre allocated DSM item       */
  uint16         offset      /* Offset with in the DSM item, from where the */
                            /* data copy should begin                      */
)
{
  uint8   i;
  uint16  length = offset;
  
  for (i = 0; i < tdsrlc_io_vec_list.num_io_vecs; i++)
  {
    /*-----------------------------------------------------------------------
     Copy the payloads stored as IO vectors into this PDU.
    -----------------------------------------------------------------------*/    
    /*memcpy((item_ptr->data_ptr + length), (tdsrlc_io_vec_list.io_vec[i].ptr),
                                          (tdsrlc_io_vec_list.io_vec[i].len));*/
    //We don't know the buffer length of dsm->data_ptr
    memscpy((item_ptr->data_ptr + length), (tdsrlc_io_vec_list.io_vec[i].len), (tdsrlc_io_vec_list.io_vec[i].ptr),
                                          (tdsrlc_io_vec_list.io_vec[i].len));
    item_ptr->used += (uint16)tdsrlc_io_vec_list.io_vec[i].len;
    
    length += (uint16)tdsrlc_io_vec_list.io_vec[i].len;
  }
  
} /* tdsrlc_ul_copy_io_vec_data */

/*===========================================================================

FUNCTION RLC_UL_INIT_IO_VEC_LIST

DESCRIPTION

  Intialising global io_vec_list before storing the set IO vector for 
  while building R99 AM new/re-tx, UM PDUs.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_init_io_vec_list(void)
{
  tdsrlc_io_vec_list.num_io_vecs = 0;
} /* tdsrlc_ul_init_io_vec_list */

/*===========================================================================

FUNCTION RLC_UL_FILL_IO_VEC

DESCRIPTION

  Stores the IO vector into the L1 tx buffer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_fill_io_vec
( 
  uint8             *tdsdata_ptr,          /* Source pointer                 */
  uint32            len,                /* No. of bytes                   */
  tdsl1_l2_data_type  *l1_ul_tx_buf_ptr,  /* Buffer to place IO vector      */
  boolean           last_io_vec         /* TRUE-Last IO vector of this PDU */
                                        /* FALSE- Not the lat Io vector    */
)
{
    if ((l1_ul_tx_buf_ptr->num_src_desc > 0) && 
        (tdsdata_ptr == 
         (l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc - 1].data_seg_list_ptr +
          l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc - 1].length)))
    {
      /*-----------------------------------------------------------------------
       Optimizing IO vectors, by check if this data is continuous with the 
       previous IO vector data.
      -----------------------------------------------------------------------*/  
      l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc - 1].length += len;
    }
    else
    {
      l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc].data_seg_list_ptr = tdsdata_ptr;

      /*-----------------------------------------------------------------------
       Provide length in bytes for  7200 I/F & PP-HW.
      -----------------------------------------------------------------------*/ 
      l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc].length = len;
      
      if (last_io_vec)
      {
        /*-----------------------------------------------------------------------
          Set this as last IO vectors for this PDU.
        -----------------------------------------------------------------------*/      
        TDSSET_PDU_LAST_IO_VEC(l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc].length);
      }
      
      l1_ul_tx_buf_ptr->num_src_desc++;
    }

} /* tdsrlc_ul_fill_io_vec */

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLC_UL_INIT_AM_LOG_PARAMS

DESCRIPTION

 Initalizes the tdslogging_params for the AM PDUs.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_init_am_log_params(void)
{
  /*-------------------------------------------------------------------------
    This array stores the logging parameter info, the type of PDU packet
    user plane or signalling plane and no. of active channels of each
    type of packet.
  -------------------------------------------------------------------------*/
  tdslogging_params[TDSRLCLOG_SIG_PDU][TDSRLCLOG_TYPE_IDX] = 
                                    TDSCDMA_UL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET;

  tdslogging_params[TDSRLCLOG_USR_PDU][TDSRLCLOG_TYPE_IDX] = 
                                    TDSCDMA_UL_RLC_AM_USR_PLANE_PDU_LOG_PACKET;

  tdslogging_params[TDSRLCLOG_SIG_PDU][TDSRLCLOG_CH_IDX] = 0;

  tdslogging_params[TDSRLCLOG_USR_PDU][TDSRLCLOG_CH_IDX] = 0;

  tdsrlci_ul_pdu_cipher_log_on = log_status(TDSCDMA_UL_RLC_PDU_CIPHER_LOG_PACKET);

} /* tdsrlc_ul_init_logging */

/*===========================================================================

FUNCTION RLC_UL_CHECK_AM_LOG_STATUS

DESCRIPTION

  Checks if the user plane/signalling plane PDU logging is turned on or not.

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE  if PDU needs to be logged
            FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_check_am_log_status
(
  tdsrlci_ul_am_ctrl_type    *am_ctrl_ptr, /* Pointer to ul_chan_am_pdu   */
  rlc_lc_id_type          rlc_id,       /* RLC ID                      */
  uint16                  pdu_size      /* RLC PDU SIZE                */
)
{
  uint8   log_index = 0;
  boolean found     = FALSE;
  
  /*---------------------------------------------------------------
    If LC ID is for user plane AM channels, store the info if
    user plane PDU logging is turned on, otherwise store the info
    if the signaling plane PDU logging is turned on.
  ---------------------------------------------------------------*/
  if((rlc_id >= TDSRLC_UL_AM_USR_PLANE_START_LC_ID) && 
     log_status(TDSCDMA_UL_RLC_AM_USR_PLANE_PDU_LOG_PACKET))
  {
    log_index = 1;
    tdslogging_params[log_index][TDSRLCLOG_CH_IDX] ++;
  }

  if((rlc_id < TDSRLC_UL_AM_USR_PLANE_START_LC_ID) && 
     log_status(TDSCDMA_UL_RLC_AM_SIG_PLANE_PDU_LOG_PACKET))
  {
    log_index = 0;
    tdslogging_params[log_index][TDSRLCLOG_CH_IDX] ++;
  }

  if((log_status(tdslogging_params[log_index][TDSRLCLOG_TYPE_IDX])) && 
                (tdslogging_params[log_index][TDSRLCLOG_CH_IDX] > 0))
  {
    found = TRUE;
  
    /*---------------------------------------------------------------
     Store pdu size for logging
    ---------------------------------------------------------------*/
    am_ctrl_ptr->ul_chan_am_pdu.pdu_size = pdu_size;
    am_ctrl_ptr->ul_chan_am_pdu.num_pdu  = 0;
  }
  if (tdsrlc_debug_var & TDSRLC_DBG_UL_CIPHERING)
  {
    TDSRLC_MSG2(MSG_LEGACY_HIGH,"Cipher: LC %d,Found %d",found,rlc_id);
  }

  return found;

} /* tdsrlc_ul_am_check_log_status */

#endif /* FEATURE_TDSCDMA_RLC_QXDM_LOGGING */

#if  0   // for no invoke
/*===========================================================================

FUNCTION TDSRLC_ENH_INDICATE_UL_DATA_TRASH

DESCRIPTION
  L1 indicates the information about PDUs which are trashed due to Config
  related procedures. L1 discards UL data if built during reconfiguration
  to speed up the reconfig time.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_indicate_ul_data_trash(tdsl1_ul_tx_data_type *l1_ul_tx_buf_ptr)
{
  int32 trch_idx, tb_id;
  uint32 num_tb;
  tdsl1_ul_tb_data_type  *trch_data_ptr;
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr;
  
  for (trch_idx = 0; trch_idx < TDSL1_UL_TRCH_MAX; trch_idx++)
  {
    num_tb = l1_ul_tx_buf_ptr->tx_data[trch_idx].num_tb;
    
    if  ((num_tb > 0) && (l1_ul_tx_buf_ptr->tx_data[trch_idx].tb_size > 0))
    {
      trch_data_ptr = &(l1_ul_tx_buf_ptr->tx_data[trch_idx].trch_data[0]);
      
      for (tb_id = 0; tb_id < num_tb; tb_id++)
      {
        TDSRLC_MSG3(MSG_LEGACY_HIGH,"RLC_ERR: NA: Dropping RLC PDU - tb_id %d, rb_id %d, ciph_on %d",
                  tb_id, trch_data_ptr[tb_id].radio_bearer_id, trch_data_ptr[tb_id].ciphering_on);
        
        /* If AM Control PDUs are trashed during L1 Reconfig, 
         *  then 
         *     RLC should remember this info to send after the reconfig.
         *     If there is no change in RLC Context, CONTROL will go out
         *     If RLC entity is reestablished, CONTROL will not go
         */
      
        if (trch_data_ptr[tb_id].ciphering_on == FALSE)
        {
          ctl_blk_ptr = tdsrlci_get_ctl_blk_ptr_am_rb_id(trch_data_ptr[tb_id].radio_bearer_id);
          if ((ctl_blk_ptr != NULL) &&
              ((trch_data_ptr[tb_id].data_ptr->data_ptr[0] & 0x80) == FALSE))
          {
            /* 
             * This is an AM CONTROL PDU and getting discarded
             * Set the Need to send Control information in RLC entity 
             */
            tdsrlci_rx_status_changed(ctl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id, TRUE);
            TDSRLC_MSG3(MSG_LEGACY_HIGH,"RLC_ERR: NA: Setting Need to send Status flag - tb_id %d, rb_id %d, first_word_pdu %d",
                      tb_id, trch_data_ptr[tb_id].radio_bearer_id, 
                      tdsrlc_copy_to_word(trch_data_ptr[tb_id].data_ptr->data_ptr));
          }
        }
      } 
    }
    else
    {
      MSG_HIGH("RLC_ERR: NA: No trashing for trch %d, num_tb %d, tb_size %d",
                trch_idx, num_tb, l1_ul_tx_buf_ptr->tx_data[trch_idx].tb_size);      
    }
  } 
}
#endif
/*===========================================================================

FUNCTION RLC_UL_BUILD_NON_EDCH_PDU

DESCRIPTION

  Builds the R99 PDU using new io vector method of building PDUs

DEPENDENCIES
  None.

RETURN VALUE
  boolean: TRUE  - R99 pdu successfully build
           FASLE - R99 pdu building failed

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_ul_build_non_edch_pdu
(
  tdsrlc_buff_type             *hdr_list,   /* Pointer to the headers         */
  tdsrlc_ul_pdu_data_type      *tdspdu_info,   /* valid if chnl_type = NON_E_DCH */
  uint16                    padding_len  /* PDU padding length             */
  ,uint32                   key_index,   /* Ciphering key_id               */
  boolean                   enabled,     /* TRUE - cipheing applicable     */
                                         /* FALSE - otherwise              */
  uecomdef_umts_cipher_algo_e_type cipher_algo,
  uint32                    cnt_c        /* COUNT_C parameter              */
)
{
  dsm_item_type *pdu_ptr  = NULL;

  /*---------------------------------------------------------------------
    This is a non E-DCH channel. Select a dsm_item from the 
    pre-allocated pdu_dsm_pool.
  ---------------------------------------------------------------------*/
  pdu_ptr = tdsrlc_ul_get_pre_alloc_pdu_dsm();
  if (pdu_ptr == NULL)
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"RLC_ERR: Pre allocated DSM not available");
    return FALSE;
  }

  /*---------------------------------------------------------------------
   Copy the headers formed into this PDU.
  ---------------------------------------------------------------------*/
  //memcpy(pdu_ptr->data_ptr, hdr_list->buff, hdr_list->num_bytes);
  //We don't know the buffer length of dsm->data_ptr
  memscpy(pdu_ptr->data_ptr, hdr_list->num_bytes, hdr_list->buff, hdr_list->num_bytes);  
  pdu_ptr->used = hdr_list->num_bytes;
  
  tdsrlc_ul_copy_io_vec_data(pdu_ptr, hdr_list->num_bytes);

  pdu_ptr->used += padding_len;

  tdspdu_info->pdu_ptr = pdu_ptr;

  tdspdu_info->pdu_cipher_enable = enabled;
  tdspdu_info->cipher_algo = cipher_algo;
  tdspdu_info->cipher_key_id = key_index;
  tdspdu_info->count_c = cnt_c;

  return TRUE;
} /* tdsrlc_ul_build_non_edch_pdu */

/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_PDUS

DESCRIPTION
  
  Builds uplink E-DCH PDUs for transmission in a particular TTI. In EDCH, 
  there is no TM PDUs.
  This function will be called by MAC and runs in interrupt context.

  PP.SW:

  RLC PDU payloads are provided as a set of IO vectors which will be 
  copied into the data section of the ERAM using mDM.
  
  Headers including MAC-es, TSN, SI, RLC HDR(+LIs) are stored in a header
  buffer as word format and is provided as a single IO vector. These
  are copied into the header section of ERAM using burst interface.
  
  7200A:

  Headers including MAC-es, TSN, SI, RLC HDR(+LIs), RLC PDU payloads are
  all provided as set of IO vectors. These are copied into the ERAM using
  burst interface.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_ul_enh_build_pdus
(
  tdsrlc_ul_frame_format_type *ul_fmt_ptr /* Pointer to frame format data */
)
{
  uint16                               i;
  uint16                               num_words;
  rlc_lc_id_type                      ctrl_index;
  tdsrlci_ul_ctrl_blk_type              *ctrl_blk_ptr;
  tdsrlc_ul_logchan_format_type         *chan_fmt_ptr;
  tdsrlc_ul_hdr_buff_type               *ul_hdr_ptr      = tdsrlc_get_ul_hdr();
  
  uint32                             data_sec_len_bytes;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  boolean found = FALSE;
  boolean log_updated = FALSE;
 
  tdsrlc_ul_init_am_log_params();
#endif // FEATURE_TDSCDMA_RLC_QXDM_LOGGING

  /*-------------------------------------------------------------------------
    Initialize ul_hdr position & l1_ul_tx_buf_ptr for this TTI.
  -------------------------------------------------------------------------*/
  ul_hdr_ptr->num_valid_words = 0;

  ul_fmt_ptr->l1_ul_tx_buf->num_src_desc = 0;
#ifdef FEATURE_TDSCDMA_DSDA
  ul_fmt_ptr->l1_ul_tx_buf->srb_data_present = FALSE;
#endif
  /*-------------------------------------------------------------------------
    Write the MAC-e header into the ul_hdr
  -------------------------------------------------------------------------*/
  num_words = TDSCONVERT_BITS_TO_WORDS(ul_fmt_ptr->mac_e_hdr_size);

  /* When only SI needs to be send then mac_e_hdr length would be zero */
  if (num_words != 0)
  {
    for (i = 0; num_words > 0; num_words--, i += 4)
    {
      if (((i + 3) < TDSMAX_MAC_E_HDR_SIZE_BYTES) && (ul_hdr_ptr->num_valid_words < TDSMAX_HDR_BUFF_SIZE))
      {
        if(ul_hdr_ptr->num_valid_words < TDSMAX_HDR_BUFF_SIZE)
        {
          ul_hdr_ptr->buff[ul_hdr_ptr->num_valid_words++] = 
                              tdsrlc_copy_to_word(&(ul_fmt_ptr->mac_e_hdr[i]));
        }
        else
        {
          TDSRLC_MSG0(MSG_LEGACY_ERROR,"Number of valid fields in ul_hdr_ptr->buff[] exceed TDSMAX_HDR_BUFF_SIZE");
        }
      }
      else
      {
        TDSRLC_MSG2(MSG_LEGACY_HIGH,"mac_e_hdr len %d exceeded MAX %d",i,TDSMAX_MAC_E_HDR_SIZE_BYTES);
      }
    }
  
    /*-------------------------------------------------------------------------
      Trigger L1 to build the Copy Engine Control Word for MAC-e header
    -------------------------------------------------------------------------*/
    if(tdsrlc_debug_var & TDSRLC_DBG_UL_PP_HW)
    {    
      TDSRLC_MSG3(MSG_LEGACY_HIGH,"Copy Eng->Mac_E_Hdr: hdr_len_bits %d, data_len_bits %d, offset_len_bits 0, Mac_E_Hdr 0x%x", 
                ul_fmt_ptr->mac_e_hdr_size, 0, ul_fmt_ptr->mac_e_hdr[0]);
    }
  
    tdsmac_rlc_build_copy_engine_ctrl_info(ul_fmt_ptr->mac_e_hdr_size, 0, 0);
  

    /*-------------------------------------------------------------------------
      Trigger L1 to build the Ciphering Engine Control Word for MAC-e header.
      Note: MAC-e header is not ciphered.
    -------------------------------------------------------------------------*/
    tdsmac_rlc_build_cipher_engine_ctrl_info(ul_fmt_ptr->mac_e_hdr_size, 0, 
                                         UE_UMTS_CIPHER_ALGO_NONE, 0, 0,
                                         //L1_RLC_CIPHERING_ALGO_NONE, 0, 0,
                                       FALSE, FALSE, TDSRLC_INVALID_COUNT_C);
  }

  /*------------------------------------------------------------------------
    Free UM SDUs transmitted in the previous TTI and initialize the queue.
  -------------------------------------------------------------------------*/
  tdsrlc_um_free_sdu_q(TDSRLC_E_DCH_CHNL);

  /*------------------------------------------------------------------------
    Initialize the control PDU DSM buffer.
  -------------------------------------------------------------------------*/
  tdsrlc_ul_init_ctrl_dsm_pool();

  /*-------------------------------------------------------------------------
    Loop until all the logical channels are processed
  -------------------------------------------------------------------------*/
  for (i = 0; i < ul_fmt_ptr->nchan; i++)
  {
    ctrl_index = tdsul_ctl_blk_id_tbl[ul_fmt_ptr->chan_fmt[i].rlc_id];
    ctrl_blk_ptr = &(tdsuplink_table.ctrl_blk[ctrl_index]);
    chan_fmt_ptr = &(ul_fmt_ptr->chan_fmt[i]);

    /*-----------------------------------------------------------------------
      If the LC ID exceeds the maximum allowed channel ID, or if the LC in 
      NULL_STATE or Uplink WM is not registered move to the next channel.
    -----------------------------------------------------------------------*/
    if (chan_fmt_ptr->rlc_id >= TDSUE_MAX_UL_LOGICAL_CHANNEL) 
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: Enh Build PDU,Invalid LC %d",chan_fmt_ptr->rlc_id);
      continue;
    }

    if (ctrl_blk_ptr->lc_state == TDSUL_NULL_STATE)
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: Enh Build PDU,LC %d in NULL_STATE %d",chan_fmt_ptr->rlc_id);
      continue;
    }

    if (ctrl_blk_ptr->ul_wm_ptr == NULL)
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: Enh Build PDU,LC %d WM unregistered",ul_fmt_ptr->chan_fmt[i].rlc_id);
      continue;
    }
#ifdef FEATURE_TDSCDMA_DSDA
    if ((UE_LOGCHAN_DCCH == ctrl_blk_ptr->lc_type) ||
        (UE_LOGCHAN_CCCH == ctrl_blk_ptr->lc_type))
    {
      ul_fmt_ptr->l1_ul_tx_buf->srb_data_present = TRUE;
    }
#endif

    switch (ctrl_blk_ptr->lc_mode)
    {
      case UE_MODE_ACKNOWLEDGED:
        if (tdsrlc_debug_var & TDSRLC_DBG_PDU_BLDG)
        {
          TDSRLC_MSG3(MSG_LEGACY_HIGH,"UL AM LC %d, Num PDUs %d, PDU size %d", 
                   chan_fmt_ptr->rlc_id,
                   chan_fmt_ptr->npdus, chan_fmt_ptr->pdu_size);
        }

        /*-------------------------------------------------------------------
         If data PDUs are being built or going to be built on UL AM user 
         plane channels, set the activity flag to TDSDATA_TRANSFERRED.
        -------------------------------------------------------------------*/
        if(ctrl_blk_ptr->ctrl.am_ctrl.ul_state.vt_s !=
            ctrl_blk_ptr->ctrl.am_ctrl.ul_state.vt_a)
        {
          TDSRLC_SET_DATA_ACTIVITY_FLAG(chan_fmt_ptr->rlc_id, 
                    TDSRLC_UL_AM_USR_PLANE_START_LC_ID);
        }

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
        found = FALSE;
        if (!(ctrl_blk_ptr->ul_pdu_logging))
        {   
          found = tdsrlc_ul_check_am_log_status(&(ctrl_blk_ptr->ctrl.am_ctrl),
                                             chan_fmt_ptr->rlc_id,
                                             chan_fmt_ptr->pdu_size);
          log_updated |= found;          
        }

        
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING

        tdsrlc_ul_enh_build_am_pdus(ctrl_blk_ptr, chan_fmt_ptr, 
                                 ul_fmt_ptr->l1_ul_tx_buf
                                 #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                                 , found
                                 #endif
                                 );

#ifdef FEATURE_QSH_EVENT_METRIC
		//Incrementing the QSH Number of PDUs TX for this cycle.
		tdsrlc_qsh_metrics.num_pdu_bytes_tx += ((chan_fmt_ptr->pdu_size >> 3)* (chan_fmt_ptr->npdus));
		TDSRLC_MSG1(MSG_LEGACY_MED,"TDS_QSH: Incrementing the PDU built count by %d bytes  ",((chan_fmt_ptr->pdu_size >> 3)* (chan_fmt_ptr->npdus))); 
#endif

      break;

      case UE_MODE_UNACKNOWLEDGED:
        
        if (tdsrlc_debug_var & TDSRLC_DBG_PDU_BLDG)
        {
          TDSRLC_MSG3(MSG_LEGACY_HIGH,"UL UM LC %d, npdus %d, pdu_size %d",
                   chan_fmt_ptr->rlc_id,
                   chan_fmt_ptr->npdus, chan_fmt_ptr->pdu_size);
        }

        tdsrlc_ul_enh_build_um_pdus(ctrl_blk_ptr, chan_fmt_ptr, 
                                 ul_fmt_ptr->l1_ul_tx_buf);

        /*-------------------------------------------------------------------
          If traffic is sent on user plane channels, set the activity
          flag to TDSDATA_TRANSFERRED.
        -------------------------------------------------------------------*/
        TDSRLC_SET_DATA_ACTIVITY_FLAG(chan_fmt_ptr->rlc_id, 
                    TDSRLC_UL_UM_USR_PLANE_START_LC_ID);


#if defined(FEATURE_TDSCDMA_RLC_QXDM_LOGGING) && defined(FEATURE_TDSCDMA_UM_PDU_LOGGING)
        if (!(ctrl_blk_ptr->ul_pdu_logging) && 
            log_status(TDSCDMA_UL_RLC_UM_USR_PLANE_PDU_LOG_PACKET))
        {
          /*-------------------------------------------------------------------
            Store the logging related inofrmation. PDU logging data is 
            stored in UM PDU building.
          -------------------------------------------------------------------*/
          log_updated = TRUE;
          tdsrlc_ul_channel_cnt++;

          /* PDU logging data is stored in UM PDU building */
          ctrl_blk_ptr->ctrl.um_ctrl.ul_chan_um_pdu.pdu_size =
                                    chan_fmt_ptr->pdu_size;
        }

#endif //FEATURE_TDSCDMA_UM_PDU_LOGGING


      break;

      default:
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"RLC_ERR: Enh Build PDU: Invalid Mode LC %d",chan_fmt_ptr->rlc_id);

    } /* End switch */
  } /* End for */

  if (ul_fmt_ptr->si_size > 0)
  {
    /*-----------------------------------------------------------------------
      Trigger L1 to build the Copy Engine Control Word for SI field.
    -----------------------------------------------------------------------*/    
    if(tdsrlc_debug_var & TDSRLC_DBG_UL_PP_HW)
    {    
       TDSRLC_MSG3(MSG_LEGACY_HIGH,"Copy Eng->SI Hdr: hdr_len_bits %d, data_len_bits %d, offset_len_bits 0, SI_Hdr 0x%x",
                 ul_fmt_ptr->si_size, 0, ul_fmt_ptr->si);
    }
      
    tdsmac_rlc_build_copy_engine_ctrl_info(ul_fmt_ptr->si_size, 0, 0);


    /*-----------------------------------------------------------------------
      Write the SI field into the ul_hdr.
    -----------------------------------------------------------------------*/
    if(ul_hdr_ptr->num_valid_words < TDSMAX_HDR_BUFF_SIZE)
    {
      ul_hdr_ptr->buff[ul_hdr_ptr->num_valid_words++] = ul_fmt_ptr->si;
    }
    else
    {
      TDSRLC_MSG0(MSG_LEGACY_ERROR,"Number of valid fields in ul_hdr_ptr->buff[] exceed TDSMAX_HDR_BUFF_SIZE");
    }

    /*-----------------------------------------------------------------------
      Trigger L1 to build the Ciphering Engine Control Word for SI field.
      Note: MAC-e header is not ciphered.
    -----------------------------------------------------------------------*/    
    tdsmac_rlc_build_cipher_engine_ctrl_info(ul_fmt_ptr->si_size, 0, 
                                         UE_UMTS_CIPHER_ALGO_NONE, 0, 0,
                                         //L1_RLC_CIPHERING_ALGO_NONE, 0, 0,
                                         FALSE, FALSE, TDSRLC_INVALID_COUNT_C);
  }

  /*-------------------------------------------------------------------------
    Trigger L1 to build the Ciphering Engine Control Word for 
    padding/last_uint_indicator.
  -------------------------------------------------------------------------*/
  if (ul_fmt_ptr->padding_size > 0)
  {
    tdsmac_rlc_build_cipher_engine_ctrl_info(ul_fmt_ptr->padding_size, 0, 
                                       UE_UMTS_CIPHER_ALGO_NONE, 0, 0,
                                       //L1_RLC_CIPHERING_ALGO_NONE, 0, 0,
                                       TRUE, FALSE, TDSRLC_INVALID_COUNT_C);
  } 

  /*-------------------------------------------------------------------------
    Trigger L1 to copy the headers stored in ul_hdr into the header section
    of the ERAM bank. L1 needs the hdr section data in bytes.
  -------------------------------------------------------------------------*/
  if(tdsrlc_debug_var & TDSRLC_DBG_UL_PP_HW)
  {
    uint16 tmp;
    TDSRLC_MSG1(MSG_LEGACY_HIGH,"Write Hdr Sec: Total hdr bytes: %d",(ul_hdr_ptr->num_valid_words << 2));

    /* Array boundary check, the code dumps 3-bytes at a time*/
    for(tmp = 0; ((tmp < ul_hdr_ptr->num_valid_words) && ((tmp + 3) < TDSMAX_HDR_BUFF_SIZE));
        tmp += 3)
    {
      TDSRLC_MSG3(MSG_LEGACY_HIGH,"Write Hdr Sec: Hdr 0x%x, Hdr 0x%x, Hdr 0x%x",
               ul_hdr_ptr->buff[tmp], ul_hdr_ptr->buff[tmp+1], 
               ul_hdr_ptr->buff[tmp+2]);
    }
  }

  if (ul_hdr_ptr->num_valid_words > TDSMAX_HDR_BUFF_SIZE)
  {
    TDSRLC_MSG2(MSG_LEGACY_ERROR,"RLC_ERR: no. of valid words %d exceeded max %d,limiting to MAX",ul_hdr_ptr->num_valid_words,TDSMAX_HDR_BUFF_SIZE);
    ul_hdr_ptr->num_valid_words = TDSMAX_HDR_BUFF_SIZE;
  }
  
  (void)tdsmac_rlc_write_hdr_section((ul_hdr_ptr->num_valid_words << 2), (uint8 *)ul_hdr_ptr->buff);


  /*-------------------------------------------------------------------------
    Calculate the no. of bytes of all IO vectors.
  -------------------------------------------------------------------------*/
  for(i = 0, data_sec_len_bytes = 0; i < ul_fmt_ptr->l1_ul_tx_buf->num_src_desc; i++)
  {
    data_sec_len_bytes +=  TDSGET_IO_VEC_LEN(ul_fmt_ptr->l1_ul_tx_buf->cur_ul_buf_ptr[i].length);
  }

  /*-------------------------------------------------------------------------
    Trigger L1 to copy the RLC Payloads provided as IO vectors into the 
    data section of ERAM bank.
  -------------------------------------------------------------------------*/
  if(tdsrlc_debug_var & TDSRLC_DBG_UL_PP_HW)
  {
  	TDSRLC_MSG2(MSG_LEGACY_HIGH,"Write Data Sec: total bytes %d,total src_desc %d",data_sec_len_bytes,ul_fmt_ptr->l1_ul_tx_buf->num_src_desc);
  }
  

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (log_updated || tdsrlci_ul_pdu_cipher_log.num_pdu)
  {
    (void) rex_set_sigs( tds_l2_ul_get_tcb(), TDSRLC_UL_START_LOGGING_SIG);
  }
#endif /* FEATURE_TDSCDMA_RLC_QXDM_LOGGING */

} /* tdsrlc_ul_enh_build_pdus */

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION tdsrlc_ul_statistic_rlc_buffer_status

DESCRIPTION
  This function is executed in RLC. This is called to get rlc buffer status.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  tdsrlc_ul_statistic_rlc_buffer_status
(
  rlc_lc_id_type rlc_id, /* rlc id */
  uint32  nbytes         /* buffer bytes */
)
{
  tdsrlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;

	ctl_blk_ptr = tdsrlci_get_ctl_blk_ptr(rlc_id);

	if (ctl_blk_ptr == NULL)
	{
	  return;
	}

	ctl_blk_ptr->ctrl.am_ctrl.ul_chan_am_stat.ul_am_buf_status = (uint16)nbytes;
}
#endif
#ifdef FEATURE_Q6_MT
/*===========================================================================
FUNCTION: TDSL2_ACQ_UL_LW_LOCK

DESCRIPTION:
  API to acquire UL data path lock based on RLC LC id passed.

DEPENDENCIES:
  None

RETURN VALUE:
  Mutex Locked.

SIDE EFFECTS:
  None
===========================================================================*/
rex_crit_sect_type* TDSL2_ACQ_UL_LW_LOCK(rlc_lc_id_type rlc_id)
{
  tdsrlc_ul_channel_type channel_type = TDSCHANNEL_MAX;

   /* If LC ID is >= TDSUE_MAX_UL_LOGICAL_CHANNEL lock both EUL and R99 */
  if (rlc_id >= TDSUE_MAX_UL_LOGICAL_CHANNEL)
  {
    REX_ISR_LOCK(&tdse_ul_isr_l2_datapath_mutex);
    REX_ISR_LOCK(&tdsr99_ul_isr_l2_datapath_mutex);
    return NULL;  
  }

  /* Get channel type from MAC */
  channel_type = tdsmac_ul_get_channel_type(rlc_id);
  if (TDSCHANNEL_MAX == channel_type)
  {
    return NULL;
  }
  else if (TDSCHANNEL_DCH == channel_type)
  {                                         
    REX_ISR_LOCK(&tdsr99_ul_isr_l2_datapath_mutex);
    return (&tdsr99_ul_isr_l2_datapath_mutex);  
  }
  else
  {                                       
    REX_ISR_LOCK(&tdse_ul_isr_l2_datapath_mutex);  
    return (&tdse_ul_isr_l2_datapath_mutex);
  }
}

/*===========================================================================
FUNCTION: TDSL2_ACQ_UL_LW_UNLOCK

DESCRIPTION:
  API to release UL data path lock based on RLC LC id passed.

DEPENDENCIES:
  None

RETURN VALUE:
  NONE

SIDE EFFECTS:
  None
===========================================================================*/ 
void TDSL2_ACQ_UL_LW_UNLOCK(rex_crit_sect_type *datapath_mutex)
{

  if (NULL != datapath_mutex)
  {
    REX_ISR_UNLOCK(datapath_mutex);
  }
  return;
}
#endif

/*===========================================================================

FUNCTION RLC_COPY_to_WORD

DESCRIPTION
  This function copies data from uint8 pointer into a word(uint32) byte-by-byte
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 tdsrlc_copy_to_word(uint8 *src_ptr)
{
  uint32 data_word = 0xDEADDEAD;
  uint8 *word_ptr = (uint8 *) &data_word;
  
  if(src_ptr != NULL)
  {
    *(word_ptr++) = *(src_ptr++);
    *(word_ptr++) = *(src_ptr++);
    *(word_ptr++) = *(src_ptr++);
    *(word_ptr++) = *(src_ptr++);
  }
  
  return data_word;
}

/*===========================================================================

FUNCTION RLCI_GET_ACTIVE_UL_UM_CHNLS

DESCRIPTION
  This function returns the number of active UM channels 
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 tdsrlci_get_active_ul_um_chnls (void)
{
  uint32 act_ul_um_chnl = 0;
  uint32 i;

  for(i = 0; (i < tdsuplink_table.active_ch); i++)
  {
      if(UE_MODE_UNACKNOWLEDGED == tdsuplink_table.ctrl_blk[i].lc_mode)
      {
        act_ul_um_chnl++;
      }
  }

  return act_ul_um_chnl;
}

#ifdef FEATURE_QXDM_DSM_TRACING
/*===========================================================================

FUNCTION RLC_FC_GET_DSM_TRACE_CB

DESCRIPTION
  This function returns the number of active UM channels 
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_fc_get_dsm_trace_cb(
    uint32 cb_data, 
    fc_dsm_trace_info_type *wm_stat_ptr)
{
  uint16 i;
  dsm_mempool_id_type pool_id;
  if (wm_stat_ptr == NULL) 
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"RLC_ERR::NULL wm statistic ptr");
    return;
  }
  if( (cb_data != 0xDEADBEEF ) ||
      (wm_stat_ptr->tech_id != DSM_UMTS_TECH) ||
      (wm_stat_ptr->client_id != UMTS_RLC_CLIENT ))
  {
    MSG_ERROR ("RLC_ERR::Incorrect CB params %x %d %d", 
              cb_data,
              wm_stat_ptr->tech_id,
              wm_stat_ptr->client_id);
    return;
  }
  wm_stat_ptr->num_pools = FC_MAX_POOL_ID;


  for( i=0; i<FC_MAX_POOL_ID; i++)
  {
    switch(i)
    {
    case FC_LARGE_POOL:
      pool_id = DSM_DS_LARGE_ITEM_POOL;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.few_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_FEW) ;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.many_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_MANY) ;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.dne_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_LINK_LAYER_DNE) ;
      break;
    case FC_SMALL_POOL:
      pool_id = DSM_DS_SMALL_ITEM_POOL;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.few_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_FEW) ;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.many_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_MANY) ;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.dne_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_LINK_LAYER_DNE) ;
      break;
      case FC_DUP_POOL:
      pool_id = DSM_DUP_ITEM_POOL;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.few_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_FEW) ;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.many_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_MANY) ;
      break;
    default:
      TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_ERR::Shouldnt happen");
      return;
    }
    wm_stat_ptr->dsm_counts[i].dsm_pool = (fc_dsm_pool_id_type)i;
    dsm_get_pool_stats(pool_id,
                       &(wm_stat_ptr->dsm_counts[i].dsm_pool_counts));
    /*memcpy(&(wm_stat_ptr->dsm_counts[i].dsm_event_hits),
           &(tdsrlc_fc_dsm_event_hits[i]),
           sizeof(fc_dsm_event_hits_stat_type));*/
    memscpy(&(wm_stat_ptr->dsm_counts[i].dsm_event_hits),
           sizeof(fc_dsm_event_hits_stat_type),
           &(tdsrlc_fc_dsm_event_hits[i]),
           sizeof(fc_dsm_event_hits_stat_type));
    memset(&(tdsrlc_fc_dsm_event_hits[i]), 0,
           sizeof(fc_dsm_event_hits_stat_type));
  }
}

#endif

#if 0 //CFCM_related_obsolete_3
/*===========================================================================

FUNCTION RLC_POST_FC_INTERNAL_CMD

DESCRIPTION
  This function post internal cmd to L@ UL task,
  so the fc cmd is processed in UL L2 task.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_post_fc_internal_cmd (
  tdsrlc_fc_cmd_entity_type entity,
  uint16 cmd)
{
  tdsrlc_fc_cmd_type *rlc_fc_cmd = NULL;
  if(entity ==  TDSRLC_FC_CMD_NULL_ENTITY)
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"RLC_ERR::NULL fc Entity");
    return;

  }
  rlc_fc_cmd = (tdsrlc_fc_cmd_type *)q_get(&tdsrlc_fc_free_cmd_q);
  if (rlc_fc_cmd == NULL)
  {
    TDSRLC_MSG2(MSG_LEGACY_ERROR,"RLC_ERR::Memory allocation failed in sending fc cmd %d for entity %d",cmd,entity);
    return;
  }
  rlc_fc_cmd->entity = entity;
  rlc_fc_cmd->cmd = cmd;
  (void) q_link(rlc_fc_cmd, &rlc_fc_cmd->link);
  q_put(&tdsrlc_fc_cmd_q, &rlc_fc_cmd->link);
  (void) rex_set_sigs(tds_l2_ul_get_tcb(), TDSRLC_FC_INTERNAL_CMD_SIG);
}

/*===========================================================================

FUNCTION RLC_FC_INTERNAL_CMD_HDLR

DESCRIPTION
  processing function of L2 UL task.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_fc_internal_cmd_hdlr (void)
{
  tdsrlc_fc_cmd_entity_type entity;
  uint16 cmd;
  tdsrlc_fc_cmd_type *cmd_ptr = NULL;
  uint8 i;
  uint16 ctl_blk_idx = 0;
  uint16 dl_fc_entity;

  rex_crit_sect_type *datapath_mutex = NULL;

  while ((cmd_ptr = (tdsrlc_fc_cmd_type *)(q_get(&tdsrlc_fc_cmd_q))) != NULL)
  {
    entity = cmd_ptr->entity;
    cmd = cmd_ptr->cmd;
    q_put( &tdsrlc_fc_free_cmd_q, &cmd_ptr->link);
    // act on the cmd
    dl_fc_entity = TDSRLC_DL_FC_ENTITY_MAX;
    if (entity == TDSRLC_FC_CMD_LARGE_DSM_ENTITY)
    {
      dl_fc_entity = (uint16)TDSRLC_DL_FC_ENTITY_DSM_LARGE;
    }
    if (entity == TDSRLC_FC_CMD_DUP_ENTITY)
    {
      dl_fc_entity = TDSRLC_DL_FC_ENTITY_DUP;
    }
    if (entity == TDSRLC_FC_CMD_SMALL_DSM_ENTITY)
    {
      dl_fc_entity = (uint16)TDSRLC_DL_FC_ENTITY_DSM_SMALL;
    }

    for (i=TDSRLC_UL_AM_USR_PLANE_START_LC_ID; i<TDSUE_MAX_UL_LOGICAL_CHANNEL; i++)
    {
      if ((ctl_blk_idx = tdsul_ctl_blk_id_tbl[i]) != TDSINIT_CTL_BLK_ID &&
          tdsuplink_table.ctrl_blk[ctl_blk_idx].lc_mode == UE_MODE_ACKNOWLEDGED &&
          tdsuplink_table.ctrl_blk[ctl_blk_idx].lc_state != TDSUL_NULL_STATE)
      {
        datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(tdsuplink_table.ctrl_blk[ctl_blk_idx].ctrl.am_ctrl.am_common->ul_data_id);
        tdsmulti_client_fc_arbitrate(&(tdsuplink_table.ctrl_blk[ctl_blk_idx].ctrl.am_ctrl.multiclient_dl_fc_arbitarator),
                                  dl_fc_entity,
                                  cmd,
                                  TRUE);
        TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
      }
    }
  } 
}
#endif //CFCM_related_obsolete_3

#ifdef FEATURE_QSH_EVENT_METRIC

/*===========================================================================
FUNCTION tdsrlc_send_qsh_cmd

DESCRIPTION
  Send local cmd TDSL2_QSH_CMD to L2 . it's called by the tds qsh timer registered callback
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrlc_send_qsh_cmd( qsh_client_cb_params_s *param )
{
  tdsl2_ul_cmd_type *rlc_cmd_ptr; /* local command pointer */

  /* send MSG to trigger log packet output */
  if ((rlc_cmd_ptr = tdsl2_ul_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("RLC_QSH::NULL Ptr when sending TDSL2_QSH_CMD ",0,0,0);
  }
  else
  {
    rlc_cmd_ptr->cmd_hdr.cmd_id= TDSRLC_QSH_CMD;
    rlc_cmd_ptr->cmd_data.tdsrlc_qsh_cmd.param= *param;
    tdsl2_ul_put_cmd( rlc_cmd_ptr );
	TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_QSH::Sent the QSH command to RLC");
    
  }
}




/*===========================================================================
FUNCTION tdsrlc_qsh_debug_callback

DESCRIPTION
This is the callback function for QSH to collect data from TDSRLC. The callback function will 
send a local command to L2-RLC to trigger the metric collection.
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrlc_qsh_debug_callback(qsh_client_cb_params_s *param)
{
  ASSERT(param != NULL);
  
  switch(param->action)
  {
    
#ifdef FEATURE_QSH_EVENT_METRIC

    case QSH_ACTION_METRIC_CFG:
    case QSH_ACTION_METRIC_TIMER_EXPIRY:
      
      tdsrlc_send_qsh_cmd(param);
      
      break;
#endif

#ifdef FEATURE_QSH_DUMP
    case QSH_ACTION_DUMP_COLLECT:
      /* placeholder for QSH DUMP support */
	// Add comments	
	  
    break;
#endif

    default:
      break;
  }
}

/*===========================================================================

FUNCTION TDSRLC_QSH_TC_METRICS_INIT

DESCRIPTION
  Initializing the metrics for test support
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_qsh_tc_metrics_init(void)
{


   qsh_client_metric_cfg_init(tdsrlc_qsh_metric_cfg_arr,TDSRLC_QSH_MAX_METRICS);
   
   tdsrlc_qsh_metric_cfg_arr[TDSRLC_QSH_METRIC_RLC_STAT].id = TDSRLC_QSH_METRIC_RLC_STAT;
   tdsrlc_qsh_metric_cfg_arr[TDSRLC_QSH_METRIC_RLC_STAT].sampling_period_ms = 1000;   
   tdsrlc_qsh_metric_cfg_arr[TDSRLC_QSH_METRIC_RLC_STAT].subs_id = 0;
   tdsrlc_qsh_metric_cfg_arr[TDSRLC_QSH_METRIC_RLC_STAT].fifo.element_size_bytes = sizeof(tdsrlc_qsh_metric_data_stats_s);
   tdsrlc_qsh_metric_cfg_arr[TDSRLC_QSH_METRIC_RLC_STAT].fifo.element_count_total = 10;
}


/*===========================================================================

FUNCTION TDSRLC_QSH_REGISTER

DESCRIPTION
  Registering TDS L2 for QSH metrics collection with QSH
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_qsh_register(void)
{
	qsh_client_reg_s tds_rlc_reg_req;
	qsh_client_reg_init(&tds_rlc_reg_req);

	//Adding some support for test:
	tdsrlc_qsh_tc_metrics_init();
    tds_rlc_reg_req.metric_info.metric_cfg_arr_ptr = &tdsrlc_qsh_metric_cfg_arr[0];
    tds_rlc_reg_req.metric_info.metric_cfg_count = TDSRLC_QSH_MAX_METRICS;
	
	
	tds_rlc_reg_req.client = QSH_CLT_TRLC;
	tds_rlc_reg_req.client_cb_ptr = tdsrlc_qsh_debug_callback;
	tds_rlc_reg_req.cb_action_support_mask = (QSH_ACTION_METRIC_CFG|QSH_ACTION_METRIC_TIMER_EXPIRY);
	tds_rlc_reg_req.major_ver = TDSRLC_QSH_MAJOR_VER;
	tds_rlc_reg_req.minor_ver = TDSRLC_QSH_MINOR_VER;
	qsh_client_reg(&tds_rlc_reg_req);
}


/*===========================================================================

FUNCTION tdsrlc_update_metric_address

DESCRIPTION
  This will update the local copy of next address and metric context recieved from QSH
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_qsh_write_metrics(uint32 time_exp_ms, sys_modem_as_id_e_type subs_id)
{

	//Add a NULL check here.
	tdsrlc_qsh_metrics.num_rlc_reset_dl = tdsrlc_qsh_dl_rlc_resets;
	tdsrlc_qsh_metrics.rlc_dl_tput = (tdsrlc_qsh_dl_tput*8000)/ time_exp_ms; //Providing the value in Bits/sec so multiplying by 8 (for bits conversion) and by 1000 for secs comversion.
	tdsrlc_qsh_metrics.rlc_ul_tput= (tdsrlc_qsh_metrics.num_pdu_bytes_tx*8000)/time_exp_ms;  //providing the value in Bits/sec
	memscpy((tdsrlc_qsh_metric_data_stats_s*)(tdsrlc_metric_address[subs_id].start_address), sizeof(tdsrlc_qsh_metric_data_stats_s), (tdsrlc_qsh_metric_data_stats_s*)&tdsrlc_qsh_metrics, sizeof(tdsrlc_qsh_metric_data_stats_s));
	TDSRLC_MSG3(MSG_LEGACY_MED,"RLC_QSH::Resets in Dl is %d and Ul is %d and unrecoverable error is %d  ",tdsrlc_qsh_metrics.num_rlc_reset_dl,tdsrlc_qsh_metrics.num_rlc_reset_ul, tdsrlc_qsh_metrics.num_rlc_urecov_err);
	TDSRLC_MSG2(MSG_LEGACY_MED,"RLC_QSH::num_pdu_bytes_tx is %d and num_pdu_bytes_retx is %d  ",tdsrlc_qsh_metrics.num_pdu_bytes_tx,tdsrlc_qsh_metrics.num_pdu_bytes_retx);
	TDSRLC_MSG2(MSG_LEGACY_MED,"RLC_QSH::rlc_dl_tput is %d and rlc_ul_tput is %d  ",tdsrlc_qsh_metrics.rlc_dl_tput,tdsrlc_qsh_metrics.rlc_ul_tput);
}


void tdsrlc_update_metric_address(qsh_metric_id_t metric_id, qsh_client_metric_context_id_t qsh_metric_context_id, sys_modem_as_id_e_type subs_id, uint8* qsh_address )
{

		if(qsh_address == NULL)
			{
				
				ERR_FATAL("RLC_QSH::Next address provided is NULL  ",0,0,0);
				
			}	

		switch(metric_id)                                                                           
  		{                                                                                 

    	case TDSRLC_QSH_METRIC_RLC_STAT:                                                   
 			{
     		tdsrlc_metric_address[subs_id].start_address = (uint8*)qsh_address;
			tdsrlc_metric_address[subs_id].rlc_qsh_metric_id = metric_id;
			tdsrlc_metric_address[subs_id].rlc_qsh_metric_cntxt_id = qsh_metric_context_id;
    		}
      	break;                                                                        
 
   	
    	//If newer metrics are required we can add here:                                                                        
 
    	default:                                                                        
 
      	break;                                                                        
 
  		}                 
		

}


/*===========================================================================

FUNCTION TDSRLC_QSH_COLLECT_REPORT_METRICS

DESCRIPTION
  This function collects the metrics on timer expiry callback. Writes the data on
  to the address provided and then notifies QSH of the data written.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_qsh_collect_report_metrics(qsh_client_cb_params_s* param)
{

	uint8* addr= NULL;
	uint8 index=0;  //i=0;
	uint8 tdsrlc_qsh_status=0;
	qsh_metric_id_t metric_id;
	qsh_client_metric_log_done_s  rlc_log_done;
  sys_modem_as_id_e_type subs_id;
  

	if(tdsuplink_table.active_ch > 0)
		{
			//for(i=0; i<TDSUE_MAX_UL_RLC_ENTITY;i++)
				//{
					//if(tdsuplink_table.ctrl_blk[i].lc_mode == UE_MODE_ACKNOWLEDGED)
						//{
								tdsrlc_qsh_status =1;
						//}
				//}
		}

	for(index=0; index < param->action_params.metric_timer_expiry.metric_id_count; index++)
	{
		metric_id = param->action_params.metric_timer_expiry.params[index].metric_id;  //Since we have to monitor only one metric in RLC, implicitly checking only first element of array. 
    subs_id = param->action_params.metric_timer_expiry.params[index].subs_id;
    
    if ((subs_id < SYS_MODEM_AS_ID_1) || (subs_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
    {
      TDSRLC_MSG1(MSG_LEGACY_MED,"RLC_QSH:: invalid as_id %d ",subs_id);
      continue;
    }
	if (subs_id != tdsrrc_get_as_id())
    {
      TDSRLC_MSG2(MSG_LEGACY_MED,"RLC_QSH:: mismatched as_id. qsh as_id %d and current as_id %d ",subs_id,tdsrrc_get_as_id());
      continue;
    }
		if((tdsrlc_qsh_metric_coll_status[subs_id] == TDSRLC_QSH_METRICS_ENABLED)&& (TDSRLC_QSH_METRIC_RLC_STAT ==metric_id) && (tdsrlc_qsh_status))
		{
			
			//Collect and report the data 
			TDSRLC_MSG0(MSG_LEGACY_MED,"RLC_QSH:: QSH called by L2-RLC for reporting the metrics ");
	#ifndef FEATURE_TDSCDMA_CFCM
			tdsrlc_qsh_metrics.rlc_fc_win_size = 0;
	#endif
			//copy tdsrlc_qsh_metrics to QSH provided memory
			tdsrlc_qsh_write_metrics(param->action_params.metric_timer_expiry.params[index].time_elapsed_ms, subs_id);
		

			//Notify QSH 
			qsh_client_metric_log_done_init(&rlc_log_done);
			rlc_log_done.client = QSH_CLT_TRLC;
			rlc_log_done.metric_id = metric_id;
			rlc_log_done.metric_context_id = tdsrlc_metric_address[subs_id].rlc_qsh_metric_cntxt_id;
			rlc_log_done.log_reason = QSH_CLIENT_METRIC_LOG_REASON_TIMER_EXPIRY;
			addr = qsh_client_metric_log_done(&rlc_log_done);
			tdsrlc_update_metric_address(metric_id,tdsrlc_metric_address[subs_id].rlc_qsh_metric_cntxt_id, subs_id, addr);

			//Clear the stats for next collection duration
			memset( &tdsrlc_qsh_metrics, 0, sizeof(tdsrlc_qsh_metric_data_stats_s) );
			tdsrlc_bad_status_pdu_cnt =0;
			tdsrlc_qsh_dl_rlc_resets = 0;
			tdsrlc_qsh_dl_tput =0;
			TDSRLC_MSG0(MSG_LEGACY_MED,"RLC_QSH:: In L2-RLC All the metrics and counters cleared for next duration ");
	

		}
		//Do this for other metrics if added in future
	  
	}
	

}

/*===========================================================================

FUNCTION tdsrlc_qsh_process_cfg

DESCRIPTION
  This will process the Metric cfg command from QSH. It also saves the start address
  and does a check on alloted size.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_qsh_process_cfg(qsh_client_cb_params_s* param)
{

	qsh_metric_id_t metric_id;
	qsh_client_metric_context_id_t metric_context_id;
  sys_modem_as_id_e_type          subs_id;
  	 
  	metric_id = param->action_params.metric_cfg.id;
	metric_context_id = param->action_params.metric_cfg.metric_context_id;
    subs_id = param->action_params.metric_cfg.subs_id;
  
    if ((subs_id < SYS_MODEM_AS_ID_1) || (subs_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
    {
      TDSRLC_MSG1(MSG_LEGACY_MED,"RLC_QSH:: invalid as_id %d",subs_id);
      return;
    }
	 
  	if(param->action_params.metric_cfg.action== QSH_METRIC_ACTION_START)
  	{
 		TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_QSH::Rcvd QSH START  command from QSH for RLC ");

		if((metric_id == TDSRLC_QSH_METRIC_RLC_STAT) && (param->action_params.metric_cfg.size_bytes >= sizeof(tdsrlc_qsh_metric_data_stats_s)))
			{
				tdsrlc_qsh_metric_coll_status[subs_id] = TDSRLC_QSH_METRICS_ENABLED;
    			tdsrlc_update_metric_address(metric_id,metric_context_id, subs_id, param->action_params.metric_cfg.start_addr);
			}
		else
			{
				TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_QSH::Size bytes allocated is less than size of tdsrlc_qsh_metric_data_stats_s ");
			}
  	}

	if(param->action_params.metric_cfg.action== QSH_METRIC_ACTION_STOP)
  	{
		TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_QSH::Rcvd QSH STOP  command from QSH for RLC ");
		tdsrlc_qsh_metric_coll_status[subs_id] = TDSRLC_QSH_METRICS_DISABLED;
		tdsrlc_metric_address[subs_id].start_address = NULL;

	}
  	


}



/*===========================================================================

FUNCTION TDSRLC_QSH_PROCESS_CMD

DESCRIPTION
  This function is a command handler for QSH commands CFG and TIMER EXPIRY
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_qsh_process_cmd(tdsrlc_qsh_command_type* rlc_qsh_cmd)
{
	qsh_client_action_done_s cb_done;
	
	TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_QSH::Rcvd command from QSH for RLC ");
	if(rlc_qsh_cmd->param.action == QSH_ACTION_METRIC_CFG)
		{
			TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_QSH::Rcvd QSH  command from QSH for RLC ");
			tdsrlc_qsh_process_cfg(&(rlc_qsh_cmd->param));
			//Clearing the stats already collected to start afresh.
			memset( &tdsrlc_qsh_metrics, 0, sizeof(tdsrlc_qsh_metric_data_stats_s) );
			tdsrlc_bad_status_pdu_cnt =0;
			tdsrlc_qsh_dl_rlc_resets =0;
			tdsrlc_qsh_dl_tput =0;
			
		}

	else if (rlc_qsh_cmd->param.action == QSH_ACTION_METRIC_TIMER_EXPIRY)
		{
			TDSRLC_MSG0(MSG_LEGACY_MED,"RLC_QSH::Rcvd Timer Expiry command from QSH for RLC ");
			tdsrlc_qsh_collect_report_metrics(&(rlc_qsh_cmd->param));
		}

	else
		{
			
			TDSRLC_MSG0(MSG_LEGACY_ERROR,"RLC_QSH::Rcvd wrong command from QSH for RLC,supported only Metric CFG and Timer Expiry cmds ");
		}


	qsh_client_action_done_init(&cb_done);
	cb_done.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;
	cb_done.cb_params_ptr = &(rlc_qsh_cmd->param);
	qsh_client_action_done(&cb_done);
	
}


void tdsrlc_qsh_update_bad_status_pdu(uint8 rb_id, uint16 sn, uint16 vt_a, uint16 vt_s)
{
	int i;
	if(tdsrlc_bad_status_pdu_cnt>0)
	{
		for(i=(TDS_RLC_QSH_MAX_BAD_STATUS_PDUS_LOGGING -2); i >=0; i--)
		{
			
			tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[i+1].rb_id = tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[i].rb_id;
			tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[i+1].sn = tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[i].sn;
			tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[i+1].vt_a = tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[i].vt_a;
			tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[i+1].vt_s = tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[i].vt_s;
		}
	}
	
	tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[0].rb_id = rb_id;
	tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[0].sn = sn;
	tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[0].vt_a = vt_a;
	tdsrlc_qsh_metrics.rlc_bad_status_pdu_stats[0].vt_s = vt_s;
	tdsrlc_bad_status_pdu_cnt++;

	if(tdsrlc_bad_status_pdu_cnt >TDS_RLC_QSH_MAX_BAD_STATUS_PDUS_LOGGING)
		{

			tdsrlc_bad_status_pdu_cnt =TDS_RLC_QSH_MAX_BAD_STATUS_PDUS_LOGGING;
		}
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"TDS_QSH: Bad status PDU recieved and count for this duration is %d ",tdsrlc_bad_status_pdu_cnt);

}

#endif /*defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)*/


