/*****************************************************************************
***
*** TITLE
***
***  GPRS LAYER 2 SERVICES CONTROL TASK FUNCTIONS
***
***
*** DESCRIPTION
***
***  REX task based scheduling framework for GPRS LLC functionality.
***
***
*** EXTERNALIZED FUNCTIONS
***
***  gllc_task()
***
***
*** INITIALIZATION AND SEQUENCING REQUIREMENTS
***
***  REX initialisation of the task control blocks and stacks is the
***  function of the TMC Task Mode Controller.
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
*** $Header: //components/rel/geran.mpss/7.4.0/gllc/src/gllctask.c#4 $
***
*** when       who     what, where, why
*** --------   ---     ------------------------------------------------------
*** 10/12/13   ra      CR563511
*** mm/dd/yy   xyz     changes
***
*****************************************************************************/


/*****************************************************************************
***
***     Included Files
***
*****************************************************************************/

#include "geran_variation.h"
#include "customer.h" /* This include must be first before #ifdef FEATURE_  */

#include "geran_tasks.h"
#include "gllci.h"
#include "gllclient.h"

#include "comdef.h"
#include "gmutex.h"
#include "dsm.h"
#include "err.h"
#include "msg.h"
#include "rex.h"
#include "task.h"
#include "rcinit.h"

#include "dog_hb_rex.h"

#include "gllclog.h"

#include "gsndcp_v_g.h"

/* LLC critical section to protect accesses to LLC messages and internal vars
*/

rex_crit_sect_type gllc_crit_sec[ NUM_GERAN_DATA_SPACES ];
rex_crit_sect_type gllc_ul_msg_crit_sec[ NUM_GERAN_DATA_SPACES ];
rex_crit_sect_type gllc_dl_msg_crit_sec[ NUM_GERAN_DATA_SPACES ];
rex_crit_sect_type gllc_cipher_crit_sec;


/*****************************************************************************
***
***     Private Constants & Defines
***
*****************************************************************************/

/* This feature is defined to support two input parameters into call back
   functions.*/



/*****************************************************************************
***
***     Private Typedefs
***
*****************************************************************************/


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

static void gllc_initialise( gas_id_t gas_id );


/*****************************************************************************
***
***     Public Data Declarations
***
***     Ensure an external reference is added to the module's header file!
***
*****************************************************************************/

rex_sigs_type     gllc_task_sigs[ NUM_GERAN_DATA_SPACES ]   = { 0 };
boolean           gllc_initialised[ NUM_GERAN_DATA_SPACES ] = { FALSE };
rex_timer_type    gllc_gmm_ready_timer[ NUM_GERAN_DATA_SPACES ];

#ifdef FEATURE_QSH_EVENT_METRIC
/* This is the QSH callback param structure which hold an array of x, one for each subscriber
*/
qsh_client_cb_params_s  gllc_qsh_cb_param[NUM_GERAN_DATA_SPACES];

#define GLLC_QSH_METRIC_DEFAULT_CONFIG_COUNT 2
#define GLLC_QSH_METRIC_DATA_COUNT 50

qsh_ext_metric_cfg_s  gllc_qsh_metric_cfg_arr[GLLC_QSH_METRIC_DEFAULT_CONFIG_COUNT];
#endif /* FEATURE_QSH_EVENT_METRIC */

#if defined (FEATURE_QSH_EVENT_METRIC) || defined (FEATURE_QSH_DUMP)
gllc_debug_data_t gllc_debug_data[NUM_GERAN_DATA_SPACES];
#endif

#ifdef FEATURE_QSH_DUMP
/* Define a structure with QSH dump header and all TLV structures */
typedef struct
{
  qsh_dump_hdr_s hdr;
  gllc_qsh_dump_tag_data_s gllc_qsh_dump_tag_data;
  gllc_qsh_dump_tag_data_s gllc_qsh_dump_tag_data_2;
} gllc_qsh_tc_dump_collect_s;

/* Pointer declared as type of QSH data structure for parser */
gllc_qsh_dump_tag_data_s  *gllc_qsh_dump_tag_0_ptr;
gllc_qsh_dump_tag_data_s  *gllc_qsh_dump_tag_1_ptr;
#endif /* FEATURE_QSH_DUMP */

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR static void gllc_qsh_mdump_collect(qsh_client_cb_params_s *param);
#endif /* FEATURE_QSH_MDUMP */

/*****************************************************************************
***
***     Private Macros
***
*****************************************************************************/


/*****************************************************************************
***
***     Private Functions
***
*****************************************************************************/

/* LLC UL Watermark Event Notification Functions */

static void gllc_ll_1_ul_ack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_1_GMM, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_1_ul_ack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_1_GMM, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_1_ul_unack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_1_GMM, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}


static void gllc_ll_1_ul_unack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_1_GMM, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}

/* ------------------------------------------------------------------------ */


/* ------------------------------------------------------------------------ */


static void gllc_ll_3_ul_ack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_3_LL3, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_3_ul_ack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_3_LL3, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_3_ul_unack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_3_LL3, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}


static void gllc_ll_3_ul_unack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_3_LL3, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}

/* ------------------------------------------------------------------------ */


static void gllc_ll_5_ul_ack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_5_LL5, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_5_ul_ack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_5_LL5, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_5_ul_unack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_5_LL5, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}


static void gllc_ll_5_ul_unack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_5_LL5, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}

/* ------------------------------------------------------------------------ */


static void gllc_ll_7_ul_ack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_7_SMS, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_7_ul_ack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_7_SMS, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_7_ul_unack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_7_SMS, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}


static void gllc_ll_7_ul_unack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_7_SMS, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}

/* ------------------------------------------------------------------------ */


/* ------------------------------------------------------------------------ */


static void gllc_ll_9_ul_ack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_9_LL9, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_9_ul_ack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_9_LL9, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_9_ul_unack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_9_LL9, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}


static void gllc_ll_9_ul_unack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_9_LL9, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}

/* ------------------------------------------------------------------------ */


static void gllc_ll_11_ul_ack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_11_LL11, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_11_ul_ack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_11_LL11, GLLC_LLE_UL_LL_ACK_Q_EVT );
}


static void gllc_ll_11_ul_unack_wm_set_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_SET_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_11_LL11, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}


static void gllc_ll_11_ul_unack_wm_clr_evt
(
  dsm_watermark_type *wm_p,
  void *context
)
{
  gas_id_t gas_id = ( gas_id_t )(uint32)context;
  GLLC_CLR_LLE_UL_EVT( gas_id, GLLC_LL_SAPI_11_LL11, GLLC_LLE_UL_LL_UNACK_Q_EVT );
}

/*===========================================================================
===
===  FUNCTION      GLLC_INITIALISE()
===
===  DESCRIPTION
===
===    For use by LLC control task to initialise the LLC Process.
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

static void gllc_initialise( gas_id_t gas_id )
{
  uint32  lle;
  uint32  llme_ul_scheme;
  uint32  lle_ul_scheme;

  /* --------------------------------------------------------------------- */

  if( gas_id == GERAN_ACCESS_STRATUM_ID_1 )
  {
    geran_tcb_set(GERAN_TASK_ID_GLLC_1, rex_self());
  }

#if (defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM))

  else if( gas_id == GERAN_ACCESS_STRATUM_ID_2 )
  {
    geran_tcb_set(GERAN_TASK_ID_GLLC_2, rex_self());
  }

#endif /* (FEATURE_DUAL_SIM) || (FEATURE_TRIPLE_SIM) */


#ifdef FEATURE_TRIPLE_SIM

  else if( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
    geran_tcb_set(GERAN_TASK_ID_GLLC_3, rex_self());
  }

#endif /* FEATURE_TRIPLE_SIM */

  /* --------------------------------------------------------------------- */

  /* LLE initialisations. */

  for ( lle = 0; lle <= (uint32)GLLC_LL_LAST_SAPI; lle++ )
  {
    /* Initialise LL SAP Client Records. */

    gllc_ll_client_record[ gas_id ][lle].ll_dl_sig_get_mes_buf_fnc_ptr = NULL;
    gllc_ll_client_record[ gas_id ][lle].ll_dl_sig_put_mes_buf_fnc_ptr = NULL;
    gllc_ll_client_record[ gas_id ][lle].ll_dl_dat_put_mes_buf_fnc_ptr = NULL;

    /* ................................................................... */

    /* Initialise LLE queues and associated parameters. */

    (void)q_init( &gllc_cr_ul_q[gas_id][lle] );
    (void)q_init( &gllc_ll_ul_sig_q[gas_id][lle] );

    gllc_lle[gas_id][lle].ack.retrx_first_p = NULL;
    gllc_lle[gas_id][lle].ack.retrx_last_p  = NULL;

    gllc_lle[gas_id][lle].ack.reseq_first_p = NULL;
    gllc_lle[gas_id][lle].ack.reseq_last_p  = NULL;

    /* ................................................................... */

    /* Initialise LLE acknowledged data watermarks. */

    gllc_ll_ul_ack_wm[gas_id][lle].lo_watermark             = 0;
    gllc_ll_ul_ack_wm[gas_id][lle].hi_watermark             = 0;
    gllc_ll_ul_ack_wm[gas_id][lle].current_cnt              = 0;

    gllc_ll_ul_ack_wm[gas_id][lle].highest_cnt              = 0;
    gllc_ll_ul_ack_wm[gas_id][lle].total_rcvd_cnt           = 0;

    gllc_ll_ul_ack_wm[gas_id][lle].each_enqueue_func_ptr    = NULL;
    gllc_ll_ul_ack_wm[gas_id][lle].gone_empty_func_ptr      = NULL;
    gllc_ll_ul_ack_wm[gas_id][lle].non_empty_func_ptr       = NULL;
    gllc_ll_ul_ack_wm[gas_id][lle].lowater_func_ptr         = NULL;
    gllc_ll_ul_ack_wm[gas_id][lle].hiwater_func_ptr         = NULL;

    /* Initialise the ack watermark queue. Don't Exceed count is set to zero
       via the function below. Ensure that Don't Exceed count is set to
       appropriate value. */

    dsm_queue_init ( &gllc_ll_ul_ack_wm[gas_id][lle], 0, &gllc_ll_ul_ack_q[gas_id][lle]);

    /* ................................................................... */

    /* Initialise LLE unacknowledged data watermarks. */

    gllc_ll_ul_unack_wm[gas_id][lle].lo_watermark           = 0;
    gllc_ll_ul_unack_wm[gas_id][lle].hi_watermark           = 0;
    gllc_ll_ul_unack_wm[gas_id][lle].current_cnt            = 0;

    gllc_ll_ul_unack_wm[gas_id][lle].highest_cnt            = 0;
    gllc_ll_ul_unack_wm[gas_id][lle].total_rcvd_cnt         = 0;

    gllc_ll_ul_unack_wm[gas_id][lle].each_enqueue_func_ptr  = NULL;
    gllc_ll_ul_unack_wm[gas_id][lle].gone_empty_func_ptr    = NULL;
    gllc_ll_ul_unack_wm[gas_id][lle].non_empty_func_ptr     = NULL;
    gllc_ll_ul_unack_wm[gas_id][lle].lowater_func_ptr       = NULL;
    gllc_ll_ul_unack_wm[gas_id][lle].hiwater_func_ptr       = NULL;

    /* Initialise the unack watermark queue. Don't Exceed count is set to zero
       via the function below. Ensure that Don't Exceed count is set to
       appropriate value. */

    dsm_queue_init ( &gllc_ll_ul_unack_wm[gas_id][lle], 0, &gllc_ll_ul_unack_q[gas_id][lle]);


    /* ................................................................... */

    /* Initialise LLE timers. */

    if ( gas_id == GERAN_ACCESS_STRATUM_ID_1 )
    {
      rex_def_timer_ex
      (
        &gllc_timer_t200[gas_id][lle],
        gllc_timer_callback_1,
        (uint32)( (lle << 16) | GLLC_LLE_UL_TIMER_T200_EVT )
      );

      rex_def_timer_ex
      (
        &gllc_timer_t201[gas_id][lle],
        gllc_timer_callback_1,
        (uint32)( (lle << 16) | GLLC_LLE_UL_TIMER_T201_EVT )
      );
    }
#ifdef FEATURE_DUAL_SIM
    else if ( gas_id == GERAN_ACCESS_STRATUM_ID_2 )
    {
      rex_def_timer_ex
      (
        &gllc_timer_t200[gas_id][lle],
        gllc_timer_callback_2,
        (uint32)( (lle << 16) | GLLC_LLE_UL_TIMER_T200_EVT )
      );

      rex_def_timer_ex
      (
        &gllc_timer_t201[gas_id][lle],
        gllc_timer_callback_2,
        (uint32)( (lle << 16) | GLLC_LLE_UL_TIMER_T201_EVT )
      );
    }
#endif
#ifdef FEATURE_TRIPLE_SIM

    else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
    {
      rex_def_timer_ex
      (
        &gllc_timer_t200[gas_id][lle],
        gllc_timer_callback_3,
        (uint32)( (lle << 16) | GLLC_LLE_UL_TIMER_T200_EVT )
      );

      rex_def_timer_ex
      (
        &gllc_timer_t201[gas_id][lle],
        gllc_timer_callback_3,
        (uint32)( (lle << 16) | GLLC_LLE_UL_TIMER_T201_EVT )
      );
    }

#endif /* FEATURE_TRIPLE_SIM */

    /* ................................................................... */

    /* Initialise LLE event masking scheme record flags. */

    for
    (
      lle_ul_scheme = 0;
      lle_ul_scheme < (uint32)GLLC_LLE_NUM_SCHEMES;
      lle_ul_scheme++
    )
    {
      gllc_lle_ul_evt_mask_scheme_flags[gas_id][lle_ul_scheme][lle] = FALSE;
    }

    /* ................................................................... */

    /* Intialise LLE active event and mask bitmaps. */

    gllc_lle_ul_event_map[gas_id][lle] = GLLC_LLE_UL_NO_EVT;
    gllc_lle_ul_mask_map[gas_id][lle]  = (uint16)~GLLC_LLE_UL_NO_EVT;

    /* ................................................................... */

    /* Finally perform a full initialisation of the LLE. */

    gllc_lle_reset( gas_id, (gllc_ll_sapi_t)lle, GLLC_LLE_TLLI_UNASSIGNED );
  }

  /* --------------------------------------------------------------------- */

  /* Non LLE specific event masking scheme flags initialisations. */

  for
  (
    llme_ul_scheme = 0;
    llme_ul_scheme < (uint32)GLLC_LLME_NUM_SCHEMES;
    llme_ul_scheme++
  )
  {
    gllc_llme_ul_evt_mask_scheme_flags[gas_id][llme_ul_scheme] = FALSE;
  }

  /* --------------------------------------------------------------------- */

  /* LLE Watermark event notification, high, low, and dne initialisations. */

  /* SAPI-1 */
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_1_GMM].gone_empty_func_ptr     = gllc_ll_1_ul_ack_wm_clr_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_1_GMM].gone_empty_func_data    = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_1_GMM].non_empty_func_ptr      = gllc_ll_1_ul_ack_wm_set_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_1_GMM].non_empty_func_data     = (void *)gas_id;

  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_1_GMM].gone_empty_func_ptr   = gllc_ll_1_ul_unack_wm_clr_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_1_GMM].gone_empty_func_data  = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_1_GMM].non_empty_func_ptr    = gllc_ll_1_ul_unack_wm_set_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_1_GMM].non_empty_func_data   = (void *)gas_id;

  /* Flow control is not supported for GMM but set a maximum limit of the unacknowledged
     watermark occupancy of N maximal length frames. */

  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_1_GMM].dont_exceed_cnt       =
    (uint16)(gllc_max_N201_U_octets[ GLLC_LL_SAPI_1_GMM ] * 5);



  /* SAPI-3 */
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_3_LL3].gone_empty_func_ptr     = gllc_ll_3_ul_ack_wm_clr_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_3_LL3].gone_empty_func_data    = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_3_LL3].non_empty_func_ptr      = gllc_ll_3_ul_ack_wm_set_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_3_LL3].non_empty_func_data     = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_3_LL3].hiwater_func_ptr        = gsn_ll3_ul_ack_hiwater;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_3_LL3].hiwater_func_data       = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_3_LL3].lowater_func_ptr        = gsn_ll3_ul_ack_lowater;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_3_LL3].lowater_func_data       = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_3_LL3].hi_watermark            = GSN_LL3_ACK_HIWATER_CNT;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_3_LL3].lo_watermark            = GSN_LL3_ACK_LOWATER_CNT;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_3_LL3].dont_exceed_cnt         = GSN_LL3_ACK_DISCARD_CNT;


  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_3_LL3].gone_empty_func_ptr   = gllc_ll_3_ul_unack_wm_clr_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_3_LL3].gone_empty_func_data  = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_3_LL3].non_empty_func_ptr    = gllc_ll_3_ul_unack_wm_set_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_3_LL3].non_empty_func_data   = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_3_LL3].hiwater_func_ptr      = gsn_ll3_ul_unack_hiwater;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_3_LL3].hiwater_func_data     = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_3_LL3].lowater_func_ptr      = gsn_ll3_ul_unack_lowater;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_3_LL3].lowater_func_data     = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_3_LL3].hi_watermark          = GSN_LL3_UNACK_HIWATER_CNT;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_3_LL3].lo_watermark          = GSN_LL3_UNACK_LOWATER_CNT;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_3_LL3].dont_exceed_cnt       = GSN_LL3_UNACK_DISCARD_CNT;


  /* SAPI-5 */
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_5_LL5].gone_empty_func_ptr     = gllc_ll_5_ul_ack_wm_clr_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_5_LL5].gone_empty_func_data    = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_5_LL5].non_empty_func_ptr      = gllc_ll_5_ul_ack_wm_set_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_5_LL5].non_empty_func_data     = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_5_LL5].hiwater_func_ptr        = gsn_ll5_ul_ack_hiwater;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_5_LL5].hiwater_func_data       = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_5_LL5].lowater_func_ptr        = gsn_ll5_ul_ack_lowater;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_5_LL5].lowater_func_data       = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_5_LL5].hi_watermark            = GSN_LL5_ACK_HIWATER_CNT;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_5_LL5].lo_watermark            = GSN_LL5_ACK_LOWATER_CNT;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_5_LL5].dont_exceed_cnt         = GSN_LL5_ACK_DISCARD_CNT;


  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_5_LL5].gone_empty_func_ptr   = gllc_ll_5_ul_unack_wm_clr_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_5_LL5].gone_empty_func_data  = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_5_LL5].non_empty_func_ptr    = gllc_ll_5_ul_unack_wm_set_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_5_LL5].non_empty_func_data   = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_5_LL5].hiwater_func_ptr      = gsn_ll5_ul_unack_hiwater;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_5_LL5].hiwater_func_data     = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_5_LL5].lowater_func_ptr      = gsn_ll5_ul_unack_lowater;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_5_LL5].lowater_func_data     = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_5_LL5].hi_watermark          = GSN_LL5_UNACK_HIWATER_CNT;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_5_LL5].lo_watermark          = GSN_LL5_UNACK_LOWATER_CNT;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_5_LL5].dont_exceed_cnt       = GSN_LL5_UNACK_DISCARD_CNT;


  /* SAPI-7 */
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_7_SMS].gone_empty_func_ptr     = gllc_ll_7_ul_ack_wm_clr_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_7_SMS].gone_empty_func_data    = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_7_SMS].non_empty_func_ptr      = gllc_ll_7_ul_ack_wm_set_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_7_SMS].non_empty_func_data     = (void *)gas_id;

  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_7_SMS].gone_empty_func_ptr   = gllc_ll_7_ul_unack_wm_clr_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_7_SMS].gone_empty_func_data  = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_7_SMS].non_empty_func_ptr    = gllc_ll_7_ul_unack_wm_set_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_7_SMS].non_empty_func_data   = (void *)gas_id;

  /* Flow control is not supported for GSMS but set a maximum limit on the unacknowledged
     watermark occupancy of N maximal length frames. */

  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_7_SMS].dont_exceed_cnt =
      (uint16)(gllc_max_N201_U_octets[ GLLC_LL_SAPI_7_SMS ] * 5);



  /* SAPI-9 */
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_9_LL9].gone_empty_func_ptr     = gllc_ll_9_ul_ack_wm_clr_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_9_LL9].gone_empty_func_data    = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_9_LL9].non_empty_func_ptr      = gllc_ll_9_ul_ack_wm_set_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_9_LL9].non_empty_func_data     = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_9_LL9].hiwater_func_ptr        = gsn_ll9_ul_ack_hiwater;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_9_LL9].hiwater_func_data       = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_9_LL9].lowater_func_ptr        = gsn_ll9_ul_ack_lowater;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_9_LL9].lowater_func_data       = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_9_LL9].hi_watermark            = GSN_LL9_ACK_HIWATER_CNT;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_9_LL9].lo_watermark            = GSN_LL9_ACK_LOWATER_CNT;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_9_LL9].dont_exceed_cnt         = GSN_LL9_ACK_DISCARD_CNT;

  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_9_LL9].gone_empty_func_ptr   = gllc_ll_9_ul_unack_wm_clr_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_9_LL9].gone_empty_func_data  = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_9_LL9].non_empty_func_ptr    = gllc_ll_9_ul_unack_wm_set_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_9_LL9].non_empty_func_data   = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_9_LL9].hiwater_func_ptr      = gsn_ll9_ul_unack_hiwater;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_9_LL9].hiwater_func_data     = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_9_LL9].lowater_func_ptr      = gsn_ll9_ul_unack_lowater;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_9_LL9].lowater_func_data     = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_9_LL9].hi_watermark          = GSN_LL9_UNACK_HIWATER_CNT;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_9_LL9].lo_watermark          = GSN_LL9_UNACK_LOWATER_CNT;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_9_LL9].dont_exceed_cnt       = GSN_LL9_UNACK_DISCARD_CNT;


  /* SAPI-11 */
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_11_LL11].gone_empty_func_ptr   = gllc_ll_11_ul_ack_wm_clr_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_11_LL11].gone_empty_func_data  = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_11_LL11].non_empty_func_ptr    = gllc_ll_11_ul_ack_wm_set_evt;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_11_LL11].non_empty_func_data   = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_11_LL11].hiwater_func_ptr      = gsn_ll11_ul_ack_hiwater;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_11_LL11].hiwater_func_data     = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_11_LL11].lowater_func_ptr      = gsn_ll11_ul_ack_lowater;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_11_LL11].lowater_func_data     = (void *)gas_id;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_11_LL11].hi_watermark          = GSN_LL11_ACK_HIWATER_CNT;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_11_LL11].lo_watermark          = GSN_LL11_ACK_LOWATER_CNT;
  gllc_ll_ul_ack_wm[gas_id][GLLC_LL_SAPI_11_LL11].dont_exceed_cnt       = GSN_LL11_ACK_DISCARD_CNT;

  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_11_LL11].gone_empty_func_ptr = gllc_ll_11_ul_unack_wm_clr_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_11_LL11].gone_empty_func_data= (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_11_LL11].non_empty_func_ptr  = gllc_ll_11_ul_unack_wm_set_evt;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_11_LL11].non_empty_func_data = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_11_LL11].hiwater_func_ptr    = gsn_ll11_ul_unack_hiwater;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_11_LL11].hiwater_func_data   = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_11_LL11].lowater_func_ptr    = gsn_ll11_ul_unack_lowater;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_11_LL11].lowater_func_data   = (void *)gas_id;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_11_LL11].hi_watermark        = GSN_LL11_UNACK_HIWATER_CNT;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_11_LL11].lo_watermark        = GSN_LL11_UNACK_LOWATER_CNT;
  gllc_ll_ul_unack_wm[gas_id][GLLC_LL_SAPI_11_LL11].dont_exceed_cnt     = GSN_LL11_UNACK_DISCARD_CNT;

  /* --------------------------------------------------------------------- */

  /* LLME initialisations. */

  gllc_llme_reset(gas_id);

  /* TLLI Last Received, GEA, Cipher Key and IOV-UI must traverse general
     LLME Reset operations, and so must be reset individually */

  gllc_llme[gas_id].tlli_last_rxd       = GLLC_TLLI_UNASSIGNED;
  gllc_llme[gas_id].gea                 = GMMLLC_GEA_NONE;
  gllc_llme[gas_id].key_Kc[0]           = 0;
  gllc_llme[gas_id].key_Kc[1]           = 0;
  gllc_llme[gas_id].key_Kc128[0]        = 0;
  gllc_llme[gas_id].key_Kc128[1]        = 0;
  gllc_llme[gas_id].key_Kc128[2]        = 0;
  gllc_llme[gas_id].key_Kc128[3]        = 0;
  gllc_llme[gas_id].input_offset_IOV_UI = 0;

#ifdef FEATURE_DUAL_SIM

  /* Access Stratum ID is reset */

  gllc_llme[gas_id].gas_id              = gas_id;

#endif /* FEATURE_DUAL_SIM */

  /* --------------------------------------------------------------------- */

  /* Other initialisations. */

  /* Host the GMM Ready Timer on behalf of GMM for restart efficiency
     purposes. */

  rex_def_timer( &gllc_gmm_ready_timer[ gas_id], rex_self(), GLLC_GMM_RDY_TMR_SIG );

  /* --------------------------------------------------------------------- */

  MSG_GERAN_LOW_0_G( "GLC ME: Initialised");
  gllc_initialised[ gas_id ] = TRUE;

  /* --------------------------------------------------------------------- */

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)  || defined(FEATURE_QSH_MDUMP)
  /* do this once for 1st gas_id only, not per task instance */
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    gllc_qsh_init();
  }
#ifdef FEATURE_QSH_MDUMP
  qsh_add_thread_id(QSH_CLT_GLLC);
#endif  /* FEATURE_QSH_MDUMP */
#endif/* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP ||  FEATURE_QSH_MDUMP */

} /* end of gllc_initialise() */


/*****************************************************************************
***
***     Public Functions
***
*****************************************************************************/

#if defined (FEATURE_QSH_EVENT_METRIC) || defined (FEATURE_QSH_DUMP)
/*===========================================================================
===
===  FUNCTION      gllc_get_debug_data_ptr
===
===  DESCRIPTION
===
===   Retrieve debug data pointer for a gas_id
===
===  DEPENDENCIES
===
===   None.
===
===  RETURN VALUE
===
===   Pointer.
===
===  SIDE EFFECTS
===
===   None.
===
===========================================================================*/
__attribute__((section(".uncompressible.text")))
gllc_debug_data_t* gllc_get_debug_data_ptr(gas_id_t gas_id)
{
  gllc_debug_data_t *debug_data_ptr = &gllc_debug_data[0];

  /* validate the GAS ID */
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    debug_data_ptr = &gllc_debug_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)];
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Debug bad gas_id:%d",gas_id);
  }

  return debug_data_ptr;
}
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP */

/*===========================================================================
===
===  FUNCTION      GSM_LLC_TASK_X_FN_INIT()
===
===  DESCRIPTION
===
===  Only use this mechanism to perform memory init, hardware init, or other
===  startup init to satisfy dependency that follow. Avoid all HLOS mechanisms
===  that will block.
===
===  Code in this function will add to startup latency. Do not use large loops
===  that can be avoided.
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

void gsm_llc_task_x_fn_init( gas_id_t gas_id )
{

  /* --------------------------------------------------------------------- */


  /* Initialise LLC critical section */

  rex_init_crit_sect(&gllc_crit_sec[ gas_id ]);

  rex_init_crit_sect(&gllc_ul_msg_crit_sec[ gas_id ]);

  rex_init_crit_sect(&gllc_dl_msg_crit_sec[ gas_id ]);

  /* This is a mutex common to all GLLC tasks, only initialise in association with task 1 */
  if( GERAN_ACCESS_STRATUM_ID_1 == gas_id )
  {
    rex_init_crit_sect(&gllc_cipher_crit_sec);
  }


  /* --------------------------------------------------------------------- */

  /* Initialise non-LLE queues. */

  (void)q_init( &gllc_gmm_ul_q[ gas_id ] );
  (void)q_init( &gllc_grr_dl_q[ gas_id ] );

  /* --------------------------------------------------------------------- */

}

/*===========================================================================
===
===  FUNCTION      GSM_LLC_TASK_1_FN_INIT()
===
===  DESCRIPTION
===
===  Only use this mechanism to perform memory init, hardware init, or other
===  startup init to satisfy dependency that follow. Avoid all HLOS mechanisms
===  that will block.
===
===  Code in this function will add to startup latency. Do not use large loops
===  that can be avoided.
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

void gsm_llc_task_1_fn_init( void )
{
  gsm_llc_task_x_fn_init( GERAN_ACCESS_STRATUM_ID_1 );
}

#if (defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM))

/*===========================================================================
===
===  FUNCTION      GSM_LLC_TASK_2_FN_INIT()
===
===  DESCRIPTION
===
===  Only use this mechanism to perform memory init, hardware init, or other
===  startup init to satisfy dependency that follow. Avoid all HLOS mechanisms
===  that will block.
===
===  Code in this function will add to startup latency. Do not use large loops
===  that can be avoided.
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

void gsm_llc_task_2_fn_init( void )
{
  gsm_llc_task_x_fn_init( GERAN_ACCESS_STRATUM_ID_2 );
}

#endif /* (FEATURE_DUAL_SIM) || (FEATURE_TRIPLE_SIM) */


#ifdef FEATURE_TRIPLE_SIM

/*===========================================================================
===
===  FUNCTION      GSM_LLC_TASK_3_FN_INIT()
===
===  DESCRIPTION
===
===  Only use this mechanism to perform memory init, hardware init, or other
===  startup init to satisfy dependency that follow. Avoid all HLOS mechanisms
===  that will block.
===
===  Code in this function will add to startup latency. Do not use large loops
===  that can be avoided.
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

void gsm_llc_task_3_fn_init( void )
{
  gsm_llc_task_x_fn_init( GERAN_ACCESS_STRATUM_ID_3 );
}

#endif /* FEATURE_TRIPLE_SIM */


/*===========================================================================
===
===  FUNCTION      GLLC_TASK_X()
===
===  DESCRIPTION
===
===    REX task based scheduling framework for GPRS LLC functionality for
===    subscriptions.
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

void gllc_task_x
(
  gas_id_t gas_id
)
{
  dog_report_type     gllc_dog_rpt;

  /* Perform startup init here. Satisfy dependency of other tasks
     provided by this task. Do not linger here, startup latency is
     impacted. Higher priority tasks in the same group will preempt
     this task at this point. Do not rely upon priority alone as the
     sole mechanism to arbitrate resource contention with other tasks. */

  rcinit_handshake_startup(); /* required rcinit handshake */

  gllc_initialise(gas_id);

  gllc_dog_rpt = dog_hb_register_rex((rex_sigs_type)GLLC_DOG_RPT_TMR_SIG);

  /* Perform remaining init here. This is sequenced by a start
     signal delivered at the end of the sequence group processing. */



  /* Register GMM, GSMS, GSNDCP proxy clients on appropriate LL SAPs. */

  gllclient_reg_gmm_gsms_proxy(gas_id);



  /* Initialise logging. */

  llc_log_init(gas_id);



  /* Begin event based processing via REX scheduling. */

  for (;;)
  {
    /* Recind processor context until a relevent signal(s) becomes active. */

    gllc_task_sigs[gas_id] =
                      rex_wait
                      (
                        (
                          TASK_START_SIG       |
                          TASK_STOP_SIG        |
                          TASK_OFFLINE_SIG     |
                          GLLC_DOG_RPT_TMR_SIG |
                          GLLC_UL_MASTER_SIG   |
                          GLLC_DL_MASTER_SIG   |
                          GLLC_GMM_RDY_TMR_SIG |
                          GLLC_TEST_MODE_SIG
#ifdef FEATURE_QSH_EVENT_METRIC
                          |
                          GLLC_QSH_METRIC_SIG
#endif
                        )
                      );

    /* Process any active signals with the following priority. */

    if ( gllc_task_sigs[gas_id] & TASK_START_SIG )
    {
      (void)rex_clr_sigs( rex_self(), TASK_START_SIG );
    }

    if ( gllc_task_sigs[gas_id] & TASK_STOP_SIG )
    {
      /* Used in QTF. Clear down the signal and stop task to fix KW error. */
      (void)rex_clr_sigs( rex_self(), TASK_STOP_SIG );

      rcinit_handshake_term();

      break;
    }

    if ( gllc_task_sigs[gas_id] & TASK_OFFLINE_SIG )
    {
      /* Not used at present so just clear down the signal. */
      (void)rex_clr_sigs( rex_self(), TASK_OFFLINE_SIG );
    }

    if ( gllc_task_sigs[gas_id] & GLLC_DOG_RPT_TMR_SIG )
    {
      (void)rex_clr_sigs( rex_self(), GLLC_DOG_RPT_TMR_SIG );
      dog_hb_report( gllc_dog_rpt );
    }

    if ( gllc_task_sigs[gas_id] & GLLC_TEST_MODE_SIG )
    {
      gllc_test_mode_handler(gas_id);
    }

    if ( gllc_task_sigs[gas_id] & GLLC_GMM_RDY_TMR_SIG )
    {
      gllc_gmm_ready_timer_handler( gas_id );
    }

    if ( gllc_task_sigs[gas_id] & (GLLC_UL_MASTER_SIG | GLLC_DL_MASTER_SIG) )
    {
      gllc_event_handler( gas_id );
    }

#ifdef FEATURE_QSH_EVENT_METRIC
    if ( gllc_task_sigs[gas_id] & GLLC_QSH_METRIC_SIG )
    {
      gllc_handle_qsh_sig(gas_id);
    }
#endif
  }

  dog_hb_deregister( gllc_dog_rpt );

  /*UNREACHED*/
}

/*===========================================================================
===
===  FUNCTION      GLLC_TASK_1()
===
===  DESCRIPTION
===
===    REX task based scheduling framework for GPRS LLC functionality for 1st
===    subscription.
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
void gllc_task_1
(
  /* Required for REX, ignore. */

  /*lint -esym(715,unused) Prevent lint from complaining about the unused
    parameter 'dummy' which is required to conform to the rex_def_task()
    definition. */

  dword  unused
)
{
  NOTUSED(unused); /* silence compiler warnings of unused parameter */
  gllc_task_x(GERAN_ACCESS_STRATUM_ID_1);
}

#if (defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM))

/*===========================================================================
===
===  FUNCTION      GLLC_TASK_2()
===
===  DESCRIPTION
===
===    REX task based scheduling framework for GPRS LLC functionality for 2nd
===    subscription.
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

void gllc_task_2
(
  /* Required for REX, ignore. */

  /*lint -esym(715,unused) Prevent lint from complaining about the unused
    parameter 'dummy' which is required to conform to the rex_def_task()
    definition. */

  dword  unused
)
{
  NOTUSED(unused); /* silence compiler warnings of unused parameter */
  gllc_task_x(GERAN_ACCESS_STRATUM_ID_2);
}

#endif /* (FEATURE_DUAL_SIM) || (FEATURE_TRIPLE_SIM) */


#ifdef FEATURE_TRIPLE_SIM

/*===========================================================================
===
===  FUNCTION      GLLC_TASK_3()
===
===  DESCRIPTION
===
===    REX task based scheduling framework for GPRS LLC functionality for 3rd
===    subscription.
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

void gllc_task_3
(
  /* Required for REX, ignore. */

  /*lint -esym(715,unused) Prevent lint from complaining about the unused
    parameter 'dummy' which is required to conform to the rex_def_task()
    definition. */

  dword  unused
)
{
  NOTUSED(unused); /* silence compiler warnings of unused parameter */
  gllc_task_x(GERAN_ACCESS_STRATUM_ID_3);
}

#endif /* FEATURE_TRIPLE_SIM */

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP) || defined (FEATURE_QSH_MDUMP)
/*===========================================================================
===
===  FUNCTION      gllc_qsh_init
===
===  DESCRIPTION
===
===    Initializes GLLC QSH client.
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
void gllc_qsh_init ( void )
{
  qsh_client_reg_s client_reg;

  /* Initialize reg params */
  qsh_client_reg_init(&client_reg);

  client_reg.cb_action_support_mask = QSH_ACTION_NONE;
  client_reg.client = QSH_CLT_GLLC;
  client_reg.major_ver = GLLC_QSH_MAJOR_VER;
  client_reg.minor_ver = GLLC_QSH_MINOR_VER;
  client_reg.client_cb_ptr = gllc_qsh_data_capture_cb;

#ifdef FEATURE_QSH_DUMP
  /* Register callback for dumps */
  client_reg.cb_action_support_mask |= QSH_ACTION_DUMP_COLLECT;
  
  client_reg.dump_info.max_size_bytes = sizeof(gllc_qsh_tc_dump_collect_s) - sizeof(qsh_dump_hdr_s);
#endif /* FEATURE_QSH_DUMP */

#ifdef FEATURE_QSH_MDUMP
  client_reg.cb_action_support_mask |= QSH_ACTION_MDUMP;
#endif /* FEATURE_QSH_MDUMP */

#ifdef FEATURE_QSH_EVENT_METRIC
  /* Register callback for metrics */
  client_reg.cb_action_support_mask |= QSH_ACTION_METRIC_CFG;

  qsh_client_metric_cfg_init(gllc_qsh_metric_cfg_arr, GLLC_QSH_METRIC_DEFAULT_CONFIG_COUNT);

  /* Signaling metric enable for AS_ID_1 */
  gllc_qsh_metric_cfg_arr[0].id = GLLC_QSH_METRIC_DATA;
  gllc_qsh_metric_cfg_arr[0].subs_id = SYS_MODEM_AS_ID_1;
  gllc_qsh_metric_cfg_arr[0].fifo.element_size_bytes  = sizeof(gllc_qsh_metric_data_s);
  gllc_qsh_metric_cfg_arr[0].fifo.element_count_total = GLLC_QSH_METRIC_DATA_COUNT;

  /* Signaling metric enable for AS_ID_2 */
  gllc_qsh_metric_cfg_arr[1].id = GLLC_QSH_METRIC_DATA;
  gllc_qsh_metric_cfg_arr[1].subs_id = SYS_MODEM_AS_ID_2;
  gllc_qsh_metric_cfg_arr[1].fifo.element_size_bytes  = sizeof(gllc_qsh_metric_data_s);
  gllc_qsh_metric_cfg_arr[1].fifo.element_count_total = GLLC_QSH_METRIC_DATA_COUNT;

  client_reg.metric_info.metric_cfg_count             = GLLC_QSH_METRIC_DEFAULT_CONFIG_COUNT;
  client_reg.metric_info.metric_cfg_arr_ptr           = &gllc_qsh_metric_cfg_arr[0];
#endif /* FEATURE_QSH_EVENT_METRIC */

  qsh_client_reg(&client_reg);
}

/*===========================================================================
===
===  FUNCTION      gllc_qsh_data_capture_cb
===
===  DESCRIPTION
===
===  Callback function registered with QSH.
===  directive passed by QSH used in this function
===  INPUT  qsh_cb_params_ptr :
===  qsh_cb_params_ptr->action :   QSH_ACTION_METRIC_CFG and QSH_ACTION_DUMP_COLLECT
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
#ifndef FEATURE_QSH_MDUMP
__attribute__((section(".uncompressible.text")))
#else
QSH_MDUMP_FN_ATTR
#endif  /* FEATURE_QSH_MDUMP */
void gllc_qsh_data_capture_cb
(
  qsh_client_cb_params_s *   qsh_cb_params_ptr
)
{
  gas_id_t gas_id;

  if(qsh_cb_params_ptr == NULL)
  {
    ERR_FATAL("GLLC QSH Invalid qsh_cb_params_ptr",0,0,0);
  }
  
  switch (qsh_cb_params_ptr->action)
  {
    case QSH_ACTION_NONE:
    case QSH_ACTION_EVENT_CFG:
    case QSH_ACTION_ANALYSIS:
    case QSH_ACTION_METRIC_TIMER_EXPIRY:
      MSG_GERAN_HIGH_1("GLLC QSH action = %u EVENT/ANALYSIS/TIMER not supported", qsh_cb_params_ptr->action);
      break;

#ifdef FEATURE_QSH_EVENT_METRIC

    case QSH_ACTION_METRIC_CFG:

      gas_id = geran_map_nas_id_to_gas_id(qsh_cb_params_ptr->action_params.metric_cfg.subs_id);
      MSG_GERAN_HIGH_1_G("GLLC QSH cb action = %u METRIC_CFG", qsh_cb_params_ptr->action);

      /* Store qsh_cb_params_ptr struct for handler to pick up later.
      ** Only one message for METRIC_CFG no need for a queue */
      gllc_qsh_cb_param[gas_id] = *qsh_cb_params_ptr;

      /* Set mask to service message in LLC task time */
      if ( gas_id == GERAN_ACCESS_STRATUM_ID_1 )
      {
        (void)rex_set_sigs( geran_tcb_read(GERAN_TASK_ID_GLLC_1), GLLC_QSH_METRIC_SIG );
      }
#ifdef FEATURE_DUAL_SIM
      else if ( gas_id == GERAN_ACCESS_STRATUM_ID_2 )
      {
        (void)rex_set_sigs( geran_tcb_read(GERAN_TASK_ID_GLLC_2), GLLC_QSH_METRIC_SIG );
      }
#endif /* FEATURE_DUAL_SIM */      
#ifdef FEATURE_TRIPLE_SIM
      else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
      {
        (void)rex_set_sigs( geran_tcb_read(GERAN_TASK_ID_GLLC_3), GLLC_QSH_METRIC_SIG );
      }
#endif /* FEATURE_TRIPLE_SIM */

      break;

#endif /* FEATURE_QSH_EVENT_METRIC */


#ifdef FEATURE_QSH_DUMP
    case QSH_ACTION_DUMP_COLLECT:
      gllc_populate_qsh_dump_data(qsh_cb_params_ptr);
      break;

#endif/* FEATURE_QSH_DUMP */

#ifdef FEATURE_QSH_MDUMP
   case QSH_ACTION_MDUMP:
     gllc_qsh_mdump_collect(qsh_cb_params_ptr);
     break;
#endif /* FEATURE_QSH_MDUMP*/

    default:
      break;
  }
}
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP ||  FEATURE_QSH_MDUMP */

#ifdef FEATURE_QSH_DUMP
/*===========================================================================
===
===  FUNCTION      gllc_populate_qsh_dump_data
===
===  DESCRIPTION
===
===   note: use qsh_action_enabled() to check which action is enabled
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
void gllc_populate_qsh_dump_data
(
  qsh_client_cb_params_s *   qsh_cb_params_ptr
)
{
  /* Data passed to QSH when client has finished 
  ** processing its action requested by QSH */
  qsh_client_action_done_s client_action_done;   
  
  /* Ptr to qsh dump loaction for LLC */
  byte   *cb_param_dump_ptr;                    

  size_t remaining_size = qsh_cb_params_ptr->action_params.dump_collect.dump_iovec.size_bytes;
  size_t total_size_written = 0;

  remaining_size = qsh_cb_params_ptr->action_params.dump_collect.dump_iovec.size_bytes;
  cb_param_dump_ptr = (byte *)(qsh_cb_params_ptr->action_params.dump_collect.dump_iovec.addr);

  /* Determine which sub needs to dump data */
  if (QSH_DUMP_TAG_ENABLED((qsh_cb_params_ptr->action_params.dump_collect.dump_tag_mask),
                            GLLC_QSH_DUMP_TAG_DATA)
                          )
  {
    if(remaining_size >= sizeof(gllc_qsh_dump_tag_data_s))
    {
      gllc_get_qsh_dump_data((gllc_qsh_dump_tag_data_s *)cb_param_dump_ptr, GLLC_QSH_DUMP_TAG_DATA);
      cb_param_dump_ptr = cb_param_dump_ptr + (sizeof(gllc_qsh_dump_tag_data_s));
      remaining_size -= sizeof(gllc_qsh_dump_tag_data_s);
      total_size_written += sizeof(gllc_qsh_dump_tag_data_s);
    }
  }

  if (QSH_DUMP_TAG_ENABLED((qsh_cb_params_ptr->action_params.dump_collect.dump_tag_mask),
                            GLLC_QSH_DUMP_TAG_DATA_2)
                          )
  {
    if(remaining_size >= sizeof(gllc_qsh_dump_tag_data_s))
    {
      gllc_get_qsh_dump_data((gllc_qsh_dump_tag_data_s *)cb_param_dump_ptr, GLLC_QSH_DUMP_TAG_DATA_2);
      cb_param_dump_ptr = cb_param_dump_ptr + (sizeof(gllc_qsh_dump_tag_data_s));
      remaining_size -= sizeof(gllc_qsh_dump_tag_data_s);
      total_size_written += sizeof(gllc_qsh_dump_tag_data_s);
    }
  }

  /* Indicate writing of data for call-back action done*/
  qsh_client_action_done_init(&client_action_done);

  client_action_done.cb_params_ptr = qsh_cb_params_ptr;
  client_action_done.params.dump_collect.size_written_bytes = total_size_written;
  client_action_done.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;

  qsh_client_action_done(&client_action_done);
}

/*===========================================================================
===
===  FUNCTION      gllc_get_qsh_dump_data
===
===  DESCRIPTION
===
===  Description:
===
===  GLLC getting QSH dump data for Dump collection action.
===  It initialises qsh_dump_tag_hdr_s
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
void gllc_get_qsh_dump_data
(
  gllc_qsh_dump_tag_data_s *dump_data_ptr,
  gllc_qsh_dump_tag_e      tag
)
{
  gas_id_t gas_id  = GERAN_ACCESS_STRATUM_ID_1;

  qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_data_ptr,(uint16)tag,
                                   (sizeof(gllc_qsh_dump_tag_data_s)));

  if(tag == GLLC_QSH_DUMP_TAG_DATA_2)
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_2;
  }

  gllc_qsh_dump_data_get( gas_id, dump_data_ptr );
}  /* gllc_get_qsh_dump_data */

/*===========================================================================
===
===  FUNCTION      gllc_qsh_dump_data_get
===
===  DESCRIPTION
===
===  GLLC copies key variables for dump data collection
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
void gllc_qsh_dump_data_get(gas_id_t gas_id, gllc_qsh_dump_tag_data_s *dump_data_ptr )
{
  uint8 lle;
  uint8 qsh_sapi = 0;

  /* LLC_ME Info */
  dump_data_ptr->data.llme_state = gllc_llme[gas_id].state;
  dump_data_ptr->data.llme_xid_version = gllc_llme[gas_id].llc_version;
  dump_data_ptr->data.llme_new_tlli = gllc_llme[gas_id].tlli;
  dump_data_ptr->data.llme_old_tlli = gllc_llme[gas_id].tlli_old;

  /* Fill values for  SAPI variables */
  for(lle = 0; lle <= GLLC_LL_LAST_SAPI; lle++)
  {
    if((lle      != GLLC_LL_SAPI_7_SMS) && 
       (qsh_sapi  < QSH_SAPI_MAX_CNT))
  {
    /* Reliability class currently in use */
    dump_data_ptr->data.rel_class[qsh_sapi]           = gllc_lle[gas_id][lle].unack.reliability_class;

    /* LLC Sapi states */
    dump_data_ptr->data.gllc_lle_sapi_addr[qsh_sapi]  = gllc_sapi_addr_lookup[lle];
    dump_data_ptr->data.gllc_lle_state[qsh_sapi]      = gllc_lle[gas_id][lle].state;
    dump_data_ptr->data.gllc_lle_substate[qsh_sapi]   = gllc_lle[gas_id][lle].substate;

    /* LLC XID Info */
    dump_data_ptr->data.current_iov_i[qsh_sapi]       = gllc_lle[gas_id][lle].ack.input_offset_IOV_I;
    dump_data_ptr->data.current_t200[qsh_sapi]        = gllc_lle[gas_id][lle].gen.retrx_interval_T200;
    dump_data_ptr->data.current_n200[qsh_sapi]        = (uint8)gllc_lle[gas_id][lle].gen.max_retrx_cnt_N200;
    dump_data_ptr->data.current_t201[qsh_sapi]        = gllc_lle[gas_id][lle].ack.retrx_interval_T201;
    dump_data_ptr->data.current_n201_u[qsh_sapi]      = gllc_lle[gas_id][lle].unack.max_info_octets_N201_U;
    dump_data_ptr->data.current_n201_i[qsh_sapi]      = gllc_lle[gas_id][lle].ack.max_info_octets_N201_I;
    dump_data_ptr->data.current_md[qsh_sapi]          = gllc_lle[gas_id][lle].ack.max_reseq_buf_octets_mD;
    dump_data_ptr->data.current_mu[qsh_sapi]          = gllc_lle[gas_id][lle].ack.max_retrx_buf_octets_mU;
    dump_data_ptr->data.current_kd[qsh_sapi]          = gllc_lle[gas_id][lle].ack.max_reseq_buf_frames_kD;
    dump_data_ptr->data.current_ku[qsh_sapi]          = gllc_lle[gas_id][lle].ack.max_retrx_buf_frames_kU;

    /* LLC PDU Stats */
    dump_data_ptr->data.unack_l3pdu_tx_cnt[qsh_sapi]  = gllc_lle[gas_id][lle].diag.pdu_stats.unack_l3pdu_tx_cnt;
    dump_data_ptr->data.unack_octet_tx_cnt[qsh_sapi]  = gllc_lle[gas_id][lle].diag.pdu_stats.unack_octet_tx_cnt;
    dump_data_ptr->data.unack_l3pdu_rx_cnt[qsh_sapi]  = gllc_lle[gas_id][lle].diag.pdu_stats.unack_l3pdu_rx_cnt;

    dump_data_ptr->data.unack_octet_rx_cnt[qsh_sapi]  = gllc_lle[gas_id][lle].diag.pdu_stats.unack_octet_rx_cnt;
    dump_data_ptr->data.llpdu_tx_cnt[qsh_sapi]        = gllc_lle[gas_id][lle].diag.pdu_stats.llpdu_tx_cnt;
    dump_data_ptr->data.llpdu_rx_cnt[qsh_sapi]        = gllc_lle[gas_id][lle].diag.pdu_stats.llpdu_rx_cnt;
    dump_data_ptr->data.llpdu_fcs_err_cnt[qsh_sapi]   = gllc_lle[gas_id][lle].diag.pdu_stats.llpdu_fcs_err_cnt;
    dump_data_ptr->data.llpdu_tlli_err_cnt[qsh_sapi]  = gllc_lle[gas_id][lle].diag.pdu_stats.llpdu_tlli_err_cnt;
    dump_data_ptr->data.llpdu_addr_err_cnt[qsh_sapi]  = gllc_lle[gas_id][lle].diag.pdu_stats.llpdu_addr_err_cnt;
    dump_data_ptr->data.llpdu_short_err_cnt[qsh_sapi] = gllc_lle[gas_id][lle].diag.pdu_stats.llpdu_short_err_cnt;
      
      qsh_sapi++;
    }
  }
}
#endif /* FEATURE_QSH_DUMP */


#ifdef FEATURE_QSH_EVENT_METRIC
/*===========================================================================
===
===  FUNCTION      gllc_handle_qsh_sig
===
===  DESCRIPTION
===
===  Description:
===
===    Handler function to process QSH METRIC_CFG callback
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
void gllc_handle_qsh_sig(gas_id_t gas_id)
{
  /* Data passed to QSH when client has finished 
  ** processing its action requested by QSH */
  qsh_client_action_done_s  client_action_done;

  gllc_debug_data_t *debug_data_ptr = gllc_get_debug_data_ptr(gas_id);

  /* Short-cut to call-back parameters */
  qsh_client_cb_params_s *cb_params_ptr = &gllc_qsh_cb_param[gas_id];

  /* Short-cut to metric config parameters */
  qsh_client_metric_cfg_s *metric_config_params_ptr = &cb_params_ptr->action_params.metric_cfg;

  if (metric_config_params_ptr->action == QSH_METRIC_ACTION_START)
  {
    /* Set up start address and metric conext id and set metric event to active */
    debug_data_ptr->saved_cb_metric_dump_ptr          = metric_config_params_ptr->start_addr;
    debug_data_ptr->qsh_metric_context_id             = metric_config_params_ptr->metric_context_id;
    debug_data_ptr->gllc_qsh_metrics_capture_activate = TRUE;
  }
  else
  {
    debug_data_ptr->gllc_qsh_metrics_capture_activate = FALSE;
  }

  /* Indicate callback action done (once the action has been processed) */
  qsh_client_action_done_init(&client_action_done);
  client_action_done.cb_params_ptr    = cb_params_ptr;
  client_action_done.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;
  qsh_client_action_done(&client_action_done);

  /* Clear mask */
  if ( gas_id == GERAN_ACCESS_STRATUM_ID_1 )
  {
    (void)rex_clr_sigs( geran_tcb_read(GERAN_TASK_ID_GLLC_1), GLLC_QSH_METRIC_SIG );
  }
#ifdef FEATURE_DUAL_SIM
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_2 )
  {
    (void)rex_clr_sigs( geran_tcb_read(GERAN_TASK_ID_GLLC_2), GLLC_QSH_METRIC_SIG );
  }
#endif /* FEATURE_DUAL_SIM */
#ifdef FEATURE_TRIPLE_SIM
  else if ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
  {
    (void)rex_clr_sigs( geran_tcb_read(GERAN_TASK_ID_GLLC_3), GLLC_QSH_METRIC_SIG );
  }
#endif /* FEATURE_TRIPLE_SIM */
  else
  {
    MSG_GERAN_ERROR_0_G("GLC LL: Invalid GAS_ID");
  }
}

/*===========================================================================
===
===  FUNCTION      gllc_qsh_metric_capture
===
===  DESCRIPTION
===
===  Description:
===
===  Copies the last rcvd or sent signal into the current 10 second QSH metric
===  buffer and increments the next entry in preparation for any subsequent
===  signal.
===
===  input: QSH provides a new pointer to the next section in it's LLC metric
===         buffer. Assumption is this is zero initialised.
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
void gllc_qsh_metric_capture(gas_id_t gas_id)
{
  qsh_client_metric_log_done_s  client_metric_done;  /* data passed to QSH when client has finished */
  gllc_qsh_metric_data_s       *metric_ptr;          /* ptr to QSH dump loaction for LLC metric data */
  uint8                         lle;
  uint8                         qsh_sapi = 0;

  gllc_debug_data_t *debug_data_ptr = gllc_get_debug_data_ptr(gas_id);

  if(debug_data_ptr->gllc_qsh_metrics_capture_activate == TRUE)
  {
    /* Load last last sent/rcvd msg into into QSH metric dump loaction */
    metric_ptr = (gllc_qsh_metric_data_s *) (debug_data_ptr->saved_cb_metric_dump_ptr);

    metric_ptr->data.llme_state = gllc_llme[gas_id].state;
    metric_ptr->data.llme_new_tlli = gllc_llme[gas_id].tlli;

    /* Fill values for SAPI variables */
    for (lle = 0; lle <= GLLC_LL_LAST_SAPI; lle++)
    {
      if((lle      != GLLC_LL_SAPI_7_SMS) && 
         (qsh_sapi  < QSH_SAPI_MAX_CNT))
    {
        metric_ptr->data.current_n200[qsh_sapi]   = (uint8)gllc_lle[gas_id][lle].gen.max_retrx_cnt_N200;
        metric_ptr->data.current_t201[qsh_sapi]   = gllc_lle[gas_id][lle].ack.retrx_interval_T201;
        metric_ptr->data.current_n201_u[qsh_sapi] = gllc_lle[gas_id][lle].unack.max_info_octets_N201_U;
        metric_ptr->data.current_n201_i[qsh_sapi] = gllc_lle[gas_id][lle].ack.max_info_octets_N201_I;
    
        qsh_sapi++;
      }
    }

    qsh_client_metric_log_done_init(&client_metric_done);

    client_metric_done.log_reason = QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL;
    client_metric_done.metric_context_id =  debug_data_ptr->qsh_metric_context_id;
    client_metric_done.metric_id = GLLC_QSH_METRIC_DATA;
    client_metric_done.client = QSH_CLT_GLLC;

    /* QSH retuns pointer to the next LLC metric dump loaction */
    debug_data_ptr->saved_cb_metric_dump_ptr = qsh_client_metric_log_done(&client_metric_done);
  }
}
#endif /* FEATURE_QSH_EVENT_METRIC */

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================

FUNCTION  gllc_qsh_mdump_collect

DESCRIPTION
  Function to collect GL1 mini dump.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void gllc_qsh_mdump_collect(qsh_client_cb_params_s *param)
{
  gas_id_t                 gas_id;
  gllc_ll_sapi_t           sapi;

  /* Dump for all active SAPIs on all SUBs */
  for (gas_id = 0; gas_id < NUM_GERAN_DATA_SPACES; gas_id++)
  {
    for (sapi = GLLC_LL_SAPI_1_GMM; sapi <= GLLC_LL_LAST_SAPI; sapi++)
    {
      if (gllc_lle[gas_id][sapi].gen.pdp_active == TRUE)
      {
        qsh_mdump_collect_high(&gllc_lle[gas_id][sapi].state, sizeof(gllc_lle_state_t));
        qsh_mdump_collect_high(&gllc_lle[gas_id][sapi].substate, sizeof(gllc_lle_substate_t));
        qsh_mdump_collect_high(&gllc_lle[gas_id][sapi].gen, sizeof(gllc_lle_general_t));
        qsh_mdump_collect_high(&gllc_lle[gas_id][sapi].ack, sizeof(gllc_lle_ack_t));
        qsh_mdump_collect_high(&gllc_lle[gas_id][sapi].unack, sizeof(gllc_lle_unack_t));
      }
    }
    qsh_mdump_collect_high(&gllc_llme[gas_id].state, sizeof(gllc_llme_state_t));
    qsh_mdump_collect_high(&gllc_llme[gas_id].key_Kc, 2*sizeof(uint32));
    qsh_mdump_collect_high(&gllc_llme[gas_id].key_Kc128, 4*sizeof(uint32));
    qsh_mdump_collect_high(&gllc_llme[gas_id].gea, sizeof(gmmllc_gea_e_type));
    qsh_mdump_collect_high(&gllc_llme[gas_id].input_offset_IOV_UI, sizeof(uint32));
    qsh_mdump_collect_high(&gllc_llme[gas_id].fcs_errors_ctr, sizeof(uint32));
    qsh_mdump_collect_high(&gllc_llme[gas_id].short_errors_ctr, sizeof(uint32));
    qsh_mdump_collect_high(&gllc_llme[gas_id].addr_errors_ctr, sizeof(uint32));
    qsh_mdump_collect_high(&gllc_llme[gas_id].frmr_errors_ctr, sizeof(uint32));
    qsh_mdump_collect_high(&gllc_llme[gas_id].tlli_errors_ctr, sizeof(uint32));
    qsh_mdump_collect_high(&gllc_gmm_ready_timer_active[gas_id], sizeof(boolean));
  }

  qsh_mdump_prioritize_client(QSH_CLT_GLLC, QSH_CLT_GSNDCP);
  qsh_mdump_prioritize_client(QSH_CLT_GLLC, QSH_CLT_GRLCUL);
  qsh_mdump_prioritize_client(QSH_CLT_GLLC, QSH_CLT_GRLCDL);
  qsh_mdump_prioritize_client(QSH_CLT_GLLC, QSH_CLT_GMAC);
  qsh_mdump_prioritize_client(QSH_CLT_GLLC, QSH_CLT_GRR);
}
#endif /* FEATURE_QSH_MDUMP */

/*** EOF: don't remove! ***/

