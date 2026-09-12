/*===========================================================================
               D O W N L I N K   R A D I O   L I N K   C O N T R O L

DESCRIPTION
  This file provides the functions to process RLC Downlink logical channels.

EXTERNALIZED FUNCTIONS

Added this comment for testing package warehouse. It is of no importance.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rlc/src/tdsrlcdl.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ----------------------------------------------------------- 
10/13/14  sarao  CFCM changes for RLC
09/08/11  mkh     Events renamed to TDS
10/24/10  guo     Initial release Upmerged WCDMA to TDS
===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
**                          Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "task.h"
#include "tdsrlcdl.h"
#include "tdsrlci.h"
#include "tdsrlcdltm.h"
#include "tdsrlcdlum.h"
#include "tdsrlcdlam.h"
#include "bit.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccmd_l2.h"
#include "tdsrlcul.h"
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
 #include "tdsrlcdllog.h"
#endif



#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
  #include "tdsrlcfc.h"
#endif

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
#include "event.h"
#include "event_defs.h"
#endif
#ifdef FEATURE_MODEM_RCINIT
#include "tdsl1m_rcinit.h"
#endif

#include "tdsrlcfc.h"
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif

#include "tdsrrcdataext.h"

/* -----------------------------------------------------------------------
** Variable Definitions
** ----------------------------------------------------------------------- */
const char tdsrlc_entry[20]="RLC DL ENTRY", tdsrlc_exit[20]= "RLC DL EXIT", tdsrlc_dup[20]= "RLC DL DUP";
/* RLC-DL Flow Control Global. */
extern tdsrlc_fc_type tdsrlc_dl_fc;

extern uint16 tdsrlc_dl_wm_unreg_f3_print_cnt;

uint8 tdswm_trace_timer[TDSUE_MAX_DL_LOGICAL_CHANNEL] = {0};
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
//rex_timer_type tdsdl_tmr_log_blk;    /* Rex timer used for qxdm loggin  */
tdsrlc_statistic_timer_type dl_statistic_timer;
#endif

tdsrlci_downlink_table_type tdsdownlink_table;
q_type tdsrlci_rcvd_q;

  #define TDSMAX_PDUS_IN_Q 1550

uint8 tdsdl_ctl_blk_id_tbl[TDSUE_MAX_DL_LOGICAL_CHANNEL];

rex_timer_type        tdsrlc_dl_am_base_tmr;
                              /* This is base 10ms timer for RLC AM channels */
                              /* at expiry, periodic status/ control related */
                              /* procedures are triggered*/
boolean               tdsrlc_dl_am_base_tmr_start = FALSE;


tdsrlc_dl_hfn_cnf_type tdsdl_hfn_cnf;        /* HFN confirmation               */


#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  tdsrlc_dl_tm_config_type tdsdl_tm_log_ptr;
  tdsrlc_dl_um_config_type tdsdl_um_log_ptr;
  tdsrlc_am_config_req_type tdsdl_am_log_ptr;
  tdsrlc_dl_log_state_type tdsdl_log_state;

  boolean tdsrlci_dl_pdu_cipher_log_on = FALSE;

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
   #define TDS_NUM_RLCI_DL_PDU_CIPHER_LOG_BUFFER 2
   tdsrlc_dl_pdu_cipher_info_type tdsrlci_dl_pdu_cipher_log[TDS_NUM_RLCI_DL_PDU_CIPHER_LOG_BUFFER];
   tdsrlc_dl_pdu_cipher_info_type* tdsrlci_dl_pdu_cipher_log_ptr;
   rex_crit_sect_type tdsrlci_dl_pdu_cipher_log_mutex;
#else
   tdsrlc_dl_pdu_cipher_info_type tdsrlci_dl_pdu_cipher_log;
#endif   

#endif


/* Variable to store log status information  */
/*  TDSRLCI_UM_USR_PLANE_LOG_MASK : 0x01
 *  TDSRLCI_AM_USR_PLANE_LOG_MASK : 0x02
 *  TDSRLCI_AM_SIG_PLANE_LOG_MASK : 0x04 */
  uint32  tdsrlci_log_mask = 0x0;

/* Q of TSN info structures */
/* MAC delivers the tsn structure to RLC in interrupt context 
 * This TSN will be processed in RLC DL task context and 
 * releases (puts) them back in Free Q (call provided by MAC-HS)*/
  q_type tdsrlc_tsn_info_q;
  /*--------------------------------------------------------------------------
 *  Mutex to protect the header read ISR and 
 *  MAC DL recfg flushing the partial/complete PDUs 
 ---------------------------------------------------------------------------*/
 rex_crit_sect_type tdsrlc_tsn_mutex;

/* Q of RLC ENH DATA structures */
/* MAC will deliver Non-HS data to RLC using this data structure
 * MAC will enqueue them into tdsrlc_enh_data_q and post a signal to RLC
 * to act in TASK context. After handling, the entry will be returned
 * to tdsrlc_enh_free_data_q */
tdsrlc_enh_data_ind_type tdsrlc_enh_data[TDSRLC_MAX_TTI_DATA_DL];

q_type tdsrlc_enh_data_q;
q_type tdsrlc_enh_free_data_q;


/* Holds the AM SDU Chain during reassembly */
extern tdsrlci_dl_temp_pdu_info_type tdsdl_am_pdu_info;
extern tdsrlci_dl_temp_pdu_info_type tdsdl_um_pdu_info;

extern rex_crit_sect_type tdsrlc_dl_mutex;
extern rex_crit_sect_type tdsrlc_dl_data_process_mutex;

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
extern rex_crit_sect_type tdsrlc_fc_sem;
#endif

rex_crit_sect_type tdsrlc_mac_dl_pdu_mutex;


/* =======================================================================
**                            Functions
** ======================================================================= */
extern void tdsmac_hs_dl_discard_tsn(tdsl1_dec_hs_tsn_info_type *int_tsn_ptr);
/*===========================================================================

FUNCTION TDSDL_ASSIGN_CTL_BLK

DESCRIPTION
  Assign Downlink control block for an RLC entity.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint8 tdsdl_assign_ctl_blk(void)
{
  uint8 ret_val = TDSINIT_CTL_BLK_ID;
  int i;
  
  //sarao: Adding the comment to test Package warehouse

  for (i=0; i<TDSUE_MAX_DL_RLC_ENTITY; i++)
  {
    if (tdsdownlink_table.ctrl_blk[i].lc_state == TDSDL_NULL_STATE)
    {
      ret_val = (uint8)i;
      break;
    }
  }
  if (i == TDSUE_MAX_DL_RLC_ENTITY)
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"Too many RLC entities");
  }
  return ret_val;

} /* tdsdl_assign_ctl_blk() */

#ifdef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
/*===========================================================================

FUNCTION TDSRLC_DL_CRIT_SECT_INIT

DESCRIPTION
  Initializes the RLC DL critical sections

DEPENDENCIES
  This function should be called at task init.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_crit_sect_init(void)
{
  rex_init_crit_sect(&tdsrlc_tsn_mutex);
  rex_init_crit_sect(&tdsrlc_dl_mutex);
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  rex_init_crit_sect(&tdsrlci_dl_pdu_cipher_log_mutex);
  rex_init_crit_sect(&tdsrlc_fc_sem);
#endif
  rex_init_crit_sect(&tdsrlc_mac_dl_pdu_mutex);
  rex_init_crit_sect(&tdsrlc_dl_data_process_mutex);
}
#endif



/*===========================================================================

FUNCTION TDSRLC_DL_INIT

DESCRIPTION
  Initializes the RLC Downlink Logical channel table.

DEPENDENCIES
  This function should be called at task start up.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_init(void)
{
  int i;


  TDSRLC_MSG0(MSG_LEGACY_LOW,"In RLC Downlink Initialization");
  /*-------------------------------------------------------------------------
    Initialize the downlink table
  -------------------------------------------------------------------------*/
  (void)memset( &tdsdownlink_table, 0, sizeof( tdsdownlink_table));
  /*-------------------------------------------------------------------------
    Initialize the mapping table for control block id and channel id.
  -------------------------------------------------------------------------*/
  for (i=0; i<TDSUE_MAX_DL_LOGICAL_CHANNEL; i++)
  {
    tdsdl_ctl_blk_id_tbl[i] = TDSINIT_CTL_BLK_ID;
  }

  /*-------------------------------------------------------------------------
    Init internal receiving queue.
  -------------------------------------------------------------------------*/
  (void)q_init(&tdsrlci_rcvd_q);

  (void)q_init(&tdsrlc_tsn_info_q);
  (void)q_init(&tdsrlc_enh_data_q);
  (void) q_init(&tdsrlc_enh_free_data_q);
    /*Initialize the mutex*/
#ifndef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
  rex_init_crit_sect(&tdsrlc_tsn_mutex);
#endif
  for (i = 0; i < TDSRLC_MAX_TTI_DATA_DL; i++)
  {
    (void) q_link(&tdsrlc_enh_data[i], &tdsrlc_enh_data[i].link);
    q_put(&tdsrlc_enh_free_data_q, &tdsrlc_enh_data[i].link);
  }

#ifndef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
  /*-------------------------------------------------------------------------
    Initialize DL mutex.
  -------------------------------------------------------------------------*/
  rex_init_crit_sect(&tdsrlc_dl_mutex);
  rex_init_crit_sect(&tdsrlc_mac_dl_pdu_mutex);
  rex_init_crit_sect(&tdsrlc_dl_data_process_mutex);
#endif

  /*-------------------------------------------------------------------------
    Perform the AM initialization.
  -------------------------------------------------------------------------*/
  tdsrlci_dl_am_init();


#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  /*---------------------------------------------------------------------
  Define the log timer here and Set the log timer
  ----------------------------------------------------------------------*/
  //rex_def_timer(&tdsdl_tmr_log_blk,tds_l2_dl_get_tcb(), TDSRLC_DL_AM_LOG_TIMER_EXP_SIG);
  //(void) rex_set_timer(&tdsdl_tmr_log_blk,(uint32)TDSRLC_AM_LOG_TIMER_BASE);
  dl_statistic_timer.statistic_timer_value = TDSRLC_AM_LOG_TIMER_BASE;

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  memset(tdsrlci_dl_pdu_cipher_log, 0, TDS_NUM_RLCI_DL_PDU_CIPHER_LOG_BUFFER*sizeof(tdsrlc_dl_pdu_cipher_info_type));
  tdsrlci_dl_pdu_cipher_log_ptr = &tdsrlci_dl_pdu_cipher_log[0];
#ifndef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
  rex_init_crit_sect(&tdsrlci_dl_pdu_cipher_log_mutex);
#endif
#else
  memset(&tdsrlci_dl_pdu_cipher_log, 0, sizeof(tdsrlci_dl_pdu_cipher_log));
#endif



#endif

} /* tdsrlc_dl_init() */



/*===========================================================================

FUNCTION TDSCRLC_DL_CIPHER_CONFIG_HANDLER

DESCRIPTION
  Configures ciphering on one or more Unacknowledged or Acknowledged
  Mode Channels.This function configures the ciphering parameters that are
  recevied from RRC. The ciphering parameters are updated only if the the
  channel is in TDSDL_DATA_TRANSFER_READY STATE.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_cipher_config_handler
(
  tdsrlc_dl_cipher_config_type *cipher_ptr   /* Ptr to Downlink Cipher Config */
)
{
  tdsrlci_dl_ctrl_blk_type *ctl_blk = NULL;
  uint16 index;
  int i;

  TDSRLC_MSG1(MSG_LEGACY_HIGH,"RLC_DBG: recevied DL cipher config for nchan %d ",cipher_ptr->nchan);

  /*-------------------------------------------------------------------------
    For each channel indicated in the Ciphering Config Request
  -------------------------------------------------------------------------*/
  for (i = 0; i < cipher_ptr->nchan; i++)
  {
    /*-----------------------------------------------------------------------
      Get the control block id
    -----------------------------------------------------------------------*/
    index = tdsdl_ctl_blk_id_tbl[cipher_ptr->ciphering[i].lc_id];
    /* just for KW error*/
    if (index >= TDSUE_MAX_DL_RLC_ENTITY)
    {
      continue;
    }
    ctl_blk = &tdsdownlink_table.ctrl_blk[index];
    /*-----------------------------------------------------------------------
      If RLC downlink is in Data Transfer Ready state, copy details
     to the downlink table entry.
    -----------------------------------------------------------------------*/
    /*lint -e655 */
    if (index != TDSINIT_CTL_BLK_ID && ctl_blk->lc_state & TDSDL_DATA_TRANSFER_READY)
    /*lint +e655 */
    {
      /*---------------------------------------------------------------------
        Copy the ciphering configuration details to the tdsdownlink_table entry.
      ---------------------------------------------------------------------*/
      if (cipher_ptr->ciphering[i].cipher_mode)
      {
        TDSRLC_MSG3(MSG_LEGACY_HIGH,"DL cipher config, LC %d, tdshfn %x, act_seq %x",
             cipher_ptr->ciphering[i].lc_id, cipher_ptr->ciphering[i].hfn,
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
            /*---------------------------------------------------------------
             There is no PDU retx in UM. We don't need to remember the
             current cipher_key and hfn when the cipher_mode moves from
             TDSCIPHER_MOD to TDSCIPHER_ON.
            ---------------------------------------------------------------*/
            case TDSNO_CIPHER:
            case TDSCIPHER_ON:
              /*-------------------------------------------------------------
                If the channel is directly config to TDSCIPHER_ON mode before,
                but the vr_h has not reached the act_sn, store the old
                and the new ciphering configurations.
              -------------------------------------------------------------*/
              if (TDSRLCI_SEQ_GT(cipher_ptr->ciphering[i].ciph_act_sn,
                               ctl_blk->ctrl.um_ctrl.vr_ur, TDSRLC_UM_MODULUS))
              {
                ctl_blk->cipher.cipher_mode =
                  (ctl_blk->cipher.cipher_mode == TDSNO_CIPHER) ?
                  TDSCIPHER_WAIT : TDSCIPHER_MOD;

                ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
#ifdef FEATURE_TDSCDMA_FUTURE_CIPHERING_ACTIVATION
                ctl_blk->cipher.wrap_valid_new = FALSE;
#endif
              }
              else
              {
#ifdef FEATURE_TDSCDMA_FUTURE_CIPHERING_ACTIVATION
                if (cipher_ptr->ciphering[i].ciph_act_sn != ctl_blk->ctrl.um_ctrl.vr_ur)
                {

                  ctl_blk->cipher.cipher_mode =
                    (ctl_blk->cipher.cipher_mode == TDSNO_CIPHER) ?
                    TDSCIPHER_WAIT : TDSCIPHER_MOD;

                  ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
                  ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
                  ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
                  ctl_blk->cipher.wrap_valid_new = TRUE;
                  TDSRLC_MSG3(MSG_LEGACY_HIGH,
                    "Future ciphering - DL Cipher config, LC %d no_cipher/cipher_on act_sn_old %d vr_ur %d",
                    ctl_blk->ctrl.um_ctrl.rlc_id, ctl_blk->cipher.act_sn, 
                    ctl_blk->ctrl.um_ctrl.vr_ur);
                }
                else
#endif
                {  
                  ctl_blk->cipher.cipher_mode = TDSCIPHER_ON;
                  //ctl_blk->cipher.new_cipher_on = TRUE;
                  ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                  ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                  ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                  ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 7) |
                                        ctl_blk->ctrl.um_ctrl.vr_ur;
                  TDSRLC_MSG3(MSG_LEGACY_HIGH,"Cipher ON DL UM, LC %d, Key_id %d, count %x",
                          cipher_ptr->ciphering[i].lc_id,
                          ctl_blk->cipher.cipher_key_id, ctl_blk->cipher.count);
                }
              }
              ctl_blk->cipher.act_sn_old = ctl_blk->cipher.act_sn;
              ctl_blk->cipher.act_sn= cipher_ptr->ciphering[i].ciph_act_sn;
              break;

            case TDSCIPHER_WAIT:
            case TDSCIPHER_MOD:
              ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
              ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
              ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
              break;

            default:
              TDSRLC_MSG0(MSG_LEGACY_ERROR,"Invalid Ciphering State");
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

              /*-----------------------------------------------------------
                If the new activation time has not been reached, store the
                new ciphering parameters.
              -------------------------------------------------------------*/
              if (TDSRLCI_SEQ_GT(cipher_ptr->ciphering[i].ciph_act_sn,
                    ctl_blk->ctrl.am_ctrl.dl_state_var.vr_h, TDSRLC_AM_MODULUS))
              {
                ctl_blk->cipher.cipher_mode =
                  (ctl_blk->cipher.cipher_mode == TDSNO_CIPHER) ?
                  TDSCIPHER_WAIT : TDSCIPHER_MOD;

                ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
                ctl_blk->cipher.act_sn_new = cipher_ptr->ciphering[i].ciph_act_sn;
#ifdef FEATURE_TDSCDMA_FUTURE_CIPHERING_ACTIVATION
                ctl_blk->cipher.wrap_valid_new = FALSE;
#endif
              }
              /*-------------------------------------------------------------
                Reached the new activation time.
              -------------------------------------------------------------*/
              else
              {
#ifdef FEATURE_TDSCDMA_FUTURE_CIPHERING_ACTIVATION
                if ((ctl_blk->ctrl.am_ctrl.am_common->rb_id <= TDSRLC_MAX_SRB_ID)
                   && (cipher_ptr->ciphering[i].ciph_act_sn !=
                    ctl_blk->ctrl.am_ctrl.dl_state_var.vr_h))
                {
                  TDSRLC_MSG3(MSG_LEGACY_HIGH,
                  "DL Cipher config, LC %d no_cipher/cipher_on act_sn_new %d act_sn_new1 %d",
                  ctl_blk->ctrl.am_ctrl.am_common->ul_data_id, ctl_blk->cipher.act_sn_new, 
                  ctl_blk->cipher.act_sn_new1);

                  ctl_blk->cipher.cipher_mode =
                    (ctl_blk->cipher.cipher_mode == TDSNO_CIPHER) ?
                    TDSCIPHER_WAIT : TDSCIPHER_MOD;

                  ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
                  ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
                  ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
                  ctl_blk->cipher.act_sn_new = cipher_ptr->ciphering[i].ciph_act_sn;
                  ctl_blk->cipher.wrap_valid_new = TRUE;
                  continue;
                }
#endif

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
                }
                /*-------------------------------------------------------------
                  No Cipher when received this new cipher config.
                -------------------------------------------------------------*/
                else
                {
                  ctl_blk->cipher.cipher_mode = TDSCIPHER_ON;
                }

                //ctl_blk->cipher.new_cipher_on = TRUE;
                /*-------------------------------------------------------------
                  Update current active cipher config.
                -------------------------------------------------------------*/
                ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 12) |
                                          ctl_blk->ctrl.am_ctrl.dl_state_var.vr_h;
                ctl_blk->cipher.act_sn = cipher_ptr->ciphering[i].ciph_act_sn;
                TDSRLC_MSG3(MSG_LEGACY_HIGH,"Cipher ON DL AM, LC %d, Key_id %d, count %x",
                          cipher_ptr->ciphering[i].lc_id,
                          ctl_blk->cipher.cipher_key_id, ctl_blk->cipher.count);
              }

              break;

            case TDSCIPHER_WAIT:
            case TDSCIPHER_MOD:
              if (ctl_blk->cipher.act_sn_new != cipher_ptr->ciphering[i].ciph_act_sn)
              {
#ifdef FEATURE_TDSCDMA_FUTURE_CIPHERING_ACTIVATION
                if ((ctl_blk->ctrl.am_ctrl.am_common->rb_id <= TDSRLC_MAX_SRB_ID)&& 
                TDSRLCI_SEQ_GT(ctl_blk->ctrl.am_ctrl.dl_state_var.vr_h,
                    cipher_ptr->ciphering[i].ciph_act_sn, TDSRLC_AM_MODULUS))
                {
                  ctl_blk->cipher.wrap_valid_new1 = TRUE;
                }
                else
                {
                  ctl_blk->cipher.wrap_valid_new1 = FALSE;
                }
#endif
                ctl_blk->cipher.act_sn_new1 = cipher_ptr->ciphering[i].ciph_act_sn;
                ctl_blk->cipher.cipher_key_id_new1 = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo_new1 = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn_new1 = cipher_ptr->ciphering[i].hfn;
                ctl_blk->cipher.two_pending = TRUE;
              }
              else
              {
               /*lint -e64 -e641 -e655 */
                ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
                /*lint +e64 +e641 +e655 */
                ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
              }
              break;

            default:
              TDSRLC_MSG0(MSG_LEGACY_ERROR,"Invalid Ciphering State");
          }
        }
        ctl_blk->cipher.rb_id = cipher_ptr->ciphering[i].rb_id;
      }
      else
      {
        ctl_blk->cipher.cipher_mode = TDSNO_CIPHER;
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"DL Cipher config,LC %d cipher OFF",cipher_ptr->ciphering[i].lc_id);
      }
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL Cipher config,LC %d not up",cipher_ptr->ciphering[i].lc_id);
    }
  }

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (log_status(TDSCDMA_DL_RLC_CIPH_CFG_LOG_PACKET))
  {
    TDSRLC_MSG0(MSG_LEGACY_LOW,"LOG DL Cipher config");
    tdsrlc_dl_tx_cipher_log_packet(cipher_ptr);
  }
  else
  {
    TDSRLC_MSG0(MSG_LEGACY_LOW,"NO LOG DL Cipher config");
  }
#endif

} /* tdscrlc_dl_cipher_config_handler() */

/*===========================================================================

FUNCTION TDSCRLC_DL_TM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of Transparent Mode Logical
  channels. This function is called when RRC sends a TDSCRLC_DL_TM_CONFIG_REQ
  to RLC. If a new channel is to be established, the function checks if
  the channels exists. If it already exists, and ERR message is dispalyed.
  If the channel does not exist, the downlink table entry for that channel
  is updated and the state is set to TDSDL_DATA_TRANSFER_READY. If a channel
  is to be released,the function checks if the channel is in the
  TDSDL_DATA_TRANSFER_READY state. If it is, the channel is set to the NULL
  state and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the
  TDSDL_DATA_TRANSFER_READY state. If it is, the downlink table entry for the
  channel is updated with the configuration received from RRC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_tm_config_handler
(
  tdsrlc_dl_tm_config_type *tm_ptr /* Pointer to TM config info               */
)
{
  tdsrrc_rlc_cmd_type *rrc_rlc_cmd_ptr; /* Command buffer to store confirm        */
                                 /* Information                            */
  uint8 index;
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
    ERR_FATAL("Could not get command buf",0,0,0);
    /*lint -e527 */
    return;
    /*lint +e527 */
  }
  rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CRLC_DL_TM_CONFIG_CNF;
  rrc_rlc_cmd_ptr->cmd.crlc_dl_tm_config_cnf.nchan = tm_ptr->nchan;
  TDSL2_ACQ_DL_LOCK();

  for (i = 0; i < tm_ptr->nchan; i++)
  {
    rrc_rlc_cmd_ptr->cmd.crlc_dl_tm_config_cnf.tm_config_result[i].lc_id =
                                                  tm_ptr->chan_info[i].lc_id;
    if (tm_ptr->chan_info[i].lc_id >= TDSUE_MAX_DL_LOGICAL_CHANNEL)
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"Invalid DL TM LC Id %d",tm_ptr->chan_info[i].lc_id);
      rrc_rlc_cmd_ptr->cmd.crlc_dl_tm_config_cnf.
                                  tm_config_result[i].result = FAILURE;
    }
    else
    {
      rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                          tm_config_result[i].result = SUCCESS;
      if(tm_ptr->dl_tm_act[i].action == TDSRLC_MODIFY)
      {
        while(tm_ptr->dl_tm_act[i].dl_cfg_mask != 0)
        {
          action = tdsrlci_get_config_action(&(tm_ptr->dl_tm_act[i].dl_cfg_mask));
          switch(action)
          {
            case TDSRLCI_MODIFY:
              /*-------------------------------------------------------------------
                Check if Channel is in NULL state. If it is not, modify
                the channel parameters and send a SUCCESS indication in the
                confirm message to RRC.
              -------------------------------------------------------------------*/
              if ((index = tdsdl_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id])
                                                          != TDSINIT_CTL_BLK_ID &&
                  (index < TDSUE_MAX_DL_RLC_ENTITY))
              {
                TDSRLC_MSG2(MSG_LEGACY_HIGH,"DL TM Modify LC %d,Index %d",tm_ptr->chan_info[i].lc_id,index);
                tdsdownlink_table.ctrl_blk[index].lc_type =
                                            tm_ptr->chan_info[i].lc_type;
                tdsdownlink_table.ctrl_blk[index].ctrl.tm_ctrl.seg_ind =
                                            tm_ptr->chan_info[i].seg_ind;
                tdsdownlink_table.ctrl_blk[index].ctrl.tm_ctrl.act_incl =
                                            tm_ptr->dl_tm_act[i].act_incl;
                tdsdownlink_table.ctrl_blk[index].ctrl.tm_ctrl.act_time =
                                            tm_ptr->dl_tm_act[i].act_time;
                rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                            tm_config_result[i].result = SUCCESS;
    #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                if (j < UE_MAX_DL_TM_CHANNEL)
                {
                  tdsdl_tm_log_ptr.chan_info[j] = tm_ptr->chan_info[i];
                  j++;
                }
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
                TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL LC %d not in right State - Cannot Mod",tm_ptr->chan_info[i].lc_id);
              }
              break;
            default:
              /*-------------------------------------------------------------------
                If the action is Invalid, send a FAILURE indication in the confirm
                to RRC.
              -------------------------------------------------------------------*/
              rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                   tm_config_result[i].result = FAILURE;
    
              TDSRLC_MSG2(MSG_LEGACY_ERROR,"Invalid Action %d for channel id %d",tm_ptr->dl_tm_act[i].action,tm_ptr->chan_info[i].lc_id);
  
          }
        }
      }
      else
      {
        switch(tm_ptr->dl_tm_act[i].action)
        {
          case TDSRLC_ESTABLISH:
          /*---------------------------------------------------------------------
            Check if the channel exists. If it does not, get an index
            into the downlink table and store all the information for the
            channel. Send a SUCCESS indication in the confirm message to RRC
          ---------------------------------------------------------------------*/
            if ((tdsdl_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id] ==
                                                  TDSINIT_CTL_BLK_ID) &&
                ((index = tdsdl_assign_ctl_blk()) != TDSINIT_CTL_BLK_ID) &&
                (index < TDSUE_MAX_DL_RLC_ENTITY))
            {
              TDSRLC_MSG2(MSG_LEGACY_HIGH,"DL TM Establish LC %d,Index %d",tm_ptr->chan_info[i].lc_id,index);
              tdsdl_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id] = index;
              tdsdownlink_table.ctrl_blk[index].lc_mode = UE_MODE_TRANSPARENT;
              tdsdownlink_table.ctrl_blk[index].lc_type =
                                          tm_ptr->chan_info[i].lc_type;
              tdsdownlink_table.ctrl_blk[index].ctrl.tm_ctrl.seg_ind =
                                          tm_ptr->chan_info[i].seg_ind;
              tdsdownlink_table.ctrl_blk[index].ctrl.tm_ctrl.act_incl =
                                          tm_ptr->dl_tm_act[i].act_incl;
              tdsdownlink_table.ctrl_blk[index].ctrl.tm_ctrl.act_time =
                                          tm_ptr->dl_tm_act[i].act_time;
              tdsdownlink_table.ctrl_blk[index].lc_state = TDSDL_DATA_TRANSFER_READY;
              tdsdownlink_table.ctrl_blk[index].dl_wm_ptr = NULL;
              tdsdownlink_table.ctrl_blk[index].rlc_post_rx_func_ptr = NULL;
              tdsdownlink_table.active_ch++;
              rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                          tm_config_result[i].result = SUCCESS;
  #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
              tdsdl_tm_log_ptr.chan_info[j] = tm_ptr->chan_info[i];
              j++;
              tdsdl_log_state.dl_state[k].rlc_id = tm_ptr->chan_info[i].lc_id;
              tdsdl_log_state.dl_state[k].lc_state = TDSTM_DL_DATA_TRANSFER_READY;
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
              TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL LC %d Exists,Cannot Establish",tm_ptr->chan_info[i].lc_id);
            }
            break;
          case TDSRLC_RELEASE:
            /*-------------------------------------------------------------------
              Check if the channel is in NULL state. If it is not, release
              the channel and send a SUCCESS indication in the confirm message
              to RRC.
            -------------------------------------------------------------------*/
            if ((index = tdsdl_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id])
                                                        != TDSINIT_CTL_BLK_ID &&
                (index < TDSUE_MAX_DL_RLC_ENTITY))
            {
              TDSRLC_MSG2(MSG_LEGACY_HIGH,"DL TM Release LC %d,Index %d",tm_ptr->chan_info[i].lc_id,index);
              tdsdownlink_table.ctrl_blk[index].lc_state = TDSDL_NULL_STATE;
              tdsdownlink_table.active_ch--;
              tdsdl_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id] = TDSINIT_CTL_BLK_ID;
              rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                          tm_config_result[i].result = SUCCESS;
  
  #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
              tdsdl_log_state.dl_state[k].rlc_id = tm_ptr->chan_info[i].lc_id;
              tdsdl_log_state.dl_state[k].lc_state = TDSRLC_DL_NULL_STATE;
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
              TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL LC %d doesn't exist - Cannot Release",tm_ptr->chan_info[i].lc_id);
            }
            break;
          default:
            /*-------------------------------------------------------------------
              If the action is Invalid, send a FAILURE indication in the confirm
              to RRC.
            -------------------------------------------------------------------*/
            rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                 tm_config_result[i].result = FAILURE;
  
            TDSRLC_MSG2(MSG_LEGACY_ERROR,"Invalid Action %d for channel id %d",tm_ptr->dl_tm_act[i].action,tm_ptr->chan_info[i].lc_id);
        }
      }
    }
  }
  
  TDSL2_ACQ_DL_UNLOCK();

  /*-------------------------------------------------------------------------
    Send confirmation to RRC.
  -------------------------------------------------------------------------*/
  tdsrrc_put_rlc_cmd(rrc_rlc_cmd_ptr);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (j > 0)
  {
    tdsdl_tm_log_ptr.nchan = j;
    tdsrlc_dl_send_tm_log_packet(&tdsdl_tm_log_ptr);
  }
  if (k > 0)
  {
    tdsdl_log_state.nchan = k;
    tdsrlc_dl_send_state_log_packet(&tdsdl_log_state);
  }
#endif

} /* tdscrlc_dl_tm_config_handler() */


/*===========================================================================

FUNCTION TDSCRLC_DL_UM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of Unacknowledged Mode Logical
  channels. This function is called when RRC sends a TDSCRLC_DL_UM_CONFIG_REQ
  to RLC. If a new channel is to be established, the function checks if
  the channels exists. If it already exists, and ERR message is dispalyed.
  If the channel does not exist, the downlink table entry for that channel
  is updated and the state is set to TDSDL_DATA_TRANSFER_READY. If a channel
  is to be released,the function checks if the channel is in the
  TDSDL_DATA_TRANSFER_READY state. If it is, the channel is set to the NULL
  state and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the
  TDSDL_DATA_TRANSFER_READY state. If it is, the downlink table entry for the
  channel is updated with the configuration received from RRC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_um_config_handler
(
  tdsrlc_dl_um_config_type *um_ptr  /* Pointer to UM config info              */
)
{
  tdsrrc_rlc_cmd_type *rrc_rlc_cmd_ptr; /* Command buffer to store confirm        */
  uint8 index;
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
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"Could not get command buf");
    return;
  }
  rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CRLC_DL_UM_CONFIG_CNF;
  rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.nchan = um_ptr->nchan;

  TDSL2_ACQ_DL_LOCK();

  for (i = 0; i < um_ptr->nchan; i++)
  {
    rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.um_config_result[i].lc_id =
                                                  um_ptr->chan_info[i].lc_id;
    if (um_ptr->chan_info[i].lc_id >= TDSUE_MAX_DL_LOGICAL_CHANNEL)
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"Invalid DL UM LC Id %d",um_ptr->chan_info[i].lc_id);
      rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                  um_config_result[i].result = FAILURE;
    }
    else
    {
      switch(um_ptr->dl_um_act[i].action)
      {
        case TDSRLC_ESTABLISH:
          /*-------------------------------------------------------------------
            Make sure the channel does not exist. If it does not, get an index
            into the downlink table and store all the information for the
            channel. Send a SUCCESS indication in the confirm message to RRC
          -------------------------------------------------------------------*/
          if ((tdsdl_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id] == TDSINIT_CTL_BLK_ID) &&
             ((index = tdsdl_assign_ctl_blk()) != TDSINIT_CTL_BLK_ID) &&
             (index < TDSUE_MAX_DL_RLC_ENTITY))
          {
            TDSRLC_MSG2(MSG_LEGACY_HIGH,"DL UM Establish LC %d,Index %d",um_ptr->chan_info[i].lc_id,index);
            tdsdl_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id] = index;

            tdsrlci_establish_dl_um(&tdsdownlink_table.ctrl_blk[index],
                                 &um_ptr->chan_info[i],
                                 &um_ptr->dl_um_act[i]);

            tdsrlc_enh_establish_dl_um(&tdsdownlink_table.ctrl_blk[index]);
            rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                 um_config_result[i].result = SUCCESS;
            tdsdownlink_table.active_ch++;
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
            tdsrlci_generate_dl_config_event     (&tdsdownlink_table.ctrl_blk[index],
                                          um_ptr->chan_info[i].rlc_size);
            tdsdl_um_log_ptr.chan_info[j] = um_ptr->chan_info[i];
            j++;
            tdsdl_log_state.dl_state[k].rlc_id = um_ptr->chan_info[i].lc_id;
            tdsdl_log_state.dl_state[k].lc_state = TDSUM_DL_DATA_TRANSFER_READY;
            k++;
#endif
          }
          else
          {
            /*-----------------------------------------------------------------
              If the channel exists, send a FAILURE indication in the confirm
              message to RRC.
            -----------------------------------------------------------------*/
            rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                        um_config_result[i].result = FAILURE;
            TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL LC %d in use - No Establish",um_ptr->chan_info[i].lc_id);
          }
          break;
        case TDSRLC_RELEASE:
          /*-------------------------------------------------------------------
            Make sure the channel is not in NULL state. If it is not, release
            the channel and send a SUCCESS indication in the confirm message
            to RRC.
          -------------------------------------------------------------------*/
          if ((index = tdsdl_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id]) != TDSINIT_CTL_BLK_ID &&
              (index < TDSUE_MAX_DL_RLC_ENTITY))
          {
            TDSRLC_MSG2(MSG_LEGACY_HIGH,"DL UM Release LC %d,Index %d",um_ptr->chan_info[i].lc_id,index);

            tdsrlci_release_dl_um(&(tdsdownlink_table.ctrl_blk[index]));

            tdsdl_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id] = TDSINIT_CTL_BLK_ID;
            rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                        um_config_result[i].result = SUCCESS;
            tdsdownlink_table.active_ch--;
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
            tdsdl_log_state.dl_state[k].rlc_id = um_ptr->chan_info[i].lc_id;
            tdsdl_log_state.dl_state[k].lc_state = TDSRLC_DL_NULL_STATE;
            k++;
#endif
          }
          else
          {
            /*-----------------------------------------------------------------
              If the channel does not exist, send a FAILURE indication in the
              confirm message to RRC.
            -----------------------------------------------------------------*/
            rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                 um_config_result[i].result = FAILURE;
            TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL LC %d Not config'd - No Release",um_ptr->chan_info[i].lc_id);
          }
          break;

        case TDSRLC_MODIFY:
          rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                                um_config_result[i].result = SUCCESS;

          if(um_ptr->dl_um_act[i].action == TDSRLC_MODIFY)
          {
            while(um_ptr->dl_um_act[i].dl_cfg_mask != 0)
            {
              action =tdsrlci_get_config_action(&(um_ptr->dl_um_act[i].dl_cfg_mask));
              switch(action)
              {
                case TDSRLCI_MODIFY:
                  /*-------------------------------------------------------------------
                    Make sure the channel is not in NULL state. If it is not, Modify
                    the channel parameters and send a SUCCESS indication in the
                    confirm message to RRC.
                  -------------------------------------------------------------------*/
                  if ((index = tdsdl_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id]) != TDSINIT_CTL_BLK_ID &&
                      (index < TDSUE_MAX_DL_RLC_ENTITY))
                  {
                    TDSRLC_MSG2(MSG_LEGACY_HIGH,"DL UM Modify LC %d,Index %d",um_ptr->chan_info[i].lc_id,index);

                    tdsrlci_modify_dl_um(&tdsdownlink_table.ctrl_blk[index],
                                      &um_ptr->chan_info[i],
                                      &um_ptr->dl_um_act[i]);
                    
                    rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                                um_config_result[i].result = SUCCESS;
        #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                    tdsrlci_generate_dl_config_event     (&tdsdownlink_table.ctrl_blk[index],
                                                  um_ptr->chan_info[i].rlc_size);
                    if (j < TDSUE_MAX_DL_UM_CHANNEL)
                    {
                      tdsdl_um_log_ptr.chan_info[j] = um_ptr->chan_info[i];
                      j++;
                    }
        #endif
                  }
                  else
                  {
                    /*-----------------------------------------------------------------
                      If the channel does not exist, send a FAILURE indication in the
                      confirm message to RRC.
                    -----------------------------------------------------------------*/
                    rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                                um_config_result[i].result = FAILURE;
                    TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL LC %d not config'd -- No Mod",um_ptr->chan_info[i].lc_id);
                  }
                  break;
        
                case TDSRLCI_STOP:
                  if ((index = tdsdl_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id]) != TDSINIT_CTL_BLK_ID &&
                      (index < TDSUE_MAX_DL_RLC_ENTITY))
                  {
                    /* Enter into Stop state. */
                    /*lint -e641 -e655 -e64 */
                    tdsdownlink_table.ctrl_blk[index].lc_state |= TDSDL_STOP;
                    tdsdownlink_table.ctrl_blk[index].lc_state &= (~TDSDL_DATA_TRANSFER_READY);
                    rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                         um_config_result[i].result = SUCCESS;
                    /*lint +e641 +e655 +e64 */
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"DL UM LC %d - Stop",um_ptr->chan_info[i].lc_id);
                  }
                  else
                  {
                    rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                         um_config_result[i].result = FAILURE;
                    TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL UM LC %d not Config'd -- No Stop",um_ptr->chan_info[i].lc_id);
                  }
                  break;
        
                case TDSRLCI_CONTINUE:
                  if ((index = tdsdl_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id]) != TDSINIT_CTL_BLK_ID &&
                      (index < TDSUE_MAX_DL_RLC_ENTITY))
                  {
                    /* Remove Stop state. Enter into the existing states before entering  */
                    /* into the Stop State.                                               */
                    /*lint -e64 -e641 -e655 */
                    tdsdownlink_table.ctrl_blk[index].lc_state &= (~TDSDL_STOP);
                    tdsdownlink_table.ctrl_blk[index].lc_state |= TDSDL_DATA_TRANSFER_READY;
                    rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                         um_config_result[i].result = SUCCESS;
                    /*lint +e64 +e641 +e655 */
                    TDSRLC_MSG1(MSG_LEGACY_HIGH,"DL UM LC %d - Continue",um_ptr->chan_info[i].lc_id);
                  }
                  else
                  {
                    rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                                um_config_result[i].result = FAILURE;
                    TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL UM LC %d not Config'd -- No Continue",um_ptr->chan_info[i].lc_id);
                  }
                  break;
      
                case TDSRLCI_RE_ESTABLISH:
                  if ((index = tdsdl_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id]) != TDSINIT_CTL_BLK_ID &&
                      (index < TDSUE_MAX_DL_RLC_ENTITY))
                  {
                    tdsrlci_re_establish_dl_um(&tdsdownlink_table.ctrl_blk[index], &um_ptr->chan_info[i]);
                    rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                         um_config_result[i].result = SUCCESS;
        #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                    tdsrlci_generate_dl_config_event     (&tdsdownlink_table.ctrl_blk[index],
                                                  um_ptr->chan_info[i].rlc_size);
                    if (j < TDSUE_MAX_DL_UM_CHANNEL)
                    {
                      tdsdl_um_log_ptr.chan_info[j] = um_ptr->chan_info[i];
                      j++;
                    }
        #endif
                  }
                  else
                  {
                    /*-----------------------------------------------------------------
                      If the channel does not exist, send a FAILURE indication in the
                      confirm message to RRC.
                    -----------------------------------------------------------------*/
                    rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                                um_config_result[i].result = FAILURE;
                    TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL LC %d not config'd -- No Re-establish",um_ptr->chan_info[i].lc_id);
                  }
                  break;
      
                default:
                  /*-------------------------------------------------------------------
                    If the action is Invalid, send a FAILURE indication in the confirm
                    to RRC.
                  -------------------------------------------------------------------*/
                  rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                                       um_config_result[i].result = FAILURE;
                  TDSRLC_MSG2(MSG_LEGACY_ERROR,"Invalid Action %d for channel %d",um_ptr->dl_um_act[i].action,um_ptr->chan_info[i].lc_id);
      
              }
            }
          }
          break;
        default:
          /*-------------------------------------------------------------------
            If the action is Invalid, send a FAILURE indication in the confirm
            to RRC.
          -------------------------------------------------------------------*/
          rrc_rlc_cmd_ptr->cmd.crlc_dl_um_config_cnf.
                               um_config_result[i].result = FAILURE;
          TDSRLC_MSG2(MSG_LEGACY_ERROR,"Invalid Action %d for channel %d",um_ptr->dl_um_act[i].action,um_ptr->chan_info[i].lc_id);
      }
    }
  }

  TDSL2_ACQ_DL_UNLOCK();
  tdsrrc_put_rlc_cmd(rrc_rlc_cmd_ptr);
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (j > 0)
  {
    tdsdl_um_log_ptr.nchan = j;
    tdsrlc_dl_send_um_log_packet(&tdsdl_um_log_ptr);
  }
  if (k > 0)
  {
    tdsdl_log_state.nchan = k;
    tdsrlc_dl_send_state_log_packet(&tdsdl_log_state);
  }
#endif

} /* tdscrlc_dl_um_config_handler() */


/*===========================================================================

FUNCTION TDSCRLC_DL_AM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of an Acknowledged Mode entity.
  it is called when RRC sends a CRLC_DL_AM_CONFIG_REQ to RLC. If a new
  entity is to be established, the function checks if the channels exists.
  If it already exists, an ERR message is dispalyed. If the entity does not
  exist, an downlink table entry for this entity is updated and the state is
  set to TDSDL_DATA_TRANSFER_READY.
  If an entity is to be released,the function checks if the channel is in the
  TDSDL_DATA_TRANSFER_READY state. If it is, the channel is set to the NULL state
  and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the
  TDSDL_DATA_TRANSFER_READY state. If it is, the downlink table entry for the
  channel is updated with the configuration received from RRC.


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_am_config_handler
(
  tdsrlci_dl_am_config_req_type *dl_am_config_req_ptr
)
{
  tdsrlc_am_config_req_type *am_config_req_ptr;
  tdsrlc_am_common_config_type *common_ptr;
  tdsl2_ul_cmd_type *ul_cmd_ptr;
  uint16 i;
  uint32 act_dl_am_chnl = 0, index = 0;
  uint8 ctl_blk_id;
  boolean result = FALSE;
  tdsrlc_channel_action_cnf_e_type action;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  uint8 j = 0;
  uint8 k = 0;
#endif

  /*-------------------------------------------------------------------------
    Post RLCI_DL_CONFIG_CNF to the Uplink for the config completion
  -------------------------------------------------------------------------*/
  am_config_req_ptr = &(dl_am_config_req_ptr->am_config_req);
  ul_cmd_ptr = tdsl2_ul_get_cmd_buf();
  if (ul_cmd_ptr == NULL)
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"Could not get command buf");
    return;
  }
  ul_cmd_ptr->cmd_hdr.cmd_id = TDSRLCI_DL_CONFIG_CNF;
  ul_cmd_ptr->cmd_data.dl_config_cnf.nchan = am_config_req_ptr->nentity;
  TDSRLC_MSG1(MSG_LEGACY_LOW,"Num of AM channels %d",am_config_req_ptr->nentity);

  TDSL2_ACQ_DL_LOCK();
 
  for (i=0; i < am_config_req_ptr->nentity; i++)
  {
    common_ptr = &(am_config_req_ptr->am_info[i]->common_config);
    ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].ul_data_id =
                                          common_ptr->ul_data_id;
    if(am_config_req_ptr->am_act[i].action == TDSRLC_ESTABLISH)
    {
      ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].action = TDSRLCI_ESTABLISH;
    }
    else if(am_config_req_ptr->am_act[i].action == TDSRLC_RELEASE)
    {
      ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].action = TDSRLCI_RELEASE;
    }
    else if(am_config_req_ptr->am_act[i].action == TDSRLC_MODIFY)
    {
		ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].action = TDSRLCI_NO_ACTION;
    }
    ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].rb_id = common_ptr->rb_id;

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
    if (tdsrlc_debug_var & TDSRLC_DBG_CORRUPTION)
    {
      MSG_9(MSG_SSID_TDSCDMA_RLC, MSG_LEGACY_HIGH,
         "Padding_DBG: DL lc_id %d, action %d, dl_cfg_mask %d, \
          ul_cfg_mask %d, ul_rlc_size %d, dl_rlc_size %d",
          common_ptr->dl_data_id, am_config_req_ptr->am_act[i].action, 
          am_config_req_ptr->am_act[i].dl_cfg_mask, 
          am_config_req_ptr->am_act[i].ul_cfg_mask, 
          am_config_req_ptr->am_info[i]->rlc_ul_am_config.ul_rlc_size, 
          am_config_req_ptr->am_info[i]->rlc_dl_am_config.dl_rlc_size, 0, 0, 0);
    }
#endif
    if ((dl_am_config_req_ptr->result[i] == SUCCESS) &&
        (common_ptr->dl_data_id < TDSUE_MAX_DL_LOGICAL_CHANNEL))
    {
      switch(am_config_req_ptr->am_act[i].action)
      {
        case TDSRLC_ESTABLISH:
          if ((tdsdl_ctl_blk_id_tbl[common_ptr->dl_data_id] == TDSINIT_CTL_BLK_ID) &&
              ((ctl_blk_id = tdsdl_assign_ctl_blk()) != TDSINIT_CTL_BLK_ID) &&
              (ctl_blk_id < TDSUE_MAX_DL_RLC_ENTITY))
          {
            tdsdl_ctl_blk_id_tbl[common_ptr->dl_data_id] = ctl_blk_id;
            tdsdl_ctl_blk_id_tbl[common_ptr->dl_control_id] = ctl_blk_id;
            tdsdownlink_table.active_ch +=
              tdsrlci_establish_dl_am(&tdsdownlink_table.ctrl_blk[ctl_blk_id],
                                   am_config_req_ptr->am_info[i]
#if   defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
                                   , &result
#endif 
               );
          
            result = tdsrlci_enh_establish_dl_am(&tdsdownlink_table.ctrl_blk[ctl_blk_id],
                                              common_ptr->dl_data_id);
            
            if (result == FALSE)
            {
              ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = FAILURE;
              break;
            }
            ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = SUCCESS;
            TDSRLC_MSG2(MSG_LEGACY_HIGH,"DL AM Establish:Data ID = %d,index = %d",common_ptr->dl_data_id,ctl_blk_id);

            /*---------------------------------------------------------------------
              Delay timer setting until this RLC's first received PDU.
            ----------------------------------------------------------------------*/
            tdsdownlink_table.ctrl_blk[ctl_blk_id].ctrl.am_ctrl.dl_state_var.fst_rx_pdu = TRUE;          
#if 0
            //if(tdsrrc_get_special_settings_for_testing()==TDSRRC_SPECIAL_SETTINGS_FOR_TESTING_NV_ON)
            if(l2_specail_test_value == TDSL2_SPECIAL_SETTINGS_FOR_CONFORMANCE)
            {
              TDSRLC_MSG1(MSG_LEGACY_HIGH,"RLC_DL: Set 20ms timer,LC %d for MTNET",tdsdownlink_table.ctrl_blk[ctl_blk_id].ctrl.am_ctrl.am_common->dl_data_id);
              /* Start base timer when first PDU on any AM channel is Rcvd */
              if (FALSE == tdsrlc_dl_am_base_tmr_start)
              {
                tdsrlc_dl_am_base_tmr_start = TRUE;
                (void) rex_set_timer(&tdsrlc_dl_am_base_tmr, (uint32)TDSRLC_DL_TIMER_BASE);
              }
              tdsdownlink_table.ctrl_blk[ctl_blk_id].ctrl.am_ctrl.dl_tmr_sts = TDSRLC_AM_TIMER_RUNNING;
            }
            else
            {
#if 0
              if(tdsrrc_get_special_settings_for_testing()==TDSRRC_SPECIAL_SETTINGS_FOR_TESTING_NV_INVALID)
              {
                TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_ERR: MTNET NV is invalid");
              }
              TDSRLC_MSG0(MSG_LEGACY_HIGH,"MTNET NV is not on");
#endif
              TDSRLC_MSG1(MSG_LEGACY_HIGH,"CMCC Conf NV is not on,value %d",l2_specail_test_value);
              tdsdownlink_table.ctrl_blk[ctl_blk_id].ctrl.am_ctrl.dl_tmr_sts = TDSRLC_AM_TIMER_UNDEF_STATUS;
             }
#endif
           TDSRLC_MSG1(MSG_LEGACY_HIGH,"RLC_DL: Set 20ms timer,LC %d",tdsdownlink_table.ctrl_blk[ctl_blk_id].ctrl.am_ctrl.am_common->dl_data_id);
           /* Start base timer when first PDU on any AM channel is Rcvd */
           if (FALSE == tdsrlc_dl_am_base_tmr_start)
           {
             tdsrlc_dl_am_base_tmr_start = TRUE;
             (void) rex_set_timer(&tdsrlc_dl_am_base_tmr, (uint32)TDSRLC_DL_TIMER_BASE);
           }
           tdsdownlink_table.ctrl_blk[ctl_blk_id].ctrl.am_ctrl.dl_tmr_sts = TDSRLC_AM_TIMER_RUNNING;


#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
            tdsrlci_generate_dl_config_event     (&tdsdownlink_table.ctrl_blk[ctl_blk_id],
                                          0);
            /*---------------------------------------------------------------------
              Update the state information for the particular AM entity.
            ----------------------------------------------------------------------*/
            if (j < TDSUE_MAX_AM_ENTITY)
            {
              tdsdl_am_log_ptr.am_info[j] = am_config_req_ptr->am_info[i];
              j++;
            }
            if (k < TDSUE_MAX_DL_LOGICAL_CHANNEL)
            {
              tdsdl_log_state.dl_state[k].rlc_id = common_ptr->dl_data_id;
              tdsdl_log_state.dl_state[k].lc_state = TDSAM_DL_DATA_TRANSFER_READY;
              k++;
            }
#endif
          }
          else
          {
            ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = FAILURE;
            TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL AM LC %d in use,No Establish ",common_ptr->dl_data_id);
          }


          break;

        case TDSRLC_RELEASE:
          if ((ctl_blk_id = tdsdl_ctl_blk_id_tbl[common_ptr->dl_data_id]) != TDSINIT_CTL_BLK_ID &&
              (ctl_blk_id < TDSUE_MAX_DL_RLC_ENTITY))
          {
            tdsdownlink_table.active_ch -=
              tdsrlci_release_dl_am(&tdsdownlink_table.ctrl_blk[ctl_blk_id]);

     
            tdsdl_ctl_blk_id_tbl[common_ptr->dl_data_id] = TDSINIT_CTL_BLK_ID;
            tdsdl_ctl_blk_id_tbl[common_ptr->dl_control_id] = TDSINIT_CTL_BLK_ID;

            for (index = 0, act_dl_am_chnl = 0; index < TDSUE_MAX_DL_LOGICAL_CHANNEL; index++)
            {
              ctl_blk_id = tdsdl_ctl_blk_id_tbl[index];
              if ((TDSINIT_CTL_BLK_ID != ctl_blk_id) && 
                   (ctl_blk_id < TDSUE_MAX_DL_RLC_ENTITY) &&
                  ((TDSDL_NULL_STATE != tdsdownlink_table.ctrl_blk[ctl_blk_id].lc_state) && 
                   (UE_MODE_ACKNOWLEDGED == tdsdownlink_table.ctrl_blk[ctl_blk_id].lc_mode)))
              {
                act_dl_am_chnl++;
                break;
              }
            }

            if ((0 == act_dl_am_chnl) && (TRUE == tdsrlc_dl_am_base_tmr_start))
            {
              /* Since all AM channels are released, stop base dl am timer */
              (void) rex_clr_timer(&tdsrlc_dl_am_base_tmr);
              tdsrlc_dl_am_base_tmr_start = FALSE;
 
  
              TDSRLC_MSG1(MSG_LEGACY_HIGH, "RLC_TMR_DBG: Stopping DL base timer,active_dl_chan %d",tdsdownlink_table.active_ch);
            }
          

            TDSRLC_MSG2(MSG_LEGACY_HIGH, "DL AM LC %d - Release,acitve_dl_channels %d",common_ptr->dl_data_id,tdsdownlink_table.active_ch);
            ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = SUCCESS;
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
            if (k < TDSUE_MAX_DL_LOGICAL_CHANNEL)
            {
              tdsdl_log_state.dl_state[k].rlc_id = common_ptr->dl_data_id;
              tdsdl_log_state.dl_state[k].lc_state = TDSRLC_DL_NULL_STATE;
              k++;
            }
#endif
          }
          else
          {
            ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = FAILURE;
            TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL AM LC %d not config'd,No Release",common_ptr->dl_data_id);
          }
          break;

      case TDSRLC_MODIFY:

        ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = SUCCESS;
        if(am_config_req_ptr->am_act[i].action == TDSRLC_MODIFY)
        {
          while(am_config_req_ptr->am_act[i].dl_cfg_mask != 0)
          {
            action = tdsrlci_get_config_action(&(am_config_req_ptr->am_act[i].dl_cfg_mask));
            switch(action)
            {
              case TDSRLCI_MODIFY :
                ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].action = TDSRLCI_MODIFY;
                if (TDSRLCI_RE_ESTABLISH == tdsrlci_get_config_action(&(am_config_req_ptr->am_act[i].ul_cfg_mask)))
                {
                  ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].action = TDSRLCI_RE_ESTABLISH;
                }

                if ((ctl_blk_id = tdsdl_ctl_blk_id_tbl[common_ptr->dl_data_id]) != TDSINIT_CTL_BLK_ID &&
                    (ctl_blk_id < TDSUE_MAX_DL_RLC_ENTITY))
                {
                  tdsrlci_modify_dl_am(&tdsdownlink_table.ctrl_blk[ctl_blk_id],
                                    am_config_req_ptr->am_info[i]);
                  TDSRLC_MSG1(MSG_LEGACY_HIGH,"DL AM LC %d - Modify",common_ptr->dl_data_id);
                  ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = SUCCESS;
      
  #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                  tdsrlci_generate_dl_config_event     (&tdsdownlink_table.ctrl_blk[ctl_blk_id],
                                                0);
                  if (j < TDSUE_MAX_AM_ENTITY)
                  {
                    tdsdl_am_log_ptr.am_info[j] = am_config_req_ptr->am_info[i];
                    j++;
                  }
  #endif
                }
                else
                {
                  ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = FAILURE;
                  TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL LC %d not config'd,No Modi",common_ptr->dl_data_id);
                }
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
				if( (!(tdsmac_is_hs_cfg_active())) && (tdsrlc_dl_fc.rlc_fc_status== TRUE))
				{
				tdsrlc_dl_fc_reset();
				}
#endif
                break;
    
              case TDSRLCI_STOP:
                ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].action = TDSRLCI_STOP;
              
                if ((ctl_blk_id = tdsdl_ctl_blk_id_tbl[common_ptr->dl_data_id]) != TDSINIT_CTL_BLK_ID &&
                    (ctl_blk_id < TDSUE_MAX_DL_RLC_ENTITY))
                {
                  /* Enter into Stop state. */
                  /*lint -e64 -e641 -e655 */
                  tdsdownlink_table.ctrl_blk[ctl_blk_id].lc_state |= TDSDL_STOP;
                  tdsdownlink_table.ctrl_blk[ctl_blk_id].lc_state &= (~TDSDL_DATA_TRANSFER_READY);
                  /*lint +e64 +e641 +e655 */
                  TDSRLC_MSG1(MSG_LEGACY_HIGH,"DL AM LC %d - Stop",common_ptr->dl_data_id);
                  ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = SUCCESS;
                }
                else
                {
                  ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = FAILURE;
                  TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL AM LC %d not config'd,No Stop",common_ptr->dl_data_id);
                }
                break;
    
              case TDSRLCI_CONTINUE:
                ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].action = TDSRLCI_CONTINUE;
              
                if ((ctl_blk_id = tdsdl_ctl_blk_id_tbl[common_ptr->dl_data_id]) != TDSINIT_CTL_BLK_ID &&
                    (ctl_blk_id < TDSUE_MAX_DL_RLC_ENTITY))
                {
                  /* Remove Stop state. Go back to the original states before */
                  /* it entered into the Stop State.                          */
                  /*lint -e64 -e641 -e655 */
                  tdsdownlink_table.ctrl_blk[ctl_blk_id].lc_state &= (~TDSDL_STOP);
                  tdsdownlink_table.ctrl_blk[ctl_blk_id].lc_state |= TDSDL_DATA_TRANSFER_READY;
                  /*lint +e64 +e641 +e655 */
                  TDSRLC_MSG1(MSG_LEGACY_HIGH,"DL AM LC %d - Continue",common_ptr->dl_data_id);
                  ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = SUCCESS;
                }
                else
                {
                  ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = FAILURE;
                  TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL AM LC %d not config'd,No Stop",common_ptr->dl_data_id);
                }
                break;
    
              case TDSRLCI_RE_ESTABLISH:
                ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].action = TDSRLCI_RE_ESTABLISH;
              
                if((ctl_blk_id = tdsdl_ctl_blk_id_tbl[common_ptr->dl_data_id])
                          != TDSINIT_CTL_BLK_ID &&
                   (ctl_blk_id < TDSUE_MAX_DL_RLC_ENTITY))
                {
                  tdsrlci_re_establish_dl_am(&tdsdownlink_table.ctrl_blk[ctl_blk_id],
                                           am_config_req_ptr->am_info[i]);
                  TDSRLC_MSG1(MSG_LEGACY_HIGH,"Re-establish DL AM LC %d",common_ptr->dl_data_id);
      
                  tdsrlc_enh_free_reseq_q(&tdsdownlink_table.ctrl_blk[ctl_blk_id], TDSRLC_RE_ASSEMBLY_Q_SIZE, 0);

                  ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = SUCCESS;
      
  #ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
                  tdsrlci_generate_dl_config_event     (&tdsdownlink_table.ctrl_blk[ctl_blk_id],
                                                0);
                  /*--------------------------------------------------------------
                    Update the state information for the particular AM entity.
                  --------------------------------------------------------------*/
                  if (j < TDSUE_MAX_AM_ENTITY)
                  {
                    tdsdl_am_log_ptr.am_info[j] = am_config_req_ptr->am_info[i];
                    j++;
                  }
  #endif
                }
                else
                {
                  ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = FAILURE;
                  TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL LC %d not config'd,Unable to Re-establish",common_ptr->dl_data_id);
                }
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
				if( (!(tdsmac_is_hs_cfg_active())) && (tdsrlc_dl_fc.rlc_fc_status== TRUE))
				{
			      tdsrlc_dl_fc_reset();
				}
#endif
                break;
    
              default:
                ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = FAILURE;
                TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL AM %d,Invalid Action",common_ptr->dl_data_id);
    
            }
          }
        }
        break;

      default:
          ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = FAILURE;
          TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL AM %d,Invalid Action",common_ptr->dl_data_id);
      }
    }
    else
    {
      ul_cmd_ptr->cmd_data.dl_config_cnf.am_cnf[i].result = FAILURE;
    }
  }

  TDSL2_ACQ_DL_UNLOCK();

  TDSRLC_MSG0(MSG_LEGACY_LOW,"Put config cnf command into queue");
  tdsl2_ul_put_cmd(ul_cmd_ptr);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (j > 0)
  {
    tdsdl_am_log_ptr.nentity = j;
    tdsrlc_dl_send_am_log_packet(&tdsdl_am_log_ptr);
  }
  if (k > 0)
  {
    tdsdl_log_state.nchan = k;
    tdsrlc_dl_send_state_log_packet(&tdsdl_log_state);
  }
#endif

} /* tdscrlc_dl_am_config_handler() */




/*===========================================================================

FUNCTION RLC_DL_REGISTER_SRVC_HANDLER

DESCRIPTION
  This function handles the registration of the watermark and the callback
  function for the logical channels on the downlink.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_register_srvc_handler
(
  tdsrlc_dl_register_srvc_type *reg_ptr      /* Ptr to WM registration Info   */
)
{
  uint16 index;
  int i;

  TDSL2_ACQ_DL_LOCK();

  TDSRLC_MSG1(MSG_LEGACY_HIGH,"RLC_WM: Register DL WM called,nchan %d",reg_ptr->nchan);

  /*-------------------------------------------------------------------------
    Register the Downlink Watermark and Callback Function (if any) for
    all the channels indicated in the registration Pointer.
  -------------------------------------------------------------------------*/
  for (i = 0; i < reg_ptr->nchan; i++)
  {
    TDSRLC_MSG3(MSG_LEGACY_HIGH,"RLC_WM: LC id %d DL-WM_ptr 0x%x context %d", reg_ptr->rlc_data[i].lc_id, reg_ptr->rlc_data[i].dl_wm_ptr, reg_ptr->rlc_data[i].context);
	
    index = tdsdl_ctl_blk_id_tbl[reg_ptr->rlc_data[i].lc_id];
    if ((reg_ptr->rlc_data[i].lc_id < TDSUE_MAX_DL_LOGICAL_CHANNEL) &&
        (index != TDSINIT_CTL_BLK_ID) &&
        (index < TDSUE_MAX_DL_RLC_ENTITY) &&
        (tdsdownlink_table.ctrl_blk[index].lc_state != TDSDL_NULL_STATE))
    {
       /* Task or Interrupt Context */
      tdsdownlink_table.ctrl_blk[index].context = reg_ptr->rlc_data[i].context;
       
      if (reg_ptr->rlc_data[i].dl_wm_ptr != NULL)
      {
        TDSRLC_MSG1(MSG_LEGACY_MED,"RLC_WM: Register DL WM,LC %d",reg_ptr->rlc_data[i].lc_id);
        tdsdownlink_table.ctrl_blk[index].dl_wm_ptr =
                                            reg_ptr->rlc_data[i].dl_wm_ptr;
      }
      else
      {
        TDSRLC_MSG1(MSG_LEGACY_ERROR,"WM ptr NULL,LC %d",reg_ptr->rlc_data[i].lc_id);
      }

      if (reg_ptr->rlc_data[i].rlc_post_rx_proc_func_ptr != NULL)
      {
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"Register Rx CB fcn,LC %d",reg_ptr->rlc_data[i].lc_id);
        tdsdownlink_table.ctrl_blk[index].rlc_post_rx_func_ptr =
                      reg_ptr->rlc_data[i].rlc_post_rx_proc_func_ptr;
        tdsdownlink_table.ctrl_blk[index].rlc_post_rx_func_ptr_para =
                      reg_ptr->rlc_data[i].rlc_post_rx_func_ptr_para;
      } 
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"LC %d not config yet",reg_ptr->rlc_data[i].lc_id);
    }
  }
  tdsrlc_dl_wm_unreg_f3_print_cnt = 0;

  TDSL2_ACQ_DL_UNLOCK();
  
} /* tdsrlc_dl_register_srvc_handler() */


/*===========================================================================

FUNCTION TDSRLC_DL_DEREGISTER_SRVC_HANDLER

DESCRIPTION
  This function handles the de-registration of the watermark and the callback
  function for the logical channels on the downlink.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_deregister_srvc_handler
(
  tdsrlc_dl_deregister_srvc_type *de_reg_ptr      /* Ptr to WM registration Info   */
)
{
  uint16 index;
  int i;


  TDSL2_ACQ_DL_LOCK();

  /*-------------------------------------------------------------------------
    Register the Downlink Watermark and Callback Function (if any) for
    all the channels indicated in the registration Pointer.
  -------------------------------------------------------------------------*/
  for (i = 0; i < de_reg_ptr->nchan; i++)
  {
    index = tdsdl_ctl_blk_id_tbl[de_reg_ptr->rlc_id[i]];
    if ((de_reg_ptr->rlc_id[i] < TDSUE_MAX_DL_LOGICAL_CHANNEL) &&
        (index != TDSINIT_CTL_BLK_ID) &&
        (index < TDSUE_MAX_DL_RLC_ENTITY))
    {
      /* Task or Interrupt Context */
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"DeRegister DL WM,LC %d",de_reg_ptr->rlc_id[i]);
      tdsdownlink_table.ctrl_blk[index].dl_wm_ptr = NULL;

      TDSRLC_MSG1(MSG_LEGACY_LOW,"DeRegister Rx CB fcn,LC %d",de_reg_ptr->rlc_id[i]);
      tdsdownlink_table.ctrl_blk[index].rlc_post_rx_func_ptr = NULL;
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"LC %d not config yet",de_reg_ptr->rlc_id[i]);
    }
  }

  TDSL2_ACQ_DL_UNLOCK();

} /* tdsrlc_dl_deregister_srvc_handler() */


/*===========================================================================

FUNCTION TDSRLC_REGISTER_AM_SDU_DISCARD_CB_REQ

DESCRIPTION
  Register AM SDU discard callback function.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_register_am_sdu_discard_cb_req
(
  tdsrlc_register_am_sdu_discard_callback_type *dl_sdu_discard_cb
)
{
  uint16 index;
  int i;

  TDSL2_ACQ_DL_LOCK();

  for (i = 0; i < dl_sdu_discard_cb->nchan; i++)
  {
    index = tdsdl_ctl_blk_id_tbl[dl_sdu_discard_cb->sdu_discard_cb[i].lc_id];
    if ((dl_sdu_discard_cb->sdu_discard_cb[i].lc_id <
         TDSUE_MAX_DL_LOGICAL_CHANNEL) &&
        (index != TDSINIT_CTL_BLK_ID) &&
        (index < TDSUE_MAX_DL_RLC_ENTITY) &&
        (tdsdownlink_table.ctrl_blk[index].lc_state != TDSDL_NULL_STATE))
    {
      if (dl_sdu_discard_cb->sdu_discard_cb[i].rlc_am_sdu_discard_cb != NULL)
      {
        TDSRLC_MSG1(MSG_LEGACY_HIGH,"Register DL WM,LC %d",i);
        tdsdownlink_table.ctrl_blk[index].ctrl.am_ctrl.sdu_discard_cb =
                  dl_sdu_discard_cb->sdu_discard_cb[i].rlc_am_sdu_discard_cb;
      }
      else
      {
        TDSRLC_MSG1(MSG_LEGACY_ERROR,"SDU discard cb fcn ptr NULL,LC %d",i);
      }
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"LC %d not config yet",dl_sdu_discard_cb->sdu_discard_cb[i].lc_id);
    }
  }

  TDSL2_ACQ_DL_UNLOCK();

}/* tdsrlc_register_am_sdu_discard_cb_req() */


/*===========================================================================

FUNCTION TDSRLC_DL_DECODE_PDU_HDR

DESCRIPTION
 This function is called by MAC to decode the RLC header of the received PDU.
 For Unacknowledged Mode PDUs, the first byte of the PDU is extracted
 and the new count is determined with the extracted byte. For Acknowledged
 Mode PDUs, the first 2 bytes of the PDU are extracted and the new count
 is determined with the extracted bytes. If ciphering is turned on, the
 structure is updated with the ciphering parameters.

DEPENDENCIES

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_dl_decode_pdu_hdr
(
   tdsrlc_dl_tb_hdr_info_type *hdr_ptr,
   tdsrlc_dl_tb_cipher_info_type *cipher_ptr
)
{
  tdsrlci_dl_ctrl_blk_type *ctl_blk = NULL;
  uint16 rlc_pdu_header = 0, rx_sn = 0, cmp_sn;
  uint8  ctl_index;
/*Status flag to return the PDU decode status*/
  boolean status = TRUE;
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  int cipher_idx;
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  boolean old_config = FALSE;

  /*---------------------------------------------------------------------
    Init cipher flag to be OFF.
  ---------------------------------------------------------------------*/
  cipher_ptr->cipher_flag = OFF;
  cipher_ptr->ciphering_algo = UE_UMTS_CIPHER_ALGO_NONE; 
  cipher_ptr->cipher_offset = 0;


  /* Get the new ctrl Index and ctrl blk ptr */
  ctl_index = TDSRLCI_GET_CTRL_INDEX(hdr_ptr->rlc_id);
  if (( ctl_index == TDSINIT_CTL_BLK_ID ) || ( ctl_index >= TDSUE_MAX_DL_RLC_ENTITY ))
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: rlc_id %d is not yet configured",hdr_ptr->rlc_id);
    status = FALSE;
    return(status);
  }
  /*Get the control block ptr*/
  ctl_blk = TDSRLCI_GET_CTRL_BLK_PTR(ctl_index);
  if ( ctl_blk->lc_state == TDSDL_NULL_STATE )
  {
    TDSRLC_MSG2(MSG_LEGACY_ERROR,"RLC_ERR: lc_state %d is NULL for rlc_id %d",ctl_blk->lc_state,hdr_ptr->rlc_id);
    status = FALSE;
    return(status);
  }


  if (tdsdl_ctl_blk_id_tbl[hdr_ptr->rlc_id] != TDSINIT_CTL_BLK_ID &&
      ctl_blk->lc_state != TDSDL_NULL_STATE)
  {
    if (ctl_blk->lc_mode != UE_MODE_TRANSPARENT)
    {
      if (tdsrlc_debug_var & TDSRLC_DBG_DL_CIPHERING)
      {
        TDSRLC_MSG3(MSG_LEGACY_HIGH,"B4 cipher %x %x %x", tdsrlc_copy_to_word(hdr_ptr->hdr_ptr),
                  tdsrlc_copy_to_word(hdr_ptr->hdr_ptr + 4),
                  tdsrlc_copy_to_word(hdr_ptr->hdr_ptr + 8));
      }

      /*---------------------------------------------------------------------
        It's in UNACKNOWLEDGED mode. Get the 1st byte of the PDU to obtain
        the SN and form count_c.
      ---------------------------------------------------------------------*/
      if (ctl_blk->lc_mode == UE_MODE_UNACKNOWLEDGED)
      {
        rlc_pdu_header = b_unpackb(hdr_ptr->hdr_ptr, hdr_ptr->rlc_pdu_start_offset, 8);
        rx_sn = (rlc_pdu_header >> 1) & 0x007F;

        /*---------------------------------------------------------------------
          Yes, we need to decipher the incoming pdu.
        ---------------------------------------------------------------------*/
        cipher_ptr->cipher_offset = hdr_ptr->rlc_pdu_start_offset + 8;
        if (tdsrlci_dl_um_cipher_pdu(ctl_blk, cipher_ptr, rx_sn))
        {
          cipher_ptr->count_c = tdsrlci_dl_um_get_count_c(ctl_blk, rx_sn);
          /*-------------------------------------------------------------------
            De-ciphering this incoming PDU, since ciphering algorithm is UEA_1
          -------------------------------------------------------------------*/
          if ((ctl_blk->cipher.cipher_algo == UE_UMTS_CIPHER_ALGO_UEA1)
               || (ctl_blk->cipher.cipher_algo == UE_UMTS_CIPHER_ALGO_UEA2)
             )
          {
            cipher_ptr->cipher_flag = ON;
            cipher_ptr->ciphering_algo = ctl_blk->cipher.cipher_algo;
            cipher_ptr->cipher_key_id = ctl_blk->cipher.cipher_key_id;
            cipher_ptr->rb_id = (rb_id_type)ctl_blk->cipher.rb_id;
            if (tdsrlc_debug_var & TDSRLC_DBG_DL_CIPHERING)
            {
              TDSRLC_MSG3(MSG_LEGACY_HIGH,"Cipher Key %x, cnt_c %x, offset %x",
                        cipher_ptr->cipher_key_id, cipher_ptr->count_c,
                                             cipher_ptr->cipher_offset);
            }
          }
          ctl_blk->cipher.new_cipher_on = FALSE;
        }
      }
      /*---------------------------------------------------------------------
        If in ACKNOWLEDGED mode. Get the first 2 bytes of the PDU to obtain
        the SN and form count_c.
      ---------------------------------------------------------------------*/
      else if (ctl_blk->lc_mode == UE_MODE_ACKNOWLEDGED)
      {
        rlc_pdu_header = b_unpackw(hdr_ptr->hdr_ptr, hdr_ptr->rlc_pdu_start_offset, 16);
        /*---------------------------------------------------------------------
          Ok, it is a data PDU.
        ---------------------------------------------------------------------*/
        if (rlc_pdu_header & 0x8000)
        {
          /* Drop data PDUs if RESET_ACK is recived but RLC is still in 
             RESET_PENDING state*/
          if ((ctl_blk->lc_state & TDSDL_RESET_PENDING) && 
              (TRUE == ctl_blk->ctrl.am_ctrl.reset_ack_rcvd))
          {
            if (tdsrlc_debug_var & TDSRLC_DBG_DL_CIPHERING)
            {
              TDSRLC_MSG3(MSG_LEGACY_HIGH,"NA: LC %d is in RESET PENDING - DATA PDU Hdr: 0x%x Payload: 0x%x in this TTI",
                              hdr_ptr->rlc_id, rlc_pdu_header,
                              b_unpackw(hdr_ptr->hdr_ptr, hdr_ptr->rlc_pdu_start_offset+16, 32));
            }       
            return FALSE;
          }
          else
          { 
            ctl_blk->ctrl.am_ctrl.reset_ack_rcvd = FALSE;
          }

          rx_sn = (rlc_pdu_header >> 3) & 0x0FFF ;
          /* set cipher_offset ir-respective of cipher flag to pass on
             header number of bits to L1 */
          cipher_ptr->cipher_offset = hdr_ptr->rlc_pdu_start_offset + 16;

          /*---------------------------------------------------------------------
            Yes, we need to decipher the incoming pdu.
          ---------------------------------------------------------------------*/
          if (ctl_blk->cipher.cipher_mode != TDSNO_CIPHER &&
              tdsrlci_dl_am_cipher_pdu(ctl_blk, rx_sn))
          {
            /*---------------------------------------------------------------------
              Find the ciphering starting.
            ---------------------------------------------------------------------*/
            if (ctl_blk->cipher.act_old_cfg1)
            {
              cmp_sn = ctl_blk->cipher.act_sn_old1;
            }
            else if (ctl_blk->cipher.act_old_cfg)
            {
              cmp_sn = ctl_blk->cipher.act_sn_old;
            }
            else
            {
              cmp_sn = ctl_blk->cipher.act_sn;
            }

            /*---------------------------------------------------------------------
            This rx'd PDU needs to be de-ciphering.
            ---------------------------------------------------------------------*/
            if (ctl_blk->cipher.cipher_all ||
                !(TDSRLCI_SEQ_GT(cmp_sn, rx_sn, TDSRLC_AM_MODULUS)))
            {
              ctl_blk->cipher.new_cipher_on = FALSE;
              cipher_ptr->count_c = tdsrlci_dl_am_get_count_c(ctl_blk, rx_sn,
                                                      &(cipher_ptr->cipher_key_id),
                                                      &cipher_ptr->cipher_flag, &old_config);
              cipher_ptr->rb_id = (rb_id_type)ctl_blk->cipher.rb_id;
              cipher_ptr->ciphering_algo = ctl_blk->cipher.cipher_algo;

              if (tdsrlc_debug_var & TDSRLC_DBG_DL_CIPHERING)
              {
                TDSRLC_MSG3(MSG_LEGACY_HIGH,"Cipher Key %x, cnt_c %x, offset %x",
                          cipher_ptr->cipher_key_id, cipher_ptr->count_c,
                                 cipher_ptr->cipher_offset);
              }
            }
          } /* end of Need to cipher pdu */
        } /* end of data PDU */
        else /* its a control PDU */
        {
          /* Set reset_ack_rcvd, if we were waiting for RESET_ACK and have
             recived RESET_ACK with expected RSN */
          if ((ctl_blk->lc_state & TDSDL_RESET_PENDING) && 
              ((rlc_pdu_header & 0xF000) == 0x2000) && 
                (NULL != tdsrlci_get_ctl_blk_ptr(ctl_blk->ctrl.am_ctrl.am_common->ul_data_id)) &&
                (((rlc_pdu_header & 0x0800) >> 11) == 
                  (uint16)tdsrlci_get_reset_sn(ctl_blk->ctrl.am_ctrl.am_common->ul_data_id)) &&
                  tdsrlci_get_reset_ack_status(ctl_blk->ctrl.am_ctrl.am_common->ul_data_id))
          {
            ctl_blk->ctrl.am_ctrl.reset_ack_rcvd = TRUE;
          }
          /* set cipher_offset ir-respective of cipher flag to pass on
             header number of bits to L1 - there is no AM header for 
             control PDU hence dont add anything to rlc_pdu_start_offset */
          cipher_ptr->cipher_offset = hdr_ptr->rlc_pdu_start_offset;
        }
      } /* AM & UM  */

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
      /*---------------------------------------------------------------------
        Ciphering parameters logging is done for SRBs and selectively for
        user plane data channels.
      ---------------------------------------------------------------------*/
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
      if (tdsrlci_dl_pdu_cipher_log_on &&
#else
      if (tdsrlci_dl_pdu_cipher_log_on && !tdsrlci_dl_pdu_cipher_log.cipher_pdu_logging &&
#endif
         (ON == cipher_ptr->cipher_flag) &&
          ((hdr_ptr->rlc_id < TDSRLC_DL_AM_USR_PLANE_START_LC_ID) || (rx_sn %100 == 0) ||
           (rx_sn < 20)))
      {
        /*---------------------------------------------------------------------
          Skip the status PDU received, since status PDU is not ciphered.
        ---------------------------------------------------------------------*/
        if (ctl_blk->lc_mode == UE_MODE_ACKNOWLEDGED &&
            (rlc_pdu_header & 0x8000) == 0)
        {
          return status;
        }
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
        TDSL2_MUTEX_LOCK(&tdsrlci_dl_pdu_cipher_log_mutex);
        if (tdsrlci_dl_pdu_cipher_log_ptr->num_pdu < TDSMAX_CIPHER_PDU)
        {
          cipher_idx = tdsrlci_dl_pdu_cipher_log_ptr->num_pdu;
          tdsrlci_dl_pdu_cipher_log_ptr->cipher_log_data[cipher_idx].log_dl_rlc_id= hdr_ptr->rlc_id;
          tdsrlci_dl_pdu_cipher_log_ptr->cipher_log_data[cipher_idx].cipher_key_idx =
                                                            cipher_ptr->cipher_key_id;
          tdsrlci_dl_pdu_cipher_log_ptr->cipher_log_data[cipher_idx].cipher_algo=
                                                          ctl_blk->cipher.cipher_algo;
          tdsrlci_dl_pdu_cipher_log_ptr->cipher_log_data[cipher_idx].count_c= cipher_ptr->count_c;
          tdsrlci_dl_pdu_cipher_log_ptr->cipher_log_data[cipher_idx].b4_cipher =
                      b_unpackd(hdr_ptr->hdr_ptr, hdr_ptr->rlc_pdu_start_offset, 32);
          /*------------------------------------------------------------------
            After filling in the PDU cipher parameters, increase the index
            to the next entry.
          --------------------------------------------------------------------*/
          tdsrlci_dl_pdu_cipher_log_ptr->num_pdu ++;
        }
        TDSL2_MUTEX_UNLOCK(&tdsrlci_dl_pdu_cipher_log_mutex);
#else
        if (tdsrlci_dl_pdu_cipher_log.num_pdu < TDSMAX_CIPHER_PDU)
        {
          cipher_idx = tdsrlci_dl_pdu_cipher_log.num_pdu;
          tdsrlci_dl_pdu_cipher_log.cipher_log_data[cipher_idx].log_dl_rlc_id= hdr_ptr->rlc_id;
          tdsrlci_dl_pdu_cipher_log.cipher_log_data[cipher_idx].cipher_key_idx =
                                                            cipher_ptr->cipher_key_id;
          tdsrlci_dl_pdu_cipher_log.cipher_log_data[cipher_idx].cipher_algo=
                                                          ctl_blk->cipher.cipher_algo;
          tdsrlci_dl_pdu_cipher_log.cipher_log_data[cipher_idx].count_c= cipher_ptr->count_c;
          tdsrlci_dl_pdu_cipher_log.cipher_log_data[cipher_idx].b4_cipher =
                      b_unpackd(hdr_ptr->hdr_ptr, hdr_ptr->rlc_pdu_start_offset, 32);
          /*------------------------------------------------------------------
            After filling in the PDU cipher parameters, increase the index
            to the next entry.
          --------------------------------------------------------------------*/
          tdsrlci_dl_pdu_cipher_log.num_pdu ++;
        }
#endif
      }
#endif //FEATURE_TDSCDMA_RLC_QXDM_LOGGING
    } /* NON TM */
  }
  else
  {
    /*Set the status flag to FALSE in case the LC ID is not configured*/
    status = FALSE;
    TDSRLC_MSG2(MSG_LEGACY_ERROR,"RLC_ERR: RLC Channel %d not exist,%d",hdr_ptr->rlc_id,ctl_blk->cipher.cipher_mode);
  }
  return status;
}/* tdsrlc_dl_decode_pdu_hdr() */


/*===========================================================================

FUNCTION TDSGET_AM_CTL_RLK

DESCRIPTION
  This function return the pointer of control block for an AM.

DEPENDENCIES
  None

RETURN VALUE
  Pointer to the AM control block.

SIDE EFFECTS
  None
===========================================================================*/

tdsrlci_dl_ctrl_blk_type *tdsget_am_ctl_blk
(
  uint8 lc_id
)
{
  /* just for KW error*/
  if (tdsdl_ctl_blk_id_tbl[lc_id] >= TDSUE_MAX_DL_RLC_ENTITY)
  {
    return (&(tdsdownlink_table.ctrl_blk[0]));
  }
  
  return (&(tdsdownlink_table.ctrl_blk[tdsdl_ctl_blk_id_tbl[lc_id]]));
} /* tdsget_am_ctl_blk */

/*===========================================================================

FUNCTION TDSRLCi_CHK_DL_RESET_PENDING

DESCRIPTION
  This function checks if the DL is in RESET pending state.

DEPENDENCIES
  None

RETURN VALUE
  Boolean indicating, RESET pending or not.

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrlci_chk_dl_reset_pending
(
  rlc_lc_id_type lc_id
)
{
  tdsrlci_dl_ctrl_blk_type *dl_ctrl_blk_ptr = tdsget_am_ctl_blk(lc_id);
  boolean               ret_val = FALSE;
  
  /*lint -save -e655 */
  if ((dl_ctrl_blk_ptr != NULL) && 
      (dl_ctrl_blk_ptr->lc_state & (tdsrlci_dl_state_e_type)TDSDL_RESET_PENDING))
  {
    ret_val = TRUE;
  }
  /*lint -restore */
  return ret_val;
} /* tdsrlci_chk_dl_reset_pending */

/*===========================================================================

FUNCTION TDSRLCI_DL_PROC_RESET_MSG

DESCRIPTION
  Process the RLCI_UL_RESET_DONE and RLCI_DL_RESET_REQ messages from
  UL AM RLC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_dl_proc_reset_msg
(
  tdsl2_dl_cmd_enum_type cmd_id,
  rlc_lc_id_type dl_rlc_data_id
)
{
  uint8 ctl_id;
  tdsrlci_dl_ctrl_blk_type *ctrl_blk;
  rex_crit_sect_type *datapath_mutex = NULL;

  TDSL2_ACQ_DL_LOCK();

  ctl_id = tdsdl_ctl_blk_id_tbl[dl_rlc_data_id];

  if ((ctl_id == TDSINIT_CTL_BLK_ID) || (ctl_id >= TDSUE_MAX_DL_RLC_ENTITY))
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC-DL LC Id:%d not found",dl_rlc_data_id);

  TDSL2_ACQ_DL_UNLOCK();
    return;
  }

  ctrl_blk = &tdsdownlink_table.ctrl_blk[ctl_id];
  TDSRLC_MSG1(MSG_LEGACY_LOW,"Received Reset related msg id:%d",cmd_id);

  if (cmd_id == TDSRLCI_UL_RESET_DONE)
  {
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
    uint16 rlc_win_size = 0;
    tdsrlci_ul_ctrl_blk_type *ul_ctrl_blk_ptr =  NULL;
    datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(ctrl_blk->ctrl.am_ctrl.am_common->ul_data_id);

    ul_ctrl_blk_ptr =  tdsrlci_get_ctl_blk_ptr( ctrl_blk->ctrl.am_ctrl.am_common->ul_data_id);
#endif//defined(FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL)
    /* Complete the RESET procedure. Return back to the original state. */
    /*lint -e64 -e641 */
    tdsdownlink_table.ctrl_blk[ctl_id].lc_state &= (~TDSDL_RESET_PENDING);
    /*lint +e64 +e641 */
#if defined(FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL)
    if ((tdsrlci_dl_fc_status())&&  (ul_ctrl_blk_ptr != NULL)
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS
          && (ul_ctrl_blk_ptr->ctrl.am_ctrl.ul_state.dl_cpu_base_fc_f)
#endif  /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS */
        /*Change the peer win to the WIN SIZE triggered by FC before RESET*/
        )
    {
      if ( ctrl_blk->ctrl.am_ctrl.dl_ctl_cnfg->rlc_info.rx_window_size > TDSMAX_RX_WINDOW_SIZE)
      {
        ctrl_blk->ctrl.am_ctrl.dl_state_var.rx_win = TDSMAX_RX_WINDOW_SIZE;
      }
      else
      {
        ctrl_blk->ctrl.am_ctrl.dl_state_var.rx_win =
                ctrl_blk->ctrl.am_ctrl.dl_ctl_cnfg->rlc_info.rx_window_size;
      }
      tdsrlc_dl_multi_client_fc_modify_max_flow
        (ctrl_blk->ctrl.am_ctrl.am_common->ul_data_id,
         ctrl_blk->ctrl.am_ctrl.dl_state_var.rx_win,
         FALSE);
                          
      rlc_win_size = tdsrlc_dl_multi_client_fc_get_win_size
        (ctrl_blk->ctrl.am_ctrl.am_common->ul_data_id);
      if(rlc_win_size < ctrl_blk->ctrl.am_ctrl.dl_state_var.rx_win)
      {
        tdsrlci_change_peer_tx_win(ctrl_blk->ctrl.am_ctrl.am_common->ul_data_id,
                      rlc_win_size);
      }
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"Sending  win SUFI with win size %d configured by FC before RESET",rlc_win_size);
    }
    else
#endif
    {
      /*----------------------------------------------------------------------
       Set the receiver's window size. If the window size is greater than
       the maximum RX_Window size that UE can handle, send a window sufi
       indicating that the receiver's window size is TDSMAX_RX_WINDOW_SIZE
      ----------------------------------------------------------------------*/
      if(ctrl_blk->ctrl.am_ctrl.dl_ctl_cnfg->rlc_info.rx_window_size > TDSMAX_RX_WINDOW_SIZE)
      {
        TDSRLC_MSG1(MSG_LEGACY_LOW,"Max window size on DL = %d",TDSMAX_RX_WINDOW_SIZE);
        ctrl_blk->ctrl.am_ctrl.dl_state_var.rx_win = TDSMAX_RX_WINDOW_SIZE;
        tdsrlc_dl_multi_client_fc_modify_max_flow
          (ctrl_blk->ctrl.am_ctrl.am_common->ul_data_id,
           ctrl_blk->ctrl.am_ctrl.dl_state_var.rx_win,
           FALSE);
        tdsrlci_change_peer_tx_win(ctrl_blk->ctrl.am_ctrl.am_common->ul_data_id,
                                tdsrlc_dl_multi_client_fc_get_win_size
                                (ctrl_blk->ctrl.am_ctrl.am_common->ul_data_id));
      }
      else
      {
        ctrl_blk->ctrl.am_ctrl.dl_state_var.rx_win =
                 ctrl_blk->ctrl.am_ctrl.dl_ctl_cnfg->rlc_info.rx_window_size;
        tdsrlc_dl_multi_client_fc_modify_max_flow
          (ctrl_blk->ctrl.am_ctrl.am_common->ul_data_id,
           ctrl_blk->ctrl.am_ctrl.dl_state_var.rx_win,
           TRUE);

      }
    }
    ctrl_blk->ctrl.am_ctrl.dl_state_var.vr_mr =
                                ctrl_blk->ctrl.am_ctrl.dl_state_var.rx_win;
    TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);

#if  defined( FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL)
    if (tdsrlci_ul_fc_status())
    {
      if (ul_ctrl_blk_ptr 
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS
         && ul_ctrl_blk_ptr->ctrl.am_ctrl.ul_state.ul_cpu_base_fc_f
#endif  /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL_WITH_QOS */
      )
      {
         /*After RESET is done configure TX WIN to previous size*/
         tdsrlci_ul_fc_calc_new_tx_win_size(ul_ctrl_blk_ptr);
      }
    }
#endif
    tdsrlci_initiate_reset_for_lower_dl_hfn(dl_rlc_data_id);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
    tdsdl_log_state.dl_state[0].rlc_id = dl_rlc_data_id;
    tdsdl_log_state.dl_state[0].lc_state = TDSAM_DL_DATA_TRANSFER_READY;
    tdsdl_log_state.nchan = 1;
    tdsrlc_dl_send_state_log_packet(&tdsdl_log_state);
#endif

  }
  else if (cmd_id == TDSRLCI_DL_RESET_REQ)
  {
    /* Rx RESET request from the peer side via the DL AM RLC. */
    tdsrlci_reset_dl_am(&tdsdownlink_table.ctrl_blk[ctl_id], FALSE);
  }
  else
  {
    TDSRLC_MSG0(MSG_LEGACY_HIGH,"Unknown DL RLC reset msg");
  }
  
  TDSL2_ACQ_DL_UNLOCK();
  
} /* tdsrlci_dl_proc_reset_msg() */


/*===========================================================================

FUNCTION TDSCRLC_DL_RELEASE_ALL_HANDLER

DESCRIPTION
  Releases all downlink Channels and sends a confirmation to the uplink
  after release.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_release_all_handler(
  void
  )
#if 0
{
  uint8 ctrl_blk_id;                /* Index into the Downlink Table       */
  tdsl2_ul_cmd_type *ul_cmd;
  uint8 i = 0;
  uint32 act_dl_am_chnl = 0, index = 0;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  uint8 j = 0;
#endif

  /*-------------------------------------------------------------------------
    Get command Buffer for posting Release ALL CNF to RLC uplink.
    -------------------------------------------------------------------------*/
  if ((ul_cmd = tdsl2_ul_get_cmd_buf()) == NULL)
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"Could not get ul cmd buffer");
    return;
  }
  ul_cmd->cmd_hdr.cmd_id = TDSRLCI_DL_RELEASE_ALL_CNF;

  TDSRLC_MSG1(MSG_LEGACY_LOW,"Release ALL Downlink Channels,No.of DL-LC %d",tdsdownlink_table.active_ch);

  TDSL2_ACQ_DL_LOCK();

  /*-------------------------------------------------------------------------
    Release all active Downlink Channels.
  -------------------------------------------------------------------------*/
  while (tdsdownlink_table.active_ch > 0)
  {
    /*-----------------------------------------------------------------------
      Check if the channel is in NULL state. If it is not, release
      the channel.
    -----------------------------------------------------------------------*/
    if ((ctrl_blk_id = tdsdl_ctl_blk_id_tbl[i]) != TDSINIT_CTL_BLK_ID)
    {
      TDSRLC_MSG3(MSG_LEGACY_HIGH,"Release DL LC %d, Index %d no.of DL-LC %d",i, ctrl_blk_id,tdsdownlink_table.active_ch);

      if (tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_mode == UE_MODE_ACKNOWLEDGED)
      {
        tdsdownlink_table.active_ch -=
               tdsrlci_release_dl_am(&tdsdownlink_table.ctrl_blk[ctrl_blk_id]);

        tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_state = TDSDL_NULL_STATE;
        tdsdl_ctl_blk_id_tbl[i] = TDSINIT_CTL_BLK_ID;
      }
      else
      {
        tdsdownlink_table.active_ch--;

        if((tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_mode == UE_MODE_UNACKNOWLEDGED) 
          )
        {
          {
            tdsrlci_release_dl_um(&(tdsdownlink_table.ctrl_blk[ctrl_blk_id]));
            tdsdl_ctl_blk_id_tbl[i] = TDSINIT_CTL_BLK_ID;                
          }
        }
        else
        {
          tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_state = TDSDL_NULL_STATE;
          tdsdl_ctl_blk_id_tbl[i] = TDSINIT_CTL_BLK_ID;
        }
      }

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
      if(j < TDSUE_MAX_DL_LOGICAL_CHANNEL)
      {
        tdsdl_log_state.dl_state[j].rlc_id = i;
        tdsdl_log_state.dl_state[j].lc_state = TDSRLC_DL_NULL_STATE;
        j++;
      }
      else
      {
        TDSRLC_MSG0(MSG_LEGACY_ERROR,"Number of valid fields in tdsdl_log_state.dl_state[] exceed TDSUE_MAX_DL_LOGICAL_CHANNEL");
      }
#endif
    }

    i++;
  }


  for (index = 0, act_dl_am_chnl = 0; index < TDSUE_MAX_DL_LOGICAL_CHANNEL; index++)
  {
    ctrl_blk_id = tdsdl_ctl_blk_id_tbl[index];
    if ((TDSINIT_CTL_BLK_ID != ctrl_blk_id) && 
        ((TDSDL_NULL_STATE != tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_state) && 
         (UE_MODE_ACKNOWLEDGED == tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_mode)))
    {
      act_dl_am_chnl++;
      break;
    }
  }

    if ((0 == act_dl_am_chnl) && (TRUE == tdsrlc_dl_am_base_tmr_start))
    {
      /* Since all AM channels are released, stop base dl am timer */
      (void) rex_clr_timer(&tdsrlc_dl_am_base_tmr);
      tdsrlc_dl_am_base_tmr_start = FALSE;
      TDSRLC_MSG1(MSG_LEGACY_HIGH, "RLC_TMR_DBG: Stopping DL base timer,active_dl_chan %d",tdsdownlink_table.active_ch);
    }

  TDSL2_ACQ_DL_UNLOCK();

  /*-------------------------------------------------------------------------
    Post Release ALL Done to Uplink 
   -------------------------------------------------------------------------*/
  tdsl2_ul_put_cmd(ul_cmd);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (j > 0)
  {
    tdsdl_log_state.nchan = j;
    tdsrlc_dl_send_state_log_packet(&tdsdl_log_state);
  }
#endif

} /* tdscrlc_dl_release_all_handler() */
#else
{
  uint8 ctrl_blk_id;                /* Index into the Downlink Table       */
  tdsrrc_rlc_cmd_type *rrc_rlc_cmd_ptr;    /* Command Buffer for sending Release  */
                                    /* All confirm to RRC                  */
  uint8 i = 0;
  uint32 act_dl_am_chnl = 0, index = 0;

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  uint8 j = 0;
#endif

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
  TDSRLC_MSG1(MSG_LEGACY_LOW,"Release ALL Downlink Channels,No.of DL-LC %d",tdsdownlink_table.active_ch);

  TDSL2_ACQ_DL_LOCK();

  /*-------------------------------------------------------------------------
    Release all active Downlink Channels.
  -------------------------------------------------------------------------*/
  while (tdsdownlink_table.active_ch > 0)
  {
    /*-----------------------------------------------------------------------
      Check if the channel is in NULL state. If it is not, release
      the channel.
    -----------------------------------------------------------------------*/
    if (((ctrl_blk_id = tdsdl_ctl_blk_id_tbl[i]) != TDSINIT_CTL_BLK_ID) &&
         (ctrl_blk_id < TDSUE_MAX_DL_RLC_ENTITY))
    {
      TDSRLC_MSG3(MSG_LEGACY_MED,"Release DL LC %d, Index %d no.of DL-LC %d",i, ctrl_blk_id,tdsdownlink_table.active_ch);

      if (tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_mode == UE_MODE_ACKNOWLEDGED)
      {
        tdsdownlink_table.active_ch -=
               tdsrlci_release_dl_am(&tdsdownlink_table.ctrl_blk[ctrl_blk_id]);

        tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_state = TDSDL_NULL_STATE;
        tdsdl_ctl_blk_id_tbl[i] = TDSINIT_CTL_BLK_ID;
      }
      else
      {
        tdsdownlink_table.active_ch--;

        if(tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_mode == UE_MODE_UNACKNOWLEDGED) 
        {
          {
            tdsrlci_release_dl_um(&(tdsdownlink_table.ctrl_blk[ctrl_blk_id]));
            tdsdl_ctl_blk_id_tbl[i] = TDSINIT_CTL_BLK_ID;                
          }
        }
        else
        {
          tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_state = TDSDL_NULL_STATE;
          tdsdl_ctl_blk_id_tbl[i] = TDSINIT_CTL_BLK_ID;
        }
      }

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
      if(j < TDSUE_MAX_DL_LOGICAL_CHANNEL)
      {
        tdsdl_log_state.dl_state[j].rlc_id = i;
        tdsdl_log_state.dl_state[j].lc_state = TDSRLC_DL_NULL_STATE;
        j++;
      }
      else
      {
        TDSRLC_MSG0(MSG_LEGACY_ERROR,"Number of valid fields in tdsdl_log_state.dl_state[] exceed TDSUE_MAX_DL_LOGICAL_CHANNEL");
      }
#endif
    }

    i++;
  }


  for (index = 0, act_dl_am_chnl = 0; index < TDSUE_MAX_DL_LOGICAL_CHANNEL; index++)
  {
    ctrl_blk_id = tdsdl_ctl_blk_id_tbl[index];
    if ((TDSINIT_CTL_BLK_ID != ctrl_blk_id) && 
        (ctrl_blk_id < TDSUE_MAX_DL_RLC_ENTITY) &&
        ((TDSDL_NULL_STATE != tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_state) && 
         (UE_MODE_ACKNOWLEDGED == tdsdownlink_table.ctrl_blk[ctrl_blk_id].lc_mode)))
    {
      act_dl_am_chnl++;
      break;
    }
  }

    if ((0 == act_dl_am_chnl) && (TRUE == tdsrlc_dl_am_base_tmr_start))
    {
      /* Since all AM channels are released, stop base dl am timer */
      (void) rex_clr_timer(&tdsrlc_dl_am_base_tmr);
      tdsrlc_dl_am_base_tmr_start = FALSE;
      TDSRLC_MSG1(MSG_LEGACY_HIGH, "RLC_TMR_DBG: Stopping DL base timer,active_dl_chan %d",tdsdownlink_table.active_ch);
    }

  TDSL2_ACQ_DL_UNLOCK();

  /*-------------------------------------------------------------------------
    Post Release All Confirm to RRC.
  -------------------------------------------------------------------------*/
  TDSRLC_MSG0(MSG_LEGACY_HIGH,"Post Release_All confirm to RRC");
  tdsrrc_put_rlc_cmd(rrc_rlc_cmd_ptr);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (j > 0)
  {
    tdsdl_log_state.nchan = j;
    tdsrlc_dl_send_state_log_packet(&tdsdl_log_state);
  }
#endif

} /* tdscrlc_dl_release_all_handler() */

#endif


/*===========================================================================

FUNCTION TDSRLCI_DL_GET_CTL_BLK_PTR

DESCRIPTION
  Builds uplink PDUs for transmission in a TTI.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
tdsrlci_dl_ctrl_blk_type *tdsrlci_dl_get_ctl_blk_ptr
(
  uint8 ch_id
)
{
  uint8 ctl_blk_id;
  tdsrlci_dl_ctrl_blk_type *ctl_blk_ptr = NULL;

  if (ch_id < TDSUE_MAX_DL_LOGICAL_CHANNEL &&
      (ctl_blk_id = tdsdl_ctl_blk_id_tbl[ch_id]) != TDSINIT_CTL_BLK_ID &&
      ctl_blk_id < TDSUE_MAX_DL_RLC_ENTITY)
  {
    ctl_blk_ptr = &tdsdownlink_table.ctrl_blk[ctl_blk_id];
  }

  return (ctl_blk_ptr);

} /* tdsrlci_dl_get_ctl_blk_ptr() */

/*===========================================================================

FUNCTION TDSRLCI_GET_RX_WIN_SIZE

DESCRIPTION
  Obtain the rx_win_size.

DEPENDENCIES
  None.

RETURN VALUE
  rx_win_size.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlci_get_rx_win_size (rlc_lc_id_type rlc_id)
{
  tdsrlci_dl_ctrl_blk_type *ctl_blk_ptr;
  uint16 rx_win = 0;

  if ((ctl_blk_ptr = tdsrlci_dl_get_ctl_blk_ptr(rlc_id)) != NULL)
  {
    rx_win = MIN(TDSMAX_RX_WINDOW_SIZE,
      ctl_blk_ptr->ctrl.am_ctrl.dl_ctl_cnfg->rlc_info.rx_window_size);
  }

  return (rx_win);
} /* tdsrlci_get_rx_win_size() */



#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING

/*===========================================================================

FUNCTION TDSRLCI_DL_AM_HANDLE_QXDM_LOG_TIMER

DESCRIPTION

This function is called when the qxdm log timer expires. This functionthen
prepares to send the packet to qxdm for logging purposes. TDSRLC_AM_LOG_TIMER_BASE
is currently set to 3seconds.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void tdsrlci_dl_am_handle_qxdm_log_timer  
(
  uint32 dl_log_timer_sig
  /*lint -esym(715,dl_log_timer_sig) */
)
{
#if 0
	/* Set this log timer tick again */
  (void) rex_set_timer(&tdsdl_tmr_log_blk, (uint32)TDSRLC_AM_LOG_TIMER_BASE);
#endif
  /*---------------------------------------------------------------------
    Send the log packet to the qxdm for logging purposes every 3seconds.
  ---------------------------------------------------------------------*/
  if (log_status(TDSCDMA_DL_RLC_AM_STAT_LOG_PACKET))
  {
    TDSRLC_MSG0(MSG_LEGACY_MED,"Sending log packet");
    tdsrlc_dl_tx_am_statistic_pkt(&tdsdownlink_table);
  }
} /* tdsrlci_dl_am_handle_qxdm_log_timer  () */

#endif // FEATURE_TDSCDMA_RLC_QXDM_LOGGING


/*===========================================================================

FUNCTION TDSRLCI_GET_DL_HFN

DESCRIPTION
   This function is called by RLC-UL to get latest HFN's for all established
   RLC-DL UM and AM radio-bearers.For cipher pending logical channels a value
   of zero is retuned. RLC populates the * to struct tdsrlc_hfn_req_type.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  If HFN's for all UM and AM rb's are populated.
  FALSE:  If no UM and AM rb's are present.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlci_get_dl_hfn                   
(
  tdsrlc_hfn_req_type* hfn_req             /*ptr to hfn request type          */
)
{
  uint16  count;                        /* Number of active Channels       */
  uint16  index = 0;
  uint8 array_len = 0;

  /*------------------------------------------------------------------------
    Get the number of active channels from the uplink table.
  ------------------------------------------------------------------------*/
  count = tdsdownlink_table.active_ch;
  hfn_req->dl_nchan = 0;
  array_len = sizeof(hfn_req->hfn_per_rb_dl)/sizeof(tdsrlc_hfn_type);
  TDSL2_ACQ_DL_LOCK(); 
 while((count > 0) && (index < TDSUE_MAX_DL_RLC_ENTITY) && (hfn_req->dl_nchan < array_len))
  {
    if (tdsdownlink_table.ctrl_blk[index].lc_state != TDSDL_NULL_STATE)
    {
      count--;
      if (tdsdownlink_table.ctrl_blk[index].lc_mode != UE_MODE_TRANSPARENT &&
          tdsdownlink_table.ctrl_blk[index].cipher.cipher_mode != TDSNO_CIPHER)
      {
        if (tdsdownlink_table.ctrl_blk[index].lc_mode == UE_MODE_ACKNOWLEDGED)
        {
          /*-----------------------------------------------------------------
            Remove the LS 12 bits as of SN.
          -------------------------------------------------------------------*/
          if (tdsdownlink_table.ctrl_blk[index].cipher.cipher_mode == TDSCIPHER_ON)
          {
            hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].hfn_valid = TRUE;
          }
          else
          {
            hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].hfn_valid = FALSE;
          }

          hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].hfn =
                      tdsdownlink_table.ctrl_blk[index].cipher.count >> 12;

          hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].rlc_sn =
            tdsdownlink_table.ctrl_blk[index].ctrl.am_ctrl.dl_state_var.vr_h;
          
          hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].rlc_id =
            tdsdownlink_table.ctrl_blk[index].ctrl.am_ctrl.am_common->dl_data_id;
        }
        else
        {
          /*----------------------------------------------------------------
            Remove the LS 7 bits as of SN.
          ------------------------------------------------------------------*/
          if (tdsdownlink_table.ctrl_blk[index].cipher.cipher_mode == TDSCIPHER_ON)
          {
            hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].hfn_valid = TRUE;
          }
          else
          {
            hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].hfn_valid = FALSE;
          }

          hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].hfn  =
                    tdsdownlink_table.ctrl_blk[index].cipher.count >> 7;

          hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].rlc_sn =
                          tdsdownlink_table.ctrl_blk[index].ctrl.um_ctrl.vr_ur;

          hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].rlc_id =
                          tdsdownlink_table.ctrl_blk[index].ctrl.um_ctrl.rlc_id;
        }

        TDSRLC_MSG3(MSG_LEGACY_HIGH,"RLC_DBG: RRC req DL Cipher LC %d, rlc_sn %d hfn 0x%x",
                          hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].rlc_id, 
                          hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].rlc_sn,
                          hfn_req->hfn_per_rb_dl[hfn_req->dl_nchan].hfn);

        hfn_req->dl_nchan ++;
      }
    }
    index++;
  }
  TDSL2_ACQ_DL_UNLOCK();
  return TRUE;

} /* tdsrlci_get_dl_hfn                   () */



#ifdef FEATURE_TDSCDMA_USE_RLC_BACKUP_TIMER
/*===========================================================================

FUNCTION  TDSRLC_DL_AM_BACKUP_TIMER

DESCRIPTION
  This function check the backup timer count. If it is greater than the
  hard-coded value (5). Start the 10ms timer for the AM channels.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_am_backup_timer            (void)
{
  uint16  i = 0;
  uint16  active_ch;

  active_ch = tdsdownlink_table.active_ch;
  //TDSRLC_MSG1(MSG_LEGACY_HIGH,"backup_timer %d",active_ch);
  while(active_ch > 0 && i < TDSUE_MAX_DL_RLC_ENTITY)
  {
    if (tdsdownlink_table.ctrl_blk[i].lc_state != TDSDL_NULL_STATE)
    {
      /* Ok, this is a AM channel.  */
      if (tdsdownlink_table.ctrl_blk[i].lc_mode == UE_MODE_ACKNOWLEDGED &&
          (tdsdownlink_table.ctrl_blk[i].lc_state == TDSDL_DATA_TRANSFER_READY ||
           tdsdownlink_table.ctrl_blk[i].lc_state == TDSDL_RESET_PENDING))
      {
        /*---------------------------------------------------------------------
          Do not resume the timer before it actually starts.
        ----------------------------------------------------------------------*/
        if(((l2_specail_test_value == TDSL2_SPECIAL_SETTINGS_FOR_CONFORMANCE)|| 
            (!tdsdownlink_table.ctrl_blk[i].ctrl.am_ctrl.dl_state_var.fst_rx_pdu)) &&
           ++tdsdownlink_table.ctrl_blk[i].ctrl.am_ctrl.
           dl_state_var.backup_timer_cnt > TDSRLC_BACKUP_TIMER_CNT)
        {
          tdsdownlink_table.ctrl_blk[i].ctrl.am_ctrl.dl_state_var.backup_timer_cnt = 0;
          tdsdownlink_table.ctrl_blk[i].ctrl.am_ctrl.dl_tmr_sts = TDSRLC_AM_TIMER_SUSPEND;
          TDSRLC_MSG1(MSG_LEGACY_LOW,"Set timer from backup,LC %d",tdsdownlink_table.ctrl_blk[i].ctrl.am_ctrl.am_common->dl_data_id);
        }
      }
      active_ch --;
    }
    i++;
  }
} /* tdsrlc_dl_am_backup_timer            () */

#endif //FEATURE_TDSCDMA_USE_RLC_BACKUP_TIMER


/*===========================================================================

FUNCTION  TDSRLCI_SUSPEND_DL_AM_TIMER

DESCRIPTION
  It suspends the DL AM 20ms timer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_suspend_dl_am_timer          
(
  byte lc_id
)
{
  TDSL2_ACQ_DL_LOCK();
  tdsdownlink_table.ctrl_blk[tdsdl_ctl_blk_id_tbl[lc_id]].ctrl.am_ctrl.dl_tmr_sts = TDSRLC_AM_TIMER_STOP;
  TDSL2_ACQ_DL_UNLOCK();
  TDSRLC_MSG2(MSG_LEGACY_HIGH,"RLC_TMR_DBG: Stopping timer for lc_id %d active_dl_chan %d",lc_id,tdsdownlink_table.active_ch);

} /* tdsrlci_suspend_dl_am_timer          () */


/*===========================================================================

FUNCTION  TDSRLCI_RESUME_DL_AM_TIMER

DESCRIPTION
  It resumes the DL AM 20ms timer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_resume_dl_am_timer           
(
 byte lc_id
)
{
  TDSL2_ACQ_DL_LOCK();
  tdsdownlink_table.ctrl_blk[tdsdl_ctl_blk_id_tbl[lc_id]].ctrl.am_ctrl.dl_tmr_sts = TDSRLC_AM_TIMER_RUNNING;
  TDSL2_ACQ_DL_UNLOCK();
  TDSRLC_MSG2(MSG_LEGACY_HIGH,"RLC_TMR_DBG: Resuming timer for lc_id %d active_dl_chan %d",lc_id,tdsdownlink_table.active_ch);
} /* tdsrlci_resume_dl_am_timer           () */


/*===========================================================================

FUNCTION TDSRLC_PROCESS_MAC_HS_RESET_REQ

DESCRIPTION
  This function handles the MAC HS Reset command from MAC. RLC sends
  a status report on receiving this command even if any status prohibit
  timer is on.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_process_mac_hs_reset_req      
(
  tdsmac_hs_dl_reset_req_type  *mac_hs_reset_req
)
{

  tdsrlci_dl_ctrl_blk_type *dl_ctl_blk = NULL;
  uint8 i =0;

  /*-------------------------------------------------------------------------
    Send a status report on all AM logical channels which are in TDSDL_DATA_TRANSFER_READY
    state, that have been indicated in the MAC HS reset command.
  -------------------------------------------------------------------------*/
  TDSL2_ACQ_DL_LOCK();

  for(i=0; i<mac_hs_reset_req->nchan; i++)
  {
    dl_ctl_blk = tdsrlci_dl_get_ctl_blk_ptr(mac_hs_reset_req->rlc_id_list[i]);
    /*lint -save -e655 */
    if((dl_ctl_blk == NULL) ||
       (dl_ctl_blk->lc_mode < UE_MODE_ACKNOWLEDGED) ||
       (dl_ctl_blk->lc_state != TDSDL_DATA_TRANSFER_READY))
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"No status report after HS reset,LC:%d",mac_hs_reset_req->rlc_id_list[i]);
      continue;
    }
    /*lint -restore */
    tdsrlci_rx_status_changed(dl_ctl_blk->ctrl.am_ctrl.am_common->ul_control_id,
                         FALSE);
    dl_ctl_blk->ctrl.am_ctrl.dl_status_opt_bitmap.next_sn_valid = FALSE;
    tdsrlci_set_ul_num_sn_li(dl_ctl_blk->ctrl.am_ctrl.am_common->ul_data_id,
            dl_ctl_blk->ctrl.am_ctrl.num_sn_li);
    tdsrlci_rx_status_changed(dl_ctl_blk->ctrl.am_ctrl.am_common->ul_control_id,
                           TRUE);

    /*---------------------------------------------------------------------
      If Status prohibit timer is active for the channel, re-start the
      timer.
    ---------------------------------------------------------------------*/
    if(dl_ctl_blk->ctrl.am_ctrl.dl_state_var.status_prohibit)
    {
      dl_ctl_blk->ctrl.am_ctrl.dl_state_var.status_prohibit_cnt = 0;
    }
  }
  
  TDSL2_ACQ_DL_UNLOCK();
}



/*===========================================================================

FUNCTION TDSCRLC_DL_ABORT_CIPHER_CONFIG

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
void tdscrlc_dl_abort_cipher_config       
(
  tdsrlc_abort_cipher_config_type *abort_cipher_info

)
{
  int i = 0;
  uint16 index = 0;                         /* Index to the control block  */
  tdsrlci_dl_ctrl_blk_type *ctl_blk = NULL;    /* Pointer to the control block*/

  TDSL2_ACQ_DL_LOCK();

  /*-------------------------------------------------------------------------
    For all the logical channels specified, revert back to the old ciphering
    configuration.
  -------------------------------------------------------------------------*/
  for(i=0;i<abort_cipher_info->nchan;i++)
  {
    /*-----------------------------------------------------------------------
      Get the index to the control block from the logical channel id
      and the control block.
    -----------------------------------------------------------------------*/
    index = tdsdl_ctl_blk_id_tbl[abort_cipher_info->lc_id[i]];
    ctl_blk = &tdsdownlink_table.ctrl_blk[index];

    TDSRLC_MSG2(MSG_LEGACY_HIGH,"Aborting DL Cipher Config for RbId: %d,LC: %d",ctl_blk->cipher.rb_id,abort_cipher_info->lc_id[i]);

    /*-----------------------------------------------------------------------
      Revert to the old ciphering configuration.
    -----------------------------------------------------------------------*/
    ctl_blk->cipher = ctl_blk->cipher_backup;
  }

  TDSL2_ACQ_DL_UNLOCK();

}

/*===========================================================================

FUNCTION TDSRLCI_GET_DL_LATEST_PENDING_HFN

DESCRIPTION
   This function is called by RRC to get latest pending HFN for indicated
   logical channel ID via RLC UL.
   RLC DL populates the info to the struct tdsrlc_pending_hfn_req_type.
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
void tdsrlci_get_dl_latest_pending_hfn    
(
  rlc_lc_id_type rlc_id,                /* queried RLC logical channel ID */
  tdsrlc_pending_hfn_req_type *pending_hfn
)
{
  tdsrlci_dl_ctrl_blk_type *dl_ctrl_blk = NULL;
  uint16 ctl_blk_id;

  if (rlc_id < TDSUE_MAX_DL_LOGICAL_CHANNEL &&
      (ctl_blk_id = (uint16)tdsdl_ctl_blk_id_tbl[rlc_id]) != TDSINIT_CTL_BLK_ID &&
      ctl_blk_id < TDSUE_MAX_DL_RLC_ENTITY)
  {
    dl_ctrl_blk = &tdsdownlink_table.ctrl_blk[ctl_blk_id];

    if (dl_ctrl_blk->cipher.cipher_mode == TDSCIPHER_WAIT ||
        dl_ctrl_blk->cipher.cipher_mode == TDSCIPHER_MOD)
    {
      pending_hfn->dl_pending_hfn = (dl_ctrl_blk->cipher.two_pending) ?
        dl_ctrl_blk->cipher.hfn_new1 : dl_ctrl_blk->cipher.hfn_new;
      pending_hfn->dl_pending_hfn_present = TRUE;
    }
  }
  else
  {
    TDSRLC_MSG1(MSG_LEGACY_HIGH,"Invalid RLC DL LC,%d",rlc_id);
  }
}

/*===========================================================================

FUNCTION TDSRLCI_DL_OVERWRITE_HFN

DESCRIPTION
   This function is called by RRC to overwrite the HFN of RLC currently using
   thru UL.

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
uecomdef_status_e_type tdsrlci_dl_overwrite_hfn             
(
  rlc_lc_id_type rlc_id,                /* queried RLC logical channel ID */
  uint32 tdshfn
)
{
  tdsrlci_dl_ctrl_blk_type *dl_ctrl_blk = NULL;
  uint16 ctl_blk_id;
  uecomdef_status_e_type dl_ret = FAILURE;

  TDSL2_ACQ_DL_LOCK();
  
  /* Perform DL AM overwrite */
  if (rlc_id < TDSUE_MAX_DL_LOGICAL_CHANNEL &&
      (ctl_blk_id = (uint16)tdsdl_ctl_blk_id_tbl[rlc_id]) != TDSINIT_CTL_BLK_ID &&
      ctl_blk_id < TDSUE_MAX_DL_RLC_ENTITY)
  {
    dl_ctrl_blk = &tdsdownlink_table.ctrl_blk[ctl_blk_id];

    if (dl_ctrl_blk->lc_mode == UE_MODE_ACKNOWLEDGED &&
        dl_ctrl_blk->cipher.cipher_mode == TDSCIPHER_ON)
    {
      dl_ctrl_blk->cipher.hfn = tdshfn;
      dl_ctrl_blk->cipher.count = tdshfn << 12;
      dl_ret = SUCCESS;
    }
  }

  TDSL2_ACQ_DL_UNLOCK();
  
  return (dl_ret);
}

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION TDSRLC_DL_LOG_CIPHER_PKT

DESCRIPTION
This function logs the dowlink PDU ciphering parameters.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_dl_log_cipher_pkt             
(
  void
)
{
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  tdsrlc_dl_pdu_cipher_info_type* tdsrlci_dl_pdu_cipher_log_backup_ptr;
  if(tdsrlci_dl_pdu_cipher_log_ptr->num_pdu)
  {
    tdsrlci_dl_pdu_cipher_log_backup_ptr= tdsrlci_dl_pdu_cipher_log_ptr;
    TDSL2_MUTEX_LOCK(&tdsrlci_dl_pdu_cipher_log_mutex);
    if(tdsrlci_dl_pdu_cipher_log_ptr==&tdsrlci_dl_pdu_cipher_log[0])
    {
      tdsrlci_dl_pdu_cipher_log_ptr=&tdsrlci_dl_pdu_cipher_log[1];
    }
    else if(tdsrlci_dl_pdu_cipher_log_ptr==&tdsrlci_dl_pdu_cipher_log[1])
    {
      tdsrlci_dl_pdu_cipher_log_ptr=&tdsrlci_dl_pdu_cipher_log[0];
    }
    else
    {
      ERR_FATAL("cipher_log_ptr %d is invalid (%d, %d)",tdsrlci_dl_pdu_cipher_log_ptr,
                &tdsrlci_dl_pdu_cipher_log[0],&tdsrlci_dl_pdu_cipher_log[1]);
    }
    TDSL2_MUTEX_UNLOCK(&tdsrlci_dl_pdu_cipher_log_mutex);
    /*-----------------------------------------------------------------------
      The logging data is filled in at the time before a PDU is decoded.
    -----------------------------------------------------------------------*/
    tdsrlc_dl_tx_pdu_cipher_log_pkt(tdsrlci_dl_pdu_cipher_log_backup_ptr);

    /*-----------------------------------------------------------------------
      Reset tdsnum_pdu after sending logging data to DIAG.
    -----------------------------------------------------------------------*/
    tdsrlci_dl_pdu_cipher_log_backup_ptr->num_pdu = 0;
  }

#else
  if(tdsrlci_dl_pdu_cipher_log.num_pdu)
  {
    /*-----------------------------------------------------------------------
      Lock the cipher pdu log info updation by setting the flag.
    -----------------------------------------------------------------------*/
    tdsrlci_dl_pdu_cipher_log.cipher_pdu_logging = TRUE;

    /*-----------------------------------------------------------------------
      The logging data is filled in at the time before a PDU is decoded.
    -----------------------------------------------------------------------*/
    tdsrlc_dl_tx_pdu_cipher_log_pkt(&tdsrlci_dl_pdu_cipher_log);

    /*-----------------------------------------------------------------------
      Reset tdsnum_pdu after sending logging data to DIAG.
    -----------------------------------------------------------------------*/
    tdsrlci_dl_pdu_cipher_log.num_pdu = 0;

    /*-----------------------------------------------------------------------
      Unlock cipher pdu log info updation.
    -----------------------------------------------------------------------*/
    tdsrlci_dl_pdu_cipher_log.cipher_pdu_logging = FALSE;
  }
#endif
} /* rlc_dl_process_log_cipher_pkt() */

/*===========================================================================

FUNCTION TDSRLC_DL_SET_CIPHER_LOGGING

DESCRIPTION
This function sets signal to log downlink cipher PDU packets in task context.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void
tdsrlc_dl_set_cipher_logging         
(
  void
)
{
#ifdef  FEATURE_TDSCDMA_FREE_FLOATING_TASK
  if((tdsrlci_dl_pdu_cipher_log_ptr != NULL) 
     && (tdsrlci_dl_pdu_cipher_log_ptr->num_pdu))
#else
  if(tdsrlci_dl_pdu_cipher_log.num_pdu)
#endif
  {
    /*-----------------------------------------------------------------------
      Set the TDSRLC_DL_LOG_CIPHER_PKT_SIG rex signal, if ciher PDUs 
      needs to be logged
    -----------------------------------------------------------------------*/
    (void)rex_set_sigs(tds_l2_dl_get_tcb(), TDSRLC_DL_LOG_CIPHER_PKT_SIG);
  }
} /* tdsrlc_dl_set_cipher_logging         () */

#endif /* FEATURE_TDSCDMA_RLC_QXDM_LOGGING */


/*===========================================================================

FUNCTION TDSRLCI_DL_CHANGE_STATE

DESCRIPTION
   This function will change the downlink RLC from TDSDL_RESET_PENDING state to
   its previous state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_dl_change_state              
(
  rlc_lc_id_type lc_id
)
{
  /*lint -e64 -e641 */
  (tdsdownlink_table.ctrl_blk[tdsdl_ctl_blk_id_tbl[lc_id]].lc_state) &= (~TDSDL_RESET_PENDING);
  /*lint +e64 +e641 */
}

/*===========================================================================

FUNCTION TDSCRLC_DL_WM_CLEANUP_CMD_HANDLER

DESCRIPTION
   this function cleanup the PDCP-RLC WM by a call back function.

DEPENDENCIES
  None.

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_wm_clean_up_cmd_handler    (tdsrlci_cleanup_wm_cmd_type *cleanup_wm_cmd)
{
  uint16 ctl_blk_id;

  if (cleanup_wm_cmd->lc_id < TDSUE_MAX_DL_LOGICAL_CHANNEL)
  {
    if (((ctl_blk_id = (uint16)tdsdl_ctl_blk_id_tbl[cleanup_wm_cmd->lc_id]) != TDSINIT_CTL_BLK_ID) &&
         (ctl_blk_id < TDSUE_MAX_DL_RLC_ENTITY))
    {
      if(tdsdownlink_table.ctrl_blk[ctl_blk_id].dl_wm_ptr == NULL)
      {
        cleanup_wm_cmd->cleanup_wm_cb(cleanup_wm_cmd->cleanup_wm_para);
      }
    }
    else
    {
      cleanup_wm_cmd->cleanup_wm_cb(cleanup_wm_cmd->cleanup_wm_para);
    }
  }
}
/*===========================================================================

FUNCTION TDSCRLC_DL_PERFORM_STOP_FUNCTIONALITY

DESCRIPTION

   Stops the DL reception on one or more logical channels.

DEPENDENCIES
  None.

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_perform_stop_functionality
(
  tdsrlc_dl_stop_request_type *rrcsmc_dl_stop_req
)
{

  uint8   index, lc_id, i;

  TDSL2_ACQ_DL_LOCK();
  
  for (i = 0; i < rrcsmc_dl_stop_req->nchan; i++)
  {
    /*-----------------------------------------------------------------------
      Get the index of the downlink table.
    -----------------------------------------------------------------------*/
    lc_id = rrcsmc_dl_stop_req->lc_id[i];
    index = tdsdl_ctl_blk_id_tbl[lc_id];

    if ((index != TDSINIT_CTL_BLK_ID) && (index < TDSUE_MAX_DL_RLC_ENTITY))
    {
      /*lint -e64 -e641 -e655 */
      tdsdownlink_table.ctrl_blk[index].lc_state |= TDSDL_STOP;
      tdsdownlink_table.ctrl_blk[index].lc_state &= (~TDSDL_DATA_TRANSFER_READY);
      /*lint +e64 +e641 +e655 */
      
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"DL  LC %d - Stop",lc_id);
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL LC %d not config'd,No Stop",lc_id);
    }
  }
  
  TDSL2_ACQ_DL_UNLOCK();
  
} /* tdscrlc_dl_perform_stop_functionality */

/*===========================================================================

FUNCTION TDSCRLC_DL_CONTINUE_HANDLER

DESCRIPTION
  Continue transmission on one or more downlink unacknowledged or acknowledged
  mode channels.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdscrlc_dl_continue_handler          
(
  tdsrlc_dl_continue_type *continue_ptr      /* Ptr to continue Info                */
)
{
  uint8 index, lc_id ,i;

  TDSL2_ACQ_DL_LOCK();
  
  for (i = 0; i < continue_ptr->nchan; i++)
  {
    lc_id = continue_ptr->channel[i];
    index = tdsdl_ctl_blk_id_tbl[lc_id];

    /*lint -save -e64 -e641 -e655 */
    if (index != TDSINIT_CTL_BLK_ID &&
        (index < TDSUE_MAX_DL_RLC_ENTITY) &&
        (tdsdownlink_table.ctrl_blk[index].lc_state & TDSDL_STOP))
    /*lint -restore */
    {
      /*lint -save -e64 -e641 -e655 */
      tdsdownlink_table.ctrl_blk[index].lc_state &= (~TDSDL_STOP);
      tdsdownlink_table.ctrl_blk[index].lc_state |= TDSDL_DATA_TRANSFER_READY;
      /*lint -restore */
      
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"DL LC %d Continue",lc_id);
    }
    else
    {
      TDSRLC_MSG1(MSG_LEGACY_HIGH,"DL LC %d not in Stop State",lc_id);
    }
  }
  
  TDSL2_ACQ_DL_UNLOCK();
  
}/* tdscrlc_dl_continue_handler          () */


#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION tdsrlc_enh_logging_data_handler      

DESCRIPTION
    - Logging functionality called for all the PDUs which belongs to given LC Id     
      If first_pdu_on_this_lch is TRUE, initialise the tdsnum_pdu to zero

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_logging_data_handler      (tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr, 
                                  tds_complete_pdu_type *complete_pdu_ptr,
                                  boolean first_pdu_on_this_lch)
{
  uint8 temp_data[4];
  uint16 pdu_size_in_bits;
  uint32 temp_word;
  tdsrlc_dl_chan_am_pdu_type  *dl_chan_am_pdu_ptr;
  uint16 dcbit;
#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
  tdsrlc_dl_chan_um_pdu_type  *dl_chan_um_pdu_ptr;
#endif
  
  if (ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED)
  {
    dl_chan_am_pdu_ptr = &(ctrl_blk_ptr->ctrl.am_ctrl.dl_chan_am_pdu);
    
    if (first_pdu_on_this_lch == TRUE)
    {
      dl_chan_am_pdu_ptr->num_pdu = 0;
    }
    
    if (dl_chan_am_pdu_ptr->num_pdu >= TDSMAX_DL_RLC_LOG_PDUS)
    {
      return;
    }
	 if (tdsrlc_debug_var & TDSRLC_DBG_DL_PDU_DUMP)
    {
      uint32 nwords =  (complete_pdu_ptr->iovec.length /4) + 1;
      uint32 idx;

      for (idx = 0; idx < nwords; idx++)
      {
        TDSRLC_MSG1(MSG_LEGACY_ERROR,"DL PDU: 0x%x",tdsrlc_copy_to_word(complete_pdu_ptr->iovec.ptr->data_ptr + complete_pdu_ptr->iovec.offset + 4 * idx));
      } 
    }
    /*-----------------------------------------------------------
      Store the number of pdus, pdu size, 4bytes of the pdu,
      data id, control id in dl_am_pdu_log for purpose of qcat
      logging.
    -----------------------------------------------------------*/
    dcbit = (uint16)TDSRLCI_GET_DC_BIT(complete_pdu_ptr->rlc_hdr_data);
    if(dcbit == 0)
    {
      pdu_size_in_bits = (((uint16)complete_pdu_ptr->iovec.length) << 3);
      (void)dsm_extract(complete_pdu_ptr->iovec.ptr, 
                      complete_pdu_ptr->iovec.offset, temp_data, 4);
      temp_word = tdsrlc_copy_to_word(temp_data);

      if(tdsrlc_debug_var & TDSRLC_DBG_DL_CTRL)
      {
        TDSRLC_MSG3(MSG_LEGACY_ERROR,"RLC_ERR: NA: rlc_id %d, rlc_hdr_data 0x%x, temp_word 0x%x",
                 complete_pdu_ptr->rlc_id,
                 complete_pdu_ptr->rlc_hdr_data, 
                 temp_word);
      }
    }
    else
    {
      pdu_size_in_bits = (((uint16) (complete_pdu_ptr->iovec.length + 
                                     TDSRLC_AM_PDU_HDR_SIZE_BYTES)) << 3);
      (void)dsm_extract(complete_pdu_ptr->iovec.ptr, 
                      complete_pdu_ptr->iovec.offset, temp_data, 2);
      
      temp_word = (uint32)(((complete_pdu_ptr->rlc_hdr_data & 0xFF) << 8) | 
                           ((complete_pdu_ptr->rlc_hdr_data & 0xFF00) >> 8) |
                           ((uint32)temp_data[0] << 16) | 
                           ((uint32)temp_data[1] << 24));
    }
    
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
    dl_chan_am_pdu_ptr->pdu_size[dl_chan_am_pdu_ptr->num_pdu] = pdu_size_in_bits;
#else
    dl_chan_am_pdu_ptr->pdu_size = pdu_size_in_bits;
#endif
    dl_chan_am_pdu_ptr->log_data[dl_chan_am_pdu_ptr->num_pdu] = temp_word;
    dl_chan_am_pdu_ptr->num_pdu++;
  }
  else
  {
#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
    if (ctrl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED)
    {
      dl_chan_um_pdu_ptr = &(ctrl_blk_ptr->ctrl.um_ctrl.dl_chan_um_pdu);
      
      if (first_pdu_on_this_lch == TRUE)
      {
        dl_chan_um_pdu_ptr->num_pdu = 0;
      }
      
      if (dl_chan_um_pdu_ptr->num_pdu >= TDSMAX_DL_RLC_LOG_PDUS)
      {
        return;
      }      
      dl_chan_um_pdu_ptr->pdu_size = (((uint16) (complete_pdu_ptr->iovec.length + 
                                               TDSRLC_UM_PDU_HDR_SIZE_BYTES)) << 3);
      if ((dsm_extract(complete_pdu_ptr->iovec.ptr, 
                       complete_pdu_ptr->iovec.offset, temp_data, 3)) > 0
         )
      {
        temp_word = (uint32) (((complete_pdu_ptr->rlc_hdr_data >> 8)& 0xFF)| 
                              ((uint32)temp_data[0] << 8) | 
                              ((uint32)temp_data[1] << 16) | 
                              ((uint32)temp_data[2] << 24));
        dl_chan_um_pdu_ptr->log_data[dl_chan_um_pdu_ptr->num_pdu] = temp_word;
        dl_chan_um_pdu_ptr->num_pdu++;
      }
    }
#endif
  }
}

/*===========================================================================

FUNCTION tdsrlc_enh_logging_post_data_handler 

DESCRIPTION
    - Logging functionality called for all the PDUs which belongs to given LC Id     

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_logging_post_data_handler (tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr, 
                                       uint8 lc_id, boolean hs_path)
{
  if ((ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED) && 
      (ctrl_blk_ptr->ctrl.am_ctrl.dl_chan_am_pdu.num_pdu > 0))
  {
    /* Log 0x413B - Non EHS Calls, EHS Call SRB */
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
    if ((hs_path == FALSE) || 
        (tdsmac_ehs_query_hs_type() != MAC_EHS) || 
        (lc_id < TDSRLC_DL_AM_USR_PLANE_START_LC_ID))
#endif
    {
      tdsrlc_dl_enh_tx_am_pdu_log_pkt(ctrl_blk_ptr, lc_id);
    }
  }
  else
  {
#ifdef FEATURE_TDSCDMA_UM_PDU_LOGGING
    if ((ctrl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED) &&
        (ctrl_blk_ptr->ctrl.um_ctrl.dl_chan_um_pdu.num_pdu > 0))
    {
      tdsrlc_dl_enh_tx_um_pdu_log_pkt(ctrl_blk_ptr);
    }
#endif
  }
}

#endif /*FEATURE_TDSCDMA_RLC_QXDM_LOGGING */

/*===========================================================================

FUNCTION tdsrlci_discard_sdu_ptr              

DESCRIPTION
    For every DSM Buffer/item in the packet chain, discard the buffer/item as 
    per conditions below
    If the DSM Buffer/item app_field is zero, 
       - DISCARD the DSM Buffer/item by calling dsmi_free_buffer
    Otherwise
       - Nothing

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_discard_sdu_ptr              
(
 dsm_item_type **dsm_ptr,
 const char * file,
 uint32 line
)
{
  dsm_item_type *ptr = *dsm_ptr;
  
  while (ptr != NULL)
  {
    if (ptr->app_field == 0)
    {
      if (tdsrlc_debug_var & TDSRLC_DBG_REASSEMBLY_1)
      {
        MSG_9(MSG_SSID_TDSCDMA_RLC, MSG_LEGACY_HIGH,
              "RLC DBG DL: rlc_sdu_discard: dsm_ptr 0x%x, pkt_ptr 0x%x, tdsdata_ptr 0x%x, ref_cnt %d, app_field 0x%x, dup_ptr 0x%x, used 0x%x size 0x%x",
               ptr, ptr->pkt_ptr, ptr->data_ptr, ptr->references, ptr->app_field, ptr->dup_ptr, ptr->used, ptr->size, 0);
      }
      /* dsmi_free_buffer returns the next item in the chain */
      ptr = dsmi_free_buffer(ptr, file, line);
    }
    else
    {
      ptr = ptr->pkt_ptr;
    }
  }
}

/*===========================================================================

FUNCTION tdsrlc_discard_dsm_item

DESCRIPTION
    Discard the DSM Item as follows
    If the app_field is greater than zero, decrement it.
    If the resulting app_field is zero, 
       - DISCARD the DSM item by calling dsm_free_packet
    Otherwise
       - Nothing

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlci_discard_dsm_item             
(
 dsm_item_type **dsm_ptr,
 const char * file,
 uint32 line
)
{
  dsm_item_type *ptr;
  ptr = *dsm_ptr;

  TDSL2_MUTEX_LOCK(&tdsrlc_mac_dl_pdu_mutex);

  if (ptr != NULL)
  {
    if (ptr->app_field > 0)
    {
      ptr->app_field--; 
      if (ptr->app_field == 0)
      {
        if(tdsrlc_debug_var & TDSRLC_DBG_MEM_LEAK)
        {
          dsmi_touch_item(*dsm_ptr, tdsrlc_exit, __LINE__);
        }
        if (tdsrlc_debug_var & TDSRLC_DBG_REASSEMBLY_1)
        {
          MSG_9(MSG_SSID_TDSCDMA_RLC, MSG_LEGACY_HIGH,
                "RLC DBG DL: rlc_discard: dsm_ptr 0x%x, pkt_ptr 0x%x, tdsdata_ptr 0x%x, ref_cnt %d, app_field 0x%x, dup_ptr 0x%x, used 0x%x size 0x%x",
                 ptr, ptr->pkt_ptr, ptr->data_ptr, ptr->references, ptr->app_field, ptr->dup_ptr, ptr->used, ptr->size, 0);
        }
        dsmi_free_packet(dsm_ptr, file, line);
      }
    }
    else
    {
      ERR_FATAL("RLC_ERR: tdsrlc_discard_dsm_item: app_field is already zero",0,0,0);
    }
  }
  else
  {
    ERR_FATAL("RLC_ERR: tdsrlc_discard_dsm_item: dsm_ptr is NULL ptr",0,0,0);
  }

  TDSL2_MUTEX_UNLOCK(&tdsrlc_mac_dl_pdu_mutex);
}
/*===========================================================================

FUNCTION tdsrlc_discard_pdu

DESCRIPTION
    Discard the PDU if not NULL

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_discard_pdu
(
  tds_complete_pdu_type       *complete_pdu_ptr
)
{

  if (NULL != complete_pdu_ptr->iovec.ptr)
  {
    tdsrlc_discard_dsm_item(&(complete_pdu_ptr->iovec.ptr));
    complete_pdu_ptr->iovec.ptr = NULL;
  }
  else
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: NULL iovec ptr for LC %d",complete_pdu_ptr->rlc_id);
  }
  
}

/*===========================================================================

FUNCTION tdsrlc_enh_calc_rlc_hdr_length       

DESCRIPTION
    - Return the RLC header length, which is 
        2 for AM Data PDU, 0 for AM Ctrl PDU, 1 for UM and 0 for TM     

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

uint8 tdsrlc_enh_calc_rlc_hdr_length       (rlc_lc_id_type rlc_id, uint8 first_byte, boolean *is_tm_mode)
{
  uint8 length = 0;
  tdsrlci_dl_ctrl_blk_type           *ctrl_blk_ptr;

  ctrl_blk_ptr = TDSRLCI_GET_CTRL_BLK_PTR(TDSRLCI_GET_CTRL_INDEX(rlc_id));
  if (ctrl_blk_ptr != NULL)
  {
    if (ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED)
    {
      if (first_byte & 0x80)
      {
        length = 2;
      }
    }
    else 
    {
      if (ctrl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED) 
      {
        length = 1;
      }
      else
      {
        *is_tm_mode = TRUE;
      }
    }
  }
  else
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: No Control Blk exists for this rlc_id %d",rlc_id);
  }
  
  return (length);
}
/*===========================================================================

FUNCTION tdsrlc_update_nak_status             

DESCRIPTION
    - This function sets/resets the nak status for a range of pdus with sn 
      starting with (fsn) and ending with sn (lsn) (fsn and lsn inclusive).   

DEPENDENCIES
  None.

INPUT ARGUMENTS
  nak_bitmap - pointer to bitmap array
  fsn        - fist sequence number for which status is to be updated.
  lsn        - last sequence number for which status is to be updated.
  update     - pdu status 
               FALSE - ACK
               TRUE  - NAK
  mod        - modulus to use..

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_update_nak_status             
(
  uint16                *nak_bitmap,
  uint16                fsn,
  uint16                lsn,
  boolean               update,
  uint16                mod
)
{
  uint8             cnt = 0;
  uint16            last_sn = 0;
/*------------------------------------------------------------------------*/

  if ( !TDSRLCI_SEQ_GE( lsn, fsn, TDSRLC_AM_MODULUS ) )
  {
    return;
  }

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
      fsn corresponds to (last_lsn), update the status for pdus fron fsn to 
      last_lsn, update fsn to point to next sn after last_lsn.
    ----------------------------------------------------------------------*/
    if ( TDSRLCI_SEQ_GT( lsn, last_sn, TDSRLC_AM_MODULUS ) )
    {
      if ( TRUE == update )
      {
        /*------------------------------------------------------------------
          Mark PDUs from fsn to last_sn as naked.
        ------------------------------------------------------------------*/
  /*lint -e502 */
        TDSRLCI_SET_AM_SN_RANGE_NAK_STATUS( nak_bitmap, fsn, last_sn, mod );
  /*lint +e502 */
        fsn = TDSRLCI_AM_SEQ_INC( last_sn );
      }
      else
      {
        /*------------------------------------------------------------------
          Mark PDUs from fsn to last_sn as received.
        ------------------------------------------------------------------*/
        TDSRLCI_RESET_AM_SN_RANGE_NAK_STATUS( nak_bitmap, fsn, last_sn, mod );
        fsn = TDSRLCI_AM_SEQ_INC( last_sn );
      }
    }
    /*----------------------------------------------------------------------
      If last_sn is less than the last sn  which the 16-bit value having
      fsn corresponds to (last_lsn), update the status for pdus from fsn to 
      lsn, since both last_sn and fsn fall in the same 16-bit value.
    ----------------------------------------------------------------------*/
    else
    {
      last_sn = lsn;
      if ( TRUE == update )
      {
        /*------------------------------------------------------------------
          Mark PDUs from fsn to last_sn as naked.
        ------------------------------------------------------------------*/
  /*lint -e502 */
        TDSRLCI_SET_AM_SN_RANGE_NAK_STATUS( nak_bitmap, fsn, lsn, mod );
  /*lint +e502 */
      }
      else
      {
        /*------------------------------------------------------------------
          Mark PDUs from fsn to last_sn as received.
        ------------------------------------------------------------------*/
        TDSRLCI_RESET_AM_SN_RANGE_NAK_STATUS( nak_bitmap, fsn, lsn, mod );
      }
    }

  }while ( last_sn != lsn );

  return;
}
/*===========================================================================

FUNCTION tdsrlc_get_nak_bitmap                

DESCRIPTION
    - This function gets the nak bitmap structure maintained on DL.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  lc_id      - logical channel id.

RETURN VALUE
  nak bitmap - pointer to nak bitmap structure.

SIDE EFFECTS
  None.

===========================================================================*/
tdsrlc_opt_status_type  *tdsrlc_get_nak_bitmap                
(
  byte               lc_id  
)
{
  tdsrlci_dl_ctrl_blk_type      *ctl_blk_ptr = NULL;
  tdsrlc_opt_status_type        *bitmap = NULL;
/*------------------------------------------------------------------------*/

  ctl_blk_ptr = tdsget_am_ctl_blk(lc_id);
  bitmap = &ctl_blk_ptr->ctrl.am_ctrl.dl_status_opt_bitmap;
  
  return ( bitmap ); 
}
/*===========================================================================

FUNCTION tdsrlc_get_nak_status                

DESCRIPTION
    - This function gets the nak status indicating whether we have any naks
      pending to be transmited or not.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  nak_bitmap - pointer to bitmap array
  fsn        - fist sequence number for which status is needed.
  lsn        - last sequence number for which status is needed.
  mod        - modulus to use..

RETURN VALUE
  nak status - TRUE - nak is present.
               FALSE - no naks are there.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrlc_get_nak_status                
(
  byte                  lc_id,
  uint16                fsn,
  uint16                lsn,
  uint16                mod 
)
{
  uint8               cnt = 0;
  tdsrlci_dl_ctrl_blk_type *ctl_blk_ptr;
  uint16 last_sn;
  uint16 *nak_bitmap = NULL;
/*------------------------------------------------------------------------*/

  ctl_blk_ptr = tdsget_am_ctl_blk(lc_id);

  if ( ctl_blk_ptr == NULL )
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR," Bad i/p arg");
    return FALSE;
  }

  if ( !TDSRLCI_SEQ_GE( lsn, fsn, TDSRLC_AM_MODULUS ) )
  {
    return FALSE;
  }

  nak_bitmap = ctl_blk_ptr->ctrl.am_ctrl.dl_status_opt_bitmap.status_bitmap;

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
      fsn corresponds to (last_lsn), update the status for pdus fron fsn to 
      last_lsn, update fsn to point to next sn after last_lsn.
    ----------------------------------------------------------------------*/
    if ( !TDSRLCI_SEQ_GT( lsn, last_sn, TDSRLC_AM_MODULUS ) )
    {
      last_sn = lsn;
    }
    /*----------------------------------------------------------------------
      If we have any bit set between fsn and lsn return TRUE..
    ----------------------------------------------------------------------*/
  /*lint -e502 */
    if( TDSRLCI_GET_AM_SN_RANGE_NAK_STATUS( nak_bitmap, fsn, last_sn, mod ) )
  /*lint +e502 */
    {
      return TRUE;
    }

    fsn = TDSRLCI_AM_SEQ_INC( last_sn );

  }while (last_sn != lsn);

  return (FALSE);

}

/*===========================================================================

FUNCTION tdsrlc_get_num_nak                   

DESCRIPTION
    - This function returns the number number of sn_li pairs in the 
      DL nak status array.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  lc_id      - logical channel id.


RETURN VALUE
  num_sn_li - number of sn_li pairs in the DL nak status array.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_get_num_nak                   
(
  byte                  lc_id  
)
{
  tdsrlci_dl_ctrl_blk_type *ctl_blk_ptr;
/*------------------------------------------------------------------------*/

  ctl_blk_ptr = tdsget_am_ctl_blk(lc_id);

  return (ctl_blk_ptr->ctrl.am_ctrl.num_sn_li);
}

/*===========================================================================

FUNCTION tdsrlc_get_num_nak_sn_li

DESCRIPTION
    - This function calculates and returns the number number of sn_li pairs 
      in the nak status array..

DEPENDENCIES
  None.

INPUT ARGUMENTS
  lc_id      - logical channel id.
  fsn        - fist sequence number from which sn_li is counted.
  lsn        - last sequence number till which sn_li is counted.
  mod        - modulus to use..


RETURN VALUE
  num_sn_li - number number of sn_li pairs in the nak status array.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 tdsrlc_get_num_nak_sn_li
(
  byte                  lc_id,
  uint16                fsn,
  uint16                lsn,
  uint16                mod 
)
{
  tdsrlci_dl_ctrl_blk_type *ctl_blk_ptr;
  uint16 temp_sn, num_sn_li = 0, sn_li_cnt = 0;
  uint16 *nak_bitmap = NULL;
/*------------------------------------------------------------------------*/

  ctl_blk_ptr = tdsget_am_ctl_blk(lc_id);

  if ( ctl_blk_ptr == NULL )
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR," Bad i/p arg");
    return 0;
  }

  if ( !TDSRLCI_SEQ_GE( lsn, fsn, TDSRLC_AM_MODULUS ) )
  {
    return 0;
  }

  if ( !TDSRLCI_SEQ_DIFF( lsn, fsn, TDSRLC_AM_MODULUS ) )
  {
    return 0;
  }

  nak_bitmap = ctl_blk_ptr->ctrl.am_ctrl.dl_status_opt_bitmap.status_bitmap;

  for (temp_sn = fsn; TDSRLCI_SEQ_GT( lsn, temp_sn, TDSRLC_AM_MODULUS); 
         (temp_sn = TDSRLCI_AM_SEQ_INC(temp_sn)))
  {
    if (TDSRLCI_GET_AM_STATUS_OPT_BITMAP(nak_bitmap, temp_sn, mod))
    {
      sn_li_cnt++;
      if (sn_li_cnt > 15)
      {
        sn_li_cnt = 0;
        num_sn_li++;
      }
    }
    else
    {
      sn_li_cnt = 0;
      num_sn_li++;
    }
  }

  if (sn_li_cnt > 0)
  {
    num_sn_li++;
  }
  return (num_sn_li);
}
/*===========================================================================

FUNCTION tdsrlc_set_num_nak_sn_li             

DESCRIPTION
    - This function sets the value of number of sn_li pairs, num_sn_li.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  lc_id      - logical channel id.
  num_sn_li  - value for num_sn_li

RETURN VALUE
  NONE

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_set_num_nak_sn_li             
(
  byte                  lc_id,
  uint16                num_sn_li
)
{
  tdsrlci_dl_ctrl_blk_type *ctl_blk_ptr;
/*------------------------------------------------------------------------*/

  ctl_blk_ptr = tdsget_am_ctl_blk(lc_id);

  if ( ctl_blk_ptr == NULL )
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR," Bad i/p arg");
    return;
  }
  ctl_blk_ptr->ctrl.am_ctrl.num_sn_li = num_sn_li;
  return;
}
/*===========================================================================

FUNCTION tdsrlci_enh_update_nak_list          

DESCRIPTION
    - As part of Post-Processing, Status message will be prepared as below
      - Prepare the LIST SUFI in which, 
        SN is the sequence number in which first hole is existing and 
        L is the number of consecutive holes present      

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_enh_update_nak_list          (tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr)
{
  uint16                        vr_h;
  tdsrlci_dl_am_ctrl_type         *am_ctrl_ptr;
  tdsrlc_dl_am_reseq_q_type       *dl_reseq_q_ptr;
  tdsrlc_opt_status_type           dl_status_opt_bitmap;
  tdsrlci_dl_am_state_type         *tdsdl_state_ptr = NULL;
  uint16 vr_r, max_rx_sn;
  uint8 distance = 0; /*li value in sl_li pair for list sufi*/
  uint16 num_sn_li = 0; /*number of sn_li pairs for list sufi*/

  rex_crit_sect_type *datapath_mutex = NULL;

  TDSL2_ACQ_DL_LOCK();
  am_ctrl_ptr = &(ctrl_blk_ptr->ctrl.am_ctrl);
  dl_reseq_q_ptr = am_ctrl_ptr->dl_reseq_q;
  
  vr_h = am_ctrl_ptr->dl_state_var.vr_h;
  

  memset(&dl_status_opt_bitmap,0,sizeof(tdsrlc_opt_status_type));
  /*---------------------------------------------------------------------
    Backup DL status Array.
  ---------------------------------------------------------------------*/
  datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(am_ctrl_ptr->am_common->ul_control_id);

  /*(void)memcpy(&dl_status_opt_bitmap,
     &ctrl_blk_ptr->ctrl.am_ctrl.dl_status_opt_bitmap, sizeof(tdsrlc_opt_status_type));*/
   memscpy(&dl_status_opt_bitmap, sizeof(tdsrlc_opt_status_type),
     &ctrl_blk_ptr->ctrl.am_ctrl.dl_status_opt_bitmap, sizeof(tdsrlc_opt_status_type));

  TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  tdsdl_state_ptr = &am_ctrl_ptr->dl_state_var;

  /*-------------------------------------------------------------------------
    Initialize num_sn_li to 0, we will be recalculating number of sn_li pairs 
    here.
  -------------------------------------------------------------------------*/
  num_sn_li = 0;

  /*------------------------------------------------------------------------
    Set NAK bits for SN less than vr_r.
  ------------------------------------------------------------------------*/
  if (TDSRLCI_SEQ_GT(tdsdl_state_ptr->vr_r, tdsdl_state_ptr->old_vr_r, TDSRLC_AM_MODULUS))
  {
    for (vr_r = tdsdl_state_ptr->old_vr_r; 
          TDSRLCI_SEQ_GT(tdsdl_state_ptr->vr_r, vr_r, TDSRLC_AM_MODULUS);
          (vr_r = TDSRLCI_AM_SEQ_INC(vr_r)))
    {
      TDSRLCI_SET_AM_SN_STATUS_OPT_BITMAP( 
               dl_status_opt_bitmap.status_bitmap, vr_r, 
               TDSRLC_AM_HALF_MOD);

      if (tdsrlc_debug_var & TDSRLC_DBG_DL_AM)
      {
        TDSRLC_MSG2(MSG_LEGACY_HIGH, "BITMAP_TST:num_sn_li %d,rcvd_sn %d",num_sn_li,vr_r);
      }
    }
  }

  /*---------------------------------------------------------------------
    if vr_h == vr_r, we dont have any further holes, copy back updated
    DL status array and num_sn_li, relase mutex.
  ---------------------------------------------------------------------*/
  if (vr_h == tdsdl_state_ptr->vr_r)
  {
    datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(am_ctrl_ptr->am_common->ul_control_id);

    /*(void)memcpy(&ctrl_blk_ptr->ctrl.am_ctrl.dl_status_opt_bitmap, 
              &dl_status_opt_bitmap,sizeof(tdsrlc_opt_status_type));*/
    memscpy(&ctrl_blk_ptr->ctrl.am_ctrl.dl_status_opt_bitmap, sizeof(tdsrlc_opt_status_type), 
              &dl_status_opt_bitmap,sizeof(tdsrlc_opt_status_type));

    ctrl_blk_ptr->ctrl.am_ctrl.num_sn_li = num_sn_li;
    am_ctrl_ptr->dl_state_var.dl_vr_h_status = vr_h;

    TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    TDSL2_ACQ_DL_UNLOCK();
    return;
  }

  /*---------------------------------------------------------------------
    Get MAX RX SN, vr_h or vr_mr
  ---------------------------------------------------------------------*/
  max_rx_sn = TDSRLCI_SEQ_GT(tdsdl_state_ptr->vr_h, 
                          tdsdl_state_ptr->vr_mr, TDSRLC_AM_MODULUS) ? 
                          tdsdl_state_ptr->vr_mr : tdsdl_state_ptr->vr_h;

  /*---------------------------------------------------------------------
    Update DL status array, resetting NAK bits for PDUs received and 
    re-calculating sn_li pairs.
  ---------------------------------------------------------------------*/
  for (vr_r = am_ctrl_ptr->dl_state_var.vr_r; 
       (TDSRLCI_SEQ_GT(max_rx_sn, vr_r, TDSRLC_AM_MODULUS) == TRUE);
       (vr_r = TDSRLCI_AM_SEQ_INC(vr_r)))
  {
    if (dl_reseq_q_ptr->pdu_info[TDSRLCI_GET_RESEQ_Q_IDX(vr_r)].dsm_ptr != NULL)
    {
      /*---------------------------------------------------------------------
        PDU is received RESET corresponding NAK bit...
      ---------------------------------------------------------------------*/
      TDSRLCI_RESET_AM_SN_STATUS_OPT_BITMAP( 
             dl_status_opt_bitmap.status_bitmap, vr_r, TDSRLC_AM_HALF_MOD);
      /*---------------------------------------------------------------------
        This PDU breaks sn_li list just increment num_sn_li and reset 
        distance to start counting next sn_li pair from next pdu onwards.
      ---------------------------------------------------------------------*/
      if (distance > 0)
      {
        num_sn_li++;
        distance = 0;
      }
    }
    else 
    {
      /*---------------------------------------------------------------------
        This is hole increment distance and num_sn_li pair accordingly..
      ---------------------------------------------------------------------*/
      distance++;
      if (distance > 15)
      {
        num_sn_li++;
        distance = 0;
      }
      if ((ctrl_blk_ptr->ctrl.am_ctrl.dl_ctl_cnfg->rlc_info.status_info.missing_pdu) && 
         (TDSRLCI_SEQ_GE(vr_r, am_ctrl_ptr->dl_state_var.prev_vr_h, TDSRLC_AM_MODULUS)))
      {
        tdsdl_state_ptr->rcvd_poll = TRUE; 
#ifdef FEATURE_TDSCDMA_RLC_MISSING_PDU_REPEAT_STATUS
        /*  
         * Remember that Missing PDU Indicator is triggering the status *
         */
		if(UE_LOGCHAN_DCCH == ctrl_blk_ptr->lc_type)
        {
          tdsdl_state_ptr->missing_pdu_ind = TRUE;
		}
#endif

      }
    }
  }
  /*---------------------------------------------------------------------
    Count for last sn_li pair..
  ---------------------------------------------------------------------*/
  if (distance > 0)
  {
    num_sn_li++;
  }
  /*---------------------------------------------------------------------
    Copy back updated DL status array and num_sn_li, relase mutex.
  ---------------------------------------------------------------------*/
  datapath_mutex = TDSL2_ACQ_UL_LW_LOCK(am_ctrl_ptr->am_common->ul_control_id);

  /*(void)memcpy(&ctrl_blk_ptr->ctrl.am_ctrl.dl_status_opt_bitmap, 
              &dl_status_opt_bitmap,sizeof(tdsrlc_opt_status_type));*/

  memscpy(&ctrl_blk_ptr->ctrl.am_ctrl.dl_status_opt_bitmap, sizeof(tdsrlc_opt_status_type),
              &dl_status_opt_bitmap,sizeof(tdsrlc_opt_status_type));  

  ctrl_blk_ptr->ctrl.am_ctrl.num_sn_li = num_sn_li;

  am_ctrl_ptr->dl_state_var.dl_vr_h_status = max_rx_sn;
  
  TDSL2_ACQ_UL_LW_UNLOCK(datapath_mutex);

  if (tdsrlc_debug_var & TDSRLC_DBG_DL_AM)
  {
    TDSRLC_MSG3(MSG_LEGACY_HIGH, "BITMAP_TST:num_sn_li %d, vr_r %d, max_rx_sn %d",
         ctrl_blk_ptr->ctrl.am_ctrl.num_sn_li,
          am_ctrl_ptr->dl_state_var.vr_r, max_rx_sn);
  }
  TDSL2_ACQ_DL_UNLOCK();
} /* tdsrlci_enh_update_nak_list          () */

/*===========================================================================

FUNCTION tdsrlci_enh_status_generation        

DESCRIPTION
    - As part of Post-Processing, Status message will be prepared
      - Copy NAK item from NAK_LIST_Q to TX_NAK_Q
      - Let the UL know that it has to send a status report to the peer entity
      - Clear the rcvd poll bit if set
      - Turn the status prohibit flag to TRUE to prohibit the sending further status

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlci_enh_status_generation        (tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr)
{
  tdsrlci_dl_am_ctrl_type *am_ctrl_ptr = &(ctrl_blk_ptr->ctrl.am_ctrl);
  tdsrlci_dl_am_state_type  *tdsdl_state_var_ptr = &(am_ctrl_ptr->dl_state_var);

  TDSL2_ACQ_DL_LOCK();

  tdsrlci_rx_status_changed(am_ctrl_ptr->am_common->ul_control_id,
                         FALSE);
  am_ctrl_ptr->dl_status_opt_bitmap.next_sn_valid = FALSE;
  tdsrlci_set_ul_num_sn_li(am_ctrl_ptr->am_common->ul_data_id,
            am_ctrl_ptr->num_sn_li);
  if(tdsrlc_debug_var & TDSRLC_DBG_DL_CTRL)
  {
    TDSRLC_MSG3(MSG_LEGACY_HIGH,"DL_CTRL: Sending status rpt for lc %d - report_vr_h %d, vr_r %d",
             am_ctrl_ptr->am_common->dl_data_id,
             tdsdl_state_var_ptr->report_vr_h, tdsdl_state_var_ptr->vr_r);
  }

  /* Let the UL know that it has to send a status report to the peer entity */
  tdsrlci_rx_status_changed(am_ctrl_ptr->am_common->ul_data_id, TRUE);

  /* Clear the rcvd poll bit if set */
  tdsdl_state_var_ptr->rcvd_poll = FALSE;

  /* Turn the status prohibit flag to TRUE to prohibit the sending further status */
  if (am_ctrl_ptr->dl_ctl_cnfg->rlc_info.status_info.status_info_incl &&
      am_ctrl_ptr->dl_ctl_cnfg->rlc_info.status_info.status_prohibit)
  {
    tdsdl_state_var_ptr->status_prohibit = TRUE;

    if(tdsrlc_debug_var & TDSRLC_DBG_DL_CTRL)
    {    
      TDSRLC_MSG2(MSG_LEGACY_HIGH,"DL_CTRL: Turn on Status Prohibit LC %d,timer_val %d",am_ctrl_ptr->am_common->dl_data_id,am_ctrl_ptr->dl_ctl_cnfg->rlc_info.status_info.status_prohibit);
    }
  }

  TDSL2_ACQ_DL_UNLOCK();
}

/*===========================================================================

FUNCTION tdsrlci_enh_dl_reassembly_post_pdu   

DESCRIPTION
    - For AM, Once the logical channel is changed, call the Post processing 
      functionality which will work on the received PDUs in this call
       - If needed NAK list will be prepared
       - If needed, reassembly functionality is called to prepare SDUs out of
         received in-seq PDUs.
       - If needed and allowed, Status message will be prepared

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_enh_dl_reassembly_post_pdu   (tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr)
{
  tdsrlci_dl_am_state_type  *tdsdl_state_var_ptr;

  tdsdl_state_var_ptr = &(ctrl_blk_ptr->ctrl.am_ctrl.dl_state_var);
 
  tdsrlci_enh_update_nak_list          (ctrl_blk_ptr);

#ifdef FEATURE_TDSCDMA_RLC_MISSING_PDU_REPEAT_STATUS  
  if ((UE_LOGCHAN_DCCH == ctrl_blk_ptr->lc_type) &&
      (tdsdl_state_var_ptr->vr_r == tdsdl_state_var_ptr->vr_h))
  {
    /* Clear the flag as there are no holes further */
    tdsdl_state_var_ptr->missing_pdu_ind = FALSE;
  }
#endif

  /* If allowed and required, Send the status */
  if ((tdsdl_state_var_ptr->status_prohibit == FALSE) &&
      (tdsdl_state_var_ptr->rcvd_poll))
  {
    tdsrlci_enh_status_generation        (ctrl_blk_ptr);
  }
  
  if (tdsrlc_debug_var & TDSRLC_DBG_REASSEMBLY)
  {
    TDSRLC_MSG2(MSG_LEGACY_HIGH,"reassembly_post_pdu: old vrR %d,vrR %d",tdsdl_state_var_ptr->old_vr_r,tdsdl_state_var_ptr->vr_r);
  }

  /* If there is update in vr_r, call the reassembly functionality */
  if (tdsdl_state_var_ptr->old_vr_r != tdsdl_state_var_ptr->vr_r)
  {
    if (tdsrlc_debug_var & TDSRLC_DBG_REASSEMBLY)
    {
      TDSRLC_MSG0(MSG_LEGACY_HIGH,"reassembly_post_pdu: Calling reassemble");
    }
    
    tdsrlci_enh_dl_am_reassemble_data_pdus(ctrl_blk_ptr, tdsdl_state_var_ptr->old_vr_r);
  /* Since there is no lock used inside tdsrlci_enh_dl_am_reassemble_data_pdus(), there is 
     no need to reassembly via posting a signal as done below and doing reassembly 10 SDUs
     at a given time*/
#if 0
      TDSRLC_MSG0(MSG_LEGACY_HIGH,"reassembly_post_pdu: Posting Reassembly SIGNAL");
    /* Post a signal to reassemble the PDUs later */
    (void) rex_set_sigs(&tds_l2_dl_tcb, TDSRLC_DL_DATA_REASSEMBLY_SIG);
#endif
  }

}

/*===========================================================================

FUNCTION tdsrlc_enh_rcvd_data_post_processing 

DESCRIPTION
    - As part of the post_processing functionality
      - 1. If configured, Post the logging data
      - 2. For all the previously received PDUs on prev_lc_id, 
             For AM - Call the Post procesing function
             For UM - Reassembly is taken care during the PDU handling itself
                      If there is anything left over in dl_pdu_info_ptr 
                      for this LgCh,
                        - Update to partial_sdu_ptr
      - 3. If any SDU is enqueued into Water mark and Call back is registerd, 
           Call the callback function to post SDU data to Upper Layers

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_enh_rcvd_data_post_processing (tdsrlci_dl_ctrl_blk_type *ctrl_blk_ptr,
                                       uint8 prev_rlc_id, 
                                       boolean hs_path)
{
  tdsrlci_dl_temp_pdu_info_type *dl_pdu_info_ptr;

  if (ctrl_blk_ptr == NULL)
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: post processing on prev_rlc_id %d - NULL Ctrl Blk",prev_rlc_id);
    return;
  }
  
  /* 
   * For all the previously received PDUs on prev_lc_id, 
   *   For AM - Call the Post procesing function
   *   For UM - Reassembly is taken care duing the PDU handling itself
   *            If there is anything left over in dl_pdu_info_ptr for this LgCh,
   *            Update to partial_sdu_ptr
   */
  if (ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED)
  {
    tdsrlci_enh_dl_reassembly_post_pdu   (ctrl_blk_ptr);
  }
  else if (ctrl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED)
  {
    dl_pdu_info_ptr = tdsrlci_enh_get_temp_pdu_info_ptr();
    if (dl_pdu_info_ptr->dsm_ptr != NULL)
    {
      if (dl_pdu_info_ptr->dsm_ptr->references > 0) 
      {
        tdsrlci_enh_update_um_partial_sdu_ptr(ctrl_blk_ptr, 
                                         dl_pdu_info_ptr, NULL);
      }
      else  
      {
        /* Case: SN-JUMP is in same DSM-item
         * {BIG-DSM1:[..._SN-1(LI)_, _SN_, _SN+1_, _SN+10_, _SN+11_,...]}
         *                             ^             ^
                                       ^             ^-<- SN-JUMP is in same BIG-DSM item,
                                       ^             So the DSM-item might be deleted earlier while
                                       ^             discarding PDUs (containing no LI) 
         *                             ^-<-dl_pdu_info (app_field = 0, ref-cnt = 0)
         * dl_pdu_info contains ref. to already freed BIG-DSM1 item. 
         * Hence avoid appending this item, & set dl_pdu_info to zero 
         * (this will help in removing dangling references)
         */
        TDSRLC_MSG3(MSG_LEGACY_HIGH,"RLC_DBG: DSM-item (within dl_pdu_info) is already freed LC-id %d vr_ur %d dl_pdu_info_ptr 0x%x",
                   prev_rlc_id, ctrl_blk_ptr->ctrl.um_ctrl.vr_ur, dl_pdu_info_ptr->dsm_ptr);
        
        memset(dl_pdu_info_ptr, 0, sizeof(tdsrlci_dl_temp_pdu_info_type));
        
      }
    }
  }
  if (tdsrlc_debug_var & TDSRLC_DBG_REASSEMBLY)
  {
    TDSRLC_MSG1(MSG_LEGACY_HIGH,"In Post processing - nSDU %d",ctrl_blk_ptr->n_sdu);
  }

  /* If any SDU in WM Queue and callback configured, post to upper layers */
  if ((ctrl_blk_ptr->n_sdu > 0) && (ctrl_blk_ptr->rlc_post_rx_func_ptr != NULL))
  {
    ctrl_blk_ptr->rlc_post_rx_func_ptr(prev_rlc_id, 
                                       (uint8)ctrl_blk_ptr->n_sdu, 
                                       ctrl_blk_ptr->rlc_post_rx_func_ptr_para);
    ctrl_blk_ptr->n_sdu = 0;
  }

  /* If configured, Post the logging data */
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
  if (((ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED) &&
        TDSRLCI_GET_AM_LOGGING_CONFIG(prev_rlc_id)) ||
      ((ctrl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED) && 
        TDSRLCI_GET_UM_LOGGING_CONFIG))
  {
    tdsrlc_enh_logging_post_data_handler (ctrl_blk_ptr, prev_rlc_id, hs_path);
  }
#endif

}


/*===========================================================================

FUNCTION tdsrlc_enh_dl_pdu_data_handler_hs

DESCRIPTION
    - For every log Channel
       - Process each PDU one after another
    - For AM, Once the logical channel is changed, call the Post processing 
      functionality (Reassembly, Status building, Status preperation)
      which will work on the received PDUs in this call
    - For UM and TM, during the PDU processing itself Post processing is completed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_enh_dl_pdu_data_handler_hs(tdsl1_dec_hs_tsn_info_type *tsn_info_buf_ptr)
{
  uint8                            index, ctrl_index, prev_rlc_id = 0xFF;
  boolean                          first_pdu_on_this_lch = FALSE;
  tds_complete_pdu_type               *complete_pdu_ptr;
  tdsrlci_dl_ctrl_blk_type           *ctrl_blk_ptr = NULL;

  if (tsn_info_buf_ptr == NULL)
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"RLC_ERR: RLC Internal TSN Info Q is empty");
    return;
  }


#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
  if ((tsn_info_buf_ptr->num_partial_pdus != 0) ||
      (tsn_info_buf_ptr->num_complete_pdus == 0))
  {
    ERR_FATAL("TSN %d - Not expected Num partial in RLC - Partial %d, Complete %d", 
               tsn_info_buf_ptr->tsn, tsn_info_buf_ptr->num_partial_pdus, 
               tsn_info_buf_ptr->num_complete_pdus);
  }
#endif
  for (index = 0; index < tsn_info_buf_ptr->num_complete_pdus; index++)
  {
    complete_pdu_ptr = &(tsn_info_buf_ptr->TDS_COMPLETE_PDU[index]);
    
    if (prev_rlc_id != complete_pdu_ptr->rlc_id)
    {
      if (prev_rlc_id != 0xFF)
      {
        /* Call the post processing function for the data rcvd on prev_rlc_id */
        tdsrlc_enh_rcvd_data_post_processing (ctrl_blk_ptr, prev_rlc_id, TRUE);
      }
      
      /* Get the new ctrl Index and ctrl blk ptr */
      ctrl_index = TDSRLCI_GET_CTRL_INDEX(complete_pdu_ptr->rlc_id);
      if ((ctrl_index == TDSINIT_CTL_BLK_ID) || (ctrl_index >= TDSUE_MAX_DL_RLC_ENTITY))
      {
        TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: rlc_id %d is not yet configured - Discard PDU",complete_pdu_ptr->rlc_id);
        tdsrlc_discard_dsm_item(&(complete_pdu_ptr->iovec.ptr));
        continue;
      }
      
      ctrl_blk_ptr = TDSRLCI_GET_CTRL_BLK_PTR(ctrl_index);
      if (ctrl_blk_ptr->lc_state == TDSDL_NULL_STATE)
      {
        TDSRLC_MSG2(MSG_LEGACY_ERROR,"RLC_ERR: lc_state %d is NULL for rlc_id %d - Discard PDU",ctrl_blk_ptr->lc_state,complete_pdu_ptr->rlc_id);
        tdsrlc_discard_dsm_item(&(complete_pdu_ptr->iovec.ptr));
        continue;
      }
      
      ctrl_blk_ptr->n_sdu = 0;
      prev_rlc_id = complete_pdu_ptr->rlc_id;
      first_pdu_on_this_lch = TRUE;
    }
    if (NULL == ctrl_blk_ptr)
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"ctrl_blk_ptr is NULL for LC id %d",complete_pdu_ptr->rlc_id);
      continue;
    }
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
    if (((ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED) &&
         (TDSRLCI_GET_AM_LOGGING_CONFIG(prev_rlc_id))) ||
        ((ctrl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED) &&
         (TDSRLCI_GET_UM_LOGGING_CONFIG)))
    {
      tdsrlc_enh_logging_data_handler      (ctrl_blk_ptr, complete_pdu_ptr, 
                                   first_pdu_on_this_lch);
    }
#endif

    switch (ctrl_blk_ptr->lc_mode)
    {
      case UE_MODE_ACKNOWLEDGED:
        tdsrlci_enh_dl_process_am_pdu(ctrl_blk_ptr, complete_pdu_ptr, 
                                   first_pdu_on_this_lch);
       break;
       
      case UE_MODE_UNACKNOWLEDGED:
        complete_pdu_ptr->rlc_hdr_data = (uint32) (complete_pdu_ptr->rlc_hdr_data >> 8);
        tdsrlci_enh_dl_process_um_pdus(ctrl_blk_ptr, complete_pdu_ptr);
       break;
       
      case UE_MODE_TRANSPARENT:
       TDSRLC_MSG0(MSG_LEGACY_ERROR,"RLC_ERR: tdsrlc_enh_dl_pdu_data_handler_hs shouldn't get TM PDU");
       break;
       
      default:
       TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: tdsrlc_enh_dl_pdu_data_handler_hs - Unknown Mode",ctrl_blk_ptr->lc_mode);
       break;
    }
    
    first_pdu_on_this_lch = FALSE;
    
    /* 
     * PDU is consumed by the RLC. Either into reassembly queue or 
     * discarded after processing.
     */
    complete_pdu_ptr->iovec.ptr = NULL;
  }
  
  /* Call the post processing function for the data rcvd on prev_rlc_id */
  tdsrlc_enh_rcvd_data_post_processing (ctrl_blk_ptr, prev_rlc_id, TRUE);

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
  /* Call the log packed for EHS Call */
  if (tdsmac_ehs_query_hs_type() == MAC_EHS)
  {
    tdsrlc_dl_enh_r7_am_pdu_log_pkt(&tdsdownlink_table);
  }
#endif
#endif

  /* Call the MAC-HS API to add processed TSN to free TSN Queue */
  tdsmac_hs_enqueue_free_tsn(tsn_info_buf_ptr);
}


/*===========================================================================

FUNCTION tdsrlc_enh_handle_tm_data            

DESCRIPTION
    - Process TM data, if present and configured to process in interrupt context
    - If some data present in TM with interrupt context TRUE, 
        - Process the TM PDUs
    - If some more PDUs are left in the rcvd, copy them into local buffer
      and back into rlc_enh_data_ptr
    - If nothing, return the entry to free queue

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_handle_tm_data            (tdsrlc_enh_data_ind_type *rlc_enh_data_ptr)
{
  uint8                            index, ctrl_index, num_processed = 0;
  tds_complete_pdu_type               *complete_pdu_ptr;
  tdsrlci_dl_ctrl_blk_type           *ctrl_blk_ptr = NULL;
  uint8                            rlc_id_list[3];
  
  for (index = 0; index < rlc_enh_data_ptr->num_complete_pdus; index++)
  {
    complete_pdu_ptr = &(rlc_enh_data_ptr->TDS_COMPLETE_PDU[rlc_enh_data_ptr->num_complete_pdus-1-index]);
    
    /* Get the new ctrl Index and ctrl blk ptr */
    ctrl_index = TDSRLCI_GET_CTRL_INDEX(complete_pdu_ptr->rlc_id);
    if ((ctrl_index == TDSINIT_CTL_BLK_ID) || (ctrl_index >= TDSUE_MAX_DL_RLC_ENTITY))
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: rlc_id %d is not yet configured - Discard PDU in Task-context",complete_pdu_ptr->rlc_id);
      continue;
    }

    ctrl_blk_ptr = TDSRLCI_GET_CTRL_BLK_PTR(ctrl_index);
    if (ctrl_blk_ptr->lc_state == TDSDL_NULL_STATE)
    {
      TDSRLC_MSG2(MSG_LEGACY_ERROR,"RLC_ERR: lc_state %d is NULL for rlc_id %d - Discard PDU in Task-context",ctrl_blk_ptr->lc_state,complete_pdu_ptr->rlc_id);
      continue;
    }
      
    /* If rcvd PDU is of TM Mode and need to be processed in Interrupt
     * context, process immediately and update iovec.ptr to NULL
     */
    if ((ctrl_blk_ptr->lc_mode == UE_MODE_TRANSPARENT) &&
        (ctrl_blk_ptr->context == TRUE))
    {
      if (NULL != ctrl_blk_ptr->dl_wm_ptr)
      {
        rlc_id_list[num_processed] = complete_pdu_ptr->rlc_id;
        tdsrlci_enh_dl_reassemble_tm_pdus(ctrl_blk_ptr, complete_pdu_ptr);
        complete_pdu_ptr->iovec.ptr = NULL;
        num_processed++;
	  }
      else
      {
        TDSRLC_MSG2(MSG_LEGACY_HIGH,"WM NULL for LC %d, lc_state %d,Discard PDU in Task-context",complete_pdu_ptr->rlc_id,ctrl_blk_ptr->lc_state);
      }
    }
  }
  
  if (num_processed == 3)
  {
    if (!((rlc_id_list[0]>rlc_id_list[1])&&(rlc_id_list[1]>rlc_id_list[2])))
    {
      TDSRLC_MSG3(MSG_LEGACY_ERROR,"RLC_ERR: wrong rlc_id sequence %d, %d, %d", 
                  rlc_id_list[0], rlc_id_list[1], rlc_id_list[2]);
    }
  }
  else if (num_processed == 2)
  {
    if (!(rlc_id_list[0]>rlc_id_list[1]))
    {
      TDSRLC_MSG2(MSG_LEGACY_ERROR,"RLC_ERR: wrong rlc_id sequence %d,%d",rlc_id_list[0],rlc_id_list[1]);
    }
  }
  
  /* 
   * If all the PDUs are processed, update the num_complete_pdus to 0
   * Otherwise, rlc_enh_data_ptr will be enqueue to RLC for processing
   * During Processing all TM data, ISR Mode will not be processed
   */
  if (num_processed == rlc_enh_data_ptr->num_complete_pdus)
  {
    rlc_enh_data_ptr->num_complete_pdus = 0;
  }
}

/*===========================================================================

FUNCTION tdsrlc_enh_dl_pdu_data_handler_non_hs

DESCRIPTION
    - For every log Channel
       - Process each PDU one after another
    - For AM, Once the logical channel is changed, call the Post processing 
      functionality (Reassembly, Status building, Status preperation)
      which will work on the received PDUs in this call
    - For UM and TM, during the PDU processing itself Post processing is completed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_enh_dl_pdu_data_handler_non_hs(tdsrlc_enh_data_ind_type  *rlc_enh_data_ptr)
{
  uint8                            index, ctrl_index, prev_rlc_id = 0xFF;
  boolean                          first_pdu_on_this_lch = FALSE;
  tds_complete_pdu_type               *complete_pdu_ptr;
  tdsrlci_dl_ctrl_blk_type           *ctrl_blk_ptr = NULL;
  tdsrlci_dl_temp_pdu_info_type      *dl_pdu_info_ptr;  
  
  if (rlc_enh_data_ptr == NULL)
  {
    TDSRLC_MSG0(MSG_LEGACY_ERROR,"RLC_ERR: RLC Internal Info Q is empty");
    return;
  }
  
  for (index = 0; index < rlc_enh_data_ptr->num_complete_pdus; index++)
  {
    complete_pdu_ptr = &(rlc_enh_data_ptr->TDS_COMPLETE_PDU[index]);
    
    if (prev_rlc_id != complete_pdu_ptr->rlc_id)
    {
      if (prev_rlc_id != 0xFF)
      {
        /* Call the post processing function for the data rcvd on prev_rlc_id */
        tdsrlc_enh_rcvd_data_post_processing (ctrl_blk_ptr, prev_rlc_id, FALSE);
      }
      
      /* Get the new ctrl Index and ctrl blk ptr */
      ctrl_index = TDSRLCI_GET_CTRL_INDEX(complete_pdu_ptr->rlc_id);
      if ((ctrl_index == TDSINIT_CTL_BLK_ID) || (ctrl_index >= TDSUE_MAX_DL_RLC_ENTITY))
      {
        TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: rlc_id %d is not yet configured - Discard PDU",complete_pdu_ptr->rlc_id);
		tdsrlc_discard_pdu(complete_pdu_ptr);
        continue;
      }
      
      ctrl_blk_ptr = TDSRLCI_GET_CTRL_BLK_PTR(ctrl_index);
      if (ctrl_blk_ptr->lc_state == TDSDL_NULL_STATE)
      {
        TDSRLC_MSG2(MSG_LEGACY_ERROR,"RLC_ERR: lc_state %d is NULL for rlc_id %d - Discard PDU",ctrl_blk_ptr->lc_state,complete_pdu_ptr->rlc_id);
		tdsrlc_discard_pdu(complete_pdu_ptr);
        continue;
      }
      
      ctrl_blk_ptr->n_sdu = 0;
      
      /* Try commenting this code - Not really needed - Added during initial
       * debugging - Not sure really needed - Anju */
       
      /* Clear the temp_tdspdu_info before starting on UM data Processing */
      if (ctrl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED)
      {
        dl_pdu_info_ptr = tdsrlci_enh_get_temp_pdu_info_ptr();
        memset(dl_pdu_info_ptr, 0, sizeof(tdsrlci_dl_temp_pdu_info_type));
      }
      
      prev_rlc_id = complete_pdu_ptr->rlc_id;
      first_pdu_on_this_lch = TRUE;
    }
    if (NULL == ctrl_blk_ptr)
    {
      TDSRLC_MSG1(MSG_LEGACY_ERROR,"ctrl_blk_ptr is NULL for LC id %d",complete_pdu_ptr->rlc_id);
      continue;
    }
#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
    if (((ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED) &&
            TDSRLCI_GET_AM_LOGGING_CONFIG(prev_rlc_id)) ||
          (ctrl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED))
    {
      tdsrlc_enh_logging_data_handler      (ctrl_blk_ptr, complete_pdu_ptr, 
                           first_pdu_on_this_lch);
    }
#endif
    
    switch (ctrl_blk_ptr->lc_mode)
    {
      case UE_MODE_ACKNOWLEDGED:
        tdsrlci_enh_dl_process_am_pdu(ctrl_blk_ptr, complete_pdu_ptr, 
                                   first_pdu_on_this_lch);
       break;
       
      case UE_MODE_UNACKNOWLEDGED:
        complete_pdu_ptr->rlc_hdr_data = (uint32) (complete_pdu_ptr->rlc_hdr_data >> 8);
        tdsrlci_enh_dl_process_um_pdus(ctrl_blk_ptr, complete_pdu_ptr);
       break;
       
      case UE_MODE_TRANSPARENT:
        /* 
         * Handle TM data here, only if configured to be handled in Task context 
         * If configured to be handled in ISR Context, Data is already processed 
         */
        if (ctrl_blk_ptr->context == FALSE)
        {
          tdsrlci_enh_dl_reassemble_tm_pdus(ctrl_blk_ptr, complete_pdu_ptr);
        }
        else if (NULL != complete_pdu_ptr->iovec.ptr)
        {
          TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: Discarding PDU for LC %d,not processed in ISR",complete_pdu_ptr->rlc_id);
          dsm_free_packet(&(complete_pdu_ptr->iovec.ptr));
        }
       break;
       
      default:
       TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR: Unknown Mode",ctrl_blk_ptr->lc_mode);
       break;
    }
    
    first_pdu_on_this_lch = FALSE;
    
    /* 
     * PDU is consumed by the RLC. Either into reassembly queue or 
     * discarded after processing.
     */
    complete_pdu_ptr->iovec.ptr = NULL;
  }
  
  /* Call the post processing function for the data rcvd on prev_rlc_id */
  tdsrlc_enh_rcvd_data_post_processing (ctrl_blk_ptr, prev_rlc_id, FALSE);

  if (tdsrlc_debug_var & TDSRLC_DBG_REASSEMBLY)
  {
    /* Add the entry into free_data_queue */
    TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_DL:R99 DATA PTR FREE: enqueue back to free Q ");
  }
  q_put(&tdsrlc_enh_free_data_q, &(rlc_enh_data_ptr->link));
}

/*===========================================================================

FUNCTION tdsrlc_enh_dl_pdu_data_handler       

DESCRIPTION
    - Process the Non-HS data, if received
       - Get the RLC Info from the rlc_enh_data_queue. Protect this operation with 
         TDS_INTLOCK, as it this queue is accessed by MAC-HS also
       - After processing all the entries(PDUs) in the received rlc_enh_data_ptr,
         rlc_enh_data_ptr will be added to free queue
    - Process the HS data, if received
       - Get the TSN from the rlc_tsn_info_q. Protect this operation with TDS_INTLOCK
         as it this queue is accessed by MAC-HS also
       - After processing all the entries(PDUs) in the received tsn_info_buf_ptr,
         tsn_info_buf_ptr will be added to free queue

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_dl_pdu_data_handler       (void)
{
  tdsrlc_enh_data_ind_type       *rlc_enh_data_ptr;
  tdsl1_dec_hs_tsn_info_type     *tsn_info_buf_ptr;
  uint32                      num_tsn;

  /* Store the UM and AM log info */
  tdsrlci_enh_dl_log_info_handler();
  
  TDSL2_MUTEX_LOCK(&tdsrlc_dl_data_process_mutex);
  
  /* Process the Non-HS data, if received */
  if ((num_tsn = q_cnt(&tdsrlc_enh_data_q)) > 0)
  {
    while (num_tsn != 0)
    {
      /* Get the RLC Info from the tdsrlc_enh_data_queue */
      if ((rlc_enh_data_ptr = (tdsrlc_enh_data_ind_type *)q_get(&tdsrlc_enh_data_q)) 
             != NULL)
      {
        tdsrlc_enh_dl_pdu_data_handler_non_hs(rlc_enh_data_ptr);
      }
      num_tsn--;
    }
  }
  
  TDSL2_MUTEX_LOCK(&tdsrlc_tsn_mutex);

  /* Process the HS data, if received */
  if ((tdsl2_nv_rel_indicator >= TDSL2_REL_INDICATOR_5) && 
      ((num_tsn = q_cnt(&tdsrlc_tsn_info_q)) > 0))
  {
    while (num_tsn != 0)
    {
      /* Get the TSN from the tdsrlc_tsn_info_q */
      if ((tsn_info_buf_ptr = (tdsl1_dec_hs_tsn_info_type *)q_get(&tdsrlc_tsn_info_q)) 
             != NULL)
      {
        /*Set the trace value to TDSDL_TSN_RLC*/
        tsn_info_buf_ptr->tracer = TDSDL_TSN_RLC;
        tdsrlc_enh_dl_pdu_data_handler_hs(tsn_info_buf_ptr);
      }
      num_tsn--;
    }
  }

  TDSL2_MUTEX_UNLOCK(&tdsrlc_tsn_mutex);

  TDSL2_MUTEX_UNLOCK(&tdsrlc_dl_data_process_mutex);
}

/*===========================================================================

FUNCTION tdsrlc_enh_get_free_data_q_entry     

DESCRIPTION
    - Returns entry from tdsrlc_enh_free_data_q. 
      If Quue is empty, NULL will be returned

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

tdsrlc_enh_data_ind_type *tdsrlc_enh_get_free_data_q_entry     (void)
{
  return((tdsrlc_enh_data_ind_type *)q_get(&tdsrlc_enh_free_data_q));
}

/*===========================================================================

FUNCTION tdsrlc_enh_add_to_free_data_q_entry  

DESCRIPTION
    - Adds the tdsrlc_enh_data entry into Free Data Queue tdsrlc_enh_free_data_q

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_enh_add_to_free_data_q_entry  (tdsrlc_enh_data_ind_type *tdsrlc_enh_data_ind_ptr)
{
  q_put(&tdsrlc_enh_free_data_q, &(tdsrlc_enh_data_ind_ptr->link));
}

/* DSM touch function for every DSM item */
static void tdsrlc_touch_entry_dsm_item          
(
  tdsrlc_enh_data_ind_type *rlc_enh_data_ptr
)
{
  uint32 i;

  for (i = 0; i < rlc_enh_data_ptr->num_complete_pdus; i++)
  {
    if (rlc_enh_data_ptr->TDS_COMPLETE_PDU[i].iovec.offset == 0)
    {
      dsmi_touch_item(rlc_enh_data_ptr->TDS_COMPLETE_PDU[i].iovec.ptr,
                      tdsrlc_entry, __LINE__);
    }
  }
}

/*===========================================================================

FUNCTION tdsrlc_enh_dl_pdu_data_handler       _mac

DESCRIPTION
    - Process TM data, if present and configured to process in interrupt context
    - If some data present, 
        - Enqueue the RLC entry into tdsrlc_enh_data_queue
        - Post a signal to RLC DL Task to process in DL Task context
      else
        - Return the entry to free queue

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_dl_pdu_data_ind           (tdsrlc_enh_data_ind_type *rlc_enh_data_ptr)
{
  if(tdsrlc_debug_var & TDSRLC_DBG_MEM_LEAK)
  {
    tdsrlc_touch_entry_dsm_item          (rlc_enh_data_ptr);
  }

  /* Process TM data, if configured and present */
  tdsrlc_enh_handle_tm_data            (rlc_enh_data_ptr);
  
  if (rlc_enh_data_ptr->num_complete_pdus > 0)
  {
    /* Enqueue the RLC entry into tdsrlc_enh_data_queue */
    q_put(&tdsrlc_enh_data_q, &(rlc_enh_data_ptr->link));
    
    /* Post a signal to RLC DL Task to process in DL Task context */
    (void) rex_set_sigs(tds_l2_dl_get_tcb(), TDSRLC_DL_DATA_IND_SIG);  
  }
  else
  {
    /* If all the PDUs are consumed, add the rlc_enh_data_ptr to free queue */
    q_put(&tdsrlc_enh_free_data_q, &(rlc_enh_data_ptr->link));
  }
}


/* DSM touch function for every DSM item */
static void tdsrlc_touch_entry_dsm_item_hs
(
  tdsl1_dec_hs_tsn_info_type *rlc_enh_data_ptr
)
{
  uint32 i;

  for (i = 0; i < rlc_enh_data_ptr->num_complete_pdus; i++)
  {
    if (rlc_enh_data_ptr->TDS_COMPLETE_PDU[i].iovec.offset == 0)
    {
      dsmi_touch_item(rlc_enh_data_ptr->TDS_COMPLETE_PDU[i].iovec.ptr,
                      tdsrlc_entry, __LINE__);
    }
  }
}

void  tdsrlc_dsm_touch_exit                
(
  dsm_item_type *pkt_head_ptr
)
{
  while (pkt_head_ptr != NULL)
  {
    dsmi_touch_item(pkt_head_ptr, tdsrlc_exit, __LINE__);
    pkt_head_ptr = pkt_head_ptr->pkt_ptr;
  }
}
/*===========================================================================

FUNCTION rlc_enh_dl_tsn_flush_hs

DESCRIPTION
    - Flush the TSN from TSN q
    - 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  There might be slight data loss dueing HS STOP

===========================================================================*/
void tdsrlc_enh_dl_tsn_flush_hs(void)
{
  tdsl1_dec_hs_tsn_info_type *tsn_info_buf_ptr = NULL;

  TDSL2_MUTEX_LOCK(&tdsrlc_tsn_mutex);
  
  while ((tsn_info_buf_ptr = (tdsl1_dec_hs_tsn_info_type *)q_get(&tdsrlc_tsn_info_q)) 
             != NULL)
  {
    /*Discard the DSM item and enqueue in free q*/
    tdsmac_hs_dl_discard_tsn(tsn_info_buf_ptr);
    tdsmac_hs_enqueue_free_tsn(tsn_info_buf_ptr);
  }
  
  TDSL2_MUTEX_UNLOCK(&tdsrlc_tsn_mutex);
  
}

/*===========================================================================

FUNCTION tdsrlc_enh_dl_pdu_data_ind_hs

DESCRIPTION
    - Enqueue the tsn entry into tdsrlc_tsn_info_q
    - Post a signal to RLC DL Task to process in DL Task context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlc_enh_dl_pdu_data_ind_hs(tdsl1_dec_hs_tsn_info_type *tsn_info_buf_ptr)
{

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
  uint8 array_len = 0;

  if (tsn_info_buf_ptr->num_partial_pdus != 0)
  {
    ERR_FATAL("TSN %d - Not expected Num partial in RLC - Partial %d, Complete %d", 
               tsn_info_buf_ptr->tsn, tsn_info_buf_ptr->num_partial_pdus, 
               tsn_info_buf_ptr->num_complete_pdus);
  }
#endif

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
  array_len = sizeof(tsn_info_buf_ptr->TDS_COMPLETE_PDU)/sizeof(tds_complete_pdu_type);
  /*Corner case: If the tb_id is set to 0xFF discard the last complete PDU which is p-t-c 
    which failed PDU decode*/
  if ((tsn_info_buf_ptr->tb_id == 0xFF) && (tsn_info_buf_ptr->num_complete_pdus > 0) && 
      (tsn_info_buf_ptr->num_complete_pdus < array_len))
  {
     if(tsn_info_buf_ptr->num_complete_pdus <= TDSL1_DEC_MAX_DL_RLC_PDU)
     {
       tdsrlc_discard_dsm_item(
       &(tsn_info_buf_ptr->TDS_COMPLETE_PDU[tsn_info_buf_ptr->num_complete_pdus-1].iovec.ptr));
       tsn_info_buf_ptr->num_complete_pdus--;
       tsn_info_buf_ptr->tb_id = 0;
     }
     else
     {
       TDSRLC_MSG0(MSG_LEGACY_ERROR,"Number of valid fields in tsn_info_buf_ptr->TDS_COMPLETE_PDU[] exceed TDSL1_DEC_MAX_DL_RLC_PDU");
     }
  }
#endif

  if (tsn_info_buf_ptr->num_complete_pdus == 0)
  {
    //TDSRLC_MSG1(MSG_LEGACY_HIGH,"TSN %d with no complete pdus",tsn_info_buf_ptr->tsn);
    /* If all the PDUs are consumed, add the tsn_info_buf_ptr to free queue */
    tdsmac_hs_enqueue_free_tsn(tsn_info_buf_ptr);
  }
  else
  {
    /*Set the tarcer value to TDSDL_TSN_RLC_Q*/
    tsn_info_buf_ptr->tracer = TDSDL_TSN_RLC_Q;

    if(tdsrlc_debug_var & TDSRLC_DBG_MEM_LEAK)
    {
       tdsrlc_touch_entry_dsm_item_hs(tsn_info_buf_ptr);
    }    

    /* Enqueue the tsn entry into tdsrlc_tsn_info_q */
    q_put(&tdsrlc_tsn_info_q, &(tsn_info_buf_ptr->link));
    
    /* Post a signal to RLC DL Task to process in DL Task context */
    (void) rex_set_sigs(tds_l2_dl_get_tcb(), TDSRLC_DL_DATA_IND_SIG);  

  }
}

#if 0
//#ifdef FEATURE_Q6_MT
/*===========================================================================

FUNCTION rlc_enh_dl_data_reassembly

DESCRIPTION  
  Go through all the AM Channels and if there is any old_vr_r which is different
  from vr_r, then there is some data left over and RLC can reassemble them.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  Need to optimize the function such that, only AM channels will be used 
  in FOR loop, than all the logical channels
  Data in the first AM LC will be completed before moving to next AM LC,
  as Signal doesnt know about the LC which triggered this action

===========================================================================*/

extern tdsrlci_downlink_table_type tdsdownlink_table;

void rlc_enh_dl_data_reassembly_signal(void)
{
  uint8 rb_id, lc_id, ctrl_index;
  uint16 old_vr_r;
  tdsrlci_dl_ctrl_blk_type           *ctrl_blk_ptr;
  tdsrlci_dl_am_state_type            *dl_state_ptr;
  //tdsrlci_dl_ctrl_blk_type           *ctrl_blk_ptr;
  
#if 0
  for (lc_index = 0; lc_index < TDSUE_MAX_DL_RLC_ENTITY; lc_index++)
  {
    ctrl_blk_ptr = TDSRLCI_GET_CTRL_BLK_PTR(lc_index);
    if ((ctrl_blk_ptr->lc_state != TDSDL_NULL_STATE) &&
        (ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED))
    {
      tdsdl_state_ptr = &(ctrl_blk_ptr->ctrl.am_ctrl.dl_state_var);
      if (tdsdl_state_ptr->old_vr_r != tdsdl_state_ptr->vr_r)
      {
        tdsrlci_enh_dl_am_reassemble_data_pdus(ctrl_blk_ptr, tdsdl_state_ptr->old_vr_r);
      }
    }
  }
#endif
  if (tdsrlc_debug_var & TDSRLC_DBG_REASSEMBLY)
  {
    TDSRLC_MSG0(MSG_LEGACY_HIGH,"Entering Reassembly Signal handler");
  }
  
  for (lc_id = 0; lc_id < TDSUE_MAX_DL_LOGICAL_CHANNEL; lc_id++)
  {
    ctrl_index = TDSRLCI_GET_CTRL_INDEX(lc_id);

    if (ctrl_index == TDSINIT_CTL_BLK_ID)
    {
      continue;
    }

    ctrl_blk_ptr = TDSRLCI_GET_CTRL_BLK_PTR(ctrl_index);
    if ((ctrl_blk_ptr->lc_state != TDSDL_NULL_STATE) &&
        (ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED))
    {
      tdsdl_state_ptr = &(ctrl_blk_ptr->ctrl.am_ctrl.dl_state_var);
      if (tdsdl_state_ptr->old_vr_r != tdsdl_state_ptr->vr_r)
      {
        tdsrlci_enh_dl_am_reassemble_data_pdus(ctrl_blk_ptr, tdsdl_state_ptr->old_vr_r);

        if (tdsrlc_debug_var & TDSRLC_DBG_REASSEMBLY)
        {
          TDSRLC_MSG2(MSG_LEGACY_HIGH,"Posting to higher layers: n_sdu %d lc id %d",ctrl_blk_ptr->n_sdu,lc_id);
        }

        /* If any SDU in WM Queue and callback configured, post to upper layers */
        if ((ctrl_blk_ptr->n_sdu > 0) && (ctrl_blk_ptr->rlc_post_rx_func_ptr != NULL))
        {
          ctrl_blk_ptr->rlc_post_rx_func_ptr(lc_id, 
                                             ctrl_blk_ptr->n_sdu, 
                                             ctrl_blk_ptr->rlc_post_rx_func_ptr_para);
          ctrl_blk_ptr->n_sdu = 0;
        }

      }
    }
  }

}
#endif /* FEATURE_Q6_MT */



/*===========================================================================

FUNCTION TDSRLCI_DEBUG_TRACE_PRINT_DATA

DESCRIPTION
  To print the contents of the data in the item_ptr.
  P.S: Needed trace length needs to be passed in number of words
  
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlci_debug_trace_print_data(dsm_item_type *walk_item_ptr, 
                                 uint16         needed_trace_length)
{
  uint32        temp_hdr[3];
  uint8         temp_i;
  uint16         length;

  length = ((needed_trace_length > TDSMAX_DATA_LOGGING) ? TDSMAX_DATA_LOGGING : needed_trace_length);
  TDSRLC_MSG1(MSG_LEGACY_HIGH,"PDU/SDU contents dumped %d words",length);
  temp_i = 0;
  while(dsm_extract(walk_item_ptr, temp_i*4, temp_hdr, 12) && (length/3 > 0))
  {
    TDSRLC_MSG3(MSG_LEGACY_HIGH,"RLC DATA : 0x%x 0x%x 0x%x", 
              TDSRLC_DEBUG_CHANGE_ENDIANNESS(temp_hdr[0]), 
              TDSRLC_DEBUG_CHANGE_ENDIANNESS(temp_hdr[1]), 
              TDSRLC_DEBUG_CHANGE_ENDIANNESS(temp_hdr[2]));
               
    temp_i = temp_i + 3;
    length =  length - 3;
  }

  if (length > 0)
  {
    TDSRLC_MSG3(MSG_LEGACY_HIGH,"RLC DATA : 0x%x 0x%x 0x%x", 
               TDSRLC_DEBUG_CHANGE_ENDIANNESS(temp_hdr[0]), 
               TDSRLC_DEBUG_CHANGE_ENDIANNESS(temp_hdr[1]), 
               TDSRLC_DEBUG_CHANGE_ENDIANNESS(temp_hdr[2]));
  }
}

/*===========================================================================

FUNCTION TDSRLCI_DBG_DUMP_DATA

DESCRIPTION
  To print the contents of the data in the item_ptr.
  P.S: Needed trace (offset,length) needs to be passed in number of bytes
  
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrlci_dbg_dump_pdu(dsm_item_type *dsm_ptr, uint16 offset, uint16 len)
{
  uint8 *data_ptr = dsm_ptr->data_ptr;
  if (len >=12)
  {
    TDSRLC_MSG3(MSG_LEGACY_HIGH,"PDU DUMP %x %x %x", 
                    tdsrlc_copy_to_word(data_ptr + offset),
                    tdsrlc_copy_to_word(data_ptr + offset + 4),
                    tdsrlc_copy_to_word(data_ptr + offset + 8));
      
  }
  else if (len >=8)
  {
    TDSRLC_MSG2(MSG_LEGACY_HIGH,"PDU DUMP %x %x %x",tdsrlc_copy_to_word(data_ptr + offset),tdsrlc_copy_to_word(data_ptr + offset + 4));
  }
  else
  {
    TDSRLC_MSG1(MSG_LEGACY_HIGH,"PDU DUMP %x %x %x",tdsrlc_copy_to_word(data_ptr + offset));
  }
}





#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION TDSRLCI_GENERATE_DL_CONFIG_EVENT

DESCRIPTION
  To generate an event whenever a DTCH logical channel is established, modified
  or restablished, to aid in automation. The argument rlc_size is used only
  in the case the UM RLC entity to pass the size to this function, as this
  information is not available in the control block.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlci_generate_dl_config_event     (tdsrlci_dl_ctrl_blk_type *ctrl_blk,
                                   uint32 rlc_size) 
{
  tdsrlci_event_tds_rlc_config_type event_tdscdma_rlc_config_data;
  //Initialization
  event_tdscdma_rlc_config_data.rlc_id = 0;
  event_tdscdma_rlc_config_data.direction_and_rlc_type = TDSDOWNLINK_FIXED_SIZE;
  event_tdscdma_rlc_config_data.size =0;
  if(UE_LOGCHAN_DTCH == ctrl_blk->lc_type)
  {
    switch (ctrl_blk->lc_mode)
    {
      case UE_MODE_UNACKNOWLEDGED:
        event_tdscdma_rlc_config_data.rlc_id = ctrl_blk->ctrl.um_ctrl.rlc_id;
        event_tdscdma_rlc_config_data.direction_and_rlc_type = TDSDOWNLINK_FIXED_SIZE;
        event_tdscdma_rlc_config_data.size = (uint16)rlc_size;
        break;
                          
      case UE_MODE_ACKNOWLEDGED:
      case UE_MODE_ACKNOWLEDGED_DATA:
        event_tdscdma_rlc_config_data.rlc_id = ctrl_blk->ctrl.am_ctrl.am_common->dl_data_id;
#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
        if (ctrl_blk->ctrl.am_ctrl.dl_ctl_cnfg->am_pdu_size_flexbile == TRUE)
        {
          event_tdscdma_rlc_config_data.direction_and_rlc_type = TDSDOWNLINK_FLEXIBLE_SIZE;
          event_tdscdma_rlc_config_data.size =
            ( ctrl_blk->ctrl.am_ctrl.dl_ctl_cnfg->am_li_type == TDSLI_7_BITS)?7:15;
        }
        else
#endif
        {
          event_tdscdma_rlc_config_data.direction_and_rlc_type = TDSDOWNLINK_FIXED_SIZE;
          event_tdscdma_rlc_config_data.size = (uint16)ctrl_blk->ctrl.am_ctrl.dl_ctl_cnfg->dl_rlc_size;
        }


        break;
      default:
        TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC_ERR:Invalid rlc mode for rlc config event %d",ctrl_blk->lc_mode);
        return;
    }
    TDSRLC_MSG3(MSG_LEGACY_HIGH,"rlc config event rlc id %d direction and rlc type %d size %d",
             event_tdscdma_rlc_config_data.rlc_id,
             event_tdscdma_rlc_config_data.direction_and_rlc_type,
             event_tdscdma_rlc_config_data.size); 
    event_report_payload ( EVENT_TDSCDMA_RLC_CONFIG, 4, &event_tdscdma_rlc_config_data);
  }
}
/*===========================================================================

FUNCTION TDSRLC_ERROR_LOG_SUBMIT

DESCRIPTION
  Function used to log the following RLC error conditions described in 
  section 11.3.4 of 25.322

PARAMETERS  
1. dl_rlc_data_id: Indicates DL rlc logical channel ID; Value starts from > 16 for AM channels
2. rlc_sn: Indicates the erroneous PDU just ignored by UE RLC; Range "0 - 4095" 
           and value of "0xFFFF" indicates that the SN is not-applicable for this
           error condition
3. rlc_err_reason: The below table gives the mapping of the error number 
to error description:
    0. PDU outside window
    1. Timer Discard timeout
    2. Invalid length
    3. PDU duplicates
    4. Buffer full
    5. Invalid size
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void tdsrlc_error_log_submit(rlc_lc_id_type dl_rlc_data_id, uint16 rlc_sn, 
                          tdsrlc_err_reason_type rlc_err_reason)
{
  /* logging code to be added here */
  
}


#endif


/*===========================================================================

FUNCTION TDSRLCI_RESET_STATUS_TIMER_CMD

DESCRIPTION
  This function would process a RLC AM RESET status timer command given to 
  L2 UL task to perform timer from R99-ISR.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  tdsrlci_reset_status_timer_cmd()
{
  tdsrlci_dl_ctrl_blk_type *rlc_dl_ctl_blk_ptr;
  uint16 i;

  TDSL2_ACQ_DL_LOCK();

  for (i=0; i<TDSUE_MAX_AM_ENTITY; i++)
  {
    if (tdsrlc_rst_satus_tmr_tbl[i] != TDSINIT_CTL_BLK_ID)
    {
      rlc_dl_ctl_blk_ptr = tdsrlci_dl_get_ctl_blk_ptr(tdsrlc_rst_satus_tmr_tbl[i]);
      
      if(NULL == rlc_dl_ctl_blk_ptr)
      {
        TDSRLC_MSG1(MSG_LEGACY_ERROR,"RLC-UL LC Id:%d not found",i);
        continue;
      }
      tdsrlc_rst_satus_tmr_tbl[i] = TDSINIT_CTL_BLK_ID;

    /*---------------------------------------------------------------------
      If Status prohibit timer is active for the channel, re-start the
      timer.
    ---------------------------------------------------------------------*/
      if(rlc_dl_ctl_blk_ptr->ctrl.am_ctrl.dl_state_var.status_prohibit)
      {
        rlc_dl_ctl_blk_ptr->ctrl.am_ctrl.dl_state_var.status_prohibit_cnt = 0;
      }
    }
  }
  
  TDSL2_ACQ_DL_UNLOCK();
}
/*===========================================================================

FUNCTION TDSRLC_CHECK_PENDING_DL_SDU

DESCRIPTION
  Checks if there are any PDUs yet to be received or processed
  for a complete SDU.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - if more PDUs are expected for a SDU
  FALSE - otherwise 
  
SIDE EFFECTS
  None.

===========================================================================*/

boolean tdsrlc_check_pending_dl_sdu
(
  rlc_lc_id_type lc_id
)
{
  tdsrlci_dl_ctrl_blk_type   *ctl_blk_ptr = NULL;
  tdsrlci_dl_am_state_type   *dl_state_ptr = NULL;
  uint16 index = 0;

  if((TDSINIT_CTL_BLK_ID != tdsdl_ctl_blk_id_tbl[lc_id]) && 
      ((ctl_blk_ptr = tdsget_am_ctl_blk(lc_id)) == NULL))
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"UL LC ID %d not present",lc_id);
    return FALSE;
  }
  else if (TDSINIT_CTL_BLK_ID == tdsdl_ctl_blk_id_tbl[lc_id])
  {
    TDSRLC_MSG1(MSG_LEGACY_ERROR,"UL LC ID %d is TDSINIT_CTL_BLK_ID",lc_id);
    return FALSE;
  }
  if(NULL == ctl_blk_ptr)
  {
  	TDSRLC_MSG0(MSG_LEGACY_ERROR,"ctl_blk_ptr is NULL");
    return FALSE;
  }

  if (UE_MODE_ACKNOWLEDGED == ctl_blk_ptr->lc_mode)
  {
    dl_state_ptr = &(ctl_blk_ptr->ctrl.am_ctrl.dl_state_var);
	
    index = TDSRLCI_GET_RESEQ_Q_IDX(dl_state_ptr->old_vr_r);

    if ((ctl_blk_ptr->ctrl.am_ctrl.dl_reseq_q->pdu_info[index].dsm_ptr != NULL) || 
        (ctl_blk_ptr->ctrl.am_ctrl.dl_reseq_q->partial_sdu_ptr != NULL) || 
        (dl_state_ptr->vr_r != dl_state_ptr->vr_h))
    { 
      TDSRLC_MSG3(MSG_LEGACY_HIGH,"pending msg on AM DL old_vr_r|vr_r 0x%x, vr_h 0x%x, partial_sdu_ptr 0x%x",
          ((dl_state_ptr->old_vr_r << 16)|dl_state_ptr->vr_r),dl_state_ptr->vr_h,
             ctl_blk_ptr->ctrl.am_ctrl.dl_reseq_q->partial_sdu_ptr);
      return TRUE;
    }
  }
  else if ((UE_MODE_UNACKNOWLEDGED == ctl_blk_ptr->lc_mode) &&
     (NULL != tdsdl_um_pdu_info.dsm_ptr))
  {
    TDSRLC_MSG2(MSG_LEGACY_HIGH,"pending msg on UM DL tdsdl_um_pdu_info.dsm_ptr 0x%x,lc_id %d",tdsdl_um_pdu_info.dsm_ptr,lc_id);
    return TRUE;
  }
  else
  {
    TDSRLC_MSG2(MSG_LEGACY_HIGH,"LC id %d,LC mode %d",lc_id,ctl_blk_ptr->lc_mode);
  }
  return(FALSE);
}

