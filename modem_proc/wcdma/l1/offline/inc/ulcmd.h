#ifndef ULCMD_H
#define ULCMD_H

/*===========================================================================
                              U L C M D . H

GENERAL DESCRIPTION
  
EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

Copyright (c) 2000 - 2012 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/ulcmd.h_v   1.15   19 Jul 2002 12:31:52   shawnm  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/ulcmd.h#4 $ $DateTime: 2020/01/28 04:44:11 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
01/28/20    sgt     UL QTA Optimisation
01/10/20   ac      Mini dump framework checkin
04/17/18    ss      Function to check whether DPCH config is done or not at FW
08/16/17    ss     Changes for LTA timeline optimization
04/11/16    sg      W+W: Fix to UE going in offline mode during FACH to DCH transition
12/15/15    ar      When resuming from LTA, ensure that NW signaled PCA is used during PCP
09/04/15    tsk     Update UL QTA state after sending qta stop confirmation to RRC.
08/18/15    ar      Doubling uplink state variables to maintain per subscription state
07/29/15    ar      Ensure that RF Tx exit mode is called on previous device in ul_modify_band_handler
07/08/15    tsk     EFACH + MSIM fixes as part of blanket CR866776
07/07/15    ar      Double the ciphering keys backup to be per subscription to support W+W LTA
07/01/15    tsk     FR 26237: DSDS with Rel.8 E-FACH HSRACH + MSIM Changes.
07/01/15    ar      Changes to move Tx enter/exit mode to UL context, along with RF Tx init/deinit
06/26/15    ar      Cleanup RFM Init related code to modularize the functions
06/26/15    vr      Code cleanup to use MACROs for UL/EUL state globals.
06/19/15    gp      Add QXDM command for debug F3 in UL RACH and DCH code-path
06/09/15    ag      Adding code to change 6D entering and exiting threshold via QXDM commands.
04/27/15    vr      QXDM command to crash UE during RACH preamble or message Tx
04/06/15    gp      FR 24883: UL DPCH TX Timeline Reduction
02/25/15    vr      Support for 0x4035 Slot level TXAGC log packet.
02/19/15    vr      UL changes for structure based RF APIs.
02/03/15    vr      Remove inits of some globals & cleanup some unused globals.
01/16/15    vr      Global reorg for W + W.
10/13/14    vr      Changes to skip RFM-INIT and RFM-DECFG when not needed.
10/10/14    ar      Moved CXM frequency ID requests and release to RFM Init and
                    RFM Deconfig separately.
08/11/14    yw      Fix for incorrect blocking PDU building during Sec RLF
08/09/14    gp      Seperation of R99 and EUL variables init for QTA
08/08/14    sks     MCPM driver code cleanup
06/23/14    yw      Ensure no 2 UL DPCCH config command is sent to FW in the same CFN
06/19/14    ash     Backed out code for 0x410E Log Pkt
06/03/14    yw      Mods to support secondary carrier Tx freq/pwr reporting 
05/29/14    raj     Call mcalwcdma_evt_enc_get_data() from task context only if any 
                    UL DPCCH cfg cmd is not send in the current frame.
05/23/14    ash     Added support for graceful handling of RFM INIT TX Failure
05/13/14    ymu     Feature name FEATURE_WCDMA_DC_HSUPA_FRAME replacement with FEATURE_WCDMA_DC_HSUPA 
04/27/14    jkb     Add check for Feature dual sim with feature qta
02/05/13    kcm     Changes to trigger TxLM disable when AICH is suspended in EDRX config
01/22/14    gp      Fix for race condition during QTA cleanup
09/17/13    jkb     Porting Dual Sim from Triton to Dime
08/22/13    gp      Moved UL Maint parameters into new API enc_set_ul_maint_event_parameters()
                    and ensured that UL DPCCH Cfg Cmd is sent within FW Config Window
08/15/13    at      Post UL DPCH cfg disable done command to L1M after UL DPCH disable done 
                    interrupt from FW is received.
08/08/13    yw      Call SetModTiming cmd before send UL DPCCH cfg cmd to FW from WPLT 
07/02/13    yw      Send Sec UL DPCCH disable cfg command from enc_get_data ISR.
07/01/13    pr      Optimizations done to change the UL DPCH Sequence
06/25/13    yw      Support for skipping PDU building during 6-slot interruption peroid
06/19/13    yw      Ensure that DPCCH cfg command for Sec UL and Primary UL is not sent in same frame.
06/04/13    sad     CCTRCH validation moved to ul_cctrch_cfg_validate
05/28/13    ash     UL unused Mutex cleanup
05/22/13    oh      Adding DC-HSUPA framework support
05/09/13    gp      Code cleanup for FEATURE_WCDMA_UL_CM_INTLV_WORKAROUND and FEATURE_WCDMA_CONCURRENT_R99_EUL_TX_ENCODE
04/01/13    gp      Handling race condition of simultaneously sending Tx event disable command 
                    and polling of Tx event due to data availability. 
03/26/13    sad     Changes to handle new RRC-L1 interface.
12/12/12    ash     MCPM changes for Dime to turn off Tx block only when Tx phy chan is disabled.
12/10/12    vr      Fix for compiler warnings when HSRACH feature is enabled
10/26/12    ash     Dime Feature clean up. Mainlined FEATURE_WCDMA_GENESIS_SW_FW flag
07/24/12    geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/19/12    vr      HSRACH code update
06/08/12    gv      Feature HS RACH Initial code check in
03/27/12    hk      Added support for FEATURE_WCDMA_HS_FACH_DRX
02/01/12    raj     Nikel Feature cleanup.
01/31/12    raj     Nickel feature cleanup. Mainlined the features FEATURE_WCDMA_UL_TFC_PL_REV_2
01/27/12    vs      Nikel feature cleanup.
01/20/12    bs      free floating changes.
11/29/11    gnk     Removed some unused features
10/21/11    sp      Nikel UL CM workaround under FEATURE_WCDMA_UL_CM_INTLV_WORKAROUND
04/04/11    oh      Changes to support UL DCH on NikeL
03/30/11    gnk     Mainlined several features
01/17/11    sp      Added support for UL_RACH_DEBUG_LOG_PKT (0x4025)
01/17/11    sp      Featurized UL RACH FW Log Pkt (0x4026) under FEATURE_WCDMA_GENESIS_SW_FW
01/07/11    sp      Added support for new UL RACH FW Log Pkt (0x4026)
                    Removed support for 0x4180, 0x4181, 0x4182
11/19/10    sp      Optimized R99 UL Data Dump Log Pkt implementation
07/30/09    gnk     Mainlining the feature FEATURE_WCDMA_OFFLINE_DEMOD_SUPPORT
07/24/09    ka      Fixed flow control between ENC_GET_DATA ISR and L1 task for 
                    x410E log packet. 
07/17/09    ka      Removed WL1_ERR_FATAL from log buff read code for 0x410E
06/29/09    oh      Mutex protection between UL DCH ISR and UL reconfig in WL1 task.
05/25/09    hk      Fixed compiler warnings
04/22/09    gnk     Fixed compiler warnings
02/13/09    ka      Fixed concurrency issue in processing UL_TRCH_CODING_LOG_PKT
10/03/08    hk      Bringing in 7k Mailine fixes
02/08/08    gnk     Added QXDM support for R99 interface
10/15/07    gnk     Added Prach log packets & F3 msgs temporarily
08/13/07    vp      Moved event handlers from dltimeline to individual modules.
09/27/06    asm     Added Feature UL TFC PL rev 2
08/15/06    asm     Added interface to query TPC Step size
08/14/06    yiz     Mod to mainline HSUPA code
08/23/05    asm     Add support for FEATURE_WCDMA_LOWER_POWER_FOR_AMR
07/26/05    yiz     Add ul_dpcch_pre_reconfig() for UL DPCCH slot format and 
                    scrambling code reconfig at ACT.
07/13/05    gs      Removed the #else parts concerning the flag definition
                    FEATURE_WCDMA_HFN_UPDATE. The #else part contained 
                    the older form of MAC-CFN update mechanism
05/12/05    gs      Externalized UL frame boundry event enabler and event
                    handler function
03/31/05    yiz     Add decl of ul_hfn_update_evt_enabled
03/17/05    yiz     Added interface to trigger dummy BFI for resume.
11/19/04    yiz     Support DYNAMIC_POWER_CLASS
11/09/04    yiz     Add WCDMA_PRACH_SELECTION support.
10/22/04    yiz     Add ul_maintain_dpch_tx_pwr flag for Sync Proc A.
                    Delete FEATURE_DROP_ADD_FOR_PHYCHL_RECONFIG.
10/05/04    yiz     Add UL_DPCH_PWR_INFO_LOG_PKT.
09/30/04    yiz     Add decl's for UL_TFC_PWR_ELMN_LOG_PKT.
08/30/04    gs      Moved definition of macros for channel timing offset from
                    source file to here
                    HSDPA related declarations
08/18/04    yiz     Added ul_asu_cfg_db_copy_cmd() to update ul pointers
                    during ASET update.
11/12/03    m       Added support to suppress end of call spike
08/01/03    gs      Externalized function calc_pre_rm_bits
07/22/03    yiz     Added functions decl to fix re-slam of UL timing on recfg.
04/09/03    scm     Remove FEATURE_WCDMA_COMP_GF.  Feature always supported.
08/22/02    scm     Add ul_teardown_all_active_phch().
08/14/02    sh      Added structure ul_cphy_setup_cmd_type for Uplink
                    channel setup.
07/31/02    gs      Updated the declaration of externalized function
                    ul_compute_gain_factors
07/25/02    scm     Fixing uplink gain factor support during compressed mode.
07/19/02    scm     Uplink gain factor support during compressed mode.
05/29/02    m       Added prototype to support log on demand request for
                    1.  UL_CCTRCH_
                    2.  UL_DPCH
                    3.  UL_RACH
05/18/02    sh      Cell Reselection related changes. Added function
                    ul_cell_trans_drop_done_cmd().
04/12/02    yiz     Added ul_phychan_dpch_log_pkt and ul_log_phychan_dpch_info()
                    to change UL_PHYCHAN_DPCH_LOG_PKT to deferred logging.
01/18/02    gs      1. Externalized the global ul_dpcch_slot_fmt_nm
                    2. Externalized the function ul_set_ciphering_keys
10/29/01    sh      Removed ul_set_mod_req flag.
08/28/01    sk      Changed FEATURE_L1_LOGGING to FEATURE_L1_LOGGING_ENABLED.
08/09/01    sk      Added logging.
06/12/01    sk      Added ul_drop_pending flag.
05/22/01    sk      Added support for SetModTiming command in DL event.
01/12/00    sk      externalized ul_dpcch_slot_fmt.
09/22/00    sk      initial version created.
                    
===========================================================================*/
#include "wcdma_variation.h"
#include "customer.h"
#include "comdef.h"
#include "l1def.h"
#include "l1sapcommon.h"
#include "l1ulcfg.h"
#include "l1rrcif.h"
#include "l1macif.h"
#include "l1task_v.h"
#include "ullog.h"

#include "wl1rfmif.h"

#ifdef FEATURE_WCDMA_MCPM_DRIVER
#include "wmcpmdrv.h"
#endif /* FEATURE_WCDMA_MCPM_DRIVER */

/* DPCH offset */
#define L1_UL_DPCH_OFFSET      4
#define L1_UL_DPCH_OFFSET_Cx8  8192 //4 * 256 * 8 (Tx offset in chipx8)

/* PRACH offsets depending on AICH TT */
#define L1_UL_PRACH_OFFSET0  (-30)
#define L1_UL_PRACH_OFFSET1  (-50)

#define L1_UL_PRACH_DL_REF_OFFSET0  (-30)
#define L1_UL_PRACH_DL_REF_OFFSET1  (-50)
#define L1_UL_DPCCH_DL_REF_OFFSET   (  4)

/*Define the config window for sending DPCCH Cfg Cmd to FW */
#define L1_UL_DPCCH_CFG_WINDOW_START 20
#define L1_UL_DPCCH_CFG_WINDOW_END 120

#define GAIN_FACTORS_ARRAY_SIZE 30 //VAI: HACK [(sizeof(Aj_table) / sizeof(Aj_table[0]))]

#define WL1_UL_DPCH_CONFIG_OPS_INTLOCK()  REX_ISR_LOCK(&ulcmd_ext_cntrl_params.ul_dpch_config_ops_crit_sect)
#define WL1_UL_DPCH_CONFIG_OPS_INTFREE()  REX_ISR_UNLOCK(&ulcmd_ext_cntrl_params.ul_dpch_config_ops_crit_sect)

#define WL1_RFM_TX_INIT_INTLOCK() REX_ISR_LOCK(&ulcmd_ext_cntrl_params.ul_rfm_tx_init_crit_sect);
#define WL1_RFM_TX_INIT_INTFREE() REX_ISR_UNLOCK(&ulcmd_ext_cntrl_params.ul_rfm_tx_init_crit_sect);


typedef struct
{
  boolean slam;    //indicates of DSP should slowly slew or slam to target
  uint8   hw_chan; // active DL h/w chan
  uint32 txPnPositioncx8; // absolute tx Pn Position in Chipx8 units
  int8    dl_ref_offset; // DL ref channel offset DPCCH = 4, RACH: AICH TT 0 = -30, AICH TT 0 = -50
} ul_set_mod_timing_struct_type;

/* Local Command for UL Physical Channel Setup */
typedef struct
{
  l1_local_cmd_hdr_type           hdr;
  l1_ul_phychan_enum_type         phychan_type;
} ul_cphy_setup_local_cmd_type;

extern const uint8 Npilot_c_for_slot_fmt[],Npilot_n_for_slot_fmt[];
/* UL_DATA_DUMP_LOG_PKT global variables declaration */

typedef enum
{
  /* Logs R99 UL DCH DATA before writing to ERAMA. This cmd 
   * is sent from wenc.c write_erama_data handler */
  UL_DATA_DUMP_LOG_PKT_CMD,
  /* Logs WCDMA RACH parameters from FW interfaces */
  UL_FW_RACH_DEBUG_LOG_PKT_CMD,
  /* Logs WCDMA RACH cfg and status parameters. */
  UL_RACH_DEBUG_LOG_PKT_CMD,
  #ifdef FEATURE_WCDMA_HS_RACH
  HS_RACH_CONFIG_LOG_PKT_CMD,
  UL_RACH_ALLOC_EDCH_RES_LOG_PKT_CMD,
  UL_RACH_CHAN_TIMING_LOG_PKT_CMD,
  UL_RACH_PROC_PARAMS_LOG_PKT_CMD,
  #endif /* FEATURE_WCDMA_HS_RACH */
  UL_LOG_CMD_NUM_TYPES
} ul_logging_cmd_enum_type;

/* The structure to accomodate all R99 UL Log packets 
 * that are to be submitted in task context */
typedef struct
{
  l1_local_cmd_hdr_type hdr;
  ul_logging_cmd_enum_type log_pkt_type;
} ul_logging_local_cmd_type;

#ifdef FEATURE_WCDMA_DC_HSUPA
/*For RF retune the below struct will be used to skip
  ** PDU building during interruption */
typedef struct
{
  uint8 num_pdu_built_before_interruption;
  uint8 num_subfn_till_interrption_ends;
}eul_skip_pdu_build_type;

#endif

/* Read TX TTI at time of slamming mod timing to FW */
#define UL_FW_RACH_DEBUG_SET_MOD_TIMING_TTI(value) (wenc_ext_cntrl_params.ul_fw_rach_debug_log_pkt.ul_rach_params_info.tx_tti = value)

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)

#define INVALID_SUB_FRAME_NUM 0xFFFF
#define INVALID_SLOT_NUM 0xFFFF

/* QTA state which will indicate if the QTA is active or not */
typedef enum
{
  UL_QTA_INACTIVE, /* QTA is not active */
  UL_QTA_ACTIVE_IN_FUTURE, /* QTA is going to get activated in future */
  UL_QTA_ACTIVE,    /* QTA is active */
  UL_QTA_ERROR   /* Invalid state which is initialized state */
} ul_qta_state_enum_type;

typedef struct {
  /* Current state of the QTA machine */
  ul_qta_state_enum_type qta_state;
  /* Start CFN of the QTA which is returned by the QTA manager */
  uint16 qta_start_cfn;
  /* Start slot of the QTA which is returned by the QTA manager */
  uint16 qta_start_slot;
  /* EUL Subframe number where the QTA can be started for EUL*/
  uint16 qta_eul_active_subfn;
} wl1_ul_qta_struct_type;

#endif /* FEATURE_QTA */

typedef enum
{
  CRASH_DISABLED,
  CRASH_IN_RACH_PREAMBLE_TX,
  CRASH_IN_RACH_MESSAGE_TX,
  CRASH_NUM_TYPES
} ul_debug_rach_crash_type;

/******************************************************************************
*******************************************************************************
              COMMAND HANDLERS FOR PHYSICAL CHANNEL MANAGEMENT
*******************************************************************************
******************************************************************************/
/*--------------------------------------------------------------------------
                  CMD STRUCTURE: UL_CPHY_SETUP_CMD_TYPE
                  
This is the generic command structure used for Uplink physical channel add,
cfg, and drop command.
--------------------------------------------------------------------------*/

typedef struct
{
  /* request mask */
  uint32 req_mask;

  /* data for CPHY_UL_CCTRCH_REQ */
  l1_ul_cctrch_cfg_struct_type*  ul_cctrch_info;


  /* data for CPHY_UL_PHYCHAN_DROP_REQ */
  l1_ul_phychan_enum_type       drop_ul_phychan;

  /*
   * data for CPHY_UL_PHYCHAN_CFG_REQ/CPHY_UL_PHYCHAN_ADD_REQ.  cfg_ul_phychan is the uplink physical
   * channel to be reconfigured.  It is assumed that it will be reconfigured
   * to use the CCTrCH referred to in the CPHY_UL_CCTRCH_REQ also included
   * in the command packet
   */
  l1_ul_phychan_enum_type        ul_phychan;
  l1_ul_phychan_cfg_struct_type* ul_phychan_db;

  mac_ul_tfcs_type  *tfcs_ptr;

  boolean prep_hs_beta_table;

  l1_e_sec_dpcch_info_struct_type sec_dpcch_info;

} ul_cphy_setup_cmd_type;

typedef enum
{
  UL_DEBUG_DISABLE_HDET =2,
  UL_DEBUG_DPCH_INIT_TX_PWR_AGC=6,
  UL_DEBUG_RACH_NO_ACK = 7,
  UL_DEBUG_SET_DPCCH_PWR_VAL =16,
  UL_DEBUG_FREEZE_TX_ACCUM = 17,
  UL_DEBUG_SET_RACH_PREAMBLE_PWR_VAL =18,
  /* UL_DATA_DUMP Log Pkt */
  UL_DEBUG_ENABLE_DATA_DUMP_LOG_VAL = 19,
  
  #ifdef FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
  /* Change the period for Tdown of Duty cycle timer  */
  UL_DEBUG_SET_THERM_BACKOFF_TDOWN,
  /* Change the period for Tup of Duty cycle timer */
  UL_DEBUG_SET_THERM_BACKOFF_TUP,
  /* Change the Step timer val  */
  UL_DEBUG_SET_THERM_BACKOFF_STEP,
  /* Initial value by which to reduce MTPL */
  UL_DEBUG_SET_THERM_BACKOFF_MTPL_INIT_DB,
  #endif /* FEATURE_WCDMA_MTPL_THERMAL_BACKOFF */

  #ifdef FEATURE_WCDMA_HS_RACH
  UL_DEBUG_HSRACH_DPCH_INIT_TX_PWR_DBM = 35,
  UL_DEBUG_HSRACH_PANIC_SW_FW = 37,
  #endif /* FEATURE_WCDMA_HS_RACH */

  /* wcdma query tx power dbm QXDM debug */
  UL_DEBUG_WCDMA_QUERY_TX_POWER_DBM,
  UL_DEBUG_SET_RACH_DPCCH_PWR_FREEZE_TX_ACCUM = 40,
  UL_DEBUG_CIPH_KEYS_RESTOR_TX_SET,
  UL_CONFIG_XY_CPC_QTA,
  UL_CONFIG_GRANT_MONITOR_CPC_QTA,
  UL_DEBUG_ENABLE_TX_TIME_PROFILE,
  UL_DEBUG_ERAM_IRAM_DUMP_CONTROL,
  UL_DEBUG_RACH_PREAMBLE_MESSAGE_TX = 46,
  UL_DEBUG_BFI_OPTIMIZATION = 47,
  UL_DEBUG_6D_THRESH_SETTING = 50,
  /*UL QTA Opts control*/
  UL_DISABLE_QTA_OPTS              = 51,
  UL_DEBUG_NUM_SWITCH_TYPES
} ul_debug_switch_type;

typedef enum
{
  WENC_DEBUG_TX_PWR_DISABLE,
  WENC_DEBUG_DPCCH_PWR,
  WENC_DEBUG_RACH_PWR,
  WENC_DEBUG_BOTH_RACH_DPCCH_PWR
} debug_pwr_values_enum_type;

#ifdef FEATURE_WCDMA_RX_TX_SPLIT
/* Enumeration to specify the action for modify band */
typedef enum {
  /* We are moving to neighbour PCCPCH, hence exiting current Tx device */
  UL_MODIFY_BAND_EXIT_TX,
  /* We are returning to original Tx device after failure, hence need to enter Tx device */
  UL_MODIFY_BAND_ENTER_TX,
  UL_MODIFY_BAND_NUM_STATES
}ul_modify_band_e_type;
#endif

/* Enumeration for RF Tx init state */
typedef enum
{
  /* We are currently deconfigured */
  ULCMD_RFM_INIT_STATE_DECFGED,

  /* We are currently initialized in SC mode */
  ULCMD_RFM_INIT_STATE_SC_INIT,

  /* We are currently initialized in DC mode */
  ULCMD_RFM_INIT_STATE_DC_INIT,
  ULCMD_RFM_INIT_STATE_MAX_STATES
} ulcmd_rfm_init_state_e_type;

typedef struct
{
  /* variable to check if RFM Init failed during Channel Add */
  boolean wl1_rfm_init_failed;
  /* Flag to indicate whether DPCH is configured(active) at FW or not */
  boolean ul_dpch_config_done;
  /* UL Phychan drop pending flag */
  boolean ul_drop_pending;
  boolean ul_tx_evt_disable_in_progress;
  /*Flag to indicate that dpcch config command should be sent to FW in enc_ul_maint_event_handler */
  boolean ul_send_dpcch_cmd_to_mdsp_in_ul_handler_evt;
  /* indicate encoder UL HS beta table computaion is required */
  boolean enchs_beta_table_prep_reqd;
  boolean wl1_ul_skip_decfg_tx_flag[WCDMA_NUM_SUBS];
  /* indicate if UL DPCH is in reconfig */
  boolean ul_reconfig;
  /* Contains parameters required for issuing mDSP SetModTiming command */
  ul_set_mod_timing_struct_type ul_set_mod_timing;
  /* UL DPCCH CFG done flag */
  boolean ul_dpcch_cfg_done;
  
  #if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
	/* Boolean which will return if the EUL module is in middle of the QTA gap or not */
  boolean wl1_ul_eul_qta_active;
	/* if we dont build the PDU then skip the qta event handler else dont skip*/
  boolean wl1_ul_qta_skip_maint_evt_hdlr;
  /* Flag for MAC trigger in FACH QTA */
  boolean wl1_ul_rach_trigger_during_qta;
  #endif /* defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA) */

} ulcmd_ext_cntrl_params_mdump_type;

extern ulcmd_ext_cntrl_params_mdump_type ulcmd_ext_cntrl_params_mdump;


typedef struct
{
  ulcmd_ext_cntrl_params_mdump_type ulcmd_ext_cntrl_params_mdump;
  /*Mutex for avoiding concurrent execution of RFM TX init in DPC and WL1 task context*/
  rex_crit_sect_type  ul_rfm_tx_init_crit_sect;  
   /*Mutex for the protection of UL DPCH configuration in ISR and task context*/
  rex_crit_sect_type  ul_dpch_config_ops_crit_sect;      
  /* variable to check if ul dpch configuration in on going*/
  boolean ul_dpch_config_in_progress;
  /* variable to check if any cphy is pending on the UL dpch configuration*/
  boolean cphy_pending_on_ul_dpch_config;
  /*Slot format that is to be used in the UL dpcch config cmd that is sent in ul maintenence event handler */
  uint8 ul_dpcch_slot_fmt_for_ul_event_handler;
  uint8 previous_cfn;
  /* This flag indicates whether there is any mDSP UL DPCCH Config command is 
  ** sent during the current frame and prevent sending any other DPCCH cfg cmd
  ** in the same frame. */
  boolean ul_dpcch_cfg_send;
  int32 ul_previous_ref_tfci;
  uint8 ul_phychan_db_idx;
  uint8 ul_dpcch_slot_fmt_nm;
  /* Keep pointer to TFCS table around. */
  mac_ul_tfcs_type *ul_tfcs_ptr;
  /* minimum RM */
  uint16 rm_min;
  /* Indicate whether to keep DPCCH TX power in Sync Proc A */
  boolean ul_maintain_dpch_tx_pwr;
  /* DPCCH slot format */
  uint8 ul_dpcch_slot_fmt;
  uint64 Aj_table_adjusted[GAIN_FACTORS_ARRAY_SIZE];

  #ifdef FEATURE_WCDMA_REL6
  boolean DEBUG_EUL_FIX_HS_DPCCH;
  #endif /* FEATURE_WCDMA_REL6 */

  #if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
  /* Structure which will hold all the QTA related parameters */
  wl1_ul_qta_struct_type wl1_ul_qta_params;
  #endif /* defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA) */

  /*Mutex between UL DCH ISR and UL reconfig in WL1 task*/
  rex_crit_sect_type l1_ul_dch_reconfig_isr_mutex;

  #ifdef FEATURE_L1_LOG_ON_DEMAND
  ul_phychan_prach_log_on_demand_struct_type ul_prach_log_pkt;
  /* following two variables could not be initialized in any of the
  ** functions in this file. Hence they will be left as it is. Since
  ** they are global varibles they will be initialized to 0 anyway */
  ul_phychan_dpch_log_on_demand_struct_type  ul_dpch_log_pkt;
  #endif /* FEATURE_L1_LOG_ON_DEMAND */

  UL_RACH_PROC_LOG_PKT_type     ul_rach_proc_log_pkt;
  UL_DPCH_PWR_INFO_LOG_PKT_type ul_dpch_pwr_info_log_pkt;
  UL_TFC_PWR_ELMN_LOG_PKT_type  ul_tfc_pwr_elmn_log_pkt;
  UL_PHYCHAN_DPCH_LOG_PKT_type  ul_phychan_dpch_log_pkt;

  #ifdef FEATURE_WCDMA_HS_RACH
  UL_HSRACH_CONFIG_LOG_PKT_type          ul_hsrach_config_log_pkt;
  UL_HSRACH_CHAN_TIMING_LOG_PKT_type     ul_hsrach_chan_timing_log_pkt;
  UL_HSRACH_ALLOC_EDCH_RSRC_LOG_PKT_type ul_hsrach_alloc_edch_rsrc_log_pkt;
  UL_HSRACH_PREAMBLE_STATUS_LOG_PKT_type ul_hsrach_preamble_status_log_pkt;
  #endif /* FEATURE_WCDMA_HS_RACH */

  #ifdef FEATURE_WCDMA_DC_HSUPA
  eul_skip_pdu_build_type eul_skip_pdu_build;
  #endif /* FEATURE_WCDMA_DC_HSUPA */
} ulcmd_ext_cntrl_params_type;

extern ulcmd_ext_cntrl_params_type ulcmd_ext_cntrl_params;

typedef struct
{
  /* ciphering key data backup. Used to program cipher keys after coming back from Power collapse*/
  l1_cipher_key_info_struct_type ul_ciphering_key_data_backup;
  /* For non-LTA cases this will always be hard-coded to PCA1, for LTA this will be NW singaled PCA*/
  l1_ul_pca_enum_type            pca_to_use_during_pcp;
} ulcmd_ext_persub_cntrl_params_type;

extern ulcmd_ext_persub_cntrl_params_type ulcmd_ext_persub_cntrl_params[WCDMA_NUM_SUBS];

typedef struct
{
  /* Flag to disable unwanted F3's in UL DCH and RACH code-path */ 
  boolean debug_code_path_flag;
  /* Flag to enable/disable HDET correction */
  boolean wenc_debug_disable_hdet_correction;
  /* Flag to set through QXDM for crashing UE when a certian no of RACH No-ACKs are seen */
  boolean enc_enable_fw_sw_crash;
  /* Counters to indicate the no. of RACH No Acks that must be seen in succession in order to crash the UE
  ** if the QXDM command is sent */
  uint8   debug_no_ack_ctr_max;

  #ifdef FEATURE_WCDMA_HS_RACH
  int16   wenc_debug_hsrach_dpch_init_tx_pwr_dbm;
  boolean hsrach_debug_crash_sw_fw;
  #endif /* FEATURE_WCDMA_HS_RACH */

  /* Variable used to set the init dpcch pwr and the Rach pwr */
  debug_pwr_values_enum_type wenc_debug_tx_pwr_type;
  int16  debug_dpcch_tx_pwr_value;
  uint8  wenc_debug_freeze_tx_accum;
  int16  debug_rach_tx_pwr_value;
  /* The flag set by QXDM CMD to enable/disable UL_DATA_DUMP Logging */
  boolean ul_data_log_qxdm_config_flag;

  #ifdef FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
  uint32 wenc_thermal_backoff_dutycycle_tdown_period_ms;
  uint8  wenc_thermal_backoff_mtpl_init_val_db;
  uint32 wenc_thermal_backoff_step_timer_period_ms;
  uint32 wenc_thermal_backoff_dutycycle_tup_period_ms;
  #endif /* FEATURE_WCDMA_MTPL_THERMAL_BACKOFF */

  /* Debug variable for controlling restoring of
  ** ciphering keys at every RF tx enable */
  uint32 wenc_debug_restore_ciph_keys_rf_tx_enable;

  #if defined(FEATURE_QTA) && defined(FEATURE_WCDMA_CPC_DTX)
  /* UE to dis-allow QTA for CFN-X to CFN+ Y duration where CFN is the CFN where enabling delay is expired.\
  ** In order to manintain this, introduced below two glabal variables and setting these to 8 and -4 respectively\
  ** as per sys requirements */
  uint16 e_ul_cpc_dtx_qta_block_X;
  int16  e_ul_cpc_dtx_qta_block_Y;
  #endif /* defined(FEATURE_QTA) && defined(FEATURE_WCDMA_CPC_DTX) */

  #if defined(FEATURE_QTA) && defined(FEATURE_WCDMA_CPC_DRX)
  /* 20 TTIs x 2ms = 40ms for 2ms TTI */
  uint16 e_ul_cpc_drx_grant_monitor_2ms;
  /* 4 TTIs x 10ms = 40ms for 10ms TTI */
  uint16 e_ul_cpc_drx_grant_monitor_10ms;
  #endif /* defined(FEATURE_QTA) && defined(FEATURE_WCDMA_CPC_DRX) */

  /* Change initial value of this var to TRUE after FC */
  boolean debug_qxdm_ul_isr_tmg_disabled;

  #if defined (FEATURE_WCDMA_BOLT_2_0) && !defined (FEATURE_WCDMA_JOLOKIA_MODEM)
  #ifdef DEBUG_ERAMA_RMRAM_DATA_DUMP
  boolean debug_read_erama_eramb_flag;
  #endif /* DEBUG_ERAMA_RMRAM_DATA_DUMP */
  #endif /* defined (FEATURE_WCDMA_BOLT_2_0) && !defined (FEATURE_WCDMA_JOLOKIA_MODEM) */
  boolean disable_qta_opts;

} ulcmd_qxdm_cntrl_params_type;

extern ulcmd_qxdm_cntrl_params_type ulcmd_qxdm_cntrl_params;

#define GET_ULCMD_DIS_HDET_FLAG()   (ulcmd_qxdm_cntrl_params.wenc_debug_disable_hdet_correction)

#define GET_ULCMD_ENC_FW_CRSH_ENA() (ulcmd_qxdm_cntrl_params.enc_enable_fw_sw_crash)
#define GET_ULCMD_RACH_NOACK_CMAX() (ulcmd_qxdm_cntrl_params.debug_no_ack_ctr_max)

#ifdef FEATURE_WCDMA_HS_RACH
#define GET_ULCMD_HSRACH_DIP_DBG()  (ulcmd_qxdm_cntrl_params.wenc_debug_hsrach_dpch_init_tx_pwr_dbm)
#define GET_ULCMD_HSRACH_CRSH_FW()  (ulcmd_qxdm_cntrl_params.hsrach_debug_crash_sw_fw)
#endif /* FEATURE_WCDMA_HS_RACH */

#define GET_ULCMD_DBG_TX_PWR_TYPE() (ulcmd_qxdm_cntrl_params.wenc_debug_tx_pwr_type)
#define GET_ULCMD_DBG_DPCH_TX_PWR() (ulcmd_qxdm_cntrl_params.debug_dpcch_tx_pwr_value)

#define GET_ULCMD_DBG_TX_FRZ_ACCU() (ulcmd_qxdm_cntrl_params.wenc_debug_freeze_tx_accum)

#define GET_ULCMD_DBG_RACH_TX_PWR() (ulcmd_qxdm_cntrl_params.debug_rach_tx_pwr_value)

#define GET_ULCMD_DATA_LOG_FLAG()   (ulcmd_qxdm_cntrl_params.ul_data_log_qxdm_config_flag)

#define GET_ULCMD_UL_DBG_DCH_RACH_CODEPATH() (ulcmd_qxdm_cntrl_params.debug_code_path_flag)
#define SET_ULCMD_UL_DBG_DCH_RACH_CODEPATH(x) (ulcmd_qxdm_cntrl_params.debug_code_path_flag = x)

#ifdef FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
#define GET_ULCMD_THRBKOF_DCYC_TDN()  (ulcmd_qxdm_cntrl_params.wenc_thermal_backoff_dutycycle_tdown_period_ms)
#define SET_ULCMD_THRBKOF_DCYC_TDN(x) (ulcmd_qxdm_cntrl_params.wenc_thermal_backoff_dutycycle_tdown_period_ms = x)

#define GET_ULCMD_THRBKOF_MTPL_INI()  (ulcmd_qxdm_cntrl_params.wenc_thermal_backoff_mtpl_init_val_db)
#define SET_ULCMD_THRBKOF_MTPL_INI(x) (ulcmd_qxdm_cntrl_params.wenc_thermal_backoff_mtpl_init_val_db = x)

#define GET_ULCMD_THRBKOF_STP_TMRP()  (ulcmd_qxdm_cntrl_params.wenc_thermal_backoff_step_timer_period_ms)
#define SET_ULCMD_THRBKOF_STP_TMRP(x) (ulcmd_qxdm_cntrl_params.wenc_thermal_backoff_step_timer_period_ms = x)

#define GET_ULCMD_THRBKOF_DCYC_TUP()  (ulcmd_qxdm_cntrl_params.wenc_thermal_backoff_dutycycle_tup_period_ms)
#define SET_ULCMD_THRBKOF_DCYC_TUP(x) (ulcmd_qxdm_cntrl_params.wenc_thermal_backoff_dutycycle_tup_period_ms = x)
#endif /* FEATURE_WCDMA_MTPL_THERMAL_BACKOFF */

#define GET_ULCMD_CIPH_KEYS_TX_ENA()  (ulcmd_qxdm_cntrl_params.wenc_debug_restore_ciph_keys_rf_tx_enable)
#define SET_ULCMD_CIPH_KEYS_TX_ENA(x) (ulcmd_qxdm_cntrl_params.wenc_debug_restore_ciph_keys_rf_tx_enable = x)

#if defined(FEATURE_QTA) && defined(FEATURE_WCDMA_CPC_DTX)
#define GET_ULCMD_CPC_DTX_QTA_BLKX()  (ulcmd_qxdm_cntrl_params.e_ul_cpc_dtx_qta_block_X)
#define SET_ULCMD_CPC_DTX_QTA_BLKX(x) (ulcmd_qxdm_cntrl_params.e_ul_cpc_dtx_qta_block_X = x)

#define GET_ULCMD_CPC_DTX_QTA_BLKY()  (ulcmd_qxdm_cntrl_params.e_ul_cpc_dtx_qta_block_Y)
#define SET_ULCMD_CPC_DTX_QTA_BLKY(y) (ulcmd_qxdm_cntrl_params.e_ul_cpc_dtx_qta_block_Y = y)
#endif /* defined(FEATURE_QTA) && defined(FEATURE_WCDMA_CPC_DTX) */

#if defined(FEATURE_QTA) && defined(FEATURE_WCDMA_CPC_DRX)
#define GET_ULCMD_CPC_DRX_GM_2MS()   (ulcmd_qxdm_cntrl_params.e_ul_cpc_drx_grant_monitor_2ms)
#define SET_ULCMD_CPC_DRX_GM_2MS(x)  (ulcmd_qxdm_cntrl_params.e_ul_cpc_drx_grant_monitor_2ms = x)

#define GET_ULCMD_CPC_DRX_GM_10MS()  (ulcmd_qxdm_cntrl_params.e_ul_cpc_drx_grant_monitor_10ms)
#define SET_ULCMD_CPC_DRX_GM_10MS(x) (ulcmd_qxdm_cntrl_params.e_ul_cpc_drx_grant_monitor_10ms = x)
#endif /* defined(FEATURE_QTA) && defined(FEATURE_WCDMA_CPC_DRX) */

#define GET_ULCMD_UL_ISR_TMG_FLAG()  (ulcmd_qxdm_cntrl_params.debug_qxdm_ul_isr_tmg_disabled)

#if defined (FEATURE_WCDMA_BOLT_2_0) && !defined (FEATURE_WCDMA_JOLOKIA_MODEM)
#ifdef DEBUG_ERAMA_RMRAM_DATA_DUMP
#define GET_ULCMD_RD_ERAMA_DMP_FLAG() (ulcmd_qxdm_cntrl_params.debug_read_erama_eramb_flag)
#endif /* DEBUG_ERAMA_RMRAM_DATA_DUMP */
#endif /* defined (FEATURE_WCDMA_BOLT_2_0) && !defined (FEATURE_WCDMA_JOLOKIA_MODEM) */
#define GET_ULCMD_IS_QTA_OPTS_DISABLED()           (ulcmd_qxdm_cntrl_params.disable_qta_opts)

/*mdump_chk*/
typedef struct
{
  /* indicate setup command done delay is applicable */
  boolean ul_delay_setup_done_cmd;
  /* RF tx init state */
  ulcmd_rfm_init_state_e_type ul_rfm_init_state;
} ulcmd_int_cntrl_params_mdump_type;


typedef struct
{
  /* Type of UL setup (ADD/RECONFIG) */
  l1_local_cmd_enum_type ul_setup_done_cmd_type;
  /* UL channel setup that was setup */
  l1_ul_phychan_enum_type ul_setup_done_chan_type;
  /* UL log header pointer */
  ul_trch_log_hdr_struct_type *ul_trch_hdr_ptr;
  uint8   free_cctrch_idx;

  /* log packets */
  UL_CCTRCH_LOG_PKT_type ul_cctrch_log_pkt;
  #ifdef FEATURE_L1_LOG_ON_DEMAND
  boolean ul_cctrch_log_pkt_valid;
  #endif /* FEATURE_L1_LOG_ON_DEMAND */

  /* Per slot TxAGC log packet */
  UL_PER_SLOT_TXAGC_LOG_PACKET_type ul_per_slot_txagc_log_pkt;
  uint8  ul_per_slot_txagc_pkt_event_cfg_status_buf;
  /* Global to save Filtered Tx power for logging */
  uint16 ul_per_slot_agc_filtered_pwr[4];
  ulcmd_int_cntrl_params_mdump_type ulcmd_int_cntrl_params_mdump;
} ulcmd_int_cntrl_params_type;

/*--------------------------------------------------------------------------
                  STRUCTURE: UL_PHYCHAN_ADD_CMD_TYPE
                  
This structure contains the parameters for the ul_phychan_add_cmd().
--------------------------------------------------------------------------*/
typedef ul_cphy_setup_cmd_type ul_phychan_add_type;

extern boolean get_bit_pos(uint16 bit_str, uint8  avail_idx, uint8  *bit_pos, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_PHYCHAN_ADD_VALIDATE

DESCRIPTION     This function validates that the addition of the physical
                channel as specified by the parameters referenced by the
                command can be carried out successfully, i.e. that hardware
                resources are available and the parameters are valid.

DEPENDENCIES    The physical channel database and command parameters

RETURN VALUE    TRUE if the command can be carried out, FALSE otherwise

SIDE EFFECTS    None
===========================================================================*/
extern boolean ul_phychan_add_validate
(
  ul_phychan_add_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_CCTRCH_RECONFIG_TFC_PL

DESCRIPTION     This function adjusts the rateAdj table by clearing away the
                histories of rateAdjs not corresponding to any existing TFCs

DEPENDENCIES    ul_tfc_pwr_elmn_state == UL_TFC_PWR_ELMN_STATE_PAUSE
                The CCTrCh should have been reconfigured

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void ul_cctrch_reconfig_tfc_pl( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_ENABLE_FRAME_BDRY_EVENT

DESCRIPTION     This function is a call back function for mDSP command to
                enable frame boundry event

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void ul_enable_frame_bdry_event(
  /* mDSP command status */
  uint16 status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_FRAME_BOUNDRY_EVENT_HANDLER

DESCRIPTION     This function is call back function for UL frame boundry event

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void ul_frame_boundry_event_handler( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ul_config_done_event

DESCRIPTION     This function sends UL DPCH config done to L1M 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void ul_config_done_event( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        wl1_rfm_init_wcdma_tx_after_lta

DESCRIPTION     This function will INIT RF WCDMA after LTA

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void wl1_rfm_init_wcdma_tx_after_lta(uint32 wsub_id);

/*===========================================================================
FUNCTION        UL_SET_MOD_TIMING_POST_CB

DESCRIPTION     This function is a post call back function for post set mod
                timing. It sends UL set done local command if delay flag is
                set to TRUE. Delay flag is cleared after this call back
                function is done.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    The uplink module will start the process of adding the
                specified physical channel.
===========================================================================*/
extern void ul_set_mod_timing_post_cb(
  /* status of mDSP set mod timing command */
  uint16 status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_PHYCHAN_ADD_CMD

DESCRIPTION     This function starts the procedure for adding a physical
                channel.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    The uplink module will start the process of adding the
                specified physical channel.
===========================================================================*/
extern void ul_phychan_add_cmd
(
  ul_phychan_add_type *cmd
, wsub_id_e_type wsub_id);

/*--------------------------------------------------------------------------
                  STRUCTURE: UL_CCTRCH_CFG_TYPE
                  
This structure contains the parameters for ul_cctrch_cfg
--------------------------------------------------------------------------*/
typedef ul_cphy_setup_cmd_type ul_cctrch_cfg_type;

/*===========================================================================
FUNCTION        UL_CCTRCH_CFG_VALIDATE

DESCRIPTION     This function validates that the reconfiguration of the 
                CCTrCH as specified by the parameters referenced
                in the command can be carried out successfully.

DEPENDENCIES    The CCTrCH parameters

RETURN VALUE    TRUE if the command can be carried out, FALSE otherwise

SIDE EFFECTS    None
===========================================================================*/
extern boolean ul_cctrch_cfg_validate
(
  ul_cctrch_cfg_type *cmd,
  l1_dl_phychan_db_struct_type *dl_phychan_db
, wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION calc_pre_rm_bits

DESCRIPTION
  This function calculates pre-rate matched bits for a given transport
  channel. The number of pre-rate matched bits depends on the CCTrCH
  configuration for the transport channel and TFC.

  This function is based heavily upon ul_pre_rm_bits() and is virtually
  identical.  The difference is this function does not manipulate the
  global array ul_trch_coding_parms[].

DEPENDENCIES
  None.

PARAMETERS
  uint8  trch_id - Transport channel id.
  uint16 tb_size - Transport block size in bits.
  uint8  crc     - CRC size.
  uint8  num_tb  - Number of transport blocks.
  l1_ul_coding_enum_type  - code_type,
  l1_tti_enum_type        - tti

RETURN VALUE
  Number of pre-rate matched bits.

SIDE EFFECTS
  None.

=========================================================================*/
extern uint32 calc_pre_rm_bits(uint8  trch_id, 
                               uint16 tb_size, 
                               uint8  crc,
                               uint8  num_tb,
                               l1_ul_coding_enum_type code_type,
                               l1_tti_enum_type tti, wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION ul_compute_gain_factors

DESCRIPTION
  This function computes the control and data gain factors Bc_j and Bd_j
  as described in 3GPP TS 25.214 V3.7.0 (2001-06), 5.1.2.5.3 Computed gain
  factors.

  These computed gain factors for the passed-in TFC use the signalled gain
  factors of a reference TFC in the computation.  A TFCS may contain up to
  4 reference TFCs, as shown in 3GPP TS 25.331 V3.7.0 (2001-06),
  10.3.5.8 Power Offset Information.

DEPENDENCIES
  This function shall only be called for the following three scenarios:
  - Normal (Non-Compressed) Mode / Computed Gain Factors
  - Compressed Mode / Signalled Gain Factors
  - Compressed Mode / Computed Gain Factors

  It shall NOT be called for:
  - Normal (Non-Compressed) Mode / Signalled Gain Factors

PARAMETERS
  uint32   ui32_tfci - TFC to compute gain factors on.
  uint32  *ui32_Bc_j - Returns Bc_j (control gain factor) to caller.  1 to 15.
  uint32  *ui32_Bd_j - Returns Bd_j (data gain factor) to caller.  0 to 15.
  boolean  f_compressed_mode - Whether or not to compute compressed mode gain factors.
                               Only used with FEATURE_CM_SUPPORTED defined.
  uint32  *deltaPc   - Returns the power control rate Adj In delta. Valid
                       only for power control
  l1_ul_cctrch_cfg_struct_type* cfg_tab_ptr - the ccTrCh CFG table
  mac_ul_tfcs_type*             tfcs_tab_ptr - the UL TFCS table
  int32  *i32_previous_ref_tfci - cache for faster calc

RETURN VALUE
  None.

SIDE EFFECTS
  May update "Aj_table_adjusted" array if necessary.

===========================================================================*/
extern void ul_compute_gain_factors(uint32 ui32_tfci, uint32 *ui32_Bc_j, uint32 *ui32_Bd_j,
                                    boolean f_compressed_mode, uint32 *deltaPc,
                                    l1_ul_cctrch_cfg_struct_type*  cfg_tab_ptr,
                                    mac_ul_tfcs_type*              tfcs_tab_ptr,
                                    int32 *i32_previous_ref_tfci, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_CCTRCH_CFG_CMD

DESCRIPTION     This function starts the process for reconfiguring an
                active CCTrCH.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    The uplink module will start the process of reconfiguring
                the specified CCTrCH.
===========================================================================*/
extern void ul_cctrch_cfg_cmd
(
  ul_cctrch_cfg_type *cmd
, wsub_id_e_type wsub_id);


/*--------------------------------------------------------------------------
                  STRUCTURE: UL_PHYCHAN_CFG_TYPE
                  
This structure contains the parameters for ul_phychan_cfg
--------------------------------------------------------------------------*/
typedef ul_cphy_setup_cmd_type ul_phychan_cfg_type;

/*===========================================================================
FUNCTION        UL_PHYCHAN_CFG_VALIDATE

DESCRIPTION     This function validates that the reconfiguration of the 
                physical channel as specified by the parameters referenced
                in the command can be carried out successfully, i.e. that
                the channel is currently active and that the parameters are
                valid.

DEPENDENCIES    The physical channel database and command parameters

RETURN VALUE    TRUE if the command can be carried out, FALSE otherwise

SIDE EFFECTS    None
===========================================================================*/
extern boolean ul_phychan_cfg_validate
(
  ul_phychan_cfg_type *cmd
, wsub_id_e_type wsub_id);



/*===========================================================================
FUNCTION        UL_PHYCHAN_CFG_CMD

DESCRIPTION     This function starts the process for reconfiguring an
                active physical channel.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    The uplink module will start the process of reconfiguring
                the specified physical channel.
===========================================================================*/
extern void ul_phychan_cfg_cmd
(
  ul_phychan_cfg_type *cmd
, wsub_id_e_type wsub_id);

/*--------------------------------------------------------------------------
                  STRUCTURE: UL_PHYCHAN_DROP_CMD_TYPE
                  
This structure contains the parameters for the physical channel drop                  
--------------------------------------------------------------------------*/
typedef ul_cphy_setup_cmd_type ul_phychan_drop_type;


/*===========================================================================
FUNCTION        UL_PHYCHAN_DROP_VALIDATE

DESCRIPTION     This function validates that the drop of the physical 
                channel as specified by the parameters referenced by the
                command can be carried out successfully, i.e. that the 
                channel is currently active.
                
DEPENDENCIES    The physical channel database and command parameters

RETURN VALUE    TRUE if the command can be carried out, FALSE otherwise

SIDE EFFECTS    None
===========================================================================*/
extern boolean ul_phychan_drop_validate
(
  ul_phychan_drop_type *cmd
, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        UL_PHYCHAN_DROP_CMD

DESCRIPTION     This function starts the process of dropping an active 
                physical channel.
                
DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    The uplink module will start the process of dropping the
                specified physical channel.
===========================================================================*/
extern void ul_phychan_drop_cmd
(
  ul_phychan_drop_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_CELL_TRANS_DROP_CMD

DESCRIPTION     This function starts the process of dropping a PRACH
                physical channel. This is called when performing a
                cell transition.

DEPENDENCIES    The command parameters

RETURN VALUE    Boolean if a physical channel is being dropped.
                False otherwise.

SIDE EFFECTS    The uplink module will start the process of dropping the
                PRACH if it is already set up.
===========================================================================*/
boolean ul_cell_trans_drop_cmd( wsub_id_e_type wsub_id);
/******************************************************************************
                       COMMAND: UL_TX_CONTROL_CMD
******************************************************************************/
/*--------------------------------------------------------------------------
                       STRUCTURE: UL_TX_CONTROL_CMD_TYPE
                  
This structure contains the parameters for the uplink PRACH transmit control.

--------------------------------------------------------------------------*/
typedef l1_ul_tx_ctrl_req_type ul_tx_control_cmd_type;

/*===========================================================================
FUNCTION        UL_TX_CONTROL_CMD

DESCRIPTION     This function starts the process of enabling or disabling
                the specified physical channel - PRACH/PCPCH.
                
DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    The uplink module will start the process of dropping the
                specified physical channel.
===========================================================================*/
extern void ul_tx_control_cmd
(
  ul_tx_control_cmd_type *cmd
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION UL_SET_CIPHERING_KEYS

DESCRIPTION
  This function validates and sets the ciphering key information for uplink
  ciphering operation.

DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE validate and update result.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean ul_set_ciphering_keys(
  /* DL ciphering info structure */
  l1_cipher_key_info_struct_type *ul_cipher_info, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION UL_VALIDATE_CIPHERING_KEY_REQ

DESCRIPTION
  This function validates the ciphering key information for uplink
  ciphering operation.

DEPENDENCIES
  None

RETURN VALUE
  TRUE or FALSE validate result.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean ul_validate_ciphering_key_req(
  /* DL ciphering info structure to validate the information */
  l1_cipher_key_info_struct_type *ul_cipher_info, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_LOG_PHYCHAN_DPCH_INFO

DESCRIPTION     This function logs DPCH info.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void ul_log_phychan_dpch_info
( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_LOG_PRACH_INFO

DESCRIPTION     This function logs PRACH info.

DEPENDENCIES    The command parameters

RETURN VALUE    None

SIDE EFFECTS    The uplink module will start the process of dropping the
                specified physical channel.
===========================================================================*/
void ul_log_prach_info
( wsub_id_e_type wsub_id);

#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION        ul_process_cctrch_log_on_demand

DESCRIPTION     This function collects all the items required for UL CCTrCH
                logging and submits the log packet to QXDM.
  
DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void ul_process_cctrch_log_on_demand( wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION        UL_PROCESS_DPCH_LOG_ON_DEMAND

FILE NAME       ulcmd.c

DESCRIPTION     This funtion creates a DPCH log packet and submits it
                to QXDM

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void ul_process_dpch_log_on_demand( wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION        UL_PROCESS_RACH_LOG_ON_DEMAND

FILE NAME       ulcmd.c

DESCRIPTION     This funtion creates a RACH log packet and submits it
                to QXDM

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void ul_process_rach_log_on_demand( wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION        UL_ASU_CFG_DB_COPY_CMD

DESCRIPTION     This function copies UL chan cfg for ASU to avoid re-config

DEPENDENCIES    RRC shall make sure NO change in the chan cfg.
                This function shall be called by L1M to replace ASU caused
                re-config. 

RETURN VALUE    None

SIDE EFFECTS    NONE
===========================================================================*/
void ul_asu_cfg_db_copy_cmd(l1_setup_cmd_type* l1_setup_cmd_ptr, wsub_id_e_type wsub_id);

/*====================================================================
FUNCTION: UL_LOG_TFC_PWR_ELMN_PKT

DESCRIPTION:
  This function sends UL_TFC_PWR_ELMN_LOG_PKT to DIAG 

DEPENDENCIES:
  NONE

RETURN VALUE:
  NONE

SIDE EFFECTS:
  NONE
====================================================================*/
void ul_log_tfc_pwr_elmn_pkt( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        UL_LOGGING_CMD_HANDLER

DESCRIPTION     This routine sends R99 uplink module log packets to diag in
                the task context. Based on the type of log pkt, specific log info
                handler will be called.    

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_logging_cmd_handler(ul_logging_cmd_enum_type log_pkt_type, wsub_id_e_type wsub_id);

/*====================================================================
FUNCTION: UL_LOGGING_PUT_LOCAL_CMD

DESCRIPTION:
  This function puts local cmds to WL1 to submit UL log packets.

DEPENDENCIES:
  NONE

RETURN VALUE:
  NONE

SIDE EFFECTS:
  NONE
====================================================================*/
void ul_logging_put_local_cmd (ul_logging_cmd_enum_type ul_log_local_cmd, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_DATA_DUMP_LOG_INFO_HANDLER

DESCRIPTION     This routine sends R99 DCH DATA recorded in write_erama_data()
                to DIAG

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_data_dump_log_info_handler( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_FW_RACH_DEBUG_LOG_INFO_HANDLER

DESCRIPTION     This routine submits RACH FIRMWARE Log Packet to DIAG. 
                This routine is called from ul_logging_cmd_handler() 
                in task context.  

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_fw_rach_debug_log_info_handler( wsub_id_e_type wsub_id);

/*====================================================================
FUNCTION: UL_LOG_RACH_DEBUG_PUT_LOCAL_CMD

DESCRIPTION: This routine puts UL_RACH_DEBUG_LOG_PKT in the local cmd queue.

DEPENDENCIES:
  NONE

RETURN VALUE:
  NONE

SIDE EFFECTS:
  NONE
====================================================================*/
void ul_log_rach_debug_put_local_cmd( wsub_id_e_type wsub_id);

/*====================================================================
FUNCTION: UL_LOG_RACH_DEBUG_INFO_HANDLER

DESCRIPTION: This routine sends UL_RACH_DEBUG_LOG_PKT to DIAG. This routine is
             called from task context.

DEPENDENCIES:
  NONE

RETURN VALUE:
  NONE

SIDE EFFECTS:
  NONE
====================================================================*/
void ul_log_rach_debug_info_handler( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_HS_RACH
/*====================================================================
FUNCTION: hs_rach_config_log_info_handler

DESCRIPTION:
  

DEPENDENCIES:
  NONE

RETURN VALUE:
  NONE

SIDE EFFECTS:
  NONE
====================================================================*/
void hs_rach_config_log_info_handler( wsub_id_e_type wsub_id);

/*====================================================================
FUNCTION: ul_rach_alloc_edch_res_log_info_handler

DESCRIPTION:
  

DEPENDENCIES:
  NONE

RETURN VALUE:
  NONE

SIDE EFFECTS:
  NONE
====================================================================*/
void ul_rach_alloc_edch_res_log_info_handler( wsub_id_e_type wsub_id);

/*====================================================================
FUNCTION: ul_rach_chan_timing_log_info_handler

DESCRIPTION:
  

DEPENDENCIES:
  NONE

RETURN VALUE:
  NONE

SIDE EFFECTS:
  NONE
====================================================================*/
void ul_rach_chan_timing_log_info_handler( wsub_id_e_type wsub_id);

/*====================================================================
FUNCTION: ul_rach_proc_params_log_info_handler

DESCRIPTION:
  

DEPENDENCIES:
  NONE

RETURN VALUE:
  NONE

SIDE EFFECTS:
  NONE
====================================================================*/
void ul_rach_proc_params_log_info_handler( wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_HS_RACH */

#ifdef FEATURE_WCDMA_L1_ZI_OPT

/*===========================================================================
FUNCTION        UL_LOG_MEM_ALLOC_DEALLOC

DESCRIPTION     This routine allocates or deallocates dynamic memory
                based on whether log status is enabled/disabled.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void ul_log_mem_alloc_dealloc(log_code_type code, boolean log_status_flag, wsub_id_e_type wsub_id);

#endif

/*====================================================================
FUNCTION: UL_LOG_DPCH_PWR_INFO_PKT

DESCRIPTION:
  This function sends UL_DPCH_PWR_INFO_LOG_PKT to DIAG 

DEPENDENCIES:
  NONE

RETURN VALUE:
  NONE

SIDE EFFECTS:
  NONE
====================================================================*/
void ul_log_dpch_pwr_info_pkt( wsub_id_e_type wsub_id);

#ifdef FEATURE_UMTS_CIPHERING_ENABLE
/*===========================================================================
FUNCTION        UL_RESUME_BFI_FOR_CIPHER

FILE NAME       ulcmd.c

DESCRIPTION     This funtion is called by L1M at resume to start dummy Build
                Frame Indication in DCH in ordr to update MAC about CFN/HFN 
                for ciphering before UL starts to request TX data.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    UL will send dummy Build Frame Indication in TX_GEN_GET_DATA
                ISR in DCH until UL starts to request TX data.
===========================================================================*/
void ul_resume_bfi_for_cipher( wsub_id_e_type wsub_id);
#endif /* FEATURE_UMTS_CIPHERING_ENABLE */

/*===========================================================================
FUNCTION        UL_DPCCH_PRE_RECONFIG

FILE NAME       ulcmd.c

DESCRIPTION     This funtion is called by SEQ one frame ahead of ACT to
                reconfig UL DPCCH for slot format and scrambling code.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    UL DPCCH will change to new config at ACT
===========================================================================*/
void ul_dpcch_pre_reconfig(l1_ext_cmd_type* ext_cmd, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        UL_GET_DPCH_TPC_STEP_SIZE_DB

FILE NAME       ulcmd.h

DESCRIPTION     This funtion is called to get the value of UL TPC step size
                in dB

DEPENDENCIES    None

RETURN VALUE    UL DPCH TPC step size in dB

SIDE EFFECTS    None
===========================================================================*/
uint8 ul_get_dpch_tpc_step_size_db( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_TX_GEN_MOD_TIMING_EVENT_HANDLER

DESCRIPTION
  This is the event handler for the TX Set Modulator Time Event.
  This function enabled the TX System Time in the mDSP.

  This function will also send local command to start initial search
  when DRX command was received and also sends local command to DRX
  on behalf of SRCHZZ when GSM clocks are enabled during a SIB
  modification update.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void ul_tx_gen_set_mod_timing_event_handler( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_TX_GEN_MOD_TIMING_POST_CB

DESCRIPTION     This function gets called once after Mod timing set is done and 
                         sets the flag dl_tx_dpch_mod_timing_set_ready to TRUE.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void ul_tx_gen_mod_timing_pre_cb ( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        UL_TX_GEN_MOD_TIMING_POST_CB

DESCRIPTION     This function gets called once after Mod timing set is done and 
                         sets the flag dl_tx_dpch_mod_timing_set_ready to TRUE.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void ul_tx_gen_mod_timing_post_cb (uint16 status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_UL_DPCH_CHANNEL_DROP_INTERRUPT_HANDLER

DESCRIPTION     This function is the interrupt handler for the UL DPCH DROP DONE from FW.
                In this function channel drop confirmation will be send to L1M.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void wl1_ul_dpch_channel_drop_interrupt_handler( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1_UL_DPCH_DROP_INTR_TIMEOUT_HANDLER

DESCRIPTION Timeout handler for UL DPCCH drop done interrupt from FW.

DEPENDENCIES DPCCH config disable command should already be issued by SW.

RETURN VALUE None

SIDE EFFECTS Sends FW panic command
===========================================================================*/
extern void wl1_ul_dpch_drop_intr_timeout_handler(timer_cb_data_type data);

/*===========================================================================
FUNCTION  UL_SET_VAR_THROUGH_QXDM

DESCRIPTION
  This function sets the uplink global variables as received
  through diag command. This function is a part of the callback function for 
  diag command dispatch table.
  
DEPENDENCIES
  This function is called when "send_data 75 4 28 0 <val>" is issued from cmd 
  line interface from QXDM

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ul_set_var_through_qxdm( uint8 debug_switch_type, uint32 val , wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_HS_FACH_DRX
/*=======================================================================
FUNCTION        UL_PHYCHAN_PRACH_PRE_MOD_TIMING

DESCRIPTION    This function is called before start of adding the RACH physical channel and again 
               called after the phychan add is done. This is used to indicate the UL DRX entity when 
               the Phychan add is started and when it is completed. The same function is used to 
               indicate the UL DRX entity if we have data/No data through RACH in L1.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
=========================================================================*/
extern void ul_phychan_prach_pre_mod_timing( wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_HS_FACH_DRX */

/*===========================================================================
FUNCTION  UL_TX_ENABLE_RFM_CLOCK

DESCRIPTION
  This function sets tx clock and enables RF as part of NikeL bringup.
  This is a temporary function only for brinup. Should be replaced later
  by wl1_enable_tx_and_clk()
DEPENDENCIES
  This function is called at the time of phychan add (DCH)

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ul_tx_enable_rfm_clock(void);

/*===========================================================================
FUNCTION        ul_phychan_is_ul_chan_active

DESCRIPTION     This function returns the state of UL phy channel,
                whether it is active or not.This will be called before 
                turning off Tx Block

DEPENDENCIES    none

RETURN VALUE    TRUE if either PRACH or DPCH channels are active
                FALSE if neither PRACH or DPCH channels are active

SIDE EFFECTS    None
===========================================================================*/
extern boolean ul_phychan_is_ul_chan_active ( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_COEXISTENCE_SW
/*=========================================================================
FUNCTION wl1ulmcmgr_register_freq_id_through_cxm(carr_idx, chan_active)

DESCRIPTION  This function is called to register freq id to MCS through CXM

DEPENDENCIES  None

 RETURN VALUE void

SIDE EFFECTS
  None.

=========================================================================*/

void wl1ulmcmgr_register_freq_id_through_cxm(wl1_ul_carr_id_enum_type carr_idx, boolean chan_active, wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_WCDMA_DC_HSUPA 
/*===========================================================================
FUNCTION        ul_config_dc_mode

DESCRIPTION     This routine sets the RF in DC mode and sends the TxLM cfg cmd to FW to enable DC HSUPA, 
                          followed DPCCH cfg cmd with WaitforScchorder set as FALSE.


DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_config_dc_mode(void);


/*===========================================================================
FUNCTION        ul_perform_txlm_rf_tune_ops

DESCRIPTION     This routine sets the RF in SC/DC mode and sends TxLM cfg cmd to 
                FW to enable SC/DC HSUPA. This function is called when HS SCCH
                order with activate or deactivate is received.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/

extern void ul_perform_txlm_rf_tune_ops (boolean, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ul_order_rcvd_act_deact_sec

DESCRIPTION     This routine indicates to UL that an order came to act/deact 
                DC HSUPA. PDU building can be ignored for the interruption period.

DEPENDENCIES    Called by DL module when HS SCCH order is rcvd to act/deact Sec UL.

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/

extern void ul_order_rcvd_act_deact_sec(boolean is_scch_ord_rcvd, wsub_id_e_type wsub_id);


#endif /*FEATURE_WCDMA_DC_HSUPA */

/*===========================================================================
FUNCTION        ul_set_mod_timing_cmd_from_wplt

DESCRIPTION     This func send FW set modulator timing cmd in WPLT mode from ul_phychan_add
                           and maint event handler.

DEPENDENCIES    Called in WPLT mode only

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/

extern void ul_set_mod_timing_cmd_from_wplt( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ul_send_txlm_disable_cfg_cmd

DESCRIPTION     This func sends the TxLM disable cfg to FW

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void ul_send_txlm_disable_cfg_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_UL_SKIP_DECFG_TX_CHECK

DESCRIPTION     This function sets the flag to skip wl1_rfm_decfg_tx() in the
                following scenarios.
                1) Forced SyncA
                2) FACH to DCH transition if not in HHO.
                3) INTRA FREQ HHO && HHO SUSPEND NEEDED is FALSE

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    wl1_rfm_decfg_tx() will not be called.
===========================================================================*/
void wl1_ul_skip_decfg_tx_check( wsub_id_e_type wsub_id);

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
/*===========================================================================
FUNCTION       WL1_UL_INIT_QTA_VARS_EUL

DESCRIPTION    This function will reset all the QTA related variables and 
               flush EDL results if the flag skip_eul_qta_init is not set. 
  
DEPENDENCIES   None
 
RETURN VALUE   Will reset all the EUL related QTA variables
 
SIDE EFFECTS   None
===========================================================================*/
extern void wl1_ul_init_qta_vars_eul(boolean skip_eul_qta_init, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION       WL1_UL_INIT_QTA_VARS_R99

DESCRIPTION    This funciton will reset all the QTA related variables and skip
               freeing of DSM items based on skip_dsm_free flag. MAC will free DSM 
               items in RACH so skip_dsm_free flag will be set to TRUE. In RACH abort case 
               due to QTA or any other reason, WL1 will send L1_PRACH_ABORT indication
               for MAC to free DSM items. For QTA in DCH case, skip_dsm_free flag will
               be set to FALSE since WL1 does DSM item free.
  
DEPENDENCIES   None
 
RETURN VALUE   Will reset all the R99 related QTA variables

SIDE EFFECTS   None
===========================================================================*/
extern void wl1_ul_init_qta_vars_r99(boolean skip_dsm_free, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_UL_PROCESS_ULTX_POST_QTA_GAP

DESCRIPTION  This funciton will update qta state variables after coming out
             of QTA gap.

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS Update CFN number and the slot number
===========================================================================*/
extern void wl1_ul_process_post_qta_gap(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WL1_UL_QUERY_QTA_MANAGER

DESCRIPTION This funciton will query the QTA manager and will get the future CFN and the slot number
where the QTA is going to be started
  
DEPENDENCIES None
 
RETURN VALUE  Will update the CFN number and the slot number

SIDE EFFECTS  None
===========================================================================*/
extern void wl1_ul_query_qta_manager( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_ul_frames_to_qta_active

DESCRIPTION  This funciton will tell the QTA manager the number of Frames 
             ahead QTA has to start.This info. is used to add diff. 
             overheads to start QTA:
                1 Frame(10ms)  - EUL (10ms/2ms TTI's)
                2 Frame(20ms)  - R99 
  
DEPENDENCIES None
 
RETURN VALUE Will update the CFN number and the slot number

SIDE EFFECTS None
===========================================================================*/

extern uint8 wl1_ul_frames_to_qta_active( wsub_id_e_type wsub_id);

#endif /* FEATURE_QTA */

/*===========================================================================
FUNCTION        ul_per_slot_agc_log_flush_handler

DESCRIPTION     This function flushes the per slot TxAGC log packet to diag

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_per_slot_agc_log_flush_handler(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ul_per_slot_agc_log_handler

DESCRIPTION     This function logs the per slot txAGC values into internal 
                database

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_per_slot_agc_log_handler(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ul_per_slot_agc_save_filtered_pwr

DESCRIPTION     This function will save the filtered values provided by 
                the internal measurements module 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_per_slot_agc_save_filtered_pwr(int16 filtered_txagc_per_evt[], wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ulcmd_rfm_decfg_wcdma_tx

DESCRIPTION     This function Deconfigures the RF TX chain for each carrier 

DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    Tx Chain for the corresponding carrier will be deconfigured 
===========================================================================*/
void ulcmd_rfm_decfg_wcdma_tx( wl1_ul_device_to_enter_exit_e_type device_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ulcmd_rfm_init_wcdma_tx_per_carrier

DESCRIPTION     This function initializes the RF TX chain for each carrier 


DEPENDENCIES    It takes the phychan type as input

RETURN VALUE    None

SIDE EFFECTS    Tx Chain for the corresponding carrier will be initialized and Tx chain may be enabled
===========================================================================*/
void ulcmd_rfm_init_wcdma_tx_per_carrier(
  wl1_rfcom_tx_multi_carrier_cfg_type rf_carr_cfg,
  boolean enable_tx, 
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ul_rach_debugging_crash_during_tx

DESCRIPTION     This func sets FW async interface to crash UE 
                in middle of RACH preamble or message Tx

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_rach_debug_crash_during_tx(
  ul_debug_rach_crash_type crash_type,
  uint8                    num_preambles,
  uint16                   num_bpg,
  wsub_id_e_type           wsub_id);


/*===========================================================================
FUNCTION        ulcmd_rfm_init_state_handler
  
DESCRIPTION     This function is the state handler for RF Tx initialization 
                handling.

DEPENDENCIES    

RETURN VALUE    Whether the RF init operation succeeded or not.

SIDE EFFECTS    RF Tx might be initialized or de-initialized and Tx device might 
                be entered or exited 
===========================================================================*/
void ulcmd_rfm_init_state_handler(
  ulcmd_rfm_init_state_e_type new_state, 
  wl1_rfcom_tx_multi_carrier_cfg_type rf_carr_cfg,
  boolean *rfm_succ,
  wl1_ul_device_to_enter_exit_e_type device_type,
  wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        ulcmd_rfm_init_wcdma_error_handler

DESCRIPTION     This function takes the UE to offline mode for RF Tx init 
                failures for R99 and SCHSUPA.
                For DCHSUPA it will crasht he UE.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    UE will be placed in offline mode or will crash.

===========================================================================*/
void ulcmd_rfm_init_wcdma_error_handler(
  boolean rf_init,
  wl1_rfcom_tx_multi_carrier_cfg_type rf_init_dc_cfg,
  wsub_id_e_type wsub_id
  );

#ifdef FEATURE_WCDMA_RX_TX_SPLIT
/*===========================================================================
FUNCTION        ul_modify_band_handler

DESCRIPTION     This function ensures that during any cell FACH modify band operations 
                Tx device is exited and decofiged or entered and initialized based on
                scenario.
                When moving to neighbour PCCPCH, we need to exit and deconfig the device
                When returning to serving cell after failure, we need to enter and
                initialize Tx device

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    RF Tx will be initialized or deinitialized and entered or exited based on scenario
  
===========================================================================*/
void ul_modify_band_handler(ul_modify_band_e_type switch_state, wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION        ul_cxm_link_info_update

DESCRIPTION     This function handles the Tx state update to Co-ex manager

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_cxm_link_info_update(boolean enable, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        ulcmd_rfm_init_decfged_state_handler
  
DESCRIPTION     This function is the state handler for RF Tx initialization 
                handling when moving out of DECFGD state
  
DEPENDENCIES    

RETURN VALUE    None.

SIDE EFFECTS    RF Tx might be initialized or de-initialized and Tx device might 
                be entered or exited 
===========================================================================*/
void ulcmd_rfm_init_decfged_state_handler(
  ulcmd_rfm_init_state_e_type new_state, 
  wl1_rfcom_tx_multi_carrier_cfg_type rf_carr_cfg, 
  boolean *rfm_succ, 
  wl1_ul_device_to_enter_exit_e_type device_type,
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ulcmd_rfm_init_sc_init_state_handler
  
DESCRIPTION     This function is the state handler for RF Tx initialization 
                handling when moving out of SC INIT state
  
DEPENDENCIES    

RETURN VALUE    None.

SIDE EFFECTS    RF Tx might be initialized or de-initialized and Tx device might 
                be entered or exited 
===========================================================================*/
void ulcmd_rfm_init_sc_init_state_handler(
  ulcmd_rfm_init_state_e_type new_state, 
  wl1_rfcom_tx_multi_carrier_cfg_type rf_carr_cfg, 
  boolean *rfm_succ, 
  wl1_ul_device_to_enter_exit_e_type device_type,
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ulcmd_rfm_init_dc_init_state_handler
  
DESCRIPTION     This function is the state handler for RF Tx initialization 
                handling when moving out of DC INIT state
  
DEPENDENCIES    

RETURN VALUE    None.

SIDE EFFECTS    RF Tx might be initialized or de-initialized and Tx device might 
                be entered or exited 
===========================================================================*/
void ulcmd_rfm_init_dc_init_state_handler(
  ulcmd_rfm_init_state_e_type new_state, 
  wl1_rfcom_tx_multi_carrier_cfg_type rf_carr_cfg, 
  boolean *rfm_succ, 
  wl1_ul_device_to_enter_exit_e_type device_type,
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ul_dpch_config_ops
  
DESCRIPTION     This function returns the status of whether a CPHY suspend is needed based on the UL-DPCH configuration 
  
DEPENDENCIES    None.

RETURN VALUE    TRUE  - If CPHY can start
                FALSE - If CPHY is suspended

SIDE EFFECTS    None.
===========================================================================*/

extern boolean ul_dpch_config_ops(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        UL_PHYCHAN_IS_UL_DPCH_CONFIG_DONE

DESCRIPTION     This function returns whether DPCH config is done or not at FW

DEPENDENCIES    none

RETURN VALUE    TRUE if UL DPCH channel is configured
                FALSE if UL DPCH channel is configured 

SIDE EFFECTS    None
===========================================================================*/
extern boolean ul_phychan_is_ul_dpch_config_done ( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_EFACH_MSIM
/*===========================================================================
FUNCTION        ulcmd_tx_enable_overlap_with_qta

DESCRIPTION     This function will return TRUE if Tx Enable falls within
                20ms of QTA GAP. Also returns true in QTA gap.

DEPENDENCIES    None

RETURN VALUE    TRUE/FALSE

SIDE EFFECTS    None

===========================================================================*/
boolean ulcmd_tx_enable_overlap_with_qta(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ulcmd_update_post_qta_ul_state

DESCRIPTION     This function will update UL QTA state when coming out of
                QTA gap after the RRC confirmation is send.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    Changes UL QTA state to UL_QTA_INACTIVE
===========================================================================*/
void ulcmd_update_post_qta_ul_state(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        hsrach_ul_enable_tx_after_qta

DESCRIPTION     This function will trigger UL TX in case of EDRX
                after coming out of QTA

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void hsrach_ul_enable_tx_after_qta(wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_EFACH_MSIM */


#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION:      wl1_qsh_mdump_collect_uplink

DESCRIPTION:   UL Minidump

DEPENDENCIES:  None

RETURN VALUE:  NONE

SIDE EFFECTS:  NONE
===========================================================================*/
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_uplink(wsub_id_e_type wsub_id);
#endif
#endif  /* ULCMD_H */
