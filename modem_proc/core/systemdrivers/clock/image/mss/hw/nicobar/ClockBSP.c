/*
==============================================================================

FILE:         ClockBSP.c

DESCRIPTION:
  This file contains the clock driver BSP data.

  Auto-generated from the following IPCat controller releases:

    Nicobarv1 (nicobar_1.0):
      PLL_HSR   : 3.0
      GCC       : nicobar_clk_ctl_p3q3r69 [nicobar_gcc.r9]
      MSS_CC    : atlas_mss_3ca_11lpp_nicobar_p3q3r40 [nicobar_mss_cc.r3]
      NAV_CC    : nav_ss_nicobar_1.0_p3q3r1.12 [nicobar_nav_cc_r1]


  Generated using the following clockdrivergen settings:

CLOCKDRIVERGEN_CONFIG = \
{'chip_aliases': ['nicobar_1.0'],
 'db_override': {'GCC': {'cbc': {'gcc_rx2_qlink_clkref_en': {'d_registers': {'cbcr_name': 'GCC_SPARE3_RESTART'},
                                                             'd_sw_owner': {'MSS': 'RW'}}}},
                 'MSS_CC': {'cbc': {'clk_axi_dma': {'d_registers': {'cbcr_name': 'MSS_AXI_DMA_CBCR'},
                                                    'dbg_mux_sel': '0x280',
                                                    'l_master': ['gcc_mss_nav_ce_axi_clk_src']},
                                    'clk_axi_mnoc': {'d_registers': {'cbcr_name': 'MSS_AXI_MNOC_CBCR'}},
                                    'clk_axi_nav': {'d_registers': {'cbcr_name': 'MSS_AXI_NAV_CBCR'}},
                                    'clk_axi_nav_sample': {'d_registers': {'cbcr_name': 'MSS_AXI_NAV_SAMPLE_CBCR'}},
                                    'clk_bit_coxm': {'d_registers': {'cbcr_name': 'MSS_BIT_COXM_CBCR'}},
                                    'clk_bus_coxm': {'d_registers': {'cbcr_name': 'MSS_BUS_COXM_CBCR'}},
                                    'clk_bus_crypto': {'d_registers': {'cbcr_name': 'MSS_BUS_CRYPTO_CBCR'}},
                                    'clk_bus_mgpi': {'d_registers': {'cbcr_name': 'MSS_BUS_MGPI_CBCR'}},
                                    'clk_bus_nav': {'d_registers': {'cbcr_name': 'MSS_BUS_NAV_CBCR'}},
                                    'clk_bus_q6': {'d_registers': {'cbcr_name': 'MSS_BUS_Q6SS_CBCR'}},
                                    'clk_bus_rbcpr': {'d_registers': {'cbcr_name': 'MSS_BUS_RBCPR_CBCR'}},
                                    'clk_bus_rfc': {'d_registers': {'cbcr_name': 'MSS_BUS_RFC_CBCR'}},
                                    'clk_bus_rscc': {'d_registers': {'cbcr_name': 'MSS_BUS_RSCC_CBCR'}},
                                    'clk_bus_slave_timeout': {'d_registers': {'cbcr_name': 'MSS_BUS_SLAVE_TIMEOUT_CBCR'}},
                                    'clk_bus_stmr': {'d_registers': {'cbcr_name': 'MSS_BUS_STMR_CBCR'}},
                                    'clk_bus_uim0': {'d_registers': {'cbcr_name': 'MSS_BUS_UIM0_CBCR'}},
                                    'clk_bus_uim1': {'d_registers': {'cbcr_name': 'MSS_BUS_UIM1_CBCR'}},
                                    'clk_bus_uim2': {'d_registers': {'cbcr_name': 'MSS_BUS_UIM2_CBCR'}},
                                    'clk_bus_uim3': {'d_registers': {'cbcr_name': 'MSS_BUS_UIM3_CBCR'}},
                                    'clk_card_src_uim0': {'d_registers': {'cbcr_name': 'MSS_CARD_SRC_UIM0_CBCR'}},
                                    'clk_card_src_uim1': {'d_registers': {'cbcr_name': 'MSS_CARD_SRC_UIM1_CBCR'}},
                                    'clk_card_src_uim2': {'d_registers': {'cbcr_name': 'MSS_CARD_SRC_UIM2_CBCR'}},
                                    'clk_card_src_uim3': {'d_registers': {'cbcr_name': 'MSS_CARD_SRC_UIM3_CBCR'}},
                                    'clk_mss_rffe_tp': {'d_registers': {'cbcr_name': 'MSS_RFFE_TP_CBCR'}},
                                    'clk_rbcpr_ref': {'d_registers': {'cbcr_name': 'MSS_RBCPR_REF_CBCR'}},
                                    'clk_uart_bit_uim0': {'d_registers': {'cbcr_name': 'MSS_UART_BIT_UIM0_CBCR'}},
                                    'clk_uart_bit_uim1': {'d_registers': {'cbcr_name': 'MSS_UART_BIT_UIM1_CBCR'}},
                                    'clk_uart_bit_uim2': {'d_registers': {'cbcr_name': 'MSS_UART_BIT_UIM2_CBCR'}},
                                    'clk_uart_bit_uim3': {'d_registers': {'cbcr_name': 'MSS_UART_BIT_UIM3_CBCR'}},
                                    'clk_xo_mdm': {'d_registers': {'cbcr_name': 'MSS_XO_MDM_CBCR'}},
                                    'clk_xo_nav': {'d_registers': {'cbcr_name': 'MSS_XO_NAV_CBCR'}},
                                    'clk_xo_rscc': {'d_registers': {'cbcr_name': 'MSS_XO_RSCC_CBCR'}}},
                            'cdiv': {'clk_src_uim_card0_prediv': {'__clockdrivergen_list_category': 'rcg',
                                                                  'd_registers': {'cfg_rcgr_name': 'MSS_UIM0_SRC_CFG_RCGR',
                                                                                  'cmd_rcgr_name': 'MSS_UIM0_SRC_CMD_RCGR'}},
                                     'clk_src_uim_card1_prediv': {'__clockdrivergen_list_category': 'rcg',
                                                                  'd_registers': {'cfg_rcgr_name': 'MSS_UIM1_SRC_CFG_RCGR',
                                                                                  'cmd_rcgr_name': 'MSS_UIM1_SRC_CMD_RCGR'}},
                                     'clk_src_uim_card2_prediv': {'__clockdrivergen_list_category': 'rcg',
                                                                  'd_registers': {'cfg_rcgr_name': 'MSS_UIM2_SRC_CFG_RCGR',
                                                                                  'cmd_rcgr_name': 'MSS_UIM2_SRC_CMD_RCGR'}},
                                     'clk_src_uim_card3_prediv': {'__clockdrivergen_list_category': 'rcg',
                                                                  'd_registers': {'cfg_rcgr_name': 'MSS_UIM3_SRC_CFG_RCGR',
                                                                                  'cmd_rcgr_name': 'MSS_UIM3_SRC_CMD_RCGR'}}},
                            'ext': {'clk_axi_crypto': {'__clockdrivergen_list_category': 'cbc',
                                                       'd_registers': {'cbcr_name': 'MSS_AXI_CRYPTO_CBCR'},
                                                       'l_master': ['gcc_mss_nav_ce_axi_clk_src']}},
                            'mux': {'clk_q6': {'__clockdrivergen_list_category': 'cbc'},
                                    'clk_src_bus_mss_config': {'__clockdrivergen_list_category': 'rcg',
                                                               'd_registers': {'cfg_rcgr_name': 'MSS_BUS_CFG_RCGR',
                                                                               'cmd_rcgr_name': 'MSS_BUS_CMD_RCGR'}}},
                            'pll': {'offline_pll': {'d_registers': {'mode_name': 'MSS_OFFLINE_PLL_MODE'},
                                                    'volt_rail_clk_cell': 'VDD_MX',
                                                    'volt_rail_core': 'VDD_MX'},
                                    'qdsp6ss_pll': {'d_registers': {'mode_name': 'MSS_QDSP6SS_PLL_MODE'}}},
                            'rcg': {'clk_src_bit_coxm_mnd': {'d_registers': {'cfg_rcgr_name': 'MSS_BIT_COXM_MND_CFG_RCGR',
                                                                             'cmd_rcgr_name': 'MSS_BIT_COXM_MND_CMD_RCGR'}},
                                    'clk_src_q6': {'d_registers': {'cfg_rcgr_name': 'MSS_QDSP6SS_CORE_CFG_RCGR',
                                                                   'cmd_rcgr_name': 'MSS_QDSP6SS_CORE_CMD_RCGR'},
                                                   'volt_rail_core': 'VDD_MSS_Q6'},
                                    'clk_src_rbcpr_ref': {'d_registers': {'cfg_rcgr_name': 'MSS_RBCPR_REF_CFG_RCGR',
                                                                          'cmd_rcgr_name': 'MSS_RBCPR_REF_CMD_RCGR'}},
                                    'clk_src_rffe_tp': {'d_registers': {'cfg_rcgr_name': 'MSS_RFFE_TP_CFG_RCGR',
                                                                        'cmd_rcgr_name': 'MSS_RFFE_TP_CMD_RCGR'}},
                                    'clk_src_uim0_mnd': {'d_registers': {'cfg_rcgr_name': 'MSS_UIM0_MND_CFG_RCGR',
                                                                         'cmd_rcgr_name': 'MSS_UIM0_MND_CMD_RCGR'}},
                                    'clk_src_uim1_mnd': {'d_registers': {'cfg_rcgr_name': 'MSS_UIM1_MND_CFG_RCGR',
                                                                         'cmd_rcgr_name': 'MSS_UIM1_MND_CMD_RCGR'}},
                                    'clk_src_uim2_mnd': {'d_registers': {'cfg_rcgr_name': 'MSS_UIM2_MND_CFG_RCGR',
                                                                         'cmd_rcgr_name': 'MSS_UIM2_MND_CMD_RCGR'}},
                                    'clk_src_uim3_mnd': {'d_registers': {'cfg_rcgr_name': 'MSS_UIM3_MND_CFG_RCGR',
                                                                         'cmd_rcgr_name': 'MSS_UIM3_MND_CMD_RCGR'}}}},
                 'NAV_CC': {'pll': {'nav_pll4_pll': {'volt_rail_clk_cell': 'VDD_CX',
                                                     'volt_rail_core': 'VDD_CX'}}}},
 'debug_mux_override': {'GCC': {'div': 4},
                        'MSS_CC_Q6': {'inputs': {'clk_q6': 1}}},
 'filter_warning': 'sw',
 'generators': [{'options': {'cc_clock_flags': {},
                             'clock_flags': {'clk_bus_rbcpr': ['SUPPRESSIBLE'],
                                             'clk_bus_slave_timeout': ['SUPPRESSIBLE'],
                                             'clk_bus_uim0': ['SUPPRESSIBLE'],
                                             'clk_bus_uim1': ['SUPPRESSIBLE'],
                                             'clk_bus_uim2': ['SUPPRESSIBLE'],
                                             'clk_bus_uim3': ['SUPPRESSIBLE'],
                                             'clk_rbcpr_ref': ['SUPPRESSIBLE'],
                                             'clk_src_bus_mss_config': ['SUPPRESSIBLE'],
                                             'clk_src_q6': ['SUPPRESSIBLE'],
                                             'clk_xo_rscc': ['SUPPRESSIBLE'],
                                             'gcc_wcss_ahb_s0_clk': ['SUPPRESSIBLE'],
                                             'gpll0': ['READ_ONLY'],
                                             'gpll6': ['READ_ONLY']},
                             'driver_flags': ['LOG_CLOCK_FREQ_CHANGE',
                                              'LOG_SOURCE_STATE_CHANGE'],
                             'env': 'mss',
                             'hal_ctl_structs': {'clk_src_q6': 'HAL_clk_mMSSQ6ClockDomainControl',
                                                 'clk_src_uim0_mnd': 'HAL_clk_mMSSUIMClockDomainControl',
                                                 'clk_src_uim1_mnd': 'HAL_clk_mMSSUIMClockDomainControl',
                                                 'clk_src_uim2_mnd': 'HAL_clk_mMSSUIMClockDomainControl',
                                                 'clk_src_uim3_mnd': 'HAL_clk_mMSSUIMClockDomainControl',
                                                 'gcc_rx2_qlink_clkref_en': 'HAL_clk_mQlinkClkRefControl',
                                                 'gpll0': 'HAL_clk_GPLLStubPLLControl'},
                             'include_HALclkPLLSettings.h': True,
                             'npa_resources': {'/clk/bimc': {'init_state': 'NPA_MAX_STATE'},
                                               '/clk/mss/config_bus': {'clock': 'clk_bus_config',
                                                                       'init_state': 75000},
                                               '/xo/cxo': {'init_state': '1'}},
                             'pll_config_flags': {'gpll6': ['AUX_OUTPUT_ENABLE'],
                                                  'nav_pll4_pll': ['AUX_OUTPUT_ENABLE'],
                                                  'offline_pll': ['EARLY_OUTPUT_ENABLE']},
                             'pll_disable_mode': {'nav_cc_pll4_pll': 'NORMAL',
                                                  'offline_pll': 'STANDBY'}},
                 'output_dir': '..',
                 'type': 'dalclock'}],
 'include_HALclkPLLSettings.h': True,
 'target': ['nicobar']}

==============================================================================

$Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/nicobar/ClockBSP.c#1 $

==============================================================================
            Copyright (c) 2018 QUALCOMM Technologies Incorporated.
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
extern HAL_clk_SourceControlType          HAL_clk_SparkPLLControl;

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
extern HAL_clk_ClockControlType           HAL_clk_mQlinkClkRefControl_v2;
extern HAL_clk_ClockDomainControlType     HAL_clk_mMSSUIMClockDomainControl;
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
  .nParentSel   = 209,
  .HALDebugMux  = {
    .pmControl  = &HAL_clk_DebugMuxControlMSSCC,
  },
};

static ClockDebugMuxType ClockDebugMux_NAVCC =
{
  .szName       = "nav_cc",
  .pParent      = &ClockDebugMux_GCC,
  .nParentSel   = 230,
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
  .szName    = "/xo/cxo",
  .nFlags    = 0x0,
};


/*=========================================================================
      Source Register Settings
==========================================================================*/

static const ClockSourceRegSettingsType ClockSourceRegSettings_MSSCC_OFFLINEPLL[] =
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
  {  600000000, &ClockSource_XO, { 0, 2, 1, 31,  0x4000000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Spark.nMain = 2}           }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};

/*
 * ClockSourceBSP_GCC_GPLL6
 *
 * Used by:
 */
static const ClockSourceFreqConfigType ClockSourceBSP_GCC_GPLL6[] =
{
  {  768000000, &ClockSource_XO, { 0, 2, 1, 40,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {.Spark.nMain = 2}           }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};

/*
 * ClockSourceBSP_MSSCC_OFFLINEPLL
 *
 * Used by:
 *   ClockSource_MSSCC_OFFLINEPLL
 */
static const ClockSourceFreqConfigType ClockSourceBSP_MSSCC_OFFLINEPLL[] =
{
  { 1593600000, &ClockSource_XO, { 0, 0, 1, 83,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_NOM,       },
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
  {  595200000, &ClockSource_XO, { 0, 2, 1, 31,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  691200000, &ClockSource_XO, { 0, 2, 1, 36,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  729600000, &ClockSource_XO, { 0, 2, 1, 38,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  806400000, &ClockSource_XO, { 0, 2, 1, 42,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  864000000, &ClockSource_XO, { 0, 2, 1, 45,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  921600000, &ClockSource_XO, { 0, 2, 1, 48,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
  {  998400000, &ClockSource_XO, { 0, 2, 1, 52,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};
static const ClockSourceFreqConfigType ClockSourceBSPCal_MSSCC_QDSP6SSPLL[] =
{
  {  787200000, &ClockSource_XO, { 0, 2, 1, 41,  0x00000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
};

/*
 * ClockSourceBSP_NAVCC_NAVCCPLL4PLL
 *
 * Used by:
 *   ClockSource_NAVCC_NAVCCPLL4PLL
 */
static const ClockSourceFreqConfigType ClockSourceBSP_NAVCC_NAVCCPLL4PLL[] =
{
  {  408000000, &ClockSource_XO, { 0, 3, 1, 21,  0x4000000000, HAL_CLK_PLL_FRAC_MODE_ALPHA, {{0}}                      }, RAIL_VOLTAGE_LEVEL_MIN_SVS,   },
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
                  CLOCK_CONFIG_PLL_EARLY_OUTPUT_ENABLE |
                  CLOCK_CONFIG_PLL_FSM_MODE_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_STANDBY,
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
  .nConfigMask  = CLOCK_CONFIG_PLL_AUX_OUTPUT_ENABLE |
                  CLOCK_CONFIG_PLL_FSM_MODE_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_STANDBY,
  .HALSource = {
    .nAddr          = HWIO_ADDR(GCC_GPLL6_MODE),
    .VoterRegister  = {HWIO_ADDR(GCC_PROC_CLK_PLL_ENA_VOTE), 0x40},
    .pmControl      = &HAL_clk_SparkPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSP_GCC_GPLL6,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSP_GCC_GPLL6),
};

static ClockSourceNodeType ClockSource_MSSCC_OFFLINEPLL =
{
  .szName       = "offline_pll",
  .nFlags       = 0x0,
  .aBSP         = ClockSourceBSP_MSSCC_OFFLINEPLL,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_MSSCC_OFFLINEPLL),
  .pRail        = &ClockRail_VDDMX,
  .nConfigMask  = CLOCK_CONFIG_PLL_EARLY_OUTPUT_ENABLE,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_OFFLINE_PLL_MODE),
    .pmControl      = &HAL_clk_SparkPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_MSSCC_OFFLINEPLL,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_MSSCC_OFFLINEPLL),
  .pRegSettings               = ClockSourceRegSettings_MSSCC_OFFLINEPLL,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_MSSCC_OFFLINEPLL),
};

static ClockSourceNodeType ClockSource_MSSCC_QDSP6SSPLL =
{
  .szName       = "qdsp6ss_pll",
  .nFlags       = CLOCK_FLAG_INTERNAL_CONTROL,
  .aBSP         = ClockSourceBSP_MSSCC_QDSP6SSPLL,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_MSSCC_QDSP6SSPLL),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = 0x0,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_QDSP6SS_PLL_MODE),
    .pmControl      = &HAL_clk_SparkPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSPCal_MSSCC_QDSP6SSPLL,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSPCal_MSSCC_QDSP6SSPLL),
  .pRegSettings               = ClockSourceRegSettings_MSSCC_OFFLINEPLL,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_MSSCC_OFFLINEPLL),
};

static ClockSourceNodeType ClockSource_NAVCC_NAVCCPLL4PLL =
{
  .szName       = "nav_cc_pll4_pll",
  .nFlags       = 0x0,
  .aBSP         = ClockSourceBSP_NAVCC_NAVCCPLL4PLL,
  .nBSPLen      = SIZEOF_ARRAY(ClockSourceBSP_NAVCC_NAVCCPLL4PLL),
  .pRail        = &ClockRail_VDDCX,
  .nConfigMask  = 0x0,
  .eDisableMode = HAL_CLK_SOURCE_DISABLE_MODE_NORMAL,
  .HALSource = {
    .nAddr          = HWIO_ADDR(MSS_NAV_CC_PLL4_PLL_MODE),
    .pmControl      = &HAL_clk_SparkPLLControl,
  },
  .pCalibrationFreqConfig     = ClockSourceBSP_NAVCC_NAVCCPLL4PLL,
  .nCalibrationFreqConfigLen  = SIZEOF_ARRAY(ClockSourceBSP_NAVCC_NAVCCPLL4PLL),
  .pRegSettings               = ClockSourceRegSettings_MSSCC_OFFLINEPLL,
  .nRegSettingsLen            = SIZEOF_ARRAY(ClockSourceRegSettings_MSSCC_OFFLINEPLL),
};


static ClockSourceNodeType *aSources[] =
{
  &ClockSource_GCC_GPLL0,
  &ClockSource_GCC_GPLL6,
  &ClockSource_MSSCC_OFFLINEPLL,
  &ClockSource_MSSCC_QDSP6SSPLL,
  &ClockSource_NAVCC_NAVCCPLL4PLL,
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
 *   ClockDomain_GCC_GCCQUPV3WRAP1S0
 *   ClockDomain_GCC_GCCQUPV3WRAP1S1
 *   ClockDomain_GCC_GCCQUPV3WRAP1S2
 *   ClockDomain_GCC_GCCQUPV3WRAP1S3
 *   ClockDomain_GCC_GCCQUPV3WRAP1S4
 *   ClockDomain_GCC_GCCQUPV3WRAP1S5
 */
static ClockMuxConfigType ClockDomainBSP_GCC_GCCQUPV3WRAP0S0[] =
{
  {    7372800, &ClockSource_GCC_GPLL0,                   { 2,     2,     384,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {   14745600, &ClockSource_GCC_GPLL0,                   { 2,     2,     768,   15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {   19200000, &ClockSource_XO,                          { 0,     2,     0,     0,     0x00 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[0] = BI_TCXO */
  {   29491200, &ClockSource_GCC_GPLL0,                   { 2,     2,     1536,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {   32000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     8,     75,    0x01 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {   48000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     4,     25,    0x02 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {   64000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     16,    75,    0x03 }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {   75000000, &ClockSource_GCC_GPLL0,                   { 2,     8,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {   80000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     4,     15,    0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {   96000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     8,     25,    0x04 }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {  100000000, &ClockSource_GCC_GPLL0,                   { 1,     12,    0,     0,     0x05 }, RAIL_VOLTAGE_LEVEL_SVS,        },                                                            /* Mux[1] = GPLL0_OUT_EARLY */
  {  102400000, &ClockSource_GCC_GPLL0,                   { 2,     2,     128,   375,   0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {  112000000, &ClockSource_GCC_GPLL0,                   { 2,     2,     28,    75,    0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {  117964800, &ClockSource_GCC_GPLL0,                   { 2,     2,     6144,  15625, 0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {  120000000, &ClockSource_GCC_GPLL0,                   { 2,     5,     0,     0,     0x06 }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[2] = GPLL0_OUT_AUX2 */
  {  128000000, &ClockSource_GCC_GPLL6,                   { 4,     6,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        },                                                            /* Mux[4] = GPLL6_OUT_MAIN */
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
  {  403200000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     4,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_LOW_SVS,    {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 3] /* 806.0  */ },   /* Mux[1] = QDSP6SS_PLL_OUT_MAIN */
  {  595200000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS,        {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 0] /* 595.0  */ },   /* Mux[1] = QDSP6SS_PLL_OUT_MAIN */
  {  729600000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_SVS_L1,     {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 2] /* 729.0  */ },   /* Mux[1] = QDSP6SS_PLL_OUT_MAIN */
  {  864000000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM,        {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 4] /* 864.0  */ },   /* Mux[1] = QDSP6SS_PLL_OUT_MAIN */
  {  921600000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_NOM_L1,     {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 5] /* 921.0  */ },   /* Mux[1] = QDSP6SS_PLL_OUT_MAIN */
  {  998400000, &ClockSource_MSSCC_QDSP6SSPLL,            { 1,     2,     0,     0,     0xFF }, RAIL_VOLTAGE_LEVEL_TUR,        {{0}}, &ClockSourceBSP_MSSCC_QDSP6SSPLL[ 6] /* 998.0  */ },   /* Mux[1] = QDSP6SS_PLL_OUT_MAIN */
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
 *   ClockDomain_GCC_GCCQUPV3WRAP1S0
 *   ClockDomain_GCC_GCCQUPV3WRAP1S1
 *   ClockDomain_GCC_GCCQUPV3WRAP1S2
 *   ClockDomain_GCC_GCCQUPV3WRAP1S3
 *   ClockDomain_GCC_GCCQUPV3WRAP1S4
 *   ClockDomain_GCC_GCCQUPV3WRAP1S5
 */
static const ClockSourceMapType ClockSourceMap_GCC_GCCQUPV3WRAP0S0[] =
{
  { 0,   19200000, &ClockSource_XO,                         },
  { 1,  600000000, &ClockSource_GCC_GPLL0,                  },
  { 2,  300000000, &ClockSource_GCC_GPLL0,                  },
  { 4,  384000000, &ClockSource_GCC_GPLL6,                  },
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
 *   ClockDomain_GCC_GCCQUPV3WRAP1S0
 *   ClockDomain_GCC_GCCQUPV3WRAP1S1
 *   ClockDomain_GCC_GCCQUPV3WRAP1S2
 *   ClockDomain_GCC_GCCQUPV3WRAP1S3
 *   ClockDomain_GCC_GCCQUPV3WRAP1S4
 *   ClockDomain_GCC_GCCQUPV3WRAP1S5
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

static ClockDomainNodeType ClockDomain_GCC_GCCQUPV3WRAP1CORE2X =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
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

static ClockDomainNodeType ClockDomain_GCC_GCCSNOCPERIPH =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .aSourceMap = ClockSourceMap_GCC_GCCQUPV3WRAP0S0,
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

static ClockDomainNodeType ClockDomain_GCC_GCCSYSNOCMONAQSFAXI =
{
  .aBSP      = NULL,
  .nBSPLen   = 0,
  .pRail     = &ClockRail_VDDCX,
  .pSource   = &ClockSource_XO,
  .nFlags    = CLOCK_FLAG_READ_ONLY,
  .HALDomain = {
    .nCGRAddr   = HWIO_ADDR(GCC_SYS_NOC_MONAQ_SF_AXI_CMD_RCGR),
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
  &ClockDomain_GCC_GCCCONFIGNOC,
  &ClockDomain_GCC_GCCQDSSAPBTSCTR,
  &ClockDomain_GCC_GCCQUPV3WRAP0CORE2X,
  &ClockDomain_GCC_GCCQUPV3WRAP0S0,
  &ClockDomain_GCC_GCCQUPV3WRAP0S1,
  &ClockDomain_GCC_GCCQUPV3WRAP0S2,
  &ClockDomain_GCC_GCCQUPV3WRAP0S3,
  &ClockDomain_GCC_GCCQUPV3WRAP0S4,
  &ClockDomain_GCC_GCCQUPV3WRAP0S5,
  &ClockDomain_GCC_GCCQUPV3WRAP1CORE2X,
  &ClockDomain_GCC_GCCQUPV3WRAP1S0,
  &ClockDomain_GCC_GCCQUPV3WRAP1S1,
  &ClockDomain_GCC_GCCQUPV3WRAP1S2,
  &ClockDomain_GCC_GCCQUPV3WRAP1S3,
  &ClockDomain_GCC_GCCQUPV3WRAP1S4,
  &ClockDomain_GCC_GCCQUPV3WRAP1S5,
  &ClockDomain_GCC_GCCSECCTRL,
  &ClockDomain_GCC_GCCSNOCPERIPH,
  &ClockDomain_GCC_GCCSYSNOC,
  &ClockDomain_GCC_GCCSYSNOCMONAQSFAXI,
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
    .szName    = "clk_axi_crypto",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
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
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
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
    .szName    = "gcc_rx2_qlink_clkref_en",
    .pDomain   = &ClockDomain_GCC_CLOCKDRIVERGENNOSOURCE,
    .pDebugMux = NULL,
    .nUniqueId = 0x5648B23F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_SPARE3_RESTART), 0, {0, 0} },
      .pmControl    = &HAL_clk_mQlinkClkRefControl_v2,
      .nDebugMuxSel = HAL_CLK_NO_DEBUG_MUX_SEL,
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
      .nDebugMuxSel = 114,
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
      .nDebugMuxSel = 210,
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
      .nDebugMuxSel = 112,
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
      .nDebugMuxSel = 100,
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
      .nDebugMuxSel = 235,
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
      .nDebugMuxSel = 217,
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
      .nDebugMuxSel = 102,
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
      .nDebugMuxSel = 101,
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
      .nDebugMuxSel = 103,
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
      .nDebugMuxSel = 104,
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
      .nDebugMuxSel = 105,
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
      .nDebugMuxSel = 106,
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
      .nDebugMuxSel = 107,
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
      .nDebugMuxSel = 108,
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
      .nDebugMuxSel = 237,
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
      .nDebugMuxSel = 236,
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
      .nDebugMuxSel = 238,
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
      .nDebugMuxSel = 239,
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
      .nDebugMuxSel = 240,
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
      .nDebugMuxSel = 241,
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
      .nDebugMuxSel = 242,
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
      .nDebugMuxSel = 243,
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
      .nDebugMuxSel = 99,
    },
  },
  {
    .szName    = "gcc_qupv3_wrap_1_m_ahb_clk",
    .pDomain   = &ClockDomain_GCC_GCCSNOCPERIPH,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x728861DF,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_QUPV3_WRAP_1_M_AHB_CBCR), HWIO_ADDR(GCC_QUPV3_WRAPPER_1_BCR), {HWIO_ADDR(GCC_PROC_CLK_BRANCH_ENA_VOTE_1), 0x100000} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 234,
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
    .szName    = "gcc_mss_nav_axi_clk",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOC,
    .pDebugMux = &ClockDebugMux_GCC,
    .nUniqueId = 0x0B672D67,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(GCC_MSS_NAV_AXI_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 200,
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
      .nDebugMuxSel = 211,
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
      .nDebugMuxSel = 212,
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
      .nDebugMuxSel = 213,
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
      .nDebugMuxSel = 214,
    },
  },

  /*-----------------------------------------------------------------------*/
  /* ClockDomain_GCC_GCCSYSNOCMONAQSFAXI                                   */
  /*-----------------------------------------------------------------------*/
  {
    .szName    = "clk_axi_mnoc",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCMONAQSFAXI,
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
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCMONAQSFAXI,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xBFB59914,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { 0, 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 960,
    },
  },
  {
    .szName    = "clk_axi_nav_sample",
    .pDomain   = &ClockDomain_GCC_GCCSYSNOCMONAQSFAXI,
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0x56DD2F9D,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_AXI_NAV_SAMPLE_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 1088,
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
    .pDebugMux = &ClockDebugMux_MSSCC,
    .nUniqueId = 0xEA06AB7F,
    .nFlags    = 0x0,
    .HALClock  = {
      .mRegisters   = { HWIO_ADDR(MSS_RFFE_TP_CBCR), 0, {0, 0} },
      .pmControl    = &HAL_clk_GenericClockControl,
      .nDebugMuxSel = 896,
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
      Remote NPA Resources
==========================================================================*/

static ClockRemoteNodeType aRemoteNodes[] =
{
  /* Commenting it out this part as separate callback need to be maintained for "/clk/bimc"
   * to request for NPA_MAX_STATE
  {
    .Resource = {
    .local_resource_name  = "/clk/bimc",
    .remote_resource_name = "clk2\x00\x00\x00\x00",
    .protocol_type        = "/protocol/rpm/rpm",
    .plugin               = &npa_max_plugin,
    .driver_fcn           = npa_remote_resource_local_aggregation_driver_fcn,
    .units      = "KHz",
    .max                  = NPA_MAX_STATE,
  },
    .nInitState = NPA_MAX_STATE,
  }, */
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
    .local_resource_name  = "/clk/snoc_monaq",
    .remote_resource_name = "clk1\x04\x00\x00\x00",
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
  .aRails            = aRails,
  .nNumRails         = SIZEOF_ARRAY(aRails),
  .aClocks           = aClocks,
  .nNumClocks        = SIZEOF_ARRAY(aClocks),
  .aDomains          = aDomains,
  .nNumDomains       = SIZEOF_ARRAY(aDomains),
  .aSources          = aSources,
  .nNumSources       = SIZEOF_ARRAY(aSources),
  .aDebugMuxes       = aDebugMuxes,
  .nNumDebugMuxes    = SIZEOF_ARRAY(aDebugMuxes),
  .nFlags            = CLOCK_GLOBAL_FLAG_LOG_CLOCK_FREQ_CHANGE |
                       CLOCK_GLOBAL_FLAG_LOG_SOURCE_STATE_CHANGE,
  .SafeMuxConfig     = { 0, 2, 0, 0 },
  .pGenericNPANode   = &GenericNPANode,
  .aRemoteNodes      = aRemoteNodes,
  .nNumRemoteNodes   = SIZEOF_ARRAY(aRemoteNodes),
};

