/*============================================================================
  FILE:         uimdrv_pmic.c

  OVERVIEW:     File conatins the functions used to initialize and control
                the pmic interface.

  DEPENDENCIES: N/A

                Copyright (c) 2012 - 2018, 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hw/pmic/uimdrv_pmic.c#4 $
$DateTime: 2021/08/09 05:06:20 $
$Author: pwbldsvc $

when        who        what, where, why
------      ----       ----------------------------------------------------------- 
08/05/21    ykr        Set MODE18 bit to one in UART_DM_UIM_CFG during power-off
08/03/21    ykr        Fixed KW issue
03/14/18    ak         Execute GPIO init only when BUA check returns TRUE  
11/17/17    ku         Adding interface layer for UIM Drivers
05/31/16    gm         F3 message removal
10/27/15    hyo        Avoid writing enum to EFS
07/20/15    ks         Consolidating UIM bring-up changes
06/08/15    ks         SW defaults for the NV 73588.
05/05/15    ks         Enhancing the voltage supply indication to QMI feature
04/28/15    ll         9x55 Bring-up changes
04/28/15    sam        Configure LDO out of Reset state in every UIM power up
04/08/15    sam        FR24498: Powerup logging new changes
03/19/15    na         Numerical value replaced with DALCHIPINFO_FAMILY_MSM8952
03/03/15    hyo        8996 Bring-up changes
02/12/15    ll         Not wait for QMI ACK after receiving TASK_STOP
01/07/15    sam        Modification of msg macros for FR24498-UIM Powerup Logging
01/05/15    ll         Compile warnings
12/30/14    ks         supoport for UIM3 external LDO on 8909
11/12/14    js         Fixed compiler warning
11/11/14    ks         Using the DAL chip enums instad of hardcoding values
11/10/14    akv        Support for user to customize LDO settings
11/06/14    nmb        8952 bring up changes
09/23/14    ks         8909 bring up changes
09/22/14    akv        UIMDRV UT - Featurization while invoking of QMI API removed
09/18/14    akv        UIMDRV UT enhancements
08/28/14    akv        UIMDRV-QMI interactions - Indications and acks for LDO events
08/21/14    ak         Replace 9x45 DAL chip family reference for compilation on BOLT
08/20/14    nmb        9x45 Bring up changes
07/25/14    lxu        Read cmd_rsp_time from nv for usb mode, give cmd_rsp_time and
                       voltage_class to ap when send usb power up to ap
07/09/14    ak         Follow new PMIC requirement for enabling batt removal shutdown
05/12/14    ak         8994 Bring up changes
05/06/14    ks         8936 bring up changes
04/11/14    lxu        Unregister automatic UICC shutdown on card removal in USB mode
02/18/14    sam        Usage of newer style diag macros
01/24/14    yk         Code Clean up
12/27/13    ks         8916 bring up changes
11/27/13    akv        Initialization of hw enumeration based on dal chip family
10/08/13    nmb        HW header file cleanup
10/03/13    sam        F3 log reduction
09/12/12    na         8x62 bring up changes
08/29/13    na         allow UIM2 config on UIM1 when UIM1 is disabled and UIM2 enabled
08/06/13    rm/ak      Bolt RUMI code
07/12/13    nmb        Global Reorganization
07/12/13    akv        HW Enumeration - modifying target specific hardware values
                       and organizing them in value-key pairs
06/24/13    js         TSTS changes
05/22/13    akv        Configuration of UICC shutdown gpio's and feature
                       enablement based on NV
05/16/13    akv        Fix for compilation warning on Triton
03/27/13    akv        UICC shutdown upon battery removal
03/18/13    js         8x26 UIM3 interface bring up fixes.
02/22/13    js         8x26 bring up changes
02/12/13    js         General F3 macro cleanup
02/01/13    akv        Run time configuration of UICC shutdown feature
12/05/12    js         UIM Parallel Processing changes
10/26/12    akv/ssr    HW Enumeration changes
============================================================================*/
#include "uimdrv_pmic.h"
#include "uimdrv_uartdm.h"
#include "uimdrv_rumi.h"
#include "DDIChipInfo.h"
#include "DDIPlatformInfo.h"
#include "uimglobals.h"
#include "uim_common_efs.h"
#include "qmi_uim.h"
#include "uimdrv_gpio.h"

static char pmic_npa_group_id_uim_enable[][30]      =
   {PMIC_NPA_GROUP_ID_UIM1_ENABLE, PMIC_NPA_GROUP_ID_UIM2_ENABLE,
    PMIC_NPA_GROUP_ID_UIM3_ENABLE};
static char pmic_npa_group_id_uim_activity[][30]    =
   {PMIC_NPA_GROUP_ID_UIM1_ACTIVITY, PMIC_NPA_GROUP_ID_UIM2_ACTIVITY,
   PMIC_NPA_GROUP_ID_UIM3_ACTIVITY};


/**
*  This function does the initial handshaking needed with PMIC
*  to get all client handles used to communicate with PMIC
*  further.
*
*  @param uim_instance   uim instance.
*/
void initialize_pmic(uim_instance_enum_type uim_instance)
{
  uimVccEnum                  pmic_ref;

  RETURN_IF_INSTANCE_INVALID(uim_instance);

  uim_hw_if.pmic[uim_instance].m_pmicNpaHandle = NULL;
  uim_hw_if.pmic[uim_instance].m_pmicActivityHandle  = NULL;

  pmic_ref = m_HWConfig.uim_slot_config[uim_instance].uimVcc;

  /* store the instance of the slot that are active */
  uim_hw_if.pmic[uim_instance].m_Vcc                 = pmic_ref;
  uim_hw_if.pmic[uim_instance].m_uiccShutBattRemoval = m_HWConfig.uicc_shutdown_feature.uiccShutdownBattRemoval;
  uim_hw_if.pmic[uim_instance].m_uiccShutCardRemoval = m_HWConfig.uicc_shutdown_feature.uiccShutdownCardRemoval[uim_instance];

  uimdrv_pmic_efs_get_user_ldo_config(uim_instance);

  uimdrv_pmic_set_voltage_regulator_settings(uim_instance);

  uim_hw_if.pmic[uim_instance].m_pmicNpaHandle =
    npa_create_sync_client(pmic_npa_group_id_uim_enable[pmic_ref],
                           "uim", NPA_CLIENT_REQUIRED);
  uim_hw_if.pmic[uim_instance].m_pmicActivityHandle =
    npa_create_sync_client (pmic_npa_group_id_uim_activity[pmic_ref],
                            "uim", NPA_CLIENT_REQUIRED);

  UIMDRV_MSG_ERR_2 ( uim_instance,"UIM pmicNpaHandle: 0x%x  pmicActivityHandle: 0x%x for pmic instance", 
                     uim_hw_if.pmic[uim_instance].m_pmicNpaHandle, uim_hw_if.pmic[uim_instance].m_pmicActivityHandle);
} /* initialize_pmic */


/**
*  This function powers up the UIM LDO and setting the dual
*  voltage UIM GPIO pads to the appropriate voltage level.
*
*  @param uim_instance   uim instance.
*/
boolean uimPowerOn(uim_instance_enum_type uim_instance)
{
  if(uim_instance>= UIM_MAX_INSTANCES)
  {
    return FALSE;
  }

  if(!(uim_hw_if.pmic[uim_instance].m_pmicNpaHandle) && (uim_hw_if.uim_is_external_ldo[uim_instance] == FALSE))
  {
    UIMDRV_MSG_ERR_0 ( uim_instance,"uimPowerOn - NULL pmicNpaHandle detected");
    return FALSE;
  }
  
  /* Check for BUA init. This is because for SDM845 and above, we do not
     use BUA pin.*/
  if (TRUE == uim_check_bua_mpm_irq_reg_applicable()) 
  {
    uim_set_shutdown_control(uim_instance, TRUE);
  }

  if (UIM_VOLTAGE_CLASS_C == uim_hw_if.pmic[uim_instance].m_voltage_class)
  {
    if(uim_hw_if.uim_is_external_ldo[uim_instance] == TRUE)
    {
      uimConfigureUimExtLdoToClassC(uim_instance);
      return TRUE;
    }

    if (uim_hw_if.pmic[uim_instance].m_pmicNpaHandle)
    {
      uimdrv_pmic_set_ldo_voltage(uim_instance,
                                  uim_hw_if.pmic[uim_instance].m_pmicNpaHandle,
                                  uim_hw_if.pmic[uim_instance].m_classC_voltageRegulator);
      uim_hw_if.pmic[uim_instance].m_pmic_npa_mode = uim_hw_if.pmic[uim_instance].m_classC_voltageRegulator;
    }

    uimdrv_rumi_vsel_on_class_c(uim_instance);

    /* Wait to ensure that the 20ns sequencing requirement from the pad
       specification is met */
    uim_clk_busy_wait(1);

    prgUIMCFG(uim_instance, 0, TRUE, UIM_SET_MODE_18, SET);
  }
  else if(UIM_VOLTAGE_CLASS_B == uim_hw_if.pmic[uim_instance].m_voltage_class)
  {
    if(uim_hw_if.uim_is_external_ldo[uim_instance] == TRUE)
    {
      uimConfigureUimExtLdoToClassB(uim_instance);
      return TRUE;
    }

    prgUIMCFG(uim_instance, 0, TRUE, UIM_CLR_MODE_18, CLR);

    /* Wait to ensure that the 20ns sequencing requirement from the pad
       specification is met */
    uim_clk_busy_wait(1);

    if (uim_hw_if.pmic[uim_instance].m_pmicNpaHandle)
    {
      uimdrv_pmic_set_ldo_voltage(uim_instance,
                                  uim_hw_if.pmic[uim_instance].m_pmicNpaHandle,
                                  uim_hw_if.pmic[uim_instance].m_classB_voltageRegulator);
      uim_hw_if.pmic[uim_instance].m_pmic_npa_mode = uim_hw_if.pmic[uim_instance].m_classB_voltageRegulator;
    }

    uimdrv_rumi_vsel_on_class_b(uim_instance);
  }
  uim_clk_busy_wait(200);
  return TRUE;
} /* uimPowerOn */


/**
*  This function powers down the UIM LDO.
*
*  @param uim_instance   uim instance.
*/
boolean uimPowerOff(uim_instance_enum_type uim_instance)
{
#ifdef FEATURE_UIM_DIVAR 
  uint32  data = 0x00;
#endif /* FEATURE_UIM_DIVAR */

  if(!(uim_hw_if.pmic[uim_instance].m_pmicNpaHandle) && (uim_hw_if.uim_is_external_ldo[uim_instance] == FALSE))
  {
    UIMDRV_MSG_ERR_0(uim_instance,"uimPowerOff - NULL pmicNpaHandle detected");
    return FALSE;
  }

  if(uim_hw_if.uim_is_external_ldo[uim_instance] == TRUE)
  {
    uimConfigureUimExtLdoToZero(uim_instance);
    return TRUE;
  }
#ifdef FEATURE_UIM_DIVAR  
  prgUIMCFG(uim_instance, 0, TRUE, UIM_SET_MODE_18, SET);
#endif /* FEATURE_UIM_DIVAR */

  uimdrv_pmic_set_ldo_voltage(uim_instance,
                              uim_hw_if.pmic[uim_instance].m_pmicNpaHandle,
                              PMIC_NPA_MODE_ID_UIM_STANDBY);
  uimdrv_rumi_vsel_off(uim_instance);

#ifdef FEATURE_UIM_DIVAR    
  /* Read UART_DM_UIM_CFG register and print the value */
  data = UIM_HWIO_IN(uim_hw_if.uart[uim_instance].start_addr + UART_DM_UIM_CFG);
  UIMDRV_MSG_HIGH_1(uim_instance,"uimPowerOff UIM_CFG: 0x%x",data);
#endif /* FEATURE_UIM_DIVAR */  
  return TRUE;
} /* uimPowerOff */


/**
*  This function is used to set the UIM LDO into HPM (High power
*  mode) during UIM activity and to LPM (Low power mode) during
*  inactivity.
*
*  @param uim_ptr    Pointer to the current instance of the UIM
*                    global structure.
*  @param powermode  Power mode (High power mode or Low power
*                    mode)
*/
void uimdrv_pmic_vote_for_ldo_powermode(uim_instance_global_type  *uim_ptr,
                                        uim_ldo_powermodes_type   powermode)
{
  if(!(uim_hw_if.pmic[uim_ptr->id].m_pmicActivityHandle))
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"uim_vote_for_ldo_powermode - NULL pmicActivityHandle detected");
    return;
  }
  npa_issue_required_request(uim_hw_if.pmic[uim_ptr->id].m_pmicActivityHandle,
                             powermode);
  return;
} /* uimdrv_pmic_vote_for_ldo_powermode */


/**
*  This function is used to issue a request to npa to set the
*  UIM LDO to the desired voltage.
*
*  @param uim_instance   uim instance.
*  @param npa_handle     Client resource handle.
*  @param req            Required request.
*/
void uimdrv_pmic_set_ldo_voltage(uim_instance_enum_type  uim_instance,
                                 npa_client_handle       npa_handle,
                                 npa_resource_state      req)
{
  boolean ldo_behaviour_modification_allowed = FALSE;

  if(!(npa_handle))
  {
    UIMDRV_MSG_ERR_0(uim_instance,"uim_vote_for_ldo_activation_or_deactivation - NULL npa_handle detected");
    return;
  }

  ldo_behaviour_modification_allowed = uim_evaluate_req_ldo_voltage_mod(uim_instance, req);

  if(ldo_behaviour_modification_allowed)
  {
    npa_issue_required_request(npa_handle, req);
  }
  return;
} /* uimdrv_pmic_set_ldo_voltage */


/**
*  This function is used to set the voltage class uim global
*  variable.
*
*  @param uim_ptr         Pointer to the current instance of the
*                         UIM global structure.
*  @param voltage_class   Voltage class
*/
void uim_program_voltage_class
(
  uim_instance_global_type *uim_ptr,
  uim_voltage_class_type    voltage_class
)
{
  if(uim_ptr == NULL)
  {
    return;
  }

  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);
  uim_hw_if.pmic[uim_ptr->id].m_voltage_class = voltage_class;
  return;
} /* uim_program_voltage_class */


/**
*  This function is used to power off the ldo.
*
*  @param uim_ptr      Pointer to the current instance of the
*                      UIM global structure.
*/
void uim_power_off_ldo_slot(uim_instance_global_type *uim_ptr)
{
  if(!uim_ptr->flag.powering_down_task)
  {
    uim_notify_voltage_change_indication(uim_ptr, UIMDRV_LDO_AWAITING_DEACTIVATION_IND);
  }
  uimdrv_rumi_config_gpios(uim_ptr->id);
  uimPowerOff(uim_ptr->id);
  return;
} /* uim_power_off_ldo_slot */


/**
*  This function is used to power on the UIM ldo and set the
*  Dual GPIO pads to the appropriate voltage.
*
*  @param uim_ptr      Pointer to the current instance of the
*                      UIM global structure.
*/
void uim_power_on_ldo_slot(uim_instance_global_type *uim_ptr)
{
  uimdrv_rumi_config_gpios(uim_ptr->id);
  uimPowerOn(uim_ptr->id);
  uim_notify_voltage_change_indication(uim_ptr, UIMDRV_LDO_ACTIVATED_IND);
  return;
} /* uim_power_on_ldo_slot */


/**
*  This function is used to control UICC shutdown upon card
*  removal and UICC shutdown upon battery removal
*
*  @param uim_instance    uim instance.
*  @param enable          TRUE - indicates that the we are
*                         attempting to enable the uicc shutdown
*/
void uim_set_shutdown_control(uim_instance_enum_type uim_instance, boolean enable)
{
  pm_err_flag_type pm_err = PM_ERR_FLAG__SUCCESS;
  /* UICC Shutdown upon card removal */
  /*
     PMIC interface expects UIM to call pm_uicc_cntrl_enable with parameters
      -- 1 and TRUE for enabling UICC shutdown upon UIM1 card removal,
      -- 2 and TRUE for enabling UICC shutdown upon UIM2 card removal,
      -- 3 and TRUE for enabling UICC shutdown upon UIM3 card removal.
  */
  UIMDRV_PUP_MSG_HIGH_3(UIMLOG_MSG_230,uim_instance, "UICC set shutdown upon card removal for slot is %d. m_uiccShutCardRemoval = %d, m_hs_enable = %d",
                 enable,
                 uim_hw_if.pmic[uim_instance].m_uiccShutCardRemoval,
                 uim_hw_if.intctrl[uim_instance].m_hs_enable);

  /* Bring UIM LDO out of reset state in every UIM power up */
  pm_err = pm_uicc_cntrl_enable(uim_hw_if.pmic[uim_instance].m_Vcc + 1, FALSE);
  if (TRUE == uim_hw_if.pmic[uim_instance].m_uiccShutCardRemoval &&
      TRUE == uim_hw_if.intctrl[uim_instance].m_hs_enable )
  {
    pm_err = pm_uicc_cntrl_enable(uim_hw_if.pmic[uim_instance].m_Vcc + 1, enable);
    if (PM_ERR_FLAG__SUCCESS != pm_err)
    {
      UIMDRV_MSG_ERR_1(uim_instance, "PMIC UICC card removal enable failed: %d",pm_err);
    }
  }

  /* UICC Shutdown upon battery removal */
  /*
     PMIC interface expects UIM to call pm_uicc_cntrl_enable with parameters
      -- 0 and TRUE for enabling battery removal batt alarm
      -- 1 and TRUE for enabling UICC shutdown for UIM1
      -- 2 and TRUE for enabling UICC shutdown for UIM2
      -- 3 and TRUE for enabling UICC shutdown for UIM3
  */
  UIMDRV_PUP_MSG_HIGH_2(UIMLOG_MSG_231,uim_instance, "UICC set shutdown upon batt removal for slot is %d. m_uiccShutBattRemoval = %d",
                         enable,uim_hw_if.pmic[uim_instance].m_uiccShutBattRemoval);
  if (TRUE == uim_hw_if.pmic[uim_instance].m_uiccShutBattRemoval)
  {
    pm_err = pm_uicc_cntrl_enable(PM_BUA_BATT_ALARM, enable);
    if (PM_ERR_FLAG__SUCCESS != pm_err)
    {
      UIMDRV_MSG_ERR_1(uim_instance, "PMIC UICC BATT ALARM enable failed: %d",pm_err);
    }

    pm_err = pm_uicc_cntrl_enable(uim_hw_if.pmic[uim_instance].m_Vcc + 1, enable);
    if (PM_ERR_FLAG__SUCCESS != pm_err)
    {
      UIMDRV_MSG_ERR_1(uim_instance, "PMIC UICC BATT ALARM instance enable failed: %d",pm_err);
    }
  }
  return;
} /* uim_set_shutdown_control */


/**
*  This function is used to return the pmic npa node.
*
*  @param uim_instance    uim instance.
*/
uint32 uim_get_pmic_npa_mode(uim_instance_enum_type uim_instance)
{
  return uim_hw_if.pmic[uim_instance].m_pmic_npa_mode;
}/* uim_get_pmic_npa_mode */
