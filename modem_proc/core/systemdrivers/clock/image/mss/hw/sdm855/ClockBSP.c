/*
==============================================================================

FILE:         ClockBSP.c

DESCRIPTION:
  This file contains the clock driver BSP data.

  Auto-generated from the following IPCat controller releases:

    SDM855v1 (hana_1.0):
      GCC       : z_hana_clk_ctl_p3q1r12 [hana_gcc.r3]
      MSS_CC    : z_hercules_a_mss_7ff_1.0_hana_p3q2r20.687 [hana_mss_cc.r2]
      NAV_CC    : nav_ss_hana_1.0_p3q2r5.6 [hana_nav_cc.r1]


==============================================================================

$Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sdm855/ClockBSP.c#1 $

==============================================================================
            Copyright (c) 2017 QUALCOMM Technologies Incorporated.
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
extern HAL_clk_DividerControlType         HAL_clk_GenericDividerControl;
extern HAL_clk_PowerDomainControlType     HAL_clk_GenericPowerDomainControl;
extern HAL_clk_SourceControlType          HAL_clk_TrionPLLControl;

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
extern HAL_clk_ClockDomainControlType     HAL_clk_mMSSQ6ClockDomainControl;
extern HAL_clk_SourceControlType          HAL_clk_GPLLStubPLLControl;

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

static ClockRailType ClockRail_VDDMSS =
{
  .szName = "/vcs/vdd_mss",
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
  &ClockRail_VDDMSS,
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
  .nParentSel   = 340,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlMSSCC,
  },
};

static ClockDebugMuxType ClockDebugMux_NAVCC =
{
  .szName       = "nav_cc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 372,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlNAVCC,
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
  &ClockDebugMux_NAVCC,
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
      Source BSP
==========================================================================*/

/*
 * ClockSourceBSP_GCC_GPLL0
 *
 * Used by:
 */
static const ClockSourceFreqConfigType ClockSourceBSP_GCC_GPLL0[] =
{
  {  600000000, &ClockSource_XO, { 0, 0, 1, 31,  0x00004000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Trion.nEven = 2}             }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
};

/*
 * ClockSourceBSP_MSSCC_MPSSPLL
 *
 * Used by:
 *   ClockSource_MSSCC_MPSSPLL
 */
static const ClockSourceFreqConfigType ClockSourceBSP_MSSCC_MPSSPLL[] =
{
  {  384000000, &ClockSource_XO, { 0, 0, 1, 20,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
};

/*
 * ClockSourceBSP_MSSCC_OFFLINEPLL
 *
 * Used by:
 *   ClockSource_MSSCC_OFFLINEPLL
 */
static const ClockSourceFreqConfigType ClockSourceBSP_MSSCC_OFFLINEPLL[] =
{
  {  307200000, &ClockSource_XO, { 0, 0, 1, 16,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  422400000, &ClockSource_XO, { 0, 0, 1, 22,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  556800000, &ClockSource_XO, { 0, 0, 1, 29,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  672000000, &ClockSource_XO, { 0, 0, 1, 35,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  844800000, &ClockSource_XO, { 0, 0, 1, 44,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  960000000, &ClockSource_XO, { 0, 0, 1, 50,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS,        },
  { 1075200000, &ClockSource_XO, { 0, 0, 1, 56,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  { 1190400000, &ClockSource_XO, { 0, 0, 1, 62,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
};
static const ClockSourceFreqConfigType ClockSourceBSPCal_MSSCC_OFFLINEPLL[] =
{
  {  748800000, &ClockSource_XO, { 0, 0, 1, 39,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS,        },
};

/*
 * ClockSourceBSP_MSSCC_QDSP6SSPLL
 *
 * Used by:
 *   ClockSource_MSSCC_QDSP6SSPLL
 */
static const ClockSourceFreqConfigType ClockSourceBSP_MSSCC_QDSP6SSPLL[] =
{
  {  364800000, &ClockSource_XO, { 0, 0, 1, 19,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  576000000, &ClockSource_XO, { 0, 0, 1, 30,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
  {  768000000, &ClockSource_XO, { 0, 0, 1, 40,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS,        },
  {  960000000, &ClockSource_XO, { 0, 0, 1, 50,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS,        },
  { 1171200000, &ClockSource_XO, { 0, 0, 1, 61,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  { 1324800000, &ClockSource_XO, { 0, 0, 1, 69,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
  { 1497600000, &ClockSource_XO, { 0, 0, 1, 78,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },
};
static const ClockSourceFreqConfigType ClockSourceBSPCal_MSSCC_QDSP6SSPLL[] =
{
  {  921600000, &ClockSource_XO, { 0, 0, 1, 48,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_SVS,        },
};

/*
 * ClockSourceBSP_NAVCC_NAVCCPLL4PLL
 *
 * Used by:
 *   ClockSource_NAVCC_NAVCCPLL4PLL
 */
static const ClockSourceFreqConfigType ClockSourceBSP_NAVCC_NAVCCPLL4PLL[] =
{
  {  408000000, &ClockSource_XO, { 0, 0, 1, 21,  0x00004000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                          }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
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
};

static ClockSourceNodeType ClockSource_MSSCC_MPSSPLL =
{
  .szName       = "mpss_pll",
  .nFlags       = 0x0,
  .aBSP         = ClockSourceBSP_MSSCC_MPSSPLL,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_MSSCC_MPSSPLL),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = 0x0,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_MPSS_PLL_MODE),
    .VoterRegister  = {0, 0},
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
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
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_MSSCC_OFFLINEPLL,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_MSSCC_OFFLINEPLL),
};

static ClockSourceNodeType ClockSource_MSSCC_QDSP6SSPLL =
{
  .szName       = "qdsp6ss_pll",
  .nFlags       = CLOCK_FLAG_SUPPORTS_SLEWING,
  .aBSP         = ClockSourceBSP_MSSCC_QDSP6SSPLL,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_MSSCC_QDSP6SSPLL),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_QDSP6SS_PLL_MODE),
    .VoterRegister  = {0, 0},
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_MSSCC_QDSP6SSPLL,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_MSSCC_QDSP6SSPLL),
};

static ClockSourceNodeType ClockSource_NAVCC_NAVCCPLL4PLL =
{
  .szName       = "nav_cc_pll4_pll",
  .nFlags       = 0x0,
  .aBSP         = ClockSourceBSP_NAVCC_NAVCCPLL4PLL,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_NAVCC_NAVCCPLL4PLL),
  .pRail        = &ClockRail_VDDMSS,
  .nConfigMask  = 0x0,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_NAV_CC_PLL4_PLL_MODE),
    .VoterRegister  = {0, 0},
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
};


static ClockSourceNodeType *aSources[] =
{
  &ClockSource_GCC_GPLL0,
  &ClockSource_MSSCC_MPSSPLL,
  &ClockSource_MSSCC_OFFLINEPLL,
  &ClockSource_MSSCC_QDSP6SSPLL,
  &ClockSource_NAVCC_NAVCCPLL4PLL,
  &ClockSource_XO,
};


/*=========================================================================
      Dividers
==========================================================================*/

static ClockDividerNodeType aDividers[] =
{
  {
    .szName    = "gcc_mss_pll0_main_div_clk_src",
    .nFlags    = 0x0,
    .HALDivider = {
      .nAddr       = HWIO_ADDR(GCC_MSS_PLL0_MAIN_DIV_CDIVR),
      .pmControl   = &HAL_clk_GenericDividerControl,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap0_core_clk_src",
    .nFlags    = 0x0,
    .HALDivider = {
      .nAddr       = HWIO_ADDR(GCC_QUPV3_WRAP0_CORE_CDIVR),
      .pmControl   = &HAL_clk_GenericDividerControl,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap1_core_clk_src",
    .nFlags    = 0x0,
    .HALDivider = {
      .nAddr       = HWIO_ADDR(GCC_QUPV3_WRAP1_CORE_CDIVR),
      .pmControl   = &HAL_clk_GenericDividerControl,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap2_core_clk_src",
    .nFlags    = 0x0,
    .HALDivider = {
      .nAddr       = HWIO_ADDR(GCC_QUPV3_WRAP2_CORE_CDIVR),
      .pmControl   = &HAL_clk_GenericDividerControl,
    },
  },
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
 *   ClockDomain_GCC_GCCQUPV3WRAP2S0
 *   ClockDomain_GCC_GCCQUPV3WRAP2S1
 *   ClockDomain_GCC_GCCQUPV3WRAP2S2
 *   ClockDomain_GCC_GCCQUPV3WRAP2S3
 *   ClockDomain_GCC_GCCQUPV3WRAP2S4
 *   ClockDomain_GCC_GCCQUPV3WRAP2S5
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCQUPV3WRAP0S0[] =
{
  {    7372800, &ClockSource_GCC_GPLL0,                   { 6,     2,     384,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   14745600, &ClockSource_GCC_GPLL0,                   { 6,     2,     768,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0x00 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   29491200, &ClockSource_GCC_GPLL0,                   { 6,     2,     1536,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   32000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     8,     75,    0x01 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   48000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     4,     25,    0x02 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
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
 * ClockDomainBSP_GCC_GCCQUPV3WRAP1CORE2X
 *
 * Note: These frequencies are used by domains possibly under DFS control. The
 *       DFS-enable state will be detected during init. If under DFS control,
 *       the domain will dynamically allocate its own BSP array and populate
 *       it with configurations detected from the DFS registers.
 *
 * Used by:
 *   ClockDomain_GCC_GCCQUPV3WRAP1CORE2X
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCQUPV3WRAP1CORE2X[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0x00 }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 6,     12,    0,     0,     0x02 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 1,     12,    0,     0,     0x03 }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  150000000, &ClockSource_GCC_GPLL0,                   { 1,     8,     0,     0,     0x04 }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  200000000, &ClockSource_GCC_GPLL0,                   { 1,     6,     0,     0,     0x05 }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCSECCTRL
 *
 * Used by:
 *   ClockDomain_GCC_GCCSECCTRL
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCSECCTRL[] =
{
  {    4800000, &ClockSource_XO,                          { 0,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCBITCOXMMND
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCBITCOXMMND
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCBITCOXMMND[] =
{
  {    1843200, &ClockSource_GCC_GPLL0,                   { 1,     2,     192,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {    7372800, &ClockSource_GCC_GPLL0,                   { 1,     2,     768,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     8,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   32000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     16,    75,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   40000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     4,     15,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   48000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     8,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   51200000, &ClockSource_GCC_GPLL0,                   { 1,     2,     128,   375,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   60000000, &ClockSource_GCC_GPLL0,                   { 1,     5,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCBUSMSSCONFIG
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCBUSMSSCONFIG[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {   75000000, &ClockSource_GCC_GPLL0,                   { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {  150000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCQ6
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCQ6
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCQ6[] =
{
  {  364800000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    {{ 0 }}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 0] /* 364.0  */ },  /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
  {  576000000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{ 0 }}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 1] /* 576.0  */ },  /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
  {  768000000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{ 0 }}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 2] /* 768.0  */ },  /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
  {  960000000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{ 0 }}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 3] /* 960.0  */ },  /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
  { 1171200000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{ 0 }}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 4] /* 1171.0 */ },  /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
  { 1324800000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{ 0 }}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 5] /* 1324.0 */ },  /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
  { 1497600000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{ 0 }}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 6] /* 1497.0 */ },  /* Mux[1] = QDSP6SS_PLL_OUT_EVEN */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCRFFE
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCRFFE
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCRFFE[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {   38400000, &ClockSource_MSSCC_MPSSPLL,               { 1,     20,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = MPSS_PLL_OUT_MAIN */
  {   76800000, &ClockSource_MSSCC_MPSSPLL,               { 1,     10,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = MPSS_PLL_OUT_MAIN */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCSERDES
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCSERDES
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCSERDES[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {   96000000, &ClockSource_MSSCC_MPSSPLL,               { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = MPSS_PLL_OUT_MAIN */
  {  384000000, &ClockSource_MSSCC_MPSSPLL,               { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = MPSS_PLL_OUT_MAIN */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCUIM0
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCUIM0
 *   ClockDomain_MSSCC_CLKSRCUIM1
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCUIM0[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCUIM0MND
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCUIM0MND
 *   ClockDomain_MSSCC_CLKSRCUIM1MND
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCUIM0MND[] =
{
  {    4807680, &ClockSource_XO,                          { 0,     2,     313,   1250,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    4953600, &ClockSource_XO,                          { 0,     2,     129,   500,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    4961280, &ClockSource_XO,                          { 0,     2,     323,   1250,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    5120000, &ClockSource_XO,                          { 0,     2,     4,     15,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    5283840, &ClockSource_XO,                          { 0,     2,     172,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    5285120, &ClockSource_XO,                          { 0,     2,     4129,  15000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    5760000, &ClockSource_XO,                          { 0,     2,     3,     10,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    5775360, &ClockSource_XO,                          { 0,     2,     188,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    6328320, &ClockSource_XO,                          { 0,     2,     206,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    6343680, &ClockSource_XO,                          { 0,     2,     413,   1250,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    6400000, &ClockSource_XO,                          { 0,     2,     1,     3,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    6604800, &ClockSource_XO,                          { 0,     2,     43,    125,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    6606080, &ClockSource_XO,                          { 0,     2,     5161,  15000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    6606400, &ClockSource_XO,                          { 0,     2,     4129,  12000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    6606720, &ClockSource_XO,                          { 0,     2,     3441,  10000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    6607360, &ClockSource_XO,                          { 0,     2,     2581,  7500,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7045120, &ClockSource_XO,                          { 0,     2,     688,   1875,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7046400, &ClockSource_XO,                          { 0,     2,     367,   1000,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7046880, &ClockSource_XO,                          { 0,     2,     14681, 40000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7047040, &ClockSource_XO,                          { 0,     2,     11011, 30000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7047680, &ClockSource_XO,                          { 0,     2,     2753,  7500,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7188480, &ClockSource_XO,                          { 0,     2,     234,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7200000, &ClockSource_XO,                          { 0,     2,     3,     8,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7201280, &ClockSource_XO,                          { 0,     2,     2813,  7500,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7203840, &ClockSource_XO,                          { 0,     2,     469,   1250,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7432320, &ClockSource_XO,                          { 0,     2,     3871,  10000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7680000, &ClockSource_XO,                          { 0,     2,     2,     5,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7925760, &ClockSource_XO,                          { 0,     2,     258,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7927680, &ClockSource_XO,                          { 0,     2,     4129,  10000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7928320, &ClockSource_XO,                          { 0,     2,     3097,  7500,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    7929600, &ClockSource_XO,                          { 0,     2,     413,   1000,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    8000000, &ClockSource_XO,                          { 0,     2,     5,     12,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    8257920, &ClockSource_XO,                          { 0,     2,     4301,  10000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    8258560, &ClockSource_XO,                          { 0,     2,     1613,  3750,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    8455680, &ClockSource_XO,                          { 0,     2,     1101,  2500,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    8456320, &ClockSource_XO,                          { 0,     2,     13213, 30000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    8458240, &ClockSource_XO,                          { 0,     2,     826,   1875,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    8640000, &ClockSource_XO,                          { 0,     2,     9,     20,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    8806400, &ClockSource_XO,                          { 0,     2,     172,   375,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    8808320, &ClockSource_XO,                          { 0,     2,     13763, 30000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    8808640, &ClockSource_XO,                          { 0,     2,     27527, 60000, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    8808960, &ClockSource_XO,                          { 0,     2,     1147,  2500,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    9000000, &ClockSource_XO,                          { 0,     2,     15,    32,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    9000960, &ClockSource_XO,                          { 0,     2,     293,   625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    9511680, &ClockSource_XO,                          { 0,     2,     2477,  5000,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    9600000, &ClockSource_XO,                          { 0,     2,     1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
  {    9909677, &ClockSource_GCC_GPLL0,                   { 1,     2,     256,   3875,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   10570323, &ClockSource_GCC_GPLL0,                   { 1,     2,     4096,  58125, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   11520000, &ClockSource_GCC_GPLL0,                   { 1,     2,     48,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   12684387, &ClockSource_GCC_GPLL0,                   { 1,     2,     4718,  55793, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   13212903, &ClockSource_GCC_GPLL0,                   { 1,     2,     1024,  11625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   14093763, &ClockSource_GCC_GPLL0,                   { 1,     2,     4818,  51278, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   14400000, &ClockSource_GCC_GPLL0,                   { 1,     2,     12,    125,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   14864516, &ClockSource_GCC_GPLL0,                   { 1,     2,     384,   3875,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   15360000, &ClockSource_GCC_GPLL0,                   { 1,     2,     64,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   15855484, &ClockSource_GCC_GPLL0,                   { 1,     2,     2048,  19375, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     8,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16516129, &ClockSource_GCC_GPLL0,                   { 1,     2,     256,   2325,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16912516, &ClockSource_GCC_GPLL0,                   { 1,     2,     772,   6847,  0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   17280000, &ClockSource_GCC_GPLL0,                   { 1,     2,     72,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   17617204, &ClockSource_GCC_GPLL0,                   { 1,     2,     4096,  34875, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   18000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     6,     50,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   19026581, &ClockSource_GCC_GPLL0,                   { 1,     2,     7034,  55454, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
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
 *   ClockDomain_GCC_GCCQUPV3WRAP2S0
 *   ClockDomain_GCC_GCCQUPV3WRAP2S1
 *   ClockDomain_GCC_GCCQUPV3WRAP2S2
 *   ClockDomain_GCC_GCCQUPV3WRAP2S3
 *   ClockDomain_GCC_GCCQUPV3WRAP2S4
 *   ClockDomain_GCC_GCCQUPV3WRAP2S5
 *   ClockDomain_GCC_GCCSECCTRL
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCQUPV3WRAP0S0[] =
{

  { 0,   19200000, &ClockSource_XO },
  { 1,  600000000, &ClockSource_GCC_GPLL0 },
  { 6,  300000000, &ClockSource_GCC_GPLL0 },
  { 7,  400000000, NULL },
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
 *   ClockDomain_GCC_GCCQUPV3WRAP2S0
 *   ClockDomain_GCC_GCCQUPV3WRAP2S1
 *   ClockDomain_GCC_GCCQUPV3WRAP2S2
 *   ClockDomain_GCC_GCCQUPV3WRAP2S3
 *   ClockDomain_GCC_GCCQUPV3WRAP2S4
 *   ClockDomain_GCC_GCCQUPV3WRAP2S5
 */
static const ClockFMaxType ClockDomainFMax_GCC_GCCQUPV3WRAP0S0[] =
{
  {   50000000, RAIL_VOLTAGE_LEVEL_MIN_SVS },
  {   75000000, RAIL_VOLTAGE_LEVEL_LOW_SVS },
  {  100000000, RAIL_VOLTAGE_LEVEL_SVS },
  {  128000000, RAIL_VOLTAGE_LEVEL_NOM },
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

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1CORE2X =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP1CORE2X,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP1CORE2X),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP1_CORE_2X_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
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

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2CORE2X =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP2_CORE_2X_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S0 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP2_S0_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S1 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP2_S1_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S2 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP2_S2_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S3 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP2_S3_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S4 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP2_S4_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
    .nDFSStates = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP2S5 =
{
  .aBSP      = ClockDomainBSP_GCC_GCCQUPV3WRAP0S0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCQUPV3WRAP0S0),
  .pRail     = &ClockRail_VDDCX,
  .aFMax     = ClockDomainFMax_GCC_GCCQUPV3WRAP0S0,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_QUPV3_WRAP2_S5_CMD_RCGR),
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

static ClockDomainNodeType ClockDomain_GCC_GCCSYSNOCMONAQAXI =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = 0,
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
    .nCGRAddr   = 0,
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
    .nCGRAddr   = 0,
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
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_mMSSQ6ClockDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCRFFE =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCRFFE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCRFFE),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCSERDES =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCSERDES,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCSERDES),
  .pRail     = &ClockRail_VDDMSS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIM0 =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCUIM0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCUIM0),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = 0,
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
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIM1 =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCUIM0,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCUIM0),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
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
    .nCGRAddr   = 0,
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
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
  &ClockDomain_GCC_GCCQUPV3WRAP1CORE2X,
  &ClockDomain_GCC_GCCQUPV3WRAP1S0,
  &ClockDomain_GCC_GCCQUPV3WRAP1S1,
  &ClockDomain_GCC_GCCQUPV3WRAP1S2,
  &ClockDomain_GCC_GCCQUPV3WRAP1S3,
  &ClockDomain_GCC_GCCQUPV3WRAP1S4,
  &ClockDomain_GCC_GCCQUPV3WRAP1S5,
  &ClockDomain_GCC_GCCQUPV3WRAP2CORE2X,
  &ClockDomain_GCC_GCCQUPV3WRAP2S0,
  &ClockDomain_GCC_GCCQUPV3WRAP2S1,
  &ClockDomain_GCC_GCCQUPV3WRAP2S2,
  &ClockDomain_GCC_GCCQUPV3WRAP2S3,
  &ClockDomain_GCC_GCCQUPV3WRAP2S4,
  &ClockDomain_GCC_GCCQUPV3WRAP2S5,
  &ClockDomain_GCC_GCCSECCTRL,
  &ClockDomain_GCC_GCCSYSNOC,
  &ClockDomain_GCC_GCCSYSNOCMONAQAXI,
  &ClockDomain_GCC_GCCSYSNOCSFAXI,
  &ClockDomain_MSSCC_CLKSRCBITCOXMMND,
  &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
  &ClockDomain_MSSCC_CLKSRCQ6,
  &ClockDomain_MSSCC_CLKSRCRFFE,
  &ClockDomain_MSSCC_CLKSRCSERDES,
  &ClockDomain_MSSCC_CLKSRCUIM0,
  &ClockDomain_MSSCC_CLKSRCUIM0MND,
  &ClockDomain_MSSCC_CLKSRCUIM1,
  &ClockDomain_MSSCC_CLKSRCUIM1MND,
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
    .szName    = "clk_xo_cx",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xD159A328,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_XO_CX_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 5,
    },
  },
  {
    .szName    = "clk_xo_rscc",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENXO,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x00D04438,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_XO_RSCC_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 44,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_2xo_rffe",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x73289FFF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_2XO_RFFE_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 28,
    },
  },
  {
    .szName    = "clk_axi_crypto",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x3469050D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_AXI_CRYPTO_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 6,
    },
  },
  {
    .szName    = "clk_axi_offline",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x9980DC57,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_AXI_OFFLINE_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 4,
    },
  },
  {
    .szName    = "clk_xo_rffe",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x3367B789,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_XO_RFFE_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 25,
    },
  },
  {
    .szName    = "gcc_rx2_qlink_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x5648B23F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_RX2_QLINK_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },
  {
    .szName    = "gcc_rx3_modem_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x7C2BB787,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_RX3_MODEM_CLKREF_EN), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCAGGRENOC                                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ce1_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x96B77A7C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_CE1_AXI_CBCR), 0, {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x10} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 181,
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
      .nDebugMuxSel = 338,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_0_m_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x79AD882E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP_0_M_AHB_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_0_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x40} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 130,
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
      .nDebugMuxSel = 142,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_2_m_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCAGGRENOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xFBEA7E74,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP_2_M_AHB_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x4} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 385,
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
      .nDebugMuxSel = 346,
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
      .nDebugMuxSel = 180,
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
      .nDebugMuxSel = 160,
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
      .nDebugMuxSel = 182,
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
      .nDebugMuxSel = 332,
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
      .nDebugMuxSel = 155,
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
      .nDebugMuxSel = 131,
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
      .nDebugMuxSel = 143,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_2_s_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x306EA255,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP_2_S_AHB_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x2} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 386,
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
      .nDebugMuxSel = 191,
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
      .nDebugMuxSel = 339,
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
      .nDebugMuxSel = 133,
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
      .nDebugMuxSel = 132,
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
      .nDebugMuxSel = 134,
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
      .nDebugMuxSel = 135,
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
      .nDebugMuxSel = 136,
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
      .nDebugMuxSel = 137,
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
      .nDebugMuxSel = 138,
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
      .nDebugMuxSel = 139,
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
      .nDebugMuxSel = 140,
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
      .nDebugMuxSel = 141,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP1CORE2X                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap1_core_2x_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xD7DB82C0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_CORE_2X_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x40000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 145,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap1_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP1CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x40B685B0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP1_CORE_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x80000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 144,
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
      .nDebugMuxSel = 146,
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
      .nDebugMuxSel = 147,
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
      .nDebugMuxSel = 148,
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
      .nDebugMuxSel = 149,
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
      .nDebugMuxSel = 150,
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
      .nDebugMuxSel = 151,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2CORE2X                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_core_2x_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x70CC1B69,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP2_CORE_2X_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x8} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 388,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap2_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2CORE2X,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xAC5F6885,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP2_CORE_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x1} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 387,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S0                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s0_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S0,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x62380FFF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP2_S0_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x10} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 389,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S1                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s1_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S1,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xF02791D2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP2_S1_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x20} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 390,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S2                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s2_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S2,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA5E32B19,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP2_S2_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x40} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 391,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S3                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s3_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S3,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xEEF92C79,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP2_S3_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x80} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 392,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S4                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s4_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S4,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x7980B17E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP2_S4_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x100} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 393,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCQUPV3WRAP2S5                                       */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_qupv3_wrap2_s5_clk",
    .pDomain   = &ClockDomain_GCC_GCCQUPV3WRAP2S5,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x9777FCA3,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP2_S5_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_2_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_2), 0x200} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 394,
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
    .szName    = "gcc_nav_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA670C24C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_NAV_AXI_CBCR), HWIO_ADDR(GCC_NAV_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 371,
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
      .nDebugMuxSel = 345,
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
      .nDebugMuxSel = 347,
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
      .nDebugMuxSel = 348,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSYSNOCMONAQAXI                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_mss_mfab_axis_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCMONAQAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x56446044,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_MFAB_AXIS_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 333,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSYSNOCSFAXI                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_mss_axis2_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCSFAXI,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5EC28930,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_AXIS2_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 334,
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
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BIT_COXM_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 3,
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
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_CONFIG_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 9,
    },
  },
  {
    .szName    = "clk_bus_coxm",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x766806AD,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_COXM_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 12,
    },
  },
  {
    .szName    = "clk_bus_crypto",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x3AA07296,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_CRYPTO_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 13,
    },
  },
  {
    .szName    = "clk_bus_geran_encrypt_acc",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x58175758,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 15,
    },
  },
  {
    .szName    = "clk_bus_mgpi",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xB96A1818,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_MGPI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 14,
    },
  },
  {
    .szName    = "clk_bus_nav",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x742FC2A8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_NAV_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 62,
    },
  },
  {
    .szName    = "clk_bus_offline",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x5C668424,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_OFFLINE_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 59,
    },
  },
  {
    .szName    = "clk_bus_q6",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xAFA015A1,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_Q6_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 57,
    },
  },
  {
    .szName    = "clk_bus_q6_alt",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xF569D4F2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_Q6_ALT_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 39,
    },
  },
  {
    .szName    = "clk_bus_rfc",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xA383C545,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_RFC_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 55,
    },
  },
  {
    .szName    = "clk_bus_rscc",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x8A227677,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_RSCC_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 23,
    },
  },
  {
    .szName    = "clk_bus_stmr",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x1CDCA95E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_STMR_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 17,
    },
  },
  {
    .szName    = "clk_bus_timeout",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x061E360A,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_TIMEOUT_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 18,
    },
  },
  {
    .szName    = "clk_bus_uim0",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x3E9C3672,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_UIM0_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 19,
    },
  },
  {
    .szName    = "clk_bus_uim1",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xACCC33FE,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_UIM1_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 20,
    },
  },
  {
    .szName    = "clk_bus_vq6ss",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x0A2DE3E8,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_BUS_VQ6SS_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 58,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCQ6                                            */
  /*-----------------------------------------------------------------------*/

  /*
   * Fake clock added with domain name since there are no actual branches
   * on the domain. Used to control frequency.
   */
  {
    .szName    = "clk_src_q6",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCQ6,
    .pDebugMux = &ClockDebugMux_MSSCCQ6,
    .nUniqueId = 0xC8E02C55,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = NULL,
      .nDebugMuxSel = 1,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCRFFE                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_4xo_rffe",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCRFFE,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xDECAFFE0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_4XO_RFFE_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 29,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCSERDES                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_serdes",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCSERDES,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x5374A829,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_SERDES_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 42,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCUIM0                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_card_src_uim0",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCUIM0,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x0F12C218,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_CARD_SRC_UIM0_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 30,
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
      .mRegisters   = { HWIO_ADDR(MSS_CLK_UART_BIT_UIM0_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 35,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_MSSCC_CLKSRCUIM1                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_card_src_uim1",
    .pDomain   = &ClockDomain_MSSCC_CLKSRCUIM1,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x7F765B7B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_CLK_CARD_SRC_UIM1_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 31,
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
      .mRegisters   = { HWIO_ADDR(MSS_CLK_UART_BIT_UIM1_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 36,
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
  .aDividers         = aDividers,
  .nNumDividers      = SIZEOF_ARRAY(aDividers),
  .aPowerDomains     = aPowerDomains,
  .nNumPowerDomains  = SIZEOF_ARRAY(aPowerDomains),
  .aDebugMuxes       = aDebugMuxes,
  .nNumDebugMuxes    = SIZEOF_ARRAY(aDebugMuxes),
  .nFlags            = CLOCK_GLOBAL_FLAG_LOG_CLOCK_FREQ_CHANGE |
                       CLOCK_GLOBAL_FLAG_LOG_SOURCE_STATE_CHANGE,
  .SafeMuxConfig     = { 0, 2, 0, 0 },
  .pGenericNPANode   = &GenericNPANode,
};

