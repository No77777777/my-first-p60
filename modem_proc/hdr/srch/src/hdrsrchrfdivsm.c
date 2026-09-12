/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             H D R    S R C H    R F    D I V   S T A T E    M A C H I N E

GENERAL DESCRIPTION
      This module contains functions for the HDR Diversity State Machine

INITIALIZATION AND SEQUENCING REQUIREMENTS

    Copyright (c) 2009 - 2021 by Qualcomm Technologies, Inc.  All Rights Reserved.

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchrfdivsm.c#4 $ $DateTime: 2021/05/18 05:54:37 $ $Author: pwbldsvc $

when         who     what, where, why 
----------   ---     ----------------------------------------------------------
05/18/2021   vaa     Fix for compilation errors in MSIM disable build flavor
02/06/2020   vaa     FR54762: HDR Concurrency Mgr changes 
12/04/2019   vaa     Changes to support clk plan V2.0 FR changes
09/11/2018   vlc     Check for connection close before setting diversity 
                     preference 
04/05/2018   vke     Delay release div if div search is active in idle/access state
03/19/2018   vke     Ignore diversity grant when runtime preference is disabled
02/27/2018   pga     Fix KW errors
08/24/2017   rmv     Mainline FEATURE_HDR_QTA_THRU_TRM 
1/16/2017    vko     Fixed HDR processed Unlock when Diversity is already disabled for QTA
04/18/2016   rmv     Keep duration for DRx R&N, 2ms less than that of PRx
12/16/2015   vko     Fixed KW issues
11/24/2015   wsh     Added logic to handle DIV grant after TX in QTA
11/18/2015   arm     Make sure we have valid chan id before rf warmup
09/21/2015   rmv     Release diversity in 4a->3a, 4a->4c cases
07/13/2015   arm     Mainlined FEATURE_RF_SVDO_API feature.
07/08/2015   rmv     Check and delay release div if div search is active 
07/06/2015   wsh     Added delay for DIV release/request 
06/29/2015   vko     Pass subreason also, while calling the TRM API
05/04/2015   mbs     Supported QTA using TRM API
02/04/2015   arm     Check if primry chain is settled before sending QTA_END 
                     to TRM
01/09/2015   sat     Updating Diversity change to MCS and FW before Diversity enabled.
08/27/2013   wsh     Fixed crash in 2nd chain test
11/13/2014   vke     Call ARD SM exit TC command only when entering the reacq
                     in hdrsrchrfdiv_diversity_disable_after_qta
11/26/2014   sat     Added hdrsrchrfdiv_enable_diversity_chain API
10/31/2014   vke     Skip disabling ARD SM in 4a to 3a state change 
                     when QTA is enabled
10/27/2014   sat     Clearing Diversity Grant command in SM while releasing Diversity.
10/14/2014   wsh     Changes to support Jolokia bringup
10/13/2013   arm     Updated hdrsrchutil_calc_time_to_tune_away to return if
                     TA time was updated or not
10/06/2014   wsh     Fixed compile warning
09/29/2014   arm     HDR DR: request RX_ANY for div in DR mode, to make sure 
                     dev 3 can be granted when DO primary is dev 2. 
09/23/2014   sat     Checking band compatibility for Diversity if device 
                     not calibrated properly.
09/22/2014   arm     HDR QTA code cleanup 
08/28/2014   arm     Changes to support QTA with diversity
08/26/2014   bb      Changes to handle diversity release when unlock event is 
                     ignored by primary client during Access state
08/12/2014   tnp     Fix compiler warnings
07/16/2014   rmv     Use TRM_RX_DIVERSITY to check Div for band while querying 
                     TRM 
07/08/2014   arm     Updated TRM API for get_device_mapping
05/15/2014   arm     TRM API changes for DR-DSDS. 
05/14/2014   vke     Fix to prevent blocking of TAs for 1x and G due to 
                     bug in the HDR TRM-interface
03/17/2014   vke     Change the request priority before calling retain lock
12/06/2013   arm     Supported Div chain frequency and power reporting.
09/25/2013   wsh     Changes to support BOLT bring up
09/04/2013   wsh     Changes to support Bolt RUMI
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
08/05/2013   wsh     Fixed 2nd chain test call on Dime
06/26/2013   wsh     Added Selective TxD feature
05/31/2013   wsh     Check w/ TRM if DIV is enabled for band before reuqesting
05/31/2013   rkc     Added SYNC state check in HDRSRCHRFDIV_GET_DIV_TRM_PRIORITY 
05/14/2013   vke     Moved reporting of mcpm diversity status to 
                     appropriate place
05/28/2013   smd     Used access priority to request div chain for access.
03/26/2013   arm     Removed per band diversity bit, only use band mask now.
02/21/2013   smd     Allowed diversity chain tune away first if primary chain 
                     release is done after unlock_by time.
01/06/2013   arm     Lowered TRM priority for div in idle/access/acq state.
01/25/2013   smd     Fixed HDR not releasing div when tune away is disabled in 
                     TC state.  
09/25/2012   rmg     Added support for including freq info in TRM interaction.
08/31/2012   wsh     Respect hard tuneaway rampdown
08/26/2012   arm     Fixed underflow for diversity tuneaway time left. 
08/24/2012   smd     Reset rl_state for div chain at release and retain_lock.
08/20/2012   arm     Released div chain before trm grant ending time.
08/03/2012   arm     Perform DRC rampdown in traffic state only. 
08/02/2012   arm     Moved to hdrsrch_tcb macro for dime.  
07/11/2012   wsh     Send HDRSRCHRFDIV_HARD_TA_REQUEST_DIV_CMD in clr_null_cover_cb()
06/06/2012   arm     Do not error fatal if we have bad rf cal. 
05/17/2011   smd     Fixed HDR not releasing div when tune away is disabled. 
05/25/2012   wsh     Moved ARD logic into it's own state machine
05/21/2012   ljl     Skipped RF APIs for Dime RUMI IQ interface.
04/20/2012   arm     Supported diversity in reacq. 
04/17/2012   ljl/rkc/glie Disabled div at warmup enter if div was not allowed.     
04/12/2012   ljl     Fixed featurization.
03/24/2012   ljl     Removed sample server interface. 
03/20/2012   ljl     Fixed compiler error for Dime.
03/16/2012   arm     Resolve TCB errors for Dime. 
03/13/2012   ljl     Added HDRSRCHRFDIV_HARD_TA_REQUEST_DIV_CMD and 
                     hdrsrchrfdiv_hard_ta_runtime_pref_ctrl_tf().
03/08/2012   wsh     Removed deprecated sample server interface
03/09/2012   arm     Disabled equalizer during second chain test call. 
02/29/2012   smd     Added a F3 message.
02/08/2012   rkc     Added ability to resume tuneaway via hdrsrchrfdiv_resume_tuneaway().
01/06/2012   smd     Set diversity enable delay time to 0. 
01/10/2012   ljl     Allowed ARD only in traffic state.
01/09/2012   smd     Supported disabling div when div is in inactive state.
01/06/2012   smd     Set diversity enable delay time to 0 and removed not-needed 
                     rfm calls when enabling diversity chain. 
01/04/2012   rkc     Added hdrsrchrfdiv_fl_adapt_disable_div_tf().
12/22/2011   rkc     Registered FL callback if ARD timeout during DRC rampdown.
12/21/2011   grl     Fixed ARD disable during second chain test call. 
12/21/2011   grl     Fixed crash when 2nd chain mode enabled while in traffic
12/08/2011   arm     Change Diversity timer from 750 ms to 100 ms.
11/29/2011   arm     Removed compiler warnings.
11/11/2011   arm     HDR idle and access state diversity implemetation.
11/10/2011   lyl     Handled FL ARD disabling in inactive state.
10/25/2011   lyl     Supported HDR FL data driven adaptive RxD. 
10/10/2011   arm     Supported second chain test call feature.
10/17/2011   rmg     Removed calls to obsolete clk regim APIs.
09/22/2011   rmg     Removed call to obsolete function hw_set_system_mode().
09/01/2011   lyl     Added function /variable declaration.
08/10/2011   ljl     Set the enabled flag to false after disable rx. 
07/06/2011   lyl     Added hdrsrchrfdiv_get_gpio_status().
06/15/2011   kss     Added rfm_enter_mode() call for new RF SV API.
06/02/2011   lyl     Supported HDR FL data driven adaptive RxD. 
04/29/2011   lyl     Stored pred. adjustment before tuneaway for svlte type 2.
04/27/2011   rmg     Added HDR MCPM support changes.
04/19/2011   lyl     Replaced the GPIO port number with macro.
04/01/2011   kss     Integrated RxLM APIs. 
03/31/2011   lyl     Defined GPIO port for PCS antenna switch feeded to MDM when
                     FEATURE_8X60_DUAL_RADIO_FUSION is defined.
01/21/2011   rmg     Supported RF API changes for NikeL.
11/04/2010   lyl     Fixed diversity switch issue under TAP call.
                     Removed the unecessary Eq diversity udpate.
10/21/2010   lyl     Fixed undefined reference.
10/19/2010   lyl     Added FEATURE_HDR_SVDO_DIV_SUPPORT around gpio config func.
10/13/2010   lyl     Supported fusion diversity.
09/22/2010   lyl     Fixed compiler warning.
09/01/2010   grl     Fixed timer error fatal.
08/16/2010   grl     Make sure clocks are synced after TCXO shutdown.
08/16/2010   grl     Resolved warning when getting the RX power.
08/13/2010   ljl     Disabled div chain only if it had been enabled.
08/10/2010   lyl     Fixed the null DRC covering sinr dips issue.
07/28/2010   lyl     Updated for runtime div ctrl for more use cases.
07/12/2010   lyl     Enabled diverstiy if runtime div ctrl is on.
07/12/2010   lyl     Fixed diversity status change by gpio with edge trigger.
07/07/2010   ljl     Disabled diversity chain before releasing it.
07/06/2010   grl     Used correct AGC values when deciding to disable div. 
06/24/2010   cnx     Added return value in hdrsrchrfdiv_change_div_status_tf()
                     Removed STM redeclarations.
06/21/2010   cnx     Fixed compiler warnings.
06/10/2010   lyl     Supported runtime diversity preference control.
06/02/2010   ljl     Removed mdsp.h.
05/26/2010   ljl     Supported hdrfmac_get_drc_length().
05/21/2010   ljl     Added gpio related header files and fixed compile error.. 
05/19/2010   ljl     Supported SVDO diversity.
03/25/2010   pxu     Removed FEATUER_HDR_EQ and FEATURE_HDRSRCH_ROTATORS feature checks. 
03/17/2010   ljl     Configured clocks for diversity chain. 
03/01/2010   rkc     Enabled/Disabled CDMA Rx Chain 1 Clocks.
02/26/2010   wsh     Fixed lint warnings
02/01/2010   rkc     Moved TRM retain lock from warm-up to enabled state.
12/07/2009   lyl     Shorten the null cover timeline for single carrier;
                     Small packet check for all carriers during hard div tuneaway.
11/02/2009   wsh     Fixed LINT warnings
10/23/2009   rkc     Configured h/w and rf mode for diversity.
10/23/2009   rkc     Verified return code of sample_server_configure().
10/21/2009   rkc     Configured sample server for div chain when granted.
10/19/2009   wsh     Fixed lint warnings
08/11/2009   grl     Fixed diversity request conditions.
07/13/2009   rkc     Moved rfm_hdr_disable_diversity() call inside
                     FEATURE_RF_SVDO_API featurization.
05/13/2009   grl     Initial revision.

===========================================================================*/ 

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "hdrsrchrf.h"

#include "hdrsrchstate.h"
#include "hdrsrchmcpm.h"
#include "hdrsrchstates.h"
#include "hdrsrchtask.h"
#include "hdrsrchutil.h"
#include "hdrsrchi.h"
#include "hdrsrchset.h"
#include "hdrmdsp.h"

#include "rfm_hdr.h"
#include "lm_types.h"
#include "rxlm_intf.h"
#include "rfm_cdma.h"

#include "hdrmdspmcr.h"

#include "hdrdebug.h"

#include "hdrsrchafc.h"
#include "hdrsrcheq.h"
#include "hdrsrchfing.h"
#include "hdrsrchdrv.h"

#include "trm.h"
#include "rex.h"
#include "timetick.h"
#ifndef FEATURE_HDR_DIME_MODEM
#include "hw.h"
#endif /* FEATURE_HDR_DIME_MODEM */
#ifdef FEATURE_HDR_REVB
#include "amssassert.h"
#endif /* FEATURE_HDR_REVB */

#include "hdrsrchrfi.h"
#include "hdrsrchrfdiv.h"
#include "stm.h"
#include "hdrsrchcom_sm.smh"
#include "hdrsrchrfdiv_sm.smt"
#include "hdrfmac.h"
#include "hdrsrchmcpm.h"

#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
#include "tlmm_gpio.h"
#include "gpio_int.h"
#include "tramp_gpio.h"
#include "DDITlmm.h"
#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */

#include "cmgr_api.h"

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
#include "hdrsrch_sm.smh"
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */ 

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
#include "hdrsrchard.h"
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
#include "hdrmultirat.h"
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

#ifdef FEATURE_HDR_RUMI
#include "hdrhitmsg.h"
#endif /* FEATURE_HDR_RUMI */
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* Turn on this feature all the time. This is a change to improve SHDR
   throughput during best effort TCP connections. */
#define FEATURE_HDR_HARD_DIV_TUNE_AWAY

/* Retrieve the diversity combining mode from the diversity NV item. */
#define HDRSRCHRFDIV_GET_COMBINING_MODE( comb ) \
  ( ( hdrsrchrf_comb_mode_enum_type ) ( ( comb >> 1 ) & 0x3 ) )

/* Retrieve the antenna selection from the diversity NV item. */
#define HDRSRCHRFDIV_GET_ANT_SEL( comb ) \
  ( ( hdrsrchrf_comb_ant_enum_type ) ( ( comb >> 3 ) & 0x3 ) )

#define HDRSRCHRFDIV_NAT_NUM( num ) ( ( ( num ) ) < 0 ? 0 : ( num ) )
  /* Natural number conversion. */

#ifdef FEATURE_HDR_HARD_DIV_TUNE_AWAY
#define HDRSRCHRFDIV_DRC_NULL_PERIOD_END_MARGIN_SCLK TIMETICK_SCLK_FROM_MS(2)
  /* Diversity shutdown NULL period should end this time before the chain is
     taken and tuned. */

#define HDRSRCHRFDIV_PRI_CHAIN_PRED_TIME_MS             \
          ( ( hdrsrchset.subaset_cnt > 1 ) ? 17 : 12 )
  /* How long ahead of the release should we switch to the primary 
     chain predictor. */

/* The ramp down buffer used for the different DRC lengths. */
#define HDRSRCHRFDIV_DRC_LEN_8_RAMP_DOWN -10
#define HDRSRCHRFDIV_DRC_LEN_4_RAMP_DOWN -3
#define HDRSRCHRFDIV_DRC_LEN_2_RAMP_DOWN 0

#if defined T_MDM9X00 || defined FEATURE_HDR_DIME_MODEM
/* Revisit in MC case */
#define HDRSRCHRFDIV_UNLOCK_TIME_BUF_MS                \
          ( ( hdrsrchset.subaset_cnt > 1 ) ? 12 : 9 )
  /* How long in ms we should try to unlock the diversity lock before the 
     deadline. For single carrier or revA, the time line is 3 ms shorter 
     for less null covering */
#else
#define HDRSRCHRFDIV_UNLOCK_TIME_BUF_MS                \
          ( ( hdrsrchset.subaset_cnt > 1 ) ? 10 : 5 )
  /* How long in ms we should try to unlock the diversity lock before the 
     deadline. For single carrier or revA, the time line is 5 ms shorter 
     for less null covering */
#endif

#define HDRSRCHRFDIV_TA_OVERHEAD_MS 2   
  /* Overhead time in milliseconds extra time we should give for
     scheduling the TA. */

#define HDRSRCHRFDIV_TA_OVERHEAD_SCLK  \
        TIMETICK_SCLK_FROM_MS( HDRSRCHRFDIV_TA_OVERHEAD_MS )
  /* Overhead time in milliseconds extra time we should give ourselves for
     scheduling the TA. */

#define HDRSRCHRFDIV_MIN_DIV_TIME_MS 100 

#ifdef FEATURE_HDR_QTA

#define HDRSRCHRFDIV_MIN_DIV_TIME_QTA_MS 18 
  /* For QTA request for shorter duration to ensure div chain is granted back
     This is intentionally kept 18, as PRx R&N duration for QTA case is 20ms.
     It is kept so as to take in to account the delay between HDR doing
     PRx R&N and DRx R&N, which happens through Div stm.
     Note: Value for this macro is arrived after subtracting 2ms from
     HDRSRCHSUS_MIN_LOCK_SCLK (PRx R&N duration) value in DO QTA traffic case */

#define HDRSRCHRFDIV_MIN_DIV_TIME_SCLK \
       ( ( hdrsrchrfdiv.qta_start  ) ? \
       TIMETICK_SCLK_FROM_MS( HDRSRCHRFDIV_MIN_DIV_TIME_QTA_MS ) : TIMETICK_SCLK_FROM_MS( HDRSRCHRFDIV_MIN_DIV_TIME_MS ) )
         /* Minimum time we need to be able to get the RF lock on the diversity
            chain to make enabling diversity worth it. */
#else

#define HDRSRCHRFDIV_MIN_DIV_TIME_SCLK \
        TIMETICK_SCLK_FROM_MS( HDRSRCHRFDIV_MIN_DIV_TIME_MS )
  /* Minimum time we need to be able to get the RF lock on the diversity
     chain to make enabling diversity worth it. */

#endif /* FEATURE_HDR_QTA */

#define HDRSRCHRFDIV_DIV_DRC_RAMP_MS      \
        ( SLOTS_TO_MS( 32 ) + HDRSRCHRFDIV_TA_OVERHEAD_MS )
  /* prepare for DRC rampdown 32 slots before tune away */

#define HDRSRCHRFDIV_DIV_DRC_RAMP_SCLK    \
        ( TIMETICK_SCLK_FROM_MS( HDRSRCHRFDIV_DIV_DRC_RAMP_MS ) )
  /* Time to start DRC rampdown in sclks */

#define HDRSRCHRFDIV_DIV_ENABLE_DELAY_MS 0
  /* How long we should wait before declaring the diversity receiver as
     enabled. With RF support of idle diversity,DO L1 does not need to
     wait any time for diversity chain warmup. */

#define HDRSRCHRFDIV_DIV_ENABLE_DELAY_INVALID 0xFFFF

#endif /* FEATURE_HDR_HARD_DIV_TUNE_AWAY */

#define HDRSRCHRFDIV_IS_DIVERSITY_ALLOWED   \
       ( ( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.state == HDRSRCHRF_RF_TUNED ) && \
         ( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.channel_cnt > 0 ) && \
         ( hdrsrchrfdiv.ext_dual_rx == TRUE ) && \
         ( hdrsrchrfdiv_check_div_for_band \
           ( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.channel[0] ) == TRUE ) )
  /* Whether the diversity is allowed by RF primary chain status/ div NV
     and diversity band support */
#define HDRSRCHRFDIV_GET_DIV_TRM_PRIORITY             \
          ( ( hdrsrch_get_current_state() == HDRSRCH_CONNECTED_STATE )? \
           HDRSRCHRF_DIVERSITY : ( ( ( hdrsrch_get_current_state() == HDRSRCH_ACQ_STATE ) || \
                               ( hdrsrch_get_current_state() == HDRSRCH_SYNC_STATE ) )? \
           HDRSRCHRF_ACQ_DIVERSITY: ( ( hdrsrchidle_is_in_access() == TRUE ) ? \
           HDRSRCHRF_ACCESS_DIVERSITY: HDRSRCHRF_IDLE_DIVERSITY ) ) )
    /* For HDR traffic TRM_DIVERSITY (25) priority,
     for idle TRM_IDLE_DIVERSITY (15),
     for acq/sync TRM_ACQ_DIVERSITY (15),
     for access TRM_ACCESS_DIVERSITY (15) */

#define HDRSRCHRFDIV_MIN_TIMER_SCLK 33
  /* The minimum timer value supported by Q6 Blast timers. */

#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
#define HDRSRCH_DIV_GPIO_CELL  85
#define HDRSRCH_DIV_GPIO_IN_CELL  GPIO_IN_85
#define HDRSRCH_DIV_GPIO_INT_CELL GPIO_INT_85
  /* GPIO 85 for Cell */

#ifdef FEATURE_8X60_DUAL_RADIO_FUSION
#error code not present
#else
#define HDRSRCH_DIV_GPIO_PCS  86
#define HDRSRCH_DIV_GPIO_IN_PCS  GPIO_IN_86
#define HDRSRCH_DIV_GPIO_INT_PCS GPIO_INT_86
  /* GPIO 86 for PCS */
#endif /*  FEATURE_8X60_DUAL_RADIO_FUSION */

#define HDRSRCH_DIV_GPIO_LOW_VALUE     0
  /* GPIO low */
#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */

#define HDRSRCHRFDIV_REL_TO_REQ_WAIT_MS     20
  /* Time to wait between release and request DIV chain */

#define HDRSRCHRFDIV_ENABLE_BOOST_DURATION_US   (40000)
  /* The combining mode register value. */

#define HDRSRCHRF_BIMC_DEBOOST                  (0)
  /* Request to deboost BIMC clock to MCPM */

enum
{
  HDRSRCHRFDIV_COMB_AUTO_MODE_SEL_REG_VAL = 0x0000,
    /* Automatic Mode Selection */

  HDRSRCHRFDIV_COMB_MRC_REG_VAL = 0x0004,
    /* Force MRC Combining Mode */

  HDRSRCHRFDIV_COMB_MMSE_REG_VAL = 0x0002,
    /* Force MMSE Combining Mode */

  HDRSRCHRFDIV_COMB_FORCE_ANT_SEL_REG_VAL = 0x0001,
    /* Force auto antenna selection. */

  HDRSRCHRFDIV_COMB_FORCE_ANT_0_REG_VAL = 0x201,
    /* Force antenna 0 selection. */

  HDRSRCHRFDIV_COMB_FORCE_ANT_1_REG_VAL = 0x301
    /* Force antenna 1 selection. */
};

#define HDRSRCHRFDIV_IS_SMALLPACKET( drc ) ( ( drc >= 6 ) || ( drc == 4 ) )
  /* Whether packets at the given DRC are considered small or large for the
     purposes of the diversity ramp down algorithm. Anything 2 slots or
     less are considered small. */

/* The combining modes retrieved from NV. */
typedef enum
{
  HDRSRCHRFDIV_COMB_AUTO_SEL = 0,
    /* Automatic Mode Selection */

  HDRSRCHRFDIV_COMB_MRC,
    /* Force MRC Combining Mode */

  HDRSRCHRFDIV_COMB_MMSE,
    /* Force MMSE Combining Mode */

  HDRSRCHRFDIV_COMB_FORCE_SEL
    /* Force Selection Combining Mode */
}
hdrsrchrf_comb_mode_enum_type;

#define HDRSRCHRFDIV_SINGLE_ANT_DEMOD_THRESH 20
  /* Difference in AGC (dBm) between RX0 and RX1 required to switch to 
    single ant div mode. */

#define HDRSRCHRFDIV_DUAL_ANT_DEMOD_THRESH   10
  /* Difference in AGC (dBm) between RX0 and RX1 required to switch to 
    dual ant div mode. */

/* The diversity combining antenna selection retrieved from NV. */
typedef enum
{
  HDRSRCHRFDIV_COMB_ANT_SEL = 0,
    /* Force auto antenna selection. */

  HDRSRCHRFDIV_COMB_ANT_0 = 1,
    /* Force antenna 0 selection. */

  HDRSRCHRFDIV_COMB_ANT_1 = 2
    /* Force antenna 1 selection. */
}
hdrsrchrf_comb_ant_enum_type;

/* The receive power filter */
enum
{
  HDRSRCHRFDIV_FILT_COEF_N    = 125,
  HDRSRCHRFDIV_FILT_COEF_NM1  = 875,
  HDRSRCHRFDIV_FILT_PRECISION = (HDRSRCHRFDIV_FILT_COEF_N + HDRSRCHRFDIV_FILT_COEF_NM1),
  HDRSRCHRFDIV_FILT_STABLE_LEN = HDRSRCHRFDIV_FILT_PRECISION/HDRSRCHRFDIV_FILT_COEF_N
};

#define HDRSRCH_FILT_POWER( prev_pwr, curr_pwr ) \
         ( ( ( prev_pwr ) * HDRSRCHRFDIV_FILT_COEF_NM1 +   \
             ( curr_pwr ) * HDRSRCHRFDIV_FILT_COEF_N )     \
                         / HDRSRCHRFDIV_FILT_PRECISION )

/* State of the diversity tune away. */
typedef enum
{
  HDRSRCHRFDIV_DIV_TUNE_AWAY_NO_CONFLICT,
    /* Not in ramp down. */

  HDRSRCHRFDIV_DIV_TUNE_AWAY_DRC_RAMP
    /* In the middle of diversity ramp down. */

#ifdef FEATURE_HDR_HARD_DIV_TUNE_AWAY
  ,HDRSRCHRFDIV_DIV_HARD_TUNE_AWAY_DRC_RAMP
    /* In the middle of a "hard" diversity ramp down. */
#endif /* FEATURE_HDR_HARD_DIV_TUNE_AWAY */

}
hdrsrchrf_div_tune_away_enum_type;

/* The diversity mode configuration */
enum
{
  HDRSRCHRF_DIVERSITY_PRI_ANT0 = 0x8000,
    /* Diversity enabled and use ANT0 as primary */

  HDRSRCHRF_NON_DIVERSITY_PRI_ANT0 = 0x0
    /* Diveristy disabled and use ANT0 as primary */

#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
  ,HDRSRCHRF_NON_DIVERSITY_PRI_ANT1 = 0x1
    /* Diveristy disabled and use ANT1 as primary */
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */
};

/* Diversity preference message sent to the diversity state machine. */
typedef struct
{
  uint8  div_ctrl;
    /* user diversity preference */

  uint32 band_mask;
    /* per band diversity enable/disable mask  */

} hdrsrchrfdiv_div_pref_type;

/* The information retained related diversity settings and state. */
typedef struct
{
  boolean                            ext_dual_rx;
    /* external control over usage of both rx chains */

  uint8                              comb_mode;
    /* Diversity combining mode bit-field.

       Bit0   : Diversity ON/Off

       Bit2:1 : 00 : Automatic Mode
                01 : MRC
                10 : MMSE
                11 : Selection

       Bit4:3 : 00 : Automatic Antenna selection
                01 : Ant0
                10 : Ant1
       Bit 5 :  1  : drx only/ second chain only
       Bit 6 :  0  : Disable diversity in access state
                1  : Enable diversity in access state
       Bit 7 :  0  : Disable diversity in idle state
                1  : Enable diversity in idle state
     
    */

  timer_type                         warmup_timer;
    /* Timer expires when the RF is done warming up. */

  timer_type                         clr_null_cover_timer;
    /* Timer expires when we should clear the null cover. */

  timer_type                         release_div_timer;
    /* Timer expires when we should consider releasing the div chain. */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
  timer_type                         div_enable_timer;
    /* Timer expires when the time for div enable in reacq expires. */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */ 

  timer_type                         wait_to_acq_timer;
    /* Timer expires when we should consider releasing the div chain. */

  hdrsrchrf_div_tune_away_enum_type  tune_away_state;
    /* State of the diversity tune away. */

  boolean                            no_tune_away;
    /* Do we know when we are supposed to tune away yet? */

  timetick_type                      end_time_sclks;
    /* Sclk timestamp for when we should tune away. */

  boolean                            demod_enabled;
    /* Whether we are enabling div demod. */

  boolean                            enable_div_ramp;
    /* Whether DRC ramp down for diversity tune aways are permitted. */

  uint32                             band_mask;
    /* per band diversity enable/disable mask  */

  boolean                            requested_by_app;
    /* Whether diversity is requested by app or not */

  boolean                            requested_by_srch;
    /* Whether diversity is requested by search stm */

  boolean                            agc_delta_small;
    /* Whether rxAGC0 and rxAGC1 delta is small */

#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
  uint32                             div_int;
    /* Div status change interrupt */

  uint32                             gpio_status;
    /* GPIO status for diversity chain */

#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */

#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
  boolean                            second_chain_only;
    /* Second chain test call only */
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
  boolean                                rel_pend;
    /* Div release pending. needed to respect rampdown */
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

#ifdef FEATURE_HDR_QTA
  boolean                                qta_start;
   /* Has qta started? */
#endif /* FEATURE_HDR_QTA */

}
hdrsrchrfdiv_struct_type;

hdrsrchrfdiv_struct_type hdrsrchrfdiv;
 /* HDR Searcher RF Diversity parameters consolidated in one structure */

#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT

DalDeviceHandle *hdrsrchrfdiv_hTlmm; 
  /* Global handle that is used to call all tlmm apis*/

uint32 hdr_cell_gpio_config, hdr_pcs_gpio_config; 
  /* GPIO config in TLMM */

#endif

#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
 
#define HDRSRCHRFDIV_SECOND_CHAIN_ONLY_NV_VALUE 0x21
  /* NV value to switch to second chain only mode */

#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */

/* Tune away constants */

#ifdef FEATURE_HDR_SUPPORTS_FW_TIMING_STATE
#define HDRSRCHRFDIV_OVERHEAD_MS              18
#else
#define HDRSRCHRFDIV_OVERHEAD_MS              13
#endif /* FEATURE_HDR_SUPPORTS_FW_TIMING_STATE */
  /* Tune away overhead */

#define HDRSRCHRFDIV_OVERHEAD_SCLK \
                              (TIMETICK_SCLK_FROM_MS(HDRSRCHRFDIV_OVERHEAD_MS))

#define HDRSRCHRFDIV_ACQ_OVERHEAD_SCLK \
                             (TIMETICK_SCLK_FROM_MS(HDRSRCHRFDIV_OVERHEAD_MS+5))
  /* For Acq, allow Additional 5ms buffer for any active search to be completed */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
/*=========================================================================

FUNCTION HDRSRCHRFDIV_ENABLE_DIV_TIMER_CB

DESCRIPTION 
  Called when we should process diversity enable timeout event.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchrfdiv_enable_div_timer_cb
(
  timer_cb_data_type              data
    /* Arbitrary data given to timer_def( ) to pass to this callback function */
)
{

   hdrsrchsm_cmd_payload_type cmd_payload;
    /* Command + payload for internal stm command */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmd_payload.cmd              = HDRSRCH_DIV_ENABLE_TIMEOUT_CMD;
  cmd_payload.payload.not_used = HDRSRCHSM_PAYLOAD_NOT_USED;
      /* No HDR srch payload other than command */

  HDRSRCH_STM_ALLOC_PUT_INTERNAL_CMD( &HDRSRCH_SM,
                                      &hdrsrch_stm_group, 
                                      HDRSRCH_DIV_ENABLE_TIMEOUT_CMD,
                                      hdrsrchsm_cmd_payload_type,
                                      cmd_payload );
    /* Send the command to the SRCH Diversity State Machine */

} /* hdrsrchrfdiv_release_div_timer_cb */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */ 

/*=========================================================================

FUNCTION HDRSRCHRFDIV_START_STM

DESCRIPTION 
  Start div stm.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchrfdiv_start_stm( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  stm_activate( &HDRSRCHRFDIV_SM, &hdrsrchcom_stm_group, TRUE );
} /* hdrsrchrfdiv_start_stm */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_WARMUP_TIMER_CB

DESCRIPTION 
  Called when the RF is completed warming up.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchrfdiv_warmup_timer_cb
(
  timer_cb_data_type              data
    /* Arbitrary data given to timer_def( ) to pass to this callback function */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                &hdrsrchcom_stm_group, 
                                HDRSRCHRFDIV_WARMUP_DONE_CMD ); 
    /* Send the command to the RF Diversity State Machine */

} /* hdrsrchrfdiv_warmup_timer_cb */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_SET_DIV_RAMP_DOWN_MODE

DESCRIPTION
  This function sets the DRC ramp down type for diversity tune aways.

DEPENDENCIES
  hdrsrchrfdiv_enable_div_ramp_down and hdrsrchrfdiv_disable_div_ramp_down
  must be appropriately called so that diversity tune aways don't
  disrupt traffic DRC ramp downs. 

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void hdrsrchrfdiv_set_div_ramp_down_mode
( 
  uint16                     ramp_down_mode
    /* Ramp down mode selected */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchrfdiv.enable_div_ramp )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Setting Div ramp down mode %d", 
                                    ramp_down_mode );

    hdrsrchmsg_set_rampdown_mode( ramp_down_mode );
      /* Set DRC ramp down mode and send the message */
  }

} /* hdrsrchrfdiv_set_div_ramp_down_mode */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_CLR_NULL_COVER_TIMER_CB

DESCRIPTION 
  Called when it is time for the null cover to be cleared.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchrfdiv_clr_null_cover_timer_cb
(
  timer_cb_data_type              data
    /* Arbitrary data given to timer_def( ) to pass to this callback function */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  NOTUSED( data );

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Cleared NULL DRC Cover, rel:%d",
                  hdrsrchrfdiv.rel_pend );

  hdrsrchrfdiv_set_div_ramp_down_mode( HDRDRC_TUNE_AWAY_DIV );
    /* Set tune away only on diversity path in the firmware. */

  if ( hdrsrchrfdiv.rel_pend )
  {
    /* If div release pending after rampdown, disable DIV. This
       is needed to move DIV SM to inactive state. */
    HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM,
                                  &hdrsrchcom_stm_group,
                                  HDRSRCHRFDIV_DISABLE_DIV_CMD );
      /* Send the command to the RF Diversity State Machine */
  }
  else
  {
    /* if no div release pending, request DIV again */    
    HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                  &hdrsrchcom_stm_group, 
                                  HDRSRCHRFDIV_HARD_TA_REQUEST_DIV_CMD );
  }

} /* hdrsrchrfdiv_clr_null_cover_timer_cb */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_RELEASE_DIV_TIMER_CB

DESCRIPTION 
  Called when we should process release diversity event.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchrfdiv_release_div_timer_cb
(
  timer_cb_data_type              data
    /* Arbitrary data given to timer_def( ) to pass to this callback function */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  

  HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                &hdrsrchcom_stm_group, 
                                HDRSRCHRFDIV_RELEASE_DIV_CMD ); 
    /* Send the command to the RF Diversity State Machine */

} /* hdrsrchrfdiv_release_div_timer_cb */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_RELEASE_DIV

DESCRIPTION 
  Send the command to diversity stm to release diversity.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchrfdiv_release_div( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                &hdrsrchcom_stm_group, 
                                HDRSRCHRFDIV_RELEASE_DIV_CMD ); 

} /* hdrsrchrfdiv_release_div */


#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
/*=========================================================================

FUNCTION HDRSRCHRFDIV_READ_GPIO_STATUS

DESCRIPTION 
  This function will return the gpio value of the pin for the current band.

DEPENDENCIES
  None

RETURN VALUE
  GPIO value of the pin for the current band.

SIDE EFFECTS
  None

=========================================================================*/
uint32 hdrsrchrfdiv_read_gpio_status( void )
{
  rf_card_band_type    current_band;
    /* The current band */

  DALGpioValueType     gpio_value_cell;
    /* GPIO pin value for cell band */

  DALGpioValueType     gpio_value_pcs;
    /* GPIO pin value for pcs band */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Read GPIO pins values */
  DalTlmm_GpioIn( hdrsrchrfdiv_hTlmm, hdr_cell_gpio_config, &gpio_value_cell );
  DalTlmm_GpioIn( hdrsrchrfdiv_hTlmm, hdr_pcs_gpio_config, &gpio_value_pcs );
  
  HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                  "DalTlmm_GpioIn: gpio_cell %x, gpio_pcs %x", 
                  gpio_value_cell, gpio_value_pcs );

  current_band = ( rf_card_band_type )( int ) 
       hdrsrchrf.chain[ HDRSRCHRF_PRI_CHAIN ].tune.channel[0].band;
    /* Get the current band */

  switch ( current_band )
  {
    case RF_BC0_BAND :

      hdrsrchrfdiv.div_int = HDRSRCH_DIV_GPIO_INT_CELL;
        /* Save GPIO int line */
      
      return( (uint32) gpio_value_cell );

    case RF_BC1_BAND :

      hdrsrchrfdiv.div_int = HDRSRCH_DIV_GPIO_INT_PCS;
        /* Save GPIO int line */

      return( (uint32) gpio_value_pcs );

    default:
      ERR_FATAL( "Unexpected band %d", current_band, 0, 0 );

      return 0;
  } /* switch */

} /* hdrsrchrfdiv_read_gpio_status */

/*=========================================================================

FUNCTION HDRSRCHRFDIV_CHANGE_DIV_STATUS_ISR

DESCRIPTION 
  Sends the command to diversity stm to inidcate div status change.
  The ISR in Q6 is through DalRPC call from ARM9.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchrfdiv_change_div_status_isr
(
  uint32           param
    /* ISR param */
)
{
  uint32       gpio_value;
    /* GPIO input value */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled )
  {

    gpio_value = hdrsrchrfdiv_read_gpio_status();
      /* Read the gpoi value */

    hdrsrchrfdiv.gpio_status = gpio_value;
      /* Save the gpio status */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                    "Based on the GPIO pin input flip the trigger %d", 
                    hdrsrchrfdiv.div_int );

    /* Register GPIO based ISR with edge trigger */
    if ( gpio_value == HDRSRCH_DIV_GPIO_LOW_VALUE )
    {
      tramp_gpio_set_trigger( hdrsrchrfdiv.div_int, TRAMP_TRIGGER_RISING );
        /* Set trigger instead of re-register ISR to avoid embedded RPC call */
    }
    else
    {
      tramp_gpio_set_trigger( hdrsrchrfdiv.div_int, TRAMP_TRIGGER_FALLING );
        /* Set trigger instead of re-register ISR to avoid embedded RPC call */
    }

    HDRSRCH_STM_ALLOC_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                        &hdrsrchcom_stm_group, 
                                        HDRSRCHRFDIV_CHANGE_DIV_STATUS_CMD,
                                        uint32,
                                        gpio_value );
      /* Post diversity status change cmd to div STM */
  } /* if ( hdrsrchrf.chain ... */

} /* hdrsrchrfdiv_change_div_status_isr */

/*=========================================================================

FUNCTION HDRSRCHRFDIV_REGISTER_GPIO_ISR

DESCRIPTION 
  This function will register GPIO based ISR through tramp gpio interface.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchrfdiv_register_gpio_isr
( 
  uint32            gpio_value
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                  "Register GPIO interrupt for div %d", 
                  hdrsrchrfdiv.div_int );

  /* Register GPIO based ISR with edge trigger */
  if ( gpio_value == HDRSRCH_DIV_GPIO_LOW_VALUE )
  {
    tramp_gpio_register_isr( hdrsrchrfdiv.div_int,
                             TRAMP_TRIGGER_RISING,
                             hdrsrchrfdiv_change_div_status_isr,
                             hdrsrchrfdiv.div_int );
  }
  else
  {
    tramp_gpio_register_isr( hdrsrchrfdiv.div_int,
                             TRAMP_TRIGGER_FALLING,
                             hdrsrchrfdiv_change_div_status_isr,
                             hdrsrchrfdiv.div_int );
  }
} /* hdrsrchrfdiv_register_gpio_isr */
#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_INIT

DESCRIPTION
  Initializes receive diversity by calling the appropriate searcher
  and finger drivers.

DEPENDENCIES
  None

PARAMETERS
  TRUE : Enable Div
  FALSE : Disable Div

RETURN VALUE
  None

SIDE EFFECTS
  Changes the hdrsrchdrv.params and hdrsrchfing.params structurs

===========================================================================*/

void hdrsrchrfdiv_diversity_init
(
  boolean div_flag
    /* enable or disable diversity */
)
{
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
  uint16 prev_div_cfg = hdrsrchrf.fw_msg.div_cfg_msg.diversityConfig;
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Set RX Diversity Mode: %d", div_flag );


  if ( div_flag == TRUE ) 
  {
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
    if ( hdrsrchrfdiv.second_chain_only == TRUE )
    {
      hdrsrchrf.fw_msg.div_cfg_msg.diversityConfig = HDRSRCHRF_NON_DIVERSITY_PRI_ANT1;       
        /* Disable diversity and use antenna 1. Only do so when DIV is enabled.
           If DIV is disabled, continue to use NON_DIV_PRI_ANT0, this is per FW
           request: FW cannot be in ANT1 when DIV is not enabled on DIME. Since 
           imbalance feature is turned off this should be OK */
    }
    else
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */
    {
      hdrsrchrf.fw_msg.div_cfg_msg.diversityConfig = HDRSRCHRF_DIVERSITY_PRI_ANT0;
        /* Enable diversity and use atnenna o as primary */
    }
  }
  else
  {
    hdrsrchrf.fw_msg.div_cfg_msg.diversityConfig = HDRSRCHRF_NON_DIVERSITY_PRI_ANT0;
      /* Disable diversity and use antenna 0 whenever DIV is turned off */
  }

  hdrsrchmsg_send_msg( &hdrsrchrf.fw_msg.div_cfg_msg, 
                       sizeof( hdrfw_diversity_cfg_msg_t ) );
    /* Send the diversity config message */

#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "2nd chain:%d div_cfg:%x=>%x", 
                  hdrsrchrfdiv.second_chain_only,
                  prev_div_cfg,
                  hdrsrchrf.fw_msg.div_cfg_msg.diversityConfig );

  if ( ( div_flag != TRUE ) &&
       ( prev_div_cfg == HDRSRCHRF_NON_DIVERSITY_PRI_ANT1 ) )
  {
    /* DIV_CFG msg is processed on HS boundary, while RX_STOP is processed
       imediately. Need to add 1HS delay to insure DIV_CFG is processed
       before RX_STOP. This lengthens Traffic->IDLE transition time, but
       is limited only to 2nd chain test call */

    hdrsrch_timed_wait_usecs( 834 ); /* 1HS = 0.833ms */
  }
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */

  hdrsrchrfdiv.demod_enabled = div_flag;
    /* Update demod enabled or not */

  hdrsrchdrv_diversity_init( div_flag );
    /* searcher settings for diversity */

  hdrsrchfing_diversity_init( div_flag );
    /* finger settings for diversity */

  hdrsrchafc_fll_rot_update_diversity( div_flag );
    /* Update diversity settings for FLL rotators. */

} /* hdrsrchrfdiv_diversity_init ( ) */


#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
/*===========================================================================

FUNCTION HDRSRCHRFDIV_DEREGISTER_CHANGE_DIV_STATUS_ISR

DESCRIPTION
  This fucntion deregisters the interrupt service routine for GPIO interrupt
  which indicates the status change of diversity in SVDO.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_deregister_change_div_status_isr( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                  "Deregister GPIO interrupt for div %d", 
                  hdrsrchrfdiv.div_int );

  tramp_gpio_deregister_isr( hdrsrchrfdiv.div_int, 
                             hdrsrchrfdiv_change_div_status_isr );

} /* hdrsrchrfdiv_deregister_change_div_status_isr */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_INIT_BY_GPIO

DESCRIPTION
  Configures receive diversity by calling the appropriate searcher
  and finger drivers based on GPIo input value based on GPIO input value.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_diversity_init_by_gpio( void )
{

  uint32            gpio_value;
    /* gpio value */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled )
  {

    gpio_value = hdrsrchrfdiv_read_gpio_status();
      /* Read the gpio value */

    hdrsrchrfdiv.gpio_status = gpio_value;
      /* Save the gpio status */

    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Diversity init by gpio %d", gpio_value);
    
    if ( ( gpio_value != HDRSRCH_DIV_GPIO_LOW_VALUE ) && 
         ( hdrsrchrfdiv.demod_enabled == FALSE ) )
    {
      hdrsrchrfdiv_diversity_init( TRUE );
        /* Enable diversity */
    }
    else
    if ( ( gpio_value == HDRSRCH_DIV_GPIO_LOW_VALUE ) && 
         ( hdrsrchrfdiv.demod_enabled == TRUE ) )
    {
      hdrsrchrfdiv_diversity_init( FALSE );
        /* Disable diversity */
    }

    hdrsrchrfdiv_register_gpio_isr( gpio_value );
      /* Register GPIO based ISR */

  } /* if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled ) */

} /* hdrsrchrfdiv_diversity_init_by_gpio */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_GET_GPIO_STATUS

DESCRIPTION
  Get the diversity GPIO status.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  gpio status

SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrsrchrfdiv_get_gpio_status( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return hdrsrchrfdiv.gpio_status;

} /* hdrsrchrfdiv_get_gpio_status */
#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */
  
    
/*===========================================================================

FUNCTION HDRSRCHRFDIV_RELEASE_DIVERSITY

DESCRIPTION
  Release the diversity receiver to TRM.  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void hdrsrchrfdiv_release_diversity( void )
{

  int ret_val;
    /* Return val from rf tune function */

  trm_release_chain_input_info  input_info;
    /* Input to info for TRM release */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  rfm_device_enum_type               rf_device;
    /* The RF physical chain mapped into the logical chain. */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  HDR_MSG_SRCH( MSG_LEGACY_HIGH,"HDR release diversity ");

  (void) timer_clr( &hdrsrchrfdiv.clr_null_cover_timer, T_NONE );
  (void) timer_clr( &hdrsrchrfdiv.release_div_timer, T_NONE );
  (void) timer_clr( &hdrsrchrfdiv.warmup_timer, T_NONE );
#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
  (void) timer_clr( &hdrsrchrfdiv.div_enable_timer, T_NONE );
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */ 
    /* Clear the release diversity timers if it they are active */

#ifdef FEATURE_HDR_QTA
  /* If QTA has started skip disabling diversity RF etc. */
  if ( hdrsrchrfdiv.qta_start == TRUE )
  {
    if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled )
    {

      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDR QTA: Disabling diversity for QTA!");

#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
      hdrsrchrfdiv_deregister_change_div_status_isr();
        /* Deregister the ISR to change div status */
#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */

      hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled = FALSE;
        /* Update the actual chain is_enabled state. */
    }    
  }
  else
#endif /* FEATURE_HDR_QTA */
  {
    if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled )
    {    
#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
      hdrsrchrfdiv_deregister_change_div_status_isr();
        /* Deregister the ISR to change div status */
#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */

      /* Update the state of the chain. */

      hdrsrchrfdiv_diversity_init( FALSE );
        /* Now shut off finger and searcher diversity settings. */

      /* Don't explicitly disable RX1 since it is done by TRM. */

#ifndef FEATURE_HDR_REVB 
      rfm_prepare_to_sleep( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rf_chain );
        /* This function saves the DC offsets. */

      rf_hdr_stop_agc( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rf_chain );
        /* stop the AGC for RX1. */
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_HDR_RUMI
    if ( hdrhit_rumi_cfg.y1y2_support )
    {
#endif /* FEATURE_HDR_RUMI */
      ASSERT(hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rxlm_handle_valid == TRUE);
 
      /* Turn off the diversity receiver. */
      ret_val =  rfm_hdr_disable_diversity ( 
        hdrsrchrf_get_rf_dev(HDRSRCHRF_DIV_CHAIN), 
                      /* Radio device to disconnected from diversity radio. */
        hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rxlm_handle,
                      /* The Rx Link Manager buffer to config the device.   */
        hdrsrchrf_rfm_cb, /* Called when div disable sequence is complete   */
        NULL ); /* Data handle to pass additional info to the cb function.  */

      hdrsrchrf_rfm_wait();
        /* Wait for signal from callback complete. */

      if (ret_val < RFM_CDMA_HEALTHY_STATE )
      {
        /* Error fatal */
        ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                  ret_val, 0, 0);
      }
#ifdef FEATURE_HDR_RUMI
    } /* if hdrhit_rumi_cfg.y1y2_support */
#endif /* FEATURE_HDR_RUMI */

      hdrsrchrf_disable_rx( HDRSRCHRF_DIV_CHAIN );
        /* Disable the RF Chain. */

      hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled = FALSE;
        /* Update the actual chain is_enabled state. */
    }

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  rf_device = hdrsrchrf_get_rf_device(
	 HDRSRCHRF_CHAIN_TO_TRM( HDRSRCHRF_DIV_CHAIN ) );
  hdrmultirat_process_div_change( FALSE, rf_device );
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */
  }

  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].owns_chain = FALSE;
    /* Update the fact this chain is no longer owned by HDR. */

  HDR_MSG_SRCH( MSG_LEGACY_LOW,"Deleting RF_GRANTED command from internal queue");
  
  HDRSRCH_STM_DEL_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                &hdrsrchcom_stm_group, 
                                HDRSRCHRFDIV_RF_GRANTED_CMD);
    /*Deleting RF granted command from the internal queue while releasing Diversity*/

  input_info.client = HDRSRCHRF_DIV_TRM_CLIENT;
  input_info.tag = 0;

  trm_release_chain( &input_info ); 
    /* Release the lock on the diversity receiver. */

  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rl_state.event = HDRSRCHRF_UNLOCK_CANCELLED;
    /* Start off assuming we have the lock indefinitely. */

  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rl_state.unlock_by_sclk = 0;
    /* Start off assuming we have the lock indefinitely. */

#ifdef FEATURE_HDR_RUMI
  hdrhit_rumi_cfg.div_enable_stage = 0;
#endif /* FEATURE_HDR_RUMI */
} /* hdrsrchrfdiv_release_diversity */


/*===========================================================================

FUNCTION      HDRSRCHRFDIV_STOP_DIV_DRC_RAMP_DOWN

DESCRIPTION
  This function stops DRC ramp down for diversity tune aways and enters 
  normal DRC mode. It does not disrupt DRC tune aways invoked by the HDR
  state machine.

DEPENDENCIES
  hdrsrchrfdiv_enable_div_ramp_down and hdrsrchrfdiv_disable_div_ramp_down
  must be appropriately called so that diversity tune aways don't
  disrupt traffic DRC ramp downs. 

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void hdrsrchrfdiv_stop_div_drc_ramp_down( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( ( hdrsrchrfdiv.enable_div_ramp ) && ( HDRDRC_GET_DRC_RAMP_DOWN() ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "Stopping Div DRC Ramp Down" );

    hdrsrchmsg_send_msg( &hdrsrchmsg.tuneaway_stop_msg.hdr, 
                         sizeof( hdrfw_tuneaway_stop_msg_t ) );
      /* Get back to normal mode of DRC i.e. stop DRC ramp down mode */
  }

} /* hdrsrchrfdiv_stop_div_drc_ramp_down */


/*===========================================================================

FUNCTION      HDRSRCHRFDIV_TUNE_AWAY_DRC_RAMP_DOWN

DESCRIPTION
  Schedule a DRC ramp down to account for the difference in SINR that will
  be the result of tuning away diversity.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_div_tune_away_drc_ramp_down
(
  uint32                     ramp_down_slots,
    /* how many slots (full slots) before tune away ? */

  uint16                     tune_away_mode
    /* The type of tune away this is */
)
{
  uint16         curr_hstr, ta_hstr;
    /* current and tune away HSTR count */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*----------------------------*
   * Start DRC Rampdown         *
   *----------------------------*/

  if ( hdrsrchrfdiv.enable_div_ramp )
  {
    curr_hstr = HDRHSTR_GET_COUNT();
      /* read the current HSTR count */
  
    ta_hstr = (curr_hstr + ramp_down_slots * 2) & 0xffff;
      /* move forward by the ramp down number of slots */
  
    hdrsrchmsg_start_drc_rampdown( tune_away_mode,
                                   ta_hstr );
      /* Start DRC ramp down process */
  
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Start Div DRC rampdown.hstr=%d tune=%d",
                    curr_hstr, ta_hstr );
  }

}/* hdrsrchrfdiv_div_tune_away_drc_ramp_down */


/*===========================================================================

FUNCTION      HDRSRCHRFDIV_CALC_TIME_TO_PRI_TUNE_AWAY

DESCRIPTION
  Calculate the time until the primary chain should be tuned away.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrsrchrfdiv_calc_time_to_pri_tune_away
(
  const hdrsrchrf_retain_lock_state_t        *pri_rl_state,
    /* State of primary retain lock. */

  uint32                                sclks_now,
    /* The current time. */

  boolean                               in_traffic
    /* whether HDR is in traffic state */
)
{

  int32                                 pri_sclks_left = HDRSRCHRF_MAX_DURATION;
    /* Sclks remaining until primary chain lock release */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( pri_rl_state->event == HDRSRCHRF_UNLOCK_CANCELLED )
  {
    pri_sclks_left = HDRSRCHRF_MAX_DURATION;
      /* There is no longer an end time so just default the requested 
         duration to the maximum duration so that the below calculations
         will work out. */
  }
  else if ( pri_rl_state->event == HDRSRCHRF_UNLOCK_BY )
  {
    if ( in_traffic)
    {    
      pri_sclks_left = (int32)( hdrsrchtc_get_primary_ta_time() - sclks_now );
        /* Calculate the newly requested number of sclks until the next tune 
           away.
           In case of unlock_by time is very close and not enough for DRC ramp
           down(less than 150 ms), primary chain tune away would happen after
           unlock_by time. hdrsrchtc_get_primary_ta_time() would return the correct
           primary tune away time.
        */
    }
    else
    {
      pri_sclks_left = (int32)( pri_rl_state->unlock_by_sclk - sclks_now );
        /* Calculate the newly requested number of sclks until the next tune 
           away. */

    }


    pri_sclks_left = ( pri_sclks_left < 0 ? 0 : pri_sclks_left );
      /* Make sure this isn't negative. */
  }
  else if ( ( pri_rl_state->event == HDRSRCHRF_UNLOCK_IMMEDIATELY ) ||
            ( pri_rl_state->event == HDRSRCHRF_UNLOCK_REQUIRED ) || 
            ( pri_rl_state->event == TRM_UNLOCK_BAND_INCOMPATIBLE ) )
  {
    pri_sclks_left = 0;
      /* The request has us releasing the lock immediately. */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Invalid div unlock request" );
  }

  return (uint32) pri_sclks_left;

} /* hdrsrchrfdiv_calc_time_to_pri_tune_away */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_ENABLE_DIV_CHAIN

DESCRIPTION
  This function enables the diversity RF chain.
  
DEPENDENCIES
  The primary receiver must already be enabled and tuned before this is
  called.  Also the diversity chain must be owned by HDR.  If diversity
  is already enabled this function is a No-op.

RETURN VALUE
  Delay in milliseconds before the chain is enabled. 

SIDE EFFECTS
  None

===========================================================================*/

static uint32 hdrsrchrfdiv_enable_div_chain( void )
{
  int ret_val;
  /* return val from rfm api */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_assign_sample_servers( HDRSRCHRF_DIV_CHAIN );

#ifndef FEATURE_HDR_RUMI
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
  /* Since in second case test call only we go from state
     div on --> to second chain only,
     device_1 can have a valid handle already */
  if( hdrsrchrfdiv.second_chain_only == FALSE )
  {
    ASSERT( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rxlm_handle_valid == FALSE );
  }
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */
#endif /* FEATURE_HDR_RUMI */

  if ( !hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rxlm_handle_valid )
  {
#ifdef FEATURE_HDR_RUMI
    if ( hdrhit_rumi_cfg.y1y2_support && 
         ( hdrhit_rumi_cfg.div_enable_stage == 0 ) )
    {
#endif /* FEATURE_HDR_RUMI */
      if (rxlm_allocate_buffer( 
                         hdrsrchrf_get_rxlm_chain(HDRSRCHRF_DIV_CHAIN),
                         LM_HDR,
                         &hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rxlm_handle)
                       == LM_SUCCESS)
      {
          hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rxlm_handle_valid = TRUE;
          /* Mark handle as valid */
      }
      else
      {
        ERR ("Unable to allocate RxLM buffer (%d)", 
              hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rf_chain, 0, 0 );
          /* Just err for now... */
      }
#ifdef FEATURE_HDR_RUMI
    } /* if hdrhit_rumi_cfg.y1y2_support */
#endif /* FEATURE_HDR_RUMI */
    hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rxlm_handle_valid = TRUE;
  }

#ifdef FEATURE_HDR_RUMI
  if ( hdrhit_rumi_cfg.y1y2_support )
  {
    if ( ( hdrhit_rumi_cfg.div_on_hack != 0 ) ||
         ( hdrhit_rumi_cfg.div_enable_stage == 0 ) )
    {
#endif /* FEATURE_HDR_RUMI */
      ret_val = rfm_enter_mode( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rf_chain,
                             (rfcom_mode_enum_type)RFM_1XEVDO_MODE,
                             NULL,
                             hdrsrchrf_rfm_cb 
#ifdef FEATURE_RF_COMMON_LM_RFM_INTERFACE 
                            , hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rf_chain
#endif
                           );

      hdrsrchrf_rfm_wait();
        /* Wait for rf task to complete. */

      if (ret_val < RFM_CDMA_HEALTHY_STATE )
      {
        /* Error fatal */
        ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                    ret_val, 0, 0);
      }
#ifdef FEATURE_HDR_RUMI
    }

    if ( ( hdrhit_rumi_cfg.div_on_hack != 0 ) ||
         ( hdrhit_rumi_cfg.div_enable_stage == 0 ) )
    {
#endif /* FEATURE_HDR_RUMI */

#ifdef FEATURE_HDR_BIMC_BOOST_DIV_ON
      if ( ( hdrsrch_get_current_state() == HDRSRCH_CONNECTED_STATE ) 
        || ( hdrsrch_get_current_state() == HDRSRCH_IDLE_STATE ) )
      {
          hdrsrchmcpm_cmgr_trans_req(CMGR_HDR_RXD_ENABLE , TRUE);
          /* DDR boost for HDR traffic, access and idle states only */
          hdrsrchmcpm_service_boost( HDR_RXD_ENABLE, 
            HDRSRCHMCPM_SCENARIO_START, 
            MCPM_BIMC_BOOST_MASK, HDRSRCHRFDIV_ENABLE_BOOST_DURATION_US);
            /* Seen instances of crash in FW due to memory access latency 
            while enabling diversity hence clk boost */

          HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Boost BIMC clk for enable_diversity" );

/* for testing only */
        HDR_MSG_PROT_4( MSG_LEGACY_HIGH, "mcpm_service_boost: Boosting clock for Scenario:%d, Scenario trigger:%d, BIMC boost:%d, Duration:%d",
          HDR_RXD_ENABLE, HDRSRCHMCPM_SCENARIO_START, MCPM_BIMC_BOOST_MASK, HDRSRCHRFDIV_ENABLE_BOOST_DURATION_US);
/* for testing only */
      }
#endif /* FEATURE_HDR_BIMC_BOOST_DIV_ON */

      ret_val =  rfm_hdr_enable_diversity ( 
        hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rf_chain,
                      /* Master radio device, device-1                      */
        hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rxlm_handle,
                      /* The Rx Link Manager buffer to config the device-1. */
        hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rf_chain,
                      /* Slave radio device, device-2                       */
        hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rxlm_handle,
                      /* The Rx Link Manager buffer to config the device-2. */
        hdrsrchrf_rfm_cb, /* Called when div enable sequence is complete    */
        NULL ); /* Data handle to pass additional info to the cb function.  */

      hdrsrchrf_rfm_wait();
        /* Wait for rf task to complete. */

#ifdef FEATURE_HDR_BIMC_BOOST_DIV_ON
      if ( ( hdrsrch_get_current_state() == HDRSRCH_CONNECTED_STATE ) 
        || ( hdrsrch_get_current_state() == HDRSRCH_IDLE_STATE ) )
      {
          hdrsrchmcpm_cmgr_trans_req(CMGR_HDR_RXD_ENABLE , FALSE);
          hdrsrchmcpm_service_boost( HDR_RXD_ENABLE, HDRSRCHMCPM_SCENARIO_STOP, HDRSRCHRF_BIMC_DEBOOST, 0);

          HDR_MSG_SRCH( MSG_LEGACY_HIGH, "De-Boost BIMC clk" );

        /* for testing only */
          HDR_MSG_PROT_4( MSG_LEGACY_HIGH, "mcpm_service_boost: Boosting clock for Scenario:%d, Scenario trigger:%d, BIMC boost:%d, Duration:%d",
            HDR_RXD_ENABLE, HDRSRCHMCPM_SCENARIO_STOP, HDRSRCHRF_BIMC_DEBOOST, 0);
        /* for testing only */
      }
#endif /* FEATURE_HDR_BIMC_BOOST_DIV_ON */
      if (ret_val < RFM_CDMA_HEALTHY_STATE )
      {
        /* Error fatal */
        ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                  ret_val, 0, 0);
      }
#ifdef FEATURE_HDR_RUMI
    }
  } /* if hdrhit_rumi_cfg.y1y2_support */
#endif /* FEATURE_HDR_RUMI */

#ifdef FEATURE_HDR_BCMCS
/* Revisit : PB is not supported in the MDSP for Rev B
  HDRMDSP_ENABLE_PB_DIVERSITY(); */
    /* Set PB Antenna Mode to diversity */
#endif /* FEATURE_HDR_BCMCS */

  return HDRSRCHRFDIV_DIV_ENABLE_DELAY_MS;
    /* 1 second. */

} /* hdrsrchrf_enable_div_chain */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_SET_COMB_MODE

DESCRIPTION
  Sets diversity combining mode preference selected by the user.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_set_diversity_comb_mode ( void )
{
  hdrsrchrf_comb_mode_enum_type         comb_mode;
    /* combining mode choice */

  hdrsrchrf_comb_ant_enum_type          select_ant;
    /* antenna selection for "selection combining" mode */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "set_diversity_comb_mode" );

#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
  /* 2nd chain test call */
  if( hdrsrchrfdiv.comb_mode == HDRSRCHRFDIV_SECOND_CHAIN_ONLY_NV_VALUE )
  {
    hdrsrchrfdiv.second_chain_only = TRUE;
  }
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */
  
  /* Diversity has been requested to be on. */
  if ( hdrsrchrfdiv.comb_mode & 0x01 )
  {
    /*--------------*
     * Diversity ON *
     *--------------*/

    comb_mode = HDRSRCHRFDIV_GET_COMBINING_MODE( hdrsrchrfdiv.comb_mode );
      /* get combining mode */

    switch ( comb_mode )
    {

      /* DSP mode selection register is :

         Bit2 to Bit0 : 0 = Auto
                        1 = Selection
                        2 = MMSE
                        4 = MRC
         Bit9 to Bit8 : 0 = Ant with best RSSI
                        2 = Select Ant0 always
                        3 = Select Ant1 always */
      case HDRSRCHRFDIV_COMB_AUTO_SEL :
      default:
      {
        hdrsrchrf.fw_msg.force_comb_mode_msg.forceCombineMode = 
                                         HDRSRCHRFDIV_COMB_AUTO_MODE_SEL_REG_VAL;
          /* Automatic Mode Selection */
        break;
      }

      case HDRSRCHRFDIV_COMB_MRC :
      {
        hdrsrchrf.fw_msg.force_comb_mode_msg.forceCombineMode = 
                                         HDRSRCHRFDIV_COMB_MRC_REG_VAL;
          /* Force MRC Combining Mode */
        break;
      }

      case HDRSRCHRFDIV_COMB_MMSE :
      {
        hdrsrchrf.fw_msg.force_comb_mode_msg.forceCombineMode = 
                                         HDRSRCHRFDIV_COMB_MMSE_REG_VAL;          
          /* Force MMSE Combining Mode */
        break;
      }

      case HDRSRCHRFDIV_COMB_FORCE_SEL :
      {
        /* Force Selection Combining Mode */

        select_ant = HDRSRCHRFDIV_GET_ANT_SEL( hdrsrchrfdiv.comb_mode );
          /* get antenna selection at bit4:3 */

        if ( select_ant == HDRSRCHRFDIV_COMB_ANT_SEL )
        {
          hdrsrchrf.fw_msg.force_comb_mode_msg.forceCombineMode = 
                                         HDRSRCHRFDIV_COMB_FORCE_ANT_SEL_REG_VAL;          /* Force MMSE Combining Mode */
            /* Automatic Antenna Selection */
        }
        else if ( select_ant == HDRSRCHRFDIV_COMB_ANT_0 )
        {
          hdrsrchrf.fw_msg.force_comb_mode_msg.forceCombineMode = 
                                         HDRSRCHRFDIV_COMB_FORCE_ANT_0_REG_VAL;          /* Force MMSE Combining Mode */
            /* Antenna0 Selection */
        }
        else if ( select_ant == HDRSRCHRFDIV_COMB_ANT_1 )
        {
          hdrsrchrf.fw_msg.force_comb_mode_msg.forceCombineMode = 
                                         HDRSRCHRFDIV_COMB_FORCE_ANT_1_REG_VAL;          /* Force MMSE Combining Mode */
            /* Antenna1 Selection */
        }
        else
        {
          HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "Invalid Comb Mode 0x%x",
                          hdrsrchrfdiv.comb_mode );

          hdrsrchrf.fw_msg.force_comb_mode_msg.forceCombineMode = 
                                         HDRSRCHRFDIV_COMB_FORCE_ANT_SEL_REG_VAL;          /* Force MMSE Combining Mode */
            /* Automatic Antenna Selection */
        }

      }/* case Selection */

    }/* switch */

    hdrsrchmsg_send_msg( &hdrsrchrf.fw_msg.force_comb_mode_msg, 
                         sizeof( hdrfw_force_comb_mode_msg_t ) );
      /* send the diveristy combining mode message */

  }/* if div is ON */

}/* hdrsrchrfdiv_set_diversity_comb_mode */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_FILTER_RX_POWER

DESCRIPTION
  This function filters receive power.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_filter_rx_power
(
  int32                      rx0_agc,
    /* RX0 AGC */

  int32                      rx1_agc
     /* RX1 AGC */
)
{
  if ( hdrsrchrf.rx_power.length < HDRSRCHRFDIV_FILT_STABLE_LEN )
  {

    /* if the filter has not stablized yet then do not use the 8 tap
       filter. Just keep incrementing the number of taps */

    hdrsrchrf.rx_power.rx0 *= hdrsrchrf.rx_power.length;
    hdrsrchrf.rx_power.rx1 *= hdrsrchrf.rx_power.length;

    hdrsrchrf.rx_power.rx0 += rx0_agc;
    hdrsrchrf.rx_power.rx1 += rx1_agc;

    hdrsrchrf.rx_power.length++;

    hdrsrchrf.rx_power.rx0 /= hdrsrchrf.rx_power.length;
    hdrsrchrf.rx_power.rx1 /= hdrsrchrf.rx_power.length;

  }/* if length < stable length */
  else
  {

    hdrsrchrf.rx_power.rx0 = HDRSRCH_FILT_POWER( hdrsrchrf.rx_power.rx0, rx0_agc );
    hdrsrchrf.rx_power.rx1 = HDRSRCH_FILT_POWER( hdrsrchrf.rx_power.rx1, rx1_agc );

  }/* else filter is stable */

} /* hdrsrchrfdiv_filter_rx_power */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_CHECK_DIV_FOR_BAND

DESCRIPTION
  This function returns the flag whether the diversity is enabled for
  the given band class given through NV item.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchrfdiv_check_div_for_band
(
  sys_channel_type                      chan
)
{
  boolean                               div_enabled = FALSE;
    /* TRUE if diversity enabled for the band class */

  rfm_device_enum_type                  rf_dev = RFM_MAX_DEVICES;

  boolean                               band_supported;
    /* whether band is supported */

  trm_get_info_input_type               input;
    /* Input to trm_get_info */

  trm_get_info_return_type              output;
    /* Output from TRM */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      
  if( ( ( hdrsrchrfdiv.band_mask >> (int32) (chan.band) ) & 0x1 ) == 0x1 )
  {
    /* Query TRM if HDR NV mask is enabled for the band */
    input.client_id = TRM_HDR_SECONDARY;
    input.info_type = TRM_DEVICE_MAPPING_INFO;
    input.input_data.band_mapping_info.band[0] = chan.band;
    input.input_data.band_mapping_info.resource = TRM_RX_DIVERSITY;
    input.input_data.band_mapping_info.num_bands = 1;

    trm_get_info(&input, &output);

    rf_dev = output.info.band_mapping_info.device[0];
    band_supported = rfm_cdma_is_band_chan_supported(rf_dev, chan);

    if ( (rf_dev != RFM_MAX_DEVICES) && (band_supported == TRUE))
    {
      div_enabled = TRUE;
    }
      /* diversity is enabled for this band class */
  }
 
  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "HDR DIV: Band mask= 0x%08x, Band= %d, Div= %d Dev=%d",
                  hdrsrchrfdiv.band_mask, chan.band, div_enabled, rf_dev );

  return ( div_enabled );

} /* hdrsrchrfdiv_check_div_for_band() */


/*============================================================================

CALLBACK HDRSRCHRFDIV_GRANT_CALLBACK

DESCRIPTION
  The trm rf grant callback function, used by the Transceiver manager
  to inform the client that the diversity receiver has been granted to HDR.
  
DEPENDENCIES
  The callback will be called by the Transceiver Manager.  It may be called
  from a task context of another client, or from interrupt context.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrchrfdiv_grant_callback
(
  trm_client_enum_t               client,
    /* The client which is being informed of an event */

  trm_grant_return_enum_type      grant,
    /* The grant being sent to the client */

  trm_request_tag_t               tag
    /* Identfying tag for this request/grant transaction */
)
{
  hdrsrchrf_logical_chain_enum_t  chain_id;
    /* The logical chain ID associated with the callback */

/*--------------------------------------------------------------------------*/

  NOTUSED( tag );

  if (  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled  )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "TRM already granted!" );
    return;
  }

  chain_id = HDRSRCHRF_CHAIN_FROM_TRM( client );
    /* Convert the client ID to the chain ID. */

  hdrsrchrf.chain[chain_id].rf_chain = hdrsrchrf_get_rf_device( client );
    /* Save the RF device for this client. */

  hdrsrchrf.chain[chain_id].owns_chain = TRUE;
    /* The client now owns the specified chain. */

  if ( grant != TRM_GRANTED )
  {
    HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                  &hdrsrchcom_stm_group, 
                                  HDRSRCHRFDIV_DISABLE_DIV_CMD ); 

    HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                  "TRM denial not expected, release DIV" );

    
  }
  /* Make sure we get the chain we expected. */
  else switch ( chain_id )
  {
    case HDRSRCHRF_DIV_CHAIN:
    {
        HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                      &hdrsrchcom_stm_group, 
                                      HDRSRCHRFDIV_RF_GRANTED_CMD ); 
          /* Send the command to the RF Diversity State Machine */
      
      break;
    }
    case HDRSRCHRF_PRI_CHAIN:
    default:
    {
      ERR_FATAL( "TRM grant sent to wrong client", 0, 0, 0 );

      break;
    }
  }
} /* hdrsrchrfdiv_grant_callback */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_REQUEST_DIVERSITY

DESCRIPTION
  Request the diversity receiver from TRM.  When we get it then enable
  diversity.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_request_diversity( void )
{

  hdrsrchrf_trm_freq_info_type          *freq_info_ptr;
    /* Pointer to frequency info to be passed to TRM. */

   trm_request_notify_input_info         req_not_info = {0};
    /* Input Information for Request and Notify */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrfdiv.tune_away_state = HDRSRCHRFDIV_DIV_TUNE_AWAY_NO_CONFLICT;
    /* Reset tune away state. */
  
  hdrsrchrfdiv.no_tune_away = TRUE;
    /* Start off assuming there is no reason to tune away. */

  hdrsrchrfdiv.end_time_sclks = HDRSRCHRF_MAX_DURATION;
    /* Start off assuming there is no reason to tune away. */

  freq_info_ptr = hdrsrchrf_get_trm_freq_info();
    /* Obtain band/chan info needed for resource request. */
  req_not_info.client_id = HDRSRCHRF_DIV_TRM_CLIENT;
    /* The client which needs the transceiver resource */

#ifdef FEATURE_HDR_DR_DSDS
  if ( hdrmultirat_is_dr_dsds_enabled() )
  {
    req_not_info.resource = (trm_resource_enum_t) HDRSRCHRF_RX_ANY;
  }
  else
#endif /* FEATURE_HDR_DR_DSDS */
  {
    req_not_info.resource = TRM_RX_DIVERSITY;
      /* The transceiver resource which is being requested */
  }
  req_not_info.duration = ( trm_duration_t ) HDRSRCHRFDIV_MIN_DIV_TIME_SCLK;
    /* How long the resource will be needed for (in sclks) */
  req_not_info.reason = (trm_reason_enum_t) HDRSRCHRFDIV_GET_DIV_TRM_PRIORITY;
    /* Why the resource is needed (used for priority decisions) */
  req_not_info.sub_reason = hdrsrchrf_get_sub_reason(HDRSRCHRFDIV_GET_DIV_TRM_PRIORITY);
    /* sub-reason in addition to the reason being passed */
  req_not_info.freq_info.num_bands = 1;
  req_not_info.freq_info.bands[0].band = freq_info_ptr->freq_info.band;
    /* Band/s that would be used */
  req_not_info.wakeup_identifier = hdrsrchsleep_get_unique_wakeup_id();
    /* Unique Identifier to indicate the wakeup for which reservation is done */
  trm_request_and_notify( &req_not_info );
    /* Nothing to do internally, just let TRM know about the reservation */
  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].reason = HDRSRCHRF_DIVERSITY;
    /* Remember the reason the lock is being held. */

#ifdef FEATURE_HDR_RUMI
  if ( hdrhit_rumi_cfg.force_trm_grant )
  {
    hdrsrchrfdiv_grant_callback( HDRSRCHRF_DIV_TRM_CLIENT,
                                 TRM_GRANTED,
                                 0 );
  }
#endif /* FEATURE_HDR_RUMI */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "hdrsrchrfdiv_request_diversity() request div" );
} /* hdrsrchrfdiv_request_diversity */

/*===========================================================================

                     COMMAND  RELATED  FUNCTION  DEFINITIONS

===========================================================================*/

/*===========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_CTRL

DESCRIPTION
  Call this function to enable or disable diversity.  It will be enabled
  whenever the RF resources are available and the primary chain is 
  enabled.  Otherwise, it will be be off.

DEPENDENCIES
  Assumes that external user diversity preference is pre-selected

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_diversity_ctrl
(
  boolean    div_flag
    /* 1: to enable diversity, 0 : disable diversity */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrfdiv.requested_by_srch = div_flag;
    /* Set the flag of diversity requested by srch stm; It's not set inside the 
       transfer functions, otherwise INTACTIVE state will need to support DISABLE cmd */

  if ( div_flag == TRUE )
  {
    HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                  &hdrsrchcom_stm_group, 
                                  HDRSRCHRFDIV_REQUEST_DIV_CMD ); 
      /* Send the command to the RF Diversity State Machine */
  }
  else
  {
    HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                  &hdrsrchcom_stm_group, 
                                  HDRSRCHRFDIV_DISABLE_DIV_CMD ); 
      /* Send the command to the RF Diversity State Machine */

    (void) stm_process_group( &hdrsrchcom_stm_group);
      /* Execute the command */
  }

} /* hdrsrchrfdiv_diversity_ctrl() */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_RESUME_TUNEAWAY

DESCRIPTION
  Resume tuneaway on diversity chain.  If diversity chain tuneaway timer
  was cleared before, this will run diversity release chain logic one time,
  which will start tuneaway timer if necessary.  If diversity chain tuneaway
  timer is already running, exercising diversity release chain logic an extra
  time will not hurt.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_resume_tuneaway( void )
{

  if (hdrsrchrfdiv_diversity_is_enabled() )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDR Div: div chain resume tuneaway");
    HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM,
                                  &hdrsrchcom_stm_group,
                                  HDRSRCHRFDIV_RELEASE_DIV_CMD );
    /* Send the command to the RF Diversity State Machine */
  }

} /* hdrsrchrfdiv_resume_tuneaway */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_USER_DIVERSITY_PREF

DESCRIPTION
  Sets diversity preference selected by the user

  This function is called during lmac powerup init

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_user_diversity_pref
( 
  uint8     div_ctrl,
    /* external user diversity preference */

  uint32    band_mask
    /* per band diversity enable/disable mask  */
)
{
  hdrsrchrfdiv_div_pref_type div_pref;
    /* Enable Diversity Ramp Down. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  div_pref.div_ctrl = div_ctrl;
  div_pref.band_mask = band_mask;

  HDRSRCH_STM_ALLOC_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                      &hdrsrchcom_stm_group, 
                                      HDRSRCHRFDIV_SET_DIV_PREF_CMD, 
                                      hdrsrchrfdiv_div_pref_type, 
                                      div_pref ); 
    /* Send the command to the RF Diversity State Machine */

} /* hdrsrchrfdiv_user_diversity_pref */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_CONSIDER_DIVERSITY_SWITCH

DESCRIPTION
  Consider switching diversity demod off based on the AGC values
  to improve overall throughput performance.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_consider_div_switch( void )
{

  HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                      &hdrsrchcom_stm_group, 
                                HDRSRCHRFDIV_CONSIDER_DIV_SWITCH_CMD ); 
    /* Send the command to the RF Diversity State Machine */

} /* hdrsrchrfdiv_consider_div_switch */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_DISABLE_DIV_RAMP_DOWN

DESCRIPTION 
  This function disables the RF module from performing ramp down
  activities related to disabling the diversity chain. The diversity
  chain can still be released. This function is called so that
  the RF module will not interfere with the HDR state machine's ramp down
  activities.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchrfdiv_disable_div_ramp_down( void )
{

  boolean enable_ramp_down = FALSE;
    /* Enable Diversity Ramp Down. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDRSRCH_STM_ALLOC_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                      &hdrsrchcom_stm_group, 
                                      HDRSRCHRFDIV_SET_RAMP_DOWN_STATE_CMD, 
                                      boolean, 
                                      enable_ramp_down ); 
    /* Send the command to the RF Diversity State Machine */


} /* hdrsrchrfdiv_disable_div_ramp_down */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_ENABLE_DIV_RAMP_DOWN

DESCRIPTION 
  This function allows the RF module to perform ramp down
  activities related to disabling the diversity chain.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchrfdiv_enable_div_ramp_down( void )
{
  boolean enable_ramp_down = TRUE;
    /* Enable Diversity Ramp Down. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDRSRCH_STM_ALLOC_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                      &hdrsrchcom_stm_group, 
                                      HDRSRCHRFDIV_SET_RAMP_DOWN_STATE_CMD, 
                                      boolean, 
                                      enable_ramp_down ); 
    /* Send the command to the RF Diversity State Machine */

} /* hdrsrchrfdiv_enable_div_ramp_down */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_RUNTIME_DIVERSITY_PREF_CTRL

DESCRIPTION
  Runtimely sets diversity preference by sending command to diversity stm.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_runtime_diversity_pref_ctrl
(
  hdrsrch_rx_div_enum_type        div_ctrl
    /* Diversity ctrl */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchtc_is_connection_close_in_progress() == TRUE ) 
  {
    HDR_MSG_SRCH_1( 
      MSG_LEGACY_HIGH, 
      "Connection close in progress, ignore runtime pref setting:%d!",
      div_ctrl );
    return;
  }

  HDRSRCH_STM_ALLOC_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                      &hdrsrchcom_stm_group, 
                                      HDRSRCHRFDIV_RUNTIME_PREF_CTRL_CMD, 
                                      hdrsrch_rx_div_enum_type, 
                                      div_ctrl ); 
    /* Send the command to the RF Diversity State Machine */

} /* hdrsrchrfdiv_runtime_diversity_pref_ctrl */


/*===========================================================================

                     GET  RELATED  FUNCTION  DEFINITIONS

===========================================================================*/

/*=========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_COMB_ENABLED

DESCRIPTION 
  This function returns if diversity combining is
  enabled from an NV perspective. It may not actually be on at this
  point in time.

DEPENDENCIES
  None

RETURN VALUE
  True  - if diversity combining is enabled.
  False - Otherwise. 
  
SIDE EFFECTS
  None

=========================================================================*/
boolean hdrsrchrfdiv_diversity_comb_enabled( void )
{
  return (boolean)(hdrsrchrfdiv.comb_mode & 0x01);
} /* hdrsrchrfdiv_diversity_comb_enabled */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_DEMOD_IS_ENABLED

DESCRIPTION
  Returns the diversity status ON/OFF whether diversity is actually
  being used at the baseband level.

DEPENDENCIES
  None

RETURN VALUE
  Boolean : Diversity Status ON or OFF

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchrfdiv_diversity_demod_is_enabled( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled && 
           hdrsrchrfdiv.demod_enabled );

} /* hdrsrchrfdiv_diversity_demod_is_enabled() */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_IS_ENABLED

DESCRIPTION 
  This function checks if diversity chain is enabled at the moment. 

DEPENDENCIES
  None

RETURN VALUE
  TRUE - If diversity chain is enabled
  FALSE - otherwise.
  
SIDE EFFECTS
  None.

=========================================================================*/
boolean hdrsrchrfdiv_diversity_is_enabled ( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled;

} /* hdrsrchrfdiv_diversity_is_enabled */


/*===========================================================================

                  STATE  MACHINE  TRANSITION  FUNCTIONS

===========================================================================*/


/*=========================================================================

FUNCTION HDRSRCHRFDIV_REQUEST_DIV_TF

DESCRIPTION 
  This function requests the diversity chain from TRM. 

DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHRFDIV_ACQUIRING_RF_STATE

SIDE EFFECTS
  None

=========================================================================*/

static stm_state_type hdrsrchrfdiv_request_div_tf
( 
  void                       *payload
    /* Payload */
)
{
  stm_state_type             state = STM_SAME_STATE;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  NOTUSED( payload );

  /* Check to make sure diversity is allowed and also requested by app or qta is inprogress */
  
  if ( hdrsrchrfdiv.requested_by_app == FALSE 
#ifdef FEATURE_HDR_QTA
     && ( hdrsrchrfdiv.qta_start == FALSE ) 
#endif /* FEATURE_HDR_QTA */
     )
  {
    state = STM_SAME_STATE;
  }
  else if ( HDRSRCHRFDIV_IS_DIVERSITY_ALLOWED )
  {
    (void) timer_clr( &hdrsrchrfdiv.wait_to_acq_timer, T_NONE );
    state = HDRSRCHRFDIV_ACQUIRING_RF_STATE;
  }

  HDR_MSG_SRCH_2( 
    MSG_LEGACY_MED, 
    "hdrsrchrfdiv_request_div_tf(): runttime pref:%d, New Diversity state:%d", 
    hdrsrchrfdiv.requested_by_app, state);
  
#ifdef FEATURE_HDR_QTA
  HDR_MSG_SRCH_1( 
    MSG_LEGACY_MED, 
    "hdrsrchrfdiv_request_div_tf(): QTA in-progress:%d", 
     hdrsrchrfdiv.qta_start);
#endif /* FEATURE_HDR_QTA */

  return state;

} /* hdrsrchrfdiv_request_div_tf */

/*=========================================================================

FUNCTION HDRSRCHRFDIV_RF_GRANTED_TF

DESCRIPTION 
  Called when TRM has granted the RF chain to the diversity state
  machine.

DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHRFDIV_WARM_UP_STATE
  
SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchrfdiv_rf_granted_tf
( 
  void                       *payload
    /* Payload */
)
{
  stm_state_type             state = STM_SAME_STATE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  NOTUSED( payload );

  if ( hdrsrchrfdiv.requested_by_app == FALSE 
#ifdef FEATURE_HDR_QTA
    && ( hdrsrchrfdiv.qta_start == FALSE ) 
#endif /* FEATURE_HDR_QTA */
     )
  {
    state = HDRSRCHRFDIV_INACTIVE_STATE;
      /* Move to inactive state and release diversity chain, 
      after runtime pref enable diversity chain will be enabled */
  }
  else
  {
    state = HDRSRCHRFDIV_WARM_UP_STATE;
      /* Move to warmup state to process diversity rf enabled */
  }

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                  "hdrsrchrfdiv_rf_granted_tf(): runttime pref:%d, New Div STM state:%d", 
                  hdrsrchrfdiv.requested_by_app, state);

  return state;
} /* hdrsrchrfdiv_rf_granted_tf */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_DISABLE_DIV_TF

DESCRIPTION 
  Called when the HDR state machine requests that diversity is disabled.

DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHRFDIV_INACTIVE_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchrfdiv_disable_div_tf
( 
  void                       *payload
    /* Payload */
)
{
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  NOTUSED( payload );

  return HDRSRCHRFDIV_INACTIVE_STATE;

} /* hdrsrchrfdiv_disable_div_tf */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_WARM_UP_DONE_TF

DESCRIPTION 
  Called when the diversity state machine is done warming up diversity.
  Diversity is enabled.

DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHRFDIV_ENABLED_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchrfdiv_warm_up_done_tf
( 
  void                       *payload
    /* Payload */
)
{
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  NOTUSED( payload );

  return HDRSRCHRFDIV_ENABLED_STATE;

} /* hdrsrchrfdiv_warm_up_done_tf */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_SET_DIV_PREF_TF

DESCRIPTION
  Sets diversity preference selected by the user.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static stm_state_type hdrsrchrfdiv_set_div_pref_tf
( 
  void                       *payload
    /* Payload */
)
{
  hdrsrchrfdiv_div_pref_type *div_pref = 
                                     ((hdrsrchrfdiv_div_pref_type *) payload);
    /* The diversity preferences. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchrfdiv.comb_mode = div_pref->div_ctrl;
    /* copy diversity mode to be used later */

  hdrsrchrfdiv.ext_dual_rx = (div_pref->div_ctrl & 0x01 ? TRUE : FALSE);
      /* check if diversity is being set ON or OFF */

  hdrsrchrfdiv.band_mask = div_pref->band_mask;
    /* set the per band diversity enable/disable mask */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                  "HDR DIV: Div on=%d, Band mask=0x%08x",
                  hdrsrchrfdiv.ext_dual_rx, 
                  hdrsrchrfdiv.band_mask );

  return STM_SAME_STATE;

} /* hdrsrchrfdiv_set_div_pref_tf */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_SET_RAMP_DOWN_STATE_TF

DESCRIPTION 
  This function tells the RF module whether to perform ramp down
  activities related to disabling the diversity chain.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchrfdiv_set_ramp_down_state_tf
( 
  void                       *payload
    /* Payload */
)
{

  boolean enable_div_ramp = *((boolean *) payload);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchrfdiv.enable_div_ramp = enable_div_ramp;
    /* Save the DRC ramp down state for diversity tune aways. */

  return STM_SAME_STATE;

} /* hdrsrchrfdiv_set_ramp_down_state_tf */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_CONSIDER_DIV_SWITCH

DESCRIPTION
  Consider switching diversity demod on/off based on the AGC values
  to improve overall throughput performance.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static stm_state_type hdrsrchrfdiv_consider_div_switch_tf
( 
  void                       *payload
    /* Payload */
)
{
  int                        rx0_agc;
    /* RX0 rxagc value */

  int                        rx1_agc;
    /* RX1 rxagc value */

  boolean                    div_enabled;
    /* Is diversity enabled */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  NOTUSED( payload );

#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
  if( hdrsrchrfdiv.gpio_status == HDRSRCH_DIV_GPIO_LOW_VALUE )
  {
    return STM_SAME_STATE;
  }
#endif

  hdrsrchrf_get_rx_power( &rx0_agc, &rx1_agc, &div_enabled );
    /* Capture the RX AGC values. */
  
  hdrsrchrfdiv_filter_rx_power( (int32)rx0_agc, (int32)rx1_agc );
    /* Filter Rx power */

  if ( hdrsrchrfdiv.demod_enabled == TRUE )
  {
    if ( 
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
         !hdrsrchrfdiv.second_chain_only &&
         /* Ignore imbalance for 2nd chaint test */
#endif     
         ( hdrsrchrf.rx_power.rx0 - hdrsrchrf.rx_power.rx1 ) 
         >= HDRSRCHRFDIV_SINGLE_ANT_DEMOD_THRESH )
    {
      hdrsrchrfdiv.agc_delta_small = FALSE;
        /* AGC delta is not small */

      HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
                      "Turn off div RX0 %d, RX1 %d, Filtered RX0 %d, RX1 %d",
                      rx0_agc, 
                      rx1_agc, 
                      hdrsrchrf.rx_power.rx0, 
                      hdrsrchrf.rx_power.rx1 );

      hdrsrchrfdiv_diversity_init( FALSE );
        /* Turn off finger and searcher diversity settings. */
    }
  }
  else /* hdrsrchrfdiv.demod_enabled == FALSE */
  {
    if ( 
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
         !hdrsrchrfdiv.second_chain_only &&
         /* Ignore imbalance for 2nd chaint test */
#endif
        ( hdrsrchrf.rx_power.rx0 - hdrsrchrf.rx_power.rx1 ) 
         <= HDRSRCHRFDIV_DUAL_ANT_DEMOD_THRESH )
    {

      hdrsrchrfdiv.agc_delta_small = TRUE;
        /* AGC delta is small */

      HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
                      "Turn on div RX0 %d, RX1 %d, Filtered RX0 %d, RX1 %d",
                      rx0_agc, 
                      rx1_agc, 
                      hdrsrchrf.rx_power.rx0, 
                      hdrsrchrf.rx_power.rx1 );

      hdrsrchrfdiv_diversity_init( TRUE);
        /* Turn on finger and searcher diversity settings. */
    }
  }

  return STM_SAME_STATE;

} /* hdrsrchrfdiv_consider_div_switch_tf */


/*===========================================================================

FUNCTION HDRSRCHRFDIV_RELEASE_DIV_IN_TRAFFIC

DESCRIPTION
  Handle the diversity release event in traffic state.
  Tell the MDSP about the div tune away 32 slots in advance of the tune away.

DEPENDENCIES
  None

RETURN VALUE
  Next state.

SIDE EFFECTS
  None

===========================================================================*/

static stm_state_type hdrsrchrfdiv_release_div_in_traffic( void )
{

  hdrsrchrf_retain_lock_state_t         div_rl_state;
    /* State of div retain lock. */

  hdrsrchrf_retain_lock_state_t         pri_rl_state;
    /* State of primary retain lock. */

  uint32                                sclks_left;
    /* Sclks remaining until lock release */

  uint32                                pri_sclks_left;
    /* Sclks remaining until primary chain lock release */

  uint32                                sclks_now;
    /* Sclks remaining until lock release */

  unsigned                              time_to_unlock = 0;
    /* The number of sclks before we need to unlock. */

  unsigned                              ms_left;
    /* Time left in milliseconds. */

  uint16                                drc_length = 0;
    /* DRC length currently set in the MDSP. */

  int32                                 time_elapsed_ms = 0;
    /* Time elapsed during function execution. We have a very specific real time
       requirement that causes us to have to finely control a timer. */

  stm_state_type                        next_state = STM_SAME_STATE;
    /* The next state that we should transition to. */

  uint8                                 sa;
    /* Loop index of subaset */

  boolean                               small_pkt = FALSE;
    /* Flag to indicate small packet */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_DIV_CHAIN, &div_rl_state );
    /* Retrieve the diversity retain lock state. */

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &pri_rl_state );
    /* Retrieve the primary retain lock state. */

  sclks_now = timetick_get();
    /* Current time in sclks. */

  sclks_left = hdrsrchutil_calc_time_to_tune_away( 
                                      sclks_now,
                                      (int32) HDRSRCHRFDIV_DIV_DRC_RAMP_SCLK,
                                      &div_rl_state,
                                      &hdrsrchrfdiv.no_tune_away,
                                      &hdrsrchrfdiv.end_time_sclks,
                                      NULL);
    /* How much more time should we stay with diversity? */

  ms_left = timetick_cvt_from_sclk( sclks_left, T_MSEC );
    /* Convert lock extension to milliseconds. */

  pri_sclks_left = hdrsrchrfdiv_calc_time_to_pri_tune_away( &pri_rl_state, 
                                                         sclks_now, TRUE );
    /* Determine the minimum time left before we tune away the primary ant */

  HDR_MSG_SRCH_4( MSG_LEGACY_MED,
                  "Diversity Tune Away=%d End Time=%d ms Pri sclks left=%d Div sclks left=%d",
                  !hdrsrchrfdiv.no_tune_away,
                  ms_left,
                  pri_sclks_left,
                  sclks_left );

    /* If we aren't already planning on tuning away the entire radio soon 
       schedule diversity to be shut off. In other words, if the div
       tune away is within 2 ms of the primary tune away, just let the normal
       TC tune away happen. 
       If primary chain tune away is disabled, need to start diversity chain timer
       even though primary chain unlock_by time and diversity chain unlock_by time
       is close. Otherwise div chain might not be released at the unlock_by time,
       because primary chain tune away is disabled and we don't release primary chain
       at unlock_by time.
       if (Div time left + 2 ms < Primary time left) or (pri chain tune away is disbaled )
    */
  if ( ( !hdrsrchrfdiv.no_tune_away ) &&
       ( ( pri_sclks_left > ( sclks_left + HDRSRCHRFDIV_TA_OVERHEAD_SCLK ) ) ||
         ( hdrsrchtc_tune_away_allowed() == FALSE )
       )
     )
  {
    /* We have greater than 32 slots plus overhead left before div tune away */
    if ( ms_left > HDRSRCHRFDIV_DIV_DRC_RAMP_MS + 5 )
    {
      if ( hdrsrchrfdiv.tune_away_state >= HDRSRCHRFDIV_DIV_TUNE_AWAY_DRC_RAMP )
      {
        /* We came back to beginning of tune away timeline while we were
           in DRC ramp down state. As long as we are in the middle
           of div DRC rampdown. */

        hdrsrchrfdiv_stop_div_drc_ramp_down();
          /* Get back to normal mode of DRC i.e. stop DRC ramp down mode */
      }

      hdrsrchrfdiv.tune_away_state = HDRSRCHRFDIV_DIV_TUNE_AWAY_NO_CONFLICT;
        /* DRC Ramp down is started. */

      time_to_unlock = ms_left - HDRSRCHRFDIV_DIV_DRC_RAMP_MS;
        /* Calculate the time in sclks before we should release
           the lock. */
    }
#ifdef FEATURE_HDR_HARD_DIV_TUNE_AWAY
    else if ( ms_left > HDRSRCHRFDIV_PRI_CHAIN_PRED_TIME_MS + 5)
#else
    else if ( ms_left > HDRSRCHRFDIV_UNLOCK_TIME_BUF_MS + 5)
#endif /* FEATURE_HDR_HARD_DIV_TUNE_AWAY */
    {
      if ( hdrsrchrfdiv.tune_away_state < HDRSRCHRFDIV_DIV_TUNE_AWAY_DRC_RAMP )
      {

        time_to_unlock = ms_left - HDRSRCHRFDIV_UNLOCK_TIME_BUF_MS; 
          /* Calculate the time in sclks before we should release
             the lock. */

        /* As long as we aren't in the middle of another type of DRC
           ramp down, start the div DRC ramp down. */
        if ( !HDRDRC_GET_DRC_RAMP_DOWN() )
        {
#ifdef FEATURE_HDR_HARD_DIV_TUNE_AWAY
          /* Check small packet transmitted on any carrier */
          for ( sa = 0; sa < hdrsrchset.subaset_cnt; sa++ )
          {
            if ( HDRSRCHRFDIV_IS_SMALLPACKET( 
                   HDRDRC_GET_CURRENT_DRC( hdrsrchset.subaset[sa].demod_idx ) ) )
            {
              small_pkt = TRUE;
                /* Set the small_pket flag to true */
              break;
            }
          }

          if ( small_pkt && ( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].reason != 
                                                HDRSRCHRF_LOW_LAT_TRAFFIC ) )
          {
            drc_length = hdrfmac_get_drc_length( hdrsrchset.subaset[ 0 ].tca_idx );
              /* Capture the current DRC length. */

            HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Hard div ramp down for DRC length=%d",
                            drc_length );

            hdrsrchrfdiv_div_tune_away_drc_ramp_down
                 ( (uint32) MS_TO_SLOTS( time_to_unlock ), HDRDRC_TUNE_AWAY_BOTH );
              /* Start tune away for remaining time. We will send a NULL cover temporarily so 
                 that no data integrity can be compromised during the transition, later we 
                 will switch to primary chain only. */

            hdrsrchrfdiv.tune_away_state = HDRSRCHRFDIV_DIV_HARD_TUNE_AWAY_DRC_RAMP;
              /* DRC Ramp down is started. */

            time_to_unlock = ms_left - HDRSRCHRFDIV_PRI_CHAIN_PRED_TIME_MS; 
              /* Calculate the time in sclks before we should release
                 the lock. */
          }
          else
#endif /* FEATURE_HDR_HARD_DIV_TUNE_AWAY */
          {
            hdrsrchrfdiv_div_tune_away_drc_ramp_down (
                 MS_TO_SLOTS( ms_left - HDRSRCHRFDIV_UNLOCK_TIME_BUF_MS ),
                 HDRDRC_TUNE_AWAY_DIV );
            /* start tune away for remaining time */

            hdrsrchrfdiv.tune_away_state = HDRSRCHRFDIV_DIV_TUNE_AWAY_DRC_RAMP;
              /* DRC Ramp down is started. */
          }
        }
      }
    }
#ifdef FEATURE_HDR_HARD_DIV_TUNE_AWAY
    else if ( hdrsrchrfdiv.tune_away_state == 
              HDRSRCHRFDIV_DIV_HARD_TUNE_AWAY_DRC_RAMP )
    {
      if ( ms_left > HDRSRCHRFDIV_UNLOCK_TIME_BUF_MS )
      {
        hdrsrchrfdiv_set_div_ramp_down_mode( HDRDRC_TUNE_AWAY_DIV_TRANS );

        HDR_MSG_SRCH( MSG_LEGACY_MED, "Feed only primary chain to predictor" );

        time_to_unlock = ms_left - HDRSRCHRFDIV_UNLOCK_TIME_BUF_MS; 
        /* Calculate the time in sclks before we should release
           the lock. */
      }
      else
      { 
        hdrsrchrfdiv_release_diversity();
          /* The event occuring while diversity is enabled means we now must
             turn off diversity. */

        time_elapsed_ms = (int32) ( TIMETICK_SCLK_FROM_MS( ms_left + 8 ) -
                                    ( timetick_get() - sclks_now ) );
          /* Time to be elapsed before the release deadline and 1 slot(2ms) beyond that 
             to make sure 2nd sinr dip is covered completely */

        /* If the timer is less than is supported on Q6/Blast timers, force the timer
           to be the minimum time. */
        if ( time_elapsed_ms < HDRSRCHRFDIV_MIN_TIMER_SCLK )
        {
          time_elapsed_ms = HDRSRCHRFDIV_MIN_TIMER_SCLK;
        }

        timer_set( &hdrsrchrfdiv.clr_null_cover_timer, 
                   (uint32) HDRSRCH_NAT_NUM( time_elapsed_ms ),
                   0, T_SCLK );
          /* Set timer to remove the NULL DRC around the diversity release 
             deadline. This code is special since there is a large variation 
             time duration in the release diversity operation.  Account for 
             this variation and set a higher resolution timer normalizing 
             for the time delay. */

        next_state = HDRSRCHRFDIV_HARD_DIV_TUNE_AWAY_STATE;
          /* Time to enter the hard tune away state */
      }
    }
#endif /* FEATURE_HDR_HARD_DIV_TUNE_AWAY */
    else
    {
      hdrsrchrfdiv_release_diversity();
        /* The event occuring while diversity is enabled means we now must
           turn off diversity. */


      HDR_MSG_SRCH( MSG_LEGACY_MED, "hdrsrchrfdiv_release_div_event_tf() wait to acquire");

      next_state = HDRSRCHRFDIV_WAIT_TO_ACQUIRE_STATE;
        /* Start diversity when the chain is available again. */
    }

    if ( time_to_unlock )
    {
      timer_set( &hdrsrchrfdiv.release_div_timer, time_to_unlock, 0, T_MSEC );
        /* Set timer for when we should release the diversity chain. */
    }
  }
  else
  {
    if ( hdrsrchrfdiv.tune_away_state >= HDRSRCHRFDIV_DIV_TUNE_AWAY_DRC_RAMP )
    {
      /* We came back to begining of tune away timeline while we were
         in DRC ramp down state */

      hdrsrchrfdiv_stop_div_drc_ramp_down();
        /* get back to normal mode of DRC i.e. stop DRC ramp down mode */
    }

    (void) timer_clr( &hdrsrchrfdiv.release_div_timer, T_NONE );
      /* clear the release diversity timer if it is active */

    hdrsrchrfdiv.tune_away_state = HDRSRCHRFDIV_DIV_TUNE_AWAY_NO_CONFLICT;
      /* DRC Ramp down is started. */
  }

  return next_state;

} /* hdrsrchrfdiv_release_div_in_traffic */

/*===========================================================================

FUNCTION HDRSRCHRFDIV_RELEASE_DIV_IN_NON_TRAFFIC

DESCRIPTION
  Handle the diversity release event in idle, acq state.
  No need to do drc rampdown.

DEPENDENCIES
  None

RETURN VALUE
  Next State.

SIDE EFFECTS
  None

===========================================================================*/

static stm_state_type hdrsrchrfdiv_release_div_in_non_traffic ( void )
{

  hdrsrchrf_retain_lock_state_t         div_rl_state;
    /* State of div retain lock. */

  hdrsrchrf_retain_lock_state_t         pri_rl_state;
    /* State of primary retain lock. */

  int32                                 sclks_left;
    /* Sclks remaining until lock release */

  uint32                                pri_sclks_left;
    /* Sclks remaining until primary chain lock release */

  uint32                                sclks_now;
    /* Sclks remaining until lock release */

  unsigned                              ms_left;
    /* Time left in milliseconds. */

  unsigned                              time_to_unlock = 0;
    /* The number of ms before we need to unlock. */

  stm_state_type                        next_state = STM_SAME_STATE;
    /* The next state that we should transition to. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_DIV_CHAIN, &div_rl_state );
    /* Retrieve the diversity retain lock state. */

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &pri_rl_state );
    /* Retrieve the primary retain lock state. */

  sclks_now = timetick_get();
    /* Current time in sclks. */

  sclks_left = hdrsrchutil_calc_time_to_tune_away( 
                                      sclks_now,
                                      (int32) HDRSRCHRFDIV_OVERHEAD_SCLK,
                                      &div_rl_state,
                                      &hdrsrchrfdiv.no_tune_away,
                                      &hdrsrchrfdiv.end_time_sclks,
                                      NULL);
    /* How much more time should we stay with diversity? */

  ms_left = timetick_cvt_from_sclk( sclks_left, T_MSEC );
    /* Convert lock extension to milliseconds. */

  /* if unlock is ignored by primary chain, default primary sclks value */
  if ( hdrsrchrf_get_pri_chain_unlock_status() == FALSE )
  {
    pri_sclks_left = hdrsrchrfdiv_calc_time_to_pri_tune_away( &pri_rl_state, 
                                                              sclks_now, FALSE );
      /* Determine the minimum time left before we tune away the primary ant */
  }
  else
  {
     pri_sclks_left = 0xFFFFFFFF;
  }

 
  HDR_MSG_SRCH_4( MSG_LEGACY_MED,
                  "Diversity Tune Away=%d End Time=%d ms Pri sclks left=%d Div sclks left=%d",
                  !hdrsrchrfdiv.no_tune_away,
                  ms_left,
                  pri_sclks_left,
                  sclks_left );

  /* Make sure diversity chain is released before primary */

  if ( sclks_left >= pri_sclks_left   )
  {
    /* Reduce ms_left by another 2 ms*/
    sclks_left = pri_sclks_left - HDRSRCHRFDIV_TA_OVERHEAD_SCLK;
  }

  /* Keep some buffer between trm grant end and diversity release (18-13 ms)*/
  if ( hdrsrch_get_current_state() == HDRSRCH_ACQ_STATE )
  {
     sclks_left -= HDRSRCHRFDIV_ACQ_OVERHEAD_SCLK;
  }
  else
  {
  sclks_left -= HDRSRCHRFDIV_OVERHEAD_SCLK;
  }

  if( sclks_left <= 0 )
  {
    time_to_unlock = 0;
  }
  else
  {
  time_to_unlock = timetick_cvt_from_sclk( sclks_left, T_MSEC );
  }

  HDR_MSG_SRCH_1( MSG_LEGACY_MED,
                  "Diversity Tune Away time to unlock %d",
                  time_to_unlock );
  /* Simply set a timer and release diversity chain. */
  if (  !hdrsrchrfdiv.no_tune_away ) 
  {
    if( time_to_unlock )
    {
      timer_set( &hdrsrchrfdiv.release_div_timer, time_to_unlock, 0, T_MSEC );
       /* Set timer for when we should release the diversity chain. */
    }
    else
    {
       if ( ( hdrsrch_get_current_state() == HDRSRCH_ACQ_STATE )  
        || ( hdrsrch_get_current_state() == HDRSRCH_IDLE_STATE ) )
       {
          /* Crash reported while releasing Diversity immediately while
             Diversity search is in progress. This fix is currently made only
              for ACQUISITION/IDLE/ACCESS state to contain changes and reduce risk */

          /* Before releasing Diversity, check if any active searches
             programmed with diversity. If no, then go ahead and release */
          if (hdrsrchdrv_is_div_search_active() == FALSE)
          {
           hdrsrchrfdiv_release_diversity();
           /* The event occuring while diversity is enabled means we now must
           turn off diversity. */

           HDR_MSG_SRCH( MSG_LEGACY_MED, "hdrsrchrfdiv_release_div_event_tf() wait to acquire");

           next_state = HDRSRCHRFDIV_WAIT_TO_ACQUIRE_STATE;
           /* Start diversity when the chain is available again. */
          }  
          else
          {
             /* If there is Active search with Diversity, then wait 5ms more
                for the search to finish, come back here and check again. 
                Also prevent any new searches being programmed with Diversity in 
                this gap. */ 
             hdrsrchrfdiv.demod_enabled = FALSE;
              /* This flag is made TRUE again during Diversity Init */

             timer_set( &hdrsrchrfdiv.release_div_timer, 5, 0, T_MSEC );
              /* Set timer for when we should release the diversity chain. */

             HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Div Search Active, wait 5ms more to release Div");
           }
        }
        else
        {
          hdrsrchrfdiv_release_diversity(); 
            /* The event occuring while diversity is enabled means we now must
                 turn off diversity. */

          HDR_MSG_SRCH( MSG_LEGACY_MED, "hdrsrchrfdiv_release_div_event_tf() enter acquiring");

          next_state = HDRSRCHRFDIV_ACQUIRING_RF_STATE;
            /* Start diversity when the chain is available again. */
        }
    }  
  }
  
  return next_state;

} /* hdrsrchrfdiv_release_div_in_non_traffic */
/*===========================================================================

FUNCTION HDRSRCHRFDIV_RELEASE_DIV_EVENT_TF

DESCRIPTION
  Handle the diversity release event.
  
DEPENDENCIES
  None

RETURN VALUE
  Next state.

SIDE EFFECTS
  None

===========================================================================*/

static stm_state_type hdrsrchrfdiv_release_div_event_tf
( 
  void                       *payload
    /* Payload */
)
{
  stm_state_type                        next_state = STM_SAME_STATE;
    /* The next state that we should transition to. */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  NOTUSED( payload );

  if ( hdrsrch_get_current_state() == HDRSRCH_CONNECTED_STATE )
  {
    next_state = hdrsrchrfdiv_release_div_in_traffic( );
  }
  else
  {
    next_state = hdrsrchrfdiv_release_div_in_non_traffic( );

  }
  return next_state;

} /* hdrsrchrfdiv_release_div_event_tf */

/*=========================================================================

FUNCTION HDRSRCHRFDIV_RUNTIME_PREF_CTRL_TF

DESCRIPTION 
  Handles run-time diversity preference control from upper layers

DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHRFDIV_INACTIVE_STATE or STM_SAME_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchrfdiv_runtime_pref_ctrl_tf
(
  void                       *payload
    /* Payload */
)
{
  hdrsrch_rx_div_enum_type div_ctrl = *((hdrsrch_rx_div_enum_type *) payload );

  uint8                      demod_carrier_mask;
    /* Demod carrier mask */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  hdrsrchrfdiv.requested_by_app = ( div_ctrl > 0 ? TRUE: FALSE );
    /* Saved the diversity control by app */

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Div runtime preference enable:%d", div_ctrl );

  if ( hdrsrchrfdiv.requested_by_app == TRUE )
  {
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL   
    /* If second chain was previously true but not now, reset diversity */
    if( div_ctrl != HDRSRCH_SECOND_CHAIN_ONLY )
    {
      if ( ( hdrsrchrfdiv.second_chain_only == TRUE ) && hdrsrchafc_afc_uses_rot()
             && ( hdrsrch_get_current_state() == HDRSRCH_CONNECTED_STATE ) )
      {
        hdrsrchrfdiv.second_chain_only = FALSE;

        demod_carrier_mask = hdrsrchutil_get_demod_carrier_mask();
          /* Get which demod carriers are enabled */

        hdrsrcheq_assign_eqs( demod_carrier_mask );
          /* Assign equalizers */
      }
      else 
      {
        hdrsrchrfdiv.second_chain_only = FALSE;
      }
      
#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
      /* Send runtime disable */
      HDR_MSG_SRCH( MSG_LEGACY_MED, "ARD runtime enabled" );
      hdrsrchard_event( HDRSRCHARD_EVENT_RUNTIME_ENABLE );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
    }
     /* Else if second chain is true now */
    else if( div_ctrl == HDRSRCH_SECOND_CHAIN_ONLY )
    {
      hdrsrchrfdiv.second_chain_only = TRUE;

      /* if second chain deassign equalizers. */
      hdrsrcheq_deassign_all_eqs();

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
      /* Send runtime disable */
      HDR_MSG_SRCH( MSG_LEGACY_MED, "ARD runtime disabled" );
      hdrsrchard_event( HDRSRCHARD_EVENT_RUNTIME_DISABLE );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
    }    
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */

    /* Check to make sure diversity is allowed, also requested by srch stm and
       (currently diversity is not enabled or switch to second chain only) */
    if ( ( HDRSRCHRFDIV_IS_DIVERSITY_ALLOWED ) &&
         ( hdrsrchrfdiv.requested_by_srch == TRUE ) )
    {
      if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled == FALSE )
      {  
        HDR_MSG_SRCH( MSG_LEGACY_MED, "hdrsrchrfdiv_runtime_pref_ctrl_tf() enter acquiring");

        return HDRSRCHRFDIV_ACQUIRING_RF_STATE;
          /* Request diversisty chain now */
      }
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL   
      else if ( hdrsrchrfdiv.second_chain_only == TRUE )
      {
        hdrsrchrfdiv_diversity_init( TRUE );
          /* Turn on finger and searcher diversity settings. */
      }
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL  */
    }
  }
  else
  {
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
    /*  If second chain was previously true, but now disabling diversity
        then reset the antenna config */

    if ( ( hdrsrchrfdiv.second_chain_only == TRUE ) && (hdrsrchafc_afc_uses_rot())
           && ( hdrsrch_get_current_state() == HDRSRCH_CONNECTED_STATE ) )
    {

      hdrsrchrfdiv.second_chain_only = FALSE;
         
      demod_carrier_mask = hdrsrchutil_get_demod_carrier_mask();
        /* Get which demod carriers are enabled */

      hdrsrcheq_assign_eqs( demod_carrier_mask );
        /* Assign equalizers */
    }
    else
    {
      hdrsrchrfdiv.second_chain_only = FALSE;
    }

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
    /* Send runtime disable */
    HDR_MSG_SRCH( MSG_LEGACY_MED, "ARD runtime enabled" );
    hdrsrchard_event( HDRSRCHARD_EVENT_RUNTIME_ENABLE );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */

    if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled )
    {
      return HDRSRCHRFDIV_INACTIVE_STATE;
        /* Turn off diversity when diversity is enabled */
    }
  }

  return STM_SAME_STATE;

} /* hdrsrchrfdiv_runtime_pref_ctrl_tf */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_HARD_TA_RUNTIME_PREF_CTRL_TF

DESCRIPTION 
  Handles run-time diversity preference control from upper layers at
  hard div tuneaway state.

DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHRFDIV_INACTIVE_STATE or STM_SAME_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchrfdiv_hard_ta_runtime_pref_ctrl_tf
(
  void                       *payload
    /* Payload */
)
{
  hdrsrch_rx_div_enum_type div_ctrl = *((hdrsrch_rx_div_enum_type *) payload );

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  hdrsrchrfdiv.requested_by_app = ( div_ctrl > 0 ? TRUE: FALSE );
    /* Saved the diversity control by app */

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Div runtime preference enable:%d", div_ctrl );

  if ( hdrsrchrfdiv.requested_by_app == TRUE )
  {
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL   
    /* If second chain was previously true but not now, reset diversity */
    if( div_ctrl != HDRSRCH_SECOND_CHAIN_ONLY )
    {
      hdrsrchrfdiv.second_chain_only = FALSE;
    }
     /* Else if second chain is true now */
    else if( div_ctrl == HDRSRCH_SECOND_CHAIN_ONLY )
    {
      hdrsrchrfdiv.second_chain_only = TRUE;
    }    
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */

    /* When clear null DRC cover timer expired, request div */

  }
  else
  {
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
    /*  If second chain was previously true, but now disabling diversity
        then reset the antenna config */

    hdrsrchrfdiv.second_chain_only = FALSE;
         
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */

    if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled )
    {
    return HDRSRCHRFDIV_INACTIVE_STATE;
        /* Turn off diversity when diversity is enabled */
    }
  }

  return STM_SAME_STATE;

} /* hdrsrchrfdiv_hard_ta_runtime_pref_ctrl_tf */


#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT 
/*===========================================================================

FUNCTION HDRSRCHRFDIV_CHANGE_DIV_STATUS_TF

DESCRIPTION
  This is transfer function to turn on or off diversity based on GPIO status 
  change.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static stm_state_type hdrsrchrfdiv_change_div_status_tf
( 
  void                       *payload
    /* Payload */
)
{
  uint32       gpio_value = *( ( uint32 *)payload );
    /* GPIO value */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Diversity init by gpio %d", gpio_value);

  if ( ( gpio_value != HDRSRCH_DIV_GPIO_LOW_VALUE ) && 
       ( hdrsrchrfdiv.demod_enabled == FALSE ) && 
       ( hdrsrchrfdiv.agc_delta_small ) )
  {
    hdrsrchrfdiv_diversity_init( TRUE );
      /* Enable diversity */
  }
  else
  if ( ( gpio_value == HDRSRCH_DIV_GPIO_LOW_VALUE ) && 
       ( hdrsrchrfdiv.demod_enabled == TRUE ) )
  {
    hdrsrchrfdiv_diversity_init( FALSE );
      /* Disable diversity */
  }

  return STM_SAME_STATE;

} /* hdrsrchrfdiv_change_div_status_tf */
#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */


/*===========================================================================

                  STATE  MACHINE  ENTRY/EXIT  FUNCTIONS

===========================================================================*/

/*=========================================================================

FUNCTION HDRSRCHRFDIV_ENTER_INACTIVE

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchrfdiv_enter_inactive
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "hdrsrchrfdiv_enter_inactive" );

  if ( previous_state != HDRSRCHRFDIV_WAIT_TO_ACQUIRE_STATE )
  {
    hdrsrchrfdiv_stop_div_drc_ramp_down( );
    /* Stop DRC ramp down mode in any case if tune away active. eg, App request 
       to disable diversity during rampdown */

    hdrsrchrfdiv_release_diversity();
      /*  Release diversity if it is enabled. */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "deactivated from wait to acq, skip release" );
  }

  hdrsrchrfdiv.tune_away_state = HDRSRCHRFDIV_DIV_TUNE_AWAY_NO_CONFLICT;

  hdrsrchrfdiv.rel_pend = FALSE;

  hdrsrchmcpm_set_diversity_status( FALSE );
    /* Completed processing, MIPS can be reduced and hence report diversity 
         status to MCPM as false */

} /* hdrsrchrfdiv_enter_inactive */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_ENTER_ACQUIRING_RF

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchrfdiv_enter_acquiring_rf
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "enter_acquiring" );

  hdrsrchrfdiv_request_diversity();   
    /* Try to get the diversity chain from TRM. */

  hdrsrchmcpm_set_diversity_status( FALSE );
    /* Completed processing, MIPS can be reduced and hence report diversity 
          status to MCPM as false */

} /* hdrsrchrfdiv_enter_acquiring_rf */

/*=========================================================================

FUNCTION HDRSRCHRFDIV_DO_WARM_UP

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  DIV enable delay, or HDRSRCHRFDIV_DIV_ENABLE_DELAY_INVALID if
  cannot be enabled.

SIDE EFFECTS
  None

=========================================================================*/
uint32 hdrsrchrfdiv_do_warm_up( void )
{
  uint32         div_enable_delay = 0;             
    /* How long should we wait before completing diversity enabling. */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  rfm_device_enum_type               rf_device;
    /* The RF physical chain mapped into the logical chain. */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_QTA
  if ( hdrsrchrfdiv.qta_start == TRUE )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR QTA: div granted for QTA end %d", hdrsrchrfdiv.qta_start );

    if ( hdrsrchsus_is_pri_chain_settled() )
    {
      hdrsrchmcpm_set_diversity_status( TRUE );
        /* Starting the processing and report diversity status to MCPM 
              to increase the MIPS */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDR Div: process div change");
      rf_device = hdrsrchrf_get_rf_device(
                     HDRSRCHRF_CHAIN_TO_TRM( HDRSRCHRF_DIV_CHAIN ) );
      hdrmultirat_process_div_change( TRUE, rf_device );
      hdrmultirat_confirm_valid_freq_id();
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

      hdrsrchrfdiv.qta_start = FALSE;
      hdrsrchrf_set_qta_client_state( HDRSRCHRF_QTA_END, NULL );
        /* Let TRM know QTA has ended */
      div_enable_delay = HDRSRCHRFDIV_DIV_ENABLE_DELAY_MS;
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDR QTA: Primary chain not done, skipping QTA end to TRM " );
      div_enable_delay = HDRSRCHRFDIV_DIV_ENABLE_DELAY_INVALID;
    }
  }
  else
#endif /* FEATURE_HDR_QTA */
  {
    hdrsrchmcpm_set_diversity_status( TRUE );
      /* Starting the processing and report diversity status to MCPM 
            to increase the MIPS */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDR Div: process div change");
    rf_device = hdrsrchrf_get_rf_device(
                   HDRSRCHRF_CHAIN_TO_TRM( HDRSRCHRF_DIV_CHAIN ) );
    hdrmultirat_process_div_change( TRUE, rf_device );
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

    div_enable_delay = hdrsrchrfdiv_enable_div_chain();
      /* This means we now must have permission to use the RF lock. */
  }

  return div_enable_delay;
}

/*=========================================================================

FUNCTION HDRSRCHRFDIV_ENTER_WARM_UP

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchrfdiv_enter_warm_up
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{

  uint32         div_enable_delay = 0;             
    /* How long should we wait before completing diversity enabling. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].owns_chain = TRUE;
    /* Update the fact this chain is now owned by HDR. */

  div_enable_delay = hdrsrchrfdiv_do_warm_up( );

  if ( div_enable_delay != HDRSRCHRFDIV_DIV_ENABLE_DELAY_INVALID )
  {
    hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled = TRUE;
      /* Diversity is enabled. */

    timer_set( &hdrsrchrfdiv.warmup_timer, div_enable_delay, 0, T_MSEC );
      /* Set the timer to go off when the warmup period has expired. */
  }

} /* hdrsrchrfdiv_enter_warm_up */

/*=========================================================================

FUNCTION HDRSRCHRFDIV_ENTER_ENABLED

DESCRIPTION 
  Enter the diversity enabled state. Actually turn on diversity here.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchrfdiv_enter_enabled
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{
#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
   hdrsrchsm_cmd_payload_type cmd_payload;
    /* Command + payload for internal stm command */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

   trm_retain_chain_lock_input_info  retain_lock_info;
    /* Input to retain_lock */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf.rx_power.length = 0;
    /* Reset receive power filter */

  hdrsrchrfdiv.agc_delta_small = TRUE;
    /* rxAGC delta is small by default */

  hdrsrchrfdiv_set_diversity_comb_mode();
    /* Write the chosen combining mode to the MDSP. */

#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
  hdrsrchrfdiv_diversity_init_by_gpio();
    /* Enable baseband diversity processing */
#else
  hdrsrchrfdiv_diversity_init( TRUE );
    /* Turn on finger and searcher diversity settings. */
#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */

  hdrsrchrfdiv_stop_div_drc_ramp_down();
    /* Make sure that DRC ramp down stopped indication is set to FW now that 
       diversity is restarted */

  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rl_state.event = HDRSRCHRF_UNLOCK_CANCELLED;
    /* Start off assuming we have the lock indefinitely. */

  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rl_state.unlock_by_sclk = 0;
    /* Start off assuming we have the lock indefinitely. */

  hdrsrchrf_change_priority( HDRSRCHRF_DIV_CHAIN, 
                             HDRSRCHRFDIV_GET_DIV_TRM_PRIORITY);
    /* Change the request prioirty based on the current state 
    just before retain lock */

  retain_lock_info.client = HDRSRCHRF_DIV_TRM_CLIENT;

  trm_retain_chain_lock( &retain_lock_info );
  /* Now retain the lock for an indefinite period of time. */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
  cmd_payload.cmd              = HDRSRCH_DIV_ENABLED_CMD;
  cmd_payload.payload.not_used = HDRSRCHSM_PAYLOAD_NOT_USED;
   /* No HDR srch payload other than command */

   HDRSRCH_STM_ALLOC_PUT_INTERNAL_CMD( &HDRSRCH_SM,
                                       &hdrsrch_stm_group, 
                                       HDRSRCH_DIV_ENABLED_CMD,
                                       hdrsrchsm_cmd_payload_type,
                                       cmd_payload );
   /* Let reacq know we are ready */

    (void) timer_clr( &hdrsrchrfdiv.div_enable_timer, T_NONE );
      /* Clear the enable diversity timer if still active */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_DIV_GRANTED );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

} /* hdrsrchrfdiv_enter_enabled */

#ifdef FEATURE_HDR_SELECTIVE_TXD
/*=========================================================================

FUNCTION HDRSRCHRFDIV_EXIT_ENABLED

DESCRIPTION 
  Exit the diversity enabled state. Notify STXD DIV is released

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchrfdiv_exit_enabled
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{
  hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_DIV_RELEASED );
}
#endif /* FEATURE_HDR_SELECTIVE_TXD */

/*=========================================================================

FUNCTION HDRSRCHRFDIV_ENTER_HARD_DIV_TUNE_AWAY

DESCRIPTION 
  Enter the hard diversity tune away state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchrfdiv_enter_hard_div_tune_away
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  NOTUSED( payload );
  NOTUSED( previous_state );

  hdrsrchmcpm_set_diversity_status( FALSE );
    /* Completed processing, MIPS can be reduced and hence report diversity 
          status to MCPM as false */

} /* hdrsrchrfdiv_enter_hard_div_tune_away */

#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
/*===========================================================================

FUNCTION HDRSRCHRFDIV_CONFIG_GPIO

DESCRIPTION
  This function will config GPIO through TLMM interface.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_config_gpio( void )
{
  hdr_cell_gpio_config = 
    DAL_GPIO_CFG( HDRSRCH_DIV_GPIO_CELL, 0, DAL_GPIO_INPUT, DAL_GPIO_NO_PULL,  DAL_GPIO_2MA );
  hdr_pcs_gpio_config = 
    DAL_GPIO_CFG( HDRSRCH_DIV_GPIO_PCS, 0, DAL_GPIO_INPUT, DAL_GPIO_NO_PULL,  DAL_GPIO_2MA );

  if ( DAL_SUCCESS == DAL_TlmmDeviceAttach(DALDEVICEID_TLMM, &hdrsrchrfdiv_hTlmm) )
  {
    DalDevice_Open(hdrsrchrfdiv_hTlmm, DAL_OPEN_SHARED);
  }
  else
  {
     HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Dal Device attach failed..." );
  }

  if( DAL_SUCCESS != 
        DalTlmm_ConfigGpio( hdrsrchrfdiv_hTlmm, hdr_cell_gpio_config, DAL_TLMM_GPIO_ENABLE ))
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "gpio for cell configured successfully" );
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "gpio for cell not configured successfully" );
  }

  if( DAL_SUCCESS != 
        DalTlmm_ConfigGpio( hdrsrchrfdiv_hTlmm, hdr_pcs_gpio_config, DAL_TLMM_GPIO_ENABLE ))
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "gpio for cell configured successfully" );
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "gpio for pcs not configured successfully" );
  }

} /* hdrsrchrfdiv_config_gpio */
#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */


/*=========================================================================

FUNCTION HDRSRCHRFDIV_WAIT_TO_ACQ_CB

DESCRIPTION 
  Called when the RF is completed warming up.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchrfdiv_wait_to_acq_cb
(
  timer_cb_data_type              data
    /* Arbitrary data given to timer_def( ) to pass to this callback function */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                &hdrsrchcom_stm_group, 
                                HDRSRCHRFDIV_WAIT_DONE_CMD ); 
    /* Send the command to the RF Diversity State Machine */

} /* hdrsrchrfdiv_warmup_timer_cb */

/*===========================================================================

FUNCTION HDRSRCHRFDIV_INIT

DESCRIPTION
  Initializes the diversity state machine.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void hdrsrchrfdiv_init
(
  stm_group_type *group
    /* The group this state machine belongs to. */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrfdiv.enable_div_ramp = TRUE;
    /* Start off allowing diversity ramp downs. */

  hdrsrchrfdiv.requested_by_app = TRUE;
    /* Diversity is requested by app by default */

  hdrsrchrfdiv.requested_by_srch = FALSE;
    /* Diversity is not requested by search by default */

#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
  hdrsrchrfdiv.second_chain_only = FALSE;
    /* Second chain only is not requested by search by default */
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */

  timer_def( &hdrsrchrfdiv.warmup_timer, NULL, HDRSRCH_TCB,
             0, hdrsrchrfdiv_warmup_timer_cb, 0 );
    /* Define the diversity chain warm up timer. */

  timer_def( &hdrsrchrfdiv.clr_null_cover_timer, NULL, NULL,
             0, hdrsrchrfdiv_clr_null_cover_timer_cb, 0 );
    /* Define the diversity chain null cover clear timer. */

  timer_def( &hdrsrchrfdiv.release_div_timer, NULL, HDRSRCH_TCB,
             0, hdrsrchrfdiv_release_div_timer_cb, 0 );
    /* Define the diversity chain release div timer. */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
  timer_def( &hdrsrchrfdiv.div_enable_timer, NULL, HDRSRCH_TCB,
             0, hdrsrchrfdiv_enable_div_timer_cb, 0 );
    /* Define the diversity chain warm up timer. */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */
  timer_def( &hdrsrchrfdiv.wait_to_acq_timer, NULL, HDRSRCH_TCB,
             0, hdrsrchrfdiv_wait_to_acq_cb, 0 );


#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
  hdrsrchrfdiv_config_gpio( );
    /* Config GPIO */
#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */

  hdrsrchrfdiv_diversity_init( FALSE );
    /* Shut off finger and searcher diversity settings. */

#ifdef FEATURE_HDR_QTA
  hdrsrchrfdiv.qta_start = FALSE;
    /* Initialize QTA flag to false */
#endif /* FEATURE_HDR_QTA */
  

} /* hdrsrchrfdiv_init */

#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
/*===========================================================================

FUNCTION HDRSRCHRFDIV_IS_SECOND_CHAIN_ONLY

DESCRIPTION
  If second chain test call is on or not.
DEPENDENCIES
  None

RETURN VALUE
  If second chain test call is on or not.

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchrfdiv_is_second_chain_only( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 return hdrsrchrfdiv.second_chain_only ;

} /* hdrsrchrfdiv_is_second_chain_only */
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
/*===========================================================================

FUNCTION HDRSRCHRFDIV_ENABLE_DIV_IN_NON_TC

DESCRIPTION
  Call this function to enable diversity in reacq state.
  It will be enable diversity and start timer to make sure
  div is enabled by the time we have reduced sleep time by.
  
DEPENDENCIES
  None
 
PARAMETERS
  div_mask : mask to specify what non tc state we want to enable
             div in.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_enable_div_in_non_tc
( 
  hdrsrch_non_tc_div_state_enum_t div_mask
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If we are in reacq state */
  if ( div_mask & HDRSRCH_DIV_IN_REACQ )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Idle Div: enabling div in reacq" );

    hdrsrchrfdiv_diversity_ctrl( TRUE ); 
      /* Enable Diversity */ 

    timer_set( &hdrsrchrfdiv.div_enable_timer, 
               HDRSRCHUTIL_DIVERSITY_ENABLE_TIME, 0, T_MSEC );
      /* Set the timer to go off when the max time for div enable
         has expired. */                   
    
  }
} /* hdrsrchrfdiv_enable_div_in_non_tc() */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
/*===========================================================================

FUNCTION HDRSRCHRFDIV_ARD_DIVERSITY_CTRL

DESCRIPTION
  Call this function to enable or disable diversity by app. 

DEPENDENCIES
  Must be called by ARD in its transition or state exit/entry function

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_ard_diversity_ctrl
(
  boolean    div_flag,
    /* 1: to enable diversity, 0 : disable diversity */
  boolean    forceful
    /* whether to respect current rampdown process */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  hdrsrchrfdiv.requested_by_srch = div_flag;

  if ( div_flag == TRUE )
  {
    hdrsrchrfdiv.rel_pend = FALSE;

#ifdef FEATURE_HDR_HARD_DIV_TUNE_AWAY
    if ( hdrsrchrfdiv.tune_away_state != 
         HDRSRCHRFDIV_DIV_HARD_TUNE_AWAY_DRC_RAMP )
#endif /* FEATURE_HDR_HARD_DIV_TUNE_AWAY */
    {
      HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                    &hdrsrchcom_stm_group,
                                    HDRSRCHRFDIV_REQUEST_DIV_CMD );
        /* Send the command to the RF Diversity State Machine */
    }
  }
  else
  {
    hdrsrchrfdiv.rel_pend = TRUE;

    if ( forceful
#ifdef FEATURE_HDR_HARD_DIV_TUNE_AWAY
         || ( hdrsrchrfdiv.tune_away_state != 
              HDRSRCHRFDIV_DIV_HARD_TUNE_AWAY_DRC_RAMP )
#endif /* FEATURE_HDR_HARD_DIV_TUNE_AWAY */
       )
    {
      HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                    &hdrsrchcom_stm_group,
                                    HDRSRCHRFDIV_DISABLE_DIV_CMD );
        /* Send the command to the RF Diversity State Machine */
    }
  }

  HDR_MSG_SRCH_3(MSG_LEGACY_MED, "div %d ramp %d rel %d",
                 div_flag, 
                 hdrsrchrfdiv.tune_away_state, 
                 hdrsrchrfdiv.rel_pend);

} /* hdrsrchrfdiv_ard_diversity_ctrl() */
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

#ifdef FEATURE_HDR_QTA
/*===========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_DISABLE_FOR_QTA

DESCRIPTION
  Call this function to disble diversity just before QTA start.
  This will set internal QTA flag so that we can skip RF disable for QTA. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_diversity_disable_for_qta( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDR QTA: disable div for QTA!" );

  if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled )
  {
    hdrsrchrfdiv.qta_start = TRUE;
  }

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
    hdrsrchard_event( HDRSRCHARD_EVENT_SUSPEND );
#else
    hdrsrchrfdiv_diversity_ctrl( FALSE );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
    /* Disable diversity before starting QTA */

} /* hdrsrchrfdiv_diversity_disable_for_qta() */

/*===========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_DISABLE_AFTER_QTA

DESCRIPTION
  Call this function to disable diversity after QTA.
  This function is needed if we decide to not go to traffic after QTA.
  For example -: 4a->3a->3f, or 4a->4c

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_diversity_disable_after_qta( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if ( hdrsrchrfdiv.qta_start == TRUE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "HDR QTA disabling div chain after QTA " );

    hdrsrchrfdiv.qta_start = FALSE;

    hdrsrchrf_set_qta_client_state( HDRSRCHRF_QTA_END, NULL );
    /* Let TRM know QTA has ended */
   
    hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled = TRUE;
      /* In QTA diversity disable this flag is reset and hence for non-QTA disable
         set the flag to proecess legacy diversity disable */
  }

  if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled == TRUE )
  {
    if ( hdrsrch_get_current_state() == HDRSRCH_REACQ_STATE )
    {
#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
      hdrsrchard_event( HDRSRCHARD_EVENT_EXIT_TC );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "HDR QTA disabling diversity STM after QTA " );
      hdrsrchrfdiv_diversity_ctrl( FALSE );
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "HDR QTA disabled diversity STM after QTA " );
      /*  Release diversity if it is enabled. This will put diversity STM inactive */
    }
  }

} /* hdrsrchrfdiv_diversity_disable_after_qta() */

/*===========================================================================

FUNCTION HDRSRCHRFDIV_IS_DIVERSITY_IN_QTA

DESCRIPTION
  Call this function to check if diversity was enabled when QTA started. 

DEPENDENCIES
  None

RETURN VALUE
  TRUE: div was enabled before QTA
  FALSE: div was diabled before QTA

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchrfdiv_is_diversity_in_qta ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
 return hdrsrchrfdiv.qta_start;

} /* hdrsrchrfdiv_is_diversity_in_qta() */

#endif /* FEATURE_HDR_QTA */

#ifdef FEATURE_HDR_RUMI
/*===========================================================================

FUNCTION HDRSRCHRFDIV_ENABLE_DIVERSITY_CHAIN

DESCRIPTION
  Call this function to enable diversity RF chain. This is a wrapper function for 
  hdrsrchrfdiv_enable_div_chain()

DEPENDENCIES
  None

RETURN VALUE
 
SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrsrchrfdiv_enable_diversity_chain( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrsrchrfdiv_enable_div_chain();

} /* hdrsrchrfdiv_enable_diversity_chain */

#endif /* FEATURE_HDR_RUMI */

/*===========================================================================

FUNCTION HDRSRCHRFDIV_WAIT_DONE_TF

DESCRIPTION
  This function handles wait_to_acquire timer timeout and kick off
  the DIV request process

DEPENDENCIES
  None

RETURN VALUE
 
SIDE EFFECTS
  None

===========================================================================*/
static stm_state_type hdrsrchrfdiv_wait_done_tf
(
  void                        *payload
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  NOTUSED( payload );

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Wait to acq done" );

  return HDRSRCHRFDIV_ACQUIRING_RF_STATE;
}

/*=========================================================================

FUNCTION HDRSRCHRFDIV_ENTER_WAIT_TO_ACQUIRE

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchrfdiv_enter_wait_to_acquire
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{
  HDR_MSG_SRCH( MSG_LEGACY_MED, "hdrsrchrfdiv_enter_wait_to_acq" );

  timer_set( &hdrsrchrfdiv.wait_to_acq_timer, 
              HDRSRCHRFDIV_REL_TO_REQ_WAIT_MS, 
              0, T_MSEC );
}

/*=========================================================================

FUNCTION HDRSRCHRFDIV_REDO_WARMUP_TF

DESCRIPTION 
  Called when the diversity state machine is done warming up diversity.
  Diversity is enabled.

DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHRFDIV_ENABLED_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchrfdiv_redo_warmup_tf
( 
  void                       *payload
    /* Payload */
)
{
  uint32 div_enable_delay = HDRSRCHRFDIV_DIV_ENABLE_DELAY_INVALID;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  NOTUSED( payload );

  if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].owns_chain &&
       ( !hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled ) )
  {
    div_enable_delay = hdrsrchrfdiv_do_warm_up( );

    if ( div_enable_delay != HDRSRCHRFDIV_DIV_ENABLE_DELAY_INVALID )
    {
      hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled = TRUE;
        /* Diversity is enabled. */

      timer_set( &hdrsrchrfdiv.warmup_timer, div_enable_delay, 0, T_MSEC );
        /* Set the timer to go off when the warmup period has expired. */
    }
  }

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
                  "Redo warmup rcvd, own:%d enabled:%d delay:%d", 
                  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].owns_chain, 
                  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled, 
                  div_enable_delay );

  return STM_SAME_STATE;

} /* hdrsrchrfdiv_reenable_div_tf */

/*=========================================================================

FUNCTION HDRSRCHRFDIV_CHK_AND_REDO_WARMUP

DESCRIPTION 
  Check if DIV is owned by not enabled. If so, requeue an redo_warmup_cmd.
  This is needed to take care of DIV being granted before PRI.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchrfdiv_chk_and_redo_warmup( void )
{
  if ( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].owns_chain &&
       ( !hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled ) )
  {
    HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHRFDIV_SM, 
                                  &hdrsrchcom_stm_group, 
                                  HDRSRCHRFDIV_REDO_WARMUP_CMD );
  }
}
