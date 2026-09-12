#ifndef GPL1_GRM_INTF_H
#define GPL1_GRM_INTF_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

      L 1 GRM GENERAL HEADER FILE

GENERAL DESCRIPTION
This module handles GERAN-to-TRM general purpose functions

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/gpl1_grm_intf.h#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
12/03/18   sp        CR2202251 Dont extend DRx chain beyond Prx Chain. Also Drx release can be inline
06/07/17   sp        CR2071477 Push loosing client's TRM request of PCHxFER to Task to avoid load on ISR.
31/10/17   sp        CR2125524 Update frame duration in task context when unlock_cancell is received
03/10/17   sp        CR2115832 Avoid MCPM State transitions in FIND_BCCH if EMBMS is Active on other sub (to utilize available gaps effectively)
23/03/16   smd       CR992904 Added a debug crash if GL1 holds tx device without rx device.
09/09/16   sn        CR1062272 TRM RESERVE AT during IDLE to transfer
20/12/17   sk        CR2030816 GERAN changes for PBR Randomization algorithm
15/12/17   ap        CR1103567 FR36133: Extended LTE Signaling (ELS) to manage tune-away gaps in DSDS 
12/06/17   sn        CR2054908 Handling to back to back DR mode change requests
14/03/16   sn        CR982754 offload TRM exchange operation from ISR to L1_TASK
07/06/16   snjv      CR1019245 Do RnN instead of TRM Req(ISR) and do a grouped release when suitable
20/07/16   sp        CR1040702: Allow TA during 2Phase access
29/03/16   mn        CR995137 Access tune away support: Allowing other tech's paging at GPRS access
29/10/17   km        CR2133917 GL1 support for TA.3.0 L+L
24/10/16   snjv      CR1081627 Gl1 to call grouped Reserve At instead of separate calls to DRx and tx clients
02/02/17   br        CR1103497 FR 39735: Allowing TA during PAGE RE-ORG mode in GERAN
15/07/16   gmr       CR1039177 Declaration of TRM callback function to include more Requests  
12/07/16   tsk       CR987216 For SSIM Idle mode RxD should be gated by Idle mode RxD NV.
12/07/16   tsk       CR978200 Support Dynamic Rxd with SSIM and WLAN Antenna Sharing configuration.
01/02/16   bg        CR951395 Skip QTA if the allocated device does not support band
22/01/16   sp        CR957208 Fetch bands supported for Diversity Chain on the granted device.
12/16/15   tsk       CR951099 TRM grant status should consider secondary client id for GBTA during TRM release.
10/12/15   sp        CR948656 Check and update trm reserve reason if CB prio change is triggered by RR.
02/12/15   ap        CR939311 Change reason from ACCESS to TRAFFIC only when DRDSDS disabled 
18/11/15   cah       CR900214 - [WLAN+WAN] Failed in acquiring the Rxd chain in Single SIM Mode during Voice Call Attempt
05/11/15   cah       CR934800 call DR disable in task context when PG is received from ACCESS
28/09/15   cjl       CR929471 Delay sleep after geran lock release in idle
20/10/15   snjv      CR927604 Update duration for DR Band registration in G2W Ded search
15/10/15   ap        CR923459 GSM logging procedure for Attach seen as CS instead of PS registration
14/10/15   cc        CR917262 Delay release IRAT band to search done for G2W dedicated search
18/09/15   nm        CR908307 Back out CR888325
21/09/15   nm        CR767743 Using actual duration in trm unlock cb for unlock by event
07/09/15   js        CR903209 Backing out CR891616
08/09/15   cah       CR892575: DR DSDS: Add support for diversity chain unlock in transfer mode
04/09/15   cws       CR901647 Use RFM_MAX_WAN_DEVICES as the max valid device
20/08/15   pa        CR893756:Grouped TRM apis for the diversity chain. 
20/08/15   nm        CR888325 Improve Debug F3 if idle sub page collides with transfer sub Ncell SCH/FCCH
26/08/15   smd       CR896555 Don't start BPLMN in QTA gap.
12/08/15   pa        CR887572: RxD: Schedule DRX wakeup after Rx/Tx and request TRM in task context.
27/08/15   cah       CR894323: Request for drx chain not populated with correct frequency information. 
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
26/08/15   js        CR891616 Setting correct duration to tune away table  
19/08/15   sjw       CR893046: send MPH_ACQUISITION_STATE_IND for all acquisition grants
07/08/15   cjl       CR877672 Handle priority inversion for TRM_RESELECT use cases
10/08/15   sp        CR884488: Correctly Resume BCCH List in BPLMN state machine after page decode
05/08/15   hd        CR884936: Handle QTA STARTUP and CLEANUP messages in MSGR IF
05/08/15   cws       CR881986 Use only gsm1/gsm2 client for trm asdiv request
17/07/15   ab        CR 871981 Address Static Analysis Warning.
29/07/15   zf        CR877814: Set TA perference for diversity chain
28/07/15   smd       CR845682 Skip IFS if GSM is granted a new device than the current device
21/07/15   cjl       CR876903 Use system procedure types for TRM subreasons 
23/07/15   fan/akh   CR865360 Use correct TRM reservation period when doing wakeup cancelation
21/07/15   cws       CR873163 fix asdiv/pchxfer compile warnings
21/07/15   cjl       CR876741 Replace TRM subreasons with system procedure types 
10/07/15   pa        CR866748: Circular buffer for grouped TRM input structures.
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
24/06/15   ap        CR866727 TRM U:support of Acquisition Cell Select as DR and Acquisition as SR 
17/06/15   ab        CR847473 : unify usage of gl1_map_gas_id_to_client_id for TX_ANT_DIV/DR_DSDS/RX_DIV/PCHXFER/AS_DIV
06/01/15   js        CR814275 GL1 changes for ROTA interface during extended paging
10/06/15   sjw       CR850737: variable connection priority for TRM-Unification
22/04/15   am        CR803948 TA in idle mode when mm_non_drx is enabled
20/04/15   sjv       CR823566 Set number of TRM denials/Xcch failures count through debug NV 
27/03/15   dv/pa     CR813849: In X+G DR-DSDS, revert back to SR after 3 consequtive TRM denials  
30/04/15   ap        CR811057 FR22272 - TRM Unification  - Logging Changes
23/04/15   cjl       CR822231 Cross WTR IRAT reselection changes to GL1 exchange mechanism
15/04/15   pa        CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
16/04/15   aga       CR822202  TRM interface for QTA
07/04/15   cjl       CR811729 FR22272 - TRM Unification - GL1 changes to allow lock retention using GRM
10/04/15   cgc       CR820213 Add grm_register_additional_irat_bands() for G2L fast search.
10/04/15   cah       CR819666:Problem in the logic for adding the denied bands to trm_reserve_at and trm_request
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
30/03/15   cs        CR813680 Remove polling mechanism for modify chain state
30/03/15   nm        CR765891 Allowing TA for TDS during PS RACH 
27/03/15   ap        CR811054 FR22272 - TRM Unification  - Extended Paging with PBR
24/03/15   ap        CR811053:FR22272 - TRM Unification  - Access and Traffic attempts
23/03/15   ap        CR811046:FR22272 - TRM Unification  - Removal of _HI reasons
12/03/15   pa        CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
25/02/14   cah       CR787616 Updates to Persistent Denial mechanism. reduce amount of inter-band power monitors in good serving cell conditions
20/02/15   sjw       CR793380: adaptive TRM durations for acquisition mode
04/02/15   nk        CR767734 Allowing TA during 2-phase access
18/02/15   cah       CR780986: TRM_PENDING not handled by grm_change_reason api.
16/02/15   sp        CR772306: Update TRM of gbta grant for PBR to work
13/02/15   pa        CR756156:G2X RF device handling using TRM apis.
16/10/14   aga       CR735726  Remove obsolete gta_set_wcdma_blacklisted functionality
21/01/15   pa        CR758792:RxD support for multi-sim.
12/01/15   sjv       CR756154 Enhance Band Registration for Activities
06/01/14   jk        CR773695:DRx device Id not requested for all bands
12/12/14   zf        CR770728: Adding function to call in task GRM release without requesting GL1 lock
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
04/12/14   sjv       CR766784 Backing out CR759568
25/11/14   zc/zf/sj  CR759568 Delay TRM release if exiting QBTA
30/10/14   pa        CR748275: In DRDSDS (Transfer Mode): allow IRAT if band registration is successful.
29/10/14   ab        CR745429 - Featurization correction for PCHXFER for SS / CR merges.
28/10/14   sjv       CR747201 Handle multiple calls for grm_reserve from task message queue (made from l1_fm_grm_reserve)
28/10/14   ss        CR746380 Set TRM extention flag outside of grm intialisation function.
24/10/14   sjv/cah   CR744418  Get serving cell band in freq info for demod_page
23/10/14   pa        CR744906: Allow one IRAT DRDSDS activity at a time.
22/10/14   cah       CR737592: optimisation for persistent denial based on Scell RSSI and SnR
17/10/14   cs        CR735918 Assign supported bands for initial ACQ based on UE support
15/10/14   pa        CR739469: Add band information in trm request for NCELL activities.
14/10/14   pa        CR738110 Store requested TRM reason in the local structure on Request and Notify/Enhanced.
08/10/14   pa        CR732823: Hybrid Tuneaway Fallback: Avg RSSI is an avergae of last 3 PCH blocks.
02/10/14   sjv       CR733766 Get RF Device from grm_info instead of querying TRM
26/09/14   cah       CR729408 Reduce number of TRM API calls in GL1 to reduce impact of processing delays on GL1
22/09/14   ap        CR720862: FR 20912: SGLTE+G DSDA with WTR asymmetric GSM support
17/09/14   sjv       CR724558 For PM, TRM band-reg and release at frame level granularity for perf. optimization
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
22/09/14   cah       CR723144: Bolt 2.0 DRDSDS - W+G - G2G interband ncell monitor activity denied while in W+G FC mode
10/09/14   cs        CR722344 Remove old deprecated TRM API usage
09/09/14   pa        CR719024: Implementation of Hybrid Tuneaway Fallback Algorithm
28/08/14   sjv       CR717166 pending activities are organized in an array for trm band release to avoid overwrite in case of diff. activities
26/08/14   pa        CR715814 IFS measured client id is not updated from GL1 before passing to TRM
21/08/14   dv        CR711417 Ensure DR mode is enabled when camping on cell
12/08/14   cah       CR707445 Overlap between PM's in PTM mode causes transaction id corruption
06/08/14   ka        CR699964 Implement Idle RXD support in GL1
31/07/14   cos       CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
24/07/14   ka        CR692214 TRM APS feature changes required with new TRM api's
27/07/14   dv        CR700582 Ensure peending IFS/IRAT activity cleared if trm extension denied
25/07/14   cs        CR697245 New GRM API support for WTR Hopping
23/07/14   cah       CR697586  trm_client.cpp:4539   Assertion  search_activity_q.cnt == 0 failed
16/07/14   ssh       CR689277: On wake up cancel do trm release only if TRM status is denied and TRM/GRM reserve reasons match
14/07/14   cs        CR693967 Get DR-DSDS mode during startup
02/07/14   cs        CR688003 First version

===========================================================================*/

/*===========================================================================

           INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"
#include "ms.h"
#include "grm_type.h"
#include "l1_fm.h"

#include "rfm_device_types.h"

#include "sys_type.h"

#include "l1i.h"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#include "gl1_drdsds.h"

/*===========================================================================

           Enums, Macros Hash and Structure Definitions

===========================================================================*/
/*TRM AND RFSW are not aligned to use 5 bands. RFSW uses PGSM/EGSM as EGSM only. 
  Therefore, we will still have only 4 bands.*/
#define MAX_BAND_INFO_GRM                                      (4)

/*
* For a particular target what is the maximum valid RF device id
* that can be assigned to GERAN WAN/WCDMA WAN and LTE WAN
* these can be different
*/
#define MAX_VALID_GERAN_RFM_DEVICE         RFM_MAX_WAN_DEVICES
#define MAX_VALID_WCDMA_RFM_DEVICE         RFM_MAX_WAN_DEVICES
#define MAX_VALID_LTE_RFM_DEVICE           RFM_MAX_WAN_DEVICES

#define TRM_BAND_TUNE_DENIED                                    (0x1)
#define TRM_BAND_TUNE_OK                                        (0x0)


#define IS_VALID_GRM_CLIENT(client)     (((client) >= GRM_1X) && ((client) <= GRM_LAST_CLIENT))

/* Threshold in dBmx16 -70dBm = -70*16*/
#define GRM_PERSISTENT_DENIAL_RSSI_THRESHOLD (-1120)
/*6dB => 10 dBx16*/
#define GRM_PERSISTENT_DENIAL_SNR_THRESHOLD  (160)

/*worst case number of paging blocks required to PM on all bands in BA list*/
#define DEFAULT_DEMOD_PAGE_COUNT                                (8)

/*======================TRM DURATIONS========================*/
/* approx 10 frames */
#define GRM_PSCN_MIN_REQUEST_MS                                 (45)
#define GRM_PSCN_MAX_REQUEST_MS             (100)
/* approx 14 frames */
#define GRM_BCCH_MIN_REQUEST_MS                                 (45)
#define GRM_BCCH_MAX_REQUEST_MS             (100)
#define GRM_ACCESS_DURATION_SCLKS           (300) /* approx 2 frames */
#define GRM_RXD_REQ_DURATION_SCLKS          (900) /*approx 6 frames*/
#define GRM_ACCESS_DURATION_PS_SCLKS        (6500) /*approx 200ms/approx 44 frames */

#define GRM_INITIALISED_MFRMS                                   (0xFF)
#define GRM_SCLKS_SCALING_FACTOR                                (160)

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  /* Set to 10ms for IDLE warmpup to have TRM access granted atleast for 2+ frames. */
  #define GRM_IDLEWARMUP_TRM_ACCESS_DURATION_SCLKS   (330)
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
/*======================TRM REASONS========================*/
#define CHECK_INCLUDES_WCDMA_ACQUISITION                       TRM_ACQUISITION

/*Header for F3's related to GRM Transaction Manager*/
#define GRM_TM_HDR "GRM_TRANS_MAN: "

/*Max.  possible active transactions of a type*/
#define GRM_ACT_MAX                                            (4)
/*Only 1 IRAT Activity is possible at one time so index will always be 0*/
#define GRM_ACT_IRAT                                           (0)
#define GRM_INVALID_ACT_INDEX                                  (-1)
/*If Diversity is needed or Not*/
#define DIVERSITY_FALSE                                        (0)
/*Max Duration Allowed for an activity, it is set to 32 as for W Irat in 
  Idle has this, and band reg for Irat is done for duration+1, we add 1 for Max here so 30+1+1*/
#define MAX_DURATION_GRM_ACT                                   (32)

#define INVALID_SCLKS                                          (0xFFFF)
#define SCLKS_PER_MS                                            32




#ifdef FEATURE_GSM_RX_DIVERSITY
#define MAX_NUM_GRM_DRX_CLIENTS     NUM_GERAN_DATA_SPACES
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define MAX_NUM_GRM_BPLMN_CLIENTS   NUM_GERAN_DATA_SPACES
#else
#define MAX_NUM_GRM_BPLMN_CLIENTS   (0)
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/


#ifdef FEATURE_GSM_RX_DIVERSITY
/*
 * data space for the RX_DIV client
 */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define MAX_NUM_GRM_CLIENTS     (NUM_GERAN_DATA_SPACES + MAX_NUM_GRM_DRX_CLIENTS + MAX_NUM_GRM_BPLMN_CLIENTS)
#else
#define MAX_NUM_GRM_CLIENTS     (NUM_GERAN_DATA_SPACES + MAX_NUM_GRM_DRX_CLIENTS)
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
#else
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define MAX_NUM_GRM_CLIENTS     (NUM_GERAN_DATA_SPACES + MAX_NUM_GRM_BPLMN_CLIENTS)
#else
#define MAX_NUM_GRM_CLIENTS     NUM_GERAN_DATA_SPACES
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
#endif /* FEATURE_GSM_RX_DIVERSITY*/
#define GRM_ACQUISITION_REASON(rsn) (((rsn) == TRM_ACQUISITION) || ((rsn) == TRM_ACQUISITION_INV))
/* Dual Receive DSDS define */
#define GRM_ACQUISITION_DR_REASON(rsn) (((rsn) == TRM_ACQUISITION_CELL_SELECT ) || ((rsn) == TRM_ACQUISITION_CELL_SELECT_INV))
#define GRM_INVERTED_ACQ_REASON(rsn) (((rsn) == TRM_ACQUISITION_INV) || ((rsn) == TRM_ACQUISITION_CELL_SELECT_INV))

#define GRM_PAGING_REASON(pr) ((pr) == TRM_DEMOD_PAGE)

/*Data space for ISR and Task*/
#define GRM_ISR_TASK_DATA_SPACE     (2)

typedef enum
{
  GRM_START_PERSISTENT_DENIAL_MONITORING,
  GRM_CHECK_PERSISTENT_DENIAL_FLAG,
  GRM_DECREMENT_DENIED_REG_COUNT,
  GRM_RESET_PERSISTENT_DENIAL_MONITORING,
  GRM_ADD_DENIED_BANDS_TO_FREQ_INFO,
  GRM_COUNT_DENIED_ARFCNS,
  GRM_PROCESS_DENIED_ARFCNS,
  GRM_GOOD_SERVING_CELL_CHECK,
  GRM_INIT_SERVING_CELL_LEVELS
}grm_persistent_denial_sm_actions_t; 

typedef enum {
  /* GRM General */
  GRM_REQUEST          = 0x10,
  GRM_RELEASE,
  GRM_RESERVE_AT,
  GRM_RETAIN_LOCK,

  /* GRM Request and Notify */
  GRM_RAN_ACCESS_RETRY,

  /* GRM Request and Notify Enhanced */
  GRM_RANE_DENIED,
  GRM_RANE_ACQUISITION,
  GRM_RANE_RESELECTION,
  GRM_RANE_TBF_SUSPEND,
  GRM_RANE_GPRS_PRIORITY_IND,
  GRM_RANE_PANIC_RESET,

  /* GRM Get Info */
  GRM_GET_GRANTED,
  GRM_GET_AS_ID_MAPPING,
  GRM_GET_REASON,
  GRM_GET_CHAIN_HOLDER,
  GRM_GET_DEVICE_MAPPING,
  GRM_GET_DR_CAPABILITY,
  GRM_GET_DEVICE_CONFIG,
  GRM_GET_DEVICE_SUPPORT_INFO,
  GRM_GET_RAT_ACCESS_INFO,

  /* GRM Modify Chain State */
  GRM_MODIFY_BAND,
  GRM_MODIFY_DURATION,
  GRM_MODIFY_REASON,

  /* GRM Dual Receive Info */
  GRM_DR_INFO,

  /* Inform GRM to share same Chain*/
  GRM_INFORM_GRANT_EVENT_INFO,
#ifdef FEATURE_GSM_RX_TX_SPLIT
  /* handles modify reason to traffic denial */
  GRM_RAN_MODIFY_TO_TRAFFIC_REASON,
#endif /* FEATURE_GSM_RX_TX_SPLIT */
  GRM_TAG_MAX
} grm_tag_t;

typedef enum
{
    PENDING_DR_DISABLE,
    PENDING_DR_ENABLE,
    PENDING_DR_NULL,
}grm_pending_dr_state;

typedef enum
{
  SC_BCCH,
  SC_PM,
  SC_FCH_SCH,
  SC_IRAT,
  SC_MAX_ACTIVITIES
}gl1_sc_activities_t;

typedef enum
{
  GRM_DIVRX_TRM_REQ_INVALID = -1,
  GRM_DIVRX_TRM_REQ_INIT,
  GRM_DIVRX_TRM_REQ_IDLE,
  GRM_DIVRX_TRM_REQ_DED,
  GRM_DIVRX_TRM_REQ_DATA
}grm_divrx_request_type;

typedef enum
{
  GRM_SLOW_MEAS_CYLE,
  GRM_QUICK_MEAS_CYCLE,
  GRM_NORMAL_MEAS_CYLE
} grm_persistent_denial_meas_cycle_t;

/*transaction manager events*/
typedef enum
{
    GRM_TM_CHECK,
    GRM_TM_FLUSH
}grm_tm_event_t;

/*Enum defined for Frames required in general*/
enum
{
    GRM_FRM_REQ_PM_XFER = 2,
    GRM_FRM_REQ_PM_IDLE = 3,
    GRM_FRM_REQ_FCH=3,
    GRM_FRM_REQ_SCH=3,
    GRM_FRM_REQ_BCCH =4
};

typedef enum
{
  DEVICE_CONFIG_DRDSDS,
  DEVICE_CONFIG_NON_DRDSDS,
  DEVICE_CONFIG_NONE
} grm_device_configuration_t;

typedef enum
{
  PS_ACCESS_NONE,  /* Not PS access */
  PS_ACCESS_MM_DEVICE,/* PS access on multimode device (DEV 0)*/
  PS_ACCESS_NON_MM_DEVICE   /* PS access on non multimode device (! DEV 0)*/
} ps_access_mode_t;


typedef enum
{
  GRM_CS_CONNECTION,
  GRM_PS_CONNECTION
} grm_conn_type_t;

typedef enum
{
  GRM_RESOURCE_LOCK_INACTIVE,
  GRM_RESOURCE_LOCK_ACTIVE,
  GRM_RESOURCE_LOCK_GOING_INACTIVE
} grm_resource_lock_t;

#define GRM_CS_DEFAULT TRM_TRAFFIC
#define GRM_PS_DEFAULT TRM_BG_TRAFFIC

typedef enum
{
  /* unlock callbacks will ignore UNLOCK_REQUIRED events */
  GRM_IGNORE_UNLOCK_REQUIRED,
  /* unlock callbacks will handle UNLOCK_REQUIRED events */
  GRM_ALLOWS_UNLOCK_REQUIRED
} grm_unlock_mode_t;


#ifdef FEATURE_GSM_RX_TX_SPLIT
typedef enum
{
  GRM_RX_TX_NULL,
  GRM_RX_TX_REQUESTED,
  GRM_PARTIAL_GRANT,
  GRM_COMPLETE_GRANT,
  GRM_PARTIAL_UNLOCK,
  GRM_COMPLETE_UNLOCK
}grm_rx_tx_status_t;
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
/**
 * Structure housing the input and output Data Types for 
 * trm_set_client_state API with TRM  
 */
typedef struct 
{
  trm_set_client_state_input_type  client_state_info; /**< the input Data*/
  trm_set_client_state_output_data trm_return_data;   /**< the output Data */
} grm_set_client_state_info_t;

typedef struct
{
  trm_set_client_state_input_type  client_state_info;
  trm_set_client_state_output_data trm_return_data;
    /*Transaction id*/
  uint32                           trans_id_act;
  uint32                           start_frame;
  uint32                           duration;
  boolean                          update_in_progress;
  boolean                          pending_meas_release;
} grm_sc_activity_state_info_t; /*stores the TRM state information per GL1 SC activity.*/

typedef struct
{
  uint8 band_tune_result;
  uint8 arfcn_count;
  boolean band_registered;
  boolean band_on_new_dev; /* whether IFS device is a new one than paging device */
  boolean next_pch_block; /*denied band has will be measured in next PCH block*/ 
}grm_denied_band_info_t;

typedef struct
{
  /*array holding all bands which G can not tune RF to (indixed by SYS_BAND type)*/
  grm_denied_band_info_t             denied_band_info[INVALID_BAND];
  grm_persistent_denial_meas_cycle_t meas_cycle;
  /* number of denied arfcns per band */ 
  uint8                              denied_arfcn_count;
  uint8                              pch_ticks_to_include_denied_bands;
  /*number of denied TRM requests before enabling persistent denial handling*/
  uint8                              request_denied_count;
  /*Flag indicating if the persistent denial algorithm is running*/
  boolean                            persistent_denial_enabled;
}grm_persistent_denial_t;

typedef enum
{
  GRM_NULL_COMMAND,
  GRM_RANE_CLEAR,
  GRM_REQUEST_CLEAR
}grm_pd_command_t;


typedef struct
{
  IMH_T                           header;
  grm_client_enum_t               client_id;
  trm_resource_enum_t             resource;
  trm_duration_t                  duration;
  trm_reason_enum_t               reason;
  sys_proc_type_e_type            sub_reason;
  grm_divrx_request_type          divrx_req;
  gas_id_t                        gas_id;
} grm_request_data_t;

typedef struct
{
  IMH_T                           header;
  grm_client_enum_t               client_id;
  trm_resource_enum_t             resource;
  trm_duration_t                  duration;
  trm_reason_enum_t               reason;
  sys_proc_type_e_type            sub_reason;
  grm_tag_t                       grm_tag;
  gas_id_t                        gas_id;
} grm_request_and_notify_enhanced_data_t;

typedef struct
{
  IMH_T                           header;
  grm_client_enum_t               client_id;
  grm_tag_t                       grm_tag;
  gas_id_t                        gas_id;
} grm_release_data_t;

typedef struct
{
  IMH_T                           header;
  grm_client_enum_t               client_id;
  boolean                         dr_setting;
  grm_tag_t                       grm_tag;
  gas_id_t                        gas_id;
} grm_disable_enable_dr_t;

typedef struct
{
  IMH_T                           header;
  grm_client_enum_t               client_id;
  trm_time_t                      when;
  trm_duration_t                  duration;
  uint32                          frames_required;
  uint32                          frame_duration;
  trm_reason_enum_t               reason;
  sys_proc_type_e_type            sub_reason;
  l1_fm_priority_T                pri;
  trm_resource_enum_t             resource;
  gas_id_t                        gas_id;
} grm_reserve_at_data_t;

typedef struct
{
  IMH_T                   header;
  grm_client_enum_t       client;
  trm_unlock_event_enum_t event;
  uint32                  unlock_by_sclk;
  gas_id_t                gas_id;
#if defined( FEATURE_G2X_IDLE_TUNEAWAY) ||defined( FEATURE_G2X_REORG_TUNEAWAY)
  uint32                  unlock_dur;
#endif
  uint32                  frames_grant;
} gl1_grm_abort_callback_t;

typedef struct
{
  IMH_T                      header;
  grm_client_enum_t          client;
  trm_grant_return_enum_type grant_status;
  trm_duration_t             duration;
  gas_id_t                   gas_id;
} gl1_grm_grant_callback_t;

#ifdef FEATURE_G2X_TUNEAWAY
typedef struct
{
  IMH_T                   header;
  grm_client_enum_t       tbf_client_id;
  grm_client_enum_t       winning_client_id;
  trm_reason_enum_t       winning_client_reason;
  trm_unlock_event_enum_t event;
  uint32                  unlock_in_sclk;
  uint32                  winning_client_duration;
  gas_id_t                gas_id;
  trm_tuneaway_enum_type  ta_info;
} gl1_grm_preemption_abort_callback_t;
#endif /*FEATURE_G2X_TUNEAWAY*/

#ifdef FEATURE_GTA_2PHASE_ACCESS
typedef struct
{
  IMH_T                   header;
  grm_client_enum_t       tbf_client_id;
  grm_client_enum_t       winning_client_id;
  trm_unlock_event_enum_t event;
  uint32                  unlock_in_sclk;
  gas_id_t                gas_id;
} gl1_grm_2phase_access_abort_callback_t;
#endif /* FEATURE_GTA_2PHASE_ACCESS */

#ifdef FEATURE_GTA_DURING_PS_RACH
typedef struct
{
  IMH_T                   header;
  grm_client_enum_t       winning_client_id;
  trm_reason_enum_t       winning_client_reason;  
  trm_unlock_event_enum_t event;
  uint32                  unlock_in_sclk;
  gas_id_t                gas_id;
} gl1_grm_rach_ps_gta_abort_callback_t;
#endif /*FEATURE_GTA_DURING_PS_RACH*/

#ifdef FEATURE_GSM_RX_DIVERSITY
typedef struct
{
  IMH_T                   header;
  grm_client_enum_t       diversity_client_id; 
  grm_client_enum_t       winning_client_id;     /*For future use*/
  trm_reason_enum_t       winning_client_reason; /*For future use*/
  trm_unlock_event_enum_t event;                 /*For future use*/
  uint32                  unlock_in_sclk;        /*For future use*/
  gas_id_t                gas_id;
} gl1_grm_diversity_unlock_callback_t;

#endif 

#ifdef FEATURE_GSM_RX_TX_SPLIT
typedef struct
{
  /*Tx Client TRM status*/
  gl1_trm_state_t                    tx_trm_status;
  trm_grant_return_enum_type         tx_trm_grant_status;
  trm_reason_enum_t                  tx_reason;
  trm_modify_data                    tx_grm_modify_chain_state_data;
  trm_modify_chain_state_return_data tx_grm_modify_state_info;
} grm_tx_info_t;
#endif /*FEATURE_GSM_RX_TX_SPLIT*/


extern boolean gl1_grm_qta_gap_empty;

void gl1_hw_trm_qta_startup( geran_gl1_qta_msg_type* qta_data_p , gas_id_t gas_id);
void gl1_hw_trm_qta_cleanup( geran_gl1_qta_msg_type* qta_data_p , gas_id_t gas_id);



typedef struct
{
  /*Enhancement for band reg.*/
  /*A 2-D Buffer for storing the Transaction details of transaction Registered with TRM*/
  grm_sc_activity_state_info_t       grm_activity_buf[SC_MAX_ACTIVITIES][GRM_ACT_MAX];

  trm_freq_input_type                current_band_info; /*list of frequency bands currently registered with TRM*/
  trm_freq_input_type                irat_band_registered;
  trm_freq_input_type                irat_denied_bands; /*list of IRAT bands we can not tune the RF to*/
  trm_freq_input_type                grm_freq_info;

  trm_request_return_data            grm_request_data;

  trm_get_info_return_type           grm_get_info_data;

  trm_modify_data                    grm_modify_chain_state_data;
  trm_modify_chain_state_return_data grm_modify_state_info;

  trm_unlock_callback_data           grm_unlock_cb_data;

  grm_unlock_callback_t              grm_unlock_callback;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /*persistent denial: Holds all variables associated with DR persistent TRM denial monitoring*/
  grm_persistent_denial_t            persistent_denial;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GSM_RX_TX_SPLIT
  /*SUB Tx Information*/
  grm_tx_info_t                      *tx_info_p;
  grm_rx_tx_status_t                 rx_tx_status;
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  uint32                             when;
  uint32                             duration;
  uint32                             start_frame;
  uint32                             end_frame;
  uint32                             req_n_notify_duration;
  uint32                             frame_duration;
  uint32                             trm_grant_fn;
  uint32                             trm_grant_frames;

  trm_reason_enum_t                  reason;
  sys_proc_type_e_type               sub_reason;

  trm_reason_enum_t                  rsv_reason;
  sys_proc_type_e_type               rsv_sub_reason;
  trm_resource_enum_t                resource;
  trm_grant_return_enum_type         trm_grant_status;
  grm_client_enum_t                  prx_client; /*used for diversity clients only to tag associated PRX client*/

  gl1_trm_state_t                    trm_status;
  ps_access_mode_t                   ps_access_mode;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* Tuneaway state machine states*/
  gl1_drdsds_hybrid_tuneaway_state   hybrid_tuneaway_state;
  gl1_drdsds_drmode_rxd_priority     rxd_priority;

  /* SNR: Tuneaway Mode: On 4th PB*/
  dBx16_T                            dualRx_SNR;

  /*SNR of paging block decode. Used for persistent denial activation/deactivation*/
  dBx16_T                            Scell_SNR;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  /* RSSI: Calculate from MPH_DSC_THRESHOLD_IND*/
  dBx16_T                            rssi;

  uint8                              curr_bs_pa_mfrms;
  uint8                              dsc_threshold;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* Range 0-4, on 3rd, disbale DRDSDS, 4th Enable DRDSDS*/
  uint8                              paging_block_count;
  /*Range 0-3, consecutive bad pch blocks*/
  uint8                              bad_paging_block_count;
  uint8                              trm_denied_count;
  /*To store the number of TRM denials or bad blocks after which DR is to be disabled*/
  uint8                              trm_den_or_bad_page_cnt_set;
  /* TRUE: when request dual Rx in tuneaway Mode: 1 in 4 PB*/
  boolean                            dualRx_request;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  boolean                            dr_enabled;
  boolean                            wreconf;
  boolean                            trm_release_required;
  boolean                            trm_requested_at_warmup;
} grm_info_t;

typedef struct
{
  uint8 prxRequest    :1; //PRX Request
  uint8 txRequest     :1; //TX Request
  uint8 drxRequest    :1; //DRX Request
  uint8 unused        :5;
}grm_grouped_request_bitmap_t;

/*===========================================================================

           Function and Data Declarations

===========================================================================*/
extern trm_band_t grm_valid_gsm_sys_bands[][MAX_BAND_INFO_GRM];
extern uint16   grm_valid_gsm_sys_bands_cnt[];

extern trm_band_t grm_valid_gsm_sys_bands_divrx[][MAX_BAND_INFO_GRM];
extern uint16 grm_valid_gsm_sys_bands_cnt_divrx[];

/*Pending DR state transition */
extern grm_pending_dr_state pending_dr_state;


rfm_device_enum_type grm_get_max_valid_rf_device_for_client(
                                      grm_client_enum_t client_id );

void grm_clear_freq_info(grm_client_enum_t  client_id, 
                         gas_id_t           gas_id);

void grm_gl1_set_current_trm_reason(trm_reason_enum_t   reason,
                                    sys_proc_type_e_type subreason,
                                    grm_client_enum_t   client_id,
                                    gas_id_t            gas_id);

void grm_add_trm_band_to_freq_input_type(trm_band_t         band,
                                         grm_client_enum_t  client_id,
                                         gas_id_t           gas_id);

trm_reason_enum_t grm_gl1_get_current_trm_reason(grm_client_enum_t  client_id,
                                                 gas_id_t           gas_id);

sys_proc_type_e_type grm_gl1_get_trm_sub_reason(grm_client_enum_t  client_id,
                                                gas_id_t           gas_id);


boolean grm_gl1_get_dr_status(gas_id_t gas_id);

boolean grm_add_arfcn_to_trm_freq_input_type(ARFCN_T            gl1_freq_info,
                                             grm_client_enum_t  client_id,
                                             gas_id_t           gas_id);

sys_band_class_e_type grm_get_trm_band_from_g_pri (l1_fm_priority_T   pri,
                                                   gas_id_t           gas_id);

sys_band_class_e_type gl1_map_gsm_band_to_sys_band(sys_band_T    band);

sys_band_T gl1_map_sys_band_to_gsm_band(sys_band_class_e_type    band_e);

/*to be mainlined under enhancement CR*/
#ifdef FEATURE_PERSISTENT_DENIAL_DEBUG
void  grm_denied_arfcn_processing (ARFCN_T                              *arfcn, 
                                   uint32                               count, 
                                   grm_persistent_denial_sm_actions_t   action, 
                                   grm_client_enum_t                    client_id,
                                   gas_id_t                             gas_id);
#endif

boolean grm_persistent_denial_monitoring(grm_persistent_denial_sm_actions_t   action,
                                         grm_client_enum_t                    client_id,
                                         grm_pd_command_t                     request,
                                         gas_id_t                             gas_id);

grm_info_t *grm_get_info_ptr_from_client(grm_client_enum_t    client_id,
                                             gas_id_t             gas_id);
grm_info_t* get_grm_info_ptr(); 

gas_id_t gl1_map_client_id_to_gas_id(grm_client_enum_t  client_id);

grm_client_enum_t gl1_map_gas_id_to_client_id(gas_id_t  gas_id);
grm_client_enum_t gl1_get_current_grm_client_id(gas_id_t gas_id );

grm_client_enum_t gl1_map_gas_id_to_client_id_pchxfer(
                           const gas_id_t          gas_id);

rfm_device_enum_type grm_get_rf_device(grm_client_enum_t  client_id,
                                       gas_id_t           gas_id);

rfm_device_enum_type grm_get_other_g_rf_device(gas_id_t   gas_id);

rfm_device_enum_type grm_get_device_mapping(grm_client_enum_t   requested_client_id,
                                            trm_get_info_data   *input_data_p,
                                            gas_id_t            gas_id);

uint32 grm_get_rat_access_info( gas_id_t gas_id );

boolean grm_is_alt_sub_in_data( gas_id_t gas_id );

trm_grant_return_enum_type grm_request(grm_client_enum_t        client_id,
                                       trm_duration_t           duration,
                                       trm_reason_enum_t        reason,
                                       sys_proc_type_e_type     sub_reason,
                                       trm_resource_enum_t      resource,
                                       grm_divrx_request_type   req_type);

trm_grant_return_enum_type grm_handle_trm_request(grm_client_enum_t    client_id,
                                                  trm_duration_t       duration,
                                                  trm_reason_enum_t    reason,
                                                  sys_proc_type_e_type sub_reason,
                                                  trm_resource_enum_t  resource,
                                                  grm_divrx_request_type  req_type,
                                                  gas_id_t             gas_id);

trm_grant_return_enum_type grm_request_rx_sec(grm_client_enum_t    divrx_client_id,
                                              trm_duration_t       duration,
                                              trm_reason_enum_t    reason,
                                              sys_proc_type_e_type sub_reason,
                                              trm_resource_enum_t  resource);

gl1_trm_state_t grm_request_diversity_chain(grm_client_enum_t             divrx_client_id,
                                            trm_duration_t                duration,
                                            trm_grant_return_enum_type    prx_trm_grant_return, 
                                            grm_divrx_request_type        req_type,
                                            gas_id_t                      gas_id);

void grm_map_band_support_diversity_chain(grm_client_enum_t divrx_client_id,
                                              gas_id_t          gas_id);

boolean grm_validate_diversity_chain_request(trm_grant_return_enum_type  prx_trm_grant_return,
                                            grm_divrx_request_type        req_type,
                                            gas_id_t                      gas_id);

trm_grant_return_enum_type grm_trm_request_api(grm_client_enum_t      client_id,
                                               trm_duration_t         duration,
                                               trm_reason_enum_t      reason,
                                               sys_proc_type_e_type   subreason,
                                               trm_resource_enum_t    resource);

void grm_inform_grant_event(grm_client_enum_t       client_id,
                            trm_resource_enum_t     resource,
                            trm_duration_t          duration,
                            trm_reason_enum_t       reason,
                            trm_grant_event_enum_t  grant_event);

trm_grant_return_enum_type grm_get_granted(grm_client_enum_t          client_id,
                                           trm_get_info_return_type   *output_data_p,
                                           gas_id_t                   gas_id);

boolean grm_change_reason(grm_client_enum_t      client_id,
                          trm_reason_enum_t      new_reason,
                          sys_proc_type_e_type   sub_reason,
                          gas_id_t               gas_id);

boolean grm_change_priority(grm_client_enum_t      client_id,
                            trm_reason_enum_t      new_priority,
                            sys_proc_type_e_type   sub_reason,
                            gas_id_t               gas_id);

void grm_check_connected_reason(grm_client_enum_t  client_id,
                                grm_conn_type_t    connected_mode,
                                gas_id_t           gas_id);

void grm_modify_chain_state(grm_client_enum_t   client_id,
                            grm_tag_t           grm_tag,
                            gas_id_t            gas_id);

void grm_retain_lock(grm_client_enum_t      client_id,
                     grm_unlock_callback_t  unlock_callback,
                     gas_id_t               gas_id);

void grm_retain_chain_lock(grm_client_enum_t     client_id,
                           grm_unlock_callback_t unlock_callback,
                           gas_id_t              gas_id);

boolean gl1_drds_debug(gas_id_t gas_id );

void grm_init_valid_gsm_sys_bands(gas_id_t gas_id );

void grm_set_valid_acquisition_bands(grm_client_enum_t  client_id,
                                     gas_id_t           gas_id);

void grm_initialise(grm_client_enum_t   client_id,
                    gas_id_t            gas_id);

boolean grm_is_a_secondary_client(grm_client_enum_t client_id);

trm_reason_enum_t grm_get_reason(grm_client_enum_t requested_client_id,
                                 gas_id_t          gas_id );

#ifdef FEATURE_PBR_RANDOMIZATION
uint32 grm_get_wakeup_identifier( grm_client_enum_t requested_client_id,
                                  gas_id_t  gas_id );
#endif

trm_dual_receive_enum_type grm_get_dr_capability(grm_client_enum_t client_id,
                                                 gas_id_t          gas_id);

boolean grm_extend_duration(grm_client_enum_t client_id,
                            trm_duration_t    duration,
                            gas_id_t          gas_id);

boolean grm_change_duration(grm_client_enum_t client_id,
                            trm_duration_t    duration,
                            gas_id_t          gas_id);

boolean gl1_drdsds_enabled(gas_id_t gas_id );

void grm_send_band_release_if_required(grm_client_enum_t  client_id,
                                       gas_id_t           gas_id);

void grm_set_drx_cycle(uint32             paging_mframes, 
                       grm_client_enum_t  client_id,
                       gas_id_t           gas_id);

void grm_set_dr_enabled_status(grm_client_enum_t  client_id,
                               gas_id_t           gas_id);

void grm_exchange_lock(grm_client_enum_t dest_client_id,
                       grm_client_enum_t source_client_id,
                       gas_id_t          gas_id );

void call_in_task_grm_request_and_notify_enhanced(
                    grm_request_and_notify_enhanced_data_t *params_p );

void call_in_task_grm_release(grm_client_enum_t client_id);

void call_in_task_grm_release_in_callback(grm_client_enum_t client_id);


void call_in_task_grm_enable_disable_dual_receive(grm_client_enum_t client_id,
                                                         boolean dr_setting);


void call_in_task_grm_reserve_at(grm_reserve_at_data_t data);

void grm_abort_callback(trm_unlock_callback_data *abort_cb_data_p);
void grm_normal_acq_unlock(
              trm_unlock_callback_data *unlock_data_p );
#ifdef FEATURE_G2X_IDLE_TUNEAWAY
extern void grm_idle_abort_callback( trm_unlock_callback_data *unlock_data_p );
#endif /* FEATURE_G2X_IDLE_TUNEAWAY */

void grm_retain_lock_for_acquisition(grm_client_enum_t acq_client,
                                     grm_unlock_mode_t unlock_mode,
                                     trm_reason_enum_t curr_acq_reason,
                                     sys_proc_type_e_type subreason,
                                     gas_id_t gas_id );

boolean grm_is_embms_active(gas_id_t gas_id);
timetick_type grm_get_acquisition_duration(gas_id_t gas_id);

void grm_enh_grant_callback(grm_client_enum_t          granted_client,
                            trm_grant_return_enum_type grant_status,
                            trm_duration_t             duration );

uint32 gpl1_multi_sim_calculate_grm_reservation_time(uint32   requested_FN,
                                                     gas_id_t gas_id );

void grm_add_sys_band_type_to_trm_freq_input_type(trm_band_t            band,
                                                  grm_client_enum_t     client_id,
       gl1_sc_activities_t sc_activity,
       gas_id_t gas_id);

void grm_clear_stale_client_state_information(grm_client_enum_t             client_id,
                                              grm_sc_activity_state_info_t  *act_info_ptr,
                                              gas_id_t                      gas_id);

void grm_update_client_state_info( grm_sc_activity_state_info_t *activity_info,
                                  gl1_sc_activities_t           activity,
                                  grm_client_enum_t             client_id,
                                  gas_id_t                      gas_id);

boolean grm_enable_disable_dual_receive( boolean enable,
                                        grm_client_enum_t client_id,
                                         gas_id_t gas_id );

void grm_set_acquisition_priority(geran_priority_t acq_priority,
                                gas_id_t          gas_id);

geran_priority_t grm_get_acquisition_priority(gas_id_t gas_id);

trm_reason_enum_t grm_get_acquisition_reason(trm_reason_enum_t curr_acq_reason, gas_id_t gas_id);

void grm_set_connection_priority(const grm_conn_type_t conn_type,
                                 trm_reason_enum_t     conn_reason,
                                 gas_id_t              gas_id);

trm_reason_enum_t grm_get_connection_priority(const grm_conn_type_t conn_type,
                                              gas_id_t              gas_id);

void grm_reserve_at(grm_client_enum_t     client_id,
                    trm_time_t            when,
                    trm_duration_t        duration,
                    trm_reason_enum_t     reason,
                    sys_proc_type_e_type  sub_reason,
                    trm_resource_enum_t   resource,
                    boolean               rxdiv_chain );

void grm_reserve_at_enhanced(grm_client_enum_t   client_id,
                    trm_time_t            when,
                    trm_duration_t        duration,
                    trm_reason_enum_t     reason,
                    sys_proc_type_e_type  sub_reason,
                    uint32                required_frame,
                    uint32                frame_duration,
                    trm_resource_enum_t   resource,
                    l1_fm_priority_T      pri,
                    gas_id_t              gas_id);

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
boolean grm_reserve_at_ccch_in_trans(
                    grm_client_enum_t   client_id,
                    trm_time_t          when,
                    trm_duration_t      duration,
                    trm_reason_enum_t   reason,
                    uint32              required_frame,
                    uint32              frame_duration,
                    trm_resource_enum_t resource,
                    gas_id_t            gas_id);

boolean grm_release_pchxfer(grm_client_enum_t client_id, gas_id_t gas_id );
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

void grm_release(grm_client_enum_t client_id,
                    gas_id_t            gas_id );

boolean grm_release_client_only(grm_client_enum_t client_id, 
                                gas_id_t gas_id );
void grm_release_or_exchange(grm_client_enum_t   client_id,
                             gas_id_t            gas_id);

void grm_register_pbr_info( grm_client_enum_t client_id,
                                   uint32            drx_cycle );

void grm_register_aps_info(grm_client_enum_t  client_id, 
                           gas_id_t           gas_id);

grm_info_t * grm_get_cur_reservation(uint32 wakeup_fn, gas_id_t gas_id );

void grm_store_cur_reservation(grm_info_t *grm_info_p, gas_id_t gas_id );

void grm_reserve_at_gts(int32 wup_duration, 
                        uint32 wakeup_fn,
                        gas_id_t gas_id);

void grm_update_reserve_reason(l1_fm_priority_T pri, gas_id_t gas_id);
boolean gl1_check_trm_grm_reasons_match(gas_id_t gas_id);

void gpl1_multi_sim_setup_afc_tt(gas_id_t gas_id);

void grm_request_at_warmup(gas_id_t gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
/*===========================================================================
FUNCTION  GRM_REQUEST_DIVRX_AT_WARMUP

DESCRIPTION
  Send TRM request and notify to the TRM layer for the diveristy client. 
  This function is called from sleep warmup callback. The duration is set to
  10ms and diversity client is mapped correctly from primary client.

DEPENDENCIES
  This module should only be called during sleep wakeup in idle mode.

RETURN VALUE
  Void

SIDE EFFECTS
===========================================================================*/
void grm_request_divrx_at_warmup(gas_id_t gas_id);
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

boolean grm_request_and_notify(
              grm_client_enum_t    client_id,
              trm_resource_enum_t  resource,
              trm_duration_t       duration,
              trm_reason_enum_t    reason,
              grm_divrx_request_type    req_type,
              sys_proc_type_e_type sub_reason,
              grm_tag_t            grm_tag );

void grm_force_request_and_notify(
              grm_client_enum_t    client_id,
              trm_resource_enum_t  resource,
              trm_duration_t       duration,
              trm_reason_enum_t    reason,
              sys_proc_type_e_type sub_reason,
              grm_tag_t            grm_tag );

boolean grm_request_and_notify_enhanced(
              grm_client_enum_t    client_id,
              trm_resource_enum_t  resource,
              trm_duration_t       duration,
              trm_reason_enum_t    reason,
              sys_proc_type_e_type sub_reason,
              grm_tag_t            grm_tag );

void gl1_msim_reset_reserve_at_count(gas_id_t gas_id );

#ifdef FEATURE_GSM_WTR_HOP
void grm_send_wtr_hop_async_rsp(boolean  wtr_hop_success,
                                gas_id_t gas_id );
#endif /* FEATURE_GSM_WTR_HOP */

void grm_set_denied_band_in_next_mon_scan_flag(sys_band_T         band, 
                                               grm_client_enum_t  client_id,
                                   gas_id_t    gas_id );

void grm_initialise_pm_denial_count(grm_client_enum_t client_id,
                           gas_id_t gas_id);

void grm_clear_registered_bands(grm_client_enum_t client_id,
                                gas_id_t          gas_id);

boolean grm_is_denied_band(sys_band_T         band, 
                           grm_client_enum_t  client_id,
                           gas_id_t gas_id);

boolean grm_is_band_registered(sys_band_T         band,
                               grm_client_enum_t  client_id,
                           gas_id_t gas_id);

void grm_set_device_configuration( gas_id_t gas_id );

boolean grm_device_config_is_drdsds( void );

trm_set_irat_band_device_info grm_get_irat_rf_dev_from_grm_info(
                                        sys_band_class_e_type   bandin,
                                        grm_client_enum_t       client_id,
                                        gas_id_t                gas_id);

void grm_async_cb_event_handler(
             trm_async_callback_information_type *grm_async_cb_event_data_p );

/* TRM band registration handling related Functions*/
void grm_transaction_manager(grm_tm_event_t       event, 
                             gl1_sc_activities_t  act, 
                             grm_client_enum_t    client_id,
                             gas_id_t             gas_id);

void grm_tick_transaction_manager(grm_client_enum_t    client_id,
                                  gas_id_t             gas_id);

void grm_activity_queue_init(grm_client_enum_t    client_id,
                             gas_id_t             gas_id);

int8 grm_find_next_indx_in_act_buf(gl1_sc_activities_t  act,
                                   grm_client_enum_t    client_id,
                                   gas_id_t             gas_id);

void grm_transaction_release(gl1_sc_activities_t            activity,
                             grm_sc_activity_state_info_t   *act_info_ptr,
                             grm_client_enum_t              client_id,
                             gas_id_t                       gas_id);

/*Function To perform Band registration for IFS Activities*/
boolean grm_check_ifs_band_reg(gl1_sc_activities_t sc_activity,
                               sys_band_T sc_band,
                               uint32 frames_required,
                               boolean div_needed,
                               gas_id_t gas_id);

/*Function to perform Band registration for IRAT Activities*/
boolean grm_check_irat_band_reg (grm_client_enum_t      meas_client,
                                 sys_band_class_e_type trm_band,
                                 uint32 frames_required,
                                 boolean div_needed,
                                 gas_id_t gas_id);

/* add additional band registrations specifically for G2L fast search */
boolean grm_register_additional_irat_bands( uint32 lte_band, uint32 num_frames, gas_id_t gas_id);

/* Function to hold and release TRM resource lock as requested by RR */
void grm_set_resource_lock_state( boolean state, gas_id_t gas_id );
boolean grm_get_resource_lock_state(gas_id_t gas_id);
void grm_set_resource_lock_enum( grm_resource_lock_t state, gas_id_t gas_id );
grm_resource_lock_t grm_get_resource_lock_enum(gas_id_t gas_id);

boolean grm_check_valid_freq_info( grm_info_t *grm_info_p,
                                   gas_id_t    gas_id );

trm_grant_return_enum_type grm_get_trm_grant_status(grm_client_enum_t client_id,
                                                    gas_id_t gas_id);

void grm_set_trm_grant_status(trm_grant_return_enum_type trm_grant_status,
                              grm_client_enum_t           client_id,
                              gas_id_t gas_id);

boolean grm_get_trm_release_required(grm_client_enum_t  grm_client_id,
                                     gas_id_t gas_id);

void grm_set_trm_release_required(boolean release_required,
                                  grm_client_enum_t   client_id,
                                  gas_id_t gas_id);

boolean grm_get_trm_requested_at_warmup(grm_client_enum_t client_id,
                                        gas_id_t gas_id);

void grm_set_trm_requested_at_warmup(boolean trm_request,
                                     grm_client_enum_t  client_id,
                                     gas_id_t gas_id);

uint32 grm_get_trm_grant_fn(grm_client_enum_t   client_id,
                            gas_id_t gas_id);

void grm_set_trm_grant_fn(uint32 trm_grant_fn,
                          grm_client_enum_t   client_id,
                          gas_id_t gas_id);

uint32 grm_get_trm_grant_frames(grm_client_enum_t   client_id,
                                gas_id_t gas_id);

void grm_set_trm_grant_frames(uint32 trm_grant_frames,
                              grm_client_enum_t   client_id,
                              gas_id_t gas_id);

uint32 grm_get_frame_duration(grm_client_enum_t   client_id,
                            gas_id_t gas_id);

void grm_set_frame_duration(uint32 trm_reserve_fn,
                            grm_client_enum_t   client_id,
                            gas_id_t gas_id);

uint32 grm_get_post_decrement_frame_duration(grm_client_enum_t  client_id,
                                             gas_id_t gas_id);

void grm_set_trm_status(gl1_trm_state_t trm_status,
                        grm_client_enum_t   client_id,
                        gas_id_t gas_id);

gl1_trm_state_t grm_get_trm_status(grm_client_enum_t  client_id,
                                              gas_id_t gas_id);

void grm_update_aps_support(const boolean  aps_state,
                                gas_id_t gas_id);

void grm_rx_diversity_unlock_cb( trm_unlock_callback_data *unlock_data_p );

boolean grm_release_rx_sec(grm_client_enum_t client_id, gas_id_t gas_id);
trm_client_enum_t grm_map_grm_client_id_to_trm_client_id(grm_client_enum_t grm_client_id);
grm_client_enum_t grm_map_trm_client_id_to_grm_client_id(trm_client_enum_t trm_client_id);
grm_client_enum_t grm_map_prx_to_divrx_client_id(grm_client_enum_t prx_client_id, gas_id_t gas_id);

void grm_set_ps_access_mode(ps_access_mode_t    ps_access_mode,
                            grm_client_enum_t   client_id,
                            gas_id_t            gas_id);

ps_access_mode_t grm_get_ps_access_mode(grm_client_enum_t  client_id,
                                        gas_id_t           gas_id);

void grm_set_req_and_notify_duration(uint32              duration,
                                     grm_client_enum_t   client_id,
                                     gas_id_t            gas_id);

uint32 grm_get_req_and_notify_duration(grm_client_enum_t  client_id,
                                       gas_id_t           gas_id);

void grm_set_modify_state_info(trm_modify_chain_state_return_data   *modify_state_info,
                               grm_client_enum_t                    client_id,
                               gas_id_t                             gas_id);

trm_modify_chain_state_return_data *grm_get_modify_state_info(grm_client_enum_t  client_id,
                                                              gas_id_t           gas_id);

void grm_set_modify_chain_state_data(trm_modify_data    *modify_data,
                                     grm_client_enum_t  client_id,
                                     gas_id_t           gas_id);

trm_modify_data *grm_get_modify_chain_state_data(grm_client_enum_t  client_id,
                                                 gas_id_t           gas_id);


#ifdef FEATURE_GSM_TDS
extern trm_band_t tdrm_get_band( uint16 freq );
#endif /* FEATURE_GSM_TDS */

#ifdef FEATURE_GSM_RX_TX_SPLIT
boolean grm_is_a_tx_client(grm_client_enum_t client_id);

void grm_configure_tx_info(grm_client_enum_t  client_id,
                           gas_id_t           gas_id);

grm_client_enum_t grm_map_rx_to_tx_client_id(grm_client_enum_t  rx_client_id, 
                                             gas_id_t           gas_id);

grm_client_enum_t grm_map_tx_to_rx_client_id(grm_client_enum_t tx_client_id,
                                                 gas_id_t gas_id);

void grm_release_tx_chain(grm_client_enum_t client_id, gas_id_t gas_id);

gl1_trm_state_t grm_request_tx_chain(grm_client_enum_t    client_id,
                                     trm_duration_t       duration,
                                     trm_reason_enum_t    reason,
                                     sys_proc_type_e_type sub_reason,
                                     trm_resource_enum_t  resource,
                                     gas_id_t             gas_id);

grm_rx_tx_status_t grm_get_rx_tx_status(grm_client_enum_t client_id,
                                        gas_id_t          gas_id);

void grm_set_rx_tx_status(grm_rx_tx_status_t          status,
                          grm_client_enum_t           client_id,
                          gas_id_t                    gas_id);

void grm_async_grouped_cb_event_handler(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p );

boolean grm_is_partial_grant_or_unlock(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p );

boolean grm_validate_request_and_notify_cb(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p );

boolean grm_validate_request_and_notify_enh_cb(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p );

boolean grm_validate_modify_chain_cb(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p );

boolean grm_validate_unlock_chain_cb(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p );

void grm_handle_tx_grant_callback(grm_client_enum_t          client,
                                  trm_grant_return_enum_type grant_status,
                                  gas_id_t                   gas_id );

boolean grm_is_tx_reason(trm_reason_enum_t reason);

trm_grant_return_enum_type grm_grouped_trm_request(grm_client_enum_t    client_id,
                                                   trm_duration_t       duration,
                                                   trm_reason_enum_t    reason,
                                                   sys_proc_type_e_type sub_reason,
                                                   trm_resource_enum_t  resource,
                                                   grm_divrx_request_type       req_type,
                                                   grm_grouped_request_bitmap_t req_bitmap,
                                                   gas_id_t             gas_id);

void grm_grouped_request_and_notify(grm_client_enum_t    client_id,
                                    trm_duration_t       duration,
                                    trm_reason_enum_t    reason,
                                    sys_proc_type_e_type sub_reason,
                                    grm_divrx_request_type  req_type,
                                    grm_tag_t            grm_tag,
                                    gas_id_t             gas_id);

void grm_grouped_enhanced_request_and_notify(grm_client_enum_t    client_id,
                                             trm_resource_enum_t  resource,
                                             trm_duration_t       duration,
                                             trm_reason_enum_t    reason,
                                             sys_proc_type_e_type sub_reason,
                                             grm_tag_t            grm_tag,
                                             gas_id_t             gas_id);

void grm_grouped_retain_chain_lock(grm_client_enum_t     client_id,
                                   grm_unlock_callback_t unlock_callback,
                                   grm_grouped_request_bitmap_t   req_bitmap,
                                   gas_id_t              gas_id);

void grm_grouped_modify_chain_state(grm_client_enum_t client_id,
                                    grm_tag_t         grm_tag,
                                    gas_id_t          gas_id );

#endif /*FEATURE_GSM_RX_TX_SPLIT*/

#ifdef FEATURE_GSM_QSC_TX_DIV
grm_client_enum_t check_asdiv_grm_client(grm_client_enum_t grm_client);
#endif /*FEATURE_GSM_QSC_TX_DIV*/

boolean gpl1_get_grm_qta_state(gas_id_t gas_id);

/*===========================================================================

FUNCTION  grm_check_device_supported

DESCRIPTION
  Check if the allocated RF device supports the band.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_check_device_supported (
                                   grm_client_enum_t    requested_client_id,
                                   trm_band_t           requested_band,
                                   rfm_device_enum_type allocated_device,
                                   gas_id_t             gas_id );

void call_in_task_grm_request(grm_request_data_t *params_p);

#if defined ( FEATURE_DUAL_SIM ) && defined ( FEATURE_CRAT_MSIM_TEST_FRAMEWORK ) 

void grm_els_state_registration(
   grm_client_enum_t grm_client_id,
   gas_id_t          gas_id);

void grm_els_state_update_aperiodic(
   trm_reason_enum_t reason,
   sys_proc_type_e_type sub_reason,
   trm_duration_t            duration,
   grm_client_enum_t grm_client_id,
   gas_id_t          gas_id);

void    grm_els_state_update(
   trm_time_t            when,
   trm_reason_enum_t reason,
   sys_proc_type_e_type sub_reason,
   trm_duration_t            duration,
   grm_client_enum_t grm_client_id,
   gas_id_t          gas_id); 

uint32  grm_get_drx_cycle(grm_client_enum_t  client_id,  gas_id_t gas_id);

void grm_els_state_update_G2X_resel ( gas_id_t gas_id );
void grm_clear_els_active ( void );

#endif /* FEATURE_DUAL_SIM | FEATURE_CRAT_MSIM_TEST_FRAMEWORK */

#ifdef FEATURE_DUAL_DATA
/*===========================================================================

FUNCTION grm_set_data_sub_priority

DESCRIPTION
  Sets specific Subscription to assigned priority as sets other subscriptions
  to GERAN_MMS_PRIORITY_NOMINAL

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void grm_set_data_sub_priority(geran_priority_t priority,gas_id_t gas_id);

/*===========================================================================

FUNCTION grm_get_data_sub_priority

DESCRIPTION
  Returns the priority of subscription

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
geran_priority_t grm_get_data_sub_priority(gas_id_t gas_id);

/*===========================================================================

FUNCTION grm_get_low_priority_data_sub

DESCRIPTION
  Returns the priority of low priority data subscription if one sub is found
  with GERAN_MMS_PRIORITY_MAXIMAL.
 
  If no sub is found with GERAN_MMS_PRIORITY_MAXIMAL priority then will
  return GERAN_ACCESS_STRATUM_ID_UNDEFINED

PARAMS
  None

DEPENDENCIES

RETURN VALUE
 low_priority_data_sub
===========================================================================*/
gas_id_t grm_get_low_priority_data_sub(void);


#endif /* FEATURE_DUAL_DATA */

void grm_reset_trm_reason_stat_unlck_and_others(grm_client_enum_t client_id, gas_id_t gas_id);
void grm_grouped_trm_release(grm_client_enum_t client_id, grm_grouped_request_bitmap_t bitmap, gas_id_t gas_id);

void grm_grouped_trm_reserve(
                             grm_client_enum_t  client_id,
                             grm_grouped_request_bitmap_t bitmap,
                             trm_time_t            when,
                             trm_duration_t        duration,
                             trm_reason_enum_t     reason,
                             sys_proc_type_e_type  sub_reason,
                             trm_resource_enum_t   resource, 
                             gas_id_t gas_id);

/*===========================================================================

FUNCTION     grm_validate_tx_chain_grant_status

DESCRIPTION  force an err_fatal if GSM holds Tx chain without Rx chain

RETURN VALUE None

===========================================================================*/
void grm_validate_tx_chain_grant_status(grm_client_enum_t client_id, gas_id_t gas_id);
#endif /* GPL1_GRM_INTF_H */
