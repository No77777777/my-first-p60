/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     G S M   T I M E R   S E R V I C E S

GENERAL DESCRIPTION
   This module contains the GSM timer drivers.  The VSTMR timers
   are supported by this module.

EXTERNALIZED FUNCTIONS
  gstmr_init
    Procedure that registers the GSTMR ISR handler and unmasks the GSTMR
    interrupt.
  gstmr_gsm_core_reset
    Procedure that takes the GSTMR out of reset.
  gstmr_register_handler
    Procedure to register a handler for the TDMA strobe processing.
  gstmr_deregister_handler
    Procedure to deregister a handler for the TDMA strobe processing.
  gstmr_delay_interrupt
    Procedure to specify when the ARM should get the TDMA interrupt.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/vtmrs.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
03/01/18   ksb      CR2164555 Geran support to send Global Time Services Information TO GPS module
27/01/17   sk       CR1104513 Fix for the crash during panic reset in IDLE mode and DTM mode
12/05/16   rv       CR998395: Excessive F3 logging in  GERAN L1 
11/10/15   tsk      CR937467: Relocate dynamic RxD handler to end of gstmr_frame_tick_process.
11/08/15   tsk      CR929882: Handle RxD exit processing.
09/10/15   akh      CR912421 FN/QS not correct for active ttg, sleep ttg resp delayed by 1s
11/09/15   dp       CR905777 Apply limit of time tracking when desired is outside limit
07/07/15   npt      CR860846 Clean up any pending frame extension when deregistering VSTMR
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
11/05/15   df       CR834120 Delay sleep if VSTMR frame extension in progress
15/04/15   pa        CR822841: Split Rx/Tx GL1 TRM changes
07/05/15   npt      CR821672 GERAN power logging update   
20/04/15   ip       CR811868 Dual Multisim mode code changes + debug overhaul
31/03/15   pa       CR815621:GRM restructuring for TH 2.0. 
23/01/15   npt      CR697236 Mainline FEATURE_GERAN_VSTMR
17/11/14   npt      CR642426 Added power reporting log packet
04/09/14   zf       CR702512 Correction in detecting GSTMR interrupt pending
20/08/14   npt      CR709922 De-register VSTMR from task context if going to sleep on opt3
18/08/14   df       CR708449 Add qs print to aid debugging 
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
09/06/14   cja      CR677926 Do not have vstmr snapshot if vstmr not registered
12/05/14   ssh      CR661514 Additional protection around panic reset handling [workaround for buffer overflow]
06/05/14   npt      CR649118 Do not apply large negative time adjustment
27/04/14   pg       CR655866 Guard against re-registering VSTMR event table when already registered
07/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
04/04/14   cah      CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
27/01/14   cah      CR605800 PCH decode failures due to incorrect HW FN after GS 1 GSTMR->VSTMR-GSTMR swapping
24/01/14   cah      CR598552 enable sleep for DSDS GSTMR+VSTMR
09/01/14   cah      CR598075 DSDS: PCH decode failures observed on sub 1 after voice call on sub 2
08/01/14   cah      fixes for VSTMR stability issues.
15/10/13   cah      created new module for common geran vtmrs api's.


===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif /* CUST_H */
#include "target.h"
#include "comdef.h"
#include "vstmr_geran.h"
#include "vtmrs_g.h"  /*GERAN VSTMR interface*/
#include "gmutex.h"
#include "mcs_hwio.h"
#include "geran_msgs.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "log.h"
#include "log_codes_gsm.h"
#include "log_codes_gsm_v.h"

#include "qurt.h"

#include "rex.h"

#include "gl1_mutex.h"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "gl1_msg_g.h"
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "gl1_defs.h"
#include "gl1_defs_g.h"

#include "l1_drx.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "l1_isr.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hw_clk_ctl_g.h"
#include "gl1_hwi.h"
#ifdef FEATURE_GSM_L4_TRACE_BUFF
#include "l4/cust/msm_syscall.h"
#endif /* FEATURE_GSM_L4_TRACE_BUFF */
#include "rfm.h"
#include "l1_task.h"
#include "l1_utils.h"
#ifdef FEATURE_GERAN_VP
#include <assert.h>
#endif /* FEATURE_GERAN_VP */

#include "string.h"

#ifdef FEATURE_WCDMA
#include "wl1api.h"
#endif /* FEATURE_WCDMA */

#include "msgr.h"
#include "geran_msg.h"

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#include "l1_os.h"
#include "l1_task.h"

static void vstmr_isr( vstmr_geran_event_tbl_s *event_tbl, void *arg, uint32 pending );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static void vstmr_g1_isr( vstmr_geran_event_tbl_s *event_tbl, void *arg, uint32 pending );
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

#if defined ( FEATURE_TRIPLE_SIM )
static void vstmr_g2_isr( vstmr_geran_event_tbl_s *event_tbl, void *arg, uint32 pending );
#endif /* FEATURE_TRIPLE_SIM */




/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

#define GL1_MAX_NO_TRACE_BUFFER_ENTRIES  50

extern supporting_frame_counters_t frame_counters[];

/*===========================================================================

FUNCTION vstmr_deactivate

DESCRIPTION
 this function clears the VSTMR for a the given subscription id. This should be called when L1 receives the
 MPH_STOP_GSM_MODE_REQ

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_deactivate(gas_id_t gas_id)
{
    int as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    /*check there  is VSTMR view registered on this GAS ID*/
    if(gtmrs_data[as_idx].geran_view)
     {
        vstmr_geran_event_cancel( &gtmrs_data[as_idx].geran_event_tbl,0 );
        vstmr_geran_interrupt_deregister( &gtmrs_data[as_idx].geran_event_tbl );

        vstmr_geran_event_tbl_deregister( &gtmrs_data[as_idx].geran_event_tbl );
        MSG_GERAN_HIGH_1_G("DEACTIVATE: de-register VSTRM on GAS ID %d ",gas_id);

        vstmr_geran_complete_frame_ext(gtmrs_data[as_idx].geran_view);

        gtmrs_data[as_idx].geran_view = NULL;
     }
    else
    {
      MSG_GERAN_HIGH_1_G("DEACTIVATE: fail on GAS ID %d ",gas_id);
    }
}

/*===========================================================================

FUNCTION VSTMR_ENABLE_INTERRUPT

DESCRIPTION
   This function enables the vstmr interrupt.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_enable_interrupt (gtmrs_hw_t gtmrs_hw, gas_id_t gas_id)
{
   vstmr_gstmr_t  gstmr_offset;
   int            as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   gstmr_offset.all = 0;

   /* Create the GSM view */
   MSG_GERAN_HIGH_1_G ("GERAN VSTMR enable interrupt %d ",gas_id );


   switch(gtmrs_hw)
   {
     case GSTMR_0:
     gtmrs_data[as_idx].geran_view = vstmr_geran_view_init ( VSTMR_RTC_GERAN_SUB0,
                                                           VSTMR_GERAN_GSTMR_VIEW,
                                                           gstmr_offset );

   gtmrs_data[as_idx].vstmr_isr_callback = vstmr_isr;
   break;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

   case GSTMR_1:
      gtmrs_data[as_idx].geran_view = vstmr_geran_view_init ( VSTMR_RTC_GERAN_SUB1,
                                                             VSTMR_GERAN_GSTMR_VIEW,
                                                             gstmr_offset );

    gtmrs_data[as_idx].vstmr_isr_callback = vstmr_g1_isr;
     break;
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

     default:
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_0_G("Unsupported VSTMR config !");
   break;

   }

   if (gtmrs_data[as_idx].geran_view == NULL)
   {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_0_G("Failed to create GERAN VSTMR view");
   }
   else
   {
      MSG_GERAN_HIGH_1_G ("GERAN VSTMR %d view successfully created",gas_id );
   }

}

/*===========================================================================

FUNCTION VSTMR_ISR

DESCRIPTION
   This is the main TDMA ISR handler.  It is used to vector the application
   handlers that were instantiated via gstmr_register_handler() calls.

   NOTE:  MAX_NUM_REGISTRATIONS is (MAX_NUM_ENTRIES-1) by design.  This
   is such so that there is always one NULL entry in the table of handlers.
   As a result this function can blindly cycle through until it reaches a
   NULL entry in the table.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_isr( vstmr_geran_event_tbl_s *event_tbl, void *arg, uint32 pending  )
{
   gtmrs_data_t          *gtmrs_data_p;
   gas_id_t         gas_id = gstmr_determine_gas_id( GSTMR_0 );
   sys_modem_as_id_e_type as_id  = geran_map_gas_id_to_nas_id( gas_id );
   int                    as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
   boolean                asynch_dbg = gl1_asynch_int_dbg( gas_id );

   gtmrs_data_p = &gtmrs_data[as_idx];

   if ( pending )
   {
     GDRV_ISR_LOCK(gas_id);

     GL1_HANDLER_PERFORMANCE_LOGGING(START_GSTMR_ISR,gas_id);

     gtmrs_data_p->tc_adjust_qs = 0;
     gtmrs_data_p->tc_set       = 0;

     /* Always clear when gstmr runs so if this occurs first then fast int ignored */
     gstmr_set_asynch_int_executing( FALSE, gas_id );

     /*
      * The asynch int has failed at the ending goto sleep decision, however the whole frame ISR
      * has effectively been executed for this frame already, and so there is no need to execute
      * in this case. This will avoid running the same effective code twice (and reduce power)
      * and the requirement to unwind any changes already performed is removed.
      */
     if ( !gl1_drx_get_asynch_int_ignore_gstmr(gas_id) )
     {
       /* Reset asynch PCH offset to ensure next cycle is treated cleanly */
       gl1_drx_set_pch_offset( 0, gas_id );

       /* Save the QS count when we start the GSTMR ISRs */
       gtmrs_data_p->gstmr_qs_start = gstmr_rd_qsymbol_count_geran( as_id );
       gtmrs_data_p->gstmr_ustmr_start = slpc_get_tick();

       /* Vstmr has ticked after slamming, so FN can now be read correctly */
       vstmr_reset_drift(as_idx);

       /* Invoke the registered handler functions for the TDMA time strobe */
       if ( asynch_dbg )
       {
         MSG_GERAN_HIGH_4_G( "VSTMR execute FN:%d start_qs:%d prev_fn_end_qs:%d gstmr_asynch_int_executing:%d",
                   GSTMR_GET_FN_GERAN(gas_id), gtmrs_data_p->gstmr_qs_start, gtmrs_data_p->gstmr_qs_end, gstmr_get_asynch_int_executing(gas_id) );
       }
#ifndef FEATURE_GSM_RX_TX_SPLIT
       /* process any pending Tx config commands */
       mdsp_issue_tx_config_command_if_required(gas_id);
#endif
       if ( mdsp_commands_pending(gas_id) )
       {
          /* Trigger the async command so that FW picks up the commands immediately */
      /* One of the Scenario:
      |--ISR--ASYNC2FW-------MDSPCmd(Task) |ASYNC2FW --ISR-- ASYNC2FW
      |FTick SWFN              |FTick+1      SWFN+1
      |    HWFN              |         HWFN+1
      After executing registered handlers on Ftick, GL1 issues ASYNC cmd to FW with HWFN and FW
      increments HWFN by 1 and executes command written on mdsp interface at HWFN+1. If GL1 receives a message
      in the task context after issuing ASYNC2FW and writes onto the mdsp interface then ASYNC2FW should be issued
      in the Ftick+1 before executing registered handlers. SWFN and HWFN are read from registered handler, therfore
      at Ftick+1 before executing resgitsered handlers, GL1 still has SWFN and HWFN in it's data structures.
      At Ftick+1, pass SWFN+1 in the mdsp_async_intf_send_immediate_cmd function which would pass HWFN to FW
      and as a result FW will execute  command at HWFN+1 as expected by GL1. */

      mdsp_async_intf_send_immediate_cmd((ADD_FN(GSTMR_GET_FN_GERAN(gas_id),1)), FALSE,gas_id);
          MSG_GERAN_HIGH_0_G("ASYNC CMD: issued commands before gstmr handlers");
       }

       /* Execute any registered handlers */
       gstmr_frame_tick_process(gas_id);

       if ( asynch_dbg )
       {
           /* Save the QS count when we end the GSTMR ISRs */
           gtmrs_data_p->gstmr_qs_end = gstmr_rd_qsymbol_count_geran( as_id );
       }

       if(l1_check_for_panic_reset(gas_id))
       {
           uint32 curr_fn = gl1_get_FN(gas_id);
           if(SUB_FN(curr_fn,l1_get_panic_reset_received_frame_no(gas_id)) > NO_OF_FRAMES_DELAY_FOR_PANIC_RESET_TRIGGER)
           {
             l1_do_panic_reset_after_delay(gas_id);
           }
           (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
       }
     }
     else
     {
       gstmr_asynch_silent_frame_isr( gas_id );
     }

     #ifdef FEATURE_GSM_RX_DIVERSITY
       gl1_msg_process_rxd_active(gas_id);
     #endif /* FEATURE_GSM_RX_DIVERSITY */

     GL1_HANDLER_PERFORMANCE_LOGGING(STOP_GSTMR_ISR,gas_id);

     GDRV_ISR_SAVE_UNLOCK(gas_id);
   }
}


/*===========================================================================


DESCRIPTION
   This is the main TDMA ISR 1 handler.  It is used to vector the application
   handlers that were instantiated via gstmr_register_handler() calls.

   NOTE:  MAX_NUM_REGISTRATIONS is (MAX_NUM_ENTRIES-1) by design.  This
   is such so that there is always one NULL entry in the table of handlers.
   As a result this function can blindly cycle through until it reaches a
   NULL entry in the table.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static void vstmr_g1_isr( vstmr_geran_event_tbl_s *event_tbl, void *arg, uint32 pending  )
{
   gtmrs_data_t          *gtmrs_data_p;
   gas_id_t         gas_id = gstmr_determine_gas_id( GSTMR_1 );
   sys_modem_as_id_e_type as_id  = geran_map_gas_id_to_nas_id( gas_id );
   int                    as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
   boolean                asynch_dbg = gl1_asynch_int_dbg( gas_id );

   gtmrs_data_p = &gtmrs_data[as_idx];

   if ( pending )
   {
     GDRV_ISR_LOCK(gas_id);

     GL1_HANDLER_PERFORMANCE_LOGGING(START_GSTMR_ISR,gas_id);

     gtmrs_data_p->tc_adjust_qs = 0;
     gtmrs_data_p->tc_set       = 0;

     /* Always clear when gstmr runs so if this occurs first then fast int ignored */
     gstmr_set_asynch_int_executing( FALSE, gas_id );

     /*
      * The asynch int has failed at the ending goto sleep decision, however the whole frame ISR
      * has effectively been executed for this frame already, and so there is no need to execute
      * in this case. This will avoid running the same effective code twice (and reduce power)
      * and the requirement to unwind any changes already performed is removed.
      */
     if ( !gl1_drx_get_asynch_int_ignore_gstmr(gas_id) )
     {
       /* Reset asynch PCH offset to ensure next cycle is treated cleanly */
       gl1_drx_set_pch_offset( 0, gas_id );

       /* Save the QS count when we start the GSTMR ISRs */
       gtmrs_data_p->gstmr_qs_start = gstmr_rd_qsymbol_count_geran( as_id );
       gtmrs_data_p->gstmr_ustmr_start = slpc_get_tick();

       /* Vstmr has ticked after slamming, so FN can now be read correctly */
       vstmr_reset_drift(as_idx);

       /* Invoke the registered handler functions for the TDMA time strobe */
       if ( asynch_dbg )
       {
         MSG_GERAN_HIGH_4_G( "VSTMR 1 execute FN:%d start_qs:%d prev_fn_end_qs:%d gstmr_asynch_int_executing:%d",
                   GSTMR_GET_FN_GERAN(gas_id), gtmrs_data_p->gstmr_qs_start, gtmrs_data_p->gstmr_qs_end, gstmr_get_asynch_int_executing(gas_id) );
       }
#ifndef FEATURE_GSM_RX_TX_SPLIT
       /* process any pending Tx config commands */
       mdsp_issue_tx_config_command_if_required(gas_id);
#endif
       if ( mdsp_commands_pending(gas_id) )
       {
          /* Trigger the async command so that FW picks up the commands immediately */
      mdsp_async_intf_send_immediate_cmd((ADD_FN(GSTMR_GET_FN_GERAN(gas_id),1)), FALSE,gas_id);
          MSG_GERAN_HIGH_0_G("ASYNC CMD: issued commands before gstmr handlers");
       }

       /* Execute any registered handlers */
       gstmr_frame_tick_process(gas_id);

       if ( asynch_dbg )
       {
           /* Save the QS count when we end the GSTMR ISRs */
           gtmrs_data_p->gstmr_qs_end = gstmr_rd_qsymbol_count_geran( as_id );
       }

       if(l1_check_for_panic_reset(gas_id))
       {
           uint32 curr_fn = gl1_get_FN(gas_id);
           if(SUB_FN(curr_fn,l1_get_panic_reset_received_frame_no(gas_id)) > NO_OF_FRAMES_DELAY_FOR_PANIC_RESET_TRIGGER)
           {
             l1_do_panic_reset_after_delay(gas_id);
           }
           (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);		   
       }
     }
     else
     {
       gstmr_asynch_silent_frame_isr( gas_id );
     }

     #ifdef FEATURE_GSM_RX_DIVERSITY
       gl1_msg_process_rxd_active(gas_id);
     #endif /* FEATURE_GSM_RX_DIVERSITY */

     GL1_HANDLER_PERFORMANCE_LOGGING(STOP_GSTMR_ISR,gas_id);

     GDRV_ISR_SAVE_UNLOCK(gas_id);
   }
}

#endif /*#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )*/

/*===========================================================================


DESCRIPTION
   This is the main TDMA ISR 2 handler.  It is used to vector the application
   handlers that were instantiated via gstmr_register_handler() calls.

   NOTE:  MAX_NUM_REGISTRATIONS is (MAX_NUM_ENTRIES-1) by design.  This
   is such so that there is always one NULL entry in the table of handlers.
   As a result this function can blindly cycle through until it reaches a
   NULL entry in the table.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#if defined ( FEATURE_TRIPLE_SIM )
void vstmr_g2_isr( vstmr_geran_event_tbl_s *event_tbl, void *arg, uint32 pending  )
{
   gtmrs_data_t          *gtmrs_data_p;
   gas_id_t               gas_id = gstmr_determine_gas_id( GSTMR_2 );
   sys_modem_as_id_e_type as_id  = geran_map_gas_id_to_nas_id( gas_id );
   int                    as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
   boolean                asynch_dbg = gl1_asynch_int_dbg( gas_id );

   gtmrs_data_p = &gtmrs_data[as_idx];

   if ( pending )
   {
     GDRV_ISR_LOCK(gas_id);

     GL1_HANDLER_PERFORMANCE_LOGGING(START_GSTMR_ISR,gas_id);

     gtmrs_data_p->tc_adjust_qs = 0;
     gtmrs_data_p->tc_set       = 0;

     /* Always clear when gstmr runs so if this occurs first then fast int ignored */
     gstmr_set_asynch_int_executing( FALSE, gas_id );

     /*
      * The asynch int has failed at the ending goto sleep decision, however the whole frame ISR
      * has effectively been executed for this frame already, and so there is no need to execute
      * in this case. This will avoid running the same effective code twice (and reduce power)
      * and the requirement to unwind any changes already performed is removed.
      */
     if ( !gl1_drx_get_asynch_int_ignore_gstmr(gas_id) )
     {
       /* Reset asynch PCH offset to ensure next cycle is treated cleanly */
       gl1_drx_set_pch_offset( 0, gas_id );

       /* Save the QS count when we start the GSTMR ISRs */
       gtmrs_data_p->gstmr_qs_start = gstmr_rd_qsymbol_count_geran( as_id );
       gtmrs_data_p->gstmr_ustmr_start = slpc_get_tick();

       /* Vstmr has ticked after slamming, so FN can now be read correctly */
       vstmr_reset_drift(as_idx);

       /* Invoke the registered handler functions for the TDMA time strobe */
       if ( asynch_dbg )
       {
         MSG_GERAN_HIGH_3_G( "GSTMR execute FN:%d start_qs:%d gstmr_asynch_int_executing:%d",
                   GSTMR_GET_FN_GERAN(gas_id), gtmrs_data_p->gstmr_qs_start, gstmr_get_asynch_int_executing(gas_id) );
       }
#ifndef FEATURE_GSM_RX_TX_SPLIT
       /* process any pending Tx config commands */
       mdsp_issue_tx_config_command_if_required(gas_id);
#endif
       if ( mdsp_commands_pending(gas_id) )
       {
          /* Trigger the async command so that FW picks up the commands immediately */
      mdsp_async_intf_send_immediate_cmd((ADD_FN(GSTMR_GET_FN_GERAN(gas_id),1)), FALSE,gas_id);
          MSG_GERAN_HIGH_0_G("ASYNC CMD: issued commands before gstmr handlers");
       }

       /* Execute any registered handlers */
       gstmr_frame_tick_process(gas_id);

       if ( asynch_dbg )
       {
         MSG_GERAN_HIGH_2_G( "GSTMR execute FN:%d end_qs:%d",
                   GSTMR_GET_FN_GERAN(gas_id), gstmr_rd_qsymbol_count_geran(as_id));
       }

       if(l1_check_for_panic_reset(gas_id))
       {
           uint32 curr_fn = gl1_get_FN(gas_id);
           if(SUB_FN(curr_fn,l1_get_panic_reset_received_frame_no(gas_id)) > NO_OF_FRAMES_DELAY_FOR_PANIC_RESET_TRIGGER)
           {
             l1_do_panic_reset_after_delay(gas_id);
           }
          (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
       }
     }
     else
     {
       gstmr_asynch_silent_frame_isr( gas_id );
     }

     #ifdef FEATURE_GSM_RX_DIVERSITY
       gl1_msg_process_rxd_active(gas_id);
     #endif /* FEATURE_GSM_RX_DIVERSITY */

     GL1_HANDLER_PERFORMANCE_LOGGING(STOP_GSTMR_ISR,gas_id);

     GDRV_ISR_SAVE_UNLOCK(gas_id);
   }
}


#endif /*FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION vstmr_adjust_terminal_count_geran

DESCRIPTION
     This function elongates the current frame, so expect the next TDMA tick to
      occur a little later than usual.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_adjust_terminal_count_geran (int16 quarter_symbol_count
                                  ,sys_modem_as_id_e_type as_id)
{
   LOG_GSM_GSTMR_ADJ_C_type  *log_ptr;
   uint32 new_terminal_cnt = quarter_symbol_count*G_STMR_QSYM_TO_RAW_13MHZ;
   vstmr_geran_ext_frame_u  gstmr_ext_frame_var;
   gas_id_t gas_id = geran_map_nas_id_to_gas_id( as_id );
   int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
   gstmr_ext_frame_var.value.value = atomic64_read((&(gtmrs_data[as_idx].geran_view->ext_frame_data.value)));

   if ( quarter_symbol_count != 0 )
   {
       gtmrs_data[as_idx].drift_qs = quarter_symbol_count;

       if (( quarter_symbol_count > DEF_QSYMB_TIME_TRACKING_ADJ ) && ( gstmr_ext_frame_var.ext_frame_tc == 0 ))
       {
           new_terminal_cnt += DEF_RAW_13MHZ_TERMINAL_COUNT;
           vstmr_geran_set_frame_tc ( gtmrs_data[as_idx].geran_view,
                                      new_terminal_cnt );
       }
       else
       {
         if(quarter_symbol_count < -DEF_QSYMB_TIME_TRACKING_ADJ)
         {
           MSG_GERAN_MED_3_G("%d qs offset < %d qs (most allowed in negative direction) - applying %d qs",
                             quarter_symbol_count, 
                             -DEF_QSYMB_TIME_TRACKING_ADJ, 
                             -DEF_QSYMB_TIME_TRACKING_ADJ);

           quarter_symbol_count = -DEF_QSYMB_TIME_TRACKING_ADJ;
         }

         vstmr_geran_view_adjust_offset(gtmrs_data[as_idx].geran_view, new_terminal_cnt);
       }
   }
   MSG_GERAN_MED_3_G("GSTMR timing adjust=%d (qs), TC_value=%d (13M), current ext_frame_tc = %d",
           quarter_symbol_count, new_terminal_cnt, gstmr_ext_frame_var.ext_frame_tc);

   /* Try to allocate memory for the log packet */
   log_ptr = (LOG_GSM_GSTMR_ADJ_C_type  *)log_alloc(LOG_GSM_GSTMR_ADJ_C, sizeof(LOG_GSM_GSTMR_ADJ_C_type));
   if (log_ptr != NULL)
   {
      log_ptr->tc_adjust = quarter_symbol_count;
      log_commit((void *)log_ptr);
   }

#ifdef DEBUG_GSTMR_ADJUST_TRACE
   /* Log the adjustment to the trace buffer */
   gstmr_adjust_trace(quarter_symbol_count, -1,gas_id);
#endif /* DEBUG_GSTMR_ADJUST_TRACE */
}



/*===========================================================================

FUNCTION vstmr_wake_up

DESCRIPTION
  Register VSTMR/GSTMR table and interrupt

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_wake_up ( gas_id_t gas_id )
{
   int as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    /* Always clear to avoid freezing out the gstmr */
   gstmr_set_asynch_int_executing(FALSE,gas_id);

   gl1_drx_set_asynch_int_ignore_gstmr(FALSE,gas_id);

   if (gtmrs_data[as_idx].geran_event_tbl.event_tbl_data.irq_id == 0)
   {
      /* Register the GSM table */
      vstmr_geran_event_tbl_register ( &gtmrs_data[as_idx].geran_event_tbl,
                                       gtmrs_data[as_idx].geran_view,
                                       VSTMR_INT_NORMAL );
      MSG_GERAN_HIGH_1_G ("GERAN VSTMR tbl register vstmr_wake_up  GAS_ID: %d ",gas_id );
      /* Register the GSTMR interrupt */
      vstmr_geran_interrupt_register ( &gtmrs_data[as_idx].geran_event_tbl,
                                       gtmrs_data[as_idx].vstmr_isr_callback,
                                       gtmrs_data[as_idx].geran_isr_cb_arg );
   }
   else
   {
      MSG_GERAN_ERROR_1_G ("GERAN VSTMR tbl not previously deregistered  GAS_ID: %d ",gas_id );
   }
}
/*===========================================================================

FUNCTION vstmr_schedule_event

DESCRIPTION
  Register VSTMR/GSTMR table and interrupt

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_schedule_event ( gas_id_t gas_id, uint16 fn, boolean next )
{
   int as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
   vstmr_gstmr_t  gstmr_evnt_expire_cnt;

   gstmr_evnt_expire_cnt.all = 0;

   /* Schedule GSTMR interrupt periodic event */
   gstmr_evnt_expire_cnt = vstmr_geran_get_current_gstmr( gtmrs_data[as_idx].geran_view );
   if ( next == TRUE )
   {
     gstmr_evnt_expire_cnt.frame_num = ((gstmr_evnt_expire_cnt.frame_num + ( fn + 1 )) & ((VSTMR_GERAN_RTC_MODULO/VSTMR_GERAN_FRAME_RANGE) - 1));
   }
   else
   {
     gstmr_evnt_expire_cnt.frame_num = fn;
   }
   gstmr_evnt_expire_cnt.raw_13M = ( 0 );

   MSG_GERAN_HIGH_2_G(" VSTMR wake up FN %d fn adjust %d",gstmr_evnt_expire_cnt.frame_num, fn);

   vstmr_geran_event_sched ( &gtmrs_data[as_idx].geran_event_tbl,
                             0,
                             gstmr_evnt_expire_cnt );
}



/*===========================================================================

FUNCTION VSTMR_GET_TC_ADJUST

DESCRIPTION
  This function returns the terminal count setting for any pending timing adjustement
  which will not get exercised before GSTMR HW is set into sleep state.

DEPENDENCIES
  None

RETURN VALUE
  int32 active_tc_adjust in 13MHz

SIDE EFFECTS
  None

===========================================================================*/
int32 vstmr_get_tc_adjustment(gas_id_t gas_id)
{
   int32 active_tc_adjust = 0;
   uint32 terminal_cnt = 0;
   vstmr_geran_ext_frame_u  gstmr_ext_frame_var;
   int           as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   gstmr_ext_frame_var.value.value = atomic64_read((&(gtmrs_data[as_idx].geran_view->ext_frame_data.value)));

   terminal_cnt = gstmr_ext_frame_var.ext_frame_tc;
   if (terminal_cnt != 0)
   {
   active_tc_adjust = terminal_cnt - DEF_RAW_13MHZ_TERMINAL_COUNT;
   }

   MSG_GERAN_HIGH_1_G(" pending VSTMR TC adjustment %d (13MHz)", active_tc_adjust);

   return active_tc_adjust;
}


/*
 * Read symbol counter
 */
uint32 vstmr_rd_symbol_count( gas_id_t gas_id )
{
   return ( gstmr_rd_qsymbol_count_geran( geran_map_gas_id_to_nas_id(gas_id) ) >> 2 );
}

/*
 * Read Quarter symbol counter
 */
uint32 vstmr_rd_qsymbol_count_geran( sys_modem_as_id_e_type as_id )
{
   uint32 gsm_time;
   vstmr_gstmr_t  current_gstmr;
   int            as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(geran_map_nas_id_to_gas_id(as_id));
   //MSG_GERAN_HIGH_3("vstmr_rd_qsymbol_count_geran as_id:%d as_idx:%d, gview =%d", as_id, as_idx, gtmrs_data[as_idx].geran_view );
   //MSG_GERAN_HIGH_1("HW reg %d", gtmrs_data[as_idx].gtmrs_hw) ;

   ASSERT(gtmrs_data[as_idx].geran_view !=NULL);
   current_gstmr = vstmr_geran_get_current_gstmr ( gtmrs_data[as_idx].geran_view );
   gsm_time = ( current_gstmr.raw_13M * G_STMR_RAW_13MHZ_TO_QSYM ) >> 16;

   return ( gsm_time );
}

/*
 * Read odd-even frame
 */
uint32 vstmr_rd_odd_even( gas_id_t gas_id )
{
   uint32 odd_even;
   vstmr_gstmr_t  current_gstmr;
   int           as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );

   current_gstmr = vstmr_geran_get_current_gstmr ( gtmrs_data[as_idx].geran_view );

   odd_even = current_gstmr.frame_mod4;

   odd_even &= 0x1;

   return( odd_even );
}

/*
 * Read new G_STMR frame number (range:0-3)
 */
uint32 vstmr_rd_mod4_frame_num( gas_id_t gas_id )
{
   uint32 frame_num;
   vstmr_gstmr_t  current_gstmr;
   int           as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );

   current_gstmr = vstmr_geran_get_current_gstmr ( gtmrs_data[as_idx].geran_view );

   frame_num = current_gstmr.frame_mod4;

   return( frame_num );
}

/*
 * Read GERAN VSTMR frame number (range:0 - (GERAN_VSTMR_MAX_FN - 1))
 */
uint32 vstmr_rd_modMaxFN_frame_num( gas_id_t gas_id )
{
   uint32 frame_num;
   vstmr_gstmr_t  current_gstmr;
   int           as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );

   current_gstmr = vstmr_geran_get_current_gstmr ( gtmrs_data[as_idx].geran_view );

   frame_num = current_gstmr.frame_num;

   return( frame_num );
}


/*===========================================================================
FUNCTION get_vstmr_FN_modMaxFN

DESCRIPTION
   This function retrieves the gstmr FN mod4096 that was stored
   at the time of the software frame tick.

DEPENDENCIES
  None

RETURN VALUE
  uint32  -- the framenumber mod 4096

SIDE EFFECTS
  None
===========================================================================*/
uint32 get_vstmr_FN_modMaxFN( sys_modem_as_id_e_type as_id  )
{
    return(gtmrs_data[geran_map_nas_id_to_gas_id(as_id)].gstmr_FN_modMaxFN);
}

/*===========================================================================
FUNCTION reset_vstmr_FN_modMaxFN

DESCRIPTION
   This function sets the gstmr FN mod4096 to 4095, to match hardware reset.

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void reset_vstmr_FN_modMaxFN (gas_id_t gas_id )
{
   gtmrs_data[gas_id].gstmr_FN_modMaxFN = GERAN_VSTMR_MAX_FN - 1;
}

/*===========================================================================

FUNCTION geran_rd_hw_frame_count_vstmr

DESCRIPTION
  HW GSM hyperframe counter based on  modMaxFN_frame_num

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint32 geran_rd_hw_frame_count_vstmr(gas_id_t gas_id )
{

/* For VSTMR targets calculate the frame number usinge the modMaxFN_frame_num at the start of the frame, */
   /* with the current modMaxFN_frame_num and the SW FN (gstmr_FN) */
   uint32 hwfn;
   uint32 fn_modMaxFN;
   int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   gdrivers_FN_mtx_lock( gas_id );

   fn_modMaxFN = vstmr_rd_modMaxFN_frame_num( gas_id );

   if (fn_modMaxFN == gtmrs_data[as_idx].gstmr_FN_modMaxFN)
   {
     hwfn = gtmrs_data[as_idx].gstmr_FN;
   }
   else
   {
     if (fn_modMaxFN < gtmrs_data[as_idx].gstmr_FN_modMaxFN)
     {
        fn_modMaxFN += GERAN_VSTMR_MAX_FN;
     }
     hwfn = ADD_FN(gtmrs_data[as_idx].gstmr_FN, fn_modMaxFN);
     hwfn = SUB_FN(hwfn, gtmrs_data[as_idx].gstmr_FN_modMaxFN);
   }
   gdrivers_FN_mtx_unlock( gas_id );

   return (hwfn);
}

/*===========================================================================

FUNCTION gl1_ustmr_vstmr_snapshot_before_sleep

DESCRIPTION
  None
  This function takes a snapshot of GSM FN, USTMR and VSTMR values. This function is called just before L1 goes
  to sleep mode.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_ustmr_vstmr_snapshot_before_sleep(gas_id_t gas_id)
{
   vstmr_gstmr_t vstmr_gstmr;
   int    as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   /* get current GFN */
   gtmrs_data[gas_id].GsmFNSleepStart = GSTMR_GET_FN_GERAN(gas_id);

   /* get current (full, 56 bit) XO time*/
   gtmrs_data[gas_id].UstmrSleepStartTime = VSTMR_XO_READ_FULL();

   /* get current GSTMR time - converted from lower 24 bits of XO time i.e. USTMR time */
   vstmr_gstmr = vstmr_geran_xo_to_gstmr(gtmrs_data[as_idx].geran_view,
                                          (gtmrs_data[gas_id].UstmrSleepStartTime & VSTMR_XO_MASK));
   /* store GSTMR frame time */
   gtmrs_data[gas_id].GstmrSleepStartTime = vstmr_gstmr.raw_13M;

}

/*===========================================================================

FUNCTION vstmr_geran_vstmr_deregister

DESCRIPTION
  Deregister VSTMR from other task context than GERAN ISR.
Current VSTMR design, updated to solve certain GSTMR/VSTMR race conditions,
requires the usage of vstmr_geran_interrupt_deregister() at all times when
we are running de-registration in a task context different than our own ISR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_geran_vstmr_deregister  (gas_id_t gas_id)
{
  int    as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  vstmr_geran_event_tbl_s *event_tbl = &gtmrs_data[as_idx].geran_event_tbl;

  MSG_GERAN_HIGH_1_G("GSTMR: de-registering VSTMR, qs=%d", 
                     gstmr_rd_qsymbol_count_geran( geran_map_gas_id_to_nas_id(gas_id) ));

  if (event_tbl->view != NULL)
  {
    vstmr_geran_event_cancel( event_tbl,0 );
  /* Interrupt deregister from other task context than our own ISR */
    vstmr_geran_interrupt_deregister( event_tbl );
    vstmr_geran_event_tbl_deregister( event_tbl );

    vstmr_geran_complete_frame_ext(gtmrs_data[as_idx].geran_view);
  }
  else
  {
    MSG_GERAN_ERROR_0_G("GSTMR: do not de-register VSTMR - already de-registered");
  }
}


/*===========================================================================

FUNCTION vstmr_geran_vstmr_deregister_from_isr

DESCRIPTION
  Deregister VSTMR from GERAN ISR task context
Current VSTMR design, updated to solve certain GSTMR/VSTMR race conditions,
requires the usage of vstmr_geran_interrupt_deregister_from_isr() at all times when
we are running de-registration in our own ISR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_geran_vstmr_deregister_from_isr (gas_id_t gas_id)
{
  int           as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  MSG_GERAN_HIGH_1_G("GSTMR: de-registering VSTMR from ISR, qs=%d",
                     gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));

  vstmr_geran_event_cancel( &gtmrs_data[as_idx].geran_event_tbl,0 );
  /* Interrupt deregister from our own ISR */
  vstmr_geran_interrupt_deregister_from_isr ( &gtmrs_data[as_idx].geran_event_tbl );
  vstmr_geran_event_tbl_deregister( &gtmrs_data[as_idx].geran_event_tbl );

  vstmr_geran_complete_frame_ext(gtmrs_data[as_idx].geran_view);

  }

/*===========================================================================

FUNCTION vstmr_snapshot_before_vstmr_deregister

DESCRIPTION
  Store current SW FN and fn_modMaxFN, to be used to set FN when register vstmr

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_snapshot_before_vstmr_deregister (gas_id_t gas_id)
{
  int    as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  vstmr_geran_event_tbl_s *event_tbl = &gtmrs_data[as_idx].geran_event_tbl;

  if (event_tbl->view != NULL)
  {
    /* Store current sw FN and FN_modMaxFN (in case they are changed whilst deregistered)*/
    gdrivers_FN_mtx_lock(gas_id);
    gtmrs_data[gas_id].vstmr_deregister_sw_fn = gtmrs_data[gas_id].gstmr_FN;
    gtmrs_data[gas_id].vstmr_deregister_fn_modMaxFN = gtmrs_data[gas_id].gstmr_FN_modMaxFN;
    gdrivers_FN_mtx_unlock(gas_id);

    MSG_GERAN_HIGH_2_G("Snapshot at VSTMR de-register: sw fn %d, vstmr fn %d", gtmrs_data[gas_id].vstmr_deregister_sw_fn, gtmrs_data[gas_id].vstmr_deregister_fn_modMaxFN );
  }
  else
  {
   MSG_GERAN_ERROR_0_G("GSTMR: no snapshot as VSTMR already de-registered");
  }
}

/*===========================================================================
FUNCTION VSTMR_X2G_WAKEUP_INIT

DESCRIPTION
   This function modifies necessary registers after X2G wake-up.
   Registers the vstmr_gsmtr and sets FN

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_x2g_wakeup_init( sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  vstmr_wake_up ( gas_id );
  vstmr_schedule_event_and_set_fn(gas_id);

  /* Always clear to avoid freezing out the gstmr */
  gstmr_set_asynch_int_executing( FALSE,gas_id );
  /* Reset asynch PCH offset to ensure next cycle is treated cleanly */
  gl1_drx_set_pch_offset( 0, gas_id );
  gl1_drx_set_asynch_int_ignore_gstmr( FALSE,gas_id );

  gtmrs_data[gas_id].init_odd_even_check = TRUE;
}

/*===========================================================================

FUNCTION vstmr_schedule_event_and_set_fn

DESCRIPTION
  Register VSTMR/GSTMR table and interrupt and set SW FN to match

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_schedule_event_and_set_fn ( gas_id_t gas_id)
{
   int           as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
   vstmr_gstmr_t  gstmr_evnt_expire_cnt;
   uint32 new_fn;
   uint16 fn = 0;

   gstmr_evnt_expire_cnt.all = 0;

   /* Schedule GSTMR interrupt periodic event */
   gstmr_evnt_expire_cnt = vstmr_geran_get_current_gstmr( gtmrs_data[as_idx].geran_view );

   /* If too close to the first frame tick, scehdule first tick for frame later */
   if (gstmr_evnt_expire_cnt.raw_13M > (G_STMR_MAX_QS_FOR_REGISTER * G_STMR_QSYM_TO_RAW_13MHZ))
   {
    fn = 1;
   }

   /* Set frame number, qs for GSTMR interrupt periodic event */
   gstmr_evnt_expire_cnt.frame_num = ((gstmr_evnt_expire_cnt.frame_num + ( fn + 1 )) & (GERAN_VSTMR_MAX_FN - 1));
   gstmr_evnt_expire_cnt.raw_13M = ( 0 );

   /* Set gstmr FN to match schdeuled event */
   new_fn = ADD_FN(gtmrs_data[gas_id].vstmr_deregister_sw_fn, gstmr_evnt_expire_cnt.frame_num);
   new_fn = SUB_FN(new_fn, gtmrs_data[gas_id].vstmr_deregister_fn_modMaxFN);
   /* Add correction for VSTMR frame wrap around */
   if (gtmrs_data[gas_id].vstmr_deregister_fn_modMaxFN > gstmr_evnt_expire_cnt.frame_num)
   {
    new_fn = ADD_FN(new_fn,GERAN_VSTMR_MAX_FN);
   }
  /* Set SW FN to the frame before first frame tick */
   new_fn = SUB_FN(new_fn,1);
   GSTMR_SET_FN_GERAN(new_fn, gas_id);
   if (gstmr_evnt_expire_cnt.frame_num != 0)
   {
    gtmrs_data[gas_id].gstmr_FN_modMaxFN = gstmr_evnt_expire_cnt.frame_num - 1;
   }
   else
   {
    gtmrs_data[gas_id].gstmr_FN_modMaxFN = (GERAN_VSTMR_MAX_FN - 1);
   }

   /* Set VSFTMR for schduled frame tick */
   vstmr_geran_event_sched ( &gtmrs_data[as_idx].geran_event_tbl,
               0,
               gstmr_evnt_expire_cnt );

   MSG_GERAN_HIGH_3_G(" VSTMR FN %d fn adjust %d, gstmr FN %d",gtmrs_data[gas_id].gstmr_FN_modMaxFN, fn, new_fn);
}


/*===========================================================================

FUNCTION vstmr_is_frame_extension_in_progress

DESCRIPTION
  When a frame extension is applied in frame n, the VSTMR ext_frame_tc is not
  cleared until frame n+1. So if we attempt to sleep at the end of frame n then
  the sleep duration calculations in gl1_hw_sleep_gsm_start_sleep() will
  incorrectly not include the extension. Therefore, don't allow sleep while
  extensions are in progress.

RETURN VALUE
  TRUE  - Frame extension is in progress, do not sleep.
  FALSE - Frame extension not required (the normal case) or has completed,
          allow sleep.

SIDE EFFECTS
  None

===========================================================================*/
boolean vstmr_is_frame_extension_in_progress( gas_id_t gas_id)
{
  vstmr_geran_ext_frame_u  gstmr_ext_frame_var;
  int                      as_idx              = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  
  gstmr_ext_frame_var.value.value = atomic64_read((&(gtmrs_data[as_idx].geran_view->ext_frame_data.value)));
  if (gstmr_ext_frame_var.ext_frame_tc != 0)
  {
    MSG_GERAN_HIGH_1_G("DRX: Don't sleep, frame extension in progress, ext_frame_tc=%d",
                       gstmr_ext_frame_var.ext_frame_tc);
    return TRUE;
  }
  return FALSE;
}


/*===========================================================================

FUNCTION vstmr_fnqs_rd_disabled_for_slamming

DESCRIPTION
  When TS allocation is changed / frame slamming happens, then FN/QS cannot be read 
  reliably till next vstmr tick. In that situation, we cannot send FN/QS information
  to GPS/GTS, and we should wait for next vstmr tick.

RETURN VALUE
  TRUE  - Frame slamming is in progress, so FN/QS cannot be read reliably
  FALSE - Frame slamming is not in progress (normal case), so FN/QS can be read

SIDE EFFECTS
  None

===========================================================================*/
boolean vstmr_fnqs_rd_disabled_for_slamming( gas_id_t gas_id )
{
  int as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);  
 boolean ret_val;
  int16 drift_qs;

  drift_qs = gtmrs_data[as_idx].drift_qs;

  if( (drift_qs > DEF_QS_GPS_TOLERANCE) || (drift_qs < -DEF_QS_GPS_TOLERANCE) )
  {
    /* Large Drift (>16qs)
     *
     * FN cannot be read correctly due to slamming (large diff generally due to a different TS).
     * Wait for next vstmr tick before reading FN/QS.
     */
    ret_val = TRUE;
  }
  else
  {
     /* Small Drift (-16qs to 16qs) */
     ret_val = FALSE;
  }

  return ret_val;
}

/*===========================================================================

FUNCTION vstmr_get_drift

DESCRIPTION
  Get drift in QS

RETURN VALUE
  Drift in QS
  
SIDE EFFECTS
  None

===========================================================================*/
int16 vstmr_get_drift( gas_id_t gas_id )
{
  int as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  return gtmrs_data[as_idx].drift_qs;
}


/*===========================================================================

FUNCTION vstmr_reset_drift

DESCRIPTION
  After vstmr tick, allow FN/QS information to be sent to GPS/GTS

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void vstmr_reset_drift( int as_idx )
{
  gtmrs_data[as_idx].drift_qs = 0;
}


