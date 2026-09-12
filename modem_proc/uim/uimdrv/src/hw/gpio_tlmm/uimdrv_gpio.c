/*============================================================================
  FILE:         uimdrv_gpio.c

  OVERVIEW:     Contains functions used by uim driver to control and initialize
                gpio/tlmm interface

  DEPENDENCIES: N/A

                Copyright (c) 2014 - 2018,2020 - 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.  Please
use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hw/gpio_tlmm/uimdrv_gpio.c#4 $
$DateTime: 2021/02/17 21:52:12 $
$Author: pwbldsvc $

 when       who        what, where, why
------      ----       -----------------------------------------------------------
02/11/21    ykr        UIM GPIO name change for Atherton target
02/20/20    rkg        Update only active slots in uim_get_hw_slot_info 
03/14/18    ak         Execute GPIO init only when BUA check returns TRUE 
05/31/16    gm         F3 message removal
01/18/16    ks         Do not error fatal even when UIMx_PRESENT isn't 
                       part of GPIO XML comfig file
06/11/15    na         Do not own battery alarm gpio when uicc shutdown upon
                       battery removal is disabled
08/04/15    sam        FR24498: Powerup logging new changes
03/19/15    na         UIM3 code change for external LDO
03/12/15    ks         SIM tray by tieing the card detect gpios
03/04/15    na         Removal of compiler warnings
01/26/15    ll         Only get GPIOID for batt alarm once
01/22/15    na         Rumi crash issue
01/16/15    na         Suppport of RUMI
01/07/15    ll         Add checks for TLMM GPIO ID return values
01/07/14    sam        Modification of msg macros for FR24498-UIM Powerup Logging
01/05/14    ll         Compile warnings
12/30/14    ks         supoport for UIM3 external LDO on 8909
12/04/14    sam        Fix for KW errors
12/02/14    ks         Feature to support GPIO based hotswap
12/02/14    sam        Incorporate the newly added TLMM API to read back GPIO configs
10/31/14    nmb        Correct slot typecasting
09/12/14    ssr        Fix to initialize the Hotswap GPIO when Hotswap feature
                       is enabled
08/25/14    ll         Fix compile warnings medium
06/16/14    ll         Switch to new string based TLMM APIs
05/12/14    ak         8994 Bring up changes
05/06/14    ks         8936 bring up changes
04/21/14    rm/akv     Fix for glitch seen on the IO line while powering down
01/29/14    sam        Usage of newer style diag macros
12/27/13    ks         8916 bring up changes
11/27/13    akv        Initialization of hw enumeration based on dal chip family
10/22/13    ll         Check UIM GPIOs as inputs upon timeout w/recovery disabled
10/08/13    nmb        HW header file cleanup
10/08/13    nmb        Changed default card detect GPIO to pull up
09/11/13    na         8x62 bring up changes
08/06/13    rm/ak      Bolt RUMI code
07/12/13    nmb        Global Reorganization
07/12/13    akv        HW Enumeration - modifying target specific hardware values
                       and organizing them in value-key pairs
05/23/13    ak         Apply NV values for PULL and DRIVE STRENGTH for UIM1 dedicated
                       UIM controller pads
05/23/13    ak         Reset CLK, DATA, and RESET PULL to NO-PULL when setting to
                       UIM control
05/22/13    akv        Warm reset fixes for Triton
02/21/13    ak         Use UIM instance number for GPIO configuration logic
02/21/13    ak         UIM1 support on 9x25 v2
02/12/13    js         General F3 macro cleanup
12/05/12    js         UIM Parallel Processing changes
11/21/12    rm         Fix to set the GPIO state to LOW after configuring it them
                       to TLMM ownership
11/09/12    akv        BATT ALARM configuration for Dime
10/26/12    akv/ssr    HW Enumeration changes
============================================================================*/
#include "uimdrv_gpio.h"
#include "uimglobals.h"
#include "uimdrv_rumi.h"

/* NOTE: X will be updated based on uim_instance in runtime in updateGpioNameWithInstance() */
#define UIM_GPIO_DATA_NAME                       "uimX_data"
#define UIM_GPIO_CLK_NAME                        "uimX_clk"
#define UIM_GPIO_RESET_NAME                      "uimX_reset"
#define UIM_GPIO_PRESENT_NAME                    "uimX_present"
#define UIM_GPIO_BATT_ALARM_NAME                 "uim_batt_alarm"
#define UIM_GPIO_EXT_LDO_EN                      "uimX_ldo_en"
#define UIM_GPIO_EXT_LDO_VSEL                    "uimX_ldo_vsel"



DALGpioIdType disabled_uim_card_presence_gpio_id;

void uim_daltlmm_getgpioId(DalDeviceHandle * m_TlmmHandle_ptr, const char* GpioName, DALGpioIdType* GpioId)
{
  DALResult result = DAL_ERROR;
  result = DalTlmm_GetGpioId( m_TlmmHandle_ptr, GpioName, GpioId );
  if (DAL_SUCCESS != result)
  {
    ERR_FATAL("Failed to get DALTLMM GPIOID",0,0,0);
  }
}/*uim_daltlmm_getgpioId*/


/*
  @brief Add instant number to GPIO name string based on physical slot number
         This physical slot number should retreive from uimUart
*/
void updateGpioNameWithInstance
(
  uim_gpios_name_type *pGpiosName,
  uim_slot_type uim_slot
)
{
  DalDeviceHandle            *phChipInfo    = NULL;
  DalChipInfoFamilyType      chipFamily     = DALCHIPINFO_FAMILY_UNKNOWN;
	
  if (UIM_SLOT_NONE == uim_slot)
  {
    return;
  }

  if(pGpiosName == NULL)
  {
    return;
  }

  if (DAL_DeviceAttach(DALDEVICEID_CHIPINFO, &phChipInfo) != DAL_SUCCESS)
  {
    return;
  }

  DalChipInfo_GetChipFamily(phChipInfo, &chipFamily);
  memset(pGpiosName, 0, sizeof(uim_gpios_name_type));
  strlcpy(pGpiosName->DataGpioName, UIM_GPIO_DATA_NAME, UIM_MAX_GPIO_NAME_LEN);
  strlcpy(pGpiosName->ClkGpioName, UIM_GPIO_CLK_NAME, UIM_MAX_GPIO_NAME_LEN);
  strlcpy(pGpiosName->ResetGpioName, UIM_GPIO_RESET_NAME, UIM_MAX_GPIO_NAME_LEN);
  strlcpy(pGpiosName->PresentGpioName, UIM_GPIO_PRESENT_NAME, UIM_MAX_GPIO_NAME_LEN);

#ifdef FEATURE_UIM_ATHERTON
  if(chipFamily == CHIPINFO_FAMILY_ATHERTON)
  {
    pGpiosName->DataGpioName[UIM_TLMM_GPIO_INST_INDEX]  = (char)(((int)'0')+ uim_slot-1);
    pGpiosName->ClkGpioName[UIM_TLMM_GPIO_INST_INDEX]   = (char)(((int)'0')+ uim_slot-1);
    pGpiosName->ResetGpioName[UIM_TLMM_GPIO_INST_INDEX] = (char)(((int)'0')+ uim_slot-1);
   }
  else
#endif /* FEATURE_UIM_ATHERTON */
  {
    pGpiosName->DataGpioName[UIM_TLMM_GPIO_INST_INDEX]  = (char)(((int)'0')+ uim_slot);
    pGpiosName->ClkGpioName[UIM_TLMM_GPIO_INST_INDEX]   = (char)(((int)'0')+ uim_slot);
    pGpiosName->ResetGpioName[UIM_TLMM_GPIO_INST_INDEX] = (char)(((int)'0')+ uim_slot);
  }	
  /* Mapping instance to string */
  if(uim_hotswap_nature[uim_slot - 1] != UIM_CONTROLLER_BASED)
  {
    pGpiosName->PresentGpioName[UIM_TLMM_GPIO_INST_INDEX] =
                    (char)(((int)'0')+ uim_hotswap_nature[uim_slot - 1]);
  }
  else
  {
#ifdef FEATURE_UIM_ATHERTON
    if(chipFamily == CHIPINFO_FAMILY_ATHERTON)
    {
      pGpiosName->PresentGpioName[UIM_TLMM_GPIO_INST_INDEX] = (char)(((int)'0')+ uim_slot-1);  
    }
    else
#endif /* FEATURE_UIM_ATHERTON */
    {
      pGpiosName->PresentGpioName[UIM_TLMM_GPIO_INST_INDEX] = (char)(((int)'0')+ uim_slot);  
    }
  }
  strlcpy(pGpiosName->BattAlarmGpioName, UIM_GPIO_BATT_ALARM_NAME, UIM_MAX_GPIO_NAME_LEN);

  strlcpy(pGpiosName->UimExtLdoEnGpioName, UIM_GPIO_EXT_LDO_EN, UIM_MAX_GPIO_NAME_LEN);
  pGpiosName->UimExtLdoEnGpioName[UIM_TLMM_GPIO_INST_INDEX] = (char)(((int)'0')+ uim_slot);
  strlcpy(pGpiosName->UimExtLdoVselGpioName, UIM_GPIO_EXT_LDO_VSEL, UIM_MAX_GPIO_NAME_LEN);
  pGpiosName->UimExtLdoVselGpioName[UIM_TLMM_GPIO_INST_INDEX] = (char)(((int)'0')+ uim_slot);
}/* update_gpio_name_with_instance */

/*
  @brief populate gpio name for present line based on uim_instance
*/
static void getPresentGpioNameWithInstance
(
  char                    *pPresentGpioName,
  uim_instance_enum_type   uim_instance
)
{
  DalDeviceHandle            *phChipInfo    = NULL;
  DalChipInfoFamilyType      chipFamily     = DALCHIPINFO_FAMILY_UNKNOWN;
  
  if(pPresentGpioName == NULL)
  {
    return;
  }

  if(uim_instance >= UIM_INSTANCE_INVALID ||
     uim_instance <  UIM_INSTANCE_1)
  {
    return;
  }
  
  if(DAL_DeviceAttach(DALDEVICEID_CHIPINFO, &phChipInfo) != DAL_SUCCESS)
  {
    return;
  }

  DalChipInfo_GetChipFamily(phChipInfo, &chipFamily);
  memset(pPresentGpioName, 0, UIM_MAX_GPIO_NAME_LEN);
  strlcpy(pPresentGpioName, UIM_GPIO_PRESENT_NAME, UIM_MAX_GPIO_NAME_LEN);
  
  /* Mapping instance to string */
  if(uim_hotswap_nature[uim_instance] != UIM_CONTROLLER_BASED)
  {
    pPresentGpioName[UIM_TLMM_GPIO_INST_INDEX] =
                   (char)(((int)'0')+ uim_hotswap_nature[uim_instance]);
  }
  else
  {
#ifdef FEATURE_UIM_ATHERTON
    if(chipFamily == CHIPINFO_FAMILY_ATHERTON)
    {
      pPresentGpioName[UIM_TLMM_GPIO_INST_INDEX] = (char)(((int)'0')+ uim_instance);  
    }
    else
#endif /* FEATURE_UIM_ATHERTON */
    {
      pPresentGpioName[UIM_TLMM_GPIO_INST_INDEX] = (char)(((int)'0')+ uim_instance+1);  
    }
  }
}/* getPresentGpioNameWithInstance */

/*
  @brief Initialize GPIO: populate device data
*/
void initialize_gpio(uim_instance_enum_type uim_instance)
{
  uint8                             i                                       = 0;
  DALResult                         eRes                                    = DAL_SUCCESS;
  uim_gpios_name_type               GpiosName;                              
  uim_slot_type                     physical_slot;                          
  uim_shared_global_type           *shared_globals_ptr                      = uim_get_common_globals_ptr();
  uim_instance_global_type         *uim_ptr;
  char                              disabledPresentGpioName[UIM_MAX_GPIO_NAME_LEN];
  
  uim_ptr                                       = uim_get_instance_ptr(uim_instance); 
  uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr = NULL;

  if(uim_ptr == NULL)
  {
    return;
  }
  /* Attach TLMM and get TLMM handle for this uim instance */
  eRes = DAL_DeviceAttach(DALDEVICEID_TLMM, &(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr));
  if((NULL == uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr)||(eRes != DAL_SUCCESS))
  {
    UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_225,uim_instance,"Failed to register with TLMM DAL");
    return;
  }

  /* Mapping instance to string */
  physical_slot = (uim_slot_type)m_HWConfig.uim_slot_config[uim_instance].uimUart + 1;

  updateGpioNameWithInstance(&GpiosName, physical_slot);
#ifndef T_RUMI_EMULATION
  /* Get GpioId via string */
  uim_daltlmm_getgpioId( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                     GpiosName.DataGpioName,
                     &(uim_hw_if.gpio[uim_instance].m_DataGpioId));
  uim_daltlmm_getgpioId( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                     GpiosName.ClkGpioName,
                     &(uim_hw_if.gpio[uim_instance].m_ClkGpioId));
  uim_daltlmm_getgpioId( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                     GpiosName.ResetGpioName,
                     &(uim_hw_if.gpio[uim_instance].m_ResetGpioId));
  DalTlmm_GetGpioId( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                     GpiosName.PresentGpioName,
                     &(uim_hw_if.gpio[uim_instance].m_PresentGpioId));

/* Make sure that the same GPIO ID is stored for all the instances that uses shared GPIO */
  if ((uim_hotswap_nature[uim_instance] != UIM_CONTROLLER_BASED)  && (uim_hw_if.gpio[uim_instance].m_PresentGpioId == 0))
  {
    for(i=0;i<UIM_MAX_INSTANCES;i++)
    {
      if((uim_hotswap_nature[uim_instance] == uim_hotswap_nature[i]) && (uim_hw_if.gpio[i].m_PresentGpioId != 0) )
      {
        uim_hw_if.gpio[uim_instance].m_PresentGpioId = uim_hw_if.gpio[i].m_PresentGpioId;
        break;
      }
    }
  }
  if (
      (TRUE  == m_HWConfig.uicc_shutdown_feature.uiccShutdownBattRemoval) ||
      (TRUE  == m_HWConfig.uicc_shutdown_feature.uiccShutdownCardRemoval[uim_instance])
     )
  {
     /* Check for BUA init. This is because for SDM845 and above, we do not
        use BUA pin.*/
     if (TRUE == uim_check_bua_mpm_irq_reg_applicable()) 
     {
       /* Get Battery Alarm GPIOID: Get only once and this task will be the owner of the Batt GPIO */
       /* Enter critical section */
       rex_enter_crit_sect(&shared_globals_ptr->gpio_get_batt_gpioid_crit_section);
       if ( FALSE == shared_globals_ptr->gpio_get_batt_gpioid_completed )
       {
         uim_daltlmm_getgpioId( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                                GpiosName.BattAlarmGpioName,
                                &(uim_hw_if.gpio[uim_instance].m_BattAlarmGpioId));
         shared_globals_ptr->gpio_get_batt_gpioid_completed = TRUE;
       }
       rex_leave_crit_sect(&shared_globals_ptr->gpio_get_batt_gpioid_crit_section);
     }
  }
#else
  (void)DalDevice_Open(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,DAL_OPEN_SHARED);
  /* This is done to cater the compiler warnings of un-used variable which is
     being treated as an error */
  (void)i;
  (void)shared_globals_ptr;
  
#endif


  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_SIMTRAY_WITH_GPIOS_TIED,
                               uim_ptr) == TRUE)
  {
    if(uim_hw_slot_info.slot_status[UIM_INSTANCE_1].slot_available == FALSE)
    {
      getPresentGpioNameWithInstance(disabledPresentGpioName, UIM_INSTANCE_1);
      uim_daltlmm_getgpioId( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             disabledPresentGpioName,
                             &(disabled_uim_card_presence_gpio_id));
    }
    else if(uim_hw_slot_info.slot_status[UIM_INSTANCE_2].slot_available == FALSE)
    {
      getPresentGpioNameWithInstance(disabledPresentGpioName, UIM_INSTANCE_2);
      uim_daltlmm_getgpioId( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             disabledPresentGpioName,
                             &(disabled_uim_card_presence_gpio_id));
    }       
  }
  /*Populate device specific configuration data to gpio struct*/
  uim_hw_if.gpio[uim_instance].m_DataGpioConfigId.eDirection          = DAL_GPIO_OUTPUT;
  uim_hw_if.gpio[uim_instance].m_DataGpioConfigId.ePull               = (DALGpioPullType)m_HWConfig.uim_slot_config[uim_instance].uimDataGpioPullSetting;
  uim_hw_if.gpio[uim_instance].m_DataGpioConfigId.eDriveStrength      = (DALGpioDriveType)m_HWConfig.uim_slot_config[uim_instance].uimDataGpioDrvStrength;
  uim_hw_if.gpio[uim_instance].m_ClkGpioConfigId.eDirection           = DAL_GPIO_OUTPUT;
  uim_hw_if.gpio[uim_instance].m_ClkGpioConfigId.ePull                = (DALGpioPullType)m_HWConfig.uim_slot_config[uim_instance].uimClkGpioPullSetting;
  uim_hw_if.gpio[uim_instance].m_ClkGpioConfigId.eDriveStrength       = (DALGpioDriveType)m_HWConfig.uim_slot_config[uim_instance].uimClkGpioDrvStrength;
  uim_hw_if.gpio[uim_instance].m_ResetGpioConfigId.eDirection         = DAL_GPIO_OUTPUT;
  uim_hw_if.gpio[uim_instance].m_ResetGpioConfigId.ePull              = (DALGpioPullType)m_HWConfig.uim_slot_config[uim_instance].uimResetGpioPullSetting;
  uim_hw_if.gpio[uim_instance].m_ResetGpioConfigId.eDriveStrength     = (DALGpioDriveType)m_HWConfig.uim_slot_config[uim_instance].uimResetGpioDrvStrength;
  uim_hw_if.gpio[uim_instance].m_PresentGpioConfigId.eDirection       = DAL_GPIO_INPUT;
  uim_hw_if.gpio[uim_instance].m_PresentGpioConfigId.ePull            = (DALGpioPullType)m_HWConfig.uim_slot_config[uim_instance].uimCardDetectGpioPullSetting;
  uim_hw_if.gpio[uim_instance].m_PresentGpioConfigId.eDriveStrength   = (DALGpioDriveType)m_HWConfig.uim_slot_config[uim_instance].uimCardDetectGpioDrvStrength;
  uim_hw_if.gpio[uim_instance].m_BattAlarmGpioConfigId.eDirection     = DAL_GPIO_OUTPUT;
  uim_hw_if.gpio[uim_instance].m_BattAlarmGpioConfigId.ePull          = (DALGpioPullType)m_HWConfig.uimBattAlarmGpioPullSetting;
  uim_hw_if.gpio[uim_instance].m_BattAlarmGpioConfigId.eDriveStrength = (DALGpioDriveType)m_HWConfig.uimBattAlarmGpioDrvStrength;

  if(uim_hw_if.uim_is_external_ldo[uim_instance])
  {
    uim_daltlmm_getgpioId( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                     GpiosName.UimExtLdoEnGpioName,
                     &(uim_hw_if.gpio[uim_instance].m_UimExtLdoEnGpioId));

    uim_daltlmm_getgpioId( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                     GpiosName.UimExtLdoVselGpioName,
                     &(uim_hw_if.gpio[uim_instance].m_UimExtLdoVselGpioId));

    uim_hw_if.gpio[uim_instance].m_UimExtLdoEnGpioConfigId.eDirection       =  DAL_GPIO_OUTPUT;
    uim_hw_if.gpio[uim_instance].m_UimExtLdoEnGpioConfigId.ePull            = (DALGpioPullType)NO_PULL;
    uim_hw_if.gpio[uim_instance].m_UimExtLdoEnGpioConfigId.eDriveStrength   = (DALGpioDriveType)DS_2MA;

    uim_hw_if.gpio[uim_instance].m_UimExtLdoVselGpioConfigId.eDirection     =  DAL_GPIO_OUTPUT;
    uim_hw_if.gpio[uim_instance].m_UimExtLdoVselGpioConfigId.ePull          = (DALGpioPullType)NO_PULL;
    uim_hw_if.gpio[uim_instance].m_UimExtLdoVselGpioConfigId.eDriveStrength = (DALGpioDriveType)DS_2MA;
  }
} /* initialize_gpio */


/*
  @ brief Configures the DATA and CLOCK lines
*/
boolean enableUartInterface(uim_instance_enum_type uim_instance)
{
#ifndef T_RUMI_EMULATION
  if( NULL == uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr ||
      0 == uim_hw_if.gpio[uim_instance].m_DataGpioId     ||
      0 == uim_hw_if.gpio[uim_instance].m_ClkGpioId )
  {
    return FALSE;
  }

  DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_DataGpioId,
                           UIM_TLMM_IO_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_DataGpioConfigId));
  DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_ClkGpioId,
                           UIM_TLMM_IO_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_ClkGpioConfigId));
#else
  if (UIM_INSTANCE_1 == uim_instance)
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM_DATA_DEFAULT_GPIO,
                              DAL_TLMM_GPIO_ENABLE);
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM_CLK_DEFAULT_GPIO,
                              DAL_TLMM_GPIO_ENABLE);
  }
  else
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                            (DALGpioSignalType)UIM2_DATA_DEFAULT_GPIO,
                             DAL_TLMM_GPIO_ENABLE);
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                            (DALGpioSignalType)UIM2_CLK_DEFAULT_GPIO,
                             DAL_TLMM_GPIO_ENABLE);
  }
#endif
  return TRUE;
} /* enableUartInterface */


/*
  @brief  Configures RESET GPIO.
*/
boolean setResetAsOutput(uim_instance_enum_type uim_instance)
{
#ifndef T_RUMI_EMULATION
  if( NULL == uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr ||
      0 == uim_hw_if.gpio[uim_instance].m_ResetGpioId )
  {
    return FALSE;
  }
  DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_ResetGpioId,
                           UIM_TLMM_IO_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_ResetGpioConfigId));
#else
  if (UIM_INSTANCE_1 == uim_instance)
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM_RESET_DEFAULT_GPIO,
                             DAL_TLMM_GPIO_ENABLE );
  }
  else
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM2_RESET_DEFAULT_GPIO,
                             DAL_TLMM_GPIO_ENABLE );
  }
#endif
  return TRUE;
} /* setResetAsOutput */


/*
  @brief  Brings the RESET line LOW.
*/
boolean assertReset(uim_instance_enum_type uim_instance)
{
#ifndef T_RUMI_EMULATION
  if( NULL == uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr ||
      0 == uim_hw_if.gpio[uim_instance].m_ResetGpioId )
  {
    return FALSE;
  }

  DalTlmm_GpioIdOut(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                    uim_hw_if.gpio[uim_instance].m_ResetGpioId,
                    DAL_GPIO_LOW_VALUE);
#else
  if(UIM_INSTANCE_1 == uim_instance)
  {
    (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                         (DALGpioSignalType)UIM_RESET_DEFAULT_GPIO,
                          DAL_GPIO_LOW_VALUE);
  }
  else
  {
    (void)DalTlmm_GpioOut(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                         (DALGpioSignalType)UIM2_RESET_DEFAULT_GPIO,
                          DAL_GPIO_LOW_VALUE);
  }
#endif
  return TRUE;
}/* assertReset */


/*
  @brief Brings the RESET line HIGH.
*/
boolean deassertReset(uim_instance_enum_type uim_instance)
{
#ifndef T_RUMI_EMULATION
  if( NULL == uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr ||
      0 == uim_hw_if.gpio[uim_instance].m_ResetGpioId )
  {
    return FALSE;
  }
  /* Switch the user of the RESET line to UIM controller */
  DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_ResetGpioId,
                           UIM_TLMM_UIMCTRL_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_ResetGpioConfigId));
#else
  if(UIM_INSTANCE_1 == uim_instance)
  {
   (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           (DALGpioSignalType)UIM_RESET_UIMCNTRL_GPIO,
                            DAL_TLMM_GPIO_ENABLE);
  }
  else
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM2_RESET_UIMCNTRL_GPIO,
                              DAL_TLMM_GPIO_ENABLE);
  }
#endif
  return TRUE;
}/* deassertReset */

/* Configure Reset for GPIO/TLMM */
boolean uimConfigureResetDefaultGpio(uim_instance_enum_type uim_instance)
{
#ifndef T_RUMI_EMULATION
  if( NULL == uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr ||
      0  == uim_hw_if.gpio[uim_instance].m_ResetGpioId )
  {
    return FALSE;
  }

  DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_ResetGpioId,
                           UIM_TLMM_IO_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_ResetGpioConfigId));
  (void)DalTlmm_GpioIdOut(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                          uim_hw_if.gpio[uim_instance].m_ResetGpioId,
                          DAL_GPIO_LOW_VALUE);
#else
  if(UIM_INSTANCE_1 == uim_instance)
  {
    (void)DalTlmm_ConfigGpio( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr, (DALGpioSignalType)UIM_RESET_DEFAULT_GPIO, DAL_TLMM_GPIO_ENABLE );
    (void)DalTlmm_GpioOut   ( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr, (DALGpioSignalType)UIM_RESET_DEFAULT_GPIO, DAL_GPIO_LOW_VALUE);
  }
  else
  {
    (void)DalTlmm_ConfigGpio( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr, (DALGpioSignalType)UIM2_RESET_DEFAULT_GPIO, DAL_TLMM_GPIO_ENABLE );
    (void)DalTlmm_GpioOut   ( uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr, (DALGpioSignalType)UIM2_RESET_DEFAULT_GPIO, DAL_GPIO_LOW_VALUE);
  }
#endif
  return TRUE;
} /* uimConfigureResetDefaultGpio */


/* Configure data for GPIO/TLMM */
boolean uimConfigureDataDefaultGpio(uim_instance_enum_type uim_instance)
{
#ifndef T_RUMI_EMULATION
  if( NULL ==uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr ||
      0 == uim_hw_if.gpio[uim_instance].m_DataGpioId )
  {
    return FALSE;
  }

  DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_DataGpioId,
                           UIM_TLMM_IO_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_DataGpioConfigId));
  DalTlmm_GpioIdOut(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                     uim_hw_if.gpio[uim_instance].m_DataGpioId,
                     DAL_GPIO_LOW_VALUE);
#else
  if(UIM_INSTANCE_1 == uim_instance)
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM_DATA_DEFAULT_GPIO,
                             DAL_TLMM_GPIO_ENABLE);
    (void)DalTlmm_GpioOut  (uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           (DALGpioSignalType)UIM_DATA_DEFAULT_GPIO,
                            DAL_GPIO_LOW_VALUE);
  }
  else
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM2_DATA_DEFAULT_GPIO,
                             DAL_TLMM_GPIO_ENABLE);
    (void)DalTlmm_GpioOut  (uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           (DALGpioSignalType)UIM2_DATA_DEFAULT_GPIO,
                            DAL_GPIO_LOW_VALUE);
  }
#endif
  return TRUE;
} /* uimConfigureDataDefaultGpio */


/* configure clock for GPIO/TLMM */
boolean uimConfigureClkDefaultGpio(uim_instance_enum_type uim_instance)
{
#ifndef T_RUMI_EMULATION
  if( NULL == uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr ||
      0 == uim_hw_if.gpio[uim_instance].m_ClkGpioId )
  {
    return FALSE;
  }

  DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_ClkGpioId,
                           UIM_TLMM_IO_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_ClkGpioConfigId));
  DalTlmm_GpioIdOut(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                    uim_hw_if.gpio[uim_instance].m_ClkGpioId,
                    DAL_GPIO_LOW_VALUE);
#else
  if(UIM_INSTANCE_1 == uim_instance)
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM_CLK_DEFAULT_GPIO,
                             DAL_TLMM_GPIO_ENABLE);
    (void)DalTlmm_GpioOut   (uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM_CLK_DEFAULT_GPIO,
                              DAL_GPIO_LOW_VALUE);
  }
  else
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM2_CLK_DEFAULT_GPIO,
                             DAL_TLMM_GPIO_ENABLE);
    (void)DalTlmm_GpioOut   (uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM2_CLK_DEFAULT_GPIO,
                              DAL_GPIO_LOW_VALUE);
  }
#endif
  return TRUE;
} /* uimConfigureClkDefaultGpio */


/* configure data for UIM controller */
boolean uimConfigureDataUimController(uim_instance_enum_type uim_instance)
{
#ifndef T_RUMI_EMULATION
  if( NULL == uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr ||
      0 == uim_hw_if.gpio[uim_instance].m_DataGpioId )
  {
    return FALSE;
  }
  DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_DataGpioId,
                           UIM_TLMM_UIMCTRL_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_DataGpioConfigId));
#else
  if(UIM_INSTANCE_1 == uim_instance)
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM_DATA_UIMCNTRL_GPIO,
                             DAL_TLMM_GPIO_ENABLE);
  }
  else
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM2_DATA_UIMCNTRL_GPIO,
                             DAL_TLMM_GPIO_ENABLE);
  }
#endif
  return TRUE;
} /* uimConfigureDataUimController */


/* configure clock for UIM controller */
boolean uimConfigureClkUimController(uim_instance_enum_type uim_instance)
{
#ifndef T_RUMI_EMULATION
  if( NULL == uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr ||
      0 == uim_hw_if.gpio[uim_instance].m_ClkGpioId )
  {
    return FALSE;
  }
  DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_ClkGpioId,
                           UIM_TLMM_UIMCTRL_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_ClkGpioConfigId));
#else
  if(UIM_INSTANCE_1 == uim_instance)
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM_CLK_UIMCNTRL_GPIO,
                             DAL_TLMM_GPIO_ENABLE);
  }
  else
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                             (DALGpioSignalType)UIM2_CLK_UIMCNTRL_GPIO,
                             DAL_TLMM_GPIO_ENABLE);
  }
#endif
  return TRUE;
} /* uimConfigureClkUimController */


/* configure bat alarm GPIO */
void uimConfigureBattAlarmUimController(uim_instance_enum_type uim_instance)
{
  /* Only configure once by the first task that get BattAlarm GpioId */
  if( NULL == uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr ||
      0 == uim_hw_if.gpio[uim_instance].m_BattAlarmGpioId )
  {
    return;
  }
  DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_BattAlarmGpioId,
                           UIM_TLMM_UIMCTRL_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_BattAlarmGpioConfigId));
} /* uimConfigureBattAlarmUimController */

/*===========================================================================
FUNCTION UIM_SET_RESET_AS_OUTPUT_SLOT

DESCRIPTION
  UIM_SET_RESET_AS_OUTPUT configures the GPIO connected to the RESET line.
  UIM1 RESET: GPIO1, FUNC_SEL = 0
  UIM2 RESET: GPIO5, FUNC_SEL = 0
===========================================================================*/
void UIM_SET_RESET_AS_OUTPUT_SLOT(uim_instance_global_type *uim_ptr)
{
  setResetAsOutput(uim_ptr->id);
  return;
} /* UIM_SET_RESET_AS_OUTPUT_SLOT */


/*===========================================================================
FUNCTION ENABLE_UART_INTERFACE_SLOT

DESCRIPTION
  ENABLE_UART_INTERFACE configures the GPIOs connected to the DATA and
  CLOCK lines to utilize the UART (func_sel=1).

  UIM1 DATA: GPIO3, FUNC_SEL = 1
  UIM1 CLOCK: GPIO2, FUNC_SEL = 1
  UIM2 DATA: GPIO7, FUNC_SEL = 1
  UIM2 CLOCK: GPIO6, FUNC_SEL = 1
===========================================================================*/
void ENABLE_UART_INTERFACE_SLOT(uim_instance_global_type *uim_ptr)
{
  enableUartInterface(uim_ptr->id);
  return;
} /* ENABLE_UART_INTERFACE_SLOT */


/*===========================================================================
FUNCTION UIM_CONFIGURE_RESET_DEFAULT_GPIO

DESCRIPTION

===========================================================================*/
void UIM_CONFIGURE_RESET_DEFAULT_GPIO(uim_instance_global_type *uim_ptr)
{
  uimConfigureResetDefaultGpio(uim_ptr->id);
  return;
} /* UIM_CONFIGURE_RESET_DEFAULT_GPIO */


/*===========================================================================
FUNCTION UIM_CONFIGURE_DATA_DEFAULT_GPIO

DESCRIPTION

===========================================================================*/
void UIM_CONFIGURE_DATA_DEFAULT_GPIO(uim_instance_global_type *uim_ptr)
{
  uimConfigureDataDefaultGpio(uim_ptr->id);
  return;
} /* UIM_CONFIGURE_DATA_DEFAULT_GPIO */


/*===========================================================================
FUNCTION UIM_CONFIGURE_CLK_DEFAULT_GPIO

DESCRIPTION

===========================================================================*/
void UIM_CONFIGURE_CLK_DEFAULT_GPIO(uim_instance_global_type *uim_ptr)
{
  uimConfigureClkDefaultGpio(uim_ptr->id);
  return;
} /* UIM_CONFIGURE_CLK_DEFAULT_GPIO */


/*===========================================================================
FUNCTION UIM_CONFIGURE_DATA_FOR_UIM_CONTROLLER

DESCRIPTION

===========================================================================*/
void UIM_CONFIGURE_DATA_FOR_UIM_CONTROLLER(uim_instance_global_type *uim_ptr)
{
  uimConfigureDataUimController(uim_ptr->id);
  return;
} /* UIM_CONFIGURE_DATA_FOR_UIM_CONTROLLER */


/*===========================================================================
FUNCTION UIM_CONFIGURE_CLK_FOR_UIM_CONTROLLER

DESCRIPTION

===========================================================================*/
void UIM_CONFIGURE_CLK_FOR_UIM_CONTROLLER(uim_instance_global_type *uim_ptr)
{
  uimConfigureClkUimController(uim_ptr->id);
  return;
} /* UIM_CONFIGURE_CLK_FOR_UIM_CONTROLLER */


/*===========================================================================
FUNCTION UIM_CONFIGURE_BATT_ALARM_FOR_UICC_SHUTDOWN

DESCRIPTION

===========================================================================*/
void UIM_CONFIGURE_BATT_ALARM_FOR_UICC_SHUTDOWN(uim_instance_global_type *uim_ptr)
{
  uimConfigureBattAlarmUimController(uim_ptr->id);
  return;
} /* UIM_CONFIGURE_BATT_ALARM_FOR_UICC_SHUTDOWN */


/*===========================================================================
FUNCTION UIM_ASSERT_RESET_SLOT

DESCRIPTION
  Brings the RESET line LOW.
===========================================================================*/
void UIM_ASSERT_RESET_SLOT(uim_instance_global_type *uim_ptr)
{
  assertReset(uim_ptr->id);
  return;
} /* UIM_ASSERT_RESET_SLOT */


/*===========================================================================
FUNCTION UIM_DEASSERT_RESET_SLOT

DESCRIPTION
  Brings the RESET line LOW.
===========================================================================*/
void UIM_DEASSERT_RESET_SLOT(uim_instance_global_type *uim_ptr)
{
  deassertReset(uim_ptr->id);
  return;
} /* UIM_DEASSERT_RESET_SLOT */

