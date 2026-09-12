#ifndef SRCHFACHDRX_H
#define SRCHFACHDRX_H

/*===========================================================================
                           S R C H F A C H E D R X . H

GENERAL DESCRIPTION

  This file contains variable declaration and function prototypes for e-DRX feature in FACH state.
  
EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

  Copyright (c) 2000- 2011 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/srchfachedrx.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------
01/18/17   db      Do not issue any search through FMO scheduler if the FMO state machine is suspended 
12/04/15   db      Don’t set res eval meas variables when 1s CR timer expires if 
                   EDRX mode has not yet been set. EDRX mode init will take care of it.
08/17/15   vs      Moving srchfachdrx_set_res_eval_status to the .h file.
07/22/15   vs      Prototype of the new API srchfachdrx_post_srch_done_cmd
07/02/15   ap      Initial check-in for EFACH+MSIM
06/30/15   vs      Removing the API srchfachdrx_post_srch_done_cmd()
06/29/15   vs      Unconditionally deregister the Interfreq Event immediately after the 
                   unreservation and post the local cmd for fmo srch done.
                   This is to handle a race condition for (4,2) EDRX, where FMO mode will get 
                   changed to INTRAF mode when UE is waiting for 15th bpg, causing event to be active every frame.
02/27/15   ap      Initial check-in for WL1 QSH framework 
03/19/15   gsk     New Idle mode unified neighbor scheduler
03/05/15   psr     Move coefficient arrays to const type
01/19/15   psr     Global re-org for W+W
12/29/14   sl      Reschedule FMO if cfg window is missed
12/11/14   psr     srchcr.h inclusion cleanup
12/05/14   ar      Support for setting res eval status in INTRAF mode
07/16/14   sr      EDRX cleanup
05/14/14   sr      Changes to fix a logging issue where srch is not logged when srch is issued and 
                   completed before SCCH decode done is received.
04/09/14   ar      Remove srchfachdrx_prepare_drx since it is not called anymore
04/04/14   gsk     Support for AbsPrio det/meas timers in FACH edrx
03/28/14   ar      Move SRCHFACHDRX_TRIAGE above in the list
03/17/14   sr      Fix to handle multiple cfn update cb getting registered for EDRX srch entity.
01/23/14   ar      Support for LTE searches in EDRX FACH
12/15/13   ar      FEFACH (FR2759) absolute priority measurements in FACH
10/15/13   ar      Support for FEFACH (FR2759) absolute priority measurements in FACH
09/09/13   sr      Fixed logging issue in 0x432A to log srch entity related
                   parameters only if srch entity is not in SLEEP MODE12/12/12   sr      Fixed compiler errors
12/12/12   sr      Fixed compiler errors
09/10/12   stk     Added support for Cell reselection optimizations with HSFACH/HSRACH
07/25/12   sr      Wait for GFW to release the task queue before posting 
                   IRAT FMO done cmd in EDRX mode.
07/09/12   sr      For Inter-freq FMO, wait for reserved number of frames
                   before performing resel evaluations
06/21/12   sr      Added support to inform edrx inter freq when searches are done
06/14/12   hk      E-DRX support for HS-RACH
03/27/12   hk      Added support for FEATURE_WCDMA_HS_FACH_DRX
11/22/11   nd      Fixed compiler warning.
03/29/11   nd      Initial creation
                    
===========================================================================*/


/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "wcdma_variation.h"
#include "srchzz.h"
#include "srchfach.h"
#include "wsrchi.h"
#include "wl1dldrxctrl.h"
#include "wl1edrxhsentity.h"

#include "srchparm.h"


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* total number of Ton time suported  in FACH edrx feature*/
#define SRCHFACH_EDRX_MAX_NUM_TON                                 5

/* total number of eDrx cycles  suported  in FACH edrx feature*/
#define SRCHFACH_EDRX_MAX_NUM_EDR_CYCLES                          4

#define SRCHFACH_EDRX_INVALID                                     0xFFFF

#define SRCHFACHDRX_BMASK(srch) ( 1 << srch)


#define SRCHFACHDRX_ALL_SRCH_TIMERS 0xFFFF


/* Measurement status on what measurements are pending and what are not*/
#define SRCHFACHDRX_ASET_SRCH_PENDING 0x1
#define SRCHFACHDRX_INTRAF_LIST_SEARCH_PENDING 0x2
#define SRCHFACHDRX_INTRAF_STEP1_SEARCH_PENDING 0x4
#define SRCHFACHDRX_INTRAF_STEP_NASTT_SEARCH_PENDING 0x8
#define SRCHFACHDRX_INTRAF_SEARCH_PENDING  \
     ( SRCHFACHDRX_INTRAF_LIST_SEARCH_PENDING | SRCHFACHDRX_INTRAF_STEP1_SEARCH_PENDING | \
     SRCHFACHDRX_INTRAF_STEP_NASTT_SEARCH_PENDING)
             
#define SRCHFACHDRX_INTERF_LIST_SEARCH_PENDING 0x2
#define SRCHFACHDRX_INTERF_STEP1_SEARCH_PENDING 0x4
#define SRCHFACHDRX_INTERF_STEP2_SEARCH_PENDING 0x8
#define SRCHFACHDRX_INTERF_STEP3_SEARCH_PENDING 0x10
#define SRCHFACHDRX_INTERF_SEARCH_PENDING  \
   ( SRCHFACHDRX_INTERF_LIST_SEARCH_PENDING | SRCHFACHDRX_INTERF_STEP1_SEARCH_PENDING | \
   SRCHFACHDRX_INTERF_STEP2_SEARCH_PENDING | SRCHFACHDRX_INTERF_STEP3_SEARCH_PENDING)


#define SRCHFACHDRX_ALL_SEARCH_PENDING  (SRCHFACHDRX_INTRAF_SEARCH_PENDING | SRCHFACHDRX_INTERF_SEARCH_PENDING) 

#define SRCHFACHDRX_MAX_NUM_REACQ_SRCH 1

#define SRCHFACHDRX_GET_CURR_MODE() (srchfachedrx_ext_cntrl_params.srchfachdrx_db.srch_mode.current)

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

typedef enum
{
  SRCHFACHDRX_SRCH_TYPE_NONE,
  SRCHFACHDRX_SRCH_TYPE_INTRAF,
  SRCHFACHDRX_SRCH_TYPE_INTERF,
  SRCHFACHDRX_SRCH_TYPE_GSM,
  SRCHFACHDRX_SRCH_TYPE_LTE,
  SRCHFACHDRX_SRCH_TYPE_MAX
}srchfachdrx_basic_srch_enum_type;

typedef enum
{
 SRCHFACHDRX_ASET,
 SRCHFACHDRX_TRIAGE,
 SRCHFACHDRX_NSET,
 SRCHFACHDRX_INTRAF_FULL_SRCH,
 SRCHFACHDRX_INTERF,
 SRCHFACHDRX_GSM,
#if (defined(FEATURE_WCDMA_TO_LTE_FACH)&&defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH))
 SRCHFACHDRX_LTE,
#endif
 SRCHFACHDRX_MAX_SRCH
}srchfachdrx_srch_type_enum_type;

/* Pending search types to be performed */
typedef enum 
{
  SRCHFACHDRX_PENDING_SEARCH_TYPE_NONE     = 0x0,
  SRCHFACHDRX_PENDING_SEARCH_TYPE_INTRAF   = 0x1,
  SRCHFACHDRX_PENDING_SEARCH_TYPE_INTERF_0 = 0x2,
  SRCHFACHDRX_PENDING_SEARCH_TYPE_INTERF_1 = 0x4,
  SRCHFACHDRX_PENDING_SEARCH_TYPE_INTERF   = 0x6,
  SRCHFACHDRX_PENDING_SEARCH_TYPE_GSM     = 0x8,
  SRCHFACHDRX_PENDING_SEARCH_TYPE_ALL      = 0xF
} srchfachdrx_pending_search_enum_type;

/* renamed srchzz structures to use same structure in fach drx  mode */
typedef struct 
{
  uint16 intra_freq_pend_status;
  uint16 inter_freq_pend_status;
  uint16 inter_rat_pend_status;
  uint16 timer_expiry_status;
}srchfachdrx_meas_status_struct_type;

/* Enum to indicate meas rules type */
typedef enum {
  SRCHFACHDRX_MEAS_NONE    = 0x0,
  SRCHFACHDRX_MEAS_INTRA   = 0x1,
  SRCHFACHDRX_MEAS_INTER_0 = 0x2,
  SRCHFACHDRX_MEAS_INTER_1 = 0x4,
  SRCHFACHDRX_MEAS_INTER   = 0x6,
  SRCHFACHDRX_MEAS_GSM    = 0x8,
  SRCHFACHDRX_MEAS_ALL     = 0xF
}srchfachdrx_meas_rules_enum_type;


/* enum for  different srch mode during edrx  */
typedef enum
{
 /* searcher will be in this mode when drx cycle is short and 
  power saving achieved by tuning off RTR only */
  SRCH_CPC_DRX,
 /* searcher will be in this mode when drx cycle is long and 
  power saving achieved by tuning off TCXO shutdown  */
  SRCH_PCH_DRX,
  SRCH_INVALID
}srchfachdrx_srch_substate_enum_type;



typedef enum 
{
  SRCHFACHDRX_NON_EDRX_MODE,
  SRCHFACHDRX_INTRAF_SRCH_MODE,
  SRCHFACHDRX_FMO_SRCH_MODE,
  SRCHFACHDRX_SLEEP_MODE,
  SRCHFACHDRX_MAX_NUM_MODE,
  SRCHFACHDRX_PENDING_MODE,
}srchfachdrx_mode_enum_type;


/* enum for Ton time  */
typedef enum
{
  TEN_MS_RX_BURST,
  TWENTY_MS_RX_BURST,
  FOURTY_MS_RX_BURST,
  EIGHTY_MS_RX_BURST,
  ONE_SIXTY_MS_RX_BURST,
  INVALID_RX_BURST
}srchfachdrx_rx_burst_len_enum_type;

/* Enum for Edrx cycle length */
typedef enum
{
  FOURTY_MS_EDRX_CYCLE_LEN,
  EIGHTY_MS_EDRX_CYCLE_LEN,
  ONE_SIXTY_MS_EDRX_CYCLE_LEN,
  THREE_TWENTY_MS_EDRX_CYCLE_LEN,
  INVALID_EDRX_CYCLE_LEN
}srchfachdrx_edrx_cycle_len_enum_type;

/* struct for a time instance */
typedef struct
{
  int16  cfn;
  int32 ticks;
}srchfachdrx_timeline_struct_type;

typedef struct
{

  boolean                             edrx_active;
  struct
  {
  /* edrx cycle length */
  uint8                               edrx_cycle_len;
  /* rx burst len */
  uint8                               rx_burst_len;
  }edrx_params;

  struct
  {
    /* search scheduler mode */
    srchfachdrx_mode_enum_type  current;
    /* search scheduler mode */
    srchfachdrx_mode_enum_type  next;  
    /* if search mode change is pending */
    boolean                     change_pending;
  }srch_mode;
  
  /* Search id registered with DRX controller */
  uint8                               srch_id;

  /* time for intaf searches  since wakeup (this will have CFN , cx1 till  we can issue intraf searches */
  srchfachdrx_timeline_struct_type     next_wkup_time;

  /* time for intaf searches  since wakeup (this will have CFN , cx1 till  we can issue intraf searches */
  srchfachdrx_timeline_struct_type     prev_wkup_time;

  /* time left for inreaf searches */
  srchfachdrx_timeline_struct_type     time_left;
  
  srchfachdrx_meas_rules_enum_type  srch_meas_rules;

  /* For HSFACH/HSRACH: Counter to reset Tresel for all IF,GSM neighbors if FMOs
	 have been blocked by RACH */
  uint8 n_fmo_skip; 

  boolean recfg_in_prog;

  srchfach_substate_enum_type last_srch;

  tlm_tfn_struct_type last_srch_wrc;
}srchfachdrx_db_struct_types;

typedef struct
{
 srchfachdrx_basic_srch_enum_type srch_active;
 /* time when next interf/IRAT srch scheduling can be done  */
 srchfachdrx_timeline_struct_type     time_fmo_srch_complete;
}srchfachdrx_fmo_srch_timing_info_struct_type;


typedef enum
{
  /* intialize the drx mode parameter for srch scheduling */
  SRCHFACH_DRX_CMD_CONFIG,
  /*align the search wakeup time with hs-scch pattern */
  SRCHFACH_DRX_CMD_ALIGN_WAKEUP_TIMING,

  SRCHFACH_DRX_CMD_ALIGN_SLEEP_TIMING,
  
  /* freeze SRCH scheduling based on DRX counts while CPHY_SETUP req received from RRC
  and switch SRCH schedling to noraml HB*/
  SRCHFACH_DRX_CMD_FREEZE_DRX_MODE_SCHEDULING,
  
  /* unfreeze SRCH scheduling i,e switch SRCH schedling to DRX counts while CPHY_SETUP_CNF
  send received from RRC*/
  SRCHFACH_DRX_CMD_UNFREEZE_DRX_MODE_SCHEDULING,
  
  /*cleanup the drx mode parameter in srch  */
  SRCHFACH_DRX_CMD_CLEANUP,

  SRCHFACH_DRX_CMD_WAKEUP,

  SRCHFACH_DRX_CMD_SLEEP,
}srchfachdrx_cmd_enum_type;

typedef struct
{
 /* drx cycle length */
  uint8 edrx_cycle_len;
  uint8 rx_burst_len;
}srchfachdrx_init_struct_type;


typedef union
{
  srchfachdrx_init_struct_type srch_init;
  srchfachdrx_timeline_struct_type align_time;
}srchfachdrx_cmd_param_union_type;

typedef struct
{
  srchfachdrx_cmd_enum_type cmd_type;
  srchfachdrx_cmd_param_union_type cmd_param;
}srchfachdrx_drxctrl_cmd_struct_type;


typedef struct
{
  uint16 scr_code;
  uint16 freq;
  uint32 ref_pos;
  uint16 wkup_srch_attempt_cnt;  
  struct
  {
   uint32 max_pos[SRCH_MAX_PILOT_PEAKS];
   uint32 max_eng[SRCH_MAX_PILOT_PEAKS];
   
  }srch_peaks[SRCHFACHDRX_MAX_NUM_REACQ_SRCH];
  
}srchfachdrx_wakeup_info_struct_type;



/* structure for search modes handlers*/
typedef struct
{
  void (*init)(wsub_id_e_type wsub_id);
  boolean (*cleanup)( wsub_id_e_type wsub_id);
  void (*scheduler) ( wsub_id_e_type wsub_id);
  void (*srch_done) (srch_done_cmd_type *cmd_ptr, srchfachdrx_basic_srch_enum_type srch_type, wsub_id_e_type wsub_id);
  void (*cmd_processor)(srchfachdrx_drxctrl_cmd_struct_type *cmd_ptr, wsub_id_e_type wsub_id);
} srchfachdrx_srch_mode_handler_struct;

typedef enum
{
 SRCHFACHDRX_LIST_SRCH_POSTPROC_OPS_DUMP_SRCH_RESULT,
 SRCHFACHDRX_LIST_SRCH_POSTPROC_OPS_WINCENTRE,
 SRCHFACHDRX_LIST_SRCH_POSTPROC_OPS_FILTER,
 SRCHFACHDRX_LIST_SRCH_POSTPROC_OPS_CALC_ECIO_RSCP,
 SRCHFACHDRX_LIST_SRCH_POSTPROC_OPS_CALC_S_CRIT,
 SRCHFACHDRX_LIST_SRCH_POSTPROC_OPS_SORT,
 SRCHFACHDRX_LIST_SRCH_POSTPROC_OPS_RESEL_EVAL,
 SRCHFACHDRX_LIST_SRCH_POSTPROC_OPS_MAX
}srchfachdrx_list_srch_postproc_ops_enum_type;

typedef struct
{
  /* Cell Cfg critical section */
  rex_crit_sect_type srchfachdrx_scheduler_crit_sect;
  /* this structure have timer expiry information for all type of searches*/
  uint16 srchfachdrx_meas_max_tmr[SRCHFACHDRX_MAX_SRCH];
  /* counter for all  searches */
  uint16 srchfachdrx_frms_since_meas[SRCHFACHDRX_MAX_SRCH];
  /* searcher data base for edrx */
  srchfachdrx_db_struct_types srchfachdrx_db;
}srchfachedrx_ext_cntrl_params_type;

extern srchfachedrx_ext_cntrl_params_type srchfachedrx_ext_cntrl_params;

void srchfachedrx_ext_cntrl_params_init(void);

void srchfachedrx_int_cntrl_params_init(void);

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================

FUNCTION SRCHFACHDRX_INIT

DESCRIPTION
   This function will intialize search database based with default values
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_init( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHFACHDRX_QUERY_EDRX_ACTIVE

DESCRIPTION
   This function will be used by outside world to know if SRCH is in EDRX mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srchfachdrx_query_edrx_active( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHFACHDRX_QUERY_IN_VALID_EDRX_MODE

DESCRIPTION
   This function will be used by outside world to know if EDRX mode has been changed 
   to a valid mode from NON-EDRX mode.
   
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srchfachdrx_query_in_valid_edrx_mode(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHFACHDRX_PROCESS_DRXCTRL_COMMANDS

DESCRIPTION
  This function will process all the commands given by DRX ctrl to SRCH. This is
  the only function that is exposed to DRX CTRL to interface with SRCH module.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_process_drxctrl_cmds(srchfachdrx_drxctrl_cmd_struct_type *cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHFACHDRX_RESET_MEAS_TIMER

DESCRIPTION
   This function reset the timer expiry status of the searches passed in through timer_bmsk.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_reset_meas_timer (uint16 timer_bmsk, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHFACHDRX_UPDATE_NEXT_WKUP_TIME

DESCRIPTION
  This is the local cmd processor and configure the next RxBurst time onto the CFN update
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_register_next_wkup_time( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srchfachdrx_handle_abort_search

DESCRIPTION
   This function is called when a INTRA FREQ search gets aborted by mDSP. This function
   doesnt reissue the search. It just pends the search to be scheduled at a later time.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_handle_abort_search(srch_step_enum_type srch_step, srch_queue_enum_type que, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCHFACHDRX_PEND_IINTRAF_SRCH

DESCRIPTION
  This function pends an intraf search which could not be performed because some other searches
  were already ongoing.
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None.
===========================================================================*/
extern void srchfachdrx_pend_intraf_srch (uint16 pend_flag, uint16 unpend_flag, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srchfachdrx_proc_srch_done

DESCRIPTION
   This function will check if further reacq search is needed or not
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_proc_srch_done(srch_done_cmd_type *cmd_ptr, srchfachdrx_basic_srch_enum_type srch_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srchfachdrx_proc_srch_ops

DESCRIPTION
   This function will check if further reacq search is needed or not
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_proc_srch_ops(srch_ops_cmd_type  *cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCHFACHDRX_DO_PENDING_SRCH

DESCRIPTION
  This function check if any type (intra, inter, irat) of search is pending. If there are, it 
  tries to do that search.
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchfachdrx_do_pending_srch (srchfachdrx_pending_search_enum_type search_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCHZZ_PEND_IINTERF_SRCH

DESCRIPTION
  This function pends an INTERF search which could not be performed because we are close to the
  frame boundary and to avoid sending FMO cmd and SRCH cmd on other sides of frm bdry, pend it
  and will be taken care in the next SCCPCH EVT.
  

DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None.
===========================================================================*/
extern void srchfachdrx_pend_interf_srch (uint16 pend_flag, uint16 unpend_flag, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHFACHDRX_QUERY_EDRX_INTERF_ACTIVE

DESCRIPTION
   This function will be used by outside world to know if SRCH is in EDRX mode with IRAT srch active.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srchfachdrx_query_edrx_interf_srch_active( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHFACHDRX_QUERY_EDRX_ACTIVE

DESCRIPTION
   This function will be used by outside world to know if SRCH is in EDRX mode with IRAT srch active.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srchfachdrx_query_edrx_gsm_srch_active( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srchfachdrx_INCREMENT_SRCH_TIMERS

DESCRIPTION
   This function will increment intraf/interf/interrat search timers.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_increment_srch_timers( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srchfachdrx_trigger_srch_entity_wakeup

DESCRIPTION
   This function is used to trigger SRCH entity wakeup procedure unconditionally
  irrespective of Rx Burst

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_trigger_srch_entity_wakeup(wl1_dl_drxctrl_time_var_struct_type *wakeup_time, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION srchfachdrx_handle_fmo_srch_done

DESCRIPTION
   This fucntion will post the FMO SRCH done

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_handle_fmo_srch_done( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srchfachdrx_fmo_srch_done_cb

DESCRIPTION
   This fucntion just posts the local command for the SRCH done handling to be done
     in task context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_fmo_srch_done_cb(uint8 evt, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srchfachdrx_post_srch_done_cmd

DESCRIPTION
   This fucntion just posts the local command for the srch done.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_post_srch_done_cmd(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        SRCHFACHDRX_DELTA_TIME_FROM_REF_TIME

DESCRIPTION     This is a utility function and calculates the difference between a reference time   
                and an event time 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern int32 srchfachdrx_delta_time_from_ref_time(
  /** reference time */
  srchfachdrx_timeline_struct_type *ref_time,
  /** event time */
  srchfachdrx_timeline_struct_type *evt_time, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     SRCHFACHDRX_START_RESEL_EVAL

DESCRIPTION
  This function is called to trigger a reselection evaluation procedure if needed or check for OOS status

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchfachdrx_start_resel_eval( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srchfachdrx_get_curr_mode

DESCRIPTION
   This fucntion returns the current srch edrx mode

DEPENDENCIES
  None.

RETURN VALUE
  Enum type.

SIDE EFFECTS
  None.

===========================================================================*/
extern srchfachdrx_mode_enum_type srchfachdrx_get_curr_mode(void);

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH
/*===========================================================================

FUNCTION srchfachedrx_set_max_timer_for_prior_scheduling

DESCRIPTION
   This fucntion updates driver/meas layer with the search/meas timer 
periodicities for InterF and GSM AbsPrio layers

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchfachedrx_set_max_timer_for_prior_scheduling( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_TO_LTE_FACH

/*===========================================================================

FUNCTION SRCHFACHDRX_GET_LTE_DETECT_TIMER

DESCRIPTION
   This function returns the LTE Detection periodicity 

DEPENDENCIES
  None.

RETURN VALUE
  LTE Detection timer (uint16)

SIDE EFFECTS
  None.

===========================================================================*/
extern uint16 srchfachdrx_get_lte_detect_timer(void);

/*===========================================================================

FUNCTION SRCHFACHDRX_GET_LTE_MEAS_TIMER

DESCRIPTION
   This function returns the LTE Measurement periodicity 

DEPENDENCIES
  None.

RETURN VALUE
  LTE measurement timer (uint8)

SIDE EFFECTS
  None.

===========================================================================*/
extern uint8 srchfachdrx_get_lte_meas_timer(void);
#endif
#endif

/*===========================================================================

FUNCTION SRCHFACHDRX_PREPARE_FOR_CONFIG

DESCRIPTION
   This function is called when HS entity receives EDRX config command.
   In this function, we will deregister the cfn update function 

DEPENDENCIES
  None.

RETURN VALUE
  NONE

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachedrx_prepare_for_config( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHFACHDRX_GET_CURR_SRCH_TYPE_FOR_LOG

DESCRIPTION
   This function is used to get the current srch type for logging.
   In addition, this function does the extra check to findout if any
     intra-freq srch was done in the previous sub-frame.
   This is to account for ASET PN getting done within a subframe

DEPENDENCIES
  None.

RETURN VALUE
  SRCH type for logging

SIDE EFFECTS
  None.

===========================================================================*/
extern uint16 srchfachdrx_get_curr_srch_type_for_log( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHFACHDRX_SET_RES_EVAL_UPON_START_CELL_RESEL

DESCRIPTION
   This function is called upon the 1sec timer expiry when EDRX is active. It sets the res eval status
   so that if we are already in IntraF mode during 1sec timer expiry, we can do INTRAF full
   searches.

DEPENDENCIES
  None.

RETURN VALUE
  NONE

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchfachdrx_set_res_eval_upon_start_cell_resel( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHFACHDRX_RESCHEDULE_FMO_SRCH

DESCRIPTION
  This function posts a local command to invoke FMO scheduler when
  FMO search is not issued due to cfg window miss
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void  srchfachedrx_reschedule_fmo_srch(uint8 evt, wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION SRCHFACHDRX_TRIGGER_FMO_SCHEDULER

DESCRIPTION
  This function invokes the FMO scheduler based on current srch_mode
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void srchfachedrx_trigger_fmo_scheduler( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================

FUNCTION WL1_POPULATE_DEBUG_SRCHFACHEDRX_INFO_STRUCT_TYPE

DESCRIPTION
   This function dumps state info into a global strcuture as part of a crash

DEPENDENCIES
  None.

RETURN VALUE
  NONE

SIDE EFFECTS
  None.

===========================================================================*/
void wl1_populate_debug_srchfachedrx_info_struct_type(wl1_cell_resel_mini_debug_info_struct_type * srchfachedrx_cell_resel_dump_ptr);
#endif /* FEATURE_WCDMA_QSH_DUMP*/

/*===========================================================================

FUNCTION SRCHFACHDRX_GET_FMO_SRCH_TYPE

DESCRIPTION
   This fucntion returns the search type of the EDRX FMO

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern srchfachdrx_basic_srch_enum_type srchfachdrx_get_fmo_srch_type( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_EFACH_MSIM
/*===========================================================================

FUNCTION srchfachedrx_does_fmo_overlap_with_qta

DESCRIPTION
   This function checks if FMO falls between QTA overhead start and QTA end.If it does, this FMO is skipped  .

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean srchfachedrx_does_fmo_overlap_with_qta(uint8 frmaes_to_rsrv,wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_EFACH_MSIM*/

/*===========================================================================

FUNCTION srchfachdrx_set_res_eval_status

DESCRIPTION
   Reset cell resel eval status.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchfachdrx_set_res_eval_status( wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION WL1_QSH_MDUMP_COLLECT_SRCHFACHEDRX_INT_PARAMS

DESCRIPTION
  This is a minidump function for dumping to QSH the selected srchfachedrx int global variables for minidump

DEPENDENCIES
  NONE

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_srchfachedrx_int_params(wsub_id_e_type wsub_id);
#endif


#endif /* SRCHFACHDRX_H */

