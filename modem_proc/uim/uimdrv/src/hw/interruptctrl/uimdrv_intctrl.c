/*============================================================================
  FILE:         uimdrv_intctrl.c

  OVERVIEW:     File contains the interrupt control functions.

  DEPENDENCIES: N/A

                Copyright (c) 2012-19 QUALCOMM Technologies, Inc (QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hw/interruptctrl/uimdrv_intctrl.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when        who        what, where, why
------      ----       -----------------------------------------------------------
07/18/19    ykr        Registering for UIM hotswap wakeup capable interrupts 
12/05/17    gm         Update only active slots in uim_get_hw_slot_info
07/22/16    hyo        Napali Chip Info update  
07/05/16    hyo        Napali bring-up changes
04/29/16    sam        Reorganise the hotswap mpm wakeup cabale registration code
03/28/16    sam        Register UIM1 hotswap interrupt eventhough it is disabled
09/02/15    hyo        Drivers and state machine abstraction
07/20/15    ks         Consolidating UIM bring-up changes
06/23/15    ll         Guard GPIOID checking for hotswap on RUMI
04/28/15    ll         9x55 Bring-up changes
04/08/15    sam        FR24498: Powerup logging new changes
03/19/15    na         Numerical value replaced with DALCHIPINFO_FAMILY_MSM8952
03/03/15    hyo        8996 Bring-up changes
12/02/14    ks         Feature to support GPIO based hotswap
11/11/14    ks         Using the DAL chip enums instad of hardcoding values
11/06/14    nmb        8952 bring up changes
09/23/14    ks         8909 bring up changes
09/08/14    ks         Enable hotswap feature if flaky mechanical switch is enabled
08/21/14    ak         Replace 9x45 DAL chip family reference for compilation on BOLT
08/20/14    nmb        9x45 Bring up changes
05/12/14    ak         8994 Bring up changes
05/06/14    ks         8936 bring up changes
02/18/14    sam        Usage of newer style diag macros
01/24/14    yk         Code Clean up
01/23/14    rm         Fix for 8916 to check for correct family
12/27/13    ks         8916 bring up changes
11/27/13    akv        Initialization of hw enumeration based on dal chip family
10/08/13    nmb        HW header file cleanup
09/11/13    na         8x62 bring up changes
08/06/13    rm/ak      Bolt RUMI code
07/12/13    akv        HW Enumeration - modifying target specific hardware values
                       and organizing them in value-key pairs
07/12/13    nmb        Global Reorganization
02/12/13    js         General F3 macro cleanup
12/05/12    js         UIM Parallel Processing changes
10/26/12    akv/ssr    HW Enumeration changes
============================================================================*/
#include "uimdrv_intctrl.h"
#include "uimglobals.h"
#include "uim_hotswap.h"
#include "uimdrv_hal_iso.h"


void uimIntctrlIsr (uint32 *context )
{
  uimdrv_iso_rx_isr((uim_instance_global_type*)context);
} /* uimIntctrlIsr */


void uimUimctrlIsr (uint32 *context )
{
  uim_hotswap_gpio_isr((uim_instance_global_type*)context);
} /* uimUimctrlIsr*/


/* Initialize interrupt controller */
void initialize_intctrl(uim_instance_global_type *uim_ptr)
{
  DALResult                    dal_attach    = DAL_SUCCESS;
  uim_instance_enum_type       uim_instance  = uim_ptr->id;

  uim_hw_if.intctrl[uim_instance].client_callback_param = (uint32)uim_ptr;
  uim_hw_if.intctrl[uim_instance].m_UimCtrl_handle_ptr  = NULL;
  uim_hw_if.intctrl[uim_instance].m_IntCtrl_handle_ptr  = NULL;
  uim_hw_if.intctrl[uim_instance].m_hs_enable = FALSE;

  uim_hw_if.intctrl[uim_instance].m_hs_enable           = m_HWConfig.uim_slot_config[uim_instance].enableUimHotswap;

  if( (FALSE == uim_hw_if.intctrl[uim_instance].m_hs_enable) &&
      (TRUE == uim_ptr->hotswap.hotswap_flaky_switch_support))
  {
    /* Enable hotswap feature (even NV says it is disabled)
       if the flaky mechanical switch feature is enabled */
    uim_hw_if.intctrl[uim_instance].m_hs_enable = TRUE;
  }

/* Do this only for on-target,as in off-target GpioConfigID is always ZERO */
#ifndef FEATURE_UIM_TEST_FRAMEWORK
#ifndef T_RUMI_EMULATION
  if(uim_hw_if.gpio[uim_instance].m_PresentGpioId == 0)
  {
    uim_hw_if.intctrl[uim_instance].m_hs_enable = FALSE;
  }
#endif /*T_RUMI_EMULATION*/
#endif /*FEATURE_UIM_TEST_FRAMEWORK*/

  uim_hw_if.intctrl[uim_instance].m_hs_polarity         = m_HWConfig.uim_slot_config[uim_instance].uimHotswapPolarity;
  uim_hw_if.intctrl[uim_instance].m_uartIRQNum          = m_HWConfig.uim_slot_config[uim_instance].uimUartIRQNum;
  uim_hw_if.intctrl[uim_instance].m_uimControllerIRQNum = m_HWConfig.uim_slot_config[uim_instance].uimControllerIRQNum;


  dal_attach = DAL_DeviceAttach(DALDEVICEID_INTERRUPTCONTROLLER, &(uim_hw_if.intctrl[uim_instance].m_IntCtrl_handle_ptr) );
  if((!uim_hw_if.intctrl[uim_instance].m_IntCtrl_handle_ptr)||(dal_attach != DAL_SUCCESS))
  {
    UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_226,uim_instance,"Failed to register with Interrupt Controller");
    return;
  }
  (void) DalDevice_Open(uim_hw_if.intctrl[uim_instance].m_IntCtrl_handle_ptr,DAL_OPEN_SHARED);
  DalInterruptController_RegisterISR(uim_hw_if.intctrl[uim_instance].m_IntCtrl_handle_ptr,
                                     uim_hw_if.intctrl[uim_instance].m_uartIRQNum,
                                     (DALISR) uimIntctrlIsr,
                                     (const DALIRQCtx) uim_ptr,
                                     DALINTRCTRL_ENABLE_LEVEL_HIGH_TRIGGER);
  if (TRUE == uim_hw_if.intctrl[uim_instance].m_hs_enable)
  {
    if(uim_hotswap_nature[uim_instance] != 0)
    {
      dal_attach = DAL_DeviceAttach(DALDEVICEID_GPIOINT, &(uim_hw_if.intctrl[uim_instance].m_UimCtrl_handle_ptr));
      if((!uim_hw_if.intctrl[uim_instance].m_UimCtrl_handle_ptr)||(dal_attach != DAL_SUCCESS))
      {
        UIMDRV_MSG_ERR_0(uim_instance,"Failed to register with UIM Interrupt Controller");
        uim_hw_if.intctrl[uim_instance].m_hs_enable = FALSE;
      }
      return;
    }
    dal_attach = DAL_DeviceAttach(DALDEVICEID_INTERRUPTCONTROLLER, &(uim_hw_if.intctrl[uim_instance].m_UimCtrl_handle_ptr));
    if((!uim_hw_if.intctrl[uim_instance].m_UimCtrl_handle_ptr)||(dal_attach != DAL_SUCCESS))
    {
      UIMDRV_MSG_ERR_0(uim_instance,"Failed to register with UIM Interrupt Controller");
      uim_hw_if.intctrl[uim_instance].m_hs_enable = FALSE;
      return;
    }
    (void) DalDevice_Open(uim_hw_if.intctrl[uim_instance].m_UimCtrl_handle_ptr,DAL_OPEN_SHARED);
    DalInterruptController_RegisterISR((DalDeviceHandle*)uim_hw_if.intctrl[uim_instance].m_UimCtrl_handle_ptr,
                                      (DALInterruptID) uim_hw_if.intctrl[uim_instance].m_uimControllerIRQNum,
                                      (DALISR) uimUimctrlIsr,
                                      (const DALIRQCtx) uim_ptr,
                                       DALINTRCTRL_ENABLE_LEVEL_HIGH_TRIGGER);

    
  }
} /* initialize_intctrl */

/*Register for IRQ that is mapped to UIM card event IRQ in MPM wakeup table*/
void uim_register_for_mpm_wakeup_capability(uim_instance_global_type *uim_ptr)
{
  DalDeviceHandle                        *phChipInfo    = NULL;
  DalChipInfoFamilyType                  chipFamily     = DALCHIPINFO_FAMILY_UNKNOWN;
  uint8                                  nChipindex     = 0;
  DALInterruptID                         interrupt_id   = 0;

  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);

  /*No need to register if Hotswap is disabled*/
  if(FALSE == uim_hw_if.intctrl[uim_ptr->id].m_hs_enable)
  {
    return;
  }

  /*If the UIM instance's (whose IRQ is mapped im MPM table)  hotswap is already enabled,
       then the IRQ is already registered in 'initialize_intrctrl'.
       It neednot be registered again*/
  if((TRUE == uim_hw_slot_info.slot_status[UIM_IRQ_MAPPED_IN_MPM_TABLE_INSTANCE].slot_available) && 
     (TRUE ==  m_HWConfig.uim_slot_config[UIM_IRQ_MAPPED_IN_MPM_TABLE_INSTANCE].enableUimHotswap ||
      TRUE ==  m_HWConfig.uim_hotswap_flaky_switch[UIM_IRQ_MAPPED_IN_MPM_TABLE_INSTANCE]))
  {
    return;
  }

  if (DAL_DeviceAttach(DALDEVICEID_CHIPINFO, &phChipInfo) != DAL_SUCCESS)
  {
    UIM_MSG_ERR_0("DAL_DeviceAttach failed in initialize_intctrl");
    return;
  }

  /*Retreive interrupt ID from IRQ table*/
  DalChipInfo_GetChipFamily(phChipInfo, &chipFamily);
  nChipindex = uim_get_chip_index_from_table(chipFamily);
  interrupt_id = (DALInterruptID)uim_hw_param[nChipindex].controller_hs_irq_num[UIM_IRQ_MAPPED_IN_MPM_TABLE_INSTANCE];

  /*register for the IRQ*/
  DalInterruptController_RegisterISR((DalDeviceHandle*)uim_hw_if.intctrl[uim_ptr->id].m_UimCtrl_handle_ptr,
                                      interrupt_id,
                                      (DALISR) uimUimctrlIsr,
                                      (const DALIRQCtx) uim_ptr,
                                       DALINTRCTRL_ENABLE_LEVEL_HIGH_TRIGGER);

  return;
}/*uim_register_for_mpm_wakeup_capability*/
