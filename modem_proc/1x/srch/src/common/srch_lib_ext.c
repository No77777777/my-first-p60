
/*====*====*====*====*====*====*====*========*====*====*====*====*====*====*==*

        S E A R C H   T A S K  --
        S E A R C H   L I B R A R Y   E X T E R N A L

GENERAL DESCRIPTION
  This file contains library interfaces from non-librarized searcher code
  into the search library.

EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

  Copyright (c) 2004 - 2015 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.


*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/srch/src/common/srch_lib_ext.c#1 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
12/03/15   ab      Increase safewin threshold by 1ms
10/27/15   pk      Cut RF_EXEC_RF_TIME_DIV_OFFSET_US by 50%
07/06/15   bb      Changes to increaser SLEEP_PROCESSING_DELAY_US by 5ms
04/17/15   srk     IDLE TL rebucketing
04/08/15   srk     Add TTL control algorithm in SW.
02/06/15   srk     Add additional warmup slacks when LTE is in traffic.
01/24/14   as      Fix BOLT timeline profiling points
01/15/14   as      BOLT Timeline optimizations
11/19/13   bb      QPCH Online bring up changes
09/23/13   bb      Changes to split DSDS overheads between pre and post
                   RF tune processing
08/28/13   cjb     Mainline FEATURE_MODEM_1X_DIME
08/19/13   cjb     Mainline FEATURE_1X_SRCH_ROT_ONLY_AFC and
                   FEATURE_1X_SRCH_AFC_SRL ( Removed VCTCXO support )
06/17/13   as      Optimize the sleep timelines for DIME.
05/09/13   cjb     Changes to export the antenna tuner periodicity macro.
04/24/13   dkb     Increase MCPM clk prep time to 2ms.
04/23/13   ab      Updated the Default RDDS Params
02/13/13   as      Change timeline calculations to use units of microseconds
02/05/13   as      Optimized the sleep timelines
01/02/13   ab      Added Enhanced RDDS Support
10/15/12   dkb     Change warmup defaults to use RF API values
09/24/12   dkb     Update 1x Internal Overhead Values to properly account for
                   the shift in RF times between Prep and Exec
07/04/12   adw     Modify reacq time to improve performance in poor geometries.
06/20/12   adw     Separate is2000 and qpch reacq overheads.
03/26/12   sst     Add srch_lib_def ovrd of JCDMA API for qpch ch est thresh
02/13/12   adw     Optimize timelines and modernize overhead defines.
01/23/12   srk     Feature Cleanup
01/18/12   adw     Bump SW safewin back out by 6ms.
01/13/12   srk     Feature Cleanup: Mainlined Always ON features
01/13/12   vks     Add support to request system exit on reacq fail.
01/03/12   bb      Increase SW safewin by 3ms
12/22/11   vks     Increase SW safewin.
12/22/11   bb/vks  Reduce SW safewin time by 3ms
12/22/11   adw     Reduce SW safewin.
10/20/11   ppr     Included "srch_diag.h" header for
                   "srch_sect_pilot_info_image_type" str declaration
09/19/11   vks     Increase SW safewin to account for DCVS latency.
08/30/11   adw     Increase SW safewin to account for power collapse latency.
04/13/11   vks     Moved the srch task pri variables to srchzz_tl_common.c as
                   these are not being used anywhere outside idle module.
                   Added support for new task priority api.
01/03/11   bb      Reduce the base RF warm-up time by 6ms and QLIC warm-up
                   latency by 1ms
12/11/10   vks     Reduce SW latency by 4ms since mdsp is being resumed
                   instead of enable on slotted wakeup.
12/09/10   vks     Reduce SW latency by 2ms since npa calls are now being
                   featurized out in slotted timelines.
12/09/10   vks     Remove mdsp_app_type from the common defines as it is not
                   being used anywhere.
11/24/10   cjb     Increase SW latency to 10ms to account for 1x demod FW
                   enable instead of resume.
11/24/10   cjb     Adding QLIC delay of 4ms to SW latency iff QLIC is enabled.
11/09/10   sst     Add ofs_thresh
11/08/10   cjb     Changed SW latency time from 4ms to 8ms.
11/01/10   jtm     Modem 1x SU API changes.
09/08/10   vks     Move RF warmup constants to srch_lib_ext.c
07/15/10   sst     Add in DSDS overhead time to rf warmup/sleep time calcs
04/28/10   vks     Featurize mdsp header file includes (these are not required
                   for Q6 interface).
02/18/10   sst     Added TC AGC check flags and thresholds
10/07/09   sst     Added method to compare structure sizes inside and outside
                   of the library
09/01/09   vks     Added PCH->QPCH channel estimator threshold type
06/16/09   adw     Set afc vco gain to "X5_SRL_X2" instead of "NORMAL".
04/02/09   adw     Categorized included header files.
03/26/09   adw     Remove T_QSC60X5 featurization, replace T_MSM8650B with
                   FEATURE_MODEM_1X_ARTEMIS feature.
03/25/09   adw     Include modem_1x_defs.h for 1x specific modem defines.
01/30/09   adw     Removed obsolete tcxo related warmup times and temporary
                   MIN_SLEEP_SETUP_TIME_SAFEWIN_MS define.
09/03/08   tjc     Correct the RDDS capacity thresholds
07/28/08   aps     T_MSM8650B bringup changes
06/25/08   adw     Added min_sleep_setup_time_safewin_ms
06/20/08   mca     Merge from //depot
03/13/08   sst     Modify FTS version to be determined at runtime
01/11/08   aps     Add RTR6500_WARMUP_OVERHEAD outside of srch library
10/29/07   sst     Add (en/dis)able of GOVERN_ACQ_FAIL outside of srch library
                   Unfeaturize all code GOVERN_ACQ_FAIL code except init
06/27/07   aps     Moved cgps_sw_overhead_* featurization to custsrch.h
06/21/07   aps     Featurize cgps_sw_overhead_* for T_QSC60X5
06/21/07   sst     Added cgps_sw_overhead_*
06/18/07   mca     Removed use of nv items defines from library
04/23/07   tjc     Changed VCO gain and SRL multiplier for T_QSC60X5.
02/21/07   aps     Added VCO gain and SRL multiplier knob.
11/08/06   tjc     Moved tunable RX diversity autoswitching parameters int
                    srch_lib_ext.c/h
02/17/05   sst     Moved reference to MDSP_APP_SRCH into srch_lib_ext.c/h to
                    facilite custumer compiles will differing Feature set
01/25/05   kwo     Exported some sched parms outside of library
12/28/04   bt      implementation, first cut

=============================================================================*/


/*=============================================================================

                        INCLUDE FILES FOR MODULE

=============================================================================*/

/* Common */
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

/* Srch */
#include "srch_chan_t.h"
#include "srch_hw_t.h"
#include "srch_hw_t_i.h"
#include "srch_lib_ext.h"
#include "srch_sect.h"
#include "srch_diag.h"

/* Other */
#include "rex.h"
#include "task.h"


/*===========================================================================
             LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE
===========================================================================*/

/*-------------------------------------------------------------------------
      Typedefs
-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------
      Constants and Macros
-------------------------------------------------------------------------*/

/* ------------------------------------------------------------------------
** Wakeup, Warmup, and Overhead Constants
** ------------------------------------------------------------------------ */

/* Wakeup overhead times are defined below. Wakeup overheads are classified
   into three major components - 1) RF prep and 2) RF exec. Also
   specified below are a handful of miscellaneous overhead values used
   by the timelines. */

/* RF prep overhead times:
       - rf prep (querried via RF warmup API)
       - clk
       - fw
       - sw overheads
*/
#define RF_PREP_RF_TIME_US                     200   /* default if RF API not used */
#define RF_PREP_RF_TIME_NON_MAX_CLK_OFFSET_US  300
#define RF_PREP_RF_TIME_LTE_OFFSET_US          400
#define RF_PREP_CLK_OVERHEAD_US                500
#define RF_PREP_CLK_OVERHEAD_LTE_OFFSET_US     350
#define RF_PREP_FW_OVERHEAD_US                 700
#define RF_PREP_FW_OVERHEAD_LTE_OFFSET_US      250
#define RF_PREP_SW_OVERHEAD_US                3000
#define RF_PREP_SW_OFFTL_OVERHEAD_US           850
#define RF_PREP_SW_OVERHEAD_CGPS_OFFSET_US       0
#define RF_PREP_SW_OVERHEAD_DSDS_OFFSET_US       0
#define RF_PREP_SW_OVERHEAD_LTE_OFFSET_US     1000

/* RF exec overhead times:
       - rf exec (querried via RF warmup API)
       - reacq time
       - finger assign and ramp overhead
       - sw overheads
 */
#define RF_EXEC_RF_TIME_US                    4500   /* default if RF API not used */
#define RF_EXEC_RF_TIME_NON_MAX_CLK_OFFSET_US 2500
#define RF_EXEC_RF_TIME_DIV_OFFSET_US         2500
#define REACQ_TIME_OVERHEAD_IS2000_US         4000
#define REACQ_TIME_OVERHEAD_QPCH_ONTL_US      2000
#define FINGA_TIME_OVERHEAD_US                5000
#define RF_EXEC_SW_OVERHEAD_US                2500
#define RF_EXEC_SW_OFFTL_OVERHEAD_US           150
#define RF_EXEC_SW_OVERHEAD_DSDS_OFFSET_US       0
#define RF_EXEC_SW_OVERHEAD_LTE_OFFSET_US     2000

/* Misc warmup overhead times...
       - sleep setup overhead time
       - sleep processing delay
*/
#define MIN_SLEEP_SETUP_TIME_SAFEWIN_US       3000
#define SLEEP_PROCESSING_DELAY_US            15000

/* TTL Algorithm enable/disable threaholds */
#define AGC_EN_TH_DB                            20 /* In dB        */
#define AGC_DIS_TH_DB                           15 /* In dB        */
#define ECIO_EN_TH_Q15                        1036 /*  5 dB in Q15 */
#define ECIO_DIS_TH_Q15                       3277 /* 10 dB in Q15 */

/* Searcher library external defines */
const srch_lib_ext_defines_type srch_lib_defs =
{
  /* common defs */
  {
    /* rx diversity autoswitching defs */
    {
      7510000,   /* RC3 Tbase : -6.50dB in linear Q25 */
      3760000,   /* RC4 Tbase : -9.50dB in linear Q25 */
      6,         /* Fq      : Time const 1.27s */
      (1<<11),   /* Tq      : 0.125 in Q14 */
      500,       /* Dq      : 500=10sec */
      32,        /* FPCAdj  : 32=2dB in Q4 */
      #ifdef FEATURE_ENHANCED_RDDS
      48,        /* sp_hist_delta_dB3 */
      32,        /* sp_low_delta_dB3  */
      12,        /* sp_high_delta_dB3 */
      1,         /* qof_pilots_thresh_num */
      1,         /* qof_sets_thresh_num */
      4,         /* quality_good_frame_thr_num */
      3          /* quality_bad_frame_thr_num */
      #endif  /* FEATURE_ENHANCED_RDDS */
    },

    TRUE,            /* rx_div_agc_ecio_ck        */
    AGC_DIS_TH_DB,   /* rx_div_agc_dis_delta_db   */
    AGC_EN_TH_DB,    /* rx_div_agc_en_delta_db    */
    ECIO_DIS_TH_Q15, /* rx_div_ecio_dis_delta_q15 */
    ECIO_EN_TH_Q15,  /* rx_div_ecio_en_delta_q15  */

    #ifdef FEATURE_FULL_TIME_SHDR_3
    SRCH_USE_FTS_3,      /* fts_ver */
    #else
    SRCH_USE_FTS_NONE,   /* fts_ver */
    #endif

    200                     /* rx_rf_antenna_tuner_time_ms */
  },
  /* init defs */
  {
    #ifdef FEATURE_SRCH_GOVERN_ACQ_FAIL
    5,     /* CD governor time       */
    TRUE,  /* CD governor enabled    */
    5,     /* ACQ governor time      */
    TRUE,  /* ACQ governor enabled   */
    #else
    0,     /* CD governor time       */
    FALSE, /* CD governor enabled    */
    0,     /* ACQ governor time      */
    FALSE, /* ACQ governor enabled   */
    #endif
  },
  /* idle defs */
  {
    /* warmup defs */
    {
      /* RF prep overhead times */
      RF_PREP_RF_TIME_US,
      RF_PREP_RF_TIME_NON_MAX_CLK_OFFSET_US,
      RF_PREP_RF_TIME_LTE_OFFSET_US,
      RF_PREP_CLK_OVERHEAD_US,
      RF_PREP_CLK_OVERHEAD_LTE_OFFSET_US,
      RF_PREP_FW_OVERHEAD_US,
      RF_PREP_FW_OVERHEAD_LTE_OFFSET_US,
      RF_PREP_SW_OVERHEAD_US,
      RF_PREP_SW_OFFTL_OVERHEAD_US,
      RF_PREP_SW_OVERHEAD_CGPS_OFFSET_US,
      RF_PREP_SW_OVERHEAD_DSDS_OFFSET_US,
      RF_PREP_SW_OVERHEAD_LTE_OFFSET_US,

      /* RF exec overhead times */
      RF_EXEC_RF_TIME_US,
      RF_EXEC_RF_TIME_NON_MAX_CLK_OFFSET_US,
      RF_EXEC_RF_TIME_DIV_OFFSET_US,
      REACQ_TIME_OVERHEAD_IS2000_US,
      REACQ_TIME_OVERHEAD_QPCH_ONTL_US,
      FINGA_TIME_OVERHEAD_US,
      RF_EXEC_SW_OVERHEAD_US,
      RF_EXEC_SW_OFFTL_OVERHEAD_US,
      RF_EXEC_SW_OVERHEAD_DSDS_OFFSET_US,
      RF_EXEC_SW_OVERHEAD_LTE_OFFSET_US,

      /* Misc overhead times */
      MIN_SLEEP_SETUP_TIME_SAFEWIN_US,
      SLEEP_PROCESSING_DELAY_US,

      /* Use RF warmup API - if set, RF API overrides RF prep and exec values */
      TRUE
    },

    /* sched defs */
    {
      13056, /* REACQ_WIN_NOISE_THRESH_LECIO */
      3,     /* FAST_RAHO_SRCH_PENALTY */
      6,     /* FAST_RAHO_REG_PENALTY */
      200,   /* FAILED_REACQ_SCAN_TIME_MS */
      #ifdef FEATURE_JCDMA
      SRCH_USE_OFS_THRESH_1,      /* OFS_THRESH */
      #else
      SRCH_USE_OFS_THRESH_LEGACY, /* OFS_THRESH */
      #endif
      #ifdef FEATURE_JCDMA
      SRCH_SYSTEM_EXIT_ON_REACQ_FAIL
      #else
      SRCH_NO_SYSTEM_EXIT_ON_REACQ_FAIL
      #endif
    },

    /* qpch defs */
    {
      /* see comment in srch_lib_ext.h for QPCH defines */
      #ifdef FEATURE_JCDMA
      THRESH_TYPE_OLD      /* ch_est_thresh_tab_type */
      #else
      THRESH_TYPE_NEW      /* ch_est_thresh_tab_type */
      #endif /* FEATURE_JCDMA */
    }
  },
  /* nv_items defs */
  {
    NV_CDMA_RX_DIVERSITY_CTRL_I,
    NV_CDMA_RX_CHAIN_SEL_THRESH_I,
    NV_SRCH_DBG_MASK_I,
    NV_CDMA_RX_DIV_BC_ENABLE_MASK_I
  }
};

/*=============================================================================

FUNCTION SRCH_LIB_EXT_CHECK_STRUCTS

DESCRIPTION    This functions is used to compare the sizes of srch structures
               inside and outside of the srch library.

DEPENDENCIES   This function must match srch_lib_int_check_structs which is
               inside the library

RETURN VALUE   Size of structures used inside and outside the srch library

SIDE EFFECTS   None

=============================================================================*/
uint32 srch_lib_ext_check_structs( void )
{
  uint32                          ext_size;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ext_size =
    /* from srch_chan_t.h */
    sizeof( srch_chan_type ) +
    /* from srch_hw_t.h */
    sizeof( srch_sect_struct_type ) +
    sizeof( srch_sect_parm_struct_type ) +
    /* from srch_lib_ext.h */
    sizeof( srch_lib_ext_defines_type ) +
    /* from srch_sect.h */
    sizeof( srch_sect_r_parms_type ) +
    sizeof( srch_sect_pilot_info_image_type );

  return( ext_size );

} /* srch_lib_ext_check_structs */


