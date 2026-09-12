/*============================================================================
  FILE:         uimdrv_enumeration.c

  OVERVIEW:     File deals with the configuration and reading of the efs file
                along with enabling defaults according to the chipset got by
                querying the Dal api to get the chip id.

  DEPENDENCIES: N/A

                Copyright (c) 2012 - 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/enumeration/uimdrv_enumeration.c#9 $
$DateTime: 2021/04/06 04:29:14 $
$Author: pwbldsvc $

 when       who        what, where, why
------      ----       -----------------------------------------------------------
04/05/21    ykr        Divar BU changes
12/23/20    ykr        Atherton BU changes
11/03/20    rkg        QTNAG2 BU changes
06/30/20    ykr        Update slot_availabe check in the HWconfig info
02/20/20    rkg        Update only active slots in uim_get_hw_slot_info
02/13/20    ykr        Agatti BU changes
07/08/19    srk        Kamorta RUMI changes
07/02/18    ku         Consistency in uim_apply_defaults
05/21/18    ku         Support Triple sim using sub manager
03/12/18    ak         Read the UIM conniguration before the Task Initialized
03/09/18    ak         SDM632 bringup changes
08/18/16    gm         8920 bringup changes
05/31/16    gm         F3 message removal
05/27/16    ks         Removal of zero value check for GPIO num in uim_hw_config
04/06/16    sam        8940 bringup changes
04/01/16    ks         8976/56 Bring up change
03/28/16    sam        move declaration of'uim_get_chip_index_from_table' to a header 
03/03/16    na         Added support of external pull up
21/01/16    ssr        8953 Bring up changes
11/05/15    ll         8998 Bring up change
10/27/15    hyo        Avoid writing enum to EFS
09/24/15    ll         Add guards depends on UARTDM version and clock tree HW
08/17/15    ll         Enable HOTSWAP by default for CUST_1
08/11/15    hyo        usb uicc removal
07/20/15    ks         Consolidating UIM bring-up changes
05/21/15    hyo        Disable UIM2 for Fluid/Liquid when we read HWConfig NV
05/14/15    hyo        Disable UIM2 for Fluid platform
05/08/15    hyo        Disable UIM2 for Liquid platform
04/28/15    ll         9x55 Bring-up changes
04/08/15    sam        FR24498: Powerup logging new changes
04/06/15    ll         UIMDRV migrates to UIM COMMON EFS APIs for all EFS items
03/03/15    hyo        HW Config validation update
03/03/15    hyo        8996 Bring-up changes
03/02/15    na         Using the DAL chip enums instead of hardcoding values for 8952
02/17/15    hyo        Check for nearly all zero HW Config and apply default values
01/16/15    na         Suppport of RUMI
01/07/15    sam        Modification of msg macros for FR24498-UIM Powerup Logging
12/02/14    ks         Feature to support GPIO based hotswap
11/24/14    kr         Fixed Compilation warnings
11/11/14    ks         Using the DAL chip enums instad of hardcoding values
11/06/14    nmb        8952 bring up changes
09/23/14    ks         8909 bring up changes
08/25/14    akv        UIM disablement for all chipsets when RUMI_EMULATION flag defined
08/21/14    ak         Replace 9x45 DAL chip family reference for compilation on BOLT
08/20/14    nmb        9x45 Bring up changes
08/14/14    ks         Hotswap enhancement for flaky mechanical switches
07/04/14    ssr        Fix for APDU debug Mask
06/27/14    ks         Fixed to update the m_hwconfig global appropriately
06/23/14    ssr        Fixed to enable the SWP protocol by default for Slot1
06/16/14    ll         Switch to new string based TLMM APIs
05/12/14    ak         8994 Bring up changes
05/06/14    ks         8936 bring up changes
04/11/14    na         Added support for SWP NV item to encode SWP in Terminal Capability
04/01/14    lxu        Use new uim remote power down API to deal with card power
                       down command,set nv 70210 default version to 2
03/27/14    lxu        USB UICC uimdrv implement
02/18/14    sam        Usage of newer style diag macros
02/04/14    sam        Fix for Klocwork issues
01/31/14    ak         Removed uimBooleanEnum type
12/27/13    ks         8916 bring up changes
11/27/13    akv        Initialization of hw enumeration based on dal chip family
11/27/13    akv        Changes for voting against/for sleep on 9x35
10/31/13    ll         Enable both UIM1 and UIM2 by default for 8974pro and 8926
10/16/13    ak         Fix to ensure that disableUim == TRUE for disabled interfaces
10/09/13    ks         Fix in uim_get_hw_max_slots_available func to return number
                       of slots appropriately
09/30/13    akv        Removal of if-else cases and unused code in hw enumeration
09/11/13    na         8x62 bring up changes
08/29/13    na         allow UIM2 config on UIM1 when UIM1 is disabled and UIM2 enabled
08/22/13    js         Add 8926 chip family check
08/22/13    js         Add 8926 and 8974pro chip family checks
08/06/13    rm/ak      Bolt RUMI code
07/12/13    akv        HW Enumeration - modifying target specific hardware values
                       and organizing them in value-key pairs
06/24/13    js         TSTS changes
05/30/13    rm         Disable UIM2 by default on 8974
05/08/13    akv        Disabling UICC shutdown upon card removal by default on 8x26
04/10/13    nmb        8x10 bring up changes
03/18/13    js         8x26 UIM3 interface bring up fixes.
02/25/13    ak         Validate UIM1 GPIO values for 9x25
02/22/13    js         8x26 bring up changes
02/21/13    ak         9x25 GPIO defaults applied for both versions
02/21/13    ak         UIM1 support on 9x25 v2
02/12/13    js         General F3 macro cleanup
02/11/13    akv        UIM API for querying the available/enabled UIM slots
02/04/13    akv        Fix for using chip family instead of chip id for enumeration
02/01/13    akv        Run time config of UICC shutdown feature and code cleanup
12/05/12    js         UIM Parallel Processing changes
11/09/12    akv        BATT ALARM configuration for Dime
11/02/12    akv        Changes in HW Enumeration for 8x26 RUMI
10/26/12    akv/ssr    HW Enumeration changes
============================================================================*/
#include "uimdrv_enumeration.h"
#include "DDIChipInfo.h"
#include "DDIPlatformInfo.h"
#include "DALDeviceId.h"
#include "fs_public.h"

#include "uimdrv_gpio.h"
#include "uimdrv_clk.h"
#include "uimdrv_pmic.h"
#include "uimdrv_intctrl.h"
#include "uimdrv_uartdm.h"
#include "uimglobals.h"
#include "uim_common_efs.h"
#include "uim_slot_mapping.h"

#include "rex.h"

#define GPIO_NUM_DEFAULT             UIM_INVALID_GPIO_NUM
#define GPIO_FUNC_SEL_DEFAULT        GPIO_FUNC_SEL_1
#define GPIO_DRV_STRENGTH_DEFAULT    DS_2MA
#define GPIO_PULL_SETTING_DEFAULT    NO_PULL
#define GPIO_PULL_SETTING_PRESENT    PULL_UP

#define DALCHIPINFO_FAMILY_KAMORTA 106

/* Local forward declarations */
static void uim_fill_global_hw_config(const uimHWConfig *hw_config_ptr);
void configure_uim_enablement(DalChipInfoFamilyType  chipFamily,
                              DalChipInfoVersionType nChipVersion,
                              DalPlatformInfoPlatformType nPlatformType,
                              uimHWConfig       *hw_config_ptr);
uint8 uim_get_hw_max_slots_available(DalChipInfoFamilyType  chipFamily,
                                     DalPlatformInfoPlatformType nPlatformType);
void uim_set_hotswap_flaky_switch_support(const uimHWConfig *hw_config_ptr);
void uim_hw_config_input_validation(uimdrv_hw_nv_config_info_type *hw_nv_config_info,
                                            uimHWConfig *hw_config_ptr);

/* uimdrv_hw_nv_config_info structure holds all hw interface info got
   by reading NV UIM_HW_CONFIG_NV_EF */
uimdrv_hw_nv_config_info_type         uimdrv_hw_nv_config_info  = {0};
uim_hw_slot_info_type                 uim_hw_slot_info;

#define UIM_ONE_PHYSICAL_SLOT_AVAILABLE    0x01
#define UIM_TWO_PHYSICAL_SLOTS_AVAILABLE   0x02
#define UIM_THREE_PHYSICAL_SLOTS_AVAILABLE 0x03
#define UIM_FOUR_PHYSICAL_SLOTS_AVAILABLE  0x04

#define INVALID_IRQ_NUM  0xFFFF


static const uint16 uim_default_uartdm_num[UIM_INSTANCES_CONFIGURABLE] =
{
  BADGER_MSS_UIM0_UART_DM,
  BADGER_MSS_UIM1_UART_DM,
  INVALID_BADGER_MSS_UART_DM,
  INVALID_BADGER_MSS_UART_DM
};

static const uint16 uim_default_pmic_npa_resource[UIM_INSTANCES_CONFIGURABLE] =
{
  E_PMIC_NPA_RESOURCE_UIM1,
  E_PMIC_NPA_RESOURCE_UIM2,
  INVALID_PMIC_NPA_RESOURCE,
  INVALID_PMIC_NPA_RESOURCE
};

const uim_hw_param_type uim_hw_param[] =
{
  { DALCHIPINFO_FAMILY_MDM9x45 /* TESLA_9x45 */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, TRUE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    { 147, 148, 149, 150 },
    /* UART IRQs for 4 slots     */
    { 151, 152, 153, 154 },
    /* There is an external Pull Up */
    { TRUE, TRUE, FALSE, FALSE}
  },  /* TESLA_9x45 */

  { DALCHIPINFO_FAMILY_MSM8952  /* TABASCO_SAHI */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    {  89, 90, 254, INVALID_IRQ_NUM },
    /* UART IRQs for 4 slots     */
    {  87, 88, 253, INVALID_IRQ_NUM },
    /* There is an external Pull Up */
    { TRUE, TRUE, FALSE, FALSE}
  },  /* TABASCO_SAHI */

  { DALCHIPINFO_FAMILY_MSM8956  /* TABASCO_ELDARION */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    {  89, 90, 254, INVALID_IRQ_NUM },
    /* UART IRQs for 4 slots     */
    {  87, 88, 253, INVALID_IRQ_NUM },
    /* There is an external Pull Up */
    { TRUE, TRUE, FALSE, FALSE}
  },  /* TABASCO_ELDARION */

  { DALCHIPINFO_FAMILY_MSM8909 /* JOLOKIA_8909 */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, TRUE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    { 89, 90, 254, INVALID_IRQ_NUM },
    /* UART IRQs for 4 slots     */
    { 87, 88, 253, INVALID_IRQ_NUM },
    /* There is an external Pull Up */
    { TRUE, TRUE, TRUE, FALSE}
  },  /* TESLA_9x45 */

  { DALCHIPINFO_FAMILY_MDM9x55 /* McLaren_9x55 */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    TRUE, TRUE,
    /* HOTSWAP IRQs for 4 slots  */
    { 305, 306, 307, 308 },
    /* UART IRQs for 4 slots     */
    { 309, 310, 311, 312 },
    /* There is an external Pull Up */
    { TRUE, TRUE, FALSE, FALSE }
  },  /* McLaren_9x55 */

  { DALCHIPINFO_FAMILY_MSM8996 /* ISTARI_8996 */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    { 147, 148, 149, 150 },
    /* UART IRQs for 4 slots     */
    { 151, 152, 153, 154 },
    /* There is an external Pull Up */
    { TRUE, TRUE, TRUE, TRUE}
  },  /* ISTARI_8996 */

  { DALCHIPINFO_FAMILY_MSM8998 /* Nazgul_8998 */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    TRUE, TRUE,
    /* HOTSWAP IRQs for 4 slots  */
    { 305, 306, 307, 308 },
    /* UART IRQs for 4 slots     */
    { 309, 310, 311, 312 },
    /* There is no external Pull Up */
    { FALSE, FALSE, FALSE, FALSE}
  },  /* Nazgul_8998 */

  { DALCHIPINFO_FAMILY_MSM8953  /* TABASCO_JACALA */ ,
#if (defined(FEATURE_UIM_DS_SUBSCRIPTION_MANAGER) && defined (FEATURE_UIM_SUPPORT_TRIPLE_SLOTS))
    UIM_THREE_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, FALSE, TRUE,
#else
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
#endif
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    {  89, 90, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* UART IRQs for 4 slots     */
    {  87, 88, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* There is an external pull up */
    {  TRUE, TRUE, FALSE, FALSE }
  },  /* TABASCO_JACALA */

  { DALCHIPINFO_FAMILY_MSM8940  /* Feero_Cat6_8940 */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    {  89, 90, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* UART IRQs for 4 slots     */
    {  87, 88, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* There is an external pull up */
    {  FALSE, FALSE, FALSE, FALSE }
  },  /* Feero_Cat6_8940 */

  { DALCHIPINFO_FAMILY_MSM8920  /* FeeroLite_Cat6_8920 */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    {  89, 90, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* UART IRQs for 4 slots     */
    {  87, 88, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* There is an external pull up */
    {  FALSE, FALSE, FALSE, FALSE }
  },  /* FeeroLite_Cat6_8920 */

  { DALCHIPINFO_FAMILY_SDM632   /* SHANTI */ ,
      /* num_of_physical_slots  uim_disable flag for 4 slots */
      UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
      /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
      /* HOTSWAP IRQs for 4 slots  */
    {  89, 90, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
      /* UART IRQs for 4 slots     */
    {  87, 88, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
      /* There is an external pull up */
      {  TRUE, TRUE, FALSE, FALSE }
  },  /* SHANTI */

  { DALCHIPINFO_FAMILY_KAMORTA   /* KAMORTA */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    {  89, 90, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* UART IRQs for 4 slots     */
    {  87, 88, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* There is an external pull up */
    {  TRUE, TRUE, FALSE, FALSE }
  },  /* KAMORTA */
  
  { CHIPINFO_FAMILY_AGATTI   /* Agatti_QM2xx */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    {  89, 90, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* UART IRQs for 4 slots     */
    {  87, 88, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* There is an external pull up */
    {  TRUE, TRUE, FALSE, FALSE }
  },  /* Agatti_QM2xx */

#ifdef FEATURE_UIM_ATHERTON
  { CHIPINFO_FAMILY_ATHERTON   /* Atherton_SDW5100 */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, TRUE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    {  89, 90, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* UART IRQs for 4 slots     */
    {  87, 88, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* There is an external pull up */
    {  TRUE, TRUE, FALSE, FALSE }
  },  /* Atherton_SDW5100 */
#endif /* FEATURE_UIM_ATHERTON */

#ifdef FEATURE_UIM_QTANG2  
  { CHIPINFO_FAMILY_QTANG2   /* QTANG2 */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    {  89, 90, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* UART IRQs for 4 slots     */
    {  87, 88, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* There is an external pull up */
    {  TRUE, TRUE, FALSE, FALSE }
  }, 
#endif

#ifdef FEATURE_UIM_DIVAR
  { CHIPINFO_FAMILY_DIVAR   /* Divar_SM6225 */ ,
    /* num_of_physical_slots  uim_disable flag for 4 slots */
    UIM_TWO_PHYSICAL_SLOTS_AVAILABLE, FALSE, FALSE, TRUE, TRUE,
    /* SimClk_slot_diff_freq_support, UartClk_300MHz_source_support */
    FALSE, FALSE,
    /* HOTSWAP IRQs for 4 slots  */
    {  89, 90, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* UART IRQs for 4 slots     */
    {  87, 88, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* There is an external pull up */
    {  TRUE, TRUE, FALSE, FALSE }
  },  /* Divar_SM6225 */
#endif /* FEATURE_UIM_DIVAR */

  { DALCHIPINFO_FAMILY_UNKNOWN /* DALCHIPINFO_FAMILY_UNKNOWN */ ,
    0x00, TRUE, TRUE, TRUE, TRUE,
    FALSE, FALSE,
    { INVALID_IRQ_NUM, INVALID_IRQ_NUM, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    { INVALID_IRQ_NUM, INVALID_IRQ_NUM, INVALID_IRQ_NUM, INVALID_IRQ_NUM },
    /* No External Pull Up */
    { FALSE, FALSE, FALSE, FALSE}
  }  /* DALCHIPINFO_FAMILY_UNKNOWN */
};


/* This function copies values to m_HWConfig*/
static void uim_fill_global_hw_config(const uimHWConfig *hw_config_ptr)
{
  uint8 i = 0;

  if(hw_config_ptr == NULL)
  {
    return;
  }

  m_HWConfig.version                     = (uimVersionEnum)hw_config_ptr->version;
  m_HWConfig.uimBattAlarmGpioNum         = hw_config_ptr->uimBattAlarmGpioNum;
  m_HWConfig.uimBattAlarmGpioFuncSel     = hw_config_ptr->uimBattAlarmGpioFuncSel;
  m_HWConfig.uimBattAlarmGpioDrvStrength = 
                          (uimDriveStrengthEnum)hw_config_ptr->uimBattAlarmGpioDrvStrength;
  m_HWConfig.uimBattAlarmGpioPullSetting = 
                          (uimPullSettingEnum)hw_config_ptr->uimBattAlarmGpioPullSetting;
  m_HWConfig.uicc_shutdown_feature.uiccShutdownBattRemoval =
                          hw_config_ptr->uicc_shutdown_feature.uiccShutdownBattRemoval;
  uim_memscpy(m_HWConfig.uicc_shutdown_feature.uiccShutdownCardRemoval,
              sizeof(m_HWConfig.uicc_shutdown_feature.uiccShutdownCardRemoval),
              hw_config_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval,
              sizeof(hw_config_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval));
  uim_memscpy(m_HWConfig.usb_uicc_obsolete,
              sizeof(m_HWConfig.usb_uicc_obsolete),
              hw_config_ptr->usb_uicc_obsolete,
              sizeof(hw_config_ptr->usb_uicc_obsolete));
  uim_memscpy(m_HWConfig.uimSwpCapabilitySupported,
              sizeof(m_HWConfig.uimSwpCapabilitySupported),
              hw_config_ptr->uimSwpCapabilitySupported,
              sizeof(hw_config_ptr->uimSwpCapabilitySupported));
  uim_memscpy(m_HWConfig.uim_hotswap_flaky_switch,
              sizeof(m_HWConfig.uim_hotswap_flaky_switch),
              hw_config_ptr->uim_hotswap_flaky_switch,
              sizeof(hw_config_ptr->uim_hotswap_flaky_switch));

  for(i = 0; i < UIM_INSTANCES_CONFIGURABLE; i++)
  {
    m_HWConfig.uim_slot_config[i].disableUim =
                                      hw_config_ptr->uim_slot_config[i].disableUim;
    m_HWConfig.uim_slot_config[i].enableUimHotswap =
                                      hw_config_ptr->uim_slot_config[i].enableUimHotswap;
    m_HWConfig.uim_slot_config[i].uimHotswapPolarity = 
                                      (uimPolarityeEnum)hw_config_ptr->uim_slot_config[i].uimHotswapPolarity;
    m_HWConfig.uim_slot_config[i].uimControllerIRQNum =
                                      hw_config_ptr->uim_slot_config[i].uimControllerIRQNum;
    m_HWConfig.uim_slot_config[i].uimUart = 
                                      (uimUartEnumList)hw_config_ptr->uim_slot_config[i].uimUart;
    m_HWConfig.uim_slot_config[i].uimUartIRQNum =
                                      hw_config_ptr->uim_slot_config[i].uimUartIRQNum;
    m_HWConfig.uim_slot_config[i].uimVcc = 
                                      (uimVccEnum)hw_config_ptr->uim_slot_config[i].uimVcc;
    m_HWConfig.uim_slot_config[i].uimResetGpioNum =
                                      hw_config_ptr->uim_slot_config[i].uimResetGpioNum;
    m_HWConfig.uim_slot_config[i].uimResetGpioFuncSel =
                                      hw_config_ptr->uim_slot_config[i].uimResetGpioFuncSel;
    m_HWConfig.uim_slot_config[i].uimResetGpioDrvStrength = 
                                      (uimDriveStrengthEnum)hw_config_ptr->uim_slot_config[i].uimResetGpioDrvStrength;
    m_HWConfig.uim_slot_config[i].uimResetGpioPullSetting = 
                                      (uimPullSettingEnum)hw_config_ptr->uim_slot_config[i].uimResetGpioPullSetting;

    m_HWConfig.uim_slot_config[i].uimDataGpioNum =
                                      hw_config_ptr->uim_slot_config[i].uimDataGpioNum;
    m_HWConfig.uim_slot_config[i].uimDataGpioFuncSel =
                                      hw_config_ptr->uim_slot_config[i].uimDataGpioFuncSel;
    m_HWConfig.uim_slot_config[i].uimDataGpioDrvStrength = 
                                      (uimDriveStrengthEnum)hw_config_ptr->uim_slot_config[i].uimDataGpioDrvStrength;
    m_HWConfig.uim_slot_config[i].uimDataGpioPullSetting = 
                                      (uimPullSettingEnum)hw_config_ptr->uim_slot_config[i].uimDataGpioPullSetting;

    m_HWConfig.uim_slot_config[i].uimClkGpioNum =
                                      hw_config_ptr->uim_slot_config[i].uimClkGpioNum;
    m_HWConfig.uim_slot_config[i].uimClkGpioFuncSel =
                                      hw_config_ptr->uim_slot_config[i].uimClkGpioFuncSel;
    m_HWConfig.uim_slot_config[i].uimClkGpioDrvStrength = 
                                      (uimDriveStrengthEnum)hw_config_ptr->uim_slot_config[i].uimClkGpioDrvStrength;
    m_HWConfig.uim_slot_config[i].uimClkGpioPullSetting = 
                                      (uimPullSettingEnum)hw_config_ptr->uim_slot_config[i].uimClkGpioPullSetting;

    m_HWConfig.uim_slot_config[i].uimCardDetectGpioNum =
                                      hw_config_ptr->uim_slot_config[i].uimCardDetectGpioNum;
    m_HWConfig.uim_slot_config[i].uimCardDetectGpioFuncSel =
                                      hw_config_ptr->uim_slot_config[i].uimCardDetectGpioFuncSel;
    m_HWConfig.uim_slot_config[i].uimCardDetectGpioDrvStrength =
                                      (uimDriveStrengthEnum)hw_config_ptr->uim_slot_config[i].uimCardDetectGpioDrvStrength;
    m_HWConfig.uim_slot_config[i].uimCardDetectGpioPullSetting = 
                                      (uimPullSettingEnum)hw_config_ptr->uim_slot_config[i].uimCardDetectGpioPullSetting;
  }
} /* uim_fill_global_hw_config */


/* This function copies m_HWConfig values to uimHWConfigEfs struct to avoid
   writing enum to EFS */
static void uimHWConfig_copy_to_efs_type(uimHWConfigEfs *hw_config_efs_ptr,
                                         const uimHWConfig    *hw_config_ptr)
{
  uint8 i = 0;

  hw_config_efs_ptr->version                     = (uint8)hw_config_ptr->version;
  hw_config_efs_ptr->uimBattAlarmGpioNum         = hw_config_ptr->uimBattAlarmGpioNum;
  hw_config_efs_ptr->uimBattAlarmGpioFuncSel     = hw_config_ptr->uimBattAlarmGpioFuncSel;
  hw_config_efs_ptr->uimBattAlarmGpioDrvStrength = (uint8)hw_config_ptr->uimBattAlarmGpioDrvStrength;
  hw_config_efs_ptr->uimBattAlarmGpioPullSetting = (uint8)hw_config_ptr->uimBattAlarmGpioPullSetting;
  hw_config_efs_ptr->uicc_shutdown_feature.uiccShutdownBattRemoval =
                                      hw_config_ptr->uicc_shutdown_feature.uiccShutdownBattRemoval;
  uim_memscpy(hw_config_efs_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval,
              sizeof(hw_config_efs_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval),
              hw_config_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval,
              sizeof(hw_config_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval));
  uim_memscpy(hw_config_efs_ptr->usb_uicc_obsolete,
              sizeof(hw_config_efs_ptr->usb_uicc_obsolete),
              hw_config_ptr->usb_uicc_obsolete,
              sizeof(hw_config_ptr->usb_uicc_obsolete));
  uim_memscpy(hw_config_efs_ptr->uimSwpCapabilitySupported,
              sizeof(hw_config_efs_ptr->uimSwpCapabilitySupported),
              hw_config_ptr->uimSwpCapabilitySupported,
              sizeof(hw_config_ptr->uimSwpCapabilitySupported));
  uim_memscpy(hw_config_efs_ptr->uim_hotswap_flaky_switch,
              sizeof(hw_config_efs_ptr->uim_hotswap_flaky_switch),
              hw_config_ptr->uim_hotswap_flaky_switch,
              sizeof(hw_config_ptr->uim_hotswap_flaky_switch));
  for (i = 0; i < UIM_INSTANCES_CONFIGURABLE; i++)
  {
    hw_config_efs_ptr->uim_slot_config[i].disableUim =
                              hw_config_ptr->uim_slot_config[i].disableUim;
    hw_config_efs_ptr->uim_slot_config[i].enableUimHotswap =
                              hw_config_ptr->uim_slot_config[i].enableUimHotswap;
    hw_config_efs_ptr->uim_slot_config[i].uimHotswapPolarity = 
                              (uint8)hw_config_ptr->uim_slot_config[i].uimHotswapPolarity;
    hw_config_efs_ptr->uim_slot_config[i].uimUart = 
                              (uint8)hw_config_ptr->uim_slot_config[i].uimUart;
    hw_config_efs_ptr->uim_slot_config[i].uimVcc = 
                              (uint8)hw_config_ptr->uim_slot_config[i].uimVcc;
    hw_config_efs_ptr->uim_slot_config[i].uimControllerIRQNum =
                              hw_config_ptr->uim_slot_config[i].uimControllerIRQNum;
    hw_config_efs_ptr->uim_slot_config[i].uimUartIRQNum =
                              hw_config_ptr->uim_slot_config[i].uimUartIRQNum;

    hw_config_efs_ptr->uim_slot_config[i].uimResetGpioNum =
                              hw_config_ptr->uim_slot_config[i].uimResetGpioNum;
    hw_config_efs_ptr->uim_slot_config[i].uimResetGpioFuncSel =
                              hw_config_ptr->uim_slot_config[i].uimResetGpioFuncSel;
    hw_config_efs_ptr->uim_slot_config[i].uimResetGpioDrvStrength =
                              (uint8)hw_config_ptr->uim_slot_config[i].uimResetGpioDrvStrength;
    hw_config_efs_ptr->uim_slot_config[i].uimResetGpioPullSetting =
                              (uint8)hw_config_ptr->uim_slot_config[i].uimResetGpioPullSetting;

    hw_config_efs_ptr->uim_slot_config[i].uimDataGpioNum =
                              hw_config_ptr->uim_slot_config[i].uimDataGpioNum;
    hw_config_efs_ptr->uim_slot_config[i].uimDataGpioFuncSel =
                              hw_config_ptr->uim_slot_config[i].uimDataGpioFuncSel;
    hw_config_efs_ptr->uim_slot_config[i].uimDataGpioDrvStrength =
                              (uint8)hw_config_ptr->uim_slot_config[i].uimDataGpioDrvStrength;
    hw_config_efs_ptr->uim_slot_config[i].uimDataGpioPullSetting =
                              (uint8)hw_config_ptr->uim_slot_config[i].uimDataGpioPullSetting;

    hw_config_efs_ptr->uim_slot_config[i].uimClkGpioNum =
                              hw_config_ptr->uim_slot_config[i].uimClkGpioNum;
    hw_config_efs_ptr->uim_slot_config[i].uimClkGpioFuncSel =
                              hw_config_ptr->uim_slot_config[i].uimClkGpioFuncSel;
    hw_config_efs_ptr->uim_slot_config[i].uimClkGpioDrvStrength =
                              (uint8)hw_config_ptr->uim_slot_config[i].uimClkGpioDrvStrength;
    hw_config_efs_ptr->uim_slot_config[i].uimClkGpioPullSetting =
                              (uint8)hw_config_ptr->uim_slot_config[i].uimClkGpioPullSetting;

    hw_config_efs_ptr->uim_slot_config[i].uimCardDetectGpioNum =
                              hw_config_ptr->uim_slot_config[i].uimCardDetectGpioNum;
    hw_config_efs_ptr->uim_slot_config[i].uimCardDetectGpioFuncSel =
                              hw_config_ptr->uim_slot_config[i].uimCardDetectGpioFuncSel;
    hw_config_efs_ptr->uim_slot_config[i].uimCardDetectGpioDrvStrength =
                              (uint8)hw_config_ptr->uim_slot_config[i].uimCardDetectGpioDrvStrength;
    hw_config_efs_ptr->uim_slot_config[i].uimCardDetectGpioPullSetting =
                              (uint8)hw_config_ptr->uim_slot_config[i].uimCardDetectGpioPullSetting;

    hw_config_efs_ptr->uimSwpCapabilitySupported[i] =
                              hw_config_ptr->uimSwpCapabilitySupported[i];
  }
} /* uimHWConfig_copy_to_efs_type */


/* This function copies values in uimHWConfigEfs struct to hw_config_ptr by
   converting to appropriate enum type */
static void uimHWConfig_copy_from_efs_type(const uimHWConfigEfs *hw_config_efs_ptr,
                                           uimHWConfig    *hw_config_ptr)
{
  uint8 i = 0;

  hw_config_ptr->version                     = (uimVersionEnum)hw_config_efs_ptr->version;
  hw_config_ptr->uimBattAlarmGpioNum         = hw_config_efs_ptr->uimBattAlarmGpioNum;
  hw_config_ptr->uimBattAlarmGpioFuncSel     = hw_config_efs_ptr->uimBattAlarmGpioFuncSel;
  hw_config_ptr->uimBattAlarmGpioDrvStrength = 
                                    (uimDriveStrengthEnum)hw_config_efs_ptr->uimBattAlarmGpioDrvStrength;
  hw_config_ptr->uimBattAlarmGpioPullSetting = 
                                    (uimPullSettingEnum)hw_config_efs_ptr->uimBattAlarmGpioPullSetting;
  hw_config_ptr->uicc_shutdown_feature.uiccShutdownBattRemoval =
                                    hw_config_efs_ptr->uicc_shutdown_feature.uiccShutdownBattRemoval;
  uim_memscpy(hw_config_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval,
              sizeof(hw_config_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval),
              hw_config_efs_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval,
              sizeof(hw_config_efs_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval));
  uim_memscpy(hw_config_ptr->usb_uicc_obsolete,
              sizeof(hw_config_ptr->usb_uicc_obsolete),
              hw_config_efs_ptr->usb_uicc_obsolete,
              sizeof(hw_config_efs_ptr->usb_uicc_obsolete));
  uim_memscpy(hw_config_ptr->uimSwpCapabilitySupported,
              sizeof(hw_config_ptr->uimSwpCapabilitySupported),
              hw_config_efs_ptr->uimSwpCapabilitySupported,
              sizeof(hw_config_efs_ptr->uimSwpCapabilitySupported));
  uim_memscpy(hw_config_ptr->uim_hotswap_flaky_switch,
              sizeof(hw_config_ptr->uim_hotswap_flaky_switch),
              hw_config_efs_ptr->uim_hotswap_flaky_switch,
              sizeof(hw_config_efs_ptr->uim_hotswap_flaky_switch));
  for(i = 0; i < UIM_INSTANCES_CONFIGURABLE; i++)
  {
    hw_config_ptr->uim_slot_config[i].disableUim =
                                      hw_config_efs_ptr->uim_slot_config[i].disableUim;
    hw_config_ptr->uim_slot_config[i].enableUimHotswap =
                                      hw_config_efs_ptr->uim_slot_config[i].enableUimHotswap;
    hw_config_ptr->uim_slot_config[i].uimHotswapPolarity =
                                      (uimPolarityeEnum)hw_config_efs_ptr->uim_slot_config[i].uimHotswapPolarity;
    hw_config_ptr->uim_slot_config[i].uimControllerIRQNum =
                                      hw_config_efs_ptr->uim_slot_config[i].uimControllerIRQNum;
    hw_config_ptr->uim_slot_config[i].uimUart =
                                      (uimUartEnumList)hw_config_efs_ptr->uim_slot_config[i].uimUart;
    hw_config_ptr->uim_slot_config[i].uimUartIRQNum =
                                      hw_config_efs_ptr->uim_slot_config[i].uimUartIRQNum;
    hw_config_ptr->uim_slot_config[i].uimVcc = 
                                      (uimVccEnum)hw_config_efs_ptr->uim_slot_config[i].uimVcc;
    hw_config_ptr->uim_slot_config[i].uimResetGpioNum =
                                      hw_config_efs_ptr->uim_slot_config[i].uimResetGpioNum;
    hw_config_ptr->uim_slot_config[i].uimResetGpioFuncSel =
                                      hw_config_efs_ptr->uim_slot_config[i].uimResetGpioFuncSel;
    hw_config_ptr->uim_slot_config[i].uimResetGpioDrvStrength =
                                      (uimDriveStrengthEnum)hw_config_efs_ptr->uim_slot_config[i].uimResetGpioDrvStrength;
    hw_config_ptr->uim_slot_config[i].uimResetGpioPullSetting =
                                      (uimPullSettingEnum)hw_config_efs_ptr->uim_slot_config[i].uimResetGpioPullSetting;

    hw_config_ptr->uim_slot_config[i].uimDataGpioNum =
                                      hw_config_efs_ptr->uim_slot_config[i].uimDataGpioNum;
    hw_config_ptr->uim_slot_config[i].uimDataGpioFuncSel =
                                      hw_config_efs_ptr->uim_slot_config[i].uimDataGpioFuncSel;
    hw_config_ptr->uim_slot_config[i].uimDataGpioDrvStrength =
                                      (uimDriveStrengthEnum)hw_config_efs_ptr->uim_slot_config[i].uimDataGpioDrvStrength;
    hw_config_ptr->uim_slot_config[i].uimDataGpioPullSetting =
                                      (uimPullSettingEnum)hw_config_efs_ptr->uim_slot_config[i].uimDataGpioPullSetting;

    hw_config_ptr->uim_slot_config[i].uimClkGpioNum =
                                      hw_config_efs_ptr->uim_slot_config[i].uimClkGpioNum;
    hw_config_ptr->uim_slot_config[i].uimClkGpioFuncSel =
                                      hw_config_efs_ptr->uim_slot_config[i].uimClkGpioFuncSel;
    hw_config_ptr->uim_slot_config[i].uimClkGpioDrvStrength = 
                                      (uimDriveStrengthEnum)hw_config_efs_ptr->uim_slot_config[i].uimClkGpioDrvStrength;
    hw_config_ptr->uim_slot_config[i].uimClkGpioPullSetting = 
                                      (uimPullSettingEnum)hw_config_efs_ptr->uim_slot_config[i].uimClkGpioPullSetting;

    hw_config_ptr->uim_slot_config[i].uimCardDetectGpioNum =
                                      hw_config_efs_ptr->uim_slot_config[i].uimCardDetectGpioNum;
    hw_config_ptr->uim_slot_config[i].uimCardDetectGpioFuncSel =
                                      hw_config_efs_ptr->uim_slot_config[i].uimCardDetectGpioFuncSel;
    hw_config_ptr->uim_slot_config[i].uimCardDetectGpioDrvStrength = 
                                      (uimDriveStrengthEnum)hw_config_efs_ptr->uim_slot_config[i].uimCardDetectGpioDrvStrength;
    hw_config_ptr->uim_slot_config[i].uimCardDetectGpioPullSetting = 
                                      (uimPullSettingEnum)hw_config_efs_ptr->uim_slot_config[i].uimCardDetectGpioPullSetting;
  }
} /* uimHWConfig_copy_from_efs_type */


static void uim_apply_defaults(uimdrv_hw_nv_config_info_type *hw_nv_config_ptr, 
                               uimHWConfig                   *hw_config_ptr)
{
  DalDeviceHandle               *phChipInfo    = NULL;
  DalChipInfoFamilyType         chipFamily     = DALCHIPINFO_FAMILY_UNKNOWN;
  DalChipInfoVersionType        nChipVersion   = 0;
  DalPlatformInfoPlatformType   nPlatformType  = DALPLATFORMINFO_TYPE_UNKNOWN;
  uint8                         nChipindex     = 0;
  uint8                         i              = 0;
  uimHWConfigEfs                m_HWConfig_efs = {0};

  if(hw_config_ptr == NULL || hw_nv_config_ptr == NULL)
  {
    return;
  }

  UIM_MSG_LOW_0("HW configuration applying defaults ");

  hw_nv_config_ptr->default_nv_applied  = TRUE;

  if (DAL_DeviceAttach(DALDEVICEID_CHIPINFO, &phChipInfo) != DAL_SUCCESS)
  {
    return;
  }

  DalChipInfo_GetChipFamily(phChipInfo, &chipFamily);
  nChipVersion = DalChipInfo_ChipVersion();
  nPlatformType = DalPlatformInfo_Platform();

  UIM_MSG_HIGH_2("Chip Family 0x%x, Chip Version 0x%x", chipFamily, nChipVersion);

  /*
     All UIM instances are initially disabled.
     They are selectively enabled based on the chipset.
  */
  hw_config_ptr->uim_slot_config[0].disableUim = TRUE;
  hw_config_ptr->uim_slot_config[1].disableUim = TRUE;
  hw_config_ptr->uim_slot_config[2].disableUim = TRUE;
  hw_config_ptr->uim_slot_config[3].disableUim = TRUE;
  /* Disable/Enable UIM1-UIM4 based on the chipset */
  configure_uim_enablement(chipFamily, nChipVersion, nPlatformType, hw_config_ptr);

  hw_config_ptr->version = VER_4;

  hw_config_ptr->uimBattAlarmGpioNum            = GPIO_NUM_DEFAULT;
  hw_config_ptr->uimBattAlarmGpioFuncSel        = GPIO_FUNC_SEL_DEFAULT;
  hw_config_ptr->uimBattAlarmGpioDrvStrength    = GPIO_DRV_STRENGTH_DEFAULT;
  hw_config_ptr->uimBattAlarmGpioPullSetting    = GPIO_PULL_SETTING_DEFAULT;
  hw_config_ptr->uicc_shutdown_feature.uiccShutdownBattRemoval  = FALSE;

  nChipindex = uim_get_chip_index_from_table(chipFamily);
  for (i = 0; i < UIM_INSTANCES_CONFIGURABLE; i++)
  {
#ifdef FEATURE_ENABLE_HOTSWAP_DEFAULT
    hw_config_ptr->uim_slot_config[i].enableUimHotswap               = TRUE;
#else
    hw_config_ptr->uim_slot_config[i].enableUimHotswap               = FALSE;
#endif /* FEATURE_ENABLE_HOTSWAP_DEFAULT */
    hw_config_ptr->uim_slot_config[i].uimHotswapPolarity             = ACTIVE_HIGH;

    hw_config_ptr->uim_slot_config[i].uimUart                        = uim_default_uartdm_num[i];
    hw_config_ptr->uim_slot_config[i].uimVcc                         = uim_default_pmic_npa_resource[i];
    hw_config_ptr->uim_slot_config[i].uimControllerIRQNum            = uim_hw_param[nChipindex].controller_hs_irq_num[i];
    hw_config_ptr->uim_slot_config[i].uimUartIRQNum                  = uim_hw_param[nChipindex].uartdm_irq_num[i];

    hw_config_ptr->uim_slot_config[i].uimResetGpioNum                = GPIO_NUM_DEFAULT;
    hw_config_ptr->uim_slot_config[i].uimResetGpioFuncSel            = GPIO_FUNC_SEL_DEFAULT;
    hw_config_ptr->uim_slot_config[i].uimResetGpioDrvStrength        = GPIO_DRV_STRENGTH_DEFAULT;
    hw_config_ptr->uim_slot_config[i].uimResetGpioPullSetting        = GPIO_PULL_SETTING_DEFAULT;

    hw_config_ptr->uim_slot_config[i].uimDataGpioNum                 = GPIO_NUM_DEFAULT;
    hw_config_ptr->uim_slot_config[i].uimDataGpioFuncSel             = GPIO_FUNC_SEL_DEFAULT;
    hw_config_ptr->uim_slot_config[i].uimDataGpioDrvStrength         = GPIO_DRV_STRENGTH_DEFAULT;
    hw_config_ptr->uim_slot_config[i].uimDataGpioPullSetting         = GPIO_PULL_SETTING_DEFAULT;

    hw_config_ptr->uim_slot_config[i].uimClkGpioNum                  = GPIO_NUM_DEFAULT;
    hw_config_ptr->uim_slot_config[i].uimClkGpioFuncSel              = GPIO_FUNC_SEL_DEFAULT;
    hw_config_ptr->uim_slot_config[i].uimClkGpioDrvStrength          = GPIO_DRV_STRENGTH_DEFAULT;
    hw_config_ptr->uim_slot_config[i].uimClkGpioPullSetting          = GPIO_PULL_SETTING_DEFAULT;

    hw_config_ptr->uim_slot_config[i].uimCardDetectGpioNum           = GPIO_NUM_DEFAULT;
    hw_config_ptr->uim_slot_config[i].uimCardDetectGpioFuncSel       = GPIO_FUNC_SEL_DEFAULT;
    hw_config_ptr->uim_slot_config[i].uimCardDetectGpioDrvStrength   = GPIO_DRV_STRENGTH_DEFAULT;
    if (FALSE == uim_hw_param[hw_nv_config_ptr->dalChipFamilyIndex].bIsExternalPullUPInstalled[i])
    {
      /* External Pull Up is not installed on UIMx_PRESENT line */
      hw_config_ptr->uim_slot_config[i].uimCardDetectGpioPullSetting   = GPIO_PULL_SETTING_PRESENT;
    }
    else
    {
      /* External Pull Up is installed on UIMx_PRESENT line */
      hw_config_ptr->uim_slot_config[i].uimCardDetectGpioPullSetting   = GPIO_PULL_SETTING_DEFAULT;	  
    }

   /*
     UICC shutdown on card removal should be enabled by default,
     but disabling them on 8x26 due to call drop issues, seen because
     PMIC incorrectly shuts down the UIM LDO.
    */
    if (DALCHIPINFO_FAMILY_MSM8x26 == chipFamily)
    {
       hw_config_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval[i]  = FALSE;
    }
    else
    {
       hw_config_ptr->uicc_shutdown_feature.uiccShutdownCardRemoval[i]  = TRUE;
    }
    /* making SWP support in Terminal Capability as NOT SUPPORTED */
    hw_config_ptr->uimSwpCapabilitySupported[i] = FALSE;
  }

  /* Most of target have the SWP support on slot1 */
  hw_config_ptr->uimSwpCapabilitySupported[0] = TRUE;

  /* Copy HWConfig to EFS structure */
  uimHWConfig_copy_to_efs_type(&m_HWConfig_efs, hw_config_ptr);

  if (UIM_COMMON_EFS_SUCCESS != uim_common_efs_write(UIM_COMMON_EFS_UIMDRV_HW_CONFIG,
                                                     UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                                     UIM_COMMON_EFS_DEVICE,
                                                    (char *)&(m_HWConfig_efs),
                                                     sizeof(m_HWConfig_efs)))
  {
    UIM_MSG_ERR_0("Writing default values to EFS failed");
  }
}/* uim_apply_defaults */


/* This function sets the flaky hotswap feature globals after reading the NV70210 */
void uim_set_hotswap_flaky_switch_support(const uimHWConfig *hw_config_ptr)
{
  uint8	                    i         = 0;
  uim_instance_global_type *uim_ptr	   = NULL;

  for (i = 0; i < UIM_INSTANCES_CONFIGURABLE; i++)
  {
    uim_ptr = uim_get_instance_ptr((uim_instance_enum_type)(i));

    if(hw_config_ptr->uim_slot_config[i].disableUim == TRUE)
    {
      continue;
    }
    if(uim_ptr != NULL)
    {
      if( hw_config_ptr->uim_hotswap_flaky_switch[i] == TRUE )
      {
        uim_ptr->hotswap.hotswap_flaky_switch_support = TRUE;
      }
      else
      {
        uim_ptr->hotswap.hotswap_flaky_switch_support = FALSE;
      }
    }
  }
}/* uim_set_hotswap_flaky_switch_support */


/*
  1. uimdrv_hw_nv_configuration() is invoked to read nv
     UIM_HW_CONFIG_NV_EF that contains information used to configure
     UIMDRV HW interfaces.
  2. It is instance independant as all instances run from information
     got from nv UIM_HW_CONFIG_NV_EF
  3. The NV is read only once by either of the following depending on who
     calls uimdrv_hw_nv_configuration() first,
     a. Either the first uim thread to reach dev init and in turn invoke
        uimdrv_hw_nv_configuration().
     b. Else by any external client who calls the UIM exposed api to get
        the UIM slot information. The exposed api is uim_get_hw_slot_info()
        which in turn invokes uimdrv_hw_nv_configuration().
*/
uim_hw_slot_info_type uimdrv_hw_nv_configuration(void)
{
  uint32                                i                = 0;
  uint32                                j                = 0;
  uint32                                k                = 0;
  /* This is used to hold the list of enabled UIM i.e
     uim_enabled_slot_config[0 .. (supportedSlot-1)].disableUim = FALSE
   */
  uimSlotConfig                         uim_enabled_slot_config[UIM_INSTANCES_CONFIGURABLE];
  DalDeviceHandle                       *phChipInfo                              = NULL;
  DalChipInfoFamilyType                 chipFamily                               = DALCHIPINFO_FAMILY_UNKNOWN;
  DalPlatformInfoPlatformType           nPlatformType                            = DALPLATFORMINFO_TYPE_UNKNOWN;
  uimHWConfigEfs                        hw_config_efs                            = {0};
  uimdrv_hw_nv_config_info_type         hw_nv_config_info                        = {0};
  uim_hw_slot_info_type                 hw_slot_info;
  uimHWConfig                           hw_config;
  uim_hotswap_enum_type	                hotswap_nature[UIM_MAX_NUMBER_INSTANCES] = {0};
  uint8                                 num_active_slots                         = 0;

  /* Return the Global info when the EFS Read and Critical section are done */
  if(uimdrv_hw_nv_config_info.hw_nv_crit_sec_init && 
     uimdrv_hw_nv_config_info.efs_file_read_completed)
  {
    return uim_hw_slot_info;
  }

  hw_nv_config_info.number_of_active_interfaces = 0;
  memset(&hw_slot_info, 0, sizeof(uim_hw_slot_info_type));

  if (DAL_DeviceAttach(DALDEVICEID_CHIPINFO, &phChipInfo) != DAL_SUCCESS)
  {
    UIM_MSG_ERR_0("DAL_DeviceAttach failed in uimdrv_hw_nv_configuration");
    return hw_slot_info;
  }

  DalChipInfo_GetChipFamily(phChipInfo, &chipFamily);

  nPlatformType = DalPlatformInfo_Platform();

  hw_nv_config_info.dalChipFamily = chipFamily;
  hw_nv_config_info.hw_max_num_slots_supported =
                                uim_get_hw_max_slots_available(chipFamily,nPlatformType);
  hw_nv_config_info.dalChipFamilyIndex =
                          uim_get_chip_index_from_table(chipFamily);

  memset(&hw_config, 0, sizeof(hw_config));
  memset(&uim_enabled_slot_config, 0, sizeof(uim_enabled_slot_config));

  if (UIM_COMMON_EFS_SUCCESS == uim_common_efs_read( UIM_COMMON_EFS_UIMDRV_HW_CONFIG,
                                                     UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                                     UIM_COMMON_EFS_DEVICE,
                                                     (uint8 *)&(hw_config_efs),
                                                     sizeof(hw_config_efs)))
  {
    /* copy HWConfig from EFS structure */
    uimHWConfig_copy_from_efs_type(&hw_config_efs, &hw_config);

    uim_hw_config_input_validation(&hw_nv_config_info, &hw_config);

    if (DALPLATFORMINFO_TYPE_LIQUID == nPlatformType ||
        DALPLATFORMINFO_TYPE_FLUID == nPlatformType)
    {
      hw_config.uim_slot_config[1].disableUim = TRUE;
    }
  }
  else
  {
    uim_apply_defaults(&hw_nv_config_info, &hw_config);
  }

  num_active_slots = uim_get_num_active_slot((uint8)hw_nv_config_info.hw_max_num_slots_supported);   
  
  for (i = 0; (i < hw_nv_config_info.hw_max_num_slots_supported) && (i < UIM_INSTANCES_CONFIGURABLE); i++)
  {
    /* Check for number of slot active in NV */	
    if (hw_config.uim_slot_config[i].disableUim == FALSE)
    {
      hw_slot_info.slot_status[i].slot_available = TRUE;
      hw_nv_config_info.number_of_active_interfaces++;
      
      if( k < num_active_slots )
      {
        hw_slot_info.slot_status[i].slot_enabled = TRUE;
        k++;
      }
    }
  }

  if (hw_nv_config_info.number_of_active_interfaces == 0)
  {
    UIM_MSG_ERR_0("Number of active interfaces is Zero, UIM cannot initialize HW");
    return hw_slot_info;
  }

  /* scanning through hw_config and picking up the uim_slot_config object corresponding to
     enabled slot, copying it into newly introduced uim_enabled_slot_config object.
     This (uim_enabled_slot_config) newly introduced object will hold the slot configuration
     data of enabled UIM only (UIM_DISABLE = FALSE)
  */
  for (i = 0; (i < hw_nv_config_info.hw_max_num_slots_supported) && (i < UIM_INSTANCES_CONFIGURABLE); i++)
  {
    if (hw_slot_info.slot_status[i].slot_available == TRUE)
    {
      uim_memscpy(&uim_enabled_slot_config[j],
                  sizeof(uimSlotConfig),
                &hw_config.uim_slot_config[i],
                sizeof(hw_config.uim_slot_config[i]));
      hw_config.uimSwpCapabilitySupported[j] = hw_config.uimSwpCapabilitySupported[i];
      hw_config.uicc_shutdown_feature.uiccShutdownCardRemoval[j] = hw_config.uicc_shutdown_feature.uiccShutdownCardRemoval[i];
      hw_config.uim_hotswap_flaky_switch[j] = hw_config.uim_hotswap_flaky_switch[i];
      hotswap_nature[j] = hotswap_nature[i];
      j++;
    }
  }
  
  /* while doing memset with ZERO,which indirectly made
     uim_enabled_slot_config[j].disableUim = ZERO (FALSE), causing this UIM to be enabled.
     therefore It is purposely made to DISABLE remaining UIM
     before overwriting hw_config with uim_enabled_slot_config
  */
  for (;j < UIM_INSTANCES_CONFIGURABLE;j++)
  {
    uim_enabled_slot_config[j].disableUim = TRUE;
  }

  uim_memscpy(&hw_config.uim_slot_config,
              sizeof(hw_config.uim_slot_config),
              &uim_enabled_slot_config,
              sizeof(uim_enabled_slot_config));

  uim_set_hotswap_flaky_switch_support(&hw_config);

  if(!uimdrv_hw_nv_config_info.hw_nv_crit_sec_init)
  {
    return hw_slot_info;
  }

  /* Updated the above deriverd information into Globals */ 
 
  /* Enter critical section */
  rex_enter_crit_sect(&uimdrv_hw_nv_config_info.uimdrv_hw_nv_crit_sect);

  /* EFS not read, read EFS and return the number_of_active_interfaces */
  if (FALSE == uimdrv_hw_nv_config_info.efs_file_read_completed)
  { 
    uimdrv_hw_nv_config_info.dalChipFamily = hw_nv_config_info.dalChipFamily;
    uimdrv_hw_nv_config_info.dalChipFamilyIndex = hw_nv_config_info.dalChipFamilyIndex;
    uimdrv_hw_nv_config_info.hw_max_num_slots_supported = hw_nv_config_info.hw_max_num_slots_supported;
    uimdrv_hw_nv_config_info.number_of_active_interfaces = hw_nv_config_info.number_of_active_interfaces;
    uimdrv_hw_nv_config_info.default_nv_applied = hw_nv_config_info.default_nv_applied;
  
    /* Fill the m_HWConfig with hw_config*/
    uim_fill_global_hw_config(&hw_config);
  
    for (i = 0, j = 0; (i < uimdrv_hw_nv_config_info.hw_max_num_slots_supported) 
         && (i < UIM_INSTANCES_CONFIGURABLE); i++)
    {
      uim_hw_slot_info.slot_status[i].slot_available = hw_slot_info.slot_status[i].slot_available;
      uim_hw_slot_info.slot_status[i].slot_enabled = hw_slot_info.slot_status[i].slot_enabled; 
  
      if (uim_hw_slot_info.slot_status[i].slot_enabled == TRUE)
      {
        m_HWConfig.uimSwpCapabilitySupported[j] = hw_config.uimSwpCapabilitySupported[i];
        m_HWConfig.uicc_shutdown_feature.uiccShutdownCardRemoval[j] = hw_config.uicc_shutdown_feature.uiccShutdownCardRemoval[i];
        m_HWConfig.uim_hotswap_flaky_switch[j] = hw_config.uim_hotswap_flaky_switch[i];
        uim_hotswap_nature[j] = hotswap_nature[i];
        j++;
      }
    }
  
    for (j = 0; j < UIM_INSTANCES_CONFIGURABLE; j++)
    {
      m_HWConfig.uim_hotswap_flaky_switch[j] = hw_config.uim_hotswap_flaky_switch[j];
      m_HWConfig.uim_slot_config[j].enableUimHotswap = hw_config.uim_slot_config[j].enableUimHotswap;
    }
  
    uimdrv_hw_nv_config_info.efs_file_read_completed = TRUE;
  }

  /* Leave critical section */
  rex_leave_crit_sect(&uimdrv_hw_nv_config_info.uimdrv_hw_nv_crit_sect);

  return uim_hw_slot_info;
} /* uimdrv_hw_nv_configuration */


/*
   uim_get_hw_slot_info() is the API is exposed for other uim modules
   to get information on the available/enabled UIM interfaces.
*/
uim_hw_slot_info_type uim_get_hw_slot_info(void)
{
  return uimdrv_hw_nv_configuration();
}/* uim_get_hw_slot_info */


/* Disables/Enables UIM1-UIM4 based on chipset defaults  */
void configure_uim_enablement(DalChipInfoFamilyType  chipFamily,
                              DalChipInfoVersionType nChipVersion,
                              DalPlatformInfoPlatformType nPlatformType,
                              uimHWConfig       *hw_config_ptr)
{
  uint32 i = 0;

  /* Find the corresponding entry in the chipset_info_table */
  /* i will be the index of the correct entry after this loop */
  for (i=0; uim_hw_param[i].chipFamily != DALCHIPINFO_FAMILY_UNKNOWN; i++)
  {
    if (uim_hw_param[i].chipFamily == chipFamily)
    {
      break;
    }
  }

  if (DALCHIPINFO_FAMILY_UNKNOWN == uim_hw_param[i].chipFamily)
  {
    UIM_MSG_ERR_0("INVALID chipFamily");
    return;
  }

  /* Enable/Disable the UIM interfaces based on the chipset_info_table entries */
  hw_config_ptr->uim_slot_config[0].disableUim = uim_hw_param[i].bDisableUIM1;
  hw_config_ptr->uim_slot_config[1].disableUim = uim_hw_param[i].bDisableUIM2;
  hw_config_ptr->uim_slot_config[2].disableUim = uim_hw_param[i].bDisableUIM3;
  hw_config_ptr->uim_slot_config[3].disableUim = uim_hw_param[i].bDisableUIM4;

  /* SPECIAL CASES */
  if (DALCHIPINFO_FAMILY_MDM9x25 == chipFamily)
  {
    if(nChipVersion < DALCHIPINFO_VERSION(2,0))
    {
      /* UIM2 is enabled by default instead of UIM1 */
      hw_config_ptr->uim_slot_config[0].disableUim = TRUE;
      hw_config_ptr->uim_slot_config[1].disableUim = FALSE;
    }
  }

  /* For Liquid and Fluid platforms, disable UIM2 */
  if (DALPLATFORMINFO_TYPE_LIQUID == nPlatformType ||
      DALPLATFORMINFO_TYPE_FLUID == nPlatformType)
  {
    hw_config_ptr->uim_slot_config[1].disableUim = TRUE;
  }
}/* configure_uim_enablement */


/* uim_get_hw_max_slots_available returns the max available uim slots on that chipset */
uint8 uim_get_hw_max_slots_available(DalChipInfoFamilyType       chipFamily,
                                     DalPlatformInfoPlatformType nPlatformType)
{
  uint8 i;
  /* If the chip id is 8626 or 8926 and the platform type is RUMI then disable all UIM slots */
  if ((DALCHIPINFO_FAMILY_MSM8x26  == chipFamily || DALCHIPINFO_FAMILY_MSM8926 == chipFamily)
      && DALPLATFORMINFO_TYPE_RUMI == nPlatformType)
  {
    return 0;
  }
  for (i=0; uim_hw_param[i].chipFamily != DALCHIPINFO_FAMILY_UNKNOWN; i++)
  {
    if (uim_hw_param[i].chipFamily == chipFamily)
    {
      return uim_hw_param[i].numUimSlotsAvailable;
    }
  }
  UIM_MSG_ERR_0("uim_get_hw_max_slots_available: Chipset unknown, cannot initialize HW");
  return 0;
}/* uim_get_hw_max_slots_available */


/* uim_get_chip_index_from_table returns the index of the information related to the chip family */
uint8 uim_get_chip_index_from_table (DalChipInfoFamilyType  chipFamily)
{
  uint8 i = 0;

  for (i=0; uim_hw_param[i].chipFamily != DALCHIPINFO_FAMILY_UNKNOWN; i++)
  {
    if (uim_hw_param[i].chipFamily == chipFamily)
    {
      return i;
    }
  }

  ERR_FATAL("uim_get_chip_index_from_table: Chipset unknown, cannot initialize HW",0,0,0);
  return 0;
}/* uim_get_chip_index_from_table */


/*
   uim_hw_config_input_validation checks if crucial items for enabled UIM
   instance is set to 0.
 */
void uim_hw_config_input_validation(uimdrv_hw_nv_config_info_type *hw_nv_config_info,
                                    uimHWConfig *hw_config)
{
  uint32 i = 0;

  if (hw_config == NULL || hw_nv_config_info == NULL)
  {
    return;
  }

  for (i = 0; i < UIM_INSTANCES_CONFIGURABLE; i++)
  {
    /* For each enabled UIM instance, check crucial items if they are 0 */
    if (FALSE == hw_config->uim_slot_config[i].disableUim &&
        (hw_config->uim_slot_config[i].uimControllerIRQNum == 0 ||
         hw_config->uim_slot_config[i].uimUartIRQNum == 0))
        {
          UIM_MSG_LOW_0("UIM_HW_CONFIG_NV_EF nearly zero, populating defaults");
      uim_apply_defaults(hw_nv_config_info, hw_config);
          break;
        }

  }
} /* uim_hw_config_input_validation */
