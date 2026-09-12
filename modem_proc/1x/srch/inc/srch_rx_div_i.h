#ifndef SRCH_RX_DIV_I_H
#define SRCH_RX_DIV_I_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             S R C H    R X   D I V  ---  H E A D E R   F I L E

GENERAL DESCRIPTION
  This module contains external header information for SRCH to use the RX DIV
  state machine.

EXTERNALIZED FUNCTIONS  (all SRCH regional)

  srch_rx_div_set_en_ovrd - change diversity settings at runtime.
  srch_rx_div_is_enabling - returns true if diversity is active or enabling.
  srch_rx_div_is_enabled - returns true if we're currently doing rx diversity.
  srch_rx_div_is_disabled - returns true if diversity is not in progress.
  srch_rx_div_allowed_for_bc - returns true if div is allowed for band class.
  srch_rx_div_set_ctrl  - sets RX diversity nv item
  srch_rx_div_is_capable - returns true if this phone supports rx diversity.
  srch_rx_div_get_algo - returns the diveristy combining algorithm type.
  srch_rx_div_access_is_capable - returns if capable of rx div in access.
  srch_rx_div_is_auto_enabled - returns true if rx diversity autoswitching is
                                enabled.
  srch_rx_div_is_off_with_no_ffpc - returns where diversity is disabled with
                                    no ffpc.
  srch_rx_div_is_bc_enable_mask_enabled - returns true if band class diversity
                                          switching is enabled.
  srch_rx_div_set_bc_enable_mask - sets the band class enable mask RX diversity
                                   nv item.
  srch_rx_div_is_bc_enable_mask_enabled - returns true if band class diversity
                                          switching is enabled.
  srch_rx_div_bc_enable_mask - returns the band class enable mask.
  srch_rx_div_request - requests that the SRCH_RX_DIV state machine
                        try and do diversity.
  srch_rx_div_stop - tells the SRCH_RX_DIV state machine to stop diversity.
  srch_rx_div_update_state - updates the rx diversity state depending on band
                             class and whether ffpc is enabled.
  srchrdiv_set_ftm_second_chain_test - sets the FTM test mode for using 2nd
                                       Rx only.
  srchrdiv_get_ftm_second_chain_test - returns true if the FTM 2nd chain test
                                       mode is set.
  srch_rx_div_enable_in_progress    -- returns TRUE if the div state machine
                                       is in the DIV_INITING or DIV_REQUESTED
                                       with DIV chain is granted

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

  Copyright (c) 2005 - 2016 Qualcomm Technologies, Inc.
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

$Header: //components/rel/1x.mpss/8.0/srch/inc/srch_rx_div_i.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/07/16   srk     Update ASDIV API name
03/11/16   ab      Donot reset ASDIV metrics for short TAs
01/21/16   srk     Changes to ACCESS and IDLE ASDIV.
12/08/15   jh      Added function to clear diversity flags
10/28/15   ab      Shifted the DRX set point decision logic to srch_mux.c
09/29/15   eye     DRX needs to be enabled/disabled based on set point
                   metrics of  DIV algorithm.
08/19/15   srk     Removed exporting unused functions.
04/21/15   srk     Added support for band specific ASDIV enable/disable for
                   all 1x states
11/25/14   srk     Export APIs to take and release control of the ASDIV switch.
10/10/14   srk     Cleanup ASDIV related code for DR-DSDS.
08/01/14   ab      Decouple INIT ASD thresholds
07/04/14   ab      Export IDLE ASD NV check function
05/30/14   srk     IDLE ASD fixes.
05/19/14   bph     Make callbacks registered with TRM asynchronous
03/25/14   bph     DSDS/DSDA Feature bring up traffic+traffic.
03/05/14   bph     DSDS/DSDA Feature bring up.
01/20/14   bph     IDLE AS-DIV support
11/08/13   srk     ASDIV Cleanup - Remove unneeded SRCH ASDIV signal handler.
11/07/13   srk     Export ASDIV band compatibility check API.
10/09/13   srk     Removed exporting unused functions.
05/13/13   srk     Add INIT AsDIV support
02/19/13   bb      Changes to avoid releasing RF without disabling diversity
01/11/12   bb      Changes to avoid releasing RF when diversity enable is
                   in progress
12/21/12   bb      Changes to avoid releasing the chain if diversity state
                   machine is posted enable command
12/17/12   vks     Add support for 1x access asdiv.
12/13/12   bb      Add flag/function to monitor request of diversity disable
12/10/12   vks     AsDiv: Do antenna switch only in 1x srch context
11/30/12   vks     Add support for 1x asdiv idle mode operation.
                   Add logging support
11/19/12   vks     Moved indicating start and stop of TX mode for AsDIV
                   algorithm into txccommon
11/09/12   vks     Add NV support for antenna selection diversity feature.
11/02/12   vks     Initial support for antenna selection diversity feature.
10/04/12   srk     Exported the definition of MIN_DIV_TIME_SCLK
05/15/12   srk     Export srch_rx_div_unlock_cb function.
01/24/12   sst     Implementation of RIN
01/13/12   srk     Feature Cleanup: Mainlined Always ON features
10/31/11   jtm     Feature clean up.
10/25/11   adw     Integrate TRAM support.
08/12/11   adw     Support second chain test [Option-2] capability on NikeL.
01/14/11   mca     Removed the immediately field from srch_rx_div_set_en_ovrd
11/01/10   jtm     Modem 1x SU API changes.
10/14/10   mca     Added support for diversity in access
06/08/10   adw     Updated RxDiv en/dis api comment block.
05/19/10   sst     Added new api command for runtime RxDiv en/dis
10/21/09   adw     Added a func to return true if div is active or enabling.
04/01/09   adw     Categorized included header files and standardized format.
03/31/09   adw     Eliminated implicit includes.
03/25/09   adw     Include modem_1x_defs.h for 1x specific modem defines.
02/10/09   adw     Merge from 2h08 branch, based on VU_MODEM_1X_SRCH.13.00.01
11/19/08   adw     Merge from main/latest, based on VU_MODEM_1X_SRCH.12.00.24
11/13/08   adw     Added external function srch_rx_div_allowed_for_bc().
05/01/07   tjc     Add bit to rx diversity nv item to enable/disable rx
                   diversity when FFPC is not available.
                   Add bit to rx diversity nv item to enabled/disable rx
                   diversity on a BC basis.
04/30/07   sst     Update RX Diversity featurization
02/13/07   tjc     Changed function name to match what it is doing
                   Start/Stop now enable/disable autodiversity
11/10/06   tjc     Add bit to rx diversity nv item to enable/disable
                   autoswitching
08/29/05   ejv     Remove featurization from *ftm_second_chain_test.
05/31/05   rng     Merged from SHDR Sandbox.
01/05/05   gs      Implementation, first cut.
===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

/* Srch */
#include "srch_rx_div.h"
#include "srch_rx_t.h"
#include "srch_rx_t_i.h"

/* Other */
#include "stm.h"


/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*-------------------------------------------------------------------------
      Constants
-------------------------------------------------------------------------*/
/* minimum useful time in diversity.  Currently 35 milli-seconds */
#define MIN_DIV_TIME_SCLK (timetick_cvt_to_sclk(35, T_MSEC))

/*-------------------------------------------------------------------------
      External Declarations
-------------------------------------------------------------------------*/
/* State Machine */
extern stm_state_machine_type DIV_SM;

/*-------------------------------------------------------------------------
      Data types
-------------------------------------------------------------------------*/

/* Diversity algorithm type */
typedef enum
{
  SRCH_RX_DIV_ALGO_MMSE,   /* MMSE combining algorithm */

  SRCH_RX_DIV_ALGO_SPW     /* Simple pilot weighting combining
                              algorithm (like MRC) */

} srch_rx_div_algo_enum_type;


/*===========================================================================

                    FUNCTION DECLARATIONS FOR MODULE

===========================================================================*/

/*===========================================================================

FUNCTION           srch_rx_div_unlock_cb

DESCRIPTION        This function is called by TRM when 1x is supposed to
                   give up the secondary chain.  This means we want to
                   stop diversity until the secondary chain is available
                   again, and thus send a "DIV_PAUSE" input to the diversity
                   state machine.

DEPENDENCIES       This function assumes that diversity is currently enabled.

RETURN VALUE       None

SIDE EFFECTS       None

===========================================================================*/
extern void srch_rx_div_unlock_cb
(
  trm_unlock_event_enum_t  event,         /* Event being sent to the client */
  uint32                   unlock_by_sclk /* Sclk timestamp for UNLOCK_BY*/
);

/*===========================================================================

FUNCTION       SRCH_RX_DIV_SET_EN_OVRD

DESCRIPTION    This function allows the caller to change the diversity
               capable settings during run time operation.
               It takes the following two inputs:
                 en_ovrd - if True this disables diversity for all future
                           traffic calls until this function is called again
                           with en_ovrd = FALSE
                 immediately - if True, the en_ovrd flag takes effect
                           immediately if a traffic call is in effect
               NOTE: This function does not override the settings of the
                     RxDiv NV item

DEPENDENCIES   None

RETURN VALUE   Returns whether RxDiv is capable

SIDE EFFECTS   None

===========================================================================*/
boolean srch_rx_div_set_en_ovrd
(
  boolean en_ovrd,
  boolean immediately
);

/*===========================================================================

FUNCTION       SRCH_RX_DIV_IS_ENABLING

DESCRIPTION    This function returns true if we're currently enabling or
               actively doing rx diversity.

DEPENDENCIES   None

RETURN VALUE   TRUE if the div state machine is in the DIV_REQUESTED,
               DIV_INITING, or DIV_ENABLED state and stop_requested is
               not true,  FALSE otherwise.

SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_rx_div_is_enabling( void );

/*===========================================================================

FUNCTION       SRCH_RX_DIV_IS_ENABLED

DESCRIPTION    This function returns true if we're currently doing rx
               diversity.

DEPENDENCIES   None

RETURN VALUE   TRUE if the div state machine is in the DIV_ENABLED state,
               FALSE otherwise

SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_rx_div_is_enabled( void );

/*===========================================================================

FUNCTION       SRCH_RX_DIV_IS_DISABLED

DESCRIPTION    This function checks to see if the RX diversity state machine
               is disabled.  Note that having the state machine enabled
               doesn't necessarily mean that RX diversity is in progress...
               we could be waiting for an RF lock or waiting for the hardware
               to warm up.

DEPENDENCIES   None

RETURN VALUE   TRUE if the RX DIVERSITY state machine is currently disabled.

SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_rx_div_is_disabled( void );

/*===========================================================================

FUNCTION SRCH_RX_DIV_ALLOWED_FOR_BC

DESCRIPTION   This function returns whether RX diversity is allowed depending
              on the band class that is passed in

DEPENDEDCIES  None.

RETURN VALUE  boolean - TRUE if diversity is allowed for the band class
                        FALSE otherwise

SIDE EFFECTS  None.

===========================================================================*/
extern boolean srch_rx_div_allowed_for_bc
(
  srch_rx_band_type  band
);

/*===========================================================================

FUNCTION        SRCH_RX_DIV_SET_CTRL

DESCRIPTION     This function sets RX diversity nv item

                Diversity CTRL NV Item details:
                -----------------------------------
                B0     = RX Diversity Enable/Disable
                B2:B1   = RX Diversity Combining Algorithm
                          00 : MMSE (not supported if QLIC is enabled)
                          01 : Simple Pilot Weights (default if QLIC is enabled)
                B3      = Reserved
                B4      = RX Diversity Autoswitching (RDDS) Enable/Disable
                B5     = RX Diversity Disabled/Enabled with no FFPC
                B6      = BC Diversity Switching Enable Mask Enable/Disable
                B7      = RX Diversity in Access Enable/Disable
                B8:9    = Reserved
                B10     = RX Diversity in IDLE Enable/Disable
                B11     = RX Diversity in INIT Enable/Disable
                B13:12  = Rx Diversity Search/Fing Combining Method
                          00 : Average
                          01 : Sum
                          10 : Max
                B14     = Rx Diversity Search Combining Enable/Disable
                B15     = QLIC Enable/Disable (1=Disable)
                          Note: this is not documented in srch_nv.h because it
                          is exported and would require support. For internal
                          use only!

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void srch_rx_div_set_ctrl
(
  uint16 rx_div_nv /* NV item which controls the Rx diversity */
);

/*===========================================================================

FUNCTION        SRCH_RX_DIV_IS_CAPABLE

DESCRIPTION     This function returns the rx diversity capability of the
                terminal

DEPENDENCIES    None

RETURN VALUE    TRUE    : Rx Diversity capable
                FALSE   : Rx Diversity incapabale

SIDE EFFECTS    None

===========================================================================*/
extern boolean srch_rx_div_is_capable( void );

/*===========================================================================

FUNCTION        SRCH_RX_DIV_GET_ALGO

DESCRIPTION     This function returns the diversity algorithm

DEPENDENCIES    None

RETURN VALUE    Algorithm type

SIDE EFFECTS    None.

===========================================================================*/
extern srch_rx_div_algo_enum_type srch_rx_div_get_algo( void );

/*===========================================================================

FUNCTION        SRCH_RX_DIV_ACCESS_IS_CAPABLE

DESCRIPTION     This function returns the rx diversity capability of the
                terminal during access

DEPENDENCIES    None

RETURN VALUE    TRUE    : Rx Diversity during access is allowed
                FALSE   : Rx Diversity during access is NOT allowed (default)

SIDE EFFECTS    None

===========================================================================*/
extern boolean srch_rx_div_access_is_capable( void );

/*===========================================================================

FUNCTION        SRCH_RX_DIV_IDLE_IS_CAPABLE

DESCRIPTION     This function returns the rx diversity capability of the
                terminal during idle

DEPENDENCIES    None

RETURN VALUE    TRUE    : Rx Diversity during idle is allowed
                FALSE   : Rx Diversity during idle is NOT allowed (default)

SIDE EFFECTS    None

===========================================================================*/
extern boolean srch_rx_div_idle_is_capable( void );

/*===========================================================================

FUNCTION        SRCH_RX_DIV_INIT_IS_CAPABLE

DESCRIPTION     This function returns the rx diversity capability of the
                terminal during init

DEPENDENCIES    None

RETURN VALUE    TRUE    : If RIN is enabled and Bit 0 of NV 67315 is set
                FALSE   : Otherwise

SIDE EFFECTS    None

===========================================================================*/
extern boolean srch_rx_div_init_is_capable( void );

/*===========================================================================

FUNCTION        SRCH_RX_DIV_IS_AUTO_ENABLED

DESCRIPTION     This function returns the whether rx diversity autoswitching
                is enabled on the terminal

DEPENDENCIES    None

RETURN VALUE    TRUE    : Rx Diversity Autoswitching is available
                FALSE   : Rx Diversity Autoswitching is not available

SIDE EFFECTS    None

===========================================================================*/
extern boolean srch_rx_div_is_auto_enabled( void );

/*===========================================================================

FUNCTION        SRCH_RX_DIV_IS_OFF_WITH_NO_FFPC

DESCRIPTION     This function returns whether rx diversity is enabled with just
                FPC.

DEPENDENCIES    None

RETURN VALUE    TRUE    : Rx Diversity is disabled with no FFPC
                FALSE   : Rx Diversity is enabled with no FFPC

SIDE EFFECTS    None

===========================================================================*/
extern boolean srch_rx_div_is_off_with_no_ffpc( void );

/*===========================================================================

FUNCTION        SRCH_RX_DIV_IS_BC_ENABLE_MASK_ENABLED

DESCRIPTION     This function returns whether the band class enable mask is
                enabled

DEPENDENCIES    None

RETURN VALUE    TRUE    : BC Diversity Switching Enable Mask enabled
                FALSE   : BC Diversity Switching Enable Mask disabled

SIDE EFFECTS    None

===========================================================================*/
boolean srch_rx_div_is_bc_enable_mask_enabled( void );

/*===========================================================================

FUNCTION        SRCH_RX_DIV_SET_BC_ENABLE_MASK

DESCRIPTION     This function sets RX diversity band class enabled mask nv item.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None

===========================================================================*/
extern void srch_rx_div_set_bc_enable_mask
(
  uint32 bc_en_mask_nv /* NV item which controls the band class enable mask */
);

/*===========================================================================

FUNCTION        SRCH_RX_DIV_BC_ENABLE_MASK

DESCRIPTION     This function returns the band class enable mask

DEPENDENCIES    None

RETURN VALUE    uint32

SIDE EFFECTS    None

===========================================================================*/
extern uint32 srch_rx_div_bc_enable_mask( void );

/*===========================================================================

FUNCTION SRCH_RX_DIV_REQUEST

DESCRIPTION   Requests diversity and enables the autoswitching algorithm if
              requested.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_request
(
  boolean auto_div   /* Whether autodiversity is requested */
);

/*===========================================================================

FUNCTION       SRCH_RX_DIV_STOP

DESCRIPTION    Stop diversity and disables the autoswitching algorithm.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_rx_div_stop( void );

/*===========================================================================

FUNCTION SRCH_RX_DIV_UPDATE_STATE

DESCRIPTION   This function updates the RX diversity state depending on the
              band class and whether ffpc is enabled.

DEPENDEDCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.

===========================================================================*/
extern void srch_rx_div_update_state
(
  srch_rx_band_type  band,
  boolean            fpc_enabled
);

/*===========================================================================

FUNCTION       SRCH_RX_DIV_DISABLE_REQUESTED

DESCRIPTION    This function returns if the diversity disable has been
               requested.

DEPENDENCIES   None.

RETURN VALUE   TRUE if diversity disable has been requested
               FALSE, otherwise.

SIDE EFFECTS   None.

===========================================================================*/
extern boolean srch_rx_div_disable_requested( void );

/*===========================================================================
FUNCTION       SRCH_RX_DIV_SET_DISABLE_COMPLETE

DESCRIPTION    This function sets that diversity disable has been completed

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_rx_div_set_disable_complete( void );

/*===========================================================================

FUNCTION       SRCH_RX_DIV_ENABLE_REQUESTED

DESCRIPTION    This function returns if the diversity enable has been
               requested.

DEPENDENCIES   None.

RETURN VALUE   TRUE if diversity enable has been requested
               FALSE, otherwise.

SIDE EFFECTS   None.

===========================================================================*/
extern boolean srch_rx_div_enable_requested( void );

/*===========================================================================

FUNCTION       SRCH_RX_DIV_CLEAR_FLAGS

DESCRIPTION    This function clears the released_chain, div_disable_req_cmd,
               and ignore_prio_update flags.

DEPENDENCIES   None.

RETURN VALUE   None

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_rx_div_clear_flags( void );

/*===========================================================================

FUNCTION       SRCH_RX_DIV_ENABLE_IN_PROGRESS

DESCRIPTION    This function returns true if diversity enable is in progress

DEPENDENCIES   None

RETURN VALUE   TRUE if the div state machine is in the DIV_INITING or
               DIV_REQUESTED with DIV chain is granted

SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_rx_div_enable_in_progress( void );

#ifdef FEATURE_MODEM_1X_SRCH_ASD
/*===========================================================================

FUNCTION       SRCH_RX_DIV_ASD_IDLE_IS_ENABLED

DESCRIPTION    The function returns a boolean indicating if IDLE ASD is enabled.

DEPENDENCIES   None.

RETURN VALUE   TRUE if IDLE ASD is enabled, otherwise FALSE.

SIDE EFFECTS   None.

===========================================================================*/
extern boolean srch_rx_div_asd_idle_is_enabled( void );

/*===========================================================================

FUNCTION       SRCH_RX_DIV_ASD_GET_AGC_THRES_GOOD

DESCRIPTION    The function gets AGC Good threshold which is used to make switch
               decision.

DEPENDENCIES   None.

RETURN VALUE   AGC Good threshold in dBm

SIDE EFFECTS   None.

===========================================================================*/
extern int32 srch_rx_div_asd_get_agc_thresh_good( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_GET_SAFE_RXAGC

DESCRIPTION   This function returns the safe RXAGC in dBm in traffic state

DEPENDENCIES  None

RETURN VALUE  Safe RXAGC in dBm

SIDE EFFECTS  None

===========================================================================*/
extern int32 srch_rx_div_asd_get_safe_rxagc( void );

#ifdef FEATURE_1X_TO_LTE
/*===========================================================================

FUNCTION       SRCH_RX_DIV_ASD_REQUEST_ANTENNA_SWITCH_LOCK

DESCRIPTION    This function is used to request a switch lock

DEPENDENCIES   None

RETURN VALUE   TRUE if successful, otherwise FALSE

SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_rx_div_asd_request_antenna_switch_lock( void );

/*===========================================================================

FUNCTION       SRCH_RX_DIV_ASD_REQUEST_SWITCH_UNLOCK

DESCRIPTION    This function is used to request a switch unlock

DEPENDENCIES   None

RETURN VALUE   TRUE if successful, otherwise FALSE

SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_rx_div_asd_request_antenna_switch_unlock( void );
#endif /* FEATURE_1X_TO_LTE */

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_TAKE_CTRL_OF_SWITCH

DESCRIPTION   This function requests to take control of the switch.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_take_ctrl_of_switch( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_RELEASE_CTRL_OF_SWITCH

DESCRIPTION   This function releases control of the switch.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_release_ctrl_of_switch( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_FRAME_EVENT

DESCRIPTION   This function is the heart of the ASD algorithm. It collects
              the sample data from FW SMEM and then checks if this needs to
              be consolidated into period data. Finally it checks if the
              switch decision needs to be made.
              If a switch is pending, it prevents from further collecting
              the data until the switch is completed

DEPENDENCIES  Expected to be called every frame

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_frame_event( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_ACCESS_FORCED_SWITCH_EVENT

DESCRIPTION   This function forces srch to do an ASDIV switch when ACCESS
              probes are started at swap config instead of pass through config.

DEPENDENCIES  Can be called only by TX task (MUX) when sending access probe

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_access_forced_switch_event( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_ACCESS_EVENT

DESCRIPTION   This function informs ASD when MUX is sending the access probe.
              The ASD algorithm requires the antenna switch position to be
              changed at the start of every new sequence after the first one

DEPENDENCIES  Can be called only by TX task (MUX) when sending access probe

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_access_event( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ENTER_INIT_MODE

DESCRIPTION   This function updates TRM about 1x initialization mode.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.

===========================================================================*/
extern void srch_rx_div_asd_enter_init_mode( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_INIT_EVENT

DESCRIPTION   This function does antenna switch during 1x Initialization.

DEPENDENCIES  None.

RETURN VALUE  Sucess or Failure.

SIDE EFFECTS  Antenna is switched.

===========================================================================*/
extern boolean srch_rx_div_asd_init_event( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_INFORM_DIV_EVENT

DESCRIPTION   This function keep track of the diversity enable and disable
              events. This is needed because the ASD algorithm needs to check
              what PCGs' data are worthy of reading from the FW shared memory.
              This is important for the cases of diversity being enabled/
              disabled as FW doesn't distinguish those cases when updating the
              shared memory with the rxagc information. So SW needs to keep a
              note of when the diversity was enabled/disabled and use that
              information to collect only valid samples

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_div_event( boolean enable );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_CAN_RDDS_DISABLE_DIV

DESCRIPTION   This function checks if RDDS can disable diversity. This is
              required because if the ASD algorithm is collecting the samples
              it wouldn't want diversity to be disabled. Once enough number
              of samples are collected, the flag is set to FALSE at which
              point, RDDS can go ahead and disable diversity

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern boolean srch_rx_div_asd_can_rdds_disable_div( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_STOP_TX_MODE

DESCRIPTION   This function is called when 1x is exiting out of TX mode. It
              informs common manager about 1x going into RX only mode (from
              RXTX mode) and also stops ASD algorithm

DEPENDENCIES  Can be called only by TX task when stopping TX

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_stop_tx_mode( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_START_TX_MODE

DESCRIPTION   This function is called when the ASD algorithm needs to be
              started for TX mode. It resets the traffic ASD algorithm
              data and informs common manager about 1x traffic mode

DEPENDENCIES  Can be called only by TX task when starting TX

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_start_tx_mode( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_RESET_METRICS

DESCRIPTION   This function resets ASD metrics

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_reset_metrics( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_IDLE_EVENT

DESCRIPTION   This function is called when 1x begins to use RX (after a RF
              tune) or is about to sleep. For the wakeup event, 1x informs
              TRM about 1x being in RX mode. For the sleep event, 1x checks
              the current RxAGC and if it is less than the threshold, 1x
              informs TRM about 1x wanting to be in 'enforced rx' mode. In
              this mode, TRM doesn't allow any other RAT to switch the antenna.
              If the RxAGC is good, 1x informs TRM about 1x going into sleep
              mode

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_idle_event
(
   boolean sleep,
   boolean change_config
);

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_ENTER_INACTIVE_MODE

DESCRIPTION   This function is called when 1x enters inactive mode. So 1x
              informs TRM about 1x going into inactive mode

DEPENDENCIES  Expected to be called on exiting 1x service

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_enter_inactive_mode( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_READ_NV_ITEM

DESCRIPTION   This function reads the EFS NV data related to ASD algorithm
               70261 - AsDIV Internal threshold NV item
               70262 - AsDIV External threshold NV item

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_read_nv_item( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_BUILD_ASDIV_SUB

DESCRIPTION   This function builds the ASDIV sub packet and should only be
              called by genlog

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_build_asdiv_sub
(
  srch_genlog_packet_id_type id,                 /* id of packet           */
  sub_commit_func_type       commit_fp           /* function to call to
                                                    commit the subpacket   */
);

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_SWITCH_ANTENNA

DESCRIPTION   This function does the necessary action to switch the antenna.
              The switch can be made only if diversity is enable and TX is on.
              It ensures, no searches are in progress, sends a message to FW to
              do the switch and restarts the searches

DEPENDENCIES  None

RETURN VALUE  TRUE:  Antenna Switch completed successfully
              FALSE: Otherwise

SIDE EFFECTS  None

===========================================================================*/
extern boolean srch_rx_div_asd_switch_antenna
(
  trm_ant_switch_div_config_type switch_pos    /* requested switch config */
);

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_RF_SET_ANT

DESCRIPTION   Update RF with desired config.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_rf_set_ant
(
  trm_ant_switch_div_config_type config      /* requested switch config */
);

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_CLEAR_TRAFFIC_DATA

DESCRIPTION   This function resets traffic related data for AsDIV algorithm

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_rx_div_asd_clear_traffic_data( void );

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_GET_INIT_SWITCH_AGC

DESCRIPTION   This function returns the AGC threshold for a Antenna
              Switch in INIT state.

DEPENDENCIES  None

RETURN VALUE  AGC threshold in dBM

SIDE EFFECTS  None

===========================================================================*/
extern int16 srch_rx_div_asd_get_init_switch_agc(void);

/*===========================================================================

FUNCTION      SRCH_RX_DIV_ASD_GET_INIT_SWITCHBACK_AGC

DESCRIPTION   This function returns the AGC threshold
              for switchback in INIT state.

DEPENDENCIES  None

RETURN VALUE  Switchback AGC threshold in dBM

SIDE EFFECTS  None

===========================================================================*/
extern int16 srch_rx_div_asd_get_init_switchback_agc(void);
#endif /* FEATURE_MODEM_1X_SRCH_ASD */
#endif /* SRCH_RX_DIV_I_H */
