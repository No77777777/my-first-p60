#ifndef SRCHCPCDRX_H
#define SRCHCPCDRX_H

/*===========================================================================
                           S R C H C P C D R X . H

GENERAL DESCRIPTION

  This file contains variable declaration and function prototypes for CPC
  DRX feature.
  
EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

  Copyright (c) 2000- 2010 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/srchcpcdrx.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
01/10/20   ac      Mini dump framework checkin
12/11/19   ac      Searcher Mini Dump Change
08/08/16    bj      Fix to correct the calculation of  time elapsed time in SRCHR if DLDRXCTRL had
                    send align/prog time CFN in past with respect to current ref time CFN.
04/22/16    bj      CPC-DRX system delay handling for first wakeup.
04/06/16    rn      Ensure we check CPC DB only if the current SUB is in connected mode.
03/29/16    vn      Checking for scheduler mode while allowing CPC searches to happen
01/27/16    bj      Fix to handle the drx_ctrl local command system delay to correct 
                    the unexpected jump in cpc-drx start CFN.
11/26/15    vn      CPC Optimization for waking up only when searches are due.
07/14/15    gj      Remove srchcpcdrx_stop_drx_mode function
06/25/15    vn      Externing new function to handle CPC DRX local command processing..
06/15/15    jd      CM optimization changes.
04/21/15    amj     Clean up mode-related code and insert sleep cycle for full srch.
02/27/15    ap      Initial check-in for WL1 QSH framework 
01/20/15    jd      Global re-org for W + W
09/04/14    sr      Clean up of logging step 3 in DCH for CPC
07/24/14    amj     Changes to ensure NN params not changed if srch or abort in progress
06/06/14    amj     Add API for expiring the ASET PN search count for CPC.
07/29/14    amj/jd  Correct CPC macros for invoking scheduler 
07/24/14    amj     Correct Conditional logic for CPC scheduler
05/27/14    amj     Do not perform searches if QTA is active or WL1 is preparing for QTA
05/20/14    amj     Clean up some unused code and name macros appropriately.
01/02/14    da      Fixed search measurement related KW errors
11/14/13    amj     Added code to ensure a CPC sleep cycle in between a Step1
                    and subsequent NASTT searches
11/08/13    amj     Add support for keeping track of time elapsed during full searches
10/23/13    sm      Search modifications for RxDPM with cpc-drx
10/23/13    amj     Added CPC DRX Support with Search Scheduler
09/12/13    jd      Searcher redesign
07/23/13    jd      parallel step1 changes
10/01/12    zr      Fixed compiler warning
08/20/12    ag      Added debug function to catch when SRCH is stuck.
07/24/12    geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
05/08/12    amj     Added macro to return whether SRCH CPC is in active awake state.
02/22/12    amj     Restructured code to use TLM APIs.
                    Made changes to make sure Searcher doesn't get stuck.
09/03/11    sv      Added element in drx structure to check if step1 NN change needed 
07/07/11    hk      Up-merge from offline/main/latest
11/03/10    sa      Added code to support CM during CPC DRX searches and 
                    rename some global structure according to file name.
10/18/10    dm      Made changes for logging search type in 0x423A log packet.
09/28/10    dm      Made changes to log search abort in 0x423A log packet.
08/20/10    dm      Added protoype for function srchcpcdrx_update_srch_exec_time.
09/18/09    sa      Initial creation
                    
===========================================================================*/


#include "wcdma_variation.h"
#include "stmr.h"
#include "srchcmd.h"
#include "mcalwcdma_srch.h"
#include "tlm.h"
#include "srchmeas.h"

#include "wl1dldrxctrl.h"


/* Different search substate's during the DRX mode */
typedef enum
{
  /* DRX mode paramaters has been intialized*/
  DRX_START_INITIATED,
  /*DRX mode is active and wakes SRCH wakes up for first time */
  DRX_FIRST_WAKEUP,
  /* DRX mode is active and SRCH in sleep state */
  DRX_ACTIVE_SLEEP,
  /* DRX mode is active and SRCH in awake state */
  DRX_ACTIVE_AWAKE,
  /* DRX mode is not active*/
  DRX_INACTIVE
}srchcpcdrx_substate_enum_type;

#ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
#error code not present
#endif /* FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED */

typedef struct
{
   /* scheduling count for aset srches*/
   int8                           aset_cset_drx_schedule_cnt;
   /* scheduling count for nset searches*/
   int8                           nset_drx_schedule_cnt[MAX_CARR];

   /* number of the drx cyles given by the drx ctrl to scheduled step1*/
   uint8                          step1_delta_cnt[MAX_CARR];
   /*number of the drx cyles given by the drx ctrl to scheduled nastt */
   uint8                          nastt_delta_cnt[MAX_CARR];
   /* number of drx cyles count for all cell search count , this search is 
   only for logging all searched cells info*/
   uint8                          all_cell_srch_drx_cnt;

}srchcpcdrx_srch_cnts_struct_type;

typedef struct
{
  /* SRCH ID number given by the DRX MGR to SRCH*/
  uint8                          srch_id;
   uint8                          drx_cycle_len ;
   /* srch is in sleep state or active state */
   srchcpcdrx_substate_enum_type     substate;  
  /* hb mode is enable step1/2 searches during CM gap and till this flag is true 
   we will be in HB mode only */
  boolean                        hb_mode_for_step12;
  /* the number of DRX cycles programmed for next wakeup */
  uint8 num_drx_cycles_to_next_wkup;
  #ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  #error code not present
#endif

  /* Index to scheduling table */
  uint8                          drx_indx;

   /* drx mode schduling will be freezed by drx ctrl  during  reception of any cphy_setup_req ,so this flag indicates that
   drx have freeze the srch schduling mode */
   boolean                        drx_mode_freeze;


  /* Srch counts to schedule searches in the DRX mode */
  srchcpcdrx_srch_cnts_struct_type  srch_cnts;
  /* time period for whcih SRCH was active in the current DRX cycle */
  tlm_cfn_struct_type    time_elapsed;
  /* DRX aligning timing i,e timing given by the SRCH to DRX to program the wakeup occassion*/
  tlm_cfn_struct_type    drx_align_time;
  /* time to keep track of the elapsed time for one particular search */
  tlm_cfn_struct_type cur_srch_time;
  /*time line from the STRM dynamic count at which DRX will call the SRCH_CB function */
  tlm_cfn_struct_type    cur_time;
}srchcpcdrx_db_struct_mdump_type;

/* Structure to store the search paramters during the DRX mode */ 
typedef struct
{

  /* variable to check if step1 NN change is needed */
  boolean                        step1_nn_change;

  #ifndef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  /* boolean set to TRUE to introduce a sleep cycle between NASTT searches */
  boolean slp_cycle_for_fs;
  #endif

  /* variable of step1 NN parameter for start/stop CPC */
  uint8                           step1_nn_value;

#ifdef FEATURE_WCDMA_DC_HSDPA
   /* flag to indicate which all nset searches ( on anchor,secondry carrier) needs to issue 
     in this drx cycle */
   uint32                         nset_srch_mask;
  #endif

  #ifdef FEATURE_WCDMA_DC_HSDPA
   /* flag to indicate which all aset searches ( on anchor,secondry carrier) needs to issue 
      in this drx cycle */
   uint32                         aset_srch_mask;
#endif

}srchcpcdrx_db_struct_type;


typedef enum
{
  /* intialize the drx mode parameter for srch scheduling */
  SRCH_CPC_DRX_CONFIG,
  /*align the search wakeup time with hs-scch pattern */
  SRCH_CPC_DRX_ALIGN_INITIAL_WAKEUP_TIMING,
  
  /* freeze SRCH scheduling based on DRX counts while CPHY_SETUP req received from RRC
  and switch SRCH schedling to noraml HB*/
  SRCH_CPC_DRX_FREEZE_DRX_MODE_SCHEDULING,
  
  /* unfreeze SRCH scheduling i,e switch SRCH schedling to DRX counts while CPHY_SETUP_CNF
  send received from RRC*/
  SRCH_CPC_DRX_UNFREEZE_DRX_MODE_SCHEDULING,
  
  /*cleanup the drx mode parameter in srch  */
  SRCH_CPC_DRX_CLEANUP
}srchcpcdrx_cmd_enum_type;

typedef struct
{
 /* drx cycle length */
  uint8 drx_cycle_len;
}srchcpcdrx_drx_init_struct_type;

typedef struct
{
  /* cfn to program srch next wakeup occasion */ 
  uint8 start_cfn;
  /* tick in a cfn  to program th srch wakeup occasion */ 
  uint8 tick_offset;
}srchcpcdrx_align_wakeup_time_struct_type;

typedef union
{
  srchcpcdrx_drx_init_struct_type srch_init;
  srchcpcdrx_align_wakeup_time_struct_type align_wakeup_time;
}srchcpcdrx_cmd_param_union_type;

typedef struct
{
  srchcpcdrx_cmd_enum_type cmd_type;
  srchcpcdrx_cmd_param_union_type cmd_param;
}srchdrx_drxctrl_cmd_struct_type;


typedef enum
{
  FILT_TAB_INDX_FOR_20_MS_SRCH_INTVL,
  FILT_TAB_INDX_FOR_80_MS_SRCH_INTVL,
  FILT_TAB_INDX_FOR_40_MS_SRCH_INTVL,
  FILT_TAB_INDX_FOR_32_MS_SRCH_INTVL,
  FILT_TAB_INDX_FOR_64_MS_SRCH_INTVL
}srchcpcdrx_filt_tab_indx_enum_type;

typedef struct
{
  srchcpcdrx_filt_tab_indx_enum_type  aset_cset_filt_coef_tab_indx; 
  srchcpcdrx_filt_tab_indx_enum_type  sync_set_filt_coef_tab_indx; 

}srchcpcdrx_filt_tab_indx_struct_type;

#define SRCH_NUM_CX1_IN_SUBFRAME            7680
#define SRCH_NUM_SUBFRAME_IN_FRAME            5
#define MAX_NUM_DRX_CYCLES          6
#define FOUR_SUBFRAME_DRX_CYCLE     4
#define FIVE_SUBFRAME_DRX_CYCLE     5
#define EIGHT_SUBFRAME_DRX_CYCLE    8
#define TEN_SUBFRAME_DRX_CYCLE      10
#define SIXTEEN_SUBFRAME_DRX_CYCLE  16
#define TWENTY_SUBFRAME_DRX_CYCLE   20

#define MAX_STMR_DYNAMIC_CNT        4096

#define SRCHCPCDRX_ASET_SRCH_MIN_PERIOD 5
#define SRCHCPCDRX_NSET_SRCH_MIN_PERIOD 10

#define SRCHCPCDRX_TIME_ELAPSED_JUMP_IN_CFN 20

typedef struct
{
  uint8 aset_cset_srch_period;
  uint8 sync_set_srch_period; 
}srchcpcdrx_srch_schedule_struct_type;

typedef struct
{
  /* this  boolean indiactes if aset, nset interleaving is possible or not */
  boolean interleave_srch;
  /* anchor nset srch offset from aset search */
  uint8 nset_srch_offset;
  /* secondry carrier nset search offset from aset search */
  uint8 dc_nset_srch_offset;
#ifdef FEATURE_WCDMA_3C_HSDPA
  /* tertiary carrier nset search offset from aset search */
  uint8 w_3c_nset_srch_offset;
#endif
}srchcpcdrx_interleave_struct_type;

typedef struct
{
  #ifdef FEATURE_WCDMA_CPC_DRX
  int16 srchcpcdrx_async_srch_time;
  #endif

  #ifdef FEATURE_WCDMA_CPC_DRX
  srchcpcdrx_db_struct_mdump_type srchcpcdrx_db_mdump;
  #endif

}srchcpcdrx_ext_cntrl_params_mdump_type;


typedef struct
{
  #ifdef FEATURE_WCDMA_CPC_DRX
  srchcpcdrx_db_struct_type srchcpcdrx_db;

  /*Mini Dump: Variables that need to be dumped moved to this structure*/
  srchcpcdrx_ext_cntrl_params_mdump_type srchcpcdrx_ext_cntrl_params_mdump;
  #endif
  
}srchcpcdrx_ext_cntrl_params_type;


extern srchcpcdrx_ext_cntrl_params_type srchcpcdrx_ext_cntrl_params;


typedef struct
{
  #ifdef FEATURE_WCDMA_CPC_DRX
  wl1_dl_drxctrl_entity_ops_request_struct_type srch_wakeup_info;
  #endif
  #ifdef FEATURE_WCDMA_CPC_DRX
  uint32 srch_cpc_prog_next_wakeup_occ_count;
  #endif
  #ifdef FEATURE_WCDMA_CPC_DRX
  /** Flag used to print some debug messages for timing
 *  calculations - will be set through QXDM command */
  boolean srch_cpc_drx_timing_dbg;
  #endif
}srchcpcdrx_int_cntrl_params_type;

#define IS_SRCH_CPC_DRX_ACTIVE_AWAKE()  \
         (WL1_MSIM_VALIDATE_CONN_SUBID() && (srchcpcdrx_ext_cntrl_params.srchcpcdrx_ext_cntrl_params_mdump.srchcpcdrx_db_mdump.substate == DRX_ACTIVE_AWAKE))

#define IS_SRCH_CPC_DRX_ACTIVE_ASLEEP()  \
         (WL1_MSIM_VALIDATE_CONN_SUBID() && (srchcpcdrx_ext_cntrl_params.srchcpcdrx_ext_cntrl_params_mdump.srchcpcdrx_db_mdump.substate == DRX_ACTIVE_SLEEP))

#define IS_SRCH_CPC_DRX_ACTIVE()    \
         (WL1_MSIM_VALIDATE_CONN_SUBID() && (srchcpcdrx_ext_cntrl_params.srchcpcdrx_ext_cntrl_params_mdump.srchcpcdrx_db_mdump.substate != DRX_INACTIVE) && (srchcpcdrx_ext_cntrl_params.srchcpcdrx_ext_cntrl_params_mdump.srchcpcdrx_db_mdump.substate != DRX_START_INITIATED))

#ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
#error code not present
#endif /* FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED */

#define SRCH_DRX_MODE_NOT_FREEZE()   \
       (WL1_MSIM_VALIDATE_CONN_SUBID() && (srchcpcdrx_ext_cntrl_params.srchcpcdrx_ext_cntrl_params_mdump.srchcpcdrx_db_mdump.drx_mode_freeze == FALSE))

#define IS_AWAKE_TIME_CAL_NEEDED() \
       ((srchcpcdrx_ext_cntrl_params.srchcpcdrx_ext_cntrl_params_mdump.srchcpcdrx_db_mdump.substate == DRX_ACTIVE_AWAKE) && SRCH_DRX_MODE_NOT_FREEZE() && (srchcpcdrx_ext_cntrl_params.srchcpcdrx_ext_cntrl_params_mdump.srchcpcdrx_db_mdump.hb_mode_for_step12 == TRUE))

/*In certain cases DL can suspend periodic ops and whichc an cause scheduler to go into suspended while CPC is active*/
/*CPC active can issue search but since scheduler is in suspended, searches wont happen and further step1 and nastt can be stopped*/
/*Adding scheduler check further*/

#define IS_SRCH_READY_FOR_DRX_SCHEDULING()  \
        (IS_SRCH_CPC_DRX_ACTIVE() && \
             SRCH_DRX_MODE_NOT_FREEZE() && (srchcpcdrx_ext_cntrl_params.srchcpcdrx_ext_cntrl_params_mdump.srchcpcdrx_db_mdump.hb_mode_for_step12 == FALSE) && \
             (!SRCHSCHED_IN_SUSPENDED_MODE()) )
#ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
#error code not present
#else
#define  SRCHCPCDRX_USE_NSET_MASK_FROM_DRX_SCHEDULER() \
         (IS_SRCH_CPC_DRX_ACTIVE() && SRCH_DRX_MODE_NOT_FREEZE() )
         
#endif /* FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED */
         
#define  SRCHDCH_DRX_SET_STEP1_NN(nn)   \
	  do{			\
		 (wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.srch_step1_parms.srchStep1NonCohIntgLen = nn); \
		  mcalwcdma_srch_update_srch_async_parms(MCALWCDMA_SRCH_STEP1,(mcalwcdma_srch_async_parms_type *) (&wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.srch_step1_parms) ,wsub_id); \
		}while(0)


 
 
 
 
 extern const srchcpcdrx_filt_tab_indx_struct_type srchcpcdrx_filt_coef_indx_tab[MAX_NUM_DRX_CYCLES];

 extern const uint16 srch_filter_coef_tab[ SRCH_FILTER_COEF_TABLE_K_VALUE ][ SRCH_FILTER_COEF_TABLE_PN_SRCH_INTERVAL ];

/*===========================================================================

FUNCTION SRCHCPCDRX_INIT

DESCRIPTION
  This function will initialize the search scheduling parameter for the DRX mode searches.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void srchcpcdrx_init(void);

/*===========================================================================

FUNCTION SRCHCPCDRX_CONFIIGURE_DRX_MODE

DESCRIPTION
   This function will configure the DRX mode parameter and start DRX mode
   in SRCH side.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void srchcpcdrx_configure_drx_mode(uint8 dch_drx_cycle_len, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION SRCHCPCDRX_CLEANUP

DESCRIPTION
  This function will cleanup the srch scheduling parameter used for DRX mode searches and
  scheduled the searches back to on ragular HB.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void srchcpcdrx_cleanup( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDRX_WAKEUP_CB

DESCRIPTION
  This function will be called by DRX manager to wake-up every DRX cyle to issue searches scheduled in
  the current drx cycle.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchcpcdrx_wakeup_cb( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDRX_SCHEDULE_SERCHES

DESCRIPTION
  This function will schedule the searches in DRX mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchcpcdrx_schedule_searches( wsub_id_e_type wsub_id);

#ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
#error code not present
#endif /* FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED */
/*===========================================================================

FUNCTION SRCHCPCDRX_SLEEP_CB

DESCRIPTION
  This function will be called by DRX MGR whwn SRCH will say that 
  it is ready to SLEEP. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void srchcpcdrx_sleep_cb( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDRX_PROGRAM_NEXT_WAKEUP_OCCASION

DESCRIPTION
  This function will program the next wakeup occasion with DRX MGR.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void srchcpcdrx_program_next_wakeup_occasion( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDCH_TIME_ELAPSED_FOR_SRCH_ACTIVITY

DESCRIPTION
  This function will give the time elapsed between two time instances.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void srchcpcdrx_tot_time_elapsed_for_srch_activity( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDRX_START_DRX_MODE

DESCRIPTION
  This function will start the drx mode .

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

/*===========================================================================

FUNCTION SRCHCPCDCH_ADD_TIME_ELAPSED_FOR_SRCH_ACTIVITY

DESCRIPTION
  This function will return the number of drx cycles elapsed between two searches instances.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchcpcdrx_add_time_elapsed_for_srch_activity(wsub_id_e_type wsub_id);

extern void srchcpcdrx_start_drx_mode( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDRX_CONFIG_DRX_MODE

DESCRIPTION
  This function will be called from local command processing context and 
  based on whether it is wakeup time init or dldrx schedule update
  respective functions are called from here.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void srchcpcdrx_config_cmd_handler( srchcpcdrx_config_cmd_type *config_cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

  FUNCTION SRCHCPCDRX_CHECK_EARLY_LATE_WAKEUP_AND_UPDATE_TIME_ELAPSED

DESCRIPTION
  This function will whether the DRX CTRL wakes up SRCH early or late and update
  the time elapsed accordingly  .

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void srchcpcdrx_check_early_late_wakeup_and_update_time_elapsed(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDRX_PROCESS_DRXCTRL_COMMANDS

DESCRIPTION
  This function will process all the commands given by DRX ctrl to SRCH. This is
  the only function that is exposed to DRX MGR to interface with SRCH module.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/


void srchcpcdrx_process_drxctrl_commands(srchdrx_drxctrl_cmd_struct_type *cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION    SRCHCPCDRX_CONFIG_DRX_FILT_MAX_LEN

DESCRIPTION
    This function will intialize the maximum fillter length during the DRX 
    mode sheduling switching.

DEPENDENCIES
  None.

PARAMETERS

 
RETURN VALUE
  None

SIDE EFFECTS
  
===========================================================================*/

extern void srchcpcdrx_config_drx_filt_max_len( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDRX_INIT_WAKEUP_TIME

DESCRIPTION
  This function will be start drx mode and program the SRCH wakeup occassion.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void srchcpcdrx_init_wakeup_time(uint8 start_cfn,
                              uint8 tick_offset, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDRX_FREEZE_SRCH_SCHEDULING

DESCRIPTION
  This function will freeze the drx mode srch scheduling and will switch the 
  scheduling back to normal HB .

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void srchcpcdrx_freeze_srch_scheduling( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDRX_UNFREEZE_SRCH_SCHEDULING

DESCRIPTION
  This function will unfreeze the drx mode srch scheduling and will switch the 
  scheduling back to DRX mode .

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void srchcpcdrx_unfreeze_srch_scheduling( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  SRCHCPCDRX_DISCARD_COMMAND

DESCRIPTION
    This function will check if the srch command need to process or not

DEPENDENCIES
  None.

PARAMETERS

 
RETURN VALUE
  None

SIDE EFFECTS
  
===========================================================================*/
 
 boolean srchcpcdrx_discard_command(srch_ops_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  SRCHDRX_STEP1_ABORT_PROC

DESCRIPTION
  This function will be called whenever step1 will be aborted in the CM gap to 
  speed up next step1 command.

DEPENDENCIES
  None.

PARAMETERS

 
RETURN VALUE
  None

SIDE EFFECTS
  
===========================================================================*/
void srchcpcdrx_step1_abort_proc(void);

/*===========================================================================

FUNCTION  SRCHDRX_STEP2_ABORT_PROC

DESCRIPTION
  This function will be called whenever step2 will be aborted in the CM gap to 
  speed up next step2 command.

DEPENDENCIES
  None.

PARAMETERS

 
RETURN VALUE
  None

SIDE EFFECTS
  
===========================================================================*/
void srchcpcdrx_step2_abort_proc(void);

/*===========================================================================
FUNCTION srchcpcdrx_log_step1_srch_start

DESCRIPTION
  This function is the pre-command callback called when MDSP starts processing step1
  search command. This function is used for logging purpose to indicate the
  start of search.

INPUT

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
  
===========================================================================*/
extern void srchcpcdrx_log_step1_srch_start( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchcpcdrx_log_step2_srch_start

DESCRIPTION
  This function is the pre-command callback called when MDSP starts processing step2
  search command. This function is used for logging purpose to indicate the
  start of search.

INPUT

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchcpcdrx_log_step2_srch_start( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchcpcdrx_log_list_srch_start

DESCRIPTION
  This function is the pre-command callback called when MDSP starts processing list
  search command. This function is used for logging purpose to indicate the
  start of search.

INPUT

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchcpcdrx_log_list_srch_start( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchcpcdrx_log_srch_stop

DESCRIPTION
 This function is used for logging purpose to indicate the stop of search.

INPUT

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchcpcdrx_log_srch_stop( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  srchcpcdrx_check_if_searches_are_programmed

DESCRIPTION
  This function increments a count every time DL DCH maintenance event is called 
  (every 10 ms) if CPC is active and DRX is in ACTIVE_AWAKE. If that count exceeds
  100 then we ERROR FATAL.

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  
===========================================================================*/
extern void srchcpcdrx_check_if_searches_are_programmed( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  srchcpcdrx_increment_counts_upon_wakeup

DESCRIPTION
  This function increments the pn search CPC counts upon wakeup 
  callback being invoked by the DLDRXCTRL

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void srchcpcdrx_increment_counts_upon_wakeup(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDRX_CALCULATE_AND_SCHEDULE_NEXT_WAKEUP_OCCASION

DESCRIPTION
  This function will calculate the next wakeup occasion and will program it with the DRX Controller

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchcpcdrx_calculate_and_schedule_next_wakeup_occasion( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCPCDRX_EXPIRE_ASET_PN_COUNT

DESCRIPTION
  This function expires the ASET PN search count for CPC DRX.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
extern void srchcpcdrx_expire_aset_pn_count(void);

/*===========================================================================

FUNCTION  srchcpcdrx_set_step1_nn_parameters

DESCRIPTION
  This function increments a count every time DL DCH maintenance event is 
  called (every 10 ms) if CPC is active and DRX is in ACTIVE_AWAKE. If that 
  count exceeds 100 then we ERROR FATAL.

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void srchcpcdrx_set_step1_nn_parameters( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================

FUNCTION  SRCHCPCDRX_QSH_DUMP_CB

DESCRIPTION
  This function collects srch cpc info for QSH module during crash.

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void srchcpcdrx_qsh_dump_cb(wl1_srcher_mini_debug_info_struct_type *addr_ptr_t);
#endif

#ifndef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
/*===========================================================================

FUNCTION  srchcpcdrx_introduce_sleep_cycle_for_fs

DESCRIPTION
  This function sets the sleep cycle for full search to TRUE.

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void srchcpcdrx_introduce_sleep_cycle_for_fs(void);

/*===========================================================================

FUNCTION  srchcpcdrx_reset_sleep_cycle_for_fs

DESCRIPTION
  This function sets the sleep cycle for full search to FALSE.

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void srchcpcdrx_clear_sleep_cycle_for_fs(void);
#endif /* FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED */

/*===========================================================================

FUNCTION        srchcpcdrx_delta_time_from_ref_time

DESCRIPTION     This function calculates the difference between a reference time   
                and an event time 

DEPENDENCIES    None

RETURN VALUE     
  TRUE ->  if  DRX CTRL wakes up SRCH late and
           delta time is -ve i.e. evt time is in the past w.r.t  curr ref_time
  FALSE -> if  DRX CTRL wakes up SRCH early and
           delta time is +ve i.e. evt_time in future w.r.t  curr ref_time

SIDE EFFECTS    None

===========================================================================*/
boolean srchcpcdrx_delta_time_from_ref_time(tlm_cfn_struct_type *ref_time, tlm_cfn_struct_type *evt_time, tlm_cfn_struct_type* delta_time);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
  FUNCTION     srchcpcdrx_qsh_mdump_collect

  DESCRIPTION
  This function dumps all the required parameters from srchcpcdrx internal and 
  external control parameter structures

  DEPENDENCIES
  None

  RETURN VALUE
  None

  SIDE EFFECTS
  None
  ===========================================================================*/

extern QSH_MDUMP_FN_ATTR void srchcpcdrx_qsh_mdump_collect(wsub_id_e_type wsub_id);
#endif

#endif  /* SRCHCPCDRX_H */
