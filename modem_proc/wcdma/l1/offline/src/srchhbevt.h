#ifndef SRCH_EVT_H
#define SRCH_EVT_H
/*===========================================================================
                            S R C H _ E V T . H

GENERAL DESCRIPTION

  This file contains the prototypes for the search module sequencer event
  handlers.
  
EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

  Copyright (c) 2000, 2001 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/srchevt.h_v   1.1   27 Apr 2001 11:48:50   wnew  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/srchhbevt.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/11/19   ac      Searcher Mini Dump Change
01/03/18   rn      FR 47254: WCDMA- Global Time Services Support
07/19/16   bj      F3 reduction in WL1 searcher module phase-II and code cleanup
09/12/13   jd      Searcher redesign
01/20/15   jd      Global re-org for W + W
===========================================================================*/
#include "l1def.h"

/*--------------------------------------------------------------------------
           Search Operation Sanity Check Control Structure Type
--------------------------------------------------------------------------*/
typedef struct
{
  boolean s1_nastt_pn_q_enabled[SRCH_NUM_Q];
  uint16 lost_s1_nastt_pn_srch_cnt[SRCH_NUM_Q];
}srch_ops_sanity_chk_struct_mdump_type;

typedef struct
{
    /* HPQ List Search Sanity Timer */
  boolean otdoa_q_enabled;
  uint16 lost_otdoa_srch_cnt;
}srch_ops_sanity_chk_struct_type;


typedef struct
{
  uint16 rssi_update_cnt;
  uint16 aset_srch_cnt;
  uint16 nset_srch_cnt;
  uint16 fing_assign_cnt;
  uint16 rxagc_log_cnt ;
  uint16 step1_cnt;
  uint16 nastt_cnt;
  uint16 dc_step1_cnt;
  uint16 dc_nastt_cnt;
  uint16 w_3c_step1_cnt;
  uint16 w_3c_nastt_cnt;
  uint16 all_cells_srch_cnt;
  uint16 periodic_meas_rpt_cnt;
  uint16 srch_log_cnt;
  uint16 srch_dbg_cnt;
}
srch_ops_ctl_parm_struct_type;

typedef struct
{
  /* This flag will be set to TRUE in PCH and FACH states in order to allow HB to still schedule searches.
  * In all other states this flag will remain FALSE */
   boolean srch_allow_hb_to_schedule_searches;
   srch_ops_sanity_chk_struct_mdump_type srch_ops_sanity_chk_mdump;
}srchhbevt_ext_cntrl_params_mdump_type;

typedef struct
{
  #if  defined(FEATURE_LOC_GTS_SUPPORT)
  /* A counter to keep track of the total expired since the last GTS update
  in DHC/FACH state. This will be used in srch_evt_ctrl_hb_searches */
  uint8 gts_update_counter;
  #endif

  /* This is used in place of sequencer CFN for intra freq search requests during
   times when CM is activated  */
  uint8 srch_heartbeat_cfn;
  /*Resel eval counter/timer. This will be incremented by heartbeat.
FACH resel evals will occur every 90ms*/
  uint16 srch_fach_resel_eval_cnt;
  srch_ops_sanity_chk_struct_type srch_ops_sanity_chk;
  /*======================
          LOCAL GLOBAL VARS
  ======================*/
  srch_ops_ctl_parm_struct_type srch_ops_ctl_parms;
  srchhbevt_ext_cntrl_params_mdump_type srchhbevt_ext_cntrl_params_mdump;
}srchhbevt_ext_cntrl_params_type;

extern srchhbevt_ext_cntrl_params_type srchhbevt_ext_cntrl_params[WCDMA_NUM_SUBS];

typedef struct
{
  #ifdef FEATURE_WCDMA_RUMI_SUPPORT
  boolean rumi_bypass_searches_in_dch;
  #endif
  /* Following variables determine if an inter freq or internal 
   periodic measurement report is due */
  boolean l1_internalmeas_periodic_meas_report_due;
  boolean srch_periodic_meas_report_due;
  boolean srch_interf_periodic_meas_report_due;
  boolean l1_qualitymeas_periodic_meas_report_due;
}srchhbevt_int_cntrl_params_type;

/*===========================================================================
FUNCTION        SRCH_HEARTBEAT_EVENT_HANDLER

DESCRIPTION     This function handles the search heartbeat event.

DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
extern void srch_heartbeat_event_handler_sched( wsub_id_e_type wsub_id);

/*===========================================================================
  FUNCTION        SRCH_EVT_CTRL_HB_SEARCHES

  DESCRIPTION     This function is used to update the state of periodic 
  searches through heartbeat

  DEPENDENCIES    

  RETURN VALUE    None

  SIDE EFFECTS    
  ===========================================================================*/
extern void srch_evt_ctrl_hb_searches(boolean enable_hb, wsub_id_e_type wsub_id);

/*===========================================================================
  FUNCTION        SRCHHBEVT_GET_HB_STATUS

  DESCRIPTION     This function is used to get heartbeat status

  DEPENDENCIES    

  RETURN VALUE    None

  SIDE EFFECTS    
  ===========================================================================*/
extern inline boolean srchhbevt_get_hb_status(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCRUTILS_IS_RESEL_IN_PROGRESS

DESCRIPTION
  This function will be called from scheduler to check if Resel is in progress and issue hb searches

DEPENDENCIES
  None.

RETURN VALUE
TRUE - If Intra/Inter-freq resel is in progress
FALSE - Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srchcrutils_is_resel_in_progress( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchhbevt_ext_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize shared variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void srchhbevt_ext_cntrl_params_init( wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION srchhbevt_int_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize exclusive variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void srchhbevt_int_cntrl_params_init( void );

/*===========================================================================
FUNCTION        SRCHEVT_PERIODIC_MEAS_REPORTING_CHECK

DESCRIPTION     This function is used to update the state of periodic searches through heartbeat

DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
extern void srchevt_periodic_meas_reporting_check(wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
  FUNCTION     srchhbevt_qsh_mdump_collect

  DESCRIPTION
  This function dumps all the required parameters from srchhbevt internal and 
  external parameters

  DEPENDENCIES
  None

  RETURN VALUE
  None

  SIDE EFFECTS
  None
  ===========================================================================*/

extern QSH_MDUMP_FN_ATTR void srchhbevt_qsh_mdump_collect(wsub_id_e_type wsub_id);
#endif


#endif  /* SRCH_EVT_H */

