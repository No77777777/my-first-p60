/*
==============================================================================

FILE:         ClockBSP.c

DESCRIPTION:
  This file contains the clock driver BSP data.

  Auto-generated from the following IPCat controller releases:

    SDX24v1 (chiron_1.0):
      GCC       : Z_chiron_clk_ctl_V1.0_P3Q2R30.5 [chiron_gcc.r12]
      MSS_CC    : z_hercules_a_mss_7ff_1.0_p3_r34_q6ss2.420 [chiron_mss_cc.r9]
      NAV_CC    : nav_ss_chiron_1.0_p3q1r6.4.4 [chiron_nav_cc.r5]


==============================================================================

$Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sdx24/ClockBSP.c#1 $

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
extern HAL_clk_SourceControlType          HAL_clk_RegeraPLLControl;
extern HAL_clk_SourceControlType          HAL_clk_TrionPLLControl;

/*
 * Debug mux control structures.
 */
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlGCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlMSSCC;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlMSSCCQ6;
extern HAL_clk_DebugMuxControlType        HAL_clk_DebugMuxControlNAVCC;

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

static ClockRailType ClockRail_VDDMX =
{
  .szName = "/vcs/vdd_mx",
};

static ClockRailType *aRails[] =
{
  &ClockRail_VDDCX,
  &ClockRail_VDDMSS,
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
  .nParentSel   = 192,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlMSSCC,
  },
};

static ClockDebugMuxType ClockDebugMux_NAVCC =
{
  .szName       = "nav_cc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 194,
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
 * ClockSourceBSP_GCC_GPLL1
 *
 * Used by:
 */
static const ClockSourceFreqConfigType ClockSourceBSP_GCC_GPLL1[] =
{
  {  614400000, &ClockSource_XO, { 0, 0, 1, 32,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Regera.nMain = 2}            }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },
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
  .nFlags       = CLOCK_FLAG_READ_ONLY |
                  CLOCK_FLAG_STUB_HW_RUMI,
  .aBSP         = ClockSourceBSP_GCC_GPLL0,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL0),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EVEN_OUTPUT_ENABLE |
                  CLOCK_CONFIG_PLL_FSM_MODE_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(GCC_GPLL0_MODE),
    .VoterRegister  = {HWIO_ADDR(GCC_PROC_CLK_PLL_ENA_VOTE), 0x1},
    .pmControl      = &HAL_clk_TrionPLLControl,
  },
};

static ClockSourceNodeType ClockSource_GCC_GPLL1 =
{
  .szName       = "gpll1",
  .nFlags       = CLOCK_FLAG_READ_ONLY |
                  CLOCK_FLAG_STUB_HW_RUMI,
  .aBSP         = ClockSourceBSP_GCC_GPLL1,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL1),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EARLY_OUTPUT_ENABLE |
                  CLOCK_CONFIG_PLL_FSM_MODE_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(GCC_GPLL1_MODE),
    .VoterRegister  = {HWIO_ADDR(GCC_PROC_CLK_PLL_ENA_VOTE), 0x2},
    .pmControl      = &HAL_clk_RegeraPLLControl,
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
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
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
  &ClockSource_GCC_GPLL1,
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
};

/*=========================================================================
      Domain BSP
==========================================================================*/

/*
 * ClockDomainBSP_GCC_GCCBLSP1QUP1I2CAPPS
 *
 * Used by:
 *   ClockDomain_GCC_GCCBLSP1QUP1I2CAPPS
 *   ClockDomain_GCC_GCCBLSP1QUP2I2CAPPS
 *   ClockDomain_GCC_GCCBLSP1QUP3I2CAPPS
 *   ClockDomain_GCC_GCCBLSP1QUP4I2CAPPS
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCBLSP1QUP1I2CAPPS[] =
{
  {    9600000, &ClockSource_XO,                          { 0,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 1,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCBLSP1QUP1SPIAPPS
 *
 * Used by:
 *   ClockDomain_GCC_GCCBLSP1QUP1SPIAPPS
 *   ClockDomain_GCC_GCCBLSP1QUP2SPIAPPS
 *   ClockDomain_GCC_GCCBLSP1QUP3SPIAPPS
 *   ClockDomain_GCC_GCCBLSP1QUP4SPIAPPS
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCBLSP1QUP1SPIAPPS[] =
{
  {     960000, &ClockSource_XO,                          { 0,     20,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {    4800000, &ClockSource_XO,                          { 0,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {    9600000, &ClockSource_XO,                          { 0,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   15000000, &ClockSource_GCC_GPLL0,                   { 6,     10,    1,     4,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   24000000, &ClockSource_GCC_GPLL0,                   { 1,     25,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   25000000, &ClockSource_GCC_GPLL0,                   { 1,     24,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 1,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCBLSP1UART1APPS
 *
 * Used by:
 *   ClockDomain_GCC_GCCBLSP1UART1APPS
 *   ClockDomain_GCC_GCCBLSP1UART2APPS
 *   ClockDomain_GCC_GCCBLSP1UART3APPS
 *   ClockDomain_GCC_GCCBLSP1UART4APPS
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCBLSP1UART1APPS[] =
{
  {    3686400, &ClockSource_GCC_GPLL0,                   { 6,     2,     192,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {    7372800, &ClockSource_GCC_GPLL0,                   { 6,     2,     384,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {    9600000, &ClockSource_XO,                          { 0,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   14745600, &ClockSource_GCC_GPLL0,                   { 6,     2,     768,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   16000000, &ClockSource_GCC_GPLL0,                   { 6,     2,     4,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19354839, &ClockSource_GCC_GPLL0,                   { 1,     31,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   20000000, &ClockSource_GCC_GPLL0,                   { 1,     30,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   20689655, &ClockSource_GCC_GPLL0,                   { 1,     29,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   21428571, &ClockSource_GCC_GPLL0,                   { 1,     28,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   22222222, &ClockSource_GCC_GPLL0,                   { 1,     27,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   23076923, &ClockSource_GCC_GPLL0,                   { 1,     26,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   24000000, &ClockSource_GCC_GPLL0,                   { 1,     10,    1,     5,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   25000000, &ClockSource_GCC_GPLL0,                   { 1,     24,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   26086957, &ClockSource_GCC_GPLL0,                   { 1,     23,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   27272727, &ClockSource_GCC_GPLL0,                   { 1,     22,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   28571429, &ClockSource_GCC_GPLL0,                   { 1,     21,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   32000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     4,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   40000000, &ClockSource_GCC_GPLL0,                   { 1,     30,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   46400000, &ClockSource_GCC_GPLL0,                   { 1,     2,     29,    375,   0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   48000000, &ClockSource_GCC_GPLL0,                   { 1,     25,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   51200000, &ClockSource_GCC_GPLL0,                   { 1,     2,     32,    375,   0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   56000000, &ClockSource_GCC_GPLL0,                   { 1,     2,     7,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   58982400, &ClockSource_GCC_GPLL0,                   { 1,     2,     1536,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   60000000, &ClockSource_GCC_GPLL0,                   { 1,     20,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {   63157895, &ClockSource_GCC_GPLL0,                   { 1,     19,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCBLSPUARTSIM
 *
 * Used by:
 *   ClockDomain_GCC_GCCBLSPUARTSIM
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCBLSPUARTSIM[] =
{
  {    3840000, &ClockSource_XO,                          { 0,     10,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
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
 * ClockDomainBSP_GCC_GCCULTAUDIOAHBFABRIC
 *
 * Used by:
 *   ClockDomain_GCC_GCCULTAUDIOAHBFABRIC
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCULTAUDIOAHBFABRIC[] =
{
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   25000000, &ClockSource_GCC_GPLL0,                   { 6,     24,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {   50000000, &ClockSource_GCC_GPLL0,                   { 6,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[6] = GPLL0_OUT_EVEN */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 1,     12,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
  {  200000000, &ClockSource_GCC_GPLL0,                   { 1,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXI2S
 *
 * Used by:
 *   ClockDomain_GCC_GCCULTAUDIOLPAIFAUXI2S
 *   ClockDomain_GCC_GCCULTAUDIOLPAIFPRII2S
 *   ClockDomain_GCC_GCCULTAUDIOLPAIFSECI2S
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXI2S[] =
{
  {     256000, &ClockSource_GCC_GPLL1,                   { 3,     32,    1,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {     512000, &ClockSource_GCC_GPLL1,                   { 3,     16,    1,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {     768000, &ClockSource_GCC_GPLL1,                   { 3,     32,    1,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {    1024000, &ClockSource_GCC_GPLL1,                   { 3,     30,    1,     20,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {    1536000, &ClockSource_GCC_GPLL1,                   { 3,     16,    1,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {    2048000, &ClockSource_GCC_GPLL1,                   { 3,     30,    1,     10,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {    3072000, &ClockSource_GCC_GPLL1,                   { 3,     8,     1,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {    4096000, &ClockSource_GCC_GPLL1,                   { 3,     6,     1,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {    6144000, &ClockSource_GCC_GPLL1,                   { 3,     4,     1,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {   12288000, &ClockSource_GCC_GPLL1,                   { 4,     4,     1,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[4] = GPLL1_OUT_EARLY */
};

/*
 * ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXPCMDATAOE
 *
 * Used by:
 *   ClockDomain_GCC_GCCULTAUDIOLPAIFAUXPCMDATAOE
 *   ClockDomain_GCC_GCCULTAUDIOLPAIFPCMDATAOE
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXPCMDATAOE[] =
{
  {   15360000, &ClockSource_GCC_GPLL1,                   { 3,     20,    1,     2,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {   30720000, &ClockSource_GCC_GPLL1,                   { 3,     20,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {   61440000, &ClockSource_GCC_GPLL1,                   { 4,     20,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[4] = GPLL1_OUT_EARLY */
  {  122880000, &ClockSource_GCC_GPLL1,                   { 4,     10,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[4] = GPLL1_OUT_EARLY */
};

/*
 * ClockDomainBSP_GCC_GCCULTAUDIOLPAIFEXTI2S
 *
 * Used by:
 *   ClockDomain_GCC_GCCULTAUDIOLPAIFEXTI2S
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCULTAUDIOLPAIFEXTI2S[] =
{
  {   12288000, &ClockSource_GCC_GPLL1,                   { 3,     2,     1,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
};

/*
 * ClockDomainBSP_GCC_GCCULTAUDIOLPAIFSLIMBUSCORE
 *
 * Used by:
 *   ClockDomain_GCC_GCCULTAUDIOLPAIFSLIMBUSCORE
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCULTAUDIOLPAIFSLIMBUSCORE[] =
{
  {    3072000, &ClockSource_GCC_GPLL1,                   { 3,     8,     1,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {    6144000, &ClockSource_GCC_GPLL1,                   { 3,     4,     1,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {   12288000, &ClockSource_GCC_GPLL1,                   { 3,     2,     1,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[3] = GPLL1_OUT_MAIN */
  {   24576000, &ClockSource_GCC_GPLL1,                   { 4,     2,     1,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[4] = GPLL1_OUT_EARLY */
};

/*
 * ClockDomainBSP_GCC_GCCULTAUDIOXO
 *
 * Used by:
 *   ClockDomain_GCC_GCCULTAUDIOXO
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCULTAUDIOXO[] =
{
  {    4800000, &ClockSource_XO,                          { 0,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {    9600000, &ClockSource_XO,                          { 0,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[0] = BI_TCXO */
};

/*
 * ClockDomainBSP_MSSCC_CLKSRCBITCOXMMND
 *
 * Used by:
 *   ClockDomain_MSSCC_CLKSRCBITCOXMMND
 */
static ClockMuxConfigType ClockDomainBSP_MSSCC_CLKSRCBITCOXMMND[] =
{
  {    1843200, NULL,                                     { 1,     2,     96,    15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {    7372800, NULL,                                     { 1,     2,     384,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16000000, NULL,                                     { 1,     2,     4,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   32000000, NULL,                                     { 1,     2,     8,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   40000000, NULL,                                     { 1,     2,     2,     15,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   48000000, NULL,                                     { 1,     2,     4,     25,    0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   51200000, NULL,                                     { 1,     2,     64,    375,   0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   60000000, NULL,                                     { 1,     10,    0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
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
  {   75000000, NULL,                                     { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {  150000000, NULL,                                     { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
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
  {    9909677, NULL,                                     { 1,     2,     128,   3875,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   10570323, NULL,                                     { 1,     2,     2048,  58125, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   11520000, NULL,                                     { 1,     2,     24,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   12684387, NULL,                                     { 1,     2,     2359,  55793, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   13212903, NULL,                                     { 1,     2,     512,   11625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   14093763, NULL,                                     { 1,     2,     2409,  51278, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   14400000, NULL,                                     { 1,     2,     6,     125,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   14864516, NULL,                                     { 1,     2,     192,   3875,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   15360000, NULL,                                     { 1,     2,     32,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   15855484, NULL,                                     { 1,     2,     1024,  19375, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16000000, NULL,                                     { 1,     2,     4,     75,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16516129, NULL,                                     { 1,     2,     128,   2325,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   16912516, NULL,                                     { 1,     2,     386,   6847,  0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   17280000, NULL,                                     { 1,     2,     36,    625,   0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   17617204, NULL,                                     { 1,     2,     2048,  34875, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   18000000, NULL,                                     { 1,     2,     3,     50,    0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   19026581, NULL,                                     { 1,     2,     3517,  55454, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[1] = GPLL0_OUT_MAIN_DIV2 */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[0] = XO */
};

/*
 * ClockDomainBSP_NAVCC_NAVCCBBMAIN
 *
 * Used by:
 *   ClockDomain_NAVCC_NAVCCBBMAIN
 */
static ClockMuxConfigType ClockDomainBSP_NAVCC_NAVCCBBMAIN[] =
{
  {  102000000, &ClockSource_NAVCC_NAVCCPLL4PLL,          { 1,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_MIN_SVS,    },                                                            /* Mux[1] = NAV_CC_PLL4_PLL_OUT_MAIN */
  {  204000000, &ClockSource_NAVCC_NAVCCPLL4PLL,          { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     },                                                            /* Mux[1] = NAV_CC_PLL4_PLL_OUT_MAIN */
};


/*=========================================================================
      Domain Source Maps
==========================================================================*/


/*=========================================================================
      Domain FMAX
==========================================================================*/


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

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1QUP1I2CAPPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1QUP1I2CAPPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1QUP1I2CAPPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_QUP1_I2C_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1QUP1SPIAPPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1QUP1SPIAPPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1QUP1SPIAPPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_QUP1_SPI_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1QUP2I2CAPPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1QUP1I2CAPPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1QUP1I2CAPPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_QUP2_I2C_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1QUP2SPIAPPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1QUP1SPIAPPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1QUP1SPIAPPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_QUP2_SPI_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1QUP3I2CAPPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1QUP1I2CAPPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1QUP1I2CAPPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_QUP3_I2C_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1QUP3SPIAPPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1QUP1SPIAPPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1QUP1SPIAPPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_QUP3_SPI_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1QUP4I2CAPPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1QUP1I2CAPPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1QUP1I2CAPPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_QUP4_I2C_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1QUP4SPIAPPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1QUP1SPIAPPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1QUP1SPIAPPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_QUP4_SPI_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1UART1APPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1UART1APPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1UART1APPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_UART1_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1UART2APPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1UART1APPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1UART1APPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_UART2_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1UART3APPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1UART1APPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1UART1APPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_UART3_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSP1UART4APPS =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSP1UART1APPS,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSP1UART1APPS),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP1_UART4_APPS_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCBLSPUARTSIM =
{
  .aBSP      = ClockDomainBSP_GCC_GCCBLSPUARTSIM,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCBLSPUARTSIM),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_BLSP_UART_SIM_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCCE1 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
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
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_CONFIG_NOC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCMEMNOC =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_MEMNOC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCMSSMCDMAMEMNOC =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_MSS_MCDMA_MEMNOC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
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

static ClockDomainNodeType ClockDomain_GCC_GCCULTAUDIOAHBFABRIC =
{
  .aBSP      = ClockDomainBSP_GCC_GCCULTAUDIOAHBFABRIC,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCULTAUDIOAHBFABRIC),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_ULTAUDIO_AHBFABRIC_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCULTAUDIOLPAIFAUXI2S =
{
  .aBSP      = ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXI2S,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXI2S),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_ULTAUDIO_LPAIF_AUX_I2S_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCULTAUDIOLPAIFAUXPCMDATAOE =
{
  .aBSP      = ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXPCMDATAOE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXPCMDATAOE),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_ULTAUDIO_LPAIF_AUX_PCM_DATAOE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCULTAUDIOLPAIFEXTI2S =
{
  .aBSP      = ClockDomainBSP_GCC_GCCULTAUDIOLPAIFEXTI2S,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCULTAUDIOLPAIFEXTI2S),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_ULTAUDIO_LPAIF_EXT_I2S_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCULTAUDIOLPAIFPCMDATAOE =
{
  .aBSP      = ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXPCMDATAOE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXPCMDATAOE),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_ULTAUDIO_LPAIF_PCM_DATAOE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCULTAUDIOLPAIFPRII2S =
{
  .aBSP      = ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXI2S,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXI2S),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_ULTAUDIO_LPAIF_PRI_I2S_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCULTAUDIOLPAIFSECI2S =
{
  .aBSP      = ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXI2S,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCULTAUDIOLPAIFAUXI2S),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_ULTAUDIO_LPAIF_SEC_I2S_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCULTAUDIOLPAIFSLIMBUSCORE =
{
  .aBSP      = ClockDomainBSP_GCC_GCCULTAUDIOLPAIFSLIMBUSCORE,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCULTAUDIOLPAIFSLIMBUSCORE),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_ULTAUDIO_LPAIF_SLIMBUS_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCULTAUDIOXO =
{
  .aBSP      = ClockDomainBSP_GCC_GCCULTAUDIOXO,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_GCC_GCCULTAUDIOXO),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_ULTAUDIO_XO_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 8,
  },
};

static ClockDomainNodeType ClockDomain_GCC_GCCXO =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_XO_CMD_RCGR),
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
    .nCGRAddr   = HWIO_ADDR(MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR),
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
    .nCGRAddr   = HWIO_ADDR(MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCQ6 =
{
  .aBSP      = ClockDomainBSP_MSSCC_CLKSRCQ6,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_MSSCC_CLKSRCQ6),
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_QDSP6SS_CORE_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
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
    .nCGRAddr   = HWIO_ADDR(MSS_CLK_SRC_RFFE_REG_CMD_RCGR),
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
    .nCGRAddr   = HWIO_ADDR(MSS_CLK_SRC_SERDES_REG_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIM0 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_CLK_SRC_UIM0_REG_CMD_RCGR),
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
    .nCGRAddr   = HWIO_ADDR(MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_MSSCC_CLKSRCUIM1 =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_CLK_SRC_UIM1_REG_CMD_RCGR),
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
    .nCGRAddr   = HWIO_ADDR(MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
    .nMNDWidth  = 16,
  },
};

static ClockDomainNodeType ClockDomain_NAVCC_NAVCCBBMAIN =
{
  .aBSP      = ClockDomainBSP_NAVCC_NAVCCBBMAIN,
  .nBSPLen   = SIZEOF_ARRAY(ClockDomainBSP_NAVCC_NAVCCBBMAIN),
  .pRail     = &ClockRail_VDDMSS,
  .pSource   = &ClockSource_XO,
  .nFlags    = 0x0,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(MSS_NAV_CC_BB_MAIN_CMD_RCGR),
    .pmControl  = &HAL_clk_GenericDomainControl,
  },
};


static ClockDomainNodeType *aDomains[] =
{
  &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
  &ClockDomain_GCC_CLOCKDRIVERGENXO,
  &ClockDomain_GCC_GCCBLSP1QUP1I2CAPPS,
  &ClockDomain_GCC_GCCBLSP1QUP1SPIAPPS,
  &ClockDomain_GCC_GCCBLSP1QUP2I2CAPPS,
  &ClockDomain_GCC_GCCBLSP1QUP2SPIAPPS,
  &ClockDomain_GCC_GCCBLSP1QUP3I2CAPPS,
  &ClockDomain_GCC_GCCBLSP1QUP3SPIAPPS,
  &ClockDomain_GCC_GCCBLSP1QUP4I2CAPPS,
  &ClockDomain_GCC_GCCBLSP1QUP4SPIAPPS,
  &ClockDomain_GCC_GCCBLSP1UART1APPS,
  &ClockDomain_GCC_GCCBLSP1UART2APPS,
  &ClockDomain_GCC_GCCBLSP1UART3APPS,
  &ClockDomain_GCC_GCCBLSP1UART4APPS,
  &ClockDomain_GCC_GCCBLSPUARTSIM,
  &ClockDomain_GCC_GCCCE1,
  &ClockDomain_GCC_GCCCONFIGNOC,
  &ClockDomain_GCC_GCCMEMNOC,
  &ClockDomain_GCC_GCCMSSMCDMAMEMNOC,
  &ClockDomain_GCC_GCCSECCTRL,
  &ClockDomain_GCC_GCCSLEEP,
  &ClockDomain_GCC_GCCSYSNOC,
  &ClockDomain_GCC_GCCULTAUDIOAHBFABRIC,
  &ClockDomain_GCC_GCCULTAUDIOLPAIFAUXI2S,
  &ClockDomain_GCC_GCCULTAUDIOLPAIFAUXPCMDATAOE,
  &ClockDomain_GCC_GCCULTAUDIOLPAIFEXTI2S,
  &ClockDomain_GCC_GCCULTAUDIOLPAIFPCMDATAOE,
  &ClockDomain_GCC_GCCULTAUDIOLPAIFPRII2S,
  &ClockDomain_GCC_GCCULTAUDIOLPAIFSECI2S,
  &ClockDomain_GCC_GCCULTAUDIOLPAIFSLIMBUSCORE,
  &ClockDomain_GCC_GCCULTAUDIOXO,
  &ClockDomain_GCC_GCCXO,
  &ClockDomain_MSSCC_CLKSRCBITCOXMMND,
  &ClockDomain_MSSCC_CLKSRCBUSMSSCONFIG,
  &ClockDomain_MSSCC_CLKSRCQ6,
  &ClockDomain_MSSCC_CLKSRCRFFE,
  &ClockDomain_MSSCC_CLKSRCSERDES,
  &ClockDomain_MSSCC_CLKSRCUIM0,
  &ClockDomain_MSSCC_CLKSRCUIM0MND,
  &ClockDomain_MSSCC_CLKSRCUIM1,
  &ClockDomain_MSSCC_CLKSRCUIM1MND,
  &ClockDomain_NAVCC_NAVCCBBMAIN,
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
    .szName    = "nav_cc_bb_qlink_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0x141E0665,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_BB_QLINK_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 9,
    },
  },
  {
    .szName    = "nav_cc_mdm_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0xA204A240,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_MDM_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 11,
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
      .nDebugMuxSel = 12,
    },
  },
  {
    .szName    = "nav_cc_wb_clk",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = &ClockDebugMux_NAVCC,
    .nUniqueId = 0x63987231,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_NAV_CC_WB_CBCR), HWIO_ADDR(MSS_NAV_CC_NAV_SS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 10,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1QUP1I2CAPPS                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_qup1_i2c_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1QUP1I2CAPPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xF0C54808,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_QUP1_I2C_APPS_CBCR), HWIO_ADDR(GCC_BLSP1_QUP1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 55,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1QUP1SPIAPPS                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_qup1_spi_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1QUP1SPIAPPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x6C579798,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_QUP1_SPI_APPS_CBCR), HWIO_ADDR(GCC_BLSP1_QUP1_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 54,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1QUP2I2CAPPS                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_qup2_i2c_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1QUP2I2CAPPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xFA8BF456,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_QUP2_I2C_APPS_CBCR), HWIO_ADDR(GCC_BLSP1_QUP2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 59,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1QUP2SPIAPPS                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_qup2_spi_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1QUP2SPIAPPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x48609714,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_QUP2_SPI_APPS_CBCR), HWIO_ADDR(GCC_BLSP1_QUP2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 58,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1QUP3I2CAPPS                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_qup3_i2c_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1QUP3I2CAPPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5287DEFF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_QUP3_I2C_APPS_CBCR), HWIO_ADDR(GCC_BLSP1_QUP3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 63,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1QUP3SPIAPPS                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_qup3_spi_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1QUP3SPIAPPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x0A3515AE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_QUP3_SPI_APPS_CBCR), HWIO_ADDR(GCC_BLSP1_QUP3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 62,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1QUP4I2CAPPS                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_qup4_i2c_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1QUP4I2CAPPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xE6015987,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_QUP4_I2C_APPS_CBCR), HWIO_ADDR(GCC_BLSP1_QUP4_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 67,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1QUP4SPIAPPS                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_qup4_spi_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1QUP4SPIAPPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x64206533,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_QUP4_SPI_APPS_CBCR), HWIO_ADDR(GCC_BLSP1_QUP4_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 66,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1UART1APPS                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_uart1_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1UART1APPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x63D4CA5B,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_UART1_APPS_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 56,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1UART2APPS                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_uart2_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1UART2APPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x3256DEC0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_UART2_APPS_CBCR), HWIO_ADDR(GCC_BLSP1_UART2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 60,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1UART3APPS                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_uart3_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1UART3APPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x367E8108,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_UART3_APPS_CBCR), HWIO_ADDR(GCC_BLSP1_UART3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 64,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSP1UART4APPS                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_uart4_apps_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSP1UART4APPS,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA3B3E957,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_UART4_APPS_CBCR), HWIO_ADDR(GCC_BLSP1_UART4_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 68,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCBLSPUARTSIM                                        */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_uart1_sim_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSPUARTSIM,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x8865E369,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_UART1_SIM_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 57,
    },
  },
  {
    .szName    = "gcc_blsp1_uart2_sim_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSPUARTSIM,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x9700837E,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_UART2_SIM_CBCR), HWIO_ADDR(GCC_BLSP1_UART2_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 61,
    },
  },
  {
    .szName    = "gcc_blsp1_uart3_sim_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSPUARTSIM,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x3782477F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_UART3_SIM_CBCR), HWIO_ADDR(GCC_BLSP1_UART3_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 65,
    },
  },
  {
    .szName    = "gcc_blsp1_uart4_sim_clk",
    .pDomain   = &ClockDomain_GCC_GCCBLSPUARTSIM,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x4BA1E075,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_UART4_SIM_CBCR), HWIO_ADDR(GCC_BLSP1_UART4_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 69,
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
      .mRegisters   = { HWIO_ADDR(GCC_CE1_CBCR), HWIO_ADDR(GCC_CE1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x20} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 94,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCCONFIGNOC                                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xDA2D04D2,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_AHB_CBCR), 0, {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x2000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 52,
    },
  },
  {
    .szName    = "gcc_boot_rom_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x20C2EB7C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BOOT_ROM_AHB_CBCR), 0, {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x400} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 75,
    },
  },
  {
    .szName    = "gcc_ce1_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5BEC4031,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_CE1_AHB_CBCR), HWIO_ADDR(GCC_CE1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x8} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 96,
    },
  },
  {
    .szName    = "gcc_ce1_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x96B77A7C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_CE1_AXI_CBCR), HWIO_ADDR(GCC_CE1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x10} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 95,
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
      .nDebugMuxSel = 73,
    },
  },
  {
    .szName    = "gcc_ultaudio_pcnoc_mport_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x5799E9F0,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_PCNOC_MPORT_CBCR), HWIO_ADDR(GCC_AUDIO_CORE_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 136,
    },
  },
  {
    .szName    = "gcc_ultaudio_pcnoc_sway_clk",
    .pDomain   = &ClockDomain_GCC_GCCCONFIGNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xD640440D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_PCNOC_SWAY_CBCR), HWIO_ADDR(GCC_AUDIO_CORE_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x80} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 137,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCMEMNOC                                             */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_mss_q6vq6_axim1_clk",
    .pDomain   = &ClockDomain_GCC_GCCMEMNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x4026FE5F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_Q6VQ6_AXIM1_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 191,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCMSSMCDMAMEMNOC                                     */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ddrss_mss_mcdma_clk",
    .pDomain   = &ClockDomain_GCC_GCCMSSMCDMAMEMNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x575B797F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_DDRSS_MSS_MCDMA_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 250,
    },
  },
  {
    .szName    = "gcc_mss_offline_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCMSSMCDMAMEMNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xEEC649B6,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_OFFLINE_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 183,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSECCTRL                                            */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_sec_ctrl_clk",
    .pDomain   = &ClockDomain_GCC_GCCSECCTRL,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xB12762ED,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_SEC_CTRL_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 84,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSLEEP                                              */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_blsp1_sleep_clk",
    .pDomain   = &ClockDomain_GCC_GCCSLEEP,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xB004FDDF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_BLSP1_SLEEP_CBCR), 0, {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x4000000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 53,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSYSNOC                                             */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_mss_ce_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x7377CD8F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_CE_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 184,
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
      .nDebugMuxSel = 261,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCULTAUDIOAHBFABRIC                                  */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ultaudio_ahbfabric_ixfabric_clk",
    .pDomain   = &ClockDomain_GCC_GCCULTAUDIOAHBFABRIC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xB48BA78D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_AHBFABRIC_IXFABRIC_CBCR), HWIO_ADDR(GCC_AUDIO_AHB_BUS_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE), 0x100} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 138,
    },
  },
  {
    .szName    = "gcc_ultaudio_ahbfabric_ixfabric_lpm_clk",
    .pDomain   = &ClockDomain_GCC_GCCULTAUDIOAHBFABRIC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x2CD4171D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_AHBFABRIC_IXFABRIC_LPM_CBCR), HWIO_ADDR(GCC_AUDIO_AHB_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 139,
    },
  },
  {
    .szName    = "gcc_ultaudio_lpaif_slimbus_bam_clk",
    .pDomain   = &ClockDomain_GCC_GCCULTAUDIOAHBFABRIC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x16EB6BAB,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_LPAIF_SLIMBUS_BAM_CBCR), HWIO_ADDR(GCC_AUDIO_AHB_BUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 140,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCULTAUDIOLPAIFAUXI2S                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ultaudio_lpaif_aux_i2s_clk",
    .pDomain   = &ClockDomain_GCC_GCCULTAUDIOLPAIFAUXI2S,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x373CA6AE,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_LPAIF_AUX_I2S_CBCR), HWIO_ADDR(GCC_ULTAUDIO_AUX_I2S_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 143,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCULTAUDIOLPAIFAUXPCMDATAOE                          */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ultaudio_lpaif_aux_pcm_dataoe_clk",
    .pDomain   = &ClockDomain_GCC_GCCULTAUDIOLPAIFAUXPCMDATAOE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x89395056,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_LPAIF_AUX_PCM_DATAOE_CBCR), HWIO_ADDR(GCC_ULTAUDIO_PCM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 148,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCULTAUDIOLPAIFEXTI2S                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ultaudio_lpaif_ext_i2s_clk",
    .pDomain   = &ClockDomain_GCC_GCCULTAUDIOLPAIFEXTI2S,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xD714ABEF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_LPAIF_EXT_I2S_CBCR), HWIO_ADDR(GCC_ULTAUDIO_EXT_I2S_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 145,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCULTAUDIOLPAIFPCMDATAOE                             */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ultaudio_lpaif_pcm_dataoe_clk",
    .pDomain   = &ClockDomain_GCC_GCCULTAUDIOLPAIFPCMDATAOE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x91ACF858,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_LPAIF_PCM_DATAOE_CBCR), HWIO_ADDR(GCC_ULTAUDIO_PCM_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 147,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCULTAUDIOLPAIFPRII2S                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ultaudio_lpaif_pri_i2s_clk",
    .pDomain   = &ClockDomain_GCC_GCCULTAUDIOLPAIFPRII2S,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x61C2006F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_LPAIF_PRI_I2S_CBCR), HWIO_ADDR(GCC_ULTAUDIO_PRI_I2S_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 141,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCULTAUDIOLPAIFSECI2S                                */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ultaudio_lpaif_sec_i2s_clk",
    .pDomain   = &ClockDomain_GCC_GCCULTAUDIOLPAIFSECI2S,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xA84A149C,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_LPAIF_SEC_I2S_CBCR), HWIO_ADDR(GCC_ULTAUDIO_SEC_I2S_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 142,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCULTAUDIOLPAIFSLIMBUSCORE                           */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ultaudio_lpaif_slimbus_core_clk",
    .pDomain   = &ClockDomain_GCC_GCCULTAUDIOLPAIFSLIMBUSCORE,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x7F896058,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_LPAIF_SLIMBUS_CORE_CBCR), HWIO_ADDR(GCC_SLIMBUS_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 146,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCULTAUDIOXO                                         */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_ultaudio_avsync_xo_clk",
    .pDomain   = &ClockDomain_GCC_GCCULTAUDIOXO,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x8ED4382F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_ULTAUDIO_AVSYNC_XO_CBCR), HWIO_ADDR(GCC_AUDIO_CXO_BCR), {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 144,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCXO                                                 */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "gcc_xo_div4_clk",
    .pDomain   = &ClockDomain_GCC_GCCXO,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0xF79A7851,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_XO_DIV4_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 99,
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
    .pDebugMux = NULL,
    .nUniqueId = 0xC8E02C55,
    .nFlags    = CLOCK_FLAG_SUPPRESSIBLE,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
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
};


/*=========================================================================
      Power Domains
==========================================================================*/

static ClockPowerDomainNodeType aPowerDomains[] =
{
  {
    .szName         = "nav_ss_gdsc",
    .nFlags         = 0x0,
    .HALPowerDomain = {
      .nGDSCRAddr   = HWIO_ADDR(MSS_NAV_CC_NAV_SS_GDSCR),
      .pmControl    = &HAL_clk_GenericPowerDomainControl,
    },
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
    150000,        /* /clk/mss/config_bus */
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

