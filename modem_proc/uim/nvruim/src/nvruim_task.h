#ifndef NVRUIM_TASK_H
#define NVRUIM_TASK_H
/*===========================================================================


              N V    R - U I M    S U B S Y S T E M    T A S K


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2013 - 2016 QUALCOMM Technologies, Incorporated and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/nvruim/src/nvruim_task.h#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
09/21/16   bcho    Handling of parallel requests in NVRUIM
04/13/16   ar      CDMA/HDR support on 2nd SUB
03/09/16   bcho    Process requests after NVRUIM task has started
10/02/15   kk      Redirect requests to NV during QCN Restore operation
06/29/15   lm      Enable memory leak detection
03/11/15   tl      Add caching of the module tcb pointer
02/03/15   ar      Checking for task stop before setting the signal for task
02/02/15   ar      Mechanism to provide lock to lower priority task like diag
08/28/14   av      Change nvruim crit sect to mutex-based from pimutex-based
07/14/14   tl      Remove always ON feature FEATURE_MODEM_RCINIT
01/10/14   df      Remove unneccessary include
11/01/13   at      Support for guaranteed response for each NVRUIM request
10/17/13   at      SIM busy support in NVRUIM
09/23/13   vr      qtf-crm MOB migration
05/13/13   vs      Initial revision
=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "task.h"
#include "uim_variation.h"
#include "rcinit.h"
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/* NVRUIM task TCB pointers. Value is initialized when task is being initialized. */
extern rex_tcb_type    * nvruim_tcb_ptr;
#define UIM_NVRUIM_TCB   nvruim_tcb_ptr

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

/*--------------------------------------------------------------------------
                             Signal Masks
--------------------------------------------------------------------------*/
#define NVRUIM_TASK_CMD_Q_SIG                 0x00000001
#define NVRUIM_RPT_TIMER_SIG                  0x00000002
#define NVRUIM_MMGSDI_ACCESS_SIG              0x00000004
#define NVRUIM_SIM_BUSY_PRIMARY_SIG           0x00000008
#define NVRUIM_SIM_BUSY_SECONDARY_SIG         0x00000010
#define NVRUIM_SIM_BUSY_TERTIARY_SIG          0x00000020

#define NVRUIM_CLASH_WITH_TASK_OFFLINE_SIG    0x00002000 /* RESERVED SIGNAL */
#define NVRUIM_CLASH_WITH_TASK_STOP_SIG       0x00004000 /* RESERVED SIGNAL */
#define NVRUIM_CLASH_WITH_TASK_START_SIG      0x00008000 /* RESERVED SIGNAL */

#define NVRUIM_TASK_STOP_SIG                  0x00010000

#define NVRUIM_CLASH_WITH_ONCRPC_REPLY_SIG    0x00800000 /* RESERVED SIGNAL */

/*******ADDITIONAL RESERVED SIGNALS**************/
#define NVRUIM_CLASH_WITH_CAMERA_SIG          0x04000000 /* RESERVED SIGNAL */
#define NVRUIM_CLASH_WITH_GRP_ACK_SIG         0x08000000 /* RESERVED SIGNAL */
#define NVRUIM_CLASH_WITH_SEC_OP_COMPLETE_SIG 0x10000000 /* RESERVED SIGNAL */
#define NVRUIM_CLASH_WITH_VS_OP_COMPLETE_SIG  0x20000000 /* RESERVED SIGNAL */
#define NVRUIM_CLASH_WITH_FS_OP_COMPLETE_SIG  0x40000000 /* RESERVED SIGNAL */
#define NVRUIM_CLASH_WITH_REX_SLEEP_SIG       0x80000000 /* RESERVED SIGNAL */
/************************************************/

/* Max requests that can be accomodated by NVRUIM queue */
#define NVRUIM_CMD_QUEUE_MAX_SIZE  100

/*=============================================================================

                       STRUCTURE DECLARATIONS

=============================================================================*/

/* ----------------------------------------------------------------------------

   ENUM:      NVRUIM_TASK_STATE_ENUM_TYPE

   DESCRIPTION:
     The NVRUIM task state enum type
-------------------------------------------------------------------------------*/
typedef enum {
  NVRUIM_TASK_INACTIVE,
  NVRUIM_TASK_ACTIVE,
  NVRUIM_TASK_ENDED
} nvruim_task_state_enum_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      NVRUIM_GENERIC_DATA_TYPE

   DESCRIPTION:
     The structure contains general global data for NVRUIM
-------------------------------------------------------------------------------*/
typedef struct {
  nvruim_task_state_enum_type   task_state;
  boolean                       rtre_config_init[NVRUIM_NUM_1X_SESSION_MAX];
  nv_rtre_control_type          rtre_control_value[NVRUIM_NUM_1X_SESSION_MAX];
  nv_rtre_polling_control_type  rtre_polling_control_value[NVRUIM_NUM_1X_SESSION_MAX];
  qurt_mutex_t                  crit_sect;
  rex_crit_sect_type            timer_crit_sect;
  nv_ruim_qcn_restore_status    nvruim_qcn_restore_status;
  /* Stores the current active CDMA sub informed by MMODE */
  nvruim_session_enum_type      nvruim_active_session;
  /* This global stores the number of commands queued by the context of current
     NVRUIM req on processing. If any SIM_BUSY_EVT or timer expired, all the
     commands for this context will be cancelled. */
  uint8                         current_commands_in_context[NVRUIM_NUM_1X_SESSION_MAX];
} nvruim_generic_data_type;

/* Global structure to general info for NVRUIM task */
extern nvruim_generic_data_type *nvruim_generic_data_ptr;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
   FUNCTION:      NVRUIM_CMD

   DESCRIPTION:
     This function allows putting of command into the NVRUIM command queue

   DEPENDENCIES:
     None

   LIMITATIONS:
     Parameter check for specific commands are not performed

   RETURN VALUE:
     nv_stat_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
nv_stat_enum_type nvruim_cmd
(
  nvruim_req_type *nvruim_req_ptr
);

/*===========================================================================
FUNCTION NVRUIM_WAIT_FOR_MMGSDI_ACCESS

DESCRIPTION
  Wait routine called when MMGSDI access is in progress

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void nvruim_wait_for_mmgsdi_access (
  void
);

/*===========================================================================
FUNCTION NVRUIM_WAIT

DESCRIPTION
  Main wait routine for the NVRUIM task.

DEPENDENCIES
  None

RETURN VALUE
  A rex_sigs_type value indicating all signals set (the current state
  of all signals). The calling unit can then check for the reason for
  exiting the wait.

SIDE EFFECTS
  None
===========================================================================*/
rex_sigs_type nvruim_wait
(
  rex_sigs_type requested_mask
);


#endif /* NVRUIM_TASK_H */
