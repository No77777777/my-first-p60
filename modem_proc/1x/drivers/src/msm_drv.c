
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             M O B I L E    D R I V E R    U T I L I T I E S

GENERAL DESCRIPTION
  A collection of utility functions for configuration of the mobile hardware
  and/or firmware to trigger variations in operation and behavior.

EXTERNALIZED FUNCTIONS
  msmdrv_enable_rl_power_control
  msmdrv_disable_rl_power_control

  Copyright (c) 2010 - 2013 Qualcomm Technologies, Inc. 
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/1x.mpss/8.0/drivers/src/msm_drv.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/18/11   adw     Mainline 1x message router support.
08/17/11   trc     Mainline RF SVDO API
08/05/11   vks     Fix compiler issues.
07/26/11   vks     Fix compiler warnings.
06/03/11   adw     SV support.
04/05/11   vks     Use new api for RF SVDO feature.
03/28/11   jtm     Removed deprecated RF includes.
01/28/11   vks     Featurize RF interface with FEATURE_MODEM_1X_SUPPORTS_RF.
01/25/11   vks     Update to new RF SVDO api for nikel
10/07/10   jtm     Initial revision.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//drivers
#include "1x_variation.h"
#include "msm_drv.h"
#include "msm_drv_i.h"

//feature dependent
#include "cdmafw_msg.h"
#include "muxmsgr.h"

#ifdef FEATURE_MODEM_1X_SUPPORTS_RF

#include "rfm_1x.h"

#include "rfm_device_types.h"
#include "txccommon_i.h"

#endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

/* Shadow copy of the registers */
volatile msm_drv_hwio_shadow_reg_type msm_drv_hwio_shadow_reg;

/*===========================================================================

FUNCTION MSMDRV_ENABLE_RL_POWER_CONTROL

DESCRIPTION
  Enable reverse link closed loop power control

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void msmdrv_enable_rl_power_control (void)
{
  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  if ( !cdma2kdiag_get_switch( CDMA2KDIAG_SW_DISABLE_RL_PWR_CTRL ) )
  {
    rfm_1x_set_tx_closed_loop_power( txc_get_tx_device(),
                                     RFM_1X_TX_CL_NORMAL, 0);
  }
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

} /* msmdrv_enable_rl_power_control */


/*===========================================================================

FUNCTION MSMDRV_ENABLE_RL_POWER_CONTROL

DESCRIPTION
  Enable reverse link closed loop power control

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void msmdrv_disable_rl_power_control (void)
{
  #ifdef FEATURE_MODEM_1X_SUPPORTS_RF
  /* The second parameter to this function is the clroverride flag.
     IF RPC is disabled, we want the clrOverride flag to be 0 so that
     FW doesn't resume RPC. If RPC is not disabled, then we want
     clrOverride to be 1 so that FW can resume RPC after this single
     override
     Since we are disabling RPC, we want the clrOverride to be 0
     always */
  rfm_1x_set_tx_closed_loop_power(
      txc_get_tx_device(),
      RFM_1X_TX_CL_OVERRIDE,
      ( (((int2)rfm_1x_get_tx_closed_loop(txc_get_tx_device())) >> 8) & 0xff ) );
  #endif /* FEATURE_MODEM_1X_SUPPORTS_RF */

} /* msmdrv_disable_rl_power_control */

