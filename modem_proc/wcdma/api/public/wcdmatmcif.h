#ifndef WCDMATMCIF_H
#define WCDMATMCIF_H

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/api/public/wcdmatmcif.h#3 $
$DateTime: 2020/01/24 03:56:36 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/08/15   as      W+W MAC UL/DL HS task variable doubling.
06/03/15   pr      W+W Phase8 Checkin.
03/23/15   rsr     W + W Phase 6 Checkin.
10/07/09   rmsd    Renamed is_sleep_drv_in_sleep(void) to wcdma_is_in_sleep(void).
                   Defined it here because it is needed outside modem (by dog.c,dogtest.c)
08/06/09   rm      Mainlined all features as part of CMI Phase 2-defeaturization effort.
03/02/09   rm      Initial Release
===========================================================================*/

#include "comdef.h"
/* --------------------------------- */
/* PUBLIC APIs of machstask.h */
/* --------------------------------- */
#include "queue.h"

/* -------------------------------------- */
/* PUBLIC API of l1task.h                 */
/* -------------------------------------- */
/*===========================================================================

FUNCTION    L1_TASK

FILE NAME   l1m.c

DESCRIPTION
  This procedure is the entrance procedure for the L1 task.
  It performs queue initialization and handshake with the Main Control
  task. It then loops forever waiting for commands from the command queues,
  watchdog timer signals, Main Control task signals, and local signals.
  Upon receipt of the signals, it dispatches the command/signal to be
  processed by the appropriate handlers.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void wcdma_l1_0_task
(
  /* Required for REX, ignore */
  uint32 dummy
);
#ifdef FEATURE_DUAL_WCDMA
extern void wcdma_l1_1_task
(
  /* Required for REX, ignore */
  uint32 dummy
);
#endif /*FEATURE_DUAL_WCDMA*/

/* -------------------------------------- */
/* PUBLIC API of l2dltask.h               */
/* -------------------------------------- */
/*===========================================================================

FUNCTION        wcdma_l2_dl_0_task

DESCRIPTION
  This task performs downlink layer 2 processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern  void  wcdma_l2_dl_0_task
(
  dword dummy

    /* Required for REX, ignore */
);

/*===========================================================================

FUNCTION        wcdma_l2_dl_1_task

DESCRIPTION
  This task performs downlink layer 2 processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern  void  wcdma_l2_dl_1_task
(
  dword dummy

    /* Required for REX, ignore */
);

/* -------------------------------------- */
/* PUBLIC API of l2ultask.h               */
/* -------------------------------------- */
/*===========================================================================

FUNCTION        wcdma_l2_ul_0_task

DESCRIPTION
  This task performs uplink layer 2 processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern  void    wcdma_l2_ul_0_task
(
  dword dummy
    /* Required for REX, ignore */
);

/*===========================================================================

FUNCTION        wcdma_l2_ul_1_task

DESCRIPTION
  This task performs uplink layer 2 processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern  void    wcdma_l2_ul_1_task
(
  dword dummy
    /* Required for REX, ignore */
);

/* -------------------------------------- */
/* PUBLIC API of rrctask.h               */
/* -------------------------------------- */
/*===========================================================================

FUNCTION RRC_0_TASK

DESCRIPTION
  This function is the entry point for RRC Task. It initializes RRC state
  and individual procedures by calling rrc_init(). The ACK signal is sent back
  to TMC and a wait is done on Task Start Signal from TMC. Once a Task Start
  Signal is received, rrc_dispatcher() is called that takes control now onwards.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern  void  rrc_0_task(dword param);

/*===========================================================================

FUNCTION RRC_1_TASK

DESCRIPTION
  This function is the entry point for RRC Task. It initializes RRC state
  and individual procedures by calling rrc_init(). The ACK signal is sent back
  to TMC and a wait is done on Task Start Signal from TMC. Once a Task Start
  Signal is received, rrc_dispatcher() is called that takes control now onwards.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern  void  rrc_1_task(dword param);

/* -------------------------------------- */
/* PUBLIC API of machstask.h               */
/* -------------------------------------- */
/*===========================================================================

FUNCTION        wcdma_mac_hs_dl_0_task

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern  void  wcdma_mac_hs_dl_0_task( dword dummy );

/*===========================================================================

FUNCTION        wcdma_mac_hs_dl_1_task

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern  void  wcdma_mac_hs_dl_1_task( dword dummy );

/*===========================================================================

FUNCTION WCDMA_IS_IN_SLEEP

DESCRIPTION
    This function returns TRUE or FALSE depending on the last interrupt
    received from the sleep controller. Sleep driver is supposed to be in
    sleep if the last interrupt is SLEEP_START_INT

DEPENDENCIES
    None

RETURN VALUE
    TRUE : if last interrupt is SLEEP_START_INT
    FALSE: all other interrupts other than SLEEP_START_INT

SIDE EFFECTS
    None

===========================================================================*/
extern boolean wcdma_is_in_sleep(void);

#endif /* WCDMATMCIF_H */

