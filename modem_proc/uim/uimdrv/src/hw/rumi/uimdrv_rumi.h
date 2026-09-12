#ifndef UIMDRV_RUMI_H
#define UIMDRV_RUMI_H

/*============================================================================
  FILE:        uimdrv_rumi.h

  OVERVIEW:    File contains RUMI specific driver code.

  DEPENDENCIES: N/A

                Copyright (c) 2013, 2015, 2019, 2021 QUALCOMM Technologies, Inc(QTI).
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hw/rumi/uimdrv_rumi.h#4 $
$DateTime: 2021/07/22 08:32:53 $
$Author: pwbldsvc $

  when      who     what, where, why
------      ----    -----------------------------------------------------------
07/20/21    ykr     QTANG2 RUMI changes
07/08/19    srk     Kamorta RUMI changes
07/15/15    ll      Add back 300MHz UART_BIT_CLK MND CSR table
06/22/15    ll      Atlas HW CR -- Add 300MHz GPLL for UART bit clk source RUMI support
04/28/15    ll      9x55 Bring-up changes
01/16/15    na      Suppport of RUMI
07/09/13    ak      Created    
============================================================================*/
#include "uimdrv_clk.h"
#include "comdef.h"
#include "uimdrv_main.h"

#define UIM_MAX_MND_CSR_CONFIG_RUMI                        4

#define UIM1_RUMI_DATA_GPIO_NUM                            55
#define UIM1_RUMI_CLOCK_GPIO_NUM                           56
#define UIM1_RUMI_RESET_GPIO_NUM                           57
#define UIM1_RUMI_PRESENT_GPIO_NUM                         58

#define UIM2_RUMI_DATA_GPIO_NUM                            51
#define UIM2_RUMI_CLOCK_GPIO_NUM                           52
#define UIM2_RUMI_RESET_GPIO_NUM                           53
#define UIM2_RUMI_PRESENT_GPIO_NUM                         54


#define UIM_RUMI_BATTERY_GPIO_NUM                          69
/* For Instance 1 */
#define UIM_RESET_DEFAULT_GPIO  \
        DAL_GPIO_CFG(UIM1_RUMI_RESET_GPIO_NUM, 0, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM_DATA_DEFAULT_GPIO  \
        DAL_GPIO_CFG(UIM1_RUMI_DATA_GPIO_NUM, 0, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM_CLK_DEFAULT_GPIO \
        DAL_GPIO_CFG(UIM1_RUMI_CLOCK_GPIO_NUM, 0,(uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM_RESET_UIMCNTRL_GPIO  \
        DAL_GPIO_CFG(UIM1_RUMI_RESET_GPIO_NUM, 1, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM_DATA_UIMCNTRL_GPIO  \
        DAL_GPIO_CFG(UIM1_RUMI_DATA_GPIO_NUM, 1, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM_CLK_UIMCNTRL_GPIO \
        DAL_GPIO_CFG(UIM1_RUMI_CLOCK_GPIO_NUM, 1,(uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM_HOTSWAP_GPIO \
        DAL_GPIO_CFG(UIM1_RUMI_PRESENT_GPIO_NUM, 1, (uint32)DAL_GPIO_INPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM_BATT_ALARM_UIMCNTRL_GPIO  \
        DAL_GPIO_CFG(UIM_RUMI_BATTERY_GPIO_NUM, 0, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM_RESET_GPIO_PU  \
        DAL_GPIO_CFG(UIM1_RUMI_RESET_GPIO_NUM, 0, (uint32)DAL_GPIO_INPUT, (uint32)DAL_GPIO_PULL_UP, (uint32)DAL_GPIO_2MA)

#define UIM_CLK_GPIO_PU    \
        DAL_GPIO_CFG(UIM1_RUMI_CLOCK_GPIO_NUM, 0, (uint32)DAL_GPIO_INPUT, (uint32)DAL_GPIO_PULL_UP, (uint32)DAL_GPIO_2MA)

#define UIM_DATA_GPIO_PU   \
        DAL_GPIO_CFG(UIM1_RUMI_DATA_GPIO_NUM, 0, (uint32)DAL_GPIO_INPUT, (uint32)DAL_GPIO_PULL_UP, (uint32)DAL_GPIO_2MA)

#define UIM_HOTSWAP_GPIO_PU\
        DAL_GPIO_CFG(UIM1_RUMI_PRESENT_GPIO_NUM, 0, (uint32)DAL_GPIO_INPUT, (uint32)DAL_GPIO_PULL_UP, (uint32)DAL_GPIO_2MA)
/* For Instance 2 */

#define UIM2_RESET_DEFAULT_GPIO  \
        DAL_GPIO_CFG(UIM2_RUMI_RESET_GPIO_NUM, 0, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM2_DATA_DEFAULT_GPIO  \
        DAL_GPIO_CFG(UIM2_RUMI_DATA_GPIO_NUM, 0, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM2_CLK_DEFAULT_GPIO \
        DAL_GPIO_CFG(UIM2_RUMI_CLOCK_GPIO_NUM, 0,(uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM2_RESET_UIMCNTRL_GPIO  \
        DAL_GPIO_CFG(UIM2_RUMI_RESET_GPIO_NUM, 1, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM2_DATA_UIMCNTRL_GPIO  \
        DAL_GPIO_CFG(UIM2_RUMI_DATA_GPIO_NUM, 1, (uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM2_CLK_UIMCNTRL_GPIO \
        DAL_GPIO_CFG(UIM2_RUMI_CLOCK_GPIO_NUM, 1,(uint32)DAL_GPIO_OUTPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM2_HOTSWAP_GPIO \
        DAL_GPIO_CFG(UIM2_RUMI_PRESENT_GPIO_NUM, 1, (uint32)DAL_GPIO_INPUT, (uint32)DAL_GPIO_NO_PULL, (uint32)DAL_GPIO_2MA)

#define UIM2_RESET_GPIO_PU  \
        DAL_GPIO_CFG(UIM2_RUMI_RESET_GPIO_NUM, 0, (uint32)DAL_GPIO_INPUT, (uint32)DAL_GPIO_PULL_UP, (uint32)DAL_GPIO_2MA)

#define UIM2_CLK_GPIO_PU    \
        DAL_GPIO_CFG(UIM2_RUMI_CLOCK_GPIO_NUM, 0, (uint32)DAL_GPIO_INPUT, (uint32)DAL_GPIO_PULL_UP, (uint32)DAL_GPIO_2MA)

#define UIM2_DATA_GPIO_PU   \
        DAL_GPIO_CFG(UIM2_RUMI_DATA_GPIO_NUM, 0, (uint32)DAL_GPIO_INPUT, (uint32)DAL_GPIO_PULL_UP, (uint32)DAL_GPIO_2MA)

#define UIM2_HOTSWAP_GPIO_PU\
        DAL_GPIO_CFG(UIM2_RUMI_PRESENT_GPIO_NUM, 0, (uint32)DAL_GPIO_INPUT, (uint32)DAL_GPIO_PULL_UP, (uint32)DAL_GPIO_2MA)

typedef enum {
  UIMDRV_MND_CSR_MAX_UARTCLK_3_8MHZ_19P2MHz,  /* Atlas and forward MND CSR RUMI config for 3.84MHZ SIM clk with 19.2MHz source clk */
  UIMDRV_MND_CSR_MAX_UARTCLK_4_8MHZ_19P2MHz,  /* Atlas and forward MND CSR RUMI config for 4.8MHZ SIM clk with 19.2MHz source clk*/
  UIMDRV_MND_CSR_MAX_UARTCLK_3_8MHZ_300MHz,   /* Atlas and forward MND CSR RUMI config for 3.84MHZ SIM clk with 300MHz source clk */
  UIMDRV_MND_CSR_MAX_UARTCLK_4_8MHZ_300MHz,   /* Atlas and forward MND CSR RUMI config for 4.8MHZ SIM clk with 300MHz source clk*/
}mnd_csr_config_type;

typedef struct
{
  word mreg;
  word nreg;
  word dreg;
  word csr;
} mnd_csr_record_type;


typedef struct 
{
 uim_clock_rate_conversion_factor_type   FI;
 uim_baud_rate_adjustment_factor_type    DI;
 mnd_csr_record_type                     mnd_csr[UIM_MAX_MND_CSR_CONFIG_RUMI];
}uimdrv_rumi_fi_di_mnd_reg_map_type;

/*===========================================================================
FUNCTION uimdrv_rumi_config_gpios

DESCRIPTION
  Configures the GPIOs connected to the V ENABLE and V SELECT lines.  
  For RUMI only.

===========================================================================*/
void uimdrv_rumi_config_gpios(uim_instance_enum_type uim_instance_id);

/*===========================================================================
FUNCTION uimdrv_rumi_vsel_off

DESCRIPTION
  Disables the V ENABLE line and sets the V SELECT line to 1.8v.  
  For RUMI only.
===========================================================================*/
void uimdrv_rumi_vsel_off(uim_instance_enum_type uim_instance_id);

/*===========================================================================
FUNCTION uimdrv_rumi_vsel_on_class_c

DESCRIPTION
  Turns on the V_ENABLE line and sets the V_SELECT line for 1.8v operation.
  For RUMI only.
===========================================================================*/
void uimdrv_rumi_vsel_on_class_c(uim_instance_enum_type uim_instance_id);

/*===========================================================================
FUNCTION uimdrv_rumi_vsel_on_class_b

DESCRIPTION
  Turns on the V_ENABLE line and sets the V_SELECT line for 3v operation.
  For RUMI only.
===========================================================================*/
void uimdrv_rumi_vsel_on_class_b(uim_instance_enum_type uim_instance_id);

/*===========================================================================
FUNCTION uimdrv_rumi_enable_uart_clock

DESCRIPTION
  Enabling the UART CLOCK
===========================================================================*/
void uimdrv_rumi_enable_uart_clock(uim_instance_enum_type uim_instance_id);

/*===========================================================================
FUNCTION uimdrv_rumi_enable_sim_clock

DESCRIPTION
  Enabling the SIM CLOCK
===========================================================================*/
void uimdrv_rumi_enable_sim_clock(uim_instance_enum_type uim_instance_id);

/*===========================================================================
FUNCTION uimdrv_rumi_enable_h_clock

DESCRIPTION
  Enabling the HCLOCK CLOCK
===========================================================================*/
void uimdrv_rumi_enable_h_clock(uim_instance_enum_type uim_instance_id);

/*===========================================================================
FUNCTION uimdrv_rumi_set_baud_rate_and_csr

DESCRIPTION
  Setting the baud rate
===========================================================================*/
void uimdrv_rumi_set_baud_rate_and_csr(uim_instance_enum_type uim_instance_id, word Fi, word Di);

/*===========================================================================
FUNCTION uimdrv_rumi_set_clk_freq_3_84Mhz

DESCRIPTION
  Configuring the clock @3.84MHz
===========================================================================*/
void uimdrv_rumi_set_clk_freq_3_84Mhz(uim_instance_enum_type uim_instance_id);

/*===========================================================================
FUNCTION uimdrv_rumi_set_clk_freq_4_8Mhz

DESCRIPTION
  Configuring the clock @4.8MHz
===========================================================================*/
void uimdrv_rumi_set_clk_freq_4_8Mhz(uim_instance_enum_type uim_instance_id);

#endif /* UIMDRV_RUMI_H */
