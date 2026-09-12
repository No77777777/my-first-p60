/*****************************************************************************
***
*** TITLE
***
***  GPRS RLC UPLINK MODULE
***
***
*** DESCRIPTION
***
***  This module contains the state machine, the event handler and initialisation
***  functions for the GPRS RLC uplink module
***
***
*** EXTERNALIZED FUNCTIONS
***
***  void grlc_ul_one_time_init ( gas_id_t gas_id )
***  void grlc_ul_init ( gas_id_t gas_id )
***
***  void grlc_ul_handle_no_ps_access (gas_id_t gas_id, delete_pdu_cause_t flag)
***  void grlc_ul_handle_internal_sig (void)
***  void grlc_ul_handle_pl1_sig (void)
***  void grlc_ul_handle_mac_sig (void)
***  void grlc_ul_handle_llc_tmp_pdu_sig (void)
***  void grlc_ul_handle_llc_pdu_sig (void)
***  void grlc_ul_handle_llc_msg_sig (void)
***  void grlc_ul_handle_tmr_sig (void)
***
*** INITIALIZATION AND SEQUENCING REQUIREMENTS
***
***  void grlc_ul_one_time_init (void)
***  void grlc_ul_init (void)
***
*** Copyright (c) 2001-2017 Qualcomm Technologies, Inc.
*** All Rights Reserved. QUALCOMM Proprietary.
***
*** Export of this technology or software is regulated by the U.S.
*** Government. Diversion contrary to U.S. law prohibited.
***
*****************************************************************************/


/*****************************************************************************
***
***                        EDIT HISTORY FOR MODULE
***
***  This section contains comments describing changes made to the module.
***  Notice that changes are listed in reverse chronological order.
***
*** $Header: //components/rel/geran.mpss/7.4.0/grlc/src/grlcul.c#5 $
***
*** when       who     what, where, why
*** --------   ---     ------------------------------------------------------
*** 03/28/01    hv     Created
*** 07/09/01    hv     mods only send PH_DATA_REQ on new TBF or after
***                    coding scheme change since L1 is so periodic in its
***                    nature that it can check for itself instead of being
***                    told when data is available.
*** 09/18/01    hv     Debugged and added test code.
*** 11/21/01    hv     Removed task lock around timer callback fn. Modified
***                    handling of DSM items in unack mode to allow correct
***                    freeing of DSMs. Used ERR_FATAL for signal without msg
***                    errors.
*** 08/03/04    hv     Added EGPRS code
*** 05/07/11    hv     Added promoting sig PDUs to the front of uplink queue
*** 09/19/12    hv     Added dual data space functionality
*** 26/10/12    hv     CR409255 - corruption(s) of RLC uplink FIFO
*** 10/04/13    hv     CR464591 - reset caused by IARs and Contention-res failures
*** 10/04/13    hv     CR472248 - Issue with reallocation in change of RLC mode
*** 01/29/13    hv     CR447071 - RLC per-tbf memory allocation
*** 02/05/13    hv     CR481731 - fixed gas-id prints
*** 18/07/13    hv     CR421583
*** 20/08/13    hv     CR531564
*** 04/10/13    ra     CR552875 - tlli_cs not updated in one phase mode
*** 08/11/13    hv     CR561807
*****************************************************************************/


/*****************************************************************************
***
***     Included Files
***
*****************************************************************************/

#include "geran_variation.h"
#include "customer.h" /* This include must be first before #ifdef FEATURE_  */

#ifdef DEBUG_GSM_GPRS_DS_UNIT_TEST
#error code not present
#endif /* ifdef DEBUG_GSM_GPRS_DS_UNIT_TEST  */

#include "comdef.h"
#include "gmutex.h"
#include "gprsdef.h"
#include "gprsdef_g.h"
#include "grlcmaccmn.h"
#include "grlci.h"
#include "grlctsk.h"
#include "geran_msgs.h"
#include "grlcutil.h"
#include "rex.h"
#include "queue.h"
#include "gllc.h"
#ifndef DEBUG_GSM_GPRS_DS_UNIT_TEST
#include "gllc_v.h"
#include "gllc_v_g.h"
#endif /* ifdef DEBUG_GSM_GPRS_DS_UNIT_TEST  */
#include "grlcultypes.h"
#include "grlcultst.h"
#include "grlctx.h"
#include "grlcpaq.h"
#include "grlcsgm.h"
#include "grlcul.h"
#include "grlculproc.h"
#include "grlculpdu.h"
#ifdef FEATURE_GSM_DTM
#include "gttp.h"
#endif /* FEATURE_GSM_DTM */

#include "geran_eng_mode_info.h"
#include "geran_tasks.h"
#include "geran_dual_sim_g.h"
#include "mcfg_nv_api.h"

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)
#include "qsh_ext.h"
#include "grlc_qsh_ext.h"
#include "geran_nv.h"
#endif /* FEATURE_QSH_DUMP || FEATURE_QSH_EVENT_METRIC */

#if defined(TEST_FRAMEWORK)
#error code not present
#endif // TEST_FRAMEWORK

/*****************************************************************************
***
***     Private Constants & Defines
***
*****************************************************************************/

/*****************************************************************************
***
***     Private Typedefs
***
*****************************************************************************/

/* RLC UL private data types and definitions are defined in gRLCulTypes.h
*/


/*****************************************************************************
***
***     Public Data
***
*****************************************************************************/


/*-----------------------------------------------------------------
** Critical sections to protect GRLC UL message and variable access
**-----------------------------------------------------------------
*/

rex_crit_sect_type grlc_ul_msg_crit_sec[NUM_GERAN_DATA_SPACES];
rex_crit_sect_type grlc_ul_no_data_crit_sec[NUM_GERAN_DATA_SPACES];
rex_crit_sect_type grlc_ul_tx_vars_crit_sec[NUM_GERAN_DATA_SPACES];
rex_crit_sect_type grlc_ul_procedure_crit_sec[NUM_GERAN_DATA_SPACES];


/*****************************************************************************
***
***     Private Data Declarations
***
*****************************************************************************/

/*****************************************************************************
***
***     Private Function Prototypes
***
*****************************************************************************/
static void grlc_ul_panic_reset (gas_id_t gas_id);

/*****************************************************************************
***
***     Public Data Declarations
***
***     Ensure an external reference is added to the module's header file!
***
*****************************************************************************/

#ifdef FEATURE_QSH_DUMP

/* Define a structure with QSH dump header and all TLV structures */
typedef struct
{
  qsh_dump_hdr_s hdr;
  grlcul_qsh_dump_tag_data_s grlcul_qsh_dump_tag_data;
  grlcul_qsh_dump_tag_data_s grlcul_qsh_dump_tag_data_2;
} grlcul_qsh_tc_dump_collect_s;

/* Pointer declared as type of QSH data UL structure for parser */
grlcul_qsh_dump_tag_data_s  *grlcul_qsh_dump_tag_0_ptr;
grlcul_qsh_dump_tag_data_s  *grlcul_qsh_dump_tag_1_ptr;

#endif /* FEATURE_QSH_DUMP */

#ifdef FEATURE_QSH_EVENT_METRIC

#define GRLC_QSH_METRIC_UL_COUNT   2
#define GRLC_UL_QSH_NUM_TRACE_MSGS 50

/* Array of pointers to metric buffers */
qsh_ext_metric_cfg_s grlcul_qsh_metric_cfg_arr[GRLC_QSH_METRIC_UL_COUNT];

__attribute__((section(".uncompressible.text")))
void grlc_ul_qsh_metric_report (gas_id_t gas_id);

#endif /* FEATURE_QSH_EVENT_METRIC */


#ifdef DEBUG_GSM_GPRS_DS_UNIT_TEST
#error code not present
#endif /* DEBUG_GSM_GPRS_DS_UNIT_TEST */

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR static void grlcul_qsh_mdump_collect_data(qsh_client_cb_params_s *param);
#endif

/*****************************************************************************
***
***     Private Functions
***
*****************************************************************************/
/*===========================================================================
===
===  FUNCTION      GRLC_UL_CREATE_TX_ARRAY()
===
===  DESCRIPTION
===
===    Allocate memory for the transmit array
===
===  DEPENDENCIES
===
===  PARAM
===    action_on_ps_access_t:  action
===      NO_CELL_CHANGE_NO_PURGE   = 0x0,   no cell reselection has happened between no
===                                         service to service
===      CELL_CHANGE_PURGE_GMM_SIG = 0x1,   cell reselection has happened between no
===                                         service to service
===      NO_CELL_CHANGE_PURGE_GMM_SIG = 0x2 no cell reselection has happened but grlc needs to delete
===                                         gmm messages.*Special case for T3142 AND T3172 timers
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
/* Host env can not allocate more than 32500 bytes so static memory is used
*/
#ifdef DEBUG_GSM_EGPRS_DS_UNIT_TEST
#error code not present
#endif

static void grlc_ul_create_tx_array (gas_id_t gas_id, uint16 tx_workspace)
{
  if (NULL != p_tx_arr[gas_id])
  {
    MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1037, "GRLU one-time p_ta_arr not NULL");
  }
  else
  {
    uint32 size = sizeof(grlc_ul_tx_array_t)*tx_workspace;
    MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1038, "TX_ARR size = %lu", size);

#ifdef DEBUG_GSM_EGPRS_DS_UNIT_TEST
    #error code not present
#else
    p_tx_arr[gas_id] = (grlc_ul_tx_array_t (*)[]) (GPRS_MEM_MALLOC(size));
#endif

    if (NULL != p_tx_arr[gas_id])
    {
      MSG_GRLC_HIGH_0_G(GRLC_F3_MSG_ENUM_1039, "TX_ARR allocated OK");

      /* Clear all elements */
      (void)memset((*p_tx_arr[gas_id]), 0, size);
    }
    else
    {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN, QSH_CLT_GRLCUL, QSH_CLT_GRLCUL)
#endif
      ERR_GERAN_FATAL_0_G("ERLU Failed to allocate TX_ARR");
    }
  }
} /* grlc_ul_create_tx_array */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_PS_ACCESS()
===
===  DESCRIPTION
===
===    Set access sub-state to enable and load PDU and make uplink TBF establishment
===    request. If action =  CELL_CHANGE_PURGE_GMM_SIG, clear N3102 NW variables
===
===  DEPENDENCIES
===
===  PARAM
===    action_on_ps_access_t:  action
===      NO_CELL_CHANGE_NO_PURGE   = 0x0,   no cell reselection has happened between no
===                                         service to service
===      CELL_CHANGE_PURGE_GMM_SIG = 0x1,   cell reselection has happened between no
===                                         service to service
===      NO_CELL_CHANGE_PURGE_GMM_SIG = 0x2 no cell reselection has happened but grlc needs to delete
===                                         gmm messages.*Special case for T3142 AND T3172 timers
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_handle_ps_access (gas_id_t gas_id, action_on_ps_access_t action)
{
  rlc_mac_msg_t   *rlc_msg_ptr;

  if ( ul[gas_id].b_ul_pdu_delete_after_stale_tmr_expiry == TRUE )
  {
    gllc_grr_ul_resume_ind(gas_id);
    MSG_GRLC_HIGH_0_G(GRLC_F3_MSG_ENUM_1040, "GRLU - UL Resumed");
  }

  /* PS_ACCESS indicated by MAC. If running, stop Stale PDU Timer now.
  */
  TIMER_STOP_STALE_PDU_DELETE(gas_id);

  /* PS_ACCESS indicated by MAC. Set ul_pdu_delete flag to FALSE.
  */
  ul[gas_id].b_ul_pdu_delete_after_stale_tmr_expiry = FALSE;


  ul[gas_id].b_access = TRUE;

  /* Refresh sysinfo regardless of cell change status
  */
  read_ul_sysinfo(gas_id);

  MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1041, "GRLU S=%u action on ps_access=%u BS_CV_MAX=%u",
            ul[gas_id].state,
            action,
            sysinfo[gas_id].bs_cv_max);

  if (action == CELL_CHANGE_PURGE_GMM_SIG)
  {
    SET_N3102(gas_id);

    /* Delete GMM_SIGNALLING PDUs on new cell to ensure RAUs are deleted
    ** Indicate to NAS so it can perform retry without waiting for due timer to expire.
    */
    grlc_ul_del_signalling_pdus(gas_id, NAS_INDICATION);
  }
  else if (action == NO_CELL_CHANGE_PURGE_GMM_SIG)
  {
    /* Delete GMM_SIGNALLING PDUs to ensure RAUs are deleted
    ** and indicate to NAS of the deletion so it can perform retry
    ** without waiting for T3330 expiry.
    */
    grlc_ul_del_signalling_pdus(gas_id, NAS_INDICATION );
  }

  /* reset the cause as we have ps_access now
  */
  ul[gas_id].no_ps_access_del_pdu_cause = NO_DEL;

  RESET_N3104(gas_id);

  /* Indicate PS access confirm to MAC before proceeding. */
  /* pointer to message buffer to send to MAC
  */

  /*-------------------------------------------------------*/

  rlc_msg_ptr = mac_get_cmd_buf_rlc();
  if (rlc_msg_ptr != NULL)
  {
    rlc_msg_ptr->msg.ps_access_cnf.sig = RM_PS_ACCESS_CNF;
    rlc_msg_ptr->msg.ps_access_cnf.gas_id = gas_id;
    mac_put_cmd_rlc(gas_id, rlc_msg_ptr);
  }
  else
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN, QSH_CLT_GRLCUL, QSH_CLT_GRLCUL)
#endif
    ERR_GERAN_FATAL_0_G("ERLU Failed to acquire MAC command buffer");
  }

  (void)load_cur_pdu(gas_id);
  est_tbf_from_null(gas_id);

} /* grlc_ul_handle_ps_access */


/*===========================================================================
===
===  FUNCTION      INDICATE_UL_RELEASE()
===
===  DESCRIPTION
===
===    Send an uplink release indication to MAC with a release cause.
===
===  DEPENDENCIES
===
===    None
===
===  PARAMS
===
===    release cause of type rm_tbf_release_t
===
===  RETURN VALUE
===
===    None
===
===  SIDE EFFECTS
===
===    None
===
============================================================================*/
void indicate_ul_release ( gas_id_t gas_id, rm_tbf_release_t rel_cause )
{
  /* pointer to message buffer to send to MAC
  */
  rlc_mac_msg_t   *rlc_msg_ptr;

  /*-------------------------------------------------------*/

  rlc_msg_ptr = mac_get_cmd_buf_rlc();
  if (rlc_msg_ptr != NULL)
  {
    rlc_msg_ptr->msg.ul_release_ind.sig = RM_UL_RELEASE_IND;
    rlc_msg_ptr->msg.ul_release_ind.gas_id = gas_id;
    rlc_msg_ptr->msg.ul_release_ind.cause = rel_cause;
    mac_put_cmd_rlc(gas_id, rlc_msg_ptr);
  }
  else
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN, QSH_CLT_GRLCUL, QSH_CLT_GRLCUL)
#endif
    ERR_GERAN_FATAL_0_G("ERLU GRLU heap exhaustion");
  }
} /* indicate_ul_release */


/*===========================================================================
===
===  FUNCTION      GRLC_UL_TIMER_CALLBACK()
===
===  DESCRIPTION
===
===    Handles callback actions for all rlc ul timers. If the timer is of
===    the general purpose type then it creates a timer message with the
===    signal as passed in the function parameter and send the message to
===    rlc ul task
===
===  DEPENDENCIES
===
===    ul timer queue is initialised and ready to accept queue item.
===
===  RETURN VALUE
===
===    none
===
===  SIDE EFFECTS
===
===    none
===
===========================================================================*/
static void grlc_ul_timer_callback (uint32  timer_param)
{
  /* to hold timer message to send to rlc ul task
  */
  rm_timer_msg_t   *timer_msg_ptr;
  rm_timer_t        timer_t = RM_TMR_STALE_PDU_DELETE;

  /*---------------------------------------------------*/

  /*---------------------------------------------------------------
  ** The timers expected are:
  **      RM_TMR_T3182_AWAIT_UL_ACKNACK,
  **      RM_TMR_AWAIT_UL_ACKNACK,
  **      RM_TMR_AWAIT_MAC_UL_RESP,
  **      RM_TMR_AWAIT_ASS_GUARD
  **
  ** send RLC UL task timer signal as specified in timer_param
  ** The signal contains only the timer id.
  **---------------------------------------------------------------
  */

  /* Get memeory for the command
  */
  timer_msg_ptr = GPRS_MEM_MALLOC(sizeof(rm_timer_msg_t));

  if (timer_msg_ptr == NULL)
  {
    /* Failed to create timer message
    */
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN, QSH_CLT_GRLCUL, QSH_CLT_GRLCUL)
#endif
    ERR_GERAN_FATAL_0("GRLC UL malloc failure");
  }
  else
  {
    gas_id_t  gas_id = (gas_id_t)TIMER_GET_DATA_SPACE_INDEX(timer_param);

    /* the timer id becomes the signal id
    */
    timer_msg_ptr->sig = (rm_timer_t)TIMER_GET_ID(timer_param);
    timer_t = timer_msg_ptr->sig;

    GRLC_UL_MSG_LOCK(gas_id);

    /* init q item by pointing to data area
    */
    (void)q_link(timer_msg_ptr, &timer_msg_ptr->link);

    /* add command to rlc ul queue
    */
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
    q_put(&UTQ(gas_id), &timer_msg_ptr->link);

    /* set signal mask as the final action in posting the message
    */
    RLC_UL_SET_TMR_SIG(gas_id);

    GRLC_UL_MSG_UNLOCK(gas_id);

    /* Log event timer expiry 
    */
    grlc_log_timer_event(gas_id, timer_t);
  }
} /* grlc_ul_timer_callback */


/*===========================================================================
===
===  FUNCTION      grlc_ul_handle_alloc_cnf_await_ass()
===
===  DESCRIPTION
===
===    Handles alloc_cnf in US_AWAIT_ASS and in US_XFER in PSHO
===
===  DEPENDENCIES
===
===
===
===  RETURN VALUE
===
===    none
===
===  SIDE EFFECTS
===
===    none
===
===========================================================================*/
static void grlc_ul_handle_alloc_cnf_await_ass(gas_id_t gas_id)
{
  /* Show UL FIFO contents, then re-init FIFO
  */
  MSG_GRLC_EXT_HIGH_0_G(GRLC_F3_MSG_ENUM_1042, "GRLC UL Alloc cnf Await. Show FIFO then re-init");
  grlc_ul_show_fifo_vars(gas_id);
  grlc_ul_fifo_re_init(gas_id);

  ul[gas_id].state = (rlc_ul_state_t)((ul[gas_id].rlc_mode == GRLC_MODE_ACK) ?
                              US_ACK_XFER :
                              US_UNACK_XFER
                             );
  UPD_UL_STATE(gas_id);

  /* Accept new resource. Transfer alloc_cnf into ul[gas_id].cur_res and set
  ** RALS state
  */
  accept_resource(gas_id);

  /* Now assign the RLC mode from the current PDU. This value
  ** determines the next state
  */
  ul[gas_id].rlc_mode = cur_pdu_ptr[gas_id]->rlc_mode;

  /* just got cnf so allocation for next pdu does not exist,
  ** and segment and tx must be at NULL state.
  */

  ul[gas_id].segment_state = SEGMENT_NULL;

  /* If next pdu is not loaded already then attempt to load from queue
  */
  if (next_pdu_ptr[gas_id] == NULL)
    dequeue_pdu(gas_id, &next_pdu_ptr[gas_id]);

  /* Initialise data block according to the access phase,
  ** ie. fields are set up for one phase access if required
  ** ! check if this valid !
  */
  if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
  {
    E_NEW_DATA_BLOCK(gas_id, &cur_blk[gas_id]);
  }
  else
  {
    NEW_DATA_BLOCK(gas_id, &cur_blk[gas_id]);
  }

  /* Init PSN of the first PDU to free
  */
  ul[gas_id].first_psn_to_free = cur_pdu_ptr[gas_id]->psn;
  ul[gas_id].pdu_freeing_state = NONE_TO_FREE;
  ul[gas_id].psn_to_free_inclusive = ul[gas_id].first_psn_to_free;

  /* cur data block and cur pdu are ready for segmentation
  ** and transmit. Call appropriate ack or unack mode xfer
  ** function
  */
  if (ul[gas_id].rlc_mode == GRLC_MODE_ACK)
  {
    /* we are in await assignment and not xfer state but
    ** we are about to process invoke ack_mode_xfer
    ** function. To prepare we will change our state
    ** to US_ACK_XFER before invoking the function
    */
    ul[gas_id].state = US_ACK_XFER;
    UPD_UL_STATE(gas_id);

    /* Set uo flag to send PH_DATA_REQ to L1 on the
    ** first data block of this TBF
    */
    ul[gas_id].b_first_fifo_load = TRUE;

    if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
    {
      e_ack_mode_xfer(gas_id);
    }
    else
    {
      ack_mode_xfer(gas_id);
    }
  }
  else
  {
    /* we are in await assignment and not xfer state but
    ** we are about to invoke unack_mode_xfer function.
    ** To prepare we will change our state to US_ACK_XFER
    ** before invoking the function
    */
    ul[gas_id].state = US_UNACK_XFER;
    UPD_UL_STATE(gas_id);

    /* ! set uo flag to send PH_DATA_REQ to L1 on the
    ** first data block of this TBF
    */
    ul[gas_id].b_first_fifo_load = TRUE;

    if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
    {
      e_unack_mode_xfer(gas_id);
    }
    else
    {
      unack_mode_xfer(gas_id);
    }
  }
#ifdef  FEATURE_GPRS_PS_HANDOVER
  ul[gas_id].b_psho_llc_pdu_absent = FALSE;
#endif /* FEATURE_GPRS_PS_HANDOVER */
}/*grlc_ul_handle_alloc_cnf_await_ass */

/*===========================================================================
===
===  FUNCTION      UPDATE_TLLI_CS
===
===  DESCRIPTION
===
===  To assign tilli_cs the coding scheme in one-phase access mode.
===
===  DEPENDENCIES
===
===    None
===
===  RETURN VALUE
===
===    None
===
===  SIDE EFFECTS
===
===    None
===
============================================================================*/
static void update_tlli_cs(gas_id_t gas_id)
{
  /* In one-phase access mode, ul[gas_id].tlli_cs should be assigned the coding scheme
  ** as follows:
  ** if alloc_cnf.tlli_block_cs = 0, tlli_cs = RM_CS_1/RM_MCS_1
  ** if alloc_cnf.tlli_block_cs = 1, tlli_cs = alloc_cnf.cs/alloc_cnf.mcs
  */
  if (ul[gas_id].cur_res.ti == RM_TLLI_PRESENT)
  {
    if (ul[gas_id].cur_res.tlli_block_cs == RM_TLLI_USE_CS_1)
      ul[gas_id].tlli_cs = RM_CS_1; /* RM_CS_1 == RM_MCS_1*/
    else
      ul[gas_id].tlli_cs = ul[gas_id].cur_res.cs;
  }
}/* update_tlli_cs */

/*===========================================================================
===
===  FUNCTION      grlc_ul_handle_alloc_cnf_ack_xfer()
===
===  DESCRIPTION
===
===    Handles alloc_cnf in US_ACK_XFER
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===    none
===
===  SIDE EFFECTS
===
===    none
===
===========================================================================*/
static void grlc_ul_handle_alloc_cnf_ack_xfer
(
  gas_id_t                gas_id,
  const mr_ul_alloc_cnf_t *ul_alloc_cnf
)
{
  if (ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS)
  {
    grlc_ul_show_new_alloc(gas_id, &ul[gas_id].cur_res,ul_alloc_cnf);
    MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1043, "ERLU A ALLOC_CNF #TS=%d mcs=%d typ=%d",
                  ul_alloc_cnf->num_timeslots,
                  ul_alloc_cnf->mcs,
                  ul_alloc_cnf->tbf_type);

    /* only if it's active
    */
    TIMER_STOP_MAC_UL_RESP(gas_id);

    /* Update coding scheme for engg_mode
    */
    if (ul[gas_id].cur_res.cs != ul_alloc_cnf->mcs)
    {
      geran_eng_mode_data_write(ENG_MODE_EGPRS_UL_MCS, (void*)(&ul_alloc_cnf->mcs), TRUE, gas_id);
    }

    /* Check and handle change of MCS/TFI/num_timeslots
    */
    if (ul[gas_id].cur_res.cs != ul_alloc_cnf->mcs                      ||
        ul[gas_id].cur_res.num_timeslots != ul_alloc_cnf->num_timeslots ||
        ul[gas_id].cur_res.ul_tfi != ul_alloc_cnf->ul_tfi
       )
    {
      MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1044, "ERLU chMCS/TFI/TS ul[gas_id].mcs=%u alloc_cnf.mcs=%u",
                ul[gas_id].cur_res.cs,
                ul_alloc_cnf->mcs,
                0
             );
      ul[gas_id].cur_res = *ul_alloc_cnf;
      ul[gas_id].cur_res.cs =  ul_alloc_cnf->mcs;

      /* Assign tilli_cs the coding scheme in one-phase access mode.
      */
      update_tlli_cs(gas_id);

      /* Freeze FIFO access for reprocessing
      */
      grlc_ul_fifo_disable_pl1_access(gas_id);

      /* parameter contains the new coding scheme
      */
      change_coding_scheme(gas_id, ul[gas_id].cur_res.cs);

      /* Re-enable PL1 access to the FIFO
      */
      grlc_ul_fifo_enable_pl1_access(gas_id);

      /* update the working coding scheme
      */
      ul[gas_id].last_valid_mcs = ul[gas_id].cur_res.cs;
    }
    else
    {
      /* For PSHO , target cell may support USF granularity
      */
      ul[gas_id].cur_res.usf_granularity = ul_alloc_cnf->usf_granularity;

     /* Enable PL1 access to FIFO (for PSHO)
      */
      grlc_ul_fifo_enable_pl1_access(gas_id);

      /* Even though there is nothing to do from an RLC perspective because
      ** the coding scheme hasn't actually changed, GPL1 will be expecting
      ** a PH_DATA_REQ to unblock the UL FIFO.
      */
      pl1_send_ph_data_req(gas_id);
    }

    if (ul[gas_id].cur_res.ws < ul_alloc_cnf->ws)
    {
      /* Since WS can't be downsized, the new WS has to be larger. So check
      ** if the current window is stalled and if so un-stall it
      */
      if (ul[gas_id].si == RM_TX_STALLED)
      {
        ul[gas_id].si = RM_TX_NOT_STALLED;
        GRLC_UL_SHOW_TX_VARS(gas_id);

        /* No re-processing aprt from re-enabling PL1
        */
        ul[gas_id].b_first_fifo_load = TRUE;
        grlc_ul_fifo_enable_pl1_access(gas_id);
        pl1_send_ph_data_req(gas_id);
      }

      ul[gas_id].cur_res.ws = ul_alloc_cnf->ws;

      /* WS has widened. Stop T3182
      */
      TIMER_STOP_T3182(gas_id);
    }
  }
  else  /* EGPRS build GPRS mode */
  {
    grlc_ul_show_new_alloc(gas_id, &ul[gas_id].cur_res, ul_alloc_cnf);
    MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1045, "GRLU A ALLOC_CNF #TS=%d cs=%d typ=%d",
              ul_alloc_cnf->num_timeslots,
              ul_alloc_cnf->cs,
              ul_alloc_cnf->tbf_type);

    /* only if it's active
    */
    TIMER_STOP_MAC_UL_RESP(gas_id);

    /* Update coding scheme for engg_mode
    */
    if (ul[gas_id].cur_res.cs != ul_alloc_cnf->cs)
    {
      geran_eng_mode_data_write(ENG_MODE_GPRS_UL_CS, (void*)(&ul_alloc_cnf->cs), TRUE, gas_id);
    }

    /* check and handle change of coding scheme
    */
    if (ul[gas_id].cur_res.cs != ul_alloc_cnf->cs                       ||
        ul[gas_id].cur_res.num_timeslots != ul_alloc_cnf->num_timeslots ||
        ul[gas_id].cur_res.ul_tfi != ul_alloc_cnf->ul_tfi
       )
    {
      ul[gas_id].cur_res = *ul_alloc_cnf;
      MSG_GRLC_EXT_MED_2_G(GRLC_F3_MSG_ENUM_1046, "GRLU chCS/TFI/TS cs=%d new cs=%d",
              ul[gas_id].cur_res.cs,
              ul_alloc_cnf->cs
              );

      /* Assign tilli_cs the coding scheme in one-phase access mode.
      */
      update_tlli_cs(gas_id);

      /* Freeze FIFO access for reprocessing
      */
      grlc_ul_fifo_disable_pl1_access(gas_id);

      /* parameter contains the new coding scheme
      */
      change_coding_scheme(gas_id, ul_alloc_cnf->cs);

      /* Re-enable PL1 access to the FIFO
      */
      grlc_ul_fifo_enable_pl1_access(gas_id);

      /* update the working coding scheme
      */
      ul[gas_id].last_valid_coding_scheme = ul[gas_id].cur_res.cs;
    }
    else
    {
      /* For PSHO , target cell may support USF granularity
      */
      ul[gas_id].cur_res.usf_granularity = ul_alloc_cnf->usf_granularity;

     /* Enable PL1 access to FIFO (for PSHO)
      */
      grlc_ul_fifo_enable_pl1_access(gas_id);

      /* Even though there is nothing to do from an RLC perspective because
      ** the coding scheme hasn't actually changed, GPL1 will be expecting
      ** a PH_DATA_REQ to unblock the UL FIFO.
      */
      pl1_send_ph_data_req(gas_id);
    }
  }
} /* grlc_ul_handle_alloc_cnf_ack_xfer() */

/*===========================================================================
===
===  FUNCTION      grlc_ul_handle_alloc_cnf_unack_xfer()
===
===  DESCRIPTION
===
===    Handles alloc_cnf in US_UNACK_XFER
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===    none
===
===  SIDE EFFECTS
===
===    none
===
===========================================================================*/

static void grlc_ul_handle_alloc_cnf_unack_xfer
(
  gas_id_t                gas_id,
  const mr_ul_alloc_cnf_t *ul_alloc_cnf
)
{
  if (ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS)
  {
    grlc_ul_show_new_alloc(gas_id, &ul[gas_id].cur_res,ul_alloc_cnf);
    MSG_GRLC_EXT_MED_2_G(GRLC_F3_MSG_ENUM_1047, "GRLU U UnsolAss old TS=%d new TS=%d",
              ul[gas_id].cur_res.num_timeslots,
              ul_alloc_cnf->num_timeslots
              );

    /* Update coding scheme for engg_mode
    */
    if (ul[gas_id].cur_res.cs != ul_alloc_cnf->mcs)
    {
      geran_eng_mode_data_write(ENG_MODE_EGPRS_UL_MCS, (void*)(&ul_alloc_cnf->mcs), TRUE, gas_id);
    }

    if (ul[gas_id].cur_res.cs != ul_alloc_cnf->mcs        ||
        ul[gas_id].cur_res.num_timeslots !=
          ul_alloc_cnf->num_timeslots             ||
        ul[gas_id].cur_res.ul_tfi != ul_alloc_cnf->ul_tfi
       )
    {
      MSG_GRLC_EXT_MED_2_G(GRLC_F3_MSG_ENUM_1048, "ERLU U chMCS/TFI/TS mcs=%u new mcs=%u",
              ul[gas_id].cur_res.cs,
              ul_alloc_cnf->mcs
              );
      ul[gas_id].cur_res = *ul_alloc_cnf;
      ul[gas_id].cur_res.cs =  ul_alloc_cnf->mcs;

      /* Assign tilli_cs the coding scheme in one-phase access mode.
      */
      update_tlli_cs(gas_id);

      /* Freeze FIFO access for reprocessing
      */
      grlc_ul_fifo_disable_pl1_access(gas_id);

      /* parameter contains the new coding scheme
      */
      change_coding_scheme(gas_id, ul[gas_id].cur_res.cs);

      /* Re-enable PL1 access to the FIFO
      */
      grlc_ul_fifo_enable_pl1_access(gas_id);

      /* update the working coding scheme
      */
      ul[gas_id].last_valid_mcs = ul[gas_id].cur_res.cs;
    }
    else
    {
      /* For PSHO , target cell may support USF granularity
      */
      ul[gas_id].cur_res.usf_granularity = ul_alloc_cnf->usf_granularity;

      /* Enable PL1 access to FIFO (for PSHO)
      */
      grlc_ul_fifo_enable_pl1_access(gas_id);

      /* Even though there is nothing to do from an RLC perspective because
      ** the coding scheme hasn't actually changed, GPL1 will be expecting
      ** a PH_DATA_REQ to unblock the UL FIFO.
      */
      pl1_send_ph_data_req(gas_id);
    }

    if (ul[gas_id].cur_res.ws < ul_alloc_cnf->ws)
    {
      /* Since WS can't be downsized, the new WS has to be larger. So check
      ** if the current window is stalled and if so un-stall it
      */
      if (ul[gas_id].si == RM_TX_STALLED)
      {
        ul[gas_id].si = RM_TX_NOT_STALLED;
        GRLC_UL_SHOW_TX_VARS(gas_id);

        /* No re-processing apart from re-enabling PL1
        */
        ul[gas_id].b_first_fifo_load = TRUE;
        grlc_ul_fifo_enable_pl1_access(gas_id);
        pl1_send_ph_data_req(gas_id);
      }

      ul[gas_id].cur_res.ws = ul_alloc_cnf->ws;

      /* WS has widened. Stop T3182
      */
      TIMER_STOP_T3182(gas_id);
    }
  } /* EGPRS mode */
  else
  {
    /* EGPRS build GPRS mode
    */
    grlc_ul_show_new_alloc(gas_id, &ul[gas_id].cur_res, ul_alloc_cnf);
    MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1049, "GRLU U ALLOC_CNF #TS=%d cs=%d typ=%d",
            ul_alloc_cnf->num_timeslots,
            ul_alloc_cnf->cs,
            ul_alloc_cnf->tbf_type);

    /* Update coding scheme for engg_mode
    */
    if (ul[gas_id].cur_res.cs != ul_alloc_cnf->cs)
    {
      geran_eng_mode_data_write(ENG_MODE_GPRS_UL_CS, (void*)(&ul_alloc_cnf->cs), TRUE, gas_id);
    }

    /* Check for changes in CS/TFI/CS
    */
    if (ul[gas_id].cur_res.cs != ul_alloc_cnf->cs                        ||
        ul[gas_id].cur_res.num_timeslots != ul_alloc_cnf->num_timeslots  ||
        ul[gas_id].cur_res.ul_tfi != ul_alloc_cnf->ul_tfi
       )
    {
      MSG_GRLC_EXT_MED_2_G(GRLC_F3_MSG_ENUM_1050, "GRLU U UnsolAss old TS=%d new TS=%d",
              ul[gas_id].cur_res.num_timeslots,
              ul_alloc_cnf->num_timeslots
              );
      ul[gas_id].cur_res = *ul_alloc_cnf;

      /* Assign tilli_cs the coding scheme in one-phase access mode.
      */
      update_tlli_cs(gas_id);

      /* Freeze FIFO access for reprocessing
      */
      grlc_ul_fifo_disable_pl1_access(gas_id);

      /* parameter contains the new coding scheme
      */
      change_coding_scheme(gas_id, ul[gas_id].cur_res.cs);

      /* Re-enable PL1 access to the FIFO
      */
      grlc_ul_fifo_enable_pl1_access(gas_id);
    }
    else
    {
      /* For PSHO , target cell may support USF granularity
      */
      ul[gas_id].cur_res.usf_granularity = ul_alloc_cnf->usf_granularity;

      /* Enable PL1 access to FIFO (for PSHO)
      */
      grlc_ul_fifo_enable_pl1_access(gas_id);

      /* Even though there is nothing to do from an RLC perspective because
      ** the coding scheme hasn't actually changed, GPL1 will be expecting
      ** a PH_DATA_REQ to unblock the UL FIFO.
      */
      pl1_send_ph_data_req(gas_id);
    }
  } /* EGPRS build GPRS mode */
} /* grlc_ul_handle_alloc_cnf_unack_xfer */


#ifdef  FEATURE_GPRS_PS_HANDOVER

/*===========================================================================
===
===  FUNCTION      GRLC_UL_START_XFER()
===
===  DESCRIPTION
===    Load current PDU and start segmentation. Load up UL FIFO and send
===    PH_DATA_REQ to PL1. Start transfer according to ack/unack mode and
===    TBF mode
===
===  DEPENDENCIES
===    ul struct
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_start_xfer
(
  gas_id_t      gas_id,
  mac_rlc_msg_t *mac_msg_ptr
)
{
  /* only if it's active
  */
  TIMER_STOP_MAC_UL_RESP(gas_id);

  /* Force open-ended if given close-ended
  */
  if (mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf.tbf_type == RM_CLOSE_ENDED_TBF)
  {
    mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf.tbf_type = RM_OPEN_ENDED_TBF;
    MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1051, "GRLU close-ended converted to open-ended");
  }

  (void)load_cur_pdu(gas_id);

  /* store info returned by MAC tbf est cnf.
  */
  grlc_ul_show_alloc_cnf_await_ass(gas_id, &mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf);

  if (ul[gas_id].state != US_ACK_XFER &&
      ul[gas_id].state != US_UNACK_XFER
     )
  {
    /* If the RLC_RESET field indicates that any given RLC entity is
    ** not reset across PS handover then the mobile station shall
    ** ignore this field and use the same RLC mode that was used for
    ** the corresponding PFC in the old cell.
    */
    if ( cur_pdu_ptr[gas_id] != NULL           &&
         mac_msg_ptr->msg.ul_psho_complete_ind.ul_rlc_mode != cur_pdu_ptr[gas_id]->rlc_mode
       )
    {
      MSG_GRLC_ERROR_2_G(GRLC_F3_MSG_ENUM_1052, "GRLU PSHO ##rlc_mode diff in cur_pdu rlc_mode =%u ass_rlc_mode=%u",
         cur_pdu_ptr[gas_id]->rlc_mode,
         mac_msg_ptr->msg.ul_psho_complete_ind.ul_rlc_mode);
    }

    ul[gas_id].rlc_mode = mac_msg_ptr->msg.ul_psho_complete_ind.ul_rlc_mode;

    ul[gas_id].state = (rlc_ul_state_t)((ul[gas_id].rlc_mode == GRLC_MODE_ACK) ?
                              US_ACK_XFER :
                              US_UNACK_XFER
                             );

    if (cur_pdu_ptr[gas_id] != NULL)
    {
      ul[gas_id].b_psho_llc_pdu_absent = FALSE;
      /* accept assignment from mac for a new uplink tbf
      */
      grlc_ul_handle_alloc_cnf_await_ass(gas_id);
    }
    else
    {
      ul[gas_id].b_psho_llc_pdu_absent = TRUE;
    }
  } /* US_STATE = US_NULL */
  else
  {
    /* US_STATE is XFER
    */
    if (ul[gas_id].state == US_ACK_XFER )
    {
      grlc_ul_handle_alloc_cnf_ack_xfer(gas_id, &mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf);
    }
    else
    {
      grlc_ul_handle_alloc_cnf_unack_xfer(gas_id, &mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf);
    }
  } /* US_STATE is XFER */

  /* Re-assess flow control
  */
  grlc_llc_ul_flow_control(gas_id);

} /* grlc_ul_start_xfer */

#endif /* FEATURE_GPRS_PS_HANDOVER */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_MAC_EVENT_US_NULL()
===
===  DESCRIPTION
===
===    Handler for MAC signals in US_NULL state.
===
===  DEPENDENCIES
===    ul struct
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_handle_mac_event_us_null ( gas_id_t gas_id )
{
  mac_rlc_msg_t *mac_msg_ptr;
  uint16        psn=0xffff;

  mac_msg_ptr = (mac_rlc_msg_t *)q_get(&UMQ(gas_id));

  if (mac_msg_ptr == NULL)
  {
    MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1053, "GRLU US_NULL event without msg");
  }
  else
  {
    if (mac_msg_ptr->msg.sig == MR_UL_RESET_REQ)
    {
      grlc_ul_panic_reset(gas_id);
    }
    else

    if (mac_msg_ptr->msg.sig == MR_NO_PS_ACCESS_IND)
    {
      rlc_log_ul_statistics_ext(gas_id);
      grlc_ul_handle_no_ps_access(gas_id, mac_msg_ptr->msg.no_ps_access_ind.delete_pdu_flag);
      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1054, "GRLU NULL cur_access=%u no-access", ul[gas_id].b_access);
    }
    else
    if (mac_msg_ptr->msg.sig == MR_PS_ACCESS_REQ)
    {
#ifdef FEATURE_GPRS_PS_HANDOVER
      if (ul[gas_id].b_psho_active)
      {
        MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1055, "GRLU NULL PSHO ignore access-ind in PSHO");
      }
      else
      {
        grlc_ul_handle_ps_access(gas_id, mac_msg_ptr->msg.ps_access_req.action);
        MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1056, "GRLU NULL cur_access=%u access", ul[gas_id].b_access);
      }
#else
      /* This is to prevent race condition
      */
      grlc_ul_handle_ps_access(gas_id, mac_msg_ptr->msg.ps_access_req.action);
      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1057, "GRLU NULL cur_access=%u access", ul[gas_id].b_access);
#endif /* FEATURE_GPRS_PS_HANDOVER  */
    }
#ifdef  FEATURE_GSM_DTM
    else
    if (mac_msg_ptr->msg.ul_release_ind.sig == MR_UL_RELEASE_IND &&
        mac_msg_ptr->msg.ul_release_ind.cause == REL_UL_NORMAL
       )
    {
      rlc_log_ul_statistics_ext(gas_id);
      /* Successful GTTP transfer: clear PDU, loads the next PDU.
      ** If the next PDU exists make request .. else goto US_NULL
      */
      if (cur_pdu_ptr[gas_id] != NULL)
      {
        psn = cur_pdu_ptr[gas_id]->psn;
        /* To transfer grouping info to the next PDU
        */
        ungroup_pdu(gas_id, cur_pdu_ptr[gas_id]);

        /* Successful transmission of PDU , set the flag to TRUE
        ** to start ready timer
        */
        grlc_ul_del_free_paq_upto_psn(gas_id, psn,TRUE);
      }

      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1058, "GRLU GTTP psn=%d",psn);

      clr_ul_data(gas_id);
      grlc_ul_unput_paq(gas_id);

      ul[gas_id].state = US_NULL;
      UPD_UL_STATE(gas_id);

      /* Consider the TBF released. Now load the next PDU and establish new TBF
      ** otherwise go back to NULL
      */
      (void)load_cur_pdu(gas_id);

      ul[gas_id].b_access = FALSE;
    }
#endif /*  FEATURE_GSM_DTM */
#ifdef  FEATURE_GPRS_PS_HANDOVER
    else
    if (mac_msg_ptr->msg.sig == MR_UL_PSHO_START_IND)
    {
      if (ul[gas_id].b_psho_active)
      {
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1059, "GRLU NULL PSHO_START whilst ALREADY active");
      }

      ul[gas_id].b_psho_active = TRUE;
      ul[gas_id].b_access = FALSE;
      MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1060, "GRLU NULL PSHO_START access is now FALSE");

      TIMER_START_AWAIT_ASS_GUARD(gas_id);
    }
    else
    if (mac_msg_ptr->msg.sig == MR_UL_PSHO_COMPLETE_IND)
    {
      if (!ul[gas_id].b_psho_active)
      {
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1061, "GRLU NULL PSHO_COMPLETE whilst NOT active");
      }

      TIMER_STOP_AWAIT_ASS_GUARD(gas_id);

      MSG_GRLC_MED_3_G(GRLC_F3_MSG_ENUM_1062, "GRLU A PSHO_COMPLETE succ = %u type = %u rlc_mode=%u",
        mac_msg_ptr->msg.ul_psho_complete_ind.psho_status,
        mac_msg_ptr->msg.ul_psho_complete_ind.psho_type,
        mac_msg_ptr->msg.ul_psho_complete_ind.ul_rlc_mode);

      /* Success
      */
      if (mac_msg_ptr->msg.ul_psho_complete_ind.psho_status == PSHO_SUCCESS)
      {
        MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1063, "GRLU NULL PSHO_COMPLETE success");

        /* Refresh sysinfo during transfer
        */
        read_ul_sysinfo(gas_id);

        /* set this counter assuming that cell reselection had just taken place
        */
        SET_N3102(gas_id);
#ifdef  GSM_GPRS_GRLU_SHOW_N3102
        MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1064, "GRLU OTInit SetN3102=max=%d inc=%d dec=%d",ul[gas_id].n3102,PAN_INC(gas_id),PAN_DEC(gas_id));
#endif
        /*----------------------------------------
        ** Consider PSHO ended, and resume access
        ** Process UL assignment.
        **----------------------------------------
        */
        ul[gas_id].b_psho_active = FALSE;
        ul[gas_id].b_access = TRUE;

        /* Promote RAU/Cell-Update PDU(s) to the front of the queue
        */
        grlc_ul_promote_sig_pdus(gas_id);

        /* Move all PDUs from temp to main queue
        */
        grlc_ul_handle_llc_tmp_pdu_sig(gas_id);

        ul[gas_id].cur_res = mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf;

        if (mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf.tbf_mode == TBF_MODE_EGPRS)
        {
          SET_TBF_MODE_EGPRS(gas_id);
          ul[gas_id].cur_res.cs = mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf.mcs;

          /* Start of a EDGE UL TBF from IDLE
          ** Allocated memory for TX_ARR ,Error handling in allocation func.
          */
          grlc_ul_create_tx_array ( gas_id, E_TX_WORKSPACE );

          /* Update coding scheme for engg_mode
          */
          geran_eng_mode_data_write(ENG_MODE_EGPRS_UL_MCS, &ul[gas_id].cur_res.cs, TRUE, gas_id);
        }
        else
        {
          SET_TBF_MODE_GPRS(gas_id);
          ul[gas_id].cur_res.cs = mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf.cs;

          /* Start of a GPRS UL TBF from IDLE
          ** Allocated memory for TX_ARR ,Error handling in allocation func.
          */
          grlc_ul_create_tx_array ( gas_id, RM_RLC_TX_WORKSPACE );

          /* Update coding scheme for engg_mode
          */
          geran_eng_mode_data_write(ENG_MODE_GPRS_UL_CS, &ul[gas_id].cur_res.cs, TRUE, gas_id);
        }

        grlc_ul_start_xfer(gas_id, mac_msg_ptr);

        /* Enable PL1 access to FIFO (for PSHO)
        */
        grlc_ul_fifo_enable_pl1_access(gas_id);

        /* Even though there is nothing to do from an RLC perspective because
        ** the coding scheme hasn't actually changed, GPL1 will be expecting
        ** a PH_DATA_REQ to unblock the UL FIFO.
        */
        pl1_send_ph_data_req(gas_id);
      } /* PSHO success in NULL */
      else if (mac_msg_ptr->msg.ul_psho_complete_ind.psho_status == PSHO_FAILURE)
      {
        ul[gas_id].b_psho_active = FALSE;
        ul[gas_id].b_access = FALSE;

        /* Promote RAU/Cell-Update PDU(s) to the front of the queue
        */
        grlc_ul_promote_sig_pdus(gas_id);

        /* Move all PDUs from temp to main queue
        */
        grlc_ul_handle_llc_tmp_pdu_sig(gas_id);

        MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1065, "GRLU NULL PSHO_COMPLETE failure");
      } /* PSHO fail in NULL */
    } /* PSHO_COMLPETE in NULL */
#endif /* FEATURE_GPRS_PS_HANDOVER */
    else
    {
      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1066, "GRLU NULL Unexp MAC sig %d", mac_msg_ptr->msg.sig);
    }

    GPRS_MEM_FREE(mac_msg_ptr);
  } /* end MAC msg not NULL */

} /* grlc_ul_handle_mac_event_us_null */


/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_MAC_EVENT_US_AWAIT_ASS()
===
===  DESCRIPTION
===
===    Handler for MAC signals in US_AWAIT_ASS state.
===
===  DEPENDENCIES
===    ul struct
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_handle_mac_event_us_await_ass ( gas_id_t gas_id )
{
  mac_rlc_msg_t *mac_msg_ptr;

  mac_msg_ptr = (mac_rlc_msg_t *)q_get(&UMQ(gas_id));

  if (mac_msg_ptr == NULL)
  {
    MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1067, "GRLU AWAIT_ASS MAC event without msg");
  }
  else
  {
    /*----------------------------------------------------------------------------
    ** If the ul tbf is getting established for Test Mode B or SRB, then the only
    ** valid signal in AWAIT_ASS is MR_UL_ALLOC_CNF. So, emit message and
    ** drop through ..
    **----------------------------------------------------------------------------
    */
    if( ((ul[gas_id].test_mode == TEST_MODE_B) || (ul[gas_id].test_mode == TEST_MODE_SRB)) &&
        (mac_msg_ptr->msg.sig != MR_UL_ALLOC_CNF)   )
    {
      MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1068, "GRLU Test Mode in US_AWAIT_ASS Rcvd Unexp sig %d , send stop test_mode to LLC",
        mac_msg_ptr->msg.sig);

      ul[gas_id].b_access = FALSE;
      rlc_log_ul_statistics_ext(gas_id);

      /* inform llc end of test mode */
      (void)gllc_grr_ul_tbf_rel_ind(gas_id);

      /* Clear data structures relating to current TBF.
      ** Clear tx window, clear all resources granted,
      ** keep current and outstanding pdus, clear all
      ** signal masks
      */
      clr_ul_data(gas_id);

      /* Back to NULL state */
      ul[gas_id].state = US_NULL;

      /* End of UL tbf for Test mode */
      ul[gas_id].test_mode = TEST_MODE_OFF;
    }

    if (mac_msg_ptr->msg.sig == MR_UL_RESET_REQ)
    {
      grlc_ul_panic_reset(gas_id);
    }
    else
    /*---------------------------------------------------------
    ** State: US_AWAIT_ASS, event: MAC
    ** Handle ALLOC_CNF by storing resource and processing PDU
    **---------------------------------------------------------
    */
    if (mac_msg_ptr->msg.sig == MR_UL_ALLOC_CNF)
    {
      rlc_log_ul_config(&mac_msg_ptr->msg.ul_alloc_cnf, gas_id);

      if (mac_msg_ptr->msg.ul_alloc_cnf.access_phase == GMAC_TWO_PHASE_ACCESS)
      {
        /* Send TBF_ACT_IND to LLC for 2 phase */
        grlc_ll_tbf_act_ind (gas_id, TBF_UL);
      }

      /* Force open-ended if given close-ended
      */
      if (mac_msg_ptr->msg.ul_alloc_cnf.tbf_type == RM_CLOSE_ENDED_TBF)
      {
        mac_msg_ptr->msg.ul_alloc_cnf.tbf_type = RM_OPEN_ENDED_TBF;
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1069, "GRLU close-ended converted to open-ended");
      }

      TIMER_STOP_AWAIT_ASS_GUARD(gas_id);

#ifdef  FEATURE_GSM_DTM
      if (mac_msg_ptr->msg.ul_alloc_cnf.dtm_access)
      {
        dsm_item_type *temp_dup_ptr;

        if(cur_pdu_ptr[gas_id] != NULL )
        {
          /* Create another dup item for the current PDU
          */
          temp_dup_ptr = gprs_pdu_duplicate(cur_pdu_ptr[gas_id]->dup_pdu_ptr,
                                            0,
                                            cur_pdu_ptr[gas_id]->len
                                           );

          if (temp_dup_ptr == NULL)
          {
            MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1070, "GRLU AWAIT_ASS GTTP ##dup returns NULL");
            grlc_ul_show_vars(gas_id);

            /* Failed to create dup'ed packet. Set flag to release at the appropriate
            ** fn level
            */
            ul[gas_id].b_error_recovery_required = TRUE;
            ul_diag_stats_st[gas_id].ul_gen_purpose_count_a++;

            /* To break out as we are not in GPRS/EDGE
            */
          }
          else
          {
            /* Enter GPRS transparent transfer protocol state to wait the
            ** confirmation from GRR via MAC
            */
            MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1071, "GRLU AWAIT_ASS send PDU via GRR: PSN %d LEN %d TLLI %08x",
                     cur_pdu_ptr[gas_id]->psn,
                     cur_pdu_ptr[gas_id]->len,
                     cur_pdu_ptr[gas_id]->tlli
                   );

            gttp_send_llc_pdu_to_rr(cur_pdu_ptr[gas_id]->tlli, temp_dup_ptr, gas_id);
            ul[gas_id].state = US_GTTP_XFER;
          } /* dup operation OK */
        }
        else
        {
          MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1072, "GRLU AWAIT_ASS ##cur_pdu_ptr NULL on entering into GTTP_XFER");
          grlc_ul_show_vars(gas_id);
          ul[gas_id].b_error_recovery_required = TRUE;
          ul_diag_stats_st[gas_id].ul_gen_purpose_count_a++;
        }

        GPRS_MEM_FREE(mac_msg_ptr);

        /* Must exit at this point as we are not in GPRS/EGPRS
        */
        return;

      } /* handle dtm access */

#endif /* FEATURE_GSM_DTM */

      /* If EDGE is supported and it's an EGPRS assignment then update our
      ** unique flag to indicate we are in EGPRS mode
      */
      ul[gas_id].cur_res = mac_msg_ptr->msg.ul_alloc_cnf;

      if (mac_msg_ptr->msg.ul_alloc_cnf.tbf_mode == TBF_MODE_EGPRS)
      {
        SET_TBF_MODE_EGPRS(gas_id);
        ul[gas_id].cur_res.cs = mac_msg_ptr->msg.ul_alloc_cnf.mcs;

        /* Start of a EDGE UL TBF from IDLE
        ** Allocated memory for TX_ARR ,Error handling in allocation func.
        */
        grlc_ul_create_tx_array ( gas_id, E_TX_WORKSPACE );

        /* Update coding scheme for engg_mode
        */
        geran_eng_mode_data_write(ENG_MODE_EGPRS_UL_MCS, &ul[gas_id].cur_res.cs, TRUE, gas_id);
      }
      else
      {
        SET_TBF_MODE_GPRS(gas_id);
        ul[gas_id].cur_res.cs = mac_msg_ptr->msg.ul_alloc_cnf.cs;

        /* Start of a GPRS UL TBF from IDLE
        ** Allocated memory for TX_ARR ,Error handling in allocation func.
        */
        grlc_ul_create_tx_array ( gas_id, RM_RLC_TX_WORKSPACE );

        /* Update coding scheme for engg_mode
        */
        geran_eng_mode_data_write(ENG_MODE_GPRS_UL_CS, &ul[gas_id].cur_res.cs, TRUE, gas_id);
      }

      /* only if it's active
      */
      TIMER_STOP_MAC_UL_RESP(gas_id);

      /* store info returned by MAC tbf est cnf.
      */
      grlc_ul_show_alloc_cnf_await_ass(gas_id, &mac_msg_ptr->msg.ul_alloc_cnf);

      if( ul[gas_id].test_mode == TEST_MODE_OFF )
      {
        grlc_ul_handle_alloc_cnf_await_ass(gas_id);
      } /* ul[gas_id].test_mode == TEST_MODE_OFF */
      else if( (ul[gas_id].test_mode == TEST_MODE_B) || (ul[gas_id].test_mode == TEST_MODE_SRB) )
      {
        MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1073, "GRLU Est UL tbf for Test Mode");

        /* Move to state US_TEST_MODE while Test Mode B or SRB is active */
        ul[gas_id].state = US_TEST_MODE;

        /* ! set uo flag to send PH_DATA_REQ to L1 on the
        ** first data block of this TBF
        */
        ul[gas_id].b_first_fifo_load = TRUE;

      } /* ul[gas_id].test_mode == TEST_MODE_B */
      else
      {
        MSG_GRLC_ERROR_1_G(GRLC_F3_MSG_ENUM_1074, "GRLU Invalid Test Mode %d",ul[gas_id].test_mode);
      }

      /* Re-assess flow control
      */
      grlc_llc_ul_flow_control(gas_id);

    } /* ALLOC_CNF */

    else
    if (mac_msg_ptr->msg.sig == MR_NO_PS_ACCESS_IND)
    {
      rlc_log_ul_statistics_ext(gas_id);
      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1075, "GRLC AWAIT_ASS no-access flag=%u",
                mac_msg_ptr->msg.no_ps_access_ind.delete_pdu_flag);
      grlc_ul_handle_no_ps_access(gas_id, mac_msg_ptr->msg.no_ps_access_ind.delete_pdu_flag);
    }
#ifdef  FEATURE_GPRS_PS_HANDOVER
    else
    if (mac_msg_ptr->msg.sig == MR_UL_PSHO_START_IND)
    {
      if (ul[gas_id].b_psho_active)
      {
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1076, "GRLU AWAIT_ASS PSHO_START whilst ALREADY active");
      }

      /*--------------------------------------------------------------------------------
      ** If MAC sends PSHO_START when RLC sends TBF_REQ, MAC shall ignore RLC's request
      ** so RLC should come out of AWAIT_ASS and return to NULL and request TBF later
      **--------------------------------------------------------------------------------
      */
      MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1077, "GRLC AWAIT_ASS PSHO_START");
      ul[gas_id].b_psho_active = TRUE;
      ul[gas_id].b_access = FALSE;

      TIMER_START_AWAIT_ASS_GUARD(gas_id);
      ul[gas_id].state = US_NULL;
      clr_ul_data(gas_id);
      grlc_ul_unput_paq(gas_id);
    }
#endif /* FEATURE_GPRS_PS_HANDOVER  */
    else
    if (mac_msg_ptr->msg.sig == MR_PS_ACCESS_REQ)
    {
      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1078, "GRLU AWAIT_ASS action on ps_access=%u",
                mac_msg_ptr->msg.ps_access_req.action);
      grlc_ul_handle_ps_access(gas_id, mac_msg_ptr->msg.ps_access_req.action);
    }
    else
    /*------------------------------------------
    ** State: US_AWAIT_ASS, Un-expected message
    **------------------------------------------
    */
    {
      /* Unexpected, so get it off the queue and free it
      */
      MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1079, "GRLU AWAIT_ASS Unexp MAC sig %d",mac_msg_ptr->msg.sig);
    }

    GPRS_MEM_FREE(mac_msg_ptr);
  } /* end MAC sig not NULL */

} /* grlc_ul_handle_mac_event_us_await_ass */


#ifdef  FEATURE_GPRS_PS_HANDOVER

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_PSHO_COMPLETE_IN_XFER()
===
===  DESCRIPTION
===
===
===  DEPENDENCIES
===    ul struct
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_handle_psho_complete_in_xfer
(
  gas_id_t      gas_id,
  mac_rlc_msg_t *mac_msg_ptr
)
{
  MSG_GRLC_MED_3_G(GRLC_F3_MSG_ENUM_1080, "GRLU A PSHO_COMPLETE type = %u reset_flag=%u rlc_mode=%u",
    mac_msg_ptr->msg.ul_psho_complete_ind.psho_type,
    mac_msg_ptr->msg.ul_psho_complete_ind.reset_flag,
    mac_msg_ptr->msg.ul_psho_complete_ind.ul_rlc_mode);

  if (mac_msg_ptr->msg.ul_psho_complete_ind.psho_status == PSHO_SUCCESS)
  {
    if (mac_msg_ptr->msg.ul_psho_complete_ind.psho_type == PSHO_GTOW)
    {
      clr_ul_data(gas_id);

      ul[gas_id].b_access = FALSE;

      grlc_ul_unput_paq(gas_id);

      /* No STATUS_IND sent if SM PDU is deleted, hence FALSE. */

      flush_llc_pdu_queue(gas_id, FALSE);

      ul[gas_id].state = US_NULL;
    }
    else if (mac_msg_ptr->msg.ul_psho_complete_ind.psho_type == PSHO_GTOG)
    {
      /* Refresh sysinfo during transfer
      */
      read_ul_sysinfo(gas_id);

      /* set this counter assuming that cell re selection had just taken place
      */
      SET_N3102(gas_id);
#ifdef  GSM_GPRS_GRLU_SHOW_N3102
      MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1081, "GRLU OTInit SetN3102=max=%d inc=%d dec=%d",ul[gas_id].n3102,PAN_INC(gas_id),PAN_DEC(gas_id));
#endif

      if (mac_msg_ptr->msg.ul_psho_complete_ind.reset_flag)
      {
        /* Clear UL resources, unput PAQ
        */
        clr_ul_data(gas_id);
        grlc_ul_unput_paq(gas_id);

        /* Promote RAU/Cell-Update PDU(s) to the front of the queue
        */
        grlc_ul_promote_sig_pdus(gas_id);

        /* Take UL assignment
        */
        ul[gas_id].cur_res = mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf;

        if (mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf.tbf_mode == TBF_MODE_EGPRS)
        {
          SET_TBF_MODE_EGPRS(gas_id);
          ul[gas_id].cur_res.cs = mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf.mcs;

          /* Start of a EDGE UL TBF from IDLE
          ** Allocated memory for TX_ARR ,Error handling in allocation func.
          */
          grlc_ul_create_tx_array ( gas_id, E_TX_WORKSPACE );

          /* Update coding scheme for engg_mode
          */
          geran_eng_mode_data_write(ENG_MODE_EGPRS_UL_MCS, &ul[gas_id].cur_res.cs, TRUE, gas_id);
        }
        else
        {
          SET_TBF_MODE_GPRS(gas_id);
          ul[gas_id].cur_res.cs = mac_msg_ptr->msg.ul_psho_complete_ind.ul_alloc_cnf.cs;

          /* Start of a GPRS UL TBF from IDLE
          ** Allocated memory for TX_ARR ,Error handling in allocation func.
          */
          grlc_ul_create_tx_array ( gas_id, RM_RLC_TX_WORKSPACE );

          /* Update coding scheme for engg_mode
          */
          geran_eng_mode_data_write(ENG_MODE_GPRS_UL_CS, &ul[gas_id].cur_res.cs, TRUE, gas_id);
        }
      }
      /* Move all PDUs from temp to main queue
      */
      grlc_ul_handle_llc_tmp_pdu_sig(gas_id);

      grlc_ul_start_xfer(gas_id, mac_msg_ptr);
    }
    else
    {
      MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1082, "GRLU A PSHO_COMPLETE success bad type %u",
        mac_msg_ptr->msg.ul_psho_complete_ind.psho_type);
    }
  }
  else if (mac_msg_ptr->msg.ul_psho_complete_ind.psho_status == PSHO_FAILURE)
  {
    /* Move all PDUs from temp to main queue
    */
    grlc_ul_handle_llc_tmp_pdu_sig(gas_id);

    /* Enable PL1 access to FIFO (for PSHO)
    */
    grlc_ul_fifo_enable_pl1_access(gas_id);

    /* Even though there is nothing to do from an RLC perspective because
    ** the coding scheme hasn't actually changed, GPL1 will be expecting
    ** a PH_DATA_REQ to unblock the UL FIFO.
    */
    pl1_send_ph_data_req(gas_id);
  }
  else
  {
    MSG_GRLC_ERROR_1_G(GRLC_F3_MSG_ENUM_1083, "GRLU A PSHP_COMPLETE bad status %u",
      mac_msg_ptr->msg.ul_psho_complete_ind.psho_status);
  }

} /* grlc_ul_handle_psho_complete_in_xfer */

#endif /* FEATURE_GPRS_PS_HANDOVER  */
/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_MAC_EVENT_US_ACK_XFER()
===
===  DESCRIPTION
===
===    Handler for MAC signals in US_ACK_XFER state.
===
===  DEPENDENCIES
===    ul struct
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_handle_mac_event_us_ack_xfer ( gas_id_t gas_id )
{
  mac_rlc_msg_t *mac_msg_ptr;
  uint16        psn;

  mac_msg_ptr = (mac_rlc_msg_t *)q_get(&UMQ(gas_id));

  if (mac_msg_ptr == NULL)
  {
    MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1084, "GRLU A MAC event without msg");
  }
  else
  {
    if (mac_msg_ptr->msg.sig == MR_UL_RESET_REQ)
    {
      grlc_ul_panic_reset(gas_id);
    }
    else
    if (mac_msg_ptr->msg.sig == MR_UL_PKT_ACKNACK_IND)
    {
      if (ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS)
      {
        e_show_rbb(gas_id, &mac_msg_ptr->msg.ul_pkt_acknack_ind.egprs_acknack_desc,
                    mac_msg_ptr->msg.ul_pkt_acknack_ind.egprs_acknack_desc.fai);
      }
      else
      {
        show_rbb(gas_id, &mac_msg_ptr->msg.ul_pkt_acknack_ind.acknack_desc,
                    mac_msg_ptr->msg.ul_pkt_acknack_ind.acknack_desc.fai);
      }

      /* If in one-phase and Contention Unresolved then reset N3104 counter
      ** as this paknak will resolve it
      */
      if (IS_TLLI_REQUIRED(gas_id))
      {
        RESET_N3104(gas_id);
#ifdef  GSM_GPRS_GRLU_SHOW_N3104
        MSG_GRLC_EXT_MED_2_G(GRLC_F3_MSG_ENUM_1085, "GRLU A ResN3104=%d max=%d",ul[gas_id].n3104, N3104_MAX(gas_id));
#endif
      }

      /*----------------------------------------------------------------------
      ** Handle UL acknack in EGPRS mode
      **----------------------------------------------------------------------
      */
      if (ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS)
      {
        ul[gas_id].egprs_acknack_desc = mac_msg_ptr->msg.ul_pkt_acknack_ind.egprs_acknack_desc;
        ul[gas_id].cur_res.resegment = mac_msg_ptr->msg.ul_pkt_acknack_ind.resegment;
        ul[gas_id].pre_emptive_tx = mac_msg_ptr->msg.ul_pkt_acknack_ind.pre_emptive_tx;
        MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1086, "ERLU A EPUAN pre_tx=%u resegment=%u tfi=%u",
                mac_msg_ptr->msg.ul_pkt_acknack_ind.pre_emptive_tx,
                mac_msg_ptr->msg.ul_pkt_acknack_ind.resegment,
                mac_msg_ptr->msg.ul_pkt_acknack_ind.tfi);
        ul[gas_id].acknack_blk_cnt = mac_msg_ptr->msg.ul_pkt_acknack_ind.blk_cnt;

        if (ul[gas_id].egprs_acknack_desc.fai == RM_ACKNACK_FAI_TBF_COMPLETE)
        {
          rlc_log_ul_statistics_ext(gas_id);
          ul[gas_id].b_access = FALSE;

          TIMER_STOP_T3182(gas_id);

          TIMER_STOP_AWAIT_UL_ACKNACK(gas_id);
          MSG_GRLC_HIGH_0_G(GRLC_F3_MSG_ENUM_1087, "ERLU FAI XTimer stopped");
          ul[gas_id].guard_timer_state = GUARD_OFF;

          /* To transfer grouping info to the next PDU
          */
          ungroup_pdu(gas_id, cur_pdu_ptr[gas_id]);

          /* EGPRS mode EXT_UTBF: interpret SSN and RRB
          */
          if (IS_EXT_UTBF_ACTIVE(gas_id))
          {
            /* Freeze FIFO access for reprocessing
            */
            grlc_ul_fifo_disable_pl1_access(gas_id);

            grlc_ul_fifo_delete_unread_nack_pack_blocks(gas_id, &ul[gas_id].nack_vsf,
                                                        &ul[gas_id].pack_vsf);

            /* Re-enable PL1 access to the FIFO
            */
            grlc_ul_fifo_enable_pl1_access(gas_id);

            UPD_UL_NACK_VSF(gas_id);
            UPD_UL_PACK_VSF(gas_id);

            /* (VA-1) holds the final ack block
            */
            e_update_ack_state_array(gas_id);

            psn = grlc_ul_paq_seek_psn_cv0(gas_id, ul[gas_id].va);
          }
          else
          {
            /* Ignore bitmap and free PDUs and LLC messages upto VS or CV=0.
            */
            psn = grlc_ul_paq_seek_psn_cv0(gas_id, ul[gas_id].vs);
          }

          clr_ul_data(gas_id);

          /* Successful transmission of PDU , set the flag to TRUE
          ** to start ready timer
          */
          grlc_ul_del_free_paq_upto_psn(gas_id, psn,TRUE);
          grlc_ul_unput_paq(gas_id);

          ul[gas_id].state = US_NULL;
          UPD_UL_STATE(gas_id);

          /* Consider the TBF released. Now load the next PDU and establish new TBF
          ** otherwise go back to NULL
          */
          (void)load_cur_pdu(gas_id);
        } /* FAI complete */
        else
        {
          /* FA=0 so update bitmap, check for ChCS and carry on
          */

          /* Re-process the UL FIFO as paknak will have made some blocks
          ** invalid. So delete the un-read blocks in the FIFO and rewind the
          ** transmit vars back to the BSN value of the oldest blocks
          ** (pack_vsf, nack_vsf)
          */
          /* Freeze FIFO access for reprocessing
          */
          grlc_ul_fifo_disable_pl1_access(gas_id);

          grlc_ul_fifo_delete_unread_nack_pack_blocks(gas_id, &ul[gas_id].nack_vsf, &ul[gas_id].pack_vsf);

          /* Re-enable PL1 access to the FIFO
          */
          grlc_ul_fifo_enable_pl1_access(gas_id);

          UPD_UL_NACK_VSF(gas_id);
          UPD_UL_PACK_VSF(gas_id);

          e_update_ack_state_array(gas_id);

          /* EGPRS RA_AT_TX_COMPLETE_CHANGE_MODE type 3
          */
          if (IS_EXT_UTBF_ACTIVE(gas_id) &&
              cur_pdu_ptr[gas_id]->realloc_action == RA_AT_TX_COMPLETE_CHANGE_MODE &&
              ul[gas_id].realloc_state != RALS_COMPLETE &&
              IS_TX_COMPLETE(gas_id) &&
              ul[gas_id].va == ul[gas_id].vs   &&
              next_pdu_ptr[gas_id] != NULL
             )
          {
            /* Send PRR with parameters from the next PDU
            */
            ul[gas_id].realloc_pdu_ptr = next_pdu_ptr[gas_id];
            ul[gas_id].realloc_psn = next_pdu_ptr[gas_id]->psn;

            /* b_cv_zero - is set to TRUE , because we
             have transitioned to
             realloc_state = RA_AT_TX_COMPLETE_CHANGE_MODE,
             that means we have Data in our Uplink Q.
            */
            realloc_for_next_pdu(gas_id, ul[gas_id].realloc_pdu_ptr,FALSE);

            /* Set RALS_COMPLETE once after sending a PRR.
            ** Expects this realloc_state to change when the next current PDU is loaded
            */
            ul[gas_id].realloc_state = RALS_COMPLETE;

            MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1088, "ERLU prr TX_COMPLETED psn=%u", next_pdu_ptr[gas_id]->psn);
          }

          /* FA = 0. Window has been un-stalled (not full) then stop TIMER
          */
          if (!IS_E_TX_WINDOW_FULL(gas_id))
          {
            TIMER_STOP_T3182(gas_id);
          }

          /* if there is abort from layer,pack fifo can be flushed.
          ** CR: 125334
          */
          ul[gas_id].b_puan = TRUE;

          /* Check and handle change of MCS
          */
          if (ul[gas_id].cur_res.cs != mac_msg_ptr->msg.ul_pkt_acknack_ind.mcs )
          {
            /* Freeze FIFO access for reprocessing
            */
            grlc_ul_fifo_disable_pl1_access(gas_id);

            /* parameter contains the new coding scheme
            */
            change_coding_scheme(gas_id, mac_msg_ptr->msg.ul_pkt_acknack_ind.mcs);

            /* Re-enable PL1 access to the FIFO
            */
            grlc_ul_fifo_enable_pl1_access(gas_id);

            /* update the 'last' working coding scheme
            */
            ul[gas_id].last_valid_mcs = ul[gas_id].cur_res.cs;

            /* Update coding scheme for engg_mode
            */
            geran_eng_mode_data_write(ENG_MODE_EGPRS_UL_MCS, &ul[gas_id].cur_res.cs, TRUE, gas_id);

          }
        } /* end FAI not set */

        process_guard_timer(gas_id);

        /* Log EPUAN
        ** Log Code: 0x5209
        */
        rlc_log_epuan(gas_id, &(mac_msg_ptr->msg.ul_pkt_acknack_ind));

      } /* TBF_MODE_EGPRS */
      else
      {
        /*--------------------------------------------------------------
        ** Handle ul acknack in GPRS mode with EGPRS support, ie. using
        ** the old code with the GPRS union reference.
        **--------------------------------------------------------------
        */

        /* store the latest packet uplink acknack message (with blk_cnt)
        ** prior to looking at coding scheme change
        */
        ul[gas_id].ul_acknack = mac_msg_ptr->msg.ul_pkt_acknack_ind.acknack_desc;
        ul[gas_id].acknack_blk_cnt = mac_msg_ptr->msg.ul_pkt_acknack_ind.blk_cnt;

        if (ul[gas_id].ul_acknack.fai == RM_ACKNACK_FAI_TBF_COMPLETE)
        {
          rlc_log_ul_statistics_ext(gas_id);
          ul[gas_id].b_access = FALSE;

          TIMER_STOP_T3182(gas_id);

          MSG_GRLC_HIGH_0_G(GRLC_F3_MSG_ENUM_1089, "GRLU FAI XTimer stopped");
          TIMER_STOP_AWAIT_UL_ACKNACK(gas_id);
          ul[gas_id].guard_timer_state = GUARD_OFF;

          /* To transfer grouping info to the next PDU
          */
          ungroup_pdu(gas_id, cur_pdu_ptr[gas_id]);

          /* GPRS mode EXT_UTBF: interpret SSN and RRB
          */
          if (IS_EXT_UTBF_ACTIVE(gas_id))
          {
            /* Freeze FIFO access for reprocessing
            */
            grlc_ul_fifo_disable_pl1_access(gas_id);

            grlc_ul_fifo_delete_unread_nack_pack_blocks(gas_id, &ul[gas_id].nack_vsf,
                                                        &ul[gas_id].pack_vsf);

            /* Re-enable PL1 access to the FIFO
            */
            grlc_ul_fifo_enable_pl1_access(gas_id);

            UPD_UL_NACK_VSF(gas_id);
            UPD_UL_PACK_VSF(gas_id);

            /* (VA-1) holds the final ack block
            */
            update_ack_state_array(gas_id);
            rlc_log_ul_acknack_params(gas_id);

            psn = grlc_ul_paq_seek_psn_cv0(gas_id, ul[gas_id].va);
          }
          else
          {
            /* Ignore bitmap and free PDUs and LLC messages upto VS or CV=0.
            */
            psn = grlc_ul_paq_seek_psn_cv0(gas_id, ul[gas_id].vs);
          }
          clr_ul_data(gas_id);

          /* Successful transmission of PDU , set the flag to TRUE
          ** to start ready timer
          */
          grlc_ul_del_free_paq_upto_psn(gas_id, psn,TRUE);
          grlc_ul_unput_paq(gas_id);

          ul[gas_id].state = US_NULL;
          UPD_UL_STATE(gas_id);

          /* Consider the TBF released. Now load the next PDU and establish new TBF
          ** otherwise go back to NULL
          */
          (void)load_cur_pdu(gas_id);
        } /* GPRS RM_ACKNACK_FAI_TBF_COMPLETE */
        else
        {
          /* FA=0 so update bitmap, check for ChCS and carry on
          */

          /* Re-process the UL FIFO as paknak will have made some blocks
          ** invalid. So delete the un-read blocks in the FIFO and rewind the
          ** transmit vars back to the BSN value of the oldest blocks
          ** (pack_vsf, nack_vsf)
          */
          /* Freeze FIFO access for reprocessing
          */
          grlc_ul_fifo_disable_pl1_access(gas_id);

          grlc_ul_fifo_delete_unread_nack_pack_blocks(gas_id, &ul[gas_id].nack_vsf, &ul[gas_id].pack_vsf);

          /* Re-enable PL1 access to the FIFO
          */
          grlc_ul_fifo_enable_pl1_access(gas_id);

          UPD_UL_NACK_VSF(gas_id);
          UPD_UL_PACK_VSF(gas_id);

          update_ack_state_array(gas_id);

          /* GPRS RA_AT_TX_COMPLETE_CHANGE_MODE type 3
          */
          if (IS_EXT_UTBF_ACTIVE(gas_id) &&
              cur_pdu_ptr[gas_id]->realloc_action == RA_AT_TX_COMPLETE_CHANGE_MODE &&
              ul[gas_id].realloc_state != RALS_COMPLETE &&
              IS_TX_COMPLETE(gas_id) &&
              ul[gas_id].va == ul[gas_id].vs   &&
              next_pdu_ptr[gas_id] != NULL
             )
          {
            /* Send PRR with parameters from the next PDU
            */
            ul[gas_id].realloc_pdu_ptr = next_pdu_ptr[gas_id];
            ul[gas_id].realloc_psn = next_pdu_ptr[gas_id]->psn;

            /* b_cv_zero - is set to TRUE , because we
             have transitioned to
             realloc_state = RA_AT_TX_COMPLETE_CHANGE_MODE,
             that means we have Data in our Uplink Q.
            */
            realloc_for_next_pdu(gas_id, ul[gas_id].realloc_pdu_ptr,FALSE);

            /* Set RALS_COMPLETE once after sending a PRR.
            ** Expects this realloc_state to change when the next current PDU is loaded
            */
            ul[gas_id].realloc_state = RALS_COMPLETE;

            MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1090, "GRLU prr TX_COMPLETED psn=%u", next_pdu_ptr[gas_id]->psn);
          }

          rlc_log_ul_acknack_params(gas_id);

          /* FA = 0. Window has been un-stalled (not full) then stop TIMER
          */
          if (!IS_TX_WINDOW_FULL(gas_id))
          {
            TIMER_STOP_T3182(gas_id);
          }

          /* Check and handle change of CS
          */
          if (ul[gas_id].cur_res.cs != mac_msg_ptr->msg.ul_pkt_acknack_ind.cs )
          {
            /* Freeze FIFO access for reprocessing
            */
            grlc_ul_fifo_disable_pl1_access(gas_id);

            /* parameter contains the new coding scheme
            */
            change_coding_scheme(gas_id, mac_msg_ptr->msg.ul_pkt_acknack_ind.cs);

            /* Re-enable PL1 access to the FIFO
            */
            grlc_ul_fifo_enable_pl1_access(gas_id);

            /* update the 'last' working coding scheme
            */
            ul[gas_id].last_valid_coding_scheme = ul[gas_id].cur_res.cs;

            /* Update coding scheme for engg_mode
            */
            geran_eng_mode_data_write(ENG_MODE_GPRS_UL_CS, &ul[gas_id].cur_res.cs, TRUE, gas_id);

          } /* RM_ACKNACK_FAI_TBF_COMPLETE */

          process_guard_timer(gas_id);

        } /* end FAI not set */
      } /* GPRS acknack with EGPRS support */

#ifdef FEATURE_QSH_EVENT_METRIC
      grlc_ul_qsh_metric_report(gas_id);
#endif

    } /* PKT_ACKNACK_IND */

    else if (mac_msg_ptr->msg.sig == MR_UL_ALLOC_CNF)
    /*------------------------------------------------------------
    ** US_ACK_XFER, UL_ALLOC_CNF signal covers both solicited and
    ** un-solicited resource assignment
    **------------------------------------------------------------
    */
    {
      rlc_log_ul_config(&mac_msg_ptr->msg.ul_alloc_cnf, gas_id);

      if (mac_msg_ptr->msg.ul_alloc_cnf.access_phase == GMAC_TWO_PHASE_ACCESS)
      {
        /* Send TBF_ACT_IND to LLC for 2 phase */
        grlc_ll_tbf_act_ind (gas_id, TBF_UL);
      }

      /* Refresh sysinfo during transfer
      */
      read_ul_sysinfo(gas_id);

      /* Force open-ended if given close-ended
      */
      if (mac_msg_ptr->msg.ul_alloc_cnf.tbf_type == RM_CLOSE_ENDED_TBF)
      {
        mac_msg_ptr->msg.ul_alloc_cnf.tbf_type = RM_OPEN_ENDED_TBF;
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1091, "GRLU close-ended converted to open-ended");
      }

      /*-------------------------------------------------------------------------
      ** If un-solicited assignment is of different tbf_mode then report error
      ** and perform RLC UL recovery, ie. tear down UL TBF and reset and request
      ** a new TBF
      **-------------------------------------------------------------------------
      */
      if (mac_msg_ptr->msg.ul_alloc_cnf.tbf_mode != ul[gas_id].cur_res.tbf_mode)
      {
        MSG_GRLC_ERROR_2_G(GRLC_F3_MSG_ENUM_1092, "ERLU A ##cur_tbf_mode=%u next_tbf_mode=%u",
                  ul[gas_id].cur_res.tbf_mode,
                  mac_msg_ptr->msg.ul_alloc_cnf.tbf_mode);

        grlc_ul_show_vars(gas_id);
        ul[gas_id].b_error_recovery_required = TRUE;
        detect_error_and_recover(gas_id);
      }

      grlc_ul_handle_alloc_cnf_ack_xfer(gas_id, &mac_msg_ptr->msg.ul_alloc_cnf);

      /* Assess flow control to cater for changes to MCS, windows size or number
      ** of timeslots
      */
      grlc_llc_ul_flow_control(gas_id);

    } /* ALLOC_CNF */

    else
    if (mac_msg_ptr->msg.sig == MR_NO_PS_ACCESS_IND)
    {
      rlc_log_ul_statistics_ext(gas_id);
      grlc_ul_handle_no_ps_access(gas_id, mac_msg_ptr->msg.no_ps_access_ind.delete_pdu_flag);
      MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1093, "GRLU XFER no-access");
    }
#ifdef  FEATURE_GPRS_PS_HANDOVER
    else
    if (mac_msg_ptr->msg.sig == MR_UL_PSHO_START_IND)
    {
      if (ul[gas_id].b_psho_active)
      {
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1094, "GRLU A XFER PSHO_START whilst ALREADY active");
      }

      ul[gas_id].b_psho_active = TRUE;
      ul[gas_id].b_access = FALSE;
      MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1095, "GRLU A XFER PSHO_START");

      /* Disable PL1 access to FIFO
      */
      grlc_ul_fifo_disable_pl1_access(gas_id);
    }
    else
    if (mac_msg_ptr->msg.sig == MR_UL_PSHO_COMPLETE_IND)
    {
      if (!ul[gas_id].b_psho_active)
      {
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1096, "GRLU A XFER PSHO_COMPLETE whilst PSHO not active");
      }

      MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1097, "GRLU A XFER PSHO_COMPLETE");
      ul[gas_id].b_psho_active = FALSE;
      ul[gas_id].b_access = TRUE;

      grlc_ul_handle_psho_complete_in_xfer(gas_id, mac_msg_ptr);

      /* Enable PL1 access to FIFO
      */
      grlc_ul_fifo_enable_pl1_access(gas_id);
    }
#endif /* FEATURE_GPRS_PS_HANDOVER  */
    else
    if (mac_msg_ptr->msg.sig == MR_PS_ACCESS_REQ)
    {
#ifdef  FEATURE_GPRS_PS_HANDOVER
      MSG_GRLC_MED_2_G(GRLC_F3_MSG_ENUM_1098, "GRLU XFER access (ignore access = %u) PSHO=%u",
        ul[gas_id].b_access, ul[gas_id].b_psho_active);
#else
      /* In ack-xfer, so reassert access state, but do nothing
      */
      ul[gas_id].b_access = TRUE;
      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1099, "GRLU XFER action on ps_access=%u",mac_msg_ptr->msg.ps_access_req.action);

#endif /* FEATURE_GPRS_PS_HANDOVER  */
    }
    else
    if (mac_msg_ptr->msg.sig == MR_UL_RELEASE_IND)
    /*--------------------------------------------------------
    ** US_ACK_XFER, MR_UL_RELEASE_IND
    ** uplink abnormal and normal release indication from MAC
    **--------------------------------------------------------
    */
    {
      rlc_log_ul_statistics_ext(gas_id);
      MSG_GRLC_MED_3_G(GRLC_F3_MSG_ENUM_1100, "GRLU A Rcvd REL_IND cause %d",
              mac_msg_ptr->msg.ul_release_ind.cause,
              0,
              0);

      /* note the uplink resource release cause
      */
      ul[gas_id].ul_release_cause = mac_msg_ptr->msg.ul_release_ind.cause;

      rlc_log_ul_release_ind(gas_id, (uint8)ul[gas_id].cur_res.ul_tfi,RLC_LOG_UL_MAC_RELEASE_IND);

      handle_ul_release_event(gas_id, ul[gas_id].ul_release_cause, TRUE);

      /* Assess flow control as MCS, window size and number of timeslots drop
      ** back to default values
      */
      grlc_llc_ul_flow_control(gas_id);

#ifdef FEATURE_QSH_EVENT_METRIC
      grlc_ul_qsh_metric_report(gas_id);
#endif

    }
    else if (mac_msg_ptr->msg.sig == MR_UL_CONTENTION_IND)
    /*-----------------------------------------------------------
    ** US_ACK_XFER, MR_UL_CONTENTION_IND
    ** Contention resolution signal from MAC in one phase access
    **-----------------------------------------------------------
    */
    {
      ul[gas_id].acknack_blk_cnt = mac_msg_ptr->msg.ul_tlli_ind.blk_cnt;

      /* ! Must check for change of MCS in contention_res..()
      */
      ul[gas_id].pre_emptive_tx = mac_msg_ptr->msg.ul_tlli_ind.pre_emptive_tx;
      ul[gas_id].cur_res.resegment = mac_msg_ptr->msg.ul_tlli_ind.resegment;
      MSG_GRLC_MED_3_G(GRLC_F3_MSG_ENUM_1101, "ERLU A Rcvd CONTENTION_IND petx=%u reseg=%u",
              ul[gas_id].pre_emptive_tx,
              ul[gas_id].cur_res.resegment,
              0);

      if (mac_msg_ptr->msg.ul_tlli_ind.contention_res ==
          RM_CONTENTION_RESOLVED
         )
      {
        handle_contention_resolution_success(gas_id, mac_msg_ptr);
        /* Send contention_resolution_success_ind to LLC */
        grlc_ll_tbf_act_ind (gas_id, TBF_UL);
      }

      /* Assess flow control as MCS, window size and number of timeslots may change
      ** after cont-resolution
      */
      grlc_llc_ul_flow_control(gas_id);

    } /* CONTENTION_IND */

    else

    /*----------------------------------------------------------
    ** US_ACK_XFER
    ** don't expect any other signals from MAC in ACK_XFER mode
    **----------------------------------------------------------
    */
    {
      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1102, "GRLU A Unexp sig %d",mac_msg_ptr->msg.sig);

      /*------------------------------------------------------------------
      ** ! don't drop into ack_mode_xfer(). So free memory and exit here
      **------------------------------------------------------------------
      */
      GPRS_MEM_FREE(mac_msg_ptr);

      return;

    } /* bad MAC sig */

    if (cur_pdu_ptr[gas_id] != NULL && ul[gas_id].state == US_ACK_XFER)
    {
      if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
      {
        e_ack_mode_xfer(gas_id);
      }
      else
      {
        ack_mode_xfer(gas_id);
      }
    }

    GPRS_MEM_FREE(mac_msg_ptr);
  } /* end MAC sig not NULL */

} /* grlc_ul_handle_mac_event_us_ack_xfer */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_TMR_EVENT_US_NULL()
===
===  DESCRIPTION
===
===    Handler for TMR signals in US_NULL state.
===
===  DEPENDENCIES
===    ul struct
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_handle_tmr_event_us_null ( gas_id_t gas_id )
{
  rm_timer_msg_t  *timer_msg_ptr;

  timer_msg_ptr = (rm_timer_msg_t *)q_get(&UTQ(gas_id));

  if (timer_msg_ptr == NULL)
  {
    MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1103, "GRLU A tmr event without msg");
  }
  else
  {
    /* Stale PDU timer has expired.
    */
    if (timer_msg_ptr->sig == RM_TMR_STALE_PDU_DELETE)
    {

      ul[gas_id].timer_stale_pdu_delete_expiry_cnt++;

      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1104, "GRLU A Expy Stale PDU tmr %u",
                 ul[gas_id].timer_stale_pdu_delete_expiry_cnt);

      /* Set timer state to inactive.
      */
      ul[gas_id].timer_stale_pdu_delete.state = TMR_INACTIVE;

      /* Clear UL realloc variables.
      */
      grlc_clear_realloc_vars(gas_id);

      /* Unput any PDUs in paq queue
      */
      grlc_ul_unput_paq(gas_id);

      /* Purge all UL PDUs. No STATUS_IND sent if SM PDU is deleted, hence FALSE.
      */
      flush_llc_pdu_queue( gas_id, FALSE );

      /* Stale timer has expired, set ul_pdu_delete flag to TRUE. Any PDUs
      ** from upper layers will be deleted while this flag is TRUE.
      */
      ul[gas_id].b_ul_pdu_delete_after_stale_tmr_expiry = TRUE;

      gllc_grr_ul_suspend_ind(gas_id);

      MSG_GRLC_HIGH_0_G(GRLC_F3_MSG_ENUM_1105, "GRLU - UL Suspended");

    }
    else
    {
      MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1106, "GRLU A Unexp TMR sig %d",timer_msg_ptr->sig);
    }

    GPRS_MEM_FREE(timer_msg_ptr);
  }
} /* grlc_ul_handle_tmr_event_us_null */


/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_TMR_EVENT_US_ACK_XFER()
===
===  DESCRIPTION
===
===    Handler for TMR signals in US_ACK_XFER state.
===
===  DEPENDENCIES
===    ul struct
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_handle_tmr_event_us_ack_xfer ( gas_id_t gas_id )
{
  rm_timer_msg_t  *timer_msg_ptr;

  timer_msg_ptr = (rm_timer_msg_t *)q_get(&UTQ(gas_id));

  if (timer_msg_ptr == NULL)
  {
    MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1107, "GRLU A tmr event without msg");
  }
  else
  {
    /*-----------------------------------------------------------------
    ** Handle T3182 expiry. Two cases: during xfer and during release.
    ** During xfer, MS shall decrement  N3102 and perform abs rel with
    ** access retry. During release, MS does the same but without
    ** decrementing N3102.
    **-----------------------------------------------------------------
    */
    if (timer_msg_ptr->sig == RM_TMR_T3182_AWAIT_UL_ACKNACK)
    {
      ul[gas_id].timer_t3182_expiry_cnt++;
      MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1108, "GRLU A Expy T3182 %u NoAcknack",ul[gas_id].timer_t3182_expiry_cnt);

      /* When T3182 expires the N3102 counter is decremented (if enabled).
      ** If N3102 expires then we perform ab rel cell resel. If not we perform
      ** abnormal release with access retry. Because abnormal release cell
      ** reselection has precedence over abnormal release with access retry
      */
      if (IS_N3102_ENABLED(gas_id))
      {
        DEC_N3102(gas_id);
#ifdef  GSM_GPRS_GRLU_SHOW_N3102
        MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1109, "GRLU A DecN3102=%d inc=%d dec=%d",ul[gas_id].n3102,PAN_INC(gas_id),PAN_DEC(gas_id));
#endif
        /* N3102 will cause abnormal release cell reselection which has
        ** precedence over T3182's action
        */
        if (IS_N3102_EXPIRED(gas_id))
        {
          MSG_GRLC_HIGH_3_G(GRLC_F3_MSG_ENUM_1110, "GRLU A N3102 Expy (cell-resel) = %d inc=%d dec=%d",
                  ul[gas_id].n3102,
                  PAN_INC(gas_id),
                  PAN_DEC(gas_id));
          /* Inform MAC of RLC initiated ul release
          */
          indicate_ul_release ( gas_id, REL_UL_ABNORMAL_CELL_RESEL );
          rlc_log_ul_release_ind( gas_id, (uint8)ul[gas_id].cur_res.ul_tfi,
                                  RLC_LOG_UL_EXPIRED_N3102_IN_ACK_TRANSFER);


          /* Cell reselection requested. Two actions are required:
          **
          ** (1). Set N3102 -
          **      Set it to the currently held max value again in case same
          **      cell is reselected. Assumed N3102 max won't change in same
          **      cell.
          **
          ** (2). Delete GMM PDUs -
          **      To avoid RAU PDU accumulation which occurs due to not
          **      receiving ack/nack for the sent RAU PDUs hence T3182/N3102
          **      expiry and resulting reselection request to mac but
          **      returning to the same cell repeatedly.
          */

          SET_N3102(gas_id);

          grlc_ul_del_signalling_pdus(gas_id, NO_NAS_INDICATION);

          /* abnormal release with cell-reselection
          */
          handle_ul_release_event(gas_id, REL_UL_ABNORMAL_CELL_RESEL, FALSE);
        }
        else
        {
          /* Inform MAC of RLC initiated ul release
          */
          indicate_ul_release ( gas_id, REL_UL_ABNORMAL_WITH_ACCESS_RETRY );

          /* abnormal release with access retry with n3102 status maintained
          */
          handle_ul_release_event(gas_id, REL_UL_ABNORMAL_WITH_ACCESS_RETRY, FALSE);

          rlc_log_ul_release_ind(gas_id, (uint8)ul[gas_id].cur_res.ul_tfi,
                                  RLC_LOG_UL_EXPIRED_T3182_IN_ACK_TRANSFER);
        }
      } /* N3102 enabled */
      else
      {
        /* Inform MAC of RLC initiated UL release
        */
        indicate_ul_release ( gas_id, REL_UL_ABNORMAL_WITH_ACCESS_RETRY );

        rlc_log_ul_release_ind(gas_id, (uint8)ul[gas_id].cur_res.ul_tfi,
                                RLC_LOG_UL_EXPIRED_T3182_IN_ACK_TRANSFER);

        /* abnormal release with access retry with n3102 status maintained
        */
        handle_ul_release_event(gas_id, REL_UL_ABNORMAL_WITH_ACCESS_RETRY, FALSE);
      } /* N3102 disabled */
    } /* TMR_T3182_AWAIT_UL_ACKNACK timeout */
    else if (timer_msg_ptr->sig == RM_TMR_AWAIT_MAC_UL_RESP)
    {
      MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1111, "GRLU A Expy MAC_RESP",ul[gas_id].state);
      ack_mode_xfer(gas_id);
    }
    /*--------------------------------------------------------------------------------------
    ** Handle TIMER_AWAIT_UL_ACKNACK. Perform abs rel with access retry if not in test mode
    **--------------------------------------------------------------------------------------
    */
    else if (timer_msg_ptr->sig == RM_TMR_AWAIT_UL_ACKNACK)
    {
      /* No release during GCF testing */
      if (FALSE == mcfg_gcf_nv_get_status())
      {
        ul[gas_id].timer_await_ul_acknack_expiry_cnt++;
        MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1112, "ERLU XTimer expiry cnt = %u", ul[gas_id].timer_await_ul_acknack_expiry_cnt);

        /* Inform MAC of RLC initiated ul release */
        indicate_ul_release ( gas_id, REL_UL_ABNORMAL_WITH_ACCESS_RETRY );

        /* rlc_log_ul_release_ind(gas_id, (uint8)ul[gas_id].cur_res.ul_tfi,
                              RLC_LOG_UL_EXPIRED_T3182_IN_ACK_TRANSFER); */

        /* abnormal release with access retry with n3102 status maintained */
        handle_ul_release_event(gas_id, REL_UL_ABNORMAL_WITH_ACCESS_RETRY, FALSE);
      }
      else
      {
        MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1113, "ERLU XTimer expiry: no release cnt = %u", ul[gas_id].timer_await_ul_acknack_expiry_cnt);
      }
    }
    else
    {
      MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1114, "GRLU A Unexp TMR sig %d",timer_msg_ptr->sig);
    }

    GPRS_MEM_FREE(timer_msg_ptr);
  } /* TMR msg not NULL */

} /* grlc_ul_handle_tmr_event_us_ack_xfer */


#ifdef  FEATURE_GSM_DTM
/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_MAC_EVENT_US_GTTP_XFER()
===
===  DESCRIPTION
===
===    Handler for MAC signals in US_X state.
===
===  DEPENDENCIES
===    ul struct
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_handle_mac_event_us_gttp_xfer ( gas_id_t gas_id )
{
  mac_rlc_msg_t *mac_msg_ptr;
  uint16        psn;

  mac_msg_ptr = (mac_rlc_msg_t *)q_get(&UMQ(gas_id));

  if (mac_msg_ptr == NULL)
  {
    MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1115, "GRLU GTTP MAC event without msg");
  }
  else
  {
    if (mac_msg_ptr->msg.sig == MR_UL_RESET_REQ)
    {
      grlc_ul_panic_reset(gas_id);
    }
    else
    if (mac_msg_ptr->msg.ul_release_ind.sig == MR_UL_RELEASE_IND)
    {
      rlc_log_ul_statistics_ext(gas_id);
      /* Successful GTTP transfer
      */
      if (mac_msg_ptr->msg.ul_release_ind.cause == REL_UL_NORMAL)
      {
        /* Successful GTTP transfer: clear PDU, loads the next PDU.
        ** If the next PDU exists make request .. else goto US_NULL
        */
        if (cur_pdu_ptr[gas_id] != NULL)
        {
          psn = cur_pdu_ptr[gas_id]->psn;

          MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1116, "GRLU GTTP psn %u sent OK",psn);

          /* To transfer grouping info to the next PDU
          */
          ungroup_pdu(gas_id, cur_pdu_ptr[gas_id]);

          /* Successful transmission of PDU , set the flag to TRUE
          ** to start ready timer
          */
          grlc_ul_del_free_paq_upto_psn(gas_id, psn,TRUE);
        }

        clr_ul_data(gas_id);
        grlc_ul_unput_paq(gas_id);

        ul[gas_id].state = US_NULL;
        UPD_UL_STATE(gas_id);

        /* In GTTP_XFER - Normal Release - MAC will not send MR_PS_ACCESS_REQ
         * as it has has not block RLC before.
         */
        ul[gas_id].b_access = TRUE;

        /*
        * Re-order signalling pdus to be sent first to NW
        */
        grlc_ul_reorder_pdus(gas_id);

        /* Consider the TBF released. Now load the next PDU and establish new TBF
        ** otherwise go back to NULL
        */
        (void)load_cur_pdu(gas_id);
        est_tbf_from_null(gas_id);
      }
      else
      {
        /* Only expect two causes above for whilst in US_GTTP_XFER
        */
        MSG_GRLC_ERROR_1_G(GRLC_F3_MSG_ENUM_1117, "GRLU GTTP RELEASE_IND Unexp cause %u",
                    mac_msg_ptr->msg.ul_release_ind.cause);
      } /* handle causes of UL release */
    } /* handle MAC signal whilst in US_GTTP_XFER */
    else if (mac_msg_ptr->msg.sig == MR_NO_PS_ACCESS_IND)
    {
      rlc_log_ul_statistics_ext(gas_id);
      MSG_GRLC_MED_3_G(GRLC_F3_MSG_ENUM_1118, "GRLU GTTP Failed. To resend psn %u delete_flag=%u",
              cur_pdu_ptr[gas_id]->psn,
              mac_msg_ptr->msg.no_ps_access_ind.delete_pdu_flag,
              0);
      grlc_ul_handle_no_ps_access (gas_id, mac_msg_ptr->msg.no_ps_access_ind.delete_pdu_flag);
      (void)load_cur_pdu(gas_id);
    }
    else
    {
      /* Only expect one signal from MAC whilst in US_GTTP_XFER
      */
      MSG_GRLC_ERROR_1_G(GRLC_F3_MSG_ENUM_1119, "GRLU GTTP Unexp MAC sig %u",
                mac_msg_ptr->msg.dl_release_ind.sig);
    } /* end release_ind */

    GPRS_MEM_FREE(mac_msg_ptr);
  } /* mac sig not NULL */

} /* grlc_ul_handle_mac_event_us_gttp_xfer */
#endif /* FEATURE_GSM_DTM */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_MAC_EVENT_US_UNACK_XFER()
===
===  DESCRIPTION
===
===    Handler for MAC signals in US_UNACK_XFER state.
===
===  DEPENDENCIES
===    ul struct
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_handle_mac_event_us_unack_xfer ( gas_id_t gas_id )
{
  mac_rlc_msg_t *mac_msg_ptr;
  uint16        psn;
  boolean       b_test_mode;
  mac_msg_ptr = (mac_rlc_msg_t *)q_get(&UMQ(gas_id));

  if (mac_msg_ptr == NULL)
  {
    MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1120, "GRLU U MAC event without msg");
  }
  else
  {
    if (mac_msg_ptr->msg.sig == MR_UL_RESET_REQ)
    {
      grlc_ul_panic_reset(gas_id);
    }
    else
    if (mac_msg_ptr->msg.sig == MR_UL_PKT_ACKNACK_IND)
    {
      TIMER_STOP_T3182(gas_id);
#ifdef  GSM_GPRS_GRLU_SHOW_T3182
      MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1121, "GRLU U T3182 stopped paknak");
#endif

      /* even though V(A) is not used for marking the ack in unack mode,
      ** it is used to calculate how many blocks have been transmitted
      ** without receiving a paknak. So, when paknak is received, make
      ** V(A) the same as V(S) to make the tx window 0
      */
      ul[gas_id].va = ul[gas_id].vs;
      UPD_UL_VA(gas_id);

      if (ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS)
      {
        MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1122, "ERLU U paknak FA=%d SSN=%d",
                mac_msg_ptr->msg.ul_pkt_acknack_ind.egprs_acknack_desc.fai,
                mac_msg_ptr->msg.ul_pkt_acknack_ind.egprs_acknack_desc.ssn,
                0);

        ul[gas_id].egprs_acknack_desc =  mac_msg_ptr->msg.ul_pkt_acknack_ind.egprs_acknack_desc;

        /* Ignore ul[gas_id].resegment field in unack mode
        */

        /* FAI set and valid rrbp
        */
        if (ul[gas_id].egprs_acknack_desc.fai == RM_ACKNACK_FAI_TBF_COMPLETE)
        {
          rlc_log_ul_statistics_ext(gas_id);
          ul[gas_id].b_access = FALSE;

          /* To transfer grouping info to the next PDU
          */
          ungroup_pdu(gas_id, cur_pdu_ptr[gas_id]);

          /* Ignore bitmap and free PDUs and LLC messages upto VS or CV=0.
          */
          psn = grlc_ul_paq_seek_psn_cv0(gas_id, ul[gas_id].vs);

          /* re-init
          */
          clr_ul_data(gas_id);

          /* Successful transmission of PDU , set the flag to TRUE
          ** to start ready timer
          */
          grlc_ul_del_free_paq_upto_psn(gas_id, psn,TRUE);
          grlc_ul_unput_paq(gas_id);

          ul[gas_id].state = US_NULL;
          UPD_UL_STATE(gas_id);

          (void)load_cur_pdu(gas_id);
        } /* FAI */
        else
        {
          /* Check for Ext UL TBF as well.
          ** Spec. mentions UNACK_RE_TX_MAX for Non-Extended UL TBF Only.
          ** Also, If there are PACK blocks in EXT_UTBF do not queue CV=0 block.
          */
          if (ul[gas_id].re_tx_cv_0_cnt < UNACK_RE_TX_MAX &&
              ul[gas_id].b_cv_0 &&
              IS_TX_COMPLETE(gas_id)
              && ( !( IS_EXT_UTBF_ACTIVE(gas_id) && (grlc_ul_fifo_get_pack_cnt(gas_id) >= 1)) )
             )
          {
            if (ul[gas_id].e_bsn_of_cv_0_block != E_GRLC_INVALID_BSN)
            {
              grlc_ul_data_block_t *tx_blk_p = &TX_ARR(gas_id, ul[gas_id].e_bsn_of_cv_0_block).blk;
			  if((tx_blk_p->cs > RM_MCS_6) && (tx_blk_p->cs <= RM_MCS_9))
              {
                /* Send Block with CV 0 as a single payload */
                grlc_ul_demote_mcs_modify_li( gas_id, ul[gas_id].e_bsn_of_cv_0_block );
              }

              /* FAI clear and re-transmit count is less than 4 so send
              ** block with CV = 0 again
              */

              /* Apply new CPS for PS change. No MCS 7/8/9 since CV 0 always goes out
              ** as a single payload
              */
              if (tx_blk_p->cs <= RM_MCS_4)
              {
                GET_CPS_HDR_3(gas_id, ul[gas_id].e_bsn_of_cv_0_block, tx_blk_p->hdr.cps);
              }
              else if (tx_blk_p->cs <= RM_MCS_6)
              {
                GET_CPS_HDR_2(gas_id, ul[gas_id].e_bsn_of_cv_0_block, tx_blk_p->hdr.cps);         
              }
              else
              {
                MSG_GRLC_ERROR_1_G(GRLC_F3_MSG_ENUM_1123, "GRLU Invalid MCS %d", tx_blk_p->cs);
              }

              E_SEND_BLOCK_CV_0(gas_id);

              /* Store new PS to old PS
              */
              tx_blk_p->old_ps = tx_blk_p->ps;

              /* Rotate PS for next Tx.
              */
              ROTATE_PS_NO_MCS_CHANGE(tx_blk_p->cs,
                                      tx_blk_p->old_ps,
                                      tx_blk_p->ps
                                     );
              ul[gas_id].re_tx_cv_0_cnt++;
            }
            else
            {
              MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1124, "GRLU Invalid bsn_cv_0 = %u",ul[gas_id].e_bsn_of_cv_0_block);
            }
          }

          /* Check and handle change of MCS
          */
          if (ul[gas_id].cur_res.cs != mac_msg_ptr->msg.ul_pkt_acknack_ind.mcs)
          {
            /* Freeze FIFO access for reprocessing
            */
            grlc_ul_fifo_disable_pl1_access(gas_id);

            /* parameter contains the new coding scheme
            */
            change_coding_scheme(gas_id, mac_msg_ptr->msg.ul_pkt_acknack_ind.mcs);

            /* Re-enable PL1 access to the FIFO
            */
            grlc_ul_fifo_enable_pl1_access(gas_id);

            /* update the 'last' working coding scheme
            */
            ul[gas_id].last_valid_mcs = ul[gas_id].cur_res.cs;

            /* Update coding scheme for engg_mode
            */
            geran_eng_mode_data_write(ENG_MODE_EGPRS_UL_MCS, &ul[gas_id].cur_res.cs, TRUE, gas_id);
          }
        } /* FAI = 0 */

        /* Log EPUAN
        ** Log Code: 0x5209
        */
        rlc_log_epuan(gas_id, &(mac_msg_ptr->msg.ul_pkt_acknack_ind));

      } /* EGPRS UNACK EPUAN */
      else
      {
        MSG_GRLC_EXT_MED_2_G(GRLC_F3_MSG_ENUM_1125, "GRLU U paknak FA=%d SSN=%d",
                  mac_msg_ptr->msg.ul_pkt_acknack_ind.acknack_desc.fai,
                  mac_msg_ptr->msg.ul_pkt_acknack_ind.acknack_desc.ssn
                  );

        /* FAI set and valid rrbp
        */
        if (mac_msg_ptr->msg.ul_pkt_acknack_ind.acknack_desc.fai == RM_ACKNACK_FAI_TBF_COMPLETE)
        {
          rlc_log_ul_statistics_ext(gas_id);
          ul[gas_id].b_access = FALSE;

          /* To transfer grouping info to the next PDU
          */
          ungroup_pdu(gas_id, cur_pdu_ptr[gas_id]);

          /* Ignore bitmap and free PDUs and LLC messages upto VS or CV=0.
          */
          psn = grlc_ul_paq_seek_psn_cv0(gas_id, ul[gas_id].vs);

          /* re-init
          */
          clr_ul_data(gas_id);

          /* Successful transmission of PDU , set the flag to TRUE
          ** to start ready timer
          */
          grlc_ul_del_free_paq_upto_psn(gas_id, psn,TRUE);
          grlc_ul_unput_paq(gas_id);

          ul[gas_id].state = US_NULL;
          UPD_UL_STATE(gas_id);

          (void)load_cur_pdu(gas_id);

        } /* FAI */
        else
        {
          /* Check for Ext UL TBF as well.
          ** Spec. mentions UNACK_RE_TX_MAX for Non-Extended UL TBF Only.
          ** Also, If there are PACK blocks do not queue CV=0 block.
          */
          if (ul[gas_id].re_tx_cv_0_cnt < UNACK_RE_TX_MAX &&
              ul[gas_id].b_cv_0 &&
              IS_TX_COMPLETE(gas_id)
              && ( !(IS_EXT_UTBF_ACTIVE(gas_id) && (grlc_ul_fifo_get_pack_cnt(gas_id) >= 1)) )
             )
          {
            /* FAI clear and re-transmit count is less than 4 so send
            ** block with CV = 0 again
            */
            SEND_BLOCK_CV_0(gas_id);
            ul[gas_id].re_tx_cv_0_cnt++;
          }

          /* handle Paknak during UNACK_XFER
          */
          /* store the latest packet uplink acknack message. No need to
          ** test FAI bits etc.. as tx is not completed. Otherwise, we would
          ** be in UNACK_RELEASE state already
          */
          ul[gas_id].ul_acknack = mac_msg_ptr->msg.ul_pkt_acknack_ind.acknack_desc;
          rlc_log_ul_acknack_params(gas_id);

          /* Check and handle change of CS
          */
          if (ul[gas_id].cur_res.cs != mac_msg_ptr->msg.ul_pkt_acknack_ind.cs)
          {
            /* Freeze FIFO access for reprocessing
            */
            grlc_ul_fifo_disable_pl1_access(gas_id);

            /* parameter contains the new coding scheme
            */
            change_coding_scheme(gas_id, mac_msg_ptr->msg.ul_pkt_acknack_ind.cs);

            /* Re-enable PL1 access to the FIFO
            */
            grlc_ul_fifo_enable_pl1_access(gas_id);

            /* update the 'last' working coding scheme
            */
            ul[gas_id].last_valid_coding_scheme = ul[gas_id].cur_res.cs;

            /* Update coding scheme for engg_mode
            */
            geran_eng_mode_data_write(ENG_MODE_GPRS_UL_CS, &ul[gas_id].cur_res.cs, TRUE, gas_id);
          }
        } /* FAI = 0 */
      }

#ifdef FEATURE_QSH_EVENT_METRIC
      grlc_ul_qsh_metric_report(gas_id);
#endif

    } /* PKT_ACKNACK */

    else if (mac_msg_ptr->msg.sig == MR_UL_ALLOC_CNF)
    {
      rlc_log_ul_config(&mac_msg_ptr->msg.ul_alloc_cnf, gas_id);

      if (mac_msg_ptr->msg.ul_alloc_cnf.access_phase == GMAC_TWO_PHASE_ACCESS)
      {
        /* Send TBF_ACT_IND to LLC for 2 phase */
        grlc_ll_tbf_act_ind (gas_id, TBF_UL);
      }
      
      /* Refresh sysinfo during transfer */
      read_ul_sysinfo(gas_id);

      /* Force open-ended if given close-ended */
      if (mac_msg_ptr->msg.ul_alloc_cnf.tbf_type == RM_CLOSE_ENDED_TBF)
      {
        mac_msg_ptr->msg.ul_alloc_cnf.tbf_type = RM_OPEN_ENDED_TBF;
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1126, "GRLU close-ended converted to open-ended");
      }

      /*-------------------------------------------------------------------------
      ** If un-solicited assignment is of different tbf_mode then report error
      ** and perform RLC UL recovery, ie. tear down UL TBF and reset and request
      ** a new TBF
      **-------------------------------------------------------------------------
      */
      if (mac_msg_ptr->msg.ul_alloc_cnf.tbf_mode != ul[gas_id].cur_res.tbf_mode)
      {
        MSG_GRLC_ERROR_2_G(GRLC_F3_MSG_ENUM_1127, "ERLU U ##cur_tbf_mode=%u next_tbf_mode=%u",
                  ul[gas_id].cur_res.tbf_mode,
                  mac_msg_ptr->msg.ul_alloc_cnf.tbf_mode
                  );
        grlc_ul_show_vars(gas_id);
        ul[gas_id].b_error_recovery_required = TRUE;
        detect_error_and_recover(gas_id);
      }

      grlc_ul_handle_alloc_cnf_unack_xfer(gas_id, &mac_msg_ptr->msg.ul_alloc_cnf);

      /* Assess flow control to cater for changes to MCS,
      ** windows size or number of timeslots.
      */
      grlc_llc_ul_flow_control(gas_id);

    } /* ALLOC_CNF */

    else if (mac_msg_ptr->msg.sig == MR_UL_RELEASE_IND)
    {
      rlc_log_ul_statistics_ext(gas_id);
      ul[gas_id].ul_release_cause = mac_msg_ptr->msg.ul_release_ind.cause;
      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1128, "GRLU U Rcvd REL_IND cause %d",ul[gas_id].ul_release_cause);
      rlc_log_ul_release_ind(gas_id, (uint8)ul[gas_id].cur_res.ul_tfi,RLC_LOG_UL_MAC_RELEASE_IND);

      handle_ul_release_event(gas_id, ul[gas_id].ul_release_cause, TRUE);

#ifdef FEATURE_QSH_EVENT_METRIC
      grlc_ul_qsh_metric_report(gas_id);
#endif

    }

    else if (mac_msg_ptr->msg.sig == MR_UL_CONTENTION_IND)
    /*-----------------------------------------------------------
    ** Contention resolution signal from MAC in one phase access
    **-----------------------------------------------------------
    */
    {
      /* We are in un-ack mode. We normally do not expect to be assigned one-phase
      ** in un-ack. But 41.2.6.4 incorrectly assigns one-phase to an un-ack PDU.
      ** In order to proceed to the main part of the test in clude the following
      ** code fragment to handle the Cont-res procedure instead of printing
      ** an error message
      */
      ul[gas_id].acknack_blk_cnt = mac_msg_ptr->msg.ul_tlli_ind.blk_cnt;

      /* ! Must check for change of MCS in contention_res..()
      */
      ul[gas_id].pre_emptive_tx = mac_msg_ptr->msg.ul_tlli_ind.pre_emptive_tx;
      ul[gas_id].cur_res.resegment = mac_msg_ptr->msg.ul_tlli_ind.resegment;

      MSG_GRLC_MED_2_G(GRLC_F3_MSG_ENUM_1129, "GRLU U Rcvd CONTENTION_IND petx=%u reseg=%u",
              ul[gas_id].pre_emptive_tx,
              ul[gas_id].cur_res.resegment);

      if (mac_msg_ptr->msg.ul_tlli_ind.contention_res == RM_CONTENTION_RESOLVED)
      {
        handle_contention_resolution_success(gas_id, mac_msg_ptr);
        /* Send contention_resolution_success_ind to LLC */
        grlc_ll_tbf_act_ind (gas_id, TBF_UL);
      }

      /* Assess flow control as MCS, window size and number of timeslots may change
      ** after cont-resolution
      */
      grlc_llc_ul_flow_control(gas_id);

    } /* CONTENTION_IND */

    else
    if (mac_msg_ptr->msg.sig == MR_NO_PS_ACCESS_IND)
    {
      rlc_log_ul_statistics_ext(gas_id);
      grlc_ul_handle_no_ps_access(gas_id, mac_msg_ptr->msg.no_ps_access_ind.delete_pdu_flag);
      MSG_GRLC_MED_3_G(GRLC_F3_MSG_ENUM_1130, "GRLU UN-XFER no-access delete_flag=%u",
              mac_msg_ptr->msg.no_ps_access_ind.delete_pdu_flag,
              0,
              0);
      b_test_mode = gllc_grr_ul_tbf_rel_ind(gas_id);
      if (b_test_mode)
      {
        MSG_GRLC_HIGH_0_G(GRLC_F3_MSG_ENUM_1131, "GRLU GPRS-TEST-MODE-A Release - Purge all PDUs");
        grlc_ul_purge_pdu_queue(gas_id);
      }
    }
#ifdef  FEATURE_GPRS_PS_HANDOVER
    else
    if (mac_msg_ptr->msg.sig == MR_UL_PSHO_START_IND)
    {
      if (ul[gas_id].b_psho_active)
      {
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1132, "GRLU U XFER PSHO_START whilst ALREADY active");
      }

      ul[gas_id].b_psho_active = TRUE;
      ul[gas_id].b_access = FALSE;

      MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1133, "GRLU U XFER PSHO_START");

      /* Disable PL1 access to FIFO
      */
      grlc_ul_fifo_disable_pl1_access(gas_id);
    }
    else
    if (mac_msg_ptr->msg.sig == MR_UL_PSHO_COMPLETE_IND)
    {
      if (!ul[gas_id].b_psho_active)
      {
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1134, "GRLU U XFER PSHO_COMPLETE whilst PSHO not active");
      }

      MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1135, "GRLU U XFER PSHO_COMPLETE");
      ul[gas_id].b_psho_active = FALSE;
      ul[gas_id].b_access = TRUE;

      grlc_ul_handle_psho_complete_in_xfer(gas_id, mac_msg_ptr);

      /* Enable PL1 access to FIFO
      */
      grlc_ul_fifo_enable_pl1_access(gas_id);
    }
#endif /* FEATURE_GPRS_PS_HANDOVER  */
    else
    if (mac_msg_ptr->msg.sig == MR_PS_ACCESS_REQ)
    {
#ifdef  FEATURE_GPRS_PS_HANDOVER
      MSG_GRLC_MED_2_G(GRLC_F3_MSG_ENUM_1136, "GRLU U XFER access (ignore access = %u) PSHO=%u",
        ul[gas_id].b_access, ul[gas_id].b_psho_active);
#else
      ul[gas_id].b_access = TRUE;
      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1137, "GRLU UN-XFER action on ps_access=%u",mac_msg_ptr->msg.ps_access_req.action);
#endif /* FEATURE_GPRS_PS_HANDOVER  */
    }
    else
    {
      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1138, "GRLU U Unexp MAC sig %d", mac_msg_ptr->msg.sig);
    }

    GPRS_MEM_FREE(mac_msg_ptr);
  } /* end MAC sig not NULL */

} /* grlc_ul_handle_mac_event_us_unack_xfer */


/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_MAC_TMR_US_UNACK_XFER()
===
===  DESCRIPTION
===
===    Handler for Timer signals in US_UNACK_XFER state.
===
===  DEPENDENCIES
===    ul struct
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_handle_tmr_event_us_unack_xfer ( gas_id_t gas_id )
{
  rm_timer_msg_t  *timer_msg_ptr;
  uint16          psn;

  timer_msg_ptr = (rm_timer_msg_t *)q_get(&UTQ(gas_id));

  if (timer_msg_ptr == NULL)
  {
    MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1139, "GRLU U timer event without msg");
  }
  else
  {
    if (timer_msg_ptr->sig == RM_TMR_T3182_AWAIT_UL_ACKNACK)
    {
      ul[gas_id].timer_t3182_expiry_cnt++;

      MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1140, "GRLU U Expy T3182 NoAcknack %u",ul[gas_id].timer_t3182_expiry_cnt);

      /* Handle T3182 expiry during UNACK_RELEASE.
      ** Release the current TBF (PDU) as if Paknak were received.
      */

      if (IS_TX_COMPLETE(gas_id))
      {
        rlc_log_ul_statistics_ext(gas_id);
        ul[gas_id].b_access = FALSE;

        psn = grlc_ul_paq_seek_psn_cv0(gas_id, ul[gas_id].vs);
        clr_ul_data(gas_id);

        /* Successful transmission of PDU , set the flag to TRUE
        ** to start ready timer
        */
        grlc_ul_del_free_paq_upto_psn(gas_id, psn,TRUE);
        grlc_ul_unput_paq(gas_id);

        /* do release
        */
        ul[gas_id].state = US_NULL;
        UPD_UL_STATE(gas_id);

        /* Inform MAC of RLC initiated ul release as if an FAI had been seen
        */
        indicate_ul_release ( gas_id, REL_UL_NORMAL );

        rlc_log_ul_release_ind(gas_id, (uint8)ul[gas_id].cur_res.ul_tfi,
                                RLC_LOG_UL_EXPIRED_T3182_IN_UNACK_RELEASE);

        ul[gas_id].realloc_state = RALS_NULL;
        (void)load_cur_pdu(gas_id);
        UPD_UL_SUBSTATE(gas_id);

      } /* T3182 expiry during UNACK_XFER TX complete */
      else
      /* handle T3182 expiry during UNACK_XFER
      */
      {
        /* when this timer expires the N3104 counter should be decremented
        ** and then the MS should perform an abnormal release with access
        ** retry. However, if N3102 expires (reaches 0) then the MS should
        ** perform an abnormal release with cell reselection as a priority.
        ** Therefore, check for this before doing access with retry.
        */
        if (IS_N3102_ENABLED(gas_id))
        {
          DEC_N3102(gas_id);
#ifdef  GSM_GPRS_GRLU_SHOW_N3102
          MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1141, "GRLU U DecN3102=%d inc=%d dec=%d",ul[gas_id].n3102,PAN_INC(gas_id),PAN_DEC(gas_id));
#endif
          if (IS_N3102_EXPIRED(gas_id))
          {
            MSG_GRLC_MED_3_G(GRLC_F3_MSG_ENUM_1142, "GRLU U N3102 Expy = %d inc=%d dec=%d",
                    ul[gas_id].n3102,
                    PAN_INC(gas_id),
                    PAN_DEC(gas_id));

            rlc_log_ul_statistics_ext(gas_id);
            rlc_log_ul_release_ind(gas_id, (uint8)ul[gas_id].cur_res.ul_tfi,
                                    RLC_LOG_UL_EXPIRED_N3102_IN_UNACK_TRANSFER);

            /* Inform MAC of RLC initiated ul release
            */
           indicate_ul_release ( gas_id, REL_UL_ABNORMAL_CELL_RESEL );

            /* Cell reselection requested. Two actions are required:
            **
            ** (1). Set N3102 -
            **      Set it to the currently held max value again in case same
            **      cell is reselected.
            **
            ** (2). Delete GMM PDUs -
            **      To avoid RAU PDU accumulation which occurs due to not
            **      receiving ack/nack for the sent RAU PDUs hence T3182/N3102
            **      expiry and resulting reselection request to mac but
            **      returning to the same cell repeatedly.
            */

            SET_N3102(gas_id);

            grlc_ul_del_signalling_pdus(gas_id, NO_NAS_INDICATION);

            handle_ul_release_event(gas_id, REL_UL_ABNORMAL_CELL_RESEL, FALSE);
          }
          else
          {
            rlc_log_ul_statistics_ext(gas_id);
            rlc_log_ul_release_ind(gas_id, (uint8)ul[gas_id].cur_res.ul_tfi,
                                    RLC_LOG_UL_EXPIRED_T3182_IN_UNACK_TRANSFER);

            /* Inform MAC of RLC initiated ul release with n3102 status
            ** maintained
            */
            indicate_ul_release ( gas_id, REL_UL_ABNORMAL_WITH_ACCESS_RETRY );
            handle_ul_release_event(gas_id, REL_UL_ABNORMAL_WITH_ACCESS_RETRY, FALSE);
          }
        } /* N3102 expiry */
        else
        {
          rlc_log_ul_statistics_ext(gas_id);
          rlc_log_ul_release_ind(gas_id, (uint8)ul[gas_id].cur_res.ul_tfi,
                                  RLC_LOG_UL_EXPIRED_T3182_IN_UNACK_TRANSFER);

          /* Inform MAC of RLC initiated ul release with n3102 status
          ** maintained
          */
          indicate_ul_release ( gas_id, REL_UL_ABNORMAL_WITH_ACCESS_RETRY );
          handle_ul_release_event(gas_id, REL_UL_ABNORMAL_WITH_ACCESS_RETRY, FALSE);
        }
      } /* T3182 expiry during UNACK_XFER */
    } /* end T3182 */
    else if (timer_msg_ptr->sig == RM_TMR_AWAIT_MAC_UL_RESP)
    {
      MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1143, "GRLU U Expy MAC_RESP");
      unack_mode_xfer(gas_id);
    }

    GPRS_MEM_FREE(timer_msg_ptr);

  } /* timer_msg_ptr not NULL */

} /* grlc_ul_handle_tmr_event_us_unack_xfer */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_MAC_EVENT_US_TEST_MODE()
===
===  DESCRIPTION
===
===    Handler for MAC signals in US_TEST_MODE state.
===
===  DEPENDENCIES
===    ul struct
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_handle_mac_event_us_test_mode ( gas_id_t gas_id )
{
  mac_rlc_msg_t *mac_msg_ptr;

  mac_msg_ptr = (mac_rlc_msg_t *)q_get(&UMQ(gas_id));

  if (mac_msg_ptr == NULL)
  {
    MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1144, "GRLU Test Mode rcvd MAC event without msg");
  }
  else
  {
    if (mac_msg_ptr->msg.sig == MR_UL_RESET_REQ)
    {
      grlc_ul_panic_reset(gas_id);
    }
    else
    if (mac_msg_ptr->msg.sig == MR_NO_PS_ACCESS_IND)
    {
      rlc_log_ul_statistics_ext(gas_id);
      ul[gas_id].b_access = FALSE;

      (void)gllc_grr_ul_tbf_rel_ind(gas_id);

      /* Clear data structures relating to current TBF.
      ** Clear tx window, clear all resources granted,
      ** keep current and outstanding pdus, clear all
      ** signal masks
      */
      clr_ul_data(gas_id);

      /* Back to NULL state */
      ul[gas_id].state = US_NULL;

      /* End of UL tbf for Test mode */
      ul[gas_id].test_mode = TEST_MODE_OFF;
    }
    else
    if (mac_msg_ptr->msg.sig == MR_UL_RELEASE_IND)
    {
      rlc_log_ul_statistics_ext(gas_id);
      ul[gas_id].ul_release_cause = mac_msg_ptr->msg.ul_release_ind.cause;
      ul[gas_id].test_mode_ul_tbf_rel_after_fai = TRUE;

      MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1145, "GRLU Test Mode Rcvd REL_IND cause %d",ul[gas_id].ul_release_cause);
      rlc_log_ul_release_ind(gas_id, (uint8)ul[gas_id].cur_res.ul_tfi,RLC_LOG_UL_MAC_RELEASE_IND);
    }
    else if( mac_msg_ptr->msg.sig == MR_UL_PKT_ACKNACK_IND)
    {
      if (ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS)
      {
        MSG_GRLC_EXT_MED_2_G(GRLC_F3_MSG_ENUM_1146, "ERLU TEST MODE paknak FA=%d SSN=%d",
                mac_msg_ptr->msg.ul_pkt_acknack_ind.egprs_acknack_desc.fai,
                mac_msg_ptr->msg.ul_pkt_acknack_ind.egprs_acknack_desc.ssn
                );

        ul[gas_id].egprs_acknack_desc = mac_msg_ptr->msg.ul_pkt_acknack_ind.egprs_acknack_desc;

        /* If FAI is set or release indication has received previously with cause normal release,
        ** then release the UL TBF. Note that NW should have sent FAI after normal release, but
        ** instead of waiting for FAI just release the UL TBF. TBC.
        */
        if( (ul[gas_id].egprs_acknack_desc.fai == RM_ACKNACK_FAI_TBF_COMPLETE) ||
            (ul[gas_id].test_mode_ul_tbf_rel_after_fai == TRUE ))
        {
          rlc_log_ul_statistics_ext(gas_id);
          /* Inform MAC of RLC initiated ul release
          */
          indicate_ul_release ( gas_id, REL_UL_ABNORMAL_WITHOUT_RETRY );
          (void)gllc_grr_ul_tbf_rel_ind(gas_id);

          /* Clear data structures relating to current TBF.
          ** Clear tx window, clear all resources granted,
          ** keep current and outstanding pdus, clear all
          ** signal masks
          */
          clr_ul_data(gas_id);

          /* Back to NULL state */
          ul[gas_id].state = US_NULL;

          /* End of UL tbf for Test mode */
          ul[gas_id].test_mode = TEST_MODE_OFF;
          ul[gas_id].b_access = FALSE;
        } /* FAI */

        /* Log EPUAN
        ** Log Code: 0x5209
        */
        rlc_log_epuan(gas_id, &(mac_msg_ptr->msg.ul_pkt_acknack_ind));

      } /* EGPRS UNACK EPUAN */
      else
      {
        MSG_GRLC_EXT_MED_2_G(GRLC_F3_MSG_ENUM_1147, "GRLU TEST MODE paknak FA=%d SSN=%d",
                mac_msg_ptr->msg.ul_pkt_acknack_ind.acknack_desc.fai,
                mac_msg_ptr->msg.ul_pkt_acknack_ind.acknack_desc.ssn
                );

        /* GPRS UNACK PUAN in EGPRS build
        */
        ul[gas_id].ul_acknack = mac_msg_ptr->msg.ul_pkt_acknack_ind.acknack_desc;

        /* If FAI is set or release indication has received previously with cause normal release,
        ** then release the UL TBF. Note that NW should have sent FAI after normal release, but
        ** instead of waiting for FAI just release the UL TBF. TBC.
        */
        if( (ul[gas_id].ul_acknack.fai == RM_ACKNACK_FAI_TBF_COMPLETE) ||
            (ul[gas_id].test_mode_ul_tbf_rel_after_fai == TRUE ))
        {
          rlc_log_ul_statistics_ext(gas_id);
          /* Inform MAC of RLC initiated ul release
          */
          indicate_ul_release ( gas_id, REL_UL_ABNORMAL_WITHOUT_RETRY );
          (void)gllc_grr_ul_tbf_rel_ind(gas_id);

          /* Clear data structures relating to current TBF.
          ** Clear tx window, clear all resources granted,
          ** keep current and outstanding pdus, clear all
          ** signal masks
          */
          clr_ul_data(gas_id);

          /* Back to NULL state */
          ul[gas_id].state = US_NULL;

          /* End of UL tbf for Test mode */
          ul[gas_id].test_mode = TEST_MODE_OFF;

          /* Consider the TBF released. Now load the next PDU and establish new TBF
          ** otherwise go back to NULL
          */
          ul[gas_id].b_access = FALSE;
        } /* FAI */
      }  /* GPRS UNACK EPUAN */
    } /* PKT_ACKNACK */
    else if (mac_msg_ptr->msg.sig == MR_UL_ALLOC_CNF)
    {
      rlc_log_ul_config(&mac_msg_ptr->msg.ul_alloc_cnf, gas_id);

      if (mac_msg_ptr->msg.ul_alloc_cnf.access_phase == GMAC_TWO_PHASE_ACCESS)
      {
        /* Send TBF_ACT_IND to LLC for 2 phase */
        grlc_ll_tbf_act_ind (gas_id, TBF_UL);
      }

      /* Refresh sysinfo during transfer
      */
      read_ul_sysinfo(gas_id);

      MSG_GRLC_EXT_MED_0_G(GRLC_F3_MSG_ENUM_1148, "GRLU Test Mode rcved alloc_cnf");
      ul[gas_id].b_first_fifo_load = TRUE;

      if(ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS)
      {
        /* Update coding scheme for engg_mode
        */
        if (ul[gas_id].cur_res.cs != mac_msg_ptr->msg.ul_alloc_cnf.mcs)
        {
          geran_eng_mode_data_write(ENG_MODE_EGPRS_UL_MCS, &mac_msg_ptr->msg.ul_alloc_cnf.mcs, TRUE, gas_id);
        }

        ul[gas_id].cur_res    = mac_msg_ptr->msg.ul_alloc_cnf;
        ul[gas_id].cur_res.cs = mac_msg_ptr->msg.ul_alloc_cnf.mcs;
      }
      else
      {
        /* Update coding scheme for engg_mode
        */
        if (ul[gas_id].cur_res.cs != mac_msg_ptr->msg.ul_alloc_cnf.cs)
        {
          geran_eng_mode_data_write(ENG_MODE_GPRS_UL_CS, &mac_msg_ptr->msg.ul_alloc_cnf.cs, TRUE, gas_id);
        }

        ul[gas_id].cur_res    = mac_msg_ptr->msg.ul_alloc_cnf;
      }

    }
    else
    {
      MSG_GRLC_ERROR_1_G(GRLC_F3_MSG_ENUM_1149, "GRLU Test Mode rcvd ivalid MAC sig %d", mac_msg_ptr->msg.sig);
    }

    GPRS_MEM_FREE(mac_msg_ptr);

  } /* mac_msg_ptr != NULL */

} /* grlc_ul_handle_mac_event_us_test_mode */

/*===========================================================================
===
===  FUNCTION      RLC_UL_STATE_MACHINE()
===
===  DESCRIPTION
===
===    RLC uplink state machine.
===
===    RLC uplink event handler calls this function when an external
===    uplink event occurs.
===
===    This function acts as a front-end, it deals with the events that can
===    occur in each state. Some limited processing is also done in here.
===    Other functions are distributed in functions such as ack_mode_xfer()
===    unack_mode_xfer() and ack_release() and unack_release()
===
===  DEPENDENCIES
===
===    est_tbf_cnf -- same info as in MAC's tbf est cnf mesage
===
===  RETURN VALUE
===
===
===  SIDE EFFECTS
===
===    xxx.
===
===========================================================================*/
static void rlc_ul_state_machine ( gas_id_t gas_id )
{
  rm_timer_msg_t  *timer_msg_ptr;

  /*---------------------------------------------------------------*/

  /* Process and show debug messages suppressed during ISR (post_tx_actions)
  ** in task-time
  */
  GRLC_UL_SHOW_POST_TX_BLKS(gas_id);

  MSG_GRLC_LOW_3_G(GRLC_F3_MSG_ENUM_1150, "GRLU Info s=%d ll-pdus=%d ll-octs=%lu",ul[gas_id].state,ul[gas_id].llc_pdus,ul[gas_id].llc_octets);

  /* No-data recovery code
  */
  if (grlc_ul_no_data_ctr[gas_id] >= GRLC_UL_NO_DATA_THRESHOLD)
  {
    /* Reset counter so that the dumping will continue
    */

    MSG_GRLC_ERROR_3_G(GRLC_F3_MSG_ENUM_1151, "GRLU ##nodata %u ext-utbf=%u pre-emptive-tx=%u",
                        GRLC_UL_NO_DATA_THRESHOLD,
                        IS_EXT_UTBF_ACTIVE(gas_id),
                        ul[gas_id].pre_emptive_tx);

    GRLC_UL_NO_DATA_LOCK(gas_id);
    grlc_ul_no_data_ctr[gas_id] = 0;
    GRLC_UL_NO_DATA_UNLOCK(gas_id);

    /* Dump GRLC vars to QXDM in task time
    */
    grlc_ul_show_vars(gas_id);
  }

  /* Check trigger for a flow control update
  */
  if (ul[gas_id].b_adj_flow_control)
  {
    grlc_ul_adjust_flow_control_thresholds(gas_id);

    /* Done adjusting flow control, then reset the flag
    */
    ul[gas_id].b_adj_flow_control = FALSE;
  }

  switch(ul[gas_id].state)
  {
    /*==================================================================================
    **  N U L L    S T A T E
    **==================================================================================
    */
    case US_NULL:
      /*--------------------------------
      ** State:US_NULL, event = LLC
      ** Expect GRR primitives from LLC
      **--------------------------------
      */
      if (ul[gas_id].event == UE_LLC)
      {
        /* LLC calls enqueue_pdu() directly so there is no message here.
        ** As a precaution, check the access state and act on it
        */
        if (ul[gas_id].b_access)
        {
          (void)load_cur_pdu(gas_id);
          est_tbf_from_null(gas_id);
        }
      }
      else if (ul[gas_id].event == UE_PL1)
      /*------------------------------
      ** State:US_NULL, event = PL1
      **------------------------------
      */
      {
        /* PL1 only sets a Rex mask. No message body
        */
        MSG_GRLC_LOW_0_G(GRLC_F3_MSG_ENUM_1152, "GRLU US_NULL Unexp PL1 sig");
      }
      else if (ul[gas_id].event == UE_MAC)
      /*------------------------------
      ** State:US_NULL, event = MAC
      **------------------------------
      */
      {
        grlc_ul_handle_mac_event_us_null(gas_id);
      }
      else if (ul[gas_id].event == UE_TMR)
      {
        grlc_ul_handle_tmr_event_us_null(gas_id);
      }
      else
      {
        MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1153, "GRLU NULL Unexp evt %d",ul[gas_id].event);
      }
      break;

    /*==================================================================================
    **  A W A I T    A S S I G N M E N T    S T A T E
    **==================================================================================
    */
    case US_AWAIT_ASS:

      if (ul[gas_id].event == UE_MAC)
      {
        grlc_ul_handle_mac_event_us_await_ass(gas_id);
      } /* end MAC event */

      else if (ul[gas_id].event == UE_LLC)
      /*-----------------------------------
      **  State: US_AWAIT_ASS, event = LLC
      **-----------------------------------
      */
      {
        /* LLC already enqueued PDU and RLC waits until a TBF is active
        */
        ;
      }
      else if (ul[gas_id].event == UE_TMR)
      /*-------------------------------------
      **  State: US_AWAIT_ASS, event = TIMER
      **-------------------------------------
      */
      {
        timer_msg_ptr = (rm_timer_msg_t *)q_get(&UTQ(gas_id));
        if (timer_msg_ptr == NULL)
        {
          MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1154, "GRLU AWAIT_ASS tmr event without msg");
        }
        else
        {
          if (timer_msg_ptr->sig == RM_TMR_AWAIT_ASS_GUARD)
          {
            MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1155, "GRLU AWAIT_ASS Expy AWAIT_ASS_GUARD = %u",
                      ul[gas_id].timer_await_ass_guard_expiry_cnt);

            ul[gas_id].state = US_AWAIT_RECOVERY;

            /* If PANIC RESET RESTART is enabled perform soft recovery otherwise
            ** do an ERR_FATAL to stop where it occurs.
            */
            if( geran_get_nv_recovery_restart_enabled(gas_id))
            {
              /* Invoke Panic Reset
              */
              geran_initiate_recovery_restart(GERAN_CLIENT_GRLC, gas_id);
            }
            else
            {
#ifdef FEATURE_QSH_MDUMP
              QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN, QSH_CLT_GRLCUL, QSH_CLT_GRLCUL)
#endif
              ERR_GERAN_FATAL_0_G("GRLU RLC AWAIT_ASS Expy AWAIT_ASS_GUARD Expiry");
            }
          }
          else
          if (timer_msg_ptr->sig == RM_TMR_AWAIT_MAC_UL_RESP)
          {
            MSG_GRLC_HIGH_0_G(GRLC_F3_MSG_ENUM_1156, "GRLU AWAIT_ASS Expy MAC_RESP");
          }
          else
          {
            MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1157, "GRLU AWAIT_ASS Unexp TMR sig %d",timer_msg_ptr->sig);
          }

          GPRS_MEM_FREE(timer_msg_ptr);
        }
      }
      else if (ul[gas_id].event == UE_PL1)
      /*-----------------------------------
      **  State: US_AWAIT_ASS, event = PL1
      **-----------------------------------
      */
      {
        MSG_GRLC_LOW_0_G(GRLC_F3_MSG_ENUM_1158, "GRLU AWAIT_ASS Unexp PL1 sig");
      }
      else
      /*------------------------------------------
      **  State: US_AWAIT_ASS, event = unexpected
      **------------------------------------------
      */
      {
        MSG_GRLC_ERROR_1_G(GRLC_F3_MSG_ENUM_1159, "GRLU AWAIT_ASS Unexp event %d",ul[gas_id].event);
      }
      break;

    /*==================================================================================
    **  A C K N O W L E D G E D    T R A N S F E R    S T A T E
    **==================================================================================
    */
    case US_ACK_XFER:

      if (ul[gas_id].event == UE_PL1)
      {
#ifdef  FEATURE_GPRS_PS_HANDOVER
        if (ul[gas_id].b_psho_active)
        {
          /* Ignore PH_READY_IND from PL1 in PSHO
          */
          MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1160, "GRLU A Ignore PH_READY_TO_SEND in PSHO");
        }
        else if (ul[gas_id].b_psho_llc_pdu_absent)
        {
          /* in case of WtoG handover , cur_pdu_ptr will be null for a
          ** short period of time until we get rau_req or cell_update.
          */
          MSG_GRLC_HIGH_0_G(GRLC_F3_MSG_ENUM_1161, "GRLU A PSHO cur_pdu_ptr NULL PH_READY_TO_SEND Ignored");
        }
        else
        {
          /* This event represents the PH_READY_TO_SEND_IND primitive from PL1.
          ** In response we invoke the ack mode transfer function to produce
          ** data blocks to feed the UL FIFO
          */
          MSG_GRLC_LOW_0_G(GRLC_F3_MSG_ENUM_1162, "GRLU A Rcvd PH_READY_TO_SEND");

          if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
          {
            e_ack_mode_xfer(gas_id);
          }
          else
          {
            ack_mode_xfer(gas_id);
          }
        }
#else   /* FEATURE_GPRS_PS_HANDOVER  */
        /* This event represents the PH_READY_TO_SEND_IND primitive from PL1.
        ** In response we invoke the ack mode transfer function to produce
        ** data blocks to feed the UL FIFO
        */
        MSG_GRLC_LOW_0_G(GRLC_F3_MSG_ENUM_1163, "GRLU A Rcvd PH_READY_TO_SEND");

        if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
        {
          e_ack_mode_xfer(gas_id);
        }
        else
        {
          ack_mode_xfer(gas_id);
        }
#endif /* FEATURE_GPRS_PS_HANDOVER  */
      }
      else if (ul[gas_id].event == UE_MAC)
      /*--------------------------------
      **  State: ACK_XFER, event = MAC
      **--------------------------------
      */
      {
        grlc_ul_handle_mac_event_us_ack_xfer(gas_id);
      }

      else if (ul[gas_id].event == UE_LLC)
      /*--------------------------------
      **  State: ACK_XFER, event = LLC
      **--------------------------------
      */
      {
        MSG_GRLC_LOW_0_G(GRLC_F3_MSG_ENUM_1164, "GRLU XFER UE_LLC");

#ifdef  FEATURE_GPRS_PS_HANDOVER
        if(ul[gas_id].b_psho_llc_pdu_absent)
        {
          /* Coming out of PSHO_COMPLETE_IND cur_pdu_ptr may not be loaded.
          ** this could mostly happen in WtoG PSHO or when only DL active.
          */
          (void)load_cur_pdu(gas_id);
          grlc_ul_handle_alloc_cnf_await_ass(gas_id);
        }
        else
        {
          /* LLC already enqueued the PDU so carry on with the transfer
          */
          if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
          {
            e_ack_mode_xfer(gas_id);
          }
          else
          {
            ack_mode_xfer(gas_id);
          }
        }
#else /* FEATURE_GPRS_PS_HANDOVER */
        /* LLC already enqueued the PDU so carry on with the transfer
        */
        if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
        {
          e_ack_mode_xfer(gas_id);
        }
        else
        {
          ack_mode_xfer(gas_id);
        }
#endif /* FEATURE_GPRS_PS_HANDOVER */
      }
      else if (ul[gas_id].event == UE_TMR)
      /*-----------------------------------
      **  State: US_ACK_XFER, event = TMR
      **-----------------------------------
      */
      {
        grlc_ul_handle_tmr_event_us_ack_xfer(gas_id);
      }
      break;

#ifdef  FEATURE_GSM_DTM
    /*==================================================================================
    **  G P R S   T R A N S P A R E N T   T R A N S F E R   P R O T O C O L   S T A T E
    **==================================================================================
    */
    case US_GTTP_XFER:

      if (ul[gas_id].event == UE_MAC)
      {
        grlc_ul_handle_mac_event_us_gttp_xfer(gas_id);
      }
      else
      {
        MSG_GRLC_LOW_1_G(GRLC_F3_MSG_ENUM_1165, "GRLU GTTP Unexp event %u",ul[gas_id].event);
      }
      break;
#endif /* FEATURE_GSM_DTM */

    /*==================================================================================
    **  U N A C K N O W L E D G E D    T R A N S F E R    S T A T E
    **==================================================================================
    */
    case US_UNACK_XFER:
      /*---------------------------------------------------------------------
      ** Expect signals from:
      ** LLC:
      ** - UNITDATA/DATA_REQ more pdus to get queue'd up
      ** MAC:
      ** - packet uplink acknack
      ** - realloc confirmation
      ** - unsolicited realloc indicator
      ** L1:
      ** - PH_DATA_READY_TO_SEND_IND or blocks just taken out of ul fifo.
      ** TMR:
      **---------------------------------------------------------------------
      */
      if (ul[gas_id].event == UE_PL1)
      {
#ifdef  FEATURE_GPRS_PS_HANDOVER
        if (ul[gas_id].b_psho_active)
        {
          /* Ignore PH_READY_IND from PL1 in PSHO
          */
          MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1166, "GRLU A Ignore PH_READY_TO_SEND in PSHO");
        }
        else if (ul[gas_id].b_psho_llc_pdu_absent)
        {
          /* in case of WtoG handover , cur_pdu_ptr will be null for a
          ** short period of time until we get rau_req or cell_update.
          */
          MSG_GRLC_HIGH_0_G(GRLC_F3_MSG_ENUM_1167, "GRLU U PSHO cur_pdu_ptr NULL PH_READY_TO_SEND Ignored");
        }
        else
        {
          /* deal with PH_DATA_READY_TO_SEND primitive from PL1 by invoking unack
          ** mode transfer function to produce data blocks to send to the RLC UL/
          ** PL1 FIFO
          */
          MSG_GRLC_LOW_0_G(GRLC_F3_MSG_ENUM_1168, "GRLU U Rcvd PH_READY_TO_SEND_IND");

          if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
          {
            e_unack_mode_xfer(gas_id);
          }
          else
          {
            unack_mode_xfer(gas_id);
          }
        }
#else   /* FEATURE_GPRS_PS_HANDOVER  */
        /* deal with PH_DATA_READY_TO_SEND primitive from PL1 by invoking unack
        ** mode transfer function to produce data blocks to send to the RLC UL/
        ** PL1 FIFO
        */
        MSG_GRLC_LOW_0_G(GRLC_F3_MSG_ENUM_1169, "GRLU U Rcvd PH_READY_TO_SEND_IND");

        if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
        {
          e_unack_mode_xfer(gas_id);
        }
        else
        {
          unack_mode_xfer(gas_id);
        }
#endif /* FEATURE_GPRS_PS_HANDOVER */
      } /* UNACK_XFER UE_PL1 */

      else if (ul[gas_id].event == UE_MAC)
      /*-------------------------------------
      **  State: US_UNACK_XFER, event = MAC
      **-------------------------------------
      */
      {
        grlc_ul_handle_mac_event_us_unack_xfer(gas_id);
      }

      else if (ul[gas_id].event == UE_LLC)
      /*-------------------------------------
      **  State: US_UNACK_XFER, event = LLC
      **-------------------------------------
      */
      {
        MSG_GRLC_LOW_0_G(GRLC_F3_MSG_ENUM_1170, "ERLU U Rcvd UE_LLC");

#ifdef  FEATURE_GPRS_PS_HANDOVER
        if(ul[gas_id].b_psho_llc_pdu_absent)
        {
          /* Coming out of PSHO_COMPLETE_IND cur_pdu_ptr may not be loaded.
          ** this could mostly happen in WtoG PSHO or when only DL active.
          */
          (void)load_cur_pdu(gas_id);

          grlc_ul_handle_alloc_cnf_await_ass(gas_id);
        }
        else
        {
          if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
          {
            e_unack_mode_xfer(gas_id);
          }
          else
          {
            unack_mode_xfer(gas_id);
          }
        }
#else /* FEATURE_GPRS_PS_HANDOVER */
        if( ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS )
        {
          e_unack_mode_xfer(gas_id);
        }
        else
        {
          unack_mode_xfer(gas_id);
        }
#endif /* FEATURE_GPRS_PS_HANDOVER */
      }
      else if (ul[gas_id].event == UE_TMR)
      /*-------------------------------------
      **  State: US_UNACK_XFER, event = TMR
      **-------------------------------------
      */
      {
        grlc_ul_handle_tmr_event_us_unack_xfer(gas_id);
      } /* TMR event */
      break;

    /*==================================================================================
    **  T E S T    M O D E    S T A T E
    **==================================================================================
    */
    case US_TEST_MODE:

      if (ul[gas_id].event == UE_PL1)
      {
        /*-------------------------------------
        **  State: US_TEST_MODE, event = PL1
        **-------------------------------------
        */
        ul[gas_id].vs = 0; /* reset index */
      }
      else if (ul[gas_id].event == UE_MAC)
      {
        /*-------------------------------------
        **  State: US_TEST_MODE, event = MAC
        **-------------------------------------
        */
        grlc_ul_handle_mac_event_us_test_mode(gas_id);
      }

      else if (ul[gas_id].event == UE_LLC)
      /*-------------------------------------
      **  State: US_TEST_MODE, event = LLC
      **-------------------------------------
      */
      {
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1171, "GRLU Test Mode rcvd ivalid LLC sig");
      }

      else if (ul[gas_id].event == UE_TMR)
      /*-------------------------------------
      **  State: US_TEST_MODE, event = TMR
      **-------------------------------------
      */
      {
        timer_msg_ptr = (rm_timer_msg_t *)q_get(&UTQ(gas_id));
        if (timer_msg_ptr == NULL)
        {
          MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1172, "GRLU Test Mode rcvd timer event without msg");
        }
        else
        {
          MSG_GRLC_ERROR_1_G(GRLC_F3_MSG_ENUM_1173, "GRLU Test Mode rcvd Inval timer sig %d",timer_msg_ptr->sig);
          GPRS_MEM_FREE(timer_msg_ptr);
        }
      }
      else
      {
        MSG_GRLC_ERROR_1_G(GRLC_F3_MSG_ENUM_1174, "GRLU Test Mode rcvd Inval UL EVENT %d",ul[gas_id].event);
      }
      break; /* US_TEST_MODE */

    /*==================================================================================
    **  A W A I T    R E C O V E R Y    S T A T E
    **==================================================================================
    */
    case US_AWAIT_RECOVERY:
      if (ul[gas_id].event ==UE_PL1)
      /*-----------------------------------------
      **  State: US_AWAIT_RECOVERY, event = PL1
      **-----------------------------------------
      */
      {
        /* No real messages from PL1
        */
        RLC_UL_CLR_PL1_SIG(gas_id);
      }
      else
      if (ul[gas_id].event == UE_MAC)
      /*-----------------------------------------
      **  State: US_AWAIT_RECOVERY, event = MAC
      **-----------------------------------------
      */
      {
        mac_rlc_msg_t *mac_msg_ptr;
        mac_msg_ptr = (mac_rlc_msg_t *)q_get(&UMQ(gas_id));

        if (mac_msg_ptr != NULL)
        {
          if (mac_msg_ptr->msg.sig == MR_UL_RESET_REQ)
          {
            grlc_ul_panic_reset(gas_id);
          }
          else
          {
            MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1175, "GRLU AWAIT_RECOVERY MAC sig %d", mac_msg_ptr->msg.sig);
          }

          GPRS_MEM_FREE(mac_msg_ptr);
        } /* end MAC msg not NULL */
        else
        {
          MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1176, "GRLU US_NULL event without msg");
        }
      }
      else if (ul[gas_id].event == UE_LLC)
      /*-----------------------------------------
      **  State: US_AWAIT_RECOVERY, event = LLC
      **-----------------------------------------
      */
      {
        /* We shall flush the messages later
        */
        RLC_UL_CLR_LLC_PDU_SIG(gas_id);
        RLC_UL_CLR_LLC_TMP_PDU_SIG(gas_id);
        RLC_UL_CLR_LLC_MSG_SIG(gas_id);
      }
      else if (ul[gas_id].event == UE_TMR)
      /*-----------------------------------------
      **  State: US_AWAIT_RECOVERY, event = TMR
      **-----------------------------------------
      */
      {
        timer_msg_ptr = (rm_timer_msg_t *)q_get(&UTQ(gas_id));
        if (timer_msg_ptr != NULL)
        {
          MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1177, "GRLU AWAIT_RECOVERY clr timer sig %d", timer_msg_ptr->sig);
          GPRS_MEM_FREE(timer_msg_ptr);
        }
      }
      break;


    default:
      /* illegal RLC UL state
      */
      MSG_GRLC_ERROR_1_G(GRLC_F3_MSG_ENUM_1178, "GRLC invalid state %d",ul[gas_id].state);
      break;

  } /* ul[gas_id].state switch */

#ifdef  GSM_GPRS_GRLU_SHOW_PAQ
  grlc_ul_show_paq(gas_id);
#endif

} /* rlc_ul_state_machine */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_PANIC_RESET
===
===  DESCRIPTION
===
===    Perform a warm reset on the RLC UL state machine. This procedure
===    frees and clears all un-delivered PDUs and incoming messages
===
===  DEPENDENCIES
===
===  RETURN VALUE
===
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
static void grlc_ul_panic_reset ( gas_id_t gas_id )
{
  /* pointer to buffer holding message intended for MAC
  */
  rlc_mac_msg_t   *rlc_msg_ptr;

  /*---------------------------------------------------------------------*/

  ul[gas_id].b_access = FALSE;

  TIMER_STOP_STALE_PDU_DELETE(gas_id);
  ul[gas_id].b_ul_pdu_delete_after_stale_tmr_expiry = FALSE;

  GRLC_UL_MSG_LOCK(gas_id);

  /* Clear all uplink variables, timers, FIFO, state/sub-state vars
  */
  clr_ul_data(gas_id);

  /* unput all the pdus in paq to the main queue
  */
  grlc_ul_unput_paq(gas_id);

  /* Clear PDU grouping info
  */
  pg_clr_tbc_info(gas_id, &pg[gas_id].tbci);

  /* Clear Diagnostic and statistic info
  */
  grlc_diag_clr_ul_statistics(gas_id);
  grlc_diag_clr_ul_abnormal_release_counts(gas_id);
  grlc_diag_clr_ul_event_counts(gas_id);

  /* Read out all PDUs in the queue and free all DSM items and clear the messages.
     No STATUS_IND sent if SM PDU is deleted, hence FALSE. */

  flush_llc_pdu_queue( gas_id, FALSE );

  /* Read out all LLC signalling messages and free them
  */
  flush_llc_sig_queue(gas_id);

  /* Read out all remaining MAC messages and free them
  */
  flush_mac_sig_queue(gas_id);

  /* Read out all remaining TIMER messages and free them
  */
  flush_tmr_sig_queue(gas_id);

  /* Allow other tasks to run now
  */
  GRLC_UL_MSG_UNLOCK(gas_id);

  ++ul[gas_id].panic_reset_cnt;
  MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1179, "GRLU Resetting %u",ul[gas_id].panic_reset_cnt);

  /* Send Reset Confirmation to MAC
  */
  rlc_msg_ptr = mac_get_cmd_buf_rlc();
  if (rlc_msg_ptr != NULL)
  {
    rlc_msg_ptr->msg.ul_reset_cnf.sig = RM_UL_RESET_CNF;
    rlc_msg_ptr->msg.ul_reset_cnf.gas_id = gas_id;
    mac_put_cmd_rlc(gas_id, rlc_msg_ptr);
  }
  else
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN, QSH_CLT_GRLCUL, QSH_CLT_GRLCUL)
#endif
    ERR_GERAN_FATAL_0_G("ERLU GRLD heap exhaustion");
  }

  /* Re-assess flow control given that PDUs may have been deleted
  */
  grlc_llc_ul_flow_control(gas_id);

} /* grlc_ul_panic_reset */

/*****************************************************************************
***
***     Public Functions
***
*****************************************************************************/

/*===========================================================================
===
===  FUNCTION      GRLC_UL_ONE_TIME_INIT
===
===  DESCRIPTION
===
===    Power up init of rlc downlink operation.
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===    none
===
===  SIDE EFFECTS
===
===    none
===
===========================================================================*/
void grlc_ul_one_time_init (gas_id_t gas_id)
{


  /* Initialise GRLC UL critical sections */

  rex_init_crit_sect(&grlc_ul_msg_crit_sec[gas_id]);

  rex_init_crit_sect(&grlc_ul_no_data_crit_sec[gas_id]);

  rex_init_crit_sect(&grlc_ul_tx_vars_crit_sec[gas_id]);

  rex_init_crit_sect(&grlc_ul_procedure_crit_sec[gas_id]);

#ifdef FEATURE_GRLC_STORE_F3_MSG
  grlc_f3_msg_init();
#endif /* FEATURE_GRLC_STORE_F3_MSG */

  ul[gas_id].b_error_recovery_required = FALSE;

  if (ul[gas_id].b_done_ul_one_time_init == FALSE)
  {
    /* clear PAQ
    */
    grlc_ul_init_paq(gas_id);

    pg[gas_id].psn = 0;

#ifdef DEBUG_GSM_GPRS_RLC_TEST
    #error code not present
#else
/* If FEATURE_MODEM_RCINIT is defined, then cannot access rr_read_public_store as GSM_RR task may not
   have completed initialisation - read_ul_sysinfo() will be accessed when really required anyway*/
#endif

#ifdef  FEATURE_GPRS_PS_HANDOVER
    ul[gas_id].b_psho_active = FALSE;
    ul[gas_id].b_psho_llc_pdu_absent = FALSE;
#endif /* FEATURE_GPRS_PS_HANDOVER */
    ul[gas_id].b_access = FALSE;
    ul[gas_id].b_done_ul_one_time_init = TRUE;
    ul[gas_id].panic_reset_cnt = 0;

    /* Define and init four RLC ul event queues from LLC, MAC and TIMER
    */
    (void)q_init(&ULQ(gas_id));
    (void)q_init(&ULTQ(gas_id));
    (void)q_init(&ULMQ(gas_id));
    (void)q_init(&UMQ(gas_id));
    (void)q_init(&UTQ(gas_id));
    /* Existing data PDUs go here
    */
    (void)q_init(&grlc_ul_old_llc_q[gas_id]);


    /* Define all the general purpose (signal sent on expiry) timers
    */
    ul[gas_id].timer_mac_ul_resp.state = TMR_INACTIVE;
    rex_def_timer_ex(&(ul[gas_id].timer_mac_ul_resp.rex_timer),
                     (rex_timer_cb_type)grlc_ul_timer_callback,
                     ((uint32)TIMER_SET_PARAM(gas_id, RM_TMR_AWAIT_MAC_UL_RESP))
                    );

    ul[gas_id].timer_t3182.state = TMR_INACTIVE;
    ul[gas_id].timer_t3182_expiry_cnt = 0;
    rex_def_timer_ex(&(ul[gas_id].timer_t3182.rex_timer),
                     (rex_timer_cb_type)grlc_ul_timer_callback,
                     ((uint32)TIMER_SET_PARAM(gas_id, RM_TMR_T3182_AWAIT_UL_ACKNACK))
                    );

    ul[gas_id].timer_await_ul_acknack.state = TMR_INACTIVE;
    ul[gas_id].timer_await_ul_acknack_expiry_cnt = 0;
    rex_def_timer_ex(&(ul[gas_id].timer_await_ul_acknack.rex_timer),
                     (rex_timer_cb_type)grlc_ul_timer_callback,
                     ((uint32)TIMER_SET_PARAM(gas_id, RM_TMR_AWAIT_UL_ACKNACK))
                    );

    ul[gas_id].timer_await_ass_guard.state = TMR_INACTIVE;
    ul[gas_id].timer_await_ass_guard_expiry_cnt = 0;
    rex_def_timer_ex(&(ul[gas_id].timer_await_ass_guard.rex_timer),
                     (rex_timer_cb_type)grlc_ul_timer_callback,
                     ((uint32)TIMER_SET_PARAM(gas_id, RM_TMR_AWAIT_ASS_GUARD))
                    );

    ul[gas_id].b_ul_pdu_delete_after_stale_tmr_expiry = FALSE;
    ul[gas_id].timer_stale_pdu_delete.state = TMR_INACTIVE;
    ul[gas_id].timer_stale_pdu_delete_expiry_cnt = 0;
    rex_def_timer_ex(&(ul[gas_id].timer_stale_pdu_delete.rex_timer),
                     (rex_timer_cb_type)grlc_ul_timer_callback,
                     ((uint32)TIMER_SET_PARAM(gas_id, RM_TMR_STALE_PDU_DELETE))
                    );

    /*----------------------------------------------------------------------
    ** if block timers service isr is not provided by the loopback (GDSTST)
    ** envir or the DSPE envir or the actual PL1 code then register a
    ** callback to do it
    **----------------------------------------------------------------------
    */
#ifdef  DEBUG_GSM_GPRS_DS_UNIT_TEST
    #error code not present
#endif /* DEBUG_GSM_GPRS_DS_UNIT_TEST */

    /* Init RLC UL FIFO
    */
    grlc_ul_fifo_one_time_init(gas_id);

    /* set this counter assuming that cell reselection had just taken place
    */
    SET_N3102(gas_id);
#ifdef  GSM_GPRS_GRLU_SHOW_N3102
    MSG_GRLC_EXT_MED_3_G(GRLC_F3_MSG_ENUM_1180, "GRLU OTInit SetN3102=max=%d inc=%d dec=%d",ul[gas_id].n3102,PAN_INC(gas_id),PAN_DEC(gas_id));
#endif

    /* .. put other one time initialisations here .. */

    ul[gas_id].flow_control_suspend = 0;
    ul[gas_id].psn_success_ind = 0;

    /* Use default coding scheme 1 until PUAN says otherwise
    */
    ul[gas_id].cur_res.cs = RM_CS_1;
    ul[gas_id].tlli_cs = RM_CS_1;

    /* Clear internal logging vars (additional to Diag's) every TBF
    */
    ul[gas_id].nack_cnt = 0;
    ul[gas_id].ptx_cnt = 0;
    ul[gas_id].pack_cnt = 0;
    ul[gas_id].num_ests = 0;

    /* Not in SUSPENSION initially
    */
    ul[gas_id].suspend_state = GRLC_RESUME;

    /* Init Diag-related vars
    */
    ul_diag_stats_st[gas_id].grlc_ul_state = (uint32)US_NULL;
    ul_diag_stats_st[gas_id].grlc_ul_substate = (uint32)RALS_NULL;

    /* Clear Diagnostic info
    */
    grlc_diag_clr_ul_statistics(gas_id);
    grlc_diag_clr_ul_abnormal_release_counts(gas_id);
    grlc_diag_clr_ul_event_counts(gas_id);

    /*------------------------------------------------------------------------
    ** Init LLC flow control info once at power up.
    **------------------------------------------------------------------------
    */
    ul[gas_id].llc_octets = 0;
    ul[gas_id].llc_pdus   = 0;
    ul[gas_id].susp_state_data_pdus = 0;

    ul[gas_id].pfi = RM_PFI_INVALID;

    ul[gas_id].prev_realloc_psn = 0;

    /* EGPRS specific inits here
    */

    /* Set the tbf_mode to GPRS as default
    */
    ul[gas_id].cur_res.tbf_mode = TBF_MODE_GPRS;

    /* .. Put other EGPRS inits here
    */

    /* ..E_NEW_DATA_BLOCK() */
  }

#ifdef GSM_GPRS_GRLU_CONFIRM_TX
  grlc_ul_init_post_tx_show_fifo(gas_id);
#endif /* GSM_GPRS_GRLU_CONFIRM_TX */

#ifdef FEATURE_QSH_EVENT_METRIC
  ul[gas_id].metric_dump_ptr = NULL;
  ul[gas_id].metric_context_id = 0;
  ul[gas_id].b_metrics_active = FALSE;
#endif

#ifdef FEATURE_QSH_DUMP
  ul[gas_id].dump_tag_ptr = NULL;
#endif


} /* grlc_ul_one_time_init() */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_INIT
===
===  DESCRIPTION
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===    xxx.
===
===  SIDE EFFECTS
===
===    return value written to module var cv.
===
===========================================================================*/
void grlc_ul_init (gas_id_t gas_id)
{
  /*------------------------------------------------*/

  clr_ul_data(gas_id);

  /* clear current and next pdu pointers
  */
  cur_pdu_ptr[gas_id] = NULL;
  next_pdu_ptr[gas_id] = NULL;

  /* load defaults
  */
  ul[gas_id].event = UE_NULL;
  ul[gas_id].rlc_mode = GRLC_MODE_UNACK;

  ul[gas_id].test_mode_ul_tbf_rel_after_fai = FALSE;

  ul[gas_id].last_valid_coding_scheme = ul[gas_id].cur_res.cs;

  ul[gas_id].re_tx_cv_0_cnt = 0;
  ul[gas_id].octets_req = 0;
  ul[gas_id].tbc = 0;
  ul[gas_id].blocks_req = 0;
  ul[gas_id].blocks_rem = 0;

  ul[gas_id].cv = BS_CV_CONST;

  ul[gas_id].psn_success_ind = 0;

  /* Ensure no reallocation when validity is in doubt
  */
  ul[gas_id].ptx_cv = BS_CV_CONST;
  ul[gas_id].pak_cv = BS_CV_CONST;

  ul[gas_id].b_forced_release = FALSE;

  ul[gas_id].b_first_fifo_load = TRUE;

  ul[gas_id].contention_status = RM_CONTENTION_UNRESOLVED;
  ul[gas_id].b_first_blk_sent = FALSE;

  ul[gas_id].realloc_state = RALS_NULL;
  UPD_UL_SUBSTATE(gas_id);

  ul[gas_id].si = RM_TX_NOT_STALLED;

  ul[gas_id].pfi = RM_PFI_INVALID;

  /* init current block
  */

  /* init tx vars
  */
  ul[gas_id].vs = 0;
  UPD_UL_VS(gas_id);

  ul[gas_id].va = 0;
  UPD_UL_VA(gas_id);

  ul[gas_id].vsf = 0;
  UPD_UL_VSF(gas_id);

  ul[gas_id].nack_vsf = 0;
  UPD_UL_NACK_VSF(gas_id);

  ul[gas_id].pack_vsf = 0;
  UPD_UL_PACK_VSF(gas_id);

  ul[gas_id].nack_cnt = 0;
  ul[gas_id].ptx_cnt = 0;
  ul[gas_id].pack_cnt = 0;
  ul[gas_id].num_ests = 0;

  /* Clear USF info relating to uplink flow control
  */
  ul[gas_id].usf_info.total_usfs = 0;
  ul[gas_id].usf_info.total_blocks_blanked = 0;
  ul[gas_id].usf_info.num_consecutive_blanked_blocks = 0;
  ul[gas_id].usf_info.blanking_percentage_average = 0;
  ul[gas_id].b_adj_flow_control = FALSE;

  ul[gas_id].temp_queue_octet_cnt = 0;

#ifdef GSM_GPRS_GRLU_CONFIRM_TX
  grlc_ul_init_post_tx_show_fifo(gas_id);
#endif /* GSM_GPRS_GRLU_CONFIRM_TX */

} /* grlc_ul_init */


/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_NO_PS_ACCESS()
===
===  DESCRIPTION
===
===    Set sub-state to no access and delete PDU as per indicated by the flag.
===    If RLC is in user suspended state then delete all PDUs. Otherwise, delete
===    the current PDU if DEL_ONE is indicated, or delete no PDU if DEL_NONE is
===    indicated
===
===  DEPENDENCIES
===
===  PARAM
===    delete_pdu_flag: DEL_NONE or DEL_ONE
===
===  RETURN VALUE
===    none
===
===  SIDE EFFECTS
===
===========================================================================*/
void grlc_ul_handle_no_ps_access (gas_id_t gas_id, delete_pdu_cause_t flag)
{
  TIMER_STOP_AWAIT_ASS_GUARD(gas_id);

  ul[gas_id].b_access = FALSE;

  /* When cont-res fails, reset N3104
  */
  if (IS_TLLI_REQUIRED(gas_id))
  {
    MSG_GRLC_HIGH_2_G(GRLC_F3_MSG_ENUM_1181, "GRLU CRes failed ResN3104=%d max=%d",ul[gas_id].n3104, N3104_MAX(gas_id));
    RESET_N3104(gas_id);
  }

  if (ul[gas_id].pdu_freeing_state == FREE_CURRENT_PDU)
  {
    /* Successful transmission of PDU , set the flag to TRUE
    ** to start ready timer
    */
    grlc_ul_del_free_paq_upto_psn(gas_id, ul[gas_id].psn_to_free_inclusive,TRUE);
  }

  if (flag == DEL_ONE)
  {
    if (cur_pdu_ptr[gas_id] != NULL)
    {
      /* Delete SM PDU if UE is not in any xfer state
      */
      if ( cur_pdu_ptr[gas_id]->cause == GRR_CAUSE_SM_SIGNALLING &&
          ul[gas_id].state != US_ACK_XFER &&
          ul[gas_id].state != US_UNACK_XFER
#ifdef  FEATURE_GSM_DTM
          && ul[gas_id].state != US_GTTP_XFER
#endif /* FEATURE_GSM_DTM */
         )
      {

        /* Inform NAS when SIG PDU is deleted and not transmitted
        ** so it can be retried quickly, rather than waiting for large
        ** duration NAS timers
        */
        if ((cur_pdu_ptr[gas_id]->cause == GRR_CAUSE_SM_SIGNALLING) &&
            (! is_pdu_transmitted(gas_id, cur_pdu_ptr[gas_id]->psn))
           )
        {
          MSG_GRLC_MED_1_G(GRLC_F3_MSG_ENUM_1182, "GRLU inform SM no ps access - DEL_ONE psn %u", cur_pdu_ptr[gas_id]->psn);
          indicate_to_llc_rlcmac_failure(gas_id, cur_pdu_ptr[gas_id]);
        }

        /* Set the flag to FALSE as PDU transmission failed
        */
        grlc_ul_del_free_paq_upto_psn(gas_id, cur_pdu_ptr[gas_id]->psn,FALSE);
      }
      else
      {
        /* Inform NAS when SIG PDU is deleted  and not fully transmitted
        ** so it can be retried quickly, rather than waiting for large
        ** duration NAS timers
        */
        if ((cur_pdu_ptr[gas_id]->cause == GRR_CAUSE_GMM_SIGNALLING) &&
            (! is_pdu_transmitted(gas_id, cur_pdu_ptr[gas_id]->psn))
           )
        {
          MSG_GRLC_MED_0_G(GRLC_F3_MSG_ENUM_1183, "GRLU inform gmm pdu tx failure to upper layer");
          indicate_to_llc_rlcmac_failure(gas_id, cur_pdu_ptr[gas_id]);
        }

        /* UN-Successful transmission of PDU , set the flag to FALSE
        ** so not to start ready timer
        */
        grlc_ul_del_free_paq_upto_psn(gas_id, cur_pdu_ptr[gas_id]->psn, FALSE);
      }
    }
  }
  else if (flag == DEL_ALL)
  {
    /* Free the current PDU and its backup. Also free the LLC msg and update
    ** the LLC PDU number and octets
    */
    if (cur_pdu_ptr[gas_id] != NULL)
    {
      /* UN-Successful transmission of PDU , set the flag to FALSE
      ** so not to start ready timer
      */
      grlc_ul_del_free_paq_upto_psn(gas_id, cur_pdu_ptr[gas_id]->psn,FALSE);
      cur_pdu_ptr[gas_id] = NULL;
    }

    /* Purge llc_q here. STATUS_IND sent if SM PDU is deleted, hence TRUE. */

    flush_llc_pdu_queue( gas_id, TRUE );

    ul[gas_id].llc_pdus   = 0;
    ul[gas_id].llc_octets = 0;
  }

  /* store the no_ps_access cuase such that it can be used for pdu deletion
  ** under no_service case for the incoming signalling pdu.
  */
  ul[gas_id].no_ps_access_del_pdu_cause = flag;

  /* 8.7.2: abort current TBF and establish new tbf to re-transmit
  ** current and outstanding pdus
  */
  /* Re-init
  */
  clr_ul_data(gas_id);

  /* Restore PDU
  */
  grlc_ul_unput_paq(gas_id);

  ul[gas_id].state = US_NULL;
  UPD_UL_STATE(gas_id);

  /*
  * Re-order signalling pdus to be sent first to NW
  */
  grlc_ul_reorder_pdus(gas_id);

  (void)load_cur_pdu(gas_id);

  grlc_llc_ul_flow_control(gas_id);

  /* Start Stale PDU delete timer. On expiry, all UL PDUs will be
  ** deleted.
  */
  TIMER_START_STALE_PDU_DELETE(gas_id);

  /* Check the Commercial Recovery NV setting, if it is DISABLED
  ** ,ie. LAB TESTING PRE-CS then if the PDU threshold is approaching being full
  ** then flush the PDUs keeping at least one newest PDU (more than 1 is OK)
  */
  grlc_ul_detect_and_flush_backlog_pdus(gas_id);

} /* grlc_ul_handle_no_ps_access */

#if defined(FEATURE_QSH_EVENT_METRIC)
/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANLDE_QSH_SIG()
===
===  DESCRIPTION
===
===    Handler function to process QSH METRIC_CFG callback
===
===  DEPENDENCIES
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===    ul.event,
===
===========================================================================*/
__attribute__((section(".uncompressible.text")))
void grlc_ul_handle_qsh_sig (gas_id_t gas_id)
{
  qsh_client_action_done_s  action_done;

  /* Short-cut to call-back parameters
  */
  qsh_client_cb_params_s *cb_params_ptr = &ul[gas_id].grlc_ul_qsh_cb_param;

  /* Short-cut to metric config parameters
  */
  qsh_client_metric_cfg_s *metric_config_params_ptr = &cb_params_ptr->action_params.metric_cfg;

  if (metric_config_params_ptr->action == QSH_METRIC_ACTION_START)
  {
    if (ul[gas_id].b_metrics_active == FALSE)
    {
      /* Set up start address and metric conext id and set metric event to active
      */
      ul[gas_id].metric_dump_ptr = metric_config_params_ptr->start_addr;
      ul[gas_id].metric_context_id = metric_config_params_ptr->metric_context_id;
      ul[gas_id].b_metrics_active = TRUE;
      if (geran_nv_qsh_debug_enabled())
      {
        MSG_GRLC_HIGH_1_G(GRLC_F3_MSG_ENUM_1184, "GRLU handle_qsh_sig ACTION_START setting b_metrics_active addr=0x%x",
          metric_config_params_ptr->start_addr);
      }
    }
    else
    {
      MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1185, "GRLU handle_qsh_sig b_metrics_active already set");
      RLC_UL_CLR_QSH_SIG(gas_id);
    }
  }
  else if (metric_config_params_ptr->action == QSH_METRIC_ACTION_STOP)
  {
    ul[gas_id].b_metrics_active = FALSE;
    if (geran_nv_qsh_debug_enabled())
    {
      MSG_GRLC_HIGH_0_G(GRLC_F3_MSG_ENUM_1186, "GRLU handle_qsh_sig ACTION_STOP inactive");
    } 
  }
  else
  {
    MSG_GRLC_ERROR_1_G(GRLC_F3_MSG_ENUM_1187, "GRLU unexpected ACTION %d", metric_config_params_ptr->action);
  }

  /* Indicate callback action done (once the action has been processed)
  */
  qsh_client_action_done_init(&action_done);
  action_done.cb_params_ptr = cb_params_ptr;
  action_done.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;
  qsh_client_action_done(&action_done);

  RLC_UL_CLR_QSH_SIG(gas_id);

} /* grlc_ul_handle_qsh_sig */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_QSH_METRIC_REPORT
===
===  DESCRIPTION
===
===    Function to fill QSH metric structure pointed to with the internal event struct
===    and report to QSH. Do this when there is an internal event, when receivoing or
===    sending a message
===
===  DEPENDENCIES
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===    ul.event,
===
===========================================================================*/
__attribute__((section(".uncompressible.text")))
void grlc_ul_qsh_metric_report (gas_id_t gas_id)
{
  qsh_client_metric_log_done_s  client_metric_done;    /* data passed to QSH when client has finished */
  grlcul_qsh_metric_data_s      *metric_ptr;           /* ptr to QSH dump loaction for metric data */

  if(ul[gas_id].b_metrics_active == TRUE)
  {
    if (ul[gas_id].metric_dump_ptr != NULL)
    {
      /* Copy the metric data
      */
      metric_ptr = (grlcul_qsh_metric_data_s*)ul[gas_id].metric_dump_ptr;

      metric_ptr->metrics_data_ul.susp_state_data_pdus = ul[gas_id].susp_state_data_pdus;
      metric_ptr->metrics_data_ul.temp_queue_octet_cnt = ul[gas_id].temp_queue_octet_cnt;
      metric_ptr->metrics_data_ul.temp_queue_pdu_cnt = ul[gas_id].temp_queue_pdu_cnt;

    qsh_client_metric_log_done_init(&client_metric_done);
    client_metric_done.log_reason = QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL;
    client_metric_done.metric_context_id =  ul[gas_id].metric_context_id;
    client_metric_done.metric_id = GRLCUL_QSH_METRIC_DATA;
    client_metric_done.client = QSH_CLT_GRLCUL;

      /* QSH retuns pointer to the next metric dump loaction
      */
      ul[gas_id].metric_dump_ptr = qsh_client_metric_log_done(&client_metric_done);
      if (ul[gas_id].metric_dump_ptr == NULL)
      {
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1188, "ERLU qsh metric_dump_ptr NULL on exit");
      }
    }
    else
    {
      MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1189, "ERLU qsh metric_dump_ptr NULL on entry");
    }
  }

} /* grlc_ul_qsh_metric_capture */

#endif /* FEATURE_QSH_EVENT_METRIC */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANLDE_INTERNAL_SIG()
===
===  DESCRIPTION
===
===    Handler function to send PRR Type 2. Because the conditions for sending
===    the PRR is triggered from ISR context in post_tx_actions(). Sending the
===    PRR involves allocating memory is now deferred from happening inside the ISR,
===    by setting the GRLC_UL_ITN_SIG mask, which later causes the PRR to be sent
===    in task time
===
===  DEPENDENCIES
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===    ul[gas_id].event,
===
===========================================================================*/
void grlc_ul_handle_internal_sig ( gas_id_t gas_id )
{
  /* when this function is called the PL1 mask is set in RLC UL space. So
  ** invoke the state machine with the event set to UE_PL1
  */
  ul[gas_id].event = UE_NULL;

  /* Send PRR with parameters from the next PDU
  */
  /* b_cv_zero - is set to TRUE , because we
     have transitioned to
     realloc_state = RALS_CURRENT_PDU_TX_COMPLETED,
     that means we have Data in our Uplink Q.
  */
  if ( ul[gas_id].realloc_pdu_ptr != NULL)
  {

    realloc_for_next_pdu(gas_id, ul[gas_id].realloc_pdu_ptr, FALSE);

    /* Set RALS_COMPLETE once after sending a PRR.
    ** Expects this realloc_state to change when the next current PDU is loaded
    */
    ul[gas_id].realloc_state = RALS_COMPLETE;
  }

  GRLC_UL_MSG_LOCK(gas_id);

  RLC_UL_CLR_ITN_SIG(gas_id);

  GRLC_UL_MSG_UNLOCK(gas_id);

} /* grlc_ul_handle_internal_sig */


/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANLDE_PL1_SIG()
===
===  DESCRIPTION
===
===    Wrapper function to invoke the RLC UL state machine to process all
===    L1 commands in the buffer.
===
===  DEPENDENCIES
===
===    grlc_ul_tmr_q
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===    ul[gas_id].event,
===
===========================================================================*/
void grlc_ul_handle_pl1_sig ( gas_id_t gas_id )
{
  /* when this function is called the PL1 mask is set in RLC UL space. So
  ** invoke the state machine with the event set to UE_PL1
  */
  ul[gas_id].event = UE_PL1;

  /* service the event
  */
  rlc_ul_state_machine(gas_id);

  /* Update diag stuff
  */
  ul_diag_evt_cnts_st[gas_id].pl1_event_cnt++;

  /* clear the event
  */
  ul[gas_id].event = UE_NULL;

  GRLC_UL_MSG_LOCK(gas_id);

  RLC_UL_CLR_PL1_SIG(gas_id);

  GRLC_UL_MSG_UNLOCK(gas_id);

} /* grlc_ul_handle_pl1_sig */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_MAC_SIG()
===
===  DESCRIPTION
===
===    Wrapper function to invoke the RLC UL state machine to process all
===    MAC commands in the buffer.
===
===  DEPENDENCIES
===
===    grlc_ul_mac_q
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===    ul[gas_id].event,
===
===========================================================================*/
void grlc_ul_handle_mac_sig ( gas_id_t gas_id )
{
  /*--------------------------------------------------*/

  ul[gas_id].event = UE_MAC;

  /* the state machine will pick up the MAC signal queue from the UE_MAC
  ** event and processes the message
  */
  rlc_ul_state_machine(gas_id);

  /* Update diag stuff
  */
  ul_diag_evt_cnts_st[gas_id].mac_event_cnt++;

  /* clear the input event after servicing
  */
  ul[gas_id].event = UE_NULL;

  GRLC_UL_MSG_LOCK(gas_id);

  if (q_cnt(&UMQ(gas_id)) == 0)
  {
    RLC_UL_CLR_MAC_SIG(gas_id);
  }

  GRLC_UL_MSG_UNLOCK(gas_id);

} /* grlc_ul_handle_mac_sig */


/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_LLC_TMP_PDU_SIG
===
===  DESCRIPTION
===
===    Wrapper function to loop and copy LLC PDUs from the temporary queue
===    and put into RLC main queue. This is to allow PDUs to be put into RLC's
===    PDU queue in RLC context which means the exsiting function enqueue_pdu()
===    can be invoked without changes except for calls to TASKLOCK/FREE().
===
===    The problem was enqueue_pdu() was called in LLC'c context, so TASKLOCK/FREE
===    was added to lock out RLC from gaining context and operated on the same
===    PDU queue and grouping information
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===    ul[gas_id].event,
===
===========================================================================*/
void grlc_ul_handle_llc_tmp_pdu_sig ( gas_id_t gas_id )
{
  /*--------------------------------------------------*/

  gprs_grr_ul_mes_t *msg_ptr;

  gas_id = check_gas_id(gas_id);

  GRLC_UL_MSG_LOCK(gas_id);

  msg_ptr = (gprs_grr_ul_mes_t *)q_get(&ULTQ(gas_id));

  while ( msg_ptr != NULL )
  {
    /* When the PDU is processed the memory will be freed then
    */
    msg_ptr->gas_id = check_gas_id(msg_ptr->gas_id);

    enqueue_pdu(gas_id, msg_ptr);
    msg_ptr = (gprs_grr_ul_mes_t *)q_get(&ULTQ(gas_id));
  }

  RLC_UL_CLR_LLC_TMP_PDU_SIG(gas_id);

  GRLC_UL_MSG_UNLOCK(gas_id);

} /* grlc_ul_handle_llc_tmp_pdu_sig */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_LLC_PDU_SIG
===
===  DESCRIPTION
===
===    Wrapper function to loop and invoke the RLC UL state machine to
===    process all LLC PDU(s) in the buffer.
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===    ul[gas_id].event,
===
===========================================================================*/
void grlc_ul_handle_llc_pdu_sig ( gas_id_t gas_id )
{
  /*--------------------------------------------------*/

  ul[gas_id].event = UE_LLC;

  /* invoke the ul state machine
  */
  rlc_ul_state_machine(gas_id);

  /* Update diag stuff
  */
  ul_diag_evt_cnts_st[gas_id].llc_event_cnt++;

  /* clear the input event after servicing
  */
  ul[gas_id].event = UE_NULL;

  /* When LLC needs to send an LL PDU it calls RLC function directly to
  ** place the PDU in RLC's queue. It then sets the LLC-to-RLC signal mask.
  ** The signal mask is only required to be set once for a number of PDUs.
  ** Once informed of the presence of a PDU RLC state machine will continue
  ** processing them without further need to be informed of other PDUs.
  ** That is why the mask is always cleared
  */

  GRLC_UL_MSG_LOCK(gas_id);

  RLC_UL_CLR_LLC_PDU_SIG(gas_id);

  GRLC_UL_MSG_UNLOCK(gas_id);

} /* grlc_ul_handle_llc_pdu_sig */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_LLC_MSG_SIG
===
===  DESCRIPTION
===
===    Wrapper function to loop and process all LLC Messages in the messaging
===    buffer.
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===    None
===
===========================================================================*/
void grlc_ul_handle_llc_msg_sig ( gas_id_t gas_id )
{
  gprs_grr_ul_mes_t *msg_ptr;

  gas_id = check_gas_id(gas_id);

  /* Mutex lock /Enter critical section here so that LLC is not able to access
  ** this message queue until RLC completed processing. Not many of following
  ** messages expected any given time.
  */

  GRLC_UL_MSG_LOCK(gas_id);

  msg_ptr = q_get(&ULMQ(gas_id));

  while ( msg_ptr != NULL )
  {
    msg_ptr->gas_id = check_gas_id(msg_ptr->gas_id);

    switch ( msg_ptr -> prim )
    {
    case GRR_SUSPEND_REQ:
      grr_suspend_req(gas_id);
      break;

    case GRR_RESUME_REQ:
      grr_resume_req(gas_id);
      break;

    case GRR_PURGE_SUSPENDED_DATA_REQ:
      grr_purge_suspended_data_req(gas_id);
      break;

    case GRR_PURGE_DATA_REQ:
      grr_purge_data_req(gas_id, msg_ptr->sapi);
      break;

    default:
      MSG_GRLC_ERROR_1_G( GRLC_F3_MSG_ENUM_1190, "GRLU LLC Get: Invalid Prim %d", msg_ptr -> prim);
      break;
    }

    GPRS_MEM_FREE( msg_ptr );

    msg_ptr = q_get(&ULMQ(gas_id));
  }

  RLC_UL_CLR_LLC_MSG_SIG(gas_id);

  GRLC_UL_MSG_UNLOCK(gas_id);

} /* grlc_ul_handle_llc_msg_sig */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_TMR_SIG
===
===  DESCRIPTION
===
===    Wrapper function to loop and invoke the RLC UL state machine to
===    process all UL timer commands in the buffer.
===
===  DEPENDENCIES
===
===    grlc_ul_tmr_q OR UTQ
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===    ul[gas_id].event,
===
===========================================================================*/
void grlc_ul_handle_tmr_sig ( gas_id_t gas_id )
{
  /*--------------------------------------------------*/

  /* mark the event to be serviced
  */
  ul[gas_id].event = UE_TMR;

  /* invoke state with LLC event
  */
  rlc_ul_state_machine(gas_id);

  /* Update diag stuff
  */
  ul_diag_evt_cnts_st[gas_id].tmr_event_cnt++;

  /* clear the input event after servicing
  */
  ul[gas_id].event = UE_NULL;

  GRLC_UL_MSG_LOCK(gas_id);

  if (q_cnt(&UTQ(gas_id)) == 0)
  {
    RLC_UL_CLR_TMR_SIG(gas_id);
  }

  GRLC_UL_MSG_UNLOCK(gas_id);

} /* grlc_ul_handle_tmr_sig */


#if defined(FEATURE_QSH_DUMP)

/*===========================================================================
===
===  FUNCTION      GRLC_UL_QSH_FILL_DUMP_DATA()
===
===  DESCRIPTION
===
===    Populate the QSH dump data
===
===  DEPENDENCIES
===
===    None
===
===  PARAMS
===
===    gas_id_t
===    grlcul_qsh_dump_tag_data_s
===
===  RETURN VALUE
===
===    None
===
===  SIDE EFFECTS
===
===    None
===
============================================================================*/
__attribute__((section(".uncompressible.text")))
void grlc_ul_qsh_fill_dump_data (gas_id_t gas_id, grlcul_qsh_dump_tag_data_s *ptr)
{
  ptr->dump_ul_data.b_done_ul_one_time_init = ul[gas_id].b_done_ul_one_time_init;

  /*-------------------------------------
  ** uplink RLC state machine variables
  **-------------------------------------
  */

  /* current rlc ul state
  */
  ptr->dump_ul_data.state = ul[gas_id].state;

  /*--------------------------------------------------------------------------
  ** Flag set when LLC sends GPRS Suspend signal. Cleared when LLC sends GPRS
  ** Resume signal and at power up
  **--------------------------------------------------------------------------
  */
  ptr->dump_ul_data.suspend_state = ul[gas_id].suspend_state;

  /* Stall Indicator field value.
  */
  ptr->dump_ul_data.si = ul[gas_id].si;

  /* Block sequence number
  */
  ptr->dump_ul_data.bsn = ul[gas_id].bsn;

  /*------------------------------------------------------
  ** transmit flags. Only meaningful during transmission
  **------------------------------------------------------
  */

  /*---------------------
  ** transmit variables
  **---------------------
  */

  /* segment index variable.
  ** Index to the next free location for a radio block to be attached
  ** to the transmit array. It is always greater than or equal to V(S).
  */
  ptr->dump_ul_data.seg_ind = ul[gas_id].seg_ind;


  /* V(S) send state variable.
  ** Denotes the sequence number of the next-in-seq RLC data block to be sent.
  ** Set to zero initially and incremented after each transmission of a
  ** PENDING_TRANSMIT (PTX) data block which has not been transmitted before.
  ** The present of the FIFO means this variable is only incremented after PL1
  ** has performed a Get-access on a data block.
  **
  ** This variable controls the PTX blocks.
  ** The range is between V(A) and In_index.
  */
  ptr->dump_ul_data.vs = ul[gas_id].vs;

  /* V(S)F send state variable which holds the BSN of the next block to be sent
  ** to the RLC-PL1 UL FIFO. This is different to V(S) since V(S) is only
  ** incremented when PL1 performs a Get-access whereas this is incremented
  ** after a block is sent to the FIFO.
  **
  ** The range is between V(S) and seg_ind
  */
  ptr->dump_ul_data.vsf = ul[gas_id].vsf;

  /* V(A) acknowledge state.
  ** In acknowledged mode it holds the BSN value of the oldest RLC data block
  ** that has not been positively ack'ed (NACK or PACK)
  **
  ** In unack mode, it marks the oldest data block sent after a paknak is
  ** received. In this way, we can check the distance between V(A) and V(S)
  ** to detect 'stall' condition so that post_tx_actions() knows when to
  ** start T3182.
  */
  ptr->dump_ul_data.va = ul[gas_id].va;

  /* Send state variable for the pending ack block.
  ** This variable holds the BSN of the next PACK block to be sent to the FIFO.
  ** This is required for RLC to keep track of the next PACK block to send to
  ** the FIFO on different invocations.
  **
  ** This variable covers the un-ack blocks which includes the NACK blocks
  ** hence the range is between V(A) and V(S). When RLC has no new NACK blocks
  ** or PTX blocks to send it re-sends the PACK blocks. When pack_vsf passes V(S)
  ** it wraps back to V(A) again.
  **
  ** Since a PUAN could result in V(A) being advanced, on every invocation
  ** pack_vsf is updated to be in its correct range
  */
  ptr->dump_ul_data.pack_vsf = ul[gas_id].pack_vsf;

  /* Send state variable for the NACK blocks.
  ** This variable holds the BSN of block where the search for the next NACK
  ** block to send to the FIFO starts
  **
  ** The range is between V(A) and V(S).
  */
  ptr->dump_ul_data.nack_vsf = ul[gas_id].nack_vsf;


  /* Starting sequence number (SSN) in the packet ack/nack msg from the n/w.
  */
  ptr->dump_ul_data.ssn = ul[gas_id].ssn;

  /* Re-transmit count for the last block (CV = 0). In unack mode,
  ** when waiting for the FAI to set after sending the last block with
  ** CV = 0, RLC keeps a count of the number of re-transmissions. When the count
  ** reaches 4, no more re-tx takes place and the timer t3182 will expire
  ** causing an abnormal release with random access.
  */
  ptr->dump_ul_data.re_tx_cv_0_cnt = ul[gas_id].re_tx_cv_0_cnt;

  /*-------------------------------
  ** operational status variables
  **-------------------------------
  */

  /* Boolean indicating access condition. FALSE at power up and on receipt of
  ** MR_NO_PS_ACCESS_IND. TRUE on receipt of MR_PS_ACCESS_REQ
  */
  ptr->dump_ul_data.b_access = ul[gas_id].b_access;

  /* uplink release cause as indicated by MAC
  */
  ptr->dump_ul_data.ul_release_cause = ul[gas_id].ul_release_cause;

  /* Current ul rlc mode
  */
  ptr->dump_ul_data.rlc_mode = ul[gas_id].rlc_mode;

  /* Current number of ts assigned.
  */

  /* pre_emptive_tx for egprs
  */
  ptr->dump_ul_data.pre_emptive_tx = ul[gas_id].pre_emptive_tx;

  /* In one-phase access mode, this value should be assigned the coding scheme
  ** as follows:
  ** if alloc_cnf.tlli_block_cs = 0, tlli_cs = RM_CS_1
  ** if alloc_cnf.tlli_block_cs = 1, tlli_cs = alloc_cnf.cs
  **
  ** For EGPRS
  ** if alloc_cnf.tlli_block_cs = 0, tlli_cs = RM_MCS_1
  ** if alloc_cnf.tlli_block_cs = 1, tlli_cs = alloc_cnf.mcs
  */
  ptr->dump_ul_data.tlli_cs = ul[gas_id].tlli_cs;

  /* MCS of the last correctly received data block
  */
  ptr->dump_ul_data.last_valid_mcs = ul[gas_id].last_valid_mcs;

  /* contention resolution status of uplink access.
  ** Only used during one-phase access. When unresolved the tlli field must
  ** be included in rlc uplink data blocks. When resolved the tlli field is
  ** omitted.
  **
  ** Default is unresolved
  */
  ptr->dump_ul_data.contention_status = ul[gas_id].contention_status;

  /* Flag to indicate a block has been transmitted by PL1. This is set to FALSE
  ** at the start of a TBF. It is set by the post_tx_action() function to trigger
  ** RLC to start sending request to re-allocate. Because when RLC is in transfer
  ** MAC and PL1 may not tey reached that state
  */
  ptr->dump_ul_data.b_first_blk_sent = ul[gas_id].b_first_blk_sent;

  /* flag to indicate to L1 to start checking the contents of the UL FIFO
  ** for data blocks. This is set to TRUE at the beginning of a TBF and
  ** after a coding change takes place.
  **
  ** The transmit function loads up the FIFO with data blocks and checks
  ** this flag. If set, it will send L1 the PH_DATA_REQ and clears the
  ** flag. So that L1 only receives the primitive once for the duration
  ** of a TBF, unless there is a coding scheme change.
  */
  ptr->dump_ul_data.b_first_fifo_load = ul[gas_id].b_first_fifo_load;

  /*--------------------------------
  ** countdown procedure variables
  **--------------------------------
  */

  /* current CV (Countdown Value). This value is pre-calculated and loaded
  ** into radio block during segmentation.
  */
  ptr->dump_ul_data.cv = ul[gas_id].cv;

  /* Indicate when a forced release of a TBF is required, eg. NORMAL_RELEASE
  */
  ptr->dump_ul_data.b_forced_release = ul[gas_id].b_forced_release;

  /* bsn of the block with CV value being 0. This is used to index the
  ** block to re-send block with CV = 0 to the peer during release state
  */
  ptr->dump_ul_data.bsn_of_cv_0_block = ul[gas_id].bsn_of_cv_0_block;

  ptr->dump_ul_data.e_bsn_of_cv_0_block = ul[gas_id].e_bsn_of_cv_0_block;

  /* flag to indicate RLC data block with CV = 0 has been sent
  */
  ptr->dump_ul_data.b_cv_0 = ul[gas_id].b_cv_0;

  /* CV value of the previously transmitted PTX block
  */
  ptr->dump_ul_data.ptx_cv = ul[gas_id].ptx_cv;

  /* CV value of the previously transmitted PACK block (for un-ack mode)
  */
  ptr->dump_ul_data.pak_cv = ul[gas_id].pak_cv;

  /*------------------------------
  ** RLC uplink operation timers
  **------------------------------
  */

  /* MS Counter for handling cell-reselection. This is a signed value
  */
  ptr->dump_ul_data.n3102 = ul[gas_id].n3102;

  /* Contention resolution counter
  */
  ptr->dump_ul_data.n3104 = ul[gas_id].n3104;

  ptr->dump_ul_data.timer_await_ul_acknack_expiry_cnt = ul[gas_id].timer_await_ul_acknack_expiry_cnt;
  ptr->dump_ul_data.guard_timer_state = ul[gas_id].guard_timer_state;

  /* stat of t3182 expiry
  */
  ptr->dump_ul_data.timer_t3182_expiry_cnt = ul[gas_id].timer_t3182_expiry_cnt;

  /* Count the number of times this timer has been activated since the last power up
  */
  ptr->dump_ul_data.timer_await_ass_guard_expiry_cnt = ul[gas_id].timer_await_ass_guard_expiry_cnt;

  /* Number of times stale pdu delete timer has expired since power up.
  */
  ptr->dump_ul_data.timer_stale_pdu_delete_expiry_cnt = ul[gas_id].timer_stale_pdu_delete_expiry_cnt;

  /*----------------------------------------
  ** transmit resource related information
  **----------------------------------------
  */

  /* the current reallocation sub state within the xfer state
  */
  ptr->dump_ul_data.realloc_state = ul[gas_id].realloc_state;

  /* PDU sequence number where PRR type 2 should be sent
  */
  ptr->dump_ul_data.realloc_psn = ul[gas_id].realloc_psn;

  /* Number of llc pdu octets received and have not been delivered with
  ** acknowledgement.
  */
  ptr->dump_ul_data.llc_pdus = ul[gas_id].llc_pdus;
  ptr->dump_ul_data.llc_octets = ul[gas_id].llc_octets;
  ptr->dump_ul_data.susp_state_data_pdus = ul[gas_id].susp_state_data_pdus;

  /* Logging counters for each type of blocks transmitted during an uplink TBF
  */
  ptr->dump_ul_data.nack_cnt = ul[gas_id].nack_cnt;
  ptr->dump_ul_data.ptx_cnt = ul[gas_id].ptx_cnt;
  ptr->dump_ul_data.pack_cnt = ul[gas_id].pack_cnt;
  ptr->dump_ul_data.num_ests = ul[gas_id].num_ests;

  /* Flag is set when an error has occurred which requires RLC UL to recover
  ** gracefully by flushing its queue and starting afresh. At present, only
  ** dup item exhaustion will lead to this sort of recovery
  */
  ptr->dump_ul_data.b_error_recovery_required = ul[gas_id].b_error_recovery_required;

  /* Pending ack queue stores pointers to the LLC PDU message that are in xfer and
  ** awaiting acknowledgement from the NW. When a PDU is loaded into 'cur_pdu_ptr',
  ** a copy is entered into this queue. When a PDU is acknowledged by the NW it is
  ** removed from this queue.
  */
  ptr->dump_ul_data.paq_in = ul[gas_id].paq_in;
  ptr->dump_ul_data.paq_out = ul[gas_id].paq_out;
  ptr->dump_ul_data.paq_ctr = ul[gas_id].paq_ctr;
  ptr->dump_ul_data.paq_ctr_max = ul[gas_id].paq_ctr_max;

  /*
  */
  ptr->dump_ul_data.psn_to_free_inclusive = ul[gas_id].psn_to_free_inclusive;

  /* Flag to indicate if LLC is flow-controlled (1)
  */
  ptr->dump_ul_data.flow_control_suspend = ul[gas_id].flow_control_suspend;

  /* Counts the number of panic resets requested by MAC from power up
  */
  ptr->dump_ul_data.panic_reset_cnt = ul[gas_id].panic_reset_cnt;

  /* used to delete incoming signalling pdu under no service case
  */
  ptr->dump_ul_data.no_ps_access_del_pdu_cause = ul[gas_id].no_ps_access_del_pdu_cause;

  /* Set when an additional flow control event is needed to adjust flow control based on tx blankings
  */
  ptr->dump_ul_data.b_adj_flow_control = ul[gas_id].b_adj_flow_control;

  /* The current octet count in the uplink temporary queue. This value and the ''llc_octets' together
  ** determines the flow control
  */
  ptr->dump_ul_data.temp_queue_octet_cnt = ul[gas_id].temp_queue_octet_cnt;
  ptr->dump_ul_data.temp_queue_pdu_cnt = ul[gas_id].temp_queue_pdu_cnt;

  /* uplink abnormal release counts
  */
  ptr->dump_ul_data.access_reject_cnt = ul_diag_abn_rel_cnts_st[gas_id].access_reject_cnt;
  ptr->dump_ul_data.arc_retry_cnt = ul_diag_abn_rel_cnts_st[gas_id].arc_retry_cnt;
  ptr->dump_ul_data.arc_wo_retry_cnt = ul_diag_abn_rel_cnts_st[gas_id].arc_wo_retry_cnt;
  ptr->dump_ul_data.arc_sys_info_cnt = ul_diag_abn_rel_cnts_st[gas_id].arc_sys_info_cnt;

  /* Event Counts
  */
  ptr->dump_ul_data.llc_event_cnt = ul_diag_evt_cnts_st[gas_id].llc_event_cnt;
  ptr->dump_ul_data.mac_event_cnt = ul_diag_evt_cnts_st[gas_id].mac_event_cnt;
  ptr->dump_ul_data.pl1_event_cnt = ul_diag_evt_cnts_st[gas_id].pl1_event_cnt;
  ptr->dump_ul_data.tmr_event_cnt = ul_diag_evt_cnts_st[gas_id].tmr_event_cnt;

  /* PDU & octet counts
  */
  ptr->dump_ul_data.ack_tx_pdu_cnt = ul_diag_stats_st[gas_id].ack_tx_pdu_cnt;
  ptr->dump_ul_data.ack_tx_oct_cnt = ul_diag_stats_st[gas_id].ack_tx_oct_cnt;
  ptr->dump_ul_data.unack_tx_pdu_cnt = ul_diag_stats_st[gas_id].unack_tx_pdu_cnt;
  ptr->dump_ul_data.unack_tx_oct_cnt = ul_diag_stats_st[gas_id].unack_tx_oct_cnt;
  ptr->dump_ul_data.coding_sch_changes = ul_diag_stats_st[gas_id].coding_sch_changes;

  /* General purpose uplink counters
  */
  ptr->dump_ul_data.ul_gen_purpose_count_a = ul_diag_stats_st[gas_id].ul_gen_purpose_count_a;

} /* grlc_ul_qsh_fill_dump_data */


/*===========================================================================
===
===  FUNCTION      GRLC_UL_QSH_GET_DUMP()
===
===  DESCRIPTION
===
===    Get the QSH dump data
===
===  DEPENDENCIES
===
===    ul struct
===
===  RETURN VALUE
===
===    none
===
===  SIDE EFFECTS
===
===    none
===
===========================================================================*/
__attribute__((section(".uncompressible.text")))
void grlc_ul_qsh_get_dump (qsh_client_cb_params_s *param)
{
  qsh_client_action_done_s  client_action_done;
  size_t total_size_written = 0;
  size_t remaining_size = 0;
  gas_id_t gas_id = GERAN_DATA_SPACE_INDEX_1;
  grlcul_qsh_dump_tag_data_s* dump_ptr = NULL;

  remaining_size = param->action_params.dump_collect.dump_iovec.size_bytes;
  dump_ptr = (grlcul_qsh_dump_tag_data_s *)(param->action_params.dump_collect.dump_iovec.addr);

  /* Filling the dump data
  */
  if (QSH_DUMP_TAG_ENABLED(param->action_params.dump_collect.dump_tag_mask, GRLCUL_QSH_DUMP_TAG_DATA))
  {
    gas_id = check_gas_id(GERAN_DATA_SPACE_INDEX_1);

    if(remaining_size >= sizeof(grlcul_qsh_dump_tag_data_s))
    {
      /* get header*/
      qsh_client_dump_tag_hdr_init(
        (qsh_dump_tag_hdr_s*)dump_ptr,
        (qsh_dump_tag_t)GRLCUL_QSH_DUMP_TAG_DATA,
        (size_t)sizeof(grlcul_qsh_dump_tag_data_s)
      );
      
      grlc_ul_qsh_fill_dump_data(gas_id, (grlcul_qsh_dump_tag_data_s*)dump_ptr);
      ++dump_ptr;
      remaining_size -= sizeof(grlcul_qsh_dump_tag_data_s);
      total_size_written += sizeof(grlcul_qsh_dump_tag_data_s);
    }
  }

  if (QSH_DUMP_TAG_ENABLED(param->action_params.dump_collect.dump_tag_mask, GRLCUL_QSH_DUMP_TAG_DATA_2))
  {
    gas_id = check_gas_id(GERAN_DATA_SPACE_INDEX_2);

    if(remaining_size >= sizeof(grlcul_qsh_dump_tag_data_s))
    {
      /* get header*/
      qsh_client_dump_tag_hdr_init(
        (qsh_dump_tag_hdr_s*)dump_ptr,
        (qsh_dump_tag_t)GRLCUL_QSH_DUMP_TAG_DATA_2,
        (size_t)sizeof(grlcul_qsh_dump_tag_data_s)
      );

      grlc_ul_qsh_fill_dump_data(gas_id, (grlcul_qsh_dump_tag_data_s*)dump_ptr);
      ++dump_ptr;
      remaining_size -= sizeof(grlcul_qsh_dump_tag_data_s);
      total_size_written += sizeof(grlcul_qsh_dump_tag_data_s);
    }
  }

  /* closing action
  */
  qsh_client_action_done_init(&client_action_done);
  client_action_done.cb_params_ptr = param;
  client_action_done.params.dump_collect.size_written_bytes = total_size_written;

  /* SYNC since data is returned right away in QSH task */
  client_action_done.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;
  qsh_client_action_done (&client_action_done);

  /* MSG_GERAN_HIGH_1_G ("GRLU qsh_cb written %u in SYNC mode", total_size_written); */

} /* grlc_ul_qsh_get_dump */

#endif /* FEATURE_QSH_DUMP */


#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP) || defined (FEATURE_QSH_MDUMP)

/*===========================================================================
===
===  FUNCTION      GRLC_UL_QSH_CALLBACK()
===
===  DESCRIPTION
===
===    Handles callback actions for all QSH requests.
===    Parese the action to see which structure/sub-id, then send message
===    to the correct task to handle the action
===
===  DEPENDENCIES
===
===    ul struct
===
===  RETURN VALUE
===
===    none
===
===  SIDE EFFECTS
===
===    none
===
===========================================================================*/
#ifndef FEATURE_QSH_MDUMP
__attribute__((section(".uncompressible.text")))
#else
QSH_MDUMP_FN_ATTR
#endif
void grlc_ul_qsh_debug_callback (qsh_client_cb_params_s *param)
{
  gas_id_t gas_id = GERAN_DATA_SPACE_INDEX_1;

  if (param == NULL)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN, QSH_CLT_GRLCUL, QSH_CLT_GRLCUL)
#endif
    ERR_GERAN_FATAL_0("GRLU qsh_debug_cb NULL ptr");
  }

  /* Which action
  */
  switch (param->action)
  {
    case QSH_ACTION_NONE:
    case QSH_ACTION_EVENT_CFG:
    case QSH_ACTION_ANALYSIS:
    case QSH_ACTION_METRIC_TIMER_EXPIRY:
      if (geran_nv_qsh_debug_enabled())
      {
        MSG_GRLC_HIGH_1(GRLC_F3_MSG_ENUM_1191, "GRLC qsh_cb action = %u", param->action);
      }
      break;

    case QSH_ACTION_METRIC_CFG:
      /* Extract and validate the gas_id */
      gas_id = geran_map_nas_id_to_gas_id(param->action_params.metric_cfg.subs_id);

      /* Store param struct for handler to pick up later.
      ** Only one message for METRIC_CFG no need for a queue
      */
      ul[gas_id].grlc_ul_qsh_cb_param = *param;

      if(geran_nv_qsh_debug_enabled())
      {
        MSG_GRLC_HIGH_2_G(GRLC_F3_MSG_ENUM_1192, "GRLU qsh_cb METRIC_CFG action=%u subs_id=%u",
          param->action,
          param->action_params.metric_cfg.subs_id);
      }

      if (!RLC_UL_IS_QSH_SIG_SET(gas_id))
      {
        /* Set mask to service message in RLC task time */
        RLC_UL_SET_QSH_SIG(gas_id);
      }
      else
      {
        MSG_GRLC_ERROR_0_G(GRLC_F3_MSG_ENUM_1193, "GRLU qsh_cb sig already set");
      }

      break;

    case QSH_ACTION_DUMP_COLLECT:
      /* get the dump data and comlpete dump action */
      grlc_ul_qsh_get_dump(param);
      break;

#ifdef FEATURE_QSH_MDUMP
    case QSH_ACTION_MDUMP:
      grlcul_qsh_mdump_collect_data(param);
      break;
#endif

    default:
      MSG_GRLC_ERROR_1(GRLC_F3_MSG_ENUM_1194, "GRLC qsh_cb action oor = %u", param->action);
      break;
  }
} /* grlc_ul_qsh_debug_callback */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_QSH_REGISTER()
===
===  DESCRIPTION
===
===    Register with QSH framework for GPRS RLC Generic Uplink Task
===
===  DEPENDENCIES
===
===    None.
===
===  RETURN VALUE
===
===    None.
===
===  SIDE EFFECTS
===
===    None.
===
===========================================================================*/
__attribute__((section(".uncompressible.text")))
void grlc_ul_qsh_register(void)
{
  qsh_client_reg_s client_reg;

  qsh_client_reg_init(&client_reg);

  qsh_client_metric_cfg_init(grlcul_qsh_metric_cfg_arr, GRLC_QSH_METRIC_UL_COUNT);

  /* SUB1
  */
  grlcul_qsh_metric_cfg_arr[0].id = GRLCUL_QSH_METRIC_DATA;
  grlcul_qsh_metric_cfg_arr[0].sampling_period_ms = 0;
  grlcul_qsh_metric_cfg_arr[0].subs_id = SYS_MODEM_AS_ID_1;
  grlcul_qsh_metric_cfg_arr[0].fifo.element_size_bytes = sizeof(grlcul_qsh_metric_data_s);
  grlcul_qsh_metric_cfg_arr[0].fifo.element_count_total = GRLC_UL_QSH_NUM_TRACE_MSGS;

  /* SUB2
  */
  grlcul_qsh_metric_cfg_arr[1].id = GRLCUL_QSH_METRIC_DATA;
  grlcul_qsh_metric_cfg_arr[1].sampling_period_ms = 0;
  grlcul_qsh_metric_cfg_arr[1].subs_id = SYS_MODEM_AS_ID_2;
  grlcul_qsh_metric_cfg_arr[1].fifo.element_size_bytes = sizeof(grlcul_qsh_metric_data_s);
  grlcul_qsh_metric_cfg_arr[1].fifo.element_count_total = GRLC_UL_QSH_NUM_TRACE_MSGS;

  client_reg.metric_info.metric_cfg_arr_ptr = &grlcul_qsh_metric_cfg_arr[0];
  client_reg.metric_info.metric_cfg_count = GRLC_QSH_METRIC_UL_COUNT;
  client_reg.dump_info.max_size_bytes = sizeof(grlcul_qsh_tc_dump_collect_s) - sizeof(qsh_dump_hdr_s);

  client_reg.client = QSH_CLT_GRLCUL;
  client_reg.major_ver = GRLCUL_QSH_MAJOR_VER ;
  client_reg.minor_ver = GRLCUL_QSH_MINOR_VER;

  client_reg.client_cb_ptr = grlc_ul_qsh_debug_callback;

  client_reg.cb_action_support_mask = 0;
#ifdef FEATURE_QSH_DUMP
  /* Register callback for dumps */
  client_reg.cb_action_support_mask |= QSH_ACTION_DUMP_COLLECT;
#endif /* FEATURE_QSH_DUMP */

#ifdef FEATURE_QSH_EVENT_METRIC
  /* Register callback for metrics */
  client_reg.cb_action_support_mask |= QSH_ACTION_METRIC_CFG;
#endif /* FEATURE_QSH_EVENT_METRIC */

#ifdef FEATURE_QSH_MDUMP
  client_reg.cb_action_support_mask |= QSH_ACTION_MDUMP;
#endif /* FEATURE_QSH_MDUMP */
  qsh_client_reg(&client_reg);

} /* grlc_ul_qsh_register */

#ifdef FEATURE_QSH_MDUMP

/*===========================================================================
FUNCTION  grlcul_qsh_mdump_collect_active

DESCRIPTION
  Function to collect GL1 mini dump.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void grlcul_qsh_mdump_collect_active(gas_id_t gas_id)
{
#ifdef FEATURE_GRLC_STORE_F3_MSG
  grlcul_qsh_mdump_collect_buf_data();
#endif
  qsh_mdump_collect_high(&ul[gas_id], sizeof(grlc_ul_t));
  qsh_mdump_collect_high(&grlc_ul_fifo[gas_id], sizeof(grlc_ul_pl1_fifo_t));
  qsh_mdump_collect_high(&ul_diag_pdu_stat[gas_id], sizeof(grlc_diag_ul_pdu_stat_t));
  qsh_mdump_collect_high(&ul_diag_block_stat[gas_id], sizeof(grlc_diag_ul_block_stat_t));
  qsh_mdump_collect_high(&ul_diag_tx_stat[gas_id], sizeof(grlc_diag_ul_tx_stat_t));
  qsh_mdump_collect_high(&cur_pdu_ptr[gas_id], sizeof(gprs_grr_ul_mes_t*));
  qsh_mdump_collect_high(cur_pdu_ptr[gas_id], sizeof(gprs_grr_ul_mes_t));
  qsh_mdump_collect_high(&next_pdu_ptr[gas_id], sizeof(gprs_grr_ul_mes_t*));
  qsh_mdump_collect_high(next_pdu_ptr[gas_id], sizeof(gprs_grr_ul_mes_t));

  qsh_mdump_collect_high(&p_tx_arr[gas_id], sizeof(grlc_ul_tx_array_t*));
  if (p_tx_arr[gas_id] != NULL)
  {
    if (ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS)
    {
      qsh_mdump_collect_high(p_tx_arr[gas_id], (sizeof(grlc_ul_tx_array_t) * E_TX_WORKSPACE));
    }
    else if (ul[gas_id].cur_res.tbf_mode == TBF_MODE_GPRS)
    {
      qsh_mdump_collect_high(p_tx_arr[gas_id], (sizeof(grlc_ul_tx_array_t) * RM_RLC_TX_WORKSPACE));
    }
  }
}

/*===========================================================================
FUNCTION  grlcul_qsh_mdump_collect_inactive

DESCRIPTION
  Function to collect GL1 mini dump in inactive state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void grlcul_qsh_mdump_collect_inactive(gas_id_t gas_id)
{
  qsh_mdump_collect_high(&ul[gas_id].state, sizeof(rlc_ul_state_t));
  qsh_mdump_collect_high(&ul[gas_id].cur_res.tbf_mode, sizeof(tbf_mode_T));
  qsh_mdump_collect_high(&cur_pdu_ptr[gas_id], sizeof(gprs_grr_ul_mes_t*));
  qsh_mdump_collect_high(cur_pdu_ptr[gas_id], sizeof(gprs_grr_ul_mes_t));
  qsh_mdump_collect_high(&next_pdu_ptr[gas_id], sizeof(gprs_grr_ul_mes_t*));
  qsh_mdump_collect_high(next_pdu_ptr[gas_id], sizeof(gprs_grr_ul_mes_t));

  qsh_mdump_collect_high(&p_tx_arr[gas_id], sizeof(grlc_ul_tx_array_t*));
  if (p_tx_arr[gas_id] != NULL)
  {
    if (ul[gas_id].cur_res.tbf_mode == TBF_MODE_EGPRS)
    {
      qsh_mdump_collect_high(p_tx_arr[gas_id], (sizeof(grlc_ul_tx_array_t) * E_TX_WORKSPACE));
    }
    else if (ul[gas_id].cur_res.tbf_mode == TBF_MODE_GPRS)
    {
      qsh_mdump_collect_high(p_tx_arr[gas_id], (sizeof(grlc_ul_tx_array_t) * RM_RLC_TX_WORKSPACE));
    }
  }
}

QSH_MDUMP_FN_ATTR void grlcul_qsh_mdump_collect_data(qsh_client_cb_params_s *param)
{
  gas_id_t gas_id;

  for (gas_id = 0; gas_id < NUM_GERAN_DATA_SPACES; gas_id++)
  {
    if (ul[gas_id].state != US_NULL)
    {
      grlcul_qsh_mdump_collect_active(gas_id);
    }
    else
    {
      grlcul_qsh_mdump_collect_inactive(gas_id);
    }
  }

  qsh_mdump_prioritize_client(QSH_CLT_GRLCUL, QSH_CLT_GRLCDL);
  qsh_mdump_prioritize_client(QSH_CLT_GRLCUL, QSH_CLT_GMAC);
  qsh_mdump_prioritize_client(QSH_CLT_GRLCUL, QSH_CLT_GL1);
  qsh_mdump_prioritize_client(QSH_CLT_GRLCUL, QSH_CLT_GRR);
  qsh_mdump_prioritize_client(QSH_CLT_GRLCUL, QSH_CLT_GLLC);
}
#endif /* FEATURE_QSH_MDUMP*/
#endif /* FEATURE_QSH_DUMP || FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_MDUMP */

/*===========================================================================
===
===  FUNCTION      GRLC_UL_HANDLE_LOG_SIG
===
===  DESCRIPTION
===
===    Function to process LOG signal.
===
===  DEPENDENCIES
===
===    None
===
===  RETURN VALUE
===
===    None
===
===  SIDE EFFECTS
===
===    None
===
===========================================================================*/
void grlc_ul_handle_log_sig(const gas_id_t gas_id)
{
  if (ul[gas_id].state != US_NULL)
  {
    rlc_log_ul_statistics_ext(gas_id);
  }
  
  RLC_UL_CLR_LOG_SIG(gas_id);
}

/*** EOF: don't remove! ***/
