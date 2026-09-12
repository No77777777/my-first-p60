/*
==============================================================================

FILE:         ClockBSP.c

DESCRIPTION:
  This file contains the clock driver BSP data.

  Auto-generated from the following IPCat controller releases:

    SM4250v1 (kamorta_1.0):
      PLL_HSR   : None
      GCC       : kamorta_gcc_Z_kamorta_clk_ctl_P3Q3R32.3301_fp5 [kamorta_gcc.r6]
      MSS_CC    : habanero_1.0_p3q3r17 [kamorta_mss_cc.r2]
      NAV_CC    : nav_ss_kamorta_1.0_p3q2r1.2 [kamorta_nav_cc.r2]


  Generated using the following clockdrivergen settings:

CLOCKDRIVERGEN_CONFIG = \
{'chip_aliases': ['kamorta_1.0'],
 'db_override': {'MSS_CC': {'cbc': {'clk_q6': {'l_master': ['q6_core_clk_src']},
                                    'clk_silver': {'d_registers': {'cbcr_name': 'MSS_QDSP6SS_VPE_VU_CBCR'},
                                                   'l_master': ['mss_cc_silver_clk_src']},
                                    'mss_cc_mpll1_out_early_div2_clk': {'d_registers': {'cbcr_name': 'MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR'}},
                                    'mss_cc_mpll1_out_early_div3_clk': {'d_registers': {'cbcr_name': 'MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR'}},
                                    'mss_cc_mpll1_out_early_div5_clk': {'d_registers': {'cbcr_name': 'MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR'}}},
                            'pll': {'mpll0': {'d_registers': {'mode_name': 'MODEM_PLL_MODE'}},
                                    'mpll1': {'d_registers': {'mode_name': 'MSS_CC_MSS_MPLL1_MODE'}},
                                    'mpll2': {'d_registers': {'mode_name': 'MSS_QDSP6SS_PLL_MODE'}}},
                            'rcg': {'mss_cc_silver_clk_src': {'d_registers': {'cfg_rcgr_name': 'MSS_CC_SILVER_CFG_RCGR',
                                                                              'cmd_rcgr_name': 'MSS_CC_SILVER_CMD_RCGR'}},
                                    'q6_core_clk_src': {'d_registers': {'cfg_rcgr_name': 'MSS_QDSP6SS_CORE_CFG_RCGR',
                                                                        'cmd_rcgr_name': 'MSS_QDSP6SS_CORE_CMD_RCGR'}}}}},
 'debug_mux_override': {'GCC': {'div': 4},
                        'MSS_CC_Q6': {'inputs': {'clk_q6': 1}}},
 'filter_warning': 'sw',
 'generators': [{'options': {'cc_clock_flags': {},
                             'clock_flags': {'gcc_wcss_ahb_s0_clk': ['SUPPRESSIBLE'],
                                             'gpll0': ['READ_ONLY'],
                                             'gpll6': ['READ_ONLY'],
                                             'mss_cc_bus_pre_mux_clk_src': ['SUPPRESSIBLE'],
                                             'mss_cc_modem_bus_clk_src': ['SUPPRESSIBLE'],
                                             'q6_core_clk_src': ['SUPPRESSIBLE']},
                             'driver_flags': ['LOG_CLOCK_FREQ_CHANGE',
                                              'LOG_SOURCE_STATE_CHANGE'],
                             'env': 'mss',
                             'hal_ctl_structs': {'mpll2': 'HAL_clk_MSSQ6PLLControl',
                                                 'mpll3': 'HAL_clk_MSSQ6PLLControl',
                                                 'mss_cc_silver_clk_src': 'HAL_clk_MSSSilverDomainControl',
                                                 'nav_ss_gdsc': 'HAL_clk_NavPowerDomainControl'},
                             'include_HALclkPLLSettings.h': True,
                             'npa_resources': {'/clk/bimc': {'init_state': 'NPA_MAX_STATE'},
                                               '/clk/mss/config_bus': {'clock': 'mss_cc_bridge_bus_clk',
                                                                       'init_state': 72000},
                                               '/xo/cxo': {'init_state': 1}},
                             'pll_config_flags': {'mpll0': ['EARLY_OUTPUT_ENABLE'],
                                                  'mpll1': ['EARLY_OUTPUT_ENABLE',
                                                            'AUX_OUTPUT_ENABLE'],
                                                  'mpll2': ['EARLY_OUTPUT_ENABLE'],
                                                  'mpll3': ['EARLY_OUTPUT_ENABLE']},
                             'pll_disable_mode': {'mpll0': 'NORMAL',
                                                  'mpll1': 'NORMAL',
                                                  'mpll3': 'NORMAL'}},
                 'output_dir': '..',
                 'type': 'dalclock'}],
 'include_HALclkPLLSettings.h': True,
 'target': ['kamorta']}

==============================================================================

$Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/kamorta/ClockBSP.c#14 $

==============================================================================
            Copyright (c) 2019 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
==============================================================================

*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "ClockBSP.h"
#include "HALclkHWIO.h"
#include "HALhwio.h"


/*=========================================================================
      Externs
==========================================================================*/

extern HAL_clk_ClockControlType           HAL_clk_GenericClockControl;
extern HAL_clk_ClockDomainControlType     HAL_clk_GenericDomainControl;
extern HAL_clk_ClockDomainControlType     HAL_clk_GenericDomainMuxControl;
extern HAL_clk_DividerControlType         HAL_clk_GenericDividerControl;
extern HAL_clk_PowerDomainControlType     HAL_clk_GenericPowerDomainControl;
extern HAL_clk_SourceControlType          HAL_clk_BrammoPLLControl;
extern HAL_clk_SourceControlType          HAL_clk_SparkPLLControl;
extern HAL_clk_SourceControlType          HAL_clk_SparkModemPLLControl;

/*
 * Debug mux control structures.
 */
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlGCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlMSSCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlMSSCCQ6;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlNAVCC;

/*
 * Custom control structures.
 */
extern HAL_clk_ClockControlType           HAL_clk_MSSSilverClockControl;
extern HAL_clk_PowerDomainControlType     HAL_clk_NavPowerDomainControl;
extern HAL_clk_SourceControlType          HAL_clk_MSSQ6PLLControl;
extern HAL_clk_SourceControlType          HAL_clk_MSSSilverPLLControl;

/*
 * NPA node driver functions.
 */
extern npa_resource_state   Clock_GenericNPANodeFunc(npa_resource*, npa_client_handle, npa_resource_state);


/*=========================================================================
      Data Declarations
==========================================================================*/


/*=========================================================================
      Voltage Rails
==========================================================================*/

static ClockRailType ClockRail_VDDCX =
{
  .szName = "/vcs/vdd_cx",
};

static ClockRailType ClockRail_VDDMX =
{
  .szName = "/vcs/vdd_mx",
};

static ClockRailType *aRails[] =
{
  &ClockRail_VDDCX,
  &ClockRail_VDDMX,
};

/*=========================================================================
      Debug Muxes
==========================================================================*/

static ClockDebugMuxType ClockDebugMux_GCC =
{
  .szName       = "gcc",
  .nDivider     = 4,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlGCC,
  },
};

static ClockDebugMuxType ClockDebugMux_MSSCC =
{
  .szName       = "mss_cc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 213,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlMSSCC,
  },
};

static ClockDebugMuxType ClockDebugMux_NAVCC =
{
  .szName       = "nav_cc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 240,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlNAVCC,
  },
};

static ClockDebugMuxType ClockDebugMux_MSSCCQ6 =
{
  .szName       = "mss_cc_q6",
  .pParent      = &ClockDebugMux_MSSCC,
  .nParentSel   = 43,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlMSSCCQ6,
  },
};

static ClockDebugMuxType *aDebugMuxes[] =
{
  &ClockDebugMux_GCC,
  &ClockDebugMux_MSSCC,
  &ClockDebugMux_MSSCCQ6,
  &ClockDebugMux_NAVCC,
};


/*=========================================================================
      Primary Sources
==========================================================================*/

static ClockSourceNodeType ClockSource_XO =
{
  .szName    = "/xo/cxo",
  .nFlags    = 0x0,
};


/*=========================================================================
      Source Register Settings
==========================================================================*/

static const ClockSourceRegSettingsType ClockSourceRegSettings_MSSCC_MPLL0[] =
{
  {
    .HALSettings = {
      .nConfigCtl   = 0x4001055B,
      .nTestCtl     = 0x00000000,
      .nTestCtl1    = 0x00000001,
      .nUserCtl     = 0x00000000,
      .nUserCtl1    = 0x00000004,
    },
  },
};

static const ClockSourceRegSettingsType ClockSourceRegSettings_MSSCC_MPLL1[] =
{
  {
    .HALSettings = {
      .nConfigCtl   = 0x00004289,
      .nTestCtl     = 0x08000000,
      .nUserCtl     = 0x00000000,
    },
  },
};


/*=========================================================================
      Source BSP
==========================================================================*/

/*
 * ClockSourceBSP_GCC_GPLL0
 *
 * Used by:
 */
static const ClockSourceFreqConfigType ClockSourceBSP_GCC_GPLL0[] =
{
  {  600000000, &ClockSource_XO, { 0, 2, 1, 31,  0x4000000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Spark.nMain = 2}           }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
};

/*
 * ClockSourceBSP_GCC_GPLL6
 *
 * Used by:
 */
static const ClockSourceFreqConfigType ClockSourceBSP_GCC_GPLL6[] =
{
  {  768000000, &ClockSource_XO, { 0, 2, 1, 40,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Spark.nMain = 2}           }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
};

/*
 * ClockSourceBSP_MSSCC_MPLL0
 *
 * Used by:
 *   ClockSource_MSSCC_MPLL0
 */
static const ClockSourceFreqConfigType ClockSourceBSP_MSSCC_MPLL0[] =
{
  {  633600000, &ClockSource_XO, { 0, 2, 1, 33,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
  {  672000000, &ClockSource_XO, { 0, 2, 1, 35,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
  {  768000000, &ClockSource_XO, { 0, 2, 1, 40,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
  {  825600000, &ClockSource_XO, { 0, 2, 1, 43,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
  {  921600000, &ClockSource_XO, { 0, 2, 1, 48,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
  {  940800000, &ClockSource_XO, { 0, 2, 1, 49,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
  {  998400000, &ClockSource_XO, { 0, 2, 1, 52,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
};
static const ClockSourceFreqConfigType ClockSourceBSPCal_MSSCC_MPLL0[] =
{
  {  806400000, &ClockSource_XO, { 0, 2, 1, 42,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
};

/*
 * ClockSourceBSP_MSSCC_MPLL1
 *
 * Used by:
 *   ClockSource_MSSCC_MPLL1
 */
static const ClockSourceFreqConfigType ClockSourceBSP_MSSCC_MPLL1[] =
{
  { 1152000000, &ClockSource_XO, { 0, 0, 1, 60,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Brammo.nMain = 2}          }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
};

/*
 * ClockSourceBSP_MSSCC_MPLL2
 *
 * Used by:
 *   ClockSource_MSSCC_MPLL2
 */
static const ClockSourceFreqConfigType ClockSourceBSP_MSSCC_MPLL2[] =
{
  {  806400000, &ClockSource_XO, { 0, 1, 1, 42,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Spark.nMain = 2}           }, RAIL_VOLTAGE_LEVEL_NOM,       },
  {  864000000, &ClockSource_XO, { 0, 1, 1, 45,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_NOM,       },
  {  960000000, &ClockSource_XO, { 0, 1, 1, 50,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_NOM,       },
  { 1056000000, &ClockSource_XO, { 0, 1, 1, 55,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_NOM,       },
  { 1190400000, &ClockSource_XO, { 0, 1, 1, 62,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Spark.nMain = 2}           }, RAIL_VOLTAGE_LEVEL_NOM,   },
  { 1190400000, &ClockSource_XO, { 0, 1, 1, 62,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_NOM,       },
  { 1459200000, &ClockSource_XO, { 0, 1, 1, 76,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Spark.nMain = 2}           }, RAIL_VOLTAGE_LEVEL_NOM,   },
};
static const ClockSourceFreqConfigType ClockSourceBSPCal_MSSCC_MPLL2[] =
{
  { 1132800000, &ClockSource_XO, { 0, 1, 1, 59,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_NOM,       },
};

/*
 * ClockSourceBSP_MSSCC_MPLL3
 *
 * Used by:
 *   ClockSource_MSSCC_MPLL3
 */
static const ClockSourceFreqConfigType ClockSourceBSP_MSSCC_MPLL3[] =
{
  {  595200000, &ClockSource_XO, { 0, 2, 1, 31,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
  {  729600000, &ClockSource_XO, { 0, 2, 1, 38,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
  {  806400000, &ClockSource_XO, { 0, 2, 1, 42,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Spark.nMain = 2}           }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
  {  864000000, &ClockSource_XO, { 0, 2, 1, 45,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
  {  921600000, &ClockSource_XO, { 0, 2, 1, 48,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
  {  998400000, &ClockSource_XO, { 0, 2, 1, 52,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
};
static const ClockSourceFreqConfigType ClockSourceBSPCal_MSSCC_MPLL3[] =
{
  {  787200000, &ClockSource_XO, { 0, 2, 1, 41,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_LOW_SVS,   },
};


/*=========================================================================
      Sources
==========================================================================*/

static ClockSourceNodeType ClockSource_GCC_GPLL0 =
{
  .szName       = "gpll0",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL |
                  CLOCK_FLAG_READ_ONLY,
  .aBSP         = ClockSourceBSP_GCC_GPLL0,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL0),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_AUX2_OUTPUT_ENABLE |
                  CLOCK_CONFIG_PLL_FSM_MODE_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_STANDBY,
  .HALSource = {
    .nAddr          = HWIO_ADDR(GCC_GPLL0_MODE),
    .VoterRegister  = {HWIO_ADDR(GCC_PROC_CLK_PLL_ENA_VOTE), 0x1},
    .pmControl      = &HAL_clk_SparkPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSP_GCC_GPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL0),
};

static ClockSourceNodeType ClockSource_GCC_GPLL6 =
{
  .szName       = "gpll6",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL |
                  CLOCK_FLAG_READ_ONLY,
  .aBSP         = ClockSourceBSP_GCC_GPLL6,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL6),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_FSM_MODE_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_STANDBY,
  .HALSource = {
    .nAddr          = HWIO_ADDR(GCC_GPLL6_MODE),
    .VoterRegister  = {HWIO_ADDR(GCC_PROC_CLK_PLL_ENA_VOTE), 0x40},
    .pmControl      = &HAL_clk_SparkPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSP_GCC_GPLL6,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL6),
};

static ClockSourceNodeType ClockSource_MSSCC_MPLL0 =
{
  .szName       = "mpll0",
  .nFlags       = 0x0,
  .aBSP         = ClockSourceBSP_MSSCC_MPLL0,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_MSSCC_MPLL0),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EARLY_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MODEM_PLL_MODE),
    .pmControl      = &HAL_clk_SparkModemPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_MSSCC_MPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_MSSCC_MPLL0),
  .pRegSettings               = ClockSourceRegSettings_MSSCC_MPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_MSSCC_MPLL0),
};

static ClockSourceNodeType ClockSource_MSSCC_MPLL1 =
{
  .szName       = "mpll1",
  .nFlags       = 0x0,
  .aBSP         = ClockSourceBSP_MSSCC_MPLL1,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_MSSCC_MPLL1),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_AUX_OUTPUT_ENABLE |
                  CLOCK_CONFIG_PLL_EARLY_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_CC_MSS_MPLL1_MODE),
    .pmControl      = &HAL_clk_BrammoPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSP_MSSCC_MPLL1,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSP_MSSCC_MPLL1),
  .pRegSettings               = ClockSourceRegSettings_MSSCC_MPLL1,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_MSSCC_MPLL1),
};

static ClockSourceNodeType ClockSource_MSSCC_MPLL2 =
{
  .szName       = "mpll2",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_MSSCC_MPLL2,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_MSSCC_MPLL2),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EARLY_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_STANDBY,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_QDSP6SS_PLL_MODE),
    .pmControl      = &HAL_clk_MSSQ6PLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_MSSCC_MPLL2,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_MSSCC_MPLL2),
  .pRegSettings               = ClockSourceRegSettings_MSSCC_MPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_MSSCC_MPLL0),
};

static ClockSourceNodeType ClockSource_MSSCC_MPLL3 =
{
  .szName       = "mpll3",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_MSSCC_MPLL3,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_MSSCC_MPLL3),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EARLY_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_CC_MPLL3_MODE),
    .pmControl      = &HAL_clk_MSSSilverPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_MSSCC_MPLL3,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_MSSCC_MPLL3),
  .pRegSettings               = ClockSourceRegSettings_MSSCC_MPLL0,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_MSSCC_MPLL0),
};


static ClockSourceNodeType *aSources[] =
{
  &ClockSource_GCC_GPLL0,
  &ClockSource_GCC_GPLL6,
  &ClockSource_MSSCC_MPLL0,
  &ClockSource_MSSCC_MPLL1,
  &ClockSource_MSSCC_MPLL2,
  &ClockSource_MSSCC_MPLL3,
  &ClockSource_XO,
};


/*=========================================================================
      Domain BSP
==========================================================================*/

/*
 * ClockDomainBSP_GCC_GCCQUPV3WRAP0S0
 *
 * Note: These frequencies are used by domains possibly under DFS control. The
 *       DFS-enable state will be detected during init. If under DFS control,
 *       the domain will dynamically allocate its own BSP array and populate
 *       it with configurations detected from the DFS registers.
 *
 * Used by:
 *   ClockDomain_GCC_GCCQUPV3WRAP0S0
 *   ClockDomain_GCC_GCCQUPV3WRAP0S1
 *   ClockDomain_GCC_GCCQUPV3WRAP0S2
 *   ClockDomain_GCC_GCCQUPV3WRAP0S3
 *   ClockDomain_GCC_GCCQUPV3WRAP0S4
 *   ClockDomain_GCC_GCCQUPV3WRAP0S5
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCQUPV3WRAP0S0[] =
{
  {    7372800, &ClockSource_GCC_GPLL0,                   { 2,     2,     384,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {   14745600, &ClockSource_GCC_GPLL0,                   { 2,     2,     768,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0x00 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   29491200, &ClockSource_GCC_GPLL0,                   { 2,     2,     1536,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {   32000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     8,     75,    0x01 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {   48000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     4,     25,    0x02 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {   64000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     16,    75,    0x03 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {   75000000, &ClockSource_GCC_GPLL0,                   { 2,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {   80000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     4,     15,    0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {   96000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     8,     25,    0x04 }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 2,     6,     0,     0,     0x05 }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {  102400000, &ClockSource_GCC_GPLL0,                   { 2,     2,     128,   375,   0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {  112000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     28,    75,    0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {  117964800, &ClockSource_GCC_GPLL0,                   { 2,     2,     6144,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {  120000000, &ClockSource_GCC_GPLL0,                   { 2,     5,     0,     0,     0x06 }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[2] = GCC_GPLL0_OUT_AUX2_PWRGRP35_CLKGEN_ACGC_CLK */
  {  128000000, &ClockSource_GCC_GPLL6,                   { 4,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[4] = GCC_GPLL6_OUT_MAIN_PWRGRP2_CLKGEN_ACGC_CLK */
};

/*
 * ClockDomainBSP_GCC_GCCSECCTRL
 *
 * Used by:
 *   ClockDomain_GCC_GCCSECCTRL
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCSECCTRL[] =
{
  {    4800000, &ClockSource_XO,                          { 0,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_MSSCC_CLKMODEMAXI
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKMODEMAXI
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKMODEMAXI[] =
{
  {  168000000, &ClockSource_MSSCC_MPLL0,                 { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 1] /* 672.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  235200000, &ClockSource_MSSCC_MPLL0,                 { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 5] /* 940.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  316800000, &ClockSource_MSSCC_MPLL0,                 { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 0] /* 633.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  384000000, &ClockSource_MSSCC_MPLL0,                 { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 2] /* 768.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  412800000, &ClockSource_MSSCC_MPLL0,                 { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 3] /* 825.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  460800000, &ClockSource_MSSCC_MPLL0,                 { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 4] /* 921.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  499200000, &ClockSource_MSSCC_MPLL0,                 { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 6] /* 998.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
};

/*
 * ClockDomainBSP_MSSCC_CLKMODEMCCS
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKMODEMCCS
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKMODEMCCS[] =
{
  {  192000000, &ClockSource_MSSCC_MPLL0,                 { 1,     7,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 1] /* 672.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  313600000, &ClockSource_MSSCC_MPLL0,                 { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 5] /* 940.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  422400000, &ClockSource_MSSCC_MPLL0,                 { 1,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 0] /* 633.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  512000000, &ClockSource_MSSCC_MPLL0,                 { 1,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 2] /* 768.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  550400000, &ClockSource_MSSCC_MPLL0,                 { 1,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 3] /* 825.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  576000000, &ClockSource_MSSCC_MPLL1,                 { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        },                                                            /* Mux[0] = MPLL1_OUT_MAIN */
};

/*
 * ClockDomainBSP_MSSCC_CLKMODEMMTC
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKMODEMMTC
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKMODEMMTC[] =
{
  {   96000000, &ClockSource_MSSCC_MPLL0,                 { 1,     14,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 1] /* 672.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  188160000, &ClockSource_MSSCC_MPLL0,                 { 1,     10,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 5] /* 940.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  253440000, &ClockSource_MSSCC_MPLL0,                 { 1,     5,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 0] /* 633.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  307200000, &ClockSource_MSSCC_MPLL0,                 { 1,     5,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 2] /* 768.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  330240000, &ClockSource_MSSCC_MPLL0,                 { 1,     5,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 3] /* 825.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  368640000, &ClockSource_MSSCC_MPLL0,                 { 1,     5,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 4] /* 921.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  399360000, &ClockSource_MSSCC_MPLL0,                 { 1,     5,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 6] /* 998.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
};

/*
 * ClockDomainBSP_MSSCC_CLKMODEMTDEC
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKMODEMTDEC
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKMODEMTDEC[] =
{
  {  288000000, &ClockSource_MSSCC_MPLL1,                 { 0,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = MPLL1_OUT_MAIN */
  {  470400000, &ClockSource_MSSCC_MPLL0,                 { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 5] /* 940.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  633600000, &ClockSource_MSSCC_MPLL0,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 0] /* 633.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  768000000, &ClockSource_MSSCC_MPLL0,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 2] /* 768.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  825600000, &ClockSource_MSSCC_MPLL0,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 3] /* 825.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  921600000, &ClockSource_MSSCC_MPLL0,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 4] /* 921.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
  {  998400000, &ClockSource_MSSCC_MPLL0,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL0[ 6] /* 998.0  */ },        /* Mux[1] = MPLL0_OUT_EARLY */
};

/*
 * ClockDomainBSP_MSSCC_MSSCCBITCOXM
 *
 * Used by:
 *   ClockDomain_MSSCC_MSSCCBITCOXM
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_MSSCCBITCOXM[] =
{
  {    1843200, &ClockSource_XO,                 { 0,     2,    12,   125,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = MPLL1_OUT_MAIN */
  {    7372800, &ClockSource_XO,                         { 0,      2,     48,  125,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = MPLL1_OUT_MAIN */
  {   16000000, &ClockSource_MSSCC_MPLL1,                 { 1,     24,    1,     3,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = MPLL1_OUT_MAIN */
  {   48000000, &ClockSource_MSSCC_MPLL1,                 { 1,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = MPLL1_OUT_MAIN */
};

/*
 * ClockDomainBSP_MSSCC_MSSCCBUSPREMUX
 *
 * Used by:
 *   ClockDomain_MSSCC_MSSCCBUSPREMUX
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_MSSCCBUSPREMUX[] =
{
  {   72000000, &ClockSource_MSSCC_MPLL1,                 { 1,     16,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = MPLL1_OUT_MAIN */
  {  144000000, &ClockSource_MSSCC_MPLL1,                 { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = MPLL1_OUT_MAIN */
};

/*
 * ClockDomainBSP_MSSCC_MSSCCMODEMBUSPREMUX
 *
 * Used by:
 *   ClockDomain_MSSCC_MSSCCMODEMBUSPREMUX
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_MSSCCMODEMBUSPREMUX[] =
{
  {  288000000, &ClockSource_MSSCC_MPLL1,                 { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = MPLL1_OUT_MAIN */
  {  384000000, &ClockSource_MSSCC_MPLL1,                 { 1,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = MPLL1_OUT_MAIN */
  {  576000000, &ClockSource_MSSCC_MPLL1,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[1] = MPLL1_OUT_MAIN */
};

/*
 * ClockDomainBSP_MSSCC_MSSCCQ6
 *
 * Used by:
 *   ClockDomain_MSSCC_MSSCCQ6
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_MSSCCQ6[] =
{
  {  384000000, &ClockSource_MSSCC_MPLL1,                 { 1,     3,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = MPLL1_OUT_MAIN */
  {  576000000, &ClockSource_MSSCC_MPLL1,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = MPLL1_OUT_MAIN */
};

/*
 * ClockDomainBSP_MSSCC_MSSCCSILVER
 *
 * Used by:
 *   ClockDomain_MSSCC_MSSCCSILVER
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_MSSCCSILVER[] =
{
  {  403200000, &ClockSource_MSSCC_MPLL3,                 { 5,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_MSSCC_MPLL3[ 2] /* 403.0  */ },        /* Mux[5] = MPLL3_OUT_MAIN */
  {  595200000, &ClockSource_MSSCC_MPLL3,                 { 5,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_MSSCC_MPLL3[ 0] /* 595.0  */ },        /* Mux[5] = MPLL3_OUT_MAIN */
  {  729600000, &ClockSource_MSSCC_MPLL3,                 { 5,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL3[ 1] /* 729.0  */ },        /* Mux[5] = MPLL3_OUT_MAIN */
  {  864000000, &ClockSource_MSSCC_MPLL3,                 { 5,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_MSSCC_MPLL3[ 3] /* 864.0  */ },        /* Mux[5] = MPLL3_OUT_MAIN */
  {  921600000, &ClockSource_MSSCC_MPLL3,                 { 5,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL3[ 4] /* 921.0  */ },        /* Mux[5] = MPLL3_OUT_MAIN */
  {  998400000, &ClockSource_MSSCC_MPLL3,                 { 5,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0}}, &ClockSourceBSP_MSSCC_MPLL3[ 5] /* 998.0  */ },        /* Mux[5] = MPLL3_OUT_MAIN */
};

/*
 * ClockDomainBSP_MSSCC_MSSCCUIM0MND
 *
 * Used by:
 *   ClockDomain_MSSCC_MSSCCUIM0MND
 *   ClockDomain_MSSCC_MSSCCUIM1MND
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_MSSCCUIM0MND[] =
{
  {    4807680, &ClockSource_XO,                          { 0,     2,     313,   1250,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    4953600, &ClockSource_XO,                          { 0,     2,     129,   500,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    4961280, &ClockSource_XO,                          { 0,     2,     323,   1250,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    5120000, &ClockSource_XO,                          { 0,     2,     4,     15,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    5283840, &ClockSource_XO,                          { 0,     2,     172,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    5285120, &ClockSource_XO,                          { 0,     2,     4129,  15000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    5760000, &ClockSource_XO,                          { 0,     2,     3,     10,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    5775360, &ClockSource_XO,                          { 0,     2,     188,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    6328320, &ClockSource_XO,                          { 0,     2,     206,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    6343680, &ClockSource_XO,                          { 0,     2,     413,   1250,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    6400000, &ClockSource_XO,                          { 0,     2,     1,     3,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    6604800, &ClockSource_XO,                          { 0,     2,     43,    125,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    6606080, &ClockSource_XO,                          { 0,     2,     5161,  15000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    6606400, &ClockSource_XO,                          { 0,     2,     4129,  12000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    6606720, &ClockSource_XO,                          { 0,     2,     3441,  10000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    6607360, &ClockSource_XO,                          { 0,     2,     2581,  7500,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7045120, &ClockSource_XO,                          { 0,     2,     688,   1875,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7046400, &ClockSource_XO,                          { 0,     2,     367,   1000,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7046880, &ClockSource_XO,                          { 0,     2,     14681, 40000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7047040, &ClockSource_XO,                          { 0,     2,     11011, 30000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7047680, &ClockSource_XO,                          { 0,     2,     2753,  7500,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7188480, &ClockSource_XO,                          { 0,     2,     234,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7200000, &ClockSource_XO,                          { 0,     2,     3,     8,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7201280, &ClockSource_XO,                          { 0,     2,     2813,  7500,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7203840, &ClockSource_XO,                          { 0,     2,     469,   1250,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7432320, &ClockSource_XO,                          { 0,     2,     3871,  10000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7680000, &ClockSource_XO,                          { 0,     2,     2,     5,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7925760, &ClockSource_XO,                          { 0,     2,     258,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7927680, &ClockSource_XO,                          { 0,     2,     4129,  10000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7928320, &ClockSource_XO,                          { 0,     2,     3097,  7500,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    7929600, &ClockSource_XO,                          { 0,     2,     413,   1000,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    8000000, &ClockSource_XO,                          { 0,     2,     5,     12,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    8257920, &ClockSource_XO,                          { 0,     2,     4301,  10000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    8258560, &ClockSource_XO,                          { 0,     2,     1613,  3750,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    8455680, &ClockSource_XO,                          { 0,     2,     1101,  2500,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    8456320, &ClockSource_XO,                          { 0,     2,     13213, 30000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    8458240, &ClockSource_XO,                          { 0,     2,     826,   1875,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    8640000, &ClockSource_XO,                          { 0,     2,     9,     20,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    8806400, &ClockSource_XO,                          { 0,     2,     172,   375,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    8808320, &ClockSource_XO,                          { 0,     2,     13763, 30000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    8808640, &ClockSource_XO,                          { 0,     2,     27527, 60000, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    8808960, &ClockSource_XO,                          { 0,     2,     1147,  2500,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    9000000, &ClockSource_XO,                          { 0,     2,     15,    32,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    9000960, &ClockSource_XO,                          { 0,     2,     293,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    9600000, &ClockSource_XO,                          { 0,     2,     1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
};

/*
 * ClockDomainBSP_MSSCC_MSSCCXO
 *
 * Used by:
 *   ClockDomain_MSSCC_MSSCCXO
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_MSSCCXO[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
};

/*
 * ClockDomainBSP_MSSCC_Q6CORE
 *
 * Used by:
 *   ClockDomain_MSSCC_Q6CORE
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_Q6CORE[] =
{
  {  403200000, &ClockSource_MSSCC_MPLL2,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_MSSCC_MPLL2[ 0] /* 403.0  */ },        /* Mux[1] = MPLL2_OUT_MAIN */
  {  595200000, &ClockSource_MSSCC_MPLL2,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_MSSCC_MPLL2[ 4] /* 595.0  */ },        /* Mux[1] = MPLL2_OUT_MAIN */
  {  729600000, &ClockSource_MSSCC_MPLL2,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL2[ 6] /* 729.0  */ },        /* Mux[1] = MPLL2_OUT_MAIN */
  {  864000000, &ClockSource_MSSCC_MPLL2,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_MSSCC_MPLL2[ 1] /* 864.0  */ },        /* Mux[1] = MPLL2_OUT_MAIN */
  {  960000000, &ClockSource_MSSCC_MPLL2,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL2[ 2] /* 960.0  */ },        /* Mux[1] = MPLL2_OUT_MAIN */
  { 1056000000, &ClockSource_MSSCC_MPLL2,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0}}, &ClockSourceBSP_MSSCC_MPLL2[ 3] /* 1056.0 */ },        /* Mux[1] = MPLL2_OUT_MAIN */
  { 1190400000, &ClockSource_MSSCC_MPLL2,                 { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR_L1,     {{0}}, &ClockSourceBSP_MSSCC_MPLL2[ 5] /* 1190.0 */ },        /* Mux[1] = MPLL2_OUT_MAIN */
};

/*
 * ClockDomainBSP_NAVCC_NAVCCBBMAIN
 *
 * Used by:
 *   ClockDomain_NAVCC_NAVCCBBMAIN
 */
static ClockMuxConfigType ClockDomainBSP_NAVCC_NAVCCBBMAIN[] =
{
  {   82285714, NULL,                                     { 1,     14,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = NAV_PLL_CLK */
  {  192000000, NULL,                                     { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = NAV_PLL_CLK */
};

/*
 * ClockDomainBSP_NAVCC_NAVCCWTRRCG
 *
 * Used by:
 *   ClockDomain_NAVCC_NAVCCWTRRCG
 */
static ClockMuxConfigType ClockDomainBSP_NAVCC_NAVCCWTRRCG[] =
{
  {  164571429, NULL,                                     { 1,     7,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = NAV_PLL_CLK */
};


/*=========================================================================
      Domain Source Maps
==========================================================================*/

/*
 * ClockSourceMap_GCC_GCCQUPV3WRAP0S0
 *
 * Used by:
 *   ClockDomain_GCC_GCCQUPV3WRAP0S0
 *   ClockDomain_GCC_GCCQUPV3WRAP0S1
 *   ClockDomain_GCC_GCCQUPV3WRAP0S2
 *   ClockDomain_GCC_GCCQUPV3WRAP0S3
 *   ClockDomain_GCC_GCCQUPV3WRAP0S4
 *   ClockDomain_GCC_GCCQUPV3WRAP0S5
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCQUPV3WRAP0S0[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 2,  300000000, &ClockSource_GCC_GPLL0,                  },
  { 4,  384000000, &ClockSource_GCC_GPLL6,                  },
  { 7,          0, NULL,                                    },
  { CLOCK_SOURCE_MAP_END }
};


/*=========================================================================
      Domain FMAX
==========================================================================*/

/*
 * ClockDomainFMax_GCC_GCCQUPV3WRAP0S0
 *
 * Used by:
 *   ClockDomain_GCC_GCCQUPV3WRAP0S0
 *   ClockDomain_GCC_GCCQUPV3WRAP0S1
 *   ClockDomain_GCC_GCCQUPV3WRAP0S2
 *   ClockDomain_GCC_GCCQUPV3WRAP0S3
 *   ClockDomain_GCC_GCCQUPV3WRAP0S4
 *   ClockDomain_GCC_GCCQUPV3WRAP0S5
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCQUPV3WRAP0S0[] =
{
  {   75000000, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },
  {  100000000, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  128000000, RAIL_VOLTAGE_LEVEL_NOM,        },
  {  0  }
};


/*=========================================================================
      Domains
==========================================================================*/

static ClockDomainNodeType ClockDomain_GCC_CLOCKDRIVERGENXO =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = NULL,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCCONFIGNOC =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_CONFIG_NOC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQDSSAPBTSCTR =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QDSS_APB_TSCTR_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0CORE2X =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP0_CORE_2X_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S0 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP0_S0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S1 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP0_S1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S2 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP0_S2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S3 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP0_S3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S4 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP0_S4_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S5 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP0_S5_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCSECCTRL =
{
  .aBSP      = ClockDomainBSP_GCC_GCCSECCTRL,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCSECCTRL),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_SEC_CTRL_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCSLEEP =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = NULL,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_SLEEP_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCSNOCPERIPH =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_SNOC_PERIPH_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCSYSNOC =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_SYS_NOC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKMODEMAXI =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKMODEMAXI,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKMODEMAXI),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKMODEMCCS =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKMODEMCCS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKMODEMCCS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKMODEMMTC =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKMODEMMTC,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKMODEMMTC),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKMODEMTDEC =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKMODEMTDEC,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKMODEMTDEC),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_MSSCCBITCOXM =
{
  .aBSP      = ClockDomainBSP_MSSCC_MSSCCBITCOXM,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_MSSCCBITCOXM),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_CC_BIT_COXM_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_MSSCCBUSPREMUX =
{
  .aBSP      = ClockDomainBSP_MSSCC_MSSCCBUSPREMUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_MSSCCBUSPREMUX),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_CC_BUS_PRE_MUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_MSSCCMODEMBUSPREMUX =
{
  .aBSP      = ClockDomainBSP_MSSCC_MSSCCMODEMBUSPREMUX,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_MSSCCMODEMBUSPREMUX),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_MSSCCQ6AXIS =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainMuxControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_MSSCCQ6 =
{
  .aBSP      = ClockDomainBSP_MSSCC_MSSCCQ6,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_MSSCCQ6),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_CC_Q6_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_MSSCCSILVER =
{
  .aBSP      = ClockDomainBSP_MSSCC_MSSCCSILVER,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_MSSCCSILVER),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_CC_SILVER_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_MSSCCUIM0MND =
{
  .aBSP      = ClockDomainBSP_MSSCC_MSSCCUIM0MND,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_MSSCCUIM0MND),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_CC_UIM0_MND_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_MSSCCUIM1MND =
{
  .aBSP      = ClockDomainBSP_MSSCC_MSSCCUIM0MND,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_MSSCCUIM0MND),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_DOMAIN_WAIT_FOR_ROOT_OFF,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_CC_UIM1_MND_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_MSSCCXO =
{
  .aBSP      = ClockDomainBSP_MSSCC_MSSCCXO,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_MSSCCXO),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_CC_XO_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_Q6CORE =
{
  .aBSP      = ClockDomainBSP_MSSCC_Q6CORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_Q6CORE),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_QDSP6SS_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_NAVCC_NAVCCBBMAIN =
{
  .aBSP      = ClockDomainBSP_NAVCC_NAVCCBBMAIN,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_NAVCC_NAVCCBBMAIN),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_NAV_CC_BB_MAIN_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_NAVCC_NAVCCWTRRCG =
{
  .aBSP      = ClockDomainBSP_NAVCC_NAVCCWTRRCG,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_NAVCC_NAVCCWTRRCG),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = NULL,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_NAV_CC_WTR_RCG_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};


static ClockDomainNodeType *aDomains[] =
{
  &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
  &ClockDomain_GCC_CLOCKDRIVERGENXO,
  &ClockDomain_GCC_GCCCONFIGNOC,
  &ClockDomain_GCC_GCCQDSSAPBTSCTR,
  &ClockDomain_GCC_GCCQUPV3WRAP0CORE2X,
  &ClockDomain_GCC_GCCQUPV3WRAP0S0,
  &ClockDomain_GCC_GCCQUPV3WRAP0S1,
  &ClockDomain_GCC_GCCQUPV3WRAP0S2,
  &ClockDomain_GCC_GCCQUPV3WRAP0S3,
  &ClockDomain_GCC_GCCQUPV3WRAP0S4,
  &ClockDomain_GCC_GCCQUPV3WRAP0S5,
  &ClockDomain_GCC_GCCSECCTRL,
  &ClockDomain_GCC_GCCSLEEP,
  &ClockDomain_GCC_GCCSNOCPERIPH,
  &ClockDomain_GCC_GCCSYSNOC,
  &ClockDomain_MSSCC_CLKMODEMAXI,
  &ClockDomain_MSSCC_CLKMODEMCCS,
  &ClockDomain_MSSCC_CLKMODEMMTC,
  &ClockDomain_MSSCC_CLKMODEMTDEC,
  &ClockDomain_MSSCC_MSSCCBITCOXM,
  &ClockDomain_MSSCC_MSSCCBUSPREMUX,
  &ClockDomain_MSSCC_MSSCCMODEMBUSPREMUX,
  &ClockDomain_MSSCC_MSSCCQ6,
  &ClockDomain_MSSCC_MSSCCQ6AXIS,
  &ClockDomain_MSSCC_MSSCCSILVER,
  &ClockDomain_MSSCC_MSSCCUIM0MND,
  &ClockDomain_MSSCC_MSSCCUIM1MND,
  &ClockDomain_MSSCC_MSSCCXO,
  &ClockDomain_MSSCC_Q6CORE,
  &ClockDomain_NAVCC_NAVCCBBMAIN,
  &ClockDomain_NAVCC_NAVCCWTRRCG,
};


/*=========================================================================
      Clocks
==========================================================================*/

static ClockNodeType aClocks[] =
{
  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_CLOCKDRIVERGENXO                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "mss_cc_mpll1_out_early_div2_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xA4774CE4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "mss_cc_mpll1_out_early_div3_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0xB85AEFE6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "mss_cc_mpll1_out_early_div5_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = NULL,
    .nUniqueId = 0x8D96D087,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "nav_cc_stmr_xo_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0xACD530C0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_STMR_XO_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 13,
    },
  },
  {
    .szName    = "nav_cc_xo_gdsc_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0x41C50A89,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_XO_GDSC_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 14,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "mss_cc_modem_atb_debug_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x67DF02F9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MODEM_ATB_DEBUG_CBCR), HWIO_ADDR(MSS_CC_MODEM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 53,
    },
  },
  {
    .szName    = "mss_cc_modem_rffe_two_xo_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x4FBB2CD4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MODEM_RFFE_TWO_XO_CBCR), HWIO_ADDR(MSS_CC_MODEM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 54,
    },
  },
  {
    .szName    = "mss_cc_modem_snoc_axi_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x7930215E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MODEM_SNOC_AXI_CBCR), HWIO_ADDR(MSS_CC_MODEM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 24,
    },
  },
  {
    .szName    = "nav_cc_adc_fifo_wr_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0xB3566069,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_ADC_FIFO_WR_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 18,
    },
  },
  {
    .szName    = "nav_cc_snoc_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0x984B1121,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_SNOC_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 16,
    },
  },
  {
    .szName    = "nav_cc_snoc_dbg_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0x4DEAB2A1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_SNOC_DBG_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 17,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCCONFIGNOC                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_boot_rom_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x20C2EB7C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BOOT_ROM_AHB_CBCR), 0, {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x400} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 118,
    },
  },
  {
    .szName    = "gcc_mss_q6_msmpu_cfg_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x62390B71,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_Q6_MSMPU_CFG_AHB_CBCR), HWIO_ADDR(GCC_MSMPU_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 219,
    },
  },
  {
    .szName    = "gcc_prng_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xBFB51CCC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_PRNG_AHB_CBCR), HWIO_ADDR(GCC_PRNG_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x2000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 116,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_0_s_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xB8C59FEE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP_0_S_AHB_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x80} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 104,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQDSSAPBTSCTR                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_wcss_tsctr_clk",
    .pDomain   = &ClockDomain_GCC_GCCQDSSAPBTSCTR,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x15E052C5,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_WCSS_TSCTR_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 226,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0CORE2X                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_core_2x_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x77020808,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP0_CORE_2X_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x200} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 106,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap0_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x1B88DC58,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP0_CORE_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x100} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 105,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S0                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s0_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x56C239B6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP0_S0_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x400} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 107,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S1                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s1_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x299AA1D4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP0_S1_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x800} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 108,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S2                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s2_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S2,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x06533C2C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP0_S2_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x1000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 109,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S3                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s3_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S3,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x939588DB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP0_S3_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x2000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 110,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S4                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s4_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S4,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x62F41847,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP0_S4_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x4000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 111,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S5                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s5_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S5,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5751B5F3,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP0_S5_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x8000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 112,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSECCTRL                                            */
  /*-----------------------------------------------------------------------*/

  /*
   * Fake clock added with domain name since there are no actual branches
   * on the domain. Used to control frequency.
   */
  {
    .szName    = "gcc_sec_ctrl_clk_src",
    .pDomain   = &ClockDomain_GCC_GCCSECCTRL,
    .pDebugMux = NULL,
    .nUniqueId = 0x44397038,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = NULL,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSLEEP                                              */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_wcss_ecahb_sleep_clk",
    .pDomain   = &ClockDomain_GCC_GCCSLEEP,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x3FA3265F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_WCSS_ECAHB_SLEEP_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 228,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSNOCPERIPH                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap_0_m_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCSNOCPERIPH,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x79AD882E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP_0_M_AHB_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x40} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 103,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSYSNOC                                             */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_mss_nav_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x0B672D67,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_NAV_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 204,
    },
  },
  {
    .szName    = "gcc_nav_snoc_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xE0F37A4F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_NAV_SNOC_AXI_CBCR), HWIO_ADDR(GCC_NAV_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 241,
    },
  },
  {
    .szName    = "gcc_wcss_ahb_s0_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xD9121961,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_WCSS_AHB_S0_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 220,
    },
  },
  {
    .szName    = "gcc_wcss_axi_m_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x0100A916,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_WCSS_AXI_M_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 221,
    },
  },
  {
    .szName    = "gcc_wcss_ecahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x708D6313,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_WCSS_ECAHB_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 222,
    },
  },
  {
    .szName    = "gcc_wcss_shdreg_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xE10F50E6,
     .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_WCSS_SHDREG_AHB_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 223,
    },
  },
  
  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKMODEMAXI                                         */
  /*-----------------------------------------------------------------------*/
  
    //Controlled by MCPM through registers
      #if 0 
  /*
   * Fake clock added with domain name since there are no actual branches
   * on the domain. Used to control frequency.
   */
  {
    .szName    = "clk_modem_axi",
    .pDomain   = &ClockDomain_MSSCC_CLKMODEMAXI,
    .pDebugMux = NULL,
    .nUniqueId = 0xAEE1BF2C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = NULL,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKMODEMCCS                                         */
  /*-----------------------------------------------------------------------*/

  /*
   * Fake clock added with domain name since there are no actual branches
   * on the domain. Used to control frequency.
   */
  {
    .szName    = "clk_modem_ccs",
    .pDomain   = &ClockDomain_MSSCC_CLKMODEMCCS,
    .pDebugMux = NULL,
    .nUniqueId = 0x2D0F3B8D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = NULL,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKMODEMMTC                                         */
  /*-----------------------------------------------------------------------*/

  /*
   * Fake clock added with domain name since there are no actual branches
   * on the domain. Used to control frequency.
   */
  {
    .szName    = "clk_modem_mtc",
    .pDomain   = &ClockDomain_MSSCC_CLKMODEMMTC,
    .pDebugMux = NULL,
    .nUniqueId = 0x9E2777A8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = NULL,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKMODEMTDEC                                        */
  /*-----------------------------------------------------------------------*/

  /*
   * Fake clock added with domain name since there are no actual branches
   * on the domain. Used to control frequency.
   */
  {
    .szName    = "clk_modem_tdec",
    .pDomain   = &ClockDomain_MSSCC_CLKMODEMTDEC,
    .pDebugMux = NULL,
    .nUniqueId = 0xB088DD3D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = NULL,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  #endif
  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_MSSCCBITCOXM                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "mss_cc_bit_coxm_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBITCOXM,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x0EB7B26A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_BIT_COXM_CBCR), HWIO_ADDR(MSS_CC_COXM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 32,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_MSSCCBUSPREMUX                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "mss_cc_atb_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x52B4B1BC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_ATB_BUS_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 10,
    },
  },
  {
    .szName    = "mss_cc_bridge_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x72487C2B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_BRIDGE_BUS_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 11,
    },
  },
  {
    .szName    = "mss_cc_bridge_mon_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = NULL,
    .nUniqueId = 0x3322FBCA,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "mss_cc_coxm_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xE01256F9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_COXM_BUS_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 4,
    },
  },
  {
    .szName    = "mss_cc_crypto_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x6B436F05,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_CRYPTO_BUS_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 5,
    },
  },
  {
    .szName    = "mss_cc_crypto_core_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x6FBC5435,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_CRYPTO_CORE_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 51,
    },
  },
  {
    .szName    = "mss_cc_csr_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x76EC37D8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_CSR_BUS_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 9,
    },
  },
  {
    .szName    = "mss_cc_mgpi_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xB10A71E8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MGPI_BUS_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 37,
    },
  },
  {
    .szName    = "mss_cc_modem_bridge_cx_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x46349057,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR), HWIO_ADDR(MSS_CC_MODEM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 25,
    },
  },
  {
    .szName    = "mss_cc_modem_ccs_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xE8292D62,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MODEM_CCS_CBCR), HWIO_ADDR(MSS_CC_MODEM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 56,
    },
  },
  {
    .szName    = "mss_cc_modem_core_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xE5DABF3C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MODEM_CORE_CBCR), HWIO_ADDR(MSS_CC_MODEM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 55,
    },
  },
  {
    .szName    = "mss_cc_modem_demback_core_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x5D128135,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MODEM_DEMBACK_CORE_CBCR), HWIO_ADDR(MSS_CC_MODEM_DEMBACK_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 58,
    },
  },
  {
    .szName    = "mss_cc_modem_tdec_core_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x95F5ABF0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MODEM_TDEC_CORE_CBCR), HWIO_ADDR(MSS_CC_MODEM_TDEC_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 59,
    },
  },
  {
    .szName    = "mss_cc_modem_tx_core_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x7A65F821,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MODEM_TX_CORE_CBCR), HWIO_ADDR(MSS_CC_MODEM_TX_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 57,
    },
  },
  {
    .szName    = "mss_cc_nav_bus_cx_to_ls_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x565F0225,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_NAV_BUS_CX_TO_LS_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 16,
    },
  },
  {
    .szName    = "mss_cc_q6_alt_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x214D6FB7,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_Q6_ALT_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 52,
    },
  },
  {
    .szName    = "mss_cc_q6_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x073639C1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_Q6_BUS_CBCR), HWIO_ADDR(MSS_CC_Q6_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 1,
    },
  },
  {
    .szName    = "mss_cc_slave_timeout_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xD89CA714,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_SLAVE_TIMEOUT_BUS_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 13,
    },
  },
  {
    .szName    = "mss_cc_uim0_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x03640994,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_UIM0_BUS_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 6,
    },
  },
  {
    .szName    = "mss_cc_uim1_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xCA0FE23E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_UIM1_BUS_CBCR), HWIO_ADDR(MSS_CC_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 7,
    },
  },

  // Controlled by MCPM through registers
  
  #if 0
  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_MSSCCMODEMBUSPREMUX                                 */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "mss_cc_modem_bus_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCMODEMBUSPREMUX,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x3587A3F9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MODEM_BUS_CBCR), HWIO_ADDR(MSS_CC_MODEM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 60,
    },
  },
  #endif
  
  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_MSSCCQ6AXIS                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "mss_cc_q6_apq_axis_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCQ6AXIS,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xAD2B3F35,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_Q6_APQ_AXIS_CBCR), HWIO_ADDR(MSS_CC_Q6_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 49,
    },
  },
  {
    .szName    = "mss_cc_q6_axis_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCQ6AXIS,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x16D3A74F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_Q6_AXIS_CBCR), HWIO_ADDR(MSS_CC_Q6_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 62,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_MSSCCQ6                                             */
  /*-----------------------------------------------------------------------*/

  /*
   * Fake clock added with domain name since there are no actual branches
   * on the domain. Used to control frequency.
   */
  {
    .szName    = "mss_cc_q6_clk_src",
    .pDomain   = &ClockDomain_MSSCC_MSSCCQ6,
    .pDebugMux = NULL,
    .nUniqueId = 0x0B24E248,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_ADDR(MSS_CC_Q6_BCR), {0, 0} },
      .pmControl    = NULL,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_MSSCCSILVER                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_silver",
    .pDomain   = &ClockDomain_MSSCC_MSSCCSILVER,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xDAE00F3F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_QDSP6SS_VPE_VU_CBCR), HWIO_ADDR(MSS_CC_Q6_BCR), {0, 0} },
      .pmControl    = &HAL_clk_MSSSilverClockControl,
      .nDebugMuxSel = 63,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_MSSCCUIM0MND                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "mss_cc_uim0_xo_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCUIM0MND,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xEF8EFA41,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_UIM0_XO_CBCR), HWIO_ADDR(MSS_CC_UIM0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 27,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_MSSCCUIM1MND                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "mss_cc_uim1_xo_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCUIM1MND,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x6F0DE94B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_UIM1_XO_CBCR), HWIO_ADDR(MSS_CC_UIM1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 29,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_MSSCCXO                                             */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "mss_cc_modem_xo_cx_to_ls_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCXO,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x52BBCFD0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_MODEM_XO_CX_TO_LS_CBCR), HWIO_ADDR(MSS_CC_MODEM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 23,
    },
  },
  {
    .szName    = "mss_cc_qtimer_sync_xo_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCXO,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x73687A67,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_QTIMER_SYNC_XO_CBCR), HWIO_ADDR(MSS_CC_XO_TO_Q6_MODEM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 61,
    },
  },
  {
    .szName    = "mss_cc_uim0_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCXO,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xAE183DDE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_UIM0_CBCR), HWIO_ADDR(MSS_CC_UIM0_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 26,
    },
  },
  {
    .szName    = "mss_cc_uim1_clk",
    .pDomain   = &ClockDomain_MSSCC_MSSCCXO,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x0684AD87,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CC_UIM1_CBCR), HWIO_ADDR(MSS_CC_UIM1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 28,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_Q6CORE                                              */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_q6",
    .pDomain   = &ClockDomain_MSSCC_Q6CORE,
    .pDebugMux = &ClockDebugMux_MSSCCQ6,
    .nUniqueId = 0x7722DE95,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, HWIO_ADDR(MSS_CC_Q6_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 1,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_NAVCC_NAVCCBBMAIN                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "nav_cc_bb_core_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCBBMAIN,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0xCA3DAA4B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_BB_CORE_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 5,
    },
  },
  {
    .szName    = "nav_cc_bb_core_dbg_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCBBMAIN,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0x847B1219,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_BB_CORE_DBG_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 18,
    },
  },
  {
    .szName    = "nav_cc_cp_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCBBMAIN,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0xCC43DFD4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_CP_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 6,
    },
  },
  {
    .szName    = "nav_cc_dma_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCBBMAIN,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0x7A7AD774,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_DMA_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 7,
    },
  },
  {
    .szName    = "nav_cc_dp_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCBBMAIN,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0xAF4F0609,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_DP_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 8,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_NAVCC_NAVCCWTRRCG                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "nav_cc_adc_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCWTRRCG,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0x15BD288D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_ADC_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 10,
    },
  },
  {
    .szName    = "nav_cc_bp1_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCWTRRCG,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0xDA9DDBF4,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_BP1_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 11,
    },
  },
  {
    .szName    = "nav_cc_bp2_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCWTRRCG,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0x818C7AC2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_BP2_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 13,
    },
  },
  {
    .szName    = "nav_cc_bp3_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCWTRRCG,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0xA3AA48E7,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_BP3_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 12,
    },
  },
  {
    .szName    = "nav_cc_bp4_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCWTRRCG,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0x8D8B7B30,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_BP4_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 14,
    },
  },
  {
    .szName    = "nav_cc_wtr_core_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCWTRRCG,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0x146176E5,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_WTR_CORE_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 9,
    },
  },
  {
    .szName    = "nav_cc_wtr_core_dbg_clk",
    .pDomain   = &ClockDomain_NAVCC_NAVCCWTRRCG,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0xE44BEF54,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_WTR_CORE_DBG_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 21,
    },
  },
};


/*=========================================================================
      Power Domains
==========================================================================*/

static ClockPowerDomainNodeType aPowerDomains[] =
{
  {
    .szName         = "modem_demback_core_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_ADDR(MSS_CC_MODEM_DEMBACK_CORE_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "modem_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_ADDR(MSS_CC_MODEM_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "modem_tdec_core_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_ADDR(MSS_CC_MODEM_TDEC_CORE_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "modem_tx_core_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_ADDR(MSS_CC_MODEM_TX_CORE_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
  },
  {
    .szName         = "nav_ss_gdsc",
    .pRail          = &ClockRail_VDDCX,
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_ADDR(MSS_NAV_CC_NAV_SS_GDSCR),
      .pmControl    = &HAL_clk_NavPowerDomainControl,
    },
  },
};


/*=========================================================================
      Remote NPA Resources
==========================================================================*/

static ClockRemoteNodeType aRemoteNodes[] =
{
  {
    .Resource = {
      .local_resource_name  = "/clk/ce1",
      .remote_resource_name = "ce\x00\x00\x00\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/cnoc",
      .remote_resource_name = "clk1\x01\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/dcvs.ena",
      .remote_resource_name = "clk0\x02\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "State",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/hwkm",
      .remote_resource_name = "hwkm\x00\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/ipa",
      .remote_resource_name = "ipa\x00\x00\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/mmnrt",
      .remote_resource_name = "mmxi\x00\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/mmrt",
      .remote_resource_name = "mmxi\x01\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/pka",
      .remote_resource_name = "pka\x00\x00\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/qdss",
      .remote_resource_name = "clk0\x01\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "Enable/Disable",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/qpic",
      .remote_resource_name = "qpic\x00\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/qup",
      .remote_resource_name = "qup\x00\x00\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/shub",
      .remote_resource_name = "clk2\x01\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/snoc",
      .remote_resource_name = "clk1\x02\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/snoc_gc",
      .remote_resource_name = "clk1\x03\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/snoc_lpass",
      .remote_resource_name = "clk1\x05\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/clk/snoc_periph",
      .remote_resource_name = "clk1\x00\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "KHz",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/ipa/pc",
      .remote_resource_name = "clk0\x03\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "Enable/Disable",
      .max                  = NPA_MAX_STATE,
    },
  },
  {
    .Resource = {
      .local_resource_name  = "/modem/xo/cxo",
      .remote_resource_name = "clk0\x00\x00\x00\x00",
      .protocol_type        = "/protocol/rpm/rpm",
      .plugin               = &npa_max_plugin,
      .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
      .units                = "Enable/Disable",
      .max                  = 1,
    },
    .nInitState = 1,
  },
};

/*=========================================================================
      NPA Resources
==========================================================================*/

static struct npa_resource_definition aNPAResources[] =
{
  {
    .name       = "/clk/mss/config_bus",
    .units      = "KHz",
    .plugin     = &npa_max_plugin,
    .attributes = NPA_RESOURCE_DEFAULT,
    .data       = (npa_user_data) &(ClockNPACtxtType){ "mss_cc_bridge_bus_clk" },
  },
};

static ClockNPANodeType GenericNPANode =
{
  .Node = {
    .name           = "/node/clk/generic",
    .driver_fcn     = Clock_GenericNPANodeFunc,
    .attributes     = NPA_NODE_DEFAULT,
    .resource_count = SIZEOF_ARRAY(aNPAResources),
    .resources      = aNPAResources,
  },
  .anInitStates = (npa_resource_state[]) {
    72000,         /* /clk/mss/config_bus */
  },
};

static ClockDividerNodeType aDividers[] =
{
  {
    .szName    = "mss_cc_uim_div_clk_src",
    .nFlags    = 0x0,
    .HALDivider = {
      .nAddr       = HWIO_ADDR(MSS_CC_UIM_DIV_CDIVR),
      .pmControl   = &HAL_clk_GenericDividerControl,
    },
  },
};

/*=========================================================================
      Main BSP
==========================================================================*/

ClockBSPType ClockBSP =
{
  .aRails            = aRails,
  .nNumRails         = SIZEOF_ARRAY(aRails),
  .aClocks           = aClocks,
  .nNumClocks        = SIZEOF_ARRAY(aClocks),
  .aDomains          = aDomains,
  .nNumDomains       = SIZEOF_ARRAY(aDomains),
  .aDividers         = aDividers,
  .nNumDividers      = SIZEOF_ARRAY(aDividers),
  .aSources          = aSources,
  .nNumSources       = SIZEOF_ARRAY(aSources),
  .aPowerDomains     = aPowerDomains,
  .nNumPowerDomains  = SIZEOF_ARRAY(aPowerDomains),
  .aDebugMuxes       = aDebugMuxes,
  .nNumDebugMuxes    = SIZEOF_ARRAY(aDebugMuxes),
  .nFlags            = CLOCK_GLOBAL_FLAG_LOG_CLOCK_FREQ_CHANGE |
                       CLOCK_GLOBAL_FLAG_LOG_SOURCE_STATE_CHANGE,
  .SafeMuxConfig     = { 0, 2, 0, 0 },
  .pGenericNPANode   = &GenericNPANode,
  .aRemoteNodes      = aRemoteNodes,
  .nNumRemoteNodes   = SIZEOF_ARRAY(aRemoteNodes),
};

