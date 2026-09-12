#ifndef WENC_H
#define WENC_H

/*============================================================================
              E N C O D E R   D R I V E R   H E A D E R   F I L E

DESCRIPTION
This files contains all necessary macros and definitions for interfacing with
the encoder hardware.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2006-2014 Qualcomm Technologies, Inc.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/enc.h_v   1.18   10 Jul 2002 19:40:12   gurdeeps  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wenc.h#6 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
01/10/20   ac      Mini dump framework checkin
11/18/19    ss      New API to return latest FW BTF delay
10/09/19    raj     Define and Register MTPL Backoff timers at W-SUB-ID level
08/07/19    rv      Compilation fix for removing HS_RACH feature
12/06/17    ss      Avoid race condition between enc_cleanup and set_mod_timing cb
08/16/17    ss      Changes for LTA timeline optimization
04/20/16    ar      New API changes : wenc_is_hs_rach_configured
11/06/15    ar      RACH vs QTA changes
10/30/15    raj     Query MIN Tx power only after INIT Tx is done
10/16/15    raj     Ensure RF Tx enable is not in secondary carrier retune time critical path for 
                    return from LTA
09/14/15    tsk     Avoid Wl1ulhsprachmgr_resume enabling Tx Events in QTA gap but enable post QTA.
09/09/15    ar      Double Uplink NV items
08/21/15    vr/cjz  [WPLT] Initialize endcoder HW for WPLT
08/18/15    ar      Doubling uplink state variables to maintain per subscription state
08/01/15    vr      Changes and fixes needed for ATLAS RUMI.
07/19/15    vr      Code cleanup to move all UL/EUL/EDL/HSRACH states to common struct.
07/09/15    vr      Code cleanup to use MACROs for EUL/EDL state globals.
07/07/15    yw      Create new file for HWIO interface access (UL part)
07/07/15    ar      Double the dpcch power for revert to be per subscription to support W+W LTA
07/02/15    ar      Added support for WL UL Feature bitmask NV
06/30/15    tsk     FR 26237: DSDS with Rel.8 E-FACH HSRACH + MSIM Changes.
06/27/15    vr      UL SW changes for unified and modified RF APIs on ATLAS.
22/06/15    abs     Corrected FACH state TX power reporting logic
02/27/15    ap      Initial check-in for WL1 QSH framework 
04/06/15    gp      FR 24883: UL DPCH TX Timeline Reduction
03/29/15    pr      v2 WplusW changes for Diag F3.
03/11/15    rsr     Phase 4 W+W Checkin.
02/28/15    rsr     W + W Phase 2 Checkin.
01/29/14    pr      WL1M global reorg for WplusW
01/26/15    ac      Datapath global reorg for X+W
01/16/15    vr      Global reorg for W + W.
01/15/15    vr      Removing unused globals.
11/19/14    vr      Deprecating and removing 0x4312 log packet code.
10/01/14    gv      Code cleanup. Removal of stale and unused code. 
08/19/14    ash     Added check to see if DL Reslam is done, before calling StmrAction API
08/18/14    pkg     Changes to disable FEATURE_WCDMA_NPA_THERMAL and FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
                    and FEATURE_TEMPERATURE_FC for MAV PL's.
07/28/14    pkg     Code Check-in for 16QAM feature.
07/15/14    ac      NV reorganization
07/11/14    gp      Added support for RF Temp Comp Split architecture
06/19/14    ash     Backed out code for 0x410E Log Pkt
06/04/14    cjz     GCF flag removal and automatic Tx power back-off upon GCF test
06/03/14    tsk     Compilation fixes for JOLOKIA 1.0
05/27/14    pkg     Changes to report Tx power during RACH.
05/23/14    ar      Fix to populate SW buffer with previous frame slotwise TxAGC in HSRACH.
05/13/14    ymu     Feature name FEATURE_WCDMA_DC_HSUPA_FRAME replacement with FEATURE_WCDMA_DC_HSUPA 
05/06/14    raj     DCUPA: QXDM support for TxAccum freeze per carrier
04/27/14    jkb     Add check for Feature dual sim with feature qta
02/14/14    ar      Porting Bolt specific uplink changes on to Bolt 2.0
09/14/13    pr      VSTMR Changes for BOLT
01/22/14    pkg     EUL optimization to avoid HICH result falling into QTA gap and improved RSN management for QTA.
12/11/13    gp      FR:3693 EUL stats API for data services
11/21/13    tsk     FR2573: SAR management with Integrated DSDA changes.
11/19/13    gp      Fix for crash due to race condition on FW dropping DPCH channels immediately
11/19/13    vs      CQI override changes for 3C.
11/14/13    pj      Wrapped HWIO access with WL1_HWIO macro
10/29/13    gp      Fix for thermal mitigation stall 
10/28/13    ar      Removed code under FEATURE_DL_IGNORE_MODE_FACH_MEAS
10/02/13    gp      Add dummy dpcch indication for ul cfg dpcch cmd
09/17/13    jkb     Porting Dual Sim from Triton to Dime
08/22/13    gp      Moved UL Maint parameters into new API enc_set_ul_maint_event_parameters()
                    and ensured that UL DPCCH Cfg Cmd is sent within FW Config Window
08/15/13    at      Post UL DPCH cfg disable done command to L1M after UL DPCH disable done 
                    interrupt from FW is received.
08/15/13    at      Post UL DPCCH cfg disable done command at frame bdry to L1M and
                    trigger fast dch cleanup.
08/13/13    at      Store MPR value per slot for TxAgc filtering and use MPR for event 6D trigger.
07/11/13    sad     Added support to Freeze TxAccum on sec for call spike and 
                    reconfigurations
06/29/13    ash     Clean-up residual transport channels in case the ul_cctrch_cfg_cmd() gets called before the enc_get_data() 
                    ISR fires for the CFN = (ACT TIME - 2) in a Trch Drop case. Wrapped in new function enc_clean_trch()
05/28/13    ash     UL unused Mutex cleanup
05/22/13    oh      Adding DC-HSUPA framework support
05/09/13    gp      Code cleanup for FEATURE_WCDMA_UL_CM_INTLV_WORKAROUND and FEATURE_WCDMA_CONCURRENT_R99_EUL_TX_ENCODE
05/09/12    ar      BOLT Compilation changes
05/06/13    at      SW Workaround for TXHW-A2 issue and cleanup FEATURE_WCDMA_NIKEL_ERAMA_ACCESS_FIX
04/01/13    gp      Code added to provide flexibility to control rach parameters 
                    through NV items.
02/14/13    ash     Adding QXDM Command to crash the UE when specified no. of RACH No-Acks are seen in succession
02/01/13    at      Added code to ensure to disable RF Tx if it is enabled for
                    for RACH procedure to begin when PCCPCH_N for interF SIB read
                    is being set up.
01/24/13    ar      Added support to blindly restore ciphering keys to memory after RF tx is enabled
01/04/13    raj     Changes as a part of ZI memory optimization
12/12/12    ash     Added QXDM cmd for setting Init RACH/DPCCH Pwr and to freeze Tx accum optionally
12/10/12    pkg     wcdma_control_rach_tx_pwr: This API may make UE non-compliant
                    to 3GPP and should be used only for debug and test
11/29/12    pkg     Fixed Compiler Warnings for Triton.
10/17/12    pkg     Added support for WCDMA Tx Power Indication API for GPS
10/01/12    ash     Fixed Compiler warnings for Dime
09/05/12    vr      Moved logging and edrx_hs_notify to task context as HSRACH done
                    ISR was getting severely delayed.
08/31/12    vr      Added fix to avoid unnecessary HARQ level ReTx after Seamless 
                    FACH-->DCH transition for HSRACH
07/24/12    geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
03/28/12    zr      Added feature definitions for Dime
06/19/12    vr      HSRACH code update
06/08/12    gv      Feature HS RACH Initial code check in
05/08/12    ar      Graceful handling of error cond if DeltaHs returned by FW is too high
04/20/12    vr      Warnings on MPSS.NI.3.x. Added function prototype.
04/13/12    ar      Feature cleanup FEATURE_WCDMA_RACH_ABORT_FW_WORKAROUND
03/13/12    sp      Move TX_ENABLED Macros outside RACH Trigger Feature
03/09/12    at      Fixed race condition and crash for Trigger based RACH
02/16/12    sp      Added support for Trigger Based Rach Procedure
02/15/12    sp      Optimized MPR bin implementation to reduce MIPS
02/10/12    rgn     Nikel feature cleanup
02/01/12    raj     Nikel Feature cleanup.
02/01/12    gnk     Nikel Feature cleanup.
02/01/12    gnk     Nikel feature cleanup
01/31/12    raj     Nickel feature cleanup. Mainlined the features FEATURE_WCDMA_UL_TFC_PL_REV_2, FEATURE_WCDMA_REL6_ENH_MPR
01/31/12    gnk     Nikel feature cleanup
01/31/12    gnk     Nikel feature cleanup
01/27/12    vs      Nikel feature cleanup.
01/20/12    bs      free floating changes.
11/29/11    gnk     Removed some unused features
11/24/11    gnk     Removed unused features
10/21/11    sp      Nikel UL CM workaround under FEATURE_WCDMA_UL_CM_INTLV_WORKAROUND
09/21/11    sp      Added NPA Support for Thermal Mitigation under FEATURE_WCDMA_NPA_THERMAL
08/16/11    sp      Delay Enc Cleanup for RACH ABORT Procedure to complete
07/18/11    sp      Added support to use AHB interface for R99 ERAMA WRITE
07/06/11    sb      Upmerge from offline/mainline
06/28/11    sp      Added support for Rach Abort under FEATURE_WCDMA_RACH_ABORT_PROCEDURE
04/28/11    asm     Mainlining FEATURE_WCDMA_TXAGC_DB10 on offline targets
05/11/11    gnk     Mainlining FEATURE_RF_TX_PWR_LIM,FEATURE_RF_TX_PWR_LIM_REV feat
04/28/11    sj      upmerge from offline/main
04/27/11    oh      Support for API to be called by MCPM as a call back 
                    after bringing a modem block out of power collapse.   
04/25/11    geg     Replaced *.h with *_v.h
04/19/11    asm     Enabling the feature FEATURE_WCDMA_HDET_CAL_ERR_IN_L1 for correct Evt 6D triggering
03/30/11    gnk     Mainlined several features
02/23/11    asm     Cleanup of unused, mainlined or irrelevant features in offline UL code
02/11/11    sp      Added support for Thermal Mitigation under FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
01/17/11    sp      Added support for UL_RACH_DEBUG_LOG_PKT (0x4025)
01/17/11    sp      Featurized UL RACH FW Log Pkt (0x4026) under FEATURE_WCDMA_GENESIS_SW_FW
01/07/11    sp      Added support for new UL RACH FW Log Pkt (0x4026)
12/08/10    sp      Keep Burst Interface Enabled in TX_GENERAL_CTL Mask
11/30/10    ksr     TX_GENERAL_CTL should be set to LITTLE ENDIAN for both
                    DCH and E-DCH
11/19/10    sp      Optimized R99 UL Data Dump Log Pkt implementation
11/16/10    gnk     Correcting the calculation of DPCCH pwr for MTD algo
09/02/10    gnk     Checking in the changes for UL open loop MTD feature
09/02/10    gv      Code Checkin for CPC DTX
08/03/10    asm     Delay HS-DPCCH enabling by 1 frame after UL is enabled
07/16/10    asm     Replaced WCDMA_INTLOCK() with local mutexes in R99 UL files
06/10/10    sa      MPR 7.1 support
06/01/10    gnk     QXDM support for setting DIP, RACH init power and Tx freeze
02/26/10    rvs     Fixes for Mustang.
01/13/10    scm     Move enchs_set_cqi_override_from_diag() from enchs.h to wenc.h.
11/20/09    vc      Fixed featuarization.
11/18/09    gv      Use db10 instead of db12 units for computation
08/19/09    ka      Changed UL_NUM_DELTA_HS_VALUES to support Ahs = 38/15.
07/21/09    oh      Fixed inc/src related compilation issue.
06/29/09    oh      Rel-7 E-MPR support for MIMO HS Types.
05/25/09    hk      Fixed compiler warnings
05/22/09    sa      Fixed compiler warnings
04/22/09    gnk     Fixed compiler warnings
04/13/09    asm     Corrections to Evt 6D evaluation based on correct HDET-corrected
                    TxAGC comparison
04/03/09    gv      Creation of wenc.h -- Renaming the file enc.h to wenc.h
01/30/09    ka      Added Support for Enhanced MPR algorithm
01/22/09    gnk     Added support for EDCH to continue TX over seamless reconfig.
12/08/08    ka      Removing external declarations for pa_on and pa_off apis.
12/01/08    oh      Providing API to DEM to turn on/off PA.
10/03/08    hk      Bringing in 7k Mailine fixes
09/16/08    gnk     Lint Fixes
08/20/08    gnk     Not using the function enc_print_rach_timing_debug
07/31/08    oh      EUL changes for Taxis.
07/11/08    ms      Replaced static __inline with INLINE for Q6.
07/11/08    ms      Replaced __inline with static __inline for Q6.
12/27/07    asm     Added a mechanism to calculate slot-wise TxAGC of the prev frame
12/13/07    gnk     Reset RACH stats counter on overflow after printing out stats
08/28/07    gnk     Added, renamed and modified some RACH statistic variables
08/20/07    kps     Change rach_stats_total_attempts to uint16
06/15/07    oh      Code under MSMHW_ENC_REV_6275 moved out of FEATURE_WCDMA_PWR_TFC_ELMN
                    so that file applies for EDGE-only and normal W builds
05/25/07    kps     Add support to output RACH Transmission Preamble Count.
04/17/07    gnk     Changed enc_rach_no_ack_print_debug() to enc_print_rach_timing_debug()
                    to print timing information for all RACH procedures
03/27/07    oh      Enhanced support to control EUL params(DPCH INIT PWR) via QXDM
03/16/07    am      4312 logpkt changes
03/12/07    asm     Inlined function enc_calc_effective_mpr()
03/01/07    vrao    Added enc_tx_pwr_det_done_isr() to process FW interrupt
                    based on WCDMA Tx AGC
02/21/07    stk     Fixed compiler error, by removing the inline
02/12/07    asm     Defined a function to calculate effective MPR
02/06/07    oh      No need to calculate instantaneous DPCCH pwr logging
12/13/06    asm     Corrected Featurization
11/09/06    asm     Added Support for Rel6 Maximum Power Reduction calculations
11/09/06    yiz     Add max_hs_tx_type_hist[] declaration
11/02/06    am      Added code to get current dpcch pwr in dbm
10/25/06    asm     Featurize out HS-specific code in UL TFC PL Rev2
10/03/06    sk      Added enc_resume_rach()
09/29/06    asm     Added Feature UL TFC Power Limiting Rev2
09/04/06    asr     Changed this header file's distinguishing feature name from
                    ENC_H to WENC_H (to avoid conflict with 1x's file)
08/14/06    yiz     Mainline HSUPA code
06/20/06    bd      Renamed global vars/functions to avoid conflict with 1x names
04/03/06    asm     Fixes trashed UL L2 ACK for DL signaling message that triggers re-config, HHO,
                    or cell transiotion in DCH or FACH state.
09/19/05    asm     Added F3 message to record number of transport blocks from
                    each transport channel. Printed everytime the corresponding DL
                    message is printed.
08/23/05    asm     Modifications for new common UL TFC PL log packet 0x4163
                    Also some optimizations to UL TFC PL implementation
07/18/05    yiz     Fixed ul_tx_rate_adj_in_tab[] dimension for compile
06/15/05    yiz     Add enc_hdet_conv_done_isr for RF_TX_PWR_LIM_REV
05/31/05    am      Merged the latest lint fixes to the mainline
05/12/05    gs      Externalized function enc_refresh_beta_table
                    Externalized variable ul_dpch_state
05/10/05    yiz     Moved some UL TFC PL decl from enc.c for Raven
04/08/05    asm     Merged Raven branch encoder files to mainline
03/31/05    yiz     Add decl of enc_hfn_update_evt_handler.
01/25/05    src     Moved in a bunch of macro-definitions from enc.c.
11/09/04    yiz     Add WCDMA_PRACH_SELECTION support.
10/11/04    gs      Added declaration for HSDPA and 6275 related changes
08/20/04    yiz     Del duplicate declaration of ul_log_coding_info_ptr
07/19/04    yiz     Rename l1_phychan_add_waiting_for_mac to
                    l1_phychan_waiting_for_mac.
                    Delete decl of enc_halt_build_frame_ind()
07/16/04    m       Featurize modifications to stop-build-frame-ind logic
06/16/04    m       New protype for halt build frame indication
04/16/04    yiz     Start or stop UL TFC power restriction for AMR only at recfg.
03/22/04    yiz     Add enc_stop_build_frame_ind_for_recfg().
03/16/04    yiz     Add state type decl for UL TFC power selection and fix.
11/12/03    m       Added support to suppress end of call spike
10/01/03    yiz     Added function enc_is_rach_in_progress() and support code
                    to send local command to L1M to drop RACH after RACH
                    DONE_ISR is called.
08/06/03    yiz     Update comments for enc_kill_pa().
06/02/03    gs      Put variable ul_tfcs_restrict_func_ptr under FEATURE
                    FEATURE_TFCS_CM_BY_HLS
05/27/03    gs      Externalized following global variables
                      l1_tti_enum_type rf_tti_map
                      ul_tfcs_restrict_func_ptr
                      rf_tti_map
03/25/03    yiz     Add decl for enc_rach_no_ack_print_debug().
02/27/03    yiz     Fix FEATURE_WCDMA_PWR_TFC_ELMN after test with MAC.
02/04/03    gs      Added TM ciphering support
10/22/02    yiz     Add data and function decls for FEATURE_WCDMA_PWR_TFC_ELMN
08/28/02    gs      Changed ul_tx_enable() to return FALSE if not in UL_PRACH_TX
                    or UL_DPCH_TX
07/10/02    gs      Externalized CM related variables.
05/16/02    scm     Implement computed gain factors.
04/23/02    yiz     Moved def of default computed GF vals from enc.c to be
                    referred in ulcmd.c for logging.
                    Deleted duplicate prototype of enc_cleanup().
01/18/02    gs      Externalized the function to set the ciphering key
12/04/01    sh      Added enc_pc_enable().
11/15/01    sk      Added flag ul_set_pa_on if PA must be turned ON.
08/28/01    sk      Changed FEATURE_L1_LOGGING to FEATURE_L1_LOGGING_ENABLED.
08/09/01    sk      Added logging code.
07/06/01    sk      Externalized prev_tfci and L UL Tx buffer.
05/17/01    sk      Added enc_kill_pa and aich_tt declarations.
01/16/00    sk      Added RACH DONE isr/
09/08/00    sk      file created.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "customer.h"
#include "comdef.h"
#include "queue.h"
#include "l1dlphychancfg.h"
#include "l1macdata.h"
#include "ul.h"
#include "l1task_v.h"
#include "ullog.h"
#include "l1cmmgr.h"
#include "mdspasync.h"
#include "mcalwcdma_evt.h"
#include "l1msm.h"
#include "rlcdsapif.h" 
#include "wl1rfmif.h"
#include "wl1hwioif.h"

/*===========================================================================

                                 TYPES

===========================================================================*/

/* dont care values for mDSP DPCCH Config command */
#define ENC_DPCCH_SLOT_FMT_X  0
#define ENC_TFCI_X            0

/* control flag defines for mDSP DPCCH processing */
#define ENC_DPCCH_ENA         1
#define ENC_DPCCH_DIS         0

/* flag to indicate dummy dpcch cfg or real dpcch cfg */
#define ENC_DUMMY_DPCCH_CFG   1
#define ENC_REAL_DPCCH_CFG    0

/* defines for Turbo interleaving parameters */
#define ENC_NUM_ROW_INDEX_PA      0  /* 20 rows */
#define ENC_NUM_ROW_INDEX_PB      1  /* 20 rows */
#define ENC_NUM_ROW_INDEX_PC      2  /* 10 rows */
#define ENC_NUM_ROW_INDEX_PD      3  /* 05 rows */

/* default and max GF values for computed GF */
#define ENC_DFLT_Bc           15
#define ENC_DFLT_Bd           15
#define ENC_MAX_BETA          15

/* RACH statistics logging constants */
/* Preamble count */
#define PREAMBLE_CNT_1          1
#define PREAMBLE_CNT_2          2
#define PREAMBLE_CNT_3          3
#define PREAMBLE_CNT_4          4
#define PREAMBLE_CNT_5          5
#define PREAMBLE_CNT_6          6
#define PREAMBLE_CNT_7          7
#define PREAMBLE_CNT_8          8
#define PREAMBLE_CNT_GRTR       9
#define PREAMBLE_CNT_MAX_BINS   10

/* wcdma query tx power timer timeout period in ms */
#define WCDMA_QUERY_TX_POWER_TIME 670 

#ifdef RF_MIN_TX_POWER_DBM
#define RACH_STATS_INITIAL_TXAGC  RF_MIN_TX_POWER_DBM
#else
#define RACH_STATS_INITIAL_TXAGC -57
#endif

/* This should be 0x80 in normal scenario when using INCRCACC Interface
 * to write to ERAMA */
#define UL_TX_GENERAL_CTL_MASK 0x80

#if defined (FEATURE_WCDMA_BOLT_2_0) && !defined (FEATURE_WCDMA_JOLOKIA_MODEM)

#define WCDMA_L1_UL_HWIO_OUT(A,B) WL1_MSG3(HIGH,"BOLT_HWIO : " #A " = 0x%x ",(B),0,0)
#define WCDMA_L1_UL_HWIO_OUTI(A,B,C) WL1_MSG3(HIGH,"BOLT_HWIO%d : " #A " = 0x%x",B,C,0)
#define WCDMA_L1_UL_HWIO_IN(A) 0x0
#define WCDMA_L1_UL_HWIO_INM(A,M) 0x0

#define WCDMA_L1_UL_HWIO_ADDRI(A,B) 0x0

#if defined(FEATURE_WCDMA_RUMI_SUPPORT) && defined(FEATURE_WCDMA_ATLAS_MODEM)
#define WCDMA_L1_UL_HWIO_ADDR(A)     WL1_HWIO_##A##_ADDR
#define wcdma_l1_ul_out_dword(A,B)   out_dword(A,B)
#else
#define WCDMA_L1_UL_HWIO_ADDR(A)     0x0
#define wcdma_l1_ul_out_dword(A,B)   WL1_MSG3(HIGH,"BOLT_HWIO : out_dword : " #A " = 0x%x",B,0,0);
#endif /* defined(FEATURE_WCDMA_RUMI_SUPPORT) && defined(FEATURE_WCDMA_ATLAS_MODEM) */

#define wcdma_l1_ul_in_dword(A) 0x0

#define TX_REG_BASE 0x0

#else //FEATURE_WCDMA_BOLT_RUMI_HWIO_UPLINK

#define WCDMA_L1_UL_HWIO_OUT(A,B)    WL1_HWIO_##A##_OUT(B)     //#define WL1_HWIO_TX_TRCH_MUX_ORDER_OUT HWIO_TX_TRCH_MUX_ORDER_OUT(val)
#define WCDMA_L1_UL_HWIO_OUTI(A,B,C) WL1_HWIO_##A##_OUTI(B,C)
#define WCDMA_L1_UL_HWIO_IN(A)       WL1_HWIO_##A##_IN
#define WCDMA_L1_UL_HWIO_INI(A,B)    WL1_HWIO_##A##_INI(B)
#define WCDMA_L1_UL_HWIO_INM(A,M)    WL1_HWIO_##A##_INM(M)

#define WCDMA_L1_UL_HWIO_ADDR(A)     WL1_HWIO_##A##_ADDR
#define WCDMA_L1_UL_HWIO_ADDRI(A,B)  WL1_HWIO_##A##_ADDR(B)

#define WCDMA_L1_UL_HWIO_FMSK(A,B)  WL1_HWIO_##A##_##B##_FMSK
#define WCDMA_L1_UL_HWIO_BMSK(A)    WL1_HWIO_##A##_BMSK

#define WCDMA_L1_UL_HWIO_SHFT(A)  WL1_HWIO_##A##_SHFT

#define wcdma_l1_ul_out_dword(A,B) out_dword(A,B)
#define wcdma_l1_ul_in_dword(A) in_dword(A)

#endif //FEATURE_WCDMA_BOLT_RUMI_HWIO_UPLINK


/* For PRACH and DPCH Long scrambling code type, we need to set the 25th bit to 1*/
#define WENC_LONG_SCR_CODE_MASK  0x1000000

#define WENC_DEBUG_RESTORE_CIPH_KEYS_BMSK_HSPRACH_GET_DATA (0x00)
#define WENC_DEBUG_RESTORE_CIPH_KEYS_BMSK_ENABLE_TX_FRM_ISR (0x01)
#define WENC_DEBUG_RESTORE_CIPH_KEYS_BMSK_ENC_GET_DATA (0x02)
#define WENC_DEBUG_RESTORE_CIPH_KEYS_BMSK_HSPRACH_MGR_ADD (0x03)

#ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
/* Mutex Lock/Unlock to protect rach state transistions */
#define WENC_TX_EVENT_MUTEX_LOCK()   REX_ISR_LOCK(&wenc_ext_cntrl_params.wenc_tx_event_mutex)
#define WENC_TX_EVENT_MUTEX_UNLOCK() REX_ISR_UNLOCK(&wenc_ext_cntrl_params.wenc_tx_event_mutex)
#define WENC_TX_EVENT_DISABLE_MUTEX_LOCK()   REX_ISR_LOCK(&wenc_ext_cntrl_params.wenc_tx_event_disable_mutex)
#define WENC_TX_EVENT_DISABLE_MUTEX_UNLOCK() REX_ISR_UNLOCK(&wenc_ext_cntrl_params.wenc_tx_event_disable_mutex)
#endif /* FEATURE_WCDMA_TRIGGER_BASED_RACH */

typedef enum{
  WENC_RACH_FW_ENABLE_ACTION,      /* Indicate FW to enable rach procedure (0)*/
  WENC_RACH_FW_ABORT_ACTION,       /* Indicate FW to abort any ongoing rach procedure (1)*/
  #ifdef FEATURE_WCDMA_HS_RACH
  WENC_HSRACH_FW_ENABLE_ACTION, /* Indicate FW to enable hsrach procedure (2)*/
  #endif
  WENC_RACH_FW_NUM_ACTION
}wenc_rach_fw_action_enum_type;

/* ENUM to identify the action occurred in FACH State */
typedef enum{
  WENC_RACH_ACTION_PRACH_ADD,           /* Triggered at the time of adding PRACH channel */
  WENC_RACH_ACTION_CFG_FW,              /* Triggered at the time of starting Rach procedure */
  WENC_RACH_ACTION_FACH_OOS_MEAS,       /* Triggered at the time of Fach OOS Measurement  */
  WENC_RACH_ACTION_PCCPCH_N_ADD,        /* Triggered when PCCPCH_N channel is to be added */
  WENC_RACH_ACTION_CELL_TRANS,          /* Triggered when Cell Trans Request Received */
  WENC_RACH_ACTION_PRACH_DROP,          /* Triggered when PRACH channel is dropped */
  WENC_RACH_ACTION_PRACH_DONE_ISR,      /* Triggered when Rach Procedure is finished */
  WENC_RACH_ACTION_CLEANUP,             /* Triggered at the time of UL Cleanup procedure */
  WENC_RACH_ACTION_EDCH_RSRC_ALLOC,
  WENC_RACH_ACTION_EDCH_RSRC_DELLOC,
  WENC_RACH_ACTION_CELL_RESEL_IND_HS_RACH,
  WENC_RACH_ACTION_QTA_REQ,             /* Request from FRM to abort ongoing RACH for QTA */
  WENC_RACH_ACTION_COMPLETE_BY_L1,      /* Triggered by above occasions to move rach to ready state */
  WENC_RACH_NUM_ACTIONS
} wenc_rach_action_enum_type;

#ifdef FEATURE_WCDMA_EFACH_MSIM
  typedef enum{
    WENC_HSRACH_MSIM_QTA_ORDER = 0,
    WENC_HSRACH_MSIM_DELAY_HSDPCCH,
    WENC_HSRACH_MSIM_DELAY_RESOURCE_REL,
    WENC_HSRACH_MSIM_SEND_SI0,
    WENC_HSRACH_MSIM_SI0_TX_PENDING,
    WENC_HSRACH_MSIM_DELAY_ENABLE_TX_EVTS,
    WENC_HSRACH_MSIM_NUM_ACTIONS
  } wenc_hsrach_msim_action_enum_type;

  #define WENC_HSRACH_MSIM_ACTION_SET_BMASK(A, B) (A |= (0x1 << B))
  #define WENC_HSRACH_MSIM_ACTION_GET_BMASK() (wenc_ext_cntrl_params.wenc_hsrach_msim_action_bmask)
  #define WENC_HSRACH_MSIM_ACTION_INCL(x) ((wenc_ext_cntrl_params.wenc_hsrach_msim_action_bmask & (1<<x)) >> x)
  #define WENC_HSRACH_MSIM_ACTION_INCL_ONLY(x) ((1<<x) == wenc_ext_cntrl_params.wenc_hsrach_msim_action_bmask)
  #define WENC_HSRACH_MSIM_ACTION_CLEAR_BMASK(x) (wenc_ext_cntrl_params.wenc_hsrach_msim_action_bmask = (wenc_ext_cntrl_params.wenc_hsrach_msim_action_bmask & (~(1<<x))))
  #define WENC_HSRACH_MSIM_ACTION_CLEAR_ALL_BMASK() (wenc_ext_cntrl_params.wenc_hsrach_msim_action_bmask = 0)
  
  #define WENC_HSRACH_MSIM_SET_REL_CAUSE(x) (eulsg_ext_cntrl_params.rel_cause_postponed = x)
  #define WENC_HSRACH_MSIM_GET_REL_CAUSE() (wl1_hs_rach_edch_rsrc_rel_cause_enum_type) (eulsg_ext_cntrl_params.rel_cause_postponed)
  
  #define WENC_HSRACH_MSIM_SET_REL_FLAG(x) (eulsg_ext_cntrl_params.e_ul_rel_resource_once = x)
  #define WENC_HSRACH_MSIM_GET_REL_FLAG() (eulsg_ext_cntrl_params.e_ul_rel_resource_once)
  
  /* First decrmented before use (at frame boundary) effectively setting 40ms */
  #define WENC_HSRACH_MSIM_SYNCAA_CNTR 5
  #define WL1_QTA_TO_START_BY_CFN 2
#endif /* FEATURE_WCDMA_EFACH_MSIM */

#ifdef FEATURE_WCDMA_QSH_DUMP
/* This macro is used for filling the bit fields in UL Mini  Dump Structures */
#define WL1_UL_MINIDUMP_SET_FIELD(buf, field1, value) \
  (buf) = (((buf) & (~WL1_UL_MINIDUMP_##field1##_BMSK)) | \
           (((value) << WL1_UL_MINIDUMP_##field1##_SHFT) & \
             WL1_UL_MINIDUMP_##field1##_BMSK))

/* For e_ul_debug_db_idx */
#define WL1_UL_MINIDUMP_EUL_CURR_PHYCHAN_CFG_DB_BMSK      0x03
#define WL1_UL_MINIDUMP_EUL_CURR_PHYCHAN_CFG_DB_SHFT      0x00

#define WL1_UL_MINIDUMP_EUL_CURR_DPCH_CFG_DB_BMSK         0x0C
#define WL1_UL_MINIDUMP_EUL_CURR_DPCH_CFG_DB_SHFT         0x02

#define WL1_UL_MINIDUMP_UL_PHYCHAN_DPCH_DB_BMSK           0x30
#define WL1_UL_MINIDUMP_UL_PHYCHAN_DPCH_DB_SHFT           0x04

#define WL1_UL_MINIDUMP_DBG_EUL_TIMING_BMSK               0xC0
#define WL1_UL_MINIDUMP_DBG_EUL_TIMING_SHFT               0x06

/* for enc_rach_dpch_qta_dbg_bmsk */
#define WL1_UL_MINIDUMP_UL_DPCCH_CFG_SEND_BMSK            0x01
#define WL1_UL_MINIDUMP_UL_DPCCH_CFG_SEND_SHFT            0x00

#define WL1_UL_MINIDUMP_RACH_ISR_PENDING_BMSK             0x02
#define WL1_UL_MINIDUMP_RACH_ISR_PENDING_SHFT             0x01

#define WL1_UL_MINIDUMP_ENC_CLEANUP_FLAG_BMSK             0x04
#define WL1_UL_MINIDUMP_ENC_CLEANUP_FLAG_SHFT             0x02

#define WL1_UL_MINIDUMP_RF_TX_EN_DONE_BMSK                0x08
#define WL1_UL_MINIDUMP_RF_TX_EN_DONE_SHFT                0x03

#define WL1_UL_MINIDUMP_RACH_TRIGGER_QTA_BMSK             0x10     
#define WL1_UL_MINIDUMP_RACH_TRIGGER_QTA_SHFT             0x04     

#define WL1_UL_MINIDUMP_UL_SET_PA_ON_BMSK                 0x20
#define WL1_UL_MINIDUMP_UL_SET_PA_ON_SHFT                 0x05

#define WL1_UL_MINIDUMP_UL_PA_ON_IN_PCP_BMSK              0x40
#define WL1_UL_MINIDUMP_UL_PA_ON_IN_PCP_SHFT              0x06

#define WL1_UL_MINIDUMP_UL_EUL_QTA_ACTIVE_BMSK            0x80
#define WL1_UL_MINIDUMP_UL_EUL_QTA_ACTIVE_SHFT            0x07

/* for ul_r99_states */
#define WL1_UL_MINIDUMP_UL_STATE_BMSK                     0x07
#define WL1_UL_MINIDUMP_UL_STATE_SHFT                     0x00

#define WL1_UL_MINIDUMP_UL_DPCH_STATE_BMSK                0x18
#define WL1_UL_MINIDUMP_UL_DPCH_STATE_SHFT                0x03

#define WL1_UL_MINIDUMP_RACH_CURR_STATE_BMSK              0xE0
#define WL1_UL_MINIDUMP_RACH_CURR_STATE_SHFT              0x05


/* for wl1_hs_rach_state_sub_state */
#define WL1_UL_MINIDUMP_HS_RACH_STATE_BMSK                0x07
#define WL1_UL_MINIDUMP_HS_RACH_STATE_SHFT                0x00

#define WL1_UL_MINIDUMP_HS_RACH_SUB_STATE_BMSK            0x78
#define WL1_UL_MINIDUMP_HS_RACH_SUB_STATE_SHFT            0x03


/*for wenc_rf_tx_therm_algo_tfc_pwr_elmn_qta_state */
#define WL1_UL_MINIDUMP_UL_TFCPWR_ELMN_STATE_BMSK         0x03
#define WL1_UL_MINIDUMP_UL_TFCPWR_ELMN_STATE_SHFT         0x00

#define WL1_UL_MINIDUMP_THERM_BACKOFF_CURR_STATE_BMSK     0x0C
#define WL1_UL_MINIDUMP_THERM_BACKOFF_CURR_STATE_SHFT     0x02

#define WL1_UL_MINIDUMP_RF_TX_STATE_BMSK                  0x10
#define WL1_UL_MINIDUMP_RF_TX_STATE_SHFT                  0x04

#define WL1_UL_MINIDUMP_QTA_STATE_BMSK                    0x60
#define WL1_UL_MINIDUMP_QTA_STATE_SHFT                    0x05


/* for e_ul_state_edpch_cpc_dtx_state */
#define WL1_UL_MINIDUMP_E_UL_STATE_BMSK                   0x03
#define WL1_UL_MINIDUMP_E_UL_STATE_SHFT                   0x00

#define WL1_UL_MINIDUMP_E_UL_EDPCH_STATE_BMSK             0x1C
#define WL1_UL_MINIDUMP_E_UL_EDPCH_STATE_SHFT             0x02

#define WL1_UL_MINIDUMP_E_UL_CPC_DTX_STATE_BMSK           0xE0
#define WL1_UL_MINIDUMP_E_UL_CPC_DTX_STATE_SHFT           0x05

/* for wl1_debug_ul_edldrx_ctrl_struct_type ->   cpc_drx_active_grant_monitor_en_entity_ids */
#define WL1_UL_MINIDUMP_CPC_DRX_STATUS_ACTIVE_BMSK        0x01
#define WL1_UL_MINIDUMP_CPC_DRX_STATUS_ACTIVE_SHFT        0x00

#define WL1_UL_MINIDUMP_UE_DRX_GRANT_MONITOR_EN_BMSK      0x02
#define WL1_UL_MINIDUMP_UE_DRX_GRANT_MONITOR_EN_SHFT      0x01

#define WL1_UL_MINIDUMP_ENTITY_ID_AG_S_RG_BMSK            0x0C
#define WL1_UL_MINIDUMP_ENTITY_ID_AG_S_RG_SHFT            0x02

#define WL1_UL_MINIDUMP_ENTITY_ID_HICH_BMSK               0x30
#define WL1_UL_MINIDUMP_ENTITY_ID_HICH_SHFT               0x04

#define WL1_UL_MINIDUMP_ENTITY_ID_NS_RG_BMSK              0xC0
#define WL1_UL_MINIDUMP_ENTITY_ID_NS_RG_SHFT              0x06

/* for wl1_debug_ul_edldrx_ctrl_struct_type ->   is_sched_tx_hich_rslt_fn */
#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_0_BMSK     0x01
#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_0_SHFT     0x00

#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_1_BMSK     0x02
#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_1_SHFT     0x01

#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_2_BMSK     0x04
#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_2_SHFT     0x02

#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_3_BMSK     0x08
#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_3_SHFT     0x03

#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_4_BMSK     0x10     
#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_4_SHFT     0x04     

#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_5_BMSK     0x20
#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_5_SHFT     0x05

#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_6_BMSK     0x40
#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_6_SHFT     0x06

#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_7_BMSK     0x80
#define WL1_UL_MINIDUMP_IS_SCHED_TX_HI_RSLT_FN_7_SHFT     0x07


/* for wl1_debug_ul_edch_cm_info_struct_type -> next_cfn_cm_flags_bmsk */
#define WL1_UL_MINIDUMP_NEXT_TTI_IS_IN_CM_FRAME_BMSK      0x01
#define WL1_UL_MINIDUMP_NEXT_TTI_IS_IN_CM_FRAME_SHFT      0x00

#define WL1_UL_MINIDUMP_NEED_CM_FACTOR_UPDATE_BMSK        0x02
#define WL1_UL_MINIDUMP_NEED_CM_FACTOR_UPDATE_SHFT        0x01

#define WL1_UL_MINIDUMP_NEXT_2ms_TTI_IS_CM_BMSK           0x04
#define WL1_UL_MINIDUMP_NEXT_2ms_TTI_IS_CM_SHFT           0x02

#define WL1_UL_MINIDUMP_FIRST_CM_SLOT_BMSK                0xF8
#define WL1_UL_MINIDUMP_FIRST_CM_SLOT_SHFT                0x03

/* for wl1_debug_ul_edch_cm_info_struct_type -> first_10ms_tx_is_cm */
#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_0_BMSK        0x01
#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_0_SHFT        0x00

#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_1_BMSK        0x02
#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_1_SHFT        0x01

#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_2_BMSK        0x04
#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_2_SHFT        0x02

#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_3_BMSK        0x08
#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_3_SHFT        0x03

#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_4_BMSK        0x10     
#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_4_SHFT        0x04     

#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_5_BMSK        0x20
#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_5_SHFT        0x05

#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_6_BMSK        0x40
#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_6_SHFT        0x06

#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_7_BMSK        0x80
#define WL1_UL_MINIDUMP_FIRST_10MS_TX_IS_CM_7_SHFT        0x07

/* for wl1_debug_ul_prach_status_struct_type-> last_signature_sub_ch */
#define WL1_UL_MINIDUMP_LAST_SIGNATURE_BMSK               0x0F
#define WL1_UL_MINIDUMP_LAST_SIGNATURE_SHFT               0x00

#define WL1_UL_MINIDUMP_LAST_SUBCHANNEL_BMSK              0xF0
#define WL1_UL_MINIDUMP_LAST_SUBCHANNEL_SHFT              0x04

#define WL1_EUL_DBG_TIMING_BUF_SIZE                       0x03

#endif /* FEATURE_WCDMA_QSH_DUMP*/

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC

#define MAX_NUM_SUB_ID 2

/* QSH Metric related DataType to hold all internal Information regarding UL metrics */
typedef struct
{
  /* RACH Params */
  uint8   num_rach_pmbl_attempted;
  uint8   num_rach_pmbl_nak;
  uint8   num_rach_pmbl_no_ack;
  uint8   num_rach_metric_samples;
  /* holds the accumuated RACH final Power in dBm, used for Average RACH TX Power Metric */
  int16   sum_last_pmbl_pwr;
  boolean enable_rach_metrics[MAX_NUM_SUB_ID];
  
  /* RACH procedure attempts params */
  uint8   num_rach_proc_cfgd_to_wfw;
  boolean enable_rach_attempts_metrics[MAX_NUM_SUB_ID];
  
  /* TX Pwr Metric Params */
  
  /* holds the accumuated TX AGC in dBm, used for Average TX AGC Metric */
  int16   sum_tx_power;
  int16   sum_ul_dpcch_pwr;
  uint8   num_tx_pwr_samples_collected; 
  boolean enable_ul_tx_pwr_metrics[MAX_NUM_SUB_ID];
  
  /* EUL Stat Metric Params */
  int16   sum_uph_val;
  int16   sum_sg_index;
  uint8   num_eul_stat_samples;
  uint8   num_hich_naks;
  uint8   num_eul_metric_samples;
  boolean enable_eul_stat_metrics[MAX_NUM_SUB_ID];
}wl1_ul_metric_internal_db_type;
#endif /* FEATURE_WCDMA_QSH_EVENT_METRIC */

/* Macros to set/clear/get actions (wenc_rach_action_enum_type) pending in FACH state */
#define WENC_RACH_ACTION_SET_BMASK(x)      (wenc_ext_cntrl_params.wenc_rach_action_bmask = (wenc_ext_cntrl_params.wenc_rach_action_bmask |(1<<x)))
#define WENC_RACH_ACTION_GET_BMASK()       (wenc_ext_cntrl_params.wenc_rach_action_bmask)
#define WENC_RACH_ACTION_INCL(x)           ((wenc_ext_cntrl_params.wenc_rach_action_bmask & (1<<x)) >> x)
#define WENC_RACH_ACTION_INCL_ONLY(x)      ((1<<x) == wenc_ext_cntrl_params.wenc_rach_action_bmask)
#define WENC_RACH_ACTION_CLEAR_BMASK(x)    (wenc_ext_cntrl_params.wenc_rach_action_bmask = (wenc_ext_cntrl_params.wenc_rach_action_bmask & (~(1<<x))))
#define WENC_RACH_ACTION_CLEAR_ALL_BMASK() (wenc_ext_cntrl_params.wenc_rach_action_bmask = 0)

/* Macros to get/set current rach state */
#define WENC_RACH_GET_CURR_STATE()        (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.wenc_rach_curr_state)
#define WENC_RACH_SET_CURR_STATE(rachSt)  (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.wenc_rach_curr_state =  (rachSt))
#define WENC_RACH_CHK_CURR_STATE(rachSt)  (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.wenc_rach_curr_state == (rachSt))

#define WENC_RACH_CHK_CURR_CONN_STATE(rachSt)   (WENC_RACH_CHK_CURR_STATE(rachSt))

#define WL1_GET_CURR_DPCH_STATE()       (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.ul_dpch_state)
#define WL1_SET_CURR_DPCH_STATE(dpchSt) (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.ul_dpch_state =  (dpchSt))
#define WL1_CHK_CURR_DPCH_STATE(dpchSt) (ulstates_ext_persub_cntrl_params[wsub_id].wl1_ul_state_db.ul_dpch_state == (dpchSt))

#define WL1_CHK_CURR_CONN_DPCH_STATE(dpchSt) (WL1_CHK_CURR_DPCH_STATE(dpchSt))

/* Mutex Lock/Unlock to protect rach state transistions */
#define WENC_RACH_STATE_LOCK()   REX_ISR_LOCK(&wenc_ext_cntrl_params.wenc_rach_state_mutex)
#define WENC_RACH_STATE_UNLOCK() REX_ISR_UNLOCK(&wenc_ext_cntrl_params.wenc_rach_state_mutex)

#define WENC_RF_TX_ENABLE_LOCK()   REX_ISR_LOCK(&wenc_ext_cntrl_params.wenc_rf_tx_enable_mutex)
#define WENC_RF_TX_ENABLE_UNLOCK() REX_ISR_UNLOCK(&wenc_ext_cntrl_params.wenc_rf_tx_enable_mutex)

/* MACROS to check if any or all TX Events enabled */
#define WENC_IS_TX_EVENTS_ANY_ENABLED() ((stmr_get_event_mask(MCALWCDMA_EVT_TX_TL_INTR ,wsub_id) & TX_EVENTS_MASK) || \
          (stmr_get_event_mask(MCALWCDMA_EVT_RX_TL_INTR,wsub_id) & (MCALWCDMA_EVT_EVENT_ID_TO_MASK(TX_GEN_SET_MOD_TIMING_EVENT))))

#define WENC_IS_TX_EVENTS_ALL_ENABLED() ((stmr_get_event_mask(MCALWCDMA_EVT_TX_TL_INTR ,wsub_id) & TX_EVENTS_MASK) && \
          (stmr_get_event_mask(MCALWCDMA_EVT_RX_TL_INTR,wsub_id) & (MCALWCDMA_EVT_EVENT_ID_TO_MASK(TX_GEN_SET_MOD_TIMING_EVENT))))

#define WENC_GET_TX_MCPM_CB() ((wsub_id == WCDMA_SUB_ID_0) ? wl1_enc_init_register_writes_cb_sub0 : wl1_enc_init_register_writes_cb_sub1)

/* Code below moved out of FEATURE_WCDMA_PWR_TFC_ELMN
so that it applies for EDGE-only and normal W builds */

#define UL_NUM_OL_PWR_BETA_PAIRS 30

/* number of unique modifiedRateAdjIn values */
#define NUM_RATEADJ  28

/* HS-DPCCH delta from 25.213 0~8 plus DTX */
#define UL_NUM_DELTA_HS_VALUES        11

/* DTX is 9 so that 0~8 match 25.213 deltas */
#define UL_DELTA_HS_DTX_IDX           (UL_NUM_DELTA_HS_VALUES - 1)

#define UL_NUM_RATEADJ_ELEM           UL_NUM_OL_PWR_BETA_PAIRS

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
#define QTA_LAST_BFI_CFN_DIFF_FOR_ANY_TTI      2
#define QTA_LAST_BFI_CFN_DIFF_FOR_20MS         4
#define QTA_LAST_BFI_CFN_DIFF_FOR_40MS         6
#define QTA_LAST_BFI_CFN_DIFF_FOR_80MS         10
#endif

typedef struct {
  /* elimination history buffer frame by frame */
 uint16 rateAdj_elmn_hist[PWR_TFC_ELMN_Y_FRAME];

 /* sum of elimination history */
 uint16 rateAdj_elmn_sum;

 /* Does this rateAdj have a corresponding TFC? */
 boolean tfc_exists;
 /* Is the modifiedRateAdjIn in Excess Power state? */
 boolean  excess_pwr;
} rateAdj_pwr_elmn_tab_elem_type;

/* table of modifiedRateAdjIn values sorted in ascending order */
extern const int16 sortedRateAdjIn[NUM_RATEADJ];

/* tfc_elmn_pwr_buf = maxRateAdjIdx + txRateAdjIn
** circular buffer of three frames:
** process data | transmitting frame | elminate TFC
**/
#define TFC_ELMN_PWR_BUF_SIZE 3

/* externalize rf_tti_map so that it is accessable outside this file */
extern const l1_tti_enum_type rf_tti_map[];

/* get the sortedRateAdjIn[] index for a betaC and betaD pair */
extern uint8 enc_get_modRateAdj_idx_for_GF(uint8 betaC, uint8 betaD, wsub_id_e_type wsub_id);

extern void ul_init_tfc_elmn( wsub_id_e_type wsub_id);

/* state of UL TFC power restriction */
typedef enum
{
  UL_TFC_PWR_ELMN_STATE_INIT,
  UL_TFC_PWR_ELMN_STATE_PREP, /* 2 frames before TFC elmination start */
  UL_TFC_PWR_ELMN_STATE_GO,
  UL_TFC_PWR_ELMN_STATE_PAUSE /* pause tfc pl for reconfig */
} ul_tfc_pwr_elmn_state_enum_type;

#define ENC_CFN_TO_SUSPEND_BFI_INVALID  ((uint32)-1)

#define UL_NUM_OL_PWR_PARMS      4

#define UL_MOD_BC_IDX 0
#define UL_MOD_BD_IDX 1
#define UL_RACH_MSGDELTA_PWR_IDX 2
#define UL_RATE_ADJ_IN_IDX 3

extern const int16 ul_tx_rate_adj_in_tab[UL_NUM_OL_PWR_BETA_PAIRS + 1][UL_NUM_OL_PWR_PARMS];

/* Macro to obtain modified Bc */
#define UL_MOD_Bc(Bc, Bd) ((Bc == ENC_MAX_BETA) ? \
  ul_tx_rate_adj_in_tab[Bd][UL_MOD_BC_IDX] : \
  ul_tx_rate_adj_in_tab[ENC_MAX_BETA+Bc][UL_MOD_BC_IDX])

/* Macro to obtain modified Bd */
#define UL_MOD_Bd(Bc, Bd) ((Bc == ENC_MAX_BETA) ? \
  ul_tx_rate_adj_in_tab[Bd][UL_MOD_BD_IDX] : \
  ul_tx_rate_adj_in_tab[ENC_MAX_BETA+Bc][UL_MOD_BD_IDX])

/* Macro to obtain Modified RateAdjIn */
#define UL_MOD_RATE_ADJ_IN(Bc, Bd) ((Bc == ENC_MAX_BETA) ? \
  ul_tx_rate_adj_in_tab[Bd][UL_RATE_ADJ_IN_IDX] : \
  ul_tx_rate_adj_in_tab[ENC_MAX_BETA+Bc][UL_RATE_ADJ_IN_IDX])

/* Macro to obtain RACH beta radio. It is same as Rate Adj */
#define RACH_PWR_BETA_RATIO(Bc, Bd) UL_MOD_RATE_ADJ_IN(Bc, Bd)

/* Macro to obtain RACH message delta power */
#define RACH_MSG_DELTA_PWR(Bc, Bd) ((Bc == ENC_MAX_BETA) ? \
  ul_tx_rate_adj_in_tab[Bd][UL_RACH_MSGDELTA_PWR_IDX] : \
  ul_tx_rate_adj_in_tab[ENC_MAX_BETA+Bc][UL_RACH_MSGDELTA_PWR_IDX])



/* Macro to obtain Modified RateAdjIn */
/* The table is sorted by DPCH power gains and the index happens
 * to be deterministic by below formula */
#define ENC_GET_BETA_PAIR_INDEX(Bc, Bd) ((Bc == 15) ? \
  Bd : UL_NUM_OL_PWR_BETA_PAIRS - Bc)

// Macros for Enhanced MPR packing

#define MDSP_MPR_VAL_PACK_BMSK             0x7
#define MDSP_MPR_VAL_PACK_DTX_SHFT         0x0
#define MDSP_MPR_VAL_PACK_ACK_SHFT         0x3
#define MDSP_MPR_VAL_PACK_NACK_SHFT        0x6
#define MDSP_MPR_VAL_PACK_CQI_SHFT         0x9
/*For MIMO HS Types*/
#define MDSP_MPR_VAL_PACK_ACK_ACK_SHFT     0xC
#define MDSP_MPR_VAL_PACK_NACK_NACK_SHFT   0xF
#define MDSP_MPR_VAL_PACK_DELTA_CQI_SHFT   0x12
#define MDSP_MPR_VAL_PACK_ACK_NACK_SHFT    0x15
#define MDSP_MPR_VAL_PACK_NACK_ACK_SHFT    0x18

/* Macro to pack MPR value into packed array */
#define MDSP_MPR_VAL_PACK( x, mpr_array, rhs_type )  \
  (mpr_array |= ( x << MDSP_MPR_VAL_PACK_##rhs_type##_SHFT ))

/* Macro to unpack MPR value from packed array */
#define MDSP_MPR_VAL_UNPACK( x, rhs_type )  \
  (( x >> MDSP_MPR_VAL_PACK_##rhs_type##_SHFT ) & MDSP_MPR_VAL_PACK_BMSK)

#define WENC_SET_FUNC_TRAVERSED(func_enum) \
  ( (wenc_int_cntrl_params.code_path) |= (1 << func_enum) )
#define WENC_SET_RET_VAL_FOR_FUNC_TRAVERSED(func_enum, retVal) \
  ( (wenc_int_cntrl_params.code_path_ret_Val) |= ( (retVal && TRUE) << func_enum) ) 

/* Rhs types. These have power delta passed OTA from N/W side */
typedef enum
{
  ENCHS_RHS_TYPE_DTX,
  ENCHS_RHS_TYPE_ACK,
  ENCHS_RHS_TYPE_NACK,
  ENCHS_RHS_TYPE_CQI,
  ENCHS_RHS_TYPE_ACK_ACK,
  ENCHS_RHS_TYPE_ACK_NACK,
  ENCHS_RHS_TYPE_NACK_ACK,
  ENCHS_RHS_TYPE_NACK_NACK,
  ENCHS_RHS_TYPE_DELTA_CQI,
  ENCHS_NUM_RHS_TYPE
} enchs_rhs_type_enum_type;

/* Data-Structure to store Rel 6 Enhanced MPR parameters */
typedef struct {
    uint16  rho_d_x15;
    uint16  rho_hs_x15[ ENCHS_NUM_RHS_TYPE ];
    uint16  rho_ec_x15;
    uint16  rho_ed_x15;
    uint8   mu;
    uint8   dpdch_en;
    uint8   hs_dpcch_en;
} ul_mpr_calc_parms;

/* Timer for wcdma query tx power dbm
 * which fires every 670ms
 */


/* Possible states for PA Sensor device monitored
 * through NPA */
typedef enum
{
  WENC_THERMAL_NPA_STATE_OKAY,
  WENC_THERMAL_NPA_STATE_WARNING,
  WENC_THERMAL_NPA_STATE_WARNING_EX,
  WENC_THERMAL_NPA_STATE_CRITICAL
} wenc_thermal_npa_state_enum;

#ifdef FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
/* Possible states in MTPL Thermal Backoff Algorithm */
typedef enum
{
  WENC_THERMAL_BACKOFF_START, /* PA Sensor Sate is WARNING_EX and UE in DCH */
  WENC_THERMAL_BACKOFF_PAUSE, /* PA Sensor state is WARNING_EX and UE not in DCH */
  WENC_THERMAL_BACKOFF_STOP,  /* PA Sensor state is not WARNING_EX */
  WENC_THERMAL_BACKOFF_NUM_STATES 
} mtpl_backoff_thermal_algo_state;

#endif /* FEATURE_WCDMA_MTPL_THERMAL_BACKOFF */

#define WENC_TX_ERAMA_INIT_VALUE 0x1

#if defined (FEATURE_WCDMA_BOLT_2_0) && !defined (FEATURE_WCDMA_JOLOKIA_MODEM)
/* 1 - Big endian. Applies to both A2 and FW AHB interfaces 
  * for A2, this value has to be zero so that data is automatically
  * converted to BIG-ENDIAN */
#define WENC_LMEM_BIG_ENDIAN_VALUE 0x1
#define WENC_LMEM_LITTLE_ENDIAN_VALUE 0x1
#else
/* 1 - Big endian. Applies to both A2 and FW AHB interfaces 
  * for A2, this value has to be zero so that data is automatically
  * converted to BIG-ENDIAN */
#define WENC_TX_MEM_ACCESS_CFG_VALUE 0x0
#endif

#ifdef FEATURE_WCDMA_MODEM_TPUT_STATS_API 
/* Filter co-eff for TX Power */
#define WCDMA_TX_POWER_COEFF           50

/* Threshold for W working at LOW Power */
#define WCDMA_TX_POWER_LOW_DBM          0

/* Threshold for W working at HIGH Power */
#define WCDMA_TX_POWER_HIGH_DBM        10

/* RACH message power offset */
#define WCDMA_RACH_POWER_OFFSET           3

/* Logging Bitmaks for Cost of modem power */
#define WCDMA_STATS_COST_OF_TX_POWER_LOGGING_BMSK 0x2

typedef struct
{
  /* Modem power enumeration*/
  wl1_ul_modem_tx_power_enum_type tx_modem_power;

  /* Maintains filtered value of UL TxAgc(dbm)  */
  int16 modem_power_tx_agc;

  /* Indicates if this is the first sample of UL TxAGC */
  boolean first_sample_for_tx_power;

  /* QXDM Control: Indicates if debugging tx_power coeff is ON */
  boolean is_dbg_tx_power_coeff;

  /* Filter co-eff used for UL Txpower(dbm) */
  uint16 tx_power_filter_coeff;
  
} wl1_ul_modem_power_struct_type;

#endif /* FEATURE_WCDMA_MODEM_TPUT_STATS_API  */

typedef struct {
  /*trch idx val*/
  uint8   trch_idx;
  /*crc val*/
  uint8   crc ;
  /* num transport block*/
  uint8   num_tb;
  /*transport block size*/
  uint16  tb_size ;
  /*ERAMB start addr*/
  uint16  eramb_start_addr;
} debug_enc_trch_cfg0_ram_struct_type;

typedef enum {
  FUNC_WENC_HANDLE_QTA_GAP,
  FUNC_WENC_PERFORM_CLEANUP,
  FUNC_WENC_VALIDATE_UL_STATE,
  FUNC_ENC_GET_DATA_DCH,
  FUNC_WENC_ENABLE_UL_TX,
  FUNC_WENC_WAIT_FOR_DPDCH_START,
  FUNC_WENC_BUILD_FRAME_INDICATION_TO_MAC,
  FUNC_ENC_PROCESS_TX_DATA,
  FUNC_ENC_GET_DATA_RACH,
  FUNC_WENC_RESERVE_FRAMES_RACH
} wl1_wenc_enc_code_path_enum_type;

typedef struct {

  /* Represent frame number based on UL timing*/
  uint8   crf;

  /* Represent next frame number on UL timing*/
  uint8   nrf;

  /* Represent frame number based on DL timing */
  uint8   curr_cfn;

  /* Flag to omit SRB data */
  boolean SRB_omit;

  /* Flag to indicate ul tx disabled to MAC*/
  boolean bfi_ul_tx_disabled;

  /* Flag to indicate tti alignment to MAC for PDU building*/
  l1_tti_enum_type tti_for_mac;

} wl1_wenc_local_param_struct_type;

typedef struct {
  /*trch idx is valid*/
  boolean valid;
  /*trch idx val*/
  uint8 val;
  /* ERAMb start address indexed by TrCH id */
  uint16 eramb_start_addr;
  /* number of code segments */
  uint8  num_code_segs;
  /* number of bits per code segment */
  uint32 num_bits_per_code_seg;
  /* number of pre-rate matched bits for a given TFCI */
  uint32 pre_rm_bits;
  /* number of filler bits to be used in the first code segment */
  uint32 num_filler_bits;
  /* Map of TTI enums to values required by TRCH_CFG1_RAM */
  uint8 tti_enc_val;
  /* Coding Type */
  uint8 cctrch_coding_type;
  /* flag to indicate if puncturing is required */
  boolean punct_flag;
  /* error variable eini1 */
  uint16  eini1;
  /* error variable eini2 */
  uint16  eini2;
  /* error variable eminus1 */
  uint16  eminus1;
  /* error variable eminus2 */
  uint16  eminus2;
  /* error variable eplus1 */
  uint16  eplus1;
  /* error variable eplus2 */
  uint16  eplus2;
  /**** TX_TURBO_INTLV_PARAM0_TRCHn parameters ****/
  /* C - 1, where C is the #columns */
  uint8  num_columns_minus1;
  /* last column in the last row */
  uint8  last_column;
  /* #rows in the interleaver table */
  uint8  num_row_index;
  /* exchange indicator (1st column cell with last column cell in last row */
  uint8  last_row_bit_exch;
  /* index of last row with valid data */
  uint8  last_row;
  /**** TX_TURBO_INTLV_PARAM1_TRCHn parameters ****/
  /* index of prime number p used in the index table */
  uint8  prime_num_index;
  /* prime number p */
  uint16 prime_number;

} debug_enc_trch_rm_struct_type;

typedef struct {
    /* TrCH id */
  uint8                 trch_id;
    /* TTI */
  l1_tti_enum_type      tti;
    /* active ? - useful for debugging */
  boolean               active;
} enc_eram_ctrl_struct_type[L1_UL_TRCH_MAX];

typedef struct{
  /*num ERAMA transport channels*/
  uint8 num_erama_trchs ;
  /*num active transport channels*/
  uint8 num_active_trchs;
  /*filler val programmed*/
  uint8 filler_val;
  /*Value prog. to Tx Gen Ctrl*/
  uint32 tx_gen_ctrl;
   /*Curr CFN*/
  uint32 curr_cfn;
} debug_enc_trch_struct_type;

typedef struct
{  
  void (*get_data)(wsub_id_e_type wsub_id);
  void (*set_mod_timing)(wsub_id_e_type wsub_id);
  void (*cleanup)(wsub_id_e_type wsub_id);
} wl1ul_evt_handlers_type;

typedef struct
{
  /* TRUE when UL TPC shall be freezed by UL state
  ** FALSE when UL TPC may be unfreezed by UL state
  ** defaults to TRUE for initial state */
  boolean ul_tpc_freeze_flag;
  /* Bmask which tracks actions triggered during rach procedure
  ** Actions based on enum (wenc_rach_action_enum_type) */
  uint32 wenc_rach_action_bmask;
  #ifdef FEATURE_WCDMA_EFACH_MSIM
  uint32 wenc_hsrach_msim_action_bmask;
  uint8 wl1_hsrach_syncaa_counter;
  #endif /* FEATURE_WCDMA_EFACH_MSIM */
  boolean is_voice_only_call;
  boolean wenc_rf_tx_enable_done_for_rach;
  /* Flag to indicate if rach_debug buf empty or not */
  boolean ul_fw_rach_debug_buf_full_flag;
  /* Flag to indicate if it is seamless DCH reconfig or not */
  boolean enc_seamless_dch_reconfig_flag;
  /* TRUE when UL TPC shall be freezed during seamless reconfig
  ** FALSE when UL TPC may be unfreezed after seamless reconfig
  ** defaults to FALSE for no restriction */
  boolean reconfig_freeze_flag;
  boolean low_sir_freeze_flag[WL1_MAX_UL_CARR_IDX];
  /* Flag to indicate PA needs to be turned ON before next DPCH Tx frame.
  ** Set when in-sync, and reset to FALSE when we go out-of-sync. */
  boolean ul_set_pa_on;
  /* debug flag to indicate if FW trigger a interrupt done cmd to SW */
  boolean debug_sec_teardown_flag;
  /* Flag to indicate if ENC driver is in a cleanup */
  boolean enc_cleanup_flg;

  /* ERAMb start address indexed by TrCH id */
  uint16 eramb_start_addr[L1_UL_TRCH_MAX];
  uint8 cfg0_idx;
  /* CFN passed to MAC for build frame indication */
  uint8 enc_build_fr_ind_cfn;
  uint8 num_active_trchs;
  /* CFN to suspend build_frame_ind() two frames before act time */
  uint32 enc_cfn_to_suspend_bfi;
  /* TFC shadow - always updated whenever new TrCHs are provided by MAC */
  ul_trch_state_enum_type enc_tfc[L1_UL_TRCH_MAX];
  /* Previous TFCI - track to issue mDSP DPCCH Config command only when TFCI
  ** changes. Temporary until Firmware folks can look into the problem with
  ** the mDSP DPCCH Config Command every 10ms frame */
  uint16 prev_tfci;

  /* TX power parameters for DPCH and HS-DPCCH */
  mdspasync_ul_tx_pwr_gain_factors_struct_type enc_pwr_params[2];
  /* This is UL comperssed mode active state and info */
  l1_cm_ul_param_struct_type ul_cm_params;
  /* Buffer to store MPR values history for each slot of previous frame */
  int16 wenc_mpr_val_hist[SLOTS_PER_FRAME];
  /* elimination history buffer index for next frame */
  uint16 rateAdj_elmn_hist_frame_idx;
  int16 min_tx_power_agc;
  int16 min_tx_power_dbm;
  uint8 enc_cur_pwr_buf_idx;
  enchs_rhs_type_enum_type max_hs_tx_type_hist[SLOTS_PER_FRAME];
  uint8 enc_prev_pwr_buf_idx;
  /* Buffer to update TxAGC history */
  int16 wenc_tx_pwr_hist_agc[SLOTS_PER_FRAME];

  /* TFCI to sortedRateAdjIn[] index map */
  uint8 tfci_rateAdj_map[L1_UL_TFC_MAX];
  /* Initialize the RF_TX state to 'DISABLED'*/
  wl1_rf_tx_status_enum wl1_rf_tx_state;

  /* UL_FW_RACH_DEBUG_LOG_PKT variable to record log samples */
  UL_FW_RACH_DEBUG_LOG_PKT_type ul_fw_rach_debug_log_pkt;
  /* Log packet declaration to record UL DATA written into ERAMA */
  UL_DATA_DUMP_LOG_PKT_type *ul_data_dump_log_pkt_ptr;
  /* RACH_DEBUG_LOG_PKT variable to record log samples */
  UL_RACH_DEBUG_LOG_PKT_type ul_rach_debug_log_pkt;

  /* Timer to detect whether FW is skipping to send the UL DPCCH Drop done interrupt*/
  timer_type wl1_ul_dpch_drop_intr_timeout_timer;

  rex_crit_sect_type wenc_rach_state_mutex;
  rex_crit_sect_type eul_and_r99_ul_mutex;
  rex_crit_sect_type wenc_rf_tx_enable_mutex;

  /* Declaring variables to access shared Mdsp memory */
  WfwTxAsyncReadStruct         *wfw_intf_tx_async_read_ptr;
  WfwTxRachLogStruct           *wfw_tx_rach_res_ptr;
  WfwTxDpcchAsyncReadStruct    *wfw_intf_dpcch_async_read_ptr[WL1_MAX_UL_CARR_IDX];
  WfwTxRachAsyncWriteStruct    *wfw_intf_rach_async_write_ptr;
  WfwTxHsDpcchAsyncReadStruct  *wfw_intf_tx_hsdpcch_async_read_ptr;
  WfwTxDpcchAsyncWriteStruct   *wfw_intf_dpcch_async_write_ptr[WL1_MAX_UL_CARR_IDX];
  WfwTxAsyncWriteStruct        *wfw_intf_tx_async_write_ptr;
  WfwTxAgcAsyncWriteStruct     *wfw_intf_txagc_async_write_ptr;
  WfwTxHsDpcchAsyncWriteStruct *wfw_intf_tx_hsdpcch_async_write_ptr;
  WfwTxRachAsyncReadStruct     *wfw_intf_rach_async_read_ptr;
  WfwTxAgcAsyncReadStruct      *wfw_intf_txagc_async_read_ptr;

  /* Function pointer to TFCS restriction evaluation function */
  L1_UL_QUERY_TFCS_RESTRICT_CB_FUNC_TYPE *ul_tfcs_restrict_func_ptr;
  int16 dpch_init_tx_pwr_dbm;
  /* index for above buffer */
  uint8 tfc_elmn_pwr_buf_idx;
  uint8 trch_cfg0_idx[L1_UL_TRCH_MAX];
  /* Flag to indicate that the UE is out of sync.
  ** When in sync, UL TX shall be delayed (PA off) until phychan establishment
  ** indication and enc_ul_out_sync becomes FALSE. */
  boolean enc_ul_out_sync;
  uint8 ul_wait_pcp_cnt;
  /* DPCH OLPC parameter */
  int16 dpch_init_tx_pwr_agc;
  /* UL TFC power restriction state */
  ul_tfc_pwr_elmn_state_enum_type ul_tfc_pwr_elmn_state;
  /* power elimination of modifiedRateAdjIn table */
  rateAdj_pwr_elmn_tab_elem_type rateAdj_pwr_elmn_tab[UL_NUM_RATEADJ_ELEM];
  uint8 ul_tx_info[TFC_ELMN_PWR_BUF_SIZE];
  /* table of TFC EXCESS_POWER state flags
  ** TRUE: Excess Power; FALSE: Supported */
  boolean enc_tfc_excess_pwr_state[L1_UL_TFC_MAX];
  boolean ul_rach_debug_buf_full_flag;
  /* Is DPCH and HS-DPCCH power aparmeters ready ? */
  boolean enc_dpch_hs_pwr_param_ready;
  /* Flag to track if closed loop power control has been enabled for DCH */
  boolean rfagc_pwr_ctrl_enable;
  /* Flag to indicate if the log buffer is available to record new data.
  ** The default value is set to TRUE. The FLAG is set to FALSE
  ** after the data is recorded in the log buffer. It is set to
  ** TRUE after the log packet is submitted to DIAG in task context */
  boolean ul_data_log_buf_full_flag;
  l1_ul_tx_data_type l1_ul_tx_buf;
  int16 msg_tx_pwr;
  /* Not under feature for RF TX power limiting */
  boolean ul_next_frame_is_compressed;

  int32 wenc_cxm_report_rach_msg_pwr;

  #ifdef FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
  rex_timer_type wenc_thermal_backoff_dutycycle_timer[WCDMA_NUM_SUBS];
  rex_timer_type wenc_thermal_backoff_step_timer[WCDMA_NUM_SUBS];
  boolean is_e911_call_active;
  /* Flag to indicate if PA SENSOR STATE is in WARNING_EX */
  boolean pa_sensor_state_warning_ex;
  /* Mutex to protect state tranisitions of MTPL Backoff Algorithm 
  ** This mutex also ensures the TIMER Task is not run in parallel with state transitions */
  rex_crit_sect_type wenc_thermal_backoff_state_mutex;
  #endif /* FEATURE_WCDMA_MTPL_THERMAL_BACKOFF */

  #ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
  /* Mutex to perform atomic state transistions */
  rex_crit_sect_type wl1_rach_sub_chan_avail_mutex;
  #endif /* FEATURE_WCDMA_FREE_FLOATING_TASK */

  #ifdef FEATURE_WCDMA_MODEM_TPUT_STATS_API 
  wl1_ul_modem_power_struct_type wl1_modem_tx_power;
  #endif /* FEATURE_WCDMA_MODEM_TPUT_STATS_API */

  #ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
  /* Indicate whether to call CM API to update the channel timing for getting Blanking pattern */
  boolean wenc_delay_cxm_ch_timing_update;
  #endif /* FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW */

  #ifdef FEATURE_WCDMA_HS_RACH
  int16 tx_agc_logging;
  uint32 last_aiSum_logging;
  #endif /* FEATURE_WCDMA_HS_RACH */

  #ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
  /* Mutex to protect enabling of Tx event from ISR and WL1 task context */
  rex_crit_sect_type wenc_tx_event_mutex;
  /* Mutex to avoid race-condition between ul_tx_gen_mod_timing_post_cb ISR and enc_cleanup  when rach state */
  rex_crit_sect_type wenc_tx_event_disable_mutex;
  #endif /* FEATURE_WCDMA_TRIGGER_BASED_RACH */

  /* define globals for use with the metrics collection feature */
  #ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
  wl1_ul_metric_internal_db_type wl1_ul_metric_internal_db;
  #endif /* FEATURE_WCDMA_QSH_EVENT_METRIC */

  boolean rach_res_type;
} wenc_ext_cntrl_params_type;

extern wenc_ext_cntrl_params_type wenc_ext_cntrl_params;

typedef struct
{
  wl1_ul_feature_bitmask_config_struct_type wl1_ul_feature_bitmask_nv_config;
  int16 dpch_init_tx_pwr_agc_for_revert;
}wenc_ext_persub_cntrl_params_type;

extern wenc_ext_persub_cntrl_params_type wenc_ext_persub_cntrl_params[WCDMA_NUM_SUBS];

typedef struct
{
  /* Variables used for creating and controlling RACH NV item */
  wl1_ul_rach_nv_config_type wl1_nv_prach_config;
}wenc_int_persub_cntrl_params_type;

typedef struct
{
  uint8 gcf_check_timer;
  /* Indicators to tell if a frame is CM
  ** Do not count CM frame for either X or Y
  ** index wenc_ext_cntrl_params.tfc_elmn_pwr_buf_idx */
  boolean enc_tfc_cm_hist[TFC_ELMN_PWR_BUF_SIZE];
  boolean enc_AMR_present_flag;
  /* Debug struct of trch related values prog. */
  debug_enc_trch_struct_type debug_enc_trch_struct;
  /* wcdma query tx power dbm to GPS Global Variables 
  ** Index to max used tx power double buffer */
  int16 max_used_tx_power[2];
  /* Power change delta required for CM */
  uint32 enc_delta_pc;
  /* flag to indicate whether need to calculate number of frames to delay PA on
  ** for proper DCH PCP frames */
  boolean need_calc_pa_delay_for_pcp;
  /* The following variables are used to get the SF for the CM if
  ** here is no active TrCh in the coming frame. Encoder can't
  ** explicitly calculate the SF. It i based on the SF calculated
  ** at the start of the TTI */
  boolean enc_cur_sf_error;
  /* Encoder ERAM control buffer */
  enc_eram_ctrl_struct_type enc_eram_ctl_buf;
  uint8 trch_accum_buf_idx;
  /* Variable to check if the Tx chain needs to be enabled 
  ** from enc_get_data event isr */
  boolean wl1_ul_tx_enable_in_isr;
  uint8 enc_eram_ctl_buf_idx;
  uint16 ul_trch_trblk_accum[2][L1_UL_TRCH_MAX];
  /* DPCH SRB dleay frames */
  uint8 srb_frame_cnt;
  /* Beta C value to set */
  uint8 enc_ul_bc;
  /* Beta D value to set */
  uint8 enc_ul_bd;
  /* wcdma query tx power dbm to GPS Global Variables 
  ** Index to max used tx power double buffer */
  uint16 current_max_used_tx_power_buf_idx;
  l1_sf_enum_type enc_cur_sf;
  /* number of TrCh's from MAC that found matching TTI boundary
  ** Used to catch MAC TFCI selection error regarding TTI boundary */
  uint8 num_trch_tti_matched;
  uint8 prev_num_active_trchs;
  /* Is an ISR pending for RACH ? */
  boolean rach_isr_pending;
  /* indicate current mDSP table index. beta table is upload
  ** ping pongs between 2 buffers */
  uint8 enc_cur_mdsp_beta_table_index;
  /* Debug trch cfg0 ram struct */
  debug_enc_trch_cfg0_ram_struct_type debug_enc_trch_cfg0_ram_struct[L1_UL_TRCH_MAX];
  /* CFN to check for pa on in DCH */
  uint16 frame_to_check_pa_on;
  /* indicate UL CM slot format change */
  boolean enc_ul_dpcch_slotfmt_change_for_cm;
  boolean ul_pa_on_in_pcp_wait;
  int16 gcf_tx_pwr_max_in_hist;
  l1_sf_enum_type enc_last_frame_sf;
 
  /* flag to indicate whether first time to enter UL_DPCH_STATE_TX state */
  boolean already_started_non_pcp_dpch;
  int16 gcf_tx_pwr_min_in_hist;
  /*Debug Encoder RM struct type*/
  debug_enc_trch_rm_struct_type debug_enc_trch_rm_struct[L1_UL_TRCH_MAX];
  uint16 sfn_at_rach_cfg;
  /* Rate adjust in delta */
  uint32 enc_rate_adj_in_cm_delta;
  uint16 btf_delay_value_cx8;
  /*FLAG used to indicate whether the UL TX buffer needs cleanup*/
  boolean enc_ul_tx_buf_clean;
  /* List of ERAMa TrCH ids */
  uint8 new_erama_trch_list[L1_UL_TRCH_MAX];

  ul_tfc_pwr_elmn_sample_struct_type *ul_tfc_pwr_elmn_sample_ptr;
  l1_cm_ul_param_struct_type enc_last_frame_cm_params;
  int16 gcf_tx_accum_min_in_hist;
  /* Stores SF used for next radio frame */
  l1_sf_enum_type debug_sf_used;
  boolean enc_last_frame_cm;
  /* positive value increases UE max TX power limit by 2 dB in TX AGC unit */
  int16 enc_tfc_pwr_restr_delta;
  
  int enc_extra_num_rach_preambles;
  /* Only for Rev 2 */
  int16 mtpl_minus_accu_hist[SLOTS_PER_FRAME];
  /* GCF power restriction related Global and Macro */
  boolean gcf_detected;
  /* wl1_cont_rach_noack_cnt is used to keep track of consecutive number of 
  ** rach no-acks */
  uint16 wl1_cont_rach_noack_cnt;
  int16 gcf_tx_accum_max_in_hist;
  /* number of DCH PCP frames */
  uint8 pcp_frame_cnt;
  /* Free location in ERAMb to store data from the next TrCH */
  uint16 eramb_free_addr;

  boolean data_txed;
  /* Number of frames before DPDCH data possibly OTA */
  uint8 fr_bf_dpdch_ota;
  uint16 debug_no_ack_ctr;
  int16 cfn_fix_by_stmr;
  l1_sf_enum_type sf;
  /* Counter to indicate how many more frames of UL DCCH data will be Txed */
  uint8 ul_fet_srb_data_present_counter;

  #if defined (FEATURE_WCDMA_BOLT_2_0) && !defined (FEATURE_WCDMA_JOLOKIA_MODEM)
  #ifdef DEBUG_ERAMA_RMRAM_DATA_DUMP
  uint32 debug_eramb_num_pre_rm_bits;
  boolean dbg_trigger_encoding_last_frame;
  uint32 debug_erama_write_count;
  #endif /* DEBUG_ERAMA_RMRAM_DATA_DUMP */
  #endif /* defined (FEATURE_WCDMA_BOLT_2_0) && !defined (FEATURE_WCDMA_JOLOKIA_MODEM) */

  #ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
  /* Indicate if W priority bump-up req is sent already */
  boolean ul_tx_blank_bump_up_req_sent;
  /* Count number of TTI's with SRB data */
  uint8 ul_tx_blank_srb_tti_cntr;
  #endif /* FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW */

  #ifdef FEATURE_WCDMA_HS_RACH
  l1_ul_prach_cfg_struct_type ul_hs_prach_cfg;
  uint32 wenc_hsrach_asc_num;
  #endif /* FEATURE_WCDMA_HS_RACH */

  #ifdef FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
  /* Counter for the step timer. Used in calculating new MTPL backoff */
  uint8 wenc_thermal_backoff_step_timer_cnt;
  /* dB value by which MTPL needs to be reduced */
  uint8 wenc_thermal_backoff_mtpl_reduce_by_db;
  /* Initialize backoff algorithm state to STOP state */
  mtpl_backoff_thermal_algo_state wenc_thermal_backoff_curr_state;
  /* Toggle switch for duty cycle. Initialized to UP */
  uint8 wenc_thermal_backoff_dutycycle_switch;
  #endif /* FEATURE_WCDMA_MTPL_THERMAL_BACKOFF */

  #ifdef FEATURE_WCDMA_L1_ZI_OPT
  /* The pointer to the dynamic memory allocated for the UL DATA DUMP Log Pkt 
  ** The pointer is initialized in ul_data_dump_log_pkt_alloc() */
  uint32 ul_data_dump_log_pkt_buf[UL_DATA_LOG_MAX_BUF_SIZE_WORDS];
  /* The pointer to the dynamic memory allocated for the UL DATA DUMP Log Pkt 
  ** The pointer is initialized in ul_data_dump_log_pkt_alloc() */
  uint32 *ul_data_dump_log_pkt_buf_ptr;
  #endif /* FEATURE_WCDMA_L1_ZI_OPT */

  /* indicator whether UL TX data buffer is empty at cleanup */
  boolean enc_ul_tx_data_trashed;

  #if defined(FEATURE_WCDMA_L1_TM_CIPHER_SUPPORTED)
  uint16 total_tm_bits_to_cipher;
  #endif /* defined(FEATURE_WCDMA_L1_TM_CIPHER_SUPPORTED) */

  uint32 code_path;

  uint32 code_path_ret_Val;
} wenc_int_cntrl_params_type;

/* bit_position = number -1. Example Master control at 0th bit postion */
#define WL1_NV_RACH_MASTER_CONTROL       0x1
#define WL1_NV_MAX_PREAMBLE_CONTROL      0x2
#define WL1_NV_MAX_NOACK_CONTROL         0x4

#define R99_UL_DPCCH_DROP_INTR_TIMEOUT_MSEC 20

/*==========================================================================

                         CONCURRENCY CONTROL PRIMITIVES

===========================================================================*/
// Macros to lock/unlock mutexes
#define EUL_AND_R99_UL_ATOMIC_MUTEX_LOCK()       rex_enter_crit_sect(&wenc_ext_cntrl_params.eul_and_r99_ul_mutex); REX_DISABLE_PREMPTION();
#define EUL_AND_R99_UL_ATOMIC_MUTEX_UNLOCK()     rex_leave_crit_sect(&wenc_ext_cntrl_params.eul_and_r99_ul_mutex); REX_ENABLE_PREMPTION(); 

#define EUL_AND_R99_UL_ISR_LOCK()                REX_ISR_LOCK(&wenc_ext_cntrl_params.eul_and_r99_ul_mutex)
#define EUL_AND_R99_UL_ISR_UNLOCK()              REX_ISR_UNLOCK(&wenc_ext_cntrl_params.eul_and_r99_ul_mutex)

#define UL_ISR_AND_DCH_RECONFIG_LOCK()           REX_ISR_LOCK(&ulcmd_ext_cntrl_params.l1_ul_dch_reconfig_isr_mutex)
#define UL_ISR_AND_DCH_RECONFIG_UNLOCK()         REX_ISR_UNLOCK(&ulcmd_ext_cntrl_params.l1_ul_dch_reconfig_isr_mutex)

#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK

#define WL1_TFC_EXCESS_PWR_INTLOCK()  REX_ISR_LOCK(&wenc_exp_cntrl_params.wl1_tfc_excess_pwr_states_mutex)
#define WL1_TFC_EXCESS_PWR_INTFREE()  REX_ISR_UNLOCK(&wenc_exp_cntrl_params.wl1_tfc_excess_pwr_states_mutex)

#define WL1_RACH_SUB_CHAN_AVAIL_INTLOCK() REX_ISR_LOCK(&wenc_ext_cntrl_params.wl1_rach_sub_chan_avail_mutex)
#define WL1_RACH_SUB_CHAN_AVAIL_INTFREE() REX_ISR_UNLOCK(&wenc_ext_cntrl_params.wl1_rach_sub_chan_avail_mutex)

#endif /*FEATURE_WCDMA_FREE_FLOATING_TASK*/

#define GET_UL_FW_TPC_ACCUM(carr) (wenc_ext_cntrl_params.wfw_intf_tx_async_read_ptr->r99[carr].tpcAdjAccumRd)

/*===========================================================================

                                 FUNCTIONS

===========================================================================*/

#ifdef FEATURE_WCDMA_16_QAM
extern uint32 eul_16_qam_get_rand_num( wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_16_QAM */

#ifdef FEATURE_WCDMA_HS_RACH

void wenc_hsprach_get_data( wsub_id_e_type wsub_id);

void wenc_hsprach_cleanup( wsub_id_e_type wsub_id);

void wenc_hsdpch_get_data( wsub_id_e_type wsub_id);

void wenc_hsdpch_cleanup( wsub_id_e_type wsub_id);

void wenc_hsdpch_set_mod_timing_evt_handler( wsub_id_e_type wsub_id);


void wenc_hsprach_set_mod_timing_evt_handler( wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION wenc_prach_process_logging

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void wenc_prach_process_logging(int16 txAgc,uint32 ul_last_aisum_val, wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION wenc_prach_process_tx

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
=========================================================================*/
void wenc_hsprach_process_tx_data( wsub_id_e_type wsub_id);
/*=========================================================================
FUNCTION wenc_phychan_set_mod_timing

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
=========================================================================*/
void wenc_phychan_set_mod_timing( wsub_id_e_type wsub_id);
/*=========================================================================
FUNCTION ENC_PRACH_GET_DATA

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
=========================================================================*/
void wenc_phychan_get_data( wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION ENC_HSRACH_DONE_ISR

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
=========================================================================*/

void enc_hsrach_done_isr( wsub_id_e_type wsub_id);
#endif /*FEATURE_WCDMA_HS_RACH*/

/*====================================================================
FUNCTION: ENC_ACCU_UL_DPCH_PWR_INFO_SAMPLE

DESCRIPTION:
  This function accumulates one UL_DPCH_PWR_INFO_LOG_PKT sample

DEPENDENCIES:
  NONE

RETURN VALUE:
  NONE

SIDE EFFECTS:
  NONE
====================================================================*/
void enc_accu_ul_dpch_pwr_info_sample
(
  uint8   tfci,
  boolean cm_frame_ind,
  uint8   beta_c,
  uint8   beta_d,
  uint8   cm_tx_pwr_delta
, wsub_id_e_type wsub_id);
/*=========================================================================
FUNCTION UL_INDICATE_AMR_ENABLED

DESCRIPTION
  This function is called from l1m_cphy_setup_req_cmd() whenever AMR is 
  added to the configuration

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void ul_indicate_AMR_enabled( wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION UL_INDICATE_AMR_DISABLED

DESCRIPTION
  This function is called from l1m_cphy_setup_req_cmd() whenever AMR is 
  removed from the configuration

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void ul_indicate_AMR_disabled( wsub_id_e_type wsub_id);


/*=========================================================================
FUNCTION WENC_THERMAL_EVT_REG

DESCRIPTION This routine is called during WL1 Init to register with NPA for events
            on PA Sensor Device

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void wenc_thermal_npa_evt_reg( wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION WENC_THERMAL_EVTINIT_CB

DESCRIPTION This routine is called after registering for events on 
            PA sensor device. This routine queries current state of PA. 
            Based on the state (OKAY, WARNING, CRITICAL) appropriate action 
            is taken (enable/disable thermal mitigation)

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void wenc_thermal_npa_evtinit_cb(void *context, 
                                 unsigned int event_type, 
                                 void *data, unsigned int data_size);


/*=========================================================================
FUNCTION WENC_THERMAL_EVTCHANGE_CB

DESCRIPTION This routine is called by NPA whenever there is a change in PA 
            Sensor State. 

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void wenc_thermal_npa_evtchange_cb(void *context, 
                                   unsigned int event_type, 
                                   void *data, unsigned int data_size);

/*=========================================================================
FUNCTION WENC_THERMAL_NPA_EVTHANDLER

DESCRIPTION This routine enables/disables thermal mitigation based on
            PA sensor state. The routine is called at the time of registration
            and everytime there is a change in event.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void wenc_thermal_npa_evthandler(wenc_thermal_npa_state_enum npa_curr_state, wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
/*=========================================================================
FUNCTION WENC_THERMAL_BACKOFF_STATE_TRANS

DESCRIPTION
  This routine takes care of transistion between different state of MTPL Backoff algorithm
  The state transistion is protected by a mutex.
  The input to the function is the next state of the algorithm

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void wenc_thermal_backoff_state_trans(
    mtpl_backoff_thermal_algo_state wenc_thermal_backoff_next_state
 , wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION WENC_THERMAL_BACKOFF_GET_CURR_STATE

DESCRIPTION
  This routine returns the current state of MTPL Backoff Algorithm

DEPENDENCIES

RETURN VALUE
  Current State of MTPL BACKOFF Algorithm (START, PAUSE, STOP)

SIDE EFFECTS
  None.
=========================================================================*/
mtpl_backoff_thermal_algo_state wenc_thermal_backoff_get_curr_state( wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION WENC_MTPL_BACKOFF_CLR_TIMERS

DESCRIPTION
  This routine clears duty cycle and step timers used for MTPL Backoff

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void wenc_thermal_backoff_clr_timers( wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION WENC_MTPL_BACKOFF_DUTYCYCLE_SIG_HANDLER

DESCRIPTION
  This routine handles the duty cycle timer. Based on the Up/Down switch
  appropriate MTPL value is set and timers are restarted.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void wenc_thermal_backoff_dutycycle_sig_handler(timer_cb_data_type cb_data);

/*=========================================================================

FUNCTION WENC_MTPL_BACKOFF_STEP_SIG_HANDLER

DESCRIPTION
  This routine handles the step timer. Based on thetimer count,
  new backoff value is calculated by which to reduce MTPL

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void wenc_thermal_backoff_step_sig_handler(timer_cb_data_type cb_data);

/*===========================================================================
FUNCTION        WENC_THERMAL_BACKOFF_FOR_VOICE_E911_CALL

DESCRIPTION     This function checks the current state of MTPL_BACKOFF state machine and 
                also checks whether any voice call or Emergency call is active and alters
                the state of MTPL_BACKOFF if required.
                If MTPL_BACKOFF is STARTED and if its a voice only call or E911 call , then PAUSE
                if MTPL_BACKOFF already Paused and if it is not a voice only call then START
 
DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void wenc_thermal_backoff_for_voice_e911_call( wsub_id_e_type wsub_id);

#endif /* FEATURE_WCDMA_MTPL_THERMAL_BACKOFF */

#ifdef FEATURE_WCDMA_L1_ZI_OPT
/* ===========================================================================

FUNCTION        UL_DATA_DUMP_LOG_PKT_ALLOC

DESCRIPTION     This function allocates the dynamic memory needed for the
                log packets and related logging structures

DEPENDENCIES    NONE

RETURN VALUE    None

SIDE EFFECTS    None

=========================================================================== */
void ul_data_dump_log_pkt_alloc( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        UL_DATA_DUMP_LOG_PKT_DEALLOC

DESCRIPTION     This function de-allocates the dynamic memory needed for the
                log packets and related logging structures

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_data_dump_log_pkt_dealloc( wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_L1_ZI_OPT */

/*===========================================================================

FUNCTION        UL_FW_RACH_DEBUG_LOG_CFG_STATUS_PARAMS

DESCRIPTION     This function records fw parameters which are related to rach
                procedure

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
void ul_fw_rach_debug_log_cfg_status_params( wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    ENC_LOG_RACH_DEBUG_CFG_PARAMS

DESCRIPTION This routine records RACH Config Parameters as part of
            UL_RACH_DEBUG_LOG_PKT 

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void enc_log_rach_debug_cfg_params(
  int8   cpich_tx_pwr,                    /* CPICH Tx Pwr */
  int16  tmp_ec,                          /* RSCP Pwr */
  int8   ul_if,                           /* UL Interference */
  int8   const_val,                       /* Constant Value  */
  int32  prach_init_tx_pwr,               /* Init Preamble Pwr*/
  uint8  max_num_rach_preambles,          /* Max Num of Rach Preambles */
  l1_dl_aich_tt_enum_type local_aich_tt   /* AICH timing */
  #ifdef FEATURE_WCDMA_HS_RACH
  , int8   poweroffset_pp_m                 /* preamble to msg pwr offset */
  #endif
, wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    ENC_LOG_RACH_DEBUG_STATUS_PARAMS

DESCRIPTION This routine records RACH Status Parameters as part of
            UL_RACH_DEBUG_LOG_PKT. It is called from enc_rach_done_isr()

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void enc_log_rach_debug_status_params( wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_PLT) || defined(FEATURE_WCDMA_PLT_MODE)
#error code not present
#endif /* defined(FEATURE_WCDMA_PLT) || defined(FEATURE_WCDMA_PLT_MODE) */

/*=========================================================================

FUNCTION WL1_ENC_INIT

DESCRIPTION
  This routine intializes the encoder driver. It initializes the TrCH mux
  order free queue and the TrCH mux queue.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void wl1_enc_init
( wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION WENC_GET_FW_BTF_DELAY

DESCRIPTION
  Get the latest fw btf delay

DEPENDENCIES

RETURN VALUE
  return btf_delay

SIDE EFFECTS
  None.

=========================================================================*/
extern int16 wenc_get_fw_btf_delay( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_BOLT_2_0
/*===========================================================================
FUNCTION     wenc_get_timing_info

DESCRIPTION  This function will return the current Tx bpg .

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS  
===========================================================================*/
uint8 wenc_get_timing_info( wsub_id_e_type wsub_id);
#endif
/*=========================================================================

FUNCTION ENC_GET_DATA

DESCRIPTION
  This function processes tx data delivered by MAC. It sets up
  appropriate encoder control registers and transfers tx data into ERAMa.
  It sets up TRCH_CFG_0_RAM, TRCH_CFG_1_RAM and TRCH_CTL_RAM as well as the
  number of active/erama transport channels. It calculates RM parameters, SF
  and Gain Factors. It has be done with all its processing within 6ms into the
  frame leaving the encoder at least 4 ms to cipher/encode ERAMa data.

DEPENDENCIES
  This function depends on MAC to have successfully built tx data into the
  Tx buffer before the GET_DATA event.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void enc_get_data
( wsub_id_e_type wsub_id);



/*=========================================================================
FUNCTION wl1_ul_rach_read_nv_item

DESCRIPTION This function read NV item to enable/disable NV controlled RACH debugging parameters

RETURN VALUE

SIDE EFFECTS
  None.
=========================================================================*/

extern void wl1_ul_rach_read_nv_item( wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION       wl1_ul_feature_bitmask_read_nv_item

DESCRIPTION    Function for reading UL Feature bitmask NV
  
DEPENDENCIES   NONE
  
RETURN VALUE   NONE
  
SIDE EFFECTS   NONE
=========================================================================*/
extern void wl1_ul_feature_bitmask_read_nv_item(wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION ENC_PC_ENABLE

DESCRIPTION
  In the first slot after OL power is set, re-enable closed loop power
  control and unfreeze the TxAgcAccum.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
extern void enc_pc_enable
( wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION ENC_RACH_DONE_ISR

DESCRIPTION
  This ISR is invoked by mDSP after a RACH transmit message request has been
  processed. It indicates the status of the transmit (ACK, NACK or a NO-ACK).

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void enc_rach_done_isr
( wsub_id_e_type wsub_id);



/*=========================================================================

FUNCTION ENC_CLEANUP

DESCRIPTION
  This routine re-initializes the encoder driver. It aborts all Tx
  processing in the encoder. It commands the mDSP to stop control channel
  processing. It disables Tx events on the sequencer Tx timeline.

  It resets ERAMb free address and the number of active TrCHs.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void enc_cleanup
( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WL1_ENC_KILL_PA

DESCRIPTION
  Kill the PA by calling ul_tx_disable()

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void wl1_enc_kill_pa( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION ENC_SET_CIPHERING_KEYS

DESCRIPTION
  This function updates the ciphering key for uplink ciphering operation
  to the ciphering key index locations requested.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
void enc_set_ciphering_key(
  /* ciphering key index (one of the 4 keys) to which the key is set */
  uint8 index,
  /* ciphering key info. Pointer to the array of 4 word32 (= 128 bit) */
  uint8 *ciphering_key_data, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ENC_CIPHER_TM_TB_KEY_STREAM

DESCRIPTION     This function generates the ciphering keystream for various
                transparant mode TBs. If there is only one TB in that is TM
                and need to be ciphered, it is ciphered in mormal mode using
                hardware so no ciphering keystrean is generated for such TB.

DEPENDENCIES    NONE

RETURN VALUE    NONE

SIDE EFFECTS    It calls function enc_gen_cipher_keystream so same
                side effects as that function.
===========================================================================*/


void enc_cipher_tm_tb_key_stream(
  /* number of TrCh to process */
  uint8 num_trch);

/*===========================================================================

FUNCTION ENC_PRINT_RACH_TIMING_DEBUG

DESCRIPTION
  This function prints debugging information to F3 when RACH no-ack happens.

DEPENDENCIES
  RACH no-ack must have just happened.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void enc_print_rach_timing_debug( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION WENC_SEND_RACH_ABORT_CMD

DESCRIPTION This routine sends mdsp prach cfg cmd to FW to abort any
            ongoing rach procedure.

DEPENDENCIES
  
RETURN VALUE  

SIDE EFFECTS  It stops further RACH TX.
==========================================================================*/
void wenc_send_rach_abort_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION WENC_PROCESS_RACH_ACTION

DESCRIPTION This routine processes actions triggered in FACH state.
            Based on the action, rach state transistion is taken care of.

DEPENDENCIES

RETURN VALUE Boolean value indicating whether the triggered action can proceed further
             or to wait till the current rach procedure is finished/aborted.
             TRUE - Proceed
             FALSE - Block till rach is done

SIDE EFFECTS
==========================================================================*/
boolean wenc_process_rach_action(wenc_rach_action_enum_type rach_curr_action, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION UL_UNFREEZE_TPC

DESCRIPTION
  This function unfreezes TPC accum.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  Unfreezes TPC accum
==========================================================================*/
void ul_unfreeze_tpc( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION UL_SEC_UNFREEZE_TPC

DESCRIPTION
  This function freezes/unfreezes TPC accum for secondary carrier.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  freezes/Unfreezes TPC accum for secondary carrier
==========================================================================*/
void ul_sec_unfreeze_tpc( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ENC_STOP_BUILD_FRAME_IND_FOR_RECFG

DESCRIPTION     This function cleans UL TX data buffer and stops
                build_fram_ind()

DEPENDENCIES    Must be called for DCH seamless recfg

RETURN VALUE    None

SIDE EFFECTS    As said above
==========================================================================*/
void enc_stop_build_frame_ind_for_recfg( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ENC_SUSPEND_BFI_ONE_FRAMES_BEFORE_ACT_TIME

DESCRIPTION     This function suspends build_fram_ind() one frames before the
                activation time. It does NOT clean UL TX data buffer.

DEPENDENCIES    Must be called within WCDMA_INTLOCK

RETURN VALUE    None

SIDE EFFECTS    As said above
==========================================================================*/
void enc_suspend_bfi_one_frames_before_act_time(l1_ext_cmd_type* cmd, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_rfm_tx_sleep

DESCRIPTION
  During LTA fast suspend procedure, RF TX chain has to be disabled & deinitialised before putting RF to sleep.
  Disable Tx chain & deinit Tx in the sleep done from FW after putting FW to Sleep/stop
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_tx_sleep(wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION ENC_CALC_CPICH_RSCP_FOR_OLPC

DESCRIPTION
  This function calculates CPICH RSCP value used for RACH or DPCH OLPC

DEPENDENCIES
  This function depends on RF for Io and SEARCHER for Ec/Io

RETURN VALUE
  int16, CPICH RSCP in dBm

SIDE EFFECTS
  None.

=========================================================================*/
int16 enc_calc_cpich_rscp_for_olpc( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ENC_UL_MAINT_EVENT_HANDLER

DESCRIPTION     This function is a maintainance function for UL. This is set
                to occur in the middle of frame. Currently it does following.
                1. Set Maintencence paremeters for UL which includes 
                   - Beta table upload to mDSP
                   - HS CM Demod and Mod table updation
                   - HS UL logging
                2. Send UL DPCCH Cmd to FW if Required based on Config Window

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void enc_ul_maint_event_handler( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ENC_SET_UL_MAINT_EVENT_PARAMETERS

DESCRIPTION     This function is a maintainance function for UL.
                This is called from the UL Maintenence Event Handler as well as from EUL FIQ Path.
                This function does the following:
                Beta table upload to mDSP
                HS CM Demod and Mod table updation
                HS UL logging

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void enc_set_ul_maint_event_parameters( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ENC_REFRESH_BETA_TABLE

DESCRIPTION     This function populates the beta table in mDSP from the current
                active beta table buffer. It doesnot adjust the mDSP beta table
                buffer to upload beta table to. It is intended to be called
                when beta table values have changes after it has been updated
                in UL maintainance event

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/

extern void enc_refresh_beta_table( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        ENC_SET_BC_BD_PAIR_VAL

DESCRIPTION     This function saves Bc and Bd values for beta table upload
                during UL maintainance function

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/

extern void enc_set_bc_bd_pair_val(
  /* Beta C value to set */
  uint8 ul_bc,
  /* Beta D value to set */
  uint8 ul_bd, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        ENC_SET_RATE_ADJ_IN_CM_DELTA

DESCRIPTION     This function saves rate adjust delta values for beta table
                upload during UL maintainance function

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/

extern void enc_set_rate_adj_in_cm_delta(
  /* rate adjust delta value */
  uint32 delta, wsub_id_e_type wsub_id);


/*=========================================================================

FUNCTION ENC_HDET_CONV_DONE_ISR

DESCRIPTION
  This ISR is invoked by mDSP after a HDET Conversion is ready for reading
  in order to compensate RF TX power limit.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  RF will be called to read HDET and update RF TX power limit

=========================================================================*/
void enc_hdet_conv_done_isr
( wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION ENC_TRCH_TB_REPORT

DESCRIPTION
  This function reports the number of trasnsport blocks sent out from each
  active transport channel. This function is called in TASK context every
  2 seconds.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  The buffer that holds the information for each transport channel is wiped
  clean.
=========================================================================*/
void enc_trch_tb_report
( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  ENC_GET_MTPL_MINUS_TPC_ACCU_HIST

DESCRIPTION
  This function is called by EUL EDPCH module in FIQ to get the history of
  MTPL minus TPC Accum for the past frame in TX AGC unit. 

DEPENDENCIES
  Called by EDPCH module in FIQ

RETURN VALUE
  const int16*, pointer to the history array
  return NULL if no history available

SIDE EFFECTS
  None
===========================================================================*/
const int16* enc_get_mtpl_minus_tpc_accu_hist( wsub_id_e_type wsub_id);

/*===========================================================================
  FUNCTION  WENC_GET_DPCCH_INIT_TX_PWR_AGC

DESCRIPTION
  This function returns DPCCH initial power in TX AGC unit

DEPENDENCIES
  Caller must be sure that DPCCH initial power is alrteady calc'ed

RETURN VALUE
  int32 DPCCH initial power in TX AGC unit

SIDE EFFECTS
  None
===========================================================================*/
int32 wenc_get_dpcch_init_tx_pwr_agc( wsub_id_e_type wsub_id);

/*===========================================================================
  FUNCTION  ENC_GET_LATEST_BETA_C

DESCRIPTION
  This function returns latest beta_C value

DEPENDENCIES
  None

RETURN VALUE
  uint32 for beta_C

SIDE EFFECTS
  None
===========================================================================*/
uint32 enc_get_latest_beta_c( wsub_id_e_type wsub_id);

/*===========================================================================
  FUNCTION  ENC_GET_LATEST_BETA_D

DESCRIPTION
  This function returns latest beta_D value

DEPENDENCIES
  None

RETURN VALUE
  uint32 for beta_D

SIDE EFFECTS
  None
===========================================================================*/
uint32 enc_get_latest_beta_d( wsub_id_e_type wsub_id);

/*===========================================================================
  FUNCTION  ENC_NEXT_FRAME_DPDCH_ACTIVE

DESCRIPTION
  This function returns whether next UL DPCH frame has DPDCH active

DEPENDENCIES
  This function can only be called in DCH state by EDPCH module after
  TX_GEN_GET_DATA event

RETURN VALUE
  boolean

SIDE EFFECTS
  None
===========================================================================*/
boolean enc_next_frame_dpdch_active( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  ENC_HAS_DPCH_DONE_PCP

DESCRIPTION
  This function returns TRUE if UL DPCH is TXing normal DPCH after PCP;
  otherwise FALSE.

DEPENDENCIES
  This function can only be called in DCH state by EDPCH module during EDPCH
  event that is after UL TX_GEN_GET_DATA event.

RETURN VALUE
  boolean

SIDE EFFECTS
  None
===========================================================================*/
boolean enc_has_dpch_done_pcp( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  ENC_IS_DPCH_IN_SRB_DELAY

DESCRIPTION
  This function returns TRUE if UL DCH is in SRB delay;
  otherwise FALSE.

DEPENDENCIES
  This function can only be called in DCH state by EDPCH module during EDPCH
  event that is after UL TX_GEN_GET_DATA event.

RETURN VALUE
  boolean

SIDE EFFECTS
  None
===========================================================================*/
boolean enc_is_dpch_in_srb_delay( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        enc_check_seamless_for_eul

DESCRIPTION     This function checks if the present recfg is a simple DCH seamless recfg

DEPENDENCIES    Modifies the enc_seamless_dch_reconfig_flag which will disable the
                EUL tx only if it is not a seamless reconfig

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
void enc_check_seamless_for_eul(l1_ext_cmd_type* ext_cmd, wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_REL6
/* Functions to calculate the Maximum Power Reduction at MTPL */

/*===========================================================================
FUNCTION enc_calc_mpr_value_enh

DESCRIPTION
  This function is called to calculate the mpr value based on the
  Enhanced Rel 6 MPR algorithm. MPR bins for each hs type are directly 
  written to FW async structures. PACK/UNPACK model is removed.

DEPENDENCIES
  None

RETURN VALUE
  Current Beta_Ed index.

SIDE EFFECTS
  None
===========================================================================*/
extern uint16 enc_calc_mpr_value_enh(ul_mpr_calc_parms *
                                     #ifdef FEATURE_WCDMA_16_QAM
                                     ,l1_e_max_ch_codes_enum_type etfc_coding_sf
                                     #endif /* FEATURE_WCDMA_16_QAM */
                                     , wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_DC_HSUPA 
/*===========================================================================
FUNCTION enc_fetch_dc_mpr_value_to_fw

DESCRIPTION
For a given combination of mu, betahs, betaec, betaed, this function will look up moment value and write to FW interface
wfw_intf_tx_eul_common_async_write_ptr.
The enchs_rhs_type_enum_type input can indicate the Primary or Secondary by its value: ENCHS_NUM_RHS_TYPE means 
secondary.type_enum_type input can indicate the Primary or Secondary by its value: ENCHS_NUM_RHS_TYPE means 
secondary.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void enc_fetch_dc_mpr_value_to_fw(uint8 mu, uint16 betahs, 
		uint16 betaec, uint16 betaed, enchs_rhs_type_enum_type betahs_type, uint8 carr_idx, wsub_id_e_type wsub_id);


#endif /*FEATURE_WCDMA_DC_HSUPA  */


#ifdef FEATURE_WCDMA_REL7P1_MPR
/*===========================================================================
FUNCTION UL_MPR_GET_RHO_ED_IDX

DESCRIPTION
  See ul_mpr_find_rho_idx(uint16, RHO_Ed) 
  

DEPENDENCIES
  Takes A_edx15 value as input

RETURN VALUE
  Correspondong rho_ed index. 

SIDE EFFECTS
  None
===========================================================================*/
extern uint16 ul_mpr_get_rho_ed_idx(uint16, wsub_id_e_type wsub_id);
#endif /*FEATURE_WCDMA_REL7P1_MPR*/

#endif /* FEATURE_WCDMA_REL6 */

/*==========================================================================
FUNCTION: ENC_SET_DPCH_INIT_TX_PWR_THROUGH_QXDM

DESCRIPTION:
  This function gives the flexibility to set the value of DPCH INIT PWR to a
  fixed value through QXDM

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:

==========================================================================*/
void enc_set_dpch_init_tx_pwr_through_QXDM(int16 val, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION WENC_GET_TXAGC_HIST

DESCRIPTION
  This function is called from L1 command handler and updates history
  table of txAGC values for past 15 slots in TX AGC units, taking into 
  account HDET corrections and CM slots

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Global wenc_tx_pwr_hist_agc[] and wenc_mpr_val_hist[] is copied to the buffer passed in
==========================================================================*/
extern void wenc_get_txagc_hist(int16 *txagc_buf, int16 *mpr_buf, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION enchs_set_cqi_override_from_diag

DESCRIPTION
  This function set CQI test mode for UL HS DPCCH. It checks override
  status in mDSP. If mDSP doesn't have override eanabled then it programs
  mDSP with override immediatey.
  If mDSP has already override enabled then it sets up pending flag and UL
  maintainance event will manage uprating pending override.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void enchs_set_cqi_override_from_diag(
  /* indicate override by CQI test pattern.
     TRUE override enabled, FALSE is DIABLED */
  boolean override,
  /* CQI repetition cycle repeat count. Must be minimum 1 for CQI test
     pattern to be effective */
  int16 cqi_repetition_cycle_repeat_count,
  /* length of ACK/NACK pattern. Max 8 */
  uint8 cqi_pattern_len,
  /* Array pointer to ACK/NACK/DTX pattern */
  uint8 cqi_test_pattern, wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION ul_tx_ctrl

DESCRIPTION


DEPENDENCIES

RETURN VALUE
    None.

SIDE EFFECTS
  None.

=========================================================================*/
extern boolean ul_tx_ctrl(boolean enable_tx, wl1_ul_carr_id_enum_type carr_idx, wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION wl1_enc_init_register_writes

DESCRIPTION
  This routine intializes the UL tx registers that are initialized during
  wl1m_init(). This API is required to be called by MCPM Tx as call back
  after bringing a modem block out of power collapse.

DEPENDENCIES

RETURN VALUE
    None.

SIDE EFFECTS
  None.

=========================================================================*/
extern void wl1_enc_init_register_writes_cb(wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION wl1_enc_init_register_writes_cb_sub0

DESCRIPTION
This is a wrapper function to Register with MCPM for SUB 0 
DEPENDENCIES

RETURN VALUE
    None.

SIDE EFFECTS
  None.

=========================================================================*/
extern void wl1_enc_init_register_writes_cb_sub0(void);

/*=========================================================================

FUNCTION wl1_enc_init_register_writes_cb_sub1

DESCRIPTION
This is a wrapper function to Register with MCPM for SUB 1

DEPENDENCIES

RETURN VALUE
    None.

SIDE EFFECTS
  None.

=========================================================================*/
extern void wl1_enc_init_register_writes_cb_sub1(void);

/*===========================================================================
FUNCTION wcdma_query_tx_power_dbm_swap

DESCRIPTION
  This is the callback funtion for the timer and gets called every 670ms.
  It updates the buffer index for the max power double buffer and clears the
  old buffer to WCDMA_QUERY_TX_POWER_DBM_NO_INFO. After clearing the buffer
  the timer is again reinitialized.

DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None
===========================================================================*/
void wcdma_query_tx_power_dbm_swap(timer_cb_data_type cb_data);

#ifdef FEATURE_WCDMA_RACH_TX_PWR
/*===========================================================================
FUNCTION wcdma_control_rach_tx_pwr

DESCRIPTION
         wcdma_control_rach_tx_pwr: This API may make UE non-compliant
         to 3GPP and should be used only for debug and test


         Range 1: -57 < rach_opt_min_pwr <= 23

         Range 2: rach_opt_adj_val = 0 User mistake and warning F3.
                  10 <= rach_opt_adj_val < 70; Warning
                  rach_opt_adj_val > 70; Discard input.      

DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None
===========================================================================*/
/* Proto-type wenc.h */	
void wcdma_control_rach_tx_pwr(uint8 init_rach_opt_adj_val, int8 init_rach_opt_min_pwr, wsub_id_e_type wsub_id);
#endif 

/*===========================================================================

FUNCTION WL1_ENC_DYNAMIC_MEM_ALLOC

DESCRIPTION
  Allocates memory for all the variables dynamically allocated in UL ENC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void wl1_enc_dynamic_mem_alloc( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WL1_ENC_DYNAMIC_MEM_DEALLOC

DESCRIPTION
  Deallocates memory for all the variables dynamically allocated in UL ENC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void wl1_enc_dynamic_mem_dealloc( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_query_tx_power_dbm

DESCRIPTION
  This function returns the greater of the max TxAGC values computed for
  the immediately previous 670ms time period and current running time period.
  If not in DCH state it will return WCDMA_QUERY_TX_POWER_DBM_NO_INFO.

DEPENDENCIES
  None

RETURN VALUE
  int16. Returns max TxAGC value which is 1 sec old on an average.
 
SIDE EFFECTS
  None
===========================================================================*/
int16 wcdma_query_tx_power_dbm( void);

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
/*===========================================================================
FUNCTION       WL1_INIT_UL_QTA_VARS

DESCRIPTION    This funciton will reset all the QTA related variables
  
DEPENDENCIES   None
 
RETURN VALUE   Will reset all the QTA related variables

SIDE EFFECTS   None
===========================================================================*/
extern void wl1_init_ul_qta_vars( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION       WL1_QTA_FREE_PENDING_PDU_MEM

DESCRIPTION    This funciton will clear the DSM items that is built just before making the QTA state to active
  
DEPENDENCIES   None
 
RETURN VALUE   Will reset all the QTA related variables

SIDE EFFECTS   None
===========================================================================*/
extern void wl1_qta_free_pending_pdu_mem(l1_ul_tx_data_type *l1_ul_tx_buf_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION WENC_QTA_PROCESS_RACH_ACTION

DESCRIPTION  This is a wrapper routine for wenc_process_rach_action() function
             called by FRM (Frame Reservation Manager) to abort ongoing RACH
             procedure before continuing with QTA procedure. 

DEPENDENCIES None

RETURN VALUE None 

SIDE EFFECTS Aborts any ongoing RACH procedure
==========================================================================*/
boolean wenc_qta_process_rach_action(boolean abort_rach, wsub_id_e_type wsub_id);
#endif /* FEATURE_QTA */

/*===========================================================================
FUNCTION enc_clean_trch()

DESCRIPTION
This function is used to clean up Transport channel data and related globals

DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None
===========================================================================*/
void enc_clean_trch( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION    WENC_POST_LOCAL_CMD_DONE_TO_L1M

DESCRIPTION
  Posts drop/cleanup command done to L1M.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void wenc_post_local_cmd_done_to_l1m( l1_ul_phychan_enum_type ul_phychan , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION    WL1_SET_DPCH_DROP_TIMEOUT

DESCRIPTION
  Sets Timeout for DPCH channel drop. This function is called in pre-callback
for MDSP DPCH disable command to set the Timeout just before the disable command
is configured to FW.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void wl1_set_dpch_drop_timeout(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_QUERY_MODEM_TX_POWER 

DESCRIPTION
   This function computes Tx Modem power in IDLE/FACH and connected state as
   LOW, MEDIUM or HIGH.

DEPENDENCIES
  None

RETURN VALUE
  wl1_ul_modem_tx_power_enum_type: Modem power is HIGH/MEDIUM or LOW.
 
SIDE EFFECTS
  None
===========================================================================*/
extern wl1_ul_modem_tx_power_enum_type wl1_query_modem_tx_power( void);

/*===========================================================================
FUNCTION WENC_UPDATE_TX_PWR_HIST_AGC

DESCRIPTION
  This function reads MDSP and updates history table of txAGC values
  for past 15 slots

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Global wenc_tx_pwr_hist_agc[] and wenc_mpr_val_hist[] are updated from MDSP
==========================================================================*/
extern void wenc_update_tx_pwr_mpr_hist_agc( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wenc_gcf_tx_pwr_hist_update(void)

DESCRIPTION
  This function 
    update the min/max Tx Power value in history
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

===========================================================================*/
void wenc_gcf_tx_pwr_hist_update( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wenc_gcf_tx_accum_hist_update(void)

DESCRIPTION
  This function 
    update the min/max increase Tx accum value in history
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wenc_gcf_tx_accum_hist_update( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wenc_power_backoff_if_gcf(void)

DESCRIPTION
  This function 
    set power headroom to 0 if UE is under GCF test;
    otherwise, keep power headroom +2 dB for field performance
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wenc_power_backoff_if_gcf( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WENC_EXT_CNTRL_PARAMS_INIT

FILE NAME

DESCRIPTION     One time init of UL globals.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
extern void wenc_ext_cntrl_params_init(void);

#ifdef FEATURE_WCDMA_HS_RACH
/*===========================================================================
FUNCTION     wenc_is_hs_rach_configured

DESCRIPTION
                    This function is a external wrapper for the WL1_HS_RACH_CONFIGURED() API.
                    This is to avoid other modules having to include wl1hsrachcntrlr.h
  
DEPENDENCIES
                    None

RETURN VALUE
                    TRUE : HSRACH is configured
                    FALSE : HSRACH is not configured
  
SIDE EFFECTS
                    None
===========================================================================*/
boolean wenc_is_hs_rach_configured(wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_HS_RACH */

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC

/*===========================================================================
FUNCTION     wl1_ul_metric_cb

DESCRIPTION
  Call back from QSH module to collect the UL metrics.
  
DEPENDENCIES
  This will be invoked by WL1 QSH module whenever QSH module calls
  the WL1 CB with one of the UL Metrics Tags set.

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern boolean wl1_ul_metric_cb(wl1_qsh_metric_e metric_id, 
                                uint8* addr_ptr, 
                                wl1_qsh_metric_action_enum_type action, 
                                wsub_id_e_type wsub_id);

#endif /* FEATURE_WCDMA_QSH_EVENT_METRIC */


#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================
FUNCTION     wl1_ul_dump_cb

DESCRIPTION
  Dump UL related Globals for UL minidump collection
    
DEPENDENCIES
  This API will be called from WL1 QSH module, which will be invoked by QSH

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_ul_dump_cb(void* addr_ptr);

/*===========================================================================
FUNCTION     wl1_ul_populate_state_vars

DESCRIPTION
  Dump various UL related State Variables and Misc Globals for UL minidump collection
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_ul_dump_state_vars_and_misc_globals(wl1_ul_mini_debug_info_struct_type * ul_mini_dump_ptr);

/*===========================================================================
FUNCTION     wl1_ul_dump_rach_vars

DESCRIPTION
  Dump PRACH related Globals for UL minidump collection
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_ul_dump_rach_vars(wl1_ul_mini_debug_info_struct_type *ul_mini_dump_ptr);

/*===========================================================================
FUNCTION     wl1_ul_dump_dpch_vars

DESCRIPTION
  Dump UL DPCH related Globals for UL minidump collection
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_ul_dump_dpch_vars(wl1_ul_mini_debug_info_struct_type * ul_mini_dump_ptr);

/*===========================================================================
FUNCTION     wl1_ul_dump_cpc_dtx_drx_edrx_vars

DESCRIPTION
  Dump CPC DTX/DRX and EDRX related Globals for UL minidump collection
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_ul_dump_cpc_dtx_drx_edrx_vars(wl1_ul_mini_debug_info_struct_type * ul_mini_dump_ptr);

#ifdef FEATURE_WCDMA_HS_RACH
/*===========================================================================
FUNCTION     wl1_ul_dump_hs_rach_vars

DESCRIPTION
  Dump HS RACH related Globals for UL minidump collection
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_ul_dump_hs_rach_vars(wl1_ul_mini_debug_info_struct_type * ul_mini_dump_ptr);
#endif /* FEATURE_WCDMA_HS_RACH */


/*===========================================================================
FUNCTION     wl1_dump_edch_cm_params

DESCRIPTION
  Dump EDCH CM related params for UL Minidump collection
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_dump_edch_cm_params(wl1_debug_ul_edch_cm_info_struct_type *edch_cm_dump_ptr);

/*===========================================================================
FUNCTION     wl1_dump_eul_isr_timing

DESCRIPTION
  Dump EUL ISR timing related info for prev 3 instances of ISR firing for minidump
  collection.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_dump_eul_isr_timing(wl1_debug_ul_eul_isr_timing_struct_type *wl1_eul_timing_dump_ptr);

/*===========================================================================
FUNCTION     wl1_ul_dump_edch_vars

DESCRIPTION
  Dump all EDCH related globals for UL minidump collection
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_ul_dump_edch_vars(wl1_ul_mini_debug_info_struct_type * ul_mini_dump_ptr);

/*===========================================================================
FUNCTION   wl1_dump_edch_phychan_cfg_struct

DESCRIPTION
  Dump EDCH Phychan/Cfg related params and globals for UL minidump collection
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_dump_edch_phychan_cfg_struct(wl1_debug_ul_e_ul_sig_cfg_db_struct_type *e_ul_cfg_db_dump_ptr);

/*===========================================================================
FUNCTION     wl1_ul_dump_edl_vars

DESCRIPTION
  Dump EDL related globals for UL minidump collection
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_ul_dump_edl_vars(wl1_ul_mini_debug_info_struct_type * ul_mini_dump_ptr);

/*===========================================================================
FUNCTION     wl1_ul_dump_agch_vars

DESCRIPTION
  Dump E-AGCH related globals and params for UL minidump collection
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_ul_dump_agch_vars(wl1_ul_mini_debug_info_struct_type * ul_mini_dump_ptr);

/*===========================================================================
FUNCTION     wl1_ul_dump_e_dl_rl_info_vars

DESCRIPTION
  Dump E-HICH/RGCH related globals for UL Minidump Collection
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_ul_dump_e_dl_rl_info_vars(wl1_ul_mini_debug_info_struct_type *ul_mini_dump_ptr);

#endif /* FEATURE_WCDMA_QSH_DUMP*/

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION     wl1_qsh_mdump_collect_wenc_int_params

DESCRIPTION
  Dump WENC INT CNTRL PARAMS
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_wenc_int_params(wsub_id_e_type wsub_id);
#endif
#endif /* WENC_H */
