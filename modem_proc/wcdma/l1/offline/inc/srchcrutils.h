#ifndef SRCHCRUTILS_H
#define SRCHCRUTILS_H
/*===========================================================================
   S R C H     C E L L     R E S E L E C T I O N     U T I L I T I E S   H E A D E R    F I L E

GENERAL DESCRIPTION
   This file contains resel utility functions called across all WCDMA L1 Files. 

RULES FOR SRCHCRUTILS
1. Common utility Fn for IDLE and FACH
2. Resel utility functions
3. Log Pkt submit functions (subset of 1)
4. No srchcr database maintenance | No writing into any data structure. 
   Only reading is allowed. 
5. All utility macros
6. Should be included only by c-files


EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
  srchcr_init() should be called once we enter DRX mode.

REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2001-2013 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/srchcrutils.h_v   1.5   11 Jul 2002 21:29:46   halbhavi  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/srchcrutils.h#4 $ $DateTime: 2020/01/24 01:28:35 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------
04/27/17   nga     Clenaup RF bufs & WRM complete meas if LTE cell is barred after RF bufs are allocated
03/28/18   sk      Fixing 0x4005 log packet bug (BEST_NBR)
01/23/18   nga     New API to check alt sub in data  And block early evals if IRAT srch done
12/18/17   sk      Reset schduling mode at SIB 19/SIB 11 change
11/03/17   sk      FR 34448: NULL2L support for SSIM and MSIM
10/09/17   mk      Reverse the order of GSM and LTE search issuance in IDLE mode.
08/30/17   gmr     FR 38044: W2L unified DETECT and MEAS scheduling
06/21/16    nga     Clear SRCHCR cell pointers while transitioning out of WCDMA
06/16/16   db      Proposed optimization to Meas/Rank USET cells during Deferred SIB reading
05/20/16   vs      Force Eval will now account for measurement time towards tresel
04/20/16   nga     CSG related fixes.
04/18/16   nga     QSH resel handlers
04/07/16   nga     Code clean up for FEATURE_DELAY_3G_TO_2G_RESEL
04/06/16   nga     Clear target lte_cell_ptr if reselection to LTE failed due to RRC proc active
04/05/16   nga     0x4005 & 0x4027 enahncements- adding 'resel_status'
04/01/16   zt      Idle meas log pkt 0x41CF Version 2
02/24/16   zt      Code clean up for FEATURE_DELAY_3G_TO_2G_RESEL
03/31/16   zt      Support for new log pkt 0x41CF
03/03/16   nga     Check abort when resel ongoing and no meas or eval
01/19/16   db      Introducing an API to check whether meas req needs to be pended.
12/11/15   gsk     Force LTE detect should be the first LTE search that needs to be issued
11/04/15   mk      Introduce API to return gap res eval meas pending flag.
10/30/15   mk      Resetting Tresel on all ngbrs if L1 is in HHO
10/07/15   vs      BSIC Id Reconf optimizations for X+W QTA scenarios.
10/08/15   gsk     Force expire Tresel for cand ngbr cell in case modify band failure
10/06/15   db      Always perform NULL2G in idle mode and perform LTE searches before GSM searches.
09/11/15   db      Check if CSG related searches are pending before allowing early RF shutdown
08/24/15   us      Updates for including rfm_path_type
08/07/15   gsk     Double instantiating srchfach and wsrchlte dB
08/07/15   zt      Double the resel log packets for W+W
08/04/15   vs      Changes related to InterF and IRAT wrm activities.
06/27/15   vr      UL SW changes for unified and modified RF APIs on ATLAS.
06/01/15   gsk     Fixed 0x4027 Log Packet issues and added support so that 0x4027 would only be flushed
                   when it contained measured neighbor cells
05/19/15   sks     Changes to support PICH and measurements in L/W/G + /G.
03/19/15   gsk     New Idle mode unified neighbor scheduler
03/05/15   sad     wl1_rf_state change
03/05/15   psr     Move coefficient arrays to const type
02/20/14   psr     Added SRCHCRUTILS_GET_MAX_TX_POWER_NV macro
02/06/15   psr     Compiler error fixes for Dual sim
01/19/15   sks     Compilation fix
01/28/15   pr      Global reorg for W + W.
01/20/15   jd      Global re-org for W + W
01/19/15   psr     Global re-org for W+W
01/19/15   sks     Global re-org for W+W
01/05/15   sks     Compilation fix.
12/15/14   psr     Thor compilation fixes on top of srchcr.h inclusion cleanup
12/11/14   psr     srchcr.h inclusion cleanup
11/26/14   psr     TH Compilation Fixes - externed functions and added feature definitions
11/11/14   psr     Initial Revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "l1def.h"
#include "wl1m.h"
#include "srchfach.h"

#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_CM_LTE_SEARCH) 
#include "wsrchlte.h"
#endif

/*===========================================================================

                        CONSTANT DECLARATIONS

===========================================================================*/

#ifdef FEATURE_FEMTO_CSG
#define INVALID_CSG_ID 0xFFFFFFFF
#endif

/* Invalid rank RSCP/EcIo field in 0x4027 log packet */
#define INVALID_RANK_VALUE (0xff)

/* Invalid priority field in 0x4027 log packet */
#define INVALID_PRIORITY_VALUE ((int16) 0xff)

/* Max number of GSM cells that are logged */
#define SRCHCR_MAX_GSM_CELLS_FOR_LOGGING 4

#define STEP1_PEAK_ELIMINATION_POS_DIFF 32

#ifdef FEATURE_2G_RANK_Q_OFFSET_OPT
/* Minimum limit of Q-offset1 for cell reselection to GSM */
#define SRCHCR_MIN_Q_OFFSET_S_N   10
#endif

#ifdef FEATURE_IMMEDIATE_RESEL_AT_LOW_CPICH_ECIO
/* CPICH Ec/Io below which default Treselection to 0 = -16 dB */
#define SRCHCR_SERV_CPICH_THRESH_FOR_TRESEL_0 (32)
#endif /* FEATURE_IMMEDIATE_RESEL_AT_LOW_CPICH_ECIO */

#ifdef FEATURE_IGNORE_TRESEL_ON_S_FAIL

/* Weak reselection condition is reached when MAX N CR or more reselections happen in
  * MAX_TCR_S or less time. The condtions exists for MAX_WEAK_RESEL_HYST after which 
  * it has to be checked again.
  */

/* MAX_N_CR should always be set to a value less then or equal to L1_MAX_N_CR_VALUE */
#define MAX_N_CR 5
#define MAX_TCR_S 60
#define MAX_WEAK_RESEL_HYST 10

#endif


/* Value of 5s in terms of 10ms frames */
#define SRCHCR_FIVE_SECONDS_IN_FRAMES   500

/* Value of 800ms in terms of FACH aset updates */
#define FACH_800_MS_IN_NUM_ASET_UPDATES (800 / (SRCH_ASET_SRCH_PERIOD * 10))

#define MAX_PEAK_IDX 3
#define INTRA_F_PEAK_IDX 2
#define LTE_NUM_MAX_MEAS_AFTER_DETECT 3

/*   
   ----------------------------------------
      Treselection Timer Max Possible Value
   ----------------------------------------
   
    For intra-freq, the actual Tresel is the Tresel times the speed_scaling factor if high-mobility state is detected;
    For inter-freq and IRAT, the actual Tresel is the Tresel times the speed_scaling factor (if high-mobility state
        is detected and if this variable is present) and the scaling factor (if present).
  
    Quote from 25.304: 
    "For inter-frequency cells and high-mobility state is detected:
      -	multiply Treselections or Treselections,PCH or Treselections,FACH by both the IEs "Speed dependent ScalingFactor
        for_Treselection" if sent on system information and "Inter-Frequency ScalingFactor for Treselection" if sent on system information."
    "For inter-RAT cells and high-mobility state is detected:
      - multiply Treselections or Treselections,PCH or Treselections,FACH by both the IEs "Speed dependent ScalingFactor for_Treselection"
        if sent on system information and "Inter-RAT ScalingFactor for Treselection" if sent on system information."

    From 25.331, the range of the parameters are summarized in below table:

       Treselections        Integer 0-31 seconds	
       Treselections,PCH    Integer 0-31 seconds          Used for UE in CELL_PCH or URA_PCH state
       Treselections,FACH	  Real (0..6.2 by step of 0.2)	Used for UE in CELL_FACH state
   
       Speed dependent Scaling Factor 	Real(0..1 by step of 0.1)       Used for UE in high mobility state as scaling factor 
       for Treselection                                                 for Treselections or Treselections,PCH or Treselections,FACH
       Inter-frequency Scaling Factor 	Real (1..4.75 by step of 0.25)	Used for UE as scaling factor for Treselections or Treselections,PCH or
       for Treselection                                                 Treselections,FACH for inter-frequency cell reselection
       Inter-RAT Scaling Factor for     Real (1..4.75 by step of 0.25)	Used by the UE as scaling factor for Treselections or Treselections,
       Treselection	                                                    PCH or Treselections,FACH for inter-RAT cell reselection

      Summary:
      For inter-freq and IRAT, the maximum Tresel = 31 seconds * 1 (speed scaling) * 4.75 = 147.25 secs.
      For intra-freq, the maximum Tresel = 31 seconds * 1 (speed scaling) = 31 secs.
 */

/* Maximum possible value of Treselection in number of frames */
#define SRCHCR_MAX_INTRA_TRESEL_FRAMES  3100

/* Maximum possible value of Treselection in number of frames */
#define SRCHCR_MAX_TRESEL_FRAMES  14800

/* 32 DRX cycles in case of DRX cycle lenght 80ms is the max t-meas-gsm value*/
#define MAX_T_MEAS_GSM_DRX_CYCLES  32 
/* 30sec / minimum DRX cycle length 80ms is max T-BSIC verif timer value*/
#define MAX_T_BSIC_VERIF_DRX_CYCLES  375  

/* Min # of frames between two normal intra freq full searches in DRX mode*/
#define SRCHCR_DRX_MIN_FRAMES_BETWEEN_NORMAL_INTRA_FULL_SRCH  3000   /* 30s */

/* Min # of frames between two panic intra freq full searches in DRX mode*/
#define SRCHCR_DRX_MIN_FRAMES_BETWEEN_PANIC_INTRA_FULL_SRCH  50   /* 500ms */

/* Min # of frames between two normal inter freq full searches in DRX mode*/
#define SRCHCR_DRX_MIN_FRAMES_BETWEEN_NORMAL_INTER_FULL_SRCH  3000   /* 30s */

/* Min # of frames between two panic inter freq full searches in DRX mode*/
#define SRCHCR_DRX_MIN_FRAMES_BETWEEN_PANIC_INTER_FULL_SRCH  100   /* 1s */

/* Min # of frames between two normal intra freq full searches in FACH mode*/
#define SRCHCR_FACH_MIN_FRAMES_BETWEEN_NORMAL_INTRA_FULL_SRCH  36  /* 360ms */

/* Num consecutive reacq failures for ignoring HCS during reselection eval */
#define NUM_REACQS_FOR_IGNORING_HCS  3


/* Max value of Sintra_search */
#define MAX_S_INTRA_SEARCH 24

#define MOBILITY_TH 5
#define PRED_MARGIN 5
#define DROP_THRESHOLD_ECIO 46
#define EXPIRY_TIME_FOR_DROP_TH 1500
/*Step1 search threshold value is -16.5 we compare with value multipled by 2*/
#define STEP1_SRCH_THRSH -33

#define MAX_PN_CELL_CNT_FOR_ENH_PN 10

#ifdef FEATURE_WCDMA_PANIC_SRCH_OPT
#define MAX_PN_ACM_CELL_CNT_FOR_INTRA (SRCH_SYNC_NSET_MAX_C0 + SRCHCR_USET_MAX)
#define MAX_PN_ACM_CELL_CNT_FOR_INTER  SRCH_SYNC_NSET_MAX_C0
#endif


/* Default value for meas prio */
//#define SRCHCR_MEAS_PRIO_NONE 0 /*Confirm and delete*/

#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  #define MAX_NUM_SRCHED_CELLS   (SRCH_SYNC_NSET_MAX_C0 +  \
                                  SRCH_OFREQ_CELL_MAX + \
                                  SRCH_GSM_NSET_MAX + \
                                  WSRCHLTE_NSET_MAX )
#else
  #define MAX_NUM_SRCHED_CELLS   ( SRCH_SYNC_NSET_MAX_C0 +  \
                                   SRCH_OFREQ_CELL_MAX + \
                                   SRCH_GSM_NSET_MAX )
#endif

/*Default value for the Treselection scaling factor*/
#define SRCHCR_DEFAULT_TRESEL_SCALE  4

#define SRCHCR_NV_MAX_TX_PWR_INCREMENT 1

/* The max number of intra-frequency cells in Unlisted Set in Idle mode
 * and FACH. The number is chosen so that <srch_sync_nset_cnt+srch_uset_cnt>
 * is 41, same as SRCH_LPQ_MAX_TASKS.
 */
#define SRCHCR_USET_MAX  9

/* Maximum value of Ncr, used to define the array size for 
 * storing reselection times 
 */
#define L1_MAX_N_CR_VALUE  16


/*===========================================================================

                        TYPE DECLARATIONS

===========================================================================*/

/* BSIC id and verification statistics */
typedef struct
{
  /* number of times BSIC identification was attempted */
  uint16 num_bsic_ids;
  /* number of times BSIC identification passed */
  uint16 num_bsic_id_pass;
  /* number of times BSIC identification was attempted */
  uint16 num_bsic_ver;
  /* number of times BSIC identification passed */
  uint16 num_bsic_ver_pass;
} srchcr_bsic_stats_struct_type;


/* Structure used to store the time at which UE sucessfully reselects.
 * This information is needed to calculate if the UE is in normal state
 * or fast moving state
 * Note that both Tcrmax and Tcrmaxhyst are in seconds and hence as a 
 * first pass the reselection times are being stored at a second 
 * granularity to avoid qword computations. If needed it can be 
 * implemented at a ms granularity using qword data type
 */
typedef struct
{
  /* Index into the reselection times structure */
  uint16 res_time_index;

  /* Time in sec when UE completes a successful reselection.
   * This is one more than Max Ncr value since UE declares
   * fast moving when num reselection in time period Tcrmax
   * EXCEEDS Ncr
   */
  dword res_time[L1_MAX_N_CR_VALUE + 1];
} srchcr_res_times_struct_type;


/* Enum to indicate status of cell reselection evaluation */
typedef enum 
{
  SRCHCR_RES_EVAL_DISABLED,                    /* Before 1s timer expiry */
  SRCHCR_RES_EVAL_INACTIVE,                    /* Possible now (1s timer has expired)  */
  SRCHCR_RES_EVAL_PENDING_MEAS_COMPLETE
} srchcr_res_eval_status_type;

/* Enum to indicate eval meas type */
typedef enum {
  SRCHCR_RES_EVAL_MEAS_NONE         = 0x0,
  SRCHCR_RES_EVAL_MEAS_INTRA_LIST   = 0x1,
  SRCHCR_RES_EVAL_MEAS_INTRA_FULL   = 0x2,
  SRCHCR_RES_EVAL_MEAS_INTRA        = 0x3,
  SRCHCR_RES_EVAL_MEAS_INTER_0_LIST = 0x4,
  SRCHCR_RES_EVAL_MEAS_INTER_0_FULL = 0x8,
  SRCHCR_RES_EVAL_MEAS_INTER_0      = 0xC,
  SRCHCR_RES_EVAL_MEAS_INTER_1_LIST = 0x10,
  SRCHCR_RES_EVAL_MEAS_INTER_1_FULL = 0x20,
  SRCHCR_RES_EVAL_MEAS_INTER_1      = 0x30,
  SRCHCR_RES_EVAL_MEAS_INTER        = 0x3C,
  SRCHCR_RES_EVAL_MEAS_INTRA_INTER  = (SRCHCR_RES_EVAL_MEAS_INTRA|SRCHCR_RES_EVAL_MEAS_INTER),
  SRCHCR_RES_EVAL_MEAS_GSM_RSSI    = 0x40,
  SRCHCR_RES_EVAL_MEAS_GSM_BSIC    = 0x80,
  SRCHCR_RES_EVAL_MEAS_GSM         = 0xC0,
  SRCHCR_RES_EVAL_MEAS_INTER_GSM   = 0xFC,
  SRCHCR_RES_EVAL_MEAS_NON_LTE = 0xFF,
  #if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  //new bitmask
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_DETECT_F0        = 0x100, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_DETECT_F1        = 0x200, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_DETECT_F2        = 0x400, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_DETECT_F3        = 0x800, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_DETECT_F4        = 0x1000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_DETECT_F5        = 0x2000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_DETECT_F6        = 0x4000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_DETECT_F7        = 0x8000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_DETECT          = 0xFF00, 

  SRCHCR_RES_EVAL_MEAS_LTE_CELL_MEAS_F0        = 0x10000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_MEAS_F1        = 0x20000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_MEAS_F2        = 0x40000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_MEAS_F3        = 0x80000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_MEAS_F4        = 0x100000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_MEAS_F5        = 0x200000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_MEAS_F6        = 0x400000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_MEAS_F7        = 0x800000, 
  SRCHCR_RES_EVAL_MEAS_LTE_CELL_MEAS           = 0xFF0000,
  SRCHCR_RES_EVAL_MEAS_LTE                     = 0xFFFF00,
  SRCHCR_RES_EVAL_MEAS_NON_INTRA = (SRCHCR_RES_EVAL_MEAS_INTER_GSM|SRCHCR_RES_EVAL_MEAS_LTE),
  SRCHCR_RES_EVAL_MEAS_IRAT                    =(SRCHCR_RES_EVAL_MEAS_GSM|SRCHCR_RES_EVAL_MEAS_LTE), 
  SRCHCR_RES_EVAL_MEAS_ALL                     = 0xFFFFFF

  #else
  SRCHCR_RES_EVAL_MEAS_NON_INTRA = SRCHCR_RES_EVAL_MEAS_INTER_GSM,
  SRCHCR_RES_EVAL_MEAS_IRAT      =(SRCHCR_RES_EVAL_MEAS_GSM|SRCHCR_RES_EVAL_MEAS_LTE), 
  SRCHCR_RES_EVAL_MEAS_ALL       = 0xFF
  #endif
} srchcr_res_eval_meas_type;


typedef enum
{
	INTER_FREQ_0,
  INTER_FREQ_1,
	ASET_FREQ,
	FREQ_MAX
} srchcr_freq_idx_enum_type;

/* Frequency index */
typedef enum
{
  FREQ_0,
  FREQ_1,
  FREQ_NONE
} srchcr_inter_f_idx_enum_type;

/* Neighbor cell type */
typedef enum
{
  INVALID_CELL=0x0,
  INTRA_F_CELL=0x1,
  INTER_F_CELL=0x2,
  INTER_S_GSM_CELL=0x4,
  INTER_S_LTE_CELL=0x8,
  ALL_CELLS=0xF
} srchcr_cell_type_enum_type;

typedef enum
{
  /* HIGH enum indicates criteria 1 to be calculated, when ABSOLUTE_PRIORITY is used */
  SRCH_HIGH_PRIORITY_LAYER,    
  /* EQUAL enum indicates criteria 2 to be calculated, when ABSOLUTE_PRIORITY is used */
  SRCH_EQUAL_PRIORITY_LAYER,
  /* LOW enum indicates criteria 3 to be calculated, when ABSOLUTE_PRIORITY is used */
  SRCH_LOW_PRIORITY_LAYER
}
srchcr_priority_enum_type;

typedef enum
{
  /*Serving cell */
  SERVING_CELL = 0,
  /*Best Neighbor - going to trgger cel resel ind */
  BEST_NBR = 1,
  /* Tresel is running */
  TRESEL_RUNNING = 2,
      /* S failed on current cell*/
  S_CRITERIA_FAILED = 3,
  /* Serv_rank > nbr_rank  Tresel cleared/not started*/
  NBR_RANK_BELOW_SERVING = 4,
    /* NBR cell does not meet threshold to run Tresel*/
  ABS_PRIO_CRITERIA_FAILED = 5,
  /* Freq Barred, Set Rank Minimum */
  FREQ_BARRED = 6,
  /* Cell Barred, Set Rank Minimum */
  CELL_BARRED = 7,
  /* Exclude detected cells based on serving cell S & nbr cell EcIo*/
  USET_CRITERIA_FAILED = 8,
  /* if Squal>Sx, exlcude x */
  CR_MEAS_RULES_NOT_SATISFIED = 9,
  /*HCS Stationary - Exclude all except highest HCS prio cell */
  HCS_CRITERIA_FAILED = 10,
  /*HCS- Fast moving, exclude all except lowest HCS prio among lower prio cells or lowest prio cell among higher prio cells*/
  HCS_CRITERIA_FAILED_FAST_MOVING_UE = 11,
  /* MAX enum */
  INVALID_RESEL_STATUS = 255
}
srchcr_resel_status_enum_type;

typedef struct
{
  /* Indicates if the cell is Intra-F, Inter-F or GSM */
  srchcr_cell_type_enum_type    cell_type;

  /* flag to exclude cell from ranking */
  boolean exclude;

  /* flag to store the resel status i.e. the reason why this cell is not picked as best cell */
  srchcr_resel_status_enum_type resel_status;

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH
  /*flag to indicate if the cell is a resel candidate*/
  boolean is_fach_absprio_resel_cand;
  boolean exclude_for_legacy;
#endif

  union
  {
    /* Pointer to Intra-Freq cell information */
    srch_cell_struct_type      *wcdma_intra;
    /* Pointer to Inter-RAT GSM cell information */
    srch_gsm_cell_struct_type  *gsm;
    /* Pointer to Intra-Freq cell information */
    srch_interf_cell_struct_type *wcdma_inter;
#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
    wsrchlte_cell_struct_type  *lte;
#endif
  }u;

} srchcr_rank_cell_struct_type;

typedef enum
{
  /* In high priority scheduling mode, cells with a higher priority will only be considered during reselection */
  SRCHCR_HIGH_PRIORITY_SCHEDULING_MODE,
  /* In all priority scheduling mode, all cells are considered regardless of priority */
  SRCHCR_ALL_PRIORITY_SCHEDULING_MODE,
  /*Invalid priority scheduling mode when absolute priority is not defined*/
  SRCHCR_INVALID_PRIORITY_SCHEDULING_MODE = 0xFF
} srchcr_prior_scheduler_mode_enum_type;

/* Enum to indicate meas rules type */
typedef enum {
  SRCHCR_MEAS_NONE    = 0x0,
  SRCHCR_MEAS_INTRA   = 0x1,
  SRCHCR_MEAS_INTER   = 0x2,
  SRCHCR_MEAS_GSM    = 0x4,
#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  SRCHCR_MEAS_LTE     = 0x8,
  SRCHCR_MEAS_IRAT     = (SRCHCR_MEAS_GSM|SRCHCR_MEAS_LTE),
#else
  SRCHCR_MEAS_IRAT     = SRCHCR_MEAS_GSM,
#endif
  SRCHCR_MEAS_NON_INTRA = 0xE,
  SRCHCR_MEAS_ALL     = 0xF

} srchcr_meas_rules_type;

/* Structure used to store UE's fast moving status and fast moving end
 * time
 */
typedef struct
{
  /* Flag to indicate that UE is in fast moving state */
  boolean fast_moving;

  /* Time in s since power up when UE exits fast moving status. This 
   * time includes Tcrmaxhyst time too
   */
  dword fast_moving_end_time;
}
srchcr_ue_fast_moving_struct_type;

/* Barred Cells list of this structure type. Same as CELL_BAR_CMD struct*/
typedef struct
{
  l1_cell_bar_info_e_type l1_cell_bar_info;        /* GSM or FDD cell*/
  union
  {
    l1_gsm_cell_identity_struct gsm;
    l1_fdd_cell_identity_struct fdd;
#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
    l1_eutra_cell_info_type lte;
#endif
  }id;
  uint32 t_barred;            /* T barred timer value */
#ifdef FEATURE_SUPPORT_FOR_PARTIAL_ROAMING
  boolean bar_for_resel_only; /* Unbar this barred cell info on emergency call*/
  boolean non_eq_plmn_barred; /* Unbar this on eq plmn change */
#endif

  boolean prev_freq_bar; /* Frequency barred initially from SIB.*/

#ifdef FEATURE_WCDMA_UNBAR_OPRTR_RSVD_FREQ
  /* indicates if reason for freq barring - reserved for operator use */
  boolean operator_use_barred;
#endif
#ifdef FEATURE_FEMTO_CSG
  boolean bar_non_white_csg;
#endif
  /* indicates the reason for barring - whether band mismatch or not */
  boolean band_mismatch;
} srchcr_barred_cell_struct;


/*===========================================================================

                   EXTERNAL VARIABLE DECLARATIONS

===========================================================================*/


/*===========================================================================

                         VARIABLE DECLARATIONS
                                
===========================================================================*/


#ifdef FEATURE_DUAL_SIM_WCDMA_TUNEAWAY
uint16 srchcr_tuneaway_dur_frms;
#endif


/*===========================================================================

                                MACROS
                                
===========================================================================*/

/* Define an ASSERT which will WL1_ERR_FATAL() */
#define ASSERT_CR( x )                      \
  if ( !(x) )                               \
  {                                         \
    WL1_ERR_FATAL( "ASSERT failed!", 0, 0, 0 ); \
  }


#define SRCHCRUTILS_INTER_F_CELLS_PRESENT( ) \
  (srchsetutil_get_interf_sync_nset_count(wsub_id) || srchsetutil_get_interf_async_nset_count(wsub_id))
#define SRCHCRUTILS_INTER_F_SYNC_CELLS_PRESENT_FREQ_0( ) \
   (srchinterf_ext_cntrl_params[wsub_id].srch_ofreq_sync_nset[FREQ_0].cell_cnt > 0)
#define SRCHCRUTILS_INTER_F_SYNC_CELLS_PRESENT_FREQ_1( ) \
  (srchinterf_ext_cntrl_params[wsub_id].srch_ofreq_sync_nset[FREQ_1].cell_cnt > 0)  
#define SRCHCRUTILS_INTER_F_SYNC_CELLS_PRESENT( )  (srchsetutil_get_interf_sync_nset_count(wsub_id) > 0)
#define SRCHCRUTILS_INTER_F_ASYNC_CELLS_PRESENT( ) (srchsetutil_get_interf_async_nset_count(wsub_id) >0)

#define SRCHCRUTILS_INTER_S_CELLS_PRESENT()  (srchsetutil_get_gsm_nset_count(wsub_id) > 0)

#ifdef FEATURE_DUAL_SIM
#define SRCHCRUTILS_MAX_POSSIBLE_BSIC_DECODES() (WL1_IN_G2W_TA_MODE() ? 2 : 4)
#else
#define SRCHCRUTILS_MAX_POSSIBLE_BSIC_DECODES() 4
#endif

#ifdef FEATURE_REL5

#define TRESEL_ACCURACY_IDLE 100 /* 1s in frames */
#define TRESEL_ACCURACY_FACH 20 /* 0.2 s in frames*/

/* Round up to the next 0.2 s in FACH or 1s in idle*/
#define ROUND_UP(number,accuracy) \
   ( ( (number+(accuracy-1)) / accuracy) * accuracy)

/* Round up depending on state.
   In IDLE round off to nearest second. 
   In FACH round off to the nearest 0.2 sec.*/
#define SRCHCR_TRESEL_ACCURACY() \
  ((WL1_GET_L1M_STATE() == L1M_FACH)? TRESEL_ACCURACY_FACH : TRESEL_ACCURACY_IDLE)

#endif

#define SRCHCRUTILS_IS_TRESEL_RUNNING(cell_ptr) (cell_ptr->evt_cr_ts.enabled)

#define CHECK_WRAP_AROUND(A)   ( A= (A>1) ? 0: A)

#define SRCHCRUTILS_INTER_F_ASYNC_CELLS_PRESENT_PER_FREQ(freq_index)  (srchinterf_ext_cntrl_params[wsub_id].srch_ofreq_async_nset[freq_index].cell_cnt > 0)
#define SRCHCRUTILS_INTER_F_SYNC_CELLS_PRESENT_PER_FREQ(freq_index)  (srchinterf_ext_cntrl_params[wsub_id].srch_ofreq_sync_nset[freq_index].cell_cnt > 0)

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY

#define SRCHCRUTILS_HIGH_PRIO_SCHED_MODE() (srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_curr_prior_scheduling_mode == SRCHCR_HIGH_PRIORITY_SCHEDULING_MODE)

#endif

#define DETMARGIN_ECIO(meas_quan) ((meas_quan == L1_EC_NO) ? 3 : 0)
#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
#define DETMARGIN_RSCP(meas_quan) ((meas_quan == L1_RSCP)? 6 : 0)
#else
#define DETMARGIN_RSCP(meas_quan) ((meas_quan == L1_RSCP)? 5 : 0)
#endif

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH
#define IS_NOT_FACH_RESEL_CAND(idx) ((wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.search_state == SRCH_FACH_STATE) && (!srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_srched_cells[idx].is_fach_absprio_resel_cand))
#endif

#ifdef FEATURE_WCDMA_REACQLESS_PICH

#define SRCHCRUTILS_NGBRS_PRESENT()\
    ((srchsetutil_get_sync_nset_count_c0(wsub_id) > 0)          || \
     (srchsetutil_get_interf_sync_nset_count(wsub_id) > 0)      || \
     (srchsetutil_get_uset_count_c0(wsub_id) > 0)               || \
     (srchcr_check_for_active_gsm_nbr_cells(wsub_id))           || \
     (wsrchlte_get_kdetected(wsub_id) > 0))

#endif

/* Additional hysterisis to be used with Qhyst2s
 * Qhyst2s_add = 2 dB,             if Ec/Io < -12dB
 *             = (14+Ec/Io) dB,    if -12dB < Ec/Io < -10dB
 *             = 4dB,              if Ec/Io > -10dB
 */
#define SRCHCR_QHYST2_ADD(ecio)                               \
  (ecio < -24) ? 4 :                                          \
   ( ((ecio <= -20) && (ecio >= -24)) ? (28+(ecio)) : 8 );

/* Check if the cell meets S criteria */
#define SRCHCRUTILS_CELL_MEETS_S_CRITERIA( cell_ptr )     \
 ((cell_ptr->s_qual > 0) && (cell_ptr->s_rxlev > 0))

/* Macro to indicate current meas rules status */
#define SRCHCRUTILS_MEAS_RULES_INDICATE_MEAS()       (srchcrutils_get_srchcr_current_meas_rules(wsub_id) != 0)

#define SRCHCRUTILS_MEAS_RULES_INDICATE_INTRA_MEAS() srchcrutils_get_srchcr_meas_rules_indicate_intra_meas(wsub_id)
#define SRCHCRUTILS_MEAS_RULES_INDICATE_INTER_MEAS() srchcrutils_get_srchcr_meas_rules_indicate_inter_meas(wsub_id)
#define SRCHCRUTILS_MEAS_RULES_INDICATE_GSM_MEAS() srchcrutils_get_srchcr_meas_rules_indicate_gsm_meas(wsub_id)


#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
#define SRCHCRUTILS_MEAS_RULES_INDICATE_LTE() srchcrutils_get_srchcr_meas_rules_indicate_lte_meas(wsub_id)
#endif

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
/* Macro to check if this prior is higher then serv cell */
#define SRCHCRUTILS_IS_PRIOR_HIGHER_THEN_SERVING_CELL_PRIOR(prior) (prior > wsrch_ext_cntrl_params[wsub_id].srch_aset_parms.prior_info.priority) 

#define SRCHCRUTILS_FACH_LEGACY_RULES() \
  ((wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.search_state == SRCH_FACH_STATE) && \
  (srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_prior_info_for_rats.fach_abs_prio_ue_cap_sib_ind == LEGACY))

#define SRCHCRUTILS_FACH_ABS_PRIO_USED() \
  ((wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.search_state == SRCH_FACH_STATE) && \
   (wsrch_ext_cntrl_params[wsub_id].srch_aset_parms.prior_used) && \
   (srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_prior_info_for_rats.fach_abs_prio_ue_cap_sib_ind != LEGACY) )

#define SRCHCRUTILS_DRX_ABS_PRIO_USED() \
  ((wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.search_state == SRCH_DRX_STATE) && \
   (wsrch_ext_cntrl_params[wsub_id].srch_aset_parms.prior_used))

/* Perform legacy ranking 
 *  When DRX/FACH absolute priority isnt defined and inter-freq/GSM doesnt have priorities
 *  When DRX/FACH absolute priority is defined and inter-freq/GSM doesnt have priorities
 *  When FACH absolute priority isnt defined and inter-freq/GSM have priorities
 */
#define SRCHCRUTILS_LEGACY_INTERF_RANKING_NEEDED() \
  (!(SRCHCRUTILS_INTERF_PRIO_PRESENT() && \
   (SRCHCRUTILS_FACH_ABS_PRIO_USED() || SRCHCRUTILS_DRX_ABS_PRIO_USED())))

#define SRCHCRUTILS_LEGACY_GSM_RANKING_NEEDED() \
  (!(SRCHCRUTILS_IS_GSM_PRIO_PRESENT() && \
      (SRCHCRUTILS_FACH_ABS_PRIO_USED() || SRCHCRUTILS_DRX_ABS_PRIO_USED())))

#define SRCHCRUTILS_INTERF_PRIO_PRESENT() (srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_prior_info_for_rats.interf_prior_info_present)
#define SRCHCRUTILS_IS_GSM_PRIO_PRESENT() (srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_prior_info_for_rats.gsm_prior_info_present)
#define SRCHCRUTILS_IS_LTE_PRIO_PRESENT() (srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_prior_info_for_rats.lte_info_present)

#endif

/* Read P_MAX from NV and increment it for correct Srxlev calculation based on 
   * UE power class. Currently NV is set to 23 dBm but we should use 24dBm for 
   * power class 3
   */
#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
#define SRCHCRUTILS_GET_MAX_TX_POWER_NV(cell_ptr)\
    wl1_rfm_get_max_tx_power_in_rf_sleep(wl1_trm_get_rf_path(WCDMA_TRM, wsub_id), \
                                         RFCOM_TXPLIM_RACH, \
                                         wl1_rfm_convert_chan_to_band(cell_ptr->freq), \
                                         wsub_id) + SRCHCR_NV_MAX_TX_PWR_INCREMENT;
#else
#define SRCHCRUTILS_GET_MAX_TX_POWER_NV(cell_ptr)\
    wl1_rfm_get_max_tx_power_in_rf_sleep(wl1_trm_get_primary_rf_device(wsub_id), \
                                         RFCOM_TXPLIM_RACH, \
                                         wl1_rfm_convert_chan_to_band(cell_ptr->freq), \
                                         wsub_id) + SRCHCR_NV_MAX_TX_PWR_INCREMENT;
#endif

#ifdef FEATURE_FEMTO_CSG
#define IS_CSG_EVALS_PENDING() ((!(wsrch_ext_cntrl_params[wsub_id].srch_aset_parms.is_camped_cell_femto)) && (!(srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_csg_info.is_csg_meas_req_processed)) && ((srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_csg_info.num_tot_asf_cells) != 0))
#define IS_CSG_SRCH_PENDING() ((IS_CSG_EVALS_PENDING()) && ((srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_csg_info.csg_meas_pending) != 0))
#endif

extern void srchcr_ext_cntrl_params_init( wsub_id_e_type wsub_id);

extern void srchcr_int_cntrl_params_init(void);


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*============================================================================
FUNCTION: SRCHCRUTILS_CLEAR_PEND_BSIC_VERIFY_G2W_TA_EXIT

DESCRIPTION:

The function will be called at G2W TA exit to clear out the pending BSIC verification
as there might not be a new gap opened since we are exiting G2W TA
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcrutils_clear_pend_bsic_verify_g2w_ta_exit(wsub_id_e_type wsub_id);

extern uint16 get_time_from_cfn_in_ms(tlm_cfn_struct_type cfn);
extern uint16 get_diff_time_from_cfn_in_ms(tlm_cfn_struct_type cfn, tlm_cfn_struct_type substract_by_cfn);

/*============================================================================
FUNCTION: SRCHCR_BYPASS_NGBR_SRCH_OPTS

DESCRIPTION:
  If the energy of the cell is less than threshold value(-25), a 15s timer is
  decremented and when the timer expires the cell is removed from the monitored
  set.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/

extern boolean srchcrutils_bypass_ngbr_srch_opts(srchcr_freq_idx_enum_type freq_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCR_CHECK_ACQ_CELL_BARRED

DESCRIPTION
  This function prevent acquistion on a barred cell
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE if cell is barred.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean srchcrutils_check_acq_cell_barred 
(
   l1_acq_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCR_CHECK_CELL_BARRED

DESCRIPTION
  This function prevent check whether a fdd cell is barred.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE if cell is barred.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srchcrutils_check_cell_barred
(
  uint16 freq,
  uint16 scr_code
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_CHECK_GSM_CELL_BARRED

DESCRIPTION
    This function checks if a GSM cell is barred or not.
    Call when adding a cell to gsm nbr set.
DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the cell corresponding to the input is barred.
  FALSE otherwise

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchcrutils_check_gsm_cell_barred ( srch_gsm_cell_struct_type*   cell_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION:  SRCHCR_CHECK_INTERF_NBR_BARRED

DESCRIPTION: This function checks if the WCDMA inter frequency is barred  
  
DEPENDENCIES
  None.

RETURN VALUE
 	TRUE - If the inter frequency is barred
 	FALSE - If the inter frequency is not barred 

SIDE EFFECTS
  None
===========================================================================*/
extern boolean srchcrutils_check_interf_nbr_barred(srchcr_inter_f_idx_enum_type freq_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     srchcrutils_chk_interf_resel_pending

DESCRIPTION
  This function checks if an inter frequency reselection is pending 
  when UE is in DRX state

DEPENDENCIES
  None.

RETURN VALUE
   TRUE -- If resel is pending when UE is in DRX state

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchcrutils_chk_interf_resel_pending( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_FULL_SRCH_CONDITION_MET

DESCRIPTION
  This function checks if a full search can be performed or not in Idle mode.
  Full search can be done only if serving cell AND monitored cells.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if full search can be performed,
  FALSE, otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchcrutils_full_search_condition_met( uint16 time_available , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      srchcrutils_get_max_tx_power

DESCRIPTION
  This function returns the max transmission power of the cell whose frequency
  is passed as argument. The result is dependent on the power class of the band 
  under which the cells falls.

DEPENDENCIES
  None.

RETURN VALUE
  max transmission power of the cell 

SIDE EFFECTS
  None.
===========================================================================*/
extern uint8  srchcrutils_get_max_tx_power(W_ARFCN_T frequency, wsub_id_e_type wsub_id);

/*======================================================================
FUNCTION SRCHCR_GET_NUM_FRAMES_SINCE_PREV_EVAL

DESCRIPTION
  Function to get num frames since previous eval

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
=======================================================================*/
extern uint16 srchcrutils_get_num_frames_since_prev_eval( wsub_id_e_type wsub_id);

/*======================================================================
FUNCTION SRCHCR_GET_WEAKEST_USET_IDX

DESCRIPTION
  Returns weakest + non target uset cell

DEPENDENCIES
  None

RETURN VALUE
    value of the index. 0xff, if none is aavailable

SIDE EFFECTS
  
=======================================================================*/
extern uint16 srchcrutils_get_weakest_uset_idx ( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCHCR_ISS_ON_NBR_FREQ

DESCRIPTION
  This function checks if a inter-frequency reselection is in progress and
  nbch is already up for it

DEPENDENCIES
  None.

RETURN VALUE
  True  - if condition met
  False - otherwise

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchcrutils_iss_on_nbr_freq( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_NGBR_SRCH_OPT_LOG_PKT_FILL_AND_SUBMIT

DESCRIPTION:
  This function populates the parameters for neighbor srch optimization log packet 
Log packet sample below:
2013 Nov 13  00:30:27.765  [127/0x7F]  0x41B3  WCDMA Neighbor Search Optimization
Version               = 1
Num Freq              = 3
Quan Type             = EC_NO
Detmargin EcIo        = 3
Detmargin RSCP        = 0
Prediction Margin     = 5
Mobility State        = Mobile
Std Deviation Value   = NA
Serving Cell Min EcIo = NA
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|     |        |       |      |        |        |      |        |      |      |      |      |Rank  |Rank  |               |                |                   |                   |                |                |              |Step1     |
|WCDMA|        |       |      |        |        |Cand  |        |Squal |Srxlev|      |      |Cand  |Cand  |Cond 3:        |Cond 4:         |Cond 5:            |Cond 6:            |Cond 7:         |Cond 8:         |              |Energy    |
|RF   |        |Qualmin|Qrxlev|Qoffset1|Qoffset2|Nbr   |Cand Nbr|Cand  |Cand  |Rank_S|Rank_S|Nbr   |Nbr   |Squal_cand_ngbr|Srxlev_cand_ngbr|ECIO_Rank_cand_ngbr|Rscp_Rank_cand_ngbr|Srxlev_cand_ngbr|Srxlev_cand_ngbr|              |Thresh    |
|Freq |Priority|Min FI |Min FI|Min FI  |Min FI  |EcIo  |RSCP    |Nbr   |Nbr   |EcIo  |RSCP  |EcIo  |RSCP  |< 0            |< 0             |< ECIO_rank_serv   |< Rscp_rank_serv   |< threshx_high  |< threshx_low   |Skip Full Srch|Linear    |
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|10725|      NA|  32767| 32767|   32767|   32767|  -0.0|     0.0|     2|    50|     0|   -66|     2|-32767|          false|            true|               true|              false|              NA|              NA|           Yes|        NA|
|10837|      NA|  32767| 32767|   32767|   32767|  -0.0|     0.0|     2|    50|     0|   -66|     2|-32767|          false|            true|               true|              false|              NA|              NA|           Yes|        NA|
|10700|      NA|    -20|  -115|       0|       0|  -1.0|   -67.0|    38|    94|     0|   -66|    -2|   -67|          false|           false|               true|              false|              NA|              NA|           Yes|        NA|
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcrutils_ngbr_srch_opt_log_pkt_fill_and_submit( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_OUT_OF_SERVICE_AREA_CHECK 

DESCRIPTION
  This function is called anytime the serving cell fails the S criteria.
  If the serving cell fails consistently for 12s during PCH or for 4s
  during FACH, this function returns a boolean indicating whether an
  out of service area indication needs to be sent to RRC.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if out of service area indication has to be sent.
  FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srchcrutils_out_of_service_area_check( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_RANK_LOG_PKT_SUBMIT

DESCRIPTION
  This function compresses the cell reselection rank log packet and submits
  it to loging.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcrutils_rank_log_pkt_submit( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      srchcrutils_fill_idle_meas_agc_values

DESCRIPTION
  This function fills the parameters for the wcdma idle meas log packet (0x41CF).

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcrutils_fill_idle_meas_agc_values(srchcr_res_eval_meas_type srch_type, WCDMA_IDLE_MEAS_LOG_PACKET_type* wcdma_idle_meas_log_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      srchcrutils_idle_meas_log_pkt_submit

DESCRIPTION
  This function submits the wcdma idle meas log packet (0x41CF).

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcrutils_idle_meas_log_pkt_submit( wsub_id_e_type wsub_id);


/*======================================================================
FUNCTION SRCHCR_START_CR_TIMER

DESCRIPTION
  Start rex timer for cell reselection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  
=======================================================================*/
extern void srchcrutils_start_CR_timer( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCHCR_RESP_EARLY_RF_SLEEP

DESCRIPTION
  This function checks for any pending srch or on going searches, and returns a boolean

DEPENDENCIES
  None.

RETURN VALUE
  True  - No Pending or ongng searches
  False - otherwise

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcrutils_resp_early_rf_sleep( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_COMPUTE_SPARAMS

DESCRIPTION:
  Compute the S critiera quantities and updates them in the cell_ptr passed 
      
DEPENDENCIES:
  None.

RETURN VALUE:
NONE

SIDE EFFECTS:
NONE
=============================================================================*/
extern void srchcrutils_compute_sparams(srchcr_cell_type_enum_type cell_type, void* cell_ptr_parm,int16 q_qualmin,int16 q_rxlevmin, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_GET_CURR_MEAS_RULES

DESCRIPTION:
  This function is called to query the current measurement rules variable.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None. 

SIDE EFFECTS:
=============================================================================*/
void srchcrutils_get_curr_meas_rules(uint8 *prev_meas_rules, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_GET_OFREQ_PRIO

DESCRIPTION:
  This function returns the Priority of the frequency
      
DEPENDENCIES:
  None.

RETURN VALUE:
 Priority value - if frequency has priority configured
 -1 - otherwise

SIDE EFFECTS:
NONE
=============================================================================*/
extern int8 srchcrutils_get_ofreq_prio(uint16 freq, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_GET_PREV_MEAS_RULES

DESCRIPTION:
  This function is called to query the previous measurement rules variable.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None. 

SIDE EFFECTS:
=============================================================================*/
void srchcrutils_get_prev_meas_rules(uint8 *prev_meas_rules, wsub_id_e_type wsub_id);

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
srchcr_cell_type_enum_type srchcrutils_is_resel_in_progress( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_PANIC_MODE

DESCRIPTION: This function is called to check for panic conditions. 
Serving cell barred
S criteria fails > Nserv or 4 (for edrx case)
      
DEPENDENCIES:
None.

RETURN VALUE:
Boolean
TRUE - PANIC mode
FALSE - otherwise

SIDE EFFECTS:
=============================================================================*/
extern boolean srchcrutils_panic_mode(uint8 nserv, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_UPDATE_TRESEL_PARAMS

DESCRIPTION:
  Starts/Updates Tresel timers 
      
DEPENDENCIES:
  None.

RETURN VALUE:
NONE

SIDE EFFECTS:
NONE
=============================================================================*/
extern void srchcrutils_update_tresel_params(srchcr_res_eval_meas_type cell_type, srch_evt_ts_struct_type* cell_evt_cr_ts, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_RESET_TRESEL

DESCRIPTION:
  Resets Tresel timer
      
DEPENDENCIES:
  None.

RETURN VALUE:
NONE

SIDE EFFECTS:
NONE
=============================================================================*/
extern void srchcrutils_reset_tresel(srch_evt_ts_struct_type* cell_evt_cr_ts, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION        srchcrutils_reset_tresel_ngbrs

DESCRIPTION      Resets the Tresel on nbrs based on cell_type passed

DEPENDENCIES    None

RETURN VALUE    None
  
SIDE EFFECTS    None 
                
===========================================================================*/
extern void srchcrutils_reset_tresel_all_ngbrs(srchcr_cell_type_enum_type cell_type, wsub_id_e_type wsub_id);

 /*============================================================================
FUNCTION: SRCHCRUTILS_POST_RESEL_EVENT

DESCRIPTION: This function is used to post an event containing cell 
             information when a reselection procedure starts. 
      
DEPENDENCIES:
None.

RETURN VALUE:
None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcrutils_post_resel_start_event(uint16 uarfcn, uint16 target_psc, wsub_id_e_type wsub_id);


#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY

/*===========================================================================

FUNCTION      srchcrutils_abs_prio_log_pkt_submit

DESCRIPTION
  This function submits the absolute priority cell reselection log packet (0x4027).

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcrutils_abs_prio_log_pkt_submit( wsub_id_e_type wsub_id);

/* ================================================================================= 
FUNCTION       srchcrutils_calculate_tresel_criteria_for_abs_prio 

DESCRIPTION
  This function is used only for REL8 UE's when Absolute Priotity 
  is used for resel eval. 

  This fn calculates the corresponding criteria based on the priority level used. 

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.
=================================================================================== */
boolean srchcrutils_calculate_tresel_criteria_for_abs_prio (srchcr_priority_enum_type priority_level, 
                                                       int16 nbr_s_rxlev, 
                                                       uint8 nbr_threshx_high, 
                                                       uint8 nbr_threshx_low, 
                                                       srchcr_cell_type_enum_type cell_type, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION       SRCHCR_CHECK_HIGH_PRIO)RSSI_TMEASGSM_SRCH_DUE

DESCRIPTION
  

DEPENDENCIES
  None.

INPUTS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean srchcrutils_check_high_prio_rssi_tmeasgsm_srch_due( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCHCR_CHECK_IF_NBR_SRCHED_THIS_DRX

DESCRIPTION
  This function checks if search type specified by the input param
  was done this drx cycle

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If they were done
  FALSE - If not done

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchcrutils_check_if_nbr_srched_this_drx(srchcr_cell_type_enum_type cell_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION       srchcrutils_find_cell_prio ()

DESCRIPTION
  This function is used only for REL8 UE's when Absolute Priotity 
  is used for resel eval. 

  This function is called from srchcr_find_best_ranked_abs_prio(), and returns the
  cell's priority based on the cell_type.

DEPENDENCIES
  None.

RETURN VALUE
  cell_priority.

SIDE EFFECTS
  None.
===========================================================================*/
extern int8 srchcrutils_find_cell_prio(srchcr_rank_cell_struct_type *cell_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION       srchcrutils_print_srched_cells ()

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcrutils_print_srched_cells( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_PRIOR_INFO_GIVEN_FOR_G_ARFCN

DESCRIPTION:
  This fn checks if a given garfcn is already present in gsm priority info sent 
  as a part of 3rd_MEAS_REQ 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:   
  None. 
=============================================================================*/

extern boolean srchcrutils_prior_info_given_for_g_arfcn(int8 idx,l1_meas_priority_info_type *prior_info,uint32 *prior_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCRUTILS_COMPUTE_SCHEDULING_MODE

DESCRIPTION
  This function checks the following for the serving cell and returns the scheduling
  mode to be used in the current drx cycle
 
  if (srxlev > s_prioritysearch1) && (Squal > s_prioritysearch2)
    Use High Prior Scheduling
  else
    Use All Prior Scheduling


DEPENDENCIES
  None.

INPUTS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern srchcr_prior_scheduler_mode_enum_type srchcrutils_compute_scheduling_mode( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_COMPUTE_W_G_MEAS_RULES_FOR_HIGH_PRIOR_SCHEDULING

DESCRIPTION
  

DEPENDENCIES
  None.

INPUTS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
srchcr_meas_rules_type srchcrutils_compute_w_g_meas_rules_for_high_prior_scheduling( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_COMPUTE_W_G_MEAS_RULES_FOR_ALL_PRIOR_SCHEDULING

DESCRIPTION
  

DEPENDENCIES
  None.

INPUTS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

srchcr_meas_rules_type srchcrutils_compute_w_g_meas_rules_for_all_prior_scheduling( wsub_id_e_type wsub_id);

#endif

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH

/*============================================================================
FUNCTION: SRCHCRUTILS_FIND_HIGHEST_PRIORITY_RAT

DESCRIPTION:
  This function is called to query the current and previous measurement rules variables.
  For non-priority cases, the values are initialized to INT8_MIN's. Also this function will
  only be called when the change in meas rules is exclusive. For eg. MeasRule = LW --> G Only.
  The routine covers the highest priority update with the measurement rules so we will return
  what is included in meas rules only. 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  Return the high priority RAT from current meas rules bitmask. 

SIDE EFFECTS:
=============================================================================*/
extern srchfach_meas_oksn_sys_enum_type srchcrutils_find_highest_priority_rat(uint8 cur_meas_bmsk, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_GET_OFREQ_PRIO_INFO

DESCRIPTION:
  This function returns the Priority Info pointer for the requested frequency
      
DEPENDENCIES:
  None.

RETURN VALUE:
 PriorityInfo pointer - if frequency has priority configured
 NULL - otherwise

SIDE EFFECTS:
NONE
=============================================================================*/
extern srch_inter_freq_priority_info_type* srchcrutils_get_ofreq_prio_info(uint16 freq, wsub_id_e_type wsub_id);

#endif

#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
/* ================================================================================= 
FUNCTION       srchcrutils_calculate_tresel_criteria_4_5_for_abs_prio 

DESCRIPTION
  This function is used only for REL8 UE's when Absolute Priotity 
  is used for resel eval. 

  This fn calculates the corresponding criteria based on the priority level used. 
   

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.
=================================================================================== */
boolean srchcrutils_calculate_tresel_criteria_4_5_for_abs_prio (srchcr_priority_enum_type priority_level, 
                                                           srchcr_cell_type_enum_type cell_type,
 						                                               int16 nbr_s_qual, 
 						                                               uint8 nbr_threshx_high2, 
 						                                               uint8 nbr_threshx_low2 , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION:  SRCHCR_CHECK_LTE_FREQ_BARRED

DESCRIPTION: This function checks if the LTE frequency is barred 
 
DEPENDENCIES
  None.

RETURN VALUE
 	TRUE - If the LTE frequency is barred
 	FALSE - If the LTE frequency is not barred 

SIDE EFFECTS
  None
===========================================================================*/
extern boolean srchcrutils_check_lte_freq_barred(uint32 earfcn, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: srchcrutils_is_lte_cell_blacklisted

DESCRIPTION:
  This function checks whether the particular set of LTE EARFCN and Physical Cell ID is 
   blacklsited or not.

   Have this function as generic, to be used outside CSG if needed
  
DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE: Blacklisted
  FALSE:Not Blacklisted

SIDE EFFECTS:
  None

=============================================================================*/
extern boolean srchcrutils_is_lte_cell_blacklisted(uint32 earfcn, uint16 phy_cell_id, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_IS_LTE_TRESEL_RUNNING

DESCRIPTION: This function checks if any LTE frequencies have Tresel started and returns a bitmask 
containing the indices of the frequencies which have Tresel active
      
DEPENDENCIES:
None.

RETURN VALUE:
UINT8 - Bitmask containing the frequencies with active Tresel on their cell[s]

SIDE EFFECTS:
=============================================================================*/
uint8 srchcrutils_is_lte_tresel_running( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCHCRUTILS_IS_TRESEL_RUNNING_FOR_ANY_LTE_CELLS

DESCRIPTION
  This function checks whether Tresel is running for any detected LTE cells 
  across all LTE EARFCNs.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - if Tresel is running on any LTE cell
  FALSE - Otherwise

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchcrutils_is_tresel_running_for_any_lte_cells( wsub_id_e_type wsub_id);


#endif


#ifdef FEATURE_FEMTO_CSG

/*============================================================================
FUNCTION: SRCHCR_ALLOCATE_CSG_MEMORY()

DESCRIPTION:
  This function allocates memory for intra and interf CSG cells
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
void srchcrutils_allocate_csg_memory(wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: srchcrutils_csg_asf_meas_req_log_pkt_submit()

DESCRIPTION:
  This function logs ASF measurement request parameters 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcrutils_csg_asf_meas_req_log_pkt_submit( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: srchcrutils_csg_cell_reselection_log_pkt_add_intra_cells()

DESCRIPTION:
  This function populates intra frequency cells for CSG Cell Reselection log packet 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcrutils_csg_cell_reselection_log_pkt_add_intra_cells( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: srchcrutils_csg_cell_reselection_log_pkt_add_inter_cells()

DESCRIPTION:
  This function populates inter-freq cells measured for CSG Cell Reselection log packet 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcrutils_csg_cell_reselection_log_pkt_add_inter_cells(srch_interf_cell_struct_type *cell_ptr, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: srchcrutils_csg_cell_reselection_log_pkt_submit()

DESCRIPTION:
  This function populates and submits the CSG Cell Reselection log packet 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcrutils_csg_cell_reselection_log_pkt_submit( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_DEALLOCATE_CSG_MEMORY()

DESCRIPTION:
  This function de-allocates memory for intra and interf CSG cells
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcrutils_deallocate_csg_memory( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_FIND_INTRA_CELL_IN_SRCHED_LIST()

DESCRIPTION:
  This function finds the intra cell that was searched in this DRX cycle based
  on the Freq, psc that is passed. 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - if the cell is found.
  FALSE - otherwise. 

SIDE EFFECTS:
=============================================================================*/
boolean srchcrutils_find_intra_cell_in_srched_list(uint16 freq, uint16 scr_code, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: srchcrutils_get_csg_id()

DESCRIPTION:
  This function 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  Retuns the csg_id mapping to freq, psc that is passed.

SIDE EFFECTS:
=============================================================================*/
extern uint32 srchcrutils_get_csg_id(uint16 freq, uint16 psc, srchcr_cell_type_enum_type cell_type, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_GET_CSG_INTERF_INDEX()

DESCRIPTION:
  This function 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  Retuns the index in the inter-f array corresponding to freq, psc that is passed.

SIDE EFFECTS:
=============================================================================*/
extern uint8 srchcrutils_get_csg_interf_index(uint16 freq, uint16 scr_code , wsub_id_e_type wsub_id);

#endif

#ifdef FEATURE_WCDMA_HS_RACH
/*===========================================================================

FUNCTION     SRCHCR_ALLOW_HS_RACH

DESCRIPTION 
  For cell reselections that are spanning across states PCH and FACH:On entering FACH state,
  the HS RACH controller will call a SRCHCR API to find if a cell reselection in ongoing from PCH state.
  If that is the case, then HS RACH controller will ensure that RACH procedure does not get started.
  The RACH Abort state machine will ensure this.
  Only if this function returns true, UL can start RACH Tx.
 
DEPENDENCIES
  None.

RETURN VALUE
  True  - If no reselections or Intraf resel is ongoing.
  False - otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean srchcrutils_allow_hs_rach( wsub_id_e_type wsub_id);

#endif

#ifdef FEATURE_WCDMA_UNBAR_OPRTR_RSVD_FREQ
/*============================================================================
FUNCTION: SRCHCR_CHECK_STEP3_BEST_PILOT_BARRED_CELL

DESCRIPTION: This function is called to check if the best pilot
detected in ACQ step3 is part of the barred cell list or not. If the
best pilot is barred then we cannot camp on the cell. If the best pilot
is not the barred cell, we can camp on the freq even though bar
type was freq barred

      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern boolean srchcrutils_check_nastt_best_pilot_barred_cell( wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_DUAL_SIM

extern boolean srchcrutils_chk_srches_needed_in_a_gap(wsub_id_e_type wsub_id);

#endif

/*---------------------------------------------------------------------------
                            SRCHCR FUNCTIONS
---------------------------------------------------------------------------*/

/*============================================================================
FUNCTION: SRCHCR_POST_OOS_IND_TO_RRC

DESCRIPTION: This function is used to post RRC_CPHY_OUT_OF_SERVICE_AREA_IND to RRC
      
DEPENDENCIES:
None.

RETURN VALUE:
None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_post_oos_ind_to_rrc(wl1_common_fail_enum_type oos_cause, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_CLEANUP_NBR_DB

DESCRIPTION: This function is called to clear all Interf/Intraf cells counts that srchcr maintains locally.
This is useful when srchset is transitioning cells to SET_E or during ASET UPD where we clear all InterF
cells as part of resel success
      
DEPENDENCIES:
None.

RETURN VALUE:
NONE

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_cleanup_nbr_db(srchcr_cell_type_enum_type cell_type, boolean clear_srched_cells, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_RESET_RESEL_POINTERS

DESCRIPTION: This function is clears intraf,interf,gsm & LTE cell pointers 
when srchset_db clears the original pointers.
      
DEPENDENCIES:
None.

RETURN VALUE:
NONE

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_reset_resel_pointers(srchcr_cell_type_enum_type cell_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     SRCHCR_RESP_EARLY_RF_SLEEP_DECISION

DESCRIPTION
  This function returns the early RF sleep decision already evaluated.

DEPENDENCIES
  None.

RETURN VALUE
  True  - No Pending or ongng searches
  False - otherwise

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchcr_resp_early_rf_sleep_decision( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION        SRCHCR_CLEANUP

DESCRIPTION
  This function resets the variables related to cell reselection. This is
  called while suspending or ending cell reselection evaluation. DRX is
  notified if it is waiting on search ready for sleep.

DEPENDENCIES
  This function should be called in L1M task context.

RETURN VALUE
  None.

SIDE EFFECTS
  ext_srchcr_cntrl->srch_cell_reselect_enabled is set to FALSE
  and other search related variables are reset.

===========================================================================*/
void srchcr_cleanup( l1m_state_enum_type next_state , wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION       SRCHCR_SORT_ENTIRE_NSET

DESCRIPTION
  This function sorts all the neighbor cells in the SYNC_NSET according
  to increasing order of their energies.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_sort_entire_nset( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION       SRCHCR_INIT_GSM_NSET

DESCRIPTION
  This function initializes gsm nset data 
  
DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
extern void srchcr_init_gsm_nset( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION        SRCHCR_INIT_INTRA_FULL_SRCH_SFN_CTR

DESCRIPTION
  This function initializes intra freq full search sfn counter

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Intra freq full search may get started after next aset list search

===========================================================================*/
extern void srchcr_init_intra_full_srch_sfn_ctr( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION       SRCHCR_SORT_ENTIRE_INTER_F_NSET

DESCRIPTION
  This function sorts the entire inter-frequency neighbor list
  according to their energies.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void srchcr_sort_entire_inter_f_nset( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION        SRCHCR_INIT_INTER_FULL_SRCH_SFN_CTR

DESCRIPTION
  This function initializes inter freq full search sfn counter

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Inter freq full search may get started after next aset list search

===========================================================================*/
extern void srchcr_init_inter_full_srch_sfn_ctr( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_SEND_DL_WEAK_IND_TO_RRC

DESCRIPTION
  This function sends a CEL RESEL IND to RRC for the PSC passed in

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  CEL RESEL IND sent to RRC
  
===========================================================================*/
extern void srchcr_send_dl_weak_ind_to_rrc( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION       SRCHCR_ADD_CELL_TO_SRCHED_LIST

DESCRIPTION
  This function adds the specified cell to the list of neighbors that 
  were searched during this cell reselection search cycle.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchcr_add_cell_to_srched_list
(
  /* Type of cell to be added */
  srchcr_cell_type_enum_type    type,
  /* Ptr to Intra-F cell      */
  srch_cell_struct_type        *wcdma_intra_ptr,
  /* Ptr to Inter-F cell      */
  srch_interf_cell_struct_type *wcdma_inter_ptr,
  /* Ptr to GSM cell */
  srch_gsm_cell_struct_type    *gsm_ptr
#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  ,wsrchlte_cell_struct_type *lte_ptr
#endif
, wsub_id_e_type wsub_id);

/*======================================================================
FUNCTION SRCHCR_BACKUP_NEIGH_CELL_PENALTY_TIMER

DESCRIPTION
    Backups the penalty timers for all neighbor cells

RETURN VALUE
    void

SIDE EFFECTS
    backup store is filled in
=======================================================================*/
extern void srchcr_backup_nbr_penalty_timer( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_CHECK_FAST_MOVING_CONDITION

DESCRIPTION
  This function checks if the UE is in fast moving condition

DEPENDENCIES
  Can ONLY be called if HCS is enabled and immediately after a 
  CELL_PARM_REQ (SIB3) or a CELL_MEAS_REQ (SIB11/12)

RETURN VALUE
  None.

SIDE EFFECTS
  UE fast moving condition updated
  
===========================================================================*/
extern void srchcr_check_fast_moving_condition( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_CHK_N_SRCH_CMD

DESCRIPTION
  This function will start an inter-frequency or an inter-RAT search if
  one is required, otherwise any search results will be processed to 
  determine if a cell reselection indication should be sent to RRC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_chk_n_srch_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     SRCHCR_CLR_EARLY_RF_SLEEP_DECISION

DESCRIPTION
  This function clears the early RF sleep decision already evaluated.

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_clr_early_rf_sleep_decision( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     srchcr_enable_heartbeat_aft_decfg_done

DESCRIPTION
  This is a CB function which enabled heartbeat if needed soon after  
  ngbr cell is deconfigured as part of cell resel failure

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_enable_heartbeat_aft_decfg_done( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_FIND_MIN_OF_PARAMS

DESCRIPTION:
  This function finds the minimum of the cell parameters(q_offset1, q_offset_2
  q_rxlevmin and q_qualmin) in a frequency.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_find_min_of_params(srchcr_freq_idx_enum_type freq_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_GSM_NSET_SRCH_CMD

DESCRIPTION
  This function processes a SRCHCR_GSM_NSET_SRCH_CMD

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void  srchcr_gsm_nset_srch_cmd
(
  srchcrgsm_local_cmd_type* cmd_ptr
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        SRCHCR_INIT

DESCRIPTION
  This function initializes this module to start cell reselection
  evaluation. This is called 1 second after we have camped on a cell.
  Currently we enable cell reselection 1 second after we receive
  the DRX command.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  srch_intra_cell_reselect_enabled is set to TRUE.
  srch_ignore_heartbeat_evt is set to TRUE.
  All the NSET neighbor cell cell reselection timers are initialized.

===========================================================================*/
extern void srchcr_init( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_INIT_ON_POWERUP

DESCRIPTION
  This function initializes cell reselection related data structures on 
  power up

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Cell reselection related globals initialized
  
===========================================================================*/
extern void srchcr_init_on_powerup( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_L1_SRCHCR_RESUME_IDLE_SEARCH_HANDLER

DESCRIPTION:
  TODO
      
DEPENDENCIES:
  TODO

RETURN VALUE:
  TODO

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_l1_srchcr_resume_idle_search_handler( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_REMOVE_CELL_FROM_SRCHED_LIST

DESCRIPTION
  This function removes specified cell from the list of neighbors that 
  were searched during this cell reselection search cycle.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchcr_remove_cell_from_srched_list
(
  /* pointer to the cell structure */
  void *cell_ptr
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        SRCHCR_RESET

DESCRIPTION
  This function resets the variables related to cell reselection. This is
  called while suspending or ending cell reselection evaluation. It is also
  called from drx_restart(), which is called on cell reselection failure.
  This gets DRX and cell reselection searches started up cleanly again.

DEPENDENCIES
  This function should be called in L1M task context.

RETURN VALUE
  None.

SIDE EFFECTS
  ext_srchcr_cntrl->srch_cell_reselect_enabled is set to FALSE
  and other search related variables are reset.

===========================================================================*/
extern void srchcr_reset( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_RESET_OOS_TIMER

DESCRIPTION
  This function resets the OOS timer and clears the signal and other
  related flags.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchcr_reset_oos_timer( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_RESET_RFTUNE_AND_AGC_STATE

DESCRIPTION
  Updates agc status post w2x

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_reset_rftune_and_agc_state(boolean skip_if_interf_resel, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_SET_FLAG_TO_SORT_ENTIRE_INTERF_NSET_AFTER_FULL_SRCH

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_set_flag_to_sort_entire_interf_nset_after_full_srch(boolean sort, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        SRCHCR_SET_OUT_OF_SERVICE_TIMER_FLAG

DESCRIPTION
  This function sets the ext_srchcr_cntrl->srchcr_out_of_service_timer_expired flag when
  the out of service timer expires.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  ext_srchcr_cntrl->srchcr_out_of_service_timer_expired flag is set.

===========================================================================*/
extern void srchcr_set_out_of_service_timer_flag( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_SIB_MOD_UPDATE

DESCRIPTION
  This function performs SIB modification update if they are pending.
  In case a cphy setup is in progress, this will not clear the SRCH
  and DL module cleanup done flags, since cphy setup request has to
  continue. This function is called when either DL or SRCHFACH module
  completes cleaning up the current searches.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_sib_mod_update( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      srchcr_start_gsm_srch_on_gsm_clk_en

DESCRIPTION
  This function will start an inter-RAT search once Tx System Time and
  GSM clk gets enabled for GSM seacrhes.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_start_gsm_srch_on_gsm_clk_en( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_TUNE_RF

DESCRIPTION
  This function tunes RF to the specified frequency.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_tune_rf( uint16 freq , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_UPDATE_CELL_RES_TIME

DESCRIPTION
  This function udpate the cell reselection time in the res_time data str

DEPENDENCIES
  Can ONLY be called after a successful cell reselection

RETURN VALUE
  None.

SIDE EFFECTS
  res_time array is updated
  
===========================================================================*/
extern void srchcr_update_cell_res_time( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_UPDATE_NLISTS

DESCRIPTION
  This function will set the flags so the sorted neighbor lists will be 
  updated correctly during the next reselection pass.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_update_nlists( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY

/*============================================================================
FUNCTION: srchcr_init_prior_info_for_all_rats()

DESCRIPTION:
  This fn initialize the global to indicate abscense of priority for all          
  RATs. This info is received in 3rd MEAS_REQ from RRC. We assume that         
  no priorities are configured in the system until the 3rd MEAS_REQ         
  containing prior info is received.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_init_prior_info_for_all_rats( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: srchcr_update_priority_and_lte_info()

DESCRIPTION:
  This fn processes the 3rd MEAS_REQ to update the priority information
  for all the RATs.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_update_priority_and_lte_info(
  l1_meas_priority_eutra_cell_list_info_type *info, wsub_id_e_type wsub_id);


#endif

#ifdef FEATURE_IGNORE_TRESEL_ON_S_FAIL
/*===========================================================================

FUNCTION       SRCHCR_CHECK_WEAK_RESEL_CONDITION

DESCRIPTION
  This function check if weak resel condition happened or not and update the flag accordingly.
  weak resel condition is when NCR=10 opt reselections (due to s-fail; bad agc; tresel short
  has been attempted in less than TCR = 60s. 
  This function should be called after receiving TRANSITON_REQ when transitioning to a new cell.
  This function is also called when there is a transition failure. It is based on the number of weak reselections
  attempted.
DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
extern void srchcr_check_weak_resel_condition( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_UPDATE_WEAK_RESEL_CONDITION

DESCRIPTION
  This function checks if weak resel condtion's end time has been reached. If so update reset the weak resel flag.
DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
extern void srchcr_update_weak_resel_times( wsub_id_e_type wsub_id);
#endif


#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)

/*===========================================================================

FUNCTION       SRCHCR_START_LTE_SRCH_ON_INIT_CNF

DESCRIPTION
  This function calls the scheduler when LTE INIT CNF message is rcvd, so 
  that LTE search can now be scheduled

DEPENDENCIES
  None.

INPUTS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srchcr_start_lte_srch_on_init_cnf( wsub_id_e_type wsub_id);
/*============================================================================
FUNCTION: SRCHCR_LTE_SRCH_OCLENAUP_ON_CELL_BAR

DESCRIPTION:
  This function gets called cleanup RF bufs & IRAT state mc when RF bufs are 
  allocated for lte srch but srch cannot go through as LTE cell is barred.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_lte_srch_cleanup_on_cell_bar(srchcr_res_eval_meas_type srch_to_be_performed, wsub_id_e_type wsub_id);

#endif

#ifdef FEATURE_WCDMA_HS_RACH

/*===========================================================================
FUNCTION     SRCHCR_UNBLOCK_CELL_RESEL

DESCRIPTION 
  This funciton is called by HSRACH module to allow reselections.
 
  Once the EDCH resource is released, the HS RACH controller will call a cell reselection API to
  unblock the cell reselections. The design is to do a immediate resel evals and restart the 80ms eval timer.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_unblock_cell_resel( wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_HS_RACH */

#ifdef FEATURE_REL5  
/*===========================================================================

FUNCTION       SRCHCR_SCALE_TRESEL

DESCRIPTION
   This funciton scales up/down tresel values for intra, inter and inter-rat cells depending on
   whether UE is in high mobility or not and based on the different scale factors.
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
extern void  srchcr_scale_tresel( wsub_id_e_type wsub_id);
#endif

/*===========================================================================

FUNCTION     SRCHCR_FORCE_EXPIRE_TRESEL_FOR_CAND_NGBR

DESCRIPTION
  This function is called after a band timeout for nbch add to force expire Tresel
  so that resel will be triggered in the next DRX cycle. 
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_force_expire_tresel_for_cand_ngbr(wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_REL7
/*===========================================================================

FUNCTION SRCHCR_UPD_OTA_PARM

DESCRIPTION
  This function updates mobility information, stored in ext_srchcr_cntrl->srchcr_ue_fast_moving 
  structure,  based on information conveyed by CPHY_UPD_OTA_PARMS_IND. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchcr_upd_ota_parm(l1_upd_ota_parms_ind_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_FEMTO_CSG
/*============================================================================
FUNCTION: SRCHSET_GET_INTRA_CSG_CELL_PTR()

DESCRIPTION:
  This function returns the CSG ptr corresponding to the freq,psc thats passed
      
DEPENDENCIES:
  None.

RETURN VALUE:
  Retuns the csg_id mapping to freq, psc that is passed.

SIDE EFFECTS:
=============================================================================*/
extern srch_cell_struct_type* srchset_get_intra_csg_cell_ptr(uint16 freq, uint16 psc, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHSET_GET_INTERF_CSG_CELL_PTR()

DESCRIPTION:
  This function returns the inter-f CSG ptr corresponding to the freq,psc thats passed
      
DEPENDENCIES:
  None.

RETURN VALUE:
  Retuns the csg_id mapping to freq, psc that is passed.

SIDE EFFECTS:
=============================================================================*/
extern srch_interf_cell_struct_type* srchset_get_interf_csg_cell_ptr(uint16 freq, uint16 psc, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: srchcr_update_csg_info()

DESCRIPTION:
  This fn processes the 4th CSG MEAS_REQ and updates the internal CSG database.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_update_csg_info(
  l1_meas_csg_cells_info_type *csg_meas_ptr, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_CLEAR_CSG_INFO()

DESCRIPTION:
  This function clears the CSG related params.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
 void srchcr_clear_csg_info(wsub_id_e_type wsub_id);

#endif

#ifdef FEATURE_WCDMA_REACQLESS_PICH

boolean srchcr_check_for_active_gsm_nbr_cells( wsub_id_e_type wsub_id);

#endif

#ifdef FEATURE_WCDMA_COEXISTENCE_SW
/*============================================================================
FUNCTION: SRCHCRUTILS_REPORT_INTERF_NBR_TO_CXM

DESCRIPTION: This function de/registers the IDLE mode InterF neighbors with 
             WL1CXM and thereby with MCS. It is called upon 2nd MEAS_REQ
             processing only in DRX state. In other states, CM module
             handles it.
      
DEPENDENCIES:
None.

RETURN VALUE:
None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_report_interf_nbr_to_cxm(boolean dereg, wsub_id_e_type wsub_id);
#endif

/*===========================================================================

FUNCTION     SRCHCRUTILS_RESET_IDLE_TIMERS

DESCRIPTION
  This function resets timers which are run for periodic searches. 
  This is called after reselecting to/selecting a cell and 
  corresponding sib is udpated.

DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None.
===========================================================================*/
void srchcrutils_reset_idle_timers(srchcr_res_eval_meas_type srch_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION: srchcrutils_check_srch_needed_for_lte_det_srch

DESCRIPTION:
  This function checks if LTE detect is needed and
  the master/gap bitmask corresponding to lte detect on earfcn index i will get reset if 
  (1) LTE det on that earfcn index  is not due
  (2) Check if lte cell is barred
  (3) if it has low prio in HIGH_PRIO_SCHEDULING_MODE
DEPENDENCIES
  None.

RETURN VALUE
  Return TRUE if LTE det needs to be done on certain earfcn

SIDE EFFECTS

===========================================================================*/
boolean srchcrutils_check_srch_needed_for_lte_det_srch(srchcr_res_eval_meas_type search_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION: srchcrutils_check_srch_needed_for_lte_meas_srch

DESCRIPTION:
  This function checks if LTE meas is needed 
  (1) if LTE meas on that earfcn index  is due
  (2) if LTE fast meas due but no Tresel running on that earfcn
  (3) if num_cells = 0 or DET is scheduled on the same earfcn
  (4) if it has low prio in HIGH_PRIO_SCHEDULING_MODE
DEPENDENCIES
  None.

RETURN VALUE
  Return TRUE if LTE meas needs to be done on certain earfcn

SIDE EFFECTS

===========================================================================*/
boolean srchcrutils_check_srch_needed_for_lte_meas_srch( srchcr_res_eval_meas_type search_type, wsub_id_e_type wsub_id);
/*---------------------------------------------------------------------------
                SRCHCRUTILS GET WRAPPER FUNCTIONS
---------------------------------------------------------------------------*/

uint8 srchcrutils_get_srch_cell_select_failed_drx_cycles( wsub_id_e_type wsub_id);

srch_cell_struct_type* srchcrutils_get_srch_cr_cell_ptr( wsub_id_e_type wsub_id);

uint32 srchcrutils_get_srchcr_barred_cell_elapsed_time( wsub_id_e_type wsub_id);

boolean srchcrutils_get_srchcr_cell_bar_ext_msg_recd( wsub_id_e_type wsub_id);

srchcr_meas_rules_type srchcrutils_get_srchcr_current_meas_rules( wsub_id_e_type wsub_id);

boolean srchcrutils_get_srchcr_drx_waiting_on_srch_sleep( wsub_id_e_type wsub_id);

srch_gsm_cell_struct_type* srchcrutils_get_srchcr_gsm_cell_ptr( wsub_id_e_type wsub_id);

boolean srchcrutils_get_srchcr_meas_rules_indicate_intra_meas(wsub_id_e_type wsub_id);

boolean srchcrutils_get_srchcr_meas_rules_indicate_inter_meas(wsub_id_e_type wsub_id);

boolean srchcrutils_get_srchcr_meas_rules_indicate_gsm_meas(wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
boolean srchcrutils_get_srchcr_meas_rules_indicate_lte_meas(wsub_id_e_type wsub_id);
#endif

void srchcrutils_set_gap_type(srchcr_res_eval_meas_type srch_type, gap_enum_type gap_type, wsub_id_e_type wsub_id);

boolean srchcrutils_get_intra_list_srch_due(wsub_id_e_type wsub_id);

boolean srchcrutils_get_intra_full_srch_due(wsub_id_e_type wsub_id);

boolean srchcrutils_get_inter_f0_full_srch_due(wsub_id_e_type wsub_id);

boolean srchcrutils_get_inter_f1_full_srch_due(wsub_id_e_type wsub_id);

boolean srchcrutils_get_inter_f0_list_srch_due(wsub_id_e_type wsub_id);

boolean srchcrutils_get_inter_f1_list_srch_due(wsub_id_e_type wsub_id);
#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
boolean srchcrutils_get_gsm_high_prio_rssi_srch_due(wsub_id_e_type wsub_id);
#endif

boolean srchcrutils_get_gsm_srch_due(wsub_id_e_type wsub_id);

boolean srchcrutils_get_gsm_rssi_srch_due(wsub_id_e_type wsub_id);

boolean srchcrutils_get_gsm_bsic_srch_due(wsub_id_e_type wsub_id);

boolean srchcrutils_get_lte_detect_srch_due(wsub_id_e_type wsub_id);

boolean srchcrutils_get_lte_meas_srch_due(wsub_id_e_type wsub_id);

boolean srchcrutils_get_lte_srch_due(wsub_id_e_type wsub_id, uint32 timer_bmask);

const uint32 srchcrutils_get_srch_idle_mode_filt_coeff_q10(uint8 idx);

const uint32 srchcrutils_get_srch_idle_mode_filt_coeff_ngbr_q10(uint8 idx);

boolean srchcrutils_get_srchcr_ignore_barred_freqs( wsub_id_e_type wsub_id);

srch_interf_cell_struct_type* srchcrutils_get_srchcr_inter_f_cell_ptr( wsub_id_e_type wsub_id);

srchcr_inter_f_idx_enum_type srchcrutils_get_srchcr_inter_f_idx( wsub_id_e_type wsub_id);

boolean srchcrutils_get_srchcr_inter_s_srch_in_progress( wsub_id_e_type wsub_id);

boolean srchcrutils_get_srchcr_pending_cell_resel( wsub_id_e_type wsub_id);

srchcr_meas_rules_type srchcrutils_get_srchcr_prev_meas_rules( wsub_id_e_type wsub_id);

boolean srchcrutils_get_srchcr_pending_immdt_resel( wsub_id_e_type wsub_id);

srchcr_res_eval_status_type srchcrutils_get_srchcr_res_eval_status( wsub_id_e_type wsub_id);

boolean srchcrutils_get_srchcr_rf_tuned_away( wsub_id_e_type wsub_id);

boolean srchcrutils_get_srchcr_start_cell_reselection( wsub_id_e_type wsub_id);

srch_cell_struct_type* srchcrutils_get_srchcr_strong_cell_ptr( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY

boolean srchcrutils_get_srchcr_abs_prio_f3_debug( wsub_id_e_type wsub_id);

#endif

extern srchcr_res_eval_meas_type srchcrutils_get_res_eval_meas_pending(wsub_id_e_type wsub_id);

extern boolean srchcrutils_pend_meas_req(wsub_id_e_type wsub_id);

uint8 srchcrutils_get_lte_detect_earfcn_idx(srchcr_res_eval_meas_type lte_srch_type);

uint8 srchcrutils_get_lte_meas_earfcn_idx(srchcr_res_eval_meas_type lte_srch_type);

extern uint32 srchcrutils_get_lte_earfcn(wsub_id_e_type wsub_id);

uint8 srchcrutils_get_log_pkt_srch_index(srchcr_res_eval_meas_type srch_type);

/*---------------------------------------------------------------------------
                SRCHCRUTILS SET WRAPPER FUNCTIONS
---------------------------------------------------------------------------*/

void srchcrutils_set_srch_cell_reselect_enabled(boolean opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srch_cr_cell_ptr(srch_cell_struct_type* opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_barred_cell_elapsed_time(uint32 opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_cell_bar_ext_msg_recd(boolean opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_drx_waiting_on_srch_sleep(boolean opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_gsm_cell_ptr(srch_gsm_cell_struct_type* opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_lte_cell_ptr(wsrchlte_cell_struct_type* opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_hcs_interrat_nset_cnt(uint8 opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_hcs_inter_sync_nset_cnt(srchcr_inter_f_idx_enum_type idx, uint8 opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_hcs_inter_async_nset_cnt(srchcr_inter_f_idx_enum_type idx, uint8 opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_hcs_intra_sync_nset_cnt(uint8 opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_ignore_barred_freqs(boolean opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_inter_f_cell_ptr(srch_interf_cell_struct_type* opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_inter_f_idx(srchcr_inter_f_idx_enum_type opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_interf_resel_fail(boolean opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_out_of_service_timer_expired(boolean opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_pending_cell_resel(boolean opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srch_prev_drx_filt_eng(uint32 opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_rem_oos_time(rex_timer_cnt_type opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_res_eval_meas_completed(srchcr_res_eval_meas_type opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_res_eval_meas_pending(srchcr_res_eval_meas_type opt, wsub_id_e_type wsub_id);

void srchcrutils_qsh_set_allow_srch_type(srchcr_res_eval_meas_type opt, wsub_id_e_type wsub_id);

void srchcrutils_qsh_set_block_srch_type(srchcr_res_eval_meas_type opt, wsub_id_e_type wsub_id);


void srchcrutils_set_srchcr_res_eval_status(srchcr_res_eval_status_type opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_rf_tuned_away(boolean opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_start_cell_reselection(boolean opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_start_CR(boolean opt, wsub_id_e_type wsub_id);

void srchcrutils_set_srchcr_strong_cell_ptr(srch_cell_struct_type* opt, wsub_id_e_type wsub_id);

void srchcrutils_reset_scheduling_mode(wsub_id_e_type wsub_id);
#ifdef FEATURE_DUAL_SIM

extern boolean srchutils_chk_srches_needed( wsub_id_e_type wsub_id);

#endif

extern void srchzz_clear_pend_bsic_verify_g2w_ta_exit(void);

boolean srchcrutils_chk_srches_needed_in_a_drx(wsub_id_e_type wsub_id);

#endif /* SRCHCRUTILS_H */	
