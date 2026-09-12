/*============================================================================
  FILE:         uimdrv_rumi.c

  OVERVIEW:     File contains RUMI specific driver code.

  DEPENDENCIES: N/A

                Copyright (c) 2015, 2019, 2020 QUALCOMM Technologies, Inc(QTI).
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hw/rumi/uimdrv_rumi.c#6 $
$DateTime: 2020/12/24 03:59:46 $
$Author: pwbldsvc $

  when      who     what, where, why
------      ----    -----------------------------------------------------------
12/23/20    ykr     Atheron BU changes
11/27/19    ykr     Configure MND values for 3.84MHz SIM clock in Kamorta RUMI.
09/27/19    ykr     Kamorta Additional RUMI changes
07/08/19    srk     Kamorta RUMI changes
08/20/15    ll      Disable 300MHz CLK temporarily for E3R0 emulation bug
07/15/15    ll      Add back 300MHz UART_BIT_CLK MND CSR table
06/22/15    ll      Atlas HW CR -- Add 300MHz GPLL for UART bit clk source RUMI support
06/09/15    ll      Change ATLAS RUMI UIM2 V ENABLE GPIO from 72 to 80
06/08/15    ll      Removing BOLT and THOR feature flag
05/07/15    ll      Change RUMI level translator GPIO numbers
04/28/15    ll      9x55 Bring-up changes
03/30/15    ak      Compile fix for Atlas RUMI
03/04/15    na      Removal of compiler warnings
01/22/15    na      Rumi crash issue
01/16/15    na      Support of RUMI
08/15/13    ak      Fix VSEL control functions for UIM2
07/09/13    ak      Created
============================================================================*/
#include "uimdrv_main.h"
#include "uim_v.h"
#include "DDITlmm.h"
#include "uimdrv_rumi.h"

#define UIM1_RUMI_V_SEL_GPIO_NUM                           68
#define UIM2_RUMI_V_SEL_GPIO_NUM                           65
#define UIM1_RUMI_V_ENABLE_GPIO_NUM                        66
#define UIM2_RUMI_V_ENABLE_GPIO_NUM                        67

#define UIM_MND_CFG_RCGR_V1_4_1                            0x2000
#define UIM_MND_CFG_RCGR_V1_4_2                            0x2001

#define UIM1_RUMI_V_ENABLE_GPIO \
	    DAL_GPIO_CFG(UIM1_RUMI_V_ENABLE_GPIO_NUM, 0, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)   /* UIM1 VOLTAGE ENABLE   */

#define UIM1_RUMI_V_SEL_GPIO \
	    DAL_GPIO_CFG(UIM1_RUMI_V_SEL_GPIO_NUM, 0, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)      /* UIM1 VOLTAGE SELECT  */

#define UIM2_RUMI_V_ENABLE_GPIO \
	    DAL_GPIO_CFG(UIM2_RUMI_V_ENABLE_GPIO_NUM, 0, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)   /* UIM2 VOLTAGE ENABLE   */

#define UIM2_RUMI_V_SEL_GPIO \
	    DAL_GPIO_CFG(UIM2_RUMI_V_SEL_GPIO_NUM, 0, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)      /* UIM2 VOLTAGE SELECT  */

#define UIM_HWIO_RUMI_IN(addr)        (*((volatile uint32 *) (addr)))
#define UIM_HWIO_RUMI_OUT(addr, val)  (*((volatile uint32 *) (addr)) = ((uint32) (val)))

/* Clock driver */
/* SIM CLK */
#define MSS_UIM0_BCR_ADDR_RUMI           HWIO_MSS_CC_UIM0_BCR_ADDR
#define MSS_UIM0_CBCR_RUMI               HWIO_MSS_CC_UIM0_CBCR_ADDR
#define MSS_UIM0_XO_CBCR_RUMI            HWIO_MSS_CC_UIM0_XO_CBCR_ADDR

#ifndef FEATURE_UIM_ATHERTON
#define MSS_UIM1_CBCR_RUMI               HWIO_MSS_CC_UIM1_CBCR_ADDR
#define MSS_UIM1_BCR_ADDR_RUMI           HWIO_MSS_CC_UIM1_BCR_ADDR
#define MSS_UIM1_XO_CBCR_RUMI            HWIO_MSS_CC_UIM1_XO_CBCR_ADDR
#endif /* FEATURE_UIM_ATHERTON */

#define MSS_UIM_DIV_CDIVR_RUMI           HWIO_MSS_CC_UIM_DIV_CDIVR_ADDR

/* UART CLK */
#define MSS_UART_BIT_UIM0_CBCR_RUMI    HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR  

#ifndef FEATURE_UIM_ATHERTON
#define MSS_UART_BIT_UIM1_CBCR_RUMI    HWIO_MSS_UIM1_UART_DM_UIM_CFG_ADDR
#endif /* FEATURE_UIM_ATHERTON */

/* HCLK */
#define MSS_BUS_UIM0_CBCR_RUMI         HWIO_MSS_CC_UIM0_BUS_CBCR_ADDR

#ifndef FEATURE_UIM_ATHERTON
#define MSS_BUS_UIM1_CBCR_RUMI         HWIO_MSS_CC_UIM1_BUS_CBCR_ADDR
#endif /* FEATURE_UIM_ATHERTON */

/* UART BAUD RATE */
#define MSS_UIM0_MND_CMD_RCGR_RUMI     HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ADDR
#define MSS_UIM0_MND_CFG_RCGR_RUMI     HWIO_MSS_CC_UIM0_MND_CFG_RCGR_ADDR
#define MSS_UIM0_MND_M_RUMI            HWIO_MSS_CC_UIM0_MND_M_ADDR
#define MSS_UIM0_MND_N_RUMI            HWIO_MSS_CC_UIM0_MND_N_ADDR
#define MSS_UIM0_MND_D_RUMI            HWIO_MSS_CC_UIM0_MND_D_ADDR
#define MSS_UIM0_UART_DM_CSR_RUMI      HWIO_MSS_UIM0_UART_DM_CSR_ADDR

#ifndef FEATURE_UIM_ATHERTON
#define MSS_UIM1_MND_CMD_RCGR_RUMI     HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ADDR
#define MSS_UIM1_MND_CFG_RCGR_RUMI     HWIO_MSS_CC_UIM1_MND_CFG_RCGR_ADDR
#define MSS_UIM1_MND_M_RUMI            HWIO_MSS_CC_UIM1_MND_M_ADDR
#define MSS_UIM1_MND_N_RUMI            HWIO_MSS_CC_UIM1_MND_N_ADDR
#define MSS_UIM1_MND_D_RUMI            HWIO_MSS_CC_UIM1_MND_D_ADDR
#define MSS_UIM1_UART_DM_CSR_RUMI      HWIO_MSS_UIM1_UART_DM_CSR_ADDR
#endif /* FEATURE_UIM_ATHERTON */


/* For RUMI debugging, runtime switching between 300MHz and 19.2MHz source CLK */
boolean is_300MHz_source_RUMI = FALSE;

/*MND and CSR REG Table*/
uimdrv_rumi_fi_di_mnd_reg_map_type uimdrv_rumi_mnd_csr_reg[] =
{{UIM_CRCF_372_1,UIM_BRAF_1,  {{0x0001,0xFF88,0xFF87,0x00FF}, {0x000B,0xFFEA,0xFFDF,0x0066}, {0x000B,0xFFEA,0xFFDF,0x0066}, {0x000B,0xFFEA,0xFFDF,0x0066}}},  /*7.928/768 - 6.606/512 - 15.855/1536  - 9.91/768*/
 {UIM_CRCF_372_1,UIM_BRAF_12, {{0x03DF,0xFA7E,0xF69F,0x00CC}, {0x03A1,0xFA40,0xF69F,0x00DD}, {0x0421,0xB600,0xB1DF,0x00AA}, {0x03DF,0xB5BE,0xB1DF,0x00BB}}},  /*7.928/64  - 7.432/48  - 15.855/1536   - 14.865/96 */
 {UIM_CRCF_512,UIM_BRAF_1,    {{0x0003,0xFFF8,0xFFF5,0x0055}, {0x0003,0xFFFA,0xFFF7,0x0055}, {0x0018,0xFDA6,0xFD8E,0x0044}, {0x0006,0xFF88,0xFF82,0x0044}}},  /*5.76/768  - 7.2/768   - 11.52/1536 - 14.4/1536  */
 {UIM_CRCF_512,UIM_BRAF_8,    {{0x0002,0xFFFC,0xFFFA,0x00AA}, {0x0003,0xFFFA,0xFFF7,0x00BB}, {0x0020,0xFDAE,0xFD8E,0x0088}, {0x0008,0xFF8A,0xFF82,0x0088}}},  /*7.68/128  - 7.2/96    - 15.36/256  - 19.2/256  */
 {UIM_CRCF_512,UIM_BRAF_16,   {{0x0002,0xFFFC,0xFFFA,0x00CC}, {0x0003,0xFFFA,0xFFF7,0x00DD}, {0x0020,0xFDAE,0xFD8E,0x00AA}, {0x0008,0xFF8A,0xFF82,0x00AA}}},  /*7.68/64   - 7.2/48    - 15.36/128  - 19.2/128   */
 {UIM_CRCF_512,UIM_BRAF_32,   {{0x0002,0xFFFC,0xFFFA,0x00EE}, {0x0001,0xFFFC,0xFFFB,0x00FF}, {0x0020,0xFDAE,0xFD8E,0x00CC}, {0x0008,0xFF8A,0xFF82,0x00CC}}},  /*7.68/32   - 4.8/16    - 15.36/64   - 19.2/64   */
 {UIM_CRCF_512,UIM_BRAF_64,   {{0x0002,0xFFFC,0xFFFA,0x00FF}, {0x0001,0xFFFE,0xFFFD,0x00FF}, {0x0020,0xFDAE,0xFD8E,0x00EE}, {0x0008,0xFF8A,0xFF82,0x00EE}}},  /*7.68/16   - 9.6/16    - 15.36/32    - 19.2/32   */
 /* End of the ROW */
 {UIM_CRCF_SIZE,UIM_BRAF_SIZE,{{0x0000,0x0000,0x0000,0x0000}, {0x0000,0x0000,0x0000,0x0000}}}
};


/*===========================================================================
FUNCTION uimdrv_rumi_config_gpios

DESCRIPTION
  CONFIG_VLTG_ENABLE_VSEL_SLOT configures the GPIOs connected to the V ENABLE and
  V SELECT lines.  For RUMI only.

===========================================================================*/
void uimdrv_rumi_config_gpios(uim_instance_enum_type uim_instance_id)
{
#ifdef T_RUMI_EMULATION
  if (UIM_INSTANCE_1 == uim_instance_id)
  {
    if(NULL != uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr)
    {
      /* Configure v_enable line */
      (void)DalTlmm_ConfigGpio( uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr,
                                (uint32)UIM1_RUMI_V_ENABLE_GPIO,
                                (uint32)DAL_TLMM_GPIO_ENABLE );
      /* Configure v_sel line */
      (void)DalTlmm_ConfigGpio( uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr,
                                (uint32)UIM1_RUMI_V_SEL_GPIO,
                                (uint32)DAL_TLMM_GPIO_ENABLE );
    }
  }
  else
  {
    if(NULL != uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr)
    {
      /* Configure v_enable line */
      (void)DalTlmm_ConfigGpio( uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr,
                                (uint32)UIM2_RUMI_V_ENABLE_GPIO,
                                (uint32)DAL_TLMM_GPIO_ENABLE );
      /* Configure v_sel line */
      (void)DalTlmm_ConfigGpio( uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr,
                                (uint32)UIM2_RUMI_V_SEL_GPIO,
                                (uint32)DAL_TLMM_GPIO_ENABLE );
    }
  }
#endif /* T_RUMI_EMULATION */

  return;
}


/*===========================================================================
FUNCTION uimdrv_rumi_vsel_off

DESCRIPTION
  CONFIG_VSEL_OFF_SLOT disables the V ENABLE line and sets the V SELECT line
  to 1.8v.  For RUMI only.
===========================================================================*/
void uimdrv_rumi_vsel_off(uim_instance_enum_type uim_instance_id)
{
#ifdef T_RUMI_EMULATION
  /* Set v_enable line to low */
  if (UIM_INSTANCE_1 == uim_instance_id)
  {
    if(NULL != uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr)
    {
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM1_RUMI_V_ENABLE_GPIO, DAL_GPIO_LOW_VALUE);
      /* set v_sel line to 1.8 v */
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM1_RUMI_V_SEL_GPIO, DAL_GPIO_LOW_VALUE);
    }
  }
  else
  {
    if(NULL != uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr)
    {
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM2_RUMI_V_ENABLE_GPIO, DAL_GPIO_LOW_VALUE);
      /* set v_sel line to 1.8 v */
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM2_RUMI_V_SEL_GPIO, DAL_GPIO_LOW_VALUE);
    }
  }
#endif /* T_RUMI_EMULATION */

  return;
}


/*===========================================================================
FUNCTION uimdrv_rumi_vsel_on_class_c

DESCRIPTION
  Turns on the V_ENABLE line and sets the V_SELECT line for 1.8v operation.
  For RUMI only.
===========================================================================*/
void uimdrv_rumi_vsel_on_class_c(uim_instance_enum_type uim_instance_id)
{
#ifdef T_RUMI_EMULATION
  if (UIM_INSTANCE_1 == uim_instance_id)
  {
    if(NULL != uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr)
    {
      /* Set v_enable line to high */
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM1_RUMI_V_ENABLE_GPIO, DAL_GPIO_HIGH_VALUE);
      /* set v_sel line to 1.8 v */
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM1_RUMI_V_SEL_GPIO, DAL_GPIO_LOW_VALUE);
    }
  }
  else
  {
    if(NULL != uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr)
    {
      /* Set v_enable line to high */
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM2_RUMI_V_ENABLE_GPIO, DAL_GPIO_HIGH_VALUE);
      /* set v_sel line to 3.0 v */
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM2_RUMI_V_SEL_GPIO, DAL_GPIO_LOW_VALUE);
    }
  }
#endif /* T_RUMI_EMULATION */

  return;
}


/*===========================================================================
FUNCTION uimdrv_rumi_vsel_on_class_b

DESCRIPTION
  Turns on the V_ENABLE line and sets the V_SELECT line for 3v operation.
  For RUMI only.
===========================================================================*/
void uimdrv_rumi_vsel_on_class_b(uim_instance_enum_type uim_instance_id)
{
#ifdef T_RUMI_EMULATION
  if (UIM_INSTANCE_1 == uim_instance_id)
  {
    if(NULL != uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr)
    {
      /* Set v_enable line to high */
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM1_RUMI_V_ENABLE_GPIO, DAL_GPIO_HIGH_VALUE);
      /* set v_sel line to 1.8 v */
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM1_RUMI_V_SEL_GPIO, DAL_GPIO_HIGH_VALUE);
    }
  }
  else
  {
    if(NULL != uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr)
    {
      /* Set v_enable line to high */
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM2_RUMI_V_ENABLE_GPIO, DAL_GPIO_HIGH_VALUE);
      /* set v_sel line to 3.0 v */
      (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance_id].m_TlmmHandle_ptr, (uint32)UIM2_RUMI_V_SEL_GPIO, DAL_GPIO_HIGH_VALUE);
    }
  }
#endif /* T_RUMI_EMULATION */

  return;
}


/*===========================================================================
FUNCTION uimdrv_rumi_enable_uart_clock

DESCRIPTION
  Enabling the UART CLOCK
===========================================================================*/
void uimdrv_rumi_enable_uart_clock(uim_instance_enum_type uim_instance_id)
{
#ifdef T_RUMI_EMULATION
  uint32 uimdrv_rumi_current_cbcr_value = 0x00;
  if(UIM_INSTANCE_1 == uim_instance_id)
  {
    uimdrv_rumi_current_cbcr_value = UIM_HWIO_RUMI_IN(MSS_UART_BIT_UIM0_CBCR_RUMI);
    UIM_HWIO_RUMI_OUT(MSS_UART_BIT_UIM0_CBCR_RUMI, 0x176A);
    uim_clk_busy_wait (100);

  }
#ifndef FEATURE_UIM_ATHERTON
  else if(UIM_INSTANCE_2 == uim_instance_id)
  {
    uimdrv_rumi_current_cbcr_value = UIM_HWIO_RUMI_IN(MSS_UART_BIT_UIM1_CBCR_RUMI);
    UIM_HWIO_RUMI_OUT(MSS_UART_BIT_UIM1_CBCR_RUMI, 0x176A);
    uim_clk_busy_wait (100);
  }
#endif /* FEATURE_UIM_ATHERTON */
#endif
  return;
}/* uimdrv_rumi_enable_uart_clock */


/*===========================================================================
FUNCTION uimdrv_rumi_enable_sim_clock

DESCRIPTION
  Enabling the SIM CLOCK
===========================================================================*/
void uimdrv_rumi_enable_sim_clock(uim_instance_enum_type uim_instance_id)
{
#ifdef T_RUMI_EMULATION
  if(UIM_INSTANCE_1 == uim_instance_id)
  {
    UIM_HWIO_RUMI_OUT(MSS_UIM0_BCR_ADDR_RUMI,0x00);
    uim_clk_busy_wait (100);
    UIM_HWIO_RUMI_OUT(MSS_UIM0_CBCR_RUMI,0x01);
    uim_clk_busy_wait (100);
    UIM_HWIO_RUMI_OUT(MSS_UIM0_XO_CBCR_RUMI,0x01);
    uim_clk_busy_wait (100);
  }
#ifndef FEATURE_UIM_ATHERTON
  else if(UIM_INSTANCE_2 == uim_instance_id)
  {
    UIM_HWIO_RUMI_OUT(MSS_UIM1_BCR_ADDR_RUMI,0x00);
    uim_clk_busy_wait (100);
    UIM_HWIO_RUMI_OUT(MSS_UIM1_CBCR_RUMI,0x01);
    uim_clk_busy_wait (100);
    UIM_HWIO_RUMI_OUT(MSS_UIM1_XO_CBCR_RUMI,0x01);
    uim_clk_busy_wait (100);
  }
#endif /* FEATURE_UIM_ATHERTON */
#endif
  return;
}/* uimdrv_rumi_enable_simt_clock */


/*===========================================================================
FUNCTION uimdrv_rumi_enable_h_clock

DESCRIPTION
  Enabling the HCLOCK CLOCK
===========================================================================*/
void uimdrv_rumi_enable_h_clock(uim_instance_enum_type uim_instance_id)
{
#ifdef T_RUMI_EMULATION
  uint32 uimdrv_rumi_current_bus_uim_cbcr_value = 0x00;
  if(UIM_INSTANCE_1 == uim_instance_id)
  {
    uimdrv_rumi_current_bus_uim_cbcr_value = UIM_HWIO_RUMI_IN(MSS_BUS_UIM0_CBCR_RUMI);
    UIM_HWIO_RUMI_OUT(MSS_BUS_UIM0_CBCR_RUMI,(uimdrv_rumi_current_bus_uim_cbcr_value |0x1));
  }
#ifndef FEATURE_UIM_ATHERTON
  else if(UIM_INSTANCE_2 == uim_instance_id)
  {
    uimdrv_rumi_current_bus_uim_cbcr_value = UIM_HWIO_RUMI_IN(MSS_BUS_UIM1_CBCR_RUMI);
    UIM_HWIO_RUMI_OUT(MSS_BUS_UIM1_CBCR_RUMI,(uimdrv_rumi_current_bus_uim_cbcr_value |0x1));
  }
#endif /* FEATURE_UIM_ATHERTON */

#endif
  return;
}/* uimdrv_rumi_enable_h_clock */


/*===========================================================================
FUNCTION uimdrv_rumi_set_baud_rate_and_csr

DESCRIPTION
  Setting the baud rate
===========================================================================*/
void uimdrv_rumi_set_baud_rate_and_csr(uim_instance_enum_type uim_instance_id, word Fi, word Di)
{
#ifdef T_RUMI_EMULATION
  uint32 loop_idx=0x0;
  mnd_csr_config_type mnd_csr_cofig;
  uint32  uim_mnd_cfg_rcgr_rumi_value;

  /* Set the MND, CSR and configuration global is_300MHz_source_RUMI */
  if(is_300MHz_source_RUMI)
  {
    uim_mnd_cfg_rcgr_rumi_value  = UIM_MND_CFG_RCGR_V1_4_2;
    if(uim_hw_if.clkreg[uim_instance_id].m_simClkFreq ==
    uim_clock_frequency[UIMDRV_CLK_FREQ_4_8_MHZ])
    {
      mnd_csr_cofig = UIMDRV_MND_CSR_MAX_UARTCLK_4_8MHZ_300MHz;
    }
    else
    {
      mnd_csr_cofig = UIMDRV_MND_CSR_MAX_UARTCLK_3_8MHZ_300MHz;
    }
  }
  else
  {
    uim_mnd_cfg_rcgr_rumi_value  = UIM_MND_CFG_RCGR_V1_4_1;
    if(uim_hw_if.clkreg[uim_instance_id].m_simClkFreq ==
    uim_clock_frequency[UIMDRV_CLK_FREQ_4_8_MHZ])
    {
      mnd_csr_cofig = UIMDRV_MND_CSR_MAX_UARTCLK_4_8MHZ_19P2MHz;
    }
    else
    {
      mnd_csr_cofig = UIMDRV_MND_CSR_MAX_UARTCLK_3_8MHZ_19P2MHz;
    }
  }

  if(UIM_INSTANCE_1 == uim_instance_id)
  {
    UIM_HWIO_RUMI_OUT(MSS_UIM0_MND_CFG_RCGR_RUMI, uim_mnd_cfg_rcgr_rumi_value);
    uim_clk_busy_wait (100);
    for(loop_idx=0; uimdrv_rumi_mnd_csr_reg[loop_idx].mnd_csr[mnd_csr_cofig].mreg; loop_idx++)
    {
      if( (Fi == uimdrv_rumi_mnd_csr_reg[loop_idx].FI) &&
          (Di == uimdrv_rumi_mnd_csr_reg[loop_idx].DI))
      {
        /* Program MND */
        UIM_HWIO_RUMI_OUT(MSS_UIM0_MND_CMD_RCGR_RUMI,0x0);
        uim_clk_busy_wait(100);
        UIM_HWIO_RUMI_OUT(MSS_UIM0_MND_M_RUMI,uimdrv_rumi_mnd_csr_reg[loop_idx].mnd_csr[mnd_csr_cofig].mreg);
        uim_clk_busy_wait(100);
        UIM_HWIO_RUMI_OUT(MSS_UIM0_MND_N_RUMI,uimdrv_rumi_mnd_csr_reg[loop_idx].mnd_csr[mnd_csr_cofig].nreg);
        uim_clk_busy_wait(100);
        UIM_HWIO_RUMI_OUT(MSS_UIM0_MND_D_RUMI,uimdrv_rumi_mnd_csr_reg[loop_idx].mnd_csr[mnd_csr_cofig].dreg);
        uim_clk_busy_wait(100);
        UIM_HWIO_RUMI_OUT(MSS_UIM0_MND_CMD_RCGR_RUMI,0x3);
        uim_clk_busy_wait(100);
        /* Program CSR */
        UIM_HWIO_RUMI_OUT(MSS_UIM0_UART_DM_CSR_RUMI, uimdrv_rumi_mnd_csr_reg[loop_idx].mnd_csr[mnd_csr_cofig].csr);
        break;
      }
    }
  }
#ifndef FEATURE_UIM_ATHERTON
  else if(UIM_INSTANCE_2 == uim_instance_id)
  {
    UIM_HWIO_RUMI_OUT(MSS_UIM1_MND_CFG_RCGR_RUMI,uim_mnd_cfg_rcgr_rumi_value);
    uim_clk_busy_wait (100);
    for(loop_idx=0; uimdrv_rumi_mnd_csr_reg[loop_idx].mnd_csr[mnd_csr_cofig].mreg; loop_idx++)
    {
      if( (Fi == uimdrv_rumi_mnd_csr_reg[loop_idx].FI) &&
          (Di == uimdrv_rumi_mnd_csr_reg[loop_idx].DI)
        )
      {
        UIM_HWIO_RUMI_OUT(MSS_UIM1_MND_CMD_RCGR_RUMI,0x0);
        uim_clk_busy_wait(100);
        UIM_HWIO_RUMI_OUT(MSS_UIM1_MND_M_RUMI,uimdrv_rumi_mnd_csr_reg[loop_idx].mnd_csr[mnd_csr_cofig].mreg);
        uim_clk_busy_wait(100);
        UIM_HWIO_RUMI_OUT(MSS_UIM1_MND_N_RUMI,uimdrv_rumi_mnd_csr_reg[loop_idx].mnd_csr[mnd_csr_cofig].nreg);
        uim_clk_busy_wait(100);
        UIM_HWIO_RUMI_OUT(MSS_UIM1_MND_D_RUMI,uimdrv_rumi_mnd_csr_reg[loop_idx].mnd_csr[mnd_csr_cofig].dreg);
        uim_clk_busy_wait(100);
        UIM_HWIO_RUMI_OUT(MSS_UIM1_MND_CMD_RCGR_RUMI,0x3);
        uim_clk_busy_wait(100);
        /* Program CSR */
        UIM_HWIO_RUMI_OUT(MSS_UIM1_UART_DM_CSR_RUMI, uimdrv_rumi_mnd_csr_reg[loop_idx].mnd_csr[mnd_csr_cofig].csr);
        break;
      }
    }
  }
#endif /* FEATURE_UIM_ATHERTON */
#endif
  return;
}/* uimdrv_rumi_set_baud_rate_and_csr */


/*===========================================================================
FUNCTION uimdrv_rumi_set_clk_freq_3_84Mhz

DESCRIPTION
  Configuring the clock @3.84MHz
===========================================================================*/
void uimdrv_rumi_set_clk_freq_3_84Mhz(uim_instance_enum_type uim_instance_id)
{
#ifdef T_RUMI_EMULATION
  /* For xo/5 = 3.84MHz, write below register with 0x9 */
    UIM_HWIO_RUMI_OUT(MSS_UIM_DIV_CDIVR_RUMI,0x04);
    uim_clk_busy_wait(100);
#endif
  return;
}/* uimdrv_rumi_set_clk_freq_3_84Mhz */


/*===========================================================================
FUNCTION uimdrv_rumi_set_clk_freq_4_8Mhz

DESCRIPTION
  Configuring the clock @4.8MHz
===========================================================================*/
void uimdrv_rumi_set_clk_freq_4_8Mhz(uim_instance_enum_type uim_instance_id)
{

#ifdef T_RUMI_EMULATION
  /* For xo/4 = 4.8MHz, write below register with 0x7 */
    UIM_HWIO_RUMI_OUT(MSS_UIM_DIV_CDIVR_RUMI,0x03);
    uim_clk_busy_wait(100);
#endif
  return;
} /* uimdrv_rumi_set_clk_freq_4_8Mhz */
