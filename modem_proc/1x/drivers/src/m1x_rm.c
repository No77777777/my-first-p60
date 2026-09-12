/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  M O D E M   1 X   R E S O U R C E   M G R

GENERAL DESCRIPTION
      This file contains functions for the modem 1x resource manager module.

EXTERNALIZED FUNCTIONS
      m1x_rm_activate
      m1x_rm_deactivate
      m1x_rm_set_state
      m1x_rm_get_state
      m1x_rm_query_capability
      m1x_rm_update_params
      m1x_rm_config_chain
      m1x_rm_preserve_cdma_timing

INTERNAL FUNCTIONS
      Resource config, invokes proper configuration method:
        rm_update_state_config
          rm_config_mcpm
           - Support for new interfaces should be added here.

      Chain config, invokes the proper chain config method:
        rm_update_chain_config
          rm_config_chain_xxx
           - Support for new interfaces should be added here.

      Callback registeration:
        rm_register_event_cb

INITIALIZATION AND SEQUENCING REQUIREMENTS
      Must call m1x_rm_activate() before any other functions in this module
      are invoked.

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Copyright (c) 2011 - 2020 Qualcomm Technologies, Inc.
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

$Header: //components/rel/1x.mpss/8.0/drivers/src/m1x_rm.c#7 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/25/20   pc      Fixed KW error
02/07/20   dmd     FR54762: 1x changes for concurrency Manager
02/06/20   dmd     Add new scenarios for finger enablement
02/03/20   dmd     Remove VPE or VECTOR MCVS param update
01/22/20   vaa     Implement QSH MDUMP framework
09/15/19   ppr     FR47558 Clock Plan 2.0 Changes
05/24/16   bb      Explictly have MCVS vote for WTR3925 enabled cases
11/05/15   eye     Added JACALA specific MCVS clk settings.
08/16/15   srk     Revert Jolokia related changes.
08/14/15   srk     Use HIGH VPE clocks for initial acquisition in ATLAS.
06/16/15   eye     1x MCVS changes for TA.
04/21/15   srk     DRDSDS and BOLT REVISIT Feature cleanup
03/06/15   sst     Move string prints inside check for msg flag
02/16/15   ab      Use NOM frequency on Jolokia in Traffic state only when
                   number of fingers are > 6.
12/22/14   bb      Changes to use right Q6 silver frequency for JOLOKIA
09/05/14   srk     Change the Jolokia revisit feature logic to make it positive
08/25/14   as      Backout work around for increasing VPE clock speeds for
                   IDLE/WAKEUP states
08/01/14   as      Update MCVS request for JOLOKIA
06/12/14   bb      Work around changes to increase VPE clcok speed to MED
                   for IDLE/WAKEUP states
04/17/14   cjb     Clean-up related to using 1x MSG macros for printing F3s
03/18/14   vks     Ensure MCVS request is not voting for clocks lower than the
                   base clock request for that state
03/11/14   vks     Add MCVS request as part of MCPM voice and data requests
03/10/14   vks     Modify 1x mcvs q6 clock request table
03/10/14   vks     Start out with requesting high MCVS Q6 for voice and data
02/12/14   vks     1x mcvs changes for bolt
12/18/13   as      Bolt Revisit MCVS requests
09/27/13   sst     Remove "Low" votes, and instead just issue a release
09/30/13   cjb     Fixed protocol shutdown with force enable all clocks
08/30/13   sst     Add two new Voice levels
08/28/13   pap     Mainlining features FEATURE_MODEM_1X_SUPPORTS_MCVS.
08/28/13   cjb     Mainline FEATURE_MODEM_1X_DIME, FEATURE_MODEM_1X_DIME_REVISIT
08/23/13   vks     srch_util_dmod_powerup_reset and srch_util_dmod_reset
                   obsolete on bolt
06/27/13   bb      Changes to shutdown MDSP FW while exiting from system
                   measurement state
08/06/13   cjb     Removed configuring DEMBACK_EN register
05/10/13   cjb     Updated fix for FW shutdown during protocol deactivation.
05/09/13   cjb     Pseudo sleep is not supported by MCPM anymore. 1x code hooks
                   left behind to support it, if the need arises in future.
04/02/13   pap     Feature cleanup
03/02/13   cjb     Mainline SLPC implementation.
02/28/13   cjb     Fixed previous checkin. DEMOD reset should be done only
                   while moving to a higher m1x_rm state.
02/26/13   vks     Move setting of dem1x registers outside of mcpm callback
02/15/13   srk     Replace mcs hwio with m1x hwio
02/08/13   dkb     Add cb for MCPM_DEMFRONT_BLOCK
02/07/13   cjb     Rollback previous checkin: clk_bus restored to MCPM config.
01/17/13   trc     Add clk_bus bump to MCVS request
01/03/13   trc     Add MCVS support, remove dead DSDS code
11/13/12   trc     Layering/Cleanup for Triton
11/08/12   cjb     Making previous change backward compatible with Triton.
11/07/12   cjb     Switched to new FW interface which takes care of configuring
                   DEMBACK_MODE_SEL to proper values based on the active TECHs.
11/06/12   srk     Reverse logic for printing strings in srch.
10/22/12   cjb     Temporary solution for DEMBACK_MODE_SEL: Set to CDMA+DO mode.
10/18/12   sst     Use a message level to trigger messages that contain strings
                   Lower some message levels
10/09/12   jj      Changes to remove preserve timing using clk interface..
09/12/12   cjb     Support enabling "forcing all clocks on" even after boot-up.
08/14/12   cjb     Added changes to use SLPC(new sleepctl replacement module).
04/18/12   adw     Promote all "voice" reqs to "advRx" to enable HIGH FW clk.
03/08/12   adw     Add support for 1x voice advanced Rx settings.
01/26/12   vks     Update mcpm parameters only when already in SLEEP state
10/24/11   cjb     Changes for IRAT:1x->LTE. Initial working version.
10/14/11   adw     Added support for registering event callbacks.
10/11/11   adw     Remove obsolete clkrgm references.
09/02/11   sk      Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
08/03/11   adw     Support for Lto1x and 1xtoL IRAT requests.
07/18/11   adw     Added parameter update support.
06/23/11   adw     Added missing include.
06/16/11   adw     Remove change allowing redundant sleep requests.
06/15/11   adw     Move dmod reset to demss block enable cb for NikeL.
06/13/11   adw     Remove temporary force enable and misc cleanup.
06/08/11   adw     Remove workaround to handle "inactive" request when shutting
                   down from sleep as MCPM now supports this transition.
06/03/11   adw     Initialize dem1x regs after coming out of power collapse.
05/25/11   adw     Temporarily force enable until Tx workaround is viable.
05/12/11   adw     Correct redundant config check.
05/10/11   adw     Notify MCPM of next 1x wakeup time.
04/28/11   adw     Handle "inactive" request when shutting down from sleep.
04/28/11   adw     Cleanup misc compile warnings.
04/27/11   adw     Support pseudo sleep mode.
04/26/11   adw     Powerup force enable debug support.
04/26/11   adw     Added initial modem block restore cb support.
03/03/11   adw     Changes needed to properly handle mcpm state transitions.
02/24/11   vks     Temporary changes for Nikel RUMI bringup.
02/10/11   adw     Updates to MCPM requests.
01/27/11   adw     Arbitrate when configuring chains to ensure proper settings.
                   Enable/disable TXC clk in required states.
01/24/11   adw     Handle invalid initialization cases.
01/24/11   adw     Cleaned up compiler warnings.
01/21/11   adw     Initial revision.

===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"
#include "m1x_hwio_mpss.h"

/* Srch */
#include "srch.h"
#include "srch_diag.h"
#include "srch_i.h"
#include "srch_pwr.h"
#include "srchi_i.h"
#include "srch_rx.h"
#include "trm.h"

/* Other */
#include "m1x_rm.h"
#include "m1x_time_i.h"
#include "mcpm_api.h"
#include "modem_mem.h"
#include "m1x_diag.h"
#include "rex.h"
#include "slpc.h"
#include "cmgr_api.h"

/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*-------------------------------------------------------------------------
      Constants and Macros
--------------------------------------------------------------------------*/
#ifdef FEATURE_MODEM_1X_JOLOKIA_REVISIT
#ifdef FEATURE_MODEM_1X_TA_MCVS
#ifdef FEATURE_MODEM_1X_JACALA_MCVS
/* MCVS Q6 floor request frequencies in KHz */
static const uint32 rm_q6_freq_req[ M1X_RM_MAX_Q6_REQ ] =
{
  326400,                  /* M1X_RM_Q6_REQ_NOM,  ( LOW SVS ) */
  480000,                  /* M1X_RM_Q6_REQ_MED,  ( SVS ) */
  691200                   /* M1X_RM_Q6_REQ_HIGH, ( NOM ) */
};

/* MCVS VPE floor request frequencies in KHz */
static const uint32 rm_vpe_freq_req[ M1X_RM_MAX_VPE_REQ ] =
{
  288000,                  /* M1X_RM_VPE_REQ_NOM,  ( LOW SVS ) */
  460800,                  /* M1X_RM_VPE_REQ_MED,  ( SVS ) */
  652800                   /* M1X_RM_VPE_REQ_HIGH, ( NOM ) */
};
#else  /* FEATURE_MODEM_1X_JACALA_MCVS */
/* MCVS Q6 floor request frequencies in KHz */
static const uint32 rm_q6_freq_req[ M1X_RM_MAX_Q6_REQ ] =
{
  230400,                  /* M1X_RM_Q6_REQ_NOM,  ( LOW SVS ) */
  384000,                  /* M1X_RM_Q6_REQ_MED,  ( SVS ) */
  576000                   /* M1X_RM_Q6_REQ_HIGH, ( NOM ) */
};

/* MCVS VPE floor request frequencies in KHz */
static const uint32 rm_vpe_freq_req[ M1X_RM_MAX_VPE_REQ ] =
{
  211200,                  /* M1X_RM_VPE_REQ_NOM,  ( LOW SVS ) */
  364800,                  /* M1X_RM_VPE_REQ_MED,  ( SVS ) */
  556800                   /* M1X_RM_VPE_REQ_HIGH, ( NOM ) */
};
#endif /* FEATURE_MODEM_1X_JACALA_MCVS */
#else /* FEATURE_MODEM_1X_TA_MCVS */
/* REVISIT - 384MHz across the board for now. Ask Vijay what these should be */

/* MCVS Q6 floor request frequencies in KHz */
static const uint32 rm_q6_freq_req[ M1X_RM_MAX_Q6_REQ ] =
{
  288000,                  /* M1X_RM_Q6_REQ_NOM, Uses MCPM default floor */
  384000,                  /* M1X_RM_Q6_REQ_MED */
  576000                   /* M1X_RM_Q6_REQ_HIGH */
};

/* MCVS VPE floor request frequencies in KHz */
static const uint32 rm_vpe_freq_req[ M1X_RM_MAX_VPE_REQ ] =
{
  288000,                  /* M1X_RM_VPE_REQ_NOM, Uses MCPM default floor */
  384000,                  /* M1X_RM_VPE_REQ_MED */
  576000                   /* M1X_RM_VPE_REQ_HIGH */
};
#endif /* FEATURE_MODEM_1X_TA_MCVS */
#else /* FEATURE_MODEM_1X_JOLOKIA_REVISIT */
/* MCVS Q6 floor request frequencies in KHz */
static const uint32 rm_q6_freq_req[ M1X_RM_MAX_Q6_REQ ] =
{
  230400,                  /* M1X_RM_Q6_REQ_NOM, Uses MCPM default floor */
  288000,                  /* M1X_RM_Q6_REQ_MED */
  307200                   /* M1X_RM_Q6_REQ_HIGH */
};

/* MCVS VPE floor request frequencies in KHz */
static const uint32 rm_vpe_freq_req[ M1X_RM_MAX_VPE_REQ ] =
{
  285000,                  /* M1X_RM_VPE_REQ_NOM, Uses MCPM default floor */
  499000,                  /* M1X_RM_VPE_REQ_MED */
  666000                   /* M1X_RM_VPE_REQ_HIGH */
};
#endif /* FEATURE_MODEM_1X_JOLOKIA_REVISIT */

/* Search power message macros */
#define RM_MSG( lvl, ... )                                               \
          M1X_MSG( MUX, lvl, __VA_ARGS__ )

#define RM_STR( lvl, ... )                                               \
          M1X_STR( MUX, lvl, __VA_ARGS__ )

/* Message levels */
#define LOW        LEGACY_LOW
#define MED        LEGACY_MED
#define HIGH       LEGACY_HIGH
#define ERROR      LEGACY_ERROR
#define FATAL      LEGACY_FATAL

/* Macro to ensure pointer is non-NULL. Will error fatal otherwise. */
#define M1X_RM_NULL_PTR_CHK( ptr )                                       \
  if ( ptr == NULL )                                                     \
  {                                                                      \
    ONEX_ERR_FATAL( "Attempt to access unallocated memory", 0, 0, 0 );        \
  }

/* Mutual exclusion between two normal tasks */
#define M1X_RM_ENTER_CRIT_SECT()                        \
          rex_enter_crit_sect( &m1x_rm_crit_sect )

#define M1X_RM_LEAVE_CRIT_SECT()                        \
          rex_leave_crit_sect( &m1x_rm_crit_sect )

/* Bitmask value type for assigning each npa node client a unique bit.
   The number of clients must never exceed the number of bits in the bitmask. */
typedef uint16  rm_bitmask_type;

/* Values used for clearing and setting bitmasks */
#define RM_BITMASK_CLEAR              0x0
#define RM_BITMASK_SET                0x1

/* Bitmask corresponding to the entire set. This value must be the size
   of the rm_bitmask_type. */
#define RM_BITMASK_ALL                0xffff

/* Bitmask corresponding to an empty set */
#define RM_BITMASK_NONE               0x0

/* Generates a bitmask based on an enum */
#define RM_ENUM_TO_BITMASK( index )  ( RM_BITMASK_SET << index )

/* Strings corresponding to each client of the modem 1x resource mgr module.
   This array must correspond to the m1x_rm_client_type. */
static const char  rm_client_str[ M1X_RM_MAX_CLIENTS ][ 7 ] =
{
  "srch_0",                  /* M1X_RM_SRCH */
  "srch_1",                  /* M1X_RM_SRCH_SECONDARY */
  "rx",                      /* M1X_RM_RX */
  "tx",                      /* M1X_RM_TX */
  "m1x"                      /* M1X_RM_INTERNAL */
};

/* Strings corresponding to each resource request state. This array must
   correspond to the m1x_rm_state_type. */
static const char  rm_state_str[ M1X_RM_MAX_STATES ][ 8 ] =
{
  "na",                      /* M1X_RM_1X_INACTIVE */
  "sleep",                   /* M1X_RM_1X_SLEEP */
  "doze",                    /* M1X_RM_1X_PSEUDO_SLEEP */
  "wakeup",                  /* M1X_RM_1X_WAKEUP */
  "Lto1x",                   /* M1X_RM_1X_MEAS_LTO1X */
  "idle",                    /* M1X_RM_1X_IDLE */
  "voice",                   /* M1X_RM_1X_VOICE */
  "data",                    /* M1X_RM_1X_DATA */
  "init",                    /* M1X_RM_1X_INIT */
  "1xtoL",                   /* M1X_RM_1X_MEAS_1XTOL */
  "all"                      /* M1X_RM_1X_ALL */
};

/* Strings corresponding to each chain type. This array must correspond
   to the m1x_rm_chain_type. */
static const char  rm_chain_str[ M1X_RM_MAX_CHAIN_CONFIGS ][ 8 ] =
{
  "none",                    /* M1X_RM_RX_CHAIN_NONE */
  "chain_0",                 /* M1X_RM_RX_CHAIN_0 */
  "chain_1",                 /* M1X_RM_RX_CHAIN_1 */
  "both"                     /* M1X_RM_RX_CHAIN_BOTH */
};

/* Strings corresponding to each capability type. This array must correspond
   to the m1x_rm_capability_type. */
static const char  rm_capability_str[ M1X_RM_MAX_CAPABILITIES ][ 6 ] =
{
  "demod",                   /* M1X_RM_DEMOD_CAPABLE */
  "vdec",                    /* M1X_RM_VD_CAPABLE */
  "tx",                      /* M1X_RM_TX_CAPABLE */
  "tdec"                     /* M1X_RM_TD_CAPABLE */
};


/*-------------------------------------------------------------------------
      Typedefs
-------------------------------------------------------------------------*/
/* Client data structure */
typedef struct
{
  /* Last resource state requested by the client */
  m1x_rm_state_type  requested_state;

  /* Lat chain config requested by the client */
  m1x_rm_chain_type  requested_chain;
}
rm_client_type;


/*-------------------------------------------------------------------------
 Modem 1x Resource Manager main data type
-------------------------------------------------------------------------*/
typedef struct
{
  /* Initialization flag */
  boolean  initialized;

  /* Array of all clients */
  rm_client_type  clients[ M1X_RM_MAX_CLIENTS ];

  /* Currently configured resource request state */
  m1x_rm_state_type  current_state;

  /* Currently set chain configuration */
  m1x_rm_chain_type  current_chain;

   /* Attributes associated with a given mcpm request */
  mcpm_request_parms_type  mcpm_parms;

  /* Flag to track if an mcvs request is outstanding */
  boolean is_mcvs_req_outstanding;

  /* Current MCVS request parameters */
  m1x_rm_mcvs_req_type mcvs_parms;

  m1x_rm_mcvs_req_type mcvs_state_parms[ M1X_RM_MAX_STATES ];

  boolean onex_received_cmgr_cb;

}
m1x_rm_type;


/*-------------------------------------------------------------------------
      Variables
-------------------------------------------------------------------------*/
/* Global lock for module critical sections */
static rex_crit_sect_type  m1x_rm_crit_sect;

/* Pointer to the data structure for the m1x_rm module. Module memory
   is dynamically allocated during init. */
static m1x_rm_type  *m1x_rm;

/* Debug flag used to force enable all resources. Setting this flag will
   result in the request state being set to "all", with subsequent requests
   having no effect. */
static volatile boolean m1x_rm_force_enable_all = FALSE;


/*=============================================================================

                      FUNCTION DEFINITIONS FOR MODULE

=============================================================================*/

/*-----------------------------------------------------------------------------
      Internal Functions
-----------------------------------------------------------------------------*/

/*=============================================================================

FUNCTION       RM_UPDATE_MCVS_PARAMS
DESCRIPTION    Update the mcvs parameters as requested.

DEPENDENCIES   The m1x_rm_activate() routine must have been called previously.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
static void rm_update_mcvs_params
(
  m1x_rm_mcvs_req_type *mcvs_req_ptr,    /* MCVS requests */
  onex_m1x_rm_scenario_type onex_scenario /* ONEX Scenario Type */
)
{
  /* MCPM Scenario Details */
  onex_m1x_rm_mcpm_scenario_type onex_mcpm_scenario = 
                             m1x_rm_get_mcpm_scenario( onex_scenario );  
  
  uint32 mcpm_scenario = 0; 
         
  mcpm_mcvsrequest_parms_type mcvs_parms = { 0 };

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  M1X_RM_NULL_PTR_CHK( mcvs_req_ptr );

  /* Validate the mcpm scenario */
  if( onex_mcpm_scenario >= ONEX_SCENARIO_MAX )
  {
    ONEX_ERR_FATAL( "Unexpected onex_scenario:%d", 
               onex_mcpm_scenario, 
               0, 
               0 );
  }

  mcvs_req_ptr->q6_freq =
    MAX( m1x_rm->mcvs_state_parms[m1x_rm->current_state].q6_freq,
         mcvs_req_ptr->q6_freq );

  mcvs_req_ptr->vpe_freq =
    MAX( m1x_rm->mcvs_state_parms[m1x_rm->current_state].vpe_freq,
         mcvs_req_ptr->vpe_freq );

  /* Error check for Q6 max request */
  if( mcvs_req_ptr->q6_freq > M1X_RM_Q6_REQ_HIGH )
  {
    mcvs_req_ptr->q6_freq = M1X_RM_Q6_REQ_HIGH;
  }

  /* Error check for vpe max request */
  if (mcvs_req_ptr->vpe_freq > M1X_RM_VPE_REQ_HIGH )
  {
    mcvs_req_ptr->vpe_freq = M1X_RM_VPE_REQ_HIGH;
  }

  /* Request the MCVS bump to q6 floor freq */
  mcvs_parms.mcvs_req_type    = MCVS_FULL_REQUEST;
  #ifdef FEATURE_MODEM_1X_JOLOKIA_REVISIT
  /* Still work to be done here.  Use the correct fields once
     MCPM have provided them */
  mcvs_parms.mcvs_update_info = MCPM_MCVS_SCALAR_PROC_CLK_UPDATE;
  #else
  mcvs_parms.mcvs_update_info = MCPM_MCVS_Q6_CLK_UPDATE;
  #endif /* FEATURE_MODEM_1X_JOLOKIA_REVISIT */

if( onex_mcpm_scenario != ONEX_SCENARIO_NONE )
  {
    /* Update MCPM about Scenario Info */
    mcvs_parms.mcvs_update_info |= MCPM_MCVS_SCENARIO_INFO_UPDATE;

    /* Mcpm uses 32 bit uint to extract scenario info as this: 
     * LSB 8 bits (7:0)     - Internal to mcpm
     * 1 bit (8)            - Scenario START/STOP
     * USB 23 bits(31:9)    - Scenario ID (range 0- 2^23) */
    mcpm_scenario = ( (onex_mcpm_scenario << 1) | TRUE ) << 8;

    /* Set MCPM Start Requsest Status */
    m1x_rm_set_mcpm_req_status(onex_mcpm_scenario, TRUE);
  }
  mcvs_parms.scenario_info = &mcpm_scenario;
  mcvs_parms.q6_clock_KHz     = rm_q6_freq_req[ mcvs_req_ptr->q6_freq ];
  #ifdef FEATURE_RF_HAS_WTR3925
  if ( ( mcvs_req_ptr->q6_freq == M1X_RM_Q6_REQ_NOM ) &&
       (IS_TRAFFIC_STATE(srch_state) ) )
  {
    mcvs_parms.q6_clock_KHz = 326000;
  }
  #endif /* FEATURE_RF_HAS_WTR3925 */

  mcvs_parms.modem_vpe_KHz    = rm_vpe_freq_req[ mcvs_req_ptr->vpe_freq ];
  MCPM_MCVSConfig_Modem( MCPM_1X_TECH, &mcvs_parms );

  /* Set our bookkeeping flag that MCVS bump request is outstanding */
  m1x_rm->is_mcvs_req_outstanding = TRUE;

  RM_MSG( HIGH, 
          "1x MCVS: Cur_State:%d, Scenario:%u, Start:%u, "
          "Q6 clk to:%u KHz, VPE clk to:%u KHz, MCPM_Start_Req_Status:%d",
          m1x_rm->current_state,
          ( (*mcvs_parms.scenario_info) >> 9),
          ( ( (*mcvs_parms.scenario_info) >> 8) & 0x1),
          mcvs_parms.q6_clock_KHz,
          mcvs_parms.modem_vpe_KHz,
          m1x_rm_get_mcpm_req_status(onex_mcpm_scenario) );

  /* Update the local structure with the new request */
  m1x_rm->mcvs_parms = *mcvs_req_ptr;

} /* rm_update_mcpm_mcvs_params */

/*=============================================================================

FUNCTION       RM_UPDATE_MCPM_PARAMS
DESCRIPTION    Update the parameters associated with the state being requested.

DEPENDENCIES   The m1x_rm_activate() routine must have been called previously.

RETURN VALUE   Pointer to the updated params data structure.
               NULL if there are no param updates associated with a state.

SIDE EFFECTS   None.

=============================================================================*/
static mcpm_request_parms_type* rm_update_mcpm_params
(
  m1x_rm_state_type  state    /* state being requested */
)
{
  uint64 wakeup_time = 0; /* time of wakeup interrupt in micro seconds */
  mcpm_request_parms_type* param_ptr; /* parameters pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Clear the mcpm params data structure */
  memset( &(m1x_rm->mcpm_parms), 0, sizeof( m1x_rm->mcpm_parms ) );

  switch( state )
  {
    case M1X_RM_1X_SLEEP:
      if ( m1x_rm->current_state == M1X_RM_1X_SLEEP )
      {
        /* get the next wakeup timetick value (and convert to microseconds) */
        wakeup_time = m1x_timetick_conv_from_ticks(
                        slpc_get_wakeup_tick( SLPC_1X ),
                        T_USEC );

        /* update mcpm params */
        m1x_rm->mcpm_parms.wkup_time_for_rf = wakeup_time;
        m1x_rm->mcpm_parms.wkup_time        = wakeup_time;
        m1x_rm->mcpm_parms.update_info      = MCPM_WKTIME_RF_UPDATE |
                                              MCPM_WKTIME_RSRC_UPDATE;
      }

      param_ptr = &(m1x_rm->mcpm_parms);
      break;

    case M1X_RM_1X_MEAS_LTO1X:
      /* update mcpm params */
      m1x_rm->mcpm_parms.neighbor_meas    = MCPM_L21XDO_MEAS_START;
      m1x_rm->mcpm_parms.update_info      = MCPM_NEIGHBOR_MEAS_UPDATE;

      param_ptr = &(m1x_rm->mcpm_parms);
      break;

    case M1X_RM_1X_MEAS_1XTOL:
      /* update mcpm params */
      m1x_rm->mcpm_parms.neighbor_meas    = MCPM_1X2L_MEAS_START;
      m1x_rm->mcpm_parms.update_info      = MCPM_NEIGHBOR_MEAS_UPDATE;

      param_ptr = &(m1x_rm->mcpm_parms);
      break;

    case M1X_RM_1X_VOICE:
    case M1X_RM_1X_DATA:
    case M1X_RM_1X_ALL:
      /* Update mcpm params to have highest MCVS request on initial transition
         to voice and data */
      m1x_rm->mcpm_parms.mcvs_request.mcvs_req_type  = MCVS_FULL_REQUEST;
      #ifdef FEATURE_MODEM_1X_JOLOKIA_REVISIT
      m1x_rm->mcpm_parms.mcvs_request.mcvs_update_info =
        MCPM_MCVS_SCALAR_PROC_CLK_UPDATE;
      #else
      m1x_rm->mcpm_parms.mcvs_request.mcvs_update_info =
        MCPM_MCVS_Q6_CLK_UPDATE;
      #endif /* FEATURE_MODEM_1X_JOLOKIA_REVISIT */

      #ifdef FEATURE_MODEM_1X_JOLOKIA
      m1x_rm->mcpm_parms.mcvs_request.q6_clock_KHz =
              rm_q6_freq_req[ M1X_RM_Q6_REQ_MED ];
      #else
      m1x_rm->mcpm_parms.mcvs_request.q6_clock_KHz =
        rm_q6_freq_req[ M1X_RM_Q6_REQ_HIGH ];
      #endif /* FEATURE_MODEM_1X_JOLOKIA */

      m1x_rm->mcpm_parms.mcvs_request.modem_vpe_KHz =
        rm_vpe_freq_req[ M1X_RM_VPE_REQ_MED ];
      param_ptr = &(m1x_rm->mcpm_parms);

      /* Update local MCVS structure to remember that we are at highest MCVS */
      m1x_rm->is_mcvs_req_outstanding = TRUE;
      #ifdef FEATURE_MODEM_1X_JOLOKIA
      m1x_rm->mcvs_parms.q6_freq = M1X_RM_Q6_REQ_MED;
      #else
      m1x_rm->mcvs_parms.q6_freq = M1X_RM_Q6_REQ_HIGH;
      #endif /* FEATURE_MODEM_1X_JOLOKIA */
      m1x_rm->mcvs_parms.vpe_freq = M1X_RM_VPE_REQ_MED;
      break;

    case M1X_RM_1X_INIT:
      /* Update mcpm params to have highest MCVS request on initial transition
         to voice and data */
      m1x_rm->mcpm_parms.mcvs_request.mcvs_req_type  = MCVS_FULL_REQUEST;

      #ifdef FEATURE_MODEM_1X_JOLOKIA_REVISIT
      m1x_rm->mcpm_parms.mcvs_request.mcvs_update_info =
        MCPM_MCVS_SCALAR_PROC_CLK_UPDATE;
      #else
      m1x_rm->mcpm_parms.mcvs_request.mcvs_update_info =
        MCPM_MCVS_Q6_CLK_UPDATE;
      #endif /* FEATURE_MODEM_1X_JOLOKIA_REVISIT */

      #ifdef FEATURE_MODEM_1X_JOLOKIA
      m1x_rm->mcpm_parms.mcvs_request.q6_clock_KHz =
              rm_q6_freq_req[ M1X_RM_Q6_REQ_MED ];
      #else
      m1x_rm->mcpm_parms.mcvs_request.q6_clock_KHz =
        rm_q6_freq_req[ M1X_RM_Q6_REQ_HIGH ];
      #endif /* FEATURE_MODEM_1X_JOLOKIA */

      #ifdef FEATURE_MODEM_1X_ATLAS_REVISIT
      m1x_rm->mcpm_parms.mcvs_request.modem_vpe_KHz =
        rm_vpe_freq_req[ M1X_RM_VPE_REQ_HIGH ];
      #else /* FEATURE_MODEM_1X_ATLAS_REVISIT */
      m1x_rm->mcpm_parms.mcvs_request.modem_vpe_KHz =
        rm_vpe_freq_req[ M1X_RM_VPE_REQ_MED ];
      #endif /* FEATURE_MODEM_1X_ATLAS_REVISIT */

      param_ptr = &(m1x_rm->mcpm_parms);

      /* Update local MCVS structure to remember that we are at highest MCVS */
      m1x_rm->is_mcvs_req_outstanding = TRUE;

      #ifdef FEATURE_MODEM_1X_JOLOKIA
      m1x_rm->mcvs_parms.q6_freq = M1X_RM_Q6_REQ_MED;
      #else
      m1x_rm->mcvs_parms.q6_freq = M1X_RM_Q6_REQ_HIGH;
      #endif /* FEATURE_MODEM_1X_JOLOKIA */

      #ifdef FEATURE_MODEM_1X_ATLAS_REVISIT
      m1x_rm->mcvs_parms.vpe_freq     = M1X_RM_VPE_REQ_HIGH;
      #else /* FEATURE_MODEM_1X_ATLAS_REVISIT */
      m1x_rm->mcvs_parms.vpe_freq = M1X_RM_VPE_REQ_MED;
      #endif /* FEATURE_MODEM_1X_ATLAS_REVISIT */
      break;

    case M1X_RM_1X_INACTIVE:
    case M1X_RM_1X_PSEUDO_SLEEP:
    case M1X_RM_1X_WAKEUP:
    case M1X_RM_1X_IDLE:
      /* No param updates associated with the above states */
      param_ptr = NULL;
      break;

    default:
      RM_MSG( ERROR, "Invalid state specified: %d",
         state );
      param_ptr = NULL;
      break;
  }

  /* check to see if we are currently in an IRAT meas state */
  if ( m1x_rm->current_state == M1X_RM_1X_MEAS_LTO1X ||
       m1x_rm->current_state == M1X_RM_1X_MEAS_1XTOL )
  {
    /* if so, also update measurement "stop" param */
    if ( state == M1X_RM_1X_MEAS_LTO1X ) /* Lto1x */
    {
      m1x_rm->mcpm_parms.neighbor_meas  = MCPM_L21XDO_MEAS_STOP;
      m1x_rm->mcpm_parms.update_info   |= MCPM_NEIGHBOR_MEAS_UPDATE;
    }
    else /* 1xtoL */
    {
      m1x_rm->mcpm_parms.neighbor_meas  = MCPM_1X2L_MEAS_STOP;
      m1x_rm->mcpm_parms.update_info   |= MCPM_NEIGHBOR_MEAS_UPDATE;
    }

    param_ptr = &(m1x_rm->mcpm_parms);
  }

  return param_ptr;

} /* rm_update_mcpm_params */


/*=============================================================================

FUNCTION       RM_CONFIG_MCPM
DESCRIPTION    Configure mcpm appropriately, based on the resource request
               being issued.

DEPENDENCIES   The m1x_rm_activate() routine must have been called previously.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
static void rm_config_mcpm
(
  m1x_rm_state_type  state    /* state being requested */
)
{
  mcpm_request_parms_type* m1x_rm_mcpm_param;
  m1x_rm_mcvs_req_type curr_mcvs_req;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Handle initial "start" request when transitioning out of inactive */
  if ( ( state != M1X_RM_1X_INACTIVE ) &&
       ( m1x_rm->current_state == M1X_RM_1X_INACTIVE ) )
  {
    MCPM_Config_Modem( MCPM_1X_START_REQ, NULL );
  }

  /* Release any pending MCVS requests when going to sleep or inactive */
  if ( ( state == M1X_RM_1X_SLEEP || state == M1X_RM_1X_INACTIVE ) &&
       m1x_rm->is_mcvs_req_outstanding )
  {
    /* Check and send MCPM Stop Requests if required */
    m1x_rm_check_and_send_mcpm_stop_req();

    MCPM_MCVSConfig_Release( MCPM_1X_TECH );

    m1x_rm->is_mcvs_req_outstanding = FALSE;
    m1x_rm->mcvs_parms.q6_freq = M1X_RM_Q6_REQ_NOM;
    m1x_rm->mcvs_parms.vpe_freq = M1X_RM_VPE_REQ_NOM;

    RM_MSG( HIGH, "1x MCVS: Q6 clk to %u KHz, VPE clk to %u KHz (release)",
            rm_q6_freq_req[ m1x_rm->mcvs_parms.q6_freq ],
            rm_vpe_freq_req[ m1x_rm->mcvs_parms.vpe_freq ] );
  }

  /* Configure mcpm for the specified state */
  switch ( state )
  {
    case M1X_RM_1X_INACTIVE:
      MCPM_Config_Modem( MCPM_1X_STOP_REQ,
                         rm_update_mcpm_params( state ) );
      onex_concmgr_check_and_send_stop_req();
      break;

    case M1X_RM_1X_SLEEP:
      onex_concmgr_scenario_config_req( ONEX_CONCMGR_SLEEP, TRUE);
      MCPM_Config_Modem( MCPM_1X_GO_TO_SLEEP_REQ,
                         rm_update_mcpm_params( state ) );
      break;

    case M1X_RM_1X_PSEUDO_SLEEP:
      ONEX_ERR_FATAL( "PSEUDO_SLEEP not supported by MCPM", 0, 0, 0 );
      break;

    case M1X_RM_1X_WAKEUP:
    case M1X_RM_1X_MEAS_LTO1X:
      onex_concmgr_scenario_config_req( ONEX_CONCMGR_WAKEUP, TRUE);
      MCPM_Config_Modem( MCPM_1X_WAKE_UP_REQ,
                         rm_update_mcpm_params( state ) );
      /* Allow dmod reset only while moving to a higher state.
         Do not allow WAKEUP --> LTO1X transition. */
      if ( state > m1x_rm->current_state &&
           m1x_rm->current_state != M1X_RM_1X_WAKEUP )
      {
        RM_MSG( HIGH, "Setting DEMSS block registers...", 0, 0, 0 );
      }
      break;

    case M1X_RM_1X_IDLE:
      onex_concmgr_scenario_config_req( ONEX_CONCMGR_IDLE, TRUE);
      MCPM_Config_Modem( MCPM_1X_IDLE_REQ,
                         rm_update_mcpm_params( state ) );
      break;

    case M1X_RM_1X_VOICE:
		
      onex_concmgr_scenario_config_req( ONEX_CONCMGR_VOICE, TRUE);
      MCPM_Config_Modem( MCPM_1X_VOICE_REQ,
                         rm_update_mcpm_params( state ) );
      #ifdef FEATURE_MODEM_1X_JOLOKIA
      curr_mcvs_req.q6_freq  = M1X_RM_Q6_REQ_MED;
      #else
      curr_mcvs_req.q6_freq  = M1X_RM_Q6_REQ_HIGH;
      #endif /* FEATURE_MODEM_1X_JOLOKIA */
      curr_mcvs_req.vpe_freq = M1X_RM_VPE_REQ_MED;
      rm_update_mcvs_params(&curr_mcvs_req,
                            ONEX_M1X_RM_SCENARIO_1X_VOICE_CALL);
      break;

    case M1X_RM_1X_DATA:
		
      onex_concmgr_scenario_config_req(ONEX_CONCMGR_DATA, TRUE);
      MCPM_Config_Modem( MCPM_1X_DATA_REQ,
                         rm_update_mcpm_params( state ) );
      #ifdef FEATURE_MODEM_1X_JOLOKIA
      curr_mcvs_req.q6_freq  = M1X_RM_Q6_REQ_MED;
      #else
      curr_mcvs_req.q6_freq  = M1X_RM_Q6_REQ_HIGH;
      #endif /* FEATURE_MODEM_1X_JOLOKIA */
      curr_mcvs_req.vpe_freq = M1X_RM_VPE_REQ_MED;
      rm_update_mcvs_params(&curr_mcvs_req, 
                            ONEX_M1X_RM_SCENARIO_1X_DATA_CALL );
      break;

    case M1X_RM_1X_MEAS_1XTOL:
      onex_concmgr_scenario_config_req(ONEX_CONCMGR_MEAS_1XTOL, TRUE);
      MCPM_Config_Modem( MCPM_1X_PARMS_UPDATE_REQ,
                         rm_update_mcpm_params( state ) );
      break;

    case M1X_RM_1X_INIT:
    case M1X_RM_1X_ALL:
      /* Idle state request needed to satisfy init transitions */
      MCPM_Config_Modem( MCPM_1X_IDLE_REQ,
                         rm_update_mcpm_params( state ) );
      MCPM_Config_Modem( MCPM_1X_DATA_REQ,
                         rm_update_mcpm_params( state ) );
      break;

    default:
      RM_MSG( ERROR, "Invalid state specified: %d",
         state );
      break;
  }

} /* rm_config_mcpm */


/*-----------------------------------------------------------------------------
      Configuration Update Functions
-----------------------------------------------------------------------------*/

/*=============================================================================

FUNCTION       RM_UPDATE_STATE_CONFIG
DESCRIPTION    Update the resource request state, invoking the proper
               configuration methods based on the supported feature
               configuration.

DEPENDENCIES   The m1x_rm_activate() routine must have been called previously.

               In addition, the parameters passed to this function should
               already be guaranteed valid. This routine is intended to
               be invoked exclusively from within the external
               m1x_rm_set_state() api, after parameters have been
               validated.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
static void rm_update_state_config
(
  m1x_rm_client_type  client,  /* client identifier */
  m1x_rm_state_type   state    /* state being requested */
)
{
  m1x_rm_state_type   next_state;  /* next state to set */
  m1x_rm_client_type  i;           /* client loop counter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update internal data structures... */

  /* Set the new request */
  m1x_rm->clients[ client ].requested_state = state;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Arbitrate next state based on existing client requests... */

  /* Initialize "next state" to lowest priority state */
  next_state = M1X_RM_FIRST_STATE;

  /* Loop through all clients... */
  for ( i = M1X_RM_FIRST_CLIENT; i < M1X_RM_MAX_CLIENTS; i++ )
  {
    /* Find the highest priority state */
    if ( m1x_rm->clients[ i ].requested_state > next_state )
    {
      next_state = m1x_rm->clients[ i ].requested_state;
    }
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the state config... */

  if ( !srch_diag_msg_get_mask_en( MSG_DBG_STRINGS ) )
  {
    RM_STR( HIGH, "Issued request ( %s : %s ) -> state: %s",
       rm_client_str[ client ],
       rm_state_str[ state ],
       rm_state_str[ next_state ] );
  }
  else
  {
    RM_MSG( HIGH, "Issued request ( %d : %d ) -> state: %d",
       client,
       state,
       next_state );
  }

  /* If the state has changed, proceed with the necessary update */
  if ( next_state != m1x_rm->current_state )
  {
    /* Update the resource request state as required... */
    rm_config_mcpm( next_state );

    /* Record the current resource request state */
    m1x_rm->current_state = next_state;
  }

} /* rm_update_state_config */


/*=============================================================================

FUNCTION       RM_UPDATE_CHAIN_CONFIG
DESCRIPTION    Update the chain configuration, invoking the proper methods
               based on the supported feature configuration.

DEPENDENCIES   The m1x_rm_activate() routine must have been called previously.

               In addition, the parameters passed to this function should
               already be guaranteed valid. This routine is intended to
               be invoked exclusively from within the external
               m1x_rm_config_chain() api, after parameters have been
               validated.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
static void rm_update_chain_config
(
  m1x_rm_client_type  client,  /* client identifier */
  m1x_rm_chain_type   chain    /* chain configuration */
)
{
  m1x_rm_chain_type   next_chain;   /* new chain config to set */
  m1x_rm_client_type  i;            /* client loop counter */
  boolean  chain_0_in_use = FALSE;  /* chain 0 in use? */
  boolean  chain_1_in_use = FALSE;  /* chain 1 in use? */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update internal data structures... */

  /* Set the new request */
  m1x_rm->clients[ client ].requested_chain = chain;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Arbitrate next chain config based on existing client requests... */

  /* Loop through all clients... */
  for ( i = M1X_RM_FIRST_CLIENT; i < M1X_RM_MAX_CLIENTS; i++ )
  {
    switch ( m1x_rm->clients[ i ].requested_chain )
    {
      case M1X_RM_RX_CHAIN_0:
        chain_0_in_use = TRUE;
        break;

      case M1X_RM_RX_CHAIN_1:
        chain_1_in_use = TRUE;
        break;

      case M1X_RM_RX_CHAIN_BOTH:
        chain_0_in_use = TRUE;
        chain_1_in_use = TRUE;
        break;

      default:
        /* No chain config requested */
        break;
    }
  }

  /* Use both chains */
  if ( ( chain_0_in_use && chain_1_in_use ) ||
       ( m1x_rm_force_enable_all ) )
  {
    next_chain = M1X_RM_RX_CHAIN_BOTH;
  }
  /* Use none of the chains */
  else if ( !chain_0_in_use && !chain_1_in_use )
  {
    next_chain = M1X_RM_RX_CHAIN_NONE;
  }
  /* Use chain 1 */
  else if ( chain_1_in_use )
  {
    next_chain = M1X_RM_RX_CHAIN_1;
  }
  /* Use chain 0 */
  else
  {
    next_chain = M1X_RM_RX_CHAIN_0;
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the chain config... */

  if ( !srch_diag_msg_get_mask_en( MSG_DBG_STRINGS ) )
  {
    RM_STR( HIGH, "Issued chain config ( %s : %s ) -> config: %s",
       rm_client_str[ client ],
       rm_chain_str[ chain ],
       rm_chain_str[ next_chain ] );
  }
  else
  {
    RM_MSG( HIGH, "Issued chain config ( %d : %d ) -> config: %d",
       client,
       chain,
       next_chain );
  }

  /* If the chain config has changed, proceed with the necessary update */
  if ( next_chain != m1x_rm->current_chain )
  {
    /* Record the current chain config */
    m1x_rm->current_chain = next_chain;
  }

} /* rm_update_chain_config */


/*=============================================================================

FUNCTION       RM_REGISTER_EVENT_CB
DESCRIPTION    Update the chain configuration, invoking the proper methods
               based on the supported feature configuration.

DEPENDENCIES   The m1x_rm_activate() routine must have been called previously.

               In addition, the parameters passed to this function should
               already be guaranteed valid. This routine is intended to
               be invoked exclusively from within the external
               m1x_rm_register_event_cb() api, after parameters have been
               validated.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
static void rm_register_event_cb
(
  m1x_rm_cb_event_type  event,  /* specified event */
  m1x_rm_cb_type        cb      /* callback to invoke when event occurs */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch( event )
  {
    case M1X_RM_CB_OPTIMIZE_TL_EVENT:
      MCPM_NPA_Set_Sleep_Timeline_Callback( MCPM_1X_TECH,
                                            (mcpm_npa_slp_tmln_callback_type) cb );
      break;

    /* case M1X_RM_CB_xxx_EVENT: Add additional events here... */

    default:
      RM_MSG( ERROR, "Invalid event specified: %d",
         event );
      break;
  }

} /* rm_register_event_cb */

/*=============================================================================

FUNCTION       ONEX_CB_FUNC_FOR_CONC_MGR
DESCRIPTION    This API is callback that can be called by CONC MGR


DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/
void onex_cb_func_for_conc_mgr 
( 
  cmgr_tech_type 		     tech,       
  uint32 			         scenario, 
  cmgr_scenario_status_e     status,
  uint32 					 return_info
)
{
  if(status != CMGR_ACCEPTED)
  {
    ONEX_ERR_FATAL("CMGR returned %d for scenario %d",status,scenario,0);
  }
  else
  {
    RM_MSG( HIGH, "Received ACCEPTED for scenario %d ", scenario);
    m1x_rm->onex_received_cmgr_cb = TRUE;
  }
}



/*-----------------------------------------------------------------------------
      Exported Functions
-----------------------------------------------------------------------------*/

/*=============================================================================

FUNCTION       M1X_RM_ACTIVATE
DESCRIPTION    Initialize the modem 1x resource manager module.

DEPENDENCIES   Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_activate( void )
{
  m1x_rm_client_type         client;     /* client loop counter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Handle redundant activations */
  if ( m1x_rm != NULL && m1x_rm->initialized == TRUE )
  {
    RM_MSG( MED, "Module already activated" );
    return;
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* REVISIT: Ensure critical section init is atomic */
  /* Initialize the critical section global lock */
  rex_init_crit_sect( &m1x_rm_crit_sect );

  RM_MSG( HIGH, "Initializing M1X_RM..." );

  M1X_RM_ENTER_CRIT_SECT();

  /* Allocate memory for the module's data structures */
  m1x_rm = modem_mem_alloc( sizeof(*m1x_rm), MODEM_MEM_CLIENT_1X_CRIT );

  /* Check for allocation failure */
  if ( m1x_rm == NULL )  /* Allocation failed */
  {
    ONEX_ERR_FATAL( "Failed to allocate memory: %d bytes",
                sizeof(*m1x_rm), 0, 0 );
  }

  //register cb function with conc_mgr
  cmgr_register_cb( CMGR_1X_TECH, onex_cb_func_for_conc_mgr);

  /* Initialize the module's data structure... */

  /* Clear the allocated memory */
  memset( m1x_rm, 0, sizeof( *m1x_rm ) );

  /* Loop through all clients... */
  for ( client = M1X_RM_FIRST_CLIENT; client < M1X_RM_MAX_CLIENTS; client++ )
  {
    /* Initialize the requested state */
    m1x_rm->clients[ client ].requested_state = M1X_RM_1X_INACTIVE;

    /* Initialize the requested chain config */
    m1x_rm->clients[ client ].requested_chain = M1X_RM_RX_CHAIN_NONE;
  }

  /* Initialize the current state */
  m1x_rm->current_state = M1X_RM_FIRST_STATE;

  /* Initialize the current chain config */
  m1x_rm->current_chain = M1X_RM_FIRST_CHAIN_CONFIG;

  /* Initialize the srch power module */
  srch_pwr_init();

  /* Mark the module initialized */
  m1x_rm->initialized = TRUE;

  /* Check for force enable - the "initialized" flag must be set previously,
     otherwise any attempt to configure chain or set state from M1X_RM
     client will fail... */
  if ( m1x_rm_force_enable_all )
  {
    /* Force enable all resources... */
    m1x_rm_config_chain( M1X_RM_INTERNAL, M1X_RM_RX_CHAIN_BOTH );
    m1x_rm_set_state( M1X_RM_INTERNAL, M1X_RM_1X_ALL );
  }

  /* Update the base Q6 and VPE clock frequencies made as part of MCPM state
     request */
  m1x_rm->mcvs_state_parms[M1X_RM_1X_INACTIVE].q6_freq  = M1X_RM_Q6_REQ_NOM;
  m1x_rm->mcvs_state_parms[M1X_RM_1X_INACTIVE].vpe_freq = M1X_RM_VPE_REQ_NOM;

  m1x_rm->mcvs_state_parms[M1X_RM_1X_SLEEP].q6_freq  = M1X_RM_Q6_REQ_NOM;
  m1x_rm->mcvs_state_parms[M1X_RM_1X_SLEEP].vpe_freq = M1X_RM_VPE_REQ_NOM;

  m1x_rm->mcvs_state_parms[M1X_RM_1X_PSEUDO_SLEEP].q6_freq  = M1X_RM_Q6_REQ_NOM;
  m1x_rm->mcvs_state_parms[M1X_RM_1X_PSEUDO_SLEEP].vpe_freq =M1X_RM_VPE_REQ_NOM;

  m1x_rm->mcvs_state_parms[M1X_RM_1X_MEAS_LTO1X].q6_freq  = M1X_RM_Q6_REQ_HIGH;
  m1x_rm->mcvs_state_parms[M1X_RM_1X_MEAS_LTO1X].vpe_freq = M1X_RM_VPE_REQ_NOM;

  /* Use right clock frequency for JOLOKIA for IDLE/WAKEUP scenarios */
  #ifdef FEATURE_MODEM_1X_JOLOKIA_REVISIT
  m1x_rm->mcvs_state_parms[M1X_RM_1X_IDLE].q6_freq   = M1X_RM_Q6_REQ_MED;
  m1x_rm->mcvs_state_parms[M1X_RM_1X_WAKEUP].q6_freq = M1X_RM_Q6_REQ_MED;
  #else /* !FEATURE_MODEM_1X_JOLOKIA_REVISIT */
  m1x_rm->mcvs_state_parms[M1X_RM_1X_IDLE].q6_freq   = M1X_RM_Q6_REQ_HIGH;
  m1x_rm->mcvs_state_parms[M1X_RM_1X_WAKEUP].q6_freq = M1X_RM_Q6_REQ_HIGH;
  #endif /* FEATURE_MODEM_1X_JOLOKIA_REVISIT */

  m1x_rm->mcvs_state_parms[M1X_RM_1X_WAKEUP].vpe_freq = M1X_RM_VPE_REQ_NOM;
  m1x_rm->mcvs_state_parms[M1X_RM_1X_IDLE].vpe_freq   = M1X_RM_VPE_REQ_NOM;

  m1x_rm->mcvs_state_parms[M1X_RM_1X_VOICE].q6_freq  = M1X_RM_Q6_REQ_NOM;
  m1x_rm->mcvs_state_parms[M1X_RM_1X_VOICE].vpe_freq = M1X_RM_VPE_REQ_NOM;

  m1x_rm->mcvs_state_parms[M1X_RM_1X_DATA].q6_freq  = M1X_RM_Q6_REQ_MED;
  m1x_rm->mcvs_state_parms[M1X_RM_1X_DATA].vpe_freq = M1X_RM_VPE_REQ_NOM;

  m1x_rm->mcvs_state_parms[M1X_RM_1X_INIT].q6_freq  = M1X_RM_Q6_REQ_MED;
  #ifdef FEATURE_MODEM_1X_ATLAS_REVISIT
  m1x_rm->mcvs_state_parms[M1X_RM_1X_INIT].vpe_freq = M1X_RM_VPE_REQ_HIGH;
  #else /* FEATURE_MODEM_1X_ATLAS_REVISIT */
  m1x_rm->mcvs_state_parms[M1X_RM_1X_INIT].vpe_freq = M1X_RM_VPE_REQ_NOM;
  #endif /* FEATURE_MODEM_1X_ATLAS_REVISIT */

  m1x_rm->mcvs_state_parms[M1X_RM_1X_MEAS_1XTOL].q6_freq  = M1X_RM_Q6_REQ_MED;
  m1x_rm->mcvs_state_parms[M1X_RM_1X_MEAS_1XTOL].vpe_freq = M1X_RM_VPE_REQ_NOM;

  m1x_rm->mcvs_state_parms[M1X_RM_1X_ALL].q6_freq  = M1X_RM_Q6_REQ_MED;
  m1x_rm->mcvs_state_parms[M1X_RM_1X_ALL].vpe_freq = M1X_RM_VPE_REQ_NOM;

  M1X_RM_LEAVE_CRIT_SECT();

} /* m1x_rm_activate */


/*=============================================================================

FUNCTION       M1X_RM_DEACTIVATE
DESCRIPTION    De-initialize the modem 1x resource manager module.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_deactivate( void )
{
  m1x_rm_client_type  client;            /* client loop counter */
  m1x_rm_state_type   requested_state;   /* state requested by the client */
  m1x_rm_chain_type   requested_chain;   /* chain config requested */
  boolean             deinit_ok = TRUE;  /* ok to deinit module */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Handle redundant deactivations */
  if ( m1x_rm == NULL || m1x_rm->initialized == FALSE )
  {
    RM_MSG( MED, "Module already deactivated" );
    return;
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  RM_MSG( HIGH, "De-initializing M1X_RM..." );

  M1X_RM_ENTER_CRIT_SECT();

  /* Check for force enable */
  if ( m1x_rm_force_enable_all )
  {
    /* Release all resources... */
    m1x_rm_config_chain( M1X_RM_INTERNAL, M1X_RM_RX_CHAIN_NONE );
    m1x_rm_set_state( M1X_RM_INTERNAL, M1X_RM_1X_INACTIVE );

    /* Clear the force enable flag */
    m1x_rm_force_enable_all = FALSE;
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Loop through all clients... */
  for ( client = M1X_RM_FIRST_CLIENT; client < M1X_RM_MAX_CLIENTS; client++ )
  {
    /* Grab the client's requested state and chain config */
    requested_state = m1x_rm->clients[ client ].requested_state;
    requested_chain = m1x_rm->clients[ client ].requested_chain;

    /* Check to see if all clients have cleared resource state requests */
    if ( requested_state != M1X_RM_1X_INACTIVE ||
         requested_chain != M1X_RM_RX_CHAIN_NONE )
    {
      if ( !srch_diag_msg_get_mask_en( MSG_DBG_STRINGS ) )
      {
        RM_STR( ERROR,
           "Active clients - abort! ( client %s : state %s : chain %s )",
           rm_client_str[ client ],
           rm_state_str[ requested_state ],
           rm_chain_str[ requested_chain ] );
      }
      else
      {
        RM_MSG( ERROR,
           "Active clients - abort! ( client %d : state %d : chain %d )",
           client,
           requested_state,
           requested_chain );
      }

      /* Do not allow deinit to continue until all clients have released
         their requests. */
      deinit_ok = FALSE;
    }
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Have all clients released their requests?... */
  if ( deinit_ok )
  {
    /* Deactivate the srch power module */
    srch_pwr_deinit();

    /* Clear the allocated memory */
    memset( m1x_rm, 0, sizeof( *m1x_rm ) );

    /* Release the dynamically allocated memory */
    modem_mem_free( m1x_rm, MODEM_MEM_CLIENT_1X_CRIT );

    /* Clear the module's data structure ptr */
    m1x_rm = NULL;

  } /* deinit_ok */

  M1X_RM_LEAVE_CRIT_SECT();

} /* m1x_rm_deactivate */


/*=============================================================================

FUNCTION       M1X_RM_SET_STATE
DESCRIPTION    Issue a resource request from a particular client, for the
               specified state.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_set_state
(
  m1x_rm_client_type  client,  /* client identifier */
  m1x_rm_state_type   state    /* state being requested */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check for invalid module initialization */
  if ( m1x_rm == NULL || m1x_rm->initialized != TRUE )
  {
    RM_MSG( ERROR, "Attempt to issue request before calling init" );

    /* Initialize the module */
    m1x_rm_activate();
  }

  /* Check for valid client */
  if ( client >= M1X_RM_MAX_CLIENTS )
  {
    RM_MSG( ERROR, "Invalid client specified: %d",
       client );
    return;
  }

  /* Check for valid resource state */
  if ( state >= M1X_RM_MAX_STATES )
  {
    RM_MSG( ERROR, "Invalid state specified: %d",
       state );
    return;
  }

  /* Check for force enable - the "initialized" flag must be set previously,
     otherwise any attempt to configure chain or set state from M1X_RM
     client will fail... */
  if ( m1x_rm_force_enable_all )
  {
    if ( !srch_diag_msg_get_mask_en( MSG_DBG_STRINGS ) )
    {
      RM_STR( ERROR, "DEBUG: Force enable all clocks. client=%s, state=%s",
         rm_client_str[ client ],
         rm_state_str[ state ] );
    }
    else
    {
      RM_MSG( ERROR, "DEBUG: Force enable all clocks. client=%d, state=%d",
         client,
         state );
    }


    /* Force enable all resources... */
    m1x_rm_config_chain( M1X_RM_INTERNAL, M1X_RM_RX_CHAIN_BOTH );
    client = M1X_RM_INTERNAL;
    state  = M1X_RM_1X_ALL;
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  M1X_RM_ENTER_CRIT_SECT();

  /* Ensure the module data structure is valid */
  M1X_RM_NULL_PTR_CHK( m1x_rm );

  /* Update the resource configuration... */
  rm_update_state_config( client, state );

  M1X_RM_LEAVE_CRIT_SECT();

} /* m1x_rm_set_state */


/*=============================================================================

FUNCTION       M1X_RM_GET_STATE
DESCRIPTION    Returns the currently configured resource request state.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   The current resource state that has been requested.

SIDE EFFECTS   None.

=============================================================================*/
m1x_rm_state_type  m1x_rm_get_state( void )
{
  m1x_rm_state_type  current_state;  /* current state config */
  m1x_rm_chain_type  current_chain;  /* current chain config */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check for invalid module initialization */
  if ( m1x_rm == NULL || m1x_rm->initialized != TRUE )
  {
    RM_MSG( ERROR, "Attempt to query state before calling init" );

    /* Initialize the module */
    m1x_rm_activate();
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  M1X_RM_ENTER_CRIT_SECT();

  /* Ensure the module data structure is valid */
  M1X_RM_NULL_PTR_CHK( m1x_rm );

  /* Get the current configuration */
  current_state = m1x_rm->current_state;
  current_chain = m1x_rm->current_chain;

  M1X_RM_LEAVE_CRIT_SECT();

  if ( !srch_diag_msg_get_mask_en( MSG_DBG_STRINGS ) )
  {
    RM_STR( MED, "Current settings: ( state %s : chain %s )",
       rm_state_str[ current_state ],
       rm_chain_str[ current_chain ] );
  }
  else
  {
    RM_MSG( MED, "Current settings: ( state %d : chain %d )",
       current_state,
       current_chain );
  }

  /* Return the current state */
  return current_state;

} /* m1x_rm_get_state */

/*=============================================================================

FUNCTION       M1X_RM_GET_MCPM_SCENARIO

DESCRIPTION    Returns the corresponding MCPM scenario type based on input.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   The current resource state that has been requested.

SIDE EFFECTS   None.

=============================================================================*/
onex_m1x_rm_mcpm_scenario_type  m1x_rm_get_mcpm_scenario
( 
  onex_m1x_rm_scenario_type  onex_scenario    /* ONEX Scenario Type */
)
{
  onex_m1x_rm_mcpm_scenario_type mcpm_scenario = ONEX_SCENARIO_NONE;

  /*----------------------------------------------------------------*/

  /* Return scenario corresponding to State */
  switch( onex_scenario )
  {
    case ONEX_M1X_RM_SCENARIO_NONE:
      mcpm_scenario = ONEX_SCENARIO_NONE;
      break;
  
    case ONEX_M1X_RM_SRCH_TC_1X2X_START_QTA:
    case ONEX_M1X_RM_SRCH_TC_1X2X_EXIT_QTA:
      mcpm_scenario = ONEX_SRCH_TC_1X2X_QTA;
      break;

    case ONEX_M1X_RM_SRCH_TC_1X_SRCH_REQ:
    case ONEX_M1X_RM_SRCH_TC_1X_SRCH_REQ_DONE:
      mcpm_scenario = ONEX_SRCH_TC_1X_SRCH_REQ;
      break;

    case ONEX_M1X_RM_SCENARIO_1X_VOICE_CALL:
      mcpm_scenario = ONEX_SCENARIO_1X_VOICE_CALL;
      break;

    case ONEX_M1X_RM_SCENARIO_1X_DATA_CALL:
      mcpm_scenario = ONEX_SCENARIO_1X_DATA_CALL;
      break;
	
    case ONEX_M1X_RM_SRCH_FING_NOM:
      mcpm_scenario = ONEX_SRCH_FING_NOM;
      break;
	
    case ONEX_M1X_RM_SRCH_FING_MED:
      mcpm_scenario = ONEX_SRCH_FING_MED;
      break;
	
    case ONEX_M1X_RM_SRCH_FING_HIGH:
      mcpm_scenario = ONEX_SRCH_FING_HIGH;
      break;
	
    case ONEX_M1X_RM_SRCH_FING_NOM_MED:
      mcpm_scenario = ONEX_SRCH_FING_NOM_MED;
      break;
	
    case ONEX_M1X_RM_SRCH_FING_MED_NOM:
      mcpm_scenario = ONEX_SRCH_FING_MED_NOM;
      break;
	
    case ONEX_M1X_RM_SRCH_FING_HIGH_NOM:
      mcpm_scenario = ONEX_SRCH_FING_HIGH_NOM;
      break;
	
    case ONEX_M1X_RM_SRCH_FING_NOM_HIGH:
      mcpm_scenario = ONEX_SRCH_FING_NOM_HIGH;
      break;
	
    case ONEX_M1X_RM_SRCH_FING_MED_HIGH:
      mcpm_scenario = ONEX_SRCH_FING_MED_HIGH;
      break;
	
    case ONEX_M1X_RM_SRCH_FING_HIGH_MED:
      mcpm_scenario = ONEX_SRCH_FING_HIGH_MED;
      break;

    default:
      RM_MSG( ERROR, 
              "Invalid onex_scenario specified:%d",
              onex_scenario );
      break;
  }

  return mcpm_scenario;
} /* m1x_rm_get_mcpm_scenario() */

/*=============================================================================

FUNCTION       M1X_RM_SET_MCPM_REQ_STATUS

DESCRIPTION    API Sets the MCPM Request status to passed argument

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_set_mcpm_req_status
( 
  /* ONEX MCPM Scenario Type */
  onex_m1x_rm_mcpm_scenario_type  onex_mcpm_scenario,
  /* Request Status */
  boolean req_status
)
{
  /* Validate the mcpm scenario */
  if( onex_mcpm_scenario >= ONEX_SCENARIO_MAX )
  {
    RM_MSG( ERROR, 
            "Ivalid MCPM Scenario:%d",
            onex_mcpm_scenario );
    return;
  }

  onex_mcvs_mcpm_req[onex_mcpm_scenario].start_req_sent = req_status;

  RM_MSG( LOW, 
          "MCPM scenario:%d, start_req_sent:%d",
          onex_mcpm_scenario,
          onex_mcvs_mcpm_req[onex_mcpm_scenario].start_req_sent );
} /* m1x_rm_set_mcpm_req_status() */

/*=============================================================================

FUNCTION       M1X_RM_GET_MCPM_REQ_STATUS

DESCRIPTION    API returns the MCPM Request status to passed argument

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   TRUE - If MCPM Request Status sent,
               FALSE - Otherwise.

SIDE EFFECTS   None.

=============================================================================*/
boolean m1x_rm_get_mcpm_req_status
( 
  /* ONEX MCPM Scenario Type */
  onex_m1x_rm_mcpm_scenario_type  onex_mcpm_scenario
)
{
  /* Validate the mcpm scenario */
  if( onex_mcpm_scenario >= ONEX_SCENARIO_MAX )
  {
    RM_MSG( ERROR, 
            "Ivalid MCPM Scenario:%d",
            onex_mcpm_scenario );
    return FALSE;
  }

  RM_MSG( LOW, 
          "MCPM scenario:%d, start_req_sent:%d",
          onex_mcpm_scenario,
          onex_mcvs_mcpm_req[onex_mcpm_scenario].start_req_sent );

  return onex_mcvs_mcpm_req[onex_mcpm_scenario].start_req_sent;
} /* m1x_rm_get_mcpm_req_status() */

/*=============================================================================

FUNCTION       M1X_RM_SEND_MCPM_STOP_REQ

DESCRIPTION    API used to send MOPM Stop Request for particular MCPM scenario.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.
               Also onex should have sent MCPM Start Request.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_send_mcpm_stop_req
( 
  /* ONEX MCPM Scenario Type */
  onex_m1x_rm_mcpm_scenario_type  onex_mcpm_scenario
)
{
  /* Scenario */
  uint32 mcpm_scenario = 0; 

  /* MCVS Request */
  mcpm_mcvsrequest_parms_type mcvs_parms = { 0 };

  /*--------------------------------------------------------------------*/

  /* Validate the mcpm scenario */
  if( (onex_mcpm_scenario == ONEX_SCENARIO_NONE) ||
      (onex_mcpm_scenario >= ONEX_SCENARIO_MAX)
    )
  {
    RM_MSG( ERROR, 
            "Ivalid MCPM Scenario:%d",
            onex_mcpm_scenario );
    return;
  }

  /* Check if we sent MCPM Start Request, 
   * if yes then only send MCPM Stop Request */
  if( m1x_rm_get_mcpm_req_status(onex_mcpm_scenario) == FALSE )
  {
    RM_MSG( HIGH, 
            "MCPM Stop Req not Required for Scenario:%d",
            onex_mcpm_scenario );
    return;
  }

  /* Fill MCVS Params details */
  mcvs_parms.mcvs_req_type    = MCVS_FULL_REQUEST;
  mcvs_parms.mcvs_update_info = MCPM_MCVS_SCENARIO_INFO_UPDATE;

  /* Mcpm uses 32 bit uint to extract scenario info as this: 
   * LSB 8 bits (7:0)     - Internal to mcpm
   * 1 bit (8)            - Scenario START/STOP
   * USB 23 bits(31:9)    - Scenario ID (range 0- 2^23) */
  mcpm_scenario = ( (onex_mcpm_scenario << 1) | FALSE ) << 8;
  mcvs_parms.scenario_info = &mcpm_scenario;
  
  /* Send MCVS Config to MCPM */
  MCPM_MCVSConfig_Modem( MCPM_1X_TECH, &mcvs_parms );

  /* Rest MCPM Start Req status */
  m1x_rm_set_mcpm_req_status(onex_mcpm_scenario, FALSE);

  RM_MSG( HIGH, 
          "1x MCVS Stop_Req: Cur_State:%d, Scenario:%u, Start:%u, "
          "MCPM_Start_Req_Status:%d",
          m1x_rm->current_state,
          ( (*mcvs_parms.scenario_info) >> 9),
          ( ( (*mcvs_parms.scenario_info) >> 8) & 0x1),
          m1x_rm_get_mcpm_req_status(onex_mcpm_scenario) );
} /* m1x_rm_send_mcpm_stop_req() */

/*=============================================================================

FUNCTION       M1X_RM_CHECK_AND_SEND_MCPM_STOP_REQ

DESCRIPTION    API used to send MOPM Stop Requests for all the MCPM scenarios, 
               for which onex already sent MCPM Start Requests to MCPM.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.
               Also onex should have sent MCPM Start Request.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_check_and_send_mcpm_stop_req ( void )
{
  int i; /* Lopp Variable */

  /*------------------------------------------------------------------*/

  /* Loop through all MCPM Scenarios */
  for( i = 1; i < ONEX_SCENARIO_MAX; i++ )
  {
    /* Check if we sent MCPM Start first for this MCPM Scenario */
    if( m1x_rm_get_mcpm_req_status(i) == TRUE )
    {
      m1x_rm_send_mcpm_stop_req( (onex_m1x_rm_mcpm_scenario_type)i );
    }
  }
} /* m1x_rm_check_and_send_mcpm_stop_req() */

/*=============================================================================

FUNCTION       M1X_RM_GET_MCVS_REQ
DESCRIPTION    Returns the currently configured MCVS request.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   The current MCVS request.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_get_mcvs_req (m1x_rm_mcvs_req_type* mcvs_req_ptr)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  M1X_RM_ENTER_CRIT_SECT();

  /* Ensure the module data structure is valid */
  M1X_RM_NULL_PTR_CHK( m1x_rm );
  M1X_RM_NULL_PTR_CHK( mcvs_req_ptr );

  *mcvs_req_ptr = m1x_rm->mcvs_parms;

  M1X_RM_LEAVE_CRIT_SECT();

} /* m1x_rm_get_mcvs_req */

/*=============================================================================

FUNCTION       M1X_RM_QUERY_CAPABILITY
DESCRIPTION    Query whether the current resource request state provides
               the specified capability.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   TRUE if the capability is supported, FALSE otherwise.

SIDE EFFECTS   None.

=============================================================================*/
boolean  m1x_rm_query_capability
(
  m1x_rm_capability_type  capability  /* capability to query */
)
{
  m1x_rm_state_type  curr_state;  /* current resource request state */
  boolean            is_capable;  /* capability supported in current state */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check for invalid module initialization */
  if ( m1x_rm == NULL || m1x_rm->initialized != TRUE )
  {
    RM_MSG( ERROR, "Attempt to query capability before calling init" );

    /* Initialize the module */
    m1x_rm_activate();
  }

  /* Check for valid capability */
  if ( capability >= M1X_RM_MAX_CAPABILITIES )
  {
    RM_MSG( ERROR, "Invalid capability specified: %d",
       capability );
    return FALSE;
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  M1X_RM_ENTER_CRIT_SECT();

  /* Ensure the module data structure is valid */
  M1X_RM_NULL_PTR_CHK( m1x_rm );

  /* Grab the current resource request state */
  curr_state = m1x_rm->current_state;

  /* Check for capability support... */
  switch ( capability )
  {
    case M1X_RM_DEMOD_CAPABLE:
      /* Is demod enabled? */
      switch ( curr_state )
      {
        case M1X_RM_1X_WAKEUP:
        case M1X_RM_1X_IDLE:
        case M1X_RM_1X_VOICE:
        case M1X_RM_1X_DATA:
        case M1X_RM_1X_INIT:
        case M1X_RM_1X_ALL:
          is_capable = TRUE;
          break;
        default:
          is_capable = FALSE;
          break;
      }
      break;

    case M1X_RM_VD_CAPABLE:
      /* Is VD enabled? */
      switch ( curr_state )
      {
        case M1X_RM_1X_IDLE:
        case M1X_RM_1X_VOICE:
        case M1X_RM_1X_DATA:
        case M1X_RM_1X_INIT:
        case M1X_RM_1X_ALL:
          is_capable = TRUE;
          break;
        default:
          is_capable = FALSE;
          break;
      }
      break;

    case M1X_RM_TX_CAPABLE:
      /* Is Tx enabled? */
      switch ( curr_state )
      {
        case M1X_RM_1X_INIT:
        case M1X_RM_1X_VOICE:
        case M1X_RM_1X_DATA:
        case M1X_RM_1X_ALL:
          is_capable = TRUE;
          break;
        default:
          is_capable = FALSE;
          break;
      }
      break;

    case M1X_RM_TD_CAPABLE:
      /* Is TD enabled? */
      switch( curr_state )
      {
        case M1X_RM_1X_INIT:
        case M1X_RM_1X_DATA:
        case M1X_RM_1X_ALL:
          is_capable = TRUE;
          break;
        default:
          is_capable = FALSE;
          break;
      }
      break;

    default:
      RM_MSG( ERROR, "Invalid capability specified: %d",
         capability );
      is_capable = FALSE;
      break;
  }

  M1X_RM_LEAVE_CRIT_SECT();

  if ( !srch_diag_msg_get_mask_en( MSG_DBG_STRINGS ) )
  {
    RM_STR( LOW, "Capability query: %s en: %d ( state: %s )",
       rm_capability_str[capability],
       is_capable,
       rm_state_str[ curr_state ] );
  }
  else
  {
    RM_MSG( LOW, "Capability query: %d en: %d ( state: %d )",
       capability,
       is_capable,
       curr_state );
  }

  return is_capable;

} /* m1x_rm_query_capability */


/*=============================================================================

FUNCTION       M1X_RM_UPDATE_PARAMS
DESCRIPTION    Update the parameters associated with the specified state.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_update_params
(
  m1x_rm_client_type   client,  /* client identifier */
  m1x_rm_state_type   state    /* update params for the specified state */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check for invalid module initialization */
  if ( m1x_rm == NULL || m1x_rm->initialized != TRUE )
  {
    RM_MSG( ERROR, "Attempt to issue request before calling init" );

    /* Initialize the module */
    m1x_rm_activate();
  }

  /* Check for valid client */
  if ( client >= M1X_RM_MAX_CLIENTS )
  {
    RM_MSG( ERROR, "Invalid client specified: %d",
       client );
    return;
  }

  /* Check for valid resource state */
  if ( state >= M1X_RM_MAX_STATES )
  {
    RM_MSG( ERROR, "Invalid state specified: %d",
       state );
    return;
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( !srch_diag_msg_get_mask_en( MSG_DBG_STRINGS ) )
  {
    RM_STR( MED, "  updating params ( state: %s )",
       rm_state_str[ state ] );
  }
  else
  {
    RM_MSG( MED, "  updating params ( state: %d )",
       state );
  }

  M1X_RM_ENTER_CRIT_SECT();

  /* Ensure the module data structure is valid */
  M1X_RM_NULL_PTR_CHK( m1x_rm );

  /* Update the specified paramenters */
  MCPM_Config_Modem( MCPM_1X_PARMS_UPDATE_REQ, rm_update_mcpm_params( state ) );

  M1X_RM_LEAVE_CRIT_SECT();

} /* m1x_rm_update_params */

/*=============================================================================

FUNCTION       M1X_RM_UPDATE_MCVS_PARAMS
DESCRIPTION    Update the mcvs parameters.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_update_mcvs_params
(
  m1x_rm_mcvs_req_type *mcvs_req_ptr,    /* MCVS requests */
  onex_m1x_rm_scenario_type onex_scenario /* ONEX Scenario Type */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  M1X_RM_ENTER_CRIT_SECT();

  /* Ensure the module data structure is valid */
  M1X_RM_NULL_PTR_CHK( m1x_rm );

  /* Ensure 1x is active */
  if ( (m1x_rm->current_state >= M1X_RM_1X_WAKEUP) &&
       (mcvs_req_ptr != NULL ))
  {
    /* MCVS updates */
    rm_update_mcvs_params(mcvs_req_ptr,onex_scenario);
  }

  M1X_RM_LEAVE_CRIT_SECT();

} /* m1x_rm_update_mcvs_params */

/*=============================================================================

FUNCTION       M1X_RM_CONFIG_CHAIN
DESCRIPTION    Configure resources required by the specified chain.
               This interface is used for both enabling and disabling chain
               based configuration.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_config_chain
(
  m1x_rm_client_type  client,  /* client identifier */
  m1x_rm_chain_type   chain    /* chain configuration */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check for invalid module initialization */
  if ( m1x_rm == NULL || m1x_rm->initialized != TRUE )
  {
    RM_MSG( ERROR, "Attempt to config chain before calling init" );

    /* Initialize the module */
    m1x_rm_activate();
  }

  /* Check for valid client */
  if ( client >= M1X_RM_MAX_CLIENTS )
  {
    RM_MSG( ERROR, "Invalid client specified: %d",
       client );
    return;
  }

  /* Check for valid chain */
  if ( chain >= M1X_RM_MAX_CHAIN_CONFIGS )
  {
    RM_MSG( ERROR, "Invalid chain specified: %d",
       chain );
    return;
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  M1X_RM_ENTER_CRIT_SECT();

  /* Ensure the module data structure is valid */
  M1X_RM_NULL_PTR_CHK( m1x_rm );

  /* Update the chain configuration... */
  rm_update_chain_config( client, chain );

  M1X_RM_LEAVE_CRIT_SECT();

} /* m1x_rm_config_chain */


/*=============================================================================

FUNCTION       M1X_RM_REGISTER_EVENT_CB
DESCRIPTION    Register a callback with M1X resource manager for a specified
               event. The registered callback will then be invoked when that
               particular event occurs.

DEPENDENCIES   The m1x_rm_activate() routine should have been called previously.
               Must not be called from interrupt context.

RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/
void m1x_rm_register_event_cb
(
  m1x_rm_cb_event_type  event,  /* specified event */
  m1x_rm_cb_type        cb      /* callback to invoke when event occurs */
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check for invalid module initialization */
  if ( m1x_rm == NULL || m1x_rm->initialized != TRUE )
  {
    RM_MSG( ERROR, "Attempt to preserve timing before calling init" );

    /* Initialize the module */
    m1x_rm_activate();
  }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  RM_MSG( MED, "Register cb, event: %d",
     event );

  M1X_RM_ENTER_CRIT_SECT();

  /* Ensure the module data structure is valid */
  M1X_RM_NULL_PTR_CHK( m1x_rm );

  /* Register the callback for the specified event */
  rm_register_event_cb( event, cb );

  M1X_RM_LEAVE_CRIT_SECT();

} /* m1x_rm_register_event_cb */


/*=============================================================================

FUNCTION       M1X_RM_PROTOCOL_SHUTDOWN_CONFIG
DESCRIPTION    API to inform M1X_RM to configure MCPM calls to shutdown MDSP
               FW while goingt to INACTIVE state

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

=============================================================================*/
void m1x_rm_protocol_shutdown_config
(
  boolean   enable  /* TRUE => configure for MDSP FW shutdown,
                       FALSE => De-configure MCPM for 1X */
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( !m1x_rm_force_enable_all )
  {
    if ( enable == TRUE )
    {
      /* Move into inactive state */
      m1x_rm_set_state( M1X_RM_SRCH_SECONDARY, M1X_RM_1X_INACTIVE );
      m1x_rm_set_state( M1X_RM_SRCH, M1X_RM_1X_INACTIVE );

      /* Temporarily enable MCPM for FW shutdown */
      MCPM_Config_Modem( MCPM_1X_START_REQ, NULL );
    }
    else
    {
      /* Disable MCPM post FW shutdown */
      MCPM_Config_Modem( MCPM_1X_STOP_REQ, NULL );
    }
  }
  else
  {
    RM_MSG( ERROR,
      "DEBUG: Force enable all clocks. "
      "Ignoring MCPM config for protocol shutdown" );
  }

} /* m1x_rm_protocol_shutdown_config */

/*=============================================================================

FUNCTION       ONEX_CONCMGR_SCENARIO_CONFIG_REQ
DESCRIPTION    This API informs conc mgr about the new scenario and stops older
               scenario.

DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/
void onex_concmgr_scenario_config_req
(
  onex_cocnmgr_state_type scenario, 
  boolean enable
)
{
  cmgr_scenario_status_e status;
  cmgr_tech_type tech = CMGR_1X_TECH;
  cmgr_params input;
  memset(&input, 0,sizeof(cmgr_params));
  onex_cocnmgr_state_type other_transient_scenario;
  onex_cocnmgr_state_type other_steady_scenario;
  onex_m1x_rm_mcpm_scenario_type mcpm_scenario;
  onex_m1x_rm_scenario_type onex_stop_scenario;
  uint8 reason;

  RM_MSG( HIGH, "Updating conc manager, scenario: %d, enable: %d",
  	      scenario,
  	      enable);
  
  if( enable == TRUE && 
  	  onex_concmgr_get_req( scenario ) == TRUE)
  {
    RM_MSG( HIGH, "Already updated conc manager");
	return;
  }


  if(enable == FALSE)
  {
    scenario_params.scenario_e = onex_concmgr_map_scenario( scenario );

    if( scenario >= ONEX_CONCMGR_FIRST_TRANSIENT )
    {
      scenario_params.type = CMGR_TRANSIENT_END;
      onex_stop_scenario = onex_concmgr_backmap_fing_scenario( scenario );
      mcpm_scenario = m1x_rm_get_mcpm_scenario( onex_stop_scenario );
      m1x_rm_send_mcpm_stop_req( mcpm_scenario );
    }
    else
    {
      scenario_params.type = CMGR_STEADY_END;
    }
  }
  /*Enable transient scenario*/
  else if( scenario >= ONEX_CONCMGR_FIRST_TRANSIENT )
  {
    other_transient_scenario = onex_concmgr_get_other_trans_active();
  
    /*Start a new transient scenario*/
    if(other_transient_scenario == -1)
    {
      scenario_params.type = CMGR_TRANSIENT_START;
      scenario_params.scenario_e = onex_concmgr_map_scenario( scenario );
    }

    /*Update transient scenario*/
    else
    {
      scenario_params.scenario_e = onex_concmgr_map_scenario( other_transient_scenario );
      scenario_params.update_scenario_e = onex_concmgr_map_scenario( scenario );
      scenario_params.type = CMGR_TRANSIENT_UPDATE;

      /*Send MCVS stop*/
      onex_stop_scenario = onex_concmgr_backmap_fing_scenario( other_transient_scenario );
      mcpm_scenario = m1x_rm_get_mcpm_scenario( onex_stop_scenario );
      m1x_rm_send_mcpm_stop_req( mcpm_scenario );

      onex_concmgr_set_req(other_transient_scenario, FALSE);
    }
  }

  /*Enable steady Scenario*/
  else
  {
    other_steady_scenario = onex_concmgr_get_other_steady_active();

    /*Start a new steady scenario*/
    if(other_steady_scenario == -1)
    {
      scenario_params.type = CMGR_STEADY_START;
      scenario_params.scenario_e = onex_concmgr_map_scenario( scenario );
    }
	
    /*Update steady scenario*/
    else
    {
      scenario_params.scenario_e = onex_concmgr_map_scenario( other_steady_scenario );
      scenario_params.update_scenario_e = onex_concmgr_map_scenario( scenario );
      scenario_params.type = CMGR_STEADY_UPDATE;
      onex_concmgr_set_req(other_steady_scenario, FALSE);
    }  
  }
  
  reason = (uint8)srch_rx_get_client_trm_reason(TRM_1X);
  scenario_params.priority = cmgr_get_priority(CMGR_1X, (cmgr_reason_enum_t)reason );
  scenario_params.start_timestamp = qurt_sysclock_get_hw_ticks();
  scenario_params.flow_ctrl_time_msec = 0xFFFFFFFF;


/*Update input for CMGR*/
  input.scenario_e = scenario_params.scenario_e;
  input.priority = scenario_params.priority;
  input.flow_ctrl_time_msec = scenario_params.flow_ctrl_time_msec;
  input.start_timestamp = scenario_params.start_timestamp;
  input.type = scenario_params.type;  
  input.update_scenario_e = scenario_params.update_scenario_e;

  /* Update concurreny manager */
  status = cmgr_scenario_config(
          tech,
          &input
          );
  
  RM_MSG( HIGH, "Updated conc manager");
  
  if(status != CMGR_ACCEPTED)
  {
    ONEX_ERR_FATAL("Conc Manager did not return accept : status %d",status,0,0);
  }
  else 
  {
    RM_MSG( HIGH, "CONC MGR returned ACCEPTED");
    onex_concmgr_set_req(scenario, enable);
  }
 
}

/*=============================================================================

FUNCTION       ONEX_CONCMGR_CHECK_AND_SEND_STOP_REQ
DESCRIPTION    This API checks for any active scneario and stops it.


DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/
void onex_concmgr_check_and_send_stop_req( void )
{
  int i; /* Loop Variable */

  /*------------------------------------------------------------------*/

  /* Loop through all Scenarios */
  for( i = 0; i < ONEX_CONCMGR_MAX_STATES; i++ )
  {
    /* Check if we sent MCPM Start first for this MCPM Scenario */
    if( onex_concmgr_get_req( (onex_cocnmgr_state_type)i) == TRUE )
    {
      onex_concmgr_scenario_stop_req( (onex_cocnmgr_state_type)i );
    }
  }
}
/*=============================================================================

FUNCTION       ONEX_CONCMGR_SET_REQ
DESCRIPTION    This API sets value for scenario request.


DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/
void onex_concmgr_set_req
( 
  onex_cocnmgr_state_type scenario,
  boolean req_status
)
{
  /* Validate the mcpm scenario */
  if( scenario >= ONEX_CONCMGR_MAX_STATES )
  {
    RM_MSG( ERROR, 
            "Conc mgr : Ivalid MCPM Scenario:%d",
            scenario );
    return;
  }

  onex_concmgr_req[scenario].start_req_sent = req_status;

  RM_MSG( HIGH, 
          "Conc MGR Set Req: scenario:%d, start_req_sent:%d",
          scenario,
          onex_concmgr_req[scenario].start_req_sent );
}

/*=============================================================================

FUNCTION       ONEX_CONCMGR_GET_REQ
DESCRIPTION    This API returns if a scenario is active or not.


DEPENDENCIES   None

RETURN VALUE   boolean.

SIDE EFFECTS   None

=============================================================================*/
boolean onex_concmgr_get_req 
(
  onex_cocnmgr_state_type scenario
)
{
  /* Validate the mcpm scenario */
  if( scenario >= ONEX_CONCMGR_MAX_STATES )
  {
    RM_MSG( ERROR, 
            "Conc MGR: Ivalid MCPM Scenario:%d",
            scenario );
    return FALSE;
  }

  RM_MSG( HIGH, 
          "Conc MGR Get Req: MCPM scenario:%d, start_req_sent:%d",
          scenario,
          onex_concmgr_req[scenario].start_req_sent );

  return onex_concmgr_req[scenario].start_req_sent;
}

/*=============================================================================

FUNCTION       ONEX_CONCMGR_GET_OTHER_TRANS_ACTIVE
DESCRIPTION    This API returns any active transient scenario.

DEPENDENCIES   None

RETURN VALUE   onex_cocnmgr_state_type

SIDE EFFECTS   None

=============================================================================*/
onex_cocnmgr_state_type onex_concmgr_get_other_trans_active( void )
{
  int i; /* Loop Variable */

  /*------------------------------------------------------------------*/

  /* Loop through all Scenarios */
  for( i = ONEX_CONCMGR_FIRST_TRANSIENT; i < ONEX_CONCMGR_MAX_STATES; i++ )
  {
    /* Check if we sent MCPM Start first for this MCPM Scenario */
    if( onex_concmgr_get_req( (onex_cocnmgr_state_type)i) == TRUE )
    {
      return (onex_cocnmgr_state_type)i ;
    }
  }
 return (onex_cocnmgr_state_type)-1;
}


/*=============================================================================

FUNCTION       ONEX_CONCMGR_GET_OTHER_STEADY_ACTIVE
DESCRIPTION    This API returns any active steady scenario.

DEPENDENCIES   None

RETURN VALUE   onex_cocnmgr_state_type

SIDE EFFECTS   None

=============================================================================*/
onex_cocnmgr_state_type onex_concmgr_get_other_steady_active( void )
{
  int i; /* Loop Variable */

  /*------------------------------------------------------------------*/

  /* Loop through all Scenarios */
  for( i = 0; i < ONEX_CONCMGR_FIRST_TRANSIENT; i++ )
  {
    /* Check if we sent MCPM Start first for this MCPM Scenario */
    if( onex_concmgr_get_req( (onex_cocnmgr_state_type)i) == TRUE )
    {
      return (onex_cocnmgr_state_type)i ;
    }
  }
 return (onex_cocnmgr_state_type)-1;
}

/*=============================================================================

FUNCTION       ONEX_CONCMGR_SCENARIO_STOP_REQ
DESCRIPTION    This API sends stop request for a given scenario.


DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/
void onex_concmgr_scenario_stop_req
(
  onex_cocnmgr_state_type scenario
)
{
  RM_MSG( HIGH,
          "Conc MGR: Stop request for scenario: %d",
          scenario );
  onex_concmgr_scenario_config_req ( scenario, FALSE);
}

/*=============================================================================

FUNCTION       ONEX_CONCMGR_MAP_SCENARIO
DESCRIPTION    This API maps internal scenario to CONC MGR scenario.


DEPENDENCIES   None

RETURN VALUE   None.

SIDE EFFECTS   None

=============================================================================*/
uint32 onex_concmgr_map_scenario
(
  onex_cocnmgr_state_type scenario
)
{
  uint32 scenario_e = 0; 
  if( scenario == ONEX_CONCMGR_SLEEP)
  {
    scenario_e = CMGR_1X_SLEEP ;
  }
  else if( scenario == ONEX_CONCMGR_WAKEUP)
  {
    scenario_e = CMGR_1X_WAKEUP ;
  }
  else if( scenario == ONEX_CONCMGR_IDLE)
  {
    scenario_e = CMGR_1X_IDLE ;
  }
  else if( scenario == ONEX_CONCMGR_VOICE)
  {
    scenario_e = CMGR_1X_VOICE ;
  }
  else if( scenario == ONEX_CONCMGR_DATA)
  {
    scenario_e = CMGR_1X_DATA ;
  }
  else if( scenario == ONEX_CONCMGR_MEAS_1XTOL)
  {
    scenario_e = CMGR_1X_MEAS_1XTOL ;
  }
  else if( scenario == ONEX_CONCMGR_SRCH_FING_NOM)
  {
    scenario_e = CMGR_1X_SRCH_FING_NOM;
  }
  else if( scenario == ONEX_CONCMGR_SRCH_FING_MED)
  {
    scenario_e = CMGR_1X_SRCH_FING_MED;
  }
  else if( scenario == ONEX_CONCMGR_SRCH_FING_HIGH)
  {
    scenario_e = CMGR_1X_SRCH_FING_HIGH;
  }
  else if( scenario == ONEX_CONCMGR_SRCH_FING_NOM_MED)
  {
    scenario_e = CMGR_1X_SRCH_FING_NOM_MED;
  }
  else if( scenario == ONEX_CONCMGR_SRCH_FING_MED_NOM)
  {
    scenario_e = CMGR_1X_SRCH_FING_MED_NOM;
  }
  else if( scenario == ONEX_CONCMGR_SRCH_FING_HIGH_NOM)
  {
    scenario_e = CMGR_1X_SRCH_FING_HIGH_NOM;
  }
  else if( scenario == ONEX_CONCMGR_SRCH_FING_NOM_HIGH)
  {
    scenario_e = CMGR_1X_SRCH_FING_NOM_HIGH;
  }
  else if( scenario == ONEX_CONCMGR_SRCH_FING_MED_HIGH)
  {
    scenario_e = CMGR_1X_SRCH_FING_MED_HIGH;
  }
  else if( scenario == ONEX_CONCMGR_SRCH_FING_HIGH_MED)
  {
    scenario_e = CMGR_1X_SRCH_FING_HIGH_MED;
  }
  else
  {
    RM_MSG( ERROR, "Conc MGR: Invalid scenario");
  }

  RM_MSG( HIGH, 
          "Conc MGR: Passed Scenario: %d, Conc MGR scenario: %d",
          scenario,
          scenario_e);
  return scenario_e;
}
/*=============================================================================

FUNCTION       M1X_RM_GET_FING_SCENARIO
DESCRIPTION    This API maps given finger request to a scenario.

DEPENDENCIES   None

RETURN VALUE   onex_m1x_rm_scenario_type

SIDE EFFECTS   None

=============================================================================*/
onex_m1x_rm_scenario_type m1x_rm_get_fing_scenario
( 
  m1x_rm_mcvs_req_type *mcvs_req_data
)
{
  onex_m1x_rm_scenario_type onex_scenario = ONEX_M1X_RM_SCENARIO_NONE;

  if(mcvs_req_data->q6_freq  == M1X_RM_Q6_REQ_NOM)
  {
    if(mcvs_req_data->vpe_freq == M1X_RM_VPE_REQ_NOM)
    {
      onex_scenario = ONEX_M1X_RM_SRCH_FING_NOM;
    }
    else if(mcvs_req_data->vpe_freq == M1X_RM_VPE_REQ_MED)
    {
      onex_scenario = ONEX_M1X_RM_SRCH_FING_NOM_MED;
    }
    else if(mcvs_req_data->vpe_freq == M1X_RM_VPE_REQ_HIGH)
    {
      onex_scenario = ONEX_M1X_RM_SRCH_FING_NOM_HIGH;
    }
  }
  else if(mcvs_req_data->q6_freq  == M1X_RM_Q6_REQ_MED)
  {
    if(mcvs_req_data->vpe_freq == M1X_RM_VPE_REQ_NOM)
    {
      onex_scenario = ONEX_M1X_RM_SRCH_FING_MED_NOM;
    }
    else if(mcvs_req_data->vpe_freq == M1X_RM_VPE_REQ_MED)
    {
      onex_scenario = ONEX_M1X_RM_SRCH_FING_MED;
    }
    else if(mcvs_req_data->vpe_freq == M1X_RM_VPE_REQ_HIGH)
    {
      onex_scenario = ONEX_M1X_RM_SRCH_FING_MED_HIGH;
    }
  }
  else if(mcvs_req_data->q6_freq  == M1X_RM_Q6_REQ_HIGH)
  {
    if(mcvs_req_data->vpe_freq == M1X_RM_VPE_REQ_NOM)
    {
      onex_scenario = ONEX_M1X_RM_SRCH_FING_HIGH_NOM;
    }
    else if(mcvs_req_data->vpe_freq == M1X_RM_VPE_REQ_MED)
    {
      onex_scenario = ONEX_M1X_RM_SRCH_FING_HIGH_MED;
    }
    else if(mcvs_req_data->vpe_freq == M1X_RM_VPE_REQ_HIGH)
    {
      onex_scenario = ONEX_M1X_RM_SRCH_FING_HIGH;
    }
  }
  else
  {
    RM_MSG( ERROR, "Conc MGR: Invalid scenario");
  }
	
  return onex_scenario;
}

/*=============================================================================

FUNCTION       ONEX_CONCMGR_MAP_FING_SCENARIO
DESCRIPTION    This API maps given request to CONC MGR scenario.

DEPENDENCIES   None

RETURN VALUE   onex_cocnmgr_state_type

SIDE EFFECTS   None

=============================================================================*/
onex_cocnmgr_state_type onex_concmgr_map_fing_scenario
(
  onex_m1x_rm_scenario_type onex_scenario
)
{
  onex_cocnmgr_state_type cmgr_scenario = ONEX_CONCMGR_INVALID_STATE;
  
  if( onex_scenario == ONEX_M1X_RM_SRCH_FING_NOM)
  {
    cmgr_scenario = ONEX_CONCMGR_SRCH_FING_NOM;
  }
  else if( onex_scenario == ONEX_M1X_RM_SRCH_FING_MED)
  {
    cmgr_scenario = ONEX_CONCMGR_SRCH_FING_MED;
  }
  else if( onex_scenario == ONEX_M1X_RM_SRCH_FING_HIGH)
  {
    cmgr_scenario = ONEX_CONCMGR_SRCH_FING_HIGH;
  }
  else if( onex_scenario == ONEX_M1X_RM_SRCH_FING_NOM_MED)
  {
    cmgr_scenario = ONEX_CONCMGR_SRCH_FING_NOM_MED;
  }
  else if( onex_scenario == ONEX_M1X_RM_SRCH_FING_MED_NOM)
  {
    cmgr_scenario = ONEX_CONCMGR_SRCH_FING_MED_NOM;
  }
  else if( onex_scenario == ONEX_M1X_RM_SRCH_FING_HIGH_NOM)
  {
    cmgr_scenario = ONEX_CONCMGR_SRCH_FING_HIGH_NOM;
  }
  else if( onex_scenario == ONEX_M1X_RM_SRCH_FING_NOM_HIGH)
  {
    cmgr_scenario = ONEX_CONCMGR_SRCH_FING_NOM_HIGH;
  }
  else if( onex_scenario == ONEX_M1X_RM_SRCH_FING_MED_HIGH)
  {
    cmgr_scenario = ONEX_CONCMGR_SRCH_FING_MED_HIGH;
  }
  else if( onex_scenario == ONEX_M1X_RM_SRCH_FING_HIGH_MED)
  {
    cmgr_scenario = ONEX_CONCMGR_SRCH_FING_HIGH_MED;
  }
  else
  {
    RM_MSG( ERROR, "Conc MGR: Invalid scenario");
  }
    RM_MSG( HIGH, 
          "Conc MGR: Passed Scenario: %d, Conc MGR scenario: %d",
          onex_scenario,
          cmgr_scenario);
  return cmgr_scenario;
}

/*=============================================================================

FUNCTION       ONEX_CONCMGR_BACKMAP_FING_SCENARIO
DESCRIPTION    This API maps given CONC MGR scenario to internal scenario type.

DEPENDENCIES   None

RETURN VALUE   onex_m1x_rm_scenario_type

SIDE EFFECTS   None

=============================================================================*/
onex_m1x_rm_scenario_type onex_concmgr_backmap_fing_scenario
(
  onex_cocnmgr_state_type cmgr_scenario
)
{
  onex_m1x_rm_scenario_type onex_scenario = ONEX_M1X_RM_SCENARIO_NONE;
  if( cmgr_scenario == ONEX_CONCMGR_SRCH_FING_NOM)
  {
    onex_scenario = ONEX_M1X_RM_SRCH_FING_NOM;
  }
  else if( cmgr_scenario == ONEX_CONCMGR_SRCH_FING_MED)
  {
    onex_scenario = ONEX_M1X_RM_SRCH_FING_MED;
  }
  else if( cmgr_scenario == ONEX_CONCMGR_SRCH_FING_HIGH)
  {
    onex_scenario = ONEX_M1X_RM_SRCH_FING_HIGH;
  }
  else if( cmgr_scenario == ONEX_CONCMGR_SRCH_FING_NOM_MED)
  {
    onex_scenario = ONEX_M1X_RM_SRCH_FING_NOM_MED;
  }
  else if( cmgr_scenario == ONEX_CONCMGR_SRCH_FING_MED_NOM)
  {
    onex_scenario = ONEX_M1X_RM_SRCH_FING_MED_NOM;
  }
  else if( cmgr_scenario == ONEX_CONCMGR_SRCH_FING_HIGH_NOM)
    {
    onex_scenario = ONEX_M1X_RM_SRCH_FING_HIGH_NOM;
    }
  else if( cmgr_scenario == ONEX_CONCMGR_SRCH_FING_NOM_HIGH)
  {
    onex_scenario = ONEX_M1X_RM_SRCH_FING_NOM_HIGH;
  }
  else if( cmgr_scenario == ONEX_CONCMGR_SRCH_FING_MED_HIGH)
  {
    onex_scenario = ONEX_M1X_RM_SRCH_FING_MED_HIGH;
  }
  else if( cmgr_scenario == ONEX_CONCMGR_SRCH_FING_HIGH_MED)
  {
    onex_scenario = ONEX_M1X_RM_SRCH_FING_HIGH_MED;
  }
  else
  {
    RM_MSG( ERROR, "Conc MGR: Invalid scenario");
  }
    RM_MSG( HIGH, 
          "Conc MGR: onex Scenario: %d, Conc MGR scenario: %d",
          onex_scenario,
          cmgr_scenario);
  return onex_scenario;  
}