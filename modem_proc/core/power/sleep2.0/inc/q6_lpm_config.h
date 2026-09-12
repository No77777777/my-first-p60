#ifndef _Q6_LPM_CONFIG_
#define _Q6_LPM_CONFIG_
/*==============================================================================
  FILE:         q6_lpm_config.h

  OVERVIEW:     This file contains the APIs for configuring Q6 cpu to low 
                power modes. These APIs are may have Q6 version specific
                implementation. 

  DEPENDENCIES: None

                Copyright (c) 2016-2017 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/sleep2.0/inc/q6_lpm_config.h#3 $
$DateTime: 2020/04/18 04:01:28 $
==============================================================================*/
#include "comdef.h"
#include "sleep.h"
#include "rsc.h"

/*==============================================================================
 *                                GLOBAL TYPES
 *============================================================================*/
/* Used to enumerate lprm configuration for cacheLPR_l2ConfigureRSC */
typedef enum
{
  MEM_RET,       /* Config processor SS & MEM for ret           */
  MEM_NORET,     /* Config processor SS & MEM for non-ret       */
} mem_state;

/* qTimer frame mask bits to enable/disable wakeup interrupt capability*
 * bit settings:
 *    0 = interrupt wakeup masked
 *    1 = interrupt allows wakeup */
typedef enum
{
  WAKEUP_EN_RESERVED_0    = 0x01, /* Reserved bit 0              */
  WAKEUP_EN_RESERVED_1    = 0x02, /* Reserved bit 1              */
  WAKEUP_EN_QTMR_FRAME_0  = 0x04, /* QTimer Frame 0 (Kernel)     */
  WAKEUP_EN_QTMR_FRAME_1  = 0x08, /* QTimer Frame 1 (User/Timer) */
  WAKEUP_EN_QTMR_FRAME_2  = 0x10, /* QTimer Frame 2 (Sleep)      */

}qtimer_wake_int_masks;

/*==============================================================================
 *                             GLOBAL FUNCTIONS
 *============================================================================*/
/**
 * q6LPMConfig_setupBase
 *
 * @brief This function configures basic low power mode related Q6 registers 
 *        that remain same across all low power modes Q6 can be put in to.
 */
void q6LPMConfig_setupBase(void);

/**
 * q6LPMConfig_setupModeConfig
 *
 * @brief Performs Q6SS register configuration related to mode passed as
 *        input.
 *
 * @param coreMode  : Specifies the low power mode we want Q6 to put when
 *                    it goes idle next.
 * @param flag      : Boolean indicating a specific option based on the coreMode
 *                    For APCR modes:
 *                      Indicates PMI interrupt needs to be triggered upon rsc exit
 *                      from low power mode.  Currently it is required only for APCR low power
 *                      modes entered from single threaded context.
 *                    For Clockgate mode:
 *                      Indicates if the mode should be enabled or disabled.  When clockgate is
 *                      disabled, the previously configured mode will be effective
 *                    For other modes:
 *                      Currently has no meaning
 */
void q6LPMConfig_setupModeConfig(rsc_low_power_mode_type  coreMode,
                                 boolean                  flag);

/**
 * q6LPMConfig_setupL2Config
 *
 * @brief Performs Q6SS register configuration to configure the L2 cache for 
 *        retention or non-retention 
 *
 * @param state: Requested L2 memory state
 */
void q6LPMConfig_setupL2Config(mem_state state);

/**
 * q6LPMConfig_setupTCMConfig
 *
 * @brief Performs Q6SS register configuration to configure the TCM cache for 
 *        retention or non-retention 
 *
 * @note TCM may or may not be supported depnding on target (currently it's only used in MSS)
 *
 * @param state: Requested TCM memory state
 */
void q6LPMConfig_setupTCMConfig(mem_state state);

/**
 * q6LPMConfig_setupQ6PLLMode
 *
 * @brief Explicit configuration of the Q6 PLL state during power collapse
 *
 * @param off:  If TRUE, PLL will be off, else in freeze/standby
 */
void q6LPMConfig_setupQ6PLLMode(boolean off);


/**
 * q6LPMConfig_setupExtPLLControl
 *
 * @brief API to enable/disable Q6 control over external modem PLL
 *
 * @param q6_controls_extpll:  If TRUE, PQ6 will control external Modem PLL
                               If FALSE, Q6 wont have control over external Modem PLL
 */
void q6LPMConfig_setupExtPLLControl(boolean q6_controls_extpll);


/**
 * q6LPMConfig_setRpmhHandshake
 *
 * @brief Enables or disables the child - parent RSC handshake
 *
 * @param performHandshake: Requested RSC handshake state
 */
void q6LPMConfig_setRpmHandshake(boolean performHandshake);

#endif

