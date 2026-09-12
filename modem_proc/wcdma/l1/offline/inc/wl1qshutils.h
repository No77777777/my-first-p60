#ifndef WL1QSHUTILS_H
#define WL1QSHUTILS_H
/*===========================================================================
   FILE: wl1qshutils.h


GENERAL DESCRIPTION
  This file contains declaration of WL1 related QSH handlers.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c)2000-2014 by Qualcomm Technologies, Inc. All Rights Reserved.

===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1qshutils.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
06/29/16    rn      Changes to support Freq common jump related QSH events
04/22/16    rn      Changes to support QSH searcher handlers

===========================================================================*/

/*=============================================================================
   INCLUDED FILES
  ===========================================================================*/
#include "l1def.h"
#include "wl1m.h"
#include "srchacq.h"


/*=============================================================================
   MODULE-SPECIFIC TYPE DEFINITIONS
  ===========================================================================*/
typedef struct
{
  uint16  qsh_srchacq_freq;
  uint16 qsh_srchacq_step;
  uint8 conn_state_freqcmn_jmp_cnt;
  uint8 idle_state_freqcmn_jmp_cnt;
  int16 prev_freq_common; 
} wsrch_qsh_db_params_type;

extern wsrch_qsh_db_params_type wsrch_qsh_db[WCDMA_NUM_SUBS];

#if defined (FEATURE_QSH_EVENT_NOTIFY_HANDLER) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH)

/*=============================================================================
   PUBLIC FUNCTION DEFINITIONS
  ===========================================================================*/

/*===========================================================================
  FUNCTION wsrchqsh_check_and_trigger_acq_failure

DESCRIPTION
    This function is called after every step during acqusition. This check if QSH handler is registered
to trigger a forced acq failure on this freq at this step

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern boolean wsrchqsh_check_and_trigger_acq_failure(srchacq_step_enum_type step, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DL_CARRCFG_GENERATE_DUMMY_INT_SCCH_ORDER


DESCRIPTION
  This function will be called by a test framework, fills the wfw interface pointers 
  to setup fake internal scch order.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_dl_carrcfg_generate_dummy_int_scch_order(wsub_id_e_type wsub_id);


/*===========================================================================
  FUNCTION wl1_qsh_check_freq_common_jump
DESCRIPTION
    This function check if there is jump in freq common for a fixed no of times
    depending on connected or idle state
DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void wl1_qsh_check_freq_common_jump(int16 freq_comon, wsub_id_e_type wsub_id);

/*===========================================================================
  FUNCTION wl1_qsh_reset_freq_common_params
DESCRIPTION
    This function reset freq common params in qsh db
DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void wl1_qsh_reset_freq_common_params(wsub_id_e_type wsub_id);

#endif /*FEATURE_QSH_EVENT_NOTIFY_HANDLER || FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/
#endif /*WL1QSHUTILS_H*/
