/*============================================================================
  FILE:         uimdrv_clk.c

  OVERVIEW:     FIle conatins the functions to intilalize and control the clk interface.

  DEPENDENCIES: N/A

                Copyright (c) 2014-2016, 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hw/clkregime/uimdrv_clk.c#6 $
$DateTime: 2019/12/16 04:20:00 $
$Author: pwbldsvc $

when        who        what, where, why
------      ----       -----------------------------------------------------------
12/16/19    rkg        MOB changes
12/01/19    ykr        program simclock cdiv instead of using DalClock_SetClockFrequency
10/22/19    ykr        Changing UIM clock Names as per Kamorta IPCAT.
09/26/19    ykr        Kamorta RUMI Changes
12/06/17    ak         F3 optimization
05/31/16    gm         F3 message removal
04/04/16    ks         Using correct clks for the TA modem
03/31/16    gm         Removed "FEATURE_TABASCO_MODEM"
03/28/16    sam        moved extern of 'uim_hw_param' to header file
03/23/16    dd         Fix for sim clock id issues
01/25/16    sam        Support for Tabasco clock macros
09/28/15    ll         Guard 300MHz CLK src for UART
09/24/15    ll         Atlas HW CR -- Add 300MHz GPLL for UART bit clk source
09/24/15    ll         Add guards depends on UARTDM version and clock tree HW
06/25/15    ll         Clean up ATLAS HW CR feature flags
06/22/15    ll         Atlas HW CR -- Add 300MHz GPLL for UART bit clk source RUMI support
06/08/15    ll         Removing BOLT and THOR feature flag
05/19/15    sam        Fix for compiler warnings 5-19
04/30/15    ll         Atlas HW CR -- Support different SIMCLK frequency
04/28/15    ll         9x55 Bring-up changes
04/08/15    sam        FR24498: Powerup logging new changes
03/24/15    na         Reverting registers write for rumi
03/11/15    na         Setting of frequency before enabling it
03/04/15    na         Removal of compiler warnings
01/16/15    na         Suppport of RUMI
01/07/14    sam        Modification of msg macros for FR24498-UIM Powerup Logging
11/25/14    sam        Check for return value on dal APIs for clock enable and disable
08/25/14    ll         Fix UIM_ATR_FI_VALUE_IS_ZERO_OR_INVALID type mismatch
08/20/14    nmb        9x45 Bring up changes
02/20/14    ak         Maximize the UART clock frequency by setting it directly
02/18/14    sam        Usage of newer style diag macros
02/08/14    sam        Fix for Klocwork issues
01/09/14    akv        Handle to vote for mss bus freq made uim instance specific
10/16/13    akv        Fixed KW errors
10/08/13    nmb        HW header file cleanup
09/19/13    akv        Reference CLK changes to support 4.8MHz
09/16/13    yk         Removed the code for old targets and for old features
07/12/13    nmb        Global Reorganization
02/22/13    js         8x26 bring up changes
02/15/13    ak         Replace MCPM calls with NPA calls to /clk/mss/config_bus
02/12/13    js         General F3 macro cleanup
12/05/12    js         UIM Parallel Processing changes
10/26/12    akv/ssr    HW Enumeration changes
============================================================================*/
#include "uimdrv_clk.h"
#include "uimdrv_uartdm.h"
#include "uimdrv_main.h"
#include "uimglobals.h"
#include "uim_msg.h"
#include "uimdrv_rumi.h"

#if defined (FEATURE_UIM_TEST_FRAMEWORK)
#error code not present
#else

#define UIM1_DAL_SIM_CLK_NAME    "mss_cc_uim0_clk"
#define UIM1_DAL_UART_CLK_NAME   "mss_cc_uim0_xo_clk"
#define UIM1_DAL_HCLK_NAME       "mss_cc_uim0_bus_clk"
#define UIM2_DAL_SIM_CLK_NAME    "mss_cc_uim1_clk"
#define UIM2_DAL_UART_CLK_NAME   "mss_cc_uim1_xo_clk"
#define UIM2_DAL_HCLK_NAME       "mss_cc_uim1_bus_clk"
#define UIM3_DAL_SIM_CLK_NAME    "mss_cc_uim2_clk"
#define UIM3_DAL_UART_CLK_NAME   "mss_cc_uim2_xo_clk"
#define UIM3_DAL_HCLK_NAME       "mss_cc_uim2_bus_clk"

#endif /*FEATURE_UIM_TEST_FRAMEWORK*/


#define UIM_DAL_SIMCLK_CDIV_NAME "mss_cc_uim_div_clk_src"
#define UIM_SIMCLK_3_84_MHZ_DIV   5
#define UIM_SIMCLK_4_8_MHZ_DIV    4

#define CLK_NAME_SIZE   20

static char dal_sim_clk_name [][CLK_NAME_SIZE]   = {UIM1_DAL_SIM_CLK_NAME, UIM2_DAL_SIM_CLK_NAME, UIM3_DAL_SIM_CLK_NAME};
static char dal_uart_clk_name [][CLK_NAME_SIZE]  = {UIM1_DAL_UART_CLK_NAME, UIM2_DAL_UART_CLK_NAME, UIM3_DAL_UART_CLK_NAME};
static char dal_hclk_name [][CLK_NAME_SIZE]      = {UIM1_DAL_HCLK_NAME, UIM2_DAL_HCLK_NAME, UIM3_DAL_HCLK_NAME};

/* List of uim clock frequencies that we could use */
const dword uim_clock_frequency[]={
  3840000,   /* UIMDRV_CLK_FREQ_3_84_MHZ */
  4800000,   /* UIMDRV_CLK_FREQ_4_8_MHZ  */
};

/* The following table holds the Baud Rate Adjustment Factors defined
   for the DI value of an ATR or PPS response */
const byte braf_values[UIM_BRAF_SIZE] =
{
   0,        /* DI = 0 - Reserved for future use  */
   1,        /* DI = 1                            */
   2,        /* DI = 2                            */
   4,        /* DI = 3                            */
   8,        /* DI = 4                            */
  16,        /* DI = 5                            */
  32,        /* DI = 6                            */
  64,        /* DI = 7                            */
  12,        /* DI = 8                            */
  20,        /* DI = 9                            */
   0,        /* DI = 10 - Reserved for future use  */
   0,        /* DI = 11 - Reserved for future use  */
   0,        /* DI = 12 - Reserved for future use  */
   0,        /* DI = 13 - Reserved for future use  */
   0,        /* DI = 14 - Reserved for future use  */
   0,        /* DI = 15 - Reserved for future use  */
};


/* The following table holds the Clock Rate Conversion Factors defined
   for the FI value of an ATR or PPS response */
const dword crcf_values[UIM_CRCF_SIZE] =
{
  372,        /* FI = 0                             */
  372,        /* FI = 1                             */
  558,        /* FI = 2                             */
  744,        /* FI = 3                             */
 1116,        /* FI = 4                             */
 1488,        /* FI = 5                             */
 1860,        /* FI = 6                             */
    0,        /* FI = 7  - Reserved for future use  */
    0,        /* FI = 8  - Reserved for future use  */
  512,        /* FI = 9                             */
  768,        /* FI = 10                            */
 1024,        /* FI = 11                            */
 1536,        /* FI = 12                            */
 2048,        /* FI = 13                            */
    0,        /* FI = 14 - Reserved for future use  */
    0,        /* FI = 15 - Reserved for future use  */
};


/* The following table holds the avaliable CSR register value */
const dword csr_values[UIM_CSR_SIZE] =
{
  24576,        /* CSR = 0                             */
  12288,        /* CSR = 1                             */
   6144,        /* CSR = 2                             */
   3072,        /* CSR = 3                             */
   1536,        /* CSR = 4                             */
    768,        /* CSR = 5                             */
    512,        /* CSR = 6                             */
    384,        /* CSR = 7                             */
    256,        /* CSR = 8                             */
    192,        /* CSR = 9                             */
    128,        /* CSR = 10                            */
     96,        /* CSR = 11                            */
     64,        /* CSR = 12                            */
     48,        /* CSR = 13                            */
     32,        /* CSR = 14                            */
     16,        /* CSR = 15                            */
};


extern uim_clock_rate_conversion_factor_type uim_FI_current[UIM_MAX_INSTANCES];
extern uim_baud_rate_adjustment_factor_type  uim_DI_current[UIM_MAX_INSTANCES];


/**
  This function returns a uart clock frequency and a CSR divider
  value for the given fi and di values.  The uart clock
  frequency that is returned is the maximum value given the
  constraints.
  @param  fi:- Clock rate conversion factor.
  @param  di:- Baud rate adjustment factor.
  @param  sim_clk_freq_hz:- The SIM clock frequency.
  @param  uim_pre_mnd_source_clk_freq_hz: - The UART source clk
                                        frequency
  @param  uart_freq_hz_ptr:- Returns the calculated maximum uart
                             freq in hertz
  @param  bit_clk_div_ptr:-  Returns the calculated UART
                         CSRdivider factor

*/
void uim_calc_uart_clk_freq(uint32 fi,
                            uint32 di,
                            const uint32 sim_clk_freq_hz,
                            uint32 uim_pre_mnd_source_clk_freq_hz,
                            uint32 *uart_freq_hz_ptr,
                            uint32 *bit_clk_div_ptr)
{
  boolean b_success = FALSE;
  int i = 0;

  uint64 uim_data_baudrate_64;
  uint64 uart_bit_gen_limit_64;
  uint64 uim_sim_clk_freq_hz_64 = (uint64)sim_clk_freq_hz;

  /* Due to silicon area consideration, the max UART BIT CLK freq is set to 19.2MHz */
  if ((uim_pre_mnd_source_clk_freq_hz >> 1) <= 19200000)
  {
    uart_bit_gen_limit_64 = (uint64)uim_pre_mnd_source_clk_freq_hz >> 1;
  }
  else
  {
    uart_bit_gen_limit_64 = (uint64)19200000;
  }
  /* All the 20 bit shifting are for increasing accuracy during division */
  uim_data_baudrate_64 = ((uim_sim_clk_freq_hz_64*di)<< 20)/fi;

  /* Looping through CSR values from largest to smallest
     to find the largest uart_src_clock with the constrain
     of the SIM data baudrate and pre-mnd clk source for
     UART_SRC_CLK
  */
  for (i=0; i<UIM_CSR_SIZE; i++)
  {
    /* Get the optimized(largest) UART_SRC_CLK frequency and corresponding CSR value
       under the constrain of MND out put should be less than 1/2 of input frequency
    */
    if ( ( uim_data_baudrate_64 * csr_values[i])<= (uart_bit_gen_limit_64 << 20))
    {
      *uart_freq_hz_ptr = ((uim_data_baudrate_64 * csr_values[i]) >> 20);
      *bit_clk_div_ptr  = csr_values[i];
      b_success = TRUE;
      break;
    }
  }/* for */

  /* uim_data_baudrate is too fast. Even with smallest CSR, UART_SRC_CLK frequency
     is exceeding the MND output limit
  */
  if (FALSE == b_success)
  {
    *uart_freq_hz_ptr = ((uim_data_baudrate_64 * csr_values[UIM_MIN_CSR_INDEX]) >> 20);
    *bit_clk_div_ptr  = csr_values[UIM_MIN_CSR_INDEX];
  }
} /* uim_calc_uart_clk_freq */


/**
 @brief   Initializes the CLK Regime.
 @param   uim_instance:- Current UIM task instance.
 @return  None
*/
void initialize_clkregime(uim_instance_enum_type uim_instance)
{
  DALResult                 eRes = 0;
  uint32                    nFreqHz = 0;
  uimUartEnumList           uart_ref;
  uim_instance_global_type *uim_ptr = uim_get_instance_ptr(uim_instance);
  uint32                    simclk_divider_id = 0;
  uint32                    simclk_divider_val = 0;

  if (uim_ptr == NULL)
  {
    return;
  }

  uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr = NULL;

  uim_hw_if.clkreg[uim_instance].m_clk_hardware_features.SimClk_slot_diff_freq_support =
          uim_hw_param[uimdrv_hw_nv_config_info.dalChipFamilyIndex].simClkDiffFreqSupport;
  uim_hw_if.clkreg[uim_instance].m_clk_hardware_features.UartClk_300MHz_source_support =
          uim_hw_param[uimdrv_hw_nv_config_info.dalChipFamilyIndex].uartClk300MHzSrcSupport;
  if ( TRUE == uim_nv_is_feature_enabled(UIMDRV_FEATURE_UIM_SET_CLK_FREQ_AT_4_8_MHZ, uim_ptr)
       && ((uimdrv_hw_nv_config_info.number_of_active_interfaces == 0x1)
       || TRUE == uim_hw_if.clkreg[uim_instance].m_clk_hardware_features.SimClk_slot_diff_freq_support))
  {
    uim_hw_if.clkreg[uim_instance].m_simClkSourceFreq            = UIM_TCXO_FREQ;
    uim_hw_if.clkreg[uim_instance].m_simClkFreq                  = uim_clock_frequency[UIMDRV_CLK_FREQ_4_8_MHZ];
    uim_hw_if.clkreg[uim_instance].m_simMaxFreqSupported         = UIMDRV_CLK_FREQ_4_8_MHZ;
    uim_hw_if.clkreg[uim_instance].m_preferred_clock_stop_ctl    = UART_SIM_CFG__SIM_CLK_OFF ;
    uim_hw_if.clkreg[uim_instance].m_preferred_clock_ctl         = UART_SIM_CFG__SIM_CLK_TD8_SEL_MASK;
    simclk_divider_val                                           = UIM_SIMCLK_4_8_MHZ_DIV;
  }
  else
  {
    uim_hw_if.clkreg[uim_instance].m_simClkSourceFreq            = UIM_TCXO_FREQ;
    uim_hw_if.clkreg[uim_instance].m_simClkFreq                  = uim_clock_frequency[UIMDRV_CLK_FREQ_3_84_MHZ];
    uim_hw_if.clkreg[uim_instance].m_simMaxFreqSupported         = UIMDRV_CLK_FREQ_3_84_MHZ;
    uim_hw_if.clkreg[uim_instance].m_preferred_clock_stop_ctl    = UART_SIM_CFG__SIM_CLK_OFF ;
    uim_hw_if.clkreg[uim_instance].m_preferred_clock_ctl         = UART_SIM_CFG__SIM_CLK_TD8_SEL_MASK;
    simclk_divider_val                                           = UIM_SIMCLK_3_84_MHZ_DIV;
  }
  
  UIMDRV_MSG_HIGH_1(uim_instance,
    "Sim Clock Frequency value is 0x%x Hz", uim_hw_if.clkreg[uim_instance].m_simClkFreq);

  /* UART and CLK are mutually dependent on the selected UART of the NV config */
  /* First get the UART reference for given uim_instance */

  /* In case of UIM3 test we set uimUART to BADGER_MSS_UIM2_UART_DM = 2 */
  uart_ref = m_HWConfig.uim_slot_config[uim_instance].uimUart;

  eRes = DAL_DeviceAttach(DALDEVICEID_CLOCK,
                          &(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr));
  if (DAL_SUCCESS != eRes)
  {
    UIMDRV_MSG_ERR_1(uim_instance,"!!FAILED TO GET CLOCK HANDLE: %d!!", eRes);
    return;
  }
  /* Read all the configuration */
  eRes = DalClock_GetClockId(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                             dal_sim_clk_name[uart_ref],
                             &(uim_hw_if.clkreg[uim_instance].m_SimClkInfo.nClkId) );

  if (DAL_SUCCESS != eRes)
  {
    UIMDRV_MSG_ERR_2(uim_instance,"!!FAILED TO GET SIM CLOCK ID FOR SLOT %d: %d!!",
                     uart_ref, eRes);
  }
  eRes = DalClock_ResetClock(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                             uim_hw_if.clkreg[uim_instance].m_SimClkInfo.nClkId,
                             CLOCK_RESET_DEASSERT );

  if (DAL_SUCCESS != eRes)
  {
    UIMDRV_MSG_ERR_2(uim_instance,"!!FAILED TO RESET SIM CLOCK ID FOR SLOT %d: %d!!",
                     uart_ref, eRes);
  }
/*Set the SIM clock frequency*/
#if defined (FEATURE_UIM_TEST_FRAMEWORK)
  #error code not present
#else
  eRes = DalClock_GetDividerId(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                               UIM_DAL_SIMCLK_CDIV_NAME,
                               &simclk_divider_id);
  if (DAL_SUCCESS != eRes)
  {
    UIMDRV_MSG_ERR_2(uim_instance,"!!FAILED TO GET SIM CLOCK DIVIDER ID FOR SLOT %d: %d!!",
                     uart_ref, eRes);
  } 

  eRes = DalClock_SetDivider(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                      simclk_divider_id,
                      simclk_divider_val);
  if (DAL_SUCCESS != eRes)
  {
    UIMDRV_MSG_ERR_2(uim_instance,"!!FAILED TO SET SIM CLOCK DIVIDER FOR SLOT %d: %d!!",
                     uart_ref, eRes);
  }
#endif //#if defined (FEATURE_UIM_TEST_FRAMEWORK)

  eRes = DalClock_GetClockId(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                             dal_uart_clk_name[uart_ref],
                             &(uim_hw_if.clkreg[uim_instance].m_UartClkInfo.nClkId));

  if (DAL_SUCCESS != eRes)
  {
    UIMDRV_MSG_ERR_2(uim_instance,"!!FAILED TO GET UART%d CLOCK ID: %d!!",
                     uart_ref, eRes);
  }

  eRes = DalClock_ResetClock(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                             uim_hw_if.clkreg[uim_instance].m_UartClkInfo.nClkId,
                             CLOCK_RESET_DEASSERT );

  if (DAL_SUCCESS != eRes)
  {
    UIMDRV_MSG_ERR_2(uim_instance,"!!FAILED TO RESET UART%d CLOCK ID: %d!!",
                     uart_ref, eRes);
  }
  /* Re-initialing to default to re-use this stack variable for UARTCLK */
  eRes = DAL_ERROR;
  if(TRUE == uim_hw_if.clkreg[uim_instance].m_clk_hardware_features.UartClk_300MHz_source_support)
  {
    /* UART Frequency at this point is considered based on default Fi(371) and Di(1) which later
       get modified in setBaudRate Function */
    uim_calc_uart_clk_freq(crcf_values[UIM_CRCF_372_1],
                           braf_values[UIM_BRAF_1],
                           uim_clock_frequency[UIMDRV_CLK_FREQ_3_84_MHZ],
                           UIM_GPLL0_FREQ,
                           &(uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq),
                           &(uim_hw_if.clkreg[uim_instance].m_uartClkCSR));
    /* Reason for this change is that in uim_dev_init, UART CLOCK is enabled in UIM_TCXO_MUST_BE_ON_SLOT
     and UART Frequency is set in SET_UIM_BAUD_RATE_SLOT this sequence of invocation causing
     the UART clock enablement prior to the actual frequency setting.*/
    eRes = DalClock_SetClockFrequency(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                                      uim_hw_if.clkreg[uim_instance].m_UartClkInfo.nClkId,
                                        uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq,
                                      CLOCK_FREQUENCY_HZ_CLOSEST,
                                      &(uim_hw_if.clkreg[uim_instance].m_uartClkActualFreq));
    UIMDRV_MSG_HIGH_3(uim_instance,"DalClock_SetClockFrequency-300MHz, requestedUartFreq: %d, ActualUartFreq: %d, eRes:%d",
                                    uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq,
                                    uim_hw_if.clkreg[uim_instance].m_uartClkActualFreq,
                                    eRes);
  }

  if (DAL_SUCCESS != eRes)
  {
    /* Re-initialing to default to re-use this stack variable */
    eRes = 0;
    /* UART Frequency at this point is considered based on default Fi(371) and Di(1) which later
       get modified in setBaudRate Function */
    uim_calc_uart_clk_freq(crcf_values[UIM_CRCF_372_1],
                           braf_values[UIM_BRAF_1],
                           uim_clock_frequency[UIMDRV_CLK_FREQ_3_84_MHZ],
                           UIM_TCXO_FREQ,
                           &(uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq),
                           &(uim_hw_if.clkreg[uim_instance].m_uartClkCSR));
    /* Reason for this change is that in uim_dev_init, UART CLOCK is enabled in UIM_TCXO_MUST_BE_ON_SLOT
       and UART Frequency is set in SET_UIM_BAUD_RATE_SLOT this sequence of invocation causing
       the UART clock enablement prior to the actual frequency setting.*/
    eRes = DalClock_SetClockFrequency(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                                      uim_hw_if.clkreg[uim_instance].m_UartClkInfo.nClkId,
                                      uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq,
                                      CLOCK_FREQUENCY_HZ_CLOSEST,
                                      &(uim_hw_if.clkreg[uim_instance].m_uartClkActualFreq));
    UIMDRV_MSG_HIGH_3(uim_instance,"DalClock_SetClockFrequency-19.2MHz, requestedUartFreq: %d, ActualUartFreq: %d, eRes:%d",
                                    uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq,
                                    uim_hw_if.clkreg[uim_instance].m_uartClkActualFreq,
                                    eRes);
  }

  if (DAL_SUCCESS == eRes)
  {
    /* Program the UART CSR register to the divider value that will give us the right bit rate clock */
    uim_prg_uart_csr_by_div_factor(uim_hw_if.clkreg[uim_instance].m_uartClkCSR, uim_instance);
  }
  else
  {
    ERR_FATAL("Set Clock Frequency is failed for UART",0,0,0);
  }

  eRes = DalClock_GetClockId(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                             dal_hclk_name[uart_ref],
                             &(uim_hw_if.clkreg[uim_instance].m_HclkInfo.nClkId));

  if (DAL_SUCCESS != eRes)
  {
    UIMDRV_MSG_ERR_2(uim_instance,"!!FAILED TO GET HCLK%d ID: %d!!",
                     uart_ref, eRes);
  }

  eRes = DalClock_ResetClock(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                             uim_hw_if.clkreg[uim_instance].m_HclkInfo.nClkId,
                             CLOCK_RESET_DEASSERT );
  if (DAL_SUCCESS != eRes)
  {
    UIMDRV_MSG_ERR_2(uim_instance,"!!FAILED TO RESET HCLK%d ID: %d!!",
                      uart_ref, eRes);
  }

  uim_ptr->hardware.mss_bus_clk_voting_handle =
    npa_create_sync_client("/clk/mss/config_bus", "uim", NPA_CLIENT_REQUIRED);
  if (NULL == uim_ptr->hardware.mss_bus_clk_voting_handle)
  {
    UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_222,uim_instance,
                     "!!FAILED TO OBTAIN NPA HANDLE FOR /CLK/MSS/CONFIG_BUS");
  }
} /* initialize_clkregime */


/**
  @brief  Programs the UIM clock for either a digital value or a frequency.
  @param  uim_instance:- Current UIM task instance.
  @param  uim_clock_control_req:- Clock control req, to set
                               clock LOW, HIGH, TD4, TD5.
  @return None
*/
void setClockControl
(
   uim_instance_enum_type uim_instance,
   uim_clock_control_type uim_clock_control_req
)
{
  switch (uim_clock_control_req)
  {
    /* Program the clock at tcxo/5 */
    case UIMDRV_CLK_FREQ_3_84_MHZ:
      uim_hw_if.clkreg[uim_instance].m_simClkFreq =
        uim_hw_if.clkreg[uim_instance].m_simClkSourceFreq/5;
      /* Evaluate the UIM clock frequency and conversion factor */
      uim_hw_if.clkreg[uim_instance].m_preferred_clock_ctl = 0;
#ifdef T_RUMI_EMULATION
      uimdrv_rumi_set_clk_freq_3_84Mhz(uim_instance);
#endif
      /* Program the UIM clock */
      prgSIMCFG(uim_instance, UART_SIM_CFG__CLK_MASK,
                   UART_SIM_CFG__SIM_CLK_ON_MASK |
                   uim_hw_if.clkreg[uim_instance].m_preferred_clock_stop_ctl |
                   uim_hw_if.clkreg[uim_instance].m_preferred_clock_ctl);
      break;

    /* Program the clock at tcxo/4 */
    case UIMDRV_CLK_FREQ_4_8_MHZ:
      /* Evaluate the UIM clock frequency and conversion factor */
      uim_hw_if.clkreg[uim_instance].m_simClkFreq =
        uim_hw_if.clkreg[uim_instance].m_simClkSourceFreq/4;
      uim_hw_if.clkreg[uim_instance].m_preferred_clock_ctl =
        UART_SIM_CFG__SIM_CLK_TD4_SEL_MASK;
#ifdef T_RUMI_EMULATION
      uimdrv_rumi_set_clk_freq_4_8Mhz(uim_instance);
#endif
      /* Program the UIM clock */
      prgSIMCFG(uim_instance, UART_SIM_CFG__CLK_MASK,
                    UART_SIM_CFG__SIM_CLK_ON_MASK |
                    uim_hw_if.clkreg[uim_instance].m_preferred_clock_stop_ctl |
                    uim_hw_if.clkreg[uim_instance].m_preferred_clock_ctl);
      break;

    /* Leave the clock signal HIGH */
    case UIMDRV_CLK_STOP_HIGH:
      uim_hw_if.clkreg[uim_instance].m_preferred_clock_stop_ctl =
        UART_SIM_CFG__SIM_CLK_STOP_HI_MASK;
      prgSIMCFG(uim_instance, UART_SIM_CFG__CLK_MASK,
                    UART_SIM_CFG__SIM_CLK_OFF |
                    uim_hw_if.clkreg[uim_instance].m_preferred_clock_stop_ctl |
                    uim_hw_if.clkreg[uim_instance].m_preferred_clock_ctl);
      break;

    /* Leave the clock signal LOW */
    case UIMDRV_CLK_STOP_LOW:
      uim_hw_if.clkreg[uim_instance].m_preferred_clock_stop_ctl =
        UART_SIM_CFG__SIM_CLK_OFF;
      prgSIMCFG(uim_instance, UART_SIM_CFG__CLK_MASK,
                   UART_SIM_CFG__SIM_CLK_OFF |
                   uim_hw_if.clkreg[uim_instance].m_preferred_clock_stop_ctl |
                   uim_hw_if.clkreg[uim_instance].m_preferred_clock_ctl);
      break;

    default:
      break;
  }
  return;
} /*  setClockControl */


/**
  @brief This procedure determines if the platform can support any {FI, DI}
  combination.
  @param  uim_instance:- Current UIM task instance.
  @param  FI:- Clock rate conversion factor.
  @param  DI:- Baud rate adjustment factor.
  @return TRUE:- If FI/DI values are supported, FALSE - if FI/DI
          values are not supported.
*/
boolean is_fi_di_supported
(
   uim_instance_enum_type    uim_instance,
   uim_clock_rate_conversion_factor_type  FI,
   uim_baud_rate_adjustment_factor_type   DI
)
{
  /*
  The FI (CRCF) and DI (BRAF) that is negotiated determines the baud rate
  that is set on the SIM card, which is:
  uim_clk / (FI / DI) = uim_clk * DI / FI.

  This baud-rate will have to be set on the UART so that UART and the SIM are
  in sync. The baud-rate that is set on the UART is a function of
  {M, N, uart_clk} which is equal to (M / N) * (uart_clk / 16).

  M/N divisor is needed to divide the input clock, and 16 division factor is
  per UART's design that the UART's baud-rate is 16 times less than the
  uart_clk frequency. Also, there is a constraint in the M/N counter, whether
  inside the UART or outside, that N/M ratio should be greater than or equal
  to 2.
  Therefore,

  (M/N) * (uart_clk / 16) = (uim_clk) * DI/ FI     --- equation (1)
  N/M >= 2            --- equation (2)

  Solving the above two equations (eliminating M and N terms), we get the
  following:

  ( uart_clk / uim_clk ) >=  (32 * DI / FI)            --- equation (3)

  This means that, a {FI, DI} combination is supported by a platform as long
  as the above equation (3) is satisfied.

  */

  if((FI < UIM_CRCF_SIZE) && (DI < UIM_BRAF_SIZE))
  {
    if( (uim_hw_if.clkreg[uim_instance].m_simClkFreq > 0) &&
        (uim_hw_if.clkreg[uim_instance].m_simClkSourceFreq > 0) &&
        (crcf_values[FI] > 0) && (braf_values[DI] > 0) )
    {
      if ( (uim_hw_if.clkreg[uim_instance].m_simClkSourceFreq / uim_hw_if.clkreg[uim_instance].m_simClkFreq) >=
           ((32 * braf_values[DI]) / crcf_values[FI]) )
      {
        UIMDRV_MSG_HIGH_0(uim_instance,"FI and DI are supported");
        return TRUE;
      }
      else
      {
        return FALSE;
      }
    }
    else
    {
      return FALSE;
    }
  }
  UIMDRV_MSG_HIGH_0(uim_instance,"FI and DI are NOT supported");
  return FALSE;
} /* is_fi_di_supported */


/**
  @brief enables the UIMclocks: SIM, UART, HCLK.
  @param  uim_instance:- Current UIM task instance.
  @return TRUE if operation is a SUCCESS, FALSE if operation
          FAILS.
*/
boolean enableClock(uim_instance_enum_type uim_instance)
{
#ifndef T_RUMI_EMULATION
  DALResult eRes = 0;
#endif /*T_RUMI_EMULATION*/
  if(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr == NULL)
  {
    return FALSE;
  }
#ifndef T_RUMI_EMULATION
  if (!(uim_hw_if.clkreg[uim_instance].m_SimClkInfo.bEnabled) )
  {
    eRes = DalClock_EnableClock(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                             uim_hw_if.clkreg[uim_instance].m_SimClkInfo.nClkId);
    if (DAL_SUCCESS == eRes)
    {
      uim_hw_if.clkreg[uim_instance].m_SimClkInfo.bEnabled = TRUE;
    }
    else
    {
      UIMDRV_MSG_ERR_1(uim_instance,"Enable sim clock failed with error %d", eRes);
    }
  }
  if (!(uim_hw_if.clkreg[uim_instance].m_UartClkInfo.bEnabled) )
  {
    eRes = DalClock_EnableClock(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                             uim_hw_if.clkreg[uim_instance].m_UartClkInfo.nClkId);
    if (DAL_SUCCESS == eRes)
    {
      uim_hw_if.clkreg[uim_instance].m_UartClkInfo.bEnabled = TRUE;
    }
    else
    {
      UIMDRV_MSG_ERR_1(uim_instance,"Enable UART clock failed with error %d", eRes);
    }
  }
  if (!(uim_hw_if.clkreg[uim_instance].m_HclkInfo.bEnabled) )
  {
    eRes = DalClock_EnableClock(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                             uim_hw_if.clkreg[uim_instance].m_HclkInfo.nClkId);
    if (DAL_SUCCESS == eRes)
    {
      uim_hw_if.clkreg[uim_instance].m_HclkInfo.bEnabled = TRUE;
    }
    else
    {
      UIMDRV_MSG_ERR_1(uim_instance,"Enable hclock failed with error %d", eRes);
    }
  }
#else
  uimdrv_rumi_enable_sim_clock(uim_instance);
  uim_hw_if.clkreg[uim_instance].m_SimClkInfo.bEnabled = TRUE;
  uimdrv_rumi_enable_uart_clock(uim_instance);
  uim_hw_if.clkreg[uim_instance].m_UartClkInfo.bEnabled = TRUE;
  uimdrv_rumi_enable_h_clock(uim_instance);
  uim_hw_if.clkreg[uim_instance].m_HclkInfo.bEnabled = TRUE;

#endif
  return TRUE;
} /* enableClock */


/**
  @brief disabled the UIMclocks: SIM, UART, HCLK.
         For RUMI3, Closk shall not be disabled
  @param  uim_instance:- Current UIM task instance.
  @return TRUE if operation is a SUCCESS, FALSE if operation
          FAILS.
*/
boolean disableClock(uim_instance_enum_type uim_instance)
{
#ifndef T_RUMI_EMULATION
  DALResult eRes = 0;
  if(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr == NULL)
  {
    return FALSE;
  }
  if (uim_hw_if.clkreg[uim_instance].m_SimClkInfo.bEnabled)
  {
    eRes = DalClock_DisableClock(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                              uim_hw_if.clkreg[uim_instance].m_SimClkInfo.nClkId);
    if (DAL_SUCCESS == eRes)
    {
      uim_hw_if.clkreg[uim_instance].m_SimClkInfo.bEnabled = FALSE;
    }
    else
    {
      UIMDRV_MSG_ERR_1(uim_instance,"Disable sim clock failed with error %d", eRes);
    }
  }
  if (uim_hw_if.clkreg[uim_instance].m_UartClkInfo.bEnabled)
  {
    eRes = DalClock_DisableClock(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                              uim_hw_if.clkreg[uim_instance].m_UartClkInfo.nClkId);
    if (DAL_SUCCESS == eRes)
    {
      uim_hw_if.clkreg[uim_instance].m_UartClkInfo.bEnabled = FALSE;
    }
    else
    {
      UIMDRV_MSG_ERR_1(uim_instance,"Disable UART clock failed with error %d", eRes);
    }
  }
#endif
  return TRUE;
} /* disableClock */


/**
  @brief Set the UART clock frequency bases on the UIM baud rate adjustment factor
         nDI, and the UIM clock rate conversion factor, nFI.
  @param  FI , DI
  @result SUCCESS or FAIL
*/
boolean setBaudRate(uim_instance_enum_type uim_instance, uint32  nFI,  uint32  nDI)
{
  boolean b_uart_clk_is_set = FALSE;
  DALResult eRes     = DAL_ERROR;

#ifdef T_RUMI_EMULATION
  uimdrv_rumi_set_baud_rate_and_csr(uim_instance, (word) nFI, (word) nDI);
  return TRUE;
#endif

  if(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr == NULL)
  {
    return FALSE;
  }

  if (TRUE == uim_hw_if.clkreg[uim_instance].m_clk_hardware_features.UartClk_300MHz_source_support)
  {
    /* Determine the maximum UART clock frequency based on FI, DI, source clk freq and SIM clock freq */
    uim_calc_uart_clk_freq(crcf_values[nFI],
                           braf_values[nDI],
                           uim_hw_if.clkreg[uim_instance].m_simClkFreq,
                           UIM_GPLL0_FREQ,
                           &(uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq),
                           &(uim_hw_if.clkreg[uim_instance].m_uartClkCSR));

    /* Set the UART clock frequency
       Note: even though the nUartFreqHz is calculated based on GPLL0, it is up to DALCLK driver to
       decide which source clock to use if the post MND outputs are identical.
       Hence, to check exactly which source is in use please check CLK register values.
    */
    eRes = DalClock_SetClockFrequency(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                                      uim_hw_if.clkreg[uim_instance].m_UartClkInfo.nClkId,
                                      uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq,
                                      CLOCK_FREQUENCY_HZ_CLOSEST,
                                      &(uim_hw_if.clkreg[uim_instance].m_uartClkActualFreq));
    if( eRes !=DAL_SUCCESS)
    {
      UIMDRV_MSG_HIGH_3(uim_instance,"DalClock_SetClockFrequency-300MHz, requestedUartFreq: %d, ActualUartFreq: %d, eRes:%d",
                                      uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq,
                                      uim_hw_if.clkreg[uim_instance].m_uartClkActualFreq,
                                      eRes);
    }
  }

  if(DAL_SUCCESS != eRes)
  {
    uim_calc_uart_clk_freq(crcf_values[nFI],
                           braf_values[nDI],
                           uim_hw_if.clkreg[uim_instance].m_simClkFreq,
                           UIM_TCXO_FREQ,
                           &(uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq),
                           &(uim_hw_if.clkreg[uim_instance].m_uartClkCSR));
    eRes = DalClock_SetClockFrequency(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                                            uim_hw_if.clkreg[uim_instance].m_UartClkInfo.nClkId,
                                            uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq,
                                            CLOCK_FREQUENCY_HZ_CLOSEST,
                                            &(uim_hw_if.clkreg[uim_instance].m_uartClkActualFreq));

    if((uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq != 
        uim_hw_if.clkreg[uim_instance].m_uartClkActualFreq) ||
       (eRes !=DAL_SUCCESS))
    {
      UIMDRV_MSG_HIGH_3(uim_instance,"DalClock_SetClockFrequency-19.2MHz, requestedUartFreq: %d, ActualUartFreq: %d, eRes:%d",
                                      uim_hw_if.clkreg[uim_instance].m_uartClkRequestFreq,
                                      uim_hw_if.clkreg[uim_instance].m_uartClkActualFreq,
                                      eRes);
    }
  }

  if (DAL_SUCCESS == eRes)
  {
    b_uart_clk_is_set = TRUE;
    /* Program the UART CSR register to the divider value that will give us the right bit rate clock */
    uim_prg_uart_csr_by_div_factor(uim_hw_if.clkreg[uim_instance].m_uartClkCSR, uim_instance);
    UIMDRV_MSG_HIGH_1(uim_instance,"DalClock_SetClockFrequency, CSR:%d", uim_hw_if.clkreg[uim_instance].m_uartClkCSR);
  }

  if (FALSE == b_uart_clk_is_set)
  {
    /* If setting the UART clock and divider failed, then let DAL set the UART clock */
    eRes = DalClock_SetUARTBaudClock(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                                     uim_hw_if.clkreg[uim_instance].m_UartClkInfo.nClkId,
                                     uim_hw_if.clkreg[uim_instance].m_SimClkInfo.nClkId,
                                     braf_values[nDI],
                                     crcf_values[nFI],
                                     &(uim_hw_if.clkreg[uim_instance].m_uartClkActualFreq));

    if (DAL_SUCCESS == eRes)
    {
      b_uart_clk_is_set = TRUE;
      /* SetUARTBaudClock assumes a CSR divider value of 16 */
      prgCSR(uim_instance, CSR_VALUE);
      uim_hw_if.clkreg[uim_instance].m_uartClkCSR = CSR_VALUE;
      UIMDRV_MSG_HIGH_1(uim_instance,"DalClock_SetClockFrequency, Set to default CSR:%d", uim_hw_if.clkreg[uim_instance].m_uartClkCSR);
    }
  }
  return b_uart_clk_is_set;
} /* setBaudRate */


/**
  @brief uim_mss_bus_clk_config
  @param nFreqKhz:- Clock frequency
  @result None
*/
void uim_mss_bus_clk_config(npa_client_handle mss_bus_clk_handle, uint32 nFreqKhz)
{
  if (mss_bus_clk_handle)
  {
    npa_issue_required_request(mss_bus_clk_handle, nFreqKhz);
  }
  else
  {
    UIM_MSG_ERR_0("uim_mss_bus_clk_config: NULL NPA handle");
  }
}/* uim_mss_bus_clk_config */


/*===========================================================================
FUNCTION SET_UIM_BAUD_RATE_SLOT

DESCRIPTION
  Set the UART clock frequency bases on the UIM baud rate adjustment factor
  nDI, and the UIM clock rate conversion factor, nFI.
===========================================================================*/
void SET_UIM_BAUD_RATE_SLOT
(
   uim_instance_enum_type  uim_instance,
   uim_clock_rate_conversion_factor_type  nFI,
   uim_baud_rate_adjustment_factor_type   nDI
)
{
  uim_instance_global_type *uim_ptr = uim_get_instance_ptr(uim_instance);

  if(uim_ptr == NULL)
  {
    UIM_MSG_ERR_0("SET_UIM_BAUD_RATE_SLOT: uim_ptr is NULL");
    return;
  }

  /* Store the current FI and DI */
  uim_ptr->state.FI_current = nFI;
  uim_ptr->state.DI_current = nDI;
  setBaudRate(uim_ptr->id, nFI, nDI);
  return;
} /* SET_UIM_BAUD_RATE_SLOT */


/*===========================================================================
FUNCTION UIM_CLOCK_REGIME_DISABLE_SLOT

DESCRIPTION
  Disables the UIM clocks: SIM, UART, HCLK.
===========================================================================*/
void UIM_CLOCK_REGIME_DISABLE_SLOT(uim_instance_enum_type uim_instance)
{
  disableClock(uim_instance);
  return;
} /* UIM_CLOCK_REGIME_DISABLE_SLOT */


/*===========================================================================
FUNCTION UIM_CLOCK_REGIME_ENABLE_SLOT

DESCRIPTION
  Enables the UIM clocks: SIM, UART, HCLK.
===========================================================================*/
void UIM_CLOCK_REGIME_ENABLE_SLOT(uim_instance_enum_type uim_instance)
{
  enableClock(uim_instance);
  return;
} /* UIM_CLOCK_REGIME_ENABLE_SLOT */


/*===========================================================================

FUNCTION UIM_CLOCK_CONTROL

DESCRIPTION
  Programs the UIM clock for either a digital value or a frequency.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  This changes the clock signal to the UIM.

===========================================================================*/
void uim_clock_control( uim_instance_enum_type uim_instance,
                        uim_clock_control_type uim_clock_control_req )

{
  setClockControl(uim_instance, uim_clock_control_req);
  return;
} /* uim_clock_control */


/*===========================================================================

FUNCTION UIM_ATR_FI_VALUE_IS_ZERO_OR_INVALID

DESCRIPTION
  Checks if ATR FI parameter is 0 or invalid.

DEPENDENCIES
  None

RETURN VALUE
  Boolean  TRUE  - Indicates ATR FI parameter is 0 or invalid.
           FALSE - Indicates FI parameter is greater than 0 and valid.

SIDE EFFECTS
  None

===========================================================================*/
boolean UIM_ATR_FI_VALUE_IS_ZERO_OR_INVALID(uim_instance_enum_type uim_instance)
{
  boolean uim_atr_fi_value_zero_or_invalid = FALSE;
  uim_atr_FI_value_type   FI;
  uint8  uim_atr_TA1_index = 0x2;
  uim_instance_global_type *uim_ptr = uim_get_instance_ptr(uim_instance);

  if(uim_ptr == NULL)
  {
    uim_atr_fi_value_zero_or_invalid = TRUE;
    return uim_atr_fi_value_zero_or_invalid;
  }

  /* Get the FI values from the ATR response */
  FI = (uim_atr_FI_value_type)(uim_ptr->atr.atr_buf.data[uim_atr_TA1_index] >> 4);
  UIMDRV_MSG_MED_1(uim_ptr->id,"FI value is %d", FI);  
  switch (FI)
  {
    case FI_0:
    {  /* UIM_CARD_MAX_FREQ_SUPPORTED_4MHZ */
      uim_atr_fi_value_zero_or_invalid = TRUE;
      break;
    }

    case FI_1:  /* UIM_CARD_MAX_FREQ_SUPPORTED_5MHZ */
    case FI_2:  /* UIM_CARD_MAX_FREQ_SUPPORTED_6MHZ */
    case FI_3:  /* UIM_CARD_MAX_FREQ_SUPPORTED_8MHZ */
    case FI_4:  /* UIM_CARD_MAX_FREQ_SUPPORTED_12MHZ */
    case FI_5:  /* UIM_CARD_MAX_FREQ_SUPPORTED_16MHZ */
    case FI_6:  /* UIM_CARD_MAX_FREQ_SUPPORTED_20MHZ */
    case FI_9:  /* UIM_CARD_MAX_FREQ_SUPPORTED_5MHZ */
    case FI_10: /* UIM_CARD_MAX_FREQ_SUPPORTED_7_5_MHZ */
    case FI_11: /* UIM_CARD_MAX_FREQ_SUPPORTED_10MHZ */
    case FI_12: /* UIM_CARD_MAX_FREQ_SUPPORTED_15MHZ */
    case FI_13: /* UIM_CARD_MAX_FREQ_SUPPORTED_20MHZ */
    {
      uim_atr_fi_value_zero_or_invalid = FALSE;
      break;
    }

    case FI_7: /* UIM_CARD_MAX_FREQ_SUPPORTED_INVALID */
    case FI_8:
    case FI_14:
    case FI_15:
    default:
    {
      uim_atr_fi_value_zero_or_invalid = TRUE;
      break;
    }
  }
  return uim_atr_fi_value_zero_or_invalid;
} /* UIM_ATR_FI_VALUE_IS_ZERO_OR_INVALID */


/*===========================================================================

FUNCTION UIM_SWITCH_CLK_FREQ_TO_3_8_MHZ

DESCRIPTION
  Changes the SIM clk frequency to 3.84MHz.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  This changes the clock frequency to the UIM.

===========================================================================*/
void UIM_SWITCH_CLK_FREQ_TO_3_8_MHZ(uim_instance_enum_type uim_instance)
{

  UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_224,uim_instance,"UIM_SWITCH_CLK_FREQ_TO_3_8_MHZ");
  uim_hw_if.clkreg[uim_instance].m_simClkFreq  = uim_clock_frequency[UIMDRV_CLK_FREQ_3_84_MHZ];

#if defined (FEATURE_UIM_TEST_FRAMEWORK)
  #error code not present
#elif defined (T_RUMI_EMULATION)
    uimdrv_rumi_set_clk_freq_3_84Mhz(uim_instance);
#else
  {
    uint32    nDivId = 0;
    DALResult eRes = 0;
    /*Set the SIM clock frequency*/
    eRes = DalClock_GetDividerId(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                                 UIM_DAL_SIMCLK_CDIV_NAME,
                                 &nDivId);
    if (DAL_SUCCESS != eRes)
    {
      UIMDRV_MSG_ERR_1(uim_instance,"!!FAILED TO GET SIM CLOCK DIVIDER ID eRes = %d!!",
                       eRes);
    }

    eRes = DalClock_SetDivider(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                               nDivId,
                               UIM_SIMCLK_3_84_MHZ_DIV);
    if (DAL_SUCCESS != eRes)
    {
      UIMDRV_MSG_ERR_1(uim_instance,"!!FAILED TO SET SIM CLOCK DIVIDER  eRes = %d!!",
                       eRes);
    }
  }
#endif
}/* UIM_SWITCH_CLK_FREQ_TO_3_8_MHZ */


/*===========================================================================

FUNCTION UIM_RESET_CLK_FREQ_TO_DEFAULT_4_8_MHZ

DESCRIPTION
  Resets the SIM clk frequency to 4.8MHz.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Resets the SIM clk frequency to the default 4.8MHz.

===========================================================================*/
void UIM_RESET_CLK_FREQ_TO_DEFAULT_4_8_MHZ(uim_instance_enum_type uim_instance)
{

  UIMDRV_MSG_HIGH_0(uim_instance,"UIM_SWITCH_CLK_FREQ_TO_DEFAULT_4_8_MHZ");
  uim_hw_if.clkreg[uim_instance].m_simClkFreq  = uim_clock_frequency[UIMDRV_CLK_FREQ_4_8_MHZ];

/*Set the SIM clock frequency*/
#if defined (FEATURE_UIM_TEST_FRAMEWORK)
  #error code not present
#else
  {
    uint32    nDivId = 0;
    DALResult eRes = 0;

    eRes = DalClock_GetDividerId(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                                 UIM_DAL_SIMCLK_CDIV_NAME,
                                 &nDivId);
    if (DAL_SUCCESS != eRes)
    {
      UIMDRV_MSG_ERR_1(uim_instance,"!!FAILED TO GET SIM CLOCK DIVIDER ID eRes = %d!!",
                       eRes);
    }
    eRes = DalClock_SetDivider(uim_hw_if.clkreg[uim_instance].m_pDalClockHandle_ptr,
                               nDivId,
                               UIM_SIMCLK_4_8_MHZ_DIV);	
    if (DAL_SUCCESS != eRes)
    {
      UIMDRV_MSG_ERR_1(uim_instance,"!!FAILED TO SET SIM CLOCK DIVIDER eRes = %d!!",
                       eRes);
    }
  }
#endif //#if defined (FEATURE_UIM_TEST_FRAMEWORK)

}/* UIM_RESET_CLK_FREQ_TO_DEFAULT_4_8_MHZ */

/*===========================================================================

FUNCTION UIM_IS_CLK_FREQ_AT_4_8_MHZ

DESCRIPTION
  Checks if the SIM clk frequency is at 4.8MHz.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE  - UIM CLK at 4.8MHz
  FALSE - UIM CLK at 3.8MHz

SIDE EFFECTS
  None.

===========================================================================*/
boolean UIM_IS_CLK_FREQ_AT_4_8_MHZ(uim_instance_enum_type uim_instance)
{
  boolean uim_clk_freq_at_4_8_mhz = FALSE;
  if (uim_hw_if.clkreg[uim_instance].m_simClkFreq ==
        uim_clock_frequency[UIMDRV_CLK_FREQ_3_84_MHZ])
  {
    uim_clk_freq_at_4_8_mhz = FALSE;
  }
  else if(uim_hw_if.clkreg[uim_instance].m_simClkFreq ==
            uim_clock_frequency[UIMDRV_CLK_FREQ_4_8_MHZ])
  {
    uim_clk_freq_at_4_8_mhz = TRUE;
  }
  return uim_clk_freq_at_4_8_mhz;
}/* UIM_IS_CLK_FREQ_AT_4_8_MHZ */

