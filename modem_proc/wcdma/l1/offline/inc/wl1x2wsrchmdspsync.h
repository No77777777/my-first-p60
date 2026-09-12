#ifndef WL1X2WSRCHMDSPSYNC_H
#define WL1X2WSRCHMDSPSYNC_H

/*===========================================================================
               WL1 G2W Search WCDMA Firmware Interface

GENERAL DESCRIPTION
This module implements low level interface support with WCDMA firmware to
support G2W searching.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2000 - 2013 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1x2wsrchmdspsync.h#5 $
$DateTime: 2020/03/24 01:42:15 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
03/24/20   rs      Handling reverse sequence for RF_TUNE_SKIP from FW
01/10/20   ac      Mini dump framework checkin
11/13/19   hs     Added uarfcn TO 0x41D7, 0x41D8, 0x41D9 log packet
04/24/17   rs      WFW GAP_END flag support
05/20/16   mk      Hard coding enabling of STTD for G2W seraches only for Tabasco targets 
04/25/16   db      Handle RF SKIP TUNE from WFW for X2W searches  
12/21/15   wz      send STOP cmd to WFW 
09/22/15   us      Fixed compilation errors got after enabling ATLAS and RFM_PATH feature
10/20/15   ks      Use 192 window size for L2W reconf searches
09/16/15   us      Fixed compilation errors got after enabling ATLAS and RFM_PATH feature
10/12/15   db      Correct the PN reconf energy threshold for L2W connected mode searches
10/02/15   gn      Added 0x41D7, 0x41D8, 0x41D9 log packet for X2W driver code
08/26/15   cc      Create cb function type for CXM ready cb
08/19/15   ks      Lower the number of X2W stage2 tasks
08/11/15   abs     Changes to support X2W MEAS frequency reporting
07/07/15   ks      Send correct CxM channel ID and device
06/22/15   cc      L2W MSGR support
06/10/15   pvr     Update CXM Featurization logic
06/04/15   cc      Port direct stage 2 change
06/03/15   pr      W+W Phase8 Checkin.
05/20/15   sks     Removing the earlier change for compilation.
05/19/15   sks     Changes to support PICH and measurements in L/W/G + /G.
04/10/15   cc      Add G2W MSGR support
04/03/15   cc      Remove GERAN internal feature
03/19/15   cc      Replace cmd queue with the queue from mdspsync
01/22/15   mm      IRAT globals re-organization for W+W
08/18/14   abs     WL1CXM support for DR-DSDS feature
07/10/14   cc      Fix a wrong type of cleanup_wcdma API 
06/24/14   cc      Moving G2W drivers to WL1 folders
06/01/14   ks      Bolt FED API checkin
12/08/13   og      CR522046 Configure WFW for COEX management in DSDA
06/12/12   cja     CR383165 Remove compiler warnings
31/10/12   pg      CR409233: updates for G2W on Dime modem 
03/15/12   mm      CR 344531 G2W drivers new design
26/10/11   mm      CR313554 X2W_MODE_CFG_CMD is issued only after WFW ack'ed start
04/10/11   cja     CR310265 Disable WFW after suspend WFW
08/06/11   cja     Nikel G2W changes (so WFW does startup)
01/05/11   yh      Added get function for abort status
30/11/10   og      G2W dedicated timeline fix. CR266049 
10/06/10   scm     Implement g2w_srch_abort_then_wfw_sleep() function.
06/05/09   scm     Extern g2w_srch_initiate_search().
21/04/09   scm     Extern g2w_srch_start_wfw() and small API mod.
16/04/09   nf      MUSTANG needs to use g2w_issue_command_to_fw()
15/04/09   scm     Extern g2w_srch_unfreeze_sample_server().
25/03/09   scm     Modify g2w_store_stage1/2_result() comment headers.
03/03/09   scm     Run-time variables to adjust amount of debug messaging.
30/03/09   ws      Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                   to replace FEATURE_GSM_GPRS_MUSTANG
23/01/09   cs      Added extra featurisation to allow WPLT to build
23/12/08   agv     Changed Enum define WfwG2WTypeEnum to align with new WFW.
12/12/08   scm     Implement idle and dedicated reconfirmation PN search.
10/12/08   agv     Added a new I/F for Mustang WFW.
12/10/08   scm     Extern g2w_store_stage1_result()/g2w_store_stage2_result().
04/12/08   og      Mustang G2W IRAT integration.
                   SCM Replace g2w_srch_stop_wfw() with g2w_srch_sleep_wfw().
09/25/08   scm     More mods to support FEATURE_GSM_GPRS_MUSTANG feature.
09/23/08   scm     Mods to write Stage1/2 PN search params to FW.
09/22/08   scm     More G2W FEATURE_GSM_GPRS_MUSTANG mods.
09/16/08   scm     Initial revision. 
===========================================================================*/
#include "comdef.h"
#include "uecomdef.h"
#include "queue.h"

#include "wfw_sw_intf.h"
#include "wfw_sw_cmd_proc_intf.h"
#include "rfcom.h"

#include "wl1x2wsrchapi.h"
#include "wl1x2wsrchlog.h"
#include "mdspsync.h"

/*=============================================================
                  DEFINITIONS
==============================================================*/
#define WL1_X2W_SRCH_PRI_CPICH_OVSF  0
                               
/* Step1 saturation energy value */
#define WL1_X2W_SRCH_STEP1_SATURATION_ENG 65535

#define WL1_X2W_SRCH_MAX_PSC_PEAKS 6 

    /* Energy Threshold of Pilot peaks */
#define WL1_X2W_SRCH_STEP3_PEAKS_PER_TASK   4

/* Energy Threshold for LPQ list search */
#define WL1_X2W_SRCH_LIST_PEAKS_PER_TASK 4
    /* Number of peaks reported for each task */
#define WL1_X2W_SRCH_LIST_LPQ_PEAKS_PER_TASK WL1_X2W_SRCH_LIST_PEAKS_PER_TASK
    /* Number of peaks reported for each LPQ task */

/* Desired PN search threshold using STTD is -22.2 dB, or energy value
** 60 in wl1_x2w_srch_mdsp_ecio_tab[], which is index 44 (-22 dB, raw:62). */
#define WL1_X2W_SRCH_LIST_SRCH_PATH_ECNO_THRESHOLD   44

/* Discard connected mode reconf PN peaks below 45, which corresponds to -22.2 dB. */
#define WL1_L2W_CONN_RECONF_PN_THRESHOLD    45

/* Desired Stage 2 search threshold using STTD is -21.1 dB, or energy value
** 76 in wl1_x2w_srch_mdsp_ecio_tab[], which is index 42 (-21 dB, raw:77). */
#define WL1_X2W_SRCH_STAGE2_SRCH_PATH_ECNO_THRESHOLD 42

/* Discard Step1 peaks below 70, which corresponds to -23.7 dB. */
#define WL1_X2W_SRCH_L2W_STEP1_THRESHOLD 70

/*Threshold to retrigger NASTT immediately*/
#define WL1_X2W_SRCH_L2W_NASTT_RETRIGGER_THRESHOLD (WL1_X2W_SRCH_L2W_STEP1_THRESHOLD + 30)

#if defined(FEATURE_WCDMA_JOLOKIA_MODEM) || defined(FEATURE_WCDMA_TABASCO_MODEM)
/* Discard NAS-TT(NASTT 2048x2) peaks below 41, which corresponds to -23.2 dB. */
#define WL1_X2W_SRCH_L2W_NASTT_THRESHOLD 41
#else
/* Discard NAS-TT peaks below 49, which corresponds to -23.1 dB. */
#define WL1_X2W_SRCH_L2W_NASTT_THRESHOLD 49
#endif

/* Discard Idle Step1 peaks below 71 (256 x 45 slots), NO RXD, which corresponds to -25 dB. */
#define WL1_X2W_SRCH_L2W_IDLE_STEP1_THRESHOLD      71
/* Discard Connected Step1 peaks below 47 (256 x 30 slots), NO RXD, which corresponds to -25 dB. */
#define WL1_X2W_SRCH_L2W_CONN_STEP1_THRESHOLD      47
/* Threshold to retrigger IDLE NASTT immediately */
/*Step-1 256x45, No RxD, -21.6, linear 121 */
#define WL1_X2W_SRCH_L2W_IDLE_NASTT_RETRIGGER_THRESHOLD 121
/* Threshold to retrigger CONN NASTT immediately */
/*Step-1 256x30, No RxD, -21.6, linear 81 */
#define WL1_X2W_SRCH_L2W_CONN_NASTT_RETRIGGER_THRESHOLD 81
                                                                                                                            
/******************************************************** 
  Dedicated mode stage1 parameters                                                        
*********************************************************/                                                          
  /* More peaks handled with this feature.*/
#define WL1_X2W_SRCH_STAGE1_NUM_TASKS 140
                                      
/******************************************************** 
  Dedicated mode stage2 parameters                                                        
*********************************************************/
#define WL1_X2W_SRCH_STAGE2_PEAKS_PER_TASK  4

#ifdef FEATURE_WCDMA_TABASCO_MODEM
#define WL1_X2W_SRCH_STAGE2_NUM_TASKS 35 /* +1 may be added if previous position is known */
#else
#define WL1_X2W_SRCH_STAGE2_NUM_TASKS 16 /* +1 may be added if previous position is known */
#endif

#define WL1_X2W_SRCH_DEDMODE_RECONF_NC  31
  /* Coherent integration length used for reconf search,
     corresponding to 2048 chips */

#define WL1_X2W_SRCH_DEDMODE_RECONF_NN  0
  /* Non-coherent integration length for reconf search,
     corresponding to 1 coherent integration pass */       

#define WL1_G2W_SRCH_DEDMODE_RECONF_WIN_SIZE 128 /* chips */

#define WL1_L2W_SRCH_DEDMODE_RECONF_WIN_SIZE 192 /* chips */

/* Debug variable for PN search over entire frame */
#define WL1_X2W_SRCH_STAGE1_PN_NUM_TASKS 30

#define WL1_X2W_SRCH_STAGE1_PN_WIN_SIZE_CHIPX1 1280

#define WL1_X2W_SRCH_STAGE2_DIRECT_PN_MAX_PSC_PER_GAP         4
#define WL1_X2W_SRCH_STAGE2_DIRECT_PN_NUM_HYPOTHESIS_PER_PSC   25
#define WL1_X2W_SRCH_STAGE2_DIRECT_PN_WIN_SIZE_CHIPX1          1536
#define WL1_X2W_SRCH_STAGE2_DIRECT_PN_MAX_NUM_TASKS_PER_GAP    75

/* Discard direct stage 2 PN( 2048x1) peaks below 81, which corresponds to -20.8 dB. */
#define  WL1_X2W_SRCH_DIRECT_STAGE2_PN_THRESHOLD 81

#ifndef FEATURE_WCDMA_TABASCO_MODEM
/* Discard Idle Step1 peaks below 75 (256 x 45 slots), which corresponds to -24.6 dB. */
#define  WL1_X2W_SRCH_STEP1_THRESHOLD      75
#else
/* Discard Idle Step1 peaks below 71 (Step-1 256x45, No RxD) which corresponds to -25 dB. */
#define  WL1_X2W_SRCH_STEP1_THRESHOLD      71
#endif

/* Maximum length of mDSP command queue */
#define WL1_X2W_SRCH_MDSP_SYNC_DSP_CMD_Q_LEN_MAX 10

/* Maximum length of free command buffers queue */
#define WL1_X2W_SRCH_MDSP_SYNC_FREE_CMD_BUF_Q_LEN 40

/* Max length of Trace debug buffer */
#define WL1_X2W_SRCH_MDSP_SYNC_TRACE_BUF_CMD_MAX 6*2*5

#define WL1_X2W_SRCH_STEP_1_NUM_PEAKS       8

#define WL1_X2W_SRCH_STEP1_CONN_NUM_PEAKS   12

/* Max number of Step1 peaks to keep */
#define WL1_X2W_SRCH_MAX_PSC_PEAKS_TO_KEEP  WL1_X2W_SRCH_STEP1_CONN_NUM_PEAKS

/* Maximum number of Step 1 peaks we pass to FW for NAS-TT search. */
#define WL1_X2W_SRCH_MAX_NASTT_PEAK_COUNT                6
#define WL1_X2W_SRCH_MAX_DED_NASTT_PEAK_COUNT_HIGH_CLK   4
#define WL1_X2W_SRCH_MAX_DED_NASTT_PEAK_COUNT_MED_CLK    3
#define WL1_X2W_SRCH_MAX_DED_NASTT_PEAK_COUNT_LOW_CLK    2

#define WL1_X2W_SRCH_NASTT_PEAKS_PER_TASK           4

#define WL1_X2W_SRCH_MAX_NASTT_RESULTS              \
        (WL1_X2W_SRCH_MAX_NASTT_PEAK_COUNT * WL1_X2W_SRCH_NASTT_PEAKS_PER_TASK)

/* Max peaks per task for PN after NAS-TT search */
#define WL1_X2W_SRCH_PN_AFTER_NASTT_PEAKS_PER_TASK       6

#define WL1_X2W_SRCH_MAX_PN_AFTER_NASTT_PEAK_COUNT       6

/* Dedicated Mode Stage1 peaks per task */
#define WL1_X2W_SRCH_STAGE1_PN_PEAKS_PER_TASK       WL1_X2W_SRCH_LIST_PEAKS_PER_TASK

#define WL1_X2W_SRCH_STAGE2_PN_PEAKS_PER_TASK       WL1_X2W_SRCH_LIST_PEAKS_PER_TASK

/* Max number of Stage1 Peaks */
#define WL1_X2W_SRCH_STAGE1_MAX_TOTAL_PEAKS         \
        (30 * WL1_X2W_SRCH_STAGE1_PN_PEAKS_PER_TASK)

/* Max tasks for Dedicated Mode Stage2 */
#define WL1_X2W_SRCH_MDSP_STAGE2_NUM_TASKS 35 /* +1 may be added */

/* Sometimes extra Stage2 search task gets added on, which is why add one here.
** Number of Stage2 PN search tasks from WFW should never be bigger than this. */
#define WL1_X2W_SRCH_STAGE2_MAX_TASKS_FINISHED  (WL1_X2W_SRCH_STAGE2_NUM_TASKS + 1)

/* Max PN peaks per task in idle */
#define WL1_X2W_SRCH_NON_STAGED_PN_PEAKS_PER_TASK   6

/* Window Size for PN Search */
#define WL1_X2W_SRCH_PN_WIN_SIZE_CHIPX1           128
#define WL1_X2W_SRCH_PN_WIN_SIZE_CHIPX8          1024

/* Ec/Io Table size entries */
#define WL1_X2W_SRCH_ECIO_TAB_SIZ 64

/* Convert from chipx1 units to 64-chipx1 units, and subtract 1, because
** that's what WFW wants. */
#define WL1_X2W_SRCH_COH_INT_LEN_TO_WFW_UNITS(coh_int_len)         \
        ((coh_int_len >> 6) - 1)

/* Does opposite of previous macro. */
#define WL1_X2W_SRCH_COH_INT_LEN_TO_SW_UNITS(coh_int_len)          \
        ((coh_int_len + 1) << 6)

/* Subtract 1 from number of slots, because that's what WFW wants. */
#define WL1_X2W_SRCH_NON_COH_INT_LEN_TO_WFW_UNITS(non_coh_int_len) \
        (non_coh_int_len - 1)

/* Does opposite of previous macro. */
#define WL1_X2W_SRCH_NON_COH_INT_LEN_TO_SW_UNITS(non_coh_int_len)  \
        (non_coh_int_len + 1)

#define WL1_X2W_SRCH_STAGE1_PN_COH_INT_LEN          \
        WL1_X2W_SRCH_COH_INT_LEN_TO_WFW_UNITS(1024)
#define WL1_X2W_SRCH_STAGE1_PN_NON_COH_INT_LEN      \
        WL1_X2W_SRCH_NON_COH_INT_LEN_TO_WFW_UNITS(1)

#define WL1_X2W_SRCH_PN_COH_INT_LEN                 \
        WL1_X2W_SRCH_COH_INT_LEN_TO_WFW_UNITS(2048)
#define WL1_X2W_SRCH_PN_NON_COH_INT_LEN             \
        WL1_X2W_SRCH_NON_COH_INT_LEN_TO_WFW_UNITS(1)

#define WL1_X2W_SRCH_CONN_STEP1_NON_COH_INT_LEN 5

/* code_group is psc >> 3 or scr >> 7   */
#define WL1_X2W_SRCH_SCR_CODE_TO_CODE_GROUP(scr_code) (scr_code >> 7)

#define wl1_x2w_srch_mdsp_start_mode_type WfwCmdStartWfwCfgEnum

#ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
#define wl1_x2w_srch_mdsp_cxm_param_cfg_type WfwCxmParamCfgCmdStruct
#define wl1_x2w_srch_mdsp_start_mode_ue_type WfwStartCmdUeTypeEnum
#endif

/* Used by the HW module to signal rdy. */
#define WL1_X2W_SRCH_L1_WAIT_HW_SIG               0x0004
/* MDSP download timeout signal */
#define WL1_X2W_SRCH_L1_TIMEOUT_HW_SIG            0x0008

#define WL1_X2W_SRCH_LPQ_MAX_TASKS  41
/* Max tasks for Reconf List Search */
#define WL1_X2W_MDSP_LIST_SRCH_MAX_TASKS WL1_X2W_SRCH_LPQ_MAX_TASKS

#define WL1_X2W_SRCH_MAX_PN_PEAKS 120

#define WL1_X2W_SRCH_INVALID_IDX                   0xFFFF
/*================================================================================
                           Enumerations
================================================================================*/
typedef enum
{
  WL1_G2W_SRCH,
  WL1_L2W_SRCH,
} wl1_x2w_drv_type_enum_type;

/* ------------------ Firmware parameters -------------------------------------*/

/* Peak Detector Mode */
typedef enum 
{
  /* "0x00": Peak Detection enabled. */
  WL1_X2W_SRCH_PD_NORMAL,
  /* "0x01": Peak Detection Disabled. */
  WL1_X2W_SRCH_PD_BYPASS,
  /* "0x02": used for DRX mode search. */  
  WL1_X2W_SRCH_PD_SHOULDER
} wl1_x2w_srch_pd_enum_type;

/* Carrier to search, primary or secondary.  Only uses primary. */
typedef enum
{
  WL1_X2W_SRCH_CARRIER_PRIMARY   = 0,
  WL1_X2W_SRCH_CARRIER_SECONDARY = 1,
  WL1_X2W_SRCH_CARRIER_MAX
} wl1_x2w_srch_carrier;


/* RxD value returned from firmware:
** - 0: Search results NOT obtained through RxD.
** - 1: Search results obtained through RxD separate mode.
** - 2: Search results obtained through RxD add mode. */
typedef enum
{
  WL1_X2W_SRCH_RXD_MODE_NO_RXD,
  WL1_X2W_SRCH_RXD_MODE_SEPARATE,
  WL1_X2W_SRCH_RXD_MODE_ADD,
  WL1_X2W_SRCH_RXD_MODE_MAX
} wl1_x2w_srch_rxd_mode_enum_type;

/* Firmware parameters */
typedef enum
{
  WL1_X2W_SRCH_ANTENNA_0,
  WL1_X2W_SRCH_ANTENNA_1,
  WL1_X2W_SRCH_ANTENNA_MAX
} wl1_x2w_srch_antenna_select;


/* Search Alignment */
typedef enum 
{
  /* "0x00": No alignment needed */
  WL1_X2W_SRCH_ALIGN_NONE = 0,
  /* "0x01": 256-chip alignment */
  WL1_X2W_SRCH_ALIGN_256,
  /* "0x02": 512-chip alignment */
  WL1_X2W_SRCH_ALIGN_512,
  /* "0x03": 1024-chip alignment */
  WL1_X2W_SRCH_ALIGN_1024
} wl1_x2w_srch_align_enum_type;

/* Step1/NASTT/PN search task mode */
typedef enum
{
  WL1_X2W_SRCH_TASK_MODE_NO_RXD_ADD_NO_STTD_NO_RXD_SEPARATE,
  WL1_X2W_SRCH_TASK_MODE_YES_RXD_ADD_NO_STTD_NO_RXD_SEPARATE,
  WL1_X2W_SRCH_TASK_MODE_NO_RXD_ADD_YES_STTD_NO_RXD_SEPARATE,
  WL1_X2W_SRCH_TASK_MODE_YES_RXD_ADD_YES_STTD_NO_RXD_SEPARATE,
  WL1_X2W_SRCH_TASK_MODE_NO_RXD_ADD_NO_STTD_YES_RXD_SEPARATE,
  WL1_X2W_SRCH_TASK_MODE_INVALID,
  WL1_X2W_SRCH_TASK_MODE_NO_RXD_ADD_YES_STTD_YES_RXD_SEPARATE,
  WL1_X2W_SRCH_TASK_MODE_MAX
} wl1_x2w_srch_task_mode;

/* FW parameters, srch_ss_freeze_flag */
typedef enum
{
  WL1_X2W_SRCH_SS_FREEZE_FLAG_NO_ACTION,
  WL1_X2W_SRCH_SS_FREEZE_FLAG_STEP1_SEARCH_FROZEN,
  WL1_X2W_SRCH_SS_FREEZE_FLAG_NASTT_SEARCH_UNFROZEN,
  WL1_X2W_SRCH_SS_FREEZE_FLAG_PN_SEARCH_UNFROZEN_BEFORE,
  WL1_X2W_SRCH_SS_FREEZE_FLAG_PN_SEARCH_UNFROZEN_AFTER,
  WL1_X2W_SRCH_SS_FREEZE_FLAG_MAX
} wl1_x2w_srch_ss_freeze_flag_enum_type;

/* FW parameters, srch_use_freeze_flag */
typedef enum
{
  WL1_X2W_SRCH_USE_FREEZE_FLAG_NO_ACTION,
  WL1_X2W_SRCH_USE_FREEZE_FLAG_NASTT_USES_FROZEN_SAMPLES,
  WL1_X2W_SRCH_USE_FREEZE_FLAG_PN_USES_FROZEN_SAMPLES,
  WL1_X2W_SRCH_USE_FREEZE_FLAG_MAX
} wl1_x2w_srch_use_freeze_flag_enum_type;

/* X2W Search Substate machine */
typedef enum
{
  WL1_X2W_SRCH_NO_SEARCH,
  WL1_X2W_SRCH_STEP1_SEARCH,
  WL1_X2W_SRCH_NASTT_SEARCH,
  WL1_X2W_SRCH_PN_SEARCH,
  WL1_X2W_SRCH_STAGE1_SEARCH,
  WL1_X2W_SRCH_SECOND_STAGE1_SEARCH,
  WL1_X2W_SRCH_STAGE1_STAGE2_SEARCH,
  WL1_X2W_SRCH_STAGE2_STAGE1_SEARCH,
  WL1_X2W_SRCH_STAGE2_SEARCH,
  WL1_X2W_SRCH_DIRECT_STAGE2_SEARCH,
  WL1_X2W_SRCH_RECONF_SEARCH,
  WL1_X2W_SRCH_NUM_SEARCHES
} wl1_x2w_srch_drv_search_state_enum_type;


/*------------------------drivers operations enums-----------------------*/

/* FW to SW ISR events */
typedef enum
{
  WL1_X2W_SRCH_MDSP_SW_COMMAND_DONE,
  WL1_X2W_SRCH_MDSP_SW_SEARCH_DONE,
  WL1_X2W_SRCH_MDSP_SW_RF_ON,
  WL1_X2W_SRCH_MDSP_SW_AGC_TUNING_DONE,
  WL1_X2W_SRCH_MDSP_SW_X2W_SAMPLE_RAM_DONE,
  WL1_X2W_SRCH_MDSP_SW_X2W_RF_TUNE_SKIP_DONE,
  WL1_X2W_SRCH_MDSP_SW_X2W_GAP_END_IND,
  WL1_X2W_SRCH_MDSP_SW_CRASH_IND,
  WL1_X2W_SRCH_MDSP_SW_NUM_EVENTS  
} wl1_x2w_srch_mdsp_event_type;

/*========================================================================
                           Typedef Callback functions
=========================================================================*/
/* Generic Callback function type for FW-to-SW event done */
typedef void (*wl1_x2w_srch_event_done_cb_fn) (wl1_x2w_srch_mdsp_event_type, WfwSwCmdEnum, wsub_id_e_type);

typedef void (*wl1_x2w_srch_cxm_ready_cb_fn) (wsub_id_e_type wsub_id); 
/*========================================================================== 
                           Structs 
===========================================================================*/
/*---------------------Dedicated mode Stage 1/2 cell structure----------------*/

/* Dedicated stage1/2 current cell search parameters */
typedef struct 
{
  uint16 freq;
  /* Scrambling Code index */
  #if defined(FEATURE_WCDMA_JOLOKIA_MODEM) || defined(FEATURE_WCDMA_TABASCO_MODEM)
  uint16  pscs[WL1_X2W_SRCH_STAGE2_DIRECT_PN_MAX_PSC_PER_GAP];
  #endif
  uint16  scr_code;
  /* original pn_pos */  
  uint16 pn_pos ;
  /* Number of tasks */
  uint8  num_tasks;
  /*To indicate STTD enable*/
  boolean sttd;
  /* Window starting PN Position in chipx1 */
  uint16  task_pos_hi[WL1_X2W_SRCH_STAGE2_NUM_TASKS+1];
} wl1_x2w_srch_ded_mode_srch_q_parm_struct_type;


/*------------------------LIST SEARCH PARAMETERS STRUCTURE----------------*/

/* List search task struct */
typedef struct 
{
  /* Scrambling Code index */
  uint16  scr_code;
  /* OVSF for Primary CPICH */
  uint16  ovsf;
  /* Window starting PN Position in chipx8 */
  uint32  pn_pos;
  /* Search Window Size in chipx8 */
  uint16  win_siz;
  /* Peak Detector Mode */
  wl1_x2w_srch_pd_enum_type  pd;
  /* Alignment for coherent integration */
  wl1_x2w_srch_align_enum_type align;
  /* Coherent integration length */
  uint16  nc;
  /* Non-coherent integration length */
  uint16  nn;
} wl1_x2w_srch_mdsp_list_srch_task_struct_type;

/* List search parameters struct*/
typedef struct 
{
  uint16 freq;
  /* Search from Sample RAM? */
  boolean sr;
  /* 3 common LSB's */
  uint16  pn_pos_lo;
  /* Number of tasks */
  uint16  num_tasks;
  /*To indicate STTD enable*/
  boolean sttd;
  /* List search tasks */
  wl1_x2w_srch_mdsp_list_srch_task_struct_type  task[WL1_X2W_MDSP_LIST_SRCH_MAX_TASKS];
} wl1_x2w_srch_mdsp_list_srch_q_parm_struct_type;

/*---------------------------------Search Results struct--------------------------*/

/* NAS-TT Results struct */
typedef struct
{
  uint16  psc;
  uint32  max_pos;
  uint32  max_eng;
} wl1_x2w_srch_peak_struct_type;

typedef struct
{
  uint8 index;
  uint16 num_peaks;
  wl1_x2w_srch_peak_struct_type nastt_peak[WL1_X2W_SRCH_MAX_NASTT_RESULTS];
} wl1_x2w_srch_nastt_result_struct_type;

/* Dedicated Mode Stage1 Result Struct */
typedef struct
{
  uint16 pn_pos;
  uint32 eng;
} wl1_x2w_srch_ded_mode_task_result_type;

/* Following structure is for storing the stage1 result   */
typedef struct
{
  uint16 freq;
  /* Input PN position */   
  uint16 in_pn_pos;  
  uint16 num_results;
  uint8 num_tasks_finished;
  #if defined(FEATURE_WCDMA_JOLOKIA_MODEM) || defined(FEATURE_WCDMA_TABASCO_MODEM)
  uint16 pscs[WL1_X2W_SRCH_STAGE2_DIRECT_PN_MAX_PSC_PER_GAP];
  /* These 2 fields keep track of the best peak found in the results back from 
     this working unit (needed because results can come back across multiple 
     gaps) */
  /* psc acq best peak energy */
  uint16 best_egy_so_far;
  /* psc acq best peak PN position */
  uint32 best_pn_pos_cx1_so_far;
  /* the hypothesis tasks in this PSC (totoal 25) for which searches have been completed */
  uint8 num_hypothesis_completed;
  #endif
  uint16 scr_code; 
  wl1_x2w_srch_ded_mode_task_result_type task[WL1_X2W_SRCH_STAGE1_NUM_TASKS];
} wl1_x2w_srch_ded_mode_srch_result_struct_type;

/* Peak structure type */
typedef struct
{
  /* position, or offset to the Slot Reference Counter,
     of this slot peak in chipx8 resolution */
  uint16  pos;
  /* Energy of this slot peak */
  uint16  eng;
} wl1_x2w_srch_slot_peak_struct_type;

/* Dedicated search output structure. It is used for sending results back to source L1 */
typedef struct 
{
  uint16                     uarfcn;
  int16                      rssi;
  uint8                      num_id_cells;
  uint8                      num_reconf_cells;
  wl1_x2w_measured_cell_type id_cells[WL1_X2W_SRCH_MAX_PSC_PER_FREQ];
  wl1_x2w_measured_cell_type reconf_cells[WL1_X2W_SRCH_MAX_PSC_PER_FREQ];
} wl1_x2w_srch_measured_result_struct_type;

typedef enum
{
  SRCH_STEP_NONE_SRCH         = 0,   /* 0 means no search is active           */
  SRCH_STEP1_X2W_SRCH         = 1,   /* Step_1 search                         */
  SRCH_STEP_LIST              = 4,   /* List Search                           */
  /* Special search types for off-line searcher.                                */
  SRCH_OFFLINE_NASTT          = 8,   /* NAS-TT search                         */
  SRCH_OFFLINE_PN_AFTER_NASTT = 9,   /* PN search after NAS-TT search         */
  SRCH_OFFLINE_STEP1          = 10,  /* Step_1 search                         */
  SRCH_OFFLINE_IDLE_RECONF_PN = 11,  /* Reconfirmation PN search in idle      */
  SRCH_OFFLINE_DED_RECONF_PN  = 12,  /* Reconfirmation PN search in dedicated */
  SRCH_OFFLINE_STAGE1         = 13,  /* Dedicated mode stage 1                */
  SRCH_OFFLINE_STAGE2         = 14,  /* Dedicated mode stage 2                */
  SRCH_STEP_ENUM_MAX          = 15
} wl1_x2w_srch_step_enum_type;

/*-----------------------------FW cmd sync params struct type----------------------*/

/*----------------UNION: MDSP_SYNC_CMD_PARAMS_TYPE-------------------*/

/*-------------------------STRUCTURE: MDSP_SYNC_CMD_TYPE-----------------*/

/* This structure is used for sending commands to the mDSP via the L1 sync
interface. */

typedef struct 
{
  /* which command is being sent */
  uint16 cmd_title;
  /* GSM Frame number at which the cmd is sent to FW */
  uint16 gfn; 

  /* WFW->G command meaning WFW is done processing the command. */
  WfwSWCmdDoneFlagsEnum cmd_done_flag;

  /* How long until we consider command timed out. */
  timetick_type          cmd_done_timeout_msec;
} wl1_x2w_srch_mdsp_sync_cmd_struct_type;

typedef struct
{
  /* Debug option for allowing PN after NAS-TT search over the entire frame */
  boolean wl1_x2w_srch_debug_search_entire_frame;
  uint8 wl1_x2w_srch_ded_nastt_max_task_per_gap;
  uint8 wl1_x2w_srch_ded_nastt_max_result_per_gap;
  uint16 wl1_x2w_srch_wfw_step1_peak_cnt;
  /* Number of tasks in LPQ. After a search dump, this will be the number
  tasks that DSP&HW have searched  */
  uint16 wl1_x2w_srch_wfw_lpq_cnt;
  uint16 wl1_x2w_srch_wfw_cur_stage2_cell_idx;
  /* Current Stage1 and Stage2 cell index */
  uint16 wl1_x2w_srch_wfw_cur_stage1_cell_idx;
  /* Stage 1 and 2 Results and result indexes */
  uint16 wl1_x2w_srch_wfw_cur_stage1_result_idx;
  uint16 wl1_x2w_srch_wfw_cur_stage2_result_idx;
  /* Outputs passed */
  wl1_x2w_srch_measured_result_struct_type wl1_x2w_srch_wfw_list_srch_result;
  wl1_x2w_srch_measured_result_struct_type wl1_x2w_srch_wfw_pn_srch_result;
  wl1_x2w_srch_nastt_result_struct_type wl1_x2w_srch_wfw_nastt_result;
  /* Dedicated mode Stage1/2 results */
  wl1_x2w_srch_ded_mode_srch_result_struct_type wl1_x2w_srch_wfw_stage1_result;
  wl1_x2w_srch_ded_mode_srch_result_struct_type wl1_x2w_srch_wfw_stage2_result;
  /* Current cell parameters doing Dedicated stage1 and stage2 searches */
  wl1_x2w_srch_ded_mode_srch_q_parm_struct_type wl1_x2w_srch_wfw_cur_stage1_cell;
  wl1_x2w_srch_ded_mode_srch_q_parm_struct_type wl1_x2w_srch_wfw_cur_stage2_cell;
  rex_crit_sect_type wl1_x2w_srch_drv_crit_sec;
  /* X2W log packets*/
  WL1_X2W_SRCH_STEP1_LOG_PACKET_type wl1_x2w_srch_step1_log_pkt;
  WL1_X2W_SRCH_NASTT_LOG_PACKET_type wl1_x2w_srch_nastt_log_pkt;
  WL1_X2W_SRCH_LIST_LOG_PACKET_type wl1_x2w_srch_list_log_pkt;
} wl1x2wsrchmdspsync_ext_cntrl_params_type;

extern wl1x2wsrchmdspsync_ext_cntrl_params_type wl1x2wsrchmdspsync_ext_cntrl_params[WCDMA_NUM_SUBS];

typedef struct
{
  /* Flag set to TRUE to WL1_ERR_FATAL if WFW command timeout was detected, FALSE graceful recovery */
  boolean wl1_x2w_srch_wfw_timeout_no_recovery;
  wl1_x2w_srch_slot_peak_struct_type wl1_x2w_srch_wfw_psc_peaks[WL1_X2W_SRCH_MAX_PSC_PEAKS_TO_KEEP];
  /* FW-SW Event done callback */
  wl1_x2w_srch_event_done_cb_fn wl1_x2w_srch_mdsp_event_done_callback;
  /* Used to detect G->WFW command timeouts. */
  timer_type wl1_x2w_srch_wfw_timeout_timer;
  rex_timer_type poll_timer;
  /* Run-time debug flag to try to detect WFW timeouts or not. */
  volatile boolean wl1_x2w_srch_detect_wfw_timeouts;
  volatile WfwSrchoffLineAsyncWriteStruct *wl1_x2w_srch_wfw_intf_srch_async_write_ptr;
  #ifdef FEATURE_WCDMA_DUAL_SYNC_INTERFACE
  volatile WfwSrchQ0RsltStruct *wl1_x2w_srch_wfw_intf_step1_srch_result_ptr;
  volatile WfwSrchQ1RsltStruct *wl1_x2w_srch_wfw_intf_nastt_pn_srch_result_ptr; 
  /* Callback function enabled */
  boolean wl1_x2w_srch_mdsp_event_cb_func_enabled[WL1_X2W_SRCH_MDSP_SW_NUM_EVENTS];
  #else
  volatile WfwSrchLpqHpqRsltStruct *wl1_x2w_srch_wfw_intf_srch_result_ptr;
  #endif
  volatile WfwSrchPNLpqHpqStruct *wl1_x2w_srch_wfw_pn_cmd_ptr;
  boolean wl1_x2w_rf_skipped_received;
} wl1x2wsrchmdspsync_int_cntrl_params_type;

/*=======================================================================
           F U N C T I O N    P R O T O T Y P E S
========================================================================*/
extern void wl1x2wsrchmdspsync_int_cntrl_params_init( wsub_id_e_type wsub_id);
extern void wl1x2wsrchmdspsync_ext_cntrl_params_init();
 
/*=======================================================================   
   
FUNCTION :      WL1_X2W_SRCH_MDSP_SYNC_INIT

DESCRIPTION :   This function initializes the command queue, logging buffer, 
                pointers to FW interface and registers the callback handlers
                for ISRs from FW.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.   
 
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_init(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION :      WL1_X2W_SRCH_SET_EVENT_DONE_CB

DESCRIPTION :   This function sets the callback function for event done ISRs.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    Callback function is set to be srch_drv_continue_srch.

===========================================================================*/
extern void wl1_x2w_srch_set_mdsp_event_done_cb(wl1_x2w_srch_event_done_cb_fn event_cb, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_START_CMD

DESCRIPTION     This function sends START command to mDSP

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    Pre-cb function will be executed, if registered
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_start_cmd(
  /* mode to start the firmware in */
  mdsp_sync_start_cfg_enum_type start_mode
#ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
  ,wl1_x2w_srch_mdsp_start_mode_ue_type ue_type
#endif
  /* pre call back to be executed before the command is sent to fw */
  , pre_cb_fn pre_cb,
  /* post call back to be executed after the command is sent to fw */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_MODE_CFG_CMD

DESCRIPTION     This function formats and sends a X2W_MODE_CFG command to the
                mDSP by way of the mDSP command queue.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void wl1_x2w_srch_mdsp_sync_send_mode_cfg_cmd(
  /* mode: idle or dedicated */
  wl1_x2w_srch_wfw_mode_enum_type mode,
  /* pre call back function */
  pre_cb_fn pre_cb,
  /* post call back function */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_RF_TUNE_CMD

DESCRIPTION     This function formats and sends a RF_TUNE command to the
                mDSP by way of the mDSP command queue.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void wl1_x2w_srch_mdsp_sync_send_rf_tune_cmd(
  /* Indicate if we need to tune RF, for online NASTT gap RF tune is not needed */
  boolean rf_tune_on,
  /* RxLM buffer index */
  uint32 rxlm_buf_idx,
  /* RF action time in WSTMR cx8 */
  uint32 rf_action_time,
  #if defined(FEATURE_WCDMA_JOLOKIA_MODEM) || defined(FEATURE_WCDMA_TABASCO_MODEM)
  uint32 gap_start_time,
  uint32 gap_end_time,
  #endif
  /* Freq error in Q6 Hz format */
  int32 freq_err_q6_hz,
  /* Inverse W DL freq */
  uint32 inv_dlo_freq,
  /* RF timing parameters */
  uint16 pll_settling_time,
  uint16 nbr_preload_offset_time,
  uint16 home_preload_offset_time,
  /* pre call back function */
  pre_cb_fn pre_cb,
  /* post call back function */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_MODE_RF_CFG_CMD

DESCRIPTION     This function formats and sends a X2W_MODE_CFG command to the
                mDSP by way of the mDSP command queue.  

DEPENDENCIES    None

RETURN VALUE    None 
 
SIDE EFFECTS   Pre-cb function will be executed, if registered
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_mode_rf_cfg_cmd(
  /* mode: idle or dedicated */
  wl1_x2w_srch_wfw_mode_enum_type mode,
  /* RxLM buffer index */
  uint32 rxlm_buf_idx,
  /* RF action time in WSTMR cx8 */
  uint32 rf_action_time,
  /* Freq error in Q6 Hz format */
  int32 freq_err_q6_hz,
  /* Inverse W DL freq */
  uint32 inv_dlo_freq,
  /* RF timing parameters */
  uint16 pll_settling_time,
  uint16 nbr_preload_offset_time,
  uint16 home_preload_offset_time,
  /* pre call back function */
  pre_cb_fn pre_cb,
  /* post call back function */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_RF_TUNE_STEP1_CMD

DESCRIPTION     This function formats and sends a RF_TUNE + STEP1 command to the
                mDSP by way of the mDSP command queue.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void wl1_x2w_srch_mdsp_sync_send_rf_tune_step1_cmd(
  /* RxLM buffer index */
  uint32 rxlm_buf_idx,
  /* RF action time in WSTMR cx8 */
  uint32 rf_action_time,
  #if defined(FEATURE_WCDMA_JOLOKIA_MODEM) || defined(FEATURE_WCDMA_TABASCO_MODEM)
  uint32 gap_start_time,
  uint32 gap_end_time,
  #endif
  /* Freq error in Q6 Hz format */
  int32 freq_err_q6_hz,
  /* Inverse W DL freq */
  uint32 inv_dlo_freq,
  /* RF timing parameters */
  uint16 pll_settling_time,
  uint16 nbr_preload_offset_time,
  uint16 home_preload_offset_time,
  /* First step1 task in connected mode */
  boolean first_task,
  /* Last step1 task in connected mode */
  boolean last_task,
  /* pre call back function */
  pre_cb_fn pre_cb,
  /* post call back function */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_STEP1_SRCH_CMD

DESCRIPTION     This function formats and sends a Step1 search command to the
                mDSP by way the the mDSP command queue.  

DEPENDENCIES    None

RETURN VALUE   None

SIDE EFFECTS    Pre-cb function will be executed, if registered
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_step1_srch_cmd(
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_NASTT_SRCH_CMD

DESCRIPTION     This function formats and sends a NAS-TT search command to the
                mDSP by way the the mDSP command queue.  

DEPENDENCIES    None

RETURN VALUE    None
 
SIDE EFFECTS    Pre-cb function will be executed, if registered

===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_nastt_srch_cmd(
  /* IDLE or Dedicated */
  boolean ded_srch,
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_PN_SRCH_CMD

DESCRIPTION     This function formats and sends a PN search command to the
                mDSP by way the the mDSP command queue.  

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    Pre-cb function will be executed, if registered

===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_pn_srch_cmd(
  /* PN search parameters for Stage1/Stage2 searches */
  wl1_x2w_srch_ded_mode_srch_q_parm_struct_type* pn_srch_ptr,
  /* PN after NAS-TT/Reconf search parameters */
  wl1_x2w_srch_mdsp_list_srch_q_parm_struct_type* srch_list_srch_q_parms,
  /* Trigger Stage1 search */
  boolean stage1_srch,
  /* Trigger Stage2 search; if both flags are FALSE, trigger Reconf/PN after NASTT search */
  boolean stage2_srch,
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_ABORT_CMD

DESCRIPTION     This function sends an ABORT search command to the
                mDSP by way of the mDSP command queue.  

DEPENDENCIES    None
 
RETURN VALUE    None 
 
SIDE EFFECTS    Pre-cb function will be executed, if registered
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_abort_cmd(
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_STOP_RF_CMD

DESCRIPTION     This function sends a STOP RF command to the
                mDSP by way of the mDSP command queue.  

DEPENDENCIES    None
 
RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_stop_rf_cmd(
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_SLEEP_WFW_CMD

DESCRIPTION     This function sends a sleep wfw command to the mDSP by way of 
 				the mDSP command queue when a search completes or drivers is
 				shut down by abort cmd.

DEPENDENCIES    None
 
RETURN VALUE    None

SIDE EFFECTS    Pre-cb function will be executed, if registered
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_sleep_wfw_cmd(
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_STOP_WFW_CMD

DESCRIPTION     This function sends a stop wfw command to the mDSP by way of 
 				the mDSP command queue when a search completes or drivers is
 				shut down by abort cmd.

DEPENDENCIES    None
 
RETURN VALUE    None

SIDE EFFECTS    Pre-cb function will be executed, if registered
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_stop_wfw_cmd(
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_MEAS_CLEANUP_CMD

DESCRIPTION     This function sends a measurement cleanup command to the mDSP to release L1S memory.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_meas_cleanup_cmd(
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_CLK_CFG_CMD

DESCRIPTION     This function sends a clk cfg command to the mDSP to configure VPE clock

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_clk_cfg_cmd(
  /* CLK sleep */
  uint16 clk_mode,
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_PANIC_CMD

DESCRIPTION     This function sends a panic command to the mDSP in case we
                need to freeze samples while WFW crashes.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_send_panic_cmd(
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED) && defined(FEATURE_WCDMA_RF_PATH_SUPPORT_IN_CXM)
/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_REPORT_X2W_MEAS_INFO_TO_CXM

DESCRIPTION
  This function registers the frequency info(uarfcn) of the
  scheduled X2W measurement with WL1CXM and de-registers
  the uency info(uarfcn) upon measurement completion

  Note:
  =====
  The following Static local variables
  **static uint8 pending_gap_idx;
  **static uint16 pending_gap_uarfcn;
  **static boolean gap_info_registered[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  **static boolean pending_gap_reg;
  have been introduced to handle back to back W2W Interf CM gaps
  The idea is retain/register the frequency of the second gap only
  after the frequency info of the first gap has been de-registered

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_report_x2w_meas_info_to_cxm(uint16 uarfcn,
                                                               rfm_path_type rfm_path,
                                                               boolean is_cleanup,
                                                               wl1_x2w_srch_cxm_ready_cb_fn cb_fn,
                                                               wsub_id_e_type wsub_id);
#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED)  && (FEATURE_WCDMA_RF_PATH_SUPPORT_IN_CXM)*/
/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_REPORT_X2W_MEAS_INFO_TO_CXM

DESCRIPTION
  This function registers the frequency info(uarfcn) of the
  scheduled X2W measurement with WL1CXM and de-registers
  the uency info(uarfcn) upon measurement completion

  Note:
  =====
  The following Static local variables
  **static uint8 pending_gap_idx;
  **static uint16 pending_gap_uarfcn;
  **static boolean gap_info_registered[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  **static boolean pending_gap_reg;
  have been introduced to handle back to back W2W Interf CM gaps
  The idea is retain/register the frequency of the second gap only
  after the frequency info of the first gap has been de-registered

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS
===========================================================================*/
extern void wl1_x2w_srch_mdsp_sync_report_x2w_meas_info_to_cxm(uint16 uarfcn,
                                                               rfm_device_enum_type rf_device,
                                                               boolean is_cleanup,
                                                               wl1_x2w_srch_cxm_ready_cb_fn cb_fn,
                                                               wsub_id_e_type wsub_id);
#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED)  && (FEATURE_WCDMA_RF_PATH_SUPPORT_IN_CXM)*/
/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_WRITE_CXM_WFW_ASYNC_INTF

DESCRIPTION     This function writes the CxM parameters to the WFW interface.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS
===========================================================================*/
void wl1_x2w_srch_mdsp_sync_write_cxm_wfw_async_intf(rfm_meas_common_param_type *rf_params, boolean l2w_search, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_CXM_CHANNEL_ID_TO_WFW

DESCRIPTION     This function sends the CXM channel ID to the mDSP. Used
                to configure DSDA COEX management.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS
===========================================================================*/
void wl1_x2w_srch_mdsp_sync_send_cxm_channel_id_to_wfw(
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  rfm_meas_common_param_type *rf_params,
  boolean l2w_search,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_CXM_PRIORITY_TBL_TO_WFW

DESCRIPTION     This function sends the CXM priority table to the mDSP. Used
                to configure DSDA COEX management.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS
===========================================================================*/
void wl1_x2w_srch_mdsp_sync_send_cxm_priority_tbl_to_wfw(
  /* pre-command callback */
  pre_cb_fn pre_cb,
  /* post-command callback */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

#endif /* FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW */

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_MDSP_SYNC_SEND_FW_PANIC_CMD

DESCRIPTION     This function sends Panic cmd to FW. Mostly used for debugging   
                purposes. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    Panic cmd sent to FW incase SW error fatals and for debugging both
                SW and FW states are required.

===========================================================================*/
extern boolean wl1_x2w_srch_mdsp_sync_send_fw_panic_cmd(
  /* the pre-command callback function */
  pre_cb_fn pre_cb,
  /* the post-commadn callback function */
  post_cb_fn post_cb,
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION       WL1_X2W_SRCH_SRCH_STEP1_DUMP

DESCRIPTION    This function retrieves Step 1 search results from WCDMA firmware and
               stores that data in srch_step1_peak_cnt and srch_psc_peaks[].

DEPENDENCIES   None.

PARAMETERS     None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
extern void wl1_x2w_srch_mdsp_step1_dump(
  wl1_x2w_drv_type_enum_type drv_type,
  uint16                     uarfcn,
  boolean ded_srch,
  boolean *nastt_retrigger, 
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      WL1_X2W_SRCH_MDSP_NASTT_DUMP

DESCRIPTION   This function retrieves NAS-TT search results from WCDMA firmware and
              stores the results in srch_nastt_result structure.

DEPENDENCIES  None.

PARAMETERS    None.

RETURN VALUE  uint16 - Number of NAS-TT peaks detected.

SIDE EFFECTS  None.
===========================================================================*/
extern uint16 wl1_x2w_srch_mdsp_nastt_dump(
  wl1_x2w_drv_type_enum_type drv_type, 
  uint16                     uarfcn,
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_X2W_SRCH_PN_AFTER_NASTT_DUMP

DESCRIPTION  This function retrieves PN search results from WCDMA firmware and
             stores the results in srch_step3_srch_result structure.

DEPENDENCIES None.

PARAMETERS   None.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
extern void wl1_x2w_srch_mdsp_pn_after_nastt_dump( uint16 uarfcn, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION     WL1_X2W_SRCH_MDSP_STORE_STAGE1_RESULT

DESCRIPTION  This function processes the results obtained from Stage 1 PN search in
 	         dedicated mode.
 
DEPENDENCIES None.

PARAMETERS   None

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
extern void wl1_x2w_srch_mdsp_store_stage1_result(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_X2W_SRCH_MDSP_STORE_STAGE2_RESULT

DESCRIPTION  This function processes the results obtained from Stage 2 PN search in
             dedicated mode.

DEPENDENCIES None.

PARAMETERS   None
 
RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
extern void wl1_x2w_srch_mdsp_store_stage2_result(
  uint16         uarfcn,
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_X2W_SRCH_MDSP_STORE_RECONF_RESULT

DESCRIPTION  This function processes the results obtained from Reconfirmation PN search in
             dedicated mode.

DEPENDENCIES None.

PARAMETERS   None
 
RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
extern void wl1_x2w_srch_mdsp_store_reconf_result(
  uint16         uarfcn,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      WL1_X2W_SRCH_MDSP_GET_RSSI

DESCRIPTION   This function returns the instantaneous RSSI value from the FW variable.
 
DEPENDENCIES  None.

RETURN VALUE  int16.

SIDE EFFECTS  None

===========================================================================*/
extern int16 wl1_x2w_srch_mdsp_get_rssi(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_X2W_MDSP_GET_ECIO_VALUE

DESCRIPTION
  This function returns the Ec/Io value from the table

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
extern uint32 wl1_x2w_mdsp_get_ecio_value(uint8 index);

/*===========================================================================

FUNCTION     WL1_X2W_SRCH_DRV_MUTEX_LOCK

DESCRIPTION
  This function locks the mutex for search drivers shared between task and
  ISR.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
extern void wl1_x2w_srch_drv_mutex_lock(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_X2W_SRCH_DRV_MUTEX_UNLOCK

DESCRIPTION
  This function unlocks the mutex for search drivers shared between task and
  ISR.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
extern void wl1_x2w_srch_drv_mutex_unlock(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_X2W_SRCH_STEP1_LOGGING

DESCRIPTION
  This function is used for logging X2W Step-1 search results

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
extern void wl1_x2w_srch_step1_logging(int16 current_rssi, 
                                       uint16 num_peaks_requested, 
                                       uint16 step1_valid_peak_cnt, 
                                       uint32 peak_threshold, 
                                       uint32 nastt_retrigger_threshold, 
                                       uint16         uarfcn,
                                       wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_X2W_SRCH_NASTT_LOGGING

DESCRIPTION
  This function is used for logging X2W NASTT search results

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
extern void wl1_x2w_srch_nastt_logging(int16 rssi, 
                                       uint16 num_peaks, 
                                       uint16         nastt_valid_peak_cnt,
                                       uint16         uarfcn,
                                       wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_X2W_SRCH_LIST_LOGGING

DESCRIPTION
  This function is used for logging X2W List search results

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
extern void wl1_x2w_srch_list_logging(uint8 srch_type, 
                                      uint8 num_tasks,
                                      int16 rssi,
                                      uint16 results_per_task,
                                      WfwSrchListRsltStruct* pn_results_ptr,
                                      uint16                 uarfcn,
                                      wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION wl1_qsh_mdump_collect_irat_wl1x2wsrchdrvmdspsync

DESCRIPTION
  This is a minidump function for dumping the selected MCAL global variables for IRAT searches

DEPENDENCIES
  NONE

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_irat_wl1x2wsrchdrvmdspsync(wsub_id_e_type wsub_id);
#endif

#endif /*#ifndef WL1X2WSRCHMDSPSYNC_H */
