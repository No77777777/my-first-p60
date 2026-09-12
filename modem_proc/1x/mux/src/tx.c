/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   M A I N    T R A N S M I T     T A S K

GENERAL DESCRIPTION
  This module contains the Main Transmit task initialization and
  startup processing.  It also contains the main processing loop
  for the Main Transmit task.

EXTERNALIZED FUNCTIONS
  tx_task
    This is the entrance procedure for the Main Transmit task.
    It contains the main processing loop for the task.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The tx_task function contains the Main Transmit task.  It must be
  specified in a call to rex_def task and started by the Main Control
  task.

  Copyright (c) 1992 - 2013 Qualcomm Technologies, Inc. 
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/tx.c_v   1.2   06 Sep 2002 09:25:22   varavamu  $
$Header: //components/rel/1x.mpss/8.0/mux/src/tx.c#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/14/13   dkb     Process muxmsgr messages in all Tx states.
03/05/13   srk     Replace existing watchdog implementation with the new
                   Dog Heartbeat Mechanism.
11/07/12   cjb     Mainlining FEATURE_MODEM_1X_SUPPORTS_RCINIT feature.
03/07/12   srk     Replaced MODEM_LEGACY_STARTED with RCINIT_RCEVT_INITGROUPS.
02/13/12   srk     Moved init fucntions after task start.
01/19/12   srk     Fixed Compiler Warnings
12/20/11   srk     Added RCInit Framework.
12/20/11   srk     Feature clean up: Mainlined FEATURE_MODEM_1X_SUPPORTS_Q6
09/02/11   srk     Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
07/26/11   cjb     Move txc_crit_sect init/deinit to txc_task().
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
08/19/10   jtm     Removed FEATURE_ACP support.
03/04/10   jj      Replaced deprecated task_xxx APIs with tmc_xxx APIs.
06/02/09   jj      Added include of newly splitted rx_v.h
04/03/09   jj      Removed Target specific feaaturization as part of
                   CMI effort.
03/20/09   jtm     Included txcmc_v.h.
12/02/08   adw     Commented out _v.h includes that no longer exist inside 1x.
07/11/05   fh      Cleaned up header file including from MUX:
                     changed txc.h to txcmc.h
09/11/03   yll     Added the featurization for ACP include files.
06/02/03   bkm     Lint cleanup.
09/06/02   va      Cleaned up lint errors.
06/07/02   jqi     Updated with multimode task operation interface.
03/06/00   ry      Added IS2000 featurization
01/17/00   lcc     Corrected message level to take care of warning during
                   compilation.
05/10/99   kmp     Changed T_ACP to FEATURE_ACP
08/02/94   jca     Added T_ACP conditional compiles to support ISS1 release.
01/06/94   jah     Changed watchdog iface to use dog_report().
09/01/92   jai     Uncommmented calls to txc_subtask and txc_powerup_init.
07/27/92   jca     Added "break" in "for" loop to facilitate PC unit testing.
07/09/92   jca     Added processing for TASK_STOP_SIG and TASK_OFFLINE_SIG.
07/07/92   arh     Updated for new mc.h. Checked into PVCS for DMSS use.
04/16/92   jca     Created module.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "1x_variation.h"
#include "comdef.h"
#include "modem_1x_defs.h"

#include "target.h"
#include "rex.h"
#include "task.h"
#include "tx_v.h"
#include "txcmc.h"
#include "txcmc_i.h"
#include "txccommon_i.h"
#include "msg.h"
#include "m1x_diag.h"
#include "mc_v.h"
#include "muxmsgr.h"
#include "rcinit.h"

/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* Create TCB pointer */
rex_tcb_type* tx_tcb_ptr = NULL;

/* <EJECT> */
/*===========================================================================

FUNCTION TX_INIT

DESCRIPTION
  This function initializes the critical section and txc powerup.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void tx_init(void)
{
  rex_init_crit_sect(&txc_crit_sect);

  /*------------------------*/
  /* Perform initialization */
  /*------------------------*/
  txc_powerup_init();

} /* tx_init() */


/*===========================================================================

FUNCTION TX_TASK_INIT

DESCRIPTION
  This function initializes the tcb pointer and other pre initializations that
  needs to be done only after TCB_PTR is updated.

DEPENDENCIES
  Must be called from "tx" task context.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void tx_task_init(void)
{
  /* Update the TX TCB pointer */
  TX_TCB_PTR = rex_self();

  /* Initialize the MSGR for TX task */
  muxmsgr_init( TX_TASK );

  /* Initialize dog parameters */
  txc_watchdog_init();

  /* Initialize timers */
  rex_def_timer (&txc_rf_timer, TX_TCB_PTR, TXC_RF_TIMER_SIG );
  rex_def_timer( &txc_failsafe_timer, TX_TCB_PTR, TXC_TIMER_SIG );
  rex_def_timer( &txc_shutdown_wait_timer, TX_TCB_PTR, TXC_TIMER_SIG );

} /* tx_task_init */

/*===========================================================================

FUNCTION TX_TASK

DESCRIPTION
  This procedure is the entrance procedure for the Main Transmit task.
  It contains the main processing loop for the Main Transmit task which
  controls the activation of the ACPTX and TXC subtasks.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void tx_task
(
  dword dummy
    /* Parameter required for REX.  Tell lint to ignore it. */
    /*lint -esym(715,dummy) */
)
{

  rex_sigs_type rex_signals_mask;  /* Mask of signals returned by rex */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*---------------------*/
  /* Task initialization */
  /*---------------------*/
  tx_task_init();

  /* Start handshake with RCINIT to start the init process */
  rcinit_handshake_startup();


  for (;;) { /* Never exit this loop... */

    rex_signals_mask =
      rex_wait( TXC_DOG_RPT_SIG | TX_CDMA_CMD_Q_SIG |
                TASK_OFFLINE_SIG | TASK_STOP_SIG | TXC_MUXMSGR_Q_SIG );

    if ( rex_signals_mask & TXC_DOG_RPT_SIG )
    {
      /*-------------------------------*/
      /*      Kick watchdog            */
      /*-------------------------------*/
      txc_dog_report();
    }

    /*---------------------------------------------------------*/
    /* Check if powerdown command signal was set.  If set then */
    /* clear signal, process task stop procedure, and proceed. */
    /*---------------------------------------------------------*/
    if ((rex_signals_mask & TASK_STOP_SIG) != 0) {
      M1X_MSG( MUX, LEGACY_MED,
        "TASK_STOP_SIG received" );
      (void) rex_clr_sigs( TX_TCB_PTR, TASK_STOP_SIG );
    }

    /*-------------------------------------------------------*/
    /* Check if offline command signal was set.  If set then */
    /* clear signal, process task offline procedure, and     */
    /* proceed.                                              */
    /*-------------------------------------------------------*/
    if ((rex_signals_mask & TASK_OFFLINE_SIG) != 0) {
      M1X_MSG( MUX, LEGACY_MED,
        "TASK_OFFLINE_SIG received" );
      (void) rex_clr_sigs( TX_TCB_PTR, TASK_OFFLINE_SIG );
    }

    /*------------------------------------------------------------------*/
    /* The (analog or CDMA) MC subtask indicates it wishes to acvtivate */
    /* the (analog or CDMA) subtask by setting the command queue signal */
    /* (via a call to acptx_cmd() or txc_cmd()).                        */
    /*------------------------------------------------------------------*/
    if ((rex_signals_mask & TX_CDMA_CMD_Q_SIG) != 0)
    {

      /*------------------------------------*/
      /* Activate the CDMA Transmit subtask */
      /*------------------------------------*/
      M1X_MSG( MUX, LEGACY_LOW,
        "Entering txc_subtask" );

      txc_subtask();

      M1X_MSG( MUX, LEGACY_LOW,
        "Exiting txc_subtask" );

      /*-------------------------------*/
      /*      Kick watchdog            */
      /*-------------------------------*/
      txc_dog_report();
    } /* end if ((rex_signals_mask & TX_CDMA_CMD_Q_SIG) != 0) */

    if ( rex_signals_mask & TXC_MUXMSGR_Q_SIG )
    {
      /* process and clear the muxmsgr signal */
      muxmsgr_process_rcv_msg(TX_TASK);
    }

  } /* end for (;;) */

  rex_del_crit_sect(&txc_crit_sect);

} /* end tx_task */
