#ifndef SRCH_H
#define SRCH_H
/*===========================================================================
                S E A R C H      H E A D E R      F I L E

GENERAL DESCRIPTION:
  This file contains basic definitions used all other srch files or 
  external modules.

EXTERNALIZED FUNCTIONS

 srch_hpq_isr
  This function is the interrupt handler for Searcher HPQ interrupt when HPQ
  search is done.

 srch_lpq_isr
  This function is the interrupt handler for Searcher LPQ interrupt when LPQ
  search is done.

 srch_get_ref_cell_scr_code
  This function retruns active cell's primary scrambling code. 

 srch_get_cpich_rscp
  This function retruns serving cell's RSCP.

 srch_get_curr_freq
  This function returns current frequency on which UE is camped.

 srch_get_filtered_rscp
  This function returns an unsigned value representing the current,
  filtered RSCP value in P(AGC) + Ec/Io form for all ASET cells. 
  
  The returned can then be arbitrarily mapped to UI display space 
  using an equation, table, or case statement.  
  
  The RSCP is filtered using an (N-1)/N IIR filter for a smoother signal
  strength display. 

 srch_compute_rscp
  This function calculates the RSCP given the ecIo and the RxAGC..
  The calculated RSCP value will be returned in uint16 units. The actual RSCP
  is -ve of the returned value.

 srch_async_parm_init
  This function initializes searcher asynchronous parameters depending
	on the current state.

 srch_event_timer_elapsed
  This function returns the time elapsed from start_ts passed to it.

 nset_process_log_on_demand
  This funtion creates a NSET log packet and submits it to QXDM

 aset_process_log_on_demand
  This funtion creates a ASET log packet and submits it to QXDM

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

Copyright(c) 2000-2012 by Qualcomm Technologies, Inc.
All Rights Reserved.
      
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/srch.h_v   1.11   29 May 2002 11:28:34   msatyana  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wsrch.h#7 $
$DateTime: 2020/11/12 06:36:39 $
$Author: pwbldsvc $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/12/20   dsk     Reporting Filtered RSCP values to IMS in all L1 states
01/10/20   ac      Mini dump framework checkin
12/11/19   ac      Searcher Mini Dump Changes
03/06/18    jb      Ensuring that only intra-frequency measurements are retained during cell transition when RRC indicates it, but reset neighbour list on CPHY_CELL_TRANSITION_REQ
09/18/19   ac      QXDM Code Enhancement
05/16/17   rn      FR 50745 MAAT for WCDMA
02/19/18   nc      Fixing KW errors
01/10/18   nc      FEATURE_WCDMA_ENH_SRCH_LOGGING to create version 9 and version 10 of log packet
                   0x4177 and version 6 and version 7 of log packet 0x4179
08/18/17   nc      Added a boolean srch_cphy_pending_on_sfn_done to wsrch_ext cntrl params
11/29/16   rn      Doubling WCDMA_SIGNAL_INFO functions based on sub id
06/10/16   vn      Updating signal info params before going to sleep
06/10/16   vn      Adding new MACRO to return search state
04/22/16   rn      Doubling the DCH internal and external db
03/31/16   zt      Support for new log pkt 0x41CF
03/30/16   bj      NASTT search power optimization changes.
02/15/16   nga     Qxdm enhancements to crash for rxagc value every HB and comparing two consecutive PNs
01/08/16   vn      Get the search step from where util calculation is done previously
01/05/16   rn      New macro to get the current search state
11/26/15   rn      Cleaning F3s
11/18/15   vn      Allow parallel Step1 when inter-f step1 is ongoing.
10/16/15   ap      Initial check-in for FR 30856
11/04/15   vn      FR 30623 MCVS searcher optimizations initial check in
10/22/15   da      Implemented changes to support ACQ in DR
10/16/15   rn      Added Rumi framework enhancement changes for searcher module.
09/14/15   zt      Adding QXDM support to deny trm lock per irat
09/01/15   gj      Delay ACQ CNF fail to RRC until after search abort completes
08/25/15   vn      call MCVS API to update srch peiodicity always in DCH
07/22/15   vn      Externed function to get current tuned UARFCN
07/17/15   pr      Ensure usage of srch_acq_check_ue_in_x2w_hho for a per sub basis
07/21/15   amj     Enhance NAP feature to RxD and DC cases.
07/17/15   pvr     QDTA changes
07/16/15   vn      Externing variable which store maximum TX power inside PCH_SLEEP and BPLMN.
07/13/15   vn      SRCHR utilization changes for ASYNC NASTT Feature
07/13/15   jk      Doubling srch_in_asu variable per sub_id.
07/07/15   vn      Using Correct PN search delay constant in searcher utilization calculation.
06/29/15   jk      Apply threshold for E1A for X ms, in case a cell was removed from ASET 
                   due to early reporting of E1B using TPR enhancement conditions.
06/26/15   jk      Adding feature FEATURE_WCDMA_ATLAS_MODEM for code featurized by 
                   FEATURE_WCDMA_THOR_MODEM.
06/20/15   ac      Fix featurization for BO
06/19/15   bj      [FR 27497] Async NASTT � Power feature in Connected Mode.
06/15/15   jd      CM optimization changes.
06/09/15   jd      Fix for compiler warning.
06/05/15   amj     Changes for Step1 Peak Pruner algorithm.
06/01/15   jk      Prevent UE from sending 1D MRM, if there is only one cell in ASET.
06/01/15   jhl     Fix compiler issue after ASDiv feature enabled
05/30/15   jk      FR 25383: Traffic-to-Pilot Ratio (TPR) Based Measurement Optimization.
05/07/15   skk     FR24494: BCH Inter SIB sleep initial check in.
02/27/15   ap      Initial check-in for WL1 QSH framework 
04/22/15   gsk     Version-3 QxDM to crash on search done results
12/2/14    vn      Added macro checks whether cell selection req is received.
01/28/15   pr      Global reorg for W + W.
01/20/15   jd      Global re-org for W + W
01/08/15   da      Searcher changes to support FR23776 : LTE to WCDMA CSFB delay optimizations
12/22/14   jd      CM optimization changes.
12/04/14   svh     Added new global for storing idle mode rxagc for ASDIV.
11/21/14   gsk     SRCHZZ wakeup cleanup changes
11/12/14   jk      Fixed compiler warnings.
10/29/14   vn      Inorder to have NBR FET coexistance a global to check 
                   whether nbrej has performed or not is introduced
10/17/14   amj     DSDA search utilization adjustment.
10/10/14   amj     Code Cleanup.
10/07/14   jk      Fixing the compiler warnings by adding forward 
                   declaration for 'wsrch_nv_read()'.
09/10/14   amj     Nastt Power Optimization changes.
09/24/14   amj     Request/Release diversity chain for NBR.
09/19/14   da      Rumi specific fix related to frequency error in ACQ
09/17/14   ac      Changed WSRCH_NBREJ and QTA_ROTA NVs to use WNV API
05/20/14   bj      Added support for Pause and Resume searches in QTA gap and disable RxD searches in QTA gap
08/21/14   da      Fixed compiler warnings 
08/07/14   jk      Fixed search compiler warnings.
07/28/14   amj     Jolokia Updated searcher tables and RUMI database
07/18/14   sad     RF api changes and inter freq tune changes
07/15/14   bj      WCDMA Narrow Band Rejection for System Search.
07/15/14   ac      NV reorganization
06/17/14   rsr     Added Check to Ensure that NBR is not Supported in Div. Only 
                   Mode.
05/30/14   ar      Remove DUAL SIM featurization for srch_is_wkup_srch
05/28/14   amj     Mainline FEATURE_WCDMA_GET_RAT_MEAS_INFO.
05/28/14   amj     MCVS searcher changes for VPE clock
05/29/14   ag      R10 periodic measurement support on second carrier.
03/31/14   jd      Fix for conflict between scch deactivation and parallel step1.
05/21/14   jk      Moved the periodic measurements handling from srch_ops_cmd to a 
                   new local command.
05/13/14   ymu     Feature name FEATURE_WCDMA_DC_HSUPA_FRAME replacement with FEATURE_WCDMA_DC_HSUPA 
03/17/14   jd      Fix to use mutex in scheduler to avoid race condition during mode change.
04/14/14   gsk     Fix for compilation errors
02/12/14   sad     DB-3C API check in
04/01/14   km      RxD/ACQ changes
07/01/14   amj     Clean up old ASET Update code
01/06/14   sr      Suspending periodic ops before CPHY req processing
12/16/13   as      Porting G2W TA from Triton to Dime.
11/25/13   jk      FR 15721: HSPA+: Call performance improvement when selecting the best cell.
11/21/13   jd      FET 2.0 Searcher two queue approach.
10/30/13   jd      Changes to disable parallel step1 search when offline clk is 72mhz and rxd is active.
10/31/13   amj     Define mutex for RXD callback
10/17/13   bs      Preliminary 3C Support
09/12/13   jd      Searcher redesign
07/26/13   da      Replaced 8 logging functions with a single generic one 
07/24/13   jd      Changes for Dual band Dual carrier
07/23/13   vs      Added support for ASDiv feature.
07/15/13   gsk     FR 3672: CTCH enhancement changes
06/06/13   amj     Added API for changing the Step1 threshold when CPC DRX mode starts/stops
05/31/13   sad     Support for enahnced Layer-1 manager
05/21/13   sad     Addded function srchdc_active
04/11/13   gsk     Idle Mode FR 2741/2743, reducing WL1 wait time between rf_tune_to_chan
                   and search cmd to FW. Also adding support for interf offline PN
03/27/13   km      Removed logging for all cell search
01/21/13   amj     Make abort srch function return boolean for offline clk change
01/03/13   zr      Moving PN bypass parameters to heap memory
11/28/12   amj     MCVS feature related changes  
11/19/12   amj     Supporting imbalanced RxD step 1 search
10/24/12   amj     Cleaned up srch reset/restore state functionality.
09/24/12   zr      Adding API to determine the dual carrier frequency
08/15/12   zr      Adding debug search option
08/14/12   zr      Adding log packets to reduce F3 logging from searcher
08/03/12   sr      Delay the CM config till DPCH reslam
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
07/05/12   pv      Offline PICH changes - add 2 parameters to legacy sample server cmd.
07/05/12   gsk     Idle mode FRs 1782/1705/1947/2152/1950
05/23/12   kcm     L/G to W handover optimizations
04/18/12   zr      Added prototypes to revert and restore the DCH substate
04/03/12   zr      Added dummy cell creation function for testing 
03/27/12   hk      Added support for FEATURE_WCDMA_HS_FACH_DRX
03/14/12   zr      Added PN bypass peak info log packet
03/06/12   zr      Adding API to reschedule aborted PN search after offline clock speed change01/30/12   vs      Feature cleanup.
01/20/12   bs      free floating changes.
12/19/11   sv      Increased the array size for "valid" element to correct size 
                   in search bypass structure.
12/07/11   sv      Added changes to support Fatpath algorithm in connected mode.
11/06/11   nd      Added structure definitions to capture event 1A and evt2D
                   info to implement 2D delay algorithm.
08/11/11   mmr     Externed wsrch_tune_to_channel()
07/25/11   mmr     Changed the rotator value from int16 to int32
06/17/11   sa      Added all cell search count for logging all cells after
                   every 1 sec.
01/31/11   sa\nd   Moved processing of INTER-RAT periodic measurements from 
                   ISR to task context.
10/19/10   sv      Added changes to support full search for DC and QICE support.
07/15/10   vsr     (Genesis) Moved CME call to triage
05/12/10   sv      Fixed prototype for full window PN search.
03/30/10   rvs     Remove FEATURE_WCDMA_CME_CONTROLLER.
02/26/10   rvs     Fixes for Mustang.
08/11/09   sv      Fix compiler warning.
07/16/09   hk      Reorganized the code to remove INC/SRC violations
07/15/09   rvs     Edited feature flags to include
                   FEATURE_WCDMA_QICE_CONTROLLER.
05/26/09   sv      Added support for sending freeze\unfreeze sample server command to FW.
05/25/09   hk      Fixed compiler warnings
05/05/09   sv      Modified prototype for srch_mcal_handle_no_srch_cb.
04/29/09   rvs     Moved CME txd change cb functions to wl1srchdlif.h.
04/24/09   stk     Compiler warning fixes
04/20/09   rvs     Add CME txd change cb prototype and reg/dereg functions.
03/13/09   sv      Externed the "srch_async_parm_init_in_pch_sleep" function.
01/12/09   sv      Externed the "srch_mcal_handle_no_srch_cb" function.
12/09/08   sv      Mainlined FEATURE_WCDMA_OFFLINE_SEARCHER, FEATURE_RUMI_BRINGUP 
                   and FEATURE_WCDMA_DL_ENHANCED.
11/15/08   vsr     Moved CME srch callback call to task context
11/09/08   sv      Externed the "srch_mcal_whl1_status_cb" function.
10/06/08   sv      Externed some functions 
08/27/08   vsr     Added CME capability under feature FEATURE_WCDMA_CME_CONTROLLER
10/26/07   sk      Checking in MBMS branch code into mainline with MBMS feature disabled
09/26/07   nd      added extern for the new ISR for freq scan done from mDSP
05/23/07  ub/sk    Intergrate GSM meas and code cleanup
03/21/07   bbd     Removed srch timing adjust feature.
11/01/06   cpe     Mainlined the following features:
                    -FEATURE_OPTIMIZE_SEARCH_FOR_HHO
06/20/06   bd      Renamed global vars/functions to avoid conflict with 1x names
03/20/06   sv      Changes to support FEATURE_MDSP_MULTI_RF_CHAIN_INTERFACE.
07/01/05   bbd     Add proto for srch_set_timing_adj_value(). 
04/14/05   vb      Externed srch_hset_srch_cmd
04/14/04   sh      Added proto for srch_compute_rscp().
03/26/04   cpe     Added two more fields to srch_ops_code_struct_type
09/09/03   m       Modifications to support Inter Freq HHO
06/27/03   sh      Added function srch_event_timer_elapsed().
05/28/03   scm     Remove srch_prepare_for_suspend/resume().
05/23/03   cpe     Added the filed meas_id to the structure
                   srch_ops_code_struct_type
08/22/02   scm     Add srch_prepare_for_suspend() and srch_prepare_for_resume().
05/29/02   m       added prototype to support LOD for NSET and ASET updates
05/18/02   sh      Added function srch_async_parm_init() to initialize srch
                   async parameters based on the current state.
03/12/02   cpe     Added periodic_meas_rpt field to srch_ops_code_struct_type
09/12/01   wjn     Added the prototype of function "srch_get_filtered_rscp()"
                   for WCDMA RSSI display by UI.
06/26/01   cpe     Added srch_get_cpich_rscp() prototype
07/05/00   wjn     Created file.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "comdef.h"
#include "srchset.h"
#include "l1task.h"
#include "l1def.h"
#include "cmapi.h"
#include "rxdiv_v.h"
#include "wnv_l1.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
#define SRCH_RESEND_THROUGH_SCHED() ((WL1_GET_L1M_STATE() == L1M_ACQ) || WL1_L1M_IS_IN_CELL_SELECTION() || (WL1_GET_L1M_STATE() == L1M_DCH) || ((WL1_GET_L1M_STATE() == L1M_FACH) && !(srchfachdrx_query_edrx_active(wsub_id))))
#define SRCH_MAX_TYPES 0x100

#define SRCH_MAX_VPE_CLK_SPEED 5
#define SRCH_MAX_NUM_GSM_TRANS_SLOTS 5

/*Constants using to debug through QXDM commands*/

#define SRCH_NASTT_MUL_PEAKS_SEC_PEAK_MIN_ENG 150
#define SRCH_NASTT_MUL_PEAKS_PRI_PEAK_MIN_ENG 400


/*NASTT Search energy peak threshold when 2 peaks for the same PSC are 
  returned check if the higher energy peak has an energy greater than 400 
  and the lower energy peak has energy greater than 150*/
#define SRCH_ARE_PRI_SEC_NASTT_PEAKS(val1, val2) (((val1 > SRCH_NASTT_MUL_PEAKS_PRI_PEAK_MIN_ENG) && \
                                                   (val2 > SRCH_NASTT_MUL_PEAKS_SEC_PEAK_MIN_ENG)) || \
                                                  ((val2 > SRCH_NASTT_MUL_PEAKS_PRI_PEAK_MIN_ENG) && \
                                                   (val1 > SRCH_NASTT_MUL_PEAKS_SEC_PEAK_MIN_ENG)))


#ifdef FEATURE_WCDMA_ANT_SWITCH_SUPPORT
/* typedef for the searcher callback function type */
typedef void ANT_SWITCH_SEARCHER_CB_FUNC_TYPE(wsub_id_e_type wsub_id);
/* searcher callback register function  */
extern void ant_switch_register_with_search_done_cb(ANT_SWITCH_SEARCHER_CB_FUNC_TYPE *ant_switch_searcher_cb, wsub_id_e_type wsub_id);
/* searcher callbacl deregister function */
extern void ant_switch_deregister_with_search_done_cb( wsub_id_e_type wsub_id);
/* wsrch_ext_cntrl_params[wsub_id].srch_rxagc_prim uses SRCH_RXAGC_VAL_FOR_RSCP and wsrch_ext_cntrl_params[wsub_id].srch_rxagc_prim_idle uses SRCH_RXAGC_VAL_DBM. */

#endif

/*--------------------------------------------------------------------------
                            SEARCHER QUEUE TYPE
--------------------------------------------------------------------------*/
typedef enum
{
  SRCH_S1_NASTT_PN_Q,           /* step1-nastt-pn srch*/
  SRCH_OTDOA_Q,            /* OTDOA srch  */
}
srch_queue_enum_type;

/*--------------------------------------------------------------------------
                         SEARCHER OPERATION TYPE
--------------------------------------------------------------------------*/
/** 
 * Enum that holds all the various types of searches supported
 */
typedef enum
{
  NO_SRCH = 0x0,
  ASET_PN_SRCH = 0x01,
  NSET_PN_SRCH = 0x02,
  STEP1_SRCH = 0x04,
  NASTT_SRCH = 0x08,
  DC_STEP1_SRCH = 0x10,
  DC_NASTT_SRCH = 0x20,
  PARALLEL_STEP1_SRCH = 0x40,
  PARALLEL_NASTT_SRCH = 0x80,
  W_3C_STEP1_SRCH = 0x100,
  W_3C_NASTT_SRCH = 0x200,
  WKUP_PN_SRCH = 0x400,
  SCCH_ACT_SRCH = 0x800
} srch_ops_code_enum_type;

typedef struct
{
  boolean aset_srch;    /* Acive and Candidate Set Search */
  boolean nset_srch;    /* Neighbor set search  */
  boolean rssi_update;  /* RSSI update */
  boolean fing_assign;  /* Finger Assignment */
  boolean step1_srch;    /* step1 search  */
  boolean nastt_srch;    /* NASTT search */
  boolean dc_step1_srch; /* DC step1 search  */
  boolean dc_nastt_srch; /* DC NASTT search */
  boolean log_srch;     /* Log search events */
  boolean srch_dbg;     /* Multi-purpose search debug option */
  srch_ops_code_enum_type srch_type;         /* Search type enum for new Scheduler */
}
srch_ops_code_struct_type;

/*--------------------------------------------------------------------------
                            SEARCH MODULE STATES

--------------------------------------------------------------------------*/
typedef enum
{
  SRCH_IDLE_STATE,    /* Idle state */
  SRCH_FS_STATE,      /* WCDMA Carrier Frequency Scan */
  SRCH_ACQ_STATE,     /* WCDMA Cell acquisition */
  SRCH_BCH_STATE,     /* Reception of a BCH channel on Serving Cell */
  SRCH_FACH_STATE,    /* Reception of a FACH channel on Serving Cell */
  SRCH_DCH_STATE,     /* Reception of a DCH channel on Serving Cell */
  SRCH_DRX_STATE,     /* DRX in Idle Mode or Connected Mode */
#ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  SRCH_BPLMN_STATE,
#endif
  SRCH_MAX_STATE      /* Must remain last item in enum. */
} srch_state_enum_type;

typedef struct
{
  /* pos can range from 0 -(2560*8 -1) in cx8 units. 16 bit is enough to hold the data */
  uint16  pos;
    /* position, or offset to the Slot Reference Counter,
       of this slot peak in chipx8 resolution */
  uint16  eng;
    /* Energy of this slot peak */
  uint16 freq_hypo_idx;
    /* which freq hypothesis the step1 peak belong to. can take [0..2] 3 hypothesis */
  uint8 ant_id;
    /* if step1 is performed with rxd mode 1 i.e separate then this will indicate which antenna
    this peak belong to. values can be 0-primary and 1 -diversity */
  uint8 carr_idx;  
#ifdef FEATURE_WCDMA_STEP1_PEAK_PRUNER
  /* flag indicating whether this peak will be pruned or not */
  boolean to_be_pruned;
#endif
}
srch_slot_peak_struct_type;

/*--------------------------------------------------------------------------
                            PILOT PEAK STRUCTURE

   This data structure stores peaks of a pilot from NAS-TT search.

--------------------------------------------------------------------------*/
typedef struct
{
  uint16  scr_code;
    /* Scrambling Code of the pilot */
  uint32  pos;
    /* position of the maximum energy path in 1/8 chip units. */
  uint32  eng;
    /* Energy of the maximum energy path */
  uint8 carrier_idx;
	/*  Carrier index of the peak*/
}
srch_pilot_peak_struct_type;


/*--------------------------------------------------------------------------
                       SEARCHER LOST_SRCH STRUCT TYPE
--------------------------------------------------------------------------*/
typedef struct
{
  boolean  s1_nastt_pn_srch_lost;    /* lost step1/nastt/pn search */
  boolean  otdoa_srch_lost;    /* lost OTDOA search */
  srch_intraf_queue_enum_type srch_queue_idx;   /* Specifies the queue on which search is lost */
}
srch_lost_srch_struct_type;

typedef struct
{
  uint16 psc;
  uint16 freq;
  uint32 bmask;
  boolean status;
} srch_wakeup_set_struct_type;

typedef enum
{
  /* when no full srch is going on*/
  SRCH_NO_SRCH,
  /* every 360 ms, this full srch is attempted, Step1-NASTT (async nbrs)*/
  SRCH_STEP1_NASTT_ASYNC,
  /* every 360 ms, this full srch is attempted, Step1-NASTT (async nbrs)*/
  SRCH_STEP1_NASTT_UNMON
} srch_full_srch_type_enum_type;

/*--------------------------------------------------------------------------
                 SEARCHER QXDM DEBUG CRASH STRUCTURES
--------------------------------------------------------------------------*/
typedef enum
{
  CRASH_NO_SRCH       = 0x00,
  CRASH_PN_SRCH       = 0x01,
  CRASH_STEP1_SRCH    = 0x02,
  CRASH_NASTT_SRCH    = 0x04,
  CRASH_EVERY_HB      = 0x08,
  CRASH_FREQ_SCAN     = 0x10,
  CRASH_ACQ_BPLMN     = 0x20,
  CRASH_NBR_DONE      = 0x40,
} srch_debug_crash_info_srch_type;

typedef enum
{
  PN_ECIO                   = 0x01,
  PN_RXAGC                  = 0x02,
  PN_RX_IMBAL               = 0x04,
  PN_USE_ONLY_MAX_ENG       = 0x08,
  PN_GREATER_EQUALS_THRES   = 0x10,
  PN_USE_DIV                = 0x20,
  PN_IN_QTA                 = 0x80,
}srch_debug_crash_comp_quan_pn_type;

typedef enum
{
  S1_RX_IMBAL               = 0x01,
  S1_RXAGC                  = 0x02,
  S1_PEAK_CNT               = 0x04,
  S1_GREATER_EQUALS_THRES   = 0x10,
  S1_USE_DIV                = 0x20,
  S1_IN_QTA                 = 0x80,
}srch_debug_crash_comp_quan_step1_type;

typedef enum
{
  NASTT_ECIO                  = 0x01,
  NASTT_RXAGC                 = 0x02,
  NASTT_PEAK_CNT              = 0x04,
  NASTT_MUL_PEAKS             = 0x08,
  NASTT_GREATER_EQUALS_THRES  = 0x10,
  NASTT_IN_QTA                = 0x80,
}srch_debug_crash_comp_quan_NASTT_type;

typedef enum
{
  HB_RXAGC                  = 0x02,
  HB_RX_IMBAL               = 0x04,
  HB_GREATER_EQUALS_THRES   = 0x10,
  HB_USE_DIV                = 0x20,
  HB_IN_QTA                 = 0x80,
}srch_debug_crash_comp_quan_HB_type;

typedef enum
{
  FS_RAW_SCAN_STEP          = 0x01,
  FS_FINE_SCAN_STEP         = 0x02,
  FS_ADD_SCAN_STEP          = 0x04,
  FS_LIST_SCAN_STEP         = 0x08,
  FS_GREATER_EQUALS_THRES   = 0x10,
}srch_debug_crash_comp_quan_freq_type;

typedef enum
{
  ACQ_BPLMN_RXAGC                = 0x02,
  ACQ_BPLMN_GREATER_EQUALS_THRES = 0x10,
}srch_debug_crash_comp_quan_acq_bplmn_type;

typedef enum
{
  NBR_IS_W   = 0x01,
  NBR_IS_G   = 0x02,
}srch_debug_crash_comp_quan_nbr_type;

/*--------------------------------------------------------------------------
                         SEARCH STEP ENUMERATION

--------------------------------------------------------------------------*/

typedef enum
{
  SRCH_STEP_NONE,     /* 0 means no search is active */
  SRCH_STEP1,         /* Step_1 search */
  SRCH_STEP_NASTT,    /* NASTT search */
  SRCH_STEP_PN        /* PN search */
} srch_step_enum_type;

#ifdef FEATURE_WCDMA_DUAL_SYNC_INTERFACE
#define SRCH_MAP_STEP_TO_QUEUE(srch_step) ( (srch_step == SRCH_STEP1 ) ? SRCH_S1_Q : SRCH_NASTT_PN_Q )
#else
#define SRCH_MAP_STEP_TO_QUEUE(srch_step) ( (srch_step == SRCH_STEP_PN ) ? SRCH_NASTT_PN_Q : SRCH_S1_Q)
#endif

/* type of search - intra OR inter */
typedef enum
{
  INTRA_F,
  INTER_F,
} srch_cell_type_enum_type;

typedef struct
{
 uint8 meas_id;
 uint8 carr_idx;
 uint16 periodic_meas_rpt_cnt;
}srch_periodic_meas_struct_type;

typedef struct
{
  uint32 evt_idx;
} l1_interrat_periodic_meas_struct_type;

typedef struct
{
  boolean active;
  uint16 ts;

  boolean freeze;
  uint16 freeze_ts;
  uint16 new_ttt;
  uint16 ota_ttt;
}srchinterf_2d_info_struct_type;

typedef struct
{
 boolean active;
 uint16 ts;
}srchdch_evt1a_info_type;



typedef enum
{
  SRCH_FILTER_ECIO,
  SRCH_FILTER_RSCP,
  SRCH_FILTER_MAX
} srch_filter_type_enum;

typedef struct
{
  /* path energy for primary antenna search */
  uint32 max_eng[SRCH_MAX_NUM_PEAKS_BYPASS_SRCH];
  /* path position for primary antenna search */
  uint32 max_pos[SRCH_MAX_NUM_PEAKS_BYPASS_SRCH];
  /* path energy for secondry antenna search */
  uint32 max_eng_div[SRCH_MAX_NUM_PEAKS_BYPASS_SRCH];
  /* path position for secondry antenna search */
  uint32 max_pos_div[SRCH_MAX_NUM_PEAKS_BYPASS_SRCH];
  /* number of peaks returned by FW for bypass mode search */
  uint16 num_bypass_peaks;
  /* number of peaks returned by FW for normal  mode search */
  uint16 num_peaks;
  /* total number of peaks returned by FW for normal and bypass mode search */
  uint16 tot_num_peaks;
  boolean valid[2*SRCH_MAX_NUM_PEAKS_BYPASS_SRCH];  
}srch_bypass_pn_srch_results_struct_type;

typedef struct
{
  /* flag to enable bypass  mode search */
  boolean enable;
  boolean  srch_result_valid[MAX_NUM_ANTENNA];
  /* results per task */
  srch_bypass_pn_srch_results_struct_type task_rslt[SRCH_BYPASS_MODE_MAX_PN_TASKS];
  
}srch_pn_srch_bypass_mode_params_struct_type;



/* Database for rumi bringup flags and globals */
typedef struct
{
  /*Flag to enable rumi bringup*/
  boolean bringup_enabled;
  
  /*variable to do full window PN during ACQ*/
  boolean acq_use_full_win_pn_srch;

  /*To try Full window PN, scrambling code is given as input*/
  uint16 full_win_pn_scr_code;

  /* Enable rumi recovery win_size boolean */
  boolean recover_win_size;

  /* Enable rumi recovery win_size for hho fail 128*36*/
  boolean recover_win_size_hho_fail;

  /*Enable reacq pn search*/
  boolean enable_reacq_pn;

  /* Enable testing with frequency error */
  boolean test_with_freq_err;

}srch_rumi_db_struct_type;

#if defined (FEATURE_WCDMA_THOR_MODEM) || defined (FEATURE_WCDMA_BOLT_MODEM)  || defined (FEATURE_WCDMA_ATLAS_MODEM)

/* DSDA searcher specific parameters*/
typedef enum
{
  GSM_TX_INACTIVE = 0,
  GSM_TX_ONE_SLOT,
  GSM_TX_TWO_SLOTS,
  GSM_TX_THREE_SLOTS,
  GSM_TX_FOUR_SLOTS,
  GSM_TX_NUM_SLOTS
} wsrch_gtx_multi_slot_capability_type;

typedef struct
{
  /* the ratio adjustment for Step1 */
  uint16 step1_desense_ratio;
  /* the ratio adjustment for NASTT */
  uint16 nastt_desense_ratio;
  /* the ratio adjustment for PN */
  uint16 pn_desense_ratio;
} srch_mcvs_srch_util_desense_struct_type;

typedef struct
{
  /* this is for maintaining the number of GSM Tx slots in DS */
  wsrch_gtx_multi_slot_capability_type gtx_multi_slot_capability;

  /* this array contains the NASTT execution time x100 (to maintain accuracy) for each VPE clk speed */
  uint16 srch_vpe_clk_nastt_exec_dch_into_100[ SRCH_MAX_VPE_CLK_SPEED ];

  /* this array contains the NASTT execution time x100 (to maintain accuracy) for each VPE clk speed */
  uint16 srch_vpe_clk_pn_exec_dch_into_10000[ SRCH_MAX_VPE_CLK_SPEED ];

  /* this array is to maintain the scaling search execution factor for Step1, NASTT and PN in various desense scenarios */
  srch_mcvs_srch_util_desense_struct_type srch_vpe_desense_exec_time_scaling[SRCH_MAX_NUM_GSM_TRANS_SLOTS];

} srch_mcvs_srch_util_struct_type;

typedef struct
{
  /** VPE clk rate {285, 499, 665, 798} */
  uint8 clk;
  /** Full search period for carrier 0 {180, 160 ms} */
  uint16 T_fs0;
  /** Full search period for carrier 1 {180, 160 ms} */
  uint16 T_fs1;
  /** Full search period for carrier 2 {180, 160 ms} */
  uint16 T_fs2;
  /** Indicates whether full search is enabled on carrier 1 - depends on QICE {0, 1} */
  uint8 I_fs1;
  /** Indicates whether full search is enabled on carrier 2 - depends on QICE {0, 1} */
  uint8 I_fs2;
  /** Indicates whether compressed mode is active {0, 1} */
  uint8 I_cm;
  /** Number of active/candidate set searches per 1080 ms {54} */
  uint8 N_ac;
  /** Number of sync searches per 1080 ms {14} */
  uint8 N_s;
  /** Number of active and cadidate cells on carrier 0 {1 to 12^5} */
  uint32 X_ac0;
  /** Number of active and cadidate cells on carrier 1 {1 to 12^5} */
  uint32 X_ac1;
  /** Number of active and cadidate cells on carrier 2 {1 to 12^5} */
  uint32 X_ac2;
  /** Number of sync cells on carrier 0  {0 to 28} */
  uint32 X_s0;
  /** Number of sync cells on carrier 1  {0 to 28} */
  uint32 X_s1;
  /** Number of sync cells on carrier 2  {0 to 28} */
  uint32 X_s2;
  /** Indicates whether UE has a secondary downlink {0, 1} */
  uint8 Ic_1;
  /** Indicates whether UE has a third downlink {0, 1} */
  uint8 Ic_2;
  /** Indicates whether RxD is enabled {0, 1} */
  uint8 I_RxD;
}srch_mcvs_srch_util_params_struct_type;

#endif /* FEATURE_WCDMA_THOR_MODEM or FEATURE_WCDMA_BOLT_MODEM */

#ifdef FEATURE_WCDMA_TABASCO_MODEM
#define SRCH_MAX_NUM_CYCLE_TYPE  4
#define SRCH_MAX_NUM_RXD_VALUES  2
#define SRCH_MAX_NUM_NASTT_TYPES  2

#define SRCH_MCVS_Q16_VALUE_NOTATION  65536

/* this number is for 1.5/4 = 0.375 into Q16 */
#define SRCH_MCVS_Q16_0375 24576

/* this number is for 0.6666667 into Q16 */
#define SRCH_MCVS_Q16_0667 43691


/* this number is for 31.5 into Q16 used for Step1 */
#define SRCH_MCVS_Q16_STEP1_315 2064384

/* this number is for 1080 * 100 into Q16 used for final utilization formula */
#define SRCH_MCVS_Q16_UTIL_1080 6068


/*Indicates util calculation is done pre step1,NASTT or PN*/
typedef enum
{
  WSRCH_UTIL_PRE_STEP1,
  WSRCH_UTIL_PRE_NASTT,
  WSRCH_UTIL_PRE_PN,  
  WSRCH_UTIL_PRE_MAX_SRCH
}wsrch_util_calc_pre_srch_enum_type;


typedef struct
{
  /** Silver Cycles {LOW, MEDIUM, HIGH} */
  uint8 cycles;
  /** Full search period for carrier 0 {180, 160 ms} */
  uint16 T_fs0;
  /** Full search period for carrier 1 {180, 160 ms} */
  uint16 T_fs1;
  /** Indicates whether compressed mode is active {0, 1} */
  uint8 I_cm;
  /** Number of active/candidate set searches per 1080 ms {54} */
  uint8 N_ac;
  /** Number of sync searches per 1080 ms {14} */
  uint8 N_s;
  /** Number of active and cadidate cells on carrier 0 {1 to 12^5} */
  uint32 X_ac0;
  /** Number of active and cadidate cells on carrier 1 {1 to 12^5} */
  uint32 X_ac1;
  /** Number of sync cells on carrier 0  {0 to 28} */
  uint32 X_s0;
  /** Number of sync cells on carrier 1  {0 to 28} */
  uint32 X_s1;
  /** Indicates whether UE has a secondary downlink {0, 1} */
  uint8 I_1;
  /** Indicates whether RxD is enabled {0, 1} */
  uint8 I_RxD;
  /** Indicates whether previous cal done with async nastt {0, 1} */
  uint8 I_Async_Nastt;
  /*Indicates util calculation is done pre step1,NASTT or PN*/
  wsrch_util_calc_pre_srch_enum_type util_done_pre_srch;
}srch_mcvs_srch_util_params_struct_type;

typedef enum
{
	  SRCH_MCVS_UTIL_PARAM_CYCLES,
	  SRCH_MCVS_UTIL_PARAM_FULL_SEARCH_PERIOD,
	  SRCH_MCVS_UTIL_PARAM_CM_STATUS,
	  SRCH_MCVS_UTIL_PARAM_SEC_CARRIER_STATUS,
	  SRCH_MCVS_UTIL_PARAM_RxD_STATUS,
	  SRCH_MCVS_UTIL_PARAM_ACSET_C0,
	  SRCH_MCVS_UTIL_PARAM_ACSET_C1,
	  SRCH_MCVS_UTIL_PARAM_NSET_C0,
	  SRCH_MCVS_UTIL_PARAM_NSET_C1,
	  SRCH_MCVS_UTIL_PARAM_MAX_TYPE
} srch_mcvs_srch_util_params_enum_type;

typedef struct
{

  /* this array is to maintain the NASTT tasks/NASTT slot values for each type of search cycles */
  uint32 srch_nastt_tasks_per_nastt_slot[SRCH_MAX_NUM_NASTT_TYPES][SRCH_MAX_NUM_CYCLE_TYPE];

  /* this array is going to maintain the PN tasks/ slot values for each type of search cycles, as a function of RxD */
  uint8 srch_pn_tasks_per_slot[SRCH_MAX_NUM_RXD_VALUES][SRCH_MAX_NUM_CYCLE_TYPE];

  /*Number of PSCs for each cycle*/
  uint32 srch_nastt_tasks[SRCH_MAX_NUM_NASTT_TYPES][SRCH_MAX_NUM_CYCLE_TYPE];

} srch_mcvs_srch_util_struct_type;

#endif


#ifdef FEATURE_WCDMA_TABASCO_MODEM
/* this number is for 0.833333 into Q16 */
#define SRCH_MCVS_Q16_1500 54613
#else
/* this number is for 0.6666667 into Q16 */
#define SRCH_MCVS_Q16_1500 98304
#endif


#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK

#define WL1_SRCH_INTERF_STORE_MEAS_DB_INTLOCK() REX_ISR_LOCK(&wsrch_ext_cntrl_params[wsub_id].wl1_srch_interf_store_meas_db_mutex)
#define WL1_SRCH_INTERF_STORE_MEAS_DB_INTFREE() REX_ISR_UNLOCK(&wsrch_ext_cntrl_params[wsub_id].wl1_srch_interf_store_meas_db_mutex)

#define WL1_SRCH_MEAS_DB_INIT_INTLOCK() REX_ISR_LOCK(&wsrch_ext_cntrl_params[wsub_id].wl1_srch_meas_db_init_mutex)
#define WL1_SRCH_MEAS_DB_INIT_INTFREE() REX_ISR_UNLOCK(&wsrch_ext_cntrl_params[wsub_id].wl1_srch_meas_db_init_mutex)
#endif /*FEATURE_WCDMA_FREE_FLOATING_TASK*/

#ifdef FEATURE_WCDMA_DC_HSUPA
/* Boolean to give status of whether DC configuration in Uplink or not*/

#endif

#define SRCH_FILT_ENG_SCALING_FACTOR  512
typedef void (*SRCH_FULL_PN_SRCH_DONE_CB_FUNC_TYPE)(uint16,uint16,wsub_id_e_type wsub_id);





#define WL1_RXD_CB_INTLOCK() REX_ISR_LOCK(&wsrch_ext_cntrl_params[wsub_id].wl1_rxd_cb_crit_sect);
#define WL1_RXD_CB_INTFREE() REX_ISR_UNLOCK(&wsrch_ext_cntrl_params[wsub_id].wl1_rxd_cb_crit_sect);

/*NBR processing states */
typedef enum
{
  WSRCH_NBR_PROC_INACTIVE,
  WSRCH_NBR_PROC_IN_PROG,
  WSRCH_NBR_PROC_COMPLETED,  
}wsrch_nbrej_proc_state_enum_type;

 /*! NBR tunable parameters through NV item also the async parameters written to fw*/
typedef struct
{
 boolean enabled;
 uint32 rotatorFreq;  /* rotator frequency to be programmed to RXFE_NB_CSR_FREQ_OFFSET_NB0 */
 uint8 accumBpg;     /*! num of bpgs to accumulate for NBEE */
 int8 threshold1;    /* threshold1 - XXX */
 int8 threshold2;    /* threshold1 - XXX */

}wsrch_nbrej_config_params;

 /*NBR result states */
 typedef enum
 {
   WSRCH_SRCH_NBR_RSLT_W_SIGNAL,
   WSRCH_SRCH_NBR_RSLT_G_SIGNAL,
   WSRCH_SRCH_NBR_RSLT_INVALID
 }wsrch_srch_nbrej_result_enum_type;

/* NBR database */
typedef struct
{
  /*NBR current state */
  wsrch_nbrej_proc_state_enum_type state;
  /* info whether RxDiv module is using the div chain or not. if using then NBR cant use it*/
  boolean rxd_enabled;
  /* keep track of the time when the NBR was started. Keeps track if f/w
  responding in time */
  uint16 start_ts;

  wsrch_srch_nbrej_result_enum_type result;
  /* if for some reason we decide to stop ACQ after NBR has been triggered, it helps to
  send the ACQ_CNF at the end of NBR done. e.g RxAGC < thresh scenario*/
  boolean pending_acq_cnf;
  /* saves the RxLM buffer index allocated for NBR */
  uint32 rxlm_idx;
  /* NBR NV parameters and also written to f/w async interface */
  wsrch_nbrej_config_params config_params;
  /* A global to check whether nbrej has performed or not */
  /*This will be set when we do NBR and reset ONLY on ACQ success*/
  /*After ACQ success based on this LM settings are configured for FET mode*/
  boolean nbrej_performed;
}wsrch_nbrej_db_struct_type;


extern boolean wsrch_nbr_debug_msg_enable;

/* Minimum time for an NBR WRM Request in SCLK time set for 6ms*/
#define WSRCH_NBR_MIN_REQUEST_TIME_IN_SCLK   ((6 * TIMETICK_NOMINAL_FREQ_HZ)/1000)

/*whether NBR in progress or not*/
#define WSRCH_NB_REJ_PROC_ACTIVE()  (wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.wsrch_nbrej_db.state == WSRCH_NBR_PROC_IN_PROG)

/*whether NBR is in inactive or not*/
#define WSRCH_NB_REJ_PROC_INACTIVE()  (wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.wsrch_nbrej_db.state == WSRCH_NBR_PROC_INACTIVE)


/*Whether NBR can be enabled in these states or not */
#define WSRCH_IS_NB_REJ_BE_ALLOWED() ((wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.wsrch_nbrej_db.config_params.enabled) &&              \
   ((wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.search_state == SRCH_ACQ_STATE) ||(wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.search_state == SRCH_BPLMN_STATE)) &&                \
    (!wl1_rxd_in_div_only_mode(wsub_id))) 
    
/*Whether a cell selection request received or not */
#define WSRCH_IS_CELL_SELECTION_REQ_RECEIVED() (wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.srch_cphy_cell_selection_recvd)




/*============================================================================
                          CELL SELECTION 
============================================================================*/

/* Cell list structure for Cell Selection evaluation. */
typedef struct
{
  /* Cell Selection frequency requested by RRC */
  uint16   freq;

  /* Primary Scrambling Code */
  uint16   scr_code;

  /* Flag indicating if the cell is searched or not.
   * Currently, if the cell timing is unknown, the cell
   * will not be searched for cell selection evaluation
   */
  boolean  evaluated;

  /* Index (or pointer) to the corresponding cell table entry */
  srch_cell_struct_type *cell_ptr;

} srch_cell_select_list_type;

typedef struct
{
   uint16 srch_wcdma_filt_rscp;
   uint16 srch_wcdma_filt_rscp_filt_len;
    /* Filtered combined ASET RSCP, This is value to be used by UI to display
     WCDMA signal strength.
   */
   uint16 srch_rscp_in_sleep_state;

   #ifdef FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY
#error code not present
#endif

   #if defined(FEATURE_UMTS_REPORT_ECIO) || defined (FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY)
   /* Ec/Io value to be rported for UI bars display */
   uint16 srch_ecio_ui_display;
   #endif

   /* Pathloss value to be reported for UI bars display */
   uint16 srch_pathloss_ui_display;

   srch_cell_struct_type *srch_camp_cell_ptr;
   /* Pointer to the cell which UE has camped on. Set to the corresponding
     cell table entry when "CAMP_ON_CELL" aset update cmd is received.
    */

   /* Boolean global variable that indicates when RAT meas info is ready for use by upper layer */
   boolean wcdma_rat_meas_info_ready;

   /*Debug flag to turn ON\OFF F3's */
   boolean wsrch_debug_print_rat_meas_info;
    
} wsrch_ui_display_params_type;

/*--------------------------------------------------------------------------
          Structure for Serving Cell Search Operation Parameters
--------------------------------------------------------------------------*/

typedef struct
{
  boolean  map_info_included;
    /* Flag indicating if mapping functionality is needed.
       FALSE indicates no further mapping functionality is needed, or using
       implicit mapping: Q_map = Q_meas_LEV. See section 7 of TS25.304.
     */
  l1_mapping_info_struct_type  meas_map_info;
    /* Measurement mapping information. It is used when the above flag is
       TRUE
     */

  l1_meas_quan_enum_type   quan_type;
    /* Measurement quantity -- Ec/Io or RSCP. Note that, for Cell Selection
       and Reselection, we only support Ec/Io Measurement Quantity on FDD cells,
       as required by TS25.304.
     */

  boolean thresh_intra_f_incl;
  int16   thresh_intra_f;
    /* Intra-frequency measurement threshold. Value (in dB) ranges (-32..20)
       by step of 2. 0xFFFF indicates the parameter is not applicable.
     */

  boolean thresh_inter_f_incl;
  int16   thresh_inter_f;
    /* Inter-frequency measurement threshold. Value (in dB) ranges (-32..20)
       by step of 2. 0xFFFF indicates the parameter is not applicable.
     */

  boolean thresh_interrat_incl;
    /* If Sinterrat,m not included, do measurements always */

  uint16  num_rats;
    /* Number of additional RATs included */
  l1_interrat_resel_info_type   inter_s_info[L1_MAX_CELL_MEAS];
    /* Inter-RAT Reselection Information */

  uint16   hyst1;
    /* Hysteresis value of the serving cell for RSCP (?), It is used for
       calculating cell-ranking R in HCS. Value ranges (0..40) by step of 2.
     */
  uint16   hyst2;
    /* Hysteresis value of the serving cell for Ec/Io (?), It is used for
       calculating cell-ranking R in HCS. Value ranges (0..40) by step of 2.
       Default value is qhyst1_s.
     */

  uint16   t_resel_frames;
    /* Cell reselection timer, in 10ms unit */

  int16   max_tx_pwr;
    /* Maximum allowed UL TX power, UE_TXPER_MAX_RACH (dBm) */
  int16   q_qualmin;
    /* Minimum required quality level for Ec/Io measurement (in dB).
       Value ranges (-20..0)
     */
  int16   q_rxlevmin;
    /* Minimum required received level for RSCP measurement (in dBm).
       Value ranges (-115..-25) by step of 2.
     */
  boolean hcs_used;
    /*Flag indicating if HCS is enabled on the current cell or not */

  boolean hcs_info_incl;
    /* Flag indicating if Serving cell HCS information included */
  l1_serv_cell_hcs_info_struct_type hcs_info;
    /* Serving cell HCS information */

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
  boolean prior_used;
    /*Flag indicating if priority information is provided by network or not */

  //TO DO : Not needed, clean it up
  //  boolean prior_info_incl;
    /* Flag indicating if Serving cell Priority information included */

  l1_serving_cell_priority_info_type prior_info;
    /* Serving cell Priority information */
#endif

  boolean thresh_hcs_incl;
  int16   thresh_hcs;
    /* HCS measurement threshold. Value (in dB) ranges (-105..91)
       by step of 2. 0xFFFF indicates the parameter is not applicable.
     */
    
    /* Part of REL5 CR-136 R1*/

    /* This specifies the maximum number of cell reselections in 
       case of non-HCS usage.
    */
  uint16 nhcs_n_cr;
    /* This specifies the duration for evaluating allowed amount of 
       cell reselection(s) in case of non-HCS usage.
    */
  l1_t_cr_max_enum_type nhcs_t_crmax;
    /* This specifies the additional time period before the UE can 
       revert to low-mobility measurements in case of non-HCS usage.
    */
  l1_t_cr_max_hyst_enum_type nhcs_t_crmaxhyst;

    /* Speed dependent ScalingFactor for Treselection */
  uint16 tresel_scale;

    /* Inter-frequency ScalingFactor for Treselection */ 
  uint16 tresel_interf_scale;

    /* GSM ScalingFactor for Treselection */
  uint16 tresel_gsm_scale;

    /* LTE Scaling factor for Treselection*/
  uint16 tresel_lte_scale;

  /* New algorithm to support intra-f detected cell reselections */
  /* Variables to store the max values from INTRA NCL cell list */
  int16  intra_ngbr_max_tx_pwr;
  int16  intra_ngbr_max_q_qualmin;
  int16  intra_ngbr_max_q_rxlevmin;
  int16  intra_ngbr_max_q_offset1;
  int16  intra_ngbr_max_q_offset2;
  
  #ifdef FEATURE_FEMTO_CSG
  /* Store the type of currently camped cell (TRUE: FEMTO, FALSE: MACRO) */
  boolean is_camped_cell_femto;
  boolean is_manual_csg_mode;
  #endif
}
srch_aset_parms_struct_type;


typedef struct
{
  /* ecio,rscp  stored in x512 format*/
  uint16 ecio_hi_res[MAX_NUM_ANTENNA];
  uint32 rscp_hi_res[MAX_NUM_ANTENNA];
  /* for IIR filtering the data with max filt len 5 */
  uint16 filt_len;
  /*bits 8..4 -> filt config ; bits 3..0 -> eng config */
  uint8 curr_config;
  boolean div_enabled;
  boolean valid;
}wsrch_sig_strength_struct_type;

typedef struct {
    
  uint16 scr_code;

  uint8 num_paths;

  uint32 win_size;

  uint32 ref_pos;

  uint32 start_pos;

  uint16 path_thresh;

  uint32 max_eng[SRCH_MAX_PILOT_PEAKS];

  uint32 max_pos[SRCH_MAX_PILOT_PEAKS];
  /* Which Set the Cell currently belongs to */
  srch_set_enum_type  set;
  /* boolean to check if ref_pos got updated between 
   * last scheduled search and current window centering.
   */
  boolean ref_pos_upd;
}  srch_ear_pos_cell_struct_type;

#ifdef FEATURE_WCDMA_TPR_BASED_MEASUREMENTS
#error code not present
#endif /* FEATURE_WCDMA_TPR_BASED_MEASUREMENTS */


typedef struct
{
  uint8 irat_type_bmask;
  //The following four variables are used to serve as the counters of trm lock deny for rat
  uint8 trm_lock_deny_counter_for_intra;
  uint8 trm_lock_deny_counter_for_inter;
  uint8 trm_lock_deny_counter_for_gsm;
  #if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  uint8 trm_lock_deny_counter_for_lte;
  #endif

  //The following four variables are used to pass the number of trm lock deny for rat
  uint8 trm_lock_deny_number_for_intra;
  uint8 trm_lock_deny_number_for_inter;
  uint8 trm_lock_deny_number_for_gsm;
  #if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  uint8 trm_lock_deny_number_for_lte;
  #endif

} srch_deny_lock_per_rat_info_struct_type;

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC 

#define SRCH_PRIMARY_ANTENNA 0
#define SRCH_DIVERSITY_ANTENNA 1
#define SRCH_MAX_ANTENNA 2

typedef struct
{
  int32 rssi[SRCH_MAX_ANTENNA];
  int32 ecio[SRCH_MAX_ANTENNA];
  int32 rscp[SRCH_MAX_ANTENNA];
} srch_cell_info_qsh_data_struct_type;
#endif /* FEATURE_WCDMA_QSH_EVENT_METRIC  */

#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
typedef struct 
{
  int32 tot_rxagc;
  int16 prev_rxagc_rpt;
  int16 avg_rxagc;
  uint16 num_samples;
}wsrch_pseudo_meas_rxagc_rpt_type;
#endif

typedef struct{

  #ifdef FEATURE_WCDMA_DC_HSUPA
  boolean srch_sec_ul_freq_configured;
  #endif

  /*--------------------------------------------------------------------------
              The Search Module State Variable
  --------------------------------------------------------------------------*/
  srch_state_enum_type search_state;

  /*A flag used to indicate to the Layer 1 code that indicates whether 
  CPHY_CELL_SELECTION_REQ has been received from RRC.*/
  boolean srch_cphy_cell_selection_recvd;
  srch_step_enum_type srch_otdoa_q_step;
  boolean srch_e1d_lone_cell_enhnc;
  /* Flag indicating if ASET search is needed following the on-going search */
  boolean srch_sync_nset_srch_due;
  /* Variable that holds indication of whether WHL1 and MCAL search vars should be cleared 
 * USAGE: set to TRUE in ISR context when processing command_done for a scheduled search.
 * gets reset in the command_isr handling in TASJ context.
 */
  boolean srch_reset_ops_sanity_vars_needed;

  /* Flag indicating if sync neighbor list search is needed following the 
     on-going search */
  boolean srch_fing_triage_due;

  /*-----------------------------------------------------------------------*
 *       Non-cell-specific Intra-Frequency measurement Parameters        *
 *-----------------------------------------------------------------------*/
  boolean srch_aset_srch_due;
  boolean srch_modem_stat_call;
  /* variable that keeps track of whether a search cleanup is pending or not */
  boolean srch_cleanup_pending;
 boolean srch_in_asu;
  boolean srch_nastt_power_opt;

  /*--------------------------------------------------------------------------
              The mobile's Current Search Frequency Variable
--------------------------------------------------------------------------*/
  uint16 srch_cur_freq;
  boolean srch_nbr_sfn_read_pending[SRCHMEAS_MAX_NUM_CARR];
  uint16 srch_step1_peak_cnt;
  /* srch_rxagc_prim uses SRCH_RXAGC_VAL_FOR_RSCP and srch_rxagc_prim_idle uses SRCH_RXAGC_VAL_DBM. */
  int16 srch_rxagc_prim;
  int16 srch_rxagc_div;
  /* number of peaks configured per task for the PN srch.
   for pd=normal --->    6 peaks (non RxD,combined)      12 peaks (RxD separate)
   for pd=shoulder --->  6*3 peaks (non RxD,combined)  12*3 peaks (RxD separate)    
*/
  uint16 srch_pn_peaks_per_task;
  uint16 srch_nastt_pilot_cnt;
  
  /*--------------------------------------------------------------------------
              The Search Module Time Stamp
--------------------------------------------------------------------------*/
  int16 srch_saved_max_tx_pwr_nv;
  srch_step_enum_type srch_s1_nastt_pn_q_step[SRCH_NUM_Q];
  uint16 srch_pn_srch_cell_cnt;
  srch_interf_cell_struct_type *srch_dmod_interf_nbr_cell_ptr;
  mcalwcdma_srch_pn_result_buffer_type *srch_pn_result_buf;
  srch_pn_srch_bypass_mode_params_struct_type *srch_pn_bypass_srch_params;
  srch_cell_struct_type *srch_dmod_nbr_cell_ptr[SRCHMEAS_MAX_NUM_CARR];
  

  /*database for storing NBR related params */
  wsrch_nbrej_db_struct_type wsrch_nbrej_db;


  
  mcalwcdma_srch_step1_result_buffer_type srch_step1_result_buf;
  /* Fw write sync interface- These are the structures through which whl1 will 
 * send the srch parameters to fw 
 
 */
 mcalwcdma_srch_pn_sync_parms_struct_type srch_pn_sync_parms;
  mcalwcdma_srch_step1_sync_parms_struct_type srch_step1_sync_parms;
    /* step1 async fw write parameters */
  mcalwcdma_srch_step1_async_parms_struct_type srch_step1_parms;
  /* nastt async fw write parameters*/
  mcalwcdma_srch_nastt_async_parms_struct_type srch_nastt_parms;
  
mcalwcdma_srch_common_async_parms_struct_type srch_common_parms;
  
  /*--------------------------------------------------------------------------
              Other Miscellaneous Search Variables
--------------------------------------------------------------------------*/
  srch_nv_struct_type wl1_srch_nv;

  mcalwcdma_srch_nastt_sync_parms_struct_type srch_nas_tt_sync_parms;
  /* Cell Table pointers for those cells sent for PN srch */
  srch_cell_struct_type *srch_pn_srch_cell_ptr[SRCH_NORMAL_MODE_MAX_PN_TASKS];


  /*--------------------------------------------------------------------------
                      PSC (slot) peaks
--------------------------------------------------------------------------*/
  srch_pilot_peak_struct_type srch_nastt_pilots[SRCH_MAX_NASTT_PILOTS];
  
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
		wsrch_pseudo_meas_rxagc_rpt_type wsrch_pseudo_meas_rxagc_rpt;
#endif



}wsrch_ext_cntrl_params_mdump_type;


typedef struct
{








/* A flag to determine when the aset_update log packet is 
   to be logged. This is needed because when aset is updated, the ref
   cell may not be chosen immediately in all cases. This flag will 
   determine when the packet needs to be logged */
  boolean log_aset_update_packet;
  srch_full_srch_type_enum_type srch_async_full_srch_type;
  boolean srch_all_cell_srch_due;

  /*--------------------------------------------------------------------------
              The Search Module State Variable
--------------------------------------------------------------------------*/

  /* Flag to indicate cell parm update due to SIB modification */
  boolean srch_sib_mod_cell_parm_update;
   /*flag to denote if cphy suspend req has been peneded 
    due to an ongoing sfn decode*/
  boolean srch_cphy_pending_on_sfn_done;

  /*-----------------------------------------------------------------------*
 *       Non-cell-specific Intra-Frequency measurement Parameters        *
 *-----------------------------------------------------------------------*/
  boolean srch_meas_db_init;
  /*Will indicate to retain intra frequency measurement database even       during cell transistion*/
  boolean srch_intraf_retain;

  boolean log_nset_update_packet;

  /* flag to indicate if the inter-freq meas database need to be saved across
   state transitions */
  boolean srch_interf_store_meas_db;

  /*--------------------------------------------------------------------------
              The mobile's Current Search Frequency Variable
--------------------------------------------------------------------------*/

  #ifdef FEATURE_MDSP_MULTI_RF_CHAIN_INTERFACE
  /* This value indicates the delay between the primary and diversity RF chains
 * For now, the hardware delays have been corrected and hence, value = 0.
 * For non-zero delays in future, set the correct value here.  
 */
  int16 srch_div_ant_grp_delay_adj;
  #endif

  /*--------------------------------------------------------------------------
              The Search Module Time Stamp
--------------------------------------------------------------------------*/
  uint16 srch_ts_cnt;
  boolean srch_sfn_read_pending_for_interf_timing_maint;

  /*The number of list searches issued since the CPHY_CELL_SELECTION_REQ has been 
  received by Layer 1. */
  uint16 srch_cell_selection_list_search_cnt;

  /* Nc=2048, Nn=1 PN, Nc=2048, Nn=2 PN*/
  uint32 *srch_pn_ecio_tab_ptr;
  /*STTD - NASTT/PN  used during ACQ Nc=128 , Nn=36 */
  uint32 *srch_nastt_pn_sttd_ecio_tab_ptr_nc128;
  uint32 *srch_step1_nn_30_ecio_rxd_tab_ptr;
  /* Non STTD PN search with Nc = 256 , Nn = 16 */
  uint32 *srch_nastt_pn_nonsttd_ecio_tab_ptr_nc256;
  uint32 *srch_step1_nn_40_ecio_tab_ptr;
  /* step1 Nc=256 Nn=40*/
  uint32 *srch_step1_nn_30_ecio_tab_ptr;
  /* Nc=2048, Nn=2 NASTT*/
  uint32 *srch_nastt_ecio_tab_ptr;
  uint32 *srch_step1_ecio_tab_ptr;
  /*step1 Nc=256  Nn=20*/
  uint32 *srch_step1_nn_20_ecio_tab_ptr;
  /* step1 Nc=256 Nn=45*/
  uint32 *srch_step1_ecio_rxd_tab_ptr;
  /* NON-STTD , NASTT/PN used during ACQ Nc=128 , Nn=36 */
  uint32 *srch_nastt_pn_nonsttd_ecio_tab_ptr_nc128;
  int16 srch_rxagc_prim_idle[L1_MAX_NON_USED_FREQ];

  SRCH_FULL_PN_SRCH_DONE_CB_FUNC_TYPE srch_full_pn_srch_cb_func;

  srch_rumi_db_struct_type srch_rumi_db;
  srch_deny_lock_per_rat_info_struct_type srch_deny_lock_per_rat_info;
  /* Data structure to store cell list for cell selection */
  srch_cell_select_list_type srch_cell_select_list;
  rex_crit_sect_type srchfs_scan_substate_crit_sect;
  /* Critical section used to arbitrate concurrent access to the MRM prepared by L1 */
  rex_crit_sect_type srch_meas_rpt_crit_sect;
  #ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
  /* Mutex to perform Intra-Frequency measurement Parameters */
  rex_crit_sect_type wl1_srch_meas_db_init_mutex;
  #endif

    #ifdef FEATURE_WCDMA_CPC_DRX
  rex_crit_sect_type srchdch_cpc_srch_mode_chk_crit_sect;
  #endif

  rex_crit_sect_type wl1_rxd_cb_crit_sect;
  rex_crit_sect_type srch_intra_freq_db_crit_sect;


  rex_crit_sect_type srchsched_intraf_searches_crit_sect;
  rex_crit_sect_type srchzz_parallel_op_mutex_lock;
  #ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
  /* Mutex to perform Inter-Frequency measurement Parameters */
  rex_crit_sect_type wl1_srch_interf_store_meas_db_mutex;
  #endif

  rex_crit_sect_type srch_inter_freq_db_crit_sect;
  /* Critical section used to arbitrate concurrent access to shared structure "srch_ops_sanity_chk" */
  rex_crit_sect_type srch_ops_sanity_chk_crit_sect;
  rex_crit_sect_type srch_internal_meas_db_crit_sect;
  /* Critical section used to arbitrate concurrent access to tbar timer elapsed count */
  rex_crit_sect_type srch_tbar_elapsed_cnt_crit_sect;
  rex_crit_sect_type srch_meas_filt_crit_sec;
  rex_crit_sect_type srchsched_intraf_counter_crit_sect;
  rex_crit_sect_type srch_ts_cnt_crit_sect;

  /* Variable holding the reason why ASET is being updated. It will be fed
     back to L1M when a "dl_cphy_aset_done" commnd is sent.
   */
  wsrch_ui_display_params_type srch_ui_display_params;
  #ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
  srch_cell_info_qsh_data_struct_type srch_cell_info_qsh_data;
  #endif

#ifdef FEATURE_WCDMA_TPR_BASED_MEASUREMENTS
  #error code not present
#endif

  /*--------------------------------------------------------------------------
              Other Miscellaneous Search Variables
--------------------------------------------------------------------------*/
  wl1_srch_e1d_enhancement_vars_struct_type wl1_srch_e1d_vars;
  /* This is the array of interf cell pointers which are sent through regular PN search
by tuning the RF to the target frequency
*/
  srch_interf_cell_struct_type *srch_pn_srch_interf_cell_ptr[SRCH_NORMAL_MODE_MAX_PN_TASKS];
  /* Saved CPHY_CELL_PARM_REQ, used during SIB modification update */
  l1_cell_parm_cmd_type srch_saved_cell_parms_cmd;
  
  /*--------------------------------------------------------------------------
                      PSC (slot) peaks
--------------------------------------------------------------------------*/
  srch_slot_peak_struct_type srch_psc_peaks[SRCH_MAX_PSC_PEAKS];

  /*-----------------------------------------------------------------------*
 * Cell Selection and Reselection Control Parameters of the Serving Cell *
 *-----------------------------------------------------------------------*/
  srch_aset_parms_struct_type srch_aset_parms;

   int16 srch_saved_max_tx_pwr_nv;
  

  /*Mini Dump: Variables that need to be dumped moved to this structure*/
  wsrch_ext_cntrl_params_mdump_type wsrch_ext_cntrl_params_mdump; 
  uint16 ims_filt_rscp_rpt;
}wsrch_ext_cntrl_params_type;

extern wsrch_ext_cntrl_params_type wsrch_ext_cntrl_params[WCDMA_NUM_SUBS];

/* Define for Cell Selection TimeOut in number of frames -- This is the
   duration of the time for which list search will be issued periodically
   using the search heart beat event (ASETsearch will be issued) for
   calculating the S criteria..*/

#define SRCH_CELL_SELECTION_TIMEOUT 20

#define SRCH_SCRIT_FAIL_CNT_RXD_TRIG 2 

/* Default minimum values for cell selection S values */
#define SRCH_SQUAL_DEFAULT_MIN (-32767)
#define SRCH_SRXLEV_DEFAULT_MIN (-32767)


#define CELL_SEL_TIMEOUT( )                                     \
  ( wsrch_ext_cntrl_params[wsub_id].srch_cell_selection_list_search_cnt*SRCH_ASET_SRCH_PERIOD \
      >= SRCH_CELL_SELECTION_TIMEOUT )

/* This enum type represents the bit positions of NV 73569`s "srch_ench_ctrl" field
   that need to be set/reset in order to enable/disable a particular feature */
typedef enum
{
  SRCH_NV_E1D_LONE_CELL       =  0x0 , //0
  SRCH_NV_STEP1_PEAK_PRUNER   =  0x1 , //0
  SRCH_NV_ASYNC_NASTT         =  0x2 , //0
  SRCH_NV_ACQ_IN_DR           =  0x3 , //0
  SRCH_NV_NASST_POWER_OPT     =  0x4 , 
  // MAX cannot exceed 32 since the var is uint32
  //For now, max value is 16
  SRCH_NV_MAX                 =  0x5
}srch_enhnc_ctrl_enums;

#define WSRCH_GET_SRCH_STATE() (wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.search_state)


#define WSRCH_IS_NASTT_POWER_OPT_ENABLED(wsub_id) (wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.srch_nastt_power_opt == TRUE)

/*===========================================================================
                          FUNCTION PROTOTYPES
                          
The followings are prototypes of functions exported to external modules.
===========================================================================*/


/*===========================================================================

FUNCTION SRCH_HPQ_ISR

DESCRIPTION
  This function is the interrupt handler for Searcher HPQ interrupt when HPQ
  search is done.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srch_hpq_isr ( void );

/*===========================================================================

FUNCTION SRCH_LPQ_ISR

DESCRIPTION
  This function is the interrupt handler for Searcher LPQ interrupt when LPQ
  search is done.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srch_lpq_isr ( void );

/*===========================================================================

FUNCTION SRCH_HEAP_MEM_FREE

DESCRIPTION
  This function performs frees searcher heap memory initializations. 

DEPENDENCIES
  srch_heap_mem_alloc must have been called beforehand, or this function
  will do nothing.

RETURN VALUE
  None.

SIDE EFFECTS
  wsrch_ext_cntrl_params[wsub_id].srch_pn_bypass_srch_params and wsrch_ext_cntrl_params[wsub_id].srch_pn_result_buf will be set to NULL.

===========================================================================*/
extern void srch_heap_mem_free( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_HEAP_MEM_ALLOC

DESCRIPTION
  This function performs allocates searcher heap memory initializations. 

DEPENDENCIES
  wsrch_int_cntrl_params[wsub_id].srch_heap_mem_allocated but be FALSE, or this functiom will do nothing.

RETURN VALUE
  None.

SIDE EFFECTS
  wsrch_ext_cntrl_params[wsub_id].srch_pn_bypass_srch_params and wsrch_ext_cntrl_params[wsub_id].srch_pn_result_buf will be set to NULL.

===========================================================================*/
extern void srch_heap_mem_alloc(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_GET_REF_CELL_SCR_CODE

DESCRIPTION
  This function retruns active cell's primary scrambling code. 

DEPENDENCIES
  None.

RETURN VALUE
  Active cell's primary scrambling code.

SIDE EFFECTS
  None.

===========================================================================*/
extern uint16 srch_get_ref_cell_scr_code ( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_GET_CPICH_RSCP

DESCRIPTION
  This function retruns serving cell's RSCP.

DEPENDENCIES
  None.

RETURN VALUE
  Serving cell's RSCP

SIDE EFFECTS
  None.

===========================================================================*/
extern uint16 srch_get_cpich_rscp( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_GET_SAVED_RSCP

DESCRIPTION
  This function returns current frequency on which UE is camped.

DEPENDENCIES
  None.

RETURN VALUE
  Current frequency

SIDE EFFECTS
  None.

===========================================================================*/
extern uint16 srch_get_curr_freq( wsub_id_e_type wsub_id) ;

#ifdef FEATURE_WCDMA_DC_HSDPA
/*===========================================================================

FUNCTION SRCH_GET_CURR_FREQ_SEC_CARR

DESCRIPTION
  This function returns current frequency of the secondary carrier.

DEPENDENCIES
  None.

RETURN VALUE
  Current frequency of secondary carrier

SIDE EFFECTS
  None.

===========================================================================*/
extern uint16 srch_get_curr_freq_sec_carr( wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_DC_HSDPA */

/* Check_again : Write the description of this function*/
extern uint16 srch_clamp_filt_eng(uint16 old_val, uint16 new_val, uint16 clamp_limit, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_GET_FILTERED_RSCP

DESCRIPTION    
  This function returns an unsigned value representing the current,
  filtered RSCP value in P(AGC) + Ec/Io form for all ASET cells. 
  
  The returned can then be arbitrarily mapped to UI display space 
  using an equation, table, or case statement.  
  
  The RSCP is filtered using an (N-1)/N IIR filter for a smoother signal
  strength display. 

DEPENDENCIES
  
    
RETURN VALUE
  An unsigned filtered RSCP value, in 0.5 dBm steps. The actual RSCP is
  -(Returned-unsigned-RSCP-value/2)dBm.

SIDE EFFECTS
  Filtered RSCP is updated.

===========================================================================*/
extern uint16 srch_get_filtered_rscp( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_COMPUTE_RSCP

DESCRIPTION
  This function calculates the RSCP given the ecIo and the RxAGC..
  The calculated RSCP value will be returned in uint16 units. The actual RSCP
  is -ve of the returned value.

DEPENDENCIES
  The cell's ecio have been computed.

RETURN VALUE
  uint16.

SIDE EFFECTS
  None.

===========================================================================*/
extern uint16 srch_compute_rscp( uint16 ecio, int16 rx_agc, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCH_ASYNC_PARM_INIT

DESCRIPTION
  This function initializes searcher asynchronous parameters depending
	on the current state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  wsrch_ext_cntrl_params[wsub_id].srch_step1_parms, srch_step2_parms, srch_step3_parms variables
	are initialized.

===========================================================================*/
extern void srch_async_parm_init( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCH_ASYNC_PARM_INIT_IN_PCH_SLEEP

DESCRIPTION
        This function initializes searcher asynchronous parameters in PCH_SLEEP state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
extern void srch_async_parm_init_in_pch_sleep( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCH_ASYNC_PARM_UPDATE_FOR_PN_IN_PCH_SLEEP

DESCRIPTION
      This function updates the flag srchPNIdleInterfFlag in
      srch PN async param FW interface to value

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
===========================================================================*/
extern void srch_async_parm_update_for_pn_in_pch_sleep(uint8 value, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCH_QUERY_ASYNC_PARM_VAL_FOR_INTERF_PN

DESCRIPTION
      This function returns the value of flag srchPNIdleInterfFlag in
      srch PN async param FW interface. 

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
===========================================================================*/
extern uint8 srch_query_async_parm_val_for_interf_pn( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_EVENT_TIMER_ELAPSED

DESCRIPTION
  This function returns the time elapsed from start_ts passed to it.

DEPENDENCIES
  start_ts should be initialized and updated correctly.

RETURN VALUE
  Time elapsed from start_ts.

SIDE EFFECTS
  None.

===========================================================================*/
extern uint16 srch_event_timer_elapsed( uint16 start_ts , wsub_id_e_type wsub_id);

#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION        NSET_PROCESS_LOG_ON_DEMAND

FILE NAME       srch.c

DESCRIPTION     This funtion creates a NSET log packet and submits it
                to QXDM

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void nset_process_log_on_demand( wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION        ASET_PROCESS_LOG_ON_DEMAND

FILE NAME       srch.c

DESCRIPTION     This funtion creates a ASET log packet and submits it
                to QXDM

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void aset_process_log_on_demand( wsub_id_e_type wsub_id);
#endif

/*===========================================================================

FUNCTION SRCH_HSET_SRCH_CMD

DESCRIPTION
  This function updates searcher HPQ, if needed, based on aset information and 
  issues a search command to DSP. 

DEPENDENCIES
  ASET size is greater than 0, which should be always the case when this
  function is called. Otherwise, DSP behavior is unpredicable.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean  srch_hset_srch_cmd( wsub_id_e_type wsub_id);

#ifdef FEATURE_MDSP_MULTI_RF_CHAIN_INTERFACE
/*===========================================================================

FUNCTION SRCH_CHANGE_ANTENNA

DESCRIPTION:
  Allows RxDiv controller to change the antenna

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Next step 1/2/3 search commands take place on new antenna
===========================================================================*/
extern void srch_change_antenna(uint8 new_ant_id);
#endif /* FEATURE_MDSP_MULTI_RF_CHAIN_INTERFACE */

/*===========================================================================

FUNCTION SRCH_WAKEUP_CELLS

DESCRIPTION:
  API called by DRX manager to wakeup cells. This API can be called in any
  L1 state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Wakeup list search may get triggered in certain states
===========================================================================*/
extern void srch_wakeup_cells(srch_wakeup_set_struct_type wakeup_set_info, wsub_id_e_type wsub_id);

#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION SRCH_WAKEUP_CELLS_NON_DRX_STATE

DESCRIPTION:
  API called by DRX manager to wakeup cells. This API can be called in any
  L1 states except PCH/PCH_SLEEP.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Wakeup list search may get triggered in certain states
===========================================================================*/
extern void srch_wakeup_cells_non_drx_state(srch_wakeup_set_struct_type wakeup_set_info, wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION        srch_resource_avlble_cb

DESCRIPTION     This function is CB funtion passed to WRM whenever TRM resource is
requested. WRM will pass success =TRUE incase lock is acquired within given time, else
success=FALSE if lock req is a failure.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern void srch_resource_avlble_cb(boolean success, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_WAKEUP_CELLS_DONE

DESCRIPTION:
  API called by SRCH to respond to wakeup request from DRX
  L1 state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_wakeup_cells_done(srch_wakeup_set_struct_type wakeup_set_info, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_ENTITY_DROP

DESCRIPTION:
  API called by DRX to drop an entity

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_entity_drop(uint32 bmask_rem_entities,int16 dropped_handle, wsub_id_e_type wsub_id);


#ifdef FEATURE_WCDMA_FS_FALSE_PEAK_ELIM_OPT
/*===========================================================================

FUNCTION SRCH_FS_PROC_DONE_ISR

DESCRIPTION
  This function handles the FS_PROC_DONE interrupt from mDSP

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srch_fs_proc_done_isr ( void );
#endif/*FEATURE_WCDMA_FS_FALSE_PEAK_ELIM_OPT*/

/*===========================================================================

FUNCTION SRCH_STEP1_SRCH_CMD

DESCRIPTION:
  - fill all the parameters to wsrch_ext_cntrl_params[wsub_id].srch_step1_sync_parms
  - call the MCAL api to issue srch
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean srch_step1_srch_cmd(boolean updt_sync_parms, boolean sec_carrier, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION SRCH_NASTT_UNMONITORED_SRCH_CMD

DESCRIPTION:
    issues a NASTT unmonitored search cmd on the 'pos_arr' passed. 'pos_arr' will 
    be the positions of the step1 peaks on which NASTT search will be issued.
    
  - fill all the parameters to srch_nastt_sync_parms
  - call the MCAL api to issue srch
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean srch_nastt_unmonitored_srch_cmd( uint32 (*pos_arr)[2], uint16 num_peaks, boolean sec_carrier, wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION SRCH_NASTT_ASYNC_SRCH_CMD

DESCRIPTION:
  - fill all the parameters to wsrch_ext_cntrl_params[wsub_id].srch_step1_sync_parms
  - call the MCAL api to issue srch
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean srch_nastt_async_srch_cmd( uint32 *pos_arr, uint16 num_peaks);

/*===========================================================================

FUNCTION SRCH_NASTT_SRCH_CMD

DESCRIPTION
  This function issues a NASTT search cmd (both async & Unmonitored). details below
  1) pick the num_step1_peaks for NASTT serch
  2) Advance srchsched_db.step1_nastt_params.cur_psc_peak_indx by the number of peaks currently searched
  3) if its a NASTT(async) then use the async_cell_mask to pick the psc
     which are sent the NASTT cmd. 
  4)For unmonitored NASTT we dont use asyn_cell_mask
  
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - indicates srch cmd was sent successfully.
  FALSE - indicates srch cmd could not be sent successfully.
  
SIDE EFFECTS
  srchsched_db.step1_nastt_params.cur_psc_peak_indx is advanced by the number of step1 peaks which are being sent for
  NASTT search here

===========================================================================*/
extern boolean srch_nastt_srch_cmd
( 
  srch_full_srch_type_enum_type nastt_type, 
  uint16 async_cell_mask,
  uint16 num_step1_peaks,
  boolean sec_carrier  
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_PN_ADD_INTERF_CELLS_FOR_SRCH

DESCRIPTION:
  Adds the inter freq cells for PN search. This PN search will be used in DRX state
  when the RF will be tuned to a diffrerent freq and normal searches will be issued
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

//extern void srch_pn_add_interf_cells_for_srch
//(
//  srch_interf_cell_struct_type  **cell_pptr,
//  uint16 cell_cnt,
//  uint16 max_tasks,
//  boolean reset
//);

/*===========================================================================

FUNCTION SRCH_UPDATE_PN_SRCH_CMD

DESCRIPTION:
    This function issues a PN search cmd on the cells given by 'cell_mask'
  - process cell_mask and add the corresponding cells to the srch_pn_srch_set_ptr
  - call function srch_pn_srch_cmd() to set sync params for the pn srch cmd.
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean srch_pn_send_srch_cmd(uint32 cell_mask, wsub_id_e_type wsub_id);

extern boolean srch_pn_send_srch_cmd_for_asu(uint32 cell_mask, wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION     : SRCHACQ_PROGRAM_FULL_PN_SRCH

DESCRIPTION  : This function  programs a full PN circle search.

DEPENDENCIES : None

RETURN VALUE : None.

SIDE EFFECTS :

=========================================================================*/
extern void srch_pn_program_full_pn_srch 
(
  uint16 freq,
  uint16 scr_code, 
  boolean txd,
  srch_cell_txd_det_status txd_status,
  uint16 num_task,
  SRCH_FULL_PN_SRCH_DONE_CB_FUNC_TYPE full_pn_srch_cb
, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION SRCH_UPDATE_PN_SRCH_CMD

DESCRIPTION:
    This function send a PN search cmd for the inter freq cells
  - fill all the parameters to wsrch_ext_cntrl_params[wsub_id].srch_pn_sync_parms
  - call the MCAL api to issue srch
  
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean srch_pn_srch_cmd_interf_cells(boolean update,uint8 pd, uint8 rxd, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION SRCH_STEP1_GET_RESULT

DESCRIPTION
  call MCAL SRCH api to get the result in wsrch_ext_cntrl_params[wsub_id].srch_step1_result_buf
  in case the RxD mode 1 is used - combine the sorted peaks from
  both the primary and diversity antenna and make a single sorted 
  list in wsrch_ext_cntrl_params[wsub_id].srch_psc_peaks[]

  run duplicate peak detection mechanism to remove duplicate peaks

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srch_step1_get_result ( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION SRCH_NASTT_GET_RESULT

DESCRIPTION:
 
 get the NAS-TT peaks from fw through MCAL in wsrch_int_cntrl_params[wsub_id].srch_nastt_result_buf[]. 
 get the BEST energy peak for a scr code and store that in wsrch_ext_cntrl_params[wsub_id].srch_nastt_pilots[]
 (peaks can be from both Rx antennas in case of RxD mode 1. Pick the strongest 
 among antennas and use it as ref pos for PN srch next time)
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern void srch_nastt_get_result( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_PN_GET_RESULT

DESCRIPTION:
 
 - get the PN peaks from fw through MCAL in wsrch_ext_cntrl_params[wsub_id].srch_pn_result_buf[]. 
 - process wsrch_ext_cntrl_params[wsub_id].srch_pn_result_buf[] and populate the cell structure with
   the results
   (peaks can be from both Rx antennas in case of RxD mode 1)
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern void srch_pn_get_result( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_DETERMINE_RXD_MODE

DESCRIPTION:
  - get the RxD mode as per the current reccomendation 
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern uint16 srch_determine_rxd_mode(srch_step_enum_type srch_step, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_CHECK_IF_FULL_SRCH_WITH_NASTT_ASYNC_NEEDED

DESCRIPTION:
 check if we want to schedule a full search with NASTT async or can we skip this
 in case we dont have any neighbors.
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean srch_check_if_full_srch_with_nastt_async_neeeded( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srch_check_if_any_srch_ongoing

DESCRIPTION
  Depending on the srch step it will check the corresponding que if any srch ongoing.
  This function is here to hide the que level informaition in a single function rather
  than distributed across all srch* files. Later if non OTDOA PN que become separate we need
  to change the logic at this place 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srch_check_if_any_srch_ongoing(srch_step_enum_type srch_step, boolean otdoa, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCH_INTRAF_UPDATE_ENG_PARAMS_POST_FULL_SRCH

DESCRIPTION
  This function updates pos/eng paramsafter a full search. Also calculates
  the ECIO and filt ECIO of the cells that were discovered during full search .

DEPENDENCIES
  This should be called after a NASTT result has already been populated to
  cell structures..

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
extern void srch_intraf_update_eng_params_post_full_srch
(
  /* Cell pointer to intra-frequency cell */
  srch_cell_struct_type *cell_ptr,
  uint16 pilot_idx
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_NONACQ_CONFIG_SEARCHER_ROTATOR

DESCRIPTION:
  Configure searcher rotator in non ACQ state through this function.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_nonacq_config_searcher_rotator(int16 rotator, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_LOG_SRCH_RESULT_STEP1

DESCRIPTION:
  Step 1 log packet version 10

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_log_srch_result_step1( wsub_id_e_type wsub_id);

#if defined (FEATURE_WCDMA_ENH_SRCH_LOGGING) && defined(FEATURE_DC_HSDPA_LOGGING)
/*===========================================================================

  FUNCTION srch_log_srch_result_step1_DCH

DESCRIPTION:
Step 1 log packet in DCH states

DEPENDENCIES
None.

RETURN VALUE
None.

SIDE EFFECTS
None
===========================================================================*/
void srch_log_srch_result_step1_DCH(wsub_id_e_type wsub_id);

/*===========================================================================

  FUNCTION srch_log_srch_result_step1_nonDCH

DESCRIPTION:
Step 1 log packet in non DCH states

DEPENDENCIES
None.

RETURN VALUE
None.

SIDE EFFECTS
None
===========================================================================*/
void srch_log_srch_result_step1_nonDCH(wsub_id_e_type wsub_id);
#endif /*(FEATURE_WCDMA_ENH_SRCH_LOGGING) && defined(FEATURE_DC_HSDPA_LOGGING)*/

/*===========================================================================

FUNCTION SRCH_LOG_SRCH_RESULT_NASTT

DESCRIPTION:
  Log the NASTT packet

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_log_srch_result_nastt( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srch_log_srch_result_pn

DESCRIPTION:
  Log the PN log packet for intra-freq searches

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_log_srch_result_pn( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srch_log_pn_no_rxd_normal_srch_result

DESCRIPTION:
  Log the PN log packet (No RxD + normal search)

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_log_pn_no_rxd_normal_srch_result(srch_cell_type_enum_type srch_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srch_log_pn_rxd_normal_srch_result

DESCRIPTION:
  Log the PN log packet (RxD + normal search)

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_log_pn_rxd_normal_srch_result(srch_cell_type_enum_type srch_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srch_log_pn_no_rxd_shoulder_srch_result

DESCRIPTION:
  Log the PN log packet (No RxD + shoulder search)

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_log_pn_no_rxd_shoulder_srch_result(srch_cell_type_enum_type srch_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srch_log_pn_no_rxd_normal_srch_result

DESCRIPTION:
  Log the PN log packet (RxD + shoulder search)

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_log_pn_rxd_shoulder_srch_result(srch_cell_type_enum_type srch_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srch_log_interf_srch_result_pn

DESCRIPTION:
  Log the PN log packet for inter-freq searches

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_log_interf_srch_result_pn( wsub_id_e_type wsub_id);

#ifdef FEATURE_DC_HSDPA_LOGGING
/*===========================================================================
FUNCTION SRCH_LOG_PN_BYPASS_PEAK_SRCH_RESULT

DESCRIPTION:
  Log the PN bypass peak log packet 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_log_pn_bypass_peak_srch_result( wsub_id_e_type wsub_id);
#endif /* FEATURE_DC_HSDPA_LOGGING */

/*===========================================================================

FUNCTION SRCH_PN_GET_RESULT_INTERF_CELLS

DESCRIPTION:
 
 - get the PN peaks from fw through MCAL in wsrch_ext_cntrl_params[wsub_id].srch_pn_result_buf[]. 
 - process wsrch_ext_cntrl_params[wsub_id].srch_pn_result_buf[] and populate the cell structure with
   the results
   (peaks can be from both Rx antennas in case of RxD mode 1)
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_pn_get_result_interf_cells( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srch_nastt_async_process_peaks

DESCRIPTION:
 
 get the NAS-TT peaks from fw through MCAL in wsrch_int_cntrl_params[wsub_id].srch_nastt_result_buf[]. 
 get the BEST energy peak for a scr code and store that in wsrch_ext_cntrl_params[wsub_id].srch_nastt_pilots[]
 (peaks can be from both Rx antennas in case of RxD mode 1. Pick the strongest 
 among antennas and use it as ref pos for PN srch next time)
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_nastt_async_process_peaks(void);

/*===========================================================================

FUNCTION SRCH_PN_SRCH_CMD

DESCRIPTION:
  - fill all the parameters to wsrch_ext_cntrl_params[wsub_id].srch_pn_sync_parms
  - call the MCAL api to issue srch
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean srch_pn_srch_cmd(boolean update,uint8 pd, uint8 rxd, wsub_id_e_type wsub_id);
extern void srch_set_pn_sync_params(uint8 pd, uint8 rxd, wsub_id_e_type wsub_id);
extern void srch_pn_add_unique_cells_for_srch(srch_cell_struct_type  **cell_pptr, uint16 cnt, uint16 max_tasks, wsub_id_e_type wsub_id);

extern int16 srch_get_freq_offset( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION WSRCH_GET_PN_RESULTS_TASKS_CNT

DESCRIPTION
    This function will be used to return the number of tasks from the last PN search issued

INPUT
    None

DEPENDENCIES
    None.

RETURN VALUE
    Number of tasks

SIDE EFFECTS
  None.
===========================================================================*/
extern uint8 wsrch_get_pn_result_tasks_cnt( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION SRCH_GET_FRAME_PEAK_DIFF

DESCRIPTION
    Given 2 frame peaks, this function computes the PN position(in cx1) between
    the two peaks.

INPUT
    PN position (in cx1) of the 2 frame peaks.

DEPENDENCIES
    PN position passed to this function MUST BE in cx1 units!!.

RETURN VALUE
   PN position (in cx1) between the two peaks.

SIDE EFFECTS
  None.
===========================================================================*/
extern uint32 srch_get_frame_peak_diff(uint32 pn1, uint32 pn2);


/*===========================================================================
FUNCTION WSRCH_GET_PN_RESULTS_PEAKS_CNT

DESCRIPTION
    This function will be used to return the number of peaks per task from the last PN search issued

INPUT
    Task index

DEPENDENCIES
    None.

RETURN VALUE
    Number of peaks

SIDE EFFECTS
  None.
===========================================================================*/
extern uint8 wsrch_get_pn_result_peaks_cnt(uint8 task_idx, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION SRCH_CREATE_DUMMY_CELL

DESCRIPTION
  This function creates a dummy cell that can be added to a search set for
  debugging purposes.

DEPENDENCIES
  None.

RETURN VALUE
  A pointer to the newly-created dummy cell on success, NULL on failure.

SIDE EFFECTS
  None.

===========================================================================*/
extern srch_cell_struct_type* srch_create_dummy_cell( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_RESET_SRCH_SUBSTATE

DESCRIPTION
  This function resets the search DCH substate when the search state is DCH.

DEPENDENCIES
  None.

RETURN VALUE
  True if the substate was reset, false otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srch_reset_srch_substate( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_RESTORE_SRCH_SUBSTATE

DESCRIPTION
  This function restores the search DCH substate to the value stored in          
  srch_dch_reset_substate for the purpose of resuming a search.  

DEPENDENCIES
  None.

RETURN VALUE
  True if the UE is in DCH and the state was not inactive, false otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srch_restore_srch_substate( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION SRCH_ACQ_CHECK_UE_IN_X2W_HHO

DESCRIPTION
  This function calls the rrc api to determine if ue is in x2w hho.

DEPENDENCIES
  None.

RETURN VALUE
 TRUE if UE is in X2W HHO, FALSE if not.
SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srch_acq_check_ue_in_x2w_hho(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION SRCH_ASYNC_PARM_UPDATE_IN_PCH_SLEEP

DESCRIPTION
  This function updates the search parms

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srch_async_parm_update_in_pch_sleep(uint16 freeze_flag, boolean usefreeze_flag, boolean rf_flag, srch_intraf_queue_enum_type queue_to_update, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCH_GET_SRCH_UTILIZATION_FOR_MCVS

FILE NAME       WSRCH.C

DESCRIPTION     This function calculates the searcher utilization based on the parameters passed in and based on other
                          parameters present and it returns the searcher utilization in percentage form.

DEPENDENCIES    

RETURN VALUE    uint8 searcher utilization in percentage (acceptable values 0 - 100)

SIDE EFFECTS    
===========================================================================*/
extern uint8 srch_get_srch_utilization_for_mcvs(
  uint16 c0_srch_period,
  uint32 clk
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCH_GET_FS_PERIOD_FOR_C0

FILE NAME       WSRCH.C

DESCRIPTION     This function returns the full search period for carrier 0.

DEPENDENCIES    

RETURN VALUE    full search period for c0

SIDE EFFECTS    
===========================================================================*/
extern uint16 srch_get_fs_period_for_c0(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_CHECK_RXD_IMBALANCE_THRESH_CONDITIONS

DESCRIPTION
  This function will get called every full DCH cycle in order to set the Step1
  energy threshold correctly. This function will return True if either one of
  the below conditions is met, and False otherwise:
  1. the searcher EcIo's from all ASET cells are below -18 dB
  2. the RxAGC level from either Rx is less than -95 dBm and their RxAGC level
  difference is greater than 5 dB.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srch_check_rxd_imbalance_thresh_conditions( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_SET_STEP1_ENG_THRESH

DESCRIPTION
  This function sets the Step1 energy threshold based on the boolean flag.
  If the flag is set to true, the threshold will be -24.6 dB, otherwise it
  will be -23.3 dB.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srch_set_step1_eng_thresh(boolean lower_thresh, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCH_NASTT_ASYNC_PARM_INIT_DURING_CM

DESCRIPTION
  writes to fw that step1 for intra freq outside CM gaps will be using RxD mode 2 during CM. All
  other step1 async params remain same

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
extern void srch_nastt_async_parm_init_during_cm( wsub_id_e_type wsub_id);

#ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
#error code not present
#endif /* FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED */

extern void srch_complete_abort_all ( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_PN_ADD_CELLS_FOR_PN_SRCH

DESCRIPTION:
    This function issues a PN search cmd on the cells given by 'cell_mask'
  - process cell_mask and add the corresponding cells to the wsrch_ext_cntrl_params[wsub_id].srch_pn_srch_cell_ptr[]
  - call function srch_pn_srch_cmd() to set sync params for the pn srch cmd.
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean srch_pn_add_cells_for_pn_srch(uint32 cell_mask, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_USE_RGS_ROTATOR

DESCRIPTION:
  Returns if RGS values can be used/discarded

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean srch_use_rgs_rotator(tcxomgr_vco_indicator_type vco_type);

/*===========================================================================

FUNCTION SRCH_CARR_ACTIVE

DESCRIPTION
  This function returns true when the specified carrier is in active state

DEPENDENCIES
  State is determined by SRCH ASET count of the carrier

RETURN VALUE
  TRUE: Carrier is active
  FALSE: Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srch_carr_active( uint8 carr_idx , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCH_ASYNC_PARM_INIT_STEP1_THRESH_ON_CPC_START

DESCRIPTION

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
===========================================================================*/
extern void srch_async_parm_init_step1_thresh_on_cpc_start( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCH_ASYNC_PARM_INIT_ON_CPC_STOP

DESCRIPTION

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS
===========================================================================*/
extern void srch_async_parm_init_step1_thresh_on_cpc_stop( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION   SRCH_GET_CARR_IDX_BASED_ON_FREQ

DESCRIPTION
  This function will return the carrier index of DL frequency specified.

DEPENDENCIES
  None.

PARAMETERS
  UARFCN to find the carrier index of

RETURN VALUE
  Carrier Index of DL UARFCN specified.

SIDE EFFECTS
  None.
===========================================================================*/
uint8 srch_get_carr_idx_based_on_freq(uint16 freq, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCH_GET_CARR_FREQ

DESCRIPTION
  This function will return the DL UARFCN of the carrier specified

DEPENDENCIES
  None.

PARAMETERS
  Carrier index to resolve into frequency (UARFCN)
 
RETURN VALUE
  Downlink UARFCN of carrier.

SIDE EFFECTS
  None
===========================================================================*/
uint16 srch_get_carr_freq(uint8 carr_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCH_IS_WKUP_SEARCH

DESCRIPTION
  This function is called by WSRCH before issuing a PN search to check if this 
  is a reacq search in which case a HPQ search is used

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  Boolean 
===========================================================================*/
extern boolean srch_is_wkup_search( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     MCALWCDMA_CHECK_IF_SRCH_IS_ISSUED

DESCRIPTION
  This function will return if the search is issued in one of the queues

DEPENDENCIES
  None.

PARAMETERS
  srch type
 
RETURN VALUE
  boolean
  
SIDE EFFECTS
  None
===========================================================================*/
extern boolean mcalwcdma_check_if_srch_is_issued( srch_step_enum_type srch_type , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     MCALWCDMA_SRCH_CHECK_IF_QUEUE_IS_BUSY

DESCRIPTION
  This function will return if the search is issued in the queue

DEPENDENCIES
  None.

PARAMETERS
  srch type
 
RETURN VALUE
  boolean
  
SIDE EFFECTS
  None
===========================================================================*/
extern boolean mcalwcdma_srch_check_if_queue_is_busy( srch_intraf_queue_enum_type queue , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     MCALWCDMA_GET_SRCH_ISSUED

DESCRIPTION
  This function will return the search which is ongoing when they are single queue at  any point.

DEPENDENCIES
  None.

PARAMETERS
  srch type
 
RETURN VALUE
  boolean
  
SIDE EFFECTS
  None
===========================================================================*/
extern srch_step_enum_type mcalwcdma_get_srch_issued( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_SRCH_E1D_ENHANCEMENT_NV_READ

DESCRIPTION
  This function will return the DL UARFCN of the carrier specified

DEPENDENCIES
  None.

PARAMETERS
  None
 
RETURN VALUE
  Downlink UARFCN of carrier.

SIDE EFFECTS
  None
===========================================================================*/
void wl1_srch_e1d_enhancement_nv_read( wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION wl1_srch_print_e1d_enhancement_vars

DESCRIPTION


DEPENDENCIES
None.

RETURN VALUE
None.

SIDE EFFECTS
None.

===========================================================================*/
void wl1_srch_print_e1d_enhancement_vars(void);

/*===========================================================================

FUNCTION srch_handle_pn_srch_rxd_acq_bch

DESCRIPTION
  After Rxd turns ON, sched is moved to CONFIG_MODE to issue an ad-hoc PN search.
  After the PN search is done, move it back to INTRAF mode

DEPENDENCIES
None.

RETURN VALUE
None.

SIDE EFFECTS
None.

===========================================================================*/
extern void srch_pn_srch_done_rxd_acq_bch( wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_THOR_MODEM) || (defined FEATURE_WCDMA_ATLAS_MODEM)
/*===========================================================================

FUNCTION        SRCH_SAVE_ALL_MCVS_SRCH_UTIL_PARAMS

FILE NAME       WSRCH.C

DESCRIPTION     This function checks if any of the parameters required to calculate the search utilization have changed
                         since the last full search.

DEPENDENCIES

RETURN VALUE    TRUE if at least one parameter has changed in the last search cycle, FALSE otherwise.

SIDE EFFECTS
===========================================================================*/
extern void srch_save_all_mcvs_srch_util_params( wsub_id_e_type wsub_id);

#endif

#ifndef FEATURE_WCDMA_TABASCO_MODEM
/*===========================================================================

FUNCTION        SRCH_CHECK_ANY_MCVS_SRCH_UTIL_PARAMS_CHANGED

FILE NAME       WSRCH.C

DESCRIPTION     This function checks if any of the parameters required to calculate the search utilization have changed
                         since the last full search.

DEPENDENCIES

RETURN VALUE    TRUE if at least one parameter has changed in the last search cycle, FALSE otherwise.

SIDE EFFECTS
===========================================================================*/
extern boolean srch_check_any_mcvs_srch_util_params_changed( wsub_id_e_type wsub_id);

#else

/*===========================================================================

FUNCTION        SRCH_CHECK_ANY_MCVS_SRCH_UTIL_PARAMS_CHANGED

FILE NAME       WSRCH.C

DESCRIPTION     This function checks if any of the parameters required to calculate the search utilization have changed
                         since the last full search.

DEPENDENCIES

RETURN VALUE    

SIDE EFFECTS
===========================================================================*/
extern boolean srch_check_any_mcvs_srch_util_params_changed(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCH_SET_SILVER_SRCH_CYCLES

FILE NAME       WSRCH.C

DESCRIPTION     This function checks if any of the parameters required to calculate the search utilization have changed 
                         since the last full search.

DEPENDENCIES    

RETURN VALUE    TRUE if at least one parameter has changed in the last search cycle, FALSE otherwise.

SIDE EFFECTS    
===========================================================================*/
extern void srch_set_silver_srch_cycles(uint8 new_cycles,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCH_GET_SILVER_SRCH_CYCLES

FILE NAME       WSRCH.C

DESCRIPTION     This function checks if any of the parameters required to calculate the search utilization have changed
                         since the last full search.

DEPENDENCIES

RETURN VALUE    TRUE if at least one parameter has changed in the last search cycle, FALSE otherwise.

SIDE EFFECTS
===========================================================================*/
extern uint8 srch_get_silver_srch_cycles(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCH_REDUCE_UTIL_CALC_NEEDED

FILE NAME       WSRCH.C

DESCRIPTION    This function checks whether reduced util calculation is needed or not. Called before issuing a NASTT search

DEPENDENCIES

RETURN VALUE    TRUE if reduce util is needed

SIDE EFFECTS
===========================================================================*/
extern boolean srch_reduced_util_calc_needed(wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION        SRCH_SET_PRE_UTIL_STEP

FILE NAME       WSRCH.C

DESCRIPTION    This function sets from which step (pre step1/prenastt/pre pn) we have done util calculation 

DEPENDENCIES

RETURN VALUE    NONE

SIDE EFFECTS
===========================================================================*/
extern void srch_set_pre_util_calc_step(wsrch_util_calc_pre_srch_enum_type util_done_pre_srch , wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION        SRCH_GET_PRE_UTIL_STEP

FILE NAME       WSRCH.C

DESCRIPTION    This function gets the previous step (step1/nastt/pn) from where have done util calculation 

DEPENDENCIES

RETURN VALUE    NONE

SIDE EFFECTS
===========================================================================*/
extern wsrch_util_calc_pre_srch_enum_type srch_get_pre_util_calc_step(wsub_id_e_type wsub_id);

#endif
/*===========================================================================
FUNCTION        SRCH_SAVE_SIGNAL_INFO_PRE_SLEEP

FILE NAME       WSRCH.C

DESCRIPTION    This function saves signal info before going to sleep

DEPENDENCIES

RETURN VALUE    NONE

SIDE EFFECTS
===========================================================================*/
extern void srch_save_signal_info_pre_sleep(wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION      wcdma_is_rat_meas_info_ready

DESCRIPTION
This function returns the status of "wcdma_rat_meas_info_ready" variable

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
boolean wcdma_is_rat_meas_info_ready( wsub_id_e_type wsub_id);

extern boolean srch_unlock_cb_for_meas(wrm_secondary_unlock_cb_data* unlock_data_ptr, wsub_id_e_type wsub_id);
extern boolean srch_unlock_cb_for_nbr(wrm_secondary_unlock_cb_data* unlock_data_ptr, wsub_id_e_type wsub_id);



/*===========================================================================

FUNCTION wsrch_nbrej_async_parm_init

DESCRIPTION
  This function writes NBR async parameters to f/w interface.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void wsrch_nbrej_async_parm_init( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wsrch_nbrej_proc_check_fw_unresponsiveness

DESCRIPTION
  This function implements a guard timer mechanism through HB timer
  if f/w doesnt respond to NBR processing .

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean wsrch_nbrej_proc_check_fw_unresponsiveness( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WSRCH_NBREJ_RESET_DB

DESCRIPTION
  This function reset all the NBREJ parameters except the items obtained from NV
  and dont reset the Rxd info.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void wsrch_nbrej_reset_db( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WSRCH_GET_NBREJ_RXLM_IDX

DESCRIPTION
  This function is a way to check if NBR proc is active and also returns
  the RxLM index.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern uint32 wsrch_get_nbrej_rxlm_idx( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wsrch_nbrej_read_nv_params

DESCRIPTION
  This function reads the NBR NV params. It reads 5 parameters.
  NBR tunable parameters through NV item
 boolean enabled; -- NBR can be enabled or disabled.
 uint32 rotatorFreq  -- rotator frequency to be programmed to RXFE_NB_CSR_FREQ_OFFSET_NB0
 uint8 accumBpg -- num of bpgs to accumulate for NBEE
 uint8 threshold1 -- threshold1
 uint8 threshold2; -- threshold1

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void wsrch_nbrej_read_nv_params( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wsrch_nbrej_proc_done_cmd_handler

DESCRIPTION
  This function handles the NBR done local cmd and process the rest of 
  the NBR done ISR from task context.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void wsrch_nbrej_proc_done_cmd_handler( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      srch_qta_start_cmd

DESCRIPTION
Initialize RxD search parmeteres on qta start command

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/

extern void srch_qta_start_cmd( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      srch_qta_stop_cmd

DESCRIPTION
Initialize RxD search parmeteres on qta stop command

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/

extern void srch_qta_stop_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
 FUNCTION WSRCH_RETUNE_TO_CHANNEL
 
 DESCRIPTION
     This change is required only for Jolokia Modem.
     This function will be used in the case where NBR and FET coexists .This function will re tune RF
     to the channel input by 'freq'.After releasing RxLM setting on doing NBR this retune enable RxLM settings ready for FET
 
 INPUT
     target freq to which RF should be tuned to.
 
 DEPENDENCIES
     None.
 
 RETURN VALUE
    .
 
 SIDE EFFECTS
   None.
 ===========================================================================*/
extern void wsrch_retune_to_channel(uint16, wsub_id_e_type wsub_id);

/*===========================================================================
 
 FUNCTION        WSRCH_NV_READ
 
 DESCRIPTION
   This function reads the Searcher NV
 
 DEPENDENCIES
 
 RETURN VALUE
 None
 
 SIDE EFFECTS
 
 ===========================================================================*/
extern void wsrch_nv_read( wsub_id_e_type wsub_id);

 /*===========================================================================
 FUNCTION WSRCH_RETUNE_TO_CHANNEL
 
 DESCRIPTION
     This change is required only for Jolokia Modem.
     This function will be used in the case where NBR and FET coexists .This function will re tune RF
     to the channel input by 'freq'.After releasing RxLM setting on doing NBR this retune enable RxLM settings ready for FET
 
 INPUT
     target freq to which RF should be tuned to.
 
 DEPENDENCIES
     None.
 
 RETURN VALUE
    .
 
 SIDE EFFECTS
   None.
 ===========================================================================*/
extern void wsrch_retune_to_channel(uint16 freq, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_CELL_SEL_RPT

DESCRIPTION
  This function sends a report CPHY_CELL_SELECTION_CNF to RRC on cell
  selection search results.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srch_cell_sel_rpt (boolean lock_failure, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_CELL_SELECT_SRCH_DONE

DESCRIPTION
  This function processes list search dump for cell selection in BCH state.
  This function retrieves the search results from the LPQ and computes
  the s_qual and s_rxlev Cell Selection criteria. It then calls a function
  to send the results to RRC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srch_cell_select_srch_done( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wsrch_ext_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize shared variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void wsrch_ext_cntrl_params_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wsrch_int_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize exclusive variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void wsrch_int_cntrl_params_init( wsub_id_e_type wsub_id);


/*=========================================================================

FUNCTION     : WCDMA_GET_RAT_MEAS_INFO 
               
DESCRIPTION  : This is the API function that CM that will call to get WCDMA 
               serving and neighbor cell info for sub0. 
  
DEPENDENCIES : None

RETURN VALUE : None
      
SIDE EFFECTS : None

=========================================================================*/
extern void wcdma_get_rat_meas_info(cmapi_wcdma_signal_info_struct_type* wcdma_signal_info,wsub_id_e_type wsub_id);


#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================
 
 FUNCTION        SRCH_QSH_STEP1_DUMP_CB
 
 DESCRIPTION
   This function collects the step1 dump cb
 
 DEPENDENCIES
 
 RETURN VALUE
 None
 
 SIDE EFFECTS
 
===========================================================================*/
extern void srch_qsh_step1_dump_cb(wl1_srcher_mini_debug_info_struct_type *addr_ptr_t, wsub_id_e_type wsub_id);

/*===========================================================================
 
 FUNCTION        SRCH_QSH_NASTT_DUMP_CB
 
 DESCRIPTION
   This function collects nastt dump for qsh module during crash.
 
 DEPENDENCIES
 
 RETURN VALUE
 None
 
 SIDE EFFECTS
 
===========================================================================*/
extern void srch_qsh_nastt_dump_cb(wl1_srcher_mini_debug_info_struct_type *addr_ptr_t, wsub_id_e_type wsub_id);

/*===========================================================================
 
 FUNCTION        WL1_SRCHER_DUMP_CB
 
 DESCRIPTION
   This function collects searcher info during crash dump for QSH module
 
 DEPENDENCIES
  None
 RETURN VALUE
  None
 
 SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_srcher_dump_cb(void* addr_ptr);

#endif

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
 /*===========================================================================
 
 FUNCTION        SRCH_LOG_RSSI_FOR_AVG 
  
 DESCRIPTION
   This function uses circular buffer to log last 5 rssi.
 
 DEPENDENCIES
 
 RETURN VALUE
 None
 
 SIDE EFFECTS
 
 ===========================================================================*/
extern void srch_log_rssi_for_avg(wsub_id_e_type wsub_id);


 /*===========================================================================
 
 FUNCTION 		SRCH_GET_AVG_RSSI
 
 DESCRIPTION
   This function returns avg rssi
 
 DEPENDENCIES
 
 RETURN VALUE
 None
 
 SIDE EFFECTS
 
 ===========================================================================*/
extern int16 srch_get_avg_rssi(wsub_id_e_type wsub_id);

/*===========================================================================
 
 FUNCTION        WL1_SRCHER_METRIC_CB
 
 DESCRIPTION
  This function collects searcher info for QSH module.
 
 DEPENDENCIES
  None
 
 RETURN VALUE
  boolean
 
 SIDE EFFECTS
  None
===========================================================================*/
extern boolean wl1_srcher_metric_cb(wl1_qsh_metric_e metric_id, uint8* addr_ptr, wl1_qsh_metric_action_enum_type action, wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_WCDMA_TPR_BASED_MEASUREMENTS
#error code not present
#endif /* FEATURE_WCDMA_TPR_BASED_MEASUREMENTS */

/*===========================================================================

FUNCTION              SRCH_GET_E1D_LONE_CELL_ENHNC_STATUS

DESCRIPTION
  The function stores the value from the NV which determines if E1D_LONE_CELL 
enhancement is defined or not.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srch_get_e1d_lone_cell_enhnc_status(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION             SRCH_IS_E1D_LONE_CELL_ENHNC_ENABLED

DESCRIPTION
  The function returns the value from the NV which determines if E1D_LONE_CELL 
enhancement is defined or not.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean srch_IS_e1d_lone_cell_enhnc_enabled(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_CHECK_NASTT_ISSUING_CONDITIONS

DESCRIPTION
  This function will check whether in DCH state we can issue NASTT search at 180 ms or not.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srch_check_nastt_issuing_conditions( wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION srch_get_uarfcn_for_carrier

DESCRIPTION:
  Provides UARFCN for specified carrier

DEPENDENCIES
  None.

RETURN VALUE
  uint16 UARFCN.

SIDE EFFECTS
  None
===========================================================================*/

extern uint16 srch_get_uarfcn_for_carrier( uint8 carrier_index , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wsrch_send_acq_cnf_after_srch_abort_complete

DESCRIPTION
  This function sends an ACQ report failure to RRC after a search abort done has completed, in case of NBREJ algo
  result has found narrowband signal instead.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void wsrch_send_acq_cnf_after_srch_abort_complete(wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION srch_enable_software_attenuation

DESCRIPTION
  This function enables software attenuation.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean srch_enable_software_attenuation(uint8 val, wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_TABASCO_MODEM

/*===========================================================================
FUNCTION        srch_mcvs_call_after_pn_srch

FILE NAME       WSRCH.C

DESCRIPTION     This function checks if mcvs call is before PN search or not

DEPENDENCIES

RETURN VALUE    TRUE if call is before PN srch

SIDE EFFECTS
===========================================================================*/
boolean srch_mcvs_call_after_pn_srch( wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC 
/*===========================================================================

FUNCTION srch_get_metric_for_cell_info

DESCRIPTION
  This function gets metrics for QSH

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srch_get_metric_for_cell_info(wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_QSH_EVENT_METRIC  */

/*===========================================================================

FUNCTION              SRCH_GET_NASTT_POWER_OPT_STATUS

DESCRIPTION
  The function stores the value from the NV which determines if NASST_POWER_OPT is defined or not.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srch_get_nastt_power_opt_status(wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
   SRCH_PROCESS_RESULTS_FOR_MAAT
  
  DESCRIPTION
    This functions checks whether ecio update for MAAT is needed or not
  
  DEPENDENCIES
    None
  
  RETURN VALUE
   None
  
  SIDE EFFECTS
    None
=============================================================================*/

extern void srch_process_results_for_maat(wsub_id_e_type wsub_id);

/*===========================================================================
  FUNCTION     WSRCH_SEND_MAAT_UPDATE

  DESCRIPTION
  This function updates needed ecio value, div info etc and calls RF i/f API

  DEPENDENCIES
  None

  RETURN VALUE
  None

  SIDE EFFECTS
  None
  ===========================================================================*/

extern void wsrch_send_maat_update(wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
  FUNCTION     wl1_qsh_mdump_collect_searcher

  DESCRIPTION
  This function dumps all the required parameters from searcher for Mini Dump

  DEPENDENCIES
  None

  RETURN VALUE
  None

  SIDE EFFECTS
  None
  ===========================================================================*/

extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_searcher(wsub_id_e_type wsub_id);
#endif
#endif /* SRCH_H */
