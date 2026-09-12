#ifndef WL1TRM_H
#define WL1TRM_H
/*===========================================================================

                    L 1   T R M   A N D   T C X O   M A N A G E R

DESCRIPTION
  This file contains global declarations and external references
  for the L1 TRM and TCXO Manager.

EXTERNALIZED FUNCTIONS

  wl1_trm_request_primary_antenna
    This function requests TRM for the primary antenna before WCDMA can 
    attempt to acquire.

  wl1_trm_debug_delay
    This function generates a delay in msec for debugging purposes

  wl1_tcxo_set_restriction
    Invoked due to either a restriction change from TCXO Mgr, or when error becomes
    large and UMTS is breaking the TCXO Mgr restriction or we are freezing the trk_lo
    for debugging purposes. This simply posts a local command which does all the work.
    Necessary because we cant call tcxomgr_report() from interrupt context.

  wl1_tcxo_set_restriction_cmd
    Generally invoked by the local command TCXO_SET_RESTRICTION_CMD to actually 
    perform the freezing or unfreezing of the TCXO. This function needs send 
    acks (when TCXO Mgr gives us a new restriction) and reports (when breaking 
    a restriction) to TCXO Mgr when necessary. Can also be invoked directly,
    if we cannot post a local command and wait for it at the moment.

  wl1_tcxo_resume_restriction
    Resumes current restriction after WL1 diverges from the restriction due to error
    in the VCO.

  wl1_tcxo_release
    Releases control of the TCXO.

  wl1_tcxo_get_vco
    This function returns the vco value from the mdsp.

  wl1_tcxo_frozen
    Returns if the tcxo is currently frozen or not.

  wl1_tcxo_rpush
    Performs the actual rotator push when using TCXO Mgr 3.0

  wl1_tcxo_request
    This function requests TCXO Mgr for control of the VCTCXO.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2000-2012 by Qualcomm Technologies, Inc. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1trm.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/10/20   ac      Mini dump framework checkin
05/16/18    ak      Introduced new MUTEX to protect APS code
02/19/18   jb      API to determine if the other is LTE 10ms before FMO starts
01/07/16   ar      Support for stack activation ind to MCS
08/11/17   sch     WCDMA related changes for PBR redesign
1/18/17    jb      Added API for state register and state update with TRM.
11/21/16   th      API to find if SRLTE is enabled or not
11/18/16   pg      Fix for resetting the APS variable properly.
08/15/16   pvr     Feature APS Changes for 1xSRLTE+W
08/29/16   pg      Fix to est req and lock wait timeout from RRC if both the 
                    clients in lock requested state.
07/28/16   ad      Backout Mods to add EF if we are unable to trigger sleep for 300ms
07/20/16   pg      Made changes to update the duty cycle related variables when W gets the grant.
06/27/16   pg      Mods to add EF if we are unable to trigger sleep for 300ms
05/24/16   sl      AGC log packet changes for WplusW
04/21/16   ad      Made changes to update client info in wrm db while moving the 
                   client to lock req
03/15/16   ad      Deny the grant for measurements if there is any other lock request
04/01/16   ar      Support to reject meas only gaps if we are pending call setup
03/23/16   ad      Made changes to invert the priority during SIB reading only if W is at CM priority
02/09/16   skk     Dont request/reserve for trm lock if Rx cmd is pending(Tx is in lock req_state)
02/08/16   gm      mods to update max priority inversion timer.
01/07/16   ad      Made changes to provide API to RRC to check if L1 client has lock
                   for serving cell or not
12/07/15   vn      Function to check if WCDMA is holding TRM Lock with SR Reason
11/20/15   ad      Mods to release TX lock when UL channels are dropped
10/28/15   ad      Made changes to release TX lock if RRC is not in lock at start
10/28/15   gm      Modified check for valid resource.
10/15/15   sad     Remove usage of wrm_primary_rf_device_supports_bandx api
10/22/15   abs     Mods to support ACQ in DR-DSDS mode
10/16/15   rn      Extended TRM lock duration for Raw scan
10/08/15   ad      Made changes to stop wrm timeout timer on aborting split acq srch
10/05/15   ar      Changes to unify the WRM mutex list
09/29/15   pvr     DBDC QCTA changes
09/18/15   ad      Changes to release lock at stop wcdma req
09/13/15   ar      Remove fake grant framework as we support ISS in SBCH
09/11/15   sad     Update prev Tx device for every cphy request
09/07/15   ad      Made changes to set only the required variables in set_pending_cmd
09/02/15   gm      To start wrm_timeout_timer for split acq.
09/01/15   us      Added wrm_resource_enum_type for ATLAS
08/31/15   ar      Ensure that previous Tx device is not invalidated prior to Exit mode
08/26/15   us      Updates for including rfm_path_type
08/18/15   abs     Changes to support new Modify activity interface
08/21/15   ar      Add prototype for lock extension with new activity
08/21/15   abs     New API to query TRM if alt sub is in data
08/19/15   sad     DBDC + Rx/Tx split feature check-in
08/21/15   ac      Update wl1_trm_get_curr_band_for_lna_switch() to take device as parameter
08/21/15   gm      Don't allow priority inversion if timer expires in invalid WRM lock state.
08/12/15   ar      Add a macro to get non-inverted reason for activity
08/17/15   pg      Mods to fix the WRM_DOES_ACTIVITY_NEED_TX API. 
08/04/15   vs      Changes related to the InterF and IRAT wrm activities.
08/04/15   ar      Modify WRM_GET_REASON_FROM_ACTIVITY macro to get inverted reason if
                   priority inversion is active
08/11/15   pvr     Handle secondary unlock timer expiry in task context
07/28/15   gj      Declared wrm_primary_rf_device_supports_band as extern
07/15/15   pv      Pass the SUB ID for DR query API.
07/17/15   pvr     QDTA changes
07/16/15   ar      TRM-U Phase 3 changes
07/15/15   pvr     Fix KW error
07/15/15   pvr     Allow measurements in X2W QTA gap without TRM notification
                   if WRM resource is already holding lock for requested bands.
07/14/15   pg      Mods for doing Modify Band during LFS.
07/09/15   bj      Added macro for FS lock req
07/08/15   ar      Changes to use SYS.h enum instead of TRM enum for sub-reason logging
07/08/15   ar      TRM-U Phase-3 changes
07/08/15   amj     RF Power clock on/off changes for Bolt W+W.
07/02/15   bj      Increase the split acq step1+NASTT step lock duration from  80 to 85 ms 
                   and boost the clock level to NOM for NASTT search in ACQ state to avoid 1x page miss.
07/02/15   gsk     Removing unused macro
06/25/15   ar      Revert changes to use TRM sub-reason
06/25/15   gm      To allow new req_n_not even if Tx is already in LOCK_REQ state.
06/13/15   ar      Changes to not set RRC to IN_LOCK status when we issued a fake grant.
06/13/15   gm      Mods to check if client's activity is valid or not.
04/21/15   bj      Split ACQ changes for W+1x.
05/27/15   gj      Defined lock times for different kinds of ACQ procedures
05/26/15   gm      mods to add support for TRM sub reason.
05/25/15   kr      correcting TRM priorities
12/12/14   skk     Change TRM priority if BCCH wakesup before sleep after PICH wakeup. 
05/20/15   sks     Compilation error fix.
05/19/15   sad     QTA cleanup code for idle tech
05/19/15   ar      Add macro for getting activities that need PRIO_INV
05/19/15   sad     Changes to support modify band returning new device
                   for Rx/ Tx split
05/15/15   abs     Featurizing TRM_UMTS2_SECONDARY client usage until
                   MCS support comes in
05/01/15   kr      FR 26325: CB/BMC support for W/G/L+G MSIM DSDS
05/07/15   skk     FR24494: BCH Inter SIB sleep initial check in.
05/06/15   abs     RX/TX Split support
04/20/15   dm      Acq priority inversion for TRM unification.
04/01/15   sad     QTA cleanup code
03/16/15   gm      mods to remove WL1_DS_IS_TRM_SIG_PENDING_ON_SLEEP() check.
02/25/15   ar      Changes to avoid posting fake drx cmd during cphy operations
02/28/15   rsr     W + W Phase 2 Checkin.
02/20/15   qj      Extern request resource API to fix compilation warning
11/7/14    sks     Call exit mode on PRx based on whether HW supports DR operation, not on the instantaneous mode.
02/06/15   rs      Code changes for requesting lock with new proc during BCH decode
01/29/14   pr      WL1M global reorg for WplusW
01/23/15   sad     Global re-org for W+W
01/23/15   abs     Global re-org for W+W
01/05/15   abs     DSDS: Send BAND_TUNE_CONFIRM to TRM in response to all
                   TRM_MODIFY_BAND requests upon receiving a TRM_GRANT
12/17/14   gm      To implement changes to inform TRM if div is needed for measurement.
12/03/14   ar      Add new macros for checking if any cmd is pending and a macro 
                   for checking if an activity needs TX.
12/02/14   ar      Add wrapper wl1idletamgr lock req api wrapper for RRC to call
11/25/14   sad     Change to handle modify activity failure
10/31/14   sad     0x41c1 log packet changes
11/04/14   sad     Cleanup of lock req for freq scan
11/05/14   ar      Add new macro to get the RF device ID for a resource
10/14/14   sd      new APIs for DR to SR fallbacks
10/10/14   sd      Remove l1m_wait_for_trm_stop and related code
10/07/14   rsr     Added Change to Extern Func. WRM Request Resources for Sec.
10/07/14   sd      Fixed compilation warnings
09/24/14   gm      Mods to wait for TRM grant to update freq.
09/26/14   dm      DSDS: Indicate the minimum time needed for unlock when changing the activity.
09/23/14   pv      Support for TRM swapping of chains for call setup in DR mode.
                   Added MACRO to check if CMD PENDING for call setup.
09/04/14   abs     Feature cleanup for Bolt
08/24/14   rsr     Added Changes to Ensure that Sec. Chain is not granted to RxD
                   erroneously, and ensure that WRM Pending request is invalidated
                   once granted, also ensured RxD Grant callback is called once 
                   Pending Request is handled.
08/18/14   skk     Request for lock for CELL_SELECTION_REQ in G2W TA mode as lock can be released as a part of SIB sleep.
08/12/14   sd      Fixed DSDS Medium and Low compiler warnings in WCDMA.
08/04/14   rs      Fixed compiler warnings
07/26/14   rkmk    Reduce image size by using right macro depending upon number of parameters
07/25/14   as      Adding secondary antenna activities.
07/08/14   dm      Fixes related to DR DSDS bring up from multisim side.
07/02/14   gm      Fixes related to DR DSDS bring up from multisim side.
06/19/14   dm      DR-DSDS first cut.
06/17/14   jd      Fix to return floored rxagc when trm is not reserved.
05/06/14   as      Moving WRM to WL1TRM.
04/28/14   sm      Internal Unlock API for Secondary
04/24/14   as      Moving wrm code to wl1trm.
04/18/14   as      DSDS Code Reorganization.
04/16/14   bj      Split ACQ code cleanup
04/03/14   jkb     Use WRM state instead of procedure mask to check lock status
04/02/14   dm      DSDS: New API to make tuneaway duration read from RRC available to 
                   all L1 modules.
02/12/14   sad     DB-3C API check in
04/01/14   rs      Externed a function to request lock for secondary chain
03/27/14   pg      To avoid back to back reservation with FRM in case tuneaway timer 
                   expires in parallel in case FACH QTA
02/14/14   qj      Changes for W diversity antenna sharing during PCH_SLEEP
03/27/14   sm      OMRD changes to make RxD trm compliant
03/24/14   as      Delaying call to dl_cell_check_tddet_status to after closing the QTA gap.
03/24/14   as      Changing wrm_unlock_trm_cb api to get additional data from trm.
01/27/14   as      Implemented Version 2 of the W2G tuneaway log packet WL1TRM_TUNEAWAY_LOG_PKT.
01/22/14   pkg     EUL optimization to avoid HICH result falling into QTA gap and improved RSN management for QTA.
01/22/14   gm      post a local command to handle unlock cancel event. 
01/21/14   abs     Cleaning up DSDA~CXM(FR:2334) changes
01/20/14   ar      Add TRM request support for W->L searches
01/17/14   jkb     Adding lock check to WL1_IN_DS_WITH_ANY_LOCK and 
                   WL1_IN_DS_WITH_NO_LOCK 
01/17/14   raj     Added changes for raising W UL Priority to CXM for SRB Data.
01/13/14   jkb     Changing WL1_IN_DS_WITH_ANY_LOCK and WL1_IN_DS_WITH_NO_LOCK
01/07/14   as      Added data structure and external funtions to support
                   WL1TRM_TUNEAWAY_LOG_PKT log packet.
12/16/13   as      Porting G2W TA from Triton to Dime.
12/06/13   ar      Modified definition wl1_ds_pre_meas_start_ind
11/15/13   mk      DSDS: Handling CPHY_DRX_REQ during sleep.
11/14/13   as      W2G TA: Adding code for RRC dependency.
10/29/13   dm      DSDS: Use WRM state to check if L1 has lock.
09/29/13   as      Continuing port of Dual Sim from Triton to Dime.
09/23/13   as      Porting Dual Sim from Triton to Dime.
09/12/13   jd      Searcher redesign
08/28/13   bs      Resolved KW issues
08/12/13   cc      DBDC CM W2W feature check in
07/24/13   jd      Changes for Dual band Dual carrier
01/11/13   scm     More partial FEATURE_DUAL_SIM support for DSDA.
12/10/12   scm     Partial FEATURE_DUAL_SIM support for DSDA.
12/07/12   scm     Pass trm_get_rf_device() return value into RFM APIs.
08/13/12   jd      Trm changes on dime.
01/30/12   vs      Feature cleanup.
06/30/10   scm     Reserve TCXO control after WFW download.
05/13/09   scm     Implement wl1_trm_set_starting_trk_lo().
10/03/08   hk      Bringing in 7k Mailine fixes
04/17/07   kps     Add wl1_tcxo_resume_restriction()
04/10/07   kps     Fully featurize file under the TRM/TCXOMGR30 features
03/09/07   kps     change the return type for wl1_tcxo_rpush()
01/19/06   kps     Added support for Rx Diversity
12/15/06   kps     Delete wl1_tcxo_freeze_request() and wl1_controls_tcxo().
11/21/06   kps     Add wl1_tcxo_freeze_request() and wl1_controls_tcxo()
11/13/06   kps     TCXO Rotator push support
                   Add wl1_tcxo_rpush()
11/06/06   kps     Add wl1_tcxo_release().
10/27/06   kps     Added support for setting restrictions in TCXOMGR 3.0
10/13/06   kps     Added basic support for TCXOMGR3.0, under FEATURE_WCDMA_TCXOMGR30
10/02/06   kps     Added declaration for wl1_trm_stop_cmd.
09/11/06   kps     Created file. Declaration of wl1_trm_request_primary_antenna.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "comdef.h"
#include "rfm.h"
#include "trm.h"
#include "l1rrcif.h"
#ifdef FEATURE_QSH_MDUMP
#include "wl1_qsh_ext.h"
#endif

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/
/*Initialization macros*/
#define WRM_INVALID_TIME_DURATION 0
#define WRM_INVALID_RES_AT_TIME 0
#define WRM_INVALID_TAG 0xFF
/*** maximum channels in a particular band ***/
#define MAX_CHANNELS_IN_BAND 352
#define WRM_MAX_CLIENTS 3
#define WRM_MAX_BANDS MAX_BAND_INFO 
/* Initial duration of trm request for primary antenna */
#define WL1_TRM_RXTX_MINIMUM_DURATION  TIMETICK_SCLK_FROM_MS(4000)
/* MACROS for Default and Invalid Measurement IRAT ID's */
#define WRM_DEFAULT_MEAS_IRAT_ID 0XFFFF


/*Lock time for frequency scan in ms*/
#define WL1_TRM_FS_LOCK_TIME_MSEC 100

/*Lock time for regular acquisition in ms*/
#define WL1_TRM_ACQ_LOCK_TIME_MSEC 100

/*Lock time for split acquisition in ms*/
#ifdef FEATURE_WCDMA_TABASCO_MODEM     
#define WL1_TRM_SPLIT_ACQ_LOCK_TIME_MSEC 85 /*step1(30) + NASTT(40) +SW delay(15) ms with NOM clock level*/
#else
#define WL1_TRM_SPLIT_ACQ_LOCK_TIME_MSEC 80
#endif

/*Lock time for Raw Scan in ms*/
#ifdef FEATURE_WCDMA_TABASCO_MODEM
#define WL1_TRM_RAW_SCAN_LOCK_TIME_MSEC 180
#else
#define WL1_TRM_RAW_SCAN_LOCK_TIME_MSEC 160
#endif

/*Lock time for Fine Scan in ms*/
#define WL1_TRM_FINE_SCAN_LOCK_TIME_MSEC 65

/*Lock time for Additional Channel Scan in ms*/
#define WL1_TRM_ADD_CHNL_SCAN_LOCK_TIME_MSEC 100

typedef trm_duration_t wrm_duration_type;
typedef trm_time_t wrm_time_type;
typedef trm_frequency_info wrm_frequency_info;
typedef trm_channel_t wrm_channel_type;
typedef trm_unlock_event_enum_t wrm_unlock_event_enum_type;
typedef trm_irat_identifier wrm_irat_identifier;
typedef trm_client_enum_t wrm_client_enum_type;
typedef trm_set_client_state_input_type wrm_set_client_state_input_type;
typedef trm_set_client_state_output_data wrm_set_client_state_output_data;
typedef trm_state_oper_enum_type wrm_state_oper_enum_type;
typedef trm_tuneaway_enum_type wrm_tuneaway_enum_type;

/****************************************************************************
                CALLBACK FUNCTION DECLRATIONS FOR MODULE
****************************************************************************/
/*============================================================================

CALLBACK WRM_GRANT_CALLBACK_TYPE

DESCRIPTION
  The prototype for event callback functions, used by WRM 
  to inform the clients of a grant.
  
DEPENDENCIES
  The callback will be called by WRM. It will called from L1 task context.

RETURN VALUE
  True - If grant is available.
  False - This is not possible at this point. 

SIDE EFFECTS
  None

============================================================================*/
typedef void (*wrm_grant_callback_type)(
  boolean grant,
  wsub_id_e_type
);

/****************************************************************************
                ENUMS
****************************************************************************/
#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*   WRM Resources */
typedef enum
{
  WCDMA_TRM,
  WCDMA_TRM_TX,
  WCDMA_TRM_SECONDARY,
  WCDMA_TRM_CA,
  WCDMA_TRM_SECONDARY_CA,
  WCDMA_TRM_NUM_RESOURCES
} wrm_resource_enum_type;
#else  /*(FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED)*/
/*   WRM Resources */
typedef enum
{
  WCDMA_TRM,
  #ifdef FEATURE_WCDMA_RX_TX_SPLIT
  WCDMA_TRM_TX,
  #endif
  WCDMA_TRM_SECONDARY,
  WCDMA_TRM_CA,
  WCDMA_TRM_SECONDARY_CA,
  WCDMA_TRM_NUM_RESOURCES
} wrm_resource_enum_type;
#endif   /*(FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED)*/

/* WRM Primary Resource Clients */
typedef enum
{
  WRM_PRIMARY_CLIENT_L1,
  WRM_PRIMARY_CLIENT_RRC,
  WRM_PRIMARY_CLIENT_MAX
} wrm_primary_client_enum_type;

/* WRM Secondary Resource Clients */
typedef enum
{
  WRM_SECONDARY_CLIENT_RXD,
  WRM_SECONDARY_CLIENT_MEAS,
  WRM_SECONDARY_CLIENT_NBR,
  WRM_SECONDARY_CLIENT_MAX
} wrm_secondary_client_enum_type;


/*Enum to identify WRM state*/
typedef enum {
WRM_NO_LOCK_STATE,
WRM_LOCK_REQ_STATE,
WRM_LOCK_RES_STATE,
WRM_IN_LOCK_STATE,
WRM_UNLOCK_IN_PROGRESS_STATE,
WRM_STATE_MAX
} wrm_state_enum_type;

/*Enum to identify client state in WRM*/
typedef enum {
WRM_CLI_NO_LOCK_STATE,
WRM_CLI_LOCK_REQ_STATE,
WRM_CLI_LOCK_RES_STATE,
WRM_CLI_IN_LOCK_STATE,
WRM_CLI_UNLOCK_IN_PROGRESS_STATE,
WRM_CLI_STATE_MAX
} wrm_client_state_enum_type;

/**Commands posted by WRM for which TRM will respond 
 **through an async callback**/
typedef enum {
  /*WRM posted request_and_notify command to TRM and waits for grant*/
  WRM_REQUEST_AND_NOTIFY,
  /*WRM posted modify chain state with param duration. Should get immediate grant or denial.*/
  WRM_MODIFY_DURATION,
  /*WRM posted modify chain state with param reason. Can get immediate or delayed grant.*/
  WRM_MODIFY_REASON,
  /*WRM got grant pending indicator. So it would expect a grant later.*/
  WRM_MODIFY_REASON_PENDING,
  /*WRM posted modify chain state with param band. Can get immediate or delayed grant.*/
  WRM_MODIFY_BAND,
  /*WRM got grant pending indicator. So it would expect a grant later.*/
  WRM_MODIFY_BAND_PENDING,
  /*WRM posted TRM exchange command and waits for a grant.*/
  WRM_EXCHANGE,
  WRM_MAX_COMMANDS
} wrm_command_enum_type;

typedef enum {
  WRM_REQUEST_FUNC,
  WRM_REQ_AND_NOTIFY_FUNC,
  WRM_RESERVE_AT_FUNC,
  WRM_RELEASE_FUNC,
  WRM_UNLOCK_TRM_CB_FUNC,
  WRM_RETAIN_LOCK_FUNC,
  WRM_MODIFY_DURATION_FUNC,
  WRM_MODIFY_ACTIVITY_FUNC,
  WRM_MODIFY_FREQ_FUNC,
  WRM_ASYNC_CALLBACK_FUNC,
  WRM_LOG_ENABLED_NUM_FUNCTIONS
} wrm_functions_log_enabled_enum_t;

/*Type of TuneAway's handled by secondary antenna
 WRM_DTA: Diversity Tuneaway. RxD is forced to stop and diversity antenna is handovered to X tech.
 WRM_QDTA: Quick Diversity Tuneaway. RxD is paused before handovering diversity antenna to X tech and resumed after QDTA gap*/
typedef enum {
  WRM_DTA,
  WRM_QDTA,
  WRM_TA_MAX
} wrm_secondary_ta_enum_type;

typedef enum {
  WCDMA_STACK_STATE_ACTIVE,
  WCDMA_STACK_STATE_SUSPENDED,
  WCDMA_STACK_STATE_STOPPED
} wrm_stack_status_enum_type;

/*APS (Adaptive Page skipping) client Ids*/
typedef enum
{
  WRM_APS_CLIENT_UL,
  WRM_APS_CLIENT_DL,
  WRM_APS_CLIENT_RRC,
  WRM_APS_CLIENT_IDLE
} wrm_aps_client_enum_t;


/*APS registration state*/
typedef enum
{
  WRM_APS_NOT_REGISTERED,
  WRM_APS_IDLE,
  WRM_APS_CONNECTED,
}wrm_aps_register_state_t;

/****************************************************************************
                WRM LOG PACKET DEFINITIONS
****************************************************************************/
/*Structure to hold WRM LOG PACKET information*/
typedef PACKED struct PACKED_POST {
  uint8 wrm_function_name;
  uint8 wrm_state;
  uint8 wrm_resource;
  uint8 wrm_client;
  uint8 wrm_primary_client_state;
  uint8 wrm_secondary_client_state;
  uint8 wl1_trm_primary_rf_device;
  uint8 wl1_trm_secondary_rf_device;
  uint8 activity;
  uint8 req_resource;
  uint8 wrm_req_reason;
  uint8 band;
  uint8 async_cb_type;
  trm_duration_t wrm_dur_sclk;
  trm_time_t wrm_time_sclk;
  uint8 grant_status;
  uint8 unlock_request_by_trm_client;
  uint8 unlock_reason;
  uint8 unlock_event;
} wrm_msg_log_pkt_struct_type;

/* Define the WRM_MSG_LOG_PKT log packet */
LOG_RECORD_DEFINE(WRM_MSG_LOG_PKT)
  /* Version number of this log packet */
  uint8 version;

  /* The data of the log packet */
  wrm_msg_log_pkt_struct_type log_data;
LOG_RECORD_END

/****************************************************************************
                STRUCTS
****************************************************************************/
/* Structure for each activity's resource and reason */
typedef struct
{
  trm_resource_enum_t resource;
  /*TRM reason for the current activity*/
  trm_reason_enum_t reason;
  /*sub reason to indicate the functional/protocol activity*/
  sys_proc_type_e_type sub_reason;
  /*TRM inversion reason for the current activity if applicable, else TRM_NO_REASON*/
  trm_reason_enum_t inv_reason;
}wrm_res_rsn_for_activity_type;

/*structure for holding Band information ***/
typedef struct
{
  sys_band_class_e_type band;
  uint16 min_channel_number;
  uint16 max_channel_number;
} trm_band_map_struct_type;


/*Frequency Info*/
typedef struct
{
  uint32             num_bands;
  wrm_frequency_info    bands[MAX_BAND_INFO];
} wrm_freq_input_type;

/*Structure to hold L1 and RRC specific information in WRM*/
typedef struct {
wrm_client_state_enum_type cli_state;

trm_duration_t req_dur;

trm_time_t res_starts_at;

trm_duration_t res_dur;

wrm_activity_enum_type  req_activity;

wrm_freq_input_type band_info;

wrm_tuneaway_enum_type ta_preference;

boolean  pending_req_for_ext_cmd;

wrm_grant_callback_type grant_cb;

} wrm_client_info_struct_type;

typedef struct {

  boolean valid;
  
  wrm_resource_enum_type resource;
  /**ID of the client requesting resource**/
  uint8 client_id;

  /**Activity for which resource is requested**/
  wrm_activity_enum_type activity;

  /**Bands for which resource is requested**/
  wrm_freq_input_type band_info;

  /**Duration for which resource is requested**/
  wrm_duration_type duration;

  /**Grant callback of the client**/
  wrm_grant_callback_type grant_callback;
  
  /**Flag to indicate if RF device swap is to be performed**/
  boolean wl1_device_swap_pending;
  
  /*Flag to indicate fake drx cmd is pending to be posted as CPHY was in progress during swap attempt.*/
  boolean wl1_fake_drx_pending;

  /*Flag to indicate that rx(i.e WCDMA_TRM resource) cmd is pending*/
  boolean wl1_rx_cmd_pending;

  /*Modify Frequency Band Info*/
  l1_freq_scan_band_enum_type mod_freq_band;
} wrm_cmd_struct_type;

typedef struct {
  trm_async_callback_type_enum_t cb_type;

  trm_async_callback_data data;

} wrm_rsp_struct_type;

/*Structure to hold WRM information*/
typedef struct {

  /**Current WRM state**/
wrm_state_enum_type wrm_state;

  /**Bitmask of active clients**/
  uint8 wrm_active_client_bmsk;

  /** Info about each client's state and request parameters**/
  wrm_client_info_struct_type wrm_client_info[WRM_MAX_CLIENTS];

  #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
  rfm_path_type rfm_path;
  #else
  /**RFM Device ID allocated for this resource**/
  rfm_device_enum_type device_id;
  #endif
  
  /**Bands currently allocated for this resource.**/
  wrm_freq_input_type band_info;

  /**Details of a WRM request and notify kept pending by WRM state machine.**/
  wrm_cmd_struct_type wrm_pending_cmd;

  /**Command posted by WRM to TRM for which WRM is waiting for a response.**/
  wrm_command_enum_type trm_cmd_posted;

  wrm_rsp_struct_type trm_cmd_posted_rsp;

} wrm_resource_info_struct_type ;

typedef struct {
  /**Number of active measurements**/
  uint8 num_active_measurements;
} wrm_meas_info_struct_type;

typedef struct {
  wrm_resource_info_struct_type wrm_resource_info[WCDMA_TRM_NUM_RESOURCES];
  wrm_meas_info_struct_type meas_db;
  
  uint64 wrm_wakeup_identifier;
} wrm_db_struct_type;

typedef struct {
  /**Structure to store current Votes for APS**/
  uint8 bmsk;
  wrm_aps_register_state_t reg_state;
  boolean wrm_current_conn_mode_aps_state;
  boolean wrm_current_idle_mode_aps_state;
} wrm_aps_db_struct_type;

typedef struct
{
  wrm_unlock_event_enum_type unlock_event;
  wrm_secondary_ta_enum_type ta_type;
}wrm_secondary_unlock_cb_data;

typedef struct {
  uint8 unlock_time_ms;
  boolean (*unlock_cb)(wrm_secondary_unlock_cb_data *unlock_data_ptr,wsub_id_e_type wsub_id);
}wrm_unlock_cb_type;

typedef struct
{
  /* mutex WRM calls*/
  rex_crit_sect_type wl1_res_crit_sect;
  /* Secondary unlock timer */
  timer_type wrm_sec_unlock_timer;
  /*mutex to protect APS variables*/
  rex_crit_sect_type wl1_aps_crit_sect;
  /*To store APS related information*/
  wrm_aps_db_struct_type wrm_aps_db;
  wrm_db_struct_type wrm_db;
}wl1trm_ext_cntrl_params_type;

typedef struct
{
  /* The structure variable that stores all the required data for WRM_MSG_LOG_PKT*/
  wrm_msg_log_pkt_struct_type wrm_log_pkt_db;

  /* Both variables are valid only within CPHY processing */
#ifdef FEATURE_WCDMA_RX_TX_SPLIT
  #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
  /* Needed in case Rx req for band_change fails and we need to revert to the old path */
  rfm_path_type prev_tx_rfm_path;
  /* Needed in case of single sim if the same band req in modify_band results in a different
   * device being granted to us */
  rfm_path_type prev_rx_rfm_path;
  #else /*(FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED)*/
  /* Needed in case Rx req for band_change fails and we need to revert to the old device */
  rfm_device_enum_type prev_tx_device_id;
  /* Needed in case of single sim if the same band req in modify_band results in a different
   * device being granted to us */
  rfm_device_enum_type prev_rx_device_id;
  #endif /*(FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED)*/
#endif /* FEATURE_WCDMA_RX_TX_SPLIT */
}wl1trm_int_cntrl_params_type;

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
extern wl1trm_ext_cntrl_params_type wl1trm_ext_cntrl_params[WCDMA_NUM_SUBS];

/* WRM mutex for WL1 */
#define WL1_RES_MUTEXLOCK()   REX_ISR_LOCK(&(wl1trm_ext_cntrl_params[wsub_id].wl1_res_crit_sect))
#define WL1_RES_MUTEXFREE()   REX_ISR_UNLOCK(&(wl1trm_ext_cntrl_params[wsub_id].wl1_res_crit_sect))

/*APS mutext for WL1 */
#define WL1_APS_MUTEXLOCK()   REX_ISR_LOCK(&(wl1trm_ext_cntrl_params[wsub_id].wl1_aps_crit_sect))
#define WL1_APS_MUTEXFREE()   REX_ISR_UNLOCK(&(wl1trm_ext_cntrl_params[wsub_id].wl1_aps_crit_sect))


/*Macro to check validity of TRM Client Id*/
/*MCS incorrectly checked in TRM_UMTS2_SECONDARY client as
**TRM_UMTS_SECONDARY2 which they are correcting in the next
**release, featurizing our code for now*/
#ifdef FEATURE_DUAL_WCDMA
  #ifdef FEATURE_WCDMA_RX_TX_SPLIT
  #define WRM_IS_TRM_CLIENT_ID_VALID(client_id) \
           ( (client_id == TRM_UMTS) || \
             (client_id == TRM_UMTS_TX) || \
             (client_id == TRM_UMTS_SECONDARY) || \
             (client_id == TRM_UMTS_CA) || \
             (client_id == TRM_UMTS_CA_SECONDARY) || \
             (client_id == TRM_UMTS2) || \
             (client_id == TRM_UMTS2_TX) || \
             (client_id == TRM_UMTS2_SECONDARY) \
           )
  #else
  #define WRM_IS_TRM_CLIENT_ID_VALID(client_id) \
           ( (client_id == TRM_UMTS) || \
             (client_id == TRM_UMTS_SECONDARY) || \
             (client_id == TRM_UMTS_CA) || \
             (client_id == TRM_UMTS_CA_SECONDARY) || \
             (client_id == TRM_UMTS2) || \
             (client_id == TRM_UMTS2_SECONDARY) \
           )
  #endif
#else
  #ifdef FEATURE_WCDMA_RX_TX_SPLIT
  #define WRM_IS_TRM_CLIENT_ID_VALID(client_id) \
           ( (client_id == TRM_UMTS) || \
             (client_id == TRM_UMTS_TX) || \
             (client_id == TRM_UMTS_SECONDARY) || \
             (client_id == TRM_UMTS_CA) || \
             (client_id == TRM_UMTS_CA_SECONDARY) \
           )
  #else
  #define WRM_IS_TRM_CLIENT_ID_VALID(client_id) \
           ( (client_id == TRM_UMTS) || \
             (client_id == TRM_UMTS_SECONDARY) || \
             (client_id == TRM_UMTS_CA) || \
             (client_id == TRM_UMTS_CA_SECONDARY) \
           )
  #endif
#endif

/*Macros to check the state of WRM*/
#define WRM_STATE_IS_IN_LOCK(resource, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_state == WRM_IN_LOCK_STATE)
#define WRM_STATE_IS_LOCK_REQ(resource, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_state == WRM_LOCK_REQ_STATE)
#define WRM_STATE_IS_LOCK_RES(resource, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_state == WRM_LOCK_RES_STATE)
#define WRM_STATE_IS_NO_LOCK(resource, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_state == WRM_NO_LOCK_STATE)
#define WRM_STATE_IS_UNLOCK_IN_PROGRESS(resource, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_state == WRM_UNLOCK_IN_PROGRESS_STATE)

/*Macros to check the state of WRM clients*/
#define WRM_CLI_STATE_IS_IN_LOCK(resource, wrm_client_id, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_client_info[wrm_client_id].cli_state == WRM_CLI_IN_LOCK_STATE)
#define WRM_CLI_STATE_IS_LOCK_REQ(resource, wrm_client_id, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_client_info[wrm_client_id].cli_state == WRM_CLI_LOCK_REQ_STATE)
#define WRM_CLI_STATE_IS_LOCK_RES(resource, wrm_client_id, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_client_info[wrm_client_id].cli_state == WRM_CLI_LOCK_RES_STATE)
#define WRM_CLI_STATE_IS_NO_LOCK(resource, wrm_client_id, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_client_info[wrm_client_id].cli_state == WRM_CLI_NO_LOCK_STATE)
#define WRM_CLI_STATE_IS_UNLOCK_IN_PROGRESS(resource, wrm_client_id, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_client_info[wrm_client_id].cli_state == WRM_CLI_UNLOCK_IN_PROGRESS_STATE)

/*Macros to get and set the state of WRM*/
#define WRM_SET_WRM_STATE(resource, state_of_wrm) (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_state = state_of_wrm)
#define WRM_GET_WRM_STATE(resource) (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_state)

/*Macros to get and set the state of WRM client*/
#define WRM_SET_WRM_CLI_STATE(resource, wrm_client_id, wrm_cli_state, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_client_info[wrm_client_id].cli_state = wrm_cli_state)
#define WRM_GET_WRM_CLI_STATE(resource, wrm_client_id, sub_id) (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_client_info[wrm_client_id].cli_state)

#define WRM_GET_CLI_GRANT_CB(resource, wrm_client_id, sub_id) \
  (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_client_info[wrm_client_id].grant_cb)

/*Macros to get alternate WRM client id*/
#define WRM_GET_ALTERNATE_CLIENT_ID(wrm_client_id) (wrm_primary_client_enum_type)((wrm_client_id + (wrm_primary_client_enum_type)1) % WRM_PRIMARY_CLIENT_MAX)

#define WRM_IS_RESOURCE_VALID(resource) ((resource >= WCDMA_TRM) && (resource < WCDMA_TRM_NUM_RESOURCES))

#ifdef FEATURE_WCDMA_RX_TX_SPLIT
#define WRM_IS_CLIENT_ID_VALID(resource, wrm_client_id) \
         (((resource == WCDMA_TRM) && (wrm_client_id < WRM_PRIMARY_CLIENT_MAX)) || \
          ((resource == WCDMA_TRM_TX) && (wrm_client_id < WRM_PRIMARY_CLIENT_MAX)) || \
          ((resource == WCDMA_TRM_SECONDARY) && (wrm_client_id < WRM_SECONDARY_CLIENT_MAX)) || \
          ((resource == WCDMA_TRM_CA) && (wrm_client_id < WRM_PRIMARY_CLIENT_MAX)) || \
          ((resource == WCDMA_TRM_SECONDARY_CA) && (wrm_client_id < WRM_SECONDARY_CLIENT_MAX)) \
          )
#else
#define WRM_IS_CLIENT_ID_VALID(resource, wrm_client_id) \
         (((resource == WCDMA_TRM) && (wrm_client_id < WRM_PRIMARY_CLIENT_MAX)) || \
          ((resource == WCDMA_TRM_SECONDARY) && (wrm_client_id < WRM_SECONDARY_CLIENT_MAX)) || \
          ((resource == WCDMA_TRM_CA) && (wrm_client_id < WRM_PRIMARY_CLIENT_MAX)) || \
          ((resource == WCDMA_TRM_SECONDARY_CA) && (wrm_client_id < WRM_SECONDARY_CLIENT_MAX)) \
          )
#endif

#define WRM_GET_REASON_FROM_ACTIVITY(activity, wsub_id)   \
  (wrm_get_trm_reason_for_activity(activity, wsub_id))

#define WRM_GET_SUB_REASON_FROM_ACTIVITY(activity) (wrm_res_rsn_for_activity[activity].sub_reason)

#define WRM_GET_NON_INV_RSN_FROM_ACTIVITY(activity) (wrm_res_rsn_for_activity[activity].reason)

#ifdef FEATURE_WCDMA_RX_TX_SPLIT
/* For RX_TX_Split feature the RX and TX resources need to acquired individually using
** RX and TX TRM clients, the following MACRO splits the TRM_RXTX_BEST resource into
** TRM_RX_BEST and TRM_TX_BEST resources, all other RX only resources remain the same */
#define WRM_GET_RESOURCE_FROM_ACTIVITY(activity) \
             ( (wrm_res_rsn_for_activity[activity].resource == TRM_RXTX_BEST) ? \
               ((resource == WCDMA_TRM_TX) ? TRM_TX_BEST : TRM_RX_BEST) : \
               (wrm_res_rsn_for_activity[activity].resource) )
#else
#define WRM_GET_RESOURCE_FROM_ACTIVITY(activity) \
             wrm_res_rsn_for_activity[activity].resource
#endif

#define WRM_GET_ACQ_DR_EQUIVALENT_REASON(value) \
             ((value == TRM_ACQUISITION_INV) ? TRM_ACQUISITION_CELL_SELECT_INV: TRM_ACQUISITION_CELL_SELECT)

#define WRM_IS_PRIO_INV_ALLOWED(activity) (wrm_res_rsn_for_activity[activity].inv_reason != TRM_NO_REASON)

#define WRM_GET_INV_PRIO_FOR_ACTIVITY(activity) \
             (wrm_res_rsn_for_activity[activity].inv_reason)

#define WRM_GET_ACTIVE_CLIENT_BMSK(resource) \
             wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_active_client_bmsk

/*APS (Adaptive Page skipping) related Macros*/
#define WRM_IS_ANY_APS_CLIENT_ACTIVE() \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_aps_db.bmsk != 0)

#define WRM_GET_ACTIVE_APS_CLI_BMSK() \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_aps_db.bmsk)

#define WRM_ADD_CLIENT_TO_APS_CLI_BMSK(aps_client_id) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_aps_db.bmsk |= (1 << aps_client_id))

#define WRM_REMOVE_CLIENT_FROM_APS_CLI_BMSK(aps_client_id) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_aps_db.bmsk  = \
               wl1trm_ext_cntrl_params[wsub_id].wrm_aps_db.bmsk & (~(1 << aps_client_id)) )

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
#define WRM_SET_RFM_PATH(resource, path) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].rfm_path = path)

#define WRM_GET_RFM_PATH(resource) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].rfm_path)
#else /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
#define WRM_SET_DEVICE_ID(resource, device) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].device_id = device)

#define WRM_GET_DEVICE_ID(resource) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].device_id)
#endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */    


#define WRM_INVALIDATE_BAND_INFO(resource) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].band_info.num_bands = 0)

#define WRM_GET_TRM_CMD_POSTED(resource) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].trm_cmd_posted)

#define WRM_SET_TRM_CMD_POSTED(resource, cmd) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].trm_cmd_posted = cmd)

#define WRM_INVALIDATE_TRM_CMD_POSTED(resource) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].trm_cmd_posted = WRM_MAX_COMMANDS)

#define WRM_ADD_CLIENT_TO_ACTIVE_CLI_BMSK(resource, client_id, sub_id) \
             (wl1trm_ext_cntrl_params[sub_id].wrm_db.wrm_resource_info[resource].wrm_active_client_bmsk |= (1 << client_id))

#define WRM_REMOVE_CLIENT_FROM_ACTIVE_CLI_BMSK(resource, client_id) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_active_client_bmsk = \
               wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_active_client_bmsk & (~(1 << client_id)))

#define WRM_GET_COMMAND_PENDING(resource) \
             (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_pending_cmd)

    /*Skipped copying wl1_device_swap_pending,wl1_fake_drx_pending,wl1_fake_grant_issued,wl1_rx_cmd_pending elements. 
      Please copy them separately as required*/             
#define WRM_SET_COMMAND_PENDING(resource_input, pending_cmd) \
  do {\
    wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource_input].wrm_pending_cmd.valid = (pending_cmd)->valid;\
    wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource_input].wrm_pending_cmd.resource = (pending_cmd)->resource;\
    wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource_input].wrm_pending_cmd.client_id = (pending_cmd)->client_id;\
    wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource_input].wrm_pending_cmd.activity = (pending_cmd)->activity;\
    wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource_input].wrm_pending_cmd.band_info = (pending_cmd)->band_info;\
    wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource_input].wrm_pending_cmd.duration = (pending_cmd)->duration;\
    wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource_input].wrm_pending_cmd.grant_callback = (pending_cmd)->grant_callback;\
    wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource_input].wrm_pending_cmd.mod_freq_band = (pending_cmd)->mod_freq_band;\
  }while(0)


#define WRM_INVALIDATE_PENDING_CMD(resource) \
  do {\
   wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_pending_cmd.valid = FALSE; \
   wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_pending_cmd.wl1_device_swap_pending = FALSE; \
   wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_pending_cmd.wl1_rx_cmd_pending = FALSE; \
  }while(0)

#define WRM_IS_CMD_PENDING_FOR_CLIENT(resource, wrm_client_id) \
          ((wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_pending_cmd.valid == TRUE) &&   \
            (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_pending_cmd.client_id == wrm_client_id))

#define WRM_RRC_CMD_PENDING() (WRM_IS_CMD_PENDING_FOR_CLIENT(WCDMA_TRM, WRM_PRIMARY_CLIENT_RRC))

#define WRM_IS_ANY_CMD_PENDING() \
  (WRM_IS_CMD_PENDING_FOR_CLIENT(WCDMA_TRM,WRM_PRIMARY_CLIENT_RRC)||  \
  WRM_IS_CMD_PENDING_FOR_CLIENT(WCDMA_TRM,WRM_PRIMARY_CLIENT_L1))
  
#define WRM_IS_ANY_CLI_IN_LOCK_REQ_STATE(resource) 0

#define WRM_DOES_ACTIVITY_NEED_TX(activity) \
          (wrm_res_rsn_for_activity[activity].resource == TRM_RXTX_BEST)

#define WRM_IS_SWAP_PENDING_FOR_CLIENT(resource) \
  (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_pending_cmd.wl1_device_swap_pending == TRUE)

#define WRM_SET_SWAP_PENDING_FOR_CLIENT(resource, value) \
  (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_pending_cmd.wl1_device_swap_pending = value)

#define WRM_GET_CURRENT_ACTIVITY_FOR_CLIENT(resource,client) \
  (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_client_info[client].req_activity)

#define WRM_IS_ACTIVITY_VALID(activity) \
  (activity < WRM_MAX_ACTIVITIES)

#define WRM_SET_CLIENT_ACTIVITY(resource, client, activity)\
  (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[resource].wrm_client_info[client].req_activity = activity)

/* Maps l1_freq_scan_band_enum_type to sys_band_class_e_type for TRM request*/
extern const trm_band_map_struct_type trm_map_l1_band_type[L1_FREQ_WCDMA_BAND_NONE];

#define WRM_SET_FAKE_DRX_PENDING_FOR_CLIENT(value) \
  (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[WCDMA_TRM].wrm_pending_cmd.wl1_fake_drx_pending = value)

#define WRM_IS_FAKE_DRX_PENDING_FOR_CLIENT() \
  (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[WCDMA_TRM].wrm_pending_cmd.wl1_fake_drx_pending == TRUE)

#ifdef FEATURE_WCDMA_RX_TX_SPLIT
#define WRM_SET_RX_CMD_PENDING_FOR_CLIENT(value) \
  (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[WCDMA_TRM_TX].wrm_pending_cmd.wl1_rx_cmd_pending = value)

#define WRM_IS_RX_CMD_PENDING_FOR_CLIENT() \
  (wl1trm_ext_cntrl_params[wsub_id].wrm_db.wrm_resource_info[WCDMA_TRM_TX].wrm_pending_cmd.wl1_rx_cmd_pending == TRUE)

#define WRM_IS_IN_LOCK_REQ_STATE() \
  (WRM_STATE_IS_LOCK_REQ(WCDMA_TRM, wsub_id) || WRM_IS_RX_CMD_PENDING_FOR_CLIENT())

#else
#define WRM_IS_IN_LOCK_REQ_STATE()  (WRM_STATE_IS_LOCK_REQ(WCDMA_TRM, wsub_id))

#endif

#define WRM_IS_MEAS_WAITING_FOR_GRANT() \
  (((WRM_GET_CURRENT_ACTIVITY_FOR_CLIENT(WCDMA_TRM,WRM_PRIMARY_CLIENT_L1) == WL1_ACTIVITY_NGBR_MEAS_IRAT) || \
    (WRM_GET_CURRENT_ACTIVITY_FOR_CLIENT(WCDMA_TRM,WRM_PRIMARY_CLIENT_L1) == WL1_ACTIVITY_IDLE_NGBR_MEAS)) && \
   (WRM_CLI_STATE_IS_LOCK_REQ(WCDMA_TRM,WRM_PRIMARY_CLIENT_L1, wsub_id) || \
    WRM_CLI_STATE_IS_LOCK_REQ(WCDMA_TRM, WRM_SECONDARY_CLIENT_MEAS, wsub_id)))

/*===========================================================================

                     FUNCTION PROTOTYPES

===========================================================================*/
#ifdef FEATURE_WCDMA_TRM
#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION     WL1_TRM_GET_RF_PATH

DESCRIPTION
  This function is invoked everytime we make a call into RFM.  All RFM APIs
  take RF path as a parameter.

  When we got TRM lock for WCDMA operation earlier, we called
  trm_get_rf_device().  We stored that value away to be returned by this
  function now.

DEPENDENCIES
  None.

PARAMETERS
  wrm_resource_enum_type: Resource Type
  wsub_id_e_type: WCDMA Subscription ID

RETURN VALUE
  rfm_path_type - RF path in use by WCDMA operation.

SIDE EFFECTS
  None.
===========================================================================*/
extern rfm_path_type wl1_trm_get_rf_path( wrm_resource_enum_type resource, wsub_id_e_type wsub_id);
#else /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION     WL1_TRM_GET_PRIMARY_RF_DEVICE

DESCRIPTION
  This function is invoked everytime we make a call into RFM.  All RFM APIs
  take RF device as a parameter.

  When we got TRM lock for WCDMA primary operation earlier, we called
  trm_get_rf_device().  We stored that value away to be returned by this
  function now.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  rfm_device_enum_type - RF device in use by primary WCDMA operation.

SIDE EFFECTS
  None.
===========================================================================*/
extern rfm_device_enum_type wl1_trm_get_primary_rf_device( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_TRM_GET_PRIMARY_TX_RF_DEVICE

DESCRIPTION
  This function is invoked everytime we make a call into RFM.  All RFM APIs
  take RF device as a parameter. Query the TX device from WRM and
  Return to the caller

DEPENDENCIES
  None.

PARAMETERS
  wsub_id

RETURN VALUE
  rfm_device_enum_type - TX RF device in use by primary WCDMA operation.

SIDE EFFECTS
  None.
===========================================================================*/
extern rfm_device_enum_type wl1_trm_get_primary_tx_rf_device( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_TRM_GET_PRIMARY_RF_DEVICE_FOR_CA

DESCRIPTION
  This function provides primary RF device ID for Carrier Aggregation (DBDC)

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  rfm_device_enum_type - RF device in use by primary WCDMA operation.

SIDE EFFECTS
  None.
===========================================================================*/
extern rfm_device_enum_type  wl1_trm_get_primary_rf_device_for_ca( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_TRM_GET_SECONDARY_RF_DEVICE

DESCRIPTION
  This function is invoked everytime we make a call into RFM.  All RFM APIs
  take RF device as a parameter.

  When we got TRM lock for WCDMA secondary operation earlier, we called
  trm_get_rf_device().  We stored that value away to be returned by this
  function now.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  rfm_device_enum_type - RF device in use by secondary WCDMA operation.

SIDE EFFECTS
  None.
===========================================================================*/
extern rfm_device_enum_type wl1_trm_get_secondary_rf_device( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_TRM_GET_SECONDARY_RF_DEVICE_FOR_CA

DESCRIPTION
  This function return secondary rf device for carrier aggregation.
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  rfm_device_enum_type - RF device in use by secondary WCDMA operation.

SIDE EFFECTS
  None.
===========================================================================*/
extern rfm_device_enum_type wl1_trm_get_secondary_rf_device_for_ca( wsub_id_e_type wsub_id);

#endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

/*===========================================================================
FUNCTION     WL1_TRM_GET_BAND

DESCRIPTION
  This function returns the band channel belongs to 

DEPENDENCIES
  None.

PARAMETERS
  uint16 channel - The channel we are tuning to

RETURN VALUE
  l1_freq_scan_band_enum_type - Enum type of the band.

SIDE EFFECTS
  None.
===========================================================================*/

extern l1_freq_scan_band_enum_type wl1_trm_get_band(uint16 channel);


/*===========================================================================
FUNCTION     WL1_TRM_CARRIER_AGGREGATION_ON

DESCRIPTION
  This function checks if it is DBDC call or normal DC call

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
boolean to indicate if DBDC call or not

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean
wl1_trm_carrier_aggregation_on
( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_TRM_RESERVE_CARRIER_AGGREGATION_RESOURCES

DESCRIPTION
  This function reserves TRM for Carrier Aggregation

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
boolean to indicate if DBDC call or not

SIDE EFFECTS
  None.
===========================================================================*/
extern void 
wl1_trm_reserve_carrier_aggregation_resources
(uint16 uarfcn, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_TRM_RELEASE_CARRIER_AGGREGATION_RESOURCES

DESCRIPTION
  This function reserves TRM for Carrier Aggregation

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
boolean to indicate if DBDC call or not

SIDE EFFECTS
  None.
===========================================================================*/
extern void 
wl1_trm_release_carrier_aggregation_resources
( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_TRM_GET_SYS_BAND_FROM_FREQ_SCAN_BAND

DESCRIPTION
  Convert fs_band(l1_freq_scan_band_enum_type) to
  sys_band(sys_band_class_e_type), return sys_band

DEPENDENCIES
  None

PARAMETERS
  l1_freq_scan_band_enum_type fs_band

RETURN VALUE
  sys_band_class_e_type sys_band

SIDE EFFECTS
  None
===========================================================================*/
extern sys_band_class_e_type wl1_trm_get_sys_band_from_freq_scan_band
(l1_freq_scan_band_enum_type fs_band, wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_TRM */

/*===========================================================================
FUNCTION     WL1_TRM_GET_CURR_BAND_FOR_LNA_SWITCH

DESCRIPTION
  Returns the current band for the given device 

DEPENDENCIES
  None.

PARAMETERS
  dl_band_idx - WL1_DL_BAND_0 for primary device,
                WL1_DL_BAND_1 for DBDC (CA) device

RETURN VALUE
  Current band if device is tuned, RFCOM_BAND_INVALID otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
extern rfcom_wcdma_band_type wl1_trm_get_curr_band_for_lna_switch(uint8 dl_band_idx, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION     WL1_TRM_INIT

DESCRIPTION
  This function initializes TRM database
  
DEPENDENCIES
  None.

PARAMETERS
  NONE

RETURN VALUE
  NONE

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_trm_init( void );


/*===========================================================================
FUNCTION     WRM_UNLOCK_TRM_CB

DESCRIPTION
  This function is called by TRM when WCDMA needs to release the lock for
  the sake of higher priority activity in other clients.

DEPENDENCIES
  WCDMA must be holding an RF resource lock

PARAMETERS
  unlock_data : Data for unlock callback advanced.
  resource: WRM resource for which UNLOCK_BY is coming for.
  wsub_id: Subscription ID

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_unlock_trm_cb(
  trm_unlock_callback_data* unlock_data, wrm_resource_enum_type resource, wsub_id_e_type wsub_id
);


/*============================================================================
FUNCTION wrm_sec_unlock_timer_expired_cmd_handler

DESCRIPTION
 This function gets called when processing SEC_UNLOCK_TIMER_EXPIRED_CMD. 
 
 This function decides to go for DTA/QTDA based on confirmation from WL1 and RxD. 

PARAMETERS
  wsub_id_e_type wsub_id - Subscription ID

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/
extern void wrm_sec_unlock_timer_expired_cmd_handler(wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION     wrm_handle_pending_request

DESCRIPTION
  This function handles any pending request_and_notify command.
  In some scenarios like say measurements is in progress and RxD asks for lock,
  we would have kept RxD's request pending till meas releases the lock.
    
DEPENDENCIES
  None

PARAMETERS
  resource: The resource for which request_and_notify is pending

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wrm_handle_pending_request(
  wrm_resource_enum_type resource
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wrm_post_local_cmd_for_pending_wrm_cmd

DESCRIPTION
  This function posts a local command for handling a pending request_and_notify.
  In some scenarios like say measurements is in progress and RxD asks for lock,
  we would have kept RxD's request pending till meas releases the lock.
    
DEPENDENCIES
  None

PARAMETERS
  resource: The resource for which request_and_notify is pending

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wrm_post_local_cmd_for_pending_wrm_cmd(
  wrm_resource_enum_type resource_id
  , wsub_id_e_type wsub_id);


/*============================================================================
FUNCTION WRM_REQUEST_PRIMARY_ANTENNA

DESCRIPTION
  Specifies the given client needs primary antenna, for the
  given duration, for the supplied activity. The resource request is immediately
  evaluated, and the result returned.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs the RF resource
  activity: The activity for which lock is being requested
  duration: How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested
  ta_preference: The type of tuneaway we would like to do
  wsubd_id: Subscription id

RETURN VALUE
  True - Resource is granted.
  False - Resource is denied.

SIDE EFFECTS
  If the client currently holds an RF chain, that chain is released before
  the request is evaluated.
============================================================================*/

extern boolean wrm_request_primary_antenna
(
  /* The client which needs the RF resource */
  wrm_primary_client_enum_type wrm_client_id,

  /* Activity for which RF resource which is being requested */
  wrm_activity_enum_type              activity,

  /* How long the resource will be needed for (in sclks) */
  wrm_duration_type                  duration,

  wrm_freq_input_type band_info,

  /* Preferred tuneaway type */
  wrm_tuneaway_enum_type ta_preference,

  wsub_id_e_type wsub_id
);

/*============================================================================
FUNCTION WRM_REQUEST_PRIMARY_ANTENNA_FOR_CA

DESCRIPTION
  Specifies the given client needs primary antenna for DBDC, for the
  given duration, for the supplied activity. The resource request is immediately
  evaluated, and the result returned.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs the RF resource
  activity: The activity for which lock is being requested
  duration: How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested

RETURN VALUE
  True - Resource is granted.
  False - Resource is denied.

SIDE EFFECTS
  If the client currently holds an RF chain, that chain is released before
  the request is evaluated.
============================================================================*/
extern boolean wrm_request_primary_antenna_for_ca
(
  /* The client which needs the RF resource */
  wrm_primary_client_enum_type wrm_client_id,

  /* Activity for which RF resource which is being requested */
  wrm_activity_enum_type  activity,

  /* How long the resource will be needed for (in sclks) */
  wrm_duration_type  duration,

  /*Bands for which RF resource which is being requested */
  wrm_freq_input_type band_info
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_REQUEST_AND_NOTIFY_PRIMARY_ANTENNA

DESCRIPTION
  This function makes an asynchronous lock request to TRM for primary antenna.If WRM 
  already has TRM lock, it is provided immediately to the caller. WRM never times out 
  in waiting for the TRM lock. The caller of wrm_request_and_notify() should should 
  implement any timer for the call.If the caller decides to timeout the request, the 
  caller should call wrm_release_primary_antenna to cancel the pending request.

DEPENDENCIES
  None.

PARAMETERS
  wrm_client_id: The client which needs the RF resource
  activity: The activity for which lock is being requested
  duration: How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested
  grant_callback: Callback to notify client when resource is granted by TRM at 
                  a later point.
  ta_preference: The type of tuneaway we would like to do
  wsubd_id: Subscription id

RETURN VALUE
  True  - Resource is granted. The grant callback will NOT be called in this case.
  False - Resource is pending grant. The grant callback will be called when MCS gives
          the grant.

SIDE EFFECTS
  None.
===========================================================================*/

extern boolean wrm_request_and_notify_primary_antenna
(
  /* The client which needs the RF resource */
  wrm_primary_client_enum_type wrm_client_id,

  /* Activity for which RF resource which is being requested */
  wrm_activity_enum_type              activity,

  /* How long the resource will be needed for (in sclks) */
  wrm_duration_type                  duration,

  wrm_freq_input_type band_info,

  /* Callback to notify client when resource is granted */
  wrm_grant_callback_type         grant_callback,

  wsub_id_e_type wsub_id
);

/*============================================================================
FUNCTION WRM_RELEASE_PRIMARY_ANTENNA

DESCRIPTION
  This function releases the lock for primary antenna previously acquired.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs to cancel

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/

extern void wrm_release_primary_antenna (
  wrm_primary_client_enum_type wrm_client_id
, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_RELEASE_PRIMARY_ANTENNA_FOR_CA

DESCRIPTION
  This function releases the lock for primary antenna previously acquired for DBDC.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs to cancel

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/
extern void wrm_release_primary_antenna_for_ca (
  /* The client releasing RF resource  */
  wrm_primary_client_enum_type wrm_client_id
, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_RELEASE_PRIMARY_RESOURCE

DESCRIPTION
  This function releases the lock for primary resource.

DEPENDENCIES
  None

PARAMETERS
  resource
  wrm_client_id: The client which needs to cancel

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/
extern void wrm_release_primary_resource (
  /*Resource being requested*/
  wrm_resource_enum_type       resource,
  
  /* The client releasing RF resource  */
  wrm_primary_client_enum_type wrm_client_id,
  
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_RESERVE_PRIMARY_ANTENNA

DESCRIPTION
  Specifies the given client needs the primary antenna at the given
  time, for the given duration, for the supplied activity.

    
DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs the RF resource
  activity: The activity for which lock is being requested
  when: When the resource will be needed (sclks timestamp)
  duration: How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested
  ta_preference: The type of tuneaway we would like to do
  wsubd_id: Subscription id

RETURN VALUE
  None

SIDE EFFECTS
  If the client currently holds an RF chain, the chain is released.
===========================================================================*/
extern void wrm_reserve_primary_antenna(
  /* The client which needs the RF resource */
  wrm_primary_client_enum_type               wrm_client_id,

  wrm_activity_enum_type activity,

  /* When the resource will be needed (sclks timestamp) */
  wrm_time_type                      when,

  /* How long the resource will be needed for (in sclks) */
  wrm_duration_type                  duration,

  wrm_freq_input_type band_info,

  /* Preferred tuneaway type */
  wrm_tuneaway_enum_type ta_preference,

  wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_RETAIN_LOCK_FOR_PRIMARY

DESCRIPTION
  Informs the Transceiver Resource Manager that the calling client wants to hold
  primary resource indefinitely.  TRM may inform the WRM that it must
  give up the lock through the registered async callback.
  
DEPENDENCIES
  The calling client must be holding a transceiver resource lock

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/
extern void wrm_retain_lock_for_primary( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_RETAIN_LOCK_FOR_SECONDARY

DESCRIPTION
  Informs the Transceiver Resource Manager that the calling client wants to hold
  primary resource indefinitely.  TRM may inform the WRM that it must
  give up the lock through the registered async callback.
  
DEPENDENCIES
  The client must be holding a transceiver resource lock

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/
extern void wrm_retain_lock_for_secondary( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_RETAIN_LOCK_FOR_PRIMARY_CA

DESCRIPTION
  Informs the Transceiver Resource Manager that the calling client wants to hold
  primary CA resource indefinitely.  TRM may inform the WRM that it must
  give up the lock through the registered async callback.

DEPENDENCIES
  The calling client must be holding a transceiver resource lock

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/
extern void wrm_retain_lock_for_primary_ca( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_RETAIN_LOCK_FOR_SECONDARY_CA

DESCRIPTION
  Informs the Transceiver Resource Manager that the calling client wants to hold
  secondary CA resource indefinitely.  TRM may inform the WRM that it must
  give up the lock through the registered async callback.
  
DEPENDENCIES
  The client must be holding a transceiver resource lock

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/
extern void wrm_retain_lock_for_secondary_ca( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_MODIFY_DURATION_FOR_PRIMARY_ANTENNA

DESCRIPTION
 Attempts to extend the duration an RF resource lock is held for. If the entire
 extension can be granted, it will be granted. If the entire extension cannot be 
 granted, the lock duration remains unchanged, and the client should release the 
 lock at the original lock expiry point. If the client had originally locked the 
 resource for longer than the required extension, the lock will remain the original
 length and the extension will be INDICATED AS BEING GRANTED. The extension is all 
 or nothing.  
 
DEPENDENCIES
  The client must be holding an RF resource lock

PARAMETERS
  wrm_client_id: The client which needs to cancel
  new_duration: The required extension, in sclks, from "now"

RETURN VALUE
  TRUE if the lock duration extends from "now" to "now + maximum".
  FALSE if the lock duration is unchanged.

SIDE EFFECTS
  None
============================================================================*/

extern boolean wrm_modify_duration_for_primary_antenna(
  wrm_primary_client_enum_type wrm_client_id,
  wrm_duration_type new_duration
, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_MODIFY_BAND_FOR_PRIMARY_ANTENNA

DESCRIPTION
 This requests for grant for a new frequency. If the grant is given immediately,
 return value will be TRUE. If TRM sends a pending status, return value will be 
 FALSE and the grant callback will be called when TRM gives the grant. The caller 
 is responsible for starting a lock wait timer  if necessary. 
 
DEPENDENCIES
  The client must be holding an RF resource lock.

PARAMETERS
  band: The band to which the client needs to tune.
  wrm_client_id: WRM client
  grant_callback: Callback to notify client when resource is granted by TRM at 
                  a later point.
  wsub_id: Subscription id

RETURN VALUE
  TRUE if the modify frequency request was granted by TRM
  FALSE if the response to modify frequency request is pending from TRM

SIDE EFFECTS
  None
============================================================================*/
extern boolean wrm_modify_band_for_primary_antenna(
  l1_freq_scan_band_enum_type band,
  uint8 wrm_client_id,
  wrm_grant_callback_type grant_callback,
  wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_MODIFY_BAND

DESCRIPTION
 This requests for grant for a new frequency. If the grant is given immediately,
 return value will be TRUE. If TRM sends a pending status, return value will be 
 FALSE and the grant callback will be called when TRM gives the grant. The caller 
 is responsible for starting a lock wait timer  if necessary. 
 
DEPENDENCIES
  The client must be holding an RF resource lock.

PARAMETERS
  band: The band to which the client needs to tune.
  resource: Resource for which we are modifying the band
  wrm_client_id: Corresponding client id
  grant_callback: Callback to notify client when resource is granted by TRM at 
                  a later point.
  wsub_id: Subscription id

RETURN VALUE
  TRUE if the modify frequency request was granted by TRM
  FALSE if the response to modify frequency request is pending from TRM

SIDE EFFECTS
  None
============================================================================*/
extern boolean wrm_modify_band(
  l1_freq_scan_band_enum_type band,
  wrm_resource_enum_type resource,
  uint8 wrm_client_id,
  wrm_grant_callback_type grant_callback,
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_MODIFY_ACTIVITY_FOR_PRIMARY_ANTENNA

DESCRIPTION
  This requests for grant for a new activity. If the grant is given immediately, return value will be TRUE. 
  If there is a DR<->hybrid transition, return value will be FALSE and the grant callback will be called when
  TRM gives the grant.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs the RF resource
  activity: The new activity for which lock is being requested
  min_duration : The duration of the activity
  grant_callback : The callback to be called for the grant
  cmd_params : params to be saved in case the cmd is pended

RETURN VALUE
  True - If lock is granted immediately
  False - If lock is not available immediately. In that case, grant callback will be called when lock
            is granted.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean wrm_modify_activity_for_primary_antenna(
  wrm_primary_client_enum_type wrm_client_id,
  wrm_activity_enum_type activity,
  /*Minimum time needed to release the lock if there is an unlock by request from TRM*/
  wrm_duration_type      min_duration, 
  wrm_grant_callback_type grant_callback,
  wrm_cmd_struct_type* cmd_params_ptr,
  boolean can_handle_denial,
  wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_REQUEST_SECONDARY_ANTENNA

DESCRIPTION
  Specifies the given client needs secondary chain, for the given duration, for
  the supplied reason. The resource request is immediately evaluated, and the 
  result returned.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs the RF resource
  activity: The activity for which lock is being requested
  duration: How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested
  ta_preference: The type of tuneaway we would like to do
  wsubd_id: Subscription id

RETURN VALUE
  True - Resource is granted.
  False - Resource is denied.

SIDE EFFECTS
  If the client currently holds an RF chain, that chain is released before
  the request is evaluated.
============================================================================*/
extern boolean wrm_request_secondary_antenna
(
  /* The client which needs the RF resource */
  wrm_secondary_client_enum_type wrm_client_id,

  /* Activity for which RF resource which is being requested */
  wrm_activity_enum_type              activity,

  /* How long the resource will be needed for (in sclks) */
  wrm_duration_type                  duration,

  wrm_freq_input_type band_info,

  /* Preferred tuneaway type */
  wrm_tuneaway_enum_type ta_preference,

  wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_REQUEST_SECONDARY_ANTENNA

DESCRIPTION
  Specifies the given client needs secondary antenna for DBDC, for the
  given duration, for the supplied activity. The resource request is immediately
  evaluated, and the result returned.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs the RF resource
  activity: The activity for which lock is being requested
  duration: How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested

RETURN VALUE
  True - Resource is granted.
  False - Resource is denied.

SIDE EFFECTS
  If the client currently holds an RF chain, that chain is released before
  the request is evaluated.
============================================================================*/

extern boolean wrm_request_secondary_antenna_for_ca
(
  /* The client which needs the RF resource */
  wrm_secondary_client_enum_type wrm_client_id,

  /* Activity for which RF resource which is being requested */
  wrm_activity_enum_type              activity,

  /* How long the resource will be needed for (in sclks) */
  wrm_duration_type                  duration,

  /*Bands for which RF resource which is being requested */
  wrm_freq_input_type band_info
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_REQUEST_AND_NOTIFY_SECONDARY_ANTENNA

DESCRIPTION
  This function makes an asynchronous lock request to TRM for secondary antenna.If WRM 
  already has TRM lock, it is provided immediately to the caller. WRM never times out 
  in waiting for the TRM lock. The caller of wrm_request_and_notify() should should 
  implement any timer for the call.If the caller decides to timeout the request, the 
  caller should call wrm_release_primary_antenna to cancel the pending request.

DEPENDENCIES
  None.

PARAMETERS
  wrm_client_id: The client which needs the RF resource
  activity: The activity for which lock is being requested
  duration: How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested
  grant_callback: Callback to notify client when resource is granted by TRM at 
                  a later point.

RETURN VALUE
  True  - Resource is granted. The grant callback will NOT be called in this case.
  False - Resource is pending grant. The grant callback will be called when MCS gives
          the grant.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wrm_request_and_notify_secondary_antenna
(
  /* The client which needs the RF resource */
  wrm_secondary_client_enum_type wrm_client_id,

  /* Activity for which RF resource which is being requested */
  wrm_activity_enum_type              activity,

  /* How long the resource will be needed for (in sclks) */
  wrm_duration_type                  duration,

  wrm_freq_input_type band_info,

  /* Callback to notify client when resource is granted */
  wrm_grant_callback_type         grant_callback
, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_RELEASE_SECONDARY_ANTENNA

DESCRIPTION
  This function releases the lock for primary antenna previously acquired.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs to cancel

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/

extern void wrm_release_secondary_antenna (
  wrm_secondary_client_enum_type wrm_client_id
, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_RELEASE_SECONDARY_ANTENNA_FOR_CA

DESCRIPTION
  This function releases the lock for secondary antenna previously acquired for DBDC.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs to cancel

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/

extern void wrm_release_secondary_antenna_for_ca (
  /* The client releasing RF resource */
  wrm_secondary_client_enum_type wrm_client_id
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_RESERVE_SECONDARY_ANTENNA

DESCRIPTION
  Specifies the given client needs the primary antenna at the given
  time, for the given duration, for the supplied activity.

    
DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs the RF resource
  activity: The activity for which lock is being requested
  when: When the resource will be needed (sclks timestamp)
  duration: How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested
  ta_preference: The type of tuneaway we would like to do
  wsubd_id: Subscription id

RETURN VALUE
  None

SIDE EFFECTS
  If the client currently holds an RF chain, the chain is released.
===========================================================================*/
extern void wrm_reserve_secondary_antenna(
  /* The client which needs the RF resource */
  wrm_secondary_client_enum_type               wrm_client_id,

  wrm_activity_enum_type activity,

  /* When the resource will be needed (sclks timestamp) */
  wrm_time_type                      when,

  /* How long the resource will be needed for (in sclks) */
  wrm_duration_type                  duration,

  wrm_freq_input_type band_info,

    /* Preferred tuneaway type */
  wrm_tuneaway_enum_type ta_preference,

  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wrm_request_resources_for_secondary

DESCRIPTION
Allocates buffers and enables clocks for an RF call to entermode
it then calls entermode for the rf chain

DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_request_resources_for_secondary(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wrm_release_resources_for_secondary

DESCRIPTION
Drops RxD clocks and places RF call to sleep

DEPENDENCIES
  None.

PARAMETERS


RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_release_resources_for_secondary( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_REQUEST_PRIMARY_ANTENNA_FOR_CA

DESCRIPTION
  Specifies the given client needs primary CA antenna, for the
  given duration, for the supplied activity. The resource request is immediately
  evaluated, and the result returned.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs the RF resource
  activity: The activity for which lock is being requested
  duration: How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested

RETURN VALUE
  True - Resource is granted.
  False - Resource is denied.

SIDE EFFECTS
  If the client currently holds an RF chain, that chain is released before
  the request is evaluated.
============================================================================*/

extern boolean wrm_request_primary_antenna_for_ca
(
  /* The client which needs the RF resource */
  wrm_primary_client_enum_type wrm_client_id,

  /* Activity for which RF resource which is being requested */
  wrm_activity_enum_type              activity,

  /* How long the resource will be needed for (in sclks) */
  wrm_duration_type                  duration,

  wrm_freq_input_type band_info
, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_RELEASE_PRIMARY_ANTENNA_FOR_CA

DESCRIPTION
  This function releases the lock for primary CA antenna previously acquired.

DEPENDENCIES
  None	

PARAMETERS
  wrm_client_id: The client which needs to cancel

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/

extern void wrm_release_primary_antenna_for_ca (
  wrm_primary_client_enum_type wrm_client_id
, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_REQUEST_SECONDARY_ANTENNA_FOR_CA

DESCRIPTION
  Specifies the given client needs secondary CA chain, for the given duration, for
  the supplied reason. The resource request is immediately evaluated, and the 
  result returned.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs the RF resource
  activity: The activity for which lock is being requested
  duration: How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested

RETURN VALUE
  True - Resource is granted.
  False - Resource is denied.

SIDE EFFECTS
  If the client currently holds an RF chain, that chain is released before
  the request is evaluated.
============================================================================*/
extern boolean wrm_request_secondary_antenna_for_ca
(
  /* The client which needs the RF resource */
  wrm_secondary_client_enum_type wrm_client_id,

  /* Activity for which RF resource which is being requested */
  wrm_activity_enum_type              activity,

  /* How long the resource will be needed for (in sclks) */
  wrm_duration_type                  duration,

  wrm_freq_input_type band_info
, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION WRM_RELEASE_SECONDARY_ANTENNA_FOR_CA

DESCRIPTION
  This function releases the lock for primary antenna previously acquired.

DEPENDENCIES
  None

PARAMETERS
  wrm_client_id: The client which needs to cancel

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/

extern void wrm_release_secondary_antenna_for_ca (
  wrm_secondary_client_enum_type wrm_client_id
, wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION     WRM_START_MEASUREMENT

DESCRIPTION
  This function calls trm_set_client_state API to request grant for performing the
  given type of measurement for the requested duration on the given bands. 
  Irat_id assigned by TRM will be filled by this API.

DEPENDENCIES
  None

PARAMETERS
  target_rat: The RAT on which measurement is to be performed.
  duration  : How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested
  irat_id    : ID given by TRM for this measurement. 
  div_needed : if diversity is needed for IRAT or not
  activity   : Meas activity type 
  diversity_granted: whether div is granted by TRM or not
  meas_primary_rfm_path_ptr: Primary rfm_path.
  meas_div_rfm_path_ptr: diversity rfm_path.
  
RETURN VALUE
  True - If there is grant for the measurement
  False - If lock is denied.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean wrm_start_measurement(
  wrm_client_enum_type target_rat,
  wrm_duration_type duration,
  wrm_freq_input_type band_info,
  wrm_irat_identifier *irat_id,
  wrm_activity_enum_type activity,
  boolean is_diversity_needed,
  boolean *diversity_granted,
  rfm_path_type *meas_primary_rfm_path_ptr,
  rfm_path_type *meas_div_rfm_path_ptr,
  wsub_id_e_type wsub_id);

#else /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION     WRM_START_MEASUREMENT

DESCRIPTION
  This function calls trn_set_client_state API to request grant for performing the given type of
  measurement for the requested duration on the given bands. Irat_id assigned by TRM will be filled by
  this API.

DEPENDENCIES
  None

PARAMETERS
  target_rat: The RAT on which measurement is to be performed.
  duration  : How long the resource will be needed for (in sclks) 
  band_info: Band for which resource is being requested
  irat_id    : ID given by TRM for this measurement. 
  div_needed : if diversity is needed for IRAT or not
  activity   : Meas activity type 
  diversity_granted: whether div is granted by TRM or not
  meas_primary_device_id: Primary device id.
  meas_div_device_id: diversity device id.
  
RETURN VALUE
  True - If there is grant for the measurement
  False - If lock is denied.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean wrm_start_measurement(
  wrm_client_enum_type target_rat,
  wrm_duration_type duration,
  wrm_freq_input_type band_info,
  wrm_irat_identifier *irat_id,
  wrm_activity_enum_type activity,
  boolean is_diversity_needed,
  boolean *diversity_granted,
  rfm_device_enum_type *meas_primary_device_id,
  rfm_device_enum_type *meas_div_device_id
, wsub_id_e_type wsub_id);
#endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

/*===========================================================================
FUNCTION     WRM_IS_HOLDING_LOCK_FOR_REQUESTED_BANDS

DESCRIPTION
  This function checks if WRM resource holds lock for all bands requested for measurement.

DEPENDENCIES
  None

PARAMETERS
  meas_band_info: Band info for which resource is being requested 
  wrm_client_id: WRM client Id
  resource    : Resource for which measurements are requested
  wsub_id     : Subscription ID
  
RETURN VALUE
  True - If holding lock for all requested bands
  False - If not holding lock for all requested bands

SIDE EFFECTS
  None

===========================================================================*/

extern boolean wrm_is_holding_lock_for_requested_bands(
  wrm_freq_input_type meas_band_info, 
  wrm_primary_client_enum_type wrm_client_id,
  wrm_resource_enum_type resource,
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_COMPLETE_MEASUREMENT

DESCRIPTION
This function calls trm_set_client state API for releasing the grant taken with the given irat id for the
given type of measurement on the given bands.

DEPENDENCIES
  None

PARAMETERS
  irat_id    : ID given by TRM for this measurement.
  activity   : Meas activity type 
  
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wrm_complete_measurement(
  wrm_irat_identifier irat_id,
  wrm_activity_enum_type activity, 
  wsub_id_e_type wsub_id);


/*============================================================================
FUNCTION WRM_PRIMARY_RF_DEVICE_SUPPORTS_TX

DESCRIPTION
  API to indicate if the primary RF device currently held supports Tx.

DEPENDENCIES
  None

PARAMETERS

RETURN VALUE
  True - If primary RF device supports Tx.
  False -Else

SIDE EFFECTS
  None
============================================================================*/
extern boolean wrm_primary_rf_device_supports_tx( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_APS_INIT

DESCRIPTION
  This function TO RESET APS RELATED GLOBALS

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_aps_init(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_INIT

DESCRIPTION
  This function must be called during UE power-up to initialize the WRM globals.
  the sake of higher priority activity in other clients.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_init(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wrm_trm_get_band

DESCRIPTION
  

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern wrm_freq_input_type wrm_trm_get_band(uint16 channel, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1TRM_IS_UE_IN_DR_MODE

DESCRIPTION
  This function returns whether UE is in DR mode or not.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
============================================================================*/

extern boolean wl1trm_is_ue_in_dr_mode(wsub_id_e_type wsub_id);



/*===========================================================================
FUNCTION     wl1trm_is_ue_dr_capable

DESCRIPTION
  This function returns whether the HW is capable of DR operation.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
============================================================================*/

extern boolean wl1trm_is_ue_dr_capable(wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION wrm_register_callback

DESCRIPTION


DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
extern void wrm_register_callback(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wrm_deregister_callback

DESCRIPTION


DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
extern void wrm_deregister_callback( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     wl1_ds_release_resources_for_secondary

DESCRIPTION
Deallocates rxlm buffers an RF call to sleep

DEPENDENCIES
  None.

PARAMETERS


RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void wl1_ds_release_rf_resources_for_secondary(void);

/*===========================================================================
FUNCTION wrm_update_QTA_state

DESCRIPTION
This function updates TRM with current W QTA state

DEPENDENCIES
None

RETURN VALUE
None

PARAMETER
wrm_set_client_state_input_type* input - Pointer to the input that will be used by TRM 
wrm_set_client_state_output_data* output - Pointer to params where TRM will populate its output

SIDE EFFECTS
None
===========================================================================*/
extern void wrm_update_qta_state(wrm_set_client_state_input_type* input, wrm_set_client_state_output_data* output);

/*===========================================================================
FUNCTION wrm_tune_done_callback

DESCRIPTION


DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
extern void wrm_tune_done_callback(wrm_resource_enum_type resource, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION WRM_COMPARE_PRIORITIES

DESCRIPTION
 This function queries TRM for comprison of two priorities.
 (Compared_With_Reason and Compared Reason)

DEPENDENCIES
None

RETURN VALUE
 Result of the comparison (Compared with Priority , Compared Reason)
 TRM_PRIORITY_COMP_PRIORITIES_GREATER - If (Compared_With_Reason > Compared_Reason)
 TRM_PRIORITY_COMP_PRIORITY_LESSER -- if (Compared_With_Reason < Compared_Reason)
 TRM_PRIORITY_COMP_PRIORITIES_EQUAL -- if (Compared_With_Reason == Compared_Reason)

SIDE EFFECTS
None
===========================================================================*/
extern trm_priority_comparison_enum_type wrm_compare_priorities
(
  /* Cleint Id */
  trm_client_enum_t       client,

  /* First activity to compare */
  wrm_activity_enum_type       activity1,

  /* Second activity to compare */
  wrm_activity_enum_type       activity2
, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION     WRM_GRANT_CALLBACK_CMD_HANDLER

DESCRIPTION
  Pending

DEPENDENCIES
  None

PARAMETERS
  Pending

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
/*extern void wrm_async_callback_cmd_handler(wl1_wrm_async_callback_cmd_type *async_cb, wsub_id_e_type wsub_id); Commented out untill GL1 removes wl1trm.h inclusion*/

/*===========================================================================
FUNCTION wrm_modify_freq_default_callback

DESCRIPTION
Default callback function for wrm_modify_freq 

DEPENDENCIES
None

PARAMETERS
boolean grant, Not used

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
extern void wrm_modify_freq_default_grant_callback(boolean grant, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wrm_modify_band_wait_for_grant

This function is called to wait on TRM grant  when immediate grant for modify_freq is not given. 

DEPENDENCIES
None

RETURN VALUE
Boolean: True if grant is given

SIDE EFFECTS
None
===========================================================================*/
extern boolean wrm_modify_band_wait_for_grant( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION WRM_TRIGGER_DUAL_TO_SINGLE_RECEIVE

DESCRIPTION
  This function is called to trigger from DR-DSDS to SR-DSDS mode. 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
===========================================================================*/
extern void wrm_trigger_dual_to_single_receive( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION WRM_TRIGGER_SINGLE_TO_DUAL_RECEIVE

DESCRIPTION
  This function is called to trigger from SR-DSDS to DR-DSDS mode. 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
 None

SIDE EFFECTS
  None
===========================================================================*/
extern void wrm_trigger_single_to_dual_receive( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wrm_req_trm_lock_for_freq_scan

DESCRIPTION
  This function request TRM lock for frequency scan or raw scan tune

DEPENDENCIES
  None.

PARAMETERS
  Current band to tune to

RETURN VALUE
  TRUE: If Request granted .
  FALSE: If Request is not granted.

SIDE EFFECTS
  None.
===========================================================================*/
boolean wrm_req_trm_lock_for_freq_scan
(
  l1_freq_scan_band_enum_type band
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WRM_RRC_REQ_AND_NOTIFY_LOCK

DESCRIPTION
  Function is as a wrapper to wl1idletamgr lock req api to handle ACQ priority inversion in L1. 

DEPENDENCIES
  None.

PARAMETERS
  activity - Type of activity for which lock is needed
  duration - How long the resource will be needed for (in sclks)
  grant_callback - Callback to notify client when resource is granted
  band_info -  Band info

RETURN VALUE
  Boolean. FALSE -> Lock failure , TRUE -> Lock successful
===========================================================================*/
extern boolean wrm_rrc_req_and_notify_lock (
  /* Type of activity for which lock is needed */
  wrm_activity_enum_type activity,

  /* How long the resource will be needed for (in sclks) */
  trm_duration_t                  duration,

  /* Callback to notify client when resource is granted */
  wrm_grant_callback_type            grant_callback,

  /* Band info */
  wrm_freq_input_type band_info
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_UPDATE_WRM_DB

DESCRIPTION
  This function updates the WRM DB when we get grant for req_n_not.

DEPENDENCIES
  None.

PARAMETERS
   resource: The resource which is being accessed
   data: Data that will be populated.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_update_wrm_db
(
  wrm_resource_enum_type resource, 

  trm_req_n_not_return_data data,

  wsub_id_e_type wsub_id
);
/*===========================================================================
FUNCTION     WRM_CLIENT_GRANT_CB

DESCRIPTION
  This function calls the grant callbacks of different clients whenever W gets a grant.

DEPENDENCIES
  None.

PARAMETERS
  resource: The resource which is being accessed

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_client_grant_cb
(
  wrm_resource_enum_type resource,

  wsub_id_e_type wsub_id
);

/*===========================================================================
FUNCTION     WRM_GET_WSUB_ID

DESCRIPTION
  This function maps TRM Client Id to corresponding
  WCDMA subscription Id

DEPENDENCIES
  None.

PARAMETERS
  trm_client_enum_t client_id

RETURN VALUE
  wsub_id_e_type wsub_id

SIDE EFFECTS
  None.
===========================================================================*/
extern wsub_id_e_type wrm_get_wsub_id(trm_client_enum_t client_id);

/*===========================================================================
FUNCTION     WRM_GET_TRM_CLIENT_ID

DESCRIPTION
  This function maps WRM resource names to TRM Client Id
  For a given WCDMA subscription Id

DEPENDENCIES
  None.

PARAMETERS
  resource: The resource which is being accessed

RETURN VALUE
  trm_client_enum_t client_id

SIDE EFFECTS
  None.
===========================================================================*/
extern trm_client_enum_t wrm_get_trm_client_id(
  wrm_resource_enum_type resource,
  wsub_id_e_type wsub_id
);

/*===========================================================================
FUNCTION     WRM_GET_TRM_IRAT_CLIENT_ID

DESCRIPTION
  This function maps WRM resource names to TRM IRAT Client Id
  For a given WCDMA subscription Id

DEPENDENCIES
  None.

PARAMETERS
  resource: The resource which is being accessed

RETURN VALUE
  trm_client_enum_t client_id

SIDE EFFECTS
  None.
===========================================================================*/
extern trm_client_enum_t wrm_get_trm_irat_client_id(
  wrm_resource_enum_type resource,
  wsub_id_e_type wsub_id
);

/*===========================================================================
FUNCTION     TRM_QUERY_GRANT_STATUS

DESCRIPTION
  This function queries with trm to check grant status for TRM_UMTS, TRM_GSM1, TRM_GSM2, TRM_WCDMA, and TRM_LTE.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void trm_query_grant_status(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_TRM_REQUEST_CARRIER_AGGREGATION_RESOURCES

DESCRIPTION
  This function request TRM resource for Carrier Aggregation

DEPENDENCIES
  None.

PARAMETERS
  uarfcn: Frequency on which to request lock
  wsub_id: Current subscription ID

RETURN VALUE
  boolean: Resource was granted or not

SIDE EFFECTS
  None.
===========================================================================*/
boolean wl1_trm_request_carrier_aggregation_resources(uint16 uarfcn, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_IS_ALT_SUB_IN_DATA

DESCRIPTION
  This API queries TRM and returns if the
  alternate subscription is in DATA call

DEPENDENCIES
  NONE

RETURN VALUE
  boolean is_alt_sub_in_data;
  TRUE implies ALT SUB is in a DATA call
  FALSE implies ALT SUB is not in a DATA call

SIDE EFFECTS
  NONE
===========================================================================*/
extern boolean wrm_is_alt_sub_in_data(wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_RX_TX_SPLIT
#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION     wl1_trm_get_prev_tx_path

DESCRIPTION
  This function will return the temp Tx path

DEPENDENCIES
  None

PARAMETERS
  wsub_id: Current Subscription id

RETURN VALUE
  rfm_path_type: will return the stored temp Tx path

SIDE EFFECTS
  None
===========================================================================*/
extern rfm_path_type wl1_trm_get_prev_tx_path(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_trm_get_prev_rx_path

DESCRIPTION
  This function will return the temp Rx path

DEPENDENCIES
  None

PARAMETERS
  wsub_id: Current Subscription id

RETURN VALUE
  rfm_path_type: will return the stored temp Rx path

SIDE EFFECTS
  None
===========================================================================*/
extern rfm_path_type wl1_trm_get_prev_rx_path(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_trm_update_prev_path

DESCRIPTION
  This function will update the temp Tx and Rx path

DEPENDENCIES
  None

PARAMETERS
  wsub_id: Current Subscription id

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_trm_update_prev_path(wsub_id_e_type wsub_id);

#else /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION     wl1_trm_get_prev_tx_device

DESCRIPTION
  This function will return the temp Tx device

DEPENDENCIES
  None

PARAMETERS
wsub_id: Current Subscription id

RETURN VALUE
  rfm_device_enum_type: will return the stored temp Tx device

SIDE EFFECTS
  None
===========================================================================*/
extern rfm_device_enum_type wl1_trm_get_prev_tx_device(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_trm_get_prev_rx_device

DESCRIPTION
  This function will return the temp Rx device

DEPENDENCIES
  None

PARAMETERS
wsub_id: Current Subscription id

RETURN VALUE
  rfm_device_enum_type: will return the stored temp Rx device

SIDE EFFECTS
  None
===========================================================================*/
extern rfm_device_enum_type wl1_trm_get_prev_rx_device(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_trm_update_prev_device

DESCRIPTION
  This function will update the temp Tx and Rx device

DEPENDENCIES
  None

PARAMETERS
  wsub_id: Current Subscription id

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_trm_update_prev_device(wsub_id_e_type wsub_id);

#endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION     WRM_RELEASE_RESOURCES_FOR_PRIMARY_TX

DESCRIPTION
RF call to perform exit mode on TX device, whenever device is released

DEPENDENCIES
  None.

PARAMETERS
rfm_path: The Tx path that we are suppose to release
wsub_id: WCDMA Subscription Id for which the TX exit mode needs to be
         performed

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_release_resources_for_primary_tx(rfm_path_type  rfm_path, wsub_id_e_type wsub_id);
#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION     WRM_RELEASE_RESOURCES_FOR_PRIMARY_TX

DESCRIPTION
RF call to perform exit mode on TX device, whenever device is released

DEPENDENCIES
  None.

PARAMETERS
rfm_device: The Tx device that we are suppose to release
wsub_id: WCDMA Subscription Id for which the TX exit mode needs to be
         performed

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_release_resources_for_primary_tx(rfm_device_enum_type  rfm_device, wsub_id_e_type wsub_id);
#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
#endif /* FEATURE_WCDMA_RX_TX_SPLIT */
#ifdef FEATURE_DUAL_SIM
/*===========================================================================
FUNCTION		 WRM_RELEASE_ALL

DESCRIPTION
This will release TRM lock for all clients and init the WRM database

DEPENDENCIES
	None.

PARAMETERS	
 wsub_id -- WCDMA SUB instance

RETURN VALUE
 None.

SIDE EFFECTS
None.
===========================================================================*/
extern void wrm_release_all(wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION      WRM_RELEASE_EXCHANGED_TX_LOCK_IF_NOT_NEEDED

DESCRIPTION
This will release TRM tx lock if RRC client is in no lock 

DEPENDENCIES
  None.

PARAMETERS
 wsub_id -- WCDMA SUB instance

RETURN VALUE
 None.

SIDE EFFECTS
None.
===========================================================================*/

void wrm_release_exchanged_tx_lock_if_not_needed(wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION     WL1_TRM_GET_CURRENT_BAND

DESCRIPTION
  This function is used to get the current band WL1 is holding lock for

DEPENDENCIES
  None.

PARAMETERS  

RETURN VALUE
  Returns current band W has lock for

SIDE EFFECTS
  None.
===========================================================================*/
extern rfcom_wcdma_band_type wl1_trm_get_current_band(rfm_device_enum_type device_id, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_GET_TRM_REASON_FOR_ACTIVITY

DESCRIPTION
  Function is called to get the TRM reason corressponding to a WRM activity.
Also considers priority inversion conditions and returns an appropriate
reason.

DEPENDENCIES
  None.

PARAMETERS  

RETURN VALUE
  TRM activity for a given WRM activity

SIDE EFFECTS
  None.
===========================================================================*/
extern trm_reason_enum_t wrm_get_trm_reason_for_activity(wrm_activity_enum_type activity, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_EXTEND_LOCK_FOR_ACTIVITY

DESCRIPTION
  Function is by WRM clients to request lock extension for a new activity.
  If the activity passed is invalid, WRM will pick the clients current 
  activity and perform the modify_activity and modify_duration

DEPENDENCIES
  None.

PARAMETERS  
 wrm_client_id -- Client which is requesting for extension
 req_activity -- Activity related to extension
 duration -- extend duration
 wsub_id -- WCDMA SUB instance

RETURN VALUE
  Boolean
  TRUE - Lock extension granted (possibly for new activity)
  FALSE - Lock extension failed

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wrm_extend_lock_for_activity(
  wrm_primary_client_enum_type wrm_client_id, wrm_activity_enum_type req_activity, 
  wrm_duration_type duration, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     WRM_START_WRM_TIMEOUT_TIMER_FOR_SPLIT_ACQ

DESCRIPTION
  This will start the wrm_timeout_timer while doing the req_n_not during split_acq.

DEPENDENCIES
  None.

PARAMETERS  
 wsub_id -- WCDMA SUB instance

RETURN VALUE
 None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_start_wrm_timeout_timer_for_split_acq( 
  wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     WRM_STOP_WRM_TIMEOUT_TIMER_FOR_SPLIT_ACQ

DESCRIPTION
  This will stop the wrm_timeout_timer while doing the release for split_acq.

DEPENDENCIES
  None.

PARAMETERS  
 wsub_id -- WCDMA SUB instance

RETURN VALUE
 None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_stop_wrm_timeout_timer_for_split_acq( 
  wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION      WRM_RELEASE_TX_LOCK_IF_NOT_NEEDED

DESCRIPTION
This will release TRM tx lock if UL PHYCH is not active

DEPENDENCIES
  None.

PARAMETERS
 wsub_id -- WCDMA SUB instance

RETURN VALUE
 None.

SIDE EFFECTS
None.
===========================================================================*/

extern void wrm_release_tx_lock_if_not_needed(
wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION   WRM_IS_WCDMA_IN_SR_MODE

DESCRIPTION
  Function queries if WCDMA is holding TRM Lock with SR Reason 

DEPENDENCIES
  None

PARAMETERS
  None 

RETURN VALUE
  booelan - TRUE if W is having lock with SR Reason
            FALSE if W is having lock with DR Reason

SIDE EFFECTS
 FALSE from this API doesn't guarrantee that we are in DR Mode.
============================================================================*/
extern boolean wrm_is_wcdma_in_sr_mode(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      WRM_DOES_L1_HAVE_LOCK_FOR_SERVING_BAND

DESCRIPTION
This will tell if L1 client has lock for serving band or not

DEPENDENCIES
  None.

PARAMETERS
 wsub_id -- WCDMA SUB instance

RETURN VALUE
Boolean - Holding lock or not

SIDE EFFECTS
None.
===========================================================================*/
extern boolean wrm_does_l1_have_lock_for_serving_band(wsub_id_e_type wsub_id);
/*===========================================================================
 FUNCTION  wl1_ds_update_duty_cycle

 DESCRIPTION
   This function updates the variable to increase/decrease the duty cycle 
 
 DEPENDENCIES
   None

 PARAMETERS
    increase_duty_cycle: duty_cycle will be increased if it is TRUE.

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
void wl1_ds_update_duty_cycle(boolean update_duty_cycle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      WRM_BLOCK_MEAS_ACTIVITY

DESCRIPTION
  This api is used to block neighbor measurements when we already have TX lock or
  when TX lock activity is pended to be handled. In both cases as UE moves out of 
  Idle states, neighbor measurements aren't needed so they would be blocked.

DEPENDENCIES
  None.

PARAMETERS
 wsub_id -- WCDMA SUB instance

RETURN VALUE
Boolean - TRUE blocks measurement

SIDE EFFECTS
None.
===========================================================================*/
extern boolean wrm_block_meas_activity(wsub_id_e_type wsub_id);


/*============================================================================
FUNCTION   WRM_GET_WCDMA_LOCK_REASON_IN_TRM

DESCRIPTION
  Function queries the current TRM reason WCDMA client is holding lock with.

DEPENDENCIES

PARAMETERS
  resource - WCDMA Antenna resource type (Primary/Secondary/CA)

RETURN VALUE
  TRM reason - Reason with which current client is holding lock at TRM

SIDE EFFECTS
  None
============================================================================*/
extern trm_reason_enum_t wrm_get_wcdma_lock_reason_in_trm(wrm_resource_enum_type resource, wsub_id_e_type wsub_id);


#ifdef FEATURE_WCDMA_MSIM_TEST_FRAMEWORK
/*===========================================================================
FUNCTION      wrm_register_els

DESCRIPTION
  API to register ELS paramters with trm

DEPENDENCIES
  None.

PARAMETERS
  wsub_id -- WCDMA SUB instance

RETURN VALUE:
  None
SIDE EFFECTS
None.
===========================================================================*/
void wrm_register_els(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      wrm_els_update

DESCRIPTION
  API to update ELS paramters with trm

DEPENDENCIES
  None.

PARAMETERS
  wl1_els_state_info -- pointer to struct trm_els_state_info
  wrm_activity       -- WRM activity
  wsub_id            -- WCDMA SUB instance

RETURN VALUE
  Boolean - TRUE: If ELS updation with TRM was success.
            FALSE: If ELS update failed with TRM.

SIDE EFFECTS
None.
===========================================================================*/
boolean wrm_els_update(trm_els_state_info *wl1_els_state_info, wrm_activity_enum_type wrm_activity, wsub_id_e_type wsub_id);
#endif /* WCDMA_ELS */

/*===========================================================================

FUNCTION wrm_idle_mode_register_aps_info

DESCRIPTION
  This function indicates to TRM which reasons to use APS (adaptive page
  skipping).Currently only for BG_TRAFFIC

DEPENDENCIES
  None 

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void wrm_idle_mode_register_aps_info(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wrm_conn_mode_register_aps_info

DESCRIPTION
  This function indicates to TRM which reasons to use APS (adaptive page
  skipping).Currently only for BG_TRAFFIC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void wrm_conn_mode_register_aps_info(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  wrm_idle_mode_update_aps_support

DESCRIPTION
  Enable/disable Adaptive Page Skipping support to be called by Idle Sub

DEPENDENCIES
  Requires API support for APS toggle from TRM

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void wrm_idle_mode_update_aps_support(const boolean aps_state, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  wrm_conn_mode_update_aps_support

DESCRIPTION
  Update APS to TRM. This may enable/disbale to page skip on other tech

DEPENDENCIES
  Requires API support for APS toggle from TRM

RETURN VALUE
  Void

SIDE EFFECTS
  None
=========================================================================*/
void wrm_conn_mode_update_aps_support(
 wrm_aps_client_enum_t aps_client_id, const boolean enable,  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      WRM_IS_SRLTE_ENABLED

DESCRIPTION
  This api is used to determine if srlte is enabled or not

DEPENDENCIES
  None.

PARAMETERS
 wsub_id -- WCDMA SUB instance

RETURN VALUE
Boolean - TRUE srlte enabled is active

SIDE EFFECTS
None.
===========================================================================*/
boolean wrm_is_srlte_enabled(wsub_id_e_type wsub_id);
#ifdef FEATURE_PBR_RANDOMIZATION
/*============================================================================
FUNCTION WRM_INFORM_GRANT_EVENT

DESCRIPTION


DEPENDENCIES
  None

PARAMETERS


RETURN VALUE


SIDE EFFECTS

============================================================================*/

extern void wrm_inform_grant_event(uint8 wrm_client_id,
                            wrm_resource_enum_type resource,
                            wrm_activity_enum_type activity,
                            boolean grant,
                            wsub_id_e_type wsub_id);

#endif
/*===========================================================================
FUNCTION     WRM_SEND_STACK_STATUS_TO_TRM

DESCRIPTION
  The function is called at stack START/RESUME/STOP to inform TRM about the status. TRM uses this
  information to evaluate multi-TECH concurrency restrictions for FW.

DEPENDENCIES
  None.

PARAMETERS  
 wrm_stack_status  - Current stack state for WCDMA SUB
 wsub_id -- WCDMA SUB instance

RETURN VALUE
 None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_send_stack_status_to_trm(wrm_stack_status_enum_type status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      WRM_IS_ALT_SUB_LTE

DESCRIPTION
  The function is called at 10ms before FMO starts and is used to check if LTE on the other SUB has requested WCDMA to open a LTA gap.
  None.

PARAMETERS  
 wsub_id -- WCDMA SUB instance

RETURN VALUE
 boolean ret_val

SIDE EFFECTS
  None
===========================================================================*/
extern boolean wrm_is_alt_sub_lte(wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION      qsh_mdump_collect_trm_int_cntrl_params

DESCRIPTION
  API to collect the dumps for the trm intrl cntrl params

DEPENDENCIES
  None.

PARAMETERS
wsub_id - Subscription ID

RETURN VALUE
None

SIDE EFFECTS
None
============================================================================*/
QSH_MDUMP_FN_ATTR void qsh_mdump_collect_trm_int_cntrl_params(wsub_id_e_type wsub_id);
#endif
#endif /* WL1TRM_H */
