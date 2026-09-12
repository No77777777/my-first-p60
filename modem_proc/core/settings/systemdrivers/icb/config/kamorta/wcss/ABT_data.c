/*==============================================================================

FILE:      ABT_data.c

DESCRIPTION: This file contains target/platform specific configuration data.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

//#CHANGE - Update when put in the depot
$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/icb/config/kamorta/wcss/ABT_data.c#3 $ 
$DateTime: 2020/07/26 11:21:23 $
$Author: pwbldsvc $
$Change: 25101703 $ 

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2016/05/26  bc     Adding WAHB power down timeout with fake syndrome registers
                   because it has none. 
2016/05/20  bc     Removing duplicate headers, and reducing public API
2016/01/07  pm     Changes for MSM8998 Adrastea 
2015/01/15  tb     Port for MSM8996
2014/12/22  tb     Added optional disable for clocks
2014/07/17  pm     Changed MSS_TOP region name to MODEM_TOP
2014/06/24  tb     Port for MDM9x45
2013/11/14  tb     Added support for multiple enable/status registers
2013/11/14  pm     Port to MSM8916
2013/04/16  pm     Added interrupt priority
2012/10/04  av     Support for disabling ABT 
2012/05/31  av     Created
 
        Copyright (c) 2012 - 2016 QUALCOMM Technologies, Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
==============================================================================*/
#include "ABT_cfg.h"
#include "ABTimeout_HWIO_WCSS.h"

#define ABT_OUT_OF_RANGE_SHFT 0xFF // uint8 to indicate out-of-range bit position

/*============================================================================
                        DEVICE CONFIG PROPERTY DATA
============================================================================*/

/*---------------------------------------------------------------------------*/
/*          Properties data for device ID  = "/dev/ABTimeout"                */
/*---------------------------------------------------------------------------*/

/* ABT HWIO map array */
static ABT_platform_hwio_info_type hwio_map_info[] =
{
  {"CORE_TOP_CSR" , 0x0,},
  {"WCSS_WRAPPER" , 0x0,},
};

/* The HW team has added an interrupt for power down read timeouts on
 * WAHB, but did not include any timeout block for it.  This is a virtual
 * timeout block.  Note the "ACCESS OFF"(0xACCE50FF) values are flags
 * to indicate that these addresses are fake, since none are captured
 * by HW.  */
static uint32_t WAHB_PWR_DWN_virtual_regs[14] =
{
   0x10000000, /**< ABT_HW_VERSION offset */
   0x00000000, /**< ABT_INST_ID offset */
   0x00000001, /**< ABT_NUM_SLAVES offset */
   0x000000FF, /**< ABT_TIMER_LOADVAL offset */
   0x00000001, /**< ABT_MODE offset */
   0x00000000, /**< ABT_INTR_STATUS offset */
   0x00000000, /**< ABT_INTR_CLEAR offset */
   0x00000000, /**< ABT_INTR_ENABLE offset */
   0x00000001, /**< ABT_SYND_VALID offset */
   0x00000000, /**< ABT_SYND_CLEAR offset */
   0xACCE50FF, /**< ABT_SYND_ID offset */
   0xACCE50FF, /**< ABT_SYND_ADDR0 offset */
   0xACCE50FF, /**< ABT_SYND_ADDR1 offset */
   0xFFFFFFFF, /**< ABT_SYND_HREADY offset */   
};

/* ABT Configuration Data*/
static ABT_slave_info_type ABT_cfgdata[] = 
{ 
  [0] = {
    .name              = "ECAHB",
    .slave_id          = HWIO_WCSS_SR_SR_INTERRUPT_STATUS_ECAHB_TSLV_INTR_SHFT,
    .base_addr         = (void*)WCSS_ECAHB_TSLV_REG_BASE,
    .clk_name_array    = NULL,
    .clk_disable_array = NULL,
    .num_of_clk        = 0, 
    .slave_enable      = false,
    .intr_enable       = false,
    .timeout_val       = 0xFF,
    .is_noc            = false,
    .wakeup_id         = ABT_OUT_OF_RANGE_SHFT  // No wakeup needed
  },
  [1] = {
    .name              = "WAHB_PWR_DWN",
    .slave_id          = (HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_WAHB_PWR_DWN_ACC_ERR_INT_SHFT + 64),
    .base_addr         = (&WAHB_PWR_DWN_virtual_regs),
    .clk_name_array    = NULL,
    .clk_disable_array = NULL,
    .num_of_clk        = 0,
    .slave_enable      = true,
    .intr_enable       = true,
    .timeout_val       = 0,
    .is_noc            = false,
    .wakeup_id         = (HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WAHB_PWR_DWN_ACC_ERR_INT_SHFT + 32),
  },
  [2] = {
    .name              = "WAHB",
    .slave_id          = (HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WAHB_AHB_TSLV_INTR_SHFT + 32),
    .base_addr         = (void*)WCSS_HM_A_WCSS_WAHB_AHB_TSLV_WFSS_WAHB_AHB_TSLV_REG_BASE,
    .clk_name_array    = NULL,
    .clk_disable_array = NULL,
    .num_of_clk        = 0,
    .slave_enable      = true,
    .intr_enable       = true,
    .timeout_val       = 220,  // 6.8 us - AHT has dependency on ECAHB.  Have smaller timeout for trigger first
    .is_noc            = false,
    .wakeup_id         = HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_WAHB_AHB_TSLV_INTR_SHFT,
  },
  [3] = {
    .name              = "WAPB",
    .slave_id          = (HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WAHB_APB_TSLV_INTR_SHFT + 32),
    .base_addr         = (void*)WCSS_HM_A_WIFI_APB_1_A_WCSS_WAHB_APB_TSLV_WFSS_WAHB_APB_TSLV_REG_BASE,
    .clk_name_array    = NULL,
    .clk_disable_array = NULL,
    .num_of_clk        = 0,
    .slave_enable      = true,
    .intr_enable       = true,
    .timeout_val       = 185,   // 5.8 us - APB has dependency on AHB.  Have smaller timeout for trigger first
    .is_noc            = false,
    .wakeup_id         = (HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WAHB_APB_TSLV_INTR_SHFT + 32),
  },
   // NOCs -- always place NOCs last on the list, so that ABTs can print out 
   // their syndrome information before NOC ERR_FATALs inside of 
   // NOC_Error_Handle_Interrupt()
  [4] = {
    .name              = "WNOC",
    .slave_id          = (HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_NOC_WCMN_INTR_SHFT + 32),
    .base_addr         = (void*)0x00000000,  // Dummy address that will not be used
    .clk_name_array    = NULL,
    .clk_disable_array = NULL,
    .num_of_clk        = 0,
    .slave_enable      = true,
    .intr_enable       = true,
    .timeout_val       = 185,   // 5.8 us - APB has dependency on AHB.  Have smaller timeout for trigger first
    .is_noc            = true, 
    .wakeup_id         = (HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_NOC_WCMN_INTR_SHFT + 32),
  },  
};

/* ABT Platform Data type */
static ABT_platform_info_type ABT_platform_info =
{
    .name = "WCSS", 
    .intr_map_addr =    (void*[]) /* Interrupt Enable Addresses */
      { (void*)HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ADDR,
        (void*)HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_ADDR,
        (void*)HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_ADDR },
    .intr_status_addr = (void*[]) /* Interrupt Status Addresses */
      { (void*)HWIO_WCSS_SR_SR_INTERRUPT_STATUS_ADDR,
        (void*)HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_ADDR,
        (void*)HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_ADDR },
    .num_status_reg =   3, /* Number of register in intr_map_addr */

    .globa_en_addr = (void*)HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_ADDR, 
    .intr_vector =  469, /* o_wcss_qdsp_intr[24] interrupt*/ 
    .intr_priority = 1,  /* ABT Interrupt priority */
    .hwio_map_info = hwio_map_info, 
    .num_hwio_regions = sizeof(hwio_map_info)/sizeof(hwio_map_info[0]),

    .wakeup_addr =    (void*[]) /* Wakeup Interrupt Enable Addresses */
      { (void*)HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_ADDR,
        (void*)HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_ADDR },
    .num_wakeup_reg = 2
};


/* ABT Configuration Property Data*/
ABT_propdata_type ABT_propdata = 
{
    /* Length of the config  data array */
    .len = sizeof(ABT_cfgdata)/sizeof(ABT_slave_info_type), 
    /* Pointer to config data array */ 
    .slave_info = ABT_cfgdata,
    /* Pointer to platform info data */ 
    .platform_info = &ABT_platform_info                                    
};

