#ifndef HDRSRCHTASK_H
#define HDRSRCHTASK_H

/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                       H D R    S R C H    T A S K

                          HDR Search Header File

GENERAL DESCRIPTION
  This file contains prototypes and declarations regional to the HDR SRCH
  task.

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000 - 2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$PVCSPath: L:/src/asw/COMMON/vcs/hdrsrchtask.h_v   1.11   11 Mar 2003 15:32:52   aneufeld  $
$Header: //components/rel/hdr.mpss/6.0/srch/inc/hdrsrchtask.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------
07/24/2015   wsh     Fixed TX chain event being treated as PRI chain 
04/27/2015   kss     Added HDRSRCH_DOG_HB_SIG for dog heartbeat support.
03/20/2012   cnx     Resolved TCB compiler errors for Dime. 
03/16/2012   arm     Resolve TCB errors for Dime. 
04/25/2011   ljl     Added HDRSRCH_TX_POWERDOWN_SIG.
09/13/2010   ljl     Redefined HDRSRCH_HIGH_PRI_RSP_SIG.
06/21/2010   cnx     Fixed compiler warnings.
04/08/2010   smd     Added  HDRSRCH_RFM_SIG in mainline.
08/05/2009   lyl     Added HDRSRCH_DEC_DEACTIVATED_SIG.
05/18/2009   ljl     Updated for firmware MSGR interface.
03/11/2009   rkc     Added signal HDRSRCH_RFM_SIG for RF SVDO API changes.
03/06/2009   pmk     Added support for MSGR
03/09/2007   grl     Added HDRSRCH_ADHOC_GP_SIG2 for general purpose
12/11/2006   ljl     Removed HDRSRC_AFC_SIG.
09/21/2006   ljl     Added signal for internal commands.
09/06/2005   ds      Added AFC signal
06/10/2005   rng     Removed HDRSRCH_NV_CMD_SIG as tcxomgr_init() in tmc now.  
05/31/2005   grl     Added signals to support TRM and TCXOMgr.
03/23/2005   hrk     Defined HDRSCH_GP_TMR2_SIG and HDRSRCH_TIMED_WAIT_SIG.
07/22/2004   ajn     Added common signals to state machine
10/29/2003   aaj     Added HDRSRCH_ADHOC_GP_SIG for general purpose 
03/11/2003   ajn     Updated Copyright notice
01/23/2002   ajn     Added Deactivate Signal.
04/25/2001   aaj     Removed reuse of srch_tcb for hdrsrch_tcb for TAZ3
11/22/2000   ajn     Code review changes / Added Tickle signal
10/31/2000   ajn     Added General Purpose Signals to State Machine
07/07/2000   ajn     Created this file

==========================================================================*/


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "comdef.h"
#include "rex.h"
#include "task.h"

/* EJECT */
/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/


/*------------------------------------------------------------------------
  HDR SRCH REX SIGNALS
------------------------------------------------------------------------*/
/* Address of the TCB for the task in which this protocol runs */
extern rex_tcb_type*    hdrsrch_tcb_ptr;
#define HDRSRCH_TCB hdrsrch_tcb_ptr

#define HDRSRCH_SIG_GET()     rex_get_sigs(HDRSRCH_TCB)
#define HDRSRCH_SIG_CLR(sig)  (void) rex_clr_sigs(HDRSRCH_TCB,(sig))
#define HDRSRCH_SIG_SET(sig)  (void) rex_set_sigs(HDRSRCH_TCB,(sig))


/* Event signals ... */
#define HDRSRCH_WDOG_SIG              0x00000001
#define HDRSRCH_CMD_Q_SIG             0x00000002
#define HDRSRCH_DUMP_SIG              0x00000004
#define HDRSRCH_ROLL_SIG              0x00000008
#define HDRSRCH_WAKE_SIG              0x00000010
#define HDRSRCH_TUNE_SIG              0x00000020
#define HDRSRCH_SCHEDULE_SIG          0x00000040
#define HDRSRCH_LOSTDUMP_SIG          0x00000080
#define HDRSRCH_TIMEOUT_SIG           0x00000100

/* General Purpose Timers ... */
#define HDRSRCH_GP_TMR0_SIG           0x00000200
#define HDRSRCH_GP_TMR1_SIG           0x00000400

/* General Purpose Signals ... */
#define HDRSRCH_GP_SIG0_SIG           0x00000800
#define HDRSRCH_GP_SIG1_SIG           0x00001000

/* Reuse GP_SIG1 for RxTx handling */
#define HDRSRCH_RXTX_SIG              HDRSRCH_GP_SIG1_SIG

/* RESERVED: TASK_OFFLINE_SIG         0x00002000 */
/* RESERVED: TASK_STOP_SIG            0x00004000 */
/* RESERVED: TASK_START_SIG           0x00008000 */

/* Tickle Signal */
#define HDRSRCH_TICKLE_SIG            0x00010000

/* Deactivate Signal */
#define HDRSRCH_DEACTIVATE_SIG        0x00020000

/* High priority response signal */
#define HDRSRCH_HIGH_PRI_RSP_SIG      0x00040000

/* More event signals ... */
#define HDRSRCH_TIME_VALID_SIG        0x00080000
#define HDRSRCH_RXCHIPX16_SIG         0x00100000
#define HDRSRCH_PRI_CHAIN_EVENT_SIG   0x00200000
#define HDRSRCH_DIV_CHAIN_EVENT_SIG   0x00400000

#define HDRSRCH_GP_TMR2_SIG           0x00800000
#define HDRSRCH_TIMED_WAIT_SIG        HDRSRCH_GP_TMR2_SIG

#define HDRSRCH_INT_CMD_SIG           0x01000000
  /* Internal command signal for the legacy state machine */

/* Stateless general purpose adhoc Signal. Used for srch4 enabling for now. */
#define HDRSRCH_ADHOC_GP_SIG2         0x02000000

#define HDRSRCH_LOW_PRI_RSP_IND_SIG   0x08000000
  /* Low priority response and indication signal */

#define HDRSRCH_DEC_DEACTIVATED_SIG   0x10000000
  /* Decoder deactivated signal */

#define HDRSRCH_TX_POWERDOWN_SIG      0x20000000
  /* Tx powerdown signal */

/* Signal to use for DOG HB mechanism.  Legacy dog signal is utilized for 
   the HDR search state machine's internal timeout checking.  */
#define  HDRSRCH_DOG_HB_SIG           0x40000000

/* Generic signal for rf complete callbacks */
#define  HDRSRCH_RFM_SIG              0x80000000

/* EJECT */
/*==========================================================================

                    FUNCTION PROTOTYPES USED IN THE MODULE

This section contains function prototypes for the functions used in this
module.

==========================================================================*/


/*==========================================================================

                    EXTERN VARIABLES and FUNCTIONS

This section contains variables that are accessible outside this module
==========================================================================*/


#endif /* HDRSRCHTASK_H */

