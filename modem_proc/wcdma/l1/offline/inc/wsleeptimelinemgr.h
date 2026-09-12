#ifndef WSLEEPTIMELINEMGR_H
#define WSLEEPTIMELINEMGR_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        WCDMA SLEEP TIMELINE MANAGER

GENERAL DESCRIPTION

  This file contains the declarations for the WCDMA L1 SLEEP timeline manager code.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

Copyright (c) 2000-2015 by Qualcomm Technologies, Inc. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wsleeptimelinemgr.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $ 
 
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/10/20   ac      Mini dump framework checkin
05/13/16   skk     Handling FW/RF resources when sleep duration is less.
01/28/16   kr      Timeline Adjustment for RxD coming up for 1st time after an Exit mode was done in last cycle
01/18/16   kr      Adjust timeline for Seq Reacq Cycle + Offline PICH-->NO LOCK cycle-->Online PICH + Seq Reacq cycle 
12/02/15   skk     Timeline adjustment when CTCH gets active as a part of BMC_SCHED_REQ. 
10/15/15   skk     Reset wsleep_div_mode_at_slp flag in drx_cmd.
10/01/15   pv      Macros moved.
09/16/15   sks     Compilation error fixes for FEATURE DUAL SIM undefined.
09/03/15   rs      Timeline fixes to avoid PO overruns 
05/19/15   sks     Changes to support PICH and measurements in L/W/G + /G.
03/20/15   rs      Created file
===========================================================================*/


#include "l1def.h"
#include "rxdiv_v.h"

#if defined (FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC)
#include "wl1_qsh_ext.h"
#endif /* defined (FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC) */


typedef struct
{
  uint32 sleep_setup_time_Cx8;
  int32 ticks_rem_adj_sleep_longer_Cx8;
  int32 ticks_rem_minus_target;
  /* timeline related globals */
  /* this variable is used for tracking if the rxd mode for the current cycle had changed between sleep and wakeup,
     and is used for disabling outer loop for the next cycle.*/
  rxd_pch_mode_enum_type wsleep_div_mode_at_slp;

}wsleeptimelinemgr_ext_cntrl_params_type;

wsleeptimelinemgr_ext_cntrl_params_type wsleeptimelinemgr_ext_cntrl_params[WCDMA_NUM_SUBS];

typedef enum
{
  WSLEEP_OUTER_LOOP_ADJ_OFFLINE_PICH_MODE,
  WSLEEP_OUTER_LOOP_ADJ_ONLINE_PICH_MODE,
  WSLEEP_OUTER_LOOP_ADJ_OFFLINE_RLP_PICH_MODE,
  WSLEEP_OUTER_LOOP_ADJ_FOR_QXT,
  WSLEEP_OUTER_LOOP_ADJ_FOR_CTCH,
  WSLEEP_OUTER_LOOP_ADJ_FOR_INTRA_RESEL,
  WSLEEP_OUTER_LOOP_ADJ_FOR_OFF_TO_ONLINE,
  WSLEEP_OUTER_LOOP_ADJ_FOR_PRLL_TO_SEQ_REACQ,
  WSLEEP_OUTER_LOOP_ADJ_FOR_OFFLINE_SEQ_TO_ONLINE_SEQ_REACQ,
  WSLEEP_APPLY_FINAL_OUTER_LOOP_ADJ,
  WSLEEP_OUTER_LOOP_ADJ_NUM_MODES
} wsleep_outer_loop_adj_mode_type;

typedef enum
{
  WSLEEP_ADJ_SLP_SETUP_TIME,
  WSLEEP_ADJ_SLP_INTERVAL_TIME,
  WSLEEP_MAX_ADJ_TYPE
}wsleep_slp_adj_type_enum_type;

/*PICH opts will be disabled in next PICH wakeup so adjust timeline by
  4 slots for offline PICH to online PICH transition and 2 slots for
  LPQ reacq search at wakeup as triage cnt > 1 FW doesnt support HPQ.
  */
#define WSLEEP_TICKS_ADJ_FOR_OFFLINE_TO_ONLINE                   -4
#define WSLEEP_TICKS_ADJ_FOR_OFFLINE_SEQ_TO_ONLINE_SEQ_REACQ     -6
#define WSLEEP_TICKS_ADJ_FOR_RLP_TO_OFFLINE_FOR_SLEEP_FAILURE    -1

#ifdef FEATURE_DUAL_SIM
#define WSLEEP_TICKS_ADJ_PRLL_TO_SEQ_REACQ_SWITCH       -4 
#else
#define WSLEEP_TICKS_ADJ_PRLL_TO_SEQ_REACQ_SWITCH       -3
#endif

/*PICH opts will be disabled in next PICH wakeup so adjust timeline by
  4 slots for offline PICH to online PICH transition and 2 slots for
  LPQ reacq search at wakeup as triage cnt > 1 FW doesnt support HPQ.*/
#define WSLEEP_ADJUST_TICKS_REM_SLP_LNGR_BY_SLOTS(num_slots)  \
  (wsleeptimelinemgr_ext_cntrl_params[wsub_id].ticks_rem_adj_sleep_longer_Cx8 -= (num_slots * CHIPX8_PER_SLOT))

#define WSLEEP_REINIT_TICKS_REM_MINUS_TARGET() \
   {  \
     wsleeptimelinemgr_ext_cntrl_params[wsub_id].ticks_rem_minus_target = 0; \
   }
   
/*Max tolerable value of ticks_rem_adj_sleep_longer_Cx8, if >15 reset it to -7 ms*/
#define WSLEEP_TIMELINE_MAX_ADJ_SLP_LNGR_MS    15
/* Initial adjustment of ext_drx_cntrl->ticks_rem_adj_sleep_longer_Cx8 */
#define TICKS_REM_ADJ_SLEEP_LONGER_CX8 (-7 * 30720) /* 7 ms */

/* Sleep setup time delta in Cx8 */
#define SLEEP_SETUP_TIME_CX8              (14* ONE_MS_IN_CX8)

#define RPM_DELAY_MS                              10

/* Keep the warm up time at 2.5 ms for DS mode/DSDA or if tune is pending
   [cases where we have done Exit mode on Device and RxD comes up in next cycle, there Combined wakeup wont happen]
   as there is no combined tuning yet.*/
#define REACQ_SRCH_TIME_DIV_CHAIN_MS  ((WL1_IN_DS_MODE() || WL1_IN_DUAL_ACTIVE_MODE() || wl1_rxd_tune_pending(wsub_id)) ? 5 : 2)

#define RF_WU_DIV_INC_DIVIDE_VAL 2

#ifdef FEATURE_WCDMA_REACQLESS_PICH 
/*===========================================================================
FUNCTION  wsleep_outer_loop_adj_rlp

DESCRIPTION
  This function adj outer loop variable while moving to RLP cycle 
  and while moving back to reacq cycle.
  Currently this Adjust ment is 6 BPG (19 -13)
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/ 
void wsleep_outer_loop_adj_rlp(boolean wakeup_cancel, wsub_id_e_type wsub_id);

#endif /* FEATURE_WCDMA_REACQLESS_PICH*/

/*===========================================================================
FUNCTION           WSLEEP_ADJUST_OUTER_LOOP

DESCRIPTION
  Adjusts WL1 timeline based on how far away the current wakeup is wrt to PI
  bits. This function helps adjust the timeline to sleep less or more
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/ 
extern void wsleep_adjust_outer_loop(wsleep_outer_loop_adj_mode_type outer_loop_adj_mode, int32 adjust_by, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     sleep_early_wakeup

DESCRIPTION This function checks whether UE need to wake up earlu depending on
                   PI bits position.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
May change sleeping time.

===========================================================================*/
extern void wsleep_adj_timeline_for_early_wkup( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION          WSLEEP_ADJ_ENTITY_SLEEP_INTERVAL    

DESCRIPTION
  This function adjusts the sleep interval specific to non-pich entity wakeup

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void wsleep_adj_entity_sleep_interval( uint32 msec_to_sleep, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WSLEEP_IS_OUTER_LOOP_ADJ_ENABLED

DESCRIPTION
  This function returns if the DRX outer loop adjustment is enabled or not.
  
PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  boolean flag - TRUE if outer loop adjustment enabled, FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean wsleep_is_outer_loop_adj_enabled( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     DRX_ENABLE_OUTER_LOOP_ADJ

DESCRIPTION
  This function is called to set/reset the flag that allows to perform 
  DRX outer loop adjustment.
  
PARAMETERS
  boolean flag.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void wsleep_enable_outer_loop_adj(boolean flag, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION             WSLEEP_APPLY_TIMELINE_ADJUSTMENT

DESCRIPTION
  This function is called to adjust the timeline for regular sleep wakeups
  It tries to adjust the timeline for RxD, QxT, RLP, early wakeup if needed
  and next entity waking up.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void wsleep_apply_timeline_adjustment(wsleep_slp_adj_type_enum_type adj_mode, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WSLEEP_REINIT_OUTER_LOOP_VARS

DESCRIPTION
  This function is called to reinitialize ticks_rem_adj_sleep_longer_cx8

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  drx_ext_cntrl_params[wsub_id].ticks_rem_adj_sleep_longer_Cx8 may not be reinitialized if less than
  default value
===========================================================================*/
extern void wsleep_reinit_outer_loop_vars( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_QSH_DUMP
extern void wl1_slptimeline_params_get_debug_info(wl1_drx_sleep_mini_debug_info_struct_type* obj);
#endif

extern void wsleeptimelinemgr_ext_cntrl_params_init(wsub_id_e_type wsub_id);
extern void wsleeptimelinemgr_int_cntrl_params_init(wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION wl1_qsh_mdump_collect_wsleeptimelinemgr_params

DESCRIPTION
  Wrapper

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_wsleeptimelinemgr_params(wsub_id_e_type wsub_id);
#endif
#endif /* END of WSLEEPTIMELINEMGR_H*/
