/*****************************************************************************
***
*** TITLE
***
***  GPRS MAC task
***
*** DESCRIPTION
***
***  Handles the initialisation of the MAC task and the signal reception
***  handler.
***
*** EXTERNALIZED FUNCTIONS
***
***
***
*** INITIALIZATION AND SEQUENCING REQUIREMENTS
***
***
***
*** Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
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
*** $Header: //components/rel/geran.mpss/7.4.0/gmac/src/gmactask.c#5 $
***
*** when       who     what, where, why
*** --------   ---     ------------------------------------------------------
*** 09/11/10   sjw     Updated for Dual SIM support
*** 06/27/01   tlx     Created
*** 03/08/02   pjr     added handling for L1 messages using static memory.
***                    Module affected rex_sigs_type gprs_mac_signal_handler,
***                    added flag free_l1_memory_flag;
*** 10/10/02  DLH      Added a gprs_l1_page_group_flag which is used to inform MAC
***                    from L1 whether the block of data has been received in the MS
***                    paging group or not.
***
*****************************************************************************/

/*****************************************************************************
***
***     Included Files
***
*****************************************************************************/
#include "geran_variation.h"
#include "customer.h"

#include "geran_tasks.h"
#include "gprsdef.h"
#include "gprsdef_g.h"

/* rex and system type operations */
#include "rex.h"
#include "task.h"
#include "rcinit.h"

/* for messaging */
#include "queue.h"
#include "dsm.h"
#include "memory.h"
#include "gprs_mem.h"
#include "string.h"

/* for debuging */
#include "msg.h"
#include "err.h"
#include "amssassert.h"

#include "gmac.h"
#include "gmacdef.h"
#include "gmacsig.h"
#include "gmacnull.h"
#ifdef FEATURE_QSH_MDUMP
#include "gmacidle.h"
#include "gmactransfer.h"
#endif
#include "gmacidle_if.h"
#include "gmactransfer_if.h"
#include "grlcmaccmn.h"
#include "gmacrb.h"
#include "gmactask.h"
#include "gmacutil.h"
#include "gmaclog.h"
#include "gmacl1.h"
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#include "geran_nv.h"
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */
#ifdef PERLUTF
#error code not present
#endif
#include "dog_hb_rex.h"
#include "gmacproc.h"
#include "gmutex.h"
#include "gmacds.h"
#include "stringl/stringl.h"

#if defined (FEATURE_QSH_DUMP) || defined (FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_MDUMP)
#include "qsh.h"
#include "qsh_ext.h"
#include "gmac_qsh_ext.h"
#endif /* FEATURE_QSH_DUMP || FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_MDUMP */

#include "gmacmac.h"
#include "gtmrs_g.h"

/*****************************************************************************
***
***     Private Data Declarations
***
*****************************************************************************/

#ifdef FEATURE_QSH_DUMP
// Define a structure with QSH dump header and all TLV structures (for T32 debugging)
typedef struct
{
  qsh_dump_hdr_s hdr;
  gmac_qsh_dump_tag_data_s gmac_qsh_dump_tag_data;
  gmac_qsh_dump_tag_data_s gmac_qsh_dump_tag_data_2;
} gmac_qsh_tc_dump_collect_s;

// Pointer declared as type of QSH data structure for parser
gmac_qsh_dump_tag_data_s  *gmac_qsh_dump_tag_0_ptr;
gmac_qsh_dump_tag_data_s  *gmac_qsh_dump_tag_1_ptr;
#endif // FEATURE_QSH_DUMP

#ifdef FEATURE_QSH_EVENT_METRIC
// Array of metric config structures (should be kept global)
qsh_ext_metric_cfg_s gmac_qsh_metric_cfg_arr[NUM_GERAN_DATA_SPACES][GMAC_QSH_METRIC_COUNT];
#endif // FEATURE_QSH_EVENT_METRIC

#define GMAC_NUM_TRACE_MSGS 100

typedef struct
{
  boolean next_entry;/* This will tell whether next entry is free or not*/

  uint32 FN;       /*current FN sig was rcvd on*/

  uint8 gmac_mode; /*NULL/IDLE/TRANSFER*/

  uint8 state;/*Corresponding mode state*/

  uint8 message_set;/*Received/sent message set*/

  uint8 message_id;/*Received/sent message ID*/

  uint8 message_frag[3];/*first three bytes of message*/
} gmac_trace_msg_t;

typedef struct
{
  gmac_trace_msg_t gmac_trace_msg_buf[GMAC_NUM_TRACE_MSGS];
  byte gmac_trace_msg_buf_index;

#if defined (FEATURE_QSH_EVENT_METRIC) || defined (FEATURE_QSH_DUMP)
  uint8 *                         saved_cb_metric_dump_ptr;           /* Points to the current QSH metic location alloacted for MAC*/
  qsh_client_metric_context_id_t  qsh_metric_context_id;              /* provided by QSH and is passeed back to QSH when metric data is dumped */
  boolean                         gmac_qsh_metrics_capture_activate;  /* set by the metric action type rcvd from QSH*/
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP  */

} gmac_task_data_t;


static byte tracebuff_ver = 1;

/*task,msg mutex/crit section declaration outside of gmac_task_data to make rex mutex operation efficient*/
/*-----------------------------------------------------------------
** Critical Section to protect GMAC shared vars.
**-----------------------------------------------------------------
*/
  /* Critical section to access control GMAC messages */
  rex_crit_sect_type gmac_msg_crit_sec[NUM_GERAN_DATA_SPACES];

/*****************************************************************************
***
***     Private Function Prototypes
***
*****************************************************************************/

#ifdef PERLUTF
#error code not present
#else
static void gprs_mac_signal_handler(gas_id_t gas_id);
static void gmac_initialise(gas_id_t gas_id);
#endif

static boolean gmac_gprs_dl_ctrl_msg_proceed_validation(uint8 message_type, boolean proceed_flag,gas_id_t gas_id);
static void gmac_gprs_dl_ctrl_msg_validation(uint8 message_type,gas_id_t gas_id);
static void gmac_store_trace_rvcd_msg_buf(uint8 gmac_mode, uint8 state,gas_id_t gas_id);
static boolean gmac_dl_msg_address_data_valid(uint8 sig_id,uint8 tsk_id,gas_id_t gas_id);
static boolean gmac_assignment_unsupported(mac_modes_t mode,uint8 sig_id,uint8 tsk_id,gas_id_t gas_id);
static boolean  gmac_check_q_for_mac_internal_sigs(mac_signals_u* item_ptr,uint8 compare_task_id,gas_id_t gas_id);

static void gmac_log_reshuffling_of_internal_sigs(gas_id_t gas_id);

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR static void gmac_qsh_mdump_collect(qsh_client_cb_params_s *param);
#endif /* FEATURE_QSH_MDUMP */

#if defined (FEATURE_QSH_EVENT_METRIC) || defined (FEATURE_QSH_DUMP) || defined (FEATURE_QSH_MDUMP) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH) || defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER)
__attribute__((section(".uncompressible.text")))
static void gmac_qsh_init(gas_id_t gas_id);
#ifndef FEATURE_QSH_MDUMP
__attribute__((section(".uncompressible.text")))
#else
QSH_MDUMP_FN_ATTR
#endif /* FEATURE_QSH_MDUMP */
static void  gmac_qsh_data_capture_cb(qsh_client_cb_params_s *   qsh_cb_params_ptr);
__attribute__((section(".uncompressible.text")))
static void gmac_qsh_run_dump_action(qsh_client_cb_params_s *   qsh_cb_params_ptr);
__attribute__((section(".uncompressible.text")))
static void gmac_qsh_metric_capture( gas_id_t gas_id, gmac_trace_msg_t *last_saved_rcvd_sent_msg_ptr);
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP || FEATURE_QSH_MDUMP */

/*****************************************************************************
***
***     Public Data Declarations
***
***     Ensure an external reference is added to the module's header file!
***
*****************************************************************************/
gmac_task_data_t gmac_task_data[NUM_GERAN_DATA_SPACES];

/*****************************************************************************
***
***     Private Macros
***
*****************************************************************************/

/*****************************************************************************
***
***     Public Functions
***
*****************************************************************************/
/*===========================================================================
===
===  FUNCTION    gmac_get_task_data_ptr()
===
===  DESCRIPTION
===  Returns a pointer to null data space using the current gas_id
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===    gmac_null_data_t*
===
===  SIDE EFFECTS
===
===
===========================================================================*/
static gmac_task_data_t* gmac_get_task_data_ptr(gas_id_t gas_id)
{
  gmac_task_data_t *task_data_ptr = &gmac_task_data[0];

  /* validate the GAS ID */
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    task_data_ptr = &gmac_task_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)];
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Task bad gas_id:%d",gas_id);
  }

  return task_data_ptr;
}

/*===========================================================================
===
===  FUNCTION    gmac_init_task_data_non_zero()
===
===  DESCRIPTION
===  Initialises non zero (including all enums) task data.
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===
===========================================================================*/
void gmac_init_task_data_non_zero( gas_id_t gas_id )
{
}


/*===========================================================================
===
===  FUNCTION    gmac_init_task_data()
===
===  DESCRIPTION
===  Initialises Task data.  Used for Lower layer resets.
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===
===========================================================================*/
void gmac_init_task_data( gas_id_t gas_id )
{
  /*No Data to reset*/
}

/*===========================================================================
===
===  FUNCTION    gmac_reset_clear_task_queue()
===
===  DESCRIPTION
===  Used for Lower layer resets.  Clears all msgs from the task queue.
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===  SIDE EFFECTS
===
===
===========================================================================*/
void gmac_reset_clear_task_queue( gas_id_t gas_id )
{
  mac_signals_u  *reset_ptr;
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);

  /* gets message and clears it from message queue */
  reset_ptr = q_get(&global_data_ptr->mac_q);

  while(reset_ptr != NULL)
  {
    GPRS_MEM_FREE(reset_ptr);

    /* check message queue for more signals. if msg_ptr returns NULL queue is empty */
    reset_ptr = q_get(&global_data_ptr->mac_q);
  }
}

void gsm_mac_fn_init(gas_id_t gas_id)
{
  /* Only use this mechanism to perform memory init, hardware init,
     or other startup init to satisfy dependency that follow. Avoid
     all HLOS mechanisms that will block. */

  gmac_initialise(gas_id);

  /* Code in this function will add to startup latency. Do not use
     large loops that can be avoided.*/
}

/*===========================================================================
===
===  FUNCTION      gmac_process_task_sigs()
===
===  DESCRIPTION
===    Process the task sigs.
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===
===  SIDE EFFECTS
===
===
===========================================================================*/
static void gmac_process_task_sigs(rex_sigs_type gmac_task_sigs,
                                   dog_report_type dogRpt,
                                   gas_id_t gas_id)
{

  /* Process any active signals with the following priority. */
  if ( gmac_task_sigs & TASK_START_SIG )
  {
    (void)rex_clr_sigs( rex_self(), TASK_START_SIG );
  }

  if ( gmac_task_sigs & TASK_OFFLINE_SIG )
  {
    (void)rex_clr_sigs( rex_self(), TASK_OFFLINE_SIG );
  }

  if ( gmac_task_sigs & GMAC_DOG_RPT_TMR_SIG )
  {
    (void)rex_clr_sigs( rex_self(), GMAC_DOG_RPT_TMR_SIG );

    dog_hb_report(dogRpt);
  }

  if ( gmac_task_sigs & GPRS_MAC_MASTER_SIG )
  {
    /* Launch MAC task signal handler */
    gprs_mac_signal_handler(gas_id);
  }
}

/*===========================================================================
===
===  FUNCTION      GPRS_MAC_TASK()
===
===  DESCRIPTION
===    Launches the MAC task and MAC signal handler
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===
===  SIDE EFFECTS
===
===
===========================================================================*/
void gprs_mac_task
(
  /* Required for REX, ignore. */

  dword unused
)
{
  gas_id_t gas_id = gmac_map_task_id_to_gas_id(GERAN_TASK_ID_GMAC);
  rex_sigs_type  gmac_task_sigs = 0;
  dog_report_type dogRpt;

  /* Put checking here for consistency between Task mapping to gas_id and
     gas_id passed within a signal*/

  NOTUSED(unused); /* silence compiler warnings of unused parameter */

  geran_tcb_set(GERAN_TASK_ID_GMAC, rex_self());

  /* Perform startup init here. Satisfy dependency of other tasks
     provided by this task. Do not linger here, startup latency is
     impacted. Higher priority tasks in the same group will preempt
     this task at this point. Do not rely upon priority alone as the
     sole mechanism to arbitrate resource contention with other tasks. */

  rcinit_handshake_startup(); /* required rcinit handshake */
  /* Optional dog registration here */
  gsm_mac_fn_init(gas_id);



  /* Perform remaining init here. This is sequenced by a start
     signal delivered at the end of the sequence group processing. */
  dogRpt = dog_hb_register_rex((rex_sigs_type)GMAC_DOG_RPT_TMR_SIG);

  /* Begin event based processing via REX scheduling. */
  for (;;)
  {
    /* Recind processor context until a relevent signal(s) becomes active. */
    gmac_task_sigs =  rex_wait
                      (
                        (
                          TASK_START_SIG       |
                          TASK_STOP_SIG        |
                          TASK_OFFLINE_SIG     |
                          GMAC_DOG_RPT_TMR_SIG |
                          GPRS_MAC_MASTER_SIG
                        )
                      );

    if ( gmac_task_sigs & TASK_STOP_SIG )
    {
      (void)rex_clr_sigs( rex_self(), TASK_STOP_SIG );
#ifdef TEST_FRAMEWORK
      #error code not present
#endif /* TEST_FRAMEWORK */
    }

    gmac_process_task_sigs(gmac_task_sigs,dogRpt,gas_id);
  }

  /* Deregister if leaving the task so that we don't ERR_FATAL */
  dog_hb_deregister(dogRpt);

  /*UNREACHED*/
}

#ifdef FEATURE_DUAL_SIM
/*===========================================================================
===
===  FUNCTION      GPRS_MAC_TASK2()
===
===  DESCRIPTION
===    Launches the MAC task and MAC signal handler
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===
===  SIDE EFFECTS
===
===
===========================================================================*/
void gprs_mac_task2
(
  /* Required for REX, ignore. */

  dword unused
)
    {
  gas_id_t gas_id = gmac_map_task_id_to_gas_id(GERAN_TASK_ID_GMAC2);
  rex_sigs_type  gmac_task_sigs = 0;
  dog_report_type dogRpt;

  /* Put checking here for consistency between Task mapping to gas_id and
     gas_id passed within a signal*/

  NOTUSED(unused); /* silence compiler warnings of unused parameter */

  geran_tcb_set(GERAN_TASK_ID_GMAC2, rex_self());

  /* Perform startup init here. Satisfy dependency of other tasks
     provided by this task. Do not linger here, startup latency is
     impacted. Higher priority tasks in the same group will preempt
     this task at this point. Do not rely upon priority alone as the
     sole mechanism to arbitrate resource contention with other tasks. */

  rcinit_handshake_startup(); /* required rcinit handshake */
  /* Optional dog registration here */
  gsm_mac_fn_init(gas_id);



  /* Perform remaining init here. This is sequenced by a start
     signal delivered at the end of the sequence group processing. */
  dogRpt = dog_hb_register_rex((rex_sigs_type)GMAC_DOG_RPT_TMR_SIG);

  /* Begin event based processing via REX scheduling. */
  for (;;)
  {
    /* Recind processor context until a relevent signal(s) becomes active. */
    gmac_task_sigs =  rex_wait
                      (
                        (
                          TASK_START_SIG       |
                          TASK_STOP_SIG        |
                          TASK_OFFLINE_SIG     |
                          GMAC_DOG_RPT_TMR_SIG |
                          GPRS_MAC_MASTER_SIG
                        )
                      );

    if ( gmac_task_sigs & TASK_STOP_SIG )
    {
      (void)rex_clr_sigs( rex_self(), TASK_STOP_SIG );
#ifdef TEST_FRAMEWORK
      #error code not present
#endif /* TEST_FRAMEWORK */
    }

    gmac_process_task_sigs(gmac_task_sigs,dogRpt,gas_id);
  }

  /* Deregister if leaving the task so that we don't ERR_FATAL */
  dog_hb_deregister(dogRpt);

  /*UNREACHED*/
}
#endif /*FEATURE_DUAL_SIM*/



#ifdef FEATURE_TRIPLE_SIM
/*===========================================================================
===
===  FUNCTION      GPRS_MAC_TASK3()
===
===  DESCRIPTION
===    Launches the MAC task and MAC signal handler
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===
===  SIDE EFFECTS
===
===
===========================================================================*/
void gprs_mac_task3
(
  /* Required for REX, ignore. */

  dword unused
)
{
  gas_id_t gas_id = gmac_map_task_id_to_gas_id(GERAN_TASK_ID_GMAC3);
  rex_sigs_type  gmac_task_sigs = 0;
  dog_report_type dogRpt;

  /* Put checking here for consistency between Task mapping to gas_id and
     gas_id passed within a signal*/

  NOTUSED(unused); /* silence compiler warnings of unused parameter */

  geran_tcb_set(GERAN_TASK_ID_GMAC3, rex_self());

  /* Perform startup init here. Satisfy dependency of other tasks
     provided by this task. Do not linger here, startup latency is
     impacted. Higher priority tasks in the same group will preempt
     this task at this point. Do not rely upon priority alone as the
     sole mechanism to arbitrate resource contention with other tasks. */

  rcinit_handshake_startup(); /* required rcinit handshake */
  /* Optional dog registration here */
  gsm_mac_fn_init(gas_id);



  /* Perform remaining init here. This is sequenced by a start
     signal delivered at the end of the sequence group processing. */
  dogRpt = dog_hb_register_rex((rex_sigs_type)GMAC_DOG_RPT_TMR_SIG);

  /* Begin event based processing via REX scheduling. */
  for (;;)
  {
    /* Recind processor context until a relevent signal(s) becomes active. */
    gmac_task_sigs =  rex_wait
                      (
                        (
                          TASK_START_SIG       |
                          TASK_STOP_SIG        |
                          TASK_OFFLINE_SIG     |
                          GMAC_DOG_RPT_TMR_SIG |
                          GPRS_MAC_MASTER_SIG
                        )
                      );

    if ( gmac_task_sigs & TASK_STOP_SIG )
    {
      (void)rex_clr_sigs( rex_self(), TASK_STOP_SIG );
#ifdef TEST_FRAMEWORK
      #error code not present
#endif /* TEST_FRAMEWORK */
    }

    gmac_process_task_sigs(gmac_task_sigs,dogRpt,gas_id);
  }

  /* Deregister if leaving the task so that we don't ERR_FATAL */
  dog_hb_deregister(dogRpt);

  /*UNREACHED*/
}
#else
/* stub for compilation */
void gprs_mac_task3(dword unused)
{
  rcinit_handshake_startup(); /* required rcinit handshake */
  return;
}
#endif /*FEATURE_TRIPLE_SIM*/


/* Normalise naming */
void gsm_mac_task (dword dummy)
{
   gprs_mac_task(dummy);
}

/*===========================================================================
===
===  FUNCTION      gmac_multislot_class_init()
===
===  DESCRIPTION
===  Multislot class parameters to be initialised depending on the
===  current multislot class supported.
===
===  NOTE: This function only supports multislot classes 1 to 12 and shall
===        cause a fatal error if any other value set.
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===
===  SIDE EFFECTS
===
===
===========================================================================*/
void gmac_multislot_class_init(gas_id_t gas_id)
{
  uint8 nv_ms_class;

  uint8 nv_egprs_ms_class;

  uint8 upper_ms_class_limit = 0;

#ifdef FEATURE_GSM_DTM
  uint8 nv_dtm_ms_class; /*used for DTM GPRS and DTM EGPRS MS Classes*/
#endif /*FEATURE_GSM_DTM*/
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);

#ifdef FEATURE_GSM_GPRS_MSC33
  upper_ms_class_limit = RRPS_MULTISLOT_CLASS_33;
#else
  upper_ms_class_limit = RRPS_MULTISLOT_CLASS_12;
#endif /*FEATURE_GSM_GPRS_MSC33*/

  MSG_GERAN_HIGH_1_G("MAC trace buffer Version%d",tracebuff_ver);

  rr_read_public_store( RRPS_MS_MULTISLOT_CLASS, &nv_ms_class, gas_id );

  if ( (nv_ms_class >= RRPS_MULTISLOT_CLASS_1 ) &&
       (nv_ms_class <= upper_ms_class_limit)    )
  {
    MSG_GERAN_HIGH_1_G("GPRS multislot class %d",nv_ms_class);
  }
  else
  {
    nv_ms_class = RRPS_MULTISLOT_CLASS_1;
    MSG_GERAN_HIGH_1_G("GPRS multislot class default %d",nv_ms_class);
  }

  switch (nv_ms_class)
  {
    case RRPS_MULTISLOT_CLASS_1:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_1");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_1;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_1;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS1_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS1_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS1_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS1_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS1_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS1_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_2:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_2");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_2;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_2;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS2_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS2_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS2_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS2_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS2_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS2_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_3:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_3");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_3;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_3;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS3_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS3_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS3_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS3_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS3_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS3_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_4:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_4");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_4;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_4;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS4_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS4_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS4_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS4_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS4_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS4_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_5:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_5");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_5;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_5;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS5_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS5_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS5_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS5_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS5_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS5_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_6:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_6");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_6;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_6;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS6_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS6_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS6_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS6_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS6_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS6_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_7:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_7");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_7;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_7;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS7_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS7_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS7_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS7_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS7_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS7_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_8:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_8");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_8;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_8;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS8_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS8_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS8_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS8_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS8_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS8_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_9:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_9");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_9;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_9;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS9_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS9_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS9_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS9_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS9_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS9_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_10:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_10");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_10;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_10;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS10_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS10_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS10_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS10_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS10_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS10_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_11:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_11");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_11;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_11;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS11_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS11_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS11_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS11_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS11_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS11_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_12:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_12");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_12;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_12;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS12_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS12_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS12_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS12_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS12_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS12_MIN_TS_TRB;
      break;

#ifdef FEATURE_GSM_GPRS_MSC33
    case RRPS_MULTISLOT_CLASS_30:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_30");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_30;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_30;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS30_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS30_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS30_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS30_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS30_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS30_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_31:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_31");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_31;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_31;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS31_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS31_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS31_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS31_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS31_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS31_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_32:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_32");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_32;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_32;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS32_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS32_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS32_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS32_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS32_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS32_MIN_TS_TRB;
      break;

     case RRPS_MULTISLOT_CLASS_33:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_33");
      global_data_ptr->gmac_ms_class = MULTISLOT_CLASS_33;
      global_data_ptr->gmac_ms_class_sum = MULTISLOT_CLASS_SUM_33;
      global_data_ptr->gmac_ms_class_max_ts_tx = MS33_MAX_TS_TX;
      global_data_ptr->gmac_ms_class_max_ts_rx = MS33_MAX_TS_RX;
      global_data_ptr->gmac_ms_class_ts_tta = MS33_MIN_TS_TTA;
      global_data_ptr->gmac_ms_class_ts_ttb = MS33_MIN_TS_TTB;
      global_data_ptr->gmac_ms_class_ts_tra = MS33_MIN_TS_TRA;
      global_data_ptr->gmac_ms_class_ts_trb = MS33_MIN_TS_TRB;
      break;
#endif /*FEATURE_GSM_GPRS_MSC33*/

    default:
      MSG_GERAN_ERROR_1_G("Multislot class NOT VALID %d",nv_ms_class);
      break;
  }

  rr_read_public_store( RRPS_MS_EGPRS_MULTISLOT_CLASS, &nv_egprs_ms_class, gas_id );

  if ( (nv_egprs_ms_class >= RRPS_MULTISLOT_CLASS_1 ) &&
       (nv_egprs_ms_class <= upper_ms_class_limit)    )
  {
    MSG_GERAN_HIGH_1_G("EGPRS multislot class %d",nv_egprs_ms_class);
  }
  else
  {
    nv_egprs_ms_class = RRPS_MULTISLOT_CLASS_1;
    MSG_GERAN_HIGH_1_G("EGPRS multislot class default %d",nv_egprs_ms_class);
  }

  switch (nv_egprs_ms_class)
  {
    case RRPS_MULTISLOT_CLASS_1:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_1");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_1;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_1;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS1_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS1_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS1_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS1_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS1_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS1_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_2:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_2");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_2;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_2;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS2_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS2_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS2_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS2_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS2_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS2_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_3:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_3");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_3;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_3;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS3_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS3_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS3_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS3_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS3_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS3_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_4:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_4");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_4;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_4;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS4_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS4_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS4_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS4_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS4_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS4_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_5:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_5");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_5;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_5;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS5_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS5_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS5_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS5_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS5_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS5_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_6:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_6");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_6;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_6;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS6_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS6_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS6_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS6_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS6_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS6_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_7:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_7");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_7;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_7;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS7_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS7_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS7_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS7_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS7_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS7_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_8:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_8");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_8;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_8;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS8_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS8_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS8_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS8_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS8_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS8_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_9:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_9");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_9;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_9;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS9_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS9_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS9_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS9_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS9_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS9_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_10:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_10");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_10;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_10;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS10_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS10_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS10_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS10_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS10_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS10_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_11:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_11");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_11;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_11;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS11_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS11_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS11_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS11_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS11_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS11_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_12:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_12");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_12;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_12;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS12_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS12_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS12_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS12_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS12_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS12_MIN_TS_TRB;
      break;

#ifdef FEATURE_GSM_GPRS_MSC33
    case RRPS_MULTISLOT_CLASS_30:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_30");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_30;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_30;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS30_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS30_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS30_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS30_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS30_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS30_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_31:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_31");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_31;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_31;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS31_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS31_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS31_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS31_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS31_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS31_MIN_TS_TRB;
      break;

    case RRPS_MULTISLOT_CLASS_32:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_32");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_32;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_32;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS32_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS32_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS32_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS32_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS32_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS32_MIN_TS_TRB;
      break;

     case RRPS_MULTISLOT_CLASS_33:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_33");
      global_data_ptr->gmac_egprs_ms_class = MULTISLOT_CLASS_33;
      global_data_ptr->gmac_egprs_ms_class_sum = MULTISLOT_CLASS_SUM_33;
      global_data_ptr->gmac_egprs_ms_class_max_ts_tx = MS33_MAX_TS_TX;
      global_data_ptr->gmac_egprs_ms_class_max_ts_rx = MS33_MAX_TS_RX;
      global_data_ptr->gmac_egprs_ms_class_ts_tta = MS33_MIN_TS_TTA;
      global_data_ptr->gmac_egprs_ms_class_ts_ttb = MS33_MIN_TS_TTB;
      global_data_ptr->gmac_egprs_ms_class_ts_tra = MS33_MIN_TS_TRA;
      global_data_ptr->gmac_egprs_ms_class_ts_trb = MS33_MIN_TS_TRB;
      break;
#endif /*FEATURE_GSM_GPRS_MSC33*/

    default:
      MSG_GERAN_ERROR_1_G("Multislot class NOT VALID %d",nv_egprs_ms_class);
      break;
  }

#ifdef FEATURE_GSM_DTM
  rr_read_public_store( RRPS_MS_DTM_EGPRS_MULTISLOT_SUBCLASS, &nv_dtm_ms_class, gas_id );

  if ( (nv_dtm_ms_class == RRPS_DTM_EGPRS_MULTISLOT_SUBCLASS_5 ) ||
       (nv_dtm_ms_class == RRPS_DTM_EGPRS_MULTISLOT_SUBCLASS_9 ) ||
       (nv_dtm_ms_class == RRPS_DTM_EGPRS_MULTISLOT_SUBCLASS_11) )
  {
    MSG_GERAN_HIGH_1_G("GMAC DTM multislot class %d",nv_dtm_ms_class);
  }
  else
  {
    nv_dtm_ms_class = RRPS_DTM_EGPRS_MULTISLOT_SUBCLASS_5;
    MSG_GERAN_HIGH_1_G("GMAC DTM multislot class out of range, default %d",nv_dtm_ms_class);
  }

  switch (nv_dtm_ms_class)
  {
    case RRPS_DTM_EGPRS_MULTISLOT_SUBCLASS_5:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_5");
      global_data_ptr->gmac_dtm_ms_class_sum = MULTISLOT_CLASS_SUM_5;
      global_data_ptr->gmac_dtm_ms_class_max_ts_tx = MS5_MAX_TS_TX;
      global_data_ptr->gmac_dtm_ms_class_max_ts_rx = MS5_MAX_TS_RX;
      global_data_ptr->gmac_dtm_ms_class_ts_tta = MS5_MIN_TS_TTA;
      global_data_ptr->gmac_dtm_ms_class_ts_ttb = MS5_MIN_TS_TTB;
      global_data_ptr->gmac_dtm_ms_class_ts_tra = MS5_MIN_TS_TRA;
      global_data_ptr->gmac_dtm_ms_class_ts_trb = MS5_MIN_TS_TRB;
      break;

    case RRPS_DTM_EGPRS_MULTISLOT_SUBCLASS_9:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_9");
      global_data_ptr->gmac_dtm_ms_class_sum = MULTISLOT_CLASS_SUM_9;
      global_data_ptr->gmac_dtm_ms_class_max_ts_tx = MS9_MAX_TS_TX;
      global_data_ptr->gmac_dtm_ms_class_max_ts_rx = MS9_MAX_TS_RX;
      global_data_ptr->gmac_dtm_ms_class_ts_tta = MS9_MIN_TS_TTA;
      global_data_ptr->gmac_dtm_ms_class_ts_ttb = MS9_MIN_TS_TTB;
      global_data_ptr->gmac_dtm_ms_class_ts_tra = MS9_MIN_TS_TRA;
      global_data_ptr->gmac_dtm_ms_class_ts_trb = MS9_MIN_TS_TRB;
      break;

    case RRPS_DTM_EGPRS_MULTISLOT_SUBCLASS_11:
      MSG_GERAN_LOW_0_G("MAC info MULTISLOT_CLASS_11");
      global_data_ptr->gmac_dtm_ms_class_sum = MULTISLOT_CLASS_SUM_11;
      global_data_ptr->gmac_dtm_ms_class_max_ts_tx = MS11_MAX_TS_TX;
      global_data_ptr->gmac_dtm_ms_class_max_ts_rx = MS11_MAX_TS_RX;
      global_data_ptr->gmac_dtm_ms_class_ts_tta = MS11_MIN_TS_TTA;
      global_data_ptr->gmac_dtm_ms_class_ts_ttb = MS11_MIN_TS_TTB;
      global_data_ptr->gmac_dtm_ms_class_ts_tra = MS11_MIN_TS_TRA;
      global_data_ptr->gmac_dtm_ms_class_ts_trb = MS11_MIN_TS_TRB;
      break;

    default:
      MSG_GERAN_ERROR_1_G("Multislot class NOT VALID %d",nv_dtm_ms_class);
      break;
  }
#endif /*FEATURE_GSM_DTM*/
}

/*****************************************************************************
***
***     Private Functions
***
*****************************************************************************/
/*===========================================================================
===
===  FUNCTION      gmac_gprs_dl_ctrl_msg_proceed_validation()
===
===  DESCRIPTION
===    This function will return whether the received control message should pass
===    it to the main processing or not
===
===  DEPENDENCIES
===    This function access mac_mode variable
===
===  RETURN VALUE
===    boolean
===
===  SIDE EFFECTS
===  This function is called only from gprs_mac_signal_handler function
===
===========================================================================*/
static boolean gmac_gprs_dl_ctrl_msg_proceed_validation(uint8 message_type, boolean proceed_flag, gas_id_t gas_id)
{
  boolean mac_to_process_msg_flg = TRUE;
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);

  if(proceed_flag == FALSE)
  {
    mac_to_process_msg_flg = FALSE;
  }
  switch(message_type)
  {
    case PACKET_PSI1:
    case PACKET_PSI2:
    case PACKET_PSI3:
    case PACKET_PSI3_BIS:
    case PACKET_PSI4:
    case PACKET_PSI5:
    case PACKET_PSI6:
    case PACKET_PSI13:
    case PACKET_PSI7:
    case PACKET_PSI8:
    case PACKET_PSI14:
    case PACKET_PSI3_QUATER:
    case PACKET_PSI15:
      /* no need to enter main processing loop */
      mac_to_process_msg_flg = FALSE;
    break;

#ifdef FEATURE_GPRS_PS_HANDOVER
    case PACKET_PS_HANDOVER:
#ifdef PERLUTF
      #error code not present
#endif
    break;
#endif /* FEATURE_GPRS_PS_HANDOVER */

    case PACKET_SERVING_CELL_DATA:
      /* no need to enter main processing loop */
      mac_to_process_msg_flg = FALSE;
    break;

    case PACKET_SERVING_CELL_SI:
      /* no need to enter main processing loop */
      mac_to_process_msg_flg = FALSE;
    break;

    case PACKET_CELL_CHANGE_CONTINUE:
      mac_to_process_msg_flg = FALSE;
    break;

    case PACKET_NEIGHBOUR_CELL_DATA:
      /* no need to enter main processing loop */
      mac_to_process_msg_flg = FALSE;
    break;

    case PACKET_DOWNLINK_DUMMY_CNTRL_BLK:
      if((global_data_ptr->mac_mode == IDLE_MODE) || (global_data_ptr->mac_mode == TRANSFER_MODE))
      {
        /* no need to enter main processing loop */
        mac_to_process_msg_flg = FALSE;
      }
    break;

    default:
    break;
  }
  return(mac_to_process_msg_flg);
}

/*===========================================================================
===
===  FUNCTION      gmac_gprs_dl_ctrl_msg_validation()
===
===  DESCRIPTION
===    This function validates the received dl control message and if the
===    message is for GRR then it forwards the message to GRR
===
===  DEPENDENCIES
===    This function access mac_mode variable
===
===  RETURN VALUE
===    void
===
===  SIDE EFFECTS
===  This functio is called only from gprs_mac_signal_handler function
===
===========================================================================*/
static void gmac_gprs_dl_ctrl_msg_validation(uint8 message_type, gas_id_t gas_id)
{
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);

  switch(message_type)
  {
    case PACKET_PSI1:
    case PACKET_PSI2:
    case PACKET_PSI3:
    case PACKET_PSI3_BIS:
    case PACKET_PSI4:
    case PACKET_PSI5:
    case PACKET_PSI6:
    case PACKET_PSI13:
    case PACKET_PSI7:
    case PACKET_PSI8:
    case PACKET_PSI14:
    case PACKET_PSI3_QUATER:
    case PACKET_PSI15:

      /* send GRR the PSI message, GRR will free CSN.1  dl ctrl msg memory */
      MSG_GERAN_LOW_0_G("MAC Rcvd PSI");
      gmac_send_data_ind_to_grr(gas_id);
      break;

#ifdef FEATURE_GPRS_PS_HANDOVER
    case PACKET_PS_HANDOVER:

      if(global_data_ptr->mac_mode == TRANSFER_MODE)
      {
        if(gmac_tran_check_psho_addressing(gas_id) == TRUE)
        {
          /*PSHO addressing MS are forwarded to GRR GRR will free CSN.1  dl ctrl msg memory */
          free_dl_ctrl_msg = FALSE;
        }
      }
#ifdef PERLUTF
      #error code not present
#endif
    break;
#endif /* FEATURE_GPRS_PS_HANDOVER */

    case PACKET_CELL_CHANGE_ORDER:
    case PACKET_MEASUREMENT_ORDER:
#ifdef FEATURE_GSM_EDTM
    case PACKET_CS_COMMAND:
#endif /*FEATURE_GSM_EDTM*/
    /* Message to GRR is sent from state machine handler */
      break;

    case PACKET_SERVING_CELL_DATA:
      gmac_util_handle_pkt_cell_data(PACKET_SERVING_CELL_DATA,
                                     global_data_ptr->msg_ptr->l1_mac_sig.msg.data_ind.bcch_arfcn,
                                     gas_id);
      break;

    case PACKET_SERVING_CELL_SI:
      gmac_util_send_serving_si(global_data_ptr->csn_dec_result_ptr,
                                global_data_ptr->msg_ptr->l1_mac_sig.msg.data_ind.bcch_arfcn,
                                gas_id);
      break;

    case PACKET_CELL_CHANGE_CONTINUE:
      /* if the TFI matches, forward the PCCC message to GRR */
      if (gmac_util_check_tfi(PACKET_CELL_CHANGE_CONTINUE,gas_id))
      {
        MSG_GERAN_MED_0_G("MAC Rcvd PACKET_CELL_CHANGE_CONTINUE");
        if (global_data_ptr->mac_mode == TRANSFER_MODE)
        {
          if ( gmac_tran_get_tbfs_active(gas_id) == DL_ONLY )
          {
            /*Only a DL TBF is active so set the the PCA type to DL.
              L1 will put this into its DL PCA queue and will gaurantee
              that it will be transmitted when we ab rel for the cell
              resel that follows the PCCC*/
            rrbp_control_block_handler_2(PCA_DL_MSG,gas_id);
          }
          else
          {
            rrbp_control_block_handler_2(PCA_UL_MSG,gas_id);
          }
        }
        gmac_send_data_ind_to_grr(gas_id);
      }
      break;

    case PACKET_NEIGHBOUR_CELL_DATA:
      gmac_util_handle_pkt_cell_data(PACKET_NEIGHBOUR_CELL_DATA,
                                     global_data_ptr->msg_ptr->l1_mac_sig.msg.data_ind.bcch_arfcn,
                                     gas_id);
      break;

    case PACKET_DOWNLINK_DUMMY_CNTRL_BLK:
      break;

    default:
      break;
  }

  return;
}
/*===========================================================================
===
===  FUNCTION      GPRS_MAC_SIGNAL_HANDLER()
===
===  DESCRIPTION
===
===    Retrieves signals form MAC signal queue and uses MAC mode to determine
===    which mode the signals are destined for.
===
===  DEPENDENCIES
===
===  csn_dec_hdr_ptr - global pointer to decoded CSN.1 info header
===
===  RETURN VALUE
===
===    None.
===
===  SIDE EFFECTS
===
===    None.
===========================================================================*/
#ifdef PERLUTF
#error code not present
#else
static void gprs_mac_signal_handler( gas_id_t gas_id )
#endif
{
  boolean       segmented = FALSE;
  uint8         signal_id;
  uint8         task_id;
  boolean       proceed_flag = TRUE;
  /*TLX free floating*/
  uint8         compare_task_id = MS_MAC_MAC;
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);
  gmac_task_data_t   *task_data_ptr   = gmac_get_task_data_ptr(gas_id);

  /*search MAC q for MAC internal signals and process these 1st.                  */
  /*q_linear_search does not remove the sig from the q just returns the pointer   */
  /*If requested item is found then q_linear_search returns the pointer to it     */
  /*otherwise it returns NULL                                                     */

  global_data_ptr->msg_ptr = (mac_signals_u*) (q_linear_search (&global_data_ptr->mac_q,(q_compare_func_type)gmac_check_q_for_mac_internal_sigs,
                                               &compare_task_id));

  /*if MAC internal signal was found fetch it off the Q 1st */
  if(global_data_ptr->msg_ptr == NULL)
  {
    /*No intrenal MAC signal found on Q. Retrieve 1st signal on q*/
    global_data_ptr->msg_ptr = q_get(&global_data_ptr->mac_q);
  }
  else
  {
    /*internal MAC signal found on Q. delete mac sig from q as q_linear_search does not do this.*/
    gmac_log_reshuffling_of_internal_sigs(gas_id);

    q_delete(
              &global_data_ptr->mac_q,
              &(global_data_ptr->msg_ptr->mac_int_sig.link));
  }

/*tlx free floating */
//was  /* gets message and clears it from message queue */
//was   msg_ptr = q_get(&mac_q);

  while(global_data_ptr->msg_ptr != NULL)
  {
    task_id = global_data_ptr->msg_ptr->grr_mac_sig.task_id;
    if (task_id == MS_MAC_L1)
    {
      signal_id = (uint8)global_data_ptr->msg_ptr->l1_mac_sig.sig_id;

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
      if (signal_id == (uint8)L1_MAC_RECOVERY_IND)
      {
        MSG_GERAN_MED_0_G("GMAC received L1_MAC_RECOVERY_IND");

        switch(global_data_ptr->mac_mode)
        {
          case NULL_MODE:
            gmac_store_trace_rvcd_msg_buf((uint8)global_data_ptr->mac_mode, (uint8)gmac_null_get_null_state(gas_id), gas_id);
            break;

          case IDLE_MODE:
            gmac_store_trace_rvcd_msg_buf((uint8)global_data_ptr->mac_mode, (uint8)gmac_idle_get_idle_state(gas_id), gas_id);
            break;

          case TRANSFER_MODE:
            gmac_store_trace_rvcd_msg_buf((uint8)global_data_ptr->mac_mode, (uint8)gmac_transfer_get_transfer_state(gas_id), gas_id);
            break;

          default:
            break;
        }

        if (TRUE == geran_nv_ptm_err_recovery())
        {
          gmac_util_handle_error_recovery(gas_id);
        }
        else
        {
          MSG_GERAN_ERROR_0_G("GMAC PTM_ERR_RECOVERY EFS disabled");
        }
        proceed_flag = FALSE;
      }
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */
    }
    else if (task_id == MS_MAC_RR)
    {
      signal_id = (uint8)global_data_ptr->msg_ptr->grr_mac_sig.sig_id;


      /*page mode hadling moved to  NUll state NULL_INACTIVE for IPA*/
      /* set page mode to re-org when changing cell */
      /* if ((signal_id == (uint8)GRR_MAC_CAMPED_ON_CELL) ||
          (signal_id == (uint8)GRR_MAC_PCCO_CAMPED_ON_CELL))
      {
         gmac_local_current_page_mode = PAGING_REORGANIZATION;
      }*/
      if (signal_id == (uint8)GRR_MAC_NACC_RELEASE_REQ)
      {
        /* handle GRR request for release of NACC storage */
        gmac_nacc_respond_to_release_req(global_data_ptr->msg_ptr,gas_id);
        proceed_flag = FALSE;
      }

      if (signal_id == (uint8)GRR_MAC_RESET_REQ)
      {
        if ( global_data_ptr->mac_mode == TRANSFER_MODE )
        {
          /* This function free's the PDAN memory, this is done before the complete reset
             because we only only know that we're in Transfer mode here*/
          gmac_transfer_dl_release_clean(gas_id);
        }

        (void)gmac_set_mac_null_substate(NULL_RESET, gas_id);

        gmac_util_send_ul_reset_req_to_rlc(gas_id);
        gmac_util_send_dl_reset_req_to_rlc(gas_id);
      }
      /*event logging changes*/
      mac_log_grr_events(signal_id,gas_id);
    }
    else if (task_id == MS_MAC_RLC)
    {
      signal_id = (uint8)global_data_ptr->msg_ptr->rlc_mac_sig.msg.sig;
    }
    else if (task_id == MS_MAC_MAC)
    {
      signal_id = (uint8)global_data_ptr->msg_ptr->mac_int_sig.sig_id;
    }
    else
    {
      GPRS_MEM_FREE(global_data_ptr->msg_ptr);
      MSG_GERAN_ERROR_1_G("MAC error TASK ID unknown: 0x%x",task_id);
      return;
    }

    /* Check task and signal id's before they get changed
    ** and set free memory flag accordingly.
    ** This applies to signals from L1 ISR layer.
    */

    /********************************************************************/
    /*GAS_ID continuity checking
     Check message GAS_ID matches the task GAS_ID
     for incoming messages from L1, RR and RLC */
    if (task_id == MS_MAC_L1)
    {
      gas_id_t gl1_msg_gas_id = gmac_get_l1_gas_id(&global_data_ptr->msg_ptr->l1_mac_sig);

      if ( gl1_msg_gas_id != gas_id)
      {
        proceed_flag = FALSE;
        MSG_GERAN_ERROR_2_G("GMAC L1 signal:0x%x, BAD gas_id:0x%x, ignore msg",global_data_ptr->msg_ptr->l1_mac_sig.sig_id,gl1_msg_gas_id);
      }
    }

    if (task_id == MS_MAC_RR)
    {
      gas_id_t grr_msg_gas_id = gmac_get_rr_gas_id(&global_data_ptr->msg_ptr->grr_mac_sig);

      if ( grr_msg_gas_id != gas_id)
      {
        proceed_flag = FALSE;
        MSG_GERAN_ERROR_2_G("GMAC RR signal:0x%x, BAD gas_id:0x%x, ignore msg",
                            global_data_ptr->msg_ptr->grr_mac_sig.sig_id,grr_msg_gas_id);
      }
    }

    if (task_id == MS_MAC_RLC)
    {
      gas_id_t grlc_msg_gas_id = gmac_get_rlc_gas_id(&global_data_ptr->msg_ptr->rlc_mac_sig);

      if ( grlc_msg_gas_id != gas_id)
      {
        proceed_flag = FALSE;
        MSG_GERAN_ERROR_2_G("GMAC RLC signal:0x%x, BAD gas_id:0x%x, ignore msg",
                            global_data_ptr->msg_ptr->rlc_mac_sig.msg.sig,grlc_msg_gas_id);
      }
    }

    if (task_id == MS_MAC_MAC)
    {
      gas_id_t gmac_msg_gas_id = global_data_ptr->msg_ptr->mac_int_sig.gas_id;

      if ( gmac_msg_gas_id != gas_id)
      {
        proceed_flag = FALSE;
        MSG_GERAN_ERROR_2_G("GMAC Internal signal:0x%x, BAD gas_id:0x%x, ignore msg",
                            global_data_ptr->msg_ptr->mac_int_sig.sig_id,gmac_msg_gas_id);
      }
    }
    /********************************************************************/

    if(signal_id == (uint8)MAC_PH_DATA_IND && task_id == MS_MAC_L1)
    {
      /* retrieve dl logical channel msg and perform RRBP functionality */
      segmented = rb_block_handler(gas_id);

      if (segmented == FALSE)
      {
        if (proceed_flag == TRUE)
        {
          /* Check "Page Mode" of all DL control messages in idle mode only */
          if ((global_data_ptr->mac_mode == IDLE_MODE) || (global_data_ptr->mac_mode == NULL_MODE))
          {
            /*MAC will process the page mode only if the mode is idle or null mode
              The reason for processing page mode in null is,
              In the case where we have camped on a 52 cell and there is no cell access.
              Under these conditions GRR will not see GRR_MAC_PS_ACCESS because the cell is
              barred. However L1 has is camped onto a PBCCH and will forward PH_DATA_IND's
              (ie PSI) to MAC. MS is obliged to monitor the page mode in the rcvd control
              msg and update L1 with changes in page mode. To accommodate this gmac has to
              monitor the page mode in NULL mode also
              Page mode initialization will be done only when gmac process PS_ACCESS IND in
              null mode*/
            gmac_util_page_mode_handler(gas_id);
          }

          /*Check whether we need to enter into the main processing loop or not*/
          proceed_flag = gmac_gprs_dl_ctrl_msg_proceed_validation(global_data_ptr->csn_dec_hdr_ptr->message_type,proceed_flag,gas_id);
        }

        /* validate DL control message */
        gmac_gprs_dl_ctrl_msg_validation(global_data_ptr->csn_dec_hdr_ptr->message_type,gas_id);
      }
      else
      {
        /* segmented message so avoid entering MAC state machine */
        proceed_flag = FALSE;
      }
      }

#ifdef DEBUG_GSM_GPRS_DS_UNIT_TEST
    #error code not present
#endif

#ifdef FEATURE_QSH_EVENT_METRIC
    if(proceed_flag == TRUE)
    {
      if(task_id == MS_MAC_MAC && signal_id == (uint8)MAC_TASK_QSH_METRIC_DATA)
      {
        qsh_client_action_done_s action_done;

        // Short-cut to call-back parameters
        qsh_client_cb_params_s *cb_params_ptr =
          &global_data_ptr->msg_ptr->mac_int_sig.msg.mac_task_qsh_metric_data.qsh_cb_params;

        // Short-cut to metric config parameters
        qsh_client_metric_cfg_s *metric_config_params_ptr = &cb_params_ptr->action_params.metric_cfg;

        if(metric_config_params_ptr->action
           == QSH_METRIC_ACTION_START)
        {
         MSG_GERAN_HIGH_0_G("MAC Rcvd MAC_TASK_QSH_METRIC_DATA (START)");

         task_data_ptr->saved_cb_metric_dump_ptr = metric_config_params_ptr->start_addr;

         task_data_ptr->qsh_metric_context_id = metric_config_params_ptr->metric_context_id;

         task_data_ptr->gmac_qsh_metrics_capture_activate = TRUE;
        }
        else
        {
         MSG_GERAN_HIGH_0_G("MAC Rcvd MAC_TASK_QSH_METRIC_DATA (STOP)");
         task_data_ptr->gmac_qsh_metrics_capture_activate = FALSE;
        }

        /* Indicate writing of data for call-back action done*/
        qsh_client_action_done_init(&action_done);

        action_done.cb_params_ptr = cb_params_ptr;
        action_done.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;

        /* indicate that callback has completed */
        qsh_client_action_done(&action_done);

        proceed_flag = FALSE;
      }
    }
#endif/* FEATURE_QSH_EVENT_METRIC */

    if(proceed_flag == TRUE)
    {
      /*filter out all FIXED assigments*/
      if (gmac_assignment_unsupported(global_data_ptr->mac_mode,signal_id,task_id,gas_id) == FALSE)
      {
        switch(global_data_ptr->mac_mode)
        {
          case NULL_MODE:
            gmac_store_trace_rvcd_msg_buf((uint8)global_data_ptr->mac_mode, (uint8)gmac_null_get_null_state(gas_id),gas_id);
            null_mode_handler(task_id, signal_id,gas_id);
            break;

          case IDLE_MODE:
            if(gmac_dl_msg_address_data_valid(signal_id,task_id,gas_id) == TRUE)
            {
              gmac_store_trace_rvcd_msg_buf((uint8)global_data_ptr->mac_mode, (uint8)gmac_idle_get_idle_state(gas_id),gas_id);
              idle_mode_handler(task_id, signal_id,gas_id);
            }
            break;

          case TRANSFER_MODE:
            gmac_store_trace_rvcd_msg_buf((uint8)global_data_ptr->mac_mode, (uint8)gmac_transfer_get_transfer_state(gas_id),gas_id);
            transfer_mode_handler(task_id, signal_id,gas_id);
            break;

          default:
            break;
        }
      }
      else
      {
        MSG_GERAN_MED_0_G(" RCVD fixed alloc in assigment msg. Not supported");
      }
    }
    else
    {
      /* reset segmented indicator before checking next signal in queue */
      segmented = FALSE;
      proceed_flag = TRUE;
    }

    /* free memory allocated by CSN.1 decoder */
    if (global_data_ptr->csn_mem_free_pending)
    {
      free_csn_decoder_ptrs_t(global_data_ptr->csn_dec_result_ptr);
      global_data_ptr->csn_mem_free_pending = FALSE;
    }

    GPRS_MEM_FREE(global_data_ptr->msg_ptr);

    /* check message queue for more signals. if msg_ptr returns NULL queue is empty */
    global_data_ptr->msg_ptr = (mac_signals_u*) (q_linear_search (&global_data_ptr->mac_q,(q_compare_func_type)gmac_check_q_for_mac_internal_sigs,
                                                 &compare_task_id));

    /*if MAC internal signal was found fetch it off the Q 1st */
    if(global_data_ptr->msg_ptr == NULL)
    {
      /*No intrenal MAC signal found on Q. Retrieve 1st signal on q*/
      global_data_ptr->msg_ptr = q_get(&global_data_ptr->mac_q);
    }
    else
    {
      /*internal MAC signal found on Q. delete mac sig from q as q_linear_search does not do this.*/

      gmac_log_reshuffling_of_internal_sigs(gas_id);

      q_delete(
                &global_data_ptr->mac_q,
                &(global_data_ptr->msg_ptr->mac_int_sig.link));
    }
  }

  /*clear GPRS_MAC_MASTER_SIG mask */

  GMAC_MSG_LOCK(gas_id);

  if ( q_cnt(&global_data_ptr->mac_q) == 0 )
    (void)rex_clr_sigs(rex_self(), GPRS_MAC_MASTER_SIG);

  GMAC_MSG_UNLOCK(gas_id);
}

/*===========================================================================
===
===  FUNCTION      gmac_store_trace_rvcd_msg_buf()
===
===  DESCRIPTION
===
===    This function stores the received signal messasge by GMAC in gmac_trace_msg_buf.
===    This function will also increment gmac_trace_msg_buf_index
===
===  DEPENDENCIES
===
===  gmac_trace_msg_buf, gmac_trace_msg_buf_index
===
===  RETURN VALUE
===
===    None.
===
===  SIDE EFFECTS
===
===    None.
===========================================================================*/
static void gmac_store_trace_rvcd_msg_buf(uint8 gmac_mode, uint8 state, gas_id_t gas_id)
{
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);
  gmac_task_data_t *task_data_ptr = gmac_get_task_data_ptr(gas_id);
  uint32 current_fn = GSTMR_GET_FN_GERAN(gas_id);

  task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].next_entry = FALSE;

  task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].FN = current_fn;

  task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].gmac_mode = gmac_mode;

  task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].state = state;

  memset(task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_frag, 0, sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag));

  if(global_data_ptr->msg_ptr->grr_mac_sig.task_id == MS_MAC_L1)
  {
    /* Do not store messages which are not used in debugging stability issues */
    if ((global_data_ptr->msg_ptr->l1_mac_sig.sig_id == L1_MAC_USF_DETECTED) ||
        (global_data_ptr->msg_ptr->l1_mac_sig.sig_id == MAC_PH_DATA_IND))
    {
      return;
    }
    
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_set = MS_MAC_L1;
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_id = (byte)global_data_ptr->msg_ptr->l1_mac_sig.sig_id;

    /*Grab 3 Bytes of Message */
    (void) memscpy(task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_frag,
                   sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag),
                   &(global_data_ptr->msg_ptr->l1_mac_sig.msg),
                   sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag)
                  );
  }
  else if(global_data_ptr->msg_ptr->grr_mac_sig.task_id == MS_MAC_RR)
  {
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_set = MS_MAC_RR;
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_id = (byte)global_data_ptr->msg_ptr->grr_mac_sig.sig_id;
    /*Grab 3 Bytes of Message */
    (void) memscpy(task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_frag,
                   sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag),
                   &(global_data_ptr->msg_ptr->grr_mac_sig.msg),
                   sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag)
                  );
  }
  else if(global_data_ptr->msg_ptr->grr_mac_sig.task_id == MS_MAC_RLC)
  {
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_set = MS_MAC_RLC;
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_id = (byte)global_data_ptr->msg_ptr->rlc_mac_sig.msg.sig;
    /*Grab 3 Bytes of Message */
    (void) memscpy(task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_frag,
                   sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag),
                   &(global_data_ptr->msg_ptr->rlc_mac_sig.msg),
                   sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag)
               );
  }
  else if(global_data_ptr->msg_ptr->grr_mac_sig.task_id == MS_MAC_MAC)
  {
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_set = MS_MAC_MAC;
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_id = (byte)global_data_ptr->msg_ptr->mac_int_sig.sig_id;
  }

#ifdef FEATURE_QSH_EVENT_METRIC
  gmac_qsh_metric_capture(gas_id, &(task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index]));
#endif /* FEATURE_QSH_EVENT_METRIC */

  task_data_ptr->gmac_trace_msg_buf_index++;

  if (task_data_ptr->gmac_trace_msg_buf_index >= GMAC_NUM_TRACE_MSGS)
  {
    task_data_ptr->gmac_trace_msg_buf_index = 0;
  }
  task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].next_entry = TRUE;

}

/*===========================================================================
===
===  FUNCTION      gmac_store_trace_send_msg_buf()
===
===  DESCRIPTION
===
===    This function stores the send signal message by GMAC in gmac_trace_msg_buf.
===    This function will also increment gmac_trace_msg_buf_index
===
===  DEPENDENCIES
===
===  gmac_trace_msg_buf, gmac_trace_msg_buf_index
===
===  RETURN VALUE
===
===    None.
===
===  SIDE EFFECTS
===
===    None.
===========================================================================*/
void gmac_store_trace_send_msg_buf(gas_id_t gas_id, uint8 message_set, void* message_ptr)
{
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);
  gmac_task_data_t *task_data_ptr = gmac_get_task_data_ptr(gas_id);
  uint32 current_fn = GSTMR_GET_FN_GERAN(gas_id);

  task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].next_entry = FALSE;

  task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].FN = current_fn;

  task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].gmac_mode = (byte)global_data_ptr->mac_mode;

  memset(task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_frag, 0, sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag));

  if(global_data_ptr->mac_mode == NULL_MODE)
  {
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].state = (byte)gmac_null_get_null_state(gas_id);
  }
  else if(global_data_ptr->mac_mode == IDLE_MODE)
  {
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].state = (byte)gmac_idle_get_idle_state(gas_id);
  }
  else if(global_data_ptr->mac_mode == TRANSFER_MODE)
  {
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].state = (byte)gmac_transfer_get_transfer_state(gas_id);
  }

  if((message_set == MS_MAC_L1) || (message_set == MS_MAC_RR))
  {
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_set = ((IMH_T *) message_ptr)->message_set;
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_id = ((IMH_T *) message_ptr)->message_id;
    /*Grab 3 Bytes of Message */
    (void) memscpy(task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_frag,
                   sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag),
                   ((byte *)message_ptr + sizeof(IMH_T)),
                   sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag)
                  );
  }
  else if(message_set == MS_MAC_RLC)
  {
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_set = MS_MAC_RLC;
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_id = (byte)((mac_rlc_msg_t*)message_ptr)->msg.sig;
    /*Grab 3 Bytes of Message */
    (void) memscpy(task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_frag,
                   sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag),
                   &(((mac_rlc_msg_t*)message_ptr)->msg),
                    sizeof(task_data_ptr->gmac_trace_msg_buf[0].message_frag)
                  );
  }
  else if(message_set == MS_MAC_MAC)
  {
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_set = MS_MAC_MAC;
    task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].message_id = (byte)((mac_int_sig_t*)message_ptr)->sig_id;
  }

#ifdef FEATURE_QSH_EVENT_METRIC
  gmac_qsh_metric_capture(gas_id, &(task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index]));
#endif /*FEATURE_QSH_EVENT_METRIC*/

  task_data_ptr->gmac_trace_msg_buf_index++;

  if (task_data_ptr->gmac_trace_msg_buf_index >= GMAC_NUM_TRACE_MSGS)
  {
    task_data_ptr->gmac_trace_msg_buf_index = 0;
  }
  task_data_ptr->gmac_trace_msg_buf[task_data_ptr->gmac_trace_msg_buf_index].next_entry = TRUE;

}

/*===========================================================================
===
===  FUNCTION      gmac_initialise()
===
===  DESCRIPTION
===  Contains all initialisation procedures required for MAC
===  upon start up
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===
===  SIDE EFFECTS
===
===
===========================================================================*/
#ifdef PERLUTF
#error code not present
#else
static void gmac_initialise(gas_id_t gas_id)
#endif
{
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);


  /* Initialise GMAC critical section */

  rex_init_crit_sect(&gmac_msg_crit_sec[gas_id]);


  /* initialise mac's signal queue */
  (void)q_init(&global_data_ptr->mac_q);

  /* define timer to OS */
  mac_timer_init(gas_id);

  /* initialise MAC mode and gmac_gprs_mode */
  global_data_ptr->mac_mode = NULL_MODE;

//  memset(&gmac_local_global_pwr_ctrl_params, 0, sizeof(global_pwr_ctrl_t));
//  memset(&gmac_local_pwr_ctrl_params, 0, sizeof(pwr_ctrl_paramtrs_t));

  /* retry bit in MAC header */
  global_data_ptr->c_flags.mac_ctrl_header_r_bit = NOT_SET;

  /* RRBP valid set to NOT valid initially */
  global_data_ptr->gmac_local_rrbp_valid = RM_SP_RRBP_NOT_VALID;

  gmac_util_init_cell_data(gas_id);

  gmac_init_transfer_flags(gas_id);
  gmac_init_idle_flags(gas_id);
  gmac_init_common_flags(gas_id);
  gmac_init_null_flags(gas_id);

  gmac_init_null_data_non_zero(gas_id);
  gmac_init_mac_proc_data_non_zero(gas_id);
  gmac_init_transfer_data_non_zero(gas_id);
  gmac_init_tran_dl_data_non_zero(gas_id);
  gmac_init_idle_data_non_zero(gas_id);
  gmac_init_rb_data_non_zero(gas_id);
  gmac_init_util_data_non_zero(gas_id);
  gmac_init_task_data_non_zero(gas_id);

#if defined (FEATURE_QSH_EVENT_METRIC) || defined (FEATURE_QSH_DUMP) || defined(FEATURE_QSH_MDUMP)
  if(gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
   gmac_qsh_init(gas_id);
  }
#ifdef FEATURE_QSH_MDUMP
  qsh_add_thread_id(QSH_CLT_GMAC);
#endif /* FEATURE_QSH_MDUMP */
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP || FEATURE_QSH_MDUMP*/

  mac_log_init(gas_id);
}
/*===========================================================================
===
===  FUNCTION      gmac_dl_msg_address_data_valid()
===
===  DESCRIPTION
===
===  If the signal that has been received is MAC_PH_DATA_IND and the
===  dl control msg within it has as after csn.1 decoding contains
===  an address_info_error then the dl_msg is ignored.
===  No further processing of the dl control msg is required.
===
===  DEPENDENCIES
===
===  csn_dec_hdr_ptr - global pointer to decoded CSN.1 info header
===
===  RETURN VALUE
===
===  FALSE if error found.
===
===========================================================================*/
static boolean gmac_dl_msg_address_data_valid(uint8 sig_id,uint8 tsk_id,gas_id_t gas_id)
{
  uint8 error = TRUE;
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);

  if ((sig_id == (uint8)MAC_PH_DATA_IND) && (tsk_id == MS_MAC_L1))
  {
    if(global_data_ptr->csn_dec_hdr_ptr->error_code == ADDRESS_INFO_ERROR)
    {
      /* DL msg address error ignore msg */
      error = FALSE;
    }
  }
  return(error);
}

/*===========================================================================
===
===  FUNCTION      gmac_assignment_unsupported()
===
===  DESCRIPTION
===
===  This function filters out all unsupported assignment control msgs i.e.
===  fixed allocations for all targets, and EGPRS for Worldphones (6800 and 7500-90)
===
===  DEPENDENCIES
===
===  csn_dec_hdr_ptr - global pointer to decoded CSN.1 info header
===
===  RETURN VALUE
===
===  TRUE if error fixed allocation is found.
===
===========================================================================*/
static boolean gmac_assignment_unsupported(mac_modes_t mode,uint8 sig_id,uint8 tsk_id,gas_id_t gas_id)
{
 boolean unsupported_assignment = FALSE;

 boolean egprs_capable_cell = FALSE;

 gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);

 /*Check cell supports EGPRS before acpeting any EGPRS assigmanets */
 rr_read_public_store( RRPS_CELL_OPTIONS_EGPRS_CAPABLE_CELL, &egprs_capable_cell,gas_id );

 if(mode == NULL_MODE)
 {
   /*Check assignments RCVD from RR*/
   if(tsk_id == MS_MAC_RR)
   {
     switch (sig_id)
     {
        case GRR_MAC_UL_TBF_EST:
          /*check for Fixed allaoctions*/
          if (global_data_ptr->msg_ptr->grr_mac_sig.msg.gm_ul_tbf_est.alloc_flag == fixed)
          {
            unsupported_assignment = TRUE;
            MSG_GERAN_MED_0_G("Fixed alloc in IA");
          }

          /*check for EGPRS assignment against cell EGPRS capability support  */
          if  ( (global_data_ptr->msg_ptr->grr_mac_sig.msg.gm_ul_tbf_est.egprs_ul_tbf.egprs_alloc == TRUE)
                && (egprs_capable_cell == FALSE) )
          {
            unsupported_assignment = TRUE;
            MSG_GERAN_MED_0_G("EGPRS Not suppressed on cell : EGPRS assignment rcvd in UL IA. Ignore assignment");
          }
          break;

        case GRR_MAC_UL_SB_EST:
           /*check for EGPRS assignment against cell EGPRS capability support  */
           if  ( (global_data_ptr->msg_ptr->grr_mac_sig.msg.gm_ul_sb_est.egprs_ul_sb_est.egprs_alloc == TRUE)
                 && (egprs_capable_cell == FALSE) )
           {
             unsupported_assignment = TRUE;
             MSG_GERAN_MED_0_G("EGPRS Not suppressed on cell : EGPRS assignment rcvd in UL SB IA. Ignore assignment");
           }
           break;

        case GRR_MAC_DL_TBF_EST:
          /*check for EGPRS assignment against cell EGPRS capability support  */
          if  ( (global_data_ptr->msg_ptr->grr_mac_sig.msg.gm_dl_tbf_est.egprs_alloc == TRUE)
                && (egprs_capable_cell == FALSE) )
          {
            unsupported_assignment = TRUE;
            MSG_GERAN_MED_0_G("EGPRS Not suppressed on cell : EGPRS assignment rcvd in DL IA Ignore assignment");
          }
          break;

        default:
          /*No action*/
          break;
     }
   }
 }
 else
 {
  /*check PH_DATA_IND for PDA,PUA,PUAN,PTR*/
  if(sig_id == (uint8)MAC_PH_DATA_IND && tsk_id == MS_MAC_L1)
  {
     switch (global_data_ptr->csn_dec_hdr_ptr->message_type)
     {
      case PACKET_DOWNLINK_ASSIGNMENT:
        if (  (((packet_downlink_assignment_t *)global_data_ptr->csn_dec_data_ptr)->mac_mode) == 2
           || (((packet_downlink_assignment_t *)global_data_ptr->csn_dec_data_ptr)->mac_mode) == 3    )
        {
          unsupported_assignment = TRUE;
          MSG_GERAN_MED_1_G("Fixed alloc in PDA %d",global_data_ptr->csn_dec_hdr_ptr->message_type);
        }

#ifdef FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED
        if ((((packet_downlink_assignment_t *)global_data_ptr->csn_dec_data_ptr)->r99_additions_flag == TRUE) &&
            (((packet_downlink_assignment_t *)global_data_ptr->csn_dec_data_ptr)->egprs_options_flag == TRUE))
        {
          unsupported_assignment = TRUE;
        }
#else /*  FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED */
        /*check for EGPRS assignment against cell EGPRS capability support  */
        if ((((packet_downlink_assignment_t *)global_data_ptr->csn_dec_data_ptr)->r99_additions_flag == TRUE) &&
            (((packet_downlink_assignment_t *)global_data_ptr->csn_dec_data_ptr)->egprs_options_flag == TRUE) &&
            (egprs_capable_cell == FALSE))
        {
          unsupported_assignment = TRUE;
          MSG_GERAN_MED_0_G("EGPRS Not suppressed on cell : EGPRS assignment rcvd in PDA. Ignore assignment");
        }
#endif /*  FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED */

        break;

      case PACKET_TIMESLOT_RECONFIGURE:
        if ((((packet_timeslot_reconfigure_t *)global_data_ptr->csn_dec_data_ptr)->alloc_flag) == fixed)
        {
          unsupported_assignment = TRUE;
          MSG_GERAN_MED_1_G("Fixed alloc in PTR %d",global_data_ptr->csn_dec_hdr_ptr->message_type);
        }

#ifdef FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED
        if (((packet_timeslot_reconfigure_t *)global_data_ptr->csn_dec_data_ptr)->egprs_ptr == TRUE)
        {
          unsupported_assignment = TRUE;
        }
#else /*  FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED */
        if ((((packet_timeslot_reconfigure_t *)global_data_ptr->csn_dec_data_ptr)->egprs_ptr == TRUE) &&
            (egprs_capable_cell == FALSE))
        {
          unsupported_assignment = TRUE;
          MSG_GERAN_MED_0_G("EGPRS Not suppressed on cell : EGPRS assignment rcvd in PTR. Ignore assignment");
        }
#endif /*  FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED */

        break;

      case PACKET_UPLINK_ACK_NACK:
        if ((((packet_uplink_ack_nack_t *)global_data_ptr->csn_dec_data_ptr)->fixed_alloc_flag) == valid)
        {
          unsupported_assignment = TRUE;
          MSG_GERAN_MED_1_G("Fixed alloc in PUAN %d",global_data_ptr->csn_dec_hdr_ptr->message_type);
        }

#ifdef FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED
        if (((packet_uplink_ack_nack_t *)global_data_ptr->csn_dec_data_ptr)->egprs_puan == TRUE)
        {
          unsupported_assignment = TRUE;
        }
#else /*  FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED */
        if ((((packet_uplink_ack_nack_t *)global_data_ptr->csn_dec_data_ptr)->egprs_puan == TRUE) &&
            (egprs_capable_cell == FALSE))
        {
          unsupported_assignment = TRUE;
          MSG_GERAN_MED_0_G("EGPRS Not suppressed on cell : EGPRS assignment rcvd in PUAN. Ignore assignment");
        }
#endif /*  FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED */
        break;

      case PACKET_UPLINK_ASSIGNMENT:
        if ((((packet_uplink_assignment_t *)global_data_ptr->csn_dec_data_ptr)->allocation_flag) == fixed_alloc)
        {
          unsupported_assignment = TRUE;
          MSG_GERAN_MED_1_G("Fixed alloc in PUA %d",global_data_ptr->csn_dec_hdr_ptr->message_type);
        }

#ifdef FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED
        if (((packet_uplink_assignment_t *)global_data_ptr->csn_dec_data_ptr)->egprs_pua == TRUE)
        {
          unsupported_assignment = TRUE;
        }
#else /*  FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED */
       if ((((packet_uplink_assignment_t *)global_data_ptr->csn_dec_data_ptr)->egprs_pua == TRUE) &&
           (egprs_capable_cell == FALSE))
        {
          unsupported_assignment = TRUE;
          MSG_GERAN_MED_0_G("EGPRS Not suppressed on cell : EGPRS assignment rcvd in PUA. Ignore assignment");
        }
#endif /*  FEATURE_EDGE_CAPABILITY_NOT_SUPPORTED */
        break;

      default:
        /*No action*/
        break;
     }
   }
 }

 return(unsupported_assignment);

}


#ifdef FEATURE_GSM_EDTM
/*===========================================================================
===
===  FUNCTION      gmac_edtm_enabled_init()
===
===  DESCRIPTION
===  Initilialise the edtm_enabled flag from the RR public store.
===
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===
===  SIDE EFFECTS
===
===
===========================================================================*/
void gmac_edtm_enabled_init(gas_id_t gas_id)
{
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);

  rr_read_public_store( RRPS_MS_EDTM_ENABLED, &global_data_ptr->gmac_edtm_enabled,gas_id );
}

#endif /*FEATURE_GSM_EDTM*/

/*tlx free floating */
/*===========================================================================
===
===  FUNCTION      gmac_check_q_for_mac_internal_sigs()
===
===  DESCRIPTION
===
===  Compare function passed to q_linear_search and is used by q_linear_search to checks
===  the Q for the item requested.
===
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===  Return 1 if the value matches and 0 otherwise
===
===  SIDE EFFECTS
===
===
===========================================================================*/
static boolean  gmac_check_q_for_mac_internal_sigs(
                                            mac_signals_u*   item_ptr,
                                            uint8            compare_task_id,
                                            gas_id_t         gas_id
                                           )
{

  if(item_ptr->mac_int_sig.task_id == MS_MAC_MAC )
  {
     return (1);
  }
  else
  {
     return (0);
  }
}

/*===========================================================================
===
===  FUNCTION      gmac_log_reshuffling_of_internal_sigs()
===
===  DESCRIPTION
===
===  Debug to monitor the re shuffling of mac internal signals to the front of the Q
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===
===  SIDE EFFECTS
===
===
===========================================================================*/
static void gmac_log_reshuffling_of_internal_sigs(gas_id_t gas_id)
{
  mac_signals_u   *next_debug_msg_ptr;
  uint8           mac_num_sigs_in_q = 0;
  uint8           task_id;
  uint8           signal_id;
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);

  mac_num_sigs_in_q = q_cnt(&global_data_ptr->mac_q);

  if(mac_num_sigs_in_q > 1)
  {

    next_debug_msg_ptr = q_check(&global_data_ptr->mac_q);
    if (next_debug_msg_ptr == NULL)
    {
      MSG_GERAN_ERROR_0_G("MAC error q_check return NULL pointer ");
      return;
    }
    task_id = next_debug_msg_ptr->grr_mac_sig.task_id;


    if (task_id == MS_MAC_L1)
    {
      signal_id = (uint8)next_debug_msg_ptr->l1_mac_sig.sig_id;
    }
    else if (task_id == MS_MAC_RR)
    {
      signal_id = (uint8)next_debug_msg_ptr->grr_mac_sig.sig_id;
    }
    else if (task_id == MS_MAC_RLC)
    {
      signal_id = (uint8)next_debug_msg_ptr->rlc_mac_sig.msg.sig;
    }
    else if (task_id == MS_MAC_MAC)
    {
      signal_id = (uint8)next_debug_msg_ptr->mac_int_sig.sig_id;
    }
    else
    {
      MSG_GERAN_ERROR_1_G("MAC error TASK ID unknown: 0x%x",task_id);
      return;
    }

    MSG_GERAN_MED_2_G("First sig in q task id %d. sig id %d",task_id,signal_id);

    next_debug_msg_ptr = q_next(&global_data_ptr->mac_q,&(next_debug_msg_ptr->l1_mac_sig.link));

    while(next_debug_msg_ptr != NULL)
    {
      task_id = next_debug_msg_ptr->grr_mac_sig.task_id;

      if (task_id == MS_MAC_L1)
      {
        signal_id = (uint8)next_debug_msg_ptr->l1_mac_sig.sig_id;
      }
      else if (task_id == MS_MAC_RR)
      {
        signal_id = (uint8)next_debug_msg_ptr->grr_mac_sig.sig_id;
      }
      else if (task_id == MS_MAC_RLC)
      {
        signal_id = (uint8)next_debug_msg_ptr->rlc_mac_sig.msg.sig;
      }
      else if (task_id == MS_MAC_MAC)
      {
        signal_id = (uint8)next_debug_msg_ptr->mac_int_sig.sig_id;
      }
      else
      {
        MSG_GERAN_ERROR_1_G("MAC error TASK ID unknown: 0x%x",task_id);
        return;
      }

      MSG_GERAN_MED_2_G("next sig in q task id %d sig id %d",task_id,signal_id);

      next_debug_msg_ptr = q_next(&global_data_ptr->mac_q,&(next_debug_msg_ptr->l1_mac_sig.link));
    }
  }
    }

#ifdef PERLUTF
#error code not present
#endif /*PERLUTF*/


#ifdef FEATURE_QSH_DUMP
/*!
 * \brief Returns the dump info
 *
 * \return qsh_client_dump_info_s
 */
__attribute__((section(".uncompressible.text")))
qsh_client_dump_info_s gmac_qsh_dump_init(void)
{
  qsh_client_dump_info_s dump_info;

  dump_info.max_size_bytes = NUM_GERAN_DATA_SPACES * sizeof(gmac_qsh_dump_tag_data_s);

  return dump_info;
}
#endif // FEATURE_QSH_DUMP

#ifdef FEATURE_QSH_EVENT_METRIC
/*!
 * \brief Returns the metric config info
 *
 * \return qsh_client_metric_info_s
 */
__attribute__((section(".uncompressible.text")))
static qsh_client_metric_info_s gmac_qsh_metric_init(void)
{
  uint8 sub_index;
  qsh_client_metric_info_s metric_info;

  for (sub_index = 0; sub_index < NUM_GERAN_DATA_SPACES; sub_index++)
  {
    qsh_ext_metric_cfg_s *metric_cfg_ptr = &gmac_qsh_metric_cfg_arr[sub_index][GMAC_QSH_METRIC_DATA];

    qsh_client_metric_cfg_init(gmac_qsh_metric_cfg_arr[sub_index], GMAC_QSH_METRIC_COUNT);

    metric_cfg_ptr->id = GMAC_QSH_METRIC_DATA;
    metric_cfg_ptr->subs_id = geran_map_gas_id_to_nas_id((gas_id_t)sub_index);
    metric_cfg_ptr->fifo.element_size_bytes = sizeof(gmac_qsh_metric_data_s);
    metric_cfg_ptr->fifo.element_count_total = GMAC_QSH_NUM_TRACE_MSGS;
    metric_cfg_ptr->sampling_period_ms = 0; // not sampled
  }

  metric_info.metric_cfg_arr_ptr = &gmac_qsh_metric_cfg_arr[0][0];
  metric_info.metric_cfg_count = NUM_GERAN_DATA_SPACES * GMAC_QSH_METRIC_COUNT;

  return metric_info;
}
#endif // FEATURE_QSH_EVENT_METRIC

#if defined (FEATURE_QSH_EVENT_METRIC) || defined (FEATURE_QSH_DUMP) || defined (FEATURE_QSH_MDUMP)
/*===========================================================================
===
===  FUNCTION      gmac_qsh_init()
===
===  DESCRIPTION   Initializes GMAC QSH client.
===
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===
===  SIDE EFFECTS
===
===
===========================================================================*/
__attribute__((section(".uncompressible.text")))
static void gmac_qsh_init
(
  gas_id_t gas_id
)
{
  qsh_client_reg_s client_reg;
  qsh_action_mask_t cb_action_support_mask = 0;

  /* initialize reg params */
  qsh_client_reg_init(&client_reg);

#ifdef FEATURE_QSH_DUMP
  // Setup the call-back support mask for dumps
  cb_action_support_mask |= QSH_ACTION_DUMP_COLLECT;

  // Populate the dump info
  client_reg.dump_info = gmac_qsh_dump_init();
#endif // FEATURE_QSH_DUMP

#ifdef FEATURE_QSH_MDUMP
  cb_action_support_mask |= QSH_ACTION_MDUMP;
#endif /* FEATURE_QSH_MDUMP */

#ifdef FEATURE_QSH_EVENT_METRIC
  // Setup the call-back support mask for metrics
  cb_action_support_mask |= QSH_ACTION_METRIC_CFG;

  // Populate the metric config info
  client_reg.metric_info = gmac_qsh_metric_init();
#endif // FEATURE_QSH_EVENT_METRIC

  /*MAC Registration detials for QSH : Client ID,data capture callback func ptr and version*/

  client_reg.client = QSH_CLT_GMAC;
  client_reg.major_ver = GMAC_QSH_MAJOR_VER;
  client_reg.minor_ver = GMAC_QSH_MINOR_VER;
  client_reg.client_cb_ptr = gmac_qsh_data_capture_cb;
  client_reg.cb_action_support_mask = cb_action_support_mask;

  /* register client */
  qsh_client_reg(&client_reg);
}




/*==============================================================================
===  FUNCTION:  gmac_qsh_data_capture_cb
===
===  Callback function registered with QSH.
===
===  directive passed by QSH used in this function
===
===  INPUT  qsh_cb_params_ptr :
===
===  qsh_cb_params_ptr->action :   QSH_ACTION_METRIC_CFG and QSH_ACTION_DUMP_COLLECT
===
===
===
============================================================================*/
#ifndef FEATURE_QSH_MDUMP
__attribute__((section(".uncompressible.text")))
#else
QSH_MDUMP_FN_ATTR
#endif /* FEATURE_QSH_MDUMP */
static void gmac_qsh_data_capture_cb
(
  qsh_client_cb_params_s *   qsh_cb_params_ptr
)
{

  gmac_global_data_t *global_data_ptr;


  switch (qsh_cb_params_ptr->action)
  {

#ifdef FEATURE_QSH_EVENT_METRIC

     case QSH_ACTION_METRIC_CFG:
     {
       gas_id_t gas_id = geran_map_nas_id_to_gas_id(qsh_cb_params_ptr->action_params.metric_cfg.subs_id);

       /*send internal sig to run action in MAC's  task context with a copy ofqsh_ch_params */
       global_data_ptr = gmac_get_global_data_ptr(gas_id);

       global_data_ptr->mac_mac_sig.msg.mac_task_qsh_metric_data.qsh_cb_params = *qsh_cb_params_ptr;
       global_data_ptr->mac_mac_sig.sig_id = MAC_TASK_QSH_METRIC_DATA;
       global_data_ptr->mac_mac_sig.task_id = MS_MAC_MAC;
       gprs_mac_mac_acc_func(&global_data_ptr->mac_mac_sig,gas_id);

       break;
     }

#endif /*FEATURE_QSH_EVENT_METRIC*/


#ifdef FEATURE_QSH_DUMP
     case QSH_ACTION_DUMP_COLLECT:

         /* run action in current context */
         gmac_qsh_run_dump_action(qsh_cb_params_ptr);

       break;

#endif/*FEATURE_QSH_DUMP*/

#ifdef FEATURE_QSH_MDUMP
     case QSH_ACTION_MDUMP:
     {
       gmac_qsh_mdump_collect(qsh_cb_params_ptr);
       break;
     }
#endif /*FEATURE_QSH_MDUMP*/

      default:
        break;
  }

}

#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP || FEATURE_QSH_MDUMP */

/*==============================================================================
===
===  FUNCTION:  gmac_qsh_run_dump_action
===
===
===      note: use qsh_action_enabled() to check which action is enabled
===
============================================================================*/
#ifdef FEATURE_QSH_DUMP

__attribute__((section(".uncompressible.text")))
static void gmac_qsh_run_dump_action
(
  qsh_client_cb_params_s *   qsh_cb_params_ptr
)
{
  qsh_client_action_done_s client_action_done;    /* data passed to QSH when client has finished*/
                                                  /* processing its action requested by QSH     */

  gmac_task_data_t *task_data_ptr;
  gas_id_t  gas_id;

  gmac_qsh_dump_tag_data_s   *cb_param_dump_ptr;         //ptr to qsh dump loaction for MAC
  size_t remaining_size;             //need to keep a check of space left when adding in dumps for each task
  size_t total_size_written = 0;
  uint8 i;

  remaining_size = qsh_cb_params_ptr->action_params.dump_collect.dump_iovec.size_bytes;
  cb_param_dump_ptr = (gmac_qsh_dump_tag_data_s *)(qsh_cb_params_ptr->action_params.dump_collect.dump_iovec.addr);


  /* determine which sub needs to dump data */
  if (QSH_DUMP_TAG_ENABLED(qsh_cb_params_ptr->action_params.dump_collect.dump_tag_mask,GMAC_QSH_DUMP_TAG_DATA))
  {

    if(remaining_size >= sizeof(gmac_qsh_dump_tag_data_s))
    {
      /* get header*/
      qsh_client_dump_tag_hdr_init(
        &cb_param_dump_ptr->hdr,
        (qsh_dump_tag_t)GMAC_QSH_DUMP_TAG_DATA,
        (size_t)sizeof(gmac_qsh_dump_tag_data_s)
      );

      /* get  dump */
      gas_id = geran_map_nas_id_to_gas_id(SYS_MODEM_AS_ID_1);
      task_data_ptr = gmac_get_task_data_ptr(gas_id);

      for( i=0;i < GMAC_QSH_NUM_TRACE_MSGS; i++ )
      {
        cb_param_dump_ptr->trace_buf.buf[i].next_entry =  task_data_ptr->gmac_trace_msg_buf[i].next_entry;
        cb_param_dump_ptr->trace_buf.buf[i].timestamp_fn =  task_data_ptr->gmac_trace_msg_buf[i].FN;
        cb_param_dump_ptr->trace_buf.buf[i].gmac_mode =  task_data_ptr->gmac_trace_msg_buf[i].gmac_mode;
        cb_param_dump_ptr->trace_buf.buf[i].state =      task_data_ptr->gmac_trace_msg_buf[i].state;
        cb_param_dump_ptr->trace_buf.buf[i].message_set =  task_data_ptr->gmac_trace_msg_buf[i].message_set;
        cb_param_dump_ptr->trace_buf.buf[i].message_id =  task_data_ptr->gmac_trace_msg_buf[i].message_id;
        cb_param_dump_ptr->trace_buf.buf[i].message_frag[0] =  task_data_ptr->gmac_trace_msg_buf[i].message_frag[0];
        cb_param_dump_ptr->trace_buf.buf[i].message_frag[1] =  task_data_ptr->gmac_trace_msg_buf[i].message_frag[1];
        cb_param_dump_ptr->trace_buf.buf[i].message_frag[2] =  task_data_ptr->gmac_trace_msg_buf[i].message_frag[2];
      }

      /* Offset the pointer by the size of the dump tag structure, ready for the next write */
      cb_param_dump_ptr += 1;

      remaining_size -= sizeof(gmac_qsh_dump_tag_data_s);
      total_size_written += sizeof(gmac_qsh_dump_tag_data_s);
    }

  }

  if (QSH_DUMP_TAG_ENABLED(qsh_cb_params_ptr->action_params.dump_collect.dump_tag_mask,GMAC_QSH_DUMP_TAG_DATA_2))
  {

    if(remaining_size >= sizeof(gmac_qsh_dump_tag_data_s))
    {
      /* get header*/
      qsh_client_dump_tag_hdr_init(
        &cb_param_dump_ptr->hdr,
        (qsh_dump_tag_t)GMAC_QSH_DUMP_TAG_DATA_2,
        (size_t)sizeof(gmac_qsh_dump_tag_data_s)
      );

      /* get  dump */
      gas_id = geran_map_nas_id_to_gas_id(SYS_MODEM_AS_ID_2);
      task_data_ptr = gmac_get_task_data_ptr(gas_id);

      for( i=0;i < GMAC_QSH_NUM_TRACE_MSGS; i++ )
      {
        cb_param_dump_ptr->trace_buf.buf[i].next_entry =  task_data_ptr->gmac_trace_msg_buf[i].next_entry;
        cb_param_dump_ptr->trace_buf.buf[i].timestamp_fn =  task_data_ptr->gmac_trace_msg_buf[i].FN;
        cb_param_dump_ptr->trace_buf.buf[i].gmac_mode =  task_data_ptr->gmac_trace_msg_buf[i].gmac_mode;
        cb_param_dump_ptr->trace_buf.buf[i].state =      task_data_ptr->gmac_trace_msg_buf[i].state;
        cb_param_dump_ptr->trace_buf.buf[i].message_set =  task_data_ptr->gmac_trace_msg_buf[i].message_set;
        cb_param_dump_ptr->trace_buf.buf[i].message_id =  task_data_ptr->gmac_trace_msg_buf[i].message_id;
        cb_param_dump_ptr->trace_buf.buf[i].message_frag[0] =  task_data_ptr->gmac_trace_msg_buf[i].message_frag[0];
        cb_param_dump_ptr->trace_buf.buf[i].message_frag[1] =  task_data_ptr->gmac_trace_msg_buf[i].message_frag[1];
        cb_param_dump_ptr->trace_buf.buf[i].message_frag[2] =  task_data_ptr->gmac_trace_msg_buf[i].message_frag[2];
      }

      /* Offset the pointer by the size of the dump tag structure, ready for the next write */
      cb_param_dump_ptr += 1;

      remaining_size -= sizeof(gmac_qsh_dump_tag_data_s);
      total_size_written += sizeof(gmac_qsh_dump_tag_data_s);
    }

  }

  /* Indicate writing of data for call-back action done*/
  qsh_client_action_done_init(&client_action_done);

  client_action_done.cb_params_ptr = qsh_cb_params_ptr;
  client_action_done.params.dump_collect.size_written_bytes = total_size_written;
  client_action_done.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;

  /* indicate that callback has completed */
  qsh_client_action_done(&client_action_done);

}

#endif /* FEATURE_QSH_DUMP */


#ifdef FEATURE_QSH_EVENT_METRIC
/*==============================================================================
===  FUNCTION:  gmac_qsh_metric_capture()
===
===  Copies the last rcvd or sent signal into the current 10 second QSH metric
===  buffer and increments the next entry in preparation for any subsequent
===  signal.
===
===  input: QSH provides a new pointer to the next section in it's MAC metric
===         buffer. Assumption is this is zero initialised.
===
============================================================================*/
__attribute__((section(".uncompressible.text")))
static void gmac_qsh_metric_capture(gas_id_t gas_id, gmac_trace_msg_t *last_saved_rcvd_sent_msg_ptr)
{
  qsh_client_metric_log_done_s client_metric_done;    /* data passed to QSH when client has finished*/

  gmac_qsh_metric_data_s   *cb_param_metric_ptr;         //ptr to QSH dump loaction for MAC metric data

  gmac_task_data_t *task_data_ptr = gmac_get_task_data_ptr(gas_id);


  if(task_data_ptr->gmac_qsh_metrics_capture_activate == TRUE)
  {
    /*load last last sent/rcvd msg into into QSH metric dump loaction */
    cb_param_metric_ptr = (gmac_qsh_metric_data_s *) (task_data_ptr->saved_cb_metric_dump_ptr);

    /*map from relevent info from gmac_trace_msg_buf[] in to the metric data */
    cb_param_metric_ptr->metric_buf.gmac_mode = last_saved_rcvd_sent_msg_ptr->gmac_mode;
    cb_param_metric_ptr->metric_buf.state = last_saved_rcvd_sent_msg_ptr->state;
    cb_param_metric_ptr->metric_buf.message_set = last_saved_rcvd_sent_msg_ptr->message_set;
    cb_param_metric_ptr->metric_buf.message_id = last_saved_rcvd_sent_msg_ptr->message_id;

    qsh_client_metric_log_done_init(&client_metric_done);

    client_metric_done.log_reason = QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL;
    client_metric_done.metric_context_id =  task_data_ptr->qsh_metric_context_id;
    client_metric_done.metric_id = GMAC_QSH_METRIC_DATA;
    client_metric_done.client = QSH_CLT_GMAC;

    /*qsh retuns pointer to the next MAC metric dump loaction*/
    task_data_ptr->saved_cb_metric_dump_ptr = qsh_client_metric_log_done(&client_metric_done);


  }

}

#endif /* FEATURE_QSH_EVENT_METRIC */

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR static void gmac_qsh_mdump_trace_buffer(gas_id_t gas_id, uint8 num_entries)
{
  uint8 buf_ind = (gmac_task_data[gas_id].gmac_trace_msg_buf_index - num_entries + GMAC_NUM_TRACE_MSGS) % GMAC_NUM_TRACE_MSGS;

  qsh_mdump_collect_high(&(gmac_task_data[gas_id].gmac_trace_msg_buf_index), sizeof(byte));

  if (buf_ind < gmac_task_data[gas_id].gmac_trace_msg_buf_index)
  {
    qsh_mdump_collect_high(&(gmac_task_data[gas_id].gmac_trace_msg_buf[buf_ind]), num_entries*sizeof(gmac_trace_msg_t));
  }
  else
  {
    qsh_mdump_collect_high(&(gmac_task_data[gas_id].gmac_trace_msg_buf[buf_ind]),
      (GMAC_NUM_TRACE_MSGS - buf_ind)*sizeof(gmac_trace_msg_t));
    qsh_mdump_collect_high(&(gmac_task_data[gas_id].gmac_trace_msg_buf[0]),
      (gmac_task_data[gas_id].gmac_trace_msg_buf_index) *sizeof(gmac_trace_msg_t));
  }
}

QSH_MDUMP_FN_ATTR void gmac_qsh_mdump_collect_active(gas_id_t gas_id)
{
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);

  gmac_qsh_mdump_collect_null_data(TRUE, gas_id);
  gmac_qsh_mdump_collect_idle_data(TRUE, gas_id);
  gmac_qsh_mdump_collect_trans_data(TRUE, gas_id);
  qsh_mdump_collect_high(global_data_ptr, sizeof(gmac_global_data_t));

  /* Active sub,dump only 40 entries from gmac_task_data trace buffer entries */
  gmac_qsh_mdump_trace_buffer(gas_id, 40);
}

QSH_MDUMP_FN_ATTR void gmac_qsh_mdump_collect_inactive(gas_id_t gas_id)
{
  gmac_global_data_t *global_data_ptr = gmac_get_global_data_ptr(gas_id);

  qsh_mdump_collect_high(&(global_data_ptr->c_flags), sizeof(common_flags_t));
  qsh_mdump_collect_high(&(global_data_ptr->mac_mode), sizeof(mac_modes_t));
  gmac_qsh_mdump_collect_null_data(FALSE, gas_id);

  /* Inactive sub, dump only 20 entries from gmac_task_data trace buffer entries */
  gmac_qsh_mdump_trace_buffer(gas_id, 20);
}

/*===========================================================================

FUNCTION  gmac_qsh_mdump_collect

DESCRIPTION
  Function to collect GL1 mini dump.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void gmac_qsh_mdump_collect(qsh_client_cb_params_s *param)
{
  gas_id_t gas_id;

  for (gas_id = 0; gas_id < NUM_GERAN_DATA_SPACES; gas_id++)
  {
    switch (gmac_get_global_data_ptr(gas_id)->mac_mode)
    {
      case NULL_MODE:
      {
        gmac_qsh_mdump_collect_inactive(gas_id);
        break;
      }
      case IDLE_MODE:
      case TRANSFER_MODE:
      {
        gmac_qsh_mdump_collect_active(gas_id);
        break;
      }
    }
  }

  qsh_mdump_prioritize_client(QSH_CLT_GMAC, QSH_CLT_GL1);
  qsh_mdump_prioritize_client(QSH_CLT_GMAC, QSH_CLT_GRR);
  qsh_mdump_prioritize_client(QSH_CLT_GMAC, QSH_CLT_GRLCUL);
  qsh_mdump_prioritize_client(QSH_CLT_GMAC, QSH_CLT_GRLCDL);
}
#endif /* FEATURE_QSH_MDUMP */

/*** EOF: don't remove! ***/
