/*
==============================================================================

FILE:         ClockBSP.c

DESCRIPTION:
  This file contains the clock driver BSP data.

  Auto-generated from the following IPCat controller releases:

    SM7150v1 (moorea_1.0):
      PLL_HSR   : 10.0
      GCC       : z_moorea_clk_ctl_1.0_p3q3r29.5.2396 [moorea_gcc.r8]
      MSS_CC    : atlas_mss_3ca_10lp_moorea_p3q2r2_frequency_plan_gcc_freq_update [moorea_mss_cc.r2]
      NavSS     : nav_ss_napali_2.0_p3q3r6.4 [napali_v2_nav_ss_cc.r1]


==============================================================================

$Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sm7150/ClockBSP.c#1 $

==============================================================================
            Copyright (c) 2018 Qualcomm Technologies Incorporated.
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
extern HAL_clk_SourceControlType          HAL_clk_FabiaPLLControl;

/*
 * Debug mux control structures.
 */
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlGCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlMSSCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlMSSCCQ6;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlNAVSS;

/*
 * Custom control structures.
 */
extern HAL_clk_ClockDomainControlType     HAL_clk_mMSSQ6ClockDomainControl;
extern HAL_clk_SourceControlType          HAL_clk_GPLLStubPLLControl;
extern HAL_clk_ClockDomainControlType     HAL_clk_mMSSUIMClockDomainControl;

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

static ClockRailType ClockRail_VDDMSSQ6 =
{
  .szName = "/vcs/vdd_mss_q6",
};

static ClockRailType ClockRail_VDDMX =
{
  .szName = "/vcs/vdd_mx",
};

static ClockRailType *aRails[] =
{
  &ClockRail_VDDCX,
  &ClockRail_VDDMSSQ6,
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
  .nParentSel   = 310,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlMSSCC,
  },
};

static ClockDebugMuxType ClockDebugMux_NAVSS =
{
  .szName       = "navss",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 341,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlNAVSS,
  },
};

static ClockDebugMuxType ClockDebugMux_MSSCCQ6 =
{
  .szName       = "mss_cc_q6",
  .pParent      = &ClockDebugMux_MSSCC,
  .nParentSel   = 105,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlMSSCCQ6,
  },
};

static ClockDebugMuxType *aDebugMuxes[] =
{
  &ClockDebugMux_GCC,
  &ClockDebugMux_MSSCC,
  &ClockDebugMux_MSSCCQ6,
  &ClockDebugMux_NAVSS,
};


/*=========================================================================
      Primary Sources
==========================================================================*/

static ClockSourceNodeType ClockSource_XO =
{
  .szName    = "/pmic/client/xo",
  .nFlags    = 0x0,
};


/*=========================================================================
      Source Register Settings
==========================================================================*/

static const ClockSourceRegSettingsType ClockSourceRegSettings_MSSCC_OFFLINEPLL[] =
{
  {
    .HALSettings = {
      .nConfigCtl   = 0x20485699,
      .nConfigCtl1  = 0x00002067,
      .nTestCtl     = 0x40000000,
      .nTestCtl1    = 0x00000000,
      .nUserCtl     = 0x00000000,
      .nUserCtl1    = 0x00004805,
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
  {  600000000, &ClockSource_XO, { 0, 0, 1, 31,  0x00004000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Fabia.nEven = 2}           }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};

/*
 * ClockSourceBSP_GCC_GPLL6
 *
 * Used by:
 */
static const ClockSourceFreqConfigType ClockSourceBSP_GCC_GPLL6[] =
{
  {  384000000, &ClockSource_XO, { 0, 0, 1, 20,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};

/*
 * ClockSourceBSP_MSSCC_OFFLINEPLL
 *
 * Used by:
 *   ClockSource_MSSCC_OFFLINEPLL
 */
static const ClockSourceFreqConfigType ClockSourceBSP_MSSCC_OFFLINEPLL[] =
{
  { 1593600000, &ClockSource_XO, { 0, 0, 1, 83,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    },
  { 1708800000, &ClockSource_XO, { 0, 0, 1, 89,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_NOM,       },
  { 1996800000, &ClockSource_XO, { 0, 0, 1, 104, 0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_NOM,       },
};
static const ClockSourceFreqConfigType ClockSourceBSPCal_MSSCC_OFFLINEPLL[] =
{
  { 1785600000, &ClockSource_XO, { 0, 0, 1, 93,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_NOM,       },
};

/*
 * ClockSourceBSP_MSSCC_QDSP6SSPLL
 *
 * Used by:
 *   ClockSource_MSSCC_QDSP6SSPLL
 */
static const ClockSourceFreqConfigType ClockSourceBSP_MSSCC_QDSP6SSPLL[] =
{
  {  460800000, &ClockSource_XO, { 0, 0, 1, 24,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  614400000, &ClockSource_XO, { 0, 0, 1, 32,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  787200000, &ClockSource_XO, { 0, 0, 1, 41,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       },
  {  940800000, &ClockSource_XO, { 0, 0, 1, 49,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       },
  { 1056000000, &ClockSource_XO, { 0, 0, 1, 55,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       },
  { 1190400000, &ClockSource_XO, { 0, 0, 1, 62,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS_L1,    },
};
static const ClockSourceFreqConfigType ClockSourceBSPCal_MSSCC_QDSP6SSPLL[] =
{
  {  825600000, &ClockSource_XO, { 0, 0, 1, 43,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_SVS,       },
};

/*
 * ClockSourceBSP_NAVSS_NAVCCPLL4PLL
 *
 * Used by:
 *   ClockSource_NAVSS_NAVCCPLL4PLL
 */
static const ClockSourceFreqConfigType ClockSourceBSP_NAVSS_NAVCCPLL4PLL[] =
{
  {  336000000, &ClockSource_XO, { 0, 0, 1, 17,  0x00008000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};


/*=========================================================================
      Sources
==========================================================================*/

static ClockSourceNodeType ClockSource_GCC_GPLL0 =
{
  .szName       = "gpll0",
  .nFlags       = CLOCK_FLAG_READ_ONLY,
  .aBSP         = ClockSourceBSP_GCC_GPLL0,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL0),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE |
                  CLOCK_CONFIG_PLL_FSM_MODE_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(GCC_GPLL0_MODE),
    .VoterRegister  = {HWIO_ADDR(GCC_PROC_CLK_PLL_ENA_VOTE), 0x1},
    .pmControl      = &HAL_clk_GPLLStubPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSP_GCC_GPLL0,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL0),
};

static ClockSourceNodeType ClockSource_GCC_GPLL6 =
{
  .szName       = "gpll6",
  .nFlags       = CLOCK_FLAG_READ_ONLY,
  .aBSP         = ClockSourceBSP_GCC_GPLL6,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL6),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE |
                  CLOCK_CONFIG_PLL_FSM_MODE_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(GCC_GPLL6_MODE),
    .VoterRegister  = {HWIO_ADDR(GCC_PROC_CLK_PLL_ENA_VOTE), 0x40},
    .pmControl      = &HAL_clk_FabiaPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSP_GCC_GPLL6,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL6),
};

static ClockSourceNodeType ClockSource_MSSCC_OFFLINEPLL =
{
  .szName       = "offline_pll",
  .nFlags       = CLOCK_FLAG_SUPPORTS_SLEWING,
  .aBSP         = ClockSourceBSP_MSSCC_OFFLINEPLL,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_MSSCC_OFFLINEPLL),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = 0x0,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_STANDBY,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_OFFLINE_PLL_MODE),
    .VoterRegister  = {0, 0},
    .pmControl      = &HAL_clk_FabiaPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_MSSCC_OFFLINEPLL,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_MSSCC_OFFLINEPLL),
  .pRegSettings               = ClockSourceRegSettings_MSSCC_OFFLINEPLL,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_MSSCC_OFFLINEPLL),
};

static ClockSourceNodeType ClockSource_MSSCC_QDSP6SSPLL =
{
  .szName       = "qdsp6ss_pll",
  .nFlags       = CLOCK_FLAG_SUPPORTS_SLEWING,
  .aBSP         = ClockSourceBSP_MSSCC_QDSP6SSPLL,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_MSSCC_QDSP6SSPLL),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_QDSP6SS_PLL_MODE),
    .VoterRegister  = {0, 0},
    .pmControl      = &HAL_clk_FabiaPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_MSSCC_QDSP6SSPLL,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_MSSCC_QDSP6SSPLL),
  .pRegSettings               = ClockSourceRegSettings_MSSCC_OFFLINEPLL,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_MSSCC_OFFLINEPLL),
};

static ClockSourceNodeType ClockSource_NAVSS_NAVCCPLL4PLL =
{
  .szName       = "nav_cc_pll4_pll",
  .nFlags       = 0x0,
  .aBSP         = ClockSourceBSP_NAVSS_NAVCCPLL4PLL,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_NAVSS_NAVCCPLL4PLL),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = 0x0,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_NAV_PLL4_PLL_MODE),
    .VoterRegister  = {0, 0},
    .pmControl      = &HAL_clk_FabiaPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSP_NAVSS_NAVCCPLL4PLL,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSP_NAVSS_NAVCCPLL4PLL),
  .pRegSettings               = ClockSourceRegSettings_MSSCC_OFFLINEPLL,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_MSSCC_OFFLINEPLL),
};


static ClockSourceNodeType *aSources[] =
{
  &ClockSource_GCC_GPLL0,
  &ClockSource_GCC_GPLL6,
  &ClockSource_MSSCC_OFFLINEPLL,
  &ClockSource_MSSCC_QDSP6SSPLL,
  &ClockSource_NAVSS_NAVCCPLL4PLL,
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
 *   ClockDomain_GCC_GCCQUPV3WRAP0S6
 *   ClockDomain_GCC_GCCQUPV3WRAP0S7
 *   ClockDomain_GCC_GCCQUPV3WRAP1S0
 *   ClockDomain_GCC_GCCQUPV3WRAP1S1
 *   ClockDomain_GCC_GCCQUPV3WRAP1S2
 *   ClockDomain_GCC_GCCQUPV3WRAP1S3
 *   ClockDomain_GCC_GCCQUPV3WRAP1S4
 *   ClockDomain_GCC_GCCQUPV3WRAP1S5
 *   ClockDomain_GCC_GCCQUPV3WRAP1S6
 *   ClockDomain_GCC_GCCQUPV3WRAP1S7
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCQUPV3WRAP0S0[] =
{
  {    7372800, &ClockSource_GCC_GPLL0,                   { 6,     2,     384,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   14745600, &ClockSource_GCC_GPLL0,                   { 6,     2,     768,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0x00 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   29491200, &ClockSource_GCC_GPLL0,                   { 6,     2,     1536,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   32000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     8,     75,    0x01 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   48000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     4,     25,    0x02 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   64000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     16,    75,    0x03 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   80000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     4,     15,    0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   96000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     8,     25,    0x04 }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 6,     6,     0,     0,     0x05 }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  102400000, &ClockSource_GCC_GPLL0,                   { 6,     2,     128,   375,   0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  112000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     28,    75,    0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  117964800, &ClockSource_GCC_GPLL0,                   { 6,     2,     6144,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  120000000, &ClockSource_GCC_GPLL0,                   { 6,     5,     0,     0,     0x06 }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  128000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     16,    75,    0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
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
 * ClockDomainBSP_MSSCC_CLKSRCBITCOXMMND
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCBITCOXMMND
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCBITCOXMMND[] =
{
  {    1843200, &ClockSource_GCC_GPLL0,                   { 1,     2,     192,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {    7372800, &ClockSource_GCC_GPLL0,                   { 1,     2,     768,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     8,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   32000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     16,    75,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   40000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     4,     15,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   48000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     8,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   51200000, &ClockSource_GCC_GPLL0,                   { 1,     2,     128,   375,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   60000000, &ClockSource_GCC_GPLL0,                   { 1,     5,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCBUSMSSCONFIG
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCBUSMSSCONFIG[] =
{
  {   75000000, &ClockSource_GCC_GPLL0,                   { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = CLK_SRC_BUS_DIV */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCQ6
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCQ6
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCQ6[] =
{
  {  460800000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 0] /* 460.8  */ },   /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
  {  614400000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 1] /* 614.4  */ },   /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
  {  787200000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 2] /* 787.2  */ },   /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
  {  940800000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 3] /* 940.8  */ },   /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
  { 1056000000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 4] /* 1056.0 */ },   /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
  { 1190400000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 5] /* 1190.4 */ },   /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCRBCPRREF
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCRBCPRREF
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCRBCPRREF[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCUIM0MND
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCUIM0MND
 *   ClockDomain_MSSCC_CLKSRCUIM1MND
 *   ClockDomain_MSSCC_CLKSRCUIM2MND
 *   ClockDomain_MSSCC_CLKSRCUIM3MND
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCUIM0MND[] =
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
  {    9511680, &ClockSource_XO,                          { 0,     2,     2477,  5000,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    9600000, &ClockSource_XO,                          { 0,     2,     1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    9909677, &ClockSource_GCC_GPLL0,                   { 1,     2,     256,   3875,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   10570323, &ClockSource_GCC_GPLL0,                   { 1,     2,     4096,  58125, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   11520000, &ClockSource_GCC_GPLL0,                   { 1,     2,     48,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   12684387, &ClockSource_GCC_GPLL0,                   { 1,     2,     4718,  55793, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   13212903, &ClockSource_GCC_GPLL0,                   { 1,     2,     1024,  11625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   14093763, &ClockSource_GCC_GPLL0,                   { 1,     2,     4818,  51278, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   14400000, &ClockSource_GCC_GPLL0,                   { 1,     2,     12,    125,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   14864516, &ClockSource_GCC_GPLL0,                   { 1,     2,     384,   3875,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   15360000, &ClockSource_GCC_GPLL0,                   { 1,     2,     64,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   15855484, &ClockSource_GCC_GPLL0,                   { 1,     2,     2048,  19375, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     8,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16516129, &ClockSource_GCC_GPLL0,                   { 1,     2,     256,   2325,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16912516, &ClockSource_GCC_GPLL0,                   { 1,     2,     772,   6847,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   17280000, &ClockSource_GCC_GPLL0,                   { 1,     2,     72,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   17617204, &ClockSource_GCC_GPLL0,                   { 1,     2,     4096,  34875, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   18000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     6,     50,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   19026581, &ClockSource_GCC_GPLL0,                   { 1,     2,     7034,  55454, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCUIMCARD0PREDIV
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCUIMCARD0PREDIV
 *   ClockDomain_MSSCC_CLKSRCUIMCARD1PREDIV
 *   ClockDomain_MSSCC_CLKSRCUIMCARD2PREDIV
 *   ClockDomain_MSSCC_CLKSRCUIMCARD3PREDIV
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCUIMCARD0PREDIV[] =
{
  {    3840000, &ClockSource_XO,                          { 0,     10,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
  {    4800000, &ClockSource_XO,                          { 0,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = XO */
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
 *   ClockDomain_GCC_GCCQUPV3WRAP0S6
 *   ClockDomain_GCC_GCCQUPV3WRAP0S7
 *   ClockDomain_GCC_GCCQUPV3WRAP1S0
 *   ClockDomain_GCC_GCCQUPV3WRAP1S1
 *   ClockDomain_GCC_GCCQUPV3WRAP1S2
 *   ClockDomain_GCC_GCCQUPV3WRAP1S3
 *   ClockDomain_GCC_GCCQUPV3WRAP1S4
 *   ClockDomain_GCC_GCCQUPV3WRAP1S5
 *   ClockDomain_GCC_GCCQUPV3WRAP1S6
 *   ClockDomain_GCC_GCCQUPV3WRAP1S7
 *   ClockDomain_GCC_GCCSECCTRL
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCQUPV3WRAP0S0[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 6,  300000000, &ClockSource_GCC_GPLL0,                  },
  { 7,  400000000, NULL,                                    },
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
 *   ClockDomain_GCC_GCCQUPV3WRAP0S6
 *   ClockDomain_GCC_GCCQUPV3WRAP0S7
 *   ClockDomain_GCC_GCCQUPV3WRAP1S0
 *   ClockDomain_GCC_GCCQUPV3WRAP1S1
 *   ClockDomain_GCC_GCCQUPV3WRAP1S2
 *   ClockDomain_GCC_GCCQUPV3WRAP1S3
 *   ClockDomain_GCC_GCCQUPV3WRAP1S4
 *   ClockDomain_GCC_GCCQUPV3WRAP1S5
 *   ClockDomain_GCC_GCCQUPV3WRAP1S6
 *   ClockDomain_GCC_GCCQUPV3WRAP1S7
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

static ClockDomainNodeType ClockDomain_GCC_GCCAGGRENOC =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_AGGRE_NOC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCCE1 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_CE1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCCONFIGNOC =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_CONFIG_NOC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCMSSQ6MEMNOCAXI =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_MSS_Q6_MEMNOC_AXI_CMD_RCGR),
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

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S6 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP0_S6_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP0S7 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP0_S7_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S0 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP1_S0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S1 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP1_S1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S2 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP1_S2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S3 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP1_S3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S4 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP1_S4_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S5 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP1_S5_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S6 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP1_S6_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1S7 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP1_S7_CMD_RCGR),
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
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_SEC_CTRL_CMD_RCGR),
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

static ClockDomainNodeType ClockDomain_GCC_GCCSYSNOCSFAXI =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_SYS_NOC_SF_AXI_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCBITCOXMMND =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCBITCOXMMND,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCBITCOXMMND),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_BIT_COXM_MND_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCBUSMSSCONFIG,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCBUSMSSCONFIG),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_BUS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCQ6 =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCQ6,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCQ6),
  .pRail     = &ClockRail_VDDMSSQ6,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_QDSP6SS_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_mMSSQ6ClockDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCRBCPRREF =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCRBCPRREF,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCRBCPRREF),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_RBCPR_REF_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCRFFETP =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_RFFE_TP_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIM0MND =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCUIM0MND,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCUIM0MND),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_UIM0_MND_CMD_RCGR),
    .pmControl  = &HAL_clk_mMSSUIMClockDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIM1MND =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCUIM0MND,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCUIM0MND),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_UIM1_MND_CMD_RCGR),
    .pmControl  = &HAL_clk_mMSSUIMClockDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIM2MND =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCUIM0MND,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCUIM0MND),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_UIM2_MND_CMD_RCGR),
    .pmControl  = &HAL_clk_mMSSUIMClockDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIM3MND =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCUIM0MND,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCUIM0MND),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_UIM3_MND_CMD_RCGR),
    .pmControl  = &HAL_clk_mMSSUIMClockDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIMCARD0PREDIV =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCUIMCARD0PREDIV,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCUIMCARD0PREDIV),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_UIM0_SRC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIMCARD1PREDIV =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCUIMCARD0PREDIV,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCUIMCARD0PREDIV),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_UIM1_SRC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIMCARD2PREDIV =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCUIMCARD0PREDIV,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCUIMCARD0PREDIV),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_UIM2_SRC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIMCARD3PREDIV =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCUIMCARD0PREDIV,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCUIMCARD0PREDIV),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_UIM3_SRC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};


static ClockDomainNodeType *aDomains[] =
{
  &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
  &ClockDomain_GCC_CLOCKDRIVERGENXO,
  &ClockDomain_GCC_GCCAGGRENOC,
  &ClockDomain_GCC_GCCCE1,
  &ClockDomain_GCC_GCCCONFIGNOC,
  &ClockDomain_GCC_GCCMSSQ6MEMNOCAXI,
  &ClockDomain_GCC_GCCQUPV3WRAP0CORE2X,
  &ClockDomain_GCC_GCCQUPV3WRAP0S0,
  &ClockDomain_GCC_GCCQUPV3WRAP0S1,
  &ClockDomain_GCC_GCCQUPV3WRAP0S2,
  &ClockDomain_GCC_GCCQUPV3WRAP0S3,
  &ClockDomain_GCC_GCCQUPV3WRAP0S4,
  &ClockDomain_GCC_GCCQUPV3WRAP0S5,
  &ClockDomain_GCC_GCCQUPV3WRAP0S6,
  &ClockDomain_GCC_GCCQUPV3WRAP0S7,
  &ClockDomain_GCC_GCCQUPV3WRAP1S0,
  &ClockDomain_GCC_GCCQUPV3WRAP1S1,
  &ClockDomain_GCC_GCCQUPV3WRAP1S2,
  &ClockDomain_GCC_GCCQUPV3WRAP1S3,
  &ClockDomain_GCC_GCCQUPV3WRAP1S4,
  &ClockDomain_GCC_GCCQUPV3WRAP1S5,
  &ClockDomain_GCC_GCCQUPV3WRAP1S6,
  &ClockDomain_GCC_GCCQUPV3WRAP1S7,
  &ClockDomain_GCC_GCCSECCTRL,
  &ClockDomain_GCC_GCCSYSNOC,
  &ClockDomain_GCC_GCCSYSNOCSFAXI,
  &ClockDomain_MSSCC_CLKSRCBITCOXMMND,
  &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
  &ClockDomain_MSSCC_CLKSRCQ6,
  &ClockDomain_MSSCC_CLKSRCRBCPRREF,
  &ClockDomain_MSSCC_CLKSRCRFFETP,
  &ClockDomain_MSSCC_CLKSRCUIM0MND,
  &ClockDomain_MSSCC_CLKSRCUIM1MND,
  &ClockDomain_MSSCC_CLKSRCUIM2MND,
  &ClockDomain_MSSCC_CLKSRCUIM3MND,
  &ClockDomain_MSSCC_CLKSRCUIMCARD0PREDIV,
  &ClockDomain_MSSCC_CLKSRCUIMCARD1PREDIV,
  &ClockDomain_MSSCC_CLKSRCUIMCARD2PREDIV,
  &ClockDomain_MSSCC_CLKSRCUIMCARD3PREDIV,
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
    .szName    = "clk_xo_mdm",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xA766F3C6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_XO_MDM_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 576,
    },
  },
  {
    .szName    = "clk_xo_nav",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xCEF773AE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_XO_NAV_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 832,
    },
  },
  {
    .szName    = "clk_xo_rscc",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x00D04438,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_XO_RSCC_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 108,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_rx3_qlink_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x72FEC8BD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_RX3_QLINK_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCAGGRENOC                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_axi_crypto",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = NULL,
    .nUniqueId = 0x3469050D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_AXI_CRYPTO_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "clk_axi_dma",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x64A18293,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_AXI_DMA_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 640,
    },
  },
  {
    .szName    = "gcc_mss_snoc_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xEE9AF65A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_SNOC_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 308,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_1_m_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x728861DF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP_1_M_AHB_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x100000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 130,
    },
  },
  {
    .szName    = "gcc_wcss_axi_m_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x0100A916,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_WCSS_AXI_M_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 316,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCCE1                                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ce1_clk",
    .pDomain   = &ClockDomain_GCC_GCCCE1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x3DF99368,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_CE1_CBCR), 0, {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x20} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 167,
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
      .nDebugMuxSel = 148,
    },
  },
  {
    .szName    = "gcc_ce1_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5BEC4031,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_CE1_AHB_CBCR), 0, {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x8} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 169,
    },
  },
  {
    .szName    = "gcc_mss_cfg_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xDEAD4F3F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_CFG_AHB_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 301,
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
      .nDebugMuxSel = 143,
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
      .nDebugMuxSel = 117,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_1_s_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x3428967E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP_1_S_AHB_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x200000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 131,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCMSSQ6MEMNOCAXI                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ddrss_mss_q6_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCMSSQ6MEMNOCAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5A494B01,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_DDRSS_MSS_Q6_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 185,
    },
  },
  {
    .szName    = "gcc_mss_q6_memnoc_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCMSSQ6MEMNOCAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x8A676C8D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_Q6_MEMNOC_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 309,
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
      .nDebugMuxSel = 119,
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
      .nDebugMuxSel = 118,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap1_core_2x_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xD7DB82C0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_CORE_2X_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x40000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 128,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap1_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x40B685B0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_CORE_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x80000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 129,
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
      .nDebugMuxSel = 120,
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
      .nDebugMuxSel = 121,
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
      .nDebugMuxSel = 122,
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
      .nDebugMuxSel = 123,
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
      .nDebugMuxSel = 124,
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
      .nDebugMuxSel = 125,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S6                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s6_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S6,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x917A11AD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP0_S6_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x10000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 126,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP0S7                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap0_s7_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP0S7,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x69D7DBAF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP0_S7_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x20000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 127,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S0                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s0_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA40B3E1C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_S0_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x400000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 132,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S1                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s1_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xDE8BDAAC,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_S1_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x800000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 133,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S2                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s2_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S2,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x45AB34F6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_S2_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x1000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 134,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S3                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s3_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S3,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x6CBC20FD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_S3_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x2000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 135,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S4                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s4_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S4,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x2A814B46,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_S4_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x4000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 136,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S5                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s5_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S5,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x8279058C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_S5_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x8000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 137,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S6                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s6_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S6,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x6FB2266A,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_S6_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x10000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 138,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1S7                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_s7_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1S7,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x90473589,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_S7_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x20000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 139,
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
  /* ClockDomain_GCC_GCCSYSNOC                                             */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_axi_nav",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x3B297F20,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_AXI_NAV_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 768,
    },
  },
  {
    .szName    = "gcc_ce1_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x96B77A7C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_CE1_AXI_CBCR), 0, {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x10} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 168,
    },
  },
  {
    .szName    = "gcc_mss_nav_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x0B672D67,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_NAV_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 304,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_0_m_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x79AD882E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP_0_M_AHB_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x40} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 116,
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
      .nDebugMuxSel = 315,
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
      .nDebugMuxSel = 317,
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
      .nDebugMuxSel = 318,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSYSNOCSFAXI                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_axi_mnoc",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCSFAXI,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x7AC37B11,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_AXI_MNOC_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 448,
    },
  },
  {
    .szName    = "clk_axi_mnoc_q6",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCSFAXI,
    .pDebugMux = NULL,
    .nUniqueId = 0xBFB59914,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "clk_axi_nav_sample",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCSFAXI,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x56DD2F9D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_AXI_NAV_SAMPLE_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 1088,
    },
  },
  {
    .szName    = "gcc_mss_axis2_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCSFAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5EC28930,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_AXIS2_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 303,
    },
  },
  {
    .szName    = "gcc_mss_mfab_axis_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCSFAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x56446044,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_MFAB_AXIS_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 302,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCBITCOXMMND                                    */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_bit_coxm",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBITCOXMMND,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xF52B3137,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BIT_COXM_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 67,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG                                  */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_bus_config",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x767F81C3,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 73,
    },
  },
  {
    .szName    = "clk_bus_coxm",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x766806AD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_COXM_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 76,
    },
  },
  {
    .szName    = "clk_bus_crypto",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x3AA07296,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_CRYPTO_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 77,
    },
  },
  {
    .szName    = "clk_bus_mgpi",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xB96A1818,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_MGPI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 78,
    },
  },
  {
    .szName    = "clk_bus_mvc",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x02594FA9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 79,
    },
  },
  {
    .szName    = "clk_bus_nav",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x742FC2A8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_NAV_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 704,
    },
  },
  {
    .szName    = "clk_bus_pll_outctrl",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xA2E6C085,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 74,
    },
  },
  {
    .szName    = "clk_bus_q6",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xAFA015A1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_Q6SS_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 384,
    },
  },
  {
    .szName    = "clk_bus_rbcpr",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x46B4DBCC,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_RBCPR_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 75,
    },
  },
  {
    .szName    = "clk_bus_rfc",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xA383C545,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_RFC_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 119,
    },
  },
  {
    .szName    = "clk_bus_rscc",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x8A227677,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_RSCC_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 87,
    },
  },
  {
    .szName    = "clk_bus_slave_timeout",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xE6CBF2AA,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_SLAVE_TIMEOUT_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 82,
    },
  },
  {
    .szName    = "clk_bus_stmr",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x1CDCA95E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_STMR_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 81,
    },
  },
  {
    .szName    = "clk_bus_uim0",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x3E9C3672,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_UIM0_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 83,
    },
  },
  {
    .szName    = "clk_bus_uim1",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xACCC33FE,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_UIM1_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 84,
    },
  },
  {
    .szName    = "clk_bus_uim2",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x61429FC4,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_UIM2_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 85,
    },
  },
  {
    .szName    = "clk_bus_uim3",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xCD558D97,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_BUS_UIM3_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 86,
    },
  },
  {
    .szName    = "mss_bus_alt_clk",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x32E98D9B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 103,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCQ6                                            */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_q6",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCQ6,
    .pDebugMux = &ClockDebugMux_MSSCCQ6,
    .nUniqueId = 0x7722DE95,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 1,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCRBCPRREF                                      */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_rbcpr_ref",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCRBCPRREF,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xC3B54B78,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_RBCPR_REF_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 68,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCRFFETP                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_mss_rffe_tp",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCRFFETP,
    .pDebugMux = NULL,
    .nUniqueId = 0xEA06AB7F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_RFFE_TP_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCUIM0MND                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_uart_bit_uim0",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCUIM0MND,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x81B7D387,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_UART_BIT_UIM0_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 99,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCUIM1MND                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_uart_bit_uim1",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCUIM1MND,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x03D72FCE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_UART_BIT_UIM1_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 100,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCUIM2MND                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_uart_bit_uim2",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCUIM2MND,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x304DA3F7,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_UART_BIT_UIM2_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 101,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCUIM3MND                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_uart_bit_uim3",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCUIM3MND,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xF645BFD9,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_UART_BIT_UIM3_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 102,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCUIMCARD0PREDIV                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_card_src_uim0",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCUIMCARD0PREDIV,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x0F12C218,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CARD_SRC_UIM0_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 94,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCUIMCARD1PREDIV                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_card_src_uim1",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCUIMCARD1PREDIV,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x7F765B7B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CARD_SRC_UIM1_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 95,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCUIMCARD2PREDIV                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_card_src_uim2",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCUIMCARD2PREDIV,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x3101D6BB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CARD_SRC_UIM2_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 97,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCUIMCARD3PREDIV                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_card_src_uim3",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCUIMCARD3PREDIV,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x6464FC33,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CARD_SRC_UIM3_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 98,
    },
  },
};


/*=========================================================================
      Power Domains
==========================================================================*/

static ClockPowerDomainNodeType aPowerDomains[] =
{
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
    .data       = (npa_user_data) &(ClockNPACtxtType){ "clk_bus_config" },
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
    75000,         /* /clk/mss/config_bus */
  },
};


/*=========================================================================
      Main BSP
==========================================================================*/

ClockBSPType ClockBSP =
{
  .aBases            = NULL,
  .nNumBases         = 0,
  .aRails            = aRails,
  .nNumRails         = SIZEOF_ARRAY(aRails),
  .aClocks           = aClocks,
  .nNumClocks        = SIZEOF_ARRAY(aClocks),
  .aDomains          = aDomains,
  .nNumDomains       = SIZEOF_ARRAY(aDomains),
  .aSources          = aSources,
  .nNumSources       = SIZEOF_ARRAY(aSources),
  .aDividers         = NULL,
  .nNumDividers      = 0,
  .aPowerDomains     = aPowerDomains,
  .nNumPowerDomains  = SIZEOF_ARRAY(aPowerDomains),
  .aDebugMuxes       = aDebugMuxes,
  .nNumDebugMuxes    = SIZEOF_ARRAY(aDebugMuxes),
  .nFlags            = CLOCK_GLOBAL_FLAG_LOG_CLOCK_FREQ_CHANGE |
                       CLOCK_GLOBAL_FLAG_LOG_SOURCE_STATE_CHANGE,
  .SafeMuxConfig     = { 0, 2, 0, 0 },
  .pGenericNPANode   = &GenericNPANode,
};

