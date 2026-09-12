#ifndef HDRSRCHRFDIV_H
#define HDRSRCHRFDIV_H
/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            H D R   S E A R C H   R F   D I V E R S I T Y
                   C O N T R O L   F U N C T I O N S

GENERAL DESCRIPTION

  This module provides routines for diversity RF control

EXTERNALIZED FUNCTIONS (Global)
  
EXTERNALIZED FUNCTIONS (Regional)

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2009 - 2015
                by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/inc/hdrsrchrfdiv.h#1 $
$DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when         who     what, where, why
--------     ---     --------------------------------------------------------- 
11/24/15     wsh     Added logic to handle DIV grant after TX in QTA
11/21/2015   arm     Resolved compilation error
11/26/14     sat     Added hdrsrchrfdiv_enable_diversity_chain API
08/28/14     arm     Changes to support QTA with diversity
07/29/14     tnp     FR22375: Fix Medium compiler warnings in HDR
08/31/12     wsh     Respect hard tuneaway rampdown
05/25/12     wsh     Moved ARD logic into it's own state machine 
04/02/12     arm     Supported diversity in reacq. 
03/09/12     arm     Disabled equalizer during second chain test call.  
02/08/12     rkc     Added ability to resume tuneaway via hdrsrchrfdiv_resume_tuneaway().
01/09/12     smd     Mainlined hdrsrchrfdiv_set_fl_data_adaptive_rxd_allowed().
11/29/11     arm     Removed compiler warnings.
10/25/11     lyl     Supported FL ARD enhancement.
10/10/11     arm     Supported second chain test call feature.
06/07/11     lyl     Added hdrsrchrfdiv_get_gpio_status().
04/27/11     lyl     Supported HDR FL data driven adaptive RxD. 
06/10/10     lyl     Added hdrsrchrfdiv_runtime_diversity_pref_ctrl().
02/04/10     ljl     Added hdrsrchrfdiv_start_stm().
05/13/09     grl     Created module

==========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"
#include "comdef.h"


/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*===========================================================================

                  COMMAND  FUNCTION  DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_CTRL

DESCRIPTION
  Call this function to enable or disable diversity.  It will be enabled
  whenever the RF resources are available and the primary chain is 
  enabled.  Otherwise, it will be be off.

DEPENDENCIES
  Assumes that external user diversity preference is pre-selected

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_diversity_ctrl 
( 
  boolean    div_flag 
    /* Whether to turn diversity on or off. */
);


/*===========================================================================

FUNCTION HDRSRCHRFDIV_USER_DIVERSITY_PREF

DESCRIPTION
  Sets diversity preference selected by the user

  This function is called during lmac powerup init

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_user_diversity_pref
( 
  uint8     div_ctrl,
    /* external user diversity preference */

  uint32    band_mask
    /* per band diversity enable/disable mask  */
);


/*=========================================================================

FUNCTION HDRSRCHRFDIV_DISABLE_DIV_RAMP_DOWN

DESCRIPTION 
  This function disables the RF module from performing ramp down
  activities related to disabling the diversity chain. The diversity
  chain can still be released. This function is called so that
  the RF module will not interfere with the HDR state machine's ramp down
  activities.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchrfdiv_disable_div_ramp_down( void );


/*=========================================================================

FUNCTION HDRSRCHRFDIV_ENABLE_DIV_RAMP_DOWN

DESCRIPTION 
  This function allows the RF module to perform ramp down
  activities related to disabling the diversity chain.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchrfdiv_enable_div_ramp_down( void );


/*===========================================================================

FUNCTION HDRSRCHRFDIV_CONSIDER_DIVERSITY_SWITCH

DESCRIPTION
  Consider switching diversity demod off based on the AGC values
  to improve overall throughput performance.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_consider_div_switch( void );


/*===========================================================================

FUNCTION HDRSRCHRFDIV_RESUME_TUNEAWAY

DESCRIPTION
  Resume tuneaway on diversity chain.  If diversity chain tuneaway timer
  was cleared before, this will run diversity release chain logic one time,
  which will start tuneaway timer if necessary.  If diversity chain tuneaway
  timer is already running, exercising diversity release chain logic an extra
  time will not hurt.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_resume_tuneaway( void );


/*===========================================================================

                     GET  RELATED  FUNCTION  DECLARATIONS

===========================================================================*/

/*=========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_COMB_ENABLED

DESCRIPTION 
  This function returns if diversity combining is
  enabled from an NV perspective. It may not actually be on at this
  point in time.

DEPENDENCIES
  None

RETURN VALUE
  True  - if diversity combining is enabled in NV.
  False - Otherwise. 
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchrfdiv_diversity_comb_enabled( void );


/*=========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_DEMOD_IS_ENABLED

DESCRIPTION 
  This function checks if diversity chain is enabled at the moment. 

DEPENDENCIES
  None

RETURN VALUE
  TRUE - If the diversity chain is enabled
  FALSE - otherwise.
  
SIDE EFFECTS
  None.

=========================================================================*/

boolean hdrsrchrfdiv_diversity_demod_is_enabled ( void );


/*===========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_IS_ENABLED

DESCRIPTION
  Returns the diversity status ON/OFF

DEPENDENCIES
  None

RETURN VALUE
  Boolean : Diversity Status ON or OFF

SIDE EFFECTS
  TBD

===========================================================================*/

boolean hdrsrchrfdiv_diversity_is_enabled( void );


/*=========================================================================

FUNCTION HDRSRCHRFDIV_RELEASE_DIV

DESCRIPTION 
  Send the command to diversity stm to release diversity.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchrfdiv_release_div( void );

/*=========================================================================

FUNCTION HDRSRCHRFDIV_START_STM

DESCRIPTION 
  Start div stm.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchrfdiv_start_stm( void );

/*===========================================================================

FUNCTION HDRSRCHRFDIV_RUNTIME_DIVERSITY_PREF_CTRL

DESCRIPTION
  Runtimely sets diversity preference by sending command to diversity stm.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_runtime_diversity_pref_ctrl
(
  hdrsrch_rx_div_enum_type        div_ctrl
    /* Diversity ctrl */
);

#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL
/*===========================================================================

FUNCTION HDRSRCHRFDIV_IS_SECOND_CHAIN_ONLY

DESCRIPTION
  If second chain test call is on or not.
DEPENDENCIES
  None

RETURN VALUE
  If second chain test call is on or not.

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchrfdiv_is_second_chain_only( void );

#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */

#ifdef FEATURE_HDR_SVDO_DIV_SUPPORT
/*===========================================================================

FUNCTION HDRSRCHRFDIV_GET_GPIO_STATUS

DESCRIPTION
  Get the diversity GPIO status.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  gpio status

SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrsrchrfdiv_get_gpio_status( void );

#endif /* FEATURE_HDR_SVDO_DIV_SUPPORT */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
/*===========================================================================

FUNCTION HDRSRCHRFDIV_ENABLE_DIV_IN_REACQ

DESCRIPTION
  Call this function to enable diversity in reacq state.
  It will enable diversity and start timer to make sure
  div is enabled by the time we have reduced sleep time by.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_enable_div_in_non_tc ( hdrsrch_non_tc_div_state_enum_t );
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
/*===========================================================================

FUNCTION HDRSRCHRFDIV_ARD_DIVERSITY_CTRL

DESCRIPTION
  Call this function to enable or disable diversity by ARD. 

DEPENDENCIES
  Must be called by ARD in its transition or state exit/entry function

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_ard_diversity_ctrl
(
  boolean    div_flag,
    /* 1: to enable diversity, 0 : disable diversity */
  boolean    forceful
    /* whether to respect current rampdown process */
);
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

/*============================================================================

FUNCTION HDRSRCHRFDIV_GRANT_CALLBACK

DESCRIPTION
  The trm rf grant callback function, used by the Transceiver manager
  to inform the client that the diversity receiver has been granted to HDR.
  
DEPENDENCIES
  The callback will be called by the Transceiver Manager.  It may be called
  from a task context of another client, or from interrupt context.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrchrfdiv_grant_callback
(
  trm_client_enum_t               client,
    /* The client which is being informed of an event */

  trm_grant_return_enum_type      grant,
    /* The grant being sent to the client */

  trm_request_tag_t               tag
    /* Identfying tag for this request/grant transaction */
);

#ifdef FEATURE_HDR_QTA
/*===========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_DISABLE_FOR_QTA

DESCRIPTION
  Call this function to disble diversity just before QTA start.
  This will set internal QTA flag so that we can skip RF disable for QTA. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_diversity_disable_for_qta( void );

/*===========================================================================

FUNCTION HDRSRCHRFDIV_DIVERSITY_DISABLE_AFTER_QTA

DESCRIPTION
  Call this function to disable diversity after QTA.
  This function is needed if we decide to not go to traffic after QTA.
  For example -: 4a->3a->3f, or 4a->4c

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrfdiv_diversity_disable_after_qta( void );

/*===========================================================================

FUNCTION HDRSRCHRFDIV_IS_DIVERSITY_IN_QTA

DESCRIPTION
  Call this function to check if diversity was enabled when QTA started. 

DEPENDENCIES
  None

RETURN VALUE
  TRUE: div was enabled before QTA
  FALSE: div was diabled before QTA

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchrfdiv_is_diversity_in_qta ( void );
#endif /* FEATURE_HDR_QTA */

#ifdef FEATURE_HDR_RUMI
/*===========================================================================

FUNCTION HDRSRCHRFDIV_ENABLE_DIVERSITY_CHAIN

DESCRIPTION
  Call this function to enable diversity RF chain. This is a wrapper function for 
  hdrsrchrfdiv_enable_div_chain()

DEPENDENCIES
  None

RETURN VALUE
 
SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrsrchrfdiv_enable_diversity_chain( void );
#endif /* FEATURE_HDR_RUMI */

/*=========================================================================

FUNCTION HDRSRCHRFDIV_CHK_AND_REDO_WARMUP

DESCRIPTION 
  Check if DIV is owned by not enabled. If so, requeue an redo_warmup_cmd.
  This is needed to take care of DIV being granted before PRI.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchrfdiv_chk_and_redo_warmup( void );
#endif
