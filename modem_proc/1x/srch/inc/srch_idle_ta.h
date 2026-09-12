#ifndef SRCH_IDLE_TA_H
#define SRCH_IDLE_TA_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             S E A R C H   I D L E   T U N E A W A Y
         H E A D E R F I L E

GENERAL DESCRIPTION
   This module handles the 1X tuneaway support for
   various technologies in IDLE state

  Copyright (c) 2015 Qualcomm Technologies, Inc.
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

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header:

when       who     what, where, why
--------   ---     ---------------------------------------------------------
07/27/16   pk      Changes to support RF script cleanup in data tech context
07/11/16   pk      Added API srch_idle_ta_is_processing_end_qta
12/10/15   sst     Modify some F3 messages for Reduction effort
11/13/15   pk      Changes to send reacq failure report to CP,
                   if OOS is triggered in QTA
09/04/15   pk      Don't do QBTA, when DR needs to be disabled
09/02/15   pk      Enable Rx DIV for QPCH online(PI2 bit), while in QTA
07/27/15   pk      Changes to ignore QTA strat cb, after wakeup cb from slpc
07/17/15   ab      Enable Split Ovhd For QTA
07/06/15   pk      Handling of protocol deactivation cmd in QTA timeline
06/29/15   ab      Read QTA NV at Bootup only
06/25/15   pk      Do LTA wakeup after reacquisition filure
06/18/15   ab      Removed diag cmd for QTA
05/20/15   ab      Removed the msgr header file
05/15/15   ab/pk   First Cut

===========================================================================*/

/*===========================================================================
        INCLUDE FILES FOR MODULE
===========================================================================*/
/* Common */
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

/* Other */
#include "slpc.h"
#include "stm.h"
#include "timer.h"
#include "wwan_coex_mgr.h"
#include "cdmafw_msg.h"

/*===========================================================================
          LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE
===========================================================================*/

/*-------------------------------------------------------------------------
      Constants
-------------------------------------------------------------------------*/

/* The Tuneaway gap of DATA Technologies */
#define DATATECH_CHAIN_RELEASE_TIME_BUFFER_MS  180

/* The RF chain release time buffer for QTA */
#define IDLE_QTA_UNLOCK_TIME_BUFFER_MS                          15
/*-------------------------------------------------------------------------
                        Exported Functions
-------------------------------------------------------------------------*/
/*===========================================================================
FUNCTION       IDLE_TA_READ_NV_ITEM

DESCRIPTION    This function reads the NV values for QTA Feature

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None
===========================================================================*/
extern void srch_idle_ta_read_nv_item( void );

/*===========================================================================
FUNCTION       SRCH_IDLE_TA_INIT

DESCRIPTION    Initialize srch_idle_ta.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
extern void srch_idle_ta_init(void);

/*===========================================================================
FUNCTION       SRCH_IDLE_TA_DEINIT

DESCRIPTION    De-Initialize srch_idle_ta.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
extern void srch_idle_ta_deinit(void);

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_IS_QTA_SUPPORTED

DESCRIPTION    This function checks if QTA is supported by NV

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None
==========================================================================*/
extern boolean  srch_idle_ta_is_qta_supported( void );

/*===========================================================================
FUNCTION       SRCH_IDLE_TA_START_QTA_TIMER

DESCRIPTION    This function starts a timer to end QTA. If the timer expires,
               due to forced sleep will move to LTA on next wakeup.

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
extern void srch_idle_ta_start_qta_timer( void );

/*===========================================================================
FUNCTION       SRCH_IDLE_TA_STOP_QTA_TIMER

DESCRIPTION    This function clears QTA timers, if any running.

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
extern void srch_idle_ta_stop_qta_timer( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_IS_FORCE_SLEEP_PENDING

DESCRIPTION    This function checks if there is pending force sleep.

DEPENDENCIES   NONE

RETURN VALUE   TRUE, if force sleep is pending; else FALSE.

SIDE EFFECTS   NONE
==========================================================================*/
extern boolean srch_idle_ta_is_force_sleep_pending( void );

/*===========================================================================
FUNCTION       SRCH_IDLE_TA_UPDATE_FORCE_SLEEP

DESCRIPTION    This function update idle_ta about pending force sleep from CP.

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   idle_ta structure gets updated
===========================================================================*/
extern void srch_idle_ta_update_force_sleep
(
  boolean update,
  boolean wakeup_in_qta
);

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_RESET_QTA_STATE

DESCRIPTION    This function resets QTA state variables

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   idle_ta structure gets updated
==========================================================================*/
extern void srch_idle_ta_reset_qta_state( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_UPDATE_QTA_STATE

DESCRIPTION    This function sets QTA is enabled now

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   idle_ta structure gets updated
==========================================================================*/
extern void srch_idle_ta_update_qta_state
(
  boolean qta_state
);

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_UPDATE_QTA_STATE

DESCRIPTION    This function sets reacq_failed

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   idle_ta structure gets updated
==========================================================================*/
extern void srch_idle_ta_set_reacq_failure( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_SET_PROTOCOL_DEACT

DESCRIPTION    This function sets protcol_deact field in QTA structure

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   idle_ta structure gets updated
==========================================================================*/
void srch_idle_ta_set_protocol_deact( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_IS_PROTOCOL_DEACT

DESCRIPTION    This function sets reacq_failed

DEPENDENCIES   NONE

RETURN VALUE   TRUE - If upper layer sent protocol deact cmd, else FALSE.

SIDE EFFECTS   None.
==========================================================================*/
boolean srch_idle_ta_is_protocol_deact( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_IS_QTA_ENABLED

DESCRIPTION    This function checks if QTA is enabled.

DEPENDENCIES   NONE

RETURN VALUE   TRUE if QTA is enabled, else FALSE.

SIDE EFFECTS   NONE
==========================================================================*/
extern boolean srch_idle_ta_is_qta_enabled( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_IS_RF_IN_QTA_MODE

DESCRIPTION    This function checks if RF has been put in QTA mode.

DEPENDENCIES   NONE

RETURN VALUE   TRUE if RF has been put in QTA mode, else FALSE.

SIDE EFFECTS   NONE
==========================================================================*/
extern boolean srch_idle_ta_is_rf_in_qta_mode( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_IS_RF_SCRIPT_BUILT

DESCRIPTION    This function checks if RF script has been built for QTA.

DEPENDENCIES   NONE

RETURN VALUE   TRUE if RF script is built, else FALSE.

SIDE EFFECTS   NONE
==========================================================================*/
extern boolean srch_idle_ta_is_rf_script_built( void );

/*==========================================================================

FUNCTION       SRCH_IDLE_TA_ENABLE_QBTA

DESCRIPTION    This function updates the QBTA flag.

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   idle_ta structure gets updated
==========================================================================*/
extern void srch_idle_ta_enable_qbta
(
  boolean enable_qta
);

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_IS_QBTA_ENABLED

DESCRIPTION    This function checks if QBTA is enabled

DEPENDENCIES   NONE

RETURN VALUE   TRUE if QBTA is enabled, else FALSE.

SIDE EFFECTS   NONE
==========================================================================*/
extern boolean srch_idle_ta_is_qbta_enabled( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_DISABLE_QBTA

DESCRIPTION    This function sets disable qbta flag in idle_ta str.

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   idle_ta structure gets updated
==========================================================================*/
extern void srch_idle_ta_disable_qbta( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_IS_QBTA_DISABLED

DESCRIPTION    This function checks if QBTA can be enabled

DEPENDENCIES   NONE

RETURN VALUE   TRUE if QBTA can be enabled, else FALSE.

SIDE EFFECTS   NONE
==========================================================================*/
extern boolean srch_idle_ta_is_qbta_disabled( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_REQUEST_NEXT_WAKEUP_IN_QTA

DESCRIPTION    This function checks, can we do next wakeup in QTA.

DEPENDENCIES   NONE

RETURN VALUE   TRUE, if 1x can do next wakeup in QTA, else FALSE.

SIDE EFFECTS   NONE
==========================================================================*/
extern boolean  srch_idle_ta_request_next_wakeup_in_qta
(
  boolean msg
);

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_BUILD_QTA_SCRIPT

DESCRIPTION    This function builds the RF script for TA

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
==========================================================================*/
extern void  srch_idle_ta_build_qta_script( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_EXIT_QTA

DESCRIPTION    This function does the cleanup for QTA exit

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
==========================================================================*/
extern void srch_idle_ta_exit_qta( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_CLEANUP_QTA_SCRIPT

DESCRIPTION    If RF is in QTA mode, this function calls meas exit API to
               bring RF out of QTA mode.

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
==========================================================================*/
void srch_idle_ta_cleanup_qta_script( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_SET_COEX_QTA_INACTIVE

DESCRIPTION    This function sets coex qta state to inactive

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
==========================================================================*/
extern void srch_idle_ta_set_coex_qta_inactive( void );

/*==========================================================================
FUNCTION       SRCH_IDLE_TA_IS_DIV_REQUIRED_IN_QTA

DESCRIPTION    This function checks, can we ask FW to enable diversity

DEPENDENCIES   API must be called, only for QTA wakeups

RETURN VALUE   TRUE, if Rx diversity can be enabled in QTA, else FALSE

SIDE EFFECTS   NONE
==========================================================================*/
extern boolean srch_idle_ta_is_div_required_in_qta( void );
#endif /* SRCH_IDLE_TA_H */