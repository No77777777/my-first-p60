/*==============================================================================
  FILE:         q6_lpm_config_v671.c

  OVERVIEW:     This file implements the APIs from q6_lpm_config header file
                on subsystems for specific Q6 versions.

  NOTE:         This file depends on how Q6 core is integrated within a
                subystem.

                Based on HPG_QDSP6SS_v67_1.0.docm (rev 4.91 - May 19, 2021)

  DEPENDENCIES: None

                Copyright (c) 2021 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/sleep2.0/src/target/q6_lpm_config_v671.c#1 $
$DateTime: 2021/05/31 07:21:52 $
==============================================================================*/
#include "comdef.h"
#include "rsc.h"
#include "q6_lpm_config.h"
#include "sleep_hwio.h"
#include "CoreVerify.h"
#include "sleep_target.h"
#include "sleep_statsi.h"

/*==============================================================================
                           EXTERNAL VARIABLES
 =============================================================================*/
/* g_sleepTargetConfig stores target specific config */
extern sleep_target_config g_sleepTargetConfig;

/*==============================================================================
 *                             GLOBAL FUNCTIONS
 *============================================================================*/
/*
 * q6LPMConfig_setupBase
 */
void q6LPMConfig_setupBase(void)
{
  /* Configure Q6SS registers that are invariant across all cpu_vdd low-power modes */
  SLEEP_HWIO_OUT(SLPC_CFG,
               SLEEP_HWIO_FVAL(SLPC_CFG, MEM_FAST_STAGGER,    0x1) | /* XO dual edge */
               SLEEP_HWIO_FVAL(SLPC_CFG, IDLE_CORE_CLK_EN,    0x0) |
               SLEEP_HWIO_FVAL(SLPC_CFG, EXTHW_WAKE_EN,       0x0) | /* Wake from ext hw trig */
               SLEEP_HWIO_FVAL(SLPC_CFG, MEM_PU_PERI_STAGGER, 0x1) | /* mem periph min in-rush */
               SLEEP_HWIO_FVAL(SLPC_CFG, MEM_PD_PERI_STAGGER, 0x0) |
               SLEEP_HWIO_FVAL(SLPC_CFG, MEM_PU_ARRY_STAGGER, 0x1) | /* mem data array min in-runsh */
               SLEEP_HWIO_FVAL(SLPC_CFG, MEM_PD_ARRY_STAGGER, 0x0) |
               SLEEP_HWIO_FVAL(SLPC_CFG, WAKEUP_IN_EN,        0x0) | /* Default - mode based */
               SLEEP_HWIO_FVAL(SLPC_CFG, CLK_GATING_MODE,     0x0)); /* Default - mode based */

#if defined (SLEEP_IMAGE_MSS) || defined (SLEEP_IMAGE_CDSP) || defined (SLEEP_IMAGE_ADSP)
  SLEEP_HWIO_OUTF(CORE_MEM_STAGGER_CTL, DIV, 0x04); /* The DIV value must satisfy the equation (DIV+1)/f >= 3 ns
                                                   * For some high performance QDSP6 in Huracan/Kona/Saipan running above 1.3GHz,
                                                   * program the DIV value to 0x4 */
#else
  SLEEP_HWIO_OUTF(CORE_MEM_STAGGER_CTL, DIV, 0x03);
#endif

  SLEEP_HWIO_OUTF(WAKEUP_CTL, L2VIC_WAKE_EN, 0x01);

  SLEEP_HWIO_OUT(RSCCTL_EN_CLAMP, 0x3);

  SLEEP_HWIO_OUT(RSCCTL_EN_CLK,   0x7ff);

  SLEEP_HWIO_OUT(RSCCTL_EN_MEM_CTL,
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_CTL, CLAMP_QMC_MEM,  0x1) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_CTL, MEM_PERIPH,     0x1));

  SLEEP_HWIO_OUT(RSCCTL_EN_MEM_STAGGER,
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_STAGGER, STAGGER_RESTORE,    0x1) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_STAGGER, STAGGER_ALT_ARES,   0x0) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_STAGGER, STAGGER_FUNC_ARES,  0x1));

  SLEEP_HWIO_OUTF(RSCCTL_EN_BHS, BHS, 0x01);

  q6LPMConfig_setupQ6PLLMode(FALSE);

#ifdef SLEEP_IMAGE_SLPI
  SLEEP_HWIO_OUTF(RSCCTL_EN_PRSC, TCS_HW_TRIGGER,  0xff);
#else
  SLEEP_HWIO_OUTF(RSCCTL_EN_PRSC, TCS_HW_TRIGGER,  0x00);
#endif

  SLEEP_HWIO_OUT(RSC_EVENT_PLL_OVR,
               SLEEP_HWIO_FVAL(RSC_EVENT_PLL_OVR, PLL_DYN_OVRRD,  0x1) |
               SLEEP_HWIO_FVAL(RSC_EVENT_PLL_OVR, PLL_FINE_OVRRD, 0x1) |
               SLEEP_HWIO_FVAL(RSC_EVENT_PLL_OVR, PLL_OVRRD,      0x0));

  SLEEP_HWIO_OUT(RSCCTL_EN_EXT, 0x0);
  
  SLEEP_HWIO_OUT(RSC_EVENT_EXTPLL_OVR, 0x1); 
  
  SLEEP_HWIO_OUT(RSC_EVENT_MEM_OVR, 0x0);

  /* Default to no handshaking with parent / hwsolver */
  sleepTarget_setRpmHandshake(FALSE);

  /* Always defer non-wakeup qTimer frames (0 and 1) because the local wakeup timer is
   * always programmed */
  SLEEP_HWIO_OUT(L2VIC_WAKEUP_EN, WAKEUP_EN_QTMR_FRAME_2);

  return;
}

/*
 * q6LPMConfig_setupModeConfig
 */
void q6LPMConfig_setupModeConfig(rsc_low_power_mode_type coreMode,
                                 boolean                 flag)
{
  uint32 ares;
  uint32 q6WakeHandshake  = 0;
  uint32 wakeupInterrupt  = 0;
  uint32 saveRestore      = 0;
  uint32 configRsc        = 0;
  uint32 ddrSleep         = 0;
#ifdef SLEEP_IMAGE_CDSP
  uint32 q6vtcm           = 0;
#endif
#ifdef SLEEP_IMAGE_MSS
  uint32 l2itcm           = 0;
#endif

  switch(coreMode)
  {
    case RSC_MODE_APCR_PLL_LPM:
    {
      /* RSC Driver does not support efficient storage of sequences when
       * multiple low-power mode employ the same rsc sequence commands.
       * RSC Long sequence supports both FULL PC and APCR_PLL_LPM modes.  */
      coreMode = RSC_MODE_PWRC_BHS;
    }

    case RSC_MODE_APCR_PLL_ON:
#ifdef SLEEP_IMAGE_SLPI
    case RSC_MODE_APCR_PLL_LPM_NO_SS:
#endif
    {
      q6WakeHandshake   = 0x1;
      saveRestore       = 0x1;
      ares              = 0xfefe;
      configRsc         = 0x1; /* Configure RSC low-power mode */
      break;
    }

    case RSC_MODE_L2_NRET_BHS:
    {
      coreMode = RSC_MODE_PWRC_BHS;
    }

    case RSC_MODE_PWRC_BHS:
    {
      ddrSleep          = 0x01; /* Enable FAL10 ddr sleep */
      ares              = 0x1ffff;

#ifdef SLEEP_IMAGE_CDSP
      q6vtcm            = 1;
#endif

#ifdef SLEEP_IMAGE_MSS
      l2itcm            = 0x03;
#endif

      configRsc         = 0x1; /* Configure RSC low-power mode */
      break;
    }

    case RSC_MODE_CLK_GATE:
    {
      /* Clock gating bit overrides all other settings, so just set this one bit.
       * This setting will be in effect until explicitly unset */
      SLEEP_HWIO_OUTF(SLPC_CFG, CLK_GATING_MODE, (flag == TRUE) ? 0x1 : 0x0);
      return;
    }

    default:
    {
      CORE_VERIFY(0);
      break;
    }
  }

  wakeupInterrupt = (flag == TRUE) ? 0x1 : 0x0;
  SLEEP_HWIO_OUTF(SLPC_CFG, WAKEUP_IN_EN, q6WakeHandshake);
#if 0 
  /* For QCTDD05239727 */
  if(0 == ddrSleep)
  {
    SLEEP_HWIO_OUT(RSCCTL_EN_PRSC_CLEAR, (0x01 << SLEEP_HWIO_SHFT(RSCCTL_EN_PRSC, FAL10_VETO)));
  }
  else
  {
    SLEEP_HWIO_OUT(RSCCTL_EN_PRSC_SET, (0x01 << SLEEP_HWIO_SHFT(RSCCTL_EN_PRSC, FAL10_VETO)));
  }

#endif 

#ifdef SLEEP_IMAGE_MSS
  SLEEP_HWIO_OUTF(RSCCTL_EN_L2ITCM_RET, L2ITCM, l2itcm);
#endif

#ifdef SLEEP_IMAGE_CDSP
  SLEEP_HWIO_OUTF(RSCCTL_EN_VTCMEM_RET, VTCM, q6vtcm);
#endif

  SLEEP_HWIO_OUT(RSCCTL_EN_ARES, ares);

  SLEEP_HWIO_OUT(RSCCTL_EN_STATERET,
#if (SLEEP_PROFILING_ID != SLEEP_PMI_ID)
               SLEEP_HWIO_FVAL(RSCCTL_EN_STATERET, PROF_IRQ, 0x1)              | /* Always gen profile int */
#endif
               SLEEP_HWIO_FVAL(RSCCTL_EN_STATERET, WAKE_IRQ, wakeupInterrupt)  | /* PMI interrupt */
               SLEEP_HWIO_FVAL(RSCCTL_EN_STATERET, RESTORE,  saveRestore)      | /* APCR */
               SLEEP_HWIO_FVAL(RSCCTL_EN_STATERET, SAVE,     saveRestore));

  /* Configure RSC if required by lprm */
  if(0 != configRsc)
  {
    sleepTarget_setRscLowPowerMode(RSC_CHILD, coreMode);
  }

  return;
}

/*
 * q6LPMConfig_setupQ6PLLMode
 */
void q6LPMConfig_setupQ6PLLMode(boolean off)
{
  if(TRUE == off)
  {
    /* Turn off PLL for subsystems that require it */
    SLEEP_HWIO_OUT(RSCCTL_EN_PLL,
                 SLEEP_HWIO_FVAL(RSCCTL_EN_PLL, PLL_STBY,     0x1) | /* Power down PLL (standby / off) */
                 SLEEP_HWIO_FVAL(RSCCTL_EN_PLL, PLL_OUTDIS,   0x1) |
                 SLEEP_HWIO_FVAL(RSCCTL_EN_PLL, PLL_OPMODE1,  0x0) | /* Not used */
                 SLEEP_HWIO_FVAL(RSCCTL_EN_PLL, PLL_RESET,    0x1)); /* PLL 0=standby/freeze, 1=off */
  }
  else
  {
    /* Default is normally PLL in standby/freeze */
    SLEEP_HWIO_OUT(RSCCTL_EN_PLL,
                 SLEEP_HWIO_FVAL(RSCCTL_EN_PLL, PLL_STBY,     0x1) | /* Power down PLL (standby / off) */
                 SLEEP_HWIO_FVAL(RSCCTL_EN_PLL, PLL_OUTDIS,   0x1) |
                 SLEEP_HWIO_FVAL(RSCCTL_EN_PLL, PLL_OPMODE1,  0x0) | /* Not used */
                 SLEEP_HWIO_FVAL(RSCCTL_EN_PLL, PLL_RESET,    0x0)); /* PLL 0=standby/freeze, 1=off */
  }

  return;
}

/*
 * q6LPMConfig_setupExtPLLControl
 */
void q6LPMConfig_setupExtPLLControl(boolean q6_controls_extpll)
{
  if(TRUE == q6_controls_extpll)
  {
    SLEEP_HWIO_OUT(RSCCTL_EN_EXT, 0xF);
  
    SLEEP_HWIO_OUT(RSC_EVENT_EXTPLL_OVR, 0x0);   
  }
  else
  {
    SLEEP_HWIO_OUT(RSCCTL_EN_EXT, 0x0);
  
    SLEEP_HWIO_OUT(RSC_EVENT_EXTPLL_OVR, 0x1);   
  }

  return;  
 }


/*
 * q6LPMConfig_setupL2Config
 */
void q6LPMConfig_setupL2Config(mem_state state)
{
  uint32 enMemRet;
  uint32 l2data = SLEEP_HWIO_INF(RSCCTL_EN_L2MEM_RET, L2DATA);

  if(MEM_RET == state)
  {
    /* Setup for cache related logic to be retained (All APCR modes & l2.ret) */
    l2data &= ~g_sleepTargetConfig.l2_mask;

    enMemRet = SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L2CLADE,   0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L2PLRU,    0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L2TAG,     0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L2STBUF,   0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1DU_TAG,  0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1DU,      0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, JU_TLB64,  0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, JU_TLB128, 0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, JU_CAM,    0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, ETB,       0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1IU_TAG,  0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1IU_PDEC, 0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1IU,      0x00) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1IU_BTB,  0x01);
  }
  else
  {
    /* Setup for cache related logic to be turned off/reset */
    l2data |= g_sleepTargetConfig.l2_mask;

    enMemRet = SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L2CLADE,   0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L2PLRU,    0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L2TAG,     0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L2STBUF,   0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1DU_TAG,  0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1DU,      0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, JU_TLB64,  0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, JU_TLB128, 0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, JU_CAM,    0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, ETB,       0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1IU_TAG,  0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1IU_PDEC, 0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1IU,      0x01) |
               SLEEP_HWIO_FVAL(RSCCTL_EN_MEM_RET, L1IU_BTB,  0x01);
  }

  SLEEP_HWIO_OUTF(RSCCTL_EN_L2MEM_RET, L2DATA, l2data);
  SLEEP_HWIO_OUT(RSCCTL_EN_MEM_RET, enMemRet);

  return;
}

/*
 * q6LPMConfig_setupTCMConfig
 */
void q6LPMConfig_setupTCMConfig(mem_state state)
{
  uint32 l2data = SLEEP_HWIO_INF(RSCCTL_EN_L2MEM_RET, L2DATA);

  if(MEM_RET == state)
  {
    /* Setup for cache related logic to be retained */
    l2data &= ~g_sleepTargetConfig.tcm_mask;
  }
  else
  {
    /* Setup for cache related logic to be turned off/reset */
    l2data |= g_sleepTargetConfig.tcm_mask;
  }

  SLEEP_HWIO_OUTF(RSCCTL_EN_L2MEM_RET, L2DATA, l2data);

  return;
}

/*
 * q6LPMConfig_setRpmhHandshake
 */
void q6LPMConfig_setRpmHandshake(boolean performHandshake)
{
  uint16  request   = (TRUE == performHandshake) ? 1 : 0;
  uint16  nRequest  = (TRUE == performHandshake) ? 0 : 1;

  /* Override the child -> parent handshake singnals */
  /* Using SET/CLEAR for QCTDD05239727 */
  if(request)
  {
    SLEEP_HWIO_OUT(RSCCTL_EN_PRSC_SET, (0x01 << SLEEP_HWIO_SHFT(RSCCTL_EN_PRSC, BRINGUP_REQ)));
    SLEEP_HWIO_OUT(RSCCTL_EN_PRSC_SET, (0x01 << SLEEP_HWIO_SHFT(RSCCTL_EN_PRSC, SHUTDOWN_REQ)));
  }
  else
  {
    SLEEP_HWIO_OUT(RSCCTL_EN_PRSC_CLEAR, (0x01 << SLEEP_HWIO_SHFT(RSCCTL_EN_PRSC, BRINGUP_REQ)));
    SLEEP_HWIO_OUT(RSCCTL_EN_PRSC_CLEAR, (0x01 << SLEEP_HWIO_SHFT(RSCCTL_EN_PRSC, SHUTDOWN_REQ)));
  }

  /* If not performing the handshake, we also need to override the ACK signal
   * in the sequence since it is generic */
  SLEEP_HWIO_OUTF(RSC_EVENT_PRSC_OVR, BRINGUP_ACK_OVRRD,  nRequest);
  SLEEP_HWIO_OUTF(RSC_EVENT_PRSC_OVR, SHUTDOWN_ACK_OVRRD, nRequest);

  return;
}

