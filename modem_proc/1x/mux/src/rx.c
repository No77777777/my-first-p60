/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   M A I N    R E C E I V E    T A S K

GENERAL DESCRIPTION
  This module contains the Main Receive task initialization and
  startup processing.  It also contains the main processing loop
  for the Main Receive task.

EXTERNALIZED FUNCTIONS
  rx_task
    This is the entrance procedure for the Main Receive task.
    It contains the main processing loop for the task.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The rx_task function contains the Main Receive task.  It must be
  specified in a call to rex_def task and started by the Main Control
  task.

  Copyright (c) 1992 - 2018 Qualcomm Technologies, Inc.
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

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/rx.c_v   1.2   06 Sep 2002 09:25:06   varavamu  $
$Header: //components/rel/1x.mpss/8.0/mux/src/rx.c#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/17/18   eye     Added new crit section for audio related API's.
04/17/18   eye     Added null so check for RX and TX VOC frame processing.
01/28/14   pap     Aggregating the variables into global structures.
08/27/13   eye     Add muxmdsp_init() for module initialization.
08/28/13   cjb     Mainline FEATURE_MODEM_1X_DIME
03/05/13   srk     Replace existing watchdog implementation with the new
                   Dog Heartbeat Mechanism.
11/07/12   cjb     Mainlining FEATURE_MODEM_1X_SUPPORTS_RCINIT feature.
03/07/12   srk     Replaced MODEM_LEGACY_STARTED with RCINIT_RCEVT_INITGROUPS.
02/13/12   srk     Moved init fucntions after task start.
12/20/11   srk     Added RCInit Framework.
12/20/11   srk     Feature clean up: Mainlined FEATURE_MODEM_1X_SUPPORTS_Q6
09/02/11   srk     Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
07/26/11   cjb     Move rxc_crit_sect init/deinit to rxc_task().
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
11/01/10   jtm     Private file clean up.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
08/19/10   jtm     Loop through rxc_cmd_q as long as the q_cnt > 0.
                   Removed FEATURE_ACP support.
03/04/10   jj      Replaced deprecated task_xxx APIs with tmc_xxx APIs.
06/02/09   jj      Added include of newly splitted rx_v.h
04/03/09   jj      Removed Target specific feaaturization as part of
                   CMI effort.
12/02/08   adw     Commented out _v.h includes that no longer exist inside 1x.
05/04/07   pg      Update to new sleep voting mechanism - removal of
                   SLEEP_RXC_OKTS_SIG.
07/11/05   fh      Cleaned up header file including from MUX:
                     changed rxc.h to rxcmc.h
04/08/05   vlc     Set SLEEP_RXC_OKTS_SIG before entering RX task for loop.
09/11/03   yll     Added the featurization for ACP include files.
06/02/03   bkm     Lint cleanup.
09/06/02   va      Cleaned up lint errors.
06/07/02   jqi     Updated with multimode task operation interface.
03/06/00   ry      Added IS2000 featurization
01/17/00   lcc     Corrected message level to take care of warning during
                   compilation.
05/10/99   kmp     Changed T_ACP to FEATURE_ACP
12/12/95   jca     When CMD_Q sigs set we're now making sure there's an item
                   on the cmd_q before passing control to ACP/CDMA subtasks.
08/02/94   jca     Added T_ACP conditional compiles to support ISS1 release.
01/06/94   jah     Changed watchdog iface to use dog_report().
09/01/92   jai     Uncommented calls to rxc_subtask and rxc_powerup_init.
07/09/92   jca     Added processing for TASK_STOP_SIG and TASK_OFFLINE_SIG.
07/07/92   arh     Updated to new MC.H. Checked into PVCS for DMSS use.
04/16/92   jca     Created module.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "1x_variation.h"
#include "modem_1x_defs.h"
#include "rex.h"
#include "task.h"
#include "rx_v.h"
#include "rxcmc_i.h"
#include "rxccommon.h"
#include "rxccommon_i.h"
#include "rxcidle.h"
#include "msg.h"
#include "m1x_diag.h"
#include "mc_v.h"
#include "target.h"
#include "muxmsgr.h"
#include "rcinit.h"
#include "muxmdsp_i.h"
#include "muxvoc.h"

/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
/* Create TCB pointer */
rex_tcb_type* rx_tcb_ptr = NULL;


/* <EJECT> */
/*===========================================================================

FUNCTION RX_INIT

DESCRIPTION
  This function initializes the critical section and rxc powerup.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void rx_init (void)
{
  rex_init_crit_sect(&rxc_crit_sect);
  rex_init_crit_sect(&rxc_voc_crit_sect);

  /*------------------------*/
  /* Perform initialization */
  /*------------------------*/
  rxc_powerup_init();

} /* rx_init() */


/*===========================================================================

FUNCTION RX_TASK_INIT

DESCRIPTION
  This function initializes the tcb pointer and other pre initializations that
  needs to be done only after TCB_PTR is updated.

DEPENDENCIES
  Must be called from "rx" task context.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void rx_task_init(void)
{
  /* Update the RX TCB pointer */
  RX_TCB_PTR = rex_self();

  /* Initialize the MSGR for RX task */
  muxmsgr_init( RX_TASK );

  /* Initialize muxmdsp related params */
  muxmdsp_init();

  /* init critical section for muxvoc */
  muxvoc_crit_sect_init();

  /* Initialize dog parameters */
  rxc_watchdog_init();

  /* Initialize timers */
  rxcidle_def_timer();
  rex_def_timer( &rxc_failsafe_timer, RX_TCB_PTR, RXC_TIMER_SIG );

} /* rx_task_init */

/*===========================================================================

FUNCTION RX_TASK

DESCRIPTION
  This procedure is the entrance procedure for the Main Receive task.
  It contains the main processing loop for the Main Receive task which
  controls the activation of the ACPRX and RXC subtasks.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void rx_task (
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
  rx_task_init();

  /* Start handshake with RCINIT to start the init process */
  rcinit_handshake_startup();


  for (;;) { /* Never exit this loop... */

    rex_signals_mask =
      rex_wait( RXC_DOG_RPT_SIG | RX_CDMA_CMD_Q_SIG |
                TASK_OFFLINE_SIG | TASK_STOP_SIG );

    if ((rex_signals_mask & RXC_DOG_RPT_SIG) != 0) {
      /*-------------------------------*/
      /* Kick watchdog and reset timer */
      /*-------------------------------*/
      rxc_dog_report();
    }

    /*---------------------------------------------------------*/
    /* Check if powerdown command signal was set.  If set then */
    /* clear signal, process task stop procedure, and proceed. */
    /*---------------------------------------------------------*/
    if ((rex_signals_mask & TASK_STOP_SIG) != 0) {
      M1X_MSG( MUX, LEGACY_MED,
        "TASK_STOP_SIG received" );
      (void) rex_clr_sigs( RX_TCB_PTR, TASK_STOP_SIG );
    }

    /*-------------------------------------------------------*/
    /* Check if offline command signal was set.  If set then */
    /* clear signal, process task offline procedrue, and     */
    /*-------------------------------------------------------*/
    if ((rex_signals_mask & TASK_OFFLINE_SIG) != 0) {
      M1X_MSG( MUX, LEGACY_MED,
        "TASK_OFFLINE_SIG received" );
      (void) rex_clr_sigs( RX_TCB_PTR, TASK_OFFLINE_SIG );
    }

    /*------------------------------------------------------------------*/
    /* The (analog or CDMA) MC subtask indicates it wishes to acvtivate */
    /* the (analog or CDMA) subtask by setting the command queue signal */
    /* (via a call to acprx_cmd() or rxc_cmd()).                        */
    /*------------------------------------------------------------------*/
    if ((rex_signals_mask & RX_CDMA_CMD_Q_SIG) != 0)
    {
      /* ---------------------------------------------------------
      ** Clear the signal before processing the commands.
      ** --------------------------------------------------------- */
      (void) rex_clr_sigs( RX_TCB_PTR, RX_CDMA_CMD_Q_SIG );

      while (q_cnt(&rxc_common_data.rxc_cmd_q) != 0)
      {
        /*-----------------------------------*/
        /* Activate the CDMA Receive subtask */
        /*-----------------------------------*/
        M1X_MSG( MUX, LEGACY_LOW,
          "Entering rxc_subtask" );

        rxc_subtask();

        M1X_MSG( MUX, LEGACY_LOW,
          "Exiting rxc_subtask" );

        /*-------------------------------*/
        /* Kick watchdog and reset timer */
        /*-------------------------------*/
        rxc_dog_report();
      }
    } /* end if ((rex_signals_mask & RX_CDMA_CMD_Q_SIG) != 0) */
  } /* end for (;;) */

  rex_del_crit_sect(&rxc_crit_sect);
  rex_del_crit_sect(&rxc_voc_crit_sect);

} /* end rx_task */
